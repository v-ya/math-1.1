#include "main.h"

pthread_attr_t *attr;
vmat *pthid_vm;
static var _thread_kill_data = {
	.type	=type_err,
	.length	=leng_no,
	.inode	=0,
	.mode	=0,
	.v	=0
} ;
var *_thread_kill=&_thread_kill_data;

int thread_init(void)
{
	s32 i;
	void *v;
	size_t l;
	struct sched_param sp;
	pthid_vm=vmat_alloc();
	if _oF(!pthid_vm) goto Err_p;
	attr=malloc(sizeof(pthread_attr_t));
	if _oF(!attr) goto Err_m;
	if _oF(pthread_attr_init(attr)) goto Err_i;
	_thread_attr->v.v_void=attr;
	// detach_state
	if _oF(pthread_attr_getdetachstate(attr,&i)) goto Err;
	switch(i)
	{
		case PTHREAD_CREATE_JOINABLE:
			_thattr_detach_state->v.v_long=0;
			break;
		case PTHREAD_CREATE_DETACHED:
			_thattr_detach_state->v.v_long=1;
			break;
		default:
			_thattr_detach_state->v.v_long=-1;
			break;
	}
	// sched_policy
	if _oF(pthread_attr_getschedpolicy(attr,&i)) goto Err;
	switch(i)
	{
		case SCHED_OTHER:
			_thattr_sched_policy->v.v_long=0;
			break;
		case SCHED_FIFO:
			_thattr_sched_policy->v.v_long=1;
			break;
		case SCHED_RR:
			_thattr_sched_policy->v.v_long=2;
			break;
		default:
			_thattr_sched_policy->v.v_long=-1;
			break;
	}
	// sched_param
	if _oF(pthread_attr_getschedparam(attr,&sp)) goto Err;
	_thattr_sched_param->v.v_int=sp.sched_priority;
	var_fixvalue(_thattr_sched_param);
	// inheritance
	if _oF(pthread_attr_getinheritsched(attr,&i)) goto Err;
	switch(i)
	{
		case PTHREAD_INHERIT_SCHED:
			_thattr_inheritance->v.v_long=0;
			break;
		case PTHREAD_EXPLICIT_SCHED:
			_thattr_inheritance->v.v_long=1;
			break;
		default:
			_thattr_inheritance->v.v_long=-1;
			break;
	}
	// scope
	if _oF(pthread_attr_getscope(attr,&i)) goto Err;
	switch(i)
	{
		case PTHREAD_SCOPE_SYSTEM:
			_thattr_scope->v.v_long=0;
			break;
		case PTHREAD_SCOPE_PROCESS:
			_thattr_scope->v.v_long=1;
			break;
		default:
			_thattr_scope->v.v_long=-1;
			break;
	}
	// stack_addr & stack_size
	if _oF(pthread_attr_getstack(attr,&v,&l)) goto Err;
	_thattr_stack_addr->v.v_void=v;
	_thattr_stack_size->v.v_long=l;
	// guard_size
	if _oF(pthread_attr_getguardsize(attr,&l)) goto Err;
	_thattr_guard_size->v.v_long=l;
	return 0;
	Err:
	pthread_attr_destroy(attr);
	Err_i:
	free(attr);
	Err_m:
	vmat_free(pthid_vm);
	Err_p:
	_thread_attr->v.v_void=NULL;
	attr=NULL;
	return -1;
}

void thread_uini(void)
{
	if _oT(_thread_attr->v.v_void)
	{
		pthread_attr_destroy(_thread_attr->v.v_void);
		free(_thread_attr->v.v_void);
	}
	vmat_free(pthid_vm);
	_thread_attr->v.v_void=NULL;
	attr=NULL;
}

static u8 _lock_[lock_NUMBER];
void lock_alloc(int id)
{
	static u8 old=0,new=1;
	u64 n=LOCK_LOOP;
	while(!__sync_bool_compare_and_swap(&_lock_[id],old,new))
		if _oF(--n==0) goto _loop_sleep;
	return ;
	_loop_sleep:
	while(!__sync_bool_compare_and_swap(_lock_+id,old,new))
		usleep(LOCK_USLEEP);
	return ;
}

void lock_free(int id)
{
	_lock_[id]=0;
}

/*
	var lock -> long[2]
	u64 [0]: takeup
	u64 [1]: pthid
*/
void lock_alloc_user(var *lock)
{
	u64 pthid;
	pthid=pthread_self();
	if _oT(lock && (lock->type&type_long) && lock->length==2 && lock->v.vp_long) while(1)
	{
		lock_alloc(lock_user);
		if _oT(!lock->v.vp_long[0]) goto End;
		else if _oF(lock->v.vp_long[1]==pthid) goto End;
		lock_free(lock_user);
		usleep(LOCK_USLEEP);
	}
	End:
	lock->v.vp_long[0]+=1;
	lock->v.vp_long[1]=pthid;
	lock_free(lock_user);
}

void lock_free_user(var *lock)
{
	if _oT(lock && (lock->type&type_long) && lock->length==2 && lock->v.vp_long)
	{
		lock_alloc(lock_user);
		if _oT(lock->v.vp_long[0] && lock->v.vp_long[1]==pthread_self())
		{
			if _oT(!(lock->v.vp_long[0]-=1)) lock->v.vp_long[1]=0;
		}
		lock_free(lock_user);
	}
}

void thread_pthid_alloc(u64 pthid)
{
	vlist *vl;
	vl=vlist_alloc_index(pthid);
	if _oF(!vl) return ;
	lock_alloc(lock_pthid);
	vmat_insert(pthid_vm,vl);
	vl->mode=0;
	lock_free(lock_pthid);
}

void thread_pthid_free(u64 pthid,u32 mask)
{
	vlist *vl;
	lock_alloc(lock_pthid);
	vl=vmat_find_index(pthid_vm,pthid);
	if _oF(vl)
	{
		vl->mode|=mask;
		if _oF((vl->mode&3)==3) vmat_delete_index(pthid_vm,pthid);
	}
	lock_free(lock_pthid);
}

int thread_pthid_test(u64 pthid)
{
	int r;
	lock_alloc(lock_pthid);
	r=!!vmat_find_index(pthid_vm,pthid);
	lock_free(lock_pthid);
	return r;
}

void* thread_handle(void *args)
{
	char *name;
	var *code,*env,*vp;
	u64 pthid;
	name=((thread_args*)args)->name;
	code=((thread_args*)args)->code;
	env=((thread_args*)args)->env;
	pthid=pthread_self();
	free(args);
	thread_pthid_alloc(pthid);
	if _oF(name)
	{
		lock_alloc(lock_thread);
		if _oF(!var_find(_thread_thid,name))
		{
			vp=var_replace(_thread_thid,name,tlog_long,leng_no);
			if _oT(vp)
			{
				vp->type|=type_unsign;
				vp->mode=vp->mode&(~auth_all)|auth_read;
				vp->v.v_long=pthid;
			}
		}
		lock_free(lock_thread);
	}
	vp=var_alloc(tlog_void,leng_no);
	if _oT(vp) vp->mode=0;
	ptvar_replace(_pt_kill,vp);
	if _oF(run_script(code,env)) clr_error();
	var_free(code);
	var_free(env);
	if _oF(name)
	{
		lock_alloc(lock_thread);
		var_delete(_thread_thid,name);
		lock_free(lock_thread);
		free(name);
	}
	ptvar_free(_pt_this);
	ptvar_free(_pt_text);
	ptvar_free(_pt_temp);
	ptvar_free(_pt_sbuf);
	ptvar_free(_pt_kill);
	ptvar_free(_pt_error);
	ptvar_free(_pt_buerr);
	thread_pthid_free(pthid,1);
	return NULL;
}

var* thread_create(char *thread_name, var *code, var *env, u64 *pthid)
{
	static char *label="thread_create";
	thread_args *args;
	size_t id;
	int size;
	if _oF(thread_name && !vname_check(thread_name)) return get_error(errid_GraQuoVarname,label);
	args=malloc(sizeof(thread_args));
	if _oF(!args) return get_error(errid_MemLess,label);
	if _oF(thread_name)
	{
		size=strlen(thread_name)+1;
		args->name=malloc(size);
		if _oF(!args->name) return get_error(errid_MemLess,label);
		memcpy(args->name,thread_name,size);
	}
	else args->name=NULL;
	var_save(code);
	args->code=code;
	var_save(env);
	args->env=env;
	if _oF(pthread_create(&id,attr,thread_handle,args))
	{
		free(args);
		return get_error(errid_MemLess,label);
	}
	*pthid=id;
	for(size=0;!thread_pthid_test(id) && size<LOCK_LOOP;size++) usleep(LOCK_USLEEP);
	thread_pthid_free(id,2);
	return NULL;
}

void thread_wait(u64 pthid)
{
	while(thread_pthid_test(pthid))
		usleep(LOCK_USLEEP);
}

void thread_waitall(void)
{
	u32 r;
	while(1)
	{
		lock_alloc(lock_pthid);
		r=pthid_vm->number;
		lock_free(lock_pthid);
		if (r) usleep(LOCK_USLEEP);
		else return ;
	}
}

int thread_getid(char *thread_name, u64 *pthid)
{
	var *vp;
	lock_alloc(lock_thread);
	if _oT(vp=var_find(_thread_thid,thread_name))
	{
		if _oT(pthid) *pthid=vp->v.v_long;
	}
	lock_free(lock_thread);
	return vp?0:-1;
}

int thread_kill(u64 pthid)
{
	vlist *vl;
	var *vp=NULL;
	lock_alloc(lock_pthid);
	vl=vmat_find_index(pthid_vm,pthid);
	if _oT(vl && !(vl->mode&1))
	{
		lock_alloc(lock_ptvar);
		if _oT(_pt_kill->type&type_vmat) vl=_pt_kill->v.v_vmat->avl[vhead_gen(pthid)&_pt_kill->v.v_vmat->mask];
		else if _oT(_pt_kill->type&type_vlist) vl=_pt_kill->v.v_vlist;
		else goto End;
		if _oF(!vl) goto End;
		if _oT(vl->head<pthid) while(vl->r&&vl->r->head<=pthid) vl=vl->r;
		else if _oT(vl->head>pthid) while(vl->l&&vl->l->head>=pthid) vl=vl->l;
		if _oT(vl->head==pthid) vp=vl->v;
		End:
		lock_free(lock_ptvar);
		if _oT(vp) vp->mode=1;
	}
	lock_free(lock_pthid);
	return vp?0:-1;
}

void thattr_detach_state(int detach)
{
	lock_alloc(lock_thread);
	if _oT(detach)
	{
		_thattr_detach_state->v.v_int=1;
		pthread_attr_setdetachstate(attr,PTHREAD_CREATE_DETACHED);
	}
	else
	{
		_thattr_detach_state->v.v_int=0;
		pthread_attr_setdetachstate(attr,PTHREAD_CREATE_JOINABLE);
	}
	lock_free(lock_thread);
}

