/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


byte *BSVM2_Interp_DecodeOpJAddr(BSVM2_CodeBlock *cblk)
{
	byte *cs;
	int i;
	
	i=BSVM2_Interp_DecodeOpSCxI(cblk);
	cs=cblk->cs+i;
	return(cs);
}

void BSVM2_Interp_DecodeTopGx(BSVM2_CodeBlock *cblk, BSVM2_TailOpcode *op)
{
//	op->i0=BSVM2_Interp_DecodeOpUCxI(cblk);
	op->v.p=BSVM2_Interp_DecodeOpAddrPtr(cblk,
		BSVM2_Interp_DecodeOpUCxI(cblk));
}

void BSVM2_Interp_DecodeTopZx(BSVM2_CodeBlock *cblk, BSVM2_TailOpcode *op)
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
		BSVM2_Interp_DecodeTopFx2(cblk, op, i, op->i1);
		break;
	case BSVM2_OPZ_ADDRESS:
		op->v.a=BSVM2_Interp_DecodeOpAddrConst(cblk, lj);
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

void BSVM2_Interp_DecodeTopFx2(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op, int exp, int zty)
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
	lj=(li&0x000FFFFFFFFFFFFFULL)|(((u64)e)<<52)|(((u64)sg)<<63);
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

void BSVM2_Interp_DecodeTopCi(BSVM2_CodeBlock *cblk, BSVM2_TailOpcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	lj=bsvm2_interp_decsignfoldl(li);
	op->v.i=lj;
}

void BSVM2_Interp_DecodeTopZn(BSVM2_CodeBlock *cblk, BSVM2_TailOpcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i1=li&15;
	lj=li>>4;
	op->i0=lj;
}

void BSVM2_Interp_DecodeTopIx(BSVM2_CodeBlock *cblk, BSVM2_TailOpcode *op)
{
	int i, j;

	i=BSVM2_Interp_DecodeOpUCxI(cblk);
	op->i0=i;
}

void BSVM2_Interp_DecodeTopJx(BSVM2_CodeBlock *cblk, BSVM2_TailOpcode *op)
{
	int i, j;

	BSVM2_Interp_DecodeOpUJxI(cblk, &i, &j);
	op->i0=i;
	op->i1=j;
}

void BSVM2_Interp_DecodeTopGj(BSVM2_CodeBlock *cblk, BSVM2_TailOpcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	int i, j, k, ix;

	BSVM2_Interp_DecodeOpUJxI(cblk, &i, &j);

	op->i0=i;
	ix=j;
	k=cblk->gitab[ix];
	if(!(k&3))
	{
		vi=BSVM2_Interp_DecodeOpAddrPtr(cblk, ix);
		op->v.p=vi;
		op->i1=vi->brty;
	}else
	{
		p=BSVM2_Interp_DecodeOpAddrPtr(cblk, ix);
		op->v.p=p;
		op->i1=BSVM2_OPZ_CONST;
	}
}

void BSVM2_Interp_SetupTopPopUnJmp(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->jcs=BSVM2_Interp_DecodeOpJAddr(cblk);

	op->Run=run;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupTopPopUn(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->Run=run;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupTopUatJmp(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->jcs=BSVM2_Interp_DecodeOpJAddr(cblk);
	op->Run=run;
//	op->t0=cblk->stkpos;
}

void BSVM2_Interp_SetupTopUat(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->Run=run;
//	op->t0=cblk->stkpos;
}

void BSVM2_Interp_SetupTopUnJmp(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->jcs=BSVM2_Interp_DecodeOpJAddr(cblk);
	op->Run=run;
	op->t0=cblk->stkpos-1;
}

void BSVM2_Interp_SetupTopUn(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos-1;
}

void BSVM2_Interp_SetupTopUnPJmp(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->jcs=BSVM2_Interp_DecodeOpJAddr(cblk);
	op->Run=run;
	op->t0=cblk->stkpos++;
}

void BSVM2_Interp_SetupTopPopBinJmp(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->jcs=BSVM2_Interp_DecodeOpJAddr(cblk);

	op->Run=run;
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupTopPopBin(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->Run=run;
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupTopJCMP(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op)
{
	int z, o;
	int i, j;
	
	i=*cblk->cs++;
	z=(i>>4)&15;
	o=i&15;

	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
	
	switch(z)
	{
	case BSVM2_OPZ_INT:
	case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQI; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEI; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTI; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTI; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEI; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEI; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQI; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEI; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_UINT:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQI; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEI; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTUI; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTUI; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEUI; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEUI; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQI; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEI; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_LONG:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQL; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEL; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTL; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTL; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEL; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEL; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQL; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEL; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_ULONG:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQL; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEL; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTUL; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTUL; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEUL; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEUL; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQL; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEL; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_FLOAT:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQF; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEF; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTF; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTF; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEF; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEF; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQF; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEF; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_DOUBLE:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQD; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NED; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTD; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTD; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LED; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GED; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQD; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NED; break;
		default:
			break;
		}
		break;
	}
}

void BSVM2_Interp_SetupTopCallG(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	char *s;
	int ix;
	int i, j;
	
	ix=BSVM2_Interp_DecodeOpUCxI(cblk);
	if((ix<0) || (ix>=cblk->ngi))
		{ BSVM2_DBGTRAP }
	i=cblk->gitab[ix];
	if(i&3)
	{
		return;
	}

	vi=BS2I_ImageGetGlobal(cblk->img, i>>2);
	op->v.p=vi;
	
	cblk->stkpos-=vi->nargs;
	op->t1=cblk->stkpos;

	if(vi->brty!=BSVM2_OPZ_VOID)
		{ op->t0=cblk->stkpos++; }
	else
		{ op->t0=cblk->stkpos; }

	if(vi->cblk)
	{
		op->Run=BSVM2_TrOp_CALLG;
		return;
	}
	
	p=BSVM2_NatCall_GetProcAddress(vi->name);
	
	if(!p)
	{
		printf("BSVM2_Interp_SetupTopCallG: No Function %s\n",
			vi->name);
		op->Run=BSVM2_TrOp_NatCallFail;
		return;
	}
	
	i=BSVM2_NatCall_GetSigIndexG0(vi->sig);
	if(i>=0)
	{
		op->v.p=p;
		op->i1=i;
		op->Run=BSVM2_TrOp_NatCallG0;
		return;
	}

//	op->v.p=p;
//	op->i1=i;
	op->Run=BSVM2_TrOp_NatCallG1;
	return;
}

void BSVM2_Interp_SetupTopCallV(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	char *s;
	int ix;
	int i, j;
	
	ix=BSVM2_Interp_DecodeOpUCxI(cblk);
	if((ix<0) || (ix>=cblk->ngi))
		{ BSVM2_DBGTRAP }
	i=cblk->gitab[ix];
	if(i&3)
	{
		return;
	}

	vi=BS2I_ImageGetGlobal(cblk->img, i>>2);
	op->v.p=vi;
	
	cblk->stkpos-=vi->nargs;
	op->t1=cblk->stkpos;

	cblk->stkpos--;		//pop object

	if(vi->brty!=BSVM2_OPZ_VOID)
		{ op->t0=cblk->stkpos++; }
	else
		{ op->t0=cblk->stkpos; }

	op->Run=BSVM2_TrOp_CALLV;
	return;
}

void BSVM2_Interp_SetupTopCallTh(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	char *s;
	int ix;
	int i, j;
	
	ix=BSVM2_Interp_DecodeOpUCxI(cblk);
	if((ix<0) || (ix>=cblk->ngi))
		{ BSVM2_DBGTRAP }
	i=cblk->gitab[ix];
	if(i&3)
	{
		return;
	}

	vi=BS2I_ImageGetGlobal(cblk->img, i>>2);
	op->v.p=vi;
	
	cblk->stkpos-=vi->nargs;
	op->t1=cblk->stkpos;

	if(vi->brty!=BSVM2_OPZ_VOID)
		{ op->t0=cblk->stkpos++; }
	else
		{ op->t0=cblk->stkpos; }

	op->Run=BSVM2_TrOp_CALLTH;
	return;
}


void BSVM2_Interp_SetupTopCallA(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	char *s;
	int ix;
	int i, j;
	
	ix=BSVM2_Interp_DecodeOpUCxI(cblk);
	if((ix<0) || (ix>=cblk->ngi))
		{ BSVM2_DBGTRAP }
	i=cblk->gitab[ix];
	if(i&3)
	{
		return;
	}

	vi=BS2I_ImageGetGlobal(cblk->img, i>>2);
	op->v.p=vi;
	
	cblk->stkpos-=vi->nargs;
	op->t1=cblk->stkpos;
//	op->t2=cblk->stkpos;

	cblk->stkpos--;		//pop func

	if(vi->brty!=BSVM2_OPZ_VOID)
		{ op->t0=cblk->stkpos++; }
	else
		{ op->t0=cblk->stkpos; }

	op->Run=BSVM2_TrOp_CALLA;
	return;
}

void BSVM2_Interp_SetupTopCallL(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	char *s;
	int ix;
	int i, j;
	
	BSVM2_Interp_DecodeOpUJxI(cblk, &j, &ix);
	if((ix<0) || (ix>=cblk->ngi))
		{ BSVM2_DBGTRAP }
	i=cblk->gitab[ix];
	if(i&3)
	{
		BSVM2_DBGTRAP
		return;
	}
	
	op->i0=j;

	vi=BS2I_ImageGetGlobal(cblk->img, i>>2);
	op->v.p=vi;
	
	cblk->stkpos-=vi->nargs;
	op->t1=cblk->stkpos;
//	op->t2=cblk->stkpos;

	if(vi->brty!=BSVM2_OPZ_VOID)
		{ op->t0=cblk->stkpos++; }
	else
		{ op->t0=cblk->stkpos; }

	op->Run=BSVM2_TrOp_CALLL;
	return;
}


BSVM2_TailOpcode *BSVM2_Interp_DecodeTailOpcode(
	BSVM2_CodeBlock *cblk, int opn)
{
	BSVM2_TailOpcode *tmp;
	int opn2;
	int i;
	
	tmp=BSVM2_Interp_AllocTailOpcode(cblk);
	tmp->opn=opn;
	
	switch(opn)
	{
	case BSVM2_OP_CMPI:
		opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
		tmp->opn2=opn2;
		switch(opn2)
		{
		case BSVM2_OP_JEQ:
			BSVM2_Interp_SetupTopPopBinJmp(cblk, tmp, BSVM2_TrOp_JCMP_EQI);
			break;
		case BSVM2_OP_JNE:
			BSVM2_Interp_SetupTopPopBinJmp(cblk, tmp, BSVM2_TrOp_JCMP_NEI);
			break;
		case BSVM2_OP_JLT:
			BSVM2_Interp_SetupTopPopBinJmp(cblk, tmp, BSVM2_TrOp_JCMP_LTI);
			break;
		case BSVM2_OP_JGT:
			BSVM2_Interp_SetupTopPopBinJmp(cblk, tmp, BSVM2_TrOp_JCMP_GTI);
			break;
		case BSVM2_OP_JLE:
			BSVM2_Interp_SetupTopPopBinJmp(cblk, tmp, BSVM2_TrOp_JCMP_LEI);
			break;
		case BSVM2_OP_JGE:
			BSVM2_Interp_SetupTopPopBinJmp(cblk, tmp, BSVM2_TrOp_JCMP_GEI);
			break;
		}
		break;
	case BSVM2_OP_CMPILL:
		BSVM2_Interp_DecodeTopJx(cblk, tmp);
		opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
		tmp->opn2=opn2;

		switch(opn2)
		{
		case BSVM2_OP_JEQ:
			BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JCMPLL_EQI);
			break;
		case BSVM2_OP_JNE:
			BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JCMPLL_NEI);
			break;
		case BSVM2_OP_JLT:
			BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JCMPLL_LTI);
			break;
		case BSVM2_OP_JGT:
			BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JCMPLL_GTI);
			break;
		case BSVM2_OP_JLE:
			BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JCMPLL_LEI);
			break;
		case BSVM2_OP_JGE:
			BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JCMPLL_GEI);
			break;
		}
		break;

	case BSVM2_OP_JEQ:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JEQ);
		break;
	case BSVM2_OP_JNE:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JNE);
		break;
	case BSVM2_OP_JLT:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JLT);
		break;
	case BSVM2_OP_JGT:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JGT);
		break;
	case BSVM2_OP_JLE:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JLE);
		break;
	case BSVM2_OP_JGE:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JGE);
		break;
//	case BSVM2_OP_JCMP:
//		BSVM2_Interp_SetupTopJCMP(cblk, tmp);
//		break;
	case BSVM2_OP_JMP:
//		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JMP);
		BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JMP);
		break;

	case BSVM2_OP_JEQNULL:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JEQNULL);
		break;
	case BSVM2_OP_JNENULL:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JNENULL);
		break;
	case BSVM2_OP_JEQNULLL:
		BSVM2_Interp_DecodeTopIx(cblk, tmp);
		BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JEQNULLL);
		break;
	case BSVM2_OP_JNENULLL:
		BSVM2_Interp_DecodeTopIx(cblk, tmp);
		BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_JNENULLL);
		break;

	case BSVM2_OP_DCJEQIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_EQIC);
		break;
	case BSVM2_OP_DCJNEIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_NEIC);
		break;
	case BSVM2_OP_DCJLTIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_LTIC);
		break;
	case BSVM2_OP_DCJGTIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_GTIC);
		break;
	case BSVM2_OP_DCJLEIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_LEIC);
		break;
	case BSVM2_OP_DCJGEIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_GEIC);
		break;

	case BSVM2_OP_CJEQIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_EQIC);
		break;
	case BSVM2_OP_CJNEIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_NEIC);
		break;
	case BSVM2_OP_CJLTIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_LTIC);
		break;
	case BSVM2_OP_CJGTIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_GTIC);
		break;
	case BSVM2_OP_CJLEIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_LEIC);
		break;
	case BSVM2_OP_CJGEIC:
		BSVM2_Interp_DecodeTopCi(cblk, tmp);
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JCMP_GEIC);
		break;

	case BSVM2_OP_RETI:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_RETI);
		break;
	case BSVM2_OP_RETL:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_RETL);
		break;
	case BSVM2_OP_RETF:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_RETF);
		break;
	case BSVM2_OP_RETD:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_RETD);
		break;
	case BSVM2_OP_RETA:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_RETA);
		break;
	case BSVM2_OP_RETV:
		BSVM2_Interp_SetupTopUat(cblk, tmp, BSVM2_TrOp_RETV);
		break;

	case BSVM2_OP_RETC:
		BSVM2_Interp_DecodeTopZx(cblk, tmp);
		switch(tmp->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETIC);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETLC);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETFC);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETDC);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETAC);
			break;
		default:
			break;
		}
		break;

	case BSVM2_OP_RET2:
		BSVM2_Interp_DecodeTopZn(cblk, tmp);
		switch(tmp->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETIL);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETLL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETFL);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETDL);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupTopUat(cblk, tmp,
				BSVM2_TrOp_RETAL);
			break;
		default:
			break;
		}
		break;

	case BSVM2_OP_CALLG:
		BSVM2_Interp_SetupTopCallG(cblk, tmp);
		break;
	case BSVM2_OP_CALLV:
		BSVM2_Interp_SetupTopCallV(cblk, tmp);
		break;
	case BSVM2_OP_CALLTH:
		BSVM2_Interp_SetupTopCallTh(cblk, tmp);
		break;

	case BSVM2_OP_CALLA:
		BSVM2_Interp_SetupTopCallA(cblk, tmp);
		break;
	case BSVM2_OP_CALLL:
		BSVM2_Interp_SetupTopCallL(cblk, tmp);
		break;

	case BSVM2_OP_THROW:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_THROW);
		break;
	case BSVM2_OP_RETHROW:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_RETHROW);
		break;

	case BSVM2_OP_BEGTRY:
		BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_BEGTRY);
		break;
	case BSVM2_OP_ENDTRY:
		BSVM2_Interp_SetupTopUnPJmp(cblk, tmp, BSVM2_TrOp_ENDTRY);
		break;
	case BSVM2_OP_CATCH:
		BSVM2_Interp_DecodeTopGx(cblk, tmp);
		BSVM2_Interp_SetupTopUnJmp(cblk, tmp, BSVM2_TrOp_CATCH);
		break;

	case BSVM2_OP_BEGLEC:
		BSVM2_Interp_DecodeTopIx(cblk, tmp);
		BSVM2_Interp_SetupTopUatJmp(cblk, tmp, BSVM2_TrOp_BEGLEC);
		break;
	case BSVM2_OP_ENDLEC:
		BSVM2_Interp_DecodeTopIx(cblk, tmp);
		BSVM2_Interp_SetupTopUat(cblk, tmp, BSVM2_TrOp_ENDLEC);
		break;
	case BSVM2_OP_ENDLEC2:
		BSVM2_Interp_DecodeTopIx(cblk, tmp);
		BSVM2_Interp_SetupTopUnPJmp(cblk, tmp, BSVM2_TrOp_ENDLEC2);
		break;

	default:
		BSVM2_Interp_FreeTailOpcode(cblk, tmp);
		tmp=NULL;
		break;
	}

	return(tmp);
}
