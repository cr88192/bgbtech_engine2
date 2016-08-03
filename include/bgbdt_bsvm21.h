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
#define BSVM2_OPZ_NLONG		12
#define BSVM2_OPZ_UNLONG	13
#define BSVM2_OPZ_X128		14

#define BSVM2_OPZ_ADDR		4
// #define BSVM2_OPZ_VARIANT	4
#define BSVM2_OPZ_VOID		11

#define BSVM2_OPZ_CI_FLOAT	12
#define BSVM2_OPZ_CI_DOUBLE	13

#define BSVM2_OPMU_SIN		0	//sin(x)
#define BSVM2_OPMU_COS		1	//cos(x)
#define BSVM2_OPMU_SQRT		2	//sqrt(x)
#define BSVM2_OPMU_RCP		3	//(1.0/x)
#define BSVM2_OPMU_ABS		4	//fabs(x)
#define BSVM2_OPMU_SQR		5	//(x*x)
#define BSVM2_OPMU_SSQRT	6	//(x>=0)?sqrt(x):-sqrt(-x)
#define BSVM2_OPMU_LOG		7	//ln(x)
#define BSVM2_OPMU_TAN		8
#define BSVM2_OPMU_ATAN		9
#define BSVM2_OPMU_FLOOR	10	//floor(x)
#define BSVM2_OPMU_CEIL		11	//ceil(x)
#define BSVM2_OPMU_EXP		12	//(e^x)
#define BSVM2_OPMU_LOG10	13	//ln(x)
#define BSVM2_OPMU_LOG2		14	//log2(x)

#define BSVM2_OPAG_ALEN		0	//Array Length
#define BSVM2_OPAG_AOFS		1	//Array Offset

#define BSVM2_OPRV_ADD		0
#define BSVM2_OPRV_SUB		1
#define BSVM2_OPRV_MUL		2
#define BSVM2_OPRV_DIV		3
#define BSVM2_OPRV_DOT		4
#define BSVM2_OPRV_CROSS	5
#define BSVM2_OPRV_CMUL		6
#define BSVM2_OPRV_CDIV		7
#define BSVM2_OPRV_CMUL2	8
#define BSVM2_OPRV_CDIV2	9

#define BSVM2_OPUV_NEG		0
#define BSVM2_OPUV_RCP		1
#define BSVM2_OPUV_SQRT		2
#define BSVM2_OPUV_RSQRT	3
#define BSVM2_OPUV_LEN		4
#define BSVM2_OPUV_LEN2		5
#define BSVM2_OPUV_NORM		6

#define BSVM2_OPVZ_V4I		0
#define BSVM2_OPVZ_V2L		1
#define BSVM2_OPVZ_V4F		2
#define BSVM2_OPVZ_V2D		3
#define BSVM2_OPVZ_V2I		4
#define BSVM2_OPVZ_V2F		5
#define BSVM2_OPVZ_V3I		6
#define BSVM2_OPVZ_V3F		7
#define BSVM2_OPVZ_I128		8
#define BSVM2_OPVZ_UI128	9
#define BSVM2_OPVZ_F128		10


#define BSVM2_OPZY_STRU8	1
#define BSVM2_OPZY_STRU16	2
#define BSVM2_OPZY_STRASC	3

#define BSVM2_OPZY_INT		5
#define BSVM2_OPZY_LONG		6
#define BSVM2_OPZY_UINT		7
#define BSVM2_OPZY_FLOAT	9
#define BSVM2_OPZY_DOUBLE	10
#define BSVM2_OPZY_RESV0	11

#define BSVM2_OPZY_STRSYM	13
#define BSVM2_OPZY_STRKEY	14
#define BSVM2_OPZY_RESV1	15

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
#define BSVM2_OP_MVA		0x2B	//MOV J to I
#define BSVM2_OP_MVI		0x2C	//MOV J to I
#define BSVM2_OP_MVL		0x2D	//MOV J to I
#define BSVM2_OP_MVF		0x2E	//MOV J to I
#define BSVM2_OP_MVD		0x2F	//MOV J to I
#define BSVM2_OP_JEQ		0x30	//Jump Equal ( (A==0) => )
#define BSVM2_OP_JNE		0x31	//Jump Not-Equal ( (A!=0) => )
#define BSVM2_OP_JLT		0x32	//Jump Less Than ( (A<0) => )
#define BSVM2_OP_JGT		0x33	//Jump Greater Than ( (A>0) => )
#define BSVM2_OP_JLE		0x34	//Jump Less Equal ( (A<=0) => )
#define BSVM2_OP_JGE		0x35	//Jump Greater Equal ( (A>=0) => )
// #define BSVM2_OP_JCMP		0x36	//Jump-Compare ( (A op B) =>)
#define BSVM2_OP_HBC		0x36	//Hint Bounds Check
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

#define BSVM2_OP_HNOEX		0x60	//Non-Execute (Trace)
#define BSVM2_OP_HNOEX1		0x61	//Non-Execute (1 Op)
#define BSVM2_OP_HNOEX2		0x62	//Non-Execute (2 Op)
#define BSVM2_OP_HNOEX3		0x63	//Non-Execute (3 Op)
#define BSVM2_OP_RETC		0x64	//Return Constant

#define BSVM2_OP_UCMPI		0x66	//Unsigned Compare Int
#define BSVM2_OP_UCMPL		0x67	//Unsigned Compare Long
#define BSVM2_OP_CMPIC		0x68	//Compare Int Const
#define BSVM2_OP_CMPIL		0x69	//Compare Int Local
#define BSVM2_OP_CMPILC		0x6A	//Compare Int Local/Const
#define BSVM2_OP_CMPILL		0x6B	//Compare Int Local/Local
#define BSVM2_OP_INCI		0x6C	//Increment Integer
#define BSVM2_OP_DECI		0x6D	//Decrement Integer
#define BSVM2_OP_INCIL		0x6E	//Increment Integer Local
#define BSVM2_OP_DECIL		0x6F	//Decrement Integer Local

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
#define BSVM2_OP_NEWARR		0x89	//Create array instance.
#define BSVM2_OP_DELETE		0x8A	//Delete object/array instance.
#define BSVM2_OP_DELNGBL	0x8B	//Delete instance if not global.

#define BSVM2_OP_HPTR		0x8C	//Hint Pointer

// #define BSVM2_OP_MOVOBJ		0x8C	//Copy object contents ( A B => )
// #define BSVM2_OP_MOVARR		0x8D	//Copy array contents ( A B => )
// #define BSVM2_OP_INCREF		0x8E	//Increment object reference count
// #define BSVM2_OP_DECREF		0x8F	//Decrement object reference count
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
// #define BSVM2_OP_SWAPA		0xAA	//Swap Address
#define BSVM2_OP_HTNULL		0xAA	//Swap Address
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

#define BSVM2_OP_ADDAA		0x0100	//Variant Add
#define BSVM2_OP_SUBAA		0x0101	//Variant Subtract
#define BSVM2_OP_MULAA		0x0102	//Variant Multiply
#define BSVM2_OP_ANDAA		0x0103	//Variant And
#define BSVM2_OP_ORAA		0x0104	//Variant Or
#define BSVM2_OP_XORAA		0x0105	//Variant Xor
#define BSVM2_OP_SHLAA		0x0106	//Variant Shl
#define BSVM2_OP_SARAA		0x0107	//Variant Sar
#define BSVM2_OP_SHRAA		0x0108	//Variant Shr
#define BSVM2_OP_DIVAA		0x0109	//Variant Divide
#define BSVM2_OP_MODAA		0x010A	//Variant Modulo
#define BSVM2_OP_NEGAA		0x010B	//Variant Negate
#define BSVM2_OP_NOTAA		0x010C	//Variant Not
#define BSVM2_OP_LNTAA		0x010D	//Variant LNot
#define BSVM2_OP_CMPAA		0x010E	//Variant Compare
#define BSVM2_OP_LDCA		0x010F	//Load Constant As Variant
#define BSVM2_OP_CVTI2AA	0x0110	//Convert Int->Variant
#define BSVM2_OP_CVTL2AA	0x0111	//Convert Long->Variant
#define BSVM2_OP_CVTF2AA	0x0112	//Convert Float->Variant
#define BSVM2_OP_CVTD2AA	0x0113	//Convert Double->Variant
#define BSVM2_OP_CVTAA2I	0x0114	//Convert Variant->Int
#define BSVM2_OP_CVTAA2L	0x0115	//Convert Variant->Long
#define BSVM2_OP_CVTAA2F	0x0116	//Convert Variant->Float
#define BSVM2_OP_CVTAA2D	0x0117	//Convert Variant->Double
#define BSVM2_OP_CVTAA2IN	0x0118	//Convert Variant->Int (Strict)
#define BSVM2_OP_CVTAA2LN	0x0119	//Convert Variant->Long (Strict)
#define BSVM2_OP_CVTAA2FN	0x011A	//Convert Variant->Float (Strict)
#define BSVM2_OP_CVTAA2DN	0x011B	//Convert Variant->Double (Strict)
#define BSVM2_OP_MVIC		0x011C	//MOV C to I
#define BSVM2_OP_MVLC		0x011D	//MOV C to I
#define BSVM2_OP_MVFC		0x011E	//MOV C to I
#define BSVM2_OP_MVDC		0x011F	//MOV C to I
#define BSVM2_OP_LDTHIS		0x0120	//Load This Variable ( => A )
#define BSVM2_OP_STTHIS		0x0121	//Store This Variable ( V => )
#define BSVM2_OP_CALLTH		0x0122	//Call This Method
#define BSVM2_OP_DCMPIC		0x0123	//DUPI+CMPIC
#define BSVM2_OP_DCJEQIC	0x0124	//DUPI+CMPIC+JEQ
#define BSVM2_OP_DCJNEIC	0x0125	//DUPI+CMPIC+JNE
#define BSVM2_OP_DCJLTIC	0x0126	//DUPI+CMPIC+JLT
#define BSVM2_OP_DCJGTIC	0x0127	//DUPI+CMPIC+JGT
#define BSVM2_OP_DCJLEIC	0x0128	//DUPI+CMPIC+JLE
#define BSVM2_OP_DCJGEIC	0x0129	//DUPI+CMPIC+JGE
#define BSVM2_OP_CJEQIC		0x012A	//CMPIC+JEQ
#define BSVM2_OP_CJNEIC		0x012B	//CMPIC+JNE
#define BSVM2_OP_CJLTIC		0x012C	//CMPIC+JLT
#define BSVM2_OP_CJGTIC		0x012D	//CMPIC+JGT
#define BSVM2_OP_CJLEIC		0x012E	//CMPIC+JLE
#define BSVM2_OP_CJGEIC		0x012F	//CMPIC+JGE
#define BSVM2_OP_LEARI		0x0130
#define BSVM2_OP_LEARL		0x0131
#define BSVM2_OP_LEARF		0x0132
#define BSVM2_OP_LEARD		0x0133
#define BSVM2_OP_LEARA		0x0134
#define BSVM2_OP_LEARB		0x0135
#define BSVM2_OP_LEARS		0x0136
#define BSVM2_OP_LEAST		0x0137
#define BSVM2_OP_LEARIC		0x0138
#define BSVM2_OP_LEARLC		0x0139
#define BSVM2_OP_LEARFC		0x013A
#define BSVM2_OP_LEARDC		0x013B
#define BSVM2_OP_LEARAC		0x013C
#define BSVM2_OP_LEARBC		0x013D
#define BSVM2_OP_LEARSC		0x013E
#define BSVM2_OP_LEASTC		0x013F
#define BSVM2_OP_IFXOBJ		0x0140
#define BSVM2_OP_IFXARR		0x0141
#define BSVM2_OP_DFXOBJ		0x0142
#define BSVM2_OP_DFXARR		0x0143
#define BSVM2_OP_LBLCLNP	0x0144
#define BSVM2_OP_STIXNC		0x0145
#define BSVM2_OP_DSTIXNC	0x0146
#define BSVM2_OP_NEWDYO		0x0147
#define BSVM2_OP_SBOS		0x0148
#define BSVM2_OP_DSBOS		0x0149
#define BSVM2_OP_DSTOS		0x014A
#define BSVM2_OP_IFXOBJA	0x014B
#define BSVM2_OP_LDDRAL		0x014C
#define BSVM2_OP_LDDRVL		0x014D
#define BSVM2_OP_STDRAL		0x014E
#define BSVM2_OP_STDRVL		0x014F
#define BSVM2_OP_LDDRI		0x0150
#define BSVM2_OP_LDDRL		0x0151
#define BSVM2_OP_LDDRF		0x0152
#define BSVM2_OP_LDDRD		0x0153
#define BSVM2_OP_STDRI		0x0154
#define BSVM2_OP_STDRL		0x0155
#define BSVM2_OP_STDRF		0x0156
#define BSVM2_OP_STDRD		0x0157
#define BSVM2_OP_LDDRSB		0x0158
#define BSVM2_OP_LDDRUB		0x0159
#define BSVM2_OP_LDDRSS		0x015A
#define BSVM2_OP_LDDRUS		0x015B
#define BSVM2_OP_LDDRA		0x015C
#define BSVM2_OP_STDRA		0x015D
#define BSVM2_OP_STDRB		0x015E
#define BSVM2_OP_STDRS		0x015F
#define BSVM2_OP_DUPAA		0x0160
#define BSVM2_OP_DROPAA		0x0161
#define BSVM2_OP_DTRAPNULL	0x0162
#define BSVM2_OP_LTRAPNULL	0x0163
#define BSVM2_OP_LDIXAA		0x0164	//Load Index Variant
#define BSVM2_OP_STIXAA		0x0165	//Store Index Variant
#define BSVM2_OP_LDIXAI		0x0166	//Load Index Variant[Int]
#define BSVM2_OP_STIXAI		0x0167	//Store Index Variant[Int]
#define BSVM2_OP_ISTYPE		0x0168	//Check if address is type
#define BSVM2_OP_DTNISTYPE	0x0169	//Trap if address not type
#define BSVM2_OP_DZNISTYPE	0x016A	//Null if address not type
#define BSVM2_OP_AGETI		0x016B	//Check if address is type
#define BSVM2_OP_LDOSL		0x016C	//Check if address is type
#define BSVM2_OP_STOSL		0x016D	//Check if address is type

#define BSVM2_OP_DIVI		0x0170	//
#define BSVM2_OP_DIVL		0x0171	//
#define BSVM2_OP_MODI		0x0172	//
#define BSVM2_OP_MODL		0x0173	//
#define BSVM2_OP_SHRI		0x0174	//
#define BSVM2_OP_SHRL		0x0175	//
#define BSVM2_OP_UDIVI		0x0176	//
#define BSVM2_OP_UDIVL		0x0177	//
#define BSVM2_OP_CMIEQ		0x0178	//
#define BSVM2_OP_CMINE		0x0179	//
#define BSVM2_OP_CMILT		0x017A	//
#define BSVM2_OP_CMIGT		0x017B	//
#define BSVM2_OP_CMILE		0x017C	//
#define BSVM2_OP_CMIGE		0x017D	//
#define BSVM2_OP_LDIXZLL	0x017E	//Load Index Z-LL ( A, I; => V )
#define BSVM2_OP_LDIXZLC	0x017F	//Load Index Z-LC ( A, C; => V )
#define BSVM2_OP_RSTIXI		0x0180	//Store Index Int ( V A I => )
#define BSVM2_OP_RSTIXL		0x0181	//Store Index Long ( V A I => )
#define BSVM2_OP_RSTIXF		0x0182	//Store Index Float ( V A I => )
#define BSVM2_OP_RSTIXD		0x0183	//Store Index Double ( V A I => )
#define BSVM2_OP_RSTIXA		0x0184	//Store Index Address ( V A I => )
#define BSVM2_OP_RSTIXB		0x0185	//Store Index Byte ( V A I => )
#define BSVM2_OP_RSTIXS		0x0186	//Store Index Short ( V A I => )
#define BSVM2_OP_STIXZLL	0x0187	//Store Index Z-LL ( A, I; V => )
#define BSVM2_OP_RSTIXIC	0x0188	//Store Index Int ( I; V A => )
#define BSVM2_OP_RSTIXLC	0x0189	//Store Index Long ( I; V A => )
#define BSVM2_OP_RSTIXFC	0x018A	//Store Index Float ( I; V A => )
#define BSVM2_OP_RSTIXDC	0x018B	//Store Index Double ( I; V A => )
#define BSVM2_OP_RSTIXAC	0x018C	//Store Index Address ( I; V A => )
#define BSVM2_OP_RSTIXBC	0x018D	//Store Index Byte ( I; V A => )
#define BSVM2_OP_RSTIXSC	0x018E	//Store Index Short ( I; V A => )
#define BSVM2_OP_STIXZLC	0x018F	//Store Index Z-LC ( A, C; V => )
#define BSVM2_OP_CVTAA2ST	0x0190	//
#define BSVM2_OP_CVTST2AA	0x0191	//
#define BSVM2_OP_CATST		0x0192	//
#define BSVM2_OP_CMPST		0x0193	//
#define BSVM2_OP_CMPSST		0x0194	//
#define BSVM2_OP_CMPUST		0x0195	//
#define BSVM2_OP_MATHUFN	0x0196	//
#define BSVM2_OP_MATHBFN	0x0197	//
#define BSVM2_OP_STFXOBJC	0x0198	//Store Fixed Object (Copy)
#define BSVM2_OP_STFXOBJD	0x0199	//Store Fixed Object (Destroy)
#define BSVM2_OP_STIXZAC	0x019A	//
#define BSVM2_OP_DSTIXZAC	0x019B	//
#define BSVM2_OP_JEQNULL	0x019C	//
#define BSVM2_OP_JNENULL	0x019D	//
#define BSVM2_OP_JEQNULLL	0x019E	//
#define BSVM2_OP_JNENULLL	0x019F	//
#define BSVM2_OP_CVTF2HF	0x01A0	//
#define BSVM2_OP_CVTD2HF	0x01A1	//
#define BSVM2_OP_CVTHF2F	0x01A2	//
#define BSVM2_OP_CVTHF2D	0x01A3	//
#define BSVM2_OP_SWAPI		0x01A4	//Swap Int
#define BSVM2_OP_SWAPL		0x01A5	//Swap Long
#define BSVM2_OP_SWAPF		0x01A6	//Swap Float
#define BSVM2_OP_SWAPD		0x01A7	//Swap Double
#define BSVM2_OP_DSTIXIC	0x01A8	//
#define BSVM2_OP_DSTIXLC	0x01A9	//
#define BSVM2_OP_DSTIXFC	0x01AA	//
#define BSVM2_OP_DSTIXDC	0x01AB	//
#define BSVM2_OP_DSTIXAC	0x01AC	//
#define BSVM2_OP_DSTIXBC	0x01AD	//
#define BSVM2_OP_DSTIXSC	0x01AE	//
#define BSVM2_OP_SWAPA		0x01AF	//Swap Address

#define BSVM2_OP_CVTA2PTR	0x01B0	//Address to Pointer
#define BSVM2_OP_CVTPTR2A	0x01B1	//Pointer to Address



#define BSVM2_OP_MKX4I		0x0400
#define BSVM2_OP_MKX2L		0x0401
#define BSVM2_OP_MKX4F		0x0402
#define BSVM2_OP_MKX2D		0x0403
#define BSVM2_OP_DUPX		0x0404
#define BSVM2_OP_POPX		0x0405
#define BSVM2_OP_LDXL		0x0406
#define BSVM2_OP_STXL		0x0407
#define BSVM2_OP_IFXX		0x0408
#define BSVM2_OP_DFXX		0x0409
#define BSVM2_OP_IFXXA		0x040A
#define BSVM2_OP_LDXX		0x040B
#define BSVM2_OP_DLDXX		0x040C
#define BSVM2_OP_STXX		0x040D
#define BSVM2_OP_BINOPX		0x040E
#define BSVM2_OP_UNOPX		0x040F

#define BSVM2_OP_CVTI2XL	0x0410
#define BSVM2_OP_CVTL2XL	0x0411
#define BSVM2_OP_CVTUI2XL	0x0412
#define BSVM2_OP_CVTUL2XL	0x0413
#define BSVM2_OP_CVTXL2I	0x0414
#define BSVM2_OP_CVTXL2L	0x0415
#define BSVM2_OP_CVTD2XF	0x0416
#define BSVM2_OP_CVTXF2D	0x0417
#define BSVM2_OP_CVTXL2XF	0x0418
#define BSVM2_OP_CVTXF2XL	0x0419
#define BSVM2_OP_CVTXL2AA	0x041A
#define BSVM2_OP_CVTXF2AA	0x041B
#define BSVM2_OP_CVTAA2XL	0x041C
#define BSVM2_OP_CVTAA2XF	0x041D

#define BSVM2_OP_CVTCF2AA	0x0420	//
#define BSVM2_OP_CVTCD2AA	0x0421	//
#define BSVM2_OP_CVTVI2AA	0x0422	//
#define BSVM2_OP_CVTVF2AA	0x0423	//
#define BSVM2_OP_CVTAA2VI	0x0424	//
#define BSVM2_OP_CVTAA2VF	0x0425	//
#define BSVM2_OP_CVTAA2X4I	0x0426	//
#define BSVM2_OP_CVTAA2X4F	0x0427	//
#define BSVM2_OP_CVTX4F2AA	0x0428	//
#define BSVM2_OP_CVTX3F2AA	0x0429	//
#define BSVM2_OP_CVTXQF2AA	0x042A	//
#define BSVM2_OP_CVTX2D2AA	0x042B	//
#define BSVM2_OP_CVTV2F2X2D	0x042C	//
#define BSVM2_OP_CVTX2D2V2F	0x042D	//
#define BSVM2_OP_CVTV2F2X4F	0x042E	//
#define BSVM2_OP_CVTX4F2V2F	0x042F	//


#define BSVM2_OP_MKV2I		0x0430	//
#define BSVM2_OP_MKV2F		0x0431	//

#define BSVM2_OP_SWEV2I		0x0432	//Swap A and B
#define BSVM2_OP_SWEV2F		0x0433	//Swap A and B
#define BSVM2_OP_SWEX2L		0x0434	//Swap A and B
#define BSVM2_OP_SWEX2D		0x0435	//Swap A and B
#define BSVM2_OP_SHUFX4I	0x0436	//Shuffle Elements
#define BSVM2_OP_SHUFX4F	0x0437	//Shuffle Elements

#define BSVM2_OP_LDV2IA		0x0440	//
#define BSVM2_OP_LDV2IB		0x0441	//
#define BSVM2_OP_LDV2FA		0x0442	//
#define BSVM2_OP_LDV2FB		0x0443	//
#define BSVM2_OP_LDX2LA		0x0444	//
#define BSVM2_OP_LDX2LB		0x0445	//
#define BSVM2_OP_LDX2DA		0x0446	//
#define BSVM2_OP_LDX2DB		0x0447	//
#define BSVM2_OP_LDX4IA		0x0448	//
#define BSVM2_OP_LDX4IB		0x0449	//
#define BSVM2_OP_LDX4IC		0x044A	//
#define BSVM2_OP_LDX4ID		0x044B	//
#define BSVM2_OP_LDX4FA		0x044C	//
#define BSVM2_OP_LDX4FB		0x044D	//
#define BSVM2_OP_LDX4FC		0x044E	//
#define BSVM2_OP_LDX4FD		0x044F	//


#define BSVM2_TRFL_TJNEXT		0x00001		//tail is simply jnext
#define BSVM2_TRFL_CANTHROW		0x00002		//trace can throw

#define BSVM2_TRFL_SAVEDESI		0x00100		//saved ESI
#define BSVM2_TRFL_SAVEDEDI		0x00200		//saved EDI
#define BSVM2_TRFL_SAVEDEBX		0x00400		//saved EBX
#define BSVM2_TRFL_SAVEDEBP		0x00800		//saved EBP
#define BSVM2_TRFL_FRMONSTK		0x01000		//frame on stack
#define BSVM2_TRFL_FRMINEDI		0x02000		//frame in EDI
#define BSVM2_TRFL_OPSINESI		0x04000		//ops in ESI
#define BSVM2_TRFL_STKINEBX		0x08000		//VM stack in EBX
#define BSVM2_TRFL_LCLINESI		0x10000		//locals in ESI

#define BSVM2_EXS_NONE			0x00000		//no error status
#define BSVM2_EXS_NULLEX		0x00001		//NullPointerException
#define BSVM2_EXS_BOUNDEX		0x00002		//BoundsCheckException
#define BSVM2_EXS_RUNLIMIT		0x00003		//Interpreter Run-Limit
#define BSVM2_EXS_CASTEX		0x00004		//CastException

#ifndef BSVM2_DBGTRAP
#if defined(_M_IX86) && defined(_MSC_VER)
#define BSVM2_DBGTRAP	__asm { int 3 }
#else
#define BSVM2_DBGTRAP	*(int *)-1=-1;
#endif
#endif

typedef union BSVM2_Value_u BSVM2_Value;
typedef union BSVM2_ValX128_u BSVM2_ValX128;
typedef struct BSVM2_Opcode_s BSVM2_Opcode;
typedef struct BSVM2_TailOpcode_s BSVM2_TailOpcode;
typedef struct BSVM2_Trace_s BSVM2_Trace;

// typedef struct BSVM2_Frame_s BSVM2_Frame;
typedef struct BSVM2_Frame_s BSVM2_Frame;

typedef struct BSVM2_Context_s BSVM2_Context;

typedef struct BSVM2_ImageGlobal_s BSVM2_ImageGlobal;
typedef struct BSVM2_CodeBlock_s BSVM2_CodeBlock;
typedef struct BSVM2_CodeImage_s BSVM2_CodeImage;

union BSVM2_Value_u {
struct { s32 i, j; };
struct { u32 ui, uj; };
struct { f32 fx, fy; };
s64 l;
u64 ul;
f32 f;
f64 d;
dtVal a;
void *p;
};

union BSVM2_ValX128_u {
struct { s32 ia, ib, ic, id; };
struct { u32 ua, ub, uc, ud; };
struct { f32 fx, fy, fz, fw; };
struct { s64 la, lb; };
struct { u64 ula, ulb; };
struct { f64 dx, dy; };
};

struct BSVM2_Opcode_s {
void (*Run)(BSVM2_Frame *frm, BSVM2_Opcode *op);
int i0, i1, i2;
int t0, t1, t2;
short opn, opn2;
int opfl;
BSVM2_Value v;
};

struct BSVM2_TailOpcode_s {
BSVM2_Trace *(*Run)(BSVM2_Frame *frm,
	BSVM2_TailOpcode *op);
BSVM2_Trace *nexttrace;
BSVM2_Trace *jmptrace;
byte *jcs;
int i0, i1;
int t0, t1;
short opn, opn2;
BSVM2_Value v;
};

struct BSVM2_Trace_s {
BSVM2_Opcode **ops;
BSVM2_TailOpcode *top;
BSVM2_Trace *(*Run)(BSVM2_Frame *frm,
	BSVM2_Trace *tr);
BSVM2_Trace *jnext;		//next trace to jump to
byte *cs;				//bytecode addr for trace
byte *jcs;				//bytecode addr for jump
void *t_ops[6];
int n_ops;
int trfl;
};

struct BSVM2_Frame_s {
BSVM2_Context *ctx;		//owning context
BSVM2_Value *stack;		//stack base
BSVM2_Value *local;		//locals base

BSVM2_Frame *rnext;		//return frame
BSVM2_Trace *rtrace;	//return trace
int rcsrv;				//return call save return value
int tstkpos;			//tstackpos
dtVal self;				//essentially, the 'this' value.
};

struct BSVM2_Context_s {
BSVM2_Context *next;		//next context
BSVM2_Frame *cstack;		//call stack
BSVM2_Value *tstack;		//temp stack (locals and stack)
int tstackref;				//ref position of temp stack

BSVM2_Frame *freeframe;		//free frames
void *freex128;				//free X128 vectors

BSVM2_Frame *frame;			//current frame
BSVM2_Trace *trace;			//current trace
int status;
};

struct BSVM2_CodeBlock_s {
BSVM2_CodeImage *img;
BSVM2_ImageGlobal *func;
byte *cs, *cse;
byte *code;
BSVM2_Trace **trace;
int *gitab;
int *altab;
int tgitab[8];
int szcode;

short ntrace;
short stkpos;

short bargs;
short stkdepth;
short ngi;
short largs;
short szframe;
// byte alrov;
};

struct BSVM2_ImageGlobal_s {
char *name;
char *qname;
char *sig;
char *flagstr;
int *figix;				//field/package GIX
int *ifgix;				//interface GIX
int gix;
short nargs;
short nfigix;
short nifgix;
// int sugix;			//superclass GIX
int giobj;				//GIX of owner or superclass
u32 tag;

s64 nameh;
s64 qnameh;

BSVM2_Trace *ctrace;	//call trace
byte brty;				//base return type (functions)
byte baty[32];			//base arg type (functions)

BSVM2_Value *gvalue;	//global value
void *objinf;

BSVM2_CodeImage *img;
BSVM2_ImageGlobal *pkg;
BSVM2_ImageGlobal *obj;
BSVM2_CodeBlock *cblk;
};

struct BSVM2_CodeImage_s {
BSVM2_CodeImage *next;

byte *data;		//image data
int szdata;		//size of image data
byte *strtab;	//string table
byte *estrtab;	//end of string table

byte *gblsdat;	//globals data area
byte *egblsdat;	//end of globals data area

byte *gixtab;
byte gixstr;

byte *pvbm;		//public visible bitmap
byte *epvbm;	//end of public visible bitmap

BSVM2_ImageGlobal **gbls;
int ngbls;

int tmaix[8];
int *maix;
int nmaix;

int seqid;

BSVM2_Opcode *opfree;
BSVM2_TailOpcode *topfree;
BSVM2_Trace *trfree;
};
