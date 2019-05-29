#ifndef	_package_vya_gspace_h_
#define	_package_vya_gspace_h_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "configure.h"

#include "../package.h"
#include "gspace.code.h"
#include "gspace.func.h"
#define AUTHOR			"vya"
#define PACKAGE			"gspace"
#define PACKAGE_major		1
#define PACKAGE_minor		0
#define PACKAGE_revision	1
#define PACKAGE_value		(PACKAGE_major<<16|PACKAGE_minor<<8|PACKAGE_revision)

#define OPENGL_MAJOR		_sys_opengl_version_major
#define OPENGL_MINOR		_sys_opengl_version_minor
#define OPENGL_PROFILE		GLUT_CORE_PROFILE
#define DISPLAY_MODE		(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE)

#define DEFAULT_CLEAR_BIT	(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT)

// flag
#define F_type		0x000f0000
#define F_isok		0x00100000
#define F_userDelete	0x00200000

#define F_type_t(t)	((t<<16)&F_type)
#define F_type_s(m, t)	m = m & ~F_type | (t<<16)
#define F_type_g(m)	((m&F_type)>>16)

// buffer type
typedef enum ESrcBufferType {
	srcBufferTypeAll,
	
	srcBufferTypeVertexAttributes,		// 顶点属性数组
		// 顶点数组指针参数将被解释为在基本机器单元中测量的缓冲区对象内的偏移量
	srcBufferTypeAtomicCounter,		// 原子计数器储存
	srcBufferTypeCopyRead,			// 缓冲区复制源
	srcBufferTypeCopyWrite,			// 缓冲区复制目标
		// 允许使用 glCopyBufferSubData 而不会干扰其他绑定的状态，但是 glCopyBufferSubData 可以与任何一对缓冲区绑定点一起使用
	srcBufferTypeDispatchIndirect,		// 间接计算调度命令
		// 通过 glDispatchComputeIndirect 发出的计算调度的参数来自该缓冲区对象的数据存储中的指定偏移量
	srcBufferTypeDrawIndirect,		// 间接命令参数
		// 通过 glDraw*Indirect 发出的绘制参数来自该缓冲区对象的数据存储中的指定偏移量
	srcBufferTypeVertexIndices,		// 顶点属性数组索引
		// gl*Draw*Elements* 的 indices 参数被解释为在基本机器单元中测量的缓冲区对象内的偏移量
	srcBufferTypePixelRead,			// 像素读取目标
		// glGetCompressedTexImage glGetTexImage glReadPixels 指针参数被解释为在基本机器单元中测量的缓冲区对象内的偏移量
	srcBufferTypeTextureSource,		// 纹理数据源
		// glCompressedTex*Image* glTex*Image* 指针参数被解释为在基本机器单元中测量的缓冲区对象内的偏移量
	srcBufferTypeQueryResult,		// 查询结果缓冲区
		// 用于指定通过调用 glGetQueryObject 系列命令来接收查询对象结果的缓冲区对象
	srcBufferTypeShaderStorage,		// 着色器读取存储
	srcBufferTypeTextureBuffer,		// 纹理数据缓冲区
	srcBufferTypeTransformFeedback,		// 传输反馈缓冲区
		// 绑定点可以传递给 glBindBuffer 而不会直接影响转换反馈状态
		// 必须通过调用 glBindBufferBase 或 glBindBufferRange 来使用索引的 GL_TRANSFORM_FEEDBACK_BUFFER 绑定
		// 这将影响通用 GL_TRANSFORM_FEEDBACK_BUFFER 绑定
	srcBufferTypeUniformBlock,		// 统一块储存
		// 可以使用使用 GL_UNIFORM_BUFFER GL_ATOMIC_COUNTER_BUFFER GL_SHADER_STORAGE_BUFFER 缓冲区绑定点
		// 必须使用 glBindBufferBase 或 glBindBufferRange 将缓冲区绑定到索引的统一缓冲区，原子计数器缓冲区或着色器存储缓冲区绑定点
	
	srcBufferTypeMax
} SrcBufferType;

#define TEXTURE_ACTIVE_MAX	16

// texture type
typedef enum ESrcTextureType {
	srcTextureTypeAll,
	
	srcTextureType1D,
		// 一维纹理
	srcTextureType1DArray,
		// 一维纹理数组
	srcTextureType2D,
		// 二维纹理
	srcTextureType2DArray,
		// 二维纹理数组
	srcTextureType2DMS,
		// 二维多重采样纹理
	srcTextureType2DMSArray,
		// 二维多重采样纹理数组
	srcTextureType3D,
		// 三维纹理
	srcTextureTypeCube,
		// 立方体映射纹理
	srcTextureTypeCubeArray,
		// 立方体映射纹理数组
	srcTextureTypeRect,
		// 矩形纹理
	srcTextureTypeBuffer,
		// 缓冲纹理
	
	srcTextureTypeMax
} SrcTextureType;

#define MODEL_COMMAND_ARGC_MAX	16

typedef void (*ModelFunc)(u64 [], var *);

typedef enum EModelCommand {
	mcRunScript,
	mcSyncUniform,
	mcBindBuffer,
	mcBindTexture,
	mcBindSampler,
	mcBindVertexAttrib,
	
	mcDrawArrays,
	mcDrawElements,
	
	mcNumber
} ModelCommand;

// model uniform type define
#define MODEL_UNIFORM_TYPE_mask		0x30
#define MODEL_UNIFORM_TYPE_num(n)	((n-1)&0x03)
#define MODEL_UNIFORM_TYPE_rc(r, c)	((((r-1)&0x03)<<2)|((c-1)&0x03))
#define MODEL_UNIFORM_TYPE_M		0x00
#define MODEL_UNIFORM_TYPE_F		0x10
#define MODEL_UNIFORM_TYPE_I		0x20
#define MODEL_UNIFORM_TYPE_U		0x30

#define MODEL_UNIFORM_TYPE_f1		MODEL_UNIFORM_TYPE_F|MODEL_UNIFORM_TYPE_num(1)
#define MODEL_UNIFORM_TYPE_f2		MODEL_UNIFORM_TYPE_F|MODEL_UNIFORM_TYPE_num(2)
#define MODEL_UNIFORM_TYPE_f3		MODEL_UNIFORM_TYPE_F|MODEL_UNIFORM_TYPE_num(3)
#define MODEL_UNIFORM_TYPE_f4		MODEL_UNIFORM_TYPE_F|MODEL_UNIFORM_TYPE_num(4)

#define MODEL_UNIFORM_TYPE_i1		MODEL_UNIFORM_TYPE_I|MODEL_UNIFORM_TYPE_num(1)
#define MODEL_UNIFORM_TYPE_i2		MODEL_UNIFORM_TYPE_I|MODEL_UNIFORM_TYPE_num(2)
#define MODEL_UNIFORM_TYPE_i3		MODEL_UNIFORM_TYPE_I|MODEL_UNIFORM_TYPE_num(3)
#define MODEL_UNIFORM_TYPE_i4		MODEL_UNIFORM_TYPE_I|MODEL_UNIFORM_TYPE_num(4)

#define MODEL_UNIFORM_TYPE_u1		MODEL_UNIFORM_TYPE_U|MODEL_UNIFORM_TYPE_num(1)
#define MODEL_UNIFORM_TYPE_u2		MODEL_UNIFORM_TYPE_U|MODEL_UNIFORM_TYPE_num(2)
#define MODEL_UNIFORM_TYPE_u3		MODEL_UNIFORM_TYPE_U|MODEL_UNIFORM_TYPE_num(3)
#define MODEL_UNIFORM_TYPE_u4		MODEL_UNIFORM_TYPE_U|MODEL_UNIFORM_TYPE_num(4)

#define MODEL_UNIFORM_TYPE_m2		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(2,2)
#define MODEL_UNIFORM_TYPE_m3		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(3,3)
#define MODEL_UNIFORM_TYPE_m4		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(4,4)
#define MODEL_UNIFORM_TYPE_m2x3		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(2,3)
#define MODEL_UNIFORM_TYPE_m3x2		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(3,2)
#define MODEL_UNIFORM_TYPE_m2x4		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(2,4)
#define MODEL_UNIFORM_TYPE_m4x2		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(4,2)
#define MODEL_UNIFORM_TYPE_m3x4		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(3,4)
#define MODEL_UNIFORM_TYPE_m4x3		MODEL_UNIFORM_TYPE_M|MODEL_UNIFORM_TYPE_rc(4,3)

// string
#define S_src		"src"
#define S_handle	"handle"
#define S_program	"program"
#define S_buffer	"buffer"
#define S_texture	"texture"
#define S_sampler	"sampler"
#define S_vertexAttrib	"vertexAttrib"
	#define S_location	"location"
	#define S_list		"list"
#define S_model		"model"
	#define S_uniform	"uniform"
	#define S_unidata	"unidata"
	#define S_bind		"bind"
	#define S_command	"command"
	#define S_script	"script"
	#define S_env		"env"


// extern
extern u64 isInit;
extern var *V_program;
extern var *V_buffer;
extern var *V_texture;
extern var *V_sampler;
extern var *V_vertexAttrib;
extern var *V_model;

extern var *INFO_OpenglVendor;
extern var *INFO_OpenglRenderer;
extern var *INFO_OpenglVersion;
extern var *INFO_OpenglShadingLanguageVersion;

extern var *VAR_OpenglMajor;
extern var *VAR_OpenglMinor;
extern var *VAR_OpenglProfile;
extern var *VAR_DisplayMode;
extern var *VAR_ClearBit;
extern var *VAR_ModelUniTran;

#endif

