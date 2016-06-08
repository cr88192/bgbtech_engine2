#include <bteifgl.h>

dtVal BS2P_ParseInnerStatement(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	int i, j, k;

	t0=BS2P_PeekToken(ctx, 0);

	if(t0 && (*t0=='I'))
	{
		if(!strcmp(t0, "Ibreak"))
		{
			BS2P_NextToken(ctx);
			i=1;
			
			t0=BS2P_PeekToken(ctx, 0);
			if(*t0=='|')
			{
				i=atoi(t0+1);
				BS2P_NextToken(ctx);
			}else if(strcmp(t0, "X;"))
			{
				n1=BS2P_ParseExpr(ctx);
				i=dtvUnwrapLong(n1);
			}
			
			n0=BS2P_ParseWrapSimpleTagInt(ctx, "break", i);
//			t0=BS2P_PeekToken(ctx, 0);
			return(n0);
		}

		if(!strcmp(t0, "Icontinue"))
		{
			BS2P_NextToken(ctx);
			i=0;
			
			t0=BS2P_PeekToken(ctx, 0);
			if(*t0=='|')
			{
				i=atoi(t0+1);
				BS2P_NextToken(ctx);
			}else if(strcmp(t0, "X;"))
			{
				n1=BS2P_ParseExpr(ctx);
				i=dtvUnwrapLong(n1);
			}
			
			n0=BS2P_ParseWrapSimpleTagInt(ctx, "continue", i);
//			t0=BS2P_PeekToken(ctx, 0);
			return(n0);
		}

		if(!strcmp(t0, "Idelete"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
//			n1=BS2P_ParseExpr(ctx);
			n1=BS2P_ParseExprList(ctx);
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "delete", n1);
			return(n0);
		}

		if(!strcmp(t0, "Igoto"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExpr(ctx);
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "goto", n1);
			return(n0);
		}

		if(!strcmp(t0, "Ireturn"))
		{
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			if(strcmp(t0, "X;"))
				{ n1=BS2P_ParseExpr(ctx); }
			else
				{ n1=DTV_NULL; }
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "return", n1);
			return(n0);
		}

		if(!strcmp(t0, "Ithrow"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExpr(ctx);
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "throw", n1);
			return(n0);
		}
	}
	
	return(DTV_NULL);
	
//	n0=BS2P_ParseExpr(ctx);
//	return(n0);
}

dtVal BS2P_ParseStatementBlock(BS2CC_CompileContext *ctx, int flag)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2, *lfn;
	dtVal stmt[4096];
	int nstmt, lln;

	nstmt=0;
	t0=BS2P_PeekToken(ctx, 0);
	while(t0 && *t0)
	{
		if(!strcmp(t0, "X}"))
			break;

		BS2P_GetCurSourceLine(ctx, &lfn, &lln);

		n0=BS2P_ParseBlockStatement(ctx);

		if(BGBDT_MapObj_IsObjectP(n0))
		{
			BS2P_SetAstNodeAttrStr(n0, "fn", lfn);
			BS2P_SetAstNodeAttrI(n0, "ln", lln);
		}
		
		stmt[nstmt++]=n0;
		t0=BS2P_PeekToken(ctx, 0);
	}
	
	if(!nstmt)
	{
		n0=BS2P_ParseWrapSimpleTag(ctx, "empty_block");
		return(n0);
	}
	
	if(nstmt==1)
	{
		return(stmt[0]);
	}
	
	n1=BS2P_NewAstWrapArray(ctx, stmt, nstmt);
	n0=BS2P_ParseWrapSimpleTagVal(ctx, "block", n1);
	return(n0);
}

dtVal BS2P_ParseBlockStatement(BS2CC_CompileContext *ctx)
{
	return(BS2P_ParseBlockStatementI(ctx, 0));
}

dtVal BS2P_ParseBlockStatementTail(BS2CC_CompileContext *ctx)
{
	return(BS2P_ParseBlockStatementI(ctx, 1));
}

dtVal BS2P_ParseBlockStatementI(BS2CC_CompileContext *ctx, int flag)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2;
	int i, j, k;

	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);

	if(t0 && !strcmp(t0, "X{"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseStatementBlock(ctx, flag);
		BS2P_ParseExpectToken(ctx, "X}");
		return(n0);
	}

	if(t0 && (*t0=='I'))
	{
		if(!strcmp(t0, "Icase"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X:");

			n0=BS2P_ParseWrapSimpleTagVal(ctx, "case", n1);
			return(n0);
		}

		if(!strcmp(t0, "Idefault"))
		{
			BS2P_NextToken(ctx);
//			n1=BS2P_ParseExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X:");

			n0=BS2P_ParseWrapSimpleTag(ctx, "case_default");
			return(n0);
		}

		if(!strcmp(t0, "Ifor"))
		{
			BS2P_NextToken(ctx);
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseOptExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X;");
			n1=BS2P_ParseOptExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X;");
			n2=BS2P_ParseOptExpr(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n3=BS2P_ParseBlockStatement(ctx);
			
			n0=BS2P_ParseWrapTagFor(ctx, "for", n0, n1, n2, n3);
			return(n0);
		}

		if(!strcmp(t0, "Iif"))
		{
			BS2P_NextToken(ctx);
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n1=BS2P_ParseBlockStatement(ctx);

			if(BS2P_ParseExpectOptToken(ctx, "Ielse"))
			{
				n2=BS2P_ParseBlockStatement(ctx);
				n0=BS2P_ParseWrapTagIfElse(ctx, "ifelse", n0, n1, n2);
			}else
			{
				n0=BS2P_ParseWrapTagIf(ctx, "if", n0, n1);
			}
			return(n0);
		}

		if(!strcmp(t0, "Iswitch"))
		{
			BS2P_NextToken(ctx);
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n1=BS2P_ParseBlockStatement(ctx);
			n0=BS2P_ParseWrapTagIf(ctx, "switch", n0, n1);
			return(n0);
		}

		if(!strcmp(t0, "Iwhile"))
		{
			BS2P_NextToken(ctx);
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n1=BS2P_ParseBlockStatement(ctx);
			n0=BS2P_ParseWrapTagIf(ctx, "while", n0, n1);
			return(n0);
		}

		if(!strcmp(t0, "Ido"))
		{
			BS2P_NextToken(ctx);

			n1=BS2P_ParseBlockStatement(ctx);

			BS2P_ParseExpectToken(ctx, "Iwhile");
			
			BS2P_ParseExpectToken(ctx, "X(");
			n0=BS2P_ParseExprAssignOp(ctx);
			BS2P_ParseExpectToken(ctx, "X)");
			n0=BS2P_ParseWrapTagIf(ctx, "do_while", n0, n1);
			BS2P_ParseExpectOptToken(ctx, "X;");
			return(n0);
		}

		if(!strcmp(t1, "X:"))
		{
			BS2P_NextToken(ctx);
			BS2P_NextToken(ctx);

			n0=BS2P_ParseWrapSimpleCmdStr(ctx, "label", t0+1);
			return(n0);
		}
	}

	n0=BS2P_TryParseDecl(ctx);
	if(dtvTrueP(n0))
		return(n0);

	n0=BS2P_ParseInnerStatement(ctx);
	if(dtvTrueP(n0))
	{
		BS2P_ParseExpectToken(ctx, "X;");
		return(n0);
	}
	
//	n0=BS2P_ParseInnerStatement(ctx);
	n0=BS2P_ParseExpr(ctx);
	if(dtvTrueP(n0))
	{
		if(!BS2P_ParseExpectOptToken(ctx, "X;"))
		{
			t0=BS2P_PeekToken(ctx, 0);
			if(t0 && !strcmp(t0, "X}"))
				n0=BS2P_ParseWrapSimpleTagVal(ctx, "tail", n0);
		}else if((flag&1) &&
			!BS2P_CheckAstNodeTag(n0, "assign") &&
			!BS2P_CheckAstNodeTag(n0, "assignop"))
		{
			n0=BS2P_ParseWrapSimpleTagVal(ctx, "tail", n0);
		}
	}else
	{
		BS2P_ParseExpectOptToken(ctx, "X;");
	}

	return(n0);
}


dtVal BS2P_ParsePackageStatement(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2;
	int i, j, k;

	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);

	if(t0 && !strcmp(t0, "X{"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParsePackageStatementBlock(ctx);
		BS2P_ParseExpectToken(ctx, "X}");
		return(n0);
	}

#if 0
	if(t0 && (*t0=='I'))
	{
		if(!strcmp(t0, "Ipackage"))
		{
			BS2P_NextToken(ctx);
			i=0;
		}
	}
#endif

	n0=BS2P_TryParseDecl(ctx);
	if(dtvTrueP(n0))
		return(n0);

	return(DTV_NULL);
}

dtVal BS2P_ParsePackageStatementBlock(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2, *lfn;
	dtVal stmt[4096];
	int nstmt, lln;

	nstmt=0;
	t0=BS2P_PeekToken(ctx, 0);
	while(t0 && *t0)
	{
		if(!strcmp(t0, "X}"))
			break;
		BS2P_GetCurSourceLine(ctx, &lfn, &lln);
		n0=BS2P_ParsePackageStatement(ctx);
		
		if(BGBDT_MapObj_IsObjectP(n0))
		{
			BS2P_SetAstNodeAttrStr(n0, "fn", lfn);
			BS2P_SetAstNodeAttrI(n0, "ln", lln);
		}
		
		stmt[nstmt++]=n0;
		t0=BS2P_PeekToken(ctx, 0);
	}
	
	n1=BS2P_NewAstWrapArray(ctx, stmt, nstmt);
	n0=BS2P_ParseWrapSimpleTagVal(ctx, "psblock", n1);
	return(n0);
}


BTEIFGL_API dtVal BS2P_ParseBuffer(BS2CC_CompileContext *ctx,
	char *srcbuf, int szsrcbuf)
{
	dtVal n0;

	BS2P_LexBuffer(ctx, srcbuf, szsrcbuf);
	n0=BS2P_ParsePackageStatementBlock(ctx);
	
	return(n0);
}

BTEIFGL_API dtVal BS2P_ParseBufferExpression(BS2CC_CompileContext *ctx,
	char *srcbuf, int szsrcbuf)
{
	dtVal n0;

	BS2P_LexBuffer(ctx, srcbuf, szsrcbuf);
	n0=BS2P_ParseExpr(ctx);
	
	return(n0);
}

BTEIFGL_API BS2CC_CompileContext *BS2CC_AllocCompileContext()
{
	BS2CC_CompileContext *ctx;
	
	ctx=dtmAlloc("bs2cc_compilecontext_t",
		sizeof(BS2CC_CompileContext));
	ctx->gsseq=1;
	return(ctx);
}

BTEIFGL_API void BS2P_FreeCompileContext(BS2CC_CompileContext *ctx)
{
	dtmFree(ctx);
}