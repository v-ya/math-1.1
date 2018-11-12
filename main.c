#include "main.h"

char* get_exp(char *path)
{
	FILE *fp=fopen(path,"r");
	int size,i;
	char* exp;
	
	if (!fp) goto err;
	fseek(fp,0,2);
	size=ftell(fp);
	exp=malloc(size+1);
	if (!exp) goto err;
	fseek(fp,0,0);
	for(i=0;i<size;i++)
	{
		exp[i]=getc(fp);
	}
	exp[size]=0;
	fclose(fp);
	return exp;
	err:
	if (fp) fclose(fp);
	return NULL;
}

char* init_args_copynew(char *str)
{
	int n;
	char *sn;
	n=strlen(str)+1;
	sn=malloc(n);
	if (sn) memcpy(sn,str,n);
	return sn;
}

int init_args(int argc, char *argv[])
{
	var *args,*vp;
	value v;
	int i;
	args=var_replace(_vm_sysm,"args",tlog_vlist,leng_no);
	args->mode=auth_read|free_pointer;
	if (!args) return 1;
	v.v_string=init_args_copynew(argv[0]);
	vp=var_set(args,"exec",tlog_string,leng_no,auth_read|auth_write|free_pointer,&v);
	if (!vp) return 1;
	v.v_string=init_args_copynew(argv[1]);
	vp=var_set(args,"script",tlog_string,leng_no,auth_read|auth_write|free_pointer,&v);
	if (!vp) return 1;
	if (argc>=2)
	{
		vp=var_replace(args,"argv",tlog_string,argc-2);
		if (!vp) return 1;
		vp->mode=auth_read|auth_write|auth_relength|free_pointer;
		for(i=2;i<argc;i++)
		{
			vp->v.vp_string[i-2]=init_args_copynew(argv[i]);
		}
	}
	return 0;
}

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
	char *exp;
	var pt_text={0};
	if (argc<2)
	{
		printf("%s v%d.%d.%d\n\t%s <script-file>\n",argv[0],VERSION_major,VERSION_minor,VERSION_revision,argv[0]);
		return 0;
	}
	exp=get_exp(argv[1]);
	if (!exp)
	{
		printf("error: 文件 %s 打开失败!\n",argv[1]);
		return 0;
	}
	
	pt_text.inode=1;
	pt_text.type=type_string;
	pt_text.mode=auth_read|free_temp;
	pt_text.v.v_string=exp;
	
	if (init_args(argc,argv)) get_error(errid_MemLess,"init_args");
	else 
	{
		if _oF(init_run()) printf("error: init.math 初始化失败\n");
		else run_script(&pt_text,_vm_user);
	}
	print_error();
	clr_error();
	
	free(exp);
	return 0;
}



