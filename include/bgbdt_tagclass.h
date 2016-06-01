/*
Instance Layout:
  ClsInfo, data...
 */

#define BGBDTC_STY_UNKNOWN	0		//unknown
#define BGBDTC_STY_FIELD	1		//instance field
#define BGBDTC_STY_METHOD	2		//virtual method

#define BGBDTC_CTY_UNKNOWN	0		//unknown
#define BGBDTC_CTY_CLASS	1		//class
#define BGBDTC_CTY_STRUCT	2		//structure
#define BGBDTC_CTY_IFACE	3		//interface

typedef struct _dtcobject_s *dtcObject;		//opaque

typedef struct BGBDTC_SlotInfo_s *dtcField;
typedef struct BGBDTC_SlotInfo_s *dtcMethod;
typedef struct BGBDTC_ClassInfo_s *dtcClass;

typedef struct BGBDTC_SlotInfo_s BGBDTC_SlotInfo;
typedef struct BGBDTC_ClassInfo_s BGBDTC_ClassInfo;

struct BGBDTC_SlotInfo_s {
char *name;			//name
char *sig;			//signature
int offs;			//offset in object body
byte slotty;		//slot type
dtVal init;			//initial value
s64 nameh;

void *(*GetPtr)(dtcObject obj, dtcField fi);

s32 (*GetI)(dtcObject obj, dtcField fi);
s64 (*GetL)(dtcObject obj, dtcField fi);
f32 (*GetF)(dtcObject obj, dtcField fi);
f64 (*GetD)(dtcObject obj, dtcField fi);
dtVal (*GetA)(dtcObject obj, dtcField fi);

void (*SetI)(dtcObject obj, dtcField fi, s32 v);
void (*SetL)(dtcObject obj, dtcField fi, s64 v);
void (*SetF)(dtcObject obj, dtcField fi, f32 v);
void (*SetD)(dtcObject obj, dtcField fi, f64 v);
void (*SetA)(dtcObject obj, dtcField fi, dtVal v);
};

struct BGBDTC_ClassInfo_s {
BGBDTC_ClassInfo *next;
BGBDTC_ClassInfo *super;
char *qname;				//qname, for public visible classes
int sqid, gix;				//image and global ID

BGBDTC_SlotInfo **slots;
int nslots, mslots;
int szdata, aldata;
byte clean;
byte clsty;

byte *cldata;				//class data
int szcldata;				//size of class data
};

static_inline s32 dtcGetI(dtcObject obj, dtcField fi)
	{ return(fi->GetI(obj, fi)); }
static_inline s64 dtcGetL(dtcObject obj, dtcField fi)
	{ return(fi->GetL(obj, fi)); }
static_inline f32 dtcGetF(dtcObject obj, dtcField fi)
	{ return(fi->GetF(obj, fi)); }
static_inline f64 dtcGetD(dtcObject obj, dtcField fi)
	{ return(fi->GetD(obj, fi)); }
static_inline dtVal dtcGetA(dtcObject obj, dtcField fi)
	{ return(fi->GetA(obj, fi)); }

static_inline void dtcSetI(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetI(obj, fi, v); }
static_inline void dtcSetL(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetL(obj, fi, v); }
static_inline void dtcSetF(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetF(obj, fi, v); }
static_inline void dtcSetD(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetD(obj, fi, v); }
static_inline void dtcSetA(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetA(obj, fi, v); }

static_inline void *dtcGetPtr(dtcObject obj, dtcField fi)
	{ return(fi->GetPtr(obj, fi)); }

static_inline s32 dtcVaGetI(dtVal obj, dtcField fi)
	{ return(fi->GetI(dtvUnwrapPtr(obj), fi)); }
static_inline s64 dtcVaGetL(dtVal obj, dtcField fi)
	{ return(fi->GetL(dtvUnwrapPtr(obj), fi)); }
static_inline f32 dtcVaGetF(dtVal obj, dtcField fi)
	{ return(fi->GetF(dtvUnwrapPtr(obj), fi)); }
static_inline f64 dtcVaGetD(dtVal obj, dtcField fi)
	{ return(fi->GetD(dtvUnwrapPtr(obj), fi)); }
static_inline dtVal dtcVaGetA(dtVal obj, dtcField fi)
	{ return(fi->GetA(dtvUnwrapPtr(obj), fi)); }

static_inline void dtcVaSetI(dtVal obj, dtcField fi, s32 v)
	{ fi->SetI(dtvUnwrapPtr(obj), fi, v); }
static_inline void dtcVaSetL(dtVal obj, dtcField fi, s64 v)
	{ fi->SetL(dtvUnwrapPtr(obj), fi, v); }
static_inline void dtcVaSetF(dtVal obj, dtcField fi, f32 v)
	{ fi->SetF(dtvUnwrapPtr(obj), fi, v); }
static_inline void dtcVaSetD(dtVal obj, dtcField fi, f64 v)
	{ fi->SetD(dtvUnwrapPtr(obj), fi, v); }
static_inline void dtcVaSetA(dtVal obj, dtcField fi, dtVal v)
	{ fi->SetA(dtvUnwrapPtr(obj), fi, v); }

static_inline void *dtcVaGetPtr(dtVal obj, dtcField fi)
	{ return(fi->GetPtr(dtvUnwrapPtr(obj), fi)); }


BTEIFGL_API s32 BGBDTC_GetCacheNameI(dtcObject obj,
	dtcField *rfi, char *qn, char *fn);
BTEIFGL_API s64 BGBDTC_GetCacheNameL(dtcObject obj,
	dtcField *rfi, char *qn, char *fn);
BTEIFGL_API f32 BGBDTC_GetCacheNameF(dtcObject obj,
	dtcField *rfi, char *qn, char *fn);
BTEIFGL_API f64 BGBDTC_GetCacheNameD(dtcObject obj,
	dtcField *rfi, char *qn, char *fn);
BTEIFGL_API dtVal BGBDTC_GetCacheNameA(dtcObject obj,
	dtcField *rfi, char *qn, char *fn);
BTEIFGL_API void *BGBDTC_GetCacheNamePtr(dtcObject obj,
	dtcField *rfi, char *qn, char *fn);

BTEIFGL_API int BGBDTC_SetCacheNameI(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, s32 val);
BTEIFGL_API int BGBDTC_SetCacheNameL(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, s64 val);
BTEIFGL_API int BGBDTC_SetCacheNameF(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, f32 val);
BTEIFGL_API int BGBDTC_SetCacheNameD(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, f64 val);
BTEIFGL_API int BGBDTC_SetCacheNameA(dtcObject obj,
	dtcField *rfi, char *qn, char *fn, dtVal val);


static_inline s32 dtcGetCacheNameI(dtcObject obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameI(obj, rfi, qn, fn)); }
static_inline s64 dtcGetCacheNameL(dtcObject obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameL(obj, rfi, qn, fn)); }
static_inline f32 dtcGetCacheNameF(dtcObject obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameF(obj, rfi, qn, fn)); }
static_inline f64 dtcGetCacheNameD(dtcObject obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameD(obj, rfi, qn, fn)); }
static_inline dtVal dtcGetCacheNameA(dtcObject obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameA(obj, rfi, qn, fn)); }
static_inline void *dtcGetCacheNamePtr(dtcObject obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNamePtr(obj, rfi, qn, fn)); }

static_inline s32 dtcVaGetCacheNameI(dtVal obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameI(dtvUnwrapPtr(obj), rfi, qn, fn)); }
static_inline s64 dtcVaGetCacheNameL(dtVal obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameL(dtvUnwrapPtr(obj), rfi, qn, fn)); }
static_inline f32 dtcVaGetCacheNameF(dtVal obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameF(dtvUnwrapPtr(obj), rfi, qn, fn)); }
static_inline f64 dtcVaGetCacheNameD(dtVal obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameD(dtvUnwrapPtr(obj), rfi, qn, fn)); }
static_inline dtVal dtcVaGetCacheNameA(dtVal obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNameA(dtvUnwrapPtr(obj), rfi, qn, fn)); }
static_inline void *dtcVaGetCacheNamePtr(dtVal obj,
		dtcField *rfi, char *qn, char *fn)
	{ return(BGBDTC_GetCacheNamePtr(dtvUnwrapPtr(obj), rfi, qn, fn)); }

static_inline void dtcSetCacheNameI(dtcObject obj,
		dtcField *rfi, char *qn, char *fn, s32 v)
	{ BGBDTC_SetCacheNameI(obj, rfi, qn, fn, v); }
static_inline void dtcSetCacheNameL(dtcObject obj,
		dtcField *rfi, char *qn, char *fn, s64 v)
	{ BGBDTC_SetCacheNameL(obj, rfi, qn, fn, v); }
static_inline void dtcSetCacheNameF(dtcObject obj,
		dtcField *rfi, char *qn, char *fn, f32 v)
	{ BGBDTC_SetCacheNameF(obj, rfi, qn, fn, v); }
static_inline void dtcSetCacheNameD(dtcObject obj,
		dtcField *rfi, char *qn, char *fn, f64 v)
	{ BGBDTC_SetCacheNameD(obj, rfi, qn, fn, v); }
static_inline void dtcSetCacheNameA(dtcObject obj,
		dtcField *rfi, char *qn, char *fn, dtVal v)
	{ BGBDTC_SetCacheNameA(obj, rfi, qn, fn, v); }

static_inline void dtcVaSetCacheNameI(dtVal obj,
		dtcField *rfi, char *qn, char *fn, s32 v)
	{ BGBDTC_SetCacheNameI(dtvUnwrapPtr(obj), rfi, qn, fn, v); }
static_inline void dtcVaSetCacheNameL(dtVal obj,
		dtcField *rfi, char *qn, char *fn, s64 v)
	{ BGBDTC_SetCacheNameL(dtvUnwrapPtr(obj), rfi, qn, fn, v); }
static_inline void dtcVaSetCacheNameF(dtVal obj,
		dtcField *rfi, char *qn, char *fn, f32 v)
	{ BGBDTC_SetCacheNameF(dtvUnwrapPtr(obj), rfi, qn, fn, v); }
static_inline void dtcVaSetCacheNameD(dtVal obj,
		dtcField *rfi, char *qn, char *fn, f64 v)
	{ BGBDTC_SetCacheNameD(dtvUnwrapPtr(obj), rfi, qn, fn, v); }
static_inline void dtcVaSetCacheNameA(dtVal obj,
		dtcField *rfi, char *qn, char *fn, dtVal v)
	{ BGBDTC_SetCacheNameA(dtvUnwrapPtr(obj), rfi, qn, fn, v); }

typedef struct BGBDTC_X128_s BGBDTC_X128;

struct BGBDTC_X128_s {
struct {
	u32 a;
	u32 b;
	u32 c;
	u32 d;
};
struct {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
};
struct {
	u64 la;
	u64 lb;
};
struct {
	f64 dx;
	f64 dy;
};
};
