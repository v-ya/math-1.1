#include "main.h"
#include "init-st.c"

// init.math 阶段该结构将挂载到 .init

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vlist	0	r	{
	// 在 init.math 中使用
	_vl_init	-export	.
	auth	fun	0	r	addr_fun(init_auth)
		// .init.auth("srwlSc",var, ...);
		// s auth_retype
		// r auth_read
		// w auth_write
		// l auth_link
		// S auth_relength
		// c auth_system
}

