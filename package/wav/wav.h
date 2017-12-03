#ifndef	_package_vya_wav_h_
#define	_package_vya_wav_h_

#include "../package.h"

#ifndef	PACKAGE
	#define AUTHOR			"vya"
	#define PACKAGE			"wav"
	#define PACKAGE_major		0
	#define PACKAGE_minor		0
	#define PACKAGE_revision	1
	#define PACKAGE_type		"vya.wav"
	
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

typedef struct Interface_vya_wav {

} interface_vya_wav;

#endif

