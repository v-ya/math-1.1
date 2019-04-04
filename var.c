#include "main.h"

// glob static var
static var _v__null = {
	.type	=type_null,
	.length	=leng_no,
	.inode	=0,
	.mode	=free_temp|auth_read,
	.v	=0
} ;

var *_var_null = &_v__null;
vmat *_refpool_vmat;

var* get_null(void)
{
	return _var_null;
}

vmat* vmat_alloc(void)
{
	vmat *vm;
	vm=malloc(sizeof(vmat));
	if _oT(vm)
	{
		vm->mask=vmat_init_size-1;
		vm->number=0;
		vm->avl=malloc(vmat_init_size*sizeof(vlist*));
		if _oF(!vm->avl)
		{
			free(vm);
			return NULL;
		}
		memset(vm->avl,0,vmat_init_size*sizeof(vlist*));
		vm->flag=vmat_flag_free;
		return vm;
	}
	else return NULL;
}

void vmat_free(vmat *vm)
{
	u32 i,mask;
	if _oF(!vm) return ;
	mask=vm->mask;
	for(i=0;i<=mask;i++)
		vlist_free(vm->avl[i]);
	if _oT(vm->flag&vmat_flag_favl) free(vm->avl);
	if _oT(vm->flag&vmat_flag_fvmat) free(vm);
}

vlist* vlist_alloc(char *name)
{
	vlist *vl;
	u32 size;
	vl=malloc(sizeof(vlist));
	if _oF(vl==NULL) return NULL;
	if _oT(name)
	{
		size=strlen(name)+1;
		vl->name=malloc(size);
		if _oF(vl->name==NULL)
		{
			free(vl);
			return NULL;
		}
		memcpy(vl->name,name,size);
		vl->head=vname_head(name);
		vl->mode=free_name;
	}
	else
	{
		vl->name=NULL;
		vl->head=0;
		vl->mode=0;
	}
	vl->v=NULL;
	vl->l=NULL;
	vl->r=NULL;
}

vlist* vlist_alloc_index(u64 head)
{
	vlist *vl;
	vl=malloc(sizeof(vlist));
	if _oF(vl==NULL) return NULL;
	vl->name=NULL;
	vl->head=head;
	vl->mode=0;
	vl->v=NULL;
	vl->l=NULL;
	vl->r=NULL;
}

void vlist_free(vlist *vl)
{
	vlist *vr;
	if _oF(!vl) return ;
	while(vl->l) vl=vl->l;
	while(vl)
	{
		vr=vl->r;
		if _oT(vl->mode&free_name) free(vl->name);
		if _oT(vl->mode&free_pointer) var_free(vl->v);
		free(vl);
		vl=vr;
	}
}

var* var_alloc(u32 tlog, u32 length)
{
	var *vp;
	if _oT(tlog<tlog_max)
	{
		vp=malloc(sizeof(var));
		if _oF(!vp) return NULL;
		vp->type=type_tlog(tlog);
		vp->length=length;
		vp->inode=0;
		vp->mode=auth_normal;
		if _oT(length==leng_no)
		{
			switch(tlog)
			{
				case tlog_float:
					vp->v.v_float=0;
					break;
				case tlog_string:
				case tlog_vlist:
					vp->v.v_void=NULL;
					vp->mode|=free_pointer;
					break;
				case tlog_vmat:
					vp->v.v_vmat=vmat_alloc();
					if _oF(vp->v.v_vmat==NULL)
					{
						free(vp);
						return NULL;
					}
					vp->mode|=free_pointer;
					break;
				default:
					vp->v.v_long=0;
					break;
			}
			return vp;
		}
		else
		{
			u32 size;
			switch(tlog)
			{
				case tlog_int:
					size=sizeof(s32);
					break;
				case tlog_long:
					size=sizeof(s64);
					break;
				case tlog_float:
					size=sizeof(double);
					break;
				case tlog_byte:
					size=sizeof(s8);
					break;
				case tlog_word:
					size=sizeof(s16);
					break;
				default:
					size=sizeof(void *);
					break;
			}
			size*=length;
			vp->mode|=free_pointer;
			vp->v.v_void=malloc(size);
			if _oF(vp->v.v_void==NULL)
			{
				free(vp);
				return NULL;
			}
			switch(tlog)
			{
				case tlog_float:
					for(size=0;size<length;size++)
						vp->v.vp_float[size]=0;
					break;
				case tlog_vmat:
					for(size=0;size<length;size++)
					{
						vp->v.vp_vmat[size]=vmat_alloc();
						if _oF(vp->v.vp_vmat[size]==NULL) break;
					}
					if _oF(size<length)
					{
						while(size>0)
						{
							size--;
							free(vp->v.vp_vmat[size]);
						}
						free(vp);
						return NULL;
					}
					break;
				default:
					memset(vp->v.v_void,0,size);
			}
			return vp;
		}
	}
	else return NULL;
}

void var_free(var *vp)
{
	// 相较于删除变量时的冗长代码，仅仅 inode-- 更容易凸显效率
	// 况且调用自定义函数会有大量的变量链接与链接删除
	// 而主动删除变量的情况少见且愿意付出较大的时间代价
	if _oF(vp==NULL) return ;
	if _oF(vp->mode&free_temp) return ;
	lock_alloc(lock_inode);
	if _oT(vp->inode>1)
	{
		vp->inode--;
		lock_free(lock_inode);
	}
	else
	{
		if _oF(vp->mode&is_refer) refer_free((u64)vp);
		lock_free(lock_inode);
		if _oF(vp->mode&free_pointer)
		{
			// 需要主动释放 vp->v.*
			if _oT(vp->length==leng_no)
			{
				// No Array
				switch(vp->type)
				{
					case type_string:
						free(vp->v.v_string);
						break;
					case type_vlist:
						vlist_free(vp->v.v_vlist);
						break;
					case type_vmat:
						vmat_free(vp->v.v_vmat);
						break;
					case type_refer:
						refer_unset(vp);
						break;
				}
			}
			else
			{
				// Array
				u32 i;
				switch(vp->type)
				{
					case type_string:
						for(i=0;i<vp->length;i++)
							free(vp->v.vp_string[i]);
						break;
					case type_vlist:
						for(i=0;i<vp->length;i++)
							vlist_free(vp->v.vp_vlist[i]);
						break;
					case type_vmat:
						for(i=0;i<vp->length;i++)
							vmat_free(vp->v.vp_vmat[i]);
						break;
				}
				free(vp->v.v_void);
			}
		}
		free(vp);
	}
}

vlist* vlist_insert(vlist *vl, vlist *v)
{
	// v->l == v->r == NULL
	u32 i;
	s32 turn;
	if _oT(!vl) return v;
	turn=0;
	lock_alloc(lock_vlist);
	while(1)
	{
		if (v->head > vl->head)
		{
			label_a:
			if _oF(turn<0)
			{
				label_ai:
				v->l=vl;
				v->r=vl->r;
				vl->r=v;
				if _oT(v->r) v->r->l=v;
				vl=v;
				break;
			}
			turn=1;
			if _oT(vl->r)
			{
				vl=vl->r;
				continue;
			}
			else goto label_ai;
		}
		else if (v->head < vl->head)
		{
			label_b:
			if _oF(turn>0)
			{
				label_bi:
				v->r=vl;
				v->l=vl->l;
				vl->l=v;
				if _oT(v->l) v->l->r=v;
				vl=v;
				break;
			}
			turn=-1;
			if _oT(vl->l)
			{
				vl=vl->l;
				continue;
			}
			else goto label_bi;
		}
		else if _oT(v->name && vl->name)
		{
			if _oT((v->head&0xff)==0) break;
			else
			{
				for(i=8;;i++)
				{
					if (v->name[i] > vl->name[i]) goto label_a;
					else if (v->name[i] < vl->name[i]) goto label_b;
					else if _oF(v->name[i]==0) break;
				}
			}
		}
		else if (v->name && !vl->name) goto label_a;
		else if (!v->name && vl->name) goto label_b;
		else break;
	}
	lock_free(lock_vlist);
	return vl;
}

vlist* vlist_find(vlist *vl, char *name)
{
	u32 i;
	s32 turn;
	u64 head;
	if _oF(vl==NULL) return NULL;
	head=vname_head(name);
	turn=0;
	while(1)
	{
		if (head > vl->head)
		{
			label_a:
			if _oF(turn<0) return NULL;
			turn=1;
			if _oT(vl->r)
			{
				vl=vl->r;
				continue;
			}
			else return NULL;
		}
		else if (head < vl->head)
		{
			label_b:
			if _oF(turn>0) return NULL;
			turn=-1;
			if _oT(vl->l)
			{
				vl=vl->l;
				continue;
			}
			else return NULL;
		}
		else
		{
			if _oF(!vl->name)
			{
				vl=vl->r;
				if _oF(!vl || vl->head!=head) return NULL;
			}
			if _oT((head&0xff)==0) return vl;
			else
			{
				for(i=8;;i++)
				{
					if (name[i] > vl->name[i]) goto label_a;
					else if (name[i] < vl->name[i]) goto label_b;
					else if _oF(name[i]==0) return vl;
				}
			}
		}
	}
}

vlist* vlist_find_index(vlist *vl, u64 head)
{
	if _oF(vl==NULL) return NULL;
	if (head > vl->head)
	{
		do
		{
			vl=vl->r;
		}
		while(vl && head>vl->head) ;
	}
	else if (head < vl->head)
	{
		do
		{
			vl=vl->l;
		}
		while(vl && head<vl->head) ;
	}
	if _oF(vl && vl->name)
	{
		do
		{
			vl=vl->l;
		}
		while(vl && !vl->name) ;
	}
	if _oF(!vl || head!=vl->head) return NULL;
	else return vl;
}

vlist* vlist_delete(vlist *vl, char *name)
{
	vlist *v;
	var *vp=NULL;
	v=vlist_find(vl,name);
	if _oF(v==NULL) return vl;
	lock_alloc(lock_vlist);
	if _oT(v->l)
	{
		vl=v->l;
		vl->r=v->r;
		if _oT(v->r) vl->r->l=vl;
		if _oT(v->mode&free_name) free(v->name);
		if _oT(v->mode&free_pointer) vp=v->v;
		free(v);
	}
	else if _oT(v->r)
	{
		vl=v->r;
		vl->l=v->l;
		if _oT(v->l) vl->l->r=vl;
		if _oT(v->mode&free_name) free(v->name);
		if _oT(v->mode&free_pointer) vp=v->v;
		free(v);
	}
	else
	{
		if _oT(v->mode&free_name) free(v->name);
		if _oT(v->mode&free_pointer) vp=v->v;
		free(v);
		vl=NULL;
	}
	lock_free(lock_vlist);
	if _oT(vp) var_free(vp);
	return vl;
}

vlist* vlist_delete_index(vlist *vl, u64 head)
{
	vlist *v;
	var *vp=NULL;
	v=vlist_find_index(vl,head);
	if _oF(v==NULL) return vl;
	lock_alloc(lock_vlist);
	if _oT(v->l)
	{
		vl=v->l;
		vl->r=v->r;
		if _oT(v->r) vl->r->l=vl;
		if _oT(v->mode&free_name) free(v->name);
		if _oT(v->mode&free_pointer) vp=v->v;
		free(v);
	}
	else if _oT(v->r)
	{
		vl=v->r;
		vl->l=v->l;
		if _oT(v->l) vl->l->r=vl;
		if _oT(v->mode&free_name) free(v->name);
		if _oT(v->mode&free_pointer) vp=v->v;
		free(v);
	}
	else
	{
		if _oT(v->mode&free_name) free(v->name);
		if _oT(v->mode&free_pointer) vp=v->v;
		free(v);
		vl=NULL;
	}
	lock_free(lock_vlist);
	if _oT(vp) var_free(vp);
	return vl;
}

void vmat_extend(vmat *vm)
{
	vlist **avl0,**avl1,*vl,*v;
	u32 i,mask0,mask1,gen;
	lock_alloc(lock_vmatex);
	if _oF(vm->flag&vmat_flag_notex) goto End;
	mask0=vm->mask;
	mask1=mask0;
	do
	{
		mask1=(mask1<<1)|1;
		if _oF(mask1&0x80000000)
		{
			vm->flag|=vmat_flag_notex;
			goto End;
		}
	}
	while(vm->number>=mask1) ;
	avl0=vm->avl;
	avl1=malloc((mask1+1)*sizeof(vlist*));
	if _oF(!avl1) goto End;
	memset(avl1,0,(mask1+1)*sizeof(vlist*));
	vm->avl=NULL;
	for(i=0;i<=mask0;i++)
	{
		vl=avl0[i];
		if _oF(vl) while(vl->l) vl=vl->l;
		while(vl)
		{
			v=vl;
			vl=vl->r;
			v->l=NULL;
			v->r=NULL;
			gen=(v->name?vname_gen(v->name):vhead_gen(v->head))&mask1;
			avl1[gen]=vlist_insert(avl1[gen],v);
		}
	}
	if _oT(vm->flag&vmat_flag_favl) free(avl0);
	vm->flag|=vmat_flag_favl;
	vm->avl=avl1;
	vm->mask=mask1;
	End:
	lock_free(lock_vmatex);
}

void vmat_insert(vmat *vm, vlist *v)
{
	// v->l == v->r == NULL
	u32 i;
	if _oF(vm==NULL) return ;
	i=(v->name?vname_gen(v->name):vhead_gen(v->head))&vm->mask;
	if ((vm->avl[i]=vlist_insert(vm->avl[i],v))!=v) return ;
	vm->number++;
	if _oF(vm->number>vm->mask && !(vm->flag&vmat_flag_notex)) vmat_extend(vm);
}

vlist* vmat_find(vmat *vm, char *name)
{
	if _oF(vm==NULL) return NULL;
	return vlist_find(vm->avl[vname_gen(name)&vm->mask],name);
}

vlist* vmat_find_index(vmat *vm, u64 head)
{
	if _oF(vm==NULL) return NULL;
	return vlist_find_index(vm->avl[vhead_gen(head)&vm->mask],head);
}

void vmat_delete(vmat *vm, char *name)
{
	u32 i;
	if _oF(vm==NULL) return ;
	i=vname_gen(name)&vm->mask;
	vm->avl[i]=vlist_delete(vm->avl[i],name);
	if _oT(vm->number) vm->number--;
}

void vmat_delete_index(vmat *vm, u64 head)
{
	u32 i;
	if _oF(vm==NULL) return ;
	i=vhead_gen(head)&vm->mask;
	vm->avl[i]=vlist_delete_index(vm->avl[i],head);
	if _oT(vm->number) vm->number--;
}

void var_save(var *vp)
{
	lock_alloc(lock_inode);
	if _oT(!(vp->mode&free_temp))
	{
		(vp->inode)++;
	}
	lock_free(lock_inode);
}

void vlist_link(vlist *vl, var *v)
{
	if _oF(vl->v==v) return ;
	if _oF(vl->v&&(vl->mode&free_pointer)) var_free(vl->v);
	lock_alloc(lock_inode);
	if _oT(v&&!(v->mode&free_temp)) v->inode++;
	vl->v=v;
	vl->mode|=free_pointer;
	lock_free(lock_inode);
}

vlist* v_find(var *obj, char *name)
{
	if _oF(obj==NULL) return NULL;
	if _oF(obj->length!=leng_no) return NULL;
	if _oT(obj->type&type_vmat) return vlist_find(obj->v.v_vmat->avl[vname_gen(name)&obj->v.v_vmat->mask],name);
	else if _oT(obj->type&type_vlist) return vlist_find(obj->v.v_vlist,name);
	else return NULL;
}

vlist* v_find_index(var *obj, u64 head)
{
	if _oF(obj==NULL) return NULL;
	if _oF(obj->length!=leng_no) return NULL;
	if _oT(obj->type&type_vmat) return vlist_find_index(obj->v.v_vmat->avl[vhead_gen(head)&obj->v.v_vmat->mask],head);
	else if _oT(obj->type&type_vlist) return vlist_find_index(obj->v.v_vlist,head);
	else return NULL;
}

var* var_find(var *obj, char *name)
{
	vlist *vl;
	if _oF(obj==NULL) return NULL;
	if _oF(obj->length!=leng_no) return NULL;
	if _oT(obj->type&type_vmat)
	{
		vl=vlist_find(obj->v.v_vmat->avl[vname_gen(name)&obj->v.v_vmat->mask],name);
		if _oT(vl) return vl->v;
		else return NULL;
	}
	else if _oT(obj->type&type_vlist)
	{
		vl=vlist_find(obj->v.v_vlist,name);
		if _oT(vl) return vl->v;
		else return NULL;
	}
	else return NULL;
}

var* var_find_index(var *obj, u64 head)
{
	vlist *vl;
	if _oF(obj==NULL) return NULL;
	if _oF(obj->length!=leng_no) return NULL;
	if _oT(obj->type&type_vmat)
	{
		vl=vlist_find_index(obj->v.v_vmat->avl[vhead_gen(head)&obj->v.v_vmat->mask],head);
		if _oT(vl) return vl->v;
		else return NULL;
	}
	else if _oT(obj->type&type_vlist)
	{
		vl=vlist_find_index(obj->v.v_vlist,head);
		if _oT(vl) return vl->v;
		else return NULL;
	}
	else return NULL;
}

vlist* var_insert(var *obj, char *name, u32 tlog, u32 length)
{
	vlist *vl;
	var *vp;
	if _oF(!(obj->type&type_object)) return NULL;
	vl=vlist_alloc(name);
	if _oF(!vl) return NULL;
	if _oF(tlog>=tlog_max) goto End;
	vp=var_alloc(tlog,length);
	if _oF(!vp)
	{
		vlist_free(vl);
		return NULL;
	}
	vlist_link(vl,vp);
	End:
	if _oT(obj->type&type_vmat) vmat_insert(obj->v.v_vmat,vl);
	else if _oT(obj->type&type_vlist) obj->v.v_vlist=vlist_insert(obj->v.v_vlist,vl);
	return vl;
}

void var_delete(var *obj, char *name)
{
	if _oT(obj->type&type_vmat) vmat_delete(obj->v.v_vmat,name);
	else if _oT(obj->type&type_vlist) obj->v.v_vlist=vlist_delete(obj->v.v_vlist,name);
}

void var_delete_index(var *obj, u64 head)
{
	if _oT(obj->type&type_vmat) vmat_delete_index(obj->v.v_vmat,head);
	else if _oT(obj->type&type_vlist) obj->v.v_vlist=vlist_delete_index(obj->v.v_vlist,head);
}

var* var_replace(var *obj, char *name, u32 tlog, u32 length)
{
	vlist *vl;
	var *vp;
	vp=var_alloc(tlog,length);
	if _oF(!vp) return NULL;
	if _oT(obj->type&type_vlist)
	{
		vl=vlist_find(obj->v.v_vlist,name);
		if _oF(!vl)
		{
			vl=vlist_alloc(name);
			if _oF(!vl)
			{
				var_free(vp);
				return NULL;
			}
			obj->v.v_vlist=vlist_insert(obj->v.v_vlist,vl);
			vlist_link(vl,vp);
			return vp;
		}
		else
		{
			vlist_link(vl,vp);
			return vp;
		}
		
	}
	else if _oT(obj->type&type_vmat)
	{
		vl=vmat_find(obj->v.v_vmat,name);
		if _oF(!vl)
		{
			vl=vlist_alloc(name);
			if _oF(!vl)
			{
				var_free(vp);
				return NULL;
			}
			vmat_insert(obj->v.v_vmat,vl);
			vlist_link(vl,vp);
			return vp;
		}
		else
		{
			vlist_link(vl,vp);
			return vp;
		}
	}
	else
	{
		var_free(vp);
		return NULL;
	}
}

var* var_set(var *obj, char *name, u32 tlog, u32 length, u32 mode, value *v)
{
	var *vp;
	vp=var_replace(obj,name,tlog,leng_no);
	if _oF(!vp) return NULL;
	vp->mode=mode;
	vp->length=length;
	vp->v.v_long=v->v_long;
	return vp;
}

var* var_link(var *obj, char *name, var *v)
{
	vlist *vl;
	u32 i;
	if _oF(obj==NULL) return NULL;
	if _oF(obj->length!=leng_no) return NULL;
	if _oF(obj->type&type_vmat)
	{
		i=vname_gen(name)&obj->v.v_vmat->mask;
		vl=vlist_find(obj->v.v_vmat->avl[i],name);
		if _oT(!vl)
		{
			vl=vlist_alloc(name);
			if _oF(!vl) return NULL;
			obj->v.v_vmat->avl[i]=vlist_insert(obj->v.v_vmat->avl[i],vl);
		}
		vlist_link(vl,v);
		return v;
	}
	else if _oT(obj->type&type_vlist)
	{
		vl=vlist_find(obj->v.v_vlist,name);
		if _oT(!vl)
		{
			vl=vlist_alloc(name);
			if _oF(!vl) return NULL;
			obj->v.v_vlist=vlist_insert(obj->v.v_vlist,vl);
		}
		vlist_link(vl,v);
		return v;
	}
	else return NULL;
}

var* var_link_index(var *obj, u64 head, var *v)
{
	vlist *vl;
	u32 i;
	if _oF(obj==NULL) return NULL;
	if _oF(obj->length!=leng_no) return NULL;
	if _oF(obj->type&type_vmat)
	{
		i=vhead_gen(head)&obj->v.v_vmat->mask;
		vl=vlist_find_index(obj->v.v_vmat->avl[i],head);
		if _oT(!vl)
		{
			vl=vlist_alloc_index(head);
			if _oF(!vl) return NULL;
			obj->v.v_vmat->avl[i]=vlist_insert(obj->v.v_vmat->avl[i],vl);
		}
		vlist_link(vl,v);
		return v;
	}
	else if _oT(obj->type&type_vlist)
	{
		vl=vlist_find_index(obj->v.v_vlist,head);
		if _oT(!vl)
		{
			vl=vlist_alloc_index(head);
			if _oF(!vl) return NULL;
			obj->v.v_vlist=vlist_insert(obj->v.v_vlist,vl);
		}
		vlist_link(vl,v);
		return v;
	}
	else return NULL;
}

void get_tmpvar(char *exp, char **expp, var *v)
{
	int is_neg=0;
	u64 tv_u64;
	double tv_d0,tv_d1;
	if _oF(*exp=='0')
	{
		exp++;
		tv_u64=0;
		if _oF(*exp=='.') goto label_dot;
		else if _oF(*exp=='x')
		{
			exp++;
			while((*exp>='0'&&*exp<='9')||(*exp>='a'&&*exp<='f')||(*exp>='A'&&*exp<='F')||*exp=='_')
			{
				if _oT(*exp>='0'&&*exp<='9')
				{
					tv_u64<<=4;
					tv_u64|=(*exp-'0')&0x0f;
					exp++;
					continue;
				}
				else if _oT(*exp>='a'&&*exp<='f')
				{
					tv_u64<<=4;
					tv_u64|=(*exp-('a'-10))&0x0f;
					exp++;
					continue;
				}
				else if _oT(*exp>='A'&&*exp<='F')
				{
					tv_u64<<=4;
					tv_u64|=(*exp-('A'-10))&0x0f;
					exp++;
					continue;
				}
				else
				{
					exp++;
					continue;
				}
			}
		}
		else if _oF((*exp>='0'&&*exp<'8')||*exp=='o'||*exp=='_')
		{
			if _oF(*exp=='o') exp++;
			while((*exp>='0'&&*exp<'8')||*exp=='_')
			{
				if _oT(*exp>='0'&&*exp<'8')
				{
					tv_u64<<=3;
					tv_u64|=(*exp-'0')&0x07;
					exp++;
					continue;
				}
				else
				{
					exp++;
					continue;
				}
			}
		}
		else if _oF(*exp=='b')
		{
			exp++;
			while(*exp=='0'||*exp=='1'||*exp=='_')
			{
				if _oT(*exp=='0')
				{
					tv_u64<<=1;
					exp++;
					continue;
				}
				else if _oT(*exp=='1')
				{
					tv_u64<<=1;
					tv_u64|=0x01;
					exp++;
					continue;
				}
				else
				{
					exp++;
					continue;
				}
			}
		}
		v->v.v_long=tv_u64;
		v->type=type_long;
	}
	else if _oT(*exp>'0'&&*exp<='9')
	{
		label_znum:
		tv_u64=0;
		while(*exp>='0'&&*exp<='9')
			tv_u64=tv_u64*10+(*(exp++)-'0');
		if _oF(*exp=='.'||*exp=='e'||*exp=='E')
		{
			label_dot:
			tv_d0=tv_u64;
			tv_d1=1;
			if _oT(*exp=='.') exp++;
			else goto label_e;
			while(*exp>='0'&&*exp<='9')
			{
				tv_d1/=10;
				tv_d0+=tv_d1*(*(exp++)-'0');
			}
			if _oF(*exp=='e'||*exp=='E') goto label_e;
			v->v.v_float=is_neg?-tv_d0:tv_d0;
			v->type=type_float;
		}
		else
		{
			v->v.v_long=is_neg?-tv_u64:tv_u64;
			v->type=type_long;
		}
		goto End;
		label_e:
		exp++;
		if _oF(is_neg)
		{
			tv_d0=-tv_d0;
			is_neg=0;
		}
		if _oF(*exp=='-')
		{
			exp++;
			is_neg=1;
		}
		else if _oF(*exp=='+') exp++;
		tv_u64=0;
		while(*exp>='0'&&*exp<='9')
			tv_u64=tv_u64*10+(*(exp++)-'0');
		v->type=type_float;
		if _oF(is_neg) v->v.v_float=tv_d0*pow(0.1,tv_u64);
		else v->v.v_float=tv_d0*pow(10.0,tv_u64);
	}
	else if _oT(*exp=='-')
	{
		exp++;
		if _oF(*exp<'0'||*exp>'9')
		{
			// 将 '-' 认做运算符
			v->type=type_long;
			v->v.v_long=0;
			exp--;
			goto End;
		}
		is_neg=1;
		goto label_znum;
	}
	else if _oF(*exp=='\"')
	{
		v->v.v_string=get_string(exp,&exp);
		v->mode|=free_pointer;
		v->type=type_string;
	}
	else if _oF(*exp=='\'')
	{
		v->v.v_long=get_ascii(exp,&exp);
		v->type=type_long;
	}
	else if _oF(*exp=='{')
	{
		v->v.v_string=get_code(exp,&exp);
		v->mode|=free_pointer;
		v->type=type_string;
	}
	else
	{
		v->type=type_void;
		v->v.v_long=0;
	}
	End:
	if _oT(expp) *expp=exp;
}

void var_fixvalue(var *vp)
{
	switch(vp->type&type_znum)
	{
		case type_long:
		case 0:
			return ;
		case type_byte:
			if _oF(vp->type&type_unsign)
			{
				vp->v.v_long&=0x00000000000000ff;
				return ;
			}
			if _oF(vp->v.v_long&0x80)
			{
				vp->v.v_long|=0xffffffffffffff00;
				return ;
			}
			vp->v.v_long&=0x00000000000000ff;
			return ;
		case type_word:
			if _oF(vp->type&type_unsign)
			{
				vp->v.v_long&=0x000000000000ffff;
				return ;
			}
			if _oF(vp->v.v_long&0x8000)
			{
				vp->v.v_long|=0xffffffffffff0000;
				return ;
			}
			vp->v.v_long&=0x000000000000ffff;
			return ;
		case type_int:
			if _oF(vp->type&type_unsign)
			{
				vp->v.v_long&=0x00000000ffffffff;
				return ;
			}
			if _oF(vp->v.v_long&0x80000000)
			{
				vp->v.v_long|=0xffffffff00000000;
				return ;
			}
			vp->v.v_long&=0x00000000ffffffff;
			return ;
	}
	
	
}

void ptvar_alloc(var *object)
{
	u64 pthid,mix;
	vlist *vl,*vl0;
	pthid=pthread_self();
	vl=vlist_alloc(NULL);
	if _oF(!vl) return ;
	vl->head=pthid;
	// 线程独占操作，加锁
	lock_alloc(lock_ptvar);
	if _oT(object->type&type_vmat)
	{
		mix=vhead_gen(pthid)&object->v.v_vmat->mask;
		vl0=object->v.v_vmat->avl[mix];
	}
	else if _oT(object->type&type_vlist) vl0=object->v.v_vlist;
	else goto Err;
	if _oF(!vl0) ;
	else if _oT(vl0->head<pthid)
	{
		while(vl0->r&&vl0->r->head<=pthid) vl0=vl0->r;
		if _oF(vl0->head==pthid) goto Err;
		vl->l=vl0;
		vl->r=vl0->r;
		vl0->r=vl;
		if _oT(vl->r) vl->r->l=vl;
	}
	else if _oT(vl0->head>pthid)
	{
		while(vl0->l&&vl0->l->head>=pthid) vl0=vl0->l;
		if _oF(vl0->head==pthid) goto Err;
		vl->r=vl0;
		vl->l=vl0->l;
		vl0->l=vl;
		if _oT(vl->l) vl->l->r=vl;
	}
	else goto Err;
	if _oT(object->type&type_vmat)
	{
		object->v.v_vmat->avl[mix]=vl;
		(object->v.v_vmat->number)++;
		if _oF(object->v.v_vmat->number>object->v.v_vmat->mask && !(object->v.v_vmat->flag&vmat_flag_notex))
			vmat_extend(object->v.v_vmat);
	}
	else if _oT(object->type&type_vlist) object->v.v_vlist=vl;
	End:
	// 解锁，退出
	lock_free(lock_ptvar);
	return ;
	Err:
	free(vl);
	goto End;
}

void ptvar_free(var *object)
{
	u64 pthid,mix;
	vlist *vl,*vl0;
	pthid=pthread_self();
	// 线程独占操作，加锁
	lock_alloc(lock_ptvar);
	if _oT(object->type&type_vmat)
	{
		mix=vhead_gen(pthid)&object->v.v_vmat->mask;
		vl=object->v.v_vmat->avl[mix];
	}
	else if _oT(object->type&type_vlist) vl=object->v.v_vlist;
	else goto End;
	
	if _oF(!vl) goto End;
	if _oT(vl->head<pthid) while(vl->r&&vl->r->head<=pthid) vl=vl->r;
	else if _oT(vl->head>pthid) while(vl->l&&vl->l->head>=pthid) vl=vl->l;
	if _oF(vl->head!=pthid) goto End;
	vl0=vl;
	if _oT(vl->l)
	{
		vl=vl->l;
		vl->r=vl0->r;
		if _oT(vl0->r) vl0->r->l=vl;
	}
	else if _oT(vl->r)
	{
		vl=vl->r;
		vl->l=vl0->l;
		if _oT(vl0->l) vl0->l->r=vl;
	}
	else vl=NULL;
	vl0->l=NULL;
	vl0->r=NULL;
	vlist_free(vl0);
	if _oT(object->type&type_vmat) object->v.v_vmat->avl[mix]=vl;
	else if _oT(object->type&type_vlist) object->v.v_vlist=vl;
	End:
	// 解锁，退出
	lock_free(lock_ptvar);
	return ;
}

var* ptvar_get(var *object)
{
	u64 pthid;
	vlist *vl;
	var *r=NULL;
	pthid=pthread_self();
	lock_alloc(lock_ptvar);
	if _oT(object->type&type_vmat) vl=object->v.v_vmat->avl[vhead_gen(pthid)&object->v.v_vmat->mask];
	else if _oT(object->type&type_vlist) vl=object->v.v_vlist;
	else goto End;
	
	if _oF(!vl) goto End;
	if _oT(vl->head<pthid) while(vl->r&&vl->r->head<=pthid) vl=vl->r;
	else if _oT(vl->head>pthid) while(vl->l&&vl->l->head>=pthid) vl=vl->l;
	if _oT(vl->head==pthid) r=vl->v;
	End:
	lock_free(lock_ptvar);
	return r;
}

vlist* ptvar_vlist(var *object)
{
	u64 pthid;
	vlist *vl=NULL;
	pthid=pthread_self();
	lock_alloc(lock_ptvar);
	if _oT(object->type&type_vmat) vl=object->v.v_vmat->avl[vhead_gen(pthid)&object->v.v_vmat->mask];
	else if _oT(object->type&type_vlist) vl=object->v.v_vlist;
	else goto End;
	if _oT(vl)
	{
		if _oF(vl->head<pthid) while(vl && vl->head!=pthid) vl=vl->r;
		else if _oF(vl->head>pthid) while(vl && vl->head!=pthid) vl=vl->l;
	}
	End:
	lock_free(lock_ptvar);
	return vl;
}

var* ptvar_replace(var *object, var *value)
{
	vlist *vl;
	var *bak_value;
	vl=ptvar_vlist(object);
	if _oF(!vl)
	{
		ptvar_alloc(object);
		vl=ptvar_vlist(object);
		if _oF(!vl) return NULL;
	}
	bak_value=vl->v;
	vl->v=value;
	vl->mode|=free_pointer;
	return bak_value;
}

void refer_alloc(u64 id)
{
	vlist *vl;
	lock_alloc(lock_refer);
	if _oF(vmat_find_index(_refpool_vmat,id)) goto End;
	vl=vlist_alloc_index(id);
	if _oF(!vl) goto End;
	vmat_insert(_refpool_vmat,vl);
	End:
	lock_free(lock_refer);
}

void refer_free(u64 id)
{
	vlist *vl;
	lock_alloc(lock_refer);
	vl=vmat_find_index(_refpool_vmat,id);
	if _oF(!vl) goto End;
	vl->mode=0;
	vl->v=NULL;
	vmat_delete_index(_refpool_vmat,id);
	End:
	lock_free(lock_refer);
}

vlist* refer_get(u64 id)
{
	vlist *vl;
	lock_alloc(lock_refer);
	vl=vmat_find_index(_refpool_vmat,id);
	lock_free(lock_refer);
	return vl;
}

void refer_set(var *rp, var *vp)
{
	vlist *vl;
	if _oT((rp->type&type_refer)&&vp)
	{
		if _oF(rp->v.v_void) refer_unset(rp);
		lock_alloc(lock_refer);
		vl=vmat_find_index(_refpool_vmat,(u64)vp);
		lock_free(lock_refer);
		if _oF(!vl)
		{
			refer_alloc((u64)vp);
			lock_alloc(lock_refer);
			vl=vmat_find_index(_refpool_vmat,(u64)vp);
			lock_free(lock_refer);
			vp->mode|=is_refer;
		}
		if _oT(vl&&(vl->mode<refer_max))
		{
			(vl->mode)++;
			rp->v.v_void=vp;
			rp->mode|=free_pointer;
		}
	}
}

void refer_unset(var *rp)
{
	vlist *vl;
	var *vp;
	if _oT((rp->type&type_refer)&&(rp->v.v_void))
	{
		vp=(var*)(rp->v.v_void);
		rp->v.v_void=NULL;
		rp->mode&=~free_pointer;
		lock_alloc(lock_refer);
		vl=vmat_find_index(_refpool_vmat,(u64)vp);
		if _oT(vl)
		{
			if _oT(vl->mode>0) (vl->mode)--;
			if _oF(vl->mode==0)
			{
				vmat_delete_index(_refpool_vmat,(u64)vp);
				vp->mode&=~is_refer;
			}
		}
		lock_free(lock_refer);
	}
}

var* refer_check(var *rp)
{
	vlist *vl;
	if _oT((rp->type&type_refer)&&(rp->v.v_void))
	{
		lock_alloc(lock_refer);
		vl=vmat_find_index(_refpool_vmat,(u64)(rp->v.v_void));
		lock_free(lock_refer);
		if _oT(vl&&(vl->mode>0)) return (var*)(rp->v.v_void);
		else
		{
			rp->v.v_void=NULL;
			rp->mode&=~free_pointer;
			return NULL;
		}
	}
	else return NULL;
}


