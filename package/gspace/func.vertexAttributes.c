
func(createVertexAttrib)
{
	static char *label=label_name("createVertexAttrib");
	static u32 type_1[1]={type_void};
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	ret->type = type_long|type_unsign;
	ret->v.v_long = createVertexAttrib();
	
	return ret;
}

func(deleteVertexAttrib)
{
	static char *label=label_name("deleteVertexAttrib");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	deleteUserVertexAttrib(sid);
	
	ret->type = type_void;
	
	return ret;
}

func(finalVertexAttrib)
{
	static char *label=label_name("finalVertexAttrib");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	setHandleOk(V_vertexAttrib, sid);
	
	ret->type = type_void;
	
	return ret;
}

func(vertexAttribPointer)
{
	static char *label=label_name("vertexAttribPointer");
	static u32 type_8[8]={type_znum, type_znum, type_znum,
		type_znum, type_znum, type_znum, type_znum, type_znum};
	var *v;
	u64 sid, bid, p;
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	
	if _oF(argc!=8||base->check_varlist(argv,8,type_8)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	bid = argv->v->v.v_long;
	argv = argv->r;
	index = argv->v->v.v_long;
	argv = argv->r;
	size = argv->v->v.v_long;
	argv = argv->r;
	type = argv->v->v.v_long;
	argv = argv->r;
	normalized = argv->v->v.v_long;
	argv = argv->r;
	stride = argv->v->v.v_long;
	argv = argv->r;
	p = argv->v->v.v_long;
	
	v = getVertexAttribObject(sid, 0, NULL);
	
	ret->type = type_long;
	ret->v.v_long = vertexAttribPointer(v, bid, index, size, type, normalized, stride, (GLvoid *) p);
	
	return ret;
}

func(enableVertexAttrib)
{
	static char *label=label_name("enableVertexAttrib");
	static u32 type_1[1]={type_znum};
	static u32 type_2[2]={type_znum, type_znum};
	var *v;
	u64 sid;
	GLuint index;
	
	switch(argc)
	{
		case 1:
			if _oF(base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			break;
		case 2:
			if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			index = argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
	}
	
	v = getVertexAttribObject(sid, F_isok, NULL);
	
	ret->type = type_void;
	if _oF(argc == 1) setVertexAttrib(v, 1, (u64)-1);
	else setVertexAttrib(v, 1, index);
	
	return ret;
}

func(disableVertexAttrib)
{
	static char *label=label_name("disableVertexAttrib");
	static u32 type_1[1]={type_znum};
	static u32 type_2[2]={type_znum, type_znum};
	var *v;
	u64 sid;
	GLuint index;
	
	switch(argc)
	{
		case 1:
			if _oF(base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			break;
		case 2:
			if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			index = argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
	}
	
	v = getVertexAttribObject(sid, F_isok, NULL);
	
	ret->type = type_void;
	if _oF(argc == 1) setVertexAttrib(v, 0, (u64)-1);
	else setVertexAttrib(v, 0, index);
	
	return ret;
}

func(useVertexAttrib)
{
	static char *label=label_name("useVertexAttrib");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = useVertexAttrib(sid);
	
	return ret;
}

