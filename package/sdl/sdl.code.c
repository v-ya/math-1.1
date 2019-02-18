#include "sdl.h"

u64 create_surface(SDL_Surface *s)
{
	u64 sid;
	var *vp;
	
	if _oF(!s) return 0;
	do
	{
		sid=base->get_sid();
	}
	while(base->vmat_find_index(_Surface_vmat,sid));
	
	vp=base->create_vmat(_Surface,NULL,sid,auth_read);
	if _oF(!vp) goto Err;
	if _oF(!base->create_void(vp,__pSurface__,0,auth_read,s)) goto Err;
	
	if _oF(!base->create_sint(vp,__width__,0,auth_read,s->w)) goto Err;
	if _oF(!base->create_sint(vp,__height__,0,auth_read,s->h)) goto Err;
	if _oF(!(vp=base->create_uint(vp,__pixels__,0,auth_read|auth_write,0))) goto Err;
	vp->v.v_void=s->pixels;
	vp->length=s->w*s->h;
	return sid;
	
	Err:
	base->vmat_delete_index(_Surface_vmat,sid);
	return 0;
}

u64 new_surface(SDL_Surface *s)
{
	u64 sid;
	sid=create_surface(s);
	if _oF(!sid && s) SDL_FreeSurface(s);
	return sid;
}

SDL_Surface* getSurface(u64 sid)
{
	var *vp;
	if _oT(vp=base->var_find_index(_Surface,sid))
	{
		vp=base->var_find(vp,__pSurface__);
		return vp?vp->v.v_void:NULL;
	}
	else return NULL;
}

void destroy_surface(u64 sid)
{
	var *vp;
	SDL_Surface *s;
	if _oT(vp=base->var_find_index(_Surface,sid))
	{
		vp=base->var_find(vp,__pSurface__);
		s=(SDL_Surface*)(vp?vp->v.v_void:NULL);
		if _oT(s)
		{
			base->vmat_delete_index(_Surface_vmat,sid);
			SDL_FreeSurface(s);
		}
	}
}

u64 create_window(SDL_Window *w)
{
	u64 wid;
	u64 sid;
	var *vp;
	u32 fmt;
	
	if _oF(!w) return 0;
	do
	{
		wid=base->get_sid();
	}
	while(base->vmat_find_index(_Windows_vmat,wid));
	
	vp=base->create_vmat(_Windows,NULL,wid,auth_read);
	if _oF(!vp) goto Err;
	if _oF(!base->create_void(vp,__pWindow__,0,auth_read,w)) goto Err;
	
	if _oF(!base->create_string(vp,__title__,0,auth_read,(char *)SDL_GetWindowTitle(w))) goto Err;
	sid=create_surface(SDL_GetWindowSurface(w));
	if _oF(!sid) goto Err;
	if _oF(!base->create_ulong(vp,__pSurface__,0,auth_read,sid)) goto Err;
	
	fmt=SDL_GetWindowPixelFormat(w);
	if _oF(fmt!=_WindowFormat)
	{
		SDL_FreeFormat(_PixelFormat);
		_PixelFormat=SDL_AllocFormat(fmt);
		_WindowFormat=fmt;
	}
	
	return wid;
	
	Err:
	base->vmat_delete_index(_Windows_vmat,wid);
	return 0;
}

u64 new_window(SDL_Window *w)
{
	u64 wid;
	wid=create_window(w);
	if _oF(!wid && w) SDL_DestroyWindow(w);
	return wid;
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

void destroy_window(u64 wid)
{
	var *vw,*vp;
	SDL_Window *w;
	u64 sid;
	
	if _oT(vw=base->var_find_index(_Windows,wid))
	{
		vp=base->var_find(vw,__pWindow__);
		w=(SDL_Window*)(vp?vp->v.v_void:NULL);
		vp=base->var_find(vw,__pSurface__);
		sid=vp?vp->v.v_long:0;
		if _oT(w)
		{
			base->vmat_delete_index(_Windows_vmat,wid);
			SDL_DestroyWindow(w);
		}
		if _oT(sid) base->vmat_delete_index(_Surface_vmat,sid);
	}
}

SDL_Surface* setDisplayFormat(SDL_Surface *s)
{
	SDL_Surface *s2;
	if _oF(!s) return NULL;
	s2=SDL_ConvertSurface(s,_PixelFormat,0);
	SDL_FreeSurface(s);
	return s2;
}

