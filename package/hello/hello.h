#ifndef	_package_test_hello_h_
#define	_package_test_hello_h_

#include "../package.h"
#define AUTHOR			"test"
#define PACKAGE			"hello"
#define PACKAGE_major		1
#define PACKAGE_minor		0
#define PACKAGE_revision	0
#define PACKAGE_value		(PACKAGE_major<<16|PACKAGE_minor<<8|PACKAGE_revision)

func(hello);

#endif

