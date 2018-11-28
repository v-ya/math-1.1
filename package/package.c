#include "package.h"

interface_base *base;
static void **interface;
static var *error=NULL;

var* get_error(char *info, char *label)
{
	var *vp;
	vp=base->var_alloc(tlog_string,leng_no);
	if _oF(!vp) goto Err_mem;
	vp->v.v_string=malloc(strlen(label)+strlen(info)+3);
	if _oF(!vp->v.v_string)
	{
		free(vp);
		goto Err_mem;
	}
	vp->mode=auth_read|free_pointer;
	sprintf(vp->v.v_string,"%s: %s",label,info);
	base->get_error(errid_IntError,NULL);
	base->set_error(vp,vp->v.v_string);
	return base->get_error(errid_IntError,NULL);
	Err_mem:
	return base->get_error(errid_MemLess,NULL);
}

void throw_error(char *info, char *label)
{
	error=get_error(info,label);
}

void set_interface(void *_if)
{
	*interface=_if;
}

// pf
u32 package_ckeck(u32 r)
{
	return r^IMPORT_CHECK;
}

void package_name(char **a, char **p)
{
	*a=author;
	*p=package;
}

var* package_init(void *Base, void **Interface)
{
	base=(interface_base*)Base;
	if _oF(base->version!=InterfaceBase_Version) return NULL;
	interface=Interface;
	error=NULL;
	root->inode=0;
	init();
	if _oF(error) return error;
	else if _oF(root->inode!=0) return base->get_error(errid_IntError,NULL);
	else return root;
}

var* package_uini(void)
{
	uini();
	if _oF(error) return error;
	*interface=NULL;
	return NULL;
}

