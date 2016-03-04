char *BGBDT_BxON_EatWhite(char *s)
{
	while(*s && (*s<=' '))
		s++;
	return(s);
}

int BGBDT_BxON_ParseHexN(char *s, int n)
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

char *BGBDT_BxON_EmitUtf8(char *t, int v)
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
}

char *BGBDT_BxON_ParseTokenBasic(char *s, char *tb)
{
	char *t, *te;
	int i, j, k;

	t=tb; te=tb+252;

	s=BGBDT_BxON_EatWhite(s);
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
				case 'e': s++; *t++='\e'; break;
				case '\\': s++; *t++='\\'; break;
				case '\"': s++; *t++='\"'; break;
				case '\'': s++; *t++='\''; break;
				case 'x':
					s++;
					*t++=BGBDT_BxON_ParseHexN(s, 2);
					s+=2;
					break;
				case 'u':
					s++;
					i=BGBDT_BxON_ParseHexN(s, 4);
					t=BGBDT_BxON_EmitUtf8(t, i);
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
				case 'e': s++; *t++='\e'; break;
				case '\\': s++; *t++='\\'; break;
				case '\"': s++; *t++='\"'; break;
				case '\'': s++; *t++='\''; break;
				case 'x':
					s++;
					*t++=BGBDT_BxON_ParseHexN(s, 2);
					s+=2;
					break;
				case 'u':
					s++;
					i=BGBDT_BxON_ParseHexN(s, 4);
					t=BGBDT_BxON_EmitUtf8(t, i);
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
	case '%':	case '|':
	case '^':	case '&':
		*t++=*s++;
		if(*s=='=')
			*t++=*s++;
		*t++=0;
		break;

	case '<':
		*t++=*s++;
		if((*s=='=') || (*s=='<'))
			{ *t++=*s++; }
		*t++=0;
		break;
	case '>':
		*t++=*s++;
		if((*s=='=') || (*s=='>'))
			{ *t++=*s++; }
		*t++=0;
		break;
	
	case '~': case '?':
	case '@': case '#':
	case '\\':
	case ';': case ':':
	case ',': case '.':
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
