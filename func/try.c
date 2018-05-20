#include "../main.h"

// .try.* 获取 try 抛出的错误信息

func(try_errid)
{
	static char *label=".try.errid";
	static u32 type_1[1]={type_void};
	var *vp;
	error_info *i;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_int|type_unsign;
	ret->v.v_long=0;
	vp=ptvar_get(_pt_buerr);
	if _oF(!vp) return ret;
	i=vp->v.v_error;
	if _oF(!i) return ret;
	ret->v.v_int=i->errid;
	return ret;
}

func(try_errstr_cl)
{
	static char *label=".try.errstr_cl";
	static u32 type_1[1]={type_void};
	var *vp;
	error_info *i;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_string;
	ret->v.v_string=NULL;
	ret->mode&=~free_pointer;
	vp=ptvar_get(_pt_buerr);
	if _oF(!vp) return ret;
	i=vp->v.v_error;
	if _oF(!i) return ret;
	ret->v.v_string=str_error_cl(i->errid);
	return ret;
}

func(try_errstr_ty)
{
	static char *label=".try.errstr_ty";
	static u32 type_1[1]={type_void};
	var *vp;
	error_info *i;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_string;
	ret->v.v_string=NULL;
	ret->mode&=~free_pointer;
	vp=ptvar_get(_pt_buerr);
	if _oF(!vp) return ret;
	i=vp->v.v_error;
	if _oF(!i) return ret;
	ret->v.v_string=str_error_ty(i->errid);
	return ret;
}

func(try_errstr_id)
{
	static char *label=".try.errstr_id";
	static u32 type_1[1]={type_void};
	var *vp;
	error_info *i;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_string;
	ret->v.v_string=NULL;
	ret->mode&=~free_pointer;
	vp=ptvar_get(_pt_buerr);
	if _oF(!vp) return ret;
	i=vp->v.v_error;
	if _oF(!i) return ret;
	ret->v.v_string=str_error_id(i->errid);
	return ret;
}

func(try_errstr_text)
{
	static char *label=".try.errstr_text";
	static u32 type_1[1]={type_void};
	var *vp;
	error_info *i;
	char *s;
	int size=0;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_string;
	ret->v.v_string=NULL;
	ret->mode&=~free_pointer;
	vp=ptvar_get(_pt_buerr);
	if _oF(!vp) return ret;
	i=vp->v.v_error;
	if _oF(!i) return ret;
	if _oF(!i->pt_text) return ret;
	s=i->pt_text->v.v_string;
	if _oF(!s) return ret;
	s+=i->text_p;
	while(s>i->pt_text->v.v_string&&*s!='\n') s--;
	if _oT(*s=='\n') s++;
	while(s[size]&&s[size]!='\n') size++;
	vp=var_alloc(tlog_string,leng_no);
	if _oF(!vp) return get_error(errid_MemLess,label);
	vp->mode=auth_tmpvar;
	vp->v.v_string=malloc(size+1);
	if _oF(!vp->v.v_string) return get_error(errid_MemLess,label);
	vp->mode|=free_pointer;
	memcpy(vp->v.v_string,s,size);
	(vp->v.v_string)[size]=0;
	return vp;
}

func(try_errstr_label)
{
	static char *label=".try.errstr_label";
	static u32 type_1[1]={type_void};
	var *vp;
	error_info *i;
	int size;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_string;
	ret->v.v_string=NULL;
	ret->mode&=~free_pointer;
	vp=ptvar_get(_pt_buerr);
	if _oF(!vp) return ret;
	i=vp->v.v_error;
	if _oF(!i) return ret;
	if _oF(!i->label) return ret;
	size=strlen(i->label)+1;
	vp=var_alloc(tlog_string,leng_no);
	if _oF(!vp) return get_error(errid_MemLess,label);
	vp->mode=auth_tmpvar;
	vp->v.v_string=malloc(size);
	if _oF(!vp->v.v_string) return get_error(errid_MemLess,label);
	vp->mode|=free_pointer;
	memcpy(vp->v.v_string,i->label,size);
	return vp;
}

func(try_errnext)
{
	static char *label=".try.errnext";
	static u32 type_1[1]={type_void};
	var *vp;
	error_info *i;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_void;
	ret->v.v_long=0;
	vp=ptvar_get(_pt_buerr);
	if _oF(!vp) return ret;
	i=vp->v.v_error;
	if _oF(!i)
	{
		ptvar_free(_pt_buerr);
		return ret;
	}
	vp->v.v_error=i->last;
	var_free(i->pt_text);
	free(i);
	if _oF(!vp->v.v_error) ptvar_free(_pt_buerr);
	return ret;
}

