#include "../main.h"

func(sprint)
{
	static char *label=".sprint";
	char *format;
	var *vp,*sbuf;
	u32 size;
	if _oF(!(argv->v->type&type_string)) goto Err_type;
	format=argv->v->v.v_string;
	argv=argv->r;
	if _oF(!format) goto Err_type;
	sbuf=sbuf_sprintf(format,argv);
	if _oF(sbuf->type&type_err) return sbuf;
	vp=var_alloc(tlog_string,leng_no);
	if _oF(!vp) goto Err_malloc;
	size=strlen(sbuf->v.v_string)+1;
	vp->v.v_string=malloc(size);
	if (!vp->v.v_string) goto Err_malloc;
	vp->mode|=free_pointer;
	memcpy(vp->v.v_string,sbuf->v.v_string,size);
	return vp;
	Err_type:
	return get_error(errid_FunArgvType,label);
	Err_malloc:
	if _oF(vp) var_free(vp);
	return get_error(errid_MemLess,label);
}

func(strlen)
{
	static char *label=".strlen";
	static u32 type_1[1]={type_string};
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_long;
	ret->v.v_long=argv->v->v.v_string?strlen(argv->v->v.v_string):-1;
	return ret;
}

func(strbyte)
{
	static char *label=".strbyte";
	static u32 type_2[2]={type_string,type_num};
	var *vp;
	char *s;
	u32 at;
	if _oF(argc!=2) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	s=vp->v.v_string;
	argv=argv->r;
	vp=argv->v;
	at=vpntof(vp);
	while(*s&&at)
	{
		at--;
		s++;
	}
	ret->type=type_byte|type_unsign;
	ret->v.v_long=(unsigned char)(*s);
	return ret;
}

keyword(strarray)
{
	static char *label=".strarray";
	var *vp;
	u32 size;
	vp=cal(*expp,expp);
	if _oF(vp->type&type_spe) return vp;
	if _oF(**expp!=';')
	{
		var_free(vp);
		return get_error(errid_GraLackSem,label);
	}
	if _oT(isstring(vp))
	{
		if _oT(vp->mode&auth_retype)
		{
			if _oT(vp->v.v_string) size=strlen(vp->v.v_string)+1;
			else size=0;
			vp->type=type_byte|type_unsign;
			vp->length=size;
			return NULL;
		}
		else
		{
			var_free(vp);
			return get_error(errid_VarNotRetype,label);
		}
	}
	else
	{
		var_free(vp);
		return get_error(errid_VarNotString,label);
	}
}

keyword(arrstring)
{
	static char *label=".arrstring";
	var *vp;
	vp=cal(*expp,expp);
	if _oF(vp->type&type_spe) return vp;
	if _oF(**expp!=';')
	{
		var_free(vp);
		return get_error(errid_GraLackSem,label);
	}
	if _oF(!vp->length || !(vp->type&type_byte))
	{
		var_free(vp);
		return get_error(errid_VarNotBytesArray,label);
	}
	if _oT(vp->mode&auth_retype)
	{
		vp->v.vp_byte[vp->length-1]=0;
		vp->length=leng_no;
		vp->type=type_string;
		return NULL;
	}
	else
	{
		var_free(vp);
		return get_error(errid_VarNotRetype,label);
	}
}

