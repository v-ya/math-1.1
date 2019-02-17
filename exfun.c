#include "main.h"


static char *type_name="_type_";

int type_check(var *obj, char *type)
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

var* type_set_auth(var *obj, var *type)
{
	static char *label="type_set_auth";
	if _oF(!(obj->mode&auth_write)) return get_error(errid_VarNotWrite,label);
	return type_set(obj,type);
}

int type_isempty(var *obj)
{
	return !var_find(obj,type_name);
}

int object_isempty(var *obj)
{
	if _oF(obj->type&type_vlist) return !obj->v.v_vlist;
	else if _oT(obj->type&type_vmat) return !obj->v.v_vmat->number;
	else return 0;
}

// var == ok || NULL == fail
var* create_var(var *obj, char *name, u64 head, u32 tlog, u32 length, u32 auth)
{
	var *r;
	vlist *vl;
	if _oT(obj->type&type_object)
	{
		if _oT(name)
		{
			vl=v_find(obj,name);
			if _oT(!vl)
			{
				vl=vlist_alloc(name);
				if _oF(!vl) return NULL;
				if _oF(obj->type&type_vlist) obj->v.v_vlist=vlist_insert(obj->v.v_vlist,vl);
				else vmat_insert(obj->v.v_vmat,vl);
			}
		}
		else
		{
			vl=v_find_index(obj,head);
			if _oT(!vl)
			{
				vl=vlist_alloc_index(head);
				if _oF(!vl) return NULL;
				if _oF(obj->type&type_vlist) obj->v.v_vlist=vlist_insert(obj->v.v_vlist,vl);
				else vmat_insert(obj->v.v_vmat,vl);
			}
		}
		r=var_alloc(tlog,length);
		if _oF(!r) return NULL;
		r->mode&=~auth_all;
		r->mode|=auth&auth_all;
		vlist_link(vl,r);
		return r;
	}
	return NULL;
}

void remove_var(var *obj, char *name, u64 head)
{
	if _oT(obj->type&type_vlist)
	{
		if _oT(name) obj->v.v_vlist=vlist_delete(obj->v.v_vlist,name);
		else obj->v.v_vlist=vlist_delete_index(obj->v.v_vlist,head);
	}
	else if _oT(obj->type&type_vmat)
	{
		if _oT(name) vmat_delete(obj->v.v_vmat,name);
		else vmat_delete_index(obj->v.v_vmat,head);
	}
}

// create void & not set free_pointer
var* create_void(var *obj, char *name, u64 head, u32 auth, void *value)
{
	var *r;
	r=create_var(obj,name,head,tlog_void,leng_no,auth);
	if _oF(!r) return NULL;
	r->v.v_void=value;
	return r;
}

// create byte
var* create_ubyte(var *obj, char *name, u64 head, u32 auth, u8 value)
{
	var *r;
	r=create_var(obj,name,head,tlog_byte,leng_no,auth);
	if _oF(!r) return NULL;
	r->type|=type_unsign;
	r->v.v_long=value;
	return r;
}
var* create_sbyte(var *obj, char *name, u64 head, u32 auth, s8 value)
{
	var *r;
	r=create_var(obj,name,head,tlog_byte,leng_no,auth);
	if _oF(!r) return NULL;
	r->v.v_long=value;
	var_fixvalue(r);
	return r;
}

// create word
var* create_uword(var *obj, char *name, u64 head, u32 auth, u16 value)
{
	var *r;
	r=create_var(obj,name,head,tlog_word,leng_no,auth);
	if _oF(!r) return NULL;
	r->type|=type_unsign;
	r->v.v_long=value;
	return r;
}
var* create_sword(var *obj, char *name, u64 head, u32 auth, s16 value)
{
	var *r;
	r=create_var(obj,name,head,tlog_word,leng_no,auth);
	if _oF(!r) return NULL;
	r->v.v_long=value;
	var_fixvalue(r);
	return r;
}

// create int
var* create_uint(var *obj, char *name, u64 head, u32 auth, u32 value)
{
	var *r;
	r=create_var(obj,name,head,tlog_int,leng_no,auth);
	if _oF(!r) return NULL;
	r->type|=type_unsign;
	r->v.v_long=value;
	return r;
}
var* create_sint(var *obj, char *name, u64 head, u32 auth, s32 value)
{
	var *r;
	r=create_var(obj,name,head,tlog_int,leng_no,auth);
	if _oF(!r) return NULL;
	r->v.v_long=value;
	var_fixvalue(r);
	return r;
}

// create long
var* create_ulong(var *obj, char *name, u64 head, u32 auth, u64 value)
{
	var *r;
	r=create_var(obj,name,head,tlog_long,leng_no,auth);
	if _oF(!r) return NULL;
	r->type|=type_unsign;
	r->v.v_long=value;
	return r;
}
var* create_slong(var *obj, char *name, u64 head, u32 auth, s64 value)
{
	var *r;
	r=create_var(obj,name,head,tlog_long,leng_no,auth);
	if _oF(!r) return NULL;
	r->v.v_long=value;
	return r;
}

// create float
var* create_float(var *obj, char *name, u64 head, u32 auth, double value)
{
	var *r;
	r=create_var(obj,name,head,tlog_float,leng_no,auth);
	if _oF(!r) return NULL;
	r->v.v_float=value;
	return r;
}

// create string
var* create_string(var *obj, char *name, u64 head, u32 auth, char *value)
{
	var *r;
	u32 size;
	r=create_var(obj,name,head,tlog_string,leng_no,auth);
	if _oF(!r) return NULL;
	if _oT(value)
	{
		size=strlen(value)+1;
		r->v.v_string=malloc(size);
		if _oF(!r->v.v_string) return NULL;
		r->mode|=free_pointer;
		memcpy(r->v.v_string,value,size);
	}
	return r;
}

// create vlist|vmat
var* create_vlist(var *obj, char *name, u64 head, u32 auth)
{
	return create_var(obj,name,head,tlog_vlist,leng_no,auth);
}
var* create_vmat(var *obj, char *name, u64 head, u32 auth)
{
	return create_var(obj,name,head,tlog_vmat,leng_no,auth);
}

// create refer
var* create_refer(var *obj, char *name, u64 head, u32 auth, var *value)
{
	var *r;
	u32 size;
	r=create_var(obj,name,head,tlog_refer,leng_no,auth);
	if _oF(!r) return NULL;
	if _oT(value)
	{
		refer_set(r,value);
		if _oF(refer_check(r)!=value) return NULL;
	}
	return r;
}

u64 get_sid(void)
{
	u64 sid;
	lock_alloc(lock_sid);
	do
	{
		sid=(_info_sid->v.v_long)++;
	} while(!sid);
	lock_free(lock_sid);
	return sid;
}

