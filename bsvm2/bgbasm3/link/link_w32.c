// #include <bgblink.h>
// #include <bgbasm.h>

#ifdef WIN32

#include <windows.h>
#include <tlhelp32.h>
#include <dbghelp.h>

HMODULE blnk_hmodapp=NULL;

char *blnk_hmodname[256];
HMODULE blnk_hmoddll[256];
int blnk_numdll=0;

static char *blnk_os_envarr[256];
static char **blnk_os_env=NULL;

BOOL (WINAPI *SymFromAddr_f) (
  __in HANDLE hProcess, __in DWORD64 Address,
  __out PDWORD64 Displacement,
  __inout PSYMBOL_INFO Symbol );

DWORD (WINAPI *SymSetOptions_f) ( __in DWORD SymOptions );

BOOL (WINAPI *SymInitialize_f) (
	__in HANDLE hProcess, __in PCTSTR UserSearchPath,
	__in BOOL fInvadeProcess );


BASM_API int BLNK_GetModuleListOS(char **buf, int max)
{
	int i, n;

	n=blnk_numdll;
	if(n>max)n=max;
	for(i=0; i<n; i++)
		buf[i]=blnk_hmodname[i];
	return(n);
}

BASM_API char **BLNK_GetEnvironOS()
{
	static char *ta[256];
	char *s, *buf;
	int n;

	if(blnk_os_env)
		return(blnk_os_env);

	buf=GetEnvironmentStrings();

	blnk_os_env=blnk_os_envarr;
	s=buf; n=0;
	while(*s)
	{
		blnk_os_env[n++]=blnk_strdup(s);
		s+=strlen(s)+1;
	}
	blnk_os_env[n]=NULL;
	
	FreeEnvironmentStrings(buf);
	return(blnk_os_env);
}

void BLNK_ProbeTst(byte *buf, int sz)
{
	int i, j;

	for(i=0; i<(sz/4096); i++)
		j=buf[i*4096+2048];
}

void BLNK_UpdateDllsOS(void *hmod)
{
	static int nrec=0;
	HMODULE hdl;
	DWORD dwPid;
	HANDLE hSnap;
	MODULEENTRY32 me32;

	if(nrec)
		return;
	nrec++;

	hdl=(HMODULE)hmod;

	dwPid=GetCurrentProcessId();
	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);

	if(hSnap!=INVALID_HANDLE_VALUE)
	{
		me32.dwSize=sizeof(MODULEENTRY32);
		if(Module32First(hSnap, &me32))
		{
			while(1)
			{
//				printf("(MOD) %s %p %d\n",
//					me32.szModule,
//					me32.modBaseAddr, me32.modBaseSize);

//				BLNK_ProbeTst(me32.modBaseAddr, me32.modBaseSize);

				if(!hdl || (me32.hModule==hdl))
				{
					BLNK_AddExeSection(me32.szModule,
						(s64)me32.modBaseAddr,
						me32.modBaseSize, 0);
				}

				BLNK_LoadDynamicOS(me32.szModule);
				if(!Module32Next(hSnap, &me32))
					break;
			}
		}
		CloseHandle(hSnap);
	}
	nrec--;
}

int BLNK_InitDllsOS()
{
	static int init=0;

	if(init)return(0);
	init=1;

	if(!blnk_hmodapp)
		blnk_hmodapp=GetModuleHandle(NULL);

	BLNK_UpdateDllsOS(NULL);

	return(1);
}

int BLNK_LoadDynamicOS(char *name)
{
	char buf[256];
	HMODULE hdl;
	char *s;
	int i;

	BLNK_InitDllsOS();

	for(i=0; i<blnk_numdll; i++)
		if(!strcmp(blnk_hmodname[i], name))
			return(0);

	if(blnk_lookup_blacklist(name)>0)
		return(-1);

	//LoadLibrary does not like '/'...
	strcpy(buf, name);
	s=buf; while(*s) { if(*s=='/')*s='\\'; s++; }

	hdl=GetModuleHandle(buf);
	if(!hdl)hdl=LoadLibrary(buf);

	if(hdl)
	{
//		printf("BLNK_LoadDynamicOS: Pass Load lib %s\n", name);

		i=blnk_numdll++;
		blnk_hmodname[i]=blnk_strdup(name);
		blnk_hmoddll[i]=hdl;

		BLNK_UpdateDllsOS(hdl);
		return(0);
	}

	printf("BLNK_LoadDynamicOS: Fail Load lib %s\n", name);

	return(-1);
}

BASM_API void *BLNK_LoadModuleFileDataOS(char *name, int *rsz)
{
	char nbuf[1024];
	HMODULE hdl;
	HANDLE hFile;
	LARGE_INTEGER szFile;
	DWORD dwBytesRead;
	void *buf;
	char *s;
	int i, sz;

	BLNK_InitDllsOS();

	if(blnk_lookup_blacklist(name)>0)
		return(NULL);

	hdl=GetModuleHandle(name);
	i=GetModuleFileName(hdl, nbuf, 1024);
	if(!i)return(NULL);

	hFile=CreateFile(nbuf,		// module name
		GENERIC_READ,			// open for reading
		FILE_SHARE_READ,		// share for reading
		NULL,					// default security
		OPEN_EXISTING,			// existing file only
		FILE_ATTRIBUTE_NORMAL,	// normal file
		NULL);					// no attr. template

	if (hFile==INVALID_HANDLE_VALUE) 
		return(NULL);

	if(!GetFileSizeEx(hFile, &szFile))
	{
		CloseHandle(hFile);
		return(NULL);
	}

	sz=szFile.QuadPart;

	//fail file is overly large
	if(sz>(1<<26))
	{
		CloseHandle(hFile);
		return(NULL);
	}

	buf=malloc(sz);

	if(!ReadFile(hFile, buf, sz, &dwBytesRead, NULL))
	{
//	printf("Could not read from file (error %d)\n", GetLastError());
		CloseHandle(hFile);
		return(NULL);
	}

	CloseHandle(hFile);
	if(rsz)*rsz=sz;
	return(buf);
}

/** load file data, if it is an ExWAD image */
BASM_API void *BLNK_LoadModuleWadDataOS(char *name, int *rsz)
{
	void *p;
	byte *pb;
	int i, sz;

	p=BLNK_LoadModuleFileDataOS(name, &sz);
	if(!p)return(NULL);
	
	pb=(byte *)p;
	for(i=0; i<(sz/16); i++)
	{
//		if(!memcmp(pb+i*16, "ExWAD100", 8))
//		if(!memcmp(pb+i*16, "ExWAD100", 8) &&
//			!memcmp(pb+(i+1)*16, "DAWxE\r\n\xFF", 8))

		if(!memcmp(pb+i*16, "ExWAD101", 6) &&
			!memcmp(pb+(i+1)*16, "DAWxE\r\n\xFF", 8))
		{
			if(rsz)*rsz=sz;
			return(p);
		}
	}

	//no ExWAD sig found
	free(p);
	return(NULL);
}

BASM_API void BLNK_FreeModuleDataOS(void *buf)
{
	free(buf);
}

void BLNK_UpdateProxyOS(char *name, void *ptr)
{
	char ntmp[256];
	void *p;
	int i;

	BLNK_InitDllsOS();

#ifdef X86_32
	sprintf(ntmp, "__iproxy_%s", name);
#else
	sprintf(ntmp, "_iproxy_%s", name);
#endif

	if(blnk_hmodapp)
	{
		p=(void *)GetProcAddress(blnk_hmodapp, TEXT(ntmp));
		if(p)*(void **)p=ptr;
	}

	for(i=0; i<blnk_numdll; i++)
	{
		p=(void *)GetProcAddress(blnk_hmoddll[i], TEXT(ntmp));
		if(p)*(void **)p=ptr;
	}
}

void *BLNK_LookupLabelOS_1(char *name)
{
	char ntmp[64];
	void *p, *q;
	int i;

	p=NULL;

	if(blnk_hmodapp)
	{
		p=(void *)GetProcAddress(blnk_hmodapp, TEXT(name));
		if(p)return(p);
	}

	for(i=0; i<blnk_numdll; i++)
	{
		p=(void *)GetProcAddress(blnk_hmoddll[i], TEXT(name));
		if(p)break;
	}

	return(p);
}

void *BLNK_LookupLabelOS_2(char *name, int *rfl)
{
	char ntmp[256];
	void *p;
	int i;

	*rfl=0;

	if(*name!='_')
	{
		*rfl|=1;

		for(i=9; i>0; i--)
		{
			sprintf(ntmp, "BSRT%d__%s", i, name);
			p=BLNK_LookupLabelOS_1(ntmp);
			if(p)break;
		}

		if(p)return(p);

		sprintf(ntmp, "BSRT__%s", name);
		p=BLNK_LookupLabelOS_1(ntmp);
		if(p)return(p);

		*rfl&=~1;
	}

	p=BLNK_LookupLabelOS_1(name);
	return(p);
}

void *BLNK_LookupLabelOS(char *name)
{
	void *p, *q;
	int fl;

	BLNK_InitDllsOS();

	if(blnk_lookup_blacklist(name)>0)
		return(NULL);
	if((*name=='_') && (blnk_lookup_blacklist(name+1)>0))
		return(NULL);

//	printf("BLNK_LookupLabelOS: module %p\n", blnk_hmodapp);

	p=BLNK_LookupLabelOS_2(name, &fl);

#ifdef X86_64
//	if(p && (((s64)p)&0xFFFFFFFF00000000LL) )
	if(p && BLNK_CheckPtrBigSpace(p))
	{
		BLNK_AddProxyPtr(name, p);
		q=BLNK_FetchSym(name);
//		printf("BLNK_LookupLabelOS: Proxy %s->%p @%p\n", name, p, q);
		return(q);
	}
#endif

	if(p && (fl&1))
	{
		BLNK_AddProxyPtr(name, p);
		q=BLNK_FetchSym(name);
		return(q);
	}

	return(p);
}

int BLNK_InitDbgHelpOS()
{
	static int init=0;
	DWORD dwPid;
	HANDLE hSnap;
	MODULEENTRY32 me32;

	if(init)return(0);
	init=1;

	BLNK_LoadDynamicOS("dbghelp");

	SymFromAddr_f=BLNK_LookupLabel("SymFromAddr");
	SymSetOptions_f=BLNK_LookupLabel("SymSetOptions");
	SymInitialize_f=BLNK_LookupLabel("SymInitialize");

	if(!SymFromAddr_f || !SymSetOptions_f || !SymInitialize_f)
	{
		printf("Failed load dbgHelp\n");
		return(-1);
	}

	SymSetOptions_f(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	SymInitialize_f(GetCurrentProcess(), NULL, TRUE);

	return(1);
}

char *BLNK_LookupAddrNameOS(void *addr, void **addr2)
{
	char buf[1024];
	HANDLE hProcess;
	DWORD64 offs;
	SYMBOL_INFO *pSym;
	int i;

	BLNK_InitDbgHelpOS();

	if(!SymFromAddr_f)
		return(NULL);

	memset(buf, 0, 1024);
	pSym=(SYMBOL_INFO *)buf;
	pSym->SizeOfStruct=sizeof(SYMBOL_INFO);
	pSym->MaxNameLen=1024-sizeof(SYMBOL_INFO);

	hProcess=GetCurrentProcess();
	i=SymFromAddr_f(hProcess, (DWORD64)addr, &offs, pSym);

	if(i)
	{
		if(addr2)*addr2=(void *)(pSym->Address);
		return(blnk_strdup(pSym->Name));
	}

	if(addr2)*addr2=NULL;
	return(NULL);
}

int BLNK_ProcessMAP(char *name)
{
	char nbuf[256], tb[256], tb1[32];
	FILE *fd;
	char *s, *t;
	s64 l;
	int i;

	fd=fopen(name, "rb");

	if(!fd)
	{
		sprintf(nbuf, "%s.map", name);
		fd=fopen(nbuf, "rb");
	}

	if(!fd)
	{
		printf("BLNK_ProcessMAP: fail open\n");
		return(-1);
	}

	printf("BLNK_ProcessMAP: Load Map '%s'\n", name);

	while(!feof(fd))
	{
		memset(nbuf, 0, 256);
		fgets(nbuf, 255, fd);

		s=nbuf;
		while(*s && (*s<=' '))s++;
		if(!*s)continue;

		i=4;
		while(i--)
		{
			if((*s>='0') && (*s<='9')) { s++; continue; }
			if((*s>='A') && (*s<='F')) { s++; continue; }
			if((*s>='a') && (*s<='f')) { s++; continue; }
			break;
		}

		if(!i)continue;
		if(*s!=':')continue;

		s++;
		i=8;
		while(i--)
		{
			if((*s>='0') && (*s<='9')) { s++; continue; }
			if((*s>='A') && (*s<='F')) { s++; continue; }
			if((*s>='a') && (*s<='f')) { s++; continue; }
			break;
		}

		if(!i)continue;
		if(!*s)continue;
		if(*s>' ')continue;

		while(*s && (*s<=' '))s++;
		if(!*s)continue;

		if((*s=='_') ||
			((*s>='a') && (*s<='z')) ||
			((*s>='A') && (*s<='Z')))

		{
			t=tb;
			while(*s)
			{
				if(*s=='_') { *t++=*s++; continue; }
				if((*s>='0') && (*s<='9'))
					{ *t++=*s++; continue; }
				if((*s>='a') && (*s<='z'))
					{ *t++=*s++; continue; }
				if((*s>='A') && (*s<='Z'))
					{ *t++=*s++; continue; }
				break;
			}
			*t++=0;

			if(!*s)continue;
			if(*s>' ')continue;
		}

		while(*s && (*s<=' '))s++;
		if(!*s)continue;

		if(	((*s>='0') && (*s<='9')) ||
			((*s>='a') && (*s<='f')) ||
			((*s>='A') && (*s<='F')))

		{
			t=tb1;
			while(*s)
			{
				if((*s>='0') && (*s<='9'))
					{ *t++=*s++; continue; }
				if((*s>='a') && (*s<='f'))
					{ *t++=*s++; continue; }
				if((*s>='A') && (*s<='F'))
					{ *t++=*s++; continue; }
				break;
			}
			*t++=0;

			if(!*s)continue;
			if(*s>' ')continue;
		}


		s=tb1; l=0;
		while(*s)
		{
			i=0;
			if((*s>='0') && (*s<='9'))i=*s-'0';
			if((*s>='a') && (*s<='f'))i=*s-'a'+10;
			if((*s>='A') && (*s<='F'))i=*s-'A'+10;

			s++;
			l=l*16+i;
		}

//		printf("%s %lX\n", tb, l);

		BLNK_AddSym(tb, (void *)(l));
	}

	fclose(fd);
	return(0);
}


int BLNK_ProcessEXE(char *name)
{
	static int init=0;

	s64 secva[8];
	int secvs[8];
	char nbuf[256];

	void *p, *q;
	byte *buf;
	FILE *fd;
	char *s;
	int sz, peoffs, coff_hdr, optoffs, symoffs, nsyms;
	int secoffs, nsecs, stroffs, mach;
	s64 disp, addr;
//	int syo;
	int i, j, k, l;

	BLNK_InitLink();

	if(init)return(-1);

	return(-1);	//strategy no longer used

	printf("BLNK_ProcessEXE '%s'\n", name);

//	printf("open %s\n", name);
	fd=fopen(name, "rb");

	if(!fd)
	{
		sprintf(nbuf, "%s.exe", name);
		fd=fopen(nbuf, "rb");
	}

	if(!fd)
	{
		printf("BLNK_ProcessEXE: fail open\n");
		return(-1);
	}

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=(byte *)malloc(sz);
	fread(buf, 1, sz, fd);
	fclose(fd);

	printf("EXE %d (0x%X) bytes\n", sz, sz);

	if((buf[0]!='M') || (buf[1]!='Z'))
	{
		printf("BLNK_ProcessEXE: EXE Sig Fail\n");
		return(-1);
	}

	peoffs=buf[0x3C]|(buf[0x3D]<<8)|(buf[0x3E]<<16)|(buf[0x3F]<<24);

	if((buf[peoffs+0]!='P') || (buf[peoffs+1]!='E'))
	{
		printf("BLNK_ProcessEXE: PE Sig Fail\n");
		return(-1);
	}

//	printf("PE Sig At 0x%X\n", peoffs);
	coff_hdr=peoffs+4;

	i=buf[coff_hdr+0]|(buf[coff_hdr+1]<<8);
	mach=i;
//	printf("\tMachine 0x%04X\n", i);

	symoffs=buf[coff_hdr+8]|(buf[coff_hdr+9]<<8)|
		(buf[coff_hdr+10]<<16)|(buf[coff_hdr+11]<<24);
	nsyms=buf[coff_hdr+12]|(buf[coff_hdr+13]<<8)|
		(buf[coff_hdr+14]<<16)|(buf[coff_hdr+15]<<24);

//	printf("\tSyms 0x%X %d\n", symoffs, nsyms);

	stroffs=symoffs+(nsyms*18);

	i=buf[coff_hdr+16]|(buf[coff_hdr+17]<<8);
	optoffs=coff_hdr+20;
	secoffs=coff_hdr+20+i;

	nsecs=buf[coff_hdr+2]|(buf[coff_hdr+3]<<8);

	if(mach==0x8664)
	{
		disp=	( (s64)buf[coff_hdr+20+24])|
			(((s64)buf[coff_hdr+20+25])<<8)|
			(((s64)buf[coff_hdr+20+26])<<16)|
			(((s64)buf[coff_hdr+20+27])<<24)|
			(((s64)buf[coff_hdr+20+28])<<32)|
			(((s64)buf[coff_hdr+20+29])<<40)|
			(((s64)buf[coff_hdr+20+30])<<48)|
			(((s64)buf[coff_hdr+20+31])<<56);
	}else
	{
		disp=buf[coff_hdr+20+28]|(buf[coff_hdr+20+29]<<8)|
			(buf[coff_hdr+20+30]<<16)|(buf[coff_hdr+20+31]<<24);
	}

	printf("\tDisplacement 0x%08X%08X\n", (u32)(disp>>32), (u32)disp);

//	printf("\tSecs 0x%X %d\n", secoffs, nsecs);

//	printf("\nSections:\n");
	for(i=0; i<nsecs; i++)
	{
		j=secoffs+(i*40);

		s=(char *)(buf+j);
		l=buf[j+8]|(buf[j+9]<<8)|(buf[j+10]<<16)|(buf[j+11]<<24);
		k=buf[j+12]|(buf[j+13]<<8)|(buf[j+14]<<16)|(buf[j+15]<<24);

		secva[i]=k+disp;
		secvs[i]=l;

		printf("\t%d %s 0x%08X%08X %d\n", i+1, s,
			(u32)(secva[i]>>32), (u32)secva[i], l);

		BLNK_AddExeSection(s, secva[i], secvs[i], 0);
	}

//	i=4096;
//	while(i<nsyms)i=i+(i>>1);
//	blnk_sym_name=malloc(i*sizeof(char *));
//	blnk_sym_addr=malloc(i*sizeof(void *));
//	blnk_sym_nsym=0;
//	blnk_sym_msym=i;

//	syo=blnk_sym_nsym;

//	printf("\nSymbols:\n");
	for(i=0; i<nsyms; i++)
	{
		j=symoffs+(i*18);

		if(buf[j])
		{
			memset(nbuf, 0, 16);
			memcpy(nbuf, buf+j, 8);
			s=nbuf;
		}else
		{
			k=buf[j+4]|(buf[j+5]<<8)|(buf[j+6]<<16)|(buf[j+7]<<24);
			s=(char *)(buf+stroffs+k);
		}

		k=buf[j+8]|(buf[j+9]<<8)|(buf[j+10]<<16)|(buf[j+11]<<24);
		l=buf[j+12]|(buf[j+13]<<8);

		if((l>0) && (l<=nsecs))addr=secva[l-1]+k;

//		if((*s=='.') || ((s[0]=='_') && (s[1]=='_')))
//		if((*s=='.') || ((s[0]=='_') && (s[1]=='_')) || (*s!='_'))
		if(*s!='_')
		{
			i+=buf[j+17];
			continue;
		}

//		printf("\t%s 0x%08X %d\n", s, k, l);

		BLNK_AddSym(s+1, (void *)addr);

		i+=buf[j+17];
	}


	if(!nsyms)
	{
		printf("BLNK_ProcessEXE: Warning: "
			"EXE has stripped symbols \n");

		strcpy(nbuf, name);
		s=nbuf+strlen(nbuf);
		while((s>nbuf) && (*s!='.'))s--;

		if(!strcmp(s, ".exe") || !strcmp(s, ".EXE"))
		{
			strcpy(s, ".map");
		}else
		{
			strcat(nbuf, ".map");
		}

		BLNK_ProcessMAP(nbuf);
	}

	p=BLNK_FetchSym("BLNK_ProcessEXE");
	q=(void *)(&BLNK_ProcessEXE);
	if(p!=q)
	{
		printf("BLNK_ProcessEXE: Alignment Failed %p->%p\n", p, q);
//		blnk_sym_nsym=0;
//		return(-1);
	}

	init=1;
	return(0);
}

#endif
