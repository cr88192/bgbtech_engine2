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


int BSVM2_Interp_DecodeOpUCxI(BSVM2_CodeBlock *cblk)
{
	int i;
	i=*cblk->cs++;
	if(i<0x80)
	{
		return(i);
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		return(i);
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}

	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}

	if(i<0xF8)
	{
		i=((i&0x0F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}
	
	cblk->cs--;
	return(BSVM2_Interp_DecodeOpUCxL(cblk));
	
//	return(0);
}

u64 BSVM2_Interp_DecodeOpUCxL(BSVM2_CodeBlock *cblk)
{
	s64 li;
	int i;

	i=*cblk->cs++;
	if(i<0x80)
	{
		return(i);
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		return(i);
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}

	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		return(i);
	}

	if(i<0xF8)
	{
		li=((i&0x0F)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		return(li);
	}

	if(i<0xFC)
	{
		li=((i&0x07)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		return(li);
	}
	if(i<0xFE)
	{
		li=((i&0x03)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		return(li);
	}

	if(i==0xFE)
	{
		li=*cblk->cs++;
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		return(li);
	}

	li=*cblk->cs++;
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	li=(li<<8)|(*cblk->cs++);
	return(li);
}

s32 BSVM2_Interp_DecodeOpSCxI(BSVM2_CodeBlock *cblk)
{
	s32 i;
	
	i=BSVM2_Interp_DecodeOpUCxI(cblk);
	i=(((u32)i)>>1)^((i<<31)>>31);
	return(i);
}

s64 BSVM2_Interp_DecodeOpSCxL(BSVM2_CodeBlock *cblk)
{
	s64 li;
	int i;
	
	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	li=(((u64)li)>>1)^((li<<63)>>63);
	return(li);
}

void BSVM2_Interp_DecodeOpUJxI(BSVM2_CodeBlock *cblk,
	int *ri, int *rj)
{
	int i, j;

	i=*cblk->cs++;
	if(i<0x80)
	{
		*ri=(i>>4)&7;
		*rj=i&15;
		return;
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		*ri=(i>>7)&127;
		*rj=(i   )&127;
		return;
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);

		*ri=(i>>11)&1023;
		*rj=(i    )&2047;
		return;
	}

	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>14)&16383;
		*rj=(i    )&16383;
		return;
	}

	if(i<0xF8)
	{
		i=((i&0x0F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>16)&65535;
		*rj=(i    )&65535;
		return;
	}
}

void BSVM2_Interp_DecodeOpUKx(BSVM2_CodeBlock *cblk,
	int *ri, u64 *rj)
{
	u64 li;
	int i, j;

	i=*cblk->cs++;
	if(i<0x80)
	{
		*ri=(i>>4)&7;
		*rj=i&15;
		return;
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		*ri=(i>>9)&31;
		*rj=(i   )&511;
		return;
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>13)&255;
		*rj=(i    )&8191;
		return;
	}
	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>20)&255;
		*rj=(i    )&((1<<20)-1);
		return;
	}
	if(i<0xF8)
	{
		li=((i&0x0F)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		*ri=(li>>27)&255;
		*rj=li&((1<<27)-1);
		return;
	}
	if(i<0xFC)
	{
		li=((i&0x07)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		*ri=(li>>32)&1023;
		*rj=li&((1LL<<32)-1);
		return;
	}
	if(i<0xFE)
	{
		li=((i&0x03)<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);

		*ri=(li>>39)&1023;
		*rj=li&((1LL<<39)-1);
		return;
	}
	if(i==0xFE)
	{
		li=*cblk->cs++;
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);

		*ri=(li>>44)&4095;
		*rj=li&((1LL<<44)-1);
		return;
	}
	if(i==0xFF)
	{
		li=*cblk->cs++;
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);
		li=(li<<8)|(*cblk->cs++);

		*ri=(li>>52)&4095;
		*rj=li&((1LL<<52)-1);
		return;
	}
}

void BSVM2_Interp_DecodeOpULxI(BSVM2_CodeBlock *cblk,
	int *ri, int *rj, int *rk)
{
	int i, j;

	i=*cblk->cs++;
	if(i<0x80)
	{
		*ri=(i>>4)&7;
		*rj=(i>>2)&3;
		*rk=(i   )&3;
		return;
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		*ri=(i>>9)&31;
		*rj=(i>>4)&31;
		*rk=(i   )&15;
		return;
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>14)&127;
		*rj=(i>> 7)&127;
		*rk=(i    )&127;
		return;
	}

	if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		*ri=(i>>18)&1023;
		*rj=(i>> 9)&511;
		*rk=(i    )&511;
		return;
	}

	if(i<0xF8)
	{
		i=((i&0x0F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
//		*ri=(i>>16)&65535;
//		*rj=(i    )&65535;
		return;
	}
}

s32 bsvm2_interp_decsignfoldi(s32 i)
	{ return((((u32)i)>>1)^((i<<31)>>31)); }

s64 bsvm2_interp_decsignfoldl(s64 i)
	{ return((((u64)i)>>1)^((i<<63)>>63)); }

dtVal BSVM2_Interp_DecodeOpAddrConst(BSVM2_CodeBlock *cblk, int ix)
{
	BSVM2_ImageGlobal *vi;
	dtVal v;
	char *s;
	int i, j;
	
	i=cblk->gitab[ix];
	
	if(!(i&3))
	{
		vi=BS2I_ImageGetGlobal(cblk->img, i>>2);

		j=vi->tag;
		if(	((i>=BS2CC_ITCC_A0) && (i<=BS2CC_ITCC_A9)) ||
			((i>=BS2CC_ITCC_Aa) && (i<=BS2CC_ITCC_Az)) )
				{ return(vi->gvalue->a); }

		return(dtvWrapPtr(vi));
	}
	
	j=i>>4;
	switch(i&15)
	{
	case BSVM2_OPZY_STRU8:
		s=cblk->img->strtab+j;
		v=BGBDT_TagStr_String(s);
		break;
	case BSVM2_OPZY_STRU16:
		s=cblk->img->strtab+j;
		v=BGBDT_TagStr_String16u8(s);
		break;
	case BSVM2_OPZY_STRASC:
		s=cblk->img->strtab+j;
		v=BGBDT_TagStr_StringAsc(s);
		break;

	case BSVM2_OPZY_STRSYM:
		s=cblk->img->strtab+j;
		v=BGBDT_TagStr_Symbol(s);
		break;
	case BSVM2_OPZY_STRKEY:
		s=cblk->img->strtab+j;
		v=BGBDT_TagStr_Keyword(s);
		break;

	case BSVM2_OPZY_INT:
		j=bsvm2_interp_decsignfoldi(j);
		v=dtvWrapInt(j);
		break;
	case BSVM2_OPZY_LONG:
		j=bsvm2_interp_decsignfoldi(j);
		v=dtvWrapLong(j);
		break;
	case BSVM2_OPZY_UINT:
		v=dtvWrapUInt(j);
		break;
	case BSVM2_OPZY_FLOAT:
	case BSVM2_OPZY_DOUBLE:
		j=bsvm2_interp_decsignfoldi(j);
		v=dtvWrapDouble(j*(1.0/4096));
		break;
	default:
		v=DTV_UNDEFINED;
		break;
	}
	return(v);
}

void *BSVM2_Interp_DecodeOpAddrPtr(BSVM2_CodeBlock *cblk, int ix)
{
	BSVM2_ImageGlobal *vi;
//	dtVal v;
	void *p;
	char *s;
	int i, j;
	
	i=cblk->gitab[ix];
	
	if(!(i&3))
	{
		vi=BS2I_ImageGetGlobal(cblk->img, i>>2);
		return(vi);
	}
	
	j=i>>4;
	switch(i&15)
	{
	case BSVM2_OPZY_STRU8:
		s=cblk->img->strtab+j;
		p=BGBDT_TagStr_Strdup(s);
		break;
	case BSVM2_OPZY_STRU16:
		s=cblk->img->strtab+j;
		p=BGBDT_TagStr_Strdup16u8(s);
		break;
	case BSVM2_OPZY_STRASC:
		s=cblk->img->strtab+j;
		p=BGBDT_TagStr_StrdupL1(s);
		break;

	case BSVM2_OPZY_STRSYM:
		s=cblk->img->strtab+j;
		p=BGBDT_TagStr_StrSymbol(s);
		break;
	case BSVM2_OPZY_STRKEY:
		s=cblk->img->strtab+j;
		p=BGBDT_TagStr_StrKeyword(s);
		break;

	case BSVM2_OPZY_INT:
	case BSVM2_OPZY_LONG:
	case BSVM2_OPZY_UINT:
	case BSVM2_OPZY_FLOAT:
	case BSVM2_OPZY_DOUBLE:
	default:
		p=NULL;
		break;
	}
	return(p);
}

void BSVM2_Interp_DecodeOpGx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	int i, ix;

	ix=BSVM2_Interp_DecodeOpUCxI(cblk);
//	op->i0=BSVM2_Interp_DecodeOpUCxI(cblk);

	i=cblk->gitab[ix];
	if(!(i&3))
	{
		vi=BSVM2_Interp_DecodeOpAddrPtr(cblk, ix);
		op->v.p=vi;
		op->i1=vi->brty;
		op->z=vi->brty;
	}else
	{
		p=BSVM2_Interp_DecodeOpAddrPtr(cblk, ix);
		op->v.p=p;
		op->i1=BSVM2_OPZ_CONST;
		op->z=BSVM2_OPZ_CONST;
	}
}

void BSVM2_Interp_DecodeOpIxGx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	int i, ix;

	op->i0=BSVM2_Interp_DecodeOpUCxI(cblk);
	ix=BSVM2_Interp_DecodeOpUCxI(cblk);
	i=cblk->gitab[ix];
	if(!(i&3))
	{
		vi=BSVM2_Interp_DecodeOpAddrPtr(cblk, ix);
		op->v.p=vi;
		op->i1=vi->brty;
		op->z=vi->brty;
	}else
	{
		p=BSVM2_Interp_DecodeOpAddrPtr(cblk, ix);
		op->v.p=p;
		op->i1=BSVM2_OPZ_CONST;
		op->z=BSVM2_OPZ_CONST;
	}
}

void BSVM2_Interp_DecodeOpGj(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	void *p;
	int i, j, k, ix;

	BSVM2_Interp_DecodeOpUJxI(cblk, &i, &j);

	op->i0=i;
	ix=j;
	k=cblk->gitab[ix];
	if(!(k&3))
	{
		vi=BSVM2_Interp_DecodeOpAddrPtr(cblk, ix);
		op->v.p=vi;
		op->i1=vi->brty;
		op->z=vi->brty;
	}else
	{
		p=BSVM2_Interp_DecodeOpAddrPtr(cblk, ix);
		op->v.p=p;
		op->i1=BSVM2_OPZ_CONST;
		op->z=BSVM2_OPZ_CONST;
	}
}

void BSVM2_Interp_DecodeOpIx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
	{ op->i0=BSVM2_Interp_DecodeOpUCxI(cblk); }

void BSVM2_Interp_DecodeOpJx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	int i, j;

	BSVM2_Interp_DecodeOpUJxI(cblk, &i, &j);
	op->i0=i;
	op->i1=j;
	op->opfl|=BSVM2_OPFL_USED_I0;
	op->opfl|=BSVM2_OPFL_USED_I1;
}

void BSVM2_Interp_DecodeOpLx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	int i, j, k;

	BSVM2_Interp_DecodeOpULxI(cblk, &i, &j, &k);
	op->i0=i;
	op->i1=j;
	op->i2=k;
	op->opfl|=BSVM2_OPFL_USED_I0;
	op->opfl|=BSVM2_OPFL_USED_I1;
	op->opfl|=BSVM2_OPFL_USED_I2;
}

void BSVM2_Interp_DecodeOpJxCx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty)
{
	BSVM2_Interp_DecodeOpJx(cblk, op);
	BSVM2_Interp_DecodeOpCx(cblk, op, zty);
}

void BSVM2_Interp_DecodeOpJx_SLL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty)
{
	BSVM2_Interp_DecodeOpJx(cblk, op);
	op->i2=op->i1;
	op->i1=op->i0;
	op->i0=(cblk->stkpos++)+cblk->largs;
	op->opfl|=BSVM2_OPFL_USED_I0;
	op->opfl|=BSVM2_OPFL_USED_I1;
	op->opfl|=BSVM2_OPFL_USED_I2;
}

void BSVM2_Interp_DecodeOpJx_LSL(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	BSVM2_Interp_DecodeOpJx(cblk, op);
	op->i2=op->i1;
	op->i1=(--cblk->stkpos)+cblk->largs;
//	op->i1=op->i0;
	op->opfl|=BSVM2_OPFL_USED_I0;
	op->opfl|=BSVM2_OPFL_USED_I1;
	op->opfl|=BSVM2_OPFL_USED_I2;
}

void BSVM2_Interp_DecodeOpKx_SLC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty)
{
	BSVM2_Interp_DecodeOpKx(cblk, op, zty);
	op->i1=op->i0;
	op->i0=(cblk->stkpos++)+cblk->largs;
	op->opfl|=BSVM2_OPFL_USED_I0;
	op->opfl|=BSVM2_OPFL_USED_I1;
}

void BSVM2_Interp_DecodeOpKx_LSC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty)
{
	BSVM2_Interp_DecodeOpKx(cblk, op, zty);
	op->i1=(--cblk->stkpos)+cblk->largs;
	op->opfl|=BSVM2_OPFL_USED_I0;
	op->opfl|=BSVM2_OPFL_USED_I1;
}

void BSVM2_Interp_DecodeOpKx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty)
{
	u64 li;
	int i, j;

	BSVM2_Interp_DecodeOpUKx(cblk, &j, &li);
	op->i0=j;
	op->z=zty;
	op->opfl|=BSVM2_OPFL_USED_I0;

	switch(zty)
	{
	case BSVM2_OPZ_INT:	case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_SHORT:
		op->v.i=bsvm2_interp_decsignfoldl(li); break;
	case BSVM2_OPZ_LONG:
		op->v.l=bsvm2_interp_decsignfoldl(li); break;

	case BSVM2_OPZ_FLOAT:
	case BSVM2_OPZ_DOUBLE:
		i=bsvm2_interp_decsignfoldl(li);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, zty);
		break;

	case BSVM2_OPZ_ADDRESS:
		op->v.a=BSVM2_Interp_DecodeOpAddrConst(cblk, li);
		break;
	case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_USHORT:
		op->v.i=li; break;
	case BSVM2_OPZ_ULONG:
		op->v.l=li; break;
	default:
		break;
	}
}

void BSVM2_Interp_DecodeOpFx2(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op,
	int exp, int zty)
{
	double f;
	u32 vi;
	u64 lj;
	s64 li, li2;
	int i, j, k, e, sg;

	e=exp;
	li=BSVM2_Interp_DecodeOpSCxL(cblk);
	li2=li;

	if(!li)
	{
		switch(zty)
		{
		case BSVM2_OPZ_FLOAT:
			op->v.f=0; break;
		case BSVM2_OPZ_DOUBLE:
			op->v.d=0; break;
		default:
			break;
		}
		return;
	}

	sg=0;
	if(li<0)
		{ li=-li; sg=1; }

	while(!(li>>32))
		{ li=(li<<16); e-=16; }
	while(!(li>>44))
		{ li=(li<<8); e-=8; }
	while(!(li>>52))
		{ li=(li<<1); e--; }

	e=e+1023;
	lj=(li&0x000FFFFFFFFFFFFFULL)|(((u64)e)<<52)|(((u64)sg)<<63);
	f=*(double *)(&lj);

	f=((double)li2)*pow(2.0, exp);

	switch(zty)
	{
	case BSVM2_OPZ_FLOAT:
		op->v.f=f; break;
	case BSVM2_OPZ_DOUBLE:
		op->v.d=f; break;
	default:
		op->v.l=0; break;
		break;
	}
}

void BSVM2_Interp_DecodeOpZx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i1=li&15;
	lj=li>>4;
	op->z=op->i1;

	switch(op->i1)
	{
	case BSVM2_OPZ_INT:
	case BSVM2_OPZ_SBYTE:
	case BSVM2_OPZ_SHORT:
		op->v.i=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_LONG:
		op->v.l=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_FLOAT:
	case BSVM2_OPZ_DOUBLE:
		i=bsvm2_interp_decsignfoldl(lj);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, op->i1);
		break;
	case BSVM2_OPZ_ADDRESS:
		op->v.a=BSVM2_Interp_DecodeOpAddrConst(cblk, lj);
		break;
	case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_USHORT:
		op->v.i=lj; break;
	case BSVM2_OPZ_ULONG:
		op->v.ul=lj; break;
	case BSVM2_OPZ_CONST:
		op->o=lj;
		op->v.i=lj; break;

	case BSVM2_OPZ_CI_FLOAT:
		op->v.f=bsvm2_interp_decsignfoldl(lj);
		break;
	case BSVM2_OPZ_CI_DOUBLE:
		op->v.d=bsvm2_interp_decsignfoldl(lj);
		break;
	
	default:
		break;
	}
}

void BSVM2_Interp_DecodeOpZy(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i1=li&15;
	lj=li>>4;
	op->z=op->i1;
	
	switch(op->i1)
	{
	case BSVM2_OPZ_INT:
		op->v.a=dtvWrapInt(bsvm2_interp_decsignfoldl(lj));
		break;
	case BSVM2_OPZ_UINT:
		op->v.a=dtvWrapUInt(lj);
		break;
	case BSVM2_OPZ_LONG:
		op->v.a=dtvWrapLong(bsvm2_interp_decsignfoldl(lj));
		break;
	case BSVM2_OPZ_ULONG:
		op->v.a=dtvWrapLong(lj);
		break;
	case BSVM2_OPZ_FLOAT:
		i=bsvm2_interp_decsignfoldl(lj);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, op->i1);
		op->v.a=dtvWrapFloat(op->v.f);
		break;
	case BSVM2_OPZ_DOUBLE:
		i=bsvm2_interp_decsignfoldl(lj);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, op->i1);
		op->v.a=dtvWrapDouble(op->v.d);
		break;

	case BSVM2_OPZ_SBYTE:
		op->v.a=BGBDT_TagStr_StringAsc(cblk->img->strtab+op->v.i);
		break;
	case BSVM2_OPZ_UBYTE:
		op->v.a=BGBDT_TagStr_String(cblk->img->strtab+op->v.i);
		break;

	case BSVM2_OPZ_USHORT:
	case BSVM2_OPZ_SHORT:
		op->v.a=BGBDT_TagStr_String16u8(cblk->img->strtab+op->v.i);
		break;

	case BSVM2_OPZ_ADDRESS:
		op->v.a=BSVM2_Interp_DecodeOpAddrConst(cblk, lj);
		break;

	default:
		break;
	}
}

void BSVM2_Interp_DecodeOpZn(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i1=li&15;
	lj=li>>4;
	op->i0=lj;
	op->z=op->i1;
}

void BSVM2_Interp_DecodeOpZnIx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i2=li&15;
	lj=li>>4;
	op->i0=lj;
	op->i1=BSVM2_Interp_DecodeOpUCxI(cblk);
	op->z=op->i2;

//	op->opfl|=BSVM2_OPFL_USED_I0;
	op->opfl|=BSVM2_OPFL_USED_I1;
}

void BSVM2_Interp_DecodeOpZiIx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 li, lj;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i2=li&15;
	lj=li>>4;
	op->i0=lj;
	op->i1=BSVM2_Interp_DecodeOpUCxI(cblk);
	op->z=op->i2;

	op->opfl|=BSVM2_OPFL_USED_I0;
	op->opfl|=BSVM2_OPFL_USED_I1;
}

void BSVM2_Interp_DecodeOpZiCi(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 li, lj, lk;
	int i, j;

	li=BSVM2_Interp_DecodeOpUCxL(cblk);
	op->i2=li&15;
	lj=li>>4;
	op->i0=lj;
	op->z=op->i2;
	op->opfl|=BSVM2_OPFL_USED_I0;
//	BSVM2_Interp_DecodeOpCx(cblk, op, )
	lk=BSVM2_Interp_DecodeOpSCxL(cblk);
//	lk=bsvm2_interp_decsignfoldl(lk);
	op->v.i=lk;
}

void BSVM2_Interp_DecodeOpFx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op,
	int zty)
{
	double f;
	u32 vi;
	u64 lj;
	s64 li;
	int i, j, k, e, sg;

	e=BSVM2_Interp_DecodeOpSCxI(cblk);
	li=BSVM2_Interp_DecodeOpSCxL(cblk);

	sg=0;
	if(li<0)
		{ li=-li; sg=1; }

	while(!(li>>32))
		{ li=(li<<16); e-=16; }
	while(!(li>>44))
		{ li=(li<<8); e-=8; }
	while(!(li>>52))
		{ li=(li<<1); e--; }

	e=e+1023;
	lj=(li&0x000FFFFFFFFFFFFFULL)|(((u64)e)<<52)|(((u64)sg)<<63);
	f=*(double *)(&lj);

	switch(zty)
	{
	case BSVM2_OPZ_FLOAT:
		op->v.f=f; break;
	case BSVM2_OPZ_DOUBLE:
		op->v.d=f; break;
	default:
		op->v.l=0; break;
		break;
	}
}

void BSVM2_Interp_DecodeOpCx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op,
	int zty)
{
	s64 lj;
	int i, j;

	if((zty==BSVM2_OPZ_FLOAT) || (zty==BSVM2_OPZ_DOUBLE))
	{
		BSVM2_Interp_DecodeOpFx(cblk, op, zty);
		return;
	}

	lj=BSVM2_Interp_DecodeOpUCxL(cblk);

	switch(zty)
	{
	case BSVM2_OPZ_INT:
	case BSVM2_OPZ_SBYTE:
	case BSVM2_OPZ_SHORT:
		op->v.i=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_LONG:
		op->v.l=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_FLOAT:
	case BSVM2_OPZ_DOUBLE:
		i=bsvm2_interp_decsignfoldl(lj);
		BSVM2_Interp_DecodeOpFx2(cblk, op, i, zty);
		break;
	case BSVM2_OPZ_ADDRESS:
		op->v.a=BSVM2_Interp_DecodeOpAddrConst(cblk, lj);
		break;
	case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_USHORT:
		op->v.i=lj; break;
	case BSVM2_OPZ_ULONG:
		op->v.ul=lj; break;
	default:
		break;
	}
}


void BSVM2_Interp_SetupOpBin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;

	cblk->stkpos--;
	op->t0=cblk->stkpos-1;
	op->t1=cblk->stkpos;

	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
}

void BSVM2_Interp_SetupOpUn(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos-1;
	op->opfl|=BSVM2_OPFL_USED_T0;
}

void BSVM2_Interp_SetupOpUnL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos++;
	op->opfl|=BSVM2_OPFL_USED_T0;
	BSVM2_Interp_DecodeOpIx(cblk, op);
}

void BSVM2_Interp_SetupOpUstL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	cblk->stkpos--;
	op->t0=cblk->stkpos;
	op->opfl|=BSVM2_OPFL_USED_T0;
	BSVM2_Interp_DecodeOpIx(cblk, op);
}

void BSVM2_Interp_SetupOpUnP(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t1=cblk->stkpos-1;
	op->t0=cblk->stkpos++;
	op->opfl|=BSVM2_OPFL_USED_T0;
//	op->opfl|=BSVM2_OPFL_USED_T1;
}

void BSVM2_Interp_SetupOpUnPGx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_DecodeOpGx(cblk, op);
	op->Run=run;
	op->t1=cblk->stkpos-1;
	op->t0=cblk->stkpos++;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
}

void BSVM2_Interp_SetupOpUat(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
}

void BSVM2_Interp_SetupOpUatL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpIx(cblk, op);
}

void BSVM2_Interp_SetupOpUatJx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_DecodeOpJx(cblk, op);
	op->Run=run;
}

void BSVM2_Interp_SetupOpUatKx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_DecodeOpKx(cblk, op, zty);
	op->Run=run;
}

void BSVM2_Interp_SetupOpUat2(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos-1;
	op->t1=cblk->stkpos-2;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
}

void BSVM2_Interp_SetupOpPopUn(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=--cblk->stkpos;
	op->opfl|=BSVM2_OPFL_USED_T0;
}

void BSVM2_Interp_SetupOpPopBin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
}

void BSVM2_Interp_SetupOpPopTrin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t2=--cblk->stkpos;
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
	op->opfl|=BSVM2_OPFL_USED_T2;
}

void BSVM2_Interp_SetupOpBinC2(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t0=cblk->stkpos-1;
	op->opfl|=BSVM2_OPFL_USED_T0;
}

void BSVM2_Interp_SetupOpBinC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpBinC2(cblk, op, zty, zty, run);
}

void BSVM2_Interp_SetupOpBinCI(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpBinC2(cblk, op, zty, BSVM2_OPZ_INT, run);
}

void BSVM2_Interp_SetupOpBinCJ(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpBinC2(cblk, op, zty, BSVM2_OPZ_UINT, run);
}

void BSVM2_Interp_SetupOpBinCiSa(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op),
	void (*run2)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, BSVM2_OPZ_INT);
	op->t0=cblk->stkpos-1;
	op->opfl|=BSVM2_OPFL_USED_T0;

	if(op->v.i<0)
	{
		op->v.i=-op->v.i;
		op->Run=run2;
	}
}

void BSVM2_Interp_SetupOpBinL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpIx(cblk, op);
	op->t0=cblk->stkpos-1;
	op->opfl|=BSVM2_OPFL_USED_T0;
}

void BSVM2_Interp_SetupOpBinLC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpKx(cblk, op, zty);
	op->t0=cblk->stkpos++;
	op->opfl|=BSVM2_OPFL_USED_T0;
}

void BSVM2_Interp_SetupOpBinLL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpJx(cblk, op);
	op->t0=cblk->stkpos++;
	op->opfl|=BSVM2_OPFL_USED_T0;
}

void BSVM2_Interp_SetupOpBinC2P(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t1=cblk->stkpos-1;
	op->t0=cblk->stkpos++;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
}


void BSVM2_Interp_SetupOpTrinC2(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t1=--cblk->stkpos;
	op->t0=cblk->stkpos-1;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
}

void BSVM2_Interp_SetupOpTrinCiSa(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op),
	void (*run2)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, BSVM2_OPZ_INT);
	op->t1=--cblk->stkpos;
	op->t0=cblk->stkpos-1;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
	
	if(op->v.i<0)
	{
		op->v.i=-op->v.i;
		op->Run=run2;
	}
}

void BSVM2_Interp_SetupOpTrinC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpTrinC2(cblk, op, zty, zty, run);
}

void BSVM2_Interp_SetupOpTrinCI(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpTrinC2(cblk, op, zty, BSVM2_OPZ_INT, run);
}

void BSVM2_Interp_SetupOpPopTrinC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
}

void BSVM2_Interp_SetupOpPopTrinCI(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpPopTrinC(cblk, op, BSVM2_OPZ_INT, run);
}


void BSVM2_Interp_SetupOpTrin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;

	op->t2=cblk->stkpos--;
	op->t1=cblk->stkpos--;
	op->t0=cblk->stkpos-1;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
	op->opfl|=BSVM2_OPFL_USED_T2;
}

void BSVM2_Interp_SetupOpQuad(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;

	cblk->stkpos--;
	op->t2=cblk->stkpos--;
	op->t1=cblk->stkpos--;
	op->t0=cblk->stkpos-1;
	op->opfl|=BSVM2_OPFL_USED_T0;
	op->opfl|=BSVM2_OPFL_USED_T1;
	op->opfl|=BSVM2_OPFL_USED_T2;
	op->opfl|=BSVM2_OPFL_USED_T3;
}


void BSVM2_Interp_SetupOpI_SLL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpJx_SLL(cblk, op, BSVM2_OPZ_INT);
}

void BSVM2_Interp_SetupOp_LSL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpJx_LSL(cblk, op);
}

void BSVM2_Interp_SetupOpI_SLC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpKx_SLC(cblk, op, BSVM2_OPZ_INT);
}

void BSVM2_Interp_SetupOpI_LSC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpKx_LSC(cblk, op, BSVM2_OPZ_INT);
}

void BSVM2_Interp_SetupOpUI_SLC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpKx_SLC(cblk, op, BSVM2_OPZ_UINT);
}

void BSVM2_Interp_SetupOpUI_LSC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpKx_LSC(cblk, op, BSVM2_OPZ_UINT);
}


void BSVM2_Interp_SetupOpI_LLC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpJxCx(cblk, op, BSVM2_OPZ_INT);
}

void BSVM2_Interp_SetupOpUI_LLC(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpJxCx(cblk, op, BSVM2_OPZ_UINT);
}

void BSVM2_Interp_SetupOpI_LLL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpLx(cblk, op);
}
