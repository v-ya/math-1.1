#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../type.h"
#include "../string.h"
#include "../var.h"

#ifdef	is_space
	#undef	is_space
#endif

#define	is_space(c)	(c==' '||c=='\t'||c=='\r')

var *_refpool;		// 通过编译

static char buff[4096];	// 缓存区
static char *val[8];	// 缓存行指针
static int vn;		// 行内读取数
static FILE *infile=NULL;
static FILE *outfile=NULL;
static int isdebug;
static var *_vm_root;

// head
static u64 _head_void;
static u64 _head_sbyte;
static u64 _head_ubyte;
static u64 _head_sword;
static u64 _head_uword;
static u64 _head_sint;
static u64 _head_uint;
static u64 _head_slong;
static u64 _head_ulong;
static u64 _head_float;
static u64 _head_string;
static u64 _head_vlist;
static u64 _head_vmat;
static u64 _head_key;
static u64 _head_fun;
static u64 _head_fvlist;

#define Max_length	16

#define	err(s)		_err(__func__, s)
#define debug(fm, ...)	if (isdebug) fprintf(stdout,"%s " fm "\n",__func__, ##__VA_ARGS__)
void _err(const char *func, char *info)
{
	fprintf(stderr,"[error] %s: %s\n",func,info);
	if (infile) fclose(infile);
	if (outfile) fclose(outfile);
	exit(-1);
}

char* get_line(char *exp)
{
	int pos;
	vn=0;
	pos=0;
	while(vn<8)
	{
		while(is_space(*exp)) exp++;
		if (*exp=='\n'||*exp==0) break;
		else if (vn==0&&!(*exp=='.'||*exp=='}'||is_Name(*exp)))
		{
			while(*exp&&*exp!='\n') exp++;
			break;
		}
		val[vn++]=&buff[pos];
		while(*exp&&*exp!='\n'&&(!is_space(*exp)))
		{
			buff[pos++]=*(exp++);
		}
		buff[pos++]=0;
	}
	if (*exp) exp++;
	return exp;
}

int get_mode(char *mode)
{
	int m=0;
	loop:
	switch(*mode)
	{
		case '-':
			mode++;
			goto loop;
		case 's':
			m|=auth_retype;
			mode++;
			goto loop;
		case 'r':
			m|=auth_read;
			mode++;
			goto loop;
		case 'w':
			m|=auth_write;
			mode++;
			goto loop;
		case 'l':
			m|=auth_link;
			mode++;
			goto loop;
		case 'S':
			m|=auth_relength;
			mode++;
			goto loop;
		case 'c':
			m|=auth_system;
			mode++;
			goto loop;
		case 0:
			return m;
		default:
			debug("unknow %c",*mode);
			err("unknow auth");
	}
}

var* bcvar(var *root, char *name, u32 tlog, u32 length)
{
	vlist *vl=NULL;
	var *vp=NULL;
	debug("create %s ...",name);
	if (root)
	{
		if (root->type&type_object)
		{
			vl=vlist_alloc(name);
			if (!vl) goto err;
			vp=var_alloc(tlog,length);
			if (!vp) goto err;
			vlist_link(vl,vp);
			if (root->type&type_vlist) root->v.v_vlist=vlist_insert(root->v.v_vlist,vl);
			else vmat_insert(root->v.v_vmat,vl);
		}
		else err("root not object");
	}
	else
	{
		vp=var_alloc(tlog,length);
		if (!vp) goto err;
		_vm_root=vp;
	}
	// 声明变量
		fprintf(outfile,"static var	_sv_vp_%p;\n",vp);
	if (vl)	fprintf(outfile,"static vlist	_sv_vl_%p;\n",vl);
	return vp;
	err:
	err("malloc fail");
}

void bclink(var *root, char *name, var *vp)
{
	vlist *vl=NULL;
	debug("link %s ...",name);
	if (root)
	{
		if (root->type&type_object)
		{
			vl=vlist_alloc(name);
			if (!vl) goto err;
			vlist_link(vl,vp);
			if (root->type&type_vlist) root->v.v_vlist=vlist_insert(root->v.v_vlist,vl);
			else vmat_insert(root->v.v_vmat,vl);
		}
		else err("root not object");
	}
	else err("root == NULL");
	// 声明变量
	fprintf(outfile,"static vlist	_sv_vl_%p;\n",vl);
	return ;
	err:
	err("malloc fail");
}

var* get_var(var *root, char *lname)
{
	var *vp;
	char c,*sc,*name;
	debug("get %s ...",lname);
	vp=root;
	sc=lname;
	if (*sc=='.') sc++;
	name=sc;
	while(*sc)
	{
		while(*sc&&*sc!='.') sc++;
		c=*sc;
		*sc=0;
		vp=var_find(vp,name);
		*sc=c;
		if (*sc) sc++;
		if (!vp) err("not find var");
		name=sc;
	}
	return vp;
}

void get_v(var *vp, char **exp)
{
	char* make_vm(char *exp, var *root);
	var v;
	switch(vp->type&type_all)
	{
		case type_vlist:
		case type_vmat:
			if (*(val[4])=='{') *exp=make_vm(*exp,vp);
			else err("object value err");
			return ;
		default:
			if (is_Name(*(val[4])))
			{
				vp->v.v_string=malloc(strlen(val[4])+1);
				if (vp->v.v_string==NULL) err("malloc fail");
				strcpy(vp->v.v_string,val[4]);
				vp->mode|=free_name;
				return ;
			}
			else
			{
				get_tmpvar(val[4],NULL,&v);
				vp->v.v_long=v.v.v_long;
				if (v.mode&free_pointer)
				{
					v.mode&=~free_pointer;
					vp->mode|=free_pointer;
				}
				else if (vp->type&type_znum) var_fixvalue(vp);
				return ;
			}
	}
}

var* make_vm_one(var *root,char **exp)
{
	int mode,length;
	u64 head;
	var *vp;
	if (vn<3) err("argc < 3");
	else if (vn==3)
	{
		if (strcmp(val[1],"-link")==0)
		{
			vp=get_var(_vm_root,val[2]);
			bclink(root,val[0],vp);
		}
		else if (strcmp(val[1],"-export")==0)
		{
			// 导出变量
			vp=get_var(_vm_root,val[2]);
			fprintf(outfile,"var *%s = &_sv_vp_%p;\n",val[0],vp);
		}
		else err("unknow -*");
		return NULL;
	}
	length=atoi(val[2]);
	if (length<0||length>Max_length) err("length err");
	mode=get_mode(val[3]);
	head=vname_head(val[1]);
	
	if (head==_head_void) vp=bcvar(root,val[0],tlog_void,length);
	else if (head==_head_sbyte) vp=bcvar(root,val[0],tlog_byte,length);
	else if (head==_head_ubyte) vp=bcvar(root,val[0],tlog_byte,length);
	else if (head==_head_sword) vp=bcvar(root,val[0],tlog_word,length);
	else if (head==_head_uword) vp=bcvar(root,val[0],tlog_word,length);
	else if (head==_head_sint) vp=bcvar(root,val[0],tlog_int,length);
	else if (head==_head_uint) vp=bcvar(root,val[0],tlog_int,length);
	else if (head==_head_slong) vp=bcvar(root,val[0],tlog_long,length);
	else if (head==_head_ulong) vp=bcvar(root,val[0],tlog_long,length);
	else if (head==_head_float) vp=bcvar(root,val[0],tlog_float,length);
	else if (head==_head_string) vp=bcvar(root,val[0],tlog_string,length);
	else if (head==_head_vlist) vp=bcvar(root,val[0],tlog_vlist,length);
	else if (head==_head_vmat) vp=bcvar(root,val[0],tlog_vmat,length);
	else if (head==_head_key)
	{
		if (length>0) err("not support key array");
		vp=bcvar(root,val[0],tlog_void,length);
		mode|=auth_key;
	}
	else if (head==_head_fun)
	{
		if (length>0) err("not support fun array");
		vp=bcvar(root,val[0],tlog_void,length);
		mode|=auth_run;
	}
	else if (head==_head_fvlist)
	{
		if (length>0) err("not support function array");
		vp=bcvar(root,val[0],tlog_vlist,length);
		mode|=auth_run;
	}
	else err("unknow type");
	if (!vp) err("bcvar err");
	vp->mode=mode;
	if ((vp->type&type_znum)&&*(val[1])=='u') vp->type|=type_unsign;
	if ((vp->length>0)&&(!(vp->type&(type_void|type_snum)))) err("not support this array");
	if (vn>4) get_v(vp,exp);
	return vp;
}

char* make_vm(char *exp, var *root)
{
	while(1)
	{
		do exp=get_line(exp); while(vn==0);
		if (*(val[0])=='}') return exp;
		make_vm_one(root,&exp);
	}
}

void put_vp_array(var *vp)
{
	u32 type,length;
	char *value;
	if ((vp->length>0)&&(vp->type&(type_snum|type_void))&&(vp->mode&free_name))
	{
		type=vp->type;
		length=vp->length;
		value=vp->v.v_string;
		if (type&type_void) fprintf(outfile,"static void*	_sv_ar_%p[%u] = %s;\n",vp,length,value);
		else if (type&type_string) fprintf(outfile,"static char*	_sv_ar_%p[%u] = %s;\n",vp,length,value);
		else if (type&type_unsign)
		{
			if (type&type_byte) fprintf(outfile,"static u8	_sv_ar_%p[%u] = %s;\n",vp,length,value);
			else if (type&type_word) fprintf(outfile,"static u16	_sv_ar_%p[%u] = %s;\n",vp,length,value);
			else if (type&type_int) fprintf(outfile,"static u32	_sv_ar_%p[%u] = %s;\n",vp,length,value);
			else if (type&type_long) fprintf(outfile,"static u64	_sv_ar_%p[%u] = %s;\n",vp,length,value);
		}
		else
		{
			if (type&type_byte) fprintf(outfile,"static s8	_sv_ar_%p[%u] = %s;\n",vp,length,value);
			else if (type&type_word) fprintf(outfile,"static s16	_sv_ar_%p[%u] = %s;\n",vp,length,value);
			else if (type&type_int) fprintf(outfile,"static s32	_sv_ar_%p[%u] = %s;\n",vp,length,value);
			else if (type&type_long) fprintf(outfile,"static s64	_sv_ar_%p[%u] = %s;\n",vp,length,value);
		}
	}
}

void put_vp(var *vp)
{
	void put_vl(vlist *vl);
	vmat *vm;
	int i;
	// 对于 inode>1 的变量，通过破坏结构来表示
	if (vp==NULL||vp->inode==0) return ;
	put_vp_array(vp);
	if (vp->type&type_vmat) fprintf(outfile,"static vmat\t_sv_vm_%p;\n",vp->v.v_vmat);
	fprintf(outfile,"static var	_sv_vp_%p = {\n",vp);
	fprintf(outfile,"\t.type\t\t=0x%08x,\n",vp->type);
	fprintf(outfile,"\t.length\t\t=%u,\n",vp->length);
	fprintf(outfile,"\t.inode\t\t=%u,\n",vp->inode);
	fprintf(outfile,"\t.mode\t\t=0x%08x,\n",vp->mode&auth_all);
	switch(vp->type)
	{
		case type_vlist:
			if (vp->v.v_vlist) fprintf(outfile,"\t.v.v_vlist\t=&_sv_vl_%p\n};\n",vp->v.v_vlist);
			else fprintf(outfile,"\t.v.v_vlist\t=NULL\n};\n");
			break;
		case type_vmat:
			fprintf(outfile,"\t.v.v_vmat\t=&_sv_vm_%p\n};\n",vp->v.v_vmat);
			vm=vp->v.v_vmat;
			fprintf(outfile,"static vmat	_sv_vm_%p = {\n",vm);
			for(i=0;i<256;i++)
				if (vm->vl[i]) fprintf(outfile,"\t.vl[%3d]\t=&_sv_vl_%p,\n",i,vm->vl[i]);
			fprintf(outfile,"};\n");
			break;
		default:
			l:
			if ((vp->length>0)&&(vp->type&(type_snum|type_void))&&(vp->mode&free_name))
			{
				fprintf(outfile,"\t.v.v_void\t=&_sv_ar_%p\n};\n",vp);
			}
			else if (vp->mode&free_name)
			{
				switch(vp->type)
				{
					case type_void:
						fprintf(outfile,"\t.v.v_void\t=%s\n};\n",vp->v.v_string);
						break;
					case type_float:
						fprintf(outfile,"\t.v.v_float\t=%s\n};\n",vp->v.v_string);
						break;
					case type_string:
						fprintf(outfile,"\t.v.v_string\t=%s\n};\n",vp->v.v_string);
						break;
					default:
						fprintf(outfile,"\t.v.v_long\t=%s\n};\n",vp->v.v_string);
						break;
				}
			}
			else if (vp->type&type_string) fprintf(outfile,"\t.v.v_string\t=\"%s\"\n};\n",vp->v.v_string);
			else fprintf(outfile,"\t.v.v_long\t=0x%016llx\n};\n",vp->v.v_long);
	}
	vp->inode=0;
	if (vp->type&type_vmat)
	{
		vm=vp->v.v_vmat;
		for(i=0;i<256;i++)
			if (vm->vl[i]) put_vl(vm->vl[i]);
	}
	else if (vp->type&type_vlist) put_vl(vp->v.v_vlist);
}

void put_vl(vlist *vl)
{
	if (vl==NULL) return ;
	while(vl->l) vl=vl->l;
	while(vl)
	{
		fprintf(outfile,"static vlist	_sv_vl_%p = {\n",vl);
		fprintf(outfile,"\t.name\t=\"%s\",\n\t.mode\t=0,\n",vl->name);
		fprintf(outfile,"\t.head\t=0x%016llx,\n",vl->head);
		if (vl->v) fprintf(outfile,"\t.v\t=&_sv_vp_%p,\n",vl->v);
		else fprintf(outfile,"\t.v\t=NULL,\n");
		if (vl->l) fprintf(outfile,"\t.l\t=&_sv_vl_%p,\n",vl->l);
		else fprintf(outfile,"\t.l\t=NULL,\n");
		if (vl->r) fprintf(outfile,"\t.r\t=&_sv_vl_%p\n};\n",vl->r);
		else fprintf(outfile,"\t.r\t=NULL\n};\n");
		put_vp(vl->v);
		vl=vl->r;
	}
}

void init(void)
{
	_head_void	=	vname_head("void");
	_head_sbyte	=	vname_head("sbyte");
	_head_ubyte	=	vname_head("ubyte");
	_head_sword	=	vname_head("sword");
	_head_uword	=	vname_head("uword");
	_head_sint	=	vname_head("sint");
	_head_uint	=	vname_head("uint");
	_head_slong	=	vname_head("slong");
	_head_ulong	=	vname_head("ulong");
	_head_float	=	vname_head("float");
	_head_string	=	vname_head("string");
	_head_vlist	=	vname_head("vlist");
	_head_vmat	=	vname_head("vmat");
	_head_key	=	vname_head("key");
	_head_fun	=	vname_head("fun");
	_head_fvlist	=	vname_head("fvlist");
}

int main(int argc, char *argv[])
{
	int size,i;
	char *exp;
	var *vp;
	
	init();
	
	if (argc<3) fprintf(stderr,"sv-build <infile> <outfile> [--debug]\n");
	infile=fopen(argv[1],"r");
	outfile=fopen(argv[2],"w");
	if (infile==NULL||outfile==NULL) err("open file fail");
	if (argc>3) isdebug=strcmp(argv[3],"--debug")?0:1;
	else isdebug=0;
	// read infile
	fseek(infile,0,2);
	size=ftell(infile);
	exp=malloc(size+1);
	if (!exp) err("malloc fail");
	fseek(infile,0,0);
	for(i=0;i<size;i++)
	{
		exp[i]=getc(infile);
	}
	exp[size]=0;
	fclose(infile);
	infile=NULL;
	// deal exp(infile)
	while(*exp)
	{
		if (*exp=='@')
		{
			exp++;
			exp=get_line(exp);
			vp=make_vm_one(NULL,&exp);
			if (vp->inode==0) vp->inode++;
			put_vp(vp);
		}
		while(*exp&&*exp!='\n')
		{
			putc(*(exp++),outfile);
		}
		if (*exp) putc(*(exp++),outfile);
	}
	
	fclose(outfile);
	return 0;
}


