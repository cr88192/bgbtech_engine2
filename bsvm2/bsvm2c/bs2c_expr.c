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

void BS2C_CompileLoadNameAsType(BS2CC_CompileContext *ctx,
	char *name, int ty)
{
	BS2C_CompileLoadName(ctx, name);
	BS2C_CompileConvType(ctx, ty);
	BS2C_CompileConvType(ctx, ty);
}

void BS2C_CompileExprResvPush(
	BS2CC_CompileContext *ctx, int n)
{
	int i;

	i=ctx->frm->stackpos+n;
	if(i>ctx->frm->stacksize)
		ctx->frm->stacksize=i;
}

void BS2C_CompileExprPushType(BS2CC_CompileContext *ctx, int dty)
{
	int i;

	i=ctx->frm->stackpos++;
	if(i>=0)
		ctx->frm->stack_bty[i]=dty;
	if(ctx->frm->stackpos>ctx->frm->stacksize)
		ctx->frm->stacksize=ctx->frm->stackpos;
}

int BS2C_CompileExprPopType(BS2CC_CompileContext *ctx)
{
	int i;
	
	ctx->frm->stackpos--;
	i=ctx->frm->stackpos;
	
	if(i<0)
	{
		ctx->frm->stackpos=0;
		BS2C_ErrStackUnderflow(ctx);
		return(-1);
	}
	
	return(ctx->frm->stack_bty[i]);
}

void BS2C_CompileExprPopType1(BS2CC_CompileContext *ctx)
{
	ctx->frm->stackpos--;
	if(ctx->frm->stackpos<0)
	{
		ctx->frm->stackpos=0;
		BS2C_ErrStackUnderflow(ctx);
	}
}

void BS2C_CompileExprPopType2(BS2CC_CompileContext *ctx)
{
	ctx->frm->stackpos-=2;
	if(ctx->frm->stackpos<0)
	{
		ctx->frm->stackpos=0;
		BS2C_ErrStackUnderflow(ctx);
	}
}

void BS2C_CompileExprPopType3(BS2CC_CompileContext *ctx)
{
	ctx->frm->stackpos-=3;
	if(ctx->frm->stackpos<0)
	{
		ctx->frm->stackpos=0;
		BS2C_ErrStackUnderflow(ctx);
	}
}

void BS2C_CompileExprPopType4(BS2CC_CompileContext *ctx)
{
	ctx->frm->stackpos-=4;
	if(ctx->frm->stackpos<0)
	{
		ctx->frm->stackpos=0;
		BS2C_ErrStackUnderflow(ctx);
	}
}

int BS2C_CompileExprPeekType(BS2CC_CompileContext *ctx)
{
	int ty;
	int i;
	
	i=ctx->frm->stackpos-1;
	ty=ctx->frm->stack_bty[i];
	return(ty);
}

void BS2C_CompilePop(BS2CC_CompileContext *ctx)
{
	int ty;
	int i;
	
	i=ctx->frm->stackpos-1;
	ty=ctx->frm->stack_bty[i];

	if(BS2C_TypeVariantP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_DROPAA);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	BS2C_CompileBasePop(ctx);
}

void BS2C_CompileBasePop(BS2CC_CompileContext *ctx)
{
	int ty;
	int i;
	
	i=ctx->frm->stackpos-1;
	ty=ctx->frm->stack_bty[i];
	
	if(BS2C_TypeSmallIntP(ctx, ty) ||
		BS2C_TypeHalfFloatP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPI);
		BS2C_CompileExprPopType1(ctx);
		return;
	}
	if(BS2C_TypeSmallLongP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPL);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeSmallFloatP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPF);
		BS2C_CompileExprPopType1(ctx);
		return;
	}
	if(BS2C_TypeSmallDoubleP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPD);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeAddressP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPA);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeX64P(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPA);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeX128P(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPX);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileNoexPop(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
	BS2C_CompileBasePop(ctx);
	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPop2(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX2);
	BS2C_CompileBasePop(ctx);
	BS2C_CompileBasePop(ctx);
	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPop3(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX3);
	BS2C_CompileBasePop(ctx);
	BS2C_CompileBasePop(ctx);
	BS2C_CompileBasePop(ctx);
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
		BS2C_CompileBasePop(ctx);
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

void BS2C_CompileNoexPush(BS2CC_CompileContext *ctx, int dty)
{
	if(BS2C_TypeVoidP(ctx, dty))
	{
		return;
	}

	if(BS2C_TypeSmallIntP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_INT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
		ctx->frm->newtrace=1;
		return;
	}

	if(BS2C_TypeSmallLongP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_LONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHL);
		ctx->frm->newtrace=1;
		return;
	}

	if(BS2C_TypeHalfFloatP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
		ctx->frm->newtrace=1;
		return;
	}

	if(BS2C_TypeSmallFloatP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
		ctx->frm->newtrace=1;
		return;
	}

	if(BS2C_TypeSmallDoubleP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_DOUBLE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
		ctx->frm->newtrace=1;
		return;
	}

	if(BS2C_TypeAddressP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
		BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		ctx->frm->newtrace=1;
		return;
	}

	if(BS2C_TypeX64P(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, dty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		ctx->frm->newtrace=1;
		return;
	}

	if(BS2C_TypeX128P(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, dty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		ctx->frm->newtrace=1;
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompilePushDummy(BS2CC_CompileContext *ctx, int dty)
{
	if(BS2C_TypeVoidP(ctx, dty))
	{
		return;
	}

	if(BS2C_TypeSmallIntP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_INT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
		return;
	}

	if(BS2C_TypeSmallLongP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_LONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHL);
		return;
	}

	if(BS2C_TypeHalfFloatP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
		return;
	}

	if(BS2C_TypeSmallFloatP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
		return;
	}

	if(BS2C_TypeSmallDoubleP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_DOUBLE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
		return;
	}

	if(BS2C_TypeAddressP(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		return;
	}

	if(BS2C_TypeX64P(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, dty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		return;
	}

	if(BS2C_TypeX128P(ctx, dty))
	{
		BS2C_CompileExprPushType(ctx, dty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
		BS2C_EmitOpcode(ctx, BSVM2_OP_MKX2D);
		return;
	}

	BS2C_CaseError(ctx);
}


void BS2C_CompileDup(BS2CC_CompileContext *ctx)
{
	int ty, z;
	int i;
	
	i=ctx->frm->stackpos-1;
	ty=ctx->frm->stack_bty[i];

	BS2C_CompileExprPushType(ctx, ty);

	if(BS2C_TypeSmallIntP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI); }
	else if(BS2C_TypeSmallLongP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPL); }
	else if(BS2C_TypeHalfFloatP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI); }
	else if(BS2C_TypeSmallFloatP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF); }
	else if(BS2C_TypeSmallDoubleP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD); }
	else if(BS2C_TypeVariantP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPAA); }
	else if(BS2C_TypeAddressP(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPA); }
	else if(BS2C_TypeX64P(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPA); }
	else if(BS2C_TypeX128P(ctx, ty))
		{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPX); }
	else
	{
//		z=BS2C_GetTypeBaseZ(ctx, ty);
//		BS2C_EmitOpcode(ctx, BSVM2_OP_DUPN);
//		BS2C_EmitOpcodeUZx(ctx, z, 1);
		BS2C_CaseError(ctx);
	}
}

void BS2C_CompileBinaryOpSS(BS2CC_CompileContext *ctx, char *op, int dty)
{
	int z, o;

	if(!strcmp(op, "+"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDL); return; }
		if(BS2C_TypeSmallFloatP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDF); return; }
		if(BS2C_TypeSmallDoubleP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDD); return; }
			
		if(BS2C_TypeOpXvP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRV_ADD);
			return;
		}

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_ADD);
			return;
		}
	}

	if(!strcmp(op, "-"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBL); return; }
		if(BS2C_TypeSmallFloatP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBF); return; }
		if(BS2C_TypeSmallDoubleP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBD); return; }

		if(BS2C_TypeOpXvP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRV_SUB);
			return;
		}

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_SUB);
			return;
		}
	}

	if(!strcmp(op, "*"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULL); return; }
		if(BS2C_TypeSmallFloatP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULF); return; }
		if(BS2C_TypeSmallDoubleP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULD); return; }

		if(BS2C_TypeOpXvP(ctx, dty))
		{
			if(BS2C_TypeOpXvCplxP(ctx, dty))
			{
				z=BS2C_GetTypeVecZ(ctx, dty);
				BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
				BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRV_CMUL);
				return;
			}
		
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRV_MUL);
			return;
		}

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_MUL);
			return;
		}
	}

	if(!strcmp(op, "/"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_TypeUnsignedP(ctx, dty))
		{
			if(BS2C_TypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_UDIVI); return; }
			if(BS2C_TypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_UDIVL); return; }
		}

		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_DIVI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_DIVL); return; }
		if(BS2C_TypeSmallFloatP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_DIVF); return; }
		if(BS2C_TypeSmallDoubleP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_DIVD); return; }

		if(BS2C_TypeOpXvP(ctx, dty))
		{
			if(BS2C_TypeOpXvCplxP(ctx, dty))
			{
				z=BS2C_GetTypeVecZ(ctx, dty);
				BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
				BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRV_CDIV);
				return;
			}
		
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRV_DIV);
			return;
		}

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			o=BSVM2_OPRI_DIV;
			if(BS2C_TypeUnsignedP(ctx, dty))
				o=BSVM2_OPRI_UDIV;
			if(BS2C_TypeFloat128P(ctx, dty))
				o=BSVM2_OPRF_DIV;

			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, o);
			return;
		}
	}

	if(!strcmp(op, "%"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MODI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MODL); return; }

		if(BS2C_TypeOpXvP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRV_CROSS);
			return;
		}

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_MOD);
			return;
		}
	}

	if(!strcmp(op, "&"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ANDI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ANDL); return; }


		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_AND);
			return;
		}
	}

	if(!strcmp(op, "|"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ORI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ORL); return; }

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_OR);
			return;
		}
	}

	if(!strcmp(op, "^"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_XORI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_XORL); return; }

		if(BS2C_TypeOpXvP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRV_DOT);
			return;
		}

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_XOR);
			return;
		}
	}

	if(!strcmp(op, "<<"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHLI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHLL); return; }

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_SHL);
			return;
		}
	}

	if(!strcmp(op, ">>"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_TypeUnsignedP(ctx, dty))
		{
			if(BS2C_TypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHRI); return; }
			if(BS2C_TypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHRL); return; }
		}else
		{
			if(BS2C_TypeSmallIntP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SARI); return; }
			if(BS2C_TypeSmallLongP(ctx, dty))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_SARL); return; }
		}

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			o=BSVM2_OPRI_SAR;
			if(BS2C_TypeUnsignedP(ctx, dty))
				o=BSVM2_OPRI_SHR;

			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, o);
			return;
		}
	}

	if(!strcmp(op, ">>>"))
	{
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_TypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHRI); return; }
		if(BS2C_TypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHRL); return; }

		if(BS2C_TypeOpXNumP(ctx, dty))
		{
			z=BS2C_GetTypeVecZ(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOPX);
			BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRI_SHR);
			return;
		}
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileCallArg(BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vai, dtVal expr)
{
	char *fn;

	if(BS2C_TypeVarRefP(ctx, vai->bty))
	{
		if(BGBDT_TagStr_IsSymbolP(expr))
		{
			fn=BGBDT_TagStr_GetUtf8(expr);
			BS2C_CompileLoadRefName(ctx, fn);
			return;
		}
		BS2C_CaseError(ctx);
		return;
	}

	BS2C_CompileExpr(ctx, expr, vai->bty);
}

void BS2C_CompileCallVariArgs(BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari, dtVal an)
{
	dtVal n0, n1;
	BS2CC_VarInfo *vai;
	int l2, lt, rt;
	int i, j, k, l;

	if(vari->nargs>=1)
	{
		i=vari->nargs-1;
		vai=vari->args[i];
	}else { vai=NULL; }

	if(vai && (vai->bty==BS2CC_TYZ_VARARG) && dtvIsArrayP(an))
	{
		l=dtvArrayGetSize(an);
		l2=vari->nargs-1;
		if(l<l2)
		{
			BS2C_ErrTooFewArgs(ctx);
			l=l2;
		}
		
		for(i=0; i<l2; i++)
		{
			vai=vari->args[i];
			n0=dtvArrayGetIndexDtVal(an, i);
			n0=BS2C_ReduceExpr(ctx, n0);
			BS2C_CompileCallArg(ctx, vai, n0);
		}

		n1=DTV_NULL;
		if(l==(l2+1))
		{
			vai=vari->args[l2];
			n0=dtvArrayGetIndexDtVal(an, l2);
			n0=BS2C_ReduceExpr(ctx, n0);
			rt=BS2C_InferExpr(ctx, n0);
			if((rt==BS2CC_TYZ_VARIANT_ARR) ||
				(rt==BS2CC_TYZ_VARARG))
					n1=n0;
		}

		if(!dtvNullP(n1))
		{
			BS2C_CompileExpr(ctx, n1,
				BS2CC_TYZ_VARIANT_ARR);
		}else if(l>l2)
		{
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
			BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
			BS2C_EmitOpcodeUZx(ctx, BS2CC_TYZ_ADDRESS, l-l2);

			for(i=l2; i<l; i++)
			{
				n0=dtvArrayGetIndexDtVal(an, i);
				n0=BS2C_ReduceExpr(ctx, n0);
				BS2C_CompileExpr(ctx, n0, BS2CC_TYZ_VARIANT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXAC);
				BS2C_EmitOpcodeSCx(ctx, i-l2);
				BS2C_CompileExprPopType1(ctx);
			}
			BS2C_CompileExprPopType1(ctx);
		}else
		{
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
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
			n0=BS2C_ReduceExpr(ctx, n0);
			BS2C_CompileCallArg(ctx, vai, n0);
		}
	}else if(vai && (vai->bty==BS2CC_TYZ_VARARG))
	{
		l=1;
		l2=vari->nargs-1;
		if(l<l2)
		{
			BS2C_ErrTooFewArgs(ctx);
		}else if(l>l2)
		{
			BS2C_ErrTooManyArgs(ctx);
			l=vari->nargs;
		}

		vai=vari->args[0];
		an=BS2C_ReduceExpr(ctx, an);
		BS2C_CompileCallArg(ctx, vai, an);

		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
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
		an=BS2C_ReduceExpr(ctx, an);
		BS2C_CompileCallArg(ctx, vai, an);
	}else
	{
		if(vari->nargs)
			BS2C_ErrTooFewArgs(ctx);
	}
}

void BS2C_CompileExprCall(BS2CC_CompileContext *ctx,
	dtVal fnn, dtVal an, int dty)
{
	BS2CC_VarInfo *vari;
	dtVal n0, n1;
	int nty, ix, stp, bty;
	int i, j, k, l;

	nty=BS2C_InferExpr(ctx, fnn);

	if(BS2C_TypeVariantP(ctx, nty))
	{
		if(dtvIsArrayP(an))
		{
			l=dtvArrayGetSize(an);

			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
			BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
			BS2C_EmitOpcodeUZx(ctx, BS2CC_TYZ_ADDRESS, l);

			for(i=0; i<l; i++)
			{
				n0=dtvArrayGetIndexDtVal(an, i);
				n0=BS2C_ReduceExpr(ctx, n0);
				BS2C_CompileExpr(ctx, n0, BS2CC_TYZ_VARIANT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXAC);
				BS2C_EmitOpcodeSCx(ctx, i);
				BS2C_CompileExprPopType1(ctx);
			}
		}else if(dtvTrueP(an))
		{			
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
			BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
			BS2C_EmitOpcodeUZx(ctx, BS2CC_TYZ_ADDRESS, 1);

			an=BS2C_ReduceExpr(ctx, an);
			BS2C_CompileExpr(ctx, an, BS2CC_TYZ_VARIANT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXAC);
			BS2C_EmitOpcodeSCx(ctx, 0);
			BS2C_CompileExprPopType1(ctx);
		}else
		{
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		}

		BS2C_CompileExpr(ctx, fnn, BS2CC_TYZ_VARIANT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_CALLAA);
		BS2C_EmitSetNewtrace(ctx);
		BS2C_CompileExprPopType2(ctx);

		if(dty!=BS2CC_TYZ_VOID)
		{
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_CompileConvType(ctx, dty);
		}else
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_DROPAA);
		}
		return;
	}
	
	if(BS2C_TypeFunctionP(ctx, nty))
	{
		vari=BS2C_GetTypeObject(ctx, nty);
		bty=vari->bty;
		
		ix=BS2C_IndexFrameGlobal(ctx, vari->gid);

		stp=ctx->frm->stackpos;

		BS2C_CompileCallVariArgs(ctx, vari, an);

		ctx->frm->stackpos=stp;

		i=BS2C_InferExprLocalIndex(ctx, fnn);

		if(i>=0)
		{
//			BS2C_CompileExpr(ctx, fnn, nty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLL);
			BS2C_EmitOpcodeJx(ctx, i, ix);
			BS2C_EmitSetNewtrace(ctx);
		}else
		{
//			BS2C_CompileExpr(ctx, fnn, BS2CC_TYZ_VARIANT);
			BS2C_CompileExpr(ctx, fnn, nty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLA);
			BS2C_EmitOpcodeIdx(ctx, ix);
			BS2C_CompileExprPopType1(ctx);
			BS2C_EmitSetNewtrace(ctx);
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
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileCall(BS2CC_CompileContext *ctx, dtVal expr, int dty)
{
	BS2CC_VarInfo *vari;
	BS2CC_VarInfo *vai;
	dtVal ln, rn;
	dtVal n0, n1;
	dtVal an, fnn;
	char *fn, *tag;
	int ix, lt, rt, bty, stp;
	int i, j, k, l, l2;

	an=BS2P_GetAstNodeAttr(expr, "args");
	fnn=BS2P_GetAstNodeAttr(expr, "func");
//	fn=BS2P_GetAstNodeAttrS(expr, "func");
//	if(fn)
	if(BGBDT_TagStr_IsSymbolP(fnn))
	{
		fn=BGBDT_TagStr_GetUtf8(fnn);

		if(!strcmp(fn, "super"))
		{
			BS2C_CompileCallSuper(ctx, expr, dty);
			return;
		}

		vari=NULL; ix=-1;

		if(ctx->frm->func && ctx->frm->func->obj)
		{
			vari=BS2C_LookupObjectFuncNameB(ctx,
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
				BS2C_CompileExprCall(ctx, fnn, an, dty);
//				BS2C_ErrNoDecl(ctx, fn);
				return;
			}

			vari=BS2C_GetFrameGlobalInfo(ctx, ix);
			bty=vari->bty;
		}

		stp=ctx->frm->stackpos;

		BS2C_CompileCallVariArgs(ctx, vari, an);

		ctx->frm->stackpos=stp;
		if(vari->vitype==BS2CC_VITYPE_STRFUNC)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLTH);
			BS2C_EmitOpcodeIdx(ctx, ix);
			BS2C_EmitSetNewtrace(ctx);
		}else
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLG);
			BS2C_EmitOpcodeIdx(ctx, ix);
			BS2C_EmitSetNewtrace(ctx);
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
		return;
	}

	tag=BS2P_GetAstNodeTag(fnn);
	
	if(!tag)
	{
		BS2C_CaseError(ctx);
		return;
	}
	
	if(!strcmp(tag, "loadslot"))
	{
		ln=BS2P_GetAstNodeAttr(fnn, "lhs");
		rn=BS2P_GetAstNodeAttr(fnn, "rhs");

		lt=BS2C_InferExpr(ctx, ln);
//		vi=BS2C_GetTypeObject(ctx, lt);

		if(BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);
			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileObjMethodCall(ctx, lt, fn, an, dty);
			return;
		}

		BS2C_CaseError(ctx);
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
	int ix, rt, bty, stp;
	int i, j, k, l, l2, na;

	if(dtvEqqP(args, DTV_EMPTYLIST))
		args=DTV_NULL;

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

		BS2C_CompileCallVariArgs(ctx, vari, an);

		ctx->frm->stackpos=stp;
		
		if(flag&1)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLTH);
			BS2C_EmitOpcodeIdx(ctx, ix);
			BS2C_EmitSetNewtrace(ctx);
		}else
		{
			BS2C_CompileExprPopType1(ctx);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CALLV);
			BS2C_EmitOpcodeIdx(ctx, ix);
			BS2C_EmitSetNewtrace(ctx);
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
	dtVal tyn, arn, init, arrs;
	dtVal n0, n1, n2;
	dtVal ln, rn;
	s64 li, lj, lk;
	BS2CC_VarInfo *vi, *vi2;
	char *tag, *fn;
	char *op;
	double f, g;
	int t0, t1, t2, t3;
	int cty, bty;
	int z, o, l2;
	int i, j, k, l;

	tyn=BS2P_GetAstNodeAttr(expr, "type");
	arn=BS2P_GetAstNodeAttr(expr, "args");
	init=BS2P_GetAstNodeAttr(expr, "init");
	
	cty=BS2C_TypeExtTypeNew(ctx, tyn);
	
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
		l2=BS2C_TypeGetArraySize(ctx, cty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
		BS2C_EmitOpcodeUZx(ctx, z, l2);

		if(dtvTrueP(init))
		{
			switch(z)
			{
			case BS2CC_TYZ_INT:			o=BSVM2_OP_DSTIXIC; break;
			case BS2CC_TYZ_UINT:		o=BSVM2_OP_DSTIXIC; break;
			case BS2CC_TYZ_LONG:		o=BSVM2_OP_DSTIXLC; break;
			case BS2CC_TYZ_ULONG:		o=BSVM2_OP_DSTIXLC; break;
			case BS2CC_TYZ_FLOAT:		o=BSVM2_OP_DSTIXFC; break;
			case BS2CC_TYZ_DOUBLE:		o=BSVM2_OP_DSTIXDC; break;
			case BS2CC_TYZ_ADDRESS:		o=BSVM2_OP_DSTIXAC; break;
			case BS2CC_TYZ_SBYTE:		o=BSVM2_OP_DSTIXBC; break;
			case BS2CC_TYZ_UBYTE:		o=BSVM2_OP_DSTIXBC; break;
			case BS2CC_TYZ_SHORT:		o=BSVM2_OP_DSTIXSC; break;
			case BS2CC_TYZ_USHORT:		o=BSVM2_OP_DSTIXSC; break;
			default:					o=BSVM2_OP_DSTIXAC; break;
			}

			ln=BS2P_GetAstNodeAttr(init, "value");

			if(dtvTrueP(ln))
			{
				l=dtvArrayGetSize(ln);
				if(l>l2)
				{
					BS2C_CaseError(ctx);
					return;
				}
				
				for(i=0; i<l; i++)
				{
					n0=dtvArrayGetIndexDtVal(ln, i);
					
//					BS2C_CompileExpr(ctx, n0, bty);

					if(dtvIsSmallLongP(n0) &&
						BS2C_TypeSmallLongP(ctx, bty))
					{
						li=dtvUnwrapLong(n0);
						BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXNC);
						BS2C_EmitOpcodeUZx(ctx, z, i);
						BS2C_EmitOpcodeSuCx(ctx, z, li);
						continue;
					}

					if(dtvIsSmallDoubleP(n0) &&
						BS2C_TypeSmallDoubleP(ctx, bty))
					{
						f=dtvUnwrapDouble(n0);
						BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXNC);
						BS2C_EmitOpcodeUZx(ctx, z, i);
						BS2C_EmitOpcodeSuCxD(ctx, z, f);
						continue;
					}

					BS2C_CompileExpr(ctx, n0, bty);
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeSCx(ctx, i);
					BS2C_CompileExprPopType1(ctx);
				}
			}
		}

		return;
	}

	if(BS2C_TypeArrayP(ctx, cty))
	{
		arrs=BS2P_GetAstNodeAttr(tyn, "arrays");
		bty=BS2C_TypeDerefType(ctx, cty);

		if(BS2C_TypeArrayP(ctx, bty))
		{
			BS2C_CaseError(ctx);
			return;
		}

		if(dtvIsArrayP(arrs))
		{
			BS2C_CaseError(ctx);

#if 0
			l=dtvArrayGetSize(expr);
			for(i=0; i<l; i++)
			{
				n0=dtvArrayGetIndexDtVal(expr, i);

				tag=BS2P_GetAstNodeTag(n0);
			
				if(tag && !strcmp(tag, "arrdef"))
					{ asza[i]=BS2P_GetAstNodeAttrI(n0, "value"); }
				else
					{ asza[i]=0; }
				if(ctx->ncfatal)
					break;
			}

			asz=0;
			al=l;
#endif
		}else if(dtvTrueP(arrs))
		{
			tag=BS2P_GetAstNodeTag(arrs);
			
			if(tag && !strcmp(tag, "arrdef"))
			{
				n1=BS2P_GetAstNodeAttr(arrs, "value");
				if(dtvIsSmallIntP(n1))
				{
					BS2C_CompileExprPushType(ctx, cty);
					z=BS2C_GetTypeBaseZ(ctx, bty);
					o=dtvUnwrapInt(n1);
					BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
					BS2C_EmitOpcodeUZx(ctx, z, o);

//					al=0; asz=dtvUnwrapInt(n1);
					return;
				}else
				{
					BS2C_CompileExpr(ctx, n1, BS2CC_TYZ_INT);
					BS2C_CompileExprPopType(ctx);

					z=BS2C_GetTypeBaseZ(ctx, bty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
					BS2C_EmitOpcodeUZx(ctx, z, 0);
					BS2C_CompileExprPushType(ctx, cty);
					return;
				}
			}else
			{
//				al=1; asz=0;
				BS2C_CaseError(ctx);
			}
		}
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

#if 1
//		if(!strcmp(tag, "object"))
		if(dtvTrueP(init))
		{
			ln=BS2P_GetAstNodeAttr(init, "value");

//			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
//			BS2C_EmitOpcode(ctx, BSVM2_OP_NEWDYO);

			if(dtvTrueP(ln))
			{
				l=dtvArrayGetSize(ln);
				for(i=0; i<l; i++)
				{
					n0=dtvArrayGetIndexDtVal(ln, i);
					n1=BS2P_GetAstNodeAttr(n0, "value");
					fn=BS2P_GetAstNodeAttrS(n0, "name");

					vi2=BS2C_LookupObjectFieldName(ctx, vi, fn);
					
					if(!vi2)
					{
						BS2C_CompileError(ctx, BS2CC_ERRN_ERRNODECL);
						return;
					}
					
					j=BS2C_IndexFrameGlobal(ctx, vi2->gid);

					BS2C_CompileExpr(ctx, n1, vi2->bty);

//					BS2C_EmitOpcode(ctx, BSVM2_OP_DSBOS);
					BS2C_EmitOpcode(ctx, BSVM2_OP_DSTOS);
					BS2C_EmitOpcodeUCx(ctx, j);

					BS2C_CompileExprPopType1(ctx);
				}
			}
		}
#endif

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
		
		if(BS2C_TypeVariantP(ctx, lt))
		{
			if(BS2C_TypeSmallIntP(ctx, rt))
			{
				BS2C_CompileConvType(ctx, BS2CC_TYZ_VARIANT);
				BS2C_CompileExpr(ctx, ln, BS2CC_TYZ_VARIANT);
				BS2C_CompileExpr(ctx, rn, BS2CC_TYZ_INT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_STIXAI);
				BS2C_CompileExprPopType3(ctx);
				return;
			}
			
			BS2C_CompileConvType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_CompileExpr(ctx, ln, BS2CC_TYZ_VARIANT);
			BS2C_CompileExpr(ctx, rn, BS2CC_TYZ_VARIANT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_STIXAA);
			BS2C_CompileExprPopType3(ctx);
			return;
		}
		
		if(dtvIsFixIntP(rn) || dtvIsFixUIntP(rn))
		{
//			li=dtvUnwrapLong(expr);
			li=dtvUnwrapLong(rn);

#if 1
			if(li==0)
			{
				o=-1;
				if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT))
					o=BSVM2_OP_STDRI;
				if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG))
					o=BSVM2_OP_STDRL;
				if(z==BSVM2_OPZ_FLOAT)
					o=BSVM2_OP_STDRF;
				if(z==BSVM2_OPZ_DOUBLE)
					o=BSVM2_OP_STDRD;
				if(z==BSVM2_OPZ_ADDRESS)
					o=BSVM2_OP_STDRA;
				if((z==BSVM2_OPZ_SBYTE) || (z==BSVM2_OPZ_UBYTE))
					o=BSVM2_OP_STDRB;
				if((z==BSVM2_OPZ_SHORT) || (z==BSVM2_OPZ_USHORT))
					o=BSVM2_OP_STDRS;

				if(o>=0)
				{
					BS2C_CompileExpr(ctx, ln, lt);

					if(BS2C_TypePointerP(ctx, lt))
						BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

					BS2C_EmitOpcode(ctx, o);
					BS2C_CompileExprPopType2(ctx);
					return;
				}
			}
#endif

#if 1
			t0=BS2C_InferExprLocalIndex(ctx, ln);
//			if(t0>=0)
			if((t0>=0) && !BS2C_TypePointerP(ctx, lt))
			{
				if(BS2C_TypePointerP(ctx, lt))
					BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

				BS2C_EmitOpcode(ctx, BSVM2_OP_STIXZLC);
				BS2C_EmitOpcodeUZx(ctx, z, t0);
				BS2C_EmitOpcodeSCx(ctx, li);
				BS2C_CompileExprPopType1(ctx);
				return;
			}
#endif

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

				if(BS2C_TypePointerP(ctx, lt))
					BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeSCx(ctx, li);
				BS2C_CompileExprPopType2(ctx);
				return;
			}
		}

#if 1
		t0=BS2C_InferExprLocalIndex(ctx, ln);
		t1=BS2C_InferExprLocalIndex(ctx, rn);
//		if((t0>=0) && (t1>=0))
		if((t0>=0) && (t1>=0) &&
			!BS2C_TypePointerP(ctx, lt))
		{
			if(BS2C_TypePointerP(ctx, lt))
				BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

			BS2C_EmitOpcode(ctx, BSVM2_OP_STIXZLL);
			BS2C_EmitOpcodeUZx(ctx, z, t0);
			BS2C_EmitOpcodeUCx(ctx, t1);
			BS2C_CompileExprPopType1(ctx);
			return;
		}
#endif

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

			if(BS2C_TypePointerP(ctx, lt))
				BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);
			BS2C_EmitOpcode(ctx, o);
			BS2C_CompileExprPopType3(ctx);
			return;
		}

		BS2C_CaseError(ctx);
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
					t0=BS2C_InferExprLocalIndex(ctx, ln);
					if(t0>=0)
					{
						BS2C_CompileExprPopType1(ctx);
//						BS2C_CompileExprPushType(ctx, vi2->bty);
						BS2C_EmitOpcode(ctx, BSVM2_OP_STOSL);
						BS2C_EmitOpcodeJx(ctx, t0, i);
//						BS2C_CompileConvType(ctx, dty);
						return;
					}

					if(BGBDT_TagStr_SymbolIsP(ln, "this"))
					{
						BS2C_CompileExprPopType1(ctx);
						BS2C_EmitOpcode(ctx, BSVM2_OP_STTHIS);
						BS2C_EmitOpcodeUCx(ctx, i);
						return;
					}

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

		if(BS2C_TypeVariantP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);
			i=BS2C_GetFrameSymbol(ctx, fn);

			t0=BS2C_InferExprLocalIndex(ctx, ln);
			if(t0>=0)
			{
				BS2C_CompileExprPopType1(ctx);
				BS2C_EmitOpcode(ctx, BSVM2_OP_STOSL);
				BS2C_EmitOpcodeJx(ctx, t0, i);
				return;
			}

			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileExprPopType2(ctx);
			BS2C_EmitOpcode(ctx, BSVM2_OP_STOS);
			BS2C_EmitOpcodeUCx(ctx, i);
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "unary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		lt=BS2C_InferExpr(ctx, ln);

		if(!strcmp(op, "*"))
		{
			if(BGBDT_TagStr_IsSymbolP(ln))
			{
				fn=BGBDT_TagStr_GetUtf8(ln);
				BS2C_CompileStoreDerefName(ctx, fn);
				return;
			}

			bty=BS2C_TypeDerefType(ctx, lt);
			z=BS2C_GetTypeBaseZ(ctx, bty);

			o=-1;
			if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT))
				o=BSVM2_OP_STDRI;
			if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG))
				o=BSVM2_OP_STDRL;
			if(z==BSVM2_OPZ_FLOAT)
				o=BSVM2_OP_STDRF;
			if(z==BSVM2_OPZ_DOUBLE)
				o=BSVM2_OP_STDRD;
			if(z==BSVM2_OPZ_ADDRESS)
				o=BSVM2_OP_STDRA;
			if((z==BSVM2_OPZ_SBYTE) || (z==BSVM2_OPZ_UBYTE))
				o=BSVM2_OP_STDRB;
			if((z==BSVM2_OPZ_SHORT) || (z==BSVM2_OPZ_USHORT))
				o=BSVM2_OP_STDRS;

			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);

				if(BS2C_TypePointerP(ctx, lt))
					BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		BS2C_CaseError(ctx);
//		BS2C_CompileExprBinary(ctx, op, ln, rn, dty);
		return;
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_CompileExprBinary(BS2CC_CompileContext *ctx,
	char *op, dtVal ln, dtVal rn, int dty)
{
	int lt, rt, ty, ty2;
	int z, o, u;
	int t0, t1, t2;
	s64 li, lj;
	int i, j, k;

	ln=BS2C_ReduceExpr(ctx, ln);
	rn=BS2C_ReduceExpr(ctx, rn);

	lt=BS2C_InferExpr(ctx, ln);
	rt=BS2C_InferExpr(ctx, rn);
	ty=BS2C_TypeBinarySuperType(ctx, op, lt, rt);
	ty2=ty;

	if(BS2C_TypeSmallLongP(ctx, ty) &&
		BS2C_TypeSmallLongP(ctx, dty))
	{
		ty2=BS2C_InferSuperType(ctx, dty, ty);
		if(((ty==BS2CC_TYZ_INT) || (ty==BS2CC_TYZ_UINT)) &&
			BS2C_TypeSmallIntP(ctx, dty))
				ty2=ty;
	}

//	ty2=BS2C_TypeBinarySuperType(ctx, op, dty, ty);

	if((BS2C_TypeArrayP(ctx, lt) ||
			BS2C_TypePointerP(ctx, lt)) &&
		(!strcmp(op, "+") || !strcmp(op, "-")))
	{
		if(dtvIsFixIntP(rn) || dtvIsFixUIntP(rn))
		{
			li=dtvUnwrapLong(rn);
			if(!strcmp(op, "-"))
				li=-li;
				
			ty=BS2C_TypeDerefType(ctx, lt);
			z=BS2C_GetTypeBaseZ(ctx, ty);
			
			o=-1;
			if((z==BS2CC_TYZ_INT) || (z==BS2CC_TYZ_UINT))
				{ o=BSVM2_OP_LEARIC; }
			else if((z==BS2CC_TYZ_LONG) || (z==BS2CC_TYZ_ULONG))
				{ o=BSVM2_OP_LEARLC; }
			else if(z==BS2CC_TYZ_FLOAT)
				{ o=BSVM2_OP_LEARFC; }
			else if(z==BS2CC_TYZ_DOUBLE)
				{ o=BSVM2_OP_LEARDC; }
			else if(z==BS2CC_TYZ_ADDRESS)
				{ o=BSVM2_OP_LEARAC; }
			else if((z==BS2CC_TYZ_SBYTE) || (z==BS2CC_TYZ_UBYTE))
				{ o=BSVM2_OP_LEARBC; }
			else if((z==BS2CC_TYZ_SHORT) || (z==BS2CC_TYZ_USHORT))
				{ o=BSVM2_OP_LEARSC; }

			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);

				if(BS2C_TypePointerP(ctx, lt))
					BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeSCx(ctx, li);

				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}
		
		if(BS2C_TypeSmallIntP(ctx, rt))
		{
			ty=BS2C_TypeDerefType(ctx, lt);
			z=BS2C_GetTypeBaseZ(ctx, ty);
			
			o=-1;
			if((z==BS2CC_TYZ_INT) || (z==BS2CC_TYZ_UINT))
				{ o=BSVM2_OP_LEARI; }
			else if((z==BS2CC_TYZ_LONG) || (z==BS2CC_TYZ_ULONG))
				{ o=BSVM2_OP_LEARL; }
			else if(z==BS2CC_TYZ_FLOAT)
				{ o=BSVM2_OP_LEARF; }
			else if(z==BS2CC_TYZ_DOUBLE)
				{ o=BSVM2_OP_LEARD; }
			else if(z==BS2CC_TYZ_ADDRESS)
				{ o=BSVM2_OP_LEARA; }
			else if((z==BS2CC_TYZ_SBYTE) || (z==BS2CC_TYZ_UBYTE))
				{ o=BSVM2_OP_LEARB; }
			else if((z==BS2CC_TYZ_SHORT) || (z==BS2CC_TYZ_USHORT))
				{ o=BSVM2_OP_LEARS; }

			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_CompileExpr(ctx, rn, BS2CC_TYZ_INT);

				if(!strcmp(op, "-"))
					BS2C_EmitOpcode(ctx, BSVM2_OP_NEGI);

				if(BS2C_TypePointerP(ctx, lt))
					BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

				BS2C_EmitOpcode(ctx, o);
//				BS2C_EmitOpcodeSCx(ctx, li);

				BS2C_CompileExprPopType1(ctx);

				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}
		
		if((BS2C_TypeArrayP(ctx, rt) ||
			BS2C_TypePointerP(ctx, rt)) &&
				!strcmp(op, "-"))
		{
			i=0;
			if(BS2C_TypeArrayP(ctx, lt))i|=1;
			if(BS2C_TypeArrayP(ctx, rt))i|=2;

			if(!BS2C_TypeSmallIntP(ctx, dty))
				i|=4;

			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileExpr(ctx, rn, rt);

			ty=BS2C_TypeDerefType(ctx, lt);
			z=BS2C_GetTypeBaseZ(ctx, ty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DIFFPTR);
			BS2C_EmitOpcodeUZx(ctx, z, i);

			BS2C_CompileExprPopType2(ctx);
			if(i&4)
				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_LONG);
			else
				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_INT);

			BS2C_CompileConvType(ctx, dty);
			return;
		}

	}

	if(BS2C_TypeStringP(ctx, lt) &&
		(!strcmp(op, "+") || !strcmp(op, "-")))
	{
		if(BS2C_TypeStringP(ctx, rt) && !strcmp(op, "+"))
		{
			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileExpr(ctx, rn, lt);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CATST);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		if(dtvIsFixIntP(rn) || dtvIsFixUIntP(rn))
		{
			li=dtvUnwrapLong(rn);
			if(!strcmp(op, "-"))
				li=-li;
			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LEASTC);
			BS2C_EmitOpcodeSCx(ctx, li);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		if(BS2C_TypeSmallIntP(ctx, rt))
		{
			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileExpr(ctx, rn, BS2CC_TYZ_INT);

			if(!strcmp(op, "-"))
				BS2C_EmitOpcode(ctx, BSVM2_OP_NEGI);

			BS2C_EmitOpcode(ctx, BSVM2_OP_LEAST);
//			BS2C_EmitOpcodeSCx(ctx, li);

			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileConvType(ctx, dty);
			return;
		}
	}

	if(BS2C_TypeStringP(ctx, lt) && !strcmp(op, "&"))
	{
		BS2C_CompileExpr(ctx, ln, lt);
		BS2C_CompileExpr(ctx, rn, rt);
		BS2C_CompileConvType(ctx, lt);
		BS2C_EmitOpcode(ctx, BSVM2_OP_CATST);
		BS2C_CompileExprPopType1(ctx);
		BS2C_CompileConvType(ctx, dty);
		return;
	}

	if(BS2C_TypeSmallIntP(ctx, ty))
	{
		if(dtvIsFixIntP(rn))
		{
			li=dtvUnwrapLong(rn);
			t0=BS2C_InferExprLocalIndex(ctx, ln);

//			if(!strcmp(op, "+"))
			if(!strcmp(op, "+") && (t0<0))
			{
				if(li==1)
				{
					BS2C_CompileExpr(ctx, ln, ty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_INCI);
					BS2C_CompileConvType(ctx, dty);
					return;
				}

				if(li==(-1))
				{
					BS2C_CompileExpr(ctx, ln, ty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_DECI);
					BS2C_CompileConvType(ctx, dty);
					return;
				}
			}

//			if(!strcmp(op, "-"))
			if(!strcmp(op, "-") && (t0<0))
			{
				if(li==1)
				{
					BS2C_CompileExpr(ctx, ln, ty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_DECI);
					BS2C_CompileConvType(ctx, dty);
					return;
				}

				if(li==(-1))
				{
					BS2C_CompileExpr(ctx, ln, ty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_INCI);
					BS2C_CompileConvType(ctx, dty);
					return;
				}
			}

			if(!strcmp(op, "/"))
			{
				if((li>0) && !(li&(li-1)))
				{
					lj=li; i=0;
					while(lj>1)
						{ lj=lj>>1; i++; }

					if(t0>=0)
					{
						BS2C_CompileExprPushType(ctx, lt);
						BS2C_EmitOpcode(ctx, BSVM2_OP_SARISLC);
						BS2C_EmitOpcodeSKx(ctx, t0, i);
						BS2C_CompileConvType(ctx, dty);
						return;
					}

					BS2C_CompileExpr(ctx, ln, ty);
					if(i>0)
					{
						BS2C_EmitOpcode(ctx, BSVM2_OP_SARIC);
						BS2C_EmitOpcodeSCx(ctx, i);
						BS2C_CompileConvType(ctx, dty);
					}
					return;
				}
			}

#if 1
			if(!strcmp(op, "*"))
			{
				if((li>0) && !(li&(li-1)))
				{
					lj=li; i=0;
					while(lj>1)
						{ lj=lj>>1; i++; }

					if(t0>=0)
					{
						BS2C_CompileExprPushType(ctx, lt);
						BS2C_EmitOpcode(ctx, BSVM2_OP_SHLISLC);
						BS2C_EmitOpcodeSKx(ctx, t0, i);
						BS2C_CompileConvType(ctx, dty);
						return;
					}

					BS2C_CompileExpr(ctx, ln, ty);
					if(i>0)
					{
						BS2C_EmitOpcode(ctx, BSVM2_OP_SHLIC);
						BS2C_EmitOpcodeSCx(ctx, i);
						BS2C_CompileConvType(ctx, dty);
					}
					return;
				}
			}
#endif

#if 1
//			t0=BS2C_InferExprLocalIndex(ctx, ln);
			if(t0>=0)
			{
				o=-1; u=0;
				if(!strcmp(op, "+")) { o=BSVM2_OP_ADDISLC; u=1; }
				if(!strcmp(op, "-")) { o=BSVM2_OP_SUBISLC; u=1; }
				if(!strcmp(op, "&"))o=BSVM2_OP_ANDISLC;
				if(!strcmp(op, "|"))o=BSVM2_OP_ORISLC;
				if(!strcmp(op, "^"))o=BSVM2_OP_XORISLC;
				if(!strcmp(op, "<<"))o=BSVM2_OP_SHLISLC;

//				if(BS2C_TypeSignedP(ctx, dty))
//				if(!BS2C_TypeUnsigned2P(ctx, dty))
				if(!BS2C_TypeUnsigned2P(ctx, ty))
				{
					if(!strcmp(op, "*"))o=BSVM2_OP_MULISLC;
					if(!strcmp(op, ">>"))o=BSVM2_OP_SARISLC;
				}

				if(o>0)
				{
					BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
					BS2C_EmitOpcode(ctx, o);
					if(u)
						{ BS2C_EmitOpcodeUKx(ctx, t0, li); }
					else
						{ BS2C_EmitOpcodeSKx(ctx, t0, li); }
					BS2C_CompileConvType(ctx, dty);
					return;
				}
			}
#endif

			o=-1; u=0;
			if(!strcmp(op, "+")) { o=BSVM2_OP_ADDIC; u=1; }
			if(!strcmp(op, "-")) { o=BSVM2_OP_SUBIC; u=1; }
			if(!strcmp(op, "&"))o=BSVM2_OP_ANDIC;
			if(!strcmp(op, "|"))o=BSVM2_OP_ORIC;
			if(!strcmp(op, "^"))o=BSVM2_OP_XORIC;
			if(!strcmp(op, "<<"))o=BSVM2_OP_SHLIC;

//			if(BS2C_TypeSignedP(ctx, dty))
//			if(!BS2C_TypeUnsigned2P(ctx, dty))
			if(!BS2C_TypeUnsigned2P(ctx, ty))
			{
				if(!strcmp(op, "*"))o=BSVM2_OP_MULIC;
				if(!strcmp(op, ">>"))o=BSVM2_OP_SARIC;
			}
			
			if(o>0)
			{
				BS2C_CompileExpr(ctx, ln, ty);
				BS2C_EmitOpcode(ctx, o);
				if(u)
					{ BS2C_EmitOpcodeUCx(ctx, li); }
				else
					{ BS2C_EmitOpcodeSCx(ctx, li); }
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}

		if(dtvIsFixIntP(ln))
		{
			li=dtvUnwrapLong(ln);
			t0=BS2C_InferExprLocalIndex(ctx, rn);

//			if(!strcmp(op, "+"))
			if(!strcmp(op, "+") && (t0<0))
			{
				if(li==1)
				{
					BS2C_CompileExpr(ctx, rn, ty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_INCI);
					BS2C_CompileConvType(ctx, dty);
					return;
				}

				if(li==(-1))
				{
					BS2C_CompileExpr(ctx, rn, ty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_DECI);
					BS2C_CompileConvType(ctx, dty);
					return;
				}
			}

#if 1
//			t0=BS2C_InferExprLocalIndex(ctx, rn);
			if(t0>=0)
			{
				o=-1; u=0;
				if(!strcmp(op, "+")) { o=BSVM2_OP_ADDISLC; u=1; }
				if(!strcmp(op, "&"))o=BSVM2_OP_ANDISLC;
				if(!strcmp(op, "|"))o=BSVM2_OP_ORISLC;
				if(!strcmp(op, "^"))o=BSVM2_OP_XORISLC;

//				if(BS2C_TypeSignedP(ctx, dty))
				if(!BS2C_TypeUnsigned2P(ctx, ty))
				{
					if(!strcmp(op, "*"))o=BSVM2_OP_MULISLC;
				}

				if(o>0)
				{
					BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
					BS2C_EmitOpcode(ctx, o);
					if(u)
						{ BS2C_EmitOpcodeUKx(ctx, t0, li); }
					else
						{ BS2C_EmitOpcodeSKx(ctx, t0, li); }
					BS2C_CompileConvType(ctx, dty);
					return;
				}
			}
#endif

			o=-1; u=0;
			if(!strcmp(op, "+")) { o=BSVM2_OP_ADDIC; u=1; }
			if(!strcmp(op, "&"))o=BSVM2_OP_ANDIC;
			if(!strcmp(op, "|"))o=BSVM2_OP_ORIC;
			if(!strcmp(op, "^"))o=BSVM2_OP_XORIC;
//			if(BS2C_TypeSignedP(ctx, ty))
			if(!BS2C_TypeUnsigned2P(ctx, ty))
				{ if(!strcmp(op, "*"))o=BSVM2_OP_MULIC; }
			
			if(o>0)
			{
				BS2C_CompileExpr(ctx, rn, ty);
				BS2C_EmitOpcode(ctx, o);
				if(u)
					{ BS2C_EmitOpcodeUCx(ctx, li); }
				else
					{ BS2C_EmitOpcodeSCx(ctx, li); }
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}

#if 1
		t0=BS2C_InferExprLocalIndex(ctx, ln);
		t1=BS2C_InferExprLocalIndex(ctx, rn);
		if((t0>=0) && (t1>=0))
		{
			o=-1; u=0;
			if(!strcmp(op, "+"))o=BSVM2_OP_ADDISLL;
			if(!strcmp(op, "-"))o=BSVM2_OP_SUBISLL;
			if(!strcmp(op, "&"))o=BSVM2_OP_ANDISLL;
			if(!strcmp(op, "|"))o=BSVM2_OP_ORISLL;
			if(!strcmp(op, "^"))o=BSVM2_OP_XORISLL;
			if(!strcmp(op, "<<"))o=BSVM2_OP_SHLISLL;

//			if(BS2C_TypeSignedP(ctx, dty))
			if(!BS2C_TypeUnsigned2P(ctx, ty))
			{
				if(!strcmp(op, "*"))o=BSVM2_OP_MULISLL;
				if(!strcmp(op, ">>"))o=BSVM2_OP_SARISLL;
			}

			if(o>0)
			{
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeJx(ctx, t0, t1);
				BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}
#endif

#if 1
		if(t1>=0)
		{
			o=-1; u=0;
			if(!strcmp(op, "+"))o=BSVM2_OP_ADDIL;
			if(!strcmp(op, "-"))o=BSVM2_OP_SUBIL;
			if(!strcmp(op, "&"))o=BSVM2_OP_ANDIL;
			if(!strcmp(op, "|"))o=BSVM2_OP_ORIL;
			if(!strcmp(op, "^"))o=BSVM2_OP_XORIL;
			if(!strcmp(op, "<<"))o=BSVM2_OP_SHLIL;

//			if(BS2C_TypeSignedP(ctx, dty))
			if(!BS2C_TypeUnsigned2P(ctx, ty))
			{
				if(!strcmp(op, "*"))o=BSVM2_OP_MULIL;
				if(!strcmp(op, ">>"))o=BSVM2_OP_SARIL;
			}

			if(o>0)
			{
				BS2C_CompileExpr(ctx, ln, ty);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeUCx(ctx, t1);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}

		if(t0>=0)
		{
			o=-1; u=0;
			if(!strcmp(op, "+"))o=BSVM2_OP_ADDIL;
			if(!strcmp(op, "&"))o=BSVM2_OP_ANDIL;
			if(!strcmp(op, "|"))o=BSVM2_OP_ORIL;
			if(!strcmp(op, "^"))o=BSVM2_OP_XORIL;

//			if(BS2C_TypeSignedP(ctx, dty))
			if(!BS2C_TypeUnsigned2P(ctx, ty))
				{ if(!strcmp(op, "*"))o=BSVM2_OP_MULIL; }

			if(o>0)
			{
				BS2C_CompileExpr(ctx, rn, ty);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeUCx(ctx, t0);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}
#endif
	}

	if(BS2C_TypeSmallLongP(ctx, ty) &&
		!BS2C_TypeSmallLongP(ctx, dty))
	{
		BS2C_CompileExpr(ctx, ln, ty);
		BS2C_CompileExpr(ctx, rn, ty);
		BS2C_CompileBinaryOpSS(ctx, op, ty);
		BS2C_CompileConvType(ctx, dty);
		return;
	}
	
//	if((BS2C_TypeVec2fP(ctx, lt) ||
//		BS2C_TypeVec3fP(ctx, lt) ||
//		BS2C_TypeVec4fP(ctx, lt) ||
//		BS2C_TypeVec2dP(ctx, lt)) &&
//		BS2C_TypeSmallDoubleP(ctx, rt) &&
//		(!strcmp(op, "*") ||
//		 !strcmp(op, "/")))

	if(BS2C_TypeOpXvP(ctx, lt) &&
		BS2C_TypeSmallDoubleP(ctx, rt) &&
		(!strcmp(op, "*") || !strcmp(op, "/")))
	{
		BS2C_CompileExpr(ctx, ln, lt);
		BS2C_CompileExpr(ctx, rn, BS2CC_TYZ_FLOAT);
		BS2C_CompileCastType(ctx, lt);
		BS2C_CompileBinaryOpSS(ctx, op, lt);
		return;
	}

	if(	BS2C_TypeSmallDoubleP(ctx, lt) &&
		BS2C_TypeOpXvP(ctx, rt) &&
		(!strcmp(op, "*") || !strcmp(op, "/")))
	{
		BS2C_CompileExpr(ctx, ln, BS2CC_TYZ_FLOAT);
		BS2C_CompileCastType(ctx, rt);
		BS2C_CompileExpr(ctx, rn, rt);
		BS2C_CompileBinaryOpSS(ctx, op, rt);
		return;
	}

	BS2C_CompileExpr(ctx, ln, ty2);
	BS2C_CompileExpr(ctx, rn, ty2);
	BS2C_CompileBinaryOpSS(ctx, op, ty2);
	BS2C_CompileConvType(ctx, dty);
	return;
}

void BS2C_CompileExprPushConstInt(
	BS2CC_CompileContext *ctx, s64 val, int dty)
{
	s64 li, lj;
	
	li=val;
	
	if(dty==BSVM2_OPZ_INT)
	{
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
			return;
		}
	
		if(li>0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
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
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UINT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
			return;
		}
	
		lj=(s32)li;
		if(lj<0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UINT);
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
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHL);
			return;
		}
	
		if(li>=0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_ULONG, li);
			return;
		}
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG, li);
		return;
	}
	if(dty==BSVM2_OPZ_ULONG)
	{
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_ULONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHL);
			return;
		}
	
		if(li<0)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_ULONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_LONG, li);
			return;
		}
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_ULONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_ULONG, li);
		return;
	}

	if((dty==BSVM2_OPZ_UBYTE) || (dty==BS2CC_TYZ_BOOL))
	{
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, dty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UBYTE, (byte)li);
		return;
	}
	if(dty==BSVM2_OPZ_SBYTE)
	{
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_SBYTE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_SBYTE, (sbyte)li);
		return;
	}
	if(dty==BSVM2_OPZ_USHORT)
	{
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_USHORT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_USHORT, (u16)li);
		return;
	}
	if(dty==BSVM2_OPZ_SHORT)
	{
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_SHORT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_SHORT, (s16)li);
		return;
	}
	
	if(dty==BS2CC_TYZ_CHAR)
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(li>=0)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UINT, li);
		}else
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, li);
		}
		return;
	}

	if(dty==BSVM2_OPZ_FLOAT)
	{
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_FLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxF(ctx, li);
		return;
	}
	if(dty==BSVM2_OPZ_DOUBLE)
	{
		if(li==0)
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxD(ctx, li);
		return;
	}

	if(dty==BS2CC_TYZ_HFLOAT)
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxF(ctx, li);
		BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2HF);
		return;
	}

	if(dty==BS2CC_TYZ_VARIANT)
	{
		if(li==((s32)li))
		{
			if(li>0)
			{
				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDCA);
				BS2C_EmitOpcodeUZy(ctx, BSVM2_OPZ_UINT, li);
				return;
			}

			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDCA);
			BS2C_EmitOpcodeSZy(ctx, BSVM2_OPZ_INT, li);
			return;
		}
			

		if(li>0)
		{
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDCA);
			BS2C_EmitOpcodeUZy(ctx, BSVM2_OPZ_ULONG, li);
			return;
		}
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDCA);
		BS2C_EmitOpcodeSZy(ctx, BSVM2_OPZ_LONG, li);
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileExprPushConstFloat(
	BS2CC_CompileContext *ctx, f64 val, int dty)
{
	if(dty==BSVM2_OPZ_FLOAT)
	{
		if(val==0)
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_FLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxF(ctx, val);
		return;
	}
	if(dty==BSVM2_OPZ_DOUBLE)
	{
		if(val==0)
		{
			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
			return;
		}
	
		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxD(ctx, val);
		return;
	}

	if(dty==BS2CC_TYZ_HFLOAT)
	{
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZxF(ctx, val);
		BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2HF);
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileExpr(BS2CC_CompileContext *ctx,
	dtVal expr, int dty)
{
	dtVal n0, n1, n2;
	dtVal ln, rn, cn, ln2, rn2;
	s64 li, lj, lk;
	BGBDTC_X128 xa, xb, xc;
	BS2CC_VarInfo *vi, *vi2, *vi3, *vi4;
	double f, g;
	char *tag, *tag2;
	char *op, *fn;
	int t0, t1, t2, t3;
	int lt, rt, lt2, rt2, cty, bty;
	int z, o, u, z2;
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
		if(dty==BS2CC_TYZ_FLOAT)
		{
			f=dtvUnwrapDouble(expr);
			if(f==0)
			{
				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
				return;
			}
	
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeZxF(ctx, f);
			return;
		}
		if(dty==BS2CC_TYZ_DOUBLE)
		{
			f=dtvUnwrapDouble(expr);
			if(f==0)
			{
				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_DOUBLE);
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
				return;
			}
	
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeZxD(ctx, f);
			return;
		}

		if(dty==BS2CC_TYZ_HFLOAT)
		{
			f=dtvUnwrapDouble(expr);
			if(f==0)
			{
				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
				return;
			}
	
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeZxF(ctx, f);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2HF);
			return;
		}

		if(dty==BS2CC_TYZ_VARIANT)
		{
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDCA);
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

	if(BGBDT_XI128_IsInt128P(expr) ||
		BGBDT_XF128_IsFloat128P(expr))
	{
		if(BGBDT_XI128_IsInt128P(expr))
		{
			xa=BGBDT_XI128_UnwrapInt128(expr);
			bty=BS2CC_TYZ_INT128;
			if(dty==BS2CC_TYZ_UINT128)
				bty=BS2CC_TYZ_UINT128;

			if(BS2C_TypeSmallLongP(ctx, dty))
			{
				BS2C_CompileExprPushConstInt(ctx,
					(s64)xa.la, dty);
				return;
			}

			if(BS2C_TypeSmallDoubleP(ctx, dty))
			{
				f=(s64)xa.la;
				BS2C_CompileExprPushType(ctx, dty);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
				BS2C_EmitOpcodeZxD(ctx, f);
				return;
			}
		}else
		{
			xa=BGBDT_XF128_UnwrapFloat128(expr);
			bty=BS2CC_TYZ_FLOAT128;

			if(BS2C_TypeSmallLongP(ctx, dty))
			{
				f=BGBDT_XF128_ToDouble(xa);
				BS2C_CompileExprPushConstInt(ctx, (s64)f, dty);
				return;
			}

			if(BS2C_TypeSmallDoubleP(ctx, dty))
			{
				f=BGBDT_XF128_ToDouble(xa);
				BS2C_CompileExprPushType(ctx, dty);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
				BS2C_EmitOpcodeZxD(ctx, f);
				return;
			}
		}

		if(xa.lb==0)
		{
			BS2C_CompileExprPushConstInt(ctx, (s64)xa.la, BS2CC_TYZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUL2XL);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, bty);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		if((((s64)xa.lb)==((s64)(-1))) &&
			((((s64)xa.la)>>63)==((s64)(-1))))
		{
			BS2C_CompileExprPushConstInt(ctx, (s64)xa.la, BS2CC_TYZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2XL);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, bty);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		if(((((s32)xa.a)==((((s32)xa.a)<<8)>>8)) &&
			(((s32)xa.b)==((((s32)xa.b)<<8)>>8)) &&
			(((s32)xa.c)==((((s32)xa.c)<<8)>>8)) &&
			(((s32)xa.d)==((((s32)xa.d)<<8)>>8))) ||
			((((s32)xa.a)==((((s32)xa.a)<<16)>>16)) &&
			 (((s32)xa.c)==((((s32)xa.c)<<16)>>16))) )
		{
			BS2C_CompileExprPushConstInt(ctx, xa.a, BS2CC_TYZ_INT);
			BS2C_CompileExprPushConstInt(ctx, xa.b, BS2CC_TYZ_INT);
			BS2C_CompileExprPushConstInt(ctx, xa.c, BS2CC_TYZ_INT);
			BS2C_CompileExprPushConstInt(ctx, xa.d, BS2CC_TYZ_INT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX4I);
			BS2C_CompileExprPopType4(ctx);
			BS2C_CompileExprPushType(ctx, bty);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		BS2C_CompileExprPushConstInt(ctx, xa.la, BS2CC_TYZ_ULONG);
		BS2C_CompileExprPushConstInt(ctx, xa.lb, BS2CC_TYZ_ULONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_MKX2L);
		BS2C_CompileExprPopType2(ctx);
		BS2C_CompileExprPushType(ctx, bty);
		BS2C_CompileConvType(ctx, dty);
		return;
	}

	if(dtvIsCharP(expr))
	{
		li=dtvUnwrapChar(expr);
		BS2C_CompileExprPushConstInt(ctx, li, dty);
		return;
	}

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		fn=BGBDT_TagStr_GetUtf8(expr);

		if(!strcmp(fn, "null"))
		{
			if(BS2C_TypeSmallDoubleP(ctx, dty))
			{
				BS2C_WarnImplicitConv(ctx, dty, BS2CC_TYZ_VARIANT);
				BS2C_CompileExprPushConstInt(ctx, 0, dty);
				return;
			}

			BS2C_CompileExprPushType(ctx, dty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
			return;
		}

		if(!strcmp(fn, "undefined"))
		{
			if(BS2C_TypeSmallDoubleP(ctx, dty))
			{
				BS2C_WarnImplicitConv(ctx, dty, BS2CC_TYZ_VARIANT);
				BS2C_CompileExprPushConstInt(ctx, 0, dty);
				return;
			}

			BS2C_CompileExprPushType(ctx, dty);
//			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);

			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_VOID, 1);
			return;
		}
		
		if(!strcmp(fn, "true"))
		{
			if(BS2C_TypeSmallDoubleP(ctx, dty))
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
			if(BS2C_TypeSmallDoubleP(ctx, dty))
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
//		i=BS2C_ImgGetString(ctx, BGBDT_TagStr_GetUtf8(expr));
//		BS2C_CompileLoadNameAsType(ctx,
//			BGBDT_TagStr_GetUtf8(expr), dty);

//		BS2C_EmitOpcode(ctx, BSVM2_OP_LDCA);
//		BS2C_EmitOpcodeUZy(ctx, BSVM2_OPZ_UBYTE, i);
//		BS2C_EmitOpcodeZyStr(ctx, BGBDT_TagStr_GetUtf8(expr));

		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_STRING);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZyStr(ctx, BGBDT_TagStr_GetUtf8(expr));
		return;
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "symbol"))
	{
		op=BS2P_GetAstNodeAttrS(expr, "value");
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZySymbol(ctx, op);
		return;
	}
	
	if(!strcmp(tag, "keyword"))
	{
		op=BS2P_GetAstNodeAttrS(expr, "value");
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
		BS2C_EmitOpcodeZyKeyword(ctx, op);
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
//		cty=BS2C_InferSuperType(ctx, lt, rt);
		cty=BS2C_TypeCompareSuperType(ctx, lt, rt);

		BS2C_CompileExpr(ctx, ln, cty);
		BS2C_CompileExpr(ctx, rn, cty);

#if 1
		o=-1;
//		if(!strcmp(op, "=="))
//			o=BSVM2_OP_CMIEQ;
		if(!strcmp(op, "=="))
			o=BSVM2_OP_LNTI;
		if(!strcmp(op, "!="))
			o=BSVM2_OP_CMINE;
		if(!strcmp(op, "<"))
			o=BSVM2_OP_CMILT;
		if(!strcmp(op, ">"))
			o=BSVM2_OP_CMIGT;
		if(!strcmp(op, "<="))
			o=BSVM2_OP_CMILE;
		if(!strcmp(op, ">="))
			o=BSVM2_OP_CMIGE;
//		if(!strcmp(op, "==="))
//			o=BSVM2_OP_CMIEQ;
		if(!strcmp(op, "==="))
			o=BSVM2_OP_LNTI;
		if(!strcmp(op, "!=="))
			o=BSVM2_OP_CMINE;

		u=-1;
		if(BS2C_TypeSmallIntP(ctx, cty))
			u=BSVM2_OP_CMPI;
		else if(BS2C_TypeSmallLongP(ctx, cty))
			u=BSVM2_OP_CMPL;
		else if(BS2C_TypeSmallFloatP(ctx, cty))
			u=BSVM2_OP_CMPF;
		else if(BS2C_TypeSmallDoubleP(ctx, cty))
			u=BSVM2_OP_CMPD;
		else if(BS2C_TypeInt128P(ctx, cty))
			u=BSVM2_OP_CMPXL;
		else if(BS2C_TypeFloat128P(ctx, cty))
			u=BSVM2_OP_CMPXF;
		else if(BS2C_TypeAddressP(ctx, cty))
			u=BSVM2_OP_CMPA;

		if((o>=0) && (u>=0))
		{
			BS2C_CompileExprPopType2(ctx);

			BS2C_EmitOpcode(ctx, u);
			BS2C_EmitOpcode(ctx, o);

			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
			BS2C_CompileConvType(ctx, dty);
			return;
		}
#endif

		BS2C_CaseError(ctx);
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
			if(BS2C_TypeSmallIntP(ctx, lt))
				{ o=BSVM2_OP_NEGI; }
			else if(BS2C_TypeSmallLongP(ctx, lt))
				{ o=BSVM2_OP_NEGL; }
			else if(BS2C_TypeSmallFloatP(ctx, lt))
				{ o=BSVM2_OP_NEGF; }
			else if(BS2C_TypeSmallDoubleP(ctx, lt))
				{ o=BSVM2_OP_NEGD; }
			
			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			if(BS2C_TypeOpXvP(ctx, lt))
			{
				BS2C_CompileExpr(ctx, ln, lt);
				z=BS2C_GetTypeVecZ(ctx, lt);
				BS2C_EmitOpcode(ctx, BSVM2_OP_UNOPX);
				BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPUV_NEG);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			if(BS2C_TypeOpXNumP(ctx, lt))
			{
				BS2C_CompileExpr(ctx, ln, lt);
				z=BS2C_GetTypeVecZ(ctx, lt);
				BS2C_EmitOpcode(ctx, BSVM2_OP_UNOPX);
				BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRU_NEG);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(!strcmp(op, "~"))
		{
			o=-1;
			if(BS2C_TypeSmallIntP(ctx, lt))
				{ o=BSVM2_OP_NOTI; }
			else if(BS2C_TypeSmallLongP(ctx, lt))
				{ o=BSVM2_OP_NOTL; }
			
			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			if(BS2C_TypeOpXNumP(ctx, lt))
			{
				BS2C_CompileExpr(ctx, ln, lt);
				z=BS2C_GetTypeVecZ(ctx, lt);
				BS2C_EmitOpcode(ctx, BSVM2_OP_UNOPX);
				BS2C_EmitOpcodeUZx(ctx, z, BSVM2_OPRU_NOT);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(!strcmp(op, "!"))
		{
			o=-1;
			if(BS2C_TypeSmallIntP(ctx, dty))
				{ o=BSVM2_OP_LNTI; }
			else if(BS2C_TypeSmallLongP(ctx, dty))
				{ o=BSVM2_OP_LNTL; }
			
			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(!strcmp(op, "&"))
		{
			if(BGBDT_TagStr_IsSymbolP(ln))
			{
				fn=BGBDT_TagStr_GetUtf8(ln);
				BS2C_CompileLoadRefName(ctx, fn);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(!strcmp(op, "*"))
		{
			if(BGBDT_TagStr_IsSymbolP(ln))
			{
				fn=BGBDT_TagStr_GetUtf8(ln);
				BS2C_CompileLoadDerefName(ctx, fn);
				return;
			}

			bty=BS2C_TypeDerefType(ctx, lt);
			z=BS2C_GetTypeBaseZ(ctx, bty);

			if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT))
				o=BSVM2_OP_LDDRI;
			if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG))
				o=BSVM2_OP_LDDRL;
			if(z==BSVM2_OPZ_FLOAT)
				o=BSVM2_OP_LDDRF;
			if(z==BSVM2_OPZ_DOUBLE)
				o=BSVM2_OP_LDDRD;
			if(z==BSVM2_OPZ_ADDRESS)
				o=BSVM2_OP_LDDRA;
			if(z==BSVM2_OPZ_SBYTE)
				o=BSVM2_OP_LDDRSB;
			if(z==BSVM2_OPZ_UBYTE)
				o=BSVM2_OP_LDDRUB;
			if(z==BSVM2_OPZ_SHORT)
				o=BSVM2_OP_LDDRSS;
			if(z==BSVM2_OPZ_USHORT)
				o=BSVM2_OP_LDDRUS;

			if(o>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);

				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, bty);

				if(BS2C_TypePointerP(ctx, lt))
					BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			BS2C_CaseError(ctx);
			return;
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

		if(BS2C_TypeVoidP(ctx, dty) && (lt==rt) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			BGBDT_TagStr_IsSymbolP(rn))
		{
			i=BS2C_InferExprLocalIndex(ctx, ln);
			j=BS2C_InferExprLocalIndex(ctx, rn);

			if((i>=0) && (j>=0))
			{
				o=-1;
				if(BS2C_TypeSmallIntP(ctx, lt))
					{ o=BSVM2_OP_MVI; }
				else if(BS2C_TypeSmallLongP(ctx, lt))
					{ o=BSVM2_OP_MVL; }
				else if(BS2C_TypeFloatP(ctx, lt))
					{ o=BSVM2_OP_MVF; }
				else if(BS2C_TypeDoubleP(ctx, lt))
					{ o=BSVM2_OP_MVD; }
				else if(BS2C_TypeHalfFloatP(ctx, lt))
					{ o=BSVM2_OP_MVI; }
				else if(BS2C_TypeAddressP(ctx, lt))
					{ o=BSVM2_OP_MVA; }

				if(o>=0)
				{
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeJx(ctx, i, j);
					return;
				}
			}
		}

		if(BS2C_TypeVoidP(ctx, dty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			dtvIsFixIntP(rn))
		{
			i=BS2C_InferExprLocalIndex(ctx, ln);
			li=dtvUnwrapLong(rn);

			if(i>=0)
			{
				o=-1;
				if(BS2C_TypeSmallIntP(ctx, lt))
					{ o=BSVM2_OP_MVIC; }
				else if(BS2C_TypeSmallLongP(ctx, lt))
					{ o=BSVM2_OP_MVLC; }
				else if(BS2C_TypeFloatP(ctx, lt))
					{ o=BSVM2_OP_MVFC; }
				else if(BS2C_TypeDoubleP(ctx, lt))
					{ o=BSVM2_OP_MVDC; }
//				else if(BS2C_TypeAddressP(ctx, lt))
//					{ o=BSVM2_OP_MVA; }

				if(o>=0)
				{
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeSuKx(ctx, lt, i, li);
					return;
				}
			}
		}

		tag2=BS2P_GetAstNodeTag(rn);
		if(BS2C_TypeVoidP(ctx, dty) &&
			BS2C_TypeSmallIntP(ctx, cty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			tag2 && !strcmp(tag2, "binary"))
		{
			ln2=BS2P_GetAstNodeAttr(rn, "lhs");
			rn2=BS2P_GetAstNodeAttr(rn, "rhs");
			op=BS2P_GetAstNodeAttrS(rn, "op");

			lt2=BS2C_InferExpr(ctx, ln2);
			rt2=BS2C_InferExpr(ctx, rn2);
			
			if(BGBDT_TagStr_IsSymbolP(ln2) &&
				BGBDT_TagStr_IsSymbolP(rn2) &&
				BS2C_TypeSmallIntP(ctx, lt2) &&
				BS2C_TypeSmallIntP(ctx, rt2))
			{
				t0=BS2C_InferExprLocalIndex(ctx, ln);
				t1=BS2C_InferExprLocalIndex(ctx, ln2);
				t2=BS2C_InferExprLocalIndex(ctx, rn2);

				o=-1;
				if(!strcmp(op, "+"))
					o=BSVM2_OP_ADDILLL;
				if(!strcmp(op, "-"))
					o=BSVM2_OP_SUBILLL;
				if(!strcmp(op, "*"))
					o=BSVM2_OP_MULILLL;
				if(!strcmp(op, "&"))
					o=BSVM2_OP_ANDILLL;
				if(!strcmp(op, "|"))
					o=BSVM2_OP_ORILLL;
				if(!strcmp(op, "^"))
					o=BSVM2_OP_XORILLL;
				if(!strcmp(op, "<<"))
					o=BSVM2_OP_SHLILLL;
				if(!BS2C_TypeUnsignedP(ctx, lt))
				{
					if(!strcmp(op, ">>"))
						o=BSVM2_OP_SARILLL;
				}

				if((o>=0) && (t0>=0) && (t1>=0) && (t2>=0))
				{
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeLx(ctx, t0, t1, t2);
					return;
				}
			}

			if(BGBDT_TagStr_IsSymbolP(ln2) &&
				BS2C_TypeSmallIntP(ctx, lt2) &&
				dtvIsFixIntP(rn2))
			{
				t0=BS2C_InferExprLocalIndex(ctx, ln);
				t1=BS2C_InferExprLocalIndex(ctx, ln2);
				li=dtvUnwrapLong(rn2);

				o=-1; u=0;
				if(!strcmp(op, "+"))
				{
					o=BSVM2_OP_ADDILLC; u=1;
					if(li<0) { o=BSVM2_OP_SUBILLC; li=-li; }
				}
				if(!strcmp(op, "-"))
				{
					o=BSVM2_OP_SUBILLC; u=1;
					if(li<0) { o=BSVM2_OP_ADDILLC; li=-li; }
				}
				if(!strcmp(op, "*"))
					o=BSVM2_OP_MULILLC;
				if(!strcmp(op, "&"))
					o=BSVM2_OP_ANDILLC;
				if(!strcmp(op, "|"))
					o=BSVM2_OP_ORILLC;
				if(!strcmp(op, "^"))
					o=BSVM2_OP_XORILLC;
				if(!strcmp(op, "<<"))
					o=BSVM2_OP_SHLILLC;
				if(!BS2C_TypeUnsignedP(ctx, lt))
				{
					if(!strcmp(op, ">>"))
						o=BSVM2_OP_SARILLC;
				}

				if((o>=0) && (t0>=0) && (t1>=0))
				{
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeJx(ctx, t0, t1);
					if(u)	{ BS2C_EmitOpcodeUCx(ctx, li); }
					else	{ BS2C_EmitOpcodeSCx(ctx, li); }
					return;
				}
			}
		}

		BS2C_CompileExpr(ctx, rn, cty);
		BS2C_CompileConvType(ctx, lt);
		
		if(!BS2C_TypeVoidP(ctx, dty))
			{ BS2C_CompileDup(ctx); }

		BS2C_CompileStoreExpr(ctx, ln);
	
		if(!BS2C_TypeVoidP(ctx, dty))
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
//		cty=BS2C_InferSuperType(ctx, lt, rt);
		cty=BS2C_TypeBinarySuperType(ctx, op, lt, rt);

		if(BS2C_TypeVoidP(ctx, dty) && (lt==rt) &&
			BS2C_TypeSmallIntP(ctx, lt) &&
			BS2C_TypeSmallIntP(ctx, cty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			BGBDT_TagStr_IsSymbolP(rn))
		{
			i=BS2C_InferExprLocalIndex(ctx, ln);
			j=BS2C_InferExprLocalIndex(ctx, rn);

			if((i>=0) && (j>=0))
			{
				o=-1;
//				if(BS2C_TypeSmallIntP(ctx, lt))
//					{ o=BSVM2_OP_MVI; }
				if(!strcmp(op, "+"))
					o=BSVM2_OP_ADDILLL;
				if(!strcmp(op, "-"))
					o=BSVM2_OP_SUBILLL;
				if(!strcmp(op, "*"))
					o=BSVM2_OP_MULILLL;
				if(!strcmp(op, "&"))
					o=BSVM2_OP_ANDILLL;
				if(!strcmp(op, "|"))
					o=BSVM2_OP_ORILLL;
				if(!strcmp(op, "^"))
					o=BSVM2_OP_XORILLL;
				if(!strcmp(op, "<<"))
					o=BSVM2_OP_SHLILLL;
				if(!BS2C_TypeUnsignedP(ctx, lt))
				{
					if(!strcmp(op, ">>"))
						o=BSVM2_OP_SARILLL;
				}

				if(o>=0)
				{
					BS2C_EmitOpcode(ctx, o);
//					BS2C_EmitOpcodeJx(ctx, i, j);
					BS2C_EmitOpcodeLx(ctx, i, i, j);
					return;
				}
			}
		}

		if(BS2C_TypeVoidP(ctx, dty) &&
			BS2C_TypeSmallIntP(ctx, lt) &&
			BS2C_TypeSmallIntP(ctx, cty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			dtvIsFixIntP(rn))
		{
			i=BS2C_InferExprLocalIndex(ctx, ln);
			li=dtvUnwrapLong(rn);

			if(i>=0)
			{
				o=-1; u=0;

				if(!strcmp(op, "+"))
				{
					o=BSVM2_OP_ADDILLC; u=1;
					if(li<0)
						{ o=BSVM2_OP_SUBILLC; li=-li; }
				}
				if(!strcmp(op, "-"))
				{
					o=BSVM2_OP_SUBILLC; u=1;
					if(li<0)
						{ o=BSVM2_OP_ADDILLC; li=-li; }
				}
				if(!strcmp(op, "*"))
					o=BSVM2_OP_MULILLC;
				if(!strcmp(op, "&"))
					o=BSVM2_OP_ANDILLC;
				if(!strcmp(op, "|"))
					o=BSVM2_OP_ORILLC;
				if(!strcmp(op, "^"))
					o=BSVM2_OP_XORILLC;
				if(!strcmp(op, "<<"))
					o=BSVM2_OP_SHLILLC;
				if(!BS2C_TypeUnsignedP(ctx, lt))
				{
					if(!strcmp(op, ">>"))
						o=BSVM2_OP_SARILLC;
				}

				if(o>=0)
				{
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeJx(ctx, i, i);
					if(u)	{ BS2C_EmitOpcodeUCx(ctx, li); }
					else	{ BS2C_EmitOpcodeSCx(ctx, li); }
					return;
				}
			}
		}

		if(BS2C_TypeVoidP(ctx, dty) &&
			BS2C_TypeSmallIntP(ctx, lt) &&
			BS2C_TypeSmallIntP(ctx, cty) &&
			BGBDT_TagStr_IsSymbolP(ln))
		{
			i=BS2C_InferExprLocalIndex(ctx, ln);
			if(i>=0)
			{
				o=-1; u=0;

				if(!strcmp(op, "+"))
					o=BSVM2_OP_ADDILSL;
				if(!strcmp(op, "*"))
					o=BSVM2_OP_MULILSL;
				if(!strcmp(op, "&"))
					o=BSVM2_OP_ANDILSL;
				if(!strcmp(op, "|"))
					o=BSVM2_OP_ORILSL;
				if(!strcmp(op, "^"))
					o=BSVM2_OP_XORILSL;

				if(o>=0)
				{
					BS2C_CompileExpr(ctx, rn, lt);
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeJx(ctx, i, i);
					BS2C_CompileExprPopType1(ctx);
					return;
				}
			}
		}


		BS2C_CompileExprBinary(ctx, op, ln, rn, cty);

//		BS2C_CompileExpr(ctx, ln, cty);
//		BS2C_CompileExpr(ctx, rn, cty);
//		BS2C_CompileBinaryOpSS(ctx, op, cty);

//		BS2C_CompileExpr(ctx, rn, cty);
//		BS2C_CompileConvType(ctx, lt);
		
		if(!BS2C_TypeVoidP(ctx, dty))
			{ BS2C_CompileDup(ctx); }
		
		BS2C_CompileStoreExpr(ctx, ln);
		
		if(!BS2C_TypeVoidP(ctx, dty))
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
		
		if(BS2C_TypeVariantP(ctx, lt))
		{
			if(BS2C_TypeSmallIntP(ctx, rt))
			{
//				BS2C_CompileConvType(ctx, BS2CC_TYZ_VARIANT);
				BS2C_CompileExpr(ctx, ln, BS2CC_TYZ_VARIANT);
				BS2C_CompileExpr(ctx, rn, BS2CC_TYZ_INT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDIXAI);
				BS2C_CompileExprPopType2(ctx);

				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
			
//			BS2C_CompileConvType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_CompileExpr(ctx, ln, BS2CC_TYZ_VARIANT);
			BS2C_CompileExpr(ctx, rn, BS2CC_TYZ_VARIANT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDIXAA);
			BS2C_CompileExprPopType2(ctx);

			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		if(dtvIsFixIntP(rn) || dtvIsFixUIntP(rn))
		{
//			li=dtvUnwrapLong(expr);
			li=dtvUnwrapLong(rn);

#if 1
			if(li==0)
			{
				o=-1;
				if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT))
					o=BSVM2_OP_LDDRI;
				if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG))
					o=BSVM2_OP_LDDRL;
				if(z==BSVM2_OPZ_FLOAT)
					o=BSVM2_OP_LDDRF;
				if(z==BSVM2_OPZ_DOUBLE)
					o=BSVM2_OP_LDDRD;
				if(z==BSVM2_OPZ_ADDRESS)
					o=BSVM2_OP_LDDRA;
				if(z==BSVM2_OPZ_SBYTE)
					o=BSVM2_OP_LDDRSB;
				if(z==BSVM2_OPZ_UBYTE)
					o=BSVM2_OP_LDDRUB;
				if(z==BSVM2_OPZ_SHORT)
					o=BSVM2_OP_LDDRSS;
				if(z==BSVM2_OPZ_USHORT)
					o=BSVM2_OP_LDDRUS;

				if(o>=0)
				{
					BS2C_CompileExpr(ctx, ln, lt);

					if(BS2C_TypePointerP(ctx, lt))
						BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

					BS2C_EmitOpcode(ctx, o);
//					BS2C_EmitOpcodeSCx(ctx, li);
					BS2C_CompileExprPopType1(ctx);
					BS2C_CompileExprPushType(ctx, bty);
					BS2C_CompileConvType(ctx, dty);
					return;
				}
			}
#endif

#if 1
			t0=BS2C_InferExprLocalIndex(ctx, ln);
//			if(t0>=0)
			if((t0>=0) && !BS2C_TypePointerP(ctx, lt))
			{
				if(BS2C_TypePointerP(ctx, lt))
					BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

				BS2C_EmitOpcode(ctx, BSVM2_OP_LDIXZLC);
				BS2C_EmitOpcodeUZx(ctx, z, t0);
				BS2C_EmitOpcodeSCx(ctx, li);
				BS2C_CompileExprPushType(ctx, bty);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
#endif

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
				BS2C_CompileExpr(ctx, ln, lt);

				if(BS2C_TypePointerP(ctx, lt))
					BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, bty);
				BS2C_EmitOpcodeSCx(ctx, li);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
		}
		
#if 1
		t0=BS2C_InferExprLocalIndex(ctx, ln);
		t1=BS2C_InferExprLocalIndex(ctx, rn);
//		if((t0>=0) && (t1>=0))
		if((t0>=0) && (t1>=0) &&
			!BS2C_TypePointerP(ctx, lt))
		{
			if(BS2C_TypePointerP(ctx, lt))
				BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

			BS2C_EmitOpcode(ctx, BSVM2_OP_LDIXZLL);
			BS2C_EmitOpcodeUZx(ctx, z, t0);
			BS2C_EmitOpcodeUCx(ctx, t1);
			BS2C_CompileExprPushType(ctx, bty);
			BS2C_CompileConvType(ctx, dty);
			return;
		}
#endif

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
			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileExpr(ctx, rn, rt);

			if(BS2C_TypePointerP(ctx, lt))
				BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);

			BS2C_EmitOpcode(ctx, o);
			BS2C_CompileExprPopType2(ctx);
			BS2C_CompileExprPushType(ctx, bty);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "postinc") || !strcmp(tag, "preinc"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		ln=BS2C_ReduceExpr(ctx, ln);

		lt=BS2C_InferExpr(ctx, ln);
		z=BS2C_GetTypeBaseZ(ctx, lt);

		if(BS2C_TypeVoidP(ctx, dty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			BS2C_TypeSmallIntP(ctx, lt))
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
		if(!BS2C_TypeVoidP(ctx, dty) && (u==0))
			{ BS2C_CompileDup(ctx); }

		if(BS2C_TypeSmallIntP(ctx, lt))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_INCI); }
		else if(BS2C_TypeSmallLongP(ctx, lt))
		{
			BS2C_CompileExprPushConstInt(ctx, 1, BS2CC_TYZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_ADDL);
		}
		else if(BS2C_TypeFloatP(ctx, lt))
		{
			BS2C_CompileExprPushConstInt(ctx, 1, BS2CC_TYZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_ADDF);
		}
		else if(BS2C_TypeDoubleP(ctx, lt))
		{
			BS2C_CompileExprPushConstInt(ctx, 1, BS2CC_TYZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_ADDD);
		}
		else if(BS2C_TypeArrayOrPointerP(ctx, lt))
		{
//			BS2C_CompileExprPushConstInt(ctx, 1, BS2CC_TYZ_DOUBLE);
//			BS2C_EmitOpcode(ctx, BSVM2_OP_ADDD);

			lt2=BS2C_TypeDerefType(ctx, lt);
			z2=BS2C_GetTypeBaseZ(ctx, lt2);
			
			o=-1;
			switch(z2)
			{
			case BSVM2_OPZ_INT: case BSVM2_OPZ_UINT:
				o=BSVM2_OP_LEARIC; break;
			case BSVM2_OPZ_LONG: case BSVM2_OPZ_ULONG:
				o=BSVM2_OP_LEARLC; break;
			case BSVM2_OPZ_FLOAT:
				o=BSVM2_OP_LEARFC; break;
			case BSVM2_OPZ_DOUBLE:
				o=BSVM2_OP_LEARDC; break;
			case BSVM2_OPZ_ADDRESS:
				o=BSVM2_OP_LEARAC; break;
			case BSVM2_OPZ_SBYTE: case BSVM2_OPZ_UBYTE:
				o=BSVM2_OP_LEARBC; break;
			case BSVM2_OPZ_SHORT: case BSVM2_OPZ_USHORT:
				o=BSVM2_OP_LEARBC; break;
			default:
				BS2C_CaseError(ctx);
			}
			
			if(BS2C_TypePointerP(ctx, lt))
				BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitOpcodeSCx(ctx, 1);
		}else
		{
			BS2C_CaseError(ctx);
		}

		if(!BS2C_TypeVoidP(ctx, dty) && (u==1))
			{ BS2C_CompileDup(ctx); }

		BS2C_CompileStoreExpr(ctx, ln);
		if(!BS2C_TypeVoidP(ctx, dty))
			{ BS2C_CompileConvType(ctx, dty); }
		return;
	}

	if(!strcmp(tag, "postdec") || !strcmp(tag, "predec"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		ln=BS2C_ReduceExpr(ctx, ln);

		lt=BS2C_InferExpr(ctx, ln);
		z=BS2C_GetTypeBaseZ(ctx, lt);

		if(BS2C_TypeVoidP(ctx, dty) &&
			BGBDT_TagStr_IsSymbolP(ln) &&
			BS2C_TypeSmallIntP(ctx, lt))
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
		if(!BS2C_TypeVoidP(ctx, dty) && (u==0))
			{ BS2C_CompileDup(ctx); }

		if(BS2C_TypeSmallIntP(ctx, lt))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_DECI); }
		else if(BS2C_TypeSmallLongP(ctx, lt))
		{
			BS2C_CompileExprPushConstInt(ctx, 1, BS2CC_TYZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_SUBL);
		}
		else if(BS2C_TypeFloatP(ctx, lt))
		{
			BS2C_CompileExprPushConstInt(ctx, 1, BS2CC_TYZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_SUBF);
		}
		else if(BS2C_TypeDoubleP(ctx, lt))
		{
			BS2C_CompileExprPushConstInt(ctx, 1, BS2CC_TYZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_SUBD);
		}
		else if(BS2C_TypeArrayOrPointerP(ctx, lt))
		{
//			BS2C_CompileExprPushConstInt(ctx, 1, BS2CC_TYZ_DOUBLE);
//			BS2C_EmitOpcode(ctx, BSVM2_OP_ADDD);

			lt2=BS2C_TypeDerefType(ctx, lt);
			z2=BS2C_GetTypeBaseZ(ctx, lt2);
			
			o=-1;
			switch(z2)
			{
			case BSVM2_OPZ_INT: case BSVM2_OPZ_UINT:
				o=BSVM2_OP_LEARIC; break;
			case BSVM2_OPZ_LONG: case BSVM2_OPZ_ULONG:
				o=BSVM2_OP_LEARLC; break;
			case BSVM2_OPZ_FLOAT:
				o=BSVM2_OP_LEARFC; break;
			case BSVM2_OPZ_DOUBLE:
				o=BSVM2_OP_LEARDC; break;
			case BSVM2_OPZ_ADDRESS:
				o=BSVM2_OP_LEARAC; break;
			case BSVM2_OPZ_SBYTE: case BSVM2_OPZ_UBYTE:
				o=BSVM2_OP_LEARBC; break;
			case BSVM2_OPZ_SHORT: case BSVM2_OPZ_USHORT:
				o=BSVM2_OP_LEARBC; break;
			default:
				BS2C_CaseError(ctx);
			}
			
			if(BS2C_TypePointerP(ctx, lt))
				BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitOpcodeSCx(ctx, 1);
		}else
		{
			BS2C_CaseError(ctx);
		}

		if(!BS2C_TypeVoidP(ctx, dty) && (u==1))
			{ BS2C_CompileDup(ctx); }

		BS2C_CompileStoreExpr(ctx, ln);
		if(!BS2C_TypeVoidP(ctx, dty))
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
					t0=BS2C_InferExprLocalIndex(ctx, ln);
					if(t0>=0)
					{
						BS2C_CompileExprPushType(ctx, vi2->bty);
						BS2C_EmitOpcode(ctx, BSVM2_OP_LDOSL);
						BS2C_EmitOpcodeJx(ctx, t0, i);
						BS2C_CompileConvType(ctx, dty);
						return;
					}
					
					if(BGBDT_TagStr_SymbolIsP(ln, "this"))
					{
						BS2C_CompileExprPushType(ctx, vi2->bty);
						BS2C_EmitOpcode(ctx, BSVM2_OP_LDTHIS);
						BS2C_EmitOpcodeUCx(ctx, i);
						BS2C_CompileConvType(ctx, dty);
						return;
					}

					BS2C_CompileExpr(ctx, ln, lt);

					BS2C_CompileExprPopType1(ctx);
					BS2C_CompileExprPushType(ctx, vi2->bty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_LDOS);
					BS2C_EmitOpcodeUCx(ctx, i);
					BS2C_CompileConvType(ctx, dty);
					return;
				}

				if(vi2->vitype==BS2CC_VITYPE_GBLVAR)
				{
					BS2C_CompileExprPushType(ctx, vi2->bty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_LDGS);
					BS2C_EmitOpcodeUCx(ctx, i);
					BS2C_CompileConvType(ctx, dty);
					return;
				}
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(BS2C_TypeSmallDoubleP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);
			z=BS2C_GetTypeBaseZ(ctx, lt);

			if(!strcmp(fn, "toString"))
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_CompileConvType(ctx, BS2CC_TYZ_STRING);
				return;
			}

			i=-1;
			if(!strcmp(fn, "sin"))
				i=BSVM2_OPMU_SIN;
			if(!strcmp(fn, "cos"))
				i=BSVM2_OPMU_COS;
			if(!strcmp(fn, "tan"))
				i=BSVM2_OPMU_TAN;
			if(!strcmp(fn, "sqrt"))
				i=BSVM2_OPUV_SQRT;
			if(!strcmp(fn, "rcp"))
				i=BSVM2_OPMU_RCP;
			if(!strcmp(fn, "atan"))
				i=BSVM2_OPMU_ATAN;
			if(!strcmp(fn, "sqr"))
				i=BSVM2_OPMU_SQR;
			if(!strcmp(fn, "ssqrt"))
				i=BSVM2_OPMU_SSQRT;
			if(!strcmp(fn, "abs"))
				i=BSVM2_OPMU_ABS;

			if((z>=0) && (i>=0))
			{
				BS2C_CompileExpr(ctx, ln, lt);

				BS2C_EmitOpcode(ctx, BSVM2_OP_MATHUFN);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(BS2C_TypeStringP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);
//			z=BS2C_GetTypeBaseZ(ctx, lt);

			rt=-1;
			if(!strcmp(fn, "toInt"))
				rt=BS2CC_TYZ_INT;
			if(!strcmp(fn, "toUInt"))
				rt=BS2CC_TYZ_UINT;
			if(!strcmp(fn, "toLong"))
				rt=BS2CC_TYZ_LONG;
			if(!strcmp(fn, "toULong"))
				rt=BS2CC_TYZ_ULONG;
			if(!strcmp(fn, "toFloat"))
				rt=BS2CC_TYZ_FLOAT;
			if(!strcmp(fn, "toDouble"))
				rt=BS2CC_TYZ_DOUBLE;

			if(rt>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_CompileConvType(ctx, rt);
				return;
			}

		}


		if(BS2C_TypeOpXvP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);
			rt=BS2C_TypeXvGetElemType(ctx, lt);
			z=BS2C_GetTypeVecZ(ctx, lt);

			j=BS2C_TypeXvGetElemCount(ctx, lt);

			i=-1;
			if(!strcmp(fn, "x"))i=0;
			if(!strcmp(fn, "y"))i=1;
			if(!strcmp(fn, "a"))i=0;
			if(!strcmp(fn, "b"))i=1;
			if(j>=3)
			{
				if(!strcmp(fn, "z"))i=2;
				if(!strcmp(fn, "c"))i=2;
			}
			if(j>=4)
			{
				if(!strcmp(fn, "w"))i=3;
				if(!strcmp(fn, "d"))i=3;
			}

			if(BS2C_TypeQuatfP(ctx, lt))
			{
				if(!strcmp(fn, "i"))i=0;
				if(!strcmp(fn, "j"))i=1;
				if(!strcmp(fn, "k"))i=2;
				if(!strcmp(fn, "l"))i=3;
				if(!strcmp(fn, "r"))i=3;
			}else if(BS2C_TypeOpXvCplxP(ctx, lt))
			{
				if(!strcmp(fn, "r"))i=0;
				if(!strcmp(fn, "i"))i=1;
			}

			if((i>=0) && (z==BSVM2_OPVZ_V2F))
			{
				BS2C_CompileExpr(ctx, ln, lt);

				o=BSVM2_OP_LDV2FA;
				if(i)o=BSVM2_OP_LDV2FB;
				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, rt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			if((i>=0) && (z==BSVM2_OPVZ_V2D))
			{
				BS2C_CompileExpr(ctx, ln, lt);

				o=BSVM2_OP_LDX2DA;
				if(i)o=BSVM2_OP_LDX2DB;
				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, rt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			if((i>=0) && ((z==BSVM2_OPVZ_V3F) ||
				(z==BSVM2_OPVZ_V4F)))
			{
				BS2C_CompileExpr(ctx, ln, lt);

				o=BSVM2_OP_LDX4FA+i;
				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, rt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			if((i>=0) && (z==BSVM2_OPVZ_V3XF))
			{
				BS2C_CompileExpr(ctx, ln, lt);

				o=BSVM2_OP_LDX3DA+i;
				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, rt);
				BS2C_EmitOpcode(ctx, o);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

#if 0
			if(i>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, rt);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDXX);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_CompileConvType(ctx, dty);
				return;
			}
#endif

			i=-1;
			if(!strcmp(fn, "neg"))
				i=BSVM2_OPUV_NEG;
			if(!strcmp(fn, "rcp"))
				i=BSVM2_OPUV_RCP;
			if(!strcmp(fn, "sqrt"))
				i=BSVM2_OPUV_SQRT;
			if(!strcmp(fn, "rsqrt"))
				i=BSVM2_OPUV_RSQRT;
			if(!strcmp(fn, "norm") || !strcmp(fn, "normalize"))
				i=BSVM2_OPUV_NORM;

			if(i>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);

				BS2C_EmitOpcode(ctx, BSVM2_OP_UNOPX);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			if(!strcmp(fn, "len") || !strcmp(fn, "length"))
				i=BSVM2_OPUV_LEN;
			if(!strcmp(fn, "len2") || !strcmp(fn, "length2"))
				i=BSVM2_OPUV_LEN2;
			if(i>=0)
			{
				BS2C_CompileExpr(ctx, ln, lt);

				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, rt);
				BS2C_EmitOpcode(ctx, BSVM2_OP_UNOPX);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(BS2C_TypeArrayP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);

			cty=BS2C_TypeDerefType(ctx, lt);
			z=BS2C_GetTypeBaseZ(ctx, cty);

			if(!strcmp(fn, "toString"))
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_CompileConvType(ctx, BS2CC_TYZ_STRING);
				return;
			}

			i=-1;
			if(!strcmp(fn, "length"))
				i=BSVM2_OPAG_ALEN;
			if(!strcmp(fn, "offset"))
				i=BSVM2_OPAG_AOFS;

			if((z>=0) && (i>=0))
			{
				BS2C_CompileExpr(ctx, ln, lt);
				BS2C_EmitOpcode(ctx, BSVM2_OP_AGETI);
				BS2C_EmitOpcodeUZx(ctx, z, i);

				BS2C_CompileExprPopType1(ctx);
				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_INT);

				BS2C_CompileConvType(ctx, dty);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(BS2C_TypeVariantP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);
//			i=BS2C_GetFrameDynamicSlotName(ctx, fn);
			i=BS2C_GetFrameSymbol(ctx, fn);

			t0=BS2C_InferExprLocalIndex(ctx, ln);
			if(t0>=0)
			{
				BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDOSL);
				BS2C_EmitOpcodeJx(ctx, t0, i);
				BS2C_CompileConvType(ctx, dty);
				return;
			}

			BS2C_CompileExpr(ctx, ln, lt);

			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDOS);
			BS2C_EmitOpcodeUCx(ctx, i);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "cast") ||
		!strcmp(tag, "cast_strict") ||
		!strcmp(tag, "prefix_cast"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		ln=BS2C_ReduceExpr(ctx, ln);

		lt=BS2C_InferExpr(ctx, ln);
		cty=BS2C_TypeExtType(ctx, rn);

		if(dtvIsSmallDoubleP(ln))
		{
//			if(BS2C_TypeSmallLongP(ctx, cty))
			if(BS2C_TypeSmallDoubleP(ctx, cty))
			{
				BS2C_CompileExpr(ctx, ln, cty);
				return;
			}
		}

		BS2C_CompileExpr(ctx, ln, lt);
		BS2C_CompileCastType(ctx, cty);
		return;
	}

	if(!strcmp(tag, "instanceof"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		ln=BS2C_ReduceExpr(ctx, ln);

		lt=BS2C_InferExpr(ctx, ln);
		cty=BS2C_TypeExtType(ctx, rn);

		if(!BS2C_TypeAddressP(ctx, lt))
		{
			if(BS2C_TypeSmallDoubleP(ctx, lt) &&
				BS2C_TypeSmallDoubleP(ctx, cty))
			{
				BS2C_CompileExprPushConstInt(ctx, lt==cty, BS2CC_TYZ_BOOL);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		vi=BS2C_GetTypeObject(ctx, cty);
		if(vi)
		{
			i=BS2C_IndexFrameGlobal(ctx, vi->gid);
			BS2C_CompileExpr(ctx, ln, lt);

			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_BOOL);
			BS2C_EmitOpcode(ctx, BSVM2_OP_ISTYPE);
			BS2C_EmitOpcodeUCx(ctx, i);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		fn=BS2C_GetTypeSig(ctx, cty);
		if(fn)
		{
			i=BS2C_ImgGetString(ctx, fn);
			j=BS2C_IndexFrameLiteral(ctx, (i<<4)|BSVM2_OPZY_STRU8);

			BS2C_CompileExpr(ctx, ln, lt);

			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_BOOL);
			BS2C_EmitOpcode(ctx, BSVM2_OP_ISTYPE);
			BS2C_EmitOpcodeUCx(ctx, j);
			BS2C_CompileConvType(ctx, dty);
			return;
		}

		BS2C_CaseError(ctx);

		return;
	}

	if(!strcmp(tag, "array"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

		if(dtvIsArrayP(ln))
			{ l=dtvArrayGetSize(ln); }
		else
			{ l=1; }

		cty=BS2CC_TYZ_VARIANT;
		
		if(BS2C_TypeArrayP(ctx, dty))
		{
			cty=BS2C_TypeDerefType(ctx, dty);
		}
		
		if(fn)
		{
			if(!strcmp(fn, "SI"))
				cty=BS2CC_TYZ_INT;
			if(!strcmp(fn, "UI"))
				cty=BS2CC_TYZ_UINT;
			if(!strcmp(fn, "SL"))
				cty=BS2CC_TYZ_LONG;
			if(!strcmp(fn, "UL"))
				cty=BS2CC_TYZ_ULONG;
			if(!strcmp(fn, "F"))
				cty=BS2CC_TYZ_FLOAT;
			if(!strcmp(fn, "D"))
				cty=BS2CC_TYZ_DOUBLE;
			if(!strcmp(fn, "SB"))
				cty=BS2CC_TYZ_SBYTE;
			if(!strcmp(fn, "UB"))
				cty=BS2CC_TYZ_UBYTE;
			if(!strcmp(fn, "SS"))
				cty=BS2CC_TYZ_SHORT;
			if(!strcmp(fn, "US"))
				cty=BS2CC_TYZ_USHORT;
		}
		
		z=BS2C_GetTypeBaseZ(ctx, cty);

#if 1
		switch(z)
		{
		case BS2CC_TYZ_INT:			o=BSVM2_OP_DSTIXIC; break;
		case BS2CC_TYZ_UINT:		o=BSVM2_OP_DSTIXIC; break;
		case BS2CC_TYZ_LONG:		o=BSVM2_OP_DSTIXLC; break;
		case BS2CC_TYZ_ULONG:		o=BSVM2_OP_DSTIXLC; break;
		case BS2CC_TYZ_FLOAT:		o=BSVM2_OP_DSTIXFC; break;
		case BS2CC_TYZ_DOUBLE:		o=BSVM2_OP_DSTIXDC; break;
		case BS2CC_TYZ_ADDRESS:		o=BSVM2_OP_DSTIXAC; break;
		case BS2CC_TYZ_SBYTE:		o=BSVM2_OP_DSTIXBC; break;
		case BS2CC_TYZ_UBYTE:		o=BSVM2_OP_DSTIXBC; break;
		case BS2CC_TYZ_SHORT:		o=BSVM2_OP_DSTIXSC; break;
		case BS2CC_TYZ_USHORT:		o=BSVM2_OP_DSTIXSC; break;
		default:					o=BSVM2_OP_DSTIXAC; break;
		}
#endif

		bty=cty|BS2CC_TYI_A1;
		BS2C_CompileExprPushType(ctx, bty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
		BS2C_EmitOpcodeUZx(ctx, z, l);

		for(i=0; i<l; i++)
		{
			if(dtvIsArrayP(ln))
				{ n0=dtvArrayGetIndexDtVal(ln, i); }
			else
				{ n0=ln; }

			if(dtvIsSmallLongP(n0) &&
				BS2C_TypeSmallLongP(ctx, cty))
			{
				li=dtvUnwrapLong(n0);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXNC);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_EmitOpcodeSuCx(ctx, z, li);
				continue;
			}

			if(dtvIsSmallDoubleP(n0) &&
				BS2C_TypeSmallDoubleP(ctx, cty))
			{
				f=dtvUnwrapDouble(n0);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXNC);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_EmitOpcodeSuCxD(ctx, z, f);
				continue;
			}

			BS2C_CompileExpr(ctx, n0, cty);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitOpcodeSCx(ctx, i);
			BS2C_CompileExprPopType1(ctx);
		}
		return;
	}

	if(!strcmp(tag, "vector"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

		if(dtvIsArrayP(ln))
			{ l=dtvArrayGetSize(ln); }
		else
			{ l=1; }

//		cty=BS2CC_TYZ_FLOAT;

		cty=-1;
		if(fn)
		{
			if(!strcmp(fn, "SI"))
				cty=BS2CC_TYZ_INT;
			if(!strcmp(fn, "UI"))
				cty=BS2CC_TYZ_UINT;
			if(!strcmp(fn, "SL"))
				cty=BS2CC_TYZ_LONG;
			if(!strcmp(fn, "UL"))
				cty=BS2CC_TYZ_ULONG;
			if(!strcmp(fn, "F"))
				cty=BS2CC_TYZ_FLOAT;
			if(!strcmp(fn, "D"))
				cty=BS2CC_TYZ_DOUBLE;
			if(!strcmp(fn, "SB"))
				cty=BS2CC_TYZ_SBYTE;
			if(!strcmp(fn, "UB"))
				cty=BS2CC_TYZ_UBYTE;
			if(!strcmp(fn, "SS"))
				cty=BS2CC_TYZ_SHORT;
			if(!strcmp(fn, "US"))
				cty=BS2CC_TYZ_USHORT;

			if(!strcmp(fn, "QF"))
				cty=BS2CC_TYZ_QUATF;
			if(!strcmp(fn, "JF"))
				cty=BS2CC_TYZ_FCPLX;
			if(!strcmp(fn, "JD"))
				cty=BS2CC_TYZ_DCPLX;

			if(!strcmp(fn, "XF"))
				cty=BS2CC_TYZ_VEC3XF;
		}else
		{
			if(BS2C_TypeOpXvP(ctx, dty))
				cty=dty;
		}

		bty=-1;

		if(cty<0)
		{
			if(l==2)
				{ bty=BS2CC_TYZ_VEC2D; cty=BS2CC_TYZ_DOUBLE; }
			if(l==3)
				{ bty=BS2CC_TYZ_VEC3XF; cty=BS2CC_TYZ_DOUBLE; }
			if(l==4)
				{ bty=BS2CC_TYZ_VEC4F; cty=BS2CC_TYZ_FLOAT; }

			if(cty<0)
				{ cty=BS2CC_TYZ_FLOAT; }
		}
		
		if(cty==BS2CC_TYZ_FLOAT)
		{
			if(l==2)
				bty=BS2CC_TYZ_VEC2F;
			if(l==3)
				bty=BS2CC_TYZ_VEC3F;
			if(l==4)
				bty=BS2CC_TYZ_VEC4F;
		}

		if(cty==BS2CC_TYZ_DOUBLE)
		{
			if(l==2)
				bty=BS2CC_TYZ_VEC2D;
			if(l==3)
				bty=BS2CC_TYZ_VEC3XF;
		}

		if(BS2C_TypeOpXvFloatP(ctx, cty))
			{ bty=cty; cty=BS2CC_TYZ_FLOAT; }
		if(BS2C_TypeOpXvDoubleP(ctx, cty))
			{ bty=cty; cty=BS2CC_TYZ_DOUBLE; }

		if(cty==BS2CC_TYZ_QUATF)
			{ bty=BS2CC_TYZ_QUATF; cty=BS2CC_TYZ_FLOAT; }
		if(cty==BS2CC_TYZ_FCPLX)
			{ bty=BS2CC_TYZ_FCPLX; cty=BS2CC_TYZ_FLOAT; }
		if(cty==BS2CC_TYZ_DCPLX)
			{ bty=BS2CC_TYZ_DCPLX; cty=BS2CC_TYZ_FLOAT; }

		if(cty==BS2CC_TYZ_VEC3XF)
			{ bty=BS2CC_TYZ_VEC3XF; cty=BS2CC_TYZ_DOUBLE; }

		if(bty<0)
		{
			BS2C_CaseError(ctx);
			return;
		}

		for(i=0; i<l; i++)
		{
			if(dtvIsArrayP(ln))
				{ n0=dtvArrayGetIndexDtVal(ln, i); }
			else
				{ n0=ln; }
//			n0=dtvArrayGetIndexDtVal(ln, i);
			BS2C_CompileExpr(ctx, n0, cty);
		}

		switch(bty)
		{
		case BS2CC_TYZ_VEC2F:
		case BS2CC_TYZ_FCPLX:
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKV2F);
			BS2C_CompileExprPopType2(ctx);
			break;
		case BS2CC_TYZ_VEC3F:
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX4F);
			BS2C_CompileExprPopType4(ctx);
			break;
		case BS2CC_TYZ_VEC4F:
		case BS2CC_TYZ_QUATF:
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX4F);
			BS2C_CompileExprPopType4(ctx);
			break;
		case BS2CC_TYZ_VEC2D:
		case BS2CC_TYZ_DCPLX:
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX2D);
			BS2C_CompileExprPopType2(ctx);
			break;

		case BS2CC_TYZ_VEC3XF:
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX3D);
			BS2C_CompileExprPopType3(ctx);
			break;
		}

		BS2C_CompileExprPushType(ctx, bty);
		BS2C_CompileConvType(ctx, dty);
		return;
	}

	if(!strcmp(tag, "imag"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

		lt=BS2CC_TYZ_FCPLX;
		if(!strcmp(fn, "JF"))
			lt=BS2CC_TYZ_FCPLX;
		if(!strcmp(fn, "JD"))
			lt=BS2CC_TYZ_DCPLX;

		switch(lt)
		{
		case BS2CC_TYZ_FCPLX:
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
			BS2C_CompileExpr(ctx, ln, BS2CC_TYZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKV2F);
			BS2C_CompileExprPopType1(ctx);
			break;
		case BS2CC_TYZ_DCPLX:
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
			BS2C_CompileExpr(ctx, ln, BS2CC_TYZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX2D);
			BS2C_CompileExprPopType1(ctx);
			break;
		}

		BS2C_CompileExprPushType(ctx, lt);
		return;
	}

	if(!strcmp(tag, "object"))
	{
#if 1
		vi=BS2C_GetTypeObject(ctx, dty);
		if(vi && ((vi->vitype==BS2CC_VITYPE_CLASS) ||
			(vi->vitype==BS2CC_VITYPE_STRUCT)))
		{
			i=BS2C_IndexFrameGlobal(ctx, vi->gid);

			BS2C_CompileExprPushType(ctx, dty);

			BS2C_EmitOpcode(ctx, BSVM2_OP_NEWOBJ);
			BS2C_EmitOpcodeUCx(ctx, i);

			ln=BS2P_GetAstNodeAttr(expr, "value");

			if(dtvTrueP(ln))
			{
				l=dtvArrayGetSize(ln);
				for(i=0; i<l; i++)
				{
					n0=dtvArrayGetIndexDtVal(ln, i);
					n1=BS2P_GetAstNodeAttr(n0, "value");
					fn=BS2P_GetAstNodeAttrS(n0, "name");

					vi2=BS2C_LookupObjectFieldName(ctx, vi, fn);
					
					if(!vi2)
					{
						BS2C_CompileError(ctx, BS2CC_ERRN_ERRNODECL);
						return;
					}
					
					j=BS2C_IndexFrameGlobal(ctx, vi2->gid);
					BS2C_CompileExpr(ctx, n1, vi2->bty);
					BS2C_EmitOpcode(ctx, BSVM2_OP_DSTOS);
					BS2C_EmitOpcodeUCx(ctx, j);
					BS2C_CompileExprPopType1(ctx);
				}
			}
			return;
		}
#endif

		ln=BS2P_GetAstNodeAttr(expr, "value");
//		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
		BS2C_EmitOpcode(ctx, BSVM2_OP_NEWDYO);
//		BS2C_EmitOpcodeUZx(ctx, z, l);

		if(dtvTrueP(ln))
		{
			l=dtvArrayGetSize(ln);
			for(i=0; i<l; i++)
			{
				n0=dtvArrayGetIndexDtVal(ln, i);
				n1=BS2P_GetAstNodeAttr(n0, "value");
				fn=BS2P_GetAstNodeAttrS(n0, "name");

				BS2C_CompileExpr(ctx, n1, BS2CC_TYZ_VARIANT);
				
//				j=BS2C_GetFrameDynamicSlotName(ctx, fn);
				j=BS2C_GetFrameSymbol(ctx, fn);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DSBOS);
				BS2C_EmitOpcodeUCx(ctx, j);

				BS2C_CompileExprPopType1(ctx);
			}
		}

		return;
	}

#if 1
	if(!strcmp(tag, "func_ind") ||
		!strcmp(tag, "func_dfl"))
	{
//		ctx->isinfer_varcapture=1;
//		ctx->frm->inf_ncapvar=0;

		i=BS2C_CompileTopFunc(ctx, expr);
		vi=ctx->globals[i];

		n0=BS2P_GetAstNodeAttr(expr, "body");
//		BS2C_InferCaptureStatement(ctx, n0);
		BS2C_InferCaptureFunc(ctx, vi);
//		ctx->isinfer_varcapture=0;

		if(vi->body->inf_ncapvar==0)
		{
//			BS2C_CompileTopFunc(ctx, expr);
			fn=BS2P_GetAstNodeAttrS(expr, "name");
			j=BS2C_LookupFrameGlobal(ctx, fn);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDGS);
			BS2C_EmitOpcodeUCx(ctx, j);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
			return;
		}

//		i=BS2C_CompileTopFunc(ctx, expr);
//		vi=ctx->globals[i];
		fn=vi->name;
		
		vi->niface=vi->body->inf_ncapvar;
		for(i=0; i<vi->niface; i++)
		{
			vi2=BS2C_AllocVarInfo(ctx);
			vi->iface[i]=vi2;
			vi2->name=BS2P_StrSym(ctx, vi->body->inf_capvar[i]);
//			vi2->bty=bty;
			
			k=BS2C_LookupLocal(ctx, vi2->name);
			vi3=ctx->frm->locals[k];
			vi2->bty=vi3->bty;
			vi2->bmfl=vi3->bmfl;
			vi2->typeExp=vi3->typeExp;
		}

//		fn=BS2P_GetAstNodeAttrS(expr, "name");
		j=BS2C_LookupFrameGlobal(ctx, fn);
		BS2C_EmitOpcode(ctx, BSVM2_OP_MKLFCN);
		BS2C_EmitOpcodeUCx(ctx, j);
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);

		for(i=0; i<vi->niface; i++)
		{
			vi2=vi->iface[i];
			n1=BGBDT_TagStr_Symbol(vi2->name);
//			t0=BS2C_LookupLocal(ctx, vi2->name);
			t0=BS2C_InferExprLocalIndex(ctx, n1);
			if(t0>=0)
			{
//				lt=BS2C_InferRetTypeName(ctx, vi2->name);
				z=BS2C_GetTypeBaseZ(ctx, vi2->bty);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXUZL);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_EmitOpcodeUCx(ctx, t0);
				continue;
			}

			lt=BS2C_InferExpr(ctx, n1);
			BS2C_CompileExpr(ctx, n1, lt);
			z=BS2C_GetTypeBaseZ(ctx, lt);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXUZ);
			BS2C_EmitOpcodeUZx(ctx, z, i);
			BS2C_CompileExprPopType1(ctx);
		}

		return;
	}
	
	if(!strcmp(tag, "func_aut"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "name");
		lt=BS2C_InferExpr(ctx, ln);
		BS2C_CompileExpr(ctx, ln, lt);
		return;
	}
#endif

	BS2C_CaseError(ctx);
	return;
}
