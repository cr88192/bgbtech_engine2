// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdarg.h>
// #include <bsvm2_bsvm21.h>

#if defined(X86) || defined(X86_64)

#ifdef X86

#define SZ_PTR	4

#ifndef X86_32
#define X86_32
#endif

char *basm_callsig_asm=
"basm_callsig_basic:\n"
"push ebp; mov ebp, esp\n"
"push esi; push edi\n"
"mov ecx, [ebp+16]; sub esp, ecx\n"
"mov esi, [ebp+12]; mov edi, esp\n"
"rep; movsb; call dword [ebp+8]\n"
"add esp, [ebp+16]\n"
"pop edi; pop esi\n"
"pop ebp; ret\n"
""
"basm_callsig_struct:\n"
"push ebp; mov ebp, esp\n"
"push esi; push edi\n"
"mov ecx, [ebp+16]; sub esp, ecx\n"
"mov esi, [ebp+12]; mov edi, esp\n"
"rep; movsb; push dword [ebp+20]\n"
"call dword [ebp+8]\n"
"pop eax\n"
"add esp, [ebp+16]\n"
"pop edi; pop esi\n"
"pop ebp; ret\n"

"basm_callsig_std_basic:\n"
"push ebp; mov ebp, esp\n"
"push esi; push edi\n"
"mov ecx, [ebp+16]; sub esp, ecx\n"
"mov esi, [ebp+12]; mov edi, esp\n"
"rep; movsb; call dword [ebp+8]\n"
"pop edi; pop esi\n"
"pop ebp; ret\n"
""
"basm_callsig_std_struct:\n"
"push ebp; mov ebp, esp\n"
"push esi; push edi\n"
"mov ecx, [ebp+16]; sub esp, ecx\n"
"mov esi, [ebp+12]; mov edi, esp\n"
"rep; movsb; push dword [ebp+20]\n"
"call dword [ebp+8]\n"
"mov eax, [ebp+20]\n"
"pop edi; pop esi\n"
"pop ebp; ret\n"


"basm_callsig_closestub_start:\n"
"push ebp; mov ebp, esp\n"
"getip eax\n"
"lea ecx, [eax+$basm_callsig_closestub_data]\n"
"push ecx\n"
"lea ecx, [ebp+8]; push ecx\n"
"mov ecx, BASM_CreateStub_Inner32; call ecx\n"
"mov esp, ebp; pop ebp; ret\n"
"basm_callsig_closestub_data resb 32\n"
"basm_callsig_closestub_end:\n"
"";

static void (*basm_callsig_void_f)(void *fcn, void *buf, int sz);
static int (*basm_callsig_int_f)(void *fcn, void *buf, int sz);
static void *(*basm_callsig_ptr_f)(void *fcn, void *buf, int sz);
static double (*basm_callsig_dbl_f)(void *fcn, void *buf, int sz);
static s64 (*basm_callsig_s64_f)(void *fcn, void *buf, int sz);

static void (*basm_callsig_str_f)(void *fcn, void *buf, int sz, void *ret);


static void (*basm_callsig_stdvoid_f)(void *fcn, void *buf, int sz);
static int (*basm_callsig_stdint_f)(void *fcn, void *buf, int sz);
static void *(*basm_callsig_stdptr_f)(void *fcn, void *buf, int sz);
static double (*basm_callsig_stddbl_f)(void *fcn, void *buf, int sz);
static s64 (*basm_callsig_stds64_f)(void *fcn, void *buf, int sz);

static void (*basm_callsig_stdstr_f)(void *fcn, void *buf, int sz, void *ret);

#endif



#ifdef X86_64

#define SZ_PTR	8

#ifdef WIN64

#if 1
char *basm_callsig_asm=
"basm_callsig_basic:\n"
"push rbp\n"
"mov rbp, rsp\n"

"mov [rbp+16], rcx\n"	//fcn
"mov [rbp+24], rdx\n"	//buf
"mov [rbp+32], r8\n"	//sz
"mov [rbp+40], r9\n"	//gregs
			//xregs [rbp+48]

"mov rcx, [rbp+32]; sub rsp, rcx\n"
"mov rsi, [rbp+24]; mov rdi, rsp\n"
"rep; movsb\n"

"mov rax, [rbp+40]\n"
"mov rcx, [rax+0]\n"
"mov rdx, [rax+8]\n"
"mov r8, [rax+16]\n"
"mov r9, [rax+24]\n"

"mov rax, [rbp+48]\n"
"movaps xmm0, [rax+0]\n"
"movaps xmm1, [rax+16]\n"
"movaps xmm2, [rax+32]\n"
"movaps xmm3, [rax+48]\n"

"sub rsp, 32\n"
"mov rax, [rbp+16]\n"
"call rax\n"
"mov rsp, rbp\n"
"pop rbp; ret\n"

"align 16\n"
"basm_callsig_closestub_start:\n"
"push rbp\n"
"lea rbp, [rsp]\n"

"sub rsp, 96\n"
"mov [rsp+0], rcx\n"
"mov [rsp+8], rdx\n"
"mov [rsp+16], r8\n"
"mov [rsp+24], r9\n"

"movaps [rsp+32], xmm0\n"
"movaps [rsp+48], xmm1\n"
"movaps [rsp+64], xmm2\n"
"movaps [rsp+80], xmm3\n"

"lea r9, [rsp+32]\n"
"mov r8, rsp\n"
"lea rdx, [basm_callsig_closestub_fcn]\n"
"lea rcx, [rbp+48]\n"
"mov rax, BASM_CreateStub_Inner64; call rax\n"
"lea rsp, [rbp]\n"
"pop rbp; ret\n"
"align 16\n"
"basm_callsig_closestub_data resb 64\n"
"basm_callsig_closestub_end:\n"
"";
#endif

#else

#define AMD64

char *basm_callsig_asm=
"basm_callsig_basic:\n"

// "ret\n"

"push rbp; mov rbp, rsp\n"

"sub rsp, 48\n"

"mov [rbp-8], rdi\n"	//void *fcn
"mov [rbp-16], rsi\n"	//void *buf
"mov [rbp-24], rdx\n"	//int sz
"mov [rbp-32], rcx\n"	//void *gregs
"mov [rbp-40], r8\n"	//void *xregs

"mov rcx, [rbp-24]; sub rsp, rcx\n"
"mov rsi, [rbp-16]; mov rdi, rsp\n"
"rep; movsb\n"

//"mov rcx, [rbp-24]; sub rsp, rcx\n"
//"mov rsi, [rbp-16]; mov rdi, rsp\n"
//"shr rcx, 3\n"
//"rep; movsq\n"

//".l0:\n"
//"cmp rcx, 0; jle .l1\n"
//"movdqu xmm0, [rsi]\n"
//"movdqa [rsi], xmm0\n"
//"add rsi, 16; add rdi, 16; sub rcx, 16\n"
//"jmp .l0\n"
//".l1:"


"mov rax, [rbp-32]\n"

"mov rdi, [rax+0]\n"
"mov rsi, [rax+8]\n"
"mov rdx, [rax+16]\n"
"mov rcx, [rax+24]\n"
"mov r8, [rax+32]\n"
"mov r9, [rax+40]\n"

"mov rax, [rbp-40]\n"
"movaps xmm0, [rax+0]\n"
"movaps xmm1, [rax+16]\n"
"movaps xmm2, [rax+32]\n"
"movaps xmm3, [rax+48]\n"
"movaps xmm4, [rax+64]\n"
"movaps xmm5, [rax+80]\n"
"movaps xmm6, [rax+96]\n"
"movaps xmm7, [rax+112]\n"

"mov rax, 8\n"
"mov r10, [rbp-8]\n"
"call r10\n"

// "add rsp, [rbp-24]\n"
// "add rsp, 48\n"
"mov rsp, rbp\n"
"pop rbp; ret\n"


"align 16\n"
"basm_callsig_closestub_start:\n"
"push rbp; mov rbp, rsp\n"

"sub rsp, 176\n"
"mov [rsp+ 0], rdi\n"
"mov [rsp+ 8], rsi\n"
"mov [rsp+16], rdx\n"
"mov [rsp+24], rcx\n"
"mov [rsp+32], r8\n"
"mov [rsp+40], r9\n"

"movaps [rsp+ 48], xmm0\n"
"movaps [rsp+ 64], xmm1\n"
"movaps [rsp+ 80], xmm2\n"
"movaps [rsp+ 96], xmm3\n"
"movaps [rsp+112], xmm4\n"
"movaps [rsp+128], xmm5\n"
"movaps [rsp+144], xmm6\n"
"movaps [rsp+160], xmm7\n"

"lea rdx, [rsp+48]\n"
"mov rcx, rsp\n"
"lea rsi, [basm_callsig_closestub_fcn]\n"
"lea rdi, [rbp+48]\n"
"mov rax, BASM_CreateStub_Inner64; call rax\n"
"mov rsp, rbp; pop rbp; ret\n"
"align 16\n"
"basm_callsig_closestub_data resb 64\n"
"basm_callsig_closestub_end:\n"
"";

#endif

static void (*basm_callsig_void_f)(void *fcn, void *buf, int sz,
	void *gregs, void *xregs);
static int (*basm_callsig_int_f)(void *fcn, void *buf, int sz,
	void *gregs, void *xregs);
static void *(*basm_callsig_ptr_f)(void *fcn, void *buf, int sz,
	void *gregs, void *xregs);
static double (*basm_callsig_dbl_f)(void *fcn, void *buf, int sz,
	void *gregs, void *xregs);
static s64 (*basm_callsig_s64_f)(void *fcn, void *buf, int sz,
	void *gregs, void *xregs);

static void (*basm_callsig_str_f)(void *fcn, void *buf, int sz, void *ret,
	void *gregs, void *xregs);

#endif

#ifdef ARM

#define SZ_PTR	4

char *basm_callsig_asm=
"basm_callsig_basic:\n"
"b lr\n"

"basm_callsig_struct:\n"
"b lr\n"

"basm_callsig_closestub_start:\n"
"b lr\n"

"";

static void (*basm_callsig_void_f)(void *fcn, void *buf, int sz);
static int (*basm_callsig_int_f)(void *fcn, void *buf, int sz);
static void *(*basm_callsig_ptr_f)(void *fcn, void *buf, int sz);
static double (*basm_callsig_dbl_f)(void *fcn, void *buf, int sz);
static s64 (*basm_callsig_s64_f)(void *fcn, void *buf, int sz);

static void (*basm_callsig_str_f)(void *fcn, void *buf, int sz, void *ret);

#endif


#ifdef X86_32
int BASM_LenSig(char *sig, char **esig)
{
	char *s;
	int i;

	s=sig; i=0;
	if(*s=='(')s++;
	while(*s && (*s!=':') && (*s!=')'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
		case 'l': case 'm':
			i+=4; break;

		case 'd': i+=8; break;
		case 'e': i+=12; break;
		case 'f': i+=4; break;
		case 'g': i+=16; break;
		case 'k': i+=4; break;

		case 'n': i+=16; break;
		case 'o': i+=16; break;

		case 'r': i+=4; break;

		case 'x': i+=8; break;
		case 'y': i+=8; break;

		case 'C':
			switch(*s++)
			{
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
				i+=4; break;

			case 'a': i+=8; break;
			case 'b': i+=16; break;
			case 'c': i+=16; break;
			case 'q': i+=16; break;
			
			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k': i+=4; break;

			case 'r': i+=8; break;

			case 'x': i+=4; break;
			case 'y': i+=4; break;
			case 'z': i+=4; break;
			}
			break;

		case 'D':
			switch(*s++)
			{
			case 'a': i+=16; break;
			case 'b': i+=32; break;
			case 'c': i+=32; break;
			case 'q': i+=32; break;
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

		case 'P':
			i+=4;
			while(*s=='P')s++;
			if((*s=='C') || (*s=='G') || (*s=='D'))s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'L':
			i+=4;
			while(*s && *s!=';')s++;
			if(*s==';')s++;
			break;

		case 'Q':
			i+=4;
			while(*s=='Q')s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='L')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		default: break;
		}
	}
	if((*s==':') || (*s==')'))s++;
	if(esig)*esig=s;

	return(i);
}
#endif

#ifdef X86_64
int BASM_LenSig(char *sig, char **esig)
{
	char *s;
	int i;

	s=sig; i=0;
	if(*s=='(')s++;
	while(*s && (*s!=':') && (*s!=')'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
		case 'l': case 'm':
			i+=8; break;

		case 'd': i+=8; break;
		case 'e': if(i&15)i+=16-(i&15); i+=16; break;
		case 'f': i+=8; break;
		case 'g': if(i&15)i+=16-(i&15); i+=16; break;
		case 'k': i+=8; break;

		case 'n': if(i&15)i+=16-(i&15); i+=16; break;
		case 'o': if(i&15)i+=16-(i&15); i+=16; break;

		case 'r': i+=8; break;

		case 'x': i+=8; break;
		case 'y': i+=8; break;

		case 'C':
			switch(*s++)
			{
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
				i+=8; break;

			case 'a': i+=8; break;
			case 'b': i+=16; break;
			case 'c': i+=16; break;
			case 'q': i+=16; break;

			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': if(i&15)i+=16-(i&15); i+=32; break;
			case 'g': if(i&15)i+=16-(i&15); i+=32; break;
			case 'k': i+=8; break;
			case 'r': i+=8; break;

			case 'x': i+=8; break;
			case 'y': i+=8; break;
			case 'z': i+=8; break;
			}
			break;

		case 'D':
			switch(*s++)
			{
			case 'a': i+=16; break;
			case 'b': i+=32; break;
			case 'c': i+=32; break;
			case 'q': i+=32; break;
			}
			break;

		case 'G':
			switch(*s++)
			{
			case 'f': i+=8; break;
			case 'd': i+=8; break;
			case 'e': if(i&15)i+=16-(i&15); i+=16; break;
			case 'g': if(i&15)i+=16-(i&15); i+=16; break;
			case 'k': i+=8; break;
			}
			break;

		case 'P':
			i+=8;
			while(*s=='P')s++;
			if((*s=='C') || (*s=='G') || (*s=='D'))s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'L':
			i+=8;
			while(*s && *s!=';')s++;
			if(*s==';')s++;
			break;

		case 'Q':
			i+=8;
			while(*s=='Q')s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='L')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		default: break;
		}
	}

	if(i&15)i+=16-(i&15); 

	if(*s && ((*s==':')||(*s==')')))s++;
	if(esig)*esig=s;

	return(i);
}
#endif

#ifdef X86_32
BASM_API void BASM_CallSig(void *fcn, char *sig, void *buf, void *ret)
{
	char *esig;
	int i;

	BASM_InitSig();

	i=BASM_LenSig(sig, &esig);
	i=(i+31)&(~15);

//	printf("BASM_CallSig: %p %s %s\n", fcn, sig, esig);

	switch(*esig)
	{
	case 0: case 'v':
		basm_callsig_void_f(fcn, buf, i);
		break;

	case 'a': case 'b': case 'c': case 'h':
	case 's': case 't': case 'w':
	case 'i': case 'j': case 'l': case 'm':
		*(int *)ret=basm_callsig_int_f(fcn, buf, i); break;

	case 'd': *(double *)ret=basm_callsig_dbl_f(fcn, buf, i); break;
	case 'e': *(long double *)ret=basm_callsig_dbl_f(fcn, buf, i); break;
	case 'f': *(float *)ret=basm_callsig_dbl_f(fcn, buf, i); break;
	case 'k': *(float *)ret=basm_callsig_dbl_f(fcn, buf, i); break;

//	case 'g':
//		*(double *)ret=basm_callsig_dbl_f(fcn, buf, i);
//		break;
//	case 'n': i+=16; break;
//	case 'o': i+=16; break;

	case 'r': case 'P': case 'L': case 'Q':
		*(void **)ret=basm_callsig_ptr_f(fcn, buf, i); break;
	case 'x': case 'y':
		*(s64 *)ret=basm_callsig_s64_f(fcn, buf, i); break;

	case 'X':
		basm_callsig_str_f(fcn, buf, i, ret); break;

	case 'C':
		switch(*esig++)
		{
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':
			*(void **)ret=basm_callsig_ptr_f(fcn, buf, i); break;

		case 'a': case 'b': case 'c': case 'q':
		case 'f': case 'd': case 'e': case 'g': case 'k':
			basm_callsig_str_f(fcn, buf, i, ret); break;
		case 'r':
			*(s64 *)ret=basm_callsig_s64_f(fcn, buf, i); break;
		default: break;
		}
		break;

	case 'D':
		switch(*esig++)
		{
		case 'a': case 'b': case 'c': case 'q':
		case 'f': case 'd': case 'e': case 'g': case 'k':
			basm_callsig_str_f(fcn, buf, i, ret); break;
		default: break;
		}
		break;

	case 'G':
		switch(*esig++)
		{
		case 'f': i+=4; break;
		case 'd': i+=8; break;
		case 'e': i+=12; break;
		case 'g': i+=16; break;
		case 'k': i+=4; break;
		}
		break;

	default:
		basm_callsig_void_f(fcn, buf, i);
		break;
	}
}

BASM_API void BASM_CallSigStd(void *fcn, char *sig, void *buf, void *ret)
{
	char *esig;
	int i;

	BASM_InitSig();

	i=BASM_LenSig(sig, &esig);
//	i=(i+31)&(~15);
//	i=(i+3)&(~3);

	switch(*esig)
	{
	case 0: case 'v':
		basm_callsig_stdvoid_f(fcn, buf, i);
		break;

	case 'a': case 'b': case 'c': case 'h':
	case 's': case 't': case 'w':
	case 'i': case 'j': case 'l': case 'm':
		*(int *)ret=basm_callsig_stdint_f(fcn, buf, i); break;

	case 'd': *(double *)ret=basm_callsig_stddbl_f(fcn, buf, i); break;
	case 'e': *(long double *)ret=basm_callsig_stddbl_f(fcn, buf, i); break;
	case 'f': *(float *)ret=basm_callsig_stddbl_f(fcn, buf, i); break;
	case 'k': *(float *)ret=basm_callsig_stddbl_f(fcn, buf, i); break;

	case 'r': case 'P': case 'L': case 'Q':
		*(void **)ret=basm_callsig_stdptr_f(fcn, buf, i); break;
	case 'x': case 'y':
		*(s64 *)ret=basm_callsig_stds64_f(fcn, buf, i); break;

	case 'X':
		basm_callsig_stdstr_f(fcn, buf, i, ret); break;

	case 'C':
		switch(*esig++)
		{
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':
			*(void **)ret=basm_callsig_stdptr_f(fcn, buf, i); break;
		case 'a': case 'b': case 'c': case 'q':
		case 'f': case 'd': case 'e': case 'g': case 'k':
			basm_callsig_str_f(fcn, buf, i, ret); break;
		case 'r':
			*(s64 *)ret=basm_callsig_s64_f(fcn, buf, i); break;
		default: break;
		}
		break;

	case 'D':
		switch(*esig++)
		{
		case 'a': case 'b': case 'c': case 'q':
		case 'f': case 'd': case 'e': case 'g': case 'k':
			basm_callsig_stdstr_f(fcn, buf, i, ret); break;
		default: break;
		}
		break;

	default:
		basm_callsig_stdvoid_f(fcn, buf, i);
		break;
	}
}

#endif

#ifdef AMD64
int BASM_SplitArgsSig(char *sig, char **esig,
	byte *buf, byte *abuf, byte *greg, byte *xreg)
{
	char *s;
	byte *sb;
	int i, j, ni, nx;

	sb=buf;
	s=sig; i=0; ni=0; nx=0;
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
				*(int *)greg=*(int *)buf;
				greg+=8; buf+=8; ni++;
				break;
			}
			*(int *)abuf=*(int *)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(ni<6)
			{
				*(s64 *)greg=*(s64 *)buf;
				greg+=8; buf+=8; ni++;
				break;
			}
			*(s64 *)abuf=*(s64 *)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'r':
			if(ni<6)
			{
				*(void **)greg=*(void **)buf;
				greg+=8; buf+=8; ni++;
				break;
			}
			*(void **)abuf=*(void **)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'P':
			if(ni<6)
			{
				*(void **)greg=*(void **)buf;
				greg+=8; buf+=8; ni++;
			}else
			{
				*(void **)abuf=*(void **)buf;
				abuf+=8; buf+=8; i+=8;
			}

			while(*s=='P')s++;
			if((*s=='C') || (*s=='G') || (*s=='D'))s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'd':
			if(nx<8)
			{
				*(double *)xreg=*(double *)buf;
				xreg+=16; buf+=8; nx++;
				break;
			}
			*(double *)abuf=*(double *)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'e':
			if((buf-sb)&15) { buf+=16-((buf-sb)&15); }
			if(nx<8)
			{
				*(long double *)xreg=*(long double *)buf;
				xreg+=16; buf+=16; nx++;
				break;
			}

			if(i&15) { abuf+=16-(i&15); i=(i+15)&(~15); }
			*(long double *)abuf=*(long double *)buf;
			abuf+=16; buf+=16; i+=16;
			break;

		case 'f': case 'k':
			if(nx<8)
			{
				*(float *)xreg=*(float *)buf;
				xreg+=16; buf+=8; nx++;
				break;
			}
			*(float *)abuf=*(float *)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'g': case 'n': case 'o':
			if((buf-sb)&15) { buf+=16-((buf-sb)&15); }
			if(nx<8)
			{
				((s64 *)xreg)[0]=((s64 *)buf)[0];
				((s64 *)xreg)[1]=((s64 *)buf)[1];
				xreg+=16; buf+=16; nx++;
				break;
			}
			if(i&15) { abuf+=16-(i&15); i=(i+15)&(~15); }
			((s64 *)abuf)[0]=((s64 *)buf)[0];
			((s64 *)abuf)[1]=((s64 *)buf)[1];
			abuf+=16; buf+=16; i+=16;
			break;

		case 'C':
			switch(*s++)
			{
			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k': i+=4; break;

			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
			case 'x': case 'y': case 'z':
			case 'r':
				if(ni<6)
				{
					*(s64 *)greg=*(s64 *)buf;
					greg+=8; buf+=8; ni++;
					break;
				}
				*(s64 *)abuf=*(s64 *)buf;
				abuf+=8; buf+=8; i+=8;
				break;
			default: break;
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

	return(i);
}
#endif

#ifdef WIN64
int BASM_SplitArgsSig(char *sig, char **esig,
	byte *buf, byte *abuf, byte *greg, byte *xreg)
{
	char *s;
	int i, j, ni, nx;

	s=sig; i=0; j=0;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			if(j<4)
			{
				*(int *)greg=*(int *)buf;
				*(int *)xreg=*(int *)buf;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(int *)abuf=*(int *)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(j<4)
			{
				*(s64 *)greg=*(s64 *)buf;
				*(s64 *)xreg=*(s64 *)buf;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(s64 *)abuf=*(s64 *)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'r':
			if(j<4)
			{
				*(void **)greg=*(void **)buf;
				*(void **)xreg=*(void **)buf;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(void **)abuf=*(void **)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'P':
			if(j<4)
			{
				*(void **)greg=*(void **)buf;
				*(void **)xreg=*(void **)buf;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}else
			{
				*(void **)abuf=*(void **)buf;
				abuf+=8; buf+=8; i+=8;
			}

			while(*s=='P')s++;
			if((*s=='C') || (*s=='G') || (*s=='D'))s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'd':
			if(j<4)
			{
				*(s64 *)greg=*(double *)buf;
				*(double *)xreg=*(double *)buf;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(double *)abuf=*(double *)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'e':
			if(j<4)
			{
				*(s64 *)greg=*(long double *)buf;
				*(long double *)xreg=*(long double *)buf;
				greg+=8; xreg+=16; buf+=16; j++;
				break;
			}

			*(long double *)abuf=*(long double *)buf;
			abuf+=16; buf+=16; i+=16;
			break;

		case 'f': case 'k':
			if(j<4)
			{
				*(s64 *)greg=*(float *)buf;
				*(float *)xreg=*(float *)buf;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(float *)abuf=*(float *)buf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'g': case 'n': case 'o':
			if(j<4)
			{
				*(s64 *)greg=*(s64 *)buf;
				((s64 *)xreg)[0]=((s64 *)buf)[0];
				((s64 *)xreg)[1]=((s64 *)buf)[1];
				greg+=8; xreg+=16; buf+=16; j++;
				break;
			}
			((s64 *)abuf)[0]=((s64 *)buf)[0];
			((s64 *)abuf)[1]=((s64 *)buf)[1];
			abuf+=16; buf+=16; i+=16;
			break;

		case 'C':
			switch(*s++)
			{
			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k': i+=4; break;

			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
			case 'x': case 'y': case 'z':
			case 'r':
				if(j<4)
				{
					*(s64 *)greg=*(s64 *)buf;
					*(s64 *)xreg=*(s64 *)buf;
					greg+=8; xreg+=16; buf+=8; j++;
					break;
				}
				*(s64 *)abuf=*(s64 *)buf;
				abuf+=8; buf+=8; i+=8;
				break;
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

	return(i);
}
#endif

#ifdef X86_64
BASM_API void BASM_CallSig(void *fcn, char *sig, void *buf, void *ret)
{
	char abuf[1024];
	char xreg[128];
	char greg[128];

	char *esig;
	int i;

	BASM_InitSig();

//	i=BASM_LenSig(sig, &esig);
	i=BASM_SplitArgsSig(sig, &esig, buf, abuf, greg, xreg);
	i=(i+31)&(~15);

//	printf("CS Dbg ESig: %s\n", esig);

	switch(*esig)
	{
	case 0: case 'v':
		basm_callsig_void_f(fcn, abuf, i, greg, xreg);
		break;

	case 'a': case 'b': case 'c': case 'h':
	case 's': case 't': case 'w':
	case 'i': case 'j':
		*(int *)ret=basm_callsig_int_f(fcn, abuf, i, greg, xreg);
		break;

	case 'l': case 'm': case 'x': case 'y':
		*(s64 *)ret=basm_callsig_s64_f(fcn, abuf, i, greg, xreg); break;

	case 'r': case 'P': case 'L': case 'Q':
		*(void **)ret=basm_callsig_ptr_f(fcn, abuf, i, greg, xreg);
		break;

	case 'X':
		basm_callsig_str_f(fcn, abuf, i, ret, greg, xreg);
		break;

	case 'd':
		*(double *)ret=basm_callsig_dbl_f(fcn, abuf, i, greg, xreg);
//		printf("CS Dbg %f\n", *(double *)ret);
		break;
	case 'e':
		*(long double *)ret=basm_callsig_dbl_f(fcn, abuf, i, greg, xreg);
		break;
	case 'f':
		*(float *)ret=basm_callsig_dbl_f(fcn, abuf, i, greg, xreg);
		break;
	case 'k':
		*(float *)ret=basm_callsig_dbl_f(fcn, abuf, i, greg, xreg);
		break;

//	case 'g':
//		*(double *)ret=basm_callsig_dbl_f(fcn, buf, i);
//		break;
//	case 'n': i+=16; break;
//	case 'o': i+=16; break;

	case 'C':
		switch(esig[1])
		{
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':
			*(s64 *)ret=basm_callsig_s64_f(fcn, abuf, i, greg, xreg); break;

		case 'a': case 'b': case 'c': case 'q':
		case 'f': case 'g': case 'd': case 'e': case 'k':
			basm_callsig_str_f(fcn, abuf, i, ret, greg, xreg);
			break;
		case 'r':
			*(s64 *)ret=basm_callsig_s64_f(fcn, abuf, i, greg, xreg); break;
		default: break;
		}
		break;

	case 'D':
		switch(esig[1])
		{
		case 'a': case 'b': case 'c': case 'q':
		case 'f': case 'g': case 'd': case 'e': case 'k':
			basm_callsig_str_f(fcn, abuf, i, ret, greg, xreg);
			break;
		default: break;
		}
		break;


	case 'G':
		break;

	default: break;
	}
}

BASM_API void BASM_CallSigStd(void *fcn, char *sig, void *buf, void *ret)
{
	BASM_CallSig(fcn, sig, buf, ret);
}

#endif

#ifdef ARM
BASM_API void BASM_CallSig(void *fcn, char *sig, void *buf, void *ret)
{
}
#endif

#ifdef X86_32
void BASM_ProcessSigArgs(char *sig, va_list lst, void *buf)
{
	char *s;
	byte *tb;
	int i;

	s=sig; tb=buf; i=0;
	while(*s && (*s!=':'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
		case 'l': case 'm':
			*(int *)(tb+i)=va_arg(lst, int);
			i+=4; break;

		case 'd':
			*(double *)(tb+i)=va_arg(lst, double);
			i+=8; break;
		case 'e':
			*(long double *)(tb+i)=va_arg(lst, long double);
			i+=12; break;
		case 'f':
			*(float *)(tb+i)=va_arg(lst, double);
			i+=4; break;
		case 'g':
			*(long double *)(tb+i+6)=va_arg(lst, long double);
			i+=16; break;
		case 'k':
			*(float *)(tb+i)=va_arg(lst, double);
			i+=4; break;

		case 'n':
		case 'o':
			*(s64 *)(tb+i)=va_arg(lst, s64);
			*(s64 *)(tb+i+8)=va_arg(lst, s64);
			i+=16; break;

		case 'r':
			*(void **)(tb+i)=va_arg(lst, void *);
			i+=4; break;

		case 'x': case 'y':
			*(s64 *)(tb+i)=va_arg(lst, s64);
			i+=8; break;

		case 'C':
			switch(*s++)
			{
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
				*(void **)(tb+i)=va_arg(lst, void *);
				i+=4; break;

			case 'f':
				*(float *)(tb+i)=va_arg(lst, double);
				*(float *)(tb+i+4)=va_arg(lst, double);
				i+=8; break;
			case 'd':
				*(double *)(tb+i)=va_arg(lst, double);
				*(double *)(tb+i+8)=va_arg(lst, double);
				i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k':
				*(int *)(tb+i)=va_arg(lst, int);
				i+=4; break;

			case 'r':
				*(s64 *)(tb+i)=va_arg(lst, s64);
				i+=8; break;

			case 'x': case 'y': case 'z':
				*(void **)(tb+i)=va_arg(lst, void *);
				i+=4; break;
			}
			break;
		case 'G':
			switch(*s++)
			{
			case 'f':
				*(float *)(tb+i)=va_arg(lst, double);
				i+=4; break;
			case 'd':
				*(double *)(tb+i)=va_arg(lst, double);
				i+=8; break;
			case 'e':
				*(long double *)(tb+i)=va_arg(lst, long double);
				i+=12; break;
			case 'g':
				*(long double *)(tb+i+6)=va_arg(lst, long double);
				i+=16; break;
			case 'k':
				*(float *)(tb+i)=va_arg(lst, double);
				i+=4; break;
			}
			break;

		case 'P':
			*(void **)(tb+i)=va_arg(lst, void *);

			i+=4;
			while(*s=='P')s++;
			if((*s=='C') || (*s=='G') || (*s=='D'))s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		default: break;
		}
	}

	return;
}
#endif


#ifdef AMD64
int BASM_MergeArgsSig(char *sig, char **esig,
	byte *buf, byte *abuf, byte *greg, byte *xreg)
{
	char *s;
	byte *sb;
	int i, j, ni, nx;

	sb=buf;
	s=sig; i=0; ni=0; nx=0;
	while(*s && (*s!=':'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			if(ni<6)
			{
				*(int *)buf=*(int *)greg;
				greg+=8; buf+=8; ni++;
				break;
			}
			*(int *)buf=*(int *)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(ni<6)
			{
				*(s64 *)buf=*(s64 *)greg;
				greg+=8; buf+=8; ni++;
				break;
			}
			*(s64 *)buf=*(s64 *)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'r':
			if(ni<6)
			{
				*(void **)buf=*(void **)greg;
				greg+=8; buf+=8; ni++;
				break;
			}
			*(void **)buf=*(void **)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'P':
			if(ni<6)
			{
				*(void **)buf=*(void **)greg;
				greg+=8; buf+=8; ni++;
				break;
			}else
			{
				*(void **)buf=*(void **)abuf;
				abuf+=8; buf+=8; i+=8;
			}

			while(*s=='P')s++;
			if((*s=='C') || (*s=='G') || (*s=='D'))s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'd':
			if(nx<8)
			{
				*(double *)buf=*(double *)xreg;
				xreg+=16; buf+=8; nx++;
				break;
			}
			*(double *)buf=*(double *)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'e':
			if((buf-sb)&15) { buf+=16-((buf-sb)&15); }
			if(nx<8)
			{
				*(long double *)buf=*(long double *)xreg;
				xreg+=16; buf+=16; nx++;
				break;
			}

			if(i&15) { abuf+=16-(i&15); i=(i+15)&(~15); }
			*(long double *)buf=*(long double *)abuf;
			abuf+=16; buf+=16; i+=16;
			break;

		case 'f': case 'k':
			if(nx<8)
			{
				*(float *)buf=*(float *)xreg;
				xreg+=16; buf+=8; nx++;
				break;
			}
			*(float *)buf=*(float *)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'g': case 'n': case 'o':
			if((buf-sb)&15) { buf+=16-((buf-sb)&15); }
			if(nx<8)
			{
				((s64 *)buf)[0]=((s64 *)xreg)[0];
				((s64 *)buf)[1]=((s64 *)xreg)[1];
				xreg+=16; buf+=16; nx++;
				break;
			}
			if(i&15) { abuf+=16-(i&15); i=(i+15)&(~15); }
			((s64 *)buf)[0]=((s64 *)abuf)[0];
			((s64 *)buf)[1]=((s64 *)abuf)[1];
			abuf+=16; buf+=16; i+=16;
			break;

		case 'C':
			switch(*s++)
			{
			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k': i+=4; break;

			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
			case 'x': case 'y': case 'z':
			case 'r':
				if(ni<6)
				{
					*(s64 *)buf=*(s64 *)greg;
					greg+=8; buf+=8; ni++;
					break;
				}
				*(s64 *)buf=*(s64 *)abuf;
				abuf+=8; buf+=8; i+=8;
				break;
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

	if(*s && *s==':')s++;
	if(esig)*esig=s;

	return(i);
}
#endif

#ifdef WIN64
int BASM_MergeArgsSig(char *sig, char **esig,
	byte *buf, byte *abuf, byte *greg, byte *xreg)
{
	char *s;
	int i, j, ni, nx;

	s=sig; i=0; j=0;
	while(*s && (*s!=':'))
	{
		switch(*s++)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 's': case 't': case 'w':
		case 'i': case 'j':
			if(j<4)
			{
				*(int *)buf=*(int *)greg;
				buf+=8; greg+=8; xreg+=16; j++;
				break;
			}
			*(int *)buf=*(int *)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'l': case 'm':
		case 'x': case 'y':
			if(j<4)
			{
				*(s64 *)buf=*(s64 *)greg;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(s64 *)buf=*(s64 *)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'r':
			if(j<4)
			{
				*(void **)buf=*(void **)greg;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(void **)buf=*(void **)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'P':
			if(j<4)
			{
				*(void **)buf=*(void **)greg;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}else
			{
				*(void **)buf=*(void **)abuf;
				abuf+=8; buf+=8; i+=8;
			}

			while(*s=='P')s++;
			if((*s=='C') || (*s=='G') || (*s=='D'))s++;
			if((*s>='a') && (*s<='z'))
				{ s++; break; }
			if(*s=='X')
			{
				while(*s && *s!=';')s++;
				if(*s==';')s++;
			}
			break;

		case 'd':
//		case 'e':
			if(j<4)
			{
				*(double *)buf=*(double *)xreg;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(double *)buf=*(double *)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

#if 1
		case 'e':
			if(j<4)
			{
				*(long double *)buf=*(long double *)xreg;
				greg+=8; xreg+=16; buf+=16; j++;
				break;
			}

			*(long double *)buf=*(long double *)abuf;
			abuf+=16; buf+=16; i+=16;
			break;
#endif

		case 'f': case 'k':
			if(j<4)
			{
				*(float *)buf=*(float *)xreg;
				greg+=8; xreg+=16; buf+=8; j++;
				break;
			}
			*(float *)buf=*(float *)abuf;
			abuf+=8; buf+=8; i+=8;
			break;

		case 'g': case 'n': case 'o':
			if(j<4)
			{
				((s64 *)buf)[0]=((s64 *)xreg)[0];
				((s64 *)buf)[1]=((s64 *)xreg)[1];
				greg+=8; xreg+=16; buf+=16; j++;
				break;
			}
			((s64 *)buf)[0]=((s64 *)abuf)[0];
			((s64 *)buf)[1]=((s64 *)abuf)[1];
			abuf+=16; buf+=16; i+=16;
			break;

		case 'C':
			switch(*s++)
			{
			case 'f': i+=8; break;
			case 'd': i+=16; break;
			case 'e': i+=32; break;
			case 'g': i+=32; break;
			case 'k': i+=4; break;

			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
			case 'x': case 'y': case 'z':
			case 'r':
				if(j<4)
				{
					*(s64 *)buf=*(s64 *)greg;
					greg+=8; xreg+=16; buf+=8; j++;
					break;
				}
				*(s64 *)buf=*(s64 *)abuf;
				abuf+=8; buf+=8; i+=8;
				break;
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

	if(*s && *s==':')s++;
	if(esig)*esig=s;

	return(i);
}
#endif

#ifdef X86_32
void BASM_CreateStub_Inner32(void *args, void **data)
{
	void (*pf_v)(void *args, void *data);
	pf_v=data[0];
	pf_v(args, data[1]);
}
#endif

#ifdef X86_64
void BASM_CreateStub_Inner64(void *args, void **data, void *greg, void *xreg)
{
	byte abuf[512];
	void (*pf_v)(void *args, void *data);

	BASM_MergeArgsSig(data[2], NULL, abuf, args, greg, xreg);
	pf_v=data[0];
	pf_v(abuf, data[1]);
}
#endif

BASM_API void BASM_CreateStub(void *buf, void *fcn, void *data, char *sig)
{
	byte *ps, *pe, *pd;
	byte *pb;
	void **pbd;

	BASM_InitSig();

	ps=BASM_GetPtr("basm_callsig_closestub_start");
	pe=BASM_GetPtr("basm_callsig_closestub_end");
	pd=BASM_GetPtr("basm_callsig_closestub_data");

	pb=buf; pbd=(void **)(pb+(pd-ps));
	memcpy(pb, ps, pe-ps);
	pbd[0]=fcn;
	pbd[1]=data;
	pbd[2]=sig;
}

BASM_API void *BASM_CreateStubGC(void *fcn, void *data, char *sig)
{
	byte *ps, *pe, *pd;
	byte *pb;
	void **pbd;

	BASM_InitSig();

	ps=BASM_GetPtr("basm_callsig_closestub_start");
	pe=BASM_GetPtr("basm_callsig_closestub_end");
	pd=BASM_GetPtr("basm_callsig_closestub_data");

	pb=BLNK_gcExec(pe-ps);

	pbd=(void **)(pb+(pd-ps));
	memcpy(pb, ps, pe-ps);
	pbd[0]=fcn;
	pbd[1]=data;
	pbd[2]=sig;

	return((void *)pb);
}

void BASM_InitSig()
{
	static int init=0;
	void *p;

	if(init)return;
	init=1;

	BASM_Init();

	BASM_BeginAssembly(NULL);
	basm_puts(basm_callsig_asm);
	BASM_EndAssemblyQuiet();

	p=BASM_GetPtr("basm_callsig_basic");
	basm_callsig_void_f=p;
	basm_callsig_int_f=p;
	basm_callsig_ptr_f=p;
	basm_callsig_dbl_f=p;
	basm_callsig_s64_f=p;

//	BDISASM_PrintOpcodes(p, 0, 32);

	p=BASM_GetPtr("basm_callsig_struct");
	basm_callsig_str_f=p;

#ifdef X86_32
	p=BASM_GetPtr("basm_callsig_std_basic");
	basm_callsig_stdvoid_f=p;
	basm_callsig_stdint_f=p;
	basm_callsig_stdptr_f=p;
	basm_callsig_stddbl_f=p;
	basm_callsig_stds64_f=p;

	p=BASM_GetPtr("basm_callsig_std_struct");
	basm_callsig_stdstr_f=p;
#endif
}


#ifdef X86
BASM_API void *BASM_MakeBufferCallStub(void *fcn, char *sig)
{
	byte *obj;
	char *esig;
	int i, j;

	BASM_InitSig();

	i=BASM_LenSig(sig, &esig);
	i=(i+15)&(~15);

//	printf("BASM_CallSig: %p %s %s\n", fcn, sig, esig);

	obj=NULL;

	switch(*esig)
	{
	case 0: case 'v':
		BASM_BeginAssembly(NULL);
		basm_print("[section .text]\n[bits 32]\n");
		basm_print("thunk_start:\n");
		basm_print("push ebp; mov ebp, esp\n");

		basm_print("sub esp, %d\n", i);
		basm_print("mov eax, [ebp+8]\n");
		for(j=0; j<(i/16); j++)
		{
			basm_print("movdqu xmm0, [eax+%d]\n", j*16);
			basm_print("movdqu [esp+%d], xmm0\n", j*16);
		}
//		basm_print("call dword 0x%08X\n", (int)fcn);
		basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
		basm_print("mov esp, ebp; pop ebp; ret\n");
		obj=BASM_EndAssemblyThunkInline(NULL);
		break;

	case 'a': case 'b': case 'c': case 'h':
	case 's': case 't': case 'w':
	case 'i': case 'j': case 'l': case 'm':

	case 'r': case 'P': case 'L': case 'Q':
		BASM_BeginAssembly(NULL);
		basm_print("[section .text]\n[bits 32]\n");
		basm_print("thunk_start:\n");
		basm_print("push ebp; mov ebp, esp\n");

		basm_print("sub esp, %d\n", i);
		basm_print("mov eax, [ebp+8]\n");
		for(j=0; j<(i/16); j++)
		{
			basm_print("movdqu xmm0, [eax+%d]\n", j*16);
			basm_print("movdqu [esp+%d], xmm0\n", j*16);
		}
//		basm_print("call dword 0x%08X\n", (int)fcn);
		basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
		basm_print("mov ecx, [ebp+8]\n");
		basm_print("mov [ecx], eax\n");
		basm_print("mov esp, ebp; pop ebp; ret\n");
		obj=BASM_EndAssemblyThunkInline(NULL);
		break;

	case 'f': case 'k':
		BASM_BeginAssembly(NULL);
		basm_print("[section .text]\n[bits 32]\n");
		basm_print("thunk_start:\n");
		basm_print("push ebp; mov ebp, esp\n");

		basm_print("sub esp, %d\n", i);
		basm_print("mov eax, [ebp+8]\n");
		for(j=0; j<(i/16); j++)
		{
			basm_print("movdqu xmm0, [eax+%d]\n", j*16);
			basm_print("movdqu [esp+%d], xmm0\n", j*16);
		}
//		basm_print("call dword 0x%08X\n", (int)fcn);
		basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
		basm_print("mov ecx, [ebp+8]\n");
		basm_print("fstp32 [ecx]\n");
		basm_print("mov esp, ebp; pop ebp; ret\n");
		obj=BASM_EndAssemblyThunkInline(NULL);
		break;

	case 'd': case 'e':
		BASM_BeginAssembly(NULL);
		basm_print("[section .text]\n[bits 32]\n");
		basm_print("thunk_start:\n");
		basm_print("push ebp; mov ebp, esp\n");

		basm_print("sub esp, %d\n", i);
		basm_print("mov eax, [ebp+8]\n");
		for(j=0; j<(i/16); j++)
		{
			basm_print("movdqu xmm0, [eax+%d]\n", j*16);
			basm_print("movdqu [esp+%d], xmm0\n", j*16);
		}
//		basm_print("call dword 0x%08X\n", (int)fcn);
		basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
		basm_print("mov ecx, [ebp+8]\n");
		basm_print("fstp64 [ecx]\n");
		basm_print("mov esp, ebp; pop ebp; ret\n");
		obj=BASM_EndAssemblyThunkInline(NULL);
		break;

	case 'x': case 'y':
		BASM_BeginAssembly(NULL);
		basm_print("[section .text]\n[bits 32]\n");
		basm_print("thunk_start:\n");
		basm_print("push ebp; mov ebp, esp\n");

		basm_print("sub esp, %d\n", i);
		basm_print("mov eax, [ebp+8]\n");
		for(j=0; j<(i/16); j++)
		{
			basm_print("movdqu xmm0, [eax+%d]\n", j*16);
			basm_print("movdqu [esp+%d], xmm0\n", j*16);
		}
//		basm_print("call dword 0x%08X\n", (int)fcn);
		basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
		basm_print("mov ecx, [ebp+8]\n");
		basm_print("mov [ecx+0], eax\n");
		basm_print("mov [ecx+4], edx\n");
		basm_print("mov esp, ebp; pop ebp; ret\n");
		obj=BASM_EndAssemblyThunkInline(NULL);
		break;

	case 'X':
		BASM_BeginAssembly(NULL);
		basm_print("[section .text]\n[bits 32]\n");
		basm_print("thunk_start:\n");
		basm_print("push ebp; mov ebp, esp\n");

		basm_print("sub esp, %d\n", i);
		basm_print("mov eax, [ebp+8]\n");
		for(j=0; j<(i/16); j++)
		{
			basm_print("movdqu xmm0, [eax+%d]\n", j*16);
			basm_print("movdqu [esp+%d], xmm0\n", j*16);
		}
		basm_print("push dword [ebp+8]\n");
//		basm_print("call dword 0x%08X\n", (int)fcn);
		basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
		basm_print("mov esp, ebp; pop ebp; ret\n");
		obj=BASM_EndAssemblyThunkInline(NULL);
		break;

	case 'C':
		switch(esig[1])
		{
		case 'a': case 'b': case 'c': case 'q':
		case 'f': case 'g': case 'd': case 'e': case 'k':
			BASM_BeginAssembly(NULL);
			basm_print("[section .text]\n[bits 32]\n");
			basm_print("thunk_start:\n");
			basm_print("push ebp; mov ebp, esp\n");

			basm_print("sub esp, %d\n", i);
			basm_print("mov eax, [ebp+8]\n");
			for(j=0; j<(i/16); j++)
			{
				basm_print("movdqu xmm0, [eax+%d]\n", j*16);
				basm_print("movdqu [esp+%d], xmm0\n", j*16);
			}
			basm_print("push dword [ebp+8]\n");
//			basm_print("call dword 0x%08X\n", (int)fcn);
			basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
			basm_print("mov esp, ebp; pop ebp; ret\n");
			obj=BASM_EndAssemblyThunkInline(NULL);
			break;

		case 'r':
			BASM_BeginAssembly(NULL);
			basm_print("[section .text]\n[bits 32]\n");
			basm_print("thunk_start:\n");
			basm_print("push ebp; mov ebp, esp\n");

			basm_print("sub esp, %d\n", i);
			basm_print("mov eax, [ebp+8]\n");
			for(j=0; j<(i/16); j++)
			{
				basm_print("movdqu xmm0, [eax+%d]\n", j*16);
				basm_print("movdqu [esp+%d], xmm0\n", j*16);
			}
//			basm_print("call dword 0x%08X\n", (int)fcn);
			basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
			basm_print("mov ecx, [ebp+8]\n");
			basm_print("mov [ecx+0], eax\n");
			basm_print("mov [ecx+4], edx\n");
			basm_print("mov esp, ebp; pop ebp; ret\n");
			obj=BASM_EndAssemblyThunkInline(NULL);
			break;
		default: obj=NULL; break;
		}
		break;

	case 'D':
		switch(esig[1])
		{
		case 'a': case 'b': case 'c': case 'q':
		case 'f': case 'g': case 'd': case 'e': case 'k':
			BASM_BeginAssembly(NULL);
			basm_print("[section .text]\n[bits 32]\n");
			basm_print("thunk_start:\n");
			basm_print("push ebp; mov ebp, esp\n");

			basm_print("sub esp, %d\n", i);
			basm_print("mov eax, [ebp+8]\n");
			for(j=0; j<(i/16); j++)
			{
				basm_print("movdqu xmm0, [eax+%d]\n", j*16);
				basm_print("movdqu [esp+%d], xmm0\n", j*16);
			}
			basm_print("push dword [ebp+8]\n");
//			basm_print("call dword 0x%08X\n", (int)fcn);
			basm_print("mov eax, 0x%08X; call eax\n", (int)fcn);
			basm_print("mov esp, ebp; pop ebp; ret\n");
			obj=BASM_EndAssemblyThunkInline(NULL);
			break;

		default: obj=NULL; break;
		}
		break;

	case 'G': break;
	default: break;
	}

	return((void *)obj);
}

#else
BASM_API void *BASM_MakeBufferCallStub(void *fcn, char *sig)
{
	return(NULL);
}
#endif

#else

void BASM_InitSig()
{
}

BASM_API void *BASM_MakeBufferCallStub(void *fcn, char *sig)
{
	return(NULL);
}
#endif