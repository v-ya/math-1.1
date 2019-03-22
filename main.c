#include "main.h"
#include "args/args.h"

int init_run(void)
{
	vlist *vl;
	var *vp;
	// 初始化变量
	if _oF(!(_refpool->type&type_vmat) || !_refpool->v.v_vmat) return 1;
	_refpool_vmat=_refpool->v.v_vmat;
	// init 脚本
	vl=vlist_alloc("init");
	if _oF(!vl)
	{
		get_error(errid_MemLess,"init_run");
		return 1;
	}
	vlist_link(vl,_vl_init);
	vmat_insert(_vm_sysm->v.v_vmat,vl);
	vp=run_script(_init_text,_vm_sysm);
	vlist_link(vl,NULL);
	var_delete(_vm_sysm,"init");
	if _oF(vp) return 1;
	else return 0;
}

int main(int argc, char *argv[])
{
	var *pt_text;
	
	pt_text=init_args(argc,argv);
	if (pt_text)
	{
		if (thread_init()) get_error(errid_IntThreadInit,"thread_init");
		else if _oF(init_run()) get_error(errid_IntInitScript,"init_run");
		else run_script(pt_text,_vm_user);
	}
	
	print_error();
	clr_error();
	
	if (pt_text)
	{
		thread_waitall();
		thread_uini();
	}
	uini_args();
	return 0;
}



