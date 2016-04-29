#include <bteifgl.h>

void *BSVM2_NatCall_GetProcAddress(char *name)
{
	if(!name)
		return(NULL);

	return(NULL);
}

void BSVM2_NatCall_Call_0_V(void *fptr,
	BSVM2_Value *rv, BSVM2_Value *av)
{	void (*fcn)(void); fcn=fptr;	fcn();			}
void BSVM2_NatCall_Call_0_I(void *fptr,
	BSVM2_Value *rv, BSVM2_Value *av)
{	int (*fcn)(void); fcn=fptr;	rv->i=fcn();	}
void BSVM2_NatCall_Call_0_L(void *fptr,
	BSVM2_Value *rv, BSVM2_Value *av)
{	s64 (*fcn)(void); fcn=fptr;		rv->l=fcn();	}
void BSVM2_NatCall_Call_0_F(void *fptr,
	BSVM2_Value *rv, BSVM2_Value *av)
{	float (*fcn)(void); fcn=fptr;	rv->f=fcn();	}
void BSVM2_NatCall_Call_0_D(void *fptr,
	BSVM2_Value *rv, BSVM2_Value *av)
{	double (*fcn)(void); fcn=fptr;	rv->d=fcn();	}
void BSVM2_NatCall_Call_0_P(void *fptr,
	BSVM2_Value *rv, BSVM2_Value *av)
{	void *(*fcn)(void); fcn=fptr;	rv->p=fcn();	}
void BSVM2_NatCall_Call_0_A(void *fptr,
	BSVM2_Value *rv, BSVM2_Value *av)
{	dtVal (*fcn)(void); fcn=fptr;	rv->a=fcn();	}

#define BSCM2_NC_CALL1V(N, T0, P0)						\
	static void BSVM2_NatCall_Call##N(void *fptr,		\
		BSVM2_Value *rv, BSVM2_Value *av)				\
	{	void (*fcn)(T0); fcn=fptr;	fcn(av[0].P0);	}

#define BSCM2_NC_CALL1T(N, RT, RP, T0, P0)				\
	static void BSVM2_NatCall_Call##N(void *fptr,		\
		BSVM2_Value *rv, BSVM2_Value *av)				\
	{	RT (*fcn)(T0); fcn=fptr;	rv->RP=fcn(av[0].P0);	}

#define BSCM2_NC_CALL1(N, T0, P0)			\
	BSCM2_NC_CALL1V(N##V, T0, P0)			\
	BSCM2_NC_CALL1T(N##I, int, i, T0, P0)	\
	BSCM2_NC_CALL1T(N##L, s64, l, T0, P0)	\
	BSCM2_NC_CALL1T(N##F, f32, f, T0, P0)	\
	BSCM2_NC_CALL1T(N##D, f64, d, T0, P0)	\
	BSCM2_NC_CALL1T(N##P, void*, p, T0, P0)	\
	BSCM2_NC_CALL1T(N##A, dtVal, a, T0, P0)

BSCM2_NC_CALL1(I, s32, i)
BSCM2_NC_CALL1(L, s64, l)
BSCM2_NC_CALL1(F, f32, f)
BSCM2_NC_CALL1(D, f64, d)
BSCM2_NC_CALL1(P, void*, p)
BSCM2_NC_CALL1(A, dtVal, a)

#define BSCM2_NC_CALL2V(N, T0, P0, T1, P1)				\
	static void BSVM2_NatCall_Call##N(void *fptr,		\
		BSVM2_Value *rv, BSVM2_Value *av)				\
	{	void (*fcn)(T0, T1); fcn=fptr;	fcn(av[0].P0, av[1].P1);	}

#define BSCM2_NC_CALL2T(N, RT, RP, T0, P0, T1, P1)		\
	static void BSVM2_NatCall_Call##N(void *fptr,		\
		BSVM2_Value *rv, BSVM2_Value *av)				\
	{	RT (*fcn)(T0, T1); fcn=fptr;	rv->RP=fcn(av[0].P0, av[1].P1);	}

#define BSCM2_NC_CALL2A(N, T0, P0, T1, P1)			\
	BSCM2_NC_CALL2V(N##V, T0, P0, T1, P1)			\
	BSCM2_NC_CALL2T(N##I, int, i, T0, P0, T1, P1)	\
	BSCM2_NC_CALL2T(N##L, s64, l, T0, P0, T1, P1)	\
	BSCM2_NC_CALL2T(N##F, f32, f, T0, P0, T1, P1)	\
	BSCM2_NC_CALL2T(N##D, f64, d, T0, P0, T1, P1)	\
	BSCM2_NC_CALL2T(N##P, void*, p, T0, P0, T1, P1)	\
	BSCM2_NC_CALL2T(N##A, dtVal, a, T0, P0, T1, P1)

#define BSCM2_NC_CALL2B(N, T0, P0)				\
	BSCM2_NC_CALL2A(N##I, T0, P0, s32, i)		\
	BSCM2_NC_CALL2A(N##L, T0, P0, s64, l)		\
	BSCM2_NC_CALL2A(N##F, T0, P0, f32, f)		\
	BSCM2_NC_CALL2A(N##D, T0, P0, f64, d)		\
	BSCM2_NC_CALL2A(N##P, T0, P0, void*, p)		\
	BSCM2_NC_CALL2A(N##A, T0, P0, dtVal, a)

BSCM2_NC_CALL2B(I, s32, i)
BSCM2_NC_CALL2B(L, s64, l)
BSCM2_NC_CALL2B(F, f32, f)
BSCM2_NC_CALL2B(D, f64, d)
BSCM2_NC_CALL2B(P, void*, p)
BSCM2_NC_CALL2B(A, dtVal, a)

#if 0
BSVM2_Trace *BSVM2_TrOp_NatCallGFx(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_NatCall_Call_N(op->v.p, op->i1,
		frm->stack+op->t0, frm->stack+op->t1);
	return(op->nexttrace);
}
#endif

#define BSCM2_NC_CASTCALL0T(N, RT, RP)				\
	case N: rv->RP=((RT(*)(void))fcn)(); break;

#define BSCM2_NC_CASTCALL1T(N, RT, RP, T0, P0)				\
	case N: rv->RP=((RT(*)(T0))fcn)(av[0].P0); break;

#define BSCM2_NC_CASTCALL2T(N, RT, RP, T0, P0, T1, P1)		\
	case N: rv->RP=((RT(*)(T0, T1))fcn)(av[0].P0, av[1].P1); break;

#define BSCM2_NC_CASTCALL3T(N, RT, RP, T0, P0, T1, P1, T2, P2)		\
	case N: rv->RP=((RT(*)(T0, T1, T2))fcn)	\
		(av[0].P0, av[1].P1, av[2].P2); break;

#define BSCM2_NC_CASTCALL4T(N, RT, RP, T0, P0, T1, P1, T2, P2, T3, P3)	\
	case N: rv->RP=((RT(*)(T0, T1, T2, T3))fcn)	\
		(av[0].P0, av[1].P1, av[2].P2, av[3].P3); break;


#define BSCM2_NC_CASTCALL0A(N)						\
	BSCM2_NC_CASTCALL0T((N*6)+0, s32, i)		\
	BSCM2_NC_CASTCALL0T((N*6)+1, s64, l)		\
	BSCM2_NC_CASTCALL0T((N*6)+2, f32, f)		\
	BSCM2_NC_CASTCALL0T((N*6)+3, f64, d)		\
	BSCM2_NC_CASTCALL0T((N*6)+4, dtVal, a)		\
	BSCM2_NC_CASTCALL0T((N*6)+5, void*, p)


#define BSCM2_NC_CASTCALL1A(N, T0, P0)				\
	BSCM2_NC_CASTCALL1T((N*6)+0, s32, i, T0, P0)		\
	BSCM2_NC_CASTCALL1T((N*6)+1, s64, l, T0, P0)		\
	BSCM2_NC_CASTCALL1T((N*6)+2, f32, f, T0, P0)		\
	BSCM2_NC_CASTCALL1T((N*6)+3, f64, d, T0, P0)		\
	BSCM2_NC_CASTCALL1T((N*6)+4, dtVal, a, T0, P0)		\
	BSCM2_NC_CASTCALL1T((N*6)+5, void*, p, T0, P0)

#define BSCM2_NC_CASTCALL1B(N)						\
	BSCM2_NC_CASTCALL1A((N*6)+0, s32, i)		\
	BSCM2_NC_CASTCALL1A((N*6)+1, s64, l)		\
	BSCM2_NC_CASTCALL1A((N*6)+2, f32, f)		\
	BSCM2_NC_CASTCALL1A((N*6)+3, f64, d)		\
	BSCM2_NC_CASTCALL1A((N*6)+4, dtVal, a)		\
	BSCM2_NC_CASTCALL1A((N*6)+5, void*, p)


#define BSCM2_NC_CASTCALL2A(N, T0, P0, T1, P1)					\
	BSCM2_NC_CASTCALL2T((N*6)+0, s32, i, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T((N*6)+1, s64, l, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T((N*6)+2, f32, f, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T((N*6)+3, f64, d, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T((N*6)+4, dtVal, a, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T((N*6)+5, void*, p, T0, P0, T1, P1)

#define BSCM2_NC_CASTCALL2B(N, T0, P0)				\
	BSCM2_NC_CASTCALL2A((N*6)+0, s32, i, T0, P0)		\
	BSCM2_NC_CASTCALL2A((N*6)+1, s64, l, T0, P0)		\
	BSCM2_NC_CASTCALL2A((N*6)+2, f32, f, T0, P0)		\
	BSCM2_NC_CASTCALL2A((N*6)+3, f64, d, T0, P0)		\
	BSCM2_NC_CASTCALL2A((N*6)+4, dtVal, a, T0, P0)		\
	BSCM2_NC_CASTCALL2A((N*6)+5, void*, p, T0, P0)

#define BSCM2_NC_CASTCALL2C(N)						\
	BSCM2_NC_CASTCALL2B((N*6)+0, s32, i)		\
	BSCM2_NC_CASTCALL2B((N*6)+1, s64, l)		\
	BSCM2_NC_CASTCALL2B((N*6)+2, f32, f)		\
	BSCM2_NC_CASTCALL2B((N*6)+3, f64, d)		\
	BSCM2_NC_CASTCALL2B((N*6)+4, dtVal, a)		\
	BSCM2_NC_CASTCALL2B((N*6)+5, void*, p)


#define BSCM2_NC_CASTCALL3A(N, T0, P0, T1, P1, T2, P2)					\
	BSCM2_NC_CASTCALL3T((N*6)+0, s32, i, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T((N*6)+1, s64, l, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T((N*6)+2, f32, f, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T((N*6)+3, f64, d, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T((N*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T((N*6)+5, void*, p, T0, P0, T1, P1, T2, P2)

#define BSCM2_NC_CASTCALL3B(N, T0, P0, T1, P1)					\
	BSCM2_NC_CASTCALL3A((N*6)+0, s32, i, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A((N*6)+1, s64, l, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A((N*6)+2, f32, f, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A((N*6)+3, f64, d, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A((N*6)+4, dtVal, a, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A((N*6)+5, void*, p, T0, P0, T1, P1)

#define BSCM2_NC_CASTCALL3C(N, T0, P0)				\
	BSCM2_NC_CASTCALL3B((N*6)+0, s32, i, T0, P0)		\
	BSCM2_NC_CASTCALL3B((N*6)+1, s64, l, T0, P0)		\
	BSCM2_NC_CASTCALL3B((N*6)+2, f32, f, T0, P0)		\
	BSCM2_NC_CASTCALL3B((N*6)+3, f64, d, T0, P0)		\
	BSCM2_NC_CASTCALL3B((N*6)+4, dtVal, a, T0, P0)		\
	BSCM2_NC_CASTCALL3B((N*6)+5, void*, p, T0, P0)

#define BSCM2_NC_CASTCALL3D(N)						\
	BSCM2_NC_CASTCALL3C((N*6)+0, s32, i)		\
	BSCM2_NC_CASTCALL3C((N*6)+1, s64, l)		\
	BSCM2_NC_CASTCALL3C((N*6)+2, f32, f)		\
	BSCM2_NC_CASTCALL3C((N*6)+3, f64, d)		\
	BSCM2_NC_CASTCALL3C((N*6)+4, dtVal, a)		\
	BSCM2_NC_CASTCALL3C((N*6)+5, void*, p)


#define BSCM2_NC_CASTCALL4A(N, T0, P0, T1, P1, T2, P2, T3, P3)			\
	BSCM2_NC_CASTCALL4T((N*6)+0, s32, i, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T((N*6)+1, s64, l, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T((N*6)+2, f32, f, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T((N*6)+3, f64, d, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T((N*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T((N*6)+5, void*, p, T0, P0, T1, P1, T2, P2, T3, P3)

#define BSCM2_NC_CASTCALL4B(N, T0, P0, T1, P1, T2, P2)					\
	BSCM2_NC_CASTCALL4A((N*6)+0, s32, i, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A((N*6)+1, s64, l, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A((N*6)+2, f32, f, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A((N*6)+3, f64, d, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A((N*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A((N*6)+5, void*, p, T0, P0, T1, P1, T2, P2)

#define BSCM2_NC_CASTCALL4C(N, T0, P0, T1, P1)					\
	BSCM2_NC_CASTCALL4B((N*6)+0, s32, i, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL4B((N*6)+1, s64, l, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL4B((N*6)+2, f32, f, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL4B((N*6)+3, f64, d, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL4B((N*6)+4, dtVal, a, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL4B((N*6)+5, void*, p, T0, P0, T1, P1)

#define BSCM2_NC_CASTCALL4D(N, T0, P0)				\
	BSCM2_NC_CASTCALL4C((N*6)+0, s32, i, T0, P0)		\
	BSCM2_NC_CASTCALL4C((N*6)+1, s64, l, T0, P0)		\
	BSCM2_NC_CASTCALL4C((N*6)+2, f32, f, T0, P0)		\
	BSCM2_NC_CASTCALL4C((N*6)+3, f64, d, T0, P0)		\
	BSCM2_NC_CASTCALL4C((N*6)+4, dtVal, a, T0, P0)		\
	BSCM2_NC_CASTCALL4C((N*6)+5, void*, p, T0, P0)

#define BSCM2_NC_CASTCALL4E(N)						\
	BSCM2_NC_CASTCALL4D((N*6)+0, s32, i)		\
	BSCM2_NC_CASTCALL4D((N*6)+1, s64, l)		\
	BSCM2_NC_CASTCALL4D((N*6)+2, f32, f)		\
	BSCM2_NC_CASTCALL4D((N*6)+3, f64, d)		\
	BSCM2_NC_CASTCALL4D((N*6)+4, dtVal, a)		\
	BSCM2_NC_CASTCALL4D((N*6)+5, void*, p)


void BSVM2_NatCall_Call_N(void *fcn, int nc,
	BSVM2_Value *rv, BSVM2_Value *av)
{
	switch(nc)
	{
		BSCM2_NC_CASTCALL0A(1)
//		BSCM2_NC_CASTCALL1B(1)
//		BSCM2_NC_CASTCALL2C(1)
//		BSCM2_NC_CASTCALL3D(1)
//		BSCM2_NC_CASTCALL4E(1)
	}
}

BSVM2_Trace *BSVM2_TrOp_NatCallG0(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_NatCall_Call_N(op->v.p, op->i1,
		frm->stack+op->t0, frm->stack+op->t1);
	return(op->nexttrace);
}

char *BSVM2_NatCall_SigNext(char *sig)
{
	char *s;
	
	s=sig;
	if((*s>='a') && (*s<='z'))
		return(s+1);

	if((*s=='C') || (*s=='D') || (*s=='R'))
	{
		return(BSVM2_NatCall_SigNext(s+1));
	}

	if(*s=='P')
	{
		while(*s=='P')s++;
		return(BSVM2_NatCall_SigNext(s));
	}
	if(*s=='Q')
	{
		while(*s=='Q')s++;
		return(BSVM2_NatCall_SigNext(s));
	}
	
	if((*s=='X') || (*s=='L') || (*s=='U'))
	{
		s++;
		if((*s>='0') && (*s<='9'))
		{
			while((*s>='0') && (*s<='9'))
				s++;
		}else
		{
			while(*s && (*s!=';'))
				s++;
			if(*s==';')
				s++;
		}
		return(s);
	}

	if(*s=='A')
	{
		s++;
		if((*s>='0') && (*s<='9'))
		{
			while((*s>='0') && (*s<='9'))
				s++;
			return(BSVM2_NatCall_SigNext(s));
		}else
		{
			return(BSVM2_NatCall_SigNext(s+1));
//			while(*s && (*s!=';'))
//				s++;
//			if(*s==';')
//				s++;
		}
//		return(BSVM2_NatCall_SigNext(s));
	}
	return(s);
}

int BSVM2_NatCall_GetSigBType(char *sig)
{
	int i;

	switch(*sig)
	{
	case 'a':	case 'b':	case 'c': case 'h':
	case 'i':	case 'j':	case 'k':
	case 's':	case 't':	case 'w':
		i=0; break;
	case 'l':	case 'm':
	case 'x':	case 'y':
		i=1; break;
	case 'f':	i=2; break;
	case 'd':	case 'e':
		i=3; break;

	case 'g':	case 'n':	case 'o':
	case 'p':	case 'q':	case 'r':
		i=4; break;

	case 'C':	case 'Q':
	case 'X':	case 'L':
		i=4; break;

	case 'v':
		i=0; break;
	case 'z':
		i=-1; break;

	case 'P':
		i=5; break;

	default:
		i=-1; break;
	}
	
	return(i);
}

int BSVM2_NatCall_GetSigIndexG0(char *sig)
{
	char *s;
	int i, j;
	
	if(*sig!='(')
		return(-1);
	
	s=sig; i=1;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		j=BSVM2_NatCall_GetSigBType(s);
		if(j<0)
			return(-1);
		i=i*6+j;
		s=BSVM2_NatCall_SigNext(s);
	}
	if(*s==')')
	{
		s++;
		j=BSVM2_NatCall_GetSigBType(s);
		if(j<0)
			return(-1);
		i=i*6+j;
	}
	return(i);
}
