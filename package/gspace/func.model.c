
func(createModel)
{
	static char *label=label_name("createModel");
	static u32 type_1[1]={type_znum};
	u64 program;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	program = argv->v->v.v_long;
	
	ret->type = type_long|type_unsign;
	ret->v.v_long = createModel(program);
	
	return ret;
}

func(deleteModel)
{
	static char *label=label_name("deleteModel");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	deleteUserModel(sid);
	
	ret->type = type_void;
	
	return ret;
}

func(finalModel)
{
	static char *label=label_name("finalModel");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	finalModel(sid);
	
	ret->type = type_void;
	
	return ret;
}

func(mcRunScript)
{
	static char *label=label_name("mcRunScript");
	static u32 type_2[2]={type_znum, type_string};
	u64 sid;
	char *script;
	
	if _oF(argc!=2||base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	script = argv->v->v.v_string;
	
	ret->type = type_long;
	ret->v.v_long = modelRunScript(getObject(V_model, sid, 0, NULL), script);
	
	return ret;
}

func(mcSyncUniform)
{
	static char *label=label_name("mcSyncUniform");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = modelSyncUniform(getObject(V_model, sid, 0, NULL));
	
	return ret;
}

func(mcBindBuffer)
{
	static char *label=label_name("mcBindBuffer");
	static u32 type_2[2]={type_znum, type_znum};
	u64 sid, buffer;
	
	if _oF(argc!=2||base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	buffer = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = modelBindBuffer(getObject(V_model, sid, 0, NULL), buffer);
	
	return ret;
}

func(mcBindVertexAttrib)
{
	static char *label=label_name("mcBindVertexAttrib");
	static u32 type_2[2]={type_znum, type_znum};
	u64 sid, va;
	
	if _oF(argc!=2||base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	va = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = modelBindVertexAttrib(getObject(V_model, sid, 0, NULL), va);
	
	return ret;
}

func(mcDrawArrays)
{
	static char *label=label_name("mcDrawArrays");
	static u32 type_3[3]={type_znum, type_znum, type_znum};
	static u32 type_4[4]={type_znum, type_znum, type_znum, type_znum};
	u64 sid;
	u32 mode, begin, count;
	
	switch(argc)
	{
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			begin = 0;
			break;
		case 4:
			if _oF(base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			begin = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
	}
	
	ret->type = type_long;
	ret->v.v_long = modelDrawArrays(getObject(V_model, sid, 0, NULL), mode, begin, count);
	
	return ret;
}

func(mcDrawElements)
{
	static char *label=label_name("mcDrawElements");
	static u32 type_3[3]={type_znum, type_znum, type_znum};
	static u32 type_4[4]={type_znum, type_znum, type_znum, type_znum};
	static u32 type_5[5]={type_znum, type_znum, type_znum, type_znum, type_znum};
	u64 sid;
	u32 mode, begin, count, pbase;
	
	switch(argc)
	{
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			begin = 0;
			pbase = 0;
			break;
		case 4:
			if _oF(base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			begin = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			pbase = 0;
			break;
		case 5:
			if _oF(base->check_varlist(argv,5,type_5)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			begin = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			argv = argv->r;
			pbase = argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
	}
	
	ret->type = type_long;
	ret->v.v_long = modelDrawElements(getObject(V_model, sid, 0, NULL), mode, begin, count, pbase);
	
	return ret;
}



func(modelDraw)
{
	static char *label=label_name("modelDraw");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = modelDraw(getObject(V_model, sid, F_isok, NULL));
	
	return ret;
}

