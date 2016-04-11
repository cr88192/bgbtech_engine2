#include <bteifgl.h>

int BSVM2_Interp_DecodeOpUCxI(BSVM2_CodeBlock *cblk)
{
	int i;
	i=*cblk->cs++;
	if(i<0x80)
	{
		return(i);
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		return(i);
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}

	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}

	if(i<0xF8)
	{
		i=((i&0x0F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}
}

u64 BSVM2_Interp_DecodeOpUCxL(BSVM2_CodeBlock *cblk)
{
	s64 li;
	int i;

	i=*cblk->cs++;
	if(i<0x80)
	{
		return(i);
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		return(i);
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}

	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}

	if(i<0xF8)
	{
		li=((i&0x0F)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		return(li);
	}

	if(i<0xFC)
	{
		li=((i&0x07)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		return(li);
	}
	if(i<0xFE)
	{
		li=((i&0x03)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		return(li);
	}

	if(i==0xFE)
	{
		li=*cblk->cs++;
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		return(li);
	}

	li=*cblk->cs++;
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	return(li);
}

s32 BSVM2_Interp_DecodeOpSCxI(BSVM2_CodeBlock *cblk)
{
	s32 i;
	
	i=BSVM2_Interp_DecodeOpUCxI(cblk);
	i=(((u32)i)>>1)^((i<<31)>>31);
	return(i);
}

s64 BSVM2_Interp_DecodeOpSCxL(BSVM2_CodeBlock *cblk)
{
	s64 li;
	int i;
	
	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	li=(((u64)li)>>1)^((li<<63)>>63);
	return(li);
}

void BSVM2_Interp_DecodeOpUJxI(BSVM2_CodeBlock *cblk,
	int *ri, int *rj)
{
	int i, j;

	i=*cblk->cs++;
	if(i<0x80)
	{
		*ri=(i>>4)&7;
		*rj=i&15;
		return;
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		*ri=(i>>7)&127;
		*rj=(i   )&127;
		return;
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);

		*ri=(i>>11)&1023;
		*rj=(i    )&2047;
		return;
	}

	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>14)&16383;
		*rj=(i    )&16383;
		return;
	}

	if(i<0xF8)
	{
		i=((i&0x0F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>16)&65535;
		*rj=(i    )&65535;
		return;
	}
}

void BSVM2_Interp_DecodeOpUKx(BSVM2_CodeBlock *cblk,
	int *ri, u64 *rj)
{
	u64 li;
	int i, j;

	i=*cblk->cs++;
	if(i<0x80)
	{
		*ri=(i>>4)&7;
		*rj=i&15;
		return;
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		*ri=(i>>9)&31;
		*rj=(i   )&511;
		return;
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>13)&255;
		*rj=(i    )&8191;
		return;
	}
	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>20)&255;
		*rj=(i    )&((1<<20)-1);
		return;
	}
	if(i<0xF8)
	{
		li=((i&0x0F)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		*ri=(li>>27)&255;
		*rj=li&((1<<27)-1);
		return;
	}
	if(i<0xFC)
	{
		li=((i&0x07)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		*ri=(li>>32)&1023;
		*rj=li&((1LL<<32)-1);
		return;
	}
	if(i<0xFE)
	{
		li=((i&0x03)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);

		*ri=(li>>39)&1023;
		*rj=li&((1LL<<39)-1);
		return;
	}
	if(i==0xFE)
	{
		li=*cblk->cs++;
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);

		*ri=(li>>44)&4095;
		*rj=li&((1LL<<44)-1);
		return;
	}
	if(i==0xFF)
	{
		li=*cblk->cs++;
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);

		*ri=(li>>52)&4095;
		*rj=li&((1LL<<52)-1);
		return;
	}
}

s64 bsvm2_interp_decsignfoldl(s64 i)
	{ return((((u64)i)>>1)^((i<<63)>>63)); }

void BSVM2_Interp_DecodeOpIx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
	{ op->i0=BSVM2_Interp_DecodeOpUCxI(cblk); }

void BSVM2_Interp_DecodeOpJx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	int i, j;

	BSVM2_Interp_DecodeOpUJxI(cblk, &i, &j);
	op->i0=i;
	op->i1=j;
}

void BSVM2_Interp_DecodeOpKx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty)
{
	u64 li;
	int i, j;

	BSVM2_Interp_DecodeOpUKx(cblk, &j, &li);
	op->i0=j;

	switch(zty)
	{
	case BSVM2_OPZ_INT:	case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_SHORT:
		op->v.i=bsvm2_interp_decsignfoldl(li); break;
	case BSVM2_OPZ_LONG:
		op->v.l=bsvm2_interp_decsignfoldl(li); break;

	case BSVM2_OPZ_FLOAT:
	case BSVM2_OPZ_DOUBLE:
		i=bsvm2_interp_decsignfoldl(li);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, zty);
		break;

	case BSVM2_OPZ_ADDRESS:
		break;
	case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_USHORT:
		op->v.i=li; break;
	case BSVM2_OPZ_ULONG:
		op->v.l=li; break;
	default:
		break;
	}
}

void BSVM2_Interp_DecodeOpFx2(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op,
	int exp, int zty)
{
	double f;
	u32 vi;
	u64 lj;
	s64 li;
	int i, j, k, e, sg;

	e=exp;
	li=BSVM2_Interp_DecodeOpSCxL(cblk);

	sg=0;
	if(li<0)
		{ li=-li; sg=1; }

	while(!(li>>32))
		{ li=(li<<16); e-=16; }
	while(!(li>>44))
		{ li=(li<<8); e-=8; }
	while(!(li>>52))
		{ li=(li<<1); e--; }

	e=e+1023;
	lj=(li&0x000FFFFFFFFFFFFFULL)|(e<<52)|(sg<<63);
	f=*(double *)(&lj);

	switch(zty)
	{
	case BSVM2_OPZ_FLOAT:
		op->v.f=f; break;
	case BSVM2_OPZ_DOUBLE:
		op->v.d=f; break;
	default:
		op->v.l=0; break;
		break;
	}
}

void BSVM2_Interp_DecodeOpZx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i1=li&15;
	lj=li>>4;

	switch(op->i1)
	{
	case BSVM2_OPZ_INT:
	case BSVM2_OPZ_SBYTE:
	case BSVM2_OPZ_SHORT:
		op->v.i=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_LONG:
		op->v.l=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_FLOAT:
	case BSVM2_OPZ_DOUBLE:
		i=bsvm2_interp_decsignfoldl(lj);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, op->i1);
		break;
	case BSVM2_OPZ_ADDRESS:
		break;
	case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_USHORT:
		op->v.i=lj; break;
	case BSVM2_OPZ_ULONG:
		op->v.ul=lj; break;
	default:
		break;
	}
}

void BSVM2_Interp_DecodeOpZy(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i1=li&15;
	lj=li>>4;
	
	switch(op->i1)
	{
	case BSVM2_OPZ_INT:
		op->v.a=dtvWrapInt(bsvm2_interp_decsignfoldl(lj));
		break;
	case BSVM2_OPZ_UINT:
		op->v.a=dtvWrapUInt(lj);
		break;
	case BSVM2_OPZ_LONG:
		op->v.a=dtvWrapLong(bsvm2_interp_decsignfoldl(lj));
		break;
	case BSVM2_OPZ_ULONG:
		op->v.a=dtvWrapLong(lj);
		break;
	case BSVM2_OPZ_FLOAT:
		i=bsvm2_interp_decsignfoldl(lj);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, op->i1);
		op->v.a=dtvWrapFloat(op->v.f);
		break;
	case BSVM2_OPZ_DOUBLE:
		i=bsvm2_interp_decsignfoldl(lj);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, op->i1);
		op->v.a=dtvWrapDouble(op->v.d);
		break;

	case BSVM2_OPZ_SBYTE:
		op->v.a=BGBDT_TagStr_StringAsc(cblk->img->strtab+op->v.i);
		break;
	case BSVM2_OPZ_UBYTE:
		op->v.a=BGBDT_TagStr_String(cblk->img->strtab+op->v.i);
		break;

	case BSVM2_OPZ_USHORT:
	case BSVM2_OPZ_SHORT:
		op->v.a=BGBDT_TagStr_String16u8(cblk->img->strtab+op->v.i);
		break;

	case BSVM2_OPZ_ADDRESS:
		break;

	default:
		break;
	}
}

void BSVM2_Interp_DecodeOpFx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op,
	int zty)
{
	double f;
	u32 vi;
	u64 lj;
	s64 li;
	int i, j, k, e, sg;

	e=BSVM2_Interp_DecodeOpSCxI(cblk);
	li=BSVM2_Interp_DecodeOpSCxL(cblk);

	sg=0;
	if(li<0)
		{ li=-li; sg=1; }

	while(!(li>>32))
		{ li=(li<<16); e-=16; }
	while(!(li>>44))
		{ li=(li<<8); e-=8; }
	while(!(li>>52))
		{ li=(li<<1); e--; }

	e=e+1023;
	lj=(li&0x000FFFFFFFFFFFFFULL)|(e<<52)|(sg<<63);
	f=*(double *)(&lj);

	switch(zty)
	{
	case BSVM2_OPZ_FLOAT:
		op->v.f=f; break;
	case BSVM2_OPZ_DOUBLE:
		op->v.d=f; break;
	default:
		op->v.l=0; break;
		break;
	}
}

void BSVM2_Interp_DecodeOpCx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op,
	int zty)
{
	s64 lj;
	int i, j;

	if((zty==BSVM2_OPZ_FLOAT) || (zty==BSVM2_OPZ_DOUBLE))
	{
		BSVM2_Interp_DecodeOpFx(cblk, op, zty);
		break;
	}

	lj=BSVM2_Interp_DecodeOpUCxL(cblk);

	switch(zty)
	{
	case BSVM2_OPZ_INT:
	case BSVM2_OPZ_SBYTE:
	case BSVM2_OPZ_SHORT:
		op->v.i=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_LONG:
		op->v.l=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_FLOAT:
	case BSVM2_OPZ_DOUBLE:
		i=bsvm2_interp_decsignfoldl(lj);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, zty);
		break;
	case BSVM2_OPZ_ADDRESS:
		break;
	case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_USHORT:
		op->v.i=lj; break;
	case BSVM2_OPZ_ULONG:
		op->v.ul=lj; break;
	default:
		break;
	}
}


void BSVM2_Interp_SetupOpBin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;

	cblk->stkpos--;
	op->t0=cblk->stkpos-1;
	op->t1=cblk->stkpos;
}

void BSVM2_Interp_SetupOpUn(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos-1;
}

void BSVM2_Interp_SetupOpUnL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos++;
	BSVM2_Interp_DecodeOpIx(cblk, op);
}

void BSVM2_Interp_SetupOpUstL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	cblk->stkpos--;
	op->t0=cblk->stkpos;
	BSVM2_Interp_DecodeOpIx(cblk, op);
}

void BSVM2_Interp_SetupOpUnP(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t1=cblk->stkpos-1;
	op->t0=cblk->stkpos++;
}

void BSVM2_Interp_SetupOpUat(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
}

void BSVM2_Interp_SetupOpUatL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpIx(cblk, op);
}

void BSVM2_Interp_SetupOpUatJx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_DecodeOpJx(cblk, op);
	op->Run=run;
}

void BSVM2_Interp_SetupOpUatKx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_DecodeOpKx(cblk, op, zty);
	op->Run=run;
}

void BSVM2_Interp_SetupOpUat2(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos-1;
	op->t1=cblk->stkpos-2;
}

void BSVM2_Interp_SetupOpPopUn(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupOpPopBin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupOpPopTrin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t2=--cblk->stkpos;
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupOpBinC2(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t0=cblk->stkpos-1;
}

void BSVM2_Interp_SetupOpBinC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpBinC2(cblk, op, zty, zty, run);
}

void BSVM2_Interp_SetupOpBinCI(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpBinC2(cblk, op, zty, BSVM2_OPZ_INT, run);
}

void BSVM2_Interp_SetupOpBinCJ(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpBinC2(cblk, op, zty, BSVM2_OPZ_UINT, run);
}

void BSVM2_Interp_SetupOpBinCiSa(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op),
	void (*run2)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, BSVM2_OPZ_INT);
	op->t0=cblk->stkpos-1;

	if(op->v.i<0)
	{
		op->v.i=-op->v.i;
		op->Run=run2;
	}
}

void BSVM2_Interp_SetupOpBinL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpIx(cblk, op);
	op->t0=cblk->stkpos-1;
}

void BSVM2_Interp_SetupOpBinLC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpKx(cblk, op);
	op->t0=cblk->stkpos++;
}

void BSVM2_Interp_SetupOpBinLL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpJx(cblk, op);
	op->t0=cblk->stkpos++;
}

void BSVM2_Interp_SetupOpBinC2P(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t1=cblk->stkpos-1;
	op->t0=cblk->stkpos++;
}


void BSVM2_Interp_SetupOpTrinC2(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t1=--cblk->stkpos;
	op->t0=cblk->stkpos-1;
}

void BSVM2_Interp_SetupOpTrinCiSa(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op),
	void (*run2)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, BSVM2_OPZ_INT);
	op->t1=--cblk->stkpos;
	op->t0=cblk->stkpos-1;
	
	if(op->v.i<0)
	{
		op->v.i=-op->v.i;
		op->Run=run2;
	}
}

void BSVM2_Interp_SetupOpTrinC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpTrinC2(cblk, op, zty, zty, run);
}

void BSVM2_Interp_SetupOpTrinCI(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpTrinC2(cblk, op, zty, BSVM2_OPZ_INT, run);
}

void BSVM2_Interp_SetupOpPopTrinC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupOpPopTrinCI(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpPopTrinC2(cblk, op, BSVM2_OPZ_INT, run);
}
