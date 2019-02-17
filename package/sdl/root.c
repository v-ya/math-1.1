#include "sdl.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

static var	_sv_vp_0x5573ddb9c920;
var *root = &_sv_vp_0x5573ddb9c920;
static var	_sv_vp_0x5573ddb9ca50;
static vlist	_sv_vl_0x5573ddb9c9f0;
static var	_sv_vp_0x5573ddb9cad0;
static vlist	_sv_vl_0x5573ddb9ca70;
static var	_sv_vp_0x5573ddb9cb70;
static vlist	_sv_vl_0x5573ddb9cb10;
static var	_sv_vp_0x5573ddb9cc10;
static vlist	_sv_vl_0x5573ddb9cbb0;
static var	_sv_vp_0x5573ddb9cc90;
static vlist	_sv_vl_0x5573ddb9cc30;
static var	_sv_vp_0x5573ddb9cd30;
static vlist	_sv_vl_0x5573ddb9ccd0;
static var	_sv_vp_0x5573ddb9cdd0;
static vlist	_sv_vl_0x5573ddb9cd70;
static var	_sv_vp_0x5573ddb9ce70;
static vlist	_sv_vl_0x5573ddb9ce10;
static var	_sv_vp_0x5573ddb9cf10;
static vlist	_sv_vl_0x5573ddb9ceb0;
static var	_sv_vp_0x5573ddb9cfb0;
static vlist	_sv_vl_0x5573ddb9cf50;
static vlist*	_sv_avl_0x5573ddb9c960[16] = {
	[  3]	=&_sv_vl_0x5573ddb9cf50,
	[  6]	=&_sv_vl_0x5573ddb9c9f0,
	[  8]	=&_sv_vl_0x5573ddb9ceb0,
};
static vmat	_sv_vm_0x5573ddb9c940 = {
	.mask	=15
,	.number	=3
,	.avl	=_sv_avl_0x5573ddb9c960
};
static var	_sv_vp_0x5573ddb9c920 = {
	.type		=0x00000100,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_vmat	=&_sv_vm_0x5573ddb9c940
};
static vlist	_sv_vl_0x5573ddb9cf50 = {
	.name	="UpdateWindow",
	.mode	=0,
	.head	=0x5570646174655769,
	.v	=&_sv_vp_0x5573ddb9cfb0,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x5573ddb9cfb0 = {
	.type		=0x00000001,
	.length		=0,
	.inode		=1,
	.mode		=0x00000012,
	.v.v_void	=addr_fun(UpdateWindow)
};
static vlist	_sv_vl_0x5573ddb9c9f0 = {
	.name	="info",
	.mode	=0,
	.head	=0x696e666f00000000,
	.v	=&_sv_vp_0x5573ddb9ca50,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x5573ddb9ca50 = {
	.type		=0x00000080,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_vlist	=&_sv_vl_0x5573ddb9cbb0
};
static vlist	_sv_vl_0x5573ddb9ca70 = {
	.name	="author",
	.mode	=0,
	.head	=0x617574686f720000,
	.v	=&_sv_vp_0x5573ddb9cad0,
	.l	=NULL,
	.r	=&_sv_vl_0x5573ddb9cb10
};
static var	_sv_vp_0x5573ddb9cad0 = {
	.type		=0x00000040,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_string	=AUTHOR
};
static vlist	_sv_vl_0x5573ddb9cb10 = {
	.name	="package",
	.mode	=0,
	.head	=0x7061636b61676500,
	.v	=&_sv_vp_0x5573ddb9cb70,
	.l	=&_sv_vl_0x5573ddb9ca70,
	.r	=&_sv_vl_0x5573ddb9cbb0
};
static var	_sv_vp_0x5573ddb9cb70 = {
	.type		=0x00000040,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_string	=PACKAGE
};
static vlist	_sv_vl_0x5573ddb9cbb0 = {
	.name	="version",
	.mode	=0,
	.head	=0x76657273696f6e00,
	.v	=&_sv_vp_0x5573ddb9cc10,
	.l	=&_sv_vl_0x5573ddb9cb10,
	.r	=NULL
};
static var	_sv_vp_0x5573ddb9cc10 = {
	.type		=0x00000080,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_vlist	=&_sv_vl_0x5573ddb9ce10
};
static vlist	_sv_vl_0x5573ddb9cc30 = {
	.name	="major",
	.mode	=0,
	.head	=0x6d616a6f72000000,
	.v	=&_sv_vp_0x5573ddb9cc90,
	.l	=NULL,
	.r	=&_sv_vl_0x5573ddb9ccd0
};
static var	_sv_vp_0x5573ddb9cc90 = {
	.type		=0x01000002,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_long	=PACKAGE_major
};
static vlist	_sv_vl_0x5573ddb9ccd0 = {
	.name	="minor",
	.mode	=0,
	.head	=0x6d696e6f72000000,
	.v	=&_sv_vp_0x5573ddb9cd30,
	.l	=&_sv_vl_0x5573ddb9cc30,
	.r	=&_sv_vl_0x5573ddb9cd70
};
static var	_sv_vp_0x5573ddb9cd30 = {
	.type		=0x01000002,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_long	=PACKAGE_minor
};
static vlist	_sv_vl_0x5573ddb9cd70 = {
	.name	="rev",
	.mode	=0,
	.head	=0x7265760000000000,
	.v	=&_sv_vp_0x5573ddb9cdd0,
	.l	=&_sv_vl_0x5573ddb9ccd0,
	.r	=&_sv_vl_0x5573ddb9ce10
};
static var	_sv_vp_0x5573ddb9cdd0 = {
	.type		=0x01000002,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_long	=PACKAGE_revision
};
static vlist	_sv_vl_0x5573ddb9ce10 = {
	.name	="value",
	.mode	=0,
	.head	=0x76616c7565000000,
	.v	=&_sv_vp_0x5573ddb9ce70,
	.l	=&_sv_vl_0x5573ddb9cd70,
	.r	=NULL
};
static var	_sv_vp_0x5573ddb9ce70 = {
	.type		=0x01000008,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_long	=PACKAGE_value
};
static vlist	_sv_vl_0x5573ddb9ceb0 = {
	.name	="CreateWindow",
	.mode	=0,
	.head	=0x4372656174655769,
	.v	=&_sv_vp_0x5573ddb9cf10,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x5573ddb9cf10 = {
	.type		=0x00000001,
	.length		=0,
	.inode		=1,
	.mode		=0x00000012,
	.v.v_void	=addr_fun(CreateWindow)
};

