#include "gspace.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vmat	0	r	{
	root	-export	.
	info	vlist	0	r	{
		author	string	0	r	AUTHOR
		package	string	0	r	PACKAGE
		version	vlist	0	r	{
			major	ubyte	0	r	PACKAGE_major
			minor	ubyte	0	r	PACKAGE_minor
			rev	ubyte	0	r	PACKAGE_revision
			value	uint	0	r	PACKAGE_value
		}
		OpenGL	vmat	0	r	{
			vendor		string	0	r	NULL
			renderer	string	0	r	NULL
			version		string	0	r	NULL
			glslv		string	0	r	NULL
			
			
			INFO_OpenglVendor			-export	.info.OpenGL.vendor
			INFO_OpenglRenderer			-export	.info.OpenGL.renderer
			INFO_OpenglVersion			-export	.info.OpenGL.version
			INFO_OpenglShadingLanguageVersion	-export	.info.OpenGL.glslv
		}
		ShaderType	vmat	0	r	{
			compute		ulong	0	r	GL_COMPUTE_SHADER
				// 计算着色器
			vertex		ulong	0	r	GL_VERTEX_SHADER
				// 顶点着色器
			tess_control	ulong	0	r	GL_TESS_CONTROL_SHADER
				// 细分控制着色器
			tess_evaluation	ulong	0	r	GL_TESS_EVALUATION_SHADER
				// 细分计算着色器
			geometry	ulong	0	r	GL_GEOMETRY_SHADER
				// 几何着色器
			fragment	ulong	0	r	GL_FRAGMENT_SHADER
				// 片元着色器
		}
		BufferType	vmat	0	r	{
			vertexArray		uint	0	r	srcBufferTypeVertexAttributes
				// 顶点属性数组
			atomicCounter		uint	0	r	srcBufferTypeAtomicCounter
				// 原子计数器储存
			copyRead		uint	0	r	srcBufferTypeCopyRead
				// 缓冲区复制源
			copyWrite		uint	0	r	srcBufferTypeCopyWrite
				// 缓冲区复制目标
			dispatchIndirect	uint	0	r	srcBufferTypeDispatchIndirect
				// 间接计算调度命令
			drawIndirect		uint	0	r	srcBufferTypeDrawIndirect
				// 间接命令参数
			vertexIndices		uint	0	r	srcBufferTypeVertexIndices
				// 顶点属性数组索引
			pixelRead		uint	0	r	srcBufferTypePixelRead
				// 像素读取目标
			textureSource		uint	0	r	srcBufferTypeTextureSource
				// 纹理数据源
			queryResult		uint	0	r	srcBufferTypeQueryResult
				// 查询结果缓冲区
			shaderStorage		uint	0	r	srcBufferTypeShaderStorage
				// 着色器读取存储
			textureBuffer		uint	0	r	srcBufferTypeTextureBuffer
				// 纹理数据缓冲区
			transformFeedback	uint	0	r	srcBufferTypeTransformFeedback
				// 传输反馈缓冲区
			uniformBlock		uint	0	r	srcBufferTypeUniformBlock
				// 统一块储存
		}
		BufferUsage	vmat	0	r	{
			stream	vlist	0	r	{
				draw	uint	0	r	GL_STREAM_DRAW
				read	uint	0	r	GL_STREAM_READ
				copy	uint	0	r	GL_STREAM_COPY
			}
			static	vlist	0	r	{
				draw	uint	0	r	GL_STATIC_DRAW
				read	uint	0	r	GL_STATIC_READ
				copy	uint	0	r	GL_STATIC_COPY
			}
			dynamic	vlist	0	r	{
				draw	uint	0	r	GL_DYNAMIC_DRAW
				read	uint	0	r	GL_DYNAMIC_READ
				copy	uint	0	r	GL_DYNAMIC_COPY
			}
			draw	vlist	0	r	{
				stream	-link	.info.BufferUsage.stream.draw
				static	-link	.info.BufferUsage.static.draw
				dynamic	-link	.info.BufferUsage.dynamic.draw
			}
			read	vlist	0	r	{
				stream	-link	.info.BufferUsage.stream.read
				static	-link	.info.BufferUsage.static.read
				dynamic	-link	.info.BufferUsage.dynamic.read
			}
			copy	vlist	0	r	{
				stream	-link	.info.BufferUsage.stream.copy
				static	-link	.info.BufferUsage.static.copy
				dynamic	-link	.info.BufferUsage.dynamic.copy
			}
		}
		TextureType	vmat	0	r	{
			t1D		uint	0	r	srcTextureType1D
				// 一维纹理
			t1DArray	uint	0	r	srcTextureType1DArray
				// 一维纹理数组
			t2D		uint	0	r	srcTextureType2D
				// 二维纹理
			t2DArray	uint	0	r	srcTextureType2DArray
				// 二维纹理数组
			t2DMS		uint	0	r	srcTextureType2DMS
				// 二维多重采样纹理
			t2DMSArray	uint	0	r	srcTextureType2DMSArray
				// 二维多重采样纹理数组
			t3D		uint	0	r	srcTextureType3D
				// 三维纹理
			tCube		uint	0	r	srcTextureTypeCube
				// 立方体映射纹理
			tCubeArray	uint	0	r	srcTextureTypeCubeArray
				// 立方体映射纹理数组
			tRect		uint	0	r	srcTextureTypeRect
				// 矩形纹理
			tBuffer		uint	0	r	srcTextureTypeBuffer
				// 缓冲纹理
		}
		TextureInternalFormat	vmat	0	r	{
			r8		uint	0	r	GL_R8
			r8_s		uint	0	r	GL_R8_SNORM
			r16		uint	0	r	GL_R16
			r16_s		uint	0	r	GL_R16_SNORM
			rg8		uint	0	r	GL_RG8
			rg8_s		uint	0	r	GL_RG8_SNORM
			rg16		uint	0	r	GL_RG16
			rg16_s		uint	0	r	GL_RG16_SNORM
			r3_g3_b2	uint	0	r	GL_R3_G3_B2
			rgb4		uint	0	r	GL_RGB4
			rgb5		uint	0	r	GL_RGB5
			rgb8		uint	0	r	GL_RGB8
			rgb8_s		uint	0	r	GL_RGB8_SNORM
			rgb10		uint	0	r	GL_RGB10
			rgb12		uint	0	r	GL_RGB12
			rgb16_s		uint	0	r	GL_RGB16_SNORM
			rgba2		uint	0	r	GL_RGBA2
			rgba4		uint	0	r	GL_RGBA4
			rgb5_a1		uint	0	r	GL_RGB5_A1
			rgba8		uint	0	r	GL_RGBA8
			rgba8_s		uint	0	r	GL_RGBA8_SNORM
			rgb10_a2	uint	0	r	GL_RGB10_A2
			rgb10_a2UI	uint	0	r	GL_RGB10_A2UI
			rgba12		uint	0	r	GL_RGBA12
			rgba16		uint	0	r	GL_RGBA16
			srgb8		uint	0	r	GL_SRGB8
			srgb8_alpha8	uint	0	r	GL_SRGB8_ALPHA8
			r16f		uint	0	r	GL_R16F
			rg16f		uint	0	r	GL_RG16F
			rgb16f		uint	0	r	GL_RGB16F
			rgba16f		uint	0	r	GL_RGBA16F
			r32f		uint	0	r	GL_R32F
			rg32f		uint	0	r	GL_RG32F
			rgb32f		uint	0	r	GL_RGB32F
			rgba32f		uint	0	r	GL_RGBA32F
			r11f_g11f_b10f	uint	0	r	GL_R11F_G11F_B10F
			rgb9_e5		uint	0	r	GL_RGB9_E5
			r8i		uint	0	r	GL_R8I
			r8ui		uint	0	r	GL_R8UI
			r16i		uint	0	r	GL_R16I
			r16ui		uint	0	r	GL_R16UI
			r32i		uint	0	r	GL_R32I
			r32ui		uint	0	r	GL_R32UI
			rg8i		uint	0	r	GL_RG8I
			rg8ui		uint	0	r	GL_RG8UI
			rg16i		uint	0	r	GL_RG16I
			rg16ui		uint	0	r	GL_RG16UI
			rg32i		uint	0	r	GL_RG32I
			rg32ui		uint	0	r	GL_RG32UI
			rgb8i		uint	0	r	GL_RGB8I
			rgb8ui		uint	0	r	GL_RGB8UI
			rgb16i		uint	0	r	GL_RGB16I
			rgb16ui		uint	0	r	GL_RGB16UI
			rgb32i		uint	0	r	GL_RGB32I
			rgb32ui		uint	0	r	GL_RGB32UI
			rgba8i		uint	0	r	GL_RGBA8I
			rgba8ui		uint	0	r	GL_RGBA8UI
			rgba16i		uint	0	r	GL_RGBA16I
			rgba16ui	uint	0	r	GL_RGBA16UI
			rgba32i		uint	0	r	GL_RGBA32I
			rgba32ui	uint	0	r	GL_RGBA32UI
		}
		TextureExternalFormat	vmat	0	r	{
			r		uint	0	r	GL_RED
			rg		uint	0	r	GL_RG
			rgb		uint	0	r	GL_RGB
			bgr		uint	0	r	GL_BGR
			rgba		uint	0	r	GL_RGBA
			bgra		uint	0	r	GL_BGRA
			depth		uint	0	r	GL_DEPTH_COMPONENT
			stencil		uint	0	r	GL_STENCIL_INDEX
		}
		TextureExternalType	vmat	0	r	{
			ubyte			uint	0	r	GL_UNSIGNED_BYTE
			sbyte			uint	0	r	GL_BYTE
			uword			uint	0	r	GL_UNSIGNED_SHORT
			sword			uint	0	r	GL_SHORT
			uint			uint	0	r	GL_UNSIGNED_INT
			sint			uint	0	r	GL_INT
			float			uint	0	r	GL_FLOAT
			ubyte_3_3_2		uint	0	r	GL_UNSIGNED_BYTE_3_3_2
			ubyte_2_3_3_r		uint	0	r	GL_UNSIGNED_BYTE_2_3_3_REV
			uword_5_6_5		uint	0	r	GL_UNSIGNED_SHORT_5_6_5
			uword_5_6_5_r		uint	0	r	GL_UNSIGNED_SHORT_5_6_5_REV
			uword_4_4_4_4		uint	0	r	GL_UNSIGNED_SHORT_4_4_4_4
			uword_4_4_4_4_r		uint	0	r	GL_UNSIGNED_SHORT_4_4_4_4_REV
			uword_5_5_5_1		uint	0	r	GL_UNSIGNED_SHORT_5_5_5_1
			uword_1_5_5_5_r		uint	0	r	GL_UNSIGNED_SHORT_1_5_5_5_REV
			uint_8_8_8_8		uint	0	r	GL_UNSIGNED_INT_8_8_8_8
			uint_8_8_8_8_r		uint	0	r	GL_UNSIGNED_INT_8_8_8_8_REV
			uint_10_10_10_2		uint	0	r	GL_UNSIGNED_INT_10_10_10_2
			uint_2_10_10_10_r	uint	0	r	GL_UNSIGNED_INT_2_10_10_10_REV
		}
		TextureCubeFace		vlist	0	r	{
			xPos	uint	0	r	0
			xNeg	uint	0	r	1
			yPos	uint	0	r	2
			yNeg	uint	0	r	3
			zPos	uint	0	r	4
			zNeg	uint	0	r	5
		}
		VertexAttribSize	vlist	0	r	{
			v1	sint	0	r	1
			v2	sint	0	r	2
			v3	sint	0	r	3
			v4	sint	0	r	4
			bgra	sint	0	r	GL_BGRA
		}
		VertexAttribType	vmat	0	r	{
			float			uint	0	r	GL_FLOAT
			double			uint	0	r	GL_DOUBLE
			half_float		uint	0	r	GL_HALF_FLOAT
			sbyte			uint	0	r	GL_BYTE
			ubyte			uint	0	r	GL_UNSIGNED_BYTE
			sword			uint	0	r	GL_SHORT
			uword			uint	0	r	GL_UNSIGNED_SHORT
			sint			uint	0	r	GL_INT
			uint			uint	0	r	GL_UNSIGNED_INT
			fixed			uint	0	r	GL_FIXED
			rev_sint_2_10_10_10	uint	0	r	GL_INT_2_10_10_10_REV
			rev_uint_2_10_10_10	uint	0	r	GL_UNSIGNED_INT_2_10_10_10_REV
			
			byte			-link	.info.VertexAttribType.sbyte
			word			-link	.info.VertexAttribType.sword
			int			-link	.info.VertexAttribType.sint
		}
		UniformType	vmat	0	r	{
			// float(32)
			f1	uint	0	r	MODEL_UNIFORM_TYPE_f1
			f2	uint	0	r	MODEL_UNIFORM_TYPE_f2
			f3	uint	0	r	MODEL_UNIFORM_TYPE_f3
			f4	uint	0	r	MODEL_UNIFORM_TYPE_f4
			// int(32)
			i1	uint	0	r	MODEL_UNIFORM_TYPE_i1
			i2	uint	0	r	MODEL_UNIFORM_TYPE_i2
			i3	uint	0	r	MODEL_UNIFORM_TYPE_i3
			i4	uint	0	r	MODEL_UNIFORM_TYPE_i4
			// uint(32)
			u1	uint	0	r	MODEL_UNIFORM_TYPE_u1
			u2	uint	0	r	MODEL_UNIFORM_TYPE_u2
			u3	uint	0	r	MODEL_UNIFORM_TYPE_u3
			u4	uint	0	r	MODEL_UNIFORM_TYPE_u4
			// mat
			m2	uint	0	r	MODEL_UNIFORM_TYPE_m2
			m3	uint	0	r	MODEL_UNIFORM_TYPE_m3
			m4	uint	0	r	MODEL_UNIFORM_TYPE_m4
			m2x3	uint	0	r	MODEL_UNIFORM_TYPE_m2x3
			m3x2	uint	0	r	MODEL_UNIFORM_TYPE_m3x2
			m2x4	uint	0	r	MODEL_UNIFORM_TYPE_m2x4
			m4x2	uint	0	r	MODEL_UNIFORM_TYPE_m4x2
			m3x4	uint	0	r	MODEL_UNIFORM_TYPE_m3x4
			m4x3	uint	0	r	MODEL_UNIFORM_TYPE_m4x3
		}
		
		
		ClearBit	vmat	0	r	{
			color	ulong	0	r	GL_COLOR_BUFFER_BIT
			depth	ulong	0	r	GL_DEPTH_BUFFER_BIT
			accum	ulong	0	r	GL_ACCUM_BUFFER_BIT
			stencil	ulong	0	r	GL_STENCIL_BUFFER_BIT
		}
		DrawMode	vmat	0	r	{
			points				uint	0	r	GL_POINTS
			line_strip			uint	0	r	GL_LINE_STRIP
			line_loop			uint	0	r	GL_LINE_LOOP
			lines				uint	0	r	GL_LINES
			triangle_strip			uint	0	r	GL_TRIANGLE_STRIP
			triangle_fan			uint	0	r	GL_TRIANGLE_FAN
			triangles			uint	0	r	GL_TRIANGLES
			line_strip_adjacency		uint	0	r	GL_LINE_STRIP_ADJACENCY
			lines_adjacency			uint	0	r	GL_LINES_ADJACENCY
			triangle_strip_adjacency	uint	0	r	GL_TRIANGLE_STRIP_ADJACENCY
			triangles_adjacency		uint	0	r	GL_TRIANGLES_ADJACENCY
			patches				uint	0	r	GL_PATCHES
		}
	}
	set	vmat	0	r	{
		init	vmat	0	r	{
			OpenGL_major	uint	0	rw	OPENGL_MAJOR
			OpenGL_minor	uint	0	rw	OPENGL_MINOR
			OpenGL_profile	uint	0	rw	OPENGL_PROFILE
			DisplayMode	uint	0	rw	DISPLAY_MODE
			
			VAR_OpenglMajor		-export	.set.init.OpenGL_major
			VAR_OpenglMinor		-export	.set.init.OpenGL_minor
			VAR_OpenglProfile	-export	.set.init.OpenGL_profile
			VAR_DisplayMode		-export	.set.init.DisplayMode
		}
		run	vmat	0	r	{
			clearBit		ulong	0	rw	DEFAULT_CLEAR_BIT
			modelUniformTranspose	uint	0	rw	1
			
			VAR_ClearBit		-export	.set.run.clearBit
			VAR_ModelUniTran	-export	.set.run.modelUniformTranspose
		}
		limit	vmat	0	r	{
			textureActiveMax	uint	0	r	TEXTURE_ACTIVE_MAX
			samplerActiveMax	uint	0	r	TEXTURE_ACTIVE_MAX
		}
	}
	
	// program
	createProgram		fun	0	r	addr_fun(createProgram)
		// ulong program = createProgram();
		// 创建着色器程序集
	deleteProgram		fun	0	r	addr_fun(deleteProgram)
		// void deleteProgram(ulong program);
		// 删除着色器程序集
	attachShader		fun	0	r	addr_fun(attachShader)
		// void|string error = attachShader(ulong program, ShaderType, string code);
		// void|string error = attachShader(ulong program, ShaderType, string[] code);
		// 编译着色器程序并将其附着到着色器程序集
	linkProgram		fun	0	r	addr_fun(linkProgram)
		// void|string error = attachShader(ulong program);
		// 链接着色器程序集，并将 program 设置为 ok 状态
	useProgram		fun	0	r	addr_fun(useProgram)
		// long error = useProgram(ulong program-ok);
		// 使用着色器程序集
	
	// buffer
	createBuffer		fun	0	r	addr_fun(createBuffer)
		// ulong buffer = createBuffer(BufferType);
		// 创建缓存，默认为 ok 状态
	deleteBuffer		fun	0	r	addr_fun(deleteBuffer)
		// void deleteBuffer(ulong buffer);
		// 删除缓存
	bufferSetData		fun	0	r	addr_fun(bufferSetData)
		// ulong error = bufferSetData(ulong buffer, znum size, BufferUsage);
		// ulong error = bufferSetData(ulong buffer, znum data[], BufferUsage);
		// 设置缓存
	bufferSubData		fun	0	r	addr_fun(bufferSubData)
		// ulong error = bufferSubData(ulong buffer, znum data[]);
		// ulong error = bufferSubData(ulong buffer, znum offset, znum data[]);
		// ulong error = bufferSubData(ulong buffer, znum offset, znum data[], znum number);
		// ulong error = bufferSubData(ulong buffer, znum offset, znum data[], znum begin, znum number);
		// 设置缓存子区数据
	useBuffer		fun	0	r	addr_fun(useBuffer)
		// long error = useBuffer(ulong buffer);
		// 使用缓冲（绑定缓冲到指定类型）
	
	// texture
	createTexture		fun	0	r	addr_fun(createTexture)
		// ulong texture = createTexture(TextureType);
		// 创建纹理
	deleteTexture		fun	0	r	addr_fun(deleteTexture)
		// void deleteTexture(ulong texture);
		// 删除纹理
	textureStorage		fun	0	r	addr_fun(textureStorage)
		// ulong error = textureStorage(ulong texture[t1D], znum levels, TextureInternalFormat, znum w);
		// ulong error = textureStorage(ulong texture[t2D, t1DArray, tCube, tRect], znum levels, TextureInternalFormat, znum w, znum h);
		// ulong error = textureStorage(ulong texture[t3D, t2DArray, tCubeArray], znum levels, TextureInternalFormat, znum w, znum h, znum d);
		// ulong error = textureStorage(ulong texture[t2DMS], znum samples, TextureInternalFormat, znum w, znum h, znum fixed);
		// ulong error = textureStorage(ulong texture[t2DMSArray], znum samples, TextureInternalFormat, znum w, znum h, znum d, znum fixed);
		// ulong error = textureStorage(ulong texture[tBuffer], TextureInternalFormat, ulong buffer-tbo);
		// 设置纹理储存属性，如果 error == 0 ，则将 texture 设置为 ok 状态
	textureSubImage		fun	0	r	addr_fun(textureSubImage)
		// level, format, ft, var pixels, [[begin, ]x, ]w
		// ulong error = textureSubImage(ulong texture-ok[t1D], znum level, TextureExternalFormat, TextureExternalType, znum data[], [[znum begin, ]znum x, ]znum w);
		// ulong error = textureSubImage(ulong texture-ok[t2D, t1DArray, tRect], znum level, TextureExternalFormat, TextureExternalType, znum data[], [[znum begin, ]znum x, znum y, ]znum w, znum h);
		// ulong error = textureSubImage(ulong texture-ok[t3D, t2DArray, tCubeArray], znum level, TextureExternalFormat, TextureExternalType, znum data[], [[znum begin, ]znum x, znum y, znum z]znum w, znum h, znum d);
		// ulong error = textureSubImage(ulong texture-ok[tCube], znum level, TextureCubeFace, TextureExternalFormat, TextureExternalType, znum data[], [[znum begin, ]znum x, znum y, ]znum w, znum h);
		// 设置纹理区块数据
	useTexture		fun	0	r	addr_fun(useTexture)
		// long error = useTexture(znum active, ulong texture);
		// 使用纹理
	
	// vertexAttrib
	createVertexAttrib	fun	0	r	addr_fun(createVertexAttrib)
		// ulong va = createVertexAttrib();
		// 创建顶点属性
	deleteVertexAttrib	fun	0	r	addr_fun(deleteVertexAttrib)
		// void createVertexAttrib(ulong va);
		// 删除顶点属性
	finalVertexAttrib	fun	0	r	addr_fun(finalVertexAttrib)
		// void finalVertexAttrib(ulong va);
		// 将 va 顶点属性设置为 ok 状态
	vertexAttribPointer	fun	0	r	addr_fun(vertexAttribPointer)
		// long error = vertexAttribPointer(ulong va, ulong buffer-vbo, znum index,
		//	VertexAttribSize, VertexAttribType, znum normalized, znum stride, znum pointer);
		// 关联顶点属性
	enableVertexAttrib	fun	0	r	addr_fun(enableVertexAttrib)
		// void enableVertexAttrib(ulong va-ok);
		// void enableVertexAttrib(ulong va-ok, znum index);
		// 启用顶点属性
	disableVertexAttrib	fun	0	r	addr_fun(disableVertexAttrib)
		// void disableVertexAttrib(ulong va-ok);
		// void disableVertexAttrib(ulong va-ok, znum index);
		// 禁用顶点属性
	useVertexAttrib		fun	0	r	addr_fun(useVertexAttrib)
		// long error = useVertexAttrib(ulong va-ok);
		// 使用顶点属性
	
	// model
	createModel		fun	0	r	addr_fun(createModel)
		// ulong model = createModel(ulong program-ok);
	deleteModel		fun	0	r	addr_fun(deleteModel)
		// void delete(ulong model);
	finalModel		fun	0	r	addr_fun(finalModel)
		// void finalModel(ulong model);
	modelLinkUniform	fun	0	r	addr_fun(modelLinkUniform)
		// long error = modelLinkUniform(ulong model, string name, [u]int[] sync, UniformType);
		// long error = modelLinkUniform(ulong model, string name, [u]int[] sync, UniformType, znum count);
		// long error = modelLinkUniform(ulong model, string name, [u]int[] sync, UniformType, znum begin, znum count);
		// long error = modelLinkUniform(ulong model, string name, [u]int[] sync, UniformType, znum begin, znum count, znum transpose);
	mcRunScript		fun	0	r	addr_fun(mcRunScript)
		// long error = mcRunScript(ulong model, string script);
	mcSyncUniform		fun	0	r	addr_fun(mcSyncUniform)
		// long error = mcSyncUniform(ulong model);
	mcBindBuffer		fun	0	r	addr_fun(mcBindBuffer)
		// long error = mcBindBuffer(ulong model, ulong buffer);
	mcBindVertexAttrib	fun	0	r	addr_fun(mcBindVertexAttrib)
		// long error = mcBindVertexAttrib(ulong model, ulong va-ok);
	mcDrawArrays		fun	0	r	addr_fun(mcDrawArrays)
		// long error = mcDrawArrays(ulong model, DrawMode, znum count);
		// long error = mcDrawArrays(ulong model, DrawMode, znum begin, znum count);
	mcDrawElements		fun	0	r	addr_fun(mcDrawElements)
		// long error = mcDrawElements(ulong model, DrawMode, znum count);
		// long error = mcDrawElements(ulong model, DrawMode, znum begin, znum count);
		// long error = mcDrawElements(ulong model, DrawMode, znum begin, znum count, znum base-vertex);
	
	modelDraw		fun	0	r	addr_fun(modelDraw)
		// long error = modelDraw(ulong model-ok);
	
	// draw
	drawClear		fun	0	r	addr_fun(drawClear)
		// void drawClear();
		// void drawClear(ClearBit);
		// 清除缓存
	drawFlush		fun	0	r	addr_fun(drawFlush)
		// void drawFlush();
	drawFinish		fun	0	r	addr_fun(drawFinish)
		// void drawFinish();
	drawArrays		fun	0	r	addr_fun(drawArrays)
		// void drawArrays(ulong va-ok, DrawMode, znum count);
		// void drawArrays(ulong va-ok, DrawMode, znum begin, znum count);
	drawElements		fun	0	r	addr_fun(drawElements)
		// void drawArrays(ulong va-ok, ulong buffer-ebo, DrawMode, znum count);
		// void drawArrays(ulong va-ok, ulong buffer-ebo, DrawMode, znum begin, znum count);
		// void drawArrays(ulong va-ok, ulong buffer-ebo, DrawMode, znum begin, znum count, znum base-vertex);
	
	// system
	initWindow		fun	0	r	addr_fun(initWindow)
		// void initWindow([string title,] znum width, znum height);
	getOpenGLError		fun	0	r	addr_fun(getOpenGLError)
		// long getOpenGLError();
	
	// link
	program		vmat	0	r	{
		type		-link	.info.ShaderType
		
		create		-link	.createProgram
		delete		-link	.deleteProgram
		attach		-link	.attachShader
		shader		-link	.attachShader
		compile		-link	.attachShader
		link		-link	.linkProgram
		use		-link	.useProgram
	}
	compileShader		-link	.attachShader
	
	buffer		vmat	0	r	{
		type		-link	.info.BufferType
		usage		-link	.info.BufferUsage
		
		create		-link	.createBuffer
		delete		-link	.deleteBuffer
		setData		-link	.bufferSetData
		subData		-link	.bufferSubData
		use		-link	.useBuffer
	}
	
	texture		vmat	0	r	{
		type		-link	.info.TextureType
		iFormat		-link	.info.TextureInternalFormat
		eFormat		-link	.info.TextureExternalFormat
		eType		-link	.info.TextureExternalType
		cubeFace	-link	.info.TextureCubeFace
		
		create		-link	.createTexture
		delete		-link	.deleteTexture
		storage		-link	.textureStorage
		subImage	-link	.textureSubImage
	}
	
	vertexAttrib	vmat	0	r	{
		size		-link	.info.VertexAttribSize
		type		-link	.info.VertexAttribType
		
		create		-link	.createVertexAttrib
		delete		-link	.deleteVertexAttrib
		final		-link	.finalVertexAttrib
		pointer		-link	.vertexAttribPointer
		enable		-link	.enableVertexAttrib
		disable		-link	.disableVertexAttrib
		use		-link	.useVertexAttrib
	}
	
	model		vmat	0	r	{
		uf_type			-link	.info.UniformType
		
		create			-link	.createModel
		delete			-link	.deleteModel
		final			-link	.finalModel
		linkUniform		-link	.modelLinkUniform
		mcRunScript		-link	.mcRunScript
		mcSyncUniform		-link	.mcSyncUniform
		mcBindBuffer		-link	.mcBindBuffer
		mcBindVertexAttrib	-link	.mcBindVertexAttrib
		mcDrawArrays		-link	.mcDrawArrays
		mcDrawElements		-link	.mcDrawElements
		
		draw			-link	.modelDraw
	}
	drawModel		-link	.modelDraw
	
	
	draw		vmat	0	r	{
		cbit		-link	.info.ClearBit
		mode		-link	.info.DrawMode
		
		clear		-link	.drawClear
		flush		-link	.drawFlush
		finish		-link	.drawFinish
		arrays		-link	.drawArrays
		elements	-link	.drawElements
		
		model		-link	.modelDraw
	}
}

