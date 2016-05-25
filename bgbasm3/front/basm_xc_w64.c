/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <bgbasm.h>

int BASM_XCallSplitSigW64(char *sig, char **esig, int *rni, int *rns)
{
	char *s;
	int i, j, ni, ns;

	s=sig; i=0; ni=0; ns=0;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			if(ni<4) { ni++; break; }
			ns++;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(ni<4) { ni++; break; }
			ns++;
			break;

		case 'r':
			if(ni<4) { ni++; break; }
			ns++;
			break;

		case 'P':
			if(ni<4) ni++; else ns++;

			while(*s=='P')s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'd':
			if(ni<4) { ni++; break; }
			ns++;
			break;

		case 'e':
			if(ni<4) { ni++; break; }
			if(ns&1)ns++;
			ns+=2;
			break;

		case 'f': case 'k':
			if(ni<4) { ni++; break; }
			ns++;
			break;

		case 'g': case 'n': case 'o':
			if(ni<4) { ni++; break; }
			if(ns&1)ns++;
			ns+=2;
			break;

		case 'C':
			switch(*s++)
			{
			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k': i+=4; break;
			}
			break;
		case 'G':
			switch(*s++)
			{
			case 'f': i+=4; break;
			case 'd': i+=8; break;
			case 'e': i+=12; break;
			case 'g': i+=16; break;
			case 'k': i+=4; break;
			}
			break;

		default: break;
		}
	}

	if(*s && *s==')')s++;
	if(esig)*esig=s;

	if(rni)*rni=ni;
	if(rns)*rns=ns;

	return(i);
}

int BASM_XCallGenRepackSigW64(char *sig)
{
	static char *gregs[4]={"rcx", "rdx", "r8", "r9"};
	static char *xregs[4]={"xmm0", "xmm1", "xmm2", "xmm3"};

	char *s;
	int i, j, ni, ns;

	s=sig; i=0; j=0; ni=0; ns=0;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			if(ni<4)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++; break;
			}
			basm_print("mov eax, [rbp+%d]\n", (i+2)*8);
			basm_print("mov [rsp+%d], eax\n", ns*8+32);
			i++; ns++;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(ni<4)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++; break;
			}
			basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
			basm_print("mov [rsp+%d], rax\n", ns*8+32);
			i++; ns++;
			break;

		case 'r':
			if(ni<4)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++; break;
			}
			basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
			basm_print("mov [rsp+%d], rax\n", ns*8+32);
			i++; ns++;
			break;

		case 'P':
			if(ni<4)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
				basm_print("mov [rsp+%d], rax\n", ns*8+32);
				i++; ns++;
			}

			while(*s=='P')s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'Q':
			if(ni<4)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
				basm_print("mov [rsp+%d], rax\n", ns*8+32);
				i++; ns++;
			}

			while(*s=='Q')s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='L')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'L':
			if(ni<4)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
				basm_print("mov [rsp+%d], rax\n", ns*8+32);
				i++; ns++;
			}

			while(*s && *s!=';')s++;
			if(*s==';')s++;
			break;

		case 'd':
			if(ni<4)
			{
				basm_print("movsd %s, [rbp+%d]\n",
					xregs[ni], (i+2)*8);
				i++; ni++; break;
			}
			basm_print("movsd xmm4, [rbp+%d]\n", (i+2)*8);
			basm_print("movsd [rsp+%d], xmm4\n", ns*8+32);
			i++; ns++;
			break;

		case 'e':
			if(ni<4)
			{
				if(i&1)i++;
				basm_print("movdqa %s, [rbp+%d]\n",
					xregs[ni], (i+2)*8);
				i+=2; ni++; break;
			}
			if(i&1)i++;
			if(ns&1)ns++;
			basm_print("movdqa xmm4, [rbp+%d]\n", (i+2)*8);
			basm_print("movdqa [rsp+%d], xmm4\n", ns*8+32);
			i+=2; ns+=2;
			break;

		case 'f': case 'k':
			if(ni<4)
			{
				basm_print("movss %s, [rbp+%d]\n",
					xregs[ni], (i+2)*8);
				i++; ni++; break;
			}
			basm_print("movss xmm4, [rbp+%d]\n", (i+2)*8);
			basm_print("movss [rsp+%d], xmm4\n", ns*8+32);
			i++; ns++;
			break;

		case 'g': case 'n': case 'o':
			if(ni<4)
			{
				if(i&1)i++;
				basm_print("movdqa %s, [rbp+%d]\n",
					xregs[ni], (i+2)*8);
				i+=2; ni++; break;
			}
			if(i&1)i++;
			if(ns&1)ns++;
			basm_print("movdqa xmm4, [rbp+%d]\n", (i+2)*8);
			basm_print("movdqa [rsp+%d], xmm4\n", ns*8+32);
			i+=2; ns+=2;
			break;

		case 'C':
			switch(*s++)
			{
			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k': i+=4; break;
			}
			break;
		case 'G':
			switch(*s++)
			{
			case 'f': i+=4; break;
			case 'd': i+=8; break;
			case 'e': i+=12; break;
			case 'g': i+=16; break;
			case 'k': i+=4; break;
			}
			break;

		default: break;
		}
	}

	if(*s && *s==')')s++;

	return(i);
}

int BASM_XCallGenNatRepackSigW64(char *sig)
{
	static char *gregs[4]={"rcx", "rdx", "r8", "r9"};
	static char *xregs[4]={"xmm0", "xmm1", "xmm2", "xmm3"};

	char *s;
	int i, j, ni, ns, offs;

	offs=16+16+32-16;	//rip+rbp + rsi+rdi + rcx/rdx/r8/r9 (-bias)

	s=sig; i=0; j=0; ni=0; ns=0;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			if(ni<4)
			{
				basm_print("mov [rsp+%d], %s\n",
					i*8, gregs[ni]);
				i++; ni++; break;
			}
			basm_print("mov eax, [rbp+%d]\n", ns*8+offs);
			basm_print("mov [rsp+%d], eax\n", i*8);
			i++; ns++;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(ni<4)
			{
				basm_print("mov [rsp+%d], %s\n",
					i*8, gregs[ni]);
				i++; ni++; break;
			}
			basm_print("mov rax, [rbp+%d]\n", ns*8+offs);
			basm_print("mov [rsp+%d], rax\n", i*8);
			i++; ns++;
			break;

		case 'r':
			if(ni<4)
			{
				basm_print("mov [rsp+%d], %s\n",
					i*8, gregs[ni]);
				i++; ni++; break;
			}
			basm_print("mov rax, [rbp+%d]\n", ns*8+offs);
			basm_print("mov [rsp+%d], rax\n", i*8);
			i++; ns++;
			break;

		case 'P':
			if(ni<4)
			{
				basm_print("mov [rsp+%d], %s\n",
					i*8, gregs[ni]);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n",
					ns*8+offs);
				basm_print("mov [rsp+%d], rax\n", i*8);
				i++; ns++;
			}

			while(*s=='P')s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'Q':
			if(ni<4)
			{
				basm_print("mov [rsp+%d], %s\n",
					i*8, gregs[ni]);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n",
					ns*8+offs);
				basm_print("mov [rsp+%d], rax\n", i*8);
				i++; ns++;
			}

			while(*s=='Q')s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='L')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'L':
			if(ni<4)
			{
				basm_print("mov [rsp+%d], %s\n",
					i*8, gregs[ni]);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n",
					ns*8+offs);
				basm_print("mov [rsp+%d], rax\n", i*8);
				i++; ns++;
			}

			while(*s && *s!=';')s++;
			if(*s==';')s++;
			break;

		case 'd':
			if(ni<4)
			{
				basm_print("movsd [rsp+%d], %s\n",
					i*8, xregs[ni]);
				i++; ni++;
			}else
			{
				basm_print("movsd xmm4, [rbp+%d]\n",
					ns*8+offs);
				basm_print("movsd [rsp+%d], xmm4\n", i*8);
				i++; ns++;
			}
			break;

		case 'e':
			if(ni<4)
			{
				if(i&1)i++;
				basm_print("movdqa [rsp+%d], %s\n",
					i*8, xregs[ni]);
				i+=2; ni++;
			}else
			{
				if(i&1)i++; if(ns&1)ns++;
				basm_print("movdqa xmm4, [rbp+%d]\n",
					ns*8+offs);
				basm_print("movdqa [rsp+%d], xmm4\n", i*8);
				i+=2; ns+=2;
			}
			break;

		case 'f': case 'k':
			if(ni<4)
			{
				basm_print("movss [rsp+%d], %s\n",
					i*8, xregs[ni]);
				i++; ni++;
			}else
			{
				basm_print("movss xmm4, [rbp+%d]\n",
					ns*8+offs);
				basm_print("movss [rsp+%d], xmm4\n", i*8);
				i++; ns++;
			}
			break;

		case 'g': case 'n': case 'o':
			if(ni<4)
			{
				if(i&1)i++;
				basm_print("movdqa [rsp+%d], %s\n",
					i*8, xregs[ni]);
				i+=2; ni++;
			}else
			{
				if(i&1)i++; if(ns&1)ns++;
				basm_print("movdqa xmm4, [rbp+%d]\n",
					ns*8+offs);
				basm_print("movdqa [rsp+%d], xmm4\n", i*8);
				i+=2; ns+=2;
			}
			break;

#if 0
		case 'C':
			switch(*s++)
			{
			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k': i+=4; break;
			}
			break;
		case 'G':
			switch(*s++)
			{
			case 'f': i+=4; break;
			case 'd': i+=8; break;
			case 'e': i+=12; break;
			case 'g': i+=16; break;
			case 'k': i+=4; break;
			}
			break;
#endif

		default: break;
		}
	}

	if(*s && *s==')')s++;

	return(i);
}

void *BASM_XCallGenThunkW64(char *name)
{
	char tb[256], tbn[64], tbs[256];
	u64 uli;
	byte *tx;
	void *p;
	char *s, *t, *esig;
	int ni, ns;
	int i, j;

	//ok, unmangle and split the name
	BASM_UnmangleXCall(name+4, tb);
	s=tb;
	t=tbn; while(*s && (*s!='('))*t++=*s++;
	*t++=0;
	t=tbs; while(*s)*t++=*s++;
	*t++=0;

	if(!tbs[0])
	{
		//BAD: XCall requires sig...
		printf("BASM_XCallGenThunkW64: bad sig %s\n", name);
		return(NULL);
	}

	p=BASM_GetPtr(tbn);
	if(!p)
	{
		//OK, there is no native function...
		printf("BASM_XCallGenThunkW64: no func '%s'->'%s'\n",
			name, tbn);
		return(NULL);
	}

	BASM_XCallSplitSigW64(tbs, &esig, &ni, &ns);

	BASM_BeginAssembly(NULL);
	basm_print("[section .text]\n[bits 64]\n");
	basm_print("%s:\n", name);
	basm_print("push rdi\n");
	basm_print("push rsi\n");
	basm_print("push rbp\n");
	basm_print("lea rbp, [rsp+16]\n");

	i=(ns+1)/2;
	basm_print("sub rsp, %d\n", i*16+32);

	BASM_XCallGenRepackSigW64(tbs);

	basm_print("int 3\n");

	basm_print("mov rax, %s\n", tbn);
	basm_print("call rax\n");

//	basm_print("call %s\n", tbn);

	basm_print("lea rsp, [rbp-16]\n");
	basm_print("pop rbp\n");
	basm_print("pop rsi\n");
	basm_print("pop rdi\n");
	basm_print("ret\n");

//	p=BASM_EndAssemblyQuiet();
	p=BASM_EndAssemblyDebug();

	return(p);
}

int BASM_XCallCheckSigDirectW64(char *sig)
{
	char *s;
	int i, j, ni, nx;

	s=sig; i=0; ni=0; nx=0;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		if(ni>=4)break;
		if(nx)break;

		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			ni++; break;

		case 'l': case 'm':
		case 'x': case 'y':
			ni++; break;

		case 'r':
			ni++; break;

		case 'P':
			ni++;

			while(*s=='P')s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'd': case 'e':
		case 'f': case 'k':
		case 'g': case 'n': case 'o':
			nx++;
			break;

		case 'C':
			switch(*s++)
			{
			case 'f': case 'd': 
			case 'e': case 'g': 
			case 'k':
				nx++; break;
			default: nx++; break;
			}
			break;
		case 'G':
			switch(*s++)
			{
			case 'f': case 'd':
			case 'e': case 'g':
			case 'k':
				nx++; break;
			default: nx++; break;
			}
			break;

		default: nx++; break;
		}
	}

	if(nx)return(0);
	if(*s && (*s!=')'))return(0);
	return(1);
}


void *BASM_XCallGenNatThunkW64(char *name, char *xcname)
{
	char tb[256], tbn[64], tbs[256];
	u64 uli;
	byte *tx;
	void *p;
	char *s, *t, *esig;
	int ni, ns, ns2;
	int i, j;

	//ok, unmangle and split the name
	BASM_UnmangleXCall(xcname+4, tb);
	s=tb;
	t=tbn; while(*s && (*s!='('))*t++=*s++;
	*t++=0;
	t=tbs; while(*s)*t++=*s++;
	*t++=0;

	if(!tbs[0])
	{
		//BAD: XCall requires sig...
		return(NULL);
	}

	p=BLNK_FetchSym(xcname);
	printf("BASM_XCallGenNatThunkW64: Targ=%p\n", p);

	ns2=BASM_XCallSplitSigW64(tbs, &esig, &ni, &ns);

	BASM_BeginAssembly(NULL);
	basm_print("[section .text]\n[bits 64]\n");
	basm_print("%s:\n", name);
	basm_print("push rdi\n");
	basm_print("push rsi\n");
	basm_print("push rbp\n");
	basm_print("lea rbp, [rsp+16]\n");

	i=(ns2+1)/2;
	basm_print("sub rsp, %d\n", i*16);

	BASM_XCallGenNatRepackSigW64(tbs);

//	basm_print("call %s\n", xcname);
	basm_print("mov rax, %s\n", xcname);
	basm_print("call rax\n");

	basm_print("lea rsp, [rbp-16]\n");
	basm_print("pop rbp\n");
	basm_print("pop rsi\n");
	basm_print("pop rdi\n");
	basm_print("ret\n");

//	p=BASM_EndAssemblyQuiet();
	p=BASM_EndAssemblyDebug();

	return(p);
}
