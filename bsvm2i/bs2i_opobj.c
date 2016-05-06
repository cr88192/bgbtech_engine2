#include <bteifgl.h>

void BSVM2_Op_LDOSI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].i=dtcVaGetI(
		frm->stack[op->t0].a, vi->objinf);		}
void BSVM2_Op_LDOSL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].l=dtcVaGetL(
		frm->stack[op->t0].a, vi->objinf);		}
void BSVM2_Op_LDOSF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].f=dtcVaGetF(
		frm->stack[op->t0].a, vi->objinf);		}
void BSVM2_Op_LDOSD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].d=dtcVaGetD(
		frm->stack[op->t0].a, vi->objinf);		}
void BSVM2_Op_LDOSA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].a=dtcVaGetA(
		frm->stack[op->t0].a, vi->objinf);		}

void BSVM2_Op_STOSI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetI(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].i);	}
void BSVM2_Op_STOSL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetL(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].l);	}
void BSVM2_Op_STOSF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetF(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].f);	}
void BSVM2_Op_STOSD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetD(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].d);	}
void BSVM2_Op_STOSA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetA(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].a);	}


void BSVM2_Op_NEWOBJ(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	vi=op->v.p;
	p=BGBDTC_AllocClassInstance(vi->objinf);
	frm->stack[op->t0].a=dtvWrapPtr(p);
}
