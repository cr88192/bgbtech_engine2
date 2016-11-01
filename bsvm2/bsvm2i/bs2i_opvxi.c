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


BS2VM_API void BSVM2_Op_BINOPX_ADD_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_AddX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_SUB_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_SubX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_MUL_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_MulX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_AND_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_AndX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_OR_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_OrX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_XOR_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_XorX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_SHL_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_ShlX(
		*(BGBDTC_X128 *)a, b->ia);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_SAR_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_SarX(
		*(BGBDTC_X128 *)a, b->ia);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_SHR_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_ShrX(
		*(BGBDTC_X128 *)a, b->ia);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_DIV_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_DivX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_MOD_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_ModX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_NEG_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_NegX(
		*(BGBDTC_X128 *)a);
}

BS2VM_API void BSVM2_Op_BINOPX_NOT_XI128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	*(BGBDTC_X128 *)a=BGBDT_XI128_NotX(
		*(BGBDTC_X128 *)a);
}


BS2VM_API void BSVM2_Op_BINOPX_ADD_XF128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XF128_AddX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_SUB_XF128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XF128_SubX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_MUL_XF128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XF128_MulX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_DIV_XF128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	*(BGBDTC_X128 *)a=BGBDT_XF128_DivX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_NEG_XF128(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	*(BGBDTC_X128 *)a=BGBDT_XF128_NegX(
		*(BGBDTC_X128 *)a);
}

BS2VM_API void BSVM2_Op_CMPXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	int i;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	i=BGBDT_XI128_CmpSgX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, a);
	BSVM2_FrameFreeX128(frm, b);
	frm->stack[op->t0].i=i;
}

BS2VM_API void BSVM2_Op_UCMPXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	int i;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	i=BGBDT_XI128_CmpX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b);
	BSVM2_FrameFreeX128(frm, a);
	BSVM2_FrameFreeX128(frm, b);
	frm->stack[op->t0].i=i;
}

BS2VM_API void BSVM2_Op_CMPXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	int i;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	i=BGBDT_XF128_CmpX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b, 0);
	BSVM2_FrameFreeX128(frm, a);
	BSVM2_FrameFreeX128(frm, b);
	frm->stack[op->t0].i=i;
}

BS2VM_API void BSVM2_Op_CMP2XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	int i;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	i=BGBDT_XF128_CmpX(
		*(BGBDTC_X128 *)a, *(BGBDTC_X128 *)b, 1);
	BSVM2_FrameFreeX128(frm, a);
	BSVM2_FrameFreeX128(frm, b);
	frm->stack[op->t0].i=i;
}


BS2VM_API void BSVM2_Op_CVTI2XL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	int i;
	a=BSVM2_FrameAllocX128(frm);
	i=frm->stack[op->t0].i;
	a->ia=i;		a->ib=i>>31;
	a->ic=i>>31;	a->id=i>>31;
	frm->stack[op->t0].p=a;
}

BS2VM_API void BSVM2_Op_CVTL2XL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	s64 li;
	a=BSVM2_FrameAllocX128(frm);
	li=frm->stack[op->t0].l;
	a->la=li; a->lb=li>>63;
	frm->stack[op->t0].p=a;
}

BS2VM_API void BSVM2_Op_CVTUI2XL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	int i;
	a=BSVM2_FrameAllocX128(frm);
	i=frm->stack[op->t0].i;
	a->ia=i; a->ib=0;
	a->ic=0; a->id=0;
	frm->stack[op->t0].p=a;
}

BS2VM_API void BSVM2_Op_CVTUL2XL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	s64 li;
	a=BSVM2_FrameAllocX128(frm);
	li=frm->stack[op->t0].l;
	a->la=li;	a->lb=0;
	frm->stack[op->t0].p=a;
}

BS2VM_API void BSVM2_Op_CVTXL2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].i=a->ia;
	BSVM2_FrameFreeX128(frm, a);
}

BS2VM_API void BSVM2_Op_CVTXL2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].l=a->la;
	BSVM2_FrameFreeX128(frm, a);
}


BS2VM_API void BSVM2_Op_CVTD2XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	f64 f;
	a=BSVM2_FrameAllocX128(frm);
	f=frm->stack[op->t0].d;
	*(BGBDTC_X128 *)a=BGBDT_XF128_FromDouble(f);
	frm->stack[op->t0].p=a;
}

BS2VM_API void BSVM2_Op_CVTXF2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].d=BGBDT_XF128_ToDouble(*(BGBDTC_X128 *)a);
	BSVM2_FrameFreeX128(frm, a);
}

BS2VM_API void BSVM2_Op_CVTXL2XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	*(BGBDTC_X128 *)a=BGBDT_XF128_FromInt128(*(BGBDTC_X128 *)a);
}

BS2VM_API void BSVM2_Op_CVTXF2XL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	*(BGBDTC_X128 *)a=BGBDT_XF128_ToInt128(*(BGBDTC_X128 *)a);
}

BS2VM_API void BSVM2_Op_CVTXL2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].a=BGBDT_XI128_WrapInt128(*(BGBDTC_X128 *)a);
	BSVM2_FrameFreeX128(frm, a);
}

BS2VM_API void BSVM2_Op_CVTXF2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].a=BGBDT_XF128_WrapFloat128(*(BGBDTC_X128 *)a);
	BSVM2_FrameFreeX128(frm, a);
}

BS2VM_API void BSVM2_Op_CVTAA2XL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=BSVM2_FrameAllocX128(frm);
	*(BGBDTC_X128 *)a=BGBDT_XI128_UnwrapInt128(frm->stack[op->t0].a);
	frm->stack[op->t0].p=a;
}

BS2VM_API void BSVM2_Op_CVTAA2XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=BSVM2_FrameAllocX128(frm);
	*(BGBDTC_X128 *)a=BGBDT_XF128_UnwrapFloat128(frm->stack[op->t0].a);
	frm->stack[op->t0].p=a;
}
