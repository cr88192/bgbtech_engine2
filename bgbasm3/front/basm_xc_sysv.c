/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <bgbasm.h>

int BASM_XCallSplitSigSysV(char *sig, char **esig, int *rni, int *rnx, int *rns)
{
	char *s;
	int i, j, ni, nx, ns;

	s=sig; i=0; ni=0; nx=0; ns=0;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			if(ni<6) { ni++; break; }
			ns++;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(ni<6) { ni++; break; }
			ns++;
			break;

		case 'r':
			if(ni<6) { ni++; break; }
			ns++;
			break;

		case 'P':
			if(ni<6) ni++; else ns++;

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
			if(nx<8) { nx++; break; }
			ns++;
			break;

		case 'e':
			if(nx<8) { nx++; break; }
			if(ns&1)ns++;
			ns+=2;
			break;

		case 'f': case 'k':
			if(nx<8) { nx++; break; }
			ns++;
			break;

		case 'g': case 'n': case 'o':
			if(nx<8) { nx++; break; }
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
	if(rnx)*rnx=nx;
	if(rns)*rns=ns;

	return(i);
}

int BASM_XCallGenRepackSigSysV(char *sig)
{
	static char *gregs[6]={"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
	static char *xregs[8]={
		"xmm0", "xmm1", "xmm2", "xmm3",
		"xmm4", "xmm5", "xmm6", "xmm7"};

	char *s;
	int i, j, ni, nx, ns;

	s=sig; i=0; j=0; ni=0; nx=0; ns=0;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			if(ni<6)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++; break;
			}
			basm_print("mov eax, [rbp+%d]\n", (i+2)*8);
			basm_print("mov [rsp+%d], eax\n", ns*8);
			i++; ns++;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(ni<6)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++; break;
			}
			basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
			basm_print("mov [rsp+%d], rax\n", ns*8);
			i++; ns++;
			break;

		case 'r':
			if(ni<6)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++; break;
			}
			basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
			basm_print("mov [rsp+%d], rax\n", ns*8);
			i++; ns++;
			break;

		case 'P':
			if(ni<6)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
				basm_print("mov [rsp+%d], rax\n", ns*8);
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
			if(ni<6)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
				basm_print("mov [rsp+%d], rax\n", ns*8);
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
			if(ni<6)
			{
				basm_print("mov %s, [rbp+%d]\n",
					gregs[ni], (i+2)*8);
				i++; ni++;
			}else
			{
				basm_print("mov rax, [rbp+%d]\n", (i+2)*8);
				basm_print("mov [rsp+%d], rax\n", ns*8);
				i++; ns++;
			}

			while(*s && *s!=';')s++;
			if(*s==';')s++;
			break;

		case 'd':
			if(nx<8)
			{
				basm_print("movsd %s, [rbp+%d]\n",
					xregs[nx], (i+2)*8);
				i++; nx++; break;
			}
			basm_print("movsd xmm8, [rbp+%d]\n", (i+2)*8);
			basm_print("movsd [rsp+%d], xmm8\n", ns*8);
			i++; ns++;
			break;

		case 'e':
			if(nx<8)
			{
				if(i&1)i++;
				basm_print("movdqa %s, [rbp+%d]\n",
					xregs[nx], (i+2)*8);
				i+=2; nx++; break;
			}
			if(i&1)i++;
			if(ns&1)ns++;
			basm_print("movdqa xmm8, [rbp+%d]\n", (i+2)*8);
			basm_print("movdqa [rsp+%d], xmm8\n", ns*8);
			i+=2; ns+=2;
			break;

		case 'f': case 'k':
			if(nx<8)
			{
				basm_print("movss %s, [rbp+%d]\n",
					xregs[nx], (i+2)*8);
				i++; nx++; break;
			}
			basm_print("movss xmm8, [rbp+%d]\n", (i+2)*8);
			basm_print("movss [rsp+%d], xmm8\n", ns*8);
			i++; ns++;
			break;

		case 'g': case 'n': case 'o':
			if(nx<8)
			{
				if(i&1)i++;
				basm_print("movdqa %s, [rbp+%d]\n",
					xregs[nx], (i+2)*8);
				i+=2; nx++; break;
			}
			if(i&1)i++;
			if(ns&1)ns++;
			basm_print("movdqa xmm8, [rbp+%d]\n", (i+2)*8);
			basm_print("movdqa [rsp+%d], xmm8\n", ns*8);
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

void *BASM_XCallGenThunkSysV(char *name)
{
	char tb[256], tbn[64], tbs[256];
	u64 uli;
	byte *tx;
	void *p;
	char *s, *t, *esig;
	int ni, nx, ns;
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
		return(NULL);
	}

	p=BASM_GetPtr(tbn);
	if(!p)
	{
		//OK, there is no native function...
		return(NULL);
	}

	BASM_XCallSplitSigSysV(tbs, &esig, &ni, &nx, &ns);

	BASM_BeginAssembly(NULL);
	basm_print("[section .text]\n[bits 64]\n");
	basm_print("%s:\n", name);
	basm_print("push rbp; mov rbp, rsp\n");

	i=(ns+1)/2;
	basm_print("sub rsp, %d\n", i*16);

	BASM_XCallGenRepackSigSysV(tbs);

	basm_print("call %s\n", tbn);

	basm_print("mov rsp, rbp\n");
	basm_print("pop rbp\n");
	basm_print("ret\n");

	p=BASM_EndAssemblyQuiet();

	return(p);
}
