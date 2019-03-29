
u64 createProgram(void)
{
	var *vp;
	u64 sid;
	GLuint p;
	
	if _oF(!isInit) return 0;
	
	p = glCreateProgram();
	if _oF(!p) return 0;
	if _oF(!(vp=createSrc(V_program, tlog_int, &sid)))
	{
		glDeleteProgram(p);
		return 0;
	}
	vp->type |= type_unsign;
	vp->v.v_long = p;
	return sid;
}

void deleteProgram(u64 sid)
{
	var *vp;
	
	vp = base->var_find_index(V_program, sid);
	if _oT(vp)
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			glDeleteProgram(vp->v.v_long);
			base->var_delete_index(V_program, sid);
		}
	}
}

void deleteUserProgram(u64 sid)
{
	var *vp;
	
	vp = base->var_find_index(V_program, sid);
	if _oT(vp && !(vp->mode&F_userDelete))
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			glDeleteProgram(vp->v.v_long);
			base->var_delete_index(V_program, sid);
		}
		vp->mode|=F_userDelete;
	}
}

int attachShader(GLuint program, GLenum type, u32 N, char *code[], char **error)
{
	char *e = NULL;
	GLuint shader;
	GLint status, size;
	u32 i;
	
	if _oF(!program)
	{
		e = "Not Find Program\n";
		goto Err;
	}
	if _oF(!code)
	{
		e = "Shader Code Array = NULL\n";
		goto Err;
	}
	for(i=0;i<N;i++)
	{
		if _oF(!code[i])
		{
			e = "Shader Code = NULL\n";
			goto Err;
		}
	}
	switch(type)
	{
		case GL_COMPUTE_SHADER:
		case GL_VERTEX_SHADER:
		case GL_TESS_CONTROL_SHADER:
		case GL_TESS_EVALUATION_SHADER:
		case GL_GEOMETRY_SHADER:
		case GL_FRAGMENT_SHADER:
			break;
		default:
			e = "Unknow Shader Type\n";
			goto Err;
	}
	
	shader = glCreateShader(type);
	if _oF(!shader)
	{
		e = "Create Shader Fail\n";
		goto Err;
	}
	glShaderSource(shader, N, (const GLchar * const *) code, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if _oF(!status)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
		*error = malloc(size+1);
		if _oF(!*error)
		{
			glDeleteShader(shader);
			return -1;
		}
		glGetShaderInfoLog(shader, size, &size, *error);
		(*error)[size] = 0;
		glDeleteShader(shader);
		return 1;
	}
	glAttachShader(program,shader);
	glDeleteShader(shader);
	
	return 0;
	Err:
	size = strlen(e)+1;
	*error = malloc(size);
	if _oF(!*error) return -1;
	memcpy(*error,e,size);
	return 1;
}

int linkProgram(GLuint program, char **error)
{
	char *e = NULL;
	GLint status, size;
	
	if _oF(!program)
	{
		e = "Not Find Program\n";
		goto Err;
	}
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if _oF(!status)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
		*error = malloc(size+1);
		if _oF(!*error) return -1;
		glGetProgramInfoLog(program, size, &size, *error);
		(*error)[size] = 0;
		return 1;
	}
	return 0;
	Err:
	size = strlen(e)+1;
	*error = malloc(size);
	if _oF(!*error) return -1;
	memcpy(*error,e,size);
	return 1;
}

int useProgram(u64 sid)
{
	GLuint program;
	
	program = getHandle(V_program, sid, F_isok, NULL);
	if _oT(program)
	{
		glUseProgram(program);
		return 0;
	}
	else return 1;
}

