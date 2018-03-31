#include "../main.h"

char* get_type(u32 type)
{
	static char *t[(tlog_max+1)*2]={"null","void","byte","word","int","long","float","string",
		"vlist","vmat",NULL,"ubyte","uword","uint","ulong"};
	u32 i;
	if _oF(type&type_spe)
	{
		switch(type&type_spe)
		{
			case type_err:
				return "err";
			case type_end:
				return "end";
		}
	}
	if _oF(type&type_unsign) i=tlog_max;
	else i=0;
	type&=type_all;
	while(type)
	{
		type>>=1;
		i++;
	}
	return t[i];
}

void print_vlist(vlist *vl, u32 tab)
{
	u32 r;
	char *s,buffer[16];
	print("%*s",tab,"");
	if _oT(vl->v)
	{
		// auth
		r=vl->v->mode;
		print("%c%c%c%c%c%c", (r&auth_retype)?'s':'-', (r&auth_read)?'r':'-', (r&auth_write)?'w':'-',
			(r&auth_link)?'l':'-', (r&auth_run)?'f':'-', (r&auth_key)?'k':'-');
		// type
		print("%8s", get_type(vl->v->type));
		// length
		if _oF(vl->v->length) print("[%6u]", vl->v->length);
		else print("        ");
		// inode
		print("  %6u", (tab==0&&vl->v->inode>0)?(vl->v->inode-1):vl->v->inode);
		// value
		if _oF((vl->v->length==leng_no)&&(vl->v->type&type_string)&&vl->v->v.v_string)
		{
			s=vl->v->v.v_string;
			for(r=0;r<14;r++) if _oF(s[r]==0||s[r]=='\n'||s[r]=='\t'||s[r]=='\v'||s[r]=='\r') break;
			sprintf(buffer,"\"%%-.%ds\"",r);
			sbuf_sprintf(buffer, vl);
		}
		else sbuf_sprintf("%?", vl);
		print("  %16s  ", sbuf_get()->v.v_string);
		// name
		print("%s\n",vl->name);
	}
	else print("......  unknow               .                 .  %s\n",vl->name);
}

func(debug_list)
{
	vlist *vl;
	vmat vm,*vmp;
	u32 i,j,k;
	while(argv)
	{
		print_vlist(argv,0);
		if _oT(argv->v&&(!argv->v->length))
		{
			if _oF(argv->v->type&type_vlist)
			{
				vl=argv->v->v.v_vlist;
				if _oT(vl) while(vl->l) vl=vl->l;
				while(vl)
				{
					print_vlist(vl,4);
					vl=vl->r;
				}
			}
			else if _oF(argv->v->type&type_vmat)
			{
				vmp=argv->v->v.v_vmat;
				j=0;
				for(i=0;i<256;i++)
					if _oT(vmp->vl[i]) vm.vl[j++]=vmp->vl[i];
				for(i=0;i<j;i++)
				{
					vl=vm.vl[i];
					while(vl->l) vl=vl->l;
					vm.vl[i]=vl;
				}
				while(j)
				{
					vl=vm.vl[0];
					k=0;
					for(i=1;i<j;i++)
					{
						if (strcmp(vm.vl[i]->name,vl->name)<0)
						{
							vl=vm.vl[i];
							k=i;
						}
					}
					print_vlist(vl,4);
					vm.vl[k]=vl->r;
					if _oF(!vm.vl[k])
					{
						j--;
						vm.vl[k]=vm.vl[j];
					}
				}
			}
		}
		argv=argv->r;
	}
	ret->type=type_void;
	ret->v.v_long=0;
}

