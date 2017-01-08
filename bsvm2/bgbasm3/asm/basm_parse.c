/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

/*
Goals:
Parse and process strings of assembler commands;
Parsed strings are sent to the underlying procedural assembler.

Syntax:
'name:',	label;
'opcode [arg[, arg]]'

arg:
immed;
reg;
mem.

mem:
	[reg]
	[reg+reg]
	[reg+reg*scale]
	[reg+reg+disp]
	[reg+reg*scale+disp]
	[reg+disp]

	[label]
	[label+reg]
	[label+reg*scale]
	[label+reg+disp]
	[label+reg*scale+disp]
	[label+disp]

	[addr]
	[addr+reg]
	[addr+reg*scale]
	[addr+reg+disp]
	[addr+reg*scale+disp]
	[addr+disp]


	[<reg|label|addr|const>(+reg(*scale))(+<disp|const>)]
*/

// #include <bgbasm.h>

#if 1
extern char *basm_ops_x86[];
extern int basm_opidx_x86[];
extern int basm_opnums_x86[];

extern char basm_opcat_x86[];
extern char *basm_opdecl_x86[];

extern char *basm_regs_x86[];
extern int basm_regidx_x86[];
#endif

#if 1
extern char **basm_ops;
extern int *basm_opidx;
extern int *basm_opnums;

extern char *basm_opcat;
extern char **basm_opdecl;

extern char **basm_regs;
extern int *basm_regidx;
#endif

#ifndef BASM_SHAVEMEM
#define BASM_SZSTRHASH 4096
#define BASM_SZSTRINIT (1<<20)
#define BASM_SZSTRLIM (BASM_SZSTRINIT-4096)
#define BASM_SZSTRVINIT 16384
#else
#define BASM_SZSTRHASH 256
#define BASM_SZSTRINIT (1<<14)
#define BASM_SZSTRLIM (BASM_SZSTRINIT-256)
#define BASM_SZSTRVINIT 1024
#endif

char *basm_strtab;	//strings table
char *basm_strtabe;	//strings table end

// char *basm_strhash[BASM_SZSTRHASH];

char **basm_str_varr;
int *basm_str_carr;
int basm_str_num;
int basm_str_max;
int basm_str_hash[BASM_SZSTRHASH];

char *basm_psop_name[1024];
char *(*basm_psop_fcn[1024])(BASM_Context *ctx, char *op, char **str);
char *(*basm_psop2_fcn[256])(BASM_Context *ctx, char *op, char **str);
int basm_psop_num=0;
int basm_psop2_num=0;

BASM_API char *BASM_ComposeLinkMetaName(char *name, char **args);
BASM_API char *BASM_ComposeLinkNotifyName(char *name, char **args);

BASM_API void BASM_RegisterSpecialOp(char *name,
	char *(*fcn)(BASM_Context *ctx, char *op, char **str))
{
	int i;

	if(!name)
	{
		i=basm_psop2_num++;
		basm_psop2_fcn[i]=fcn;
		return;
	}

	i=basm_psop_num++;
	basm_psop_name[i]=basm_strdup(name);
	basm_psop_fcn[i]=fcn;
}

#if 1
char *basm_strdup_alloc(char *str)
{
	char *s, *t;
	int i, hi;

	if(!basm_strtab)
	{
		basm_strtab=(char *)malloc(BASM_SZSTRINIT);
		basm_strtabe=basm_strtab+1;
	}

	if((str>=basm_strtab) && (str<basm_strtabe))
		return(str);

	if(!str)return(NULL);
	if(!*str)return(basm_strtab);

	s=basm_strtabe;
	strcpy(s, str);
	basm_strtabe=s+strlen(s)+1;

	i=basm_strtabe-basm_strtab;
	if(i>=BASM_SZSTRLIM)
	{
		//new string table
		basm_strtab=(char *)malloc(BASM_SZSTRINIT);
		basm_strtabe=basm_strtab+1;
	}

	return(s);
}

BASM_API int basm_strdup_i(char *str)
{
	char *s, *t;
	int i, hi;

	if(!str)return(0);

#if 1
	if(!basm_str_varr)
	{
		for(i=0; i<BASM_SZSTRHASH; i++)basm_str_hash[i]=0;
		basm_str_varr=malloc(BASM_SZSTRVINIT*sizeof(char *));
		basm_str_carr=malloc(BASM_SZSTRVINIT*sizeof(int));

		basm_str_varr[0]=NULL;
		basm_str_carr[0]=0;
		basm_str_num=1;
		basm_str_max=BASM_SZSTRVINIT;
	}
#endif

	s=str; hi=0;
	while(*s)hi=(hi*251)+(*s++);
	hi&=BASM_SZSTRHASH-1;

	i=basm_str_hash[hi];
	while(i)
	{
		if(!strcmp(basm_str_varr[i], str))
			return(i);
		i=basm_str_carr[i];
	}

	if((basm_str_num+1)>=basm_str_max)
	{
		i=basm_str_max+(basm_str_max>>1);
		basm_str_varr=realloc(basm_str_varr, i*sizeof(char *));
		basm_str_carr=realloc(basm_str_carr, i*sizeof(int));
		basm_str_max=i;
	}

	t=basm_strdup_alloc(str);

	i=basm_str_num++;
	basm_str_varr[i]=t;
	basm_str_carr[i]=basm_str_hash[hi];
	basm_str_hash[hi]=i;

	return(i);
}

BASM_API char *basm_strtab_i(int i)
	{ return(basm_str_varr[i]); }

BASM_API char *basm_strdup(char *str)
{
	int i;

	if(!str)return(NULL);
	i=basm_strdup_i(str);
	return(basm_str_varr[i]);
}
#endif

BASM_API int basm_strildup_i(char *str)
{
	char *s, *t, *b;
	int i;

	b=basm_ralloc(strlen(str)+1);

	s=str; t=b;
	while(*s)
	{
		i=*s++;
		if((i>='A') && (i<='Z'))
			i=i-'A'+'a';
		*t++=i;
	}
	*t++=0;

	return(basm_strdup_i(b));
}

BASM_API void *basm_ralloc(int sz)
{
	static char *buf=NULL;
	static int pos=0;
	char *s;

	if(!buf)buf=malloc(1<<16);
	if((pos+sz)>=(1<<16))pos=0;

	s=buf+pos;
	pos=(pos+sz+7)&(~7);
	return(s);
}

BASM_API char *basm_rstrdup(char *str)
{
	char *t;
	t=basm_ralloc(strlen(str)+1);
	strcpy(t, str);
	return(t);
}

BASM_API int basm_strisreal(char *str)
{
	char *s;

	s=str;
	if((s[0]=='0') && (s[1]=='x'))
		return(0);
	while(*s)
	{
		if(*s=='.')break;
		if(*s=='e')break;
		if(*s=='E')break;
		s++;
	}
	if(*s)return(1);
	return(0);
}

u32 basm_atof_u32(char *str)
{
	float f;
	f=atof(str);
	return(*(u32 *)(&f));
}

u64 basm_atof_u64(char *str)
{
	double f;
	f=atof(str);
	return(*(u64 *)(&f));
}

u32 basm_atof_u32_sg(char *str, int sg)
{
	float f;
	f=atof(str)*sg;
	return(*(u32 *)(&f));
}

u64 basm_atof_u64_sg(char *str, int sg)
{
	double f;
	f=atof(str)*sg;
	return(*(u64 *)(&f));
}


s64 basm_atoi(char *str)
{
	long long l;
	char *s;
	int i, sg;

	s=str;
	if((s[0]=='0') && (s[1]=='x'))
	{
		l=0; s+=2;
		while(*s)
		{
			l*=16;
			if((*s>='0') && (*s<='9'))l+=*s-'0';
			if((*s>='A') && (*s<='F'))l+=*s-'A'+10;
			if((*s>='a') && (*s<='f'))l+=*s-'a'+10;
			s++;
		}

		return(l);
	}

	if(s[0]=='0')
	{
		l=0; s++;
		while(*s)
		{
			l*=8;
			if((*s>='0') && (*s<='7'))l+=*s-'0';
				else break;
			s++;
		}

		return(l);
	}

	while(s[1])s++;
	if((*s=='b') || (*s=='B'))
	{
		l=0; s=str;
		while(*s)
		{
			l*=2;
			if((*s>='0') && (*s<='1'))l+=*s-'0';
				else break;
			s++;
		}

		return(l);
	}

	if((*s=='h') || (*s=='H'))
	{
		l=0; s=str;
		while(*s)
		{
			l*=16;
			if((*s>='0') && (*s<='9'))
				{ l+=*s-'0'; }
			else if((*s>='A') && (*s<='F'))
				{ l+=*s-'A'+10; }
			else if((*s>='a') && (*s<='f'))
				{ l+=*s-'a'+10; }
			else break;
			s++;
		}

		return(l);
	}


	s=str;
	l=0; sg=0;
	if(*s=='-') { sg=1; s++; }
	while(*s)
	{
		l*=10;
		if((*s>='0') && (*s<='9')) { l+=*s-'0'; }
			else break;
		s++;
	}
	if(sg)l=-l;

	return(l);
}

int basm_stricmp(char *s1, char *s2)
{
	int i, j;

	if(s1==s2)return(0);

	while(*s1 && *s2)
	{
		i=((*s1>='A') && (*s1<='Z'))?(*s1-'A'+'a'):(*s1);
		j=((*s2>='A') && (*s2<='Z'))?(*s2-'A'+'a'):(*s2);
		if(i<j)return(-1);
		if(i>j)return(1);
		s1++; s2++;
	}

	if(*s1)return(1);
	if(*s2)return(-1);
	return(0);
}


BASM_API char *BASM_Parse_EatWhiteOnly(char *s)
{
	while(*s && (*s<=' '))s++;
	return(s);
}

BASM_API char *BASM_Parse_EatWhite(char *s)
{
	char *is;
	int i;

	is=s;
	while(*s && (*s<=' '))s++;

#if 0
	if((s!=is) && (s[0]==';'))
	{
		while(*s && (*s!='\n'))s++;
		s=BASM_Parse_EatWhite(s);
		return(s);
	}
#endif

	if((s[0]=='/') && (s[1]=='/'))
	{
		while(*s && (*s!='\n'))s++;
		s=BASM_Parse_EatWhite(s);
		return(s);
	}
	if((s[0]=='/') && (s[1]=='*'))
	{
		s+=2;
		i=1;
		while(*s && i)
		{
			if((s[0]=='/') && (s[1]=='*'))
			{
				s+=2;
				i++;
				continue;
			}
			if((s[0]=='*') && (s[1]=='/'))
			{
				s+=2;
				i--;
				continue;
			}
			s++;
		}
		s=BASM_Parse_EatWhite(s);
		return(s);
	}

	return(s);
}

BASM_API int BASM_Parse_IsWhiteOnly(char *s)
{
	while((*s==' ') || (*s=='\t'))
		s++;

	if((s[0]=='/') && (s[1]=='/'))return(0);
	if((s[0]=='/') && (s[1]=='*'))return(0);
	if((s[0]=='*') && (s[1]=='/'))return(0);

	if(*s>' ')return(1);
	if(*s==' ')return(1);
	if(*s=='\t')return(1);

	return(0);
}

BASM_API int BASM_Parse_IsLineBreak(char *se)
{
	char *s, *b, *t;
	int i, j;

	s=se;
	while(*s && (*s<=' '))
	{
		if(*s=='\r')return(1);
		if(*s=='\n')return(1);
		s++;
	}
	if(!*s)return(1);

	return(0);
}


BASM_API int BASM_Parse_OpChar(int c)
{
	int i;

	switch(c)
	{
	case '~': case '!': case '@': case '#':
	case '$': case '%': case '^': case '&':
	case '*': case '-': case '=': case '+':
	case '\\': case '|':
	case ':':

	case '<': case '>': case '?': case '/':
		i=1;
		break;

	default:
		i=0;
		break;
	}
	return(i);
}

BASM_API int BASM_Parse_PeekChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(i>=0xFC)
	{
		i=(i&1)<<30;
		i+=((*s++)&63)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF8)
	{
		i=(i&3)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF0)
	{
		i=(i&7)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xE0)
	{
		i=(i&15)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xC0)
	{
		i=(i&31)<<6;
		i+=(*s++)&63;
	}else if(i>=0x80)
	{
//		continue;
	}

	return(i);
}

BASM_API int BASM_Parse_ParseChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(i>=0xFC)
	{
		i=(i&1)<<30;
		i+=((*s++)&63)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF8)
	{
		i=(i&3)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF0)
	{
		i=(i&7)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xE0)
	{
		i=(i&15)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xC0)
	{
		i=(i&31)<<6;
		i+=(*s++)&63;
	}else if(i>=0x80)
	{
//		continue;
	}

	*str=(char *)s;
	return(i);
}

BASM_API int BASM_Parse_EmitChar(char **str, int j)
{
	unsigned char *t;
	int i;

	t=(unsigned char *)(*str);

	if(j>0x3FFFFFF)
	{
		*t++=0xFC+(j>>30);
		*t++=0x80+((j>>24)&0x3F);
		*t++=0x80+((j>>18)&0x3F);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0x1FFFFF)
	{
		*t++=0xF8+(j>>24);
		*t++=0x80+((j>>18)&0x3F);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0xFFFF)
	{
		*t++=0xF0+(j>>18);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0x7FF)
	{
		*t++=0xE0+(j>>12);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if((j>0x7F) || !j)
	{
		*t++=0xC0+(j>>6);
		*t++=0x80+(j&0x3F);
	}else
	{
		if(!j) //Modified-UTF8
			{ *t++=0xC0; *t++=0x80; }
			else *t++=j;
	}

	*str=(char *)t;
	return(0);
}

int BASM_Parse_MatchTokenListLen(char *str, char **lst)
{
	char *s, *t;

	while(*lst)
	{
		s=str; t=*lst;

		while(*t)
		{
			if((*s)!=(*t))break;
			s++; t++;
		}

		if(!(*t))return(strlen(*lst));
		lst++;
	}
	return(0);
}

BASM_API char *BASM_Parse_Token(char *s, char *b, int *ty)
{
	static char *token_ops[]={
		"+++", "---", "==", "!=", "<=", ">=", "++", "--",
		"<<", ">>", "(*", "*)", NULL};

	char *t;
	int i, j, k;

	*b=0;
	s=BASM_Parse_EatWhite(s);
	if(!*s)
	{
		*ty=BASM_TOKEN_NULL;
		return(s);
	}
	t=b;

	if(	((*s>='a') && (*s<='z')) ||
		((*s>='A') && (*s<='Z')) ||
		(*s=='_') || (*s=='.') || (*s=='$') || (*s=='@')
		)
	{
		while(
			((*s>='a') && (*s<='z')) ||
			((*s>='A') && (*s<='Z')) ||
			((*s>='0') && (*s<='9')) ||
			(*s=='_') || (*s=='.') ||
			(*s=='$') || (*s=='@')
			)
			*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_NAME;
		return(s);
	}

	if((*s>='0') && (*s<='9'))
	{
		if((s[0]=='0') && (s[1]=='x'))
		{
			*t++=*s++;
			*t++=*s++;
			while(((*s>='0') && (*s<='9')) ||
				((*s>='A') && (*s<='F')) ||
				((*s>='a') && (*s<='f')))
			{
				*t++=*s++;
			}
			*t++=0;

			*ty=BASM_TOKEN_NUMBER;
			return(s);
		}

		if(*s=='-')*t++=*s++;
		while(((*s>='0') && (*s<='9')) || (*s=='.') || (*s=='e') ||
			(*s=='b') || (*s=='h') || (*s=='B') || (*s=='H'))
		{
			if(*s=='e')
			{
				*t++=*s++;
				if((*s=='+') || (*s=='-'))
					*t++=*s++;
				continue;
			}
			*t++=*s++;
		}
		*t++=0;

		*ty=BASM_TOKEN_NUMBER;
		return(s);
	}

	if((*s=='"') || (*s=='\'')) /* quoted string */
	{
		if(*s=='\'')i=1;
			else i=0;
		s++;
		while(*s)
		{
			if(!i)if(*s=='\"')break;
			if(i)if(*s=='\'')break;

			if((s[0]=='\\') && (s[1]=='x'))
			{
				k=2; s+=2; j=0;
				while(k--)
				{
					j<<=4;
					if((*s>='0') && (*s<='9'))j+=*s-'0';
					if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
					if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
					s++;
				}

				*t++=j;
//				BASM_Parse_EmitChar(&t, j);
				continue;
			}

			if((s[0]=='\\') &&
				((s[1]=='u') || (s[1]=='U') || (s[1]=='X')))
			{
				k=2;
				if(s[1]=='u')k=4;
				if(s[1]=='U')k=8;
				if(s[1]=='X')k=4;	//raw 16-bit hex value

				s+=2; j=0;
				while(k--)
				{
					j<<=4;
					if((*s>='0') && (*s<='9'))j+=*s-'0';
					if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
					if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
					s++;
				}

				BASM_Parse_EmitChar(&t, j);
				continue;
			}

			if(*s=='\\')
			{
				s++;
				switch(*s)
				{
				case 'a': *t++='\b'; break;
				case 'b': *t++='\b'; break;
				case 'v': *t++='\v'; break;
				case 'f': *t++='\f'; break;
				case 'e': *t++='\x1B'; break;
				case 'n': *t++='\n'; break;
				case 't': *t++='\t'; break;
				case 'r': *t++='\r'; break;
				case '\\': *t++='\\'; break;
				case '\"': *t++='\"'; break;
				case '\'': *t++='\''; break;
				case '?': *t++='?'; break;
				default:
					break;
				}
				s++;
				continue;
			}

//			*t++=*s++;
			j=BASM_Parse_ParseChar(&s);
			BASM_Parse_EmitChar(&t, j);
		}
		*t++=0;
		if(*s)s++;

		*ty=BASM_TOKEN_STRING;
		if(i)*ty=BASM_TOKEN_CHARSTRING;
		return(s);
	}

//	if((*s==',') || (*s==';') || ((*s=='.') && (s[1]!='.')))
	if((*s==',') || (*s==';'))
	{
		*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_SEPERATOR;
		return(s);
	}

	if(
		(*s=='[') || (*s==']') ||
		(*s=='{') || (*s=='}') ||
		(*s=='(') || (*s==')'))
	{
		*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_BRACE;
		return(s);
	}

	i=BASM_Parse_MatchTokenListLen(s, token_ops);
	if(i)
	{
		strncpy(t, s, i);
		t[i]=0; s+=i;
		*ty=BASM_TOKEN_OPERATOR;
		return(s);
	}

	if(BASM_Parse_OpChar(*s))
	{
		*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_OPERATOR;
		if(!strcmp(b, ":"))*ty=BASM_TOKEN_SEPERATOR;

		return(s);
	}

	*ty=BASM_TOKEN_NULL;
	return(NULL);
}

char *BASM_Parse_TokenGAS(char *s, char *b, int *ty)
{
	char *t;
	int i;

	*b=0;
	s=BASM_Parse_EatWhite(s);
	if(!*s)
	{
		*ty=BASM_TOKEN_NULL;
		return(s);
	}
	t=b;

	if((*s=='_') || (*s=='.') ||
		((*s>='a') && (*s<='z')) || ((*s>='A') && (*s<='Z')))
	{
		while(
			(*s=='_') || (*s=='.') ||
			(*s=='$') || (*s=='@') ||
			((*s>='a') && (*s<='z')) ||
			((*s>='A') && (*s<='Z')) ||
			((*s>='0') && (*s<='9')))
			*t++=*s++;
		*t++=0;

		*ty=BASM_TOKEN_NAME;
		return(s);
	}

	s=BASM_Parse_Token(s, b, ty);
	return(s);
}


#ifdef BASM_SHAVEMEM
BASM_API int BASM_ParseReg(BASM_Context *ctx, char *b)
{
	int i;

	for(i=0; basm_regs[i]; i++)
		if(!strcmp(basm_regs[i], b))
			return(basm_regidx[i]);

	return(-1);
}
#endif

#ifndef BASM_SHAVEMEM
BASM_API int BASM_ParseReg(BASM_Context *ctx, char *b)
{
	static int hash[256];
	static int lcpu=-1;
	char *s;
	int i, h;

	if(lcpu!=ctx->cpu)
	{
		for(i=0; i<256; i++)
			hash[i]=-1;
		lcpu=ctx->cpu;
	}

	s=b; h=0;
	while(*s)h=(h*31)+(*s++);
	h&=255;

	i=hash[h];
	if((i>=0) && !basm_stricmp(basm_regs[i], b))
		return(basm_regidx[i]);

	for(i=0; basm_regs[i]; i++)
		if(!basm_stricmp(basm_regs[i], b))
	{
		hash[h]=i;
		return(basm_regidx[i]);
	}

	return(-1);
}
#endif

char *BASM_ParseArgGAS(BASM_Context *ctx, char *str,
	int *breg, int *ireg, int *sc, long long *disp, char **lbl)
{
	char b[64], b2[64];
	char *s, *t;
	int ty, ty2, sg;
	int i;

	*breg=BASM_Z; *ireg=BASM_Z;
	*sc=0; *disp=0;
	*lbl=NULL;

	s=BASM_Parse_TokenGAS(str, b, &ty);

	if(!strcmp(b, "%"))
	{
		s=BASM_Parse_TokenGAS(str, b, &ty);

		i=-1;
		if(!basm_stricmp(b, "cs"))i=BASM_REG_X86_CS;
		if(!basm_stricmp(b, "ss"))i=BASM_REG_X86_SS;
		if(!basm_stricmp(b, "ds"))i=BASM_REG_X86_DS;
		if(!basm_stricmp(b, "es"))i=BASM_REG_X86_ES;
		if(!basm_stricmp(b, "fs"))i=BASM_REG_X86_FS;
		if(!basm_stricmp(b, "gs"))i=BASM_REG_X86_GS;

		if(i<0)
		{
			*breg=BASM_ParseReg(ctx, b);
			return(s);
		}

		ctx->seg=i;
	}else if(!strcmp(b, "$"))
	{
		s=BASM_Parse_TokenGAS(str, b, &ty);

		if(ty==BASM_TOKEN_NAME)
		{
			*lbl=basm_strdup(b);
			return(s);
		}

		sg=1;
		if(!strcmp(b, "-"))
			{ s=BASM_Parse_TokenGAS(s, b, &ty); sg=-1; }
		if(ty==BASM_TOKEN_NUMBER)
		{
			if(basm_strisreal(b))
			{
				if(ctx->w==64)
				{
					*disp=basm_atof_u64_sg(b, sg);
					return(s);
				}

				ctx->w=32;	//force 32 bits
				*disp=basm_atof_u32_sg(b, sg);
				return(s);
			}

			*disp=sg*basm_atoi(b);
			return(s);
		}
	}

	*sc=1;

	if(ty==BASM_TOKEN_NAME)
	{
		*lbl=basm_strdup(b);
		s=BASM_Parse_TokenGAS(s, b, &ty);
	}

	sg=1;
	if(!strcmp(b, "-")) { s=BASM_Parse_TokenGAS(s, b, &ty); sg=-1; }
	if(ty==BASM_TOKEN_NUMBER)
	{
		*disp=sg*basm_atoi(b);
		s=BASM_Parse_TokenGAS(s, b, &ty);
	}

	if(!strcmp(b, "(") && (ty==BASM_TOKEN_BRACE))
	{
		*sc=1;

		s=BASM_Parse_TokenGAS(s, b, &ty);
		if(!strcmp(b, "%"))s=BASM_Parse_TokenGAS(s, b, &ty);
		BASM_Parse_TokenGAS(s, b2, &ty2);

		if(ty==BASM_TOKEN_NAME)
		{
			*breg=BASM_ParseReg(ctx, b);
			s=BASM_Parse_TokenGAS(s, b, &ty);
		}

		if(!strcmp(b, ")"))return(s);
		if(!strcmp(b, ","))
			s=BASM_Parse_TokenGAS(s, b, &ty);
		if(!strcmp(b, "%"))s=BASM_Parse_TokenGAS(s, b, &ty);
		BASM_Parse_TokenGAS(s, b2, &ty2);

		if(ty==BASM_TOKEN_NAME)
		{
			*ireg=BASM_ParseReg(ctx, b);;
			s=BASM_Parse_TokenGAS(s, b, &ty);
		}

		if(!strcmp(b, ")"))return(s);
		if(!strcmp(b, ","))
			s=BASM_Parse_TokenGAS(s, b, &ty);
		BASM_Parse_TokenGAS(s, b2, &ty2);

		sg=1;
		if(!strcmp(b, "-"))
			{ s=BASM_Parse_TokenGAS(s, b, &ty); sg=-1; }
		if(ty==BASM_TOKEN_NUMBER)
		{
			*sc+=sg*basm_atoi(b);
			s=BASM_Parse_TokenGAS(s, b, &ty);
		}

		if(!strcmp(b, ")"))return(s);

		printf("BASM_ParseArgGAS: Bad token '%s'\n", b);
		return(s);
	}

	printf("BASM_ParseArgGAS: Bad token '%s'\n", b);
	return(s);
}

BASM_API char *BASM_ParseArg(BASM_Context *ctx, char *str,
	int *breg, int *ireg, int *sc, long long *disp, char **lbl)
{
	char b[64], b2[64];
	char *s, *t;
	int ty, ty2, sg;
	int i;

	*breg=BASM_Z; *ireg=BASM_Z;
	*sc=0; *disp=0;
	*lbl=NULL;

	s=BASM_Parse_EatWhite(str);
	if((*s=='%') || (*s=='$') || (ctx->fl&BASM_FL_GAS))
	{
		ctx->fl|=BASM_FL_GAS;
		s=BASM_ParseArgGAS(ctx, s, breg, ireg, sc, disp, lbl);
		return(s);
	}


	s=BASM_Parse_Token(str, b, &ty);

	if(ty==BASM_TOKEN_NAME)
	{
		*breg=BASM_ParseReg(ctx, b);
		if(*breg!=BASM_Z)return(s);

		*lbl=basm_strdup(b);
		return(s);
	}

	sg=1;
//	if(!strcmp(b, "-"))
	if(b[0]=='-')
		{ s=BASM_Parse_Token(s, b, &ty); sg=-1; }
	if(ty==BASM_TOKEN_NUMBER)
	{
//		BASM_Parse_Token(s, b2, &ty2);
//		if(!strcmp(b2, "("))
//		if(b2[0]=='(')
		if(*s=='(')
		{
			ctx->fl|=BASM_FL_GAS;
			s=BASM_ParseArgGAS(ctx, str,
				breg, ireg, sc, disp, lbl);
			return(s);
		}


		if(basm_strisreal(b))
		{
			if(ctx->w==64)
			{
				*disp=basm_atof_u64_sg(b, sg);
				return(s);
			}

			ctx->w=32;	//force 32 bits
			*disp=basm_atof_u32_sg(b, sg);
			return(s);
		}

		*disp=sg*basm_atoi(b);
		return(s);
	}

//	if(!strcmp(b, "[") && (ty==BASM_TOKEN_BRACE))
	if(b[0]=='[')
	{
		*sc=1;

		s=BASM_Parse_Token(s, b, &ty);
//		BASM_Parse_Token(s, b2, &ty2);

//		if(!strcmp(b2, ":") &&
//		if((b2[0]==':') &&
		if((*s==':') &&
			(!basm_stricmp(b, "cs") || !basm_stricmp(b, "ss") ||
			!basm_stricmp(b, "ds") || !basm_stricmp(b, "es") ||
			!basm_stricmp(b, "fs") || !basm_stricmp(b, "gs")) )
		{
			s++;	//':'
//			s=BASM_Parse_Token(s, b2, &ty2);	//':'

			if(!basm_stricmp(b, "cs"))ctx->seg=BASM_REG_X86_CS;
			if(!basm_stricmp(b, "ss"))ctx->seg=BASM_REG_X86_SS;
			if(!basm_stricmp(b, "ds"))ctx->seg=BASM_REG_X86_DS;
			if(!basm_stricmp(b, "es"))ctx->seg=BASM_REG_X86_ES;
			if(!basm_stricmp(b, "fs"))ctx->seg=BASM_REG_X86_FS;
			if(!basm_stricmp(b, "gs"))ctx->seg=BASM_REG_X86_GS;

			s=BASM_Parse_Token(s, b, &ty);
//			BASM_Parse_Token(s, b2, &ty2);
		}

		sg=1;
//		if(!strcmp(b, "+") || !strcmp(b, "-"))
		if((b[0]=='+') || (b[0]=='-'))
		{
			if(!strcmp(b, "-"))sg=-1;

			*sc=-1;
			s=BASM_Parse_Token(s, b, &ty);
//			BASM_Parse_Token(s, b2, &ty2);
		}

		if(ty==BASM_TOKEN_NAME)
		{
			*breg=BASM_ParseReg(ctx, b);
			if(*breg==BASM_Z)
			{
				for(i=0; i<ctx->n_const; i++)
					if(!basm_stricmp(ctx->const_name[i], b))
				{
					*disp=ctx->const_value[i];
					break;
				}
				if(i>=ctx->n_const)*lbl=basm_strdup(b);
			}

			s=BASM_Parse_Token(s, b, &ty);
//			if(!strcmp(b, "]"))
			if(b[0]==']')
				return(s);
//			if(!strcmp(b, "+"))
			if(b[0]=='+')
				s=BASM_Parse_Token(s, b, &ty);
//			BASM_Parse_Token(s, b2, &ty2);
		}else if(ty==BASM_TOKEN_NUMBER)
		{
			*disp=sg*basm_atoi(b);
			s=BASM_Parse_Token(s, b, &ty);
//			if(!strcmp(b, "]"))
			if(b[0]==']')
				return(s);
//			if(!strcmp(b, "+"))
			if(b[0]=='+')
				s=BASM_Parse_Token(s, b, &ty);
//			BASM_Parse_Token(s, b2, &ty2);
		}else
		{
			printf("BASM_ParseArg: Bad token '%s'\n", b);
			return(s);
		}

		i=BASM_Z;
		if(ty==BASM_TOKEN_NAME)
			i=BASM_ParseReg(ctx, b);

//		if((ty==BASM_TOKEN_NAME) && strcmp(b2, "*") &&
		if((ty==BASM_TOKEN_NAME) && ((*s)!='*') &&
			(i!=BASM_Z) && (*breg==BASM_Z))
		{
			*breg=i;

			s=BASM_Parse_Token(s, b, &ty);
//			if(!strcmp(b, "]"))
			if(b[0]==']')
				return(s);
//			if(!strcmp(b, "+"))
			if(b[0]=='+')
				s=BASM_Parse_Token(s, b, &ty);

			if(ty==BASM_TOKEN_NAME)
				i=BASM_ParseReg(ctx, b);
		}

		if((ty==BASM_TOKEN_NAME) && (i!=BASM_Z))
		{
			*ireg=i;
//			if(!strcmp(b2, "*"))
//			if(b2[0]=='*')
			if(*s=='*')
			{
				s++;	//'*'
//				s=BASM_Parse_Token(s, b, &ty);
				s=BASM_Parse_Token(s, b, &ty);
				*sc=atoi(b);
			}

			s=BASM_Parse_Token(s, b, &ty);
//			if(!strcmp(b, "]"))
			if(b[0]==']')
				return(s);
//			if(!strcmp(b, "+"))
			if(b[0]=='+')
				s=BASM_Parse_Token(s, b, &ty);
		}

		sg=1;
//		if(!strcmp(b, "-"))
		if(b[0]=='-')
			{ s=BASM_Parse_Token(s, b, &ty); sg=-1; }
		if(ty==BASM_TOKEN_NUMBER)
		{
			*disp+=sg*basm_atoi(b);
			s=BASM_Parse_Token(s, b, &ty);
		}

		if(ty==BASM_TOKEN_NAME)
		{
			for(i=0; i<ctx->n_const; i++)
				if(!strcmp(ctx->const_name[i], b))
			{
				*disp+=sg*ctx->const_value[i];
				break;
			}

			if(i>=ctx->n_const)*lbl=basm_strdup(b);

			s=BASM_Parse_Token(s, b, &ty);
		}

//		if(strcmp(b, "]"))
		if(b[0]!=']')
			printf("BASM_ParseArg: Bad token '%s'\n", b);
		return(s);
	}

	//ARM/Thumb
	if((ctx->fl&BASM_FL_ARM) && (b[0]=='='))
	{
		s=BASM_Parse_Token(s, b, &ty);

		if(ty==BASM_TOKEN_NUMBER)
		{
			*disp+=sg*basm_atoi(b);
//			s=BASM_Parse_Token(s, b, &ty);
		}

		if(ty==BASM_TOKEN_NAME)
		{
			for(i=0; i<ctx->n_const; i++)
				if(!strcmp(ctx->const_name[i], b))
			{
				*disp+=sg*ctx->const_value[i];
				break;
			}

			if(i>=ctx->n_const)*lbl=basm_strdup(b);

//			s=BASM_Parse_Token(s, b, &ty);
		}

		return(s);
	}

	printf("BASM_ParseArg: Bad token '%s'\n", b);
	return(s);
}


BASM_API int BASM_OpOnlySingleP(int op)
{
	int i;

	i=basm_opidx[op];
	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpSingleP(i))return(0);
	}
//	if(i!=basm_opidx[op])return(0);
	return(1);
}

#if 0
BASM_API char *BASM_ParseXMeta(BASM_Context *ctx, char *str,
	char *op, char *tag)
{
	char *ta[64];
	char tb[1024];
	char b[256], b2[256];
	char *s, *t, *s1;
	int i, j, iq, ty;
	
	s=str; t=tb; i=0; iq=0;
	j=0; ta[0]=t;

	while(*s && ((*s==' ') || (*s=='\t')))s++;
	while(*s)
	{
		if(!iq && !i)
		{
			if(*s==';')		break;
			if(*s==']')		break;
			if(*s==')')		break;
			if(*s=='\n')	break;

			if(*s==',')
			{
				s++; *t++=0;
				ta[j++]=t;
				while(*s && ((*s==' ') || (*s=='\t')))s++;

				if(*s=='\"')
				{
					s=BASM_Parse_Token(s, b, &ty);
					strcpy(t, b); t+=strlen(t);
					while(*s && ((*s==' ') || (*s=='\t')))s++;
					continue;	
				}
				continue;
			}
		}
		
		if(iq)
		{
			if(*s=='\\')
			{
				*t++=*s++;	// '\'
				if(*s=='x')
				{
					*t++=*s++; *t++=*s++; *t++=*s++;
					continue;
				}
				if(*s=='u')
				{
					*t++=*s++;
					*t++=*s++; *t++=*s++;
					*t++=*s++; *t++=*s++;
					continue;
				}
				*t++=*s++;
				continue;
			}

			if((iq==1) && (*s=='\"')) { iq=0; *t++=*s++; continue; }
			if((iq==2) && (*s=='\'')) { iq=0; *t++=*s++; continue; }
			*t++=*s++;
			continue;
		}

		if(*s=='\\')
		{
			s1=s+1;
			while(*s1 && (*s1==' ') || (*s1=='\r'))s1++;
			if(*s1=='\n') { s=s1+1; continue; }
		}

		if(*s=='\"') { iq=1; *t++=*s++; continue; }
		if(*s=='\'') { iq=2; *t++=*s++; continue; }
		if(*s=='(') { i++; *t++=*s++; continue; }
		if(*s=='[') { i++; *t++=*s++; continue; }
		if(*s==')') { i--; *t++=*s++; continue; }
		if(*s==']') { i--; *t++=*s++; continue; }
				
		*t++=*s++;
	}
	*t++=0; ta[j++]=NULL;

	if(*s==';')		s++;
	if(*s=='\n')	s++;
	
	if(!basm_stricmp(op, "xnotify"))
	{
		t=BASM_ComposeLinkNotifyName(tag, ta);
		BASM_EmitLabel(ctx, t);
		return(s);
	}
	
	if(!basm_stricmp(op, "xmeta"))
	{
		t=BASM_ComposeLinkMetaName(tag, ta);
//		BASM_EmitLabel(ctx, t);
		if((ctx->cpu==BASM_CPU_X86) ||
			(ctx->cpu==BASM_CPU_X64) ||
			(ctx->cpu==BASM_CPU_NACL_X86) ||
			(ctx->cpu==BASM_CPU_NACL_X64))
		{
			BASM_OutOpImm(ctx, BASM_OP_X86_CALL, 0, 0, t);
		}

//		if(ctx->cpu==BASM_CPU_ARM)
//			{ BASM_OutOpImm(ctx, BASM_OP_ARM_BL, 0, 0, t); }
		if(ctx->cpu==BASM_CPU_THUMB)
			{ BASM_OutOpImm(ctx, BASM_OP_THUMB_BL, 0, 0, t); }
		return(s);
	}

	return(NULL);
}
#endif

BASM_API char *BASM_ParseSpecial(BASM_Context *ctx, char *str, char *op)
{
	char b[64], b2[64];
	char *s, *t, *l0, *l1;
	byte *ip;
	long long li;
	int ty, ty2;
	int i, j, k, w, sg;

	s=str;
//	if(!basm_stricmp(op, "A16")) { ctx->fl|=2; return(s); }
//	if(!basm_stricmp(op, "A32")) { ctx->fl&=~3; return(s); }
//	if(!basm_stricmp(op, "A64")) { ctx->fl|=1; return(s); }

	if(!strcmp(op, "A16")) { ctx->fl|=2; return(s); }
	if(!strcmp(op, "A32")) { ctx->fl&=~3; return(s); }
	if(!strcmp(op, "A64")) { ctx->fl|=1; return(s); }

	if(!basm_stricmp(op, ".a16")) { ctx->fl|=2; return(s); }
	if(!basm_stricmp(op, ".a32")) { ctx->fl&=~3; return(s); }
	if(!basm_stricmp(op, ".a64")) { ctx->fl|=1; return(s); }

	if(!basm_stricmp(op, "use16")) { ctx->fl|=2; return(s); }
	if(!basm_stricmp(op, "use32")) { ctx->fl&=~3; return(s); }
	if(!basm_stricmp(op, "use64")) { ctx->fl|=1; return(s); }

	if(!basm_stricmp(op, ".text")) { ctx->fl&=~4; return(s); }
	if(!basm_stricmp(op, ".data")) { ctx->fl|=4; return(s); }

	if(!basm_stricmp(op, ".proxy"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=ctx->n_proxy++;
		if(i>=ctx->m_proxy)
		{
			j=i+(i>>1);
			ctx->proxy_name=realloc(ctx->proxy_name,
				j*sizeof(char *));
			ctx->m_proxy=j;
		}
		ctx->proxy_name[i]=basm_strdup(b);
		return(s);
	}

	if(!basm_stricmp(op, "section"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		if(!basm_stricmp(b, ".text"))ctx->fl&=~4;
		if(!basm_stricmp(b, ".data"))ctx->fl|=4;
		if(!basm_stricmp(b, ".bss"))ctx->fl|=4;
		return(s);
	}

	if(!basm_stricmp(op, ".section"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		if(!basm_stricmp(b, ".text"))ctx->fl&=~4;
		if(!basm_stricmp(b, ".data"))ctx->fl|=4;
		if(!basm_stricmp(b, ".bss"))ctx->fl|=4;
		return(s);
	}

	if(!basm_stricmp(op, "org"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		return(s);
	}

	if(!basm_stricmp(op, ".org"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		return(s);
	}

	if(!basm_stricmp(op, ".syntax"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		return(s);
	}

	if(!basm_stricmp(op, "bits"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		if(!basm_stricmp(b, "16"))
		{
			if((ctx->cpu!=BASM_CPU_X86) &&
				(ctx->cpu!=BASM_CPU_NACL_X86))
					BASM_SetCPU(ctx, BASM_CPU_X86);
			ctx->fl|=2;
		}
		if(!basm_stricmp(b, "32"))
		{
			if((ctx->cpu!=BASM_CPU_X86) &&
				(ctx->cpu!=BASM_CPU_NACL_X86))
					BASM_SetCPU(ctx, BASM_CPU_X86);
			ctx->fl&=~3;
		}
		if(!basm_stricmp(b, "64"))
		{
			if((ctx->cpu!=BASM_CPU_X64) &&
				(ctx->cpu!=BASM_CPU_NACL_X64))
					BASM_SetCPU(ctx, BASM_CPU_X64);
			ctx->fl|=1;
		}
		return(s);
	}

	if(!basm_stricmp(op, "cpu"))
	{
		s=BASM_Parse_Token(s, b, &ty);

		if(!basm_stricmp(b, "x86"))
			BASM_SetCPU(ctx, BASM_CPU_X86);
		if(!basm_stricmp(b, "x64"))
			BASM_SetCPU(ctx, BASM_CPU_X64);
		if(!basm_stricmp(b, "x86_64"))
			BASM_SetCPU(ctx, BASM_CPU_X64);

		if(!basm_stricmp(b, "arm"))
			BASM_SetCPU(ctx, BASM_CPU_ARM);
		if(!basm_stricmp(b, "thumb"))
			BASM_SetCPU(ctx, BASM_CPU_THUMB);

		if(!basm_stricmp(b, "nacl_x86"))
			BASM_SetCPU(ctx, BASM_CPU_NACL_X86);
		if(!basm_stricmp(b, "nacl_x64"))
			BASM_SetCPU(ctx, BASM_CPU_NACL_X64);
		if(!basm_stricmp(b, "nacl_x86_64"))
			BASM_SetCPU(ctx, BASM_CPU_NACL_X64);

		return(s);
	}

	if(!basm_stricmp(op, ".i386"))
		{ BASM_SetCPU(ctx, BASM_CPU_X86); return(s); }
	if(!basm_stricmp(op, ".x86_64"))
		{ BASM_SetCPU(ctx, BASM_CPU_X64); return(s); }
	if(!basm_stricmp(op, ".arm"))
		{ BASM_SetCPU(ctx, BASM_CPU_ARM); return(s); }
	if(!basm_stricmp(op, ".thumb"))
		{ BASM_SetCPU(ctx, BASM_CPU_THUMB); return(s); }

	if(!basm_stricmp(op, "extern"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		return(s);
	}

	if(!basm_stricmp(op, "global"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		return(s);
	}

	if(!basm_stricmp(op, ".align"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		BASM_Align(ctx, 1<<basm_atoi(b));
		return(s);
	}

	if(!basm_stricmp(op, "align") || !basm_stricmp(op, ".balign"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		BASM_Align(ctx, basm_atoi(b));
		return(s);
	}

	if(!basm_stricmp(op, "align_vli"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		BASM_AlignVLI(ctx, basm_atoi(b));
		return(s);
	}


	if(!basm_stricmp(op, ".ascii"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			if(ty2==BASM_TOKEN_STRING)
				{ BASM_OutStr8(ctx, b2); }
			if(ty2==BASM_TOKEN_CHARSTRING)
				{ BASM_OutStr8(ctx, b2); }
		}
		return(s);
	}

	if(!basm_stricmp(op, ".asciz"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			if(ty2==BASM_TOKEN_STRING)
				{ BASM_OutStr8(ctx, b2); }
			if(ty2==BASM_TOKEN_CHARSTRING)
				{ BASM_OutStr8(ctx, b2); }
		}
		BASM_OutByte(ctx, 0);
		return(s);
	}

	if(!basm_stricmp(op, "db") || !basm_stricmp(op, ".byte"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			if(ty2==BASM_TOKEN_NAME)
			{
				if(b2[0]=='$')
				{
					BASM_OutByte(ctx, 1);
					BASM_EmitGoto(ctx, b2+1, BASM_JMP_SHORT);
					continue;
				}

				BASM_OutByte(ctx, 0);
//				BASM_EmitGoto(ctx, b2, BASM_JMP_ABS32);
				continue;
			}

			sg=1;
			if(!basm_stricmp(b2, "-"))
			{
				s=BASM_Parse_Token(s, b2, &ty2);
				sg=-1;
			}

			if(ty2==BASM_TOKEN_NUMBER)
				BASM_OutByte(ctx, basm_atoi(b2)*sg);
//			if(ty2==BASM_TOKEN_STRING)
//				{ t=b2; while(*t)BASM_OutByte(ctx, *t++); }
//			if(ty2==BASM_TOKEN_CHARSTRING)
//				{ t=b2; while(*t)BASM_OutByte(ctx, *t++); }

			if(ty2==BASM_TOKEN_STRING)
				{ BASM_OutStr8(ctx, b2); }
			if(ty2==BASM_TOKEN_CHARSTRING)
				{ BASM_OutStr8(ctx, b2); }
		}
		return(s);
	}

	if(!basm_stricmp(op, "dw") || !basm_stricmp(op, ".short") ||
		!basm_stricmp(op, ".hword"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			if(ty2==BASM_TOKEN_NAME)
			{
				if(b2[0]=='$')
				{
					BASM_OutWord(ctx, 2);
					BASM_EmitGoto(ctx, b2+1, BASM_JMP_NEAR16);
					continue;
				}

				BASM_OutWord(ctx, 0);
				BASM_EmitGoto(ctx, b2, BASM_JMP_ABS16);
				continue;
			}

			sg=1;
			if(!basm_stricmp(b2, "-"))
			{
				s=BASM_Parse_Token(s, b2, &ty2);
				sg=-1;
			}

			if(ty2==BASM_TOKEN_NUMBER)
				BASM_OutWord(ctx, basm_atoi(b2)*sg);
//			if(ty2==BASM_TOKEN_STRING)
//				{ t=b2; while(*t)BASM_OutWord(ctx, *t++); }
//			if(ty2==BASM_TOKEN_CHARSTRING)
//				{ t=b2; while(*t)BASM_OutWord(ctx, *t++); }

			if(ty2==BASM_TOKEN_STRING)
				{ BASM_OutStr16(ctx, b2); }
			if(ty2==BASM_TOKEN_CHARSTRING)
				{ BASM_OutStr16(ctx, b2); }
		}
		return(s);
	}

	if(!basm_stricmp(op, "dd") || !basm_stricmp(op, ".word") ||
		!basm_stricmp(op, ".int") || !basm_stricmp(op, ".long"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			if(ty2==BASM_TOKEN_NAME)
			{
				if(b2[0]=='$')
				{
					BASM_OutDWord(ctx, 4);
					BASM_EmitGoto(ctx, b2+1, BASM_JMP_NEAR32);
					continue;
				}

				BASM_OutDWord(ctx, 0);
				BASM_EmitGoto(ctx, b2, BASM_JMP_ABS32);
				continue;
			}

			sg=1;
			if(!basm_stricmp(b2, "-"))
			{
				s=BASM_Parse_Token(s, b2, &ty2);
				sg=-1;
			}

			if(ty2==BASM_TOKEN_NUMBER)
			{
				if(basm_strisreal(b2))
				{
					BASM_OutDWord(ctx,
						basm_atof_u32_sg(b2, sg));
				}else
				{
					BASM_OutDWord(ctx, basm_atoi(b2)*sg);
				}
			}
		}
		return(s);
	}

	if(!basm_stricmp(op, "dq") || !basm_stricmp(op, ".quad"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;
//			if(ty2==BASM_TOKEN_NUMBER)
//				BASM_OutQWord(ctx, basm_atoi(b2));

			if(ty2==BASM_TOKEN_NAME)
			{
				BASM_OutQWord(ctx, 0);
				BASM_EmitGoto(ctx, b2, BASM_JMP_ABS64);
				continue;
			}

			sg=1;
			if(!basm_stricmp(b2, "-"))
			{
				s=BASM_Parse_Token(s, b2, &ty2);
				sg=-1;
			}

			if(ty2==BASM_TOKEN_NUMBER)
			{
				if(basm_strisreal(b2))
				{
					BASM_OutQWord(ctx,
						basm_atof_u64_sg(b2, sg));
				}else
				{
					BASM_OutQWord(ctx, basm_atoi(b2)*sg);
				}
			}
		}
		return(s);
	}

	if(!basm_stricmp(op, ".float"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			sg=1;
			if(!basm_stricmp(b2, "-"))
				{ s=BASM_Parse_Token(s, b2, &ty2); sg=-1; }

			if(ty2==BASM_TOKEN_NUMBER)
				BASM_OutDWord(ctx, basm_atof_u32_sg(b2, sg));
		}
		return(s);
	}

	if(!basm_stricmp(op, ".double"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			sg=1;
			if(!basm_stricmp(b2, "-"))
				{ s=BASM_Parse_Token(s, b2, &ty2); sg=-1; }

			if(ty2==BASM_TOKEN_NUMBER)
				BASM_OutQWord(ctx, basm_atof_u64_sg(b2, sg));
		}
		return(s);
	}

	if(!basm_stricmp(op, "duv"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			if(ty2==BASM_TOKEN_NAME)
			{
				li=BASM_PredictDisp(ctx, b2);

				if(!li)
				{
					BASM_OutDWord(ctx, 0xE0000000);
					continue;
				}

				if(li<((1<<7)-16))li-=1;
				else if(li<((1<<14)-16))li-=2;
				else if(li<((1<<21)-16))li-=3;
				else if(li<((1<<28)-16))li-=4;
				BASM_OutUVLIP(ctx, li, 16);
				continue;
			}

			if(ty2==BASM_TOKEN_NUMBER)
			{
				BASM_OutUVLI(ctx, basm_atoi(b2));
			}
		}
		return(s);
	}

	if(!basm_stricmp(op, "dsv"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			if(ty2==BASM_TOKEN_NAME)
			{
				li=BASM_PredictDisp(ctx, b2);

				if(!li)
				{
					BASM_OutDWord(ctx, 0xE0000000);
					continue;
				}

				BASM_OutSVLIP(ctx, li, 16);
				continue;
			}

			sg=1;
			if(!basm_stricmp(b2, "-"))
			{
				s=BASM_Parse_Token(s, b2, &ty2);
				sg=-1;
			}

			if(ty2==BASM_TOKEN_NUMBER)
			{
				BASM_OutSVLI(ctx, basm_atoi(b2)*sg);
			}
		}
		return(s);
	}

#if 0
	if(!basm_stricmp(op, "drd"))
	{
		while(1)
		{
			if(BASM_Parse_IsLineBreak(s))break;
			s=BASM_Parse_Token(s, b2, &ty2);
			if(!basm_stricmp(b2, ";"))break;
			if(!basm_stricmp(b2, ","))continue;

			if(ty2==BASM_TOKEN_NAME)
			{
				BASM_OutDWord(ctx, 4);
				BASM_EmitGoto(ctx, b2, BASM_JMP_NEAR32);
				continue;
			}

			sg=1;
			if(!basm_stricmp(b2, "-"))
			{
				s=BASM_Parse_Token(s, b2, &ty2);
				sg=-1;
			}

			if(ty2==BASM_TOKEN_NUMBER)
			{
				if(basm_strisreal(b2))
				{
					BASM_OutDWord(ctx,
						basm_atof_u32_sg(b2, sg));
				}else
				{
					BASM_OutDWord(ctx, basm_atoi(b2)*sg);
				}
			}
		}
		return(s);
	}
#endif

	if(!basm_stricmp(op, "resb"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=basm_atoi(b);
		while(i--)BASM_OutByte(ctx, 0);
		return(s);
	}

	if(!basm_stricmp(op, "resw"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=basm_atoi(b);
		while(i--)BASM_OutWord(ctx, 0);
		return(s);
	}

	if(!basm_stricmp(op, "resd"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=basm_atoi(b);
		while(i--)BASM_OutDWord(ctx, 0);
		return(s);
	}

	if(!basm_stricmp(op, "resq"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=basm_atoi(b);
		while(i--)BASM_OutQWord(ctx, 0);
		return(s);
	}

	if(!basm_stricmp(op, "resbto"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=basm_atoi(b);
		j=BASM_GetSectionOffset(ctx);
		BASM_OutPadZero(ctx, i-j);
		return(s);
	}

	if(!basm_stricmp(op, "padto"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=basm_atoi(b);
		j=BASM_GetSectionOffset(ctx);
		BASM_OutPad(ctx, i-j);
		return(s);
	}

#if 0
	if(!basm_stricmp(op, "xmeta"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		s=BASM_ParseXMeta(ctx, s, op, b);
		return(s);
	}

	if(!basm_stricmp(op, "xnotify"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		s=BASM_ParseXMeta(ctx, s, op, b);
		return(s);
	}
#endif

	if(!basm_stricmp(op, "getip"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=BASM_ParseReg(ctx, b);

		//call .l0; .l0:
		BASM_OutByte(ctx, 0xE8);	//call
		if(ctx->fl&2)BASM_OutWord(ctx, 0);
			else BASM_OutDWord(ctx, 0);

		//pop reg
		if(i&8)BASM_OutByte(ctx, 0x41);	//REX.B
		BASM_OutByte(ctx, 0x58|(i&0x7));

		if(i&8)BASM_OutByte(ctx, 0x41);	//REX.B
		BASM_OutByte(ctx, 0x83);
		BASM_OutByte(ctx, 0xC0|(0<<3)|(i&3));
		BASM_OutByte(ctx, (i&8)?6:4);

		return(s);
	}

	if(!basm_stricmp(op, "getbase"))
	{
		s=BASM_Parse_Token(s, b, &ty);
		i=BASM_ParseReg(ctx, b);

		//call .l0; .l0:
		BASM_OutByte(ctx, 0xE8);	//call
		if(ctx->fl&2)BASM_OutWord(ctx, 0);
			else BASM_OutDWord(ctx, 0);
		j=ctx->ip-ctx->base_ip;

		//pop reg
		if(i&8)BASM_OutByte(ctx, 0x41);	//REX.B
		BASM_OutByte(ctx, 0x58|(i&0x7));

		//sub reg, offs
		if(i&8)BASM_OutByte(ctx, 0x41);	//REX.B

		if(j<128)
		{
			BASM_OutByte(ctx, 0x83);
			BASM_OutByte(ctx, 0xC0|(5<<3)|(i&3));
			BASM_OutByte(ctx, j);
		}else
		{
			BASM_OutByte(ctx, 0x81);
			BASM_OutByte(ctx, 0xC0|(5<<3)|(i&3));
			if(ctx->fl&2)BASM_OutWord(ctx, j);
				else BASM_OutDWord(ctx, j);
		}

		return(s);
	}

	if(!basm_stricmp(op, "setbase"))
	{
		s=BASM_Parse_Token(s, b, &ty);
//		ip=BASM_LookupLabel(ctx, b);
//		if(ip)ctx->base_ip=ip;
		return(s);
	}

#if 1
	//custom special opcodes

	for(i=0; i<basm_psop_num; i++)
	{
		if(!basm_stricmp(basm_psop_name[i], op))
		{
			l0=basm_psop_fcn[i](ctx, op, &s);
			if(l0)BASM_ParseOpcodeList(ctx, l0);
			return(s);
		}
	}

	for(i=0; i<basm_psop2_num; i++)
	{
		l0=basm_psop2_fcn[i](ctx, op, &s);
		if(l0)
		{
			BASM_ParseOpcodeList(ctx, l0);
			return(s);
		}
	}
#endif

	return(NULL);
}

BASM_API int BASM_LookupOpcode(BASM_Context *ctx, char *str)
{
#ifndef BASM_SHAVEMEM
	static int *hash=NULL;
	static int lcpu=-1;
#endif

	int i, h;
	char *s;

#ifndef BASM_SHAVEMEM
	if(!hash)
	{
		hash=malloc(1024*sizeof(int));
		lcpu=-1;
	}
	if(ctx->cpu!=lcpu)
	{
		//ISA changed, flush hash
		for(i=0; i<1024; i++)
			hash[i]=-1;
		lcpu=ctx->cpu;
	}

	s=str; h=0;
	while(*s)h=h*251+(*s++);
	h&=0x3FF;

	if(hash[h]>=0)
	{
		i=hash[h];
		if(!basm_stricmp(basm_ops[i], str))
			return(i);
	}
#endif

	for(i=0; basm_ops[i]; i++)
		if(!basm_stricmp(basm_ops[i], str))
			break;

#ifndef BASM_SHAVEMEM
	if(basm_ops[i])
		hash[h]=i;
#endif

	return(i);
}

int BASM_CheckARMCC(BASM_Context *ctx, char **str)
{
	char *s;
	int fl;
	
	s=*str; fl=0;

	if(!basm_stricmp(s-2, "eq"))
		{ fl|=BASM_ARM_COND_EQ<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "ne"))
		{ fl|=BASM_ARM_COND_NE<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "cs"))
		{ fl|=BASM_ARM_COND_CS<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "cc"))
		{ fl|=BASM_ARM_COND_CC<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "mi"))
		{ fl|=BASM_ARM_COND_MI<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "pl"))
		{ fl|=BASM_ARM_COND_PL<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "vs"))
		{ fl|=BASM_ARM_COND_VS<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "vc"))
		{ fl|=BASM_ARM_COND_VC<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "hi"))
		{ fl|=BASM_ARM_COND_HI<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "ls"))
		{ fl|=BASM_ARM_COND_LS<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "ge"))
		{ fl|=BASM_ARM_COND_GE<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "lt"))
		{ fl|=BASM_ARM_COND_LT<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "gt"))
		{ fl|=BASM_ARM_COND_GT<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "le"))
		{ fl|=BASM_ARM_COND_LE<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }
	else if(!basm_stricmp(s-2, "al"))
		{ fl|=BASM_ARM_COND_AL<<BASM_FL_ARMCC_SHL; s-=2; *s=0; }

	*str=s;
	return(fl);
}

BASM_API int BASM_LookupOpcodeARMCC(BASM_Context *ctx, char *str)
{
	char tb[256], tb1[256];
	char *s, *s0, *s1;
	int i, j, fl;

	ctx->fl&=~(BASM_FL_ARMCC_MASK|BASM_FL_ARMCC_S);
	fl=0;

	strcpy(tb, str);
	s=tb+strlen(tb);

#if 0
	if((*(s-1)=='s') || (*(s-1)=='S'))
	{
		fl|=BASM_FL_ARMCC_S;
		s--; *s=0;
	}

//	i=BASM_LookupOpcode(ctx, tb);
//	if(basm_ops[i])return(i);
	sprintf(tb1, "%s.c", tb);
	i=BASM_LookupOpcode(ctx, tb1);
	if(basm_ops[i])
	{
		fl|=BASM_ARM_COND_AL<<BASM_FL_ARMCC_SHL;
		ctx->fl|=fl;
		return(i);
	}

	sprintf(tb1, "%s.cs", tb);
	i=BASM_LookupOpcode(ctx, tb1);
	if(basm_ops[i])
	{
		fl|=BASM_ARM_COND_AL<<BASM_FL_ARMCC_SHL;
		ctx->fl|=fl;
		return(i);
	}
#endif

	s0=s;
	fl=BASM_CheckARMCC(ctx, &s);
	if((s==s0) && ((*(s-1)=='s') || (*(s-1)=='S')))
	{
		s--; *s=0;
		s0=s;
		fl=BASM_CheckARMCC(ctx, &s);
		fl|=BASM_FL_ARMCC_S;
	}
	if(s==s0)
		{ fl|=BASM_ARM_COND_AL<<BASM_FL_ARMCC_SHL; }

	sprintf(tb1, "%s.c", tb);
	i=BASM_LookupOpcode(ctx, tb1);
	if(basm_ops[i])
		{ ctx->fl|=fl; return(i); }
	sprintf(tb1, "%s.cs", tb);
	i=BASM_LookupOpcode(ctx, tb1);
	if(basm_ops[i])
		{ ctx->fl|=fl; return(i); }

	return(i);
}

#if 1
char *BASM_ParseOpcodeInner(BASM_Context *ctx, char *str,
	char *opname, int opnum)
{
	char b[64], b2[64];
	char *s, *t, *l0, *l1, *l2;
	int ty, ty2;
	int i, j;

	long long disp0, disp1, disp2;
	int br0, ir0, sc0;
	int br1, ir1, sc1;
	int br2, ir2, sc2;

#if 1
	static char *kw_byte=NULL, *kw_word, *kw_dword, *kw_qword;
	static char *kw_oword, *kw_yword, *kw_tword, *kw_dqword;
	static char *kw_short, *kw_near16, *kw_near32, *kw_ptr, *kw_offset;

	if(!kw_byte)
	{
		kw_byte=basm_strdup("byte");
		kw_word=basm_strdup("word");
		kw_dword=basm_strdup("dword");
		kw_qword=basm_strdup("qword");
		kw_oword=basm_strdup("oword");
		kw_yword=basm_strdup("yword");
		kw_tword=basm_strdup("tword");
		kw_dqword=basm_strdup("dqword");

		kw_short=basm_strdup("short");
		kw_near16=basm_strdup("near16");
		kw_near32=basm_strdup("near32");
		kw_ptr=basm_strdup("ptr");
		kw_offset=basm_strdup("offset");
	}
#endif

	b[0]=0; b2[0]=0;

//	s=BASM_Parse_EatWhite(str);
//	if(!(*s))return(s);

	s=str;
	strcpy(b, opname);

//	s=BASM_Parse_Token(s, b, &ty);
//	BASM_Parse_Token(s, b2, &ty2);

//	if(!b[0])return(s);

	i=opnum;

	if(BASM_OpOnlySingleP(i))
	{
		BASM_OutOpSingle(ctx, i);
		if(*s==';')s++;
		return(s);
	}

	if((*s==';') || BASM_Parse_IsLineBreak(s) || (b2[0]=='|'))
	{
		BASM_OutOpSingle(ctx, i);
		if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
		if(*s==';')s++;
		return(s);
	}


	BASM_Parse_Token(s, b2, &ty2);

#if 0
	if(!basm_stricmp(b2, "byte")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=8; }
	if(!basm_stricmp(b2, "word")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=16; }
	if(!basm_stricmp(b2, "dword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=32; }
	if(!basm_stricmp(b2, "qword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=64; }
	if(!basm_stricmp(b2, "oword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=128; }
	if(!basm_stricmp(b2, "yword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=256; }
	if(!basm_stricmp(b2, "tword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=80; }

	if(!basm_stricmp(b2, "dqword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=128; }

	if(!basm_stricmp(b2, "short")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=8; }
	if(!basm_stricmp(b2, "near16")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=16; }
	if(!basm_stricmp(b2, "near32")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=32; }

//	BASM_Parse_Token(s, b2, &ty2);

	if(!basm_stricmp(b2, "ptr")) s=BASM_Parse_Token(s, b2, &ty2);
	if(!basm_stricmp(b2, "offset")) s=BASM_Parse_Token(s, b2, &ty2);
#endif

#if 0
	if(!stricmp(b2, "byte")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=8; }
	if(!stricmp(b2, "word")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=16; }
	if(!stricmp(b2, "dword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=32; }
	if(!stricmp(b2, "qword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=64; }
	if(!stricmp(b2, "oword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=128; }
	if(!stricmp(b2, "yword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=256; }
	if(!stricmp(b2, "tword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=80; }

	if(!stricmp(b2, "dqword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=128; }

	if(!stricmp(b2, "short")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=8; }
	if(!stricmp(b2, "near16")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=16; }
	if(!stricmp(b2, "near32")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=32; }

	if(!stricmp(b2, "ptr")) s=BASM_Parse_Token(s, b2, &ty2);
	if(!stricmp(b2, "offset")) s=BASM_Parse_Token(s, b2, &ty2);
#endif

#if 1
	if(ty2==BASM_TOKEN_NAME)
	{
		l0=basm_strdup(b2);

		if(l0==kw_byte) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=8; }
		if(l0==kw_word) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=16; }
		if(l0==kw_dword) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=32; }
		if(l0==kw_qword) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=64; }
		if(l0==kw_oword) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=128; }
		if(l0==kw_yword) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=256; }
		if(l0==kw_tword) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=80; }
		if(l0==kw_dqword) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=128; }

		if(l0==kw_short) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=8; }
		if(l0==kw_near16) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=16; }
		if(l0==kw_near32) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=32; }

		if(ctx->w)l0=basm_strdup(b2);

		if(l0==kw_ptr) s=BASM_Parse_Token(s, b2, &ty2);
		if(l0==kw_offset) s=BASM_Parse_Token(s, b2, &ty2);
	}
#endif

//	BASM_Parse_Token(s, b2, &ty2);

	s=BASM_ParseArg(ctx, s, &br0, &ir0, &sc0, &disp0, &l0);
	if(l0 && (*l0=='.') && ctx->label_base)
		{ sprintf(b2, "%s%s", ctx->label_base, l0);
		  l0=basm_strdup(b2); }
	BASM_Parse_Token(s, b2, &ty2);

	if((*s==';') || BASM_Parse_IsLineBreak(s) || (b2[0]=='|'))
	{
		BASM_OutOpGeneric1(ctx, i, ctx->w, l0, br0, ir0, sc0, disp0);

		BASM_Parse_Token(s, b2, &ty2);
		if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
		if(*s==';')s++;
		return(s);
	}

	if(b2[0]==',')s=BASM_Parse_Token(s, b2, &ty2);

	s=BASM_ParseArg(ctx, s, &br1, &ir1, &sc1, &disp1, &l1);
	if(l1 && (*l1=='.') && ctx->label_base)
		{ sprintf(b2, "%s%s", ctx->label_base, l1);
		  l1=basm_strdup(b2); }
	BASM_Parse_Token(s, b2, &ty2);

	if((*s==';') || BASM_Parse_IsLineBreak(s) || (b2[0]=='|'))
	{
		if(ctx->fl&BASM_FL_GAS)
		{
			BASM_OutOpGeneric2(ctx, i, ctx->w,
				l1, br1, ir1, sc1, disp1,
				l0, br0, ir0, sc0, disp0);
		}else
		{
			BASM_OutOpGeneric2(ctx, i, ctx->w,
				l0, br0, ir0, sc0, disp0,
				l1, br1, ir1, sc1, disp1);
		}

		BASM_Parse_Token(s, b2, &ty2);
		if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
		if(*s==';')s++;
		return(s);
	}

	BASM_Parse_Token(s, b2, &ty2);
	if(!basm_stricmp(b2, ","))s=BASM_Parse_Token(s, b2, &ty2);

	s=BASM_ParseArg(ctx, s, &br2, &ir2, &sc2, &disp2, &l2);
	if(l2 && (*l2=='.') && ctx->label_base)
		{ sprintf(b2, "%s%s", ctx->label_base, l2);
		  l2=basm_strdup(b2); }
	BASM_Parse_Token(s, b2, &ty2);

	if((*s==';') || BASM_Parse_IsLineBreak(s) || !(b2[0]=='|'))
	{
		if(ctx->fl&BASM_FL_GAS)
		{
			BASM_OutOpGeneric3(ctx, i, ctx->w,
				l2, br2, ir2, sc2, disp2,
				l1, br1, ir1, sc1, disp1,
				l0, br0, ir0, sc0, disp0);
		}else
		{
			BASM_OutOpGeneric3(ctx, i, ctx->w,
				l0, br0, ir0, sc0, disp0,
				l1, br1, ir1, sc1, disp1,
				l2, br2, ir2, sc2, disp2);
		}

		BASM_Parse_Token(s, b2, &ty2);
		if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
		if(*s==';')s++;
		return(s);
	}

	printf("BASM_ParseOpcode: Opcode Parse Error\n");

	BASM_Parse_Token(s, b2, &ty2);
	if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
	if(*s==';')s++;
	return(s);
}

BASM_API char *BASM_ParseOpcode(BASM_Context *ctx, char *str)
{
	char b[64], b2[64];
	char *s, *t, *l0, *l1, *l2;
	int ty, ty2;
	int i, j, opnum;

	long long disp0, disp1, disp2;
	int br0, ir0, sc0;
	int br1, ir1, sc1;
	int br2, ir2, sc2;

	b[0]=0; b2[0]=0;

	s=BASM_Parse_EatWhite(str);
	if(!(*s))return(s);

	s=BASM_Parse_Token(s, b, &ty);
//	BASM_Parse_Token(s, b2, &ty2);

	if(!b[0])return(s);

//	if(!strcmp(b, "["))
	if(b[0]=='[')
	{
		s=BASM_Parse_Token(s, b, &ty);
		t=BASM_ParseSpecial(ctx, s, b);
		if(t)
		{
			s=t;
			BASM_Parse_Token(s, b, &ty);
			if(!strcmp(b, "]"))
				s=BASM_Parse_Token(s, b, &ty);
			return(s);
		}

		//fail, seek to next ']'

		while(*s && (*s!=']'))s++;
		if(*s==']')s++;
		return(s);
	}

#if 1
	if(ty==BASM_TOKEN_NAME)
	{
		if(*s==':')
		{
			s++;

			if(b[0]=='.')
			{
				if(!ctx->label_base)
				{
					printf("BASM_ParseOpcode: "
						"local label '%s' absent base\n", b);
					return(s);
				}

				BASM_BundleAlign(ctx);
				sprintf(b2, "%s%s", ctx->label_base, b);
				BASM_EmitLabel(ctx, b2);
				return(s);
			}else
			{
				ctx->label_base=basm_strdup(b);
			}

			BASM_BundleAlign(ctx);
			BASM_EmitLabel(ctx, b);
			return(s);
		}

		opnum=BASM_LookupOpcode(ctx, b);

		if(!basm_ops[opnum] && (ctx->cpu==BASM_CPU_ARM))
			opnum=BASM_LookupOpcodeARMCC(ctx, b);

		if(basm_ops[opnum])
		{
			ctx->fl&=~(BASM_FL_ADDRSZ|BASM_FL_GAS);
			ctx->seg=BASM_Z;
			ctx->w=0;

			s=BASM_ParseOpcodeInner(ctx, s, b, opnum);
			return(s);
		}
	}else
	{
		printf("BASM_ParseOpcode: Bad Opcode Token '%s'\n", b);
		return(s);
	}
#endif

	BASM_Parse_Token(s, b2, &ty2);

	if(	(!strcmp(b2, ":") || !basm_stricmp(b2, "db") ||
		!basm_stricmp(b2, "dw") || !basm_stricmp(b2, "dd") ||
		!basm_stricmp(b2, "dq") || !basm_stricmp(b2, "resb") ||
		!basm_stricmp(b2, "resw") || !basm_stricmp(b2, "resd") ||
		!basm_stricmp(b2, "resq") ))
	{
		if(!strcmp(b2, ":"))
			s=BASM_Parse_Token(s, b2, &ty2);

		if(b[0]=='.')
		{
			if(!ctx->label_base)
			{
				printf("BASM_ParseOpcode: "
					"local label '%s' absent base\n", b);
				return(s);
			}

			sprintf(b2, "%s%s", ctx->label_base, b);
			BASM_EmitLabel(ctx, b2);
			return(s);
		}else
		{
			ctx->label_base=basm_strdup(b);
		}

		BASM_EmitLabel(ctx, b);
		return(s);
	}

#if 1
	if(!basm_stricmp(b2, "equ"))
	{
		s=BASM_Parse_Token(s, b2, &ty2);		//equ
		s=BASM_Parse_Token(s, b2, &ty2);		//value

		i=1;
		if(!strcmp(b2, "-"))
		{
			i=-1;
			s=BASM_Parse_Token(s, b2, &ty2);		//value
		}
			

		BASM_EmitConst(ctx, b, i*basm_atoi(b2));
		return(s);
	}
#endif

#if 1
	if(!basm_stricmp(b, ".equ"))
	{
		s=BASM_Parse_Token(s, b, &ty);			//name
		s=BASM_Parse_Token(s, b2, &ty2);		//,
		s=BASM_Parse_Token(s, b2, &ty2);		//value

		i=1;
		if(!strcmp(b2, "-"))
		{
			i=-1;
			s=BASM_Parse_Token(s, b2, &ty2);		//value
		}
			

		BASM_EmitConst(ctx, b, i*basm_atoi(b2));
		return(s);
	}
#endif

#if 1
//	i=BASM_LookupOpcode(ctx, b);
	i=opnum;
#endif

	ctx->fl&=~(BASM_FL_ADDRSZ|BASM_FL_GAS);
	ctx->seg=BASM_Z;
	ctx->w=0;

	if(!basm_stricmp(b, "a16"))
	{
		if(!(ctx->fl&2))ctx->fl|=BASM_FL_ADDRSZ;

		s=BASM_Parse_Token(s, b, &ty);
		BASM_Parse_Token(s, b2, &ty2);
		i=BASM_LookupOpcode(ctx, b);
	}

	if(!basm_stricmp(b, "a32"))
	{
		if(ctx->fl&2)ctx->fl|=BASM_FL_ADDRSZ;

		s=BASM_Parse_Token(s, b, &ty);
		BASM_Parse_Token(s, b2, &ty2);
		i=BASM_LookupOpcode(ctx, b);
	}

	if(!basm_ops[i])
	{
		j=b[strlen(b)-1];
		if((j=='b') || (j=='w') || (j=='l'))
		{
			ctx->fl|=BASM_FL_GAS;
			if(j=='b')ctx->w=8;
			if(j=='w')ctx->w=16;
			if(j=='l')ctx->w=32;
			b[strlen(b)-1]=0;
//			b[j]=0;

			i=BASM_LookupOpcode(ctx, b);

		}else if(b[0]=='l')
		{
			ctx->fl|=BASM_FL_GAS; ctx->w=32;

			i=BASM_LookupOpcode(ctx, b+1);
		}
	}

	if((i<0) || !basm_ops[i])
	{
		t=BASM_ParseSpecial(ctx, s, b);
		if(t)return(t);

		printf("BASM_ParseOpcode: Bad Opcode '%s'\n", b);
		return(s);
	}

	s=BASM_ParseOpcodeInner(ctx, s, b, i);
	return(s);
}
#endif

#if 0
BASM_API char *BASM_ParseOpcode(BASM_Context *ctx, char *str)
{
	char b[64], b2[64];
	char *s, *t, *l0, *l1, *l2;
	int ty, ty2;
	int i, j, opnum;

	long long disp0, disp1, disp2;
	int br0, ir0, sc0;
	int br1, ir1, sc1;
	int br2, ir2, sc2;

	b[0]=0; b2[0]=0;

	s=BASM_Parse_EatWhite(str);
	if(!(*s))return(s);

	s=BASM_Parse_Token(s, b, &ty);
	BASM_Parse_Token(s, b2, &ty2);

	if(!b[0])return(s);

	if(!strcmp(b, "["))
	{
		s=BASM_Parse_Token(s, b, &ty);
		t=BASM_ParseSpecial(ctx, s, b);
		if(t)
		{
			s=t;
			BASM_Parse_Token(s, b, &ty);
			if(!strcmp(b, "]"))
				s=BASM_Parse_Token(s, b, &ty);
			return(s);
		}

		//fail, seek to next ']'

		while(*s && (*s!=']'))s++;
		if(*s==']')s++;
		return(s);
	}

//	opnum=BASM_LookupOpcode(ctx, b);
#if 0
	if(!basm_ops[opnum])
	{
#endif

	if((ty==BASM_TOKEN_NAME) &&
		(!basm_stricmp(b2, ":") || !basm_stricmp(b2, "db") ||
		!basm_stricmp(b2, "dw") || !basm_stricmp(b2, "dd") ||
		!basm_stricmp(b2, "dq") || !basm_stricmp(b2, "resb") ||
		!basm_stricmp(b2, "resw") || !basm_stricmp(b2, "resd") ||
		!basm_stricmp(b2, "resq") ))
	{
		if(!basm_stricmp(b2, ":"))
			s=BASM_Parse_Token(s, b2, &ty2);

		if(b[0]=='.')
		{
			if(!ctx->label_base)
			{
				printf("BASM_ParseOpcode: "
					"local label '%s' absent base\n", b);
				return(s);
			}

			sprintf(b2, "%s%s", ctx->label_base, b);
			BASM_EmitLabel(ctx, b2);
			return(s);
		}else
		{
			ctx->label_base=basm_strdup(b);
		}

		BASM_EmitLabel(ctx, b);
		return(s);
	}

#if 1
	if((ty==BASM_TOKEN_NAME) && !basm_stricmp(b2, "equ") )
	{
		s=BASM_Parse_Token(s, b2, &ty2);		//equ
		s=BASM_Parse_Token(s, b2, &ty2);		//value

		i=1;
		if(!basm_stricmp(b2, "-"))
		{
			i=-1;
			s=BASM_Parse_Token(s, b2, &ty2);		//value
		}
			

		BASM_EmitConst(ctx, b, i*basm_atoi(b2));
		return(s);
	}
#endif

#if 0
	}
#endif

	if(ty!=BASM_TOKEN_NAME)
	{
		printf("BASM_ParseOpcode: Bad Opcode Token '%s'\n", b);
		return(s);
	}

#if 1
	i=BASM_LookupOpcode(ctx, b);
//	i=opnum;
	if(i<0)
	{
		t=BASM_ParseSpecial(ctx, s, b);
		if(t)return(t);

		printf("BASM_ParseOpcode: Bad Opcode '%s'\n", b);
		return(s);
	}
#endif

//	t=BASM_ParseSpecial(ctx, s, b);
//	if(t)return(t);

//	ctx->fl&=~8;

	ctx->fl&=~(BASM_FL_ADDRSZ|BASM_FL_GAS);

	ctx->seg=BASM_Z;
	ctx->w=0;

	if(!basm_stricmp(b, "a16"))
	{
		if(!(ctx->fl&2))ctx->fl|=BASM_FL_ADDRSZ;
//		t=BASM_ParseSpecial(ctx, s, b);

		s=BASM_Parse_Token(s, b, &ty);
		BASM_Parse_Token(s, b2, &ty2);
		i=BASM_LookupOpcode(ctx, b);
	}

	if(!basm_stricmp(b, "a32"))
	{
		if(ctx->fl&2)ctx->fl|=BASM_FL_ADDRSZ;
//		t=BASM_ParseSpecial(ctx, s, b);

		s=BASM_Parse_Token(s, b, &ty);
		BASM_Parse_Token(s, b2, &ty2);
		i=BASM_LookupOpcode(ctx, b);
	}

#if 0
	i=BASM_LookupOpcode(ctx, b);
	if(i<0)
	{
		printf("BASM_ParseOpcode: Bad Opcode '%s'\n", b);
		return(s);
	}
#endif

	if(!basm_ops[i])
	{
		j=b[strlen(b)-1];
		if((j=='b') || (j=='w') || (j=='l'))
		{
			ctx->fl|=BASM_FL_GAS;
			if(j=='b')ctx->w=8;
			if(j=='w')ctx->w=16;
			if(j=='l')ctx->w=32;
			b[j]=0;

			i=BASM_LookupOpcode(ctx, b);

		}else if(b[0]=='l')
		{
			ctx->fl|=BASM_FL_GAS; ctx->w=32;

			i=BASM_LookupOpcode(ctx, b+1);
		}
	}

	if((i<0) || !basm_ops[i])
	{
		printf("BASM_ParseOpcode: Bad Opcode '%s'\n", b);
		return(s);
	}

	if(BASM_OpOnlySingleP(i))
	{
		BASM_OutOpSingle(ctx, i);
		if(*s==';')s++;
		return(s);
	}

	if((*s==';') || BASM_Parse_IsLineBreak(s) || !strcmp(b2, "|"))
	{
		BASM_OutOpSingle(ctx, i);
		if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
		if(*s==';')s++;
		return(s);
	}

	BASM_Parse_Token(s, b2, &ty2);


	if(!basm_stricmp(b2, "byte")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=8; }
	if(!basm_stricmp(b2, "word")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=16; }
	if(!basm_stricmp(b2, "dword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=32; }
	if(!basm_stricmp(b2, "qword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=64; }
	if(!basm_stricmp(b2, "oword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=128; }
	if(!basm_stricmp(b2, "yword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=256; }
	if(!basm_stricmp(b2, "tword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=80; }

	if(!basm_stricmp(b2, "dqword")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=128; }

	if(!basm_stricmp(b2, "short")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=8; }
	if(!basm_stricmp(b2, "near16")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=16; }
	if(!basm_stricmp(b2, "near32")) { s=BASM_Parse_Token(s, b2, &ty2); ctx->w=32; }

	BASM_Parse_Token(s, b2, &ty2);

	if(!basm_stricmp(b2, "ptr")) s=BASM_Parse_Token(s, b2, &ty2);
	if(!basm_stricmp(b2, "offset")) s=BASM_Parse_Token(s, b2, &ty2);

	BASM_Parse_Token(s, b2, &ty2);

	s=BASM_ParseArg(ctx, s, &br0, &ir0, &sc0, &disp0, &l0);
	if(l0 && (*l0=='.') && ctx->label_base)
		{ sprintf(b2, "%s%s", ctx->label_base, l0);
		  l0=basm_strdup(b2); }
	BASM_Parse_Token(s, b2, &ty2);

//	if(!basm_stricmp(b2, ";") || BASM_Parse_IsLineBreak(s))
	if((*s==';') || BASM_Parse_IsLineBreak(s) || !strcmp(b2, "|"))
	{
		BASM_OutOpGeneric1(ctx, i, ctx->w, l0, br0, ir0, sc0, disp0);

		BASM_Parse_Token(s, b2, &ty2);
		if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
		if(*s==';')s++;
		return(s);
	}

	if(!strcmp(b2, ","))s=BASM_Parse_Token(s, b2, &ty2);

	s=BASM_ParseArg(ctx, s, &br1, &ir1, &sc1, &disp1, &l1);
	if(l1 && (*l1=='.') && ctx->label_base)
		{ sprintf(b2, "%s%s", ctx->label_base, l1);
		  l1=basm_strdup(b2); }
	BASM_Parse_Token(s, b2, &ty2);

	if((*s==';') || BASM_Parse_IsLineBreak(s) || !strcmp(b2, "|"))
	{
		if(ctx->fl&BASM_FL_GAS)
		{
			BASM_OutOpGeneric2(ctx, i, ctx->w,
				l1, br1, ir1, sc1, disp1,
				l0, br0, ir0, sc0, disp0);
		}else
		{
			BASM_OutOpGeneric2(ctx, i, ctx->w,
				l0, br0, ir0, sc0, disp0,
				l1, br1, ir1, sc1, disp1);
		}

		BASM_Parse_Token(s, b2, &ty2);
		if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
		if(*s==';')s++;
		return(s);
	}

	BASM_Parse_Token(s, b2, &ty2);
	if(!basm_stricmp(b2, ","))s=BASM_Parse_Token(s, b2, &ty2);

	s=BASM_ParseArg(ctx, s, &br2, &ir2, &sc2, &disp2, &l2);
	if(l2 && (*l2=='.') && ctx->label_base)
		{ sprintf(b2, "%s%s", ctx->label_base, l2);
		  l2=basm_strdup(b2); }
	BASM_Parse_Token(s, b2, &ty2);

	if((*s==';') || BASM_Parse_IsLineBreak(s) || !strcmp(b2, "|"))
	{
		if(ctx->fl&BASM_FL_GAS)
		{
			BASM_OutOpGeneric3(ctx, i, ctx->w,
				l2, br2, ir2, sc2, disp2,
				l1, br1, ir1, sc1, disp1,
				l0, br0, ir0, sc0, disp0);
		}else
		{
			BASM_OutOpGeneric3(ctx, i, ctx->w,
				l0, br0, ir0, sc0, disp0,
				l1, br1, ir1, sc1, disp1,
				l2, br2, ir2, sc2, disp2);
		}

		BASM_Parse_Token(s, b2, &ty2);
		if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
		if(*s==';')s++;
		return(s);
	}

	printf("BASM_ParseOpcode: Opcode Parse Error\n");

	BASM_Parse_Token(s, b2, &ty2);
//	if(!basm_stricmp(b2, ";"))s=BASM_Parse_Token(s, b2, &ty2);
	if(b2[0]=='|')s=BASM_Parse_Token(s, b2, &ty2);
	if(*s==';')s++;
	return(s);
}
#endif

BASM_API char *BASM_ParseOpcodeList(BASM_Context *ctx, char *str)
{
	char *s;

	s=str;

	while(*s)
	{
		if(ctx->seg==0)
		{
			while(*s && (*s!='\n'))s++;
			if(*s=='\n')ctx->seg=BASM_Z;
			continue;
		}

		while(*s && (*s<=' '))s++;
		if(!*s)break;

		if((*s==';') || (*s=='#'))
		{
			ctx->seg=0;
//			printf("ASM: Comment\n");
			while(*s && (*s!='\n'))s++;
			if(*s=='\n')ctx->seg=BASM_Z;
			continue;
		}

		s=BASM_ParseOpcode(ctx, s);
	}

	return(s);
}

