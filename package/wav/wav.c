#include "wav.h"
#include "wav.interface.h"
#include "wav.string.h"

char *author	=AUTHOR;
char *package	=PACKAGE;
extern var *type;

void init(void)
{
	root->inode=0;
	set_interface(&_if_vya_wav);
}

void uini(void)
{
	static char *label="uini";
	if _oF(type->inode>1) throw_error(error_takeup,label);
}



