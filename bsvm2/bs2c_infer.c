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

int BS2C_GetTypeBaseZ(BS2CC_CompileContext *ctx, int ty)
{
	if((ty>=0) && (ty<16))
		return(ty);
	if(ty>=256)
		return(BSVM2_OPZ_ADDRESS);
	
	if(ty==BS2CC_TYZ_VARIANT)
		return(BSVM2_OPZ_ADDRESS);
	if(ty==BS2CC_TYZ_STRING)
		return(BSVM2_OPZ_ADDRESS);
	if(ty==BS2CC_TYZ_CHAR)
		return(BSVM2_OPZ_USHORT);
	if(ty==BS2CC_TYZ_CHAR8)
		return(BSVM2_OPZ_UBYTE);
	if(ty==BS2CC_TYZ_NLONG)
		return(BSVM2_OPZ_LONG);
	if(ty==BS2CC_TYZ_UNLONG)
		return(BSVM2_OPZ_ULONG);
	if(ty==BS2CC_TYZ_BOOL)
		return(BSVM2_OPZ_UBYTE);

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
		case BS2CC_TYZ_NLONG:	*t++='l'; break;
		case BS2CC_TYZ_UNLONG:	*t++='m'; break;
		case BS2CC_TYZ_VOID:	*t++='v'; break;
		case BS2CC_TYZ_VARIANT:	*t++='r'; break;
		case BS2CC_TYZ_VARARG:	*t++='z'; break;
		case BS2CC_TYZ_CHAR:	*t++='w'; break;
		case BS2CC_TYZ_CHAR8:	*t++='c'; break;
		case BS2CC_TYZ_STRING:	*t++='C'; *t++='s'; break;
		case BS2CC_TYZ_CSTRING:	*t++='P'; *t++='c'; break;
		case BS2CC_TYZ_BOOL:	*t++='b'; break;
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

BS2CC_VarInfo *BS2C_GetTypeObject(BS2CC_CompileContext *ctx, int ty)
{
	BS2CC_VarInfo *vi;
	int bty;

	if(ty<0)
		return(NULL);

	bty=(ty>>BS2CC_TYE_SHR)&BS2CC_TYE_MASK2;

	if(bty>=256)
	{
		vi=ctx->globals[bty-256];
		return(vi);
	}
	return(NULL);
}

BS2CC_VarInfo *BS2C_LookupObjectFieldName(BS2CC_CompileContext *ctx, 
	BS2CC_VarInfo *obj, char *name)
{
	BS2CC_VarInfo *vi;
	int i, j, k;
	
	if(!obj || !name)
		return(NULL);
	
	for(i=0; i<obj->nargs; i++)
	{
		vi=obj->args[i];
		if((vi->vitype!=BS2CC_VITYPE_STRVAR) &&
			(vi->vitype!=BS2CC_VITYPE_GBLVAR))
				continue;
//		if(vi->vitype!=BS2CC_VITYPE_STRVAR)
//				continue;
		if(vi->name && !strcmp(vi->name, name))
		{
			return(vi);
		}
	}
	return(NULL);
}

BS2CC_VarInfo *BS2C_LookupObjectFuncName(BS2CC_CompileContext *ctx, 
	BS2CC_VarInfo *obj, char *name)
{
	BS2CC_VarInfo *vi;
	int i, j, k;
	
	if(!obj || !name)
		return(NULL);
	
	for(i=0; i<obj->nargs; i++)
	{
		vi=obj->args[i];
//		if((vi->vitype!=BS2CC_VITYPE_STRFUNC) &&
//			(vi->vitype!=BS2CC_VITYPE_GBLFUNC))
//				continue;
		if(vi->vitype!=BS2CC_VITYPE_STRFUNC)
				continue;
		if(vi->name && !strcmp(vi->name, name))
		{
			return(vi);
		}
	}
	return(NULL);
}

BS2CC_VarInfo *BS2C_LookupObjectFuncNameArgs(
	BS2CC_CompileContext *ctx, 
	BS2CC_VarInfo *obj, char *name,
	int *aty, int na)
{
	BS2CC_VarInfo *vi;
	int i, j, k;
	
	if(!obj || !name)
		return(NULL);
	
	for(i=0; i<obj->nargs; i++)
	{
		vi=obj->args[i];
		if(vi->vitype!=BS2CC_VITYPE_STRFUNC)
			continue;
		if(!vi->name || strcmp(vi->name, name))
			continue;

		if(vi->nargs!=na)
			continue;
		for(j=0; j<na; j++)
		{
			if(!BS2C_TypeCompatibleP(ctx, vi->args[i]->bty, aty[i]))
				break;
		}
		if(j<na)
			continue;

		return(vi);
	}
	return(NULL);
}

int BS2C_InferTypeSmallIntP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BS2CC_TYZ_INT) || (ty==BS2CC_TYZ_UINT))
		return(1);
	if((ty==BS2CC_TYZ_SBYTE) || (ty==BS2CC_TYZ_UBYTE))
		return(1);
	if((ty==BS2CC_TYZ_SHORT) || (ty==BS2CC_TYZ_USHORT))
		return(1);
	if((ty==BS2CC_TYZ_CHAR) || (ty==BS2CC_TYZ_CHAR8))
		return(1);
	if(ty==BS2CC_TYZ_BOOL)
		return(1);
	return(0);
}

int BS2C_InferTypeSmallLongP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BS2CC_TYZ_LONG) || (ty==BS2CC_TYZ_ULONG))
		return(1);
	if((ty==BS2CC_TYZ_NLONG) || (ty==BS2CC_TYZ_UNLONG))
		return(1);
	if(BS2C_InferTypeSmallIntP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_InferTypeSmallFloatP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_FLOAT)
		return(1);
	if(BS2C_InferTypeSmallIntP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_InferTypeSmallDoubleP(BS2CC_CompileContext *ctx, int ty)
{
	if((ty==BS2CC_TYZ_FLOAT) || (ty==BS2CC_TYZ_DOUBLE))
		return(1);
	if(BS2C_InferTypeSmallLongP(ctx, ty))
		return(1);
	return(0);
}

int BS2C_InferTypeSignedP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_INT)
		return(1);
	if(ty==BS2CC_TYZ_SBYTE)
		return(1);
	if(ty==BS2CC_TYZ_SHORT)
		return(1);
	if(ty==BS2CC_TYZ_LONG)
		return(1);
	if(ty==BS2CC_TYZ_NLONG)
		return(1);
	return(0);
}

int BS2C_InferTypeUnsignedP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_UINT)
		return(1);
	if(ty==BS2CC_TYZ_UBYTE)
		return(1);
	if(ty==BS2CC_TYZ_USHORT)
		return(1);
	if(ty==BS2CC_TYZ_ULONG)
		return(1);
	if(ty==BS2CC_TYZ_UNLONG)
		return(1);
	if(ty==BS2CC_TYZ_CHAR)
		return(1);
	if(ty==BS2CC_TYZ_CHAR8)
		return(1);
	if(ty==BS2CC_TYZ_BOOL)
		return(1);
	return(0);
}

int BS2C_InferTypeAddressP(BS2CC_CompileContext *ctx, int ty)
{
	int i;
	
	i=ty&BS2CC_TYE_MASK;
	if(i>=256)
		return(1);
	
	return(ty==BS2CC_TYZ_ADDRESS);
}

int BS2C_InferTypeArrayP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty&BS2CC_TYS_MASK)
		return(1);
	if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
		return(1);
	return(0);
}

int BS2C_TypeFloatP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_FLOAT)
		return(1);
	return(0);
}

int BS2C_TypeDoubleP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty==BS2CC_TYZ_DOUBLE)
		return(1);
	return(0);
}

int BS2C_TypeArrayP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty&BS2CC_TYS_MASK)
		return(1);
	if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
		return(1);
	return(0);
}

int BS2C_TypeSizedArrayP(BS2CC_CompileContext *ctx, int ty)
{
	if(ty&BS2CC_TYS_MASK)
		return(1);
//	if((ty&BS2CC_TYI_MASK) && ((ty&BS2CC_TYI_MASK)<BS2CC_TYI_P1))
//		return(1);
	return(0);
}

int BS2C_InferTypeVoidP(BS2CC_CompileContext *ctx, int ty)
{
	return(ty==BS2CC_TYZ_VOID);
}

int BS2C_InferTypeVariantP(BS2CC_CompileContext *ctx, int ty)
{
	return(ty==BS2CC_TYZ_VARIANT);
}

int BS2C_TypeDerefType(BS2CC_CompileContext *ctx, int ty)
{
	int ty1;
	int al;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYS_MASK)
		{
			ty1=ty&(BS2CC_TYE_MASK|BS2CC_TYI_MASK);
			return(ty1);
		}
		
		al=(ty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;

		if(!al)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_ERRCANTDEREF);
			return(ty);
		}
		
		if((al==1) || (al==5))
		{
			ty1=ty&BS2CC_TYE_MASK;
			return(ty1);
		}
		
		ty1=ty-BS2CC_TYI_A1;
		return(ty1);
	}

	BS2C_CaseError(ctx);
	return(-1);
}

int BS2C_TypeGetArraySize(BS2CC_CompileContext *ctx, int ty)
{
	int ty1, sz;
	int al;

	if((ty&BS2CC_TYT_MASK)==BS2CC_TYT_BASIC)
	{
		if(ty&BS2CC_TYS_MASK)
		{
			sz=(ty>>BS2CC_TYS_SHR)&BS2CC_TYS_MASK2;
			return(sz);
		}
		
		al=(ty>>BS2CC_TYI_SHR)&BS2CC_TYI_MASK2;
		if(!al)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_ERRCANTDEREF);
			return(0);
		}
		return(0);
	}

	BS2C_CaseError(ctx);
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
		return(BS2CC_TYZ_INT);
	}

	if(BS2C_InferTypeSmallLongP(ctx, lty) &&
		BS2C_InferTypeSmallLongP(ctx, rty))
	{
		return(BS2CC_TYZ_LONG);
	}

	if(BS2C_InferTypeSmallFloatP(ctx, lty) &&
		BS2C_InferTypeSmallFloatP(ctx, rty))
	{
		return(BS2CC_TYZ_FLOAT);
	}

	if(BS2C_InferTypeSmallDoubleP(ctx, lty) &&
		BS2C_InferTypeSmallDoubleP(ctx, rty))
	{
		return(BS2CC_TYZ_DOUBLE);
	}
	
	if(BS2C_InferTypeVariantP(ctx, lty) ||
		BS2C_InferTypeVariantP(ctx, rty))
	{
		return(BS2CC_TYZ_VARIANT);
	}

	BS2C_CaseError(ctx);
	return(BS2CC_TYZ_VARIANT);
}

int BS2C_TypeAssignSuperType(
	BS2CC_CompileContext *ctx, int lty, int rty)
{
	int lty1, rty1;

	if(BS2C_TypeArrayP(ctx, lty) && BS2C_TypeArrayP(ctx, rty))
	{
		lty1=BS2C_TypeDerefType(ctx, lty);
		rty1=BS2C_TypeDerefType(ctx, rty);
		
		if(BS2C_TypeCompatibleP(ctx, lty1, rty1))
		{
			return(lty);
		}
	}
	
	lty1=BS2C_InferSuperType(ctx, lty, rty);
	return(lty1);
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
		if(lbt==rbt)
			return(1);
	
		lvi=ctx->globals[lbt-256];
		rvi=ctx->globals[rbt-256];
		
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
				if(!BS2C_TypeCompatibleP(ctx,
						lvi->args[i]->bty, rvi->args[i]->bty))
					return(0);
			}
			return(1);
		}
		return(0);
	}

	if(lbt==rbt)
		return(1);

	if((lbt==BS2CC_TYZ_CSTRING) && (rbt==BS2CC_TYZ_STRING))
		return(1);
	if((lbt==BS2CC_TYZ_STRING) && (rbt==BS2CC_TYZ_CSTRING))
		return(1);

	return(0);
}

int BS2C_InferTypeBaseType(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal arrs;
	char *tyn;
	int i;

	arrs=BS2P_GetAstNodeAttr(expr, "arrays");
	if(dtvTrueP(arrs))
		return(BS2CC_TYZ_ADDRESS);

	i=BS2P_GetAstNodeAttrI(expr, "ptrlvl");
	if(i)
		return(BS2CC_TYZ_ADDRESS);

	tyn=BS2P_GetAstNodeAttrS(expr, "name");
	
	if(tyn)
	{
		if(!strcmp(tyn, "int"))
			return(BS2CC_TYZ_INT);
		if(!strcmp(tyn, "uint"))
			return(BS2CC_TYZ_UINT);
		if(!strcmp(tyn, "long"))
			return(BS2CC_TYZ_LONG);
		if(!strcmp(tyn, "ulong"))
			return(BS2CC_TYZ_ULONG);
		if(!strcmp(tyn, "float"))
			return(BS2CC_TYZ_FLOAT);
		if(!strcmp(tyn, "double"))
			return(BS2CC_TYZ_DOUBLE);
		if(!strcmp(tyn, "byte"))
			return(BS2CC_TYZ_UBYTE);
		if(!strcmp(tyn, "sbyte"))
			return(BS2CC_TYZ_SBYTE);
		if(!strcmp(tyn, "short"))
			return(BS2CC_TYZ_SHORT);
		if(!strcmp(tyn, "ushort"))
			return(BS2CC_TYZ_USHORT);
		if(!strcmp(tyn, "nlong"))
			return(BS2CC_TYZ_NLONG);
		if(!strcmp(tyn, "unlong"))
			return(BS2CC_TYZ_UNLONG);
		if(!strcmp(tyn, "var"))
			return(BS2CC_TYZ_VARIANT);
		if(!strcmp(tyn, "variant"))
			return(BS2CC_TYZ_VARIANT);
		if(!strcmp(tyn, "void"))
			return(BS2CC_TYZ_VOID);
		if(!strcmp(tyn, "string"))
			return(BS2CC_TYZ_STRING);
		if(!strcmp(tyn, "cstring"))
			return(BS2CC_TYZ_CSTRING);
		if(!strcmp(tyn, "char"))
			return(BS2CC_TYZ_CHAR);
		if(!strcmp(tyn, "char8"))
			return(BS2CC_TYZ_CHAR8);
		if(!strcmp(tyn, "bool"))
			return(BS2CC_TYZ_BOOL);
	}
	return(BS2CC_TYZ_ADDRESS);
}

int BS2C_InferTypeExtType(BS2CC_CompileContext *ctx, dtVal expr)
{
	int cty;

	if(!ctx->frm || !ctx->frm->func)
	{
		cty=BS2C_InferTypeBaseType(ctx, expr);
		return(cty);
	}
	
	cty=BS2C_InferTypeRefinedType(ctx, ctx->frm->func, expr);
	return(cty);
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
//		return(BS2CC_TYZ_ADDRESS);

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
//		return(BS2CC_TYZ_ADDRESS);

	tyn=BS2P_GetAstNodeAttrS(expr, "name");
	
	if(tyn)
	{
		if(!strcmp(tyn, "void"))
			{ bt=BS2CC_TYZ_VOID; }
		else if(!strcmp(tyn, "int"))
			{ bt=BS2CC_TYZ_INT; }
		else if(!strcmp(tyn, "uint"))
			{ bt=BS2CC_TYZ_UINT; }
		else if(!strcmp(tyn, "long"))
			{ bt=BS2CC_TYZ_LONG; }
		else if(!strcmp(tyn, "ulong"))
			{ bt=BS2CC_TYZ_ULONG; }
		else if(!strcmp(tyn, "float"))
			{ bt=BS2CC_TYZ_FLOAT; }
		else if(!strcmp(tyn, "double"))
			{ bt=BS2CC_TYZ_DOUBLE; }
		else if(!strcmp(tyn, "byte"))
			{ bt=BS2CC_TYZ_UBYTE; }
		else if(!strcmp(tyn, "sbyte"))
			{ bt=BS2CC_TYZ_SBYTE; }
		else if(!strcmp(tyn, "short"))
			{ bt=BS2CC_TYZ_SHORT; }
		else if(!strcmp(tyn, "ushort"))
			{ bt=BS2CC_TYZ_USHORT; }
		else if(!strcmp(tyn, "nlong"))
			{ bt=BS2CC_TYZ_NLONG; }
		else if(!strcmp(tyn, "unlong"))
			{ bt=BS2CC_TYZ_UNLONG; }
		else if(!strcmp(tyn, "var"))
			{ bt=BS2CC_TYZ_VARIANT; }
		else if(!strcmp(tyn, "variant"))
			{ bt=BS2CC_TYZ_VARIANT; }
		else if(!strcmp(tyn, "string"))
			{ bt=BS2CC_TYZ_STRING; }
		else if(!strcmp(tyn, "cstring"))
			{ bt=BS2CC_TYZ_CSTRING; }
		else if(!strcmp(tyn, "char"))
			{ bt=BS2CC_TYZ_CHAR; }
		else if(!strcmp(tyn, "char8"))
			{ bt=BS2CC_TYZ_CHAR8; }
		else if(!strcmp(tyn, "bool"))
			{ bt=BS2CC_TYZ_BOOL; }
		else
		{
			i=BS2C_LookupVariGlobal(ctx, vari, tyn);
			if(i>=0)
				{ bt=i+256; }
			else
			{
				bt=BS2CC_TYZ_ADDRESS;
			}
		}
	}else
	{
		bt=BS2CC_TYZ_VARIANT;
	}
	
	ty=(bt<<BS2CC_TYE_SHR)|(al<<BS2CC_TYI_SHR)|
		(asz<<BS2CC_TYS_SHR);
	return(ty);
}

int BS2C_InferExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn;
	BS2CC_VarInfo *vi, *vi2;
	char *tag, *fn;
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
		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		ty=BS2C_InferSuperType(ctx, lt, rt);
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

	if(!strcmp(tag, "cast") || !strcmp(tag, "cast_strict"))
	{
//		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
//		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);
//		ty=rt;
//		ty=BS2C_InferTypeBaseType(ctx, rn);
		ty=BS2C_InferTypeExtType(ctx, rn);
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
//		ty=BS2C_InferTypeBaseType(ctx, rn);
		ty=BS2C_InferTypeExtType(ctx, rn);
		return(ty);
	}

	if(!strcmp(tag, "loadindex"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		lt=BS2C_InferExpr(ctx, ln);
//		ty=BS2C_InferTypeExtType(ctx, rn);
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

	BS2C_CaseError(ctx);
	return(-1);
}
