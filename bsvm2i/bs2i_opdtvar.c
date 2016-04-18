#include <bteifgl.h>

void BSVM2_Op_ADDAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Add(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_SUBAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Sub(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_MULAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Mul(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_ANDAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_And(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_ORAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Or(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_XORAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Xor(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_SHLAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Shl(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_SARAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Sar(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_SHRAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Shr(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_DIVAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Div(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_MODAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Mod(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

void BSVM2_Op_NEGAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Neg(frm->stack[op->t0].a);
}

void BSVM2_Op_NOTAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Not(frm->stack[op->t0].a);
}

void BSVM2_Op_LNTAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_LNot(frm->stack[op->t0].a);
}

void BSVM2_Op_CMPAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].i=BGBDT_TagAri_Compare(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}
