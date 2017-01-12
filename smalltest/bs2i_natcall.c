#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
#endif

#ifndef __cplusplus

#ifndef BOOL_T
#define BOOL_T
typedef unsigned char bool;
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#endif


#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif

#ifndef F32_T
#define F32_T
typedef float f32;
#endif

#ifndef F64_T
#define F64_T
typedef double f64;
#endif

#define BSVM2_OPZ_INT		0
#define BSVM2_OPZ_LONG		1
#define BSVM2_OPZ_FLOAT		2
#define BSVM2_OPZ_DOUBLE	3
#define BSVM2_OPZ_ADDRESS	4
#define BSVM2_OPZ_UINT		5
#define BSVM2_OPZ_UBYTE		6
#define BSVM2_OPZ_SHORT		7
#define BSVM2_OPZ_SBYTE		8
#define BSVM2_OPZ_USHORT	9
#define BSVM2_OPZ_ULONG		10
#define BSVM2_OPZ_CONST		11
#define BSVM2_OPZ_NLONG		12
#define BSVM2_OPZ_UNLONG	13
#define BSVM2_OPZ_X128		14

#define BSVM2_OPZ_ADDR		4
// #define BSVM2_OPZ_VARIANT	4
#define BSVM2_OPZ_VOID		11

#define BS2CC_TYZ_VEC4F		14


#define BSVM2_EXS_CALLUNDEF -2


#define BS2VM_API
#define BSVM2_DBGTRAP


typedef union BGBDT_TagValue_s		BGBDT_TagValue;
typedef union BGBDT_TagValue_s		dtVal;

typedef union BSVM2_Value_u BSVM2_Value;
typedef union BSVM2_ValX128_u BSVM2_ValX128;

typedef struct BSVM2_Opcode_s BSVM2_Opcode;
typedef struct BSVM2_TailOpcode_s BSVM2_TailOpcode;
typedef struct BSVM2_Trace_s BSVM2_Trace;

typedef struct BSVM2_Frame_s BSVM2_Frame;

typedef struct BSVM2_Context_s BSVM2_Context;

typedef struct BSVM2_ImageGlobal_s BSVM2_ImageGlobal;

union BGBDT_TagValue_s {
struct { u32 lo, hi; };
u64 vi;
void *vp;
};

union BSVM2_Value_u {
struct { s32 i, j; };
struct { u32 ui, uj; };
struct { f32 fx, fy; };
s64 l;
u64 ul;
f32 f;
f64 d;
dtVal a;
void *p;
};

union BSVM2_ValX128_u {
struct { s32 ia, ib, ic, id; };
struct { u32 ua, ub, uc, ud; };
struct { f32 fx, fy, fz, fw; };
struct { s64 la, lb; };
struct { u64 ula, ulb; };
struct { f64 dx, dy; };
};

/** VM Opcode, Everything before Run is serialized in JIT */
struct BSVM2_Opcode_s {
short i0, i1, i2;
short t0, t1, t2;
BSVM2_Value v;

void (*Run)(BSVM2_Frame *frm, BSVM2_Opcode *op);
short opn, opn2;
byte z, o;
int opfl;
};

/** VM Opcode, Everything before Run is serialized in JIT */
struct BSVM2_TailOpcode_s {
short i0, i1;
short t0, t1;
BSVM2_Value v;
BSVM2_Trace *nexttrace;
BSVM2_Trace *jmptrace;

BSVM2_Trace *(*Run)(BSVM2_Frame *frm,
	BSVM2_TailOpcode *op);
byte *jcs;
short opn, opn2;
byte z, o;
};

struct BSVM2_Trace_s {
BSVM2_Trace *(*Run)(BSVM2_Frame *frm,
	BSVM2_Trace *tr);
BSVM2_TailOpcode *top;
BSVM2_Trace *jnext;		//next trace to jump to
//BSVM2_CodeBlock *cblk;	//code block
s64 runcnt;				//execution count
s64 rsv[3];

BSVM2_Opcode **ops;
byte *cs;				//bytecode addr for trace
byte *jcs;				//bytecode addr for jump
char *gensym;			//trace gensym
char *gensym_fn;		//trace gensym (function)
void *t_ops[6];
int n_ops;
u64 trfl;
//BSVM2_JitTraceTemp *jitmp;	//JIT temp data
};

struct BSVM2_Frame_s {
BSVM2_Context *ctx;		//owning context
BSVM2_Value *stack;		//stack base
BSVM2_Value *local;		//locals base
BSVM2_Value *lxvar;		//lexical vars

BSVM2_Frame *rnext;		//return frame
BSVM2_Trace *rtrace;	//return trace
int rcsrv;				//return call save return value
int tstkpos;			//tstackpos
dtVal self;				//essentially, the 'this' value.
};

struct BSVM2_Context_s {
BSVM2_Context *next;		//next context
BSVM2_Frame *cstack;		//call stack
BSVM2_Value *tstack;		//temp stack (locals and stack)
int tstackref;				//ref position of temp stack

BSVM2_Frame *freeframe;		//free frames
void *freex128;				//free X128 vectors

BSVM2_Frame *frame;			//current frame
BSVM2_Trace *trace;			//current trace
BSVM2_Value *dynenv;		//dynamic environment
int szdynenv;				//allocated size of dynamic environment
int status;					//status code
dtVal thrownex;				//thrown exception object

BSVM2_Trace *ehstack_tr[256];	//exception handler stack trace
BSVM2_Frame *ehstack_fr[256];	//exception handler stack frame
int ehstackpos;					//exception handler stackpos
};

struct BSVM2_ImageGlobal_s {
char *name;				//name of variable
char *qname;			//qname of variable
char *sig;				//signature string
char *flagstr;			//flags string
int *figix;				//field/package GIX
int *ifgix;				//interface GIX
int gix;				//variable index
short nargs;			//number of arguments
short nfigix;			//number of fields
short nifgix;			//number of interfaces
// int sugix;			//superclass GIX
int giobj;				//GIX of owner or superclass
u32 tag;				//TLV type tag

s64 nameh;				//name hash
s64 qnameh;				//qname hash
u64 flags;				//variable flags (decoded from string)
s64 runcnt;				//execution count

BSVM2_Trace *ctrace;	//call trace
byte brty;				//base return type (functions)
union {
s64 batl[4];
byte baty[32];			//base arg type (functions)
};

BSVM2_Value *gvalue;	//global value
void *objinf;			//object info

//BSVM2_CodeImage *img;
BSVM2_ImageGlobal *pkg;
BSVM2_ImageGlobal *obj;
//BSVM2_CodeBlock *cblk;
};



#define DTV_NULL		DTV_MagicConst(0)
// #define DTV_NULL		DTV_MagicNull()
#define DTV_UNDEFINED	DTV_MagicConst(1)
#define DTV_FALSE		DTV_MagicConst(2)
#define DTV_TRUE		DTV_MagicConst(3)
#define DTV_EMPTYLIST	DTV_MagicConst(4)

dtVal DTV_MagicConst(int id)
{
	dtVal val;
	
//	val.hi=BGBDT_TAG_MCONST;
	val.lo=id;
	return(val);
}

dtVal dtvWrapPtr(void *ptr)
{
	dtVal val;
	val.vp=ptr;
	return(val);
}

void *dtvUnwrapPtr(dtVal val)
{
	return(val.vp);
}

dtVal dtvWrapInt(s32 val)
{
}

dtVal dtvWrapUInt(u32 val)
{
}

dtVal dtvWrapLong(s64 val)
{
}

dtVal dtvWrapFloat(f32 val)
{
}

dtVal dtvWrapDouble(f64 val)
{
}

s32 dtvUnwrapInt(dtVal val)
{
	return(val.vi);
}

s64 dtvUnwrapLong(dtVal val)
{
	return(val.vi);
}

s64 dtvUnwrapFloat(dtVal val)
{
	return(val.vi);
}

s64 dtvUnwrapDouble(dtVal val)
{
	return(val.vi);
}

char *bsvm2_natcall_nhashn[4096];
void *bsvm2_natcall_nhashv[4096];

BS2VM_API void *BSVM2_NatCall_GetProcAddress(char *name)
{
	char *s;
	void *p;
	int i, j, h;

	if(!name)
		return(NULL);

//	h=BGBDT_MM_QHashName(name);
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
//		p=BIPRO_LookupLabel(name);
		p=NULL;

		printf("BSVM2_NatCall_GetProcAddress: %s %p\n", name, p);

//		bsvm2_natcall_nhashn[j]=bgbdt_mm_strdup(name);
		bsvm2_natcall_nhashv[j]=p;
		return(p);
	}

//	p=BIPRO_LookupLabel(name);
	p=NULL;
	
	printf("BSVM2_NatCall_GetProcAddress: %s %p\n", name, p);
	
	return(p);

//	return(NULL);
}

BS2VM_API int BSVM2_NatCall_RegisterProcAddress(char *name, void *addr)
{
//	BIPRO_AddSym(name, addr);
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

#define BSVM2_NC_CALL1V(N, T0, P0)						\
	static void BSVM2_NatCall_Call##N(void *fptr,		\
		BSVM2_Value *rv, BSVM2_Value *av)				\
	{	void (*fcn)(T0); fcn=fptr;	fcn(av[0].P0);	}

#define BSVM2_NC_CALL1T(N, RT, RP, T0, P0)				\
	static void BSVM2_NatCall_Call##N(void *fptr,		\
		BSVM2_Value *rv, BSVM2_Value *av)				\
	{	RT (*fcn)(T0); fcn=fptr;	rv->RP=fcn(av[0].P0);	}

#define BSVM2_NC_CALL1(N, T0, P0)			\
	BSVM2_NC_CALL1V(N##V, T0, P0)			\
	BSVM2_NC_CALL1T(N##I, int, i, T0, P0)	\
	BSVM2_NC_CALL1T(N##L, s64, l, T0, P0)	\
	BSVM2_NC_CALL1T(N##F, f32, f, T0, P0)	\
	BSVM2_NC_CALL1T(N##D, f64, d, T0, P0)	\
	BSVM2_NC_CALL1T(N##P, void*, p, T0, P0)	\
	BSVM2_NC_CALL1T(N##A, dtVal, a, T0, P0)

#if 0
BSVM2_NC_CALL1(I, s32, i)
BSVM2_NC_CALL1(L, s64, l)
BSVM2_NC_CALL1(F, f32, f)
BSVM2_NC_CALL1(D, f64, d)
BSVM2_NC_CALL1(P, void*, p)
BSVM2_NC_CALL1(A, dtVal, a)
#endif

#define BSVM2_NC_CALL2V(N, T0, P0, T1, P1)				\
	static void BSVM2_NatCall_Call##N(void *fptr,		\
		BSVM2_Value *rv, BSVM2_Value *av)				\
	{	void (*fcn)(T0, T1); fcn=fptr;	fcn(av[0].P0, av[1].P1);	}

#define BSVM2_NC_CALL2T(N, RT, RP, T0, P0, T1, P1)		\
	static void BSVM2_NatCall_Call##N(void *fptr,		\
		BSVM2_Value *rv, BSVM2_Value *av)				\
	{	RT (*fcn)(T0, T1); fcn=fptr;	rv->RP=fcn(av[0].P0, av[1].P1);	}

#define BSVM2_NC_CALL2A(N, T0, P0, T1, P1)			\
	BSVM2_NC_CALL2V(N##V, T0, P0, T1, P1)			\
	BSVM2_NC_CALL2T(N##I, int, i, T0, P0, T1, P1)	\
	BSVM2_NC_CALL2T(N##L, s64, l, T0, P0, T1, P1)	\
	BSVM2_NC_CALL2T(N##F, f32, f, T0, P0, T1, P1)	\
	BSVM2_NC_CALL2T(N##D, f64, d, T0, P0, T1, P1)	\
	BSVM2_NC_CALL2T(N##P, void*, p, T0, P0, T1, P1)	\
	BSVM2_NC_CALL2T(N##A, dtVal, a, T0, P0, T1, P1)

#define BSVM2_NC_CALL2B(N, T0, P0)				\
	BSVM2_NC_CALL2A(N##I, T0, P0, s32, i)		\
	BSVM2_NC_CALL2A(N##L, T0, P0, s64, l)		\
	BSVM2_NC_CALL2A(N##F, T0, P0, f32, f)		\
	BSVM2_NC_CALL2A(N##D, T0, P0, f64, d)		\
	BSVM2_NC_CALL2A(N##P, T0, P0, void*, p)		\
	BSVM2_NC_CALL2A(N##A, T0, P0, dtVal, a)

#if 0
BSVM2_NC_CALL2B(I, s32, i)
BSVM2_NC_CALL2B(L, s64, l)
BSVM2_NC_CALL2B(F, f32, f)
BSVM2_NC_CALL2B(D, f64, d)
BSVM2_NC_CALL2B(P, void*, p)
BSVM2_NC_CALL2B(A, dtVal, a)
#endif

#if 0
BSVM2_Trace *BSVM2_TrOp_NatCallGFx(BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_NatCall_Call_N(op->v.p, op->i1,
		frm->stack+op->t0, frm->stack+op->t1);
	return(op->nexttrace);
}
#endif

#define BSVM2_NC_CASTCALL0T(N, RT, RP)				\
	case (N): rv->RP=((RT(*)(void))fcn)(); break;

#define BSVM2_NC_CASTCALL1T(N, RT, RP, T0, P0)				\
	case (N): rv->RP=((RT(*)(T0))fcn)(av[0].P0); break;

#define BSVM2_NC_CASTCALL2T(N, RT, RP, T0, P0, T1, P1)		\
	case (N): rv->RP=((RT(*)(T0, T1))fcn)(av[0].P0, av[1].P1); break;

#define BSVM2_NC_CASTCALL3T(N, RT, RP, T0, P0, T1, P1, T2, P2)		\
	case (N): rv->RP=((RT(*)(T0, T1, T2))fcn)	\
		(av[0].P0, av[1].P1, av[2].P2); break;

#define BSVM2_NC_CASTCALL4T(N, RT, RP, T0, P0, T1, P1, T2, P2, T3, P3)	\
	case (N): rv->RP=((RT(*)(T0, T1, T2, T3))fcn)	\
		(av[0].P0, av[1].P1, av[2].P2, av[3].P3); break;


#define BSVM2_NC_CASTCALL0A(N)					\
	BSVM2_NC_CASTCALL0T(((N)*6)+0, s32, i)		\
	BSVM2_NC_CASTCALL0T(((N)*6)+1, s64, l)		\
	BSVM2_NC_CASTCALL0T(((N)*6)+2, f32, f)		\
	BSVM2_NC_CASTCALL0T(((N)*6)+3, f64, d)		\
	BSVM2_NC_CASTCALL0T(((N)*6)+4, dtVal, a)	\
	BSVM2_NC_CASTCALL0T(((N)*6)+5, void*, p)


#define BSVM2_NC_CASTCALL1A(N, T0, P0)						\
	BSVM2_NC_CASTCALL1T(((N)*6)+0, s32, i, T0, P0)			\
	BSVM2_NC_CASTCALL1T(((N)*6)+1, s64, l, T0, P0)			\
	BSVM2_NC_CASTCALL1T(((N)*6)+2, f32, f, T0, P0)			\
	BSVM2_NC_CASTCALL1T(((N)*6)+3, f64, d, T0, P0)			\
	BSVM2_NC_CASTCALL1T(((N)*6)+4, dtVal, a, T0, P0)		\
	BSVM2_NC_CASTCALL1T(((N)*6)+5, void*, p, T0, P0)

#define BSVM2_NC_CASTCALL1B(N)								\
	BSVM2_NC_CASTCALL1A(((N)*6)+0, s32, i)					\
	BSVM2_NC_CASTCALL1A(((N)*6)+1, s64, l)					\
	BSVM2_NC_CASTCALL1A(((N)*6)+2, f32, f)					\
	BSVM2_NC_CASTCALL1A(((N)*6)+3, f64, d)					\
	BSVM2_NC_CASTCALL1A(((N)*6)+4, dtVal, a)				\
	BSVM2_NC_CASTCALL1A(((N)*6)+5, void*, p)


#define BSVM2_NC_CASTCALL2A(N, T0, P0, T1, P1)					\
	BSVM2_NC_CASTCALL2T(((N)*6)+0, s32, i, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL2T(((N)*6)+1, s64, l, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL2T(((N)*6)+2, f32, f, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL2T(((N)*6)+3, f64, d, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL2T(((N)*6)+4, dtVal, a, T0, P0, T1, P1)	\
	BSVM2_NC_CASTCALL2T(((N)*6)+5, void*, p, T0, P0, T1, P1)

#if 0
#define BSVM2_NC_CASTCALL2B(N, T0, P0)						\
	BSVM2_NC_CASTCALL2A(((N)*6)+0, s32, i, T0, P0)			\
	BSVM2_NC_CASTCALL2A(((N)*6)+1, s64, l, T0, P0)			\
	BSVM2_NC_CASTCALL2A(((N)*6)+2, f32, f, T0, P0)			\
	BSVM2_NC_CASTCALL2A(((N)*6)+3, f64, d, T0, P0)			\
	BSVM2_NC_CASTCALL2A(((N)*6)+4, dtVal, a, T0, P0)		\
	BSVM2_NC_CASTCALL2A(((N)*6)+5, void*, p, T0, P0)
#endif

#if 1
#define BSVM2_NC_CASTCALL2B(N, T0, P0)						\
	BSVM2_NC_CASTCALL2A(((N)*6)+0, T0, P0, s32, i)			\
	BSVM2_NC_CASTCALL2A(((N)*6)+1, T0, P0, s64, l)			\
	BSVM2_NC_CASTCALL2A(((N)*6)+2, T0, P0, f32, f)			\
	BSVM2_NC_CASTCALL2A(((N)*6)+3, T0, P0, f64, d)			\
	BSVM2_NC_CASTCALL2A(((N)*6)+4, T0, P0, dtVal, a)		\
	BSVM2_NC_CASTCALL2A(((N)*6)+5, T0, P0, void*, p)
#endif

#define BSVM2_NC_CASTCALL2C(N)						\
	BSVM2_NC_CASTCALL2B(((N)*6)+0, s32, i)			\
	BSVM2_NC_CASTCALL2B(((N)*6)+1, s64, l)			\
	BSVM2_NC_CASTCALL2B(((N)*6)+2, f32, f)			\
	BSVM2_NC_CASTCALL2B(((N)*6)+3, f64, d)			\
	BSVM2_NC_CASTCALL2B(((N)*6)+4, dtVal, a)		\
	BSVM2_NC_CASTCALL2B(((N)*6)+5, void*, p)


#define BSVM2_NC_CASTCALL3A(N, T0, P0, T1, P1, T2, P2)					\
	BSVM2_NC_CASTCALL3T(((N)*6)+0, s32, i, T0, P0, T1, P1, T2, P2)		\
	BSVM2_NC_CASTCALL3T(((N)*6)+1, s64, l, T0, P0, T1, P1, T2, P2)		\
	BSVM2_NC_CASTCALL3T(((N)*6)+2, f32, f, T0, P0, T1, P1, T2, P2)		\
	BSVM2_NC_CASTCALL3T(((N)*6)+3, f64, d, T0, P0, T1, P1, T2, P2)		\
	BSVM2_NC_CASTCALL3T(((N)*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2)	\
	BSVM2_NC_CASTCALL3T(((N)*6)+5, void*, p, T0, P0, T1, P1, T2, P2)

#if 0
#define BSVM2_NC_CASTCALL3B(N, T0, P0, T1, P1)					\
	BSVM2_NC_CASTCALL3A(((N)*6)+0, s32, i, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL3A(((N)*6)+1, s64, l, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL3A(((N)*6)+2, f32, f, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL3A(((N)*6)+3, f64, d, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL3A(((N)*6)+4, dtVal, a, T0, P0, T1, P1)	\
	BSVM2_NC_CASTCALL3A(((N)*6)+5, void*, p, T0, P0, T1, P1)

#define BSVM2_NC_CASTCALL3C(N, T0, P0)						\
	BSVM2_NC_CASTCALL3B(((N)*6)+0, s32, i, T0, P0)			\
	BSVM2_NC_CASTCALL3B(((N)*6)+1, s64, l, T0, P0)			\
	BSVM2_NC_CASTCALL3B(((N)*6)+2, f32, f, T0, P0)			\
	BSVM2_NC_CASTCALL3B(((N)*6)+3, f64, d, T0, P0)			\
	BSVM2_NC_CASTCALL3B(((N)*6)+4, dtVal, a, T0, P0)		\
	BSVM2_NC_CASTCALL3B(((N)*6)+5, void*, p, T0, P0)
#endif

#if 1
#define BSVM2_NC_CASTCALL3B(N, T0, P0, T1, P1)					\
	BSVM2_NC_CASTCALL3A(((N)*6)+0, T0, P0, T1, P1, s32, i)		\
	BSVM2_NC_CASTCALL3A(((N)*6)+1, T0, P0, T1, P1, s64, l)		\
	BSVM2_NC_CASTCALL3A(((N)*6)+2, T0, P0, T1, P1, f32, f)		\
	BSVM2_NC_CASTCALL3A(((N)*6)+3, T0, P0, T1, P1, f64, d)		\
	BSVM2_NC_CASTCALL3A(((N)*6)+4, T0, P0, T1, P1, dtVal, a)	\
	BSVM2_NC_CASTCALL3A(((N)*6)+5, T0, P0, T1, P1, void*, p)

#define BSVM2_NC_CASTCALL3C(N, T0, P0)						\
	BSVM2_NC_CASTCALL3B(((N)*6)+0, T0, P0, s32, i)			\
	BSVM2_NC_CASTCALL3B(((N)*6)+1, T0, P0, s64, l)			\
	BSVM2_NC_CASTCALL3B(((N)*6)+2, T0, P0, f32, f)			\
	BSVM2_NC_CASTCALL3B(((N)*6)+3, T0, P0, f64, d)			\
	BSVM2_NC_CASTCALL3B(((N)*6)+4, T0, P0, dtVal, a)		\
	BSVM2_NC_CASTCALL3B(((N)*6)+5, T0, P0, void*, p)
#endif

#define BSVM2_NC_CASTCALL3D(N)						\
	BSVM2_NC_CASTCALL3C(((N)*6)+0, s32, i)			\
	BSVM2_NC_CASTCALL3C(((N)*6)+1, s64, l)			\
	BSVM2_NC_CASTCALL3C(((N)*6)+2, f32, f)			\
	BSVM2_NC_CASTCALL3C(((N)*6)+3, f64, d)			\
	BSVM2_NC_CASTCALL3C(((N)*6)+4, dtVal, a)		\
	BSVM2_NC_CASTCALL3C(((N)*6)+5, void*, p)


#define BSVM2_NC_CASTCALL4A(N, T0, P0, T1, P1, T2, P2, T3, P3)				\
	BSVM2_NC_CASTCALL4T(((N)*6)+0, s32, i, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSVM2_NC_CASTCALL4T(((N)*6)+1, s64, l, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSVM2_NC_CASTCALL4T(((N)*6)+2, f32, f, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSVM2_NC_CASTCALL4T(((N)*6)+3, f64, d, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSVM2_NC_CASTCALL4T(((N)*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2, T3, P3)	\
	BSVM2_NC_CASTCALL4T(((N)*6)+5, void*, p, T0, P0, T1, P1, T2, P2, T3, P3)

#if 0
#define BSVM2_NC_CASTCALL4B(N, T0, P0, T1, P1, T2, P2)					\
	BSVM2_NC_CASTCALL4A(((N)*6)+0, s32, i, T0, P0, T1, P1, T2, P2)		\
	BSVM2_NC_CASTCALL4A(((N)*6)+1, s64, l, T0, P0, T1, P1, T2, P2)		\
	BSVM2_NC_CASTCALL4A(((N)*6)+2, f32, f, T0, P0, T1, P1, T2, P2)		\
	BSVM2_NC_CASTCALL4A(((N)*6)+3, f64, d, T0, P0, T1, P1, T2, P2)		\
	BSVM2_NC_CASTCALL4A(((N)*6)+4, dtVal, a, T0, P0, T1, P1, T2, P2)	\
	BSVM2_NC_CASTCALL4A(((N)*6)+5, void*, p, T0, P0, T1, P1, T2, P2)

#define BSVM2_NC_CASTCALL4C(N, T0, P0, T1, P1)						\
	BSVM2_NC_CASTCALL4B(((N)*6)+0, s32, i, T0, P0, T1, P1)			\
	BSVM2_NC_CASTCALL4B(((N)*6)+1, s64, l, T0, P0, T1, P1)			\
	BSVM2_NC_CASTCALL4B(((N)*6)+2, f32, f, T0, P0, T1, P1)			\
	BSVM2_NC_CASTCALL4B(((N)*6)+3, f64, d, T0, P0, T1, P1)			\
	BSVM2_NC_CASTCALL4B(((N)*6)+4, dtVal, a, T0, P0, T1, P1)		\
	BSVM2_NC_CASTCALL4B(((N)*6)+5, void*, p, T0, P0, T1, P1)

#define BSVM2_NC_CASTCALL4D(N, T0, P0)						\
	BSVM2_NC_CASTCALL4C(((N)*6)+0, s32, i, T0, P0)			\
	BSVM2_NC_CASTCALL4C(((N)*6)+1, s64, l, T0, P0)			\
	BSVM2_NC_CASTCALL4C(((N)*6)+2, f32, f, T0, P0)			\
	BSVM2_NC_CASTCALL4C(((N)*6)+3, f64, d, T0, P0)			\
	BSVM2_NC_CASTCALL4C(((N)*6)+4, dtVal, a, T0, P0)		\
	BSVM2_NC_CASTCALL4C(((N)*6)+5, void*, p, T0, P0)
#endif

#if 1
#define BSVM2_NC_CASTCALL4B(N, T0, P0, T1, P1, T2, P2)					\
	BSVM2_NC_CASTCALL4A(((N)*6)+0, T0, P0, T1, P1, T2, P2, s32, i)		\
	BSVM2_NC_CASTCALL4A(((N)*6)+1, T0, P0, T1, P1, T2, P2, s64, l)		\
	BSVM2_NC_CASTCALL4A(((N)*6)+2, T0, P0, T1, P1, T2, P2, f32, f)		\
	BSVM2_NC_CASTCALL4A(((N)*6)+3, T0, P0, T1, P1, T2, P2, f64, d)		\
	BSVM2_NC_CASTCALL4A(((N)*6)+4, T0, P0, T1, P1, T2, P2, dtVal, a)	\
	BSVM2_NC_CASTCALL4A(((N)*6)+5, T0, P0, T1, P1, T2, P2, void*, p)

#define BSVM2_NC_CASTCALL4C(N, T0, P0, T1, P1)						\
	BSVM2_NC_CASTCALL4B(((N)*6)+0, T0, P0, T1, P1, s32, i)			\
	BSVM2_NC_CASTCALL4B(((N)*6)+1, T0, P0, T1, P1, s64, l)			\
	BSVM2_NC_CASTCALL4B(((N)*6)+2, T0, P0, T1, P1, f32, f)			\
	BSVM2_NC_CASTCALL4B(((N)*6)+3, T0, P0, T1, P1, f64, d)			\
	BSVM2_NC_CASTCALL4B(((N)*6)+4, T0, P0, T1, P1, dtVal, a)		\
	BSVM2_NC_CASTCALL4B(((N)*6)+5, T0, P0, T1, P1, void*, p)

#define BSVM2_NC_CASTCALL4D(N, T0, P0)						\
	BSVM2_NC_CASTCALL4C(((N)*6)+0, T0, P0, s32, i)			\
	BSVM2_NC_CASTCALL4C(((N)*6)+1, T0, P0, s64, l)			\
	BSVM2_NC_CASTCALL4C(((N)*6)+2, T0, P0, f32, f)			\
	BSVM2_NC_CASTCALL4C(((N)*6)+3, T0, P0, f64, d)			\
	BSVM2_NC_CASTCALL4C(((N)*6)+4, T0, P0, dtVal, a)		\
	BSVM2_NC_CASTCALL4C(((N)*6)+5, T0, P0, void*, p)
#endif


#define BSVM2_NC_CASTCALL4E(N)						\
	BSVM2_NC_CASTCALL4D(((N)*6)+0, s32, i)			\
	BSVM2_NC_CASTCALL4D(((N)*6)+1, s64, l)			\
	BSVM2_NC_CASTCALL4D(((N)*6)+2, f32, f)			\
	BSVM2_NC_CASTCALL4D(((N)*6)+3, f64, d)			\
	BSVM2_NC_CASTCALL4D(((N)*6)+4, dtVal, a)		\
	BSVM2_NC_CASTCALL4D(((N)*6)+5, void*, p)


void BSVM2_NatCall_Call_N(void *fcn, int nc,
	BSVM2_Value *rv, BSVM2_Value *av)
{
	switch(nc)
	{
		BSVM2_NC_CASTCALL0A(1)
		BSVM2_NC_CASTCALL1B(1)
		BSVM2_NC_CASTCALL2C(1)
		BSVM2_NC_CASTCALL3D(1)
		BSVM2_NC_CASTCALL4E(1)
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
//		i=BS2CC_TYZ_INT128;
		break;

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

#if 0
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
//				v0=dtvArrayGetIndexDtVal(va, j);
				
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
#endif

BS2VM_API BSVM2_Trace *BSVM2_TrOp_NatCallG1(
	BSVM2_Frame *frm, BSVM2_TailOpcode *op)
{
	BSVM2_Value argt[64];
	BSVM2_ImageGlobal *vi;
	void *p;
	int ic;

	vi=op->v.p;
	p=BSVM2_NatCall_GetProcAddress(vi->name);

//	ic=BSVM2_NatCall_GetSigIndexG1(vi->sig,
//		frm->stack+op->t1, argt);

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

int main()
{
	BSVM2_Value argt[64];
	void *p;
	int i, j, k;
	
	p=(void *)(rand()); j=rand();
	
	for(i=0; i<256; i++)
	{
		j=j*251+1;
		BSVM2_NatCall_Call_N(p, j, argt, argt);
	}
}