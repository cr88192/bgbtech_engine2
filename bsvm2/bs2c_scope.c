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
	BS2CC_VarInfo *vari;
	int i, j;
	
	for(i=0; i<ctx->frm->ngbl; i++)
	{
		j=ctx->frm->gbltab[i];
//		vari=ctx->globals[j];
//		if(!strcmp(vari->name, name))
		if(j==gix)
		{
			return(i);
//			return(j);
		}
	}

	j=gix;
	if(j>=0)
	{
		i=ctx->frm->ngbl++;
		ctx->frm->gbltab[i]=j;
		return(i);
//		return(j);
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
		vari=ctx->globals[j];
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
		ctx->frm->gbltab[i]=j;
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
	return(ctx->globals[j]);
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
			if(!BS2C_TypeCompatibleP(ctx, vi->args[i]->bty, aty[i]))
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
	int bty;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->bty;
		
		switch(bty)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_UBYTE:	case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDI);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDL);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		case BSVM2_OPZ_FLOAT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDF);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		case BSVM2_OPZ_DOUBLE:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDD);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
			break;
		case BSVM2_OPZ_ADDRESS:
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeIdx(ctx, i);
			j=ctx->frm->stackpos++;
			ctx->frm->stack_bty[j]=bty;
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
		j=ctx->frm->stackpos++;
		ctx->frm->stack_bty[j]=bty;

		return(0);
	}
	
	BS2C_ErrNoDecl(ctx, name);
	return(-1);
}

int BS2C_CompileStoreName(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_VarInfo *vari;
	int bty;
	int i, j, k;
	
	i=BS2C_LookupLocal(ctx, name);
	if(i>=0)
	{
		vari=ctx->frm->locals[i];
		bty=vari->bty;
		
		switch(bty)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_UBYTE:	case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STI);
			BS2C_EmitOpcodeIdx(ctx, i);
			ctx->frm->stackpos--;
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STL);
			BS2C_EmitOpcodeIdx(ctx, i);
			ctx->frm->stackpos--;
			break;
		case BSVM2_OPZ_FLOAT:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STF);
			BS2C_EmitOpcodeIdx(ctx, i);
			ctx->frm->stackpos--;
			break;
		case BSVM2_OPZ_DOUBLE:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STD);
			BS2C_EmitOpcodeIdx(ctx, i);
			ctx->frm->stackpos--;
			break;
		case BSVM2_OPZ_ADDRESS:
			BS2C_EmitOpcode(ctx, BSVM2_OP_STA);
			BS2C_EmitOpcodeIdx(ctx, i);
			ctx->frm->stackpos--;
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

	BS2C_ErrNoDecl(ctx, name);
	return(-1);
}
