#include "main.h"

// 注：过程中应该加锁，包管理锁

var* package_import(char *path, char **name)
{
	static char *label="package_import";
	void *hdl=NULL,**interface;
	var *vp,*vp2;
	vlist *vl;
	u32 check;
	// 预定义符号
	u32 (*package_ckeck)(u32);
	var* (*package_init)(void *, void **);
	void (*package_name)(char **, char **);
	char *author=NULL;
	char *package=NULL;
	// 链接
	if _oF(!path) goto Err_path;
	dlerror();
	hdl=dlopen(path,RTLD_LAZY);
	if _oF(!hdl) goto Err_path;
	// 获取预定义符号
	package_ckeck=dlsym(hdl,"package_ckeck");
	package_init=dlsym(hdl,"package_init");
	package_name=dlsym(hdl,"package_name");
	if _oF(dlerror()) goto Err_sym;
	// 验证
	check=time(NULL);
	if _oF((check^IMPORT_CHECK)!=package_ckeck(check)) goto Err_check;
	package_name(&author,&package);
	if _oF(author==NULL||package==NULL) goto Err_check;
	if _oF(vname_check(author)==0||vname_check(package)==0) goto Err_check;
	// 生成索引
	vp=var_find(_package,author);
	if _oF(!vp)
	{
		vl=var_insert(_package,author,tlog_vlist,leng_no);
		if _oF(!vl) goto Err_mem;
		vp=vl->v;
		vp->mode&=~auth_normal|auth_read;
	}
	if _oF(var_find(vp,package)) goto Err_pname;
	vl=var_insert(vp,package,tlog_vlist,leng_no);
	if _oF(!vl) goto Err_mem;
	vp=vl->v;
	vp->mode&=~auth_normal|auth_read;
	vl=var_insert(vp,"handle",tlog_void,leng_no);
	if _oF(!vl) goto Err_mem;
	vl->v->v.v_void=hdl;
	vl->v->mode&=~auth_normal|auth_read;
	vl=var_insert(vp,"interface",tlog_void,leng_no);
	if _oF(!vl) goto Err_mem;
	interface=&(vl->v->v.v_void);
	vl->v->mode&=~auth_normal|auth_read;
	vl=vlist_alloc("root");
	if _oF(!vl) goto Err_mem;
	vp->v.v_vlist=vlist_insert(vp->v.v_vlist,vl);
	vp=package_init(&BaseFunction,interface);
	if _oF(!vp) goto Err_base;
	if _oF(vp->type&type_spe) goto Err_init;
	vlist_link(vl,vp);
	vl->mode&=~free_pointer;
	// 生成全名
	if _oT(name)
	{
		*name=malloc(strlen(author)+strlen(package)+2);
		if _oF(!*name) goto Err_mem;
		sprintf(*name,"%s.%s",author,package);
	}
	return vp;
	Err_path:
	if _oF(path)
	{
		path=(char*) dlerror();
		if _oT(path)
		{
			vp=var_alloc(tlog_string,leng_no);
			if (!vp) goto Err_mem;
			vp->v.v_string=path;
			vp->mode=auth_read;
			get_error(errid_IntError,label);
			set_error(vp,vp->v.v_string);
		}
	}
	vp=get_error(errid_SysDlpathNotopen,label);
	goto Err;
	Err_sym:
	vp=get_error(errid_SysDlsymNotfind,label);
	goto Err;
	Err_mem:
	vp=get_error(errid_MemLess,label);
	goto Err;
	Err_check:
	vp=get_error(errid_SysPackageCheck,label);
	goto Err;
	Err_pname:
	vp=get_error(errid_SysPackageName,label);
	goto Err;
	Err_base:
	vp=get_error(errid_SysPackageBase,label);
	goto Err;
	Err_init:
	vp=get_error(errid_SysPackageInit,label);
	goto Err;
	Err:
	if _oT(name) *name=NULL;
	if _oF(author&&package&&(vp2=var_find(_package,author)))
	{
		var_delete(vp2,package);
		if _oF(!vp2->v.v_vlist) var_delete(_package,author);
	}
	if _oT(hdl) dlclose(hdl);
	return vp;
}

var* package_remove(char *name)
{
	static char *label="package_remove";
	char *buffer=NULL,*author,*package;
	var *va,*vp,*vr,*ret=NULL;
	void *hdl;
	u32 i;
	// 预定义符号
	var* (*package_uini)(void);
	// 获取 author, package
	if _oF(!name) goto Err_notfind;
	i=strlen(name)+1;
	buffer=malloc(i);
	if _oF(!buffer) goto Err_mem;
	memcpy(buffer,name,i);
	i=0;
	author=buffer;
	while(buffer[i]&&buffer[i]!='.') i++;
	if _oT(buffer[i]=='.') buffer[i++]=0;
	package=buffer+i;
	while(buffer[i]&&buffer[i]!='.') i++;
	buffer[i]=0;
	// get va, vp
	va=var_find(_package,author);
	if _oF(!va) goto Err_notfind;
	if _oF(va->inode>1) goto Err_takeup;
	vp=var_find(va,package);
	if _oF(!vp) goto Err_notfind;
	if _oF(vp->inode>1) goto Err_takeup;
	// check root && integer
	vr=var_find(vp,"root");
	if _oF(!vr) goto Err_damage;
	if _oF(refer_get((u64)vr))
	{
		refer_free((u64)vr);
		if _oT(vr->inode) (vr->inode)--;
	}
	if _oF(vr->inode>1) goto Err_takeup;
	vr=var_find(vp,"interface");
	if _oF(!vr) goto Err_damage;
	if _oF(vr->inode>1) goto Err_takeup;
	// get hdl
	vr=var_find(vp,"handle");
	if _oF(!vr) goto Err_damage;
	hdl=vr->v.v_void;
	// call package_uini && close hdl
	if _oT(hdl)
	{
		dlerror();
		package_uini=dlsym(hdl,"package_uini");
		if _oF(dlerror()) goto Err_sym;
		ret=package_uini();
		if _oF(ret) goto End;
		dlclose(hdl);
	}
	// free
	var_delete(va,package);
	if _oF(!va->v.v_vlist) var_delete(_package,author);
	End:
	if _oT(buffer) free(buffer);
	return ret;
	Err_mem:
	ret=get_error(errid_MemLess,label);
	goto End;
	Err_notfind:
	ret=get_error(errid_SysPackageNotfind,label);
	goto End;
	Err_takeup:
	ret=get_error(errid_SysPackageTakeup,label);
	goto End;
	Err_damage:
	ret=get_error(errid_SysPackageDamage,label);
	goto End;
	Err_sym:
	ret=get_error(errid_SysDlsymNotfind,label);
	goto End;
}


