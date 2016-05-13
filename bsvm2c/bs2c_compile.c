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

void BS2C_ErrStackMisalign(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRSTACKMISAL);
}

void BS2C_ErrStackUnderflow(BS2CC_CompileContext *ctx)
{
	BS2C_CompileError(ctx, BS2CC_ERRN_ERRSTACKUFLOW);
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

void BS2C_CompileFuncBodyCleanupVar(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vi, int ix)
{
	BS2CC_VarInfo *vi2;
	int bty, sz, z;
	int i;

	bty=vi->bty;

	if(BS2C_TypeSizedArrayP(ctx, bty))
	{
		sz=BS2C_TypeGetArraySize(ctx, bty);
		z=BS2C_GetTypeBaseZ(ctx, bty);

		BS2C_EmitOpcode(ctx, BSVM2_OP_DFXARR);
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

		BS2C_EmitOpcode(ctx, BSVM2_OP_DFXOBJ);
		BS2C_EmitOpcodeUCx(ctx, ix);
		BS2C_EmitOpcodeUCx(ctx, i);
		return;
	}

	if(BS2C_TypeX128P(ctx, bty))
	{
		BS2C_EmitOpcode(ctx, BSVM2_OP_DFXX);
		BS2C_EmitOpcodeUCx(ctx, ix);
		return;
	}
}

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
		BS2C_CompileExprPushType(ctx, ctx->frm->func->rty);
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
	
	ctx->frm=ctx->frmstack[i];
	ctx->frmstackpos=i;
}

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
}

BTEIFGL_API void BS2C_CompileFuncs(
	BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *vari;
	int i;

	for(i=0; i<ctx->nglobals; i++)
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
	
	for(i=0; i<ctx->nglobals; i++)
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

BTEIFGL_API int BS2C_CompileModuleBuffer(
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

BTEIFGL_API int BS2C_CompileModuleList(
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
	
		tbuf=vf_loadfile(tb, &tsz);
		if(!tbuf)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_FATALNOFILE);
		}

		v0=BS2P_ParseBuffer(ctx, tbuf, tsz);
		
		if(ctx->dbgprn)
			{ BGBDT_MM_FormatValue(ctx->dbgprn, v0); }
		
		BS2C_CompileTopStatement(ctx, v0);

		vf_freefdbuf(tbuf);
	}

	BS2C_CompileFuncs(ctx);

	if(ctx->dbgprn)
	{
		BS2C_DisAsmFuncs(ctx->dbgprn, ctx);
	}
	return(0);
}
