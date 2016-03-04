#define MSP430_REG_PC	0				//Program Counter
#define MSP430_REG_SP	1				//Stack Pointer
#define MSP430_REG_SR	2				//Status Register
#define MSP430_REG_CG	3				//Constant Generator (0)

#define MSP430_REG_CG1	2
#define MSP430_REG_CG2	3

#define MSP430_SRM_C		0x0001		//Carry
#define MSP430_SRM_Z		0x0002		//Zero
#define MSP430_SRM_N		0x0004		//Negative (Sign)
#define MSP430_SRM_GIE		0x0008		//Interrupt Enable
#define MSP430_SRM_CPUOFF	0x0010		//CPU Off
#define MSP430_SRM_OSCOFF	0x0020		//Oscillator Off
#define MSP430_SRM_SCG0		0x0040
#define MSP430_SRM_SCG1		0x0080
#define MSP430_SRM_V		0x0100		//Overflow

#define MSP430_SRM_ARITH	0x0107		//Arithmetic Flags

#define MSP430_SRB_C		0			//Carry
#define MSP430_SRB_Z		1			//Zero
#define MSP430_SRB_N		2			//Negative (Sign)
#define MSP430_SRB_GIE		3			//Interrupt Enable
#define MSP430_SRB_CPUOFF	4			//CPU Off
#define MSP430_SRB_OSCOFF	5			//Oscillator Off
#define MSP430_SRB_SCG0		6
#define MSP430_SRB_SCG1		7
#define MSP430_SRB_V		8			//Overflow

#define MSP430_OPFL_MEM		0x0001		//Memory Op
#define MSP430_OPFL_IMM		0x0002		//Immediate Op (!MEM)
#define MSP430_OPFL_ABS		0x0002		//Absolute (MEM & ABS)
#define MSP430_OPFL_AINC	0x0004		//Autoincrement
#define MSP430_OPFL_BYTE	0x0008		//Byte Operation
#define MSP430_OPFL_AD		0x0010		//Byte Operation
#define MSP430_OPFL_A20B	0x0020		//20 Bit Address
#define MSP430_OPFL_PCREL	0x0040		//20 Bit Address

#define MSP430_VAM_IMM		0xC0000000	//VA Immediate
#define MSP430_VAM_REG		0xC1000000	//VA Immediate
#define MSP430_VAM_BURN		0xC2000000	//Burning Write

#define MSP430_OPID_UNK			0		//Unknown Opcode
#define MSP430_OPID_ADD			1		//
#define MSP430_OPID_ADDC		2		//
#define MSP430_OPID_AND			3		//
#define MSP430_OPID_BIC			4		//
#define MSP430_OPID_BIS			5		//
#define MSP430_OPID_BIT			6		//
#define MSP430_OPID_BR			7		//
#define MSP430_OPID_CALL		8		//
#define MSP430_OPID_CMP			9		//
#define MSP430_OPID_DADD		10		//
#define MSP430_OPID_JC			11		//
#define MSP430_OPID_JEQ			12		//
#define MSP430_OPID_JGE			13		//
#define MSP430_OPID_JL			14		//
#define MSP430_OPID_JMP			15		//
#define MSP430_OPID_JN			16		//
#define MSP430_OPID_JNC			17		//
#define MSP430_OPID_JNE			18		//
#define MSP430_OPID_MOV			19		//
#define MSP430_OPID_POP			20		//
#define MSP430_OPID_PUSH		21		//
#define MSP430_OPID_RET			22		//
#define MSP430_OPID_RETI		23		//
#define MSP430_OPID_RRA			24		//
#define MSP430_OPID_RRC			25		//
#define MSP430_OPID_SUB			26		//
#define MSP430_OPID_SUBC		27		//
#define MSP430_OPID_SWPB		28		//
#define MSP430_OPID_SXT			29		//
#define MSP430_OPID_XOR			30		//

#define MSP430_OPID_DINTERP		254		//Direct Interpreter

#define MSP430_PERIB_P1IN		0x0020		//Byte Operation
#define MSP430_PERIB_P1OUT		0x0021		//Byte Operation
#define MSP430_PERIB_P1DIR		0x0022		//Byte Operation
#define MSP430_PERIB_P1IFG		0x0023		//Byte Operation
#define MSP430_PERIB_P1IES		0x0024		//Byte Operation
#define MSP430_PERIB_P1IE		0x0025		//Byte Operation
#define MSP430_PERIB_P1SEL		0x0026		//Byte Operation
#define MSP430_PERIB_P1REN		0x0027		//Byte Operation

#define MSP430_PERIB_P2IN		0x0028		//Byte Operation
#define MSP430_PERIB_P2OUT		0x0029		//Byte Operation
#define MSP430_PERIB_P2DIR		0x002A		//Byte Operation
#define MSP430_PERIB_P2IFG		0x002B		//Byte Operation
#define MSP430_PERIB_P2IES		0x002C		//Byte Operation
#define MSP430_PERIB_P2IE		0x002D		//Byte Operation
#define MSP430_PERIB_P2SEL		0x002E		//Byte Operation
#define MSP430_PERIB_P2REN		0x002F		//Byte Operation

#define MSP430_PERIB_P1SEL2		0x0041		//Byte Operation

#define MSP430_PERIB_ADC10DTC0	0x0048		//ADC10 Data Transfer 0
#define MSP430_PERIB_ADC10DTC1	0x0049		//ADC10 Data Transfer 1
#define MSP430_PERIB_ADC10AE0	0x004A		//ADC10 Input Enable 0
#define MSP430_PERIB_ADC10AE1	0x004B		//ADC10 Input Enable 1

#define MSP430_PERIW_IX(V)		((V-0x100)>>1)		//ADC10 Memory

#define MSP430_PERIW_ADC10CTL0	MSP430_PERIW_IX(0x01B0)	//ADC10 Control 0
#define MSP430_PERIW_ADC10CTL1	MSP430_PERIW_IX(0x01B2)	//ADC10 Control 1
#define MSP430_PERIW_ADC10MEM	MSP430_PERIW_IX(0x01B4)	//ADC10 Memory
#define MSP430_PERIW_ADC10SA	MSP430_PERIW_IX(0x01BC)	//ADC10 TSA

#define MSP430_PERIW_MPY	MSP430_PERIW_IX(0x0130)	//Multiply
#define MSP430_PERIW_MPYS	MSP430_PERIW_IX(0x0132)	//Signed Multiply
#define MSP430_PERIW_MAC	MSP430_PERIW_IX(0x0134)	//Multiply Accumulate
#define MSP430_PERIW_MACS	MSP430_PERIW_IX(0x0136)	//Signed Multiply Acc
#define MSP430_PERIW_OP2	MSP430_PERIW_IX(0x0138)	//Operand 2
#define MSP430_PERIW_RESLO	MSP430_PERIW_IX(0x013A)	//Result Low
#define MSP430_PERIW_RESHI	MSP430_PERIW_IX(0x013C)	//Result High
#define MSP430_PERIW_SUMEXT	MSP430_PERIW_IX(0x013E)	//Sum Extension

#define MSP430_ADC10_SC			0x0001		//ADC10 Transfer Start Address
#define MSP430_ADC10_ENC		0x0002		//ADC10 Transfer Start Address
#define MSP430_ADC10_IFG		0x0004		//ADC10 Transfer Start Address
#define MSP430_ADC10_IE			0x0008		//ADC10 Transfer Start Address
#define MSP430_ADC10_ON			0x0010		//ADC10 Transfer Start Address
#define MSP430_ADC10_REFON		0x0020		//ADC10 Transfer Start Address
#define MSP430_ADC10_REF2_5V	0x0040		//ADC10 Transfer Start Address
#define MSP430_ADC10_MSC		0x0080		//ADC10 Transfer Start Address
#define MSP430_ADC10_REFBURST	0x0100		//ADC10 Transfer Start Address
#define MSP430_ADC10_REFOUT		0x0200		//ADC10 Transfer Start Address
#define MSP430_ADC10_SR			0x0400		//ADC10 Transfer Start Address
#define MSP430_ADC10_SHTx		0x1800		//ADC10 Transfer Start Address
#define MSP430_ADC10_SREFx		0xE000		//ADC10 Transfer Start Address

#define MSP430_ERR_NONE			0			//No Error
#define MSP430_ERR_GENERIC		1			//Generic Error
#define MSP430_ERR_INV_READ		2			//Read from invalid address
#define MSP430_ERR_INV_WRITE	3			//Write to invalid address
#define MSP430_ERR_INV_WRROM	4			//Write to ROM
#define MSP430_ERR_INV_OP		5			//Invalid Opcode

#define MSP430_ERR_TRAP_SMC		64			//Self-Modifying Code

#define MSP430_PIN_P1_0			0			//I/O Pin
#define MSP430_PIN_P1_1			1			//I/O Pin
#define MSP430_PIN_P1_2			2			//I/O Pin
#define MSP430_PIN_P1_3			3			//I/O Pin
#define MSP430_PIN_P1_4			4			//I/O Pin
#define MSP430_PIN_P1_5			5			//I/O Pin
#define MSP430_PIN_P1_6			6			//I/O Pin
#define MSP430_PIN_P1_7			7			//I/O Pin
#define MSP430_PIN_P2_0			8			//I/O Pin
#define MSP430_PIN_P2_1			9			//I/O Pin
#define MSP430_PIN_P2_2			10			//I/O Pin
#define MSP430_PIN_P2_3			11			//I/O Pin
#define MSP430_PIN_P2_4			12			//I/O Pin
#define MSP430_PIN_P2_5			13			//I/O Pin
#define MSP430_PIN_P2_6			14			//I/O Pin
#define MSP430_PIN_P2_7			15			//I/O Pin

typedef struct MSP430_Opcode_s MSP430_Opcode;
typedef struct MSP430_Context_s MSP430_Context;

struct MSP430_Opcode_s {
MSP430_Opcode *next;		//next in context
MSP430_Opcode *hnext;		//next in hash
MSP430_Opcode *seqnext;		//next in sequence
MSP430_Opcode *jmpnext;		//next for jumps
u32 pc, pc2;
int opn;
byte dreg, sreg;
byte dfl, sfl;
byte opid;
int doffs, soffs;
MSP430_Opcode *(*Run)(MSP430_Context *ctx, MSP430_Opcode *op);
};

struct MSP430_Context_s {
MSP430_Opcode *freeops;
MSP430_Opcode *ops;
MSP430_Opcode *pc_op;
MSP430_Opcode *ophash[256];
u32 reg[16];
// byte regh[16];
byte *ram;
byte *rom;
byte *ram_cfl;
int szRam;
int szRom, vaRom;
byte errStatus;
u32 errParm1, errParm2;
u64 runCycles;

byte perib[256];
u16 periw[128];
byte drom[256];

MSP430_Opcode *tmp_diop[64];
MSP430_Opcode *tmp_lsrop;
MSP430_Opcode *ophist[64];
byte oprov;
byte dioprov;

float lpinv[16];					//last pin input voltage
float pinv[16];						//pin input voltage
};