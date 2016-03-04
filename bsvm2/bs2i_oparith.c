
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
