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


typedef struct BS2CC_CompileContext_s BS2CC_CompileContext;

typedef struct BS2CC_VarInfo_s BS2CC_VarInfo;
typedef struct BS2CC_CcFrame_s BS2CC_CcFrame;

struct BS2CC_VarInfo_s
{
char *name;
byte bty;
int bmfl;
};

struct BS2CC_CcFrame_s
{
byte *ct;

byte stack_bty[256];	//temporary stack base-type
int stackpos;			//stack pos for temp stack

BS2CC_VarInfo locals[256];
int nlocals;
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
BS2CC_CcFrame *frm;
};

