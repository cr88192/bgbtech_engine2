#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

//#include <GL/gl.h>

#ifndef BTEIFGL_H
#define BTEIFGL_H

#include <bteifgl_conf.h>

#ifndef GLES

#include <GL/gl.h>
#include <GL_3/glext.h>

#ifdef _WIN32
#include <GL_3/wglext.h>
#endif

#endif

#ifndef frgl_state
#define frgl_state	((FRGL_State *)FRGL_GetState())
#endif

#ifndef printf
#define printf frgl_printf
#endif

#ifndef M_PI
#define M_PI		3.1415926535897932384626433832795
#endif

#define FRGL_MAKETCC(a, b)	((a)+((b)<<8))

#define FRGL_MAKEFCC(a, b, c, d)	((a)+((b)<<8)+((c)<<16)+((d)<<24))
#define FRGL_UNHEX_NIBBLE(a)		( \
	(((a)>='0')&&((a)<='9'))?((a)-'0') : \
	(((a)>='A')&&((a)<='F'))?((a)-'A'+10) : \
	(((a)>='a')&&((a)<='f'))?((a)-'a'+10) : 0)
#define FRGL_HEX_NIBBLE(a)		(((a)<10)?((a)+'0'):((a)+'A'))

#define FRGL_MAKEHEXFCC(a)	FRGL_MAKEFCC( \
					FRGL_HEX_NIBBLE(((a)>>12)&0xf), \
					FRGL_HEX_NIBBLE(((a)>>8)&0xf), \
					FRGL_HEX_NIBBLE(((a)>>4)&0xf), \
					FRGL_HEX_NIBBLE((a)&0xf) \
					)

#define FRGL_FCC_GETA(a)	(((a)    )&255)
#define FRGL_FCC_GETB(a)	(((a)>> 8)&255)
#define FRGL_FCC_GETC(a)	(((a)>>16)&255)
#define FRGL_FCC_GETD(a)	(((a)>>24)&255)

#define FRGL_FONT_BOLD			1
#define FRGL_FONT_ITALIC		2
#define FRGL_FONT_UNDERLINE		4
#define FRGL_FONT_OVERLINE		8
#define FRGL_FONT_STRIKEOUT		16
#define FRGL_FONT_BLINK			32
#define FRGL_FONT_SUBSCRIPT		64
#define FRGL_FONT_SUPERSCRIPT	128

//WARNING: These constants are magic (Align with those in OpenGL)
#define FRGL_POINTS						0
#define FRGL_LINES						1
#define FRGL_LINE_LOOP					2
#define FRGL_LINE_STRIP					3
#define FRGL_TRIANGLES					4
#define FRGL_TRIANGLE_STRIP				5
#define FRGL_TRIANGLE_FAN				6
#define FRGL_QUADS						7
#define FRGL_QUAD_STRIP					8
#define FRGL_POLYGON					9

#define	BGBBTJ_JPG_RGBA			0	//RGBA(32)
#define	BGBBTJ_JPG_RGB			1	//RGB(24)
#define	BGBBTJ_JPG_BGRA			3	//BGRA(32)
#define	BGBBTJ_JPG_BGR			4	//BGR(24)
#define	BGBBTJ_JPG_YUVA			5	//YUVA(32)
#define	BGBBTJ_JPG_YUV			6	//YUV(24)
#define	BGBBTJ_JPG_Y			7	//Y(8)
#define	BGBBTJ_JPG_YA			8	//YA(16)
#define	BGBBTJ_JPG_YUV422		9	//YYUV
#define	BGBBTJ_JPG_YUV420		10	//YYU/YYV (YYYYUV)

#define	BGBBTJ_JPG_YYYA			11
#define	BGBBTJ_JPG_ARGB			12
#define	BGBBTJ_JPG_ABGR			13
#define	BGBBTJ_JPG_UYVY			14	//UYVY Ordering (4:2:2)
#define	BGBBTJ_JPG_YUYV			15	//YUYV Ordering (4:2:2)

#define	BGBBTJ_JPG_BC1			16	//BC1 / DXT1
#define	BGBBTJ_JPG_BC2			17	//BC2 / DXT3
#define	BGBBTJ_JPG_BC3			18	//BC3 / DXT5
#define	BGBBTJ_JPG_BC4			19	//BC4
#define	BGBBTJ_JPG_BC5			20	//BC5
#define	BGBBTJ_JPG_BC6			21	//BC6H (Signed)
#define	BGBBTJ_JPG_BC7			22	//BC7
#define	BGBBTJ_JPG_BC1F			23	//BC1 / DXT1
#define	BGBBTJ_JPG_BC3F			24	//BC3 / DXT5
#define	BGBBTJ_JPG_BC1A			25	//BC1 / DXT1
#define	BGBBTJ_JPG_BC3_UVAY		26	//DXT5-UVAY
#define	BGBBTJ_JPG_BC7_SRGB		27	//BC7 (SRGBA)
#define	BGBBTJ_JPG_BC6_UF16		28	//BC6H (Unsigned)
#define	BGBBTJ_JPG_BC4A			29	//BC4 (Alpha Only)

#define	BGBBTJ_JPG_RGBA_Q11_4	32	//RGBA(64 bit, fixed Q11.4)
#define	BGBBTJ_JPG_RGB_Q11_4	33	//RGB(48 bit, fixed Q11.4)
#define	BGBBTJ_JPG_RGBA_F32		34	//RGBA(128 bit, float32)
#define	BGBBTJ_JPG_RGB_F32		35	//RGB(96 bit, float32)
#define	BGBBTJ_JPG_RGBA_F16		36	//RGBA(64 bit, float16)
#define	BGBBTJ_JPG_RGB_F16		37	//RGB(48 bit, float16)
#define	BGBBTJ_JPG_RGBA_LS16	38	//RGBA(64 bit, log-scale)
#define	BGBBTJ_JPG_RGB_LS16		39	//RGB(48 bit, log-scale)

#define	BGBBTJ_JPG_RGBA_VF		40	//RGBA (Vertical Flip)
#define	BGBBTJ_JPG_RGB_VF		41	//RGB (Vertical Flip)
#define	BGBBTJ_JPG_RGBA_F16_VF	42	//RGBA(64 bit, float16)
#define	BGBBTJ_JPG_RGB_F16_VF	43	//RGB(48 bit, float16)

#define	BGBBTJ_JPG_RGBX			44	//RGBx(32)
#define	BGBBTJ_JPG_BGRX			45	//BGRx(32)
#define	BGBBTJ_JPG_RGBX_VF		46	//RGBx(32)
#define	BGBBTJ_JPG_BGRX_VF		47	//BGRx(32)

#define	BGBBTJ_JPG_BC1_VF		48	//BC1 / DXT1 (Vertical Flip)
#define	BGBBTJ_JPG_BC2_VF		49	//BC2 / DXT3 (Vertical Flip)
#define	BGBBTJ_JPG_BC3_VF		50	//BC3 / DXT5 (Vertical Flip)
#define	BGBBTJ_JPG_BC4_VF		51	//BC4 (Vertical Flip)
#define	BGBBTJ_JPG_BC5_VF		52	//BC5 (Vertical Flip)
#define	BGBBTJ_JPG_BC6_VF		53	//BC6H (Signed) (Vertical Flip)
#define	BGBBTJ_JPG_BC7_VF		54	//BC7 (Vertical Flip)
#define	BGBBTJ_JPG_BC6_VF2		55	//BC6H (Signed) (Vertical Flip)
#define	BGBBTJ_JPG_BC7_VF2		56	//BC7 (Vertical Flip)

#define	BGBBTJ_JPG_MB5C			57	//MB5C
#define	BGBBTJ_JPG_MB5C_VF		58	//BC7 (Vertical Flip)

#define	BGBBTJ_JPG_BC6_UVF		60	//BC6H (Unsigned)

#define	BGBBTJ_JPG_RGBX_F16		62	//RGBx(32)
#define	BGBBTJ_JPG_BGRX_F16		63	//BGRx(32)

#define	BGBBTJ_JPG_DXT1			BGBBTJ_JPG_BC1
#define	BGBBTJ_JPG_DXT3			BGBBTJ_JPG_BC2
#define	BGBBTJ_JPG_DXT5			BGBBTJ_JPG_BC3
#define	BGBBTJ_JPG_DXT1F		BGBBTJ_JPG_BC1F
#define	BGBBTJ_JPG_DXT5F		BGBBTJ_JPG_BC3F
#define	BGBBTJ_JPG_DXT5_UVAY	BGBBTJ_JPG_BC3_UVAY
#define	BGBBTJ_JPG_DXT1A		BGBBTJ_JPG_BC1A
#define	BGBBTJ_JPG_BC6H			BGBBTJ_JPG_BC6
#define	BGBBTJ_JPG_BC6H_SF16	BGBBTJ_JPG_BC6
#define	BGBBTJ_JPG_BC6H_UF16	BGBBTJ_JPG_BC6_UF16

#define	BGBBTJ_JPG_BC6H_VF		BGBBTJ_JPG_BC6_VF
#define	BGBBTJ_JPG_BC6H_SF16_VF	BGBBTJ_JPG_BC6_VF
// #define	BGBBTJ_JPG_BC6H_UF16_VF	BGBBTJ_JPG_BC6_UF16

#define	BGBBTJ_JPG_CLRS_YCBCR		0
#define	BGBBTJ_JPG_CLRS_ORCT		1
#define	BGBBTJ_JPG_CLRS_RGB			2
#define	BGBBTJ_JPG_CLRS_YCCK		3

#define	BGBBTJ_JPG_QFL_RDCT			256
#define	BGBBTJ_JPG_QFL_444			512
#define	BGBBTJ_JPG_QFL_ORCT			1024
#define	BGBBTJ_JPG_QFL_RGB			2048

#define BGBBTJ_ZFL_ESCAPE_FF	1
#define BGBBTJ_ZFL_DEFLATE64	2
#define BGBBTJ_ZFL_ESCBTLZA		4
#define BGBBTJ_ZFL_ESCBTLZH		8
#define BGBBTJ_ZFL_ISBTLZH		16
#define BGBBTJ_ZFL_ARITH		32


#define VFILE		FILE
// #define vffopen		fopen
#define vffopen		frgl_fopen
#define vfclose		fclose
#define vfread		fread
#define vfwrite		fwrite
#define vfgetc		fgetc
#define vfputc		fputc
#define vftell		ftell
#define vfseek		fseek

typedef struct FRGL_State_s FRGL_State;

struct FRGL_State_s
{
int mx, my;		//current mouse pos
int lmx, lmy;	//mouse pos last frame
int omx, omy;	//last non-dragging pos
int dmx, dmy;	//delta mouse pos
int dx, dy;		//drag from lmx
int odx, ody;	//drag from omx

int rmx, rmy;	//"real" mouse pos
int rlmx, rlmy;	//"real" mouse pos last frame

int mb, lmb;
unsigned short *keys;

int orgx, orgy;
int zoom;
int time;

int xs, ys;		//screen x and y size
int lxs, lys;	//screen last x and y size
double dt_f;	//current frame delta time
double adt_f;	//avg frame time
double adt2_f;	//avg frame time 2
double time_f;	//current time

int doshot;
int kill;
int maxhz;
int maxfasthz;

byte keymap[32];
};

typedef struct FRGL_FontFrag_s FRGL_FontFrag;
typedef struct FRGL_FontInfo_s FRGL_FontInfo;

struct FRGL_FontFrag_s {
FRGL_FontFrag *next, *prev; //in parent font
FRGL_FontInfo *parent;
int base;	//base character

// byte usemap[8];		//cells which have been drawn
// byte badmap[8];		//cells which are not valid

u16 usemap[16];		//cells which have been drawn
u16 badmap[16];		//cells which are not valid
byte *buffer;
int texnum;
};

struct FRGL_FontInfo_s {
FRGL_FontInfo *next;	//next in the list of fonts
FRGL_FontInfo *chain;	//links for font fragments
char *name;

byte *buf;
int w, h;
int s, e;
FRGL_FontFrag *frag;

FRGL_FontInfo *alts[16];	//alternate fonts to check
int n_alts;
};

typedef struct {
byte cur_clr[4];
float cur_st[2];
int cur_tex;
int cur_prim;
int cur_basexyz;

float *xyz;		//XYZ vertex coords
float *st;		//ST vertex coords
byte *rgba;		//RGBA vertex color
int n_xyz;		//num vertices
int m_xyz;		//max vertices

int ofs_xyz;	//XYZ offset
int ofs_st;		//ST offset
int ofs_rgba;	//RGBA offset
int ofs_end;	//offset of end
int sz_xyz;		//XYZ offset
int sz_st;		//ST offset
int sz_rgba;	//RGBA offset

int vbo;		//VBO number
byte *vbo_buf;	//VBO buffer
int sz_vbo;		//size of VBO

int *prim;		//(start, count, prim, tex)
int n_prim;
int m_prim;
}FRGL_TextVBO;

typedef struct BGBDT_MM_ParsePrintInfo_s BGBDT_MM_ParsePrintInfo;

#include <libvecmath.h>

#include <bteifgl_keys.h>
#include <bteifgl_concmd.h>
#include <bteifgl_net.h>
// #include <bteifgl_btic1g.h>

#include <bteifgl_mm.h>

#include <bteifgl_tagval.h>
#include <bgbdt_tagclass.h>
#include <bteifgl_entobj.h>

#include <bgbdt_bs2cc.h>
#include <bgbdt_bsvm21.h>

#include <bteifgl_texmat.h>

#include <bteifgl_chunk.h>

#include <bteifgl_wire.h>
#include <bteifgl_msp430.h>

struct BGBDT_MM_ParsePrintInfo_s {
BGBDT_MM_ParsePrintInfo *next;
char *buf;
char *cs, *css, *cse;
char *ct, *cts, *cte;

dtVal recarr[256];
int nrec;
int nchars;

int (*putstr)(BGBDT_MM_ParsePrintInfo *inf, char *str);
};

#include <bteifgl_auto.h>

#endif
