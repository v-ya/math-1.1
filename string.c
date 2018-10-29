#include "main.h"

u32 vname_check(char *name)
{
	char c;
	c=*name;
	if _oT(is_Name(c))
	{
		for(c=*(++name);c!=0;c=*(++name))
			if _oF(!is_name(c)) return 0;
		return 1;
	}
	else return 0;
}

u64 vname_gen(unsigned char *name)
{
	u32 ngen=0;
	while(*name)
		ngen+=*(name++);
	ngen&=0xff;
	return ngen;
}

u64 vhead_gen(u64 head)
{
	head=(head>>32)^head;
	head=(head>>16)^head;
	head=(head>>8)^head;
	return head&0xff;
}

u64 vname_head(unsigned char *name)
{
	u32 i;
	u64 head=0;
	char *cp=(char *) &head;
	for(i=0;i<8;i++)
	{
		if _oF(name[i]==0) return head;
		cp[7-i]=name[i];
	}
	return head;
}

char* skip_string(char *exp)
{
	char c;
	if _oT(*exp=='\"') c='\"';
	else if _oT(*exp=='\'') c='\'';
	else return exp;
	exp++;
	while(*exp)
	{
		if _oF(*exp==c) break;
		if _oF(*exp=='\\')
		{
			exp++;
			if _oF(*exp==c)
			{
				exp++;
				continue;
			}
			continue;
		}
		exp++;
	}
	if _oT(*exp) exp++;
	return exp;
}

char* get_string(char *exp, char **pexp)
{
	char *save;
	u32 size=0;
	if _oT(*exp=='\"')
	{
		save=exp;
		exp++;
		while(*exp)
		{
			if _oF(*exp=='\\')
			{
				switch(*(++exp))
				{
					case 'n':
					case 'r':
					case 't':
					case 'v':
					case '\\':
					case '\"':
					case '\'':
						exp++;
						size++;
						continue;
					case 'x':
						exp++;
						if _oT((*exp>='0'&&*exp<='9')||(*exp>='a'&&*exp<='f')||(*exp>='A'&&*exp<='F'))
						{
							exp++;
							if _oT((*exp>='0'&&*exp<='9')||(*exp>='a'&&*exp<='f')||(*exp>='A'&&*exp<='F'))
								exp++;
						}
						size++;
						continue;
					default:
						if _oT(*exp>='0'&&*exp<='7')
						{
							exp++;
							if _oT(*exp>='0'&&*exp<='7')
							{
								exp++;
								if _oT(*exp>='0'&&*exp<='7') exp++;
							}
							size++;
							continue;
						}
						continue;
				}
			}
			else if _oF(*exp=='\"') break;
			exp++;
			size++;
		}
		size++;
		exp=save;
		save=malloc(size);
		if _oF(save==NULL)
		{
			if _oT(pexp) *pexp=exp;
			return NULL;
		}
		exp++;
		size=0;
		while(*exp)
		{
			if _oF(*exp=='\\')
			{
				switch(*(++exp))
				{
					case 'n':
						exp++;
						save[size++]='\n';
						continue;
					case 'r':
						exp++;
						save[size++]='\r';
						continue;
					case 't':
						exp++;
						save[size++]='\t';
						continue;
					case 'v':
						exp++;
						save[size++]='\v';
						continue;
					case '\\':
						exp++;
						save[size++]='\\';
						continue;
					case '\"':
						exp++;
						save[size++]='\"';
						continue;
					case '\'':
						exp++;
						save[size++]='\'';
						continue;
					case 'x':
						exp++;
						save[size]='x';
						if _oT((*exp>='0'&&*exp<='9')||(*exp>='a'&&*exp<='f')||(*exp>='A'&&*exp<='F'))
						{
							if _oT(*exp>='0'&&*exp<='9') save[size]=*exp-'0';
							else if _oT(*exp>='a'&&*exp<='f') save[size]=*exp-('a'-10);
							else save[size]=*exp-('A'-10);
							exp++;
							if _oT((*exp>='0'&&*exp<='9')||(*exp>='a'&&*exp<='f')||(*exp>='A'&&*exp<='F')) 
							{
								save[size]<<=4;
								if _oT(*exp>='0'&&*exp<='9') save[size]|=*exp-'0';
								else if _oT(*exp>='a'&&*exp<='f') save[size]|=*exp-('a'-10);
								else save[size]|=*exp-('A'-10);
								exp++;
							}
						}
						size++;
						continue;
					default:
						if _oT(*exp>='0'&&*exp<='7')
						{
							save[size]=*exp-'0';
							exp++;
							if _oT(*exp>='0'&&*exp<='7')
							{
								save[size]<<=3;
								save[size]|=*exp-'0';
								exp++;
								if _oT(*exp>='0'&&*exp<='7')
								{
									save[size]<<=3;
									save[size]|=*exp-'0';
									exp++;
								}
							}
							size++;
							continue;
						}
						continue;
				}
			}
			else if _oF(*exp=='\"') break;
			save[size++]=*(exp++);
		}
		save[size]=0;
		if _oT(pexp)
		{
			if _oT(*exp=='\"') exp++;
			*pexp=exp;
			return save;
		}
		return save;
	}
	else
	{
		if _oT(pexp) *pexp=exp;
		return NULL;
	}
}

u64 get_ascii(char *exp, char **pexp)
{
	u64 ascii=0;
	char c=0;
	if _oT(*exp=='\'')
	{
		exp++;
		while(*exp)
		{
			ascii<<=8;
			ascii|=c;
			if _oF(*exp=='\\')
			{
				
				switch(*(++exp))
				{
					case 'n':
						exp++;
						c='\n';
						continue;
					case 'r':
						exp++;
						c='\r';
						continue;
					case 't':
						exp++;
						c='\t';
						continue;
					case 'v':
						exp++;
						c='\v';
						continue;
					case '\\':
						exp++;
						c='\\';
						continue;
					case '\"':
						exp++;
						c='\"';
						continue;
					case '\'':
						exp++;
						c='\'';
						continue;
					case 'x':
						exp++;
						c='x';
						if _oT((*exp>='0'&&*exp<='9')||(*exp>='a'&&*exp<='f')||(*exp>='A'&&*exp<='F'))
						{
							if _oT(*exp>='0'&&*exp<='9') c=*exp-'0';
							else if _oT(*exp>='a'&&*exp<='f') c=*exp-('a'-10);
							else c=*exp-('A'-10);
							exp++;
							if _oT((*exp>='0'&&*exp<='9')||(*exp>='a'&&*exp<='f')||(*exp>='A'&&*exp<='F')) 
							{
								c<<=4;
								if _oT(*exp>='0'&&*exp<='9') c|=*exp-'0';
								else if _oT(*exp>='a'&&*exp<='f') c|=*exp-('a'-10);
								else c|=*exp-('A'-10);
								exp++;
							}
						}
						continue;
					default:
						if _oT(*exp>='0'&&*exp<='7')
						{
							c=*exp-'0';
							exp++;
							if _oT(*exp>='0'&&*exp<='7')
							{
								c<<=3;
								c|=*exp-'0';
								exp++;
								if _oT(*exp>='0'&&*exp<='7')
								{
									c<<=3;
									c|=*exp-'0';
									exp++;
								}
							}
							continue;
						}
						continue;
				}
			}
			else if _oF(*exp=='\'') break;
			c=*(exp++);
		}
		if _oT(pexp)
		{
			if _oT(*exp=='\'') exp++;
			*pexp=exp;
			return ascii;
		}
		return ascii;
	}
	else
	{
		if _oT(pexp) *pexp=exp;
		return ascii;
	}
}

char* skip_code(char *exp)
{
	if _oF(*exp!='{') return exp;
	exp++;
	Loop:
	switch(*exp)
	{
		case 0:
		case '}':
			if _oT(*exp) exp++;
			return exp;
		case '\"':
		case '\'':
			exp=skip_string(exp);
			goto Loop;
		case '{':
			exp=skip_code(exp);
			goto Loop;
		default:
			exp++;
			goto Loop;
	}
}

char* get_code(char *exp, char **pexp)
{
	char *save,*t;
	u32 size=0,ii;
	if _oT(*exp=='{')
	{
		save=exp;
		exp++;
		ii=1;
		while(is_space(*exp)) exp++;
		while(ii)
		{
			switch(*exp)
			{
				case 0:
					ii=0;
					size++;
					continue;
				case '\n':
					exp++;
					while(*exp==' '||*exp=='\t') exp++;
					size++;
					continue;
				case ' ':
				case '\t':
					while(*exp==' '||*exp=='\t') exp++;
					if _oF(*exp=='\r')
					{
						exp++;
						if _oT(*exp=='\n') exp++;
					}
					else if _oF(*exp=='\n') exp++;
					size++;
					continue;
				case '\"':
				case '\'':
					t=skip_string(exp);
					size+=(size_t)t-(size_t)exp;
					exp=t;
					continue;
				case '{':
					exp++;
					ii++;
					size++;
					continue;
				case '}':
					exp++;
					ii--;
					size++;
					continue;
				default:
					exp++;
					size++;
					continue;
			}
		}
		exp=save;
		save=malloc(size);
		if _oF(save==NULL)
		{
			if _oT(pexp) *pexp=exp;
			return NULL;
		}
		exp++;
		size=0;
		ii=1;
		while(is_space(*exp)) exp++;
		while(ii)
		{
			switch(*exp)
			{
				case 0:
					ii=0;
					save[size]=0;
					continue;
				case '\n':
					exp++;
					while(*exp==' '||*exp=='\t') exp++;
					save[size++]='\n';
					continue;
				case ' ':
				case '\t':
					while(*exp==' '||*exp=='\t') exp++;
					if _oF(*exp=='\r')
					{
						exp++;
						if _oT(*exp=='\n')
						{
							exp++;
							save[size++]='\n';
						}
					}
					else if _oF(*exp=='\n')
					{
						exp++;
						save[size++]='\n';
					}
					else save[size++]=' ';
					continue;
				case '\"':
				case '\'':
					t=skip_string(exp);
					while(exp<t)
						save[size++]=*(exp++);
					continue;
				case '{':
					exp++;
					ii++;
					save[size++]='{';
					continue;
				case '}':
					exp++;
					ii--;
					if _oT(ii==0) save[size]=0;
					else save[size++]='}';
					continue;
				default:
					save[size++]=*(exp++);
					continue;
			}
		}
		if _oT(pexp)
		{
			*pexp=exp;
			return save;
		}
		return save;
	}
	else
	{
		if _oT(pexp) *pexp=exp;
		return NULL;
	}
}

char* get_nextcol(char *exp)
{
	u32 b1=0,b2=0;
	Loop:
	switch(*exp)
	{
		case ':':
		case ',':
			if _oF(b1||b2)
			{
				exp++;
				goto Loop;
			}
		case ';':
		case 0:
			return exp;
		case '\"':
		case '\'':
			exp=skip_string(exp);
			goto Loop;
		case '{':
			exp=skip_code(exp);
			goto Loop;
		case '(':
			b1++;
			exp++;
			goto Loop;
		case '[':
			b2++;
			exp++;
			goto Loop;
		case ')':
			if _oF(b1==0) return exp;
			b1--;
			exp++;
			goto Loop;
		case ']':
			if _oF(b2==0) return exp;
			b2--;
			exp++;
			goto Loop;
		default:
			exp++;
			goto Loop;
	}
}

char* get_nextsem(char *exp)
{
	Loop:
	switch(*exp)
	{
		case 0:
		case ';':
			return exp;
		case '\"':
		case '\'':
			exp=skip_string(exp);
			goto Loop;
		case '{':
			exp=skip_code(exp);
			goto Loop;
		default:
			exp++;
			goto Loop;
	}
}

char* skip_string_last(char *exp, char *text)
{
	char c;
	if _oT(*exp=='\"') c='\"';
	else if _oT(*exp=='\'') c='\'';
	else return exp;
	if _oT(exp>text) exp--;
	while(exp>text)
	{
		if _oF(*exp==c)
		{
			if _oF(*(exp-1)=='\\')
			{
				exp--;
				if _oF(exp==text) return exp;
				else if _oF(*(exp-1)=='\\') return exp;
			}
			else break;
		}
		exp--;
	}
	if _oT(exp>text) exp--;
	return exp;
}

char* skip_code_last(char *exp, char *text)
{
	if _oF(*exp!='}') return exp;
	if _oT(exp>text) exp--;
	else return exp;
	Loop:
	if _oF(exp<=text) return text;
	switch(*exp)
	{
		case '{':
			exp--;
			return exp;
		case '\"':
		case '\'':
			exp=skip_string_last(exp,text);
			goto Loop;
		case '}':
			exp=skip_code_last(exp,text);
			goto Loop;
		default:
			exp--;
			goto Loop;
	}
}

char* get_lastsem(char *exp, char *text)
{
	Loop:
	if _oF(exp<=text) return text;
	switch(*exp)
	{
		case ';':
			return exp;
		case '\"':
		case '\'':
			exp=skip_string_last(exp,text);
			goto Loop;
		case '{':
			exp=skip_code_last(exp,text);
			goto Loop;
		default:
			exp--;
			goto Loop;
	}
}

u32 cmp_label(char *exp, char *label)
{
	if _oF(!is_name(*label)) return 1;
	while(is_name(*label))
	{
		if _oF(*exp!=*label) return 1;
		exp++;
		label++;
	}
	if _oT(*exp==';') return 0;
	else return 1;
}

char* skip_note(char *exp)
{
	/***
	注释：
		# 单行注释
		###
			块注释
		###
		PS: #label 为标签
	***/
	if _oF(*exp!='#') return exp;
	exp++;
	if _oF(*exp=='#'&&*(++exp)=='#')
	{
		while(1)
		{
			while(*exp&&*exp!='#') exp++;
			if _oF(*exp==0) return exp;
			if _oF(*(++exp)=='#'&&*(++exp)=='#') break;
		}
		exp++;
		return exp;
	}
	else
	{
		while(*exp&&*exp!='\n') exp++;
		if _oT(*exp) exp++;
		return exp;
	}
}


