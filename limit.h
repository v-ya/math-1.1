#ifndef	_limit_h_
#define	_limit_h_
/*
初始化值的设置，静态储存值的设置
*/

// 版本号
	#define	VERSION_major		1
	#define VERSION_minor		6
	#define VERSION_revision	0
	#define VERSION_value		(VERSION_major<<16|VERSION_minor<<8|VERSION_revision)

// 限制
	// 自定义数组最大长度 1 MiB - 8 MiB
	#define MAX_array		0x160000
	// sbuf 缓冲区的基本增量（初始化长度）
	#define BASE_sbuf		0x1000
	// sbuf 缓冲区最大大小
	#define MAX_sbuf		0x8000
	// 自定义函数最大传递参数
	#define MAX_fargc		16
	// 文本文件转换成字符串的最大大小（byte）: 1 MiB
	#define SIZE_ftos		0x160000

// 路径
	// path 的分割符
	#define PATH_cutup_c		':'
	#define PATH_cutup_s		":"
	// path 内的分割符
	#define PATH_incutup_c		'/'
	#define PATH_incutup_s		"/"
	// 内置环境目录路径
	//#define PATH_root		"/usr/lib/math"
	#ifdef PATH_root
		// package 的默认储存路径
		#define PATH_import		"." PATH_cutup_s "import" PATH_cutup_s PATH_root PATH_incutup_s "import"
		// 脚本文件的默认储存路径
		#define PATH_include		"." PATH_cutup_s "include" PATH_cutup_s PATH_root PATH_incutup_s "include"
		// 数据文件的默认储存路径
		#define PATH_data		"." PATH_cutup_s "data" PATH_cutup_s PATH_root PATH_incutup_s "data"
	#else
		#define PATH_import		"." PATH_cutup_s "import"
		#define PATH_include		"." PATH_cutup_s "include"
		#define PATH_data		"." PATH_cutup_s "data"
	#endif
#endif

