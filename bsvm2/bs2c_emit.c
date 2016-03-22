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
	}

	if(ix<0x40000000000ULL)
	{
		BS2C_EmitByte(ctx, 0xF8|(ix>>40));
		BS2C_EmitByte(ctx, ix>>32);
		BS2C_EmitByte(ctx, ix>>24);
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>> 8);
		BS2C_EmitByte(ctx, ix    );
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
	}

	if(ix<0x100000000000000ULL)
	{
		BS2C_EmitByte(ctx, 0xFE|(ix>>56));
		BS2C_EmitByte(ctx, ix>>48);
		BS2C_EmitByte(ctx, ix>>40);
		BS2C_EmitByte(ctx, ix>>32);
		BS2C_EmitByte(ctx, ix>>24);
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>> 8);
		BS2C_EmitByte(ctx, ix    );
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

void BS2C_EmitOpcodeZxF(BS2CC_CompileContext *ctx, double f)
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
	
	BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_FLOAT, e);
	BS2C_EmitOpcodeSCx(ctx, m);
}

void BS2C_EmitOpcodeZxD(BS2CC_CompileContext *ctx, double f)
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
