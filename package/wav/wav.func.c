#include "wav.h"
#include "wav.func.h"
#include "wav.code.h"
#include "wav.string.h"

#define	label_name(n) "[vya.wav]." n
extern var *type;
static char *_wav_="_wav_";

vya_wav* get_wav(var *obj)
{
	var *vp;
	vp=base->var_find(obj,_wav_);
	if _oF(!vp) return NULL;
	return (vya_wav*) vp->v.v_void;
}

func(new)
{
	static char *label=label_name("new");
	static u32 type_2[2]={type_object,type_num|type_string};
	var *obj,*vp;
	value v={0};
	double time;
	char *path;
	vya_wav *wav;
	extern var *_lim_maxtime;
	
	ret->type=type_void;
	ret->v.v_long=0;
	if _oF((argc!=2)||base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	obj=argv->v;
	vp=base->type_empty(obj);
	if _oF(vp) return vp;
	vp=argv->r->v;
	if _oF(vp->type&type_string)
	{
		path=vp->v.v_string;
		if _oF(!path) goto Err_notopen;
		vp=base->type_empty(obj);
		if _oF(vp) return vp;
		wav=wav_load(&path);
		if _oF(!wav)
		{
			if _oT(path) return get_error(path,label);
			else goto Err_mem;
		}
	}
	else
	{
		time=vpntof(vp);
		vp=base->type_empty(obj);
		if _oF(vp) return vp;
		if _oF((time<=0)||(time>_phy_maxtime)||(time>_lim_maxtime->v.v_float)) return base->get_error(errid_ReqOver,label);
		wav=wav_new(time);
		if _oF(!wav) goto Err_mem;
	}
	v.v_void=wav->data;
	vp=base->var_set(obj,"data",tlog_int,wav->size,free_pointer|auth_read|auth_write,&v);
	if _oF(!vp)
	{
		wav_free(wav);
		goto Err_mem;
	}
	v.v_void=wav;
	vp=base->var_set(obj,_wav_,tlog_void,leng_no,free_pointer,&v);
	if _oF(!vp)
	{
		free(wav);
		goto Err_mem;
	}
	v.v_long=wav->size;
	vp=base->var_set(obj,"size",tlog_int,leng_no,auth_read,&v);
	if _oF(!vp) goto Err_mem;
	vp->type|=type_unsign;
	vp=base->type_set(obj,type);
	if _oF(vp) return vp;
	return ret;
	Err_mem:
	return base->get_error(errid_MemLess,label);
	Err_notopen:
	return get_error(error_load_path,label);
}

func(write)
{
	static char *label=label_name("write");
	static u32 type_2[2]={type_object,type_string};
	var *obj,*vp;
	vya_wav *wav;
	char *path;
	
	ret->type=type_void;
	ret->v.v_long=0;
	if _oF((argc!=2)||base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	obj=argv->v;
	vp=argv->r->v;
	path=vp->v.v_string;
	if _oF(base->type_check(obj,type->v.v_string)<=0) goto Err_check;
	if _oF(!path) goto Err_path;
	wav=get_wav(obj);
	if _oF(!wav) goto Err_wav;
	if _oF(wav_write(wav,path)) goto Err_path;
	return ret;
	Err_check:
	return get_error(error_type,label);
	Err_path:
	return get_error(error_path,label);
	Err_wav:
	return get_error(error_wav,label);
}

func(loudness)
{
	static char *label=label_name("loudness");
	static u32 type_1[1]={type_num};
	var *vp;
	
	ret->type=type_int;
	if _oF((argc!=1)||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->v.v_long=loudness(vpntof(vp));
	return ret;
}

func(loudexpe)
{
	static char *label=label_name("loudexpe");
	static u32 type_1[1]={type_num};
	var *vp;
	
	ret->type=type_float;
	if _oF((argc!=1)||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->v.v_float=loudexpe(vpntof(vp));
	return ret;
}

func(clear)
{
	static char *label=label_name("clear");
	static u32 type_1[1]={type_object};
	static u32 type_3[3]={type_object,type_num,type_num};
	var *obj,*vp;
	vya_wav *wav;
	u32 b;
	s32 t;
	
	ret->type=type_void;
	ret->v.v_long=0;
	switch(argc)
	{
		case 1:
			if _oF(base->check_varlist(argv,1,type_1)) goto Err_argv;
			obj=argv->v;
			if _oF(base->type_check(obj,type->v.v_string)<=0) goto Err_check;
			wav=get_wav(obj);
			if _oF(!wav) goto Err_wav;
			b=0;
			t=wav->size;
			break;
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) goto Err_argv;
			obj=argv->v;
			if _oF(base->type_check(obj,type->v.v_string)<=0) goto Err_check;
			wav=get_wav(obj);
			if _oF(!wav) goto Err_wav;
			argv=argv->r;
			vp=argv->v;
			b=vpntof(vp);
			argv=argv->r;
			vp=argv->v;
			t=vpntof(vp);
			break;
		default:
			Err_argv:
			return base->get_error(errid_FunArgvType,label);
	}
	clear(wav,b,t);
	return ret;
	Err_check:
	return get_error(error_type,label);
	Err_wav:
	return get_error(error_wav,label);
}

func(rsin_inc)
{
	static char *label=label_name("rsin_inc");
	static u32 type_5[5]={type_object,type_num,type_num,type_num,type_num};
	var *obj,*vp;
	vya_wav *wav;
	double loud;
	u32 n,b;
	s32 t;
	
	ret->type=type_void;
	ret->v.v_long=0;
	if _oF((argc!=5)||base->check_varlist(argv,5,type_5)) return base->get_error(errid_FunArgvType,label);
	// obj
	obj=argv->v;
	if _oF(base->type_check(obj,type->v.v_string)<=0) goto Err_check;
	wav=get_wav(obj);
	if _oF(!wav) goto Err_wav;
	// n
	argv=argv->r;
	vp=argv->v;
	n=vpntof(vp);
	// loud
	argv=argv->r;
	vp=argv->v;
	loud=vpntof(vp);
	// b
	argv=argv->r;
	vp=argv->v;
	b=vpntof(vp);
	// t
	argv=argv->r;
	vp=argv->v;
	t=vpntof(vp);
	// call
	rsin_inc(wav,n,loudness(loud),b,t);
	return ret;
	Err_check:
	return get_error(error_type,label);
	Err_wav:
	return get_error(error_wav,label);
}

func(rcos_inc)
{
	static char *label=label_name("rcos_inc");
	static u32 type_5[5]={type_object,type_num,type_num,type_num,type_num};
	var *obj,*vp;
	vya_wav *wav;
	double loud;
	u32 n,b;
	s32 t;
	
	ret->type=type_void;
	ret->v.v_long=0;
	if _oF((argc!=5)||base->check_varlist(argv,5,type_5)) return base->get_error(errid_FunArgvType,label);
	// obj
	obj=argv->v;
	if _oF(base->type_check(obj,type->v.v_string)<=0) goto Err_check;
	wav=get_wav(obj);
	if _oF(!wav) goto Err_wav;
	// n
	argv=argv->r;
	vp=argv->v;
	n=vpntof(vp);
	// loud
	argv=argv->r;
	vp=argv->v;
	loud=vpntof(vp);
	// b
	argv=argv->r;
	vp=argv->v;
	b=vpntof(vp);
	// t
	argv=argv->r;
	vp=argv->v;
	t=vpntof(vp);
	// call
	rcos_inc(wav,n,loudness(loud),b,t);
	return ret;
	Err_check:
	return get_error(error_type,label);
	Err_wav:
	return get_error(error_wav,label);
}

func(rsin_get)
{
	static char *label=label_name("rsin_get");
	static u32 type_4[4]={type_object,type_num,type_num,type_num};
	var *obj,*vp;
	vya_wav *wav;
	u32 n,b;
	s32 t;
	
	ret->type=type_float;
	if _oF((argc!=4)||base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
	// obj
	obj=argv->v;
	if _oF(base->type_check(obj,type->v.v_string)<=0) goto Err_check;
	wav=get_wav(obj);
	if _oF(!wav) goto Err_wav;
	// n
	argv=argv->r;
	vp=argv->v;
	n=vpntof(vp);
	// b
	argv=argv->r;
	vp=argv->v;
	b=vpntof(vp);
	// t
	argv=argv->r;
	vp=argv->v;
	t=vpntof(vp);
	// call
	ret->v.v_float=loudexpe(rsin_get(wav,n,b,t));
	return ret;
	Err_check:
	return get_error(error_type,label);
	Err_wav:
	return get_error(error_wav,label);
}

func(rcos_get)
{
	static char *label=label_name("rcos_get");
	static u32 type_4[4]={type_object,type_num,type_num,type_num};
	var *obj,*vp;
	vya_wav *wav;
	u32 n,b;
	s32 t;
	
	ret->type=type_float;
	if _oF((argc!=4)||base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
	// obj
	obj=argv->v;
	if _oF(base->type_check(obj,type->v.v_string)<=0) goto Err_check;
	wav=get_wav(obj);
	if _oF(!wav) goto Err_wav;
	// n
	argv=argv->r;
	vp=argv->v;
	n=vpntof(vp);
	// b
	argv=argv->r;
	vp=argv->v;
	b=vpntof(vp);
	// t
	argv=argv->r;
	vp=argv->v;
	t=vpntof(vp);
	// call
	ret->v.v_float=loudexpe(rcos_get(wav,n,b,t));
	return ret;
	Err_check:
	return get_error(error_type,label);
	Err_wav:
	return get_error(error_wav,label);
}

func(aloud)
{
	static char *label=label_name("aloud");
	static u32 type_1[1]={type_object|type_num};
	static u32 type_3[3]={type_object|type_num,type_num,type_num};
	double sa=0;
	void *sp;
	u32 s,t,i;
	var *o,*vp;
	vya_wav *wav;
	
	ret->type=type_float;
	ret->v.v_float=0;
	switch(argc)
	{
		case 1:
			if _oF(base->check_varlist(argv,1,type_1)) goto Err_argv;
			o=argv->v;
			if _oF(o->type&type_object)
			{
				// wav
				if _oF(base->type_check(o,type->v.v_string)<=0) goto Err_check;
				wav=get_wav(o);
				if _oF(!wav) goto Err_wav;
				s=0;
				t=wav->size;
				goto _w;
			}
			else
			{
				// array
				if _oF(o->length==leng_no) goto Err_argv;
				s=0;
				t=o->length;
				if _oF(o->type&type_float) goto _f;
				else goto _i;
			}
			goto Err_argv;
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) goto Err_argv;
			o=argv->v;
			argv=argv->r;
			vp=argv->v;
			s=vpntof(vp);
			argv=argv->r;
			vp=argv->v;
			t=vpntof(vp);
			if _oF(o->type&type_object)
			{
				// wav
				if _oF(base->type_check(o,type->v.v_string)<=0) goto Err_check;
				wav=get_wav(o);
				if _oF(!wav) goto Err_wav;
				if _oF(s>=wav->size) goto End;
				if _oF(s+t>wav->size) goto End;
				if _oF(s+t<=s) goto End;
				goto _w;
			}
			else
			{
				// array
				if _oF(o->length==leng_no) goto Err_argv;
				if _oF(s>=o->length) goto End;
				if _oF(s+t>o->length) goto End;
				if _oF(s+t<=s) goto End;
				if _oF(o->type&type_float) goto _f;
				else goto _i;
			}
			goto Err_argv;
		default:
			Err_argv:
			return base->get_error(errid_FunArgvType,label);
	}
	_f:
	sp=o->v.v_void;
	t+=s;
	for(i=s;i<t;i++)
		sa+=((double*)sp)[i]*((double*)sp)[i];
	ret->v.v_float=sqrt(sa/(t-s));
	goto End;
	_i:
	sp=o->v.v_void;
	t+=s;
	switch(o->type)
	{
		case type_byte:
			for(i=s;i<t;i++)
				sa+=(double)((s8*)sp)[i]*((s8*)sp)[i];
			break;
		case type_byte|type_unsign:
			for(i=s;i<t;i++)
				sa+=(double)((u8*)sp)[i]*((u8*)sp)[i];
			break;
		case type_word:
			for(i=s;i<t;i++)
				sa+=(double)((s16*)sp)[i]*((s16*)sp)[i];
			break;
		case type_word|type_unsign:
			for(i=s;i<t;i++)
				sa+=(double)((u16*)sp)[i]*((u16*)sp)[i];
			break;
		case type_int:
			for(i=s;i<t;i++)
				sa+=(double)((s32*)sp)[i]*((s32*)sp)[i];
			break;
		case type_int|type_unsign:
			for(i=s;i<t;i++)
				sa+=(double)((u32*)sp)[i]*((u32*)sp)[i];
			break;
		case type_long:
			for(i=s;i<t;i++)
				sa+=(double)((s64*)sp)[i]*((s64*)sp)[i];
			break;
		case type_long|type_unsign:
			for(i=s;i<t;i++)
				sa+=(double)((u64*)sp)[i]*((u64*)sp)[i];
			break;
		default:
			goto Err_argv;
	}
	ret->v.v_float=sqrt(sa/(t-s));
	goto End;
	_w:
	sp=o->v.v_void;
	t+=s;
	for(i=s;i<t;i++)
		sa+=(double)wav->data[i]*wav->data[i];
	sa=sqrt(sa/(t-s));
	ret->v.v_float=loudexpe((s32)(sa+0.5));
	End:
	return ret;
	Err_check:
	return get_error(error_type,label);
	Err_wav:
	return get_error(error_wav,label);
}

func(noise)
{
	static char *label=label_name("noise");
	static u32 type_1[1]={type_object|type_num};
	static u32 type_3[3]={type_object|type_num,type_num,type_num};
	double *sa;
	u32 s,t,i;
	var *o,*vp;
	vya_wav *wav;
	
	ret->type=type_float;
	ret->v.v_float=0;
	switch(argc)
	{
		case 1:
			if _oF(base->check_varlist(argv,1,type_1)) goto Err_argv;
			o=argv->v;
			if _oF(o->type&type_object)
			{
				// wav
				if _oF(base->type_check(o,type->v.v_string)<=0) goto Err_check;
				wav=get_wav(o);
				if _oF(!wav) goto Err_wav;
				s=0;
				t=wav->size;
				goto _w;
			}
			else
			{
				// array
				if _oF(o->length==leng_no) goto Err_argv;
				s=0;
				t=o->length;
				if _oF(o->type&type_float) goto _f;
				else goto _i;
			}
			goto Err_argv;
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) goto Err_argv;
			o=argv->v;
			argv=argv->r;
			vp=argv->v;
			s=vpntof(vp);
			argv=argv->r;
			vp=argv->v;
			t=vpntof(vp);
			if _oF(o->type&type_object)
			{
				// wav
				if _oF(base->type_check(o,type->v.v_string)<=0) goto Err_check;
				wav=get_wav(o);
				if _oF(!wav) goto Err_wav;
				if _oF(s>=wav->size) goto End;
				if _oF(s+t>wav->size) goto End;
				if _oF(s+t<=s) goto End;
				goto _w;
			}
			else
			{
				// array
				if _oF(o->length==leng_no) goto Err_argv;
				if _oF(s>=o->length) goto End;
				if _oF(s+t>o->length) goto End;
				if _oF(s+t<=s) goto End;
				if _oF(o->type&type_float) goto _f;
				else goto _i;
			}
			goto Err_argv;
		default:
			Err_argv:
			return base->get_error(errid_FunArgvType,label);
	}
	_f:
	sa=malloc(sizeof(double)*t);
	if _oF(!sa) goto Err_mem;
	for(i=0;i<t;i++)
		sa[i]=o->v.vp_float[s+i];
	ret->v.v_float=noise(sa,t);
	free(sa);
	goto End;
	_i:
	sa=malloc(sizeof(double)*t);
	if _oF(!sa) goto Err_mem;
	switch(o->type)
	{
		case type_byte:
			for(i=0;i<t;i++)
				sa[i]=o->v.vp_byte[s+i];
			break;
		case type_byte|type_unsign:
			for(i=0;i<t;i++)
				sa[i]=(u8)o->v.vp_byte[s+i];
			break;
		case type_word:
			for(i=0;i<t;i++)
				sa[i]=o->v.vp_word[s+i];
			break;
		case type_word|type_unsign:
			for(i=0;i<t;i++)
				sa[i]=(u16)o->v.vp_word[s+i];
			break;
		case type_int:
			for(i=0;i<t;i++)
				sa[i]=o->v.vp_int[s+i];
			break;
		case type_int|type_unsign:
			for(i=0;i<t;i++)
				sa[i]=(u32)o->v.vp_int[s+i];
			break;
		case type_long:
			for(i=0;i<t;i++)
				sa[i]=o->v.vp_long[s+i];
			break;
		case type_long|type_unsign:
			for(i=0;i<t;i++)
				sa[i]=(u64)o->v.vp_long[s+i];
			break;
		default:
			free(sa);
			goto Err_argv;
	}
	ret->v.v_float=noise(sa,t);
	free(sa);
	goto End;
	_w:
	sa=malloc(sizeof(double)*t);
	if _oF(!sa) goto Err_mem;
	for(i=0;i<t;i++)
		sa[i]=wav->data[s+i];
	ret->v.v_float=loudexpe((s32)(noise(sa,t)+0.5));
	free(sa);
	End:
	return ret;
	Err_check:
	return get_error(error_type,label);
	Err_wav:
	return get_error(error_wav,label);
	Err_mem:
	return base->get_error(errid_MemLess,label);
}

