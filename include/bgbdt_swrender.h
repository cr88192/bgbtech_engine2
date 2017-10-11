typedef u16 bgbdt_swpixel;
typedef u16 bgbdt_swzval;
typedef u32 bgbdt_fbpixel;

typedef struct BGBDT_SwContext_s BGBDT_SwContext;
typedef struct BGBDT_SwSpan_s BGBDT_SwSpan;
typedef struct BGBDT_SwTriVert_s BGBDT_SwTriVert;
typedef struct BGBDT_SwTriangle_s BGBDT_SwTriangle;
typedef struct BGBDT_SwTexture_s BGBDT_SwTexture;

struct BGBDT_SwContext_s {
bgbdt_fbpixel *fbuf;
bgbdt_swzval *zbuf;
byte *eozbuf;
int xs;
int ys;

BGBDT_SwSpan *freespan;
BGBDT_SwTriangle *freetris;
BGBDT_SwTexture *texlist;

int lim_alloc_span;
int lim_alloc_tris;

BGBDT_SwSpan *splst[4096];
BGBDT_SwTriangle *trilst;

float xform[16];		//modelview*projection
BGBDT_SwTexture *cur_tex;

float mat_mdlv[16];		//modelview
float mat_proj[16];		//projection
byte matmode;
byte ztest, fcull;
byte en_tex2d;
byte st_ilace;
byte st_ilace_msk;

u32 texidmap[1024];
u16 texidrov;

bgbdt_swzval mzbuf_zbuf[16*16];
u16 mzbuf_cmax[16*16];
u16 mzbuf_ccnt[16*16];

s16 viewport_x;
s16 viewport_y;
s16 viewport_xs;
s16 viewport_ys;

s16 vclip_minx;
s16 vclip_maxx;
s16 vclip_miny;
s16 vclip_maxy;

// bgbdt_swpixel *tex_buf;
// int tex_xshr;
// int tex_yshr;

void *xyzptr_ptr;
void *stptr_ptr;
void *rgbptr_ptr;
byte xyzptr_size;
byte stptr_size;
byte rgbptr_size;
byte xyzptr_stride;
byte stptr_stride;
byte rgbptr_stride;
u16 xyzptr_type;
u16 stptr_type;
u16 rgbptr_type;

int oq_cnt;
int oq_id;
int oq_cntarr[4096];

void *os_hdc;
void *os_hbmp;
void *os_rgba;
void *os_hdcdib;
void *os_hdlsel;
};

struct BGBDT_SwSpan_s {
BGBDT_SwSpan *next;
bgbdt_swpixel *tex_buf;
u32 ca, cb;
int sa, sb;
int ta, tb;
s16 xa, xb;
s16 za, zb;
s16 y;
byte tex_xshr;
byte tex_yshr;
};

struct BGBDT_SwTriVert_s {
int x, y, z, s, t;
u32 clr;
};

struct BGBDT_SwTriangle_s {
BGBDT_SwTriangle *next;
BGBDT_SwTriVert v0, v1, v2;
BGBDT_SwTexture *tex;
};

struct BGBDT_SwTexture_s {
BGBDT_SwTexture *next;
bgbdt_swpixel *mbuf[16];
int txs, tys;
int xshl, yshl;
int id;
};
