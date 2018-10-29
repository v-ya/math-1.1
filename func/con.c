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
			mask=type_all^type_null;
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
	root=ret->v.v_var;
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
	var *vp,*root;
	vlist *vl;
	char *name,*path,*fullname=NULL;
	if _oF(argc!=2) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
	root=ret->v.v_var;
	if _oF(root->mode&auth_system) return get_error(errid_VarIsSystem,label);
	// 判断 root 是否可写
	if _oF(!(root->mode&auth_write)) goto Err_write;
	// 空 vlist 不可写
	if _oF(!root->v.v_void) goto Err_write;
	name=argv->v->v.v_string;
	argv=argv->r;
	// 检查 name
	if _oF(!vname_check(name)) goto Err_name;
	if _oF(var_find(_vm_gobj,name)) goto Err_name;
	if _oF(var_find(root,name)) goto Err_samename;
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
	if _oF(root->type&type_vlist) root->v.v_vlist=vlist_insert(root->v.v_vlist,vl);
	else vmat_insert(root->v.v_vmat,vl);
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

var *_func__add(char *label, var *root, var *index, var *vt, var *argv, u32 isreplace)
{
	u32 tlog,length,u=0,type,size;
	var *vp;
	vlist *vl;
	void *array;
	
	// check auth_system
	if _oF(root->mode&auth_system) return get_error(errid_VarIsSystem,label);
	// get type
	if _oT(vt->type&type_string)
	{
		if _oT(vt->v.v_string)
		{
			tlog=get_tlog(vt->v.v_string);
			if (*(vt->v.v_string)=='u' && tlog<tlog_max) u=1;
		}
		else tlog=tlog_max;
	}
	else
	{
		tlog=vt->v.v_int&type_all;
		for(length=0;tlog;tlog>>=1) length++;
		tlog=length?length-1:tlog_max;
		if _oF(vt->v.v_int&type_unsign && tlog>=tlog_byte && tlog <=tlog_long) u=1;
	}
	if _oF(tlog>=tlog_max) goto Err_nottype;
	type=1<<tlog|(u?type_unsign:0);
	// get length
	length=0;
	if _oF(!(tlog==tlog_refer) && argv)
	{
		if _oT(argv->type&type_znum && argv->length==leng_no) length=argv->v.v_long;
		else return get_error(errid_FunArgvType,label);
		if _oF(length&&(tlog==tlog_vlist||tlog==tlog_vmat)) goto Err_nottype;
	}
	// check length
	if _oF(length>_lim_array_max->v.v_long) goto Err_notlength;
	// check var
	if _oT(index->type&type_string) vl=v_find(root,index->v.v_string);
	else vl=v_find_index(root,index->v.v_long);
	if _oF(vl)
	{
		if _oT(!isreplace)
		{
			vp=vl->v;
			if _oT(vp) goto End;
			vp=_var_null;
			goto End;
		}
		// check var's auth
		vp=vl->v;
		if _oT(vp)
		{
			if _oF(!(vp->mode&auth_retype))
			{
				if _oT(vp->type^(1<<tlog)^(u?type_unsign:0)) goto Err_notretype;
				else if _oF(!(vp->mode&auth_relength)) goto Err_notrelength;
			}
			if _oF((vp->type==type) && (vp->mode&auth_relength) && length && vp->length)
			{
				if _oF(vp->length==length) goto End;
				switch(tlog)
				{
					case tlog_int:
						size=sizeof(s32);
						break;
					case tlog_long:
						size=sizeof(s64);
						break;
					case tlog_float:
						size=sizeof(double);
						break;
					case tlog_byte:
						size=sizeof(s8);
						break;
					case tlog_word:
						size=sizeof(s16);
						break;
					default:
						size=sizeof(void *);
						break;
				}
				array=malloc(length*size);
				memcpy(array,vp->v.v_void,size*(length>vp->length?vp->length:length));
				if _oF(length>vp->length)
				{
					if _oF(tlog==tlog_float)
					{
						for(u=vp->length;u<length;u++)
							((double*)array)[u]=(double)0;
					}
					else memset(array+(vp->length*size),0,(length-vp->length)*size);
				}
				if _oT(vp->mode&free_pointer)
				{
					if _oF(tlog==tlog_string)
					{
						for(u=length;u<vp->length;u++)
							free((vp->v.vp_string)[u]);
						free(vp->v.v_void);
					}
					vp->v.v_void=array;
					vp->length=length;
				}
				goto End;
			}
			if _oF(!(vp->mode&auth_retype)) goto Err_notretype;
		}
		vp=var_alloc(tlog,length);
		if _oF(!vp) goto Err_malloc;
		if _oT(vl->v) vp->mode=(vp->mode&~auth_all)|(vl->v->mode&auth_all);
		vlist_link(vl,vp);
		goto End;
	}
	// check name
	if _oF(index->type&type_string && var_find(_vm_gobj,index->v.v_string)) goto Err_notname;
	// check root's auth
	if _oF(!(root->mode&auth_write)) goto Err_nowrite;
	// alloc vl
	if _oT(index->type&type_string) vl=vlist_alloc(index->v.v_string);
	else vl=vlist_alloc_index(index->v.v_long);
	if _oF(!vl) goto Err_malloc;
	// insert vl
	if _oT(root->type&type_vlist) root->v.v_vlist=vlist_insert(root->v.v_vlist,vl);
	else vmat_insert(root->v.v_vmat,vl);
	// alloc var
	vp=var_alloc(tlog,length);
	if _oF(!vp) goto Err_malloc;
	// link
	vlist_link(vl,vp);
	End:
	if _oF(vp->type==type_refer)
	{
		if _oT(argv)
		{
			if _oT(argv->mode&auth_read)
			{
				if _oF(argv->type&type_refer) goto Err_refer;
				refer_set(vp,argv);
				vp=refer_check(vp);
				if _oF(!vp) goto Err_refer;
			}
			else goto Err_notlink;
		}
		else vp=_var_null;
	}
	var_save(vp);
	Err:
	return vp;
	Err_nottype:
	vp=get_error(errid_VarUnknowType,label);
	goto End;
	Err_notname:
	vp=get_error(errid_GraQuoVarname,label);
	goto End;
	Err_malloc:
	vp=get_error(errid_MemLess,label);
	goto End;
	Err_notlength:
	vp=get_error(errid_ReqOver,label);
	goto End;
	Err_notretype:
	vp=get_error(errid_VarNotRetype,label);
	goto End;
	Err_nowrite:
	vp=get_error(errid_VarNotWrite,label);
	goto End;
	Err_notrelength:
	if _oF(!vp->length || !length) goto Err_notretype;
	vp=get_error(errid_VarNotRelength,label);
	goto End;
	Err_refer:
	vp=get_error(errid_VarReferFail,label);
	goto End;
	Err_notlink:
	vp=get_error(errid_VarNotLink,label);
	goto End;
}

func(add)
{
	static char *label="[object].add";
	static u32 type_2[2]={type_string|type_znum,type_string|type_znum};
	static u32 type_3[3]={type_string|type_znum,type_string|type_znum,type_all|type_allowarray};
	var *index,*type;
	
	if _oF(argc==2 && !check_varlist(argv,2,type_2)) ;
	else if _oF(argc==3 && !check_varlist(argv,3,type_3)) ;
	else return get_error(errid_FunArgvType,label);
	// get argv
	index=argv->v;
	argv=argv->r;
	type=argv->v;
	argv=argv->r;
	if _oF(argv) return _func__add(label,ret->v.v_var,index,type,argv->v,0);
	else return _func__add(label,ret->v.v_var,index,type,NULL,0);
}

func(replace)
{
	static char *label="[object].replace";
	static u32 type_2[2]={type_string|type_znum,type_string|type_znum};
	static u32 type_3[3]={type_string|type_znum,type_string|type_znum,type_all|type_allowarray};
	var *index,*type;
	
	if _oF(argc==2 && !check_varlist(argv,2,type_2)) ;
	else if _oF(argc==3 && !check_varlist(argv,3,type_3)) ;
	else return get_error(errid_FunArgvType,label);
	// get argv
	index=argv->v;
	argv=argv->r;
	type=argv->v;
	argv=argv->r;
	if _oF(argv) return _func__add(label,ret->v.v_var,index,type,argv->v,1);
	else return _func__add(label,ret->v.v_var,index,type,NULL,1);
}

func(sub)
{
	static char *label="[object].sub";
	static u32 type_1[1]={type_string|type_znum};
	var *vp,*index,*root;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	
	index=argv->v;
	root=ret->v.v_var;
	if _oF(root->mode&auth_system) return get_error(errid_VarIsSystem,label);
	if _oT(index->type&type_string) vp=var_find(root,index->v.v_string);
	else vp=var_find_index(root,index->v.v_long);
	if _oT(vp)
	{
		if _oF(!(vp->mode&auth_link)) return get_error(errid_VarNotLink,label);
		if _oT(index->type&type_string) var_delete(root,index->v.v_string);
		else var_delete_index(root,index->v.v_long);
	}
	
	ret->type=type_void;
	ret->v.v_long=0;
	return ret;
}


