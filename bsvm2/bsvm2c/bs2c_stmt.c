/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


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
	BS2CC_VarInfo *vi, *vi2, *vi3;
	dtVal nn, nt, ni;
	char *name;
	int bty, cty, ix, ix2, sz, z;
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

	if((bty==BS2CC_TYZ_AUTOVAR) && dtvTrueP(ni))
	{
		cty=BS2C_InferExpr(ctx, ni);
		if(BS2C_TypeConcreteP(ctx, cty))
			{ vi->bty=cty; bty=cty; }
	}

	if(vi->bmfl&BS2CC_TYFL_DYNAMIC)
	{
		ix2=BS2C_LookupFrameGlobal(ctx, name);
		BS2C_EmitOpcode(ctx, BSVM2_OP_IFXDYV);
		BS2C_EmitOpcodeJx(ctx, ix, ix2);

		if(dtvTrueP(ni))
		{
			BS2C_CompileExpr(ctx, ni, bty);
			BS2C_CompileStoreName(ctx, name);
			return;
		}

		if(ctx->frm && ctx->frm->jcleanup<=0)
			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);
		return;
	}

#if 0
	if(vi->vitype==BS2CC_VITYPE_LCLFUNC)
	{
		ix2=BS2C_LookupFrameGlobal(ctx, name);
		BS2C_EmitOpcode(ctx, BSVM2_OP_IFXLFCN);
		BS2C_EmitOpcodeJx(ctx, ix, ix2);

		vi2=BS2C_GetFrameGlobalInfo(ctx, ix2);

		for(i=0; i<vi2->niface; i++)
		{
			vi3=vi->iface[i];
			n1=BGBDT_TagStr_Symbol(vi3->name);

			t0=BS2C_InferExprLocalIndex(ctx, n1);
			if(t0<0)
			{
				continue;
			}

			z=BS2C_GetTypeBaseZ(ctx, vi3->bty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DSTIXUZRL);
			BS2C_EmitOpcodeUZx(ctx, z, i);
			BS2C_EmitOpcodeUCx(ctx, t0);
		}

		if(ctx->frm && ctx->frm->jcleanup<=0)
			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);
		return;
	}
#endif

	if(BS2C_TypeSizedArrayP(ctx, bty))
	{
		cty=BS2C_TypeDerefType(ctx, bty);
		sz=BS2C_TypeGetArraySize(ctx, bty);
		z=BS2C_GetTypeBaseZ(ctx, cty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_IFXARR);
		BS2C_EmitOpcodeUZx(ctx, z, sz);
		BS2C_EmitOpcodeUCx(ctx, ix);

//		BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
//		BS2C_EmitOpcodeUZx(ctx, z, sz);
//		BS2C_CompileStoreName(ctx, name);

		if(ctx->frm && ctx->frm->jcleanup<=0)
			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);

		return;
	}
	
	if(BS2C_TypeFixedStructP(ctx, bty))
	{
		vi2=BS2C_GetTypeObject(ctx, bty);
		i=BS2C_IndexFrameGlobal(ctx, vi2->gid);

		BS2C_EmitOpcode(ctx, BSVM2_OP_IFXOBJ);
//		BS2C_EmitOpcodeUCx(ctx, ix);
//		BS2C_EmitOpcodeUCx(ctx, i);
		BS2C_EmitOpcodeJx(ctx, ix, i);

		if(dtvTrueP(ni))
		{
			BS2C_CompileExpr(ctx, ni, bty);
			BS2C_CompileStoreName(ctx, name);
			return;
		}

		if(ctx->frm && ctx->frm->jcleanup<=0)
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

		if(ctx->frm && ctx->frm->jcleanup<=0)
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
	int *swtgt, s64 *swval, int swn, int swdfl)
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


void BS2C_CompileStmtSwitchDiR_L(BS2CC_CompileContext *ctx,
	int *swtgt, s64 *swval, int swn, int swdfl)
{
	int tl, tg;
	int i, j, k;
	
//	if(swn<7)
	if(swn<5)
//	if(swn<3)
//	if(swn<4)
	{
		if((swtgt[swn]>0) && (swval[swn]==(swval[swn-1]+1)))
		{
			for(i=0; i<(swn-1); i++)
			{
				BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DUPL);
				BS2C_CompileExprPushConstInt(ctx, swval[i], BS2CC_TYZ_LONG);
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPL);
				BS2C_EmitOpcode(ctx, BSVM2_OP_JEQ);
				BS2C_EmitTempJAddr(ctx, swtgt[i]);
				BS2C_CompileExprPopType2(ctx);
			}

			BS2C_EmitTempJump(ctx, swtgt[swn-1]);
			return;
		}
	
		for(i=0; i<swn; i++)
		{
			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPL);
			BS2C_CompileExprPushConstInt(ctx, swval[i], BS2CC_TYZ_LONG);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPL);
			BS2C_EmitOpcode(ctx, BSVM2_OP_JEQ);
			BS2C_EmitTempJAddr(ctx, swtgt[i]);
			BS2C_CompileExprPopType2(ctx);
		}
		BS2C_EmitTempJump(ctx, swdfl);
		return;
	}

	tg=BS2C_GenTempLabel(ctx);

	i=swn>>1;

	BS2C_CompileExprPushType(ctx, BSVM2_OPZ_LONG);
	BS2C_EmitOpcode(ctx, BSVM2_OP_DUPL);
	BS2C_CompileExprPushConstInt(ctx, swval[i], BS2CC_TYZ_LONG);
	BS2C_EmitOpcode(ctx, BSVM2_OP_CMPL);
	BS2C_EmitOpcode(ctx, BSVM2_OP_JGE);
	BS2C_EmitTempJAddr(ctx, tg);
	BS2C_CompileExprPopType2(ctx);

	BS2C_CompileStmtSwitchDiR_L(ctx, swtgt, swval, i, swdfl);

	BS2C_EmitTempLabelB(ctx, tg);
	BS2C_CompileStmtSwitchDiR_L(ctx, swtgt+i, swval+i, swn-i, swdfl);
}

void BS2C_CompileStmtSwitchDiR_D(BS2CC_CompileContext *ctx,
	int *swtgt, f64 *swvalm, f64 *swvaln, int swn, int swdfl)
{
	int tl, tg;
	int i, j, k;
	
//	if(swn<7)
	if(swn<5)
//	if(swn<3)
//	if(swn<4)
	{
#if 0
		if((swtgt[swn]>0) && (swval[swn]==(swval[swn-1]+1)))
		{
			for(i=0; i<(swn-1); i++)
			{
				BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD);
				BS2C_CompileExprPushConstFloat(ctx,
					swvalm[i], BS2CC_TYZ_DOUBLE);
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPD);
				BS2C_EmitOpcode(ctx, BSVM2_OP_JEQ);
				BS2C_EmitTempJAddr(ctx, swtgt[i]);
				BS2C_CompileExprPopType2(ctx);
			}

			BS2C_EmitTempJump(ctx, swtgt[swn-1]);
			return;
		}
#endif
	
		for(i=0; i<swn; i++)
		{
			if(swvalm[i]==swvaln[i])
			{
				BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
				BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD);
				BS2C_CompileExprPushConstFloat(ctx,
					swvalm[i], BS2CC_TYZ_DOUBLE);
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPD);
				BS2C_EmitOpcode(ctx, BSVM2_OP_JEQ);
				BS2C_EmitTempJAddr(ctx, swtgt[i]);
				BS2C_CompileExprPopType2(ctx);
				continue;
			}
		
			tg=BS2C_GenTempLabel(ctx);

			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD);
			BS2C_CompileExprPushConstFloat(ctx,
				swvalm[i], BS2CC_TYZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPD);
			BS2C_EmitOpcode(ctx, BSVM2_OP_JLT);
			BS2C_EmitTempJAddr(ctx, tg);
			BS2C_CompileExprPopType2(ctx);

			BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD);
			BS2C_CompileExprPushConstFloat(ctx,
				swvaln[i], BS2CC_TYZ_DOUBLE);
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPD);
			BS2C_EmitOpcode(ctx, BSVM2_OP_JGT);
			BS2C_EmitTempJAddr(ctx, tg);
			BS2C_CompileExprPopType2(ctx);

			BS2C_EmitTempJump(ctx, swtgt[i]);

			BS2C_EmitTempLabelB(ctx, tg);
		}
		BS2C_EmitTempJump(ctx, swdfl);
		return;
	}

	tg=BS2C_GenTempLabel(ctx);

	i=swn>>1;

	BS2C_CompileExprPushType(ctx, BSVM2_OPZ_DOUBLE);
	BS2C_EmitOpcode(ctx, BSVM2_OP_DUPD);
	BS2C_CompileExprPushConstFloat(ctx, swvalm[i], BS2CC_TYZ_DOUBLE);
	BS2C_EmitOpcode(ctx, BSVM2_OP_CMPD);
	BS2C_EmitOpcode(ctx, BSVM2_OP_JGE);
	BS2C_EmitTempJAddr(ctx, tg);
	BS2C_CompileExprPopType2(ctx);

	BS2C_CompileStmtSwitchDiR_D(ctx, swtgt,
		swvalm, swvaln, i, swdfl);

	BS2C_EmitTempLabelB(ctx, tg);
	BS2C_CompileStmtSwitchDiR_D(ctx, swtgt+i,
		swvalm+i, swvaln+i, swn-i, swdfl);
}

void BS2C_CompileStmtSwitchDouble(BS2CC_CompileContext *ctx, dtVal expr)
{
	int t_sw_tgt[512];
	int t_sw2_tgt[512];
	f64 t_sw_valm[512];
	f64 t_sw_valn[512];
	f64 t_sw2_valm[512];
	f64 t_sw2_valn[512];

	int *sw_tgt;
	f64 *sw_valm, *sw_valn;
	int *sw2_tgt;
	f64 *sw2_valm, *sw2_valn;

	int swn, swm, swdfl, swgdfl;
	
	dtVal n0, n1, n2, n3;
	dtVal cc, nt, nf, ni, ns;
	double f, g, h;
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
	
//	if(!BS2C_TypeSmallIntP(ctx, cty))
	if(!BS2C_TypeSmallLongP(ctx, cty))
	{
		BS2C_CaseError(ctx);
		return;
	}

	nf=BS2P_GetAstNodeAttr(nt, "value");

	sw_tgt=t_sw_tgt;	sw_valm=t_sw_valm;		sw_valn=t_sw_valn;
	sw2_tgt=t_sw2_tgt;	sw2_valm=t_sw2_valm;	sw2_valn=t_sw2_valn;
	swm=512;

	swn=0; swdfl=-1;
	l=dtvArrayGetSize(nf);
	for(i=0; i<l; i++)
	{
		if((swn+64)>=swm)
		{
			if(sw_tgt==t_sw_tgt)
			{
				k=swm*2;
				while((k+64)>=swm)
					k=k+(k>>1);
				sw_tgt=bgbdt_mm_malloc(k*sizeof(int));
				sw_valm=bgbdt_mm_malloc(k*sizeof(f64));
				sw_valn=bgbdt_mm_malloc(k*sizeof(f64));
				memcpy(sw_tgt, t_sw_tgt, swn*sizeof(int));
				memcpy(sw_valm, t_sw_valm, swn*sizeof(f64));
				memcpy(sw_valn, t_sw_valn, swn*sizeof(f64));
				sw2_tgt=NULL;	sw2_valm=NULL;	sw2_valn=NULL;
				swm=k;
			}else
			{
				k=swm+(swm>>1);
				while((k+64)>=swm)
					k=k+(k>>1);
				sw_tgt=bgbdt_mm_realloc(sw_tgt, k*sizeof(int));
				sw_valm=bgbdt_mm_realloc(sw_valm, k*sizeof(f64));
				sw_valn=bgbdt_mm_realloc(sw_valn, k*sizeof(f64));
				swm=k;
			}
		}
	
		n0=dtvArrayGetIndexDtVal(nf, i);
		tag=BS2P_GetAstNodeTag(n0);
		if(!strcmp(tag, "case"))
		{
			ni=BS2P_GetAstNodeAttr(n0, "value");

			if(dtvIsSmallDoubleP(ni))
			{
				sw_tgt[swn]=BS2C_GenTempLabel(ctx);
				f=dtvUnwrapDouble(ni);
				sw_valm[swn]=f; sw_valn[swn]=f;
				swn++;
				continue;
			}

			fn=BS2P_GetAstNodeTag(n0);
			if(!strcmp(fn, "range"))
			{
				f=BS2P_GetAstNodeAttrF(n0, "lhs");
				g=BS2P_GetAstNodeAttrF(n0, "rhs");
				if(f<g) { h=f; f=g; g=h; }
				sw_tgt[swn]=BS2C_GenTempLabel(ctx);
				sw_valm[swn]=f; sw_valn[swn]=g;
				swn++;
				continue;
			}

			BS2C_CaseError(ctx);
			continue;
		}
		if(!strcmp(tag, "case_default"))
		{
			swdfl=BS2C_GenTempLabel(ctx);
			continue;
		}
	}

	if(!sw2_tgt)
	{
		sw2_tgt=bgbdt_mm_malloc(swm*sizeof(int));
		sw2_valm=bgbdt_mm_malloc(swm*sizeof(f64));
		sw2_valn=bgbdt_mm_malloc(swm*sizeof(f64));
	}

	for(i=0; i<swn; i++)
	{
		sw2_tgt[i]=sw_tgt[i];
		sw2_valm[i]=sw_valm[i];
		sw2_valn[i]=sw_valn[i];
	}
	for(i=0; i<swn; i++)
		for(j=i+1; j<swn; j++)
	{
		if(sw2_valm[j]<sw2_valm[i])
		{
			f=sw2_valm[i]; sw2_valm[i]=sw2_valm[j]; sw2_valm[j]=f;
			g=sw2_valn[i]; sw2_valn[i]=sw2_valn[j]; sw2_valn[j]=g;
			k=sw2_tgt[i]; sw2_tgt[i]=sw2_tgt[j]; sw2_tgt[j]=k;
		}
	}
	sw2_tgt[swn]=0;
	sw2_valm[swn]=0;
	sw2_valn[swn]=0;

	t0=BS2C_GenTempLabel(ctx);
	swgdfl=0;

	if(swdfl<0)
	{
		swdfl=BS2C_GenTempLabel(ctx);
		swgdfl=1;
	}

	l0=ctx->frm->brkstkpos++;
	ctx->frm->brkstk[l0]=t0;

	BS2C_CompileExpr(ctx, cc, BS2CC_TYZ_DOUBLE);
	BS2C_CompileStmtSwitchDiR_D(ctx, sw2_tgt,
		sw2_valm, sw2_valn, swn, swdfl);

	l=dtvArrayGetSize(nf); k=0;
	for(i=0; i<l; i++)
	{
		n0=dtvArrayGetIndexDtVal(nf, i);
		tag=BS2P_GetAstNodeTag(n0);
		if(!strcmp(tag, "case"))
		{
			if(k)
				BS2C_CompileNoexPush(ctx, cty);
			BS2C_EmitTempLabelB(ctx, sw_tgt[k++]);
			BS2C_CompilePop(ctx);
			continue;
		}
		if(!strcmp(tag, "case_default"))
		{
			if(k)
				BS2C_CompileNoexPush(ctx, cty);
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
		BS2C_CompileNoexPush(ctx, cty);
		BS2C_EmitTempLabelB(ctx, swdfl);
		BS2C_CompilePop(ctx);
	}

	BS2C_EmitTempLabelB(ctx, t0);
	ctx->frm->brkstkpos=l0;

	if(sw_tgt!=t_sw_tgt)
	{
		bgbdt_mm_free(sw_tgt);
		bgbdt_mm_free(sw_valm);
		bgbdt_mm_free(sw_valn);
		bgbdt_mm_free(sw2_tgt);
		bgbdt_mm_free(sw2_valm);
		bgbdt_mm_free(sw2_valn);
	}
}

void BS2C_CompileStmtSwitch(BS2CC_CompileContext *ctx, dtVal expr)
{
	int t_sw_tgt[512];
	s64 t_sw_val[512];

	int t_sw2_tgt[512];
	s64 t_sw2_val[512];

	int *sw_tgt;
	s64 *sw_val;
	int *sw2_tgt;
	s64 *sw2_val;

	int swn, swm, swdfl, swgdfl;
	
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

//	if(!BS2C_TypeSmallIntP(ctx, cty))
	if(!BS2C_TypeSmallLongP(ctx, cty))
	{
		if(BS2C_TypeSmallDoubleP(ctx, cty))
		{
			BS2C_CompileStmtSwitchDouble(ctx, expr);
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	nf=BS2P_GetAstNodeAttr(nt, "value");

	sw_tgt=t_sw_tgt;	sw_val=t_sw_val;
	sw2_tgt=t_sw2_tgt;	sw2_val=t_sw2_val;
	swm=512;

	swn=0; swdfl=-1;
	l=dtvArrayGetSize(nf);
	for(i=0; i<l; i++)
	{
		if((swn+64)>=swm)
		{
			if(sw_tgt==t_sw_tgt)
			{
				k=swm*2;
				while((k+64)>=swm)
					k=k+(k>>1);
				sw_tgt=bgbdt_mm_malloc(k*sizeof(int));
				sw_val=bgbdt_mm_malloc(k*sizeof(s64));
				memcpy(sw_tgt, t_sw_tgt, swn*sizeof(int));
				memcpy(sw_val, t_sw_val, swn*sizeof(s64));
				sw2_tgt=NULL;	sw2_val=NULL;
				swm=k;
			}else
			{
				k=swm+(swm>>1);
				while((k+64)>=swm)
					k=k+(k>>1);
				sw_tgt=bgbdt_mm_realloc(sw_tgt, k*sizeof(int));
				sw_val=bgbdt_mm_realloc(sw_val, k*sizeof(s64));
				swm=k;
			}
		}
	
		n0=dtvArrayGetIndexDtVal(nf, i);
//		BS2C_CompileStatement(ctx, n0);
//		if(ctx->ncfatal)
//			break;
		tag=BS2P_GetAstNodeTag(n0);
		if(!strcmp(tag, "case"))
		{
			ni=BS2P_GetAstNodeAttr(n0, "value");

			if(dtvIsSmallLongP(ni))
			{
				sw_tgt[swn]=BS2C_GenTempLabel(ctx);
				sw_val[swn]=dtvUnwrapLong(ni);
				swn++;
				continue;
			}

			fn=BS2P_GetAstNodeTag(n0);
			if(!strcmp(fn, "range"))
			{
				li=BS2P_GetAstNodeAttrI(n0, "lhs");
				lj=BS2P_GetAstNodeAttrI(n0, "rhs");
				if(lj<li) { lk=li; li=lj; lj=lk; }
				if((lj-li)>64)
					BS2C_CaseError(ctx);
				j=BS2C_GenTempLabel(ctx);
				for(lk=li; lk<=lj; lk++)
				{
					sw_tgt[swn]=j;
					sw_val[swn]=lk;
					swn++;
				}
				continue;
			}

//			j=BS2P_GetAstNodeAttrI(n0, "value");
//			sw_tgt[swn]=BS2C_GenTempLabel(ctx);
//			sw_val[swn]=j;
//			swn++;
			BS2C_CaseError(ctx);
			continue;
		}
		if(!strcmp(tag, "case_default"))
		{
			swdfl=BS2C_GenTempLabel(ctx);
			continue;
		}
	}

	if(!sw2_tgt)
	{
		sw2_tgt=bgbdt_mm_malloc(swm*sizeof(int));
		sw2_val=bgbdt_mm_malloc(swm*sizeof(s64));
	}

	for(i=0; i<swn; i++)
		{ sw2_tgt[i]=sw_tgt[i]; sw2_val[i]=sw_val[i]; }
	for(i=0; i<swn; i++)
		for(j=i+1; j<swn; j++)
	{
		if(sw2_val[j]<sw2_val[i])
		{
			lk=sw2_val[i]; sw2_val[i]=sw2_val[j]; sw2_val[j]=lk;
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

	if(BS2C_TypeSmallIntP(ctx, cty))
	{
		BS2C_CompileStmtSwitchDiR(ctx, sw2_tgt, sw2_val, swn, swdfl);
	}else if(BS2C_TypeSmallLongP(ctx, cty))
	{
		BS2C_CompileStmtSwitchDiR_L(ctx, sw2_tgt, sw2_val, swn, swdfl);
	}

	l=dtvArrayGetSize(nf); k=0;
	for(i=0; i<l; i++)
	{
		n0=dtvArrayGetIndexDtVal(nf, i);
		tag=BS2P_GetAstNodeTag(n0);
		if(!strcmp(tag, "case"))
		{
			if(k)
				BS2C_CompileNoexPush(ctx, cty);
			BS2C_EmitTempLabelB(ctx, sw_tgt[k++]);
			BS2C_CompilePop(ctx);
			continue;
		}
		if(!strcmp(tag, "case_default"))
		{
			if(k)
				BS2C_CompileNoexPush(ctx, cty);
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
		BS2C_CompileNoexPush(ctx, cty);
		BS2C_EmitTempLabelB(ctx, swdfl);
		BS2C_CompilePop(ctx);
	}

	BS2C_EmitTempLabelB(ctx, t0);
	ctx->frm->brkstkpos=l0;

	if(sw_tgt!=t_sw_tgt)
	{
		bgbdt_mm_free(sw_tgt);
		bgbdt_mm_free(sw_val);
		bgbdt_mm_free(sw2_tgt);
		bgbdt_mm_free(sw2_val);
	}
}

void BS2C_CompileStmtTryCatch2(
	BS2CC_CompileContext *ctx, dtVal expr, int lblf)
{
	dtVal n0, n1, n2, n3;
	dtVal cc, ln, rn, en, et;
	BS2CC_VarInfo *vi;
	char *tag, *ens;
	int t0, t1, t2, t3;
	int ty, ix;

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "catch"))
	{
		cc=BS2P_GetAstNodeAttr(expr, "cond");
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");

		en=BS2P_GetAstNodeAttr(cc, "name");
		et=BS2P_GetAstNodeAttr(cc, "type");
		ty=BS2C_TypeExtType(ctx, et);
		
		ens=BGBDT_TagStr_GetUtf8(en);

		vi=BS2C_GetTypeObject(ctx, ty);
		ix=BS2C_IndexFrameGlobal(ctx, vi->gid);

		BS2C_CompileStmtTryCatch2(ctx, ln, lblf);

		t0=BS2C_GenTempLabel(ctx);

		BS2C_EmitOpcode(ctx, BSVM2_OP_CATCH);
		BS2C_EmitOpcodeUCx(ctx, ix);
		BS2C_EmitTempJAddr(ctx, t0);
		BS2C_CompileStoreName(ctx, ens);

		BS2C_CompileStatement(ctx, rn);

//		BS2C_EmitOpcode(ctx, BSVM2_OP_PUSHA);
		BS2C_CompilePushDummy(ctx, BS2CC_TYZ_ADDRESS);
		BS2C_EmitTempJump(ctx, lblf);
//		BS2C_CompileNoexPush(ctx, BS2CC_TYZ_ADDRESS);
		BS2C_EmitTempLabelB(ctx, t0);
		return;
	}

	if(!strcmp(tag, "try"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "body");

		t0=BS2C_GenTempLabel(ctx);
//		t1=BS2C_GenTempLabel(ctx);

		BS2C_EmitOpcode(ctx, BSVM2_OP_BEGTRY);
		BS2C_EmitTempJAddr(ctx, t0);

		BS2C_CompileStatement(ctx, ln);

		BS2C_EmitTempLabelB(ctx, t0);
		BS2C_CompileExprPushType(ctx, BS2CC_TYZ_ADDRESS);
		BS2C_EmitOpcode(ctx, BSVM2_OP_ENDTRY);
		BS2C_EmitTempJAddr(ctx, lblf);
//		BS2C_EmitTempLabelB(ctx, t0);
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileStmtTryCatch(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal n0, n1, n2, n3;
	dtVal cc, nt, nf, ni, ns, na;
	s64 li, lj, lk;
	int cty, ln, z;
	char *tag, *fn;
	int t0, t1, t2, t3;
	int l0, l1, l2, l3;
	int i, j, k, l;

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "finally"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "lhs");
		n1=BS2P_GetAstNodeAttr(expr, "rhs");

		t0=BS2C_GenTempLabel(ctx);
		BS2C_CompileStmtTryCatch2(ctx, n0, t0);

		BS2C_EmitTempLabelB(ctx, t0);
//		BS2C_CompilePop(ctx);
		BS2C_CompileStatement(ctx, n1);

		BS2C_EmitOpcode(ctx, BSVM2_OP_RETHROW);
		BS2C_EmitSetNewtrace(ctx);
		BS2C_CompileExprPopType1(ctx);
		
		return;
	}

	if(!strcmp(tag, "try") ||
		!strcmp(tag, "catch"))
	{
		t0=BS2C_GenTempLabel(ctx);
		BS2C_CompileStmtTryCatch2(ctx, expr, t0);
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETHROW);
		BS2C_EmitSetNewtrace(ctx);

		BS2C_EmitTempLabelB(ctx, t0);
//		BS2C_CompilePop(ctx);
		BS2C_EmitOpcode(ctx, BSVM2_OP_RETHROW);
		BS2C_EmitSetNewtrace(ctx);
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileStatement(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal n0, n1, n2, n3;
	dtVal cc, nt, nf, ni, ns, na;
	s64 li, lj, lk;
	int cty, ln, z;
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
		t2=BS2C_GenTempLabel(ctx);

		l0=ctx->frm->constkpos++;
		l1=ctx->frm->brkstkpos++;
		ctx->frm->constk[l0]=t2;
		ctx->frm->brkstk[l1]=t1;

		BS2C_CompileExpr(ctx, ni, BSVM2_OPZ_VOID);

		BS2C_EmitTempLabelB(ctx, t0);
		BS2C_CompileTempJmpElse(ctx, cc, t1);
		BS2C_CompileStatement(ctx, nt);

		BS2C_EmitTempLabelB(ctx, t2);
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


//	if(!strcmp(tag, "return"))
	if(!strcmp(tag, "return") ||
		!strcmp(tag, "tail"))
	{
		nt=BS2P_GetAstNodeAttr(expr, "value");
		nt=BS2C_ReduceExpr(ctx, nt);

		cty=ctx->frm->func->rty;
		
		ctx->frm->wasret=1;

		if(!strcmp(tag, "tail") && (cty==BSVM2_OPZ_VOID))
		{
			if(!dtvNullP(nt))
			{
				BS2C_CompileExpr(ctx, nt, BSVM2_OPZ_VOID);
			}

			if(ctx->frm->jcleanup>0)
			{
				BS2C_EmitTempJump(ctx, ctx->frm->jcleanup);
				return;
			}

			BS2C_EmitOpcode(ctx, BSVM2_OP_RETV);
			ctx->frm->newtrace=1;
			return;
		}
		
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

		t0=BS2C_InferExprLocalIndex(ctx, nt);
		if(t0>=0)
		{
			z=BS2C_GetTypeBaseZ(ctx, cty);
			BS2C_EmitOpcode(ctx, BSVM2_OP_RET2);
			BS2C_EmitOpcodeUZx(ctx, z, t0);
			ctx->frm->newtrace=1;
			return;
		}

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

	if(!strcmp(tag, "throw"))
	{
		na=BS2P_GetAstNodeAttr(expr, "value");
		nt=BS2C_ReduceExpr(ctx, na);
		t0=BS2C_InferExpr(ctx, nt);
		BS2C_CompileExpr(ctx, nt, t0);
		BS2C_EmitOpcode(ctx, BSVM2_OP_THROW);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(	!strcmp(tag, "try") ||
		!strcmp(tag, "catch") ||
		!strcmp(tag, "finally"))
	{
		BS2C_CompileStmtTryCatch(ctx, expr);
		return;
	}

	if(!strcmp(tag, "let_escape"))
	{
		cc=BS2P_GetAstNodeAttr(expr, "cond");
		nt=BS2P_GetAstNodeAttr(expr, "then");

		cc=BS2C_ReduceExpr(ctx, cc);

		if(BGBDT_TagStr_IsSymbolP(cc))
			{ ns=cc; }
		else
			{ ns=BS2P_GetAstNodeAttr(cc, "name"); }

		t0=BS2C_GenTempLabel(ctx);

		l0=BS2C_InferExprLocalIndex(ctx, ns);

		BS2C_EmitOpcode(ctx, BSVM2_OP_BEGLEC);
		BS2C_EmitOpcodeIdx(ctx, l0);
		BS2C_EmitTempJAddr(ctx, t0);

		BS2C_CompileStatement(ctx, nt);

		BS2C_EmitTempLabelB(ctx, t0);
		BS2C_EmitOpcode(ctx, BSVM2_OP_ENDLEC);
		BS2C_EmitOpcodeIdx(ctx, l0);
		
		return;
	}

	if(!strcmp(tag, "empty_block"))
	{
		return;
	}

	BS2C_CompileExpr(ctx, expr, BSVM2_OPZ_VOID);
}

void BS2C_CompileFuncVarExpr(BS2CC_CompileContext *ctx, dtVal expr)
{
	dtVal ln, rn, an, fnn;
	dtVal n0, n1;
	BS2CC_VarInfo *vi, *vi2;
	char *tag, *fn, *op;
	int i, j, k, l;

	if(dtvIsFixIntP(expr))
		{ return; }
	if(dtvIsFixUIntP(expr))
		{ return; }
	if(dtvIsFixLongP(expr))
		{ return; }
	if(dtvIsFixFloatP(expr))
		{ return; }
	if(dtvIsFixDoubleP(expr))
		{ return; }

	if(BGBDT_XI128_IsInt128P(expr))
		{ return; }
	if(BGBDT_XF128_IsFloat128P(expr))
		{ return; }

	if(dtvIsCharP(expr))
		{ return; }

	if(BGBDT_TagStr_IsSymbolP(expr))
		{ return; }

	if(BGBDT_TagStr_IsStringP(expr))
		{ return; }

	tag=BS2P_GetAstNodeTag(expr);
	
	if(!tag)
	{
		return;
	}
	
	if(!strcmp(tag, "binary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		op=BS2P_GetAstNodeAttrS(expr, "op");
		BS2C_CompileFuncVarExpr(ctx, ln);
		BS2C_CompileFuncVarExpr(ctx, rn);
		return;
	}

	if(!strcmp(tag, "bincmp"))
	{
		return;
	}

	if(!strcmp(tag, "unary"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		op=BS2P_GetAstNodeAttrS(expr, "op");
		BS2C_CompileFuncVarExpr(ctx, ln);
		return;
	}

	if(!strcmp(tag, "assign"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFuncVarExpr(ctx, ln);
		BS2C_CompileFuncVarExpr(ctx, rn);
		return;
	}

	if(!strcmp(tag, "assignop"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFuncVarExpr(ctx, ln);
		BS2C_CompileFuncVarExpr(ctx, rn);
		return;
	}

	if(!strcmp(tag, "tern"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFuncVarExpr(ctx, ln);
		BS2C_CompileFuncVarExpr(ctx, rn);
		return;
	}

	if(!strcmp(tag, "range"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFuncVarExpr(ctx, ln);
		BS2C_CompileFuncVarExpr(ctx, rn);
		return;
	}

	if(!strcmp(tag, "instanceof"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFuncVarExpr(ctx, ln);
		BS2C_CompileFuncVarExpr(ctx, rn);
		return;
	}

	if(!strcmp(tag, "cast") || !strcmp(tag, "cast_strict") ||
		!strcmp(tag, "prefix_cast"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
//		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFuncVarExpr(ctx, ln);
//		BS2C_CompileFuncVarExpr(ctx, rn);
		return;
	}

	if(!strcmp(tag, "call"))
	{
		an=BS2P_GetAstNodeAttr(expr, "args");

		fnn=BS2P_GetAstNodeAttr(expr, "func");
		if(BGBDT_TagStr_IsSymbolP(fnn))
		{
//			fn=BGBDT_TagStr_GetUtf8(fnn);
//			ty=BS2C_InferRetTypeName(ctx, fn);
			return;
		}

		tag=BS2P_GetAstNodeTag(fnn);
		
		if(!tag)
		{
			BS2C_CaseError(ctx);
			return;
		}

		if(!strcmp(tag, "loadslot"))
		{
			ln=BS2P_GetAstNodeAttr(expr, "lhs");
			rn=BS2P_GetAstNodeAttr(expr, "rhs");
			BS2C_CompileFuncVarExpr(ctx, ln);
			BS2C_CompileFuncVarExpr(ctx, rn);
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "new"))
	{
//		rn=BS2P_GetAstNodeAttr(expr, "type");
//		ty=BS2C_TypeBaseType(ctx, rn);
//		ty=BS2C_TypeExtType(ctx, rn);
		return;
	}

	if(!strcmp(tag, "loadindex"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFuncVarExpr(ctx, ln);
		BS2C_CompileFuncVarExpr(ctx, rn);
		return;

//		ln=BS2P_GetAstNodeAttr(expr, "lhs");
//		lt=BS2C_InferExpr(ctx, ln);
//		ty=BS2C_TypeExtType(ctx, rn);
//		ty=BS2C_TypeDerefType(ctx, lt);
//		return(ty);
	}

	if(!strcmp(tag, "postinc") || !strcmp(tag, "postdec") ||
		!strcmp(tag, "preinc") || !strcmp(tag, "predec"))
	{
//		ln=BS2P_GetAstNodeAttr(expr, "value");
//		ty=BS2C_InferExpr(ctx, ln);
		return;
	}

	if(!strcmp(tag, "loadslot"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "lhs");
		rn=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFuncVarExpr(ctx, ln);
		BS2C_CompileFuncVarExpr(ctx, rn);
		return;
	}

	if(!strcmp(tag, "array"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

		if(dtvIsArrayP(ln))
			{ l=dtvArrayGetSize(ln); }
		else
			{ l=1; }

		for(i=0; i<l; i++)
		{
			if(dtvIsArrayP(ln))
				{ n0=dtvArrayGetIndexDtVal(ln, i); }
			else
				{ n0=ln; }
			BS2C_CompileFuncVarExpr(ctx, n0);
		}

//		l=dtvArrayGetSize(ln);
//		ty=BS2CC_TYZ_VARIANT;
		
		return;
	}

	if(!strcmp(tag, "object"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");

		if(dtvIsArrayP(ln))
		{
			l=dtvArrayGetSize(ln);
			for(i=0; i<l; i++)
			{
				n0=dtvArrayGetIndexDtVal(ln, i);
				n1=BS2P_GetAstNodeAttr(n0, "value");
				fn=BS2P_GetAstNodeAttrS(n0, "name");

				BS2C_CompileFuncVarExpr(ctx, n1);
			}
		}

//		return(BS2CC_TYZ_VARIANT);
		return;
	}

	if(!strcmp(tag, "imag"))
	{
		return;
	}

	if(!strcmp(tag, "vector"))
	{
		ln=BS2P_GetAstNodeAttr(expr, "value");
		fn=BS2P_GetAstNodeAttrS(expr, "sfx");

		l=dtvArrayGetSize(ln);

		return;
	}

	if(!strcmp(tag, "func_dfl") ||
		!strcmp(tag, "func_ind"))
	{
		return;
	}

	if(!strcmp(tag, "func_aut"))
	{
		BS2C_CompileFunVar(ctx, expr);
		return;
	}

	BS2C_CaseError(ctx);
	return;
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
	{
		BS2C_CompileFuncVarExpr(ctx, expr);
		return;
	}
	if(!strcmp(tag, "assignop"))
	{
		BS2C_CompileFuncVarExpr(ctx, expr);
		return;
	}
	if(!strcmp(tag, "break"))
		return;
	if(!strcmp(tag, "call"))
	{
		BS2C_CompileFuncVarExpr(ctx, expr);
		return;
	}
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

	if(!strcmp(tag, "postdec"))
		return;
	if(!strcmp(tag, "postinc"))
		return;
	if(!strcmp(tag, "predec"))
		return;
	if(!strcmp(tag, "preinc"))
		return;

	if(!strcmp(tag, "return"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_CompileFuncVarExpr(ctx, n0);
		return;
	}
	if(!strcmp(tag, "throw"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_CompileFuncVarExpr(ctx, n0);
		return;
	}

	if(!strcmp(tag, "tail"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "value");
		BS2C_CompileFuncVarExpr(ctx, n0);
		return;
	}

	if(!strcmp(tag, "finally"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "lhs");
		n1=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFunVarStatement(ctx, n0);
		BS2C_CompileFunVarStatement(ctx, n1);
		return;
	}

	if(!strcmp(tag, "catch"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "cond");
		BS2C_CompileFunVar(ctx, n2);

		n0=BS2P_GetAstNodeAttr(expr, "lhs");
		n1=BS2P_GetAstNodeAttr(expr, "rhs");
		BS2C_CompileFunVarStatement(ctx, n0);
		BS2C_CompileFunVarStatement(ctx, n1);
		return;
	}

	if(!strcmp(tag, "try"))
	{
		n0=BS2P_GetAstNodeAttr(expr, "body");
		BS2C_CompileFunVarStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "let_escape"))
	{
		n2=BS2P_GetAstNodeAttr(expr, "cond");
		BS2C_CompileFunVar(ctx, n2);

		n0=BS2P_GetAstNodeAttr(expr, "then");
		BS2C_CompileFunVarStatement(ctx, n0);
		return;
	}

	if(!strcmp(tag, "empty_block"))
		return;

	BS2C_CaseError(ctx);
}
