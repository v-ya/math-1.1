#include "../main.h"

func(init_auth)
{
	static char *label=".init.auth";
	static u32 mask=~(auth_retype|auth_read|auth_write|auth_link|auth_relength|auth_system);
	var *vp;
	char *sa;
	u32 isplus=1,mode;
	if _oF(argc<1) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	argv=argv->r;
	if _oF(!(vp->type&type_string)) return get_error(errid_FunArgvType,label);
	sa=vp->v.v_string;
	ret->type=type_void;
	if _oF(!sa) return ret;
	mode=0;
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
	while(argv)
	{
		vp=argv->v;
		argv=argv->r;
		vp->mode=vp->mode&mask|mode;
	}
	return ret;
}

