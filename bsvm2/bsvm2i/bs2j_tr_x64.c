/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/*
RAX( 0): Scratch
RCX( 1): Scratch
RDX( 2): Scratch
RBX( 3): Var Cache
RSP( 4): OS Stack
RBP( 5): Frame / Cache
RSI( 6): Locals + VM Stack
RDI( 7): VM Frame
R8 ( 8): Scratch
R9 ( 9): Scratch
R10(10): Scratch
R11(11): Scratch
R12(12): Cache
R13(13): Cache
R14(14): Cache
R15(15): Cache

[RBP+24]: Trace
[RBP+16]: Frame
[RBP+ 8]/[RSP+504]: Return RIP
[RBP+ 0]/[RSP+496]: Saved RBP
[RBP- 8]/[RSP+488]: Saved RSI
[RBP-16]/[RSP+480]: Saved RDI
[RBP-24]/[RSP+472]: Saved RBX
[RBP-32]/[RSP+464]: Saved R12
[RBP-40]/[RSP+456]: Saved R13
[RBP-48]/[RSP+448]: Saved R14
[RBP-56]/[RSP+440]: Saved R15

Frame 

Args:
	 RCX,  RDX,   R8,   R9
	XMM0, XMM1, XMM2, XMM3

[RSP+4]: Opcode
[RSP+0]: Frame

Locals and stack are placed end-to-end in the frame.
The stack is placed after the locals.

Possible Naive Reg Allocator:
  First Pass: Stat most-used vars in a trace.
  Assign top N vars to cache registers.
  Operations loading/storing this var directed to register.
  Write back when trace ends.

 */

// #include <bteifgl.h>

#if defined(X86_64) && defined(WIN64) && defined(BS2I_USE_BASM)

#include <bsvm2_basm.h>

#define BS2J_X64_FRAMESZ	504

#define BS2J_X64_OFS_TRACE	 24
#define BS2J_X64_OFS_FRAME	 16
#define BS2J_X64_OFS_RIP	 8
#define BS2J_X64_OFS_RBP	 0
#define BS2J_X64_OFS_RSI	-8
#define BS2J_X64_OFS_RDI	-16
#define BS2J_X64_OFS_RBX	-24
#define BS2J_X64_OFS_R12	-32
#define BS2J_X64_OFS_R13	-40
#define BS2J_X64_OFS_R14	-48
#define BS2J_X64_OFS_R15	-56

int bs2j_op_miscnt[4096];		//count of missed opcodes
int bs2j_op_miscnt2[4096];		//count of missed opcodes (multi-part)

int bs2j_op_usecnt[4096];		//count of used opcodes
int bs2j_op_usecnt2[4096];		//count of used opcodes (multi-part)

int BS2J_VirtualEBP(BSVM2_Trace *tr, int ofs)
{
	return(ofs+BS2J_X64_FRAMESZ-8);
}

int BS2J_CheckSavedESI(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDESI))
	{
		basm_print("mov [rsp+%d], rsi\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_RSI));
		tr->trfl|=BSVM2_TRFL_SAVEDESI;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedEDI(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDEDI))
	{
		basm_print("mov [rsp+%d], rdi\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_RDI));
		tr->trfl|=BSVM2_TRFL_SAVEDEDI;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedEBX(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDEBX))
	{
		basm_print("mov [rsp+%d], rbx\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_RBX));
		tr->trfl|=BSVM2_TRFL_SAVEDEBX;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedEBP(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDEBP))
	{
		basm_print("mov [rsp+%d], rbp\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_RBP));
		tr->trfl|=BSVM2_TRFL_SAVEDEBP;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedR12(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDR12))
	{
		basm_print("mov [rsp+%d], r12\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_R12));
		tr->trfl|=BSVM2_TRFL_SAVEDR12;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedR13(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDR13))
	{
		basm_print("mov [rsp+%d], r13\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_R13));
		tr->trfl|=BSVM2_TRFL_SAVEDR13;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedR14(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDR14))
	{
		basm_print("mov [rsp+%d], r14\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_R14));
		tr->trfl|=BSVM2_TRFL_SAVEDR14;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedR15(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDR15))
	{
		basm_print("mov [rsp+%d], r15\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_R15));
		tr->trfl|=BSVM2_TRFL_SAVEDR15;
		return(1);
	}
	return(0);
}

int BS2J_CheckFrameOnStack(BSVM2_Trace *tr)
{
	if(tr->trfl&BSVM2_TRFL_FRMONSTK)
		return(0);

	if(tr->trfl&BSVM2_TRFL_FRMINEDI)
	{
		basm_print("mov [rsp], rdi\n");
		tr->trfl|=BSVM2_TRFL_FRMONSTK;
	}else
	{
		basm_print("mov rax, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_FRAME));
		basm_print("mov [rsp], rax\n");
		tr->trfl|=BSVM2_TRFL_FRMONSTK;
	}
	return(1);
}

int BS2J_CheckFrameInRCX(BSVM2_Trace *tr)
{
	if(tr->trfl&BSVM2_TRFL_NOFRAME)
	{
		return(1);
	}

	if(tr->trfl&BSVM2_TRFL_FRMINEDI)
	{
		basm_print("mov rcx, rdi\n");
	}else
	{
		basm_print("mov rcx, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_FRAME));
	}
	return(1);
}

int BS2J_CheckCacheFrameEDI(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_FRMINEDI))
	{
		BS2J_CheckSavedEDI(tr);
		basm_print("mov rdi, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_FRAME));
		tr->trfl|=BSVM2_TRFL_FRMINEDI;
		return(1);
	}
	return(0);
}

int BS2J_CheckCacheLocalsESI(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_LCLINESI))
	{
		BS2J_CheckSavedESI(tr);
		BS2J_CheckCacheFrameEDI(tr);

		basm_print("mov rsi, [rdi+%d]\n", offsetof(BSVM2_Frame, local));
		tr->trfl&=~BSVM2_TRFL_OPSINESI;
		tr->trfl|=BSVM2_TRFL_LCLINESI;
		return(1);
	}
	return(0);
}

int BS2J_GetLocalRegSpanID(BSVM2_Trace *tr, int idx)
{
	BSVM2_JitTraceTemp *jt;
	int i;
	
	jt=tr->jitmp;
	if(!jt)return(-1);
	
	for(i=0; i<jt->n_lclstat; i++)
	{
		if(jt->opix<jt->lclstat_beg[i])
			continue;
		if(jt->opix>jt->lclstat_end[i])
			continue;
		if(idx!=jt->lclstat_idx[i])
			continue;
		return(i);
	}
	return(-1);
}

int BS2J_GetLocalRegID(BSVM2_Trace *tr, int idx)
{
	int i, j, k;
	i=BS2J_GetLocalRegSpanID(tr, idx);
	if(i>=0)
		return(tr->jitmp->lclstat_reg[i]);
	return(-1);
}

static char *bs2j_regsq[16]={
	"rax", "rcx", "rdx", "rbx",
	"rsp", "rbp", "rsi", "rdi",
	"r8" , "r9" , "r10", "r11",
	"r12", "r13", "r14", "r15"};

static char *bs2j_regsd[16]={
	"eax" , "ecx" , "edx" , "ebx" ,
	"esp" , "ebp" , "esi" , "edi" ,
	"r8d" , "r9d" , "r10d", "r11d",
	"r12d", "r13d", "r14d", "r15d"};

static char *bs2j_regsw[16]={
	"ax"  , "cx"  , "dx"  , "bx"  ,
	"sp"  , "bp"  , "si"  , "di"  ,
	"r8w" , "r9w" , "r10w", "r11w",
	"r12w", "r13w", "r14w", "r15w"};

static char *bs2j_regsb[16]={
	"al"  , "cl"  , "dl"  , "bl"  ,
	"spl" , "bpl" , "sil" , "dil" ,
	"r8b" , "r9b" , "r10b", "r11b",
	"r12b", "r13b", "r14b", "r15b"};

char *BS2J_RegIdxToName(int id)
	{ return(bs2j_regsq[id&15]); }
char *BS2J_RegIdxToNameD(int id)
	{ return(bs2j_regsd[id&15]); }
char *BS2J_RegIdxToNameW(int id)
	{ return(bs2j_regsw[id&15]); }
char *BS2J_RegIdxToNameB(int id)
	{ return(bs2j_regsb[id&15]); }

int BS2J_RegNameToIdx(char *rn)
{
	int i;
	
	if(*rn=='r')
	{
		for(i=0; i<16; i++)
		{
			if(!strcmp(bs2j_regsq[i], rn))
				return(i);
		}

		for(i=8; i<16; i++)
		{
			if(!strcmp(bs2j_regsd[i], rn))
				return(i);
		}
	}else if(*rn=='e')
	{
		for(i=0; i<8; i++)
		{
			if(!strcmp(bs2j_regsd[i], rn))
				return(i);
		}
	}
	return(-1);
}

char *BS2J_RegNameAsNameD(char *rn)
{
	int i;
	i=BS2J_RegNameToIdx(rn);
	if(i<0)return(rn);
	return(BS2J_RegIdxToNameD(i));
}

char *BS2J_RegNameAsNameW(char *rn)
{
	int i;
	i=BS2J_RegNameToIdx(rn);
	if(i<0)return(rn);
	return(BS2J_RegIdxToNameW(i));
}

char *BS2J_RegNameAsNameB(char *rn)
{
	int i;
	i=BS2J_RegNameToIdx(rn);
	if(i<0)return(rn);
	return(BS2J_RegIdxToNameB(i));
}

int BS2J_FlushLocalRegID(BSVM2_Trace *tr, int idx)
{
	char *s0;
	int i;
	
	if(!tr->jitmp)
		return(-1);
	
	for(i=0; i<16; i++)
	{
		if(tr->jitmp->reg_lclmap[i]==idx)
		{
			if(tr->jitmp->reg_dirty&(1<<i))
			{
				tr->jitmp->reg_dirty&=~(1<<i);
				s0=BS2J_RegIdxToName(i);
				basm_print("mov [rsi+%d], %s\n", idx*8, s0);
			}
			tr->jitmp->reg_lclmap[i]=-1;
		}
	}
	return(0);
}

int BS2J_FlushStackRegID(BSVM2_Trace *tr, int idx)
{
	return(BS2J_FlushLocalRegID(tr, idx+tr->cblk->largs));
}

char *BS2J_GetMemRefLocalI(BSVM2_Trace *tr, int idx)
{
	char tb[256];
	sprintf(tb, "[rsi+%d]", idx*8);
	return(bgbdt_mm_rstrdup(tb));
}

/** Get Local, Read/Write, Mem */
char *BS2J_GetMemRefLocal(BSVM2_Trace *tr, int idx)
{
	BS2J_FlushLocalRegID(tr, idx);
	return(BS2J_GetMemRefLocalI(tr, idx));
}

/** Get Stack, Read/Write, Mem */
char *BS2J_GetMemRefStack(BSVM2_Trace *tr, int idx)
{
//	char tb[256];
//	sprintf(tb, "[rsi+%d]", (idx+tr->cblk->largs)*8);
//	return(bgbdt_mm_rstrdup(tb));
	return(BS2J_GetMemRefLocal(tr, idx+tr->cblk->largs));
}

/** Get Local, Read */
char *BS2J_GetMemRefLocalRd(BSVM2_Trace *tr, int idx)
{
	char *s0;
	int i, j;

	i=BS2J_GetLocalRegSpanID(tr, idx);
	if(i>=0)
	{
		j=tr->jitmp->lclstat_reg[i];

		if(j<0)
			return(BS2J_GetMemRefLocalI(tr, idx));

		if(tr->jitmp->reg_lclmap[j]==idx)
		{
			if(tr->jitmp->opix>=tr->jitmp->lclstat_end[i])
			{
				if(tr->jitmp->reg_dirty&(1<<j))
				{
					s0=BS2J_RegIdxToName(j);
					basm_print("mov [rsi+%d], %s\n", idx*8, s0);
					tr->jitmp->reg_dirty&=~(1<<j);
				}
				tr->jitmp->reg_lclmap[j]=-1;
			}

			s0=BS2J_RegIdxToName(j);
			return(s0);
		}

		if(tr->jitmp->opix<tr->jitmp->lclstat_end[i])
		{
			tr->jitmp->reg_lclmap[j]=idx;
			tr->jitmp->reg_dirty&=~(1<<j);
			s0=BS2J_RegIdxToName(j);
			basm_print("mov %s, [rsi+%d]\n", s0, idx*8);
			return(s0);
		}
	}
	return(BS2J_GetMemRefLocalI(tr, idx));
}

/** Get Local, Write */
char *BS2J_GetMemRefLocalWr(BSVM2_Trace *tr, int idx)
{
	char *s0;
	int i, j;

	i=BS2J_GetLocalRegSpanID(tr, idx);
	if(i>=0)
	{
		j=tr->jitmp->lclstat_reg[i];

		if(j<0)
			return(BS2J_GetMemRefLocalI(tr, idx));

		if(tr->jitmp->opix>=tr->jitmp->lclstat_end[i])
		{
			tr->jitmp->reg_lclmap[j]=-1;
			tr->jitmp->reg_dirty&=~(1<<j);
			return(BS2J_GetMemRefLocalI(tr, idx));
		}
		
		if(tr->jitmp->reg_lclmap[j]==idx)
		{
			tr->jitmp->reg_dirty|=(1<<j);
			s0=BS2J_RegIdxToName(j);
			return(s0);
		}

		if(tr->jitmp->reg_lclmap[j]<0)
		{
			tr->jitmp->reg_lclmap[j]=idx;
			tr->jitmp->reg_dirty|=(1<<j);
			s0=BS2J_RegIdxToName(j);
			return(s0);
		}
	}

	return(BS2J_GetMemRefLocalI(tr, idx));
}

/** Get Local, Read/Write */
char *BS2J_GetMemRefLocalRdWr(BSVM2_Trace *tr, int idx)
{
	char *s0;
	int i, j;

	i=BS2J_GetLocalRegSpanID(tr, idx);
	if(i>=0)
	{
		j=tr->jitmp->lclstat_reg[i];

		if(j<0)
			return(BS2J_GetMemRefLocalI(tr, idx));

		if(tr->jitmp->reg_lclmap[j]==idx)
		{
			if(tr->jitmp->opix>=tr->jitmp->lclstat_end[i])
			{
				s0=BS2J_RegIdxToName(j);
				basm_print("mov [rsi+%d], %s\n", idx*8, s0);

				tr->jitmp->reg_lclmap[j]=-1;
				tr->jitmp->reg_dirty&=~(1<<j);
				return(BS2J_GetMemRefLocalI(tr, idx));
			}

			tr->jitmp->reg_dirty|=(1<<j);
			s0=BS2J_RegIdxToName(j);
			return(s0);
		}

		if(tr->jitmp->reg_lclmap[j]<0)
		{
			tr->jitmp->reg_lclmap[j]=idx;
			tr->jitmp->reg_dirty|=(1<<j);
			s0=BS2J_RegIdxToName(j);
			basm_print("mov %s, [rsi+%d]\n", s0, idx*8);
			return(s0);
		}
	}

	return(BS2J_GetMemRefLocalI(tr, idx));
}


/** Get Local, Read/Write */
char *BS2J_GetMemRefLocalD(BSVM2_Trace *tr, int idx)
{
	char tb[256];
	BS2J_FlushLocalRegID(tr, idx);
	sprintf(tb, "[rsi+%d]", idx*8);
	return(bgbdt_mm_rstrdup(tb));
}

/** Get Stack, Read/Write */
char *BS2J_GetMemRefStackD(BSVM2_Trace *tr, int idx)
{
	return(BS2J_GetMemRefLocalD(tr, idx+tr->cblk->largs));
}

/** Get Local, Read, DW */
char *BS2J_GetMemRefLocalRdD(BSVM2_Trace *tr, int idx)
{
	char *s0;
	int i;
	
	s0=BS2J_GetMemRefLocalRd(tr, idx);
	i=BS2J_RegNameToIdx(s0);
	if(i>=0)
		s0=BS2J_RegIdxToNameD(i);
	return(s0);
//	return(BS2J_GetMemRefLocalD(tr, idx));
}

/** Get Local, Write, DW */
char *BS2J_GetMemRefLocalWrD(BSVM2_Trace *tr, int idx)
{
	char *s0;
	int i;
	
	s0=BS2J_GetMemRefLocalWr(tr, idx);
	i=BS2J_RegNameToIdx(s0);
	if(i>=0)
		s0=BS2J_RegIdxToNameD(i);
	return(s0);

//	return(BS2J_GetMemRefLocalD(tr, idx));
}

/** Get Local, Read/Write, DW */
char *BS2J_GetMemRefLocalRdWrD(BSVM2_Trace *tr, int idx)
{
	char *s0;
	int i;
	
	s0=BS2J_GetMemRefLocalRdWr(tr, idx);
	i=BS2J_RegNameToIdx(s0);
	if(i>=0)
		s0=BS2J_RegIdxToNameD(i);
	return(s0);

//	return(BS2J_GetMemRefLocalD(tr, idx));
}


/** Get Stack, Read */
char *BS2J_GetMemRefStackRd(BSVM2_Trace *tr, int idx)
	{ return(BS2J_GetMemRefLocalRd(tr, idx+tr->cblk->largs)); }
/** Get Stack, Write */
char *BS2J_GetMemRefStackWr(BSVM2_Trace *tr, int idx)
	{ return(BS2J_GetMemRefLocalWr(tr, idx+tr->cblk->largs)); }
/** Get Stack, Read/Write */
char *BS2J_GetMemRefStackRdWr(BSVM2_Trace *tr, int idx)
	{ return(BS2J_GetMemRefLocalRdWr(tr, idx+tr->cblk->largs)); }

/** Get Stack, Read, DW */
char *BS2J_GetMemRefStackRdD(BSVM2_Trace *tr, int idx)
	{ return(BS2J_GetMemRefLocalRdD(tr, idx+tr->cblk->largs)); }
/** Get Stack, Write, DW */
char *BS2J_GetMemRefStackWrD(BSVM2_Trace *tr, int idx)
	{ return(BS2J_GetMemRefLocalWrD(tr, idx+tr->cblk->largs)); }
/** Get Stack, Read/Write, DW */
char *BS2J_GetMemRefStackRdWrD(BSVM2_Trace *tr, int idx)
	{ return(BS2J_GetMemRefLocalRdWrD(tr, idx+tr->cblk->largs)); }


/** Get Stack, Read, XMM */
char *BS2J_GetMemRefStackRdX(BSVM2_Trace *tr, int idx)
{
	return(BS2J_GetMemRefStack(tr, idx));
}

/** Get Stack, Write, XMM */
char *BS2J_GetMemRefStackWrX(BSVM2_Trace *tr, int idx)
{
	return(BS2J_GetMemRefStack(tr, idx));
}

#if 0
char *BS2J_GetMemRefLocalP(BSVM2_Trace *tr, int idx, int ofs)
{
	char tb[256];
	sprintf(tb, "[rsi+%d]", idx*8+ofs);
	return(bgbdt_mm_rstrdup(tb));
}

char *BS2J_GetMemRefStackP(BSVM2_Trace *tr, int idx, int ofs)
{
	char tb[256];
	sprintf(tb, "[rsi+%d]", (idx+tr->cblk->largs)*8+ofs);
	return(bgbdt_mm_rstrdup(tb));
}
#endif

int BS2J_JitRefIsRegP(BSVM2_Trace *tr, char *dst)
{
	if(*dst=='[')
		return(0);
	return(1);
}

int BS2J_JitBasicMov(BSVM2_Trace *tr, char *dst, char *src)
{
	if(!strcmp(dst, src))
		return(0);
	
	if(BS2J_JitRefIsRegP(tr, dst) ||
		BS2J_JitRefIsRegP(tr, src))
	{
		basm_print("mov %s, %s\n", dst, src);
		return(0);
	}
	
	basm_print("mov rax, %s\n", src);
	basm_print("mov %s, rax\n", dst);
	return(0);
}

int BS2J_JitBasicMovD(BSVM2_Trace *tr, char *dst, char *src)
{
	if(!strcmp(dst, src))
		return(0);
	
	if(BS2J_JitRefIsRegP(tr, dst) ||
		BS2J_JitRefIsRegP(tr, src))
	{
		basm_print("mov %s, %s\n", dst, src);
		return(0);
	}
	
	basm_print("mov eax, %s\n", src);
	basm_print("mov %s, eax\n", dst);
	return(0);
}

int BS2J_TryJitOpcode(BSVM2_Trace *tr, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	char *s0, *s1, *s2, *s3;
	int i, j, k;

	if(op->opn==BSVM2_OP_LABEL)
	{
		return(1);
	}

//	return(0);

#if 1
	if(	(op->opn==BSVM2_OP_LDI) ||
		(op->opn==BSVM2_OP_LDF))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRdD(tr, op->i0);
		s1=BS2J_GetMemRefStackWrD(tr, op->t0);
//		basm_print("mov eax, %s\n", s0);
//		basm_print("mov %s, eax\n", s1);
		BS2J_JitBasicMovD(tr, s1, s0);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_LDL) ||
		(op->opn==BSVM2_OP_LDD) ||
		(op->opn==BSVM2_OP_LDA))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRd(tr, op->i0);
		s1=BS2J_GetMemRefStackWr(tr, op->t0);
//		basm_print("mov rax, %s\n", s0);
//		basm_print("mov %s, rax\n", s1);
		BS2J_JitBasicMov(tr, s1, s0);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_STI) ||
		(op->opn==BSVM2_OP_STF))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		s1=BS2J_GetMemRefLocalWrD(tr, op->i0);
//		basm_print("mov eax, %s\n", s0);
//		basm_print("mov %s, eax\n", s1);
		BS2J_JitBasicMovD(tr, s1, s0);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_STL) ||
		(op->opn==BSVM2_OP_STD) ||
		(op->opn==BSVM2_OP_STA))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackRd(tr, op->t0);
		s1=BS2J_GetMemRefLocalWr(tr, op->i0);
//		basm_print("mov rax, %s\n", s0);
//		basm_print("mov %s, rax\n", s1);
		BS2J_JitBasicMov(tr, s1, s0);
		return(1);
	}
#endif

#if 1
	if(	(op->opn==BSVM2_OP_MVI) ||
		(op->opn==BSVM2_OP_MVF))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRdD(tr, op->i1);
		s1=BS2J_GetMemRefLocalWrD(tr, op->i0);
//		basm_print("mov eax, %s\n", s0);
//		basm_print("mov %s, eax\n", s1);
		BS2J_JitBasicMovD(tr, s1, s0);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_MVL) ||
		(op->opn==BSVM2_OP_MVD) ||
		(op->opn==BSVM2_OP_MVA))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRd(tr, op->i1);
		s1=BS2J_GetMemRefLocalWr(tr, op->i0);
//		basm_print("mov rax, %s\n", s0);
//		basm_print("mov %s, rax\n", s1);
		BS2J_JitBasicMov(tr, s1, s0);
		return(1);
	}

	if(op->opn==BSVM2_OP_MVIC)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalWrD(tr, op->i0);
		
		if(BS2J_JitRefIsRegP(tr, s0))
		{
			basm_print("mov %s, 0x%08X\n", s0, op->v.i);
			return(1);
		}
		
//		basm_print("mov %s, 0x%08X\n", s0, op->v.i);
		basm_print("mov eax, 0x%08X\n", op->v.i);
		basm_print("mov %s, eax\n", s0);
		return(1);
	}

	if(op->opn==BSVM2_OP_MVLC)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalWr(tr, op->i0);

		if(BS2J_JitRefIsRegP(tr, s0))
		{
			basm_print("mov %s, 0x%016llX\n", s0, op->v.l);
			return(1);
		}

		basm_print("mov rax, 0x%016llX\n", op->v.l);
		basm_print("mov %s, rax\n", s0);
		return(1);
	}
#endif

#if 1
	if(op->opn==BSVM2_OP_LDC)
	{
		if((op->z==BSVM2_OPZ_INT) ||
			(op->z==BSVM2_OPZ_UINT) ||
			(op->z==BSVM2_OPZ_SHORT) ||
			(op->z==BSVM2_OPZ_USHORT) ||
			(op->z==BSVM2_OPZ_SBYTE) ||
			(op->z==BSVM2_OPZ_UBYTE) ||
			(op->z==BSVM2_OPZ_FLOAT))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackWrD(tr, op->t0);
//			basm_print("mov %s, 0x%08X\n", s0, op->v.i);

			if(BS2J_JitRefIsRegP(tr, s0))
			{
				if(!op->v.i)
				{
					basm_print("xor %s, %s\n", s0, s0);
					return(1);
				}

				basm_print("mov %s, 0x%08X\n", s0, op->v.i);
				return(1);
			}

			basm_print("mov eax, 0x%08X\n", op->v.i);
			basm_print("mov %s, eax\n", s0);
			return(1);
		}

		if(	(op->z==BSVM2_OPZ_LONG) ||
			(op->z==BSVM2_OPZ_ULONG) ||
			(op->z==BSVM2_OPZ_NLONG) ||
			(op->z==BSVM2_OPZ_UNLONG) ||
			(op->z==BSVM2_OPZ_DOUBLE) ||
			(op->z==BSVM2_OPZ_ADDRESS) ||
			((op->z==BSVM2_OPZ_CONST) && (op->o!=4)))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackWr(tr, op->t0);

			if(BS2J_JitRefIsRegP(tr, s0))
			{
				if(!op->v.l)
				{
					basm_print("xor %s, %s\n", s0, s0);
					return(1);
				}

				basm_print("mov %s, 0x%016llX\n", s0, op->v.l);
				return(1);
			}

			basm_print("mov rax, 0x%016llX\n", op->v.l);
			basm_print("mov %s, rax\n", s0);
			return(1);
		}
	}
#endif

//	return(0);

#if 1
	if(op->opn==BSVM2_OP_LDCA)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackWr(tr, op->t0);

		if(BS2J_JitRefIsRegP(tr, s0))
		{
			basm_print("mov %s, 0x%016llX\n", s0, op->v.l);
			return(1);
		}

		basm_print("mov rax, 0x%016llX\n", op->v.l);
		basm_print("mov %s, rax\n", s0);
		return(1);
	}

	if((op->opn==BSVM2_OP_POPI) ||
		(op->opn==BSVM2_OP_POPL) ||
		(op->opn==BSVM2_OP_POPF) ||
		(op->opn==BSVM2_OP_POPD) ||
		(op->opn==BSVM2_OP_POPA))
	{
		return(1);
	}

	if((op->opn==BSVM2_OP_PUSHI) ||
		(op->opn==BSVM2_OP_PUSHL) ||
		(op->opn==BSVM2_OP_PUSHF) ||
		(op->opn==BSVM2_OP_PUSHD) ||
		(op->opn==BSVM2_OP_PUSHA))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackWr(tr, op->t0);

		if(BS2J_JitRefIsRegP(tr, s0))
		{
			basm_print("xor %s, %s\n", s0, s0);
			return(1);
		}

		basm_print("xor rax, rax\n");
		basm_print("mov %s, rax\n", s0);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_DUPI) ||
		(op->opn==BSVM2_OP_DUPF))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackRdD(tr, op->t1);
		s1=BS2J_GetMemRefStackWrD(tr, op->t0);
//		basm_print("mov eax, %s\n", s0);
//		basm_print("mov %s, eax\n", s1);
		BS2J_JitBasicMovD(tr, s1, s0);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_DUPL) ||
		(op->opn==BSVM2_OP_DUPD) ||
		(op->opn==BSVM2_OP_DUPA))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackRd(tr, op->t1);
		s1=BS2J_GetMemRefStackWr(tr, op->t0);
//		basm_print("mov rax, %s\n", s0);
//		basm_print("mov %s, rax\n", s1);
		BS2J_JitBasicMov(tr, s1, s0);
		return(1);
	}
#endif

//	return(0);

#if 1
	if(	(op->opn==BSVM2_OP_ADDI)||
		(op->opn==BSVM2_OP_SUBI)||
		(op->opn==BSVM2_OP_MULI)||
		(op->opn==BSVM2_OP_ANDI)||
		(op->opn==BSVM2_OP_ORI)||
		(op->opn==BSVM2_OP_XORI))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s1=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		s0=BS2J_GetMemRefStackRdD(tr, op->t1);

		if(BS2J_JitRefIsRegP(tr, s1))
		{
			if(op->opn==BSVM2_OP_ADDI)
				{ basm_print("add %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_SUBI)
				{ basm_print("sub %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_MULI)
				{ basm_print("imul %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_ANDI)
				{ basm_print("and %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_ORI)
				{ basm_print("or %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_XORI)
				{ basm_print("xor %s, %x\n", s1, s0); }
			return(1);
		}

		basm_print("mov eax, %s\n", s1);

		if(op->opn==BSVM2_OP_ADDI)
			{ basm_print("add eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_SUBI)
			{ basm_print("sub eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_MULI)
			{ basm_print("imul eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_ANDI)
			{ basm_print("and eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_ORI)
			{ basm_print("or eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_XORI)
			{ basm_print("xor eax, %x\n", s0); }

		basm_print("mov %s, eax\n", s1);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_ADDIC)||
		(op->opn==BSVM2_OP_SUBIC)||
		(op->opn==BSVM2_OP_MULIC)||
		(op->opn==BSVM2_OP_ANDIC)||
		(op->opn==BSVM2_OP_ORIC)||
		(op->opn==BSVM2_OP_XORIC))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);

		if(BS2J_JitRefIsRegP(tr, s0))
		{
			if(op->opn==BSVM2_OP_ADDIC)
				{ basm_print("add %s, %d\n", s0, op->v.i); }
			else if(op->opn==BSVM2_OP_SUBIC)
				{ basm_print("sub %s, %d\n", s0, op->v.i); }
			else if(op->opn==BSVM2_OP_MULIC)
				{ basm_print("imul %s, %d\n", s0, op->v.i); }
			else if(op->opn==BSVM2_OP_ANDIC)
				{ basm_print("and %s, %d\n", s0, op->v.i); }
			else if(op->opn==BSVM2_OP_ORIC)
				{ basm_print("or %s, %d\n", s0, op->v.i); }
			else if(op->opn==BSVM2_OP_XORIC)
				{ basm_print("xor %s, %d\n", s0, op->v.i); }
			return(1);
		}

		basm_print("mov eax, %s\n", s0);
		if(op->opn==BSVM2_OP_ADDIC)
			{ basm_print("add eax, %d\n", op->v.i); }
		else if(op->opn==BSVM2_OP_SUBIC)
			{ basm_print("sub eax, %d\n", op->v.i); }
		else if(op->opn==BSVM2_OP_MULIC)
			{ basm_print("imul eax, %d\n", op->v.i); }
		else if(op->opn==BSVM2_OP_ANDIC)
			{ basm_print("and eax, %d\n", op->v.i); }
		else if(op->opn==BSVM2_OP_ORIC)
			{ basm_print("or eax, %d\n", op->v.i); }
		else if(op->opn==BSVM2_OP_XORIC)
			{ basm_print("xor eax, %d\n", op->v.i); }
		basm_print("mov %s, eax\n", s0);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_ADDIL)||
		(op->opn==BSVM2_OP_SUBIL)||
		(op->opn==BSVM2_OP_MULIL)||
		(op->opn==BSVM2_OP_ANDIL)||
		(op->opn==BSVM2_OP_ORIL)||
		(op->opn==BSVM2_OP_XORIL))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s1=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		s0=BS2J_GetMemRefLocalRdD(tr, op->i0);

		if(BS2J_JitRefIsRegP(tr, s1))
		{
			if(op->opn==BSVM2_OP_ADDIL)
				{ basm_print("add %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_SUBIL)
				{ basm_print("sub %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_MULIL)
				{ basm_print("imul %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_ANDIL)
				{ basm_print("and %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_ORIL)
				{ basm_print("or %s, %s\n", s1, s0); }
			else if(op->opn==BSVM2_OP_XORIL)
				{ basm_print("xor %s, %x\n", s1, s0); }
			return(1);
		}

		basm_print("mov eax, %s\n", s1);

		if(op->opn==BSVM2_OP_ADDIL)
			{ basm_print("add eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_SUBIL)
			{ basm_print("sub eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_MULIL)
			{ basm_print("imul eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_ANDIL)
			{ basm_print("and eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_ORIL)
			{ basm_print("or eax, %s\n", s0); }
		else if(op->opn==BSVM2_OP_XORIL)
			{ basm_print("xor eax, %x\n", s0); }

		basm_print("mov %s, eax\n", s1);
		return(1);
	}
#endif

//	return(0);

#if 1
	if(	(op->opn==BSVM2_OP_SHLI)||
		(op->opn==BSVM2_OP_SARI)||
		(op->opn==BSVM2_OP_SHRI))
	{
		BS2J_CheckCacheLocalsESI(tr);
//		s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		s1=BS2J_GetMemRefStackRdD(tr, op->t1);
		basm_print("mov eax, %s\n", s0);
		basm_print("mov ecx, %s\n", s1);

		if(op->opn==BSVM2_OP_SHLI)
			{ basm_print("shl eax, cl\n"); }
		else if(op->opn==BSVM2_OP_SARI)
			{ basm_print("sar eax, cl\n"); }
		else if(op->opn==BSVM2_OP_SHRI)
			{ basm_print("shr eax, cl\n"); }

		s2=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("mov %s, eax\n", s2);
		return(1);
	}

	if((op->opn==BSVM2_OP_SHLIC) ||
		(op->opn==BSVM2_OP_SARIC))
	{
		if((op->v.i>(-32)) && (op->v.i<32))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);
//			s0=BS2J_GetMemRefStackRdD(tr, op->t0);
			s1=s0;

			if(BS2J_JitRefIsRegP(tr, s0))
			{
				if((op->opn==BSVM2_OP_SHLIC) && (op->v.i>=0))
					{ basm_print("shl %s, %d\n", s0, op->v.i); }
				else if((op->opn==BSVM2_OP_SARIC) && (op->v.i>=0))
					{ basm_print("sar %s, %d\n", s0, op->v.i); }
				else if((op->opn==BSVM2_OP_SHLIC) && (op->v.i<0))
					{ basm_print("shr %s, %d\n", s0, -op->v.i); }
				else if((op->opn==BSVM2_OP_SARIC) && (op->v.i<0))
					{ basm_print("shl %s, %d\n", s0, -op->v.i); }
				return(1);
			}

			basm_print("mov eax, %s\n", s0);
			if((op->opn==BSVM2_OP_SHLIC) && (op->v.i>=0))
				{ basm_print("shl eax, %d\n", op->v.i); }
			else if((op->opn==BSVM2_OP_SARIC) && (op->v.i>=0))
				{ basm_print("sar eax, %d\n", op->v.i); }
			else if((op->opn==BSVM2_OP_SHLIC) && (op->v.i<0))
				{ basm_print("shr eax, %d\n", -op->v.i); }
			else if((op->opn==BSVM2_OP_SARIC) && (op->v.i<0))
				{ basm_print("shl eax, %d\n", -op->v.i); }
//			s1=BS2J_GetMemRefStackWrD(tr, op->t0);
			basm_print("mov %s, eax\n", s1);
			return(1);
		}
	}
#endif

//	return(0);

#if 1
	if(	(op->opn==BSVM2_OP_SHLIL)||
		(op->opn==BSVM2_OP_SARIL))
	{
		BS2J_CheckCacheLocalsESI(tr);
//		s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		s1=BS2J_GetMemRefLocalRdD(tr, op->i0);
		basm_print("mov eax, %s\n", s0);
		basm_print("mov ecx, %s\n", s1);
		if(op->opn==BSVM2_OP_SHLIL)
			{ basm_print("shl eax, cl\n"); }
		else if(op->opn==BSVM2_OP_SARIL)
			{ basm_print("sar eax, cl\n"); }
		s2=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("mov %s, eax\n", s2);
		return(1);
	}

	if(op->opn==BSVM2_OP_INCI)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		basm_print("inc dword %s\n", s0);
		return(1);
	}

	if(op->opn==BSVM2_OP_INCIL)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRdWrD(tr, op->i0);
		basm_print("inc dword %s\n", s0);
		return(1);
	}

	if(op->opn==BSVM2_OP_DECI)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		basm_print("dec dword %s\n", s0);
		return(1);
	}

	if(op->opn==BSVM2_OP_DECIL)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRdWrD(tr, op->i0);
		basm_print("dec dword %s\n", s0);
		return(1);
	}

//	return(0);

	if(op->opn==BSVM2_OP_CMPI)
	{
		BS2J_CheckCacheLocalsESI(tr);
		
//		s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		s1=BS2J_GetMemRefStackRdD(tr, op->t1);
		
		basm_print("mov eax, %s\n", s0);
		basm_print("mov ecx, %s\n", s1);
		basm_print("sub eax, ecx\n");
		basm_print("sub ecx, %s\n", s0);
		basm_print("sar eax, 31\n");
		basm_print("shr ecx, 31\n");
		basm_print("or eax, ecx\n");
		s2=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("mov %s, eax\n", s2);
		return(1);
	}

	if(op->opn==BSVM2_OP_CMPIC)
	{
		BS2J_CheckCacheLocalsESI(tr);
//		s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		basm_print("mov eax, %s\n", s0);
		basm_print("mov ecx, %d\n", op->v.i);
		basm_print("sub eax, ecx\n");
		basm_print("sub ecx, %s\n", s0);
		basm_print("sar eax, 31\n");
		basm_print("shr ecx, 31\n");
		basm_print("or eax, ecx\n");
		s1=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("mov %s, eax\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CMPIL)
	{
		BS2J_CheckCacheLocalsESI(tr);
//		s0=BS2J_GetMemRefStackRdWrD(tr, op->t0);
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		s1=BS2J_GetMemRefLocalRdD(tr, op->i0);
		basm_print("mov eax, %s\n", s0);
		basm_print("mov ecx, %s\n", s1);
		basm_print("sub eax, ecx\n");
		basm_print("sub ecx, %s\n", s0);
		basm_print("sar eax, 31\n");
		basm_print("shr ecx, 31\n");
		basm_print("or eax, ecx\n");
		s2=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("mov %s, eax\n", s2);
		return(1);
	}

	if(op->opn==BSVM2_OP_CMPILL)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRdD(tr, op->i0);
		s1=BS2J_GetMemRefLocalRdD(tr, op->i1);
		basm_print("mov eax, %s\n", s0);
		basm_print("mov ecx, %s\n", s1);
		basm_print("sub eax, ecx\n");
		basm_print("sub ecx, %s\n", s0);
		basm_print("sar eax, 31\n");
		basm_print("shr ecx, 31\n");
		basm_print("or eax, ecx\n");
		s2=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("mov %s, eax\n", s2);
		return(1);
	}

	if(op->opn==BSVM2_OP_CMPILC)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRdD(tr, op->i0);
		basm_print("mov eax, %s\n", s0);
		basm_print("mov ecx, %d\n", op->v.i);
		basm_print("sub eax, ecx\n");
		basm_print("sub ecx, %s\n", s0);
		basm_print("sar eax, 31\n");
		basm_print("shr ecx, 31\n");
		basm_print("or eax, ecx\n");
		s1=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("mov %s, eax\n", s1);
		return(1);
	}
#endif

//	return(0);

#if 1
	if(	(op->opn==BSVM2_OP_CVTSB2I) ||
		(op->opn==BSVM2_OP_CVTUB2I) ||
		(op->opn==BSVM2_OP_CVTSS2I) ||
		(op->opn==BSVM2_OP_CVTUS2I))
	{
		BS2J_CheckCacheLocalsESI(tr);
		
		if(op->opn==BSVM2_OP_CVTSB2I)
			s0="movsxb";
		if(op->opn==BSVM2_OP_CVTUB2I)
			s0="movzxb";
		if(op->opn==BSVM2_OP_CVTSS2I)
			s0="movsxw";
		if(op->opn==BSVM2_OP_CVTUS2I)
			s0="movzxw";
		
		s1=BS2J_GetMemRefStackRdWr(tr, op->t0);

		i=BS2J_RegNameToIdx(s1);
		if(i>=0)
		{
			if(	(op->opn==BSVM2_OP_CVTSS2I) ||
				(op->opn==BSVM2_OP_CVTUS2I))
					s1=BS2J_RegIdxToNameW(i);
			if(	(op->opn==BSVM2_OP_CVTSB2I) ||
				(op->opn==BSVM2_OP_CVTUB2I))
					s1=BS2J_RegIdxToNameB(i);
			s2=BS2J_RegIdxToNameD(i);

			basm_print("%s %s, %s\n", s0, s2, s1);
			return(1);
		}else
		{
			s2=s1;
		}
		
		basm_print("%s eax, %s\n", s0, s1);

//		s2=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("mov %s, eax\n", s2);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTI2L)
	{
		s0=BS2J_GetMemRefStackRd(tr, op->t0);
		s1=BS2J_GetMemRefStackWr(tr, op->t0);
		basm_print("movsxd rax, %s\n", s0);
		basm_print("mov %s, rax\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTI2F)
	{
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		s1=BS2J_GetMemRefStackWrX(tr, op->t0);
		basm_print("mov eax, %s\n", s0);
		basm_print("cvtsi2ss xmm0, eax\n");
		basm_print("movss %s, xmm0\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTI2D)
	{
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		s1=BS2J_GetMemRefStackWrX(tr, op->t0);
		basm_print("mov eax, %s\n", s0);
		basm_print("cvtsi2sd xmm0, eax\n");
		basm_print("movsd %s, xmm0\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTL2I)
	{
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTL2F)
	{
		s0=BS2J_GetMemRefStackRd(tr, op->t0);
		s1=BS2J_GetMemRefStackWrX(tr, op->t0);
		basm_print("mov rax, %s\n", s0);
		basm_print("cvtsi2ss xmm0, rax\n");
		basm_print("movss %s, xmm0\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTL2D)
	{
		s0=BS2J_GetMemRefStackRd(tr, op->t0);
		s1=BS2J_GetMemRefStackWrX(tr, op->t0);
		basm_print("mov rax, %s\n", s0);
		basm_print("cvtsi2sd xmm0, rax\n");
		basm_print("movsd %s, xmm0\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTF2I)
	{
		s0=BS2J_GetMemRefStackRdX(tr, op->t0);
		s1=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("cvtss2si eax, %s\n", s0);
		basm_print("mov %s, eax\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTF2L)
	{
		s0=BS2J_GetMemRefStackRdX(tr, op->t0);
		s1=BS2J_GetMemRefStackWr(tr, op->t0);
		basm_print("cvtss2si rax, %s\n", s0);
		basm_print("mov %s, rax\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTF2D)
	{
		s0=BS2J_GetMemRefStackRdX(tr, op->t0);
		s1=BS2J_GetMemRefStackWrX(tr, op->t0);
		basm_print("cvtss2sd xmm0, %s\n", s0);
		basm_print("movsd %s, xmm0\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTD2I)
	{
		s0=BS2J_GetMemRefStackRdX(tr, op->t0);
		s1=BS2J_GetMemRefStackWrD(tr, op->t0);
		basm_print("cvtsd2si eax, %s\n", s0);
		basm_print("mov %s, eax\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTD2L)
	{
		s0=BS2J_GetMemRefStackRdX(tr, op->t0);
		s1=BS2J_GetMemRefStackWr(tr, op->t0);
		basm_print("cvtsd2si rax, %s\n", s0);
		basm_print("mov %s, rax\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTD2F)
	{
		s0=BS2J_GetMemRefStackRdX(tr, op->t0);
		s1=BS2J_GetMemRefStackWrX(tr, op->t0);
		basm_print("cvtsd2ss xmm0, %s\n", s0);
		basm_print("movss %s, xmm0\n", s1);
		return(1);
	}

	if(op->opn==BSVM2_OP_CVTUI2L)
	{
//		basm_print("movzxd rax, %s\n",
//			BS2J_GetMemRefStack(tr, op->t0));
		s0=BS2J_GetMemRefStackRdD(tr, op->t0);
		s1=BS2J_GetMemRefStackWr(tr, op->t0);
		basm_print("mov eax, %s\n", s0);
		basm_print("mov %s, rax\n", s1);
		return(1);
	}
#endif

//	return(0);

#if 1
	if(	(op->opn==BSVM2_OP_ADDISLL)||
		(op->opn==BSVM2_OP_SUBISLL)||
		(op->opn==BSVM2_OP_MULISLL)||
		(op->opn==BSVM2_OP_ANDISLL)||
		(op->opn==BSVM2_OP_ORISLL) ||
		(op->opn==BSVM2_OP_XORISLL)||
		(op->opn==BSVM2_OP_ADDILSL)||
		(op->opn==BSVM2_OP_SUBILSL)||
		(op->opn==BSVM2_OP_MULILSL)||
		(op->opn==BSVM2_OP_ANDILSL)||
		(op->opn==BSVM2_OP_ORILSL) ||
		(op->opn==BSVM2_OP_XORILSL)||
		(op->opn==BSVM2_OP_ADDILLL)||
		(op->opn==BSVM2_OP_SUBILLL)||
		(op->opn==BSVM2_OP_MULILLL)||
		(op->opn==BSVM2_OP_ANDILLL)||
		(op->opn==BSVM2_OP_ORILLL) ||
		(op->opn==BSVM2_OP_XORILLL)
		)
	{
		BS2J_CheckCacheLocalsESI(tr);
		s1=BS2J_GetMemRefLocalRdD(tr, op->i1);
//		s1=BS2J_GetMemRefLocalRdWrD(tr, op->i1);
		s0=BS2J_GetMemRefLocalRdD(tr, op->i2);
		s2=BS2J_GetMemRefLocalWrD(tr, op->i0);

#if 1
		if(BS2J_JitRefIsRegP(tr, s2))
		{
			basm_print("mov %s, %s\n", s2, s1);
			if(		(op->opn==BSVM2_OP_ADDISLL) ||
					(op->opn==BSVM2_OP_ADDILSL) ||
					(op->opn==BSVM2_OP_ADDILLL))
				{ basm_print("add %s, %s\n", s2, s0); }
			else if(	(op->opn==BSVM2_OP_SUBISLL) ||
						(op->opn==BSVM2_OP_SUBILSL) ||
						(op->opn==BSVM2_OP_SUBILLL))
				{ basm_print("sub %s, %s\n", s2, s0); }
			else if(	(op->opn==BSVM2_OP_MULISLL) ||
						(op->opn==BSVM2_OP_MULILSL) ||
						(op->opn==BSVM2_OP_MULILLL))
				{ basm_print("imul %s, %s\n", s2, s0); }
			else if(	(op->opn==BSVM2_OP_ANDISLL) ||
						(op->opn==BSVM2_OP_ANDILSL) ||
						(op->opn==BSVM2_OP_ANDILLL))
				{ basm_print("and %s, %s\n", s2, s0); }
			else if(	(op->opn==BSVM2_OP_ORISLL) ||
						(op->opn==BSVM2_OP_ORILSL) ||
						(op->opn==BSVM2_OP_ORILLL))
				{ basm_print("or %s, %s\n", s2, s0); }
			else if(	(op->opn==BSVM2_OP_XORISLL) ||
						(op->opn==BSVM2_OP_XORILSL) ||
						(op->opn==BSVM2_OP_XORILLL))
				{ basm_print("xor %s, %x\n", s2, s0); }
			return(1);
		}
#endif

		basm_print("mov eax, %s\n", s1);

//		s0=BS2J_GetMemRefLocalRdD(tr, op->i2);
		if(		(op->opn==BSVM2_OP_ADDISLL) ||
				(op->opn==BSVM2_OP_ADDILSL) ||
				(op->opn==BSVM2_OP_ADDILLL))
			{ basm_print("add eax, %s\n", s0); }
		else if(	(op->opn==BSVM2_OP_SUBISLL) ||
					(op->opn==BSVM2_OP_SUBILSL) ||
					(op->opn==BSVM2_OP_SUBILLL))
			{ basm_print("sub eax, %s\n", s0); }
		else if(	(op->opn==BSVM2_OP_MULISLL) ||
					(op->opn==BSVM2_OP_MULILSL) ||
					(op->opn==BSVM2_OP_MULILLL))
			{ basm_print("imul eax, %s\n", s0); }
		else if(	(op->opn==BSVM2_OP_ANDISLL) ||
					(op->opn==BSVM2_OP_ANDILSL) ||
					(op->opn==BSVM2_OP_ANDILLL))
			{ basm_print("and eax, %s\n", s0); }
		else if(	(op->opn==BSVM2_OP_ORISLL) ||
					(op->opn==BSVM2_OP_ORILSL) ||
					(op->opn==BSVM2_OP_ORILLL))
			{ basm_print("or eax, %s\n", s0); }
		else if(	(op->opn==BSVM2_OP_XORISLL) ||
					(op->opn==BSVM2_OP_XORILSL) ||
					(op->opn==BSVM2_OP_XORILLL))
			{ basm_print("xor eax, %x\n", s0); }

//		s2=BS2J_GetMemRefLocalWrD(tr, op->i0);
		basm_print("mov %s, eax\n", s2);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_ADDISLC)||
		(op->opn==BSVM2_OP_SUBISLC)||
		(op->opn==BSVM2_OP_MULISLC)||
		(op->opn==BSVM2_OP_ANDISLC)||
		(op->opn==BSVM2_OP_ORISLC) ||
		(op->opn==BSVM2_OP_XORISLC)||
		(op->opn==BSVM2_OP_ADDILSC)||
		(op->opn==BSVM2_OP_SUBILSC)||
		(op->opn==BSVM2_OP_MULILSC)||
		(op->opn==BSVM2_OP_ANDILSC)||
		(op->opn==BSVM2_OP_ORILSC) ||
		(op->opn==BSVM2_OP_XORILSC)||
		(op->opn==BSVM2_OP_ADDILLC)||
		(op->opn==BSVM2_OP_SUBILLC)||
		(op->opn==BSVM2_OP_MULILLC)||
		(op->opn==BSVM2_OP_ANDILLC)||
		(op->opn==BSVM2_OP_ORILLC)||
		(op->opn==BSVM2_OP_XORILLC)
		)
	{
		BS2J_CheckCacheLocalsESI(tr);
//		s0=BS2J_GetMemRefLocalRdWrD(tr, op->i1);
		s0=BS2J_GetMemRefLocalRdD(tr, op->i1);
		s1=BS2J_GetMemRefLocalWrD(tr, op->i0);

#if 1
		if(BS2J_JitRefIsRegP(tr, s1))
		{
			basm_print("mov %s, %s\n", s1, s0);
			if(		(op->opn==BSVM2_OP_ADDISLC) ||
					(op->opn==BSVM2_OP_ADDILSC) ||
					(op->opn==BSVM2_OP_ADDILLC))
				{ basm_print("add %s, %d\n", s1, op->v.i); }
			else if(	(op->opn==BSVM2_OP_SUBISLC) ||
						(op->opn==BSVM2_OP_SUBILSC) ||
						(op->opn==BSVM2_OP_SUBILLC))
				{ basm_print("sub %s, %d\n", s1, op->v.i); }
			else if(	(op->opn==BSVM2_OP_MULISLC) ||
						(op->opn==BSVM2_OP_MULILSC) ||
						(op->opn==BSVM2_OP_MULILLC))
				{ basm_print("imul %s, %d\n", s1, op->v.i); }
			else if(	(op->opn==BSVM2_OP_ANDISLC) ||
						(op->opn==BSVM2_OP_ANDILSC) ||
						(op->opn==BSVM2_OP_ANDILLC))
				{ basm_print("and %s, %d\n", s1, op->v.i); }
			else if(	(op->opn==BSVM2_OP_ORISLC) ||
						(op->opn==BSVM2_OP_ORILSC) ||
						(op->opn==BSVM2_OP_ORILLC))
				{ basm_print("or %s, %d\n", s1, op->v.i); }
			else if(	(op->opn==BSVM2_OP_XORISLC) ||
						(op->opn==BSVM2_OP_XORILSC) ||
						(op->opn==BSVM2_OP_XORILLC))
				{ basm_print("xor %s, %d\n", s1, op->v.i); }
			return(1);
		}
#endif
		
		basm_print("mov eax, %s\n", s0);
		if(		(op->opn==BSVM2_OP_ADDISLC) ||
				(op->opn==BSVM2_OP_ADDILSC) ||
				(op->opn==BSVM2_OP_ADDILLC))
			{ basm_print("add eax, %d\n", op->v.i); }
		else if(	(op->opn==BSVM2_OP_SUBISLC) ||
					(op->opn==BSVM2_OP_SUBILSC) ||
					(op->opn==BSVM2_OP_SUBILLC))
			{ basm_print("sub eax, %d\n", op->v.i); }
		else if(	(op->opn==BSVM2_OP_MULISLC) ||
					(op->opn==BSVM2_OP_MULILSC) ||
					(op->opn==BSVM2_OP_MULILLC))
			{ basm_print("imul eax, %d\n", op->v.i); }
		else if(	(op->opn==BSVM2_OP_ANDISLC) ||
					(op->opn==BSVM2_OP_ANDILSC) ||
					(op->opn==BSVM2_OP_ANDILLC))
			{ basm_print("and eax, %d\n", op->v.i); }
		else if(	(op->opn==BSVM2_OP_ORISLC) ||
					(op->opn==BSVM2_OP_ORILSC) ||
					(op->opn==BSVM2_OP_ORILLC))
			{ basm_print("or eax, %d\n", op->v.i); }
		else if(	(op->opn==BSVM2_OP_XORISLC) ||
					(op->opn==BSVM2_OP_XORILSC) ||
					(op->opn==BSVM2_OP_XORILLC))
			{ basm_print("xor eax, %d\n", op->v.i); }

		basm_print("mov %s, eax\n", s1);
		return(1);
	}
#endif

//	return(0);

#if 1
	if(op->opn==BSVM2_OP_HPTR)
	{
		if(	(op->opn2==BSVM2_OP_LDIXI) ||
			(op->opn2==BSVM2_OP_LDIXL) ||
			(op->opn2==BSVM2_OP_LDIXF) ||
			(op->opn2==BSVM2_OP_LDIXD) ||
			(op->opn2==BSVM2_OP_LDIXA) ||
			(op->opn2==BSVM2_OP_LDIXSS) ||
			(op->opn2==BSVM2_OP_LDIXUS) ||
			(op->opn2==BSVM2_OP_LDIXSB) ||
			(op->opn2==BSVM2_OP_LDIXUB))
		{
			BS2J_CheckCacheLocalsESI(tr);

			s0=BS2J_GetMemRefStackRd(tr, op->t0);
			s1=BS2J_GetMemRefStackRdD(tr, op->t1);
			s2=BS2J_GetMemRefStackWr(tr, op->t0);

			if(BS2J_JitRefIsRegP(tr, s1) &&
				BS2J_JitRefIsRegP(tr, s2) &&
				BS2J_JitRefIsRegP(tr, s0))
			{

				if((op->opn2==BSVM2_OP_LDIXI) ||
					(op->opn2==BSVM2_OP_LDIXF))
				{
					s2=BS2J_RegNameAsNameD(s2);
					basm_print("movsxd rcx, %s\n", s1);
					basm_print("mov %s, [%s+rcx*4]\n", s2, s0);
					return(1);
				}

				if((op->opn2==BSVM2_OP_LDIXL) ||
					(op->opn2==BSVM2_OP_LDIXD) ||
					(op->opn2==BSVM2_OP_LDIXA))
				{
					basm_print("movsxd rcx, %s\n", s1);
					basm_print("mov %s, [%s+rcx*4]\n", s2, s0);
					return(1);
				}

				s3=NULL;
				if(op->opn2==BSVM2_OP_LDIXSS) { s3="movsxw"; j=2; }
				if(op->opn2==BSVM2_OP_LDIXUS) { s3="movzxw"; j=2; }
				if(op->opn2==BSVM2_OP_LDIXSB) { s3="movsxb"; j=1; }
				if(op->opn2==BSVM2_OP_LDIXUB) { s3="movzxb"; j=1; }
				if(s3)
				{
					s2=BS2J_RegNameAsNameD(s2);
					if(j>1)
					{
						basm_print("movsxd rcx, %s\n", s1);
						basm_print("%s %s, [%s+rcx*%d]\n",
							s3, s2, s0, j);
					}
					else
					{
						basm_print("movsxd rcx, %s\n", s1);
						basm_print("%s %s, [%s+rcx]\n",
							s3, s2, s0);
					}
					return(1);
				}

			}

//			s1=BS2J_RegNameAsNameD(s1);
			basm_print("mov rax, %s\n", s0);
			basm_print("movsxd rcx, %s\n", s1);

//			basm_print("shl rax, 16\n");
//			basm_print("sar rax, 16\n");

//			basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//			basm_print("and rax, rdx\n");
			
			if((op->opn2==BSVM2_OP_LDIXI) ||
				(op->opn2==BSVM2_OP_LDIXF))
			{
				s2=BS2J_RegNameAsNameD(s2);
				basm_print("mov edx, [rax+rcx*4]\n");
				basm_print("mov %s, edx\n", s2);
				return(1);
			}

			if((op->opn2==BSVM2_OP_LDIXL) ||
				(op->opn2==BSVM2_OP_LDIXD) ||
				(op->opn2==BSVM2_OP_LDIXA))
			{
//				s2=BS2J_RegNameAsNameD(s2);
				basm_print("mov rdx, [rax+rcx*8]\n");
				basm_print("mov %s, rdx\n", s2);
				return(1);
			}

			s3=NULL;
			if(op->opn2==BSVM2_OP_LDIXSS) { s3="movsxw"; j=2; }
			if(op->opn2==BSVM2_OP_LDIXUS) { s3="movzxw"; j=2; }
			if(op->opn2==BSVM2_OP_LDIXSB) { s3="movsxb"; j=1; }
			if(op->opn2==BSVM2_OP_LDIXUB) { s3="movzxb"; j=1; }
			if(s3)
			{
				s2=BS2J_RegNameAsNameD(s2);
				if(j>1)
					basm_print("%s edx, [rax+rcx*%d]\n", s3, j);
				else
					basm_print("%s edx, [rax+rcx]\n", s3);
				basm_print("mov %s, edx\n", s2);
				return(1);
			}
		}

		if(	(op->opn2==BSVM2_OP_STIXI) ||
			(op->opn2==BSVM2_OP_STIXL) ||
			(op->opn2==BSVM2_OP_STIXF) ||
			(op->opn2==BSVM2_OP_STIXD) ||
			(op->opn2==BSVM2_OP_STIXA) ||
			(op->opn2==BSVM2_OP_STIXS) ||
			(op->opn2==BSVM2_OP_STIXB))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s1=BS2J_GetMemRefStackRd(tr, op->t1);
			s2=BS2J_GetMemRefStackRdD(tr, op->t2);
			s0=BS2J_GetMemRefStackRd(tr, op->t0);

			if(BS2J_JitRefIsRegP(tr, s1) &&
				BS2J_JitRefIsRegP(tr, s2) &&
				BS2J_JitRefIsRegP(tr, s0))
			{
				if((op->opn2==BSVM2_OP_STIXI) ||
					(op->opn2==BSVM2_OP_STIXF))
				{
					s0=BS2J_RegNameAsNameD(s0);
					basm_print("movsxd rcx, %s\n", s2);
					basm_print("mov [%s+rcx*4], %s\n", s1, s0);
					return(1);
				}

				if((op->opn2==BSVM2_OP_STIXL) ||
					(op->opn2==BSVM2_OP_STIXD) ||
					(op->opn2==BSVM2_OP_STIXA))
				{
					basm_print("movsxd rcx, %s\n", s2);
					basm_print("mov [%s+rcx*8], %s\n", s1, s0);
					return(1);
				}

				if(op->opn2==BSVM2_OP_STIXS)
				{
					s0=BS2J_RegNameAsNameW(s0);
					basm_print("movsxd rcx, %s\n", s2);
					basm_print("mov [%s+rcx*2], %s\n", s1, s0);
					return(1);
				}

				if(op->opn2==BSVM2_OP_STIXB)
				{
					s0=BS2J_RegNameAsNameB(s0);
					basm_print("movsxd rcx, %s\n", s2);
					basm_print("mov [%s+rcx], %s\n", s1, s0);
					return(1);
				}
			}

//			s2=BS2J_RegNameAsNameD(s2);
			basm_print("mov rax, %s\n", s1);
			basm_print("movsxd rcx, %s\n", s2);
			
//			basm_print("shl rax, 16\n");
//			basm_print("sar rax, 16\n");

//			basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//			basm_print("and rax, rdx\n");
			
			if((op->opn2==BSVM2_OP_STIXI) ||
				(op->opn2==BSVM2_OP_STIXF))
			{
//				s0=BS2J_GetMemRefStackRdD(tr, op->t0);
				s0=BS2J_RegNameAsNameD(s0);
				basm_print("mov edx, %s\n", s0);
				basm_print("mov [rax+rcx*4], edx\n");
				return(1);
			}

			if((op->opn2==BSVM2_OP_STIXL) ||
				(op->opn2==BSVM2_OP_STIXD) ||
				(op->opn2==BSVM2_OP_STIXA))
			{
//				s0=BS2J_GetMemRefStackRd(tr, op->t0);
				basm_print("mov rdx, %s\n", s0);
				basm_print("mov [rax+rcx*8], rdx\n");
				return(1);
			}

			if(op->opn2==BSVM2_OP_STIXS)
			{
//				s0=BS2J_GetMemRefStackRdD(tr, op->t0);
				s0=BS2J_RegNameAsNameD(s0);
				basm_print("mov edx, %s\n", s0);
				basm_print("mov [rax+rcx*2], dx\n");
				return(1);
			}

			if(op->opn2==BSVM2_OP_STIXB)
			{
//				s0=BS2J_GetMemRefStackRdD(tr, op->t0);
				s0=BS2J_RegNameAsNameD(s0);
				basm_print("mov edx, %s\n", s0);
				basm_print("mov [rax+rcx], dl\n");
				return(1);
			}
		}


#if 1
		if(	(op->opn2==BSVM2_OP_LDIXIC) ||
			(op->opn2==BSVM2_OP_LDIXLC) ||
			(op->opn2==BSVM2_OP_LDIXFC) ||
			(op->opn2==BSVM2_OP_LDIXDC) ||
			(op->opn2==BSVM2_OP_LDIXAC) ||
			(op->opn2==BSVM2_OP_LDIXSSC) ||
			(op->opn2==BSVM2_OP_LDIXUSC) ||
			(op->opn2==BSVM2_OP_LDIXSBC) ||
			(op->opn2==BSVM2_OP_LDIXUBC))
		{
			BS2J_CheckCacheLocalsESI(tr);
//			s0=BS2J_GetMemRefStackRd(tr, op->t0);
//			s1=BS2J_GetMemRefStackWr(tr, op->t0);
			s0=BS2J_GetMemRefStackRdWr(tr, op->t0);
			basm_print("mov rax, %s\n", s0);
//			basm_print("movsxd rcx, %s\n", BS2J_GetMemRefStack(tr, op->t1));

//			basm_print("shl rax, 16\n");
//			basm_print("sar rax, 16\n");

//			basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//			basm_print("and rax, rdx\n");

			if(BS2J_JitRefIsRegP(tr, s0))
			{
				if((op->opn2==BSVM2_OP_LDIXIC) ||
					(op->opn2==BSVM2_OP_LDIXFC))
				{
					s1=BS2J_RegNameAsNameD(s0);
					basm_print("mov %s, [%s+%d]\n", s1, s0, op->v.i*4);
					return(1);
				}

				if((op->opn2==BSVM2_OP_LDIXLC) ||
					(op->opn2==BSVM2_OP_LDIXDC) ||
					(op->opn2==BSVM2_OP_LDIXAC))
				{
					basm_print("mov %s, [%s+%d]\n", s0, s0, op->v.i*8);
					return(1);
				}

				s2=NULL;
				if(op->opn2==BSVM2_OP_LDIXSSC)
					{ s2="movsxw"; j=op->v.i*2; }
				if(op->opn2==BSVM2_OP_LDIXUSC)
					{ s2="movzxw"; j=op->v.i*2; }
				if(op->opn2==BSVM2_OP_LDIXSBC)
					{ s2="movsxb"; j=op->v.i; }
				if(op->opn2==BSVM2_OP_LDIXUBC)
					{ s2="movzxb"; j=op->v.i; }
				if(s2)
				{
					s1=BS2J_RegNameAsNameD(s0);
					basm_print("%s %s, [%s+%d]\n", s2, s1, s0, j);
					return(1);
				}
			}

			if((op->opn2==BSVM2_OP_LDIXIC) ||
				(op->opn2==BSVM2_OP_LDIXFC))
			{
//				s1=BS2J_GetMemRefStackWrD(tr, op->t0);
				s1=BS2J_RegNameAsNameD(s0);
				basm_print("mov edx, [rax+%d]\n", op->v.i*4);
				basm_print("mov %s, edx\n", s1);
				return(1);
			}

			if((op->opn2==BSVM2_OP_LDIXLC) ||
				(op->opn2==BSVM2_OP_LDIXDC) ||
				(op->opn2==BSVM2_OP_LDIXAC))
			{
//				s1=BS2J_GetMemRefStackWr(tr, op->t0);
				s1=s0;
				basm_print("mov rdx, [rax+%d]\n", op->v.i*8);
				basm_print("mov %s, rdx\n", s1);
				return(1);
			}

			s2=NULL;
			if(op->opn2==BSVM2_OP_LDIXSSC) { s2="movsxw"; j=op->v.i*2; }
			if(op->opn2==BSVM2_OP_LDIXUSC) { s2="movzxw"; j=op->v.i*2; }
			if(op->opn2==BSVM2_OP_LDIXSBC) { s2="movsxb"; j=op->v.i; }
			if(op->opn2==BSVM2_OP_LDIXUBC) { s2="movzxb"; j=op->v.i; }
			if(s2)
			{
//				s1=BS2J_GetMemRefStackWrD(tr, op->t0);
				s1=BS2J_RegNameAsNameD(s0);
				basm_print("%s edx, [rax+%d]\n", s2, j);
				basm_print("mov %s, edx\n", s1);
				return(1);
			}
		}

		if(	(op->opn2==BSVM2_OP_STIXIC) ||
			(op->opn2==BSVM2_OP_STIXLC) ||
			(op->opn2==BSVM2_OP_STIXFC) ||
			(op->opn2==BSVM2_OP_STIXDC) ||
			(op->opn2==BSVM2_OP_STIXAC) ||
			(op->opn2==BSVM2_OP_STIXSC) ||
			(op->opn2==BSVM2_OP_STIXBC))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackRd(tr, op->t1);
			s1=BS2J_GetMemRefStackRd(tr, op->t0);

			if(BS2J_JitRefIsRegP(tr, s0) &&
				BS2J_JitRefIsRegP(tr, s1))
			{
				if((op->opn2==BSVM2_OP_STIXIC) ||
					(op->opn2==BSVM2_OP_STIXFC))
				{
					s1=BS2J_RegNameAsNameD(s1);
					basm_print("mov [%s+%d], %s\n", s0, op->v.i*4, s1);
					return(1);
				}

				if((op->opn2==BSVM2_OP_STIXLC) ||
					(op->opn2==BSVM2_OP_STIXDC) ||
					(op->opn2==BSVM2_OP_STIXAC))
				{
					basm_print("mov [%s+%d], %s\n", s0, op->v.i*8, s1);
					return(1);
				}

				if(op->opn2==BSVM2_OP_STIXSC)
				{
					s1=BS2J_RegNameAsNameW(s1);
					basm_print("mov [%s+%d], %s\n", s0, op->v.i*2, s1);
					return(1);
				}

				if(op->opn2==BSVM2_OP_STIXBC)
				{
					s1=BS2J_RegNameAsNameB(s1);
					basm_print("mov [%s+%d], %s\n", s0, op->v.i, s1);
					return(1);
				}
			}

			basm_print("mov rax, %s\n", s0);
//			basm_print("movsxd rcx, %s\n", BS2J_GetMemRefStack(tr, op->t2));
			
//			basm_print("shl rax, 16\n");
//			basm_print("sar rax, 16\n");

//			basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//			basm_print("and rax, rdx\n");
			
			if((op->opn2==BSVM2_OP_STIXIC) ||
				(op->opn2==BSVM2_OP_STIXFC))
			{
//				s1=BS2J_GetMemRefStackRdD(tr, op->t0);
				s1=BS2J_RegNameAsNameD(s1);
				basm_print("mov edx, %s\n", s1);
				basm_print("mov [rax+%d], edx\n", op->v.i*4);
				return(1);
			}

			if((op->opn2==BSVM2_OP_STIXLC) ||
				(op->opn2==BSVM2_OP_STIXDC) ||
				(op->opn2==BSVM2_OP_STIXAC))
			{
//				s1=BS2J_GetMemRefStackRd(tr, op->t0);
				basm_print("mov rdx, %s\n", s1);
				basm_print("mov [rax+%d], rdx\n", op->v.i*8);
				return(1);
			}

			if(op->opn2==BSVM2_OP_STIXSC)
			{
//				s1=BS2J_GetMemRefStackRdD(tr, op->t0);
				s1=BS2J_RegNameAsNameD(s1);
				basm_print("mov edx, %s\n", s1);
				basm_print("mov [rax+%d], dx\n", op->v.i*2);
				return(1);
			}

			if(op->opn2==BSVM2_OP_STIXBC)
			{
//				s1=BS2J_GetMemRefStackRdD(tr, op->t0);
				s1=BS2J_RegNameAsNameD(s1);
				basm_print("mov edx, %s\n", s1);
				basm_print("mov [rax+%d], dl\n", op->v.i);
				return(1);
			}
		}

		if(op->opn2==BSVM2_OP_CMPA)
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackRd(tr, op->t0);
			s1=BS2J_GetMemRefStackRd(tr, op->t1);
			s2=BS2J_GetMemRefStackWrD(tr, op->t0);
			basm_print("mov rax, %s\n", s0);
			basm_print("mov rcx, %s\n", s1);
			basm_print("sub rax, rcx\n");
			basm_print("sub rcx, %s\n", s0);
			basm_print("sar rax, 63\n");
			basm_print("shr rcx, 63\n");
			basm_print("or eax, ecx\n");
			basm_print("mov %s, eax\n", s2);
			return(1);
		}
#endif

#if 1
		if(	(op->opn2==BSVM2_OP_LEARI) ||
			(op->opn2==BSVM2_OP_LEARL) ||
			(op->opn2==BSVM2_OP_LEARF) ||
			(op->opn2==BSVM2_OP_LEARD) ||
			(op->opn2==BSVM2_OP_LEARA) ||
			(op->opn2==BSVM2_OP_LEARS) ||
			(op->opn2==BSVM2_OP_LEARB))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackRdWr(tr, op->t0);
			s1=BS2J_GetMemRefStackRdD(tr, op->t1);

			j=0;
			if((op->opn2==BSVM2_OP_LEARI) ||
				(op->opn2==BSVM2_OP_LEARF))
					j=4;
			if((op->opn2==BSVM2_OP_LEARL) ||
				(op->opn2==BSVM2_OP_LEARD) ||
				(op->opn2==BSVM2_OP_LEARA))
					j=8;
			if(op->opn2==BSVM2_OP_LEARS)
				j=2;
			if(op->opn2==BSVM2_OP_LEARB)
				j=1;

#if 1
			if(BS2J_JitRefIsRegP(tr, s0) &&
				BS2J_JitRefIsRegP(tr, s1))
			{
				if(j>1)
				{
//					s2=BS2J_RegNameAsNameD(s1);
					basm_print("movsxd rcx, %s\n", s1);
					basm_print("lea %s, [%s+rcx*%d]\n", s0, s0, j);
					return(1);
				}
				if(j)
				{
//					s2=BS2J_RegNameAsNameD(s1);
					basm_print("movsxd rcx, %s\n", s1);
					basm_print("lea %s, [%s+rcx]\n", s0, s0);
					return(1);
				}
			}
#endif

//			s1=BS2J_RegNameAsNameD(s1);

			basm_print("mov rax, %s\n", s0);
			basm_print("movsxd rcx, %s\n", s1);
			
//			basm_print("shl rax, 16\n");
//			basm_print("sar rax, 16\n");

//			basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//			basm_print("and rax, rdx\n");

			if(j>1)
				basm_print("lea rdx, [rax+rcx*%d]\n", j);
			else
				basm_print("lea rdx, [rax+rcx]\n");
			basm_print("mov %s, rdx\n", s0);
			return(1);
		}
#endif

#if 1
		if(	(op->opn2==BSVM2_OP_LEARIC) ||
			(op->opn2==BSVM2_OP_LEARLC) ||
			(op->opn2==BSVM2_OP_LEARFC) ||
			(op->opn2==BSVM2_OP_LEARDC) ||
			(op->opn2==BSVM2_OP_LEARAC) ||
			(op->opn2==BSVM2_OP_LEARSC) ||
			(op->opn2==BSVM2_OP_LEARBC))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackRdWr(tr, op->t0);

			j=0;
			if((op->opn2==BSVM2_OP_LEARIC) ||
				(op->opn2==BSVM2_OP_LEARFC))
					j=op->v.i*4;
			if((op->opn2==BSVM2_OP_LEARLC) ||
				(op->opn2==BSVM2_OP_LEARDC) ||
				(op->opn2==BSVM2_OP_LEARAC))
					j=op->v.i*8;
			if(op->opn2==BSVM2_OP_LEARSC)
				j=op->v.i*2;
			if(op->opn2==BSVM2_OP_LEARBC)
				j=op->v.i*1;

#if 1
			if(BS2J_JitRefIsRegP(tr, s0))
			{
				basm_print("lea %s, [%s+%d]\n", s0, s0, j);
				return(1);
			}
#endif

			basm_print("mov rax, %s\n", s0);
//			basm_print("movsxd rcx, %s\n", BS2J_GetMemRefStack(tr, op->t1));
			
//			basm_print("shl rax, 16\n");
//			basm_print("sar rax, 16\n");

//			basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//			basm_print("and rax, rdx\n");
			
			basm_print("lea rdx, [rax+%d]\n", j);
			basm_print("mov %s, rdx\n", s0);
			return(1);
		}
#endif

#if 1
		if(	(op->opn2==BSVM2_OP_LDDRI) ||
			(op->opn2==BSVM2_OP_LDDRL) ||
			(op->opn2==BSVM2_OP_LDDRF) ||
			(op->opn2==BSVM2_OP_LDDRD) ||
			(op->opn2==BSVM2_OP_LDDRA) ||
			(op->opn2==BSVM2_OP_LDDRSS) ||
			(op->opn2==BSVM2_OP_LDDRUS) ||
			(op->opn2==BSVM2_OP_LDDRSB) ||
			(op->opn2==BSVM2_OP_LDDRUB))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackRdWr(tr, op->t0);

			if(BS2J_JitRefIsRegP(tr, s0))
			{
				if((op->opn2==BSVM2_OP_LDDRI) ||
					(op->opn2==BSVM2_OP_LDDRF))
				{
					s1=BS2J_RegNameAsNameD(s0);
					basm_print("mov %s, [%s]\n", s1, s0);
					return(1);
				}

				if((op->opn2==BSVM2_OP_LDDRL) ||
					(op->opn2==BSVM2_OP_LDDRD) ||
					(op->opn2==BSVM2_OP_LDDRA))
				{
					s1=s0;
					basm_print("mov %s, [%s]\n", s1, s0);
					return(1);
				}

				s2=NULL;
				if(op->opn2==BSVM2_OP_LDDRSS) { s2="movsxw"; }
				if(op->opn2==BSVM2_OP_LDDRUS) { s2="movzxw"; }
				if(op->opn2==BSVM2_OP_LDDRSB) { s2="movsxb"; }
				if(op->opn2==BSVM2_OP_LDDRUB) { s2="movzxb"; }
				if(s2)
				{
					s1=BS2J_RegNameAsNameD(s0);
					basm_print("%s %s, [%s]\n", s2, s1, s0);
					return(1);
				}
			}

			basm_print("mov rax, %s\n", s0);

//			basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//			basm_print("and rax, rdx\n");
			
			if((op->opn2==BSVM2_OP_LDDRI) ||
				(op->opn2==BSVM2_OP_LDDRF))
			{
				s1=BS2J_GetMemRefStackWrD(tr, op->t0);
				basm_print("mov edx, [rax]\n");
				basm_print("mov %s, edx\n", s1);
				return(1);
			}

			if((op->opn2==BSVM2_OP_LDDRL) ||
				(op->opn2==BSVM2_OP_LDDRD) ||
				(op->opn2==BSVM2_OP_LDDRA))
			{
				s1=BS2J_GetMemRefStackWr(tr, op->t0);
				basm_print("mov rdx, [rax]\n");
				basm_print("mov %s, rdx\n", s1);
				return(1);
			}

			s2=NULL;
			if(op->opn2==BSVM2_OP_LDDRSS) { s2="movsxw"; }
			if(op->opn2==BSVM2_OP_LDDRUS) { s2="movzxw"; }
			if(op->opn2==BSVM2_OP_LDDRSB) { s2="movsxb"; }
			if(op->opn2==BSVM2_OP_LDDRUB) { s2="movzxb"; }
			if(s2)
			{
				s1=BS2J_GetMemRefStackWrD(tr, op->t0);
				basm_print("%s edx, [rax]\n", s2);
				basm_print("mov %s, edx\n", s1);
				return(1);
			}
		}

		if(	(op->opn2==BSVM2_OP_STDRI) ||
			(op->opn2==BSVM2_OP_STDRL) ||
			(op->opn2==BSVM2_OP_STDRF) ||
			(op->opn2==BSVM2_OP_STDRD) ||
			(op->opn2==BSVM2_OP_STDRA) ||
			(op->opn2==BSVM2_OP_STDRS) ||
			(op->opn2==BSVM2_OP_STDRB))
		{
			BS2J_CheckCacheLocalsESI(tr);
			s0=BS2J_GetMemRefStackRd(tr, op->t1);
			s1=BS2J_GetMemRefStackRd(tr, op->t0);

			if(BS2J_JitRefIsRegP(tr, s0) &&
				BS2J_JitRefIsRegP(tr, s1))
			{
				if((op->opn2==BSVM2_OP_STDRI) ||
					(op->opn2==BSVM2_OP_STDRF))
				{
					s1=BS2J_RegNameAsNameD(s1);
					basm_print("mov [%s], %s\n", s0, s1);
					return(1);
				}

				if((op->opn2==BSVM2_OP_STDRL) ||
					(op->opn2==BSVM2_OP_STDRD) ||
					(op->opn2==BSVM2_OP_STDRA))
				{
					basm_print("mov [%s], %s\n", s0, s1);
					return(1);
				}

				if(op->opn2==BSVM2_OP_STDRS)
				{
					s1=BS2J_RegNameAsNameW(s1);
					basm_print("mov [%s], %s\n", s0, s1);
					return(1);
				}

				if(op->opn2==BSVM2_OP_STDRB)
				{
					s1=BS2J_RegNameAsNameB(s1);
					basm_print("mov [%s], %s\n", s0, s1);
					return(1);
				}
			}

			basm_print("mov rax, %s\n", s0);
			
//			basm_print("shl rax, 16\n");
//			basm_print("sar rax, 16\n");

//			basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//			basm_print("and rax, rdx\n");
			
			if((op->opn2==BSVM2_OP_STDRI) ||
				(op->opn2==BSVM2_OP_STDRF))
			{
				s1=BS2J_GetMemRefStackRdD(tr, op->t0);
				basm_print("mov edx, %s\n", s1);
				basm_print("mov [rax], edx\n");
				return(1);
			}

			if((op->opn2==BSVM2_OP_STDRL) ||
				(op->opn2==BSVM2_OP_STDRD) ||
				(op->opn2==BSVM2_OP_STDRA))
			{
				s1=BS2J_GetMemRefStackRd(tr, op->t0);
				basm_print("mov rdx, %s\n", s1);
				basm_print("mov [rax], rdx\n");
				return(1);
			}

			if(op->opn2==BSVM2_OP_STDRS)
			{
				s1=BS2J_GetMemRefStackRdD(tr, op->t0);
				basm_print("mov edx, %s\n", s1);
				basm_print("mov [rax], dx\n");
				return(1);
			}

			if(op->opn2==BSVM2_OP_STDRB)
			{
				s1=BS2J_GetMemRefStackRdD(tr, op->t0);
				basm_print("mov edx, %s\n", s1);
				basm_print("mov [rax], dl\n");
				return(1);
			}
		}
#endif
		k=-1;
	}
#endif

#if 1
	if(	(op->opn==BSVM2_OP_LDDRPL) ||
		(op->opn==BSVM2_OP_LDDRVL))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRd(tr, op->i0);
		s1=BS2J_GetMemRefStackWr(tr, op->t0);

		if(BS2J_JitRefIsRegP(tr, s0) &&
			BS2J_JitRefIsRegP(tr, s1))
		{
			if(	(op->z==BSVM2_OPZ_INT) ||
				(op->z==BSVM2_OPZ_UINT) ||
				(op->z==BSVM2_OPZ_FLOAT))
			{
				s1=BS2J_RegNameAsNameD(s1);
				basm_print("mov %s, [%s]\n", s1, s0);
				return(1);
			}

			if(	(op->z==BSVM2_OPZ_LONG) ||
				(op->z==BSVM2_OPZ_ULONG) ||
				(op->z==BSVM2_OPZ_DOUBLE) ||
				(op->z==BSVM2_OPZ_ADDRESS))
			{
				basm_print("mov %s, [%s]\n", s1, s0);
				return(1);
			}

			s2=NULL;
			if(op->z==BSVM2_OPZ_SHORT) { s2="movsxw"; }
			if(op->z==BSVM2_OPZ_USHORT) { s2="movzxw"; }
			if(op->z==BSVM2_OPZ_SBYTE) { s2="movsxb"; }
			if(op->z==BSVM2_OPZ_UBYTE) { s2="movzxb"; }
			if(s2)
			{
				s1=BS2J_RegNameAsNameD(s1);
				basm_print("%s %s, [%s]\n", s2, s1, s0);
				return(1);
			}
		}

		basm_print("mov rax, %s\n", s0);

//		basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//		basm_print("and rax, rdx\n");
		
		if(	(op->z==BSVM2_OPZ_INT) ||
			(op->z==BSVM2_OPZ_UINT) ||
			(op->z==BSVM2_OPZ_FLOAT))
		{
//			s1=BS2J_GetMemRefStackWrD(tr, op->t0);
			s1=BS2J_RegNameAsNameD(s1);
			basm_print("mov edx, [rax]\n");
			basm_print("mov %s, edx\n", s1);
			return(1);
		}

		if(	(op->z==BSVM2_OPZ_LONG) ||
			(op->z==BSVM2_OPZ_ULONG) ||
			(op->z==BSVM2_OPZ_DOUBLE) ||
			(op->z==BSVM2_OPZ_ADDRESS))
		{
//			s1=BS2J_GetMemRefStackWr(tr, op->t0);
			s1=BS2J_RegNameAsNameD(s1);
			basm_print("mov rdx, [rax]\n");
			basm_print("mov %s, rdx\n", s1);
			return(1);
		}

		s2=NULL;
		if(op->z==BSVM2_OPZ_SHORT) { s2="movsxw"; }
		if(op->z==BSVM2_OPZ_USHORT) { s2="movzxw"; }
		if(op->z==BSVM2_OPZ_SBYTE) { s2="movsxb"; }
		if(op->z==BSVM2_OPZ_UBYTE) { s2="movzxb"; }
		if(s2)
		{
//			s1=BS2J_GetMemRefStackWrD(tr, op->t0);
			s1=BS2J_RegNameAsNameD(s1);
			basm_print("%s edx, [rax]\n", s2);
			basm_print("mov %s, edx\n", s1);
			return(1);
		}
	}

	if(	(op->opn==BSVM2_OP_STDRPL) ||
		(op->opn==BSVM2_OP_STDRVL))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRd(tr, op->i0);
		s1=BS2J_GetMemRefStackRd(tr, op->t0);

		if(BS2J_JitRefIsRegP(tr, s0) &&
			BS2J_JitRefIsRegP(tr, s1))
		{
			if(	(op->z==BSVM2_OPZ_INT) ||
				(op->z==BSVM2_OPZ_UINT) ||
				(op->z==BSVM2_OPZ_FLOAT))
			{
				s1=BS2J_RegNameAsNameD(s1);
				basm_print("mov [%s], %s\n", s0, s1);
				return(1);
			}

			if(	(op->z==BSVM2_OPZ_LONG) ||
				(op->z==BSVM2_OPZ_ULONG) ||
				(op->z==BSVM2_OPZ_DOUBLE) ||
				(op->z==BSVM2_OPZ_ADDRESS))
			{
				basm_print("mov [%s], %s\n", s0, s1);
				return(1);
			}

			if(	(op->z==BSVM2_OPZ_SHORT) ||
				(op->z==BSVM2_OPZ_USHORT))
			{
				s1=BS2J_RegNameAsNameW(s1);
				basm_print("mov [%s], %s\n", s0, s1);
				return(1);
			}

			if(	(op->z==BSVM2_OPZ_SBYTE) ||
				(op->z==BSVM2_OPZ_UBYTE))
			{
				s1=BS2J_RegNameAsNameB(s1);
				basm_print("mov [%s], %s\n", s0, s1);
				return(1);
			}
		}

		basm_print("mov rax, %s\n", s0);

//		basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//		basm_print("and rax, rdx\n");
		
		if(	(op->z==BSVM2_OPZ_INT) ||
			(op->z==BSVM2_OPZ_UINT) ||
			(op->z==BSVM2_OPZ_FLOAT))
		{
//			s1=BS2J_GetMemRefStackRdD(tr, op->t0);
			s1=BS2J_RegNameAsNameD(s1);
			basm_print("mov edx, %s\n", s1);
			basm_print("mov [rax], edx\n");
			return(1);
		}

		if(	(op->z==BSVM2_OPZ_LONG) ||
			(op->z==BSVM2_OPZ_ULONG) ||
			(op->z==BSVM2_OPZ_DOUBLE) ||
			(op->z==BSVM2_OPZ_ADDRESS))
		{
//			s1=BS2J_GetMemRefStackRd(tr, op->t0);
//			s1=BS2J_RegNameAsNameD(s1);
			basm_print("mov rdx, %s\n", s1);
			basm_print("mov [rax], rdx\n");
			return(1);
		}

		if(	(op->z==BSVM2_OPZ_SHORT) ||
			(op->z==BSVM2_OPZ_USHORT))
		{
//			s1=BS2J_GetMemRefStackRdD(tr, op->t0);
			s1=BS2J_RegNameAsNameD(s1);
			basm_print("mov edx, %s\n", s1);
			basm_print("mov [rax], dx\n");
			return(1);
		}

		if(	(op->z==BSVM2_OPZ_SBYTE) ||
			(op->z==BSVM2_OPZ_UBYTE))
		{
//			s1=BS2J_GetMemRefStackRdD(tr, op->t0);
			s1=BS2J_RegNameAsNameD(s1);
			basm_print("mov edx, %s\n", s1);
			basm_print("mov [rax], dl\n");
			return(1);
		}
	}

	if((op->opn==BSVM2_OP_DIFFPTR) && ((op->i0&0xFFFB)==0))
	{
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefStackRd(tr, op->t0);
		s1=BS2J_GetMemRefStackRd(tr, op->t1);
		basm_print("mov rax, %s\n", s0);
		basm_print("mov rcx, %s\n", s1);

//		basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
//		basm_print("and rax, rdx\n");
//		basm_print("and rcx, rdx\n");

		basm_print("sub rax, rcx\n");
		
		switch(op->z)
		{
		case BSVM2_OPZ_X128:
			basm_print("sar rax, 4\n");
			break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
		case BSVM2_OPZ_DOUBLE:
		case BSVM2_OPZ_ADDRESS:
#ifndef WIN64
		case BSVM2_OPZ_NLONG:
		case BSVM2_OPZ_UNLONG:
#endif
			basm_print("sar rax, 3\n");
			break;
		case BSVM2_OPZ_INT:
		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_FLOAT:
#ifdef WIN64
		case BSVM2_OPZ_NLONG:
		case BSVM2_OPZ_UNLONG:
#endif
			basm_print("sar rax, 2\n");
			break;
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_USHORT:
			basm_print("sar rax, 1\n");
			break;
		case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_UBYTE:
			break;
		default:
			basm_print("sar rax, 3\n");
			break;
		}
		
		s2=BS2J_GetMemRefStackWr(tr, op->t0);
		basm_print("mov %s, rax\n", s2);
		return(1);
	}
#endif

#if 1
	if(op->opn==BSVM2_OP_LDOSL)
	{
		vi=op->v.p;
		j=dtcFieldOffsetOf(vi->objinf);
		if(j<0)
			return(0);
		
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocalRd(tr, op->i0);
		basm_print("mov rax, %s\n", s0);

		basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
		basm_print("and rax, rdx\n");
		
		if((op->z==BSVM2_OPZ_INT) ||
			(op->z==BSVM2_OPZ_UINT) ||
			(op->z==BSVM2_OPZ_FLOAT))
		{
			s1=BS2J_GetMemRefStackWrD(tr, op->t0);
			basm_print("mov edx, [rax+%d]\n", j);
			basm_print("mov %s, edx\n", s1);
			return(1);
		}

		if((op->z==BSVM2_OPZ_LONG) ||
			(op->z==BSVM2_OPZ_DOUBLE) ||
			(op->z==BSVM2_OPZ_ADDRESS))
		{
			s1=BS2J_GetMemRefStackWr(tr, op->t0);
			basm_print("mov rdx, [rax+%d]\n", j);
			basm_print("mov %s, rdx\n", s1);
			return(1);
		}

		s2=NULL;
		if(op->z==BSVM2_OPZ_SHORT) { s2="movsxw"; }
		if(op->z==BSVM2_OPZ_USHORT) { s2="movzxw"; }
		if(op->z==BSVM2_OPZ_SBYTE) { s2="movsxb"; }
		if(op->z==BSVM2_OPZ_UBYTE) { s2="movzxb"; }
		if(s2)
		{
			s1=BS2J_GetMemRefStackWr(tr, op->t0);
			basm_print("%s edx, [rax+%d]\n", s2, j);
			basm_print("mov %s, edx\n", s1);
			return(1);
		}
	}

	if(op->opn==BSVM2_OP_STOSL)
	{
		vi=op->v.p;
		j=dtcFieldOffsetOf(vi->objinf);
		if(j<0)
			return(0);
		
		BS2J_CheckCacheLocalsESI(tr);
		s0=BS2J_GetMemRefLocal(tr, op->i0);
		basm_print("mov rax, %s\n", s0);

		basm_print("mov rdx, 0x0000FFFFFFFFFFFF\n");
		basm_print("and rax, rdx\n");
		
		if((op->z==BSVM2_OPZ_INT) ||
			(op->z==BSVM2_OPZ_UINT) ||
			(op->z==BSVM2_OPZ_FLOAT))
		{
			s1=BS2J_GetMemRefStackRdD(tr, op->t0);
			basm_print("mov edx, %s\n", s1);
			basm_print("mov [rax+%d], edx\n", j);
			return(1);
		}

		if((op->z==BSVM2_OPZ_LONG) ||
			(op->z==BSVM2_OPZ_DOUBLE) ||
			(op->z==BSVM2_OPZ_ADDRESS))
		{
			s1=BS2J_GetMemRefStackRd(tr, op->t0);
			basm_print("mov rdx, %s\n", s1);
			basm_print("mov [rax+%d], rdx\n", j);
			return(1);
		}

		if((op->z==BSVM2_OPZ_SHORT) ||
			(op->z==BSVM2_OPZ_USHORT))
		{
			s1=BS2J_GetMemRefStackRdD(tr, op->t0);
			basm_print("mov edx, %s\n", s1);
			basm_print("mov [rax+%d], dx\n", j);
			return(1);
		}

		if((op->z==BSVM2_OPZ_SBYTE) ||
			(op->z==BSVM2_OPZ_UBYTE))
		{
			s1=BS2J_GetMemRefStackRdD(tr, op->t0);
			basm_print("mov edx, %s\n", s1);
			basm_print("mov [rax+%d], dl\n", j);
			return(1);
		}
	}
#endif

#if 0
	if(op->opn==BSVM2_OP_LDIXZLL)
	{
		if(((op->i2)==BSVM2_OPZ_INT) ||
			((op->i2)==BSVM2_OPZ_UINT) ||
			((op->i2)==BSVM2_OPZ_FLOAT))
		{
			BS2J_CheckCacheLocalsESI(tr);
			basm_print("mov eax, %s\n",
				BS2J_GetMemRefLocal(tr, op->i0));
			basm_print("mov edx, %s\n",
				BS2J_GetMemRefLocalP(tr, op->i0, 4));
			basm_print("mov ecx, %s\n",
				BS2J_GetMemRefLocal(tr, op->i1));

			basm_print("and edx, 0x0FFFFFFF\n");
			basm_print("lea ecx, [edx+ecx*4]\n");
			basm_print("mov eax, [eax+%d]\n",
				offsetof(BGBDT_TagArrHead, data));

			basm_print("mov eax, [eax+ecx]\n");
			basm_print("mov %s, eax\n", BS2J_GetMemRefStack(tr, op->t0));
			return(1);
		}
	}

	if(op->opn==BSVM2_OP_STIXZLL)
	{
		if(((op->i2)==BSVM2_OPZ_INT) ||
			((op->i2)==BSVM2_OPZ_UINT) ||
			((op->i2)==BSVM2_OPZ_FLOAT))
		{
			BS2J_CheckCacheLocalsESI(tr);
			basm_print("mov eax, %s\n",
				BS2J_GetMemRefLocal(tr, op->i0));
			basm_print("mov edx, %s\n",
				BS2J_GetMemRefLocalP(tr, op->i0, 4));
			basm_print("mov ecx, %s\n",
				BS2J_GetMemRefLocal(tr, op->i1));

			basm_print("and edx, 0x0FFFFFFF\n");
			basm_print("lea ecx, [edx+ecx*4]\n");
			basm_print("mov edx, [eax+%d]\n",
				offsetof(BGBDT_TagArrHead, data));

			basm_print("mov eax, %s\n", BS2J_GetMemRefStack(tr, op->t0));
			basm_print("mov [edx+ecx], eax\n");
			return(1);
		}
	}


	if(	(op->opn==BSVM2_OP_LDIXI) ||
		(op->opn==BSVM2_OP_LDIXL) ||
		(op->opn==BSVM2_OP_LDIXF) ||
		(op->opn==BSVM2_OP_LDIXD) ||
		(op->opn==BSVM2_OP_LDIXA))
	{
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov eax, %s\n", BS2J_GetMemRefStack(tr, op->t0));
		basm_print("mov edx, %s\n", BS2J_GetMemRefStackP(tr, op->t0, 4));
		basm_print("mov ecx, %s\n", BS2J_GetMemRefStack(tr, op->t1));

		basm_print("and edx, 0x0FFFFFFF\n");
		basm_print("mov eax, [eax+%d]\n",
			offsetof(BGBDT_TagArrHead, data));
		
		if((op->opn==BSVM2_OP_LDIXI) ||
			(op->opn==BSVM2_OP_LDIXF))
		{
			basm_print("lea ecx, [edx+ecx*4]\n");
			basm_print("mov eax, [eax+ecx]\n");
			basm_print("mov %s, eax\n",
				BS2J_GetMemRefStack(tr, op->t0));
			return(1);
		}

		if((op->opn==BSVM2_OP_LDIXL) ||
			(op->opn==BSVM2_OP_LDIXD) ||
			(op->opn==BSVM2_OP_LDIXA))
		{
			basm_print("lea ecx, [edx+ecx*8]\n");
			basm_print("movq xmm0, [eax+ecx]\n");
			basm_print("movq %s, xmm0\n",
				BS2J_GetMemRefStack(tr, op->t0));
			return(1);
		}
	}

	if(	(op->opn==BSVM2_OP_LDIXIC) ||
		(op->opn==BSVM2_OP_LDIXLC) ||
		(op->opn==BSVM2_OP_LDIXFC) ||
		(op->opn==BSVM2_OP_LDIXDC) ||
		(op->opn==BSVM2_OP_LDIXAC))
	{
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov eax, %s\n", BS2J_GetMemRefStack(tr, op->t0));
		basm_print("mov edx, %s\n", BS2J_GetMemRefStackP(tr, op->t0, 4));

		basm_print("and edx, 0x0FFFFFFF\n");
		basm_print("mov eax, [eax+%d]\n",
			offsetof(BGBDT_TagArrHead, data));
		
		if((op->opn==BSVM2_OP_LDIXIC) ||
			(op->opn==BSVM2_OP_LDIXFC))
		{
			basm_print("lea ecx, [edx+%d]\n", op->v.i*4);
			basm_print("mov eax, [eax+ecx]\n");
			basm_print("mov %s, eax\n",
				BS2J_GetMemRefStack(tr, op->t0));
			return(1);
		}

		if((op->opn==BSVM2_OP_LDIXLC) ||
			(op->opn==BSVM2_OP_LDIXDC) ||
			(op->opn==BSVM2_OP_LDIXAC))
		{
			basm_print("lea ecx, [edx+%d]\n", op->v.i*4);
			basm_print("movq xmm0, [eax+ecx]\n");
			basm_print("movq %s, xmm0\n",
				BS2J_GetMemRefStack(tr, op->t0));
			return(1);
		}
	}
#endif

	return(0);
}

int BS2J_CheckAllocTemp(BSVM2_Trace *tr)
{
	if(tr->jitmp)
		return(0);
		
	tr->jitmp=bgbdt_mm_malloc(sizeof(BSVM2_JitTraceTemp));
	return(1);
}

int BS2J_CheckFreeTemp(BSVM2_Trace *tr)
{
	if(!tr->jitmp)
		return(0);
		
	bgbdt_mm_free(tr->jitmp);
	return(1);
}

int BS2J_StatAllocRegs_AddLcl(BSVM2_Trace *tr, int pos, int reg)
{
	if((reg<0) || (reg>=256))
	{
		BSVM2_DBGTRAP
	}

	if(!tr->jitmp->lclstat_cnt[reg])
	{
		tr->jitmp->lclstat_beg[reg]=pos;
		tr->jitmp->lclstat_idx[reg]=reg;
	}
	tr->jitmp->lclstat_cnt[reg]++;
	tr->jitmp->lclstat_end[reg]=pos;
	return(0);
}

int BS2J_StatAllocRegs_AddStk(BSVM2_Trace *tr, int pos, int reg)
{
	BS2J_StatAllocRegs_AddLcl(tr, pos, reg+tr->cblk->largs);
	return(0);
}

int BS2J_StatAllocRegs_MoveLcl(BSVM2_Trace *tr, int dst, int src)
{
	if((dst<0) || (dst>=256))
		*(int *)-1=-1;
	if((src<0) || (src>=256))
		*(int *)-1=-1;

	tr->jitmp->lclstat_cnt[dst]=tr->jitmp->lclstat_cnt[src];
	tr->jitmp->lclstat_idx[dst]=tr->jitmp->lclstat_idx[src];
	tr->jitmp->lclstat_beg[dst]=tr->jitmp->lclstat_beg[src];
	tr->jitmp->lclstat_end[dst]=tr->jitmp->lclstat_end[src];
	return(0);
}

int BS2J_StatAllocRegs_CheckCollide(BSVM2_Trace *tr, int dst, int src)
{
	int b0, b1, b2, b3;

	b0=tr->jitmp->lclstat_beg[dst];
	b1=tr->jitmp->lclstat_end[dst];
	b2=tr->jitmp->lclstat_beg[src];
	b3=tr->jitmp->lclstat_end[src];

	if(b0>b3)return(0);
	if(b2>b1)return(0);
	return(1);
}

int BS2J_StatAllocRegs(BSVM2_Trace *tr)
{
	short uregs[]={12, 13, 14, 15, 3, 8, 9, 10, 11, -1};
//	short uregs[]={12, 13, 14, 15, 3, -1};
//	short uregs[]={12, 13, 14, 15, -1};
	BSVM2_Opcode *op;
	int n, ri;
	int i, j, k;
	
//	return(-1);
	
	BS2J_CheckAllocTemp(tr);
	
	/* Get counts and spans */
	for(i=0; i<tr->n_ops; i++)
	{
		op=tr->ops[i];

		if(op->opfl&BSVM2_OPFL_USED_T0)
			BS2J_StatAllocRegs_AddStk(tr, i, op->t0);
		if(op->opfl&BSVM2_OPFL_USED_T1)
			BS2J_StatAllocRegs_AddStk(tr, i, op->t1);
		if(op->opfl&BSVM2_OPFL_USED_T2)
			BS2J_StatAllocRegs_AddStk(tr, i, op->t2);
		if(op->opfl&BSVM2_OPFL_USED_T3)
			BS2J_StatAllocRegs_AddStk(tr, i, op->t2+1);

		if(op->opfl&BSVM2_OPFL_USED_I0)
			BS2J_StatAllocRegs_AddLcl(tr, i, op->i0);
		if(op->opfl&BSVM2_OPFL_USED_I1)
			BS2J_StatAllocRegs_AddLcl(tr, i, op->i1);
		if(op->opfl&BSVM2_OPFL_USED_I2)
			BS2J_StatAllocRegs_AddLcl(tr, i, op->i2);
	}
	
	/* Collapse down to only used vars */
	n=0;
	for(i=0; i<256; i++)
	{
		if(tr->jitmp->lclstat_cnt[i])
			{ BS2J_StatAllocRegs_MoveLcl(tr, n++, i); }
	}
	tr->jitmp->n_lclstat=n;

	/* Sort ranges by use-count */
	for(i=0; i<n; i++)
		for(j=i+1; j<n; j++)
	{
		if(tr->jitmp->lclstat_cnt[j]>tr->jitmp->lclstat_cnt[i])
		{
			BS2J_StatAllocRegs_MoveLcl(tr, n, i);
			BS2J_StatAllocRegs_MoveLcl(tr, i, j);
			BS2J_StatAllocRegs_MoveLcl(tr, j, n);
		}
	}
	
	for(i=0; i<n; i++)
	{
		tr->jitmp->lclstat_chn[i]=-99;
		tr->jitmp->lclstat_reg[i]=-99;
	}

	for(i=0; i<16; i++)
	{
		tr->jitmp->reg_lclstat[i]=-1;
		tr->jitmp->reg_lclmap[i]=-1;
	}
	tr->jitmp->reg_dirty=0;
	
	for(i=0; uregs[i]>=0; i++)
	{
		ri=uregs[i];
		for(j=0; j<n; j++)
			if(tr->jitmp->lclstat_reg[j]==(-99))
		{
			k=tr->jitmp->reg_lclstat[ri];
			while(k>=0)
			{
				if(BS2J_StatAllocRegs_CheckCollide(tr, j, k))
					break;
				k=tr->jitmp->lclstat_chn[k];
			}
			if(k<0)
			{
				tr->jitmp->lclstat_reg[j]=ri;
				k=tr->jitmp->reg_lclstat[ri];
				tr->jitmp->lclstat_chn[j]=k;
				tr->jitmp->reg_lclstat[ri]=j;
			}
		}
	}
	
	return(0);
}

int BS2J_CheckSyncRegs(BSVM2_Trace *tr, BSVM2_Opcode *op)
{
	static byte flush[16]={
		1,1,1,0, 0,0,0,0, 1,1,1,1, 0,0,0,0};
	char *s0;
	int i, j, k;

	if(!tr->jitmp)
		return(0);

	if(op)
	{
		for(i=0; i<16; i++)
		{
			k=tr->jitmp->reg_lclmap[i];
			if(!flush[i])
				continue;
			if((k>=0) && (tr->jitmp->reg_dirty&(1<<i)))
			{
				s0=BS2J_RegIdxToName(i);
				basm_print("mov [rsi+%d], %s\n", k*8, s0);
				tr->jitmp->reg_dirty&=~(1<<i);
			}
			tr->jitmp->reg_lclmap[i]=-1;
		}

		if(op->opfl&BSVM2_OPFL_USED_T0)
			BS2J_FlushStackRegID(tr, op->t0);
		if(op->opfl&BSVM2_OPFL_USED_T1)
			BS2J_FlushStackRegID(tr, op->t1);
		if(op->opfl&BSVM2_OPFL_USED_T2)
			BS2J_FlushStackRegID(tr, op->t2);
		if(op->opfl&BSVM2_OPFL_USED_T3)
			BS2J_FlushStackRegID(tr, op->t2+1);

		if(op->opfl&BSVM2_OPFL_USED_I0)
			BS2J_FlushLocalRegID(tr, op->i0);
		if(op->opfl&BSVM2_OPFL_USED_I1)
			BS2J_FlushLocalRegID(tr, op->i1);
		if(op->opfl&BSVM2_OPFL_USED_I2)
			BS2J_FlushLocalRegID(tr, op->i2);
	}else
	{
		for(i=0; i<16; i++)
		{
			k=tr->jitmp->reg_lclmap[i];
			if((k>=0) && (tr->jitmp->reg_dirty&(1<<i)))
			{
				s0=BS2J_RegIdxToName(i);
				basm_print("mov [rsi+%d], %s\n", k*8, s0);
			}
			tr->jitmp->reg_lclmap[i]=-1;
		}

		tr->jitmp->reg_dirty=0;
	}
	return(0);
}

int BS2J_CheckRestoreRegs(BSVM2_Trace *tr)
{
	if(tr->trfl&BSVM2_TRFL_SAVEDEBX)
	{
		basm_print("mov rbx, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_RBX));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDEDI)
	{
		basm_print("mov rdi, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_RDI));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDESI)
	{
		basm_print("mov rsi, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_RSI));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDEBP)
	{
		basm_print("mov rbp, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_RBP));
	}

	if(tr->trfl&BSVM2_TRFL_SAVEDR12)
	{
		basm_print("mov r12, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_R12));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDR13)
	{
		basm_print("mov r13, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_R13));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDR14)
	{
		basm_print("mov r14, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_R14));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDR15)
	{
		basm_print("mov r15, [rsp+%d]\n",
			BS2J_VirtualEBP(tr, BS2J_X64_OFS_R15));
	}

	return(0);
}

int BS2J_EmitNormalReturn(BSVM2_Trace *tr)
{
	BS2J_CheckRestoreRegs(tr);
	if(!(tr->trfl&BSVM2_TRFL_NOFRAME))
		basm_print("add rsp, %d\n", BS2J_X64_FRAMESZ);
	basm_print("ret\n");
	return(1);
}

int BS2J_EmitReturnJumpNext(BSVM2_Trace *tr)
{
	BS2J_CheckFrameInRCX(tr);

	BS2J_CheckRestoreRegs(tr);
	if(!(tr->trfl&BSVM2_TRFL_NOFRAME))
		basm_print("add rsp, %d\n", BS2J_X64_FRAMESZ);
//	basm_print("ret\n");

	basm_print("mov rdx, rax\n");
	basm_print("mov rax, [rdx+%d]\n", offsetof(BSVM2_Trace, Run));
	basm_print("jmp rax\n");

	return(1);
}

char *BS2J_JitAddrToStr(void *addr)
{
	char tb[64];
	void *p;
	char *s;

#if 1
//	s=BASM_GetPtrName(addr);
	s=BSVM2_Interp_ReverseLookupPublicOp(addr);
	if(s)
	{
		p=BASM_GetPtr(s);
		if(p)
//		if(p && (p==addr))
			return(s);
			
		if(p)
		{
			__debugbreak();
		}
	}
#endif

	sprintf(tb, "0x%016llX", (u64)addr);
	return(bgbdt_mm_strdup(tb));
}

int BS2J_JitTraceHandleTail(BSVM2_Trace *tr)
{
	char *s0, *s1;

#if 1
	if(tr->trfl&BSVM2_TRFL_TJNEXT)
	{
		if(!(tr->trfl&BSVM2_TRFL_CANTHROW))
		{
			BS2J_CheckFrameInRCX(tr);

			BS2J_CheckRestoreRegs(tr);

//			basm_print("mov rcx, [rsp+%d]\n",
//				BS2J_VirtualEBP(tr, BS2J_X64_OFS_FRAME));
//			basm_print("mov rdx, 0x%016llX\n", (u64)(tr->jnext));

			basm_print("mov rdx, %s\n", tr->jnext->gensym);

			if(!(tr->trfl&BSVM2_TRFL_NOFRAME))
				basm_print("add rsp, 504\n");
			basm_print("mov rax, [rdx+%d]\n", offsetof(BSVM2_Trace, Run));
			basm_print("jmp rax\n");
		}else
		{
//			basm_print("mov rax, 0x%016llX\n", (u64)(tr->jnext));
			basm_print("mov rax, %s\n", tr->jnext->gensym);
			BS2J_EmitNormalReturn(tr);
		}
		
		return(1);
	}
#endif

#if 1
	if(	!(tr->trfl&BSVM2_TRFL_NOFRAME) &&
		(tr->top->opn==BSVM2_OP_CMPI) &&
		((tr->top->opn2==BSVM2_OP_JEQ) ||
		 (tr->top->opn2==BSVM2_OP_JNE) ||
		 (tr->top->opn2==BSVM2_OP_JLT) ||
		 (tr->top->opn2==BSVM2_OP_JGT) ||
		 (tr->top->opn2==BSVM2_OP_JLE) ||
		 (tr->top->opn2==BSVM2_OP_JGE)))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheLocalsESI(tr);

		basm_print("mov ecx, %s\n", BS2J_GetMemRefStack(tr, tr->top->t0));
//		basm_print("mov rax, 0x%016llX\n", (u64)(tr->top->nexttrace));
		basm_print("mov rax, %s\n", tr->top->nexttrace->gensym);
		basm_print("cmp ecx, %s\n", BS2J_GetMemRefStack(tr, tr->top->t1));

		switch(tr->top->opn2)
		{
			case BSVM2_OP_JEQ: s0="cmove"; break;
			case BSVM2_OP_JNE: s0="cmovne"; break;
			case BSVM2_OP_JLT: s0="cmovl"; break;
			case BSVM2_OP_JGT: s0="cmovg"; break;
			case BSVM2_OP_JLE: s0="cmovle"; break;
			case BSVM2_OP_JGE: s0="cmovge"; break;
			default: s0="cmove"; break;
		}
//		basm_print("mov r10, 0x%016llX\n",
//			(u64)(tr->top->jmptrace));
		basm_print("mov r10, %s\n", tr->top->jmptrace->gensym);
		basm_print("%s rax, r10\n", s0);

		BS2J_EmitNormalReturn(tr);
//		BS2J_EmitReturnJumpNext(tr);

		return(1);
	}
#endif

#if 1
	if(	!(tr->trfl&BSVM2_TRFL_NOFRAME) &&
		(tr->top->opn==BSVM2_OP_CMPILL) &&
		((tr->top->opn2==BSVM2_OP_JEQ) ||
		 (tr->top->opn2==BSVM2_OP_JNE) ||
		 (tr->top->opn2==BSVM2_OP_JLT) ||
		 (tr->top->opn2==BSVM2_OP_JGT) ||
		 (tr->top->opn2==BSVM2_OP_JLE) ||
		 (tr->top->opn2==BSVM2_OP_JGE)))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheLocalsESI(tr);

		basm_print("mov ecx, %s\n", BS2J_GetMemRefLocal(tr, tr->top->i0));
//		basm_print("mov rax, 0x%016llX\n", (u64)(tr->top->nexttrace));
		basm_print("mov rax, %s\n", tr->top->nexttrace->gensym);
		basm_print("cmp ecx, %s\n", BS2J_GetMemRefLocal(tr, tr->top->i1));

		switch(tr->top->opn2)
		{
			case BSVM2_OP_JEQ: s0="cmove"; break;
			case BSVM2_OP_JNE: s0="cmovne"; break;
			case BSVM2_OP_JLT: s0="cmovl"; break;
			case BSVM2_OP_JGT: s0="cmovg"; break;
			case BSVM2_OP_JLE: s0="cmovle"; break;
			case BSVM2_OP_JGE: s0="cmovge"; break;
			default: s0="cmove"; break;
		}
//		basm_print("mov r10, 0x%016llX\n",
//			(u64)(tr->top->jmptrace));
		basm_print("mov r10, %s\n", tr->top->jmptrace->gensym);
		basm_print("%s rax, r10\n", s0);

		BS2J_EmitNormalReturn(tr);
//		BS2J_EmitReturnJumpNext(tr);

		return(1);
	}
#endif

#if 1
	if(	!(tr->trfl&BSVM2_TRFL_NOFRAME) &&
		 ((tr->top->opn==BSVM2_OP_JEQ) ||
		  (tr->top->opn==BSVM2_OP_JNE) ||
		  (tr->top->opn==BSVM2_OP_JLT) ||
		  (tr->top->opn==BSVM2_OP_JGT) ||
		  (tr->top->opn==BSVM2_OP_JLE) ||
		  (tr->top->opn==BSVM2_OP_JGE)))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheLocalsESI(tr);

		basm_print("mov ecx, %s\n", BS2J_GetMemRefStack(tr, tr->top->t0));
		basm_print("mov rax, %s\n", tr->top->nexttrace->gensym);
		basm_print("cmp ecx, 0\n");

		switch(tr->top->opn)
		{
			case BSVM2_OP_JEQ: s0="cmove"; break;
			case BSVM2_OP_JNE: s0="cmovne"; break;
			case BSVM2_OP_JLT: s0="cmovl"; break;
			case BSVM2_OP_JGT: s0="cmovg"; break;
			case BSVM2_OP_JLE: s0="cmovle"; break;
			case BSVM2_OP_JGE: s0="cmovge"; break;
			default: s0="cmove"; break;
		}
		basm_print("mov r10, %s\n", tr->top->jmptrace->gensym);
		basm_print("%s rax, r10\n", s0);

		BS2J_EmitNormalReturn(tr);
//		BS2J_EmitReturnJumpNext(tr);

		return(1);
	}
#endif

	return(0);
}

int BS2J_CheckSetupJitTrace(BSVM2_Trace *tr)
{
	BSVM2_Opcode *escop_op[512];
	char *escop_gs[512];
	int n_escop;
	
	void *p;
	char *s0, *s1;
	char *fn, *fnth, *topgs;
	int i, j, k;
	
	fn=BASM_GenSym();
	if(tr->trfl&BSVM2_TRFL_CANTHROW)
		fnth=BASM_GenSym();

	tr->gensym_fn=bgbdt_mm_strdup(fn);

	BS2J_StatAllocRegs(tr);

//	BASM_BeginAssembly(NULL);

	basm_print("[section .text]\n");

	basm_print("/* %s %06X */\n",
		tr->cblk->func->name,
		tr->cs-tr->cblk->code);

	basm_print("%s:\n", fn);

//	basm_print("sub rsp, %d\n", BS2J_X64_FRAMESZ);
	
	n_escop=0;

	if(tr->n_ops>0)
//	if(1)
	{
		basm_print("sub rsp, %d\n", BS2J_X64_FRAMESZ);

		BS2J_CheckSavedESI(tr);
		BS2J_CheckSavedEDI(tr);

		if(tr->jitmp)
		{
			if(tr->jitmp->reg_lclstat[3]>=0)
				BS2J_CheckSavedEBX(tr);
			if(tr->jitmp->reg_lclstat[12]>=0)
				BS2J_CheckSavedR12(tr);
			if(tr->jitmp->reg_lclstat[13]>=0)
				BS2J_CheckSavedR13(tr);
			if(tr->jitmp->reg_lclstat[14]>=0)
				BS2J_CheckSavedR14(tr);
			if(tr->jitmp->reg_lclstat[15]>=0)
				BS2J_CheckSavedR15(tr);
		}

		basm_print("mov rdi, rcx\n");
		basm_print("mov rsi, [rdi+%d]\n", offsetof(BSVM2_Frame, local));
		tr->trfl|=BSVM2_TRFL_LCLINESI|BSVM2_TRFL_FRMINEDI;		
	}else
	{
		tr->trfl|=BSVM2_TRFL_NOFRAME;
	}
	
	if(tr->n_ops>0)
//	if(1)
	{
		n_escop=0;
		for(i=0; i<tr->n_ops; i++)
		{
			bs2j_op_usecnt[tr->ops[i]->opn]++;
			if(tr->ops[i]->opn2)
				bs2j_op_usecnt2[tr->ops[i]->opn2]++;

			if(tr->jitmp)
				tr->jitmp->opix=i;

			if(BS2J_TryJitOpcode(tr, tr->ops[i])>0)
				continue;

			bs2j_op_miscnt[tr->ops[i]->opn]++;
			if(tr->ops[i]->opn2)
				bs2j_op_miscnt2[tr->ops[i]->opn2]++;

			BS2J_CheckSyncRegs(tr, tr->ops[i]);

			j=n_escop++;
			escop_op[j]=tr->ops[i];
			escop_gs[j]=BASM_GenSym();

			BS2J_CheckFrameInRCX(tr);
			basm_print("mov rdx, %s\n", escop_gs[j]);

			s0=BS2J_JitAddrToStr(tr->ops[i]->Run);
			if((*s0=='@') || ((*s0>='0') && (*s0<='9')))
			{
				basm_print("mov rax, %s\n", s0);
				basm_print("call rax /* OP:%04X-%04X */\n",
					tr->ops[i]->opn, tr->ops[i]->opn2);
			}else
			{
				basm_print("call %s /* OP:%04X-%04X */\n",
					s0, tr->ops[i]->opn, tr->ops[i]->opn2);
			}

			if(tr->ops[i]->opfl&BSVM2_TRFL_CANTHROW)
			{
				BS2J_CheckCacheFrameEDI(tr);
				basm_print("mov rcx, [rdi+%d]\n",
					offsetof(BSVM2_Frame, ctx));
				basm_print("mov eax, [rcx+%d]\n",
					offsetof(BSVM2_Context, status));
				basm_print("and eax, eax\n");
				basm_print("jnz %s\n", fnth);
			}
		}

	//	BS2J_JitTraceHandleTail(tr);
	}

	if(tr->jitmp)
		tr->jitmp->opix=tr->n_ops;
	BS2J_CheckSyncRegs(tr, NULL);

	topgs=NULL;
	if(!BS2J_JitTraceHandleTail(tr))
	{
		bs2j_op_miscnt[tr->top->opn]++;
		if(tr->top->opn2)
			bs2j_op_miscnt2[tr->top->opn2]++;

		topgs=BASM_GenSym();

		BS2J_CheckFrameInRCX(tr);
		BS2J_CheckRestoreRegs(tr);
//		basm_print("mov rdx, 0x%016llX\n", (u64)(tr->top));
		basm_print("mov rdx, %s\n", topgs);
//		basm_print("mov rax, 0x%016llX\n", (u64)(tr->top->Run));
//		basm_print("mov rax, %s\n",
//			BS2J_JitAddrToStr(tr->top->Run));
//		basm_print("add rsp, %d\n", BS2J_X64_FRAMESZ);
//		basm_print("jmp rax\n");

		s0=BS2J_JitAddrToStr(tr->top->Run);
		if((*s0=='@') || ((*s0>='0') && (*s0<='9')))
		{
			basm_print("mov rax, %s\n",
				BS2J_JitAddrToStr(tr->top->Run));
			if(!(tr->trfl&BSVM2_TRFL_NOFRAME))
				basm_print("add rsp, %d\n", BS2J_X64_FRAMESZ);
			basm_print("jmp rax\n");
		}else
		{
			if(!(tr->trfl&BSVM2_TRFL_NOFRAME))
				basm_print("add rsp, %d\n", BS2J_X64_FRAMESZ);
			basm_print("jmp %s\n", s0);
		}
	}

	if(tr->trfl&BSVM2_TRFL_CANTHROW)
	{
		basm_print("%s:\n", fnth);

//		basm_print("mov r10, 0x%016llX\n", (u64)(tr));
		basm_print("mov r10, %s\n", tr->gensym);
		basm_print("mov [rcx+%d], r10\n",
			offsetof(BSVM2_Context, trace));

		basm_print("xor eax, eax\n");
		BS2J_EmitNormalReturn(tr);
	}

//	if(n_escop || topgs)
//	basm_print("[section .data]\n");

	basm_print("align 8\n");

	for(i=0; i<n_escop; i++)
	{
		basm_print("%s:\n", escop_gs[i]);
		k=offsetof(BSVM2_Opcode, Run);
		k=(k+3)/4;
		for(j=0; j<k; j++)
		{
			basm_print("\tdd 0x%08X\n",
				((u32 *)(escop_op[i]))[j]);
		}
		basm_print("\tdq 0\n");
	}
	
	if(topgs)
	{
		basm_print("%s:\n", topgs);
//		k=offsetof(BSVM2_TailOpcode, Run);
		k=offsetof(BSVM2_TailOpcode, nexttrace);
		k=(k+3)/4;
		for(j=0; j<k; j++)
		{
			basm_print("\tdd 0x%08X\n",
				((u32 *)(tr->top))[j]);
		}

#if 1
		basm_print("\tdq %s\n",
			tr->top->nexttrace?
				tr->top->nexttrace->gensym:"0");
		basm_print("\tdq %s\n",
			tr->top->jmptrace?
				tr->top->jmptrace->gensym:"0");
#endif
		basm_print("\tdq 0\n");
	}

	basm_print("%s:\n", tr->gensym);
	basm_print("\tdq %s\n", fn);
	basm_print("\tdq %s\n", topgs?topgs:"0");
	basm_print("\tdq %s\n", tr->jnext->gensym);
	basm_print("\tdq 0x%016llX\n", (u64)(tr->cblk));
	basm_print("\tdq 0\n");
	basm_print("\tdq 0\n");
	basm_print("\tdq 0\n");
	basm_print("\tdq 0\n");
	
//	basm_print("[section .text]\n");

//	BASM_EndAssembly();
//	BASM_EndAssemblyQuiet();

#if 0
	p=BASM_GetPtr(fn);
//	if(p) ((void (*)())p)();

	if(p)
	{
		tr->Run=p;
	}
#endif

	return(0);
}

int BS2J_BeginJitTraces(BSVM2_CodeBlock *cblk)
{
	BASM_BeginAssembly(NULL);
	return(0);
}

int BS2J_EndJitTraces(BSVM2_CodeBlock *cblk)
{
	BASM_EndAssembly();
//	BASM_EndAssemblyQuiet();
	return(0);
}

int BS2J_CheckPreSetupJitTrace(BSVM2_Trace *tr)
{
	if(!tr->gensym)
		tr->gensym=bgbdt_mm_strdup(BASM_GenSym());
	return(0);
}

int BS2J_CheckPostSetupJitTrace(BSVM2_Trace *tr)
{
	void *p;

	BS2J_CheckFreeTemp(tr);

	if(!tr->gensym_fn)
		return(-1);

	p=BASM_GetPtr(tr->gensym_fn);
	if(p)
		{ tr->Run=p; }
	return(0);
}


BS2VM_API int BS2J_DumpMissCounts()
{
	int tabv[4096];
	int tabc[4096];
	int i, j, k, n, t;
	
	n=0; t=0;
	for(i=0; i<4096; i++)
	{
		if(!bs2j_op_miscnt[i])
			continue;

		t+=bs2j_op_miscnt[i];
		j=n++;
		tabv[j]=i;
		tabc[j]=bs2j_op_miscnt[i];
	}
	
	for(i=0; i<n; i++)
		for(j=i+1; j<n; j++)
	{
		if(tabc[j]>tabc[i])
		{
			k=tabv[i]; tabv[i]=tabv[j]; tabv[j]=k;
			k=tabc[i]; tabc[i]=tabc[j]; tabc[j]=k;
		}
	}

	if(n>10)n=10;

	printf("Top %d most missed:\n", n);

#if 1
	for(i=0; i<n; i++)
	{
		printf("%04X %d/%d\n", tabv[i], tabc[i], t);
	}
#endif

#if 0
	for(i=0; i<4096; i++)
	{
		if(!bs2j_op_miscnt[i])
			continue;
		printf("%04X %d/%d\n", i, bs2j_op_miscnt[i], t);
	}
#endif
	return(0);
}

BS2VM_API int BS2J_DumpUseCounts_MUMB()
{
	int tabv[4096];
	int tabc[4096];
	int i, j, k, n, t;
	
	n=0; t=0;
//	for(i=0; i<4096; i++)
	for(i=0xC0; i<4096; i++)
	{
		if(!bs2j_op_usecnt[i])
			continue;

		t+=bs2j_op_usecnt[i];
		j=n++;
		tabv[j]=i;
		tabc[j]=bs2j_op_usecnt[i];
	}
	
	for(i=0; i<n; i++)
		for(j=i+1; j<n; j++)
	{
		if(tabc[j]>tabc[i])
		{
			k=tabv[i]; tabv[i]=tabv[j]; tabv[j]=k;
			k=tabc[i]; tabc[i]=tabc[j]; tabc[j]=k;
		}
	}

	if(n>10)n=10;

	printf("Top %d most used multi-byte:\n", n);

#if 1
	for(i=0; i<n; i++)
	{
		printf("%04X %d/%d\n", tabv[i], tabc[i], t);
	}
#endif
	return(0);
}

BS2VM_API int BS2J_DumpUseCounts_LUSB()
{
	int tabv[4096];
	int tabc[4096];
	int i, j, k, n, t;
	
	n=0; t=0;
	for(i=0; i<0xC0; i++)
	{
//		if(!bs2j_op_usecnt[i])
//			continue;

		t+=bs2j_op_usecnt[i];
		j=n++;
		tabv[j]=i;
		tabc[j]=bs2j_op_usecnt[i];
	}
	
	for(i=0; i<n; i++)
		for(j=i+1; j<n; j++)
	{
//		if(tabc[j]>tabc[i])
		if(tabc[j]<tabc[i])
		{
			k=tabv[i]; tabv[i]=tabv[j]; tabv[j]=k;
			k=tabc[i]; tabc[i]=tabc[j]; tabc[j]=k;
		}
	}

	if(n>10)n=10;

	printf("Top %d least used single-byte:\n", n);

#if 1
	for(i=0; i<n; i++)
	{
		printf("%04X %d/%d\n", tabv[i], tabc[i], t);
	}
#endif
	return(0);
}

BS2VM_API int BS2J_DumpUseCounts()
{
	BS2J_DumpUseCounts_MUMB();
	BS2J_DumpUseCounts_LUSB();
	return(0);
}

#endif
