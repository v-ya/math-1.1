#include "../main.h"

vlist* _func__get_index_new(vlist *vf, u64 head)
{
	vlist *vl;
	var *vp;
	vl=vlist_alloc_index(head);
	if _oF(!vl) return NULL;
	vp=var_alloc(tlog_void,leng_no);
	if _oF(!vp)
	{
		vlist_free(vl);
		return NULL;
	}
	vlist_link(vl,vp);
	if _oF(vf->name)
	{
		vp->type=type_string;
		head=strlen(vf->name)+1;
		vp->v.v_string=malloc(head);
		if _oF(!vp->v.v_string)
		{
			vlist_free(vl);
			return NULL;
		}
		memcpy(vp->v.v_string,vf->name,head);
		vp->mode|=free_pointer;
	}
	else
	{
		vp->type=type_long|type_unsign;
		vp->v.v_long=vf->head;
	}
	return vl;
}

func(get_index)
{
	static char *label="[object].get_index";
	static u32 type_1[1]={type_vmat};
	static u32 type_2[2]={type_vmat,type_znum};
	var *root;
	vmat *vm,*vmf;
	vlist *vl,*vlf;
	u64 type,p;
	u32 i;
	
	switch(argc)
	{
		case 1:
			if _oF(check_varlist(argv,1,type_1)) return get_error(errid_FunArgvType,label);
			root=argv->v;
			type=3;
			break;
		case 2:
			if _oF(check_varlist(argv,2,type_2)) return get_error(errid_FunArgvType,label);
			root=argv->v;
			type=argv->r->v->v.v_long;
			break;
		default:
			return get_error(errid_FunArgvType,label);
	}
	if _oF(!(root->mode&auth_write)) return get_error(errid_VarNotWrite,label);
	vm=root->v.v_vmat;
	root=ret->v.v_var;
	ret->type=type_long|type_unsign;
	ret->v.v_long=0;
	p=0;
	switch(type)
	{
		case 1:
			// only name
			if _oF(root->type&type_vlist)
			{
				// vlist
				vlf=root->v.v_vlist;
				if _oT(vlf) while(vlf->l) vlf=vlf->l;
				while(vlf)
				{
					if _oT(vlf->name)
					{
						if _oT(!vmat_find_index(vm,p))
						{
							vl=_func__get_index_new(vlf,p);
							if _oF(!vl) return get_error(errid_MemLess,label);
							vmat_insert(vm,vl);
						}
						p++;
					}
					vlf=vlf->r;
				}
			}
			else
			{
				// vmat
				vmf=root->v.v_vmat;
				for(i=0;i<=vmf->mask;i++)
				{
					vlf=vmf->avl[i];
					if _oT(vlf) while(vlf->l) vlf=vlf->l;
					while(vlf)
					{
						if _oT(vlf->name)
						{
							if _oT(!vmat_find_index(vm,p))
							{
								vl=_func__get_index_new(vlf,p);
								if _oF(!vl) return get_error(errid_MemLess,label);
								vmat_insert(vm,vl);
							}
							p++;
						}
						vlf=vlf->r;
					}
				}
			}
			break;
		case 2:
			// only head
			if _oF(root->type&type_vlist)
			{
				// vlist
				vlf=root->v.v_vlist;
				if _oT(vlf) while(vlf->l) vlf=vlf->l;
				while(vlf)
				{
					if _oT(!vlf->name)
					{
						if _oT(!vmat_find_index(vm,p))
						{
							vl=_func__get_index_new(vlf,p);
							if _oF(!vl) return get_error(errid_MemLess,label);
							vmat_insert(vm,vl);
						}
						p++;
					}
					vlf=vlf->r;
				}
			}
			else
			{
				// vmat
				vmf=root->v.v_vmat;
				for(i=0;i<=vmf->mask;i++)
				{
					vlf=vmf->avl[i];
					if _oT(vlf) while(vlf->l) vlf=vlf->l;
					while(vlf)
					{
						if _oT(!vlf->name)
						{
							if _oT(!vmat_find_index(vm,p))
							{
								vl=_func__get_index_new(vlf,p);
								if _oF(!vl) return get_error(errid_MemLess,label);
								vmat_insert(vm,vl);
							}
							p++;
						}
						vlf=vlf->r;
					}
				}
			}
			break;
		case 3:
			// all
			if _oF(root->type&type_vlist)
			{
				// vlist
				vlf=root->v.v_vlist;
				if _oT(vlf) while(vlf->l) vlf=vlf->l;
				while(vlf)
				{
					if _oT(!vmat_find_index(vm,p))
					{
						vl=_func__get_index_new(vlf,p);
						if _oF(!vl) return get_error(errid_MemLess,label);
						vmat_insert(vm,vl);
					}
					p++;
					vlf=vlf->r;
				}
			}
			else
			{
				// vmat
				vmf=root->v.v_vmat;
				for(i=0;i<=vmf->mask;i++)
				{
					vlf=vmf->avl[i];
					if _oT(vlf) while(vlf->l) vlf=vlf->l;
					while(vlf)
					{
						if _oT(!vmat_find_index(vm,p))
						{
							vl=_func__get_index_new(vlf,p);
							if _oF(!vl) return get_error(errid_MemLess,label);
							vmat_insert(vm,vl);
						}
						p++;
						vlf=vlf->r;
					}
				}
			}
			break;
	}
	ret->v.v_long=p;
	return ret;
}

