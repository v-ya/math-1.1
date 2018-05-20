#include "main.h"
// 错误码转字符串
#include "error.string"


// 异常系统出错时使用
static var _v__err = {
	.type	=type_err,
	.length	=leng_no,
	.inode	=0,
	.mode	=0,
	.v	=0
} ;

error_info* error_info_alloc(void)
{
	error_info *i;
	i=malloc(sizeof(error_info));
	if _oF(!i) return NULL;
	memset(i,0,sizeof(error_info));
	return i;
}

error_info* error_info_get(void)
{
	var *vp;
	vp=ptvar_get(_pt_error);
	if _oF(!vp) return NULL;
	return vp->v.v_error;
}

var* error_insert(error_info *i)
{
	var *vp;
	vp=ptvar_get(_pt_error);
	if _oF(!vp)
	{
		// alloc
		vp=var_alloc(tlog_void,leng_no);
		// :(
		if _oF(!vp) goto Err;
		vp->type=type_err;
		vp->v.v_error=i;
		ptvar_replace(_pt_error,vp);
		if _oF(!ptvar_get(_pt_error)) goto Err;
		return vp;
	}
	i->last=vp->v.v_error;
	vp->v.v_error=i;
	return vp;
	Err:
	if _oF(vp) free(vp);
	free(i);
	return &_v__err;
}

var* get_error(u32 errid, char *label)
{
	error_info *i;
	i=error_info_alloc();
	if _oF(!i) return &_v__err;
	i->errid=errid;
	i->label=label;
	return error_insert(i);
}

void set_error(var *pt_text, char *script)
{
	error_info *i;
	char *text;
	i=error_info_get();
	if _oF(!i) return ;
	var_save(pt_text);
	i->pt_text=pt_text;
	text=pt_text->v.v_string;
	if _oF(!text) return ;
	i->text_p=(u32)(script-text);
}

void clr_error(void)
{
	var *vp;
	error_info *i;
	vp=ptvar_get(_pt_error);
	if _oT(vp)
	{
		i=vp->v.v_error;
		while(i)
		{
			vp->v.v_error=i->last;
			var_free(i->pt_text);
			free(i);
			i=vp->v.v_error;
		}
	}
	ptvar_free(_pt_error);
}

// [object].try's error backup

void backup_error(void)
{
	var *vp;
	backup_clr_error();
	vp=ptvar_get(_pt_error);
	if _oF(vp)
	{
		ptvar_replace(_pt_error,NULL);
		ptvar_free(_pt_error);
		ptvar_replace(_pt_buerr,vp);
	}
}

void backup_clr_error(void)
{
	var *vp;
	error_info *i;
	vp=ptvar_get(_pt_buerr);
	if _oT(vp)
	{
		i=vp->v.v_error;
		while(i)
		{
			vp->v.v_error=i->last;
			var_free(i->pt_text);
			free(i);
			i=vp->v.v_error;
		}
	}
	ptvar_free(_pt_buerr);
}

// End

char* str_error_cl(u32 errid)
{
	errid>>=16;
	errid&=0xff;
	return error_clA[errid];
}

char* str_error_ty(u32 errid)
{
	char **errtyA;
	errtyA=error_tyA[(errid>>16)&0xff];
	return errtyA[(errid>>8)&0xff];
}

char* str_error_id(u32 errid)
{
	char ***errtyA,**erridA;
	errtyA=error_idA[(errid>>16)&0xff];
	erridA=errtyA[(errid>>8)&0xff];
	return erridA[errid&0xff];
}

void print_error(void)
{
	error_info *i;
	var *pt_text;
	char *text,*s;
	u32 line,colu,size;
	i=error_info_get();
	while(i)
	{
		print("%s: errid = %08x, %s.%s.%s\n",i->label,i->errid,
			str_error_cl(i->errid),str_error_ty(i->errid),str_error_id(i->errid));
		text=NULL;
		pt_text=i->pt_text;
		if _oT(pt_text) text=pt_text->v.v_string;
		if _oT(text)
		{
			s=text+i->text_p;
			// get line
			for(line=1;text<s;text++)
			{
				if _oF(*text=='\n') line++;
			}
			text=pt_text->v.v_string;
			// get colu
			for(colu=0;s>text;s--)
			{
				if _oF(*s=='\n') break;
				colu++;
			}
			if (*s=='\n') s++;
			text=s;
			while(*s&&*s!='\n') s++;
			size=s-text;
			s=malloc(size+1);
			if _oF(!s) text=str_error_id(errid_MemLess);
			else
			{
				memcpy(s,text,size);
				s[size]=0;
				for(size=0;s[size];size++)
				{
					if _oF(s[size]=='\t') s[size]=' ';
				}
				text=s;
			}
			print("    @(%d,%d): %s\n",line,colu,text);
			free(s);
			print("    @(%d,%d): %*c\n",line,colu,colu,'^');
		}
		i=i->last;
	}
}



