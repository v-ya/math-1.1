#ifndef	_limit_h_
#define	_limit_h_
/*
初始化值的设置，静态储存值的设置
*/

// 版本号
	#define	VERSION_major		1
	#define VERSION_minor		1
	#define VERSION_revision	0

// 限制
	// 自定义数组最大长度
	#define MAX_array		0x1000
	// sbuf 缓冲区的基本增量（初始化长度）
	#define BASE_sbuf		0x1000
	// sbuf 缓冲区最大大小
	#define MAX_sbuf		0x8000
	// 自定义函数最大传递参数	
	#define MAX_fargc		16

#endif
