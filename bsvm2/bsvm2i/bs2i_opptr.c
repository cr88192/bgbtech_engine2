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


BS2VM_API void BSVM2_Op_CVTA2PTR(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvUnwrapPtr(frm->stack[op->t0].a);
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

BS2VM_API void BSVM2_Op_CVTPTR2A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	frm->stack[op->t0].a=dtvWrapPtr(p);
}

BS2VM_API void BSVM2_Op_LDRL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(&frm->local[op->i0], op->i1+1);
	frm->stack[op->t0].p=&frm->local[op->i0];
}

BS2VM_API void BSVM2_Op_LDRGS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;	vi=op->v.p;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(vi->gvalue, op->i1+1);
	frm->stack[op->t0].p=vi->gvalue;
}

BS2VM_API void BSVM2_Op_LDROS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	vi=op->v.p;
	p=dtcVaGetPtr(frm->stack[op->t0].a, vi->objinf);
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, op->i1+1);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_DIFFPTR_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p1, *p2;
//	p1=dtvUnwrapPtr(frm->stack[op->t0].a);
//	p2=dtvUnwrapPtr(frm->stack[op->t1].a);

	p1=frm->stack[op->t0].p;
	p2=frm->stack[op->t1].p;
	frm->stack[op->t0].l=((s32 *)p1)-((s32 *)p2);
}

void BSVM2_Op_DIFFPTR_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p1, *p2;
//	p1=dtvUnwrapPtr(frm->stack[op->t0].a);
//	p2=dtvUnwrapPtr(frm->stack[op->t1].a);
	p1=frm->stack[op->t0].p;
	p2=frm->stack[op->t1].p;
	frm->stack[op->t0].l=((s64 *)p1)-((s64 *)p2);
}

void BSVM2_Op_DIFFPTR_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p1, *p2;
//	p1=dtvUnwrapPtr(frm->stack[op->t0].a);
//	p2=dtvUnwrapPtr(frm->stack[op->t1].a);
	p1=frm->stack[op->t0].p;
	p2=frm->stack[op->t1].p;
	frm->stack[op->t0].l=((dtVal *)p1)-((dtVal *)p2);
}

void BSVM2_Op_DIFFPTR_B(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p1, *p2;
//	p1=dtvUnwrapPtr(frm->stack[op->t0].a);
//	p2=dtvUnwrapPtr(frm->stack[op->t1].a);
	p1=frm->stack[op->t0].p;
	p2=frm->stack[op->t1].p;
	frm->stack[op->t0].l=((byte *)p1)-((byte *)p2);
}

void BSVM2_Op_DIFFPTR_S(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p1, *p2;
//	p1=dtvUnwrapPtr(frm->stack[op->t0].a);
//	p2=dtvUnwrapPtr(frm->stack[op->t1].a);
	p1=frm->stack[op->t0].p;
	p2=frm->stack[op->t1].p;
	frm->stack[op->t0].l=((s16 *)p1)-((s16 *)p2);
}

void BSVM2_Op_DIFFPTR_X(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p1, *p2;
//	p1=dtvUnwrapPtr(frm->stack[op->t0].a);
//	p2=dtvUnwrapPtr(frm->stack[op->t1].a);
	p1=frm->stack[op->t0].p;
	p2=frm->stack[op->t1].p;
	frm->stack[op->t0].l=(((byte *)p1)-((byte *)p2))>>4;
}


void BSVM2_Op_CMPA_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	byte *p1, *p2;
	int i;
//	p1=dtvUnwrapPtr(frm->stack[op->t0].a);
//	p2=dtvUnwrapPtr(frm->stack[op->t1].a);
	p1=frm->stack[op->t0].p;
	p2=frm->stack[op->t1].p;
	i=0;
	if(p1>p2)i= 1;
	if(p1<p2)i=-1;
	frm->stack[op->t0].i=i;
}

void BSVM2_Op_LDIXI_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((u32 *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].i=*(s32 *)p;
}

void BSVM2_Op_LDIXL_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((u64 *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].l=*(s64 *)p;
}

void BSVM2_Op_LDIXF_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((u32 *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].f=*(f32 *)p;
}

void BSVM2_Op_LDIXD_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((u64 *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].d=*(f64 *)p;
}

void BSVM2_Op_LDIXA_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((dtVal *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].a=*(dtVal *)p;
}

void BSVM2_Op_LDIXSB_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((byte *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].i=*(sbyte *)p;
}

void BSVM2_Op_LDIXUB_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((byte *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].i=*(byte *)p;
}

void BSVM2_Op_LDIXSS_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((u16 *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].i=*(s16 *)p;
}

void BSVM2_Op_LDIXUS_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(
//		frm->stack[op->t0].a, frm->stack[op->t1].i);
	p=frm->stack[op->t0].p;
	p=((u16 *)p)+frm->stack[op->t1].i;
	frm->stack[op->t0].i=*(u16 *)p;
}


void BSVM2_Op_RSTIXI_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(
//		frm->stack[op->t1].a, frm->stack[op->t2].i);

	p=frm->stack[op->t1].p;
	p=((u32 *)p)+frm->stack[op->t2].i;

	*(s32 *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_RSTIXL_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(
//		frm->stack[op->t1].a, frm->stack[op->t2].i);
	p=frm->stack[op->t1].p;
	p=((u64 *)p)+frm->stack[op->t2].i;
	*(s64 *)p=frm->stack[op->t0].l;
}

void BSVM2_Op_RSTIXF_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(
//		frm->stack[op->t1].a, frm->stack[op->t2].i);
	p=frm->stack[op->t1].p;
	p=((u32 *)p)+frm->stack[op->t2].i;
	*(f32 *)p=frm->stack[op->t0].f;
}

void BSVM2_Op_RSTIXD_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(
//		frm->stack[op->t1].a, frm->stack[op->t2].i);
	p=frm->stack[op->t1].p;
	p=((u64 *)p)+frm->stack[op->t2].i;
	*(f64 *)p=frm->stack[op->t0].d;
}

void BSVM2_Op_RSTIXA_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(
//		frm->stack[op->t1].a, frm->stack[op->t2].i);
	p=frm->stack[op->t1].p;
	p=((dtVal *)p)+frm->stack[op->t2].i;
	*(dtVal *)p=frm->stack[op->t0].a;
}

void BSVM2_Op_RSTIXB_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(
//		frm->stack[op->t1].a, frm->stack[op->t2].i);
	p=frm->stack[op->t1].p;
	p=((byte *)p)+frm->stack[op->t2].i;
	*(byte *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_RSTIXS_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(
//		frm->stack[op->t1].a, frm->stack[op->t2].i);
	p=frm->stack[op->t1].p;
	p=((s16 *)p)+frm->stack[op->t2].i;
	*(s16 *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_LDIXIC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((u32 *)p)+op->v.i;
	frm->stack[op->t0].i=*(s32 *)p;
}

void BSVM2_Op_LDIXLC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((s64 *)p)+op->v.i;
	frm->stack[op->t0].l=*(s64 *)p;
}

void BSVM2_Op_LDIXFC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((u32 *)p)+op->v.i;
	frm->stack[op->t0].f=*(f32 *)p;
}

void BSVM2_Op_LDIXDC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((u64 *)p)+op->v.i;
	frm->stack[op->t0].d=*(f64 *)p;
}

void BSVM2_Op_LDIXAC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((dtVal *)p)+op->v.i;
	frm->stack[op->t0].a=*(dtVal *)p;
}

void BSVM2_Op_LDIXSBC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((byte *)p)+op->v.i;
	frm->stack[op->t0].i=*(sbyte *)p;
}

void BSVM2_Op_LDIXUBC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((byte *)p)+op->v.i;
	frm->stack[op->t0].i=*(byte *)p;
}

void BSVM2_Op_LDIXSSC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((s16 *)p)+op->v.i;
	frm->stack[op->t0].i=*(s16 *)p;
}

void BSVM2_Op_LDIXUSC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(frm->stack[op->t0].a, op->v.i);
	p=frm->stack[op->t0].p; p=((u16 *)p)+op->v.i;
	frm->stack[op->t0].i=*(u16 *)p;
}

void BSVM2_Op_RSTIXIC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(frm->stack[op->t1].a, op->v.i);
	p=frm->stack[op->t1].p; p=((u32 *)p)+op->v.i;
	*(s32 *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_RSTIXLC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	p=frm->stack[op->t1].p; p=((u64 *)p)+op->v.i;
	*(s64 *)p=frm->stack[op->t0].l;
}

void BSVM2_Op_RSTIXFC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(frm->stack[op->t1].a, op->v.i);
	p=frm->stack[op->t1].p; p=((u32 *)p)+op->v.i;
	*(f32 *)p=frm->stack[op->t0].f;
}

void BSVM2_Op_RSTIXDC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	p=frm->stack[op->t1].p; p=((u64 *)p)+op->v.i;
	*(f64 *)p=frm->stack[op->t0].d;
}

void BSVM2_Op_RSTIXAC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->stack[op->t1].a, op->v.i);
	p=frm->stack[op->t1].p; p=((dtVal *)p)+op->v.i;
	*(dtVal *)p=frm->stack[op->t0].a;
}

void BSVM2_Op_RSTIXBC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(frm->stack[op->t1].a, op->v.i);
	p=frm->stack[op->t1].p; p=((byte *)p)+op->v.i;
	*(byte *)p=frm->stack[op->t0].i;
}

void BSVM2_Op_RSTIXSC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(frm->stack[op->t1].a, op->v.i);
	p=frm->stack[op->t1].p; p=((byte *)p)+op->v.i;
	*(u16 *)p=frm->stack[op->t0].i;
}


void BSVM2_Op_LEARI_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((s32 *)p)+frm->stack[op->t1].i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARL_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((s64 *)p)+frm->stack[op->t1].i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARF_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((f32 *)p)+frm->stack[op->t1].i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARD_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((f64 *)p)+frm->stack[op->t1].i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARA_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((dtVal *)p)+frm->stack[op->t1].i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARB_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((byte *)p)+frm->stack[op->t1].i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARS_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((s16 *)p)+frm->stack[op->t1].i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARIC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((s32 *)p)+op->v.i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARLC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((s64 *)p)+op->v.i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARFC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((f32 *)p)+op->v.i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARDC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((f64 *)p)+op->v.i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARAC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((dtVal *)p)+op->v.i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARBC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((byte *)p)+op->v.i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}

void BSVM2_Op_LEARSC_P(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvUnwrapPtrF(frm->stack[op->t0].a);
	p=frm->stack[op->t0].p;
	p=((s16 *)p)+op->v.i;
//	frm->stack[op->t0].a=dtvWrapTyTagPtrF(p, 0);
	frm->stack[op->t0].p=p;
}


#if 1
void BSVM2_Op_LDDRVL_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(s32 *)p;
}

void BSVM2_Op_LDDRVL_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].l=*(s64 *)p;
}

void BSVM2_Op_LDDRVL_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].f=*(f32 *)p;
}

void BSVM2_Op_LDDRVL_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].d=*(f64 *)p;
}

void BSVM2_Op_LDDRVL_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].a=*(dtVal *)p;
}

void BSVM2_Op_LDDRVL_SB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(sbyte *)p;
}

void BSVM2_Op_LDDRVL_UB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(byte *)p;
}

void BSVM2_Op_LDDRVL_SS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(s16 *)p;
}

void BSVM2_Op_LDDRVL_US(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(u16 *)p;
}
#endif


#if 1
void BSVM2_Op_STDRVL_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; //p=dtvUnwrapPtrF(frm->local[op->i0].a);
	p=frm->local[op->i0].p;
	*(s32 *)p=frm->stack[op->t0].i;
}
void BSVM2_Op_STDRVL_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; //p=dtvUnwrapPtrF(frm->local[op->i0].a);
	p=frm->local[op->i0].p;
	*(s64 *)p=frm->stack[op->t0].l;
}
void BSVM2_Op_STDRVL_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; //p=dtvUnwrapPtrF(frm->local[op->i0].a);
	p=frm->local[op->i0].p;
	*(f32 *)p=frm->stack[op->t0].f;
}
void BSVM2_Op_STDRVL_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; //p=dtvUnwrapPtrF(frm->local[op->i0].a);
	p=frm->local[op->i0].p;
	*(f64 *)p=frm->stack[op->t0].d;
}
void BSVM2_Op_STDRVL_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; //p=dtvUnwrapPtrF(frm->local[op->i0].a);
	p=frm->local[op->i0].p;
	*(dtVal *)p=frm->stack[op->t0].a;
}
void BSVM2_Op_STDRVL_B(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; //p=dtvUnwrapPtrF(frm->local[op->i0].a);
	p=frm->local[op->i0].p;
	*(byte *)p=frm->stack[op->t0].i;
}
void BSVM2_Op_STDRVL_S(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; //p=dtvUnwrapPtrF(frm->local[op->i0].a);
	p=frm->local[op->i0].p;
	*(s16 *)p=frm->stack[op->t0].i;
}
#endif




#if 1
void BSVM2_Op_LDDINCZLP_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(s32 *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s32 *)p)+1, 0);
}

void BSVM2_Op_LDDINCZLP_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].l=*(s64 *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s64 *)p)+1, 0);
}

void BSVM2_Op_LDDINCZLP_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB4F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].f=*(f32 *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((f32 *)p)+1, 0);
}

void BSVM2_Op_LDDINCZLP_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].d=*(f64 *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((f64 *)p)+1, 0);
}

void BSVM2_Op_LDDINCZLP_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].a=*(dtVal *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((dtVal *)p)+1, 0);
}

void BSVM2_Op_LDDINCZLP_SB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB1F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(sbyte *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((sbyte *)p)+1, 0);
}

void BSVM2_Op_LDDINCZLP_UB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB1F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(byte *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((byte *)p)+1, 0);
}

void BSVM2_Op_LDDINCZLP_SS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB2F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(s16 *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s16 *)p)+1, 0);
}

void BSVM2_Op_LDDINCZLP_US(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
	p=dtvTagPtrGetIndexAddrB2F(frm->local[op->i0].a, op->v.i);
	frm->stack[op->t0].i=*(u16 *)p;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((u16 *)p)+1, 0);
}
#endif

#if 1
void BSVM2_Op_STDINCZLP_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(s32 *)p=frm->stack[op->t0].i;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s32 *)p)+1, 0);
}
void BSVM2_Op_STDINCZLP_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(s64 *)p=frm->stack[op->t0].l;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s64 *)p)+1, 0);
}
void BSVM2_Op_STDINCZLP_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(f32 *)p=frm->stack[op->t0].f;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((f32 *)p)+1, 0);
}
void BSVM2_Op_STDINCZLP_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(f64 *)p=frm->stack[op->t0].d;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((f64 *)p)+1, 0);
}
void BSVM2_Op_STDINCZLP_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(dtVal *)p=frm->stack[op->t0].a;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((dtVal *)p)+1, 0);
}
void BSVM2_Op_STDINCZLP_B(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(byte *)p=frm->stack[op->t0].i;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((byte *)p)+1, 0);
}
void BSVM2_Op_STDINCZLP_S(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(s16 *)p=frm->stack[op->t0].i;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s16 *)p)+1, 0);
}
#endif


#if 1
void BSVM2_Op_LDDDECZLP_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(s32 *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s32 *)p)-1, 0);
	frm->local[op->i0].p=((s32 *)p)-1;
}

void BSVM2_Op_LDDDECZLP_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].l=*(s64 *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s64 *)p)-1, 0);
	frm->local[op->i0].p=((s64 *)p)-1;
}

void BSVM2_Op_LDDDECZLP_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB4F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].f=*(f32 *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((f32 *)p)-1, 0);
	frm->local[op->i0].p=((s32 *)p)-1;
}

void BSVM2_Op_LDDDECZLP_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].d=*(f64 *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((f64 *)p)-1, 0);
	frm->local[op->i0].p=((s64 *)p)-1;
}

void BSVM2_Op_LDDDECZLP_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB8F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].a=*(dtVal *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((dtVal *)p)-1, 0);
	frm->local[op->i0].p=((dtVal *)p)-1;
}

void BSVM2_Op_LDDDECZLP_SB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(sbyte *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((sbyte *)p)-1, 0);
	frm->local[op->i0].p=((byte *)p)-1;
}

void BSVM2_Op_LDDDECZLP_UB(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB1F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(byte *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((byte *)p)-1, 0);
	frm->local[op->i0].p=((byte *)p)-1;
}

void BSVM2_Op_LDDDECZLP_SS(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(s16 *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s16 *)p)-1, 0);
	frm->local[op->i0].p=((s16 *)p)-1;
}

void BSVM2_Op_LDDDECZLP_US(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p;
//	p=dtvTagPtrGetIndexAddrB2F(frm->local[op->i0].a, op->v.i);
	p=frm->local[op->i0].p;
	frm->stack[op->t0].i=*(u16 *)p;
//	frm->local[op->i0].a=dtvWrapTyTagPtrF(((u16 *)p)-1, 0);
	frm->local[op->i0].p=((s16 *)p)-1;
}
#endif

#if 1
void BSVM2_Op_STDDECZLP_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(s32 *)p=frm->stack[op->t0].i;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s32 *)p)-1, 0);
}
void BSVM2_Op_STDDECZLP_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(s64 *)p=frm->stack[op->t0].l;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s64 *)p)-1, 0);
}
void BSVM2_Op_STDDECZLP_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(f32 *)p=frm->stack[op->t0].f;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((f32 *)p)-1, 0);
}
void BSVM2_Op_STDDECZLP_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(f64 *)p=frm->stack[op->t0].d;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((f64 *)p)-1, 0);
}
void BSVM2_Op_STDDECZLP_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(dtVal *)p=frm->stack[op->t0].a;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((dtVal *)p)-1, 0);
}
void BSVM2_Op_STDDECZLP_B(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(byte *)p=frm->stack[op->t0].i;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((byte *)p)-1, 0);
}
void BSVM2_Op_STDDECZLP_S(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	void *p; p=dtvUnwrapPtrF(frm->local[op->i0].a);
	*(s16 *)p=frm->stack[op->t0].i;
	frm->local[op->i0].a=dtvWrapTyTagPtrF(((s16 *)p)-1, 0);
}
#endif
