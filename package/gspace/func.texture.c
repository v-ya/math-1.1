
func(createTexture)
{
	static char *label=label_name("createTexture");
	static u32 type_1[1]={type_znum};
	u32 type;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	type = argv->v->v.v_long;
	
	ret->type = type_long|type_unsign;
	ret->v.v_long = createTexture(type);
	
	return ret;
}

func(deleteTexture)
{
	static char *label=label_name("deleteTexture");
	static u32 type_1[1]={type_znum};
	u64 sid;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	deleteUserTexture(sid);
	
	ret->type = type_void;
	
	return ret;
}

func(textureStorage)
{
	static char *label=label_name("textureStorage");
	static u32 type_1[1]={type_znum};
	static u32 type_2[2]={type_znum, type_znum};
	static u32 type_3[3]={type_znum, type_znum, type_znum};
	static u32 type_4[4]={type_znum, type_znum, type_znum, type_znum};
	static u32 type_5[5]={type_znum, type_znum, type_znum, type_znum, type_znum};
	static u32 type_6[6]={type_znum, type_znum, type_znum, type_znum, type_znum, type_znum};
	
	u64 sid, buffer;
	GLuint texture, lsb;
	u32 type;
	GLenum format;
	GLsizei w, h, d;
	GLboolean fixed;
	
	if _oF(argc<1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	argc--;
	
	texture = getHandle(V_texture, sid, srcTextureTypeAll, &type);
	type = F_type_g(type);
	
	ret->type = type_long;
	switch(type)
	{
		case srcTextureType1D:
			if _oF(argc!=3||base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
			lsb = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			w = argv->v->v.v_long;
			h = d = fixed = 0;
			break;
		case srcTextureType2D:
		case srcTextureType1DArray:
		case srcTextureTypeCube:
		case srcTextureTypeRect:
			if _oF(argc!=4||base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
			lsb = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			w = argv->v->v.v_long;
			argv = argv->r;
			h = argv->v->v.v_long;
			d = fixed = 0;
			break;
		case srcTextureType3D:
		case srcTextureType2DArray:
		case srcTextureTypeCubeArray:
			if _oF(argc!=5||base->check_varlist(argv,5,type_5)) return base->get_error(errid_FunArgvType,label);
			lsb = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			w = argv->v->v.v_long;
			argv = argv->r;
			h = argv->v->v.v_long;
			argv = argv->r;
			d = argv->v->v.v_long;
			fixed = 0;
			break;
		case srcTextureType2DMS:
			if _oF(argc!=5||base->check_varlist(argv,5,type_5)) return base->get_error(errid_FunArgvType,label);
			lsb = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			w = argv->v->v.v_long;
			argv = argv->r;
			h = argv->v->v.v_long;
			argv = argv->r;
			fixed = argv->v->v.v_long;
			d = 0;
			break;
		case srcTextureType2DMSArray:
			if _oF(argc!=6||base->check_varlist(argv,6,type_6)) return base->get_error(errid_FunArgvType,label);
			lsb = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			w = argv->v->v.v_long;
			argv = argv->r;
			h = argv->v->v.v_long;
			argv = argv->r;
			d = argv->v->v.v_long;
			argv = argv->r;
			fixed = argv->v->v.v_long;
			break;
		case srcTextureTypeBuffer:
			if _oF(argc!=2||base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
			format = argv->v->v.v_long;
			argv = argv->r;
			buffer = argv->v->v.v_long;
			lsb = getHandle(V_buffer, buffer, F_isok|F_type_t(srcBufferTypeTextureBuffer), NULL);
			w = h = d = fixed = 0;
			break;
		default:
			ret->v.v_long = -1;
			return ret;
	}
	ret->v.v_long = textureStorage(texture, type, lsb, format, w, h, d, fixed);
	
	if _oT(ret->v.v_long == 0) setHandleOk(V_texture, sid);
	
	return ret;
}

func(textureSubImage)
{
	static char *label=label_name("textureSubImage");
	static u32 type_1[1]={type_znum};
	
	static u32 type_5[5]={type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum};
	static u32 type_6[6]={type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum};
	static u32 type_7[7]={type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum, type_znum};
	static u32 type_8[8]={type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum, type_znum, type_znum};
	static u32 type_9[9]={type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum, type_znum, type_znum, type_znum};
	static u32 type_10[10]={type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum, type_znum, type_znum, type_znum, type_znum};
	static u32 type_11[11]={type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum, type_znum, type_znum, type_znum, type_znum, type_znum};
	static u32 type_c7[7]={type_znum, type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum};
	static u32 type_c9[9]={type_znum, type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum, type_znum, type_znum};
	static u32 type_c10[10]={type_znum, type_znum, type_znum, type_znum, type_znum|type_onlyarray, type_znum, type_znum, type_znum, type_znum, type_znum};
	
	u64 sid, length;
	var *vp;
	void *pixels;
	GLuint texture;
	GLint level, x, y, z;
	u32 type, cube, begin;
	GLenum format, ft;
	GLsizei w, h, d;
	
	if _oF(argc<1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	sid = argv->v->v.v_long;
	argv = argv->r;
	argc--;
	
	texture = getHandle(V_texture, sid, F_isok, &type);
	type = F_type_g(type);
	
	x = y = z = w = h = d = cube = begin = 0;
	ret->type = type_long;
	switch(type)
	{
		case srcTextureType1D:
			switch(argc)
			{
				case 5:
					if _oF(base->check_varlist(argv,5,type_5)) return base->get_error(errid_FunArgvType,label);
					break;
				case 6:
					if _oF(base->check_varlist(argv,6,type_6)) return base->get_error(errid_FunArgvType,label);
					break;
				case 7:
					if _oF(base->check_varlist(argv,7,type_7)) return base->get_error(errid_FunArgvType,label);
					break;
				default:
					return base->get_error(errid_FunArgvType,label);
			}
			// level, format, ft, var pixels, [[begin, ]x, ]w
			level = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			ft = argv->v->v.v_long;
			argv = argv->r;
			vp = argv->v;
			switch(argc)
			{
				case 7:
					argv = argv->r;
					begin = argv->v->v.v_long;
				case 6:
					argv = argv->r;
					x = argv->v->v.v_long;
			}
			argv = argv->r;
			w = argv->v->v.v_long;
			break;
		case srcTextureType2D:
		case srcTextureType1DArray:
		case srcTextureTypeRect:
			switch(argc)
			{
				case 6:
					if _oF(base->check_varlist(argv,6,type_6)) return base->get_error(errid_FunArgvType,label);
					break;
				case 8:
					if _oF(base->check_varlist(argv,8,type_8)) return base->get_error(errid_FunArgvType,label);
					break;
				case 9:
					if _oF(base->check_varlist(argv,9,type_9)) return base->get_error(errid_FunArgvType,label);
					break;
				default:
					return base->get_error(errid_FunArgvType,label);
			}
			// level, format, ft, var pixels, [[begin, ]x, y, ]w, h
			level = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			ft = argv->v->v.v_long;
			argv = argv->r;
			vp = argv->v;
			switch(argc)
			{
				case 9:
					argv = argv->r;
					begin = argv->v->v.v_long;
				case 8:
					argv = argv->r;
					x = argv->v->v.v_long;
					argv = argv->r;
					y = argv->v->v.v_long;
			}
			argv = argv->r;
			w = argv->v->v.v_long;
			argv = argv->r;
			h = argv->v->v.v_long;
			break;
		case srcTextureType3D:
		case srcTextureType2DArray:
		case srcTextureTypeCubeArray:
			switch(argc)
			{
				case 7:
					if _oF(base->check_varlist(argv,7,type_7)) return base->get_error(errid_FunArgvType,label);
					break;
				case 10:
					if _oF(base->check_varlist(argv,10,type_10)) return base->get_error(errid_FunArgvType,label);
					break;
				case 11:
					if _oF(base->check_varlist(argv,11,type_11)) return base->get_error(errid_FunArgvType,label);
					break;
				default:
					return base->get_error(errid_FunArgvType,label);
			}
			// level, format, ft, var pixels, [[begin, ]x, y, z, ]w, h, d
			level = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			ft = argv->v->v.v_long;
			argv = argv->r;
			vp = argv->v;
			switch(argc)
			{
				case 11:
					argv = argv->r;
					begin = argv->v->v.v_long;
				case 10:
					argv = argv->r;
					x = argv->v->v.v_long;
					argv = argv->r;
					y = argv->v->v.v_long;
					argv = argv->r;
					z = argv->v->v.v_long;
			}
			argv = argv->r;
			w = argv->v->v.v_long;
			argv = argv->r;
			h = argv->v->v.v_long;
			argv = argv->r;
			d = argv->v->v.v_long;
			break;
		case srcTextureTypeCube:
			switch(argc)
			{
				case 7:
					if _oF(base->check_varlist(argv,7,type_c7)) return base->get_error(errid_FunArgvType,label);
					break;
				case 9:
					if _oF(base->check_varlist(argv,9,type_c9)) return base->get_error(errid_FunArgvType,label);
					break;
				case 10:
					if _oF(base->check_varlist(argv,10,type_c10)) return base->get_error(errid_FunArgvType,label);
					break;
				default:
					return base->get_error(errid_FunArgvType,label);
			}
			// level, cube, format, ft, var pixels, [[begin, ]x, y, ]w, h
			level = argv->v->v.v_long;
			argv = argv->r;
			cube = argv->v->v.v_long;
			argv = argv->r;
			format = argv->v->v.v_long;
			argv = argv->r;
			ft = argv->v->v.v_long;
			argv = argv->r;
			vp = argv->v;
			switch(argc)
			{
				case 9:
					argv = argv->r;
					begin = argv->v->v.v_long;
				case 8:
					argv = argv->r;
					x = argv->v->v.v_long;
					argv = argv->r;
					y = argv->v->v.v_long;
			}
			argv = argv->r;
			w = argv->v->v.v_long;
			argv = argv->r;
			h = argv->v->v.v_long;
			break;
		case srcTextureType2DMS:
		case srcTextureType2DMSArray:
		case srcTextureTypeBuffer:
			ret->v.v_long = 3;
			return ret;
		default:
			ret->v.v_long = -1;
			return ret;
	}
	
	if _oT(vp->length > begin) switch(vp->type&type_all)
	{
		case type_byte:
			pixels = (void *)(vp->v.vp_byte + begin);
			length = ((u64) (vp->length - begin)) * sizeof(u8);
			break;
		case type_word:
			pixels = (void *)(vp->v.vp_word + begin);
			length = ((u64) (vp->length - begin)) * sizeof(u16);
			break;
		case type_int:
			pixels = (void *)(vp->v.vp_int + begin);
			length = ((u64) (vp->length - begin)) * sizeof(u32);
			break;
		case type_long:
			pixels = (void *)(vp->v.vp_long + begin);
			length = ((u64) (vp->length - begin)) * sizeof(u64);
			break;
		default:
			ret->v.v_long = -1;
			return ret;
	}
	else pixels = NULL;
	
	ret->v.v_long = textureSubImage(texture, type, level, cube, format, ft, pixels, length, x, y, z, w, h, d);
	
	return ret;
}

func(useTexture)
{
	static char *label=label_name("useTexture");
	static u32 type_2[2]={type_znum, type_znum};
	u64 sid;
	u32 active;
	
	if _oF(argc!=2||base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	
	active = argv->v->v.v_long;
	argv = argv->r;
	sid = argv->v->v.v_long;
	
	ret->type = type_long;
	ret->v.v_long = useTexture(sid, srcTextureTypeAll, active);
	
	return ret;
}

