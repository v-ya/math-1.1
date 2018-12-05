#include "socket.h"

#define	label_name(n) "[system.socket]." n

func(address)
{
	static char *label=label_name("address");
	static u32 type_2[2]={type_object,type_znum};
	static u32 type_inet[2]={type_znum,type_znum};
	var *obj,*vp;
	int af;
	u32 ipv4=-1;
	u16 port=0;
	if _oF(argc<2) return base->get_error(errid_FunArgvType,label);
	if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	obj=argv->v;
	argv=argv->r;
	af=argv->v->v.v_int;
	argv=argv->r;
	if _oF(!(obj->mode&auth_write)) return base->get_error(errid_VarNotWrite,label);
	switch(af)
	{
		case AF_INET:
			if _oF(argv)
			{
				if _oF(base->check_varlist(argv,argc-2,type_inet)) return base->get_error(errid_FunArgvType,label);
				ipv4=argv->v->v.v_int;
				argv=argv->r;
				if _oF(argv) port=argv->v->v.v_word;
			}
			vp=base->var_find(obj,SID_ADDR_IPV4);
			if _oF(vp && !(vp->mode&auth_retype)) return base->get_error(errid_VarNotWrite,label);
			if _oF(!base->create_uint(obj,SID_ADDR_IPV4,0,auth_read|auth_write,ipv4)) return NULL;
			vp=base->var_find(obj,SID_ADDR_PORT);
			if _oF(vp && !(vp->mode&auth_retype)) return base->get_error(errid_VarNotWrite,label);
			if _oF(!base->create_uword(obj,SID_ADDR_PORT,0,auth_read|auth_write,port)) return NULL;
			break;
	}
	base->var_save(obj);
	return obj;
}

func(socket)
{
	static char *label=label_name("socket");
	static u32 type_3[3]={type_znum,type_znum,type_znum};
	int af,type,ptc;
	if _oF(argc!=3) return base->get_error(errid_FunArgvType,label);
	if _oF(base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
	af=argv->v->v.v_int;
	argv=argv->r;
	type=argv->v->v.v_int;
	argv=argv->r;
	ptc=argv->v->v.v_int;
	ret->type=type_long|type_unsign;
	ret->v.v_long=0;
	switch(af)
	{
		case AF_INET:
			switch(type)
			{
				case SOCK_STREAM:
					switch(ptc)
					{
						case IPPROTO_TCP:
							goto _new;
					}
					break;
			}
			break;
	}
	return ret;
	_new:
	ret->v.v_long=new_sid(af,type,ptc,NULL);
	if _oF(!ret->v.v_long) return base->get_error(errid_MemLess,label);
	return ret;
}

func(close)
{
	static char *label=label_name("close");
	static u32 type_1[1]={type_znum};
	u64 sid;
	if _oF(argc!=1) return base->get_error(errid_FunArgvType,label);
	if _oF(base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	sid=argv->v->v.v_long;
	close_sid(sid);
	ret->type=type_void;
	ret->v.v_long=0;
	return ret;
}

func(bind)
{
	static char *label=label_name("bind");
	static u32 type_2[2]={type_znum,type_object};
	u64 sid;
	var *addr,*s;
	struct sockaddr *a;
	int sock,af,addrlen;
	
	if _oF(argc!=2) return base->get_error(errid_FunArgvType,label);
	if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	
	sid=argv->v->v.v_long;
	addr=argv->r->v;
	s=get_sid(sid);
	if _oF(!sid) goto Err;
	if _oF(!get_sock(s,&sock)) goto Err;
	if _oF(!get_af(s,&af)) goto Err;
	a=get_addr(addr,af,&addrlen);
	if _oF(!a)
	{
		if _oF(addrlen<0) return base->get_error(errid_FunArgvType,label);
		else return base->get_error(errid_MemLess,label);
	}
	ret->type=type_long;
	ret->v.v_long=bind(sock,a,addrlen);
	if _oT(ret->v.v_long>=0) set_addr(base->var_find(s,SID_LOCAL),af,a);
	free(a);
	return ret;
	Err:
	ret->type=type_long;
	ret->v.v_long=-1;
	return ret;
}

func(connect)
{
	static char *label=label_name("connect");
	static u32 type_2[2]={type_znum,type_object};
	u64 sid;
	var *addr,*s;
	struct sockaddr *a;
	int sock,af,addrlen;
	
	if _oF(argc!=2) return base->get_error(errid_FunArgvType,label);
	if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
	
	sid=argv->v->v.v_long;
	addr=argv->r->v;
	s=get_sid(sid);
	if _oF(!sid) goto Err;
	if _oF(!get_sock(s,&sock)) goto Err;
	if _oF(!get_af(s,&af)) goto Err;
	a=get_addr(addr,af,&addrlen);
	if _oF(!a)
	{
		if _oF(addrlen<0) return base->get_error(errid_FunArgvType,label);
		else return base->get_error(errid_MemLess,label);
	}
	ret->type=type_long;
	ret->v.v_long=connect(sock,a,addrlen);
	if _oT(ret->v.v_long>=0)
	{
		set_addr(base->var_find(s,SID_PEER),af,a);
		free(a);
		a=get_local_addr(sock,af,&addrlen);
		if _oF(!a) return base->get_error(errid_MemLess,label);
		set_addr(base->var_find(s,SID_LOCAL),af,a);
	}
	free(a);
	return ret;
	Err:
	ret->type=type_long;
	ret->v.v_long=-1;
	return ret;
}

/*
	send(sid,array-byte);
	send(sid,array-byte,size);
	send(sid,array-byte,at,size);
*/
func(send)
{
	static char *label=label_name("send");
	static u32 type_2[2]={type_znum,type_byte|type_onlyarray};
	static u32 type_3[3]={type_znum,type_byte|type_onlyarray,type_znum};
	static u32 type_4[4]={type_znum,type_byte|type_onlyarray,type_znum,type_znum};
	var *vp,*s;
	u64 sid,at,size;
	int sock;
	
	switch(argc)
	{
		case 2:
			if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
			sid=argv->v->v.v_long;
			argv=argv->r;
			vp=argv->v;
			at=0;
			size=vp->length;
			break;
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
			sid=argv->v->v.v_long;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			at=0;
			size=argv->v->v.v_long;
			break;
		case 4:
			if _oF(base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
			sid=argv->v->v.v_long;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			at=argv->v->v.v_long;
			argv=argv->r;
			size=argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
			break;
	}
	ret->type=type_long|type_unsign;
	if _oF(at>=vp->length)
	{
		ret->v.v_long=0;
		return ret;
	}
	if _oF((vp->length-at)<size) size=vp->length-at;
	// get sock
	s=get_sid(sid);
	if _oF(!s) goto Err;
	if _oF(!get_sock(s,&sock)) goto Err;
	ret->v.v_long=write(sock,vp->v.vp_byte+at,size);
	return ret;
	Err:
	ret->v.v_long=-1;
	return ret;
}

/*
	recv(sid,array-byte);
	recv(sid,array-byte,size);
	recv(sid,array-byte,at,size);
*/
func(recv)
{
	static char *label=label_name("recv");
	static u32 type_2[2]={type_znum,type_byte|type_onlyarray};
	static u32 type_3[3]={type_znum,type_byte|type_onlyarray,type_znum};
	static u32 type_4[4]={type_znum,type_byte|type_onlyarray,type_znum,type_znum};
	var *vp,*s;
	u64 sid,at,size;
	int sock;
	
	switch(argc)
	{
		case 2:
			if _oF(base->check_varlist(argv,2,type_2)) return base->get_error(errid_FunArgvType,label);
			sid=argv->v->v.v_long;
			argv=argv->r;
			vp=argv->v;
			at=0;
			size=vp->length;
			break;
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
			sid=argv->v->v.v_long;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			at=0;
			size=argv->v->v.v_long;
			break;
		case 4:
			if _oF(base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
			sid=argv->v->v.v_long;
			argv=argv->r;
			vp=argv->v;
			argv=argv->r;
			at=argv->v->v.v_long;
			argv=argv->r;
			size=argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
			break;
	}
	ret->type=type_long|type_unsign;
	if _oF(at>=vp->length)
	{
		ret->v.v_long=0;
		return ret;
	}
	if _oF((vp->length-at)<size) size=vp->length-at;
	// get sock
	s=get_sid(sid);
	if _oF(!s) goto Err;
	if _oF(!get_sock(s,&sock)) goto Err;
	ret->v.v_long=read(sock,vp->v.vp_byte+at,size);
	return ret;
	Err:
	ret->v.v_long=-1;
	return ret;
}

func(listen)
{
	
}

func(accept)
{
	
}

func(shutdown)
{
	
}


