
#define NotTarget	((GLenum)-1)

GLenum getTextureTarget(u32 type)
{
	static GLenum list[srcTextureTypeMax] = {
		NotTarget,
		GL_TEXTURE_1D,
		GL_TEXTURE_1D_ARRAY,
		GL_TEXTURE_2D,
		GL_TEXTURE_2D_ARRAY,
		GL_TEXTURE_2D_MULTISAMPLE,
		GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
		GL_TEXTURE_3D,
		GL_TEXTURE_CUBE_MAP,
		GL_TEXTURE_CUBE_MAP_ARRAY,
		GL_TEXTURE_RECTANGLE,
		GL_TEXTURE_BUFFER
	};
	if _oF(type >= srcTextureTypeMax) return NotTarget;
	else return list[type];
}

u64 createTexture(u32 type)
{
	var *vp;
	u64 sid;
	GLenum ty;
	GLuint t;
	
	if _oF(!isInit) return 0;
	
	ty = getTextureTarget(type);
	if _oF(ty == NotTarget) return 0;
	glGenTextures(1, &t);
	glBindTexture(ty, t);
	if _oF(!t) return 0;
	
	if _oF(!(vp=createSrc(V_texture, tlog_int, &sid)))
	{
		glDeleteTextures(1, &t);
		return 0;
	}
	vp->type |= type_unsign;
	F_type_s(vp->mode, type);
	vp->v.v_long = t;
	return sid;
}

void deleteTexture(u64 sid)
{
	var *vp;
	GLuint t;
	
	vp = base->var_find_index(V_texture, sid);
	if _oT(vp)
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			t = vp->v.v_long;
			glDeleteTextures(1, &t);
			base->var_delete_index(V_texture, sid);
		}
	}
}

void deleteUserTexture(u64 sid)
{
	var *vp;
	GLuint t;
	
	vp = base->var_find_index(V_texture, sid);
	if _oT(vp && !(vp->mode&F_userDelete))
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			t = vp->v.v_long;
			glDeleteTextures(1, &t);
			base->var_delete_index(V_texture, sid);
		}
		vp->mode|=F_userDelete;
	}
}

int isTextureFormat(GLenum format)
{
	switch(format)
	{
		case GL_R8:
		case GL_R8_SNORM:
		case GL_R16:
		case GL_R16_SNORM:
		case GL_RG8:
		case GL_RG8_SNORM:
		case GL_RG16:
		case GL_RG16_SNORM:
		case GL_R3_G3_B2:
		case GL_RGB4:
		case GL_RGB5:
		case GL_RGB8:
		case GL_RGB8_SNORM:
		case GL_RGB10:
		case GL_RGB12:
		case GL_RGB16_SNORM:
		case GL_RGBA2:
		case GL_RGBA4:
		case GL_RGB5_A1:
		case GL_RGBA8:
		case GL_RGBA8_SNORM:
		case GL_RGB10_A2:
		case GL_RGB10_A2UI:
		case GL_RGBA12:
		case GL_RGBA16:
		case GL_SRGB8:
		case GL_SRGB8_ALPHA8:
		case GL_R16F:
		case GL_RG16F:
		case GL_RGB16F:
		case GL_RGBA16F:
		case GL_R32F:
		case GL_RG32F:
		case GL_RGB32F:
		case GL_RGBA32F:
		case GL_R11F_G11F_B10F:
		case GL_RGB9_E5:
		case GL_R8I:
		case GL_R8UI:
		case GL_R16I:
		case GL_R16UI:
		case GL_R32I:
		case GL_R32UI:
		case GL_RG8I:
		case GL_RG8UI:
		case GL_RG16I:
		case GL_RG16UI:
		case GL_RG32I:
		case GL_RG32UI:
		case GL_RGB8I:
		case GL_RGB8UI:
		case GL_RGB16I:
		case GL_RGB16UI:
		case GL_RGB32I:
		case GL_RGB32UI:
		case GL_RGBA8I:
		case GL_RGBA8UI:
		case GL_RGBA16I:
		case GL_RGBA16UI:
		case GL_RGBA32I:
		case GL_RGBA32UI:
			return 1;
		default:
			return 0;
	}
}

int textureStorage(GLuint texture, u32 type, GLuint lsb, GLenum format, GLsizei w, GLsizei h, GLsizei d, GLboolean fixed)
{
	GLenum t;
	int r;
	
	if _oF(!texture) return 1;
	if _oF(!isTextureFormat(format)) return 2;
	if _oF(type==srcTextureTypeBuffer && !lsb) return 3;
	
	t = getTextureTarget(type);
	if _oF(t == NotTarget) return -1;
	
	// clear error
	while(glGetError()) ;
	
	glBindTexture(t, texture);
	r = glGetError();
	if _oF(r) return r;
	
	fixed = fixed?GL_TRUE:GL_FALSE;
	
	switch(type)
	{
		case srcTextureType1D:
			glTexStorage1D(t, lsb, format, w);
			break;
		case srcTextureType2D:
		case srcTextureType1DArray:
		case srcTextureTypeCube:
		case srcTextureTypeRect:
			glTexStorage2D(t, lsb, format, w, h);
			break;
		case srcTextureType3D:
		case srcTextureType2DArray:
		case srcTextureTypeCubeArray:
			glTexStorage3D(t, lsb, format, w, h, d);
			break;
		case srcTextureType2DMS:
			glTexImage2DMultisample(t, lsb, format, w, h, fixed);
			break;
		case srcTextureType2DMSArray:
			glTexImage3DMultisample(t, lsb, format, w, h, d, fixed);
			break;
		case srcTextureTypeBuffer:
			glTexBuffer(t, format, lsb);
			break;
		default:
			return -1;
	}
	
	if _oF(r=glGetError()) return r;
	
	return 0;
}

int isTextureEFT(GLenum format, GLenum type, u32 *psz)
{
	u32 size = 1;
	switch(format)
	{
		case GL_RED:
			size = 1;
			break;
		case GL_RG:
			size = 2;
			break;
		case GL_RGB:
		case GL_BGR:
			size = 3;
			break;
		case GL_RGBA:
		case GL_BGRA:
			size = 4;
			break;
		case GL_DEPTH_COMPONENT:
		case GL_STENCIL_INDEX:
			size = 1;
			break;
		default:
			return 0;
	}
	switch(type)
	{
		case GL_UNSIGNED_BYTE:
		case GL_BYTE:
			size *= sizeof(GLbyte);
			break;
		case GL_UNSIGNED_SHORT:
		case GL_SHORT:
			size *= sizeof(GLshort);
			break;
		case GL_UNSIGNED_INT:
		case GL_INT:
			size *= sizeof(GLint);
			break;
		case GL_FLOAT:
			size *= sizeof(GLfloat);
			break;
		case GL_UNSIGNED_BYTE_3_3_2:
		case GL_UNSIGNED_BYTE_2_3_3_REV:
			size = sizeof(GLubyte);
			goto _l1;
		case GL_UNSIGNED_SHORT_5_6_5:
		case GL_UNSIGNED_SHORT_5_6_5_REV:
			size = sizeof(GLushort);
			_l1:
			if _oF(format!=GL_RGB && format!=GL_BGR) return 0;
			break;
		case GL_UNSIGNED_SHORT_4_4_4_4:
		case GL_UNSIGNED_SHORT_4_4_4_4_REV:
		case GL_UNSIGNED_SHORT_5_5_5_1:
		case GL_UNSIGNED_SHORT_1_5_5_5_REV:
			size = sizeof(GLushort);
			goto _l2;
		case GL_UNSIGNED_INT_8_8_8_8:
		case GL_UNSIGNED_INT_8_8_8_8_REV:
		case GL_UNSIGNED_INT_10_10_10_2:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
			size = sizeof(GLuint);
			_l2:
			if _oF(format!=GL_RGBA && format!=GL_BGRA) return 0;
			break;
	}
	if _oT(psz) *psz = size;
	return 1;
}

int textureSubImage(GLuint texture, u32 type, GLint level, u32 cube, GLenum format, GLenum ft,
	GLvoid *pixels, u64 length, GLint x, GLint y, GLint z, GLsizei w, GLsizei h, GLsizei d)
{
	GLenum t;
	u32 sz;
	u64 size;
	int r;
	
	if _oF(!texture) return 1;
	if _oF(!isTextureEFT(format, ft, &sz)) return 2;
	switch(type)
	{
		case srcTextureType1D:
			size = (u64) sz * w;
			break;
		case srcTextureType2D:
		case srcTextureType1DArray:
		case srcTextureTypeCube:
		case srcTextureTypeRect:
			size = (u64) sz * w * h;
			break;
		case srcTextureType3D:
		case srcTextureType2DArray:
		case srcTextureTypeCubeArray:
			size = (u64) sz * w * h * d;
			break;
		case srcTextureType2DMS:
		case srcTextureType2DMSArray:
		case srcTextureTypeBuffer:
			return 3;
		default:
			return -1;
	}
	
	if _oF(!pixels || size>length) return 4;
	
	t = getTextureTarget(type);
	if _oF(t == NotTarget) return -1;
	if _oF(t == GL_TEXTURE_CUBE_MAP) switch(cube)
	{
		case 0:
			t = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			break;
		case 1:
			t = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			break;
		case 2:
			t = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			break;
		case 3:
			t = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			break;
		case 4:
			t = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			break;
		case 5:
			t = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
			break;
		default:
			return 5;
	}
	
	// clear error
	while(glGetError()) ;
	
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	r = glGetError();
	if _oF(r) return r;
	glBindTexture(t, texture);
	r = glGetError();
	if _oF(r) return r;
	
	if _oT(size) switch(type)
	{
		case srcTextureType1D:
			glTexSubImage1D(t, level, x, w, format, ft, pixels);
			break;
		case srcTextureType2D:
		case srcTextureType1DArray:
		case srcTextureTypeCube:
		case srcTextureTypeRect:
			glTexSubImage2D(t, level, x, y, w, h, format, ft, pixels);
			break;
		case srcTextureType3D:
		case srcTextureType2DArray:
		case srcTextureTypeCubeArray:
			glTexSubImage3D(t, level, x, y, z, w, h, d, format, ft, pixels);
			break;
		default:
			return -1;
	}
	
	if _oF(r=glGetError()) return r;
	
	return 0;
}

int useTexture(u64 sid, u32 type, u32 active)
{
	GLuint texture;
	GLenum t;
	
	texture = getHandle(V_texture, sid, F_type_t(type)|F_isok, &type);
	t = getTextureTarget(F_type_g(type));
	
	if _oT(texture && t!=NotTarget)
	{
		if _oF(active >= TEXTURE_ACTIVE_MAX) return 1;
		glActiveTexture(GL_TEXTURE0 + active);
		glBindTexture(t, texture);
		return 0;
	}
	else return 1;
}

#undef NotTarget

