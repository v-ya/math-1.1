#include "../main.h"
#include "args.h"

var* init_args(int argc, char *argv[])
{
	static char *label="init_args";
	var *args, *vc, *vp;
	vlist *vl;
	u64 i;
	char *script;
	
	// mount {script}, {help} => [.sys.args].*
	if _oF(!(vl=vlist_alloc(S_script))) goto Err;
	vl->v=_SCRIPT_ROOT;
	_args_list->v.v_vlist=vlist_insert(_args_list->v.v_vlist,vl);
	if _oF(!(vl=vlist_alloc(S_help))) goto Err;
	vl->v=_HELP_ROOT;
	_args_list->v.v_vlist=vlist_insert(_args_list->v.v_vlist,vl);
	
	// .args
	args=create_vmat(_vm_sysm,S_args,0,auth_read|auth_write);
	if _oF(!args) goto Err;
	// .args.exec
	if _oF(!create_string(args,S_exec,0,auth_read,argv[0])) goto Err;
	// .args._argv_[] ~[tmp]
	vc=create_ulong(args,S_argc,0,auth_read|auth_write,0);
	if _oF(!vc) goto Err;
	
	// format run script
	vp=create_var(args,S_sysargv,0,tlog_string,argc,auth_read);
	if _oF(!vp) goto Err;
	for(i=0;i<argc;i++)
		vp->v.vp_string[i]=argv[i];
	for(i=1;i<argc;)
	{
		if _oT(argv[i][0]=='-')
		{
			vc->v.v_long=i+1;
			format(argv[i]);
			i=vc->v.v_long;
		}
		else break;
	}
	memset(vp->v.vp_string,0,argc*sizeof(char*));
	remove_var(args,S_sysargv,0);
	
	// save script
	if _oT(i<argc) script=argv[i++];
	else
	{
		if _oF(argc==1) format("--help");
		return NULL;
	}
	// set .args.argc && build .args.argv
	vc->v.v_long=argc-i;
	argv=&argv[i];
	argc-=i;
	vc=create_vmat(args,S_argv,0,auth_read|auth_write);
	if _oF(!vc) goto Err;
	for(i=0;i<argc;i++)
	{
		if _oF(!create_string(vc,NULL,i,auth_normal,*argv));
		argv++;
	}
	
	// .args.script
	vc=create_string(args,S_argc,0,auth_read|auth_write,script);
	if _oF(!vc) goto Err;;
	
	// .args.text
	vp=create_string(args,S_text,0,auth_read,NULL);
	if _oF(!vp) goto Err;
	
	// load script
	script=load_string(script);
	if _oF(!script)
	{
		get_error(errid_SysFileNotLoad,label);
		set_error(vc,vc->v.v_string);
		return NULL;
	}
	
	vp->mode|=free_pointer;
	vp->v.v_string=script;
	
	return vp;
	Err:
	get_error(errid_MemLess,label);
	return NULL;
}

void uini_args(void)
{
	var_delete(_vm_sysm,S_args);
}

void format(char *f)
{
	var *vp;
	if _oT(*f=='-')
	{
		f++;
		if _oF(*f=='-') vp=var_find(_SCRIPT_NAME,++f);
		else vp=var_find(_SCRIPT_ALISA,f);
		if _oF(!vp) vp=_SCRIPT_NotFind;
		if _oF(run_script(vp,_vm_sysm));
	}
}


