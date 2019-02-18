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
	
	// Windows
	// Surface
	
	// functions
	CreateWindow	fun	0	r	addr_fun(CreateWindow)
		// ulong wid = CreateWindow(string title, znum x, znum y, znum w, znum h, znum flags);
	UpdateWindow	fun	0	r	addr_fun(UpdateWindow)
		// void UpdateWindow(znum wid);
	DestroyWindow	fun	0	r	addr_fun(DestroyWindow)
		// void DestroyWindow(znum wid);
	LoadBMP		fun	0	r	addr_fun(LoadBMP)
		// ulong sid = LoadBMP(string path);
	SaveBMP		fun	0	r	addr_fun(SaveBMP)
		// long (0:ok) = SaveBMP(znum sid, string path);
	
}

