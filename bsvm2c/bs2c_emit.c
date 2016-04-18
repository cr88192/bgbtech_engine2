#include <bteifgl.h>

void BS2C_EmitCheckExpand(BS2CC_CompileContext *ctx, int sz)
{
	int ct, sz1;

	if(!ctx->frm->cts)
	{
		sz1=1024;
		while(sz1<sz)
			sz1=sz1+(sz1>>1);
		ctx->frm->cts=frgl_malloc(sz1);
		ctx->frm->cte=ctx->frm->cts+sz1;
		ctx->frm->ct=ctx->frm->cts;
		return;
	}
	
	if((ctx->frm->ct+sz)>ctx->frm->cte)
	{
		sz1=ctx->frm->cte-ctx->frm->cts;
		ct=ctx->frm->ct-ctx->frm->cts;
		
		while(sz1<(ct+sz))
			sz1=sz1+(sz1>>1);

		ctx->frm->cts=frgl_realloc(ctx->frm->cts, sz1);
		ctx->frm->cte=ctx->frm->cts+sz1;
		ctx->frm->ct=ctx->frm->cts+ct;
		return;
	}
}

void BS2C_EmitByte(BS2CC_CompileContext *ctx, int op)
{
	BS2C_EmitCheckExpand(ctx, 1);
	*ctx->frm->ct++=op;
}

void BS2C_EmitWord(BS2CC_CompileContext *ctx, int op)
{
	BS2C_EmitCheckExpand(ctx, 2);
	*ctx->frm->ct++=op>>8;
	*ctx->frm->ct++=op;

//	BS2C_EmitByte(ctx, op>>8);
//	BS2C_EmitByte(ctx, op);
}

void BS2C_EmitOpcode(BS2CC_CompileContext *ctx, int op)
{
	ctx->frm->newtrace=0;

	if(op<0xE0)
	{
		BS2C_EmitByte(ctx, op);
		return;
	}

	if(op<0x1000)
	{
		BS2C_EmitByte(ctx, 0xE0|(op>>8));
		BS2C_EmitByte(ctx, op);
		return;
	}

	if(op<0x80000)
	{
		BS2C_EmitByte(ctx, 0xF0|(op>>16));
		BS2C_EmitByte(ctx, (op>> 8));
		BS2C_EmitByte(ctx, (op    ));
		return;
	}

	if(op<0x4000000)
	{
		BS2C_EmitByte(ctx, 0xF8|(op>>24));
		BS2C_EmitByte(ctx, (op>>16));
		BS2C_EmitByte(ctx, (op>> 8));
		BS2C_EmitByte(ctx, (op    ));
		return;
	}

	BS2C_EmitByte(ctx, 0xFC);
	BS2C_EmitByte(ctx, (op>>24));
	BS2C_EmitByte(ctx, (op>>16));
	BS2C_EmitByte(ctx, (op>> 8));
	BS2C_EmitByte(ctx, (op    ));
}

void BS2C_EmitOpcodeIdx(BS2CC_CompileContext *ctx, int ix)
{
	if(ix<0x80)
	{
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<0x4000)
	{
		BS2C_EmitByte(ctx, 0x80|(ix>>8));
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<0x200000)
	{
		BS2C_EmitByte(ctx, 0xC0|(ix>>16));
		BS2C_EmitByte(ctx, ix>>8);
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<0x10000000)
	{
		BS2C_EmitByte(ctx, 0xE0|(ix>>24));
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>>8);
		BS2C_EmitByte(ctx, ix);
		return;
	}

	BS2C_EmitByte(ctx, 0xF0);
	BS2C_EmitByte(ctx, ix>>24);
	BS2C_EmitByte(ctx, ix>>16);
	BS2C_EmitByte(ctx, ix>> 8);
	BS2C_EmitByte(ctx, ix    );
}

void BS2C_EmitOpcodeUCx(BS2CC_CompileContext *ctx, u64 ix)
{
	if(ix<0x80)
	{
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<0x4000)
	{
		BS2C_EmitByte(ctx, 0x80|(ix>>8));
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<0x200000)
	{
		BS2C_EmitByte(ctx, 0xC0|(ix>>16));
		BS2C_EmitByte(ctx, ix>>8);
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<0x10000000)
	{
		BS2C_EmitByte(ctx, 0xE0|(ix>>24));
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>>8);
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<0x800000000ULL)
	{
		BS2C_EmitByte(ctx, 0xF0|(ix>>32));
		BS2C_EmitByte(ctx, ix>>24);
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>> 8);
		BS2C_EmitByte(ctx, ix    );
		return;
	}

	if(ix<0x40000000000ULL)
	{
		BS2C_EmitByte(ctx, 0xF8|(ix>>40));
		BS2C_EmitByte(ctx, ix>>32);
		BS2C_EmitByte(ctx, ix>>24);
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>> 8);
		BS2C_EmitByte(ctx, ix    );
		return;
	}

	if(ix<0x2000000000000ULL)
	{
		BS2C_EmitByte(ctx, 0xFC|(ix>>48));
		BS2C_EmitByte(ctx, ix>>40);
		BS2C_EmitByte(ctx, ix>>32);
		BS2C_EmitByte(ctx, ix>>24);
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>> 8);
		BS2C_EmitByte(ctx, ix    );
		return;
	}

	if(ix<0x100000000000000ULL)
	{
//		BS2C_EmitByte(ctx, 0xFE|(ix>>56));
		BS2C_EmitByte(ctx, 0xFE);
		BS2C_EmitByte(ctx, ix>>48);
		BS2C_EmitByte(ctx, ix>>40);
		BS2C_EmitByte(ctx, ix>>32);
		BS2C_EmitByte(ctx, ix>>24);
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>> 8);
		BS2C_EmitByte(ctx, ix    );
		return;
	}

	BS2C_EmitByte(ctx, 0xFF);
	BS2C_EmitByte(ctx, ix>>56);
	BS2C_EmitByte(ctx, ix>>48);
	BS2C_EmitByte(ctx, ix>>40);
	BS2C_EmitByte(ctx, ix>>32);
	BS2C_EmitByte(ctx, ix>>24);
	BS2C_EmitByte(ctx, ix>>16);
	BS2C_EmitByte(ctx, ix>> 8);
	BS2C_EmitByte(ctx, ix    );
}

void BS2C_EmitOpcodeSCx(BS2CC_CompileContext *ctx, s64 ix)
{
	BS2C_EmitOpcodeUCx(ctx, (ix<<1)^(ix>>63));
}

void BS2C_EmitOpcodeSuCx(BS2CC_CompileContext *ctx, int z, s64 ix)
{
	if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_LONG) ||
		(z==BSVM2_OPZ_SBYTE) || (z==BSVM2_OPZ_SHORT))
	{
		BS2C_EmitOpcodeUCx(ctx, (ix<<1)^(ix>>63));
		return;
	}

	if((z==BSVM2_OPZ_FLOAT) || (z==BSVM2_OPZ_DOUBLE))
	{
		BS2C_EmitOpcodeFxD(ctx, ix);
		return;
	}

	BS2C_EmitOpcodeUCx(ctx, ix);
}

void BS2C_EmitOpcodeSuCxD(BS2CC_CompileContext *ctx, int z, double v)
{
	if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_LONG) ||
		(z==BSVM2_OPZ_SBYTE) || (z==BSVM2_OPZ_SHORT))
	{
		BS2C_EmitOpcodeSuCx(ctx, z, v);
		return;
	}

	if((z==BSVM2_OPZ_FLOAT) || (z==BSVM2_OPZ_DOUBLE))
	{
		BS2C_EmitOpcodeFxD(ctx, v);
		return;
	}

	BS2C_EmitOpcodeUCx(ctx, v);
}

void BS2C_EmitOpcodeFxD(BS2CC_CompileContext *ctx, double f)
{
	u64 ix;
	s64 m;
	int e;

	ix=*(u64 *)(&f);

	m=(s64)((ix&0x000FFFFFFFFFFFFFULL)|
		0x0010000000000000ULL);
	e=((ix>>52)&2047)-(1023+52);

	while(!(m&255))
		{ m=m>>8; e=e+8; }
	while(!(m&1))
		{ m=m>>1; e++; }
	
	if(ix>>63)
		{ m=-m; }
	
	BS2C_EmitOpcodeSCx(ctx, e);
	BS2C_EmitOpcodeSCx(ctx, m);
}

void BS2C_EmitOpcodeJx(BS2CC_CompileContext *ctx, int vi, int vj)
{
	int k;
	
	if((vi<8) && (vj<16))
	{
		k=(vi<<4)|vj;
		BS2C_EmitByte(ctx, k);
		return;
	}

	if((vi<128) && (vj<128))
	{
		k=(vi<<7)|vj;
		BS2C_EmitOpcodeUCx(ctx, k);
		return;
	}

	if((vi<1024) && (vj<2048))
	{
		k=(vi<<11)|vj;
		BS2C_EmitOpcodeUCx(ctx, k);
		return;
	}
}

void BS2C_EmitOpcodeUKx(BS2CC_CompileContext *ctx, int vi, u64 vj)
{
	u64 lk;
	int k;
	
	if((vi<8) && (vj<16))
	{
		k=(vi<<4)|vj;
		BS2C_EmitByte(ctx, k);
		return;
	}

	if((vi<32) && (vj<512))
	{
		k=(vi<<9)|vj;
		BS2C_EmitByte(ctx, 0x80|(k>>8));
		BS2C_EmitByte(ctx, (k   )&255);
//		BS2C_EmitOpcodeUCx(ctx, k);
		return;
	}

	if((vi<256) && (vj<8192))
	{
		k=(vi<<13)|vj;
		BS2C_EmitByte(ctx, 0xC0|(k>>16));
		BS2C_EmitByte(ctx, (k>>8)&255);
		BS2C_EmitByte(ctx, (k   )&255);
//		BS2C_EmitOpcodeUCx(ctx, k);
		return;
	}

	if((vi<256) && (vj<(1<<20)))
	{
		k=(vi<<20)|vj;
		BS2C_EmitByte(ctx, 0xE0|(k>>24));
		BS2C_EmitByte(ctx, (k>>16)&255);
		BS2C_EmitByte(ctx, (k>> 8)&255);
		BS2C_EmitByte(ctx, (k    )&255);
//		BS2C_EmitOpcodeUCx(ctx, k);
		return;
	}

	if((vi<256) && (vj<(1<<27)))
	{
		lk=(((u64)vi)<<27)|vj;

		BS2C_EmitByte(ctx, 0xF0|(lk>>32));
		BS2C_EmitByte(ctx, (lk>>24)&255);
		BS2C_EmitByte(ctx, (lk>>16)&255);
		BS2C_EmitByte(ctx, (lk>> 8)&255);
		BS2C_EmitByte(ctx, (lk    )&255);

//		BS2C_EmitOpcodeUCx(ctx, lk);
		return;
	}

	if((vi<1024) && (vj<(1LL<<32)))
	{
		lk=(((u64)vi)<<32)|vj;
		BS2C_EmitByte(ctx, 0xF8|(lk>>40));
		BS2C_EmitByte(ctx, (lk>>32)&255);
		BS2C_EmitByte(ctx, (lk>>24)&255);
		BS2C_EmitByte(ctx, (lk>>16)&255);
		BS2C_EmitByte(ctx, (lk>> 8)&255);
		BS2C_EmitByte(ctx, (lk    )&255);
//		BS2C_EmitOpcodeUCx(ctx, lk);
		return;
	}

	if((vi<1024) && (vj<(1LL<<39)))
	{
		lk=(((u64)vi)<<39)|vj;
		BS2C_EmitByte(ctx, 0xFC|(lk>>48));
		BS2C_EmitByte(ctx, (lk>>40)&255);
		BS2C_EmitByte(ctx, (lk>>32)&255);
		BS2C_EmitByte(ctx, (lk>>24)&255);
		BS2C_EmitByte(ctx, (lk>>16)&255);
		BS2C_EmitByte(ctx, (lk>> 8)&255);
		BS2C_EmitByte(ctx, (lk    )&255);
//		BS2C_EmitOpcodeUCx(ctx, lk);
		return;
	}

	if((vi<4096) && (vj<(1LL<<44)))
	{
		lk=(((u64)vi)<<44)|vj;
		BS2C_EmitByte(ctx, 0xFE|(lk>>56));
		BS2C_EmitByte(ctx, (lk>>48)&255);
		BS2C_EmitByte(ctx, (lk>>40)&255);
		BS2C_EmitByte(ctx, (lk>>32)&255);
		BS2C_EmitByte(ctx, (lk>>24)&255);
		BS2C_EmitByte(ctx, (lk>>16)&255);
		BS2C_EmitByte(ctx, (lk>> 8)&255);
		BS2C_EmitByte(ctx, (lk    )&255);
//		BS2C_EmitOpcodeUCx(ctx, lk);
		return;
	}

	if((vi<4096) && (vj<(1LL<<52)))
	{
		lk=(((u64)vi)<<52)|vj;
		BS2C_EmitByte(ctx, 0xFF);
		BS2C_EmitByte(ctx, (lk>>56)&255);
		BS2C_EmitByte(ctx, (lk>>48)&255);
		BS2C_EmitByte(ctx, (lk>>40)&255);
		BS2C_EmitByte(ctx, (lk>>32)&255);
		BS2C_EmitByte(ctx, (lk>>24)&255);
		BS2C_EmitByte(ctx, (lk>>16)&255);
		BS2C_EmitByte(ctx, (lk>> 8)&255);
		BS2C_EmitByte(ctx, (lk    )&255);
//		BS2C_EmitOpcodeUCx(ctx, lk);
		return;
	}
}

void BS2C_EmitOpcodeSKx(BS2CC_CompileContext *ctx, int i, s64 ix)
{
	BS2C_EmitOpcodeUKx(ctx, i, (ix<<1)^(ix>>63));
}

void BS2C_EmitOpcodeSuKx(BS2CC_CompileContext *ctx, int z, int vi, s64 ix)
{
	if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_LONG) ||
		(z==BSVM2_OPZ_SBYTE) || (z==BSVM2_OPZ_SHORT))
	{
		BS2C_EmitOpcodeSKx(ctx, vi, ix);
		return;
	}

	if((z==BSVM2_OPZ_FLOAT) || (z==BSVM2_OPZ_DOUBLE))
	{
		BS2C_EmitOpcodeFKxD(ctx, vi, ix);
		return;
	}

	BS2C_EmitOpcodeUKx(ctx, vi, ix);
}

void BS2C_EmitOpcodeFKxD(BS2CC_CompileContext *ctx, int vi, double f)
{
	u64 ix;
	s64 m;
	int e;

	ix=*(u64 *)(&f);

	m=(s64)((ix&0x000FFFFFFFFFFFFFULL)|
		0x0010000000000000ULL);
	e=((ix>>52)&2047)-(1023+52);

	while(!(m&255))
		{ m=m>>8; e=e+8; }
	while(!(m&1))
		{ m=m>>1; e++; }
	
	if(ix>>63)
		{ m=-m; }
	
	BS2C_EmitOpcodeSKx(ctx, vi, e);
	BS2C_EmitOpcodeSCx(ctx, m);
}


void BS2C_EmitOpcodeUZx(BS2CC_CompileContext *ctx, int z, u64 ix)
{
	BS2C_EmitOpcodeUCx(ctx, (ix<<4)|(z&15));
}

#if 0
void BS2C_EmitOpcodeUZx(BS2CC_CompileContext *ctx, int z, u64 ix)
{
	if(ix<8)
	{
		BS2C_EmitByte(ctx, (z<<4)|ix);
		return;
	}
	if(ix<1024)
	{
		BS2C_EmitByte(ctx, (z<<4)|8|(ix>>8));
		BS2C_EmitByte(ctx, ix);
		return;
	}
	if(ix<131072)
	{
		BS2C_EmitByte(ctx, (z<<4)|12|(ix>>16));
		BS2C_EmitByte(ctx, ix>>8);
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<(1ULL<<32))
	{
		BS2C_EmitByte(ctx, (z<<4)|14);
		BS2C_EmitByte(ctx, ix>>24);
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>> 8);
		BS2C_EmitByte(ctx, ix    );
		return;
	}

	BS2C_EmitByte(ctx, (z<<4)|15);
	BS2C_EmitByte(ctx, ix>>56);
	BS2C_EmitByte(ctx, ix>>48);
	BS2C_EmitByte(ctx, ix>>40);
	BS2C_EmitByte(ctx, ix>>32);
	BS2C_EmitByte(ctx, ix>>24);
	BS2C_EmitByte(ctx, ix>>16);
	BS2C_EmitByte(ctx, ix>> 8);
	BS2C_EmitByte(ctx, ix    );
	return;
}
#endif

void BS2C_EmitOpcodeSZx(BS2CC_CompileContext *ctx, int z, s64 ix)
{
	BS2C_EmitOpcodeUZx(ctx, z, (ix<<1)^(ix>>63));
}

void BS2C_EmitOpcodeUZy(BS2CC_CompileContext *ctx, int z, u64 ix)
{
	BS2C_EmitOpcodeUCx(ctx, (ix<<4)|(z&15));

#if 0
	int z1;

	if(z==BSVM2_OPZ_ADDRESS)
	{
		BS2C_EmitOpcodeUCx(ctx, (ix<<2)|0);
		return;
	}
	
	switch(z)
	{
	case BSVM2_OPZ_INT: z1=BSVM2_OPZY_INT; break;
	case BSVM2_OPZ_UINT: z1=BSVM2_OPZY_UINT; break;
	case BSVM2_OPZ_LONG: z1=BSVM2_OPZY_LONG; break;
	case BSVM2_OPZ_FLOAT: z1=BSVM2_OPZY_FLOAT; break;
	case BSVM2_OPZ_DOUBLE: z1=BSVM2_OPZY_DOUBLE; break;

	case BSVM2_OPZ_UBYTE: z1=BSVM2_OPZY_STRU8; break;
	case BSVM2_OPZ_SBYTE: z1=BSVM2_OPZY_STRASC; break;
	case BSVM2_OPZ_SHORT: z1=BSVM2_OPZY_STRU16; break;

	default: z1=BSVM2_OPZY_INT; break;
	}

	BS2C_EmitOpcodeUCx(ctx, (ix<<4)|(z1&15));
#endif
}

void BS2C_EmitOpcodeSZy(BS2CC_CompileContext *ctx, int z, s64 ix)
{
	BS2C_EmitOpcodeUZy(ctx, z, (ix<<1)^(ix>>63));
}

void BS2C_EmitOpcodeZyStr(BS2CC_CompileContext *ctx, char *str)
{
	int i, j;

	i=BS2C_ImgGetString(ctx, str);
//	BS2C_EmitOpcodeUZy(ctx, BSVM2_OPZ_UBYTE, i);

	j=BS2C_IndexFrameLiteral(ctx, (i<<4)|BSVM2_OPZY_STRU8);
	BS2C_EmitOpcodeUZy(ctx, BSVM2_OPZ_ADDR, j);


//	BS2C_CompileLoadNameAsType(ctx,
//		BGBDT_TagStr_GetUtf8(expr), dty);
//		BS2C_EmitOpcodeUZy(ctx, BSVM2_OPZ_UBYTE, i);
}

void BS2C_EmitOpcodeZxF(BS2CC_CompileContext *ctx, double f)
{
	BS2C_EmitOpcodeZxFI(ctx, f, BSVM2_OPZ_FLOAT);
}

void BS2C_EmitOpcodeZxFI(BS2CC_CompileContext *ctx, double f, int z)
{
	float fc;
	u32 ix;
	int m;
	int e;

	fc=f; ix=*(u32 *)(&fc);
//	BS2C_EmitByte(ctx, (BSVM2_OPZ_FLOAT<<4)|14);
//	BS2C_EmitByte(ctx, ix>>24);
//	BS2C_EmitByte(ctx, ix>>16);
//	BS2C_EmitByte(ctx, ix>> 8);
//	BS2C_EmitByte(ctx, ix    );

	m=(ix&0x007FFFFF)|0x00800000;
	e=((ix>>23)&255)-(127+23);

	while(!(m&255))
		{ m=m>>8; e=e+8; }
	while(!(m&1))
		{ m=m>>1; e++; }
	
	if(ix>>31)
		{ m=-m; }
	
	BS2C_EmitOpcodeSZx(ctx, z, e);
	BS2C_EmitOpcodeSCx(ctx, m);
}

void BS2C_EmitOpcodeZxD(BS2CC_CompileContext *ctx, double f)
{
	double er;
	float g;
	u64 ix;
	s64 m;
	int e;

	g=f; er=f-g;
	if(fabs(er<0.0000001))
	{
		BS2C_EmitOpcodeZxFI(ctx, f, BSVM2_OPZ_DOUBLE);
		return;
	}

	ix=*(u64 *)(&f);

	m=(s64)((ix&0x000FFFFFFFFFFFFFULL)|
		0x0010000000000000ULL);
	e=((ix>>52)&2047)-(1023+52);

	while(!(m&255))
		{ m=m>>8; e=e+8; }
	while(!(m&1))
		{ m=m>>1; e++; }
	
	if(ix>>63)
		{ m=-m; }
	
	BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_DOUBLE, e);
	BS2C_EmitOpcodeSCx(ctx, m);

#if 0
	BS2C_EmitByte(ctx, (BSVM2_OPZ_DOUBLE<<4)|15);
	BS2C_EmitByte(ctx, ix>>56);
	BS2C_EmitByte(ctx, ix>>48);
	BS2C_EmitByte(ctx, ix>>40);
	BS2C_EmitByte(ctx, ix>>32);
	BS2C_EmitByte(ctx, ix>>24);
	BS2C_EmitByte(ctx, ix>>16);
	BS2C_EmitByte(ctx, ix>> 8);
	BS2C_EmitByte(ctx, ix    );
#endif
}

int BS2C_GenTempLabel(BS2CC_CompileContext *ctx)
{
	int i;
	i=ctx->gsseq++;
	return(i);
}

void BS2C_EmitTempLabel(BS2CC_CompileContext *ctx, int tid)
{
	int i;
	
	i=ctx->frm->ntlbl++;
	ctx->frm->tlbl_id[i]=tid;
	ctx->frm->tlbl_ct[i]=ctx->frm->ct-ctx->frm->cts;
}

void BS2C_EmitTempReloc(BS2CC_CompileContext *ctx, int tid)
{
	int i;
	
	i=ctx->frm->ntrlc++;
	ctx->frm->trlc_id[i]=tid;
	ctx->frm->trlc_ct[i]=ctx->frm->ct-ctx->frm->cts;
}

void BS2C_EmitTempJump(BS2CC_CompileContext *ctx, int tid)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_JMP);
	BS2C_EmitWord(ctx, 0);
	BS2C_EmitTempReloc(ctx, tid);
	ctx->frm->newtrace=1;
}

void BS2C_EmitTempJAddr(BS2CC_CompileContext *ctx, int tid)
{
	BS2C_EmitWord(ctx, 0);
	BS2C_EmitTempReloc(ctx, tid);
	ctx->frm->newtrace=1;
}

void BS2C_EmitTempLabelB(BS2CC_CompileContext *ctx, int tid)
{
	if(!ctx->frm->newtrace)
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_LABEL);
		ctx->frm->newtrace=1;
	}
	BS2C_EmitTempLabel(ctx, tid);
}

int BS2C_GetNamedLabel(BS2CC_CompileContext *ctx, char *name)
{
	int i, j, k;

	for(i=0; i<ctx->frm->nnlbl; i++)
	{
		if(!strcmp(ctx->frm->nlbl_n[i], name))
		{
			j=ctx->frm->nlbl_id[i];
			return(j);
		}
	}
	
	j=BS2C_GenTempLabel(ctx);
	i=ctx->frm->nnlbl++;
	ctx->frm->nlbl_id[i]=j;
	ctx->frm->nlbl_n[i]=BS2P_StrSym(ctx, name);
	return(j);
}

void BS2C_FixupLabels(BS2CC_CompileContext *ctx)
{
	byte *ct;
	int i, j, k, l;
	
	for(i=0; i<ctx->frm->ntrlc; i++)
	{
		l=ctx->frm->trlc_id[i];
		for(j=0; j<ctx->frm->ntlbl; j++)
		{
			if(ctx->frm->tlbl_id[j]==l)
				break;
		}
		
		ct=ctx->frm->cts+ctx->frm->trlc_ct[i]-2;
		k=(ct[0]<<8)|ct[1];
		k=(k>>1)^((k<<31)>>31);
		k=k+(ctx->frm->tlbl_ct[j]-ctx->frm->trlc_ct[i]);
		k=(k<<1)^(k>>31);
		k|=0x8000;
		ct[0]=k>>8;
		ct[1]=k;
		
//		ctx->frm->trlc_id[i]=tid;
//		ctx->frm->trlc_ct[i]=ctx->frm->ct-ctx->frm->cts;
	}
}


int BS2C_ImgLookupString(BS2CC_CompileContext *ctx, char *str)
{
	char *cs;

	if(!str)
		return(0);
	if(!(*str))
		return(1);
	if(!ctx->strtab)
		return(-1);
	
	cs=ctx->strtab+2;
	while(cs<ctx->strct)
	{
		if(!strcmp(cs, str))
			return(cs-ctx->strtab);
		cs=cs+strlen(cs)+1;
	}
	return(-1);
}

int BS2C_ImgGetString(BS2CC_CompileContext *ctx, char *str)
{
	char *cs;
	int i, j, k, l;

	i=BS2C_ImgLookupString(ctx, str);
	if(i>=0)return(i);
	
	if(!ctx->strtab)
	{
		ctx->strtab=frgl_malloc(1<<16);
		ctx->strtabe=ctx->strtab+(1<<16);
		ctx->strct=ctx->strtab+2;
	}
	
	l=strlen(str);
	if((ctx->strct+l+1)>=ctx->strtabe)
	{
		i=ctx->strct-ctx->strtab;
		k=ctx->strtabe-ctx->strtab;
		while((i+l+1)>=k)
			k=k+(k>>1);
		ctx->strtab=frgl_realloc(ctx->strtab, k);
		ctx->strtabe=ctx->strtab+k;
		ctx->strct=ctx->strtab+i;
	}
	
	cs=ctx->strct;
	strcpy(cs, str);
	ctx->strct=cs+l+1;
	
	return(cs-ctx->strtab);
}

void BS2C_EmitReturnV(BS2CC_CompileContext *ctx)
{
	int cty;

	cty=ctx->frm->func->rty;

	if(cty==BSVM2_OPZ_VOID)
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETV);
		return;
	}

	if(BS2C_TypeSmallIntP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, 0);
		return;
	}

	if(BS2C_TypeSmallLongP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG, 0);
		return;
	}

	if(BS2C_TypeSmallFloatP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETF);
		return;
	}

	if(BS2C_TypeSmallDoubleP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETD);
		return;
	}

	if(BS2C_TypeAddressP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETA);
		return;
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_EmitReturnVal(BS2CC_CompileContext *ctx)
{
	int cty;

	cty=ctx->frm->func->rty;

	if(cty==BSVM2_OPZ_VOID)
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETV);
		return;
	}

	if(BS2C_TypeSmallIntP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETI);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeSmallLongP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETL);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeSmallFloatP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETF);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeSmallDoubleP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETD);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeAddressP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETA);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	BS2C_CaseError(ctx);
	return;
}


void BS2C_EmitReturnCleanupV(BS2CC_CompileContext *ctx, int last)
{
	int cty;

	cty=ctx->frm->func->rty;

	if(cty==BSVM2_OPZ_VOID)
	{
//		BS2C_EmitOpcode(ctx, BSVM2_OP_RETV);
		if(!(last&1))
			{ BS2C_EmitTempJump(ctx, ctx->frm->jcleanup); }

		return;
	}

	if(BS2C_TypeSmallIntP(ctx, cty))
	{
//		BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
//		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, 0);

		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
		BS2C_EmitTempJump(ctx, ctx->frm->jcleanup);
		BS2C_CompileNoexPop(ctx);
		return;
	}

	if(BS2C_TypeSmallLongP(ctx, cty))
	{
//		BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
//		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG, 0);

		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHL);
		BS2C_EmitTempJump(ctx, ctx->frm->jcleanup);
		BS2C_CompileNoexPop(ctx);
		return;
	}

	if(BS2C_TypeSmallFloatP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
//		BS2C_EmitOpcode(ctx, BSVM2_OP_RETF);
		BS2C_EmitTempJump(ctx, ctx->frm->jcleanup);
		BS2C_CompileNoexPop(ctx);
		return;
	}

	if(BS2C_TypeSmallDoubleP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
//		BS2C_EmitOpcode(ctx, BSVM2_OP_RETD);
		BS2C_EmitTempJump(ctx, ctx->frm->jcleanup);
		BS2C_CompileNoexPop(ctx);
		return;
	}

	if(BS2C_TypeAddressP(ctx, cty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
//		BS2C_EmitOpcode(ctx, BSVM2_OP_RETA);
		BS2C_EmitTempJump(ctx, ctx->frm->jcleanup);
		BS2C_CompileNoexPop(ctx);
		return;
	}

	BS2C_CaseError(ctx);
	return;
}
