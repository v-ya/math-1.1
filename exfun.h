#ifndef	_exfun_h_
#define	_exfun_h_

#ifndef __nosym__

s32 type_check(var *obj, char *type);
var* type_set(var *obj, var *type);
var* type_empty(var *obj);
u64 get_sid(void);

#endif

#endif

