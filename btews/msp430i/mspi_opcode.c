#include <bteifgl.h>

MSP430_Opcode *MSP430_AllocOpcode(MSP430_Context *ctx)
{
	MSP430_Opcode *op;
	
	if(ctx->freeops)
	{
		op=ctx->freeops;
		ctx->freeops=op->next;
		memset(op, 0, sizeof(MSP430_Opcode));
		return(op);
	}
	
	op=malloc(sizeof(MSP430_Opcode));
	memset(op, 0, sizeof(MSP430_Opcode));
	return(op);
}

MSP430_Opcode *MSP430_LookupOpcode(MSP430_Context *ctx, int addr)
{
	MSP430_Opcode *cur;

	cur=ctx->ops;
	while(cur)
	{
		if(cur->pc==addr)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

MSP430_Opcode *MSP430_GetOpcode(MSP430_Context *ctx, int addr)
{
	MSP430_Opcode *cur;

	cur=MSP430_LookupOpcode(ctx, addr);
	if(cur)return(cur);

	cur=MSP430_AllocOpcode(ctx);
	cur->pc=addr;
	
	cur->next=ctx->ops;
	ctx->ops=cur;

	return(cur);
}

MSP430_Opcode *MSP430_LookupHashOpcode(MSP430_Context *ctx, int addr)
{
	MSP430_Opcode *cur;
	int hi;

//	hi=((addr*251)>>8)&255;
	hi=(byte)(addr^(addr>>8));
	cur=ctx->ophash[hi];
	while(cur)
	{
		if(cur->pc==addr)
			return(cur);
		cur=cur->hnext;
	}

	cur=MSP430_LookupOpcode(ctx, addr);
	if(cur)
	{
		cur->hnext=ctx->ophash[hi];
		ctx->ophash[hi]=cur;
		return(cur);
	}

	return(NULL);
}

int MSP430_GetOpMemDestI(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int a, a2, a3;

	if(op->dfl&MSP430_OPFL_AINC)
	{
		if(!(op->dfl&MSP430_OPFL_A20B))
		{
			a2=ctx->reg[op->dreg];
			a=a2+op->doffs;
			if((op->dfl&MSP430_OPFL_BYTE) && (op->dreg!=MSP430_REG_SP))
				{ a3=a2+1; }
			else
				{ a3=a2+2; }

			if(!(a2>>16))
			{
				a=(u16)a;
				a3=(u16)a3;
			}

			ctx->reg[op->dreg]=a3;
//			ctx->regh[op->dreg]=a2>>16;

//			a=ctx->reg[op->dreg]+op->doffs;
//			a=(u16)a;
//			if((op->dfl&MSP430_OPFL_BYTE) && (op->dreg!=MSP430_REG_SP))
//				{ ctx->reg[op->dreg]++; }
//			else
//				{ ctx->reg[op->dreg]+=2; }
		}else
		{
			a2=ctx->reg[op->dreg];
//			a2=ctx->reg[op->dreg]|(ctx->regh[op->dreg]<<16);
			a=a2+op->doffs;
			if((op->dfl&MSP430_OPFL_BYTE) && (op->dreg!=MSP430_REG_SP))
				{ a2++; }
			else
				{ a2+=2; }
			ctx->reg[op->dreg]=a2;
//			ctx->regh[op->dreg]=a2>>16;
		}
	}else
	{
		if(!(op->dfl&MSP430_OPFL_A20B))
		{
			a2=ctx->reg[op->dreg];
			a=a2+op->doffs;
			if(!(a2>>16))
				{ a=(u16)a; }

//			a=ctx->reg[op->dreg]+op->doffs;
//			a=(u16)a;
		}else
		{
			a2=ctx->reg[op->dreg];
//			a2=ctx->reg[op->dreg]|(ctx->regh[op->dreg]<<16);
			a=a2+op->doffs;
		}
//		a=(op->dfl&MSP430_OPFL_ABS)?0:ctx->reg[op->dreg];
//		a+=op->doffs;
	}
	return(a);
}

int MSP430_GetOpMemSrcI(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int a, a2, a3;

	if(op->sfl&MSP430_OPFL_AINC)
	{
		if(!(op->sfl&MSP430_OPFL_A20B))
		{
			a2=ctx->reg[op->sreg];
			a=a2+op->soffs;
			if((op->sfl&MSP430_OPFL_BYTE) && (op->sreg!=MSP430_REG_SP))
				{ a3=a2+1; }
			else
				{ a3=a2+2; }

			if(!(a2>>16))
			{
				a=(u16)a;
				a3=(u16)a3;
			}

			ctx->reg[op->sreg]=a3;

//			a=ctx->reg[op->sreg]+op->soffs;
//			a=(u16)a;
//			if((op->sfl&MSP430_OPFL_BYTE) && (op->sreg!=MSP430_REG_SP))
//				{ ctx->reg[op->sreg]++; }
//			else
//				{ ctx->reg[op->sreg]+=2; }
		}else
		{
			a2=ctx->reg[op->sreg];
			a=a2+op->soffs;
			if((op->sfl&MSP430_OPFL_BYTE) && (op->sreg!=MSP430_REG_SP))
				{ a2++; }
			else
				{ a2+=2; }
			ctx->reg[op->sreg]=a2;
		}
	}else
	{
		if(!(op->sfl&MSP430_OPFL_A20B))
		{
			a2=ctx->reg[op->sreg];
			a=a2+op->soffs;
			if(!(a2>>16))
				{ a=(u16)a; }

//			a=ctx->reg[op->sreg]+op->soffs;
//			a=(u16)a;
		}else
		{
			a2=ctx->reg[op->sreg];
			a=a2+op->soffs;
		}
	}
	return(a);
}

int MSP430_GetOpMemDest(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int a, a2;

	if(op->dfl&MSP430_OPFL_MEM)
	{
		a=MSP430_GetOpMemDestI(ctx, op);
		return(a);
	}

	if(op->dfl&MSP430_OPFL_IMM)
	{
		return(0xC0000000|op->doffs);
	}
	
	return(0xC1000000|op->dreg);
}

int MSP430_GetOpMemDestValueByte(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, a, a2;

	if(op->dfl&MSP430_OPFL_MEM)
	{
		a=MSP430_GetOpMemDestI(ctx, op);
		i=MSP430_VaReadByte(ctx, a);
		return(i);
	}

	if(op->dfl&MSP430_OPFL_IMM)
	{
		return((byte)op->doffs);
	}
	
	return((byte)ctx->reg[op->dreg]);
}

int MSP430_GetOpMemDestValueWord(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, a, a2;

	if(op->dfl&MSP430_OPFL_MEM)
	{
		a=MSP430_GetOpMemDestI(ctx, op);
		i=MSP430_VaReadWord(ctx, a);
		return(i);
	}

	if(op->dfl&MSP430_OPFL_IMM)
	{
		return(op->doffs);
	}
	
	return(ctx->reg[op->dreg]);
}

int MSP430_GetOpMemSrcValueByte(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, a;

	if(op->sfl&MSP430_OPFL_MEM)
	{
		a=MSP430_GetOpMemSrcI(ctx, op);
		i=MSP430_VaReadByte(ctx, a);
		return(i);
	}

	if(op->sfl&MSP430_OPFL_IMM)
	{
		return((byte)op->soffs);
	}
	
	return((byte)ctx->reg[op->sreg]);
}

int MSP430_GetOpMemSrcValueWord(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, a, a2;

	if(op->sfl&MSP430_OPFL_MEM)
	{
		a=MSP430_GetOpMemSrcI(ctx, op);
		i=MSP430_VaReadWord(ctx, a);
		return(i);
	}

	if(op->sfl&MSP430_OPFL_IMM)
	{
		return(op->soffs);
	}
	
	return(ctx->reg[op->sreg]);
}

void MSP430_UpdateStatusW(MSP430_Context *ctx, int a, int b, int k)
{
	int i;
	i=ctx->reg[MSP430_REG_SR];
	i&=~MSP430_SRM_ARITH;
	if(k&0x10000)
		i|=MSP430_SRM_C;
	if(!((u16)k))
		i|=MSP430_SRM_Z;
	if(k&0x8000)
		i|=MSP430_SRM_N;
//	if((k^(k>>1))&0x8000)
//		i|=MSP430_SRM_V;
//	if((a^b^k)&0x8000)
	if(!((a^b)&0x8000) && ((a^k)&0x8000))
		i|=MSP430_SRM_V;
	ctx->reg[MSP430_REG_SR]=i;
}

void MSP430_UpdateStatusB(MSP430_Context *ctx, int a, int b, int k)
{
	int i;
	i=ctx->reg[MSP430_REG_SR];
	i&=~MSP430_SRM_ARITH;
	if(k&0x100)
		i|=MSP430_SRM_C;
	if(!((byte)k))
		i|=MSP430_SRM_Z;
	if(k&0x80)
		i|=MSP430_SRM_N;
//	if((k^(k>>1))&0x80)
//		i|=MSP430_SRM_V;
//	if((a^b^k)&0x80)
	if(!((a^b)&0x80) && ((a^k)&0x80))
		i|=MSP430_SRM_V;
	ctx->reg[MSP430_REG_SR]=i;
}


MSP430_Opcode *MSP430_OpTrapInv(MSP430_Context *ctx, MSP430_Opcode *op)
{
	ctx->errParm1=op->pc;
	ctx->errParm2=op->opn;
	ctx->errStatus=MSP430_ERR_INV_OP;
	return(NULL);
}

void MSP430_DecodeOpcodeArgsUOp(MSP430_Context *ctx,
	MSP430_Opcode *cur, int opn, int *raddr)
{
	cur->dreg=opn&15;
	cur->sreg=-1;

	switch((opn>>4)&3)
	{
	case 0:
		if(cur->dreg==MSP430_REG_PC)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM|MSP430_OPFL_PCREL;
			cur->doffs=*raddr;
		}else if(cur->dreg==MSP430_REG_CG)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM;
			cur->doffs=0;
		}
		break;
	case 1:
		if(cur->dreg==MSP430_REG_PC)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_MEM|MSP430_OPFL_ABS|MSP430_OPFL_PCREL;
			cur->doffs=(s16)MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
			cur->doffs+=*raddr;
		}else if(cur->dreg==MSP430_REG_SR)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_MEM|MSP430_OPFL_ABS;
			cur->doffs=MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
		}else if(cur->dreg==MSP430_REG_CG)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM;
			cur->doffs=1;
		}else
		{
			cur->dfl|=MSP430_OPFL_MEM;
			cur->doffs=(s16)MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
		}
		break;
	case 2:
		if(cur->dreg==MSP430_REG_PC)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_MEM|MSP430_OPFL_ABS|MSP430_OPFL_PCREL;
			cur->doffs=*raddr;
		}else if(cur->dreg==MSP430_REG_SR)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM;
			cur->doffs=4;
		}else if(cur->dreg==MSP430_REG_CG)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM;
			cur->doffs=2;
		}else
		{
			cur->dfl|=MSP430_OPFL_MEM;
			cur->doffs=0;
		}
		break;
	case 3:
		if(cur->dreg==MSP430_REG_PC)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM;
			cur->doffs=MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
		}else if(cur->dreg==MSP430_REG_SR)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM;
			cur->doffs=8;
		}else if(cur->dreg==MSP430_REG_CG)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM;
			cur->doffs=-1;
		}else
		{
			cur->dfl|=MSP430_OPFL_MEM|MSP430_OPFL_AINC;
		}
		break;
	default:
		break;
	}
}


void MSP430_DecodeOpcodeArgsBinOp(MSP430_Context *ctx,
	MSP430_Opcode *cur, int opn, int *raddr)
{
	if(opn&0x0040)
	{
		cur->dfl|=MSP430_OPFL_BYTE;
		cur->sfl|=MSP430_OPFL_BYTE;
	}
//	if(opn&0x0080)
//		{ cur->fl|=MSP430_OPFL_AD; }

	cur->sreg=(opn>>8)&15;
	cur->dreg=opn&15;
	switch((opn>>4)&3)
	{
	case 0:
		if(cur->sreg==MSP430_REG_PC)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_IMM|MSP430_OPFL_PCREL;
			cur->soffs=*raddr;
		}else if(cur->sreg==MSP430_REG_CG)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_IMM;
			cur->soffs=0;
		}
		break;
	case 1:
		if(cur->sreg==MSP430_REG_PC)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_MEM|MSP430_OPFL_ABS|MSP430_OPFL_PCREL;
			cur->soffs=(s16)MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
			cur->soffs+=*raddr;
		}else if(cur->sreg==MSP430_REG_SR)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_MEM|MSP430_OPFL_ABS;
			cur->soffs=MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
		}else if(cur->sreg==MSP430_REG_CG)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_IMM;
			cur->soffs=1;
		}else
		{
			cur->sfl|=MSP430_OPFL_MEM;
			cur->soffs=(s16)MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
		}
		break;
	case 2:
		if(cur->sreg==MSP430_REG_PC)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_MEM|MSP430_OPFL_ABS|MSP430_OPFL_PCREL;
			cur->soffs=*raddr;
		}else if(cur->sreg==MSP430_REG_SR)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_IMM;
			cur->soffs=4;
		}else if(cur->sreg==MSP430_REG_CG)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_IMM;
			cur->soffs=2;
		}else
		{
			cur->sfl|=MSP430_OPFL_MEM;
			cur->soffs=0;
		}
		break;
	case 3:
		if(cur->sreg==MSP430_REG_PC)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_IMM;
			cur->soffs=MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
		}else if(cur->sreg==MSP430_REG_SR)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_IMM;
			cur->soffs=8;
		}else if(cur->sreg==MSP430_REG_CG)
		{
			cur->sreg=MSP430_REG_CG;
			cur->sfl|=MSP430_OPFL_IMM;
			cur->soffs=-1;
		}else
		{
			cur->sfl|=MSP430_OPFL_MEM|MSP430_OPFL_AINC;
		}
		break;
	default:
		break;
	}

	if(opn&0x0080)
	{
		if(cur->dreg==MSP430_REG_PC)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_MEM|MSP430_OPFL_ABS|MSP430_OPFL_PCREL;
			cur->doffs=(s16)MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
			cur->doffs+=*raddr;
		}else if(cur->dreg==MSP430_REG_SR)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_MEM|MSP430_OPFL_ABS;
			cur->doffs=MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
		}else
		{
			cur->dfl|=MSP430_OPFL_MEM;
			cur->doffs=(s16)MSP430_VaReadWordPC(ctx, *raddr);
			*raddr=*raddr+2;
		}
	}else
	{
#if 0
		if(cur->dreg==MSP430_REG_PC)
		{
			cur->dreg=MSP430_REG_CG;
			cur->dfl|=MSP430_OPFL_IMM;
			cur->doffs=*raddr;
		}
#endif
	}
}

void MSP430_DecodeOpcode_ClobberLSR(MSP430_Context *ctx,
	MSP430_Opcode *cur)
{
	if(!cur)
		return;

	switch(cur->opid)
	{
	case MSP430_OPID_ADD:
		if(cur->Run==MSP430_OpAddRIB)
			{ cur->Run=MSP430_OpAddRIB_NoSR; }
		else if(cur->Run==MSP430_OpAddRIW)
			{ cur->Run=MSP430_OpAddRIW_NoSR; }
		else if(cur->Run==MSP430_OpAddRRB)
			{ cur->Run=MSP430_OpAddRRB_NoSR; }
		else if(cur->Run==MSP430_OpAddRRW)
			{ cur->Run=MSP430_OpAddRRW_NoSR; }
		else if(cur->Run==MSP430_OpAddMW)
			{ cur->Run=MSP430_OpAddMW_NoSR; }
		else if(cur->Run==MSP430_OpAddMB)
			{ cur->Run=MSP430_OpAddMB_NoSR; }
		break;

	case MSP430_OPID_ADDC:
		if(cur->Run==MSP430_OpAddcRIB)
			{ cur->Run=MSP430_OpAddcRIB_NoSR; }
		else if(cur->Run==MSP430_OpAddcRIW)
			{ cur->Run=MSP430_OpAddcRIW_NoSR; }
		else if(cur->Run==MSP430_OpAddcRRB)
			{ cur->Run=MSP430_OpAddcRRB_NoSR; }
		else if(cur->Run==MSP430_OpAddcRRW)
			{ cur->Run=MSP430_OpAddcRRW_NoSR; }
		break;

	case MSP430_OPID_SUB:
		if(cur->Run==MSP430_OpSubRIB)
			{ cur->Run=MSP430_OpSubRIB_NoSR; }
		else if(cur->Run==MSP430_OpSubRIW)
			{ cur->Run=MSP430_OpSubRIW_NoSR; }
		else if(cur->Run==MSP430_OpSubRRB)
			{ cur->Run=MSP430_OpSubRRB_NoSR; }
		else if(cur->Run==MSP430_OpSubRRW)
			{ cur->Run=MSP430_OpSubRRW_NoSR; }
		break;

	case MSP430_OPID_SUBC:
		if(cur->Run==MSP430_OpSubcRIB)
			{ cur->Run=MSP430_OpSubcRIB_NoSR; }
		else if(cur->Run==MSP430_OpSubcRIW)
			{ cur->Run=MSP430_OpSubcRIW_NoSR; }
		else if(cur->Run==MSP430_OpSubcRRB)
			{ cur->Run=MSP430_OpSubcRRB_NoSR; }
		else if(cur->Run==MSP430_OpSubcRRW)
			{ cur->Run=MSP430_OpSubcRRW_NoSR; }
		break;

	case MSP430_OPID_XOR:
		if(cur->Run==MSP430_OpXorRIB)
			{ cur->Run=MSP430_OpXorRIB_NoSR; }
		else if(cur->Run==MSP430_OpXorRIW)
			{ cur->Run=MSP430_OpXorRIW_NoSR; }
		else if(cur->Run==MSP430_OpXorRRB)
			{ cur->Run=MSP430_OpXorRRB_NoSR; }
		else if(cur->Run==MSP430_OpXorRRW)
			{ cur->Run=MSP430_OpXorRRW_NoSR; }
		break;

	case MSP430_OPID_AND:
		if(cur->Run==MSP430_OpAndRIB)
			{ cur->Run=MSP430_OpAndRIB_NoSR; }
		else if(cur->Run==MSP430_OpAndRIW)
			{ cur->Run=MSP430_OpAndRIW_NoSR; }
		else if(cur->Run==MSP430_OpAndRRB)
			{ cur->Run=MSP430_OpAndRRB_NoSR; }
		else if(cur->Run==MSP430_OpAndRRW)
			{ cur->Run=MSP430_OpAndRRW_NoSR; }
		break;

	default:
		break;
	}
}

void MSP430_DecodeOpcode_CopyValues(MSP430_Context *ctx,
	MSP430_Opcode *dop, MSP430_Opcode *sop)
{
	dop->doffs=sop->doffs;
	dop->dreg=sop->dreg;
	dop->dfl=sop->dfl;

	dop->soffs=sop->soffs;
	dop->sreg=sop->sreg;
	dop->sfl=sop->sfl;

	dop->opid=sop->opid;
	dop->Run=sop->Run;
}

int MSP430_DecodeOpcodeI(MSP430_Context *ctx,
	MSP430_Opcode *cur, int opn, int *raddr)
{
	MSP430_Opcode *cur2;
	int etr, isdi;
	int i, j, k;

	isdi=(cur->opid==MSP430_OPID_DINTERP);

	etr=0;
	switch((opn>>12)&15)
	{
	case 0:
		etr=1;
		break;
	case 1:
		if((opn&0x0800) && !isdi)
		{
			cur2=MSP430_DecodeOpcode(ctx, raddr);
			if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
			{
				MSP430_DecodeOpcode_CopyValues(ctx, cur, cur2);
				cur->pc2=cur2->pc2;
				
				if(cur->sfl&MSP430_OPFL_MEM)
				{
					cur->sfl|=MSP430_OPFL_A20B;
					if(cur->sfl&MSP430_OPFL_PCREL)
					{
						i=(u16)(cur->soffs-(*raddr));
						i=i|(((opn>>8)&15)<<16);
						i=(i<<12)>>12;
						cur->soffs=i+(*raddr);
					}else
					{
						cur->soffs=((u16)cur->soffs)|(((opn>>8)&15)<<16);
					}
				
//					cur->soffs=((u16)cur->soffs)|(((opn>>8)&15)<<16);
				}

				if(cur->sfl&MSP430_OPFL_MEM)
				{
					cur->dfl|=MSP430_OPFL_A20B;

					if(cur->dfl&MSP430_OPFL_PCREL)
					{
						i=(u16)(cur->doffs-(*raddr));
						i=i|(((opn>>8)&15)<<16);
						i=(i<<12)>>12;
						cur->doffs=i+(*raddr);
					}else
					{
						cur->doffs=((u16)cur->doffs)|((opn&15)<<16);
					}
				}
				
				break;
			}
			
			if(opn&0x80)
			{
				cur->Run=MSP430_OpRepR;
				cur->dreg=opn&15;
			}else
			{
				cur->Run=MSP430_OpRepI;
				cur->doffs=opn&15;
			}
			
			break;
		}

		switch((opn>>7)&31)
		{
		case 0:
			MSP430_DecodeOpcodeArgsUOp(ctx, cur, opn, raddr);
			cur->opid=MSP430_OPID_RRC;

			MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
			ctx->tmp_lsrop=cur;

			if(cur->dfl&MSP430_OPFL_MEM)
			{
				if(opn&0x0040)
					{ cur->dfl|=MSP430_OPFL_BYTE; }
				if(opn&0x0040)
					{ cur->Run=MSP430_OpRrcMB; }
				else
					{ cur->Run=MSP430_OpRrcMW; }
			}else
			{
				if(opn&0x0040)
					{ cur->Run=MSP430_OpRrcRB; }
				else
					{ cur->Run=MSP430_OpRrcRW; }
			}
			break;
		case 1:
			MSP430_DecodeOpcodeArgsUOp(ctx, cur, opn, raddr);
			cur->opid=MSP430_OPID_SWPB;
			if(cur->dfl&MSP430_OPFL_MEM)
				{ cur->Run=MSP430_OpSwpbMW; }
			else
				{ cur->Run=MSP430_OpSwpbRW; }
			break;
		case 2:
			MSP430_DecodeOpcodeArgsUOp(ctx, cur, opn, raddr);
			cur->opid=MSP430_OPID_RRA;

			MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
			ctx->tmp_lsrop=cur;

			if(cur->dfl&MSP430_OPFL_MEM)
			{
				if(opn&0x0040)
					{ cur->dfl|=MSP430_OPFL_BYTE; }
				if(opn&0x0040)
					{ cur->Run=MSP430_OpRraMB; }
				else
					{ cur->Run=MSP430_OpRraMW; }
			}else
			{
				if(opn&0x0040)
					{ cur->Run=MSP430_OpRraRB; }
				else
					{ cur->Run=MSP430_OpRraRW; }
			}
			break;
		case 3:
			MSP430_DecodeOpcodeArgsUOp(ctx, cur, opn, raddr);
			cur->opid=MSP430_OPID_SXT;

			MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
			ctx->tmp_lsrop=cur;

			if(cur->dfl&MSP430_OPFL_MEM)
				{ cur->Run=MSP430_OpSxtMW; }
			else
				{ cur->Run=MSP430_OpSxtRW; }
			break;
		case 4:
			MSP430_DecodeOpcodeArgsUOp(ctx, cur, opn, raddr);
			cur->opid=MSP430_OPID_PUSH;
			
			if(cur->dreg==MSP430_REG_SR)
				ctx->tmp_lsrop=NULL;

			if(cur->dfl&MSP430_OPFL_MEM)
			{
				if(opn&0x0040)
					{ cur->dfl|=MSP430_OPFL_BYTE; }
				if(opn&0x0040)
					{ cur->Run=MSP430_OpPushMB; }
				else
					{ cur->Run=MSP430_OpPushMW; }
			}else
			{
				if(opn&0x0040)
					{ cur->Run=MSP430_OpPushRB; }
				else
					{ cur->Run=MSP430_OpPushRW; }
			}
			break;
		case 5:
			MSP430_DecodeOpcodeArgsUOp(ctx, cur, opn, raddr);
			cur->opid=MSP430_OPID_CALL;

			if(isdi)
			{
				cur->Run=MSP430_OpCallDiMW;
				break;
			}
			
			if(cur->dfl&MSP430_OPFL_MEM)
				{ cur->Run=MSP430_OpCallMW; }
			else if(cur->dfl&MSP430_OPFL_IMM)
				{ cur->Run=MSP430_OpCallIW; }
			else
				{ cur->Run=MSP430_OpCallRW; }
			break;
		case 6:
			cur->opid=MSP430_OPID_RETI;

			if(isdi)
			{
				cur->Run=MSP430_OpRetiDi;
				break;
			}

			cur->Run=MSP430_OpReti;
			break;
		default:
			etr=1;
			break;
		}
		break;

	case 2:
	case 3:
		i=opn&1023;
		i=(i<<22)>>22;
		cur->dfl|=MSP430_OPFL_IMM;
		cur->dreg=MSP430_REG_CG;
		cur->doffs=(*raddr)+(i<<1);
		cur->sreg=-1;
		
		ctx->tmp_lsrop=NULL;

		if(isdi)
		{
			switch((opn>>10)&7)
			{
			case 0:
				cur->opid=MSP430_OPID_JNE;
				cur->Run=MSP430_OpJneDi;
				break;
			case 1: 
				cur->opid=MSP430_OPID_JEQ;
				cur->Run=MSP430_OpJeqDi;
				break;
			case 2: 
				cur->opid=MSP430_OPID_JNC;
				cur->Run=MSP430_OpJncDi;
				break;
			case 3: 
				cur->opid=MSP430_OPID_JC;
				cur->Run=MSP430_OpJcDi;
				break;
			case 4: 
				cur->opid=MSP430_OPID_JN;
				cur->Run=MSP430_OpJnDi;
				break;
			case 5: 
				cur->opid=MSP430_OPID_JGE;
				cur->Run=MSP430_OpJgeDi;
				break;
			case 6: 
				cur->opid=MSP430_OPID_JL;
				cur->Run=MSP430_OpJlDi;
				break;
			case 7: 
				cur->opid=MSP430_OPID_JMP;
				cur->Run=MSP430_OpJmpDi;
				etr=1;
				break;
			default:
				break;
			}
			break;
		}

		switch((opn>>10)&7)
		{
		case 0:
			cur->opid=MSP430_OPID_JNE;
			cur->Run=MSP430_OpJne;
			break;
		case 1: 
			cur->opid=MSP430_OPID_JEQ;
			cur->Run=MSP430_OpJeq;
			break;
		case 2: 
			cur->opid=MSP430_OPID_JNC;
			cur->Run=MSP430_OpJnc;
			break;
		case 3: 
			cur->opid=MSP430_OPID_JC;
			cur->Run=MSP430_OpJc;
			break;
		case 4: 
			cur->opid=MSP430_OPID_JN;
			cur->Run=MSP430_OpJn;
			break;
		case 5: 
			cur->opid=MSP430_OPID_JGE;
			cur->Run=MSP430_OpJge;
			break;
		case 6: 
			cur->opid=MSP430_OPID_JL;
			cur->Run=MSP430_OpJl;
			break;
		case 7: 
			cur->opid=MSP430_OPID_JMP;
			cur->Run=MSP430_OpJmp;
			etr=1;
			break;
		default:
			break;
		}
		break;
	case 4:
		if(!isdi)
		{
			if(opn==0x4130)
			{
				cur->dreg=-1;
				cur->sreg=-1;

				cur->opid=MSP430_OPID_RET;
				cur->Run=MSP430_OpRet;
				etr=1;
				break;
			}

			if((opn&0xFFF0)==0x4130)
			{
				cur->dreg=opn&15;
				cur->sreg=-1;

				cur->opid=MSP430_OPID_POP;
				cur->Run=MSP430_OpPopRW;
				break;
			}

			if((opn&0xFFF0)==0x41B0)
			{
				cur->dreg=opn&15;
				cur->sreg=-1;

				cur->opid=MSP430_OPID_POP;
				cur->Run=MSP430_OpPopRB;
				break;
			}
		}else
		{
			if(opn==0x4130)
			{
				cur->dreg=-1;
				cur->sreg=-1;

				cur->opid=MSP430_OPID_RET;
				cur->Run=MSP430_OpRetDi;
				etr=1;
				break;
			}

#if 1
			if((opn&0xFFF0)==0x4130)
			{
				cur->dreg=opn&15;
				cur->sreg=-1;

				cur->opid=MSP430_OPID_POP;
				cur->Run=MSP430_OpPopRW;
				break;
			}
#endif
		}
	
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_MOV;
		if((cur->dreg==MSP430_REG_PC) &&
			!(cur->dfl&MSP430_OPFL_MEM) &&
			!isdi)
		{
			cur->opid=MSP430_OPID_BR;
			cur->Run=MSP430_OpBrMW;
			etr=1;
		}else if((cur->dfl&MSP430_OPFL_MEM) ||
			(cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpMovMB; }
			else
				{ cur->Run=MSP430_OpMovMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
			{ cur->Run=MSP430_OpMovRIW; }
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpMovRRB; }
			else
				{ cur->Run=MSP430_OpMovRRW; }
		}
		break;

	case 5:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_ADD;

		MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
		ctx->tmp_lsrop=cur;

		if((cur->dreg==MSP430_REG_PC) &&
			!(cur->dfl&MSP430_OPFL_MEM) &&
			!isdi)
		{
			if(cur->sfl&MSP430_OPFL_MEM)
				{ cur->Run=MSP430_OpAddPcMW; }
			else
				{ cur->Run=MSP430_OpAddPcRW; }
			etr=1;
			break;
		}
		
		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAddMB; }
			else
				{ cur->Run=MSP430_OpAddMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAddRIB; }
			else
				{ cur->Run=MSP430_OpAddRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAddRRB; }
			else
				{ cur->Run=MSP430_OpAddRRW; }
		}
		break;
	case 6:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_ADDC;

//		MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
		ctx->tmp_lsrop=cur;

		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAddcMB; }
			else
				{ cur->Run=MSP430_OpAddcMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAddcRIB; }
			else
				{ cur->Run=MSP430_OpAddcRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAddcRRB; }
			else
				{ cur->Run=MSP430_OpAddcRRW; }
		}
		break;

	case 7:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_SUBC;

//		MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
		ctx->tmp_lsrop=cur;

		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpSubcMB; }
			else
				{ cur->Run=MSP430_OpSubcMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpSubcRIB; }
			else
				{ cur->Run=MSP430_OpSubcRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpSubcRRB; }
			else
				{ cur->Run=MSP430_OpSubcRRW; }
		}
		break;
	case 8:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_SUB;

		MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
		ctx->tmp_lsrop=cur;

		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpSubMB; }
			else
				{ cur->Run=MSP430_OpSubMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpSubRIB; }
			else
				{ cur->Run=MSP430_OpSubRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpSubRRB; }
			else
				{ cur->Run=MSP430_OpSubRRW; }
		}
		break;

	case 9:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_CMP;

		MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
		ctx->tmp_lsrop=cur;

		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpCmpMB; }
			else
				{ cur->Run=MSP430_OpCmpMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpCmpRIB; }
			else
				{ cur->Run=MSP430_OpCmpRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpCmpRRB; }
			else
				{ cur->Run=MSP430_OpCmpRRW; }
		}
		break;

	case 11:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_BIT;

		MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
		ctx->tmp_lsrop=cur;

		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBitMB; }
			else
				{ cur->Run=MSP430_OpBitMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBitRIB; }
			else
				{ cur->Run=MSP430_OpBitRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBitRRB; }
			else
				{ cur->Run=MSP430_OpBitRRW; }
		}
		break;

	case 12:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_BIC;
		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBicMB; }
			else
				{ cur->Run=MSP430_OpBicMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBicRIB; }
			else
				{ cur->Run=MSP430_OpBicRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBicRRB; }
			else
				{ cur->Run=MSP430_OpBicRRW; }
		}
		break;

	case 13:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_BIS;
		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBisMB; }
			else
				{ cur->Run=MSP430_OpBisMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBisRIB; }
			else
				{ cur->Run=MSP430_OpBisRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpBisRRB; }
			else
				{ cur->Run=MSP430_OpBisRRW; }
		}
		break;

	case 14:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_XOR;

		MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
		ctx->tmp_lsrop=cur;

		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpXorMB; }
			else
				{ cur->Run=MSP430_OpXorMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpXorRIB; }
			else
				{ cur->Run=MSP430_OpXorRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpXorRRB; }
			else
				{ cur->Run=MSP430_OpXorRRW; }
		}
		break;

	case 15:
		MSP430_DecodeOpcodeArgsBinOp(ctx, cur, opn, raddr);
		cur->opid=MSP430_OPID_AND;

		MSP430_DecodeOpcode_ClobberLSR(ctx, ctx->tmp_lsrop);
		ctx->tmp_lsrop=cur;

		if((cur->dfl&MSP430_OPFL_MEM) || (cur->sfl&MSP430_OPFL_MEM))
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAndMB; }
			else
				{ cur->Run=MSP430_OpAndMW; }
		}else if(cur->sfl&MSP430_OPFL_IMM)
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAndRIB; }
			else
				{ cur->Run=MSP430_OpAndRIW; }
		}
		else
		{
			if(cur->dfl&MSP430_OPFL_BYTE)
				{ cur->Run=MSP430_OpAndRRB; }
			else
				{ cur->Run=MSP430_OpAndRRW; }
		}
		break;

	default:
		etr=1;
		break;
	}
	
	return(etr);
}

MSP430_Opcode *MSP430_DecodeOpcode(MSP430_Context *ctx, int *raddr)
{
	MSP430_Opcode *cur, *cur2;
	int addr, opn, etr;
	int i, j, k;
	
	addr=*raddr;
	cur=MSP430_GetOpcode(ctx, addr);
	if(cur->Run)
	{
		*raddr=-1;
		return(cur);
	}
	
	opn=MSP430_VaReadWordPC(ctx, addr);
	cur->opn=opn;
	*raddr=addr+2;
	
	cur->Run=MSP430_OpTrapInv;
	etr=0;

	etr=MSP430_DecodeOpcodeI(ctx, cur, opn, raddr);

	cur->pc2=*raddr;

	if(etr || (*raddr>=0x10000))
	{
		*raddr=-1;
	}
	
	return(cur);
}

MSP430_Opcode *MSP430_DirectInterpOpcode(MSP430_Context *ctx,
	MSP430_Opcode *cur)
{
	MSP430_Opcode *cur2;
	int addr, addr2, opn, etr;
	int i, j, k;

	ctx->tmp_lsrop=NULL;
	
	addr=ctx->reg[MSP430_REG_PC];
	cur->pc=addr;

	cur->dreg=0;	cur->sreg=0;
	cur->dfl=0;		cur->sfl=0;
	cur->doffs=0;	cur->soffs=0;
	
	cur->seqnext=NULL;
	cur->jmpnext=NULL;
	cur->opid=MSP430_OPID_DINTERP;
	cur->Run=MSP430_OpTrapInv;
	etr=0;

	opn=MSP430_VaReadWordPC(ctx, addr);
	cur->opn=opn;
	addr=addr+2;

	addr2=addr;
	etr=MSP430_DecodeOpcodeI(ctx, cur, opn, &addr2);
	cur->pc2=addr2;

	if(addr2<addr)
	{
		*(int *)-1=-1;
	}

	ctx->reg[MSP430_REG_PC]=addr2;

//	MSP430_DumpOpcode(ctx, cur);

	cur2=cur->Run(ctx, cur);	
	if(cur2)
	{
		return(cur2);
	}

//	return(cur);

	i=(ctx->dioprov+1)&63;
	ctx->dioprov=i;
		
	cur=ctx->tmp_diop[i];
	if(!cur)
	{
		cur=MSP430_AllocOpcode(ctx);
		ctx->tmp_diop[i]=cur;
	}

	addr=ctx->reg[MSP430_REG_PC];
	cur->pc=addr;
	cur->pc=addr;
	cur->Run=MSP430_DirectInterpOpcode;
	return(cur);

}

MSP430_Opcode *MSP430_GetDecodeOpcode(MSP430_Context *ctx, int addr)
{
	MSP430_Opcode *cur, *ofst, *olst;
	int a1, hi;
	int i, j, k;
	
	cur=MSP430_LookupHashOpcode(ctx, addr);
	if(cur)return(cur);

	if(addr&1)
		return(NULL);

#if 1
//	if(1)
	if(!MSP430_VaAddrIsRomP(ctx, addr))
	{
		i=(ctx->dioprov+1)&63;
		ctx->dioprov=i;
		
		cur=ctx->tmp_diop[i];
		if(!cur)
		{
			cur=MSP430_AllocOpcode(ctx);
			ctx->tmp_diop[i]=cur;
		}
		cur->Run=MSP430_DirectInterpOpcode;
		return(cur);
	}
#endif
	
	ctx->tmp_lsrop=NULL;
	a1=addr;
	ofst=MSP430_DecodeOpcode(ctx, &a1); olst=ofst;
	while(a1>0)
	{
		cur=MSP430_DecodeOpcode(ctx, &a1);
		olst->seqnext=cur;
		olst=cur;
	}
	
	if(ofst)
	{
//		hi=((addr*251)>>8)&255;
		hi=(byte)(addr^(addr>>8));
		ofst->hnext=ctx->ophash[hi];
		ctx->ophash[hi]=ofst;
	}
	return(ofst);
}

MSP430_Opcode *MSP430_GetDecodeRomOpcode(MSP430_Context *ctx, int addr)
{
	MSP430_Opcode *cur;

#if 1
	if(MSP430_VaAddrIsRomP(ctx, addr))
	{
		cur=MSP430_GetDecodeOpcode(ctx, addr);
		return(cur);
	}
#endif

	cur=MSP430_LookupHashOpcode(ctx, addr);
	return(cur);
}

char *MSP430_DumpOpcode_NameForOp(MSP430_Context *ctx, MSP430_Opcode *cur)
{
	char *s;
	switch(cur->opid)
	{
		case MSP430_OPID_UNK:	s=".UNK"; break;
		case MSP430_OPID_ADD:	s="ADD";  break;
		case MSP430_OPID_ADDC:	s="ADDC"; break;
		case MSP430_OPID_AND:	s="AND";  break;
		case MSP430_OPID_BIC:	s="BIC";  break;
		case MSP430_OPID_BIS:	s="BIS";  break;
		case MSP430_OPID_BIT:	s="BIT";  break;
		case MSP430_OPID_BR:	s="BR";   break;
		case MSP430_OPID_CALL:	s="CALL"; break;
		case MSP430_OPID_CMP:	s="CMP";  break;
		case MSP430_OPID_DADD:	s="DADD"; break;
		case MSP430_OPID_JC:	s="JC";   break;
		case MSP430_OPID_JEQ:	s="JEQ";  break;
		case MSP430_OPID_JGE:	s="JGE";  break;
		case MSP430_OPID_JL:	s="JL";   break;
		case MSP430_OPID_JMP:	s="JMP";  break;
		case MSP430_OPID_JN:	s="JN";   break;
		case MSP430_OPID_JNC:	s="JNC";  break;
		case MSP430_OPID_JNE:	s="JNE";  break;
		case MSP430_OPID_MOV:	s="MOV";  break;
		case MSP430_OPID_POP:	s="POP";  break;
		case MSP430_OPID_PUSH:	s="PUSH"; break;
		case MSP430_OPID_RET:	s="RET";  break;
		case MSP430_OPID_RETI:	s="RETI"; break;
		case MSP430_OPID_RRA:	s="RRA";  break;
		case MSP430_OPID_RRC:	s="RRC";  break;
		case MSP430_OPID_SUB:	s="SUB";  break;
		case MSP430_OPID_SUBC:	s="SUBC"; break;
		case MSP430_OPID_SWPB:	s="SWPB"; break;
		case MSP430_OPID_SXT:	s="SXT";  break;
		case MSP430_OPID_XOR:	s="XOR";  break;
		
		case MSP430_OPID_DINTERP:	s=".DINT"; break;
		default: s=".UNK2"; break;
	}
	return(s);
}

char *MSP430_DumpOpcode_NameForReg(MSP430_Context *ctx, int reg)
{
	char *s;
	switch(reg)
	{
		case  0: s="PC"; break;
		case  1: s="SP"; break;
		case  2: s="SR"; break;
		case  3: s="CG"; break;
		case  4: s="R4"; break;
		case  5: s="R5"; break;
		case  6: s="R6"; break;
		case  7: s="R7"; break;
		case  8: s="R8"; break;
		case  9: s="R9"; break;
		case 10: s="R10"; break;
		case 11: s="R11"; break;
		case 12: s="R12"; break;
		case 13: s="R13"; break;
		case 14: s="R14"; break;
		case 15: s="R15"; break;
		default:
			s="<ERROR>"; break;
	}
	return(s);
}

void MSP430_DumpOpcode_Param(MSP430_Context *ctx, MSP430_Opcode *cur,
	int fl, int reg, int offs)
{
	int a, v;
	if(fl&MSP430_OPFL_MEM)
	{
		if(fl&MSP430_OPFL_ABS)
		{
			a=offs;
//			printf("[0x%04X]", offs);
			printf("[0x%05X]", offs&0xFFFFF);
		}else if(fl&MSP430_OPFL_AINC)
		{
			if(offs)
			{
				a=(u16)(ctx->reg[reg]+offs);
//				printf("[(%s++)+0x%04X]",
//					MSP430_DumpOpcode_NameForReg(ctx, reg),
//					offs);
				printf("[(%s++)+0x%05X]",
					MSP430_DumpOpcode_NameForReg(ctx, reg),
					offs&0xFFFFF);
			}else
			{
				a=ctx->reg[reg];
				printf("[%s++]",
					MSP430_DumpOpcode_NameForReg(ctx, reg));
			}
		}else
		{
			if(offs)
			{
				a=(u16)(ctx->reg[reg]+offs);
//				printf("[%s+0x%04X]",
//					MSP430_DumpOpcode_NameForReg(ctx, reg),
//					offs);
				printf("[%s+0x%05X]",
					MSP430_DumpOpcode_NameForReg(ctx, reg),
					offs&0xFFFFF);
			}else
			{
				a=ctx->reg[reg];
				printf("[%s]",
					MSP430_DumpOpcode_NameForReg(ctx, reg));
			}
		}

#if 0
		if(fl&MSP430_OPFL_BYTE)
		{
			v=MSP430_VaReadByte(ctx, a);
			printf("=%02X", v);
		}else
		{
			v=MSP430_VaReadWord(ctx, a);
			printf("=%04X", v);
		}
#endif
	}else if(fl&MSP430_OPFL_IMM)
	{
//		printf("0x%04X", offs);
		printf("0x%05X", offs&0xFFFFF);
	}else
	{
		printf("%s",
			MSP430_DumpOpcode_NameForReg(ctx, reg));

//		printf("%s=%04X",
//			MSP430_DumpOpcode_NameForReg(ctx, reg),
//			ctx->reg[reg]);
	}
}

void MSP430_DumpOpcode(MSP430_Context *ctx, MSP430_Opcode *cur)
{
	int i, j, n;

	if(!cur)
		return;

	printf("@%04X: ", cur->pc);

	n=(cur->pc2-cur->pc)/2;
	for(i=0; i<n; i++)
	{
		j=MSP430_VaReadWord(ctx, cur->pc+i*2);
		printf("%04X ", j);
	}
	for(; i<4; i++)
	{
		printf("     ", j);
	}

	printf("%s", MSP430_DumpOpcode_NameForOp(ctx, cur));
	if(cur->dfl&MSP430_OPFL_BYTE)
		printf(".B");

	if(((signed char)cur->dreg)>=0)
	{
		printf(" ");
		MSP430_DumpOpcode_Param(ctx, cur,
			cur->dfl, cur->dreg, cur->doffs);

		if(((signed char)cur->sreg)>=0)
		{
			printf(", ");
			MSP430_DumpOpcode_Param(ctx, cur,
				cur->sfl, cur->sreg, cur->soffs);
		}
	}
	
	switch(cur->opid)
	{
	case MSP430_OPID_JC:	case MSP430_OPID_JEQ:
	case MSP430_OPID_JGE:	case MSP430_OPID_JL:
	case MSP430_OPID_JN:	case MSP430_OPID_JNC:
	case MSP430_OPID_JNE:
		printf("  (SR=%04X)", ctx->reg[MSP430_REG_SR]);
		break;
	default:
		break;
	}

	printf("\n");
}
