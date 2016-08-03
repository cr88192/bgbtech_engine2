#include <bteifgl.h>

int BS2C_LookupLocal(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int i, j;
	
	for(i=0; i<ctx->frm->nlocals; i++)
	{
		vari=ctx->frm->locals[i];
		if(!strcmp(vari->name, name))
			return(i);
	}
	return(-1);
}

int BS2C_LookupGlobal(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int i, j;
	
	for(i=0; i<ctx->nglobals; i++)
	{
		vari=ctx->globals[i];
		if(!vari)
			continue;

		if(!vari->qname)
			continue;
		
//		if(!strcmp(vari->name, name))
		if(!strcmp(vari->qname, name))
			return(i);
	}
	return(-1);
}

int BS2C_IndexFrameGlobal(BS2CC_CompileContext *ctx, int gix)
{
	return(BS2C_IndexFrameLiteral(ctx, gix<<2));
}

int BS2C_IndexFrameLiteral(BS2CC_CompileContext *ctx, int gix)
{
	BS2CC_VarInfo *vari;
	int i, j;
	
	for(i=0; i<ctx->frm->ngbl; i++)
	{
		j=ctx->frm->gbltab[i];
		if(j==gix)
			return(i);
	}

	if(gix>=0)
	{
		i=ctx->frm->ngbl++;
		ctx->frm->gbltab[i]=gix;
		return(i);
	}
	return(-1);
}

int BS2C_LookupFrameGlobal(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int i, j;
	
	for(i=0; i<ctx->frm->ngbl; i++)
	{
		j=ctx->frm->gbltab[i];
		if(j&3)continue;
		vari=ctx->globals[j>>2];
		if(!strcmp(vari->name, name))
		{
			return(i);
//			return(j);
		}
	}

	j=-1;
	if(ctx->frm->func)
	{
		j=BS2C_LookupVariGlobal(ctx,
			ctx->frm->func, name);
	}

	if(j<0)
		j=BS2C_LookupGlobal(ctx, name); 

	if(j>=0)
	{
		i=ctx->frm->ngbl++;
		ctx->frm->gbltab[i]=j<<2;
		return(i);
//		return(j);
	}
	return(-1);
}

int BS2C_LookupPkgGlobal(
	BS2CC_CompileContext *ctx,
	BS2CC_PkgFrame *pkg,
	char *name)
{
	BS2CC_VarInfo *cur;
	
	if(!pkg)
		return(-1);
	
	cur=pkg->vars;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			{ return(cur->gid); }
		cur=cur->pknext;
	}
	return(-1);
}

int BS2C_LookupPkgImpGlobal(
	BS2CC_CompileContext *ctx,
	BS2CC_PkgFrame *pkg,
	char *name)
{
	BS2CC_VarInfo *cur;
	int i, j, k, l;
	
	if(!pkg)
		return(-1);
	
	i=BS2C_LookupPkgGlobal(ctx, pkg, name);
	if(i>=0)
		return(i);

	for(i=0; i<pkg->nimps; i++)
	{
		j=BS2C_LookupPkgGlobal(ctx, pkg->imps[i], name);
		if(j>=0)
			return(j);
	}

	return(-1);
}

int BS2C_LookupVariGlobal(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	char *name)
{
	int i, j;

	i=BS2C_LookupPkgImpGlobal(ctx, vari->pkg, name);
	if(i>=0)
		return(i);

	i=BS2C_LookupGlobal(ctx, name);
	if(i>=0)
		return(i);
	return(-1);
}

BS2CC_VarInfo *BS2C_GetFrameGlobalInfo(BS2CC_CompileContext *ctx, int idx)
{
	int i, j;
//	j=idx;
	j=ctx->frm->gbltab[idx];
	if(j&3)
		return(NULL);
	return(ctx->globals[j>>2]);
}

BS2CC_VarInfo *BS2C_LookupObjectFieldName(BS2CC_CompileContext *ctx, 
	BS2CC_VarInfo *obj, char *name)
{
	BS2CC_VarInfo *vi;
	int i, j, k;
	
	if(!obj || !name)
		return(NULL);
	
	if(obj->super)
	{
		vi=BS2C_LookupObjectFieldName(ctx, obj->super, name);
		if(vi)return(vi);
	}
	
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

	if(obj->super)
	{
		vi=BS2C_LookupObjectFuncName(ctx, obj->super, name);
		if(vi)return(vi);
	}

	for(i=0; i<obj->nargs; i++)
	{
		vi=obj->args[i];
//		if((vi->vitype!=BS2CC_VITYPE_STRFUNC) &&
//			(vi->vitype!=BS2CC_VITYPE_GBLFUNC))
//				continue;
		if((vi->vitype!=BS2CC_VITYPE_STRFUNC))
				continue;
		if(vi->name && !strcmp(vi->name, name))
		{
			return(vi);
		}
	}
	return(NULL);
}

BS2CC_VarInfo *BS2C_LookupObjectFuncNameB(BS2CC_CompileContext *ctx, 
	BS2CC_VarInfo *obj, char *name)
{
	BS2CC_VarInfo *vi;
	int i, j, k;
	
	if(!obj || !name)
		return(NULL);

	if(obj->super)
	{
		vi=BS2C_LookupObjectFuncName(ctx, obj->super, name);
		if(vi)return(vi);
	}

	for(i=0; i<obj->nargs; i++)
	{
		vi=obj->args[i];
		if((vi->vitype!=BS2CC_VITYPE_STRFUNC) &&
			(vi->vitype!=BS2CC_VITYPE_GBLFUNC))
				continue;
//		if((vi->vitype!=BS2CC_VITYPE_STRFUNC))
//				continue;
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
	
	if(obj->super)
	{
		vi=BS2C_LookupObjectFuncNameArgs(
			ctx, obj->super, name, aty, na);
		if(vi)return(vi);
	}

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
			if(!BS2C_TypeCompatibleP(ctx, vi->args[j]->bty, aty[j]))
				break;
		}
		if(j<na)
			continue;

		return(vi);
	}
	return(NULL);
}


int BS2C_CompileLoadName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty, bty2, z;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->bty;

		if(bty==BS2CC_TYZ_VARARG)
			bty=BS2CC_TYZ_VARIANT_ARR;

		z=BS2C_GetTypeBaseZ(ctx, bty);

		if(BS2C_TypeVarRefP(ctx, bty))
		{
			return(BS2C_CompileLoadDerefName(ctx, name));
		}
		
		if(BS2C_TypeFixedStructP(ctx, bty))
		{
			bty2=BS2C_TypeRefType(ctx, bty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPushType(ctx, bty2);
			return(0);
		}
		
		if(BS2C_TypeX64P(ctx, bty))
			z=BSVM2_OPZ_ADDRESS;

		if(BS2C_TypeX128P(ctx, bty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDXL);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPushType(ctx, bty);
			return(0);
		}
		
		switch(z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_UBYTE:	case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDI);
			BS2C_EmitOpcodeIdx(ctx, i);
//			j=ctx->frm->stackpos++;
//			ctx->frm->stack_bty[j]=bty;
			BS2C_CompileExprPushType(ctx, bty);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDL);
			BS2C_EmitOpcodeIdx(ctx, i);
//			j=ctx->frm->stackpos++;
//			ctx->frm->stack_bty[j]=bty;
			BS2C_CompileExprPushType(ctx, bty);
			break;
		case BSVM2_OPZ_FLOAT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDF);
			BS2C_EmitOpcodeIdx(ctx, i);
//			j=ctx->frm->stackpos++;
//			ctx->frm->stack_bty[j]=bty;
			BS2C_CompileExprPushType(ctx, bty);
			break;
		case BSVM2_OPZ_DOUBLE:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDD);
			BS2C_EmitOpcodeIdx(ctx, i);
//			j=ctx->frm->stackpos++;
//			ctx->frm->stack_bty[j]=bty;
			BS2C_CompileExprPushType(ctx, bty);
			break;
		case BSVM2_OPZ_ADDRESS:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeIdx(ctx, i);
//			j=ctx->frm->stackpos++;
//			ctx->frm->stack_bty[j]=bty;
			BS2C_CompileExprPushType(ctx, bty);
			break;
		}
		return(0);
	}

//	if(ctx->frm->func && ctx->frm->func->obj &&
//		(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
	if(ctx->frm->func && ctx->frm->func->obj)
	{
		vari=BS2C_LookupObjectFieldName(ctx,
			ctx->frm->func->obj, name);
//		if(vari)
		if(vari && (vari->vitype==BS2CC_VITYPE_STRVAR) &&
			(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
		{
			bty=vari->bty;

			i=BS2C_IndexFrameGlobal(ctx, vari->gid);

//			BS2C_CompileExpr(ctx, ln, lt);
			BS2C_CompileExprPushType(ctx, bty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDTHIS);
			BS2C_EmitOpcodeIdx(ctx, i);
			return(0);
		}

		if(vari && (vari->vitype==BS2CC_VITYPE_GBLVAR))
		{
			bty=vari->bty;
			i=BS2C_IndexFrameGlobal(ctx, vari->gid);
			BS2C_CompileExprPushType(ctx, bty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDGS);
			BS2C_EmitOpcodeIdx(ctx, i);
			return(0);
		}
	}

	i=BS2C_LookupFrameGlobal(ctx, name);
	if(i>=0)
	{
		vari=BS2C_GetFrameGlobalInfo(ctx, i);
		bty=vari->bty;

		BS2C_EmitOpcode(ctx, BSVM2_OP_LDGS);
		BS2C_EmitOpcodeIdx(ctx, i);
//		j=ctx->frm->stackpos++;
//		ctx->frm->stack_bty[j]=bty;
		BS2C_CompileExprPushType(ctx, bty);

		return(0);
	}

	BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
	BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
	BS2C_ErrNoDecl(ctx, name);
	return(-1);
}

int BS2C_CompileStoreName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari, *vi;
	int bty, bty2, z;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->bty;

		z=BS2C_GetTypeBaseZ(ctx, bty);
		
		if(BS2C_TypeVarRefP(ctx, bty))
		{
			return(BS2C_CompileStoreDerefName(ctx, name));
		}

		if(BS2C_TypeFixedStructP(ctx, bty))
		{
			vi=BS2C_GetTypeObject(ctx, bty);
			j=BS2C_IndexFrameGlobal(ctx, vi->gid);

			bty2=BS2C_CompileExprPeekType(ctx);

			if(BS2C_TypeVarRefP(ctx, bty2))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_STFXOBJC);
				BS2C_EmitOpcodeIdx(ctx, i);
				BS2C_EmitOpcodeIdx(ctx, j);
				BS2C_CompileExprPopType1(ctx);
				return(0);
			}

			BS2C_EmitOpcode(ctx, BSVM2_OP_STFXOBJD);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_EmitOpcodeIdx(ctx, j);
			BS2C_CompileExprPopType1(ctx);
			return(0);
		}

		if(BS2C_TypeX64P(ctx, bty))
			z=BSVM2_OPZ_ADDRESS;

		if(BS2C_TypeX128P(ctx, bty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_STXL);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPopType1(ctx);
			return(0);
		}
		
		switch(z)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_UBYTE:	case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STI);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPopType1(ctx);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STL);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPopType1(ctx);
			break;
		case BSVM2_OPZ_FLOAT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STF);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPopType1(ctx);
			break;
		case BSVM2_OPZ_DOUBLE:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STD);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPopType1(ctx);
			break;
		case BSVM2_OPZ_ADDRESS:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STA);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPopType1(ctx);
			break;
		}
		return(0);
	}

//	if(ctx->frm->func && ctx->frm->func->obj &&
//		(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
	if(ctx->frm->func && ctx->frm->func->obj)
	{
		vari=BS2C_LookupObjectFieldName(ctx,
			ctx->frm->func->obj, name);
//		if(vari)
		if(vari && (vari->vitype==BS2CC_VITYPE_STRVAR) &&
			(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
		{
			bty=vari->bty;

			i=BS2C_IndexFrameGlobal(ctx, vari->gid);

//			BS2C_CompileExpr(ctx, ln, lt);
//			BS2C_CompileExprPushType(ctx, bty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_STTHIS);
			BS2C_EmitOpcodeIdx(ctx, i);
			ctx->frm->stackpos--;
			return(0);
		}

		if(vari && (vari->vitype==BS2CC_VITYPE_GBLVAR))
		{
			bty=vari->bty;
			i=BS2C_IndexFrameGlobal(ctx, vari->gid);
			BS2C_EmitOpcode(ctx, BSVM2_OP_STGS);
			BS2C_EmitOpcodeIdx(ctx, i);
			ctx->frm->stackpos--;
			return(0);
		}
	}

	i=BS2C_LookupFrameGlobal(ctx, name);
	if(i>=0)
	{
		vari=BS2C_GetFrameGlobalInfo(ctx, i);
		bty=vari->bty;

		BS2C_EmitOpcode(ctx, BSVM2_OP_STGS);
		BS2C_EmitOpcodeIdx(ctx, i);
		ctx->frm->stackpos--;

		return(0);
	}

	BS2C_EmitOpcode(ctx, BSVM2_OP_POPA);
	BS2C_CompileExprPopType1(ctx);
	BS2C_ErrNoDecl(ctx, name);
	return(-1);
}

int BS2C_CompileLoadRefName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty, cty, z;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->bty;

		if(BS2C_TypeVarRefP(ctx, bty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeUCx(ctx, i);
			BS2C_CompileExprPushType(ctx, bty);
			return(0);
		}

		cty=BS2C_TypeRefType(ctx, bty);
		z=BS2C_GetTypeBaseZ(ctx, bty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_LDRL);
		BS2C_EmitOpcodeUZx(ctx, z, i);
		BS2C_CompileExprPushType(ctx, cty);
		return(0);
	}

	if(ctx->frm->func && ctx->frm->func->obj)
	{
		vari=BS2C_LookupObjectFieldName(ctx,
			ctx->frm->func->obj, name);
		if(vari && (vari->vitype==BS2CC_VITYPE_STRVAR) &&
			(ctx->frm->func->vitype!=BS2CC_VITYPE_GBLFUNC))
		{
			bty=vari->bty;
			cty=BS2C_TypeRefType(ctx, bty);

			i=BS2C_IndexFrameGlobal(ctx, vari->gid);

			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_VOID, 4);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDROS);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPushType(ctx, cty);
			return(0);
		}

		if(vari && (vari->vitype==BS2CC_VITYPE_GBLVAR))
		{
			bty=vari->bty;
			cty=BS2C_TypeRefType(ctx, bty);

			i=BS2C_IndexFrameGlobal(ctx, vari->gid);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDRGS);
			BS2C_EmitOpcodeIdx(ctx, i);
			BS2C_CompileExprPushType(ctx, cty);
			return(0);
		}
	}

	i=BS2C_LookupFrameGlobal(ctx, name);
	if(i>=0)
	{
		vari=BS2C_GetFrameGlobalInfo(ctx, i);
		bty=vari->bty;
			cty=BS2C_TypeRefType(ctx, bty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_LDRGS);
		BS2C_EmitOpcodeIdx(ctx, i);
		BS2C_CompileExprPushType(ctx, cty);

		return(0);
	}
	
	BS2C_ErrNoDecl(ctx, name);
	return(-1);
}

int BS2C_CompileLoadDerefName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty, cty, z, o;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->bty;
		cty=BS2C_TypeDerefType(ctx, bty);
		z=BS2C_GetTypeBaseZ(ctx, cty);

		if(BS2C_TypeVarRefP(ctx, bty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDDRVL);
			BS2C_EmitOpcodeUZx(ctx, z, i);
			BS2C_CompileExprPushType(ctx, cty);
			return(0);

#if 0
			o=-1;
//			if(BS2C_TypeSmallIntP(ctx, cty))
//				{ o=BSVM2_OP_LDDRVIL; }
//			else if(BS2C_TypeDoubleP(ctx, cty))
//				{ o=BSVM2_OP_LDDRDL; }
//			else if(BS2C_TypeAddressP(ctx, cty))
//				{ o=BSVM2_OP_LDDRVAL; }

//			if(o>=0)
			if(1)
			{
//				BS2C_EmitOpcode(ctx, o);
//				BS2C_EmitOpcodeUCx(ctx, i);

				BS2C_EmitOpcode(ctx, BSVM2_OP_LDDRVL);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_CompileExprPushType(ctx, cty);
				return(0);
			}

			if(BS2C_TypeSmallIntP(ctx, cty))
				{ o=BSVM2_OP_LDDRI; }
			else if(BS2C_TypeSmallLongP(ctx, cty))
				{ o=BSVM2_OP_LDDRL; }
			else if(BS2C_TypeSmallFloatP(ctx, cty))
				{ o=BSVM2_OP_LDDRF; }
			else if(BS2C_TypeSmallDoubleP(ctx, cty))
				{ o=BSVM2_OP_LDDRD; }
			else if(BS2C_TypeAddressP(ctx, cty))
				{ o=BSVM2_OP_LDDRA; }

			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeUCx(ctx, i);
			BS2C_EmitOpcode(ctx, o);
			BS2C_CompileExprPushType(ctx, cty);
#endif
			return(0);
		}

		if(BS2C_TypePointerP(ctx, bty) || BS2C_TypeArrayP(ctx, bty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDDRAL);
			BS2C_EmitOpcodeUZx(ctx, z, i);
			BS2C_CompileExprPushType(ctx, cty);
			return(0);

#if 0
//			o=-1;
//			if(BS2C_TypeSmallIntP(ctx, cty))
//				{ o=BSVM2_OP_LDDRIL; }
//			else if(BS2C_TypeDoubleP(ctx, cty))
//				{ o=BSVM2_OP_LDDRDL; }
//			else if(BS2C_TypeAddressP(ctx, cty))
//				{ o=BSVM2_OP_LDDRAL; }

//			if(o>=0)
			if(1)
			{
//				BS2C_EmitOpcode(ctx, o);
//				BS2C_EmitOpcodeUCx(ctx, i);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDDRAL);
				BS2C_EmitOpcodeUZx(ctx, z, i);
				BS2C_CompileExprPushType(ctx, cty);
				return(0);
			}

			if(BS2C_TypeSmallIntP(ctx, cty))
				{ o=BSVM2_OP_LDDRI; }
			else if(BS2C_TypeSmallLongP(ctx, cty))
				{ o=BSVM2_OP_LDDRL; }
			else if(BS2C_TypeSmallFloatP(ctx, cty))
				{ o=BSVM2_OP_LDDRF; }
			else if(BS2C_TypeSmallDoubleP(ctx, cty))
				{ o=BSVM2_OP_LDDRD; }
			else if(BS2C_TypeAddressP(ctx, cty))
				{ o=BSVM2_OP_LDDRA; }

			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeUCx(ctx, i);
			BS2C_EmitOpcode(ctx, o);
			BS2C_CompileExprPushType(ctx, cty);
#endif
			return(0);
		}

		BS2C_CaseError(ctx);
		return(-1);
	}

	BS2C_CaseError(ctx);
	return(-1);
}

int BS2C_CompileStoreDerefName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty, cty, z, o;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->bty;
		cty=BS2C_TypeDerefType(ctx, bty);
		z=BS2C_GetTypeBaseZ(ctx, cty);

		if(BS2C_TypeVarRefP(ctx, bty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_STDRVL);
			BS2C_EmitOpcodeUZx(ctx, z, i);
			BS2C_CompileExprPopType1(ctx);
			return(0);

#if 0
			o=-1;
			if(BS2C_TypeSmallIntP(ctx, cty))
				{ o=BSVM2_OP_STDRVIL; }
			else if(BS2C_TypeDoubleP(ctx, cty))
				{ o=BSVM2_OP_STDRDL; }
			else if(BS2C_TypeAddressP(ctx, cty))
				{ o=BSVM2_OP_STDRVAL; }

			if(o>=0)
			{
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeUCx(ctx, i);
				BS2C_CompileExprPopType1(ctx);
//				BS2C_CompileExprPushType(ctx, cty);
				return(0);
			}

			if(BS2C_TypeSmallIntP(ctx, cty))
				{ o=BSVM2_OP_STDRI; }
			else if(BS2C_TypeSmallLongP(ctx, cty))
				{ o=BSVM2_OP_STDRL; }
			else if(BS2C_TypeSmallFloatP(ctx, cty))
				{ o=BSVM2_OP_STDRF; }
			else if(BS2C_TypeSmallDoubleP(ctx, cty))
				{ o=BSVM2_OP_STDRD; }
			else if(BS2C_TypeAddressP(ctx, cty))
				{ o=BSVM2_OP_STDRA; }

			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeUCx(ctx, i);
			BS2C_EmitOpcode(ctx, o);
//			BS2C_CompileExprPushType(ctx, cty);
			BS2C_CompileExprPopType1(ctx);
			return(0);
#endif
		}

		if(BS2C_TypePointerP(ctx, bty) || BS2C_TypeArrayP(ctx, bty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_STDRAL);
			BS2C_EmitOpcodeUZx(ctx, z, i);
			BS2C_CompileExprPopType1(ctx);
			return(0);

#if 0
			o=-1;
			if(BS2C_TypeSmallIntP(ctx, cty))
				{ o=BSVM2_OP_LDDRIL; }
			else if(BS2C_TypeDoubleP(ctx, cty))
				{ o=BSVM2_OP_LDDRDL; }
			else if(BS2C_TypeAddressP(ctx, cty))
				{ o=BSVM2_OP_LDDRAL; }

			if(o>=0)
			{
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitOpcodeUCx(ctx, i);
				BS2C_CompileExprPushType(ctx, cty);
				return(0);
			}

			if(BS2C_TypeSmallIntP(ctx, cty))
				{ o=BSVM2_OP_STDRI; }
			else if(BS2C_TypeSmallLongP(ctx, cty))
				{ o=BSVM2_OP_STDRL; }
			else if(BS2C_TypeSmallFloatP(ctx, cty))
				{ o=BSVM2_OP_STDRF; }
			else if(BS2C_TypeSmallDoubleP(ctx, cty))
				{ o=BSVM2_OP_STDRD; }
			else if(BS2C_TypeAddressP(ctx, cty))
				{ o=BSVM2_OP_STDRA; }

			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeUCx(ctx, i);
			BS2C_EmitOpcode(ctx, o);
//			BS2C_CompileExprPushType(ctx, cty);
			BS2C_CompileExprPopType1(ctx);
			return(0);
#endif
		}

		BS2C_CaseError(ctx);
		return(-1);
	}

	BS2C_CaseError(ctx);
	return(-1);
}


int BS2C_LookupDynamicSlotName(
	BS2CC_CompileContext *ctx, char *name)
{
	char tb[256];
	BS2CC_VarInfo *vari;
	int i, j;
	
	sprintf(tb, "$DY$/%s", name);
	
	for(i=0; i<ctx->nglobals; i++)
	{
		vari=ctx->globals[i];
		if(!vari)
			continue;
		if(!vari->qname)
			continue;
		
		if(vari->vitype!=BS2CC_VITYPE_STRVAR)
			continue;
		
//		if(!strcmp(vari->name, name))
		if(!strcmp(vari->qname, tb))
			return(i);
	}
	return(-1);
}

int BS2C_GetDynamicSlotName(
	BS2CC_CompileContext *ctx, char *name)
{
	char tb[256];
	BS2CC_VarInfo *vi;
	int i, j;
	
	i=BS2C_LookupDynamicSlotName(ctx, name);
	if(i>=0)return(i);

	sprintf(tb, "$DY$/%s", name);

	vi=BS2C_AllocVarInfo(ctx);

	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->bty=BS2CC_TYZ_VARIANT;
	vi->vitype=BS2CC_VITYPE_STRVAR;

	vi->name=BS2P_StrSym(ctx, name);
	vi->qname=BS2P_StrSym(ctx, tb);
	vi->sig=BS2P_StrSym(ctx, "r");

	return(i);
}

int BS2C_GetFrameDynamicSlotName(
	BS2CC_CompileContext *ctx, char *name)
{
	int i, j;
	
	i=BS2C_GetDynamicSlotName(ctx, name);
	j=BS2C_IndexFrameGlobal(ctx, i);
	return(j);
}

int BS2C_GetFrameSymbol(
	BS2CC_CompileContext *ctx, char *name)
{
	int i, j;
	
//	i=BS2C_GetDynamicSlotName(ctx, name);
//	j=BS2C_IndexFrameGlobal(ctx, i);

	i=BS2C_ImgGetString(ctx, name);
	j=BS2C_IndexFrameLiteral(ctx, (i<<4)|BSVM2_OPZY_STRSYM);

	return(j);
}
