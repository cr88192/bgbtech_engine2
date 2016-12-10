BS2VM_API u64 BSVM2_Double2XFloat(double v)
{
	u64 lv, lv1;
	int e1;
	lv=*(u64 *)(&v);
	e1=(((lv>>52)&2047)-1023)+127;
	if(e1<0)e1=0;
	if(e1>255)e1=255;
	lv1=((lv&0x8000000000000000ULL)   )|
		(((u64)e1)<<55)|
		((lv&0x000FFFFFFFFFFFFFULL)<<3);
	return(lv1);
}

BS2VM_API double BSVM2_XFloat2Double(u64 lv)
{
	u64 lv1;
	int e1;
	
	e1=(((lv>>55)&255)-127)+1023;
	lv1=((lv&0x8000000000000000ULL)   )|
		(((u64)e1)<<52)|
		((lv&0x007FFFFFFFFFFFFFULL)>>3);
	return(*(double *)(&lv1));
}

BS2VM_API u64 BSVM2_Double2XFloat2(u64 lv)
{
	u64 lv1;
	int e1;
	e1=(((lv>>52)&2047)-1023)+127;
	if(e1<0)e1=0;
	if(e1>255)e1=255;
	lv1=((lv&0x8000000000000000ULL)   )|
		(((u64)e1)<<55)|
		((lv&0x000FFFFFFFFFFFFFULL)<<3);
	return(lv1);
}

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
	pv[0]=lx>>32;
	pv[1]=ly>>32;
	pv[2]=lz>>32;
	pv[3]=
		(((lx>>20)&4095)    )|
		(((ly>>20)&4095)<<12)|
		(((lz>>24)& 255)<<24);
}

BS2VM_API void BSVM2_Pack3DblTo3Xf(u32 *pv, double x, double y, double z)
{
	double dv[3];
	dv[0]=x; dv[1]=y; dv[2]=z;
	BSVM2_Pack3DvTo3Xf(pv, dv);
}
#endif

BS2VM_API void BSVM2_Unpack3XfTo3Dv(u32 *pv, double *px)
{
	u64 lx, ly, lz;

	if(pv[3])
	{
		lx=(((u64)pv[0])<<32)|((pv[3]&0x00000FFF)<<20);
		ly=(((u64)pv[1])<<32)|((pv[3]&0x00FFF000)<< 8);
		lz=(((u64)pv[2])<<32)|((pv[3]&0xFF000000)    );
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