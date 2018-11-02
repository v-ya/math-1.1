#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../type.h"
#include "../string.h"

char* load_string(char *path)
{
	FILE *fp;
	u64 size;
	char *str;
	if (!path) return NULL;
	fp=fopen(path,"rb");
	if (!fp) return NULL;
	// 获取文件大小
	fseek(fp,0,SEEK_END);
	size=ftell(fp);
	fseek(fp,0,SEEK_SET);
	str=malloc(size+1);
	if (!str)
	{
		fclose(fp);
		return NULL;
	}
	if (fread(str,1,size,fp)<size)
	{
		fclose(fp);
		free(str);
		return NULL;
	}
	str[size]=0;
	fclose(fp);
	return str;
}

// init-makec <init.math-path> <to-c-file> <c-var-name>
int main(int argc, char *argv[])
{
	unsigned char *code,*cpcode;
	int size,i;
	FILE *fp;
	if (argc<4) printf("%s <init.math-path> <to-c-file> <c-var-name>\n",argv[0]);
	code=load_string(argv[1]);
	if (!code)
	{
		printf("Error: Not Load %s!\n",argv[1]);
		return -1;
	}
	cpcode=compress_code(code);
	free(code);
	if (!cpcode)
	{
		printf("Error: Not Compress Code!\n");
		return -1;
	}
	fp=fopen(argv[2],"w");
	if (!fp)
	{
		printf("Error: Not Open %s!\n",argv[2]);
		free(cpcode);
		return -1;
	}
	size=strlen(cpcode)+1;
	fprintf(fp,"unsigned char %s_st[%d]={%d",argv[3],size,cpcode[0]);
	for(i=1;i<size;i++)
		fprintf(fp,",%d",cpcode[i]);
	fprintf(fp,"};\nvar %s_vst={.type=type_string,.inode=1,.mode=free_temp|auth_read,.length=0,.v.v_string=%s_st};\n",argv[3],argv[3]);
	fprintf(fp,"var *%s=&%s_vst;\n",argv[3],argv[3]);
	fclose(fp);
	free(cpcode);
	return 0;
}

