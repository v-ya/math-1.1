#include "../main.h"

// .thread.* 线程相关函数

func(thread_self)
{
	ret->type=type_long|type_unsign;
	ret->v.v_long=pthread_self();
	return ret;
}

func(thread_create)
{
	static char *label="[object].thread";
	static u32 type_1[1]={type_string};
	static u32 type_2[2]={type_string,type_string};
	char *name=NULL;
	var *code,*env;
	env=ret->v.v_var;
	switch(argc)
	{
		case 1:
			if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
			code=argv->v;
			break;
		case 2:
			if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
			name=argv->v->v.v_string;
			code=argv->r->v;
			break;
		default:
			return get_error(errid_FunArgvType,label);
	}
	if _oF(!code->v.v_string) return get_error(errid_FunArgvType,label);
	ret->type=type_long|type_unsign;
	if _oF(env=thread_create(name,code,env,&(ret->v.v_long))) return env;
	return ret;
}

func(thread_kill)
{
	static char *label=".thread.kill";
	var *vp;
	u64 pthid;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	if _oF(vp->length) return get_error(errid_FunArgvType,label);
	ret->type=type_long;
	switch(vp->type&type_all)
	{
		case type_string:
			if _oT(vp->v.v_string) ret->v.v_long=thread_getid(vp->v.v_string,&pthid);
			else return get_error(errid_FunArgvType,label);
			if _oF(ret->v.v_long) return ret;
			break;
		default:
			if _oT(vp->type&type_znum) pthid=vp->v.v_long;
			else return get_error(errid_FunArgvType,label);
	}
	ret->v.v_long=thread_kill(pthid);
	return ret;
}

func(thread_wait)
{
	static char *label=".thread.wait";
	var *vp;
	u64 pthid;
	ret->type=type_void;
	ret->v.v_long=0;
	if _oF(argc==1 && (argv->v->type&type_void))
	{
		thread_waitall();
		return ret;
	}
	while(argv)
	{
		vp=argv->v;
		argv=argv->r;
		if _oF(vp->length) return get_error(errid_FunArgvType,label);
		switch(vp->type&type_all)
		{
			case type_string:
				if _oT(vp->v.v_string) ret->v.v_long=thread_getid(vp->v.v_string,&pthid);
				else return get_error(errid_FunArgvType,label);
				if _oF(ret->v.v_long) return ret;
				break;
			default:
				if _oT(vp->type&type_znum) pthid=vp->v.v_long;
				else return get_error(errid_FunArgvType,label);
				break;
		}
		thread_wait(pthid);
	}
	return ret;
}

#define _LOCK_TYPE	"lock"
#define _LOCK_LIST	"list"
#define _LOCK_TAKEUP	"takeup"
#define _LOCK_USED	"used"
#define _LOCK_FATHERS	"fathers"
#define _LOCK_create	"create"
#define _LOCK_remove	"remove"
#define _LOCK_use	"use"

static var _lock_type = {
	.type	=type_string,
	.length	=leng_no,
	.inode	=1,
	.mode	=auth_read,
	.v	=_LOCK_TYPE
} ;

func(lock_index_create)
{
	static char *label="[lock].create";
	var *vp,*list;
	char *name;
	u64 head;
	vp=ret->v.v_var;
	if _oF(type_check(vp,_LOCK_TYPE)<=0) return get_error(errid_VarErrorCaller,label);
	list=var_find(vp,_LOCK_LIST);
	if _oT(list) while(argv)
	{
		vp=argv->v;
		argv=argv->r;
		if _oT((vp->type&type_string) && vp->v.v_string)
		{
			name=vp->v.v_string;
			head=0;
		}
		else if _oT(vp->type&type_znum)
		{
			name=NULL;
			head=vp->v.v_long;
		}
		else return get_error(errid_FunArgvType,label);
		lock_alloc(lock_user);
		if _oF(name?var_find(list,name):var_find_index(list,head)) ;
		else
		{
			vp=create_var(list,name,head,tlog_long,2,auth_read);
			if _oF(!vp) return get_error(errid_MemLess,label);
			vp->type|=type_unsign;
		}
		lock_free(lock_user);
	}
	ret->type=type_void;
	ret->v.v_long=0;
	return ret;
}

func(lock_index_remove)
{
	static char *label="[lock].remove";
	var *vp,*list;
	char *name;
	u64 head;
	vp=ret->v.v_var;
	if _oF(type_check(vp,_LOCK_TYPE)<=0) return get_error(errid_VarErrorCaller,label);
	list=var_find(vp,_LOCK_LIST);
	if _oT(list) while(argv)
	{
		vp=argv->v;
		argv=argv->r;
		if _oT((vp->type&type_string) && vp->v.v_string)
		{
			name=vp->v.v_string;
			head=0;
		}
		else if _oT(vp->type&type_znum)
		{
			name=NULL;
			head=vp->v.v_long;
		}
		else return get_error(errid_FunArgvType,label);
		lock_alloc(lock_user);
		vp=name?var_find(list,name):var_find_index(list,head);
		if _oT(vp)
		{
			if _oF(vp->v.vp_long[0]) return get_error(errid_SysLockTakeup,label);
			remove_var(list,name,head);
		}
		lock_free(lock_user);
	}
	ret->type=type_void;
	ret->v.v_long=0;
	return ret;
}

void _used_plus(var *lock, int value, int takeup)
{
	var *vp,*used;
	var **locks;
	u32 i,n;
	u64 pthid;
	pthid=pthread_self();
	lock_alloc(lock_user);
	vp=var_find(lock,_LOCK_FATHERS);
	if _oT(!vp) goto self;
	locks=(var**) vp->v.vp_void;
	n=vp->length;
	for(i=0;i<n;i++)
	{
		vp=var_find(locks[i],_LOCK_TAKEUP);
		while(vp && vp->v.vp_long[0] && vp->v.vp_long[1]!=pthid)
		{
			lock_free(lock_user);
			usleep(LOCK_USLEEP);
			lock_alloc(lock_user);
		}
		vp=var_find(locks[i],_LOCK_USED);
		if _oT(vp && (value>=0 || vp->v.v_long)) vp->v.v_long+=value;
	}
	self:
	used=var_find(lock,_LOCK_USED);
	vp=var_find(lock,_LOCK_TAKEUP);
	if _oF(takeup)
	{
		while(1)
		{
			if _oF(vp && vp->v.vp_long[0] && vp->v.vp_long[1]==pthid) break;
			else if _oF((!used || !used->v.v_long) && (!vp || !vp->v.vp_long[0])) break;
			lock_free(lock_user);
			usleep(LOCK_USLEEP);
			lock_alloc(lock_user);
		}
		if _oT(vp && (value>=0 || vp->v.vp_long[0]))
		{
			vp->v.vp_long[0]+=value;
			if _oT(vp->v.vp_long[0]) vp->v.vp_long[1]=pthid;
			else vp->v.vp_long[1]=0;
		}
	}
	else 
	{
		while(vp && vp->v.vp_long[0] && vp->v.vp_long[1]!=pthid)
		{
			lock_free(lock_user);
			usleep(LOCK_USLEEP);
			lock_alloc(lock_user);
		}
	}
	vp=used;
	if _oT(vp && (value>=0 || vp->v.v_long)) vp->v.v_long+=value;
	lock_free(lock_user);
}

keyword(lock_index_use)
{
	static char *label="[lock].use";
	var *index,*lock,*takeup,*vp;
	char *exp,*name;
	u64 head;
	
	if _oF(type_check(call,_LOCK_TYPE)<=0) return get_error(errid_VarErrorCaller,label);
	exp=*expp;
	switch(*exp)
	{
		case ':':
			exp++;
			while(is_space(*exp)) exp++;
			*expp=exp;
			if _oT(is_Name(*exp))
			{
				index=NULL;
				name=get_name(exp,&exp);
				head=0;
			}
			else if _oT(*exp>='0' && *exp<='9' || *exp=='-')
			{
				index=temp_get();
				if _oF(index->type&type_spe) return index;
				get_tmpvar(exp,&exp,index);
				if _oT(index->type&type_znum) head=index->v.v_long;
				else return get_error(errid_GraUnknowSym,label);
				name=NULL;
			}
			else return get_error(errid_GraUnknowSym,label);
			break;
		case '@':
			exp++;
			while(is_space(*exp)) exp++;
			*expp=exp;
			index=get_var(exp,&exp,NULL);
			*expp=exp;
			if _oF(index->type&type_spe) return index;
			if _oT(index->type&type_string)
			{
				name=index->v.v_string;
				if _oF(!name) return get_error(errid_FunArgvType,label);
				head=0;
			}
			else if _oT(index->type&type_znum)
			{
				head=index->v.v_long;
				name=NULL;
			}
			break;
		default:
			goto _takeup;
	}
	while(is_space(*exp)) exp++;
	*expp=exp;
	_used_plus(call,1,0);
	lock=var_find(call,_LOCK_LIST);
	if _oT(lock) lock=name?var_find(lock,name):var_find_index(lock,head);
	if _oF(index) var_free(index);
	if _oT(lock)
	{
		lock_alloc_user(lock);
		vp=cal(*expp,expp);
		lock_free_user(lock);
	}
	else
	{
		*expp=get_nextsem(*expp);
		vp=NULL;
	}
	_used_plus(call,-1,0);
	if _oT(vp)
	{
		if _oF(vp->type&type_spe) return vp;
		var_free(vp);
	}
	return NULL;
	_takeup:
	_used_plus(call,1,1);
	vp=cal(*expp,expp);
	_used_plus(call,-1,1);
	if _oT(vp)
	{
		if _oF(vp->type&type_spe) return vp;
		var_free(vp);
	}
	return NULL;
}

func(lock_create)
{
	static char *label="[object].lock";
	var *root,*lock,*fathers,*vp;
	char *name=NULL;
	u64 head=0;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	if _oT(vp->type&type_string)
	{
		name=vp->v.v_string;
		if _oF(!name) return get_error(errid_FunArgvType,label);
	}
	else if _oT(vp->type&type_znum) head=vp->v.v_long;
	else return get_error(errid_FunArgvType,label);
	root=ret->v.v_var;
	if _oT(root->mode&auth_write)
	{
		if _oF(name?var_find(root,name):var_find_index(root,head)) return get_error(errid_VarIsExist,label);
		lock=create_vlist(root,name,head,auth_read|auth_write);
		if _oF(!lock) return get_error(errid_MemLess,label);
		vp=create_vlist(lock,_LOCK_LIST,0,auth_read);
		if _oF(!vp) return get_error(errid_MemLess,label);
		vp=create_ulong(lock,_LOCK_USED,0,auth_read,0);
		if _oF(!vp) return get_error(errid_MemLess,label);
		vp=create_var(lock,_LOCK_TAKEUP,0,tlog_long,2,auth_read);
		if _oF(!vp) return get_error(errid_MemLess,label);
		vp->type|=type_unsign;
		if _oF(type_check(root,_LOCK_TYPE)>0)
		{
			// create fathers
			fathers=var_find(root,_LOCK_FATHERS);
			if _oF(fathers && (fathers->type&type_void) && fathers->length)
			{
				vp=create_var(lock,_LOCK_FATHERS,0,tlog_void,fathers->length+1,auth_read);
				if _oF(!vp) return get_error(errid_MemLess,label);
				for(head=0;head<fathers->length;head++)
					vp->v.vp_void[head]=fathers->v.vp_void[head];
				vp->v.vp_void[head]=root;
			}
			else
			{
				vp=create_var(lock,_LOCK_FATHERS,0,tlog_void,1,auth_read);
				if _oF(!vp) return get_error(errid_MemLess,label);
				vp->v.vp_void[0]=root;
			}
		}
		vp=create_void(lock,_LOCK_create,0,auth_read|auth_run,addr_fun(lock_index_create));
		if _oF(!vp) return get_error(errid_MemLess,label);
		vp=create_void(lock,_LOCK_remove,0,auth_read|auth_run,addr_fun(lock_index_remove));
		if _oF(!vp) return get_error(errid_MemLess,label);
		vp=create_void(lock,_LOCK_use,0,auth_read|auth_key,addr_key(lock_index_use));
		if _oF(!vp) return get_error(errid_MemLess,label);
		vp=type_set(lock,&_lock_type);
		if _oF(vp) return vp;
	}
	else return get_error(errid_VarNotWrite,label);
	ret->type=type_void;
	ret->v.v_long=0;
	return ret;
}

#undef _LOCK_TYPE
#undef _LOCK_LIST
#undef _LOCK_TAKEUP
#undef _LOCK_USED
#undef _LOCK_FATHERS
#undef _LOCK_create
#undef _LOCK_remove
#undef _LOCK_use

func(sleep)
{
	static char *label=".thread/time.sleep";
	static u32 type_1[1]={type_znum};
	u64 t;
	if _oF(argc!=1 || check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_void;
	ret->v.v_long=0;
	t=argv->v->v.v_long;
	sleep(t);
	return ret;
}

func(msleep)
{
	static char *label=".thread/time.msleep";
	static u32 type_1[1]={type_znum};
	u64 t;
	if _oF(argc!=1 || check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_void;
	ret->v.v_long=0;
	t=argv->v->v.v_long&0x003fffffffffffffUL;
	usleep(t*1000);
	return ret;
}

func(usleep)
{
	static char *label=".thread/time.usleep";
	static u32 type_1[1]={type_znum};
	u64 t;
	if _oF(argc!=1 || check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	ret->type=type_void;
	ret->v.v_long=0;
	t=argv->v->v.v_long;
	usleep(t);
	return ret;
}

