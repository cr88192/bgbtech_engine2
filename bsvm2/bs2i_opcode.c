BSVM2_Opcode *BSVM2_Interp_AllocOpcode(BSVM2_CodeBlock *cblk)
{
}

void BSVM2_Interp_SetupOpBin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;

	cblk->stkpos--;
	op->t0=cblk->stkpos-1;
	op->t1=cblk->stkpos;
}

void BSVM2_Interp_SetupOpUn(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos-1;
}

void BSVM2_Interp_DecodeOpIx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	int i;
	i=*cblk->cs++;
	if(i<0x80)
	{
		op->i0=i;
		return;
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		op->i0=i;
		return;
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		op->i0=i;
		return;
	}
}

void BSVM2_Interp_DecodeOpJx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	int i, j;

	i=*cblk->cs++;
	if(i<0x80)
	{
		op->i0=(i>>4)&7;
		op->i1=i&15;
		return;
	}
	if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		op->i0=(i>>7)&127;
		op->i1=(i   )&127;
		return;
	}
	if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);

		op->i0=(i>>10)&1023;
		op->i1=(i    )&1023;
		return;
	}
}


s64 bsvm2_interp_decsignfoldl(s64 i)
	{ return((i>>1)^(i>>63)); }

void BSVM2_Interp_DecodeOpZx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op)
{
	s64 lj;
	int i, j;
	i=*cblk->cs++;
	op->i1=(i>>4);

	switch(i&15)
	{
	case 0: case 1: case 2: case 3:
	case 4: case 5: case 6: case 7:
		lj=(i&15);
		break;
	case 8: case 9: case 10: case 11:
		lj=(((i&15)-8)<<8)|(*cblk->cs++);
		break;
	case 12: case 13:
		lj=(((i&15)-12)<<8)|(*cblk->cs++);
		lj=(lj<<8)|(*cblk->cs++);
		break;
	case 14:
		lj=*cblk->cs++;					lj=(lj<<8)|(*cblk->cs++);
		lj=(lj<<8)|(*cblk->cs++);		lj=(lj<<8)|(*cblk->cs++);
		break;
	case 15:
		lj=*cblk->cs++;					lj=(lj<<8)|(*cblk->cs++);
		lj=(lj<<8)|(*cblk->cs++);		lj=(lj<<8)|(*cblk->cs++);
		lj=(lj<<8)|(*cblk->cs++);		lj=(lj<<8)|(*cblk->cs++);
		lj=(lj<<8)|(*cblk->cs++);		lj=(lj<<8)|(*cblk->cs++);
		break;
	}

	switch(op->i1)
	{
	case BSVM2_OPZ_INT:
	case BSVM2_OPZ_SBYTE:
	case BSVM2_OPZ_SHORT:
		op->v.i=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_LONG:
		op->v.l=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_FLOAT:
		op->v.f=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_DOUBLE:
		op->v.d=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_ADDRESS:
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

void BSVM2_Interp_DecodeOpFx(BSVM2_CodeBlock *cblk, BSVM2_Opcode *op,
	int zty)
{
	double f;
	u32 vi;
	u64 lj;
	int i, j, k;

	i=*cblk->cs++;
	if(i<0x80)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		i=i<<1;
	}else if(i<0xC0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		vi=i<<10;
		f=*(f32 *)(&vi);
	}else if(i<0xE0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		vi=i<<3;
		f=*(f32 *)(&vi);
	}else if(i<0xF0)
	{
		i=((i&0x0F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		j=*cblk->cs++;
		lj=(((u64)i)<<36)|(((u64)j)<<28);
		f=*(f64 *)(&lj);
	}else if(i<0xF8)
	{
		i=((i&0x07)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		j=*cblk->cs++;
		j=(j<<8)|(*cblk->cs++);
		lj=(((u64)i)<<37)|(((u64)j)<<21);
		f=*(f64 *)(&lj);
	}else if(i<0xFC)
	{
		i=((i&0x03)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		j=*cblk->cs++;
		j=(j<<8)|(*cblk->cs++);
		j=(j<<8)|(*cblk->cs++);
		lj=(((u64)i)<<38)|(((u64)j)<<14);
		f=*(f64 *)(&lj);
	}else if(i<0xFE)
	{
		i=((i&0x03)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		j=*cblk->cs++;
		j=(j<<8)|(*cblk->cs++);
		j=(j<<8)|(*cblk->cs++);
		k=*cblk->cs++;
		lj=(((u64)i)<<39)|(((u64)j)<<15)|(k<<7);
		f=*(f64 *)(&lj);
	}else if(i==0xFE)
	{
		j=*cblk->cs++;
		j=(j<<8)|(*cblk->cs++);
		j=(j<<8)|(*cblk->cs++);
		j=(j<<8)|(*cblk->cs++);
		vi=j;
		f=*(f32 *)(&vi);
	}else if(i==0xFF)
	{
		i=*cblk->cs++;
		i=(i<<8)|(*cblk->cs++);
		j=*cblk->cs++;
		j=(j<<8)|(*cblk->cs++);
		j=(j<<8)|(*cblk->cs++);
		k=*cblk->cs++;
		k=(k<<8)|(*cblk->cs++);
		k=(k<<8)|(*cblk->cs++);
		lj=(((u64)i)<<48)|(((u64)j)<<24)|k;
		f=*(f64 *)(&lj);
	}

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
		break;
	}

	i=*cblk->cs++;
	if(i<0x80)
	{
		lj=i;
	}else if(i<0xC0)
	{
		i=((i&0x7F)<<8)|(*cblk->cs++);
		lj=i;
	}else if(i<0xE0)
	{
		i=((i&0x3F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		lj=i;
	}else if(i<0xF0)
	{
		i=((i&0x1F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		lj=i;
	}else if(i<0xF8)
	{
		i=((i&0x0F)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
//		i=(i<<8)|(*cblk->cs++);
		j=*cblk->cs++;
		lj=(((s64)i)<<8)|j;
	}else if(i<0xFC)
	{
		i=((i&0x07)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		j=*cblk->cs++;
		j=(j<<8)|(*cblk->cs++);
		lj=(((s64)i)<<16)|j;
	}else if(i<0xFE)
	{
		i=((i&0x03)<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
		j=*cblk->cs++;
		j=(j<<8)|(*cblk->cs++);
		j=(j<<8)|(*cblk->cs++);
		lj=(((s64)i)<<24)|j;
	}

	switch(zty)
	{
	case BSVM2_OPZ_INT:
	case BSVM2_OPZ_SBYTE:
	case BSVM2_OPZ_SHORT:
		op->v.i=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_LONG:
		op->v.l=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_FLOAT:
		op->v.f=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_DOUBLE:
		op->v.d=bsvm2_interp_decsignfoldl(lj); break;
	case BSVM2_OPZ_ADDRESS:
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

void BSVM2_Interp_SetupOpUnL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos++;
	BSVM2_Interp_DecodeOpIx(cblk, op);
}

void BSVM2_Interp_SetupOpUstL(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	cblk->stkpos--;
	op->t0=cblk->stkpos;
	BSVM2_Interp_DecodeOpIx(cblk, op);
}

void BSVM2_Interp_SetupOpUnP(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int zty,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=cblk->stkpos++;
}

void BSVM2_Interp_SetupOpUat(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
}

void BSVM2_Interp_SetupOpUatJx(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_DecodeOpJx(cblk, op);
	op->Run=run;
}

void BSVM2_Interp_SetupOpPopUn(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupOpPopBin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupOpPopTrin(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	op->t2=--cblk->stkpos;
	op->t1=--cblk->stkpos;
	op->t0=--cblk->stkpos;
}

void BSVM2_Interp_SetupOpBinC2(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t0=cblk->stkpos-1;
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

void BSVM2_Interp_SetupOpTrinC2(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op, int ztyr, int ztyc,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	op->Run=run;
	BSVM2_Interp_DecodeOpCx(cblk, op, ztyc);
	op->t1=--cblk->stkpos;
	op->t0=cblk->stkpos-1;
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
}

void BSVM2_Interp_SetupOpPopTrinCI(BSVM2_CodeBlock *cblk,
	BSVM2_Opcode *op,
	void (*run)(BSVM2_Frame *frm, BSVM2_Opcode *op))
{
	BSVM2_Interp_SetupOpPopTrinC2(cblk, op, BSVM2_OPZ_INT, run);
}


BSVM2_Opcode *BSVM2_Interp_DecodeOpcode(BSVM2_CodeBlock *cblk)
{
	BSVM2_Opcode *tmp;
	int i;
	
	i=(*cblk->cs++);
	if((i>=0xE0) && (i<=0xEF))
	{
		i=((i-0xE0)<<8)|(*cblk->cs++);
	}else if((i>=0xF0) && (i<=0xF7))
	{
		i=i-0xF0;
		i=(i<<8)|(*cblk->cs++);
		i=(i<<8)|(*cblk->cs++);
	}

	tmp=BSVM2_Interp_AllocOpcode(cblk);
	switch(i)
	{
	case BSVM2_OP_ADDI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_ADDI);
		break;
	case BSVM2_OP_SUBI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_SUBI);
		break;
	case BSVM2_OP_MULI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_MULI);
		break;
	case BSVM2_OP_ANDI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_ANDI);
		break;
	case BSVM2_OP_ORI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_ORI);
		break;
	case BSVM2_OP_XORI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_XORI);
		break;
	case BSVM2_OP_SHLI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_SHLI);
		break;
	case BSVM2_OP_SARI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_SARI);
		break;
	case BSVM2_OP_ADDL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_ADDL);
		break;
	case BSVM2_OP_SUBL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_SUBL);
		break;
	case BSVM2_OP_MULL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_MULL);
		break;
	case BSVM2_OP_ANDL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_ANDL);
		break;
	case BSVM2_OP_ORL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_ORL);
		break;
	case BSVM2_OP_XORL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_XORL);
		break;
	case BSVM2_OP_SHLL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_SHLL);
		break;
	case BSVM2_OP_SARL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_SARL);
		break;
	case BSVM2_OP_ADDF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_ADDF);
		break;
	case BSVM2_OP_SUBF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_SUBF);
		break;
	case BSVM2_OP_MULF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_MULF);
		break;
	case BSVM2_OP_DIVF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_DIVF);
		break;
	case BSVM2_OP_ADDD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_ADDD);
		break;
	case BSVM2_OP_SUBD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_SUBD);
		break;
	case BSVM2_OP_MULD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_MULD);
		break;
	case BSVM2_OP_DIVD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_DIVD);
		break;

	case BSVM2_OP_NEGI:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_NEGI);
		break;
	case BSVM2_OP_NEGL:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_NEGL);
		break;
	case BSVM2_OP_NEGF:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_NEGF);
		break;
	case BSVM2_OP_NEGD:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_NEGD);
		break;
	case BSVM2_OP_NOTI:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_NOTI);
		break;
	case BSVM2_OP_NOTL:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_NOTL);
		break;
	case BSVM2_OP_LNTI:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LNTI);
		break;
	case BSVM2_OP_LNTL:
		BSVM2_Interp_SetupOpUn(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LNTL);
		break;

	case BSVM2_OP_LDI:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDI);
		break;
	case BSVM2_OP_LDL:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_LDL);
		break;
	case BSVM2_OP_LDF:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_LDF);
		break;
	case BSVM2_OP_LDD:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_LDD);
		break;
	case BSVM2_OP_LDA:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_ADDRESS, BSVM2_Op_LDA);
		break;

	case BSVM2_OP_STI:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_STI);
		break;
	case BSVM2_OP_STL:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_STL);
		break;
	case BSVM2_OP_STF:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_STF);
		break;
	case BSVM2_OP_STD:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_STD);
		break;
	case BSVM2_OP_STA:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_ADDRESS, BSVM2_Op_STA);
		break;
	case BSVM2_OP_LDC:
		BSVM2_Interp_DecodeOpZx(cblk, op);
		switch(op->i1)
		{
		case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
			BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDCI);
			break;
		case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
			BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_LDCL);
			break;
		case BSVM2_OPZ_FLOAT:
			BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_LDCF);
			break;
		case BSVM2_OPZ_DOUBLE:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_LDCD);
			break;
		case BSVM2_OPZ_ADDRESS:
			BSVM2_Interp_SetupOpUnP(cblk, op,
				BSVM2_OPZ_ADDRESS, BSVM2_Op_LDCA);
			break;
		default:
			break;
		}
		break;

	case BSVM2_OP_MVI:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVI);
		break;
	case BSVM2_OP_MVL:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVL);
		break;
	case BSVM2_OP_MVF:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVF);
		break;
	case BSVM2_OP_MVD:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVD);
		break;
	case BSVM2_OP_MVA:
		BSVM2_Interp_SetupOpUatJx(cblk, op, BSVM2_Op_MVA);
		break;

	case BSVM2_OP_CMPI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPI);
		break;
	case BSVM2_OP_CMPL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPL);
		break;
	case BSVM2_OP_CMPF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPF);
		break;
	case BSVM2_OP_CMPD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPD);
		break;
	case BSVM2_OP_CMPA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPA);
		break;
	case BSVM2_OP_CMP2A:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPA);
		break;
	case BSVM2_OP_CMP2F:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPF);
		break;
	case BSVM2_OP_CMP2D:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_CMPD);
		break;

	case BSVM2_OP_LDIXI:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXI);
		break;
	case BSVM2_OP_LDIXL:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_LONG, BSVM2_Op_LDIXL);
		break;
	case BSVM2_OP_LDIXF:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXF);
		break;
	case BSVM2_OP_LDIXD:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXD);
		break;
	case BSVM2_OP_LDIXA:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LDIXA);
		break;
	case BSVM2_OP_LDIXSB:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXSB);
		break;
	case BSVM2_OP_LDIXUB:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXUB);
		break;
	case BSVM2_OP_LDIXSS:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXSS);
		break;
	case BSVM2_OP_LDIXUS:
		BSVM2_Interp_SetupOpBin(cblk, op, BSVM2_OPZ_INT, BSVM2_Op_LDIXUS);
		break;
	
	case BSVM2_OP_STIXI:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXI);
		break;
	case BSVM2_OP_STIXL:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXL);
		break;
	case BSVM2_OP_STIXF:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXF);
		break;
	case BSVM2_OP_STIXD:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXD);
		break;
	case BSVM2_OP_STIXA:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXA);
		break;
	case BSVM2_OP_STIXB:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXB);
		break;
	case BSVM2_OP_STIXS:
		BSVM2_Interp_SetupOpPopTrin(cblk, op, BSVM2_Op_STIXS);
		break;

	case BSVM2_OP_LDIXIC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXIC);
		break;
	case BSVM2_OP_LDIXLC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_LONG, BSVM2_Op_LDIXLC);
		break;
	case BSVM2_OP_LDIXFC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDIXFC);
		break;
	case BSVM2_OP_LDIXDC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_LDIXDC);
		break;
	case BSVM2_OP_LDIXAC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_LDIXAC);
		break;
	case BSVM2_OP_LDIXSBC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXSBC);
		break;
	case BSVM2_OP_LDIXUBC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXUBC);
		break;
	case BSVM2_OP_LDIXSSC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXSSC);
		break;
	case BSVM2_OP_LDIXUSC:
		BSVM2_Interp_SetupOpBinCI(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDIXUSC);
		break;

	case BSVM2_OP_STIXI:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXI);
		break;
	case BSVM2_OP_STIXL:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXL);
		break;
	case BSVM2_OP_STIXF:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXF);
		break;
	case BSVM2_OP_STIXD:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXD);
		break;
	case BSVM2_OP_STIXA:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXA);
		break;
	case BSVM2_OP_STIXB:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXB);
		break;
	case BSVM2_OP_STIXS:
		BSVM2_Interp_SetupOpPopTrinCI(cblk, op, BSVM2_Op_STIXS);
		break;
	}
}
