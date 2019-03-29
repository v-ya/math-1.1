
func(createProgram)
{
	static char *label=label_name("createProgram");
	static u32 type_1[1]={type_void};
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	ret->type = type_long|type_unsign;
	ret->v.v_long = createProgram();
	
	return ret;
}

func(deleteProgram)
{
	static char *label=label_name("deleteProgram");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	deleteUserProgram(sid);
	
	ret->type = type_void;
	
	return ret;
}

func(attachShader)
{
	static char *label=label_name("attachShader");
	static u32 type_3[3]={type_znum, type_znum, type_string|type_allowarray};
	u64 sid;
	GLuint program;
	GLenum type;
	char *error;
	var *r;
	
	if _oF(argc!=3||base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	type = (u64) argv->v->v.v_long;
	argv = argv->r;
	r = argv->v;
	
	program = getHandle(V_program, sid, 0, NULL);
	if _oF(r->length)
	{
		if _oF(attachShader(program, type, r->length, r->v.vp_string, &error)) goto Err;
	}
	else
	{
		if _oF(attachShader(program, type, 1, &(r->v.v_string), &error)) goto Err;
	}
	
	ret->type = type_void;
	return ret;
	
	Err:
	if _oF(!error) return base->get_error(errid_MemLess,label);
	r=base->var_alloc(tlog_string,leng_no);
	if _oF(!r)
	{
		free(error);
		return base->get_error(errid_MemLess,label);
	}
	r->v.v_string=error;
	r->mode=auth_read|free_pointer;
	return r;
}

func(linkProgram)
{
	static char *label=label_name("linkProgram");
	static u32 type_1[1]={type_znum};
	u64 sid;
	GLuint program;
	char *error;
	var *r;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	
	program = getHandle(V_program, sid, 0, NULL);
	if _oF(linkProgram(program, &error)) goto Err;
	
	// finally
	setHandleOk(V_program, sid);
	
	ret->type = type_void;
	return ret;
	
	Err:
	if _oF(!error) return base->get_error(errid_MemLess,label);
	r=base->var_alloc(tlog_string,leng_no);
	if _oF(!r)
	{
		free(error);
		return base->get_error(errid_MemLess,label);
	}
	r->v.v_string=error;
	r->mode=auth_read|free_pointer;
	return r;
}

func(useProgram)
{
	static char *label=label_name("useProgram");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = useProgram(sid);
	
	return ret;
}


