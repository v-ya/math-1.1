#include "../main.h"

func(echo)
{
	static char *label=".echo";
	static u32 type_1[1]={type_string};
	var *vp;
	ret->type=type_void;
	ret->v.v_long=0;
	vp=argv->v;
	if _oF(argc<1) goto Err;
	else if _oF(check_varlist(argv,1,type_1)) goto Err;
	argv=argv->r;
	if _oF(vp->v.v_string)
	{
		vp=sbuf_sprintf(vp->v.v_string,argv);
		if _oF(vp->type&type_err) return vp;
		print("%s",vp->v.vp_byte);
	}
	else goto Err;
	return ret;
	Err:
	return get_error(errid_FunArgvType,label);
}


func(length)
{
	static char *label=".length";
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	ret->type=type_long;
	ret->v.v_long=argv->v->length;
	return ret;
}

func(this)
{
	static char *label=".this";
	static u32 type_1[1]={type_void};
	var *vp;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	vp=ptvar_get(_pt_this);
	if _oF(!vp) return get_error(errid_SysPtvarNotfind,label);
	var_save(vp);
	return vp;
}

func(exist)
{
	static char *label="[object].exist";
	static u32 type_1[1]={type_string};
	static u32 type_2[2]={type_string,type_num};
	var *root,*vp;
	char *path;
	u32 mask;
	switch(argc)
	{
		case 1:
			if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			mask=type_all;
			break;
		case 2:
			if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			vp=argv->r->v;
			mask=vpntof(vp);
			break;
		default:
			return get_error(errid_FunArgvType,label);
	}
	root=ret;
	if (path)
	{
		root=ptvar_replace(_pt_this,root);
		vp=get_var(path,NULL,NULL);
		root=ptvar_replace(_pt_this,root);
		ret->type=type_long;
		if (vp->type&type_err)
		{
			clr_error();
			ret->v.v_long=0;
		}
		else
		{
			ret->v.v_long=(vp->type&mask)?1:0;
			var_free(vp);
		}
	}
	return ret;
}

func(get_type)
{
	static char *label=".get_type";
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	ret->type=type_long;
	ret->v.v_long=argv->v->type;
	return ret;
}

func(import)
{
	static char *label="[object].import";
	static u32 type_2[2]={type_string,type_string};
	var *vp;
	vlist *vl;
	char *name,*path,*fullname=NULL;
	if _oF(argc!=2) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
	// 判断 ret 是否可写
	if _oF(!(ret->mode&auth_write)) goto Err_write;
	// 空 vlist 不可写
	if _oF(!ret->v.v_void) goto Err_write;
	name=argv->v->v.v_string;
	argv=argv->r;
	// 检查 name
	if _oF(!vname_check(name)) goto Err_name;
	if _oF(var_find(_vm_gobj,name)) goto Err_name;
	if _oF(var_find(ret,name)) goto Err_samename;
	// 获取 path
	path=argv->v->v.v_string;
	if _oF(_path_import->v.v_string&&path&&path[0]!=_path_incutup->v.v_byte)
	{
		path=get_path(_path_import->v.v_string,path);
		vp=package_import(path,&fullname);
		free(path);
	}
	else vp=package_import(path,&fullname);
	if _oF(vp->type&type_err) goto Err;
	// 产生资源链接变量
	vl=vlist_alloc(name);
	if _oF(!vl) goto Err_mem;
		// 这里不赋回原变量，因为赋回后也不会写入实际变量，况且 ret 一定是 object 且有子变量
	if _oF(ret->type&type_vlist) vlist_insert(ret->v.v_vlist,vl);
	else vmat_insert(ret->v.v_vmat,vl);
	vlist_link(vl,vp);
	// 生成返回值
	vp=var_alloc(tlog_string,leng_no);
	if _oF(!vp) goto Err_mem;
	vp->v.v_string=fullname;
	vp->mode=auth_read|free_pointer;
	fullname=NULL;
	Err:
	if _oF(fullname) free(fullname);
	return vp;
	Err_write:
	vp=get_error(errid_VarNotWrite,label);
	goto Err;
	Err_name:
	vp=get_error(errid_GraQuoVarname,label);
	goto Err;
	Err_samename:
	vp=get_error(errid_VarIsExist,label);
	goto Err;
	Err_mem:
	vp=get_error(errid_MemLess,label);
	goto Err;
}

func(remove)
{
	static char *label=".remove";
	static u32 type_1[1]={type_string};
	var *vp;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_void;
	ret->v.v_long=0;
	vp=package_remove(argv->v->v.v_string);
	if _oF(vp) return vp;
	else return ret;
}

func(issame)
{
	static char *label=".issame";
	var *vp;
	if _oF(argc<2) return get_error(errid_FunArgvType,label);
	ret->type=type_long;
	vp=argv->v;
	for(argv=argv->r;argv&&(vp==argv->v);argv=argv->r) ;
	ret->v.v_long=argv?0:1;
	return ret;
}


