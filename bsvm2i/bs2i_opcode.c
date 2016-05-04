#include <bteifgl.h>

BSVM2_Opcode *BSVM2_Interp_DecodeOpcode(
	BSVM2_CodeBlock *cblk, int opn)
{
	BSVM2_Opcode *op;
	int opn2;
	int i;
	
	op=BSVM2_Interp_AllocOpcode(cblk);
	switch(opn)
	{
	case BSVM2_OP_ADDI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_ADDI);
		break;
	case BSVM2_OP_SUBI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_SUBI);
		break;
	case BSVM2_OP_MULI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_MULI);
		break;
	case BSVM2_OP_ANDI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_ANDI);
		break;
	case BSVM2_OP_ORI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_ORI);
		break;
	case BSVM2_OP_XORI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_XORI);
		break;
	case BSVM2_OP_SHLI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_SHLI);
		break;
	case BSVM2_OP_SARI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_SARI);
		break;
	case BSVM2_OP_ADDL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_ADDL);
		break;
	case BSVM2_OP_SUBL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_SUBL);
		break;
	case BSVM2_OP_MULL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_MULL);
		break;
	case BSVM2_OP_ANDL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_ANDL);
		break;
	case BSVM2_OP_ORL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_ORL);
		break;
	case BSVM2_OP_XORL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_XORL);
		break;
	case BSVM2_OP_SHLL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_SHLL);
		break;
	case BSVM2_OP_SARL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_SARL);
		break;
	case BSVM2_OP_ADDF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_ADDF);
		break;
	case BSVM2_OP_SUBF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_SUBF);
		break;
	case BSVM2_OP_MULF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_MULF);
		break;
	case BSVM2_OP_DIVF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_DIVF);
		break;
	case BSVM2_OP_ADDD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_ADDD);
		break;
	case BSVM2_OP_SUBD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_SUBD);
		break;
	case BSVM2_OP_MULD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_MULD);
		break;
	case BSVM2_OP_DIVD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_DIVD);
		break;

	case BSVM2_OP_NEGI:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_NEGI);
		break;
	case BSVM2_OP_NEGL:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_NEGL);
		break;
	case BSVM2_OP_NEGF:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_NEGF);
		break;
	case BSVM2_OP_NEGD:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_NEGD);
		break;
	case BSVM2_OP_NOTI:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_NOTI);
		break;
	case BSVM2_OP_NOTL:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_NOTL);
		break;
	case BSVM2_OP_LNTI:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LNTI);
		break;
	case BSVM2_OP_LNTL:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LNTL);
		break;

	case BSVM2_OP_LDI:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDI);
		break;
	case BSVM2_OP_LDL:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_LDL);
		break;
	case BSVM2_OP_LDF:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_LDF);
		break;
	case BSVM2_OP_LDD:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_LDD);
		break;
	case BSVM2_OP_LDA:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_ADDRESS, BSVM2_Op_LDA);
		break;

	case BSVM2_OP_STI:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_STI);
		break;
	case BSVM2_OP_STL:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_STL);
		break;
	case BSVM2_OP_STF:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_STF);
		break;
	case BSVM2_OP_STD:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_STD);
		break;
	case BSVM2_OP_STA:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_ADDRESS, BSVM2_Op_STA);
		break;
	case BSVM2_OP_LDC:
		BSVM2_Interp_DecodeOpZx(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDCI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDCL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDCF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDCD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_LDCA);
			break;
		default:
			break;
		}
		break;

	case BSVM2_OP_NEWARR:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		
		if(op->i0==0)
		{
			switch(op->i1)
			{
			case BSVM2_OPZ_INT:	
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_I);
				break;
			case BSVM2_OPZ_UINT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_UI);
				break;
			case BSVM2_OPZ_SBYTE:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_SB);
				break;
			case BSVM2_OPZ_UBYTE:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_UB);
				break;
			case BSVM2_OPZ_SHORT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_SS);
				break;
			case BSVM2_OPZ_USHORT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_US);
				break;
			case BSVM2_OPZ_LONG:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_L);
				break;
			case BSVM2_OPZ_ULONG:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_UL);
				break;
			case BSVM2_OPZ_FLOAT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_F);
				break;
			case BSVM2_OPZ_DOUBLE:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_D);
				break;
			case BSVM2_OPZ_ADDR:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_A);
				break;
			}
			break;
		}
		
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_IC);
			break;
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_UIC);
			break;
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_SBC);
			break;
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_UBC);
			break;
		case BSVM2_OPZ_SHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_SSC);
			break;
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_USC);
			break;
		case BSVM2_OPZ_LONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_LC);
			break;
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_ULC);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_FC);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_DC);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_NEWARR_AC);
			break;
		}
		break;


	case BSVM2_OP_LDOS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSA);
			break;
		}
		break;
	case BSVM2_OP_STOS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopBin(cblk, op,
				BSVM2_Op_STOSI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopBin(cblk, op,
				BSVM2_Op_STOSL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopBin(cblk, op,
				BSVM2_Op_STOSF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopBin(cblk, op,
				BSVM2_Op_STOSD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpPopBin(cblk, op,
				BSVM2_Op_STOSA);
			break;
		}
		break;

	case BSVM2_OP_LDGS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDGSI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDGSL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDGSF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDGSD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDGSA);
			break;
		}
		break;
	case BSVM2_OP_STGS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STGSI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STGSL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STGSF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STGSD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STGSA);
			break;
		}
		break;
		
	case BSVM2_OP_MVI:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVI);
		break;
	case BSVM2_OP_MVL:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVL);
		break;
	case BSVM2_OP_MVF:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVF);
		break;
	case BSVM2_OP_MVD:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVD);
		break;
	case BSVM2_OP_MVA:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVA);
		break;

	case BSVM2_OP_CMPI:
		opn2=BSVM2_Interp_PeekOpcodeNumber(cblk);
		if((opn2>=BSVM2_OP_JEQ) && (opn2<=BSVM2_OP_JGE))
			{ BSVM2_Interp_FreeOpcode(cblk, op); op=NULL; break; }
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPI);
		break;
	case BSVM2_OP_CMPL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPL);
		break;
	case BSVM2_OP_CMPF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPF);
		break;
	case BSVM2_OP_CMPD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPD);
		break;
	case BSVM2_OP_CMPA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPA);
		break;
	case BSVM2_OP_CMP2A:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPA);
		break;
	case BSVM2_OP_CMP2F:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPF);
		break;
	case BSVM2_OP_CMP2D:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPD);
		break;

	case BSVM2_OP_UCMPI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_UCMPI);
		break;
	case BSVM2_OP_UCMPL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_UCMPL);
		break;

	case BSVM2_OP_LDIXI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXI);
		break;
	case BSVM2_OP_LDIXL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_LDIXL);
		break;
	case BSVM2_OP_LDIXF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXF);
		break;
	case BSVM2_OP_LDIXD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXD);
		break;
	case BSVM2_OP_LDIXA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LDIXA);
		break;
	case BSVM2_OP_LDIXSB:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXSB);
		break;
	case BSVM2_OP_LDIXUB:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXUB);
		break;
	case BSVM2_OP_LDIXSS:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXSS);
		break;
	case BSVM2_OP_LDIXUS:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXUS);
		break;
	
#if 0
	case BSVM2_OP_STIXI:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXI);
		break;
	case BSVM2_OP_STIXL:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXL);
		break;
	case BSVM2_OP_STIXF:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXF);
		break;
	case BSVM2_OP_STIXD:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXD);
		break;
	case BSVM2_OP_STIXA:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXA);
		break;
	case BSVM2_OP_STIXB:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXB);
		break;
	case BSVM2_OP_STIXS:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXS);
		break;

	case BSVM2_OP_RSTIXI:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXI);
		break;
	case BSVM2_OP_RSTIXL:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXL);
		break;
	case BSVM2_OP_RSTIXF:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXF);
		break;
	case BSVM2_OP_RSTIXD:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXD);
		break;
	case BSVM2_OP_RSTIXA:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXA);
		break;
	case BSVM2_OP_RSTIXB:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXB);
		break;
	case BSVM2_OP_RSTIXS:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXS);
		break;
#endif

#if 1
	case BSVM2_OP_STIXI:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXI);
		break;
	case BSVM2_OP_STIXL:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXL);
		break;
	case BSVM2_OP_STIXF:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXF);
		break;
	case BSVM2_OP_STIXD:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXD);
		break;
	case BSVM2_OP_STIXA:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXA);
		break;
	case BSVM2_OP_STIXB:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXB);
		break;
	case BSVM2_OP_STIXS:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXS);
		break;

	case BSVM2_OP_RSTIXI:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXI);
		break;
	case BSVM2_OP_RSTIXL:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXL);
		break;
	case BSVM2_OP_RSTIXF:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXF);
		break;
	case BSVM2_OP_RSTIXD:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXD);
		break;
	case BSVM2_OP_RSTIXA:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXA);
		break;
	case BSVM2_OP_RSTIXB:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXB);
		break;
	case BSVM2_OP_RSTIXS:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXS);
		break;
#endif

	case BSVM2_OP_LDIXIC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXIC);
		break;
	case BSVM2_OP_LDIXLC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_LONG, BSVM2_Op_LDIXLC);
		break;
	case BSVM2_OP_LDIXFC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXFC);
		break;
	case BSVM2_OP_LDIXDC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXDC);
		break;
	case BSVM2_OP_LDIXAC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LDIXAC);
		break;
	case BSVM2_OP_LDIXSBC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXSBC);
		break;
	case BSVM2_OP_LDIXUBC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXUBC);
		break;
	case BSVM2_OP_LDIXSSC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXSSC);
		break;
	case BSVM2_OP_LDIXUSC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXUSC);
		break;

#if 0
	case BSVM2_OP_STIXIC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXIC);
		break;
	case BSVM2_OP_STIXLC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXLC);
		break;
	case BSVM2_OP_STIXFC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXFC);
		break;
	case BSVM2_OP_STIXDC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXDC);
		break;
	case BSVM2_OP_STIXAC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXAC);
		break;
	case BSVM2_OP_STIXBC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXBC);
		break;
	case BSVM2_OP_STIXSC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXSC);
		break;

	case BSVM2_OP_RSTIXIC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXIC);
		break;
	case BSVM2_OP_RSTIXLC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXLC);
		break;
	case BSVM2_OP_RSTIXFC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXFC);
		break;
	case BSVM2_OP_RSTIXDC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXDC);
		break;
	case BSVM2_OP_RSTIXAC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXAC);
		break;
	case BSVM2_OP_RSTIXBC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXBC);
		break;
	case BSVM2_OP_RSTIXSC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXSC);
		break;
#endif

#if 1
	case BSVM2_OP_STIXIC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXIC);
		break;
	case BSVM2_OP_STIXLC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXLC);
		break;
	case BSVM2_OP_STIXFC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXFC);
		break;
	case BSVM2_OP_STIXDC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXDC);
		break;
	case BSVM2_OP_STIXAC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXAC);
		break;
	case BSVM2_OP_STIXBC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXBC);
		break;
	case BSVM2_OP_STIXSC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXSC);
		break;

	case BSVM2_OP_RSTIXIC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXIC);
		break;
	case BSVM2_OP_RSTIXLC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXLC);
		break;
	case BSVM2_OP_RSTIXFC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXFC);
		break;
	case BSVM2_OP_RSTIXDC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXDC);
		break;
	case BSVM2_OP_RSTIXAC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXAC);
		break;
	case BSVM2_OP_RSTIXBC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXBC);
		break;
	case BSVM2_OP_RSTIXSC:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXSC);
		break;
#endif

	case BSVM2_OP_STIXZLL:
		BSVM2_Interp_DecodeOpZiIx(cblk, op);
		switch(op->i2)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXILL);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXLLL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXFLL);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXDLL);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXALL);
			break;
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXBLL);
			break;
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXSLL);
			break;
		}
		break;
	case BSVM2_OP_STIXZLC:
		BSVM2_Interp_DecodeOpZiCi(cblk, op);
		switch(op->i2)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXILC);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXLLC);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXFLC);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXDLC);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXALC);
			break;
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXBLC);
			break;
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STIXSLC);
			break;
		}
		break;

#if 1
	case BSVM2_OP_LDIXZLL:
		BSVM2_Interp_DecodeOpZiIx(cblk, op);
		switch(op->i2)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXILL);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDIXLLL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXFLL);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXDLL);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_LDIXALL);
			break;
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSBLL);
			break;
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUBLL);
			break;
		case BSVM2_OPZ_SHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSSLL);
			break;
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUSLL);
			break;
		}
		break;
	case BSVM2_OP_LDIXZLC:
		BSVM2_Interp_DecodeOpZiCi(cblk, op);
		switch(op->i2)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXILC);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDIXLLC);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXFLC);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXDLC);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_LDIXALC);
			break;
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSBLC);
			break;
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUBLC);
			break;
		case BSVM2_OPZ_SHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSSLC);
			break;
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUSLC);
			break;
		}
		break;
#endif

	case BSVM2_OP_CVTI2L:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_CVTI2L);
		break;
	case BSVM2_OP_CVTI2F:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_CVTI2F);
		break;
	case BSVM2_OP_CVTI2D:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_CVTI2D);
		break;
	case BSVM2_OP_CVTL2I:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CVTL2I);
		break;
	case BSVM2_OP_CVTL2F:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_CVTL2F);
		break;
	case BSVM2_OP_CVTL2D:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_CVTL2D);
		break;
	case BSVM2_OP_CVTF2I:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CVTF2I);
		break;
	case BSVM2_OP_CVTF2L:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_CVTF2L);
		break;
	case BSVM2_OP_CVTF2D:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_CVTF2D);
		break;
	case BSVM2_OP_CVTD2I:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CVTD2I);
		break;
	case BSVM2_OP_CVTD2L:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_CVTD2L);
		break;
	case BSVM2_OP_CVTD2F:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_CVTD2F);
		break;
	case BSVM2_OP_CVTSB2I:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CVTSB2I);
		break;
	case BSVM2_OP_CVTUB2I:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CVTUB2I);
		break;
	case BSVM2_OP_CVTSS2I:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CVTSS2I);
		break;
	case BSVM2_OP_CVTUS2I:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CVTUS2I);
		break;
	
	case BSVM2_OP_POPI:
	case BSVM2_OP_POPL:
	case BSVM2_OP_POPF:
	case BSVM2_OP_POPD:
	case BSVM2_OP_POPA:
		BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_NOP);
		break;
	case BSVM2_OP_DUPI:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_DUPI);
		break;
	case BSVM2_OP_DUPL:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_DUPL);
		break;
	case BSVM2_OP_DUPF:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_DUPF);
		break;
	case BSVM2_OP_DUPD:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_DUPD);
		break;
	case BSVM2_OP_DUPA:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_ADDRESS, BSVM2_Op_DUPA);
		break;
	case BSVM2_OP_PUSHI:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_PUSHI);
		break;
	case BSVM2_OP_PUSHL:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_PUSHL);
		break;
	case BSVM2_OP_PUSHF:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_PUSHF);
		break;
	case BSVM2_OP_PUSHD:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_PUSHD);
		break;
	case BSVM2_OP_PUSHA:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_ADDRESS, BSVM2_Op_PUSHA);
		break;
	case BSVM2_OP_SWAPA:
		BSVM2_Interp_SetupOpUat2(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_SWAPA);
		break;
	
	case BSVM2_OP_ADDIC:
		BSVM2_Interp_SetupOpBinCJ(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_ADDIC);
		break;
	case BSVM2_OP_SUBIC:
		BSVM2_Interp_SetupOpBinCJ(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_SUBIC);
		break;
	case BSVM2_OP_MULIC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_MULIC);
		break;
	case BSVM2_OP_ANDIC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_ANDIC);
		break;
	case BSVM2_OP_ORIC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_ORIC);
		break;
	case BSVM2_OP_XORIC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_XORIC);
		break;

	case BSVM2_OP_SHLIC:
		BSVM2_Interp_SetupOpBinCiSa(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_SHLIC, BSVM2_Op_SHRIC);
		break;
	case BSVM2_OP_SARIC:
		BSVM2_Interp_SetupOpBinCiSa(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_SARIC, BSVM2_Op_SHLIC);
		break;

	case BSVM2_OP_ADDIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_ADDIL);
		break;
	case BSVM2_OP_SUBIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_SUBIL);
		break;
	case BSVM2_OP_MULIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_MULIL);
		break;
	case BSVM2_OP_ANDIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_ANDIL);
		break;
	case BSVM2_OP_ORIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_ORIL);
		break;
	case BSVM2_OP_XORIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_XORIL);
		break;
	case BSVM2_OP_SHLIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_SHLIL);
		break;
	case BSVM2_OP_SARIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_SARIL);
		break;

	case BSVM2_OP_CMPIC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPIC);
		break;
	case BSVM2_OP_CMPIL:
		BSVM2_Interp_SetupOpBinL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPIL);
		break;
	case BSVM2_OP_CMPILC:
		BSVM2_Interp_SetupOpBinLC(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPILC);
		break;
	case BSVM2_OP_CMPILL:
		opn2=BSVM2_Interp_PeekIxOpcodeNumber(cblk);
		if((opn2>=BSVM2_OP_JEQ) && (opn2<=BSVM2_OP_JGE))
			{ BSVM2_Interp_FreeOpcode(cblk, op); op=NULL; break; }

		BSVM2_Interp_SetupOpBinLL(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPILL);
		break;
	case BSVM2_OP_DCMPIC:
		BSVM2_Interp_SetupOpBinC2P(cblk, op,
			BSVM2_OPZ_INT, BSVM2_OPZ_INT, BSVM2_Op_DCMPIC);
		break;

	case BSVM2_OP_INCI:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_INCI);
		break;
	case BSVM2_OP_DECI:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_DECI);
		break;
	case BSVM2_OP_INCIL:
		BSVM2_Interp_SetupOpUatL(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_INCIL);
		break;
	case BSVM2_OP_DECIL:
		BSVM2_Interp_SetupOpUatL(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_DECIL);
		break;


	case BSVM2_OP_ADDAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_ADDAA);
		break;
	case BSVM2_OP_SUBAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_SUBAA);
		break;
	case BSVM2_OP_MULAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_MULAA);
		break;
	case BSVM2_OP_ANDAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_ANDAA);
		break;
	case BSVM2_OP_ORAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_ORAA);
		break;
	case BSVM2_OP_XORAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_XORAA);
		break;
	case BSVM2_OP_SHLAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_SHLAA);
		break;
	case BSVM2_OP_SARAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_SARAA);
		break;
	case BSVM2_OP_SHRAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_SHRAA);
		break;
	case BSVM2_OP_DIVAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_DIVAA);
		break;
	case BSVM2_OP_MODAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_MODAA);
		break;
	case BSVM2_OP_NEGAA:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_NEGAA);
		break;
	case BSVM2_OP_NOTAA:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_NOTAA);
		break;
	case BSVM2_OP_LNTAA:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LNTAA);
		break;
	case BSVM2_OP_CMPAA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPAA);
		break;
	case BSVM2_OP_LDCA:
		BSVM2_Interp_DecodeOpZy(cblk, op);
		BSVM2_Interp_SetupOpUnP(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDCA);
		break;

	case BSVM2_OP_CVTI2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTI2AA);
		break;
	case BSVM2_OP_CVTL2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTL2AA);
		break;
	case BSVM2_OP_CVTF2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTF2AA);
		break;
	case BSVM2_OP_CVTD2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTD2AA);
		break;

	case BSVM2_OP_CVTAA2I:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CVTAA2I);
		break;
	case BSVM2_OP_CVTAA2L:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_LONG, BSVM2_Op_CVTAA2L);
		break;
	case BSVM2_OP_CVTAA2F:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_CVTAA2F);
		break;
	case BSVM2_OP_CVTAA2D:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_CVTAA2D);
		break;

#if 1
	case BSVM2_OP_CVTAA2IN:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CVTAA2I);
		break;
	case BSVM2_OP_CVTAA2LN:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_LONG, BSVM2_Op_CVTAA2L);
		break;
	case BSVM2_OP_CVTAA2FN:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_CVTAA2F);
		break;
	case BSVM2_OP_CVTAA2DN:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_CVTAA2D);
		break;
#endif

	case BSVM2_OP_MVIC:
		BSVM2_Interp_SetupOpUatKx(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_MVIC);
		break;
	case BSVM2_OP_MVLC:
		BSVM2_Interp_SetupOpUatKx(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_MVLC);
		break;
	case BSVM2_OP_MVFC:
		BSVM2_Interp_SetupOpUatKx(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_MVFC);
		break;
	case BSVM2_OP_MVDC:
		BSVM2_Interp_SetupOpUatKx(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_MVDC);
		break;

	case BSVM2_OP_LEARI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LEARI);
		break;
	case BSVM2_OP_LEARL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LEARL);
		break;
	case BSVM2_OP_LEARF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LEARF);
		break;
	case BSVM2_OP_LEARD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LEARD);
		break;
	case BSVM2_OP_LEARA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LEARA);
		break;
	case BSVM2_OP_LEARB:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LEARB);
		break;
	case BSVM2_OP_LEARS:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LEARS);
		break;
	case BSVM2_OP_LEAST:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LEAST);
		break;

	case BSVM2_OP_LEARIC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARIC);
		break;
	case BSVM2_OP_LEARLC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARLC);
		break;
	case BSVM2_OP_LEARFC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARFC);
		break;
	case BSVM2_OP_LEARDC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARDC);
		break;
	case BSVM2_OP_LEARAC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARAC);
		break;
	case BSVM2_OP_LEARBC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARBC);
		break;
	case BSVM2_OP_LEARSC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARSC);
		break;
	case BSVM2_OP_LEASTC:
		BSVM2_Interp_SetupOpBinC(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARSC);
		break;

	default:
		BSVM2_Interp_FreeOpcode(cblk, op);
		op=NULL;
		break;
	}
	
	return(op);
}
