
func(createSampler)
{
	static char *label=label_name("createSampler");
	static u32 type_1[1]={type_void};
	u32 type;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	ret->type = type_long|type_unsign;
	ret->v.v_long = createSampler();
	
	return ret;
}

func(deleteSampler)
{
	static char *label=label_name("deleteSampler");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	deleteUserSampler(sid);
	
	ret->type = type_void;
	
	return ret;
}

func(samplerParam)
{
	static char *label=label_name("SamplerParam");
	static u32 type_2[2]={type_znum, type_znum};
	extern var *VAR_SamplerParamLodMin;
	extern var *VAR_SamplerParamLodMax;
	extern var *VAR_SamplerParamBorderColor;
	u64 sid;
	GLuint sampler;
	GLenum pname;
	GLint iv = 0;
	GLfloat bc[4];
	var *vp;
	
	if _oF(argc!=2 && argc!=3) return base->get_error(errid_FunArgvType,label);
	if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	pname = argv->v->v.v_long;
	if _oT(argc > 2)
	{
		argv = argv->r;
		vp = argv->v;
	}
	else vp = NULL;
	
	ret->type = type_long;
	sampler = getHandle(V_sampler, sid, F_isok, NULL);
	
	if _oF(!sampler)
	{
		ret->v.v_long = 1;
		return ret;
	}
	
	if _oT(vp && (vp->type&type_znum)) iv = vp->v.v_long;
	if _oF(vp && (vp->length && !(vp->type&type_float))) goto Err;
	
	switch(pname)
	{
		case GL_TEXTURE_MIN_FILTER:
			if _oF(vp && vp->type) switch(iv)
			{
				case GL_NEAREST:
				case GL_LINEAR:
				case GL_NEAREST_MIPMAP_NEAREST:
				case GL_LINEAR_MIPMAP_NEAREST:
				case GL_NEAREST_MIPMAP_LINEAR:
				case GL_LINEAR_MIPMAP_LINEAR:
					glSamplerParameteri(sampler, pname, iv);
					goto End;
				default:
					goto Err;
			}
			else goto Err;
			break;
		case GL_TEXTURE_MAG_FILTER:
			if _oF(vp && vp->type) switch(iv)
			{
				case GL_NEAREST:
				case GL_LINEAR:
					glSamplerParameteri(sampler, pname, iv);
					goto End;
				default:
					goto Err;
			}
			else goto Err;
			break;
		case GL_TEXTURE_MIN_LOD:
			if _oF(!vp) iv = VAR_SamplerParamLodMin->v.v_long;
			else if _oF(!(vp->type&type_znum)) goto Err;
			glSamplerParameteri(sampler, pname, iv);
			VAR_SamplerParamLodMin->v.v_long = iv;
			goto End;
			break;
		case GL_TEXTURE_MAX_LOD:
			if _oF(!vp) iv = VAR_SamplerParamLodMax->v.v_long;
			else if _oF(!(vp->type&type_znum)) goto Err;
			glSamplerParameteri(sampler, pname, iv);
			VAR_SamplerParamLodMax->v.v_long = iv;
			goto End;
			break;
		case GL_TEXTURE_WRAP_S:
		case GL_TEXTURE_WRAP_T:
		case GL_TEXTURE_WRAP_R:
			if _oF(vp && vp->type) switch(iv)
			{
				case GL_CLAMP_TO_EDGE:
				case GL_MIRRORED_REPEAT:
				case GL_REPEAT:
					glSamplerParameteri(sampler, pname, iv);
					goto End;
				default:
					goto Err;
			}
			else goto Err;
			break;
		case GL_TEXTURE_BORDER_COLOR:
			if _oT(vp)
			{
				if _oT(vp->type&type_znum)
				{
					bc[0] = (double)((iv&0x00ff0000)>>16) / 255;	// R
					bc[1] = (double)((iv&0x0000ff00)>>8) / 255;	// G
					bc[0] = (double)((iv&0x000000ff)>>0) / 255;	// B
					bc[0] = (double)((iv&0xff000000)>>24) / 255;	// A
				}
				else if _oT((vp->type&type_float) && vp->length==4)
				{
					bc[0] = vp->v.vp_float[0];
					bc[1] = vp->v.vp_float[1];
					bc[2] = vp->v.vp_float[2];
					bc[3] = vp->v.vp_float[3];
				}
				else goto Err;
			}
			else
			{
				bc[0] = (VAR_SamplerParamBorderColor->v.vp_float)[0];
				bc[1] = VAR_SamplerParamBorderColor->v.vp_float[1];
				bc[2] = VAR_SamplerParamBorderColor->v.vp_float[2];
				bc[3] = VAR_SamplerParamBorderColor->v.vp_float[3];
			}
			glSamplerParameterfv(sampler, pname, bc);
			VAR_SamplerParamBorderColor->v.vp_float[0] = bc[0];
			VAR_SamplerParamBorderColor->v.vp_float[1] = bc[1];
			VAR_SamplerParamBorderColor->v.vp_float[2] = bc[2];
			VAR_SamplerParamBorderColor->v.vp_float[3] = bc[3];
			goto End;
			break;
		case GL_TEXTURE_COMPARE_MODE:
			if _oF(vp && vp->type) switch(iv)
			{
				case GL_COMPARE_REF_TO_TEXTURE:
				case GL_NONE:
					glSamplerParameteri(sampler, pname, iv);
					goto End;
				default:
					goto Err;
			}
			else goto Err;
			break;
		case GL_TEXTURE_COMPARE_FUNC:
			if _oF(vp && vp->type) switch(iv)
			{
				case GL_LEQUAL:
				case GL_GEQUAL:
				case GL_LESS:
				case GL_GREATER:
				case GL_EQUAL:
				case GL_NOTEQUAL:
				case GL_ALWAYS:
				case GL_NEVER:
					glSamplerParameteri(sampler, pname, iv);
					goto End;
				default:
					goto Err;
			}
			else goto Err;
			break;
		default:
			ret->v.v_long = 2;
			return ret;
	}
	
	End:
	ret->v.v_long = 0;
	return ret;
	Err:
	ret->v.v_long = 3;
	return ret;
}

func(useSampler)
{
	static char *label=label_name("useSampler");
	static u32 type_2[2]={type_znum, type_znum};
	u64 sid;
	u32 active;
	
	if _oF(argc!=2||base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	
	active = argv->v->v.v_long;
	argv = argv->r;
	sid = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = useSampler(sid, active);
	
	return ret;
}

