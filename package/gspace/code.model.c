
u64 createModel(u64 program)
{
	var *o, *bind, *vp;
	u64 sid;
	
	if _oF(!isInit) return 0;
	
	if _oF(!getHandle(V_program, program, F_isok, NULL)) return 0;
	referSrcHandle(V_program, program);
	
	if _oF(!(o=createSrc(V_model, tlog_vmat, &sid))) goto Err_1;
	
	// src refer
	if _oF(!base->create_ulong(o, S_program, 0, auth_read, program)) goto Err;
	if _oF(!base->create_vmat(o, S_buffer, 0, auth_read)) goto Err;
	if _oF(!base->create_vmat(o, S_vertexAttrib, 0, auth_read)) goto Err;
	
	// this bind
	if _oF(!(bind=base->create_vlist(o, S_bind, 0, auth_read))) goto Err;
	if _oF(!(vp=base->create_var(bind, S_buffer, 0, tlog_long, srcBufferTypeMax, auth_read))) goto Err;
	vp->type |= type_unsign;
	if _oF(!base->create_ulong(bind, S_vertexAttrib, 0, auth_read, 0)) goto Err;
	
	// command
	if _oF(!base->create_vmat(o, S_command, 0, auth_read)) goto Err;
	
	// script && env
	if _oF(!base->create_vmat(o, S_script, 0, auth_read)) goto Err;
	if _oF(!base->create_vmat(o, S_env, 0, auth_read|auth_write)) goto Err;
	
	return sid;
	
	Err:
	base->var_delete_index(V_model, sid);
	Err_1:
	deleteProgram(program);
	return 0;
}

void _model_d_b(var *v) {deleteBuffer(v->v.v_long);}
void _model_d_va(var *v) {deleteVertexAttrib(v->v.v_long);}
void model_clearSrc(var *v)
{
	var *vp;
	
	vp = base->var_find(v, S_program);
	if _oT(vp) deleteProgram(vp->v.v_long);
	
	base->clear_vmsrc(v, S_buffer, _model_d_b);
	base->clear_vmsrc(v, S_vertexAttrib, _model_d_va);
}

void deleteModel(u64 sid)
{
	var *vp;
	
	vp = base->var_find_index(V_model, sid);
	if _oT(vp)
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			model_clearSrc(vp);
			base->var_delete_index(V_model, sid);
		}
	}
}

void deleteUserModel(u64 sid)
{
	var *vp;
	
	vp = base->var_find_index(V_model, sid);
	if _oT(vp && !(vp->mode&F_userDelete))
	{
		if _oT(vp->inode>1) vp->inode-=1;
		else
		{
			model_clearSrc(vp);
			base->var_delete_index(V_model, sid);
		}
		vp->mode|=F_userDelete;
	}
}

void finalModel(u64 sid)
{
	var *vp;
	
	vp = base->var_find_index(V_model, sid);
	if _oT(vp && !(vp->mode&F_isok))
	{
		base->var_delete(vp, S_bind);
		vp->mode |= F_isok;
	}
}

ulong* modelNewCommand(var *vp, u32 argc)
{
	var *v;
	
	if _oF(argc > MODEL_COMMAND_ARGC_MAX) return NULL;
	
	vp = base->var_find(vp, S_command);
	if _oF(!vp) return NULL;
	
	v = base->create_var(vp, NULL, vp->v.v_vmat->number, tlog_long, argc, auth_read);
	if _oF(!v) return NULL;
	v->type |= type_unsign;
	
	return v->v.vp_long;
}

void modelDeleteCommand(var *vp, u32 n)
{
	u32 m;
	
	vp = base->var_find(vp, S_command);
	if _oF(!vp) return ;
	
	m = vp->v.v_vmat->number;
	if _oT(m > n) n = m - n;
	else n = 0;
	
	for(;n<m;n++)
	{
		base->var_delete_index(vp, n);
	}
}

int modelRunScript(var *vp, char *script)
{
	var *vs;
	u64 i, *c;
	
	if _oF(!vp) return 1;
	
	vs = base->var_find(vp, S_script);
	if _oF(!vs || !script) return 1;
	
	i = vs->v.v_vmat->number;
	if _oF(!base->create_string(vs, NULL, i, auth_read, script)) return -1;
	
	c = modelNewCommand(vp, 2);
	if _oF(!c)
	{
		base->var_delete_index(vs, i);
		return -1;
	}
	c[0] = mcRunScript;
	c[1] = i;
	
	return 0;
}

int modelSyncUniform(var *vp)
{
	u64 *c;
	
	if _oF(!vp) return 1;
	
	c = modelNewCommand(vp, 1);
	if _oF(!c) return -1;
	c[0] = mcSyncUniform;
	
	return 0;
}

int modelSetBindBuffer(var *vp, u32 type, u64 buffer, u32 *ptype, u64 *plast)
{
	vp = base->var_find(vp, S_bind);
	vp = base->var_find(vp, S_buffer);
	if _oF(!vp) goto Err;
	
	if _oT(!plast || type<srcBufferTypeMax && getHandle(V_buffer, buffer, F_type_t(type)|F_isok, &type))
	{
		if _oT(plast) type = F_type_g(type);
		if _oT(ptype) *ptype = type;
		if _oT(plast) *plast = vp->v.vp_long[type];
		if _oF(vp->v.vp_long[type] == buffer) return 0;
		else
		{
			vp->v.vp_long[type] = buffer;
			return 1;
		}
	}
	else
	{
		Err:
		if _oT(ptype) *ptype = 0;
		if _oT(plast) *plast = 0;
		return -1;
	}
}

int modelBindBuffer(var *vp, u64 buffer)
{
	u32 type;
	u64 last, *c;
	var *vb;
	
	if _oF(!vp) return 1;
	vb = base->var_find(vp, S_buffer);
	if _oF(!vb) return -1;
	
	if _oF(createSrcRefer(vb, V_buffer, buffer, buffer) < 0) return -1;
	
	switch(modelSetBindBuffer(vp, srcBufferTypeAll, buffer, &type, &last))
	{
		case 0:
			break;
		case 1:
			c = modelNewCommand(vp, 2);
			if _oF(!c)
			{
				modelSetBindBuffer(vp, type, last, NULL, NULL);
				return -1;
			}
			c[0] = mcBindBuffer;
			c[1] = buffer;
			break;
		default:
			return -1;
	}
	
	return 0;
}

int modelSetVertexAttrib(var *vp, u64 va, u64 *plast)
{
	vp = base->var_find(vp, S_bind);
	vp = base->var_find(vp, S_vertexAttrib);
	if _oF(!vp) goto Err;
	
	if _oT(!plast || getObjectHandle(getObject(V_vertexAttrib, va, F_isok, NULL)))
	{
		if _oT(plast) *plast = vp->v.v_long;
		if _oF(vp->v.v_long == va) return 0;
		else
		{
			vp->v.v_long = va;
			return 1;
		}
	}
	else
	{
		Err:
		if _oT(plast) *plast = 0;
		return -1;
	}
}

int modelBindVertexAttrib(var *vp, u64 va)
{
	u64 last, *c;
	var *vv;
	
	if _oF(!vp) return 1;
	vv = base->var_find(vp, S_vertexAttrib);
	if _oF(!vv) return -1;
	
	if _oF(createSrcRefer(vv, V_vertexAttrib, va, va) < 0) return -1;
	
	switch(modelSetVertexAttrib(vp, va, &last))
	{
		case 0:
			break;
		case 1:
			c = modelNewCommand(vp, 2);
			if _oF(!c)
			{
				modelSetVertexAttrib(vp, last, NULL);
				return -1;
			}
			c[0] = mcBindVertexAttrib;
			c[1] = va;
			break;
		default:
			return -1;
	}
	
	return 0;
}

int modelDrawArrays(var *vp, u32 mode, u32 begin, u32 count)
{
	u64 *c;
	
	if _oF(!vp) return 1;
	if _oF(!isDrawMode(mode)) return 1;
	
	c = modelNewCommand(vp, 4);
	if _oF(!c) return -1;
	
	c[0] = mcDrawArrays;
	c[1] = mode;
	c[2] = begin;
	c[3] = count;
	
	return 0;
}

int modelDrawElements(var *vp, u32 mode, u32 begin, u32 count, u32 pbase)
{
	u64 *c;
	
	if _oF(!vp) return 1;
	if _oF(!isDrawMode(mode)) return 1;
	
	c = modelNewCommand(vp, 5);
	if _oF(!c) return -1;
	
	c[0] = mcDrawElements;
	c[1] = mode;
	c[2] = begin;
	c[3] = count;
	c[4] = pbase;
	
	return 0;
}

// drawModel's function's pointer
#define ModelPointerFunction(name)	void _ModelFunc_##name(u64 c[], var *model)

ModelPointerFunction(RunScript)
{
	var *vp, *e;
	e = base->var_find(model, S_env);
	vp = base->var_find(model, S_script);
	vp = base->var_find_index(vp, c[1]);
	if _oT(vp)
	{
		e = base->run_script(vp, e);
		if _oF(e)
		{
			base->print_error();
			base->clr_error();
		}
	}
}

ModelPointerFunction(SyncUniform)
{
	print("SyncUniform\n");
}

ModelPointerFunction(BindBuffer)
{
	useBuffer(c[1], srcBufferTypeAll);
}

ModelPointerFunction(BindVertexAttrib)
{
	useVertexAttrib(c[1]);
}

ModelPointerFunction(DrawArrays)
{
	glDrawArrays(c[1], c[2], c[3]);
}

ModelPointerFunction(DrawElements)
{
	if _oF(c[4]) glDrawElementsBaseVertex(c[1], c[3], GL_UNSIGNED_INT, (GLvoid *)(c[2] * sizeof(GLuint)), c[4]);
	else glDrawElements(c[1], c[3], GL_UNSIGNED_INT, (GLvoid *)(c[2] * sizeof(GLuint)));
}

#undef ModelPointerFunction

#define getModelFunc(name)		_ModelFunc_##name
ModelFunc ModelFuncPool[mcNumber] = {
	getModelFunc(RunScript),
	getModelFunc(SyncUniform),
	getModelFunc(BindBuffer),
	getModelFunc(BindVertexAttrib),
	
	getModelFunc(DrawArrays),
	getModelFunc(DrawElements),
} ;
#undef getModelFunc

int modelDraw(var *model)
{
	var *command, *vp;
	u64 i, n, c;
	
	command = base->var_find(model, S_command);
	if _oF(!model || !command) return 1;
	
	n = command->v.v_vmat->number;
	for(i=0;i<n;i++)
	{
		vp = base->var_find_index(command, i);
		if _oF(!vp) return -1;
		if _oF(vp->type!=(type_long|type_unsign) || !vp->length) return -1;
		c = vp->v.vp_long[0];
		if _oF(c >= mcNumber) return -1;
		ModelFuncPool[c](vp->v.vp_long, model);
	}
	
	return 0;
}

