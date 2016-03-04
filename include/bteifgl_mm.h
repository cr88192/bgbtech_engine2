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

#define dtmAlloc(ty, sz)				\
	BGBDT_MM_AllocLLn(ty, sz, __FILE__, __LINE__)
#define dtmAllocLLn(ty, sz, fn, ln)		\
	BGBDT_MM_AllocLLn(ty, sz, fn, ln)
#define dtmFree(ptr)					\
	BGBDT_MM_Free(ptr)
#define dtmFreeAny(ptr)					\
	BGBDT_MM_FreeAny(ptr)


typedef struct BGBDT_MM_ChunkInfo_s BGBDT_MM_ChunkInfo;
typedef struct BGBDT_MM_LObjInfo_s BGBDT_MM_LObjInfo;

typedef struct BGBDT_MM_ObjHead_s BGBDT_MM_ObjHead;
typedef struct BGBDT_MM_ObjTypeInfo_s BGBDT_MM_ObjTypeInfo;
typedef struct BGBDT_MM_ObjLLnInfo_s BGBDT_MM_ObjLLnInfo;

struct BGBDT_MM_ChunkInfo_s {
byte *data;		//data region
byte *edata;	//end of data region
byte *map;		//map region
int ncell;		//number of cells
int shcell;		//cell shift
int rov;
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
u32 pad1;	//reserved
u32 pad2;	//reserved
};

struct BGBDT_MM_ObjTypeInfo_s {
char *name;
int tyid;
};

struct BGBDT_MM_ObjLLnInfo_s {
char *fn;
int ln;
};
