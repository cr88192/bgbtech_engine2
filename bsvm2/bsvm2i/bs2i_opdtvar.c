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


BS2VM_API void BSVM2_Op_ADDAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Add(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_SUBAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Sub(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_MULAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Mul(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_ANDAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_And(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_ORAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Or(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_XORAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Xor(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_SHLAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Shl(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_SARAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Sar(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_SHRAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Shr(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_DIVAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Div(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_MODAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Mod(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}

BS2VM_API void BSVM2_Op_NEGAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Neg(frm->stack[op->t0].a);
}

BS2VM_API void BSVM2_Op_NOTAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_Not(frm->stack[op->t0].a);
}

BS2VM_API void BSVM2_Op_LNTAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].a=BGBDT_TagAri_LNot(frm->stack[op->t0].a);
}

BS2VM_API void BSVM2_Op_CMPAA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{ 
	frm->stack[op->t0].i=BGBDT_TagAri_Compare(
		frm->stack[op->t0].a, frm->stack[op->t1].a);
}
