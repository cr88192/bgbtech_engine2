#include <bteifgl.h>

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
//	bty=BS2C_TypeBaseType(ctx, nt);

//	vi->name=BS2P_StrSym(ctx, name);
//	vi->bty=bty;
}

void BS2C_CompileStmtVar(BS2CC_CompileContext *ctx, dtVal expr)
{
	BS2CC_VarInfo *vi, *vi2;
	dtVal nn, nt, ni;
	char *name;
	int bty, ix, sz, z;
	int i;

	name=BS2P_GetAstNodeAttrS(expr, "name");
//	nt=BS2P_GetAstNodeAttr(expr, "type");
//	bty=BS2C_TypeBaseType(ctx, nt);
	ni=BS2P_GetAstNodeAttr(expr, "init");

	ix=BS2C_LookupLocal(ctx, name);
	if(ix<0)
	{
		return;
	}

	vi=ctx->frm->locals[ix];
	bty=vi->bty;

	if(BS2C_TypeSizedArrayP(ctx, bty))
	{
		sz=BS2C_TypeGetArraySize(ctx, bty);
		z=BS2C_GetTypeBaseZ(ctx, bty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_IFXARR);
		BS2C_EmitOpcodeUCx(ctx, ix);
		BS2C_EmitOpcodeUZx(ctx, z, sz);

//		BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
//		BS2C_EmitOpcodeUZx(ctx, z, sz);
//		BS2C_CompileStoreName(ctx, name);

		if(ctx->frm->jcleanup<=0)
			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);

		return;
	}
	
	if(BS2C_TypeFixedStructP(ctx, bty))
	{
		vi2=BS2C_GetTypeObject(ctx, bty);
		i=BS2C_IndexFrameGlobal(ctx, vi2->gid);

		BS2C_EmitOpcode(ctx, BSVM2_OP_IFXOBJ);
		BS2C_EmitOpcodeUCx(ctx, ix);
		BS2C_EmitOpcodeUCx(ctx, i);

		if(dtvTrueP(ni))
		{
			BS2C_CompileExpr(ctx, ni, bty);
			BS2C_CompileStoreName(ctx, name);
			return;
		}

		if(ctx->frm->jcleanup<=0)
			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);
		return;
	}
	
	if(BS2C_TypeX128P(ctx, bty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_IFXX);
		BS2C_EmitOpcodeUCx(ctx, ix);

		if(dtvTrueP(ni))
		{
			BS2C_CompileExpr(ctx, ni, bty);
			BS2C_CompileStoreName(ctx, name);
			return;
		}

		if(ctx->frm->jcleanup<=0)
			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);
		return;
	}

	if(dtvTrueP(ni))
	{
		BS2C_CompileExpr(ctx, ni, bty);
		BS2C_CompileStoreName(ctx, name);
		return;
	}

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
	if(swn<3)
//	if(swn<4)
	{
//		if((swn==1) && (swtgt[1]>0) && (swval[1]==(swval[0]+1)))
//		{
//			BS2C_EmitTempJump(ctx, swtgt[0]);
//			return;
//		}

		if((swtgt[swn]>0) && (swval[swn]==(swval[swn-1]+1)))
		{
			for(i=0; i<(swn-1); i++)
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_DCJEQIC);
				BS2C_EmitOpcodeSCx(ctx, swval[i]);
				BS2C_EmitTempJAddr(ctx, swtgt[i]);
			}

			BS2C_EmitTempJump(ctx, swtgt[swn-1]);
			return;
		}
	
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

#if 0
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
#endif

#if 1
//	tl=BS2C_GenTempLabel(ctx);
	tg=BS2C_GenTempLabel(ctx);

	i=swn>>1;

	BS2C_EmitOpcode(ctx, BSVM2_OP_DCJGEIC);
	BS2C_EmitOpcodeSCx(ctx, swval[i]);
	BS2C_EmitTempJAddr(ctx, tg);

//	BS2C_EmitTempJump(ctx, tl);
//	BS2C_EmitTempLabelB(ctx, tl);

	BS2C_CompileStmtSwitchDiR(ctx, swtgt, swval, i, swdfl);

	BS2C_EmitTempLabelB(ctx, tg);
	BS2C_CompileStmtSwitchDiR(ctx, swtgt+i, swval+i, swn-i, swdfl);
#endif
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
	
	if(!BS2C_TypeSmallIntP(ctx, cty))
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
	sw2_tgt[swn]=0;
	sw2_val[swn]=0;

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
	dtVal cc, nt, nf, ni, ns, na;
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

	if(!strcmp(tag, "if") || !strcmp(tag, "ifelse"))
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

	if(!strcmp(tag, "delete"))
	{
		na=BS2P_GetAstNodeAttr(expr, "value");
//		nt=BS2C_ReduceExpr(ctx, nt);
		if(dtvIsArrayP(na))
		{
			l=dtvArrayGetSize(na);
			for(i=0; i<l; i++)
			{
				nt=dtvArrayGetIndexDtVal(na, i);
				nt=BS2C_ReduceExpr(ctx, nt);
				t0=BS2C_InferExpr(ctx, nt);
				BS2C_CompileExpr(ctx, nt, t0);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DELETE);
				BS2C_CompileExprPopType1(ctx);
			}
			return;
		}else
		{
			nt=BS2C_ReduceExpr(ctx, na);
			t0=BS2C_InferExpr(ctx, nt);
			BS2C_CompileExpr(ctx, nt, t0);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DELETE);
			BS2C_CompileExprPopType1(ctx);
			return;
		}
	}


	if(!strcmp(tag, "return"))
	{
		nt=BS2P_GetAstNodeAttr(expr, "value");
		nt=BS2C_ReduceExpr(ctx, nt);

		cty=ctx->frm->func->rty;
		
		ctx->frm->wasret=1;
		
		if(dtvNullP(nt))
		{
			if(ctx->frm->jcleanup>0)
			{
				BS2C_CompilePushDummy(ctx, cty);
				BS2C_EmitTempJump(ctx, ctx->frm->jcleanup);
				if(cty!=BSVM2_OPZ_VOID)
					BS2C_CompileNoexPop(ctx);
				return;
			}

			if(cty==BSVM2_OPZ_VOID)
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETV);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_TypeSmallIntP(ctx, cty))
			{
//				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHI);
//				BS2C_EmitOpcode(ctx, BSVM2_OP_RETI);

				BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
				BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_INT, 0);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_TypeSmallLongP(ctx, cty))
			{
//				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHL);
//				BS2C_EmitOpcode(ctx, BSVM2_OP_RETL);
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETC);
				BS2C_EmitOpcodeSZx(ctx, BSVM2_OPZ_LONG, 0);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_TypeSmallFloatP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHF);
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETF);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_TypeSmallDoubleP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHD);
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETD);
				ctx->frm->newtrace=1;
				return;
			}

			if(BS2C_TypeAddressP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
				BS2C_EmitOpcode(ctx, BSVM2_OP_RETA);
				ctx->frm->newtrace=1;
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(ctx->frm->jcleanup>0)
		{
//			BS2C_CompilePushDummy(ctx, cty);
			BS2C_CompileExpr(ctx, nt, cty);
			BS2C_EmitTempJump(ctx, ctx->frm->jcleanup);
			if(cty!=BSVM2_OPZ_VOID)
				BS2C_CompileNoexPop(ctx);
			return;
		}

#if 1
		if(dtvIsSmallLongP(nt))
		{
			li=dtvUnwrapLong(nt);
		
			if(BS2C_TypeSmallIntP(ctx, cty))
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

			if(BS2C_TypeSmallLongP(ctx, cty))
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

		if(BS2C_TypeSmallIntP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETI);
			ctx->frm->newtrace=1;
			ctx->frm->stackpos--;
			return;
		}
		if(BS2C_TypeSmallLongP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETL);
			ctx->frm->newtrace=1;
			ctx->frm->stackpos--;
			return;
		}
		if(BS2C_TypeSmallFloatP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETF);
			ctx->frm->newtrace=1;
			ctx->frm->stackpos--;
			return;
		}
		if(BS2C_TypeSmallDoubleP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETD);
			ctx->frm->newtrace=1;
			ctx->frm->stackpos--;
			return;
		}
		if(BS2C_TypeAddressP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_RETA);
			ctx->frm->newtrace=1;
			ctx->frm->stackpos--;
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

	if(!strcmp(tag, "ifelse"))
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
	if(!strcmp(tag, "assignop"))
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
	if(!strcmp(tag, "delete"))
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
