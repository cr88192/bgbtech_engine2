/*
Instance Layout:
  ClsInfo, data...
 */

typedef struct _dtcobject_s *dtcObject;		//opaque

typedef struct BGBDTC_SlotInfo_s *dtcField;
typedef struct BGBDTC_ClassInfo_s *dtcClass;

typedef struct BGBDTC_SlotInfo_s BGBDTC_SlotInfo;
typedef struct BGBDTC_ClassInfo_s BGBDTC_ClassInfo;

struct BGBDTC_SlotInfo_s {
char *name;
char *sig;
int offs;

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
int szdata;
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