
func(createBuffer)
{
	static char *label=label_name("createBuffer");
	static u32 type_1[1]={type_znum};
	u32 type;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	type = argv->v->v.v_long;
	
	ret->type = type_long|type_unsign;
	ret->v.v_long = createBuffer(type);
	
	return ret;
}

func(deleteBuffer)
{
	static char *label=label_name("deleteBuffer");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	deleteUserBuffer(sid);
	
	ret->type = type_void;
	
	return ret;
}

func(bufferSetData)
{
	static char *label=label_name("bufferSetData");
	static u32 type_3[3]={type_znum, type_num|type_allowarray, type_znum};
	u64 sid;
	var *vp;
	GLuint buffer;
	u32 type;
	GLenum usage;
	
	if _oF(argc!=3||base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	vp = argv->v;
	argv = argv->r;
	usage = argv->v->v.v_long;
	
	buffer = getHandle(V_buffer, sid, F_isok, &type);
	type = F_type_g(type);
	
	ret->type = type_long;
	if _oT(vp->length)
	{
		switch(vp->type&type_all)
		{
			case type_byte:
				sid = 1ul * vp->length;
				ret->v.v_long = bufferSetData(buffer, type, sid, vp->v.v_void, usage);
				break;
			case type_word:
				sid = 2ul * vp->length;
				ret->v.v_long = bufferSetData(buffer, type, sid, vp->v.v_void, usage);
				break;
			case type_int:
				sid = 4ul * vp->length;
				ret->v.v_long = bufferSetData(buffer, type, sid, vp->v.v_void, usage);
				break;
			case type_long:
			case type_float:
				sid = 8ul * vp->length;
				ret->v.v_long = bufferSetData(buffer, type, sid, vp->v.v_void, usage);
				break;
			default:
				ret->v.v_long = -2;
		}
	}
	else if _oF(vp->type&type_float) return base->get_error(errid_FunArgvType,label);
	else ret->v.v_long = bufferSetData(buffer, type, vp->v.v_long, NULL, usage);
	
	return ret;
}

func(bufferSubData)
{
	static char *label=label_name("bufferSubData");
	static u32 type_2[2]={type_znum, type_num|type_onlyarray};
	static u32 type_3[3]={type_znum, type_znum, type_num|type_onlyarray};
	static u32 type_4[4]={type_znum, type_znum, type_num|type_onlyarray, type_znum};
	static u32 type_5[5]={type_znum, type_znum, type_num|type_onlyarray, type_znum, type_znum};
	u64 sid, bs;
	var *vp;
	GLuint buffer;
	u32 type, offset, size, begin;
	char *v;
	
	switch(argc)
	{
		case 2:
			if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			vp = argv->v;
			
			offset = 0;
			size = 0;
			begin = 0;
			break;
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			offset = argv->v->v.v_long;
			argv = argv->r;
			vp = argv->v;
			
			size = 0;
			begin = 0;
			break;
		case 4:
			if _oF(base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			offset = argv->v->v.v_long;
			argv = argv->r;
			vp = argv->v;
			argv = argv->r;
			size = argv->v->v.v_long;
			
			begin = 0;
			break;
		case 5:
			if _oF(base->check_varlist(argv,5,type_5)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			offset = argv->v->v.v_long;
			argv = argv->r;
			vp = argv->v;
			argv = argv->r;
			begin = argv->v->v.v_long;
			argv = argv->r;
			size = argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
	}
	
	buffer = getHandle(V_buffer, sid, F_isok, &type);
	type = F_type_g(type);
	
	if _oF(begin>=vp->length)
	{
		begin = vp->length;
		size = 0;
	}
	else if _oF((u64)begin+size > vp->length) size = vp->length - begin;
	v = vp->v.vp_byte;
	
	ret->type = type_long;
	switch(vp->type&type_all)
	{
		case type_byte:
			bs = 1;
			break;
		case type_word:
			bs = 2;
			break;
		case type_int:
			bs = 4;
			break;
		case type_long:
		case type_float:
			bs = 8;
			break;
		default:
			ret->v.v_long = -2;
			return ret;
	}
	
	ret->v.v_long = size?bufferSubData(buffer, type, offset, size*bs, v + (begin*bs)):0;
	
	return ret;
}

func(useBuffer)
{
	static char *label=label_name("useBuffer");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = useBuffer(sid, srcBufferTypeAll);
	
	return ret;
}

