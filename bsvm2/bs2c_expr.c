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
		if(!strcmp(vari->name, name))
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
		vari=ctx->globals[j];
		if(!strcmp(vari->name, name))
		{
//			return(i);
			return(j);
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
//		return(i);
		return(j);
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
	j=idx;
//	j=ctx->frm->gbltab[idx];
	return(ctx->globals[j]);
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

void BS2C_CompileConvType(BS2CC_CompileContext *ctx, int dty)
{
	int sty;
	int i;
	
	i=ctx->frm->stackpos-1;
	sty=ctx->frm->stack_bty[i];
	
	if(sty==dty)
		return;
	
	if(BS2C_TypeCompatibleP(ctx, dty, sty))
		return;
	
	if(dty==BSVM2_OPZ_INT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_INT;
		if(sty==BSVM2_OPZ_LONG)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I); return; }
		if(sty==BSVM2_OPZ_FLOAT)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTF2I); return; }
		if(sty==BSVM2_OPZ_DOUBLE)
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTD2I); return; }
		
		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2I); return; }
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
		
		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2L); return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ return; }
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

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2F); return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2F); return; }
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

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTI2F); return; }
		if(BS2C_InferTypeSmallLongP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTL2F); return; }
	}

	if(dty==BSVM2_OPZ_SBYTE)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_SBYTE;

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSB2I); return; }

		if(BS2C_InferTypeSmallLongP(ctx, sty))
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
	}

	if(dty==BSVM2_OPZ_UBYTE)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_UBYTE;

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUB2I); return; }

		if(BS2C_InferTypeSmallLongP(ctx, sty))
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
	}

	if(dty==BSVM2_OPZ_SHORT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_SHORT;

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTSS2I); return; }

		if(BS2C_InferTypeSmallLongP(ctx, sty))
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
	}

	if(dty==BSVM2_OPZ_USHORT)
	{
		ctx->frm->stack_bty[i]=BSVM2_OPZ_USHORT;

		if(BS2C_InferTypeSmallIntP(ctx, sty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_CVTUS2I); return; }

		if(BS2C_InferTypeSmallLongP(ctx, sty))
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
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_CompileLoadNameAsType(BS2CC_CompileContext *ctx,
	char *name, int ty)
{
	BS2C_CompileLoadName(ctx, name);
	BS2C_CompileConvType(ctx, ty);
}

void BS2C_CompileExprPushType(BS2CC_CompileContext *ctx, int dty)
{
	int i;
	
	i=ctx->frm->stackpos++;
	ctx->frm->stack_bty[i]=dty;
}

void BS2C_CompilePop(BS2CC_CompileContext *ctx)
{
	int ty;
	int i;
	
	i=ctx->frm->stackpos-1;
	ty=ctx->frm->stack_bty[i];
	
	if(BS2C_InferTypeSmallIntP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPI);
		ctx->frm->stackpos--;
		return;
	}
	if(BS2C_InferTypeSmallLongP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPL);
		ctx->frm->stackpos--;
		return;
	}

	if(BS2C_InferTypeSmallFloatP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPF);
		ctx->frm->stackpos--;
		return;
	}
	if(BS2C_InferTypeSmallDoubleP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPD);
		ctx->frm->stackpos--;
		return;
	}
	if(BS2C_InferTypeAddressP(ctx, ty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_POPA);
		ctx->frm->stackpos--;
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileNoexPop(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX1);
	BS2C_CompilePop(ctx);
	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPop2(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX2);
	BS2C_CompilePop(ctx);
	BS2C_CompilePop(ctx);
	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPop3(BS2CC_CompileContext *ctx)
{
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX3);
	BS2C_CompilePop(ctx);
	BS2C_CompilePop(ctx);
	BS2C_CompilePop(ctx);
	ctx->frm->newtrace=1;
}

void BS2C_CompileNoexPopN(BS2CC_CompileContext *ctx, int n)
{
	int i;

	if(n==1)
		{ BS2C_CompileNoexPop(ctx); return; }
	if(n==2)
		{ BS2C_CompileNoexPop2(ctx); return; }
	if(n==3)
		{ BS2C_CompileNoexPop3(ctx); return; }
	
	i=n;
	BS2C_EmitOpcode(ctx, BSVM2_OP_HNOEX);
	while((i--)>0)
		BS2C_CompilePop(ctx);
//	BS2C_EmitOpcode(ctx, BSVM2_OP_LABEL);
//	ctx->frm->newtrace=1;
}

void BS2C_CompileBinaryOpSS(BS2CC_CompileContext *ctx, char *op, int dty)
{
	if(!strcmp(op, "+"))
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDL); return; }
		if(BS2C_InferTypeSmallFloatP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDF); return; }
		if(BS2C_InferTypeSmallDoubleP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ADDD); return; }
	}

	if(!strcmp(op, "-"))
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBL); return; }
		if(BS2C_InferTypeSmallFloatP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBF); return; }
		if(BS2C_InferTypeSmallDoubleP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SUBD); return; }
	}

	if(!strcmp(op, "*"))
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULL); return; }
		if(BS2C_InferTypeSmallFloatP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULF); return; }
		if(BS2C_InferTypeSmallDoubleP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_MULD); return; }
	}

	if(!strcmp(op, "/"))
	{
		BS2C_CompileExprPushType(ctx, dty);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOP);
			BS2C_EmitByte(ctx, (BSVM2_OPZ_INT<<4)|BSVM2_OPRI_DIV);
			return;
		}
		if(BS2C_InferTypeSmallLongP(ctx, dty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_BINOP);
			BS2C_EmitByte(ctx, (BSVM2_OPZ_LONG<<4)|BSVM2_OPRI_DIV);
			return;
		}

		if(BS2C_InferTypeSmallFloatP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_DIVF); return; }
		if(BS2C_InferTypeSmallDoubleP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_DIVD); return; }
	}

	if(!strcmp(op, "&"))
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ANDI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ANDL); return; }
	}

	if(!strcmp(op, "|"))
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ORI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_ORL); return; }
	}

	if(!strcmp(op, "^"))
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_XORI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_XORL); return; }
	}

	if(!strcmp(op, "<<"))
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHLI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SHLL); return; }
	}

	if(!strcmp(op, ">>"))
	{
		BS2C_CompileExprPushType(ctx, dty);
		if(BS2C_InferTypeSmallIntP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SARI); return; }
		if(BS2C_InferTypeSmallLongP(ctx, dty))
			{ BS2C_EmitOpcode(ctx, BSVM2_OP_SARL); return; }
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileCall(BS2CC_CompileContext *ctx, dtVal expr, int dty)
{
	BS2CC_VarInfo *vari;
	BS2CC_VarInfo *vai;
	dtVal n0, n1;
	dtVal an;
	char *fn;
	int ix, bty;
	int i, j, k, l;

	an=BS2P_GetAstNodeAttr(expr, "args");
	fn=BS2P_GetAstNodeAttrS(expr, "func");
	if(fn)
	{
		ix=BS2C_LookupFrameGlobal(ctx, fn);
		if(ix<0)
		{
			BS2C_ErrNoDecl(ctx, fn);
			return;
		}

		vari=BS2C_GetFrameGlobalInfo(ctx, ix);
		bty=vari->bty;

		if(dtvIsArrayP(an))
		{
			l=dtvArrayGetSize(an);
			if(l<vari->nargs)
			{
				BS2C_ErrTooFewArgs(ctx);
			}else if(l>vari->nargs)
			{
				BS2C_ErrTooManyArgs(ctx);
				l=vari->nargs;
			}
			
			for(i=0; i<l; i++)
			{
				vai=vari->args[i];
				n0=dtvArrayGetIndexDtVal(an, i);
				BS2C_CompileExpr(ctx, n0, vai->bty);
			}
		}else if(!dtvNullP(an))
		{
			l=1;
			if(l<vari->nargs)
			{
				BS2C_ErrTooFewArgs(ctx);
			}else if(l>vari->nargs)
			{
				BS2C_ErrTooManyArgs(ctx);
				l=vari->nargs;
			}

			vai=vari->args[0];
			BS2C_CompileExpr(ctx, an, vai->bty);
		}else
		{
			if(vari->nargs)
				BS2C_ErrTooFewArgs(ctx);
		}
	
		BS2C_EmitOpcode(ctx, BSVM2_OP_CALLG);
		BS2C_EmitOpcodeIdx(ctx, ix);

		if((vari->rty!=BSVM2_OPZ_VOID) && (dty!=BSVM2_OPZ_VOID))
		{
			BS2C_CompileExprPushType(ctx, vari->rty);
			BS2C_CompileConvType(ctx, dty);
		}else if(vari->rty!=BSVM2_OPZ_VOID)
		{
			BS2C_CompileExprPushType(ctx, vari->rty);
			BS2C_CompilePop(ctx);
		}else if(dty!=BSVM2_OPZ_VOID)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_ERRBADCONV);
		}

//		BS2C_EmitOpcode(ctx, BSVM2_OP_LDGS);
//		BS2C_EmitOpcodeIdx(ctx, i);
//		j=ctx->frm->stackpos++;
//		ctx->frm->stack_bty[j]=bty;

		return;
	}

	BS2C_CaseError(ctx);
	return;
}

void BS2C_CompileExpr(BS2CC_CompileContext *ctx,
	dtVal expr, int dty)
{
	dtVal ln, rn, cn;
	s64 li, lj, lk;
	int t0, t1, t2, t3;
	int lt, rt, cty;
	char *tag;
	char *op;
	int z, o;
	int i;

	if(dtvIsFixIntP(expr) || dtvIsFixUIntP(expr) ||
		dtvIsFixLongP(expr) || dtvIsFixFloatP(expr) ||
		dtvIsFixDoubleP(expr))
	{
		if(dty==BSVM2_OPZ_INT)
		{
			li=dtvUnwrapLong(expr);
			if(li>0)
			{
				BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UINT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
				BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UINT, li);
				return;
			}
			
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, li);
			return;
		}
		if(dty==BSVM2_OPZ_UINT)
		{
			li=dtvUnwrapLong(expr);
			lj=(int)li;
			if(lj<0)
			{
				BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
				BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
				BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, lj);
				return;
			}
		
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UINT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UINT, li);
			return;
		}
		if(dty==BSVM2_OPZ_LONG)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG,
				dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_ULONG)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_ULONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_ULONG,
				dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_FLOAT)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_FLOAT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeZxF(ctx,
				dtvUnwrapDouble(expr));
			return;
		}
		if(dty==BSVM2_OPZ_DOUBLE)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeZxD(ctx,
				dtvUnwrapDouble(expr));
			return;
		}

		if(dty==BSVM2_OPZ_UBYTE)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_UBYTE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UBYTE,
				(byte)dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_SBYTE)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_SBYTE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_SBYTE,
				(sbyte)dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_USHORT)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_USHORT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_USHORT,
				(u16)dtvUnwrapLong(expr));
			return;
		}
		if(dty==BSVM2_OPZ_SHORT)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_SHORT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDC);
			BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_SHORT,
				(s16)dtvUnwrapLong(expr));
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	if(BGBDT_TagStr_IsSymbolP(expr))
	{
		BS2C_CompileLoadNameAsType(ctx,
			BGBDT_TagStr_GetUtf8(expr), dty);
		return;
	}

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		BS2C_CaseError(ctx);
		return;
	}
	
	if(!strcmp(tag, "binary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
		{
			if(dtvIsFixIntP(rn))
			{
				li=dtvUnwrapLong(rn);

				o=-1;
				if(!strcmp(op, "+"))o=BSVM2_OP_ADDIC;
				if(!strcmp(op, "-"))o=BSVM2_OP_SUBIC;
				if(!strcmp(op, "&"))o=BSVM2_OP_ANDIC;
				if(!strcmp(op, "|"))o=BSVM2_OP_ORIC;
				if(!strcmp(op, "^"))o=BSVM2_OP_XORIC;
				if(!strcmp(op, "<<"))o=BSVM2_OP_SHLIC;

				if(BS2C_InferTypeSignedP(ctx, dty))
				{
					if(!strcmp(op, "*"))o=BSVM2_OP_MULIC;
					if(!strcmp(op, ">>"))o=BSVM2_OP_SARIC;
				}
				
				if(o>0)
				{
					BS2C_CompileExpr(ctx, ln, dty);
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeSCx(ctx, li);
					return;
				}
			}

			if(dtvIsFixIntP(ln))
			{
				li=dtvUnwrapLong(ln);

				o=-1;
				if(!strcmp(op, "+"))o=BSVM2_OP_ADDIC;
				if(!strcmp(op, "&"))o=BSVM2_OP_ANDIC;
				if(!strcmp(op, "|"))o=BSVM2_OP_ORIC;
				if(!strcmp(op, "^"))o=BSVM2_OP_XORIC;
				if(BS2C_InferTypeSignedP(ctx, dty))
					{ if(!strcmp(op, "*"))o=BSVM2_OP_MULIC; }
				
				if(o>0)
				{
					BS2C_CompileExpr(ctx, rn, dty);
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitOpcodeSCx(ctx, li);
					return;
				}
			}
		}

		BS2C_CompileExpr(ctx, ln, dty);
		BS2C_CompileExpr(ctx, rn, dty);

		BS2C_CompileBinaryOpSS(ctx, op, dty);
		return;

//		lt=BS2C_InferExpr(ctx, ln);
//		rt=BS2C_InferExpr(ctx, rn);
//		ty=BS2C_InferSuperType(ctx, lt, rt);
//		return(ty);
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
		cty=BS2C_InferSuperType(ctx, lt, rt);

		BS2C_CompileExpr(ctx, ln, cty);
		BS2C_CompileExpr(ctx, rn, cty);

		z=BS2C_GetTypeBaseZ(ctx, dty);
		if(!strcmp(op, "=="))
			o=BSVM2_OPRC_EQ;
		if(!strcmp(op, "!="))
			o=BSVM2_OPRC_NE;
		if(!strcmp(op, "<"))
			o=BSVM2_OPRC_LT;
		if(!strcmp(op, ">"))
			o=BSVM2_OPRC_GT;
		if(!strcmp(op, "<="))
			o=BSVM2_OPRC_LE;
		if(!strcmp(op, ">="))
			o=BSVM2_OPRC_GE;
		if(!strcmp(op, "==="))
			o=BSVM2_OPRC_EQQ;
		if(!strcmp(op, "!=="))
			o=BSVM2_OPRC_NEQ;

		BS2C_EmitOpcode(ctx, BSVM2_OP_CMPOP);
		BS2C_EmitByte(ctx, (z<<4)|o);

		if(BS2C_InferTypeSmallIntP(ctx, dty))
		{
			BS2C_CompileExprPushType(ctx, dty);
			return;
		}

		BS2C_CompileExprPushType(ctx, BSVM2_OPZ_INT);
		BS2C_CompileConvType(ctx, dty);
		return;
	}

	if(!strcmp(tag, "assign"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
//		op=BS2P_GetAstNodeAttrS(expr, "op");

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		cty=BS2C_InferSuperType(ctx, lt, rt);

//		BS2C_CompileExpr(ctx, ln, cty);
//		BS2C_CompileExpr(ctx, rn, cty);

		BS2C_CompileExpr(ctx, rn, cty);
		BS2C_CompileConvType(ctx, lt);
		
		if(!BS2C_InferTypeVoidP(ctx, dty))
		{
			if(BS2C_InferTypeSmallIntP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI); }
			else if(BS2C_InferTypeSmallLongP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPL); }
			else if(BS2C_InferTypeSmallFloatP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF); }
			else if(BS2C_InferTypeSmallDoubleP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD); }
			else if(BS2C_InferTypeAddressP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPA); }
			else
			{
				z=lt&15;
				BS2C_EmitOpcode(ctx, BSVM2_OP_DUPN);
				BS2C_EmitByte(ctx, (z<<4)|1);
			}
		}
		
		if(BGBDT_TagStr_IsSymbolP(ln))
		{
			BS2C_CompileStoreName(ctx,
				BGBDT_TagStr_GetUtf8(ln));
		}else
		{
			BS2C_CaseError(ctx);
			return;
		}
		
		if(!BS2C_InferTypeVoidP(ctx, dty))
		{
			BS2C_CompileConvType(ctx, dty);
		}
		return;
	}

	if(!strcmp(tag, "assignop"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		cty=BS2C_InferSuperType(ctx, lt, rt);

		BS2C_CompileExpr(ctx, ln, cty);
		BS2C_CompileExpr(ctx, rn, cty);
		BS2C_CompileBinaryOpSS(ctx, op, cty);

//		BS2C_CompileExpr(ctx, rn, cty);
//		BS2C_CompileConvType(ctx, lt);
		
		if(!BS2C_InferTypeVoidP(ctx, dty))
		{
			if(BS2C_InferTypeSmallIntP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI); }
			else if(BS2C_InferTypeSmallLongP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPL); }
			else if(BS2C_InferTypeSmallFloatP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPF); }
			else if(BS2C_InferTypeSmallDoubleP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD); }
			else if(BS2C_InferTypeAddressP(ctx, lt))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_DUPA); }
			else
			{
				z=lt&15;
				BS2C_EmitOpcode(ctx, BSVM2_OP_DUPN);
				BS2C_EmitByte(ctx, (z<<4)|1);
			}
		}
		
		if(BGBDT_TagStr_IsSymbolP(ln))
		{
			BS2C_CompileStoreName(ctx,
				BGBDT_TagStr_GetUtf8(ln));
		}else
		{
			BS2C_CaseError(ctx);
			return;
		}
		
		if(!BS2C_InferTypeVoidP(ctx, dty))
		{
			BS2C_CompileConvType(ctx, dty);
		}
		return;
	}

	if(!strcmp(tag, "tern"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		cn=BS2P_GetAstNodeAttr(expr, "cond");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);
		cn=BS2C_ReduceExpr(ctx, cn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
		cty=BS2C_InferSuperType(ctx, lt, rt);
		
//		t0=BS2C_GenTempLabel(ctx);
		t1=BS2C_GenTempLabel(ctx);
		t2=BS2C_GenTempLabel(ctx);

		BS2C_CompileTempJmpElse(ctx, cn, t1);

//		BS2C_EmitTempLabel(ctx, t0);
		BS2C_CompileExpr(ctx, ln, cty);
		BS2C_EmitTempJump(ctx, t2);
		BS2C_CompileNoexPop(ctx);

		BS2C_EmitTempLabelB(ctx, t1);
		BS2C_CompileExpr(ctx, rn, cty);

		BS2C_EmitTempLabelB(ctx, t2);
		return;
	}
	
	if(!strcmp(tag, "call"))
	{
		BS2C_CompileCall(ctx, expr, dty);
		return;
	}

	BS2C_CaseError(ctx);
	return;
}
