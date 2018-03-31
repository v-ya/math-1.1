#include "wav.h"
#include "wav.interface.h"
#include "wav.string.h"

char *author	=AUTHOR;
char *package	=PACKAGE;

void init(void)
{
	root->inode=0;
	set_interface(&_if_vya_wav);
}

void uini(void)
{
	static char *label="uini";
	if _oF(type->inode>1) goto Err_takeup;
	if _oF(type_spwav->inode>1) goto Err_takeup;
	return ;
	Err_takeup:
	throw_error(error_takeup,label);
}



