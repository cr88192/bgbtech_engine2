#ifndef BLNK_OBJ_H
#define BLNK_OBJ_H


typedef struct BLNK_COFF_Info_s BLNK_COFF_Info;

struct BLNK_COFF_Info_s
{
BLNK_COFF_Info *next;		//next loaded object
BLNK_COFF_Info *tnext;		//temporary next link

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

#endif
