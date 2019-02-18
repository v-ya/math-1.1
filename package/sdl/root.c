#include "sdl.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

static var	_sv_vp_0x55e403fe4a30;
var *root = &_sv_vp_0x55e403fe4a30;
static var	_sv_vp_0x55e403fe4b60;
static vlist	_sv_vl_0x55e403fe4b00;
static var	_sv_vp_0x55e403fe4be0;
static vlist	_sv_vl_0x55e403fe4b80;
static var	_sv_vp_0x55e403fe4c80;
static vlist	_sv_vl_0x55e403fe4c20;
static var	_sv_vp_0x55e403fe4d20;
static vlist	_sv_vl_0x55e403fe4cc0;
static var	_sv_vp_0x55e403fe4da0;
static vlist	_sv_vl_0x55e403fe4d40;
static var	_sv_vp_0x55e403fe4e40;
static vlist	_sv_vl_0x55e403fe4de0;
static var	_sv_vp_0x55e403fe4ee0;
static vlist	_sv_vl_0x55e403fe4e80;
static var	_sv_vp_0x55e403fe4f80;
static vlist	_sv_vl_0x55e403fe4f20;
static var	_sv_vp_0x55e403fe5020;
static vlist	_sv_vl_0x55e403fe4fc0;
static var	_sv_vp_0x55e403fe50c0;
static vlist	_sv_vl_0x55e403fe5060;
static var	_sv_vp_0x55e403fe5160;
static vlist	_sv_vl_0x55e403fe5100;
static var	_sv_vp_0x55e403fe5200;
static vlist	_sv_vl_0x55e403fe51a0;
static var	_sv_vp_0x55e403fe52a0;
static vlist	_sv_vl_0x55e403fe5240;
static vlist*	_sv_avl_0x55e403fe4a70[16] = {
	[  0]	=&_sv_vl_0x55e403fe5240,
	[  2]	=&_sv_vl_0x55e403fe5100,
	[  3]	=&_sv_vl_0x55e403fe5060,
	[  6]	=&_sv_vl_0x55e403fe4b00,
	[  8]	=&_sv_vl_0x55e403fe4fc0,
	[ 13]	=&_sv_vl_0x55e403fe51a0,
};
static vmat	_sv_vm_0x55e403fe4a50 = {
	.mask	=15
,	.number	=6
,	.avl	=_sv_avl_0x55e403fe4a70
};
static var	_sv_vp_0x55e403fe4a30 = {
	.type		=0x00000100,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_vmat	=&_sv_vm_0x55e403fe4a50
};
static vlist	_sv_vl_0x55e403fe5240 = {
	.name	="SaveBMP",
	.mode	=0,
	.head	=0x53617665424d5000,
	.v	=&_sv_vp_0x55e403fe52a0,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x55e403fe52a0 = {
	.type		=0x00000001,
	.length		=0,
	.inode		=1,
	.mode		=0x00000012,
	.v.v_void	=addr_fun(SaveBMP)
};
static vlist	_sv_vl_0x55e403fe5100 = {
	.name	="DestroyWindow",
	.mode	=0,
	.head	=0x44657374726f7957,
	.v	=&_sv_vp_0x55e403fe5160,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x55e403fe5160 = {
	.type		=0x00000001,
	.length		=0,
	.inode		=1,
	.mode		=0x00000012,
	.v.v_void	=addr_fun(DestroyWindow)
};
static vlist	_sv_vl_0x55e403fe5060 = {
	.name	="UpdateWindow",
	.mode	=0,
	.head	=0x5570646174655769,
	.v	=&_sv_vp_0x55e403fe50c0,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x55e403fe50c0 = {
	.type		=0x00000001,
	.length		=0,
	.inode		=1,
	.mode		=0x00000012,
	.v.v_void	=addr_fun(UpdateWindow)
};
static vlist	_sv_vl_0x55e403fe4b00 = {
	.name	="info",
	.mode	=0,
	.head	=0x696e666f00000000,
	.v	=&_sv_vp_0x55e403fe4b60,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x55e403fe4b60 = {
	.type		=0x00000080,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_vlist	=&_sv_vl_0x55e403fe4cc0
};
static vlist	_sv_vl_0x55e403fe4b80 = {
	.name	="author",
	.mode	=0,
	.head	=0x617574686f720000,
	.v	=&_sv_vp_0x55e403fe4be0,
	.l	=NULL,
	.r	=&_sv_vl_0x55e403fe4c20
};
static var	_sv_vp_0x55e403fe4be0 = {
	.type		=0x00000040,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_string	=AUTHOR
};
static vlist	_sv_vl_0x55e403fe4c20 = {
	.name	="package",
	.mode	=0,
	.head	=0x7061636b61676500,
	.v	=&_sv_vp_0x55e403fe4c80,
	.l	=&_sv_vl_0x55e403fe4b80,
	.r	=&_sv_vl_0x55e403fe4cc0
};
static var	_sv_vp_0x55e403fe4c80 = {
	.type		=0x00000040,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_string	=PACKAGE
};
static vlist	_sv_vl_0x55e403fe4cc0 = {
	.name	="version",
	.mode	=0,
	.head	=0x76657273696f6e00,
	.v	=&_sv_vp_0x55e403fe4d20,
	.l	=&_sv_vl_0x55e403fe4c20,
	.r	=NULL
};
static var	_sv_vp_0x55e403fe4d20 = {
	.type		=0x00000080,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_vlist	=&_sv_vl_0x55e403fe4f20
};
static vlist	_sv_vl_0x55e403fe4d40 = {
	.name	="major",
	.mode	=0,
	.head	=0x6d616a6f72000000,
	.v	=&_sv_vp_0x55e403fe4da0,
	.l	=NULL,
	.r	=&_sv_vl_0x55e403fe4de0
};
static var	_sv_vp_0x55e403fe4da0 = {
	.type		=0x01000002,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_long	=PACKAGE_major
};
static vlist	_sv_vl_0x55e403fe4de0 = {
	.name	="minor",
	.mode	=0,
	.head	=0x6d696e6f72000000,
	.v	=&_sv_vp_0x55e403fe4e40,
	.l	=&_sv_vl_0x55e403fe4d40,
	.r	=&_sv_vl_0x55e403fe4e80
};
static var	_sv_vp_0x55e403fe4e40 = {
	.type		=0x01000002,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_long	=PACKAGE_minor
};
static vlist	_sv_vl_0x55e403fe4e80 = {
	.name	="rev",
	.mode	=0,
	.head	=0x7265760000000000,
	.v	=&_sv_vp_0x55e403fe4ee0,
	.l	=&_sv_vl_0x55e403fe4de0,
	.r	=&_sv_vl_0x55e403fe4f20
};
static var	_sv_vp_0x55e403fe4ee0 = {
	.type		=0x01000002,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_long	=PACKAGE_revision
};
static vlist	_sv_vl_0x55e403fe4f20 = {
	.name	="value",
	.mode	=0,
	.head	=0x76616c7565000000,
	.v	=&_sv_vp_0x55e403fe4f80,
	.l	=&_sv_vl_0x55e403fe4e80,
	.r	=NULL
};
static var	_sv_vp_0x55e403fe4f80 = {
	.type		=0x01000008,
	.length		=0,
	.inode		=1,
	.mode		=0x00000002,
	.v.v_long	=PACKAGE_value
};
static vlist	_sv_vl_0x55e403fe4fc0 = {
	.name	="CreateWindow",
	.mode	=0,
	.head	=0x4372656174655769,
	.v	=&_sv_vp_0x55e403fe5020,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x55e403fe5020 = {
	.type		=0x00000001,
	.length		=0,
	.inode		=1,
	.mode		=0x00000012,
	.v.v_void	=addr_fun(CreateWindow)
};
static vlist	_sv_vl_0x55e403fe51a0 = {
	.name	="LoadBMP",
	.mode	=0,
	.head	=0x4c6f6164424d5000,
	.v	=&_sv_vp_0x55e403fe5200,
	.l	=NULL,
	.r	=NULL
};
static var	_sv_vp_0x55e403fe5200 = {
	.type		=0x00000001,
	.length		=0,
	.inode		=1,
	.mode		=0x00000012,
	.v.v_void	=addr_fun(LoadBMP)
};

