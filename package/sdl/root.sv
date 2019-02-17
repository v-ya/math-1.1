#include "sdl.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vmat	0	r	{
	root	-export	.
	info	vlist	0	r	{
		author	string	0	r	AUTHOR
		package	string	0	r	PACKAGE
		version	vlist	0	r	{
			major	ubyte	0	r	PACKAGE_major
			minor	ubyte	0	r	PACKAGE_minor
			rev	ubyte	0	r	PACKAGE_revision
			value	uint	0	r	PACKAGE_value
		}
	}
	
	// functions
	CreateWindow	fun	0	r	addr_fun(CreateWindow)
		// ulong wid = CreateWindow(string title, znum x, znum y, znum w, znum h, znum flags);
	UpdateWindow	fun	0	r	addr_fun(UpdateWindow)
		// void UpdateWindow(znum wid);
	
}

