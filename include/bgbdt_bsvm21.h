#define BSVM2_OPRI_ADD		0
#define BSVM2_OPRI_SUB		1
#define BSVM2_OPRI_MUL		2
#define BSVM2_OPRI_AND		3
#define BSVM2_OPRI_OR		4
#define BSVM2_OPRI_XOR		5
#define BSVM2_OPRI_SHL		6
#define BSVM2_OPRI_SAR		7
#define BSVM2_OPRI_SHR		8
#define BSVM2_OPRI_DIV		9
#define BSVM2_OPRI_MOD		10
#define BSVM2_OPRI_UMUL		11
#define BSVM2_OPRI_UMULH	12
#define BSVM2_OPRI_UDIV		13

#define BSVM2_OPRF_ADD		0
#define BSVM2_OPRF_SUB		1
#define BSVM2_OPRF_MUL		2
#define BSVM2_OPRF_DIV		3

#define BSVM2_OPRC_EQ		0
#define BSVM2_OPRC_NE		1
#define BSVM2_OPRC_LT		2
#define BSVM2_OPRC_GT		3
#define BSVM2_OPRC_LE		4
#define BSVM2_OPRC_GE		5
#define BSVM2_OPRC_EQQ		6
#define BSVM2_OPRC_NEQ		7

#define BSVM2_OPZ_INT		0
#define BSVM2_OPZ_LONG		1
#define BSVM2_OPZ_FLOAT		2
#define BSVM2_OPZ_DOUBLE	3
#define BSVM2_OPZ_ADDRESS	4
#define BSVM2_OPZ_UINT		5
#define BSVM2_OPZ_UBYTE		6
#define BSVM2_OPZ_SHORT		7
#define BSVM2_OPZ_SBYTE		8
#define BSVM2_OPZ_USHORT	9
#define BSVM2_OPZ_ULONG		10
#define BSVM2_OPZ_CONST		11

#define BSVM2_OPZ_ADDR		4
#define BSVM2_OPZ_VARIANT	4
#define BSVM2_OPZ_VOID		11

#define BSVM2_OP_ADDI		0x00	//Add Int (A+B)
#define BSVM2_OP_SUBI		0x01	//Sub Int (A-B)
#define BSVM2_OP_MULI		0x02	//Mul Int (A*B)
#define BSVM2_OP_ANDI		0x03	//And Int (A&B)
#define BSVM2_OP_ORI		0x04	//Or  Int (A|B)
#define BSVM2_OP_XORI		0x05	//Xor Int (A^B)
#define BSVM2_OP_SHLI		0x06	//Shl Int (A<<B)
#define BSVM2_OP_SARI		0x07	//Sar Int (A>>B)
#define BSVM2_OP_ADDL		0x08	//Add Long (A+B)
#define BSVM2_OP_SUBL		0x09	//Add Long (A-B)
#define BSVM2_OP_MULL		0x0A	//Add Long (A*B)
#define BSVM2_OP_ANDL		0x0B	//Add Long (A&B)
#define BSVM2_OP_ORL		0x0C	//Add Long (A|B)
#define BSVM2_OP_XORL		0x0D	//Add Long (A^B)
#define BSVM2_OP_SHLL		0x0E	//Add Long (A<<B)
#define BSVM2_OP_SARL		0x0F	//Add Long (A>>B)
#define BSVM2_OP_ADDF		0x10	//Add Float (A+B)
#define BSVM2_OP_SUBF		0x11	//Sub Float (A-B)
#define BSVM2_OP_MULF		0x12	//Mul Float (A*B)
#define BSVM2_OP_DIVF		0x13	//Div Float (A/B)
#define BSVM2_OP_ADDD		0x14	//Add Double (A+B)
#define BSVM2_OP_SUBD		0x15	//Add Double (A-B)
#define BSVM2_OP_MULD		0x16	//Add Double (A*B)
#define BSVM2_OP_DIVD		0x17	//Add Double (A/B)
#define BSVM2_OP_NEGI		0x18	//Negate Int (-A)
#define BSVM2_OP_NEGL		0x19	//Negate Long (-A)
#define BSVM2_OP_NEGF		0x1A	//Negate Float (-A)
#define BSVM2_OP_NEGD		0x1B	//Negate Double (-A)
#define BSVM2_OP_NOTI		0x1C	//Not Int (~A)
#define BSVM2_OP_NOTL		0x1D	//Not Long (~A)
#define BSVM2_OP_LNTI		0x1E	//LNot Int (!A)
#define BSVM2_OP_LNTL		0x1F	//LNot Long (!A)
#define BSVM2_OP_LDI		0x20	// => I
#define BSVM2_OP_LDL		0x21	// => I
#define BSVM2_OP_LDF		0x22	// => I
#define BSVM2_OP_LDD		0x23	// => I
#define BSVM2_OP_STI		0x24	// A =>
#define BSVM2_OP_STL		0x25	// A =>
#define BSVM2_OP_STF		0x26	// A =>
#define BSVM2_OP_STD		0x27	// A =>
#define BSVM2_OP_LDA		0x28	// => I
#define BSVM2_OP_STA		0x29	// A =>
#define BSVM2_OP_LDC		0x2A	//Load Constant
#define BSVM2_OP_MVA		0x2B	//MOV I to J
#define BSVM2_OP_MVI		0x2C	//MOV I to J
#define BSVM2_OP_MVL		0x2D	//MOV I to J
#define BSVM2_OP_LVF		0x2E	//MOV I to J
#define BSVM2_OP_MVD		0x2F	//MOV I to J
#define BSVM2_OP_JEQ		0x30	//Jump Equal ( (A==0) => )
#define BSVM2_OP_JNE		0x31	//Jump Not-Equal ( (A!=0) => )
#define BSVM2_OP_JLT		0x32	//Jump Less Than ( (A<0) => )
#define BSVM2_OP_JGT		0x33	//Jump Greater Than ( (A>0) => )
#define BSVM2_OP_JLE		0x34	//Jump Less Equal ( (A<=0) => )
#define BSVM2_OP_JGE		0x35	//Jump Greater Equal ( (A>=0) => )
#define BSVM2_OP_JCMP		0x36	//Jump-Compare ( (A op B) =>)
#define BSVM2_OP_JMP		0x37	//Jump
#define BSVM2_OP_CMPI		0x38	//Compare Int
#define BSVM2_OP_CMPL		0x39	//Compare Long
#define BSVM2_OP_CMPF		0x3A	//Compare Float
#define BSVM2_OP_CMPD		0x3B	//Compare Double
#define BSVM2_OP_CMPA		0x3C	//Compare Address (Logical)
#define BSVM2_OP_CMP2A		0x3D	//Compare Address (Strict)
#define BSVM2_OP_CMP2F		0x3E	//Compare Float (Unordered)
#define BSVM2_OP_CMP2D		0x3F	//Compare Double (Unordered)
#define BSVM2_OP_LDIXI		0x40	//Load Index Int ( A I => B )
#define BSVM2_OP_LDIXL		0x41	//Load Index Long ( A I => B )
#define BSVM2_OP_LDIXF		0x42	//Load Index Float ( A I => B )
#define BSVM2_OP_LDIXD		0x43	//Load Index Double ( A I => B )
#define BSVM2_OP_STIXI		0x44	//Store Index Int ( A I V => )
#define BSVM2_OP_STIXL		0x45	//Store Index Long ( A I V => )
#define BSVM2_OP_STIXF		0x46	//Store Index Float ( A I V => )
#define BSVM2_OP_STIXD		0x47	//Store Index Double ( A I V => )
#define BSVM2_OP_LDIXSB		0x48	//Load Index Signed Byte ( A I => B )
#define BSVM2_OP_LDIXUB		0x49	//Load Index Unsigned Byte ( A I => B )
#define BSVM2_OP_LDIXSS		0x4A	//Load Index Signed Short ( A I => B )
#define BSVM2_OP_LDIXUS		0x4B	//Load Index Unsigned Short ( A I => B )
#define BSVM2_OP_LDIXA		0x4C	//Load Index Address ( A I => B )
#define BSVM2_OP_STIXA		0x4D	//Store Index Address ( A I V => )
#define BSVM2_OP_STIXB		0x4E	//Store Index Byte ( A I V => )
#define BSVM2_OP_STIXS		0x4F	//Store Index Short ( A I V => )
#define BSVM2_OP_LDIXIC		0x50	//Load Index Int ( I; A => B )
#define BSVM2_OP_LDIXLC		0x51	//Load Index Long ( I; A => B )
#define BSVM2_OP_LDIXFC		0x52	//Load Index Float ( I; A => B )
#define BSVM2_OP_LDIXDC		0x53	//Load Index Double ( I; A => B )
#define BSVM2_OP_STIXIC		0x54	//Store Index Int ( I; A V => )
#define BSVM2_OP_STIXLC		0x55	//Store Index Long ( I; A V => )
#define BSVM2_OP_STIXFC		0x56	//Store Index Float ( I; A V => )
#define BSVM2_OP_STIXDC		0x57	//Store Index Double ( I; A V => )
#define BSVM2_OP_LDIXSBC	0x58	//Load Index Signed Byte ( I; A => B )
#define BSVM2_OP_LDIXUBC	0x59	//Load Index Unsigned Byte ( I; A => B )
#define BSVM2_OP_LDIXSSC	0x5A	//Load Index Signed Short ( I; A => B )
#define BSVM2_OP_LDIXUSC	0x5B	//Load Index Unsigned Short ( I; A => B )
#define BSVM2_OP_LDIXAC		0x5C	//Load Index Address ( I; A => B )
#define BSVM2_OP_STIXAC		0x5D	//Store Index Address ( I; A V => )
#define BSVM2_OP_STIXBC		0x5E	//Store Index Byte ( I; A V => )
#define BSVM2_OP_STIXSC		0x5F	//Store Index Short ( I; A V => )
#define BSVM2_OP_BINOP		0x60	//Binary Operator
#define BSVM2_OP_CMPOP		0x61	//Compare Operator
#define BSVM2_OP_BINOPL		0x62	//Binary Operator (A op I)
#define BSVM2_OP_CMPOPL		0x63	//Compare Operator (A op I)
#define BSVM2_OP_BINOPLL	0x64	//Binary Operator (I op J)
#define BSVM2_OP_CMPOPLL	0x65	//Compare Operator (I op J)
#define BSVM2_OP_BINOPC		0x66	//Binary Operator (A op C)
#define BSVM2_OP_BINOPLC	0x67	//Binary Operator (I op C)
#define BSVM2_OP_CMPOPC		0x68	//Compare Operator (A op C)
#define BSVM2_OP_CMPOPLC	0x69	//Compare Operator (I op C)
#define BSVM2_OP_PUSHN		0x6A	//Push n Items (null/zero)
#define BSVM2_OP_POPN		0x6B	//Pop n Items
#define BSVM2_OP_SWAPN		0x6C	//Swap top-of-stack with Item n
#define BSVM2_OP_ROTLN		0x6D	//Rotate Left 1 position over n items.
#define BSVM2_OP_ROTRN		0x6E	//Rotate Right 1 position over n items.
#define BSVM2_OP_DUPN		0x6F	//Duplicate Top n items.
#define BSVM2_OP_CALLG		0x70	//Call Static/Global Function
#define BSVM2_OP_CALLA		0x71	//Call Function Reference (Pops Func)
#define BSVM2_OP_CALLV		0x72	//Call Virtual Method (Pops Obj)
#define BSVM2_OP_CALLL		0x73	//Call Local Function Reference
#define BSVM2_OP_RETI		0x74	//Return Int
#define BSVM2_OP_RETL		0x75	//Return Long
#define BSVM2_OP_RETF		0x76	//Return Float
#define BSVM2_OP_RETD		0x77	//Return Double
#define BSVM2_OP_RETA		0x78	//Return Address
#define BSVM2_OP_RETV		0x79	//Return Void
#define BSVM2_OP_RET2		0x7A	//Return Local
#define BSVM2_OP_LABEL		0x7B	//Break the current trace
#define BSVM2_OP_LDOS		0x7C	//Load Instance Variable, ( O => A )
#define BSVM2_OP_STOS		0x7D	//Store Instance Variable ( O V => )
#define BSVM2_OP_LDGS		0x7E	//Load Global Variable, ( => A )
#define BSVM2_OP_STGS		0x7F	//Store Global Variable ( V => )
#define BSVM2_OP_LDRL		0x80	//Load Ref To Local
#define BSVM2_OP_LDROS		0x81	//Load Ref to Instance Variable ( O => A )
#define BSVM2_OP_LDRGS		0x82	//Load Ref to Global Variable, ( => A )
#define BSVM2_OP_LDRLX		0x83	//Load Ref To Lexical
#define BSVM2_OP_LDLX		0x84	//Load Lexical
#define BSVM2_OP_STLX		0x85	//Store Lexical
#define BSVM2_OP_DCLX		0x86	//Declare Lexical Variables
#define BSVM2_OP_DELX		0x87	//Delete Lexical Variables
#define BSVM2_OP_NEWOBJ		0x88	//Create object instance.
#define BSVM2_OP_DELOBJ		0x89	//Delete object instance.
#define BSVM2_OP_NEWARR		0x8A	//Create array instance.
#define BSVM2_OP_DELARR		0x8B	//Delete array instance.
#define BSVM2_OP_MOVOBJ		0x8C	//Copy object contents ( A B => )
#define BSVM2_OP_MOVARR		0x8D	//Copy array contents ( A B => )
#define BSVM2_OP_INCREF		0x8E	//Increment object reference count
#define BSVM2_OP_DECREF		0x8F	//Decrement object reference count
#define BSVM2_OP_CVTI2L		0x90	//Convert Int->Long
#define BSVM2_OP_CVTI2F		0x91	//Convert Int->Float
#define BSVM2_OP_CVTI2D		0x92	//Convert Int->Double
#define BSVM2_OP_CVTL2I		0x93	//Convert Long->Int
#define BSVM2_OP_CVTL2F		0x94	//Convert Long->Float
#define BSVM2_OP_CVTL2D		0x95	//Convert Long->Double
#define BSVM2_OP_CVTF2I		0x96	//Convert Float->Int
#define BSVM2_OP_CVTF2L		0x97	//Convert Float->Long
#define BSVM2_OP_CVTF2D		0x98	//Convert Float->Double
#define BSVM2_OP_CVTD2I		0x99	//Convert Double->Int
#define BSVM2_OP_CVTD2L		0x9A	//Convert Double->Long
#define BSVM2_OP_CVTD2F		0x9B	//Convert Double->Float
#define BSVM2_OP_CVTSB2I	0x9C	//Convert SByte->Int
#define BSVM2_OP_CVTUB2I	0x9D	//Convert UByte->Int
#define BSVM2_OP_CVTSS2I	0x9E	//Convert Short->Int
#define BSVM2_OP_CVTUS2I	0x9F	//Convert UShort->Int
#define BSVM2_OP_POPI		0xA0	//Pop Int
#define BSVM2_OP_POPL		0xA1	//Pop Long
#define BSVM2_OP_POPF		0xA2	//Pop Float
#define BSVM2_OP_POPD		0xA3	//Pop Double
#define BSVM2_OP_DUPI		0xA4	//Duplicate Int
#define BSVM2_OP_DUPL		0xA5	//Duplicate Long
#define BSVM2_OP_DUPF		0xA6	//Duplicate Float
#define BSVM2_OP_DUPD		0xA7	//Duplicate Double
#define BSVM2_OP_POPA		0xA8	//Pop Address
#define BSVM2_OP_DUPA		0xA9	//Dup Address
#define BSVM2_OP_SWAPA		0xAA	//Swap Address
#define BSVM2_OP_PUSHA		0xAB	//Push Address null
#define BSVM2_OP_PUSHI		0xAC	//Push Int 0
#define BSVM2_OP_PUSHL		0xAD	//Push Long 0
#define BSVM2_OP_PUSHF		0xAE	//Push Float 0.0
#define BSVM2_OP_PUSHD		0xAF	//Push Double 0.0
#define BSVM2_OP_ADDIC		0xB0	//A B => A+C
#define BSVM2_OP_SUBIC		0xB1	//A B => A-C
#define BSVM2_OP_MULIC		0xB2	//A B => A*C
#define BSVM2_OP_ANDIC		0xB3	//A B => A&C
#define BSVM2_OP_ORIC		0xB4	//A B => A|C
#define BSVM2_OP_XORIC		0xB5	//A B => A^C
#define BSVM2_OP_SHLIC		0xB6	//A B => A<<C
#define BSVM2_OP_SARIC		0xB7	//A B => A>>C
#define BSVM2_OP_ADDIL		0xB8	//A B => A+I
#define BSVM2_OP_SUBIL		0xB9	//A B => A-I
#define BSVM2_OP_MULIL		0xBA	//A B => A*I
#define BSVM2_OP_ANDIL		0xBB	//A B => A&I
#define BSVM2_OP_ORIL		0xBC	//A B => A|I
#define BSVM2_OP_XORIL		0xBD	//A B => A^I
#define BSVM2_OP_SHLIL		0xBE	//A B => A<<I
#define BSVM2_OP_SARIL		0xBF	//A B => A>>I

#define BSVM2_OP_HNOEX		0xC0	//Non-Execute (Trace)
#define BSVM2_OP_HNOEX1		0xC1	//Non-Execute (1 Op)
#define BSVM2_OP_HNOEX2		0xC2	//Non-Execute (2 Op)
#define BSVM2_OP_HNOEX3		0xC3	//Non-Execute (3 Op)
#define BSVM2_OP_RETC		0xC4	//Return Constant

#define BSVM2_OP_UCMPI		0xC6	//Unsigned Compare Int
#define BSVM2_OP_UCMPL		0xC7	//Unsigned Compare Long

#define BSVM2_OP_CMPIC		0xC8	//Compare Int Const
#define BSVM2_OP_CMPIL		0xC9	//Compare Int Local
#define BSVM2_OP_CMPILC		0xCA	//Compare Int Local/Const
#define BSVM2_OP_CMPILL		0xCB	//Compare Int Local/Local


typedef union BSVM2_Value_u BSVM2_Value;
typedef struct BSVM2_Opcode_s BSVM2_Opcode;
typedef struct BSVM2_TailOpcode_s BSVM2_TailOpcode;
typedef struct BSVM2_Trace_s BSVM2_Trace;
typedef struct BSVM2_Frame_s BSVM2_Frame;
typedef struct BSVM2_Context_s BSVM2_Context;

union BSVM2_Value_u {
struct { s32 i, j; };
struct { u32 ui, uj; };
s64 l;
u64 ul;
f32 f;
f64 d;
dtVal a;
};

struct BSVM2_Opcode_s {
void (*Run)(BSVM2_Frame *frm, BSVM2_Opcode *op);
int i0, i1;
int t0, t1;
BSVM2_Value v;
};

struct BSVM2_TailOpcode_s {
BSVM2_Trace *(*Run)(BSVM2_Frame *frm,
	BSVM2_TailOpcode *op);
BSVM2_Trace *nexttrace;
BSVM2_Trace *jmptrace;
int i0, i1;
int t0, t1;
};

struct BSVM2_Trace_s {
BSVM2_Opcode **ops;
BSVM2_TailOpcode *top;
};

struct BSVM2_Frame_s {
BSVM2_Context *ctx;		//owning context
BSVM2_Value *stack;		//stack base
BSVM2_Value *local;		//locals base
};

struct BSVM2_Context_s {
BSVM2_Frame *cstack;		//call stack
BSVM2_Value *tstack;		//temp stack (locals and stack)
int tstackref;				//ref position of temp stack
};

struct BSVM2_CodeBlock_s {
byte *cs;
byte *code;
byte *ecode;
int stkpos;
};

