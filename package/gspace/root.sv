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
			extensions	string	0	r	NULL
			
			
			INFO_OpenglVendor			-export	.info.OpenGL.vendor
			INFO_OpenglRenderer			-export	.info.OpenGL.renderer
			INFO_OpenglVersion			-export	.info.OpenGL.version
			INFO_OpenglShadingLanguageVersion	-export	.info.OpenGL.glslv
			INFO_OpenglExtensions			-export	.info.OpenGL.extensions
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
			clearBit	ulong	0	rw	DEFAULT_CLEAR_BIT
			
			VAR_ClearBit		-export	.set.run.clearBit
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
		// long error = vertexAttribPointer(ulong va, ulong buffer, znum index,
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
	
	// test
	test			fun	0	r	addr_fun(test)
	
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
		create			-link	.createModel
		delete			-link	.deleteModel
		final			-link	.finalModel
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

