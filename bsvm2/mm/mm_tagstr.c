/*
 * Strings.
 */

// #include <bteifgl.h>

static int objty_tagstr=-1;
static int objty_tagstr_l1=-1;
static int objty_tagwstr=-1;
static int objty_tagsym=-1;
static int objty_tagkeyw=-1;

void BGBDT_TagStr_InitTypes(void)
{
	if(objty_tagstr>=0)
		return;

	objty_tagstr=0;		//anti recursion hack
	objty_tagstr=BGBDT_MM_GetIndexObjTypeName("_string_t");
	objty_tagwstr=BGBDT_MM_GetIndexObjTypeName("_wstring_t");
	objty_tagstr_l1=BGBDT_MM_GetIndexObjTypeName("_string_l1_t");

	objty_tagsym=BGBDT_MM_GetIndexObjTypeName("_symbol_t");
	objty_tagkeyw=BGBDT_MM_GetIndexObjTypeName("_keyword_t");
}


// BS2VM_API char *bgbdt_mm_strdup(char *str)
//	{ return(strdup(str)); }

char *BGBDT_TagStr_TabStrdup(BGBDT_TagStrTab *tab, char *str)
{
	char *s, *t;
	int i;

	if(!str)
	{
//		*(int *)-1=-1;
		return(NULL);
	}

	BGBDT_TagStr_InitTypes();

//	if(!*str)return("");

	if(!tab->strhash)
	{
//		tab->strtab=bgbdt_mm_malloc(1<<20);
//		tab->strtab=bgbdt_mm_tyalloc("bgbdt_mm_strtab_t", 1<<20);
		tab->strtab=dtmAlloc(tab->strtyn, 1<<20);
		tab->strtabe=tab->strtab+1;
		tab->estrtab=tab->strtab+(1<<20);

		tab->pstrtab[0]=tab->strtab;
		tab->npstrtab=1;

		i=4096*sizeof(char *);
//		tab->strhash=bgbdt_mm_malloc(i);
		tab->strhash=dtmAlloc("tagstr_hash_t", i);
		memset(tab->strhash, 0, i);
	}

	if(!*str)return(tab->pstrtab[0]);

	if((tab->strtabe+(strlen(str)+2))>=tab->estrtab)
	{
//		tab->strtab=bgbdt_mm_malloc(1<<20);
//		tab->strtab=bgbdt_mm_tyalloc("bgbdt_mm_strtab_t", 1<<20);
		tab->strtab=dtmAlloc(tab->strtyn, 1<<20);
		tab->strtabe=tab->strtab;
		tab->estrtab=tab->strtab+(1<<20);

		tab->pstrtab[tab->npstrtab++]=tab->strtab;
	}

	i=0; s=str;
	while(*s)i=i*251+(*s++);
	i=((i*251)>>8)&0xFFF;
//	i=((i*251)>>8)&0x3FFF;

	t=tab->strhash[i];
	while(t)
	{
		s=(char *)(((char **)t)+1);
		t=*(char **)t;
		if(!strcmp(s, str))return(s);
	}

	t=tab->strtabe;
	tab->strtabe=t+strlen(str)+1+sizeof(char *);

	s=(char *)(((char **)t)+1);
	strcpy(s, str);

	*(char **)t=tab->strhash[i];
	tab->strhash[i]=t;
	return(s);
}

int bgbdt_strcmp16(u16 *s1, u16 *s2)
{
	while(*s1 && *s2 && (*s1==*s2))
		{ s1++; s2++; }
	if(*s1>*s2)
		return(1);
	if(*s1<*s2)
		return(-1);
	return(0);
}

int bgbdt_strlen16(u16 *str)
{
	u16 *s;
	s=str;
	while(*s)s++;
	return(s-str);
}

void bgbdt_strcpy16(u16 *sdst, u16 *ssrc)
{
	u16 *s, *t;
	
	s=ssrc;
	t=sdst;
	while(*s)
		*t++=*s++;
	*t++=0;
}

u16 *BGBDT_TagStr_TabStrdup16(BGBDT_TagStrTab16 *tab, u16 *str)
{
	u16 *s, *t;
	int i;

	if(!str)
	{
//		*(int *)-1=-1;
		return(NULL);
	}

	BGBDT_TagStr_InitTypes();

	if(!tab->strhash)
	{
//		tab->strtab=bgbdt_mm_malloc(1<<20);
//		tab->strtab=bgbdt_mm_tyalloc("bgbdt_mm_strtab_t", 1<<20);
		tab->strtab=dtmAlloc(tab->strtyn, 1<<20);
		tab->strtabe=tab->strtab;
		tab->estrtab=tab->strtab+(1<<19);

		tab->pstrtab[0]=tab->strtab;
		tab->npstrtab=1;

		i=4096*sizeof(u16 *);
//		tab->strhash=bgbdt_mm_malloc(i);
		tab->strhash=dtmAlloc("tagstr_hash_t", i);
		memset(tab->strhash, 0, i);
	}

	if(!*str)return(tab->pstrtab[0]);

	if((tab->strtabe+(bgbdt_strlen16(str)+2))>=tab->estrtab)
	{
//		tab->strtab=bgbdt_mm_malloc(1<<20);
//		tab->strtab=bgbdt_mm_tyalloc("bgbdt_mm_strtab_t", 1<<20);
		tab->strtab=dtmAlloc(tab->strtyn, 1<<20);
		tab->strtabe=tab->strtab;
		tab->estrtab=tab->strtab+(1<<19);

		tab->pstrtab[tab->npstrtab++]=tab->strtab;
	}

	i=0; s=str;
	while(*s)i=i*251+(*s++);
	i=((i*251)>>8)&0xFFF;
//	i=((i*251)>>8)&0x3FFF;

	t=tab->strhash[i];
	while(t)
	{
		s=(u16 *)(((u16 **)t)+1);
		t=*(u16 **)t;
		if(!bgbdt_strcmp16(s, str))return(s);
	}

	t=tab->strtabe;
	tab->strtabe=t+bgbdt_strlen16(str)+1+sizeof(u16 *);

	s=(u16 *)(((u16 **)t)+1);
	bgbdt_strcpy16(s, str);

	*(u16 **)t=tab->strhash[i];
	tab->strhash[i]=t;
	return(s);
}

BS2VM_API char *BGBDT_TagStr_Strdup(char *str)
{
	static struct BGBDT_TagStrTab_s tab_strdup_u8={"_string_t"};
	return(BGBDT_TagStr_TabStrdup(&tab_strdup_u8, str));
}

BS2VM_API char *BGBDT_TagStr_StrdupL1(char *str)
{
	static struct BGBDT_TagStrTab_s tab_strdup_l1={"_string_l1_t"};
	return(BGBDT_TagStr_TabStrdup(&tab_strdup_l1, str));
}

BS2VM_API u16 *BGBDT_TagStr_Strdup16(u16 *str)
{
	static struct BGBDT_TagStrTab16_s tab_strdup_u16={"_wstring_t"};
	return(BGBDT_TagStr_TabStrdup16(&tab_strdup_u16, str));
}

BS2VM_API char *BGBDT_TagStr_StrSymbol(char *str)
{
	static struct BGBDT_TagStrTab_s tab_strdup_u8={"_symbol_t"};
	return(BGBDT_TagStr_TabStrdup(&tab_strdup_u8, str));
}

BS2VM_API char *BGBDT_TagStr_StrKeyword(char *str)
{
	static struct BGBDT_TagStrTab_s tab_strdup_u8={"_keyword_t"};
	return(BGBDT_TagStr_TabStrdup(&tab_strdup_u8, str));
}

BS2VM_API u16 *BGBDT_TagStr_Strdup16u8(char *str)
{
	u16 *tbuf;
	u16 *t;
	byte *s;
	int i, j, k, l;

	l=strlen(str);
	tbuf=bgbdt_mm_ralloc(l*2+3);
	
	s=(byte *)str;
	t=tbuf;
	
	while(*s)
	{
		if(*s>=0x80)
		{
			if((*s&0xE0)==0xC0)
				{ i=((s[0]&0x3F)<<6)|(s[1]&63); s+=2; }
			else if((*s&0xF0)==0xE0)
				{ i=((s[0]&0x1F)<<12)|((s[1]&63)<<6)|(s[2]&63); s+=3; }
			else if((*s&0xF8)==0xF0)
			{
				i=((s[0]&0x0F)<<18)|((s[1]&63)<<12)|
					((s[2]&63)<<6)|(s[3]&63);
				s+=4;
			}else
			{
				i=*s++;
			}
			if(i>=0x10000)
			{
				i=i-0x10000;
				*t++=0xD800+((i>>10)&1023);
				*t++=0xDC00+((i    )&1023);
				continue;
			}
			*t++=i;
			continue;
		}
		*t++=*s++;
	}
	*t++=0;
	
	return(BGBDT_TagStr_Strdup16(tbuf));
}

BS2VM_API dtVal BGBDT_TagStr_String(char *str)
{
	return(dtvWrapPtr(BGBDT_TagStr_Strdup(str)));
}

BS2VM_API dtVal BGBDT_TagStr_StringAsc(char *str)
{
	return(dtvWrapPtr(BGBDT_TagStr_StrdupL1(str)));
}

BS2VM_API dtVal BGBDT_TagStr_String16(u16 *str)
{
	return(dtvWrapPtr(BGBDT_TagStr_Strdup16(str)));
}

BS2VM_API dtVal BGBDT_TagStr_String16u8(byte *str)
{
	return(dtvWrapPtr(BGBDT_TagStr_Strdup16u8(str)));
}

BS2VM_API dtVal BGBDT_TagStr_Symbol(char *str)
{
	return(dtvWrapPtr(BGBDT_TagStr_StrSymbol(str)));
}

BS2VM_API dtVal BGBDT_TagStr_Keyword(char *str)
{
	return(dtvWrapPtr(BGBDT_TagStr_StrKeyword(str)));
}


BS2VM_API int BGBDT_TagStr_IsStringP(dtVal val)
{
	int tag;

	tag=dtvGetPtrTagF(val);

	if((tag==objty_tagstr)|(tag==objty_tagwstr)|
		(tag==objty_tagstr_l1))
			return(1);

//	if(dtvCheckPtrTagP(val, objty_tagstr))
//		return(1);
//	if(dtvCheckPtrTagP(val, objty_tagwstr))
//		return(1);
//	if(dtvCheckPtrTagP(val, objty_tagstr_l1))
//		return(1);
	return(0);
}

BS2VM_API int BGBDT_TagStr_IsSymbolP(dtVal val)
{
	int tag;
	tag=dtvGetPtrTagF(val);
	if((tag==objty_tagsym))
			return(1);
	return(0);
}

BS2VM_API int BGBDT_TagStr_IsKeywordP(dtVal val)
{
	int tag;
	tag=dtvGetPtrTagF(val);
	if((tag==objty_tagkeyw))
			return(1);
	return(0);
}

BS2VM_API char *BGBDT_TagStr_GetUtf8(dtVal val)
{
	char *tbuf;
	char *str;
	u16 *ws;
	byte *s, *t;
	int tag, len;
	int i, j, k;

	tag=dtvGetPtrTagF(val);
	str=dtvUnwrapPtrF(val);
	
	if((tag==objty_tagstr) | (tag==objty_tagsym) |
		(tag==objty_tagkeyw))
	{
		return(str);
	}

	if(tag==objty_tagstr_l1)
	{
		len=strlen(str);
		tbuf=bgbdt_mm_ralloc(len*2+1);
		s=(byte *)str;	t=(byte *)tbuf;
		while(*s)
		{
			if(*s<0x80)
				{ *t++=*s++; continue; }
			i=*s++;
			*t++=0xC0|(i>>6);
			*t++=0x80|(i&63);
		}
		*t++=0;
		return(tbuf);
	}

	if(tag==objty_tagwstr)
	{
		len=bgbdt_strlen16((u16 *)str);
		tbuf=bgbdt_mm_ralloc(len*3+1);
		ws=(u16 *)str;
		t=(byte *)tbuf;
		while(*ws)
		{
			if(*ws<0x80)
				{ *t++=*ws++; continue; }
			i=*ws++;
			if(i<0x800)
			{
				*t++=0xC0|(i>>6);
				*t++=0x80|(i&63);
				continue;
			}
			*t++=0xE0|(i>>12);
			*t++=0x80|((i>>6)&63);
			*t++=0x80|(i&63);
		}
		*t++=0;
		return(tbuf);
	}
	
	return(NULL);
}

BS2VM_API char *BGBDT_TagStr_GetChars(dtVal val)
{
	char *tbuf;
	char *str;
	u16 *ws;
	byte *s, *t;
	int tag, len;
	int i, j, k;

	tag=dtvGetPtrTagF(val);
	str=dtvUnwrapPtrF(val);
	
	if((tag==objty_tagstr) | (tag==objty_tagsym) |
		(tag==objty_tagkeyw) | (tag==objty_tagstr_l1))
	{
		return(str);
	}

	return(BGBDT_TagStr_GetUtf8(val));
}

BS2VM_API int BGBDT_TagStr_SymbolIsP(dtVal val, char *name)
{
	char *str;
	int tag;

	tag=dtvGetPtrTagF(val);
	if((tag==objty_tagsym))
	{
		str=dtvUnwrapPtrF(val);
		if(!strcmp(str, name))
			return(1);
		return(0);
	}
	return(0);
}


BS2VM_API dtVal BGBDT_TagStr_StringAdjustOffset(dtVal val, int idx)
{
	char *tbuf;
	char *str;
	u16 *ws;
	byte *s, *t;
	int tag, len;
	int i, j, k;

	tag=dtvGetPtrTagF(val);
	str=dtvUnwrapPtrF(val);
	
	if((tag==objty_tagstr) | (tag==objty_tagsym) |
		(tag==objty_tagkeyw))
	{
		s=(byte *)str;
		if(idx>0)
		{
			i=idx;
			while(i--)
			{
				if(!*s)break;
				if(*s<0x80)
					{ s++; continue; }
				if(	(((*(s+0))&0xE0)==0xC0) &&
					(((*(s+1))&0xC0)==0x80))
						{ s+=2; continue; }
				if(	(((*(s+0))&0xF0)==0xE0) &&
					(((*(s+1))&0xC0)==0x80) &&
					(((*(s+2))&0xC0)==0x80))
						{ s+=3; continue; }
				s++;
			}
			if(i>0)return(DTV_NULL);
			return(dtvWrapPtr(s));
		}else if(idx<0)
		{
			i=idx;
			while(i--)
			{
				if(!*(s-1))break;
				if(*(s-1)<0x80)
					{ s--; continue; }
				if(	(((*(s-1))&0xC0)==0x80) &&
					(((*(s-2))&0xE0)==0xC0))
						{ s-=2; continue; }
				if(	(((*(s-1))&0xC0)==0x80) &&
					(((*(s-2))&0xC0)==0x80) &&
					(((*(s-3))&0xE0)==0xC0))
						{ s-=3; continue; }
				s--;
			}
			if(i>0)return(DTV_NULL);
			return(dtvWrapPtr(s));
		}
//		s=str+idx;
		return(dtvWrapPtr(s));
	}

	if(tag==objty_tagstr_l1)
	{
		s=(byte *)str;
		if(idx>0)
		{
			i=idx;
			while(i--)
			{
				if(!*s)break;
				s++;
			}
			if(i>0)return(DTV_NULL);
			return(dtvWrapPtr(s));
		}else if(idx<0)
		{
			i=idx;
			while(i--)
			{
				if(!*(s-1))break;
				s--;
			}
			if(i>0)return(DTV_NULL);
			return(dtvWrapPtr(s));
		}
//		s=str+idx;
		return(dtvWrapPtr(s));
	}

	if(tag==objty_tagwstr)
	{
		ws=(u16 *)str;
		if(idx>0)
		{
			i=idx;
			while(i--)
			{
				if(!*ws)break;
				ws++;
			}
			if(i>0)return(DTV_NULL);
			return(dtvWrapPtr(ws));
		}else if(idx<0)
		{
			i=idx;
			while(i--)
			{
				if(!*(ws-1))break;
				ws--;
			}
			if(i>0)return(DTV_NULL);
			return(dtvWrapPtr(ws));
		}
//		ws=ws+idx;
		return(dtvWrapPtr(ws));
	}
	
	return(DTV_NULL);
}
