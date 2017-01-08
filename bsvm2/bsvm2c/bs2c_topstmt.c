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


void BS2C_CompileSetupVarInfo(
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vi, dtVal expr)
{
	char tb[256];
	BS2CC_VarInfo *vi2, *vi3, *vi4;
	dtVal nn, nt, ni, na, nb;
	dtVal n0, n1;
	s64 bmfl;
	char *name, *tag;
	int bty, tk;
	int i, j, k, l;

	tag=BS2P_GetAstNodeTag(expr);

	if(!strcmp(tag, "var"))
	{
		name=BS2P_GetAstNodeAttrS(expr, "name");
		bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
		nt=BS2P_GetAstNodeAttr(expr, "type");
		ni=BS2P_GetAstNodeAttr(expr, "init");

//		bty=BS2C_TypeBaseType(ctx, nt);
		bty=BS2C_TypeExtType(ctx, nt);

		vi->name=BS2P_StrSym(ctx, name);
		vi->bty=bty;
		vi->bmfl=bmfl;
		vi->typeExp=nt;
		vi->initExp=ni;
		
		if(!vi->vitype)
			vi->vitype=BS2CC_VITYPE_LCLVAR;
		return;
	}

	if(!strcmp(tag, "vararg"))
	{
		name=BS2P_GetAstNodeAttrS(expr, "name");

//		vi->name=NULL;
		vi->name=BS2P_StrSym(ctx, name);
		vi->bty=BS2CC_TYZ_VARARG;
		vi->bmfl=0;
//		vi->typeExp=nt;

		if(!vi->vitype)
			vi->vitype=BS2CC_VITYPE_LCLVAR;

		if(ctx->frm->jcleanup<=0)
			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);

		return;
	}

	if(!strcmp(tag, "func") ||
		!strcmp(tag, "func_aut"))
	{
		name=BS2P_GetAstNodeAttrS(expr, "name");
		bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
		nt=BS2P_GetAstNodeAttr(expr, "type");
		na=BS2P_GetAstNodeAttr(expr, "args");
		nb=BS2P_GetAstNodeAttr(expr, "body");
		tk=BS2P_GetAstNodeAttrI(expr, "tokcnt");

//		bty=BS2C_TypeBaseType(ctx, nt);
		bty=BS2C_TypeExtType(ctx, nt);

		vi->name=BS2P_StrSym(ctx, name);
		vi->bty=bty;
		vi->bmfl=bmfl;
		vi->typeExp=nt;

//		if(!vi->vitype)
			vi->vitype=BS2CC_VITYPE_LCLFUNC;

#if 0
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
#endif

		vi->bodyExp=nb;

#if 1
		i=BS2C_CompileTopFunc(ctx, expr);
		vi2=ctx->globals[i];

		vi->bty=256+i;

		n0=BS2P_GetAstNodeAttr(expr, "body");
		BS2C_InferCaptureFunc(ctx, vi2);

		if(vi2->body->inf_ncapvar==0)
		{
			return;
		}

//		fn=vi2->name;
		
		vi2->niface=vi2->body->inf_ncapvar;
		for(i=0; i<vi2->niface; i++)
		{
			vi3=BS2C_AllocVarInfo(ctx);
			vi2->iface[i]=vi3;
			vi3->name=BS2P_StrSym(ctx, vi2->body->inf_capvar[i]);
			
			k=BS2C_LookupLocal(ctx, vi3->name);
			vi4=ctx->frm->locals[k];
			vi3->bty=BS2C_TypeRefType(ctx, vi4->bty);
			vi3->bmfl=vi4->bmfl;
			vi3->typeExp=vi4->typeExp;
		}
#endif

		if(ctx->frm->jcleanup<=0)
			ctx->frm->jcleanup=BS2C_GenTempLabel(ctx);

		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileFunVar(BS2CC_CompileContext *ctx, dtVal expr)
{
	char tb[256];
	BS2CC_VarInfo *vi;
	s64 bmfl;
	int i;

	bmfl=BS2P_GetAstNodeAttrI(expr, "modi");

	if(bmfl&BS2CC_TYFL_STATIC)
	{
		vi=BS2C_AllocVarInfo(ctx);

		i=ctx->nglobals++;
		ctx->globals[i]=vi;
		vi->gid=i;
		vi->vitype=BS2CC_VITYPE_GBLVAR;

		vi->pkg=ctx->pkg;
		vi->obj=ctx->obj;

		BS2C_CompileSetupVarInfo(ctx, vi, expr);

		sprintf(tb, "%s/%s", ctx->frm->func->qname, vi->name);
		vi->qname=BS2P_StrSym(ctx, tb);

		return;
	}

	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->frm->nlocals++;
	ctx->frm->locals[i]=vi;
	vi->vitype=BS2CC_VITYPE_LCLVAR;
	
	BS2C_CompileSetupVarInfo(ctx, vi, expr);
	
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

	if(!(vi->bmfl&BS2CC_TYFL_PPP))
	{
		if(ctx->obj->vitype==BS2CC_VITYPE_STRUCT)
			{ vi->bmfl|=BS2CC_TYFL_PUBLIC; }
		else
			{ vi->bmfl|=BS2CC_TYFL_PROTECTED; }
	}

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
	int i, j, k, l, tk;

	name=BS2P_GetAstNodeAttrS(expr, "name");
	bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
	nt=BS2P_GetAstNodeAttr(expr, "type");
	na=BS2P_GetAstNodeAttr(expr, "args");
	nb=BS2P_GetAstNodeAttr(expr, "body");
	tk=BS2P_GetAstNodeAttrI(expr, "tokcnt");
	
	if(dtvTrueP(nt))
	{
		bty=BS2C_TypeBaseType(ctx, nt);
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
	

	if(!(bmfl&BS2CC_TYFL_PPP))
	{
		if(vi->vitype==BS2CC_VITYPE_STRUCT)
			{ bmfl|=BS2CC_TYFL_PUBLIC; }
		else
			{ bmfl|=BS2CC_TYFL_PROTECTED; }
	}

	vi->name=BS2P_StrSym(ctx, name);
//	vi->bty=BSVM2_OPZ_ADDRESS;
	vi->bty=vi->gid+256;
	vi->rty=bty;
	vi->typeExp=nt;
	vi->bmfl=bmfl;
	vi->tokcnt=tk;

	vi->pkg=ctx->pkg;
	vi->obj=ctx->obj;

//	vi->def_rlcty=1;
//	if(tk>=2048)vi->def_rlcty=2;
//	if(tk>=524288)vi->def_rlcty=3;

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
	dtVal nn, nt, ni;
	s64 bmfl;
	char *name;
	int bty;
	int i;

	name=BS2P_GetAstNodeAttrS(expr, "name");
	bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
	nt=BS2P_GetAstNodeAttr(expr, "type");
	ni=BS2P_GetAstNodeAttr(expr, "init");

	if(!(bmfl&BS2CC_TYFL_PPP))
		{ bmfl|=BS2CC_TYFL_PROTECTED; }

	bty=BS2C_TypeBaseType(ctx, nt);

	vi->name=BS2P_StrSym(ctx, name);
	vi->bty=bty;
	vi->bmfl=bmfl;
	vi->typeExp=nt;
	vi->initExp=ni;

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

int BS2C_CompileTopFunc(BS2CC_CompileContext *ctx, dtVal expr)
{
	char tb[256];
	dtVal n0, n1, n2, n3;
	dtVal nn, nt, na, nb;
	s64 bmfl;
	BS2CC_VarInfo *vi;
	BS2CC_CcFrame *frm;
	char *name;
	int bty, tk;
	int i, j, k, l;

	name=BS2P_GetAstNodeAttrS(expr, "name");
	bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
	nt=BS2P_GetAstNodeAttr(expr, "type");
	na=BS2P_GetAstNodeAttr(expr, "args");
	nb=BS2P_GetAstNodeAttr(expr, "body");
	tk=BS2P_GetAstNodeAttrI(expr, "tokcnt");

	if(!(bmfl&BS2CC_TYFL_PPP))
		{ bmfl|=BS2CC_TYFL_PROTECTED; }
	
	bty=BS2C_TypeBaseType(ctx, nt);
	
	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->vitype=BS2CC_VITYPE_GBLFUNC;
	
	vi->name=BS2P_StrSym(ctx, name);
//	vi->bty=BSVM2_OPZ_ADDRESS;
	vi->bty=vi->gid+256;
	vi->rty=bty;
	vi->typeExp=nt;
	vi->bmfl=bmfl;
	vi->tokcnt=tk;

//	vi->def_rlcty=1;
//	if(tk>=2048)vi->def_rlcty=2;
//	if(tk>=524288)vi->def_rlcty=3;

	if(ctx->pkg)
	{
		vi->pkg=ctx->pkg;
		vi->pknext=ctx->pkg->vars;
		ctx->pkg->vars=vi;
	}
	
//	if(ctx->pkg && ctx->pkg->qname)
	if(ctx->pkg && ctx->pkg->qname && strncmp(name, "$GS$", 4))
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
	return(vi->gid);
}

void BS2C_CompileTopStruct(BS2CC_CompileContext *ctx, dtVal expr)
{
	char tb[256];
	dtVal n0, n1, n2, n3;
	dtVal nn, nt, na, nb, ne, ni;
	s64 bmfl;
	BS2CC_VarInfo *vi;
	BS2CC_CcFrame *frm;
	char *name, *tag;
	int bty;
	int i, j, k, l;

	name=BS2P_GetAstNodeAttrS(expr, "name");
	bmfl=BS2P_GetAstNodeAttrI(expr, "modi");
	ne=BS2P_GetAstNodeAttr(expr, "exts");
	ni=BS2P_GetAstNodeAttr(expr, "impl");
	nb=BS2P_GetAstNodeAttr(expr, "body");
	tag=BS2P_GetAstNodeTag(expr);
	
	if(!(bmfl&BS2CC_TYFL_PPP))
		bmfl|=BS2CC_TYFL_PROTECTED;
	
	vi=BS2C_AllocVarInfo(ctx);
	i=ctx->nglobals++;
	ctx->globals[i]=vi;
	vi->gid=i;
	vi->bmfl=bmfl;
	vi->vitype=BS2CC_VITYPE_STRUCT;
	if(!strcmp(tag, "class"))
		vi->vitype=BS2CC_VITYPE_CLASS;
	if(!strcmp(tag, "interface"))
		vi->vitype=BS2CC_VITYPE_IFACE;
	
	vi->name=BS2P_StrSym(ctx, name);
//	vi->bty=BSVM2_OPZ_ADDRESS;
	vi->bty=vi->gid+256;

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


BS2VM_API void BS2C_CompileTopStatement(
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
		!strcmp(tag, "interface"))
	{
		BS2C_CompileTopStruct(ctx, expr);
		return;
	}
	
	BS2C_CaseError(ctx);
}
