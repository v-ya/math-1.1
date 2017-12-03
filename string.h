#ifndef	_string_h_
#define	_string_h_

#define is_Name(c)	((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c=='_'))
#define is_name(c)	((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c=='_')||(c>='0'&&c<='9'))

#define is_space(c)	(c==' '||c=='\t'||c=='\n'||c=='\r')

#ifndef __nosym__

u32 vname_check(char *name);
u32 vname_gen(unsigned char *name);
u64 vname_head(unsigned char *name);
char* skip_string(char *exp);
char* get_string(char *exp, char **pexp);
u64 get_ascii(char *exp, char **pexp);
char* skip_code(char *exp);
char* get_code(char *exp, char **pexp);
char* get_nextcol(char *exp);
char* get_nextsem(char *exp);
char* skip_string_last(char *exp, char *text);
char* skip_code_last(char *exp, char *text);
char* get_lastsem(char *exp, char *text);
u32 cmp_label(char *exp, char *label);
char* skip_note(char *exp);

#endif

#endif

