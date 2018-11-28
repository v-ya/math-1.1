#include "../main.h"

func(file_size)
{
	static char *label=".file.size";
	static u32 type_1[1]={type_string};
	char *path;
	FILE *fp;
	u64 size;
	if _oF(argc!=1) return get_error(errid_FunArgvType,label);
	else if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
	path=argv->v->v.v_string;
	if _oF(!path) return get_error(errid_SysFileNotLoad,label);
	fp=fopen(path,"rb");
	if _oF(!fp) return get_error(errid_SysFileNotLoad,label);
	// 获取文件大小
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	fclose(fp);
	ret->type=type_long|type_unsign;
	ret->v.v_long=size;
	return ret;
}

/*
	read("path",array);
	read("path",array,file-at);
	read("path",array,file-at,count);
	read("path",array,file-at,array-at,count);
*/
func(file_read)
{
	static char *label=".file.read";
	static u32 type_2[2]={type_string,type_znum|type_onlyarray};
	static u32 type_3[3]={type_string,type_znum|type_onlyarray,type_znum};
	static u32 type_4[4]={type_string,type_znum|type_onlyarray,type_znum,type_znum};
	static u32 type_5[5]={type_string,type_znum|type_onlyarray,type_znum,type_znum,type_znum};
	char *path;
	var *vp;
	FILE *fp;
	u64 file_at,array_at,count;
	
	switch(argc)
	{
		case 2:
			if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			file_at=0;
			array_at=0;
			count=vp->length;
			break;
		case 3:
			if _oF(check_varlist(argv,3,type_3)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			file_at=argv->v->v.v_long;
			array_at=0;
			count=vp->length;
			break;
		case 4:
			if _oF(check_varlist(argv,4,type_4)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			file_at=argv->v->v.v_long;
			argv=argv->r;
			array_at=0;
			count=argv->v->v.v_long;
			break;
		case 5:
			if _oF(check_varlist(argv,5,type_5)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			file_at=argv->v->v.v_long;
			argv=argv->r;
			array_at=argv->v->v.v_long;
			argv=argv->r;
			count=argv->v->v.v_long;
			break;
		default:
			return get_error(errid_FunArgvType,label);
			break;
	}
	ret->type=type_long|type_unsign;
	if _oF(array_at>=vp->length)
	{
		ret->v.v_long=0;
		return ret;
	}
	if _oF((vp->length-array_at)<count) count=vp->length-array_at;
	// open file
	if _oF(!path) return get_error(errid_SysFileNotLoad,label);
	fp=fopen(path,"rb");
	if _oF(!fp) return get_error(errid_SysFileNotLoad,label);
	fseek(fp,file_at,SEEK_SET);
	// read
	switch(vp->type&type_all)
	{
		case type_byte:
			count=fread(vp->v.vp_byte+array_at,sizeof(u8),count,fp);
			break;
		case type_word:
			count=fread(vp->v.vp_word+array_at,sizeof(u16),count,fp);
			break;
		case type_int:
			count=fread(vp->v.vp_int+array_at,sizeof(u32),count,fp);
			break;
		case type_long:
			count=fread(vp->v.vp_long+array_at,sizeof(u64),count,fp);
			break;
		default:
			fclose(fp);
			return get_error(errid_VarUnknowType,label);
	}
	fclose(fp);
	ret->v.v_long=count;
	return ret;
}

/*
	write("path",array);
	write("path",array,isadd);
	write("path",array,isadd,file-at);
	write("path",array,isadd,file-at,count);
	write("path",array,isadd,file-at,array-at,count);
*/
func(file_write)
{
	static char *label=".file.write";
	static u32 type_2[2]={type_string,type_znum|type_onlyarray};
	static u32 type_3[3]={type_string,type_znum|type_onlyarray,type_znum};
	static u32 type_4[4]={type_string,type_znum|type_onlyarray,type_znum,type_znum};
	static u32 type_5[5]={type_string,type_znum|type_onlyarray,type_znum,type_znum,type_znum};
	static u32 type_6[6]={type_string,type_znum|type_onlyarray,type_znum,type_znum,type_znum,type_znum};
	char *path;
	var *vp;
	FILE *fp;
	u32 isadd;
	u64 file_at,array_at,count;
	
	switch(argc)
	{
		case 2:
			if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			isadd=0;
			file_at=0;
			array_at=0;
			count=vp->length;
			break;
		case 3:
			if _oF(check_varlist(argv,3,type_3)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			isadd=argv->v->v.v_long?1:0;
			file_at=0;
			array_at=0;
			count=vp->length;
			break;
		case 4:
			if _oF(check_varlist(argv,3,type_3)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			isadd=argv->v->v.v_long?1:0;
			argv=argv->r;
			file_at=argv->v->v.v_long;
			array_at=0;
			count=vp->length;
			break;
		case 5:
			if _oF(check_varlist(argv,4,type_4)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			isadd=argv->v->v.v_long?1:0;
			argv=argv->r;
			file_at=argv->v->v.v_long;
			argv=argv->r;
			array_at=0;
			count=argv->v->v.v_long;
			break;
		case 6:
			if _oF(check_varlist(argv,5,type_5)) return get_error(errid_FunArgvType,label);
			path=argv->v->v.v_string;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			isadd=argv->v->v.v_long?1:0;
			argv=argv->r;
			file_at=argv->v->v.v_long;
			argv=argv->r;
			array_at=argv->v->v.v_long;
			argv=argv->r;
			count=argv->v->v.v_long;
			break;
		default:
			return get_error(errid_FunArgvType,label);
			break;
	}
	ret->type=type_long|type_unsign;
	if _oF(array_at>=vp->length)
	{
		ret->v.v_long=0;
		return ret;
	}
	if _oF((vp->length-array_at)<count) count=vp->length-array_at;
	// open file
	if _oF(!path) return get_error(errid_SysFileNotLoad,label);
	if _oF(isadd) fp=fopen(path,"rb+");
	else fp=fopen(path,"wb");
	if _oF(!fp) return get_error(errid_SysFileNotLoad,label);
	fseek(fp,file_at,SEEK_SET);
	// write
	switch(vp->type&type_all)
	{
		case type_byte:
			count=fwrite(vp->v.vp_byte+array_at,sizeof(u8),count,fp);
			break;
		case type_word:
			count=fwrite(vp->v.vp_word+array_at,sizeof(u16),count,fp);
			break;
		case type_int:
			count=fwrite(vp->v.vp_int+array_at,sizeof(u32),count,fp);
			break;
		case type_long:
			count=fwrite(vp->v.vp_long+array_at,sizeof(u64),count,fp);
			break;
		default:
			fclose(fp);
			return get_error(errid_VarUnknowType,label);
	}
	fclose(fp);
	ret->v.v_long=count;
	return ret;
}


