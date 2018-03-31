#include "main.h"

static char *type_name="_type_";

u32 type_check(var *obj, char *type)
{
	var *vp;
	char *st;
	u32 i;
	if _oF(!(obj->type&type_object)) goto Not;
	vp=var_find(obj,type_name);
	if _oF(!vp) goto Null;
	if _oF(vp->type!=type_string) goto Null;
	if _oF((vp->mode&auth_all)!=auth_read) goto Null;
	st=vp->v.v_string;
	for(i=0;st[i]&&(st[i]==type[i]);i++) ;
	if _oF(type[i]!=0) goto Not;
	if _oT((st[i]==0)||(st[i]=='.')) return 1;
	Not:
	return -1;
	Null:
	return 0;
}

var* type_set(var *obj, var *type)
{
	static char *label="type_set";
	vlist *vl;
	if _oF(!(obj->mode&auth_write)) return get_error(errid_VarNotWrite,label);
	if _oF(!(obj->mode&auth_retype)) return get_error(errid_VarNotRetype,label);
	if _oT(obj->type&type_vlist)
	{
		vl=vlist_find(obj->v.v_vlist,type_name);
		if _oF(!vl)
		{
			vl=vlist_alloc(type_name);
			if _oF(!vl) goto Err_mem;
			obj->v.v_vlist=vlist_insert(obj->v.v_vlist,vl);
		}
	}
	else if _oT(obj->type&type_vmat)
	{
		vl=vmat_find(obj->v.v_vmat,type_name);
		if _oF(!vl)
		{
			vl=vlist_alloc(type_name);
			if _oF(!vl) goto Err_mem;
			vmat_insert(obj->v.v_vmat,vl);
		}
	}
	else return get_error(errid_VarNotObject,label);
	vlist_link(vl,type);
	return NULL;
	Err_mem:
	return get_error(errid_MemLess,label);
}

var* type_empty(var *obj)
{
	return type_set(obj,_type_null);
}

