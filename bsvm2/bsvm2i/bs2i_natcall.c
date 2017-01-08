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


char *bsvm2_natcall_nhashn[4096];
void *bsvm2_natcall_nhashv[4096];

BS2VM_API void *BSVM2_NatCall_GetProcAddress(char *name)
{
	char *s;
	void *p;
	int i, j, h;

	if(!name)
		return(NULL);

	h=BGBDT_MM_QHashName(name);
	for(i=0; i<256; i++)
	{
		j=(h>>12)&4095;
		s=bsvm2_natcall_nhashn[j];
		if(!s)
			break;
		if(!strcmp(s, name))
		{
			p=bsvm2_natcall_nhashv[j];
			return(p);
		}
		h=h*4093+1;
	}

	if(i<256)
	{
		p=BIPRO_LookupLabel(name);

		printf("BSVM2_NatCall_GetProcAddress: %s %p\n", name, p);

		bsvm2_natcall_nhashn[j]=bgbdt_mm_strdup(name);
		bsvm2_natcall_nhashv[j]=p;
		return(p);
	}

	p=BIPRO_LookupLabel(name);
	
	printf("BSVM2_NatCall_GetProcAddress: %s %p\n", name, p);
	
	return(p);

//	return(NULL);
}

BS2VM_API int BSVM2_NatCall_RegisterProcAddress(char *name, void *addr)
{
	BIPRO_AddSym(name, addr);
	return(0);
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

#if 0
BSCM2_NC_CALL1(I, s32, i)
BSCM2_NC_CALL1(L, s64, l)
BSCM2_NC_CALL1(F, f32, f)
BSCM2_NC_CALL1(D, f64, d)
BSCM2_NC_CALL1(P, void*, p)
BSCM2_NC_CALL1(A, dtVal, a)
#endif

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

#if 0
BSCM2_NC_CALL2B(I, s32, i)
BSCM2_NC_CALL2B(L, s64, l)
BSCM2_NC_CALL2B(F, f32, f)
BSCM2_NC_CALL2B(D, f64, d)
BSCM2_NC_CALL2B(P, void*, p)
BSCM2_NC_CALL2B(A, dtVal, a)
#endif

#if 0
BSVM2_Trace *BSVM2_TrOp_NatCallGFx(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_NatCall_Call_N(op->v.p, op->i1,
		frm->stack+op->t0, frm->stack+op->t1);
	return(op->nexttrace);
}
#endif

#define BSCM2_NC_CASTCALL0T(N, RT, RP)				\
	case (N): rv->RP=((RT(*)(void))fcn)(); break;

#define BSCM2_NC_CASTCALL1T(N, RT, RP, T0, P0)				\
	case (N): rv->RP=((RT(*)(T0))fcn)(av[0].P0); break;

#define BSCM2_NC_CASTCALL2T(N, RT, RP, T0, P0, T1, P1)		\
	case (N): rv->RP=((RT(*)(T0, T1))fcn)(av[0].P0, av[1].P1); break;

#define BSCM2_NC_CASTCALL3T(N, RT, RP, T0, P0, T1, P1, T2, P2)		\
	case (N): rv->RP=((RT(*)(T0, T1, T2))fcn)	\
		(av[0].P0, av[1].P1, av[2].P2); break;

#define BSCM2_NC_CASTCALL4T(N, RT, RP, T0, P0, T1, P1, T2, P2, T3, P3)	\
	case (N): rv->RP=((RT(*)(T0, T1, T2, T3))fcn)	\
		(av[0].P0, av[1].P1, av[2].P2, av[3].P3); break;


#define BSCM2_NC_CASTCALL0A(N)					\
	BSCM2_NC_CASTCALL0T(((N)*6)+0, s32, i)		\
	BSCM2_NC_CASTCALL0T(((N)*6)+1, s64, l)		\
	BSCM2_NC_CASTCALL0T(((N)*6)+2, f32, f)		\
	BSCM2_NC_CASTCALL0T(((N)*6)+3, f64, d)		\
	BSCM2_NC_CASTCALL0T(((N)*6)+4, dtVal, a)	\
	BSCM2_NC_CASTCALL0T(((N)*6)+5, void*, p)


#define BSCM2_NC_CASTCALL1A(N, T0, P0)						\
	BSCM2_NC_CASTCALL1T(((N)*6)+0, s32, i, T0, P0)			\
	BSCM2_NC_CASTCALL1T(((N)*6)+1, s64, l, T0, P0)			\
	BSCM2_NC_CASTCALL1T(((N)*6)+2, f32, f, T0, P0)			\
	BSCM2_NC_CASTCALL1T(((N)*6)+3, f64, d, T0, P0)			\
	BSCM2_NC_CASTCALL1T(((N)*6)+4, dtVal, a, T0, P0)		\
	BSCM2_NC_CASTCALL1T(((N)*6)+5, void*, p, T0, P0)

#define BSCM2_NC_CASTCALL1B(N)								\
	BSCM2_NC_CASTCALL1A(((N)*6)+0, s32, i)					\
	BSCM2_NC_CASTCALL1A(((N)*6)+1, s64, l)					\
	BSCM2_NC_CASTCALL1A(((N)*6)+2, f32, f)					\
	BSCM2_NC_CASTCALL1A(((N)*6)+3, f64, d)					\
	BSCM2_NC_CASTCALL1A(((N)*6)+4, dtVal, a)				\
	BSCM2_NC_CASTCALL1A(((N)*6)+5, void*, p)


#define BSCM2_NC_CASTCALL2A(N, T0, P0, T1, P1)					\
	BSCM2_NC_CASTCALL2T(((N)*6)+0, s32, i, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T(((N)*6)+1, s64, l, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T(((N)*6)+2, f32, f, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T(((N)*6)+3, f64, d, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL2T(((N)*6)+4, dtVal, a, T0, P0, T1, P1)	\
	BSCM2_NC_CASTCALL2T(((N)*6)+5, void*, p, T0, P0, T1, P1)

#if 0
#define BSCM2_NC_CASTCALL2B(N, T0, P0)						\
	BSCM2_NC_CASTCALL2A(((N)*6)+0, s32, i, T0, P0)			\
	BSCM2_NC_CASTCALL2A(((N)*6)+1, s64, l, T0, P0)			\
	BSCM2_NC_CASTCALL2A(((N)*6)+2, f32, f, T0, P0)			\
	BSCM2_NC_CASTCALL2A(((N)*6)+3, f64, d, T0, P0)			\
	BSCM2_NC_CASTCALL2A(((N)*6)+4, dtVal, a, T0, P0)		\
	BSCM2_NC_CASTCALL2A(((N)*6)+5, void*, p, T0, P0)
#endif

#if 1
#define BSCM2_NC_CASTCALL2B(N, T0, P0)						\
	BSCM2_NC_CASTCALL2A(((N)*6)+0, T0, P0, s32, i)			\
	BSCM2_NC_CASTCALL2A(((N)*6)+1, T0, P0, s64, l)			\
	BSCM2_NC_CASTCALL2A(((N)*6)+2, T0, P0, f32, f)			\
	BSCM2_NC_CASTCALL2A(((N)*6)+3, T0, P0, f64, d)			\
	BSCM2_NC_CASTCALL2A(((N)*6)+4, T0, P0, dtVal, a)		\
	BSCM2_NC_CASTCALL2A(((N)*6)+5, T0, P0, void*, p)
#endif

#define BSCM2_NC_CASTCALL2C(N)						\
	BSCM2_NC_CASTCALL2B(((N)*6)+0, s32, i)			\
	BSCM2_NC_CASTCALL2B(((N)*6)+1, s64, l)			\
	BSCM2_NC_CASTCALL2B(((N)*6)+2, f32, f)			\
	BSCM2_NC_CASTCALL2B(((N)*6)+3, f64, d)			\
	BSCM2_NC_CASTCALL2B(((N)*6)+4, dtVal, a)		\
	BSCM2_NC_CASTCALL2B(((N)*6)+5, void*, p)


#define BSCM2_NC_CASTCALL3A(N, T0, P0, T1, P1, T2, P2)					\
	BSCM2_NC_CASTCALL3T(((N)*6)+0, s32, i, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T(((N)*6)+1, s64, l, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T(((N)*6)+2, f32, f, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T(((N)*6)+3, f64, d, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL3T(((N)*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2)	\
	BSCM2_NC_CASTCALL3T(((N)*6)+5, void*, p, T0, P0, T1, P1, T2, P2)

#if 0
#define BSCM2_NC_CASTCALL3B(N, T0, P0, T1, P1)					\
	BSCM2_NC_CASTCALL3A(((N)*6)+0, s32, i, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A(((N)*6)+1, s64, l, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A(((N)*6)+2, f32, f, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A(((N)*6)+3, f64, d, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL3A(((N)*6)+4, dtVal, a, T0, P0, T1, P1)	\
	BSCM2_NC_CASTCALL3A(((N)*6)+5, void*, p, T0, P0, T1, P1)

#define BSCM2_NC_CASTCALL3C(N, T0, P0)						\
	BSCM2_NC_CASTCALL3B(((N)*6)+0, s32, i, T0, P0)			\
	BSCM2_NC_CASTCALL3B(((N)*6)+1, s64, l, T0, P0)			\
	BSCM2_NC_CASTCALL3B(((N)*6)+2, f32, f, T0, P0)			\
	BSCM2_NC_CASTCALL3B(((N)*6)+3, f64, d, T0, P0)			\
	BSCM2_NC_CASTCALL3B(((N)*6)+4, dtVal, a, T0, P0)		\
	BSCM2_NC_CASTCALL3B(((N)*6)+5, void*, p, T0, P0)
#endif

#if 1
#define BSCM2_NC_CASTCALL3B(N, T0, P0, T1, P1)					\
	BSCM2_NC_CASTCALL3A(((N)*6)+0, T0, P0, T1, P1, s32, i)		\
	BSCM2_NC_CASTCALL3A(((N)*6)+1, T0, P0, T1, P1, s64, l)		\
	BSCM2_NC_CASTCALL3A(((N)*6)+2, T0, P0, T1, P1, f32, f)		\
	BSCM2_NC_CASTCALL3A(((N)*6)+3, T0, P0, T1, P1, f64, d)		\
	BSCM2_NC_CASTCALL3A(((N)*6)+4, T0, P0, T1, P1, dtVal, a)	\
	BSCM2_NC_CASTCALL3A(((N)*6)+5, T0, P0, T1, P1, void*, p)

#define BSCM2_NC_CASTCALL3C(N, T0, P0)						\
	BSCM2_NC_CASTCALL3B(((N)*6)+0, T0, P0, s32, i)			\
	BSCM2_NC_CASTCALL3B(((N)*6)+1, T0, P0, s64, l)			\
	BSCM2_NC_CASTCALL3B(((N)*6)+2, T0, P0, f32, f)			\
	BSCM2_NC_CASTCALL3B(((N)*6)+3, T0, P0, f64, d)			\
	BSCM2_NC_CASTCALL3B(((N)*6)+4, T0, P0, dtVal, a)		\
	BSCM2_NC_CASTCALL3B(((N)*6)+5, T0, P0, void*, p)
#endif

#define BSCM2_NC_CASTCALL3D(N)						\
	BSCM2_NC_CASTCALL3C(((N)*6)+0, s32, i)			\
	BSCM2_NC_CASTCALL3C(((N)*6)+1, s64, l)			\
	BSCM2_NC_CASTCALL3C(((N)*6)+2, f32, f)			\
	BSCM2_NC_CASTCALL3C(((N)*6)+3, f64, d)			\
	BSCM2_NC_CASTCALL3C(((N)*6)+4, dtVal, a)		\
	BSCM2_NC_CASTCALL3C(((N)*6)+5, void*, p)


#define BSCM2_NC_CASTCALL4A(N, T0, P0, T1, P1, T2, P2, T3, P3)				\
	BSCM2_NC_CASTCALL4T(((N)*6)+0, s32, i, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T(((N)*6)+1, s64, l, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T(((N)*6)+2, f32, f, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T(((N)*6)+3, f64, d, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T(((N)*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSCM2_NC_CASTCALL4T(((N)*6)+5, void*, p, T0, P0, T1, P1, T2, P2, T3, P3)

#if 0
#define BSCM2_NC_CASTCALL4B(N, T0, P0, T1, P1, T2, P2)					\
	BSCM2_NC_CASTCALL4A(((N)*6)+0, s32, i, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A(((N)*6)+1, s64, l, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A(((N)*6)+2, f32, f, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A(((N)*6)+3, f64, d, T0, P0, T1, P1, T2, P2)		\
	BSCM2_NC_CASTCALL4A(((N)*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2)	\
	BSCM2_NC_CASTCALL4A(((N)*6)+5, void*, p, T0, P0, T1, P1, T2, P2)

#define BSCM2_NC_CASTCALL4C(N, T0, P0, T1, P1)						\
	BSCM2_NC_CASTCALL4B(((N)*6)+0, s32, i, T0, P0, T1, P1)			\
	BSCM2_NC_CASTCALL4B(((N)*6)+1, s64, l, T0, P0, T1, P1)			\
	BSCM2_NC_CASTCALL4B(((N)*6)+2, f32, f, T0, P0, T1, P1)			\
	BSCM2_NC_CASTCALL4B(((N)*6)+3, f64, d, T0, P0, T1, P1)			\
	BSCM2_NC_CASTCALL4B(((N)*6)+4, dtVal, a, T0, P0, T1, P1)		\
	BSCM2_NC_CASTCALL4B(((N)*6)+5, void*, p, T0, P0, T1, P1)

#define BSCM2_NC_CASTCALL4D(N, T0, P0)						\
	BSCM2_NC_CASTCALL4C(((N)*6)+0, s32, i, T0, P0)			\
	BSCM2_NC_CASTCALL4C(((N)*6)+1, s64, l, T0, P0)			\
	BSCM2_NC_CASTCALL4C(((N)*6)+2, f32, f, T0, P0)			\
	BSCM2_NC_CASTCALL4C(((N)*6)+3, f64, d, T0, P0)			\
	BSCM2_NC_CASTCALL4C(((N)*6)+4, dtVal, a, T0, P0)		\
	BSCM2_NC_CASTCALL4C(((N)*6)+5, void*, p, T0, P0)
#endif

#if 1
#define BSCM2_NC_CASTCALL4B(N, T0, P0, T1, P1, T2, P2)					\
	BSCM2_NC_CASTCALL4A(((N)*6)+0, T0, P0, T1, P1, T2, P2, s32, i)		\
	BSCM2_NC_CASTCALL4A(((N)*6)+1, T0, P0, T1, P1, T2, P2, s64, l)		\
	BSCM2_NC_CASTCALL4A(((N)*6)+2, T0, P0, T1, P1, T2, P2, f32, f)		\
	BSCM2_NC_CASTCALL4A(((N)*6)+3, T0, P0, T1, P1, T2, P2, f64, d)		\
	BSCM2_NC_CASTCALL4A(((N)*6)+4, T0, P0, T1, P1, T2, P2, dtVal, a)	\
	BSCM2_NC_CASTCALL4A(((N)*6)+5, T0, P0, T1, P1, T2, P2, void*, p)

#define BSCM2_NC_CASTCALL4C(N, T0, P0, T1, P1)						\
	BSCM2_NC_CASTCALL4B(((N)*6)+0, T0, P0, T1, P1, s32, i)			\
	BSCM2_NC_CASTCALL4B(((N)*6)+1, T0, P0, T1, P1, s64, l)			\
	BSCM2_NC_CASTCALL4B(((N)*6)+2, T0, P0, T1, P1, f32, f)			\
	BSCM2_NC_CASTCALL4B(((N)*6)+3, T0, P0, T1, P1, f64, d)			\
	BSCM2_NC_CASTCALL4B(((N)*6)+4, T0, P0, T1, P1, dtVal, a)		\
	BSCM2_NC_CASTCALL4B(((N)*6)+5, T0, P0, T1, P1, void*, p)

#define BSCM2_NC_CASTCALL4D(N, T0, P0)						\
	BSCM2_NC_CASTCALL4C(((N)*6)+0, T0, P0, s32, i)			\
	BSCM2_NC_CASTCALL4C(((N)*6)+1, T0, P0, s64, l)			\
	BSCM2_NC_CASTCALL4C(((N)*6)+2, T0, P0, f32, f)			\
	BSCM2_NC_CASTCALL4C(((N)*6)+3, T0, P0, f64, d)			\
	BSCM2_NC_CASTCALL4C(((N)*6)+4, T0, P0, dtVal, a)		\
	BSCM2_NC_CASTCALL4C(((N)*6)+5, T0, P0, void*, p)
#endif


#define BSCM2_NC_CASTCALL4E(N)						\
	BSCM2_NC_CASTCALL4D(((N)*6)+0, s32, i)			\
	BSCM2_NC_CASTCALL4D(((N)*6)+1, s64, l)			\
	BSCM2_NC_CASTCALL4D(((N)*6)+2, f32, f)			\
	BSCM2_NC_CASTCALL4D(((N)*6)+3, f64, d)			\
	BSCM2_NC_CASTCALL4D(((N)*6)+4, dtVal, a)		\
	BSCM2_NC_CASTCALL4D(((N)*6)+5, void*, p)


void BSVM2_NatCall_Call_N(void *fcn, int nc,
	BSVM2_Value *rv, BSVM2_Value *av)
{
	switch(nc)
	{
		BSCM2_NC_CASTCALL0A(1)
		BSCM2_NC_CASTCALL1B(1)
		BSCM2_NC_CASTCALL2C(1)
		BSCM2_NC_CASTCALL3D(1)
		BSCM2_NC_CASTCALL4E(1)
		default:
			BSVM2_DBGTRAP
			break;
	}
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_NatCallG0(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	static int rec=0;
	
	if(op->i0!=99)
	{
		printf("BSVM2_TrOp_NatCallG0: %p ix=%d\n", op->v.p, op->i1);
		op->i0=99;
	}

	if(rec>=16)
	{
		printf("BSVM2_TrOp_NatCallG0: Recursion Limit\n");
		return(op->nexttrace);
	}
	
	if(rec>0)
	{
		printf("BSVM2_TrOp_NatCallG0: Rec %d\n", rec);
	}
	
	rec++;
	BSVM2_NatCall_Call_N(op->v.p, op->i1,
		frm->stack+op->t0, frm->stack+op->t1);
	rec--;
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

char *BSVM2_NatCall_SigGetRet(char *sig)
{
	char *s;

	if(*sig=='(')
	{
		s=sig+1;

		while(*s && (*s!=')'))
			{ s=BSVM2_NatCall_SigNext(s); }
		if(*s==')')
			return(s+1);
		return(NULL);
	}
	return(NULL);
}

int BSVM2_NatCall_GetSigOpZ(char *sig)
{
	int i;

	switch(*sig)
	{
	case 'a':	case 'b':	case 'c': case 'h':
	case 'i':		case 'k':
	case 's':	case 't':	case 'w':
		i=BSVM2_OPZ_INT; break;
	case 'j':
		i=BSVM2_OPZ_UINT; break;
	case 'l':	case 'x':
		i=BSVM2_OPZ_LONG; break;
	case 'm':	case 'y':
		i=BSVM2_OPZ_ULONG; break;
	case 'f':
		i=BSVM2_OPZ_FLOAT; break;
	case 'd':	case 'e':
		i=BSVM2_OPZ_DOUBLE; break;

	case 'g':	case 'n':	case 'o':
		i=BS2CC_TYZ_INT128; break;

	case 'p':	case 'q':	case 'r':
		i=BSVM2_OPZ_ADDRESS; break;

	case 'C':
		if((sig[1]=='b') || (sig[1]=='c') ||
			(sig[1]=='d') || (sig[1]=='e') ||
			(sig[1]=='h') ||
			(sig[1]=='q'))
				{ i=BS2CC_TYZ_VEC4F; break; }
		i=BSVM2_OPZ_ADDRESS; break;

	case 'Q':
	case 'X':	case 'L':
		i=BSVM2_OPZ_ADDRESS; break;

	case 'v':
		i=BSVM2_OPZ_VOID; break;
	case 'z':
		i=BSVM2_OPZ_ADDR; break;

	case 'P':
		i=BSVM2_OPZ_ADDR; break;
//		i=5; break;

	case 'A':
		i=BSVM2_OPZ_ADDR; break;

	default:
		i=-1; break;
	}
	
	return(i);
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

	case 'C':
//		if(sig[1]=='s')
//			{ i=-1; break; }
		if((sig[1]=='b') || (sig[1]=='c') ||
			(sig[1]=='d') || (sig[1]=='e') ||
			(sig[1]=='h') ||
			(sig[1]=='q'))
				{ i=5; break; }

		i=4; break;

	case 'Q':
	case 'X':	case 'L':
		i=4; break;

	case 'v':
		i=0; break;
	case 'z':
		i=-1; break;

	case 'P':
		i=-1; break;
//		i=5; break;

	case 'R':
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


int BSVM2_NatCall_GetSigIndexG1(char *sig,
	BSVM2_Value *iav, BSVM2_Value *oav)
{
	BSVM2_Value *avs, *avt;
	dtVal va, v0;
	char *s;
	int i, j, k, l;
	
	if(*sig!='(')
		return(-1);
	
	s=sig; i=1;
	avs=iav;
	avt=oav;
	if(*s=='(')s++;
	while(*s && (*s!=')'))
	{
		if(*s=='z')break;

		if(*s=='P')
		{
			if(s[1]=='c')
			{
				s+=2;
				avt->p=BGBDT_TagStr_GetUtf8(avs->a);
				avt++; avs++;
				i=i*6+5;
				continue;
			}

			s=BSVM2_NatCall_SigNext(s);
			avt->p=dtvUnwrapPtr(avs->a);
			avt++; avs++;
			i=i*6+5;
			continue;
		}
		
		if(*s=='C')
		{
#if 0
			if(s[1]=='s')
			{
				s+=2;
				avt->p=BGBDT_TagStr_GetUtf8(avs->a);
				avt++; avs++;
				i=i*6+5;
				continue;
			}
#endif
		}
		
		*avt++=*avs++;
		j=BSVM2_NatCall_GetSigBType(s);
		if(j<0)
			return(-1);
		i=i*6+j;
		s=BSVM2_NatCall_SigNext(s);
	}
	if(*s=='z')
	{
		s++;
		va=avs->a;
		if(dtvIsArrayP(va))
		{
			l=dtvArrayGetSize(va);
			for(j=0; j<l; j++)
			{
				v0=dtvArrayGetIndexDtVal(va, j);
				
				if(dtvIsSmallIntP(v0))
				{	avt->i=dtvUnwrapInt(v0);
					avt++; i=i*6+0; continue;	}
				if(dtvIsSmallLongP(v0))
				{	avt->l=dtvUnwrapLong(v0);
					avt++; i=i*6+1; continue;	}
				if(dtvIsSmallDoubleP(v0))
				{	avt->d=dtvUnwrapDouble(v0);
					avt++; i=i*6+3; continue;	}

				avt->a=v0;
				avt++;
				i=i*6+4;
				continue;
			}
		}
	}
	if(*s==')')
	{
		s++;
		
		if(*s=='P')
		{
			i=i*6+5;
		}else
		{
			j=BSVM2_NatCall_GetSigBType(s);
			if(j<0)
				return(-1);
			i=i*6+j;
		}
	}
	return(i);
}

BS2VM_API BSVM2_Trace *BSVM2_TrOp_NatCallG1(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Value argt[64];
	BSVM2_ImageGlobal *vi;
	void *p;
	int ic;

	vi=op->v.p;
	p=BSVM2_NatCall_GetProcAddress(vi->name);

	ic=BSVM2_NatCall_GetSigIndexG1(vi->sig,
		frm->stack+op->t1, argt);

	BSVM2_NatCall_Call_N(p, ic,
		frm->stack+op->t0, argt);
	return(op->nexttrace);
}


dtVal BSVM2_Sig_GetSigPtrDtVal(void *ptr, char *sig)
{
	dtVal v;
	int i;

	switch(*sig)
	{
	case 'a':	case 'c':
		v=dtvWrapInt(*(sbyte *)ptr); break;
	case 'b':	case 'h':
		v=dtvWrapInt(*(byte *)ptr); break;
	case 'k':
	case 's':
		v=dtvWrapInt(*(s16 *)ptr); break;
	case 't':	case 'w':
		v=dtvWrapInt(*(u16 *)ptr); break;

	case 'i':
		v=dtvWrapInt(*(s32 *)ptr); break;
	case 'j':
		v=dtvWrapUInt(*(u32 *)ptr); break;
	case 'l':	case 'm':
		v=dtvWrapLong(*(long *)ptr); break;
	case 'x':	case 'y':
		v=dtvWrapLong(*(s64 *)ptr); break;
	case 'f':
		v=dtvWrapFloat(*(f32 *)ptr); break;
	case 'd':	case 'e':
		v=dtvWrapDouble(*(f64 *)ptr); break;

	case 'g':	case 'n':	case 'o':

	case 'p':	case 'q':	case 'r':
		v=*(dtVal *)ptr; break;

	case 'C':
		if((sig[1]=='b') || (sig[1]=='c') ||
			(sig[1]=='d') || (sig[1]=='e') ||
			(sig[1]=='h') ||
			(sig[1]=='q'))
		{
			i=BS2CC_TYZ_VEC4F; break;
		}
		v=*(dtVal *)ptr; break;

	case 'Q':
	case 'X':	case 'L':
		v=*(dtVal *)ptr; break;

	case 'v':
		i=BSVM2_OPZ_VOID; break;
	case 'z':
		i=BSVM2_OPZ_ADDR; break;

	case 'P':
		v=dtvWrapPtr(*(void **)ptr); break;
//		i=BSVM2_OPZ_ADDR;
		break;
//		i=5; break;

	default:
		v=DTV_UNDEFINED;
		break;
	}
	
	return(v);
}

dtVal BSVM2_Sig_SetSigPtrDtVal(void *ptr, char *sig, dtVal v)
{
	int i;

	switch(*sig)
	{
	case 'a':	case 'c':
		*(sbyte *)ptr=dtvUnwrapInt(v); break;
	case 'b':	case 'h':
		*(byte *)ptr=dtvUnwrapInt(v); break;
	case 'k':
	case 's':
		*(s16 *)ptr=dtvUnwrapInt(v); break;
	case 't':	case 'w':
		*(u16 *)ptr=dtvUnwrapInt(v); break;

	case 'i':
		*(s32 *)ptr=dtvUnwrapInt(v); break;
	case 'j':
		*(u32 *)ptr=dtvUnwrapInt(v); break;
	case 'l':	case 'm':
		*(long *)ptr=dtvUnwrapLong(v); break;
	case 'x':	case 'y':
		*(s64 *)ptr=dtvUnwrapLong(v); break;
	case 'f':
		*(f32 *)ptr=dtvUnwrapFloat(v); break;
	case 'd':	case 'e':
		*(f64 *)ptr=dtvUnwrapDouble(v); break;

	case 'g':	case 'n':	case 'o':

	case 'p':	case 'q':	case 'r':
		*(dtVal *)ptr=v; break;

	case 'C':
		if((sig[1]=='b') || (sig[1]=='c') ||
			(sig[1]=='d') || (sig[1]=='e') ||
			(sig[1]=='h') ||
			(sig[1]=='q'))
		{
			i=BS2CC_TYZ_VEC4F; break;
		}
		*(dtVal *)ptr=v; break;

	case 'Q':
	case 'X':	case 'L':
		*(dtVal *)ptr=v; break;

	case 'v':
		break;
	case 'z':
		break;

	case 'P':
		*(void **)ptr=dtvUnwrapPtr(v); break;
		break;

	default:
		break;
	}
	
	return(v);
}

BSVM2_Trace *BSVM2_TrOp_NatCallFail(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	frm->ctx->status=BSVM2_EXS_CALLUNDEF;
	return(NULL);
}
