#include <bgbasm.h>

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
extern char *basm_ops_arm[];
extern int basm_opidx_arm[];
extern int basm_opnums_arm[];
extern char basm_opcat_arm[];
extern char *basm_opdecl_arm[];
extern char *basm_regs_arm[];
extern int basm_regidx_arm[];
#endif

#if 1
extern char *basm_ops_thumb[];
extern int basm_opidx_thumb[];
extern int basm_opnums_thumb[];
extern char basm_opcat_thumb[];
extern char *basm_opdecl_thumb[];
extern char *basm_regs_thumb[];
extern int basm_regidx_thumb[];
#endif

#if 1
char **basm_ops;
int *basm_opidx;
int *basm_opnums;

char *basm_opcat;
char **basm_opdecl;

char **basm_regs;
int *basm_regidx;
#endif

int basm_cpu;

FILE *basm_log=NULL;

void BASM_SetCPU(BASM_Context *ctx, int cpu)
{
	ctx->fl&=~(BASM_FL_X86_64|BASM_FL_X86_16|BASM_FL_ARM|BASM_FL_THUMB);
	ctx->fl&=~(BASM_FL_DATA|BASM_FL_ADDRSZ|BASM_FL_GAS);
	ctx->fl&=~(BASM_FL_BUNDLE);

	if((cpu==BASM_CPU_X64) || (cpu==BASM_CPU_NACL_X64))
		ctx->fl|=BASM_FL_X86_64;
	if(cpu==BASM_CPU_ARM)
		ctx->fl|=BASM_FL_ARM;
	if(cpu==BASM_CPU_THUMB)
		ctx->fl|=BASM_FL_ARM|BASM_FL_THUMB;

	if((cpu==BASM_CPU_NACL_X86) || (cpu==BASM_CPU_NACL_X64))
		ctx->fl|=BASM_FL_BUNDLE;

	ctx->cpu=cpu;
	basm_cpu=cpu;

	switch(cpu)
	{
	case BASM_CPU_X86:
	case BASM_CPU_X64:
	case BASM_CPU_NACL_X86:
	case BASM_CPU_NACL_X64:
		basm_ops=basm_ops_x86;
		basm_opidx=basm_opidx_x86;
		basm_opnums=basm_opnums_x86;
		basm_opcat=basm_opcat_x86;
		basm_opdecl=basm_opdecl_x86;
		basm_regs=basm_regs_x86;
		basm_regidx=basm_regidx_x86;
		break;
	case BASM_CPU_ARM:
		basm_ops=basm_ops_arm;
		basm_opidx=basm_opidx_arm;
		basm_opnums=basm_opnums_arm;
		basm_opcat=basm_opcat_arm;
		basm_opdecl=basm_opdecl_arm;
		basm_regs=basm_regs_arm;
		basm_regidx=basm_regidx_arm;
		break;
	case BASM_CPU_THUMB:
		basm_ops=basm_ops_thumb;
		basm_opidx=basm_opidx_thumb;
		basm_opnums=basm_opnums_thumb;
		basm_opcat=basm_opcat_thumb;
		basm_opdecl=basm_opdecl_thumb;
		basm_regs=basm_regs_thumb;
		basm_regidx=basm_regidx_thumb;
		break;
	default:
		break;
	}
}

void basm_warning(char *str, ...)
{
	char tb[4096];
	va_list lst;

	if(!basm_log)basm_log=fopen("bgbasm_log.txt", "wt");

	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);

	if(basm_log)
	{
		fputs(tb, basm_log);
		fflush(basm_log);
	}
	fputs(tb, stderr);
	fflush(stderr);
}

void basm_error(char *str, ...)
{
	char tb[4096];
	va_list lst;

	if(!basm_log)basm_log=fopen("bgbasm_log.txt", "wt");

	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);

	if(basm_log)
	{
		fputs(tb, basm_log);
		fflush(basm_log);
	}
	fputs(tb, stderr);
	fflush(stderr);
}


BASM_API BASM_Context *BASM_NewContext()
{
	BASM_Context *tmp;

	tmp=(BASM_Context *)malloc(sizeof(BASM_Context));
	memset(tmp, 0, sizeof(BASM_Context));

	tmp->label_name=(char **)malloc(256*sizeof(char *));
	tmp->label_pos=(int *)malloc(256*sizeof(int));

	tmp->goto_name=(char **)malloc(256*sizeof(char *));
	tmp->goto_pos=(int *)malloc(256*sizeof(int));
	tmp->goto_type=(byte *)malloc(256*sizeof(byte));

	tmp->const_name=(char **)malloc(256*sizeof(char *));
	tmp->const_value=(long long *)malloc(256*sizeof(long long));

	tmp->proxy_name=(char **)malloc(256*sizeof(char *));

	tmp->m_labels=256;
	tmp->m_gotos=256;
	tmp->m_const=256;
	tmp->m_proxy=256;

	tmp->llbl_name=(char **)malloc(256*sizeof(char *));
	tmp->llbl_pos=(int *)malloc(256*sizeof(int));

	return(tmp);
}

BASM_API void BASM_DestroyContext(BASM_Context *ctx)
{
	if(!ctx)return;

	free(ctx->label_name);
	free(ctx->label_pos);

	free(ctx->goto_name);
	free(ctx->goto_pos);
	free(ctx->goto_type);

	free(ctx->const_name);
	free(ctx->const_value);

	free(ctx->proxy_name);

	free(ctx->llbl_name);
	free(ctx->llbl_pos);
	
	free(ctx);
}

BASM_API void *BASM_TempAllocTy(BASM_Context *ctx, char *ty, int sz)
{
	void *p;

	if(!ctx->alloc_rov)
	{
		ctx->alloc_srov=malloc(1<<18);
		ctx->alloc_erov=ctx->alloc_srov+(1<<18);

		ctx->alloc_rov=ctx->alloc_srov;

		ctx->alloc_block[ctx->alloc_nblock++]=
			ctx->alloc_rov;
	}

	if((ctx->alloc_rov+sz)>=(ctx->alloc_erov-4096))
	{
		printf("BASM_TempAlloc: Expand\n");

		ctx->alloc_srov=malloc(1<<18);
		ctx->alloc_erov=ctx->alloc_srov+(1<<18);
		ctx->alloc_rov=ctx->alloc_srov;

		ctx->alloc_block[ctx->alloc_nblock++]=
			ctx->alloc_rov;
	}

	p=ctx->alloc_rov;
	ctx->alloc_rov+=(sz+15)&(~15);

	memset(p, 0, sz);
	return(p);
}

BASM_API void *BASM_TempAlloc(BASM_Context *ctx, int sz)
	{ return(BASM_TempAllocTy(ctx, NULL, sz)); }


BASM_API void BASM_OutPadText(BASM_Context *ctx, int i)
{
	static byte seqs[9][16]={
	{ 0x90, }, { 0x66, 0x90, }, { 0x0F, 0x1F, 0x00, },
	{ 0x0F, 0x1F, 0x40, 0x00, }, { 0x0F, 0x1F, 0x44, 0x00, 0x00, },
	{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00, },
	{ 0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00, },
	{ 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, },
	{ 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, },
	};
	int j;

	if(i<=0)return;

#if 0
	j=i;
	while(j>1)
	{
		BASM_OutByte(ctx, 0x66);
		BASM_OutByte(ctx, 0x90);
		j-=2;
	}
	if(j)BASM_OutByte(ctx, 0x90);
	return;
#endif

#if 1
	if(i>9)
	{
		if(i==10)
		{
			BASM_OutPadText(ctx, 7);
			BASM_OutPadText(ctx, 3);
			return;
		}

		if(i==14)
		{
			BASM_OutPadText(ctx, 7);
			BASM_OutPadText(ctx, 7);
			return;
		}

		BASM_OutPadText(ctx, 8);
		BASM_OutPadText(ctx, i-8);
		return;
	}

	for(j=0; j<i; j++)
		BASM_OutByte(ctx, seqs[i][j]);
#endif
}

BASM_API void BASM_OutPadZero(BASM_Context *ctx, int i)
{
	while(i--)BASM_OutByte(ctx, 0x00);
}

BASM_API void BASM_OutPadVLI(BASM_Context *ctx, int i)
{
	switch(i)
	{
	case 0: break;
	case 1: BASM_OutByte(ctx, 0x00); break;
	case 2: BASM_OutByte(ctx, 0x80); BASM_OutByte(ctx, 0x00); break;
	case 3:	BASM_OutByte(ctx, 0xC0); BASM_OutPadZero(ctx, 2); break;
	case 4:	BASM_OutByte(ctx, 0xE0); BASM_OutPadZero(ctx, 3); break;
	case 5:	BASM_OutByte(ctx, 0xF0); BASM_OutPadZero(ctx, 4); break;
	case 6:	BASM_OutByte(ctx, 0xF8); BASM_OutPadZero(ctx, 5); break;
	case 7:	BASM_OutByte(ctx, 0xFC); BASM_OutPadZero(ctx, 6); break;
	case 8:	BASM_OutByte(ctx, 0xFE); BASM_OutPadZero(ctx, 7); break;
	case 9:	BASM_OutByte(ctx, 0xFF); BASM_OutByte(ctx, 0x00);
		BASM_OutPadZero(ctx, 7); break;
	case 10: BASM_OutByte(ctx, 0xFF); BASM_OutByte(ctx, 0x80);
		BASM_OutPadZero(ctx, 8); break;
	case 11: BASM_OutByte(ctx, 0xFF); BASM_OutByte(ctx, 0xC0);
		BASM_OutPadZero(ctx, 9); break;
	case 12: BASM_OutByte(ctx, 0xFF); BASM_OutByte(ctx, 0xE0);
		BASM_OutPadZero(ctx, 10); break;
	case 13: BASM_OutByte(ctx, 0xFF); BASM_OutByte(ctx, 0xE0);
		BASM_OutPadZero(ctx, 11); break;
	case 14: BASM_OutByte(ctx, 0xFF); BASM_OutByte(ctx, 0xF0);
		BASM_OutPadZero(ctx, 12); break;
	case 15: BASM_OutByte(ctx, 0xFF); BASM_OutByte(ctx, 0xF8);
		BASM_OutPadZero(ctx, 13); break;
	}
}

BASM_API void BASM_OutPad(BASM_Context *ctx, int i)
{
	int j, k;

	if(i<=0)return;
	if(ctx->fl&4)
		{ BASM_OutPadZero(ctx, i); }
	else
		{ BASM_OutPadText(ctx, i); }
}

#if 0
BASM_API void BASM_Align(BASM_Context *ctx, int i)
{
	int j, k;

	if(ctx->fl&4)
	{
		j=ctx->dp-ctx->data;
		k=j; if(j%i)j+=i-(j%i);

		if(j>=ctx->data_sz)
		{
			while(j>=ctx->data_sz)
				ctx->data_sz+=ctx->data_sz>>1;
			ctx->data=(byte *)realloc(ctx->data, ctx->data_sz);
//			ctx->dp=ctx->data+j;
		}
		while(k<j) { *ctx->dp++=0; k++; }
	}else
	{
#if 1
		j=ctx->ip-ctx->text;
		if(j%i)BASM_OutPadText(ctx, i-(j%i));
#else
		j=ctx->ip-ctx->text;
		k=j; if(j%i)j+=i-(j%i);

		if(j>=ctx->text_sz)
		{
			while(j>=ctx->text_sz)
				ctx->text_sz+=ctx->text_sz>>1;
			ctx->text=(byte *)realloc(ctx->text, ctx->text_sz);
//			ctx->ip=ctx->text+j;
		}
		while(k<j) { *ctx->ip++=0; k++; }
#endif
	}
}
#endif

BASM_API int BASM_GetSectionOffset(BASM_Context *ctx)
{
	int j, k;

	if(ctx->fl&4)
	{
		j=ctx->dp-ctx->data;
		return(j);
	}else
	{
		j=ctx->ip-ctx->text;
		return(j);
	}
}

BASM_API void BASM_BundleAlign(BASM_Context *ctx)
{
	if(!(ctx->fl&BASM_FL_BUNDLE))
		return;
	if(ctx->fl&BASM_FL_DATA)
		return;

	BASM_Align(ctx, 32);
}

BASM_API void BASM_Align(BASM_Context *ctx, int i)
{
	int j, k;

	if(ctx->fl&4)
	{
		j=ctx->dp-ctx->data;
		if(j%i)BASM_OutPadZero(ctx, i-(j%i));
	}else
	{
		j=ctx->ip-ctx->text;
		if(j%i)BASM_OutPadText(ctx, i-(j%i));
	}
}

BASM_API void BASM_AlignVLI(BASM_Context *ctx, int i)
{
	int j, k;

	if(ctx->fl&4)
	{
		j=ctx->dp-ctx->data;
		if(j%i)BASM_OutPadVLI(ctx, i-(j%i));
	}else
	{
		j=ctx->ip-ctx->text;
		if(j%i)BASM_OutPadVLI(ctx, i-(j%i));
	}
}

BASM_API void BASM_OutByte(BASM_Context *ctx, int i)
{
	int j;

	if(ctx->fl&4)
	{
		j=ctx->dp-ctx->data;
		if(j>=ctx->data_sz)
		{
			ctx->data_sz+=ctx->data_sz>>1;
			ctx->data=(byte *)realloc(ctx->data, ctx->data_sz);
			ctx->dp=ctx->data+j;
		}

		*ctx->dp++=i;
	}else
	{
		j=ctx->ip-ctx->text;
		if(j>=ctx->text_sz)
		{
			ctx->text_sz+=ctx->text_sz>>1;
			ctx->text=(byte *)realloc(ctx->text, ctx->text_sz);
			ctx->ip=ctx->text+j;
		}

		*ctx->ip++=i;
	}
}

BASM_API void BASM_OutWord(BASM_Context *ctx, int i)
{
	BASM_OutByte(ctx, i&0xFF);
	BASM_OutByte(ctx, (i>>8)&0xFF);
}

BASM_API void BASM_OutDWord(BASM_Context *ctx, int i)
{
	BASM_OutByte(ctx, i&0xFF);
	BASM_OutByte(ctx, (i>>8)&0xFF);
	BASM_OutByte(ctx, (i>>16)&0xFF);
	BASM_OutByte(ctx, (i>>24)&0xFF);
}

BASM_API void BASM_OutQWord(BASM_Context *ctx,
	long long i)
{
	BASM_OutByte(ctx, i&0xFF);
	BASM_OutByte(ctx, (i>>8)&0xFF);
	BASM_OutByte(ctx, (i>>16)&0xFF);
	BASM_OutByte(ctx, (i>>24)&0xFF);
	BASM_OutByte(ctx, (i>>32)&0xFF);
	BASM_OutByte(ctx, (i>>40)&0xFF);
	BASM_OutByte(ctx, (i>>48)&0xFF);
	BASM_OutByte(ctx, (i>>56)&0xFF);
}

BASM_API void BASM_OutUVLIP(BASM_Context *ctx,
	unsigned long long i, int pad)
{
	if(i<((1LL<<7)-pad))
	{
		BASM_OutByte(ctx, i&0x7F); return;
	}
	if(i<((1LL<<14)-pad))
	{
		BASM_OutByte(ctx, 0x80|((i>>8)&0x3F));
		BASM_OutByte(ctx, i&0xFF);
		return;
	}
	if(i<((1LL<<21)-pad))
	{
		BASM_OutByte(ctx, 0xC0|((i>>16)&0x1F));
		BASM_OutByte(ctx, (i>>8)&0xFF);
		BASM_OutByte(ctx, i&0xFF);
		return;
	}

	if(i<((1LL<<28)-pad))
	{
		BASM_OutByte(ctx, 0xE0|((i>>24)&0x0F));
		BASM_OutByte(ctx, (i>>16)&0xFF);
		BASM_OutByte(ctx, (i>>8)&0xFF);
		BASM_OutByte(ctx, i&0xFF);
		return;
	}

	if(i<((1LL<<35)-pad))
	{
		BASM_OutByte(ctx, 0xF0|((i>>32)&0x07));
		BASM_OutByte(ctx, (i>>24)&0xFF);
		BASM_OutByte(ctx, (i>>16)&0xFF);
		BASM_OutByte(ctx, (i>>8)&0xFF);
		BASM_OutByte(ctx, i&0xFF);
		return;
	}

	if(i<((1LL<<42)-pad))
	{
		BASM_OutByte(ctx, 0xF8|((i>>40)&0x03));
		BASM_OutByte(ctx, (i>>32)&0xFF);
		BASM_OutByte(ctx, (i>>24)&0xFF);
		BASM_OutByte(ctx, (i>>16)&0xFF);
		BASM_OutByte(ctx, (i>>8)&0xFF);
		BASM_OutByte(ctx, i&0xFF);
		return;
	}

	if(i<((1LL<<49)-pad))
	{
		BASM_OutByte(ctx, 0xFC|((i>>48)&0x01));
		BASM_OutByte(ctx, (i>>40)&0xFF);
		BASM_OutByte(ctx, (i>>32)&0xFF);
		BASM_OutByte(ctx, (i>>24)&0xFF);
		BASM_OutByte(ctx, (i>>16)&0xFF);
		BASM_OutByte(ctx, (i>>8)&0xFF);
		BASM_OutByte(ctx, i&0xFF);
		return;
	}

	if(i<((1LL<<56)-pad))
	{
		BASM_OutByte(ctx, 0xFE);
		BASM_OutByte(ctx, (i>>48)&0xFF);
		BASM_OutByte(ctx, (i>>40)&0xFF);
		BASM_OutByte(ctx, (i>>32)&0xFF);
		BASM_OutByte(ctx, (i>>24)&0xFF);
		BASM_OutByte(ctx, (i>>16)&0xFF);
		BASM_OutByte(ctx, (i>>8)&0xFF);
		BASM_OutByte(ctx, i&0xFF);
		return;
	}

	if(i<((1LL<<63)-pad))
	{
		BASM_OutByte(ctx, 0xFF);
		BASM_OutByte(ctx, (i>>56)&0x7F);
		BASM_OutByte(ctx, (i>>48)&0xFF);
		BASM_OutByte(ctx, (i>>40)&0xFF);
		BASM_OutByte(ctx, (i>>32)&0xFF);
		BASM_OutByte(ctx, (i>>24)&0xFF);
		BASM_OutByte(ctx, (i>>16)&0xFF);
		BASM_OutByte(ctx, (i>>8)&0xFF);
		BASM_OutByte(ctx, i&0xFF);
		return;
	}

//	BASM_OutByte(ctx, 0x80|((i>>64)&0x3F));

	BASM_OutByte(ctx, 0xFF);
	BASM_OutByte(ctx, 0x80);
	BASM_OutByte(ctx, (i>>56)&0xFF);
	BASM_OutByte(ctx, (i>>48)&0xFF);
	BASM_OutByte(ctx, (i>>40)&0xFF);
	BASM_OutByte(ctx, (i>>32)&0xFF);
	BASM_OutByte(ctx, (i>>24)&0xFF);
	BASM_OutByte(ctx, (i>>16)&0xFF);
	BASM_OutByte(ctx, (i>>8)&0xFF);
	BASM_OutByte(ctx, i&0xFF);
	return;
}

BASM_API void BASM_OutUVLI(BASM_Context *ctx, unsigned long long i)
{
	BASM_OutUVLIP(ctx, i, 0);
}

BASM_API void BASM_OutSVLI(BASM_Context *ctx, long long i)
{
	i=(i<0)?(((-i)<<1)-1):(i<<1);
	BASM_OutUVLIP(ctx, i, 0);
}

BASM_API void BASM_OutSVLIP(BASM_Context *ctx, long long i, int pad)
{
	i=(i<0)?(((-i)<<1)-1):(i<<1);
	BASM_OutUVLIP(ctx, i, pad);
}

BASM_API void BASM_OutStr8(BASM_Context *ctx, char *str)
{
	unsigned char *s;

	s=(unsigned char *)str;
	while(*s)BASM_OutByte(ctx, *s++);
}

BASM_API void BASM_OutStr16(BASM_Context *ctx, char *str)
{
	char *s;
	int i, j;

//	s=str; while(*s)BASM_OutWord(ctx, *s++);

	s=str;
	while(*s)
	{
		i=BASM_Parse_ParseChar(&s);

		if(i>=0x10000)
		{
			//encode as UTF-16 surrogate pairs
			j=i-0x10000;
			BASM_OutWord(ctx, 0xD800+((j>>10)&0x3FF));
			BASM_OutWord(ctx, 0xDC00+(j&0x3FF));
			continue;
		}

		//chars <= 0xFFFF and CESU-8
		BASM_OutWord(ctx, i);
	}
}

BASM_API void BASM_OutStr8Z(BASM_Context *ctx, char *str)
	{ BASM_OutStr8(ctx, str); BASM_OutByte(ctx, 0); }
BASM_API void BASM_OutStr16Z(BASM_Context *ctx, char *str)
	{ BASM_OutStr16(ctx, str); BASM_OutWord(ctx, 0); }

BASM_API void BASM_OutBytes(BASM_Context *ctx, byte *buf, int sz)
{
	int i;
	for(i=0; i<sz; i++)
		BASM_OutByte(ctx, buf[i]);
}

BASM_API void BASM_OutSection(BASM_Context *ctx, char *str)
{
	if(!basm_stricmp(str, ".text"))ctx->fl&=~4;
	if(!basm_stricmp(str, ".data"))ctx->fl|=4;
	if(!basm_stricmp(str, ".bss"))ctx->fl|=4;
}

BASM_API void BASM_OutBits(BASM_Context *ctx, int bits)
{
	if(bits==16) ctx->fl|=2;
	if(bits==32) ctx->fl&=~3;
	if(bits==64) ctx->fl|=1;
}

BASM_API void BASM_OutRelPtrDisp(BASM_Context *ctx, char *lbl, int disp)
{
	if(!lbl) { BASM_OutDWord(ctx, disp); return; }
	BASM_OutDWord(ctx, disp+4);
	BASM_EmitGoto(ctx, lbl, BASM_JMP_NEAR32);
}

BASM_API void BASM_OutRelSPtrDisp(BASM_Context *ctx, char *lbl, int disp)
{
	if(!lbl) { BASM_OutWord(ctx, disp); return; }
	BASM_OutWord(ctx, disp+2);
	BASM_EmitGoto(ctx, lbl, BASM_JMP_NEAR16);
}

//static int is_s8(int i)
//{
//	if(i<-128)return(0);
//	if(i>127)return(0);
//	return(1);
//}

void BASM_ModRM16(BASM_Context *ctx, int reg,
	int rm, int sc, int idx, int disp, char *lbl)
{
	int i;

	i=-1;

	if(sc && (sc!=1))
	{
		basm_error("BASM_ModRM16: Invalid 16-bit Mem Reference (Scale)\n");
		*(int *)-1=-1;
		return;
	}

	if(sc)
	{
		if((rm==BASM_REG_X86_BX) && (idx==BASM_REG_X86_SI))i=0;
		if((rm==BASM_REG_X86_BX) && (idx==BASM_REG_X86_DI))i=1;
		if((rm==BASM_REG_X86_BP) && (idx==BASM_REG_X86_SI))i=2;
		if((rm==BASM_REG_X86_BP) && (idx==BASM_REG_X86_DI))i=3;
		if((rm==BASM_REG_X86_SI) && (idx==BASM_Z))i=4;
		if((rm==BASM_REG_X86_DI) && (idx==BASM_Z))i=5;
		if((rm==BASM_REG_X86_BP) && (idx==BASM_Z))i=6;
		if((rm==BASM_REG_X86_BX) && (idx==BASM_Z))i=7;
	}

	if((i<0) && sc && ((rm!=BASM_Z) || (idx!=BASM_Z)))
	{
		basm_error("BASM_ModRM16: Invalid 16-bit Mem Reference (Regs)\n");
		*(int *)-1=-1;
		return;
	}

	if(i>=0)
	{
		if(!disp && (i!=6))
		{
			BASM_OutByte(ctx, ((reg&7)<<3)|i);
			return;
		}

		if((disp>=-128) && (disp<=127))
		{
			BASM_OutByte(ctx, 0x40|((reg&7)<<3)|i);
			BASM_OutByte(ctx, disp);
			return;
		}

		BASM_OutByte(ctx, 0x80|((reg&7)<<3)|i);
		BASM_OutWord(ctx, disp);
		return;
	}

	if(disp || lbl)
	{
		BASM_OutByte(ctx, ((reg&7)<<3)|6);
		BASM_OutWord(ctx, disp);
		BASM_EmitGoto(ctx, lbl, BASM_JMP_ABS16);
		return;
	}else
	{
		BASM_OutByte(ctx, 0xC0|((reg&7)<<3)|(rm&7));
		return;
	}
}

void BASM_Sib(BASM_Context *ctx, int rm, int sc, int idx)
{
	if(idx==BASM_Z)idx=4;

	switch(sc)
	{
	case 0:
	case 1:
		BASM_OutByte(ctx, ((idx&7)<<3)|(rm&7));
		break;
	case 2:
		BASM_OutByte(ctx, 0x40|((idx&7)<<3)|(rm&7));
		break;
	case 4:
		BASM_OutByte(ctx, 0x80|((idx&7)<<3)|(rm&7));
		break;
	case 8:
		BASM_OutByte(ctx, 0xC0|((idx&7)<<3)|(rm&7));
		break;
	}
}

BASM_API void BASM_LabelDisp(BASM_Context *ctx, char *lbl, int disp)
{
	int i;

	if(!lbl)
	{
		BASM_OutDWord(ctx, disp);
		return;
	}

	if(!strcmp(lbl, "$."))
	{
		i=ctx->ip-ctx->base_ip;
		BASM_OutDWord(ctx, disp+i+4);
		return;
	}

	if(*lbl=='$')
	{
		i=ctx->ip-ctx->base_ip;
		BASM_OutDWord(ctx, disp+i+4);
		BASM_EmitGoto(ctx, lbl+1, BASM_JMP_NEAR32);
		return;
	}

	if(*lbl=='@')lbl++;	//ABS reference

	//default: ABS reference
	BASM_OutDWord(ctx, disp);
	BASM_EmitGoto(ctx, lbl, BASM_JMP_ABS32);
}

void BASM_LabelDispRel(BASM_Context *ctx, char *lbl, int disp)
{
	if(!lbl || (lbl && !strcmp(lbl, "$")))
	{
		BASM_OutDWord(ctx, disp);
		return;
	}

	//default: REL reference
	BASM_OutDWord(ctx, disp);
	BASM_EmitGoto(ctx, lbl, BASM_JMP_NEAR32);
}

void BASM_ModRM64(BASM_Context *ctx, int reg,
	int rm, int sc, int idx, int disp, char *lbl)
{
	//RIP-relative address
	if(rm==BASM_REG_X86_RIP)
	{
		BASM_OutByte(ctx, ((reg&7)<<3)|5);
		BASM_OutDWord(ctx, disp);
		BASM_EmitGoto(ctx, lbl, BASM_JMP_NEAR32);
		return;
	}

	//scaled index, using ESP in referencing memory
	//or an absolute offset
	if((idx!=BASM_Z) || ((sc || disp || lbl) && ((rm&7)==4)) ||
		(disp && !lbl && (sc>=0)) || (lbl && (*lbl=='@')))
	{
		if(rm==BASM_Z)
		{
			BASM_OutByte(ctx, ((reg&7)<<3)|4);
			BASM_Sib(ctx, 5, sc, idx);
			BASM_LabelDisp(ctx, lbl, disp);
			return;
		}

		if(disp || lbl || ((rm&7)==5))
		{
			if(!lbl && (disp>=-128) && (disp<127))
			{
				BASM_OutByte(ctx, 0x40|((reg&7)<<3)|4);
				BASM_Sib(ctx, rm, sc, idx);
				BASM_OutByte(ctx, disp);
				return;
			}

			BASM_OutByte(ctx, 0x80|((reg&7)<<3)|4);
			BASM_Sib(ctx, rm, sc, idx);
			BASM_LabelDisp(ctx, lbl, disp);
			return;
		}

		BASM_OutByte(ctx, ((reg&7)<<3)|4);
		BASM_Sib(ctx, rm, sc, idx);
		return;
	}

	//[EBP] (along with [R13]/[RJX]) needs an offset
	if((sc && !disp && !lbl) && ((rm&7)==5))
	{
		BASM_OutByte(ctx, 0x40|((reg&7)<<3)|(rm&7));
		BASM_OutByte(ctx, 0);
		return;
	}


	if(disp || lbl)
	{
		if(rm==BASM_Z)
		{
			BASM_OutByte(ctx, ((reg&7)<<3)|5);
//			BASM_OutDWord(ctx, disp);
//			BASM_EmitGoto(ctx, lbl, BASM_JMP_NEAR32);
			BASM_LabelDispRel(ctx, lbl, disp);
			return;
		}
		if(!lbl && (disp>=-128) && (disp<127))
		{
			BASM_OutByte(ctx, 0x40|((reg&7)<<3)|(rm&7));
			BASM_OutByte(ctx, disp);
			return;
		}

		BASM_OutByte(ctx, 0x80|((reg&7)<<3)|(rm&7));
		BASM_LabelDisp(ctx, lbl, disp);
	}else
	{
		if(sc)	BASM_OutByte(ctx, ((reg&7)<<3)|(rm&7));
			else BASM_OutByte(ctx, 0xC0|((reg&7)<<3)|(rm&7));
		return;
	}
}

BASM_API void BASM_ModRM(BASM_Context *ctx, int reg,
	int rm, int sc, int idx, int disp, char *lbl)
{
	if(((ctx->fl&2) && !(ctx->fl&8)) ||
		(!(ctx->fl&2) && (ctx->fl&8)))
	{
		BASM_ModRM16(ctx, reg, rm, sc, idx, disp, lbl);
		return;
	}

	if(ctx->fl&1)
	{
		BASM_ModRM64(ctx, reg, rm, sc, idx, disp, lbl);
		return;
	}

	//scaled index, using ESP in referencing memory
	//or an absolute offset on x86-64
	if((idx!=BASM_Z) || ((sc || disp || lbl) && ((rm&7)==4)))
	{
		if(rm==BASM_Z)
		{
			BASM_OutByte(ctx, ((reg&7)<<3)|4);
			BASM_Sib(ctx, 5, sc, idx);
			BASM_LabelDisp(ctx, lbl, disp);
			return;
		}

		if(disp || lbl || ((rm&7)==5))
		{
			if(!lbl && (disp>=-128) && (disp<127))
			{
				BASM_OutByte(ctx, 0x40|((reg&7)<<3)|4);
				BASM_Sib(ctx, rm, sc, idx);
				BASM_OutByte(ctx, disp);
				return;
			}

			BASM_OutByte(ctx, 0x80|((reg&7)<<3)|4);
			BASM_Sib(ctx, rm, sc, idx);
			BASM_LabelDisp(ctx, lbl, disp);
			return;
		}

		BASM_OutByte(ctx, ((reg&7)<<3)|4);
		BASM_Sib(ctx, rm, sc, idx);
		return;
	}

	//[EBP] needs an offset
	if((sc && !disp && !lbl) && ((rm&7)==5))
	{
		BASM_OutByte(ctx, 0x40|((reg&7)<<3)|(rm&7));
		BASM_OutByte(ctx, 0);
		return;
	}

	if(disp || lbl)
	{
		if(rm==BASM_Z)
		{
			BASM_OutByte(ctx, ((reg&7)<<3)|5);
			BASM_LabelDisp(ctx, lbl, disp);
			return;
		}
		if(!lbl && (disp>=-128) && (disp<127))
		{
			BASM_OutByte(ctx, 0x40|((reg&7)<<3)|(rm&7));
			BASM_OutByte(ctx, disp);
			return;
		}

		BASM_OutByte(ctx, 0x80|((reg&7)<<3)|(rm&7));
		BASM_LabelDisp(ctx, lbl, disp);
	}else
	{
		if(sc)
		{
			BASM_OutByte(ctx, ((reg&7)<<3)|(rm&7));
		}else
		{
			BASM_OutByte(ctx, 0xC0|((reg&7)<<3)|(rm&7));
		}
		return;
	}
}


#ifndef BASM_HEXVAL_DEF
#define BASM_HEXVAL_DEF
static int hexval(int i)
{
	if((i>='0') && (i<='9'))return(i-'0');
	if((i>='A') && (i<='F'))return(i-'A'+10);
	if((i>='a') && (i<='f'))return(i-'a'+10);
	return(-1);
}

static int hexbyte(char *s)
{
	int i, j;
	i=hexval(s[0]);
	j=hexval(s[1]);
	if((i<0) || (j<0))return(-1);
	return((i<<4)+j);
}
#endif

static int hexword(char *s)
{
	int i, j;
	i=hexbyte(s+0);
	j=hexbyte(s+2);
	if((i<0) || (j<0))return(-1);
	return((i<<8)+j);
}

static int hexdword(char *s)
{
	int i, j;
	i=hexword(s+0);
	j=hexword(s+4);
	if((i<0) || (j<0))return(-1);
	return((i<<16)+j);
}

static int altval(int i)
{
	if((i>='i') && (i<='x'))return(i-'i');
	return(-1);
}

static int altbyte(char *s)
{
	int i, j;
	i=altval(s[0]);
	j=altval(s[1]);
	if((i<0) || (j<0))return(-1);
	return((i<<4)+j);
}

BASM_API int BASM_RegREXW(int reg)
{
	if(reg==BASM_Z)return(0);
	if((reg&255)>=64)return(0);
	return(((reg>>4)&15)==3);
}

BASM_API int BASM_Reg16P(int reg)
{
	if(reg==BASM_Z)return(0);
	if((reg&255)>=64)return(0);
	return(((reg>>4)&15)==1);
}

BASM_API int BASM_Reg32P(int reg)
{
	if(reg==BASM_Z)return(0);
	if((reg&255)>=64)return(0);
	return(((reg>>4)&15)==2);
}


BASM_API void BASM_AddrOverride(BASM_Context *ctx, int breg)
{
//	ctx->fl|=8;

	if(BASM_Reg16P(breg) && !(ctx->fl&2))ctx->fl|=8;
	if(BASM_Reg32P(breg) && (ctx->fl&2))ctx->fl|=8;

	if(ctx->fl&8)BASM_OutByte(ctx, 0x67);
}

BASM_API void BASM_SegOverride(BASM_Context *ctx)
{
	if(ctx->seg==BASM_REG_X86_CS)BASM_OutByte(ctx, 0x2E);
	if(ctx->seg==BASM_REG_X86_SS)BASM_OutByte(ctx, 0x36);
	if(ctx->seg==BASM_REG_X86_DS)BASM_OutByte(ctx, 0x3E);
	if(ctx->seg==BASM_REG_X86_ES)BASM_OutByte(ctx, 0x26);
	if(ctx->seg==BASM_REG_X86_FS)BASM_OutByte(ctx, 0x64);
	if(ctx->seg==BASM_REG_X86_GS)BASM_OutByte(ctx, 0x65);
}


BASM_API char *BASM_OutSufBytes(BASM_Context *ctx, char *s)
{
	int i;

	while(*s)
	{
		i=hexbyte(s);
		if(i<0)break;
		s+=2; while(*s && (*s<=' '))s++;
		BASM_OutByte(ctx, i);
	}

	return(s);
}

BASM_API char *BASM_OutBodyBytes(BASM_Context *ctx, char *s, int rex)
{
	int i, j, k, l;

	while(*s && (*s<=' '))s++;
	while(*s)
	{
		if((*s=='V') || (*s=='W') || (*s=='S') || (*s=='T'))
		{
			if((*s=='V') && !(ctx->fl&2))BASM_OutByte(ctx, 0x67);
			if((*s=='W') && !(ctx->fl&2))BASM_OutByte(ctx, 0x66);
			if((*s=='S') && (ctx->fl&2))BASM_OutByte(ctx, 0x67);
			if((*s=='T') && (ctx->fl&2))BASM_OutByte(ctx, 0x66);
			s++; continue;
		}
		if((*s=='X') && (ctx->fl&1))
		{
			if(rex)BASM_OutByte(ctx, 0x40|(rex&0x0F));
			s++; continue;
		}
//		if(*s=='X') { s++; continue; }

		if((*s=='X') && !(ctx->fl&1))
		{
			if(rex)
			{
				//BGB: PREX, N/E on standard x86
				i=0xD0;
				j=(((rex&7)<<5)|(i>>3))^0xE0;
				k=((rex&8)<<4)|(((rex>>4)&15)<<3)|(i&7);
				BASM_OutByte(ctx, 0x8F);
				BASM_OutByte(ctx, j^0xE0);
				BASM_OutByte(ctx, k^0x78);
			}
			s++; continue;
		}

		if(*s=='H')
		{
			s++;
			i=altval(*s++);
			k=((rex&4)<<5)|(((rex>>4)&15)<<3)|(i&7);
			BASM_OutByte(ctx, 0xC5);
			BASM_OutByte(ctx, k^0xF8);
			continue;
		}
		if(*s=='I')
		{
			s++;
			i=altbyte(s); s+=2;
			j=((rex&7)<<5)|(i>>3);
			k=((rex&8)<<4)|(((rex>>4)&15)<<3)|(i&7);
			BASM_OutByte(ctx, 0xC4);
			BASM_OutByte(ctx, j^0xE0);
			BASM_OutByte(ctx, k^0x78);
			continue;
		}
		if(*s=='J')
		{
			s++;
			i=altbyte(s); s+=2;
			j=((rex&7)<<5)|(i>>3);
			k=((rex&8)<<4)|(((rex>>4)&15)<<3)|(i&7);
			BASM_OutByte(ctx, 0x8F);
			BASM_OutByte(ctx, j^0xE0);
			BASM_OutByte(ctx, k^0x78);
			continue;
		}

		if((*s=='K') && rex)
		{
			s++;
			i=altbyte(s); s+=2;
			j=((rex&7)<<5)|(i>>3);
			k=((rex&8)<<4)|(((rex>>4)&15)<<3)|(i&7);
			BASM_OutByte(ctx, 0xC4);
			BASM_OutByte(ctx, j^0xE0);
			BASM_OutByte(ctx, k^0x78);
			continue;
		}
		if((*s=='L') && rex)
		{
			s++;
			i=altbyte(s); s+=2;
			j=(((rex&7)<<5)|(i>>3))^0xE0;
			k=((rex&8)<<4)|(((rex>>4)&15)<<3)|(i&7);
			BASM_OutByte(ctx, 0x8F);
			BASM_OutByte(ctx, j^0xE0);
			BASM_OutByte(ctx, k^0x78);
			continue;
		}

		if(*s=='K') { s+=3; continue; }
		if(*s=='L') { s+=3; continue; }

		if((*s=='G') && (s[1]=='t'))	//Thumb
		{
			s+=2;
			i=hexbyte(s);
			if(i<0)break;
			s+=2;
			j=hexbyte(s);
			if(j<0)break;
			s+=2;
			BASM_OutByte(ctx, j);
			BASM_OutByte(ctx, i);
			continue;
		}

		if(*s=='G')	//ARM
		{
			s+=2;
			i=hexdword(s);
			if(i<0)break;
			s+=8;
			i|=(ctx->fl<<8)&0xF0100000;
			BASM_OutDWord(ctx, i);
#if 0
			i=hexbyte(s);
			if(i<0)break;
			s+=2;
			j=hexbyte(s);
			if(j<0)break;
			s+=2;
			k=hexbyte(s);
			if(k<0)break;
			s+=2;
			l=hexbyte(s);
			if(l<0)break;
			s+=2;
			BASM_OutByte(ctx, l);
			BASM_OutByte(ctx, k);
			BASM_OutByte(ctx, j);
			BASM_OutByte(ctx, i);
#endif
			continue;
		}

		i=hexbyte(s);
		if(i<0)break;
		s+=2; while(*s && (*s<=' '))s++;
		BASM_OutByte(ctx, i);
	}

	return(s);
}

BASM_API char *BASM_OutImm(BASM_Context *ctx, char *s,
	long long imm, char *lbl)
{
	if(*s==',')
	{
		if(!strncmp(s, ",ib", 3))
		{
			BASM_OutByte(ctx, imm);
			s+=3;
		}else if(!strncmp(s, ",iw", 3))
		{
			BASM_OutWord(ctx, imm);
			s+=3;
		}else if(!strncmp(s, ",id", 3))
		{
			BASM_OutDWord(ctx, imm);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_ABS32);
			s+=3;
		}else if(!strncmp(s, ",iq", 3))
		{
			BASM_OutQWord(ctx, imm);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_ABS64);
			s+=3;
		}else if(!strncmp(s, ",rb", 3))
		{
			BASM_OutByte(ctx, imm);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_SHORT);
			s+=3;
		}else if(!strncmp(s, ",rw", 3))
		{
			BASM_OutWord(ctx, imm);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_NEAR16);
			s+=3;
		}else if(!strncmp(s, ",rd", 3))
		{
			BASM_OutDWord(ctx, imm);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_NEAR32);
			s+=3;
		}
	}

	if(*s=='|')
	{
		if(!strncmp(s, "|ia", 3))
		{
			*(ctx->ip-4)|=(imm&255);
			*(ctx->ip-3)|=((imm>>8)&255);
			*(ctx->ip-2)|=((imm>>16)&255);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_ARM_NEAR24);
			s+=3;
		}else if(!strncmp(s, "|ib", 3))
		{
			*(ctx->ip-4)|=(imm&255);
			s+=3;
		}else if(!strncmp(s, "|ic", 3))
		{
			*(ctx->ip-4)|=(imm&255);
			*(ctx->ip-3)|=((imm>>8)&255);
			*(ctx->ip-2)|=((imm>>16)&255);
			*(ctx->ip-1)|=((imm>>24)&255);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_ABS32);
			s+=3;
		}else if(!strncmp(s, "|id", 3))
		{
			*(ctx->ip-4)|=(imm&255);
			*(ctx->ip-3)|=((imm>>8)&255);
			*(ctx->ip-2)|=((imm>>16)&255);
			*(ctx->ip-1)|=((imm>>24)&255);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_ABS32);
			s+=3;
		}else if(!strncmp(s, "|io", 3))
		{
			*(ctx->ip-4)|=(imm&255);
			*(ctx->ip-3)|=((imm>>8)&15);
			s+=3;
		}else if(!strncmp(s, "|is", 3))
		{
			*(ctx->ip-3)|=(imm&15);
			s+=3;
		}else if(!strncmp(s, "|irb", 3))
		{
			*(ctx->ip-4)|=(imm&255);
			*(ctx->ip-3)|=((imm>>8)&15);
			s+=3;
		}else if(!strncmp(s, "|ja", 3))
		{
			*(ctx->ip-2)|=(imm&255);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_ARM_NEAR24);
			s+=3;
		}else if(!strncmp(s, "|jb", 3))
		{
			*(ctx->ip-2)|=(imm&255);
			s+=3;
		}else if(!strncmp(s, "|jc", 3))
		{
			*(ctx->ip-2)|=(imm&127);
			s+=3;
		}else if(!strncmp(s, "|jd", 3))
		{
			*(ctx->ip-2)|=(imm&255);
			*(ctx->ip-1)|=((imm>>8)&7);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_THUMB_NEAR11);
			s+=3;
		}else if(!strncmp(s, "|je", 3))
		{
			*(ctx->ip-2)|=((imm>>11)&255);
			*(ctx->ip-1)|=((imm>>19)&7);
			s+=3;
		}else if(!strncmp(s, "|jf", 3))
		{
			*(ctx->ip-2)|=(imm&255);
			*(ctx->ip-1)|=((imm>>8)&7);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_THUMB_NEAR22);
			s+=3;
		}else if(!strncmp(s, "|ji", 3))
		{
			*(ctx->ip-2)|=(imm&3)<<6;
			*(ctx->ip-1)|=((imm>>2)&1);
			s+=3;
		}else if(!strncmp(s, "|jj", 3))
		{
			*(ctx->ip-2)|=(imm&3)<<6;
			*(ctx->ip-1)|=((imm>>2)&7);
			s+=3;
		}else if(!strncmp(s, "|jk", 3))
		{
			*(ctx->ip-2)|=(imm&7);
			s+=3;
		}else if(!strncmp(s, "|jl", 3))
		{
			*(ctx->ip-2)|=(imm&255);
			*(ctx->ip-1)|=((imm>>8)&7)<<4;
			*(ctx->ip-4)|=((imm>>11)&15);
			*(ctx->ip-3)|=((imm>>15)&1)<<2;
			s+=3;
		}
	}

	return(s);
}

BASM_API char *BASM_OutMOffs(BASM_Context *ctx,
	char *s, int disp, char *lbl)
{
	if(*s==',')
	{
		if(!strncmp(s, ",mw", 3))
		{
			BASM_OutWord(ctx, disp);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_ABS16);
			s+=3;
		}else if(!strncmp(s, ",md", 3))
		{
			BASM_OutDWord(ctx, disp);
			BASM_EmitGoto(ctx, lbl, BASM_JMP_ABS32);
			s+=3;
		}
	}

	return(s);
}

BASM_API char *BASM_OutModRM(BASM_Context *ctx, char *s, int reg,
	int breg, int sc, int ireg, int disp, char *lbl)
{
	int i;

	if(ctx->fl&BASM_FL_ARM)
		return(s);

	if(*s=='|')
	{
		s+=2;
		*(ctx->ip-1)|=(reg&7);
	}

	if((s[0]=='/') && (s[1]=='r'))
	{
//		if(!sc)sc=1;
		BASM_ModRM(ctx, reg, breg, sc, ireg, disp, lbl);
		s+=2;
	}

	if(*s=='/')
	{
		s++;
		i=(*s++)-'0';
		BASM_ModRM(ctx, BASM_REG_X86_OP0+i, breg, sc, ireg, disp, lbl);
	}

	return(s);
}

BASM_API char *BASM_OutArmReg(BASM_Context *ctx, char *s, int reg)
{
	int i;

	if(!(ctx->fl&BASM_FL_ARM))
		return(s);

	if(*s=='|')
	{
		if(!strncmp(s, "|ra", 3))
		{
			*(ctx->ip-4)|=(reg&15);
			s+=3;
		}else if(!strncmp(s, "|rb", 3))
		{
			*(ctx->ip-4)|=(reg&15)<<4;
			s+=3;
		}else if(!strncmp(s, "|rc", 3))
		{
			*(ctx->ip-3)|=(reg&15);
			s+=3;
		}else if(!strncmp(s, "|rd", 3))
		{
			*(ctx->ip-3)|=(reg&15)<<4;
			s+=3;
		}else if(!strncmp(s, "|re", 3))
		{
			*(ctx->ip-2)|=(reg&15);
			s+=3;
		}else if(!strncmp(s, "|rf", 3))
		{
			*(ctx->ip-2)|=(reg&15)<<4;
			s+=3;
		}else if(!strncmp(s, "|rg", 3))
		{
			*(ctx->ip-2)|=(reg&7);
			s+=3;
		}else if(!strncmp(s, "|rh", 3))
		{
			*(ctx->ip-2)|=(reg&7)<<3;
			s+=3;
		}else if(!strncmp(s, "|ri", 3))
		{
			*(ctx->ip-2)|=(reg&3)<<6;
			*(ctx->ip-1)|=((reg>>2)&1);
			s+=3;
		}else if(!strncmp(s, "|rj", 3))
		{
			*(ctx->ip-2)|=(reg&7)<<1;
			s+=3;
		}else if(!strncmp(s, "|rk", 3))
		{
			*(ctx->ip-2)|=(reg&15)<<3;
			s+=3;
		}else if(!strncmp(s, "|rl", 3))
		{
			*(ctx->ip-2)|=(reg&7);
			*(ctx->ip-1)|=((reg>>3)&1)<<7;
			s+=3;
		}else if(!strncmp(s, "|rz", 3))
		{
			s+=3;
		}
	}
	return(s);
}

BASM_API int BASM_SizeOpStr(BASM_Context *ctx, char *str)
{
	char *s;
	int sz;
	int i;
	
	s=str; sz=0;
	while(*s)
	{
		if(*s=='|')
		{
			if(s[1]=='r')
				{ s+=2; continue; }
			break;
		}

		if(*s=='/')
		{
			if(s[1]=='r')
			{
				sz+=6;	//worst case
				s+=2;
				continue;
			}
			break;
		}

		if(*s==',')
		{
			if((s[1]=='i') || (s[1]=='r') || (s[1]=='m'))
			{
				if(s[2]=='b')
					{ s+=3; sz+=1; continue; }
				if(s[2]=='w')
					{ s+=3; sz+=2; continue; }
				if(s[2]=='d')
					{ s+=3; sz+=4; continue; }
				if(s[2]=='q')
					{ s+=3; sz+=8; continue; }
			}
			break;
		}

		if(*s=='X')
		{
			if(ctx->fl&BASM_FL_X86_64)
				sz++;
			s++;
			continue;
		}

		if((*s=='V') || (*s=='W'))
		{
			if(!(ctx->fl&BASM_FL_X86_16))
				sz++;
			s++;
			continue;
		}

		if((*s=='S') || (*s=='T'))
		{
			if(ctx->fl&BASM_FL_X86_16)
				sz++;
			s++;
			continue;
		}

		i=hexbyte(s);
		if(i<0)break;
		sz++; s+=2; while(*s && (*s<=' '))s++;
	}
	
	return(sz);
}

BASM_API void BASM_BundleAlignOpStr(BASM_Context *ctx, char *str)
{
	int sz;
	int j, k;

	if(!(ctx->fl&BASM_FL_BUNDLE))
		return;
	if(ctx->fl&BASM_FL_DATA)
		return;
		
	sz=BASM_SizeOpStr(ctx, str);

	j=ctx->ip-ctx->text; k=j+sz;
	if((j&(~31))!=(k&(~31)))
	{
		if(j%32)BASM_OutPadText(ctx, 32-(j%32));
	}
}

BASM_API void BASM_OutOpStr(BASM_Context *ctx, char *s)
{
	BASM_BundleAlignOpStr(ctx, s);
	BASM_AddrOverride(ctx, BASM_Z);
	s=BASM_OutBodyBytes(ctx, s, 0);
}

BASM_API void BASM_OutOpStrReg(BASM_Context *ctx, char *s, int reg)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_AddrOverride(ctx, BASM_Z);

	i=(BASM_RegREXW(reg)?8:0)|((reg&8)?4:0)|((reg&8)?1:0);
	if(reg&256)i|=256;
	s=BASM_OutBodyBytes(ctx, s, i);

	s=BASM_OutModRM(ctx, s, reg, reg, 0, BASM_Z, 0, NULL);
	s=BASM_OutArmReg(ctx, s, reg);
	s=BASM_OutSufBytes(ctx, s);

	while(*s==';')
	{
		s++;
		s=BASM_OutBodyBytes(ctx, s, 0);
		s=BASM_OutArmReg(ctx, s, reg);
	}
}

BASM_API void BASM_OutOpStrImm(BASM_Context *ctx,
	char *s, long long imm, char *lbl)
{
	BASM_BundleAlignOpStr(ctx, s);
	BASM_AddrOverride(ctx, BASM_Z);
	s=BASM_OutBodyBytes(ctx, s, 0);
	s=BASM_OutImm(ctx, s, imm, lbl);
	s=BASM_OutSufBytes(ctx, s);
	
	while(*s==';')
	{
		s++;
		s=BASM_OutBodyBytes(ctx, s, 0);
		s=BASM_OutImm(ctx, s, imm, lbl);
	}
}

BASM_API void BASM_OutOpStrMem(BASM_Context *ctx, char *s,
	char *lbl, int breg, int ireg, int sc, int disp)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_SegOverride(ctx);
	BASM_AddrOverride(ctx, breg);

	i=((breg!=BASM_Z) && (breg&8))?1:0;
	i|=((ireg!=BASM_Z) && (ireg&8))?2:0;
	s=BASM_OutBodyBytes(ctx, s, i);

	s=BASM_OutModRM(ctx, s, 0, breg, sc, ireg, disp, lbl);
	s=BASM_OutMOffs(ctx, s, disp, lbl);
	s=BASM_OutSufBytes(ctx, s);
}

BASM_API void BASM_OutOpStrRegReg(BASM_Context *ctx,
	char *s, int r0, int r1)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_AddrOverride(ctx, BASM_Z);

	i=(BASM_RegREXW(r0)?8:0)|((r0&8)?4:0)|((r1&8)?1:0);
	if((r0&256) || (r1&256))i|=256;
	s=BASM_OutBodyBytes(ctx, s, i);
	s=BASM_OutModRM(ctx, s, r0, r1, 0, BASM_Z, 0, NULL);
	s=BASM_OutArmReg(ctx, s, r0);
	s=BASM_OutArmReg(ctx, s, r1);
	s=BASM_OutSufBytes(ctx, s);

	while(*s==';')
	{
		s++;
		s=BASM_OutBodyBytes(ctx, s, 0);
		s=BASM_OutArmReg(ctx, s, r0);
		s=BASM_OutArmReg(ctx, s, r1);
	}
}

BASM_API void BASM_OutOpStrRegImm(BASM_Context *ctx, char *s, int reg,
	long long imm, char *lbl)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_AddrOverride(ctx, BASM_Z);

	i=(BASM_RegREXW(reg)?8:0)|((reg&8)?4:0)|((reg&8)?1:0);
	if(reg&256)i|=256;
	s=BASM_OutBodyBytes(ctx, s, i);

	s=BASM_OutModRM(ctx, s, reg, reg, 0, BASM_Z, 0, NULL);
	s=BASM_OutArmReg(ctx, s, reg);
	s=BASM_OutImm(ctx, s, imm, lbl);
	s=BASM_OutSufBytes(ctx, s);

	while(*s==';')
	{
		s++;
		s=BASM_OutBodyBytes(ctx, s, 0);
		s=BASM_OutArmReg(ctx, s, reg);
		s=BASM_OutImm(ctx, s, imm, lbl);
	}
}

BASM_API void BASM_OutOpStrRegMem(BASM_Context *ctx, char *s, int reg,
	char *lbl, int breg, int ireg, int sc, int disp)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_SegOverride(ctx);
	BASM_AddrOverride(ctx, breg);

	i=(BASM_RegREXW(reg)?8:0)|((reg&8)?4:0);
	i|=((breg!=BASM_Z) && (breg&8))?1:0;
	i|=((ireg!=BASM_Z) && (ireg&8))?2:0;
	if(reg&256)i|=256;
	s=BASM_OutBodyBytes(ctx, s, i);

	s=BASM_OutModRM(ctx, s, reg, breg, sc, ireg, disp, lbl);
	s=BASM_OutMOffs(ctx, s, disp, lbl);
	s=BASM_OutSufBytes(ctx, s);
}

BASM_API void BASM_OutOpStrMemImm(BASM_Context *ctx, char *s, int w,
	char *lbl, int breg, int ireg, int sc, int disp,
	long long imm, char *lbl2)
{
	char *t;
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_SegOverride(ctx);
	BASM_AddrOverride(ctx, breg);

	if(breg==BASM_REG_X86_RIP)
	{
		t=s;
		while(*t && (*t!=','))t++;
		if(*t==',')
		{
			t++;
			if(!strncmp(t, "ib", 2))disp--;
			if(!strncmp(t, "iw", 2))disp-=2;
			if(!strncmp(t, "id", 2))disp-=4;
			if(!strncmp(t, "iq", 2))disp-=8;
			if(!strncmp(t, "rb", 2))disp--;
			if(!strncmp(t, "rw", 2))disp-=2;
			if(!strncmp(t, "rd", 2))disp-=4;
		}
	}

	i=(w==64)?8:0;
	i|=((breg!=BASM_Z) && (breg&8))?1:0;
	i|=((ireg!=BASM_Z) && (ireg&8))?2:0;
	s=BASM_OutBodyBytes(ctx, s, i);

	s=BASM_OutModRM(ctx, s, 0, breg, sc, ireg, disp, lbl);
	s=BASM_OutMOffs(ctx, s, disp, lbl);
	s=BASM_OutImm(ctx, s, imm, lbl2);
	s=BASM_OutSufBytes(ctx, s);
}


BASM_API void BASM_OutOpStrRegRegImm(BASM_Context *ctx, char *s,
	int r0, int r1, long long imm, char *lbl)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_AddrOverride(ctx, BASM_Z);

	i=(BASM_RegREXW(r0)?8:0)|((r0&8)?4:0)|((r1&8)?1:0);
	if((r0&256) || (r1&256))i|=256;
	s=BASM_OutBodyBytes(ctx, s, i);

	s=BASM_OutModRM(ctx, s, r0, r1, 0, BASM_Z, 0, NULL);
	s=BASM_OutArmReg(ctx, s, r0);
	s=BASM_OutArmReg(ctx, s, r1);
	s=BASM_OutImm(ctx, s, imm, lbl);
	s=BASM_OutSufBytes(ctx, s);

	while(*s==';')
	{
		s++;
		s=BASM_OutBodyBytes(ctx, s, 0);
		s=BASM_OutArmReg(ctx, s, r0);
		s=BASM_OutArmReg(ctx, s, r1);
		s=BASM_OutImm(ctx, s, imm, lbl);
	}
}

BASM_API void BASM_OutOpStrRegMemImm(BASM_Context *ctx, char *s, int reg,
	char *lbl, int breg, int ireg, int sc, int disp,
	long long imm, char *lbl2)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_SegOverride(ctx);
	BASM_AddrOverride(ctx, breg);

	i=(BASM_RegREXW(reg)?8:0)|((reg&8)?4:0);
	i|=((breg!=BASM_Z) && (breg&8))?1:0;
	i|=((ireg!=BASM_Z) && (ireg&8))?2:0;
	if(reg&256)i|=256;
	s=BASM_OutBodyBytes(ctx, s, i);

	s=BASM_OutModRM(ctx, s, reg, breg, sc, ireg, disp, lbl);
	s=BASM_OutMOffs(ctx, s, disp, lbl);
	s=BASM_OutImm(ctx, s, imm, lbl2);
	s=BASM_OutSufBytes(ctx, s);
}


BASM_API void BASM_OutOpStrRegRegReg(BASM_Context *ctx, char *s,
	int r0, int r1, int r2)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_AddrOverride(ctx, BASM_Z);

	i=(BASM_RegREXW(r0)?8:0)|((r0&8)?4:0)|((r1&8)?1:0);
	if((r0&256) || (r1&256))i|=256;
	i|=(r2&15)<<4;
	s=BASM_OutBodyBytes(ctx, s, i);
	s=BASM_OutModRM(ctx, s, r0, r1, 0, BASM_Z, 0, NULL);
	s=BASM_OutArmReg(ctx, s, r0);
	s=BASM_OutArmReg(ctx, s, r1);
	s=BASM_OutArmReg(ctx, s, r2);
	s=BASM_OutSufBytes(ctx, s);

	while(*s==';')
	{
		s++;
		s=BASM_OutBodyBytes(ctx, s, 0);
		s=BASM_OutArmReg(ctx, s, r0);
		s=BASM_OutArmReg(ctx, s, r1);
		s=BASM_OutArmReg(ctx, s, r2);
	}
}

BASM_API void BASM_OutOpStrRegMemReg(BASM_Context *ctx, char *s, int reg,
	char *lbl, int breg, int ireg, int sc, int disp, int reg2)
{
	int i;

	BASM_BundleAlignOpStr(ctx, s);
	BASM_SegOverride(ctx);
	BASM_AddrOverride(ctx, breg);

	i=(BASM_RegREXW(reg)?8:0)|((reg&8)?4:0);
	i|=((breg!=BASM_Z) && (breg&8))?1:0;
	i|=((ireg!=BASM_Z) && (ireg&8))?2:0;
	if(reg&256)i|=256;
	i|=(reg2&15)<<4;
	s=BASM_OutBodyBytes(ctx, s, i);

	s=BASM_OutModRM(ctx, s, reg, breg, sc, ireg, disp, lbl);
	s=BASM_OutMOffs(ctx, s, disp, lbl);
	s=BASM_OutSufBytes(ctx, s);
}


BASM_API int BASM_OpSingleP(int i)
{
	if(basm_opcat[i*16+0])return(0);
	if(basm_opcat[i*16+1])return(0);
	if(basm_opcat[i*16+2])return(0);
	return(1);
}

BASM_API int BASM_GetRegWidth(int reg)
{
	int w;

	w=0;

	if((reg&255)<64)
	{
		if(((reg>>4)&15)==0)w=8;
		if(((reg>>4)&15)==1)w=16;
		if(((reg>>4)&15)==2)w=32;
		if(((reg>>4)&15)==3)w=64;
		return(w);
	}

	if(	(reg==BASM_REG_X86_CS) || (reg!=BASM_REG_X86_SS) ||
		(reg==BASM_REG_X86_DS) || (reg!=BASM_REG_X86_ES) ||
		(reg==BASM_REG_X86_FS) || (reg!=BASM_REG_X86_GS))return(16);

	return(0);
}

#if 0
int BASM_OpArgRegP(int i, int j, int reg)
{
	int w;


	if((basm_opcat[i*16+j]==OPCAT_AL) && (reg==BASM_REG_X86_AL))return(1);
	if((basm_opcat[i*16+j]==OPCAT_CL) && (reg==BASM_REG_X86_CL))return(1);
	if((basm_opcat[i*16+j]==OPCAT_DL) && (reg==BASM_REG_X86_DL))return(1);
	if((basm_opcat[i*16+j]==OPCAT_BL) && (reg==BASM_REG_X86_BL))return(1);

	if((basm_opcat[i*16+j]==OPCAT_AX) && (reg==BASM_REG_X86_AX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_CX) && (reg==BASM_REG_X86_CX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_DX) && (reg==BASM_REG_X86_DX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_BX) && (reg==BASM_REG_X86_BX))return(1);

	if((basm_opcat[i*16+j]==OPCAT_EAX) && (reg==BASM_REG_X86_EAX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_ECX) && (reg==BASM_REG_X86_ECX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_EDX) && (reg==BASM_REG_X86_EDX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_EBX) && (reg==BASM_REG_X86_EBX))return(1);

	if((basm_opcat[i*16+j]==OPCAT_RAX) && (reg==BASM_REG_X86_RAX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_RCX) && (reg==BASM_REG_X86_RCX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_RDX) && (reg==BASM_REG_X86_RDX))return(1);
	if((basm_opcat[i*16+j]==OPCAT_RBX) && (reg==BASM_REG_X86_RBX))return(1);


	if((basm_opcat[i*16+j]==OPCAT_SR) &&
		((reg==BASM_REG_X86_CS) || (reg==BASM_REG_X86_SS) ||
		 (reg==BASM_REG_X86_DS) || (reg==BASM_REG_X86_ES) ||
		 (reg==BASM_REG_X86_FS) || (reg==BASM_REG_X86_GS)))
			return(1);

	if((basm_opcat[i*16+j]==OPCAT_CS) && (reg==BASM_REG_X86_CS))return(1);
	if((basm_opcat[i*16+j]==OPCAT_SS) && (reg==BASM_REG_X86_SS))return(1);
	if((basm_opcat[i*16+j]==OPCAT_DS) && (reg==BASM_REG_X86_DS))return(1);
	if((basm_opcat[i*16+j]==OPCAT_ES) && (reg==BASM_REG_X86_ES))return(1);
	if((basm_opcat[i*16+j]==OPCAT_FS) && (reg==BASM_REG_X86_FS))return(1);
	if((basm_opcat[i*16+j]==OPCAT_GS) && (reg==BASM_REG_X86_GS))return(1);

	if((basm_opcat[i*16+j]>=OPCAT_MM0) &&
		(basm_opcat[i*16+j]<=OPCAT_MM7))
	{
		if((reg<BASM_REG_X86_MM0) || (reg>BASM_REG_X86_MM7))
			return(0);
		if((basm_opcat[i*16+j]-OPCAT_MM0)==(reg-BASM_REG_X86_MM0))
			return(1);
		return(0);
	}

	if((basm_opcat[i*16+j]==OPCAT_CR) &&
		((reg>=BASM_REG_X86_CR0)&&(reg<=BASM_REG_X86_CR15)))
			return(1);
	if((basm_opcat[i*16+j]==OPCAT_DR) &&
		((reg>=BASM_REG_X86_DR0)&&(reg<=BASM_REG_X86_DR15)))
			return(1);
	if((basm_opcat[i*16+j]==OPCAT_TR) &&
		((reg>=BASM_REG_X86_TR0)&&(reg<=BASM_REG_X86_TR15)))
			return(1);

	if((basm_opcat[i*16+j]==OPCAT_FREG) &&
		((reg>=BASM_REG_X86_MM0)&&(reg<=BASM_REG_X86_MM7)))
			return(1);
	if((basm_opcat[i*16+j]==OPCAT_FRM) &&
		((reg>=BASM_REG_X86_MM0)&&(reg<=BASM_REG_X86_MM7)))
			return(1);

	if((basm_opcat[i*16+j]==OPCAT_XREG) &&
		((reg>=BASM_REG_X86_XMM0)&&(reg<=BASM_REG_X86_XMM15)))
			return(1);
	if((basm_opcat[i*16+j]==OPCAT_XRM) &&
		((reg>=BASM_REG_X86_XMM0)&&(reg<=BASM_REG_X86_XMM15)))
			return(1);


	w=0;
	if((reg&255)<64)
	{
		if(((reg>>4)&15)==0)w=8;
		if(((reg>>4)&15)==1)w=16;
		if(((reg>>4)&15)==2)w=32;
		if(((reg>>4)&15)==3)w=64;
	}

	if((basm_opcat[i*16+j]==OPCAT_REG8 ) && (w==8 ))return(1);
	if((basm_opcat[i*16+j]==OPCAT_REG16) && (w==16))return(1);
	if((basm_opcat[i*16+j]==OPCAT_REG32) && (w==32))return(1);
	if((basm_opcat[i*16+j]==OPCAT_REG64) && (w==64))return(1);

	if((basm_opcat[i*16+j]==OPCAT_RM8 ) && (w==8 ))return(1);
	if((basm_opcat[i*16+j]==OPCAT_RM16) && (w==16))return(1);
	if((basm_opcat[i*16+j]==OPCAT_RM32) && (w==32))return(1);
	if((basm_opcat[i*16+j]==OPCAT_RM64) && (w==64))return(1);


	return(0);
}
#endif

#if 1
int BASM_OpArgRegP(int i, int j, int reg)
{
	int v, w, ret;

	v=basm_opcat[i*16+j];

	if( (basm_cpu==BASM_CPU_ARM) ||
		(basm_cpu==BASM_CPU_THUMB))
	{
		switch(v)
		{
			case OPCAT_CS: ret=(reg==BASM_REG_ARM_PC); break;
			case OPCAT_SS: ret=(reg==BASM_REG_ARM_SP); break;
			default:
				ret=0; break;
		}
		if(ret)return(ret);
	}

	ret=0;
	switch(v)
	{
	case OPCAT_AL: ret=(reg==BASM_REG_X86_AL); break;
	case OPCAT_CL: ret=(reg==BASM_REG_X86_CL); break;
	case OPCAT_DL: ret=(reg==BASM_REG_X86_DL); break;
	case OPCAT_BL: ret=(reg==BASM_REG_X86_BL); break;

	case OPCAT_AX: ret=(reg==BASM_REG_X86_AX); break;
	case OPCAT_CX: ret=(reg==BASM_REG_X86_CX); break;
	case OPCAT_DX: ret=(reg==BASM_REG_X86_DX); break;
	case OPCAT_BX: ret=(reg==BASM_REG_X86_BX); break;

	case OPCAT_EAX: ret=(reg==BASM_REG_X86_EAX); break;
	case OPCAT_ECX: ret=(reg==BASM_REG_X86_ECX); break;
	case OPCAT_EDX: ret=(reg==BASM_REG_X86_EDX); break;
	case OPCAT_EBX: ret=(reg==BASM_REG_X86_EBX); break;

	case OPCAT_RAX: ret=(reg==BASM_REG_X86_RAX); break;
	case OPCAT_RCX: ret=(reg==BASM_REG_X86_RCX); break;
	case OPCAT_RDX: ret=(reg==BASM_REG_X86_RDX); break;
	case OPCAT_RBX: ret=(reg==BASM_REG_X86_RBX); break;

	case OPCAT_SR:
		if(	((reg==BASM_REG_X86_CS) || (reg==BASM_REG_X86_SS) ||
			 (reg==BASM_REG_X86_DS) || (reg==BASM_REG_X86_ES) ||
			 (reg==BASM_REG_X86_FS) || (reg==BASM_REG_X86_GS)))
				ret=1;
		break;

	case OPCAT_CS: ret=(reg==BASM_REG_X86_CS); break;
	case OPCAT_SS: ret=(reg==BASM_REG_X86_SS); break;
	case OPCAT_DS: ret=(reg==BASM_REG_X86_DS); break;
	case OPCAT_ES: ret=(reg==BASM_REG_X86_ES); break;
	case OPCAT_FS: ret=(reg==BASM_REG_X86_FS); break;
	case OPCAT_GS: ret=(reg==BASM_REG_X86_GS); break;

	case OPCAT_MM0: ret=(reg==BASM_REG_X86_MM0); break;
	case OPCAT_MM1: ret=(reg==BASM_REG_X86_MM1); break;
	case OPCAT_MM2: ret=(reg==BASM_REG_X86_MM2); break;
	case OPCAT_MM3: ret=(reg==BASM_REG_X86_MM3); break;
	case OPCAT_MM4: ret=(reg==BASM_REG_X86_MM4); break;
	case OPCAT_MM5: ret=(reg==BASM_REG_X86_MM5); break;
	case OPCAT_MM6: ret=(reg==BASM_REG_X86_MM6); break;
	case OPCAT_MM7: ret=(reg==BASM_REG_X86_MM7); break;

	case OPCAT_CR:
		if((reg>=BASM_REG_X86_CR0) && (reg<=BASM_REG_X86_CR15))ret=1;
		break;
	case OPCAT_DR:
		if((reg>=BASM_REG_X86_DR0) && (reg<=BASM_REG_X86_DR15))ret=1;
		break;
	case OPCAT_TR:
		if((reg>=BASM_REG_X86_TR0) && (reg<=BASM_REG_X86_TR15))ret=1;
		break;

	case OPCAT_FREG:
	case OPCAT_FRM:
		if((reg>=BASM_REG_X86_MM0) && (reg<=BASM_REG_X86_MM7))ret=1;
		if(ret) { *(int *)-1=-1; }
		break;
	case OPCAT_XREG:
	case OPCAT_XRM:
		if((reg>=BASM_REG_X86_XMM0) && (reg<=BASM_REG_X86_XMM15))ret=1;
		break;

	case OPCAT_REG8: ret=(((reg>>4)&15)==0); break;
	case OPCAT_REG16: ret=(((reg>>4)&15)==1); break;
	case OPCAT_REG32: ret=(((reg>>4)&15)==2); break;
	case OPCAT_REG64: ret=(((reg>>4)&15)==3); break;

	case OPCAT_RM8: ret=(((reg>>4)&15)==0); break;
	case OPCAT_RM16: ret=(((reg>>4)&15)==1); break;
	case OPCAT_RM32: ret=(((reg>>4)&15)==2); break;
	case OPCAT_RM64: ret=(((reg>>4)&15)==3); break;

	default: ret=0; break;
	}

	return(ret);

//	if(ret)return(1);

#if 0
	w=0;
	if((reg&255)<64)
	{
		if(((reg>>4)&15)==0)w=8;
		if(((reg>>4)&15)==1)w=16;
		if(((reg>>4)&15)==2)w=32;
		if(((reg>>4)&15)==3)w=64;
	}
#endif

#if 0
	switch((reg>>4)&15)
	{
	case 0: w=8; break;
	case 1: w=16; break;
	case 2: w=32; break;
	case 3: w=64; break;
	default: w=0; break;
	}

	switch(v)
	{
	case OPCAT_REG8: ret=(w==8); break;
	case OPCAT_REG16: ret=(w==16); break;
	case OPCAT_REG32: ret=(w==32); break;
	case OPCAT_REG64: ret=(w==64); break;

	case OPCAT_RM8: ret=(w==8); break;
	case OPCAT_RM16: ret=(w==16); break;
	case OPCAT_RM32: ret=(w==32); break;
	case OPCAT_RM64: ret=(w==64); break;
	}
#endif

//	return(ret);
}
#endif

#if 1
int BASM_OpArgMemP(int i, int j, int w, int fl)
{
	int v, ret;

	v=basm_opcat[i*16+j];

	ret=0;
	switch(v)
	{
	case OPCAT_RM8:
		ret=!(fl&4);
		if(w && (w!=8) && (w!=80))ret=0;
		break;

	case OPCAT_RM16:
		ret=!(fl&4) && !(w&&(w!=16));
		break;
	case OPCAT_RM32:
		ret=!(fl&4) && !(w&&(w!=32));
		break;
	case OPCAT_RM64:
		ret=!(fl&4) && !(w&&(w!=32));
		break;

	case OPCAT_MEM: ret=!(fl&4); break;
	case OPCAT_FRM: ret=!(fl&4); break;
	case OPCAT_XRM: ret=!(fl&4); break;

	case OPCAT_MOFFS16:
		ret=!(fl&7);
		break;

	case OPCAT_MOFFS32:
		ret=!(fl&5);
		break;

	case OPCAT_MOFFS64:
		ret=!(fl&1);
		break;


	default: ret=0; break;
	}
	return(ret);
}
#endif

#if 0
int BASM_OpArgMemP(int i, int j, int w, int fl)
{
	if(	(basm_opcat[i*16+j]!=OPCAT_RM8 ) &&
		(basm_opcat[i*16+j]!=OPCAT_RM16) &&
		(basm_opcat[i*16+j]!=OPCAT_RM32) &&
		(basm_opcat[i*16+j]!=OPCAT_RM64) &&
		(basm_opcat[i*16+j]!=OPCAT_MEM) &&
		(basm_opcat[i*16+j]!=OPCAT_FRM) &&
		(basm_opcat[i*16+j]!=OPCAT_XRM) &&
		(basm_opcat[i*16+j]!=OPCAT_MOFFS16) &&
		(basm_opcat[i*16+j]!=OPCAT_MOFFS32) &&
		(basm_opcat[i*16+j]!=OPCAT_MOFFS64))return(0);

	if((fl&1) && (basm_opcat[i*16+j]==OPCAT_MOFFS16))return(0);
	if((fl&1) && (basm_opcat[i*16+j]==OPCAT_MOFFS32))return(0);
	if((fl&1) && (basm_opcat[i*16+j]==OPCAT_MOFFS64))return(0);
	if((fl&2) && (basm_opcat[i*16+j]==OPCAT_MOFFS16))return(0);
	if((fl&4) && (basm_opcat[i*16+j]==OPCAT_MOFFS32))return(0);

	if((fl&4) && (basm_opcat[i*16+j]!=OPCAT_MOFFS64))return(0);

	if(w)
	{
		if((basm_opcat[i*16+j]==OPCAT_RM8 ) && (w!=8 ) && (w!=80))
			return(0);

		if((basm_opcat[i*16+j]==OPCAT_RM16) && (w!=16))return(0);
		if((basm_opcat[i*16+j]==OPCAT_RM32) && (w!=32))return(0);
		if((basm_opcat[i*16+j]==OPCAT_RM64) && (w!=64))return(0);
	}

	return(1);
}
#endif

int BASM_OpArgImmP(int i, int j, int w, long long imm)
{
	if(!w)
	{
		w=8;
		if((imm<(-128)) || (imm>127))w=16;
		if((imm<(-32768)) || (imm>32767))w=32;
		if((imm<(-(1LL<<31))) || (imm>((1LL<<31)-1)))w=64;
	}

	if((basm_opcat[i*16+j]==OPCAT_IMM0) && (imm==0))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM1) && (imm==1))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM2) && (imm==2))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM3) && (imm==3))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM4) && (imm==4))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM5) && (imm==5))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM6) && (imm==6))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM7) && (imm==7))return(1);

	if((basm_opcat[i*16+j]==OPCAT_UIMM8 ) && (imm>=0) && (imm<=255))
		return(1);
	if((basm_opcat[i*16+j]==OPCAT_UIMM16) && (imm>=0) && (imm<=65535))
		return(1);

//	if((basm_opcat[i*16+j]==OPCAT_UIMM8 ) && (w==8 ))return(1);
//	if((basm_opcat[i*16+j]==OPCAT_UIMM16) && (w<=16))return(1);


	if((basm_opcat[i*16+j]==OPCAT_IMM8 ) && (w==8 ))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM16) && (w<=16))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM32) && (w<=32))return(1);
	if((basm_opcat[i*16+j]==OPCAT_IMM64) && (w<=64))return(1);

	if((basm_opcat[i*16+j]==OPCAT_REL8 ) && (w==8 ))return(1);
	if((basm_opcat[i*16+j]==OPCAT_REL16) && (w<=16))return(1);
	if((basm_opcat[i*16+j]==OPCAT_REL32) && (w<=32))return(1);
	if((basm_opcat[i*16+j]==OPCAT_REL64) && (w<=64))return(1);

	return(0);
}

int BASM_OpArchP(BASM_Context *ctx, int i)
{
	int j;

	if((basm_opcat[i*16+7]&OPFL_LONG) && !(ctx->fl&1))return(0);
	if((basm_opcat[i*16+7]&OPFL_LEG) && (ctx->fl&1))return(0);
	
	if(ctx->cpu==BASM_CPU_THUMB)
	{
		j=ctx->ip-ctx->text;
		if(basm_opcat[i*16+7]&OPFL_EVEN)
			{ if((j&3)!=0)return(0); }
		if(basm_opcat[i*16+7]&OPFL_ODD)
			{ if((j&3)!=2)return(0); }
			
		if(j&1)printf("BASM_OpArchP: Misaligned Thumb Op\n");
	}
	
	return(1);
}

int BASM_OpRegP(int i, int reg)
{
	if(!BASM_OpArgRegP(i, 0, reg))return(0);
	if(basm_opcat[i*16+1])return(0);
	if(basm_opcat[i*16+2])return(0);

	return(1);
}

int BASM_OpImmP(int i, int w, long long imm)
{
	int j;

	if(basm_opcat[i*16+1])return(0);
	if(basm_opcat[i*16+2])return(0);

	j=BASM_OpArgImmP(i, 0, w, imm);
	return(j);
}

int BASM_OpMemP(int i, int w)
{
	if(!BASM_OpArgMemP(i, 0, w, 0))return(0);
	if(basm_opcat[i*16+1])return(0);
	if(basm_opcat[i*16+2])return(0);
	return(1);
}

int BASM_OpRegRegP(int i, int r0, int r1)
{
	if(!BASM_OpArgRegP(i, 0, r0))return(0);
	if(!BASM_OpArgRegP(i, 1, r1))return(0);
	if(basm_opcat[i*16+2])return(0);
	return(1);
}

int BASM_OpRegImmP(int i, int reg, long long imm)
{
	int w;

	w=BASM_GetRegWidth(reg);
	if(w==8) imm=(char)imm;
	if(w==16) imm=(short)imm;
	if(w==32) imm=(int)imm;

	w=0;

	if(!BASM_OpArgRegP(i, 0, reg))return(0);
	if(!BASM_OpArgImmP(i, 1, w, imm))return(0);
	if(basm_opcat[i*16+2])return(0);

	return(1);
}

int BASM_OpRegMemP(int i, int reg, int fl)
{
	if(!BASM_OpArgRegP(i, 0, reg))return(0);
	if(!BASM_OpArgMemP(i, 1, 0, fl))return(0);
	if(basm_opcat[i*16+2])return(0);

	return(1);
}

int BASM_OpMemRegP(int i, int reg, int fl)
{
	if(!BASM_OpArgMemP(i, 0, 0, fl))return(0);
	if(!BASM_OpArgRegP(i, 1, reg))return(0);
	if(basm_opcat[i*16+2])return(0);

	return(1);
}

int BASM_OpMemImmP(int i, int w, long long imm)
{
	if(w==8)imm=(char)imm;
	if(w==16)imm=(short)imm;
	if(w==32)imm=(int)imm;

	if(!BASM_OpArgMemP(i, 0, w, 0))return(0);
	if(!BASM_OpArgImmP(i, 1, w, imm))return(0);
	if(basm_opcat[i*16+2])return(0);

	return(1);
}


int BASM_OpRegRegImmP(int i, int r0, int r1, long long imm)
{
	if(!BASM_OpArgRegP(i, 0, r0))return(0);
	if(!BASM_OpArgRegP(i, 1, r1))return(0);
	if(!BASM_OpArgImmP(i, 2, 0, imm))return(0);
	if(basm_opcat[i*16+3])return(0);
	return(1);
}

int BASM_OpRegMemImmP(int i, int reg, int fl, long long imm)
{
	if(!BASM_OpArgRegP(i, 0, reg))return(0);
	if(!BASM_OpArgMemP(i, 1, 0, fl))return(0);
	if(!BASM_OpArgImmP(i, 2, 0, imm))return(0);
	if(basm_opcat[i*16+3])return(0);

	return(1);
}

int BASM_OpMemRegImmP(int i, int reg, int fl, long long imm)
{
	if(!BASM_OpArgMemP(i, 0, 0, fl))return(0);
	if(!BASM_OpArgRegP(i, 1, reg))return(0);
	if(!BASM_OpArgImmP(i, 2, 0, imm))return(0);
	if(basm_opcat[i*16+3])return(0);

	return(1);
}


int BASM_OpRegRegRegP(int i, int r0, int r1, int r2)
{
	if(!BASM_OpArgRegP(i, 0, r0))return(0);
	if(!BASM_OpArgRegP(i, 1, r1))return(0);
	if(!BASM_OpArgRegP(i, 2, r2))return(0);
	if(basm_opcat[i*16+3])return(0);
	return(1);
}

int BASM_OpRegMemRegP(int i, int r0, int r1, int fl)
{
	if(!BASM_OpArgRegP(i, 0, r0))return(0);
	if(!BASM_OpArgMemP(i, 1, 0, fl))return(0);
	if(!BASM_OpArgRegP(i, 2, r1))return(0);
	if(basm_opcat[i*16+3])return(0);

	return(1);
}

int BASM_OpMemRegRegP(int i, int r0, int r1, int fl)
{
	if(!BASM_OpArgMemP(i, 0, 0, fl))return(0);
	if(!BASM_OpArgRegP(i, 1, r0))return(0);
	if(!BASM_OpArgRegP(i, 2, r1))return(0);
	if(basm_opcat[i*16+3])return(0);

	return(1);
}


BASM_API void BASM_OutOpSingle(BASM_Context *ctx, int op)
{
	int i;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];
	
	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpSingleP(i))
			continue;
		BASM_OutOpStr(ctx, basm_opdecl[i]);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpReg(BASM_Context *ctx, int op, int reg)
{
	int i;

//	if(!(ctx->fl&1) && (((reg>>4)==1) || ((reg>>4)==2)))
//	{
//		if(op==BASM_OP_INC)op=BASM_OP_INC_R;
//		if(op==BASM_OP_DEC)op=BASM_OP_DEC_R;
//	}

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpRegP(i, reg))continue;
		BASM_OutOpStrReg(ctx, basm_opdecl[i], reg);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s reg\n", basm_ops[op]);
//	*(int *)-1=-1;
}


BASM_API void BASM_OutOpImm(BASM_Context *ctx, int op, int w,
	long long imm, char *lbl)
{
	int i;

	if(lbl && !w)w=(ctx->fl&2)?16:32;
	if(!w && (op==BASM_OP_X86_PUSH))w=(ctx->fl&2)?16:32;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpImmP(i, w, imm))continue;
		BASM_OutOpStrImm(ctx, basm_opdecl[i], imm, lbl);
		return;
	}

	i=basm_opidx[op];
	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpImmP(i, 0, imm))continue;
		BASM_OutOpStrImm(ctx, basm_opdecl[i], imm, lbl);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s imm\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpMem(BASM_Context *ctx, int op, int w,
	char *lbl, int breg, int ireg, int sc, long long disp)
{
	int i;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	if((w==0) && (basm_opnums[i+1]==op))
	{
		basm_warning("ASM: Warn, '%s mem', arg size needed\n",
			basm_ops[op]);
		w=(ctx->fl&BASM_FL_X86_64)?64:
			((ctx->fl&BASM_FL_X86_16)?16:32);
	}

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpMemP(i, w))continue;
		BASM_OutOpStrMem(ctx, basm_opdecl[i],
			lbl, breg, ireg, sc, disp);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s mem\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpRegReg(BASM_Context *ctx, int op, int r0, int r1)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpRegRegP(i, r0, r1))continue;

		if(	(basm_opcat[i*16+0]!=OPCAT_REG8) &&
			(basm_opcat[i*16+0]!=OPCAT_REG16) &&
			(basm_opcat[i*16+0]!=OPCAT_REG32) &&
			(basm_opcat[i*16+0]!=OPCAT_REG64) &&
			(basm_opcat[i*16+0]!=OPCAT_FREG) &&
			(basm_opcat[i*16+0]!=OPCAT_XREG))
			{ j=r0; r0=r1; r1=j; }

		BASM_OutOpStrRegReg(ctx, basm_opdecl[i], r0, r1);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s reg reg\n", basm_ops[op]);
//	*(int *)-1=-1;
}


BASM_API void BASM_OutOpRegImm(BASM_Context *ctx, int op,
	int reg, long long imm, char *lbl)
{
	int i;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpRegImmP(i, reg, lbl?0x7F7F7F7F7F7FLL:imm))continue;

//		printf("OpRegImm: %s\n", basm_opdecl[i]);
		BASM_OutOpStrRegImm(ctx, basm_opdecl[i], reg, imm, lbl);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s reg, imm\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpRegMem(BASM_Context *ctx, int op, int reg,
	char *lbl, int breg, int ireg, int sc, long long disp)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;

		j=0; if(breg!=BASM_Z)j|=1; if(ireg!=BASM_Z)j|=1;
		if(sc && (sc!=1))j|=1;
		if(!(ctx->fl&2) && lbl)j|=2;
		if((disp<0) || (disp>=65536))j|=2;
		if((disp<-2147483648LL) || (disp>2147483647LL))j|=4;

		if(!BASM_OpRegMemP(i, reg, j))continue;
		BASM_OutOpStrRegMem(ctx, basm_opdecl[i], reg,
			lbl, breg, ireg, sc, disp);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s reg, mem\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpMemReg(BASM_Context *ctx, int op, int reg,
	char *lbl, int breg, int ireg, int sc, long long disp)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;

		j=0; if(breg!=BASM_Z)j|=1; if(ireg!=BASM_Z)j|=1;
		if(sc && (sc!=1))j|=1;
		if(!(ctx->fl&2) && lbl)j|=2;
		if((disp<0) || (disp>=65536))j|=2;
		if((disp<-2147483648LL) || (disp>2147483647LL))j|=4;

		if(!BASM_OpMemRegP(i, reg, j))continue;
		BASM_OutOpStrRegMem(ctx, basm_opdecl[i], reg,
			lbl, breg, ireg, sc, disp);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s mem, reg\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpMemImm(BASM_Context *ctx, int op, int w,
	char *lbl, int breg, int ireg, int sc, long long disp,
	long long imm, char *lbl2)
{
	int i;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	if((w==0) && (basm_opnums[i+1]==op))
	{
		basm_warning("ASM: Warn, '%s mem, imm', arg size needed\n",
			basm_ops[op]);
		w=(ctx->fl&BASM_FL_X86_64)?64:
			((ctx->fl&BASM_FL_X86_16)?16:32);
	}

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpMemImmP(i, w, lbl2?0x7F7F7F7F7F7FLL:imm))continue;
		BASM_OutOpStrMemImm(ctx, basm_opdecl[i], w,
			lbl, breg, ireg, sc, disp, imm, lbl2);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s mem, imm\n", basm_ops[op]);
//	*(int *)-1=-1;
}


BASM_API void BASM_OutOpRegRegImm(BASM_Context *ctx,
	int op, int r0, int r1,
	long long imm, char *lbl2)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpRegRegImmP(i, r0, r1, lbl2?0x7F7F7F7F7F7FLL:imm))
			continue;

		if(	(basm_opcat[i*16+0]!=OPCAT_REG8) &&
			(basm_opcat[i*16+0]!=OPCAT_REG16) &&
			(basm_opcat[i*16+0]!=OPCAT_REG32) &&
			(basm_opcat[i*16+0]!=OPCAT_REG64) &&
			(basm_opcat[i*16+0]!=OPCAT_FREG) &&
			(basm_opcat[i*16+0]!=OPCAT_XREG))
				{ j=r0; r0=r1; r1=j; }
		BASM_OutOpStrRegRegImm(ctx, basm_opdecl[i], r0, r1, imm, lbl2);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s reg, reg, imm\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpRegMemImm(BASM_Context *ctx, int op, int reg,
	char *lbl, int breg, int ireg, int sc, long long disp,
	long long imm, char *lbl2)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;

		j=0; if(breg!=BASM_Z)j|=1; if(ireg!=BASM_Z)j|=1;
		if(sc && (sc!=1))j|=1;
		if(!(ctx->fl&2) && lbl)j|=2;
		if((disp<0) || (disp>=65536))j|=2;
		if((disp<-2147483648LL) || (disp>2147483647LL))j|=4;

		if(!BASM_OpRegMemImmP(i, reg, j, lbl2?0x7F7F7F7F7F7FLL:imm))
			continue;
		BASM_OutOpStrRegMemImm(ctx, basm_opdecl[i], reg,
			lbl, breg, ireg, sc, disp, imm, lbl2);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s reg, mem\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpMemRegImm(BASM_Context *ctx, int op, int reg,
	char *lbl, int breg, int ireg, int sc, long long disp,
	long long imm, char *lbl2)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;

		j=0; if(breg!=BASM_Z)j|=1; if(ireg!=BASM_Z)j|=1;
		if(sc && (sc!=1))j|=1;
		if(!(ctx->fl&2) && lbl)j|=2;
		if((disp<0) || (disp>=65536))j|=2;
		if((disp<-2147483648LL) || (disp>2147483647LL))j|=4;

		if(!BASM_OpMemRegImmP(i, reg, j, lbl2?0x7F7F7F7F7F7FLL:imm))
			continue;
		BASM_OutOpStrRegMemImm(ctx, basm_opdecl[i], reg,
			lbl, breg, ireg, sc, disp, imm, lbl2);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s mem, reg\n", basm_ops[op]);
//	*(int *)-1=-1;
}


BASM_API void BASM_OutOpRegRegReg(BASM_Context *ctx,
	int op, int r0, int r1, int r2)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;
		if(!BASM_OpRegRegRegP(i, r0, r1, r2))
			continue;

		if(	(basm_opcat[i*16+0]!=OPCAT_REG8) &&
			(basm_opcat[i*16+0]!=OPCAT_REG16) &&
			(basm_opcat[i*16+0]!=OPCAT_REG32) &&
			(basm_opcat[i*16+0]!=OPCAT_REG64) &&
			(basm_opcat[i*16+0]!=OPCAT_FREG) &&
			(basm_opcat[i*16+0]!=OPCAT_XREG))
				{ j=r0; r0=r1; r1=j; }
		BASM_OutOpStrRegRegReg(ctx, basm_opdecl[i], r0, r1, r2);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s reg, reg, reg\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpRegMemReg(BASM_Context *ctx, int op, int reg,
	char *lbl, int breg, int ireg, int sc, long long disp,
	int reg2)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;

		j=0; if(breg!=BASM_Z)j|=1; if(ireg!=BASM_Z)j|=1;
		if(sc && (sc!=1))j|=1;
		if(!(ctx->fl&2) && lbl)j|=2;
		if((disp<0) || (disp>=65536))j|=2;
		if((disp<-2147483648LL) || (disp>2147483647LL))j|=4;

		if(!BASM_OpRegMemRegP(i, reg, reg2, j))
			continue;
		BASM_OutOpStrRegMemReg(ctx, basm_opdecl[i], reg,
			lbl, breg, ireg, sc, disp, reg2);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s reg, mem\n", basm_ops[op]);
//	*(int *)-1=-1;
}

BASM_API void BASM_OutOpMemRegReg(BASM_Context *ctx, int op, int reg,
	char *lbl, int breg, int ireg, int sc, long long disp,
	int reg2)
{
	int i, j;

	i=basm_opidx[op];
	if(basm_opnums[i]!=op)
		op=basm_opnums[i];

	for(; basm_opnums[i]==op; i++)
	{
		if(!BASM_OpArchP(ctx, i))continue;

		j=0; if(breg!=BASM_Z)j|=1; if(ireg!=BASM_Z)j|=1;
		if(sc && (sc!=1))j|=1;
		if(!(ctx->fl&2) && lbl)j|=2;
		if((disp<0) || (disp>=65536))j|=2;
		if((disp<-2147483648LL) || (disp>2147483647LL))j|=4;

		if(!BASM_OpMemRegRegP(i, reg, reg2, j))
			continue;
		BASM_OutOpStrRegMemReg(ctx, basm_opdecl[i], reg,
			lbl, breg, ireg, sc, disp,
			reg2);
		return;
	}

	basm_error("ASM: Bad Opcode Args, %s mem, reg\n", basm_ops[op]);
//	*(int *)-1=-1;
}


BASM_API void BASM_OutOpGeneric1(BASM_Context *ctx, int op, int w,
	char *lbl, int breg, int ireg, int sc, long long disp)
{
	if(sc)
	{
		BASM_OutOpMem(ctx, op, w, lbl, breg, ireg, sc, disp);
	}else if(breg!=BASM_Z)
	{
		BASM_OutOpReg(ctx, op, breg);
	}else
	{
		BASM_OutOpImm(ctx, op, w, disp, lbl);
	}
}

BASM_API void BASM_OutOpGeneric2(BASM_Context *ctx, int op, int w,
	char *lbl0, int breg0, int ireg0, int sc0, long long disp0,
	char *lbl1, int breg1, int ireg1, int sc1, long long disp1)
{
	if(sc0)
	{
		if(sc1)
		{
			basm_warning("BASM_ParseOpcode: Invalid opcode args\n");
			return;
		}

		if(breg1!=BASM_Z)
		{
			BASM_OutOpMemReg(ctx, op, breg1,
				lbl0, breg0, ireg0, sc0, disp0);
		}else
		{
			BASM_OutOpMemImm(ctx, op, w,
				lbl0, breg0, ireg0, sc0, disp0, disp1, lbl1);
		}
	}else if(breg0!=BASM_Z)
	{
		if(sc1)
		{
			if(ctx->fl&BASM_FL_ARM)
			{
				if(ireg1!=BASM_Z)
				{
					BASM_OutOpRegRegReg(ctx, op, breg0, breg1, ireg1);
					return;
				}
				if(disp1 || lbl1)
				{
					BASM_OutOpRegRegImm(ctx, op, breg0, breg1, disp1, lbl1);
					return;
				}
				BASM_OutOpRegReg(ctx, op, breg0, breg1);
				return;
			}
		
			BASM_OutOpRegMem(ctx, op, breg0,
				lbl1, breg1, ireg1, sc1, disp1);
		}else if(breg1!=BASM_Z)
		{
			BASM_OutOpRegReg(ctx, op, breg0, breg1);
		}else
		{
			BASM_OutOpRegImm(ctx, op, breg0, disp1, lbl1);
		}
	}else
	{
		basm_error("BASM_ParseOpcode: Invalid opcode args\n");
		return;
	}
}

BASM_API void BASM_OutOpGeneric3(BASM_Context *ctx, int op, int w,
	char *lbl0, int breg0, int ireg0, int sc0, long long disp0,
	char *lbl1, int breg1, int ireg1, int sc1, long long disp1,
	char *lbl2, int breg2, int ireg2, int sc2, long long disp2)
{
	if(sc0)
	{
		if(sc1)
		{
			basm_error("BASM_ParseOpcode: Invalid opcode args\n");
			return;
		}

		if(breg1!=BASM_Z)
		{
			if(breg2!=BASM_Z)
			{
				BASM_OutOpMemRegReg(ctx, op, breg1,
					lbl0, breg0, ireg0, sc0, disp0,
					breg2);
				return;
			}

			BASM_OutOpMemRegImm(ctx, op, breg1,
				lbl0, breg0, ireg0, sc0, disp0, disp2, lbl2);
		}else
		{
			basm_error("BASM_ParseOpcode: Invalid opcode args\n");
//			BASM_OutOpMemImmImm(ctx, i, w,
//				l0, br0, ir0, sc0, disp0, disp1, l1,
//				disp2, l2);
		}
	}else if(breg0!=BASM_Z)
	{
		if(sc1)
		{
			if(breg2!=BASM_Z)
			{
				BASM_OutOpRegMemReg(ctx, op, breg0,
					lbl1, breg1, ireg1, sc1, disp1,
					breg2);
				return;
			}

			BASM_OutOpRegMemImm(ctx, op, breg0,
				lbl1, breg1, ireg1, sc1, disp1, disp2, lbl2);
		}else if(breg1!=BASM_Z)
		{
			if(breg2!=BASM_Z)
			{
				BASM_OutOpRegRegReg(ctx, op,
					breg0, breg1, breg2);
				return;
			}

			BASM_OutOpRegRegImm(ctx, op, breg0, breg1,
				disp2, lbl2);
		}else
		{
			basm_error("BASM_ParseOpcode: Invalid opcode args\n");
//			BASM_OutOpRegImmImm(ctx, op, breg0, disp1, lbl1,
//				disp2, lbl2);
		}
	}else
	{
		basm_error("BASM_ParseOpcode: Invalid opcode args\n");
		return;
	}
}

BASM_API void BASM_EmitLabelPos(BASM_Context *ctx, char *name, int pos)
{
	int i, j;

//	printf("Emit Label %s @ %p\n", name, ctx->ip);

	i=ctx->n_labels++;
	if(i>=ctx->m_labels)
	{
		j=ctx->m_labels+(ctx->m_labels>>1);
		ctx->label_name=(char **)
			realloc(ctx->label_name, j*sizeof(char *));
		ctx->label_pos=(int *)realloc(ctx->label_pos, j*sizeof(int));

		ctx->llbl_name=(char **)
			realloc(ctx->llbl_name, j*sizeof(char *));
		ctx->llbl_pos=(int *)realloc(ctx->llbl_pos, j*sizeof(int));

		ctx->m_labels=j;
	}

	ctx->label_name[i]=basm_strdup(name);
	ctx->label_pos[i]=pos;
}

BASM_API void BASM_EmitGotoPos(BASM_Context *ctx, char *name, int ty, int pos)
{
	int i, j;

//	printf("Emit Goto %s\n", name);

	i=ctx->n_gotos++;
	if(i>=ctx->m_gotos)
	{
		j=ctx->m_gotos+(ctx->m_gotos>>1);
		ctx->goto_name=(char **)realloc(ctx->goto_name, j*sizeof(char *));
		ctx->goto_pos=(int *)realloc(ctx->goto_pos, j*sizeof(int));
		ctx->goto_type=(byte *)realloc(ctx->goto_type, j);
		ctx->m_gotos=j;
	}

	ctx->goto_name[i]=name?basm_strdup(name):NULL;
	ctx->goto_pos[i]=pos;
	ctx->goto_type[i]=ty;
}

BASM_API void BASM_EmitLabel(BASM_Context *ctx, char *name)
{
	int i;

	if(!name)return;

	if(ctx->fl&4)i=(2<<24)+(ctx->dp-ctx->data);
		else i=(1<<24)+(ctx->ip-ctx->text);
	BASM_EmitLabelPos(ctx, name, i);
}

BASM_API void BASM_EmitGoto(BASM_Context *ctx, char *name, int ty)
{
	int i;

	if(!name)return;

	if(ctx->fl&4)i=(2<<24)+(ctx->dp-ctx->data);
		else i=(1<<24)+(ctx->ip-ctx->text);
	BASM_EmitGotoPos(ctx, name, ty, i);
}


BASM_API void BASM_EmitConst(BASM_Context *ctx, char *name, long long val)
{
	int i, j;

	if(!name)return;

	i=ctx->n_const++;
	if(i>=ctx->m_const)
	{
		j=ctx->m_const+(ctx->m_const>>1);
		ctx->const_name=
			(char **)realloc(ctx->const_name, j*sizeof(char *));
		ctx->const_value=
			(long long *)realloc(ctx->const_value,
				j*sizeof(long long));
		ctx->m_const=j;
	}

	ctx->const_name[i]=basm_strdup(name);
	ctx->const_value[i]=val;
}

BASM_API int BASM_PredictPos(BASM_Context *ctx, char *name)
{
	int i;

	if(!name)return(-1);

	for(i=0; i<ctx->n_labels; i++)
		if(!strcmp(ctx->label_name[i], name))
			return(ctx->label_pos[i]);

	for(i=0; i<ctx->n_llbl; i++)
		if(!strcmp(ctx->llbl_name[i], name))
			return(ctx->llbl_pos[i]);

	return(-1);
}

BASM_API int BASM_PredictDisp(BASM_Context *ctx, char *name)
{
	int i, j;

	if(!name)return(-1);

	j=BASM_PredictPos(ctx, name);
	if(j<0)return(0);

	if(ctx->fl&4)
	{
//		i=(2<<24)+(ctx->dp-ctx->data);
		if(((j>>24)&0x0F)!=2)return(0);

		i=(j&0xFFFFFF)-(ctx->dp-ctx->data);
		return(i);
	}else
	{
//		i=(1<<24)+(ctx->ip-ctx->text);
		if(((j>>24)&0x0F)!=1)return(0);

		i=(j&0xFFFFFF)-(ctx->ip-ctx->text);
		return(i);
	}
}
