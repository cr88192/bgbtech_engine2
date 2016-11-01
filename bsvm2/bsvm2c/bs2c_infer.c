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

int BS2C_InferName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->bty;

		if(BS2C_TypeVarRefP(ctx, bty))
		{
			bty=BS2C_TypeDerefType(ctx, bty);
		}

		return(bty);
	}
	
	i=BS2C_LookupLexical(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->func->iface[i];
		bty=vari->bty;

		if(BS2C_TypeVarRefP(ctx, bty))
		{
			bty=BS2C_TypeDerefType(ctx, bty);
		}

		return(bty);
	}
	
//	if(ctx->frm->func && ctx->frm->func->obj &&
//		(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
	if(ctx->frm->func && ctx->frm->func->obj)
	{
		vari=BS2C_LookupObjectFieldName(ctx,
			ctx->frm->func->obj, name);
//		if(vari)
//			{ return(vari->bty); }
		if(vari && (vari->vitype==BS2CC_VITYPE_STRVAR) &&
			(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
				{ return(vari->bty); }
		if(vari && (vari->vitype==BS2CC_VITYPE_GBLVAR))
			{ return(vari->bty); }
	}

	i=BS2C_LookupFrameGlobal(ctx, name);
	if(i>=0)
	{
		vari=BS2C_GetFrameGlobalInfo(ctx, i);
		bty=vari->bty;
		return(bty);
	}
	
	if(ctx->frm->isinfer_varcapture)
	{
		for(i=0; i<ctx->frm->inf_ncapvar; i++)
			if(!strcmp(ctx->frm->inf_capvar[i], name))
				break;
		if(i<ctx->frm->inf_ncapvar)
			return(0);
		i=ctx->frm->inf_ncapvar++;
		ctx->frm->inf_capvar[i]=bgbdt_mm_strdup(name);
		return(0);
	}

	BS2C_ErrNoDecl(ctx, name);
	return(0);
//	return(-1);
}

int BS2C_InferRetTypeName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->rty;
		return(bty);
	}

	if(ctx->frm->func && ctx->frm->func->obj)
	{
//		vari=BS2C_LookupObjectFieldName(ctx,
//			ctx->frm->func->obj, name);
		vari=BS2C_LookupObjectFuncNameB(ctx,
			ctx->frm->func->obj, name);

		if(vari && (vari->vitype==BS2CC_VITYPE_STRFUNC) &&
			(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
				{ return(vari->rty); }

		if(vari && (vari->vitype==BS2CC_VITYPE_GBLFUNC))
			{ return(vari->rty); }
//		if(vari)
//			{ return(vari->rty); }
	}

	i=BS2C_LookupFrameGlobal(ctx, name);
	if(i>=0)
	{
		vari=BS2C_GetFrameGlobalInfo(ctx, i);
		bty=vari->rty;
		return(bty);
	}

	BS2C_ErrNoDecl(ctx, name);
	return(-1);
}

int BS2C_InferExprLocalIndex(BS2CC_CompileContext *ctx, dtVal expr)
{
	BS2CC_VarInfo *vari;
	char *tag, *fn, *op;
	int bty;
	int lt, rt, ty;
	int i;

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		fn=BGBDT_TagStr_GetUtf8(expr);
		i=BS2C_LookupLocal(ctx, fn);
		if(i<0)
			return(-1);

		vari=ctx->frm->locals[i];
		bty=vari->bty;

		if(vari->bmfl&BS2CC_TYFL_DYNAMIC)
			return(-1);

		if(BS2C_TypeVarRefP(ctx, bty))
			return(-1);
		
		return(i);
	}
	return(-1);
}

int BS2C_InferExprObjMethodCall(BS2CC_CompileContext *ctx,
	int clsty, char *name, dtVal args)
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
			return(-1);
		}
				
		bty=vari->bty;
		return(bty);
	}

	BS2C_CaseError(ctx);
	return(-1);
}


int BS2C_InferExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn, an, fnn;
	dtVal n0, n1;
	BS2CC_VarInfo *vi, *vi2;
	char *tag, *fn, *op;
	int lt, rt, ty;
	int i, j, k, l;

	if(dtvIsFixIntP(expr))
		{ return(BS2CC_TYZ_INT); }
	if(dtvIsFixUIntP(expr))
		{ return(BS2CC_TYZ_UINT); }
	if(dtvIsFixLongP(expr))
		{ return(BS2CC_TYZ_LONG); }
	if(dtvIsFixFloatP(expr))
		{ return(BS2CC_TYZ_FLOAT); }
	if(dtvIsFixDoubleP(expr))
		{ return(BS2CC_TYZ_DOUBLE); }

	if(BGBDT_XI128_IsInt128P(expr))
		{ return(BS2CC_TYZ_INT128); }
	if(BGBDT_XF128_IsFloat128P(expr))
		{ return(BS2CC_TYZ_FLOAT128); }

	if(dtvIsCharP(expr))
		{ return(BS2CC_TYZ_CHAR); }

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		fn=BGBDT_TagStr_GetUtf8(expr);
		
		if(!strcmp(fn, "null"))
			{ return(BS2CC_TYZ_ADDRESS); }
		if(!strcmp(fn, "undefined"))
			{ return(BS2CC_TYZ_ADDRESS); }
		if(!strcmp(fn, "true"))
			{ return(BS2CC_TYZ_BOOL); }
		if(!strcmp(fn, "false"))
			{ return(BS2CC_TYZ_BOOL); }

		if(!strcmp(fn, "this"))
		{
			if(ctx->frm->func && ctx->frm->func->obj &&
				(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
			{
				i=ctx->frm->func->obj->gid;
				ty=i+256;
				return(ty);
			}
		}
		
		i=BS2C_InferName(ctx, fn);
		return(i);
	}

	if(BGBDT_TagStr_IsStringP(expr))
	{
		return(BS2CC_TYZ_STRING);
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		return(BS2CC_TYZ_VARIANT);
	}
	
	if(!strcmp(tag, "binary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");
		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		ty=BS2C_TypeBinarySuperType(ctx, op, lt, rt);
		return(ty);
	}

	if(!strcmp(tag, "bincmp"))
	{
		return(BS2CC_TYZ_BOOL);
	}

	if(!strcmp(tag, "unary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		op=BS2P_GetAstNodeAttrS(expr, "op");
		lt=BS2C_InferExpr(ctx, ln);
		
		if(!strcmp(op, "!"))
			return(BS2CC_TYZ_BOOL);

		if(!strcmp(op, "*"))
		{
			ty=BS2C_TypeDerefType(ctx, lt);
			return(ty);
		}

		if(!strcmp(op, "&"))
		{
			ty=BS2C_TypeRefType(ctx, lt);
			return(ty);
		}
		
		ty=lt;
		return(ty);
	}

	if(!strcmp(tag, "assign"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
//		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);
		ty=lt;
//		ty=rt;
		return(ty);
	}

	if(!strcmp(tag, "assignop"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		ty=BS2C_InferSuperType(ctx, lt, rt);
//		ty=rt;
		return(ty);
	}

	if(!strcmp(tag, "tern"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		ty=BS2C_InferSuperType(ctx, lt, rt);
//		ty=rt;
		return(ty);
	}

	if(!strcmp(tag, "range"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		ty=BS2C_InferSuperType(ctx, lt, rt);
		return(ty);
	}

	if(!strcmp(tag, "instanceof"))
	{
		return(BS2CC_TYZ_BOOL);
	}

	if(!strcmp(tag, "cast") || !strcmp(tag, "cast_strict") ||
		!strcmp(tag, "prefix_cast"))
	{
//		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
//		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);
//		ty=rt;
//		ty=BS2C_TypeBaseType(ctx, rn);
		ty=BS2C_TypeExtType(ctx, rn);
		return(ty);
	}

	if(!strcmp(tag, "call"))
	{
		an=BS2P_GetAstNodeAttr(expr, "args");

//		fn=BS2P_GetAstNodeAttrS(expr, "func");
		fnn=BS2P_GetAstNodeAttr(expr, "func");
//		if(fn)
		if(BGBDT_TagStr_IsSymbolP(fnn))
		{
			fn=BGBDT_TagStr_GetUtf8(fnn);
			ty=BS2C_InferRetTypeName(ctx, fn);
			return(ty);
		}

		tag=BS2P_GetAstNodeTag(fnn);
		
		if(!tag)
		{
			BS2C_CaseError(ctx);
			return(-1);
		}

		if(!strcmp(tag, "loadslot"))
		{
			ln=BS2P_GetAstNodeAttr(fnn, "lhs");
			rn=BS2P_GetAstNodeAttr(fnn, "rhs");

			lt=BS2C_InferExpr(ctx, ln);
			if(BGBDT_TagStr_IsSymbolP(rn))
			{
				fn=BGBDT_TagStr_GetUtf8(rn);
				ty=BS2C_InferExprObjMethodCall(ctx, lt, fn, an);
				return(ty);
			}

			BS2C_CaseError(ctx);
			return(-1);
		}

		BS2C_CaseError(ctx);
		return(-1);
	}

	if(!strcmp(tag, "new"))
	{
		rn=BS2P_GetAstNodeAttr(expr, "type");
//		ty=BS2C_TypeBaseType(ctx, rn);
		ty=BS2C_TypeExtTypeNew(ctx, rn);
		return(ty);
	}

	if(!strcmp(tag, "loadindex"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		lt=BS2C_InferExpr(ctx, ln);
//		ty=BS2C_TypeExtType(ctx, rn);
		ty=BS2C_TypeDerefType(ctx, lt);
		return(ty);
	}

	if(!strcmp(tag, "postinc") || !strcmp(tag, "postdec") ||
		!strcmp(tag, "preinc") || !strcmp(tag, "predec"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		ty=BS2C_InferExpr(ctx, ln);
		return(ty);
	}

	if(!strcmp(tag, "loadslot"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
		
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
					return(-1);
				}
				
				return(vi2->bty);
			}

			BS2C_CaseError(ctx);
			return(-1);
		}

		if(BS2C_TypeSmallDoubleP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);
//			z=BS2C_GetTypeBaseZ(ctx, lt);

			if(!strcmp(fn, "toString"))
				return(BS2CC_TYZ_STRING);

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
			if(!strcmp(fn, "ssqr"))
				i=BSVM2_OPMU_SSQRT;
			if(!strcmp(fn, "abs"))
				i=BSVM2_OPMU_ABS;
				
			if(i>=0)
				return(lt);

			BS2C_CaseError(ctx);
			return(-1);
		}

		if(BS2C_TypeStringP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);

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
				return(rt);

			BS2C_CaseError(ctx);
			return(-1);
		}

		if(BS2C_TypeOpXvP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);
			rt=BS2C_TypeXvGetElemType(ctx, lt);
//			z=BS2C_GetTypeVecZ(ctx, lt);

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
			
			if(i>=0)
				{ return(rt); }

			ty=-1;
			if(!strcmp(fn, "neg"))
				ty=lt;
			if(!strcmp(fn, "rcp"))
				ty=lt;
			if(!strcmp(fn, "sqrt"))
				ty=lt;
			if(!strcmp(fn, "rsqrt"))
				ty=lt;
			if(!strcmp(fn, "norm"))
				ty=lt;

			if(!strcmp(fn, "len"))
				ty=rt;
			if(!strcmp(fn, "len2"))
				ty=rt;

			if(ty>=0)
				return(ty);

			BS2C_CaseError(ctx);
			return(-1);
		}

		if(BS2C_TypeArrayP(ctx, lt) && BGBDT_TagStr_IsSymbolP(rn))
		{
			fn=BGBDT_TagStr_GetUtf8(rn);

			i=-1;
			if(!strcmp(fn, "length"))i=0;
			if(!strcmp(fn, "offset"))i=1;

			if(i>=0)
			{
				return(BS2CC_TYZ_INT);
			}

			BS2C_CaseError(ctx);
			return(-1);
		}

		if(BS2C_TypeVariantP(ctx, lt))
			return(BS2CC_TYZ_VARIANT);

		BS2C_CaseError(ctx);
		return(-1);
	}

	if(!strcmp(tag, "array"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

//		l=dtvArrayGetSize(ln);

		ty=BS2CC_TYZ_VARIANT;
		
		if(fn)
		{
			if(!strcmp(fn, "SI"))
				ty=BS2CC_TYZ_INT;
			if(!strcmp(fn, "UI"))
				ty=BS2CC_TYZ_UINT;
			if(!strcmp(fn, "SL"))
				ty=BS2CC_TYZ_LONG;
			if(!strcmp(fn, "UL"))
				ty=BS2CC_TYZ_ULONG;
			if(!strcmp(fn, "F"))
				ty=BS2CC_TYZ_FLOAT;
			if(!strcmp(fn, "D"))
				ty=BS2CC_TYZ_DOUBLE;
			if(!strcmp(fn, "SB"))
				ty=BS2CC_TYZ_SBYTE;
			if(!strcmp(fn, "UB"))
				ty=BS2CC_TYZ_UBYTE;
			if(!strcmp(fn, "SS"))
				ty=BS2CC_TYZ_SHORT;
			if(!strcmp(fn, "US"))
				ty=BS2CC_TYZ_USHORT;
		}

		if(ctx->frm->isinfer_varcapture)
		{
			ln=BS2P_GetAstNodeAttr(expr, "value");

			if(dtvIsArrayP(ln))
			{
				l=dtvArrayGetSize(ln);
				for(i=0; i<l; i++)
				{
					n0=dtvArrayGetIndexDtVal(ln, i);
					n1=BS2P_GetAstNodeAttr(n0, "value");
					fn=BS2P_GetAstNodeAttrS(n0, "name");
					BS2C_InferExpr(ctx, n1);
				}
			}
		}

		ty=ty|BS2CC_TYI_A1;
		return(ty);
	}

	if(!strcmp(tag, "object"))
	{
		if(ctx->frm->isinfer_varcapture)
		{
			ln=BS2P_GetAstNodeAttr(expr, "value");

			if(dtvIsArrayP(ln))
			{
				l=dtvArrayGetSize(ln);
				for(i=0; i<l; i++)
				{
					n0=dtvArrayGetIndexDtVal(ln, i);
					n1=BS2P_GetAstNodeAttr(n0, "value");
					fn=BS2P_GetAstNodeAttrS(n0, "name");
					BS2C_InferExpr(ctx, n1);
				}
			}
		}
	
		return(BS2CC_TYZ_VARIANT);
	}

	if(!strcmp(tag, "imag"))
	{
		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

		lt=BS2CC_TYZ_FCPLX;
		if(!strcmp(fn, "JF"))
			lt=BS2CC_TYZ_FCPLX;
		if(!strcmp(fn, "JD"))
			lt=BS2CC_TYZ_DCPLX;
		return(lt);
	}

	if(!strcmp(tag, "vector"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

		l=dtvArrayGetSize(ln);

		lt=BS2CC_TYZ_FLOAT;
		
		if(fn)
		{
			if(!strcmp(fn, "SI"))
				lt=BS2CC_TYZ_INT;
			if(!strcmp(fn, "UI"))
				lt=BS2CC_TYZ_UINT;
			if(!strcmp(fn, "SL"))
				lt=BS2CC_TYZ_LONG;
			if(!strcmp(fn, "UL"))
				lt=BS2CC_TYZ_ULONG;
			if(!strcmp(fn, "F"))
				lt=BS2CC_TYZ_FLOAT;
			if(!strcmp(fn, "D"))
				lt=BS2CC_TYZ_DOUBLE;
			if(!strcmp(fn, "SB"))
				lt=BS2CC_TYZ_SBYTE;
			if(!strcmp(fn, "UB"))
				lt=BS2CC_TYZ_UBYTE;
			if(!strcmp(fn, "SS"))
				lt=BS2CC_TYZ_SHORT;
			if(!strcmp(fn, "US"))
				lt=BS2CC_TYZ_USHORT;

			if(!strcmp(fn, "QF"))
				lt=BS2CC_TYZ_QUATF;
			if(!strcmp(fn, "JF"))
				lt=BS2CC_TYZ_FCPLX;
			if(!strcmp(fn, "JD"))
				lt=BS2CC_TYZ_DCPLX;
		}
		
		ty=-1;
		if(lt==BS2CC_TYZ_FLOAT)
		{
			if(l==2)
				ty=BS2CC_TYZ_VEC2F;
			if(l==3)
				ty=BS2CC_TYZ_VEC3F;
			if(l==4)
				ty=BS2CC_TYZ_VEC4F;
		}

		if(lt==BS2CC_TYZ_DOUBLE)
		{
			if(l==2)
				ty=BS2CC_TYZ_VEC2D;
		}

		if(lt==BS2CC_TYZ_QUATF)
			{ ty=BS2CC_TYZ_QUATF; }
		if(lt==BS2CC_TYZ_FCPLX)
			{ ty=BS2CC_TYZ_FCPLX; }
		if(lt==BS2CC_TYZ_DCPLX)
			{ ty=BS2CC_TYZ_DCPLX; }

		if(ty>=0)
			return(ty);

		BS2C_CaseError(ctx);
		return(-1);
	}

	if(!strcmp(tag, "func_dfl") ||
		!strcmp(tag, "func_aut") ||
		!strcmp(tag, "func_ind"))
	{
		return(BS2CC_TYZ_VARIANT);
	}

	BS2C_CaseError(ctx);
	return(-1);
}

void BS2C_InferCaptureStatement(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal n0, n1, n2, n3;
	char *tag, *fn;
	int ln;
	int i, j, k, l;

	if(ctx->ncfatal)
		return;

	if(dtvIsArrayP(expr))
	{
		l=dtvArrayGetSize(expr);
		for(i=0; i<l; i++)
		{
			n0=dtvArrayGetIndexDtVal(expr, i);
			BS2C_InferCaptureStatement(ctx, n0);
			if(ctx->ncfatal)
				break;
		}
		return;
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		BS2C_CaseError(ctx);
		return;
	}

	fn=BS2P_GetAstNodeAttrS(expr, "fn");
	ln=BS2P_GetAstNodeAttrI(expr, "ln");
	if(fn)ctx->srcfn=fn;
	if(ln>0)ctx->srcln=ln;

	if(!strcmp(tag, "block"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_InferCaptureStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "var"))
	{
//		BS2C_CompileFunVar(ctx, expr);
		return;
	}
	
	if(!strcmp(tag, "vars"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		l=dtvArrayGetSize(n0);
		for(i=0; i<l; i++)
		{
			n1=dtvArrayGetIndexDtVal(n0, i);
//			BS2C_CompileFunVar(ctx, n1);
		}
		return;
	}

	if(!strcmp(tag, "func"))
	{
		BS2C_CompileFunVar(ctx, expr);
		return;
	}

	if(!strcmp(tag, "for"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "init");
		n1=BS2P_GetAstNodeAttr(expr, "cond");
		n2=BS2P_GetAstNodeAttr(expr, "step");
		BS2C_InferExpr(ctx, n0);
		BS2C_InferExpr(ctx, n1);
		BS2C_InferExpr(ctx, n2);

		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_InferCaptureStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "if"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "cond");
		BS2C_InferExpr(ctx, n2);

		n0=BS2P_GetAstNodeAttr(expr, "then");
		n1=BS2P_GetAstNodeAttr(expr, "else");
		BS2C_InferCaptureStatement(ctx, n0);
//		if(!dtvNullP(n1))
		if(dtvTrueP(n1))
			BS2C_InferCaptureStatement(ctx, n1);
		return;
	}

	if(!strcmp(tag, "ifelse"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "cond");
		BS2C_InferExpr(ctx, n2);

		n0=BS2P_GetAstNodeAttr(expr, "then");
		n1=BS2P_GetAstNodeAttr(expr, "else");
		BS2C_InferCaptureStatement(ctx, n0);
//		if(!dtvNullP(n1))
		if(dtvTrueP(n1))
			BS2C_InferCaptureStatement(ctx, n1);
		return;
	}

	if(!strcmp(tag, "while"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "cond");
		BS2C_InferExpr(ctx, n2);

		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_InferCaptureStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "do_while"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "cond");
		BS2C_InferExpr(ctx, n2);

		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_InferCaptureStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "switch"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "cond");
		BS2C_InferExpr(ctx, n2);

		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_InferCaptureStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "assign"))
	{
		BS2C_InferExpr(ctx, expr);
		return;
	}
	if(!strcmp(tag, "assignop"))
	{
		BS2C_InferExpr(ctx, expr);
		return;
	}
	if(!strcmp(tag, "break"))
		return;
	if(!strcmp(tag, "call"))
	{
		BS2C_InferExpr(ctx, expr);
		return;
	}
	if(!strcmp(tag, "case"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_InferExpr(ctx, n2);
		return;
	}
	if(!strcmp(tag, "case_default"))
		return;
	if(!strcmp(tag, "continue"))
		return;
	if(!strcmp(tag, "default"))
		return;
	if(!strcmp(tag, "delete"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_InferExpr(ctx, n2);
		return;
	}
	if(!strcmp(tag, "goto"))
		return;
	if(!strcmp(tag, "label"))
		return;

	if(!strcmp(tag, "postdec"))
	{
		BS2C_InferExpr(ctx, expr);
		return;
	}
	if(!strcmp(tag, "postinc"))
	{
		BS2C_InferExpr(ctx, expr);
		return;
	}
	if(!strcmp(tag, "predec"))
	{
		BS2C_InferExpr(ctx, expr);
		return;
	}
	if(!strcmp(tag, "preinc"))
	{
		BS2C_InferExpr(ctx, expr);
		return;
	}

	if(!strcmp(tag, "return"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_InferExpr(ctx, n2);
		return;
	}
	if(!strcmp(tag, "throw"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_InferExpr(ctx, n2);
		return;
	}

	if(!strcmp(tag, "tail"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_InferExpr(ctx, n2);
		return;
	}

	if(!strcmp(tag, "empty_block"))
		return;

	BS2C_CaseError(ctx);
}

void BS2C_InferCaptureFunc(BS2CC_CompileContext *ctx, BS2CC_VarInfo *func)
{
	BS2CC_CcFrame *frm;
	int tk;
	int i, j, k, l;

	frm=BS2C_AllocCcFrame(ctx);

	tk=func->tokcnt;
	frm->def_rlcty=0;
	if(tk>=32)frm->def_rlcty=1;
	if(tk>=4096)frm->def_rlcty=2;
	if(tk>=(1<<20))frm->def_rlcty=3;

	i=ctx->frmstackpos++;
	ctx->frmstack[i]=ctx->frm;
	ctx->frm=frm;
	func->body=frm;
	frm->func=func;
	frm->ctx=ctx;

	frm->isinfer_varcapture=1;
	BS2C_CompileFunVarStatement(ctx,
		func->bodyExp);
	
	frm->bargs=frm->nlocals;
	for(j=0; j<func->nargs; j++)
	{
		k=frm->nlocals++;
		frm->locals[k]=func->args[j];
	}

	BS2C_InferCaptureStatement(ctx, func->bodyExp);

	frm->isinfer_varcapture=0;

#if 0
	BS2C_CompileStatement(ctx,
		func->bodyExp);
	
	if(!ctx->frm->wasret)
	{
		if(ctx->frm->jcleanup>0)
			{ BS2C_EmitReturnCleanupV(ctx, 1); }
		else
			{ BS2C_EmitReturnV(ctx); }
	}
	
	if(ctx->frm->jcleanup>0)
	{
		BS2C_CompileFuncBodyCleanup(ctx);
	}
	
	if(ctx->frm->stackpos!=0)
	{
		BS2C_ErrStackMisalign(ctx);
	}
	
	BS2C_FixupLabels(ctx);
	
	frm->szt=frm->ct-frm->cts;
	
//	printf("Avg B/tk=%f\n", frm->szt/(tk+0.00001));
#endif
	
	ctx->frm=ctx->frmstack[i];
	ctx->frmstackpos=i;
}
