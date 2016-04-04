#include <bteifgl.h>

void BS2C_CompileConvType(BS2CC_CompileContext *ctx, int dty)
{
	int sty;
	int i;
	
	i=ctx->frm->stackpos-1;
	sty=ctx->frm->stack_bty[i];
	
	if(sty==dty)
		return;
	
	if(BS2C_TypeCompatibleP(ctx, dty, sty))
		return;

	if(dty==BS2CC_TYZ_VARIANT)
	{
		ctx->frm->stack_bty[i]=BS2CC_TYZ_VARIANT;

		if(sty==BSVM2_OPZ_INT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2AA); return; }
		if(sty==BSVM2_OPZ_LONG)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2AA); return; }
		if(sty==BSVM2_OPZ_FLOAT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2AA); return; }
		if(sty==BSVM2_OPZ_DOUBLE)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2AA); return; }

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2AA); return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2AA); return; }

		if(BS2C_InferTypeAddressP(ctx, sty))
			{ return; }

		return;
	}
	
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

		if(sty==BS2CC_TYZ_VARIANT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I); return; }
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

		if(sty==BS2CC_TYZ_VARIANT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2L); return; }
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

		if(sty==BS2CC_TYZ_VARIANT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2F); return; }
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

		if(sty==BS2CC_TYZ_VARIANT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2D); return; }
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

		if(sty==BS2CC_TYZ_VARIANT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
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

		if(sty==BS2CC_TYZ_VARIANT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
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

		if(sty==BS2CC_TYZ_VARIANT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
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

		if(sty==BS2CC_TYZ_VARIANT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUS2I);
			return;
		}
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_CompileLoadNameAsType(BS2CC_CompileContext *ctx,
	char *name, int ty)
{
	BS2C_CompileLoadName(ctx, name);
	BS2C_CompileConvType(ctx, ty);
}

void BS2C_CompileExprPushType(BS2CC_CompileContext *ctx, int dty)
{
	int i;
	
	i=ctx->frm->stackpos++;
	ctx->frm->stack_bty[i]=dty;
}

int BS2C_CompileExprPopType(BS2CC_CompileContext *ctx)
{
	int i;
	
	ctx->frm->stackpos--;
	i=ctx->frm->stackpos;
	return(ctx->frm->stack_bty[i]);
}

void BS2C_CompileExprPopType1(BS2CC_CompileContext *ctx)
	{ ctx->frm->stackpos--; }
void BS2C_CompileExprPopType2(BS2CC_CompileContext *ctx)
	{ ctx->frm->stackpos-=2; }
void BS2C_CompileExprPopType3(BS2CC_CompileContext *ctx)
	{ ctx->frm->stackpos-=3; }
void BS2C_CompileExprPopType4(BS2CC_CompileContext *ctx)
	{ ctx->frm->stackpos-=4; }

void BS2C_CompilePop(BS2CC_CompileContext *ctx)
{
	int ty;
	int i;
	
	i=ctx->frm->stackpos-1;
	ty=ctx->frm->stack_bty[i];
	
	if(BS2C_InferTypeSmallIntP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPI);
		ctx->frm->stackpos--;
		return;
	}
	if(BS2C_InferTypeSmallLongP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPL);
		ctx->frm->stackpos--;
		return;
	}

	if(BS2C_InferTypeSmallFloatP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPF);
		ctx->frm->stackpos--;
		return;
	}
	if(BS2C_InferTypeSmallDoubleP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPD);
		ctx->frm->stackpos--;
		return;
	}
	if(BS2C_InferTypeAddressP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPA);
		ctx->frm->stackpos--;
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileNoexPop(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
	BS2C_CompilePop(ctx);
	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPop2(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX2);
	BS2C_CompilePop(ctx);
	BS2C_CompilePop(ctx);
	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPop3(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX3);
	BS2C_CompilePop(ctx);
	BS2C_CompilePop(ctx);
	BS2C_CompilePop(ctx);
	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPopN(BS2CC_CompileContext *ctx, int n)
{
	int i;

	if(n==1)
		{ BS2C_CompileNoexPop(ctx); return; }
	if(n==2)
		{ BS2C_CompileNoexPop2(ctx); return; }
	if(n==3)
		{ BS2C_CompileNoexPop3(ctx); return; }
	
	i=n;
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX);
	while((i--)>0)
		BS2C_CompilePop(ctx);
//	BS2C_EmitOpcode(ctx, BSVM2_OP_LABEL);
//	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPushI(BS2CC_CompileContext *ctx)
{
	BS2C_CompileExprPushType(ctx, BS2CC_TYZ_INT);
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
	BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
//	BS2C_CompilePop(ctx);
	ctx->frm->newtrace=1;
}


void BS2C_CompileDup(BS2CC_CompileContext *ctx)
{
	int ty, z;
	int i;
	
	i=ctx->frm->stackpos-1;
	ty=ctx->frm->stack_bty[i];

	BS2C_CompileExprPushType(ctx, ty);

	if(BS2C_InferTypeSmallIntP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI); }
	else if(BS2C_InferTypeSmallLongP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPL); }
	else if(BS2C_InferTypeSmallFloatP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF); }
	else if(BS2C_InferTypeSmallDoubleP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD); }
	else if(BS2C_InferTypeAddressP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPA); }
	else
	{
		z=BS2C_GetTypeBaseZ(ctx, ty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_DUPN);
		BS2C_EmitOpcodeUZx(ctx, z, 1);
	}
}

void BS2C_CompileBinaryOpSS(BS2CC_CompileContext *ctx, char *op, int dty)
{
	if(!strcmp(op, "+"))
	{
		BS2C_CompileExprPopType2(ctx);
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
		BS2C_CompileExprPopType2(ctx);
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
		BS2C_CompileExprPopType2(ctx);
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
		BS2C_CompileExprPopType2(ctx);
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
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ANDI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ANDL); return; }
	}

	if(!strcmp(op, "|"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ORI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ORL); return; }
	}

	if(!strcmp(op, "^"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_XORI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_XORL); return; }
	}

	if(!strcmp(op, "<<"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHLI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHLL); return; }
	}

	if(!strcmp(op, ">>"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SARI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SARL); return; }
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileCall(BS2CC_CompileContext *ctx, dtVal expr, int dty)
{
	BS2CC_VarInfo *vari;
	BS2CC_VarInfo *vai;
	dtVal n0, n1;
	dtVal an;
	char *fn;
	int ix, bty, stp;
	int i, j, k, l, l2;

	an=BS2P_GetAstNodeAttr(expr, "args");
	fn=BS2P_GetAstNodeAttrS(expr, "func");
	if(fn)
	{
		if(!strcmp(fn, "super"))
		{
			BS2C_CompileCallSuper(ctx, expr, dty);
			return;
		}

		vari=NULL; ix=-1;

		if(ctx->frm->func && ctx->frm->func->obj)
		{
			vari=BS2C_LookupObjectFuncName(ctx,
				ctx->frm->func->obj, fn);
			if(vari && (vari->vitype==BS2CC_VITYPE_STRFUNC) &&
				(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
			{
				bty=vari->bty;
				ix=BS2C_IndexFrameGlobal(ctx, vari->gid);
			}

			if(vari && (vari->vitype==BS2CC_VITYPE_GBLFUNC))
			{
				bty=vari->bty;
				ix=BS2C_IndexFrameGlobal(ctx, vari->gid);
			}
		}

		if(ix<0)
		{
			ix=BS2C_LookupFrameGlobal(ctx, fn);
			if(ix<0)
			{
				BS2C_ErrNoDecl(ctx, fn);
				return;
			}

			vari=BS2C_GetFrameGlobalInfo(ctx, ix);
			bty=vari->bty;
		}

		stp=ctx->frm->stackpos;

		if(vari->nargs>=1)
		{
			i=vari->nargs-1;
			vai=vari->args[i];
		}else { vai=NULL; }

		if(vai && vai->bty==BS2CC_TYZ_VARARG)
		{
			l=dtvArrayGetSize(an);
			l2=vari->nargs-1;
			if(l<l2)
			{
				l=l2;
				BS2C_ErrTooFewArgs(ctx);
			}
			
			for(i=0; i<l2; i++)
			{
				vai=vari->args[i];
				n0=dtvArrayGetIndexDtVal(an, i);
				BS2C_CompileExpr(ctx, n0, vai->bty);
			}

			if(l>l2)
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
				BS2C_EmitOpcodeUZx(ctx, BS2CC_TYZ_ADDRESS, l-l2);

				for(i=l2; i<l; i++)
				{
					BS2C_EmitOpcode(ctx, BSVM2_OP_DUPA);
					n0=dtvArrayGetIndexDtVal(an, i);
					BS2C_CompileExpr(ctx, n0, BS2CC_TYZ_VARIANT);
					BS2C_EmitOpcode(ctx, BSVM2_OP_STIXAC);
					BS2C_EmitOpcodeUCx(ctx, i-l2);
				}
			}else
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
			}
		}else if(dtvIsArrayP(an))
		{
			l=dtvArrayGetSize(an);
			if(l<vari->nargs)
			{
				BS2C_ErrTooFewArgs(ctx);
			}else if(l>vari->nargs)
			{
				BS2C_ErrTooManyArgs(ctx);
				l=vari->nargs;
			}
			
			for(i=0; i<l; i++)
			{
				vai=vari->args[i];
				n0=dtvArrayGetIndexDtVal(an, i);
				BS2C_CompileExpr(ctx, n0, vai->bty);
			}
		}else if(!dtvNullP(an))
		{
			l=1;
			if(l<vari->nargs)
			{
				BS2C_ErrTooFewArgs(ctx);
			}else if(l>vari->nargs)
			{
				BS2C_ErrTooManyArgs(ctx);
				l=vari->nargs;
			}

			vai=vari->args[0];
			BS2C_CompileExpr(ctx, an, vai->bty);
		}else
		{
			if(vari->nargs)
				BS2C_ErrTooFewArgs(ctx);
		}

		ctx->frm->stackpos=stp;
		if(vari->vitype==BS2CC_VITYPE_STRFUNC)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLTH);
			BS2C_EmitOpcodeIdx(ctx, ix);
		}else
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLG);
			BS2C_EmitOpcodeIdx(ctx, ix);
		}

		if((vari->rty!=BSVM2_OPZ_VOID) && (dty!=BSVM2_OPZ_VOID))
		{
			BS2C_CompileExprPushType(ctx, vari->rty);
			BS2C_CompileConvType(ctx, dty);
		}else if(vari->rty!=BSVM2_OPZ_VOID)
		{
			BS2C_CompileExprPushType(ctx, vari->rty);
			BS2C_CompilePop(ctx);
		}else if(dty!=BSVM2_OPZ_VOID)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_ERRBADCONV);
		}

//		BS2C_EmitOpcode(ctx, BSVM2_OP_LDGS);
//		BS2C_EmitOpcodeIdx(ctx, i);
//		j=ctx->frm->stackpos++;
//		ctx->frm->stack_bty[j]=bty;

		return;
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_CompileObjMethodCall(BS2CC_CompileContext *ctx,
	int clsty, char *name,
	dtVal args, int dty)
{
	BS2C_CompileObjMethodCallB(ctx, clsty, name, args, dty, 0);
}

void BS2C_CompileObjMethodCallB(BS2CC_CompileContext *ctx,
	int clsty, char *name,
	dtVal args, int dty, int flag)
{
	int aty[256];
	BS2CC_VarInfo *vari, *clzi;
	BS2CC_VarInfo *vai;
	dtVal n0, n1;
	dtVal an;
	char *fn;
	int ix, bty, stp;
	int i, j, k, l, l2, na;

	if(dtvIsArrayP(args))
	{
		na=dtvArrayGetSize(args);
		for(i=0; i<na; i++)
		{
			n0=dtvArrayGetIndexDtVal(args, i);
			n0=BS2C_ReduceExpr(ctx, n0);
			aty[i]=BS2C_InferExpr(ctx, n0);
		}
	}else if(dtvTrueP(args))
	{
		n0=BS2C_ReduceExpr(ctx, args);
		aty[0]=BS2C_InferExpr(ctx, n0);
		na=1;
	}else
	{
		na=0;
	}
	
	an=args;
	fn=name;
	if(fn)
	{
		stp=ctx->frm->stackpos;

		clzi=BS2C_GetTypeObject(ctx, clsty);

		vari=BS2C_LookupObjectFuncNameArgs(ctx, clzi, fn, aty, na);
		if(!vari)
			vari=BS2C_LookupObjectFuncName(ctx, clzi, fn);

		if(!vari)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_ERRNODECL);
			return;
		}
				
		ix=BS2C_IndexFrameGlobal(ctx, vari->gid);

		bty=vari->bty;

		if(vari->nargs>=1)
		{
			i=vari->nargs-1;
			vai=vari->args[i];
		}else { vai=NULL; }

		if(vai && vai->bty==BS2CC_TYZ_VARARG)
		{
			l=dtvArrayGetSize(an);
			l2=vari->nargs-1;
			if(l<l2)
			{
				l=l2;
				BS2C_ErrTooFewArgs(ctx);
			}
			
			for(i=0; i<l2; i++)
			{
				vai=vari->args[i];
				n0=dtvArrayGetIndexDtVal(an, i);
				BS2C_CompileExpr(ctx, n0, vai->bty);
			}

			if(l>l2)
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
				BS2C_EmitOpcodeUZx(ctx, BS2CC_TYZ_ADDRESS, l-l2);

				for(i=l2; i<l; i++)
				{
					BS2C_EmitOpcode(ctx, BSVM2_OP_DUPA);
					n0=dtvArrayGetIndexDtVal(an, i);
					BS2C_CompileExpr(ctx, n0, BS2CC_TYZ_VARIANT);
					BS2C_EmitOpcode(ctx, BSVM2_OP_STIXAC);
					BS2C_EmitOpcodeUCx(ctx, i-l2);
				}
			}else
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
			}
		}else if(dtvIsArrayP(an))
		{
			l=dtvArrayGetSize(an);
			if(l<vari->nargs)
			{
				BS2C_ErrTooFewArgs(ctx);
			}else if(l>vari->nargs)
			{
				BS2C_ErrTooManyArgs(ctx);
				l=vari->nargs;
			}
			
			for(i=0; i<l; i++)
			{
				vai=vari->args[i];
				n0=dtvArrayGetIndexDtVal(an, i);
				BS2C_CompileExpr(ctx, n0, vai->bty);
			}
		}else if(dtvTrueP(an))
		{
			l=1;
			if(l<vari->nargs)
			{
				BS2C_ErrTooFewArgs(ctx);
			}else if(l>vari->nargs)
			{
				BS2C_ErrTooManyArgs(ctx);
				l=vari->nargs;
			}

			vai=vari->args[0];
			BS2C_CompileExpr(ctx, an, vai->bty);
		}else
		{
			if(vari->nargs)
				BS2C_ErrTooFewArgs(ctx);
		}

		ctx->frm->stackpos=stp;
		
		if(flag&1)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLTH);
			BS2C_EmitOpcodeIdx(ctx, ix);
		}else
		{
			BS2C_CompileExprPopType1(ctx);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLV);
			BS2C_EmitOpcodeIdx(ctx, ix);
		}

		if((vari->rty!=BS2CC_TYZ_VOID) && (dty!=BS2CC_TYZ_VOID))
		{
			BS2C_CompileExprPushType(ctx, vari->rty);
			BS2C_CompileConvType(ctx, dty);
		}else if(vari->rty!=BS2CC_TYZ_VOID)
		{
			BS2C_CompileExprPushType(ctx, vari->rty);
			BS2C_CompilePop(ctx);
		}else if(dty!=BS2CC_TYZ_VOID)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_ERRBADCONV);
		}

//		BS2C_EmitOpcode(ctx, BSVM2_OP_LDGS);
//		BS2C_EmitOpcodeIdx(ctx, i);
//		j=ctx->frm->stackpos++;
//		ctx->frm->stack_bty[j]=bty;

		return;
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_CompileCallSuper(BS2CC_CompileContext *ctx, dtVal expr, int dty)
{
	BS2CC_VarInfo *vari;
	BS2CC_VarInfo *vai;
	dtVal n0, n1;
	dtVal an;
	char *fn;
	int ix, bty, stp;
	int i, j, k, l, l2;

	an=BS2P_GetAstNodeAttr(expr, "args");
//	fn=BS2P_GetAstNodeAttrS(expr, "func");

	if(!ctx->frm->func->obj || !ctx->frm->func->obj->super)
	{
		BS2C_CaseError(ctx);
		return;
	}

//	BS2C_CompileExprPushType(ctx, dty);
//	BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
//	BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_VOID, 4);

	ix=ctx->frm->func->obj->gid;
	bty=ix+256;

	BS2C_CompileObjMethodCallB(ctx, bty, "$CTOR$", an, BS2CC_TYZ_VOID, 1);
}

void BS2C_CompileExprNew(BS2CC_CompileContext *ctx, dtVal expr, int dty)
{
	dtVal tyn, arn;
	s64 li, lj, lk;
	BS2CC_VarInfo *vi, *vi2;
	char *tag;
	char *op;
	int t0, t1, t2, t3;
	int cty, bty;
	int z, o;
	int i;

	tyn=BS2P_GetAstNodeAttr(expr, "type");
	arn=BS2P_GetAstNodeAttr(expr, "args");
	
	cty=BS2C_InferTypeExtType(ctx, tyn);
	
	if(BS2C_TypeSizedArrayP(ctx, cty))
	{
		bty=BS2C_TypeDerefType(ctx, cty);

		if(BS2C_TypeSizedArrayP(ctx, bty))
		{
			BS2C_CaseError(ctx);
			return;
		}

		BS2C_CompileExprPushType(ctx, cty);

		z=BS2C_GetTypeBaseZ(ctx, bty);
		o=BS2C_TypeGetArraySize(ctx, cty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
		BS2C_EmitOpcodeUZx(ctx, z, o);
		return;
	}

	if(1)
	{
		vi=BS2C_GetTypeObject(ctx, cty);
		i=BS2C_IndexFrameGlobal(ctx, vi->gid);

//				BS2C_CompileExpr(ctx, ln, lt);

//				BS2C_CompileExprPopType2(ctx);
//				BS2C_CompileExprPushType(ctx, vi2->bty);

		BS2C_CompileExprPushType(ctx, cty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_NEWOBJ);
		BS2C_EmitOpcodeUCx(ctx, i);
		if(dtvTrueP(arn))
		{
			BS2C_CompileDup(ctx);
			BS2C_CompileObjMethodCall(
				ctx, cty, "$CTOR$", arn, BS2CC_TYZ_VOID);
		}
		return;
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_CompileStoreExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn, cn;
	s64 li, lj, lk;
	BS2CC_VarInfo *vi, *vi2;
	char *tag;
	char *op, *fn;
	int t0, t1, t2, t3;
	int lt, rt, cty, bty;
	int z, o;
	int i;

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		BS2C_CompileStoreName(ctx,
			BGBDT_TagStr_GetUtf8(expr));
		return;
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "loadindex"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
//		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		bty=BS2C_TypeDerefType(ctx, lt);

		z=BS2C_GetTypeBaseZ(ctx, bty);
		
		if(dtvIsFixIntP(rn) || dtvIsFixUIntP(rn))
		{
			li=dtvUnwrapLong(expr);

			o=-1;
			if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT))
				o=BSVM2_OP_STIXIC;
			if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG))
				o=BSVM2_OP_STIXLC;
			if(z==BSVM2_OPZ_FLOAT)
				o=BSVM2_OP_STIXFC;
			if(z==BSVM2_OPZ_DOUBLE)
				o=BSVM2_OP_STIXDC;
			if(z==BSVM2_OPZ_ADDRESS)
				o=BSVM2_OP_STIXAC;
			if((z==BSVM2_OPZ_SBYTE) || (z==BSVM2_OPZ_UBYTE))
				o=BSVM2_OP_STIXBC;
			if((z==BSVM2_OPZ_SHORT) || (z==BSVM2_OPZ_USHORT))
				o=BSVM2_OP_STIXSC;

			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeSCx(ctx, li);
				BS2C_CompileExprPopType2(ctx);
				return;
			}
		}
		
		o=-1;
		if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT))
			o=BSVM2_OP_STIXI;
		if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG))
			o=BSVM2_OP_STIXL;
		if(z==BSVM2_OPZ_FLOAT)
			o=BSVM2_OP_STIXF;
		if(z==BSVM2_OPZ_DOUBLE)
			o=BSVM2_OP_STIXD;
		if(z==BSVM2_OPZ_ADDRESS)
			o=BSVM2_OP_STIXA;
		if((z==BSVM2_OPZ_SBYTE) || (z==BSVM2_OPZ_UBYTE))
			o=BSVM2_OP_STIXB;
		if((z==BSVM2_OPZ_SHORT) || (z==BSVM2_OPZ_USHORT))
			o=BSVM2_OP_STIXS;

		if(o>=0)
		{
			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileExpr(ctx, rn, rt);
			BS2C_EmitOpcode(ctx, o);
			BS2C_CompileExprPopType3(ctx);
			return;
		}

//		BS2C_EmitOpcode(ctx, o);
//		BS2C_EmitOpcodeSCx(ctx, li);
	}

	if(!strcmp(tag, "loadslot"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		lt=BS2C_InferExpr(ctx, ln);
		vi=BS2C_GetTypeObject(ctx, lt);

		if(vi && BGBDT_TagStr_IsSymbolP(rn))
		{
			if((vi->vitype==BS2CC_VITYPE_STRUCT) ||
				(vi->vitype==BS2CC_VITYPE_CLASS) ||
				(vi->vitype==BS2CC_VITYPE_IFACE))
			{
				fn=BGBDT_TagStr_GetUtf8(rn);
				vi2=BS2C_LookupObjectFieldName(ctx, vi, fn);
				
				if(!vi2)
				{
					BS2C_CompileError(ctx, BS2CC_ERRN_ERRNODECL);
					return;
				}
				
				i=BS2C_IndexFrameGlobal(ctx, vi2->gid);

				if(vi2->vitype==BS2CC_VITYPE_STRVAR)
				{
					BS2C_CompileExpr(ctx, ln, lt);

					BS2C_CompileExprPopType2(ctx);
//					BS2C_CompileExprPushType(ctx, vi2->bty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_STOS);
					BS2C_EmitOpcodeUCx(ctx, i);
					return;
				}

				if(vi2->vitype==BS2CC_VITYPE_GBLVAR)
				{
//					BS2C_CompileExpr(ctx, ln, lt);

					BS2C_CompileExprPopType1(ctx);
//					BS2C_CompileExprPushType(ctx, vi2->bty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_STGS);
					BS2C_EmitOpcodeUCx(ctx, i);
					return;
				}
			}
		}

		BS2C_CaseError(ctx);
		return;
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_CompileExprBinary(BS2CC_CompileContext *ctx,
	char *op, dtVal ln, dtVal rn, int dty)
{
	int o, u;
	s64 li;

	ln=BS2C_ReduceExpr(ctx, ln);
	rn=BS2C_ReduceExpr(ctx, rn);

	if(BS2C_InferTypeSmallIntP(ctx, dty))
	{
		if(dtvIsFixIntP(rn))
		{
			li=dtvUnwrapLong(rn);

			if(!strcmp(op, "+"))
			{
				if(li==1)
				{
					BS2C_CompileExpr(ctx, ln, dty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_INCI);
					return;
				}

				if(li==(-1))
				{
					BS2C_CompileExpr(ctx, ln, dty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_DECI);
					return;
				}
			}

			if(!strcmp(op, "-"))
			{
				if(li==1)
				{
					BS2C_CompileExpr(ctx, ln, dty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_DECI);
					return;
				}

				if(li==(-1))
				{
					BS2C_CompileExpr(ctx, ln, dty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_INCI);
					return;
				}
			}

			o=-1; u=0;
			if(!strcmp(op, "+")) { o=BSVM2_OP_ADDIC; u=1; }
			if(!strcmp(op, "-")) { o=BSVM2_OP_SUBIC; u=1; }
			if(!strcmp(op, "&"))o=BSVM2_OP_ANDIC;
			if(!strcmp(op, "|"))o=BSVM2_OP_ORIC;
			if(!strcmp(op, "^"))o=BSVM2_OP_XORIC;
			if(!strcmp(op, "<<"))o=BSVM2_OP_SHLIC;

			if(BS2C_InferTypeSignedP(ctx, dty))
			{
				if(!strcmp(op, "*"))o=BSVM2_OP_MULIC;
				if(!strcmp(op, ">>"))o=BSVM2_OP_SARIC; u=1;
			}
			
			if(o>0)
			{
				BS2C_CompileExpr(ctx, ln, dty);
				BS2C_EmitOpcode(ctx, o);
				if(u)
					{ BS2C_EmitOpcodeUCx(ctx, li); }
				else
					{ BS2C_EmitOpcodeSCx(ctx, li); }
				return;
			}
		}

		if(dtvIsFixIntP(ln))
		{
			li=dtvUnwrapLong(ln);

			if(!strcmp(op, "+"))
			{
				if(li==1)
				{
					BS2C_CompileExpr(ctx, rn, dty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_INCI);
					return;
				}

				if(li==(-1))
				{
					BS2C_CompileExpr(ctx, rn, dty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_DECI);
					return;
				}
			}

			o=-1; u=0;
			if(!strcmp(op, "+")) { o=BSVM2_OP_ADDIC; u=1; }
			if(!strcmp(op, "&"))o=BSVM2_OP_ANDIC;
			if(!strcmp(op, "|"))o=BSVM2_OP_ORIC;
			if(!strcmp(op, "^"))o=BSVM2_OP_XORIC;
			if(BS2C_InferTypeSignedP(ctx, dty))
				{ if(!strcmp(op, "*"))o=BSVM2_OP_MULIC; }
			
			if(o>0)
			{
				BS2C_CompileExpr(ctx, rn, dty);
				BS2C_EmitOpcode(ctx, o);
				if(u)
					{ BS2C_EmitOpcodeUCx(ctx, li); }
				else
					{ BS2C_EmitOpcodeSCx(ctx, li); }
				return;
			}
		}
	}

	BS2C_CompileExpr(ctx, ln, dty);
	BS2C_CompileExpr(ctx, rn, dty);

	BS2C_CompileBinaryOpSS(ctx, op, dty);
	return;
}

void BS2C_CompileExprPushConstInt(
	BS2CC_CompileContext *ctx, s64 val, int dty)
{
	s64 li, lj;
	
	li=val;
	
	if(dty==BSVM2_OPZ_INT)
	{
		if(li>0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UINT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UINT, li);
			return;
		}
		
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, li);
		return;
	}
	if(dty==BSVM2_OPZ_UINT)
	{
		lj=(int)li;
		if(lj<0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, lj);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UINT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UINT, li);
		return;
	}

	if(dty==BSVM2_OPZ_LONG)
	{
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG, li);
		return;
	}
	if(dty==BSVM2_OPZ_ULONG)
	{
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_ULONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_ULONG, li);
		return;
	}

	if(dty==BSVM2_OPZ_UBYTE)
	{
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UBYTE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UBYTE, (byte)li);
		return;
	}
	if(dty==BSVM2_OPZ_SBYTE)
	{
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_SBYTE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_SBYTE, (sbyte)li);
		return;
	}
	if(dty==BSVM2_OPZ_USHORT)
	{
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_USHORT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_USHORT, (u16)li);
		return;
	}
	if(dty==BSVM2_OPZ_SHORT)
	{
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_SHORT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_SHORT, (s16)li);
		return;
	}

	if(dty==BSVM2_OPZ_FLOAT)
	{
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_FLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxF(ctx, li);
		return;
	}
	if(dty==BSVM2_OPZ_DOUBLE)
	{
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxD(ctx, li);
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileExpr(BS2CC_CompileContext *ctx,
	dtVal expr, int dty)
{
	dtVal ln, rn, cn;
	s64 li, lj, lk;
	BS2CC_VarInfo *vi, *vi2;
	char *tag;
	char *op, *fn;
	int t0, t1, t2, t3;
	int lt, rt, cty, bty;
	int z, o, u;
	int i, j, k, l;

	if(dtvIsFixIntP(expr) || dtvIsFixUIntP(expr) || dtvIsFixLongP(expr))
	{
		li=dtvUnwrapLong(expr);
		BS2C_CompileExprPushConstInt(ctx, li, dty);
		return;
	}

	if(dtvIsFixFloatP(expr) ||
		dtvIsFixDoubleP(expr))
	{
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

		li=dtvUnwrapLong(expr);
		BS2C_CompileExprPushConstInt(ctx, li, dty);
		return;

//		BS2C_CaseError(ctx);
//		return;
	}

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		fn=BGBDT_TagStr_GetUtf8(expr);

		if(!strcmp(fn, "null"))
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
			return;
		}

		if(!strcmp(fn, "undefined"))
		{
			BS2C_CompileExprPushType(ctx, dty);
//			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);

			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_VOID, 1);
			return;
		}
		
		if(!strcmp(fn, "true"))
		{
			if(BS2C_InferTypeSmallDoubleP(ctx, dty))
			{
				BS2C_CompileExprPushConstInt(ctx, 1, dty);
				return;
			}

			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_VOID, 2);
			return;
		}
		
		if(!strcmp(fn, "false"))
		{
			if(BS2C_InferTypeSmallDoubleP(ctx, dty))
			{
				BS2C_CompileExprPushConstInt(ctx, 0, dty);
				return;
			}

			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_VOID, 3);
			return;
		}

		if(!strcmp(fn, "this"))
		{
			if(ctx->frm->func && ctx->frm->func->obj &&
				(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
			{
				BS2C_CompileExprPushType(ctx, dty);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
				BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_VOID, 4);
				return;
			}
		}
		
		BS2C_CompileLoadNameAsType(ctx, fn, dty);
		return;
	}

	if(BGBDT_TagStr_IsStringP(expr))
	{
		i=BS2C_ImgGetString(ctx, BGBDT_TagStr_GetUtf8(expr));
//		BS2C_CompileLoadNameAsType(ctx,
//			BGBDT_TagStr_GetUtf8(expr), dty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_LDCA);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UBYTE, i);

		return;
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		BS2C_CaseError(ctx);
		return;
	}
	
	if(!strcmp(tag, "binary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		BS2C_CompileExprBinary(ctx, op, ln, rn, dty);
		return;
	}

	if(!strcmp(tag, "bincmp"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		cty=BS2C_InferSuperType(ctx, lt, rt);

		BS2C_CompileExpr(ctx, ln, cty);
		BS2C_CompileExpr(ctx, rn, cty);

		z=BS2C_GetTypeBaseZ(ctx, dty);
		if(!strcmp(op, "=="))
			o=BSVM2_OPRC_EQ;
		if(!strcmp(op, "!="))
			o=BSVM2_OPRC_NE;
		if(!strcmp(op, "<"))
			o=BSVM2_OPRC_LT;
		if(!strcmp(op, ">"))
			o=BSVM2_OPRC_GT;
		if(!strcmp(op, "<="))
			o=BSVM2_OPRC_LE;
		if(!strcmp(op, ">="))
			o=BSVM2_OPRC_GE;
		if(!strcmp(op, "==="))
			o=BSVM2_OPRC_EQQ;
		if(!strcmp(op, "!=="))
			o=BSVM2_OPRC_NEQ;

		BS2C_CompileExprPopType2(ctx);

		BS2C_EmitOpcode(ctx, BSVM2_OP_CMPOP);
		BS2C_EmitByte(ctx, (z<<4)|o);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
		{
			BS2C_CompileExprPushType(ctx, dty);
			return;
		}

		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
		BS2C_CompileConvType(ctx, dty);
		return;
	}

	if(!strcmp(tag, "unary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		lt=BS2C_InferExpr(ctx, ln);

		if(!strcmp(op, "-"))
		{
			o=-1;
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ o=BSVM2_OP_NEGI; }
			else if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ o=BSVM2_OP_NEGL; }
			else if(BS2C_InferTypeSmallFloatP(ctx, dty))
				{ o=BSVM2_OP_NEGF; }
			else if(BS2C_InferTypeSmallDoubleP(ctx, dty))
				{ o=BSVM2_OP_NEGD; }
			
			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}

		if(!strcmp(op, "~"))
		{
			o=-1;
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ o=BSVM2_OP_NOTI; }
			else if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ o=BSVM2_OP_NOTL; }
			
			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}

		if(!strcmp(op, "!"))
		{
			o=-1;
			if(BS2C_InferTypeSmallIntP(ctx, dty))
				{ o=BSVM2_OP_LNTI; }
			else if(BS2C_InferTypeSmallLongP(ctx, dty))
				{ o=BSVM2_OP_LNTL; }
			
			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}

		BS2C_CaseError(ctx);
//		BS2C_CompileExprBinary(ctx, op, ln, rn, dty);
		return;
	}

	if(!strcmp(tag, "assign"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
//		op=BS2P_GetAstNodeAttrS(expr, "op");

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
//		cty=BS2C_InferSuperType(ctx, lt, rt);
		cty=BS2C_TypeAssignSuperType(ctx, lt, rt);

//		BS2C_CompileExpr(ctx, ln, cty);
//		BS2C_CompileExpr(ctx, rn, cty);

		if(BS2C_InferTypeVoidP(ctx, dty) && (lt==rt) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			BGBDT_TagStr_IsSymbolP(rn))
		{
			i=BS2C_LookupLocal(ctx,
				BGBDT_TagStr_GetUtf8(ln));
			j=BS2C_LookupLocal(ctx,
				BGBDT_TagStr_GetUtf8(rn));

			if((i>=0) && (j>=0))
			{
				o=-1;
				if(BS2C_InferTypeSmallIntP(ctx, lt))
					{ o=BSVM2_OP_MVI; }
				else if(BS2C_InferTypeSmallLongP(ctx, lt))
					{ o=BSVM2_OP_MVL; }
				else if(BS2C_TypeFloatP(ctx, lt))
					{ o=BSVM2_OP_MVF; }
				else if(BS2C_TypeDoubleP(ctx, lt))
					{ o=BSVM2_OP_MVD; }
				else if(BS2C_InferTypeAddressP(ctx, lt))
					{ o=BSVM2_OP_MVA; }

				if(o>=0)
				{
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeJx(ctx, i, j);
					return;
				}
			}
		}

		if(BS2C_InferTypeVoidP(ctx, dty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			dtvIsFixIntP(rn))
		{
			i=BS2C_LookupLocal(ctx,
				BGBDT_TagStr_GetUtf8(ln));
			li=dtvUnwrapLong(rn);

			if(i>=0)
			{
				o=-1;
				if(BS2C_InferTypeSmallIntP(ctx, lt))
					{ o=BSVM2_OP_MVIC; }
				else if(BS2C_InferTypeSmallLongP(ctx, lt))
					{ o=BSVM2_OP_MVLC; }
				else if(BS2C_TypeFloatP(ctx, lt))
					{ o=BSVM2_OP_MVFC; }
				else if(BS2C_TypeDoubleP(ctx, lt))
					{ o=BSVM2_OP_MVDC; }
//				else if(BS2C_InferTypeAddressP(ctx, lt))
//					{ o=BSVM2_OP_MVA; }

				if(o>=0)
				{
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeSuKx(ctx, lt, i, li);
					return;
				}
			}
		}

		BS2C_CompileExpr(ctx, rn, cty);
		BS2C_CompileConvType(ctx, lt);
		
		if(!BS2C_InferTypeVoidP(ctx, dty))
			{ BS2C_CompileDup(ctx); }

		BS2C_CompileStoreExpr(ctx, ln);
	
		if(!BS2C_InferTypeVoidP(ctx, dty))
		{
			BS2C_CompileConvType(ctx, dty);
		}
		return;
	}

	if(!strcmp(tag, "assignop"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		cty=BS2C_InferSuperType(ctx, lt, rt);

		BS2C_CompileExprBinary(ctx, op, ln, rn, cty);

//		BS2C_CompileExpr(ctx, ln, cty);
//		BS2C_CompileExpr(ctx, rn, cty);
//		BS2C_CompileBinaryOpSS(ctx, op, cty);

//		BS2C_CompileExpr(ctx, rn, cty);
//		BS2C_CompileConvType(ctx, lt);
		
		if(!BS2C_InferTypeVoidP(ctx, dty))
		{
			if(BS2C_InferTypeSmallIntP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI); }
			else if(BS2C_InferTypeSmallLongP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPL); }
			else if(BS2C_InferTypeSmallFloatP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF); }
			else if(BS2C_InferTypeSmallDoubleP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD); }
			else if(BS2C_InferTypeAddressP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPA); }
			else
			{
				z=lt&15;
				BS2C_EmitOpcode(ctx, BSVM2_OP_DUPN);
				BS2C_EmitByte(ctx, (z<<4)|1);
			}
		}
		
		BS2C_CompileStoreExpr(ctx, ln);
		
		if(!BS2C_InferTypeVoidP(ctx, dty))
			{ BS2C_CompileConvType(ctx, dty); }
		return;
	}

	if(!strcmp(tag, "tern"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		cn=BS2P_GetAstNodeAttr(expr, "cond");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);
		cn=BS2C_ReduceExpr(ctx, cn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		cty=BS2C_InferSuperType(ctx, lt, rt);
		
//		t0=BS2C_GenTempLabel(ctx);
		t1=BS2C_GenTempLabel(ctx);
		t2=BS2C_GenTempLabel(ctx);

		BS2C_CompileTempJmpElse(ctx, cn, t1);

//		BS2C_EmitTempLabel(ctx, t0);
		BS2C_CompileExpr(ctx, ln, cty);
		BS2C_EmitTempJump(ctx, t2);
		BS2C_CompileNoexPop(ctx);

		BS2C_EmitTempLabelB(ctx, t1);
		BS2C_CompileExpr(ctx, rn, cty);

		BS2C_EmitTempLabelB(ctx, t2);
		return;
	}
	
	if(!strcmp(tag, "call"))
	{
		BS2C_CompileCall(ctx, expr, dty);
		return;
	}

	if(!strcmp(tag, "new"))
	{
		BS2C_CompileExprNew(ctx, expr, dty);
		return;
	}

	if(!strcmp(tag, "loadindex"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		bty=BS2C_TypeDerefType(ctx, lt);

		z=BS2C_GetTypeBaseZ(ctx, bty);
		
		if(dtvIsFixIntP(rn) || dtvIsFixUIntP(rn))
		{
			li=dtvUnwrapLong(expr);

			o=-1;
			if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT))
				o=BSVM2_OP_LDIXIC;
			if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG))
				o=BSVM2_OP_LDIXLC;
			if(z==BSVM2_OPZ_FLOAT)
				o=BSVM2_OP_LDIXFC;
			if(z==BSVM2_OPZ_DOUBLE)
				o=BSVM2_OP_LDIXDC;
			if(z==BSVM2_OPZ_ADDRESS)
				o=BSVM2_OP_LDIXAC;
			if(z==BSVM2_OPZ_SBYTE)
				o=BSVM2_OP_LDIXSBC;
			if(z==BSVM2_OPZ_UBYTE)
				o=BSVM2_OP_LDIXUBC;
			if(z==BSVM2_OPZ_SHORT)
				o=BSVM2_OP_LDIXSSC;
			if(z==BSVM2_OPZ_USHORT)
				o=BSVM2_OP_LDIXUSC;

			if(o>=0)
			{
				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, bty);

				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeSCx(ctx, li);
				return;
			}
		}
		
		o=-1;
		if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT))
			o=BSVM2_OP_LDIXI;
		if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG))
			o=BSVM2_OP_LDIXL;
		if(z==BSVM2_OPZ_FLOAT)
			o=BSVM2_OP_LDIXF;
		if(z==BSVM2_OPZ_DOUBLE)
			o=BSVM2_OP_LDIXD;
		if(z==BSVM2_OPZ_ADDRESS)
			o=BSVM2_OP_LDIXA;
		if(z==BSVM2_OPZ_SBYTE)
			o=BSVM2_OP_LDIXSB;
		if(z==BSVM2_OPZ_UBYTE)
			o=BSVM2_OP_LDIXUB;
		if(z==BSVM2_OPZ_SHORT)
			o=BSVM2_OP_LDIXSS;
		if(z==BSVM2_OPZ_USHORT)
			o=BSVM2_OP_LDIXUS;

		if(o>=0)
		{
			BS2C_CompileExprPopType2(ctx);
			BS2C_CompileExprPushType(ctx, bty);

			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileExpr(ctx, rn, rt);
			BS2C_EmitOpcode(ctx, o);
			return;
		}
	}

	if(!strcmp(tag, "postinc") || !strcmp(tag, "preinc"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		ln=BS2C_ReduceExpr(ctx, ln);

		lt=BS2C_InferExpr(ctx, ln);
		z=BS2C_GetTypeBaseZ(ctx, lt);

		if(BS2C_InferTypeVoidP(ctx, dty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			BS2C_InferTypeSmallIntP(ctx, lt))
		{
			i=BS2C_LookupLocal(ctx,
				BGBDT_TagStr_GetUtf8(ln));
			if(i>=0)
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_INCIL);
				BS2C_EmitOpcodeUCx(ctx, i);
				return;
			}
		}

		u=0;
		if(!strcmp(tag, "preinc"))
			u=1;

		BS2C_CompileExpr(ctx, ln, lt);
		if(!BS2C_InferTypeVoidP(ctx, dty) && (u==0))
			{ BS2C_CompileDup(ctx); }

		if(BS2C_InferTypeSmallIntP(ctx, lt))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_INCI); }
		else if(BS2C_InferTypeSmallDoubleP(ctx, lt))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPC);
			BS2C_EmitByte(ctx, (z<<4)|BSVM2_OPRI_ADD);
			BS2C_EmitOpcodeSuCx(ctx, z, 1);
		}

		if(!BS2C_InferTypeVoidP(ctx, dty) && (u==1))
			{ BS2C_CompileDup(ctx); }

		BS2C_CompileStoreExpr(ctx, ln);
		if(!BS2C_InferTypeVoidP(ctx, dty))
			{ BS2C_CompileConvType(ctx, dty); }
		return;
	}

	if(!strcmp(tag, "postdec") || !strcmp(tag, "predec"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		ln=BS2C_ReduceExpr(ctx, ln);

		lt=BS2C_InferExpr(ctx, ln);
		z=BS2C_GetTypeBaseZ(ctx, lt);

		if(BS2C_InferTypeVoidP(ctx, dty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			BS2C_InferTypeSmallIntP(ctx, lt))
		{
			i=BS2C_LookupLocal(ctx,
				BGBDT_TagStr_GetUtf8(ln));
			if(i>=0)
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_DECIL);
				BS2C_EmitOpcodeUCx(ctx, i);
				return;
			}
		}

		u=0;
		if(!strcmp(tag, "predec"))
			u=1;

		BS2C_CompileExpr(ctx, ln, lt);
		if(!BS2C_InferTypeVoidP(ctx, dty) && (u==0))
			{ BS2C_CompileDup(ctx); }

		if(BS2C_InferTypeSmallIntP(ctx, lt))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_DECI); }
		else if(BS2C_InferTypeSmallDoubleP(ctx, lt))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPC);
			BS2C_EmitByte(ctx, (z<<4)|BSVM2_OPRI_SUB);
			BS2C_EmitOpcodeSuCx(ctx, z, 1);
		}

		if(!BS2C_InferTypeVoidP(ctx, dty) && (u==1))
			{ BS2C_CompileDup(ctx); }

		BS2C_CompileStoreExpr(ctx, ln);
		if(!BS2C_InferTypeVoidP(ctx, dty))
			{ BS2C_CompileConvType(ctx, dty); }
		return;
	}

	if(!strcmp(tag, "loadslot"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		lt=BS2C_InferExpr(ctx, ln);
		vi=BS2C_GetTypeObject(ctx, lt);

		if(vi && BGBDT_TagStr_IsSymbolP(rn))
		{
			if((vi->vitype==BS2CC_VITYPE_STRUCT) ||
				(vi->vitype==BS2CC_VITYPE_CLASS) ||
				(vi->vitype==BS2CC_VITYPE_IFACE))
			{
				fn=BGBDT_TagStr_GetUtf8(rn);
				vi2=BS2C_LookupObjectFieldName(ctx, vi, fn);
				
				if(!vi2)
				{
					BS2C_CompileError(ctx, BS2CC_ERRN_ERRNODECL);
					return;
				}
				
				i=BS2C_IndexFrameGlobal(ctx, vi2->gid);

				if(vi2->vitype==BS2CC_VITYPE_STRVAR)
				{
					BS2C_CompileExpr(ctx, ln, lt);

					BS2C_CompileExprPopType1(ctx);
					BS2C_CompileExprPushType(ctx, vi2->bty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_LDOS);
					BS2C_EmitOpcodeUCx(ctx, i);
					return;
				}

				if(vi2->vitype==BS2CC_VITYPE_GBLVAR)
				{
					BS2C_CompileExprPushType(ctx, vi2->bty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_LDGS);
					BS2C_EmitOpcodeUCx(ctx, i);
					return;
				}
			}
		}

		BS2C_CaseError(ctx);
		return;
	}

	BS2C_CaseError(ctx);
	return;
}
