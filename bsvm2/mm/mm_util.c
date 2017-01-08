/*
 * Misc utility functions.
 */

static char *bgbdt_mm_strtab=NULL;
static char *bgbdt_mm_strtabe=NULL;
static char *bgbdt_mm_estrtab=NULL;
static char **bgbdt_mm_strhash;


BS2VM_API void *bgbdt_mm_malloc(int sz)
{
	void *p;
	p=malloc(sz);
	memset(p, 0, sz);
	return(p);
}

BS2VM_API void *bgbdt_mm_realloc(void *ptr, int sz)
{
	return(realloc(ptr, sz));
}

BS2VM_API void bgbdt_mm_free(void *ptr)
{
	free(ptr);
}

BS2VM_API char *bgbdt_mm_strdup(char *str)
{
	char *s, *t;
	int i, l;

	if(!str)
	{
//		*(int *)-1=-1;
		return(NULL);
	}
	if(!*str)return("");

	if(!bgbdt_mm_strhash)
	{
		bgbdt_mm_strtab=bgbdt_mm_malloc(1<<20);
		bgbdt_mm_strtabe=bgbdt_mm_strtab;
		bgbdt_mm_estrtab=bgbdt_mm_strtab+(1<<20);

		i=16384*sizeof(char *);
		bgbdt_mm_strhash=bgbdt_mm_malloc(i);
		memset(bgbdt_mm_strhash, 0, i);
	}
	
	if((bgbdt_mm_strtabe+(strlen(str)+2))>=bgbdt_mm_estrtab)
	{
		bgbdt_mm_strtab=bgbdt_mm_malloc(1<<20);
//		bgbdt_mm_strtab=bgbdt_mm_tyalloc("bgbdt_mm_strtab_t", 1<<20);
		bgbdt_mm_strtabe=bgbdt_mm_strtab;
		bgbdt_mm_estrtab=bgbdt_mm_strtab+(1<<20);
	}

	i=0; s=str;
	while(*s)i=i*251+(*s++);
//	i=((i*251)>>8)&0xFFF;
	i=((i*251)>>8)&0x3FFF;

	t=bgbdt_mm_strhash[i];
	while(t)
	{
		s=(char *)(((char **)t)+1);
		t=*(char **)t;
		if(!strcmp(s, str))return(s);
	}

//	t=bgbdt_mm_malloc(strlen(str)+1+sizeof(char *));
//	t=bgbdt_mm_tyalloc("bgbdt_mm_string_t", strlen(str)+1+sizeof(char *));

	l=strlen(str)+1+sizeof(char *);
	l=(l+7)&(~7);
	t=bgbdt_mm_strtabe;
	bgbdt_mm_strtabe=t+l;

	s=(char *)(((char **)t)+1);
	strcpy(s, str);

	*(char **)t=bgbdt_mm_strhash[i];
	bgbdt_mm_strhash[i]=t;
	return(s);
}

BS2VM_API void *bgbdt_mm_ralloc(int sz)
{
	static char *buf=NULL;
	static int pos=0;
	char *s;

//	if(!bgbgc_ralloc_mutex)
//		bgbgc_ralloc_mutex=thFastMutex();
//	thLockFastMutex(bgbgc_ralloc_mutex);

	if(!buf)buf=malloc(1<<20);
	if((pos+sz)>=(1<<20))pos=0;

	s=buf+pos;
	pos=(pos+sz+7)&(~7);

//	thUnlockFastMutex(bgbgc_ralloc_mutex);
	return(s);
}

BS2VM_API char *bgbdt_mm_rstrdup(char *str)
{
	char *t;
	t=bgbdt_mm_ralloc(strlen(str)+1);
	strcpy(t, str);
	return(t);
}

BS2VM_API s64 bgbdt_mm_ratoi(char *str)
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

//void *bgbdt_mm_loadfile(char *name, int *rsz)
//{
//	return(basm_loadfile(name, rsz));
//}

BS2VM_API char **bgbdt_mm_rsplit(char *s)
{
	char tb[256];
	char **a, *t;
	int i, j;

	a=bgbdt_mm_ralloc(256*sizeof(char *));
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
			t=bgbdt_mm_ralloc(j);
			memcpy(t, tb, j);
			a[i++]=t;
			continue;
		}

		while(*s && (*s>' ') && (*s!='\"'))*t++=*s++;
		*t++=0;
		*t++=0;
		*t++=0;

		j=t-tb;
		t=bgbdt_mm_ralloc(j);
		memcpy(t, tb, j);
		a[i++]=t;
	}
	a[i]=NULL;

	return(a);
}

BS2VM_API char *bgbdt_mm_rsgets(char *buf, int lim, char *str)
{
	char *s, *t, *te;

	s=str; t=buf; te=t+lim-1;
	while(*s && (*s!='\n') && (*s!='\r') && (t<te))*t++=*s++;
	*t++=0;

	if(*s=='\r')s++;
	if(*s=='\n')s++;
	return(s);
}
