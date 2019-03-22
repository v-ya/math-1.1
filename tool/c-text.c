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

// c-text <text-path> <to-c-file> <c-var-name> [--compress]
int main(int argc, char *argv[])
{
	unsigned char *text,*cptext;
	int size,i;
	FILE *fp;
	if (argc<4) printf("%s <text-path> <to-c-file> <c-var-name> [--compress]\n",argv[0]);
	text=load_string(argv[1]);
	if (!text)
	{
		printf("Error: Not Load %s!\n",argv[1]);
		return -1;
	}
	if (argc>4 && strcmp(argv[4],"--compress")==0)
	{
		cptext=compress_code(text);
		free(text);
	}
	else cptext=text;
	if (!cptext)
	{
		printf("Error: Not Compress Text!\n");
		return -1;
	}
	fp=fopen(argv[2],"w");
	if (!fp)
	{
		printf("Error: Not Open %s!\n",argv[2]);
		free(cptext);
		return -1;
	}
	size=strlen(cptext)+1;
	fprintf(fp,"static char %s[%d]={%d",argv[3],size,cptext[0]);
	for(i=1;i<size;i++)
		fprintf(fp,",%d",cptext[i]);
	fprintf(fp,"};\n");
	fclose(fp);
	free(cptext);
	return 0;
}

