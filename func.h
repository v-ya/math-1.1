#ifndef	_func_h_
#define	_func_h_


// var* (var *call, char **expp)
#define keyword(name)	_kw_(_key_,name)
// void (var *ret, int argc, vlist *argv)
#define func(name)	_func_(_func_,name)

#define addr_key(name)	(&_key_##name)
#define addr_fun(name)	(&_func_##name)

#ifndef __nosym__

// 静态储存
extern var *_vm_sysm;
extern var *_vm_user;
extern var *_vm_gobj;
extern var *_package;
// 线程相关
extern var *_pt_this;
extern var *_pt_text;
extern var *_pt_temp;
extern var *_pt_sbuf;
extern var *_pt_kill;
extern var *_pt_error;
extern var *_pt_buerr;
// 引用池
extern var *_refpool;
// 限制
extern var *_lim_array_max;
extern var *_lim_sbuf_base;
extern var *_lim_sbuf_max;
extern var *_lim_fargc_max;
extern var *_lim_ftos_size;
// 路径
extern var *_path_cutup;
extern var *_path_incutup;
extern var *_path_import;
extern var *_path_include;
extern var *_path_data;
// 特殊
extern var *_type_null;

// 头文件

#include "func/function.h"
#include "func/key.h"
#include "func/con.h"
#include "func/math.h"
#include "func/string.h"
#include "func/time.h"
#include "func/file.h"
#include "func/try.h"
#include "func/thread.h"
#include "func/debug.h"

#endif

#endif

