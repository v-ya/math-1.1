#ifndef	_math_h_
#define	_math_h_

#define clear_vp(vp) if _oF((vp)->mode&free_pointer) {free((vp)->v.v_void);(vp)->mode&=(~free_pointer);}

#ifndef __nosym__

var* temp_get(void);
void temp_free(void);
var* get_varlist(char **expp, int *argc, vlist **argv);
int check_varlist(vlist *vl, u32 argc, u32 type[]);
char* run_key(char *exp, var *root, var **function);
var* run_fun_vlist(var *func, int argc, vlist *argv);
char* run_fun(char *exp, var *root, var **function);
var* get_var(char *exp, char **expp, int *array_n);
var* cal(char *exp, char **expp);
s64 get_int(char* exp, char **expp, var **vpp_err);
double get_float(char* exp, char **expp, var **vpp_err);
var* run_script(var *pt_text, var *pt_this);

#endif

#endif

