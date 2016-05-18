#include <bteifgl.h>

char *BGBDT_TagParse_EatWhiteOnly(char *s)
{
	while(*s && (*s<=' '))
		s++;
	return(s);
}

char *BGBDT_TagParse_EatWhite(char *s)
{
	int i;
	while(*s && (*s<=' '))
		s++;
	if((s[0]=='/') && (s[1]=='/'))
	{
		s+=2;
		while(*s && (*s!='\r') && (*s!='\n'))
			s++;
		s=BGBDT_TagParse_EatWhite(s);
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
		s=BGBDT_TagParse_EatWhite(s);
		return(s);
	}
	return(s);
}

int BGBDT_TagParse_ParseHexN(char *s, int n)
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

char *BGBDT_TagParse_ReadUtf8(char *sb, int *rv)
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

char *BGBDT_TagParse_EmitUtf8(char *t, int v)
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

char *BGBDT_TagParse_ParseTokenBasic(char *s, char *tb)
{
	char *t, *te;
	int i, j, k;

	t=tb; te=tb+252;

	s=BGBDT_TagParse_EatWhite(s);
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
					*t++=BGBDT_TagParse_ParseHexN(s, 2);
					s+=2;
					break;
				case 'u':
					s++;
					i=BGBDT_TagParse_ParseHexN(s, 4);
					t=BGBDT_TagParse_EmitUtf8(t, i);
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
					*t++=BGBDT_TagParse_ParseHexN(s, 2);
					s+=2;
					break;
				case 'u':
					s++;
					i=BGBDT_TagParse_ParseHexN(s, 4);
					t=BGBDT_TagParse_EmitUtf8(t, i);
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

s64 BGBDT_TagParse_ParseInt(char *s)
{
	s64 v;
	int sg;
	int i, j, k;

	if(*s=='-')
		{ s++; sg=-1; }
	else { sg=1; }

	if(*s=='0')
	{
		if(s[1]=='x')
		{
			v=0; s+=2;
			while(*s)
			{
				i=*s++; j=0;
				if(i=='_')
					continue;
				if((i>='0') && (i<='9'))
					{ j=i-'0'; }
				if((i>='A') && (i<='F'))
					{ j=10+(i-'A'); }
				if((i>='a') && (i<='f'))
					{ j=10+(i-'a'); }
				v=(v<<4)|j;
			}
			return(v*sg);
		}

		if(s[1]=='b')
		{
			v=0; s+=2;
			while(*s)
			{
				i=*s++; j=0;
				if(i=='_')
					continue;
				if((i>='0') && (i<='1'))
					{ j=i-'0'; }
				v=(v<<1)|j;
			}
			return(v*sg);
		}

		if(s[1]=='c')
		{
			v=0; s+=2;
			while(*s)
			{
				i=*s++; j=0;
				if(i=='_')
					continue;
				if((i>='0') && (i<='7'))
					{ j=i-'0'; }
				v=(v<<3)|j;
			}
			return(v*sg);
		}

		v=0; s+=2;
		while(*s)
		{
			i=*s++; j=0;
			if(i=='_')
				continue;
			if((i>='0') && (i<='7'))
				{ j=i-'0'; }
			v=(v<<3)|j;
		}
		return(v*sg);
	}

	v=0;
	while(*s)
	{
		i=*s++; j=0;
		if(i=='_')
			continue;
		if((i>='0') && (i<='9'))
			{ j=i-'0'; }
		v=(v*10)+j;
	}
	return(v*sg);
}

double BGBDT_TagParse_ParseFloat(char *s)
{
#if 1
	double v, sc;
	int sg, se;
	int i, j, k;

	if(*s=='-')
		{ s++; sg=-1; }
	else { sg=1; }

	v=0;
	while(*s)
	{
		i=*s++; j=0;
		if(i=='_')
			continue;
		if(i=='.')
			{ s--; break; }
		if((i>='0') && (i<='9'))
			{ j=i-'0'; }
		v=(v*10)+j;
	}
	if(*s=='.')
	{
		s++; sc=0.1;

		while(*s)
		{
			i=*s++; j=0;
			if(i=='_')
				continue;
			if((i>='0') && (i<='9'))
				{ j=i-'0'; }
			else
				{ break; }
			v=v+(j*sc); sc=sc*0.1;
		}
		
		if((*s=='e') || (*s=='E'))
		{
			s++;
			if(*s=='+')s++;
			se=BGBDT_TagParse_ParseInt(s);
			if(se>0)
			{
				i=se;
				while(i--)v=v*10;
			}else if(se<0)
			{
				i=-se;
				while(i--)v=v*0.1;
			}
		}
	}
	return(v*sg);
#endif
//	return(atof(s));
}


dtVal BGBDT_TagParse_ParseValueArr(BGBDT_MM_ParsePrintInfo *inf)
{
	char tb1[256], tb2[256];
	dtVal tva[1024];
	dtVal tv, arr;
	char *sig, *tsig;
	char *s, *s1, *s2;
	int ntva, sfx;
	int i, j, k;

	s=inf->cs;

	s=BGBDT_TagParse_ParseTokenBasic(s, tb1);
	s1=BGBDT_TagParse_ParseTokenBasic(s, tb2);

	if(strcmp(tb1, "X["))
		{ return(DTV_UNDEFINED); }

	ntva=0;
	while(1)
	{
		s1=BGBDT_TagParse_ParseTokenBasic(s, tb1);
		if(!strcmp(tb1, "X]"))
			{ s=s1; break; }
		
		inf->cs=s;
		tv=BGBDT_TagParse_ParseValue(inf);
		tva[ntva++]=tv;
		s=inf->cs;

		s1=BGBDT_TagParse_ParseTokenBasic(s, tb2);
		if(!strcmp(tb2, "X,"))
			{ s=s1; }
	}

	s1=BGBDT_TagParse_ParseTokenBasic(s, tb2);
	sfx=-1;
	if(!strcmp(tb2, "II") || !strcmp(tb2, "Ii"))
		sfx=BGBDT_BASETY_INT;
	if(!strcmp(tb2, "IL") || !strcmp(tb2, "Il"))
		sfx=BGBDT_BASETY_LONG;
	if(!strcmp(tb2, "IF") || !strcmp(tb2, "If"))
		sfx=BGBDT_BASETY_FLOAT;
	if(!strcmp(tb2, "ID") || !strcmp(tb2, "Id"))
		sfx=BGBDT_BASETY_DOUBLE;
	if(!strcmp(tb2, "IUI") || !strcmp(tb2, "Iui"))
		sfx=BGBDT_BASETY_UINT;
	if(!strcmp(tb2, "IUL") || !strcmp(tb2, "Iul"))
		sfx=BGBDT_BASETY_ULONG;
	if(!strcmp(tb2, "IUB") || !strcmp(tb2, "Iub"))
		sfx=BGBDT_BASETY_UBYTE;
	if(!strcmp(tb2, "ISB") || !strcmp(tb2, "Isb"))
		sfx=BGBDT_BASETY_SBYTE;
	if(!strcmp(tb2, "IUS") || !strcmp(tb2, "Ius"))
		sfx=BGBDT_BASETY_USHORT;
	if(!strcmp(tb2, "IS") || !strcmp(tb2, "Is"))
		sfx=BGBDT_BASETY_SHORT;
	if(!strcmp(tb2, "ISS") || !strcmp(tb2, "Iss"))
		sfx=BGBDT_BASETY_SHORT;

	if(sfx>=0)
	{
		s=s1;
	}else
	{
		sfx=BGBDT_BASETY_ADDRESS;
	}
	
	arr=dtvNewArray(ntva, sfx);
	for(i=0; i<ntva; i++)
	{
		tv=tva[i];
		switch(sfx)
		{
		case BGBDT_BASETY_INT:
		case BGBDT_BASETY_UINT:
			dtvArraySetIndexInt(arr, i, dtvUnwrapInt(tv));
			break;
		case BGBDT_BASETY_LONG:
		case BGBDT_BASETY_ULONG:
			dtvArraySetIndexInt(arr, i, dtvUnwrapLong(tv));
			break;
		case BGBDT_BASETY_FLOAT:
			dtvArraySetIndexFloat(arr, i, dtvUnwrapFloat(tv));
			break;
		case BGBDT_BASETY_DOUBLE:
			dtvArraySetIndexDouble(arr, i, dtvUnwrapDouble(tv));
			break;
		case BGBDT_BASETY_UBYTE:
		case BGBDT_BASETY_SBYTE:
			dtvArraySetIndexByte(arr, i, dtvUnwrapInt(tv));
			break;
		case BGBDT_BASETY_USHORT:
		case BGBDT_BASETY_SHORT:
			dtvArraySetIndexShort(arr, i, dtvUnwrapInt(tv));
			break;
		default:
			dtvArraySetIndexDtVal(arr, i, tv);
			break;
		}
	}

	inf->cs=s;
	return(arr);
}

dtVal BGBDT_TagParse_ParseValue(BGBDT_MM_ParsePrintInfo *inf)
{
	char tb1[256], tb2[256];
	dtVal tv;
	s64 vi;
	double vf;
	char *s, *s1, *s2;

	s=inf->cs;

	s1=BGBDT_TagParse_ParseTokenBasic(s, tb1);
	s2=BGBDT_TagParse_ParseTokenBasic(s1, tb2);

	if(tb1[0]=='|')
	{
		s=s1;
		vi=BGBDT_TagParse_ParseInt(tb1+1);
		if(vi==((int)vi))
			{ tv=dtvWrapInt(vi); }
		else
			{ tv=dtvWrapLong(vi); }
		inf->cs=s;
		return(tv);
	}
		
	if(tb1[0]=='#')
	{
		s=s1;
		vf=BGBDT_TagParse_ParseFloat(tb1+1);
		tv=dtvWrapDouble(vf);
		inf->cs=s;
		return(tv);
	}

	if(tb1[0]=='S')
	{
		s=s1;
		tv=BGBDT_TagStr_String(tb1+1);
		inf->cs=s;
		return(tv);
	}

	if(!strcmp(tb1, "Itrue"))
	{
		s=s1;
		tv=DTV_TRUE;
		inf->cs=s;
		return(tv);
	}

	if(!strcmp(tb1, "Ifalse"))
	{
		s=s1;
		tv=DTV_FALSE;
		inf->cs=s;
		return(tv);
	}

	if(!strcmp(tb1, "Inull"))
	{
		s=s1;
		tv=DTV_NULL;
		inf->cs=s;
		return(tv);
	}

	if(!strcmp(tb1, "X-"))
	{
		if(tb2[0]=='|')
		{
			s=s2;
			vi=BGBDT_TagParse_ParseInt(tb2+1);
			vi=-vi;
			if(vi==((int)vi))
				{ tv=dtvWrapInt(vi); }
			else
				{ tv=dtvWrapLong(vi); }
			inf->cs=s;
			return(tv);
		}
		
		if(tb2[0]=='#')
		{
			s=s2;
			vf=BGBDT_TagParse_ParseFloat(tb2+1);
			tv=dtvWrapDouble(-vf);
			inf->cs=s;
			return(tv);
		}
	}

	if(!strcmp(tb1, "X{"))
	{
		inf->cs=s;
		tv=BGBDT_TagParse_ParseEntObj(inf);
		return(tv);
	}

	if(!strcmp(tb1, "X["))
	{
		inf->cs=s;
		tv=BGBDT_TagParse_ParseValueArr(inf);
		return(tv);
	}
	
	inf->cs=s;
	return(DTV_UNDEFINED);
}


dtVal BGBDT_TagParse_ParseEntObj(BGBDT_MM_ParsePrintInfo *inf)
{
	char tb1[256], tb2[256];
	char *afn[256];
	dtVal afv[256];
	dtcClass clz;
	dtcField fi;
	dtVal tval;
	dtVal obj;
	char *fn, *sig;
	char *s, *s1, *s2;
	int nfn;
	int i, j, k;
	
	s=inf->cs;
	s=BGBDT_TagParse_ParseTokenBasic(s, tb1);
	BGBDT_TagParse_ParseTokenBasic(s, tb2);
	
	if(strcmp(tb1, "X{"))
		return(DTV_UNDEFINED);
	
	nfn=0;
	while(1)
	{
		s=BGBDT_TagParse_ParseTokenBasic(s, tb1);
		s1=BGBDT_TagParse_ParseTokenBasic(s, tb2);

		if(!strcmp(tb1, "X}"))
			break;
		
		if((tb1[0]!='I') || strcmp(tb2, "X:"))
		{
			break;
		}
		
		fn=BGBDT_TagStr_StrSymbol(tb1+1);
		
		inf->cs=s1;
		tval=BGBDT_TagParse_ParseValue(inf);
		s=inf->cs;

		s1=BGBDT_TagParse_ParseTokenBasic(s, tb2);
		if(!strcmp(tb2, "X,"))
			{ s=s1; }

		i=nfn++;
		afn[i]=fn;
		afv[i]=tval;
	}

	s1=BGBDT_TagParse_ParseTokenBasic(s, tb1);
	s2=BGBDT_TagParse_ParseTokenBasic(s1, tb2);

	if(!strcmp(tb1, "X:"))
	{
		s=s2;
		
		clz=BGBDTC_LookupClassQName(tb2+1);
		if(clz)
		{
			obj=dtvWrapPtr(BGBDTC_AllocClassInstance(clz));
			for(i=0; i<nfn; i++)
			{
				fi=BGBDTC_LookupClassSlotName(clz, afn[i]);
				if(fi)
				{
					dtcVaSetA(obj, fi, afv[i]);
				}
			}
			inf->cs=s;
			return(obj);
		}
	}

	obj=BGBDT_MapObj_AllocObjv();
	for(i=0; i<nfn; i++)
	{
		BGBDT_MapObj_BindObjvSlotValueName(obj, afn[i], afv[i]);
	}

	inf->cs=s;
	return(obj);
}

BTEIFGL_API BGBDT_MM_ParsePrintInfo *BGBDT_TagParse_NewStringParser(
	char *strbuf, int szbuf)
{
	BGBDT_MM_ParsePrintInfo *tmp;

	tmp=BGBDT_MM_AllocParsePrintInfo();

	tmp->cs=strbuf;
	tmp->css=strbuf;
	tmp->cse=strbuf+szbuf;
//	tmp->putstr=bgbdt_mm_strprint_putstr;
	
	return(tmp);
}

BTEIFGL_API dtVal BGBDT_TagParse_ParseValueFromStrBuf(
	char *strbuf, int szbuf)
{
	BGBDT_MM_ParsePrintInfo *inf;
	dtVal val;
	int i;

	inf=BGBDT_TagParse_NewStringParser(strbuf, szbuf);
	val=BGBDT_TagParse_ParseValue(inf);
	BGBDT_MM_FreeParsePrintInfo(inf);
	return(val);
}
