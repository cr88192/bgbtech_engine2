char *BS2J2C_CNameMangle(char *pfx, char *cn)
{
	char tb[256];
	char *s, *t;
	int i, j, k;
	
	s=cn; t=tb;
	
	if(pfx)
	{
		strcpy(t, pfx);
		t=t+strlen(t);
	}else
	{
		if((*s>='0') && (*s<='9'))
			*t++='_';
	}
	
	while(*s)
	{
		if((*s>='0') && (*s<='9'))
			{ *t++=*s++; continue; }
		if((*s>='A') && (*s<='Z'))
			{ *t++=*s++; continue; }
		if((*s>='a') && (*s<='z'))
			{ *t++=*s++; continue; }
			
		if(*s=='_')
			{ s++; *t++='_'; *t++='1'; continue; }
		if(*s==';')
			{ s++; *t++='_'; *t++='2'; continue; }
		if(*s=='[')
			{ s++; *t++='_'; *t++='3'; continue; }
		if(*s=='(')
			{ s++; *t++='_'; *t++='4'; continue; }
		if(*s==')')
			{ s++; *t++='_'; *t++='5'; continue; }
		if(*s=='/')
			{ s++; *t++='_'; *t++='6'; continue; }
		
		i=*s++;
		if((i>=0x00) && (i<=0xFF))
		{
			sprintf(t, "_9%02X", i);
			t+=strlen(t);
			continue;
		}

		sprintf(t, "_0%04X", i);
		t+=strlen(t);
		continue;
	}
	*t++=0;
	
	return(bgbdt_mm_rstrdup(tb));
}

char *BS2J2C_GetVariQNameMangle(BSVM2_TransContext *ctx,
	BSVM2_ImageGlobal *vari)
{
	return(BS2J2C_CNameMangle("BQN_", vari->qname));
}

char *BS2J2C_GenSym(BSVM2_TransContext *ctx)
{
	char *fn;
	fn=BASM_GenSym();
	return(fn);
}

char *BS2J2C_GetStr_BinOpTok(BSVM2_TransContext *ctx, int op)
{
	char *s;
	switch(op)
	{
	case BSVM2_OPRI_ADD: s="+"; break;
	case BSVM2_OPRI_SUB: s="-"; break;
	case BSVM2_OPRI_MUL: s="*"; break;
	case BSVM2_OPRI_AND: s="&"; break;
	case BSVM2_OPRI_OR:  s="|"; break;
	case BSVM2_OPRI_XOR: s="^"; break;
	case BSVM2_OPRI_SHL: s="<<"; break;
	case BSVM2_OPRI_SAR: s=">>"; break;
	case BSVM2_OPRI_SHR: s=">>"; break;
	case BSVM2_OPRI_DIV: s="/"; break;
	case BSVM2_OPRI_MOD: s="%"; break;
	}
	return(s);
}

char *BS2J2C_GetStr_UnOpTok(BSVM2_TransContext *ctx, int op)
{
	char *s;
	switch(op)
	{
	case BSVM2_OPRU_NEG: s="-"; break;
	case BSVM2_OPRU_NOT: s="~"; break;
	}
	return(s);
}

char *BS2J2C_GetStr_OpZChar(BSVM2_TransContext *ctx, int z)
{
	char *s;
	switch(z)
	{
	case BSVM2_OPZ_INT: s="i"; break;
	case BSVM2_OPZ_LONG: s="l"; break;
	case BSVM2_OPZ_FLOAT: s="f"; break;
	case BSVM2_OPZ_DOUBLE: s="d"; break;
	case BSVM2_OPZ_ADDRESS: s="a"; break;
	case BSVM2_OPZ_UINT: s="ui"; break;
	case BSVM2_OPZ_UBYTE: s="ub"; break;
	case BSVM2_OPZ_SHORT: s="s"; break;
	case BSVM2_OPZ_SBYTE: s="sb"; break;
	case BSVM2_OPZ_USHORT: s="us"; break;
	case BSVM2_OPZ_ULONG: s="ul"; break;
	case BSVM2_OPZ_CONST: s="v"; break;
	case BSVM2_OPZ_NLONG: s="nl"; break;
	case BSVM2_OPZ_UNLONG: s="unl"; break;
	case BSVM2_OPZ_X128: s="x"; break;
	}
	return(s);
}

char *BS2J2C_GetStr_StackVarI(BSVM2_TransContext *ctx, int idx)
{
	char tb[256];
	sprintf(tb, "stack[%d].i", idx);
	return(bgbdt_mm_rstrdup(tb));
}

char *BS2J2C_GetStr_StackVarZ(BSVM2_TransContext *ctx,
	int idx, int z)
{
	char tb[256];
	sprintf(tb, "stack[%d].%s", idx, BS2J2C_GetStr_OpZChar(ctx, z));
	return(bgbdt_mm_rstrdup(tb));
}

char *BS2J2C_GetStr_LocalVarZ(BSVM2_TransContext *ctx,
	int idx, int z)
{
	char tb[256];
	sprintf(tb, "local[%d].%s", idx, BS2J2C_GetStr_OpZChar(ctx, z));
	return(bgbdt_mm_rstrdup(tb));
}

char *BS2J2C_GetStr_LexVarZ(BSVM2_TransContext *ctx,
	int idx, int z)
{
	char tb[256];
	sprintf(tb, "lexvar[%d].%s", idx, BS2J2C_GetStr_OpZChar(ctx, z));
	return(bgbdt_mm_rstrdup(tb));
}

char *BS2J2C_GetStr_VarZ(BSVM2_TransContext *ctx,
	int idx, int z)
{
	if((idx&BSVM2_VAM_MASK)==BSVM2_VAM_STACK)
		return(BS2J2C_GetStr_StackVarZ(ctx, idx, z));
	if((idx&BSVM2_VAM_MASK)==BSVM2_VAM_LOCAL)
		return(BS2J2C_GetStr_LocalVarZ(ctx, idx, z));
	if((idx&BSVM2_VAM_MASK)==BSVM2_VAM_LEXVAR)
		return(BS2J2C_GetStr_LexVarZ(ctx, idx, z));
}

void BS2J2C_Emit_BinOpSSS(BSVM2_TransContext *ctx,
	int op, int z, int v0, int v1, int v2)
{
	BGBDT_MM_PrintPutPrintf(ctx->ob_func, "\t%s=%s%s%s;",
		BS2J2C_GetStr_StackVarZ(ctx, v0, z),
		BS2J2C_GetStr_StackVarZ(ctx, v1, z),
		BS2J2C_GetStr_BinOpTok(ctx, op),
		BS2J2C_GetStr_StackVarZ(ctx, v2, z));
}

void BS2J2C_Emit_UnOpSS(BSVM2_TransContext *ctx,
	int op, int z, int v0, int v1)
{
	BGBDT_MM_PrintPutPrintf(ctx->ob_func, "\t%s=%s%s;",
		BS2J2C_GetStr_StackVarZ(ctx, v0, z),
		BS2J2C_GetStr_UnOpTok(ctx, op),
		BS2J2C_GetStr_StackVarZ(ctx, v1, z));
}

void BS2J2C_Emit_MovOp(BSVM2_TransContext *ctx,
	int z, int v0, int v1)
{
	BGBDT_MM_PrintPutPrintf(ctx->ob_func, "\t%s=%s;",
		BS2J2C_GetStr_VarZ(ctx, v0, z),
		BS2J2C_GetStr_VarZ(ctx, v1, z));
}


int BS2J2C_TransOpcode(BSVM2_TransContext *ctx,
	BSVM2_Trace *tr, BSVM2_Opcode *op)
{
	switch(op->opn)
	{
	case BSVM2_OP_ADDI:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_ADD, BSVM2_OPZ_INT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_SUBI:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_SUB, BSVM2_OPZ_INT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_MULI:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_MUL, BSVM2_OPZ_INT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_ANDI:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_AND, BSVM2_OPZ_INT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_ORI:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_OR, BSVM2_OPZ_INT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_XORI:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_XOR, BSVM2_OPZ_INT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_SHLI:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_SHL, BSVM2_OPZ_INT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_SARI:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_SAR, BSVM2_OPZ_INT,
			op->t0, op->t0, op->t1);	break;


	case BSVM2_OP_ADDL:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_ADD, BSVM2_OPZ_LONG,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_SUBL:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_SUB, BSVM2_OPZ_LONG,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_MULL:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_MUL, BSVM2_OPZ_LONG,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_ANDL:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_AND, BSVM2_OPZ_LONG,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_ORL:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_OR, BSVM2_OPZ_LONG,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_XORL:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_XOR, BSVM2_OPZ_LONG,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_SHLL:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_SHL, BSVM2_OPZ_LONG,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_SARL:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_SAR, BSVM2_OPZ_LONG,
			op->t0, op->t0, op->t1);	break;

	case BSVM2_OP_ADDF:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_ADD, BSVM2_OPZ_FLOAT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_SUBF:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_SUB, BSVM2_OPZ_FLOAT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_MULF:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_MUL, BSVM2_OPZ_FLOAT,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_DIVF:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_DIV, BSVM2_OPZ_FLOAT,
			op->t0, op->t0, op->t1);	break;

	case BSVM2_OP_ADDD:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_ADD, BSVM2_OPZ_DOUBLE,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_SUBD:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_SUB, BSVM2_OPZ_DOUBLE,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_MULD:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_MUL, BSVM2_OPZ_DOUBLE,
			op->t0, op->t0, op->t1);	break;
	case BSVM2_OP_DIVD:
		BS2J2C_Emit_BinOpSSS(ctx,	BSVM2_OPRI_DIV, BSVM2_OPZ_DOUBLE,
			op->t0, op->t0, op->t1);	break;

	case BSVM2_OP_NEGI:
		BS2J2C_Emit_UnOpSS(ctx,	BSVM2_OPRU_NEG, BSVM2_OPZ_INT,
			op->t0, op->t1);	break;
	case BSVM2_OP_NEGL:
		BS2J2C_Emit_UnOpSS(ctx,	BSVM2_OPRU_NEG, BSVM2_OPZ_LONG,
			op->t0, op->t1);	break;
	case BSVM2_OP_NEGF:
		BS2J2C_Emit_UnOpSS(ctx,	BSVM2_OPRU_NEG, BSVM2_OPZ_FLOAT,
			op->t0, op->t1);	break;
	case BSVM2_OP_NEGD:
		BS2J2C_Emit_UnOpSS(ctx,	BSVM2_OPRU_NEG, BSVM2_OPZ_DOUBLE,
			op->t0, op->t1);	break;
	case BSVM2_OP_NOTI:
		BS2J2C_Emit_UnOpSS(ctx,	BSVM2_OPRU_NOT, BSVM2_OPZ_INT,
			op->t0, op->t1);	break;
	case BSVM2_OP_NOTL:
		BS2J2C_Emit_UnOpSS(ctx,	BSVM2_OPRU_NOT, BSVM2_OPZ_LONG,
			op->t0, op->t1);	break;
			
	case BSVM2_OP_LDI:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_INT,
			BSVM2_VAM_STACK|op->t0, BSVM2_VAM_LOCAL|op->i0);
		break;
	case BSVM2_OP_LDL:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_LONG,
			BSVM2_VAM_STACK|op->t0, BSVM2_VAM_LOCAL|op->i0);
		break;
	case BSVM2_OP_LDF:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_FLOAT,
			BSVM2_VAM_STACK|op->t0, BSVM2_VAM_LOCAL|op->i0);
		break;
	case BSVM2_OP_LDD:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_DOUBLE,
			BSVM2_VAM_STACK|op->t0, BSVM2_VAM_LOCAL|op->i0);
		break;
	case BSVM2_OP_LDA:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_ADDRESS,
			BSVM2_VAM_STACK|op->t0, BSVM2_VAM_LOCAL|op->i0);
		break;

	case BSVM2_OP_STI:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_INT,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_STACK|op->t0);
		break;
	case BSVM2_OP_STL:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_LONG,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_STACK|op->t0);
		break;
	case BSVM2_OP_STF:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_FLOAT,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_STACK|op->t0);
		break;
	case BSVM2_OP_STD:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_DOUBLE,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_STACK|op->t0);
		break;
	case BSVM2_OP_STA:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_ADDRESS,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_STACK|op->t0);
		break;

	case BSVM2_OP_MVI:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_INT,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_LOCAL|op->i1);
		break;
	case BSVM2_OP_MVL:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_LONG,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_LOCAL|op->i1);
		break;
	case BSVM2_OP_MVF:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_FLOAT,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_LOCAL|op->i1);
		break;
	case BSVM2_OP_MVD:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_DOUBLE,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_LOCAL|op->i1);
		break;
	case BSVM2_OP_MVA:
		BS2J2C_Emit_MovOp(ctx, BSVM2_OPZ_ADDRESS,
			BSVM2_VAM_LOCAL|op->i0, BSVM2_VAM_LOCAL|op->i1);
		break;
	}
//	if(tr->ops[i]->opfl&BSVM2_TRFL_CANTHROW)
}

int BS2J2C_TransTrace(BSVM2_TransContext *ctx,
	BSVM2_Trace *tr)
{
	void *p;
	char *fn, *fnth;
	int i, j, k;
	
	fn=BASM_GenSym();
//	if(tr->trfl&BSVM2_TRFL_CANTHROW)
//		fnth=BASM_GenSym();

//	BASM_BeginAssembly(NULL);
	
	BGBDT_MM_PrintPutPrintf(ctx->ob_func, "%s:\n", fn);

	for(i=0; i<tr->n_ops; i++)
	{
		BS2J2C_TransOpcode(ctx, tr, tr->ops[i]);
	}


#if 0
//	BS2J2C_JitTraceHandleTail(tr);

	if(tr->trfl&BSVM2_TRFL_CANTHROW)
	{
		basm_print("%s:\n", fnth);

		basm_print("mov dword [ecx+%d], 0x%08X\n",
			offsetof(BSVM2_Context, trace), (u32)(tr));

		basm_print("xor eax, eax\n");
		BS2J_EmitNormalReturn(tr);
	}
#endif

//	BASM_EndAssembly();

	return(0);
}

void BS2J2C_TransFunc(BSVM2_TransContext *ctx,
	BSVM2_ImageGlobal *vari)
{
	BSVM2_CodeBlock *cblk;
	int i, j, k, l;
	
	cblk=vi->cblk;
	if(!vi->cblk)
		return;

	BS2I_ImageGetFuncTrace(vari);

	BGBDT_MM_PrintPutPrintf(ctx->ob_func, "void %s()\n{\n",
		BS2J2C_GetVariQNameMangle(vari));
	
	for(i=0; i<cblk->ntrace; i++)
	{
		BS2J2C_TransTrace(ctx, cblk->trace[i]);
	}
	
	BGBDT_MM_PrintPutPrintf(ctx->ob_func, "}\n");
}


BSVM2_TransContext *BS2J2C_NewTransContext()
{
	BSVM2_TransContext *ctx;
	
	ctx=dtmAlloc("bsvm2_transcontext_t", sizeof(BSVM2_TransContext));
	
	ctx->ob_hdr=BGBDT_MM_NewDynamicStringPrinter(0);
	ctx->ob_topvar=BGBDT_MM_NewDynamicStringPrinter(0);
	ctx->ob_topfcn=BGBDT_MM_NewDynamicStringPrinter(0);
	ctx->ob_func=BGBDT_MM_NewDynamicStringPrinter(0);
	
	return(ctx);
}
