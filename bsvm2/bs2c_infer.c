#include <bteifgl.h>

int BS2C_InferName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=&(ctx->frm->locals[i]);
		bty=vari->bty;
		return(bty);
	}
}

int BS2C_InferTypeSmallIntP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BSVM2_OPZ_INT) || (ty==BSVM2_OPZ_UINT))
		return(1);
	if((ty==BSVM2_OPZ_SBYTE) || (ty==BSVM2_OPZ_UBYTE))
		return(1);
	if((ty==BSVM2_OPZ_SHORT) || (ty==BSVM2_OPZ_USHORT))
		return(1);
	return(0);
}

int BS2C_InferTypeSmallLongP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BSVM2_OPZ_LONG) || (ty==BSVM2_OPZ_ULONG))
		return(1);
	if(BS2C_InferTypeSmallIntP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_InferTypeSmallFloatP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BSVM2_OPZ_FLOAT)
		return(1);
	if(BS2C_InferTypeSmallIntP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_InferTypeSmallDoubleP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BSVM2_OPZ_FLOAT) || (ty==BSVM2_OPZ_DOUBLE))
		return(1);
	if(BS2C_InferTypeSmallLongP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_InferSuperType(
	BS2CC_CompileContext *ctx, int lty, int rty)
{
	if(lty==rty)
		return(lty);
	
	if(BS2C_InferTypeSmallIntP(ctx, lty) &&
		BS2C_InferTypeSmallIntP(ctx, rty))
	{
		return(BSVM2_OPZ_INT);
	}

	if(BS2C_InferTypeSmallLongP(ctx, lty) &&
		BS2C_InferTypeSmallLongP(ctx, rty))
	{
		return(BSVM2_OPZ_LONG);
	}

	if(BS2C_InferTypeSmallFloatP(ctx, lty) &&
		BS2C_InferTypeSmallFloatP(ctx, rty))
	{
		return(BSVM2_OPZ_FLOAT);
	}

	if(BS2C_InferTypeSmallDoubleP(ctx, lty) &&
		BS2C_InferTypeSmallDoubleP(ctx, rty))
	{
		return(BSVM2_OPZ_DOUBLE);
	}

}

int BS2C_InferTypeBaseType(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal arrs;
	char *tyn;
	int i;

	arrs=BS2P_GetAstNodeAttr(expr, "arrays");
	if(dtvTrueP(arrs))
		return(BSVM2_OPZ_ADDRESS);

	i=BS2P_GetAstNodeAttrI(expr, "ptrlvl");
	if(i)
		return(BSVM2_OPZ_ADDRESS);

	tyn=BS2P_GetAstNodeAttrS(expr, "name");
	
	if(tyn)
	{
		if(!strcmp(tyn, "int"))
			return(BSVM2_OPZ_INT);
		if(!strcmp(tyn, "uint"))
			return(BSVM2_OPZ_UINT);
		if(!strcmp(tyn, "long"))
			return(BSVM2_OPZ_LONG);
		if(!strcmp(tyn, "ulong"))
			return(BSVM2_OPZ_ULONG);
		if(!strcmp(tyn, "float"))
			return(BSVM2_OPZ_FLOAT);
		if(!strcmp(tyn, "double"))
			return(BSVM2_OPZ_DOUBLE);
		if(!strcmp(tyn, "byte"))
			return(BSVM2_OPZ_BYTE);
		if(!strcmp(tyn, "sbyte"))
			return(BSVM2_OPZ_SBYTE);
		if(!strcmp(tyn, "short"))
			return(BSVM2_OPZ_SHORT);
		if(!strcmp(tyn, "ushort"))
			return(BSVM2_OPZ_USHORT);
		if(!strcmp(tyn, "var"))
			return(BSVM2_OPZ_VARIANT);
		if(!strcmp(tyn, "variant"))
			return(BSVM2_OPZ_VARIANT);
	}
	return(BSVM2_OPZ_ADDRESS);
}

int BS2C_InferExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn;
	int lt, rt;
	char *tag;
	int i;

	if(dtvIsFixIntP(expr))
		{ return(BSVM2_OPZ_INT); }
	if(dtvIsFixUIntP(expr))
		{ return(BSVM2_OPZ_UINT); }
	if(dtvIsFixLongP(expr))
		{ return(BSVM2_OPZ_LONG); }
	if(dtvIsFixFloatP(expr))
		{ return(BSVM2_OPZ_FLOAT); }
	if(dtvIsFixDoubleP(expr))
		{ return(BSVM2_OPZ_DOUBLE); }

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
		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		ty=BS2C_InferSuperType(ctx, lt, rt);
		return(ty);
	}

	if(!strcmp(tag, "bincmp"))
	{
		return(BSVM2_OPZ_INT);
	}

	if(!strcmp(tag, "assign"))
	{
//		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
//		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);
		ty=rt;
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
		return(BSVM2_OPZ_INT);
	}

	if(!strcmp(tag, "cast") || !strcmp(tag, "cast_strict"))
	{
//		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
//		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);
//		ty=rt;
		ty=BS2C_InferTypeBaseType(ctx, rn);
		return(ty);
	}
}
