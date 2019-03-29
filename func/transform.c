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

