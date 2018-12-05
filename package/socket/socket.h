#ifndef	_package_system_socket_h_
#define	_package_system_socket_h_

#include <sys/socket.h>
#include <netinet/in.h>
#include "../package.h"
#include "socket.code.h"
#include "socket.func.h"
#define AUTHOR			"system"
#define PACKAGE			"socket"
#define PACKAGE_major		0
#define PACKAGE_minor		0
#define PACKAGE_revision	1
#define PACKAGE_value		(PACKAGE_major<<16|PACKAGE_minor<<8|PACKAGE_revision)

#define SID		"sid"
#define SID_SOCK	"sock"
#define SID_AF		"af"
#define SID_TYPE	"type"
#define SID_PTC		"ptc"
#define SID_LOCAL	"local"
#define SID_PEER	"peer"
#define SID_ADDR_IPV4	"ipv4"
#define SID_ADDR_PORT	"port"

extern var *_sid;
extern vmat *_sid_vm;

#endif

