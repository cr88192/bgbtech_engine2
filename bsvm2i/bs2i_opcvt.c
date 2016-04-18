#include <bteifgl.h>

void BSVM2_Op_NOP(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
}

void BSVM2_Op_DUPI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t1].i; }
void BSVM2_Op_DUPL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t1].l; }
void BSVM2_Op_DUPF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t1].f; }
void BSVM2_Op_DUPD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t1].d; }
void BSVM2_Op_DUPA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=frm->stack[op->t1].a; }

void BSVM2_Op_PUSHI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=0; }
void BSVM2_Op_PUSHL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=0; }
void BSVM2_Op_PUSHF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=0; }
void BSVM2_Op_PUSHD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=0; }
void BSVM2_Op_PUSHA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=DTV_NULL; }

void BSVM2_Op_SWAPA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal n0, n1;
	n0=frm->stack[op->t0].a;
	n1=frm->stack[op->t1].a;
	frm->stack[op->t0].a=n1;
	frm->stack[op->t1].a=n0;
}

void BSVM2_Op_CVTI2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t0].i; }
void BSVM2_Op_CVTI2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].i; }
void BSVM2_Op_CVTI2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t0].i; }

void BSVM2_Op_CVTL2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].l; }
void BSVM2_Op_CVTL2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].l; }
void BSVM2_Op_CVTL2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t0].l; }

void BSVM2_Op_CVTF2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].f; }
void BSVM2_Op_CVTF2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t0].f; }
void BSVM2_Op_CVTF2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t0].f; }

void BSVM2_Op_CVTD2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].d; }
void BSVM2_Op_CVTD2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t0].d; }
void BSVM2_Op_CVTD2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].d; }

void BSVM2_Op_CVTSB2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(sbyte)frm->stack[op->t0].i; }
void BSVM2_Op_CVTUB2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(byte)frm->stack[op->t0].i; }
void BSVM2_Op_CVTSS2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(s16)frm->stack[op->t0].i; }
void BSVM2_Op_CVTUS2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(u16)frm->stack[op->t0].i; }

void BSVM2_Op_CVTI2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvWrapInt(frm->stack[op->t0].i); }
void BSVM2_Op_CVTL2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvWrapLong(frm->stack[op->t0].l); }
void BSVM2_Op_CVTF2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvWrapFloat(frm->stack[op->t0].f); }
void BSVM2_Op_CVTD2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvWrapDouble(frm->stack[op->t0].d); }

void BSVM2_Op_CVTAA2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvUnwrapInt(frm->stack[op->t0].a); }
void BSVM2_Op_CVTAA2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=dtvUnwrapLong(frm->stack[op->t0].a); }
void BSVM2_Op_CVTAA2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=dtvUnwrapFloat(frm->stack[op->t0].a); }
void BSVM2_Op_CVTAA2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=dtvUnwrapDouble(frm->stack[op->t0].a); }
