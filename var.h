#ifndef	_var_h_
#define	_var_h_

union VALUE;
struct VAR;
struct VLIST;
struct VMAT;

// tlog
#define tlog_void	0
#define tlog_byte	1
#define tlog_word	2
#define tlog_int	3
#define tlog_long	4
#define tlog_float	5
#define tlog_string	6
#define tlog_vlist	7
#define tlog_vmat	8
#define tlog_refer	9

#define tlog_data_max	10
#define tlog_max	10

// refer var max number
#define refer_max	0xffff
// type normal
#define type_tlog(t)	(1<<t)
#define type_null	0x00800000
#define type_void	0x00000001
#define type_byte	0x00000002
#define type_word	0x00000004
#define type_int	0x00000008
#define type_long	0x00000010
#define type_float	0x00000020
#define type_string	0x00000040
#define type_vlist	0x00000080
#define type_vmat	0x00000100
#define type_refer	0x00000200
// unsigned type, only used by byte, word, int, long
#define type_unsign	0x01000000
// only used at check_varlist
#define type_allowarray	0x01000000
#define type_onlyarray	0x02000000
// type class
#define type_all	0x00ffffff
#define type_argvall	0x0fffffff
#define type_znum	0x0000001e
#define type_num	0x0000003e
#define type_snum	0x0000007e
#define type_object	0x00000180
// type system
#define	type_err	0x80000000
#define	type_end	0x40000000

#define type_spe	0xc0000000

// auth
#define	auth_retype	0x00000001
#define	auth_read	0x00000002
#define	auth_write	0x00000004
#define auth_link	0x00000008
#define auth_run	0x00000010
#define auth_key	0x00000020
#define auth_relength	0x00000040
#define auth_system	0x00008000

#define auth_all	0x0000ffff
#define auth_normal	0x0000004f
#define auth_tmpvar	0x00000002
#define auth_sysfun	0x00000012
#define auth_syskey	0x00000022
#define auth_none	0x00000000

// free
#define free_pointer	0x80000000
#define free_name	0x40000000
// var_free 忽略
#define free_temp	0x20000000
// refer set
#define is_refer	0x10000000

// length
#define leng_no		0

// vmat info
#define vmat_init_size	16

#define vmat_flag_fvmat	0x00000001
#define vmat_flag_favl	0x00000002
#define vmat_flag_notex	0x00000004

#define vmat_flag_free	0x00000003

#define vpntof(vp)	(((vp)->type&type_znum)?((vp)->v.v_long):((vp)->v.v_float))
#define vpbool(vp)	(((vp)->type&(type_void|type_null))?0:(((vp)->type&type_float)?(((vp)->v.v_float!=0)?1:0):(((vp)->v.v_long)?1:0)))
#define isstring(vp)	(((vp)->type&type_string)&&(!(vp)->length))

#define _kw_(p,n)	var* p##n(var *call, char **expp)
#define _func_(p,n)	var* p##n(var *ret, int argc, vlist *argv)

typedef struct VAR* (*key) (struct VAR *, char **);
typedef struct VAR* (*fun) (struct VAR *, int, struct VLIST *);

struct ERROR_INFO;

typedef union VALUE {
	// leng_no
	void *v_void;
	s8 v_byte;
	s16 v_word;
	s32 v_int;
	s64 v_long;
	double v_float;
	char *v_string;
	struct VLIST *v_vlist;
	struct VMAT *v_vmat;
	// array
	void **vp_void;
	s8 *vp_byte;
	s16 *vp_word;
	s32 *vp_int;
	s64 *vp_long;
	double *vp_float;
	char **vp_string;
	struct VLIST **vp_vlist;
	struct VMAT **vp_vmat;
	// special
	key v_key;
	fun v_fun;
	struct VAR *v_var;
	struct ERROR_INFO *v_error;
} value;

typedef struct VAR {
	u32 type;
	u32 length;
	u32 inode;
	u32 mode;
	union VALUE v;
} var;

typedef struct VLIST {
	char* name;
	u32 mode;
	// 快速比较位 head: name[0-7] | 变量组索引位 index
	u64 head;
	struct VAR *v;
	struct VLIST *l;
	struct VLIST *r;
} vlist;

typedef struct VMAT {
	u32 mask;
	u32 number;
	u64 flag;
	struct VLIST **avl;
} vmat;

#ifndef __nosym__
// static var
extern var *_var_null;
extern vmat *_refpool_vmat;
var* get_null(void);
// alloc && free
vmat* vmat_alloc(void);
void vmat_free(vmat *vm);
vlist* vlist_alloc(char *name);
vlist* vlist_alloc_index(u64 head);
void vlist_free(vlist *vl);
var* var_alloc(u32 tlog, u32 length);
void var_free(var *vp);
// insert && find && delete
vlist* vlist_insert(vlist *vl, vlist *v);
vlist* vlist_find(vlist *vl, char *name);
vlist* vlist_find_index(vlist *vl, u64 head);
vlist* vlist_delete(vlist *vl, char *name);
vlist* vlist_delete_index(vlist *vl, u64 head);
void vmat_insert(vmat *vm, vlist *v);
vlist* vmat_find(vmat *vm, char *name);
vlist* vmat_find_index(vmat *vm, u64 head);
void vmat_delete(vmat *vm, char *name);
void vmat_delete_index(vmat *vm, u64 head);
// vlist && var 's fun
void var_save(var *vp);
void vlist_link(vlist *vl, var *v);
vlist* v_find(var *obj, char *name);
vlist* v_find_index(var *obj, u64 head);
var* var_find(var *obj, char *name);
var* var_find_index(var *obj, u64 head);
vlist* var_insert(var *obj, char *name, u32 tlog, u32 length);
void var_delete(var *obj, char *name);
void var_delete_index(var *obj, u64 head);
var* var_replace(var *obj, char *name, u32 tlog, u32 length);
var* var_set(var *obj, char *name, u32 tlog, u32 length, u32 mode, value *v);
var* var_link(var *obj, char *name, var *v);
var* var_link_index(var *obj, u64 head, var *v);
void get_tmpvar(char *exp, char **expp, var *v);

void var_fixvalue(var *vp);

// ptvar
void ptvar_alloc(var *object);
void ptvar_free(var *object);
var* ptvar_get(var *object);
vlist* ptvar_vlist(var *object);
var* ptvar_replace(var *object, var *value);
// refer
void refer_alloc(u64 id);
void refer_free(u64 id);
vlist* refer_get(u64 id);
void refer_set(var *rp, var *vp);
void refer_unset(var *rp);
var* refer_check(var *rp);

#endif

#endif

