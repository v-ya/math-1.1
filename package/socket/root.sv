#include "socket.h"

// 开始自动生成静态结构
//	name	type	length	auth	value

@.	vmat	0	r	{
	root	-export	.
	info	vlist	0	r	{
		author	string	0	r	AUTHOR
		package	string	0	r	PACKAGE
		version	vlist	0	r	{
			major	ubyte	0	r	PACKAGE_major
			minor	ubyte	0	r	PACKAGE_minor
			rev	ubyte	0	r	PACKAGE_revision
			value	uint	0	r	PACKAGE_value
		}
	}
	af	vlist	0	r	{
		// protofamily 协议族
		inet	sint	0	r	AF_INET
			// ipv4
	}
	type	vlist	0	r	{
		// socket 类型
		stream	sint	0	r	SOCK_STREAM
			// stream
	}
	ptc	vlist	0	r	{
		// protocol 具体的协议
		tcp	sint	0	r	IPPROTO_TCP
			// tcp
	}
	shut	vlist	0	r	{
		rd	ulong	0	r	SHUT_RD
		wr	ulong	0	r	SHUT_WR
		rdwr	ulong	0	r	SHUT_RDWR
	}
	address	fun	0	r	addr_fun(address)
		// var object = address(var object ,af.?, ...);
		// inet = address(var object ,af.inet[, znum ipv4[, znum port]]);
			// inet.{uint ipv4, uword port};
	socket	fun	0	r	addr_fun(socket)
		// ulong sid = socket(af.? ,type.? ,ptc.?);
	close	fun	0	r	addr_fun(close)
		// close(ulong sid);
	bind	fun	0	r	addr_fun(bind)
		// [-1|0] = bind(ulong sid, object address);
	connect	fun	0	r	addr_fun(connect)
		// [-1|0] = bind(ulong sid, object address);
	send	fun	0	r	addr_fun(send)
		// ulong send(sid,array);
		// ulong send(sid,array,size);
		// ulong send(sid,array,at,size);
		// ulong send(znum,byte[],znum,znum);
	recv	fun	0	r	addr_fun(recv)
		// ulong recv(sid,array);
		// ulong recv(sid,array,size);
		// ulong recv(sid,array,at,size);
		// ulong recv(znum,byte[],znum,znum);
	listen	fun	0	r	addr_fun(listen)
		// [-1|0] = listen(ulong sid, znum limit);
	accept	fun	0	r	addr_fun(accept)
		// ulong sid = accept(ulong sid);
	shutdown fun	0	r	addr_fun(shutdown)
		// [-1|0] = shutdown(ulong sid, shut.?);
}

