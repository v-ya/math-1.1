#include "main.h"

// sbuf 操作
// 新建 sbuf
var* sbuf_alloc(void)
{
	static char *label="sbuf_alloc";
	vlist *vl;
	var *vp;
	vp=ptvar_get(_pt_sbuf);
	if _oF(vp) return vp;
	ptvar_alloc(_pt_sbuf);
	vl=ptvar_vlist(_pt_sbuf);
	if _oF(!vl) return get_error(errid_MemLess,label);
	vp=var_alloc(tlog_byte,_lim_sbuf_base->v.v_long);
	if _oF(!vp) return get_error(errid_MemLess,label);
	vl->v=vp;
	return vp;
}
// 删除 sbuf
void sbuf_free(void)
{
	ptvar_free(_pt_sbuf);
}
// 获取 sbuf，如果没有则建立一个
var* sbuf_get(void)
{
	var *vp;
	vp=ptvar_get(_pt_sbuf);
	if _oF(!vp) vp=sbuf_alloc();
	return vp;
}
// 扩充 sbuf
var* sbuf_expand(void)
{
	static char *label="sbuf_expand";
	var *vp;
	u8 *s;
	u32 length;
	vp=sbuf_get();
	if _oF(vp->type&type_err) return vp;
	length=vp->length+_lim_sbuf_base->v.v_long;
	if _oF(length>_lim_sbuf_max->v.v_long) return get_error(errid_ReqOver,label);
	s=realloc(vp->v.vp_byte,length);
	if _oF(!s) return get_error(errid_MemLess,label);
	vp->length=length;
	vp->v.vp_byte=s;
	return vp;
}
// 缩减 sbuf
var* sbuf_reduce(void)
{
	static char *label="sbuf_reduce";
	var *vp;
	u8 *s;
	u32 length;
	vp=sbuf_get();
	if _oF(vp->type&type_err) return vp;
	length=_lim_sbuf_base->v.v_long;
	if _oF(length==vp->length) return vp;
	s=realloc(vp->v.vp_byte,length);
	if _oF(!s) return get_error(errid_MemLess,label);
	vp->length=length;
	vp->v.vp_byte=s;
	return vp;
}

var* sbuf_sprintf(char *format, vlist *vl)
{
	static char *label="sbuf_sprintf";
	static char *type_name_void="(void)",*type_name_vlist="(vlist)",*type_name_vmat="(vmat)",*type_name_null="(null)",*type_name_unknow="(unknow)";
	static char *type_array_name[tlog_max*2]={"(void[])","(byte[])","(word[])","(int[])","(long[])","(float[])","(string[])",
		"(vlist[])","(vmat[])",NULL,NULL,"(ubyte[])","(uword[])","(uint[])","(ulong[])"};
	var *sbuf,*vp;
	char fix[16],*buffer;
	u32 n,nm,mode;
	int i,m1,m2;
	void *addr;
	value v;
	n=0;
	sbuf=sbuf_get();
	if _oF(sbuf->type&type_err) return sbuf;
	buffer=sbuf->v.vp_byte;
	nm=sbuf->length;
	Loop:
	if _oF(*format==0) goto End;
	else if _oF(*format=='%')
	{
		format++;
		if _oF(*format=='%') goto Normal;
		i=0;
		mode=0;
		fix[i++]='%';
		// 跳过 Flags
		while(i<15&&(*format=='+'||*format==' '||*format=='-'||*format=='#'||*format=='0')) fix[i++]=*(format++);
		// 跳过 Field Width
		if _oF(i<15&&*format=='*')
		{
			fix[i++]=*(format++);
			mode++;
		}
		else while(i<15&&(*format>='0'&&*format<='9')) fix[i++]=*(format++);
		// 跳过 Precision
		if _oF(i<15&&*format=='.')
		{
			fix[i++]=*(format++);
			if _oF(i<15&&*format=='*')
			{
				fix[i++]=*(format++);
				mode++;
			}
			else while(i<15&&(*format>='0'&&*format<='9')) fix[i++]=*(format++);
		}
		// 忽略 Length，根据变量类型自动适配
		// [*][.*]
		if _oF(i>=15) goto Err;
		switch(mode)
		{
			case 1:
				vp=vl?vl->v:NULL;
				if _oF(!vp) goto Err;
				else vl=vl->r;
				if _oT(vp->type&type_znum) m1=vp->v.v_int;
				else if _oT(vp->type&type_float) m1=(u32)vp->v.v_float;
				else goto Err;
				break;
			case 2:
				vp=vl?vl->v:NULL;
				if _oF(!vp) goto Err;
				else vl=vl->r;
				if _oT(vp->type&type_znum) m1=vp->v.v_int;
				else if _oT(vp->type&type_float) m1=(u32)vp->v.v_float;
				else goto Err;
				vp=vl?vl->v:NULL;
				if _oF(!vp) goto Err;
				else vl=vl->r;
				if _oT(vp->type&type_znum) m2=vp->v.v_int;
				else if _oT(vp->type&type_float) m2=(u32)vp->v.v_float;
				else goto Err;
				break;
		}
		// Type
		vp=vl?vl->v:NULL;
		if _oF(!vp) goto Err;
		else vl=vl->r;
		if _oF(vp->length&&*format!='?') goto Err;
		switch(*format)
		{
			case 'd':
				_znum_entra:
				if _oF(vp->type&type_unsign) goto _unsign;
				if _oT(vp->type&type_znum) v.v_long=vp->v.v_long;
				else if _oT(vp->type&type_float) v.v_long=(s64)vp->v.v_float;
				else goto Err;
				_xo:
				if _oT(i<13)
				{
					fix[i++]='l';
					fix[i++]='l';
					fix[i++]=*format=='?'?'d':*format;
					fix[i]=0;
					goto _znum;
				}
				else goto Err;
			case 'u':
				_unsign:
				if _oT(vp->type&type_znum) v.v_long=vp->v.v_long;
				else if _oT(vp->type&type_float) v.v_long=(u64)vp->v.v_float;
				else goto Err;
				if _oT(i<13)
				{
					fix[i++]='l';
					fix[i++]='l';
					fix[i++]='u';
					fix[i]=0;
					goto _znum;
				}
				else goto Err;
			case 's':
				if _oT(vp->type&type_string) v.v_string=vp->v.v_string;
				else goto Err;
				if _oT(i<15)
				{
					fix[i++]='s';
					fix[i]=0;
					goto _string;
				}
				else goto Err;
			case 'c':
				if _oT(vp->type&type_znum) v.v_int=vp->v.v_int;
				else goto Err;
				if _oT(i<15)
				{
					fix[i++]='c';
					fix[i]=0;
					goto _char;
				}
				else goto Err;
			case 'x':
			case 'X':
			case 'o':
				if _oT(vp->type&type_znum) v.v_long=vp->v.v_long;
				else goto Err;
				goto _xo;
			case 'f':
			case 'F':
			case 'e':
			case 'E':
			case 'g':
			case 'G':
			case 'a':
			case 'A':
				if _oF(vp->type&type_znum)
				{
					if _oF(vp->type&type_unsign) v.v_float=(u64)vp->v.v_long;
					else v.v_float=(s64)vp->v.v_long;
				}
				else if _oT(vp->type&type_float) v.v_float=vp->v.v_float;
				else goto Err;
				if _oT(i<14)
				{
					fix[i++]='l';
					fix[i++]=*format;
					fix[i]=0;
					goto _float;
				}
				else goto Err;
			case '?':
				if _oF(vp->length) goto _auto_array;
				switch(vp->type&type_all)
				{
					case type_float:
						v.v_float=vp->v.v_float;
						if _oT(i<14)
						{
							fix[i++]='l';
							fix[i++]='g';
							fix[i]=0;
							goto _float;
						}
						else goto Err;
					case type_string:
						v.v_string=vp->v.v_string;
						if _oT(i<15)
						{
							fix[i++]='s';
							fix[i]=0;
							goto _string;
						}
						else goto Err;
					default:
						if _oT(vp->type&type_znum) goto _znum_entra;
						if _oT(i<15)
						{
							fix[i++]='s';
							fix[i]=0;
						}
						else goto Err;
						switch(vp->type&type_all)
						{
							case type_void:
								v.v_string=type_name_void;
								goto _string;
							case type_vlist:
								v.v_string=type_name_vlist;
								goto _string;
							case type_vmat:
								v.v_string=type_name_vmat;
								goto _string;
							case type_null:
								v.v_string=type_name_null;
								goto _string;
							default:
								v.v_string=type_name_unknow;
								goto _string;
						}
				}
				_auto_array:
				if _oT(i<15)
				{
					fix[i++]='s';
					fix[i]=0;
				}
				v.v_int=0;
				i=vp->type&type_all;
				while(i)
				{
					v.v_int++;
					i>>=1;
				}
				if _oF(v.v_int<=0) i=0;
				else i=v.v_int-1;
				if _oF((vp->type&type_unsign)&&(vp->type&type_znum)) i+=tlog_max;
				v.v_string=type_array_name[i];
				goto _string;
			default:
				goto Err;
		}
		// 长整型
		_znum:
		switch(mode)
		{
			case 0:
				i=snprintf(buffer+n,nm-n,fix,v.v_long);
				break;
			case 1:
				i=snprintf(buffer+n,nm-n,fix,m1,v.v_long);
				break;
			case 2:
				i=snprintf(buffer+n,nm-n,fix,m1,m2,v.v_long);
				break;
			default:
				goto Err;
		}
		if _oF(i<0||i>=(nm-n))
		{
			addr=&&_znum;
			goto _expand;
		}
		else goto _next;
		// 双精度浮点数
		_float:
		switch(mode)
		{
			case 0:
				i=snprintf(buffer+n,nm-n,fix,v.v_float);
				break;
			case 1:
				i=snprintf(buffer+n,nm-n,fix,m1,v.v_float);
				break;
			case 2:
				i=snprintf(buffer+n,nm-n,fix,m1,m2,v.v_float);
				break;
			default:
				goto Err;
		}
		if _oF(i<0||i>(nm-n))
		{
			addr=&&_float;
			goto _expand;
		}
		else goto _next;
		// 字符串
		_string:
		switch(mode)
		{
			case 0:
				i=snprintf(buffer+n,nm-n,fix,v.v_string);
				break;
			case 1:
				i=snprintf(buffer+n,nm-n,fix,m1,v.v_string);
				break;
			case 2:
				i=snprintf(buffer+n,nm-n,fix,m1,m2,v.v_string);
				break;
			default:
				goto Err;
		}
		if _oF(i<0||i>(nm-n))
		{
			addr=&&_string;
			goto _expand;
		}
		else goto _next;
		// 字符
		_char:
		switch(mode)
		{
			case 0:
				i=snprintf(buffer+n,nm-n,fix,v.v_int);
				break;
			case 1:
				i=snprintf(buffer+n,nm-n,fix,m1,v.v_int);
				break;
			case 2:
				i=snprintf(buffer+n,nm-n,fix,m1,m2,v.v_int);
				break;
			default:
				goto Err;
		}
		if _oF(i<0||i>(nm-n))
		{
			addr=&&_char;
			goto _expand;
		}
		else goto _next;
		// 结束处理
		_next:
		format++;
		while(buffer[n]) n++;
		goto Loop;
	}
	Normal:
	if _oT(n<nm) buffer[n++]=*(format++);
	else
	{
		addr=&&_expand_end;
		// 缓冲区扩展
		_expand:
		sbuf=sbuf_expand();
		if _oF(sbuf->type&type_err) return sbuf;
		buffer=sbuf->v.vp_byte;
		nm=sbuf->length;
		goto *addr;
		_expand_end:
		buffer[n++]=*(format++);
	}
	goto Loop;
	End:
	if _oT(n<nm) buffer[n]=0;
	else
	{
		// :( ...
		addr=&&End;
		goto _expand;
	}
	return sbuf;
	Err:
	vp=get_error(errid_GraSprintfFormat,label);
	return vp;
}

char* get_name(char *exp, char **pexp)
{
	var *pt_sbuf;
	char *name;
	u32 size;
	name=exp;
	pt_sbuf=sbuf_get();
	if _oF(!pt_sbuf) return NULL;
	if _oF(!is_Name(*exp)) return NULL;
	for(size=1;is_name(*exp);exp++) size++;
	if _oT(pexp) *pexp=exp;
	if _oF(size>pt_sbuf->length)
	{
		while(size>pt_sbuf->length)
		{
			pt_sbuf=sbuf_expand();
			if _oF(pt_sbuf->type&type_err) return NULL;
		}
	}
	size--;
	memcpy(pt_sbuf->v.vp_byte,name,size);
	pt_sbuf->v.vp_byte[size]=0;
	return pt_sbuf->v.vp_byte;
}



