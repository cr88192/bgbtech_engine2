/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bgbasm.h>

struct basm_ppdef_s {
struct basm_ppdef_s *next;
char *name;
char *value;
char **args;
};

char *basm_pp_ibuf;
char *basm_pp_obuf;

struct basm_ppdef_s *basm_pp_sdef;	//static defines
struct basm_ppdef_s *basm_pp_def;	//normal defines

struct basm_ppdef_s *basm_pp_ildef[256];	//include-local defs

int basm_pp_warn, basm_pp_err;

int basm_pp_ilvl;	//include level

int basm_pp_iflvl;	//nested ifdef-level (true branches)
int basm_pp_iflvl2;	//nested ifdef-level (false branch)

char *basm_pp_spos;	//temp source position
char *basm_pp_lfn;	//last file name

char *basm_pp_inmacro;

//includes
char *basm_pp_sinc[16];	//static include paths
int basm_pp_nsinc=0;	//num static include paths

char *basm_pp_inc[16];	//include paths
int basm_pp_ninc;	//num include paths

char *basm_pp_headn[1024];	//virtual header names
char *basm_pp_headv[1024];	//virtual header values
int basm_pp_nhead;		//num virtual headers


void *basm_loadfile(char *name, int *rsz);


int BASM_PP_AddIncludePathFront(char *str)
{
	int i;

	i=basm_pp_nsinc++;
	basm_pp_sinc[i]=basm_strdup(str);
	return(i);
}

int BASM_PP_AddIncludePathBack(char *str)
{
	int i;

	for(i=basm_pp_nsinc; i>0; i++)
		basm_pp_sinc[i]=basm_pp_sinc[i-1];
	basm_pp_nsinc++;
	basm_pp_sinc[0]=basm_strdup(str);
	return(i);
}

int BASM_PP_AddVirtualHeader(char *name, char *buf)
{
	int i;

	for(i=0; i<basm_pp_nhead; i++)
		if(!strcmp(basm_pp_headn[i], name))
	{
		free(basm_pp_headv[i]);
		basm_pp_headv[i]=strdup(buf);
		return(i);
	}

	i=basm_pp_nhead++;
	basm_pp_headn[i]=basm_strdup(name);
	basm_pp_headv[i]=strdup(buf);
	return(i);
}

char *BASM_PP_LookupVirtualHeader(char *name)
{
	static int hash[256];
	static int init=0;
	char *s;
	int i, hi;

	if(!init)
	{
		memset(hash, 0, 256*sizeof(int));
		init=1;
	}

	hi=0; s=name;
	while(*s)hi=hi*4093+(*s++);
	hi=(hi>>12)&255;

	i=hash[hi];
	if(basm_pp_headn[i] && !strcmp(basm_pp_headn[i], name))
		return(basm_pp_headv[i]);

	for(i=0; i<basm_pp_nhead; i++)
		if(!strcmp(basm_pp_headn[i], name))
	{
		hash[hi]=i;
		return(basm_pp_headv[i]);
	}

	return(NULL);
}

char *BASM_PP_EmitString(char *t, char *s)
{
	int i;

	*t++='\"';
	while(*s)
	{
		i=BASM_Parse_ParseChar(&s);

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
		BASM_Parse_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\"';
	*t=0;

	return(t);
}

char *BASM_PP_EmitCharString(char *t, char *s)
{
	int i;

	*t++='\'';
	while(*s)
	{
		i=BASM_Parse_ParseChar(&s);

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
		BASM_Parse_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\'';
	*t=0;

	return(t);
}

struct basm_ppdef_s *BASM_PP_LookupDefineLvls(char *name);

struct basm_ppdef_s *BASM_PP_LookupDefine(char *name)
{
	struct basm_ppdef_s *cur;

	cur=BASM_PP_LookupDefineLvls(name);
	if(cur)return(cur);

	cur=basm_pp_def;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	cur=basm_pp_sdef;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	return(NULL);
}

struct basm_ppdef_s *BASM_PP_LookupDefine2(char *name)
{
	struct basm_ppdef_s *cur;

	cur=basm_pp_def;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	return(NULL);
}

struct basm_ppdef_s *BASM_PP_LookupDefineLvl(char *name, int lvl)
{
	struct basm_ppdef_s *cur;

	cur=basm_pp_ildef[lvl];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	return(NULL);
}

struct basm_ppdef_s *BASM_PP_LookupDefineLvls(char *name)
{
	struct basm_ppdef_s *cur;
	int i;

	for(i=basm_pp_ilvl; i>=0; i--)
	{
		cur=BASM_PP_LookupDefineLvl(name, i);
		if(cur)return(cur);
	}

	return(NULL);
}

struct basm_ppdef_s *BASM_PP_LookupDefine3(char *name)
{
	struct basm_ppdef_s *cur;

	cur=BASM_PP_LookupDefineLvls(name);
	if(cur)return(cur);
	cur=BASM_PP_LookupDefine2(name);
	return(cur);
}

void BASM_PP_DeleteDefine(char *name)
{
	struct basm_ppdef_s *cur, *prv;

	cur=basm_pp_def; prv=NULL;
	while(cur)
	{
		if(!strcmp(cur->name, name))
		{
			if(prv)
			{
				prv->next=cur->next;
			}else
			{
				basm_pp_def=cur->next;
			}
			if(cur->args)free(cur->args);
			free(cur->value);
			free(cur);
			return;
		}
		prv=cur; cur=cur->next;
	}
}

void BASM_PP_CleanupDefines()
{
	struct basm_ppdef_s *cur, *nxt;

	cur=basm_pp_def;
	basm_pp_def=NULL;

	while(cur)
	{
		nxt=cur->next;
		if(cur->args)free(cur->args);
		free(cur->value);
		free(cur);
		cur=nxt; 
	}
}

void BASM_PP_CleanupLocalDefines()
{
	struct basm_ppdef_s *cur, *nxt;

	cur=basm_pp_ildef[basm_pp_ilvl];
	basm_pp_ildef[basm_pp_ilvl]=NULL;

	while(cur)
	{
		nxt=cur->next;
		if(cur->args)free(cur->args);
		free(cur->value);
		free(cur);
		cur=nxt; 
	}
}

void BASM_PP_AddDefine(char *name, char *str)
{
	struct basm_ppdef_s *cur;

//	cur=BASM_PP_LookupDefine2(name);
	cur=BASM_PP_LookupDefine3(name);
	if(cur && !cur->args)
	{
		free(cur->value);
		cur->value=strdup(str);
		return;
	}

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	cur->next=basm_pp_def;
	basm_pp_def=cur;
}

void BASM_PP_AddDefineArgs(char *name, char **args, char *str)
{
	struct basm_ppdef_s *cur;
	int i;

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	for(i=0; args[i]; i++);
	cur->args=malloc((i+1)*sizeof(char *));
	for(i=0; args[i]; i++)cur->args[i]=args[i];
	cur->args[i]=NULL;

	cur->next=basm_pp_def;
	basm_pp_def=cur;
}

void BASM_PP_AppendDefineLine(char *name, char *str)
{
	char tb[1024];
	struct basm_ppdef_s *cur;

	cur=BASM_PP_LookupDefine2(name);
	if(!cur)return;

	if(!(*cur->value))
	{
		free(cur->value);
		cur->value=strdup(str);
		return;
	}

	sprintf(tb, "%s\n%s", cur->value, str);
	free(cur->value);
	cur->value=strdup(tb);
	return;
}


void BASM_PP_AddStaticDefine(char *name, char *str)
{
	struct basm_ppdef_s *cur;

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	cur->next=basm_pp_sdef;
	basm_pp_sdef=cur;
}

void BASM_PP_AddStaticDefineArgs(char *name, char **args, char *str)
{
	struct basm_ppdef_s *cur;
	int i;

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	for(i=0; args[i]; i++);
	cur->args=malloc((i+1)*sizeof(char *));
	for(i=0; args[i]; i++)
		cur->args[i]=args[i];
	cur->args[i]=NULL;

	cur->next=basm_pp_sdef;
	basm_pp_sdef=cur;
}

void BASM_PP_AddLocalDefine(char *name, char *str)
{
	struct basm_ppdef_s *cur;

	cur=BASM_PP_LookupDefineLvl(name, basm_pp_ilvl);
	if(cur && !cur->args)
	{
		free(cur->value);
		cur->value=strdup(str);
		return;
	}

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	cur->next=basm_pp_ildef[basm_pp_ilvl];
	basm_pp_ildef[basm_pp_ilvl]=cur;
}

void BASM_PP_AddLocalDefineArgs(char *name, char **args, char *str)
{
	struct basm_ppdef_s *cur;
	int i;

	cur=malloc(sizeof(struct basm_ppdef_s));
	memset(cur, 0, sizeof(struct basm_ppdef_s));

	cur->name=basm_strdup(name);
	cur->value=strdup(str);

	for(i=0; args[i]; i++);
	cur->args=malloc((i+1)*sizeof(char *));
	for(i=0; args[i]; i++)cur->args[i]=args[i];
	cur->args[i]=NULL;

	cur->next=basm_pp_ildef[basm_pp_ilvl];
	basm_pp_ildef[basm_pp_ilvl]=cur;
}

char *BASM_PP_LoadInclude(char *name, int *rsz)
{
	char b[256];
	char *buf;
	int i, sz;

	buf=BASM_PP_LookupVirtualHeader(name);
	if(buf)
	{
		sz=strlen(buf);
		buf=strdup(buf);
		if(rsz)*rsz=sz;
		return(buf);
	}

	buf=NULL;
	for(i=(basm_pp_ninc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", basm_pp_inc[i], name);
		buf=basm_loadfile(b, &sz);
		if(buf) { if(rsz)*rsz=sz; return(buf); }
	}

	for(i=(basm_pp_nsinc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", basm_pp_sinc[i], name);
		buf=basm_loadfile(b, &sz);
		if(buf) { if(rsz)*rsz=sz; return(buf); }
	}

	return(NULL);
}

void BASM_PP_Include(char *str)
{
	char b[256];
	char *buf;
	int i;

	buf=BASM_PP_LookupVirtualHeader(str);
	if(buf)
	{
//		BASM_PP_PushIncludeLevel();
		BASM_PP_Buffer(buf);
//		BASM_PP_PopIncludeLevel();
		return;
	}

	buf=NULL;
	for(i=(basm_pp_ninc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", basm_pp_inc[i], str);
		buf=basm_loadfile(b, NULL);
		if(buf)break;
	}

	if(!buf)
	{
		for(i=(basm_pp_nsinc-1); i>=0; i--)
		{
			sprintf(b, "%s/%s", basm_pp_sinc[i], str);
			buf=basm_loadfile(b, NULL);
			if(buf)break;
		}
	}

	if(!buf)
	{
		printf("BASM_PP_Directive: fail open include"
			" '%s'\n", str);
		basm_pp_err++;
		return;
	}

//	BASM_PP_PushIncludeLevel();
	BASM_PP_Buffer(buf);
//	BASM_PP_PopIncludeLevel();
	free(buf);
}

void BASM_PP_PushIncludeLevel()
{
	basm_pp_ilvl++;
	basm_pp_ildef[basm_pp_ilvl]=NULL;
}

void BASM_PP_PopIncludeLevel()
{
	BASM_PP_CleanupLocalDefines();
	basm_pp_ilvl--;
}


void BASM_PP_Directive2(char *str)
{
	char b[256], b2[256];
	struct basm_ppdef_s *def;
	int ty, ty2;
	char *s, *t, *s1;
	int i;

	s=str;
	s++;

	s=BASM_Parse_Token(s, b, &ty);
	BASM_Parse_Token(s, b2, &ty2);

	if(!strcmp(b, "if"))		{ basm_pp_iflvl2++; return; }
	if(!strcmp(b, "ifdef"))		{ basm_pp_iflvl2++; return; }
	if(!strcmp(b, "ifndef"))	{ basm_pp_iflvl2++; return; }
	if(!strcmp(b, "endif"))		{ basm_pp_iflvl2--; return; }

	if(!strcmp(b, "else"))
	{
		if(basm_pp_iflvl2==1)
		{
			basm_pp_iflvl2--;
			basm_pp_iflvl++;
			return;
		}
		return;
	}

	if(!strcmp(b, "elif"))
	{
		if(basm_pp_iflvl2==1)
		{
			strcpy(b2, s);
			i=1; while(i)i=BASM_PP_Line(b2);

			s1=b2;
//			n0=BASM_Parse_Expression(&s1);
//			n0=BGBCC_ReduceExpr(NULL, n0);
//			i=BGBCC_BoolExpr(NULL, n0);

			i=0;

			if(i>0)
			{
				basm_pp_iflvl2--;
				basm_pp_iflvl++;
				return;
			}

			return;
		}

		return;
	}
}

void BASM_PP_Directive(char *str)
{
	char b[256], b2[256];
	char *ab[64];
	struct basm_ppdef_s *def;
	char *s, *t, *s1, *s2, *tn;
	int ty, ty2;
	int i, j, n;

	if(basm_pp_iflvl2)
	{
		BASM_PP_Directive2(str);
		return;
	}

	s=str;
	s++;

	s=BASM_Parse_Token(s, b, &ty);
	s1=BASM_Parse_Token(s, b2, &ty2);

	if(!strcmp(b, "include"))
	{
		i=1; strcpy(b, s);
		while(i)i=BASM_PP_Line(b);

		s1=BASM_Parse_Token(b, b2, &ty2);

		if(!strcmp(b2, "<"))
		{
			t=b2;
			while(*s1 && (*s1!='>'))*t++=*s1++;
			*t++=0;

			BASM_PP_Include(b2);
			return;
		}

		t=b2; t+=strlen(t);
		while(*s1)
		{
			s1=BASM_Parse_Token(s1, t, &ty2);
			t+=strlen(t);
		}
		*t=0;

		BASM_PP_Include(b2);
		return;
	}

	if(!strcmp(b, "define"))
	{
		if(*s1!='(')
		{
			while(*s1 && (*s1<=' '))s1++;
			BASM_PP_AddDefine(b2, s1);
			return;
		}

		tn=basm_strdup(b2);

		s1++;
		n=0;
		while(1)
		{
			s1=BASM_Parse_Token(s1, b, &ty);
			if(!strcmp(b, ")"))break;
			if(!strcmp(b, ","))continue;

			ab[n++]=basm_strdup(b);
		}
		ab[n]=NULL;

		while(*s1 && (*s1<=' '))s1++;

		BASM_PP_AddDefineArgs(tn, ab, s1);
		return;
	}

	if(!strcmp(b, "definelocal"))
	{
		if(*s1!='(')
		{
			while(*s1 && (*s1<=' '))s1++;
			BASM_PP_AddLocalDefine(b2, s1);
			return;
		}

		tn=basm_strdup(b2);

		s1++;
		n=0;
		while(1)
		{
			s1=BASM_Parse_Token(s1, b, &ty);
			if(!strcmp(b, ")"))break;
			if(!strcmp(b, ","))continue;

			ab[n++]=basm_strdup(b);
		}
		ab[n]=NULL;

		while(*s1 && (*s1<=' '))s1++;

		BASM_PP_AddLocalDefineArgs(tn, ab, s1);
		return;
	}

	if(!strcmp(b, "endif"))
	{
		basm_pp_iflvl--;
		return;
	}

	if(!strcmp(b, "else"))
	{
		basm_pp_iflvl--;
		basm_pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "elif"))
	{
		basm_pp_iflvl--;
		basm_pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "ifdef"))
	{
		def=BASM_PP_LookupDefine(b2);
		if(!def)basm_pp_iflvl2++;
			else basm_pp_iflvl++;
		return;
	}
	if(!strcmp(b, "ifndef"))
	{
		def=BASM_PP_LookupDefine(b2);
		if(!def)basm_pp_iflvl++;
			else basm_pp_iflvl2++;
		return;
	}

	if(!strcmp(b, "undef"))
	{
		BASM_PP_DeleteDefine(b2);
		return;
	}

	if(!strcmp(b, "if"))
	{
		i=1;
		strcpy(b2, s);
		while(i)i=BASM_PP_Line(b2);

//		s2=b2;
//		n0=BASM_Parse_Expression(&s2);
//		n0=BGBCC_ReduceExpr(NULL, n0);
//		i=BGBCC_BoolExpr(NULL, n0);
//		i=0;

		while(i)i=BASM_PP_LineArith(b2);

		s2=b2; while(*s2 && *s2<=' ')s2++;
		i=atoi(s2);

		if(i>0)basm_pp_iflvl++;
			else basm_pp_iflvl2++;

		return;
	}

	if(!strcmp(b, "assign"))
	{
		strcpy(b2, s1);
		i=1; while(i)i=BASM_PP_Line(b2);
		i=1; while(i)i=BASM_PP_LineArith(b2);
		s2=b2; while(*s2 && (*s2<=' '))s2++;
		BASM_PP_AddDefine(b2, s2);
		return;
	}

	if(!strcmp(b, "assignlocal"))
	{
		strcpy(b2, s1);
		i=1; while(i)i=BASM_PP_Line(b2);
		i=1; while(i)i=BASM_PP_LineArith(b2);
		s2=b2; while(*s2 && (*s2<=' '))s2++;
		BASM_PP_AddLocalDefine(b2, s2);
		return;
	}

	if(!strcmp(b, "macro"))
	{
		tn=basm_strdup(b2);

		n=0;
		while(*s1)
		{
			s1=BASM_Parse_Token(s1, b, &ty);
			if(!strcmp(b, ","))continue;

			ab[n++]=basm_strdup(b);
		}
		ab[n]=NULL;

		BASM_PP_AddDefineArgs(tn, ab, "");
		basm_pp_inmacro=tn;
		return;
	}

	if(!strcmp(b, "endmacro"))
	{
		basm_pp_inmacro=NULL;
		return;
	}

	if(!strcmp(b, "enterscope"))
	{
		BASM_PP_PushIncludeLevel();
		return;
	}

	if(!strcmp(b, "exitscope"))
	{
		BASM_PP_PopIncludeLevel();
		return;
	}


	printf("BASM_PP_Directive: Unsupported preproc directive %s\n", b);
	basm_pp_err++;
}

char *BASM_PP_ParseArg(char *s, char *b)
{
	int i;

	i=0;
	while(*s)
	{
		if(*s=='(') { i++; *b++=*s++; continue; }
		if(*s==')') { if(!i)break; i--; *b++=*s++; continue; }
		if(*s=='[') { i++; *b++=*s++; continue; }
		if(*s==']') { i--; *b++=*s++; continue; }
		if(*s=='{') { i++; *b++=*s++; continue; }
		if(*s=='}') { i--; *b++=*s++; continue; }
		if(*s==',') { if(!i)break; *b++=*s++; continue; }
		*b++=*s++;
	}

	*b++=0;
	return(s);
}

char *BASM_PP_PrintName(char *t, char *s)
{
	int i;

	i=*(t-1);
	if((i==';') || (i=='_') || ((i>='0') && (i<='9')) ||
		((i>='A') && (i<='Z')) || ((i>='a') && (i<='z')))
			*t++=' ';
	strcpy(t, s);
	t+=strlen(s);
	return(t);
}

char *BASM_PP_PrintToken(char *t, char *s, int ty)
{
	if(ty==BASM_TOKEN_NAME)
	{
		t=BASM_PP_PrintName(t, s);
		return(t);
	}

	if(ty==BASM_TOKEN_NUMBER)
	{
		t=BASM_PP_PrintName(t, s);
		return(t);
	}

	if(ty==BASM_TOKEN_STRING)
	{
		t=BASM_PP_EmitString(t, s);
		return(t);
	}

	if(ty==BASM_TOKEN_CHARSTRING)
	{
		t=BASM_PP_EmitCharString(t, s);
		return(t);
	}

	if(!strcmp(s, ";"))
	{
		*t++=';'; *t++=' '; *t=0;
		return(t);
	}

	sprintf(t, "%s", s);
	t+=strlen(t);
	return(t);
}

int BASM_PP_Expand(char **src, char **dst, char *op)
{
	char b[256], b2[256];
	char *ab[64];
	char *s, *s1, *t;
	struct basm_ppdef_s *def;
	int i, j, ty, ty2;

	s=*src; t=*dst;

	if(!strcmp(op, "defined"))
	{
		s=BASM_Parse_Token(s, b, &ty);	//<name> | '('
		if(!strcmp(b, "("))
		{
			s=BASM_Parse_Token(s, b, &ty); //<name>
			def=BASM_PP_LookupDefine(b);
			s=BASM_Parse_Token(s, b, &ty); //')'

			sprintf(t, "%d ", def?1:0);
			t+=strlen(t);

			*src=s; *dst=t;
			return(1);
		}

		def=BASM_PP_LookupDefine(b);
		sprintf(t, "%d ", def?1:0);
		t+=strlen(t);

		*src=s; *dst=t;
		return(1);
	}

	def=BASM_PP_LookupDefine(op);
	if(!def)return(0);

	if(!def->args)
	{
		t=BASM_PP_PrintName(t, def->value);
		*src=s; *dst=t;
		return(1);
	}

//	if(*s!='(')return(0);

	if(*s=='(')s++;
	j=0;
	while(def->args[j])
	{
		if(*s==')') break;
		if(*s==',') { s++; continue; }
		while(*s && (*s<=' '))s++;

		s=BASM_PP_ParseArg(s, b2);
		ab[j++]=basm_strdup(b2);
	}
	ab[j]=NULL;

	if(def->args[j] || (*s!=')'))
	{
		printf("BASM_PP_Expand: macro parse problem\n");
		basm_pp_err++;
		return(0);
	}
	s++;

	*src=s;

	s=def->value;
	while(*s)
	{
		s=BASM_Parse_Token(s, b, &ty);

		if(ty==BASM_TOKEN_NAME)
		{
			for(i=0; def->args[i]; i++)
				if(!strcmp(def->args[i], b))
					break;

			if(def->args[i])
			{
				t=BASM_PP_PrintName(t, ab[i]);
				continue;
			}

			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_NUMBER)
		{
			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_STRING)
		{
			t=BASM_PP_EmitString(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_CHARSTRING)
		{
			t=BASM_PP_EmitCharString(t, b);
			continue;
		}

		sprintf(t, "%s", b);
		t+=strlen(t);
	}

	*t=0;

	*dst=t;
	return(1);
}

int BASM_PP_ExpandBinary(char *larg, char *op, char *rarg, char *dst)
{
	float f, g, h;
	int i, j, k;

	f=atof(larg); g=atof(rarg); i=0;
	if(!strcmp(op, "+")) { h=f+g; i=1; }
	if(!strcmp(op, "-")) { h=f-g; i=1; }
	if(!strcmp(op, "*")) { h=f*g; i=1; }

	if(!strcmp(op, "/"))
	{
		if(g==0)return(-1);
		h=f/g; i=1;
	}

	if(i)
	{
		if(((int)h)==h)
		{
			sprintf(dst, "%d", (int)h);
			return(0);
		}
		sprintf(dst, "%f", h);
		return(0);
	}

	i=f; j=g;
	if(i!=f)return(-1);
	if(j!=g)return(-1);

	if(!strcmp(op, "&"))
		{ sprintf(dst, "%d", i&j); return(0); }
	if(!strcmp(op, "|"))
		{ sprintf(dst, "%d", i|j); return(0); }
	if(!strcmp(op, "^"))
		{ sprintf(dst, "%d", i^j); return(0); }
	if(!strcmp(op, "&&"))
		{ sprintf(dst, "%d", i&&j); return(0); }
	if(!strcmp(op, "||"))
		{ sprintf(dst, "%d", i||j); return(0); }

	return(-1);
}

int BASM_PP_LineArith(char *str)
{
	char lbuf[4096];
	char b[256], b2[256], b3[256];
	char *s, *s1, *s2, *t, *t1;
	int i, ty, ty2, ty3, ni;

	ni=0;
	s=str; t=lbuf;
	while(*s)
	{
		s=BASM_Parse_Token(s, b, &ty);
		s1=BASM_Parse_Token(s, b2, &ty2);
		s2=BASM_Parse_Token(s1, b3, &ty3);

#if 1
		if((ty==BASM_TOKEN_NUMBER) &&
			(ty2==BASM_TOKEN_OPERATOR) &&
			(ty3==BASM_TOKEN_NUMBER))
		{
			i=BASM_PP_ExpandBinary(b, b2, b3, b2);
			if(i>=0)
			{
				s=s2; t=BASM_PP_PrintName(t, b2);
				ni++;
				continue;
			}

			t=BASM_PP_PrintName(t, b);
			continue;
		}
		if(!strcmp(b, "(") && !strcmp(b3, ")") &&
			(ty2==BASM_TOKEN_NUMBER))
		{
			t=BASM_PP_PrintName(t, b2);
			ni++;
			continue;
		}
#endif

		if(ty==BASM_TOKEN_NAME)
		{
			i=BASM_PP_Expand(&s, &t, b);
			if(i>0)
			{
				ni++;
				continue;
			}

			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_NUMBER)
		{
			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_STRING)
		{
			t=BASM_PP_EmitString(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_CHARSTRING)
		{
			t=BASM_PP_EmitCharString(t, b);
			continue;
		}

		sprintf(t, "%s", b);
		t+=strlen(t);
	}
	*t=0;

	strcpy(str, lbuf);
	return(ni);
}


int BASM_PP_Line(char *str)
{
	char lbuf[4096];
	char b[256], b2[256];
	char *s, *t, *t1;
	int i, ty, ty2, ty3, ni;

	ni=0;
	s=str; t=lbuf;
	while(*s)
	{
		while(*s && (*s<=' ') && (*s!='\n'))s++;
		if(*s=='\n') { *t++=*s++; *t=0; continue; }

		s=BASM_Parse_Token(s, b, &ty);
		BASM_Parse_Token(s, b2, &ty2);

		if((ty2==BASM_TOKEN_OPERATOR) &&
			(!strcmp(b2, "##") || !strcmp(b, "%:%:")))
		{
			if(ty==BASM_TOKEN_NAME)
			{
				t1=t;
				i=BASM_PP_Expand(&s, &t1, b);
				if(i>0) { strcpy(b, t); ni++; }
			}

			s=BASM_Parse_Token(s, b2, &ty2);	//'##'

			s=BASM_Parse_Token(s, b2, &ty2);

			if(ty2==BASM_TOKEN_NAME)
			{
				t1=t;
				i=BASM_PP_Expand(&s, &t1, b2);
				if(i>0) { strcpy(b2, t); ni++; }
			}

			strcat(b, b2);
			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_NAME)
		{
			i=BASM_PP_Expand(&s, &t, b);
			if(i>0)
			{
				ni++;
				continue;
			}

			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if((ty==BASM_TOKEN_OPERATOR) &&
			(!strcmp(b, "#") || !strcmp(b, "%:")))
				if(ty2==BASM_TOKEN_NAME)
		{
			s=BASM_Parse_Token(s, b2, &ty2);

			t1=t;
			i=BASM_PP_Expand(&s, &t1, b2);
			if(i>0) { strcpy(b2, t); ni++; }

			t=BASM_PP_PrintToken(t, b2, BASM_TOKEN_STRING);
			continue;
		}

		if(ty==BASM_TOKEN_NUMBER)
		{
			t=BASM_PP_PrintName(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_STRING)
		{
			t=BASM_PP_EmitString(t, b);
			continue;
		}

		if(ty==BASM_TOKEN_CHARSTRING)
		{
			t=BASM_PP_EmitCharString(t, b);
			continue;
		}

		sprintf(t, "%s", b);
		t+=strlen(t);
	}
	*t=0;

	strcpy(str, lbuf);
	return(ni);
}

char *BASM_PP_ParseLine(char *s, char *b)
{
	static int is=0;
	char *t;

	t=b;
//	while(*s && (*s!='\n') && (*s!='\r'))
	while(*s)
	{

		if(*s=='\"')
		{
			is=!is;
			*t++=*s++;
			continue;
		}

		if(is)
		{
			if(is && (s[0]=='\\') && (s[1]=='\"'))
				{ *t++=*s++; *t++=*s++; continue; }
			if(is && (s[0]=='\\') && (s[1]=='\\'))
				{ *t++=*s++; *t++=*s++; continue; }

			*t++=*s++;
			continue;
		}

		if(s[0]<=' ')
		{
			if((*s=='\n') || (*s=='\r'))
				break;

//			if((s[0] && (s[0]<=' ')) && (s[1]==';'))
//			if((s[0]<=' ') && (s[1]==';'))
			if(s[1]==';')
			{
				while(*s && (*s!='\n') && (*s!='\r'))
					s++;
				break;
			}

			*t++=*s++;
			continue;
		}

		if(s[0]=='/')
		{
			if((s[0]=='/') && (s[1]=='/'))
			{
				while(*s && (*s!='\n') && (*s!='\r'))
					s++;
				break;
			}

			if((s[0]=='/') && (s[1]=='*'))
			{
				s+=2;
//				while(*s && (*s!='\n') && (*s!='\r'))
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
		}

		if((*s=='\\') && ((s[1]=='\r') || (s[1]=='\n')))
		{
			s++;
			while(*s && (*s<=' '))s++;
			*t++=' ';
			continue;
		}
		*t++=*s++;
	}
	*t++=0;
	if(*s=='\r')s++;
	if(*s=='\n')s++;

	return(s);
}


void BASM_PP_Buffer(char *ibuf)
{
	char b[4096];
	char *s, *t, *s1;
	int i;

	s=ibuf;
	while(*s)
	{
		basm_pp_spos=s;
		s=BASM_PP_ParseLine(s, b);

		t=b; while(*t && (*t<=' '))t++;
		if(!*t)
		{
			*basm_pp_obuf=0;
			continue;
		}

		if((b[0]=='#') && (b[1]=='!'))
		{
			*basm_pp_obuf=0;
			continue;
		}

		if((b[0]=='#') && (b[1]=='#'))
		{
			if(basm_pp_inmacro)
			{
				if(basm_pp_iflvl2)continue;
				BASM_PP_AppendDefineLine(
					basm_pp_inmacro, t+1);
				continue;
			}
		}

		if(b[0]=='#')
		{
			BASM_PP_Directive(b);
			continue;
		}
		if(b[0]=='%')
		{
			BASM_PP_Directive(b);
			continue;
		}

		if(basm_pp_iflvl2)continue;

		if(basm_pp_inmacro)
		{
			BASM_PP_AppendDefineLine(basm_pp_inmacro, b);
			continue;
		}

		i=1;
		while(i)i=BASM_PP_Line(b);

#if 1
		//special case: macros expand to multiple lines or
		//preprocessor directives
		s1=b; while(*s1 && (*s1<=' '))s1++;
		if(*s1!='#') { while(*s1 && (*s1!='\n'))s1++; }
		if(*s1)
		{
			BASM_PP_Buffer(b);
			continue;
		}
#endif

		sprintf(basm_pp_obuf, "%s\n", b);
		basm_pp_obuf+=strlen(basm_pp_obuf);
	}
}


int BASM_PP_Filter(char *ibuf, char *obuf)
{
	char tb[256];
	char *s, *t;
	int i;


	for(i=0; i<basm_pp_nsinc; i++)
		basm_pp_inc[i]=basm_pp_sinc[i];
	basm_pp_ninc=basm_pp_nsinc;

	basm_pp_ibuf=ibuf;
	basm_pp_obuf=obuf;

	basm_pp_def=NULL;
	basm_pp_ilvl=0;
	basm_pp_ildef[0]=NULL;

	basm_pp_warn=0;
	basm_pp_err=0;

	BASM_PP_Buffer(ibuf);

	BASM_PP_CleanupDefines();

	return(0);
}

