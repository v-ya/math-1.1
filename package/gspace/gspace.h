#ifndef	_package_vya_gspace_h_
#define	_package_vya_gspace_h_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "../package.h"
#include "gspace.code.h"
#include "gspace.func.h"
#define AUTHOR			"vya"
#define PACKAGE			"gspace"
#define PACKAGE_major		0
#define PACKAGE_minor		0
#define PACKAGE_revision	1
#define PACKAGE_value		(PACKAGE_major<<16|PACKAGE_minor<<8|PACKAGE_revision)

#define OPENGL_MAJOR		4
#define OPENGL_MINOR		2
#define OPENGL_PROFILE		GLUT_CORE_PROFILE
#define DISPLAY_MODE		(GLUT_RGBA|GLUT_DEPTH)

#define DEFAULT_CLEAR_BIT	(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT)

// flag
#define F_type		0x000f0000
#define F_isok		0x00100000
#define F_userDelete	0x00200000

#define F_type_t(t)	(t<<16)
#define F_type_s(m, t)	m = m & ~F_type | (t<<16)
#define F_type_g(m)	((m&F_type)>>16)

// type
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

// string
#define S_src		"src"
#define S_handle	"handle"
#define S_program	"program"
#define S_buffer	"buffer"
#define S_vertexAttrib	"vertexAttrib"
	#define S_location	"location"
	#define S_list		"list"
#define S_model		"model"
	#define S_bind		"bind"


// extern
extern u64 isInit;
extern var *V_program;
extern var *V_buffer;
extern var *V_vertexAttrib;
extern var *V_model;

extern var *INFO_OpenglVendor;
extern var *INFO_OpenglRenderer;
extern var *INFO_OpenglVersion;
extern var *INFO_OpenglShadingLanguageVersion;
extern var *INFO_OpenglExtensions;

extern var *VAR_OpenglMajor;
extern var *VAR_OpenglMinor;
extern var *VAR_OpenglProfile;
extern var *VAR_DisplayMode;
extern var *VAR_ClearBit;

#endif
