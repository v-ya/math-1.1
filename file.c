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
	ret=malloc(sz_s);
	if _oF(!ret) return NULL;
	memcpy(ret,path_short,sz_s);
	return ret;
}


