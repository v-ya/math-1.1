#include "gspace.h"

#define	label_name(n) "[vya.gspace]." n

#include "func.program.c"
#include "func.buffer.c"
#include "func.vertexAttributes.c"
#include "func.model.c"
#include "func.draw.c"

func(initWindow)
{
	static char *label=label_name("initWindow");
	static u32 type_2[2]={type_znum,type_znum};
	static u32 type_3[3]={type_string,type_znum,type_znum};
	
	char *title;
	s32 w,h;
	
	switch(argc)
	{
		case 2:
			if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
			title=NULL;
			w=argv->v->v.v_long;
			argv=argv->r;
			h=argv->v->v.v_long;
			break;
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
			title=argv->v->v.v_string;
			argv=argv->r;
			w=argv->v->v.v_long;
			argv=argv->r;
			h=argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
	}
	
	init_window(title,w,h);
	
	ret->type=type_void;
	return ret;
}

func(test)
{
	ret->type = type_void;
	return ret;
}

