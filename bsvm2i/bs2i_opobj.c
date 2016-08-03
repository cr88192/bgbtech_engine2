#include <bteifgl.h>

void BSVM2_Op_LDOSI_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].i=dtcVaGetI(
		frm->stack[op->t0].a, vi->objinf);		}
void BSVM2_Op_LDOSL_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].l=dtcVaGetL(
		frm->stack[op->t0].a, vi->objinf);		}
void BSVM2_Op_LDOSF_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].f=dtcVaGetF(
		frm->stack[op->t0].a, vi->objinf);		}
void BSVM2_Op_LDOSD_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].d=dtcVaGetD(
		frm->stack[op->t0].a, vi->objinf);		}
void BSVM2_Op_LDOSA_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtcVaGetA(
		frm->stack[op->t0].a, vi->objinf);		}

void BSVM2_Op_STOSI_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtcVaSetI(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].i);	}
void BSVM2_Op_STOSL_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtcVaSetL(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].l);	}
void BSVM2_Op_STOSF_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtcVaSetF(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].f);	}
void BSVM2_Op_STOSD_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtcVaSetD(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].d);	}
void BSVM2_Op_STOSA_TN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtcVaSetA(frm->stack[op->t1].a, vi->objinf, frm->stack[op->t0].a);	}

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

void BSVM2_Op_LDOSLI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].i=dtcVaGetI(
		frm->local[op->i0].a, vi->objinf);		}
void BSVM2_Op_LDOSLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].l=dtcVaGetL(
		frm->local[op->i0].a, vi->objinf);		}
void BSVM2_Op_LDOSLF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].f=dtcVaGetF(
		frm->local[op->i0].a, vi->objinf);		}
void BSVM2_Op_LDOSLD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].d=dtcVaGetD(
		frm->local[op->i0].a, vi->objinf);		}
void BSVM2_Op_LDOSLA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].a=dtcVaGetA(
		frm->local[op->i0].a, vi->objinf);		}

void BSVM2_Op_STOSLI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetI(frm->local[op->i0].a, vi->objinf, frm->stack[op->t0].i);	}
void BSVM2_Op_STOSLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetL(frm->local[op->i0].a, vi->objinf, frm->stack[op->t0].l);	}
void BSVM2_Op_STOSLF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetF(frm->local[op->i0].a, vi->objinf, frm->stack[op->t0].f);	}
void BSVM2_Op_STOSLD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetD(frm->local[op->i0].a, vi->objinf, frm->stack[op->t0].d);	}
void BSVM2_Op_STOSLA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	dtcVaSetA(frm->local[op->i0].a, vi->objinf, frm->stack[op->t0].a);	}

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

void BSVM2_Op_ISTYPE(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;

	vi=op->v.p;
	if((vi->tag==BS2CC_ITCC_CL) ||
		(vi->tag==BS2CC_ITCC_IF))
	{
		frm->stack[op->t0].i=
			BGBDTC_CheckObjvInstanceof(
				frm->stack[op->t0].a, vi->objinf);
		return;
	}

	frm->stack[op->t0].i=0;
}

void BSVM2_Op_DZNISTYPE(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	int i;

	vi=op->v.p;
	if((vi->tag==BS2CC_ITCC_CL) ||
		(vi->tag==BS2CC_ITCC_IF))
	{
		i=BGBDTC_CheckObjvInstanceof(
			frm->stack[op->t0].a, vi->objinf);
		if(!i)frm->stack[op->t0].a=DTV_NULL;
		return;
	}

	frm->stack[op->t0].a=DTV_NULL;
}

void BSVM2_Op_DTNISTYPE(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	int i;

	vi=op->v.p;
	if((vi->tag==BS2CC_ITCC_CL) ||
		(vi->tag==BS2CC_ITCC_IF))
	{
		i=BGBDTC_CheckObjvInstanceof(
			frm->stack[op->t0].a, vi->objinf);
//		if(!i)frm->stack[op->t0].a=DTV_NULL;
		if(!i)
			frm->ctx->status=BSVM2_EXS_CASTEX;
		return;
	}

//	frm->stack[op->t0].a=DTV_NULL;
}

void BSVM2_Op_DTRAPNULL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal v;
	v=frm->stack[op->t0].a;
	if(dtvNullP(v) || dtvUndefinedP(v))
		frm->ctx->status=BSVM2_EXS_NULLEX;
}

void BSVM2_Op_LTRAPNULL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal v;
	v=frm->local[op->i0].a;
	if(dtvNullP(v) || dtvUndefinedP(v))
		frm->ctx->status=BSVM2_EXS_NULLEX;
}

int BSVM2_OpInt_IsTypeA(dtVal v, char *s)
{
	int r;

	switch(*s)
	{
	case 'b':	r=dtvIsBoolP(v); break;
	case 'c':	case 'w':
		r=dtvIsCharP(v); break;
	case 'd':	r=dtvIsFixDoubleP(v); break;
	case 'f':	r=dtvIsFixFloatP(v); break;
	case 'i':	r=dtvIsFixIntP(v); break;
	case 'j':	r=dtvIsFixUIntP(v); break;
	case 'x':	r=dtvIsFixLongP(v); break;
	case 'y':	r=dtvIsFixLongP(v); break;
	case 'C':
		switch(s[1])
		{
		case 'i':	r=dtvIsSmallIntP(v); break;
		case 'j':	r=dtvIsSmallDoubleP(v); break;
		case 'l':	r=dtvIsSmallLongP(v); break;
		case 'm':	r=dtvIsSmallDoubleP(v); break;

		case 'n': r=BGBDT_TagStr_IsKeywordP(v); break;
		case 'o': r=BGBDTC_CheckObjvObjectP(v); break;
		case 'p': r=BGBDT_MapObj_IsObjectP(v); break;
		case 's': r=BGBDT_TagStr_IsStringP(v); break;
		case 't': r=BGBDT_TagStr_IsSymbolP(v); break;
		default: r=0; break;
		}
		break;
	case 'Q':
		switch(s[1])
		{
		case 'a':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_SBYTE);
			break;
		case 'd':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_DOUBLE);
			break;
		case 'f':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_FLOAT);
			break;
		case 'h':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_UBYTE);
			break;
		case 'i':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_INT);
			break;
		case 'j':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_UINT);
			break;
		case 'r':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_ADDRESS);
			break;
		case 's':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_SHORT);
			break;
		case 't':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_USHORT);
			break;
		case 'x':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_LONG);
			break;
		case 'y':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_ULONG);
			break;

		case 'Q':
		case 'L':
			r=BGBDT_TagArr_IsArrayBtyP(v, BGBDT_BASETY_ADDRESS);
			break;

		default:
			r=BGBDT_TagArr_IsArrayP(v);
			break;
		}
	default:
		r=0; break;
	}
	return(r);
}

void BSVM2_Op_DZNISTYPE_AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal v;
	char *s;
	int r;

	s=op->v.p;
	v=frm->stack[op->t0].a;
	r=BSVM2_OpInt_IsTypeA(v, s);
	if(!r)
		frm->stack[op->t0].a=DTV_NULL;
}

void BSVM2_Op_DTNISTYPE_AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal v;
	char *s;
	int r;

	s=op->v.p;
	v=frm->stack[op->t0].a;
	r=BSVM2_OpInt_IsTypeA(v, s);
	if(!r)
		frm->ctx->status=BSVM2_EXS_CASTEX;
}

void BSVM2_Op_ISTYPE_AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal v;
	char *s;
	int r;

	s=op->v.p;
	v=frm->stack[op->t0].a;
	r=BSVM2_OpInt_IsTypeA(v, s);
	frm->stack[op->t0].i=r;
}

//void BSVM2_Op_ISTYPE_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
//	{ frm->stack[op->t0].i=dtvIsSmallIntP(frm->stack[op->t0].a); }

void BSVM2_Op_ISTYPE_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvIsFixIntP(frm->stack[op->t0].a); }
void BSVM2_Op_ISTYPE_UI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvIsFixUIntP(frm->stack[op->t0].a); }

void BSVM2_Op_ISTYPE_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvIsFixLongP(frm->stack[op->t0].a); }
void BSVM2_Op_ISTYPE_UL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvIsFixLongP(frm->stack[op->t0].a); }

void BSVM2_Op_ISTYPE_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvIsFixFloatP(frm->stack[op->t0].a); }
void BSVM2_Op_ISTYPE_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvIsFixDoubleP(frm->stack[op->t0].a); }

void BSVM2_Op_ISTYPE_B(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvIsBoolP(frm->stack[op->t0].a); }
void BSVM2_Op_ISTYPE_C(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvIsCharP(frm->stack[op->t0].a); }

//void BSVM2_Op_ISTYPE_STR(BSVM2_Frame *frm, BSVM2_Opcode *op)
//	{ frm->stack[op->t0].i=BGBDT_TagStr_IsStringP(frm->stack[op->t0].a); }

void BSVM2_OpInit_IsTypeS(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, char *sig)
{
	switch(*sig)
	{
	case 'b':	op->Run=BSVM2_Op_ISTYPE_B; break;
	case 'c':	case 'w':
		op->Run=BSVM2_Op_ISTYPE_C; break;
	case 'd':	op->Run=BSVM2_Op_ISTYPE_D; break;
	case 'f':	op->Run=BSVM2_Op_ISTYPE_F; break;
	case 'i':	op->Run=BSVM2_Op_ISTYPE_I; break;
	case 'j':	op->Run=BSVM2_Op_ISTYPE_UI; break;
	case 'x':	op->Run=BSVM2_Op_ISTYPE_L; break;
	case 'y':	op->Run=BSVM2_Op_ISTYPE_UL; break;
	default:	op->Run=BSVM2_Op_ISTYPE_AA; break;
	}
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


void BSVM2_Op_LDOSX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	BSVM2_ValX128 *a;
	void *p;

	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }

	a=BSVM2_FrameAllocX128(frm);
	vi=op->v.p;
	p=dtcVaGetPtr(frm->stack[op->t0].a, vi->objinf);
	*a=*(BSVM2_ValX128 *)p;
	frm->stack[op->t0].p=a;
}

void BSVM2_Op_STOSX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	BSVM2_ValX128 *a;
	void *p;

	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }

	a=frm->stack[op->t0].p;
	vi=op->v.p;
	p=dtcVaGetPtr(frm->stack[op->t1].a, vi->objinf);
	*(BSVM2_ValX128 *)p=*a;
	BSVM2_FrameFreeX128(frm, a);
}

void BSVM2_Op_LDTHISX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	BSVM2_ValX128 *a;
	void *p;

	a=BSVM2_FrameAllocX128(frm);
	vi=op->v.p;
	p=dtcVaGetPtr(frm->self, vi->objinf);
	*a=*(BSVM2_ValX128 *)p;
	frm->stack[op->t0].p=a;
}

void BSVM2_Op_STTHISX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	BSVM2_ValX128 *a;
	void *p;

	a=frm->stack[op->t0].p;
	vi=op->v.p;
	p=dtcVaGetPtr(frm->self, vi->objinf);
	*(BSVM2_ValX128 *)p=*a;
	BSVM2_FrameFreeX128(frm, a);
}

void BSVM2_Op_NEWDYO(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=BGBDT_MapObj_AllocObjv();
}

void BSVM2_Op_SBOS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	vi=op->v.p;
	BGBDT_MapObj_BindObjvSlotValueName(
		frm->stack[op->t1].a,
		vi->name,
		frm->stack[op->t0].a);
}

void BSVM2_Op_DSBOS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	vi=op->v.p;
	BGBDT_MapObj_BindObjvSlotValueName(
		frm->stack[op->t0].a,
		vi->name,
		frm->stack[op->t1].a);
}

void BSVM2_Op_DSTOS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_Op_DSBOS(frm, op);
}

void BSVM2_Op_DUPAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=frm->stack[op->t1].a;
}

void BSVM2_Op_DROPAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
}

void BSVM2_Op_LDOSAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal a, obj, idx;
	char *fn;
	void *p;
	
//	BSVM2_ImageGlobal *vi;
//	vi=op->v.p;
//	BGBDT_MapObj_BindObjvSlotValueName(
//		frm->stack[op->t0].a,
//		vi->name,
//		frm->stack[op->t1].a);

	obj=frm->stack[op->t0].a;
	if(BGBDT_MapObj_IsObjectP(obj))
	{
		fn=op->v.p;
		a=BGBDT_MapObj_GetObjvSlotValueName(obj, fn);
		frm->stack[op->t0].a=a;
		return;
	}
	
	frm->stack[op->t0].a=DTV_UNDEFINED;
}

void BSVM2_Op_STOSAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal obj, idx, val;
	char *fn;
	void *p;
	
//	BSVM2_ImageGlobal *vi;
//	vi=op->v.p;
//	BGBDT_MapObj_BindObjvSlotValueName(
//		frm->stack[op->t0].a,
//		vi->name,
//		frm->stack[op->t1].a);

	val=frm->stack[op->t0].a;
	obj=frm->stack[op->t1].a;

	if(BGBDT_MapObj_IsObjectP(obj))
	{
		fn=op->v.p;
		BGBDT_MapObj_BindObjvSlotValueName(obj, fn, val);
		return;
	}
	
	frm->stack[op->t0].a=DTV_UNDEFINED;
}

void BSVM2_Op_SBOSAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal obj;
	obj=frm->stack[op->t1].a;
	if(BGBDT_MapObj_IsObjectP(obj))
	{
		BGBDT_MapObj_BindObjvSlotValueName(
			frm->stack[op->t1].a,
			op->v.p,
			frm->stack[op->t0].a);
	}
}

void BSVM2_Op_DSBOSAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal obj;
	obj=frm->stack[op->t0].a;
	if(BGBDT_MapObj_IsObjectP(obj))
	{
		BGBDT_MapObj_BindObjvSlotValueName(
			frm->stack[op->t0].a,
			op->v.p,
			frm->stack[op->t1].a);
	}
}

void BSVM2_Op_DSTOSAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_Op_DSBOSAA(frm, op);
}

void BSVM2_Op_LDOSLAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal a, obj, idx;
	char *fn;
	void *p;
	
	obj=frm->local[op->i0].a;
	if(BGBDT_MapObj_IsObjectP(obj))
	{
		fn=op->v.p;
		a=BGBDT_MapObj_GetObjvSlotValueName(obj, fn);
		frm->stack[op->t0].a=a;
		return;
	}
	
	frm->stack[op->t0].a=DTV_UNDEFINED;
}

void BSVM2_Op_STOSLAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal a, obj, idx, val;
	char *fn;
	void *p;
	
	val=frm->stack[op->t0].a;
	obj=frm->local[op->i0].a;

	if(BGBDT_MapObj_IsObjectP(obj))
	{
		fn=op->v.p;
		BGBDT_MapObj_BindObjvSlotValueName(obj, fn, val);
		return;
	}
	
	frm->stack[op->t0].a=DTV_UNDEFINED;
}

void BSVM2_Op_LDIXAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal a, obj, idx;
	char *fn;
	void *p;
	
	obj=frm->stack[op->t0].a;
	idx=frm->stack[op->t1].a;
	if(dtvIsArrayP(obj))
	{
		if(dtvIsSmallLongP(idx))
		{
			a=BGBDT_TagArr_GetIndexVariant(
				obj, dtvUnwrapInt(idx));
			frm->stack[op->t0].a=a;
			return;
		}
	}
	
	if(BGBDT_MapObj_IsObjectP(obj))
	{
		if(BGBDT_TagStr_IsSymbolP(idx) ||
			BGBDT_TagStr_IsKeywordP(idx) ||
			BGBDT_TagStr_IsStringP(idx))
		{
			fn=BGBDT_TagStr_GetChars(idx);
			a=BGBDT_MapObj_GetObjvSlotValueName(obj, fn);
			frm->stack[op->t0].a=a;
			return;
		}
	}
	
	frm->stack[op->t0].a=DTV_UNDEFINED;
}

void BSVM2_Op_STIXAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal obj, idx, val;
	char *fn;
	void *p;
	
	obj=frm->stack[op->t1].a;
	idx=frm->stack[op->t2].a;
	val=frm->stack[op->t0].a;
	if(dtvIsArrayP(obj))
	{
		if(dtvIsSmallLongP(obj))
		{
			BGBDT_TagArr_SetIndexVariant(
				obj, dtvUnwrapInt(idx), val);
			return;
		}
		return;
	}

	if(BGBDT_MapObj_IsObjectP(obj))
	{
		if(BGBDT_TagStr_IsSymbolP(idx) ||
			BGBDT_TagStr_IsKeywordP(idx) ||
			BGBDT_TagStr_IsStringP(idx))
		{
			fn=BGBDT_TagStr_GetChars(idx);
			BGBDT_MapObj_BindObjvSlotValueName(obj, fn, val);
			return;
		}
		return;
	}
}

void BSVM2_Op_LDIXAI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal a;
	void *p;

	if(dtvIsArrayP(frm->stack[op->t0].a))
	{
		a=BGBDT_TagArr_GetIndexVariant(
			frm->stack[op->t0].a,
			frm->stack[op->t1].i);
		frm->stack[op->t0].a=a;
		return;
	}

	frm->stack[op->t0].a=DTV_UNDEFINED;
}

void BSVM2_Op_STIXAI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvIsArrayP(frm->stack[op->t1].a))
	{
		BGBDT_TagArr_SetIndexVariant(
			frm->stack[op->t1].a,
			frm->stack[op->t2].i,
			frm->stack[op->t0].a);
		return;
	}
}
