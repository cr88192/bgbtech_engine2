#include <bteifgl.h>

void BS2P_RaiseError(BS2CC_CompileContext *ctx, char *errn)
{
}

void BS2P_RaiseError1(BS2CC_CompileContext *ctx, char *errn,
	char *parm0)
{
}

dtVal BS2P_ParseWrapStringL(BS2CC_CompileContext *ctx, char *str)
{
	u16 tb[1024];
	char *s, *se;
	u16 *t, *tbuf;
	int i, j, k, l;
	
	l=strlen(str);
	if(l>1021)
	{
		i=256;
		while(i<(l+3))
			i=i+(i>>1);
		tbuf=malloc(i*2);
		t=tbuf;
	}else
	{
		tbuf=NULL;
		t=tb;
	}

	s=str; se=str+strlen(s);
//	t=tb;
	while(*s && (s<se))
	{
		s=BS2P_ReadUtf8(s, &i);
		if(i>=65536)
		{
			*t++=0xD800|(i>>10)&1023;
			*t++=0xDC00|(i    )&1023;
			continue;
		}
		*t++=i;
	}
	*t++=0;

	return(BGBDT_TagStr_String16(tb));
}

dtVal BS2P_ParseStringMulti(BS2CC_CompileContext *ctx)
{
	char *strbuf, *strbufe;
	char *t0, *t1, *t2;
	char *ct;
	dtVal val;
	int i, j, k;

	strbuf=malloc(4096);
	strbufe=strbuf+4096;
	ct=strbuf;

	t0=BS2P_PeekToken(ctx, 0);
	while(t0 && (*t0=='S'))
	{
		i=strlen(t0+1);
		if((ct+i+1)>=strbufe)
		{
			j=ct-strbuf;
			k=strbufe-strbuf;
			k=k+(k>>1);
			strbuf=realloc(strbuf, k);
			strbufe=strbuf+k;
			ct=strbuf+j;
		}
		
		memcpy(ct, t0+1, i);
		ct+=i;

		BS2P_NextToken(ctx);
		t0=BS2P_PeekToken(ctx, 0);
	}
	*ct=0;

	if(t0 && (*t0=='I'))
	{
		if(!strcmp(t0, "IL") || !strcmp(t0, "Il"))
		{
			BS2P_NextToken(ctx);
			val=BS2P_ParseWrapStringL(ctx, strbuf);
			free(strbuf);
			return(val);
		}
	}

	val=BS2P_ParseWrapString(ctx, strbuf);
	free(strbuf);
	return(val);
}

dtVal BS2P_ParseStringTriple(BS2CC_CompileContext *ctx)
{
	char *strbuf, *strbufe;
	char *t0, *t1, *t2;
	char *cs, *ct;
	dtVal val;
	int i, j, k;

	strbuf=malloc(4096);
	strbufe=strbuf+4096;
	ct=strbuf;
	cs=BS2P_GetCurSrcPosition(ctx, 0);

	while(*cs)
	{
		if((cs[0]=='\"') && (cs[1]=='\"') && (cs[2]=='\"'))
		{
			cs+=3;
			break;
		}
	
		if((ct+1)>=strbufe)
		{
			j=ct-strbuf;
			k=strbufe-strbuf;
			k=k+(k>>1);
			strbuf=realloc(strbuf, k);
			strbufe=strbuf+k;
			ct=strbuf+j;
		}
		*ct++=*cs++;
	}
	*ct=0;
	
	BS2P_SetCurSrcPosition(ctx, cs);
	t0=BS2P_PeekToken(ctx, 0);

	if(t0 && (*t0=='I'))
	{
		if(!strcmp(t0, "IL") || !strcmp(t0, "Il"))
		{
			BS2P_NextToken(ctx);
			val=BS2P_ParseWrapStringL(ctx, strbuf);
			free(strbuf);
			return(val);
		}
	}

	val=BS2P_ParseWrapString(ctx, strbuf);
	free(strbuf);
	return(val);
}


void BS2P_ParseExpectToken(BS2CC_CompileContext *ctx, char *tok)
{
	char *t0, *t1, *t2;

	t0=BS2P_PeekToken(ctx, 0);
	if(!strcmp(t0, tok))
		{ BS2P_NextToken(ctx); }
	else
		{ BS2P_RaiseError1(ctx, "ParserExpectToken", tok); }
}

int BS2P_ParseExpectOptToken(BS2CC_CompileContext *ctx, char *tok)
{
	char *t0, *t1, *t2;

	t0=BS2P_PeekToken(ctx, 0);
	if(!strcmp(t0, tok))
	{
		BS2P_NextToken(ctx);
		return(1);
	}
	return(0);
}


dtVal BS2P_ParseLitExpr(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);
	t2=BS2P_PeekToken(ctx, 2);
	
	if(*t0=='#')
	{
		if(!strcmp(t1, "IF") || !strcmp(t1, "If"))
		{
			BS2P_NextTokenN(ctx, 2);
			return(dtvWrapFloatF(atof(t0+1)));

		}
	
		BS2P_NextToken(ctx);
		return(dtvWrapDoubleF(atof(t0+1)));
	}

	if(*t0=='|')
	{
		if(t1 && *t1=='I')
		{
			if(!strcmp(t1, "II") || !strcmp(t1, "Ii"))
			{
				BS2P_NextTokenN(ctx, 2);
				return(dtvWrapUIntF(frgl_ratoi(t0+1)));
			}

			if(!strcmp(t1, "IU") || !strcmp(t1, "Iu") ||
				!strcmp(t1, "IUI") || !strcmp(t1, "Iui"))
			{
				BS2P_NextTokenN(ctx, 2);
				return(dtvWrapUIntF(frgl_ratoi(t0+1)));
			}

			if(!strcmp(t1, "IL") || !strcmp(t1, "Il"))
			{
				BS2P_NextTokenN(ctx, 2);
				return(dtvWrapLongF(frgl_ratoi(t0+1)));
			}

			if(!strcmp(t1, "IUL") || !strcmp(t1, "Iul"))
			{
				BS2P_NextTokenN(ctx, 2);
				return(dtvWrapLongF(frgl_ratoi(t0+1)));
			}

			BS2P_NextTokenN(ctx, 2);

			li=frgl_ratoi(t0+1);
			if((li>=(1LL<<31)) || (li<(-(1LL<<31))))
			{
				return(dtvWrapLongF(li));
			}
			return(dtvWrapIntF(li));
		}


		BS2P_NextToken(ctx);

		li=frgl_ratoi(t0+1);
		if((li>=(1LL<<31)) || (li<(-(1LL<<31))))
		{
			return(dtvWrapLongF(li));
		}

		return(dtvWrapIntF(li));
	}
	
	if(*t0=='S')
	{
		if(t1 && (*t1=='S'))
		{
			return(BS2P_ParseStringMulti(ctx));
		}

		if(t1 && (*t1=='I'))
		{
			if(!strcmp(t1, "IL") || !strcmp(t1, "Il"))
			{
				BS2P_NextTokenN(ctx, 2);
				return(BS2P_ParseWrapStringL(ctx, t0+1));
			}
		}

		BS2P_NextToken(ctx);
		return(BS2P_ParseWrapString(ctx, t0+1));
	}

	if((*t0=='X') && !strcmp(t0, "X\"\"\""))
	{
		BS2P_NextToken(ctx);
		return(BS2P_ParseStringTriple(ctx));
	}

	if(*t0=='C')
	{
		BS2P_ReadUtf8(t0+1, &i);
		BS2P_NextToken(ctx);
		return(dtvWrapChar(i));
	}
	
	if(*t0=='I')
	{
		if(!strcmp(t0, "Inew"))
		{
			BS2P_NextToken(ctx);

			n1=BS2P_ParseTypeExpr(ctx);

			t0=BS2P_PeekToken(ctx, 0);
			if(!strcmp(t0, "X("))
			{
				BS2P_NextToken(ctx);
//				n2=BS2P_ParseFunVars(ctx);
				n2=BS2P_ParseExprList(ctx);
				BS2P_ParseExpectToken(ctx, "X)");

				if(!dtvTrueP(n2))
					n2=DTV_EMPTYLIST;
			}else
			{
				n2=DTV_NULL;
			}

			n0=BS2P_NewAstNode(ctx, "new");
			BS2P_SetAstNodeAttr(n0, "type", n1);
			if(dtvTrueP(n2))
				BS2P_SetAstNodeAttr(n0, "args", n2);

			return(n0);
		}
	
		BS2P_NextToken(ctx);
		return(BS2P_ParseWrapIdentifier(ctx, t0+1));
	}

	if(!strcmp(t0, "X("))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseExprList(ctx);

//		t0=BS2P_PeekToken(ctx, 0);
//		if(!strcmp(t0, "#)"))
//		{
//			BS2P_NextToken(ctx);
//		}else
//		{
//			BS2P_RaiseError1(ctx, "ParserExpectToken", "X)");
//		}

		BS2P_ParseExpectToken(ctx, "X)");
		
		return(n0);
	}

	if(!strcmp(t0, "X{"))
	{
		BS2P_NextToken(ctx);

//		n0=BS2P_ParseExprList(ctx);
		n0=BS2P_ParseNameExprList(ctx);

//		t0=BS2P_PeekToken(ctx, 0);
//		if(!strcmp(t0, "#}"))
//		{
//			BS2P_NextToken(ctx);
//		}else
//		{
//			BS2P_RaiseError1(ctx, "ParserExpectToken", "X}");
//		}

		BS2P_ParseExpectToken(ctx, "X}");
		
		n0=BS2P_ParseWrapObject(ctx, n0);
		
		return(n0);
	}

	if(!strcmp(t0, "X["))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseExprList(ctx);

//		t0=BS2P_PeekToken(ctx, 0);
//		if(!strcmp(t0, "#]"))
//		{
//			BS2P_NextToken(ctx);
//		}else
//		{
//			BS2P_RaiseError1(ctx, "ParserExpectToken", "X]");
//		}

		BS2P_ParseExpectToken(ctx, "X]");

		t0=BS2P_PeekToken(ctx, 0); t2=NULL;
		if(!strcmp(t0, "II") || !strcmp(t0, "Ii"))t2="SI";
		if(!strcmp(t0, "ISI") || !strcmp(t0, "Isi"))t2="SI";
		if(!strcmp(t0, "IUI") || !strcmp(t0, "Iui"))t2="UI";
		if(!strcmp(t0, "IL") || !strcmp(t0, "Il"))t2="SL";
		if(!strcmp(t0, "ISL") || !strcmp(t0, "Isl"))t2="SL";
		if(!strcmp(t0, "IUL") || !strcmp(t0, "Iul"))t2="UL";
		if(!strcmp(t0, "IS") || !strcmp(t0, "Is"))t2="SS";
		if(!strcmp(t0, "ISS") || !strcmp(t0, "Iss"))t2="SS";
		if(!strcmp(t0, "IUS") || !strcmp(t0, "Ius"))t2="US";
		if(!strcmp(t0, "ISB") || !strcmp(t0, "Isb"))t2="SB";
		if(!strcmp(t0, "IUB") || !strcmp(t0, "Iub"))t2="UB";
		if(!strcmp(t0, "IB") || !strcmp(t0, "Ib"))t2="B";
		if(!strcmp(t0, "IF") || !strcmp(t0, "If"))t2="F";
		if(!strcmp(t0, "ID") || !strcmp(t0, "Id"))t2="D";
		if(!strcmp(t0, "IC") || !strcmp(t0, "Ic"))t2="C";
		if(!strcmp(t0, "IW") || !strcmp(t0, "Iw"))t2="W";
		
		if(t2)
			{ BS2P_NextToken(ctx); }
		
		n0=BS2P_ParseWrapArray(ctx, n0);

		if(t2)
			BS2P_SetAstNodeAttrStr(n0, "sfx", t2);
		
		return(n0);
	}

	if(!strcmp(t0, "X#["))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseExprList(ctx);

		BS2P_ParseExpectToken(ctx, "X]");

		t0=BS2P_PeekToken(ctx, 0); t2=NULL;
		if(!strcmp(t0, "II") || !strcmp(t0, "Ii"))t2="SI";
		if(!strcmp(t0, "ISI") || !strcmp(t0, "Isi"))t2="SI";
		if(!strcmp(t0, "IUI") || !strcmp(t0, "Iui"))t2="UI";
		if(!strcmp(t0, "IL") || !strcmp(t0, "Il"))t2="SL";
		if(!strcmp(t0, "ISL") || !strcmp(t0, "Isl"))t2="SL";
		if(!strcmp(t0, "IUL") || !strcmp(t0, "Iul"))t2="UL";
		if(!strcmp(t0, "IS") || !strcmp(t0, "Is"))t2="SS";
		if(!strcmp(t0, "ISS") || !strcmp(t0, "Iss"))t2="SS";
		if(!strcmp(t0, "IUS") || !strcmp(t0, "Ius"))t2="US";
		if(!strcmp(t0, "ISB") || !strcmp(t0, "Isb"))t2="SB";
		if(!strcmp(t0, "IUB") || !strcmp(t0, "Iub"))t2="UB";
		if(!strcmp(t0, "IB") || !strcmp(t0, "Ib"))t2="B";
		if(!strcmp(t0, "IF") || !strcmp(t0, "If"))t2="F";
		if(!strcmp(t0, "ID") || !strcmp(t0, "Id"))t2="D";
		if(!strcmp(t0, "IC") || !strcmp(t0, "Ic"))t2="C";
		if(!strcmp(t0, "IW") || !strcmp(t0, "Iw"))t2="W";
		
		if(t2)
			{ BS2P_NextToken(ctx); }
		
		n0=BS2P_ParseWrapVector(ctx, n0);

		if(t2)
			BS2P_SetAstNodeAttrStr(n0, "sfx", t2);
		
		return(n0);
	}

	if(!strcmp(t0, "X,"))
		return(DTV_NULL);
	if(!strcmp(t0, "X;"))
		return(DTV_NULL);

	BS2P_RaiseError1(ctx, "ParserUnexpectedToken", t0);
	return(DTV_UNDEFINED);
}

dtVal BS2P_ParseExprPostfix(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseLitExpr(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
//	t1=BS2P_PeekToken(ctx, 1);
//	t2=BS2P_PeekToken(ctx, 2);
	
	while(t0)
	{
		if(!strcmp(t0, "X++"))
		{
			n0=BS2P_ParseWrapUnTag(ctx, "postinc", n0);
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(!strcmp(t0, "X--"))
		{
			n0=BS2P_ParseWrapUnTag(ctx, "postdec", n0);
			BS2P_NextToken(ctx);
			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		
		if(!strcmp(t0, "X["))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprList(ctx);

//			t0=BS2P_PeekToken(ctx, 0);
//			if(!strcmp(t0, "#]"))
//				{ BS2P_NextToken(ctx); }
//			else
//				{ BS2P_RaiseError1(ctx, "ParserExpectToken", "X]"); }

			BS2P_ParseExpectToken(ctx, "X]");

			n0=BS2P_ParseWrapBinTag(ctx, "loadindex", n0, n1);
			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(!strcmp(t0, "X("))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprList(ctx);

//			t0=BS2P_PeekToken(ctx, 0);
//			if(!strcmp(t0, "#)"))
//				{ BS2P_NextToken(ctx); }
//			else
//				{ BS2P_RaiseError1(ctx, "ParserExpectToken", "X)"); }

			BS2P_ParseExpectToken(ctx, "X)");

//			if(!strcmp(BS2P_GetAstNodeAttrS(n0, "tag"), "ident"))
			if(BS2P_CheckAstNodeTag(n0, "ident"))
			{
				t2=BS2P_GetAstNodeAttrS(n0, "value");
				n0=BS2P_NewAstNode(ctx, "call");
				BS2P_SetAstNodeAttrS(n0, "name", t2);
				BS2P_SetAstNodeAttr(n0, "args", n1);
			}else
			{
				n2=n0;
				n0=BS2P_NewAstNode(ctx, "call");
				BS2P_SetAstNodeAttr(n0, "func", n2);
				BS2P_SetAstNodeAttr(n0, "args", n1);
			}
//			n0=BS2P_ParseWrapBinTag(ctx, "call", n0, n1);
			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(!strcmp(t0, "X.") || !strcmp(t0, "X->"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseLitExpr(ctx);

			n0=BS2P_ParseWrapBinTag(ctx, "loadslot", n0, n1);
			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExprPrefixCast(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2, n3;
	char *t0, *t1, *t2;
	int i0, i1, i2;
	s64 li;
	int i, j, k;
	
//	n0=BS2P_ParseLitExpr(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);
//	t2=BS2P_PeekToken(ctx, 2);

	if(!strcmp(t0, "X("))
	{
		i0=BS2P_GetCurPosition(ctx);
		BS2P_NextToken(ctx);
		n1=BS2P_ParseModifierList(ctx);
		n2=BS2P_ParseTypeExpr(ctx);
		t0=BS2P_PeekToken(ctx, 0);
		t1=BS2P_PeekToken(ctx, 1);
		if(dtvTrueP(n2) && !strcmp(t0, "X)") &&
			strcmp(t1, "X,") && strcmp(t1, "X;"))
		{
			BS2P_NextToken(ctx);
			n3=BS2P_ParseExprPrefixCast(ctx);
			
			if(dtvTrueP(n2) && dtvTrueP(n3))
			{
//				n0=BS2P_ParseWrapUnTag(ctx, "prefix_cast", n3);
//				n0=BS2P_ParseWrapTagBinary(ctx, )
				n0=BS2P_ParseWrapTagBinary(ctx,
					"prefix_cast", NULL, n3, n2);
//				BS2P_SetAstNodeAttr(n0, "type", n2);
				if(dtvTrueP(n1))
					BS2P_SetAstNodeAttr(n0, "modi", n1);
				return(n0);
			}
		}

		BS2P_SetCurPosition(ctx, i0);
	}

	n0=BS2P_ParseExprPostfix(ctx);
	return(n0);
}

dtVal BS2P_ParseExprPrefix(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
//	int i0, i1, i2;
	s64 li;
	int i, j, k;
	
//	n0=BS2P_ParseLitExpr(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);
//	t2=BS2P_PeekToken(ctx, 2);

	if(!strcmp(t0, "X++"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseExprPrefix(ctx);
		n0=BS2P_ParseWrapUnTag(ctx, "preinc", n0);
		return(n0);
	}

	if(!strcmp(t0, "X--"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseExprPrefix(ctx);
		n0=BS2P_ParseWrapUnTag(ctx, "predec", n0);
		return(n0);
	}

	if(!strcmp(t0, "X+") || !strcmp(t0, "X-") ||
		!strcmp(t0, "X~") || !strcmp(t0, "X!") ||
		!strcmp(t0, "X*") || !strcmp(t0, "X&"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseExprPrefix(ctx);

		if(!strcmp(t0, "X-"))
		{
			if(dtvIsFixIntP(n0) || dtvIsFixUIntP(n0))
				{ return(dtvWrapInt(-dtvUnwrapInt(n0))); }
			if(dtvIsFixLongP(n0))
				{ return(dtvWrapLong(-dtvUnwrapLong(n0))); }
			if(dtvIsFixFloatP(n0))
				{ return(dtvWrapFloat(-dtvUnwrapFloat(n0))); }
			if(dtvIsFixDoubleP(n0))
				{ return(dtvWrapDouble(-dtvUnwrapDouble(n0))); }
		}

		if(!strcmp(t0, "X~"))
		{
			if(dtvIsFixIntP(n0) || dtvIsFixUIntP(n0))
				{ return(dtvWrapInt(~dtvUnwrapInt(n0))); }
			if(dtvIsFixLongP(n0))
				{ return(dtvWrapLong(~dtvUnwrapLong(n0))); }
		}

		n0=BS2P_ParseWrapUnary(ctx, t0+1, n0);
		return(n0);
	}

//	n0=BS2P_ParseExprPostfix(ctx);
	n0=BS2P_ParseExprPrefixCast(ctx);
	return(n0);
}

dtVal BS2P_ParseExprMulDiv(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprPrefix(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	while(t0)
	{
		if(!strcmp(t0, "X*") || !strcmp(t0, "X/") ||
			!strcmp(t0, "X%"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprPrefix(ctx);
			n0=BS2P_ParseWrapBinary(ctx, t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExprAddSub(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprMulDiv(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	while(t0)
	{
		if(!strcmp(t0, "X+") || !strcmp(t0, "X-"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprMulDiv(ctx);
			n0=BS2P_ParseWrapBinary(ctx, t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExprShlr(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprAddSub(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	while(t0)
	{
		if(!strcmp(t0, "X<<") || !strcmp(t0, "X>>") ||
			!strcmp(t0, "X>>>"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprAddSub(ctx);
			n0=BS2P_ParseWrapBinary(ctx, t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExprRelCmp(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprShlr(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	while(t0)
	{
		if(	!strcmp(t0, "X<") || !strcmp(t0, "X>") ||
			!strcmp(t0, "X<=") || !strcmp(t0, "X>="))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprShlr(ctx);
			n0=BS2P_ParseWrapTagBinary(ctx, "bincmp", t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(!strcmp(t0, "Iin"))
		{
			BS2P_NextToken(ctx);

			n1=BS2P_ParseExprShlr(ctx);
			n0=BS2P_ParseWrapTagBinary(ctx, "binary", t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(!strcmp(t0, "Iinstanceof") || !strcmp(t0, "Iis"))
		{
			BS2P_NextToken(ctx);

			n1=BS2P_ParseTypeExpr(ctx);
			n0=BS2P_ParseWrapTagBinary(ctx, "instanceof", t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(!strcmp(t0, "Ias"))
		{
			BS2P_NextToken(ctx);

			if(BS2P_ParseExpectOptToken(ctx, "X!"))
			{
				n1=BS2P_ParseTypeExpr(ctx);
				n0=BS2P_ParseWrapTagBinary(ctx,
					"cast_strict", t0+1, n0, n1);
			}else
			{
				n1=BS2P_ParseTypeExpr(ctx);
				n0=BS2P_ParseWrapTagBinary(ctx, "cast", t0+1, n0, n1);
			}

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExprEqCmp(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprRelCmp(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	while(t0)
	{
		if(!strcmp(t0, "X==") || !strcmp(t0, "X!=") ||
			!strcmp(t0, "X===") || !strcmp(t0, "X!=="))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprRelCmp(ctx);
			n0=BS2P_ParseWrapTagBinary(ctx, "bincmp", t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExprBitOp(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprEqCmp(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	while(t0)
	{
		if(!strcmp(t0, "X&") || !strcmp(t0, "X|") ||
			!strcmp(t0, "X^"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprEqCmp(ctx);
			n0=BS2P_ParseWrapBinary(ctx, t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExprLogOp(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprBitOp(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	while(t0)
	{
		if(!strcmp(t0, "X&&") || !strcmp(t0, "X||"))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprBitOp(ctx);
			n0=BS2P_ParseWrapBinary(ctx, t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExprTern(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprLogOp(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	if(!strcmp(t0, "X?"))
	{
		BS2P_NextToken(ctx);
		n1=BS2P_ParseExprTern(ctx);
		BS2P_ParseExpectToken(ctx, "X:");
		n2=BS2P_ParseExprTern(ctx);
		n0=BS2P_ParseWrapTern(ctx, n0, n1, n2);
	}
	
	return(n0);
}

dtVal BS2P_ParseExprAssignOp(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	s64 li;
	int i, j, k;
	
	n0=BS2P_ParseExprTern(ctx);
	
	t0=BS2P_PeekToken(ctx, 0);
	while(t0)
	{
		if(!strcmp(t0, "X="))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprTern(ctx);
			n0=BS2P_ParseWrapBinTag(ctx, "assign", n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}

		if(	!strcmp(t0, "X+=") || !strcmp(t0, "X-=") ||
			!strcmp(t0, "X*=") || !strcmp(t0, "X/=") ||
			!strcmp(t0, "X<<=") || !strcmp(t0, "X>>=") ||
			!strcmp(t0, "X>>>=") || !strcmp(t0, "X&=") ||
			!strcmp(t0, "X|=") || !strcmp(t0, "X^="))
		{
			BS2P_NextToken(ctx);
			n1=BS2P_ParseExprTern(ctx);
			n0=BS2P_ParseWrapTagBinary(ctx, "assignop", t0+1, n0, n1);

			t0=BS2P_PeekToken(ctx, 0);
			continue;
		}
		break;
	}
	
	return(n0);
}

dtVal BS2P_ParseExpr(BS2CC_CompileContext *ctx)
{
	return(BS2P_ParseExprAssignOp(ctx));
}

dtVal BS2P_ParseOptExpr(BS2CC_CompileContext *ctx)
{
	char *t0, *t1, *t2;

	t0=BS2P_PeekToken(ctx, 0);
	if(t0)
	{
		if(!strcmp(t0, "X,"))
			return(DTV_NULL);
		if(!strcmp(t0, "X;"))
			return(DTV_NULL);
		if(!strcmp(t0, "X)"))
			return(DTV_NULL);
		if(!strcmp(t0, "X]"))
			return(DTV_NULL);
		if(!strcmp(t0, "X}"))
			return(DTV_NULL);
	}

	return(BS2P_ParseExprAssignOp(ctx));
}

dtVal BS2P_ParseExprList(BS2CC_CompileContext *ctx)
{
	dtVal dtna[1024];
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	int ndtna;
	int i, j, k;
	
	ndtna=0;
	
	n0=BS2P_ParseExprAssignOp(ctx);
	dtna[ndtna++]=n0;

	t0=BS2P_PeekToken(ctx, 0);
	while(t0 && !strcmp(t0, "X,"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseExprAssignOp(ctx);
		dtna[ndtna++]=n0;
		t0=BS2P_PeekToken(ctx, 0);
	}
	
	if(ndtna<=0)
		return(DTV_NULL);
	if(ndtna==1)
		return(dtna[0]);
	
	n0=BS2P_NewAstArray(ctx, ndtna);
	for(i=0; i<ndtna; i++)
	{
		BS2P_SetAstArrayIdx(n0, i, dtna[i]);
	}

	return(n0);
}

dtVal BS2P_ParseNameExpr(BS2CC_CompileContext *ctx)
{
	dtVal n0, n1, n2;
	char *t0, *t1, *t2, *tn;

	t0=BS2P_PeekToken(ctx, 0);
	t1=BS2P_PeekToken(ctx, 1);
	if((*t0=='I') && !strcmp(t1, "X:"))
	{
		tn=t0+1;
		BS2P_NextToken(ctx);
		BS2P_NextToken(ctx);

		n1=BS2P_ParseExprAssignOp(ctx);
		n0=BS2P_ParseWrapUnTag(ctx, "namedexpr", n1);
		BS2P_SetAstNodeAttrStr(n0, "name", tn);
		return(n0);
	}

	n0=BS2P_ParseExprAssignOp(ctx);
	return(n0);
}

dtVal BS2P_ParseNameExprList(BS2CC_CompileContext *ctx)
{
	dtVal dtna[1024];
	dtVal n0, n1, n2;
	char *t0, *t1, *t2;
	int ndtna;
	int i, j, k;
	
	ndtna=0;
	
	n0=BS2P_ParseNameExpr(ctx);
	dtna[ndtna++]=n0;

	t0=BS2P_PeekToken(ctx, 0);
	while(t0 && !strcmp(t0, "X,"))
	{
		BS2P_NextToken(ctx);
		n0=BS2P_ParseNameExpr(ctx);
		dtna[ndtna++]=n0;
		t0=BS2P_PeekToken(ctx, 0);
	}
	
	if(ndtna<=0)
		return(DTV_NULL);
	if(ndtna==1)
		return(dtna[0]);
	
	n0=BS2P_NewAstArray(ctx, ndtna);
	for(i=0; i<ndtna; i++)
	{
		BS2P_SetAstArrayIdx(n0, i, dtna[i]);
	}

	return(n0);
}
