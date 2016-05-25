/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <bgbasm.h>

#ifdef WIN32
#include <windows.h>
#include <winternl.h>
#endif


#if 0
/* kept here for reference, from Wine under the LGPL */
typedef struct _TEB
{                                                               /* win32/win64 */
NT_TIB                       Tib;                               /* 000/0000 */
PVOID                        EnvironmentPointer;                /* 01c/0038 */
CLIENT_ID                    ClientId;                          /* 020/0040 */
PVOID                        ActiveRpcHandle;                   /* 028/0050 */
PVOID                        ThreadLocalStoragePointer;         /* 02c/0058 */
PPEB                         Peb;                               /* 030/0060 */
ULONG                        LastErrorValue;                    /* 034/0068 */
ULONG                        CountOfOwnedCriticalSections;      /* 038/006c */
PVOID                        CsrClientThread;                   /* 03c/0070 */
PVOID                        Win32ThreadInfo;                   /* 040/0078 */
ULONG                        Win32ClientInfo[31];               /* 044/0080 used for user32 private data in Wine */
PVOID                        WOW32Reserved;                     /* 0c0/0100 */
ULONG                        CurrentLocale;                     /* 0c4/0108 */
ULONG                        FpSoftwareStatusRegister;          /* 0c8/010c */
PVOID                        SystemReserved1[54];               /* 0cc/0110 used for kernel32 private data in Wine */
LONG                         ExceptionCode;                     /* 1a4/02c0 */
ACTIVATION_CONTEXT_STACK     ActivationContextStack;            /* 1a8/02c8 */
BYTE                         SpareBytes1[24];                   /* 1bc/02e8 used for ntdll private data in Wine */
PVOID                        SystemReserved2[10];               /* 1d4/0300 used for ntdll private data in Wine */
GDI_TEB_BATCH                GdiTebBatch;                       /* 1fc/0350 used for vm86 private data in Wine */
HANDLE                       gdiRgn;                            /* 6dc/0838 */
HANDLE                       gdiPen;                            /* 6e0/0840 */
HANDLE                       gdiBrush;                          /* 6e4/0848 */
CLIENT_ID                    RealClientId;                      /* 6e8/0850 */
HANDLE                       GdiCachedProcessHandle;            /* 6f0/0860 */
ULONG                        GdiClientPID;                      /* 6f4/0868 */
ULONG                        GdiClientTID;                      /* 6f8/086c */
PVOID                        GdiThreadLocaleInfo;               /* 6fc/0870 */
ULONG                        UserReserved[5];                   /* 700/0878 */
PVOID                        glDispachTable[280];               /* 714/0890 */
PVOID                        glReserved1[26];                   /* b74/1150 */
PVOID                        glReserved2;                       /* bdc/1220 */
PVOID                        glSectionInfo;                     /* be0/1228 */
PVOID                        glSection;                         /* be4/1230 */
PVOID                        glTable;                           /* be8/1238 */
PVOID                        glCurrentRC;                       /* bec/1240 */
PVOID                        glContext;                         /* bf0/1248 */
ULONG                        LastStatusValue;                   /* bf4/1250 */
UNICODE_STRING               StaticUnicodeString;               /* bf8/1258 used by advapi32 */
WCHAR                        StaticUnicodeBuffer[261];          /* c00/1268 used by advapi32 */
PVOID                        DeallocationStack;                 /* e0c/1478 */
PVOID                        TlsSlots[64];                      /* e10/1480 */
LIST_ENTRY                   TlsLinks;                          /* f10/1680 */
PVOID                        Vdm;                               /* f18/1690 */
PVOID                        ReservedForNtRpc;                  /* f1c/1698 */
PVOID                        DbgSsReserved[2];                  /* f20/16a0 */
ULONG                        HardErrorDisabled;                 /* f28/16b0 */
PVOID                        Instrumentation[16];               /* f2c/16b8 */
PVOID                        WinSockData;                       /* f6c/1738 */
ULONG                        GdiBatchCount;                     /* f70/1740 */
ULONG                        Spare2;                            /* f74/1744 */
PVOID                        Spare3;                            /* f78/1748 */
PVOID                        Spare4;                            /* f7c/1750 */
PVOID                        ReservedForOle;                    /* f80/1758 */
ULONG                        WaitingOnLoaderLock;               /* f84/1760 */
PVOID                        Reserved5[3];                      /* f88/1768 */
PVOID                       *TlsExpansionSlots;                 /* f94/1780 */
ULONG                        ImpersonationLocale;               /* f98/1788 */
ULONG                        IsImpersonating;                   /* f9c/178c */
PVOID                        NlsCache;                          /* fa0/1790 */
PVOID                        ShimData;                          /* fa4/1798 */
ULONG                        HeapVirtualAffinity;               /* fa8/17a0 */
PVOID                        CurrentTransactionHandle;          /* fac/17a8 */
PVOID                        ActiveFrame;                       /* fb0/17b0 */
#ifdef _WIN64
PVOID                        unknown[2];                        /*     17b8 */
#endif
PVOID                       *FlsSlots;                          /* fb4/17c8 */
} TEB, *PTEB;
#endif

static int basm_decnibble(char i)
{
	if((i>='0') && (i<='9'))return(i-'0');
	if((i>='A') && (i<='F'))return(i-'A'+10);
	if((i>='a') && (i<='f'))return(i-'a'+10);
	return(0);
}

BASM_API int BASM_UnmangleXCall(char *str, char *buf)
{
	char *s, *t;
	int i;

	s=str; t=buf;

	while(*s)
	{
		if((s[0]=='_') && (s[1]=='1'))
			{ s+=2; *t++='_'; continue; }
		if((s[0]=='_') && (s[1]=='2'))
			{ s+=2; *t++=';'; continue; }
		if((s[0]=='_') && (s[1]=='3'))
			{ s+=2; *t++='['; continue; }
		if((s[0]=='_') && (s[1]=='4'))
			{ s+=2; *t++='('; continue; }
		if((s[0]=='_') && (s[1]=='5'))
			{ s+=2; *t++=')'; continue; }
		if((s[0]=='_') && (s[1]=='6'))
			{ s+=2; *t++='/'; continue; }

		if((s[0]=='_') && (s[1]>='a') && (s[1]<='z'))
		{
			i=-1;
			switch(s[1])
			{
			case 'a': i='+'; break;  case 'b': i=']'; break;
			case 'c': i=','; break;  case 'd': i='.'; break;
			case 'e': i='='; break;  case 'f': i='\\'; break;
			case 'g': i='>'; break;  case 'h': i='#'; break;
			case 'i': i='\''; break; case 'j': i='-'; break;
			case 'k': i=':'; break;  case 'l': i='<'; break;
			case 'm': i='*'; break;  case 'n': i='!'; break;
			case 'o': i='|'; break;  case 'p': i='?'; break;
			case 'q': i='\"'; break; case 'r': i='%'; break;
			case 's': i='$'; break;  case 't': i='~'; break;
			case 'u': i='{'; break;  case 'v': i='}'; break;
			case 'w': i='`'; break;  case 'x': i='^'; break;
			case 'y': i='&'; break;  case 'z': i='@'; break;
			}
			*t++=i; s+=2;
			continue;
		}

		if((s[0]=='_') && (s[1]=='9'))
		{
			i=(basm_decnibble(s[2])<<4)|basm_decnibble(s[3]);
			s+=4;

			if((!i) || (i>=128))
			{
				*t++=0xC0|((i>>6)&31);
				*t++=0x80|(i&63);
				continue;
			}
			*t++=i;
			continue;
		}

		if((s[0]=='_') && (s[1]=='0'))
		{
			i=	(basm_decnibble(s[2])<<12)|
				(basm_decnibble(s[3])<<8)|
				(basm_decnibble(s[4])<<4)|
				(basm_decnibble(s[5])<<0);
			s+=6;

			if(i>=2048)
			{
				*t++=0xE0|((i>>12)&15);
				*t++=0x80|((i>>6)&63);
				*t++=0x80|(i&63);
				continue;
			}
			if((!i) || (i>=128))
			{
				*t++=0xC0|((i>>6)&31);
				*t++=0x80|(i&63);
				continue;
			}
			*t++=i;
			continue;
		}

		*t++=*s++;
	}
	*t++=0;

	return(0);
}

BASM_API char **BASM_UnmangleSplit(char *str)
{
	char buf[256];
	char **abuf;
	char *s, *t;
	int n;

	abuf=malloc(64*sizeof(char *));

	s=str; n=0;
	while(*s)
	{
		t=buf;
		while(*s)
		{
			if((s[0]=='_') && (s[1]=='_'))
				{ s+=2; break; }
			*t++=*s++;
		}
		*t++=0;

		BASM_UnmangleXCall(buf, buf);
		abuf[n++]=strdup(buf);
	}
	abuf[n]=NULL;

	return(abuf);
}

BASM_API int BASM_MangleXCall(char *str, char *buf)
{
//	char tb[256];
	char *s, *t;
	int i;

//	s=str; t=tb;
	s=str; t=buf;
	while(*s)
	{
		if(*s=='_') { s++; *t++='_'; *t++='1'; continue; }
		if(*s==';') { s++; *t++='_'; *t++='2'; continue; }
		if(*s=='[') { s++; *t++='_'; *t++='3'; continue; }
		if(*s=='(') { s++; *t++='_'; *t++='4'; continue; }
		if(*s==')') { s++; *t++='_'; *t++='5'; continue; }
		if(*s=='/') { s++; *t++='_'; *t++='6'; continue; }

		i=-1;
		switch(*s)
		{
		case '+': i='a'; break;		case ']': i='b'; break;
		case ',': i='c'; break;		case '.': i='d'; break;
		case '=': i='e'; break;		case '\\': i='f'; break;
		case '>': i='g'; break;		case '#': i='h'; break;
		case '\'': i='i'; break;	case '-': i='j'; break;
		case ':': i='k'; break;		case '<': i='l'; break;
		case '*': i='m'; break;		case '!': i='n'; break;
		case '|': i='o'; break;		case '?': i='p'; break;
		case '\"': i='q'; break;	case '%': i='r'; break;
		case '$': i='s'; break;		case '~': i='t'; break;
		case '{': i='u'; break;		case '}': i='v'; break;
		case '`': i='w'; break;		case '^': i='x'; break;
		case '&': i='y'; break;		case '@': i='z'; break;
		}

		if(i>=0) { s++; *t++='_'; *t++=i; continue; }

		if((*s>='0') && (*s<='9')) { *t++=*s++; continue; }
		if((*s>='A') && (*s<='Z')) { *t++=*s++; continue; }
		if((*s>='a') && (*s<='z')) { *t++=*s++; continue; }

		i=(*s++)&0xFF;

		if((i>0) && (i<=127))
		{
			sprintf(t, "_9%02X", i);
			t+=4; continue;
		}

		if(i>=0xE0)
			{ i=(i&15)<<12; i|=((*s++)&63)<<6; i|=(*s++)&63; }
		else if(i>=0xC0)
			{ i=(i&31)<<6; i|=(*s++)&63; }

		if((i>=0) && (i<=255))
			{ sprintf(t, "_9%02X", i); t+=4; }
			else { sprintf(t, "_0%04X", i); t+=6; }
	}
	*t++=0;

//	strcpy(buf, tb);
	return(0);
}


void *BASM_GetLabelXCall(char *name)
{
	char tb[256], tbn[64], tbs[256];
	u64 uli;
	byte *tx;
	void *p;
	char *s, *t, *esig;
	int ni, nx, ns;
	int i, j;

	printf("BASM_GetLabelXCall: '%s'\n", name);

	//Firstly: If the call is vararg check if there is a normal function
	//with the same prefix and also the same calling convention
	if(!strncmp(name, "_XV_", 4))
	{
		//compose '_XC_<name>', but leave off most of the signature
		s=name+4; t=tb;
		*t++='_'; *t++='X'; *t++='C'; *t++='_';
		while(*s && !((s[0]=='_') && (s[1]=='4')))
			*t++=*s++;
		*t++='_'; *t++='4';	//so that '(' is matched as well
		*t++=0;

		//OK, lookup name by prefix
		p=BASM_FetchSymPrefix(tb);
		if(p)
		{
			//OK, we have a function, now make a trampoline

			tx=BLNK_AllocTextSz(16);

#ifdef X86_64
			uli=(unsigned long long)p;
			tx[0]=0x48;	//REX.W
			tx[1]=0xB8;	//mov rax, imm64
			tx[2]=(uli>> 0)&0xFF;
			tx[3]=(uli>> 8)&0xFF;
			tx[4]=(uli>>16)&0xFF;
			tx[5]=(uli>>24)&0xFF;
			tx[6]=(uli>>32)&0xFF;
			tx[7]=(uli>>40)&0xFF;
			tx[8]=(uli>>48)&0xFF;
			tx[9]=(uli>>56)&0xFF;

			tx[10]=0x48;	//REX.W
			tx[11]=0xFF;	//jmp FF/4
			tx[12]=0xE0;	//rax
#else
			uli=(int)p;
			tx[0]=0xB8;	//mov rax, imm64
			tx[1]=(uli>> 0)&0xFF;
			tx[2]=(uli>> 8)&0xFF;
			tx[3]=(uli>>16)&0xFF;
			tx[4]=(uli>>24)&0xFF;
			tx[5]=0xFF;	//jmp FF/4
			tx[6]=0xE0;	//rax
#endif

			//So that next time this symbol is located directly:
			BASM_RegisterPtr(name, tx);

			return((void *)tx);
		}
	}

	//So, this was not just a simple varargs case
	//We may then need to generate a proper thunk

	p=NULL;

#ifdef linux
	p=BASM_XCallGenThunkSysV(name);
#endif

#ifdef WIN64
	p=BASM_XCallGenThunkW64(name);
#endif

#ifdef X86
	//x86 XCall is, more or less, cdecl
	BASM_BeginAssembly(NULL);
	basm_print("[section .text]\n[bits 32]\n");
	basm_print("%s:\n", name);
	basm_print("jmp %s\n", tbn);
	p=BASM_EndAssemblyQuiet();
#endif

	printf("BASM_GetLabelXCall: %p\n", p);

	return(p);
}

void *BASM_GetLabelNativeXCall(char *name)
{
	char tb[256];
	void *p;
	char *s, *t;

	s=name; t=tb;
	*t++='_'; *t++='X'; *t++='C'; *t++='_';
	while(*s)
	{
		if(*s=='_') { s++; *t++='_'; *t++='1'; continue; }
		*t++=*s++;
	}
	*t++='_'; *t++='4';	//so that '(' is matched as well
	*t++=0;

	printf("BASM_GetLabelNativeXCall: %s (pf='%s')\n", name, tb);

	//OK, lookup name by prefix
	s=BASM_FetchSymPrefixName(tb);
	if(!s)return(NULL);

	printf("BASM_GetLabelNativeXCall: -> %s\n", s);

	p=NULL;

#ifdef linux
//	p=BASM_XCallGenNatThunkSysV(name, s);
#endif

#ifdef WIN64
	p=BASM_XCallGenNatThunkW64(name, s);
#endif

#ifdef X86
	//x86 XCall is, more or less, cdecl
	BASM_BeginAssembly(NULL);
	basm_print("[section .text]\n[bits 32]\n");
	basm_print("%s:\n", name);
	basm_print("jmp %s\n", s);
//	p=BASM_EndAssemblyQuiet();
	p=BASM_EndAssemblyDebug();
#endif

	printf("BASM_GetLabelNativeXCall: %p\n", p);

	return(p);
}

void *BASM_GetLabelXTls(char *name)
{
	char tb[256];
	char *s, *t, *vn, *reg;
	void *p;
	int sz, i, tls, tls2;

	s=name+4; t=tb;
	while(*s && !((s[0]=='_') && (s[1]=='_')))*t++=*s++;
	*t++=0;

	BASM_UnmangleXCall(tb, tb);
	vn=basm_strdup(tb);

	if((s[0]=='_') && (s[1]=='_'))s+=2;
	t=tb; while(*s && (*s!='_')) { *t++=*s++; } *t++=0;

	sz=atoi(tb);

	if(*s=='_')s++;
	t=tb; while(*s && (*s!='_')) { *t++=*s++; } *t++=0;
	reg=basm_strdup(tb);

	tls=BLNK_gcAllocTls(vn, sz);

#ifdef WIN32
#ifdef X86
	tls2=BLNK_gcGetTlsTls();

	if(tls2<64)
	{
#if 0
		BASM_BeginAssembly(NULL);
		basm_print("%s:\n", name);
		basm_print("mov %s, [fs:%d]\n", reg, 0xE10+tls2*4);
		basm_print("lea %s, [%s+%d]\n", reg, reg, tls*4);
		basm_print("ret\n");
		p=BASM_EndAssemblyQuiet();
		return(p);
#endif
		BASM_BeginAssembly(NULL);
		basm_print("%s:\n", name);
		basm_print("mov %s, [fs:%d]\n",
			reg, FIELD_OFFSET(NT_TIB, Self));
		basm_print("mov %s, [%s+%d]\n",
			reg, reg, FIELD_OFFSET(TEB, TlsSlots)+tls2*4);
		basm_print("lea %s, [%s+%d]\n", reg, reg, tls*4);
		basm_print("ret\n");
		p=BASM_EndAssemblyQuiet();
		return(p);

	}

	if(tls2>=64)
	{
#if 0
		BASM_BeginAssembly(NULL);
		basm_print("%s:\n", name);
//		basm_print("mov %s, [fs:0x2C]\n", reg);
		basm_print("mov %s, [fs:0xF94]\n", reg);
		basm_print("mov %s, [%s+%d]\n", reg, reg, (tls2-64)*4);
		basm_print("lea %s, [%s+%d]\n", reg, reg, tls*4);
		basm_print("ret\n");
		p=BASM_EndAssemblyQuiet();
		return(p);
#endif
		BASM_BeginAssembly(NULL);
		basm_print("%s:\n", name);
		basm_print("mov %s, [fs:%d]\n",
			reg, FIELD_OFFSET(NT_TIB, Self));
		basm_print("mov %s, [%s+%d]\n",
			reg, reg, FIELD_OFFSET(TEB, TlsExpansionSlots));
		basm_print("mov %s, [%s+%d]\n", reg, reg, (tls2-64)*4);
		basm_print("lea %s, [%s+%d]\n", reg, reg, tls*4);
		basm_print("ret\n");
		p=BASM_EndAssemblyQuiet();
		return(p);

	}
#endif
#endif

#ifdef WIN64

#ifdef X86_64
	tls2=BLNK_gcGetTlsTls();

	if(tls2<64)
	{
		BASM_BeginAssembly(NULL);
		basm_print("%s:\n", name);
		basm_print("mov %s, [gs:%d]\n",
			reg, FIELD_OFFSET(NT_TIB, Self));
		basm_print("mov %s, [%s+%d]\n",
			reg, reg, FIELD_OFFSET(TEB, TlsSlots)+tls2*8);
		basm_print("lea %s, [%s+%d]\n", reg, reg, tls*8);
		basm_print("ret\n");
		p=BASM_EndAssemblyQuiet();
		return(p);
	}

	if(tls2>=64)
	{
		BASM_BeginAssembly(NULL);
		basm_print("%s:\n", name);
		basm_print("mov %s, [gs:%d]\n",
			reg, FIELD_OFFSET(NT_TIB, Self));
		basm_print("mov %s, [%s+%d]\n",
			reg, reg, FIELD_OFFSET(TEB, TlsExpansionSlots));
		basm_print("mov %s, [%s+%d]\n", reg, reg, (tls2-64)*8);
		basm_print("lea %s, [%s+%d]\n", reg, reg, tls*8);
		basm_print("ret\n");
		p=BASM_EndAssemblyQuiet();
		return(p);
	}
#endif

#if 0
//#ifdef X86_64
	tls2=BLNK_gcGetTlsTls();

	BASM_BeginAssembly(NULL);
	basm_print("%s:\n", name);
	basm_print("push rbp\n");
	basm_print("mov rbp, rsp\n");

	basm_print("push rax\n");	//return reg
	basm_print("push rax\n");
	basm_print("push rcx\n");
	basm_print("push rdx\n");
	basm_print("push r8\n");
	basm_print("push r9\n");
	basm_print("push r10\n");
	basm_print("push r11\n");

	basm_print("mov rcx, %d\n", reg, tls2);
	basm_print("call TlsGetValue\n");
	basm_print("lea %s, [rax+%d]\n", reg, tls*8);
	basm_print("mov [rbp-8], rax\n");

	basm_print("pop r11\n");
	basm_print("pop r10\n");
	basm_print("pop r9\n");
	basm_print("pop r8\n");
	basm_print("pop rdx\n");
	basm_print("pop rcx\n");
	basm_print("pop rax\n");

	basm_print("pop %s\n", reg);

	basm_print("pop rbp\n");
	basm_print("ret\n");
	p=BASM_EndAssemblyQuiet();
	return(p);
#endif
#endif

#ifdef __i386__
	BASM_BeginAssembly(NULL);
	basm_print("%s:\n", name);
	basm_print("push ebp; mov ebp, esp\n");
	basm_print("push eax\n");
	basm_print("pushad\n");
	basm_print("push dword [ebp+8]\n");
	basm_print("call thGetTlsPtr\n");
	basm_print("mov [ebp-4], eax; pop edx\n");
	basm_print("popad\n");
	basm_print("pop %s\n", reg);
	basm_print("pop ebp\n");
	basm_print("ret\n");
	p=BASM_EndAssemblyQuiet();
	return(p);
#endif

	return(NULL);
}
