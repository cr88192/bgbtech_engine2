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


BS2VM_API void BSVM2_Op_ADDI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i+=frm->stack[op->t1].i; }
BS2VM_API void BSVM2_Op_SUBI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i-=frm->stack[op->t1].i; }
BS2VM_API void BSVM2_Op_MULI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i*=frm->stack[op->t1].i; }

BS2VM_API void BSVM2_Op_ADDL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l+=frm->stack[op->t1].l; }
BS2VM_API void BSVM2_Op_SUBL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l-=frm->stack[op->t1].l; }
BS2VM_API void BSVM2_Op_MULL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l*=frm->stack[op->t1].l; }

BS2VM_API void BSVM2_Op_ANDI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i&=frm->stack[op->t1].i; }
BS2VM_API void BSVM2_Op_ORI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i|=frm->stack[op->t1].i; }
BS2VM_API void BSVM2_Op_XORI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i^=frm->stack[op->t1].i; }

BS2VM_API void BSVM2_Op_ANDL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l&=frm->stack[op->t1].l; }
BS2VM_API void BSVM2_Op_ORL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l|=frm->stack[op->t1].l; }
BS2VM_API void BSVM2_Op_XORL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l^=frm->stack[op->t1].l; }

BS2VM_API void BSVM2_Op_SHLI(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_SARI(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_SHRI(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_SHLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_SARL(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_SHRL(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_DIVI(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_MODI(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_UDIVI(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_DIVL(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_MODL(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_UDIVL(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_ADDF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f+=frm->stack[op->t1].f; }
BS2VM_API void BSVM2_Op_SUBF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f-=frm->stack[op->t1].f; }
BS2VM_API void BSVM2_Op_MULF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f*=frm->stack[op->t1].f; }
BS2VM_API void BSVM2_Op_DIVF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f/=frm->stack[op->t1].f; }

BS2VM_API void BSVM2_Op_ADDD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d+=frm->stack[op->t1].d; }
BS2VM_API void BSVM2_Op_SUBD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d-=frm->stack[op->t1].d; }
BS2VM_API void BSVM2_Op_MULD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d*=frm->stack[op->t1].d; }
BS2VM_API void BSVM2_Op_DIVD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d/=frm->stack[op->t1].d; }


BS2VM_API void BSVM2_Op_NEGI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=-frm->stack[op->t0].i; }
BS2VM_API void BSVM2_Op_NEGL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=-frm->stack[op->t0].l; }
BS2VM_API void BSVM2_Op_NEGF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=-frm->stack[op->t0].f; }
BS2VM_API void BSVM2_Op_NEGD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=-frm->stack[op->t0].d; }

BS2VM_API void BSVM2_Op_NOTI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=~frm->stack[op->t0].i; }
BS2VM_API void BSVM2_Op_NOTL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=~frm->stack[op->t0].l; }

BS2VM_API void BSVM2_Op_LNTI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=!frm->stack[op->t0].i; }
BS2VM_API void BSVM2_Op_LNTL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=!frm->stack[op->t0].l; }

BS2VM_API void BSVM2_Op_RCPF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=1.0/frm->stack[op->t0].f; }
BS2VM_API void BSVM2_Op_RCPD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=1.0/frm->stack[op->t0].d; }

BS2VM_API void BSVM2_Op_SQRTF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=sqrt(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_SQRTD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=sqrt(frm->stack[op->t0].d); }

BS2VM_API void BSVM2_Op_RSQRTF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=1.0/sqrt(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_RSQRTD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=1.0/sqrt(frm->stack[op->t0].d); }


BS2VM_API void BSVM2_Op_SINF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=sin(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_SIND(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=sin(frm->stack[op->t0].d); }
BS2VM_API void BSVM2_Op_COSF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=cos(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_COSD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=cos(frm->stack[op->t0].d); }
BS2VM_API void BSVM2_Op_TANF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=tan(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_TAND(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=tan(frm->stack[op->t0].d); }
BS2VM_API void BSVM2_Op_ATANF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=atan(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_ATAND(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=atan(frm->stack[op->t0].d); }

BS2VM_API void BSVM2_Op_SQRF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].f*frm->stack[op->t0].f; }
BS2VM_API void BSVM2_Op_SQRD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t0].d*frm->stack[op->t0].d; }

double bsvm2_ssqrt(double x)
	{ return((x>0)?sqrt(x):(-sqrt(-x))); }
BS2VM_API void BSVM2_Op_SSQRTF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=bsvm2_ssqrt(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_SSQRTD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=bsvm2_ssqrt(frm->stack[op->t0].d); }

double bsvm2_fabs(double x)
	{ return((x>0)?x:(-x)); }
BS2VM_API void BSVM2_Op_ABSF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=bsvm2_fabs(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_ABSD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=bsvm2_fabs(frm->stack[op->t0].d); }

s64 bsvm2_labs(s64 x)
	{ return((x>0)?x:(-x)); }
BS2VM_API void BSVM2_Op_ABSI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=bsvm2_labs(frm->stack[op->t0].i); }
BS2VM_API void BSVM2_Op_ABSL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=bsvm2_labs(frm->stack[op->t0].l); }

BS2VM_API void BSVM2_Op_SQRI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].i*frm->stack[op->t0].i; }
BS2VM_API void BSVM2_Op_SQRL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t0].l*frm->stack[op->t0].l; }


BS2VM_API void BSVM2_Op_CMPI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->stack[op->t0].i;
	b=frm->stack[op->t1].i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 a, b;
	a=frm->stack[op->t0].l;
	b=frm->stack[op->t1].l;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	f32 a, b;
	a=frm->stack[op->t0].f;
	b=frm->stack[op->t1].f;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	f64 a, b;
	a=frm->stack[op->t0].d;
	b=frm->stack[op->t1].d;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMP2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	f32 a, b;
	a=frm->stack[op->t0].f;
	b=frm->stack[op->t1].f;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMP2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	f64 a, b;
	a=frm->stack[op->t0].d;
	b=frm->stack[op->t1].d;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u64 a, b;
	a=frm->stack[op->t0].ul;
	b=frm->stack[op->t1].ul;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMP2A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u64 a, b;
	a=frm->stack[op->t0].ul;
	b=frm->stack[op->t1].ul;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_UCMPI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u32 a, b;
	a=frm->stack[op->t0].ui;
	b=frm->stack[op->t1].ui;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_UCMPL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	u64 a, b;
	a=frm->stack[op->t0].ul;
	b=frm->stack[op->t1].ul;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->stack[op->t0].i;
	b=op->v.i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->stack[op->t0].i;
	b=frm->local[op->i0].i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->local[op->i0].i;
	b=op->v.i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->local[op->i0].i;
	b=frm->local[op->i1].i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPILL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->local[op->i0].i;
	b=frm->local[op->i1].i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMPILC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->local[op->i0].i;
	b=op->v.i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}


BS2VM_API void BSVM2_Op_DCMPIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 a, b;
	a=frm->stack[op->t1].i;
	b=op->v.i;
	frm->stack[op->t0].i=(a>b)|(0-(a<b));
}

BS2VM_API void BSVM2_Op_CMIEQ(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i==0); }
BS2VM_API void BSVM2_Op_CMINE(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i!=0); }
BS2VM_API void BSVM2_Op_CMILT(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i< 0); }
BS2VM_API void BSVM2_Op_CMIGT(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i> 0); }
BS2VM_API void BSVM2_Op_CMILE(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i<=0); }
BS2VM_API void BSVM2_Op_CMIGE(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(frm->stack[op->t0].i>=0); }


BS2VM_API void BSVM2_Op_ADDIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i+=op->v.i; }
BS2VM_API void BSVM2_Op_SUBIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i-=op->v.i; }
BS2VM_API void BSVM2_Op_MULIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i*=op->v.i; }

BS2VM_API void BSVM2_Op_ADDLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l+=op->v.l; }
BS2VM_API void BSVM2_Op_SUBLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l-=op->v.l; }
BS2VM_API void BSVM2_Op_MULLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l*=op->v.l; }

BS2VM_API void BSVM2_Op_ANDIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i&=op->v.i; }
BS2VM_API void BSVM2_Op_ORIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i|=op->v.i; }
BS2VM_API void BSVM2_Op_XORIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i^=op->v.i; }

BS2VM_API void BSVM2_Op_ANDLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l&=op->v.l; }
BS2VM_API void BSVM2_Op_ORLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l|=op->v.l; }
BS2VM_API void BSVM2_Op_XORLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l^=op->v.l; }


BS2VM_API void BSVM2_Op_SHLIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=i<<op->v.i;
}

BS2VM_API void BSVM2_Op_SARIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=i>>op->v.i;
}

BS2VM_API void BSVM2_Op_SHRIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=((u32)i)>>op->v.i;
}

BS2VM_API void BSVM2_Op_SHLLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=i<<op->v.i;
}

BS2VM_API void BSVM2_Op_SARLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=i>>op->v.i;
}

BS2VM_API void BSVM2_Op_SHRLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=((u64)i)>>op->v.i;
}

BS2VM_API void BSVM2_Op_DIVIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=i/op->v.i;
}

BS2VM_API void BSVM2_Op_MODIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i;
	i=frm->stack[op->t0].i;
	frm->stack[op->t0].i=i%op->v.i;
}

BS2VM_API void BSVM2_Op_DIVLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=i/op->v.l;
}

BS2VM_API void BSVM2_Op_MODLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s64 i;
	i=frm->stack[op->t0].l;
	frm->stack[op->t0].l=i%op->v.l;
}

BS2VM_API void BSVM2_Op_ADDFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f+=op->v.f; }
BS2VM_API void BSVM2_Op_SUBFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f-=op->v.f; }
BS2VM_API void BSVM2_Op_MULFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f*=op->v.f; }
BS2VM_API void BSVM2_Op_DIVFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f/=op->v.f; }

BS2VM_API void BSVM2_Op_ADDDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d+=op->v.d; }
BS2VM_API void BSVM2_Op_SUBDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d-=op->v.d; }
BS2VM_API void BSVM2_Op_MULDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d*=op->v.d; }
BS2VM_API void BSVM2_Op_DIVDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d/=op->v.d; }


BS2VM_API void BSVM2_Op_ADDIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i+=frm->local[op->i0].i; }
BS2VM_API void BSVM2_Op_SUBIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i-=frm->local[op->i0].i; }
BS2VM_API void BSVM2_Op_MULIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i*=frm->local[op->i0].i; }

BS2VM_API void BSVM2_Op_ANDIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i&=frm->local[op->i0].i; }
BS2VM_API void BSVM2_Op_ORIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i|=frm->local[op->i0].i; }
BS2VM_API void BSVM2_Op_XORIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i^=frm->local[op->i0].i; }

BS2VM_API void BSVM2_Op_SHLIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
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

BS2VM_API void BSVM2_Op_SARIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
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


BS2VM_API void BSVM2_Op_INCI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i++; }
BS2VM_API void BSVM2_Op_DECI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i--; }

BS2VM_API void BSVM2_Op_INCIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i++; }
BS2VM_API void BSVM2_Op_DECIL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i--; }



BS2VM_API void BSVM2_Op_ADDI_LLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i+frm->local[op->i2].i; }
BS2VM_API void BSVM2_Op_SUBI_LLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i-frm->local[op->i2].i; }
BS2VM_API void BSVM2_Op_MULI_LLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i*frm->local[op->i2].i; }

BS2VM_API void BSVM2_Op_ANDI_LLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i&frm->local[op->i2].i; }
BS2VM_API void BSVM2_Op_ORI_LLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i|frm->local[op->i2].i; }
BS2VM_API void BSVM2_Op_XORI_LLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i^frm->local[op->i2].i; }

BS2VM_API void BSVM2_Op_SHLI_LLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->local[op->i1].i;
	j=frm->local[op->i2].i;
	if(j>=32)
		{ i=0; }
	else
		{ i=i<<j; }
	frm->local[op->i0].i=i;
}

BS2VM_API void BSVM2_Op_SARI_LLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	s32 i, j;

	i=frm->local[op->i1].i;
	j=frm->local[op->i2].i;
	if(j>=32)
		{ i=((i>=0)?0:(-1)); }
	else
		{ i=i>>j; }
	frm->local[op->i0].i=i;
}

BS2VM_API void BSVM2_Op_ADDI_LLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i+op->v.i; }
BS2VM_API void BSVM2_Op_SUBI_LLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i-op->v.i; }
BS2VM_API void BSVM2_Op_MULI_LLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i*op->v.i; }
BS2VM_API void BSVM2_Op_ANDI_LLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i&op->v.i; }
BS2VM_API void BSVM2_Op_ORI_LLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i|op->v.i; }
BS2VM_API void BSVM2_Op_XORI_LLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i^op->v.i; }
BS2VM_API void BSVM2_Op_SHLI_LLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i<<op->v.i; }
BS2VM_API void BSVM2_Op_SARI_LLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i>>op->v.i; }
