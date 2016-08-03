void BSVM2_Op_CVTA2PTR(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvUnwrapPtr(frm->stack[op->t0].a);
	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
}

void BSVM2_Op_CVTPTR2A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	frm->stack[op->t0].a=dtvWrapPtr(p);
}

void BSVM2_Op_LDIXI_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(s32 *)p;
}

void BSVM2_Op_LDIXL_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].l=*(s64 *)p;
}

void BSVM2_Op_LDIXF_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].f=*(f32 *)p;
}

void BSVM2_Op_LDIXD_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].d=*(f64 *)p;
}

void BSVM2_Op_LDIXA_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].a=*(dtVal *)p;
}

void BSVM2_Op_LDIXSB_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB1F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(sbyte *)p;
}

void BSVM2_Op_LDIXUB_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB1F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(byte *)p;
}

void BSVM2_Op_LDIXSS_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB2F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(s16 *)p;
}

void BSVM2_Op_LDIXUS_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB2F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(u16 *)p;
}


void BSVM2_Op_RSTIXI_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(s32 *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_RSTIXL_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(s64 *)p=frm->stack[op->t0].l;
}

void BSVM2_Op_RSTIXF_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(f32 *)p=frm->stack[op->t0].f;
}

void BSVM2_Op_RSTIXD_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(f64 *)p=frm->stack[op->t0].d;
}

void BSVM2_Op_RSTIXA_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(dtVal *)p=frm->stack[op->t0].a;
}

void BSVM2_Op_RSTIXB_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB1F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(byte *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_RSTIXS_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB2F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(s16 *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_LDIXIC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(s32 *)p;
}

void BSVM2_Op_LDIXLC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].l=*(s64 *)p;
}

void BSVM2_Op_LDIXFC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].f=*(f32 *)p;
}

void BSVM2_Op_LDIXDC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].d=*(f64 *)p;
}

void BSVM2_Op_LDIXAC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].a=*(dtVal *)p;
}

void BSVM2_Op_LDIXSBC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB1F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(sbyte *)p;
}

void BSVM2_Op_LDIXUBC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB1F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(byte *)p;
}

void BSVM2_Op_LDIXSSC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB2F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(s16 *)p;
}

void BSVM2_Op_LDIXUSC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB2F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(u16 *)p;
}

void BSVM2_Op_RSTIXIC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(frm->stack[op->t1].a, op->v.i);
	*(s32 *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_RSTIXLC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	*(s64 *)p=frm->stack[op->t0].l;
}

void BSVM2_Op_RSTIXFC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(frm->stack[op->t1].a, op->v.i);
	*(f32 *)p=frm->stack[op->t0].f;
}

void BSVM2_Op_RSTIXDC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	*(f64 *)p=frm->stack[op->t0].d;
}

void BSVM2_Op_RSTIXAC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	*(dtVal *)p=frm->stack[op->t0].a;
}

void BSVM2_Op_RSTIXBC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB1F(frm->stack[op->t1].a, op->v.i);
	*(byte *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_RSTIXSC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB2F(frm->stack[op->t1].a, op->v.i);
	*(u16 *)p=frm->stack[op->t0].i;
}
