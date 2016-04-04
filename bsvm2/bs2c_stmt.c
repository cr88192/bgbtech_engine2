#include <bteifgl.h>

int BS2C_CompileErrorGetSourceLine(BS2CC_CompileContext *ctx)
{
	int fi;
	int i;
	
	for(i=0; i<ctx->nsrcfns; i++)
	{
		if(!strcmp(ctx->srcfns[i], ctx->srcfn))
			break;
	}
	if(i>=ctx->nsrcfns)
	{
		i=ctx->nsrcfns++;
		ctx->srcfns[i]=BS2P_StrSym(ctx, ctx->srcfn);
	}
	fi=i;
	
	return((fi<<20)|(ctx->srcln));
}

void BS2C_CompileError(BS2CC_CompileContext *ctx, int errn)
{
	int i, j;
	
	j=BS2C_CompileErrorGetSourceLine(ctx);
	
	if(((errn&BS2CC_ERRN_TMASK)==BS2CC_ERRN_WARNING) ||
		((errn&BS2CC_ERRN_TMASK)==BS2CC_ERRN_HINT))
	{
		if(ctx->ncwarn<256)
		{
			i=ctx->ncwarn++;
			ctx->cwarn[i]=errn;
			ctx->cwarnln[i]=j;
			return;
		}
		return;
	}
	
	if((errn&BS2CC_ERRN_TMASK)==BS2CC_ERRN_ERROR)
	{
		if(ctx->ncerr<256)
		{
			i=ctx->ncerr++;
			ctx->cerr[i]=errn;
			ctx->cerrln[i]=j;
			return;
		}
		
		if(!ctx->ncfatal)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_FATALERRCNT);
			return;
		}
		return;
	}

	if((errn&BS2CC_ERRN_TMASK)==BS2CC_ERRN_FATAL)
	{
		if(ctx->ncfatal<16)
		{
			i=ctx->ncfatal++;
			ctx->cfatal[i]=errn;
			ctx->cfatalln[i]=j;
			return;
		}
	}
}

void BS2C_CaseError(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_CASEERROR);
}

void BS2C_StubError(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_STUBERROR);
}

void BS2C_WarnConstRange(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_CONSTRANGE);
}

void BS2C_ErrDivZero(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_DIVZERROR);
}

void BS2C_ErrNoDecl(BS2CC_CompileContext *ctx, char *name)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRNODECL);
}

void BS2C_ErrTooFewArgs(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRTOOFEWARGS);
}

void BS2C_ErrTooManyArgs(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRTOOMANYARGS);
}

void BS2C_CompileStmtVarInit(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vi, dtVal expr)
{
//	char tb[256];
	dtVal nn, nt;
	char *name;
	int bty;
	int i;

//	name=BS2P_GetAstNodeAttrS(expr, "name");
//	nt=BS2P_GetAstNodeAttr(expr, "type");
//	bty=BS2C_InferTypeBaseType(ctx, nt);

//	vi->name=BS2P_StrSym(ctx, name);
//	vi->bty=bty;
}

void BS2C_CompileStmtVar(BS2CC_CompileContext *ctx, dtVal expr)
{
	BS2CC_VarInfo *vi;

//	vi=BS2C_AllocVarInfo(ctx);
//	i=ctx->frm->nlocals++;
//	ctx->frm->locals[i]=vi;
	
//	BS2C_CompileStmtVarInit(ctx, vi, expr);
}

void BS2C_CompileStmtSwitchDiR(BS2CC_CompileContext *ctx,
	int *swtgt, int *swval, int swn, int swdfl)
{
	int tl, tg;
	int i, j, k;
	
//	if(swn<7)
//	if(swn<5)
//	if(swn<3)
	if(swn<4)
	{
		for(i=0; i<swn; i++)
		{
//			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI);
//			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPIC);
//			BS2C_EmitOpcodeSCx(ctx, swval[i]);
//			BS2C_EmitOpcode(ctx, BSVM2_OP_JEQ);
//			BS2C_EmitTempJAddr(ctx, swtgt[i]);

			BS2C_EmitOpcode(ctx, BSVM2_OP_DCJEQIC);
			BS2C_EmitOpcodeSCx(ctx, swval[i]);
			BS2C_EmitTempJAddr(ctx, swtgt[i]);
		}

//		BS2C_CompilePop(ctx);
//		BS2C_EmitOpcode(ctx, BSVM2_OP_POPI);
		BS2C_EmitTempJump(ctx, swdfl);
		return;
	}

	tl=BS2C_GenTempLabel(ctx);
	tg=BS2C_GenTempLabel(ctx);

	i=swn>>1;

//	BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI);
//	BS2C_EmitOpcode(ctx, BSVM2_OP_CMPIC);
//	BS2C_EmitOpcodeSCx(ctx, swval[i]);
//	BS2C_EmitOpcode(ctx, BSVM2_OP_JLT);
//	BS2C_EmitTempJAddr(ctx, tl);

	BS2C_EmitOpcode(ctx, BSVM2_OP_DCJLTIC);
	BS2C_EmitOpcodeSCx(ctx, swval[i]);
	BS2C_EmitTempJAddr(ctx, tl);

//	BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI);
//	BS2C_EmitOpcode(ctx, BSVM2_OP_CMPIC);
//	BS2C_EmitOpcodeSCx(ctx, swval[i]);
//	BS2C_EmitOpcode(ctx, BSVM2_OP_JGT);
//	BS2C_EmitTempJAddr(ctx, tg);

	BS2C_EmitOpcode(ctx, BSVM2_OP_DCJGTIC);
	BS2C_EmitOpcodeSCx(ctx, swval[i]);
	BS2C_EmitTempJAddr(ctx, tg);

	BS2C_EmitTempJump(ctx, swtgt[i]);
//	BS2C_EmitTempJAddr(ctx, swtgt[i]);

	BS2C_EmitTempLabelB(ctx, tl);
	BS2C_CompileStmtSwitchDiR(ctx, swtgt, swval, i, swdfl);

	BS2C_EmitTempLabelB(ctx, tg);
	j=i+1;
	BS2C_CompileStmtSwitchDiR(ctx, swtgt+j, swval+j, swn-j, swdfl);
}

void BS2C_CompileStmtSwitch(BS2CC_CompileContext *ctx, dtVal expr)
{
	int sw_tgt[4096];
	int sw_val[4096];

	int sw2_tgt[4096];
	int sw2_val[4096];

	int swn, swdfl, swgdfl;
	
	dtVal n0, n1, n2, n3;
	dtVal cc, nt, nf, ni, ns;
	s64 li, lj, lk;
	int cty, ln;
	char *tag, *fn;
	int t0, t1, t2, t3;
	int l0, l1, l2, l3;
	int i, j, k, l;


	cc=BS2P_GetAstNodeAttr(expr, "cond");
	nt=BS2P_GetAstNodeAttr(expr, "then");

	cc=BS2C_ReduceExpr(ctx, cc);
	cty=BS2C_InferExpr(ctx, cc);
	
	if(!BS2C_InferTypeSmallIntP(ctx, cty))
	{
		BS2C_CaseError(ctx);
		return;
	}

	nf=BS2P_GetAstNodeAttr(nt, "value");

	swn=0; swdfl=-1;
	l=dtvArrayGetSize(nf);
	for(i=0; i<l; i++)
	{
		n0=dtvArrayGetIndexDtVal(nf, i);
//		BS2C_CompileStatement(ctx, n0);
//		if(ctx->ncfatal)
//			break;
		tag=BS2P_GetAstNodeTag(n0);
		if(!strcmp(tag, "case"))
		{
			j=BS2P_GetAstNodeAttrI(n0, "value");
			sw_tgt[swn]=BS2C_GenTempLabel(ctx);
			sw_val[swn]=j;
			swn++;
			continue;
		}
		if(!strcmp(tag, "case_default"))
		{
			swdfl=BS2C_GenTempLabel(ctx);
			continue;
		}
	}

	for(i=0; i<swn; i++)
		{ sw2_tgt[i]=sw_tgt[i]; sw2_val[i]=sw_val[i]; }
	for(i=0; i<swn; i++)
		for(j=i+1; j<swn; j++)
	{
		if(sw2_val[j]<sw2_val[i])
		{
			k=sw2_val[i]; sw2_val[i]=sw2_val[j]; sw2_val[j]=k;
			k=sw2_tgt[i]; sw2_tgt[i]=sw2_tgt[j]; sw2_tgt[j]=k;
		}
	}

	t0=BS2C_GenTempLabel(ctx);
	swgdfl=0;

	if(swdfl<0)
	{
		swdfl=BS2C_GenTempLabel(ctx);
		swgdfl=1;
	}

	l0=ctx->frm->brkstkpos++;
	ctx->frm->brkstk[l0]=t0;

	BS2C_CompileExpr(ctx, cc, cty);

	BS2C_CompileStmtSwitchDiR(ctx, sw2_tgt, sw2_val, swn, swdfl);
//	BS2C_CompileNoexPop(ctx);

#if 0
	for(i=0; i<swn; i++)
	{
//		BS2C_CompileDup(ctx);

		BS2C_EmitOpcode(ctx, BSVM2_OP_DUPI);

		BS2C_EmitOpcode(ctx, BSVM2_OP_CMPIC);
		BS2C_EmitOpcodeSCx(ctx, sw_val[i]);

		BS2C_EmitOpcode(ctx, BSVM2_OP_JEQ);
		BS2C_EmitTempJAddr(ctx, sw_tgt[i]);
	}

//	BS2C_CompilePop(ctx);
	BS2C_EmitTempJump(ctx, swdfl);
	BS2C_CompileNoexPop(ctx);
#endif

//	BS2C_EmitTempLabelB(ctx, t0);
//	if(l3!=1)
//		BS2C_CompileTempJmpElse(ctx, cc, t1);
//	BS2C_CompileStatement(ctx, nt);
//	BS2C_EmitTempJump(ctx, t0);

	l=dtvArrayGetSize(nf); k=0;
	for(i=0; i<l; i++)
	{
		n0=dtvArrayGetIndexDtVal(nf, i);
		tag=BS2P_GetAstNodeTag(n0);
		if(!strcmp(tag, "case"))
		{
			if(k)
				BS2C_CompileNoexPushI(ctx);
			BS2C_EmitTempLabelB(ctx, sw_tgt[k++]);
			BS2C_CompilePop(ctx);
			continue;
		}
		if(!strcmp(tag, "case_default"))
		{
			if(k)
				BS2C_CompileNoexPushI(ctx);
			BS2C_EmitTempLabelB(ctx, swdfl);
			BS2C_CompilePop(ctx);
			continue;
		}
		BS2C_CompileStatement(ctx, n0);
		if(ctx->ncfatal)
			break;
	}

	if(swgdfl)
	{
		BS2C_CompileNoexPushI(ctx);
		BS2C_EmitTempLabelB(ctx, swdfl);
		BS2C_CompilePop(ctx);
	}

	BS2C_EmitTempLabelB(ctx, t0);
	ctx->frm->brkstkpos=l0;
}

void BS2C_CompileStatement(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal n0, n1, n2, n3;
	dtVal cc, nt, nf, ni, ns;
	s64 li, lj, lk;
	int cty, ln;
	char *tag, *fn;
	int t0, t1, t2, t3;
	int l0, l1, l2, l3;
	int i, j, k, l;

	if(ctx->ncfatal)
		return;

	ctx->frm->wasret=0;

	if(dtvIsArrayP(expr))
	{
		l=dtvArrayGetSize(expr);
		for(i=0; i<l; i++)
		{
			n0=dtvArrayGetIndexDtVal(expr, i);
			BS2C_CompileStatement(ctx, n0);
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
		BS2C_CompileStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "var"))
	{
		BS2C_CompileStmtVar(ctx, expr);
		return;
	}
	
	if(!strcmp(tag, "vars"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		l=dtvArrayGetSize(n0);
		for(i=0; i<l; i++)
		{
			n1=dtvArrayGetIndexDtVal(n0, i);
			BS2C_CompileStmtVar(ctx, n1);
		}
		return;
	}

	if(!strcmp(tag, "switch"))
	{
		BS2C_CompileStmtSwitch(ctx, expr);
		return;
	}

	if(!strcmp(tag, "if"))
	{
		cc=BS2P_GetAstNodeAttr(expr, "cond");
		nt=BS2P_GetAstNodeAttr(expr, "then");
		nf=BS2P_GetAstNodeAttr(expr, "else");

		cc=BS2C_ReduceExpr(ctx, cc);

		i=BS2C_CompileGetCondBool(ctx, cc);
		if(i==1)
		{
			BS2C_CompileStatement(ctx, nt);
			return;
		}

		if(i==0)
		{
			if(dtvTrueP(nf))
				BS2C_CompileStatement(ctx, nf);
			return;
		}

		if(dtvTrueP(nf))
		{
			t1=BS2C_GenTempLabel(ctx);
			t2=BS2C_GenTempLabel(ctx);

			BS2C_CompileTempJmpElse(ctx, cc, t1);

			BS2C_CompileStatement(ctx, nt);
			BS2C_EmitTempJump(ctx, t2);

			BS2C_EmitTempLabelB(ctx, t1);
			BS2C_CompileStatement(ctx, nf);

			BS2C_EmitTempLabelB(ctx, t2);
			return;
		}

		t1=BS2C_GenTempLabel(ctx);
		BS2C_CompileTempJmpElse(ctx, cc, t1);
		BS2C_CompileStatement(ctx, nt);
		BS2C_EmitTempLabelB(ctx, t1);
		return;
	}

	if(!strcmp(tag, "for"))
	{
		ni=BS2P_GetAstNodeAttr(expr, "init");
		cc=BS2P_GetAstNodeAttr(expr, "cond");
		ns=BS2P_GetAstNodeAttr(expr, "step");
		nt=BS2P_GetAstNodeAttr(expr, "then");

		ni=BS2C_ReduceExpr(ctx, ni);
		cc=BS2C_ReduceExpr(ctx, cc);
		ns=BS2C_ReduceExpr(ctx, ns);

		t0=BS2C_GenTempLabel(ctx);
		t1=BS2C_GenTempLabel(ctx);

		l0=ctx->frm->constkpos++;
		l1=ctx->frm->brkstkpos++;
		ctx->frm->constk[l0]=t0;
		ctx->frm->brkstk[l1]=t1;

		BS2C_CompileExpr(ctx, ni, BSVM2_OPZ_VOID);
		BS2C_EmitTempLabelB(ctx, t0);
		BS2C_CompileTempJmpElse(ctx, cc, t1);
		BS2C_CompileStatement(ctx, nt);
		BS2C_CompileExpr(ctx, ns, BSVM2_OPZ_VOID);
		BS2C_EmitTempJump(ctx, t0);
		BS2C_EmitTempLabelB(ctx, t1);
		
		ctx->frm->constkpos=l0;
		ctx->frm->brkstkpos=l1;
		return;
	}

	if(!strcmp(tag, "while"))
	{
		cc=BS2P_GetAstNodeAttr(expr, "cond");
		nt=BS2P_GetAstNodeAttr(expr, "then");

		cc=BS2C_ReduceExpr(ctx, cc);
		l3=BS2C_CompileGetCondBool(ctx, cc);

		if(l3==0)
		{
			return;
		}

		t0=BS2C_GenTempLabel(ctx);
		t1=BS2C_GenTempLabel(ctx);

		l0=ctx->frm->constkpos++;
		l1=ctx->frm->brkstkpos++;
		ctx->frm->constk[l0]=t0;
		ctx->frm->brkstk[l1]=t1;

		BS2C_EmitTempLabelB(ctx, t0);
		if(l3!=1)
			BS2C_CompileTempJmpElse(ctx, cc, t1);
		BS2C_CompileStatement(ctx, nt);
		BS2C_EmitTempJump(ctx, t0);
		BS2C_EmitTempLabelB(ctx, t1);
		
		ctx->frm->constkpos=l0;
		ctx->frm->brkstkpos=l1;
		return;
	}

	if(!strcmp(tag, "do_while"))
	{
		cc=BS2P_GetAstNodeAttr(expr, "cond");
		nt=BS2P_GetAstNodeAttr(expr, "then");

		cc=BS2C_ReduceExpr(ctx, cc);
		l3=BS2C_CompileGetCondBool(ctx, cc);

		t0=BS2C_GenTempLabel(ctx);
		t1=BS2C_GenTempLabel(ctx);

		l0=ctx->frm->constkpos++;
		l1=ctx->frm->brkstkpos++;
		ctx->frm->constk[l0]=t0;
		ctx->frm->brkstk[l1]=t1;

		BS2C_EmitTempLabelB(ctx, t0);
		BS2C_CompileStatement(ctx, nt);
		BS2C_CompileTempJmpThen(ctx, cc, t0);
		BS2C_EmitTempLabelB(ctx, t1);
		
		ctx->frm->constkpos=l0;
		ctx->frm->brkstkpos=l1;
		return;
	}

	if(!strcmp(tag, "break"))
	{
		i=BS2P_GetAstNodeAttrI(expr, "value");
		if(i<=0)i=1;
		l1=ctx->frm->brkstkpos-i;
		t1=ctx->frm->brkstk[l1];
		BS2C_EmitTempJump(ctx, t1);
		return;
	}

	if(!strcmp(tag, "continue"))
	{
		i=BS2P_GetAstNodeAttrI(expr, "value");
		if(i<=0)i=1;
		l1=ctx->frm->constkpos-i;
		t1=ctx->frm->constk[l1];
		BS2C_EmitTempJump(ctx, t1);
		return;
	}

	if(!strcmp(tag, "return"))
	{
		nt=BS2P_GetAstNodeAttr(expr, "value");
		nt=BS2C_ReduceExpr(ctx, nt);

		cty=ctx->frm->func->rty;
		
		ctx->frm->wasret=1;
		
		if(dtvNullP(nt))
		{
			if(cty==BSVM2_OPZ_VOID)
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETV);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_InferTypeSmallIntP(ctx, cty))
			{
//				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
//				BS2C_EmitOpcode(ctx, BSVM2_OP_RETI);

				BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
				BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, 0);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_InferTypeSmallLongP(ctx, cty))
			{
//				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHL);
//				BS2C_EmitOpcode(ctx, BSVM2_OP_RETL);
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
				BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG, 0);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_InferTypeSmallFloatP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETF);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_InferTypeSmallDoubleP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETD);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_InferTypeAddressP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETA);
				ctx->frm->newtrace=1;
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

#if 1
		if(dtvIsSmallLongP(nt))
		{
			li=dtvUnwrapLong(nt);
		
			if(BS2C_InferTypeSmallIntP(ctx, cty))
			{
				if(li>0)
				{
					BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
					BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_UINT, li);
					ctx->frm->newtrace=1;
				}else
				{
					BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
					BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, li);
					ctx->frm->newtrace=1;
				}
				return;
			}

			if(BS2C_InferTypeSmallLongP(ctx, cty))
			{
				if(li>0)
				{
					BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
					BS2C_EmitOpcodeUZx(ctx, BSVM2_OPZ_ULONG, li);
					ctx->frm->newtrace=1;
				}else
				{
					BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
					BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG, li);
					ctx->frm->newtrace=1;
				}
				return;
			}
		}
#endif

		BS2C_CompileExpr(ctx, nt, cty);

		if(BS2C_InferTypeSmallIntP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETI);
			ctx->frm->newtrace=1;
			return;
		}
		if(BS2C_InferTypeSmallLongP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETL);
			ctx->frm->newtrace=1;
			return;
		}
		if(BS2C_InferTypeSmallFloatP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETF);
			ctx->frm->newtrace=1;
			return;
		}
		if(BS2C_InferTypeSmallDoubleP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETD);
			ctx->frm->newtrace=1;
			return;
		}
		if(BS2C_InferTypeAddressP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETA);
			ctx->frm->newtrace=1;
			return;
		}
		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "label"))
	{
		fn=BS2P_GetAstNodeAttrS(expr, "value");
		i=BS2C_GetNamedLabel(ctx, fn);
		BS2C_EmitTempLabelB(ctx, i);
//		BS2C_EmitTempJump(ctx, t1);
		return;
	}

	if(!strcmp(tag, "goto"))
	{
		fn=BS2P_GetAstNodeAttrS(expr, "value");
		i=BS2C_GetNamedLabel(ctx, fn);
		BS2C_EmitTempJump(ctx, i);
		return;
	}

	if(!strcmp(tag, "empty_block"))
	{
		return;
	}

	BS2C_CompileExpr(ctx, expr, BSVM2_OPZ_VOID);
}

BS2CC_VarInfo *BS2C_AllocVarInfo(BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *tmp;
	
	tmp=dtmAlloc("bs2cc_varinfo_t",
		sizeof(BS2CC_VarInfo));
	return(tmp);
}

BS2CC_CcFrame *BS2C_AllocCcFrame(BS2CC_CompileContext *ctx)
{
	BS2CC_CcFrame *tmp;
	
	tmp=dtmAlloc("bs2cc_ccframe_t",
		sizeof(BS2CC_CcFrame));
	return(tmp);
}

BS2CC_PkgFrame *BS2C_AllocPkgFrame(BS2CC_CompileContext *ctx)
{
	BS2CC_PkgFrame *tmp;
	
	tmp=dtmAlloc("bs2cc_pkgframe_t",
		sizeof(BS2CC_PkgFrame));
	return(tmp);
}

BS2CC_PkgFrame *BS2C_LookupPkgFrame(
	BS2CC_CompileContext *ctx, char *qname)
{
	BS2CC_PkgFrame *cur;

	if(!qname)
	{
		cur=ctx->pkg_first;
		while(cur)
		{
			if(!cur->qname)
				return(cur);
			cur=cur->next;
		}
		return(NULL);
	}

	cur=ctx->pkg_first;
	while(cur)
	{
		if(!cur->qname)
		{
			cur=cur->next;
			continue;
		}
		if(!strcmp(cur->qname, qname))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BS2CC_PkgFrame *BS2C_GetPkgFrame(
	BS2CC_CompileContext *ctx, char *qname)
{
	BS2CC_PkgFrame *pkg;
	
	pkg=BS2C_LookupPkgFrame(ctx, qname);
	if(pkg)
		return(pkg);
	
	pkg=BS2C_AllocPkgFrame(ctx);
	pkg->qname=BS2P_StrSym(ctx, qname);
	
	pkg->next=ctx->pkg_first;
	ctx->pkg_first=pkg;
	return(pkg);
}

BS2CC_PkgFrame *BS2C_EnterPkgFrame(
	BS2CC_CompileContext *ctx, char *name)
{
	char tb[256];
	BS2CC_PkgFrame *pkg;
	int i;
	
	if(ctx->pkg && ctx->pkg->qname)
	{
		sprintf(tb, "%s/%s", ctx->pkg->qname, name);
		pkg=BS2C_GetPkgFrame(ctx, tb);
	}else
	{
		pkg=BS2C_GetPkgFrame(ctx, name);
	}
	
	i=ctx->pkgstkpos++;
	ctx->pkgstk[i]=ctx->pkg;
	ctx->pkg=pkg;
	return(pkg);
}

void BS2C_ExitPkgFrame(
	BS2CC_CompileContext *ctx)
{
	int i;

	i=--ctx->pkgstkpos;
	ctx->pkg=ctx->pkgstk[i];
}

void BS2C_CompileSetupVarInfo(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vi, dtVal expr)
{
	char tb[256];
	dtVal nn, nt;
	s64 bmfl;
	char *name, *tag;
	int bty;
	int i;

	tag=BS2P_GetAstNodeTag(expr);

	if(!strcmp(tag, "var"))
	{
		name=BS2P_GetAstNodeAttrS(expr, "name");
		bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
		nt=BS2P_GetAstNodeAttr(expr, "type");

//		bty=BS2C_InferTypeBaseType(ctx, nt);
		bty=BS2C_InferTypeExtType(ctx, nt);

		vi->name=BS2P_StrSym(ctx, name);
		vi->bty=bty;
		vi->bmfl=bmfl;
		vi->typeExp=nt;
		return;
	}

	if(!strcmp(tag, "vararg"))
	{
		vi->name=NULL;
		vi->bty=BS2CC_TYZ_VARARG;
//		vi->typeExp=nt;
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileFunVar(BS2CC_CompileContext *ctx, dtVal expr)
{
	BS2CC_VarInfo *vi;
	int i;

	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->frm->nlocals++;
	ctx->frm->locals[i]=vi;
	
	BS2C_CompileSetupVarInfo(ctx, vi, expr);
}

void BS2C_CompileFunVarStatement(BS2CC_CompileContext *ctx, dtVal expr)
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
			BS2C_CompileFunVarStatement(ctx, n0);
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
		BS2C_CompileFunVarStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "var"))
	{
		BS2C_CompileFunVar(ctx, expr);
		return;
	}
	
	if(!strcmp(tag, "vars"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		l=dtvArrayGetSize(n0);
		for(i=0; i<l; i++)
		{
			n1=dtvArrayGetIndexDtVal(n0, i);
			BS2C_CompileFunVar(ctx, n1);
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
		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_CompileFunVarStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "if"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "then");
		n1=BS2P_GetAstNodeAttr(expr, "else");
		BS2C_CompileFunVarStatement(ctx, n0);
//		if(!dtvNullP(n1))
		if(dtvTrueP(n1))
			BS2C_CompileFunVarStatement(ctx, n1);
		return;
	}

	if(!strcmp(tag, "while"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_CompileFunVarStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "do_while"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_CompileFunVarStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "switch"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_CompileFunVarStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "assign"))
		return;
	if(!strcmp(tag, "break"))
		return;
	if(!strcmp(tag, "call"))
		return;
	if(!strcmp(tag, "case"))
		return;
	if(!strcmp(tag, "case_default"))
		return;
	if(!strcmp(tag, "continue"))
		return;
	if(!strcmp(tag, "default"))
		return;
	if(!strcmp(tag, "goto"))
		return;
	if(!strcmp(tag, "label"))
		return;
	if(!strcmp(tag, "return"))
		return;
	if(!strcmp(tag, "throw"))
		return;

	if(!strcmp(tag, "empty_block"))
		return;

	BS2C_CaseError(ctx);
}

void BS2C_CompileStructVar(BS2CC_CompileContext *ctx, dtVal expr)
{
	char tb[256];
	BS2CC_VarInfo *vi;
	int i;

	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->obj->nargs++;
	ctx->obj->args[i]=vi;

	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->vitype=BS2CC_VITYPE_STRVAR;

	vi->pkg=ctx->pkg;
	vi->obj=ctx->obj;

	BS2C_CompileSetupVarInfo(ctx, vi, expr);

	if(vi->bmfl&BS2CC_TYFL_STATIC)
		vi->vitype=BS2CC_VITYPE_GBLVAR;

	sprintf(tb, "%s/%s", ctx->obj->qname, vi->name);
	vi->qname=BS2P_StrSym(ctx, tb);
}

void BS2C_CompileStructFunc(BS2CC_CompileContext *ctx, dtVal expr)
{
	char tb[256];
	dtVal n0, n1, n2, n3;
	dtVal nn, nt, na, nb;
	BS2CC_VarInfo *vi;
	BS2CC_CcFrame *frm;
	char *name;
	s64 bmfl;
	int bty;
	int i, j, k, l;

	name=BS2P_GetAstNodeAttrS(expr, "name");
	bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
	nt=BS2P_GetAstNodeAttr(expr, "type");
	na=BS2P_GetAstNodeAttr(expr, "args");
	nb=BS2P_GetAstNodeAttr(expr, "body");
	
	if(dtvTrueP(nt))
	{
		bty=BS2C_InferTypeBaseType(ctx, nt);
	}else
	{
		bty=BS2CC_TYZ_VOID;
	}
	
	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->obj->nargs++;
	ctx->obj->args[i]=vi;

	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->vitype=BS2CC_VITYPE_STRFUNC;
	if(bmfl&BS2CC_TYFL_STATIC)
		vi->vitype=BS2CC_VITYPE_GBLFUNC;
	
	vi->name=BS2P_StrSym(ctx, name);
	vi->bty=BSVM2_OPZ_ADDRESS;
	vi->rty=bty;
	vi->typeExp=nt;
	vi->bmfl=bmfl;

	vi->pkg=ctx->pkg;
	vi->obj=ctx->obj;

	if(ctx->obj && ctx->obj->qname)
	{
		sprintf(tb, "%s/%s", ctx->obj->qname, name);
		vi->qname=BS2P_StrSym(ctx, tb);
	}else
	{
		vi->qname=vi->name;
	}

	if(dtvIsArrayP(na))
	{
		vi->nargs=0;
		l=dtvArrayGetSize(na);
		for(i=0; i<l; i++)
		{
			n0=dtvArrayGetIndexDtVal(na, i);
			BS2C_CompileTopFuncArg(ctx, vi, n0);
		}
	}else if(dtvTrueP(na))
	{
		vi->nargs=0;
		BS2C_CompileTopFuncArg(ctx, vi, na);
	}else
	{
		vi->nargs=0;
	} 
	
	vi->bodyExp=nb;
}


void BS2C_CompileStructVarStatement(BS2CC_CompileContext *ctx, dtVal expr)
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
			BS2C_CompileStructVarStatement(ctx, n0);
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

	if(!strcmp(tag, "block") || !strcmp(tag, "psblock"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_CompileStructVarStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "var"))
	{
		BS2C_CompileStructVar(ctx, expr);
		return;
	}
	
	if(!strcmp(tag, "vars"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		l=dtvArrayGetSize(n0);
		for(i=0; i<l; i++)
		{
			n1=dtvArrayGetIndexDtVal(n0, i);
			BS2C_CompileStructVar(ctx, n1);
		}
		return;
	}

	if(!strcmp(tag, "func"))
	{
		BS2C_CompileStructFunc(ctx, expr);
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileSetupTopVarInfo(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vi, dtVal expr)
{
	char tb[256];
	dtVal nn, nt;
	s64 bmfl;
	char *name;
	int bty;
	int i;

	name=BS2P_GetAstNodeAttrS(expr, "name");
	bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
	nt=BS2P_GetAstNodeAttr(expr, "type");

	bty=BS2C_InferTypeBaseType(ctx, nt);

	vi->name=BS2P_StrSym(ctx, name);
	vi->bty=bty;
	vi->bmfl=bmfl;
	vi->typeExp=nt;

	if(ctx->pkg)
	{
		vi->pkg=ctx->pkg;
		vi->pknext=ctx->pkg->vars;
		ctx->pkg->vars=vi;
	}
	
	if(ctx->pkg && ctx->pkg->qname)
	{
		sprintf(tb, "%s/%s", ctx->pkg->qname, name);
		vi->qname=BS2P_StrSym(ctx, tb);
	}else
	{
		vi->qname=vi->name;
	}
}

void BS2C_CompileTopVar(BS2CC_CompileContext *ctx, dtVal expr)
{
	BS2CC_VarInfo *vi;
	int i;

	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->vitype=BS2CC_VITYPE_GBLVAR;
	
	BS2C_CompileSetupTopVarInfo(ctx, vi, expr);
}

void BS2C_CompileTopFuncArg(BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *func, dtVal expr)
{
	BS2CC_VarInfo *vi;
	int i;

	vi=BS2C_AllocVarInfo(ctx);
	i=func->nargs++;
	func->args[i]=vi;

	BS2C_CompileSetupVarInfo(ctx, vi, expr);
}

void BS2C_CompileTopFunc(BS2CC_CompileContext *ctx, dtVal expr)
{
	char tb[256];
	dtVal n0, n1, n2, n3;
	dtVal nn, nt, na, nb;
	BS2CC_VarInfo *vi;
	BS2CC_CcFrame *frm;
	char *name;
	int bty;
	int i, j, k, l;

	name=BS2P_GetAstNodeAttrS(expr, "name");
	nt=BS2P_GetAstNodeAttr(expr, "type");
	na=BS2P_GetAstNodeAttr(expr, "args");
	nb=BS2P_GetAstNodeAttr(expr, "body");
	
	bty=BS2C_InferTypeBaseType(ctx, nt);
	
	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->vitype=BS2CC_VITYPE_GBLFUNC;
	
	vi->name=BS2P_StrSym(ctx, name);
	vi->bty=BSVM2_OPZ_ADDRESS;
	vi->rty=bty;
	vi->typeExp=nt;
	
	if(ctx->pkg)
	{
		vi->pkg=ctx->pkg;
		vi->pknext=ctx->pkg->vars;
		ctx->pkg->vars=vi;
	}
	
	if(ctx->pkg && ctx->pkg->qname)
	{
		sprintf(tb, "%s/%s", ctx->pkg->qname, name);
		vi->qname=BS2P_StrSym(ctx, tb);
	}else
	{
		vi->qname=vi->name;
	}

	if(dtvIsArrayP(na))
	{
		vi->nargs=0;
		l=dtvArrayGetSize(na);
		for(i=0; i<l; i++)
		{
			n0=dtvArrayGetIndexDtVal(na, i);
			BS2C_CompileTopFuncArg(ctx, vi, n0);
		}
	}else if(dtvTrueP(na))
	{
		vi->nargs=0;
		BS2C_CompileTopFuncArg(ctx, vi, na);
	}else
	{
		vi->nargs=0;
	}
	 	
	vi->bodyExp=nb;
}

void BS2C_CompileFuncBody(BS2CC_CompileContext *ctx, BS2CC_VarInfo *func)
{
	BS2CC_CcFrame *frm;
	int i, j, k, l;

	frm=BS2C_AllocCcFrame(ctx);
	
	i=ctx->frmstackpos++;
	ctx->frmstack[i]=ctx->frm;
	ctx->frm=frm;
	func->body=frm;
	frm->func=func;
	frm->ctx=ctx;

	BS2C_CompileFunVarStatement(ctx,
		func->bodyExp);
	
	frm->bargs=frm->nlocals;
	for(j=0; j<func->nargs; j++)
	{
		k=frm->nlocals++;
		frm->locals[k]=func->args[j];
	}
		
	BS2C_CompileStatement(ctx,
		func->bodyExp);
	
	if(!ctx->frm->wasret)
	{
		BS2C_EmitReturnV(ctx);
	}
	
	BS2C_FixupLabels(ctx);
	
	frm->szt=frm->ct-frm->cts;
	
	ctx->frm=ctx->frmstack[i];
	ctx->frmstackpos=i;
}

void BS2C_CompileTopStruct(BS2CC_CompileContext *ctx, dtVal expr)
{
	char tb[256];
	dtVal n0, n1, n2, n3;
	dtVal nn, nt, na, nb, ne, ni;
	BS2CC_VarInfo *vi;
	BS2CC_CcFrame *frm;
	char *name, *tag;
	int bty;
	int i, j, k, l;

	name=BS2P_GetAstNodeAttrS(expr, "name");
	ne=BS2P_GetAstNodeAttr(expr, "exts");
	ni=BS2P_GetAstNodeAttr(expr, "impl");
	nb=BS2P_GetAstNodeAttr(expr, "body");
	tag=BS2P_GetAstNodeTag(expr);
	
	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->vitype=BS2CC_VITYPE_STRUCT;
	if(!strcmp(tag, "class"))
		vi->vitype=BS2CC_VITYPE_CLASS;
	if(!strcmp(tag, "iface"))
		vi->vitype=BS2CC_VITYPE_IFACE;
	
	vi->name=BS2P_StrSym(ctx, name);
	vi->bty=BSVM2_OPZ_ADDRESS;

	vi->extsExp=ne;
	vi->implExp=ni;

	if(ctx->pkg)
	{
		vi->pkg=ctx->pkg;
		vi->pknext=ctx->pkg->vars;
		ctx->pkg->vars=vi;
	}
	
	if(ctx->pkg && ctx->pkg->qname)
	{
		sprintf(tb, "%s/%s", ctx->pkg->qname, name);
		vi->qname=BS2P_StrSym(ctx, tb);
	}else
	{
		vi->qname=vi->name;
	}

	j=ctx->objstkpos++;
	ctx->objstk[j]=ctx->obj;
	ctx->obj=vi;

	BS2C_CompileStructVarStatement(ctx, nb);
	
	ctx->objstkpos=j;
	ctx->obj=ctx->objstk[j];
}


BTEIFGL_API void BS2C_CompileTopStatement(
	BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal n0, n1, n2, n3;
	char *tag, *fn, *s0;
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
			BS2C_CompileTopStatement(ctx, n0);
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

	if(!strcmp(tag, "psblock"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");

		if(!ctx->pkgstkpos)
		{
			BS2C_EnterPkgFrame(ctx, NULL);
			BS2C_CompileTopStatement(ctx, n0);
			BS2C_ExitPkgFrame(ctx);
		}else
		{
			BS2C_CompileTopStatement(ctx, n0);
		}
		return;
	}

	if(!strcmp(tag, "package"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "body");
		s0=BS2P_GetAstNodeAttrS(expr, "name");
		BS2C_EnterPkgFrame(ctx, s0);
		BS2C_CompileTopStatement(ctx, n0);
		BS2C_ExitPkgFrame(ctx);
		return;
	}

	if(!strcmp(tag, "import"))
	{
//		n0=BS2P_GetAstNodeAttr(expr, "body");
		s0=BS2P_GetAstNodeAttrS(expr, "name");
		
		for(i=0; i<ctx->pkg->nimps; i++)
		{
			if(!strcmp(ctx->pkg->imps[i]->qname, s0))
				break;
		}
		
		if(i<ctx->pkg->nimps)
			return;
		
		i=ctx->pkg->nimps++;
		ctx->pkg->imps[i]=BS2C_GetPkgFrame(ctx, s0);
		return;
	}

	if(!strcmp(tag, "var"))
	{
		BS2C_CompileTopVar(ctx, expr);
		return;
	}
	
	if(!strcmp(tag, "vars"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		l=dtvArrayGetSize(n0);
		for(i=0; i<l; i++)
		{
			n1=dtvArrayGetIndexDtVal(n0, i);
			BS2C_CompileTopVar(ctx, n1);
		}
		return;
	}

	if(!strcmp(tag, "func"))
	{
		BS2C_CompileTopFunc(ctx, expr);
		return;
	}
	
	if(!strcmp(tag, "struct") ||
		!strcmp(tag, "class") ||
		!strcmp(tag, "iface"))
	{
		BS2C_CompileTopStruct(ctx, expr);
		return;
	}
	
	BS2C_CaseError(ctx);
}

void BS2C_CompileRebuildVarType(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	char *s;

	if(dtvTrueP(vari->typeExp))
	{
		vari->bty=BS2C_InferTypeRefinedType(
			ctx, vari, vari->typeExp);
	}
	s=BS2C_GetTypeSig(ctx, vari->bty);
	vari->sig=BS2P_StrSym(ctx, s);
}

void BS2C_CompileRebuildFuncType(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	char tb[256];
	char *s, *t;
	int i, j, k, l;

	if(dtvTrueP(vari->typeExp))
	{
		vari->rty=BS2C_InferTypeRefinedType(
			ctx, vari, vari->typeExp);
	}
		
	for(i=0; i<vari->nargs; i++)
	{
		BS2C_CompileRebuildVarType(ctx, vari->args[i]);
	}
	
	t=tb;
	*t++='(';
	for(i=0; i<vari->nargs; i++)
	{
		s=vari->args[i]->sig;
		strcpy(t, s);
		t=t+strlen(t);
	}
	*t++=')';

	s=BS2C_GetTypeSig(ctx, vari->rty);
	strcpy(t, s);
	t=t+strlen(t);

	vari->sig=BS2P_StrSym(ctx, tb);
}

void BS2C_CompileRebuildStructType(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	BS2CC_VarInfo *vi2;
	dtVal ne;
	char *fn;
	int i, j, k;
	
	ne=vari->extsExp;
	if(dtvIsArrayP(ne))
	{
		ne=dtvArrayGetIndexDtVal(ne, 0);
		fn=BGBDT_TagStr_GetUtf8(ne);
		i=BS2C_LookupVariGlobal(ctx, vari, fn);
		if(i>=0)
			{ vi2=ctx->globals[i]; }
		else
			{ vi2=NULL; }
		vari->super=vi2;

	}else if(dtvTrueP(ne))
	{
		fn=BGBDT_TagStr_GetUtf8(ne);
		i=BS2C_LookupVariGlobal(ctx, vari, fn);
		if(i>=0)
			{ vi2=ctx->globals[i]; }
		else
			{ vi2=NULL; }
		vari->super=vi2;
	}
}

BTEIFGL_API void BS2C_CompileFuncs(
	BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *vari;
	int i;

	for(i=0; i<ctx->nglobals; i++)
	{
		vari=ctx->globals[i];

//		if(!dtvTrueP(vari->typeExp))
//			continue;

		if((vari->vitype==BS2CC_VITYPE_GBLVAR) ||
			(vari->vitype==BS2CC_VITYPE_STRVAR))
		{
			BS2C_CompileRebuildVarType(ctx, vari);
			continue;
		}

		if((vari->vitype==BS2CC_VITYPE_GBLFUNC) ||
			(vari->vitype==BS2CC_VITYPE_STRFUNC))
		{
			BS2C_CompileRebuildFuncType(ctx, vari);
			continue;
		}

		if((vari->vitype==BS2CC_VITYPE_STRUCT) ||
			(vari->vitype==BS2CC_VITYPE_CLASS) ||
			(vari->vitype==BS2CC_VITYPE_IFACE))
		{
			BS2C_CompileRebuildStructType(ctx, vari);
			continue;
		}
	}
	
	for(i=0; i<ctx->nglobals; i++)
	{
		vari=ctx->globals[i];
//		if(dtvNullP(vari->bodyExp))

		if((vari->vitype==BS2CC_VITYPE_GBLFUNC) ||
			(vari->vitype==BS2CC_VITYPE_STRFUNC))
		{
			if(!dtvTrueP(vari->bodyExp))
				continue;
			BS2C_CompileFuncBody(ctx, vari);
		}
	}
	
	if(vari->gid<65280)
	{
		vari->bty=vari->gid+256;
	}
}

BTEIFGL_API void BS2C_DumpErrors(
	BS2CC_CompileContext *ctx)
{
	int i, j, k;
	
	if(!ctx->ncerr && !ctx->ncfatal)
	{
		for(i=0; i<ctx->ncwarn; i++)
		{
			printf("%d:%d %04X\n",
				(ctx->cwarnln[i]>>20), ctx->cwarnln[i]&0xFFFFF,
				ctx->cwarn[i]);
		}
		printf("%d warnings\n", ctx->ncwarn);
	}
	
	for(i=0; i<ctx->ncerr; i++)
	{
		printf("%d:%d %04X\n",
			(ctx->cerrln[i]>>20), ctx->cerrln[i]&0xFFFFF,
			ctx->cerr[i]);
	}
	printf("%d errors\n", ctx->ncerr);

	if(ctx->ncfatal)
	{
		for(i=0; i<ctx->ncfatal; i++)
		{
			printf("%d:%d %04X\n",
				(ctx->cfatalln[i]>>20), ctx->cfatalln[i]&0xFFFFF,
				ctx->cfatal[i]);
		}
		printf("%d fatal errors\n", ctx->ncfatal);
	}
}
