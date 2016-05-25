/*
Eval Expression
 */
 
#include <bteifgl.h>

dtVal BS2E_LoadName(BS2CC_CompileContext *ctx, char *name)
{
	BSVM2_ImageGlobal *gbl;
	dtVal v;

	gbl=BS2I_GlobalLookupGlobalVar(name);
	if(!gbl)
		return(DTV_UNDEFINED);
	v=BSVM2_Interp_GetGlobalDy(gbl);
	return(v);
}

dtVal BS2E_StoreName(BS2CC_CompileContext *ctx,
	char *name, dtVal val)
{
	BSVM2_ImageGlobal *gbl;
	dtVal v;

	gbl=BS2I_GlobalLookupGlobalVar(name);
	if(!gbl)
		return(DTV_UNDEFINED);
	BSVM2_Interp_SetGlobalDy(gbl, val);
	return(val);
}

dtVal BS2E_EvalExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn, cc, v;
	BS2CC_VarInfo *vi, *vi2;
	char *tag, *fn, *op;
	int lt, rt, ty;
	int i, j, k, l;

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

	if(dtvIsCharP(expr))
		{ return(expr); }

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		fn=BGBDT_TagStr_GetUtf8(expr);
		
		if(!strcmp(fn, "null"))
			{ return(DTV_NULL); }
		if(!strcmp(fn, "undefined"))
			{ return(DTV_UNDEFINED); }
		if(!strcmp(fn, "true"))
			{ return(DTV_TRUE); }
		if(!strcmp(fn, "false"))
			{ return(DTV_FALSE); }

		v=BS2E_LoadName(ctx, fn);
		return(v);
	}

	if(BGBDT_TagStr_IsStringP(expr))
	{
		return(expr);
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
		{ return(DTV_UNDEFINED); }
	
	if(!strcmp(tag, "binary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");
//		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_TypeBinarySuperType(ctx, op, lt, rt);

		ln=BS2E_EvalExpr(ctx, ln);
		rn=BS2E_EvalExpr(ctx, rn);
		
		if(!strcmp(op, "+"))
			return(BGBDT_TagAri_Add(ln, rn));
		if(!strcmp(op, "-"))
			return(BGBDT_TagAri_Sub(ln, rn));
		if(!strcmp(op, "*"))
			return(BGBDT_TagAri_Mul(ln, rn));
		if(!strcmp(op, "/"))
			return(BGBDT_TagAri_Div(ln, rn));
		if(!strcmp(op, "%"))
			return(BGBDT_TagAri_Mod(ln, rn));
		if(!strcmp(op, "&"))
			return(BGBDT_TagAri_And(ln, rn));
		if(!strcmp(op, "|"))
			return(BGBDT_TagAri_Or(ln, rn));
		if(!strcmp(op, "^"))
			return(BGBDT_TagAri_Xor(ln, rn));
		if(!strcmp(op, "<<"))
			return(BGBDT_TagAri_Shl(ln, rn));
		if(!strcmp(op, ">>"))
			return(BGBDT_TagAri_Sar(ln, rn));
		if(!strcmp(op, ">>>"))
			return(BGBDT_TagAri_Shr(ln, rn));

		return(DTV_UNDEFINED);
	}

	if(!strcmp(tag, "unary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		op=BS2P_GetAstNodeAttrS(expr, "op");
		ln=BS2E_EvalExpr(ctx, ln);

		if(!strcmp(op, "-"))
			return(BGBDT_TagAri_Neg(ln));
		if(!strcmp(op, "~"))
			return(BGBDT_TagAri_Not(ln));
		if(!strcmp(op, "!"))
			return(BGBDT_TagAri_LNot(ln));

		return(DTV_UNDEFINED);
	}

	if(!strcmp(tag, "bincmp"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2E_EvalExpr(ctx, ln);
		rn=BS2E_EvalExpr(ctx, rn);
		
		if(!strcmp(op, "=="))
			return(dtvWrapBool(BGBDT_TagAri_Compare(ln, rn)==0));
		if(!strcmp(op, "!="))
			return(dtvWrapBool(BGBDT_TagAri_Compare(ln, rn)!=0));
		if(!strcmp(op, "<"))
			return(dtvWrapBool(BGBDT_TagAri_Compare(ln, rn)<0));
		if(!strcmp(op, ">"))
			return(dtvWrapBool(BGBDT_TagAri_Compare(ln, rn)>0));
		if(!strcmp(op, "<="))
			return(dtvWrapBool(BGBDT_TagAri_Compare(ln, rn)<=0));
		if(!strcmp(op, ">="))
			return(dtvWrapBool(BGBDT_TagAri_Compare(ln, rn)>=0));

		return(DTV_UNDEFINED);
	}

	if(!strcmp(tag, "assign"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		fn=BGBDT_TagStr_GetUtf8(ln);
		v=BS2E_EvalExpr(ctx, rn);
		BS2E_StoreName(ctx, fn, v);
		return(v);
	}

	if(!strcmp(tag, "assignop"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");
//		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);

		fn=BGBDT_TagStr_GetUtf8(ln);

		ln=BS2E_EvalExpr(ctx, ln);
		rn=BS2E_EvalExpr(ctx, rn);

		if(!strcmp(op, "+"))
			{ v=BGBDT_TagAri_Add(ln, rn); }
		else if(!strcmp(op, "-"))
			{ v=BGBDT_TagAri_Sub(ln, rn); }
		else if(!strcmp(op, "*"))
			{ v=BGBDT_TagAri_Mul(ln, rn); }
		else if(!strcmp(op, "/"))
			{ v=BGBDT_TagAri_Div(ln, rn); }
		else if(!strcmp(op, "%"))
			{ v=BGBDT_TagAri_Mod(ln, rn); }
		else if(!strcmp(op, "&"))
			{ v=BGBDT_TagAri_And(ln, rn); }
		else if(!strcmp(op, "|"))
			{ v=BGBDT_TagAri_Or(ln, rn); }
		else if(!strcmp(op, "^"))
			{ v=BGBDT_TagAri_Xor(ln, rn); }
		else if(!strcmp(op, "<<"))
			{ v=BGBDT_TagAri_Shl(ln, rn); }
		else if(!strcmp(op, ">>"))
			{ v=BGBDT_TagAri_Sar(ln, rn); }
		else if(!strcmp(op, ">>>"))
			{ v=BGBDT_TagAri_Shr(ln, rn); }

		BS2E_StoreName(ctx, fn, v);
		return(v);
	}

	if(!strcmp(tag, "tern"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		cc=BS2P_GetAstNodeAttr(expr, "cond");
		
		cc=BS2E_EvalExpr(ctx, cc);
		if(dtvTrueP(cc))
			{ v=BS2E_EvalExpr(ctx, ln); }
		else
			{ v=BS2E_EvalExpr(ctx, rn); }
		
//		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);
//		ty=rt;
		return(v);
	}

	if(!strcmp(tag, "instanceof"))
	{
//		return(BS2CC_TYZ_INT);
	}

	if(!strcmp(tag, "cast") || !strcmp(tag, "cast_strict") ||
		!strcmp(tag, "prefix_cast"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		ln=BS2E_EvalExpr(ctx, ln);

		return(ln);
	}

	return(DTV_UNDEFINED);

#if 0
	if(!strcmp(tag, "call"))
	{
		fn=BS2P_GetAstNodeAttrS(expr, "func");
		if(fn)
		{
			ty=BS2C_InferRetTypeName(ctx, fn);
			return(ty);
		}

		BS2C_CaseError(ctx);
		return(-1);
	}

	if(!strcmp(tag, "new"))
	{
		rn=BS2P_GetAstNodeAttr(expr, "type");
//		ty=BS2C_TypeBaseType(ctx, rn);
		ty=BS2C_TypeExtType(ctx, rn);
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
		}

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
		
		ty=ty|BS2CC_TYI_A1;
		return(ty);
	}

	if(!strcmp(tag, "object"))
	{
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

	BS2C_CaseError(ctx);
	return(-1);
#endif
}
