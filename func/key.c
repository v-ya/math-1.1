#include "../main.h"

keyword(var)
{
	static char *label="[object].var";
	var *vp;
	if _oF(!call) return get_error(errid_IntError,label);
	if _oF(call->mode&auth_system) return get_error(errid_VarIsSystem,label);
	if _oF(!(call->type&type_object)) return get_error(errid_VarNotObject,label);
	Loop:
	switch(**expp)
	{
		case 0:
		case ';':
			return NULL;
		case ',':
			(*expp)++;
			while(is_space(**expp)) (*expp)++;
			goto Loop;
		default:
			vp=var_define(*expp,expp,call);
			if _oF(vp&&(vp->type&type_err)) return vp;
			goto Loop;
	}
}

keyword(delete)
{
	static char *label="[object].delete";
	char *name;
	var *vp;
	if _oF(!call) return get_error(errid_IntError,label);
	if _oF(call->mode&auth_system) return get_error(errid_VarIsSystem,label);
	if _oF(!(call->type&type_object)) return get_error(errid_VarNotObject,label);
	Loop:
	switch(**expp)
	{
		case 0:
		case ';':
			return NULL;
		case ',':
			(*expp)++;
			while(is_space(**expp)) (*expp)++;
			goto Loop;
		default:
			if _oF(!is_Name(**expp)) return get_error(errid_GraQuoVarname,label);
			name=get_name(*expp,expp);
			if _oF(!name) return get_error(errid_MemLess,label);
			vp=var_find(call,name);
			if _oT(vp)
			{
				if _oT(vp->mode&auth_link) var_delete(call,name);
				else return get_error(errid_VarNotLink,label);
			}
			goto Loop;
	}
}

keyword(function)
{
	static char *label="[object].function";
	var *root,*_vt_=NULL,*_vn_=NULL,*vp;
	vlist *vl;
	char *exp,*name=NULL,*type=NULL,*s;
	u32 tlog;
	if _oF(!call) return get_error(errid_IntError,label);
	if _oF(call->mode&auth_system) return get_error(errid_VarIsSystem,label);
	if _oF(!(call->type&type_object)) return get_error(errid_VarNotObject,label);
	if _oF(!(call->mode&auth_write)) return get_error(errid_VarNotWrite,label);
	exp=*expp;
	s=get_name(exp,&exp);
	if _oF(!s) goto Err_malloc;
	type=malloc(strlen(s)+1);
	if (!type) goto Err_malloc;
	strcpy(type,s);
	tlog=get_tlog(type);
	if _oF(tlog>=tlog_max) goto Err_nottype;
	while(is_space(*exp)) exp++;
	if _oT(*exp==':')
	{
		exp++;
		while(is_space(*exp)) exp++;
	}
	if _oF(!is_Name(*exp)) goto Err_notname;
	s=get_name(exp,&exp);
	if _oF(!s) goto Err_malloc;
	name=malloc(strlen(s)+1);
	if (!name) goto Err_malloc;
	strcpy(name,s);
	if _oF(var_find(_vm_gobj,name)) goto Err_notname;
	vl=var_insert(call,name,tlog_vlist,leng_no);
	if _oF(!vl) goto Err_malloc;
	root=vl->v;
	// define _ret_ srw-
	vl=var_insert(root,"_ret_",tlog,leng_no);
	if _oF(!vl) goto Err_malloc;
	if _oF(type[0]=='u') vl->v->type|=type_unsign;
	vl->v->mode&=~auth_link;
	while(is_space(*exp)) exp++;
	if _oF(*exp!='(') goto Err_notbra;
	exp++;
	// get _vt_, _vn_
	vp=function_define(exp,&exp,&_vt_,&_vn_);
	if _oF(vp) goto Err;
	if _oF(*exp!=')') goto Err_notbra;
	exp++;
	// define _vt_ -r--
	vl=var_insert(root,"_vt_",tlog_max,leng_no);
	if _oF(!vl) goto Err_malloc;
	vlist_link(vl,_vt_);
	_vt_=NULL;
	// define _vn_ -r--
	vl=var_insert(root,"_vn_",tlog_max,leng_no);
	if _oF(!vl) goto Err_malloc;
	vlist_link(vl,_vn_);
	_vn_=NULL;
	// define _text_ -rw-
	while(is_space(*exp)) exp++;
	vl=var_insert(root,"_text_",tlog_string,leng_no);
	if _oF(!vl) goto Err_malloc;
	vl->v->mode&=auth_read|auth_write;
	if _oT(*exp=='{') {
		vl->v->v.v_string=get_code(exp,&exp);
		if _oF(!vl->v->v.v_string) goto Err_malloc;
		vl->v->mode|=free_pointer;
		while(is_space(*exp)) exp++;
	}
	if _oF(*exp!=';') goto Err_notsem;
	// define _takeup_ -r--
	vp=create_var(root,"_takeup_",0,tlog_long,2,auth_read);
	if _oF(!vp) goto Err_malloc;
	vp->type|=type_unsign;
	vp=NULL;
	root->mode|=auth_run;
	Err:
	free(type);
	free(name);
	if _oF(_vt_) var_free(_vt_);
	if _oF(_vn_) var_free(_vn_);
	*expp=exp;
	return vp;
	Err_nottype:
	vp=get_error(errid_VarUnknowType,label);
	goto Err;
	Err_notname:
	vp=get_error(errid_GraQuoVarname,label);
	goto Err;
	Err_notbra:
	vp=get_error(errid_GraBraMismatch,label);
	goto Err;
	Err_malloc:
	vp=get_error(errid_MemLess,label);
	goto Err;
	Err_notsem:
	vp=get_error(errid_GraLackSem,label);
	goto Err;
}

keyword(goto)
{
	static char *label=".goto";
	char *exp;
	var *vp;
	exp=*expp;
	while(is_name(**expp)) (*expp)++;
	while(is_space(**expp)) (*expp)++;
	if _oF(**expp!=';'&&**expp!=0) return get_error(errid_GraLackSem,label);
	vp=ptvar_get(_pt_text);
	if _oF((!vp)||(!vp->v.v_string)) return get_error(errid_SysPtvarNotfind,label);
	exp=goto_exp(vp->v.v_string,exp);
	if _oF(!exp) return get_error(errid_FunNotfindLabel,label);
	*expp=exp;
	return NULL;
}

keyword(gotodw)
{
	static char *label=".gotodw";
	char *exp,*l;
	exp=*expp;
	l=exp;
	while(is_name(*exp)) exp++;
	while(is_space(*exp)) exp++;
	*expp=exp;
	if _oF(*exp!=';'&&*exp!=0) return get_error(errid_GraLackSem,label);
	exp=goto_exp(exp,l);
	if _oF(!exp) return get_error(errid_FunNotfindLabel,label);
	*expp=exp;
	return NULL;
}

keyword(gotoup)
{
	static char *label=".gotoup";
	char *exp,*l;
	var *vp;
	exp=*expp;
	l=exp;
	while(is_name(*exp)) exp++;
	while(is_space(*exp)) exp++;
	*expp=exp;
	if _oF(*exp!=';'&&*exp!=0) return get_error(errid_GraLackSem,label);
	vp=ptvar_get(_pt_text);
	if _oF((!vp)||(!vp->v.v_string)) return get_error(errid_SysPtvarNotfind,label);
	exp=goto_exp_last(l,vp->v.v_string,l);
	if _oF(!exp) return get_error(errid_FunNotfindLabel,label);
	*expp=exp;
	return NULL;
}

keyword(if)
{
	static char *label=".if";
	char *exp;
	var *vp;
	exp=*expp;
	vp=cal(exp,&exp);
	if _oF(vp->type&type_spe) goto Err;
	if _oF(*exp!=';')
	{
		var_free(vp);
		vp=get_error(errid_GraLackSem,label);
		goto Err;
	}
	if _oF(vpbool(vp))
	{
		// True
		var_free(vp);
		*expp=exp;
		return NULL;
	}
	else
	{
		// False
		var_free(vp);
		exp=get_nextsem(++exp);
		*expp=exp;
		return NULL;
	}
	Err:
	*expp=exp;
	return vp;
}

keyword(while)
{
	static char *label=".while";
	char *exp_c,*exp_x,*exp_e;
	var *vp,*pt_kill;
	pt_kill=ptvar_get(_pt_kill);
	exp_c=*expp;
	vp=cal(*expp,expp);
	exp_x=*expp;
	if _oF(vp->type&type_spe) return vp;
	if _oF(*exp_x!=';')
	{
		var_free(vp);
		return get_error(errid_GraLackSem,label);
	}
	exp_x++;
	while(is_space(*exp_x)) exp_x++;
	*expp=get_nextsem(exp_x);
	while(1)
	{
		if _oF(vp->type&type_spe)
		{
			*expp=exp_e;
			return vp;
		}
		if _oT(vpbool(vp))
		{
			var_free(vp);
			if _oF(pt_kill&&pt_kill->mode) return get_error(errid_IntKilled,label);
			vp=cal(exp_x,&exp_e);
			if _oF(vp->type&type_spe)
			{
				*expp=exp_e;
				return vp;
			}
			var_free(vp);
			vp=cal(exp_c,&exp_e);
			continue;
		}
		else
		{
			var_free(vp);
			return NULL;
		}
	}
}

keyword(exit)
{
	static var exit = {
		.type = type_end,
		.length = leng_no,
		.inode = 0,
		.mode = auth_read,
		.v.v_long = 0
	};
	var *vp;
	vp=cal(*expp,expp);
	if _oF(vp->type&type_spe) return vp;
	var_free(vp);
	return &exit;
}

keyword(run)
{
	static char *label="[object].run";
	var *vp,*code;
	if _oF(!call) return get_error(errid_IntError,label);
	if _oF(!(call->type&type_object)) return get_error(errid_VarNotObject,label);
	
	vp=cal(*expp,expp);
	if _oF(vp->type&type_spe) return vp;
	if _oF(**expp!=';')
	{
		var_free(vp);
		return get_error(errid_GraLackSem,label);
	}
	if _oT(isstring(vp))
	{
		code=vp;
		vp=run_script(code,call);
		var_free(code);
		return vp;
	}
	else
	{
		var_free(vp);
		return get_error(errid_VarNotString,label);
	}
}

keyword(try)
{
	static char *label="[object].try";
	var *vp,*code,*deal;
	if _oF(!call) return get_error(errid_IntError,label);
	if _oF(!(call->type&type_object)) return get_error(errid_VarNotObject,label);
	
	vp=cal(*expp,expp);
	if _oF(vp->type&type_spe) return vp;
	code=vp;
	if _oF(**expp==',')
	{
		(*expp)++;
		vp=cal(*expp,expp);
		if _oF(vp->type&type_spe)
		{
			var_free(code);
			return vp;
		}
		deal=vp;
	}
	else deal=NULL;
	if _oF(**expp!=';')
	{
		var_free(code);
		var_free(deal);
		return get_error(errid_GraLackSem,label);
	}
	if _oT(isstring(code)&&(deal==NULL||isstring(deal)))
	{
		vp=run_script(code,call);
		var_free(code);
		if _oF(vp&&deal)
		{
			// Throw Error && run deal
			backup_error();
			vp=run_script(deal,call);
			backup_clr_error();
		}
		else
		{
			clr_error();
			vp=NULL;
		}
		var_free(deal);
		return vp;
	}
	else
	{
		var_free(code);
		var_free(deal);
		return get_error(errid_VarNotString,label);
	}
}

keyword(include)
{
	static char *label="[object].include";
	char *path;
	var *vp,*code;
	if _oF(!call) return get_error(errid_IntError,label);
	if _oF(!(call->type&type_object)) return get_error(errid_VarNotObject,label);
	
	vp=cal(*expp,expp);
	if _oF(vp->type&type_spe) return vp;
	if _oF(**expp!=';')
	{
		var_free(vp);
		return get_error(errid_GraLackSem,label);
	}
	if _oT(isstring(vp))
	{
		// alloc code
		code=var_alloc(tlog_string,leng_no);
		if _oF(!code)
		{
			var_free(vp);
			return get_error(errid_MemLess,label);
		}
		// get path
		path=vp->v.v_string;
		if _oF(_path_include->v.v_string&&path&&path[0]!=_path_incutup->v.v_byte)
		{
			path=get_path(_path_include->v.v_string,path);
			code->v.v_string=load_string(path);
			free(path);
		}
		else code->v.v_string=load_string(path);
		var_free(vp);
		if _oF(!code->v.v_string)
		{
			var_free(code);
			vp=ptvar_get(_pt_error);
			if _oF(vp) return vp;
			else return get_error(errid_SysFileNotLoad,label);
		}
		else code->mode|=free_pointer;
		vp=run_script(code,call);
		return vp;
	}
	else
	{
		var_free(vp);
		return get_error(errid_VarNotString,label);
	}
}


