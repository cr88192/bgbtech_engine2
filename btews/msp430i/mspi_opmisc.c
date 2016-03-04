#include <bteifgl.h>


MSP430_Opcode *MSP430_OpRrcRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[MSP430_REG_SR];
//	ctx->reg[MSP430_REG_SR]=(j&(~1))|(i&1);
	k=(byte)((i>>1)|(j<<7));
	ctx->reg[op->dreg]=k;
	k=k|(i<<8);
	MSP430_UpdateStatusB(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpRrcRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=ctx->reg[MSP430_REG_SR];
//	ctx->reg[MSP430_REG_SR]=(j&(~1))|(i&1);
	k=(u16)((i>>1)|(j<<15));
	ctx->reg[op->dreg]=k;
	k=k|(i<<16);
	MSP430_UpdateStatusW(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpRrcMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
	j=ctx->reg[MSP430_REG_SR];
	ctx->reg[MSP430_REG_SR]=(j&(~1))|(i&1);
	k=(byte)((i>>1)|(j<<7));
	MSP430_VaWriteByte(ctx, a, k);
	k=k|(i<<8);
	MSP430_UpdateStatusB(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpRrcMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	j=ctx->reg[MSP430_REG_SR];
	ctx->reg[MSP430_REG_SR]=(j&(~1))|(i&1);
	k=(u16)((i>>1)|(j<<15));
	MSP430_VaWriteWord(ctx, a, k);
	k=k|(i<<16);
	MSP430_UpdateStatusW(ctx, k, k, k);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpSwpbRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j;
	i=ctx->reg[op->dreg];
	i=((i>>8)&255)|((i&255)<<8);
	ctx->reg[op->dreg]=i;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSwpbMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	i=((i>>8)&255)|((i&255)<<8);
	MSP430_VaWriteWord(ctx, a, i);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpRraRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=((signed char)i)>>1;
	ctx->reg[op->dreg]=j;
//	MSP430_UpdateStatusB(ctx, j|(i<<8));
	k=j|(i<<8);
	MSP430_UpdateStatusB(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpRraRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k;
	i=ctx->reg[op->dreg];
	j=((signed short)i)>>1;
	ctx->reg[op->dreg]=j;
//	MSP430_UpdateStatusW(ctx, j|(i<<16));
	k=j|(i<<16);
	MSP430_UpdateStatusW(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpRraMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadByte(ctx, a);
//	i=(byte)((i>>1)|(i<<7));
	j=((signed char)i)>>1;
	MSP430_VaWriteByte(ctx, a, j);
//	MSP430_UpdateStatusB(ctx, j|(i<<8));
	k=j|(i<<8);
	MSP430_UpdateStatusB(ctx, k, k, k);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpRraMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, k, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
//	i=(u16)((i>>1)|(i<<15));
	j=((s16)i)>>1;
	MSP430_VaWriteWord(ctx, a, j);
//	MSP430_UpdateStatusW(ctx, j|(i<<16));
	k=j|(i<<16);
	MSP430_UpdateStatusW(ctx, k, k, k);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpSxtRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j;
	i=ctx->reg[op->dreg];
	i=(signed char)i;
	ctx->reg[op->dreg]=i;
//	MSP430_UpdateStatusW(ctx, i);
	j=((u16)i)|((!i)<<16);
	MSP430_UpdateStatusW(ctx, j, j, j);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpSxtMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, a;
	a=MSP430_GetOpMemDest(ctx, op);
	i=MSP430_VaReadWord(ctx, a);
	i=(signed char)i;
	MSP430_VaWriteWord(ctx, a, i);
//	MSP430_UpdateStatusW(ctx, i);
	j=((u16)i)|((!i)<<16);
	MSP430_UpdateStatusW(ctx, j, j, j);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpPushRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, a;
	i=(byte)ctx->reg[op->dreg];
	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, i);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpPushRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, a;
	i=ctx->reg[op->dreg];
	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, i);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpPushMB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, a;
//	a=MSP430_GetOpMemDest(ctx, op);
//	i=MSP430_VaReadByte(ctx, a);
	i=MSP430_GetOpMemDestValueByte(ctx, op);
	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, i);
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpPushMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, a;
//	a=MSP430_GetOpMemDest(ctx, op);
//	i=MSP430_VaReadWord(ctx, a);
	i=MSP430_GetOpMemDestValueWord(ctx, op);
	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, i);
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpPopRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, a;

	a=ctx->reg[MSP430_REG_SP];
	i=MSP430_VaReadWordSP(ctx, a);
	ctx->reg[MSP430_REG_SP]=a+2;
	ctx->reg[op->dreg]=i;
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpPopRB(MSP430_Context *ctx, MSP430_Opcode *op)
{
	int i, j, a;

	a=ctx->reg[MSP430_REG_SP];
//	i=MSP430_VaReadByte(ctx, a);
	i=MSP430_VaReadWordSP(ctx, a);
	ctx->reg[MSP430_REG_SP]=a+2;
	ctx->reg[op->dreg]=(byte)i;
	return(op->seqnext);
}


MSP430_Opcode *MSP430_OpCallRW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;

	i=ctx->reg[op->dreg];
	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, op->pc2);

	ctx->reg[MSP430_REG_PC]=i;

	if(op->jmpnext && (op->jmpnext->pc==i))
		return(op->jmpnext);
	op2=MSP430_GetDecodeOpcode(ctx, i);
	op->jmpnext=op2;
	return(op2);
}

MSP430_Opcode *MSP430_OpCallMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;
//	a=MSP430_GetOpMemDest(ctx, op);
//	i=MSP430_VaReadWord(ctx, a);
	i=MSP430_GetOpMemDestValueWord(ctx, op);

	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, op->pc2);
	
	ctx->reg[MSP430_REG_PC]=i;

	if(op->jmpnext && (op->jmpnext->pc==i))
		return(op->jmpnext);
	op2=MSP430_GetDecodeOpcode(ctx, i);
	op->jmpnext=op2;
	return(op2);
}

MSP430_Opcode *MSP430_OpCallIW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;

	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, op->pc2);	

	if(op->jmpnext)
		return(op->jmpnext);
	
	i=(u16)op->doffs;
	ctx->reg[MSP430_REG_PC]=i;
	op2=MSP430_GetDecodeOpcode(ctx, i);
	op->jmpnext=op2;
	return(op2);
}

MSP430_Opcode *MSP430_OpBrMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;
	i=MSP430_GetOpMemSrcValueWord(ctx, op);	
	ctx->reg[MSP430_REG_PC]=i;
	if(op->jmpnext && (op->jmpnext->pc==i))
		return(op->jmpnext);
	op2=MSP430_GetDecodeOpcode(ctx, i);
	op->jmpnext=op2;
	return(op2);
}

MSP430_Opcode *MSP430_OpRet(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;

	a=ctx->reg[MSP430_REG_SP];
	i=MSP430_VaReadWordSP(ctx, a);
	ctx->reg[MSP430_REG_SP]=a+2;
	ctx->reg[MSP430_REG_PC]=i;

	if(op->jmpnext && (op->jmpnext->pc==i))
		return(op->jmpnext);
	op2=MSP430_GetDecodeOpcode(ctx, i);
	op->jmpnext=op2;
	return(op2);
}

MSP430_Opcode *MSP430_OpReti(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;

	a=ctx->reg[MSP430_REG_SP];
	i=MSP430_VaReadWordSP(ctx, a);
	j=MSP430_VaReadWordSP(ctx, a+2);
	ctx->reg[MSP430_REG_SP]=a+4;

	ctx->reg[MSP430_REG_SR]=i;
	ctx->reg[MSP430_REG_PC]=j;

	if(op->jmpnext && (op->jmpnext->pc==j))
		return(op->jmpnext);
	op2=MSP430_GetDecodeOpcode(ctx, j);
	op->jmpnext=op2;
	return(op2);
}

MSP430_Opcode *MSP430_OpJmp(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	
	if(op->jmpnext)
		return(op->jmpnext);
	
	ctx->reg[MSP430_REG_PC]=op->doffs;
	op2=MSP430_GetDecodeOpcode(ctx, op->doffs);
	op->jmpnext=op2;
	return(op2);
}

MSP430_Opcode *MSP430_OpJne(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(!(i&MSP430_SRM_Z))
	{
		if(op->jmpnext)
			return(op->jmpnext);
	
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeOpcode(ctx, op->doffs);
		op->jmpnext=op2;
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJeq(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(i&MSP430_SRM_Z)
	{
		if(op->jmpnext)
			return(op->jmpnext);
	
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeOpcode(ctx, op->doffs);
		op->jmpnext=op2;
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJnc(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(!(i&MSP430_SRM_C))
	{
		if(op->jmpnext)
			return(op->jmpnext);
	
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeOpcode(ctx, op->doffs);
		op->jmpnext=op2;
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJc(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(i&MSP430_SRM_C)
	{
		if(op->jmpnext)
			return(op->jmpnext);
	
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeOpcode(ctx, op->doffs);
		op->jmpnext=op2;
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJn(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(i&MSP430_SRM_N)
	{
		if(op->jmpnext)
			return(op->jmpnext);
	
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeOpcode(ctx, op->doffs);
		op->jmpnext=op2;
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJge(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(!(((i>>MSP430_SRB_N)^(i>>MSP430_SRB_V))&1))
	{
		if(op->jmpnext)
			return(op->jmpnext);
	
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeOpcode(ctx, op->doffs);
		op->jmpnext=op2;
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJl(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(((i>>MSP430_SRB_N)^(i>>MSP430_SRB_V))&1)
	{
		if(op->jmpnext)
			return(op->jmpnext);
	
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeOpcode(ctx, op->doffs);
		op->jmpnext=op2;
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpRepI(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op1, *op2;
	int i;
	
	op1=op->seqnext;
	op2=op1->seqnext;
	
	i=op->doffs;
	while((i--)>=0)
		{ op1->Run(ctx, op1); }
	return(op2);
}

MSP430_Opcode *MSP430_OpRepR(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op1, *op2;
	int i;
	
	op1=op->seqnext;
	op2=op1->seqnext;
	
	i=ctx->reg[op->dreg];
	while((i--)>=0)
		{ op1->Run(ctx, op1); }
	return(op2);
}



MSP430_Opcode *MSP430_OpJmpDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	
	ctx->reg[MSP430_REG_PC]=op->doffs;
	op2=MSP430_GetDecodeRomOpcode(ctx, op->doffs);
	return(op2);
}

MSP430_Opcode *MSP430_OpJneDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(!(i&MSP430_SRM_Z))
	{
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeRomOpcode(ctx, op->doffs);
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJeqDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(i&MSP430_SRM_Z)
	{
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeRomOpcode(ctx, op->doffs);
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJncDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(!(i&MSP430_SRM_C))
	{
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeRomOpcode(ctx, op->doffs);
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJcDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(i&MSP430_SRM_C)
	{
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeRomOpcode(ctx, op->doffs);
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJnDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(i&MSP430_SRM_N)
	{
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeRomOpcode(ctx, op->doffs);
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJgeDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(!(((i>>MSP430_SRB_N)^(i>>MSP430_SRB_V))&1))
	{
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeRomOpcode(ctx, op->doffs);
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpJlDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i;
	
	i=ctx->reg[MSP430_REG_SR];
	
	if(((i>>MSP430_SRB_N)^(i>>MSP430_SRB_V))&1)
	{
		ctx->reg[MSP430_REG_PC]=op->doffs;
		op2=MSP430_GetDecodeRomOpcode(ctx, op->doffs);
		return(op2);
	}
	return(op->seqnext);
}

MSP430_Opcode *MSP430_OpCallDiMW(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;

	i=MSP430_GetOpMemDestValueWord(ctx, op);

	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, op->pc2);
	
	if(!op->pc2)
	{
		*(int *)-1=-1;
	}
	
	ctx->reg[MSP430_REG_PC]=i;
	op2=MSP430_GetDecodeRomOpcode(ctx, i);
	return(op2);
}

MSP430_Opcode *MSP430_OpRetiDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;

	a=ctx->reg[MSP430_REG_SP];
	i=MSP430_VaReadWordSP(ctx, a);
	j=MSP430_VaReadWordSP(ctx, a+2);
	ctx->reg[MSP430_REG_SP]=a+4;

	ctx->reg[MSP430_REG_SR]=i;
	ctx->reg[MSP430_REG_PC]=j;

	op2=MSP430_GetDecodeRomOpcode(ctx, j);
	return(op2);
}

MSP430_Opcode *MSP430_OpRetDi(MSP430_Context *ctx, MSP430_Opcode *op)
{
	MSP430_Opcode *op2;
	int i, j, a;

	a=ctx->reg[MSP430_REG_SP];
	i=MSP430_VaReadWordSP(ctx, a);
	ctx->reg[MSP430_REG_SP]=a+2;
	ctx->reg[MSP430_REG_PC]=i;

	if(!i)
	{
//		*(int *)-1=-1;
	}

	op2=MSP430_GetDecodeRomOpcode(ctx, i);
	return(op2);
}
