#ifndef	_sbuf_h_
#define	_sbuf_h_


#ifndef __nosym__

var* sbuf_alloc(void);
void sbuf_free(void);
var* sbuf_get(void);
var* sbuf_expand(void);
var* sbuf_reduce(void);
var* sbuf_sprintf(char *format, vlist *vl);
char* get_name(char *exp, char **pexp);

#endif

#endif

