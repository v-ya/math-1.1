#include "main.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vmat	0	r	{
	user	vmat	0	rw
	gobj	vmat	0	r	{
		// 对象通用子项
		var	key	0	r	addr_key(var)
		delete	key	0	r	addr_key(delete)
		function key	0	r	addr_key(function)
		exist	fun	0	r	addr_fun(exist)
		import	fun	0	r	addr_fun(import)
		run	key	0	r	addr_key(run)
		try	key	0	r	addr_key(try)
		include	key	0	r	addr_key(include)
		
		// 占位符
		_vt_	void	0	r
		_vn_	void	0	r
		_text_	void	0	r
		_ret_	void	0	r
		_type_	string	0	r	"null"
		_type_null	-export	.gobj._type_
	}
	package	vlist	0	r
	
	_vm_sysm	-export	.
	_vm_user	-export	.user
	_vm_gobj	-export	.gobj
	_package	-export	.package
	// 线程相关
	thread	vlist	0	r	{
		this	vmat	0	-
		text	vmat	0	-
		temp	vmat	0	-
		sbuf	vmat	0	-
		kill	vlist	0	-
		error	vlist	0	-
		buerr	vlist	0	-
		
		_pt_this	-export	.thread.this
		_pt_text	-export	.thread.text
		_pt_temp	-export	.thread.temp
		_pt_sbuf	-export	.thread.sbuf
		_pt_kill	-export .thread.kill
		_pt_error	-export	.thread.error
		_pt_buerr	-export	.thread.buerr
	}
	// 系统相关
	sys	vmat	0	r	{
		// 版本号
		version	vlist	0	r	{
			major	ubyte	0	r	VERSION_major
			minor	ubyte	0	r	VERSION_minor
			rev	ubyte	0	r	VERSION_revision
		}
		// 限制
		limit	vlist	0	r	{
			array_max	sint	0	rw	MAX_array
			sbuf_base	uword	0	rw	BASE_sbuf
			sbuf_max	uword	0	rw	MAX_sbuf
			fargc_max	ubyte	0	rw	MAX_fargc
			file_str_size	uint	0	rw	SIZE_ftos
			
			_lim_array_max	-export	.sys.limit.array_max
			_lim_sbuf_base	-export	.sys.limit.sbuf_base
			_lim_sbuf_max	-export	.sys.limit.sbuf_max
			_lim_fargc_max	-export	.sys.limit.fargc_max
			_lim_ftos_size	-export	.sys.limit.file_str_size
		}
		// 路径
		path	vlist	0	r	{
			cutup	sbyte	0	rw	PATH_cutup
			incutup	sbyte	0	rw	PATH_incutup
			_path_cutup	-export	.sys.path.cutup
			_path_incutup	-export	.sys.path.incutup
			
			import	string	0	rw	PATH_import
			include	string	0	rw	PATH_include
			data	string	0	rw	PATH_data
			
			_path_import	-export	.sys.path.import
			_path_include	-export	.sys.path.include
			_path_data	-export	.sys.path.data
		}
		// 信息
		info	vlist	0	r	{
			rand_max	ulong	0	r	RAND_MAX_L
		}
	}
	// 关键字函数
	goto	key	0	r	addr_key(goto)
	gotodw	key	0	r	addr_key(gotodw)
	gotoup	key	0	r	addr_key(gotoup)
	if	key	0	r	addr_key(if)
	while	key	0	r	addr_key(while)
	exit	key	0	r	addr_key(exit)
	// 内嵌函数
	echo	fun	0	r	addr_fun(echo)
	print	-link	.echo
	length	fun	0	r	addr_fun(length)
	this	fun	0	r	addr_fun(this)
	get_type fun	0	r	addr_fun(get_type)
	remove	fun	0	r	addr_fun(remove)
		// 数学库
		srand	fun	0	r	addr_fun(srand)
		rand	fun	0	r	addr_fun(rand)
		sin	fun	0	r	addr_fun(sin)
		cos	fun	0	r	addr_fun(cos)
		tan	fun	0	r	addr_fun(tan)
		asin	fun	0	r	addr_fun(asin)
		acos	fun	0	r	addr_fun(acos)
		atan	fun	0	r	addr_fun(atan)
		sinh	fun	0	r	addr_fun(sinh)
		cosh	fun	0	r	addr_fun(cosh)
		tanh	fun	0	r	addr_fun(tanh)
		asinh	fun	0	r	addr_fun(asinh)
		acosh	fun	0	r	addr_fun(acosh)
		atanh	fun	0	r	addr_fun(atanh)
		atan2	fun	0	r	addr_fun(atan2)
		acot2	fun	0	r	addr_fun(acot2)
		hypot	fun	0	r	addr_fun(hypot)
		exp	fun	0	r	addr_fun(exp)
		sqrt	fun	0	r	addr_fun(sqrt)
		ln	fun	0	r	addr_fun(ln)
		log2	fun	0	r	addr_fun(log2)
		log10	fun	0	r	addr_fun(log10)
		log	fun	0	r	addr_fun(log)
		ceil	fun	0	r	addr_fun(ceil)
		floor	fun	0	r	addr_fun(floor)
		round	fun	0	r	addr_fun(round)
		abs	fun	0	r	addr_fun(abs)
		// 类型转换临时变量
		void	void	0	r
		sbyte	sbyte	0	rw
		ubyte	ubyte	0	rw
		byte	-link	.sbyte
		sword	sword	0	rw
		uword	uword	0	rw
		word	-link	.sword
		sint	sint	0	rw
		uint	uint	0	rw
		int	-link	.sint
		long	slong	0	rw
		float	float	0	rw
		string	string	NULL	rw
		// 字符串处理库
		sprint	fun	0	r	addr_fun(sprint)
		s	-link	.sprint
	// 常数表
	const	vmat	0	r	{
		e	float	0	r	M_E
		log2e	float	0	r	M_LOG2E
		log10e	float	0	r	M_LOG10E
		ln2	float	0	r	M_LN2
		ln10	float	0	r	M_LN10
		pi	float	0	r	M_PI
		pi_2	float	0	r	M_PI_2
		pi_4	float	0	r	M_PI_4
		_1_pi	float	0	r	M_1_PI
		_2_pi	float	0	r	M_2_PI
		_2_sqrtpi	float	0	r	M_2_SQRTPI
		sqrt2	float	0	r	M_SQRT2
		sqrt1_2	float	0	r	M_SQRT1_2
	}
	// 变量类型，用于定义函数
	type	vlist	0	r	{
		void	uint	0	r	type_void
		byte	uint	0	r	type_byte
		word	uint	0	r	type_word
		int	uint	0	r	type_int
		long	uint	0	r	type_long
		float	uint	0	r	type_float
		string	uint	0	r	type_string
		vlist	uint	0	r	type_vlist
		vmat	uint	0	r	type_vmat
		// 复合类型
		znum	uint	0	r	type_znum
		num	uint	0	r	type_num
		snum	uint	0	r	type_snum
		object	uint	0	r	type_object
		func	uint	0	r	type_void|type_vlist
		// 特殊类型
		unsign	uint	0	r	type_unsign
	}
	// try 处理错误函数
	try	vlist	0	r	{
		errid	fun	0	r	addr_fun(try_errid)
		errstr_cl	fun	0	r	addr_fun(try_errstr_cl)
		errstr_ty	fun	0	r	addr_fun(try_errstr_ty)
		errstr_id	fun	0	r	addr_fun(try_errstr_id)
		errstr_text	fun	0	r	addr_fun(try_errstr_text)
		errstr_label	fun	0	r	addr_fun(try_errstr_label)
		errnext	fun	0	r	addr_fun(try_errnext)
	}
	// 调试相关
	debug	vlist	0	r	{
		list	fun	0	r	addr_fun(debug_list)
	}
}


