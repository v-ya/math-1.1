#ifndef	_package_system_sdl_h_
#define	_package_system_sdl_h_

#include <SDL2/SDL.h>

#include "../package.h"
#include "sdl.code.h"
#include "sdl.func.h"
#define AUTHOR			"system"
#define PACKAGE			"sdl"
#define PACKAGE_major		0
#define PACKAGE_minor		0
#define PACKAGE_revision	1
#define PACKAGE_value		(PACKAGE_major<<16|PACKAGE_minor<<8|PACKAGE_revision)

#define _INIT_FLAGES	(SDL_INIT_VIDEO|SDL_INIT_AUDIO)

#define __Windows__	"Windows"
#define __pWindow__	"pWindow"
#define __pSurface__	"pSurface"
#define __title__	"title"
#define __width__	"width"
#define __height__	"height"
#define __pixels__	"pixels"

extern var *_Windows;
extern vmat *_Windows_vmat;

#endif

