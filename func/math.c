#include "../main.h"

static u32 t_v[1]={type_void};
static u32 t_f[1]={type_num};
static u32 t_z[1]={type_znum};
static u32 t_ff[2]={type_num, type_num};

// srand, rand
func(srand)
{
	static char *label=".srand";
	ret->type=type_void;
	ret->v.v_long=0;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	if _oF(!check_varlist(argv,1,t_v))
	{
		srand(time(NULL));
		return ret;
	}
	else if _oT(!check_varlist(argv,1,t_z))
	{
		srand(argv->v->v.v_long);
		return ret;
	}
	else return get_error(errid_FunArgvType,label);
}
func(rand)
{
	static char *label=".rand";
	s64 rand_max;
	var *vp;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	if _oF(!check_varlist(argv,1,t_v))
	{
		ret->type=type_long;
		ret->v.v_long=rand();
		return ret;
	}
	else if _oT(!check_varlist(argv,1,t_f))
	{
		vp=argv->v;
		if _oF(vp->type&type_float)
		{
			// float mode
			ret->type=type_float;
			ret->v.v_float=vp->v.v_float*rand()/RAND_MAX_L;
			return ret;
		}
		else
		{
			// integer mode
			ret->type=type_long;
			rand_max=vp->v.v_long;
			ret->v.v_long=(s64)((float)vp->v.v_long*rand()/RAND_MAX_L);
			rand_max/=RAND_MAX_L;
			if _oF(rand_max) do
			{
				ret->v.v_long+=(s64)(vp->v.v_float*rand()/RAND_MAX_L);
				rand_max/=RAND_MAX_L;
			} while(rand_max);
			return ret;
		}
	}
	else return get_error(errid_FunArgvType,label);
}

// [a]{sin, cos, tan}[h], atan2, acot2
func(sin)
{
	static char *label=".sin";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=sin(vpntof(vp));
	return ret;
}
func(cos)
{
	static char *label=".cos";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=cos(vpntof(vp));
	return ret;
}
func(tan)
{
	static char *label=".tan";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=tan(vpntof(vp));
	return ret;
}
func(asin)
{
	static char *label=".asin";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=asin(vpntof(vp));
	return ret;
}
func(acos)
{
	static char *label=".acos";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=acos(vpntof(vp));
	return ret;
}
func(atan)
{
	static char *label=".atan";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=atan(vpntof(vp));
	return ret;
}
func(sinh)
{
	static char *label=".sinh";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=sinh(vpntof(vp));
	return ret;
}
func(cosh)
{
	static char *label=".cosh";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=cosh(vpntof(vp));
	return ret;
}
func(tanh)
{
	static char *label=".tanh";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=tanh(vpntof(vp));
	return ret;
}
func(asinh)
{
	static char *label=".asinh";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=asinh(vpntof(vp));
	return ret;
}
func(acosh)
{
	static char *label=".acosh";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=acosh(vpntof(vp));
	return ret;
}
func(atanh)
{
	static char *label=".atanh";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=atanh(vpntof(vp));
	return ret;
}
func(atan2)
{
	static char *label=".atan2";
	var *vp1,*vp2;
	if _oF((argc!=2)||check_varlist(argv,2,t_ff)) return get_error(errid_FunArgvType,label);
	vp1=argv->v;
	vp2=argv->r->v;
	ret->type=type_float;
	ret->v.v_float=atan2(vpntof(vp1),vpntof(vp2));
	return ret;
}
func(acot2)
{
	static char *label=".acot2";
	var *vp1,*vp2;
	if _oF((argc!=2)||check_varlist(argv,2,t_ff)) return get_error(errid_FunArgvType,label);
	vp1=argv->v;
	vp2=argv->r->v;
	ret->type=type_float;
	ret->v.v_float=atan2(vpntof(vp2),vpntof(vp1));
	return ret;
}
func(hypot)
{
	static char *label=".hypot";
	var *vp1,*vp2;
	if _oF((argc!=2)||check_varlist(argv,2,t_ff)) return get_error(errid_FunArgvType,label);
	vp1=argv->v;
	vp2=argv->r->v;
	ret->type=type_float;
	ret->v.v_float=hypot(vpntof(vp1),vpntof(vp2));
	return ret;
}

// exp, sqrt, ln, lg, log
func(exp)
{
	static char *label=".exp";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=exp(vpntof(vp));
	return ret;
}
func(sqrt)
{
	static char *label=".sqrt";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=sqrt(vpntof(vp));
	return ret;
}
func(ln)
{
	static char *label=".ln";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=log(vpntof(vp));
	return ret;
}
func(log2)
{
	static char *label=".log2";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=log2(vpntof(vp));
	return ret;
}
func(log10)
{
	static char *label=".log10";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=log10(vpntof(vp));
	return ret;
}
func(log)
{
	static char *label=".log";
	var *vp1,*vp2;
	if _oF((argc!=2)||check_varlist(argv,2,t_ff)) return get_error(errid_FunArgvType,label);
	vp1=argv->v;
	vp2=argv->r->v;
	ret->type=type_float;
	ret->v.v_float=log(vpntof(vp2))/log(vpntof(vp1));
	return ret;
}

// ceil, floor, round, abs
func(ceil)
{
	static char *label=".ceil";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=ceil(vpntof(vp));
	return ret;
}
func(floor)
{
	static char *label=".floor";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=floor(vpntof(vp));
	return ret;
}
func(round)
{
	static char *label=".round";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	ret->type=type_float;
	ret->v.v_float=round(vpntof(vp));
	return ret;
}
func(abs)
{
	static char *label=".abs";
	var *vp;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	if _oF(vp->type&type_float)
	{
		ret->type=type_float;
		ret->v.v_float=(vp->v.v_float<0)?(-vp->v.v_float):vp->v.v_float;
		return ret;
	}
	else
	{
		ret->type=type_long;
		ret->v.v_long=(vp->v.v_long<0)?(-vp->v.v_long):vp->v.v_long;
		return ret;
	}
}


func(ftol)
{
	static char *label=".ftol";
	var *vp;
	double f;
	if _oF((argc!=1)||check_varlist(argv,1,t_f)) return get_error(errid_FunArgvType,label);
	vp=argv->v;
	f=vpntof(vp);
	ret->type=type_long|type_unsign;
	ret->v.v_long=*((u64*)(&f));
	return ret;
}


