#ifndef	_args_h_
#define	_args_h_

#define S_args		"args"
#define S_argc		"argc"
#define S_argv		"argv"
#define S_exec		"exec"
#define S_text		"text"
#define S_script	"script"
#define S_help		"help"
#define S_sysargv	"_argv_"

extern var *_SCRIPT_ROOT;
extern var *_SCRIPT_NAME;
extern var *_SCRIPT_ALISA;
extern var *_SCRIPT_NotFind;
extern var *_HELP_ROOT;

#ifndef __nosym__

var* init_args(int argc, char *argv[]);
void uini_args(void);
void format(char *f);

#endif

#endif

