#include <bteifgl.h>

BSVM2_ValX128 *BSVM2_FrameAllocX128(BSVM2_Frame *frm)
{
	BSVM2_ValX128 *a;
	void *p;
	int i;

	p=frm->ctx->freex128;
	if(p)
	{
		frm->ctx->freex128=*(void **)p;
		return(p);
	}
	
	a=dtmAlloc("bsvm2_x128_t", 128*16);
	for(i=0; i<127; i++)
		{ BSVM2_FrameFreeX128(frm, a++); }
	return(a);
}

void BSVM2_FrameFreeX128(BSVM2_Frame *frm, BSVM2_ValX128 *val)
{
	*(void **)val=frm->ctx->freex128;
	frm->ctx->freex128=val;
}

void BSVM2_Op_BINOPX_ADD_V3F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	a->fx=a->fx+b->fx;
	a->fy=a->fy+b->fy;
	a->fz=a->fz+b->fz;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_SUB_V3F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	a->fx=a->fx-b->fx;
	a->fy=a->fy-b->fy;
	a->fz=a->fz-b->fz;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_MUL_V3F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	a->fx=a->fx*b->fx;
	a->fy=a->fy*b->fy;
	a->fz=a->fz*b->fz;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_DIV_V3F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	a->fx=a->fx/b->fx;
	a->fy=a->fy/b->fy;
	a->fz=a->fz/b->fz;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_DOT_V3F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	float f;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	f=	(a->fx*b->fx)+
		(a->fy*b->fy)+
		(a->fz*b->fz);
	BSVM2_FrameFreeX128(frm, a);
	BSVM2_FrameFreeX128(frm, b);
	frm->stack[op->t0].f=f;
}

void BSVM2_Op_BINOPX_CROSS_V3F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	float x, y, z;
	
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	z=(a->fx*b->fy)-(a->fy*b->fx);
	x=(a->fy*b->fz)-(a->fz*b->fy);
	y=(a->fz*b->fx)-(a->fx*b->fz);
	a->fx=x;	a->fy=y;	a->fz=z;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_CMUL_V3F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	float x, y, z;
	
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	z=(a->fx*b->fy)-(a->fy*b->fx);
	x=(a->fy*b->fz)-(a->fz*b->fy);
	y=(a->fz*b->fx)-(a->fx*b->fz);
	a->fx=x;	a->fy=y;	a->fz=z;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_ADD_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	a->fx=a->fx+b->fx;
	a->fy=a->fy+b->fy;
	a->fz=a->fz+b->fz;
	a->fw=a->fw+b->fw;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_SUB_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	a->fx=a->fx-b->fx;
	a->fy=a->fy-b->fy;
	a->fz=a->fz-b->fz;
	a->fw=a->fw-b->fw;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_MUL_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	a->fx=a->fx*b->fx;
	a->fy=a->fy*b->fy;
	a->fz=a->fz*b->fz;
	a->fw=a->fw*b->fw;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_DIV_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	a->fx=a->fx/b->fx;
	a->fy=a->fy/b->fy;
	a->fz=a->fz/b->fz;
	a->fw=a->fw/b->fw;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_DOT_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	float f;
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	f=	(a->fx*b->fx)+
		(a->fy*b->fy)+
		(a->fz*b->fz)+
		(a->fw*b->fw);
	BSVM2_FrameFreeX128(frm, a);
	BSVM2_FrameFreeX128(frm, b);
	frm->stack[op->t0].f=f;
}

void BSVM2_Op_BINOPX_CROSS_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	float x, y, z, w;
	
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	z=(a->fx*b->fy)-(a->fy*b->fx);
	x=(a->fy*b->fz)-(a->fz*b->fy);
	y=(a->fz*b->fx)-(a->fx*b->fz);
	w=a->fw*b->fw;
	a->fx=x;	a->fy=y;	a->fz=z;	a->fw=w;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_CMUL_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	float x, y, z, w;
	
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	z=(a->fw*b->fz)+(a->fz*b->fw)+(a->fx*b->fy)-(a->fy*b->fx);
	x=(a->fw*b->fx)+(a->fx*b->fw)+(a->fy*b->fz)-(a->fz*b->fy);
	y=(a->fw*b->fy)+(a->fy*b->fw)+(a->fz*b->fx)-(a->fx*b->fz);
	w=(a->fw*b->fw)-(a->fx*b->fx)-(a->fy*b->fy)-(a->fz*b->fz);
	a->fx=x;	a->fy=y;	a->fz=z;	a->fw=w;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_CMUL2_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	float x, y, z, w;
	
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	z=(a->fw*b->fz)+(a->fz*b->fw)+(b->fx*a->fy)-(b->fy*a->fx);
	x=(a->fw*b->fx)+(a->fx*b->fw)+(b->fy*a->fz)-(b->fz*a->fy);
	y=(a->fw*b->fy)+(a->fy*b->fw)+(b->fz*a->fx)-(b->fx*a->fz);
	w=(a->fw*b->fw)-(a->fx*b->fx)-(a->fy*b->fy)-(a->fz*b->fz);
	a->fx=x;	a->fy=y;	a->fz=z;	a->fw=w;
	BSVM2_FrameFreeX128(frm, b);
}

void bsvm2_opf_qrcp(BSVM2_ValX128 *a, BSVM2_ValX128 *b)
{
	float fa, fb, fc, fd;
	float f, g;
	
	fa=a->fx;	fb=a->fy;
	fc=a->fz;	fd=a->fw;
	f=fa*fa+fb*fb+fc*fc+fd*fd;
	g=1.0/f;
	b->fx=-fa*g;
	b->fy=-fb*g;
	b->fz=-fc*g;
	b->fw=fd*g;
}

void BSVM2_Op_BINOPX_CDIV_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 trb;
	BSVM2_ValX128 *a, *b, *rb;
	float x, y, z, w;
	
	rb=&trb;	
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	bsvm2_opf_qrcp(b, rb);
	z=(a->fw*rb->fz)+(a->fz*rb->fw)+(a->fx*rb->fy)-(a->fy*rb->fx);
	x=(a->fw*rb->fx)+(a->fx*rb->fw)+(a->fy*rb->fz)-(a->fz*rb->fy);
	y=(a->fw*rb->fy)+(a->fy*rb->fw)+(a->fz*rb->fx)-(a->fx*rb->fz);
	w=(a->fw*rb->fw)-(a->fx*rb->fx)-(a->fy*rb->fy)-(a->fz*rb->fz);
	a->fx=x;	a->fy=y;	a->fz=z;	a->fw=w;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_BINOPX_CDIV2_V4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 trb;
	BSVM2_ValX128 *a, *b, *rb;
	float x, y, z, w;
	
	rb=&trb;	
	a=frm->stack[op->t0].p;
	b=frm->stack[op->t1].p;
	bsvm2_opf_qrcp(b, rb);
	z=(rb->fw*a->fz)+(rb->fz*a->fw)+(rb->fx*a->fy)-(rb->fy*a->fx);
	x=(rb->fw*a->fx)+(rb->fx*a->fw)+(rb->fy*a->fz)-(rb->fz*a->fy);
	y=(rb->fw*a->fy)+(rb->fy*a->fw)+(rb->fz*a->fx)-(rb->fx*a->fz);
	w=(rb->fw*a->fw)-(rb->fx*a->fx)-(rb->fy*a->fy)-(rb->fz*a->fz);
	a->fx=x;	a->fy=y;	a->fz=z;	a->fw=w;
	BSVM2_FrameFreeX128(frm, b);
}


void BSVM2_Op_DUPX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;
	a=frm->stack[op->t0].p;
	b=BSVM2_FrameAllocX128(frm);
	frm->stack[op->t1].p=b;
	*b=*a;
}

void BSVM2_Op_POPX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	BSVM2_FrameFreeX128(frm, a);
}

void BSVM2_Op_LDXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;

	a=frm->local[op->i0].p;
	b=BSVM2_FrameAllocX128(frm);
	*b=*a;
	frm->stack[op->t0].p=b;
}

void BSVM2_Op_STXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a, *b;

	a=frm->local[op->i0].p;
	b=frm->stack[op->t0].p;
	*a=*b;
	BSVM2_FrameFreeX128(frm, b);
}

void BSVM2_Op_IFXX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=BSVM2_FrameAllocX128(frm);
	frm->local[op->i0].p=a;
}

void BSVM2_Op_DFXX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->local[op->i0].p;
	BSVM2_FrameFreeX128(frm, a);
}

void BSVM2_Op_IFXXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
}

void BSVM2_Op_MKX4I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=BSVM2_FrameAllocX128(frm);
	a->ia=frm->stack[op->t0+0].i;
	a->ib=frm->stack[op->t0+1].i;
	a->ic=frm->stack[op->t0+2].i;
	a->id=frm->stack[op->t0+3].i;
	frm->stack[op->t0].p=a;
}

void BSVM2_Op_MKX2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=BSVM2_FrameAllocX128(frm);
	a->la=frm->stack[op->t0].l;
	a->lb=frm->stack[op->t1].l;
	frm->stack[op->t0].p=a;
}

void BSVM2_Op_MKX4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=BSVM2_FrameAllocX128(frm);
	a->fx=frm->stack[op->t0+0].f;
	a->fy=frm->stack[op->t0+1].f;
	a->fz=frm->stack[op->t0+2].f;
	a->fw=frm->stack[op->t0+3].f;
	frm->stack[op->t0].p=a;
}

void BSVM2_Op_MKX2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=BSVM2_FrameAllocX128(frm);
	a->dx=frm->stack[op->t0].d;
	a->dy=frm->stack[op->t1].d;
	frm->stack[op->t0].p=a;
}

void BSVM2_Op_LDX2LA_X2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].l=a->la;
	BSVM2_FrameFreeX128(frm, a);
}

void BSVM2_Op_LDX2LB_X2L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].l=a->lb;
	BSVM2_FrameFreeX128(frm, a);
}

void BSVM2_Op_LDX2DA_X2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].d=a->dx;
	BSVM2_FrameFreeX128(frm, a);
}

void BSVM2_Op_LDX2DB_X2D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].d=a->dy;
	BSVM2_FrameFreeX128(frm, a);
}

void BSVM2_Op_LDX4IA_X4I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].i=a->ia;
	BSVM2_FrameFreeX128(frm, a);	}
void BSVM2_Op_LDX4IB_X4I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].i=a->ib;
	BSVM2_FrameFreeX128(frm, a);	}
void BSVM2_Op_LDX4IC_X4I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].i=a->ic;
	BSVM2_FrameFreeX128(frm, a);	}
void BSVM2_Op_LDX4ID_X4I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].i=a->id;
	BSVM2_FrameFreeX128(frm, a);	}

void BSVM2_Op_LDX4FA_X4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].f=a->fx;
	BSVM2_FrameFreeX128(frm, a);	}
void BSVM2_Op_LDX4FB_X4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].f=a->fy;
	BSVM2_FrameFreeX128(frm, a);	}
void BSVM2_Op_LDX4FC_X4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].f=a->fz;
	BSVM2_FrameFreeX128(frm, a);	}
void BSVM2_Op_LDX4FD_X4F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ValX128 *a;
	a=frm->stack[op->t0].p;
	frm->stack[op->t0].f=a->fw;
	BSVM2_FrameFreeX128(frm, a);	}
