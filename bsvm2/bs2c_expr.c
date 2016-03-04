#include <bteifgl.h>

int BS2C_LookupLocal(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int i, j;
	
	for(i=0; i<ctx->frm->nlocals; i++)
	{
		vari=&(ctx->frm->locals[i]);
		if(!strcmp(vari->name, name))
			return(i);
	}
	return(-1);
}

int BS2C_CompileLoadName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=&(ctx->frm->locals[i]);
		bty=vari->bty;
		
		switch(bty)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_UBYTE:	case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDI);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDL);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		case BSVM2_OPZ_FLOAT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDF);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		case BSVM2_OPZ_DOUBLE:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDD);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		case BSVM2_OPZ_ADDRESS:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		}
		return(0);
	}
}

void BS2C_CompileConvType(BS2CC_CompileContext *ctx, int dty)
{
	int sty;
	int i;
	
	i=ctx->frm->stackpos-1;
	sty=ctx->frm->stack_bty[i];
	
	if(sty==dty)
		return;
	
	if(dty==BSVM2_OPZ_INT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_INT;
		if(sty==BSVM2_OPZ_LONG)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I); return; }
		if(sty==BSVM2_OPZ_FLOAT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2I); return; }
		if(sty==BSVM2_OPZ_DOUBLE)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2I); return; }
		
		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I); return; }
	}

	if(dty==BSVM2_OPZ_LONG)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_LONG;
		if(sty==BSVM2_OPZ_INT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2L); return; }
		if(sty==BSVM2_OPZ_FLOAT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2L); return; }
		if(sty==BSVM2_OPZ_DOUBLE)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2L); return; }
		
		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2L); return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ return; }
	}

	if(dty==BSVM2_OPZ_FLOAT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_FLOAT;
		if(sty==BSVM2_OPZ_INT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2F); return; }
		if(sty==BSVM2_OPZ_LONG)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2F); return; }
		if(sty==BSVM2_OPZ_DOUBLE)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2F); return; }

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2F); return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2F); return; }
	}

	if(dty==BSVM2_OPZ_DOUBLE)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_DOUBLE;
		if(sty==BSVM2_OPZ_INT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2D); return; }
		if(sty==BSVM2_OPZ_LONG)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2D); return; }
		if(sty==BSVM2_OPZ_FLOAT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2D); return; }

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2F); return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2F); return; }
	}

	if(dty==BSVM2_OPZ_SBYTE)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_SBYTE;

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSB2I); return; }

		if(BS2C_InferTypeSmallLongP(ctx, sty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSB2I);
			return;
		}
		if(sty==BSVM2_OPZ_FLOAT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSB2I);
			return;
		}
		if(sty==BSVM2_OPZ_DOUBLE)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSB2I);
			return;
		}
	}

	if(dty==BSVM2_OPZ_UBYTE)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_UBYTE;

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUB2I); return; }

		if(BS2C_InferTypeSmallLongP(ctx, sty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUB2I);
			return;
		}
		if(sty==BSVM2_OPZ_FLOAT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUB2I);
			return;
		}
		if(sty==BSVM2_OPZ_DOUBLE)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUB2I);
			return;
		}
	}

	if(dty==BSVM2_OPZ_SHORT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_SHORT;

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSS2I); return; }

		if(BS2C_InferTypeSmallLongP(ctx, sty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSS2I);
			return;
		}
		if(sty==BSVM2_OPZ_FLOAT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSS2I);
			return;
		}
		if(sty==BSVM2_OPZ_DOUBLE)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSS2I);
			return;
		}
	}

	if(dty==BSVM2_OPZ_USHORT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_USHORT;

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUS2I); return; }

		if(BS2C_InferTypeSmallLongP(ctx, sty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUS2I);
			return;
		}
		if(sty==BSVM2_OPZ_FLOAT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUS2I);
			return;
		}
		if(sty==BSVM2_OPZ_DOUBLE)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUS2I);
			return;
		}
	}

	BS2C_CaseError(ctx);
	return;
}

int BS2C_CompileLoadNameAsType(BS2CC_CompileContext *ctx,
	char *name, int ty)
{
	BS2C_CompileLoadName(ctx, name);
	BS2C_CompileConvType(ctx, ty);
}

void BS2C_CompileExprPushType(BS2CC_CompileContext *ctx, int dty)
{
	int i;
	
	i=ctx->frm->stackpos++;
	ctx->frm->stack_bty[i]=bty;
}

void BS2C_CompileError(BS2CC_CompileContext *ctx, int errn)
{
}

void BS2C_CaseError(BS2CC_CompileContext *ctx, int errn)
{
}

void BS2C_StubError(BS2CC_CompileContext *ctx, int errn)
{
}

void BS2C_CompileExpr(BS2CC_CompileContext *ctx,
	dtVal expr, int dty)
{
	dtVal ln, rn;
	int lt, rt;
	char *tag;
	char *op;
	int i;

	if(dtvIsFixIntP(expr) || dtvIsFixUIntP(expr) ||
		dtvIsFixLongP(expr) || dtvIsFixFloatP(expr) ||
		dtvIsFixDoubleP(expr))
	{
		if(dty==BSVM2_OPZ_INT)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT,
				dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_UINT)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UINT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UINT,
				dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_LONG)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG,
				dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_ULONG)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_ULONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_ULONG,
				dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_FLOAT)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeZxF(ctx,
				dtvUnwrapDouble(expr));
			return;
		}
		if(dty==BSVM2_OPZ_DOUBLE)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeZxD(ctx,
				dtvUnwrapDouble(expr));
			return;
		}

		if(dty==BSVM2_OPZ_UBYTE)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UBYTE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UBYTE,
				(byte)dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_SBYTE)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_SBYTE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_SBYTE,
				(sbyte)dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_USHORT)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_USHORT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_USHORT,
				(u16)dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_SHORT)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_SHORT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_SHORT,
				(s16)dtvUnwrapLong(expr));
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

#if 0
	if(dtvIsFixIntP(expr))
	{
		BS2C_CompileExprPushType(ctx,
			BSVM2_OPZ_INT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT,
			dtvUnwrapLong(expr));
		return;
	}
	if(dtvIsFixUIntP(expr))
	{
		BS2C_CompileExprPushType(ctx,
			BSVM2_OPZ_UINT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UINT,
			dtvUnwrapLong(expr));
		return;
	}
	if(dtvIsFixLongP(expr))
	{
		BS2C_CompileExprPushType(ctx,
			BSVM2_OPZ_LONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG,
			dtvUnwrapLong(expr));
		return;
	}
	if(dtvIsFixFloatP(expr))
	{
		BS2C_CompileExprPushType(ctx,
			BSVM2_OPZ_FLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxF(ctx,
			dtvUnwrapDouble(expr));
		return;
	}
	if(dtvIsFixDoubleP(expr))
	{
		BS2C_CompileExprPushType(ctx,
			BSVM2_OPZ_DOUBLE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxD(ctx,
			dtvUnwrapDouble(expr));
		return;
	}
#endif

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		i=BS2C_InferName(ctx, BGBDT_TagStr_GetUtf8(expr));
		return(i);
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		return(BSVM2_OPZ_VARIANT);
	}
	
	if(!strcmp(tag, "binary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		BS2C_CompileExpr(ctx, ln, dty);
		BS2C_CompileExpr(ctx, rn, dty);

		if(!strcmp(op, "+"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDI); return; }
			if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDL); return; }
			if(BS2C_InferTypeSmallFloatP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDF); return; }
			if(BS2C_InferTypeSmallDoubleP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDD); return; }
		}

		if(!strcmp(op, "-"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBI); return; }
			if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBL); return; }
			if(BS2C_InferTypeSmallFloatP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBF); return; }
			if(BS2C_InferTypeSmallDoubleP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBD); return; }
		}

		if(!strcmp(op, "*"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULI); return; }
			if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULL); return; }
			if(BS2C_InferTypeSmallFloatP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULF); return; }
			if(BS2C_InferTypeSmallDoubleP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULD); return; }
		}

		if(!strcmp(op, "/"))
		{
			BS2C_CompileExprPushType(ctx, dty);

			if(BS2C_InferTypeSmallIntP(ctx, dty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_BINOP);
				BS2C_EmitByte(ctx, (BSVM2_OPZ_INT<<4)|BSVM2_OPRI_DIV);
				return;
			}
			if(BS2C_InferTypeSmallLongP(ctx, dty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_BINOP);
				BS2C_EmitByte(ctx, (BSVM2_OPZ_LONG<<4)|BSVM2_OPRI_DIV);
				return;
			}

			if(BS2C_InferTypeSmallFloatP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DIVF); return; }
			if(BS2C_InferTypeSmallDoubleP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DIVD); return; }
		}

		if(!strcmp(op, "&"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_ANDI); return; }
			if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_ANDL); return; }
		}

		if(!strcmp(op, "|"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_ORI); return; }
			if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_ORL); return; }
		}

		if(!strcmp(op, "^"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_XORI); return; }
			if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_XORL); return; }
		}

		if(!strcmp(op, "<<"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHLI); return; }
			if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHLL); return; }
		}

		if(!strcmp(op, ">>"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SARI); return; }
			if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SARL); return; }
		}


		BS2C_CaseError(ctx);
		return;

//		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);
//		return(ty);
	}
}
