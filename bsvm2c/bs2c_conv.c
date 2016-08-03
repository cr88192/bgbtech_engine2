#include <bteifgl.h>

void BS2C_CompileConvType(BS2CC_CompileContext *ctx, int dty)
{
	BS2C_CompileConvTypeI(ctx, dty, 1);
}

void BS2C_CompileCastType(BS2CC_CompileContext *ctx, int dty)
{
	BS2C_CompileConvTypeI(ctx, dty, 0);
}

void BS2C_CompileConvTypeI(BS2CC_CompileContext *ctx, int dty,
	int flag)
{
	int sty;
	int i;
	
	i=ctx->frm->stackpos-1;
	sty=ctx->frm->stack_bty[i];
	
	if(sty==dty)
		return;
	
	if(BS2C_TypeCompatibleP(ctx, dty, sty))
		return;

	if(BS2C_TypeArrayP(ctx, dty) && BS2C_TypeArrayP(ctx, sty))
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);
		return;
	}

	if((flag&1) && BS2C_TypeConvIsNarrowingP(ctx, dty, sty))
	{
		BS2C_WarnNarrowingConv(ctx, dty, sty);
	}

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

		if(BS2C_TypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2AA); return; }
		if(BS2C_TypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2AA); return; }

		if(BS2C_TypeAddressP(ctx, sty))
			{ return; }

		BS2C_CaseError(ctx);
		return;
	}

	if(BS2C_TypeAddressP(ctx, dty) && (sty==BS2CC_TYZ_VARIANT))
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);
		ctx->frm->stack_bty[i]=dty;
		return;
	}

	if(BS2C_TypePointerP(ctx, dty) &&
		BS2C_TypeAddressP(ctx, sty))
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);
		ctx->frm->stack_bty[i]=dty;
		BS2C_EmitOpcode(ctx, BSVM2_OP_CVTA2PTR);
		return;
	}

	if(BS2C_TypeAddressP(ctx, dty) &&
		BS2C_TypePointerP(ctx, sty))
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);
		ctx->frm->stack_bty[i]=dty;
		BS2C_EmitOpcode(ctx, BSVM2_OP_CVTPTR2A);
		return;
	}

	if(BS2C_TypeSmallDoubleP(ctx, dty) && (sty==BS2CC_TYZ_STRING))
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);
	
		ctx->frm->stack_bty[i]=BS2CC_TYZ_VARIANT;
		BS2C_EmitOpcode(ctx, BSVM2_OP_CVTST2AA);
		BS2C_CompileConvType(ctx, dty);
		return;
	}

	if((dty==BS2CC_TYZ_STRING) || (dty==BS2CC_TYZ_CSTRING))
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);

		ctx->frm->stack_bty[i]=dty;

//		if(sty==BS2CC_TYZ_VARIANT)
//			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2ST); return; }

		if(sty==BSVM2_OPZ_INT)
		{	BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2AA);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2ST); return; }
		if(sty==BSVM2_OPZ_LONG)
		{	BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2AA);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2ST); return; }
		if(sty==BSVM2_OPZ_FLOAT)
		{	BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2AA);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2ST); return; }
		if(sty==BSVM2_OPZ_DOUBLE)
		{	BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2AA);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2ST); return; }

		if(BS2C_TypeSmallIntP(ctx, sty))
		{	BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2AA);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2ST); return; }
		if(BS2C_TypeSmallLongP(ctx, sty))
		{	BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2AA);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2ST); return; }

		BS2C_CaseError(ctx);
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
		
		if(BS2C_TypeSmallIntP(ctx, sty))
			{ return; }
		if(BS2C_TypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I); return; }

		if(sty==BS2CC_TYZ_VARIANT)
		{
			if(flag&1)
				BS2C_WarnImplicitConv(ctx, dty, sty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
			return;
		}
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
		
		if(BS2C_TypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2L); return; }
		if(BS2C_TypeSmallLongP(ctx, sty))
			{ return; }

		if(sty==BS2CC_TYZ_VARIANT)
		{
			if(flag&1)
				BS2C_WarnImplicitConv(ctx, dty, sty);

			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2L);
			return;
		}
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

		if(BS2C_TypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2F); return; }
		if(BS2C_TypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2F); return; }

		if(sty==BS2CC_TYZ_HFLOAT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTHF2F); return; }

		if(sty==BS2CC_TYZ_VARIANT)
		{
			if(flag&1)
				BS2C_WarnImplicitConv(ctx, dty, sty);

			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2F);
			return;
		}
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

		if(BS2C_TypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2F); return; }
		if(BS2C_TypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2F); return; }

		if(sty==BS2CC_TYZ_HFLOAT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTHF2D); return; }

		if(sty==BS2CC_TYZ_VARIANT)
		{
			if(flag&1)
				BS2C_WarnImplicitConv(ctx, dty, sty);

			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2D);
			return;
		}
	}

	if(dty==BSVM2_OPZ_SBYTE)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_SBYTE;

		if(BS2C_TypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSB2I); return; }

		if(BS2C_TypeSmallLongP(ctx, sty))
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
			if(flag&1)
				BS2C_WarnImplicitConv(ctx, dty, sty);

			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSB2I);
			return;
		}
	}

	if(dty==BSVM2_OPZ_UBYTE)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_UBYTE;

		if(BS2C_TypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUB2I); return; }

		if(BS2C_TypeSmallLongP(ctx, sty))
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
			if(flag&1)
				BS2C_WarnImplicitConv(ctx, dty, sty);

			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUB2I);
			return;
		}
	}

	if(dty==BSVM2_OPZ_SHORT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_SHORT;

		if(BS2C_TypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSS2I); return; }

		if(BS2C_TypeSmallLongP(ctx, sty))
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
			if(flag&1)
				BS2C_WarnImplicitConv(ctx, dty, sty);

			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSS2I);
			return;
		}
	}

	if(dty==BSVM2_OPZ_USHORT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_USHORT;

		if(BS2C_TypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUS2I); return; }

		if(BS2C_TypeSmallLongP(ctx, sty))
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
			if(flag&1)
				BS2C_WarnImplicitConv(ctx, dty, sty);

			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTAA2I);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUS2I);
			return;
		}
	}

	if(dty==BS2CC_TYZ_BOOL)
	{
		ctx->frm->stack_bty[i]=BS2CC_TYZ_BOOL;

		if(BS2C_TypeSmallIntP(ctx, sty))
			return;
//			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUB2I); return; }

		BS2C_CompileConvType(ctx, BS2CC_TYZ_INT);
		ctx->frm->stack_bty[i]=BS2CC_TYZ_BOOL;
		return;
	}

	if(dty==BS2CC_TYZ_HFLOAT)
	{
		if(sty==BSVM2_OPZ_FLOAT)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2HF);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
			return;
		}

		if(sty==BSVM2_OPZ_DOUBLE)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2HF);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
			return;
		}

		if(BS2C_TypeSmallFloatP(ctx, sty))
		{
			BS2C_CompileConvType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2HF);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
			return;
		}

		if(BS2C_TypeSmallFloatP(ctx, sty))
		{
			BS2C_CompileConvType(ctx, BS2CC_TYZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2HF);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_HFLOAT);
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	if(dty==BS2CC_TYZ_FCPLX)
	{
		if(BS2C_TypeSmallDoubleP(ctx, sty))
		{
			BS2C_CompileConvType(ctx, BSVM2_OPZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKV2F);
			BS2C_CompileExprPopType2(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FCPLX);
			return;
		}

		if(BS2C_TypeDComplexP(ctx, sty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTX2D2V2F);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FCPLX);
			return;
		}

		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);

		if(sty==BS2CC_TYZ_VEC2F)
			{ return; }

		BS2C_CaseError(ctx);
		return;
	}

	if(dty==BS2CC_TYZ_DCPLX)
	{
		if(BS2C_TypeSmallDoubleP(ctx, sty))
		{
			BS2C_CompileConvType(ctx, BSVM2_OPZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX2D);
			BS2C_CompileExprPopType2(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_DCPLX);
			return;
		}
		
		if(BS2C_TypeFComplexP(ctx, sty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTV2F2X2D);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FCPLX);
			return;
		}

		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);

		if(sty==BS2CC_TYZ_VEC2D)
			{ return; }

		BS2C_CaseError(ctx);
		return;
	}

	if(dty==BS2CC_TYZ_QUATF)
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);

		if(sty==BS2CC_TYZ_VEC3F)
			{ return; }
		if(sty==BS2CC_TYZ_VEC4F)
			{ return; }

		if(BS2C_TypeSmallDoubleP(ctx, sty))
		{
			BS2C_CompileConvType(ctx, BSVM2_OPZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
			BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX4F);

			BS2C_EmitOpcode(ctx, BSVM2_OP_SHUFX4F);
			BS2C_EmitByte(ctx, BS2CC_V4SHUF(3, 3, 3, 0));

			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);

			BS2C_CompileExprPopType4(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_QUATF);
			return;
		}

		if(BS2C_TypeFComplexP(ctx, sty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTV2F2X4F);
			BS2C_EmitOpcode(ctx, BSVM2_OP_SHUFX4F);
			BS2C_EmitByte(ctx, BS2CC_V4SHUF(1, 2, 3, 0));

			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_QUATF);
			return;
		}

		if(BS2C_TypeDComplexP(ctx, sty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTX2D2V2F);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTV2F2X4F);

			BS2C_EmitOpcode(ctx, BSVM2_OP_SHUFX4F);
			BS2C_EmitByte(ctx, BS2CC_V4SHUF(1, 2, 3, 0));

			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_QUATF);
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	if(dty==BS2CC_TYZ_VEC2F)
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);

		if(sty==BS2CC_TYZ_FCPLX)
			{ return; }

		if((sty==BS2CC_TYZ_DCPLX) ||
			(sty==BS2CC_TYZ_VEC2D))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTX2D2V2F);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VEC2F);
			return;
		}

		if((dty==BS2CC_TYZ_VEC3F) ||
			(dty==BS2CC_TYZ_VEC4F) ||
			(dty==BS2CC_TYZ_QUATF))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTX4F2V2F);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VEC2F);
			return;
		}

		if(BS2C_TypeSmallDoubleP(ctx, sty))
		{
			BS2C_CompileConvType(ctx, BSVM2_OPZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKV2F);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_CompileExprPopType2(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VEC2F);
			return;
		}
	}

	if(dty==BS2CC_TYZ_VEC2D)
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);

		if(sty==BS2CC_TYZ_DCPLX)
			{ return; }

		if((sty==BS2CC_TYZ_FCPLX) ||
			(sty==BS2CC_TYZ_VEC2F))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTV2F2X2D);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VEC2D);
			return;
		}

		if(	(sty==BS2CC_TYZ_VEC3F)||
			(sty==BS2CC_TYZ_VEC4F)||
			(sty==BS2CC_TYZ_QUATF))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTX4F2V2F);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTV2F2X2D);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, dty);
			return;
		}

		if(BS2C_TypeSmallDoubleP(ctx, sty))
		{
			BS2C_CompileConvType(ctx, BSVM2_OPZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX2D);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_DOUBLE);
			BS2C_CompileExprPopType2(ctx);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VEC2D);
			return;
		}
	}

	if((dty==BS2CC_TYZ_VEC3F) ||
		(dty==BS2CC_TYZ_VEC4F))
	{
		if(flag&1)
			BS2C_WarnImplicitConv(ctx, dty, sty);

		if(sty==BS2CC_TYZ_VEC3F)
			{ return; }
		if(sty==BS2CC_TYZ_VEC4F)
			{ return; }
		if(sty==BS2CC_TYZ_QUATF)
			{ return; }

		if((sty==BS2CC_TYZ_FCPLX) ||
			(sty==BS2CC_TYZ_VEC2F))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTV2F2X4F);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, dty);
			return;
		}

		if((sty==BS2CC_TYZ_DCPLX) ||
			(sty==BS2CC_TYZ_VEC2D))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTX2D2V2F);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CVTV2F2X4F);
			BS2C_CompileExprPopType1(ctx);
			BS2C_CompileExprPushType(ctx, dty);
			return;
		}

		if(BS2C_TypeSmallDoubleP(ctx, sty))
		{
			BS2C_CompileConvType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF);
			BS2C_EmitOpcode(ctx, BSVM2_OP_MKX4F);

			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_FLOAT);
			BS2C_CompileExprPopType4(ctx);
			BS2C_CompileExprPushType(ctx, dty);
			return;
		}
	}

	BS2C_CaseError(ctx);
	return;
}
