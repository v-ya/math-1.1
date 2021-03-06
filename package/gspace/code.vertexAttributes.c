
u64 createVertexAttrib(void)
{
	var *o;
	u64 sid;
	GLuint va;
	
	if _oF(!isInit) return 0;
	
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);
	
	if _oF(!va) return 0;
	if _oF(!(o=createSrc(V_vertexAttrib, tlog_vlist, &sid))) goto Err_1;
	
	if _oF(!base->create_vmat(o, S_buffer, 0, auth_read)) goto Err;
	if _oF(!base->create_vmat(o, S_location, 0, auth_read)) goto Err;
	if _oF(!base->create_vmat(o, S_list, 0, auth_read)) goto Err;
	
	if _oF(!base->create_uint(o, S_handle, 0, auth_read, va)) goto Err;
	
	return sid;
	
	Err:
	base->var_delete_index(V_vertexAttrib, sid);
	Err_1:
	glDeleteVertexArrays(1, &va);
	return 0;
}

void vertexAttrib_clearBuffer(var *v)
{
	deleteBuffer(v->v.v_long);
}

void deleteVertexAttrib(u64 sid)
{
	var *vp;
	GLuint va;
	
	vp = base->var_find_index(V_vertexAttrib, sid);
	if _oT(vp)
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			va = getObjectHandle(vp);
			glDeleteVertexArrays(1, &va);
			base->clear_vmsrc(vp, S_buffer, vertexAttrib_clearBuffer);
			base->var_delete_index(V_vertexAttrib, sid);
		}
	}
}

void deleteUserVertexAttrib(u64 sid)
{
	var *vp;
	GLuint va;
	
	vp = base->var_find_index(V_vertexAttrib, sid);
	if _oT(vp && !(vp->mode&F_userDelete))
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			va = getObjectHandle(vp);
			glDeleteVertexArrays(1, &va);
			base->clear_vmsrc(vp, S_buffer, vertexAttrib_clearBuffer);
			base->var_delete_index(V_vertexAttrib, sid);
		}
		vp->mode|=F_userDelete;
	}
}

int vertexAttribPointer(var *v, u64 bid, GLuint index,
	GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer)
{
	var *vb, *vi, *vl;
	u64 i;
	GLuint buffer, va;
	int r;
	
	if _oF(!v) return 1;
	buffer = getHandle(V_buffer, bid, F_type_t(srcBufferTypeVertexAttributes)|F_isok, NULL);
	if _oF(!buffer) return 2;
	va = getObjectHandle(v);
	if _oF(!va) return -1;
	
	normalized = normalized?GL_TRUE:GL_FALSE;
	switch(size)
	{
		case 1:
		case 2:
		case 3:
		case 4:
			break;
		case GL_BGRA:
			if _oF(type!=GL_UNSIGNED_BYTE &&
				type!=GL_INT_2_10_10_10_REV &&
				type!=GL_UNSIGNED_INT_2_10_10_10_REV)
				return 5;
			if _oF(normalized!=GL_TRUE) return 5;
			break;
		default:
			return 3;
	}
	switch(type)
	{
		case GL_FLOAT:
		case GL_DOUBLE:
		case GL_HALF_FLOAT:
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_INT:
		case GL_UNSIGNED_INT:
		case GL_FIXED:
			break;
		case GL_INT_2_10_10_10_REV:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
			if _oT(size==4 || size==GL_BGRA) break;
			return 5;
		default:
			return 4;
	}
	
	if _oF(index >= GL_MAX_VERTEX_ATTRIBS) return -2;
	if _oF(stride < 0) return -3;
	
	// set object
	vb = base->var_find(v, S_buffer);
	vi = base->var_find(v, S_location);
	vl = base->var_find(v, S_list);
	if _oT(vb && vi && vl) ;
	else return -1;
	
	i = vl->v.v_vmat->number;
	r = createSrcRefer(vi, NULL, index, i);
	switch(r)
	{
		case 0:
			break;
		case 1:
			if _oT(base->create_ulong(vl, NULL, i, auth_read, index)) break;
			deleteSrcRefer(vi, NULL, index);
		default:
			return -4;
	}
	if _oF(createSrcRefer(vb, V_buffer, bid, bid) < 0)
	{
		if _oT(r > 0)
		{
			deleteSrcRefer(vi, NULL, index);
			base->var_delete_index(vl, i);
		}
		return -4;
	}
	
	glBindVertexArray(va);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	
	return 0;
}

void setVertexAttrib(var *v, u32 enable, u64 index)
{
	var *vp;
	u64 i, n;
	GLuint va;
	
	if _oF(!v) return ;
	va = getObjectHandle(v);
	if _oF(!va) return ;
	
	glBindVertexArray(va);
	if _oF(index&0x8000000000000000)
	{
		v = base->var_find(v, S_list);
		n = v->v.v_vmat->number;
		if _oT(enable)
		{
			for(i=0;i<n;i++)
			{
				vp = base->var_find_index(v, i);
				if _oT(vp) glEnableVertexAttribArray(vp->v.v_long);
			}
		}
		else
		{
			for(i=0;i<n;i++)
			{
				vp = base->var_find_index(v, i);
				if _oT(vp) glDisableVertexAttribArray(vp->v.v_long);
			}
		}
	}
	else
	{
		if _oT(enable) glEnableVertexAttribArray(index);
		else glDisableVertexAttribArray(index);
	}
}

int useVertexAttrib(u64 sid)
{
	GLuint va;
	
	va = getObjectHandle(getObject(V_vertexAttrib, sid, F_isok, NULL));
	if _oF(!va) return 1;
	
	glBindVertexArray(va);
	return 0;
}

