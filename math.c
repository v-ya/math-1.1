#include "main.h"

var* temp_get(void)
{
	static char *label="temp_get";
	var *vp;
	vp=ptvar_get(_pt_temp);
	if _oF(!vp)
	{
		ptvar_alloc(_pt_temp);
		vp=(var*) ptvar_vlist(_pt_temp);
		if _oF(!vp) return get_error(errid_MemLess,label);
		((vlist*)vp)->v=var_alloc(tlog_void,leng_no);
		if _oF(!((vlist*)vp)->v) return get_error(errid_MemLess,label);
		vp=((vlist*)vp)->v;
		vp->mode=auth_tmpvar|free_temp;
		return vp;
	}
	return vp;
}

void temp_free(void)
{
	var *vp;
	vp=ptvar_get(_pt_temp);
	if _oT(vp) vp->mode&=~free_temp;
	ptvar_free(_pt_temp);
}

var* get_varlist(char **expp, int *argc, vlist **argv)
{
	static char *label="get_varlist";
	var *vp,*temp;
	vlist *vl;
	char *exp;
	// 初始化
	*argc=0;
	*argv=NULL;
	exp=*expp;
	temp=temp_get();
	if _oF(temp->type&type_err) return temp;
	// 循环入口
	Loop:
	while(is_space(*exp)) exp++;
	vp=cal(exp,&exp);
	if _oF(vp->type&type_spe)
	{
		vlist_free(*argv);
		*argc=0;
		*argv=NULL;
		*expp=exp;
		return vp;
	}
	vl=vlist_alloc(NULL);
	if _oF(!vl)
	{
		_err_mem:
		vlist_free(*argv);
		*argc=0;
		*argv=NULL;
		*expp=exp;
		return get_error(errid_MemLess,label);
	}
	if _oT(*argv)
	{
		(*argv)->r=vl;
		vl->l=*argv;
	}
	*argv=vl;
	vl->mode=free_pointer;
	// 硬链接
	if _oF(vp==temp)
	{
		vp=malloc(sizeof(var));
		if _oF(!vp) goto _err_mem;
		vp->type=temp->type;
		vp->length=0;
		vp->inode=1;
		vp->mode=auth_tmpvar;
		vp->v.v_long=temp->v.v_long;
	}
	vl->v=vp;
	(*argc)++;
	while(is_space(*exp)) exp++;
	switch(*exp)
	{
		case 0:
		case ')':
			while(vl->l) vl=vl->l;
			*argv=vl;
			*expp=exp;
			return NULL;
		
		case ',':
			exp++;
			goto Loop;
		default:
			// 未知符号
			vlist_free(*argv);
			*argc=0;
			*argv=NULL;
			*expp=exp;
			return get_error(errid_GraUnknowSym,label);
	}
}

int check_varlist(vlist *vl, u32 argc, u32 type[])
{
	u32 i;
	for(i=0;i<argc;i++)
	{
		if _oF(!vl) return 1;
		else if _oF(!vl->v) return 1;
		else if _oF(!(vl->v->type&type[i])) return 1;
		else if _oF((!(type[i]&type_allowarray))&&vl->v->length) return 1;
		vl=vl->r;
	}
	return 0;
}

char* run_key(char *exp, var *root, var **function)
{
	static char *label="run_key";
	if _oT((*function)->type&type_void)
	{
		root=(*((*function)->v.v_key))(root,&exp);
		if _oF(root)
		{
			*function=root;
			return exp;
		}
		else
		{
			*function=temp_get();
			if _oF((*function)->type&type_err) return exp;
			(*function)->type=type_void;
			(*function)->v.v_long=0;
			return exp;
		}
	}
	else
	{
		*function=get_error(errid_VarNotKey,label);
		return exp;
	}
}

var* run_fun_vlist(var *func, int argc, vlist *argv)
{
	static char *label="run_fun_vlist";
	var *ret,*pt_text,*_vn_,*caller;
	u32 *vt,i;
	char **vn;
	vlist *vl,*vl1,*vc;
	// get _vt_, _vn_, _text_
	ret=var_find(func,"_vt_");
	if _oF(!ret) goto Err_func;
	if _oF(ret->length!=argc) goto Err_argv;
	vt=ret->v.vp_int;
	_vn_=var_find(func,"_vn_");
	if _oF(!_vn_) goto Err_func;
	if _oF(_vn_->length!=argc) goto Err_argv;
	vn=_vn_->v.vp_string;
	pt_text=var_find(func,"_text_");
	if _oF(!pt_text) goto Err_func;
	// get caller
	caller=ptvar_get(_pt_this);
	// check var list type
	vl=argv;
	for(i=0;i<argc;i++)
	{
		if _oF(!(vt[i]&vl->v->type)) goto Err_argv;
		vl=vl->r;
	}
	// link caller
	vc=vlist_find(func->v.v_vlist,"_caller_");
	if _oF(!vc)
	{
		vc=var_insert(func,"_caller_",tlog_max,leng_no);
		if _oF(!vc) goto Err_malloc;
		func->v.v_vlist=vc;
		vc->mode|=free_pointer;
	}
	if _oT(caller) vlist_link(vc,caller);
	// link argv to .*
	vl1=argv;
	for(i=0;i<argc;i++)
	{
		vl=vlist_find(func->v.v_vlist,vn[i]);
		if _oF(!vl)
		{
			vl=var_insert(func,vn[i],tlog_max,leng_no);
			if _oF(!vl) goto Err_malloc;
			func->v.v_vlist=vl;
			vl->mode|=free_pointer;
		}
		vlist_link(vl,vl1->v);
		vl1=vl1->r;
	}
	// run
	var_save(func);
	var_save(_vn_);
	ret=run_script(pt_text,func);
	if _oT(!ret) ret=var_find(func,"_ret_");
	// unlink .*
	if _oT(vc&&vc->v)
	{
		var_free(vc->v);
		vc->v=NULL;
	}
	vl1=argv;
	for(i=0;i<argc;i++)
	{
		vl=vlist_find(func->v.v_vlist,vn[i]);
		if _oT(vl)
		{
			var_free(vl->v);
			vl->v=NULL;
		}
	}
	if _oF(func->inode<=1)
	{
		if _oT(ret&&(!(ret->type&type_err)))
		ret=NULL;
	}
	var_free(_vn_);
	var_free(func);
	return ret;
	Err_func:
	return get_error(errid_VarUnknowFun,label);
	Err_argv:
	return get_error(errid_FunArgvType,label);
	Err_malloc:
	argc=i;
	vl1=argv;
	for(i=0;i<argc;i++)
	{
		vl=vlist_find(func->v.v_vlist,vn[i]);
		if _oT(vl)
		{
			var_free(vl->v);
			vl->v=NULL;
		}
	}
	return get_error(errid_MemLess,label);
}

char* run_fun(char *exp, var *root, var **function)
{
	static char *label="run_fun";
	int argc;
	vlist *argv;
	var v={.length=0,.inode=0,.mode=auth_tmpvar};
	if _oF(*exp!='(') return exp;
	exp++;
	v.v.v_void=get_varlist(&exp,&argc,&argv);
	if _oF(v.v.v_void)
	{
		*function=(var*)(v.v.v_void);
		return exp;
	}
	if _oF(*exp!=')')
	{
		*function=get_error(errid_GraBraMismatch,label);
		return exp;
	}
	exp++;
	if _oT((*function)->type&type_void)
	{
		// 指针函数，系统内设函数
		v.type=root->type;
		v.mode=root->mode;
		v.v.v_void=root->v.v_void;
		*function=(*((*function)->v.v_fun))(&v,argc,argv);
		if _oT(*function==&v)
		{
			*function=temp_get();
			if _oT(!((*function)->type&type_err))
			{
				(*function)->type=v.type;
				(*function)->v.v_long=v.v.v_long;
			}
		}
	}
	else if _oT((*function)->type&type_vlist)
	{
		*function=run_fun_vlist(*function,argc,argv);
		if _oF(!(*function))
		{
			*function=temp_get();
			if _oT(!((*function)->type&type_err))
			{
				(*function)->type=type_void;
				(*function)->v.v_long=0;
			}
		}
		else var_save(*function);
	}
	else *function=get_error(errid_VarUnknowFun,label);
	// 清除获取的变量链
	vlist_free(argv);
	return exp;
}

var* get_var(char *exp, char **expp, int *array_n)
{
	static char *label="get_var";
	var *root,*vp,*func,v;
	char *name,*s=exp;
	u32 length,mode;
	if _oF((*exp>='0'&&*exp<='9')||*exp=='\"'||*exp=='\''||*exp=='{'||*exp=='-')
	{
		vp=temp_get();
		if _oF(vp->type&type_err) return vp;
		get_tmpvar(exp,&exp,vp);
		if _oF(vp->type&type_string)
		{
			root=vp;
			vp=malloc(sizeof(var));
			if _oF(!vp)
			{
				root->mode&=~free_pointer;
				goto Err_malloc;
			}
			memcpy(vp,root,sizeof(var));
			root->mode&=~free_pointer;
			vp->mode&=~free_temp;
			vp->inode=1;
		}
		while(is_space(*exp)) exp++;
		if _oT(expp) *expp=exp;
		return vp;
	}
	// lock
	if _oF(*exp=='$')
	{
		root=_vm_user;
		exp++;
		while(is_space(*exp)) exp++;
	}
	else if _oF(*exp=='.')
	{
		root=_vm_sysm;
		exp++;
		while(is_space(*exp)) exp++;
	}
	else
	{
		// 获取该线程的 pt_this;
		root=ptvar_get(_pt_this);
		if _oF(!root) goto Err_ptvar;
	}
	Loop:
	if _oF(root->length||(!(root->type&type_object))) goto Err_notobj;
	if _oT(is_Name(*exp))
	{
		name=get_name(exp,&exp);
		if _oF(!name) goto Err_malloc;
		vp=var_find(root,name);
		if _oF(!vp) vp=var_find(_vm_gobj,name);
	}
	else if _oT(*exp=='[')
	{
		exp++;
		while(is_space(*exp)) exp++;
		vp=cal(exp,&exp);
		if _oF(vp->type&type_err) goto Err;
		if _oF((!(vp->type&type_string))||(vp->v.v_string==NULL))
		{
			var_free(vp);
			goto Err_notstring;
		}
		func=vp;
		vp=var_find(root,func->v.v_string);
		if _oF(!vp) vp=var_find(_vm_gobj,func->v.v_string);
		var_free(func);
		if _oF(*exp!=']') goto Err_notbra;
		exp++;
	}
	else
	{
		name=s;
		if _oF(*name=='.'||*name=='$') name++;
		while(is_space(*name)) name++;
		if _oT(name==exp)
		{
			if _oF(*s=='.'||*s=='$')
			{
				vp=root;
				goto End;
			}
			else
			{
				vp=temp_get();
				if _oF(vp->type&type_err) goto Err;
				vp->type=type_void;
				vp->v.v_long=0;
				goto End;
			}
		}
		else goto Err_notname;
	}
	if _oF(!vp) goto Err_notfind;
	if _oF(vp->type==type_refer)
	{
		vp=refer_check(vp);
		if _oF(!vp) goto Err_notrefer;
	}
	LoopEntra:
	while(is_space(*exp)) exp++;
	if _oF(!(vp->mode&auth_read)) goto Err_notread;
	if _oF((vp->mode&auth_key))
	{
		func=vp;
		var_save(root);
		var_save(func);
		// unlock
		exp=run_key(exp,root,&vp);
		var_free(func);
		var_free(root);
		if _oF(expp) *expp=exp;
		return vp;
	}
	if _oF(vp->length&&array_n) *array_n=-1;
	switch(*exp)
	{
		case '.':
			exp++;
			while(is_space(*exp)) exp++;
			root=vp;
			goto Loop;
		case '(':
			if _oF(!(vp->mode&auth_run)) goto Err_notrun;
			func=vp;
			mode=func->mode;
			func->mode&=~auth_retype;
			var_save(func);
			// unlock
			exp=run_fun(exp,root,&vp);
			// lock
			func->mode=mode;
			if _oF(vp->type&type_err)
			{
				var_free(func);
				goto Err;
			}
			if _oF(vp->inode==0) ;
			else if _oF((!(vp->mode&free_temp))&&(vp->inode==1))
			{
				var_free(vp);
				vp=temp_get();
				if _oF(vp->type&type_err)
				{
					var_free(func);
					goto Err;
				}
				vp->type=type_void;
				vp->v.v_long=0;
			}
			else var_free(vp);
			var_free(func);
			goto LoopEntra;
		case '[':
			if _oF(!vp->length)
			{
				if _oT(vp->type&type_object)
				{
					root=vp;
					goto Loop;
				}
				else goto Err_notarray;
			}
			if _oF(!vp->v.v_void) goto Err_arraybuffer;
			exp++;
			root=vp;
			mode=root->mode;
			root->mode&=~(auth_retype|auth_write);
			var_save(root);
			// unlock
			length=get_int(exp,&exp,&vp);
			// lock
			root->mode=mode;
			if _oF(vp)
			{
				var_free(root);
				goto Err;
			}
			if _oF(root->inode<=1)
			{
				var_free(root);
				goto Err_notfind;
			}
			var_free(root);
			if _oF(*exp!=']') goto Err_notbra;
			if _oF(length>=root->length) goto Err_arraylength;
			exp++;
			while(is_space(*exp)) exp++;
			if _oF(array_n&&(*exp!='.'||*exp!='('||*exp!='['))
			{
				*array_n=length;
				vp=root;
				goto End;
			}
			vp=temp_get();
			if _oF(vp->type&type_err) goto Err;
			vp->type=root->type;
			switch(root->type&type_all)
			{
				case type_byte:
					vp->v.v_long=root->v.vp_byte[length];
					var_fixvalue(vp);
					goto LoopEntra;
				case type_word:
					vp->v.v_long=root->v.vp_word[length];
					var_fixvalue(vp);
					goto LoopEntra;
				case type_int:
					vp->v.v_long=root->v.vp_int[length];
					var_fixvalue(vp);
					goto LoopEntra;
				case type_long:
					vp->v.v_long=root->v.vp_long[length];
					var_fixvalue(vp);
					goto LoopEntra;
				case type_float:
					vp->v.v_float=root->v.vp_float[length];
					goto LoopEntra;
				default:
					vp->v.v_void=root->v.vp_void[length];
					goto LoopEntra;
			}
	}
	End:
	if _oF(isstring(vp)&&vp->v.v_string&&(vp==temp_get()))
	{
		root=vp;
		vp=var_alloc(tlog_string,leng_no);
		if _oF(!vp) goto Err_malloc;
		length=strlen(root->v.v_string)+1;
		vp->v.v_string=malloc(length);
		if _oF(!vp->v.v_string)
		{
			free(vp);
			goto Err_malloc;
		}
		vp->mode=auth_tmpvar|free_pointer;
		memcpy(vp->v.v_string,root->v.v_string,length);
	}
	var_save(vp);
	Err:
	// unlock
	if _oF(expp) *expp=exp;
	return vp;
	Err_malloc:
	vp=get_error(errid_MemLess,label);
	goto Err;
	Err_ptvar:
	vp=get_error(errid_SysPtvarNotfind,label);
	goto Err;
	Err_notobj:
	vp=get_error(errid_VarNotObject,label);
	goto Err;
	Err_notname:
	vp=get_error(errid_GraQuoVarname,label);
	goto Err;
	Err_notfind:
	vp=get_error(errid_VarNotFind,label);
	goto Err;
	Err_notrefer:
	vp=get_error(errid_VarReferFail,label);
	goto Err;
	Err_notread:
	vp=get_error(errid_VarNotRead,label);
	goto Err;
	Err_notarray:
	vp=get_error(errid_VarNotArray,label);
	goto Err;
	Err_notrun:
	vp=get_error(errid_VarNotRun,label);
	goto Err;
	Err_notstring:
	vp=get_error(errid_VarNotString,label);
	goto Err;
	Err_notbra:
	vp=get_error(errid_GraBraMismatch,label);
	goto Err;
	Err_arraybuffer:
	vp=get_error(errid_ArrayBuffer,label);
	goto Err;
	Err_arraylength:
	vp=get_error(errid_ArrayLength,label);
	goto Err;
}

var* cal_strcat(var *r0, var *r1)
{
	static char *label="cal_strcat";
	var *r;
	int s0,s1;
	if _oF(r0->length||r1->length||r0->type!=type_string||r1->type!=type_string) goto Err_type;
	r=malloc(sizeof(var));
	if _oF(!r) goto Err_malloc;
	r->type=type_string;
	r->length=leng_no;
	r->mode=auth_tmpvar;
	r->inode=1;
	r->v.v_string=NULL;
	if _oT(r0->v.v_string) s0=strlen(r0->v.v_string);
	else s0=0;
	if _oT(r1->v.v_string) s1=strlen(r1->v.v_string);
	else s1=0;
	if _oT(s0||s1)
	{
		if _oT(s0&&s1)
		{
			r->v.v_string=malloc(s0+s1+1);
			if _oF(!r->v.v_string) goto Err_malloc;
			r->mode|=free_pointer;
			memcpy(r->v.v_string,r0->v.v_string,s0);
			memcpy(r->v.v_string+s0,r1->v.v_string,s1+1);
		}
		else if _oT(s0)
		{
			r->v.v_string=malloc(s0+1);
			if _oF(!r->v.v_string) goto Err_malloc;
			r->mode|=free_pointer;
			memcpy(r->v.v_string,r0->v.v_string,s0+1);
		}
		else
		{
			r->v.v_string=malloc(s1+1);
			if _oF(!r->v.v_string) goto Err_malloc;
			r->mode|=free_pointer;
			memcpy(r->v.v_string,r1->v.v_string,s1+1);
		}
	}
	return r;
	Err_type:
	return get_error(errid_GraVarType,label);
	Err_malloc:
	if (r) free(r);
	return get_error(errid_MemLess,label);
}

var* cal_strtran(var *r0, var *r1)
{
	static char *label="cal_strtran";
	var *sbuf,t={.mode=auth_tmpvar|free_temp};
	vlist vl;
	int size;
	// r0 = r1 || r1 @ r0
	if _oF(r0->length) goto Err_type;
	if _oT((r1->type&type_string)&&(r1->length==leng_no))
	{
		if _oT(r0->type&type_string) goto _SS;
		else goto _NS;
	}
	else if _oT(r0->type&type_string) goto _SN;
	else goto Err_type;
	_SS:
	// str = str
		if _oF(r0->mode&free_pointer)
		{
			free(r0->v.v_string);
			r0->mode&=~free_pointer;
		}
		r0->v.v_string=NULL;
		if _oT(r1->v.v_string)
		{
			size=strlen(r1->v.v_string)+1;
			r0->v.v_string=malloc(size);
			if _oF(!r0->v.v_string) goto Err_malloc;
			memcpy(r0->v.v_string,r1->v.v_string,size);
			r0->mode|=free_pointer;
		}
		return NULL;
	_NS:
	// num = str
		if _oF(!r1->v.v_string) t.type=type_void;
		else get_tmpvar(r1->v.v_string,NULL,&t);
		if _oF(!(t.type&type_num))
		{
			if _oF(t.mode&free_pointer) free(t.v.v_void);
			t.type=type_long;
			t.v.v_long=0;
		}
		size=(r0->type|t.type)&type_all;
		if _oT(!(size&~type_znum)) r0->v.v_long = t.v.v_long;
		else if _oT(!(size&~type_num))
		{
			if _oT(r0->type&type_znum) r0->v.v_long = (s64)t.v.v_float;
			else r0->v.v_float = (t.type&type_znum)?t.v.v_long:t.v.v_float;
		}
		else goto Err_type;
		return NULL;
	_SN:
	// str = num
		if _oF(r0->mode&free_pointer)
		{
			free(r0->v.v_string);
			r0->mode&=~free_pointer;
		}
		r0->v.v_string=NULL;
		if _oF((r1->type&type_void)&&(r1->length==leng_no)) return NULL;
		vl.l=NULL;
		vl.r=NULL;
		vl.v=r1;
		sbuf=sbuf_sprintf("%?",&vl);
		size=strlen(sbuf->v.v_string)+1;
		r0->v.v_string=malloc(size);
		if _oF(!r0->v.v_string) goto Err_malloc;
		memcpy(r0->v.v_string,sbuf->v.v_string,size);
		r0->mode|=free_pointer;
		return NULL;
	// error
	Err_type:
	return get_error(errid_IntError,label);
	Err_malloc:
	return get_error(errid_MemLess,label);
}

var* cal(char *exp, char **expp)
{
	// 定义语法块入口
	static void *GraAddr[256]={
		[0]=&&GraAddr_End,		// 结束
		[';']=&&GraAddr_End,		// 结束		;
		[',']=&&GraAddr_End,		// 结束		,
		[')']=&&GraAddr_End,		// 结束		)
		[']']=&&GraAddr_End,		// 结束		]
		['+']=&&GraAddr_Add,		// 加法		+, +=
		['-']=&&GraAddr_Sub,		// 减法		-, -=
		['*']=&&GraAddr_Mul,		// 乘法		*, *=, **
		['/']=&&GraAddr_Div,		// 除法		/, /=
		['%']=&&GraAddr_Divr,		// 求余		%, %=
		['=']=&&GraAddr_Equ,		// 相等，赋值	==, =
		['!']=&&GraAddr_Not,		// 非		!=
		['<']=&&GraAddr_Lss,		// 小于		<, <=
		['>']=&&GraAddr_Gtr,		// 大于		>, >=
		['&']=&&GraAddr_And,		// 且		&, &=, &&
		['|']=&&GraAddr_Or,		// 或		|, |=, ||
		['^']=&&GraAddr_Xor,		// 异或		^, ^=, ^^
		['?']=&&GraAddr_Con,		// 条件选择	?
		[':']=&&GraAddr_Clr,		// 重新开始	:
		['@']=&&GraAddr_Ass,		// 赋值		@
		['$']=&&GraAddr_Tmp		// 临时化	$
	} ;
	static char *label="cal";
	void *RetAddr;
	int *array_n,an;
	u32 t,con=0;
	var *r,*r0=NULL,*ra,*vt,real={.inode=0,.length=0,.mode=auth_tmpvar|free_temp};
	// 获取 r
	array_n=NULL;
	Begin:
	RetAddr=&&LoopEntra;
	goto FuncGetVar;
	LoopEntra:
	RetAddr=GraAddr[*exp];
	if _oF(!RetAddr) goto Err_unknowsym;
	exp++;
	goto *RetAddr;
	// 入口分拣
	GraAddr_End:
		exp--;
		if _oF(r==&real)
		{
			r=temp_get();
			if _oF(r->type&type_err) goto Err;
			r->type=real.type;
			r->v.v_long=real.v.v_long;
		}
		if _oT(expp) *expp=exp;
		return r;
	GraAddr_Add:
		if _oF(*exp=='=')
		{
			// +=
			exp++;
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			RetAddr=&&GraAddr_Add_AddEqu;
			goto FuncGetVar;
			GraAddr_Add_AddEqu:
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r0->v.v_long += r->v.v_long;
			else if _oT(!(t&~type_num))
			{
				if _oT(r0->type&type_znum) r0->v.v_long += (s64)r->v.v_float;
				else r0->v.v_float += vpntof(r);
			}
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
		else
		{
			// +
			if _oF(r->type&type_string)
			{
				// strcat
				r0=r;
				RetAddr=&&GraAddr_Add_AddStr;
				goto FuncGetVar;
				GraAddr_Add_AddStr:
				vt=r;
				r=cal_strcat(r0,vt);
				var_free(r0);
				var_free(vt);
				r0=NULL;
				if _oF(r->type&type_spe) goto Err;
				goto LoopEntra;
			}
			else if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Add_Add;
			goto FuncGetVar;
			GraAddr_Add_Add:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				real.v.v_long=real.v.v_long + r->v.v_long;
				real.type=type_long;
			}
			else if _oT(!(t&~type_num))
			{
				real.v.v_float=vpntof(&real) + vpntof(r);
				real.type=type_float;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Sub:
		if _oF(*exp=='=')
		{
			// -=
			exp++;
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			RetAddr=&&GraAddr_Sub_SubEqu;
			goto FuncGetVar;
			GraAddr_Sub_SubEqu:
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r0->v.v_long -= r->v.v_long;
			else if _oT(!(t&~type_num))
			{
				if _oT(r0->type&type_znum) r0->v.v_long -= (s64)r->v.v_float;
				else r0->v.v_float -= vpntof(r);
			}
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
		else
		{
			// -
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Sub_Sub;
			goto FuncGetVar;
			GraAddr_Sub_Sub:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				real.v.v_long=real.v.v_long - r->v.v_long;
				real.type=type_long;
			}
			else if _oT(!(t&~type_num))
			{
				real.v.v_float=vpntof(&real) - vpntof(r);
				real.type=type_float;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Mul:
		if _oF(*exp=='=')
		{
			// *=
			exp++;
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			RetAddr=&&GraAddr_Mul_MulEqu;
			goto FuncGetVar;
			GraAddr_Mul_MulEqu:
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r0->v.v_long *= r->v.v_long;
			else if _oT(!(t&~type_num))
			{
				if _oT(r0->type&type_znum) r0->v.v_long *= (s64)r->v.v_float;
				else r0->v.v_float *= vpntof(r);
			}
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
		if _oF(*exp=='*')
		{
			// **
			exp++;
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Mul_MulMul;
			goto FuncGetVar;
			GraAddr_Mul_MulMul:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				real.v.v_long=(s64)pow(real.v.v_long, r->v.v_long);
				real.type=type_long;
			}
			else if _oT(!(t&~type_num))
			{
				real.v.v_float=pow(vpntof(&real), vpntof(r));
				real.type=type_float;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
		else
		{
			// *
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Mul_Mul;
			goto FuncGetVar;
			GraAddr_Mul_Mul:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				real.v.v_long=real.v.v_long * r->v.v_long;
				real.type=type_long;
			}
			else if _oT(!(t&~type_num))
			{
				real.v.v_float=vpntof(&real) * vpntof(r);
				real.type=type_float;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Div:
		if _oF(*exp=='=')
		{
			// /=
			exp++;
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			RetAddr=&&GraAddr_Div_DivEqu;
			goto FuncGetVar;
			GraAddr_Div_DivEqu:
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				if _oF(r->v.v_long==0) goto Err_divzero;
				r0->v.v_long /= r->v.v_long;
			}
			else if _oT(!(t&~type_num))
			{
				if _oF(vpntof(r)==0) goto Err_divzero;
				if _oT(r0->type&type_znum) r0->v.v_long /= (s64)r->v.v_float;
				else r0->v.v_float /= vpntof(r);
			}
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
		else
		{
			// /
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Div_Div;
			goto FuncGetVar;
			GraAddr_Div_Div:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				if _oF(r->v.v_long==0) goto Err_divzero;
				real.v.v_long=real.v.v_long / r->v.v_long;
				real.type=type_long;
			}
			else if _oT(!(t&~type_num))
			{
				if _oF(vpntof(r)==0) goto Err_divzero;
				real.v.v_float=vpntof(&real) / vpntof(r);
				real.type=type_float;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Divr:
		if _oF(*exp=='=')
		{
			// %=
			exp++;
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			RetAddr=&&GraAddr_Divr_DivrEqu;
			goto FuncGetVar;
			GraAddr_Divr_DivrEqu:
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				if _oF(r->v.v_long==0) goto Err_divzero;
				r0->v.v_long %= r->v.v_long;
			}
			else if _oT(!(t&~type_num))
			{
				if _oF(vpntof(r)==0) goto Err_divzero;
				if _oT(r0->type&type_znum) r0->v.v_long %= (s64)r->v.v_float;
				else r0->v.v_float = fmod(r0->v.v_float, vpntof(r));
			}
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
		else
		{
			// %
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Divr_Divr;
			goto FuncGetVar;
			GraAddr_Divr_Divr:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				if _oF(r->v.v_long==0) goto Err_divzero;
				real.v.v_long=real.v.v_long % r->v.v_long;
				real.type=type_long;
			}
			else if _oT(!(t&~type_num))
			{
				if _oF(vpntof(r)==0) goto Err_divzero;
				real.v.v_float=fmod(vpntof(&real),vpntof(r));
				real.type=type_float;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Equ:
		if _oF(*exp=='=')
		{
			// ==
			exp++;
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				if _oF(r->type&type_string) r0=r;
				else
				{
					real.v.v_long=r->v.v_long;
					var_free(r);
				}
			}
			RetAddr=&&GraAddr_Equ_EquEqu;
			goto FuncGetVar;
			GraAddr_Equ_EquEqu:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_num))
			{
				real.v.v_long=(vpntof(&real) == vpntof(r));
				real.type=type_long;
			}
			else if _oT(!(t&~type_string))
			{
				real.v.v_long=!strcmp(r0->v.v_string,r->v.v_string);
				real.type=type_long;
				var_free(r0);
				r0=NULL;
			}
			else if _oF(r0)
			{
				var_free(r0);
				r0=NULL;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
		else
		{
			// =
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			r=cal(exp,&exp);
			if _oF(r->type&type_spe) goto Err;
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r0->v.v_long = r->v.v_long;
			else if _oT(!(t&~type_num))
			{
				if _oT(r0->type&type_znum) r0->v.v_long = (s64)r->v.v_float;
				else r0->v.v_float = vpntof(r);
			}
			else if _oT((!(t&~type_snum))||(r0->type&type_string))
			{
				vt=cal_strtran(r0,r);
				if _oF(vt)
				{
					var_free(r);
					r=vt;
					goto Err;
				}
			}
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
	GraAddr_Not:
		if _oT(*exp=='=')
		{
			// !=
			exp++;
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Not_NotEqu;
			goto FuncGetVar;
			GraAddr_Not_NotEqu:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_num))
			{
				real.v.v_long=(vpntof(&real) != vpntof(r));
				real.type=type_long;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
		else
		{
			exp--;
			goto Err_unknowsym;
		}
	GraAddr_Lss:
		if _oF(*exp=='=')
		{
			// <=
			exp++;
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Lss_LssEqu;
			goto FuncGetVar;
			GraAddr_Lss_LssEqu:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_num))
			{
				real.v.v_long=(vpntof(&real) <= vpntof(r));
				real.type=type_long;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
		else
		{
			// <
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Lss_Lss;
			goto FuncGetVar;
			GraAddr_Lss_Lss:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_num))
			{
				real.v.v_long=(vpntof(&real) < vpntof(r));
				real.type=type_long;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Gtr:
		if _oF(*exp=='=')
		{
			// >=
			exp++;
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Gtr_GtrEqu;
			goto FuncGetVar;
			GraAddr_Gtr_GtrEqu:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_num))
			{
				real.v.v_long=(vpntof(&real) >= vpntof(r));
				real.type=type_long;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
		else
		{
			// >
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Gtr_Gtr;
			goto FuncGetVar;
			GraAddr_Gtr_Gtr:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_num))
			{
				real.v.v_long=(vpntof(&real) > vpntof(r));
				real.type=type_long;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_And:
		if _oF(*exp=='=')
		{
			// &=
			exp++;
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			RetAddr=&&GraAddr_And_AndEqu;
			goto FuncGetVar;
			GraAddr_And_AndEqu:
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r0->v.v_long &= r->v.v_long;
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
		else if (*exp=='&')
		{
			// &&
			exp++;
			if _oF(r!=&real)
			{
				if _oF(r->length) real.type=type_long;
				else real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_And_AndAnd;
			goto FuncGetVar;
			GraAddr_And_AndAnd:
			real.v.v_long=vpbool(&real) & vpbool(r);
			real.type=type_long;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
		else
		{
			// &
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_And_And;
			goto FuncGetVar;
			GraAddr_And_And:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				real.v.v_long=real.v.v_long & r->v.v_long;
				real.type=type_long;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Or:
		if _oF(*exp=='=')
		{
			// |=
			exp++;
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			RetAddr=&&GraAddr_Or_OrEqu;
			goto FuncGetVar;
			GraAddr_Or_OrEqu:
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r0->v.v_long |= r->v.v_long;
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
		else if (*exp=='|')
		{
			// ||
			exp++;
			if _oF(r!=&real)
			{
				if _oF(r->length) real.type=type_long;
				else real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Or_OrOr;
			goto FuncGetVar;
			GraAddr_Or_OrOr:
			real.v.v_long=vpbool(&real) | vpbool(r);
			real.type=type_long;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
		else
		{
			// |
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Or_Or;
			goto FuncGetVar;
			GraAddr_Or_Or:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				real.v.v_long=real.v.v_long | r->v.v_long;
				real.type=type_long;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Xor:
		if _oF(*exp=='=')
		{
			// ^=
			exp++;
			if _oF(!(r->mode&auth_write)) goto Err_notwrite;
			r0=r;
			RetAddr=&&GraAddr_Xor_XorEqu;
			goto FuncGetVar;
			GraAddr_Xor_XorEqu:
			if _oF(r0->length||r->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r0->v.v_long ^= r->v.v_long;
			else goto Err_nottype;
			var_fixvalue(r0);
			var_free(r);
			r=r0;
			r0=NULL;
			goto LoopEntra;
		}
		else if (*exp=='^')
		{
			// ^^
			exp++;
			if _oF(r!=&real)
			{
				if _oF(r->length) real.type=type_long;
				else real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Xor_XorXor;
			goto FuncGetVar;
			GraAddr_Xor_XorXor:
			real.v.v_long=vpbool(&real) ^ vpbool(r);
			real.type=type_long;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
		else
		{
			// ^
			if _oF(r!=&real)
			{
				if _oF(r->length) goto Err_nottype;
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
			}
			RetAddr=&&GraAddr_Xor_Xor;
			goto FuncGetVar;
			GraAddr_Xor_Xor:
			if _oF(r->length) goto Err_nottype;
			t=(real.type|r->type)&type_all;
			if _oT(!(t&~type_znum))
			{
				real.v.v_long=real.v.v_long ^ r->v.v_long;
				real.type=type_long;
			}
			else goto Err_nottype;
			var_free(r);
			r=&real;
			goto LoopEntra;
		}
	GraAddr_Con:
		if _oT(vpbool(r))
		{
			con=1;
			var_free(r);
			RetAddr=&&LoopEntra;
			goto FuncGetVar;
		}
		else
		{
			con=0;
			exp=get_nextcol(exp);
			if _oF(*exp!=':') goto LoopEntra;
			exp++;
			goto GraAddr_Clr_Entra;
		}
	GraAddr_Clr:
		if _oF(con)
		{
			con=0;
			exp=get_nextcol(exp);
			if _oF(*exp!=':') goto LoopEntra;
			exp++;
		}
		GraAddr_Clr_Entra:
		var_free(r);
		RetAddr=&&LoopEntra;
		goto FuncGetVar;
	GraAddr_Ass:
		if _oF(r->length) goto Err_nottype;
		r0=r;
		if _oT(r0->type&type_num)
		{
			real.type=r0->type;
			real.v.v_long=r0->v.v_long;
			var_free(r0);
			r0=&real;
		}
		array_n=&an;
		RetAddr=&&GraAddr_Ass_Ass;
		goto FuncGetVar;
		GraAddr_Ass_Ass:
		array_n=NULL;
		if _oF(!(r->mode&auth_write)) goto Err_notwrite;
		// r = r0
		if _oF(r->length)
		{
			ra=r;
			r=malloc(sizeof(var));
			if _oF(!r)
			{
				var_free(ra);
				goto Err_malloc;
			}
			r->type=ra->type;
			r->mode=auth_tmpvar;
			r->length=leng_no;
			r->inode=1;
			// tran
			if _oF(r0->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r->v.v_long = r0->v.v_long;
			else if _oT(!(t&~type_num))
			{
				if _oT(r->type&type_znum) r->v.v_long = (s64)r0->v.v_float;
				else r->v.v_float = vpntof(r0);
			}
			else if _oT((!(t&~type_snum))||(r->type&type_string))
			{
				vt=cal_strtran(r,r0);
				if _oF(vt)
				{
					var_free(r);
					r=vt;
					goto Err;
				}
			}
			else goto Err_nottype;
			var_fixvalue(r);
			var_free(r0);
			r0=NULL;
			// set array ra[an]
			switch(ra->type&type_all)
			{
				case type_byte:
					ra->v.vp_byte[an]=r->v.v_byte;
					break;
				case type_word:
					ra->v.vp_word[an]=r->v.v_word;
					break;
				case type_int:
					ra->v.vp_int[an]=r->v.v_int;
					break;
				case type_long:
					ra->v.vp_long[an]=r->v.v_long;
					break;
				case type_float:
					ra->v.vp_float[an]=r->v.v_float;
					break;
				case type_string:
					if _oF(ra->v.vp_string[an]&&(ra->mode&free_pointer)) free(ra->v.vp_string[an]);
					ra->v.vp_string[an]=NULL;
					if _oF(!r->v.v_string) break;
					t=strlen(r->v.v_string)+1;
					ra->v.vp_string[an]=malloc(t);
					if _oF(!ra->v.vp_string[an])
					{
						var_free(ra);
						goto Err_malloc;
					}
					memcpy(ra->v.vp_string[an],r->v.v_string,t);
					break;
				default:
					var_free(ra);
					goto Err_nottype;
			}
			var_free(ra);
			goto LoopEntra;
		}
		else
		{
			if _oF(r0->length) goto Err_nottype;
			t=(r0->type|r->type)&type_all;
			if _oT(!(t&~type_znum)) r->v.v_long = r0->v.v_long;
			else if _oT(!(t&~type_num))
			{
				if _oT(r->type&type_znum) r->v.v_long = (s64)r0->v.v_float;
				else r->v.v_float = vpntof(r0);
			}
			else if _oT((!(t&~type_snum))||(r->type&type_string))
			{
				vt=cal_strtran(r,r0);
				if _oF(vt)
				{
					var_free(r);
					r=vt;
					goto Err;
				}
			}
			else goto Err_nottype;
			var_fixvalue(r);
			var_free(r0);
			r0=NULL;
			goto LoopEntra;
		}
	GraAddr_Tmp:
		if _oT(r!=&real)
		{
			if _oF(r->length) goto Err_nottype;
			if _oF(!(r->type&type_snum)) goto Err_nottype;
			if _oF(r->type&type_string)
			{
				r0=r;
				r=malloc(sizeof(var));
				if _oF(!r) goto Err_malloc;
				r->type=type_string;
				r->mode=auth_tmpvar;
				r->inode=1;
				r->length=leng_no;
				if _oF(!r0->v.v_string) r->v.v_string=NULL;
				else
				{
					t=strlen(r0->v.v_string)+1;
					r->v.v_string=malloc(t);
					if _oF(!r->v.v_string) goto Err_malloc;
					memcpy(r->v.v_string,r0->v.v_string,t);
					r->mode|=free_pointer;
				}
				var_free(r0);
				r0=NULL;
			}
			else
			{
				real.type=r->type;
				real.v.v_long=r->v.v_long;
				var_free(r);
				r=&real;
			}
		}
		while(is_space(*exp)) exp++;
		goto LoopEntra;
	
	// 内部函数 获取变量 =>r (RetAddr, array_n)
	FuncGetVar:
	t=0;
	_FuncGetVar_loop:
	while(is_space(*exp)) exp++;
	switch(*exp)
	{
		case '!':
			exp++;
			if _oF((t&0xff)>=0x10) goto Err_manysym;
			t=((t&0xff)+1)|((t&~0xff)<<1);
			goto _FuncGetVar_loop;
		case '~':
			exp++;
			if _oF((t&0xff)>=0x10) goto Err_manysym;
			t=((t&0xff)+1)|((t&~0xff)<<1)|0x100;
			goto _FuncGetVar_loop;
		case '(':
			exp++;
			while(is_space(*exp)) exp++;
			r=cal(exp,&exp);
			if _oF(r->type&type_spe) goto Err;
			if _oF(*exp!=')') goto Err_notbra;
			exp++;
			while(is_space(*exp)) exp++;
			if _oF(t) goto _FuncGetVar_sym;
			goto *RetAddr;
		default:
			r=get_var(exp,&exp,array_n);
			if _oF(r->type&type_spe) goto Err;
			if _oF(t) goto _FuncGetVar_sym;
			goto *RetAddr;
	}
	_FuncGetVar_sym:
	vt=temp_get();
	if _oF(vt->type&type_err)
	{
		var_free(r);
		r=vt;
		goto Err;
	}
	vt->type=r->type;
	vt->v.v_long=r->v.v_long;
	if _oF(r->length) vt->type=type_string;
	var_free(r);
	r=vt;
	while(t&0xff)
	{
		if _oF(t&0x100)
		{
			// ~
			if _oF(r->type&type_znum)
			{
				r->v.v_long=~r->v.v_long;
				var_fixvalue(r);
			}
			else goto Err_nottype;
		}
		else
		{
			// !
			r->v.v_long=!vpbool(r);
			r->type=type_long;
		}
		t=((t&0xff)-1)|((t>>1)&~0xff);
	}
	goto *RetAddr;
	
	// error
	Err:
	if _oF(r0) var_free(r0);
	if _oT(expp) *expp=exp;
	return r;
	Err_unknowsym:
	var_free(r);
	r=get_error(errid_GraUnknowSym,label);
	goto Err;
	Err_notbra:
	var_free(r);
	r=get_error(errid_GraBraMismatch,label);
	goto Err;
	Err_notwrite:
	var_free(r);
	r=get_error(errid_VarNotWrite,label);
	goto Err;
	Err_nottype:
	var_free(r);
	r=get_error(errid_GraVarType,label);
	goto Err;
	Err_divzero:
	var_free(r);
	r=get_error(errid_IntDivZero,label);
	goto Err;
	Err_malloc:
	var_free(r);
	r=get_error(errid_MemLess,label);
	goto Err;
	Err_manysym:
	var_free(r);
	r=get_error(errid_GraOvermuchUnOp,label);
	goto Err;
}

s64 get_int(char* exp, char **expp, var **vpp_err)
{
	static char *label="get_int";
	s64 r;
	*vpp_err=cal(exp,&exp);
	if _oT(expp) *expp=exp;
	if _oF((*vpp_err)->type&type_spe) return 0;
	if _oT((*vpp_err)->type&type_znum)
	{
		r=(*vpp_err)->v.v_long;
		var_free(*vpp_err);
		*vpp_err=NULL;
		return r;
	}
	else if _oT((*vpp_err)->type&type_float)
	{
		r=(s64)(*vpp_err)->v.v_float;
		var_free(*vpp_err);
		*vpp_err=NULL;
		return r;
	}
	else
	{
		var_free(*vpp_err);
		*vpp_err=get_error(errid_VarNotNumber,label);
		return 0;
	}
}

double get_float(char* exp, char **expp, var **vpp_err)
{
	static char *label="get_float";
	double r;
	*vpp_err=cal(exp,&exp);
	if _oT(expp) *expp=exp;
	if _oF((*vpp_err)->type&type_spe) return 0;
	if _oT((*vpp_err)->type&type_znum)
	{
		r=(*vpp_err)->v.v_long;
		var_free(*vpp_err);
		*vpp_err=NULL;
		return r;
	}
	else if _oT((*vpp_err)->type&type_float)
	{
		r=(*vpp_err)->v.v_float;
		var_free(*vpp_err);
		*vpp_err=NULL;
		return r;
	}
	else
	{
		var_free(*vpp_err);
		*vpp_err=get_error(errid_VarNotNumber,label);
		return 0;
	}
}

// 脚本执行 （文本指针，运行环境：线程独立）
var* run_script(var *pt_text, var *pt_this)
{
	static char *label="run_script";
	var *vp,v,*pt_kill;
	char *script;
	u32 mode;
	
	if _oF(!(pt_text->type&type_string)) return get_error(errid_VarNotString,label);
	vp=NULL;
	v.inode=0;
	v.length=0;
	v.mode=auth_tmpvar;
	mode=pt_text->mode;
	// 清除 pt_text 的权限，计算过程中会更改 pt_text 内容
	pt_text->mode=0;
	script=pt_text->v.v_string;
	// 保留原有的 pt_this, pt_text
	var_save(pt_text);
	pt_text=ptvar_replace(_pt_text,pt_text);
	var_save(pt_this);
	pt_this=ptvar_replace(_pt_this,pt_this);
	pt_kill=ptvar_get(_pt_kill);
	
	if _oF(!script) goto End;
	while(*script)
	{
		if _oF(pt_kill&&pt_kill->v.v_void)
		{
			vp=get_error(errid_IntKilled,label);
			goto Err;
		}
		while(is_space(*script)) script++;
		if _oF(*script==0)
		{
			vp=NULL;
			goto End;
		}
		else if _oF(*script=='#')
		{
			script=skip_note(script);
			continue;
		}
		else if _oF(*script==';')
		{
			script++;
			continue;
		}
		vp=cal(script,&script);
		if _oF(vp->type&type_err)
		{
			// 错误处理
			Err:
			set_error(ptvar_get(_pt_text), script);
			vp=get_error(errid_IntFuncErr, label);
			goto End;
		}
		else if _oF(vp->type&type_end)
		{
			// 正常退出
			vp=NULL;
			goto End;
		}
		var_free(vp);
		if _oF(*script!=';')
		{
			vp=get_error(errid_GraLackSem,label);
			goto Err;
		}
		script++;
	}
	vp=NULL;
	End:
	// 切回原有的 pt_this, pt_text，并删除保存的“副本”
	pt_this=ptvar_replace(_pt_this,pt_this);
	var_free(pt_this);
	pt_text=ptvar_replace(_pt_text,pt_text);
	pt_text->mode=mode;
	var_free(pt_text);
	return vp;
}


