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

int init_args(int argc, char *argv[])
{
	var *args,*vp;
	value v;
	int i;
	args=var_replace(_vm_sysm,"args",tlog_vlist,leng_no);
	args->mode=auth_read|free_pointer;
	if (!args) return 1;
	v.v_string=argv[0];
	vp=var_set(args,"exec",tlog_string,leng_no,auth_read,&v);
	if (!vp) return 1;
	v.v_string=argv[1];
	vp=var_set(args,"script",tlog_string,leng_no,auth_read,&v);
	if (!vp) return 1;
	if (argc>2)
	{
		vp=var_replace(args,"argv",tlog_string,argc-2);
		if (!vp) return 1;
		vp->mode=auth_read;
		v.v_void=vp->v.v_void;
		if (!var_set(args,"_argv_",tlog_void,leng_no,free_pointer,&v))
		{
			vp->mode|=free_pointer;
			return 1;
		}
		for(i=2;i<argc;i++)
		{
			vp->v.vp_string[i-2]=argv[i];
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	char *exp;
	var pt_text={0};
	if (argc<2) return 0;
	exp=get_exp(argv[1]);
	if (!exp)
	{
		printf("error: 文件 %s 打开失败!\n",argv[1]);
		return 0;
	}
	
	pt_text.inode=1;
	pt_text.type=type_string;
	pt_text.mode=auth_tmpvar;
	pt_text.v.v_string=exp;
	
	if (init_args(argc,argv)) get_error(errid_MemLess,"init_args");
	else run_script(&pt_text,_vm_user);
	print_error();
	clr_error();
	
	free(exp);
	return 0;
}



