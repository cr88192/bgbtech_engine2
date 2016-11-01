/*
stuff related to the assembler and jit compiler.
opcodes will be represented by hex strings:
"04" ADD al, imm8
"05" ADD eax, imm32

registers will have the low 4 bits as a reg num, and the high 4 will identify
the type: 0=byte, 1=word, 2=dword, 3=qword, 4=opcode ext, 5=mmx, 6=xmm
*/

#ifndef BLNK_CPU_H
#define BLNK_CPU_H

#define BLNK_CPU_X86		1
#define BLNK_CPU_X64		2
#define BLNK_CPU_ARM		3
#define BLNK_CPU_THUMB		4

#define BLNK_JMP_SHORT		1
#define BLNK_JMP_NEAR16		2
#define BLNK_JMP_NEAR32		3
#define BLNK_JMP_ABS16		4
#define BLNK_JMP_ABS32		5
#define BLNK_JMP_ABS64		6
#define BLNK_JMP_ABS		7
#define BLNK_JMP_ABS32NB	8
#define BLNK_JMP_SECTION	9
#define BLNK_JMP_SECREL		10

#define BLNK_JMP_ARM_NEAR24			16
#define BLNK_JMP_THUMB_NEAR8		17
#define BLNK_JMP_THUMB_NEAR11		18
#define BLNK_JMP_THUMB_NEAR22		19

#define BLNK_FL_X86_64		1
#define BLNK_FL_X86_16		2
#define BLNK_FL_DATA		4
#define BLNK_FL_ADDRSZ		8
#define BLNK_FL_GAS			16
#define BLNK_FL_ARM			32
#define BLNK_FL_THUMB		64

typedef struct BLNK_Object_s BLNK_Object;

struct BLNK_Object_s {
BLNK_Object *next;
char *name;

char **label_name;
int *label_pos;
int n_labels, m_labels;

char **goto_name;
int *goto_pos;
byte *goto_type;
int n_gotos, m_gotos;

char **proxy_name;
int n_proxy, m_proxy;

byte *text, *data;
int text_sz, data_sz;
int flags;
};

#endif
