#ifndef	_func_thread_h_
#define	_func_thread_h_


#ifndef __nosym__

func(thread_self);
func(thread_create);
func(thread_kill);
func(thread_wait);
func(lock_create);
func(sleep);
func(msleep);
func(usleep);
// set attr
func(thattr_detach_state);
func(thattr_sched_policy);
func(thattr_sched_param);
func(thattr_inheritance);
func(thattr_scope);
func(thattr_guard_size);
func(thattr_stack_addr);
func(thattr_stack_size);
func(thattr_stack);

#endif

#endif

