#include "../main.h"

keyword(strarray)
{
	static char *label=".strarray";
	var *vp;
	u32 size;
	vp=cal(*expp,expp);
	if _oF(vp->type&type_spe) return vp;
	if _oF(**expp!=';')
	{
		var_free(vp);
		return get_error(errid_GraLackSem,label);
	}
	if _oT(isstring(vp))
	{
		if _oT(vp->mode&auth_retype)
		{
			if _oT(vp->v.v_string) size=strlen(vp->v.v_string)+1;
			else size=0;
			vp->type=type_byte|type_unsign;
			vp->length=size;
			return NULL;
		}
		else
		{
			var_free(vp);
			return get_error(errid_VarNotRetype,label);
		}
	}
	else
	{
		var_free(vp);
		return get_error(errid_VarNotString,label);
	}
}

keyword(arrstring)
{
	static char *label=".arrstring";
	var *vp;
	vp=cal(*expp,expp);
	if _oF(vp->type&type_spe) return vp;
	if _oF(**expp!=';')
	{
		var_free(vp);
		return get_error(errid_GraLackSem,label);
	}
	if _oF(!vp->length || !(vp->type&type_byte))
	{
		var_free(vp);
		return get_error(errid_VarNotBytesArray,label);
	}
	if _oT(vp->mode&auth_retype)
	{
		vp->v.vp_byte[vp->length-1]=0;
		vp->length=leng_no;
		vp->type=type_string;
		return NULL;
	}
	else
	{
		var_free(vp);
		return get_error(errid_VarNotRetype,label);
	}
}

keyword(transform)
{
	static char *label=".transform";
	char buffer[16], *exp;
	var *vp;
	u32 tlog, i, length;
	u8 *pu8;
	u16 *pu16;
	u32 *pu32;
	u64 *pu64;
	float *pf32;
	double *pf64;
	
	// get to-type
	exp = *expp;
	i=0;
	while(i<15 && is_name(*exp))
	{
		buffer[i++]=*(exp++);
	}
	buffer[i]=0;
	*expp = exp;
	tlog=get_tlog(buffer);
	if _oF(tlog>=tlog_max) return get_error(errid_VarUnknowType,label);
	
	// get this var
	while(*exp && is_space(*exp)) exp++;
	if _oT(*exp == ',') exp++;
	while(*exp && is_space(*exp)) exp++;
	*expp = exp;
	vp=cal(*expp, expp);
	if _oF(vp->type&type_spe) return vp;
	if _oF(**expp!=';')
	{
		var_free(vp);
		return get_error(errid_GraLackSem,label);
	}
	
	// check var
	if _oF(!vp->length)
	{
		var_free(vp);
		return get_error(errid_VarNotBytesArray,label);
	}
	if _oF(!(vp->mode&auth_retype))
	{
		var_free(vp);
		return get_error(errid_VarNotRetype,label);
	}
	length = vp->length;
	
	#define TranArray(_s, _d)	p##_s = (_s *) vp->v.v_void;\
					p##_d = malloc(sizeof(_d)*length);\
					if _oF(!p##_d) goto Err_mem;\
					for(i=0;i<length;i++) p##_d[i] = p##_s[i];\
					vp->v.v_void = p##_d;\
					if _oT(vp->mode&free_pointer) free(p##_s)
	#define f32	float
	#define f64	double
	
	switch(vp->type & type_all)
	{
		case type_byte:
			switch(tlog)
			{
				case tlog_byte:
					goto End;
				case tlog_word:
					TranArray(u8, u16);
					goto End;
				case tlog_int:
					TranArray(u8, u32);
					goto End;
				case tlog_long:
					TranArray(u8, u64);
					goto End;
				default:
					goto Err_type;
			}
			break;
		case type_word:
			switch(tlog)
			{
				case tlog_byte:
					TranArray(u16, u8);
					goto End;
				case tlog_word:
					goto End;
				case tlog_int:
					TranArray(u16, u32);
					goto End;
				case tlog_long:
					TranArray(u16, u64);
					goto End;
				default:
					goto Err_type;
			}
			break;
		case type_int:
			switch(tlog)
			{
				case tlog_byte:
					TranArray(u32, u8);
					goto End;
				case tlog_word:
					TranArray(u32, u16);
					goto End;
				case tlog_int:
					goto End;
				case tlog_long:
					TranArray(u32, u64);
					goto End;
				case tlog_float:
					TranArray(f32, f64);
					goto End;
				default:
					goto Err_type;
			}
			break;
		case type_long:
			switch(tlog)
			{
				case tlog_byte:
					TranArray(u64, u8);
					goto End;
				case tlog_word:
					TranArray(u64, u16);
					goto End;
				case tlog_int:
					TranArray(u64, u32);
					goto End;
				case tlog_long:
					goto End;
				case tlog_float:
					goto End;
				default:
					goto Err_type;
			}
			break;
		case type_float:
			switch(tlog)
			{
				case tlog_int:
					TranArray(f64, f32);
					goto End;
				case tlog_long:
					goto End;
				case tlog_float:
					goto End;
				default:
					goto Err_type;
			}
			break;
		default:
			Err_type:
			var_free(vp);
			return get_error(errid_GraVarType,label);
	}
	
	#undef TranArray
	#undef f32
	#undef f64
	
	End:
	if _oT(buffer[0] == 'u') vp->type = type_tlog(tlog)|type_unsign;
	else vp->type = type_tlog(tlog);
	vp->mode |= free_pointer;
	var_free(vp);
	return NULL;
	
	Err_mem:
	var_free(vp);
	return get_error(errid_MemLess,label);
}

func(array_copy)
{
	static char *label=".array_copy";
	static u32 type_2[2]={type_num|type_onlyarray, type_num|type_onlyarray};
	static u32 type_5[5]={type_num|type_onlyarray, type_num|type_onlyarray, type_znum, type_znum, type_znum};
	var *vs, *vd;
	u32 sb, db, i, n, iu;
	u8 *pu8, *s, *d;
	u16 *pu16;
	u32 *pu32;
	u64 *pu64;
	s8 *ps8;
	s16 *ps16;
	s32 *ps32;
	s64 *ps64;
	double *pf64;
	
	switch(argc)
	{
		case 2:
			if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
			vd = argv->v;
			argv = argv->r;
			vs = argv->v;
			sb = db = 0;
			n = -1;
			break;
		case 5:
			if _oF(check_varlist(argv,5,type_5)) return get_error(errid_FunArgvType,label);
			vd = argv->v;
			argv = argv->r;
			vs = argv->v;
			argv = argv->r;
			db = argv->v->v.v_long;
			argv = argv->r;
			sb = argv->v->v.v_long;
			argv = argv->r;
			n = argv->v->v.v_long;
			break;
		default:
			return get_error(errid_FunArgvType,label);
	}
	
	ret->type = type_void;
	if _oF(db >= vd->length || sb >= vs->length) return ret;
	if _oF(((u64)db+n) > vd->length) n = vd->length - db;
	if _oF(((u64)sb+n) > vs->length) n = vs->length - sb;
	
	iu = ((vs->type&type_unsign)?1:0) | ((vd->type&type_unsign)?2:0);
	
	#define f64	double
	#define NoTranCopy(_t)	\
		s = vs->v.vp_byte + (sb*sizeof(_t));\
		d = vd->v.vp_byte + (db*sizeof(_t));\
		memcpy(d,s,n*sizeof(_t));\
		break
	#define TranCopy4(_dt, _dn, _st, _sn)	\
		p##_st##_sn = (_st##_sn *) vs->v.v_void;\
		p##_st##_sn += sb;\
		p##_dt##_dn = (_dt##_dn *) vd->v.v_void;\
		p##_dt##_dn += db;\
		for(i=0;i<n;i++)\
			p##_dt##_dn[i] = p##_st##_sn[i];\
		break
	#define TranCopy2(_d_n, _s_n)	\
		switch(iu)\
		{\
			case 0: TranCopy4(s,_d_n,s,_s_n);\
			case 1: TranCopy4(s,_d_n,u,_s_n);\
			case 2: TranCopy4(u,_d_n,s,_s_n);\
			case 3: TranCopy4(u,_d_n,u,_s_n);\
		}\
		break
	
	switch(vd->type & type_all)
	{
		case type_byte:
			switch(vs->type &type_all)
			{
				case type_byte: NoTranCopy(u8);
				case type_word: TranCopy2(8,16);
				case type_int: TranCopy2(8,32);
				case type_long: TranCopy2(8,64);
				case type_float:
					switch(iu)
					{
						case 0:
						case 1: TranCopy4(s,8,f,64);
						case 2:
						case 3: TranCopy4(u,8,f,64);
					}
					break;
			}
			break;
		case type_word:
			switch(vs->type &type_all)
			{
				case type_byte: TranCopy2(16,8);
				case type_word: NoTranCopy(u16);
				case type_int: TranCopy2(16,32);
				case type_long: TranCopy2(16,64);
				case type_float:
					switch(iu)
					{
						case 0:
						case 1: TranCopy4(s,16,f,64);
						case 2:
						case 3: TranCopy4(u,16,f,64);
					}
					break;
			}
			break;
		case type_int:
			switch(vs->type &type_all)
			{
				case type_byte: TranCopy2(32,8);
				case type_word: TranCopy2(32,16);
				case type_int: NoTranCopy(u32);
				case type_long: TranCopy2(32,64);
				case type_float:
					switch(iu)
					{
						case 0:
						case 1: TranCopy4(s,32,f,64);
						case 2:
						case 3: TranCopy4(u,32,f,64);
					}
					break;
			}
			break;
		case type_long:
			switch(vs->type &type_all)
			{
				case type_byte: TranCopy2(64,8);
				case type_word: TranCopy2(64,16);
				case type_int: TranCopy2(64,32);
				case type_long: NoTranCopy(u64);
				case type_float:
					switch(iu)
					{
						case 0:
						case 1: TranCopy4(s,64,f,64);
						case 2:
						case 3: TranCopy4(u,64,f,64);
					}
					break;
			}
			break;
		case type_float:
			switch(vs->type &type_all)
			{
				case type_byte:
					switch(iu)
					{
						case 0:
						case 2: TranCopy4(f,64,s,8);
						case 1:
						case 3: TranCopy4(f,64,u,8);
					}
					break;
				case type_word:
					switch(iu)
					{
						case 0:
						case 2: TranCopy4(f,64,s,16);
						case 1:
						case 3: TranCopy4(f,64,u,16);
					}
					break;
				case type_int:
					switch(iu)
					{
						case 0:
						case 2: TranCopy4(f,64,s,32);
						case 1:
						case 3: TranCopy4(f,64,u,32);
					}
					break;
				case type_long:
					switch(iu)
					{
						case 0:
						case 2: TranCopy4(f,64,s,64);
						case 1:
						case 3: TranCopy4(f,64,u,64);
					}
					break;
				case type_float: NoTranCopy(u64);
			}
			break;
	}
	
	#undef f64
	#undef NoTranCopy
	#undef TranCopy4
	#undef TranCopy2
	
	return ret;
}

func(array_store)
{
	static char *label=".array_store";
	static u32 type_2[2]={type_num|type_onlyarray, type_num|type_onlyarray};
	static u32 type_5[5]={type_num|type_onlyarray, type_num|type_onlyarray, type_znum, type_znum, type_znum};
	var *vs, *vd;
	u32 sb, db, i, n;
	u8 *pu8, *s, *d;
	u16 *pu16;
	u32 *pu32;
	u64 *pu64;
	float *pf32;
	double *pf64;
	
	switch(argc)
	{
		case 2:
			if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
			vd = argv->v;
			argv = argv->r;
			vs = argv->v;
			sb = db = 0;
			n = -1;
			break;
		case 5:
			if _oF(check_varlist(argv,5,type_5)) return get_error(errid_FunArgvType,label);
			vd = argv->v;
			argv = argv->r;
			vs = argv->v;
			argv = argv->r;
			db = argv->v->v.v_long;
			argv = argv->r;
			sb = argv->v->v.v_long;
			argv = argv->r;
			n = argv->v->v.v_long;
			break;
		default:
			return get_error(errid_FunArgvType,label);
	}
	
	ret->type = type_void;
	if _oF(db >= vd->length || sb >= vs->length) return ret;
	if _oF(((u64)db+n) > vd->length) n = vd->length - db;
	if _oF(((u64)sb+n) > vs->length) n = vs->length - sb;
	
	#define NoTranStore(_t)	\
		s = vs->v.vp_byte + (sb*sizeof(_t));\
		d = vd->v.vp_byte + (db*sizeof(_t));\
		memcpy(d,s,n*sizeof(_t));\
		break
	#define TranStore(_d, _s)	\
		p##_s = (_s *) vs->v.v_void;\
		p##_d = (_d *) vd->v.v_void;\
		p##_s += sb;\
		p##_d += db;\
		for(i=0;i<n;i++)\
			p##_d[i] = p##_s[i];\
		break
	#define f32	float
	#define f64	double
	
	switch(vd->type & type_all)
	{
		case type_byte:
			switch(vs->type & type_all)
			{
				case type_byte: NoTranStore(u8);
				case type_word: TranStore(u8, u16);
				case type_int: TranStore(u8, u32);
				case type_long: TranStore(u8, u64);
				default: return get_error(errid_FunArgvType,label);
			}
			break;
		case type_word:
			switch(vs->type & type_all)
			{
				case type_byte: TranStore(u16, u8);
				case type_word: NoTranStore(u16);
				case type_int: TranStore(u16, u32);
				case type_long: TranStore(u16, u64);
				default: return get_error(errid_FunArgvType,label);
			}
			break;
		case type_int:
			switch(vs->type & type_all)
			{
				case type_byte: TranStore(u32, u8);
				case type_word: TranStore(u32, u16);
				case type_int: NoTranStore(u32);
				case type_long: TranStore(u32, u64);
				case type_float: TranStore(f32, f64);
				default: return get_error(errid_FunArgvType,label);
			}
			break;
		case type_long:
			switch(vs->type & type_all)
			{
				case type_byte: TranStore(u64, u8);
				case type_word: TranStore(u64, u16);
				case type_int: TranStore(u64, u32);
				case type_long:
				case type_float: NoTranStore(u64);
				default: return get_error(errid_FunArgvType,label);
			}
			break;
		case type_float:
			switch(vs->type & type_all)
			{
				case type_int: TranStore(f64, f32);
				case type_long:
				case type_float: NoTranStore(u64);
				default: return get_error(errid_FunArgvType,label);
			}
			break;
		default:
			return get_error(errid_GraVarType,label);
	}
	
	#undef NoTranStore
	#undef TranStore
	#undef f32
	#undef f64
	
	return ret;
}

