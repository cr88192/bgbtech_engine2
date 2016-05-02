#include <bteifgl.h>

BSVM2_Opcode *BSVM2_Interp_AllocOpcode(BSVM2_CodeBlock *cblk)
{
	BSVM2_Opcode *tmp;
	
	if(cblk->img->opfree)
	{
		tmp=cblk->img->opfree;
		cblk->img->opfree=*(BSVM2_Opcode **)tmp;
		memset(tmp, 0, sizeof(BSVM2_Opcode));
		return(tmp);
	}
	
	tmp=dtmAlloc("bsvm2_opcode_t", sizeof(BSVM2_Opcode));
	return(tmp);
}

BSVM2_TailOpcode *BSVM2_Interp_AllocTailOpcode(BSVM2_CodeBlock *cblk)
{
	BSVM2_TailOpcode *tmp;
	
	if(cblk->img->topfree)
	{
		tmp=cblk->img->topfree;
		cblk->img->topfree=*(BSVM2_TailOpcode **)tmp;
		memset(tmp, 0, sizeof(BSVM2_TailOpcode));
		return(tmp);
	}
	
	tmp=dtmAlloc("bsvm2_tailopcode_t", sizeof(BSVM2_TailOpcode));
	return(tmp);
}

BSVM2_Trace *BSVM2_Interp_AllocTrace(BSVM2_CodeBlock *cblk)
{
	BSVM2_Trace *tmp;
	
	if(cblk && cblk->img->trfree)
	{
		tmp=cblk->img->trfree;
		cblk->img->trfree=*(BSVM2_Trace **)tmp;
		memset(tmp, 0, sizeof(BSVM2_Trace));
		return(tmp);
	}
	
	tmp=dtmAlloc("bsvm2_trace_t", sizeof(BSVM2_Trace));
	return(tmp);
}

void BSVM2_Interp_FreeOpcode(BSVM2_CodeBlock *cblk, BSVM2_Opcode *tmp)
{
	*(BSVM2_Opcode **)tmp=cblk->img->opfree;
	cblk->img->opfree=tmp;
}	

void BSVM2_Interp_FreeTailOpcode(BSVM2_CodeBlock *cblk,
	BSVM2_TailOpcode *tmp)
{
	*(BSVM2_TailOpcode **)tmp=cblk->img->topfree;
	cblk->img->topfree=tmp;
}	

int BSVM2_Interp_ReadOpcodeNumber(BSVM2_CodeBlock *cblk)
{
	int i;

	i=(*cblk->cs++);
	if(i<0xE0)
		return(i);

	if((i>=0xE0) && (i<=0xEF))
	{
		i=((i-0xE0)<<8)|(*cblk->cs++);
	}else if((i>=0xF0) && (i<=0xF7))
	{
		i=i-0xF0;
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
	}
	
	return(i);
}

int BSVM2_Interp_DecodeBlockNoexOp(BSVM2_CodeBlock *cblk, int opn)
{
	switch(opn)
	{
	case BSVM2_OP_POPI:		case BSVM2_OP_POPL:
	case BSVM2_OP_POPF:		case BSVM2_OP_POPD:
	case BSVM2_OP_POPA:
		cblk->stkpos--;
		break;
	case BSVM2_OP_PUSHI:	case BSVM2_OP_PUSHL:
	case BSVM2_OP_PUSHF:	case BSVM2_OP_PUSHD:
	case BSVM2_OP_PUSHA:
		cblk->stkpos++;
		break;
	default:
		break;
	}
	return(0);
}

BSVM2_Trace *BSVM2_Interp_DecodeBlockTraces(BSVM2_CodeBlock *cblk)
{
	BSVM2_Trace *trsa[1024];
	BSVM2_Opcode *opsa[64];

	BSVM2_Opcode *op;
	BSVM2_TailOpcode *top;
	BSVM2_Trace *tr, *tr1;
	byte *csa, *jcs;
	int opn, nopsa, ntrsa;
	int i, j, k, n;

	cblk->cs=cblk->code;
	cblk->cse=cblk->code+cblk->szcode;
	
	nopsa=0; ntrsa=0; csa=cblk->cs;
	while(cblk->cs<cblk->cse)
	{
		opn=BSVM2_Interp_ReadOpcodeNumber(cblk);
		
		if(opn==BSVM2_OP_JMP)
		{
			jcs=BSVM2_Interp_DecodeOpJAddr(cblk);

			tr=BSVM2_Interp_DecodeBlockSetupTraceDfl(
				cblk, opsa, nopsa, NULL, 1);			
			trsa[ntrsa++]=tr;
			tr->cs=csa;
			tr->jcs=jcs;
			csa=cblk->cs; nopsa=0;
			continue;
		}
		
		if((opn==BSVM2_OP_LABEL) || (opn==BSVM2_OP_LBLCLNP))
		{
			tr=BSVM2_Interp_DecodeBlockSetupTraceDfl(
				cblk, opsa, nopsa, NULL, 1);			
			trsa[ntrsa++]=tr;
			tr->cs=csa;
			tr->jcs=cblk->cs;
			csa=cblk->cs; nopsa=0;
			continue;
		}

		if((opn==BSVM2_OP_HNOEX1) ||
			(opn==BSVM2_OP_HNOEX2) ||
			(opn==BSVM2_OP_HNOEX3))
		{
			n=(opn-BSVM2_OP_HNOEX1)+1;
			for(i=0; i<n; i++)
			{	opn=BSVM2_Interp_ReadOpcodeNumber(cblk);
				BSVM2_Interp_DecodeBlockNoexOp(cblk, opn);	}
			csa=cblk->cs; nopsa=0;
			continue;
		}

		if(opn==BSVM2_OP_HNOEX)
		{
			while(cblk->cs<cblk->cse)
			{
				opn=BSVM2_Interp_ReadOpcodeNumber(cblk);
				if(opn==BSVM2_OP_LABEL)break;
				BSVM2_Interp_DecodeBlockNoexOp(cblk, opn);
			}
			csa=cblk->cs; nopsa=0;
			continue;
		}
		
		op=BSVM2_Interp_DecodeOpcode(cblk, opn);
		if(op && (nopsa<63))
		{
			opsa[nopsa++]=op;
			continue;
		}
		
		if(op)
		{
			opsa[nopsa++]=op;
			tr=BSVM2_Interp_DecodeBlockSetupTraceDfl(
				cblk, opsa, nopsa, NULL, 1);			
			trsa[ntrsa++]=tr;
			tr->cs=csa;
			tr->jcs=cblk->cs;
			csa=cblk->cs; nopsa=0;
			continue;
		}
		
		top=BSVM2_Interp_DecodeTailOpcode(cblk, opn);
		if(top)
		{
			tr=BSVM2_Interp_DecodeBlockSetupTraceDfl(
				cblk, opsa, nopsa, top, 0);			
			trsa[ntrsa++]=tr;
			tr->cs=csa;
			tr->jcs=cblk->cs;
			if(top->jcs)
				tr->jcs=top->jcs;
			csa=cblk->cs; nopsa=0;
			continue;
		}
	}
	trsa[ntrsa]=NULL;
	
	for(i=0; i<ntrsa; i++)
	{
		tr=trsa[i];
		tr1=trsa[i+1];
		if(((i+1)<ntrsa) && (tr->jcs==tr1->cs))
		{
			tr->jnext=tr1;
			if(tr->top)
			{
				tr->top->nexttrace=tr1;
				tr->top->jmptrace=tr1;
			}
			continue;
		}
		
		if(tr->top)
			{ tr->top->nexttrace=tr1; }
		for(j=0; j<ntrsa; j++)
		{
			tr1=trsa[j];
			if(tr->jcs==tr1->cs)
				break;
		}

		tr->jnext=tr1;
		if(tr->top)
			{ tr->top->jmptrace=tr1; }
	}
	
	cblk->trace=dtmAlloc("bsvm2_tracearr_t",
		ntrsa*sizeof(BSVM2_Trace *));
	cblk->ntrace=ntrsa;
	
	for(i=0; i<ntrsa; i++)
		{ cblk->trace[i]=trsa[i]; }
	return(cblk->trace[0]);
}
