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


s64 BS2C_ReduceClampConstRangeL(BS2CC_CompileContext *ctx, s64 iv, int dty)
{
	s64 ov;
	switch(dty)
	{
	case BSVM2_OPZ_INT:
		ov=(s32)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_LONG:
		ov=(s64)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_UINT:
		ov=(u32)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_UBYTE:
		ov=(byte)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_SHORT:
		ov=(s16)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_SBYTE:
		ov=(sbyte)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_USHORT:
		ov=(u16)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;
	case BSVM2_OPZ_ULONG:
		ov=(u64)iv;
		if(iv!=ov)
			BS2C_WarnConstRange(ctx);
		break;

	default:
		BS2C_CaseError(ctx);
		break;
	}
	return(ov);
}

dtVal BS2C_ReduceExprVariConst(BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari)
{
	dtVal n0, n1, n2, n3;

	if(!vari)
		return(DTV_UNDEFINED);

	if((vari->vitype!=BS2CC_VITYPE_GBLVAR) &&
		(vari->vitype!=BS2CC_VITYPE_LCLVAR))
			return(DTV_UNDEFINED);

	if(!(vari->bmfl&BS2CC_TYFL_FINAL))
		return(DTV_UNDEFINED);

	n0=vari->initExp;
	if(!dtvTrueP(n0))
		return(DTV_UNDEFINED);
	
	n1=BS2C_ReduceExpr(ctx, n0);

	if(dtvIsFixIntP(n1))
		{ return(n1); }
	if(dtvIsFixUIntP(n1))
		{ return(n1); }
	if(dtvIsFixLongP(n1))
		{ return(n1); }
	if(dtvIsFixFloatP(n1))
		{ return(n1); }
	if(dtvIsFixDoubleP(n1))
		{ return(n1); }

	if(BGBDT_TagStr_IsStringP(n1))
		{ return(n1); }

	return(DTV_UNDEFINED);
}

dtVal BS2C_ReduceExprName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		return(BS2C_ReduceExprVariConst(ctx, vari));
	}
	
	i=BS2C_LookupLexical(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->func->iface[i];
		return(BS2C_ReduceExprVariConst(ctx, vari));
	}
	
//	if(ctx->frm->func && ctx->frm->func->obj &&
//		(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
	if(ctx->frm && ctx->frm->func && ctx->frm->func->obj)
	{
		vari=BS2C_LookupObjectFieldName(ctx,
			ctx->frm->func->obj, name);
//		if(vari)
//			{ return(vari->bty); }
		if(vari && (vari->vitype==BS2CC_VITYPE_STRVAR) &&
			(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
		{
			return(BS2C_ReduceExprVariConst(ctx, vari));
		}
		if(vari && (vari->vitype==BS2CC_VITYPE_GBLVAR))
		{
			return(BS2C_ReduceExprVariConst(ctx, vari));
		}
	}

	i=BS2C_LookupFrameGlobal(ctx, name);
	if(i>=0)
	{
		vari=BS2C_GetFrameGlobalInfo(ctx, i);
		return(BS2C_ReduceExprVariConst(ctx, vari));
	}
	
	i=BS2C_LookupGlobal(ctx, name);
	if(i>=0)
	{
		vari=ctx->globals[i];
		return(BS2C_ReduceExprVariConst(ctx, vari));
	}

	return(DTV_UNDEFINED);
}

dtVal BS2C_ReduceExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn, cn, tn;
	BGBDTC_X128 xa, xb, xc;
	s64 li, lj, lk;
	double lf, lg, lh;
	int lt, rt, cty;
	char *tag, *op, *fn;
	int i;

	if(dtvIsFixIntP(expr))
		{ return(expr); }
	if(dtvIsFixUIntP(expr))
		{ return(expr); }
	if(dtvIsFixLongP(expr))
		{ return(expr); }
	if(dtvIsFixFloatP(expr))
		{ return(expr); }
	if(dtvIsFixDoubleP(expr))
		{ return(expr); }

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		fn=BGBDT_TagStr_GetUtf8(expr);
		tn=BS2C_ReduceExprName(ctx, fn);
		if(!dtvUndefinedP(tn))
			return(tn);

		return(expr);
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
		{ return(expr); }

	if(!strcmp(tag, "binary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
//		cty=BS2C_InferSuperType(ctx, lt, rt);
		cty=BS2C_TypeBinarySuperType(ctx, op, lt, rt);

		if(BS2C_TypeSmallLongP(ctx, cty) &&
			dtvIsSmallLongP(ln) && dtvIsSmallLongP(rn))
		{
			li=dtvUnwrapLong(ln);
			lj=dtvUnwrapLong(rn);
			lk=0;
			if(!strcmp(op, "+"))lk=li+lj;
			else if(!strcmp(op, "-"))lk=li-lj;
			else if(!strcmp(op, "*"))lk=li*lj;
			else if(!strcmp(op, "&"))lk=li&lj;
			else if(!strcmp(op, "|"))lk=li|lj;
			else if(!strcmp(op, "^"))lk=li^lj;
			else if(!strcmp(op, "<<"))
				{ i=(u16)lj; lk=(i<64)?(li<<i):0; }
			else if(!strcmp(op, ">>"))
			{
				i=(u16)lj;
				if(BS2C_TypeUnsignedP(ctx, lt))
					{ lk=(i<64)?(((u64)li)>>i):0; }
				else
					{ lk=(i<64)?(li>>i):(li>>63); }
			}
			else if(!strcmp(op, ">>>"))
				{ i=(u16)lj; lk=(i<64)?(((u64)li)>>i):0; }
			else if(!strcmp(op, "/") || !strcmp(op, "%"))
			{
				if(lj==0)
				{
					BS2C_ErrDivZero(ctx);
					return(expr);
				}
				if(!strcmp(op, "/"))lk=li/lj;
				if(!strcmp(op, "%"))lk=li%lj;
			}else
			{
				return(expr);
			}
			
			lk=BS2C_ReduceClampConstRangeL(ctx, lk, cty);			
			if(BS2C_TypeSmallIntP(ctx, cty))
				{ tn=dtvWrapInt(lk); }
			else
				{ tn=dtvWrapLong(lk); }
			return(tn);
		}

		if(BS2C_TypeSmallDoubleP(ctx, cty) &&
			dtvIsSmallDoubleP(ln) && dtvIsSmallDoubleP(rn))
		{
			lf=dtvUnwrapDouble(ln);
			lg=dtvUnwrapDouble(rn);
			lh=0;
			if(!strcmp(op, "+"))lh=lf+lg;
			else if(!strcmp(op, "-"))lh=lf-lg;
			else if(!strcmp(op, "*"))lh=lf*lg;
			else if(!strcmp(op, "/"))
			{
				if(lg==0)
				{
					BS2C_ErrDivZero(ctx);
					return(expr);
				}
				lh=lf/lg;
			}else
			{
				return(expr);
			}
			
//			lk=BS2C_ReduceClampConstRangeL(ctx, lk, cty);			
			if(BS2C_TypeSmallFloatP(ctx, cty))
				{ tn=dtvWrapFloat(lh);}
			else
				{ tn=dtvWrapDouble(lh); }
			return(tn);
		}

		if(BS2C_TypeSmallInt128P(ctx, cty) &&
			BGBDT_XI128_IsSmallInt128P(ln) &&
			BGBDT_XI128_IsSmallInt128P(rn))
		{
//			li=dtvUnwrapLong(ln);
//			lj=dtvUnwrapLong(rn);
			xa=BGBDT_XI128_UnwrapInt128(ln);
			xb=BGBDT_XI128_UnwrapInt128(rn);

			lk=0;
			if(!strcmp(op, "+"))
				{ xc=BGBDT_XI128_AddX(xa, xb); }
			else if(!strcmp(op, "-"))
				{ xc=BGBDT_XI128_SubX(xa, xb); }
			else if(!strcmp(op, "*"))
				{ xc=BGBDT_XI128_MulX(xa, xb); }
			else if(!strcmp(op, "&"))
				{ xc=BGBDT_XI128_AndX(xa, xb); }
			else if(!strcmp(op, "|"))
				{ xc=BGBDT_XI128_OrX(xa, xb); }
			else if(!strcmp(op, "^"))
				{ xc=BGBDT_XI128_XorX(xa, xb); }
			else if(!strcmp(op, "<<"))
				{ xc=BGBDT_XI128_ShlX(xa, xb.a); }
			else if(!strcmp(op, ">>"))
			{
				if(BS2C_TypeUnsignedP(ctx, lt))
					{ xc=BGBDT_XI128_ShrX(xa, xb.a); }
				else
					{ xc=BGBDT_XI128_SarX(xa, xb.a); }
			}
			else if(!strcmp(op, ">>>"))
				{ xc=BGBDT_XI128_ShrX(xa, xb.a); }
			else if(!strcmp(op, "/") || !strcmp(op, "%"))
			{
				if(BGBDT_XI128_ZeroXP(xb))
				{
					BS2C_ErrDivZero(ctx);
					return(expr);
				}
				if(!strcmp(op, "/"))
					{ xc=BGBDT_XI128_DivX(xa, xb); }
				if(!strcmp(op, "%"))
					{ xc=BGBDT_XI128_ModX(xa, xb); }
			}else
			{
				return(expr);
			}
			
//			lk=BS2C_ReduceClampConstRangeL(ctx, lk, cty);			
//			if(BS2C_TypeSmallIntP(ctx, cty))
//				{ tn=dtvWrapInt(lk); }
//			else
//				{ tn=dtvWrapLong(lk); }
			tn=BGBDT_XI128_WrapInt128(xc);
			return(tn);
		}

		if(BS2C_TypeSmallFloat128P(ctx, cty) &&
			BGBDT_XF128_IsSmallFloat128P(ln) &&
			BGBDT_XF128_IsSmallFloat128P(rn))
		{
			xa=BGBDT_XF128_UnwrapFloat128(ln);
			xb=BGBDT_XF128_UnwrapFloat128(rn);

			if(!strcmp(op, "+"))
				{ xc=BGBDT_XF128_AddX(xa, xb); }
			else if(!strcmp(op, "-"))
				{ xc=BGBDT_XF128_SubX(xa, xb); }
			else if(!strcmp(op, "*"))
				{ xc=BGBDT_XF128_MulX(xa, xb); }
			else if(!strcmp(op, "/"))
			{
				if(BGBDT_XF128_ToDouble(xb)==0.0)
				{
					BS2C_ErrDivZero(ctx);
					return(expr);
				}
				xc=BGBDT_XF128_DivX(xa, xb);
			}else
			{
				return(expr);
			}
			
			tn=BGBDT_XF128_WrapFloat128(xc);
			return(tn);
		}

		return(expr);
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

		if(BS2C_TypeSmallLongP(ctx, cty) &&
			dtvIsSmallLongP(ln) && dtvIsSmallLongP(rn))
		{
			li=dtvUnwrapLong(ln);
			lj=dtvUnwrapLong(rn);
			if(!strcmp(op, "=="))i=(li==lj);
			else if(!strcmp(op, "!="))i=(li!=lj);
			else if(!strcmp(op, "<"))i=(li<lj);
			else if(!strcmp(op, ">"))i=(li>lj);
			else if(!strcmp(op, "<="))i=(li<=lj);
			else if(!strcmp(op, ">="))i=(li>=lj);
			else if(!strcmp(op, "==="))i=(li==lj);
			else if(!strcmp(op, "!=="))i=(li!=lj);
			else
			{
				return(expr);
			}
			
			tn=dtvWrapInt(i);
			return(tn);
		}

		if(BS2C_TypeSmallDoubleP(ctx, cty) &&
			dtvIsSmallDoubleP(ln) && dtvIsSmallDoubleP(rn))
		{
			lf=dtvUnwrapDouble(ln);
			lg=dtvUnwrapDouble(rn);
			if(!strcmp(op, "=="))i=(lf==lg);
			else if(!strcmp(op, "!="))i=(lf!=lg);
			else if(!strcmp(op, "<"))i=(lf<lg);
			else if(!strcmp(op, ">"))i=(lf>lg);
			else if(!strcmp(op, "<="))i=(lf<=lg);
			else if(!strcmp(op, ">="))i=(lf>=lg);
			{
				return(expr);
			}
			
			tn=dtvWrapInt(i);
			return(tn);
		}

		return(expr);
	}

	if(!strcmp(tag, "tern"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		cn=BS2P_GetAstNodeAttr(expr, "cond");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);
		cn=BS2C_ReduceExpr(ctx, cn);
		
		i=BS2C_CompileGetCondBool(ctx, cn);
		if(i==1)return(ln);
		if(i==0)return(rn);
		return(expr);
	}

	if(!strcmp(tag, "instanceof"))
	{
		return(expr);
	}

	if(!strcmp(tag, "cast") || !strcmp(tag, "cast_strict"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		ln=BS2C_ReduceExpr(ctx, ln);
//		rn=BS2C_ReduceExpr(ctx, rn);

		cty=BS2C_TypeBaseType(ctx, rn);

		if(BS2C_TypeSmallLongP(ctx, cty) && dtvIsSmallLongP(ln))
		{
			li=dtvUnwrapLong(ln);
			lk=BS2C_ReduceClampConstRangeL(ctx, li, cty);
			if(BS2C_TypeSmallIntP(ctx, cty))
				{ tn=dtvWrapInt(lk); }
			else
				{ tn=dtvWrapLong(lk); }
			return(tn);
		}

		return(expr);
	}

	return(expr);
}


dtVal BS2C_ReduceEvaluateExprAs(
	BS2CC_CompileContext *ctx, dtVal expr, int dty)
{
	int cty;
	dtVal n0, n1;
	dtVal ln, rn;
	dtVal ni, na;
	char *tag, *fn;
	int z, l2;
	int i, j, k, l;

	ni=BS2C_ReduceExpr(ctx, expr);
	if(!dtvTrueP(ni))
		return(DTV_UNDEFINED);

	if(dtvIsSmallLongP(ni) ||
		dtvIsSmallDoubleP(ni) ||
		BGBDT_TagStr_IsStringP(ni))
	{
		ni=BS2C_CompileVarInit_NormalizeValueType(ctx, ni, dty);
		return(ni);
	}

	if(BGBDT_TagStr_IsSymbolP(ni))
	{
		fn=BGBDT_TagStr_GetUtf8(expr);
		
		if(BS2C_TypeSmallLongP(ctx, dty))
		{
			if(!strcmp(fn, "false"))
				return(dtvWrapInt(0));
			if(!strcmp(fn, "true"))
				return(dtvWrapInt(1));
			if(!strcmp(fn, "null"))
				return(dtvWrapInt(0));
		}

//		tn=BS2C_ReduceExprName(ctx, fn);
//		if(!dtvUndefinedP(tn))
//			return(tn);

		return(DTV_UNDEFINED);
//		return(ni);
	}

	tag=BS2P_GetAstNodeTag(ni);
	if(!tag)
		{ return(DTV_UNDEFINED); }

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
			l2=BS2C_TypeGetArraySize(ctx, dty);
			if(!l2)l2=l;
		}else if(fn)
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

		na=dtvNewArray(l2, z);
		
		if(l2<l)
		{
			l=l2;
		}

		for(i=0; i<l; i++)
		{
			if(dtvIsArrayP(ln))
				{ n0=dtvArrayGetIndexDtVal(ln, i); }
			else
				{ n0=ln; }

			n1=BS2C_ReduceEvaluateExprAs(ctx, n0, cty);

			switch(z)
			{
			case BS2CC_TYZ_INT:
				dtvArraySetIndexInt(na, i, dtvUnwrapLong(n1));
				break;
			case BS2CC_TYZ_LONG:
				dtvArraySetIndexLong(na, i, dtvUnwrapLong(n1));
				break;
			case BS2CC_TYZ_FLOAT:
				dtvArraySetIndexFloat(na, i, dtvUnwrapDouble(n1));
				break;
			case BS2CC_TYZ_DOUBLE:
				dtvArraySetIndexDouble(na, i, dtvUnwrapDouble(n1));
				break;
			case BS2CC_TYZ_ADDRESS:
				dtvArraySetIndexDtVal(na, i, n1);
				break;
			case BS2CC_TYZ_SBYTE:
			case BS2CC_TYZ_UBYTE:
				dtvArraySetIndexByte(na, i, dtvUnwrapLong(n1));
				break;
			case BS2CC_TYZ_SHORT:
			case BS2CC_TYZ_USHORT:
				dtvArraySetIndexShort(na, i, dtvUnwrapLong(n1));
				break;
			default:
				BSVM2_DBGTRAP
				break;
			}
		}
		return(na);
	}

	return(DTV_UNDEFINED);
}
