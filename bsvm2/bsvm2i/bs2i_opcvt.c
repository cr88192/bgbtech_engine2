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


void BSVM2_Op_NOP(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
}

void BSVM2_Op_DUPI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t1].i; }
void BSVM2_Op_DUPL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t1].l; }
void BSVM2_Op_DUPF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t1].f; }
void BSVM2_Op_DUPD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t1].d; }
void BSVM2_Op_DUPA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=frm->stack[op->t1].a; }

void BSVM2_Op_PUSHI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=0; }
void BSVM2_Op_PUSHL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=0; }
void BSVM2_Op_PUSHF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=0; }
void BSVM2_Op_PUSHD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=0; }
void BSVM2_Op_PUSHA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=DTV_NULL; }

void BSVM2_Op_SWAPI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	s32 n0, n1;
	n0=frm->stack[op->t0].i;	n1=frm->stack[op->t1].i;
	frm->stack[op->t0].i=n1;	frm->stack[op->t1].i=n0;	}
void BSVM2_Op_SWAPL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	s64 n0, n1;
	n0=frm->stack[op->t0].l;	n1=frm->stack[op->t1].l;
	frm->stack[op->t0].l=n1;	frm->stack[op->t1].l=n0;	}
void BSVM2_Op_SWAPF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	float n0, n1;
	n0=frm->stack[op->t0].f;	n1=frm->stack[op->t1].f;
	frm->stack[op->t0].f=n1;	frm->stack[op->t1].f=n0;	}
void BSVM2_Op_SWAPD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	double n0, n1;
	n0=frm->stack[op->t0].d;	n1=frm->stack[op->t1].d;
	frm->stack[op->t0].d=n1;	frm->stack[op->t1].d=n0;	}
void BSVM2_Op_SWAPA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	dtVal n0, n1;
	n0=frm->stack[op->t0].a;	n1=frm->stack[op->t1].a;
	frm->stack[op->t0].a=n1;	frm->stack[op->t1].a=n0;	}

void BSVM2_Op_CVTI2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t0].i; }
void BSVM2_Op_CVTI2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].i; }
void BSVM2_Op_CVTI2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t0].i; }

void BSVM2_Op_CVTL2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].l; }
void BSVM2_Op_CVTL2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].l; }
void BSVM2_Op_CVTL2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t0].l; }

void BSVM2_Op_CVTF2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].f; }
void BSVM2_Op_CVTF2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t0].f; }
void BSVM2_Op_CVTF2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->stack[op->t0].f; }

void BSVM2_Op_CVTD2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].d; }
void BSVM2_Op_CVTD2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t0].d; }
void BSVM2_Op_CVTD2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].d; }

void BSVM2_Op_CVTSB2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(sbyte)frm->stack[op->t0].i; }
void BSVM2_Op_CVTUB2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(byte)frm->stack[op->t0].i; }
void BSVM2_Op_CVTSS2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(s16)frm->stack[op->t0].i; }
void BSVM2_Op_CVTUS2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=(u16)frm->stack[op->t0].i; }

void BSVM2_Op_CVTUI2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->stack[op->t0].ui; }

BS2VM_API void BSVM2_Op_CVTI2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvWrapInt(frm->stack[op->t0].i); }
BS2VM_API void BSVM2_Op_CVTL2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvWrapLong(frm->stack[op->t0].l); }
BS2VM_API void BSVM2_Op_CVTF2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvWrapFloat(frm->stack[op->t0].f); }
BS2VM_API void BSVM2_Op_CVTD2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=dtvWrapDouble(frm->stack[op->t0].d); }

BS2VM_API void BSVM2_Op_CVTAA2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=dtvUnwrapInt(frm->stack[op->t0].a); }
BS2VM_API void BSVM2_Op_CVTAA2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=dtvUnwrapLong(frm->stack[op->t0].a); }
BS2VM_API void BSVM2_Op_CVTAA2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=dtvUnwrapFloat(frm->stack[op->t0].a); }
BS2VM_API void BSVM2_Op_CVTAA2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=dtvUnwrapDouble(frm->stack[op->t0].a); }


BS2VM_API void BSVM2_Op_CVTAA2ST(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	char tb[4096];
	dtVal v;
	v=frm->stack[op->t0].a;
	BGBDT_MM_PrintValueToStrBuf(tb, 4095, v);
	frm->stack[op->t0].a=BGBDT_TagStr_String(tb);
}

BS2VM_API void BSVM2_Op_CVTST2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	char *s;
	dtVal v;

	v=frm->stack[op->t0].a;
	s=BGBDT_TagStr_GetChars(v);
	v=BGBDT_TagParse_ParseValueFromStrBuf(s, strlen(s));
	frm->stack[op->t0].a=v;
}

BS2VM_API void BSVM2_Op_CATST(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	char *s0, *s1, *s2;
	dtVal v0, v1;

	v0=frm->stack[op->t0].a;
	v1=frm->stack[op->t1].a;
	s0=BGBDT_TagStr_GetChars(v0);
	s1=BGBDT_TagStr_GetChars(v1);
	s2=bgbdt_mm_ralloc(strlen(s0)+strlen(s1)+2);
	strcpy(s2, s0);
	strcat(s2, s1);
	frm->stack[op->t0].a=BGBDT_TagStr_String(s2);
}

int bsvm2_op_strcmp(byte *s0, byte *s1)
{
	if(!s0 || !s1)
	{
		if(s0)
			return(1);
		if(s1)
			return(-1);
		return(0);
	}
	while(*s0 && *s1 && (*s0==*s1))
		{ s0++; s1++; }
	if((*s0)<(*s1))
		return(-1);
	if((*s0)>(*s1))
		return(1);
	return(0);
}

byte *bsvm2_op_strgetutf8(byte *cs, int *rv)
{
	int i;

	i=*cs++;

	if(i<0x80)
		{ *rv=i; return(cs); }
	if(i<0xE0)
	{
		i=((i&0x1F)<<6)|((*cs++)&0x3F);
		*rv=i;
		return(cs);
	}
	if(i<0xF0)
	{
		i=((i&0x0F)<<6)|((*cs++)&0x3F);
		i=(i<<6)|((*cs++)&0x3F);
		*rv=i;
		return(cs);
	}
	if(i<0xF8)
	{
		i=((i&0x07)<<6)|((*cs++)&0x3F);
		i=(i<<6)|((*cs++)&0x3F);
		i=(i<<6)|((*cs++)&0x3F);
		*rv=i;
		return(cs);
	}
	*rv=i;
	return(cs);
}

int bsvm2_op_mapucase(int ch)
{
	if((ch>='a') && (ch<='z'))
		return((ch-'a')+'A');
	return(ch);
}

int bsvm2_op_stricmp(byte *s0, byte *s1)
{
	int i, j;

	if(!s0 || !s1)
	{
		if(s0)
			return(1);
		if(s1)
			return(-1);
		return(0);
	}
	while(*s0 && *s1)
	{
		s0=bsvm2_op_strgetutf8(s0, &i);
		s1=bsvm2_op_strgetutf8(s1, &j);
		i=bsvm2_op_mapucase(i);
		j=bsvm2_op_mapucase(j);
		if(i!=j)
			break;
	}
	
	if(!(*s0) || !(*s1))
	{
		if(*s0)
			return(1);
		if(*s1)
			return(-1);
		return(0);
	}
	
	if(i<j)
		return(-1);
	if(i>j)
		return(1);
	return(0);
}

BS2VM_API void BSVM2_Op_CMPST(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	char *s0, *s1, *s2;
	dtVal v0, v1;

	v0=frm->stack[op->t0].a;
	v1=frm->stack[op->t1].a;
	s0=BGBDT_TagStr_GetChars(v0);
	s1=BGBDT_TagStr_GetChars(v1);
	frm->stack[op->t0].i=bsvm2_op_strcmp((byte *)s0, (byte *)s1);
}

BS2VM_API void BSVM2_Op_CMPSST(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	char *s0, *s1, *s2;
	dtVal v0, v1;

	v0=frm->stack[op->t0].a;
	v1=frm->stack[op->t1].a;
	s0=BGBDT_TagStr_GetUtf8(v0);
	s1=BGBDT_TagStr_GetUtf8(v1);
	frm->stack[op->t0].i=bsvm2_op_strcmp((byte *)s0, (byte *)s1);
}

BS2VM_API void BSVM2_Op_CMPUST(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	char *s0, *s1, *s2;
	dtVal v0, v1;

	v0=frm->stack[op->t0].a;
	v1=frm->stack[op->t1].a;
	s0=BGBDT_TagStr_GetUtf8(v0);
	s1=BGBDT_TagStr_GetUtf8(v1);
	frm->stack[op->t0].i=bsvm2_op_stricmp((byte *)s0, (byte *)s1);
}
