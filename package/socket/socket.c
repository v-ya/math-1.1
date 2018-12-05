#include "socket.h"

char *author	=AUTHOR;
char *package	=PACKAGE;

var *_sid;
vmat *_sid_vm;

void init(void)
{
	_sid=base->create_vmat(root,SID,0,auth_read|auth_system);
	if _oF(!_sid)
	{
		throw_errid(errid_MemLess);
		return ;
	}
	_sid_vm=_sid->v.v_vmat;
}

void uini(void)
{
	clear_sid();
	base->var_delete(root,SID);
}

