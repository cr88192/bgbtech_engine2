#define BASM_IMAGE_FILE_MACHINE_UNKNOWN		0x0000
#define BASM_IMAGE_FILE_MACHINE_ALPHA		0x0184
#define BASM_IMAGE_FILE_MACHINE_ARM		0x01c0
#define BASM_IMAGE_FILE_MACHINE_ALPHA64		0x0284
#define BASM_IMAGE_FILE_MACHINE_I386		0x014c
#define BASM_IMAGE_FILE_MACHINE_IA64		0x0200
#define BASM_IMAGE_FILE_MACHINE_M68K		0x0268
#define BASM_IMAGE_FILE_MACHINE_MIPS16		0x0266
#define BASM_IMAGE_FILE_MACHINE_MIPSFPU		0x0366
#define BASM_IMAGE_FILE_MACHINE_MIPSFPU16	0x0466
#define BASM_IMAGE_FILE_MACHINE_POWERPC		0x01f0
#define BASM_IMAGE_FILE_MACHINE_R3000		0x0162
#define BASM_IMAGE_FILE_MACHINE_R4000		0x0166
#define BASM_IMAGE_FILE_MACHINE_R10000		0x0168
#define BASM_IMAGE_FILE_MACHINE_SH3		0x01a2
#define BASM_IMAGE_FILE_MACHINE_SH4		0x01a6
#define BASM_IMAGE_FILE_MACHINE_THUMB		0x01c2

// #define BASM_IMAGE_FILE_MACHINE_I386		0x014c
// #define BASM_IMAGE_FILE_MACHINE_IA64		0x0200
#define BASM_IMAGE_FILE_MACHINE_EBC		0x0EBC
#define BASM_IMAGE_FILE_MACHINE_X64		0x8664


typedef struct BASM_COFF_Info_s BASM_COFF_Info;
typedef struct BASM_PE_Info_s BASM_PE_Info;


struct BASM_COFF_Info_s
{
BASM_COFF_Info *next;		//next loaded object
BASM_COFF_Info *tnext;		//temporary next link

char *name;	//object file name
byte *buf;	//object file buffer
int bsz;	//object fize size

byte *text;	//location of in-memory code
byte *data;	//location of in-memory data (data, bss, ...)
int tsz, dsz;	//size of in-memory code/data

int peoffs, coff_hdr, symoffs, nsyms;
int secoffs, nsecs, stroffs;


char *sec_name[16];
int sec_flag[16];

int sec_rva_off[16];
int sec_rva_sz[16];
int sec_buf_off[16];
int sec_buf_sz[16];
int sec_rlc_off[16];
int sec_rlc_num[16];

char **sym_name;	//symbol name
void **sym_addr;	//symbol address, internal or external
int *sym_offs;		//offset of symbol
int *sym_sec;		//section for symbol
byte *sym_stype;		//storage type for symbol
};


struct BASM_PE_Info_s
{
BASM_PE_Info *next;		//next loaded object
BASM_PE_Info *tnext;		//temporary next link

char *name;	//object file name
byte *buf;	//object file buffer
int bsz;	//object fize size

int peoffs, coff_hdr, symoffs, nsyms;
int secoffs, nsecs, stroffs, mach;
int nddir, ddiroffs;

s64 img_base;
int img_size;
byte *img_addr;		//image address

int ddir_rva[32];
int ddir_sz[32];

char *sec_name[16];
int sec_flag[16];

int sec_rva_off[16];
int sec_rva_sz[16];
int sec_buf_off[16];
int sec_buf_sz[16];
int sec_rlc_off[16];
int sec_rlc_num[16];

char **sym_name;	//symbol name
void **sym_addr;	//symbol address, internal or external
int *sym_rva;		//offset of symbol
int *sym_offs;		//offset of symbol
int *sym_sec;		//section for symbol
byte *sym_stype;		//storage type for symbol
};

