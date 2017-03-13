BS2VM_API u64 BSVM2_Double2XFloat(double v)
{
	u64 lv, lv1;
	int e1;
	lv=*(u64 *)(&v);
//	e1=(((lv>>52)&2047)-1023)+127;
	e1=((lv>>52)&2047)-896;
	if(e1>>8)
	{
		if(e1<0)e1=0;
		if(e1>255)e1=255;
	}
	lv1=((lv&0x8000000000000000ULL)   )|
		(((u64)e1)<<55)|
		((lv&0x000FFFFFFFFFFFFFULL)<<3);
	return(lv1);
}

BS2VM_API double BSVM2_XFloat2Double(u64 lv)
{
	u64 lv1;
	int e1;
	
//	e1=(((lv>>55)&255)-127)+1023;
//	e1=((lv>>55)&255)+896;
//	lv1=((lv&0x8000000000000000ULL)   )|
//		(((u64)e1)<<52)|
//		((lv&0x007FFFFFFFFFFFFFULL)>>3);
	lv1= ((lv&0x8000000000000000ULL)   )|
		(((lv&0x7FFFFFFFFFFFFFFFULL)>>3)+0x3800000000000000ULL);
	return(*(double *)(&lv1));
}

#if 0
BS2VM_API u64 BSVM2_Double2XFloat2(u64 lv)
{
	u64 lv1;
	int e1;
//	e1=(((lv>>52)&2047)-1023)+127;
	e1=((lv>>52)&2047)-896;
	if(e1>>8)
	{
		if(e1<0)e1=0;
		if(e1>255)e1=255;
	}
	lv1=((lv&0x8000000000000000ULL)   )|
		(((u64)e1)<<55)|
		((lv&0x000FFFFFFFFFFFFFULL)<<3);
	return(lv1);
}
#endif

#if 1
BS2VM_API u64 BSVM2_Double2XFloat2(u64 lv)
{
	const u64 n0=0x7FFFFFFFFFFFFFFFULL;
	const u64 n1=0x3800000000000000ULL;
	const u64 n2=0x8000000000000000ULL;
	u64 m3;
	u64 lv1;
//	int e1;
	
	lv1=(lv&n0)-n1;
//	lv1=lv1&(~(((s64)(lv1<<3))>>63));
//	lv1=lv1&(~(((s64)lv1)>>63));
//	lv2=lv1|(lv1<<1)|(lv1<<2);
	m3=lv1|(lv1<<1); m3|=m3<<2;
	m3=~(((s64)m3)>>63);
	lv1=((lv1&m3)<<3)|(lv&n2);
	return(lv1);
}
#endif

#if 0
BS2VM_API void BSVM2_Pack3DblTo3Xf(u32 *pv, double x, double y, double z)
{
	u64 lx, ly, lz;
	
	lx=BSVM2_Double2XFloat(x);
	ly=BSVM2_Double2XFloat(y);
	lz=BSVM2_Double2XFloat(z);
	pv[0]=lx>>32;
	pv[1]=ly>>32;
	pv[2]=lz>>32;
	pv[3]=
		(((lx>>20)&4095)    )|
		(((ly>>20)&4095)<<12)|
		(((lz>>24)& 255)<<24);
}

BS2VM_API void BSVM2_Pack3DvTo3Xf(u32 *pv, double *px)
{
	BSVM2_Pack3DblTo3Xf(pv, px[0], px[1], px[2]);
}
#endif

#if 1
BS2VM_API void BSVM2_Pack3DvTo3Xf(u32 *pv, double *px)
{
	u64 lx, ly, lz;
	
//	lx=((u64 *)px)[0]|((u64 *)px)[1]|((u64 *)px)[2];
	lx=((u64 *)px)[0];
	ly=((u64 *)px)[1];
	lz=((u64 *)px)[2];

	if(!((u32)(lx|ly|lz)))
//	if(0)
	{
		((float *)pv)[0]=px[0];
		((float *)pv)[1]=px[1];
		((float *)pv)[2]=px[2];
		pv[3]=0;
		return;
	}
	
	lx=BSVM2_Double2XFloat2(lx);
	ly=BSVM2_Double2XFloat2(ly);
	lz=BSVM2_Double2XFloat2(lz);
//	lx=BSVM2_Double2XFloat(px[0]);
//	ly=BSVM2_Double2XFloat(px[1]);
//	lz=BSVM2_Double2XFloat(px[2]);

	lx+=0x000FFFFFU;
	ly+=0x000FFFFFU;
	lz+=0x001FFFFFU;

	pv[0]=lx>>32;
	pv[1]=ly>>32;
	pv[2]=lz>>32;
//	pv[3]=
//		(((lx>>20)&4095)    )|
//		(((ly>>20)&4095)<<12)|
//		(((lz>>24)& 255)<<24);
//	pv[3]=
//		(((lx>>21)&2047)    )|
//		(((ly>>21)&2047)<<11)|
//		(((lz>>22)&1023)<<22);
//	pv[3]=
//		((lx>>21)&0x000007FF)|
//		((ly>>10)&0x003FF800)|
//		((lz    )&0xFFC00000);
	pv[3]=
		((lz>>22)&0x000003FF)|
		((ly>>11)&0x001FFC00)|
		((lx    )&0xFFE00000);
}

BS2VM_API void BSVM2_Pack3DblTo3Xf(u32 *pv, double x, double y, double z)
{
	double dv[3];
	dv[0]=x; dv[1]=y; dv[2]=z;
	BSVM2_Pack3DvTo3Xf(pv, dv);
}
#endif

#if 0
BS2VM_API void BSVM2_Unpack3XfTo3Dv(u32 *pv, double *px)
{
	u64 lx, ly, lz;

	if(pv[3])
	{
//		lx=(((u64)pv[0])<<32)|((pv[3]&0x00000FFF)<<20);
//		ly=(((u64)pv[1])<<32)|((pv[3]&0x00FFF000)<< 8);
//		lz=(((u64)pv[2])<<32)|((pv[3]&0xFF000000)    );
		lx=(((u64)pv[0])<<32)|((pv[3]&0x000007FF)<<21);
		ly=(((u64)pv[1])<<32)|((pv[3]&0x003FF800)<<10);
		lz=(((u64)pv[2])<<32)|((pv[3]&0xFFC00000)    );
		px[0]=BSVM2_XFloat2Double(lx);
		px[1]=BSVM2_XFloat2Double(ly);
		px[2]=BSVM2_XFloat2Double(lz);
	}else
	{
		px[0]=((float *)pv)[0];
		px[1]=((float *)pv)[1];
		px[2]=((float *)pv)[2];
	}
}
#endif

#if 1
BS2VM_API void BSVM2_Unpack3XfTo3Dv(u32 *pv, double *px)
{
	const u64 m0=0x8000000000000000ULL;
//	const u64 m1=0x7FFFFFFFFFFFFFFFULL;
	const u64 m1=0x7FFFFFFFFFE00000ULL;
	const u64 m2=0x3800000000000000ULL;
	u64 lx, ly, lz;
	u64 lx1, ly1, lz1;
//	u64 m0, m1, m2;

	if(pv[3])
//	if(1)
	{
//		lx=(((u64)pv[0])<<32)|((pv[3]&0x000007FF)<<21);
//		ly=(((u64)pv[1])<<32)|((pv[3]&0x003FF800)<<10);
//		lz=(((u64)pv[2])<<32)|((pv[3]&0xFFC00000)    );
//		lx=(((u64)pv[0])<<32)|((u32)(pv[3]<<21));
//		ly=(((u64)pv[1])<<32)|((u32)(pv[3]<<10));
//		lz=(((u64)pv[2])<<32)|((u32)(pv[3]    ));
		lx=(((u64)pv[0])<<32)|((u32)(pv[3]    ));
		ly=(((u64)pv[1])<<32)|((u32)(pv[3]<<11));
		lz=(((u64)pv[2])<<32)|((u32)(pv[3]<<22));
		lx1=(lx&m0)|(((lx&m1)>>3)+m2);
		ly1=(ly&m0)|(((ly&m1)>>3)+m2);
		lz1=(lz&m0)|(((lz&m1)>>3)+m2);
		px[0]=*(double *)(&lx1);
		px[1]=*(double *)(&ly1);
		px[2]=*(double *)(&lz1);
	}else
	{
		px[0]=((float *)pv)[0];
		px[1]=((float *)pv)[1];
		px[2]=((float *)pv)[2];
	}
}
#endif

BS2VM_API void BSVM2_Op_MKX3D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=BSVM2_FrameAllocX128(frm);
//	a->dx=frm->stack[op->t0].d;
//	a->dy=frm->stack[op->t1].d;
	BSVM2_Pack3DblTo3Xf((u32 *)a,
		frm->stack[op->t0].d,
		frm->stack[op->t1].d,
		frm->stack[op->t2].d);
	frm->stack[op->t0].p=a;
}

BS2VM_API void BSVM2_Op_LDX3DA_X3D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	frm->stack[op->t0].d=dv[0];
	BSVM2_FrameFreeX128(frm, a);
}

BS2VM_API void BSVM2_Op_LDX3DB_X3D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	frm->stack[op->t0].d=dv[1];
	BSVM2_FrameFreeX128(frm, a);
}

BS2VM_API void BSVM2_Op_LDX3DC_X3D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	frm->stack[op->t0].d=dv[2];
	BSVM2_FrameFreeX128(frm, a);
}


BS2VM_API void BSVM2_Op_BINOPX_ADD_V3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dva[3], dvb[3], dvc[3];
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;

	BSVM2_Unpack3XfTo3Dv((u32 *)a, dva);
	BSVM2_Unpack3XfTo3Dv((u32 *)b, dvb);
	dvc[0]=dva[0]+dvb[0];
	dvc[1]=dva[1]+dvb[1];
	dvc[2]=dva[2]+dvb[2];
//	BSVM2_Pack3DblTo3Xf((u32 *)a,
//		dvc[0], dvc[1], dvc[2]);
	BSVM2_Pack3DvTo3Xf((u32 *)a, dvc);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_SUB_V3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dva[3], dvb[3], dvc[3];
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;

	BSVM2_Unpack3XfTo3Dv((u32 *)a, dva);
	BSVM2_Unpack3XfTo3Dv((u32 *)b, dvb);
	dvc[0]=dva[0]-dvb[0];
	dvc[1]=dva[1]-dvb[1];
	dvc[2]=dva[2]-dvb[2];
//	BSVM2_Pack3DblTo3Xf((u32 *)a,
//		dvc[0], dvc[1], dvc[2]);
	BSVM2_Pack3DvTo3Xf((u32 *)a, dvc);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_MUL_V3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dva[3], dvb[3], dvc[3];
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;

	BSVM2_Unpack3XfTo3Dv((u32 *)a, dva);
	BSVM2_Unpack3XfTo3Dv((u32 *)b, dvb);
	dvc[0]=dva[0]*dvb[0];
	dvc[1]=dva[1]*dvb[1];
	dvc[2]=dva[2]*dvb[2];
//	BSVM2_Pack3DblTo3Xf((u32 *)a,
//		dvc[0], dvc[1], dvc[2]);
	BSVM2_Pack3DvTo3Xf((u32 *)a, dvc);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_DIV_V3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dva[3], dvb[3], dvc[3];
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;

	BSVM2_Unpack3XfTo3Dv((u32 *)a, dva);
	BSVM2_Unpack3XfTo3Dv((u32 *)b, dvb);
	dvc[0]=dva[0]/dvb[0];
	dvc[1]=dva[1]/dvb[1];
	dvc[2]=dva[2]/dvb[2];
//	BSVM2_Pack3DblTo3Xf((u32 *)a,
//		dvc[0], dvc[1], dvc[2]);
	BSVM2_Pack3DvTo3Xf((u32 *)a, dvc);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_BINOPX_DOT_V3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dva[3], dvb[3];
	BSVM2_ValX128 *a, *b;
	double f;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dva);
	BSVM2_Unpack3XfTo3Dv((u32 *)b, dvb);
	f=	(dva[0]*dvb[0])+
		(dva[1]*dvb[1])+
		(dva[2]*dvb[2]);
	BSVM2_FrameFreeX128(frm, a);
	BSVM2_FrameFreeX128(frm, b);
	frm->stack[op->t0].d=f;
}

BS2VM_API void BSVM2_Op_BINOPX_CROSS_V3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dva[3], dvb[3], dvc[3];
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dva);
	BSVM2_Unpack3XfTo3Dv((u32 *)b, dvb);
	dvc[2]=(dva[0]*dvb[1])-(dva[1]*dvb[0]);
	dvc[1]=(dva[1]*dvb[2])-(dva[2]*dvb[1]);
	dvc[0]=(dva[2]*dvb[0])-(dva[0]*dvb[2]);
//	BSVM2_Pack3DblTo3Xf((u32 *)a,
//		dvc[0], dvc[1], dvc[2]);
	BSVM2_Pack3DvTo3Xf((u32 *)a, dvc);
	BSVM2_FrameFreeX128(frm, b);
}

BS2VM_API void BSVM2_Op_CVTX3F2X3D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	a->ud=0;
}

BS2VM_API void BSVM2_Op_CVTX3D2X3F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	a->ud=0;
}


BS2VM_API void BSVM2_Op_UNOPX_NEG_X3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	dv[0]=-dv[0]; dv[1]=-dv[1]; dv[2]=-dv[2];
	BSVM2_Pack3DvTo3Xf((u32 *)a, dv);
}

BS2VM_API void BSVM2_Op_UNOPX_RCP_X3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	dv[0]=1.0/dv[0]; dv[1]=1.0/dv[1]; dv[2]=1.0/dv[2];
	BSVM2_Pack3DvTo3Xf((u32 *)a, dv);
}

BS2VM_API void BSVM2_Op_UNOPX_SQRT_X3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	dv[0]=bsvm2_opf_ssqrtf(dv[0]);
	dv[1]=bsvm2_opf_ssqrtf(dv[1]);
	dv[2]=bsvm2_opf_ssqrtf(dv[2]);
	BSVM2_Pack3DvTo3Xf((u32 *)a, dv);
}

BS2VM_API void BSVM2_Op_UNOPX_RSQRT_X3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	dv[0]=bsvm2_opf_ssqrtf(1.0/dv[0]);
	dv[1]=bsvm2_opf_ssqrtf(1.0/dv[1]);
	dv[2]=bsvm2_opf_ssqrtf(1.0/dv[2]);
	BSVM2_Pack3DvTo3Xf((u32 *)a, dv);
}

BS2VM_API void BSVM2_Op_UNOPX_LEN_X3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	BSVM2_FrameFreeX128(frm, a);
	frm->stack[op->t0].d=sqrt((dv[0]*dv[0])+(dv[1]*dv[1])+(dv[2]*dv[2]));
}

BS2VM_API void BSVM2_Op_UNOPX_LEN2_X3XF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	double dv[3];
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_Unpack3XfTo3Dv((u32 *)a, dv);
	BSVM2_FrameFreeX128(frm, a);
	frm->stack[op->t0].d=((dv[0]*dv[0])+(dv[1]*dv[1])+(dv[2]*dv[2]));
}
