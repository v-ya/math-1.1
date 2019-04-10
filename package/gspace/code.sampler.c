
u64 createSampler(void)
{
	var *vp;
	u64 sid;
	GLuint t;
	
	if _oF(!isInit) return 0;
	
	glGenSamplers(1, &t);
	glBindSampler(0, t);
	if _oF(!t) return 0;
	
	if _oF(!(vp=createSrc(V_sampler, tlog_int, &sid)))
	{
		glDeleteTextures(1, &t);
		return 0;
	}
	vp->type |= type_unsign;
	vp->mode |= F_isok;
	vp->v.v_long = t;
	return sid;
}

void deleteSampler(u64 sid)
{
	var *vp;
	GLuint t;
	
	vp = base->var_find_index(V_sampler, sid);
	if _oT(vp)
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			t = vp->v.v_long;
			glDeleteSamplers(1, &t);
			base->var_delete_index(V_sampler, sid);
		}
	}
}

void deleteUserSampler(u64 sid)
{
	var *vp;
	GLuint t;
	
	vp = base->var_find_index(V_sampler, sid);
	if _oT(vp && !(vp->mode&F_userDelete))
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			t = vp->v.v_long;
			glDeleteSamplers(1, &t);
			base->var_delete_index(V_sampler, sid);
		}
		vp->mode|=F_userDelete;
	}
}

int useSampler(u64 sid, u32 active)
{
	GLuint sampler;
	
	if _oF(!sid)
	{
		sampler = 0;
		goto bind;
	}
	
	sampler = getHandle(V_sampler, sid, F_isok, NULL);
	
	if _oT(sampler)
	{
		bind:
		if _oF(active >= TEXTURE_ACTIVE_MAX) return 2;
		glBindSampler(active, sampler);
		return 0;
	}
	else return 1;
}

