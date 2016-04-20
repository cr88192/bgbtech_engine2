#include <bteifgl.h>

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

	BS2C_ErrNoDecl(ctx, name);
	return(-1);
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
		vari=BS2C_LookupObjectFieldName(ctx,
			ctx->frm->func->obj, name);
		if(vari)
			{ return(vari->rty); }
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

int BS2C_InferExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn;
	BS2CC_VarInfo *vi, *vi2;
	char *tag, *fn, *op;
	int lt, rt, ty;
	int i;

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
		return(BS2CC_TYZ_INT);
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

	if(!strcmp(tag, "instanceof"))
	{
		return(BS2CC_TYZ_INT);
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

	BS2C_CaseError(ctx);
	return(-1);
}