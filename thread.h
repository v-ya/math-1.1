#ifndef	_thread_h_
#define	_thread_h_

// lock id
enum {
	lock_user,
	lock_thread,
	lock_pthid,
	lock_ptvar,
	lock_inode,
	lock_vlist,
	lock_vmatex,
	lock_refer,
	lock_sid,
	
	lock_NUMBER
} ;

typedef struct {
	char *name;
	var *code;
	var *env;
} thread_args;

#define LOCK_LOOP	1000
#define LOCK_USLEEP	1000

#ifndef __nosym__

int thread_init(void);
void thread_uini(void);
void lock_alloc(int id);
void lock_free(int id);
void lock_alloc_user(var *lock);
void lock_free_user(var *lock);
var* thread_create(char *thread_name, var *code, var *env, u64 *pthid);
void thread_wait(u64 pthid);
void thread_waitall(void);
int thread_getid(char *thread_name, u64 *pthid);
int thread_kill(u64 pthid);
// set attr
int thattr_detach_state(int detach);
int thattr_sched_policy(int policy);
int thattr_sched_param(int param);
int thattr_inheritance(int explicit);
int thattr_scope(int scope);
int thattr_guard_size(u64 guard_size);
int thattr_stack_addr(u64 stack_addr);
int thattr_stack_size(u64 stack_size);
int thattr_stack(u64 stack_addr, u64 stack_size);

#endif

#endif

