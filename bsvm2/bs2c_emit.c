#include <bteifgl.h>

void BS2C_EmitByte(BS2CC_CompileContext *ctx, int op)
{
	*ctx->frm->ct++=op;
}

void BS2C_EmitOpcode(BS2CC_CompileContext *ctx, int op)
{
	if(op<0xE0)
	{
		BS2C_EmitByte(ctx, op);
		return;
	}

	if(op<0x1000)
	{
		BS2C_EmitByte(ctx, 0xE0|(op>>8));
		BS2C_EmitByte(ctx, op);
		return;
	}

	if(op<0x80000)
	{
		BS2C_EmitByte(ctx, 0xF0|(op>>16));
		BS2C_EmitByte(ctx, (op>> 8));
		BS2C_EmitByte(ctx, (op    ));
		return;
	}

	if(op<0x4000000)
	{
		BS2C_EmitByte(ctx, 0xF8|(op>>24));
		BS2C_EmitByte(ctx, (op>>16));
		BS2C_EmitByte(ctx, (op>> 8));
		BS2C_EmitByte(ctx, (op    ));
		return;
	}

	BS2C_EmitByte(ctx, 0xFC);
	BS2C_EmitByte(ctx, (op>>24));
	BS2C_EmitByte(ctx, (op>>16));
	BS2C_EmitByte(ctx, (op>> 8));
	BS2C_EmitByte(ctx, (op    ));
}

void BS2C_EmitOpcodeIdx(BS2CC_CompileContext *ctx, int ix)
{
	if(op<0x80)
	{
		BS2C_EmitByte(ctx, op);
		return;
	}

	if(op<0x4000)
	{
		BS2C_EmitByte(ctx, 0x80|(op>>8));
		BS2C_EmitByte(ctx, op);
		return;
	}

	if(op<0x200000)
	{
		BS2C_EmitByte(ctx, 0xC0|(op>>16));
		BS2C_EmitByte(ctx, op>>8);
		BS2C_EmitByte(ctx, op);
		return;
	}

	if(op<0x10000000)
	{
		BS2C_EmitByte(ctx, 0xE0|(op>>24));
		BS2C_EmitByte(ctx, op>>16);
		BS2C_EmitByte(ctx, op>>8);
		BS2C_EmitByte(ctx, op);
		return;
	}

	BS2C_EmitByte(ctx, 0xF0);
	BS2C_EmitByte(ctx, op>>24);
	BS2C_EmitByte(ctx, op>>16);
	BS2C_EmitByte(ctx, op>>8);
	BS2C_EmitByte(ctx, op);
}

void BS2C_EmitOpcodeUZx(BS2CC_CompileContext *ctx, int z, u64 ix)
{
	if(ix<8)
	{
		BS2C_EmitByte(ctx, (z<<4)|ix);
		return;
	}
	if(ix<1024)
	{
		BS2C_EmitByte(ctx, (z<<4)|8|(ix>>8));
		BS2C_EmitByte(ctx, ix);
		return;
	}
	if(ix<131072)
	{
		BS2C_EmitByte(ctx, (z<<4)|12|(ix>>16));
		BS2C_EmitByte(ctx, ix>>8);
		BS2C_EmitByte(ctx, ix);
		return;
	}

	if(ix<(1ULL<<32))
	{
		BS2C_EmitByte(ctx, (z<<4)|14);
		BS2C_EmitByte(ctx, ix>>24);
		BS2C_EmitByte(ctx, ix>>16);
		BS2C_EmitByte(ctx, ix>> 8);
		BS2C_EmitByte(ctx, ix    );
		return;
	}

	BS2C_EmitByte(ctx, (z<<4)|15);
	BS2C_EmitByte(ctx, ix>>56);
	BS2C_EmitByte(ctx, ix>>48);
	BS2C_EmitByte(ctx, ix>>40);
	BS2C_EmitByte(ctx, ix>>32);
	BS2C_EmitByte(ctx, ix>>24);
	BS2C_EmitByte(ctx, ix>>16);
	BS2C_EmitByte(ctx, ix>> 8);
	BS2C_EmitByte(ctx, ix    );
	return;
}

void BS2C_EmitOpcodeSZx(BS2CC_CompileContext *ctx, int z, s64 ix)
{
	BS2C_EmitOpcodeUZx(ctx, z, (ix<<1)^(ix>>63));
}

void BS2C_EmitOpcodeZxF(BS2CC_CompileContext *ctx, double f)
{
	float fc;
	u32 ix;

	fc=f; ix=*(u32 *)(&fc);
	BS2C_EmitByte(ctx, (BSVM2_OPZ_FLOAT<<4)|14);
	BS2C_EmitByte(ctx, ix>>24);
	BS2C_EmitByte(ctx, ix>>16);
	BS2C_EmitByte(ctx, ix>> 8);
	BS2C_EmitByte(ctx, ix    );
}

void BS2C_EmitOpcodeZxD(BS2CC_CompileContext *ctx, double f)
{
	u64 ix;

	ix=*(u64 *)(&f);
	BS2C_EmitByte(ctx, (BSVM2_OPZ_DOUBLE<<4)|15);
	BS2C_EmitByte(ctx, ix>>56);
	BS2C_EmitByte(ctx, ix>>48);
	BS2C_EmitByte(ctx, ix>>40);
	BS2C_EmitByte(ctx, ix>>32);
	BS2C_EmitByte(ctx, ix>>24);
	BS2C_EmitByte(ctx, ix>>16);
	BS2C_EmitByte(ctx, ix>> 8);
	BS2C_EmitByte(ctx, ix    );
}
