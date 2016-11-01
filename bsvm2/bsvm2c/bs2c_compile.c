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
/** \file
 * BS2 Compiler
 *
 */

/** Get index for current souce file and line number for error message. */
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

/** Get an index for a name to be used in an error message. */
int BS2C_CompileErrorIndexName(
	BS2CC_CompileContext *ctx, char *name)
{
	int fi;
	int i;
	
	for(i=0; i<ctx->nsrcnixs; i++)
	{
		if(!strcmp(ctx->srcnix[i], name))
			break;
	}
	if(i>=ctx->nsrcnixs)
	{
		i=ctx->nsrcnixs++;
		ctx->srcnix[i]=BS2P_StrSym(ctx, name);
	}
	fi=i;
	
	return(fi);
}

/** Emit an error given an error code. */
int BS2C_CompileError(BS2CC_CompileContext *ctx, int errn)
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
			ctx->cwarnpix[i]=ctx->ncwparm;

			return(i);
		}
		return(-1);
	}
	
	if((errn&BS2CC_ERRN_TMASK)==BS2CC_ERRN_ERROR)
	{
		BSVM2_DBGTRAP
	
		if(ctx->ncerr<256)
		{
			i=ctx->ncerr++;
			ctx->cerr[i]=errn;
			ctx->cerrln[i]=j;
			ctx->cerrpix[i]=ctx->ncwparm;
			return(i);
		}
		
		if(!ctx->ncfatal)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_FATALERRCNT);
			return(-1);
		}
		return(-1);
	}

	if((errn&BS2CC_ERRN_TMASK)==BS2CC_ERRN_FATAL)
	{
		BSVM2_DBGTRAP

		if(ctx->ncfatal<16)
		{
			i=ctx->ncfatal++;
			ctx->cfatal[i]=errn;
			ctx->cfatalln[i]=j;
			return(i);
		}
		return(-1);
	}
	
	return(-1);
}

/** Error, we have stumbled onto an unimplemented/unsupported special case. */
void BS2C_CaseError(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_CASEERROR);
}

/** Error, this compiler logic is merely a stub. */
void BS2C_StubError(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_STUBERROR);
}

/** Warning, constant is out of range for a type. */
void BS2C_WarnConstRange(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_CONSTRANGE);
}

/** Error, code tried to divide something by 0. */
void BS2C_ErrDivZero(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_DIVZERROR);
}

/** Error, undeclared variable. */
void BS2C_ErrNoDecl(BS2CC_CompileContext *ctx, char *name)
{
	int i, j, k;
	i=BS2C_CompileError(ctx, BS2CC_ERRN_ERRNODECL);
	j=BS2C_CompileErrorIndexName(ctx, name);
	ctx->cwparm[ctx->ncwparm++]=j;
}

/** Error, Too few arguments for a function. */
void BS2C_ErrTooFewArgs(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRTOOFEWARGS);
}

/** Error, too many arguments for a function. */
void BS2C_ErrTooManyArgs(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRTOOMANYARGS);
}

/** Generate error about compile-time stack being misaligned. */
void BS2C_ErrStackMisalign(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRSTACKMISAL);
}

/** Generate an error about compile-time stack underflowing. */
void BS2C_ErrStackUnderflow(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRSTACKUFLOW);
}

/** Warn about an implicit conversion. */
void BS2C_WarnImplicitConv(BS2CC_CompileContext *ctx, int dty, int sty)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_CONVIMPLICIT);
	ctx->cwparm[ctx->ncwparm++]=dty;
	ctx->cwparm[ctx->ncwparm++]=sty;
}

/** Warn about a narrowing conversion. */
void BS2C_WarnNarrowingConv(BS2CC_CompileContext *ctx, int dty, int sty)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_CONVNARROW);
	ctx->cwparm[ctx->ncwparm++]=dty;
	ctx->cwparm[ctx->ncwparm++]=sty;
}

/** Warning, not typedef. */
void BS2C_WarnNotTypedef(BS2CC_CompileContext *ctx, char *name)
{
	int i, j, k;
	i=BS2C_CompileError(ctx, BS2CC_ERRN_NOTYPEDEF);
	j=BS2C_CompileErrorIndexName(ctx, name);
	ctx->cwparm[ctx->ncwparm++]=j;
}

/** Allocate memory for variable. */
BS2CC_VarInfo *BS2C_AllocVarInfo(BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *tmp;
	
	tmp=dtmAlloc("bs2cc_varinfo_t",
		sizeof(BS2CC_VarInfo));
	tmp->alcnext=ctx->lst_vari;
	ctx->lst_vari=tmp;
	return(tmp);
}

/** Allocate memory for compile frame. */
BS2CC_CcFrame *BS2C_AllocCcFrame(BS2CC_CompileContext *ctx)
{
	BS2CC_CcFrame *tmp;
	
	tmp=dtmAlloc("bs2cc_ccframe_t",
		sizeof(BS2CC_CcFrame));
	tmp->alcnext=ctx->lst_ccfrm;
	ctx->lst_ccfrm=tmp;
	return(tmp);
}

/** Allocate memory for package frame. */
BS2CC_PkgFrame *BS2C_AllocPkgFrame(BS2CC_CompileContext *ctx)
{
	BS2CC_PkgFrame *tmp;
	
	tmp=dtmAlloc("bs2cc_pkgframe_t",
		sizeof(BS2CC_PkgFrame));
	return(tmp);
}

/** Release memory associated with a variable definition. */
int BS2C_CleanupFreeVarInfo(BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	dtmFree(vari);
	return(0);
}

/** Release memory associated with a compiler frame. */
int BS2C_CleanupFreeCcFrame(BS2CC_CompileContext *ctx, BS2CC_CcFrame *frm)
{
	if(frm->cts && (frm->cts!=frm->t_textbuf))
		bgbdt_mm_free(frm->cts);

	if(frm->tlbl_id && (frm->tlbl_id!=frm->t_tlbl_id))
	{
		bgbdt_mm_free(frm->tlbl_id);
		bgbdt_mm_free(frm->tlbl_ct);
	}

	if(frm->trlc_id && (frm->trlc_id!=frm->t_trlc_id))
	{
		bgbdt_mm_free(frm->trlc_id);
		bgbdt_mm_free(frm->trlc_ct);
	}

	if(frm->nlbl_id && (frm->nlbl_id!=frm->t_nlbl_id))
	{
		bgbdt_mm_free(frm->nlbl_id);
		bgbdt_mm_free(frm->nlbl_n);
	}

	dtmFree(frm);
	return(0);
}

/** Lookup the package frame associated with a given package qname. */
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

/** Get the package frame associated with a given package qname. */
BS2CC_PkgFrame *BS2C_GetPkgFrame(
	BS2CC_CompileContext *ctx, char *qname)
{
	BS2CC_PkgFrame *pkg;
	BS2CC_VarInfo *vi;
	int i;
	
	pkg=BS2C_LookupPkgFrame(ctx, qname);
	if(pkg)
		return(pkg);

	pkg=BS2C_AllocPkgFrame(ctx);
	pkg->qname=BS2P_StrSym(ctx, qname);

	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->vitype=BS2CC_VITYPE_PACKAGE;
	vi->pkg=pkg;
	vi->qname=pkg->qname;

	pkg->gid=i;
	pkg->next=ctx->pkg_first;
	ctx->pkg_first=pkg;
	return(pkg);
}

/** Enter into a package. */
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

/** Exit the current package. */
void BS2C_ExitPkgFrame(
	BS2CC_CompileContext *ctx)
{
	int i;

	i=--ctx->pkgstkpos;
	ctx->pkg=ctx->pkgstk[i];
}

/** Generate code for cleaning up a variable. */
void BS2C_CompileFuncBodyCleanupVar(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vi, int ix)
{
	BS2CC_VarInfo *vi2;
	int bty, cty, sz, z, ix2;
	int i;

	bty=vi->bty;

	if(vi->bmfl&BS2CC_TYFL_DYNAMIC)
	{
		ix2=BS2C_LookupFrameGlobal(ctx, vi->name);
		BS2C_EmitOpcode(ctx, BSVM2_OP_DFXDYV);
		BS2C_EmitOpcodeJx(ctx, ix, ix2);
		return;
	}
	
	if(vi->vitype==BS2CC_VITYPE_LCLFUNC)
	{
		ix2=BS2C_LookupFrameGlobal(ctx, vi->name);
		BS2C_EmitOpcode(ctx, BSVM2_OP_DFXLFCN);
		BS2C_EmitOpcodeJx(ctx, ix, ix2);
		return;
	}

	if(BS2C_TypeSizedArrayP(ctx, bty))
	{
		cty=BS2C_TypeDerefType(ctx, bty);
		sz=BS2C_TypeGetArraySize(ctx, bty);
		z=BS2C_GetTypeBaseZ(ctx, cty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_DFXARR);
		BS2C_EmitOpcodeUZx(ctx, z, sz);
		BS2C_EmitOpcodeUCx(ctx, ix);

//		BS2C_EmitOpcode(ctx, BSVM2_OP_NEWARR);
//		BS2C_EmitOpcodeUZx(ctx, z, sz);
//		BS2C_CompileStoreName(ctx, name);

//		if(ctx->frm->jcleanup<=0)
//			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);

		return;
	}

	if(BS2C_TypeFixedStructP(ctx, bty))
	{
		vi2=BS2C_GetTypeObject(ctx, bty);
		i=BS2C_IndexFrameGlobal(ctx, vi2->gid);

		BS2C_EmitOpcode(ctx, BSVM2_OP_DFXOBJ);
//		BS2C_EmitOpcodeUCx(ctx, ix);
//		BS2C_EmitOpcodeUCx(ctx, i);
		BS2C_EmitOpcodeJx(ctx, ix, i);
		return;
	}

	if(BS2C_TypeX128P(ctx, bty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_DFXX);
		BS2C_EmitOpcodeUCx(ctx, ix);
		return;
	}
}

/** Generate code for initializing a variable. */
void BS2C_CompileFuncBodyInitVar(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vi, int ix)
{
	BS2CC_VarInfo *vi2, *vi3;
	int t0, t1, t2, t3;
	int z, ix2;
	dtVal n0, n1;
	int i, j, k, l;
	
#if 1
	if(vi->vitype==BS2CC_VITYPE_LCLFUNC)
	{
		ix2=BS2C_LookupFrameGlobal(ctx, vi->name);
		BS2C_EmitOpcode(ctx, BSVM2_OP_IFXLFCN);
		BS2C_EmitOpcodeJx(ctx, ix, ix2);

		vi2=BS2C_GetFrameGlobalInfo(ctx, ix2);

		if(vi2->niface>0)
		{
			BS2C_CompileExprPushType(ctx, BS2CC_TYZ_VARIANT);
			BS2C_EmitOpcode(ctx, BSVM2_OP_LDA);
			BS2C_EmitOpcodeUCx(ctx, ix);

			for(i=0; i<vi2->niface; i++)
			{
				vi3=vi2->iface[i];
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

			BS2C_EmitOpcode(ctx, BSVM2_OP_POPA);
			BS2C_CompileExprPopType1(ctx);
		}
		return;
	}
#endif

}

/** Emit cleanup and final return code for a function. */
void BS2C_CompileFuncBodyCleanup(
	BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *vi;
	int i, j, k, l;

	if(ctx->frm->jcleanup<=0)
		return;

//	BS2C_CompileNoexPush(ctx, ctx->frm->func->rty);
//	BS2C_EmitTempLabelB(ctx, ctx->frm->jcleanup);

	if(ctx->frm->func->rty!=BS2CC_TYZ_VOID)
	{
//		BS2C_CompileExprPushType(ctx, ctx->frm->func->rty);
		BS2C_CompileNoexPush(ctx, ctx->frm->func->rty);
	}

	BS2C_EmitOpcode(ctx, BSVM2_OP_LBLCLNP);
	ctx->frm->newtrace=1;
	BS2C_EmitTempLabelB(ctx, ctx->frm->jcleanup);

	for(i=0; i<ctx->frm->nlocals; i++)
	{
		vi=ctx->frm->locals[i];
		BS2C_CompileFuncBodyCleanupVar(ctx, vi, i);
	}
	
	BS2C_EmitReturnVal(ctx);
}

/** Emit initialization code for a function. */
void BS2C_CompileFuncBodyInit(
	BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *vi;
	int i, j, k, l;

	if(ctx->frm->jcleanup<=0)
		return;

	for(i=0; i<ctx->frm->nlocals; i++)
	{
		vi=ctx->frm->locals[i];
		BS2C_CompileFuncBodyInitVar(ctx, vi, i);
	}
}

/** Compile the body of a function. */
void BS2C_CompileFuncBody(BS2CC_CompileContext *ctx, BS2CC_VarInfo *func)
{
	BS2CC_CcFrame *frm;
	int tk;
	int i, j, k, l;

//	if(func->body)
	if(0)
	{
		frm->func->body;
		frm->nlocals=0;
		frm->ntrlc=0;
		frm->ntlbl=0;
	}else
	{
		frm=BS2C_AllocCcFrame(ctx);
	}

	tk=func->tokcnt;
	frm->def_rlcty=0;
	if(tk>=32)frm->def_rlcty=1;
	if(tk>=4096)frm->def_rlcty=2;
	if(tk>=(1<<20))frm->def_rlcty=3;

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

	BS2C_CompileFuncBodyInit(ctx);
		
	BS2C_CompileStatement(ctx,
		func->bodyExp);
	
	if(!ctx->frm->wasret)
	{
		if(ctx->frm->jcleanup>0)
			{ BS2C_EmitReturnCleanupV(ctx, 1); }
		else
			{ BS2C_EmitReturnV(ctx); }
	}
	
	if(ctx->frm->jcleanup>0)
	{
		BS2C_CompileFuncBodyCleanup(ctx);
	}
	
	if(ctx->frm->stackpos!=0)
	{
		BS2C_ErrStackMisalign(ctx);
	}
	
	BS2C_FixupLabels(ctx);
	
	frm->szt=frm->ct-frm->cts;
	
//	printf("Avg B/tk=%f\n", frm->szt/(tk+0.00001));
	
	ctx->frm=ctx->frmstack[i];
	ctx->frmstackpos=i;
}

BS2CC_VarInfo *BS2C_GetNewGlobalVari(
	BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *vi;
	int i;
	
	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
//	vi->vitype=BS2CC_VITYPE_PACKAGE;
//	vi->pkg=pkg;
//	vi->qname=pkg->qname;

	return(vi);
}

dtVal BS2C_CompileVarInit_NormalizeValueType(
	BS2CC_CompileContext *ctx, dtVal ni, int ty)
{
	int z;

	z=BS2C_GetTypeBaseZ(ctx, ty);
	switch(z)
	{
	case BSVM2_OPZ_INT:
		ni=dtvWrapInt(dtvUnwrapLong(ni)); break;
	case BSVM2_OPZ_LONG:
		ni=dtvWrapLong(dtvUnwrapLong(ni)); break;
	case BSVM2_OPZ_FLOAT:
		ni=dtvWrapFloat(dtvUnwrapDouble(ni)); break;
	case BSVM2_OPZ_DOUBLE:
		ni=dtvWrapDouble(dtvUnwrapDouble(ni)); break;
	case BSVM2_OPZ_UINT:
		ni=dtvWrapUInt(dtvUnwrapLong(ni)); break;
	case BSVM2_OPZ_UBYTE:
		ni=dtvWrapInt((byte)dtvUnwrapInt(ni)); break;
	case BSVM2_OPZ_SBYTE:
		ni=dtvWrapInt((sbyte)dtvUnwrapInt(ni)); break;
	case BSVM2_OPZ_USHORT:
		ni=dtvWrapInt((u16)dtvUnwrapInt(ni)); break;
	case BSVM2_OPZ_SHORT:
		ni=dtvWrapInt((s16)dtvUnwrapInt(ni)); break;
	default: break;
	}
	return(ni);
}

void BS2C_CompileVarInit(BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	BS2CC_VarInfo *vi;
	char *tag;
	dtVal ni, nv;
	int z;

	ni=BS2C_ReduceExpr(ctx, vari->initExp);
	if(!dtvTrueP(ni))
		return;

	if(dtvIsSmallLongP(ni) ||
		dtvIsSmallDoubleP(ni) ||
		BGBDT_TagStr_IsStringP(ni))
	{
		ni=BS2C_CompileVarInit_NormalizeValueType(ctx, ni, vari->bty);
		vari->initVal=ni;
		return;
	}
	
	nv=BS2C_ReduceEvaluateExprAs(ctx, ni, vari->bty);
//	if(dtvTrueP(nv))
	if(!dtvUndefinedP(nv))
	{
		vari->initVal=nv;
		
		if(dtvIsArrayP(nv))
		{
			vi=BS2C_GetNewGlobalVari(ctx);
			vi->vitype=BS2CC_VITYPE_INITARR;
			vi->initVal=nv;

			vari->initVal=dtvWrapContIdx(vi->gid);
			vari->initGid=vi->gid;
		}
		return;
	}
	
//	nv=BS2E_EvalExpr(ctx, ni);
//	if(dtvTrueP(nv))
//	{
//		vari->initVal=nv;
//		return;
//	}

	BSVM2_DBGTRAP
}

/** Rebuild types for a variable. */
void BS2C_CompileRebuildVarType(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	char *s;

	if(dtvTrueP(vari->typeExp))
	{
		vari->bty=BS2C_TypeRefinedType(
			ctx, vari, vari->typeExp);
	}
	s=BS2C_GetTypeSig(ctx, vari->bty);
	if((vari->bty==BS2CC_TYZ_VARARG) && vari->name)
		{ s="Cz"; }
	if(!s)
	{
		BSVM2_DBGTRAP
		s=BS2C_GetTypeSig(ctx, vari->bty);
	}
	vari->sig=BS2P_StrSym(ctx, s);
}

/** Rebuild types for a function. */
void BS2C_CompileRebuildFuncType(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	char tb[256];
	char *s, *t;
	int i, j, k, l;

	if(dtvTrueP(vari->typeExp))
	{
		vari->rty=BS2C_TypeRefinedType(
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

/** Rebuild the types for a structure. */
void BS2C_CompileRebuildStructType(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	BS2CC_VarInfo *vi2;
	dtVal n0;
	dtVal ne, ni;
	char *fn;
	int i, j, k, l;
	
	ne=vari->extsExp;
	ni=vari->implExp;
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

	if(dtvIsArrayP(ni))
	{
		l=dtvArrayGetSize(ni);
		for(i=0; i<l; i++)
		{
			n0=dtvArrayGetIndexDtVal(ni, i);
			fn=BGBDT_TagStr_GetUtf8(n0);
			j=BS2C_LookupVariGlobal(ctx, vari, fn);
			if(j<0)continue;
			vi2=ctx->globals[j];
			vari->iface[i]=vi2;
		}
		vari->niface=l;
	}else if(dtvTrueP(ni))
	{
		fn=BGBDT_TagStr_GetUtf8(ni);
		i=BS2C_LookupVariGlobal(ctx, vari, fn);
		if(i>=0)
			{ vi2=ctx->globals[i]; }
		else
			{ vi2=NULL; }
		vari->iface[0]=vi2;
		vari->niface=1;
	}

	for(i=0; i<vari->nargs; i++)
	{
		BS2C_CompileRebuildVarType(ctx, vari->args[i]);
	}
}

/** Compile the functions within a context. */
BS2VM_API void BS2C_CompileFuncs(
	BS2CC_CompileContext *ctx)
{
	BS2C_CompileFuncs2(ctx, 0);
}

/** Compile the functions within a context starting from base. */
BS2VM_API void BS2C_CompileFuncs2(
	BS2CC_CompileContext *ctx, int base)
{
	BS2CC_VarInfo *vari;
	int ongbl;
	int i, j;

	ongbl=ctx->nglobals;
//	for(i=base; i<ctx->nglobals; i++)
	for(j=0; j<4; j++)
	{
		for(i=base; i<ongbl; i++)
		{
			vari=ctx->globals[i];
			if(!vari)
				continue;

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
	}
	
//	for(i=base; i<ctx->nglobals; i++)
	for(i=base; i<ongbl; i++)
	{
		vari=ctx->globals[i];
//		if(dtvNullP(vari->bodyExp))

		if(!vari)
			continue;

		if((vari->vitype==BS2CC_VITYPE_GBLFUNC) ||
			(vari->vitype==BS2CC_VITYPE_STRFUNC))
		{
			if(!dtvTrueP(vari->bodyExp))
				continue;
			BS2C_CompileFuncBody(ctx, vari);
		}

		if((vari->vitype==BS2CC_VITYPE_GBLVAR) ||
			(vari->vitype==BS2CC_VITYPE_STRVAR))
		{
			if(!dtvTrueP(vari->initExp))
				continue;
			BS2C_CompileVarInit(ctx, vari);
		}
	}
	
	/* Did compilation create more globals? */
	if(ongbl!=ctx->nglobals)
	{
		BS2C_CompileFuncs2(ctx, ongbl);
	}
	
//	if(vari->gid<65280)
//	{
//		vari->bty=vari->gid+256;
//	}
}

/**
  * Convert error message ID into a string description.
  * /param ix Index for the error.
  * /param cn Status code number for error.
  * /param pix Offset of parameters for message.
  */
BS2VM_API char *BS2C_ErrStringForMsg(
	BS2CC_CompileContext *ctx, int ix, int cn, int pix)
{
	char tb[256];
	char *s;

	switch(cn)
	{
	case BS2CC_ERRN_CASEERROR:
		s="(Internal) Unhandled Edge Case."; break;
	case BS2CC_ERRN_STUBERROR:
		s="(Internal) Stub Called."; break;
	case BS2CC_ERRN_DIVZERROR:
		s="Value divided by zero."; break;

	case BS2CC_ERRN_ERRNODECL:
		s="Undeclared Variable."; break;
	case BS2CC_ERRN_ERRTOOFEWARGS:
		s="Too few arguments for call."; break;
	case BS2CC_ERRN_ERRTOOMANYARGS:
		s="Too many arguments for call."; break;

	case BS2CC_ERRN_ERRBADCONV:
		s="Invalid Type Conversion."; break;
	case BS2CC_ERRN_ERRCANTDEREF:
		s="Type can't be dereferenced."; break;

	case BS2CC_ERRN_ERRSTACKMISAL:
		s="(Internal) Stack Misalignment Detected."; break;
	case BS2CC_ERRN_ERRSTACKUFLOW:
		s="(Internal) Stack Underflow Detected."; break;
	
	case BS2CC_ERRN_CONSTRANGE:
		s="Constant out of range."; break;
	case BS2CC_ERRN_CONVIMPLICIT:
		sprintf(tb, "Implicit type conversion '%s'->'%s'.",
			BS2C_GetTypeNameStr(ctx, ctx->cwparm[pix+1]),
			BS2C_GetTypeNameStr(ctx, ctx->cwparm[pix+0]));
		s=bgbdt_mm_rstrdup(tb); break;
	case BS2CC_ERRN_CONVNARROW:
		sprintf(tb, "Implicit conversion '%s'->'%s' may result in loss.",
			BS2C_GetTypeNameStr(ctx, ctx->cwparm[pix+1]),
			BS2C_GetTypeNameStr(ctx, ctx->cwparm[pix+0]));
		s=bgbdt_mm_rstrdup(tb); break;
	case BS2CC_ERRN_NOSEMICOLON:
		s="Missing Expected Semicolon."; break;
	case BS2CC_ERRN_NOTYPEDEF:
		s="Type Not Typedef."; break;

	default:
		sprintf(tb, "(Internal) Unknown %04X", cn);
		s=bgbdt_mm_rstrdup(tb); break;
//		s="(Internal) Unknown"; break;
	}
	return(s);
}

BS2VM_API void BS2C_DumpErrors(
	BS2CC_CompileContext *ctx)
{
	int i, j, k;
	
	if(ctx->ncwarn && !ctx->ncerr && !ctx->ncfatal)
	{
		for(i=0; i<ctx->ncwarn; i++)
		{
			bgbdt_mm_printf("%d:%d %04X %s\n",
				(ctx->cwarnln[i]>>20), ctx->cwarnln[i]&0xFFFFF,
				ctx->cwarn[i],
				BS2C_ErrStringForMsg(ctx, i,
					ctx->cwarn[i], ctx->cwarnpix[i]));
		}
		bgbdt_mm_printf("%d warnings\n", ctx->ncwarn);
	}
	
	if(ctx->ncerr)
	{
		for(i=0; i<ctx->ncerr; i++)
		{
			bgbdt_mm_printf("%d:%d %04X %s\n",
				(ctx->cerrln[i]>>20), ctx->cerrln[i]&0xFFFFF,
				ctx->cerr[i],
				BS2C_ErrStringForMsg(ctx, i,
					ctx->cerr[i], ctx->cerrpix[i]));
		}
		bgbdt_mm_printf("%d errors\n", ctx->ncerr);
	}

	if(ctx->ncfatal)
	{
		for(i=0; i<ctx->ncfatal; i++)
		{
			bgbdt_mm_printf("%d:%d %04X %s\n",
				(ctx->cfatalln[i]>>20), ctx->cfatalln[i]&0xFFFFF,
				ctx->cfatal[i],
				BS2C_ErrStringForMsg(ctx, i,
					ctx->cfatal[i], 0));
		}
		bgbdt_mm_printf("%d fatal errors\n", ctx->ncfatal);
	}
}

/**
  * Compile a module given as a string buffer.
  */
BS2VM_API int BS2C_CompileModuleBuffer(
	BS2CC_CompileContext *ctx, char *buf, int szbuf)
{
	dtVal v0;

	v0=BS2P_ParseBuffer(ctx, buf, szbuf);

//	BGBDT_MM_PrintValueToStrBuf(tb, 1023, v0);
//	BGBDT_MM_FormatValueToStrBuf(tb, 16383, v0);
//	printf("TB v0: %s\n", tb);
	
	BS2C_CompileTopStatement(ctx, v0);
	BS2C_CompileFuncs(ctx);

//	BS2C_DumpErrors2(ctx);
//	prn=BGBDT_MM_NewStringPrinter(tb, 16383);
//	BS2C_DisAsmFuncs(prn, ctx);
//	printf("%s\n", tb);

	return(0);
}

/**
  * Compile a list of modules within an existing compile context.
  * Modules are given as dotted names or as paths (without extensions).
  * /ctx Compiler context.
  * /param base Base path used when trying to load modules.
  * /param names List of names terminated with a NULL.
  */
BS2VM_API int BS2C_CompileModuleList(
	BS2CC_CompileContext *ctx,
	char *base, char **names)
{
	char tb[256], tb1[256];
	byte *tbuf;
	char *s, *t;
	int tsz;
	dtVal v0;
	int i, j, k;

	if(!ctx->nglobals)
		ctx->nglobals++;

	for(i=0; names[i]; i++)
	{
		s=names[i];
		t=tb1;
		while(*s)
		{
			if(*s=='.')
				{ *t++='/'; s++; continue; }
			*t++=*s++;
		}
		*t++=0;
	
		if(base)
		{
			sprintf(tb, "%s/%s.bs", base, tb1);
		}else
		{
			sprintf(tb, "%s.bs", tb1);
		}
	
		tbuf=bgbdt_mm_loadfile(tb, &tsz);
		if(!tbuf)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_FATALNOFILE);
		}

		ctx->srcfn=bgbdt_mm_strdup(tb);
		v0=BS2P_ParseBuffer(ctx, tbuf, tsz);
		
		if(ctx->dbgprn)
			{ BGBDT_MM_FormatValue(ctx->dbgprn, v0); }
		
		BS2C_CompileTopStatement(ctx, v0);

		bgbdt_mm_free(tbuf);
//		vf_freefdbuf(tbuf);
	}

	BS2C_CompileFuncs(ctx);

	if(ctx->dbgprn)
	{
		BS2C_DisAsmFuncs(ctx->dbgprn, ctx);
	}

	BS2C_DumpErrors(ctx);

	return(0);
}

/**
  * Compile an image based on a definition file.
  * /param img Name of file used for the produced bytecode image.
  * /param def Name of the definition file.
  * /return Status code for compile, negative means didn't compile.
  */
BS2VM_API int BS2C_CompileImageDef(char *img, char *def)
{
	char tb[256];
	char *mods[256];
	int nmods;

	BGBDT_MM_ParsePrintInfo *prn;
	BS2CC_CompileContext *ctx;
//	BSVM2_Context *vctx;
//	BSVM2_ImageGlobal *vi;

	char **a;
	char *tbuf, *cs, *cse;
	dtVal v0, v1, v2, v3;
	int tsz;
	int i, j, k;

	tbuf=bgbdt_mm_loadfile(def, &tsz);
	if(!tbuf)
		{ return(-1); }

	nmods=0;
	
	cs=tbuf; cse=tbuf+tsz;
	while(cs<cse)
	{
		cs=bgbdt_mm_rsgets(tb, 256, cs);
		a=bgbdt_mm_rsplit(tb);
		if(!a || !a[0])
			continue;
		if((*a[0]=='/') || (*a[0]=='#') || (*a[0]==';'))
			continue;

		if(!strcmp(a[0], "use"))
		{
			for(i=1; a[i]; i++)
				mods[nmods++]=bgbdt_mm_strdup(a[i]);
		}
	}

	bgbdt_mm_free(tbuf);
	mods[nmods]=0;

	ctx=BS2CC_AllocCompileContext();

	prn=BGBDT_MM_NewConsolePrinter();
	ctx->dbgprn=prn;

	BS2C_CompileModuleList(ctx, NULL, mods);

	if(ctx->ncerr || ctx->ncfatal)
	{
		BGBDT_MM_FreeParsePrintInfo(prn);
		BS2P_FreeCompileContext(ctx);
		return(-1);
	}

	tbuf=bgbdt_mm_malloc(1<<20);

	i=BS2C_FlattenImage(ctx, tbuf, 1<<20);
	if(i>=0)
	{
		bgbdt_mm_storefile(img, tbuf, i);
	}

	bgbdt_mm_free(tbuf);

	BGBDT_MM_FreeParsePrintInfo(prn);
	BS2P_FreeCompileContext(ctx);

	return(0);
}

/** Evaluate Expression.
  * This uses a special interpreter for evaluating expressions.
  * This allows lower overhead, but its capabilities are limited.
  */
BS2VM_API dtVal BS2C_EvalExpr(char *str)
{
	BS2CC_CompileContext *ctx;
	dtVal n0, v;
	
	ctx=BS2CC_AllocCompileContext();
	n0=BS2P_ParseBufferExpression(ctx, str, strlen(str));
	v=BS2E_EvalExpr(ctx, n0);
	BS2P_FreeCompileContext(ctx);
	return(v);
}
