#ifndef	_package_vya_wav_h_
#define	_package_vya_wav_h_

#include "../package.h"

#ifndef	PACKAGE
	#define AUTHOR			"vya"
	#define PACKAGE			"wav"
	#define PACKAGE_major		1
	#define PACKAGE_minor		0
	#define PACKAGE_revision	1
	#define PACKAGE_type		"vya.wav"
	#define PACKAGE_type_spwav	"vya.spwav"
	
	#define SamFre			96000
	#define SamBytes		4
	#define SamBits			32
	#define	_phy_maxtime		11184.810
	#define _def_maxtime		600.000
	
	// 响度用户输入值类型模式
		#define loudmode_no		0
		#define loudmode_ra		1
		#define loudmode_db_atte	2
#endif

extern var *type;
extern var *type_spwav;

typedef struct __attribute__ ((packed)) Svya_wav_hi {
	// head
	char RIFF[4];
	u32 fsz;
	char WAVE[4];
	// info
	char Label_fmt[4];
	u32 fmt_sz;
	u16 format;
	u16 channel;
	u32 samp_f;
	u32 samp_bf;
	u16 align;
	u16 depth;
	char Label_data[4];
	u32 data_sz;
} vya_wav_hi;

typedef struct Svya_wav {
	u32 size;
	s32 *data;
} vya_wav;

typedef struct Svya_wav_spwav {
	// 状态标志
	// >0 未初始化; ==0 已初始化; <0 读取结束
	s8 *status;
	// 中间变量
	double *ts;
	double *te;
	double *aloud;
	// 约束变量
	double *et;
	double *ea;
	double *el;
	double *amin;
	double *ta;
	double *tb;
	// 初始化周期约束
	double *tmin;
	double *tmax;
	// 链接波形结构
	vya_wav *wav;
} vya_wav_spwav;

typedef struct Svya_wav_fse {
	// 傅里叶展开最大级数
	u8 sn;
	// 周期 平均响度
	double *t;
	double *ar;
	// 相对幅值 相位 [sn+1]
	double *r;
	double *p;
} vya_wav_fse;

typedef struct Interface_vya_wav {

} interface_vya_wav;

#endif

