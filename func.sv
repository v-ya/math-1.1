#include "main.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vmat	0	r	{
	user	vmat	0	rw
	gobj	vmat	0	r	{
		// 对象通用子项
		var	key	0	r	addr_key(var)
			// var type:name, ...;
			// var type:(name, ...), ...;
			// var type:name=value, ...;
			// var type:(name=value, ...), ...;
		delete	key	0	r	addr_key(delete)
			// delete name, ...;
		function key	0	r	addr_key(function)
			// function type:name ();
			// function type:name (vname:.type.*, ...);
			// function type:name () { -code- };
			// function type:name (vname:.type.*, ...) { -code- };
		exist	fun	0	r	addr_fun(exist)
			// long exist("var-path");
			// long exist("var-path",.type.*);
		import	fun	0	r	addr_fun(import)
			// string so-name= import("var-name","so-path");
		run	key	0	r	addr_key(run)
			// run string-code;
			// run exp(string-code);
		try	key	0	r	addr_key(try)
			// try string-code;
			// try exp(string-code);
			// try string-code, string-catch;
			// try exp(string-code), exp(string-catch);
		include	key	0	r	addr_key(include)
			// include "path";
		add	fun	0	r	addr_fun(add)
			// var new-var= add("name"|index, "type"|.type.*);
			// var new-var-array= add("name"|index, "type"|.type.*, length);
			// var var-need-refer= add("name"|index, "refer"|.type.refer, var-need-refer);
		replace	fun	0	r	addr_fun(replace)
			// like add();
			// add not replace, and return new's or exist's var
		sub	fun	0	r	addr_fun(sub)
			// sub("name"|index);
			// like delete name;
		
		// 占位符
		_vt_	void	0	r
		_vn_	void	0	r
		_text_	void	0	r
		_ret_	void	0	r
		_caller_ void	0	r
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
		this	vmat	0	rc
		text	vmat	0	rc
		temp	vmat	0	rc
		sbuf	vmat	0	rc
		kill	vlist	0	rc
		error	vlist	0	rc
		buerr	vlist	0	rc
		
		_pt_this	-export	.thread.this
		_pt_text	-export	.thread.text
		_pt_temp	-export	.thread.temp
		_pt_sbuf	-export	.thread.sbuf
		_pt_kill	-export .thread.kill
		_pt_error	-export	.thread.error
		_pt_buerr	-export	.thread.buerr
		
		// 线程相关函数
		self	fun	0	r	addr_fun(thread_self)
	}
	// 引用池
	refpool	vmat	0	rc
	_refpool	-export	.refpool
	// 系统相关
	sys	vmat	0	r	{
		// 版本号
		version	vlist	0	r	{
			major	ubyte	0	r	VERSION_major
				// 主版本号
			minor	ubyte	0	r	VERSION_minor
				// 次版本号
			rev	ubyte	0	r	VERSION_revision
				// 最小的版本号 ...
			value	uint	0	r	VERSION_value
				// major << 16 | minor <<8 | rev
		}
		// 限制
		limit	vlist	0	r	{
			array_max	sint	0	rw	MAX_array
				// 最大数组 length
			sbuf_base	uword	0	rw	BASE_sbuf
				// sbuf 缓冲区的基本增量（初始化长度）
			sbuf_max	uword	0	rw	MAX_sbuf
				// sbuf 缓冲区最大大小
			fargc_max	ubyte	0	rw	MAX_fargc
				// 自定义函数最大传递参数，如需要更多请尽量用 object : )
			file_str_size	uint	0	rw	SIZE_ftos
				// 文本文件转换成字符串的最大大小，主要用于 include ;的限制
			
			_lim_array_max	-export	.sys.limit.array_max
			_lim_sbuf_base	-export	.sys.limit.sbuf_base
			_lim_sbuf_max	-export	.sys.limit.sbuf_max
			_lim_fargc_max	-export	.sys.limit.fargc_max
			_lim_ftos_size	-export	.sys.limit.file_str_size
		}
		// 路径
		path	vlist	0	r	{
			cutup	sbyte	0	rw	PATH_cutup_c
				// path 的分割符 path:path || path;path
			incutup	sbyte	0	rw	PATH_incutup_c
				// path 内的分割符 dir/dir || dir\dir
			_path_cutup	-export	.sys.path.cutup
			_path_incutup	-export	.sys.path.incutup
			
			import	string	0	rw	PATH_import
				// package 的默认储存路径
			include	string	0	rw	PATH_include
				// 脚本文件的默认储存路径
			data	string	0	rw	PATH_data
				// 数据文件的默认储存路径(暂时无用 :3)
			
			_path_import	-export	.sys.path.import
			_path_include	-export	.sys.path.include
			_path_data	-export	.sys.path.data
		}
		// 信息
		info	vlist	0	r	{
			rand_max	ulong	0	r	RAND_MAX_L
				// .rand() 默认返回的上限 [0, rand_max)
			clocks_per_sec	ulong	0	r	CLOCKS_PER_SEC
				// .time.clock() 返回值与时间关系
		}
	}
	// 关键字函数
	goto	key	0	r	addr_key(goto)
		// .goto label;
		// 从文件/函数开始处向下查找
	gotodw	key	0	r	addr_key(gotodw)
		// .gotodw label;
		// 从命令处向下查找
	gotoup	key	0	r	addr_key(gotoup)
		// .gotoup label;
		// 从命令处向上查找
	if	key	0	r	addr_key(if)
		// .if var|(-exp-); command:command: ...;
	while	key	0	r	addr_key(while)
		// .while var|(-exp-); command:command: ...;
	exit	key	0	r	addr_key(exit)
		// .exit command:command: ...;
	// 内嵌函数
	echo	fun	0	r	addr_fun(echo)
	print	-link	.echo
		// .print("Hello World!\n");
		// .print("%%%c ...",'z', ...);
		// % d u s c x X o f F e E g G a A ?
	length	fun	0	r	addr_fun(length)
		// long array-length= .length(var);
	this	fun	0	r	addr_fun(this)
		// var object-env= .this();
	get_type fun	0	r	addr_fun(get_type)
		// long type= .get_type(var);
	remove	fun	0	r	addr_fun(remove)
		// .remove("so-name");
	issame	fun	0	r	addr_fun(issame)
		// long is-one-var= issame(var, var, ...);
		// 数学库
		srand	fun	0	r	addr_fun(srand)
			// .srand(); => srand(time(NULL));
			// .srand(var-znum);
		rand	fun	0	r	addr_fun(rand)
			// long [0, .sys.info.rand_max)= .rand();
			// long [0, var-znum) = .rand(var-znum);
			// float [0, var-float) = .rand(var-float);
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
			// float .[a]{sin,cos,tan}[h](num);
		atan2	fun	0	r	addr_fun(atan2)
		acot2	fun	0	r	addr_fun(acot2)
			// float .a{tan,cot}2(num,num);
		hypot	fun	0	r	addr_fun(hypot)
			// float .hypot(num,num);
		exp	fun	0	r	addr_fun(exp)
		sqrt	fun	0	r	addr_fun(sqrt)
		ln	fun	0	r	addr_fun(ln)
		log2	fun	0	r	addr_fun(log2)
		log10	fun	0	r	addr_fun(log10)
			// float .{exp,sqrt,ln,log2,log10}(num);
		log	fun	0	r	addr_fun(log)
			// float .log(num-e,num-N);
		ceil	fun	0	r	addr_fun(ceil)
		floor	fun	0	r	addr_fun(floor)
		round	fun	0	r	addr_fun(round)
			// float .{ceil,floor,round}(num);
		abs	fun	0	r	addr_fun(abs)
			// long .abs(znum);
			// float .abs(float);
		ftol	fun	0	r	addr_fun(ftol)
			// ulong .ftol(num); => (double) -> u8
		ftoi	fun	0	r	addr_fun(ftoi)
			// uint .ftoi(num); => (float) -> u4
		ltof	fun	0	r	addr_fun(ltof)
			// float .ltof(long); => (double)u8 -> double
			// float .ltof(int); => (float)u4 -> float -> double
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
		slong	slong	0	rw
		ulong	ulong	0	rw
		long	-link	.slong
		float	float	0	rw
		string	string	0	rw
		// 字符串处理库
		sprint	fun	0	r	addr_fun(sprint)
		s	-link	.sprint
			// like .print
			// string .sprint("format", ...);
		strlen	fun	0	r	addr_fun(strlen)
			// long .strlen(string);
		strbyte fun	0	r	addr_fun(strbyte)
			// ubyte .strbyte(string,num);
			// 'e' <= .strbyte("Hello",1);
		cpcode	fun	0	r	addr_fun(compress_code)
			// string cpcode(string);
		strarray key	0	r	addr_key(strarray)
			// .strarray exp(string);
			// string => ubyte[]
		arrstring key	0	r	addr_key(arrstring)
			// .arrstring exp(ubyte[]);
			// ubyte[] => string
	// 常数表
	const	vmat	0	r	{
		// 数学常量
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
		_2_sqrtpi float	0	r	M_2_SQRTPI
		sqrt2	float	0	r	M_SQRT2
		sqrt1_2	float	0	r	M_SQRT1_2
		// 常用字符串
		error	string	0	r	"error"
	}
	// 变量类型，用于定义函数
	type	vmat	0	r	{
		void	uint	0	r	type_void
		byte	uint	0	r	type_byte
		word	uint	0	r	type_word
		int	uint	0	r	type_int
		long	uint	0	r	type_long
		float	uint	0	r	type_float
		string	uint	0	r	type_string
		vlist	uint	0	r	type_vlist
		vmat	uint	0	r	type_vmat
		refer	uint	0	r	type_refer
		// sign
		sbyte	-link	.type.byte
		sword	-link	.type.word
		sint	-link	.type.int
		slong	-link	.type.long
		// unsign
		ubyte	uint	0	r	type_byte|type_unsign
		uword	uint	0	r	type_word|type_unsign
		uint	uint	0	r	type_int|type_unsign
		ulong	uint	0	r	type_long|type_unsign
		// 复合类型
		znum	uint	0	r	type_znum
			// byte, word, int, long
		num	uint	0	r	type_num
			// byte, word, int, long, float
		snum	uint	0	r	type_snum
			// byte, word, int, long, float, string
		object	uint	0	r	type_object
			// vlist, vmat
		func	uint	0	r	type_void|type_vlist
		all	uint	0	r	type_all
		// 特殊类型
		unsign	uint	0	r	type_unsign
	}
	// 时间相关
	time	vlist	0	r	{
		stamp	fun	0	r	addr_fun(stamp)
			// ulong .time.stamp(); => time(NULL)
			// 自 1970-01-01 经过的秒数
		clock	fun	0	r	addr_fun(clock)
			// ulong .time.clock(); => clock()
			// 自进程开始经历的滴答，单位见 .sys.info.clocks_per_sec
	}
	// 文件相关
	file	vlist	0	r	{
		size	fun	0	r	addr_fun(file_size)
			// ulong .file.size("path");
		read	fun	0	r	addr_fun(file_read)
			// ulong .file.read("path",array);
			// ulong .file.read("path",array,file-at);
			// ulong .file.read("path",array,file-at,count);
			// ulong .file.read("path",array,file-at,array-at,count);
			// ulong .file.read(string,znum[],znum,znum,znum);
		write	fun	0	r	addr_fun(file_write)
			// ulong .file.write("path",array);
			// ulong .file.write("path",array,isadd);
			// ulong .file.write("path",array,isadd,file-at);
			// ulong .file.write("path",array,isadd,file-at,count);
			// ulong .file.write("path",array,isadd,file-at,array-at,count);
			// ulong .file.write(string,znum[],znum,znum,znum,znum);
	}
	// try 处理错误函数
	try	vlist	0	r	{
		// use at try catch's code, else not find errid
		errid		fun	0	r	addr_fun(try_errid)
			// uint .try.errid(); => errid|0(end)
		errstr_cl	fun	0	r	addr_fun(try_errstr_cl)
			// string error-class= .try.errstr_cl();
		errstr_ty	fun	0	r	addr_fun(try_errstr_ty)
			// string error-type= .try.errstr_ty();
		errstr_id	fun	0	r	addr_fun(try_errstr_id)
			// string error-id= .try.errstr_id();
		errstr_text	fun	0	r	addr_fun(try_errstr_text)
			// string error-at-text= .try.errstr_text();
		errstr_label	fun	0	r	addr_fun(try_errstr_label)
			// string error-func= .try.errstr_label();
		errnext		fun	0	r	addr_fun(try_errnext)
			// void .try.errnext();
	}
	// include 相关信息
	include	vmat	0	rw
	// 调试相关
	debug	vlist	0	r	{
		list	fun	0	r	addr_fun(debug_list)
		szvmat	fun	0	r	addr_fun(debug_szvmat)
	}
}


