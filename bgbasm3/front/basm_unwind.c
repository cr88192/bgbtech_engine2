/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

/*

Unwind API:
Will provide code both for a setjmp/longjmp like interface, and also for
nested unwinding. May or may not wrap SEH on Win32.

 */

#include <bgbasm.h>

#ifdef WIN32
#include <windows.h>
#endif

static void *(*basm_getregs_f)(void **regs);
static void (*basm_jmpregs_f)(void **regs, void *val);
static void *(*basm_startunwind_f)(void *ctx);
static void (*basm_endunwind_f)(void *ctx);

static int (*basm_saveregs_f)(void *regs);

void BASM_InitUnwind()
{
	static int init=0;
	void *p;

	if(init)return;
	init=1;

#ifdef __i386__
	BASM_BeginAssembly(NULL);

	//void basm_getregs(void **regs);
	basm_print("basm_getregs:\n");
	basm_print("mov eax, [esp+4]\n");
	basm_print("mov ecx, [esp]\n");
	basm_print("lea edx, [esp+4]\n");
	basm_print("mov [eax+ 0], ecx\n");	//eip
	basm_print("mov [eax+ 4], edx\n");	//esp
	basm_print("mov [eax+ 8], ecx\n");	//ebp
	basm_print("mov [eax+12], esi\n");	//esi
	basm_print("mov [eax+16], edi\n");	//edi
	basm_print("mov [eax+20], ebx\n");	//ebx
	basm_print("xor eax, eax\n");		//NULL return
	basm_print("ret\n");

	//__noret basm_jmpregs(void **regs, void *val);
	basm_print("basm_jmpregs:\n");
	basm_print("mov edx, [esp+4]\n");
	basm_print("mov eax, [esp+8]\n");
	basm_print("mov ebp, [edx+ 8]\n");	//ebp
	basm_print("mov esi, [edx+12]\n");	//esi
	basm_print("mov edi, [edx+16]\n");	//edi
	basm_print("mov edx, [edx+20]\n");	//ebx
	basm_print("mov esp, [edx+ 4]\n");	//esp
	basm_print("jmp dword [edx+0]\n");	//eip
	basm_print("ret\n");			//non-reached

#ifdef WIN32
	basm_print("basm_startunwind:\n");
	basm_print("mov eax, [esp+4]\n");
	basm_print("mov edx, basm_sehmagic\n");
	basm_print("mov [eax+4], edx\n");
	basm_print("mov edx, [fs:0]\n");
	basm_print("mov [eax], edx\n");
	basm_print("mov [fs:0], eax\n");
	basm_print("lea eax, [eax+8]\n");
	basm_print("mov [esp+4], eax\n");
	basm_print("jmp basm_getregs\n");

	basm_print("basm_endunwind:\n");
	basm_print("mov eax, [esp+4]\n");
	basm_print("mov edx, [eax+0]\n");
	basm_print("mov [fs:0], edx\n");
	basm_print("ret\n");

	basm_print("basm_sehmagic:\n");
	basm_print("mov ecx, [esp+4]\n");
	basm_print("mov eax, [ecx+0]\n");
	basm_print("xor eax, 0xE0424742\n");	//BGB SEH magic...
	basm_print("jnz .l0\n");		//crap, actual SEH

	basm_print("mov eax, [esp+8]\n");	//BASM_JmpBuf
	basm_print("mov ecx, [fs:0]\n");	//SEH list
	basm_print("xor ecx, eax\n");		//see if same
	basm_print("jnz .l1\n");		//errm, need to unwind
	basm_print(".l2:\n");			//jump here is unwound

	//now, my code just unwinds...
	basm_print("mov eax, [esp+8]\n");	//BASM_JmpBuf
	basm_print("mov ecx, [esp+4]\n");	//EXCEPTION_RECORD
	basm_print("mov edx, [esp+12]\n");	//CONTEXT
	basm_print("mov [eax+56], ecx\n");
	basm_print("mov [eax+60], edx\n");
	basm_print("mov edx, [eax+0]\n");	//pt1, unlink SEH
	basm_print("mov [fs:0], edx\n");	//pt2
	basm_print("lea ecx, [eax+8]\n");	//JmpBuf->regs
	basm_print("push eax\n");
	basm_print("push ecx\n");
	basm_print("call basm_jmpregs\n");
	basm_print(".l0:\n");
	basm_print("mov eax, 1\n");
	basm_print("ret\n");

	basm_print(".l1:\n");
//	basm_print("push dword 0\n");
//	basm_print("push dword [esp+4]\n");
//	basm_print("push dword .l2\n");
//	basm_print("push dword [esp+8]\n");
//	basm_print("call RtlUnwind\n");

#if 1
//	basm_print("push ebp\n");
//	basm_print("push ebp, esp\n");

//	basm_print("push ebx\n");
//	basm_print("push edi\n");
//	basm_print("push esi\n");

//	basm_print("mov ebx, [ebp+8]\n");
//	basm_print("mov dword [ebx+4], 2\n");
//	basm_print("mov esi, [ebp+12]\n");

	basm_print("mov ebx, [esp+4]\n");
	basm_print("mov dword [ebx+4], 2\n");
	basm_print("mov esi, [esp+8]\n");

	basm_print("mov edi, [fs:0]\n");
	basm_print(".l3:\n");
//	basm_print("cmp dword [edi], -1\n");
//	basm_print("jz .l4\n");
	basm_print("cmp [edi], esi\n");
	basm_print("jz .l4\n");

	basm_print("push dword [ebp+16]\n");
	basm_print("push edi\n");
	basm_print("push ebx\n");
	basm_print("call dword [edi+4]\n");
	basm_print("add esp, 12\n");
	basm_print("mov edi, [edi]\n");
	basm_print("jmp .l3\n");
	basm_print(".l4:\n");

//	basm_print("pop esi\n");
//	basm_print("pop edi\n");
//	basm_print("pop ebx\n");

//	basm_print("pop ebp\n");
#endif

	basm_print("jmp .l2\n");

#else
	basm_print("basm_startunwind:\n");
	basm_print("call _XT_unwind__4_ecx\n");
	basm_print("mov eax, [esp+4]\n");
//	basm_print("mov edx, basm_sehmagic\n");
//	basm_print("mov [eax+4], edx\n");
	basm_print("mov edx, [ecx]\n");
	basm_print("mov [eax], edx\n");
	basm_print("mov [ecx], eax\n");
	basm_print("lea eax, [eax+8]\n");
	basm_print("mov [esp+4], eax\n");
	basm_print("jmp basm_getregs\n");

	basm_print("basm_endunwind:\n");
	basm_print("call _XT_unwind__4_ecx\n");
	basm_print("mov eax, [esp+4]\n");
	basm_print("mov edx, [eax+0]\n");
	basm_print("mov [ecx], edx\n");
	basm_print("ret\n");

#endif

#if 1
	basm_print("basm_saveregs:\n");
	basm_print("push ecx\n");
	basm_print("mov ecx, [esp+8]\n");

	basm_print("mov [ecx+ 0], eax\n");		//eax
	basm_print("mov [ecx+ 4], ecx\n");		//ecx
	basm_print("mov [ecx+ 8], edx\n");		//edx
	basm_print("mov [ecx+12], ebx\n");		//ebx
	basm_print("mov [ecx+16], esp\n");		//esp
	basm_print("mov [ecx+20], ebp\n");		//ebp
	basm_print("mov [ecx+24], esi\n");		//esi
	basm_print("mov [ecx+28], edi\n");		//edi
	basm_print("movdqu [ecx+ 32], xmm0\n");		//xmm0
	basm_print("movdqu [ecx+ 48], xmm1\n");		//xmm1
	basm_print("movdqu [ecx+ 64], xmm2\n");		//xmm2
	basm_print("movdqu [ecx+ 80], xmm3\n");		//xmm3
	basm_print("movdqu [ecx+ 96], xmm4\n");		//xmm4
	basm_print("movdqu [ecx+112], xmm5\n");		//xmm5
	basm_print("movdqu [ecx+128], xmm6\n");		//xmm6
	basm_print("movdqu [ecx+144], xmm7\n");		//xmm7

	basm_print("pop eax\n");
	basm_print("mov [ecx+ 4], eax\n");		//ecx

	basm_print("mov eax, [esp]\n");		//get eip
	basm_print("lea edx, [esp+4]\n");	//get esp
	basm_print("mov [ecx+160], eax\n");	//eip
	basm_print("mov [ecx+160], edx\n");	//esp (IP popped)

	basm_print("xor eax, eax\n");		//NULL return
	basm_print("ret\n");
#endif

	p=BASM_EndAssemblyQuiet();
//	return(p);

	basm_getregs_f=BASM_GetPtr("basm_getregs");
	basm_jmpregs_f=BASM_GetPtr("basm_jmpregs");

	basm_startunwind_f=BASM_GetPtr("basm_startunwind");
	basm_endunwind_f=BASM_GetPtr("basm_endunwind");

	basm_saveregs_f=BASM_GetPtr("basm_saveregs");
#endif

	//need x86-64 support...

#ifdef X86_64
	BASM_BeginAssembly(NULL);

	//void basm_getregs(void **regs);
	basm_print("basm_getregs:\n");
//	basm_print("mov rax, rcx\n");		//get regs
	basm_print("mov r8, [rsp]\n");		//get rip
	basm_print("lea r9, [rsp+8]\n");	//get rsp
	basm_print("mov [rcx+ 0], r8\n");	//rip
	basm_print("mov [rcx+ 8], r9\n");	//rsp (IP popped)
	basm_print("mov [rcx+16], rbp\n");	//rbp
	basm_print("mov [rcx+24], rsi\n");	//rsi
	basm_print("mov [rcx+32], rdi\n");	//rdi
	basm_print("mov [rcx+40], rbx\n");	//rbx
	basm_print("mov [rcx+48], r12\n");	//r12
	basm_print("mov [rcx+56], r13\n");	//r13
	basm_print("mov [rcx+64], r14\n");	//r14
	basm_print("mov [rcx+72], r15\n");	//r15
#ifdef WIN64
	basm_print("movdqu [rcx+128], xmm6\n");		//xmm6
	basm_print("movdqu [rcx+144], xmm7\n");		//xmm7
	basm_print("movdqu [rcx+160], xmm8\n");		//xmm8
	basm_print("movdqu [rcx+176], xmm9\n");		//xmm9
	basm_print("movdqu [rcx+192], xmm10\n");	//xmm10
	basm_print("movdqu [rcx+208], xmm11\n");	//xmm11
	basm_print("movdqu [rcx+224], xmm12\n");	//xmm12
	basm_print("movdqu [rcx+240], xmm13\n");	//xmm13
	basm_print("movdqu [rcx+256], xmm14\n");	//xmm14
	basm_print("movdqu [rcx+272], xmm15\n");	//xmm15
#endif

	basm_print("xor eax, eax\n");		//NULL return
	basm_print("ret\n");

	//__noret basm_jmpregs(void **regs, void *val);
	basm_print("basm_jmpregs:\n");
	basm_print("mov rax, rdx\n");		//get val
//	basm_print("mov rdx, rcx\n");		//get regs
	basm_print("mov rbp, [rcx+16]\n");	//ebp
	basm_print("mov rsi, [rcx+24]\n");	//esi
	basm_print("mov rdi, [rcx+32]\n");	//edi
	basm_print("mov rdx, [rcx+40]\n");	//ebx
	basm_print("mov rsp, [rcx+ 8]\n");	//esp
	basm_print("mov r12, [rcx+48]\n");	//r12
	basm_print("mov r13, [rcx+56]\n");	//r13
	basm_print("mov r14, [rcx+64]\n");	//r14
	basm_print("mov r15, [rcx+72]\n");	//r15

#ifdef WIN64
	basm_print("movdqu xmm6, [rcx+128]\n");		//xmm6
	basm_print("movdqu xmm7, [rcx+144]\n");		//xmm7
	basm_print("movdqu xmm8, [rcx+160]\n");		//xmm8
	basm_print("movdqu xmm9, [rcx+176]\n");		//xmm9
	basm_print("movdqu xmm10, [rcx+192]\n");	//xmm10
	basm_print("movdqu xmm11, [rcx+208]\n");	//xmm11
	basm_print("movdqu xmm12, [rcx+224]\n");	//xmm12
	basm_print("movdqu xmm13, [rcx+240]\n");	//xmm13
	basm_print("movdqu xmm14, [rcx+256]\n");	//xmm14
	basm_print("movdqu xmm15, [rcx+272]\n");	//xmm15
#endif

	basm_print("jmp dword [rcx+0]\n");	//rip
	basm_print("ret\n");			//non-reached

#ifdef WIN64
	basm_print("basm_startunwind:\n");
	basm_print("mov rax, rcx\n");
	basm_print("mov rdx, basm_sehmagic\n");
	basm_print("mov [rax+8], rdx\n");
	basm_print("mov rdx, [gs:0]\n");
	basm_print("mov [rax], rdx\n");
	basm_print("mov [gs:0], rax\n");
	basm_print("lea rax, [rax+16]\n");
	basm_print("mov [rsp+8], rax\n");
	basm_print("jmp basm_getregs\n");

	basm_print("basm_endunwind:\n");
	basm_print("mov rax, rcx\n");
	basm_print("mov rdx, [rax+0]\n");
	basm_print("mov [gs:0], rdx\n");
	basm_print("ret\n");


//yeah...

	basm_print("basm_sehmagic:\n");
	basm_print("mov [rsp+8], rcx\n");
	basm_print("mov [rsp+16], rdx\n");
	basm_print("mov [rsp+24], r8\n");
	basm_print("mov [rsp+32], r9\n");

//	basm_print("mov ecx, [esp+4]\n");
	basm_print("mov rax, [rcx+0]\n");
//	basm_print("xor rax, 0xE0424742\n");	//BGB SEH magic...

	basm_print("mov r10, 0xE0424742\n");	//BGB SEH magic...
	basm_print("xor rax, r10\n");		//BGB SEH magic...
	basm_print("jnz .l0\n");		//crap, actual SEH

//	basm_print("mov rax, [rsp+16]\n");	//BASM_JmpBuf
	basm_print("mov rax, rdx\n");		//BASM_JmpBuf
	basm_print("mov rcx, [gs:0]\n");	//SEH list
	basm_print("xor rcx, rax\n");		//see if same
	basm_print("jnz .l1\n");		//errm, need to unwind
	basm_print(".l2:\n");			//jump here is unwound

	//now, my code just unwinds...
	basm_print("mov rax, [esp+16]\n");	//BASM_JmpBuf
	basm_print("mov rcx, [esp+8]\n");	//EXCEPTION_RECORD
	basm_print("mov rdx, [esp+24]\n");	//CONTEXT
	basm_print("mov [rax+112], rcx\n");
	basm_print("mov [rax+120], rdx\n");
	basm_print("mov rdx, [rax+0]\n");	//pt1, unlink SEH
	basm_print("mov [fs:0], rdx\n");	//pt2
	basm_print("lea rcx, [rax+16]\n");	//JmpBuf->regs
//	basm_print("push rax\n");
//	basm_print("push rcx\n");
	basm_print("mov rdx, rax\n");
	basm_print("call basm_jmpregs\n");
	basm_print(".l0:\n");
	basm_print("mov rax, 1\n");
	basm_print("ret\n");

	basm_print(".l1:\n");

#if 1
	basm_print("mov rbx, [rsp+8]\n");
	basm_print("mov dword [rbx+8], 2\n");
	basm_print("mov rsi, [rsp+16]\n");

	basm_print("mov rdi, [gs:0]\n");
	basm_print(".l3:\n");
	basm_print("cmp [rdi], rsi\n");
	basm_print("jz .l4\n");

//	basm_print("push dword [rbp+32]\n");
//	basm_print("push rdi\n");
//	basm_print("push rbx\n");

	basm_print("sub rsp, 32\n");
	basm_print("mov r8, [rbp+32]\n");
	basm_print("mov rdx, rdi\n");
	basm_print("mov rcx, rbx\n");
	basm_print("call qword [rdi+8]\n");
//	basm_print("add rsp, 24\n");
	basm_print("add rsp, 32\n");
	basm_print("mov rdi, [rdi]\n");
	basm_print("jmp .l3\n");
	basm_print(".l4:\n");

#endif

	basm_print("jmp .l2\n");

#else
	basm_print("basm_startunwind:\n");
	basm_print("mov [rsp+8], rcx\n");
	basm_print("call _XT_unwind__8_rcx\n");
	basm_print("mov rax, [rsp+8]\n");
	basm_print("mov rdx, [rcx]\n");
	basm_print("mov [rax], rdx\n");
	basm_print("mov [rcx], rax\n");
	basm_print("lea rax, [rax+16]\n");
//	basm_print("mov [rsp+8], rax\n");
	basm_print("mov rcx, rax\n");
	basm_print("jmp basm_getregs\n");

	basm_print("basm_endunwind:\n");
	basm_print("mov [rsp+8], rcx\n");
	basm_print("call _XT_unwind__8_rcx\n");
	basm_print("mov rax, [rsp+8]\n");
	basm_print("mov rdx, [rax+0]\n");
	basm_print("mov [rcx], rdx\n");
	basm_print("ret\n");

#endif


	basm_print("basm_saveregs:\n");
	basm_print("mov [rcx+  0], rax\n");		//rax
	basm_print("mov [rcx+  8], rcx\n");		//rcx
	basm_print("mov [rcx+ 16], rdx\n");		//rdx
	basm_print("mov [rcx+ 24], rbx\n");		//rbx
	basm_print("mov [rcx+ 32], rsp\n");		//rsp
	basm_print("mov [rcx+ 40], rbp\n");		//rbp
	basm_print("mov [rcx+ 48], rsi\n");		//rsi
	basm_print("mov [rcx+ 56], rdi\n");		//rdi
	basm_print("mov [rcx+ 64], r8\n");		//r8
	basm_print("mov [rcx+ 72], r9\n");		//r9
	basm_print("mov [rcx+ 80], r10\n");		//r10
	basm_print("mov [rcx+ 88], r11\n");		//r11
	basm_print("mov [rcx+ 96], r12\n");		//r12
	basm_print("mov [rcx+104], r13\n");		//r13
	basm_print("mov [rcx+112], r14\n");		//r14
	basm_print("mov [rcx+128], r15\n");		//r15
	basm_print("movdqu [rcx+128], xmm0\n");		//xmm0
	basm_print("movdqu [rcx+144], xmm1\n");		//xmm1
	basm_print("movdqu [rcx+160], xmm2\n");		//xmm2
	basm_print("movdqu [rcx+176], xmm3\n");		//xmm3
	basm_print("movdqu [rcx+192], xmm4\n");		//xmm4
	basm_print("movdqu [rcx+208], xmm5\n");		//xmm5
	basm_print("movdqu [rcx+224], xmm6\n");		//xmm6
	basm_print("movdqu [rcx+240], xmm7\n");		//xmm7
	basm_print("movdqu [rcx+256], xmm8\n");		//xmm8
	basm_print("movdqu [rcx+272], xmm9\n");		//xmm9
	basm_print("movdqu [rcx+288], xmm10\n");	//xmm10
	basm_print("movdqu [rcx+304], xmm11\n");	//xmm11
	basm_print("movdqu [rcx+320], xmm12\n");	//xmm12
	basm_print("movdqu [rcx+336], xmm13\n");	//xmm13
	basm_print("movdqu [rcx+352], xmm14\n");	//xmm14
	basm_print("movdqu [rcx+368], xmm15\n");	//xmm15

	basm_print("mov r8, [rsp]\n");		//get rip
	basm_print("lea r9, [rsp+8]\n");	//get rsp
	basm_print("mov [rcx+384], r8\n");	//rip
	basm_print("mov [rcx+392], r9\n");	//rsp (IP popped)

	basm_print("xor eax, eax\n");		//NULL return
	basm_print("ret\n");

	p=BASM_EndAssemblyQuiet();
//	return(p);

	basm_getregs_f=BASM_GetPtr("basm_getregs");
	basm_jmpregs_f=BASM_GetPtr("basm_jmpregs");

	basm_startunwind_f=BASM_GetPtr("basm_startunwind");
	basm_endunwind_f=BASM_GetPtr("basm_endunwind");

	basm_saveregs_f=BASM_GetPtr("basm_saveregs");
#endif
}

BASM_API int BASM_SaveRegs(BGBASM_RegsBuf *buf)
{
	if(basm_saveregs_f)
		return(basm_saveregs_f(buf));
	return(-1);
}

BASM_API void *BASM_SetJmp(BGBASM_JmpBuf *buf)
{
	if(basm_getregs_f)
		return(basm_getregs_f(buf->regs));
	return(NULL);
}

BASM_API void BASM_LongJmp(BGBASM_JmpBuf *buf, void *val)
{
	if(basm_jmpregs_f)
		basm_jmpregs_f(buf->regs, val);
}

BASM_API void *BASM_StartUnwind(BGBASM_JmpBuf *buf)
{
	if(basm_startunwind_f)
		return(basm_startunwind_f(buf));
	return(NULL);
}

BASM_API void BASM_EndUnwind(BGBASM_JmpBuf *buf)
{
	if(basm_endunwind_f)
		basm_endunwind_f(buf);
}

BASM_API void *BASM_BeginUnwind(BGBASM_JmpBuf *buf)
{
#ifdef WIN32
	BGBASM_JmpBuf *tbuf;
	EXCEPTION_RECORD *er;
	void *p;

	tbuf=BASM_StartUnwind(buf);
	if(!tbuf)return(NULL);

	er=tbuf->seh_ehr;
	p=(void *)(er->ExceptionInformation[0]);
	return(p);
#endif
}

BASM_API void BASM_DoUnwind(void *val)
{
#ifdef WIN32
	RaiseException(0xE0424742, EXCEPTION_NONCONTINUABLE, 1,
		(ULONG_PTR *)(&val));
	return;
#endif
}

