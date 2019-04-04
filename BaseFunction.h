#ifndef	_BaseFunction_h_
#define	_BaseFunction_h_

#define InterfaceBase_Version	5

typedef struct Interface_Base {
	// version
	u32	version;
	// string.h
	u32	(*vname_check)		(char *name);
	u32	(*vname_gen)		(unsigned char *name);
	u32	(*vhead_gen)		(u64 head);
	u64	(*vname_head)		(unsigned char *name);
	char*	(*skip_string)		(char *exp);
	char*	(*get_string)		(char *exp, char **pexp);
	u64	(*get_ascii)		(char *exp, char **pexp);
	char*	(*skip_code)		(char *exp);
	char*	(*get_code)		(char *exp, char **pexp);
	char*	(*compress_code)	(char *s);
	char*	(*get_nextcol)		(char *exp);
	char*	(*get_nextsem)		(char *exp);
	char*	(*skip_string_last)	(char *exp, char *text);
	char*	(*skip_code_last)	(char *exp, char *text);
	char*	(*get_lastsem)		(char *exp, char *text);
	u32	(*cmp_label)		(char *exp, char *label);
	char*	(*skip_note)		(char *exp);
	// var.h
	var*	(*get_null)		(void);
	vmat*	(*vmat_alloc)		(void);
	void	(*vmat_free)		(vmat *vm);
	vlist*	(*vlist_alloc)		(char *name);
	vlist*	(*vlist_alloc_index)	(u64 head);
	void	(*vlist_free)		(vlist *vl);
	var*	(*var_alloc)		(u32 tlog, u32 length);
	void	(*var_free)		(var *vp);
	vlist*	(*vlist_insert)		(vlist *vl, vlist *v);
	vlist*	(*vlist_find)		(vlist *vl, char *name);
	vlist*	(*vlist_find_index)	(vlist *vl, u64 head);
	vlist*	(*vlist_delete)		(vlist *vl, char *name);
	vlist*	(*vlist_delete_index)	(vlist *vl, u64 head);
	void	(*vmat_insert)		(vmat *vm, vlist *v);
	vlist*	(*vmat_find)		(vmat *vm, char *name);
	vlist*	(*vmat_find_index)	(vmat *vm, u64 head);
	void	(*vmat_delete)		(vmat *vm, char *name);
	void	(*vmat_delete_index)	(vmat *vm, u64 head);
	void	(*var_save)		(var *vp);
	void	(*vlist_link)		(vlist *vl, var *v);
	vlist*	(*v_find)		(var *obj, char *name);
	vlist*	(*v_find_index)		(var *obj, u64 head);
	var*	(*var_find)		(var *obj, char *name);
	var*	(*var_find_index)	(var *obj, u64 head);
	vlist*	(*var_insert)		(var *obj, char *name, u32 tlog, u32 length);
	void	(*var_delete)		(var *obj, char *name);
	void	(*var_delete_index)	(var *obj, u64 head);
	var*	(*var_replace)		(var *obj, char *name, u32 tlog, u32 length);
	var*	(*var_set)		(var *obj, char *name, u32 tlog, u32 length, u32 mode, value *v);
	var*	(*var_link)		(var *obj, char *name, var *v);
	var*	(*var_link_index)	(var *obj, u64 head, var *v);
	void	(*get_tmpvar)		(char *exp, char **expp, var *v);
	void	(*var_fixvalue)		(var *vp);
	void	(*ptvar_alloc)		(var *object);
	void	(*ptvar_free)		(var *object);
	var*	(*ptvar_get)		(var *object);
	vlist*	(*ptvar_vlist)		(var *object);
	var*	(*ptvar_replace)	(var *object, var *value);
	void	(*refer_alloc)		(u64 id);
	void	(*refer_free)		(u64 id);
	vlist*	(*refer_get)		(u64 id);
	void	(*refer_set)		(var *rp, var *vp);
	void	(*refer_unset)		(var *rp);
	var*	(*refer_check)		(var *rp);
	// error.h
	var*	(*get_error)		(u32 errid, char *label);
	void	(*set_error)		(var *pt_text, char *script);
	void	(*clr_error)		(void);
	char*	(*str_error_cl)		(u32 errid);
	char*	(*str_error_ty)		(u32 errid);
	char*	(*str_error_id)		(u32 errid);
	void	(*print_error)		(void);
	// sbuf.h
	var*	(*sbuf_alloc)		(void);
	void	(*sbuf_free)		(void);
	var*	(*sbuf_get)		(void);
	var*	(*sbuf_expand)		(void);
	var*	(*sbuf_reduce)		(void);
	var*	(*sbuf_sprintf)		(char *format, vlist *vl);
	char*	(*get_name)		(char *exp, char **pexp);
	// file.h
	char*	(*get_path)		(char *path_list, char *path_short);
	char*	(*load_string)		(char *path);
	var*	(*store_data)		(char *path, var *obj, u32 isfast);
	var*	(*load_data)		(var *obj, char *path);
	// math.h
	var*	(*temp_get)		(void);
	void	(*temp_free)		(void);
	var*	(*get_varlist)		(char **expp, int *argc, vlist **argv);
	int	(*check_varlist)	(vlist *vl, u32 argc, u32 type[]);
	char*	(*run_key)		(char *exp, var *root, var **function);
	var*	(*run_fun_vlist)	(var *func, int argc, vlist *argv);
	char*	(*run_fun)		(char *exp, var *root, var **function);
	var*	(*get_var)		(char *exp, char **expp, int *array_n);
	var*	(*cal)			(char *exp, char **expp);
	s64	(*get_int)		(char* exp, char **expp, var **vpp_err);
	double	(*get_float)		(char* exp, char **expp, var **vpp_err);
	var*	(*run_script)		(var *pt_text, var *pt_this);
	// exfun.h
	var*	(*create_var)		(var *obj, char *name, u64 head, u32 tlog, u32 length, u32 auth);
	void	(*remove_var)		(var *obj, char *name, u64 head);
	var*	(*create_void)		(var *obj, char *name, u64 head, u32 auth, void *value);
	var*	(*create_ubyte)		(var *obj, char *name, u64 head, u32 auth, u8 value);
	var*	(*create_sbyte)		(var *obj, char *name, u64 head, u32 auth, s8 value);
	var*	(*create_uword)		(var *obj, char *name, u64 head, u32 auth, u16 value);
	var*	(*create_sword)		(var *obj, char *name, u64 head, u32 auth, s16 value);
	var*	(*create_uint)		(var *obj, char *name, u64 head, u32 auth, u32 value);
	var*	(*create_sint)		(var *obj, char *name, u64 head, u32 auth, s32 value);
	var*	(*create_ulong)		(var *obj, char *name, u64 head, u32 auth, u64 value);
	var*	(*create_slong)		(var *obj, char *name, u64 head, u32 auth, s64 value);
	var*	(*create_float)		(var *obj, char *name, u64 head, u32 auth, double value);
	var*	(*create_string)	(var *obj, char *name, u64 head, u32 auth, char *value);
	var*	(*create_vlist)		(var *obj, char *name, u64 head, u32 auth);
	var*	(*create_vmat)		(var *obj, char *name, u64 head, u32 auth);
	var*	(*create_refer)		(var *obj, char *name, u64 head, u32 auth, var *value);
	u64	(*get_sid)		(void);
	void	(*clear_vmsrc)		(var *root, char *name, FreeSrcFunc f);
	
} interface_base;

#ifndef __nosym__

extern interface_base BaseFunction;

#endif

#endif

