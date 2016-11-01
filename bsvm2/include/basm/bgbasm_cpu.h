/*
stuff related to the assembler and jit compiler.
opcodes will be represented by hex strings:
"04" ADD al, imm8
"05" ADD eax, imm32

registers will have the low 4 bits as a reg num, and the high 4 will identify
the type: 0=byte, 1=word, 2=dword, 3=qword, 4=opcode ext, 5=mmx, 6=xmm
*/

#ifndef BGBASM_CPU_H
#define BGBASM_CPU_H


#define BASM_TOKEN_NAME				1
#define BASM_TOKEN_NUMBER			2
#define BASM_TOKEN_STRING			3
#define BASM_TOKEN_CHARSTRING		4
#define BASM_TOKEN_SEPERATOR		5
#define BASM_TOKEN_OPERATOR			6
#define BASM_TOKEN_BRACE			7
#define BASM_TOKEN_MISC				8
#define BASM_TOKEN_NULL				9

#define BASM_CPUID_EXT_SSE3			1
#define BASM_CPUID_EXT_MONITOR		8
#define BASM_CPUID_EXT_DS_CPL		16
#define BASM_CPUID_EXT_VMX			32
#define BASM_CPUID_EXT_EST			128
#define BASM_CPUID_EXT_TM2			256
#define BASM_CPUID_EXT_SSSE3		512
#define BASM_CPUID_EXT_CNXT_ID		1024
#define BASM_CPUID_EXT_CMPXCHG16B	8192
#define BASM_CPUID_EXT_XTPR			16384

#define BASM_CPUID_FPU		0x00000001
#define BASM_CPUID_VME		0x00000002
#define BASM_CPUID_DE		0x00000004
#define BASM_CPUID_PSE		0x00000008
#define BASM_CPUID_TSC		0x00000010
#define BASM_CPUID_MSR		0x00000020
#define BASM_CPUID_PAE		0x00000040
#define BASM_CPUID_MCE		0x00000080
#define BASM_CPUID_CXB		0x00000100
#define BASM_CPUID_APIC		0x00000200
#define BASM_CPUID_SEP		0x00000800
#define BASM_CPUID_MTRR		0x00001000
#define BASM_CPUID_PGE		0x00002000
#define BASM_CPUID_MCA		0x00004000
#define BASM_CPUID_CMOV		0x00008000
#define BASM_CPUID_PAT		0x00010000
#define BASM_CPUID_PSE36	0x00020000
#define BASM_CPUID_PSN		0x00040000
#define BASM_CPUID_CFLSH	0x00080000
#define BASM_CPUID_DS		0x00200000
#define BASM_CPUID_ACPI		0x00400000
#define BASM_CPUID_MMX		0x00800000
#define BASM_CPUID_FXSR		0x01000000
#define BASM_CPUID_SSE		0x02000000
#define BASM_CPUID_SSE2		0x04000000
#define BASM_CPUID_SS		0x08000000
#define BASM_CPUID_HTT		0x10000000
#define BASM_CPUID_TM		0x20000000
#define BASM_CPUID_PBE		0x80000000

#define BASM_Z	-1

#include "bgbasm_opcat.h"

#define BASM_CPU_X86		1
#define BASM_CPU_X64		2
#define BASM_CPU_ARM		3
#define BASM_CPU_THUMB		4

#define BASM_CPU_NACL_X86	5
#define BASM_CPU_NACL_X64	6


#define BASM_JMP_SHORT		1
#define BASM_JMP_NEAR16		2
#define BASM_JMP_NEAR32		3
#define BASM_JMP_ABS16		4
#define BASM_JMP_ABS32		5
#define BASM_JMP_ABS64		6

#define BASM_JMP_NEAR64		7
#define BASM_JMP_ABSREL32	8

#define BASM_JMP_ARM_NEAR24			16
#define BASM_JMP_THUMB_NEAR8		17
#define BASM_JMP_THUMB_NEAR11		18
#define BASM_JMP_THUMB_NEAR22		19

#define BASM_COND_O		0x0	//of
#define BASM_COND_NO	0x1	//!of
#define BASM_COND_B		0x2	//cf
#define BASM_COND_C		0x2
#define BASM_COND_NAE	0x2
#define BASM_COND_AE	0x3	//!cf
#define BASM_COND_NB	0x3
#define BASM_COND_NC	0x3
#define BASM_COND_E		0x4	//zf
#define BASM_COND_Z		0x4
#define BASM_COND_NE	0x5	//!zf
#define BASM_COND_NZ	0x5
#define BASM_COND_BE	0x6	//cf|zf
#define BASM_COND_NA	0x6
#define BASM_COND_A		0x7	//!(cf|zf)
#define BASM_COND_NBE	0x7
#define BASM_COND_S		0x8	//sf
#define BASM_COND_NS	0x9	//!sf
#define BASM_COND_P		0xA	//pf
#define BASM_COND_PE	0xA
#define BASM_COND_NP	0xB	//!pf
#define BASM_COND_PO	0xB
#define BASM_COND_L		0xC	//sf^of
#define BASM_COND_NGE	0xC
#define BASM_COND_GE	0xD	//!(sf^of)
#define BASM_COND_NL	0xD
#define BASM_COND_LE	0xE	//(sf^of)|zf
#define BASM_COND_NG	0xE
#define BASM_COND_G		0xF	//!((sf^of)|zf)
#define BASM_COND_NLE	0xF

#define BS1_JITFL_EAX		1	//top of stack in eax
#define BS1_JITFL_EAXFN		2	//eax is a machine integer
#define BS1_JITFL_ECX_EARG	4	//ecx contains eargs

#define BASM_FL_X86_64		1
#define BASM_FL_X86_16		2
#define BASM_FL_DATA		4
#define BASM_FL_ADDRSZ		8
#define BASM_FL_GAS			16
#define BASM_FL_ARM			32
#define BASM_FL_THUMB		64
#define BASM_FL_BUNDLE		128

#define BASM_FL_ARMCC_MASK	0x00F00000
#define BASM_FL_ARMCC_S		0x00001000
#define BASM_FL_ARMCC_SHL	20

#define BASM_ARM_COND_EQ	0x0
#define BASM_ARM_COND_NE	0x1
#define BASM_ARM_COND_CS	0x2
#define BASM_ARM_COND_CC	0x3
#define BASM_ARM_COND_MI	0x4
#define BASM_ARM_COND_PL	0x5
#define BASM_ARM_COND_VS	0x6
#define BASM_ARM_COND_VC	0x7
#define BASM_ARM_COND_HI	0x8
#define BASM_ARM_COND_LS	0x9
#define BASM_ARM_COND_GE	0xA
#define BASM_ARM_COND_LT	0xB
#define BASM_ARM_COND_GT	0xC
#define BASM_ARM_COND_LE	0xD
#define BASM_ARM_COND_AL	0xE
#define BASM_ARM_COND_Z		0xF


typedef struct {
char **label_name;
int *label_pos;
int n_labels, m_labels;

char **goto_name;
int *goto_pos;
byte *goto_type;
int n_gotos, m_gotos;

char **const_name;
long long *const_value;
int n_const, m_const;

char **proxy_name;
int n_proxy, m_proxy;

char **llbl_name;
int *llbl_pos;
int n_llbl;


char *label_base;	//label base name
byte *base_ip;		//base IP (PIC)
char *modname;

byte *text, *data;
int text_sz, data_sz;
byte *ip, *dp;

int fl;		//&1=x86-64, &2=x86-16, &4=data, &8=addr-override
int seg;	//segment override (per opcode)
int w;		//opcode width
int cpu;

//TempAlloc allocator
void *alloc_block[64];
int alloc_nblock;

byte *alloc_rov;
byte *alloc_srov;
byte *alloc_erov;

}BASM_Context;

#endif
