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


