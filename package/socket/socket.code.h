#ifndef	_package_system_socket_code_h_
#define	_package_system_socket_code_h_

var* new_addr(var *addr, int af, u32 auth);
u64 new_sid(int af, int type, int ptc, int *psock);
var* get_sid(u64 sid);
void close_sid(u64 sid);
void clear_sid(void);
var* get_sock(var *sid, int *sock);
var* get_af(var *sid, int *af);
var* get_addr_ipv4(var *addr, u32 *ipv4);
void set_addr_ipv4(var *addr, u32 ipv4);
var* get_addr_port(var *addr, u16 *port);
void set_addr_port(var *addr, u16 port);
struct sockaddr* get_local_addr(int sock, int af, int *addrlen);
struct sockaddr* get_peer_addr(int sock, int af, int *addrlen);
struct sockaddr* get_addr(var *addr, int af, int *addrlen);
void set_addr(var *addr, int af, struct sockaddr* a);

#endif

