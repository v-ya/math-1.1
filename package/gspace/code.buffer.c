
#define NotTarget	((GLenum)-1)

GLenum getBufferTarget(u32 type)
{
	static GLenum list[srcBufferTypeMax] = {
		NotTarget,
		GL_ARRAY_BUFFER,
		GL_ATOMIC_COUNTER_BUFFER,
		GL_COPY_READ_BUFFER,
		GL_COPY_WRITE_BUFFER,
		GL_DISPATCH_INDIRECT_BUFFER,
		GL_DRAW_INDIRECT_BUFFER,
		GL_ELEMENT_ARRAY_BUFFER,
		GL_PIXEL_PACK_BUFFER,
		GL_PIXEL_UNPACK_BUFFER,
		GL_QUERY_BUFFER,
		GL_SHADER_STORAGE_BUFFER,
		GL_TEXTURE_BUFFER,
		GL_TRANSFORM_FEEDBACK_BUFFER,
		GL_UNIFORM_BUFFER
	};
	if _oF(type >= srcBufferTypeMax) return NotTarget;
	else return list[type];
}

u64 createBuffer(u32 type)
{
	var *vp;
	u64 sid;
	GLenum t;
	GLuint b;
	
	if _oF(!isInit) return 0;
	
	t = getBufferTarget(type);
	if _oF(t == NotTarget) return 0;
	glGenBuffers(1,&b);
	glBindBuffer(t, b);
	if _oF(!b) return 0;
	
	if _oF(!(vp=createSrc(V_buffer, tlog_int, &sid)))
	{
		glDeleteBuffers(1,&b);
		return 0;
	}
	vp->type |= type_unsign;
	F_type_s(vp->mode, type);
	vp->mode |= F_isok;
	vp->v.v_long = b;
	return sid;
}

void deleteBuffer(u64 sid)
{
	var *vp;
	GLuint b;
	
	vp = base->var_find_index(V_buffer, sid);
	if _oT(vp)
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			b = vp->v.v_long;
			glDeleteBuffers(1, &b);
			base->var_delete_index(V_buffer, sid);
		}
	}
}

void deleteUserBuffer(u64 sid)
{
	var *vp;
	GLuint b;
	
	vp = base->var_find_index(V_buffer, sid);
	if _oT(vp && !(vp->mode&F_userDelete))
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			b = vp->v.v_long;
			glDeleteBuffers(1, &b);
			base->var_delete_index(V_buffer, sid);
		}
		vp->mode|=F_userDelete;
	}
}

#define OPENGL_NEED_MAJOR	4
#define OPENGL_NEED_MINOR	5

int bufferSetData(GLuint buffer, u32 type, GLsizeiptr size, void *data, GLenum usage)
{
	GLenum t;
	
	if _oF(!buffer) return 1;
	switch(usage)
	{
		case GL_STREAM_DRAW:
		case GL_STREAM_READ:
		case GL_STREAM_COPY:
		case GL_STATIC_DRAW:
		case GL_STATIC_READ:
		case GL_STATIC_COPY:
		case GL_DYNAMIC_DRAW:
		case GL_DYNAMIC_READ:
		case GL_DYNAMIC_COPY:
			break;
		default:
			return 2;
	}
	#if OPENGL_MAJOR >= OPENGL_NEED_MAJOR && OPENGL_MINOR >= OPENGL_NEED_MINOR
	glNamedBufferData(buffer, size, data, usage);
	#else
	t = getBufferTarget(type);
	if _oF(t == NotTarget) return -1;
	glBindBuffer(t, buffer);
	glBufferData(t, size, data, usage);
	#endif
	
	return 0;
}

int bufferSubData(GLuint buffer, u32 type, GLintptr offset, GLsizeiptr size, void *data)
{
	GLenum t;
	
	if _oF(!buffer) return 1;
	#if OPENGL_MAJOR >= OPENGL_NEED_MAJOR && OPENGL_MINOR >= OPENGL_NEED_MINOR
	glNamedBufferSubData(buffer, offset, size, data);
	#else
	t = getBufferTarget(type);
	if _oF(t == NotTarget) return -1;
	glBindBuffer(t, buffer);
	glBufferSubData(t, offset, size, data);
	#endif
	
	return 0;
}

#undef OPENGL_NEED_MAJOR
#undef OPENGL_NEED_MINOR

int useBuffer(u64 sid, u32 type)
{
	GLuint buffer;
	GLenum t;
	
	buffer = getHandle(V_buffer, sid, F_type_t(type)|F_isok, &type);
	t = getBufferTarget(F_type_g(type));
	
	if _oT(buffer && t!=NotTarget)
	{
		glBindBuffer(t, buffer);
		return 0;
	}
	else return 1;
}

#undef NotTarget

