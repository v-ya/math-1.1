#ifndef	_error_h_
#define	_error_h_

// define errid

// Code Class
#define errcl_Code	0x00010000
	// Interrupted
	#define errty_CodeInt	errcl_Code|0x0100
		#define errid_IntError		(errty_CodeInt|0x01)
		#define errid_IntFuncErr	(errty_CodeInt|0x02)
		#define errid_IntDivZero	(errty_CodeInt|0x03)
		#define errid_IntKilled		(errty_CodeInt|0x04)
	// grammar
	#define errty_CodeGra	errcl_Code|0x0200
		#define errid_GraQuoVarname	(errty_CodeGra|0x01)
		#define errid_GraBraMismatch	(errty_CodeGra|0x02)
		#define errid_GraVarType	(errty_CodeGra|0x03)
		#define errid_GraUnknowSym	(errty_CodeGra|0x04)
		#define errid_GraLackSem	(errty_CodeGra|0x05)
		#define errid_GraSprintfFormat	(errty_CodeGra|0x06)
		#define errid_GraOvermuchUnOp	(errty_CodeGra|0x07)
	#define errty_CodeSys	errcl_Code|0x0300
		#define errid_SysPtvarNotfind	(errty_CodeSys|0x01)
		#define errid_SysDlpathNotopen	(errty_CodeSys|0x02)
		#define errid_SysDlsymNotfind	(errty_CodeSys|0x03)
		#define errid_SysPackageCheck	(errty_CodeSys|0x04)
		#define errid_SysPackageName	(errty_CodeSys|0x05)
		#define errid_SysPackageInit	(errty_CodeSys|0x06)
		#define errid_SysPackageNotfind	(errty_CodeSys|0x07)
		#define errid_SysPackageTakeup	(errty_CodeSys|0x08)
		#define errid_SysPackageDamage	(errty_CodeSys|0x09)
	#define errty_CodeFun	errcl_Code|0x0400
		#define errid_FunArgvType	(errty_CodeFun|0x01)
		#define errid_FunNotfindLabel	(errty_CodeFun|0x02)
// Resource Class
#define errcl_Res	0x00020000
	// Memory
	#define errty_ResMem	errcl_Res|0x0100
		#define errid_MemLess		(errty_ResMem|0x01)
		#define errid_ReqOver		(errty_ResMem|0x02)

// Var Class
#define errcl_Var	0x00030000
	// Var Exist
	#define errty_VarExist	errcl_Var|0x0100
		#define errid_VarNotFind	(errty_VarExist|0x01)
		#define errid_VarNotUsetmp	(errty_VarExist|0x02)
		#define errid_ArrayLength	(errty_VarExist|0x03)
		#define errid_ArrayBuffer	(errty_VarExist|0x04)
	// Var Type
	#define errty_VarType	errcl_Var|0x0200
		#define errid_VarUnknowType	(errty_VarType|0x01)
		#define errid_VarNotObject	(errty_VarType|0x02)
		#define errid_VarNotArray	(errty_VarType|0x03)
		#define errid_VarNotKey		(errty_VarType|0x04)
		#define errid_VarNotFun		(errty_VarType|0x05)
		#define errid_VarNotString	(errty_VarType|0x06)
		#define errid_VarNotNumber	(errty_VarType|0x07)
		#define errid_VarUnknowFun	(errty_VarType|0x08)
	// Var Auth
	#define errty_VarAuth	errcl_Var|0x0300
		#define errid_VarNotRetype	(errty_VarAuth|0x01)
		#define errid_VarNotRead	(errty_VarAuth|0x02)
		#define errid_VarNotWrite	(errty_VarAuth|0x03)
		#define errid_VarNotLink	(errty_VarAuth|0x04)
		#define errid_VarNotRun		(errty_VarAuth|0x05)
		#define errid_VarNotRelength	(errty_VarAuth|0x06)

typedef struct ERROR_INFO {
	u32 errid;
	u32 text_p;
	char *label;
	var *pt_text;
	struct ERROR_INFO *last;
} error_info;

#ifndef __nosym__

var* get_error(u32 errid, char *label);
void set_error(var *pt_text, char *script);
void clr_error(void);
char* str_error_cl(u32 errid);
char* str_error_ty(u32 errid);
char* str_error_id(u32 errid);
void print_error(void);

#endif

#endif

