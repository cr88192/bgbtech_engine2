/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>

#include <bgbasm.h>

char basm_cpuid_str[16];
int basm_cpuid_std[4];

extern char *bs1_ops_strs[];
extern char *bs1_ops_args[];

BASM_THREAD BASM_Context *basm_asm;
// int basm_seq=1;

BASM_THREAD char *basm_asmbuf, *basm_asmbuf_end;
BASM_THREAD char *basm_asmend;

BASM_API char *BASM_CPUID_String()
	{ return(basm_cpuid_str); }
BASM_API u32 BASM_CPUID_Version()
	{ return(basm_cpuid_std[0]); }
BASM_API u32 BASM_CPUID_Unit()
	{ return(basm_cpuid_std[1]); }
BASM_API u32 BASM_CPUID_Ext()
	{ return(basm_cpuid_std[2]); }
BASM_API u32 BASM_CPUID_Feature()
	{ return(basm_cpuid_std[3]); }

BASM_API char *BASM_CPUID_FeatureStr()
{
	static char tb[4096];
	char *t;
	u32 i;
	
	t=tb;
	
	i=BASM_CPUID_Feature();
	if(i&BASM_CPUID_FPU  ) { strcpy(t, "FPU "); t+=strlen(t); }
	if(i&BASM_CPUID_VME  ) { strcpy(t, "VME "); t+=strlen(t); }
	if(i&BASM_CPUID_DE   ) { strcpy(t, "DE "); t+=strlen(t); }
	if(i&BASM_CPUID_PSE  ) { strcpy(t, "PSE "); t+=strlen(t); }
	if(i&BASM_CPUID_TSC  ) { strcpy(t, "TSC "); t+=strlen(t); }
	if(i&BASM_CPUID_MSR  ) { strcpy(t, "MSR "); t+=strlen(t); }
	if(i&BASM_CPUID_PAE  ) { strcpy(t, "PAE "); t+=strlen(t); }
	if(i&BASM_CPUID_MCE  ) { strcpy(t, "MCE "); t+=strlen(t); }
	if(i&BASM_CPUID_CXB  ) { strcpy(t, "CXB "); t+=strlen(t); }
	if(i&BASM_CPUID_APIC ) { strcpy(t, "APIC "); t+=strlen(t); }
	if(i&BASM_CPUID_SEP  ) { strcpy(t, "SEP "); t+=strlen(t); }
	if(i&BASM_CPUID_MTRR ) { strcpy(t, "MTRR "); t+=strlen(t); }
	if(i&BASM_CPUID_PGE  ) { strcpy(t, "PGE "); t+=strlen(t); }
	if(i&BASM_CPUID_MCA  ) { strcpy(t, "MCA "); t+=strlen(t); }
	if(i&BASM_CPUID_CMOV ) { strcpy(t, "CMOV "); t+=strlen(t); }
	if(i&BASM_CPUID_PAT  ) { strcpy(t, "PAT "); t+=strlen(t); }
	if(i&BASM_CPUID_PSE36) { strcpy(t, "PSE36 "); t+=strlen(t); }
	if(i&BASM_CPUID_PSN  ) { strcpy(t, "PSN "); t+=strlen(t); }
	if(i&BASM_CPUID_CFLSH) { strcpy(t, "CFLSH "); t+=strlen(t); }
	if(i&BASM_CPUID_DS   ) { strcpy(t, "DS "); t+=strlen(t); }
	if(i&BASM_CPUID_ACPI ) { strcpy(t, "ACPI "); t+=strlen(t); }
	if(i&BASM_CPUID_MMX  ) { strcpy(t, "MMX "); t+=strlen(t); }
	if(i&BASM_CPUID_FXSR ) { strcpy(t, "FXSR "); t+=strlen(t); }
	if(i&BASM_CPUID_SSE  ) { strcpy(t, "SSE "); t+=strlen(t); }
	if(i&BASM_CPUID_SSE2 ) { strcpy(t, "SSE2 "); t+=strlen(t); }
	if(i&BASM_CPUID_SS   ) { strcpy(t, "SS "); t+=strlen(t); }
	if(i&BASM_CPUID_HTT  ) { strcpy(t, "HTT "); t+=strlen(t); }
	if(i&BASM_CPUID_TM   ) { strcpy(t, "TM "); t+=strlen(t); }
	if(i&BASM_CPUID_PBE  ) { strcpy(t, "PBE "); t+=strlen(t); }
	
	*t++=0;
	
	return(tb);
}

BASM_API void BASM_AddNameBlacklist(char *name)
{
	blnk_blacklist(name);
}

BASM_API void BASM_AddNamesBlacklist(char *nameslst)
{
	char tb[256];
	char *s, *t;
	
	s=nameslst; t=tb;
	while(*s)
	{
		if(*s==';')
		{
			*t++=0;
			blnk_blacklist(tb);
			
			s++;
			while(*s && (*s<=' '))s++;
			
			t=tb;
			continue;
		}
		*t++=*s++;
	}

	*t++=0;
	if(tb[0])
		{ blnk_blacklist(tb); }
}

BASM_API void *BASM_GetEBP()
{
	static void *p=NULL;

	BASM_Init();

	if(!p)p=BASM_GetPtr("basm_getbp");
	if(p)return(((void *(*)())p)());
	return(NULL);
}

BASM_API int BASM_GetReturnState(void **regs)
{
	static void *getbp=NULL;
	void *p;

	BASM_Init();

#ifdef X86
	if(!getbp)getbp=BASM_GetPtr("basm_getbp");
	if(getbp)p=((void *(*)())getbp)();
		else p=NULL;

	if(!p)return(-1);

	regs[0]=((void **)p)[1];	//eip
	regs[1]=((void **)p)[0];	//ebp
	regs[2]=((byte *)p)+8;		//esp

	return(1);
#endif

	return(-1);
}

#ifdef ARM
BASM_API int BASM_StackTrace(void **ip, void **bp, int lim)
{
	return(-1);
}
#endif

#ifdef X86
BASM_API int BASM_StackTrace(void **ip, void **bp, int lim)
{
	static void *getbp=NULL;
	void *p, *q, *r, *ibp;
	char *s;
	int i, n;

	BASM_Init();

	if(!getbp)getbp=BASM_GetPtr("basm_getbp");
	if(getbp)p=((void *(*)())getbp)();
		else p=NULL;

	if(!p)return(-1);

	i=((byte *)p)-((byte *)(&p));

	if((i<=0) || (i>256))	//this isn't right...
		return(-1);

	ibp=p; n=0;
	while(1)
	{
		if(n>=lim)break;
		if((p<ibp) || (p>((void *)(((byte *)ibp)+65536))))
			break;

		q=((void **)p)[1];
		p=((void **)p)[0];

		i=n++; ip[i]=q; bp[i]=p;
	}

	return(n);
}
#endif

#ifdef X86_64
#ifdef WIN64

int BASM_Win64CheckEpilogueRet(byte *ip)
{
	byte *cs;

	cs=ip;

	//eat: pop r64
	while(((cs[0]&0xFB)==0x48) &&
		((cs[1]&0xF8)==0x58))
			cs+=2;
	//ret or jmp
//	if((*cs==0xC3)||(*cs==0xEB)||(*cs==0xE9))
//		return(1);

	//ret only
	if(*cs==0xC3)
		return(1);

	return(0);
}

BASM_API byte *BASM_Win64LocateEpilogue(byte *ip)
{
	byte *cs;
	int i, j;

	i=BLNK_CheckPointerImageP(ip);
	if(i<=0)return(NULL);

	cs=ip;
	for(i=0; i<4096; i++)
	{
		//add rsp, i8
		if((cs[0]==0x48) && (cs[1]==0x83) && (cs[2]==0xC4))
		{
			j=BASM_Win64CheckEpilogueRet(cs+4);
			if(j>0)break;
		}

		//add rsp, i32
		if((cs[0]==0x48) && (cs[1]==0x81) && (cs[2]==0xC4))
		{
			j=BASM_Win64CheckEpilogueRet(cs+7);
			if(j>0)break;
		}

		j=cs[2]&0xF8;

		//lea rsp, [reg]
		if(((cs[0]&0xFE)==0x48) && (cs[1]==0x8D) && (j==0x20))
		{
			j=BASM_Win64CheckEpilogueRet(cs+3);
			if(j>0)break;
		}

		//lea rsp, [reg+disp8]
		if(((cs[0]&0xFE)==0x48) && (cs[1]==0x8D) && (j==0x60))
		{
			j=BASM_Win64CheckEpilogueRet(cs+4);
			if(j>0)break;
		}

		//lea rsp, [reg+disp32]
		if(((cs[0]&0xFE)==0x48) && (cs[1]==0x8D) && (j==0xA0))
		{
			j=BASM_Win64CheckEpilogueRet(cs+7);
			if(j>0)break;
		}

		cs++;
	}

	if(i<4096)return(cs);
	return(NULL);
}

BASM_API byte *BASM_Win64AdjustEpilogueRSP(byte *ip, byte *sp,
	BGBASM_RegsBuf *regs)
{
	byte *cip, *csp;
	int i, j;

	cip=ip; csp=sp;

	//add rsp, imm8
	if((cip[0]==0x48) && (cip[1]==0x83) && (cip[2]==0xC4))
		{ csp+=cip[3]; cip+=4; }

	//add rsp, imm32
	if((cip[0]==0x48) && (cip[1]==0x81) && (cip[2]==0xC4))
	{
		i=cip[3]+(cip[4]<<8)+(cip[5]<<16)+(cip[6]<<24);
		csp+=i; cip+=7;
	}

	//lea rsp, [reg]
	if(((cip[0]&0xFE)==0x48) && (cip[1]==0x8D) && ((cip[2]&0xF8)==0x20))
	{
		i=(cip[2]&7)|((cip[0]&1)<<3);
		csp=regs->gregs[i]; cip+=3;
	}

	//lea rsp, [reg+disp8]
	if(((cip[0]&0xFE)==0x48) && (cip[1]==0x8D) && ((cip[2]&0xF8)==0x60))
	{
		i=(cip[2]&7)|((cip[0]&1)<<3);
		csp=regs->gregs[i]; csp+=cip[3]; cip+=4;
	}

	//lea rsp, [reg+disp32]
	if(((cip[0]&0xFE)==0x48) && (cip[1]==0x8D) && ((cip[2]&0xF8)==0xA0))
	{
		i=(cip[2]&7)|((cip[0]&1)<<3);
		j=cip[3]+(cip[4]<<8)+(cip[5]<<16)+(cip[6]<<24);
		csp=regs->gregs[i]; csp+=j; cip+=7;
	}

	while(((cip[0]&0xFB)==0x48) &&
		((cip[1]&0xF8)==0x58))
	{
		i=(cip[1]&7)|((cip[0]&4)<<1);
		regs->gregs[i]=*(void **)csp;
		csp+=8; cip+=2;
	}

	return(csp);
}

BASM_API int BASM_StackTrace(void **ip, void **bp, int lim)
{
	BGBASM_RegsBuf regs;
	void *p, *q, *r;
	void *cip, *csp, *cbp;
	byte *cs, *cs1;
	char *s;
	int i, j, n;

	BASM_Init();

	i=BASM_SaveRegs(&regs);
	if(i<0)return(-1);

	cip=regs.spregs[0];
	csp=regs.spregs[1];
	n=0;
	while(1)
	{
		if(n>=lim)break;
//		if((p<ibp) || (p>((void *)(((byte *)ibp)+65536))))
//			break;

//		q=((void **)p)[1];
//		p=((void **)p)[0];

		cs=BASM_Win64LocateEpilogue(cip);
		if(!cs)break;

		cbp=BASM_Win64AdjustEpilogueRSP(cs, csp, &regs);
		if(!cbp)break;

		i=n++;
		ip[i]=cip;
		bp[i]=cbp;

		cip=*(byte **)cbp;
		csp=((byte *)cbp)+8;
	}

	return(n);
}
#else
BASM_API int BASM_StackTrace(void **ip, void **bp, int lim)
{
	return(-1);
}
#endif
#endif

BASM_API void basm_putc(int c)
{
	int i, sz;

	if((basm_asmend+256)>=basm_asmbuf_end)
	{
		i=basm_asmend-basm_asmbuf;
		sz=basm_asmbuf_end-basm_asmbuf;
		sz+=sz>>1;
		basm_asmbuf=realloc(basm_asmbuf, sz);
		basm_asmbuf_end=basm_asmbuf+sz;
		basm_asmend=basm_asmbuf+i;
	}

	*basm_asmend++=c;
}

BASM_API void basm_puts(char *str)
{
	int i, j, sz;

	if((basm_asmend+strlen(str)+256)>=basm_asmbuf_end)
	{
		i=basm_asmend-basm_asmbuf;
		sz=basm_asmbuf_end-basm_asmbuf;
		j=i+strlen(str)+256;
		while(sz<=j)sz+=sz>>1;
		basm_asmbuf=realloc(basm_asmbuf, sz);
		basm_asmbuf_end=basm_asmbuf+sz;
		basm_asmend=basm_asmbuf+i;
	}

	strcpy(basm_asmend, str);
	basm_asmend+=strlen(basm_asmend);
}

BASM_API void basm_vprint(char *str, va_list lst)
{
	static char *buf=NULL;
	static int bufsz;
	va_list lst1;
//	byte *ip, *dp;
	char *s, *s1, *t;
	int i;

	BASM_Init();

	s=str;
	while(*s && (*s!='%'))s++;

	if(!*s)
	{
		basm_puts(str);
		return;
	}

	if(!buf)
	{
		bufsz=4096;
		buf=malloc(bufsz);
	}

//	ip=basm_asm->ip;
//	dp=basm_asm->dp;

	vsnprintf(buf, bufsz, str, lst);

#if 0
	while(1)
	{
		va_copy(lst1, lst);
		i=vsnprintf(buf, bufsz, str, lst1);
		va_end(lst1);

		if(i<0)
		{
			printf("basm_print: error '%s'\n", str);
			return;
		}
		if((i+2)<=bufsz)break;

		while(bufsz<=(i+2))
			bufsz+=bufsz>>1;
		buf=realloc(buf, bufsz);
	}
#endif

	basm_puts(buf);
}

BASM_API void basm_print(char *str, ...)
{
	static char *buf=NULL;
	static int bufsz;
	va_list lst;
//	byte *ip, *dp;
	char *s, *s1, *t;
	int i;

	BASM_Init();

	s=str;
	while(*s && (*s!='%'))s++;

	if(!*s)
	{
		basm_puts(str);
		return;
	}

	if(!strncmp(s, "%s", 2))
	{
		t=s+2;
		while(*t && (*t!='%'))t++;

		if(!*t)
		{
			va_start(lst, str);
			s1=va_arg(lst, char *);
			va_end(lst);

			s=str;
			while(*s && (*s!='%'))
				basm_putc(*s++);
			basm_puts(s1);
			s+=2; basm_puts(s);

			return;
		}
	}

	if(!buf)
	{
		bufsz=4096;
		buf=malloc(bufsz);
	}

//	ip=basm_asm->ip;
//	dp=basm_asm->dp;

	while(1)
	{
		va_start(lst, str);
		i=vsnprintf(buf, bufsz, str, lst);
		va_end(lst);

		if(i<0)
		{
			printf("basm_print: error '%s'\n", str);
			return;
		}
		if((i+2)<=bufsz)break;

		while(bufsz<=(i+2))
			bufsz+=bufsz>>1;
		buf=realloc(buf, bufsz);
	}

	basm_puts(buf);

//	printf("%s", buf);
//	BASM_ParseOpcodeList(basm_asm, buf);

//	printf("Assembled %d bytes text, %d bytes data\n",
//		basm_asm->ip-ip, basm_asm->dp-dp);
//	BDISASM_PrintOpcodes(ip, 0, basm_asm->ip-ip);
}

BASM_API void BASM_RegisterPtr(char *lbl, void *ptr)
{
	BASM_Init();

//	printf("Register Ptr %s %p\n", lbl, ptr);

	BLNK_FlushQuickHash();

//	BASM_EmitLabelPtr(basm_asm, lbl, ptr);
	BLNK_AddSym(lbl, ptr);
}

BASM_API void *BASM_GetPtr(char *lbl)
{
	void *p;

	BASM_Init();
	p=BLNK_LookupLabel(lbl);
	return(p);
}

BASM_API char *BASM_GetPtrName(void *ptr)
{
	return(BLNK_GetNamePtr(ptr));
}

BASM_API char *BASM_GetLastNamePtr(void *ptr, void **rbp)
{
	return(BLNK_GetLastNamePtr(ptr, rbp));
}

BASM_API void *BASM_FetchSymPrefix(char *name)
{
	return(BLNK_FetchSymPrefix(name));
}

BASM_API char *BASM_FetchSymPrefixName(char *name)
{
	return(BLNK_FetchSymPrefixName(name));
}

BASM_API char *BASM_LookupExeSectionPtrName(void *ptr)
{
	return(BLNK_LookupExeSectionPtrName(ptr));
}

BASM_API char *BASM_LookupSectionPtrName(void *ptr)
{
	char *s;
	s=BLNK_LookupExeSectionPtrName(ptr);
	if(s)return(s);
	s=BLNK_LookupModulePtrName(ptr);
	if(s)return(s);
	
	return(NULL);
}

BASM_API void *BASM_CreateLabelBSS(char *lbl, int sz)
{
	void *p;

	BASM_Init();
	p=BLNK_LookupLabel(lbl);
	if(p)return(p);
	
	p=BLNK_AllocDataSz(sz);
	BLNK_AddSym(lbl, p);

	return(p);
}


BASM_API void BASM_DumpModule(char *name)
{
	void *p;
	int i, sz;

	i=BLNK_LookupModule(name);
	if(i<0)return;

	printf("%s:\n", name);

	p=BLNK_LookupModuleText(name, &sz);

#ifdef X86_64
	BDISASM_PrintOpcodes(p, 1, sz);
#else
	BDISASM_PrintOpcodes(p, 0, sz);
#endif
	printf("\n");

	p=BLNK_LookupModuleText(name, &sz);
	BDISASM_HexDump(p, sz);
	printf("\n");
}

static int basm_fptr_dummy() { return(0); }
BASM_API void *BASM_GetFPtrDummy(char *lbl)
{
	void *p;

	p=BASM_GetPtr(lbl);
	if(p)return(p);
	return((void *)(basm_fptr_dummy));
}


void *BASM_GetIP()
{
	BASM_Init();
	return(basm_asm->ip);
}

void *BASM_GetDP()
{
	BASM_Init();
	return(basm_asm->dp);
}

BASM_API char *BASM_GetAsmBuf()
{
	BASM_Init();
	return(basm_asmbuf);
}

BASM_API void BASM_ResetCtx(BASM_Context *ctx)
{
	int i;

	if(!ctx->text)
	{
		ctx->text=(byte *)malloc(1<<12);
		ctx->data=(byte *)malloc(1<<12);
		ctx->text_sz=1<<12;
		ctx->data_sz=1<<12;
	}

	ctx->ip=ctx->text;
	ctx->dp=ctx->data;

	ctx->fl&=~4;	//default output .text

#ifdef X86_64
	ctx->fl|=BASM_FL_X86_64;	//default x86-64
	BASM_SetCPU(ctx, BASM_CPU_X64);
#else
#ifdef X86
	ctx->fl&=~BASM_FL_X86_64;	//default x86-32
	BASM_SetCPU(ctx, BASM_CPU_X86);
#endif
#endif

#ifdef ARM
	ctx->fl&=~BASM_FL_X86_64;	//disable x86-64
	ctx->fl|=BASM_FL_ARM;	//enable ARM
	BASM_SetCPU(ctx, BASM_CPU_ARM);
#endif

	ctx->n_labels=0;
	ctx->n_gotos=0;
	ctx->n_const=0;
	ctx->n_proxy=0;

	//free temp-alloc stuff
	for(i=0; i<ctx->alloc_nblock; i++)
		free(ctx->alloc_block[i]);
	ctx->alloc_nblock=0;
}

BASM_API void BASM_CopyLblCtx(BASM_Context *ctx)
{
	int i;

	for(i=0; i<ctx->n_labels; i++)
	{
		ctx->llbl_name[i]=ctx->label_name[i];
		ctx->llbl_pos[i]=ctx->label_pos[i];
	}

	ctx->n_llbl=ctx->n_labels;
}

BASM_API void BASM_AssembleCtxBuf(BASM_Context *ctx, char *buf)
{
	static char *tbuf=NULL;
	int i, j;

	if(!tbuf)tbuf=malloc(1<<18);

//	strcpy(tbuf, buf);
	BASM_PP_Filter(buf, tbuf);

//	printf("BASM_PP: %s\n", tbuf);

	BASM_ResetCtx(basm_asm);
	ctx->n_llbl=0;

#if 1
	for(i=0; i<16; i++)
	{
		BASM_ParseOpcodeList(basm_asm, tbuf);

		if(ctx->n_llbl==ctx->n_labels)
		{
			for(j=0; j<ctx->n_labels; j++)
				if(ctx->llbl_pos[j]!=ctx->label_pos[j])
					break;
			if(j==ctx->n_labels)break;
		}

		BASM_CopyLblCtx(basm_asm);
		BASM_ResetCtx(basm_asm);
	}

	if(i>=16)BASM_ParseOpcodeList(basm_asm, tbuf);
#else
	BASM_ParseOpcodeList(basm_asm, tbuf);
#endif

//	free(tbuf);
}

BASM_API void BASM_AssembleCtxBufFast(BASM_Context *ctx, char *buf)
{
	int i, j;

	BASM_ResetCtx(basm_asm);
	ctx->n_llbl=0;
	BASM_ParseOpcodeList(basm_asm, buf);
}

#if 1
BASM_API void BASM_BeginAssembly(char *name)
{
	long i, j;

	BASM_Init();
	BASM_ResetCtx(basm_asm);

	basm_asm->modname=basm_strdup(name);
	basm_asmend=basm_asmbuf;
}
#endif

BASM_API void *BASM_EndAssembly()
{
	char buf[256];
	byte *ip, *obj;
	int sz;

//	printf("%s\n", basm_asmbuf);

#if 1
	if(basm_asm->modname)
	{
		sprintf(buf, "dump/%s.as", basm_asm->modname);
		blnk_storetextfile(buf, basm_asmbuf);
	}
#endif

//	BASM_ParseOpcodeList(basm_asm, basm_asmbuf);
	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);

	ip=NULL;

//	ip=basm_asm->text;
//	BDISASM_PrintOpcodes(ip, 0, basm_asm->ip-ip);

//	ip=BLNK_LinkModule(basm_asm);

	obj=BASM_COFF_EncodeObject(basm_asm, &sz);
	ip=BLNK_LinkModuleObj(NULL, obj, sz);
	free(obj);

	return((void *)ip);
}


BASM_API void *BASM_EndAssemblyQuiet()
{
	byte *ip, *obj;
	int sz;

//	BASM_ParseOpcodeList(basm_asm, basm_asmbuf);
	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);

//	ip=BLNK_LinkModule(basm_asm);

	obj=BASM_COFF_EncodeObject(basm_asm, &sz);
	ip=BLNK_LinkModuleObj(NULL, obj, sz);
	free(obj);

	return((void *)ip);
}

BASM_API void *BASM_EndAssemblyDebug()
{
	byte *ip, *obj;
	int sz;

	printf("%s\n", basm_asmbuf);

	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);

	obj=BASM_COFF_EncodeObject(basm_asm, &sz);
	ip=BLNK_LinkModuleObj(NULL, obj, sz);
	free(obj);

	return((void *)ip);
}

BASM_API void *BASM_EndAssemblyCache(char *name)
{
	byte *ip, *buf;
	int sz;

//	printf("%s\n", basm_asmbuf);

//	BASM_ParseOpcodeList(basm_asm, basm_asmbuf);
	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);

//	ip=BLNK_LinkModule(basm_asm);

	buf=BASM_COFF_EncodeObject(basm_asm, &sz);
	ip=BLNK_LinkModuleObj(name, buf, sz);
	blnk_storefile(name, buf, sz);
	free(buf);

	return((void *)ip);
}

BASM_API void BASM_EndAssemblyFile(char *name)
{
	byte *buf;
	int sz;
//	printf("%s\n", basm_asmbuf);

//	BASM_ParseOpcodeList(basm_asm, basm_asmbuf);
	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);
//	BASM_COFF_StoreObject(name, basm_asm);

	buf=BASM_COFF_EncodeObject(basm_asm, &sz);
	blnk_storefile(name, buf, sz);
	free(buf);
}

BASM_API byte *BASM_EndAssemblyObjBuf(int *rsz)
{
//	BASM_ParseOpcodeList(basm_asm, basm_asmbuf);
	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);
	return(BASM_COFF_EncodeObject(basm_asm, rsz));
}

BASM_API byte *BASM_EndAssemblyCacheObjBuf(int *rsz)
{
	byte *obj;
	int sz;

//	BASM_ParseOpcodeList(basm_asm, basm_asmbuf);
	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);
//	BLNK_LinkModule(basm_asm);

	obj=BASM_COFF_EncodeObject(basm_asm, &sz);
	BLNK_LinkModuleObj(NULL, obj, sz);

	return(obj);
}

BASM_API BLNK_Thunk *BASM_EndAssemblyThunk(BLNK_Scope *scope)
{
	BLNK_Thunk *obj;
	byte *buf;
	int sz;

//	BASM_ParseOpcodeList(basm_asm, basm_asmbuf);
	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);

	buf=BASM_COFF_EncodeObject(basm_asm, &sz);
	obj=BLNK_LinkModuleThunkObj(scope, buf, sz);
	free(buf);

//	obj=BLNK_LinkModuleThunk(basm_asm, scope);

	return(obj);
}

BASM_API byte *BASM_EndAssemblyThunkInline(BLNK_Scope *scope)
{
	byte *obj, *buf;
	int sz;

//	printf("%s\n", basm_asmbuf);

//	BASM_ParseOpcodeList(basm_asm, basm_asmbuf);
	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);
//	obj=BLNK_LinkThunkInline(basm_asm, scope);

	buf=BASM_COFF_EncodeObject(basm_asm, &sz);
	obj=BLNK_LinkModuleThunkInlineObj(scope, buf, sz);
	free(buf);

//	BDISASM_PrintOpcodes(obj, 0, 64);

	return(obj);
}

BASM_API void BASM_EndAssemblyQueue()
{
	byte *ip, *obj;
	int sz;

	BASM_AssembleCtxBuf(basm_asm, basm_asmbuf);
	obj=BASM_COFF_EncodeObject(basm_asm, &sz);
	BLNK_LinkModuleObjQueue(NULL, obj, sz);
	free(obj);
}

BASM_API void *BASM_EndAssemblyFast()
{
	byte *ip, *obj;
	int sz;

	BASM_AssembleCtxBufFast(basm_asm, basm_asmbuf);

	obj=BASM_COFF_EncodeObject(basm_asm, &sz);
	ip=BLNK_LinkModuleObj(NULL, obj, sz);
	free(obj);

	return((void *)ip);
}

BASM_API byte *BASM_EndAssemblyObjBufFast(int *rsz)
{
	BASM_AssembleCtxBufFast(basm_asm, basm_asmbuf);
	return(BASM_COFF_EncodeObject(basm_asm, rsz));
}

BASM_API byte *BASM_EndAssemblyCacheObjBufFast(int *rsz)
{
	byte *obj;
	int sz;

	BASM_AssembleCtxBufFast(basm_asm, basm_asmbuf);
	obj=BASM_COFF_EncodeObject(basm_asm, &sz);
	BLNK_LinkModuleObj(NULL, obj, sz);

	return(obj);
}

BASM_API void BASM_EndAssemblyQueueFast()
{
	byte *ip, *obj;
	int sz;

	BASM_AssembleCtxBufFast(basm_asm, basm_asmbuf);
	obj=BASM_COFF_EncodeObject(basm_asm, &sz);
	BLNK_LinkModuleObjQueue(NULL, obj, sz);
	free(obj);
}

BASM_API void BASM_EndAssemblyVirtualHeader()
{
	BASM_PP_AddVirtualHeader(basm_asm->modname, basm_asmbuf);
}

BASM_API void BASM_FreeAssembly(void *ip)
{
}

BASM_API void BASM_FreeObjBuf(void *ip)
{
	free(ip);
}


BASM_API char *BASM_GenSym()
{
	char buf[64];

	BASM_Init();

//	sprintf(buf, "BASM$%d", basm_seq++);
//	sprintf(buf, "BASM$%s", basm_rand_key18());
//	sprintf(buf, "$%s", basm_rand_key18());
	sprintf(buf, "%s", basm_rand_key18());
	return(basm_rstrdup(buf));
}

BASM_API char *BASM_GenSym2()
{
	char buf[64];
//	sprintf(buf, "BASM$%d", basm_seq++);
//	sprintf(buf, "BASM$%s", basm_rand_key18());
//	sprintf(buf, "$%s", basm_rand_key18());
	sprintf(buf, "_%s", basm_rand_key18());
	return(basm_rstrdup(buf));
}

BASM_API int BASM_LoadObjectBuffer(char *name, byte *buf, int sz)
{
	BLNK_LinkModuleObjQueue(name, buf, sz);
	return(0);
}

BASM_API int BASM_LoadFile(char *name)
{
	byte *ip, *dp;
	char *buf, *s;
	FILE *fd;
	int sz;

	BASM_Init();

	s=name;
	while(*s && (*s!='.'))s++;

	if(!*s)
	{
		printf("BASM_LoadFile: no file extension for %s\n", name);
		return(-1);
	}

	if(!basm_stricmp(s, ".o") || !basm_stricmp(s, ".obj"))
	{
		BLNK_COFF_LoadObject(name);
		return(0);
	}

	if(!basm_stricmp(s, ".a") || !basm_stricmp(s, ".lib"))
	{
		BLNK_COFF_LoadLibrary(name);
		return(0);
	}

	if(basm_stricmp(s, ".s") && basm_stricmp(s, ".as") &&
		basm_stricmp(s, ".asm"))
	{
		printf("BASM_LoadFile: unknown file extension %s\n", s+1);
		return(-1);
	}


//	fd=fopen(name, "rb");
//	if(!fd)return(-1);

//	fseek(fd, 0, 2);
//	sz=ftell(fd);
//	buf=(char *)malloc(sz+1);

//	fseek(fd, 0, 0);
//	fread(buf, 1, sz, fd);
//	buf[sz]=0;


	buf=basm_loadfile(name, &sz);
	if(!buf)return(-1);

	BASM_BeginAssembly(name);

	ip=basm_asm->ip;
	dp=basm_asm->dp;

//	BASM_AssembleCtxBuf(basm_asm, buf);
//	BASM_ParseOpcodeList(basm_asm, buf);

	basm_puts(buf);
	BASM_EndAssembly();

	printf("Assembled %d bytes text, %d bytes data\n",
		basm_asm->ip-ip, basm_asm->dp-dp);
//	BDISASM_PrintOpcodes(ip, 0, basm_asm->ip-ip);

	free(buf);
	return(0);
}

BASM_API int BASM_AssembleFile(char *iname, char *oname)
{
	byte *ip, *dp;
	char *buf, *s;
	FILE *fd;
	int sz;

	BASM_Init();

	s=iname;
	while(*s && (*s!='.'))s++;


//	fd=fopen(iname, "rb");
//	if(!fd)return(-1);

//	fseek(fd, 0, 2);
//	sz=ftell(fd);
//	buf=(char *)malloc(sz+1);

//	fseek(fd, 0, 0);
//	fread(buf, 1, sz, fd);
//	buf[sz]=0;


	buf=basm_loadfile(iname, &sz);
	if(!buf)return(-1);

	BASM_BeginAssembly(iname);
//	BASM_ParseOpcodeList(basm_asm, buf);
//	BASM_AssembleCtxBuf(basm_asm, buf);
	basm_puts(buf);
	BASM_EndAssemblyFile(oname);
	free(buf);

	return(0);
}

BASM_API int BASM_ProcessEXE(char *name)
{
	BLNK_ProcessEXE(name);
	return(0);
}


BASM_API void BASM_ThreadLocalInit()
{
	if(!basm_asmbuf)
	{
		basm_asmbuf=(char *)malloc(1<<16);
		basm_asmbuf_end=basm_asmbuf+(1<<16);

		basm_asmend=basm_asmbuf;
//		memset(basm_asmbuf, 0, 1<<20);
	}

	if(!basm_asm)
	{
		basm_asm=BASM_NewContext();

//		basm_asm->text=basm_text;
//		basm_asm->data=basm_data;
//		basm_asm->ip=basm_text;
//		basm_asm->dp=basm_data;

		basm_asm->text=(byte *)malloc(1<<12);
		basm_asm->data=(byte *)malloc(1<<12);
		basm_asm->text_sz=1<<12;
		basm_asm->data_sz=1<<12;

		basm_asm->ip=basm_asm->text;
		basm_asm->dp=basm_asm->data;

#ifdef X86_64
		basm_asm->fl|=BASM_FL_X86_64;
		BASM_SetCPU(basm_asm, BASM_CPU_X64);
#else
#ifdef X86
		BASM_SetCPU(basm_asm, BASM_CPU_X86);
#endif
#endif

#ifdef ARM
		basm_asm->fl|=BASM_FL_ARM;
		BASM_SetCPU(basm_asm, BASM_CPU_ARM);
#endif
	}
}

BASM_API void BASM_Init()
{
	static int init=0;

	BASM_Context *ctx;
	void *p;
	int i, j, k, l;

	if(init)
	{
		BASM_ThreadLocalInit();
		return;
	}
	init=1;

#ifdef WIN32
//	i=GetModuleFileNameA(NULL, tb, 256);
//	if(i>0)BASM_ProcessEXE(tb);
#endif

	BLNK_InitLink();
//	BASM_InitXMeta();

	BASM_InitRand();

	BASM_ThreadLocalInit();

	BASM_RegisterPtr("basm_cpuid_str", basm_cpuid_str);
	BASM_RegisterPtr("basm_cpuid_std", basm_cpuid_std);

#ifdef X86
	BASM_PP_AddStaticDefine("x86", "1");
	BASM_PP_AddStaticDefine("__i386__", "1");

	BASM_BeginAssembly(NULL);

	BASM_ParseOpcodeList(basm_asm, "__dummy: nop; nop;\n\n");

	basm_print(
		"basm_cpuinfo:\n"
		"push ebx\n"
		"xor eax, eax; cpuid\n"
		"mov [basm_cpuid_str+0], ebx\n"
		"mov [basm_cpuid_str+4], edx\n"
		"mov [basm_cpuid_str+8], ecx\n"
		"xor eax, eax; inc eax; cpuid\n"
		"mov [basm_cpuid_std+0], eax\n"
		"mov [basm_cpuid_std+4], ebx\n"
		"mov [basm_cpuid_std+8], ecx\n"
		"mov [basm_cpuid_std+12], edx\n"
		"pop ebx; ret\n"
		"basm_getbp: mov eax, ebp; ret\n"
	);

	BASM_EndAssemblyQuiet();

	p=BASM_GetPtr("basm_cpuinfo");
	if(p) ((void (*)())p)();

#if 0
	printf("CPU Sig=%s Ver=%08X Unit=%08X Ext=%08X Feature=%08X\n",
		basm_cpuid_str,
		basm_cpuid_std[0], basm_cpuid_std[1],
		basm_cpuid_std[2], basm_cpuid_std[3]);

	printf("CPU FeatureStr=%s\n", BASM_CPUID_FeatureStr());
#endif
#endif


#ifdef X86_64
	BASM_PP_AddStaticDefine("x64", "1");
	BASM_PP_AddStaticDefine("x86_64", "1");
	BASM_PP_AddStaticDefine("__x86_64__", "1");

	BASM_BeginAssembly(NULL);

	BASM_ParseOpcodeList(basm_asm, "__dummy: nop; nop;\n\n");

	basm_print(
		"[bits 64]\n"
		"basm_cpuinfo:\n"
		"push rbx\n"
		"xor eax, eax; cpuid\n"
		"mov r8, basm_cpuid_str\n"
		"mov [r8+0], ebx\n"
		"mov [r8+4], edx\n"
		"mov [r8+8], ecx\n"
		"xor eax, eax; inc eax; cpuid\n"
		"mov r8, basm_cpuid_std\n"
		"mov [r8+0], eax\n"
		"mov [r8+4], ebx\n"
		"mov [r8+8], ecx\n"
		"mov [r8+12], edx\n"
		"pop rbx; ret\n"
		"basm_getbp: mov rax, rbp; ret\n"
	);

	BASM_EndAssembly();

	p=BASM_GetPtr("basm_cpuinfo");

//	if(p)
//	{
//		printf("Got %p\n", p);
//		BDISASM_PrintOpcodes(p, 1, 64);
//	}

	if(p) ((void (*)())p)();

#if 0
	printf("CPU Sig=%s Ver=%08X Unit=%08X Ext=%08X Feature=%08X\n\n",
		basm_cpuid_str,
		basm_cpuid_std[0], basm_cpuid_std[1],
		basm_cpuid_std[2], basm_cpuid_std[3]);

	printf("CPU FeatureStr=%s\n", BASM_CPUID_FeatureStr());
#endif
#endif

#ifdef ARM
	BASM_PP_AddStaticDefine("arm", "1");
	BASM_PP_AddStaticDefine("__arm__", "1");
#endif

//	BASM_InitUnwind();
}
