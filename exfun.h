#ifndef	_exfun_h_
#define	_exfun_h_

typedef void (*FreeSrcFunc)(var *);

#ifndef __nosym__


int type_check(var *obj, char *type);
var* type_set(var *obj, var *type);
var* type_set_auth(var *obj, var *type);
int type_isempty(var *obj);
int object_isempty(var *obj);
var* create_var(var *obj, char *name, u64 head, u32 tlog, u32 length, u32 auth);
void remove_var(var *obj, char *name, u64 head);
var* create_void(var *obj, char *name, u64 head, u32 auth, void *value);
var* create_ubyte(var *obj, char *name, u64 head, u32 auth, u8 value);
var* create_sbyte(var *obj, char *name, u64 head, u32 auth, s8 value);
var* create_uword(var *obj, char *name, u64 head, u32 auth, u16 value);
var* create_sword(var *obj, char *name, u64 head, u32 auth, s16 value);
var* create_uint(var *obj, char *name, u64 head, u32 auth, u32 value);
var* create_sint(var *obj, char *name, u64 head, u32 auth, s32 value);
var* create_ulong(var *obj, char *name, u64 head, u32 auth, u64 value);
var* create_slong(var *obj, char *name, u64 head, u32 auth, s64 value);
var* create_float(var *obj, char *name, u64 head, u32 auth, double value);
var* create_string(var *obj, char *name, u64 head, u32 auth, char *value);
var* create_vlist(var *obj, char *name, u64 head, u32 auth);
var* create_vmat(var *obj, char *name, u64 head, u32 auth);
var* create_refer(var *obj, char *name, u64 head, u32 auth, var *value);
u64 get_sid(void);
void clear_vmsrc(var *root, char *name, FreeSrcFunc f);

#endif

#endif

