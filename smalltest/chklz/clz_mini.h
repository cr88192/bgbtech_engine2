#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
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

#ifndef UINT_T
#define UINT_T
typedef unsigned int uint;
#endif

#if defined(__x86_64__) || defined(_M_X64)
#ifndef X86_64
#define X86_64
#endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#ifndef X86
#define X86
#endif
#endif

#if defined(__arm__) || defined(_M_ARM)
#ifndef ARM
#define ARM
#endif
#endif


#if !defined(LITTLEENDIAN) && !defined(BIGENDIAN)
#if defined(X86) || defined(X86_64) || defined(ARM)
#define LITTLEENDIAN
#endif
#endif

typedef struct BGBDT_RiceContext_s BGBDT_RiceContext;

struct BGBDT_RiceContext_s {
u32 mark1;

BGBDT_RiceContext *next;			//next context

byte *bs_ct;						//bitstream output
byte *bs_cts;						//bitstream output start
byte *bs_cte;						//bitstream output end

byte *bs_cs;						//bitstream input
byte *bs_cse;						//bitstream input end

u32 bs_win;							//bitstream window
int bs_pos;							//bitstream window position
int bs_bits;						//bitstream total bits

byte *blks;							//blocks buffer
byte *lblks;						//last-frame blocks buffer

byte cmdwin[256];					//command window
short cmdidx[256];					//command window index
byte cmdwpos;						//command window position

u32 mark2;

byte lzwin[256];					//command window
byte lzidx[256];					//command window index
byte lzwpos;						//command window position

int lzstat[256];

// byte *lzhash[256];					//LZ hash
byte *lzhash[4096];					//LZ hash

int lzctrl;

int stat_kr, stat_ks, stat_kl, stat_kd;
int stat_nr, stat_ns, stat_nl, stat_nd;

int (*ReadAdRiceLL)(BGBDT_RiceContext *ctx, int *rk);
void (*WriteAdRiceLL)(BGBDT_RiceContext *ctx, int val, int *rk);

int (*ReadAdDist)(BGBDT_RiceContext *ctx, int *rk);
void (*WriteAdDist)(BGBDT_RiceContext *ctx, int val, int *rk);

int (*ReadSym)(BGBDT_RiceContext *ctx, int *rk);
void (*WriteSym)(BGBDT_RiceContext *ctx, int sym, int *rk);

};

void *frgl_malloc(int sz);

