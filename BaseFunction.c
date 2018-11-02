#include "main.h"

interface_base BaseFunction = {
	// string.h
	&vname_check,
	&vname_gen,
	&vhead_gen,
	&vname_head,
	&skip_string,
	&get_string,
	&get_ascii,
	&skip_code,
	&get_code,
	&compress_code,
	&get_nextcol,
	&get_nextsem,
	&skip_string_last,
	&skip_code_last,
	&get_lastsem,
	&cmp_label,
	&skip_note,
	// var.h
	&get_null,
	&vmat_alloc,
	&vmat_free,
	&vlist_alloc,
	&vlist_alloc_index,
	&vlist_free,
	&var_alloc,
	&var_free,
	&vlist_insert,
	&vlist_find,
	&vlist_find_index,
	&vlist_delete,
	&vlist_delete_index,
	&vmat_insert,
	&vmat_find,
	&vmat_find_index,
	&vmat_delete,
	&vmat_delete_index,
	&var_save,
	&vlist_link,
	&v_find,
	&v_find_index,
	&var_find,
	&var_find_index,
	&var_insert,
	&var_delete,
	&var_delete_index,
	&var_replace,
	&var_set,
	&var_link,
	&get_tmpvar,
	&var_fixvalue,
	&ptvar_alloc,
	&ptvar_free,
	&ptvar_get,
	&ptvar_vlist,
	&ptvar_replace,
	// error.h
	&get_error,
	&set_error,
	&clr_error,
	&str_error_cl,
	&str_error_ty,
	&str_error_id,
	&print_error,
	// sbuf.h
	&sbuf_alloc,
	&sbuf_free,
	&sbuf_get,
	&sbuf_expand,
	&sbuf_reduce,
	&sbuf_sprintf,
	&get_name,
	// math.h
	&temp_get,
	&temp_free,
	&get_varlist,
	&check_varlist,
	&run_key,
	&run_fun_vlist,
	&run_fun,
	&get_var,
	&cal,
	&get_int,
	&get_float,
	&run_script,
	// exfun.h
	&type_check,
	&type_set,
	&type_empty,
} ;

