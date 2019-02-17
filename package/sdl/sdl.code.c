#include "sdl.h"

u64 new_window(SDL_Window *w)
{
	u64 wid;
	var *vp;
	SDL_Surface *s;
	
	if _oF(!w) return 0;
	do
	{
		wid=base->get_sid();
	}
	while(base->vmat_find_index(_Windows_vmat,wid));
	
	s=SDL_GetWindowSurface(w);
	vp=base->create_vmat(_Windows,NULL,wid,auth_read);
	if _oF(!vp) goto Err;
	if _oF(!base->create_void(vp,__pWindow__,0,auth_read,w)) goto Err;
	if _oF(!base->create_void(vp,__pSurface__,0,auth_read,s)) goto Err;
	
	if _oF(!base->create_string(vp,__title__,0,auth_read,(char *)SDL_GetWindowTitle(w))) goto Err;
	if _oF(!base->create_sint(vp,__width__,0,auth_read,s->w)) goto Err;
	if _oF(!base->create_sint(vp,__height__,0,auth_read,s->h)) goto Err;
	if _oF(!(vp=base->create_uint(vp,__pixels__,0,auth_read|auth_write,0))) goto Err;
	vp->v.v_void=s->pixels;
	vp->length=s->w*s->h;
	return wid;
	
	Err:
	base->vmat_delete_index(_Windows_vmat,wid);
	return 0;
}

SDL_Window* getWindow(u64 wid)
{
	var *vp;
	if _oT(vp=base->var_find_index(_Windows,wid))
	{
		vp=base->var_find(vp,__pWindow__);
		return vp?vp->v.v_void:NULL;
	}
	else return NULL;
}

