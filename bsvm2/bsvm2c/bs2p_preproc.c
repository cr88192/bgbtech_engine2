char *bs2pp_ibuf;
char *bs2pp_obuf;
char *bs2pp_obufe;

BS2CC_PpDef *bs2pp_sdef;	//static defines

int bs2pp_warn, bs2pp_err;

int bs2pp_iflvl;	//nested ifdef-level (true branches)
int bs2pp_iflvl2;	//nested ifdef-level (false branch)
int bs2pp_ifmark;	//set if 'true' branch already taken
int bs2pp_ifflag[256];	//ifdef level flags

char *bs2pp_ifdname[256];	//ifdef level flags
char *bs2pp_ifdname2[256];	//ifdef level flags

char *bs2pp_spos;	//temp source position
char *bs2pp_lfn;	//last file name

char *bs2pp_cfn;	//current file name
int bs2pp_cln;	//current file name


//includes
char *bs2pp_sinc[256];	//static include paths
int bs2pp_nsinc=0;	//num static include paths

char *bs2pp_inc[256];	//include paths
int bs2pp_ninc;	//num include paths

int bs2pp_noinclude_p=0;	//_noinclude set

char *bs2pp_cache_incname[4096];
byte *bs2pp_cache_incbuf[4096];
int bs2pp_cache_incsz[4096];
int bs2pp_cache_ninc=0;

BS2CC_PpDef *bs2pp_freedef=NULL;

char *bs2pp_lfnstk_fn[256];
int bs2pp_lfnstk_ln[256];
char *bs2pp_lfnstk_buf[256];
int bs2pp_lfnstk_pos;

char *BS2PP_GetFilename()
{
	int i;

	i=bs2pp_lfnstk_pos-1;
	if(i<0)i=0;
	return(bs2pp_lfnstk_fn[i]);
}

int BS2PP_CalcLinenum(char *spos)
{
	char *cs;
	int ln;
	int i;

	i=bs2pp_lfnstk_pos-1;
	if(i<0)i=0;

//	fn=bs2pp_lfnstk_fn[i];
	ln=bs2pp_lfnstk_ln[i];
	cs=bs2pp_lfnstk_buf[i];

	while(*cs && (cs<spos))
	{
		if(*cs=='\r')
		{
			cs++;	ln++;
			if(*cs=='\n')
				cs++;
			continue;
		}
		if(*cs=='\n')
			{ cs++; ln++; continue; }
		cs++;
	}
	return(ln);
}
	
void BS2PP_FlushToken(char *str)
{
}

void BS2PP_PushLinenum()
{
	int i;
	i=bs2pp_lfnstk_pos++;
	if(i>0)
	{
		bs2pp_lfnstk_fn[i]=bs2pp_lfnstk_fn[i-1];
		bs2pp_lfnstk_ln[i]=bs2pp_lfnstk_ln[i-1];
		bs2pp_lfnstk_buf[i]=bs2pp_lfnstk_buf[i-1];
	}
}

void BS2PP_SetLinenum(char *fn, char *buf, int ln)
{
	int i;
	i=bs2pp_lfnstk_pos-1;
	if(i<0)i=0;
	bs2pp_lfnstk_fn[i]=fn;
	bs2pp_lfnstk_ln[i]=ln;
	bs2pp_lfnstk_buf[i]=buf;
}

void BS2PP_PopLinenum()
{
	bs2pp_lfnstk_pos--;
	if(bs2pp_lfnstk_pos<0)
		bs2pp_lfnstk_pos=0;
}

int BS2PP_OpChar(int ch)
{
	int i;
	switch(ch)
	{
//	case '$':

	case '!':	case '@':
	case '#':	case '%':
	case '^':	case '&':
	case '*':	case '-':
	case '+':	case '=':
	case '~':	case '\\':
	case '|':	case ';':
	case ',':	case '.':
	case '<':	case '>':
	case '/':	case '?':
		i=1;
		break;
	default:
		i=0; break;
	}
	return(i);
}

int BS2PP_CheckIncludeHashName(char *str)
{
	int h;
	char *s;
	
	s=str; h=0;
	while(*s)h=(h*251)+(*s++);
	h=h*251;
	return((h>>8)&4095);
}

int BS2PP_CheckIncludeHashNext(int hash)
{
	return(((hash*251)>>8)&4095);
}

int BS2PP_CheckIncludeCache(char *str, byte **rbuf, int *rsz)
{
	int hi, n;
	
	hi=BS2PP_CheckIncludeHashName(str); n=64;
	while(bs2pp_cache_incname[hi] && ((n--)>0))
	{
		if(!strcmp(bs2pp_cache_incname[hi], str))
		{
			if(rbuf)*rbuf=bs2pp_cache_incbuf[hi];
			if(rsz)*rsz=bs2pp_cache_incsz[hi];
			return(1);
		}
		hi=BS2PP_CheckIncludeHashNext(hi);
	}

	if(rbuf)*rbuf=NULL;
	if(rsz)*rsz=0;
	return(0);
}

int BS2PP_AddIncludeCache(char *str, byte *buf, int sz)
{
	int hi, n;
	
	hi=BS2PP_CheckIncludeHashName(str); n=64;
	while(bs2pp_cache_incname[hi] && ((n--)>0))
	{
		if(!strcmp(bs2pp_cache_incname[hi], str))
		{
			bs2pp_cache_incbuf[hi]=buf;
			bs2pp_cache_incsz[hi]=sz;
			return(1);
		}
		hi=BS2PP_CheckIncludeHashNext(hi);
	}
	
	if(n>0)
	{
		bs2pp_cache_incname[hi]=bgbdt_mm_strdup(str);
		bs2pp_cache_incbuf[hi]=buf;
		bs2pp_cache_incsz[hi]=sz;
		return(1);
	}
	
	return(0);
}

int BS2PP_AddIncludePathFront(char *str)
{
	int i;

	if(!str)return(-1);

	for(i=0; i<bs2pp_nsinc; i++)
		if(!strcmp(bs2pp_sinc[i], str))
			return(i);
	i=bs2pp_nsinc++;
	bs2pp_sinc[i]=bgbdt_mm_strdup(str);
	return(i);
}

int BS2PP_AddIncludePathBack(char *str)
{
	int i;

	if(!str)return(-1);

	for(i=0; i<bs2pp_nsinc; i++)
		if(!strcmp(bs2pp_sinc[i], str))
			return(i);

	for(i=bs2pp_nsinc; i>0; i++)
		bs2pp_sinc[i]=bs2pp_sinc[i-1];
	bs2pp_nsinc++;
	bs2pp_sinc[0]=bgbdt_mm_strdup(str);
	return(i);
}


#if 1
int BS2PP_ParseChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(i>=0xF0)
	{
		i=(i&7)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xE0)
	{
		i=(i&15)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xC0)
	{
		i=(i&31)<<6;
		i+=(*s++)&63;
	}else if(i>=0x80)
	{
//		continue;
	}

	*str=(char *)s;
	return(i);
}

int BS2PP_EmitChar(char **str, int j)
{
	unsigned char *t;
	int i;

	t=(unsigned char *)(*str);

	if(j>0x3FFFFFF)
	{
		*t++=0xFC+(j>>30);
		*t++=0x80+((j>>24)&0x3F);
		*t++=0x80+((j>>18)&0x3F);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0x1FFFFF)
	{
		*t++=0xF8+(j>>24);
		*t++=0x80+((j>>18)&0x3F);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0xFFFF)
	{
		*t++=0xF0+(j>>18);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0x7FF)
	{
		*t++=0xE0+(j>>12);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if((j>0x7F) || !j)
	{
		*t++=0xC0+(j>>6);
		*t++=0x80+(j&0x3F);
	}else
	{
		if(!j) //Modified-UTF8
			{ *t++=0xC0; *t++=0x80; }
			else *t++=j;
	}

	*str=(char *)t;
	return(0);
}
#endif

char *BS2PP_EmitString(char *t, char *s)
{
	int i;

	if((*(t-1)=='\"') || (*(t-1)=='\''))
		*t++=' ';

	*t++='\"';
	while(*s)
	{
		i=BS2PP_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
				sprintf(t, "\\u%04X", i); t+=6;
				continue;
			}

			switch(i)
			{
			case '\r': *t++='\\'; *t++='r'; break;
			case '\n': *t++='\\'; *t++='n'; break;
			case '\t': *t++='\\'; *t++='t'; break;
			case '\x1B': *t++='\\'; *t++='e'; break;
			default: sprintf(t, "\\x%02X", i); t+=4; break;
			}
			continue;
		}

		if(i=='\"')*t++='\\';
		if(i=='\'')*t++='\\';
		if(i=='\\')*t++='\\';
		BS2PP_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\"';
	*t=0;

	return(t);
}

char *BS2PP_EmitCharString(char *t, char *s)
{
	int i;

	if((*(t-1)=='\"') || (*(t-1)=='\''))
		*t++=' ';

	*t++='\'';
	while(*s)
	{
		i=BS2PP_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
				sprintf(t, "\\u%04X", i); t+=6;
				continue;
			}

			switch(i)
			{
			case '\r': *t++='\\'; *t++='r'; break;
			case '\n': *t++='\\'; *t++='n'; break;
			case '\t': *t++='\\'; *t++='t'; break;
			case '\x1B': *t++='\\'; *t++='e'; break;
			default: sprintf(t, "\\x%02X", i); t+=4; break;
			}
			continue;
		}

		if(i=='\"')*t++='\\';
		if(i=='\'')*t++='\\';
		if(i=='\\')*t++='\\';
		BS2PP_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\'';
	*t=0;

	return(t);
}

void BS2PP_Trigraphs(BS2CC_CompileContext *ctx, char *str)
{
	char *s, *t;

#if 1
	//limit trigraphs to C/C++
	if((ctx->srclang!=BS2CC_SRCLANG_C) &&
		(ctx->srclang!=BS2CC_SRCLANG_CPP))
			return;

	s=str; t=str;
	while(*s)
	{
		if((s[0]=='?') && (s[1]=='?'))
		{
			if(s[2]=='=') { *t++='#'; s+=3; continue; }
			if(s[2]==')') { *t++=']'; s+=3; continue; }
			if(s[2]=='!') { *t++='|'; s+=3; continue; }
			if(s[2]=='(') { *t++='['; s+=3; continue; }
			if(s[2]=='\'') { *t++='^'; s+=3; continue; }
			if(s[2]=='>') { *t++='}'; s+=3; continue; }
			if(s[2]=='/') { *t++='\\'; s+=3; continue; }
			if(s[2]=='<') { *t++='{'; s+=3; continue; }
			if(s[2]=='-') { *t++='~'; s+=3; continue; }
		}
		*t++=*s++;
		continue;
	}
	*t++=0;
#endif
}

void BS2PP_UniEscape(BS2CC_CompileContext *ctx, char *str)
{
	char *s, *t;
	int i, j, k;

	s=str; t=str;
	while(*s)
	{
		if((s[0]=='\\') && ((s[1]=='u') || (s[1]=='U')))
		{
			k=2;
			if(s[1]=='u')k=4;
			if(s[1]=='U')k=8;

			s+=2; j=0;
			while(k--)
			{
				j<<=4;
				if((*s>='0') && (*s<='9'))j+=*s-'0';
				if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
				if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
				s++;
			}
			BS2PP_EmitChar(&t, j);
			continue;
		}
		
		*t++=*s++;
		continue;
	}
	*t++=0;
}

int BS2PP_Error(BS2CC_CompileContext *ctx, char *str, ...)
{
	char tb[1024];
	va_list lst;
	int l;
	
	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);
	
	l=BS2PP_CalcLinenum(bs2pp_spos);
	printf("pp-error: %s:%d: %s\n", bs2pp_lfn, l, tb);
	bs2pp_err++;

	return(0);
}

BS2CC_PpDef *BS2PP_AllocDefine(BS2CC_CompileContext *ctx)
{
	BS2CC_PpDef *tmp;
	
#if 1
	if(bs2pp_freedef)
	{
		tmp=bs2pp_freedef;
		bs2pp_freedef=tmp->next;
		
		memset(tmp, 0, sizeof(BS2CC_PpDef));
		return(tmp);
	}
#endif

	tmp=bgbdt_mm_malloc(sizeof(BS2CC_PpDef));
	memset(tmp, 0, sizeof(BS2CC_PpDef));
	return(tmp);
}

void BS2PP_FreeDefine(BS2CC_CompileContext *ctx, BS2CC_PpDef *def)
{
	def->next=bs2pp_freedef;
	bs2pp_freedef=def;
}

BS2CC_PpDef *BS2PP_LookupDefine(BS2CC_CompileContext *ctx, char *name)
{
	char *args[64];
	char buf[4096];
	BS2CC_PpDef *cur;
	char *s;
	int i;

	s=name; i=0; while(*s)i=(i*251)+(*s++);
	i&=255;

	cur=ctx->ppdef[i];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	cur=bs2pp_sdef;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	if(!bs2pp_noinclude_p)
		return(NULL);

#if 0
	s=BS2PP_LookupPPKey(ctx, name);
	if(s && !strcmp(s, "define"))
	{
		sprintf(buf, "%s:value", name);
		s=BS2PP_LookupPPKey(ctx, buf);
		if(!s)s="";
		BS2PP_AddDefine(ctx, name, s);

		cur=BS2PP_LookupDefine(ctx, name);
		return(cur);
	}

	if(s && !strcmp(s, "macro"))
	{
		sprintf(buf, "%s:value", name);
		s=BS2PP_LookupPPKey(ctx, buf);
		if(!s)s="";

		for(i=0; i<64; i++)
		{
			sprintf(buf, "%s:arg.%d", name, i);
			s=BS2PP_LookupPPKey(ctx, buf);
			if(!s)break;
			args[i]=bgbdt_mm_strdup(s);
		}
		args[i]=NULL;

		BS2PP_AddDefineArgs(ctx, name, args, s);

		cur=BS2PP_LookupDefine(ctx, name);
		return(cur);
	}
#endif

	return(NULL);
}

void BS2PP_DeleteDefine(BS2CC_CompileContext *ctx, char *name)
{
	BS2CC_PpDef *cur, *prv;
	char *s;
	int i;

	s=name; i=0; while(*s)i=(i*251)+(*s++);
	i&=255;

	cur=ctx->ppdef[i]; prv=NULL;
	while(cur)
	{
		if(!strcmp(cur->name, name))
		{
			if(prv)
			{
				prv->next=cur->next;
			}else
			{
				ctx->ppdef[i]=cur->next;
			}
			BS2PP_FreeDefine(ctx, cur);
			return;
		}
		prv=cur; cur=cur->next;
	}
}

void BS2PP_CleanupDefines(BS2CC_CompileContext *ctx)
{
	BS2CC_PpDef *cur, *nxt;
	int i;

	for(i=0; i<256; i++)
	{
		cur=ctx->ppdef[i];
		ctx->ppdef[i]=NULL;

		while(cur)
		{
			nxt=cur->next;
			BS2PP_FreeDefine(ctx, cur);
			cur=nxt; 
		}
	}
}

void BS2PP_AddDefine(BS2CC_CompileContext *ctx, char *name, char *str)
{
	BS2CC_PpDef *cur;
	char *s;
	int i;

	s=name; i=0;
	while(*s)i=(i*251)+(*s++);
	i&=255;

	cur=BS2PP_AllocDefine(ctx);

	cur->name=bgbdt_mm_strdup(name);
	cur->value=bgbdt_mm_strdup(str);

	cur->next=ctx->ppdef[i];
	ctx->ppdef[i]=cur;
}

void BS2PP_AddDefineArgs(BS2CC_CompileContext *ctx,
	char *name, char **args, char *str)
{
	BS2CC_PpDef *cur;
	char *s;
	int i, hi;

	s=name; hi=0; while(*s)hi=(hi*251)+(*s++);
	hi&=255;

	cur=BS2PP_AllocDefine(ctx);

	cur->name=bgbdt_mm_strdup(name);
	cur->value=bgbdt_mm_strdup(str);

	for(i=0; args[i]; i++);

	if(i>=8)
		{ cur->args=bgbdt_mm_malloc((i+1)*sizeof(char *)); }
	else
		{ cur->args=cur->t_args; }

	for(i=0; args[i]; i++)cur->args[i]=bgbdt_mm_strdup(args[i]);
	cur->args[i]=NULL;

	cur->next=ctx->ppdef[hi];
	ctx->ppdef[hi]=cur;
}


void BS2PP_AddStaticDefine(BS2CC_CompileContext *ctx,
	char *name, char *str)
{
	BS2CC_PpDef *cur;

	cur=bgbdt_mm_malloc(sizeof(BS2CC_PpDef));
	memset(cur, 0, sizeof(BS2CC_PpDef));

	cur->name=bgbdt_mm_strdup(name);
	cur->value=bgbdt_mm_strdup(str);

	cur->next=bs2pp_sdef;
	bs2pp_sdef=cur;
}

void BS2PP_AddStaticDefineArgs(BS2CC_CompileContext *ctx,
	char *name, char **args, char *str)
{
	BS2CC_PpDef *cur;
	int i;

	cur=bgbdt_mm_malloc(sizeof(BS2CC_PpDef));
	memset(cur, 0, sizeof(BS2CC_PpDef));

	cur->name=bgbdt_mm_strdup(name);
	cur->value=bgbdt_mm_strdup(str);

	for(i=0; args[i]; i++);
	if(i>=8)
		{ cur->args=bgbdt_mm_malloc((i+1)*sizeof(char *)); }
	else
		{ cur->args=cur->t_args; }

	for(i=0; args[i]; i++)
		cur->args[i]=bgbdt_mm_strdup(args[i]);
	cur->args[i]=NULL;

	cur->next=bs2pp_sdef;
	bs2pp_sdef=cur;
}

#if 0
// void BS2PP_SendDefines(BS2CC_CompileContext *ctx)
{
	char buf[4096];
	BS2CC_PpDef *cur;
	char *s;
	int i, j;

	for(i=0; i<256; i++)
	{
		cur=ctx->ppdef[i];
		while(cur)
		{
			if(cur->args)
			{
				BGBCC_BindPPKey(ctx, cur->name, "macro");
				sprintf(buf, "%s:value", cur->name);
				BGBCC_BindPPKey(ctx, buf, cur->value);

				for(j=0; cur->args[j]; j++)
				{
					sprintf(buf, "%s:arg.%d",
						cur->name, j);
					BGBCC_BindPPKey(ctx,
						buf, cur->args[j]);
				}

				sprintf(buf, "%s:arg.%d", cur->name, j);
				BGBCC_BindPPKey(ctx, buf, NULL);
			}else
			{
				BGBCC_BindPPKey(ctx, cur->name, "define");
				sprintf(buf, "%s:value", cur->name);
				BGBCC_BindPPKey(ctx, buf, cur->value);
			}

			cur=cur->next;
		}
	}
}
#endif

char *BS2PP_LoadInclude(BS2CC_CompileContext *ctx, char *name, int *rsz)
{
	char b[4096];
	char *buf;
	int i, sz;

	if(BS2PP_CheckIncludeCache(name, (byte **)(&buf), &sz)>0)
	{
		if(rsz)*rsz=sz;
		return(buf);
	}

	buf=NULL;
	for(i=(bs2pp_ninc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", bs2pp_inc[i], name);
		buf=bgbdt_mm_loadfile(b, &sz);
		if(buf)break;
	}

	if(!buf)
	{
		for(i=(bs2pp_nsinc-1); i>=0; i--)
		{
			sprintf(b, "%s/%s", bs2pp_sinc[i], name);
			buf=bgbdt_mm_loadfile(b, &sz);
			if(buf)break;
		}
	}

	if(buf)
	{
		BS2PP_AddIncludeCache(name, (byte *)buf, sz);
		if(rsz)*rsz=sz;
		return(buf);
	}

	return(NULL);
}

void BS2PP_Include(BS2CC_CompileContext *ctx, char *str)
{
	char b[4096];
	char *buf;
	char *s;
	int l0, l1, l2, l3;
	int i;

	buf=BS2PP_LoadInclude(ctx, str, NULL);

	if(!buf)
	{
		printf("BS2PP_Include: fail open include"
			" '%s'\n", str);
		bs2pp_err++;
		return;
	}

	l0=bs2pp_iflvl;
	l1=bs2pp_iflvl2;

	BS2PP_PushLinenum();
	BS2PP_SetLinenum(str, buf, 1);

	BS2PP_Buffer(ctx, buf);
//	free(buf);

	BS2PP_PopLinenum();
	
	l2=bs2pp_iflvl;
	l3=bs2pp_iflvl2;
	if((l0!=l2) || (l1!=l3))
	{
		printf("BS2PP_Include: PP Mismatched Nesting Levels"
			" '%s': %d->%d, %d->%d\n", str, l0, l2, l1, l3);

		for(i=l0-2; i<l2+2; i++)
		{
			if(i<0)continue;
			s=bs2pp_ifdname[i];
			if(!s)s="--";
			printf("    %d: %s\n", i, s);
		}
		for(i=l1-2; i<l3+2; i++)
		{
			if(i<0)continue;
			s=bs2pp_ifdname2[i];
			if(!s)s="--";
			printf("    %d: %s\n", i, s);
		}

		bs2pp_iflvl=l0;
		bs2pp_iflvl2=l1;
	}
}

char *BS2PP_TokenCtx(BS2CC_CompileContext *ctx,
	char *str, char *buf, int *rty)
{
	char tb[256];
	char *s;
	
	s=BS2P_ParseTokenBasic(str, tb);
	strcpy(buf, tb+1);
	*rty=tb[0];
	return(s);
}

void BS2PP_Directive2(BS2CC_CompileContext *ctx, char *str)
{
	char b[256], b2[4096];
	int ty, ty2;
	char *s, *t, *s1;
//	BCCX_Node *n0;
	dtVal nexp;
	int i;

	s=str;
	s=BS2P_EatWhiteOnly(s);
	s++;

	s=BS2PP_TokenCtx(ctx, s, b, &ty);
	BS2PP_TokenCtx(ctx, s, b2, &ty2);

	if(!strcmp(b, "if"))
	{
		bs2pp_ifdname2[bs2pp_iflvl2]=bgbdt_mm_strdup(str);
		bs2pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "ifdef"))
	{
		bs2pp_ifdname2[bs2pp_iflvl2]=bgbdt_mm_strdup(b2);
		bs2pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "ifndef"))
	{
		bs2pp_ifdname2[bs2pp_iflvl2]=bgbdt_mm_strdup(b2);
		bs2pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "endif"))
	{
		bs2pp_iflvl2--;
		return;
	}

	if(!strcmp(b, "else"))
	{
		if((bs2pp_iflvl2==1) && !bs2pp_ifmark)
		{
			bs2pp_ifdname[bs2pp_iflvl]=
				bs2pp_ifdname2[bs2pp_iflvl2-1];
			bs2pp_iflvl2--;
			bs2pp_iflvl++;
			return;
		}
		return;
	}

	if(!strcmp(b, "elif"))
	{
		if((bs2pp_iflvl2==1) && !bs2pp_ifmark)
		{
			strcpy(b2, s);
			i=1; while(i)i=BS2PP_Line(ctx, b2);

			s1=b2;
//			n0=BS2PP_Expression(ctx, &s1);
//			n0=BGBCC_ReduceExpr(NULL, n0);
//			i=BGBCC_BoolExpr(NULL, n0);

			nexp=BS2P_ParseBufferExpression(ctx, b2, strlen(b2));
			nexp=BS2C_ReduceExpr(ctx, nexp);
			i=BS2C_CompileGetCondBool(ctx, nexp);

			if(i>0)
			{
				bs2pp_ifdname[bs2pp_iflvl]=
					bs2pp_ifdname2[bs2pp_iflvl2-1];
				bs2pp_iflvl2--;
				bs2pp_iflvl++;
				return;
			}else if(i<0)
			{
				BS2PP_Error(ctx, "#elif pp-expr couldn't be evaluated\n");
				//error
			}

			return;
		}

		return;
	}
}

void BS2PP_Directive(BS2CC_CompileContext *ctx, char *str)
{
	char b[4096], b2[4096], b3[4096];
	char *ab[64];
	BS2CC_PpDef *def;
//	BCCX_Node *exp;
	dtVal nexp;
	char *s, *t, *s1, *s2, *tn;
	int ty, ty2;
	int i, j, n;

	if(bs2pp_iflvl2)
	{
		BS2PP_Directive2(ctx, str);
		return;
	}

	s=str;
	s=BS2P_EatWhiteOnly(s);

	s++;

	s=BS2PP_TokenCtx(ctx, s, b, &ty);
	s1=BS2PP_TokenCtx(ctx, s, b2, &ty2);

	if(!strcmp(b, "pragma"))
	{
		return;
	}

	if(!strcmp(b, "error"))
	{
		s=BS2P_EatWhiteOnly(s);
		printf("BS2PP_Directive: %s:%d: #error: %s\n",
			BS2PP_GetFilename(),
			BS2PP_CalcLinenum(bs2pp_spos), s);
		bs2pp_err++;

		return;
	}

	if(!strcmp(b, "warning"))
	{
		s=BS2P_EatWhiteOnly(s);
		printf("BS2PP_Directive: %s:%d: #warning: \"%s\"\n",
			BS2PP_GetFilename(),
			BS2PP_CalcLinenum(bs2pp_spos), s);
		bs2pp_warn++;

		return;
	}

	if(!strcmp(b, "include"))
	{
		i=1; strcpy(b, s);
		while(i)i=BS2PP_Line(ctx, b);

		s1=BS2PP_TokenCtx(ctx, b, b2, &ty2);

		if(!strcmp(b2, "<"))
		{
			t=b2;
			while(*s1 && (*s1!='>'))*t++=*s1++;
			*t++=0;

			BS2PP_Include(ctx, b2);
			return;
		}

		t=b2; t+=strlen(t);
		while(*s1)
		{
			s1=BS2PP_TokenCtx(ctx, s1, t, &ty2);
			t+=strlen(t);
		}
		*t=0;

		BS2PP_Include(ctx, b2);
		return;
	}

	if(!strcmp(b, "define"))
	{
		if(*s1!='(')
		{
			if(!strcmp(b2, "_noinclude"))
				{ bs2pp_noinclude_p=1; return; }

			while(*s1 && (*s1<=' '))s1++;
			BS2PP_AddDefine(ctx, b2, s1);
			return;
		}

		tn=bgbdt_mm_strdup(b2);

		s1++;
		n=0;
		while(1)
		{
			s1=BS2PP_TokenCtx(ctx, s1, b, &ty);
			if(!strcmp(b, ")"))break;
			if(!strcmp(b, ","))continue;

			ab[n++]=bgbdt_mm_strdup(b);
		}
		ab[n]=NULL;

		while(*s1 && (*s1<=' '))s1++;

		BS2PP_AddDefineArgs(ctx, tn, ab, s1);
		return;
	}

	if(!strcmp(b, "define_arch"))
	{
		if(*s1!='(')
		{
			while(*s1 && (*s1<=' '))s1++;
			if(*s1)
			{
				sprintf(b3, "#ARCH:%s", s1);
			}else
			{
				sprintf(b3, "#ARCH:%s", b2);
			}
			BS2PP_AddDefine(ctx, b2, b3);
			return;
		}

		return;
	}

	if(!strcmp(b, "endif"))
	{
		if(bs2pp_ifflag[bs2pp_iflvl]&1)
		{
			sprintf(bs2pp_obuf, "/*%d*/ }\n",
				BS2PP_CalcLinenum(bs2pp_spos));
		}
		bs2pp_iflvl--;
		return;
	}

	if(!strcmp(b, "else"))
	{
		if(bs2pp_ifflag[bs2pp_iflvl]&1)
		{
			sprintf(bs2pp_obuf, "/*%d*/ }else{\n",
				BS2PP_CalcLinenum(bs2pp_spos));
			return;
		}

		bs2pp_ifdname2[bs2pp_iflvl2]=
			bs2pp_ifdname[bs2pp_iflvl-1];
		bs2pp_iflvl--;
		bs2pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "elif"))
	{
		bs2pp_ifdname2[bs2pp_iflvl2]=
			bs2pp_ifdname[bs2pp_iflvl-1];
		bs2pp_iflvl--;
		bs2pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "ifdef"))
	{
		bs2pp_ifmark=0;
		bs2pp_ifflag[bs2pp_iflvl]=0;
		def=BS2PP_LookupDefine(ctx, b2);
		if(!def)
		{
			bs2pp_ifdname2[bs2pp_iflvl2]=bgbdt_mm_strdup(b2);
			bs2pp_iflvl2++;
		}else
		{
			if(def->value && !strncmp(def->value, "#ARCH:", 6))
			{
				bs2pp_ifflag[bs2pp_iflvl]|=1;
				sprintf(bs2pp_obuf, "/*%d*/ __ifdef(%s) {\n",
					BS2PP_CalcLinenum(bs2pp_spos), def->value+6);
			}
			bs2pp_ifdname[bs2pp_iflvl]=bgbdt_mm_strdup(b2);
			bs2pp_iflvl++;
		}
		if(!bs2pp_iflvl2)bs2pp_ifmark=1;
		return;
	}
	if(!strcmp(b, "ifndef"))
	{
		bs2pp_ifmark=0;
		bs2pp_ifflag[bs2pp_iflvl]=0;
		def=BS2PP_LookupDefine(ctx, b2);
		if(!def)
		{
			bs2pp_ifdname[bs2pp_iflvl]=bgbdt_mm_strdup(b2);
			bs2pp_iflvl++;
		}else
		{
			if(def->value && !strncmp(def->value, "#ARCH:", 6))
			{
				bs2pp_ifflag[bs2pp_iflvl]|=1;
				sprintf(bs2pp_obuf, "/*%d*/ __ifdef(%s) {\n",
					BS2PP_CalcLinenum(bs2pp_spos), def->value+6);
				bs2pp_ifdname[bs2pp_iflvl]=bgbdt_mm_strdup(b2);
				bs2pp_iflvl++;
			}else
			{
				bs2pp_ifdname2[bs2pp_iflvl2]=bgbdt_mm_strdup(b2);
				bs2pp_iflvl2++;
			}
		}
		if(!bs2pp_iflvl2)bs2pp_ifmark=1;
		return;
	}

	if(!strcmp(b, "undef"))
	{
		BS2PP_DeleteDefine(ctx, b2);
		return;
	}

	if(!strcmp(b, "if"))
	{
		i=1;
		strcpy(b2, s);
		while(i)i=BS2PP_Line(ctx, b2);

		s2=b2;
//		exp=BS2PP_Expression(ctx, &s2);
//		exp=BGBCC_ReduceExpr(NULL, exp);
//		i=BGBCC_BoolExpr(NULL, exp);

		nexp=BS2P_ParseBufferExpression(ctx, b2, strlen(b2));
		nexp=BS2C_ReduceExpr(ctx, nexp);
		i=BS2C_CompileGetCondBool(ctx, nexp);

		bs2pp_ifmark=0;
		if(i>0)
		{
			bs2pp_ifdname[bs2pp_iflvl]=bgbdt_mm_strdup(b2);
			bs2pp_iflvl++;
		} else {
			bs2pp_ifdname2[bs2pp_iflvl2]=bgbdt_mm_strdup(b2);
			bs2pp_iflvl2++;
		}
		if(!bs2pp_iflvl2)bs2pp_ifmark=1;

		if(i<0)
		{
			BS2PP_Error(ctx, "#if pp-expr couldn't be evaluated\n");
//			BCCX_Print(exp);
		}
		return;
	}

	printf("BS2PP_Directive: %s:%d: Unsupported preproc directive %s\n",
		BS2PP_GetFilename(),
		BS2PP_CalcLinenum(bs2pp_spos), b);
	bs2pp_err++;
}


char *BS2PP_ParseArg(BS2CC_CompileContext *ctx, char *s, char *b)
{
	char b2[4096];
	int i, ty2;

	i=0;
	while(*s)
	{
		if((*s=='\"') || (*s=='\''))
		{
			s=BS2PP_TokenCtx(ctx, s, b2, &ty2);
			b=BS2PP_PrintToken(b, b2, ty2);
			continue;
		}

		if(*s=='(') { i++; *b++=*s++; continue; }
		if(*s==')') { if(!i)break; i--; *b++=*s++; continue; }
		if(*s==',') { if(!i)break; *b++=*s++; continue; }
		*b++=*s++;
	}

	*b++=0;
	return(s);
}

char *BS2PP_PrintName(char *t, char *s)
{
	int i;

	i=*(t-1);
	if((i=='_') || ((i>='0') && (i<='9')) ||
		((i>='A') && (i<='Z')) || ((i>='a') && (i<='z')))
			*t++=' ';
	strcpy(t, s);
	t+=strlen(s);
	return(t);
}

#define BTK_NAME		'I'
#define BTK_NUMBER		'|'
#define BTK_STRING		'S'
#define BTK_CHARSTRING	'C'
#define BTK_OPERATOR	'X'

char *BS2PP_PrintToken(char *t, char *s, int ty)
{
	if(ty==BTK_NAME)
	{
		t=BS2PP_PrintName(t, s);
		return(t);
	}

	if(ty==BTK_NUMBER)
	{
		t=BS2PP_PrintName(t, s);
		return(t);
	}

	if(ty==BTK_STRING)
	{
		t=BS2PP_EmitString(t, s);
		return(t);
	}

	if(ty==BTK_CHARSTRING)
	{
		t=BS2PP_EmitCharString(t, s);
		return(t);
	}

	if(ty==BTK_OPERATOR)
	{
		if(BS2PP_OpChar(*(t-1)))
			*t++=' ';
		sprintf(t, "%s", s);
		t+=strlen(t);
		return(t);
	}

	sprintf(t, "%s", s);
	t+=strlen(t);
	return(t);
}

int BS2PP_Expand(BS2CC_CompileContext *ctx,
	char **src, char **dst, char *op, char *dste)
{
	char b[4096], b2[4096];
	char *ab[64];
	BS2CC_PpDef *def;
	char *s, *s1, *t, *te;
	int i, j, ty, ty2;

	s=*src; t=*dst; te=dste;

	if((t+16)>=te)
	{
		BS2PP_Error(ctx, "BS2PP_Expand: Buffer Overflow\n");
		return(-1);
	}

	if(!strcmp(op, "defined"))
	{
		s=BS2PP_TokenCtx(ctx, s, b, &ty);	//<name> | '('
		if(!strcmp(b, "("))
		{
			s=BS2PP_TokenCtx(ctx, s, b, &ty); //<name>
			def=BS2PP_LookupDefine(ctx, b);
			s=BS2PP_TokenCtx(ctx, s, b, &ty); //')'

			sprintf(t, "%d ", def?1:0);
			t+=strlen(t);

			*src=s; *dst=t;
			return(1);
		}

		def=BS2PP_LookupDefine(ctx, b);
		sprintf(t, "%d ", def!=NULL);
		t+=strlen(t);

		*src=s; *dst=t;
		return(1);
	}

	def=BS2PP_LookupDefine(ctx, op);
	if(!def)return(0);

	if(!def->args)
	{
		if((t+strlen(def->value)+16)>=te)
		{
			BS2PP_Error(ctx, "BS2PP_Expand: Buffer Overflow\n");
			return(-1);
		}

		t=BS2PP_PrintName(t, def->value);
		*src=s; *dst=t;
		return(1);
	}

	s=BS2P_EatWhiteOnly(s);

	if(*s!='(')return(0);

	s++; j=0;
	while(def->args[j])
	{
		while(*s && (*s<=' '))s++;

		if(*s==',')
		{
			ab[j++]=bgbdt_mm_strdup("");
			s++; continue;
		}
		if(*s==')') break;
		while(*s && (*s<=' '))s++;

		s=BS2PP_ParseArg(ctx, s, b2);
		ab[j++]=bgbdt_mm_strdup(b2);

		if(*s==',')
		{
			s++;
			while(*s && (*s<=' '))s++;
			if(*s==')')
			{
				ab[j++]=bgbdt_mm_strdup("");
				break;
			}
			continue;
		}
	}
	ab[j]=NULL;

	if(def->args[j] || (*s!=')'))
	{
		BS2PP_Error(ctx, "BS2PP_Expand: macro parse problem (%s)\n", op);
		bs2pp_err++;
		return(0);
	}
	s++;

	*src=s;

	s=def->value;
	while(*s)
	{
		s=BS2PP_TokenCtx(ctx, s, b, &ty);

		if(ty==BTK_NAME)
		{
			for(i=0; def->args[i]; i++)
				if(!strcmp(def->args[i], b))
					break;

			if(def->args[i])
			{
				if((t+strlen(ab[i])+16)>=te)
				{
					BS2PP_Error(ctx, "BS2PP_Expand: Buffer Overflow\n");
					return(-1);
				}

				t=BS2PP_PrintName(t, ab[i]);
				continue;
			}

			if((t+strlen(b)+16)>=te)
			{
				BS2PP_Error(ctx, "BS2PP_Expand: Buffer Overflow\n");
				return(-1);
			}

			t=BS2PP_PrintName(t, b);
			continue;
		}

		if((t+strlen(b)+16)>=te)
		{
			BS2PP_Error(ctx, "BS2PP_Expand: Buffer Overflow\n");
			return(-1);
		}

		if(ty==BTK_NUMBER)
		{
			t=BS2PP_PrintName(t, b);
			continue;
		}

		if(ty==BTK_STRING)
		{
			t=BS2PP_EmitString(t, b);
			continue;
		}

		if(ty==BTK_CHARSTRING)
		{
			t=BS2PP_EmitCharString(t, b);
			continue;
		}

		if(ty==BTK_OPERATOR)
		{
			if(BS2PP_OpChar(*(t-1)))
				*t++=' ';
			sprintf(t, "%s", b);
			t+=strlen(t);
			continue;
		}

		sprintf(t, "%s", b);
		t+=strlen(t);
	}

	*t=0;

	*dst=t;
	return(1);
}

int BS2PP_Line(BS2CC_CompileContext *ctx, char *str)
{
	static char lbuf[1<<20];
	char b[4096], b2[4096];
	char *s, *t, *t1, *te;
	int i, ty, ty2, ni;

	BS2PP_FlushToken(str);

	ni=0;
	s=str; t=lbuf; te=lbuf+((1<<20)-4096);
	while(*s)
	{
		s=BS2PP_TokenCtx(ctx, s, b, &ty);
		BS2PP_TokenCtx(ctx, s, b2, &ty2);

		if((ty2==BTK_OPERATOR) &&
			(!strcmp(b2, "##") || !strcmp(b, "%:%:")))
		{
			if(ty==BTK_NAME)
			{
				t1=t+1;
				i=BS2PP_Expand(ctx, &s, &t1, b, te);
				if(i>0) { strcpy(b, t+1); ni++; }
			}

			s=BS2PP_TokenCtx(ctx, s, b2, &ty2);	//'##'

			s=BS2PP_TokenCtx(ctx, s, b2, &ty2);

			if(ty2==BTK_NAME)
			{
				t1=t+1;
				i=BS2PP_Expand(ctx, &s, &t1, b2, te);
				if(i>0) { strcpy(b2, t+1); ni++; }
			}

//			printf("## operator: '%s' ## '%s' ('%s')\n", b, b2, str);

			*t=0;
			strcat(b, b2);
			
			if((t+strlen(b)+16)>=te)
			{
				BS2PP_Error(ctx, "BS2PP_Line: Buffer Overflow\n");
				return(-1);
			}
			
			if((ty==BTK_STRING)||(ty2==BTK_STRING))
			{
				t=BS2PP_EmitString(t, b);
			}else if((ty==BTK_CHARSTRING)||(ty2==BTK_CHARSTRING))
			{
				t=BS2PP_EmitCharString(t, b);
			}else
			{
				t=BS2PP_PrintName(t, b);
			}
			
//			printf("## operator: '%s'\n", b);
			
			ni++;
			continue;
		}

		if((t+strlen(b)+16)>=te)
		{
			BS2PP_Error(ctx, "BS2PP_Line: Buffer Overflow\n");
			return(-1);
		}

		if(ty==BTK_NAME)
		{
			i=BS2PP_Expand(ctx, &s, &t, b, te);
			if(i>0)
			{
				ni++;
				continue;
			}

			t=BS2PP_PrintName(t, b);
			continue;
		}

		if((ty==BTK_OPERATOR) &&
			(!strcmp(b, "#") || !strcmp(b, "%:")))
		{
			if(ty2==BTK_NAME)
			{
				s=BS2PP_TokenCtx(ctx, s, b2, &ty2);

				t1=t;
				i=BS2PP_Expand(ctx, &s, &t1, b2, te);
				if(i>0) { strcpy(b2, t); ni++; }

				t=BS2PP_PrintToken(t, b2, BTK_STRING);
				ni++;
				continue;
			}

			s=BS2PP_TokenCtx(ctx, s, b2, &ty2);
			t=BS2PP_PrintToken(t, b2, BTK_STRING);
			ni++;
			continue;
		}

		if(ty==BTK_NUMBER)
		{
			t=BS2PP_PrintName(t, b);
			continue;
		}

		if(ty==BTK_STRING)
		{
			t=BS2PP_EmitString(t, b);
			continue;
		}

		if(ty==BTK_CHARSTRING)
		{
			t=BS2PP_EmitCharString(t, b);
			continue;
		}

		if(ty==BTK_OPERATOR)
		{
			if(BS2PP_OpChar(*(t-1)))
				*t++=' ';
			sprintf(t, "%s", b);
			t+=strlen(t);
			continue;
		}

		sprintf(t, "%s", b);
		t+=strlen(t);
	}
	*t=0;


	BS2PP_FlushToken(str);

	memset(str, 0, 256);
	strcpy(str, lbuf);
	return(ni);
}

int BS2PP_LineDigraph(BS2CC_CompileContext *ctx, char *str)
{
	static char lbuf[1<<20];
	char b[4096];
	char *s, *t;
	int i, ty;

	//limit digraphs to C/C++
	if((ctx->srclang!=BS2CC_SRCLANG_C) &&
		(ctx->srclang!=BS2CC_SRCLANG_CPP))
			return(0);

	s=str; t=lbuf;
	while(*s)
	{
		s=BS2PP_TokenCtx(ctx, s, b, &ty);

//		if((ty==BTK_BRACE) || (ty==BTK_OPERATOR))
		if(ty==BTK_OPERATOR)
		{
			if(!strcmp(b, "<:"))strcpy(b, "[");
			if(!strcmp(b, ":>"))strcpy(b, "]");
			if(!strcmp(b, "<%"))strcpy(b, "{");
			if(!strcmp(b, "%>"))strcpy(b, "}");
			if(!strcmp(b, "%:"))strcpy(b, "#");
			if(!strcmp(b, "%:%:"))strcpy(b, "##");
		}

		t=BS2PP_PrintToken(t, b, ty);
	}
	*t=0;

	strcpy(str, lbuf);
	return(0);
}

int BS2PP_LinePostFilter(BS2CC_CompileContext *ctx, char *str)
{
	BS2PP_LineDigraph(ctx, str);
	return(0);
}


char *BS2PP_ParseLine(BS2CC_CompileContext *ctx, char *s, char *b)
{
	static int is=0, is2=0;
	char *t, *s1;
	int nplvl;

	t=b; nplvl=0;
	while(*s)
	{
		if(is)
		{
			if(*s=='\"')
			{
				is=0;
				*t++=*s++;
				continue;
			}

			if((s[0]=='\\') && (s[1]=='\''))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\"'))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\\'))
				{ *t++=*s++; *t++=*s++; continue; }

			*t++=*s++;
			continue;
		}

		if(is2)
		{
			if(*s=='\'')
			{
				is2=0;
				*t++=*s++;
				continue;
			}

			if((s[0]=='\\') && (s[1]=='\''))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\"'))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\\'))
				{ *t++=*s++; *t++=*s++; continue; }

			*t++=*s++;
			continue;
		}

		if(*s=='\"')
		{
			is=1;
			*t++=*s++;
			continue;
		}

		if(*s=='\'')
		{
			is2=1;
			*t++=*s++;
			continue;
		}

		if((*s=='\n') || (*s=='\r'))
		{
			if((t-b)>8192)
				break;
				
			s1=s;
			while(*s1 && (*s1<=' '))s1++;
			if(*s1=='#')
				break;

			if(nplvl>0)
			{
				*t++=*s++;
				continue;
			}
			break;
		}

		if((s[0]=='/') && (s[1]=='/'))
		{
			if(nplvl>0)
			{
				*t++=' ';
				while(*s && (*s!='\n') && (*s!='\r'))
					s++;
				continue;
			}
			break;
		}

		if((s[0]=='/') && (s[1]=='*'))
		{
			s+=2;
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
				{
					s+=2;
					break;
				}
				s++;
			}
			*t++=' ';
			continue;
		}

		if((*s=='\\') && ((s[1]=='\r') || (s[1]=='\n')))
		{
			s++;
			if(*s=='\r')s++;
			if(*s=='\n')s++;
			while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))
				s++;
			*t++=' ';
			continue;
		}
		
		if(*s=='(')nplvl++;
		if(*s==')')nplvl--;
		
		*t++=*s++;
	}
	*t++=0;
	if(*s=='\r')s++;
	if(*s=='\n')s++;

	return(s);
}

int BS2PP_BufferLine(BS2CC_CompileContext *ctx, char *b)
{
	char *s, *t, *s1;
	int i;

	t=b; while(*t && (*t<=' '))t++;
	if(!*t)
	{
		*b=0;
		return(0);
	}

	BS2PP_Trigraphs(ctx, b);
	BS2PP_UniEscape(ctx, b);

	s1=b; while(*s1 && (*s1<=' '))s1++;
	if(*s1=='#')
	{
		BS2PP_FlushToken(b);
		BS2PP_Directive(ctx, b);
		return(0);
	}

	if(bs2pp_iflvl2)
		{ return(0); }

	i=1;
	while(i)
	{
		BS2PP_FlushToken(b);
		i=BS2PP_Line(ctx, b);
	}

	BS2PP_FlushToken(b);
	BS2PP_LinePostFilter(ctx, b);

	if((bs2pp_obuf+strlen(b)+256)>=bs2pp_obufe)
	{
		BS2PP_Error(ctx, "BS2PP_BufferLine: Output Buffer Overflow\n");
		return(-1);
	}

	s1=BS2PP_GetFilename();
	if(s1!=bs2pp_lfn)
	{
		sprintf(bs2pp_obuf, "/*\"%s\"%d*/ %s\n",
			s1, BS2PP_CalcLinenum(bs2pp_spos), b);
		bs2pp_lfn=s1;
	}else
	{
		sprintf(bs2pp_obuf, "/*%d*/ %s\n",
			BS2PP_CalcLinenum(bs2pp_spos), b);
	}
	return(0);
}

int BS2PP_Buffer(BS2CC_CompileContext *ctx, char *ibuf)
{
	static char b[1<<20];
	char *s, *t, *s1;
	int i;

	s=ibuf;
	while(*s)
	{
		if((bs2pp_obuf+4096)>=bs2pp_obufe)
		{
			BS2PP_Error(ctx, "BS2PP_Buffer: Output Buffer Overflow\n");
			return(-1);
		}
	
		bs2pp_spos=s;
		
#if 1
		s1=s;
		while(*s1 && (*s1<=' '))s1++;
		if((s1[0]=='/') && (s1[1]=='/'))
		{
			s=s1; t=bs2pp_obuf;
			while(*s && (*s!='\r') && (*s!='\n')) *t++=*s++;
			if(*s=='\r')s++; if(*s=='\n')*t++=*s++;
			bs2pp_obuf=t; *t=0;
			continue;
		}

		if((s1[0]=='/') && (s1[1]=='*') && (s1[2]=='*'))
		{
			s=s1; t=bs2pp_obuf;
			*t++=*s++; *t++=*s++; *t++=*s++;
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
					{ *t++=*s++; *t++=*s++; break; }
				if(*s=='\r') { s++; continue; }
				*t++=*s++;
			}
			while(*s && ((*s==' ')||(*s=='\t'))) *t++=*s++;
			if(*s=='\r')s++; if(*s=='\n')*t++=*s++;
			bs2pp_obuf=t; *t=0;
			continue;
		}

		if((s1[0]=='/') && (s1[1]=='*'))
		{
			s=s1; t=bs2pp_obuf;
			s+=2;
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
					{ s+=2; break; }
				s++;
			}
			while(*s && (*s<=' '))
				{ s++; }
			bs2pp_obuf=t; *t=0;
			continue;
		}
#endif
		
		s=BS2PP_ParseLine(ctx, s, b);
		BS2PP_BufferLine(ctx, b);
		bs2pp_obuf+=strlen(bs2pp_obuf);
	}
	
	return(0);
}


int BS2PP_Filter(BS2CC_CompileContext *ctx,
	char *ibuf, char *obuf, int osz)
{
	char tb[4096];
	char *s, *t;
	int i;


	for(i=0; i<bs2pp_nsinc; i++)
		bs2pp_inc[i]=bs2pp_sinc[i];
	bs2pp_ninc=bs2pp_nsinc;

	s=BS2PP_GetFilename();
	if(*s)
	{
		strcpy(tb, s);
		s=tb+strlen(tb);
		while((s>tb) && (*s!='/'))s--;

		if(*s=='/')
		{
			*s=0;

			i=bs2pp_ninc++;
			bs2pp_inc[i]=bgbdt_mm_strdup(tb);
		}
	}

	bs2pp_ibuf=ibuf;
	bs2pp_obuf=obuf;
	bs2pp_obufe=obuf+osz;
	bs2pp_noinclude_p=0;

	for(i=0; i<256; i++)
		ctx->ppdef[i]=NULL;

	bs2pp_warn=0;
	bs2pp_err=0;

	bs2pp_iflvl=0;
	bs2pp_iflvl2=0;

	BS2PP_FlushToken(ibuf);
	BS2PP_Buffer(ctx, ibuf);

//	BS2PP_SendDefines(ctx);
	BS2PP_FlushToken(obuf);
	
	BS2PP_CleanupDefines(ctx);
	
	if(bs2pp_err>0)
		return(-1);
	return(0);
}
