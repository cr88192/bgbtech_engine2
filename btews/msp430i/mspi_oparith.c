#include <bteifgl.h>

MSP430_Opcode *MSP430_OpMovRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	ctx->reg[op->dreg]=(u16)ctx->reg[op->sreg];
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpMovRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	ctx->reg[op->dreg]=(byte)ctx->reg[op->sreg];
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpMovRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	ctx->reg[op->dreg]=(u16)op->soffs;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpMovMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;
	a=MSP430_GetOpMemDest(ctx, op);
//	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	MSP430_VaWriteWord(ctx, a, j);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpMovMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;
	a=MSP430_GetOpMemDest(ctx, op);
//	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	MSP430_VaWriteByte(ctx, a, j);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpAddRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i+j;
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i+j;
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i+j;
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i+j;
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i+j;
	MSP430_VaWriteWord(ctx, a, k);
	MSP430_UpdateStatusW(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i+j;
	MSP430_VaWriteByte(ctx, a, k);
	MSP430_UpdateStatusW(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddPcRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
//	i=ctx->reg[MSP430_REG_PC];
	i=op->pc2;
	j=ctx->reg[op->sreg];
	k=i+j;
	
	k=(u16)k;
	ctx->reg[MSP430_REG_PC]=k;
//	MSP430_UpdateStatusW(ctx, i, j, k);

	if(op->jmpnext && (op->jmpnext->pc==k))
		return(op->jmpnext);

	op2=MSP430_GetDecodeOpcode(ctx, (u16)k);
	op->jmpnext=op2;
	return(op2);
}

MSP430_Opcode *MSP430_OpAddPcMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
//	i=ctx->reg[MSP430_REG_PC];
	i=op->pc2;
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i+j;
	k=(u16)k;
	ctx->reg[MSP430_REG_PC]=k;
//	MSP430_UpdateStatusW(ctx, i, j, k);

	if(op->jmpnext && (op->jmpnext->pc==k))
		return(op->jmpnext);

	op2=MSP430_GetDecodeOpcode(ctx, (u16)k);
	op->jmpnext=op2;
	return(op2);
}


MSP430_Opcode *MSP430_OpAddRRW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i+j;
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddRRB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i+j;
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddRIW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i+j;
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddRIB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i+j;
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddMW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i+j;
	MSP430_VaWriteWord(ctx, a, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddMB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i+j;
	MSP430_VaWriteByte(ctx, a, k);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpAddcRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddcRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddcRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddcRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddcMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	MSP430_VaWriteWord(ctx, a, k);
	MSP430_UpdateStatusW(ctx, i, j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddcMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	MSP430_VaWriteByte(ctx, a, k);
	MSP430_UpdateStatusW(ctx, i, j, k);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpAddcRRW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddcRRB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddcRIW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAddcRIB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i+j+(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpSubRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i-j;
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i-j;
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i-j;
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i-j;
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i-j;
	MSP430_VaWriteWord(ctx, a, k);
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i-j;
	MSP430_VaWriteByte(ctx, a, k);
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpSubRRW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i-j;
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubRRB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i-j;
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubRIW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i-j;
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubRIB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i-j;
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpSubcRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	MSP430_VaWriteWord(ctx, a, k);
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	MSP430_VaWriteByte(ctx, a, k);
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcRRW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcRRB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcRIW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSubcRIB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i-j-(ctx->reg[MSP430_REG_SR]&1);
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpCmpRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i-j;
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpCmpRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i-j;
	MSP430_UpdateStatusB(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpCmpRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i-j;
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpCmpRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i-j;
	MSP430_UpdateStatusB(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpCmpMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i-j;
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpCmpMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i-j;
	MSP430_UpdateStatusW(ctx, i, -j, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBitRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i&j;
//	ctx->reg[op->dreg]=k;
	MSP430_UpdateStatusW(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBitRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i&j;
//	ctx->reg[op->dreg]=k;
	MSP430_UpdateStatusB(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBitRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i&j;
//	ctx->reg[op->dreg]=k;
	MSP430_UpdateStatusW(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBitRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i&j;
//	ctx->reg[op->dreg]=k;
	MSP430_UpdateStatusB(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBitMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i&j;
//	MSP430_VaWriteWord(ctx, a, k);
	MSP430_UpdateStatusW(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBitMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i&j;
//	MSP430_VaWriteByte(ctx, a, k);
	MSP430_UpdateStatusB(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBicRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i&(~j);
	ctx->reg[op->dreg]=(u16)k;
//	MSP430_UpdateStatusW(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBicRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i&(~j);
	ctx->reg[op->dreg]=(byte)k;
//	MSP430_UpdateStatusB(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBicRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i&(~j);
	ctx->reg[op->dreg]=(u16)k;
//	MSP430_UpdateStatusW(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBicRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i&(~j);
	ctx->reg[op->dreg]=(byte)k;
//	MSP430_UpdateStatusB(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBicMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i&(~j);
	MSP430_VaWriteWord(ctx, a, k);
//	MSP430_UpdateStatusW(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBicMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i&(~j);
	MSP430_VaWriteByte(ctx, a, k);
//	MSP430_UpdateStatusW(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBisRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i|j;
	ctx->reg[op->dreg]=(u16)k;
//	MSP430_UpdateStatusW(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBisRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i|j;
	ctx->reg[op->dreg]=(byte)k;
//	MSP430_UpdateStatusB(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBisRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i|j;
	ctx->reg[op->dreg]=(u16)k;
//	MSP430_UpdateStatusW(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBisRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i|j;
	ctx->reg[op->dreg]=(byte)k;
//	MSP430_UpdateStatusB(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBisMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i|j;
	MSP430_VaWriteWord(ctx, a, k);
//	MSP430_UpdateStatusW(ctx, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpBisMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i|j;
	MSP430_VaWriteByte(ctx, a, k);
//	MSP430_UpdateStatusW(ctx, k);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpXorRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i^j;
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpXorRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i^j;
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpXorRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i^j;
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpXorRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i^j;
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpXorMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i^j;
	MSP430_VaWriteWord(ctx, a, k);
	MSP430_UpdateStatusW(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpXorMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i^j;
	MSP430_VaWriteByte(ctx, a, k);
	MSP430_UpdateStatusW(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpXorRRW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i^j;
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpXorRRB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i^j;
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpXorRIW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i^j;
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpXorRIB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i^j;
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpAndRRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i&j;
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAndRRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i&j;
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAndRIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i&j;
	ctx->reg[op->dreg]=(u16)k;
	MSP430_UpdateStatusW(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAndRIB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i&j;
	ctx->reg[op->dreg]=(byte)k;
	MSP430_UpdateStatusB(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAndMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=MSP430_GetOpMemSrcValueWord(ctx, op);
	k=i&j;
	MSP430_VaWriteWord(ctx, a, k);
	MSP430_UpdateStatusW(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAndMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=MSP430_GetOpMemSrcValueByte(ctx, op);
	k=i&j;
	MSP430_VaWriteByte(ctx, a, k);
	MSP430_UpdateStatusW(ctx, k, k, k|((!k)<<16));
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpAndRRW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[op->sreg];
	k=i&j;
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAndRRB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)ctx->reg[op->sreg];
	k=i&j;
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAndRIW_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=op->soffs;
	k=i&j;
	ctx->reg[op->dreg]=(u16)k;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpAndRIB_NoSR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=(byte)ctx->reg[op->dreg];
	j=(byte)op->soffs;
	k=i&j;
	ctx->reg[op->dreg]=(byte)k;
	return(op->seqnext);
}
