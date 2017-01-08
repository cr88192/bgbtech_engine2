/*
64-bit tag value, consisting of a low and high part.

High 4 bits:
  0  : Object Pointer
  1  : Reduced tag space
  2-3: Resv
  4-7: Fixlong
  8-B: Flonum
  C  : TagArray
  D  : Vec2f
  E  : TT Pointer (48 bit)
  F  : Raw Pointer (60 bit)
  
  Reduced Tag Space:
  10-17: 56-bit spaces
  1800-1BFF: 48 bit spaces
  1C000000-1DFFFFFF: 32-bit spaces

  10: RotLong
  11: FComplex

Object Pointer:
  Low 48 bits: Address
  Bits 48-59: Type ID

TagArray
  Low 32 bits: Array Object ID
  Bits 32-59: Base Offset

RotLong
   0-47: Value Bits
  48-53: Rot
  54/55: Fill Bits
Stores a long value, where 48 bits are represented directly.
The fill bits are repeated to create a fill pattern for the remaining 16 bits.
The value is then rotated left N bits to get the desired value.

Vec2f
   0-29: X
  30-59: Y
Consists of a pair of floating-point values (X and Y).
These are stored at 30 bits each.


TT Pointer:
  Low 48 bits: Base Address
  Bits 48-59: Type Tag

The TT Pointer will represent a raw "C style" pointer.

Type Tag 0 will be special, and refer to an untagged "void *" pointer.
	Other type-tags will identify types.
	Part of the range will be for fixed/primitive types.
	The rest will be for registered types.

The type-tag for a TT Pointer will primarily serve as a hint for cases where a pointer is cast to variant. A raw pointer which has not neen cast to variant will not necessarily be required to have a valid type-tag.

*/

#define BGBDT_HTAG_PTR		 0
#define BGBDT_HTAG_TAG2		 1

#define BGBDT_HTAG_FIXLONG0	 4
#define BGBDT_HTAG_FIXLONG1	 5
#define BGBDT_HTAG_FIXLONG2	 6
#define BGBDT_HTAG_FIXLONG3	 7

#define BGBDT_HTAG_FIXREAL0	 8
#define BGBDT_HTAG_FIXREAL1	 9
#define BGBDT_HTAG_FIXREAL2	10
#define BGBDT_HTAG_FIXREAL3	11

#define BGBDT_HTAG_TAGARR	12
#define BGBDT_HTAG_VEC2F	13
#define BGBDT_HTAG_TTPTR	14
#define BGBDT_HTAG_PTR60	15

#define BGBDT_TAG_FCOMPLEX	0x11000000

#define BGBDT_TAG_INT32		0x1C000000
#define BGBDT_TAG_FLOAT32	0x1C000001
#define BGBDT_TAG_UINT32	0x1C000002
#define BGBDT_TAG_MCONST	0x1C000003	//Magic Constant
#define BGBDT_TAG_MCHAR		0x1C000004	//Character Codepoint
#define BGBDT_TAG_MCONTIDX	0x1C000005	//Context-Dependent Index

/* Logical Base Types
 * Note that these need not necessarily match up with C's types.
 */
#define BGBDT_BASETY_INT		0		//int32
#define BGBDT_BASETY_LONG		1		//int64
#define BGBDT_BASETY_FLOAT		2		//float (32-bit)
#define BGBDT_BASETY_DOUBLE		3		//double (64-bit)
#define BGBDT_BASETY_ADDRESS	4		//address/tagval (64-bit)
#define BGBDT_BASETY_UINT		5
#define BGBDT_BASETY_UBYTE		6
#define BGBDT_BASETY_SHORT		7
#define BGBDT_BASETY_SBYTE		8
#define BGBDT_BASETY_USHORT		9
#define BGBDT_BASETY_ULONG		10
#define BGBDT_BASETY_VOID		11
#define BGBDT_BASETY_NLONG		12
#define BGBDT_BASETY_UNLONG		13
#define BGBDT_BASETY_X128		14

#define BGBDT_BASETY_VARIANT	16		//tagval

typedef union BGBDT_TagValue_s		BGBDT_TagValue;
typedef union BGBDT_TagValue_s		dtVal;

typedef struct BGBDT_TagArrHead_s	BGBDT_TagArrHead;

typedef struct BGBDT_MapObjHead_s	BGBDT_MapObjHead;
typedef struct BGBDT_MapObjNode_s	BGBDT_MapObjNode;
typedef struct BGBDT_MapObjFieldInfo_s	BGBDT_MapObjFieldInfo;
typedef struct BGBDT_MapObjClassInfo_s	BGBDT_MapObjClassInfo;

union BGBDT_TagValue_s {
struct { u32 lo, hi; };
u64 vi;
void *vp;
};

struct BGBDT_TagArrHead_s {
byte *data;		//data for array
int sz;			//array size
int str;		//array stride
byte bty;		//base type
byte strsh;		//stride shift
int scsz;		//scaled size
};

struct BGBDT_MapObjHead_s {
BGBDT_MapObjClassInfo *cli;	//class info
void *idat;					//instance data (class/instance objects)
BGBDT_MapObjNode *node;		//B+Tree node
dtVal sval[8];				//fixed slot value
u16 sidx[8];				//fixed slot index
u16 nslot;					//number of slots
};

struct BGBDT_MapObjNode_s {
BGBDT_MapObjNode *lno;	//left node (same level)
BGBDT_MapObjNode *rno;	//right node (same level)
BGBDT_MapObjNode *pno;	//parent node (same level)
dtVal sval[16];		//fixed slot value
u16 sidx[16];		//fixed slot index
byte nslot;			//number of items in node
byte lvl;			//level of node
};

struct BGBDT_MapObjFieldInfo_s {
char *name;		//field name
char *sig;		//type signature
char *flsig;	//flag type signature
u16 fidx;		//field index number
u16 hnext;		//next field in hash
};

struct BGBDT_MapObjClassInfo_s {
char *qname;
BGBDT_MapObjClassInfo *super;

int *sidx;		//fixed field index
int *soffs;		//field offs
int ns;			//num fields
int idsz;		//object instance data size
};

// #define DTV_NULL		DTV_MagicConst(0)
#define DTV_NULL		DTV_MagicNull()
#define DTV_UNDEFINED	DTV_MagicConst(1)
#define DTV_FALSE		DTV_MagicConst(2)
#define DTV_TRUE		DTV_MagicConst(3)
#define DTV_EMPTYLIST	DTV_MagicConst(4)

BS2VM_API void *BGBDT_MM_GetPtrForObjId(int objid);
BS2VM_API int BGBDT_MM_GetObjIdForPtr(void *ptr);
BS2VM_API void *BGBDT_MM_GetDataPtrForObjId(int objid);

BS2VM_API dtVal BGBDT_TagTy_EncodeRotLong(s64 value);
BS2VM_API s64 BGBDT_TagTy_DecodeRotLong(dtVal val);

BS2VM_API int BGBDT_MM_GetIndexObjTypeName(char *name);


static_inline dtVal DTV_MagicConst(int id)
{
	dtVal val;
	
	val.hi=BGBDT_TAG_MCONST;
	val.lo=id;
	return(val);
}

static_inline dtVal DTV_MagicNull(void)
{
	dtVal val;
	
//	val.hi=BGBDT_TAG_MCONST;
//	val.lo=id;
	val.vi=0;
	return(val);
}

#if defined(X86) || defined(ARM) || defined(__EMSCRIPTEN__)
//static_inline void *DTV_GetPtrForObjId(int objid)
//	{ return((void *)(objid)); }
//static_inline int DTV_GetObjIdForPtr(void *ptr)
//	{ return((int)(ptr)); }
//static_inline void *DTV_GetDataPtrForObjId(int objid)
//	{ return((void *)(objid)); }

#define DTV_GetPtrForObjId(objid)		((void *)(objid))
#define DTV_GetObjIdForPtr(ptr)			((int)(ptr))
#define DTV_GetDataPtrForObjId(objid)	((void *)(objid))

#else
//static_inline void *DTV_GetPtrForObjId(int objid)
//	{ return(BGBDT_MM_GetPtrForObjId(objid)); }
//static_inline int DTV_GetObjIdForPtr(void *ptr)
//	{ return(BGBDT_MM_GetObjIdForPtr(ptr)); }
//static_inline void *DTV_GetDataPtrForObjId(int objid)
//	{ return(BGBDT_MM_GetDataPtrForObjId(objid)); }

#define DTV_GetPtrForObjId(objid)		BGBDT_MM_GetPtrForObjId(objid)
#define DTV_GetObjIdForPtr(ptr)			BGBDT_MM_GetObjIdForPtr(ptr)
#define DTV_GetDataPtrForObjId(objid)	BGBDT_MM_GetDataPtrForObjId(objid)
#endif

#if defined(X86) || defined(ARM) || defined(__EMSCRIPTEN__)
#define DTV_UNWRAP_PTR_P

static_inline void *dtvUnwrapPtrF(dtVal val)
	{ return((void *)val.lo); }
static_inline dtVal dtvWrapPtrF(void *ptr)
	{ dtVal val; val.lo=(u32)ptr; val.hi=0; return(val); }

static_inline dtVal dtvWrapTagPtrF(void *ptr, int tag)
{
	dtVal val;
	val.lo=(u32)ptr;
	val.hi=(tag&4095)<<16;
	return(val);
}

static_inline dtVal dtvWrapTyTagPtrF(void *ptr, int tag)
{
	dtVal val;
	val.lo=(u32)ptr;
	val.hi=(0xE000|(tag&4095))<<16;
	return(val);
}

static_inline void *dtvUnwrapPtr60F(dtVal val)
	{ return((void *)val.lo); }
#endif

#if defined(X86_64) || defined(ARM64)
#define DTV_UNWRAP_PTR_P
static_inline void *dtvUnwrapPtrF(dtVal val)
{
	s64 h;
//	s32 hi;
//	hi=((s32)(val.hi<<16))>>16;
//	return((void *)((val.lo)|(((s64)hi)<<32)));
//	h=*(s64 *)(&val);
	h=val.vi;
	h=(h<<16)>>16;
	return((void *)h);
}

static_inline dtVal dtvWrapPtrF(void *ptr)
{
	dtVal val;
	u64 pint;
	pint=(u64)ptr;
	pint=(pint<<16)>>16;
	val.vi=pint;
//	*(u64 *)(&val)=pint;
//	val.lo=(u32)pi;
//	val.hi=(pi>>32)&0xFFFF;
	return(val);
}

static_inline dtVal dtvWrapTagPtrF(void *ptr, int tag)
{
	dtVal val;
	u64 pi;
	pi=(u64)ptr;
	pi=(pi<<16)>>16;
	pi|=((u64)(tag&4095))<<48;
	val.vi=pi;
//	*(u64 *)(&val)=pi;
//	val.lo=(u32)pi;
//	val.hi=(pi>>32)&0xFFFF;
	return(val);
}

static_inline dtVal dtvWrapTyTagPtrF(void *ptr, int tag)
{
	dtVal val;
	u64 pi;
	pi=(u64)ptr;
	pi=(pi<<16)>>16;
	pi|=((u64)(0xE000|(tag&4095)))<<48;
	val.vi=pi;
	return(val);
}

static_inline void *dtvUnwrapPtr60F(dtVal val)
{
	return((void *)((((s64)val.vi)<<4)>>4));
}
#endif

#ifndef DTV_UNWRAP_PTR_P
static_inline void *dtvUnwrapPtrF(dtVal val)
{
	s64 h;
	h=val.vi;
	h=(h<<16)>>16;
	return((void *)h);
}

static_inline dtVal dtvWrapPtrF(void *ptr)
{
	dtVal val;
	u64 pint;
	pint=(u64)ptr;
	pint=(pint<<16)>>16;
	val.vi=pint;
	return(val);
}

static_inline dtVal dtvWrapTagPtrF(void *ptr, int tag)
{
	dtVal val;
	u64 pi;
	pi=(u64)ptr;
	pi=(pi<<16)>>16;
	pi|=((u64)(tag&4095))<<48;
	val.vi=pi;
	return(val);
}

static_inline dtVal dtvWrapTyTagPtrF(void *ptr, int tag)
{
	dtVal val;
	u64 pi;
	pi=(u64)ptr;
	pi=(pi<<16)>>16;
	pi|=((u64)(0xE000|(tag&4095)))<<48;
	val.vi=pi;
	return(val);
}

static_inline void *dtvUnwrapPtr60F(dtVal val)
{
	return((void *)((((s64)val.vi)<<4)>>4));
}
#endif

BS2VM_API dtVal BGBDT_MM_DtvWrapObjPtr(void *ptr);

static_inline dtVal dtvWrapPtr(void *ptr)
	{ return(BGBDT_MM_DtvWrapObjPtr(ptr)); }

static_inline int dtvGetPtrTagF(dtVal val)
	{ return(val.hi>>16); }
static_inline int dtvCheckPtrTagP(dtVal val, int tag)
	{ return((val.hi>>16)==tag); }

static_inline bool dtvIsPtrP(dtVal val)
	{ return((val.hi>>28)==0); }
static_inline bool dtvIsFixLongP(dtVal val)
	{ return((val.hi>>30)==1); }
static_inline bool dtvIsFixDoubleP(dtVal val)
	{ return((val.hi>>30)==2); }

static_inline bool dtvIsFixIntP(dtVal val)
	{ return(val.hi==BGBDT_TAG_INT32); }
static_inline bool dtvIsFixFloatP(dtVal val)
	{ return(val.hi==BGBDT_TAG_FLOAT32); }
static_inline bool dtvIsFixUIntP(dtVal val)
	{ return(val.hi==BGBDT_TAG_UINT32); }

static_inline int dtvNullP(dtVal val)
{
	if(val.hi==BGBDT_TAG_MCONST)
	{
		if(val.lo==0)
			return(1);
		return(0);
	}
	
	if(!val.vi)
		return(1);
	return(0);
}

static_inline int dtvTrueP(dtVal val)
{
	int ret;
	
	if(val.hi==BGBDT_TAG_MCONST)
	{
		switch(val.lo)
		{
		case 0:
		case 1:
		case 2:
			ret=0;
			break;
		default:
			ret=1;
		}
		return(ret);
//		val.lo=id;
	}
	
	if((val.hi==BGBDT_TAG_INT32) || (val.hi==BGBDT_TAG_UINT32))
	{
		return((val.lo)!=0);
	}

	if(!val.vi)
		return(0);
	
	return(1);
//	return((val.hi>>16)==tag);
}

static_inline int dtvUndefinedP(dtVal val)
{
	if(val.hi==BGBDT_TAG_MCONST)
	{
		if(val.lo==1)
			return(1);
		return(0);
	}
	
//	if(!val.vi)
//		return(1);
	return(0);
}

static_inline bool dtvIsBoolP(dtVal val)
{
	int ret;
	
	if(val.hi==BGBDT_TAG_MCONST)
	{
		switch(val.lo)
		{
		case 2: case 3:
			ret=1;
			break;
		default:
			ret=0;
		}
		return(ret);
	}
	
	return(0);
}

static_inline bool dtvIsCharP(dtVal val)
{
	int ret;
	
	if(val.hi==BGBDT_TAG_MCHAR)
		return(1);
	return(0);
}

static_inline bool dtvIsContIdxP(dtVal val)
{
	int ret;
	
	if(val.hi==BGBDT_TAG_MCONTIDX)
		return(1);
	return(0);
}


static_inline int dtvEqqP(dtVal val1, dtVal val2)
{
	return(val1.vi==val2.vi);
}

static_inline int dtvNeqP(dtVal val1, dtVal val2)
{
	return(val1.vi!=val2.vi);
}

static_inline s32 dtvUnwrapIntF(dtVal val)
	{ return((s32)val.lo); }
static_inline dtVal dtvWrapIntF(s32 v)
	{ dtVal val; val.lo=(u32)v; val.hi=BGBDT_TAG_INT32; return(val); }

// static_inline s32 dtvUnwrapInt(dtVal val)
//	{ return(dtvUnwrapIntF(val)); }
static_inline dtVal dtvWrapInt(s32 v)
	{ return(dtvWrapIntF(v)); }

static_inline u32 dtvUnwrapUIntF(dtVal val)
	{ return(val.lo); }
static_inline dtVal dtvWrapUIntF(u32 v)
	{ dtVal val; val.lo=v; val.hi=BGBDT_TAG_UINT32; return(val); }
static_inline dtVal dtvWrapUInt(s32 v)
	{ return(dtvWrapUIntF(v)); }

static_inline dtVal dtvWrapChar(s32 v)
	{ dtVal val; val.lo=(u32)v; val.hi=BGBDT_TAG_MCHAR; return(val); }
static_inline dtVal dtvWrapContIdx(s32 v)
	{ dtVal val; val.lo=(u32)v; val.hi=BGBDT_TAG_MCONTIDX; return(val); }

static_inline dtVal dtvWrapBool(int v)
	{ dtVal val; val.lo=v?3:2; val.hi=BGBDT_TAG_MCONST; return(val); }


static_inline float dtvUnwrapFloatF(dtVal val)
	{ return(*(float *)(&val.lo)); }

static_inline dtVal dtvWrapFloatF(float v)
{
	dtVal val;
	val.lo=*(u32 *)(&v);
	val.hi=BGBDT_TAG_FLOAT32;
	return(val);
}

static_inline dtVal dtvWrapFloat(float v)
	{ return(dtvWrapFloatF(v)); }


static_inline s64 dtvUnwrapLongF(dtVal val)
{
	s64 v;
	v=*(s64 *)(&val);
	v=(v<<2)>>2;
	return(v);
}

static_inline dtVal dtvWrapLongF(s64 v)
{
	dtVal val;
	u64 h;
	h=((u64)(v<<2))>>2;
//	*(u64 *)(&val)=h;
	val.vi=h;
	val.hi|=0x40000000U;
	return(val);
}

static_inline double dtvUnwrapDoubleF(dtVal val)
{
	u64 v;
//	v=*(u64 *)(&val);
	v=val.vi;
	v=v<<2;
	return(*(double *)(&v));
}

static_inline dtVal dtvWrapDoubleF(double v)
{
	dtVal val;
	u64 h;
	h=*(u64 *)(&v);
//	h=(h>>2);
	h=((h+2)>>2);
	val.vi=h;
//	*(u64 *)(&val)=h;
	val.hi|=0x80000000U;
	return(val);
}

static_inline dtVal dtvWrapDouble(double v)
	{ return(dtvWrapDoubleF(v)); }

static_inline dtVal dtvWrapLong(s64 v)
{
	dtVal val;
	u64 h;
	
	if(((v<<2)>>2)!=v)
	{
		val=BGBDT_TagTy_EncodeRotLong(v);
		return(val);
	}
	
	h=((u64)(v<<2))>>2;
//	*(u64 *)(&val)=h;
	val.vi=h;
	val.hi|=0x40000000U;
	return(val);
}

static_inline int dtvIsSmallIntP(dtVal val)
{
	int v;
	switch(val.hi>>28)
	{
	case 0:
		v=0;
		break;
	case 1:
		if(val.hi==BGBDT_TAG_INT32)
			{ v=1; break; }
		if(val.hi==BGBDT_TAG_UINT32)
			{ v=1; break; }
		if(val.hi==BGBDT_TAG_FLOAT32)
			{ v=0; break; }
		if(val.hi==BGBDT_TAG_MCONST)
		{
			switch(val.lo)
			{
			case 0: case 1:
				v=0; break;
			case 2: case 3:
				v=1; break;
			default:	v=0; break;
			}
			break;
		}
//		if(val.hi==BGBDT_TAG_MCHAR)
		if(val.hi==BGBDT_TAG_MCHAR)
			{ v=1; break; }
		if((val.hi>>24)==0x10)
			{ v=1; break; }
		v=0;
		break;
	case 4: case 5: case 6: case 7:
		v=0; break;
	case 8: case 9: case 10: case 11:
		v=0; break;
	default:
		v=0;
		break;
	}
	return(v);
}

static_inline int dtvIsSmallLongP(dtVal val)
{
	int v;
	switch(val.hi>>28)
	{
	case 0:
		v=0;
		break;
	case 1:
		if(val.hi==BGBDT_TAG_INT32)
			{ v=1; break; }
		if(val.hi==BGBDT_TAG_UINT32)
			{ v=1; break; }
		if(val.hi==BGBDT_TAG_FLOAT32)
			{ v=0; break; }
		if(val.hi==BGBDT_TAG_MCONST)
		{
			switch(val.lo)
			{
			case 0: case 1:
				v=0; break;
			case 2: case 3:
				v=1; break;
			default:	v=0; break;
			}
			break;
		}
		if(val.hi==BGBDT_TAG_MCHAR)
			{ v=1; break; }
		if((val.hi>>24)==0x10)
			{ v=1; break; }
		v=0;
		break;
	case 4: case 5: case 6: case 7:
		v=1; break;
	case 8: case 9: case 10: case 11:
		v=0; break;
	default:
		v=0;
		break;
	}
	return(v);
}

static_inline int dtvIsSmallDoubleP(dtVal val)
{
	int v;
	switch(val.hi>>28)
	{
	case 0:
		v=0;
		break;
	case 1:
		if(val.hi==BGBDT_TAG_INT32)
			{ v=1; break; }
		if(val.hi==BGBDT_TAG_UINT32)
			{ v=1; break; }
		if(val.hi==BGBDT_TAG_FLOAT32)
			{ v=1; break; }
		if(val.hi==BGBDT_TAG_MCONST)
		{
			switch(val.lo)
			{
			case 0: case 1:
				v=0; break;
			case 2: case 3:
				v=1; break;
			default:	v=0; break;
			}
			break;
		}
		if(val.hi==BGBDT_TAG_MCHAR)
			{ v=1; break; }
		if((val.hi>>24)==0x10)
			{ v=1; break; }
		v=0;
		break;
	case 4: case 5: case 6: case 7:
		v=1; break;
	case 8: case 9: case 10: case 11:
		v=1; break;
	default:
		v=0;
		break;
	}
	return(v);
}

static_inline s64 dtvUnwrapLong(dtVal val)
{
	s64 v;
	switch(val.hi>>28)
	{
	case 0:
		v=0;
		break;
	case 1:
		if(val.hi==BGBDT_TAG_INT32)
			{ v=(s32)val.lo; break; }
		if(val.hi==BGBDT_TAG_UINT32)
			{ v=val.lo; break; }
		if(val.hi==BGBDT_TAG_FLOAT32)
			{ v=*(float *)(&val.lo); break; }
		if(val.hi==BGBDT_TAG_MCONST)
		{
			switch(val.lo)
			{
			case 0: case 1: case 2:
				v=0; break;
			case 3:		v=1; break;
			default:	v=0; break;
			}
			break;
		}
		if(val.hi==BGBDT_TAG_MCHAR)
			{ v=(s32)val.lo; break; }
		if(val.hi==BGBDT_TAG_MCONTIDX)
			{ v=(s32)val.lo; break; }
		if((val.hi>>24)==0x10)
			{ v=BGBDT_TagTy_DecodeRotLong(val); break; }

		v=0;
		break;
	case 4: case 5: case 6: case 7:
		v=dtvUnwrapLongF(val); break;
	case 8: case 9: case 10: case 11:
		v=dtvUnwrapDoubleF(val); break;
	default:
		v=0;
		break;
	}
	return(v);
}

static_inline double dtvUnwrapDouble(dtVal val)
{
	double v;
	switch(val.hi>>28)
	{
	case 0:
		v=0;
		break;
	case 1:
		if(val.hi==BGBDT_TAG_INT32)
			{ v=(s32)val.lo; break; }
		if(val.hi==BGBDT_TAG_UINT32)
			{ v=val.lo; break; }
		if(val.hi==BGBDT_TAG_FLOAT32)
			{ v=*(float *)(&val.lo); break; }
		if(val.hi==BGBDT_TAG_MCONST)
		{
			switch(val.lo)
			{
			case 0: case 1: case 2:
				v=0; break;
			case 3:		v=1; break;
			default:	v=0; break;
			}
			break;
		}
		if(val.hi==BGBDT_TAG_MCHAR)
			{ v=(s32)val.lo; break; }
		if((val.hi>>24)==0x10)
			{ v=BGBDT_TagTy_DecodeRotLong(val); break; }
		v=0;
		break;
	case 4: case 5: case 6: case 7:
		v=dtvUnwrapLongF(val); break;
	case 8: case 9: case 10: case 11:
		v=dtvUnwrapDoubleF(val); break;
	default:
		v=0;
		break;
	}
	return(v);
}

static_inline int dtvUnwrapInt(dtVal val)
{
	return(dtvUnwrapLong(val));
}

static_inline float dtvUnwrapFloat(dtVal val)
{
	return(dtvUnwrapDouble(val));
}

static_inline int dtvUnwrapChar(dtVal val)
{
	return(dtvUnwrapInt(val));
}

static_inline int dtvUnwrapContIdx(dtVal val)
{
	return(dtvUnwrapInt(val));
}


BS2VM_API dtVal BGBDT_TagArr_NewArray(int size, int bty);

static_inline dtVal dtvNewArray(int sz, int bty)
	{ return(BGBDT_TagArr_NewArray(sz, bty)); }

static_inline int dtvIsArrayP(dtVal arv)
{
	static int objty_arrhead=-1;
	if((arv.hi>>28)==12)
		return(1);
	if(objty_arrhead<0)
	{
		objty_arrhead=BGBDT_MM_GetIndexObjTypeName(
			"bgbdt_tagarrhead_t");
	}
	return(dtvCheckPtrTagP(arv, objty_arrhead));
}

static_inline int dtvIsArrayFP(dtVal arv)
{
	return((arv.hi>>28)==12);
}

static_inline int dtvArrayGetBaseType(dtVal arv)
{
	BGBDT_TagArrHead *arr;
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);
	return(arr->bty);
}

static_inline void *dtvArrayGetIndexAddr(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	byte *pv;
	int bx0, bx;
	
//	arr=dtvUnwrapPtrF(arv);
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx0=arv.hi&0x0FFFFFFF;
//	bx=idx+bx0+arr->base;
//	bx=bx0+idx*arr->str;
	bx=bx0+(idx<<arr->strsh);
//	if((bx<0) || (bx>=(arr->sz*arr->str)))
//	if((bx<0) || ((bx>>arr->strsh)>=arr->sz))
	if((bx<0) || (bx>=arr->scsz))
		{ return(NULL); }
	return(arr->data+bx);
}

static_inline dtVal dtvArrayAdjustOffset(dtVal arv, int idx)
{
	dtVal arv2;
	BGBDT_TagArrHead *arr;
	byte *pv;
	int bx0, bx1;
	
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx0=arv.hi&0x0FFFFFFF;
	bx1=bx0+(idx*arr->str);
//	if((bx1<0) || (bx1>=(arr->sz*arr->str)))
//	if((bx1<0) || ((bx1>>arr->strsh)>=arr->sz))
	if((bx1<0) || (bx1>=arr->scsz))
		{ return(DTV_UNDEFINED); }
	
	arv2.lo=arv.lo;
	arv2.hi=(0xC0000000UL)|bx1;
	return(arv2);
}


static_inline void *dtvArrayGetIndexAddrB1(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+idx;
	if((bx<0) || (bx>=arr->scsz))
		{ return(NULL); }
	return(arr->data+bx);
}

static_inline void *dtvArrayGetIndexAddrB2(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+(idx<<1);
	if((bx<0) || (bx>=arr->scsz))
		{ return(NULL); }
	return(arr->data+bx);
}

static_inline void *dtvArrayGetIndexAddrB4(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+(idx<<2);
	if((bx<0) || (bx>=arr->scsz))
		{ return(NULL); }
	return(arr->data+bx);
}

static_inline void *dtvArrayGetIndexAddrB8(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+(idx<<3);
	if((bx<0) || (bx>=arr->scsz))
		{ return(NULL); }
	return(arr->data+bx);
}

static_inline void *dtvArrayGetIndexAddrB16(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+(idx<<4);
	if((bx<0) || (bx>=arr->scsz))
		{ return(NULL); }
	return(arr->data+bx);
}

#if 1
static_inline void *dtvArrayGetIndexAddrB1F(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+idx;
	return(arr->data+bx);
}

static_inline void *dtvArrayGetIndexAddrB2F(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+(idx<<1);
	return(arr->data+bx);
}

static_inline void *dtvArrayGetIndexAddrB4F(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+(idx<<2);
	return(arr->data+bx);
}

static_inline void *dtvArrayGetIndexAddrB8F(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+(idx<<3);
	return(arr->data+bx);
}

static_inline void *dtvArrayGetIndexAddrB16F(dtVal arv, int idx)
{
	BGBDT_TagArrHead *arr;
	int bx;
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=(arv.hi&0x0FFFFFFF)+(idx<<4);
	return(arr->data+bx);
}
#endif

static_inline int dtvArrayGetSize(dtVal arv)
{
	BGBDT_TagArrHead *arr;
	byte *pv;
	int bx;
	
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=arv.hi&0x0FFFFFFF;
	return((arr->scsz-bx)>>arr->strsh);
//	return(arr->sz-(bx>>arr->strsh));
//	return(arr->sz-bx);
}

static_inline int dtvArrayGetOffset(dtVal arv)
{
	BGBDT_TagArrHead *arr;
	byte *pv;
	int bx;
	
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);
	bx=arv.hi&0x0FFFFFFF;
	return(bx>>arr->strsh);

//	return((arr->scsz-bx)>>arr->strsh);
//	return(arr->sz-(bx>>arr->strsh));
//	return(arr->sz-bx);
}

static_inline s32 dtvArrayGetIndexInt(dtVal arv, int idx)
	{ return(*(s32 *)dtvArrayGetIndexAddr(arv, idx)); }
static_inline s64 dtvArrayGetIndexLong(dtVal arv, int idx)
	{ return(*(s64 *)dtvArrayGetIndexAddr(arv, idx)); }
static_inline f32 dtvArrayGetIndexFloat(dtVal arv, int idx)
	{ return(*(f32 *)dtvArrayGetIndexAddr(arv, idx)); }
static_inline f64 dtvArrayGetIndexDouble(dtVal arv, int idx)
	{ return(*(f64 *)dtvArrayGetIndexAddr(arv, idx)); }

static_inline dtVal dtvArrayGetIndexDtVal(dtVal arv, int idx)
	{ return(*(dtVal *)dtvArrayGetIndexAddr(arv, idx)); }

static_inline int dtvArrayGetIndexSByte(dtVal arv, int idx)
	{ return(*(sbyte *)dtvArrayGetIndexAddr(arv, idx)); }
static_inline int dtvArrayGetIndexByte(dtVal arv, int idx)
	{ return(*(byte *)dtvArrayGetIndexAddr(arv, idx)); }
static_inline int dtvArrayGetIndexShort(dtVal arv, int idx)
	{ return(*(s16 *)dtvArrayGetIndexAddr(arv, idx)); }
static_inline int dtvArrayGetIndexUShort(dtVal arv, int idx)
	{ return(*(u16 *)dtvArrayGetIndexAddr(arv, idx)); }

static_inline void dtvArraySetIndexInt(dtVal arv, int idx, s32 val)
	{ (*(s32 *)dtvArrayGetIndexAddr(arv, idx))=val; }
static_inline void dtvArraySetIndexLong(dtVal arv, int idx, s64 val)
	{ (*(s64 *)dtvArrayGetIndexAddr(arv, idx))=val; }
static_inline void dtvArraySetIndexFloat(dtVal arv, int idx, f32 val)
	{ (*(f32 *)dtvArrayGetIndexAddr(arv, idx))=val; }
static_inline void dtvArraySetIndexDouble(dtVal arv, int idx, f64 val)
	{ (*(f64 *)dtvArrayGetIndexAddr(arv, idx))=val; }

static_inline void dtvArraySetIndexDtVal(dtVal arv, int idx, dtVal val)
	{ (*(dtVal *)dtvArrayGetIndexAddr(arv, idx))=val; }

static_inline void dtvArraySetIndexByte(dtVal arv, int idx, int val)
	{ (*(byte *)dtvArrayGetIndexAddr(arv, idx))=val; }
static_inline void dtvArraySetIndexShort(dtVal arv, int idx, int val)
	{ (*(s16 *)dtvArrayGetIndexAddr(arv, idx))=val; }


static_inline dtVal dtvArrayAdjustOffsetB1(dtVal arv, int idx)
{
	dtVal arv2;
	BGBDT_TagArrHead *arr;
	byte *pv;
	int bx0, bx1;
	
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);

	bx0=arv.hi&0x0FFFFFFF;
	bx1=bx0+idx;
//	if((bx1<0) || (bx1>=(arr->sz*arr->str)))
//	if((bx1<0) || ((bx1>>arr->strsh)>=arr->sz))
	if((bx1<0) || (bx1>=arr->scsz))
		{ return(DTV_UNDEFINED); }
	
	arv2.lo=arv.lo;
	arv2.hi=(0xC0000000UL)|bx1;
	return(arv2);
}

static_inline dtVal dtvArrayAdjustOffsetB2(dtVal arv, int idx)
{
	dtVal arv2;
	BGBDT_TagArrHead *arr;
	byte *pv;
	int bx0, bx1;
	
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);
	
	bx0=arv.hi&0x0FFFFFFF;
	bx1=bx0+(idx<<1);
//	if((bx1<0) || (bx1>=arr->sz))
//	if((bx1<0) || ((bx1>>arr->strsh)>=arr->sz))
	if((bx1<0) || (bx1>=arr->scsz))
		{ return(DTV_UNDEFINED); }
	
	arv2.lo=arv.lo;
	arv2.hi=(0xC0000000UL)|bx1;
	return(arv2);
}

static_inline dtVal dtvArrayAdjustOffsetB4(dtVal arv, int idx)
{
	dtVal arv2;
	BGBDT_TagArrHead *arr;
	byte *pv;
	int bx0, bx1;
	
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);
	
	bx0=arv.hi&0x0FFFFFFF;
	bx1=bx0+(idx<<2);
//	if((bx1<0) || (bx1>=arr->sz))
//	if((bx1<0) || ((bx1>>arr->strsh)>=arr->sz))
	if((bx1<0) || (bx1>=arr->scsz))
		{ return(DTV_UNDEFINED); }
	
	arv2.lo=arv.lo;
	arv2.hi=(0xC0000000UL)|bx1;
	return(arv2);
}

static_inline dtVal dtvArrayAdjustOffsetB8(dtVal arv, int idx)
{
	dtVal arv2;
	BGBDT_TagArrHead *arr;
	byte *pv;
	int bx0, bx1;
	
//	arr=BGBDT_MM_GetDataPtrForObjId(arv.lo);
	arr=DTV_GetDataPtrForObjId(arv.lo);
	
	bx0=arv.hi&0x0FFFFFFF;
	bx1=bx0+(idx<<3);
//	if((bx1<0) || (bx1>=arr->sz))
//	if((bx1<0) || ((bx1>>arr->strsh)>=arr->sz))
	if((bx1<0) || (bx1>=arr->scsz))
		{ return(DTV_UNDEFINED); }
	
	arv2.lo=arv.lo;
	arv2.hi=(0xC0000000UL)|bx1;
	return(arv2);
}


static_inline void *dtvTagPtrGetIndexAddrB1F(dtVal arv, int idx)
	{ return(((byte *)dtvUnwrapPtrF(arv))+idx); }
static_inline void *dtvTagPtrGetIndexAddrB2F(dtVal arv, int idx)
	{ return(((byte *)dtvUnwrapPtrF(arv))+(idx<<1)); }
static_inline void *dtvTagPtrGetIndexAddrB4F(dtVal arv, int idx)
	{ return(((byte *)dtvUnwrapPtrF(arv))+(idx<<2)); }
static_inline void *dtvTagPtrGetIndexAddrB8F(dtVal arv, int idx)
	{ return(((byte *)dtvUnwrapPtrF(arv))+(idx<<3)); }
static_inline void *dtvTagPtrGetIndexAddrB16F(dtVal arv, int idx)
	{ return(((byte *)dtvUnwrapPtrF(arv))+(idx<<4)); }


static_inline void *dtvUnwrapPtr(dtVal val)
{
	if((val.hi>>28)==0)
		return(dtvUnwrapPtrF(val));
	if((val.hi>>28)==BGBDT_HTAG_TAGARR)
		return(dtvArrayGetIndexAddr(val, 0));
	if((val.hi>>28)==BGBDT_HTAG_TTPTR)
		return(dtvUnwrapPtrF(val));
	if((val.hi>>28)==BGBDT_HTAG_PTR60)
		return(dtvUnwrapPtr60F(val));
	return(NULL);
}


static_inline bool dtvIsVec2fP(dtVal val)
	{ return((val.hi>>28)==0xD); }

static_inline dtVal dtvWrapVec2f(float x, float y)
{
	dtVal c;
	u32 ix, iy;
	ix=*(u32 *)(&x);
	iy=*(u32 *)(&y);
	ix=(ix+2)>>2;
	iy=(iy+2)>>2;
	c.vi=ix|(((u64)iy)<<30)|0xD000000000000000ULL;
	return(c);
}

static_inline dtVal dtvWrapVec2fv(float *fv)
	{ return(dtvWrapVec2f(fv[0], fv[1])); }

static_inline void dtvUnwrapVec2fv(dtVal v, float *fv)
{
	u32 ix, iy;
	ix=v.lo<<2;
	iy=v.vi>>28;
	fv[0]=*(float *)(&ix);
	fv[1]=*(float *)(&iy);
}

#if 0
static_inline dtVal dtvWrapVec2v(vec2 v)
	{ return(dtvWrapVec2f(v2x(v), v2y(v))); }

static_inline vec2 dtvUnwrapVec2v(dtVal v)
{
	u32 ix, iy;
	float fx, fy;
	ix=v.lo<<2;
	iy=v.vi>>28;
	fx=*(float *)(&ix);
	fy=*(float *)(&iy);
	return(vec2(fx, fy));
}
#endif


static_inline bool dtvIsCplxfP(dtVal val)
	{ return((val.hi>>24)==0x11); }

static_inline dtVal dtvWrapCplxf(float x, float y)
{
	dtVal c;
	u32 ix, iy;
	ix=*(u32 *)(&x);
	iy=*(u32 *)(&y);
	ix=(ix+7)>>4;
	iy=(iy+7)>>4;
	c.vi=ix|(((u64)iy)<<28)|0x1100000000000000ULL;
	return(c);
}

static_inline dtVal dtvWrapCplxfv(float *fv)
	{ return(dtvWrapCplxf(fv[0], fv[1])); }

static_inline void dtvUnwrapCplxfv(dtVal v, float *fv)
{
	u32 ix, iy;
	ix=v.lo<<4;
	iy=v.vi>>24;
	fv[0]=*(float *)(&ix);
	fv[1]=*(float *)(&iy);
}

