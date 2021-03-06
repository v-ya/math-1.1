#include "main.h"

char* get_path(char *path_list, char *path_short)
{
	int sz_l,sz_s,p;
	char *ret,cu,icu;
	sz_s=strlen(path_short)+1;
	p=0;
	cu=_path_cutup->v.v_byte;
	icu=_path_incutup->v.v_byte;
	while(path_list[p])
	{
		sz_l=p;
		while(path_list[sz_l]&&path_list[sz_l]!=cu) sz_l++;
		sz_l-=p;
		ret=malloc(sz_l+sz_s+1);
		if _oF(!ret) return NULL;
		memcpy(ret,path_list+p,sz_l);
		ret[sz_l]=icu;
		memcpy(ret+sz_l+1,path_short,sz_s);
		if _oT(access(ret,F_OK|R_OK)==0) return ret;
		free(ret);
		p+=sz_l;
		if _oT(path_list[p]==cu) p++;
	}
	return NULL;
}

char* load_string(char *path)
{
	static char *label="load_string";
	FILE *fp;
	u64 size;
	char *str;
	if _oF(!path) return NULL;
	fp=fopen(path,"rb");
	if _oF(!fp) return NULL;
	// 获取文件大小
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	if _oF(size>=_lim_ftos_size->v.v_long)
	{
		fclose(fp);
		get_error(errid_SysFileStringSize,label);
		return NULL;
	}
	fseek(fp,0,SEEK_SET);
	str=malloc(size+1);
	if _oF(!str)
	{
		fclose(fp);
		get_error(errid_MemLess,label);
		return NULL;
	}
	if _oF(fread(str,1,size,fp)<size)
	{
		fclose(fp);
		free(str);
		return NULL;
	}
	str[size]=0;
	fclose(fp);
	return str;
}

static u32 data_magic = 0xda0da0da;
	// data 文件魔数
/* file struct
u32 data magic
u32 refer number
+ u32 refer[]
info var struct
*/

/* var struct
name
-----
u+ len
u8 s[len]
=====
index
-----
u64 index
=====
void
-----
u8 tlog
info name
=====
byte word int long float ubyte uword uint ulong
-----
u8 tlog
info name
u* value
=====
string
-----
u8 tlog
info name
info value
	u+ len+1
	u8 s[len]
=====
array
-----
u8 tlog
info name
u32 length
* value[]
=====
vlist vmat
-----
u8 tlog
info name
info var struct[]
u8 tlog_data_max
*/

typedef struct _load_ds_S {
	u32 i;
	vmat *vm;
	vlist *rl;
} _load_ds;

#define data_tlog_name		0x10
#define data_tlog_array		0x20
#define data_tlog_unsign	0x40
static u32 f_max;
static u32 z_max;

var* store_uplus(FILE *fp, u32 u, u32 *psize)
{
	static char *label="store_uplus";
	if _oF(u&0xf0000000) return get_error(errid_StrLength,label);
	do
	{
		if _oF(fputc(u&0x7f|((u>>7)?0x80:0),fp)<0) return get_error(errid_SysFileErrorWrite,label);
		u>>=7;
		(*psize)++;
	} while(u);
	if _oF(*psize>f_max) return get_error(errid_FileDataSize,label);
	return NULL;
}

var* load_uplus(FILE *fp, u32 *u, u32 *psize)
{
	static char *label="load_uplus";
	s32 c;
	u32 r;
	c=fgetc(fp);
	if _oF(c<0) return get_error(errid_SysFileErrorRead,label);
	(*psize)++;
	r=c&0x7f;
	if _oF(c&0x80)
	{
		c=fgetc(fp);
		if _oF(c<0) return get_error(errid_SysFileErrorRead,label);
		(*psize)++;
		r|=(c&0x7f)<<7;
		if _oF(c&0x80)
		{
			c=fgetc(fp);
			if _oF(c<0) return get_error(errid_SysFileErrorRead,label);
			(*psize)++;
			r|=(c&0x7f)<<14;
			if _oF(c&0x80)
			{
				c=fgetc(fp);
				if _oF(c<0) return get_error(errid_SysFileErrorRead,label);
				(*psize)++;
				r|=(c&0x7f)<<21;
			}
		}
	}
	*u=r;
	if _oF(*psize>f_max) return get_error(errid_FileDataSize,label);
	return NULL;
}

var* store_head(FILE *fp, vlist *vl, u32 tlog, u32 *psize)
{
	static char *label="store_head";
	var *vp,*err;
	vp=vl->v;
	if _oF(vl->name) tlog|=data_tlog_name;
	if _oF(vp->length) tlog|=data_tlog_array;
	if _oF(vp->type&type_unsign) tlog|=data_tlog_unsign;
	if _oF(fputc(tlog,fp)<0) return get_error(errid_SysFileErrorWrite,label);
	(*psize)++;
	if _oF(vl->mode&free_temp) ;
	else if _oF(vl->name)
	{
		tlog=strlen(vl->name);
		if _oF(err=store_uplus(fp,tlog,psize)) return err;
		if _oF(fwrite(vl->name,1,tlog,fp)!=tlog) return get_error(errid_SysFileErrorWrite,label);
		(*psize)+=tlog;
	}
	else
	{
		if _oF(fwrite(&(vl->head),sizeof(u64),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
		(*psize)+=sizeof(u64);
	}
	if _oF(vp->length)
	{
		if _oF(fwrite(&(vp->length),sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
		(*psize)+=sizeof(u32);
	}
	if _oF(*psize>f_max) return get_error(errid_FileDataSize,label);
	return NULL;
}

var* load_head(FILE *fp, vlist *vl, u32 *psize)
{
	static char *label="store_head";
	var *vp,*err;
	s32 c;
	u32 tlog,length,i;
	
	c=fgetc(fp);
	if _oF(c<0) return get_error(errid_SysFileErrorRead,label);
	(*psize)++;
	if _oF((tlog=c&0xf)>=tlog_data_max) return get_error(errid_FileDataError,label);
	if _oF(vl->mode&free_temp && vl->v)
	{
		// z == 0
		vp=vl->v;
		if _oF(c&data_tlog_array)
		{
			if _oF(fread(&length,sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u32);
		}
		else length=0;
		if _oF(length && (tlog==tlog_vlist || tlog==tlog_vmat)) return get_error(errid_FileDataError,label);
		if _oF((1<<tlog|((c&data_tlog_unsign)?type_unsign:0))!=vp->type)
		{
			L_retype:
			if _oF(!(vp->mode&auth_retype)) return get_error(errid_VarNotRetype,label);
			return get_error(errid_GraVarType,label);
		}
		else if _oF(length!=vp->length)
		{
			if _oF(!length || !vp->length) goto L_retype;
			if _oF(!(vp->mode&auth_relength)) return get_error(errid_VarNotRelength,label);
			return get_error(errid_GraVarType,label);
		}
		if _oF(tlog==tlog_string)
		{
			// clear string, string[]
			if _oF(length)
			{
				if _oT(vp->mode&free_pointer)
				{
					while(length)
					{
						length--;
						if _oF(vp->v.vp_string[length])
						{
							free(vp->v.vp_string[length]);
							vp->v.vp_string[length]=NULL;
						}
					}
				}
				else
				{
					err=malloc(length*sizeof(char*));
					if _oF(!err) return get_error(errid_MemLess,label);
					vp->v.vp_string=(char**)err;
					vp->mode|=free_pointer;
					while(length)
					{
						length--;
						vp->v.vp_string[length]=NULL;
					}
				}
			}
			else
			{
				if _oF(vp->v.v_string)
				{
					if _oT(vp->mode&free_pointer) free(vp->v.v_string);
					vp->v.v_string=NULL;
				}
				vp->mode&=~free_pointer;
			}
		}
	}
	else
	{
		if _oF(vl->mode&free_temp) ;
		else if _oF(c&data_tlog_name)
		{
			if _oF(err=load_uplus(fp,&i,psize)) return err;
			vl->name=malloc(i+1);
			if _oF(!vl->name) return get_error(errid_MemLess,label);
			vl->mode|=free_name;
			vl->name[i]=0;
			if _oF(fread(vl->name,1,i,fp)!=i)
			{
				free(vl->name);
				vl->name=NULL;
				vl->mode&=~free_name;
				return get_error(errid_SysFileErrorRead,label);
			}
			vl->head=vname_head(vl->name);
			(*psize)+=i;
		}
		else
		{
			vl->name=NULL;
			if _oF(fread(&(vl->head),sizeof(u64),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u64);
		}
		if _oF(c&data_tlog_array)
		{
			if _oF(fread(&length,sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u32);
		}
		else length=0;
		if _oF(length>_lim_array_max->v.v_long) return get_error(errid_ReqOver,label);
		if _oF(length && (tlog==tlog_vlist || tlog==tlog_vmat)) return get_error(errid_FileDataError,label);
		vl->v=var_alloc(tlog,length);
		if _oF(!vl->v) return get_error(errid_MemLess,label);
		vl->mode|=free_pointer;
		var_save(vl->v);
		if _oF(c&data_tlog_unsign && vl->v->type&type_znum) vl->v->type|=type_unsign;
	}
	if _oF(*psize>f_max) return get_error(errid_FileDataSize,label);
	return NULL;
}

var* store_var(FILE *fp, vlist *vl, u32 *psize, u32 z, vmat *pvm)
{
	static char *label="store_var";
	static u32 norefer=0xffffffff;
	var *err,*vp;
	vmat *vm;
	u32 i;
	
	vp=vl->v;
	if _oF(z>z_max || !vp) return NULL;
	if _oF((vp->mode&auth_all)!=auth_normal) return NULL;
	if _oT(!vp->length) switch(vp->type&type_all)
	{
		// not array
		case type_void:
			if _oF(err=store_head(fp,vl,tlog_void,psize)) return err;
			break;
		case type_byte:
			if _oF(err=store_head(fp,vl,tlog_byte,psize)) return err;
			if _oF(fwrite(&(vp->v.v_byte),sizeof(u8),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(u8);
			break;
		case type_word:
			if _oF(err=store_head(fp,vl,tlog_word,psize)) return err;
			if _oF(fwrite(&(vp->v.v_word),sizeof(u16),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(u16);
			break;
		case type_int:
			if _oF(err=store_head(fp,vl,tlog_int,psize)) return err;
			if _oF(fwrite(&(vp->v.v_int),sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(u32);
			break;
		case type_long:
			if _oF(err=store_head(fp,vl,tlog_long,psize)) return err;
			if _oF(fwrite(&(vp->v.v_long),sizeof(u64),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(u64);
			break;
		case type_float:
			if _oF(err=store_head(fp,vl,tlog_float,psize)) return err;
			if _oF(fwrite(&(vp->v.v_float),sizeof(double),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(double);
			break;
		case type_string:
			if _oF(err=store_head(fp,vl,tlog_string,psize)) return err;
			if _oT(vp->v.v_string)
			{
				i=strlen(vp->v.v_string);
				if _oF(err=store_uplus(fp,i+1,psize)) return err;
				if _oT(i)
				{
					if _oF(fwrite(vp->v.v_string,1,i,fp)!=i) return get_error(errid_SysFileErrorWrite,label);
					(*psize)+=i;
				}
			}
			else
			{
				if _oF(fputc(0,fp)<0) return get_error(errid_SysFileErrorWrite,label);
				(*psize)++;
			}
			break;
		case type_vlist:
			if _oF(err=store_head(fp,vl,tlog_vlist,psize)) return err;
			vl=vp->v.v_vlist;
			if _oT(vl)
			{
				z++;
				while(vl->l) vl=vl->l;
				while(vl)
				{
					if _oF(err=store_var(fp,vl,psize,z,pvm)) return err;
					vl=vl->r;
				}
			}
			if _oF(fputc(tlog_data_max,fp)<0) return get_error(errid_SysFileErrorWrite,label);
			(*psize)++;
			break;
		case type_vmat:
			if _oF(err=store_head(fp,vl,tlog_vmat,psize)) return err;
			vm=vp->v.v_vmat;
			z++;
			if _oT(vm && vm->avl) for(i=0;i<=vm->mask;i++)
			{
				if _oT(vl=vm->avl[i])
				{
					while(vl->l) vl=vl->l;
					while(vl)
					{
						if _oF(err=store_var(fp,vl,psize,z,pvm)) return err;
						vl=vl->r;
					}
				}
			}
			if _oF(fputc(tlog_data_max,fp)<0) return get_error(errid_SysFileErrorWrite,label);
			(*psize)++;
			break;
		case type_refer:
			if _oT(pvm)
			{
				if _oF(err=store_head(fp,vl,tlog_refer,psize)) return err;
				if _oT(vl=vmat_find_index(pvm,(u64)vp))
				{
					if _oF(fwrite(&(vl->mode),sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
				}
				else
				{
					if _oF(fwrite(&norefer,sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorWrite,label);
				}
				(*psize)+=sizeof(u32);
				break;
			}
		default:
			return NULL;
	}
	else switch(vp->type&type_all)
	{
		// array
		case type_void:
			if _oF(err=store_head(fp,vl,tlog_void,psize)) return err;
			break;
		case type_byte:
			if _oF(err=store_head(fp,vl,tlog_byte,psize)) return err;
			if _oF(fwrite(vp->v.vp_byte,sizeof(u8),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(u8)*vp->length;
			break;
		case type_word:
			if _oF(err=store_head(fp,vl,tlog_word,psize)) return err;
			if _oF(fwrite(vp->v.vp_word,sizeof(u16),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(u16)*vp->length;
			break;
		case type_int:
			if _oF(err=store_head(fp,vl,tlog_int,psize)) return err;
			if _oF(fwrite(vp->v.vp_int,sizeof(u32),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(u32)*vp->length;
			break;
		case type_long:
			if _oF(err=store_head(fp,vl,tlog_long,psize)) return err;
			if _oF(fwrite(vp->v.vp_long,sizeof(u64),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(u64)*vp->length;
			break;
		case type_float:
			if _oF(err=store_head(fp,vl,tlog_float,psize)) return err;
			if _oF(fwrite(vp->v.vp_float,sizeof(double),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorWrite,label);
			(*psize)+=sizeof(double)*vp->length;
			break;
		case type_string:
			if _oF(err=store_head(fp,vl,tlog_string,psize)) return err;
			if _oT(vp->v.vp_string) for(z=0;z<vp->length;z++)
			{
				if _oT(vp->v.vp_string[z])
				{
					i=strlen(vp->v.vp_string[z]);
					if _oF(err=store_uplus(fp,i+1,psize)) return err;
					if _oT(i)
					{
						if _oF(fwrite(vp->v.vp_string[z],1,i,fp)!=i) return get_error(errid_SysFileErrorWrite,label);
						(*psize)+=i;
					}
				}
				else
				{
					if _oF(fputc(0,fp)<0) return get_error(errid_SysFileErrorWrite,label);
					(*psize)++;
				}
			}
			break;
		default:
			return NULL;
	}
	if _oF(*psize>f_max) return get_error(errid_FileDataSize,label);
	return NULL;
}

var* load_var(FILE *fp, vlist *vl, u32 *psize, u32 z, _load_ds *ds)
{
	static char *label="load_var";
	var *vp,*err;
	s32 c;
	u32 i;
	if _oF(z>z_max) return get_error(errid_FileDataZmax,label);
	if _oF(err=load_head(fp,vl,psize)) return err;
	if _oF(vp=(var*)vmat_find_index(ds->vm,ds->i)) ((vlist*)vp)->v=vl->v;
	(ds->i)++;
	vp=vl->v;
	if _oT(!vp->length) switch(vp->type&type_all)
	{
		// not array
		case type_byte:
			if _oF(fread(&(vp->v.v_byte),sizeof(u8),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			var_fixvalue(vp);
			(*psize)+=sizeof(u8);
			break;
		case type_word:
			if _oF(fread(&(vp->v.v_word),sizeof(u16),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			var_fixvalue(vp);
			(*psize)+=sizeof(u16);
			break;
		case type_int:
			if _oF(fread(&(vp->v.v_int),sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			var_fixvalue(vp);
			(*psize)+=sizeof(u32);
			break;
		case type_long:
			if _oF(fread(&(vp->v.v_long),sizeof(u64),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u64);
			break;
		case type_float:
			if _oF(fread(&(vp->v.v_float),sizeof(double),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(double);
			break;
		case type_string:
			if _oF(err=load_uplus(fp,&i,psize)) return err;
			if _oT(i)
			{
				vp->v.v_string=malloc(i);
				if _oF(!vp->v.v_string) return get_error(errid_MemLess,label);
				vp->mode|=free_pointer;
				i-=1;
				vp->v.v_string[i]=0;
				if _oT(i)
				{
					if _oF(fread(vp->v.v_string,1,i,fp)!=i) return get_error(errid_SysFileErrorRead,label);
					(*psize)+=i;
				}
			}
			break;
		case type_vlist:
			z++;
			while(1)
			{
				c=fgetc(fp);
				if _oF(c<0) return get_error(errid_SysFileErrorRead,label);
				if _oF(c==tlog_data_max) break;
				if _oF(ungetc(c,fp)<0) return get_error(errid_SysFileErrorRead,label);
				vl=vlist_alloc(NULL);
				if _oF(!vl) return get_error(errid_MemLess,label);
				if _oF(err=load_var(fp,vl,psize,z,ds)) return err;
				if _oF(vl->name)
				{
					if _oF(vlist_find(vp->v.v_vlist,vl->name))
					{
						vlist_free(vl);
						return get_error(errid_VarIsExist,label);
					}
					else vp->v.v_vlist=vlist_insert(vp->v.v_vlist,vl);
				}
				else
				{
					if _oF(vlist_find_index(vp->v.v_vlist,vl->head))
					{
						vlist_free(vl);
						return get_error(errid_VarIsExist,label);
					}
					else vp->v.v_vlist=vlist_insert(vp->v.v_vlist,vl);
				}
			}
			(*psize)++;
			break;
		case type_vmat:
			z++;
			while(1)
			{
				c=fgetc(fp);
				if _oF(c<0) return get_error(errid_SysFileErrorRead,label);
				if _oF(c==tlog_data_max) break;
				if _oF(ungetc(c,fp)<0) return get_error(errid_SysFileErrorRead,label);
				vl=vlist_alloc(NULL);
				if _oF(!vl) return get_error(errid_MemLess,label);
				if _oF(err=load_var(fp,vl,psize,z,ds)) return err;
				if _oF(vl->name)
				{
					if _oF(vmat_find(vp->v.v_vmat,vl->name))
					{
						vlist_free(vl);
						return get_error(errid_VarIsExist,label);
					}
					else vmat_insert(vp->v.v_vmat,vl);
				}
				else
				{
					if _oF(vmat_find_index(vp->v.v_vmat,vl->head))
					{
						vlist_free(vl);
						return get_error(errid_VarIsExist,label);
					}
					else vmat_insert(vp->v.v_vmat,vl);
				}
			}
			(*psize)++;
			break;
		case type_refer:
			vl=vlist_alloc_index((u64)vp);
			if _oF(!vl) return get_error(errid_MemLess,label);
			if _oF(fread(&(vl->mode),sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u32);
			if _oT(ds->rl)
			{
				ds->rl->r=vl;
				vl->l=ds->rl;
			}
			ds->rl=vl;
			break;
	}
	else switch(vp->type&type_all)
	{
		// array
		case type_byte:
			if _oF(fread(vp->v.vp_byte,sizeof(u8),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u8)*vp->length;
			break;
		case type_word:
			if _oF(fread(vp->v.vp_word,sizeof(u16),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u16)*vp->length;
			break;
		case type_int:
			if _oF(fread(vp->v.vp_int,sizeof(u32),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u32)*vp->length;
			break;
		case type_long:
			if _oF(fread(vp->v.vp_long,sizeof(u64),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(u64)*vp->length;
			break;
		case type_float:
			if _oF(fread(vp->v.vp_float,sizeof(double),vp->length,fp)!=vp->length) return get_error(errid_SysFileErrorRead,label);
			(*psize)+=sizeof(double)*vp->length;
			break;
		case type_string:
			if _oT(vp->v.vp_string) for(z=0;z<vp->length;z++)
			{
				if _oF(err=load_uplus(fp,&i,psize)) return err;
				if _oT(i)
				{
					vp->v.vp_string[z]=malloc(i);
					if _oF(!vp->v.v_string[z]) return get_error(errid_MemLess,label);
					i-=1;
					vp->v.vp_string[z][i]=0;
					if _oT(i)
					{
						if _oF(fread(vp->v.vp_string[z],1,i,fp)!=i) return get_error(errid_SysFileErrorRead,label);
						(*psize)+=i;
					}
				}
				else vp->v.vp_string[z]=NULL;
			}
			break;
		default:
			return NULL;
	}
	if _oF(*psize>f_max) return get_error(errid_FileDataSize,label);
	return NULL;
}

var* store_get_refer(var *vp, vlist **pvl, vmat *pvm, u32 z)
{
	static char *label="store_get_refer";
	var *err;
	vlist *vl;
	vmat *vm;
	u32 i;
	
	if _oF(z>z_max || !vp) return NULL;
	if _oF((vp->mode&auth_all)!=auth_normal) return NULL;
	if _oF(vmat_find_index(pvm,(u64)vp)) return get_error(errid_FileDataHardlink,label);
	vl=vlist_alloc_index((u64)vp);
	if _oF(!vl) return get_error(errid_MemLess,label);
	vl->mode=pvm->number;
	vmat_insert(pvm,vl);
	if _oT(!vp->length) switch(vp->type&type_all)
	{
		case type_vlist:
			vl=vp->v.v_vlist;
			if _oT(vl)
			{
				z++;
				while(vl->l) vl=vl->l;
				while(vl)
				{
					if _oF(err=store_get_refer(vl->v,pvl,pvm,z)) return err;
					vl=vl->r;
				}
			}
			break;
		case type_vmat:
			vm=vp->v.v_vmat;
			z++;
			if _oT(vm && vm->avl) for(i=0;i<=vm->mask;i++)
			{
				if _oT(vl=vm->avl[i])
				{
					while(vl->l) vl=vl->l;
					while(vl)
					{
						if _oF(err=store_get_refer(vl->v,pvl,pvm,z)) return err;
						vl=vl->r;
					}
				}
			}
			break;
		case type_refer:
			vl=vlist_alloc_index((u64)vp);
			if _oF(!vl) return get_error(errid_MemLess,label);
			vl->v=refer_check(vp);
			if _oT(vl->v)
			{
				if _oT(*pvl)
				{
					(*pvl)->r=vl;
					vl->l=*pvl;
				}
				*pvl=vl;
			}
			else free(vl);
			break;
	}
	return NULL;
}

var* store_refer(FILE *fp, u32 *n, vlist *pvl, vmat *pvm)
{
	static char *label="store_refer";
	vlist *vl;
	vmat *rvm;
	var *err;
	rvm=vmat_alloc();
	if _oF(!rvm)
	{
		err=get_error(errid_MemLess,label);
		goto Err;
	}
	if _oF(fwrite(n,sizeof(u32),1,fp)!=1)
	{
		err=get_error(errid_SysFileErrorWrite,label);
		goto Err;
	}
	while(pvl)
	{
		vl=pvl->r;
		if _oT(pvl->v)
		{
			if _oT(!vmat_find_index(rvm,pvl->mode))
			{
				if _oF(fwrite(&(pvl->mode),sizeof(u32),1,fp)!=1)
				{
					err=get_error(errid_SysFileErrorWrite,label);
					goto Err;
				}
				pvl->v=(var*)vlist_alloc_index(pvl->mode);
				if _oF(!pvl->v)
				{
					err=get_error(errid_MemLess,label);
					goto Err;
				}
				vmat_insert(rvm,(vlist*)pvl->v);
			}
			pvl->l=NULL;
			pvl->r=NULL;
			pvl->v=NULL;
			vmat_insert(pvm,pvl);
		}
		else free(pvl);
		pvl=vl;
	}
	if _oF(*n!=rvm->number)
	{
		fseek(fp,sizeof(u32),SEEK_SET);
		if _oF(fwrite(&(rvm->number),sizeof(u32),1,fp)!=1)
		{
			err=get_error(errid_SysFileErrorWrite,label);
			goto Err;
		}
		fseek(fp,sizeof(u32)*(2+rvm->number),SEEK_SET);
		*n=rvm->number;
	}
	vmat_free(rvm);
	return NULL;
	Err:
	if _oT(rvm) vmat_free(rvm);
	while(pvl)
	{
		vl=pvl->r;
		free(pvl);
		pvl=vl;
	}
	return err;
}

var* load_refer(FILE *fp, vmat *pvm, u32 *pn)
{
	static char *label="load_refer";
	u32 n,r;
	vlist *vl;
	var *err;
	if _oF(fread(&n,sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
	*pn=n;
	while(n)
	{
		if _oF(fread(&r,sizeof(u32),1,fp)!=1) return get_error(errid_SysFileErrorRead,label);
		if _oF(!vmat_find_index(pvm,r))
		{
			vl=vlist_alloc_index(r);
			vmat_insert(pvm,vl);
		}
		n--;
	}
	return NULL;
}

var* store_data(char *path, var *obj, u32 isfast)
{
	static char *label="store_data";
	u32 f;
	vlist vl={0},*pvl,*tvl;
	vmat *pvm;
	FILE *fp;
	if _oF(!obj) return NULL;
	if _oF((obj->mode&auth_all)!=auth_normal)
	{
		remove(path);
		return NULL;
	}
	f_max=_lim_data_fmax->v.v_long;
	z_max=_lim_data_zmax->v.v_long;
	if _oT(path) fp=fopen(path,"wb");
	else fp=NULL;
	if _oF(!fp)
	{
		obj=get_error(errid_SysFileNotLoad,label);
		goto Err;
	}
	vl.v=obj;
	vl.mode=free_temp;
	if _oF(isfast)
	{
		// write magic
		if _oF(fwrite(&data_magic,sizeof(u32),1,fp)!=1)
		{
			obj=get_error(errid_SysFileErrorWrite,label);
			goto Err;
		}
		// write refer number
		f=0;
		if _oF(fwrite(&f,sizeof(u32),1,fp)!=1)
		{
			obj=get_error(errid_SysFileErrorWrite,label);
			goto Err;
		}
		f=sizeof(u32)*2;
		// store var
		obj=store_var(fp,&vl,&f,0,NULL);
	}
	else
	{
		// check & get refer
		pvm=vmat_alloc();
		if _oF(!pvm)
		{
			obj=get_error(errid_MemLess,label);
			goto Err;
		}
		pvl=NULL;
		if _oF(obj=store_get_refer(vl.v,&pvl,pvm,0))
		{
			vmat_free(pvm);
			goto Err_pvl;
		}
		f=0;
		while(pvl)
		{
			if _oT(pvl->v && (tvl=vmat_find_index(pvm,(u64)(pvl->v))))
			{
				pvl->mode=tvl->mode;
				f++;
			}
			else pvl->v=NULL;
			if _oF(!pvl->l) break;
			pvl=pvl->l;
		}
		vmat_free(pvm);
		// write magic
		if _oF(fwrite(&data_magic,sizeof(u32),1,fp)!=1)
		{
			obj=get_error(errid_SysFileErrorWrite,label);
			goto Err_pvl;
		}
		// write refer
		pvm=vmat_alloc();
		if _oF(!pvm)
		{
			obj=get_error(errid_MemLess,label);
			goto Err_pvl;
		}
		if _oF(obj=store_refer(fp,&f,pvl,pvm)) goto Err;
		f=sizeof(u32)*(2+f);
		// store var
		obj=store_var(fp,&vl,&f,0,pvm);
		vmat_free(pvm);
	}
	fclose(fp);
	if _oF(obj) remove(path);
	return obj;
	Err_pvl:
	while(pvl)
	{
		tvl=pvl->r;
		free(pvl);
		pvl=tvl;
	}
	Err:
	if _oT(fp)
	{
		fclose(fp);
		remove(path);
	}
	return obj;
}

var* load_data(var *obj, char *path)
{
	static char *label="load_data";
	u32 f,magic;
	vlist vl={0},*pvl,*tvl;
	FILE *fp;
	_load_ds ds={0};
	if _oF(!obj) return NULL;
	f_max=_lim_data_fmax->v.v_long;
	z_max=_lim_data_zmax->v.v_long;
	if _oT(path) fp=fopen(path,"rb");
	else fp=NULL;
	if _oF(!fp)
	{
		obj=get_error(errid_SysFileNotLoad,label);
		goto Err;
	}
	vl.v=obj;
	vl.mode=free_temp;
	// read magic
	if _oF(fread(&magic,sizeof(u32),1,fp)!=1)
	{
		obj=get_error(errid_SysFileErrorRead,label);
		goto Err;
	}
	if _oF(magic!=data_magic)
	{
		obj=get_error(errid_FileDataError,label);
		goto Err;
	}
	// read refer
	ds.vm=vmat_alloc();
	if _oF(!ds.vm)
	{
		obj=get_error(errid_MemLess,label);
		goto Err;
	}
	if _oF(obj=load_refer(fp,ds.vm,&f))
	{
		vmat_free(ds.vm);
		goto Err;
	}
	f=sizeof(u32)*(2+f);
	// load var
	obj=load_var(fp,&vl,&f,0,&ds);
	// set refer
	if _oF(ds.rl)
	{
		while(ds.rl)
		{
			pvl=ds.rl;
			ds.rl=pvl->l;
			if _oT(tvl=vmat_find_index(ds.vm,pvl->mode)) refer_set((var*)(pvl->head),tvl->v);
			free(pvl);
		}
	}
	vmat_free(ds.vm);
	fclose(fp);
	return obj;
	Err:
	if _oT(fp) fclose(fp);
	return obj;
}


