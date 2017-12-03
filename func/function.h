#ifndef	_func_function_h_
#define	_func_function_h_


#ifndef __nosym__

u32 get_tlog(char *type);
var* var_define(char *exp, char **expp, var *root);
var* function_define(char *exp, char **expp, var **_vt_, var **_vn_);
char* goto_exp(char *text, char *label);
char* goto_exp_last(char *this, char *text, char *label);

#endif

#endif

