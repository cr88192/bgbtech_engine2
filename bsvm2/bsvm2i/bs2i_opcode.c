/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


BSVM2_Opcode *BSVM2_Interp_DecodeOpcode(
	BSVM2_CodeBlock *cblk, int opn)
{
	BSVM2_ImageGlobal *vari, *vi;
	BSVM2_Opcode *op;
	int opn2;
	int i;
	
	op=BSVM2_Interp_AllocOpcode(cblk);
	op->opn=opn;
	
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
	case BSVM2_OP_DIVI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_DIVI);
		break;
	case BSVM2_OP_MODI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_MODI);
		break;
	case BSVM2_OP_SHRI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_SHRI);
		break;
	case BSVM2_OP_UDIVI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_UDIVI);
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
	case BSVM2_OP_DIVL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_DIVL);
		break;
	case BSVM2_OP_MODL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_MODL);
		break;
	case BSVM2_OP_SHRL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_SHRL);
		break;
	case BSVM2_OP_UDIVL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_UDIVL);
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
		case BSVM2_OPZ_CI_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDCF);
			break;
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_CI_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDCD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_LDCA);
			break;
		case BSVM2_OPZ_CONST:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_LDCA);
			switch(op->v.i)
			{
			case 0: op->v.a=DTV_NULL; break;
			case 1: op->v.a=DTV_UNDEFINED; break;
			case 2: op->v.a=DTV_TRUE; break;
			case 3: op->v.a=DTV_FALSE; break;
			case 4: op->Run=BSVM2_Op_LDCTH; break;
			default: break;
			}
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_LDRL:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		BSVM2_Interp_SetupOpUnP(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_LDRL);
		break;
	case BSVM2_OP_LDRGS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		BSVM2_Interp_SetupOpUnP(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_LDRGS);
		break;
	case BSVM2_OP_LDROS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		BSVM2_Interp_SetupOpUnP(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_LDROS);
		break;

	case BSVM2_OP_LDLX:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDLXI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDLXL);
			break;
		case BSVM2_OPZ_FLOAT:
		case BSVM2_OPZ_CI_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDLXF);
			break;
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_CI_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDLXD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_LDLXA);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_STLX:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STLXI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STLXL);
			break;
		case BSVM2_OPZ_FLOAT:
		case BSVM2_OPZ_CI_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STLXF);
			break;
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_CI_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STLXD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STLXA);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_LDDRLX:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRLXI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDDRLXL);
			break;
		case BSVM2_OPZ_FLOAT:
		case BSVM2_OPZ_CI_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDDRLXF);
			break;
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_CI_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDDRLXD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_LDDRLXA);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_STDRLX:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STDRLXI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STDRLXL);
			break;
		case BSVM2_OPZ_FLOAT:
		case BSVM2_OPZ_CI_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STDRLXF);
			break;
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_CI_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STDRLXD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STDRLXA);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_MKLFCN:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		BSVM2_Interp_SetupOpUnP(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_MKLFCN);
		break;
	case BSVM2_OP_IFXLFCN:
		BSVM2_Interp_DecodeOpGj(cblk, op);
		BSVM2_Interp_SetupOpUat(cblk, op,
			BSVM2_Op_IFXLFCN);
		break;
	case BSVM2_OP_DFXLFCN:
		BSVM2_Interp_DecodeOpGj(cblk, op);
		BSVM2_Interp_SetupOpUat(cblk, op,
			BSVM2_Op_DFXLFCN);
		break;

	case BSVM2_OP_DSTIXUZ:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZ_I);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZ_L);
			break;
		case BSVM2_OPZ_FLOAT:
		case BSVM2_OPZ_CI_FLOAT:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZ_F);
			break;
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_CI_DOUBLE:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZ_D);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZ_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_DSTIXUZL:
		BSVM2_Interp_DecodeOpZiIx(cblk, op);
		switch(op->z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZL_I);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZL_L);
			break;
		case BSVM2_OPZ_FLOAT:
		case BSVM2_OPZ_CI_FLOAT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZL_F);
			break;
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_CI_DOUBLE:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZL_D);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZL_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_DSTIXUZRL:
		BSVM2_Interp_DecodeOpZiIx(cblk, op);
		switch(op->z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZRL_I);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZRL_L);
			break;
		case BSVM2_OPZ_FLOAT:
		case BSVM2_OPZ_CI_FLOAT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZRL_F);
			break;
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_CI_DOUBLE:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZRL_D);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_DSTIXUZRL_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	
	case BSVM2_OP_DIFFPTR:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:	
		case BSVM2_OPZ_FLOAT:	
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_DIFFPTR_I);
			break;
		case BSVM2_OPZ_LONG:	
		case BSVM2_OPZ_ULONG:	
		case BSVM2_OPZ_DOUBLE:	
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_DIFFPTR_L);
			break;
		case BSVM2_OPZ_ADDRESS:	
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_DIFFPTR_A);
			break;
		case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_DIFFPTR_B);
			break;
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_DIFFPTR_S);
			break;
		case BSVM2_OPZ_X128:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_DIFFPTR_X);
			break;
		default:
			BSVM2_DBGTRAP
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
			default:
				BSVM2_DBGTRAP
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
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;


#if 1
	case BSVM2_OP_IFXARR:
//		BSVM2_Interp_DecodeOpZn(cblk, op);
		BSVM2_Interp_DecodeOpZnIx(cblk, op);
		
		if(op->i0==0)
		{
			switch(op->z)
			{
			case BSVM2_OPZ_INT:	
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_I);
				break;
			case BSVM2_OPZ_UINT:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_UI);
				break;
			case BSVM2_OPZ_SBYTE:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_SB);
				break;
			case BSVM2_OPZ_UBYTE:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_UB);
				break;
			case BSVM2_OPZ_SHORT:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_SS);
				break;
			case BSVM2_OPZ_USHORT:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_US);
				break;
			case BSVM2_OPZ_LONG:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_L);
				break;
			case BSVM2_OPZ_ULONG:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_UL);
				break;
			case BSVM2_OPZ_FLOAT:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_F);
				break;
			case BSVM2_OPZ_DOUBLE:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_D);
				break;
			case BSVM2_OPZ_ADDR:
				BSVM2_Interp_SetupOpPopUn(cblk, op,
					BSVM2_Op_IFXARR_A);
				break;
			default:
				BSVM2_DBGTRAP
				break;
			}
			break;
		}
		
		switch(op->z)
		{
		case BSVM2_OPZ_INT:	
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_IC);
			break;
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_UIC);
			break;
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_SBC);
			break;
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_UBC);
			break;
		case BSVM2_OPZ_SHORT:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_SSC);
			break;
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_USC);
			break;
		case BSVM2_OPZ_LONG:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_LC);
			break;
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_ULC);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_FC);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_DC);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpUat(cblk, op,
				BSVM2_Op_IFXARR_AC);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_DFXARR:
		BSVM2_Interp_DecodeOpZnIx(cblk, op);
		BSVM2_Interp_SetupOpUat(cblk, op, BSVM2_Op_DFXARR);
		break;
#endif

	case BSVM2_OP_LDOS:
		BSVM2_Interp_DecodeOpGx(cblk, op);

		vi=op->v.p;
		if(!vi || !vi->objinf)
			{ BSVM2_DBGTRAP }
		
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
		case BS2CC_TYZ_INT128:
		case BS2CC_TYZ_VEC4F:
		case BSVM2_OPZ_X128:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSX);
			break;
		case BSVM2_OPZ_CONST:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSAA);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_STOS:
		BSVM2_Interp_DecodeOpGx(cblk, op);

		vi=op->v.p;
		if(!vi || !vi->objinf)
			{ BSVM2_DBGTRAP }

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
		case BS2CC_TYZ_INT128:
		case BS2CC_TYZ_VEC4F:
		case BSVM2_OPZ_X128:
			BSVM2_Interp_SetupOpPopBin(cblk, op,
				BSVM2_Op_STOSX);
			break;
		case BSVM2_OPZ_CONST:
			BSVM2_Interp_SetupOpPopBin(cblk, op,
				BSVM2_Op_STOSAA);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_LDGS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		
		vari=op->v.p;
		if(vari->flags&BS2CC_TYFL_DYNAMIC)
		{
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDGS_DY);
			break;
		}
		
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
		case BS2CC_TYZ_INT128:
		case BS2CC_TYZ_VEC4F:
		case BSVM2_OPZ_X128:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_X128, BSVM2_Op_LDGSX);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_STGS:
		BSVM2_Interp_DecodeOpGx(cblk, op);

		vari=op->v.p;
		if(vari->flags&BS2CC_TYFL_DYNAMIC)
		{
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STGS_DY);
			break;
		}

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
		case BS2CC_TYZ_INT128:
		case BS2CC_TYZ_VEC4F:
		case BSVM2_OPZ_X128:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STGSX);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_LDOSL:
		BSVM2_Interp_DecodeOpGj(cblk, op);

		vi=op->v.p;
		if(!vi || !vi->objinf)
			{ BSVM2_DBGTRAP }

		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSLI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSLL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSLF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSLD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSLA);
			break;
		case BSVM2_OPZ_CONST:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSLAA);
			break;
		case BS2CC_TYZ_INT128:
		case BS2CC_TYZ_VEC4F:
		case BSVM2_OPZ_X128:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDOSLX);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_STOSL:
		BSVM2_Interp_DecodeOpGj(cblk, op);

		vi=op->v.p;
		if(!vi || !vi->objinf)
			{ BSVM2_DBGTRAP }

		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STOSLI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STOSLL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STOSLF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STOSLD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STOSLA);
			break;
		case BSVM2_OPZ_CONST:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STOSLAA);
			break;
		case BS2CC_TYZ_INT128:
		case BS2CC_TYZ_VEC4F:
		case BSVM2_OPZ_X128:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STOSLX);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_NEWOBJ:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		BSVM2_Interp_SetupOpUnP(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_NEWOBJ);
		break;
	case BSVM2_OP_DELETE:
		BSVM2_Interp_SetupOpPopUn(cblk, op,
			BSVM2_Op_DELOBJ);
		break;

	case BSVM2_OP_NEWDYO:
		BSVM2_Interp_SetupOpUnP(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_NEWDYO);
		break;
	case BSVM2_OP_SBOS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		if(op->i1==BSVM2_OPZ_CONST)
		{
			BSVM2_Interp_SetupOpPopBin(cblk, op,
				BSVM2_Op_SBOSAA);
			break;
		}
		BSVM2_Interp_SetupOpPopBin(cblk, op,
			BSVM2_Op_SBOS);
		break;
	case BSVM2_OP_DSBOS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		if(op->i1==BSVM2_OPZ_CONST)
		{
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_DSBOSAA);
			break;
		}
		BSVM2_Interp_SetupOpPopUn(cblk, op,
			BSVM2_Op_DSBOS);
		break;
	case BSVM2_OP_DSTOS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		if(op->i1==BSVM2_OPZ_CONST)
		{
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_DSTOSAA);
			break;
		}
		BSVM2_Interp_SetupOpPopUn(cblk, op,
			BSVM2_Op_DSTOS);
		break;

	case BSVM2_OP_ISTYPE:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		if(op->i1==BSVM2_OPZ_CONST)
		{
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_ISTYPE_AA);
			BSVM2_OpInit_IsTypeS(cblk, op, op->v.p);
			break;
		}
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_ISTYPE);
		break;
	case BSVM2_OP_DTNISTYPE:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		op->opfl|=BSVM2_TRFL_CANTHROW;
		if(op->i1==BSVM2_OPZ_CONST)
		{
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_DTNISTYPE_AA);
			break;
		}
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_DTNISTYPE);
		break;
	case BSVM2_OP_DZNISTYPE:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		if(op->i1==BSVM2_OPZ_CONST)
		{
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_DZNISTYPE_AA);
			break;
		}
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_DZNISTYPE);
		break;

	case BSVM2_OP_HTNULL:
		opn2=BSVM2_Interp_PeekOpcodeNumber(cblk);
//		if((opn2>=BSVM2_OP_JEQ) && (opn2<=BSVM2_OP_JGE))
//			{ BSVM2_Interp_FreeOpcode(cblk, op); op=NULL; break; }
		switch(opn2)
		{
		case BSVM2_OP_LDOS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_DecodeOpGx(cblk, op);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			switch(op->i1)
			{
			case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
			case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
			case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_LDOSI_TN);
				break;
			case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_LDOSL_TN);
				break;
			case BSVM2_OPZ_FLOAT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_LDOSF_TN);
				break;
			case BSVM2_OPZ_DOUBLE:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_LDOSD_TN);
				break;
			case BSVM2_OPZ_ADDRESS:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_LDOSA_TN);
				break;
			case BS2CC_TYZ_INT128:
			case BS2CC_TYZ_VEC4F:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_LDOSX);
				break;
			case BSVM2_OPZ_CONST:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_ADDR, BSVM2_Op_LDOSAA);
				break;
			}
			break;
		case BSVM2_OP_STOS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_DecodeOpGx(cblk, op);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			switch(op->i1)
			{
			case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
			case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
			case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
				BSVM2_Interp_SetupOpPopBin(cblk, op,
					BSVM2_Op_STOSI_TN);
				break;
			case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
				BSVM2_Interp_SetupOpPopBin(cblk, op,
					BSVM2_Op_STOSL_TN);
				break;
			case BSVM2_OPZ_FLOAT:
				BSVM2_Interp_SetupOpPopBin(cblk, op,
					BSVM2_Op_STOSF_TN);
				break;
			case BSVM2_OPZ_DOUBLE:
				BSVM2_Interp_SetupOpPopBin(cblk, op,
					BSVM2_Op_STOSD_TN);
				break;
			case BSVM2_OPZ_ADDRESS:
				BSVM2_Interp_SetupOpPopBin(cblk, op,
					BSVM2_Op_STOSA_TN);
				break;
			case BS2CC_TYZ_INT128:
			case BS2CC_TYZ_VEC4F:
				BSVM2_Interp_SetupOpPopBin(cblk, op,
					BSVM2_Op_STOSX);
				break;
			case BSVM2_OPZ_CONST:
				BSVM2_Interp_SetupOpPopBin(cblk, op,
					BSVM2_Op_STOSAA);
				break;
			}
			break;

		default:
			opn2=-1;
			break;
		}

		if(opn2<0)
		{
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_DTRAPNULL);
		}
		break;

	case BSVM2_OP_DTRAPNULL:
		op->opfl|=BSVM2_TRFL_CANTHROW;
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_DTRAPNULL);
		break;
	case BSVM2_OP_LTRAPNULL:
		BSVM2_Interp_DecodeOpIx(cblk, op);
		op->opfl|=BSVM2_TRFL_CANTHROW;
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LTRAPNULL);
		break;

	case BSVM2_OP_LDIXAA:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LDIXAA);
		break;
	case BSVM2_OP_STIXAA:
		BSVM2_Interp_SetupOpPopTrin(cblk, op,
			BSVM2_Op_STIXAA);
		break;
	case BSVM2_OP_LDIXAI:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LDIXAI);
		break;
	case BSVM2_OP_STIXAI:
		BSVM2_Interp_SetupOpPopTrin(cblk, op,
			BSVM2_Op_STIXAI);
		break;

	case BSVM2_OP_LDTHIS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDTHISI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDTHISL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDTHISF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDTHISD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDTHISA);
			break;

		case BS2CC_TYZ_INT128:
		case BS2CC_TYZ_VEC4F:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDTHISX);
			break;
		}
		break;
	case BSVM2_OP_STTHIS:
		BSVM2_Interp_DecodeOpGx(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STTHISI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STTHISL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STTHISF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STTHISD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STTHISA);
			break;

		case BS2CC_TYZ_INT128:
		case BS2CC_TYZ_VEC4F:
			BSVM2_Interp_SetupOpPopUn(cblk, op,
				BSVM2_Op_STTHISX);
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

//	case BSVM2_OP_CMIEQ:
//		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMIEQ);
//		break;
	case BSVM2_OP_CMINE:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMINE);
		break;
	case BSVM2_OP_CMILT:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMILT);
		break;
	case BSVM2_OP_CMIGT:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMIGT);
		break;
	case BSVM2_OP_CMILE:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMILE);
		break;
	case BSVM2_OP_CMIGE:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMIGE);
		break;

	case BSVM2_OP_HBC:
		opn2=BSVM2_Interp_PeekOpcodeNumber(cblk);
		op->opn2=opn2;
		switch(opn2)
		{
//		case BSVM2_OP_LDOS:
//			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
//			BSVM2_Interp_DecodeOpGx(cblk, op);
//			op->opfl|=BSVM2_TRFL_CANTHROW;

#if 1
		case BSVM2_OP_LDIXI:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXI_BC);
			break;
		case BSVM2_OP_LDIXL:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDIXL_BC);
			break;
		case BSVM2_OP_LDIXF:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXF_BC);
			break;
		case BSVM2_OP_LDIXD:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXD_BC);
			break;
		case BSVM2_OP_LDIXA:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDIXA_BC);
			break;
		case BSVM2_OP_LDIXSB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSB_BC);
			break;
		case BSVM2_OP_LDIXUB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUB_BC);
			break;
		case BSVM2_OP_LDIXSS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSS_BC);
			break;
		case BSVM2_OP_LDIXUS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUS_BC);
			break;

		case BSVM2_OP_STIXI:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXI_BC);
			break;
		case BSVM2_OP_STIXL:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXL_BC);
			break;
		case BSVM2_OP_STIXF:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXF_BC);
			break;
		case BSVM2_OP_STIXD:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXD_BC);
			break;
		case BSVM2_OP_STIXA:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXA_BC);
			break;
		case BSVM2_OP_STIXB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXB_BC);
			break;
		case BSVM2_OP_STIXS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXS_BC);
			break;


		case BSVM2_OP_LDIXIC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXIC_BC);
			break;
		case BSVM2_OP_LDIXLC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDIXLC_BC);
			break;
		case BSVM2_OP_LDIXFC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXFC_BC);
			break;
		case BSVM2_OP_LDIXDC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXDC_BC);
			break;
		case BSVM2_OP_LDIXAC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDIXAC_BC);
			break;
		case BSVM2_OP_LDIXSBC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSBC_BC);
			break;
		case BSVM2_OP_LDIXUBC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUBC_BC);
			break;
		case BSVM2_OP_LDIXSSC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSSC_BC);
			break;
		case BSVM2_OP_LDIXUSC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUSC_BC);
			break;

		case BSVM2_OP_STIXIC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXIC_BC);
			break;
		case BSVM2_OP_STIXLC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXLC_BC);
			break;
		case BSVM2_OP_STIXFC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXFC_BC);
			break;
		case BSVM2_OP_STIXDC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXDC_BC);
			break;
		case BSVM2_OP_STIXAC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXAC_BC);
			break;
		case BSVM2_OP_STIXBC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXBC_BC);
			break;
		case BSVM2_OP_STIXSC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXSC_BC);
			break;
#endif

		default:
//			op->opfl|=BSVM2_TRFL_CANTHROW;
			BSVM2_Interp_SetupOpUat(cblk, op, BSVM2_Op_NOP);
			break;
		}
		break;


	case BSVM2_OP_LDDRAL:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRAL_I);
			break;
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRAL_SB);
			break;
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRAL_UB);
			break;
		case BSVM2_OPZ_SHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRAL_SS);
			break;
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRAL_US);
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDDRAL_L);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDDRAL_F);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDDRAL_D);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDDRAL_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_LDDRVL:
	case BSVM2_OP_LDDRPL:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRVL_I);
			break;
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRVL_SB);
			break;
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRVL_UB);
			break;
		case BSVM2_OPZ_SHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRVL_SS);
			break;
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDRVL_US);
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDDRVL_L);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDDRVL_F);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDDRVL_D);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDDRVL_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_STDRAL:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRAL_I);
			break;
		case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRAL_B);
			break;
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRAL_S);
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRAL_L);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRAL_F);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRAL_D);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRAL_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_STDRVL:
	case BSVM2_OP_STDRPL:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRVL_I);
			break;
		case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRVL_B);
			break;
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRVL_S);
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRVL_L);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRVL_F);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRVL_D);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDRVL_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;

	case BSVM2_OP_CVTA2PTR:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTA2PTR);
		break;
	case BSVM2_OP_CVTPTR2A:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTPTR2A);
		break;

#if 1
	case BSVM2_OP_HPTR:
		opn2=BSVM2_Interp_PeekOpcodeNumber(cblk);
		op->opn2=opn2;
		switch(opn2)
		{
//		case BSVM2_OP_LDOS:
//			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
//			BSVM2_Interp_DecodeOpGx(cblk, op);
//			op->opfl|=BSVM2_TRFL_CANTHROW;

		case BSVM2_OP_CMPA:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_CMPA_P);
			break;

		case BSVM2_OP_LDIXI:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXI_P);
			break;
		case BSVM2_OP_LDIXL:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDIXL_P);
			break;
		case BSVM2_OP_LDIXF:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXF_P);
			break;
		case BSVM2_OP_LDIXD:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXD_P);
			break;
		case BSVM2_OP_LDIXA:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDIXA_P);
			break;
		case BSVM2_OP_LDIXSB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSB_P);
			break;
		case BSVM2_OP_LDIXUB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUB_P);
			break;
		case BSVM2_OP_LDIXSS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSS_P);
			break;
		case BSVM2_OP_LDIXUS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUS_P);
			break;

		case BSVM2_OP_STIXI:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXI_P);
			break;
		case BSVM2_OP_STIXL:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXL_P);
			break;
		case BSVM2_OP_STIXF:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXF_P);
			break;
		case BSVM2_OP_STIXD:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXD_P);
			break;
		case BSVM2_OP_STIXA:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXA_P);
			break;
		case BSVM2_OP_STIXB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXB_P);
			break;
		case BSVM2_OP_STIXS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_RSTIXS_P);
			break;

		case BSVM2_OP_LDIXIC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXIC_P);
			break;
		case BSVM2_OP_LDIXLC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDIXLC_P);
			break;
		case BSVM2_OP_LDIXFC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXFC_P);
			break;
		case BSVM2_OP_LDIXDC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXDC_P);
			break;
		case BSVM2_OP_LDIXAC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDIXAC_P);
			break;
		case BSVM2_OP_LDIXSBC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSBC_P);
			break;
		case BSVM2_OP_LDIXUBC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUBC_P);
			break;
		case BSVM2_OP_LDIXSSC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSSC_P);
			break;
		case BSVM2_OP_LDIXUSC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUSC_P);
			break;

		case BSVM2_OP_STIXIC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXIC_P);
			break;
		case BSVM2_OP_STIXLC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXLC_P);
			break;
		case BSVM2_OP_STIXFC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXFC_P);
			break;
		case BSVM2_OP_STIXDC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXDC_P);
			break;
		case BSVM2_OP_STIXAC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXAC_P);
			break;
		case BSVM2_OP_STIXBC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXBC_P);
			break;
		case BSVM2_OP_STIXSC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_RSTIXSC_P);
			break;

		case BSVM2_OP_LEARI:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARI_P);
			break;
		case BSVM2_OP_LEARL:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARL_P);
			break;
		case BSVM2_OP_LEARF:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARF_P);
			break;
		case BSVM2_OP_LEARD:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARD_P);
			break;
		case BSVM2_OP_LEARA:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARA_P);
			break;
		case BSVM2_OP_LEARB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARB_P);
			break;
		case BSVM2_OP_LEARS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARS_P);
			break;

		case BSVM2_OP_LEARIC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARIC_P);
			break;
		case BSVM2_OP_LEARLC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARLC_P);
			break;
		case BSVM2_OP_LEARFC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARFC_P);
			break;
		case BSVM2_OP_LEARDC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARDC_P);
			break;
		case BSVM2_OP_LEARAC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARAC_P);
			break;
		case BSVM2_OP_LEARBC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARBC_P);
			break;
		case BSVM2_OP_LEARSC:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpBinCI(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LEARSC_P);
			break;

#if 1
		case BSVM2_OP_LDDRI:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXIC_P);
			break;
		case BSVM2_OP_LDDRL:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDIXLC_P);
			break;
		case BSVM2_OP_LDDRF:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXFC_P);
			break;
		case BSVM2_OP_LDDRD:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXDC_P);
			break;
		case BSVM2_OP_LDDRA:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDIXAC_P);
			break;
		case BSVM2_OP_LDDRSB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSBC_P);
			break;
		case BSVM2_OP_LDDRUB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUBC_P);
			break;
		case BSVM2_OP_LDDRSS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXSSC_P);
			break;
		case BSVM2_OP_LDDRUS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDIXUSC_P);
			break;

		case BSVM2_OP_STDRI:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXIC_P);
			break;
		case BSVM2_OP_STDRL:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXLC_P);
			break;
		case BSVM2_OP_STDRF:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXFC_P);
			break;
		case BSVM2_OP_STDRD:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXDC_P);
			break;
		case BSVM2_OP_STDRA:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXAC_P);
			break;
		case BSVM2_OP_STDRB:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXBC_P);
			break;
		case BSVM2_OP_STDRS:
			opn2=BSVM2_Interp_ReadOpcodeNumber(cblk);
			BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXSC_P);
			break;
#endif

		default:
			BSVM2_Interp_SetupOpUat(cblk, op, BSVM2_Op_NOP);
			break;
		}
		break;
#endif

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

#if 1
	case BSVM2_OP_DSTIXIC:
		BSVM2_Interp_SetupOpTrinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_STIXIC);
		break;
	case BSVM2_OP_DSTIXLC:
		BSVM2_Interp_SetupOpTrinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_STIXLC);
		break;
	case BSVM2_OP_DSTIXFC:
		BSVM2_Interp_SetupOpTrinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_STIXFC);
		break;
	case BSVM2_OP_DSTIXDC:
		BSVM2_Interp_SetupOpTrinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_STIXDC);
		break;
	case BSVM2_OP_DSTIXAC:
		BSVM2_Interp_SetupOpTrinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_STIXAC);
		break;
	case BSVM2_OP_DSTIXBC:
		BSVM2_Interp_SetupOpTrinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_STIXBC);
		break;
	case BSVM2_OP_DSTIXSC:
		BSVM2_Interp_SetupOpTrinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_STIXSC);
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

#if 1
	case BSVM2_OP_LDDRI:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXIC);
		break;
	case BSVM2_OP_LDDRL:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_LONG, BSVM2_Op_LDIXLC);
		break;
	case BSVM2_OP_LDDRF:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXFC);
		break;
	case BSVM2_OP_LDDRD:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXDC);
		break;
	case BSVM2_OP_LDDRA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LDIXAC);
		break;
	case BSVM2_OP_LDDRSB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXSBC);
		break;
	case BSVM2_OP_LDDRUB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXUBC);
		break;
	case BSVM2_OP_LDDRSS:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXSSC);
		break;
	case BSVM2_OP_LDDRUS:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXUSC);
		break;

	case BSVM2_OP_STDRI:
		BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXIC);
		break;
	case BSVM2_OP_STDRL:
		BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXLC);
		break;
	case BSVM2_OP_STDRF:
		BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXFC);
		break;
	case BSVM2_OP_STDRD:
		BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXDC);
		break;
	case BSVM2_OP_STDRA:
		BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXAC);
		break;
	case BSVM2_OP_STDRB:
		BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXBC);
		break;
	case BSVM2_OP_STDRS:
		BSVM2_Interp_SetupOpPopBin(cblk, op, BSVM2_Op_RSTIXSC);
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
	case BSVM2_OP_CVTUI2L:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_CVTUI2L);
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

	case BSVM2_OP_SWAPI:
		BSVM2_Interp_SetupOpUat2(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_SWAPI);
		break;
	case BSVM2_OP_SWAPL:
		BSVM2_Interp_SetupOpUat2(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_SWAPL);
		break;
	case BSVM2_OP_SWAPF:
		BSVM2_Interp_SetupOpUat2(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_SWAPF);
		break;
	case BSVM2_OP_SWAPD:
		BSVM2_Interp_SetupOpUat2(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_SWAPD);
		break;
	case BSVM2_OP_SWAPA:
		BSVM2_Interp_SetupOpUat2(cblk, op,
			BSVM2_OPZ_ADDRESS, BSVM2_Op_SWAPA);
		break;

	case BSVM2_OP_DROPAA:
		BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_DROPAA);
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

#if 1
	case BSVM2_OP_ADDISLC:
		BSVM2_Interp_SetupOpUI_SLC(cblk, op, BSVM2_Op_ADDI_LLC);
		break;
	case BSVM2_OP_SUBISLC:
		BSVM2_Interp_SetupOpUI_SLC(cblk, op, BSVM2_Op_SUBI_LLC);
		break;
	case BSVM2_OP_MULISLC:
		BSVM2_Interp_SetupOpI_SLC(cblk, op, BSVM2_Op_MULI_LLC);
		break;
	case BSVM2_OP_ANDISLC:
		BSVM2_Interp_SetupOpI_SLC(cblk, op, BSVM2_Op_ANDI_LLC);
		break;
	case BSVM2_OP_ORISLC:
		BSVM2_Interp_SetupOpI_SLC(cblk, op, BSVM2_Op_ORI_LLC);
		break;
	case BSVM2_OP_XORISLC:
		BSVM2_Interp_SetupOpI_SLC(cblk, op, BSVM2_Op_XORI_LLC);
		break;
	case BSVM2_OP_SHLISLC:
		BSVM2_Interp_SetupOpI_SLC(cblk, op, BSVM2_Op_SHLI_LLC);
		break;
	case BSVM2_OP_SARISLC:
		BSVM2_Interp_SetupOpI_SLC(cblk, op, BSVM2_Op_SARI_LLC);
		break;

	case BSVM2_OP_ADDISLL:
		BSVM2_Interp_SetupOpI_SLL(cblk, op, BSVM2_Op_ADDI_LLL);
		break;
	case BSVM2_OP_SUBISLL:
		BSVM2_Interp_SetupOpI_SLL(cblk, op, BSVM2_Op_SUBI_LLL);
		break;
	case BSVM2_OP_MULISLL:
		BSVM2_Interp_SetupOpI_SLL(cblk, op, BSVM2_Op_MULI_LLL);
		break;
	case BSVM2_OP_ANDISLL:
		BSVM2_Interp_SetupOpI_SLL(cblk, op, BSVM2_Op_ANDI_LLL);
		break;
	case BSVM2_OP_ORISLL:
		BSVM2_Interp_SetupOpI_SLL(cblk, op, BSVM2_Op_ORI_LLL);
		break;
	case BSVM2_OP_XORISLL:
		BSVM2_Interp_SetupOpI_SLL(cblk, op, BSVM2_Op_XORI_LLL);
		break;
	case BSVM2_OP_SHLISLL:
		BSVM2_Interp_SetupOpI_SLL(cblk, op, BSVM2_Op_SHLI_LLL);
		break;
	case BSVM2_OP_SARISLL:
		BSVM2_Interp_SetupOpI_SLL(cblk, op, BSVM2_Op_SARI_LLL);
		break;

	case BSVM2_OP_ADDILSC:
		BSVM2_Interp_SetupOpUI_LSC(cblk, op, BSVM2_Op_ADDI_LLC);
		break;
	case BSVM2_OP_SUBILSC:
		BSVM2_Interp_SetupOpUI_LSC(cblk, op, BSVM2_Op_SUBI_LLC);
		break;
	case BSVM2_OP_MULILSC:
		BSVM2_Interp_SetupOpI_LSC(cblk, op, BSVM2_Op_MULI_LLC);
		break;
	case BSVM2_OP_ANDILSC:
		BSVM2_Interp_SetupOpI_LSC(cblk, op, BSVM2_Op_ANDI_LLC);
		break;
	case BSVM2_OP_ORILSC:
		BSVM2_Interp_SetupOpI_LSC(cblk, op, BSVM2_Op_ORI_LLC);
		break;
	case BSVM2_OP_XORILSC:
		BSVM2_Interp_SetupOpI_LSC(cblk, op, BSVM2_Op_XORI_LLC);
		break;
	case BSVM2_OP_SHLILSC:
		BSVM2_Interp_SetupOpI_LSC(cblk, op, BSVM2_Op_SHLI_LLC);
		break;
	case BSVM2_OP_SARILSC:
		BSVM2_Interp_SetupOpI_LSC(cblk, op, BSVM2_Op_SARI_LLC);
		break;

	case BSVM2_OP_ADDILSL:
		BSVM2_Interp_SetupOp_LSL(cblk, op, BSVM2_Op_ADDI_LLL);
		break;
	case BSVM2_OP_SUBILSL:
		BSVM2_Interp_SetupOp_LSL(cblk, op, BSVM2_Op_SUBI_LLL);
		break;
	case BSVM2_OP_MULILSL:
		BSVM2_Interp_SetupOp_LSL(cblk, op, BSVM2_Op_MULI_LLL);
		break;
	case BSVM2_OP_ANDILSL:
		BSVM2_Interp_SetupOp_LSL(cblk, op, BSVM2_Op_ANDI_LLL);
		break;
	case BSVM2_OP_ORILSL:
		BSVM2_Interp_SetupOp_LSL(cblk, op, BSVM2_Op_ORI_LLL);
		break;
	case BSVM2_OP_XORILSL:
		BSVM2_Interp_SetupOp_LSL(cblk, op, BSVM2_Op_XORI_LLL);
		break;
	case BSVM2_OP_SHLILSL:
		BSVM2_Interp_SetupOp_LSL(cblk, op, BSVM2_Op_SHLI_LLL);
		break;
	case BSVM2_OP_SARILSL:
		BSVM2_Interp_SetupOp_LSL(cblk, op, BSVM2_Op_SARI_LLL);
		break;
#endif

#if 1
	case BSVM2_OP_ADDILLC:
		BSVM2_Interp_SetupOpUI_LLC(cblk, op, BSVM2_Op_ADDI_LLC);
		break;
	case BSVM2_OP_SUBILLC:
		BSVM2_Interp_SetupOpUI_LLC(cblk, op, BSVM2_Op_SUBI_LLC);
		break;
	case BSVM2_OP_MULILLC:
		BSVM2_Interp_SetupOpI_LLC(cblk, op, BSVM2_Op_MULI_LLC);
		break;
	case BSVM2_OP_ANDILLC:
		BSVM2_Interp_SetupOpI_LLC(cblk, op, BSVM2_Op_ANDI_LLC);
		break;
	case BSVM2_OP_ORILLC:
		BSVM2_Interp_SetupOpI_LLC(cblk, op, BSVM2_Op_ORI_LLC);
		break;
	case BSVM2_OP_XORILLC:
		BSVM2_Interp_SetupOpI_LLC(cblk, op, BSVM2_Op_XORI_LLC);
		break;
	case BSVM2_OP_SHLILLC:
		BSVM2_Interp_SetupOpI_LLC(cblk, op, BSVM2_Op_SHLI_LLC);
		break;
	case BSVM2_OP_SARILLC:
		BSVM2_Interp_SetupOpI_LLC(cblk, op, BSVM2_Op_SARI_LLC);
		break;

	case BSVM2_OP_ADDILLL:
		BSVM2_Interp_SetupOpI_LLL(cblk, op, BSVM2_Op_ADDI_LLL);
		break;
	case BSVM2_OP_SUBILLL:
		BSVM2_Interp_SetupOpI_LLL(cblk, op, BSVM2_Op_SUBI_LLL);
		break;
	case BSVM2_OP_MULILLL:
		BSVM2_Interp_SetupOpI_LLL(cblk, op, BSVM2_Op_MULI_LLL);
		break;
	case BSVM2_OP_ANDILLL:
		BSVM2_Interp_SetupOpI_LLL(cblk, op, BSVM2_Op_ANDI_LLL);
		break;
	case BSVM2_OP_ORILLL:
		BSVM2_Interp_SetupOpI_LLL(cblk, op, BSVM2_Op_ORI_LLL);
		break;
	case BSVM2_OP_XORILLL:
		BSVM2_Interp_SetupOpI_LLL(cblk, op, BSVM2_Op_XORI_LLL);
		break;
	case BSVM2_OP_SHLILLL:
		BSVM2_Interp_SetupOpI_LLL(cblk, op, BSVM2_Op_SHLI_LLL);
		break;
	case BSVM2_OP_SARILLL:
		BSVM2_Interp_SetupOpI_LLL(cblk, op, BSVM2_Op_SARI_LLL);
		break;
#endif

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

	case BSVM2_OP_CVTAA2ST:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTAA2ST);
		break;
	case BSVM2_OP_CVTST2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTST2AA);
		break;
	case BSVM2_OP_CATST:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CATST);
		break;
	case BSVM2_OP_CMPST:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPST);
		break;
	case BSVM2_OP_CMPSST:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPSST);
		break;
	case BSVM2_OP_CMPUST:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPUST);
		break;

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
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARIC);
		break;
	case BSVM2_OP_LEARLC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARLC);
		break;
	case BSVM2_OP_LEARFC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARFC);
		break;
	case BSVM2_OP_LEARDC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARDC);
		break;
	case BSVM2_OP_LEARAC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARAC);
		break;
	case BSVM2_OP_LEARBC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARBC);
		break;
	case BSVM2_OP_LEARSC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARSC);
		break;
	case BSVM2_OP_LEASTC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LEARSC);
		break;

	case BSVM2_OP_IFXOBJ:
//		BSVM2_Interp_DecodeOpIxGx(cblk, op);
		BSVM2_Interp_DecodeOpGj(cblk, op);
		BSVM2_Interp_SetupOpUat(cblk, op,
			BSVM2_Op_IFXOBJ);
		break;
	case BSVM2_OP_DFXOBJ:
//		BSVM2_Interp_DecodeOpIxGx(cblk, op);
		BSVM2_Interp_DecodeOpGj(cblk, op);
		BSVM2_Interp_SetupOpUat(cblk, op,
			BSVM2_Op_DFXOBJ);
		break;

	case BSVM2_OP_IFXDYV:
		BSVM2_Interp_DecodeOpGj(cblk, op);
		BSVM2_Interp_SetupOpUat(cblk, op,
			BSVM2_Op_IFXDYV);
		break;
	case BSVM2_OP_DFXDYV:
		BSVM2_Interp_DecodeOpGj(cblk, op);
		BSVM2_Interp_SetupOpUat(cblk, op,
			BSVM2_Op_DFXDYV);
		break;

#if 1
	case BSVM2_OP_LDDINCZLP:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDINCZLP_I);
			break;
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDINCZLP_SB);
			break;
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDINCZLP_UB);
			break;
		case BSVM2_OPZ_SHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDINCZLP_SS);
			break;
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDINCZLP_US);
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDDINCZLP_L);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDDINCZLP_F);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDDINCZLP_D);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDDINCZLP_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_STDINCZLP:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDINCZLP_I);
			break;
		case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDINCZLP_B);
			break;
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDINCZLP_S);
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDINCZLP_L);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDINCZLP_F);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDINCZLP_D);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDINCZLP_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
#endif

#if 1
	case BSVM2_OP_LDDDECZLP:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDDECZLP_I);
			break;
		case BSVM2_OPZ_SBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDDECZLP_SB);
			break;
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDDECZLP_UB);
			break;
		case BSVM2_OPZ_SHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDDECZLP_SS);
			break;
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_INT, BSVM2_Op_LDDDECZLP_US);
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_LONG, BSVM2_Op_LDDDECZLP_L);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_LDDDECZLP_F);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDDDECZLP_D);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_LDDDECZLP_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
	case BSVM2_OP_STDDECZLP:
		BSVM2_Interp_DecodeOpZn(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:	
		case BSVM2_OPZ_UINT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDDECZLP_I);
			break;
		case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_UBYTE:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDDECZLP_B);
			break;
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDDECZLP_S);
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDDECZLP_L);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDDECZLP_F);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDDECZLP_D);
			break;
		case BSVM2_OPZ_ADDR:
			BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_STDDECZLP_A);
			break;
		default:
			BSVM2_DBGTRAP
			break;
		}
		break;
#endif

	case BSVM2_OP_MATHUFN:
		BSVM2_Interp_DecodeOpZn(cblk, op);

		switch(op->i1)
		{
		case BSVM2_OPZ_FLOAT:
			switch(op->i0)
			{
			case BSVM2_OPMU_SIN:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_SINF);
				break;
			case BSVM2_OPMU_COS:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_COSF);
				break;
			case BSVM2_OPMU_TAN:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_TANF);
				break;
			case BSVM2_OPMU_SQRT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_SQRTF);
				break;
			case BSVM2_OPMU_RCP:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_RCPF);
				break;
			case BSVM2_OPMU_ATAN:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_ATANF);
				break;
			case BSVM2_OPMU_SQR:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_SQRF);
				break;
			case BSVM2_OPMU_SSQRT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_SSQRTF);
				break;
			case BSVM2_OPMU_ABS:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_ABSF);
				break;
			default:
				break;
			}
			break;
		case BSVM2_OPZ_DOUBLE:
			switch(op->i0)
			{
			case BSVM2_OPMU_SIN:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_SIND);
				break;
			case BSVM2_OPMU_COS:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_COSD);
				break;
			case BSVM2_OPMU_TAN:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_TAND);
				break;
			case BSVM2_OPMU_SQRT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_SQRTD);
				break;
			case BSVM2_OPMU_RCP:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_RCPD);
				break;
			case BSVM2_OPMU_ATAN:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_ATAND);
				break;
			case BSVM2_OPMU_SQR:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_SQRD);
				break;
			case BSVM2_OPMU_SSQRT:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_SSQRTD);
				break;
			case BSVM2_OPMU_ABS:
				BSVM2_Interp_SetupOpUn(cblk, op,
					BSVM2_OPZ_FLOAT, BSVM2_Op_ABSD);
				break;
			default:
				break;
			}
			break;
		}

		if(!op->Run)
		{
			BSVM2_Interp_FreeOpcode(cblk, op);
			op=NULL;
		}
		break;

	default:
		if((opn>=0x400) && (opn<=0x4FF))
		{
			op=BSVM2_OpVec_DecodeVectorOpcode(cblk, op, opn);
			break;
		}

		BSVM2_Interp_FreeOpcode(cblk, op);
		op=NULL;
		break;
	}
	
	return(op);
}
