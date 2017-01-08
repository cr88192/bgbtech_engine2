#include <bteifgl.h>

void *bgbgc_ralloc_mutex=NULL;

static char *frgl_strtab=NULL;
static char *frgl_strtabe=NULL;
static char *frgl_estrtab=NULL;
static char **frgl_strhash=NULL;


// BTEIFGL_API char *frgl_strdup(char *str)
//	{ return(strdup(str)); }

#ifdef __EMSCRIPTEN__
#define frgl_dbgprn_ems	frgl_printf
#else

#define frgl_dbgprn_ems	frgl_printf_null
BTEIFGL_API void frgl_printf_null(char *str, ...)
{
}

#endif

BTEIFGL_API char *frgl_strdup(char *str)
{
	char *s, *t;
	int i, l;

	if(!str)
	{
//		*(int *)-1=-1;
		return(NULL);
	}
	if(!*str)return("");

	if(!frgl_strhash)
	{
		frgl_dbgprn_ems("frgl_strdup %d\n", sizeof(char *));

		frgl_strtab=frgl_malloc(1<<20);
//		frgl_strtab=frgl_tyalloc("frgl_strtab_t", 1<<20);
		frgl_strtabe=frgl_strtab;
		frgl_estrtab=frgl_strtab+(1<<20);

//		i=16384*sizeof(char *);
		i=4096*sizeof(char *);
		frgl_strhash=frgl_malloc(i);
		memset(frgl_strhash, 0, i);
	}
	
	if((frgl_strtabe+(strlen(str)+2))>=frgl_estrtab)
	{
		frgl_strtab=frgl_malloc(1<<20);
//		frgl_strtab=frgl_tyalloc("frgl_strtab_t", 1<<20);
		frgl_strtabe=frgl_strtab;
		frgl_estrtab=frgl_strtab+(1<<20);
	}

	i=0; s=str;
	while(*s)i=i*251+(*s++);
	i=((i*251)>>8)&0xFFF;
//	i=((i*251)>>8)&0x3FFF;

	t=frgl_strhash[i];
	while(t)
	{
		s=(char *)(((char **)t)+1);
		t=*(char **)t;
		if(!strcmp(s, str))return(s);
	}

//	t=frgl_malloc(strlen(str)+1+sizeof(char *));
//	t=frgl_tyalloc("frgl_string_t", strlen(str)+1+sizeof(char *));

	l=strlen(str)+1+sizeof(char *);
	l=(l+7)&(~7);

	t=frgl_strtabe;
	frgl_strtabe=t+l;

	s=(char *)(((char **)t)+1);
	strcpy(s, str);

	*(char **)t=frgl_strhash[i];
	frgl_strhash[i]=t;
	return(s);
}


BTEIFGL_API void *frgl_ralloc(int sz)
{
	static char *buf=NULL;
	static int pos=0;
	char *s;

	if(!bgbgc_ralloc_mutex)
		bgbgc_ralloc_mutex=thFastMutex();

	thLockFastMutex(bgbgc_ralloc_mutex);

	if(!buf)buf=malloc(1<<20);
	if((pos+sz)>=(1<<20))pos=0;

	s=buf+pos;
	pos=(pos+sz+7)&(~7);

	thUnlockFastMutex(bgbgc_ralloc_mutex);
	return(s);
}

BTEIFGL_API char *frgl_rstrdup(char *str)
{
	char *t;
	t=frgl_ralloc(strlen(str)+1);
	strcpy(t, str);
	return(t);
}

BTEIFGL_API char *frgl_rsprint(char *str, ...)
{
	char buf[4096];
	va_list lst;
	char *t;

	va_start(lst, str);
	vsprintf(buf, str, lst);
	va_end(lst);

	t=frgl_rstrdup(buf);
	return(t);
}

BTEIFGL_API char *frgl_rsprints(char *buf, char *str, ...)
{
//	char buf[4096];
	va_list lst;
	char *t;

	va_start(lst, str);
	vsprintf(buf, str, lst);
	va_end(lst);

	t=frgl_rstrdup(buf);
	return(t);
}

BTEIFGL_API char **frgl_rsplit(char *s)
{
	char tb[256];
	char **a, *t;
	int i, j;

	a=frgl_ralloc(256*sizeof(char *));
	i=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;
//		t=frgl_ralloc(256);
//		a[i++]=t;

		t=tb;

		if(*s=='"')
		{
			s++;

			while(*s && (*s!='"'))
			{
				if(*s=='\\')
				{
					*t++=*s++;

					if(*s=='x')
					{
						*t++=*s++;
						*t++=*s++; *t++=*s++;
						continue;
					}
					if(*s=='u')
					{
						*t++=*s++;
						*t++=*s++; *t++=*s++;
						*t++=*s++; *t++=*s++;
						continue;
					}
					if(*s=='U')
					{
						*t++=*s++;
						*t++=*s++; *t++=*s++;
						*t++=*s++; *t++=*s++;
						*t++=*s++; *t++=*s++;
						*t++=*s++; *t++=*s++;
						continue;
					}

					*t++=*s++;
					continue;
				}

				*t++=*s++;
			}

			if(*s=='"')s++;

			*t++=0;
			*t++='\"';
			*t++=0;

			j=t-tb;
			t=frgl_ralloc(j);
			memcpy(t, tb, j);
			a[i++]=t;
			continue;
		}

		while(*s && (*s>' ') && (*s!='\"'))*t++=*s++;
		*t++=0;
		*t++=0;
		*t++=0;

		j=t-tb;
		t=frgl_ralloc(j);
		memcpy(t, tb, j);
		a[i++]=t;
	}
	a[i]=NULL;

	return(a);
}

BTEIFGL_API char *frgl_rsgets(char *buf, int lim, char *str)
{
	char *s, *t, *te;

	s=str; t=buf; te=t+lim-1;
	while(*s && (*s!='\n') && (*s!='\r') && (t<te))*t++=*s++;
	*t++=0;

	if(*s=='\r')s++;
	if(*s=='\n')s++;
	return(s);
}

BTEIFGL_API char *frgl_rgets(char **str)
{
	char *s, *s1, *b, *t;

	s=*str; s1=s;
	while(*s1 && (*s1!='\n') && (*s1!='\r'))s1++;

	b=frgl_ralloc((s1-s)+1);
	t=b;
	while(s<s1)*t++=*s++;
	*t++=0;

	if(*s=='\r')s++;
	if(*s=='\n')s++;
	*str=s;
	
	return(b);
}

BTEIFGL_API char *frgl_rgettoken(char **str)
{
	char *s, *s0, *s1, *b, *t;

	s=*str; s0=s;
	while(*s0 && (*s0<=' '))s0++;
	if(*s0=='\"')
	{
		s1=s0+1;
		while(*s1 && (*s1!='\"'))
		{
			if(*s1=='\\')
			{
				s1++;

				if(*s1=='x')
				{
					s1++; s1++; s1++;
					continue;
				}
				if(*s=='u')
				{
					s1++; s1++; s1++; s1++;
					continue;
				}
				if(*s=='U')
				{
					s1++; s1++; s1++; s1++;
					s1++; s1++; s1++; s1++; s1++;
					continue;
				}
				s1++;
				continue;
			}
			s1++;
		}
	}else
	{
		s1=s0;
		while(*s1 && (*s1>' '))s1++;
	}

	b=frgl_ralloc((s1-s0)+3);
	s=s0; t=b;
	while(s<s1)*t++=*s++;
	*t++=0;
	if(*s=='\"')
		{ *t++=*s++; }
	*str=s;
	return(b);
}

BTEIFGL_API char *frgl_rlltoa(s64 v)
{
	char *t;
	t=frgl_ralloc(64);
#ifdef _MSC_VER
	sprintf(t, "%I64d", v);
#else
	sprintf(t, "%lld", v);
#endif
	return(t);
}

BTEIFGL_API char *frgl_rdtoa(double v)
{
	char *t;
	if(((s64)v)==v)
		return(frgl_rlltoa((s64)v));
	t=frgl_ralloc(64);
	sprintf(t, "%f", v);
	return(t);
}

BTEIFGL_API char *frgl_rdtoa1p(double v)
{
	s64 i;
	i=(s64)((v>=0)?(v+0.5):(v-0.5));
	if(fabs(v-i)<0.1)return(frgl_rlltoa(i));
	return(frgl_rsprint("%.1f", v));
}

BTEIFGL_API char *frgl_rdtoa2p(double v)
{
	s64 i;
	i=(s64)((v>=0)?(v+0.5):(v-0.5));
	if(fabs(v-i)<0.01)return(frgl_rlltoa(i));
	return(frgl_rsprint("%.2f", v));
}

BTEIFGL_API char *frgl_rdtoa3p(double v)
{
	s64 i;
	i=(s64)((v>=0)?(v+0.5):(v-0.5));
	if(fabs(v-i)<0.001)return(frgl_rlltoa(i));
	return(frgl_rsprint("%.3f", v));
}

BTEIFGL_API char *frgl_rdtoa4p(double v)
{
	s64 i;
	i=(s64)((v>=0)?(v+0.5):(v-0.5));
	if(fabs(v-i)<0.0001)return(frgl_rlltoa(i));
	return(frgl_rsprint("%.4f", v));
}

BTEIFGL_API s64 frgl_ratoi(char *str)
{
	s64 l;
	char *s, *t;
	int i, sg;

	t=str;
	if((t[0]=='0') && (t[1]=='x'))
	{
		l=0; t+=2;
		while(*t)
		{
			if(*t=='_') { t++; continue; }
			l*=16;
			if((*t>='0') && (*t<='9'))l+=*t-'0';
			if((*t>='A') && (*t<='F'))l+=*t-'A'+10;
			if((*t>='a') && (*t<='f'))l+=*t-'a'+10;
			t++;
		}

		return(l);
	}

	if((t[0]=='0') && (t[1]=='d'))
	{
		t+=2; l=0; sg=0;
//		if(*t=='-') { sg=1; t++; }
		while(*t)
		{
			if(*t=='_') { t++; continue; }
			l*=10;
			if((*t>='0') && (*t<='9'))l+=*t-'0';
				else break;
			t++;
		}
//		if(sg)l=-l;
		return(l);
	}

	if((t[0]=='0') && (t[1]=='c'))
	{
		t+=2; l=0; sg=0;
//		if(*t=='-') { sg=1; t++; }
		while(*t)
		{
			if(*t=='_') { t++; continue; }
			l*=8;
			if((*t>='0') && (*t<='7'))l+=*t-'0';
				else break;
			t++;
		}
//		if(sg)l=-l;
		return(l);
	}

	if((t[0]=='0') && (t[1]=='b'))
	{
		t+=2; l=0; sg=0;
//		if(*t=='-') { sg=1; t++; }
		while(*t)
		{
			if(*t=='_') { t++; continue; }
			l*=2;
			if((*t>='0') && (*t<='1'))l+=*t-'0';
				else break;
			t++;
		}
//		if(sg)l=-l;
		return(l);
	}

	if(t[0]=='0')
	{
		l=0; t++;
		while(*t)
		{
			l*=8;
			if((*t>='0') && (*t<='7'))l+=*t-'0';
				else break;
			t++;
		}

		return(l);
	}

	t=str; l=0; sg=0;
	if(*t=='-') { sg=1; t++; }
	while(*t)
	{
		l*=10;
		if((*t>='0') && (*t<='9'))l+=*t-'0';
			else break;
		t++;
	}
	if(sg)l=-l;

	return(l);
}

BTEIFGL_API int frgl_stricmp(char *s1, char *s2)
{
	int c1, c2;

	c1=*s1; c2=*s2;
	while(*s1 && *s2)
	{
		c1=*s1++;
		c2=*s2++;
		if((c1>='a') && (c1<='z'))
			c1=(c1-'a')+'A';
		if((c2>='a') && (c2<='z'))
			c2=(c2-'a')+'A';
		if(c1!=c2)
			break;
	}
	
	if(c1>c2)return( 1);
	if(c2>c1)return(-1);
	return(0);
}
