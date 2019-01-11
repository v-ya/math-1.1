#ifndef	_main_h_
#define	_main_h_

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>
#include "type.h"
#include "limit.h"
#include "string.h"
#include "var.h"
#include "error.h"
#include "sbuf.h"
#include "math.h"
#include "file.h"
#include "exfun.h"
#include "BaseFunction.h"
#include "package.h"
#include "func.h"
#include "thread.h"
#include "init.h"

// 定义 RAND_MAX_L
#define RAND_MAX_L (1ull+RAND_MAX)

// 信息抛出
#define print(format, ...) fprintf(stdout, format, ##__VA_ARGS__)
#define err_throw(name, format, ...) fprintf(stderr, #name ": " format "\n", ##__VA_ARGS__)
#define info_throw(name, format, ...) fprintf(stdout, #name ": " format "\n", ##__VA_ARGS__)

// 比较优化

#ifndef _O_TF
	#ifdef __GNUC__
		#if (__GNUC__ >= 3)
			#define _oT(x) (__builtin_expect(!!(x), 1))
			#define _oF(x) (__builtin_expect(!!(x), 0))
			#define _O_TF
		#endif
	#endif
#endif

#ifndef _O_TF
	#define _oT(x) (x)
	#define _oF(x) (x)
#endif

#endif

