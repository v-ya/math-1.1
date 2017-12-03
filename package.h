#ifndef	_package_h_
#define	_package_h_

#define IMPORT_CHECK	0x12345678

#ifndef __nosym__

var* package_import(char *path, char **name);
var* package_remove(char *name);

#endif

#endif

