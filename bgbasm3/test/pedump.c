#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

char **basm_sym_name;
void **basm_sym_addr;
int basm_sym_nsym;
int basm_sym_msym;

void add_sym(char *name, void *addr)
{
	int i;

	i=basm_sym_nsym++;
	basm_sym_name[i]=strdup(name);
	basm_sym_addr[i]=addr;
}

void *fetch_sym(char *name)
{
	int i;

	for(i=0; i<basm_sym_nsym; i++)
		if(!strcmp(basm_sym_name[i], name))
			return(basm_sym_addr[i]);
	return(NULL);
}

void dump_relocs(byte *buf, int offs, int cnt)
{
	int b;
	int i, j, k, l;

	for(i=0; i<cnt; i++)
	{
		b=offs+i*10;
		j=buf[b+0]|(buf[b+1]<<8)|(buf[b+2]<<8)|(buf[b+3]<<8);
		k=buf[b+4]|(buf[b+5]<<8)|(buf[b+6]<<8)|(buf[b+7]<<8);
		l=buf[b+8]|(buf[b+9]<<8);

		printf("\t\t%08X %5d %04X %s\n", j, k, l,
			basm_sym_name[k]);
	}
}

int main(int argc, char *argv[])
{
	int secva[16];
	char *sec_name[16];
	int sec_rlco[16];
	int sec_rlcn[16];

	char nbuf[16];

	void *p, *q;
	byte *buf;
	FILE *fd;
	char *s;
	int sz, peoffs, coff_hdr, symoffs, nsyms;
	int secoffs, nsecs, stroffs;
	int i, j, k, l;

	if(argc>1)
	{
		printf("open %s\n", argv[1]);
		fd=fopen(argv[1], "rb");
	}else
	{
		printf("open %s\n", argv[0]);
		fd=fopen(argv[0], "rb");
	}

	if(!fd)
	{
		printf("fail open\n");
		return(-1);
	}

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=malloc(sz);
	fread(buf, 1, sz, fd);

	fclose(fd);

	printf("EXE %d (0x%X) bytes\n", sz, sz);

	coff_hdr=0;
	if((buf[0]=='M') || (buf[1]=='Z'))
	{
		printf("EXE Sig\n");

		peoffs=buf[0x3C]|(buf[0x3D]<<8)|(buf[0x3E]<<16)|(buf[0x3F]<<24);

		if((buf[peoffs+0]!='P') || (buf[peoffs+1]!='E'))
		{
			printf("PE Sig Fail\n");
			return(-1);
		}

		printf("PE Sig At 0x%X\n", peoffs);
		coff_hdr=peoffs+4;
	}

	if((buf[coff_hdr+0]!=0x4C) || (buf[coff_hdr+1]!=0x01))
	{
		printf("COFF-i386 Sig Fail\n");
		return(-1);
	}

	i=buf[coff_hdr+0]|(buf[coff_hdr+1]<<8);
	printf("\tMachine 0x%04X\n", i);

	symoffs=buf[coff_hdr+8]|(buf[coff_hdr+9]<<8)|
		(buf[coff_hdr+10]<<16)|(buf[coff_hdr+11]<<24);
	nsyms=buf[coff_hdr+12]|(buf[coff_hdr+13]<<8)|
		(buf[coff_hdr+14]<<16)|(buf[coff_hdr+15]<<24);

	printf("\tSyms 0x%X %d\n", symoffs, nsyms);

	stroffs=symoffs+(nsyms*18);

	i=buf[coff_hdr+16]|(buf[coff_hdr+17]<<8);
	printf("Optional Header Size %d\n", i);

	secoffs=coff_hdr+20+i;
	nsecs=buf[coff_hdr+2]|(buf[coff_hdr+3]<<8);

	printf("\tSecs 0x%X %d\n", secoffs, nsecs);

	printf("\nSections:\n");
	for(i=0; i<nsecs; i++)
	{
		j=secoffs+(i*40);

		s=buf+j;
//		k=buf[j+12]|(buf[j+13]<<8)|(buf[j+14]<<16)|(buf[j+15]<<24);

		sec_name[i]=s;
		secva[i]=k;

//		printf("\t%d %s 0x%08X\n", i+1, s, k);

		printf("\t%d %s\n", i+1, s);

		l=buf[j+8]|(buf[j+9]<<8)|(buf[j+10]<<16)|(buf[j+11]<<24);
		k=buf[j+12]|(buf[j+13]<<8)|(buf[j+14]<<16)|(buf[j+15]<<24);
		printf("\t  VA Addr 0x%08X %d\n", k, l);

		l=buf[j+16]|(buf[j+17]<<8)|(buf[j+18]<<16)|(buf[j+19]<<24);
		k=buf[j+20]|(buf[j+21]<<8)|(buf[j+22]<<16)|(buf[j+23]<<24);
		printf("\t  Img Offs 0x%08X %d\n", k, l);

		k=buf[j+24]|(buf[j+25]<<8)|(buf[j+26]<<16)|(buf[j+27]<<24);
		l=buf[j+32]|(buf[j+33]<<8);
		printf("\t  Reloc 0x%08X %d\n", k, l);

		sec_rlco[i]=k;
		sec_rlcn[i]=l;

		k=buf[j+28]|(buf[j+29]<<8)|(buf[j+30]<<16)|(buf[j+31]<<24);
		l=buf[j+34]|(buf[j+35]<<8);
		printf("\t  Line 0x%08X %d\n", k, l);

		k=buf[j+36]|(buf[j+37]<<8)|(buf[j+38]<<16)|(buf[j+39]<<24);
		printf("\t  Flags 0x%08X\n", k);
	}

	i=4096;
	while(i<nsyms)i=i+(i>>1);
	basm_sym_name=malloc(i*sizeof(char *));
	basm_sym_addr=malloc(i*sizeof(void *));
	basm_sym_nsym=0;
	basm_sym_msym=i;

	printf("\nSymbols:\n");
	for(i=0; i<nsyms; i++)
	{
		j=symoffs+(i*18);

		if(buf[j])
		{
			memset(nbuf, 0, 16);
			memcpy(nbuf, buf+j, 8);
			s=nbuf;
		}else
		{
			k=buf[j+4]|(buf[j+5]<<8)|(buf[j+6]<<16)|(buf[j+7]<<24);
			s=buf+stroffs+k;
		}

		k=buf[j+8]|(buf[j+9]<<8)|(buf[j+10]<<16)|(buf[j+11]<<24);
		l=buf[j+12]|(buf[j+13]<<8);

//		if((l>0) && (l<=nsecs))k+=secva[l-1];

//		if((*s=='.') || ((s[0]=='_') && (s[1]=='_')))
//		{
//			i+=buf[j+17];
//			continue;
//		}

		printf("\t%s 0x%08X %d %d  T: %d %d\n", s, k, l, buf[j+16],
			buf[j+14], buf[j+15]);

		add_sym(s, (void *)k);

//		i+=buf[j+17];
	}

	p=fetch_sym("_main");
	q=&main;

	k=(int)(q-p);
	printf("Displacement %X\n", k);
	for(i=0; i<basm_sym_nsym; i++)
		basm_sym_addr[i]+=k;

	for(i=0; i<basm_sym_nsym; i++)
	{
//		printf("\t%s 0x%08X\n", basm_sym_name[i],
//			basm_sym_addr[i]);
	}

	printf("\nSections:\n");
	for(i=0; i<nsecs; i++)
	{
		printf("\t%d %s\n", i+1, sec_name[i]);
		dump_relocs(buf, sec_rlco[i], sec_rlcn[i]);
	}

	return(0);
}
