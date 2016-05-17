#include <bteifgl.h>

void BSVM2_Op_ADDI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i+=frm->stack[op->t1].i; }
void BSVM2_Op_SUBI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i-=frm->stack[op->t1].i; }
void BSVM2_Op_MULI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i*=frm->stack[op->t1].i; }

void BSVM2_Op_ADDL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l+=frm->stack[op->t1].l; }
void BSVM2_Op_SUBL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l-=frm->stack[op->t1].l; }
void BSVM2_Op_MULL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l*=frm->stack[op->t1].l; }

void BSVM2_Op_ANDI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i&=frm->stack[op->t1].i; }
void BSVM2_Op_ORI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i|=frm->stack[op->t1].i; }
void BSVM2_Op_XORI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i^=frm->stack[op->t1].i; }

void BSVM2_Op_ANDL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l&=frm->stack[op->t1].l; }
void BSVM2_Op_ORL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l|=frm->stack[op->t1].l; }
void BSVM2_Op_XORL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l^=frm->stack[op->t1].l; }

void BSVM2_Op_SHLI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->stack[op->t0].i;
	j=frm->stack[op->t1].i;
	if(j>=32)
		{ i=0; }
	else
		{ i=i<<j; }
	frm->stack[op->t0].i=i;
}

void BSVM2_Op_SARI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->stack[op->t0].i;
	j=frm->stack[op->t1].i;
	if(j>=32)
		{ i=((i>=0)?0:(-1)); }
	else
		{ i=i>>j; }
	frm->stack[op->t0].i=i;
}

void BSVM2_Op_SHRI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->stack[op->t0].i;
	j=frm->stack[op->t1].i;
	if(j>=32)
		{ i=0; }
	else
		{ i=((u32)i)>>j; }
	frm->stack[op->t0].i=i;
}

void BSVM2_Op_SHLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i, j;

	i=frm->stack[op->t0].l;
	j=frm->stack[op->t1].l;
	if(j>=64)
		{ i=0; }
	else
		{ i=i<<j; }
	frm->stack[op->t0].l=i;
}

void BSVM2_Op_SARL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i, j;

	i=frm->stack[op->t0].l;
	j=frm->stack[op->t1].l;
	if(j>=64)
		{ i=((i>=0)?0:(-1)); }
	else
		{ i=i>>j; }
	frm->stack[op->t0].l=i;
}

void BSVM2_Op_SHRL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i, j;

	i=frm->stack[op->t0].l;
	j=frm->stack[op->t1].l;
	if(j>=64)
		{ i=0; }
	else
		{ i=((u64)i)>>j; }
	frm->stack[op->t0].l=i;
}

void BSVM2_Op_DIVI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->stack[op->t0].i;
	j=frm->stack[op->t1].i;
	if(!j)
		{ i=0; }
	else
		{ i=i/j; }
	frm->stack[op->t0].i=i;
}

void BSVM2_Op_MODI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->stack[op->t0].i;
	j=frm->stack[op->t1].i;
	if(!j)
		{ i=0; }
	else
		{ i=i%j; }
	frm->stack[op->t0].i=i;
}

void BSVM2_Op_UDIVI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u32 i, j;

	i=frm->stack[op->t0].ui;
	j=frm->stack[op->t1].ui;
	if(!j)
		{ i=0; }
	else
		{ i=i/j; }
	frm->stack[op->t0].ui=i;
}

void BSVM2_Op_DIVL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i, j;

	i=frm->stack[op->t0].l;
	j=frm->stack[op->t1].l;
	if(!j)
		{ i=0; }
	else
		{ i=i/j; }
	frm->stack[op->t0].l=i;
}

void BSVM2_Op_MODL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i, j;

	i=frm->stack[op->t0].l;
	j=frm->stack[op->t1].l;
	if(!j)
		{ i=0; }
	else
		{ i=i%j; }
	frm->stack[op->t0].l=i;
}

void BSVM2_Op_UDIVL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u64 i, j;

	i=frm->stack[op->t0].ul;
	j=frm->stack[op->t1].ul;
	if(!j)
		{ i=0; }
	else
		{ i=i/j; }
	frm->stack[op->t0].ul=i;
}

void BSVM2_Op_ADDF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f+=frm->stack[op->t1].f; }
void BSVM2_Op_SUBF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f-=frm->stack[op->t1].f; }
void BSVM2_Op_MULF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f*=frm->stack[op->t1].f; }
void BSVM2_Op_DIVF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f/=frm->stack[op->t1].f; }

void BSVM2_Op_ADDD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d+=frm->stack[op->t1].d; }
void BSVM2_Op_SUBD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d-=frm->stack[op->t1].d; }
void BSVM2_Op_MULD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d*=frm->stack[op->t1].d; }
void BSVM2_Op_DIVD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d/=frm->stack[op->t1].d; }


void BSVM2_Op_NEGI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=-frm->stack[op->t0].i; }
void BSVM2_Op_NEGL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=-frm->stack[op->t0].l; }
void BSVM2_Op_NEGF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=-frm->stack[op->t0].f; }
void BSVM2_Op_NEGD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=-frm->stack[op->t0].d; }

void BSVM2_Op_NOTI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=~frm->stack[op->t0].i; }
void BSVM2_Op_NOTL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=~frm->stack[op->t0].l; }

void BSVM2_Op_LNTI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=!frm->stack[op->t0].i; }
void BSVM2_Op_LNTL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=!frm->stack[op->t0].l; }


void BSVM2_Op_CMPI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->stack[op->t0].i;
	b=frm->stack[op->t1].i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 a, b;
	a=frm->stack[op->t0].l;
	b=frm->stack[op->t1].l;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	f32 a, b;
	a=frm->stack[op->t0].f;
	b=frm->stack[op->t1].f;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	f64 a, b;
	a=frm->stack[op->t0].d;
	b=frm->stack[op->t1].d;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMP2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	f32 a, b;
	a=frm->stack[op->t0].f;
	b=frm->stack[op->t1].f;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMP2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	f64 a, b;
	a=frm->stack[op->t0].d;
	b=frm->stack[op->t1].d;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u64 a, b;
	a=frm->stack[op->t0].ul;
	b=frm->stack[op->t1].ul;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMP2A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u64 a, b;
	a=frm->stack[op->t0].ul;
	b=frm->stack[op->t1].ul;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_UCMPI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u32 a, b;
	a=frm->stack[op->t0].ui;
	b=frm->stack[op->t1].ui;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_UCMPL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u64 a, b;
	a=frm->stack[op->t0].ul;
	b=frm->stack[op->t1].ul;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->stack[op->t0].i;
	b=op->v.i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->stack[op->t0].i;
	b=frm->local[op->i0].i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->local[op->i0].i;
	b=op->v.i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->local[op->i0].i;
	b=frm->local[op->i1].i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPILL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->local[op->i0].i;
	b=frm->local[op->i1].i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMPILC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->local[op->i0].i;
	b=op->v.i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}


void BSVM2_Op_DCMPIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->stack[op->t1].i;
	b=op->v.i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

void BSVM2_Op_CMIEQ(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i==0); }
void BSVM2_Op_CMINE(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i!=0); }
void BSVM2_Op_CMILT(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i< 0); }
void BSVM2_Op_CMIGT(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i> 0); }
void BSVM2_Op_CMILE(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i<=0); }
void BSVM2_Op_CMIGE(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i>=0); }


void BSVM2_Op_ADDIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i+=op->v.i; }
void BSVM2_Op_SUBIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i-=op->v.i; }
void BSVM2_Op_MULIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i*=op->v.i; }

void BSVM2_Op_ADDLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l+=op->v.l; }
void BSVM2_Op_SUBLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l-=op->v.l; }
void BSVM2_Op_MULLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l*=op->v.l; }

void BSVM2_Op_ANDIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i&=op->v.i; }
void BSVM2_Op_ORIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i|=op->v.i; }
void BSVM2_Op_XORIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i^=op->v.i; }

void BSVM2_Op_ANDLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l&=op->v.l; }
void BSVM2_Op_ORLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l|=op->v.l; }
void BSVM2_Op_XORLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l^=op->v.l; }


void BSVM2_Op_SHLIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=i<<op->v.i;
}

void BSVM2_Op_SARIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=i>>op->v.i;
}

void BSVM2_Op_SHRIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=((u32)i)>>op->v.i;
}

void BSVM2_Op_SHLLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=i<<op->v.i;
}

void BSVM2_Op_SARLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=i>>op->v.i;
}

void BSVM2_Op_SHRLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=((u64)i)>>op->v.i;
}

void BSVM2_Op_DIVIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=i/op->v.i;
}

void BSVM2_Op_MODIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=i%op->v.i;
}

void BSVM2_Op_DIVLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=i/op->v.l;
}

void BSVM2_Op_MODLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=i%op->v.l;
}

void BSVM2_Op_ADDFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f+=op->v.f; }
void BSVM2_Op_SUBFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f-=op->v.f; }
void BSVM2_Op_MULFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f*=op->v.f; }
void BSVM2_Op_DIVFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f/=op->v.f; }

void BSVM2_Op_ADDDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d+=op->v.d; }
void BSVM2_Op_SUBDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d-=op->v.d; }
void BSVM2_Op_MULDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d*=op->v.d; }
void BSVM2_Op_DIVDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d/=op->v.d; }


void BSVM2_Op_ADDIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i+=frm->local[op->i0].i; }
void BSVM2_Op_SUBIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i-=frm->local[op->i0].i; }
void BSVM2_Op_MULIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i*=frm->local[op->i0].i; }

void BSVM2_Op_ANDIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i&=frm->local[op->i0].i; }
void BSVM2_Op_ORIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i|=frm->local[op->i0].i; }
void BSVM2_Op_XORIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i^=frm->local[op->i0].i; }

void BSVM2_Op_SHLIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->stack[op->t0].i;
	j=frm->local[op->i0].i;
	if(j>=32)
		{ i=0; }
	else
		{ i=i<<j; }
	frm->stack[op->t0].i=i;
}

void BSVM2_Op_SARIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->stack[op->t0].i;
	j=frm->local[op->i0].i;
	if(j>=32)
		{ i=((i>=0)?0:(-1)); }
	else
		{ i=i>>j; }
	frm->stack[op->t0].i=i;
}


void BSVM2_Op_INCI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i++; }
void BSVM2_Op_DECI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i--; }

void BSVM2_Op_INCIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i++; }
void BSVM2_Op_DECIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i--; }
