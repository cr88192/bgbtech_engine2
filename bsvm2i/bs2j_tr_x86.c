/*
EAX(0): Scratch
ECX(1): Scratch
EDX(2): Scratch
EBX(3): VM Stack
ESP(4): OS Stack
EBP(5): Base Pointer
ESI(6): / Opcode Array
EDI(7): VM Frame

[EBP+ 4]/[ESP+60]: Return EIP
[EBP+ 0]/[ESP+56]: Saved EBP
[EBP- 4]/[ESP+52]: Saved ESI
[EBP- 8]/[ESP+48]: Saved EDI
[EBP-12]/[ESP+44]: Saved EBX

[ESP+4]: Opcode
[ESP+0]: Frame

 */

#include <bteifgl.h>

#if defined(X86) && defined(BS2I_USE_BASM)

#include <bgbasm.h>

int BS2J_VirtualEBP(BSVM2_Trace *tr, int ofs)
{
	return(ofs+56);
}

int BS2J_CheckSavedESI(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDESI))
	{
//		basm_print("mov [ebp-4], esi\n");
		basm_print("mov [esp+%d], esi\n", BS2J_VirtualEBP(tr, -4));
		tr->trfl|=BSVM2_TRFL_SAVEDESI;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedEDI(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDEDI))
	{
//		basm_print("mov [ebp-8], edi\n");
		basm_print("mov [esp+%d], edi\n", BS2J_VirtualEBP(tr, -8));
		tr->trfl|=BSVM2_TRFL_SAVEDEDI;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedEBX(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDEBX))
	{
//		basm_print("mov [ebp-12], ebx\n");
		basm_print("mov [esp+%d], ebx\n", BS2J_VirtualEBP(tr, -12));
		tr->trfl|=BSVM2_TRFL_SAVEDEBX;
		return(1);
	}
	return(0);
}

int BS2J_CheckSavedEBP(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_SAVEDEBP))
	{
		basm_print("mov [esp+%d], ebp\n", BS2J_VirtualEBP(tr, 0));
		tr->trfl|=BSVM2_TRFL_SAVEDEBP;
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
		basm_print("mov [esp], edi\n");
		tr->trfl|=BSVM2_TRFL_FRMONSTK;
	}else
	{
//		basm_print("mov eax, [ebp+8]; mov [esp], eax\n");
		basm_print("mov eax, [esp+%d]\n", BS2J_VirtualEBP(tr, 8));
		basm_print("mov [esp], eax\n");
		tr->trfl|=BSVM2_TRFL_FRMONSTK;
	}
	return(1);
}

int BS2J_CheckCacheFrameEDI(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_FRMINEDI))
	{
		BS2J_CheckSavedEDI(tr);
//		basm_print("mov edi, [ebp+8]\n");
		basm_print("mov edi, [esp+%d]\n", BS2J_VirtualEBP(tr, 8));
		tr->trfl|=BSVM2_TRFL_FRMINEDI;
		return(1);
	}
	return(0);
}

int BS2J_CheckCacheOpsESI(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_OPSINESI))
	{
		BS2J_CheckSavedESI(tr);
//		basm_print("mov edi, [ebp+8]\n");
//		basm_print("mov eax, [ebp+12]; mov [ebp-4], esi\n");

		basm_print("mov eax, [esp+%d]\n", BS2J_VirtualEBP(tr, 12));
//		basm_print("mov [esp+%d], esi\n", BS2J_VirtualEBP(tr, -4));

		basm_print("mov esi, [eax+%d]\n", offsetof(BSVM2_Trace, ops));
		tr->trfl&=~BSVM2_TRFL_LCLINESI;
		tr->trfl|=BSVM2_TRFL_OPSINESI;
		return(1);
	}
	return(0);
}

int BS2J_CheckCacheStackEBX(BSVM2_Trace *tr)
{
	if(!(tr->trfl&BSVM2_TRFL_STKINEBX))
	{
		BS2J_CheckSavedEBX(tr);
		BS2J_CheckCacheFrameEDI(tr);
		basm_print("mov ebx, [edi+%d]\n", offsetof(BSVM2_Frame, stack));
		tr->trfl|=BSVM2_TRFL_STKINEBX;
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

//		basm_print("mov ebx, [edi+%d]\n", offsetof(BSVM2_Frame, stack));
		basm_print("mov esi, [edi+%d]\n", offsetof(BSVM2_Frame, local));
		tr->trfl&=~BSVM2_TRFL_OPSINESI;
		tr->trfl|=BSVM2_TRFL_LCLINESI;
		return(1);
	}
	return(0);
}

int BS2J_TryJitOpcode(BSVM2_Trace *tr, BSVM2_Opcode *op)
{
	if(op->opn==BSVM2_OP_LABEL)
	{
		return(1);
	}

	if(	(op->opn==BSVM2_OP_LDI) ||
		(op->opn==BSVM2_OP_LDF))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov eax, [esi+%d]\n", op->i0*8);
		basm_print("mov [ebx+%d], eax\n", op->t0*8);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_LDL) ||
		(op->opn==BSVM2_OP_LDD) ||
		(op->opn==BSVM2_OP_LDA))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov eax, [esi+%d]\n", op->i0*8);
		basm_print("mov edx, [esi+%d]\n", op->i0*8+4);
		basm_print("mov [ebx+%d], eax\n", op->t0*8);
		basm_print("mov [ebx+%d], edx\n", op->t0*8+4);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_STI) ||
		(op->opn==BSVM2_OP_STF))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov eax, [ebx+%d]\n", op->t0*8);
		basm_print("mov [esi+%d], eax\n", op->i0*8);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_STL) ||
		(op->opn==BSVM2_OP_STD) ||
		(op->opn==BSVM2_OP_STA))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov eax, [ebx+%d]\n", op->t0*8+0);
		basm_print("mov edx, [ebx+%d]\n", op->t0*8+4);
		basm_print("mov [esi+%d], eax\n", op->i0*8+0);
		basm_print("mov [esi+%d], edx\n", op->i0*8+4);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_ADDI)||
		(op->opn==BSVM2_OP_SUBI)||
		(op->opn==BSVM2_OP_MULI)||
		(op->opn==BSVM2_OP_ANDI)||
		(op->opn==BSVM2_OP_ORI)||
		(op->opn==BSVM2_OP_XORI))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		basm_print("mov eax, [ebx+%d]\n", op->t0*8);
		basm_print("mov ecx, [ebx+%d]\n", op->t1*8);
		if(op->opn==BSVM2_OP_ADDI)
			{ basm_print("add eax, ecx\n"); }
		else if(op->opn==BSVM2_OP_SUBI)
			{ basm_print("sub eax, ecx\n"); }
		else if(op->opn==BSVM2_OP_MULI)
			{ basm_print("imul eax, ecx\n"); }
		else if(op->opn==BSVM2_OP_ANDI)
			{ basm_print("and eax, ecx\n"); }
		else if(op->opn==BSVM2_OP_ORI)
			{ basm_print("or eax, ecx\n"); }
		else if(op->opn==BSVM2_OP_XORI)
			{ basm_print("xor eax, ecx\n"); }
		basm_print("mov [ebx+%d], eax\n", op->t0*8);
		return(1);
	}

	if(	(op->opn==BSVM2_OP_ADDIC)||
		(op->opn==BSVM2_OP_SUBIC)||
		(op->opn==BSVM2_OP_MULIC)||
		(op->opn==BSVM2_OP_ANDIC)||
		(op->opn==BSVM2_OP_ORIC)||
		(op->opn==BSVM2_OP_XORIC))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		basm_print("mov eax, [ebx+%d]\n", op->t0*8);
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
		basm_print("mov [ebx+%d], eax\n", op->t0*8);
		return(1);
	}
	
	if((op->opn==BSVM2_OP_SHLIC) ||
		(op->opn==BSVM2_OP_SARIC))
	{
		if((op->v.i>(-32)) && (op->v.i<32))
		{
			BS2J_CheckCacheFrameEDI(tr);
			BS2J_CheckCacheStackEBX(tr);
			basm_print("mov eax, [ebx+%d]\n", op->t0*8);
			if((op->opn==BSVM2_OP_SHLIC) && (op->v.i>=0))
				{ basm_print("shl eax, %d\n", op->v.i); }
			else if((op->opn==BSVM2_OP_SARIC) && (op->v.i>=0))
				{ basm_print("sar eax, %d\n", op->v.i); }
			else if((op->opn==BSVM2_OP_SHLIC) && (op->v.i<0))
				{ basm_print("shr eax, %d\n", -op->v.i); }
			else if((op->opn==BSVM2_OP_SARIC) && (op->v.i<0))
				{ basm_print("shl eax, %d\n", -op->v.i); }
			basm_print("mov [ebx+%d], eax\n", op->t0*8);
			return(1);
		}
	}

	if(op->opn==BSVM2_OP_INCI)
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		basm_print("inc dword [ebx+%d]\n", op->t0*8);
		return(1);
	}

	if(op->opn==BSVM2_OP_INCIL)
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("inc dword [esi+%d]\n", op->i0*8);
		return(1);
	}

	if(op->opn==BSVM2_OP_DECI)
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		basm_print("dec dword [ebx+%d]\n", op->t0*8);
		return(1);
	}

	if(op->opn==BSVM2_OP_DECIL)
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("dec dword [esi+%d]\n", op->i0*8);
		return(1);
	}

	if(op->opn==BSVM2_OP_CMPI)
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		basm_print("mov eax, [ebx+%d]\n", op->t0*8);
		basm_print("mov ecx, [ebx+%d]\n", op->t1*8);
		basm_print("sub eax, ecx\n");
		basm_print("sub ecx, [ebx+%d]\n", op->t0*8);
		basm_print("sar eax, 31\n");
		basm_print("shr ecx, 31\n");
		basm_print("or eax, ecx\n");
		basm_print("mov [ebx+%d], eax\n", op->t0*8);
		return(1);
	}

	if(op->opn==BSVM2_OP_CMPILL)
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov eax, [esi+%d]\n", op->i0*8);
		basm_print("mov ecx, [esi+%d]\n", op->i1*8);
		basm_print("sub eax, ecx\n");
		basm_print("sub ecx, [esi+%d]\n", op->i0*8);
		basm_print("sar eax, 31\n");
		basm_print("shr ecx, 31\n");
		basm_print("or eax, ecx\n");
		basm_print("mov [ebx+%d], eax\n", op->t0*8);
		return(1);
	}

	if(op->opn==BSVM2_OP_CMPILC)
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov eax, [esi+%d]\n", op->i0*8);
		basm_print("mov ecx, %d\n", op->v.i);
		basm_print("sub eax, ecx\n");
		basm_print("sub ecx, [esi+%d]\n", op->i0*8);
		basm_print("sar eax, 31\n");
		basm_print("shr ecx, 31\n");
		basm_print("or eax, ecx\n");
		basm_print("mov [ebx+%d], eax\n", op->t0*8);
		return(1);
	}

	if(op->opn==BSVM2_OP_LDIXZLL)
	{
		if(((op->i2)==BSVM2_OPZ_INT) ||
			((op->i2)==BSVM2_OPZ_UINT) ||
			((op->i2)==BSVM2_OPZ_FLOAT))
		{
			BS2J_CheckCacheFrameEDI(tr);
			BS2J_CheckCacheStackEBX(tr);
			BS2J_CheckCacheLocalsESI(tr);
			basm_print("mov eax, [esi+%d]\n", op->i0*8);
			basm_print("mov edx, [esi+%d]\n", op->i0*8+4);
			basm_print("mov ecx, [esi+%d]\n", op->i1*8);

			basm_print("and edx, 0x0FFFFFFF\n");
			basm_print("lea ecx, [edx+ecx*4]\n");
			basm_print("mov eax, [eax+%d]\n",
				offsetof(BGBDT_TagArrHead, data));

			basm_print("mov eax, [eax+ecx]\n");
			basm_print("mov [ebx+%d], eax\n", op->t0*8);
			return(1);
		}
	}

	if(op->opn==BSVM2_OP_STIXZLL)
	{
		if(((op->i2)==BSVM2_OPZ_INT) ||
			((op->i2)==BSVM2_OPZ_UINT) ||
			((op->i2)==BSVM2_OPZ_FLOAT))
		{
			BS2J_CheckCacheFrameEDI(tr);
			BS2J_CheckCacheStackEBX(tr);
			BS2J_CheckCacheLocalsESI(tr);
			basm_print("mov eax, [esi+%d]\n", op->i0*8);
			basm_print("mov edx, [esi+%d]\n", op->i0*8+4);
			basm_print("mov ecx, [esi+%d]\n", op->i1*8);

			basm_print("and edx, 0x0FFFFFFF\n");
			basm_print("lea ecx, [edx+ecx*4]\n");
			basm_print("mov edx, [eax+%d]\n",
				offsetof(BGBDT_TagArrHead, data));

			basm_print("mov eax, [ebx+%d]\n", op->t0*8);
			basm_print("mov [edx+ecx], eax\n");
			return(1);
		}
	}

	return(0);
}

int BS2J_CheckRestoreRegs(BSVM2_Trace *tr)
{
	if(tr->trfl&BSVM2_TRFL_SAVEDEBX)
	{
//		basm_print("mov ebx, [ebp-12]\n");
		basm_print("mov ebx, [esp+%d]\n", BS2J_VirtualEBP(tr, -12));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDEDI)
	{
//		basm_print("mov edi, [ebp-8]\n");
		basm_print("mov edi, [esp+%d]\n", BS2J_VirtualEBP(tr, -8));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDESI)
	{
//		basm_print("mov esi, [ebp-4]\n");
		basm_print("mov esi, [esp+%d]\n", BS2J_VirtualEBP(tr, -4));
	}
	if(tr->trfl&BSVM2_TRFL_SAVEDEBP)
	{
		basm_print("mov ebp, [esp+%d]\n", BS2J_VirtualEBP(tr, 0));
	}
	return(0);
}

int BS2J_EmitNormalReturn(BSVM2_Trace *tr)
{
	BS2J_CheckRestoreRegs(tr);
//	basm_print("mov esp, ebp; pop ebp; ret\n");
	basm_print("add esp, 60\n");
	basm_print("ret\n");
	return(1);
}

int BS2J_JitTraceHandleTail(BSVM2_Trace *tr)
{
	char *s0, *s1;

	if(tr->trfl&BSVM2_TRFL_TJNEXT)
	{
		if(!(tr->trfl&BSVM2_TRFL_CANTHROW))
		{
			BS2J_CheckRestoreRegs(tr);
			basm_print("add esp, 60\n");
//			basm_print("mov esp, ebp; pop ebp\n");
//			basm_print("mov dword [esp+8], 0x%08X\n", (u32)(tr->jnext));
//			basm_print("jmp @0x%08X\n", (u32)(tr->jnext->Run));

			basm_print("mov edx, 0x%08X\n", (u32)(tr->jnext));
			basm_print("mov [esp+8], edx\n");
			basm_print("jmp dword [edx+%d]\n",
				offsetof(BSVM2_Trace, Run));
		}else
		{
			basm_print("mov eax, 0x%08X\n", (u32)(tr->jnext));
//			BS2J_CheckRestoreRegs(tr);
//			basm_print("mov esp, ebp; pop ebp; ret\n");
//			basm_print("add esp, 60\n");
//			basm_print("ret\n");
			BS2J_EmitNormalReturn(tr);
		}
		
		return(1);
	}

#if 1
	if((tr->top->opn==BSVM2_OP_CMPI) &&
		((tr->top->opn==BSVM2_OP_JEQ) ||
		 (tr->top->opn==BSVM2_OP_JNE) ||
		 (tr->top->opn==BSVM2_OP_JLT) ||
		 (tr->top->opn==BSVM2_OP_JGT) ||
		 (tr->top->opn==BSVM2_OP_JLE) ||
		 (tr->top->opn==BSVM2_OP_JGE)))
	{
		BS2J_CheckCacheFrameEDI(tr);
		BS2J_CheckCacheStackEBX(tr);
//		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov ecx, [ebx+%d]\n", tr->top->t0*8);
//		basm_print("mov edx, [ebx+%d]\n", tr->top->t1*8);

		basm_print("mov eax, 0x%08X\n", (u32)(tr->top->nexttrace));
//		basm_print("cmp ecx, edx\n");
		basm_print("cmp ecx, [ebx+%d]\n", tr->top->t1*8);

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
		basm_print("%s eax, [0x%08X]\n",
			s0, (u32)(&tr->top->jmptrace));
		BS2J_EmitNormalReturn(tr);
		return(1);
	}

//	if(tr->top->Run==BSVM2_TrOp_JCMP_GEI)
	if((tr->top->opn==BSVM2_OP_CMPILL) &&
		((tr->top->opn==BSVM2_OP_JEQ) ||
		 (tr->top->opn==BSVM2_OP_JNE) ||
		 (tr->top->opn==BSVM2_OP_JLT) ||
		 (tr->top->opn==BSVM2_OP_JGT) ||
		 (tr->top->opn==BSVM2_OP_JLE) ||
		 (tr->top->opn==BSVM2_OP_JGE)))
	{
		BS2J_CheckCacheFrameEDI(tr);
//		BS2J_CheckCacheStackEBX(tr);
		BS2J_CheckCacheLocalsESI(tr);
		basm_print("mov ecx, [esi+%d]\n", tr->top->i0*8);
//		basm_print("mov edx, [esi+%d]\n", tr->top->i1*8);

		basm_print("mov eax, 0x%08X\n", (u32)(tr->top->nexttrace));
//		basm_print("cmp ecx, edx\n");
		basm_print("cmp ecx, [esi+%d]\n", tr->top->i1*8);

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

//		basm_print("cmovge eax, [0x%08X]\n",
//			(u32)(&tr->top->jmptrace));
		basm_print("%s eax, [0x%08X]\n",
			s0, (u32)(&tr->top->jmptrace));

//		BS2J_CheckRestoreRegs(tr);
//		basm_print("add esp, 60\n");
//		basm_print("ret\n");
		BS2J_EmitNormalReturn(tr);
		return(1);
	}
#endif

#if 0
	BS2J_CheckFrameOnStack(tr);
	basm_print("mov eax, [ebp+12]; mov ecx, [eax+%d]\n",
		offsetof(BSVM2_Trace, top));
	basm_print("mov [esp+4], ecx\n");
	basm_print("call @0x%08X\n", (u32)(tr->top->Run));
//		basm_print("mov eax, 0x%08X; call eax\n", (u32)(tr->top->Run));

	BS2J_CheckRestoreRegs(tr);
	basm_print("mov esp, ebp; pop ebp; ret\n");
#endif

#if 1
	BS2J_CheckRestoreRegs(tr);
//	basm_print("mov esp, ebp; pop ebp\n");
	basm_print("add esp, 60\n");
	basm_print("mov dword [esp+8], 0x%08X\n", (u32)(tr->top));
	basm_print("jmp @0x%08X\n", (u32)(tr->top->Run));
#endif
	return(0);
}

int BS2J_CheckSetupJitTrace(BSVM2_Trace *tr)
{
	void *p;
	char *fn, *fnth;
	int i, j, k;
	
	fn=BASM_GenSym();
	if(tr->trfl&BSVM2_TRFL_CANTHROW)
		fnth=BASM_GenSym();

	BASM_BeginAssembly(NULL);
	
	basm_print("%s:\n", fn);

//	basm_print("push ebp; mov ebp, esp; sub esp, 24\n");
//	basm_print("push ebp; mov ebp, esp; sub esp, 56\n");
	basm_print("sub esp, 60\n");
	
	for(i=0; i<tr->n_ops; i++)
	{
		if(BS2J_TryJitOpcode(tr, tr->ops[i])>0)
			continue;

		BS2J_CheckFrameOnStack(tr);
		basm_print("mov dword [esp+4], 0x%08X\n", (u32)(tr->ops[i]));
		basm_print("call @0x%08X\n", (u32)(tr->ops[i]->Run));

		if(tr->ops[i]->opfl&BSVM2_TRFL_CANTHROW)
		{
			BS2J_CheckCacheFrameEDI(tr);
			basm_print("mov ecx, [edi+%d]\n",
				offsetof(BSVM2_Frame, ctx));
			basm_print("mov eax, [ecx+%d]\n",
				offsetof(BSVM2_Context, status));
			basm_print("and eax, eax\n");
			basm_print("jnz %s\n", fnth);
		}
	}

	BS2J_JitTraceHandleTail(tr);

	if(tr->trfl&BSVM2_TRFL_CANTHROW)
	{
		basm_print("%s:\n", fnth);

		basm_print("mov dword [ecx+%d], 0x%08X\n",
			offsetof(BSVM2_Context, trace), (u32)(tr));

		basm_print("xor eax, eax\n");
		BS2J_EmitNormalReturn(tr);
	}


	BASM_EndAssembly();
//	BASM_EndAssemblyQuiet();

	p=BASM_GetPtr(fn);
//	if(p) ((void (*)())p)();

	if(p)
	{
		tr->Run=p;
	}
	return(0);
}

#endif
