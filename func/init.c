#include "../main.h"

func(init_auth)
{
	static char *label=".init.auth";
	static u32 type_2[2]={type_all|type_allowarray,type_string};
	var *vp;
	char *sa;
	u32 isplus=1,mode;
	if _oF(argc!=2) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	sa=argv->r->v->v.v_string;
	ret->type=type_void;
	if _oF(!sa) return ret;
	mode=vp->mode&~(auth_all^auth_run^auth_key);
	while(*sa)
	{
		switch(*sa)
		{
			case 's':
				mode|=auth_retype;
				break;
			case 'r':
				mode|=auth_read;
				break;
			case 'w':
				mode|=auth_write;
				break;
			case 'l':
				mode|=auth_link;
				break;
			case 'S':
				mode|=auth_relength;
				break;
			case 'c':
				mode|=auth_system;
				break;
		}
		sa++;
	}
	vp->mode=mode;
	return ret;
}

