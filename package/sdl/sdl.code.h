#ifndef	_package_system_sdl_code_h_
#define	_package_system_sdl_code_h_

u64 create_surface(SDL_Surface *s);
u64 new_surface(SDL_Surface *s);
SDL_Surface* getSurface(u64 sid);
void destroy_surface(u64 sid);
u64 create_window(SDL_Window *w);
u64 new_window(SDL_Window *w);
SDL_Window* getWindow(u64 wid);
void destroy_window(u64 wid);
SDL_Surface* setDisplayFormat(SDL_Surface *s);

#endif

