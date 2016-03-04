
void BSVM2_Op_LDI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->local[op->i0].i; }
void BSVM2_Op_LDL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->local[op->i0].l; }
void BSVM2_Op_LDF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->local[op->i0].f; }
void BSVM2_Op_LDD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->local[op->i0].d; }
void BSVM2_Op_LDA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=frm->local[op->i0].a; }

void BSVM2_Op_STI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->stack[op->t0].i; }
void BSVM2_Op_STL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].l=frm->stack[op->t0].l; }
void BSVM2_Op_STF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].f=frm->stack[op->t0].f; }
void BSVM2_Op_STD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].d=frm->stack[op->t0].d; }
void BSVM2_Op_STA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].a=frm->stack[op->t0].a; }

void BSVM2_Op_LDCI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=op->v.i; }
void BSVM2_Op_LDCL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=op->v.l; }
void BSVM2_Op_LDCF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=op->v.f; }
void BSVM2_Op_LDCD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=op->v.d; }
void BSVM2_Op_LDCA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=op->v.a; }

void BSVM2_Op_MVI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i; }
void BSVM2_Op_MVL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].l=frm->local[op->i1].l; }
void BSVM2_Op_MVF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].f=frm->local[op->i1].f; }
void BSVM2_Op_MVD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].d=frm->local[op->i1].d; }
void BSVM2_Op_MVA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].a=frm->local[op->i1].a; }
