#include <bteifgl.h>

void BSVM2_Op_LDIXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexInt(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LDIXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].l=dtvArrayGetIndexLong(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LDIXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].f=dtvArrayGetIndexFloat(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LDIXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].d=dtvArrayGetIndexDouble(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LDIXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayGetIndexDtVal(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LDIXSB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexSByte(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LDIXUB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexByte(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LDIXSS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexShort(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LDIXUS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexUShort(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}


void BSVM2_Op_STIXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexInt(
		frm->stack[op->t0].a, frm->stack[op->t1].i,
		frm->stack[op->t2].i);
}

void BSVM2_Op_STIXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexLong(
		frm->stack[op->t0].a, frm->stack[op->t1].i,
		frm->stack[op->t2].l);
}

void BSVM2_Op_STIXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexFloat(
		frm->stack[op->t0].a, frm->stack[op->t1].i,
		frm->stack[op->t2].f);
}

void BSVM2_Op_STIXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexDouble(
		frm->stack[op->t0].a, frm->stack[op->t1].i,
		frm->stack[op->t2].d);
}

void BSVM2_Op_STIXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexDtVal(
		frm->stack[op->t0].a, frm->stack[op->t1].i,
		frm->stack[op->t2].a);
}

void BSVM2_Op_STIXB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexByte(
		frm->stack[op->t0].a, frm->stack[op->t1].i,
		frm->stack[op->t2].i);
}

void BSVM2_Op_STIXS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexShort(
		frm->stack[op->t0].a, frm->stack[op->t1].i,
		frm->stack[op->t2].i);
}

void BSVM2_Op_RSTIXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexInt(
		frm->stack[op->t1].a, frm->stack[op->t2].i,
		frm->stack[op->t0].i);
}

void BSVM2_Op_RSTIXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexLong(
		frm->stack[op->t1].a, frm->stack[op->t2].i,
		frm->stack[op->t0].l);
}

void BSVM2_Op_RSTIXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexFloat(
		frm->stack[op->t1].a, frm->stack[op->t2].i,
		frm->stack[op->t0].f);
}

void BSVM2_Op_RSTIXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexDouble(
		frm->stack[op->t1].a, frm->stack[op->t2].i,
		frm->stack[op->t0].d);
}

void BSVM2_Op_RSTIXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexDtVal(
		frm->stack[op->t1].a, frm->stack[op->t2].i,
		frm->stack[op->t0].a);
}

void BSVM2_Op_RSTIXB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexByte(
		frm->stack[op->t1].a, frm->stack[op->t2].i,
		frm->stack[op->t0].i);
}

void BSVM2_Op_RSTIXS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexShort(
		frm->stack[op->t1].a, frm->stack[op->t2].i,
		frm->stack[op->t0].i);
}


void BSVM2_Op_LDIXIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexInt(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LDIXLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].l=dtvArrayGetIndexLong(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LDIXFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].f=dtvArrayGetIndexFloat(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LDIXDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].d=dtvArrayGetIndexDouble(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LDIXAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayGetIndexDtVal(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LDIXSBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexSByte(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LDIXUBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexByte(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LDIXSSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexShort(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LDIXUSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetIndexUShort(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_STIXIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexInt(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].i);
}

void BSVM2_Op_STIXLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexLong(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].l);
}

void BSVM2_Op_STIXFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexFloat(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].f);
}

void BSVM2_Op_STIXDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexDouble(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].d);
}

void BSVM2_Op_STIXAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexDtVal(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].a);
}

void BSVM2_Op_STIXBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexByte(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].i);
}

void BSVM2_Op_STIXSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexShort(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].i);
}

void BSVM2_Op_RSTIXIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexInt(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].i);
}

void BSVM2_Op_RSTIXLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexLong(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].l);
}

void BSVM2_Op_RSTIXFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexFloat(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].f);
}

void BSVM2_Op_RSTIXDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexDouble(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].d);
}

void BSVM2_Op_RSTIXAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexDtVal(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].a);
}

void BSVM2_Op_RSTIXBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexByte(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].i);
}

void BSVM2_Op_RSTIXSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtvArraySetIndexShort(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].i);
}


void BSVM2_Op_LEARI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB4(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LEARL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LEARF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB4(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LEARD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LEARA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LEARB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB1(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LEARS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB2(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LEARIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB4(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LEARLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LEARFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB4(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LEARDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LEARAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LEARBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB1(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LEARSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB2(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_LEAST(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=BGBDT_TagStr_StringAdjustOffset(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

void BSVM2_Op_LEASTC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=BGBDT_TagStr_StringAdjustOffset(
		frm->stack[op->t0].a, op->v.i);
}

void BSVM2_Op_NEWARR_IC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_INT); }
void BSVM2_Op_NEWARR_UIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_UINT); }

void BSVM2_Op_NEWARR_SBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_SBYTE); }
void BSVM2_Op_NEWARR_UBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_UBYTE); }
void BSVM2_Op_NEWARR_SSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_SHORT); }
void BSVM2_Op_NEWARR_USC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_USHORT); }

void BSVM2_Op_NEWARR_LC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_LONG); }
void BSVM2_Op_NEWARR_ULC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_ULONG); }

void BSVM2_Op_NEWARR_FC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_FLOAT); }
void BSVM2_Op_NEWARR_DC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_DOUBLE); }
void BSVM2_Op_NEWARR_AC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_ADDRESS); }

void BSVM2_Op_NEWARR_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_INT);		}
void BSVM2_Op_NEWARR_UI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_UINT);		}
void BSVM2_Op_NEWARR_SB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_SBYTE);		}
void BSVM2_Op_NEWARR_UB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_UBYTE);		}
void BSVM2_Op_NEWARR_SS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_SHORT);		}
void BSVM2_Op_NEWARR_US(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_USHORT);		}
void BSVM2_Op_NEWARR_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_LONG);		}
void BSVM2_Op_NEWARR_UL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_ULONG);		}
void BSVM2_Op_NEWARR_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_FLOAT);		}
void BSVM2_Op_NEWARR_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_DOUBLE);		}
void BSVM2_Op_NEWARR_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_ADDRESS);	}


void BSVM2_Op_LDIXILL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexInt(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
void BSVM2_Op_LDIXLLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].l=dtvArrayGetIndexLong(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
void BSVM2_Op_LDIXFLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].f=dtvArrayGetIndexFloat(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
void BSVM2_Op_LDIXDLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].d=dtvArrayGetIndexDouble(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
void BSVM2_Op_LDIXALL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvArrayGetIndexDtVal(
		frm->local[op->i0].a, frm->local[op->i1].i);	}

void BSVM2_Op_LDIXSBLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexSByte(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
void BSVM2_Op_LDIXUBLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexByte(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
void BSVM2_Op_LDIXSSLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexShort(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
void BSVM2_Op_LDIXUSLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexUShort(
		frm->local[op->i0].a, frm->local[op->i1].i);	}

void BSVM2_Op_LDIXILC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexInt(
		frm->local[op->i0].a, op->v.i);	}
void BSVM2_Op_LDIXLLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].l=dtvArrayGetIndexLong(
		frm->local[op->i0].a, op->v.i);	}
void BSVM2_Op_LDIXFLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].f=dtvArrayGetIndexFloat(
		frm->local[op->i0].a, op->v.i);	}
void BSVM2_Op_LDIXDLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].d=dtvArrayGetIndexDouble(
		frm->local[op->i0].a, op->v.i);	}
void BSVM2_Op_LDIXALC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvArrayGetIndexDtVal(
		frm->local[op->i0].a, op->v.i);	}

void BSVM2_Op_LDIXSBLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexSByte(
		frm->local[op->i0].a, op->v.i);	}
void BSVM2_Op_LDIXUBLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexByte(
		frm->local[op->i0].a, op->v.i);	}
void BSVM2_Op_LDIXSSLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexShort(
		frm->local[op->i0].a, op->v.i);	}
void BSVM2_Op_LDIXUSLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexUShort(
		frm->local[op->i0].a, op->v.i);	}


void BSVM2_Op_STIXILL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexInt(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].i);		}
void BSVM2_Op_STIXLLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexLong(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].l);		}
void BSVM2_Op_STIXFLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexFloat(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].f);		}
void BSVM2_Op_STIXDLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDouble(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].d);		}
void BSVM2_Op_STIXALL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDtVal(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].a);		}
void BSVM2_Op_STIXBLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexByte(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].i);		}
void BSVM2_Op_STIXSLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexShort(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].i);		}

void BSVM2_Op_STIXILC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexInt(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].i);		}
void BSVM2_Op_STIXLLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexLong(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].l);		}
void BSVM2_Op_STIXFLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexFloat(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].f);		}
void BSVM2_Op_STIXDLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDouble(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].d);		}
void BSVM2_Op_STIXALC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDtVal(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].a);		}
void BSVM2_Op_STIXBLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexByte(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].i);		}
void BSVM2_Op_STIXSLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexShort(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].i);		}
