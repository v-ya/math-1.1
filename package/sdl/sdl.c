#include "sdl.h"

char *author	=AUTHOR;
char *package	=PACKAGE;

var *_Windows;
vmat *_Windows_vmat;
var *_Surface;
vmat *_Surface_vmat;

SDL_PixelFormat *_PixelFormat;
u32 _WindowFormat=SDL_PIXELFORMAT_BGRA32;

void init(void)
{
	_Windows=base->create_vmat(root,__Windows__,0,auth_read|auth_system);
	if _oF(!_Windows)
	{
		throw_errid(errid_MemLess);
		return ;
	}
	_Windows_vmat=_Windows->v.v_vmat;
	_Surface=base->create_vmat(root,__Surface__,0,auth_read|auth_system);
	if _oF(!_Surface)
	{
		throw_errid(errid_MemLess);
		return ;
	}
	_Surface_vmat=_Surface->v.v_vmat;
	_PixelFormat=SDL_AllocFormat(_WindowFormat);
	if _oF(!_PixelFormat)
	{
		throw_errid(errid_MemLess);
		return ;
	}
	if _oF(SDL_Init(_INIT_FLAGES)) throw_error("初始化失败","SDL_Init");
}

void uini(void)
{
	base->var_delete(root,__Windows__);
	base->var_delete(root,__Surface__);
	SDL_FreeFormat(_PixelFormat);
	SDL_Quit();
}

