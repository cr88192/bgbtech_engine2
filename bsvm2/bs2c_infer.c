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
		return(bty);
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

int BS2C_GetTypeBaseZ(BS2CC_CompileContext *ctx, int ty)
{
	if((ty>=0) && (ty<16))
		return(ty);
	if(ty>=256)
		return(BSVM2_OPZ_ADDRESS);
	
	if(ty==BSVM2_OPZ_VARIANT)
		return(BSVM2_OPZ_ADDRESS);
		
	return(BSVM2_OPZ_ADDRESS);
}

char *BS2C_GetTypeSig(BS2CC_CompileContext *ctx, int ty)
{
	char tb[256];
	BS2CC_VarInfo *vi;
	char *t;
	int al, asz, bty;
	int i, j, k;
	
	al =(ty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
	asz=(ty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
	bty=(ty>>BS2CC_TYE_SHR)&BS2CC_TYE_MASK2;
	
	t=tb;
	if(asz)
	{
		sprintf(t, "A%d", asz);
		t+=strlen(t);
	}
	
	if((al>=1) && (al<=4))
	{
		i=al;
		while(i--)
			*t++='Q';
		*t=0;
	}else if((al>=5) && (al<=7))
	{
		i=al-4;
		while(i--)
			*t++='P';
		*t=0;
	}
	
	if(bty<256)
	{
		switch(bty)
		{
		case BS2CC_TYZ_INT:		*t++='i'; break;
		case BS2CC_TYZ_LONG:	*t++='x'; break;
		case BS2CC_TYZ_FLOAT:	*t++='f'; break;
		case BS2CC_TYZ_DOUBLE:	*t++='d'; break;
		case BS2CC_TYZ_ADDRESS:	*t++='r'; break;
		case BS2CC_TYZ_UINT:	*t++='j'; break;
		case BS2CC_TYZ_UBYTE:	*t++='h'; break;
		case BS2CC_TYZ_SHORT:	*t++='s'; break;
		case BS2CC_TYZ_SBYTE:	*t++='a'; break;
		case BS2CC_TYZ_USHORT:	*t++='t'; break;
		case BS2CC_TYZ_ULONG:	*t++='y'; break;
		case BS2CC_TYZ_VOID:	*t++='v'; break;
		case BS2CC_TYZ_VARIANT:	*t++='r'; break;
		default:				*t++='r'; break;
		}
		*t=0;
	}else
	{
		vi=ctx->globals[bty-256];
		switch(vi->vitype)
		{
		case BS2CC_VITYPE_STRUCT:
			sprintf(t, "X%d", bty-256);
			break;
		case BS2CC_VITYPE_CLASS:
		case BS2CC_VITYPE_IFACE:
			sprintf(t, "L%d", bty-256);
			break;
		default:
			sprintf(t, "L%d", bty-256);
			break;
		}
	}
	return(frgl_rstrdup(tb));
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

int BS2C_InferTypeSignedP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BSVM2_OPZ_INT)
		return(1);
	if(ty==BSVM2_OPZ_SBYTE)
		return(1);
	if(ty==BSVM2_OPZ_SHORT)
		return(1);
	if(ty==BSVM2_OPZ_LONG)
		return(1);
	return(0);
}

int BS2C_InferTypeUnsignedP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BSVM2_OPZ_UINT)
		return(1);
	if(ty==BSVM2_OPZ_UBYTE)
		return(1);
	if(ty==BSVM2_OPZ_USHORT)
		return(1);
	if(ty==BSVM2_OPZ_ULONG)
		return(1);
	return(0);
}

int BS2C_InferTypeAddressP(BS2CC_CompileContext *ctx, int ty)
{
	int i;
	
	i=ty&BS2CC_TYE_MASK;
	if(i>=256)
		return(1);
	
	return(ty==BSVM2_OPZ_ADDRESS);
}

int BS2C_InferTypeArrayP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty&BS2CC_TYS_MASK)
		return(1);
	if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
		return(1);
	return(0);
}

int BS2C_InferTypeVoidP(BS2CC_CompileContext *ctx, int ty)
{
	return(ty==BSVM2_OPZ_VOID);
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

	return(BSVM2_OPZ_VARIANT);
}

int BS2C_TypeCompatibleP(
	BS2CC_CompileContext *ctx, int dty, int sty)
{
	BS2CC_VarInfo *lvi, *rvi;
	int lbt, rbt, lal, ral, lsz, rsz;
	int i, j, k, l;

	if(dty==sty)
		return(1);
	
	lbt=dty&BS2CC_TYE_MASK;
	rbt=sty&BS2CC_TYE_MASK;
	lal=(dty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
	ral=(sty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
	lsz=(dty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
	rsz=(sty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
	
	if((lal==1) || (lal==5))
	{
		if((ral!=1) && (ral!=5) && !rsz)
			return(0);
	}else if(lal!=0)
	{
		if(lal!=ral)
			return(0);
	}else
	{
		if(ral || rsz)
			return(0);
	}
	
	if((lbt>=256) && (rbt>=256))
	{
		lvi=ctx->globals[lvi-256];
		rvi=ctx->globals[rvi-256];
		
		if((lvi->vitype==BS2CC_VITYPE_GBLFUNC) &&
			(rvi->vitype==BS2CC_VITYPE_GBLFUNC))
		{
			if(!BS2C_TypeCompatibleP(ctx, lvi->rty, rvi->rty))
				return(0);
			if(lvi->nargs!=rvi->nargs)
				return(0);
			l=lvi->nargs;
			for(i=0; i<l; i++)
			{
				if(!BS2C_TypeCompatibleP(ctx, lvi->args[i], rvi->args[i]))
					return(0);
			}
			return(1);
		}
		return(0);
	}

	return(0);
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
			return(BSVM2_OPZ_UBYTE);
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
		if(!strcmp(tyn, "void"))
			return(BSVM2_OPZ_VOID);
	}
	return(BSVM2_OPZ_ADDRESS);
}

int BS2C_InferTypeRefinedType(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	dtVal expr)
{
	int asza[16];
	dtVal arrs;
	dtVal n0, n1;
	char *tyn;
	char *tag;
	int al, pl, asz, bt, ty;
	int i, j, k, l;

	arrs=BS2P_GetAstNodeAttr(expr, "arrays");
//	if(dtvTrueP(arrs))
//		return(BSVM2_OPZ_ADDRESS);

	if(dtvIsArrayP(arrs))
	{
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
	}else if(dtvTrueP(arrs))
	{
		tag=BS2P_GetAstNodeTag(arrs);
		
		if(tag && !strcmp(tag, "arrdef"))
		{
			al=0;
			asz=BS2P_GetAstNodeAttrI(arrs, "value");
		}else
		{
			al=1; asz=0;
		}
	}else
	{
		al=0; asz=0;
	}

	pl=BS2P_GetAstNodeAttrI(expr, "ptrlvl");
//	if(pl)
//		return(BSVM2_OPZ_ADDRESS);

	tyn=BS2P_GetAstNodeAttrS(expr, "name");
	
	if(tyn)
	{
		if(!strcmp(tyn, "void"))
			{ bt=BSVM2_OPZ_VOID; }
		else if(!strcmp(tyn, "int"))
			{ bt=BSVM2_OPZ_INT; }
		else if(!strcmp(tyn, "uint"))
			{ bt=BSVM2_OPZ_UINT; }
		else if(!strcmp(tyn, "long"))
			{ bt=BSVM2_OPZ_LONG; }
		else if(!strcmp(tyn, "ulong"))
			{ bt=BSVM2_OPZ_ULONG; }
		else if(!strcmp(tyn, "float"))
			{ bt=BSVM2_OPZ_FLOAT; }
		else if(!strcmp(tyn, "double"))
			{ bt=BSVM2_OPZ_DOUBLE; }
		else if(!strcmp(tyn, "byte"))
			{ bt=BSVM2_OPZ_UBYTE; }
		else if(!strcmp(tyn, "sbyte"))
			{ bt=BSVM2_OPZ_SBYTE; }
		else if(!strcmp(tyn, "short"))
			{ bt=BSVM2_OPZ_SHORT; }
		else if(!strcmp(tyn, "ushort"))
			{ bt=BSVM2_OPZ_USHORT; }
		else if(!strcmp(tyn, "var"))
			{ bt=BSVM2_OPZ_VARIANT; }
		else if(!strcmp(tyn, "variant"))
			{ bt=BSVM2_OPZ_VARIANT; }
		else
		{
			i=BS2C_LookupVariGlobal(ctx, vari, tyn);
			if(i>=0)
				bt=i+256;
		}
	}else
	{
		bt=BSVM2_OPZ_VARIANT;
	}
	
	ty=(bt<<BS2CC_TYE_SHR)|(al<<BS2CC_TYI_SHR)|
		(asz<<BS2CC_TYS_SHR);
	return(ty);
}

int BS2C_InferExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn;
	int lt, rt, ty;
	char *tag, *fn;
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

	BS2C_CaseError(ctx);
	return(-1);
}
