#include "../main.h"

u32 get_tlog(char *type)
{
	/*
		type list:
		void void[]
		[s,u]byte [s,u]byte[]
		[s,u]word [s,u]word[]
		[s,u]int [s,u]int[]
		[s,u]long [s,u]long[]
		float float[]
		string string[]
		vlist vmat
	*/
	// get type
	switch(type[0])
	{
		case 'b':
			if _oF(strcmp(type,"byte")==0) return tlog_byte;
			else return tlog_max;
		case 'f':
			if _oF(strcmp(type,"float")==0) return tlog_float;
			else return tlog_max;
		case 'i':
			if _oF(strcmp(type,"int")==0) return tlog_int;
			else return tlog_max;
		case 'l':
			if _oF(strcmp(type,"long")==0) return tlog_long;
			else return tlog_max;
		case 's':
			if _oF(strcmp(type,"string")==0) return tlog_string;
			else if _oF(strcmp(type,"sbyte")==0) return tlog_byte;
			else if _oF(strcmp(type,"sint")==0) return tlog_int;
			else if _oF(strcmp(type,"slong")==0) return tlog_long;
			else if _oF(strcmp(type,"sword")==0) return tlog_word;
			else return tlog_max;
		case 'u':
			if _oF(strcmp(type,"ubyte")==0) return tlog_byte;
			else if _oF(strcmp(type,"uint")==0) return tlog_int;
			else if _oF(strcmp(type,"ulong")==0) return tlog_long;
			else if _oF(strcmp(type,"uword")==0) return tlog_word;
			else return tlog_max;
		case 'v':
			if _oF(strcmp(type,"void")==0) return tlog_void;
			else if _oF(strcmp(type,"vlist")==0) return tlog_vlist;
			else if _oF(strcmp(type,"vmat")==0) return tlog_vmat;
			else return tlog_max;
		case 'w':
			if _oF(strcmp(type,"word")==0) return tlog_word;
			else return tlog_max;
		default:
			return tlog_max;
	}
}

var* var_define(char *exp, char **expp, var *root)
{
	static char *label="var_define";
	char buffer[16],*name=NULL;
	var *vp;
	vlist *vl;
	u32 length;
	int i=0,tlog,ex=0;
	while(i<15&&is_name(*exp))
	{
		buffer[i++]=*(exp++);
	}
	buffer[i]=0;
	tlog=get_tlog(buffer);
	if _oF(tlog>=tlog_max) goto Err_nottype;
	while(is_space(*exp)) exp++;
	if _oT(*exp==':')
	{
		exp++;
		while(is_space(*exp)) exp++;
	}
	// ex struct type:(name= , ...), ...
	if _oF(*exp=='(')
	{
		ex=1;
		exp++;
		while(is_space(*exp)) exp++;
	}
	Loop:
	// get name
	if _oF(!is_Name(*exp)) goto Err_notname;
	name=get_name(exp,&exp);
	if _oF(!name) goto Err_malloc;
	// check var
	vl=v_find(root,name);
	if _oF(vl)
	{
		// check var's auth
		vp=vl->v;
		if _oF(!(vp->mode&auth_retype))
		{
			if _oT(vp->type^(1<<tlog)^(buffer[0]=='u'?type_unsign:0)) goto Err_notretype;
			else if _oF(!(vp->mode&auth_relength)) goto Err_notrelength;
		}
	}
	// check name
	else if _oF(var_find(_vm_gobj,name)) goto Err_notname;
	// alloc link
	if _oT(!vl)
	{
		// chack root's auth
		if _oF(!(root->mode&auth_write)) goto Err_nowrite;
		vl=vlist_alloc(name);
		if _oF(!vl) goto Err_malloc;
		if _oT(root->type&type_vlist) root->v.v_vlist=vlist_insert(root->v.v_vlist,vl);
		else vmat_insert(root->v.v_vmat,vl);
	}
	// get [length]
	while(is_space(*exp)) exp++;
	if _oF(*exp=='[')
	{
		exp++;
		while(is_space(*exp)) exp++;
		length=get_int(exp,&exp,&vp);
		if _oF(vp) goto End;
		if _oF(*exp!=']') goto Err_notbra;
		exp++;
		while(is_space(*exp)) exp++;
	}
	else length=0;
	if _oF(length&&(tlog==tlog_vlist||tlog==tlog_vmat)) goto Err_nottype;
	// check length
	if _oF(length>_lim_array_max->v.v_long) goto Err_notlength;
	// alloc var
	vp=var_alloc(tlog,length);
	if _oF(!vp)
	{
		if _oT(root->type&type_vlist) root->v.v_vlist=vlist_delete(root->v.v_vlist,name);
		else vmat_delete(root->v.v_vmat,name);
		goto Err_malloc;
	}
	if _oF(buffer[0]=='u') vp->type|=type_unsign;
	// set var auth
	if _oF(vl->v) vp->mode=((vp->mode)&(~auth_all))|((vl->v->mode)&auth_all);
	// var link
	vlist_link(vl,vp);
	// get [value]
	if _oF((vp->type&(type_num|type_string))&&*exp=='=')
	{
		exp++;
		while(is_space(*exp)) exp++;
		if _oF(vp->length)
		{
			// array
			if _oT(*exp=='[')
			{
				exp++;
				while(is_space(*exp)) exp++;
				i=0;
				switch(vp->type&type_all)
				{
					case type_byte:
					while(i<length)
					{
						vl->v->v.vp_byte[i]=get_int(exp,&exp,&vp);
						if _oF(vp) goto End;
						if _oF(*exp!=',') break;
						while(i<length&&*exp==',')
						{
							exp++;
							i++;
							while(is_space(*exp)) exp++;
						}
					}
					break;
					case type_word:
					while(i<length)
					{
						vl->v->v.vp_word[i]=get_int(exp,&exp,&vp);
						if _oF(vp) goto End;
						if _oF(*exp!=',') break;
						while(i<length&&*exp==',')
						{
							exp++;
							i++;
							while(is_space(*exp)) exp++;
						}
					}
					break;
					case type_int:
					while(i<length)
					{
						vl->v->v.vp_int[i]=get_int(exp,&exp,&vp);
						if _oF(vp) goto End;
						if _oF(*exp!=',') break;
						while(i<length&&*exp==',')
						{
							exp++;
							i++;
							while(is_space(*exp)) exp++;
						}
					}
					break;
					case type_long:
					while(i<length)
					{
						vl->v->v.vp_long[i]=get_int(exp,&exp,&vp);
						if _oF(vp) goto End;
						if _oF(*exp!=',') break;
						while(i<length&&*exp==',')
						{
							exp++;
							i++;
							while(is_space(*exp)) exp++;
						}
					}
					break;
					case type_float:
					while(i<length)
					{
						vl->v->v.vp_float[i]=get_float(exp,&exp,&vp);
						if _oF(vp) goto End;
						if _oF(*exp!=',') break;
						while(i<length&&*exp==',')
						{
							exp++;
							i++;
							while(is_space(*exp)) exp++;
						}
					}
					break;
					case type_string:
					while(i<length)
					{
						if _oT(*exp=='\"')
						{
							vp->v.vp_string[i]=get_string(exp,&exp);
						}
						else if _oT(*exp=='{')
						{
							vp->v.vp_string[i]=get_code(exp,&exp);
						}
						while(is_space(*exp)) exp++;
						if _oF(*exp!=',') break;
						while(i<length&&*exp==',')
						{
							exp++;
							i++;
							while(is_space(*exp)) exp++;
						}
					}
					break;
				}
				if _oF(*exp!=']') goto Err_notbra;
				exp++;
			}
		}
		else
		{
			if _oF(vp->type&type_znum)
			{
				vl->v->v.v_long=get_int(exp,&exp,&vp);
				if _oF(vp) goto End;
				var_fixvalue(vl->v);
			}
			else if _oF(vp->type&type_float)
			{
				 vl->v->v.v_float=get_float(exp,&exp,&vp);
				 if _oF(vp) goto End;
			}
			else
			{
				if _oT(*exp=='\"')
				{
					vp->v.v_string=get_string(exp,&exp);
					vp->mode|=free_pointer;
				}
				else if _oT(*exp=='{')
				{
					vp->v.v_string=get_code(exp,&exp);
					vp->mode|=free_pointer;
				}
			}
		}
	}
	while(is_space(*exp)) exp++;
	if _oF(ex)
	{
		if _oT(*exp==',')
		{
			exp++;
			while(is_space(*exp)) exp++;
			goto Loop;
		}
		else if _oT(*exp==')')
		{
			exp++;
			while(is_space(*exp)) exp++;
			goto End;
		}
		goto Err_notbra;
	}
	End:
	if _oT(expp) *expp=exp;
	return vp;
	Err_nottype:
	vp=get_error(errid_VarUnknowType,label);
	goto End;
	Err_notname:
	vp=get_error(errid_GraQuoVarname,label);
	goto End;
	Err_malloc:
	vp=get_error(errid_MemLess,label);
	goto End;
	Err_notbra:
	vp=get_error(errid_GraBraMismatch,label);
	goto End;
	Err_notlength:
	vp=get_error(errid_ReqOver,label);
	goto End;
	Err_notretype:
	vp=get_error(errid_VarNotRetype,label);
	goto End;
	Err_nowrite:
	vp=get_error(errid_VarNotWrite,label);
	goto End;
	Err_notrelength:
	vp=get_error(errid_VarNotRelength,label);
	goto End;
}

var* function_define(char *exp, char **expp, var **_vt_, var **_vn_)
{
	static char *label="function_define";
	vlist *vl=NULL;
	var *vp,*vt,*vn;
	char *name;
	u32 size;
	while(is_space(*exp)) exp++;
	if _oF(*exp==')')
	{
		size=1;
		vl=vlist_alloc("void");
		if _oF(!vl) goto Err_malloc;
		vl->mode|=type_void;
		goto Turn;
	}
	size=0;
	Loop:
	if _oF(!is_Name(*exp)) goto Err_notname;
	name=get_name(exp,&exp);
	if _oF(!name) goto Err_malloc;
	if _oF(var_find(_vm_gobj,name)) goto Err_notname;
	if _oT(vl)
	{
		vl->r=vlist_alloc(name);
		if _oF(!vl->r) goto Err_malloc;
		vl->r->l=vl;
		vl=vl->r;
	}
	else
	{
		vl=vlist_alloc(name);
		if _oF(!vl) goto Err_malloc;
	}
	size++;
	while(is_space(*exp)) exp++;
	if _oT(*exp==':')
	{
		exp++;
		while(is_space(*exp)) exp++;
	}
	vl->mode|=get_int(exp,&exp,&vp)&type_all;
	if _oF(vp) goto Err;
	switch(*exp)
	{
		case 0:
		case ';':
		case ')':
			goto Turn;
		case ',':
			exp++;
			while(is_space(*exp)) exp++;
			goto Loop;
		default:
			goto Err_unknowsym;
	}
	Turn:
	if _oF(size>_lim_fargc_max->v.v_long) goto Err_fargc;
	vt=var_alloc(tlog_int,size);
	if _oF(!vt) goto Err_malloc;
	vn=var_alloc(tlog_string,size);
	if _oF(!vn)
	{
		var_free(vt);
		goto Err_malloc;
	}
	vt->type|=type_unsign;
	vt->mode=auth_read|free_pointer;
	vn->mode=auth_read|free_pointer;
	do
	{
		size--;
		vt->v.vp_int[size]=vl->mode&type_all;
		vn->v.vp_string[size]=vl->name;
		vl->mode&=~free_name;
		if _oT(vl->l) vl=vl->l;
	}
	while(size);
	*_vt_=vt;
	*_vn_=vn;
	vp=NULL;
	Err:
	if _oT(vl) vlist_free(vl);
	if _oT(expp) *expp=exp;
	return vp;
	Err_notname:
	vp=get_error(errid_GraQuoVarname,label);
	goto Err;
	Err_unknowsym:
	vp=get_error(errid_GraUnknowSym,label);
	goto Err;
	Err_fargc:
	vp=get_error(errid_ReqOver,label);
	goto Err;
	Err_malloc:
	vp=get_error(errid_MemLess,label);
	goto Err;
}

char* goto_exp(char *text, char *label)
{
	while(*text)
	{
		while(is_space(*text)) text++;
		if _oF(*text=='#') 
		{
			text++;
			if _oF(is_name(*text))
			{
				if _oF(cmp_label(text,label)==0)
				{
					while(is_name(*text)) text++;
					while(is_space(*text)) text++;
					return text;
				}
			}
			else
			{
				text=skip_note(text-1);
				continue;
			}
		}
		text=get_nextsem(text);
		if _oT(*text==';') text++;
	}
	return NULL;
}

char* goto_exp_last(char *this, char *text, char *label)
{
	char *res;
	while(this>text)
	{
		this=get_lastsem(this,text);
		res=this;
		if _oT(*this==';') this++;
		Loop:
		while(is_space(*this)) this++;
		if _oF(*this=='#')
		{
			this++;
			if _oF(is_name(*this))
			{
				if _oF(cmp_label(this,label)==0)
				{
					while(is_name(*this)) this++;
					while(is_space(*this)) this++;
					return this;
				}
			}
			else
			{
				this=skip_note(this-1);
				goto Loop;
			}
		}
		if _oF(res==text) return NULL;
		else this=res-1;
	}
	return NULL;
}


