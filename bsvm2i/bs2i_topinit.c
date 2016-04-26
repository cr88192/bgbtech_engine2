#include <bteifgl.h>

byte *BSVM2_Interp_DecodeOpJAddr(BSVM2_CodeBlock *cblk)
{
	byte *cs;
	int i;
	
	i=BSVM2_Interp_DecodeOpSCxI(cblk);
	cs=cblk->cs+i;
	return(cs);
}

void BSVM2_Interp_DecodeTOpGx(BSVM2_CodeBlock *cblk, BSVM2_TailOpcode *op)
{
//	op->i0=BSVM2_Interp_DecodeOpUCxI(cblk);
	op->v.p=BSVM2_Interp_DecodeOpAddrPtr(cblk,
		BSVM2_Interp_DecodeOpUCxI(cblk));
}

void BSVM2_Interp_SetupTopPopUnJmp(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->jcs=BSVM2_Interp_DecodeOpJAddr(cblk);

	op->Run=run;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupTopPopUn(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->Run=run;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupTopUatJmp(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->jcs=BSVM2_Interp_DecodeOpJAddr(cblk);
	op->Run=run;
}

void BSVM2_Interp_SetupTopUat(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op,
	BSVM2_Trace *(*run)(BSVM2_Frame *frm, BSVM2_TailOpcode *op))
{
	op->Run=run;
}

void BSVM2_Interp_SetupTopJCMP(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op)
{
	int z, o;
	int i, j;
	
	i=*cblk->cs++;
	z=(i>>4)&15;
	o=i&15;

	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
	
	switch(z)
	{
	case BSVM2_OPZ_INT:
	case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQI; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEI; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTI; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTI; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEI; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEI; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQI; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEI; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_UINT:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQI; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEI; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTUI; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTUI; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEUI; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEUI; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQI; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEI; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_LONG:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQL; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEL; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTL; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTL; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEL; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEL; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQL; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEL; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_ULONG:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQL; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEL; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTUL; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTUL; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEUL; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEUL; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQL; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEL; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_FLOAT:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQF; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NEF; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTF; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTF; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LEF; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GEF; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQF; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NEF; break;
		default:
			break;
		}
		break;
	case BSVM2_OPZ_DOUBLE:
		switch(o)
		{
		case BSVM2_OPRC_EQ: op->Run=BSVM2_TrOp_JCMP_EQD; break;
		case BSVM2_OPRC_NE: op->Run=BSVM2_TrOp_JCMP_NED; break;
		case BSVM2_OPRC_LT: op->Run=BSVM2_TrOp_JCMP_LTD; break;
		case BSVM2_OPRC_GT: op->Run=BSVM2_TrOp_JCMP_GTD; break;
		case BSVM2_OPRC_LE: op->Run=BSVM2_TrOp_JCMP_LED; break;
		case BSVM2_OPRC_GE: op->Run=BSVM2_TrOp_JCMP_GED; break;
		case BSVM2_OPRC_EQQ: op->Run=BSVM2_TrOp_JCMP_EQD; break;
		case BSVM2_OPRC_NEQ: op->Run=BSVM2_TrOp_JCMP_NED; break;
		default:
			break;
		}
		break;
	}
}

void BSVM2_Interp_SetupTopCallG(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	char *s;
	int i, j;
	
	i=BSVM2_Interp_DecodeOpUCxI(cblk);
	i=cblk->gitab[i];
	if(i&3)
	{
		return;
	}

	vi=BS2I_ImageGetGlobal(cblk->img, i>>2);
	op->v.p=vi;
	
	cblk->stkpos-=vi->nargs;
	op->t1=cblk->stkpos;

	if(vi->brty!=BSVM2_OPZ_VOID)
		{ op->t0=cblk->stkpos++; }
	else
		{ op->t0=cblk->stkpos; }

	if(vi->cblk)
	{
		op->Run=BSVM2_TrOp_CALLG;
		return;
	}
}

BSVM2_TailOpcode *BSVM2_Interp_DecodeTailOpcode(
	BSVM2_CodeBlock *cblk, int opn)
{
	BSVM2_TailOpcode *tmp;
	int i;
	
	tmp=BSVM2_Interp_AllocTailOpcode(cblk);
	switch(opn)
	{
	case BSVM2_OP_JEQ:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JEQ);
		break;
	case BSVM2_OP_JNE:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JNE);
		break;
	case BSVM2_OP_JLT:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JLT);
		break;
	case BSVM2_OP_JGT:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JGT);
		break;
	case BSVM2_OP_JLE:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JLE);
		break;
	case BSVM2_OP_JGE:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JGE);
		break;
	case BSVM2_OP_JCMP:
		BSVM2_Interp_SetupTopJCMP(cblk, tmp);
		break;
	case BSVM2_OP_JMP:
		BSVM2_Interp_SetupTopPopUnJmp(cblk, tmp, BSVM2_TrOp_JMP);
		break;

	case BSVM2_OP_RETI:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_JCMP_RETI);
		break;
	case BSVM2_OP_RETL:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_JCMP_RETL);
		break;
	case BSVM2_OP_RETF:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_JCMP_RETF);
		break;
	case BSVM2_OP_RETD:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_JCMP_RETD);
		break;
	case BSVM2_OP_RETA:
		BSVM2_Interp_SetupTopPopUn(cblk, tmp, BSVM2_TrOp_JCMP_RETA);
		break;
	case BSVM2_OP_RETV:
		BSVM2_Interp_SetupTopUat(cblk, tmp, BSVM2_TrOp_JCMP_RETV);
		break;

	case BSVM2_OP_CALLG:
		BSVM2_Interp_SetupTopCallG(cblk, tmp);
		break;

	default:
		BSVM2_Interp_FreeTailOpcode(cblk, tmp);
		tmp=NULL;
		break;
	}

	return(tmp);
}
