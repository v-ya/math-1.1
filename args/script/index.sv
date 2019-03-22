#include "../../main.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vlist	0	r	{
	// 全称
	name	vmat	0	r	{
		help		string	0	r	_script_help
		version		string	0	r	_script_version
	}
	// 别名
	alisa	vmat	0	r	{
		h	-link	.name.help
		v	-link	.name.version
	}
	_NotFind_	string	0	r	_script__NotFind_
	// 导出
	_SCRIPT_ROOT	-export	.
	_SCRIPT_NAME	-export	.name
	_SCRIPT_ALISA	-export	.alisa
	_SCRIPT_NotFind	-export	._NotFind_
}

