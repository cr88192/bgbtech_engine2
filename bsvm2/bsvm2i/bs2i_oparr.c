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


BS2VM_API void BSVM2_Op_LDIXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB4F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(s32 *)p;
}

BS2VM_API void BSVM2_Op_LDIXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].l=*(s64 *)p;
}

BS2VM_API void BSVM2_Op_LDIXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB4F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].f=*(f32 *)p;
}

BS2VM_API void BSVM2_Op_LDIXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].d=*(f64 *)p;
}

BS2VM_API void BSVM2_Op_LDIXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].a=*(dtVal *)p;
}

BS2VM_API void BSVM2_Op_LDIXSB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB1F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(sbyte *)p;
}

BS2VM_API void BSVM2_Op_LDIXUB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB1F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(byte *)p;
}

BS2VM_API void BSVM2_Op_LDIXSS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB2F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(s16 *)p;
}

BS2VM_API void BSVM2_Op_LDIXUS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB2F(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	frm->stack[op->t0].i=*(u16 *)p;
}


BS2VM_API void BSVM2_Op_STIXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB4(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(s32 *)p=frm->stack[op->t2].i;

//	dtvArraySetIndexInt(
//		frm->stack[op->t0].a, frm->stack[op->t1].i,
//		frm->stack[op->t2].i);
}

BS2VM_API void BSVM2_Op_STIXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(s64 *)p=frm->stack[op->t2].l;

//	dtvArraySetIndexLong(
//		frm->stack[op->t0].a, frm->stack[op->t1].i,
//		frm->stack[op->t2].l);
}

BS2VM_API void BSVM2_Op_STIXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB4(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(f32 *)p=frm->stack[op->t2].f;

//	dtvArraySetIndexFloat(
//		frm->stack[op->t0].a, frm->stack[op->t1].i,
//		frm->stack[op->t2].f);
}

BS2VM_API void BSVM2_Op_STIXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(f64 *)p=frm->stack[op->t2].d;

//	dtvArraySetIndexDouble(
//		frm->stack[op->t0].a, frm->stack[op->t1].i,
//		frm->stack[op->t2].d);
}

BS2VM_API void BSVM2_Op_STIXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(dtVal *)p=frm->stack[op->t2].a;

//	dtvArraySetIndexDtVal(
//		frm->stack[op->t0].a, frm->stack[op->t1].i,
//		frm->stack[op->t2].a);
}

BS2VM_API void BSVM2_Op_STIXB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB1(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(byte *)p=frm->stack[op->t2].i;

//	dtvArraySetIndexByte(
//		frm->stack[op->t0].a, frm->stack[op->t1].i,
//		frm->stack[op->t2].i);
}

BS2VM_API void BSVM2_Op_STIXS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB2(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(s16 *)p=frm->stack[op->t2].i;

//	dtvArraySetIndexShort(
//		frm->stack[op->t0].a, frm->stack[op->t1].i,
//		frm->stack[op->t2].i);
}


BS2VM_API void BSVM2_Op_RSTIXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB4F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(s32 *)p=frm->stack[op->t0].i;
}

BS2VM_API void BSVM2_Op_RSTIXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(s64 *)p=frm->stack[op->t0].l;
}

BS2VM_API void BSVM2_Op_RSTIXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB4F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(f32 *)p=frm->stack[op->t0].f;
}

BS2VM_API void BSVM2_Op_RSTIXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(f64 *)p=frm->stack[op->t0].d;
}

BS2VM_API void BSVM2_Op_RSTIXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(dtVal *)p=frm->stack[op->t0].a;
}

BS2VM_API void BSVM2_Op_RSTIXB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB1F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(byte *)p=frm->stack[op->t0].i;
}

BS2VM_API void BSVM2_Op_RSTIXS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB2F(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	*(s16 *)p=frm->stack[op->t0].i;
}

#if 1
BS2VM_API void BSVM2_Op_LDIXI_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB4(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].i=*(s32 *)p;
}

BS2VM_API void BSVM2_Op_LDIXL_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].l=*(s64 *)p;
}

BS2VM_API void BSVM2_Op_LDIXF_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB4(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].f=*(f32 *)p;
}

BS2VM_API void BSVM2_Op_LDIXD_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].d=*(f64 *)p;
}

BS2VM_API void BSVM2_Op_LDIXA_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].a=*(dtVal *)p;
}

BS2VM_API void BSVM2_Op_LDIXSB_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB1(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].i=*(sbyte *)p;
}

BS2VM_API void BSVM2_Op_LDIXUB_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB1(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].i=*(byte *)p;
}

BS2VM_API void BSVM2_Op_LDIXSS_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB2(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].i=*(s16 *)p;
}

BS2VM_API void BSVM2_Op_LDIXUS_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB2(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	frm->stack[op->t0].i=*(u16 *)p;
}

BS2VM_API void BSVM2_Op_RSTIXI_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB4(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(s32 *)p=frm->stack[op->t0].i;
}

BS2VM_API void BSVM2_Op_RSTIXL_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(s64 *)p=frm->stack[op->t0].l;
}

BS2VM_API void BSVM2_Op_RSTIXF_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB4(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(f32 *)p=frm->stack[op->t0].f;
}

BS2VM_API void BSVM2_Op_RSTIXD_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(f64 *)p=frm->stack[op->t0].d;
}

BS2VM_API void BSVM2_Op_RSTIXA_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB8(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(dtVal *)p=frm->stack[op->t0].a;
}

BS2VM_API void BSVM2_Op_RSTIXB_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB1(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(byte *)p=frm->stack[op->t0].i;
}

BS2VM_API void BSVM2_Op_RSTIXS_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(!dtvIsArrayFP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	p=dtvArrayGetIndexAddrB2(
		frm->stack[op->t1].a, frm->stack[op->t2].i);
	if(!p)
		{ frm->ctx->status=BSVM2_EXS_BOUNDEX; return; }
	*(s16 *)p=frm->stack[op->t0].i;
}
#endif

BS2VM_API void BSVM2_Op_LDIXIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].i=dtvArrayGetIndexInt(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB4F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(s32 *)p;
}

BS2VM_API void BSVM2_Op_LDIXLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].l=dtvArrayGetIndexLong(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].l=*(s64 *)p;
}

BS2VM_API void BSVM2_Op_LDIXFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].f=dtvArrayGetIndexFloat(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB4F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].f=*(f32 *)p;
}

BS2VM_API void BSVM2_Op_LDIXDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].d=dtvArrayGetIndexDouble(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].d=*(f64 *)p;
}

BS2VM_API void BSVM2_Op_LDIXAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].a=dtvArrayGetIndexDtVal(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].a=*(dtVal *)p;
}

BS2VM_API void BSVM2_Op_LDIXSBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].i=dtvArrayGetIndexSByte(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB1F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(sbyte *)p;
}

BS2VM_API void BSVM2_Op_LDIXUBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].i=dtvArrayGetIndexByte(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB1F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(byte *)p;
}

BS2VM_API void BSVM2_Op_LDIXSSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].i=dtvArrayGetIndexShort(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB2F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(s16 *)p;
}

BS2VM_API void BSVM2_Op_LDIXUSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t0].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	frm->stack[op->t0].i=dtvArrayGetIndexUShort(
//		frm->stack[op->t0].a, op->v.i);
	p=dtvArrayGetIndexAddrB2F(frm->stack[op->t0].a, op->v.i);
	frm->stack[op->t0].i=*(u16 *)p;
}

BS2VM_API void BSVM2_Op_STIXIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexInt(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_STIXLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexLong(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].l);
}

BS2VM_API void BSVM2_Op_STIXFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexFloat(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].f);
}

BS2VM_API void BSVM2_Op_STIXDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexDouble(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].d);
}

BS2VM_API void BSVM2_Op_STIXAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexDtVal(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_STIXBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexByte(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_STIXSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexShort(
		frm->stack[op->t0].a, op->v.i,
		frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_RSTIXIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t1].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	dtvArraySetIndexInt(
//		frm->stack[op->t1].a, op->v.i,
//		frm->stack[op->t0].i);
	p=dtvArrayGetIndexAddrB4F(frm->stack[op->t1].a, op->v.i);
	*(s32 *)p=frm->stack[op->t0].i;
}

BS2VM_API void BSVM2_Op_RSTIXLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t1].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	dtvArraySetIndexLong(
//		frm->stack[op->t1].a, op->v.i,
//		frm->stack[op->t0].l);
	p=dtvArrayGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	*(s64 *)p=frm->stack[op->t0].l;
}

BS2VM_API void BSVM2_Op_RSTIXFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t1].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	dtvArraySetIndexFloat(
//		frm->stack[op->t1].a, op->v.i,
//		frm->stack[op->t0].f);
	p=dtvArrayGetIndexAddrB4F(frm->stack[op->t1].a, op->v.i);
	*(f32 *)p=frm->stack[op->t0].f;
}

BS2VM_API void BSVM2_Op_RSTIXDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t1].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	dtvArraySetIndexDouble(
//		frm->stack[op->t1].a, op->v.i,
//		frm->stack[op->t0].d);
	p=dtvArrayGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	*(f64 *)p=frm->stack[op->t0].d;
}

BS2VM_API void BSVM2_Op_RSTIXAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t1].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	dtvArraySetIndexDtVal(
//		frm->stack[op->t1].a, op->v.i,
//		frm->stack[op->t0].a);
	p=dtvArrayGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	*(dtVal *)p=frm->stack[op->t0].a;
}

BS2VM_API void BSVM2_Op_RSTIXBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t1].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	dtvArraySetIndexByte(
//		frm->stack[op->t1].a, op->v.i,
//		frm->stack[op->t0].i);
	p=dtvArrayGetIndexAddrB1F(frm->stack[op->t1].a, op->v.i);
	*(byte *)p=frm->stack[op->t0].i;
}

BS2VM_API void BSVM2_Op_RSTIXSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	if(dtvNullP(frm->stack[op->t1].a))
//		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
//	dtvArraySetIndexShort(
//		frm->stack[op->t1].a, op->v.i,
//		frm->stack[op->t0].i);
	p=dtvArrayGetIndexAddrB2F(frm->stack[op->t1].a, op->v.i);
	*(u16 *)p=frm->stack[op->t0].i;
}

#if 1
BS2VM_API void BSVM2_Op_LDIXIC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].i=dtvArrayGetIndexInt(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LDIXLC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].l=dtvArrayGetIndexLong(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LDIXFC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].f=dtvArrayGetIndexFloat(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LDIXDC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].d=dtvArrayGetIndexDouble(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LDIXAC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayGetIndexDtVal(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LDIXSBC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].i=dtvArrayGetIndexSByte(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LDIXUBC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].i=dtvArrayGetIndexByte(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LDIXSSC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].i=dtvArrayGetIndexShort(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LDIXUSC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].i=dtvArrayGetIndexUShort(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_RSTIXIC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexInt(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].i);
}

BS2VM_API void BSVM2_Op_RSTIXLC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexLong(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].l);
}

BS2VM_API void BSVM2_Op_RSTIXFC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexFloat(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].f);
}

BS2VM_API void BSVM2_Op_RSTIXDC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexDouble(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].d);
}

BS2VM_API void BSVM2_Op_RSTIXAC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexDtVal(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].a);
}

BS2VM_API void BSVM2_Op_RSTIXBC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexByte(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].i);
}

BS2VM_API void BSVM2_Op_RSTIXSC_BC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t1].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	dtvArraySetIndexShort(
		frm->stack[op->t1].a, op->v.i,
		frm->stack[op->t0].i);
}
#endif

BS2VM_API void BSVM2_Op_LEARI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB4(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_LEARL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_LEARF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB4(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_LEARD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_LEARA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_LEARB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB1(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_LEARS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB2(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_LEARIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB4(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LEARLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LEARFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB4(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LEARDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LEARAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB8(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LEARBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB1(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LEARSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=dtvArrayAdjustOffsetB2(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_LEAST(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=BGBDT_TagStr_StringAdjustOffset(
		frm->stack[op->t0].a, frm->stack[op->t1].i);
}

BS2VM_API void BSVM2_Op_LEASTC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }
	frm->stack[op->t0].a=BGBDT_TagStr_StringAdjustOffset(
		frm->stack[op->t0].a, op->v.i);
}

BS2VM_API void BSVM2_Op_NEWARR_IC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_INT); }
BS2VM_API void BSVM2_Op_NEWARR_UIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_UINT); }

BS2VM_API void BSVM2_Op_NEWARR_SBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_SBYTE); }
BS2VM_API void BSVM2_Op_NEWARR_UBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_UBYTE); }
BS2VM_API void BSVM2_Op_NEWARR_SSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_SHORT); }
BS2VM_API void BSVM2_Op_NEWARR_USC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_USHORT); }

BS2VM_API void BSVM2_Op_NEWARR_LC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_LONG); }
BS2VM_API void BSVM2_Op_NEWARR_ULC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_ULONG); }

BS2VM_API void BSVM2_Op_NEWARR_FC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_FLOAT); }
BS2VM_API void BSVM2_Op_NEWARR_DC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_DOUBLE); }
BS2VM_API void BSVM2_Op_NEWARR_AC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvNewArray(op->i0, BGBDT_BASETY_ADDRESS); }

BS2VM_API void BSVM2_Op_NEWARR_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_INT);		}
BS2VM_API void BSVM2_Op_NEWARR_UI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_UINT);		}
BS2VM_API void BSVM2_Op_NEWARR_SB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_SBYTE);		}
BS2VM_API void BSVM2_Op_NEWARR_UB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_UBYTE);		}
BS2VM_API void BSVM2_Op_NEWARR_SS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_SHORT);		}
BS2VM_API void BSVM2_Op_NEWARR_US(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_USHORT);		}
BS2VM_API void BSVM2_Op_NEWARR_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_LONG);		}
BS2VM_API void BSVM2_Op_NEWARR_UL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_ULONG);		}
BS2VM_API void BSVM2_Op_NEWARR_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_FLOAT);		}
BS2VM_API void BSVM2_Op_NEWARR_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_DOUBLE);		}
BS2VM_API void BSVM2_Op_NEWARR_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_ADDRESS);	}

#if 1
BS2VM_API void BSVM2_Op_IFXARR_IC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_INT); }
BS2VM_API void BSVM2_Op_IFXARR_UIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_UINT); }

BS2VM_API void BSVM2_Op_IFXARR_SBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_SBYTE); }
BS2VM_API void BSVM2_Op_IFXARR_UBC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_UBYTE); }
BS2VM_API void BSVM2_Op_IFXARR_SSC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_SHORT); }
BS2VM_API void BSVM2_Op_IFXARR_USC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_USHORT); }

BS2VM_API void BSVM2_Op_IFXARR_LC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_LONG); }
BS2VM_API void BSVM2_Op_IFXARR_ULC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_ULONG); }

BS2VM_API void BSVM2_Op_IFXARR_FC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_FLOAT); }
BS2VM_API void BSVM2_Op_IFXARR_DC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_DOUBLE); }
BS2VM_API void BSVM2_Op_IFXARR_AC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i1].a=dtvNewArray(op->i0, BGBDT_BASETY_ADDRESS); }

BS2VM_API void BSVM2_Op_IFXARR_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_INT);		}
BS2VM_API void BSVM2_Op_IFXARR_UI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_UINT);		}
BS2VM_API void BSVM2_Op_IFXARR_SB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_SBYTE);		}
BS2VM_API void BSVM2_Op_IFXARR_UB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_UBYTE);		}
BS2VM_API void BSVM2_Op_IFXARR_SS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_SHORT);		}
BS2VM_API void BSVM2_Op_IFXARR_US(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_USHORT);		}
BS2VM_API void BSVM2_Op_IFXARR_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_LONG);		}
BS2VM_API void BSVM2_Op_IFXARR_UL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_ULONG);		}
BS2VM_API void BSVM2_Op_IFXARR_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_FLOAT);		}
BS2VM_API void BSVM2_Op_IFXARR_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_DOUBLE);		}
BS2VM_API void BSVM2_Op_IFXARR_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->local[op->i1].a=dtvNewArray(
		frm->stack[op->t0].i, BGBDT_BASETY_ADDRESS);	}

BS2VM_API void BSVM2_Op_DFXARR(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	dtVal a;
	void *p;

	a=frm->local[op->i1].a;
	BGBDT_TagArr_FreeArray(a);

//	p=dtvUnwrapPtr(a);
//	if(p)
//	{
//		dtmFree(p);
//	}
}
#endif

BS2VM_API void BSVM2_Op_LDIXILL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexInt(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
BS2VM_API void BSVM2_Op_LDIXLLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].l=dtvArrayGetIndexLong(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
BS2VM_API void BSVM2_Op_LDIXFLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].f=dtvArrayGetIndexFloat(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
BS2VM_API void BSVM2_Op_LDIXDLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].d=dtvArrayGetIndexDouble(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
BS2VM_API void BSVM2_Op_LDIXALL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvArrayGetIndexDtVal(
		frm->local[op->i0].a, frm->local[op->i1].i);	}

BS2VM_API void BSVM2_Op_LDIXSBLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexSByte(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
BS2VM_API void BSVM2_Op_LDIXUBLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexByte(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
BS2VM_API void BSVM2_Op_LDIXSSLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexShort(
		frm->local[op->i0].a, frm->local[op->i1].i);	}
BS2VM_API void BSVM2_Op_LDIXUSLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexUShort(
		frm->local[op->i0].a, frm->local[op->i1].i);	}

BS2VM_API void BSVM2_Op_LDIXILC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexInt(
		frm->local[op->i0].a, op->v.i);	}
BS2VM_API void BSVM2_Op_LDIXLLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].l=dtvArrayGetIndexLong(
		frm->local[op->i0].a, op->v.i);	}
BS2VM_API void BSVM2_Op_LDIXFLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].f=dtvArrayGetIndexFloat(
		frm->local[op->i0].a, op->v.i);	}
BS2VM_API void BSVM2_Op_LDIXDLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].d=dtvArrayGetIndexDouble(
		frm->local[op->i0].a, op->v.i);	}
BS2VM_API void BSVM2_Op_LDIXALC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].a=dtvArrayGetIndexDtVal(
		frm->local[op->i0].a, op->v.i);	}

BS2VM_API void BSVM2_Op_LDIXSBLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexSByte(
		frm->local[op->i0].a, op->v.i);	}
BS2VM_API void BSVM2_Op_LDIXUBLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexByte(
		frm->local[op->i0].a, op->v.i);	}
BS2VM_API void BSVM2_Op_LDIXSSLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexShort(
		frm->local[op->i0].a, op->v.i);	}
BS2VM_API void BSVM2_Op_LDIXUSLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].i=dtvArrayGetIndexUShort(
		frm->local[op->i0].a, op->v.i);	}


BS2VM_API void BSVM2_Op_STIXILL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexInt(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].i);		}
BS2VM_API void BSVM2_Op_STIXLLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexLong(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].l);		}
BS2VM_API void BSVM2_Op_STIXFLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexFloat(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].f);		}
BS2VM_API void BSVM2_Op_STIXDLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDouble(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].d);		}
BS2VM_API void BSVM2_Op_STIXALL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDtVal(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].a);		}
BS2VM_API void BSVM2_Op_STIXBLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexByte(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].i);		}
BS2VM_API void BSVM2_Op_STIXSLL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexShort(
		frm->local[op->i0].a, frm->local[op->i1].i,
		frm->stack[op->t0].i);		}

BS2VM_API void BSVM2_Op_STIXILC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexInt(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].i);		}
BS2VM_API void BSVM2_Op_STIXLLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexLong(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].l);		}
BS2VM_API void BSVM2_Op_STIXFLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexFloat(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].f);		}
BS2VM_API void BSVM2_Op_STIXDLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDouble(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].d);		}
BS2VM_API void BSVM2_Op_STIXALC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDtVal(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].a);		}
BS2VM_API void BSVM2_Op_STIXBLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexByte(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].i);		}
BS2VM_API void BSVM2_Op_STIXSLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexShort(
		frm->local[op->i0].a, op->v.i,
		frm->stack[op->t0].i);		}


#if 1
BS2VM_API void BSVM2_Op_LDDRAL_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB4F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(s32 *)p;
}

BS2VM_API void BSVM2_Op_LDDRAL_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].l=*(s64 *)p;
}

BS2VM_API void BSVM2_Op_LDDRAL_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB4F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].f=*(f32 *)p;
}

BS2VM_API void BSVM2_Op_LDDRAL_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].d=*(f64 *)p;
}

BS2VM_API void BSVM2_Op_LDDRAL_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].a=*(dtVal *)p;
}

BS2VM_API void BSVM2_Op_LDDRAL_SB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB1F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(sbyte *)p;
}

BS2VM_API void BSVM2_Op_LDDRAL_UB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB1F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(byte *)p;
}

BS2VM_API void BSVM2_Op_LDDRAL_SS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB2F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(s16 *)p;
}

BS2VM_API void BSVM2_Op_LDDRAL_US(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvArrayGetIndexAddrB2F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(u16 *)p;
}
#endif

#if 1
BS2VM_API void BSVM2_Op_STDRAL_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexInt(
		frm->local[op->i0].a, 0,
		frm->stack[op->t0].i);		}
BS2VM_API void BSVM2_Op_STDRAL_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexLong(
		frm->local[op->i0].a, 0,
		frm->stack[op->t0].l);		}
BS2VM_API void BSVM2_Op_STDRAL_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexFloat(
		frm->local[op->i0].a, 0,
		frm->stack[op->t0].f);		}
BS2VM_API void BSVM2_Op_STDRAL_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDouble(
		frm->local[op->i0].a, 0,
		frm->stack[op->t0].d);		}
BS2VM_API void BSVM2_Op_STDRAL_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexDtVal(
		frm->local[op->i0].a, 0,
		frm->stack[op->t0].a);		}
BS2VM_API void BSVM2_Op_STDRAL_B(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexByte(
		frm->local[op->i0].a, 0,
		frm->stack[op->t0].i);		}
BS2VM_API void BSVM2_Op_STDRAL_S(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtvArraySetIndexShort(
		frm->local[op->i0].a, 0,
		frm->stack[op->t0].i);		}
#endif


BS2VM_API void BSVM2_Op_AGETI_LEN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetSize(frm->stack[op->t0].a);
}

BS2VM_API void BSVM2_Op_AGETI_OFS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].i=dtvArrayGetOffset(frm->stack[op->t0].a);
}
