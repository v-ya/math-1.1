#include "hello.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vmat	0	rl	{
	info	vlist	0	r	{
		author	string	0	r	AUTHOR
		package	string	0	r	PACKAGE
		version	vlist	0	r	{
			major	ubyte	0	r	PACKAGE_major
			minor	ubyte	0	r	PACKAGE_minor
			rev	ubyte	0	r	PACKAGE_revision
		}
	}
	root	-export	.
	hello	fun	0	r	addr_fun(hello)
}
