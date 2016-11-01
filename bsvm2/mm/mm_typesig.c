/*
 * Stuff related to signature strings.
 */

// #include <bteifgl.h>

char *BGBDTC_SigNext(char *sig)
{
	char *s;
	
	s=sig;
	if((*s>='a') && (*s<='z'))
		return(s+1);

	if((*s=='C') || (*s=='D') || (*s=='R'))
	{
		return(BGBDTC_SigNext(s+1));
	}

	if(*s=='P')
	{
		while(*s=='P')s++;
		return(BGBDTC_SigNext(s));
	}
	if(*s=='Q')
	{
		while(*s=='Q')s++;
		return(BGBDTC_SigNext(s));
	}
	
	if((*s=='X') || (*s=='L') || (*s=='U'))
	{
		s++;
		if((*s>='0') && (*s<='9'))
		{
			while((*s>='0') && (*s<='9'))
				s++;
		}else
		{
			while(*s && (*s!=';'))
				s++;
			if(*s==';')
				s++;
		}
		return(s);
	}

	if(*s=='A')
	{
		s++;
		if((*s>='0') && (*s<='9'))
		{
			while((*s>='0') && (*s<='9'))
				s++;
			return(BGBDTC_SigNext(s));
		}else
		{
			return(BGBDTC_SigNext(s+1));
//			while(*s && (*s!=';'))
//				s++;
//			if(*s==';')
//				s++;
		}
//		return(BGBDTC_SigNext(s));
	}
	return(s);
}

char *BGBDTC_SigGetRet(char *sig)
{
	char *s;

	if(*sig=='(')
	{
		s=sig+1;

		while(*s && (*s!=')'))
			{ s=BGBDTC_SigNext(s); }
		if(*s==')')
			return(s+1);
		return(NULL);
	}
	return(NULL);
}

int BGBDTC_GetSigOpZ(char *sig)
{
	int i;

	switch(*sig)
	{
	case 'a':	case 'b':	case 'c': case 'h':
	case 'i':		case 'k':
	case 's':	case 't':	case 'w':
		i=BSVM2_OPZ_INT; break;
	case 'j':
		i=BSVM2_OPZ_UINT; break;
	case 'l':	case 'x':
		i=BSVM2_OPZ_LONG; break;
	case 'm':	case 'y':
		i=BSVM2_OPZ_ULONG; break;
	case 'f':
		i=BSVM2_OPZ_FLOAT; break;
	case 'd':	case 'e':
		i=BSVM2_OPZ_DOUBLE; break;

	case 'g':	case 'n':	case 'o':
	case 'p':	case 'q':	case 'r':
		i=BSVM2_OPZ_ADDRESS; break;

	case 'C':
//		if(sig[1]=='s')
//			{ i=-1; break; }
		i=BSVM2_OPZ_ADDRESS; break;

	case 'Q':
	case 'X':	case 'L':
		i=BSVM2_OPZ_ADDRESS; break;

	case 'v':
		i=BSVM2_OPZ_VOID; break;
	case 'z':
		i=BSVM2_OPZ_ADDR; break;

	case 'P':
		i=BSVM2_OPZ_ADDR; break;
//		i=5; break;

	case 'A':
		i=BSVM2_OPZ_ADDR; break;

	default:
		BSVM2_DBGTRAP
		i=-1; break;
	}
	
	return(i);
}

int BGBDTC_GetSigBType(char *sig)
{
	int i;

	switch(*sig)
	{
	case 'a':	case 'b':	case 'c': case 'h':
	case 'i':	case 'j':	case 'k':
	case 's':	case 't':	case 'w':
		i=0; break;
	case 'l':	case 'm':
	case 'x':	case 'y':
		i=1; break;
	case 'f':	i=2; break;
	case 'd':	case 'e':
		i=3; break;

	case 'g':	case 'n':	case 'o':
	case 'p':	case 'q':	case 'r':
		i=4; break;

	case 'C':
//		if(sig[1]=='s')
//			{ i=-1; break; }
		i=4; break;

	case 'Q':
	case 'X':	case 'L':
		i=4; break;

	case 'v':
		i=0; break;
	case 'z':
		i=-1; break;

	case 'P':
		i=-1; break;
//		i=5; break;

	default:
		i=-1; break;
	}
	
	return(i);
}


int BGBDTC_GetSigSizeAlign(char *sig, int *rsz, int *ral)
{
	char *s;
	int sz, al, sz2, al2, n;
	int i;

	switch(*sig)
	{
	case 'a':	case 'b':	case 'c': case 'h':
		sz=1; al=1; break;
	case 'i':	case 'j':
		sz=4; al=4; break;
	case 'k':
		sz=2; al=2; break;
	case 's':	case 't':	case 'w':
		sz=2; al=2; break;

	case 'l':	case 'm':
	case 'x':	case 'y':
		sz=8; al=8; break;
	case 'f':
		sz=4; al=4; break;
	case 'd':	case 'e':
		sz=8; al=8; break;

	case 'g':	case 'n':	case 'o':
		sz=16; al=16; break;
	case 'p':	case 'q':	case 'r':
		sz=8; al=8; break;

	case 'C':
		switch(sig[1])
		{
		case 'a': case 'f':
			sz=8; al=8; break;
		case 'b': case 'c':
		case 'd': case 'e':
		case 'q':
			sz=16; al=16; break;
		case 's':
		case 't':
			sz=8; al=8; break;
		default:
			sz=-1; al=0; break;
		}
		break;

	case 'Q':
	case 'X':	case 'L':
		sz=8; al=8; break;

	case 'v':
		i=0; break;
	case 'z':
		i=-1; break;

	case 'P':
		sz=sizeof(void *);
		al=sz;
		break;
	
	case 'A':
		s=sig+1;

		n=1;
		while((*s>='0') && (*s<='9'))
		{
			n=n*atoi(s);
			while((*s>='0') && (*s<='9'))
				s++;
			if(*s==',')s++;
			if(*s==';')
				{ s++; break; }
		}
		BGBDTC_GetSigSizeAlign(s, &sz2, &al2);
		al=al2;
		sz=n*sz2;
		break;

	default:
		BSVM2_DBGTRAP
		sz=-1;
		al=0;
		break;
	}
	
	if(rsz)*rsz=sz;
	if(ral)*ral=al;
	return(0);
}
