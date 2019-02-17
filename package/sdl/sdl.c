#include "sdl.h"

char *author	=AUTHOR;
char *package	=PACKAGE;

var *_Windows;
vmat *_Windows_vmat;

void init(void)
{
	_Windows=base->create_vmat(root,__Windows__,0,auth_read|auth_system);
	if _oF(!_Windows)
	{
		throw_errid(errid_MemLess);
		return ;
	}
	_Windows_vmat=_Windows->v.v_vmat;
	if _oF(SDL_Init(_INIT_FLAGES)) throw_error("初始化失败","SDL_Init");
}

void uini(void)
{
	base->var_delete(root,__Windows__);
	SDL_Quit();
}

