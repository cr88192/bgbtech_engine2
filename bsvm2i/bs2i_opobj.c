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

void BSVM2_Op_DELOBJ(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal a;
	void *p;

	a=frm->stack[op->t0].a;
	p=dtvUnwrapPtr(a);
	if(p)
		dtmFree(p);
}

void BSVM2_Op_IFXOBJ(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	vi=op->v.p;
	p=BGBDTC_AllocClassInstance(vi->objinf);
//	frm->stack[op->t0].a=dtvWrapPtr(p);
	frm->local[op->i0].a=dtvWrapPtr(p);
}

void BSVM2_Op_DFXOBJ(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal a;
	void *p;

	a=frm->local[op->i0].a;
	p=dtvUnwrapPtr(a);
	if(p)
		dtmFree(p);
}

void BSVM2_Op_LDTHISI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].i=dtcVaGetI(frm->self, vi->objinf);		}
void BSVM2_Op_LDTHISL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].l=dtcVaGetL(frm->self, vi->objinf);		}
void BSVM2_Op_LDTHISF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].f=dtcVaGetF(frm->self, vi->objinf);		}
void BSVM2_Op_LDTHISD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].d=dtcVaGetD(frm->self, vi->objinf);		}
void BSVM2_Op_LDTHISA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].a=dtcVaGetA(frm->self, vi->objinf);		}

void BSVM2_Op_STTHISI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetI(frm->self, vi->objinf, frm->stack[op->t0].i);	}
void BSVM2_Op_STTHISL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetL(frm->self, vi->objinf, frm->stack[op->t0].l);	}
void BSVM2_Op_STTHISF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetF(frm->self, vi->objinf, frm->stack[op->t0].f);	}
void BSVM2_Op_STTHISD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetD(frm->self, vi->objinf, frm->stack[op->t0].d);	}
void BSVM2_Op_STTHISA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetA(frm->self, vi->objinf, frm->stack[op->t0].a);	}
