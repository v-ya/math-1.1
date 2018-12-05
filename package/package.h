#ifndef	_package_main_h_
#define	_package_main_h_

#define __nosym__
#include "../main.h"

#define export __attribute__ ((visibility ("default")))

// 外部需定义变量
extern var *root;
extern char *author;
extern char *package;
void init(void);
void uini(void);

// 内部变量
extern interface_base *base;
var* get_error(char *info, char *label);
void throw_error(char *info, char *label);
void throw_errid(u32 errid);
void set_interface(void *_if);
// 导出变量
export u32 package_ckeck(u32 r);
export void package_name(char **a, char **p);
export var* package_init(void *Base, void **Interface);
export var* package_uini(void);

#endif

