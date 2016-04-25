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
