#include <bteifgl.h>

BGBDT_MM_ParsePrintInfo *bgbdt_mm_freeprint=NULL;

BTEIFGL_API BGBDT_MM_ParsePrintInfo *BGBDT_MM_AllocParsePrintInfo()
{
	BGBDT_MM_ParsePrintInfo *tmp;
	
	tmp=bgbdt_mm_freeprint;
	if(tmp)
	{
		bgbdt_mm_freeprint=tmp->next;
		memset(tmp, 0, sizeof(BGBDT_MM_ParsePrintInfo));
		return(tmp);
	}
	
	tmp=dtmAlloc("bgbdt_parseprintinfo_t",
		sizeof(BGBDT_MM_ParsePrintInfo));
	return(tmp);
}

BTEIFGL_API int BGBDT_MM_FreeParsePrintInfo(BGBDT_MM_ParsePrintInfo *inf)
{
	inf->next=bgbdt_mm_freeprint;
	bgbdt_mm_freeprint=inf;
}

int bgbdt_mm_strprint_putstr(BGBDT_MM_ParsePrintInfo *inf, char *str)
{
	char *s;
	int l;

	l=strlen(str);
	if((inf->ct+l)>=inf->cte)
	{
		s=str;
		while((inf->ct+1)<inf->cte)
			{ *inf->ct++=*s++; }
		if(inf->ct<inf->cte)
			*inf->ct=0;
		return(-1);
	}

	inf->nchars+=l;
	memcpy(inf->ct, str, l+1);
	inf->ct+=l;
//	return(inf->cte-inf->ct);
	return(inf->ct-inf->cts);
}

BTEIFGL_API BGBDT_MM_ParsePrintInfo *BGBDT_MM_NewStringPrinter(
	char *strbuf, int szbuf)
{
	BGBDT_MM_ParsePrintInfo *tmp;

	tmp=BGBDT_MM_AllocParsePrintInfo();

	tmp->ct=strbuf;
	tmp->cts=strbuf;
	tmp->cte=strbuf+szbuf;
	tmp->putstr=bgbdt_mm_strprint_putstr;
	
	return(tmp);
}

int BGBDT_MM_PrintPutStr(BGBDT_MM_ParsePrintInfo *inf, char *str)
{
	return(inf->putstr(inf, str));
}

int BGBDT_MM_PrintPutInt(BGBDT_MM_ParsePrintInfo *inf, s64 v)
{
	char tb[64];

#ifdef _MSC_VER
	sprintf(tb, "%I64d", v);
#else
	sprintf(tb, "%lld", v);
#endif
	return(BGBDT_MM_PrintPutStr(inf, tb));
}

int BGBDT_MM_PrintPutIntHex64(BGBDT_MM_ParsePrintInfo *inf, s64 v)
{
	char tb[64];

	sprintf(tb, "%08X_%08X", (u32)(v>>32), (u32)v);
	return(BGBDT_MM_PrintPutStr(inf, tb));
}

int BGBDT_MM_PrintPutIntHex64B(BGBDT_MM_ParsePrintInfo *inf, s64 v)
{
	char tb[64];

	sprintf(tb, "0x%08X_%08X", (u32)(v>>32), (u32)v);
	return(BGBDT_MM_PrintPutStr(inf, tb));
}

int BGBDT_MM_PrintPutFloat(BGBDT_MM_ParsePrintInfo *inf, double v)
{
	char tb[64];
	char *s;

	sprintf(tb, "%.32f", v);
	s=tb+strlen(tb)-1;

	if(*s=='0')
	{
		while((s>tb) && (*s=='0'))
			s--;
		s[1]=0;
	}
	
	if((*s=='9') && ((s-3)>tb) && !strcmp(s-3, "9999"))
	{
		while((s>tb) && (*s=='9'))
			s--;
		if((*s>='0') && (*s<='8'))
			{ *s=*s+1; s[1]=0; }
	}
	
	return(BGBDT_MM_PrintPutStr(inf, tb));
}

int BGBDT_MM_PrintPutPrintf(BGBDT_MM_ParsePrintInfo *inf, char *str, ...)
{
	char tb[1024];
	va_list lst;
	
	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);
	return(BGBDT_MM_PrintPutStr(inf, tb));
}

static int bgbdt_hexnib(int v)
{
	int i;
	i=v&15;
	if(i<10)
		return('0'+i);
	return('A'+(i-10));
}

static int bgbdt_symcleanp(char *str)
{
	char *s;
	
	s=str;

	if((*s>='0') && (*s<='9'))
		return(0);

	while(*s)
	{
		if((*s=='_') || (*s=='$'))
			{ s++; continue; }
		if((*s>='A') && (*s<='Z'))
			{ s++; continue; }
		if((*s>='a') && (*s<='z'))
			{ s++; continue; }
		if((*s>='0') && (*s<='9'))
			{ s++; continue; }
		break;
	}
	return(!(*s));
}

int BGBDT_MM_PrintPutString(
	BGBDT_MM_ParsePrintInfo *inf, char *str)
{
	char tb[1024];
	char *s, *t;
	int i, j, k;
	
	s=str;	t=tb;
	
	*t++='\"';
	while(*s)
	{
		s=BS2P_ReadUtf8(s, &i);
		if((i>=' ') && (i<='~') && (i!='\"') && (i!='\''))
			{ *t++=i; continue; }
		if(i=='\"')
			{ *t++='\\'; *t++='\"'; continue; }
		if(i=='\'')
			{ *t++='\\'; *t++='\''; continue; }
		if(i=='\r')
			{ *t++='\\'; *t++='r'; continue; }
		if(i=='\n')
			{ *t++='\\'; *t++='n'; continue; }
		if(i=='\t')
			{ *t++='\\'; *t++='t'; continue; }
		if(i=='\b')
			{ *t++='\\'; *t++='b'; continue; }
		if(i<0x0080)
		{
			*t++='\\'; *t++='x';
			*t++=bgbdt_hexnib(i>>4);
			*t++=bgbdt_hexnib(i);
			continue;
		}
		*t++='\\'; *t++='u';
		*t++=bgbdt_hexnib(i>>12);
		*t++=bgbdt_hexnib(i>>8);
		*t++=bgbdt_hexnib(i>>4);
		*t++=bgbdt_hexnib(i);
		continue;
	}
	*t++='\"';
	*t++=0;

	return(BGBDT_MM_PrintPutStr(inf, tb));
}

int BGBDT_MM_PrintObjValue(BGBDT_MM_ParsePrintInfo *inf, dtVal val)
{
	char tb[256];
	int slis[256];
	s64 vi;
	double vf;
	dtVal v0;
	int tyid, ns, bt;
	char *tyn, *fn;
	int i, j, k;
	
	if(dtvNullP(val))
	{
		i=BGBDT_MM_PrintPutStr(inf, "null");
		return(i);
	}

	if(dtvIsArrayP(val))
	{
		i=BGBDT_MM_PrintTagArr(inf, val);
		return(i);
	}

	if(BGBDT_MapObj_IsObjectP(val))
	{
		i=BGBDT_MM_PrintMapObjValue(inf, val);
		return(i);
	}
	
	if(BGBDT_TagStr_IsStringP(val))
	{
		fn=BGBDT_TagStr_GetUtf8(val);
		i=BGBDT_MM_PrintPutString(inf, fn);
		return(i);
	}

	if(BGBDT_TagStr_IsSymbolP(val))
	{
		fn=BGBDT_TagStr_GetUtf8(val);
		BGBDT_MM_PrintPutStr(inf, "#");
		if(!bgbdt_symcleanp(fn))
			{ i=BGBDT_MM_PrintPutString(inf, fn); }
		else
			{ i=BGBDT_MM_PrintPutStr(inf, fn); }
		return(i);
	}

	if(BGBDT_TagStr_IsKeywordP(val))
	{
		fn=BGBDT_TagStr_GetUtf8(val);
		BGBDT_MM_PrintPutStr(inf, "#:");
//		i=BGBDT_MM_PrintPutString(inf, fn);
		if(!bgbdt_symcleanp(fn))
			{ i=BGBDT_MM_PrintPutString(inf, fn); }
		else
			{ i=BGBDT_MM_PrintPutStr(inf, fn); }
		return(i);
	}

	BGBDT_MM_PrintPutStr(inf, "#?");
	i=BGBDT_MM_PrintPutIntHex64(inf, val.vi);

	return(i);
}

int BGBDT_MM_PrintMapObjValue(BGBDT_MM_ParsePrintInfo *inf, dtVal val)
{
	char tb[256];
	int slis[256];
	s64 vi;
	double vf;
	dtVal v0;
	int tyid, ns, bt;
	char *tyn, *fn;
	int i, j, k;
	
	for(i=0; i<inf->nrec; i++)
		if(dtvEqqP(val, inf->recarr[i]))
	{
		j=BGBDT_MM_PrintPutPrintf(inf, "#%d", i);
		return(j);
	}
	
	i=inf->nrec++;
	inf->recarr[i]=val;
	
	BGBDT_MM_PrintPutStr(inf, "{");
	ns=BGBDT_MapObj_QueryObjSlotList(val, slis, 256);
	for(i=0; i<ns; i++)
	{
		fn=BGBDT_MapObj_GetFieldIndexName(slis[i]);
		sprintf(tb, "%s: ", fn);
		BGBDT_MM_PrintPutStr(inf, tb);
		
		v0=BGBDT_MapObj_GetObjvSlotValue(val, slis[i]);
		BGBDT_MM_PrintValue(inf, v0);

		if((i+1)<ns)
			{ BGBDT_MM_PrintPutStr(inf, ", "); }
	}

	i=BGBDT_MM_PrintPutStr(inf, "}");
	inf->nrec--;
	return(i);
}

int BGBDT_MM_PrintTagArr(BGBDT_MM_ParsePrintInfo *inf, dtVal val)
{
	s64 vi;
	double vf;
	dtVal v0;
	int tyid, ns, bt;
	char *tyn, *fn;
	int i, j, k;
	
	for(i=0; i<inf->nrec; i++)
		if(dtvEqqP(val, inf->recarr[i]))
	{
		j=BGBDT_MM_PrintPutPrintf(inf, "#%d", i);
		return(j);
	}
	
	i=inf->nrec++;
	inf->recarr[i]=val;
	
	BGBDT_MM_PrintPutStr(inf, "[");

	bt=dtvArrayGetBaseType(val);
	ns=dtvArrayGetSize(val);
	for(i=0; i<ns; i++)
	{
		switch(bt)
		{
		case BGBDT_BASETY_INT:
			vi=dtvArrayGetIndexInt(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_LONG:
			vi=dtvArrayGetIndexLong(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_FLOAT:
			vf=dtvArrayGetIndexFloat(val, i);
			BGBDT_MM_PrintPutFloat(inf, vf);
			break;
		case BGBDT_BASETY_DOUBLE:
			vf=dtvArrayGetIndexDouble(val, i);
			BGBDT_MM_PrintPutFloat(inf, vf);
			break;
		case BGBDT_BASETY_ADDRESS:
			v0=dtvArrayGetIndexDtVal(val, i);
			BGBDT_MM_PrintValue(inf, v0);
			break;
		case BGBDT_BASETY_UINT:
			vi=(u32)dtvArrayGetIndexInt(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_UBYTE:
			vi=dtvArrayGetIndexByte(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_SHORT:
			vi=dtvArrayGetIndexShort(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_SBYTE:
			vi=dtvArrayGetIndexSByte(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_USHORT:
			vi=dtvArrayGetIndexUShort(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_ULONG:
			vi=dtvArrayGetIndexLong(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		default:
			i=BGBDT_MM_PrintPutStr(inf, "<?!>");
			break;
		}
		if((i+1)<ns)
			{ BGBDT_MM_PrintPutStr(inf, ", "); }
	}

	i=BGBDT_MM_PrintPutStr(inf, "]");
	inf->nrec--;
	return(i);
}

BTEIFGL_API int BGBDT_MM_PrintValue(
	BGBDT_MM_ParsePrintInfo *inf, dtVal val)
{
	double vf;
	s64 v;
	int i;

	switch(val.hi>>28)
	{
	case BGBDT_HTAG_PTR:
		i=BGBDT_MM_PrintObjValue(inf, val);
		break;
	case BGBDT_HTAG_TAG2:
		if(val.hi==BGBDT_TAG_INT32)
		{
			v=(s32)val.lo;
			i=BGBDT_MM_PrintPutInt(inf, v);
			break;
		}
		if(val.hi==BGBDT_TAG_UINT32)
		{
			v=val.lo;
			i=BGBDT_MM_PrintPutInt(inf, v);
			break;
		}
		if(val.hi==BGBDT_TAG_FLOAT32)
		{
			vf=*(float *)(&val.lo);
			i=BGBDT_MM_PrintPutFloat(inf, vf);
			break;
		}

		if(val.hi==BGBDT_TAG_MCHAR)
		{
			if((val.lo>=' ') && (val.lo<='~') &&
				(val.lo!='\'') && (val.lo!='\"'))
			{
				i=BGBDT_MM_PrintPutPrintf(inf, "'%c'", val.lo);
				break;
			}
			
			if(val.lo<256)
			{
				switch(val.lo)
				{
				case '\'':	case '\"':
					i=BGBDT_MM_PrintPutPrintf(inf, "'\\%c'", val.lo);
					break;
				case '\r': i=BGBDT_MM_PrintPutStr(inf, "'\\r'"); break;
				case '\n': i=BGBDT_MM_PrintPutStr(inf, "'\\n'"); break;
				case '\t': i=BGBDT_MM_PrintPutStr(inf, "'\\r'"); break;
				case '\b': i=BGBDT_MM_PrintPutStr(inf, "'\\b'"); break;
				default:
					i=BGBDT_MM_PrintPutPrintf(inf, "'\\x%02X'", val.lo);
					break;
				}
				break;
			}

			if(val.lo<65536)
			{
				i=BGBDT_MM_PrintPutPrintf(inf, "'\\u%04X'", val.lo);
				break;
			}

			i=BGBDT_MM_PrintPutPrintf(inf, "'\\U%08X'", val.lo);
			break;
		}

		if(val.hi==BGBDT_TAG_MCONST)
		{
			switch(val.lo)
			{
			case 0:
				i=BGBDT_MM_PrintPutStr(inf, "null");
				break;
			case 1:
				i=BGBDT_MM_PrintPutStr(inf, "undefined");
				break;
			case 2:
				i=BGBDT_MM_PrintPutStr(inf, "false");
				break;
			case 3:
				i=BGBDT_MM_PrintPutStr(inf, "true");
				break;
			default:
				i=BGBDT_MM_PrintPutPrintf(inf, "__unknown_%d", val.lo);
				break;
			}
			break;
		}

		if((val.hi>>24)==0x10)
		{
			v=BGBDT_TagTy_DecodeRotLong(val);
//			i=BGBDT_MM_PrintPutInt(inf, v);
			i=BGBDT_MM_PrintPutIntHex64B(inf, v);
			break;
		}

		BGBDT_MM_PrintPutStr(inf, "#?");
		i=BGBDT_MM_PrintPutIntHex64(inf, val.vi);
		break;
	case BGBDT_HTAG_FIXLONG0:	case BGBDT_HTAG_FIXLONG1:
	case BGBDT_HTAG_FIXLONG2:	case BGBDT_HTAG_FIXLONG3:
		v=dtvUnwrapLongF(val);
		i=BGBDT_MM_PrintPutInt(inf, v);
		break;
	case BGBDT_HTAG_FIXREAL0:	case BGBDT_HTAG_FIXREAL1:
	case BGBDT_HTAG_FIXREAL2:	case BGBDT_HTAG_FIXREAL3:
		vf=dtvUnwrapDoubleF(val);
		i=BGBDT_MM_PrintPutFloat(inf, vf);
		break;
	case BGBDT_HTAG_TAGARR:
		BGBDT_MM_PrintTagArr(inf, val);
		break;
	default:
		BGBDT_MM_PrintPutStr(inf, "#?");
		i=BGBDT_MM_PrintPutIntHex64(inf, val.vi);
		break;
	}
	return(i);
}

BTEIFGL_API int BGBDT_MM_PrintValueToStrBuf(
	char *strbuf, int szbuf, dtVal val)
{
	BGBDT_MM_ParsePrintInfo *inf;
	int i;

	inf=BGBDT_MM_NewStringPrinter(strbuf, szbuf);
	BGBDT_MM_PrintValue(inf, val);
	i=inf->ct-strbuf;
	BGBDT_MM_FreeParsePrintInfo(inf);
	return(i);
}


BTEIFGL_API int BGBDT_MM_FormatPrintTabs(
	BGBDT_MM_ParsePrintInfo *inf, int ntab)
{
	char tb[256];
	char *t;
	int i;
	
	t=tb; i=ntab;
	while(i--)
		{ *t++=' '; *t++=' '; }
	*t++=0;
	i=BGBDT_MM_PrintPutStr(inf, tb);
	return(i);
}

int BGBDT_MM_FormatObjValue(BGBDT_MM_ParsePrintInfo *inf, dtVal val)
{
	char tb[256];
	int slis[256];
	s64 vi;
	double vf;
	dtVal v0;
	int tyid, ns, bt;
	char *tyn, *fn;
	int i, j, k;
	
	if(dtvNullP(val))
	{
		i=BGBDT_MM_PrintPutStr(inf, "null");
		return(i);
	}

	if(dtvIsArrayP(val))
	{
		i=BGBDT_MM_FormatTagArr(inf, val);
		return(i);
	}

	if(BGBDT_MapObj_IsObjectP(val))
	{
		i=BGBDT_MM_FormatMapObjValue(inf, val);
		return(i);
	}
	
	i=BGBDT_MM_PrintObjValue(inf, val);
	return(i);
}

int BGBDT_MM_FormatMapObjValue(BGBDT_MM_ParsePrintInfo *inf, dtVal val)
{
	char tb[256];
	int slis[256];
	s64 vi;
	double vf;
	dtVal v0;
	int tyid, ns, bt;
	char *tyn, *fn;
	int i, j, k;
	
	BGBDT_MM_PrintValueToStrBuf(tb, 252, val);
	if((2*inf->nrec+strlen(tb))<76)
	{
		i=BGBDT_MM_PrintPutStr(inf, tb);
		return(i);
	}
	
	for(i=0; i<inf->nrec; i++)
		if(dtvEqqP(val, inf->recarr[i]))
	{
		j=BGBDT_MM_PrintPutPrintf(inf, "#%d", i);
		return(j);
	}
	
	i=inf->nrec++;
	inf->recarr[i]=val;
	
	BGBDT_MM_PrintPutStr(inf, "{\n");
	ns=BGBDT_MapObj_QueryObjSlotList(val, slis, 256);
	for(i=0; i<ns; i++)
	{
		BGBDT_MM_FormatPrintTabs(inf, inf->nrec);
		fn=BGBDT_MapObj_GetFieldIndexName(slis[i]);
		sprintf(tb, "%s: ", fn);
		BGBDT_MM_PrintPutStr(inf, tb);
		
		v0=BGBDT_MapObj_GetObjvSlotValue(val, slis[i]);
		BGBDT_MM_FormatValue(inf, v0);

		if((i+1)<ns)
		{
			BGBDT_MM_PrintPutStr(inf, ",\n");
		}else
		{
			BGBDT_MM_PrintPutStr(inf, "\n");
		}
	}

	BGBDT_MM_FormatPrintTabs(inf, inf->nrec-1);
	i=BGBDT_MM_PrintPutStr(inf, "}");
	inf->nrec--;
	return(i);
}

int BGBDT_MM_FormatTagArr(BGBDT_MM_ParsePrintInfo *inf, dtVal val)
{
	char tb[256];
	s64 vi;
	double vf;
	dtVal v0;
	int tyid, ns, bt;
	char *tyn, *fn;
	int i, j, k, kp;
	
	BGBDT_MM_PrintValueToStrBuf(tb, 252, val);
	if((2*inf->nrec+strlen(tb))<76)
	{
		i=BGBDT_MM_PrintPutStr(inf, tb);
		return(i);
	}
	
	for(i=0; i<inf->nrec; i++)
		if(dtvEqqP(val, inf->recarr[i]))
	{
		j=BGBDT_MM_PrintPutPrintf(inf, "#%d", i);
		return(j);
	}
	
	i=inf->nrec++;
	inf->recarr[i]=val;
	
	BGBDT_MM_PrintPutStr(inf, "[\n");

	kp=inf->nchars;
	BGBDT_MM_FormatPrintTabs(inf, inf->nrec);

	bt=dtvArrayGetBaseType(val);
	ns=dtvArrayGetSize(val);
	for(i=0; i<ns; i++)
	{
		switch(bt)
		{
		case BGBDT_BASETY_INT:
			vi=dtvArrayGetIndexInt(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_LONG:
			vi=dtvArrayGetIndexLong(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_FLOAT:
			vf=dtvArrayGetIndexFloat(val, i);
			BGBDT_MM_PrintPutFloat(inf, vf);
			break;
		case BGBDT_BASETY_DOUBLE:
			vf=dtvArrayGetIndexDouble(val, i);
			BGBDT_MM_PrintPutFloat(inf, vf);
			break;
		case BGBDT_BASETY_ADDRESS:
			v0=dtvArrayGetIndexDtVal(val, i);

			BGBDT_MM_PrintValueToStrBuf(tb, 252, v0);
			if((((inf->nchars-kp)+strlen(tb))>=76) &&
				((inf->nchars-kp)>(2*inf->nrec)))
			{
				BGBDT_MM_PrintPutStr(inf, "\n");
				kp=inf->nchars;
				BGBDT_MM_FormatPrintTabs(inf, inf->nrec);
			}
			BGBDT_MM_FormatValue(inf, v0);
			break;
		case BGBDT_BASETY_UINT:
			vi=(u32)dtvArrayGetIndexInt(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_UBYTE:
			vi=dtvArrayGetIndexByte(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_SHORT:
			vi=dtvArrayGetIndexShort(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_SBYTE:
			vi=dtvArrayGetIndexSByte(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_USHORT:
			vi=dtvArrayGetIndexUShort(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		case BGBDT_BASETY_ULONG:
			vi=dtvArrayGetIndexLong(val, i);
			BGBDT_MM_PrintPutInt(inf, vi);
			break;
		default:
			i=BGBDT_MM_PrintPutStr(inf, "<?!>");
			break;
		}
		if((i+1)<ns)
		{
			BGBDT_MM_PrintPutStr(inf, ", ");
			if((inf->nchars-kp)>=76)
			{
				BGBDT_MM_PrintPutStr(inf, "\n");
				kp=inf->nchars;
				BGBDT_MM_FormatPrintTabs(inf, inf->nrec);
			}
		}
	}

	i=BGBDT_MM_PrintPutStr(inf, "]");
	inf->nrec--;
	return(i);
}

BTEIFGL_API int BGBDT_MM_FormatValue(
	BGBDT_MM_ParsePrintInfo *inf, dtVal val)
{
	int i;

	switch(val.hi>>28)
	{
	case BGBDT_HTAG_PTR:
		i=BGBDT_MM_FormatObjValue(inf, val);
		break;
	case BGBDT_HTAG_TAGARR:
		BGBDT_MM_FormatTagArr(inf, val);
		break;
	default:
		i=BGBDT_MM_PrintValue(inf, val);
		break;
	}
	return(i);
}

BTEIFGL_API int BGBDT_MM_FormatValueToStrBuf(
	char *strbuf, int szbuf, dtVal val)
{
	BGBDT_MM_ParsePrintInfo *inf;
	int i;

	inf=BGBDT_MM_NewStringPrinter(strbuf, szbuf);
	BGBDT_MM_FormatValue(inf, val);
	i=inf->ct-strbuf;
	BGBDT_MM_FreeParsePrintInfo(inf);
	return(i);
}
