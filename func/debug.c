#include "../main.h"

char* get_type(u32 type, u32 auth)
{
	static char *t[(tlog_max+1)*2]={"null","void","byte","word","int","long","float","string",
		"vlist","vmat","refer",NULL,"ubyte","uword","uint","ulong"};
	u32 i;
	if _oF(auth&auth_key && type==type_void) return "key";
	if _oF(auth&auth_run && type==type_void) return "fun";
	if _oF(auth&auth_run && type==type_vlist) return "objfun";
	
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

void print_vlist(vlist *vl, u32 tab, void *rp)
{
	u32 r;
	var *vp;
	vlist vt={0};
	char *s,buffer[16];
	print("%*s",tab,"");
	vp=vl->v;
	if _oT(vp)
	{
		if _oF(vp->type&type_refer)
		{
			vp=refer_check(vp);
			if _oF(!vp) vp=vl->v;
		}
		// auth
		r=vl->v->mode;
		print("%c%c%c%c%c%c", (r&auth_retype)?'s':(r&auth_relength?'S':'-'), (r&auth_read)?(r&auth_system?'R':'r'):'-',
			(r&auth_write)?'w':'-', (r&auth_link)?'l':'-', (r&auth_run)?'f':'-', (r&auth_key)?'k':'-');
		if _oF((vl->v->type&type_refer)&&(vl->v!=vp))
		{
			r=vp->mode;
			print("=>%c%c%c%c%c%c", (r&auth_retype)?'s':(r&auth_relength?'S':'-'), (r&auth_read)?(r&auth_system?'R':'r'):'-',
				(r&auth_write)?'w':'-', (r&auth_link)?'l':'-', (r&auth_run)?'f':'-', (r&auth_key)?'k':'-');
		}
		else print("        ");
		// type
		print("%8s", get_type(vl->v->type,vl->v->mode));
		// length or refer
		if _oF((vl->v->type&type_refer)&&(vl->v!=vp))
		{
			print("=>");
			print("%6s",get_type(vp->type,vp->mode));
		}
		else if _oF(vp->length) print("[%6u]", vp->length);
		else print("        ");
		// inode
		if _oF((vl->v->type&type_refer)&&(vl->v!=vp)) print("  %4u=>%4u", (vl->v==rp&&vl->v->inode>0)?(vl->v->inode-1):vl->v->inode, (vp==rp&&vp->inode>0)?(vp->inode-1):vp->inode);
		else print("      %6u", (tab==0&&vp->inode>0)?(vp->inode-1):vp->inode);
		// value
		vt.v=vp;
		if _oF((vp->length==leng_no)&&(vp->type&type_string)&&vp->v.v_string)
		{
			s=vp->v.v_string;
			for(r=0;r<14;r++) if _oF(s[r]==0||s[r]=='\n'||s[r]=='\t'||s[r]=='\v'||s[r]=='\r') break;
			sprintf(buffer,"\"%%-.%ds\"",r);
			sbuf_sprintf(buffer, &vt);
		}
		else sbuf_sprintf("%?", &vt);
		print("  %16s  ", sbuf_get()->v.v_string);
		// name
		if _oT(vl->name) print("(%s)\n",vl->name);
		else print("[%llu]\n",vl->head);
	}
	else
	{
		if _oT(vl->name) print("......          unknow m=%016llx v=%016llx  (%s)\n",(u64)vl->mode,(u64)vl->v,vl->name);
		else print("......          unknow m=%016llx v=%016llx  [%016llx]\n",(u64)vl->mode,(u64)vl->v,vl->head);
	}
}

func(debug_list)
{
	vlist *vl;
	vmat vm,*vmp;
	u32 i,j,k;
	while(argv)
	{
		print_vlist(argv,0,argv->v);
		if _oT(argv->v&&(!argv->v->length))
		{
			if _oF(argv->v->type&type_vlist)
			{
				vl=argv->v->v.v_vlist;
				if _oT(vl) while(vl->l) vl=vl->l;
				while(vl)
				{
					if (vl->name) print_vlist(vl,4,argv->v);
					vl=vl->r;
				}
				vl=argv->v->v.v_vlist;
				if _oT(vl) while(vl->l) vl=vl->l;
				while(vl)
				{
					if (!vl->name) print_vlist(vl,4,argv->v);
					vl=vl->r;
				}
			}
			else if _oF(argv->v->type&type_vmat)
			{
				// index == string
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
					vl=NULL;
					k=256;
					for(i=0;;i++)
					{
						loop_s:
						if _oF(i>=j) break;
						while(vm.vl[i] && !vm.vl[i]->name) vm.vl[i]=(vm.vl[i])->r;
						if _oF(!vm.vl[i])
						{
							j--;
							vm.vl[i]=vm.vl[j];
							vm.vl[j]=NULL;
							goto loop_s;
						}
						if (!vl || strcmp(vm.vl[i]->name,vl->name)<0)
						{
							vl=vm.vl[i];
							k=i;
						}
					}
					if (vl)
					{
						print_vlist(vl,4,argv->v);
						vm.vl[k]=vl->r;
						if _oF(!vm.vl[k])
						{
							j--;
							vm.vl[k]=vm.vl[j];
							vm.vl[j]=NULL;
						}
					}
				}
				// index == number
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
					vl=NULL;
					k=256;
					for(i=0;;i++)
					{
						loop_n:
						if _oF(i>=j) break;
						while(vm.vl[i] && vm.vl[i]->name) vm.vl[i]=(vm.vl[i])->r;
						if _oF(!vm.vl[i])
						{
							j--;
							vm.vl[i]=vm.vl[j];
							vm.vl[j]=NULL;
							goto loop_n;
						}
						if (!vl || vm.vl[i]->head<vl->head)
						{
							vl=vm.vl[i];
							k=i;
						}
					}
					if (vl)
					{
						print_vlist(vl,4,argv->v);
						vm.vl[k]=vl->r;
						if _oF(!vm.vl[k])
						{
							j--;
							vm.vl[k]=vm.vl[j];
							vm.vl[j]=NULL;
						}
					}
				}
			}
		}
		argv=argv->r;
	}
	ret->type=type_void;
	ret->v.v_long=0;
}

