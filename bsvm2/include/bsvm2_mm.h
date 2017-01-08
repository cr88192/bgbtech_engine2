/*
Cell Allocator

Cell SObj, 16 byte cells
Cell MObj, 256 byte cells

SObj Chunks: 16MB
  Size Range: 16B-16kB
MObj Chunks: 16MB
  16kB-1MB
LObj: VirtualAlloc

Cells each have 1 byte.
  0: Free
  1: Head
  2: Data
  3: Resv

LObj managed via AVL Trees.

*/

#ifndef UNDEFINED
#define UNDEFINED ((void *)(-1))
#endif

// #define BGBDT_MM_SMALLHEAP

#ifdef __EMSCRIPTEN__
#ifndef BGBDT_MM_SMALLHEAP
#define BGBDT_MM_SMALLHEAP
#endif
#endif

#define BGBDT_MM_CHKMAG1		0x31415927
#define BGBDT_MM_CHKMAG2		0x72951413

#ifndef BGBDT_MM_SMALLHEAP

#define BGBDT_MM_SOBJ_CHKSHL	20
#define BGBDT_MM_SOBJ_CELSHL	 4
#define BGBDT_MM_SOBJ_TOTSHL	24
#define BGBDT_MM_SOBJ_CELMSK	15
#define BGBDT_MM_SOBJ_OBJLIM	(1<<14)

#define BGBDT_MM_MOBJ_CHKSHL	16
#define BGBDT_MM_MOBJ_CELSHL	 8
#define BGBDT_MM_MOBJ_TOTSHL	24
#define BGBDT_MM_MOBJ_CELMSK	255
#define BGBDT_MM_MOBJ_OBJLIM	(1<<20)

#else

#define BGBDT_MM_SOBJ_CHKSHL	18
#define BGBDT_MM_SOBJ_CELSHL	 4
#define BGBDT_MM_SOBJ_TOTSHL	22
#define BGBDT_MM_SOBJ_CELMSK	15
#define BGBDT_MM_SOBJ_OBJLIM	(1<<14)

#define BGBDT_MM_MOBJ_CHKSHL	14
#define BGBDT_MM_MOBJ_CELSHL	 8
#define BGBDT_MM_MOBJ_TOTSHL	22
#define BGBDT_MM_MOBJ_CELMSK	255
#define BGBDT_MM_MOBJ_OBJLIM	(1<<18)

#endif

#define dtmAlloc(ty, sz)				\
	BGBDT_MM_AllocLLn(ty, sz, __FILE__, __LINE__)
#define dtmAllocLLn(ty, sz, fn, ln)		\
	BGBDT_MM_AllocLLn(ty, sz, fn, ln)
#define dtmFree(ptr)					\
	BGBDT_MM_Free(ptr)
#define dtmFreeAny(ptr)					\
	BGBDT_MM_FreeAny(ptr)

#define dtmMalloc(sz)				\
	BGBDT_MM_AllocLLn("_raw_t", sz, __FILE__, __LINE__)

#define dtmRealloc(ptr, sz)				\
	BGBDT_MM_ReallocLLn(ptr, sz, __FILE__, __LINE__)
#define dtmReallocLLn(ptr, sz, fn, ln)		\
	BGBDT_MM_ReallocLLn(ptr, sz, fn, ln)

#define dtmGetSize(ptr)			BGBDT_MM_GetSize(ptr)
#define dtmGetType(ptr)			BGBDT_MM_GetTypeName(ptr)
#define dtmGetTypeID(ptr)		BGBDT_MM_GetTypeID(ptr)
#define dtmGetBase(ptr)			BGBDT_MM_GetBase(ptr)

typedef struct BGBDT_MM_ChunkInfo_s BGBDT_MM_ChunkInfo;
typedef struct BGBDT_MM_LObjInfo_s BGBDT_MM_LObjInfo;

typedef struct BGBDT_MM_ObjHead_s BGBDT_MM_ObjHead;
typedef struct BGBDT_MM_ObjTypeVtable_s BGBDT_MM_ObjTypeVtable;
typedef struct BGBDT_MM_ObjTypeInfo_s BGBDT_MM_ObjTypeInfo;
typedef struct BGBDT_MM_ObjLLnInfo_s BGBDT_MM_ObjLLnInfo;

typedef struct BGBDT_MM_RegionInfo_s BGBDT_MM_RegionInfo;

typedef struct BGBDT_WorkItem_s BGBDT_WorkItem;


typedef struct th_context_s thContext;
typedef struct BIPRO_RegisterState_s BIPRO_RegisterState;

#define bgbdt_mm_max(x, y) (((x)>(y))?(x):(y))

struct BGBDT_MM_ChunkInfo_s {
u32 magic1;
byte *data;		//data region
byte *edata;	//end of data region
byte *map;		//map region
int ncell;		//number of cells
int shcell;		//cell shift
int rov;
u32 magic2;
};

struct BGBDT_MM_LObjInfo_s {
byte *data;		//object base
byte *edata;	//end of object
int lno;		//left subtree
int rno;		//right subtree
int depth;		//node depth
};

struct BGBDT_MM_ObjHead_s {
u32 sz;		//object size, up to 4GB
u16 ty;		//object type, low 12 bits, high 4=ref cnt
u16 lln;	//src file/line number, low 12, high 4=check
u32 mrgn;	//memory region, 0-11=Level, 12-23=Region ID
u32 pad2;	//reserved
};

struct BGBDT_MM_ObjTypeVtable_s {
void (*Destroy)(void *ptr);
};

struct BGBDT_MM_ObjTypeInfo_s {
char *name;
int tyid;
BGBDT_MM_ObjTypeVtable *vt;
};

struct BGBDT_MM_ObjLLnInfo_s {
char *fn;
int ln;
};

struct BGBDT_MM_RegionInfo_s {
BGBDT_MM_RegionInfo *next;
u16 rgnid;
int rifree;

int rilvl[4096];

void **ri_ptr;
int *ri_next;
int ri_nptr, ri_mptr;
};

typedef struct {
void *(*fopen_fp)(char *name, char *mode);
void (*fclose_fp)(void *fd);
int (*fread_fp)(void *buf, int m, int n, void *fd);
int (*fwrite_fp)(void *buf, int m, int n, void *fd);
int (*feof_fp)(void *fd);
s64 (*ftell_fp)(void *fd);
int (*fseek_fp)(void *fd, s64 pos, int rel);
}BGBDT_MM_IOFuncs;


typedef struct BGBDT_TagStrTab_s BGBDT_TagStrTab;
typedef struct BGBDT_TagStrTab16_s BGBDT_TagStrTab16;

struct BGBDT_TagStrTab_s {
char *strtyn;
char *pstrtab[256];
char *strtab;
char *strtabe;
char *estrtab;
char **strhash;
int npstrtab;
};

struct BGBDT_TagStrTab16_s {
char *strtyn;
u16 *pstrtab[256];
u16 *strtab;
u16 *strtabe;
u16 *estrtab;
u16 **strhash;
int npstrtab;
};

struct BGBDT_WorkItem_s {
volatile BGBDT_WorkItem *next;
// BTIC4B_Context tctx;
void *data;
volatile byte done;
int (*DoWork)(BGBDT_WorkItem *item);
};
