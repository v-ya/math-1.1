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

