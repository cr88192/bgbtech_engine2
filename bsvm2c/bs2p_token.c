#include <bteifgl.h>

char *BS2P_EatWhiteOnly(char *s)
{
	while(*s && (*s<=' '))
		s++;
	return(s);
}

char *BS2P_EatWhite(char *s)
{
	int i;
	while(*s && (*s<=' '))
		s++;
	if((s[0]=='/') && (s[1]=='/'))
	{
		s+=2;
		while(*s && (*s!='\r') && (*s!='\n'))
			s++;
		s=BS2P_EatWhite(s);
		return(s);
	}

	if((s[0]=='/') && (s[1]=='*'))
	{
		s+=2; i=1;
		while(*s && (i>0))
		{
			if((s[0]=='/') && (s[1]=='*'))
				{ i++; s+=2; continue; }
			if((s[0]=='*') && (s[1]=='/'))
				{ i--; s+=2; continue; }
			s++;
		}
		s=BS2P_EatWhite(s);
		return(s);
	}
	return(s);
}

int BS2P_ParseHexN(char *s, int n)
{
	int i, j, k;
	
	k=0;
	for(i=0; i<n; i++)
	{
		j=0;
		if((*s>='0') && (*s<='9'))
			j=*s-'0';
		if((*s>='A') && (*s<='F'))
			j=10+(*s-'A');
		if((*s>='a') && (*s<='f'))
			j=10+(*s-'a');
		s++;
		k=(k<<4)|j;
	}
	return(k);
}

char *BS2P_ReadUtf8(char *sb, int *rv)
{
	byte *s;
	int v;
	
	s=(byte *)sb;
	v=*s++;
	if(v<0x80)
		{ *rv=v; return((char *)s); }
	if(v<0xC0)
		{ *rv=-(v&63); return((char *)s); }
	if(v<0xE0)
	{
		v=((v&0x1F)<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	if(v<0xF0)
	{
		v=((v&0x0F)<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	if(v<0xF8)
	{
		v=((v&0x07)<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	if(v<0xFC)
	{
		v=((v&0x03)<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	if(v<0xFE)
	{
		v=((v&0x01)<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	
	return(NULL);
}

char *BS2P_EmitUtf8(char *t, int v)
{
	if(v<0x80)
	{
		*t++=v;
		return(t);
	}
	if(v<0x800)
	{
		*t++=0xC0|(v>>6);
		*t++=0x80|(v&0x3F);
		return(t);
	}
	if(v<0x10000)
	{
		*t++=0xE0|(v>>12);
		*t++=0x80|((v>>6)&0x3F);
		*t++=0x80|(v&0x3F);
		return(t);
	}
	if(v<0x200000)
	{
		*t++=0xF0|(v>>18);
		*t++=0x80|((v>>12)&0x3F);
		*t++=0x80|((v>>6)&0x3F);
		*t++=0x80|(v&0x3F);
		return(t);
	}
	
	return(NULL);
}

char *BS2P_ParseTokenBasic(char *s, char *tb)
{
	char *t, *te;
	int i, j, k;

	t=tb; te=tb+252;

	s=BS2P_EatWhite(s);
	if(!(*s))
	{
		*t++=0; *t++=0;
		return(s);
	}
	
	if((*s=='_') || (*s=='$') || ((*s>='A') && (*s<='Z')) ||
		((*s>='a') && (*s<='z')))
	{
		*t++='I';
		while(*s && (t<te) &&
			((*s=='_') || (*s=='$') ||
			((*s>='A') && (*s<='Z')) ||
			((*s>='a') && (*s<='z')) ||
			((*s>='0') && (*s<='9'))))
				*t++=*s++;
		*t++=0;
		return(s);
	}
	
	if((*s>='0') && (*s<='9'))
	{
		*t++='#';

		*t++=*s++;
//		if(*s=='x')
		if((*s=='x') || (*s=='b') || (*s=='c') || (*s=='d'))
		{
			*tb='|';
			
			*t++=*s++;

			while(*s && (t<te) &&
				((*s=='_') ||
				((*s>='0') && (*s<='9')) ||
				((*s>='A') && (*s<='F')) ||
				((*s>='a') && (*s<='f'))))
					*t++=*s++;
			*t++=0;
			return(s);
		}

		while(*s && (t<te) && ((*s=='_') || ((*s>='0') && (*s<='9'))))
			*t++=*s++;
		if(*s=='.')
		{
			*t++=*s++;
			while(*s && (t<te) && ((*s=='_') || ((*s>='0') && (*s<='9'))))
				*t++=*s++;
			if((*s=='e') || (*s=='E'))
			{
				*t++=*s++;
				if((*s=='+') || (*s=='-'))
					*t++=*s++;
				while(*s && (t<te) &&
					((*s=='_') || ((*s>='0') && (*s<='9'))))
						*t++=*s++;
			}
		}else
		{
			*tb='|';
		}
		*t++=0;
		return(s);
	}
	
	if(*s=='\"')
	{
		if((s[1]=='\"') && (s[2]=='\"'))
		{
			*t++='X';
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=0;
			return(s);
		}
	
		s++;
		*t++='S';
		while(*s && (t<te))
		{
			if(*s=='\\')
			{
				s++;
				switch(*s)
				{
				case 'n': s++; *t++='\n'; break;
				case 'r': s++; *t++='\r'; break;
				case 'b': s++; *t++='\b'; break;
//				case 'e': s++; *t++='\e'; break;
				case 't': s++; *t++='\t'; break;
				case '\\': s++; *t++='\\'; break;
				case '\"': s++; *t++='\"'; break;
				case '\'': s++; *t++='\''; break;
				case 'x':
					s++;
					*t++=BS2P_ParseHexN(s, 2);
					s+=2;
					break;
				case 'u':
					s++;
					i=BS2P_ParseHexN(s, 4);
					t=BS2P_EmitUtf8(t, i);
					s+=4;
					break;
				}
				continue;
			}
			if(*s=='\"')
				break;
			*t++=*s++;
		}
		if(*s=='\"')
			s++;
		*t++=0;
		return(s);
	}

	if(*s=='\'')
	{
		s++;
		*t++='C';
		while(*s && (t<te))
		{
			if(*s=='\\')
			{
				s++;
				switch(*s)
				{
				case 'n': s++; *t++='\n'; break;
				case 'r': s++; *t++='\r'; break;
				case 'b': s++; *t++='\b'; break;
//				case 'e': s++; *t++='\e'; break;
				case 't': s++; *t++='\t'; break;
				case '\\': s++; *t++='\\'; break;
				case '\"': s++; *t++='\"'; break;
				case '\'': s++; *t++='\''; break;
				case 'x':
					s++;
					*t++=BS2P_ParseHexN(s, 2);
					s+=2;
					break;
				case 'u':
					s++;
					i=BS2P_ParseHexN(s, 4);
					t=BS2P_EmitUtf8(t, i);
					s+=4;
					break;
				}
				continue;
			}
			if(*s=='\'')
				break;
			*t++=*s++;
		}
		if(*s=='\'')
			s++;
		*t++=0;
		return(s);
	}
	
	*t++='X';
	switch(*s)
	{
	case '-':
		*t++=*s++;
		if((*s=='-') || (*s=='=') || (*s=='>'))
			{ *t++=*s++; }
		*t++=0;
		break;
	case '+':
		*t++=*s++;
		if((*s=='+') || (*s=='='))
			{ *t++=*s++; }
		*t++=0;
		break;

	case '=':	case '!':
	case '/':	case '*':
	case '%':	case '^':
		*t++=*s++;
		if(*s=='=')
			*t++=*s++;
		*t++=0;
		break;

	case '|':
		*t++=*s++;
		switch(*s)
		{
		case '=': *t++=*s++; break;
		case '|':
			*t++=*s++;
			switch(*s)
			{
			case '=': *t++=*s++; break;
			default: break;
			}
			break;
		default: break;
		}
		*t++=0;
		break;

	case '&':
		*t++=*s++;
		switch(*s)
		{
		case '=': *t++=*s++; break;
		case '&':
			*t++=*s++;
			switch(*s)
			{
			case '=': *t++=*s++; break;
			default: break;
			}
			break;
		default: break;
		}
		*t++=0;
		break;

	case '<':
		*t++=*s++;
//		if((*s=='=') || (*s=='<'))
//			{ *t++=*s++; }
		switch(*s)
		{
		case '=': *t++=*s++; break;
		case '<':
			*t++=*s++;
			switch(*s)
			{
			case '<':
			case '=': *t++=*s++; break;
			default: break;
			}
			break;
		default: break;
		}
		*t++=0;
		break;
	case '>':
		*t++=*s++;
		switch(*s)
		{
		case '=': *t++=*s++; break;
		case '>':
			*t++=*s++;
			switch(*s)
			{
			case '>':
			case '=': *t++=*s++; break;
			default: break;
			}
//			if(*s=='>')
//				*t++=*s++;
			break;
		default: break;
		}
//		if((*s=='=') || (*s=='>'))
//			{ *t++=*s++; }
		*t++=0;
		break;

	case '#':
		*t++=*s++;
		switch(*s)
		{
		case '#': *t++=*s++; break;
		case ':': *t++=*s++; break;
		case '(': *t++=*s++; break;
		case ')': *t++=*s++; break;
		case '[': *t++=*s++; break;
		case ']': *t++=*s++; break;
		case '{': *t++=*s++; break;
		case '}': *t++=*s++; break;
		default: break;
		}
		*t++=0;
		break;
	
	case '~': case '?':
	case '@':
	case '\\':
	case ';': case ':':
	case ',':
		*t++=*s++;
		*t++=0;
		break;
	
	case '.':
		if((s[1]=='.') && (s[2]=='.'))
		{
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=0;
			break;
		}
		*t++=*s++;
		*t++=0;
		break;

	case '(': case ')':
	case '[': case ']':
	case '{': case '}':
		*t++=*s++;
		*t++=0;
		break;

	default:
		*t++=*s++;
		*t++=0;
		break;
	}
	return(s);
}

int BS2P_LexBuffer(BS2CC_CompileContext *ctx, char *ibuf, int szbuf)
{
	char tb[256];
	char *cs, *cs0, *cse, *ct;
	char *lexbuf, *elexbuf;
	int *srcofs;
	int *lexarr;
	int ntok, mtok;
	int i, j, k;
	
	lexbuf=malloc(262144);
	elexbuf=lexbuf+262144;

	lexarr=malloc(4096*sizeof(int));
	srcofs=malloc(4096*sizeof(int));
	ntok=0; mtok=4096;

	ct=lexbuf;
	*ct++=0;
	*ct++=0;
	
	cs=ibuf; cse=ibuf+szbuf;
	while(*cs && (cs<cse))
	{
		cs0=cs;
		cs=BS2P_ParseTokenBasic(cs, tb);
		
		if((ct+strlen(tb)+64)>=elexbuf)
		{
			i=ct-lexbuf;
			j=elexbuf-lexbuf;
			j=j+(j>>1);
			lexbuf=realloc(lexbuf, j);
			elexbuf=lexbuf+j;
			ct=lexbuf+i;
		}
		
		if((ntok+3)>=mtok)
		{
			mtok=mtok+(mtok>>1);
			lexarr=realloc(lexarr, mtok*sizeof(int));
			srcofs=realloc(srcofs, mtok*sizeof(int));
		}
		
		i=ntok++;
		srcofs[i]=cs0-ibuf;
		lexarr[i]=ct-lexbuf;
		strcpy(ct, tb);
		ct+=strlen(tb)+1;

#if 1
		if(!strcmp(tb, "X\"\"\""))
		{
			i=ntok++;
			srcofs[i]=cs-ibuf;
			lexarr[i]=ct-lexbuf;
			*ct++=0;

			while(*cs && (cs<cse))
			{
				if((cs[0]=='\"') && (cs[1]=='\"') && (cs[2]=='\"'))
				{
					cs+=3;
					break;
				}
				cs++;
			}
			continue;
		}
#endif
	}

	i=ntok;
	srcofs[i]=0;
	lexarr[i]=0;

	ctx->srcbuf=strdup(ibuf);
	ctx->srcofs=srcofs;

	ctx->tokbuf=lexbuf;
	ctx->tokarr=lexarr;
	ctx->ntok=ntok;
	ctx->tok=0;

	return(0);
}

char *BS2P_PeekToken(BS2CC_CompileContext *ctx, int idx)
{
	return(ctx->tokbuf+ctx->tokarr[ctx->tok+idx]);
}

void BS2P_NextTokenN(BS2CC_CompileContext *ctx, int cnt)
{
	ctx->tok+=cnt;
}

void BS2P_NextToken(BS2CC_CompileContext *ctx)
{
	ctx->tok++;
}

char *BS2P_GetCurSrcPosition(BS2CC_CompileContext *ctx, int idx)
{
	return(ctx->srcbuf+ctx->srcofs[ctx->tok+idx]);
}

void BS2P_SetCurSrcPosition(BS2CC_CompileContext *ctx, char *pos)
{
	int i;

	if(pos>(ctx->srcbuf+ctx->srcofs[ctx->tok]))
		{ i=ctx->tok; }
	else
		{ i=0; }

	while(pos>(ctx->srcbuf+ctx->srcofs[i]))
		i++;
	ctx->tok=i;

//	return(ctx->srcbuf+ctx->srcofs[ctx->tok+idx]);
}


int BS2P_GetCurPosition(BS2CC_CompileContext *ctx)
{
	return(ctx->tok);
}

void BS2P_SetCurPosition(BS2CC_CompileContext *ctx, int tok)
{
	ctx->tok=tok;
}

int BS2P_GetCurSourceLine(BS2CC_CompileContext *ctx,
	char **rlfn, int *rlln)
{
	char *cs1, *cs2;
	char *fn;
	int ln;
	
	cs2=BS2P_GetCurSrcPosition(ctx, 0);
	cs1=ctx->srcbuf;
	
	fn="unknown";
	ln=1;
	while(cs1<cs2)
	{
		while(*cs1 && *cs1!='\n')
			cs1++;
		if(*cs1=='\n')
		{
			cs1++;
			ln++;
			continue;
		}
//		break;
		return(-1);
	}
	
	*rlfn=fn;
	*rlln=ln;
	return(0);
}
