#define BS2CC_TYFL_PUBLIC			0x00000001
#define BS2CC_TYFL_PRIVATE			0x00000002
#define BS2CC_TYFL_PROTECTED		0x00000004
#define BS2CC_TYFL_STATIC			0x00000008
#define BS2CC_TYFL_FINAL			0x00000010
#define BS2CC_TYFL_SUPER			0x00000020
#define BS2CC_TYFL_SYNCHRONIZED		BS2CC_TYFL_SUPER
#define BS2CC_TYFL_VOLATILE			0x00000040
#define BS2CC_TYFL_TRANSIENT		0x00000080
#define BS2CC_TYFL_NATIVE			0x00000100
#define BS2CC_TYFL_INTERFACE		0x00000200
#define BS2CC_TYFL_ABSTRACT			0x00000400
#define BS2CC_TYFL_STRICT			0x00000800

#define BS2CC_TYFL_PPP				0x00000007

#define BS2CC_TYFL_BYREF			0x00001000
#define BS2CC_TYFL_DELEGATE			0x00002000
#define BS2CC_TYFL_GETTER			0x00004000
#define BS2CC_TYFL_SETTER			0x00008000

// #define BS2CC_TYFL_SIGNED			0x00010000
// #define BS2CC_TYFL_UNSIGNED			0x00020000
#define BS2CC_TYFL_EXTERN			0x00040000
#define BS2CC_TYFL_CONST			0x00080000
#define BS2CC_TYFL_INLINE			0x00100000
// #define BS2CC_TYFL_EVENT			0x00200000
// #define BS2CC_TYFL_WIDE				0x00400000
#define BS2CC_TYFL_THREAD			0x00800000
#define BS2CC_TYFL_DYNAMIC			0x01000000
#define BS2CC_TYFL_ASYNC			0x02000000

#define BS2CC_TYFL_LTLENDIAN		0x10000000
#define BS2CC_TYFL_BIGENDIAN		0x20000000
#define BS2CC_TYFL_DLLEXPORT		0x40000000
#define BS2CC_TYFL_DLLIMPORT		0x80000000

#define BS2CC_TYFL_CANREACH			0x000100000000LL
#define BS2CC_TYFL_PUBVISIBLE		0x000200000000LL
#define BS2CC_TYFL_REACHDONE		0x000400000000LL

#define BS2CC_ERRN_TMASK			0xF000
#define BS2CC_ERRN_ERROR			0x1000
#define BS2CC_ERRN_WARNING			0x2000
#define BS2CC_ERRN_HINT				0x3000
#define BS2CC_ERRN_FATAL			0x4000

#define BS2CC_ERRN_CASEERROR		(BS2CC_ERRN_ERROR|1)
#define BS2CC_ERRN_STUBERROR		(BS2CC_ERRN_ERROR|2)
#define BS2CC_ERRN_DIVZERROR		(BS2CC_ERRN_ERROR|3)
#define BS2CC_ERRN_ERRNODECL		(BS2CC_ERRN_ERROR|4)
#define BS2CC_ERRN_ERRTOOFEWARGS	(BS2CC_ERRN_ERROR|5)
#define BS2CC_ERRN_ERRTOOMANYARGS	(BS2CC_ERRN_ERROR|6)
#define BS2CC_ERRN_ERRBADCONV		(BS2CC_ERRN_ERROR|7)
#define BS2CC_ERRN_ERRCANTDEREF		(BS2CC_ERRN_ERROR|8)

#define BS2CC_ERRN_ERRSTACKMISAL	(BS2CC_ERRN_ERROR|9)
#define BS2CC_ERRN_ERRSTACKUFLOW	(BS2CC_ERRN_ERROR|10)

#define BS2CC_ERRN_CONSTRANGE		(BS2CC_ERRN_WARNING|1)
#define BS2CC_ERRN_CONVIMPLICIT		(BS2CC_ERRN_WARNING|2)
#define BS2CC_ERRN_CONVNARROW		(BS2CC_ERRN_WARNING|3)
#define BS2CC_ERRN_NOSEMICOLON		(BS2CC_ERRN_WARNING|4)

#define BS2CC_ERRN_FATALERRCNT		(BS2CC_ERRN_FATAL|1)
#define BS2CC_ERRN_FATALNOFILE		(BS2CC_ERRN_FATAL|2)

#define BS2CC_VITYPE_UNKNOWN		0
#define BS2CC_VITYPE_GBLVAR			1		//Global/Pkg or Static Var
#define BS2CC_VITYPE_GBLFUNC		2		//Global/Pkg or Static Func
#define BS2CC_VITYPE_STRVAR			3		//Instance Variable
#define BS2CC_VITYPE_STRFUNC		4		//Virtual Method
#define BS2CC_VITYPE_STRUCT			5		//Structure
#define BS2CC_VITYPE_CLASS			6		//Class
#define BS2CC_VITYPE_IFACE			7		//Interface
#define BS2CC_VITYPE_LCLVAR			8		//Local Var
#define BS2CC_VITYPE_LXLVAR			9		//Lexical Var
#define BS2CC_VITYPE_PACKAGE		10		//Package

//Type IDs:
//  0..   15: Core Types
// 16..  255: Extended Core Types
//256..65535: Composite Types


#define BS2CC_TYZ_MASK		0x0000000F		//Core (Major) Type
#define BS2CC_TYE_MASK		0x00003FFF		//Exteded Type (Fptr/Obj)
#define BS2CC_TYF_MASK		0x0000C000		//Exteded Type (Fptr/Obj)
#define BS2CC_TYI_MASK		0x00070000		//Indirection Levels
#define BS2CC_TYS_MASK		0x3FF80000		//Array Size
#define BS2CC_TYT_MASK		0xC0000000		//Type Type

#define BS2CC_TYO_MASK		0x000FFFFF		//Overflowed Type

#define BS2CC_TYZ_SHR		0				//Core (Major) Type
#define BS2CC_TYE_SHR		0				//Exteded Type (Fptr/Obj)
#define BS2CC_TYI_SHR		16				//Indirection Levels
#define BS2CC_TYS_SHR		19				//Array Size
#define BS2CC_TYT_SHR		30				//Type Type

#define BS2CC_TYZ_MASK2		0x000F			//Core (Major) Type
#define BS2CC_TYE_MASK2		0x3FFF			//Exteded Type (Fptr/Obj)
#define BS2CC_TYI_MASK2		0x0007			//Indirection Levels
#define BS2CC_TYS_MASK2		0x07FF			//Array Size
#define BS2CC_TYT_MASK2		0x0003			//Type Type

#define BS2CC_TYI_A0		0x00000000		//T
#define BS2CC_TYI_A1		0x00010000		//T[]
#define BS2CC_TYI_A2		0x00020000		//T[][]
#define BS2CC_TYI_A3		0x00030000		//T[][][]
//#define BS2CC_TYI_A4		0x00040000		//T[][][][]
#define BS2CC_TYI_R1		0x00040000		//&T
#define BS2CC_TYI_P1		0x00050000		//*T
#define BS2CC_TYI_P2		0x00060000		//**T
#define BS2CC_TYI_P3		0x00070000		//***T

#define BS2CC_TYF_NAB		0x00008000		//T? or T!

#define BS2CC_TYT_BASIC		0x00000000		//Basic Type
#define BS2CC_TYT_OVF		0x40000000		//Overflow

#define BS2CC_TYZ_INT		0x0000
#define BS2CC_TYZ_LONG		0x0001
#define BS2CC_TYZ_FLOAT		0x0002
#define BS2CC_TYZ_DOUBLE	0x0003
#define BS2CC_TYZ_ADDRESS	0x0004
#define BS2CC_TYZ_UINT		0x0005
#define BS2CC_TYZ_UBYTE		0x0006
#define BS2CC_TYZ_SHORT		0x0007
#define BS2CC_TYZ_SBYTE		0x0008
#define BS2CC_TYZ_USHORT	0x0009
#define BS2CC_TYZ_ULONG		0x000A
#define BS2CC_TYZ_CONST		0x000B

#define BS2CC_TYZ_ADDR		0x0004
#define BS2CC_TYZ_VOID		0x000B
#define BS2CC_TYZ_VARIANT	0x0010
#define BS2CC_TYZ_STRING	0x0011
#define BS2CC_TYZ_VARARG	0x0012
#define BS2CC_TYZ_CHAR		0x0013
#define BS2CC_TYZ_CHAR8		0x0014
#define BS2CC_TYZ_CSTRING	0x0015
#define BS2CC_TYZ_NLONG		0x0016
#define BS2CC_TYZ_UNLONG	0x0017
#define BS2CC_TYZ_BOOL		0x0018
#define BS2CC_TYZ_HFLOAT	0x0019

#define BS2CC_TYZ_INT128	0x0020
#define BS2CC_TYZ_UINT128	0x0021
#define BS2CC_TYZ_FLOAT128	0x0022

#define BS2CC_TYZ_VEC2F		0x0024
#define BS2CC_TYZ_VEC3F		0x0025
#define BS2CC_TYZ_VEC4F		0x0026
#define BS2CC_TYZ_QUATF		0x0027
#define BS2CC_TYZ_VEC2D		0x0028
#define BS2CC_TYZ_FCPLX		0x0029
#define BS2CC_TYZ_DCPLX		0x002A


#define BS2CC_TYZ_VARIANT_ARR	(BS2CC_TYI_A1|BS2CC_TYZ_VARIANT)

//#define BS2CC_IMG_TWOCC(a, b)			(((b)<<8)|(a))
//#define BS2CC_IMG_FOURCC(a, b, c, d)	(((d)<<24)|((c)<<16)|((b)<<8)|(a))

//#define BS2CC_IMG_EIGHTCC(a, b, c, d, e, f, g, h)	\
//	((((u64)BS2CC_IMG_FOURCC(e, f, g, h))<<32)|		\
//	BS2CC_IMG_FOURCC(a, b, c, d))

#define BS2CC_IMG_TWOCC(a, b)			(((a)<<8)|(b))
#define BS2CC_IMG_FOURCC(a, b, c, d)	(((a)<<24)|((b)<<16)|((c)<<8)|(d))

#define BS2CC_IMG_EIGHTCC(a, b, c, d, e, f, g, h)	\
	((((u64)BS2CC_IMG_FOURCC(a, b, c, d))<<32)|		\
	BS2CC_IMG_FOURCC(e, f, g, h))

#define BS2CC_IECC_BS2I0EXE	BS2CC_IMG_EIGHTCC('B','S','2','I','0','E','X','E')
#define BS2CC_IECC_BS2I0DLL	BS2CC_IMG_EIGHTCC('B','S','2','I','0','D','L','L')

#define BS2CC_IFCC_BS2I	BS2CC_IMG_FOURCC('B', 'S', '2', 'I')
#define BS2CC_IFCC_BS2L	BS2CC_IMG_FOURCC('B', 'S', '2', 'L')

#define BS2CC_IFCC_STRS	BS2CC_IMG_FOURCC('S', 'T', 'R', 'S')
#define BS2CC_IFCC_BADD	BS2CC_IMG_FOURCC('B', 'A', 'D', 'D')

#define BS2CC_IFCC_GBLS	BS2CC_IMG_FOURCC('G', 'B', 'L', 'S')
#define BS2CC_IFCC_GIX2	BS2CC_IMG_FOURCC('G', 'I', 'X', '2')
#define BS2CC_IFCC_GIX3	BS2CC_IMG_FOURCC('G', 'I', 'X', '3')
#define BS2CC_IFCC_GIX4	BS2CC_IMG_FOURCC('G', 'I', 'X', '4')

#define BS2CC_IFCC_MAIN	BS2CC_IMG_FOURCC('M', 'A', 'I', 'N')
#define BS2CC_IFCC_PVBM	BS2CC_IMG_FOURCC('P', 'V', 'B', 'M')

#define BS2CC_ITCC_BC	BS2CC_IMG_TWOCC('B', 'C')
#define BS2CC_ITCC_CL	BS2CC_IMG_TWOCC('C', 'L')
#define BS2CC_ITCC_GF	BS2CC_IMG_TWOCC('G', 'F')
#define BS2CC_ITCC_GV	BS2CC_IMG_TWOCC('G', 'V')
#define BS2CC_ITCC_IF	BS2CC_IMG_TWOCC('I', 'F')
#define BS2CC_ITCC_PK	BS2CC_IMG_TWOCC('P', 'K')
#define BS2CC_ITCC_PV	BS2CC_IMG_TWOCC('P', 'V')
#define BS2CC_ITCC_SF	BS2CC_IMG_TWOCC('S', 'F')
#define BS2CC_ITCC_ST	BS2CC_IMG_TWOCC('S', 'T')
#define BS2CC_ITCC_SV	BS2CC_IMG_TWOCC('S', 'V')
// #define BS2CC_ITCC_SA	BS2C_IMG_TWOCC('S', 'A')

#define BS2CC_I1CC_NULL		' '

#define BS2CC_I1CC_NARGS	'A'
#define BS2CC_I1CC_BARGS	'B'
#define BS2CC_I1CC_CODE		'C'
#define BS2CC_I1CC_STKDEPTH	'D'
#define BS2CC_I1CC_ECLASS	'E'
#define BS2CC_I1CC_FLAGS	'F'
#define BS2CC_I1CC_GITAG	'G'
#define BS2CC_I1CC_VARINIT	'H'
#define BS2CC_I1CC_IMPL		'I'

#define BS2CC_I1CC_NLOCALS	'L'

#define BS2CC_I1CC_MAIN		'M'
#define BS2CC_I1CC_NAME		'N'

#define BS2CC_I1CC_PACKAGE	'P'
#define BS2CC_I1CC_QNAME	'Q'
#define BS2CC_I1CC_QNAMEH	'R'
#define BS2CC_I1CC_SIG		'S'
#define BS2CC_I1CC_NAMEH	'T'

#define BS2CC_ILEN_SVLI		-2

#define BS2CC_V4SHUF(a, b, c, d)  (((d)<<6)|((c)<<4)|((b)<<2)|(a))

typedef struct BS2CC_CompileContext_s BS2CC_CompileContext;

typedef struct BS2CC_VarInfo_s BS2CC_VarInfo;
typedef struct BS2CC_TypeOverflow_s BS2CC_TypeOverflow;
typedef struct BS2CC_CcFrame_s BS2CC_CcFrame;
typedef struct BS2CC_PkgFrame_s BS2CC_PkgFrame;

struct BS2CC_VarInfo_s
{
BS2CC_VarInfo *next;		//next in list
BS2CC_VarInfo *pknext;		//next in package
BS2CC_PkgFrame *pkg;		//owning package (globals)
BS2CC_VarInfo *obj;			//owning object (fields/methods)
char *name;					//name (local)
char *qname;				//name (fully qualified)
char *sig;
int bty;					//base type ID
int rty;					//return type ID
s64 bmfl;					//base flags
int gid;					//var ID
int vitype;					//varinfo type

BS2CC_VarInfo *args[256];
BS2CC_VarInfo *iface[32];
BS2CC_VarInfo *super;
int nargs, niface;
BS2CC_CcFrame *body;
dtVal typeExp;				//AST for variable type
dtVal bodyExp;				//AST for function body
dtVal initExp;				//AST for variable initialization
dtVal extsExp;				//AST for extends
dtVal implExp;				//AST for implements
};

struct BS2CC_TypeOverflow_s
{
int base;		//base type
byte al;		//array level
byte pl;		//pointer level
byte rf;		//reference
byte an;		//fixed array levels
byte nab;		//nullable / nonnull
int asz[16];	//fixed array sizes
};

struct BS2CC_CcFrame_s
{
byte *ct;
byte *cts;
byte *cte;
byte newtrace;
byte zpf, zpo;
byte wasret;
int szt;

byte *cs;
byte *cse;

BS2CC_CompileContext *ctx;	//owning context
BS2CC_VarInfo *func;		//parent function

u32 stack_bty[256];			//temporary stack base-type
int stackpos;				//stack pos for temp stack
int stacksize;				//max size of stack frame

BS2CC_VarInfo *locals[256];
int nlocals;
int bargs;					//start of arguments list

int tlbl_id[256];
int tlbl_ct[256];
int ntlbl;

int trlc_id[256];
int trlc_ct[256];
int ntrlc;

int nlbl_id[256];
char *nlbl_n[256];
int nnlbl;

int constk[64];			//continue stack
int brkstk[64];			//break stack
int constkpos;			//continue stack pos
int brkstkpos;			//break stack pos

int gbltab[256];
// int littab[256];
int ngbl;
// int nlit;
int jcleanup;
};

struct BS2CC_PkgFrame_s
{
BS2CC_PkgFrame *next;
BS2CC_PkgFrame *parent;
char *name;
char *qname;

BS2CC_VarInfo *vars;		//declarations

BS2CC_PkgFrame *imps[256];
int nimps;
int gid;
};

struct BS2CC_CompileContext_s
{
/* Lexer / Parser */
char *srcbuf;
int *srcofs;
char *tokbuf;
int *tokarr;
int ntok;
int tok;
char *cname;

/* Bytecode Generation */
BS2CC_CcFrame *frmstack[256];
int frmstackpos;

BS2CC_PkgFrame *pkg_first;

BS2CC_CcFrame *frm;
BS2CC_PkgFrame *pkg;
BS2CC_VarInfo *obj;
int gsseq;

BS2CC_VarInfo *objstk[16];
BS2CC_PkgFrame *pkgstk[16];
int objstkpos;
int pkgstkpos;

// char *curpkg;
BS2CC_VarInfo *globals[4096];
int nglobals;

BS2CC_TypeOverflow *tyovf[4096];
int ntyovf;


int cwarnln[256];	//warning line number
u16 cwarn[256];		//warning and hint types
int cerrln[256];	//error line number
u16 cerr[256];		//error type
int cfatalln[16];	//fatal line number
u16 cfatal[16];		//fatal type
int ncwarn;			//number of warnings
int ncerr;			//number of errors
int ncfatal;		//number of fatal errors

s16 cwarnpix[256];	//warning parameter index
s16 cerrpix[256];	//error parameter intex

char *srcfns[256];	//source file names (warning/error)
int nsrcfns;
char *srcnix[256];	//source name index (warning/error)
int nsrcnixs;

int cwparm[1024];	//warning/error parameters
int ncwparm;

char *srcfn;		//source file name
int srcln;			//source line number

BGBDT_MM_ParsePrintInfo *dbgprn;

char *strtab;
char *strtabe;
char *strct;

};

