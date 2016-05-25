#define OPCAT_NONE	0
#define OPCAT_MEM	1	//generic memory

#define OPCAT_FREG	2	//float/MMX reg
#define OPCAT_FRM	3	//float/MMX reg/memory
#define OPCAT_XREG	4	//SSE reg
#define OPCAT_XRM	5	//SSE reg/memory
#define OPCAT_SR	6	//seg reg
#define OPCAT_CR	7	//control reg
#define OPCAT_DR	8	//debug reg
#define OPCAT_TR	9	//test reg

#define OPCAT_MOFFS16	10	//fixed offset (16 bit)
#define OPCAT_MOFFS32	11	//fixed offset (32 bit)
#define OPCAT_MOFFS64	12	//fixed offset (64 bit)

#define OPCAT_UIMM8	13	//unsigned 8 bit immediate
#define OPCAT_UIMM16	14	//unsigned 16 bit immediate

#define OPCAT_IMM8	16	//8 bit immediate
#define OPCAT_IMM16	17	//16 bit immediate
#define OPCAT_IMM32	18	//32 bit immediate
#define OPCAT_IMM64	19	//64 bit immediate

#define OPCAT_REG8	20	//general purpose reg
#define OPCAT_REG16	21	//general purpose reg
#define OPCAT_REG32	22	//general purpose reg
#define OPCAT_REG64	23	//general purpose reg

#define OPCAT_RM8	24	//general reg/memory
#define OPCAT_RM16	25	//general reg/memory
#define OPCAT_RM32	26	//general reg/memory
#define OPCAT_RM64	27	//general reg/memory

#define OPCAT_REL8	28	//rel address
#define OPCAT_REL16	29	//rel address
#define OPCAT_REL32	30	//rel address
#define OPCAT_REL64	31	//rel address

#define OPCAT_AL	32
#define OPCAT_CL	33
#define OPCAT_DL	34
#define OPCAT_BL	35
#define OPCAT_AH	36
#define OPCAT_CH	37
#define OPCAT_DH	38
#define OPCAT_BH	39

#define OPCAT_AX	40
#define OPCAT_CX	41
#define OPCAT_DX	42
#define OPCAT_BX	43
#define OPCAT_SP	44
#define OPCAT_BP	45
#define OPCAT_SI	46
#define OPCAT_DI	47

#define OPCAT_EAX	48
#define OPCAT_ECX	49
#define OPCAT_EDX	50
#define OPCAT_EBX	51
#define OPCAT_ESP	52
#define OPCAT_EBP	53
#define OPCAT_ESI	54
#define OPCAT_EDI	55

#define OPCAT_RAX	56
#define OPCAT_RCX	57
#define OPCAT_RDX	58
#define OPCAT_RBX	59
#define OPCAT_RSP	60
#define OPCAT_RBP	61
#define OPCAT_RSI	62
#define OPCAT_RDI	63

#define OPCAT_IMM0	64
#define OPCAT_IMM1	65
#define OPCAT_IMM2	66
#define OPCAT_IMM3	67
#define OPCAT_IMM4	68
#define OPCAT_IMM5	69
#define OPCAT_IMM6	70
#define OPCAT_IMM7	71

#define OPCAT_CS	72
#define OPCAT_SS	73
#define OPCAT_DS	74
#define OPCAT_ES	75
#define OPCAT_FS	76
#define OPCAT_GS	77

#define OPCAT_XRV	78	//SSE reg (VEX/XOP vvvv field)
#define OPCAT_XRI	79	//SSE reg (VEX/XOP immed)

#define OPCAT_MM0	80
#define OPCAT_MM1	81
#define OPCAT_MM2	82
#define OPCAT_MM3	83
#define OPCAT_MM4	84
#define OPCAT_MM5	85
#define OPCAT_MM6	86
#define OPCAT_MM7	87

#define OPCAT_YREG	88	//SSE YMM reg
#define OPCAT_YRM	89	//SSE YMM reg/memory
#define OPCAT_YRV	90	//SSE YMM reg (VEX/XOP vvvv field)
#define OPCAT_YRI	91	//SSE YMM reg (VEX/XOP immed)

#define OPCAT_ARMSH	92	//ARM Shift (Shift-Reg, Shift-Imm)

#define OPFL_LONG	1	//only in long mode
#define OPFL_LEG	2	//only in legacy mode
#define OPFL_VEX	4	//uses VEX

#define OPFL_ALEPH	16	//aleph subset (x86)
#define OPFL_BET	32	//bet subset (x86)

#define OPFL_EVEN	16	//even word aligned
#define OPFL_ODD	32	//odd word aligned
