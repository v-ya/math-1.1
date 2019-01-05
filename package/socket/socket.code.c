#include "socket.h"

// NULL fail
var* new_addr(var *addr, int af, u32 auth)
{
	switch(af)
	{
		case AF_INET:
			if _oF(!base->create_uint(addr,SID_ADDR_IPV4,0,auth,-1)) return NULL;
			if _oF(!base->create_uword(addr,SID_ADDR_PORT,0,auth,0)) return NULL;
			break;
	}
	return addr;
}

// 0 fail
u64 new_sid(int af, int type, int ptc, int *psock)
{
	u64 sid;
	int sock=-1;
	var *vp,*va;
	do
	{
		sid=base->get_sid();
	}
	while(base->vmat_find_index(_sid_vm,sid));
	
	vp=base->create_vlist(_sid,NULL,sid,auth_read);
	if _oF(!vp) goto Err;
	
	if _oT(psock)
	{
		*psock=sock=socket(af,type,ptc);
		if _oF(sock<0) goto Err;
	}
	
	if _oF(!base->create_sint(vp,SID_SOCK,0,auth_read,sock)) goto Err;
	if _oF(!base->create_sint(vp,SID_AF,0,auth_read,af)) goto Err;
	if _oF(!base->create_sint(vp,SID_TYPE,0,auth_read,type)) goto Err;
	if _oF(!base->create_sint(vp,SID_PTC,0,auth_read,ptc)) goto Err;
	
	if _oF(!(va=base->create_vlist(vp,SID_LOCAL,0,auth_read))) goto Err;
	if _oF(!new_addr(va,af,auth_read)) goto Err;
	if _oF(!(va=base->create_vlist(vp,SID_PEER,0,auth_read))) goto Err;
	if _oF(!new_addr(va,af,auth_read)) goto Err;
	
	return sid;
	Err:
	if _oF(sock>=0) close(sock);
	base->vmat_delete_index(_sid_vm,sid);
	return 0;
}

u64 copy_sid(u64 sid, var **v, int *swap)
{
	int sock,af,type,ptc;
	u32 ipv4;
	u16 port;
	var *vp,*vs,*va;
	
	vs=get_sid(sid);
	if _oF(!vs) return 0;
	do
	{
		sid=base->get_sid();
	}
	while(base->vmat_find_index(_sid_vm,sid));
	
	if _oF(!get_sock(vs,&sock)) goto Err;
	if _oF(swap)
	{
		af=*swap;
		*swap=sock;
		sock=af;
	}
	if _oF(!get_af(vs,&af)) goto Err;
	if _oF(!get_type(vs,&type)) goto Err;
	if _oF(!get_ptc(vs,&ptc)) goto Err;
	
	vp=base->create_vlist(_sid,NULL,sid,auth_read);
	if _oF(!vp) goto Err;
	
	if _oF(!base->create_sint(vp,SID_SOCK,0,auth_read,sock)) goto Err;
	if _oF(!base->create_sint(vp,SID_AF,0,auth_read,af)) goto Err;
	if _oF(!base->create_sint(vp,SID_TYPE,0,auth_read,type)) goto Err;
	if _oF(!base->create_sint(vp,SID_PTC,0,auth_read,ptc)) goto Err;
	
	if (!(va=base->var_find(vs,SID_LOCAL))) goto Err;
	get_addr_ipv4(va,&ipv4);
	get_addr_port(va,&port);
	if _oF(!(va=base->create_vlist(vp,SID_LOCAL,0,auth_read))) goto Err;
	if _oF(!new_addr(va,af,auth_read)) goto Err;
	set_addr_ipv4(va,ipv4);
	set_addr_port(va,port);
	if (!(va=base->var_find(vs,SID_PEER))) goto Err;
	get_addr_ipv4(va,&ipv4);
	get_addr_port(va,&port);
	if _oF(!(va=base->create_vlist(vp,SID_PEER,0,auth_read))) goto Err;
	if _oF(!new_addr(va,af,auth_read)) goto Err;
	set_addr_ipv4(va,ipv4);
	set_addr_port(va,port);
	
	if _oT(v) *v=vp;
	return sid;
	Err:
	base->vmat_delete_index(_sid_vm,sid);
	return 0;
}

// NULL fail
var* get_sid(u64 sid)
{
	vlist *vl;
	vl=base->vmat_find_index(_sid_vm,sid);
	if _oT(vl) return vl->v;
	else return NULL;
}

void close_sid(u64 sid)
{
	var *vp;
	int sock;
	vp=get_sid(sid);
	if _oT(vp && get_sock(vp,&sock))
	{
		base->vmat_delete_index(_sid_vm,sid);
		if _oT(sock>=0) close(sock);
	}
}

void clear_sid(void)
{
	u64 sid;
	vmat *vm;
	vlist *vl,*vt;
	var *vp;
	u32 i;
	int sock;
	vm=_sid_vm;
	for(i=0;i<=vm->mask;i++)
	{
		vl=vm->avl[i];
		vm->avl[i]=NULL;
		if _oF(vl) while(vl->l) vl=vl->l;
		while(vl)
		{
			sid=vl->head;
			vt=vl->r;
			vp=base->var_find(vl->v,SID_SOCK);
			if _oT(vp) sock=vp->v.v_int;
			else sock=-1;
			vl->l=vl->r=NULL;
			base->vlist_free(vl);
			if _oT(sock>=0) close(sock);
			vl=vt;
		}
	}
	vm->number=0;
}

var* get_sock(var *sid, int *sock)
{
	sid=base->var_find(sid,SID_SOCK);
	if _oF(!sid) return NULL;
	if _oF(!(sid->type&type_znum)) return NULL;
	if _oT(sock) *sock=sid->v.v_int;
	return sid;
}

void set_sock(var *sid, int sock)
{
	sid=base->var_find(sid,SID_SOCK);
	if _oT(sid && (sid->type&type_znum)) sid->v.v_long=sock;
}

var* get_af(var *sid, int *af)
{
	sid=base->var_find(sid,SID_AF);
	if _oF(!sid) return NULL;
	if _oF(!(sid->type&type_znum)) return NULL;
	if _oT(af) *af=sid->v.v_int;
	return sid;
}

var* get_type(var *sid, int *type)
{
	sid=base->var_find(sid,SID_TYPE);
	if _oF(!sid) return NULL;
	if _oF(!(sid->type&type_znum)) return NULL;
	if _oT(type) *type=sid->v.v_int;
	return sid;
}

var* get_ptc(var *sid, int *ptc)
{
	sid=base->var_find(sid,SID_PTC);
	if _oF(!sid) return NULL;
	if _oF(!(sid->type&type_znum)) return NULL;
	if _oT(ptc) *ptc=sid->v.v_int;
	return sid;
}

var* get_addr_ipv4(var *addr, u32 *ipv4)
{
	addr=base->var_find(addr,SID_ADDR_IPV4);
	if _oF(!addr) return NULL;
	if _oF(!(addr->type&type_znum)) return NULL;
	if _oT(ipv4) *ipv4=addr->v.v_int;
	return addr;
}

void set_addr_ipv4(var *addr, u32 ipv4)
{
	addr=base->var_find(addr,SID_ADDR_IPV4);
	if _oT(addr && (addr->type&type_znum))
	{
		addr->v.v_int=ipv4;
		base->var_fixvalue(addr);
	}
}

var* get_addr_port(var *addr, u16 *port)
{
	addr=base->var_find(addr,SID_ADDR_PORT);
	if _oF(!addr) return NULL;
	if _oF(!(addr->type&type_znum)) return NULL;
	if _oT(port) *port=addr->v.v_word;
	return addr;
}

void set_addr_port(var *addr, u16 port)
{
	addr=base->var_find(addr,SID_ADDR_PORT);
	if _oT(addr && (addr->type&type_znum))
	{
		addr->v.v_word=port;
		base->var_fixvalue(addr);
	}
}

struct sockaddr* get_local_addr(int sock, int af, int *addrlen)
{
	struct sockaddr *a;
	int len;
	switch (af)
	{
		case AF_INET:
			a=malloc(sizeof(struct sockaddr_in));
			if _oF(!a) goto Err_mem;
			if _oT(addrlen) *addrlen=sizeof(struct sockaddr_in);
			getsockname(sock,a,&len);
			return a;
	}
	Err_mem:
	if _oT(addrlen) *addrlen=0;
	return NULL;
}

struct sockaddr* get_peer_addr(int sock, int af, int *addrlen)
{
	struct sockaddr *a;
	int len;
	switch (af)
	{
		case AF_INET:
			a=malloc(sizeof(struct sockaddr_in));
			if _oF(!a) goto Err_mem;
			if _oT(addrlen) *addrlen=sizeof(struct sockaddr_in);
			getpeername(sock,a,&len);
			return a;
	}
	Err_mem:
	if _oT(addrlen) *addrlen=0;
	return NULL;
}

struct sockaddr* get_addr(var *addr, int af, int *addrlen)
{
	struct sockaddr *a=NULL;
	u32 ipv4;
	u16 port;
	switch (af)
	{
		case AF_INET:
			if _oT(addr)
			{
				if _oF(!get_addr_ipv4(addr,&ipv4)) goto Err;
				if _oF(!get_addr_port(addr,&port)) goto Err;
			}
			a=malloc(sizeof(struct sockaddr_in));
			if _oF(!a) goto Err_mem;
			if _oT(addrlen) *addrlen=sizeof(struct sockaddr_in);
			if _oT(addr)
			{
				((struct sockaddr_in*)a)->sin_family=AF_INET;
				((struct sockaddr_in*)a)->sin_port=htons(port);
				((struct sockaddr_in*)a)->sin_addr.s_addr=htonl(ipv4);
			}
			return a;
	}
	Err:
	if _oT(addrlen) *addrlen=-1;
	return NULL;
	Err_mem:
	if _oF(a) free(a);
	if _oT(addrlen) *addrlen=0;
	return NULL;
}

void set_addr(var *addr, int af, struct sockaddr* a)
{
	if _oF(!addr) return ;
	switch (af)
	{
		case AF_INET:
			set_addr_ipv4(addr,ntohl(((struct sockaddr_in*)a)->sin_addr.s_addr));
			set_addr_port(addr,ntohs(((struct sockaddr_in*)a)->sin_port));
			return ;
	}
}


