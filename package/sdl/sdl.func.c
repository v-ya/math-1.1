#include "sdl.h"

#define	label_name(n) "[system.sdl]." n

func(CreateWindow)
{
	static char *label=label_name("CreateWindow");
	static u32 type_6[6]={type_string,type_znum,type_znum,type_znum,type_znum,type_znum};
	char *title;
	int x,y,w,h;
	u32 flags;
	SDL_Window *pwindow;
	
	if _oF(argc!=6) return base->get_error(errid_FunArgvType,label);
	else if _oF(base->check_varlist(argv,6,type_6)) return base->get_error(errid_FunArgvType,label);
	title=argv->v->v.v_string;
	argv=argv->r;
	x=argv->v->v.v_int;
	argv=argv->r;
	y=argv->v->v.v_int;
	argv=argv->r;
	w=argv->v->v.v_int;
	argv=argv->r;
	h=argv->v->v.v_int;
	argv=argv->r;
	flags=argv->v->v.v_int;
	pwindow=SDL_CreateWindow(title,x,y,w,h,flags);
	
	ret->type=type_long|type_unsign;
	ret->v.v_long=new_window(pwindow);
	return ret;
}

func(UpdateWindow)
{
	static char *label=label_name("UpdateWindow");
	static u32 type_1[1]={type_znum};
	SDL_Window *pwindow;
	
	if _oF(argc!=1) return base->get_error(errid_FunArgvType,label);
	else if _oF(base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	pwindow=getWindow(argv->v->v.v_long);
	
	if _oT(pwindow) SDL_UpdateWindowSurface(pwindow);
	
	ret->type=type_void;
	ret->v.v_long=0;
	return ret;
}

