/*
use istruction table to generate assembler data.
each instruction will be given an ID code, which will identify its opcode name.
this will be part of the public interface.

each opcode will itself be sorted into categories.
the first byte will give the type of the first arg;
the second will give the type of the second;
the third will give the type of the third;
the fourth will give the bit size (generic).

change:
the fourth will give flags

it will be assumed that more preferable encodings precede less preferable ones
in the case where multiple cases are possible.

an opcode will be given multiple entries if it accepts a wider variety of
types.

change (2009-06-23):
expand opcat to 16 bytes.

the first byte will give the type of the first arg;
the second will give the type of the second;
the third will give the type of the third;
the fourth will give the type of the fourth;
the fifth will give the type of the fifth;
the sixth will give the type of the sixth;
the seventh will be reserved.

the eight will give the flags.

bytes 9-15 are reserved.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bgbasm_opcat.h"


char *in_txt="basm_insns.txt";
char *out_c="asm/basm_insns.c";
char *out_h="include/bgbasm_x86ops.h";

char *ops[16384];	//op names
int opidx[16384];	//op index
int nops;

int   opnum [16384];
char  opcat [16384*16];
char *opdecl[16384];
int ndecls;

char *regs[1024];	//register names
int regidx[1024];	//register index
int nregs;

char *kralloc_buffer=NULL;
int kralloc_pos=0;

void *kralloc(int sz)
{
	char *s;

	if(!kralloc_buffer)kralloc_buffer=malloc(262144);
	if((kralloc_pos+sz)>=262144)kralloc_pos=0;

	s=kralloc_buffer+kralloc_pos;
	kralloc_pos+=sz;
	return((void *)s);
}

char **ksplit(char *s)
{
	char **a, *t;
	int i;

	a=kralloc(64*sizeof(char *));
	memset(a, 0, 64*sizeof(char *));

	i=0;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		if(!*s)break;
		if(*s=='#')break;
		if(*s==';')break;
		if((s[0]=='/') && (s[1]=='/'))
			break;

		t=kralloc(256);
		a[i++]=t;

		if(*s=='"')
		{
			s++;
			while(*s && (*s!='"'))*t++=*s++;
			if(*s=='"')s++;

			*t++=0;
			continue;
		}

		while(*s && (*s>' '))*t++=*s++;
		*t++=0;
	}
	a[i]=NULL;

	return(a);
}

int get_opcat(char **str)
{
	char *s;
	int i;

	s=*str;
	i=-1;

	if((*s>='0') && (*s<='7'))	{ i=OPCAT_IMM0+(*s-'0'); s++; }

	if(!strncmp(s, "i8", 2))	{ i=OPCAT_IMM8; s+=2; }
	if(!strncmp(s, "i16", 3))	{ i=OPCAT_IMM16; s+=3; }
	if(!strncmp(s, "i32", 3))	{ i=OPCAT_IMM32; s+=3; }
	if(!strncmp(s, "i64", 3))	{ i=OPCAT_IMM64; s+=3; }

	if(!strncmp(s, "u8", 2))	{ i=OPCAT_UIMM8; s+=2; }
	if(!strncmp(s, "u16", 3))	{ i=OPCAT_UIMM16; s+=3; }

	if(!strncmp(s, "r8", 2))	{ i=OPCAT_REG8; s+=2; }
	if(!strncmp(s, "r16", 3))	{ i=OPCAT_REG16; s+=3; }
	if(!strncmp(s, "r32", 3))	{ i=OPCAT_REG32; s+=3; }
	if(!strncmp(s, "r64", 3))	{ i=OPCAT_REG64; s+=3; }

	if(!strncmp(s, "rm8", 3))	{ i=OPCAT_RM8; s+=3; }
	if(!strncmp(s, "rm16", 4))	{ i=OPCAT_RM16; s+=4; }
	if(!strncmp(s, "rm32", 4))	{ i=OPCAT_RM32; s+=4; }
	if(!strncmp(s, "rm64", 4))	{ i=OPCAT_RM64; s+=4; }


	if(!strncmp(s, "al", 2))	{ i=OPCAT_AL; s+=2; }
	if(!strncmp(s, "cl", 2))	{ i=OPCAT_CL; s+=2; }
	if(!strncmp(s, "dl", 2))	{ i=OPCAT_DL; s+=2; }
	if(!strncmp(s, "bl", 2))	{ i=OPCAT_BL; s+=2; }
	if(!strncmp(s, "ah", 2))	{ i=OPCAT_AH; s+=2; }
	if(!strncmp(s, "ch", 2))	{ i=OPCAT_CH; s+=2; }
	if(!strncmp(s, "dh", 2))	{ i=OPCAT_DH; s+=2; }
	if(!strncmp(s, "bh", 2))	{ i=OPCAT_BH; s+=2; }

	if(!strncmp(s, "ax", 2))	{ i=OPCAT_AX; s+=2; }
	if(!strncmp(s, "cx", 2))	{ i=OPCAT_CX; s+=2; }
	if(!strncmp(s, "dx", 2))	{ i=OPCAT_DX; s+=2; }
	if(!strncmp(s, "bx", 2))	{ i=OPCAT_BX; s+=2; }
	if(!strncmp(s, "sp", 2))	{ i=OPCAT_SP; s+=2; }
	if(!strncmp(s, "bp", 2))	{ i=OPCAT_BP; s+=2; }
	if(!strncmp(s, "si", 2))	{ i=OPCAT_SI; s+=2; }
	if(!strncmp(s, "di", 2))	{ i=OPCAT_DI; s+=2; }

	if(!strncmp(s, "eax", 3))	{ i=OPCAT_EAX; s+=3; }
	if(!strncmp(s, "ecx", 3))	{ i=OPCAT_ECX; s+=3; }
	if(!strncmp(s, "edx", 3))	{ i=OPCAT_EDX; s+=3; }
	if(!strncmp(s, "ebx", 3))	{ i=OPCAT_EBX; s+=3; }
	if(!strncmp(s, "esp", 3))	{ i=OPCAT_ESP; s+=3; }
	if(!strncmp(s, "ebp", 3))	{ i=OPCAT_EBP; s+=3; }
	if(!strncmp(s, "esi", 3))	{ i=OPCAT_ESI; s+=3; }
	if(!strncmp(s, "edi", 3))	{ i=OPCAT_EDI; s+=3; }

	if(!strncmp(s, "rax", 3))	{ i=OPCAT_RAX; s+=3; }
	if(!strncmp(s, "rcx", 3))	{ i=OPCAT_RCX; s+=3; }
	if(!strncmp(s, "rdx", 3))	{ i=OPCAT_RDX; s+=3; }
	if(!strncmp(s, "rbx", 3))	{ i=OPCAT_RBX; s+=3; }
	if(!strncmp(s, "rsp", 3))	{ i=OPCAT_RSP; s+=3; }
	if(!strncmp(s, "rbp", 3))	{ i=OPCAT_RBP; s+=3; }
	if(!strncmp(s, "rsi", 3))	{ i=OPCAT_RSI; s+=3; }
	if(!strncmp(s, "rdi", 3))	{ i=OPCAT_RDI; s+=3; }

	if(!strncmp(s, "st0", 3))	{ i=OPCAT_MM0; s+=3; }
	if(!strncmp(s, "st1", 3))	{ i=OPCAT_MM1; s+=3; }
	if(!strncmp(s, "st2", 3))	{ i=OPCAT_MM2; s+=3; }
	if(!strncmp(s, "st3", 3))	{ i=OPCAT_MM3; s+=3; }
	if(!strncmp(s, "st4", 3))	{ i=OPCAT_MM4; s+=3; }
	if(!strncmp(s, "st5", 3))	{ i=OPCAT_MM5; s+=3; }
	if(!strncmp(s, "st6", 3))	{ i=OPCAT_MM6; s+=3; }
	if(!strncmp(s, "st7", 3))	{ i=OPCAT_MM7; s+=3; }

	if(!strncmp(s, "ar8", 3))	{ i=OPCAT_REL8; s+=3; }
	if(!strncmp(s, "ar16", 4))	{ i=OPCAT_REL16; s+=4; }
	if(!strncmp(s, "ar32", 4))	{ i=OPCAT_REL32; s+=4; }
	if(!strncmp(s, "ar64", 4))	{ i=OPCAT_REL64; s+=4; }

	if(!strncmp(s, "sr", 2))	{ i=OPCAT_SR; s+=2; }
	if(!strncmp(s, "cr", 2))	{ i=OPCAT_CR; s+=2; }
	if(!strncmp(s, "dr", 2))	{ i=OPCAT_DR; s+=2; }

	if(!strncmp(s, "mo16", 4))	{ i=OPCAT_MOFFS16; s+=4; }
	if(!strncmp(s, "mo32", 4))	{ i=OPCAT_MOFFS32; s+=4; }
	if(!strncmp(s, "mo64", 4))	{ i=OPCAT_MOFFS64; s+=4; }

	if(!strncmp(s, "m256", 4))	{ i=OPCAT_MEM; s+=4; }
	if(!strncmp(s, "m128", 4))	{ i=OPCAT_MEM; s+=4; }
	if(!strncmp(s, "m64", 3))	{ i=OPCAT_MEM; s+=3; }
	if(!strncmp(s, "m32", 3))	{ i=OPCAT_RM32; s+=3; }
	if(!strncmp(s, "m16", 3))	{ i=OPCAT_RM16; s+=3; }
	if(!strncmp(s, "m8", 2))	{ i=OPCAT_RM8; s+=2; }
	if(!strncmp(s, "m", 1))		{ i=OPCAT_MEM; s+=1; }

	if(!strncmp(s, "frm16", 5))	{ i=OPCAT_RM16; s+=5; }
	if(!strncmp(s, "frm32", 5))	{ i=OPCAT_RM32; s+=5; }
	if(!strncmp(s, "frm64", 5))	{ i=OPCAT_RM64; s+=5; }
	if(!strncmp(s, "frm80", 5))	{ i=OPCAT_RM8; s+=5; }

	if(!strncmp(s, "frm", 3))	{ i=OPCAT_FRM; s+=3; }
	if(!strncmp(s, "fr2", 3))	{ i=OPCAT_FRM; s+=3; }
	if(!strncmp(s, "fr", 2))	{ i=OPCAT_FREG; s+=2; }

	if(!strncmp(s, "xrm", 3))	{ i=OPCAT_XRM; s+=3; }
	if(!strncmp(s, "xr2", 3))	{ i=OPCAT_XRM; s+=3; }
	if(!strncmp(s, "xrv", 3))	{ i=OPCAT_XRV; s+=3; }
	if(!strncmp(s, "xri", 3))	{ i=OPCAT_XRI; s+=3; }
	if(!strncmp(s, "xr", 2))	{ i=OPCAT_XREG; s+=2; }

	if(!strncmp(s, "yrm", 3))	{ i=OPCAT_YRM; s+=3; }
	if(!strncmp(s, "yr2", 3))	{ i=OPCAT_YRM; s+=3; }
	if(!strncmp(s, "yrv", 3))	{ i=OPCAT_YRV; s+=3; }
	if(!strncmp(s, "yri", 3))	{ i=OPCAT_YRI; s+=3; }
	if(!strncmp(s, "yr", 2))	{ i=OPCAT_YREG; s+=2; }

	if(!strncmp(s, "cs", 2))	{ i=OPCAT_CS; s+=2; }
	if(!strncmp(s, "ss", 2))	{ i=OPCAT_SS; s+=2; }
	if(!strncmp(s, "ds", 2))	{ i=OPCAT_DS; s+=2; }
	if(!strncmp(s, "es", 2))	{ i=OPCAT_ES; s+=2; }
	if(!strncmp(s, "fs", 2))	{ i=OPCAT_FS; s+=2; }
	if(!strncmp(s, "gs", 2))	{ i=OPCAT_GS; s+=2; }

	*str=s;
	return(i);
}

int main()
{
	char buf[256];
	FILE *fd;
	char *s, *t, **a;
	int i, j, k, n;

	fd=fopen(in_txt, "rt");
	if(!fd)
	{
		printf("fail open %s\n", in_txt);
		return(-1);
	}

	nops=0; nregs=0; n=0;
	while(!feof(fd))
	{
		memset(buf, 0, 256);
		fgets(buf, 255, fd);

		s=buf;
		while(*s && (*s<=' '))s++;
		if(!*s)continue;
		if(*s==';')continue;
		if(*s=='#')continue;
		if(*s=='/')continue;

		a=ksplit(s);
		if(!a[0])continue;

		//locate opcode number
		if(buf[0]>' ')
		{
			if(!strcmp(a[0], ".reg"))
			{
				i=nregs++;
				regs[i]=strdup(a[1]);
				regidx[i]=atoi(a[2]);
				continue;
			}

			for(n=0; n<nops; n++)
				if(!strcmp(a[0], ops[n]))
					break;
			if(n>=nops)
			{
				n=nops++;
				ops[n]=strdup(a[0]);
				opidx[n]=ndecls;
			}else if(n<(nops-1))
			{
				printf("discontinuity for op '%s'\n", a[0]);
			}

			a++;
			if(!a[0])continue;
		}

		i=ndecls++;
		opnum[i]=n;
		opdecl[i]=strdup(a[0]);

		opcat[i*16+ 0]=0; opcat[i*16+ 1]=0;
		opcat[i*16+ 2]=0; opcat[i*16+ 3]=0;
		opcat[i*16+ 4]=0; opcat[i*16+ 5]=0;
		opcat[i*16+ 6]=0; opcat[i*16+ 7]=0;
		opcat[i*16+ 8]=0; opcat[i*16+ 9]=0;
		opcat[i*16+10]=0; opcat[i*16+11]=0;
		opcat[i*16+12]=0; opcat[i*16+13]=0;
		opcat[i*16+14]=0; opcat[i*16+15]=0;

		if(a[1] && (a[1][0]!='-'))
		{
			j=0; k=0;

			s=a[1];
			while(*s)
			{
				if(*s==',') { j++; s++; }

				k=get_opcat(&s);
				opcat[i*16+j]=k;
				if(k>=0)continue;

				printf("parse problem op '%s'\n", ops[n]);
				break;
			}

//			opcat[i*4+3]=k;
//			opcat[i*16+7]=0;
		}

		if(a[2] && (a[2][0]!='-'))
		{
			s=a[2]; j=0;
			while(*s)
			{
				if(*s==',') { s++; continue; }
				if(!strncmp(s, "aleph", 5))
					{ j|=OPFL_ALEPH; s+=5; continue; }
				if(!strncmp(s, "long", 4))
					{ j|=OPFL_LONG; s+=4; continue; }
				if(!strncmp(s, "leg", 3))
					{ j|=OPFL_LEG; s+=3; continue; }
				if(!strncmp(s, "vex", 3))
					{ j|=OPFL_VEX; s+=3; continue; }
				if(!strncmp(s, "bet", 3))
					{ j|=OPFL_BET; s+=3; continue; }

				printf("parse problem op '%s' fl '%s' \n", ops[n], a[2]);
				break;
			}

			opcat[i*16+7]=j;
		}
	}
	fclose(fd);


	fd=fopen(out_c, "wt");
	fprintf(fd, "/* Autogenerated source */\n\n");

	fprintf(fd, "#include <stdio.h>\n\n");
//	fprintf(fd, "#ifndef NULL\n");
//	fprintf(fd, "#define NULL ((void *)0)\n");
//	fprintf(fd, "#endif\n\n");

	fprintf(fd, "char *basm_ops[]={\n");
	j=0;
	for(i=0; i<nops; i++)
	{
		strcpy(buf, ops[i]);
		s=buf;
		while(*s)
		{
			if((*s>='A') && (*s<='Z'))
				*s=*s-'A'+'a';
			s++;
		}


		fprintf(fd, "\"%s\", ", buf);
		j+=strlen(buf)+4;

		if(j>64)
		{
			fprintf(fd, "\n");
			j=0;
		}
	}
	fprintf(fd, "NULL};\n\n");

	fprintf(fd, "int basm_opidx[]={\n");
	j=0;
	for(i=0; i<nops; i++)
	{
		fprintf(fd, "%d, ", opidx[i]);
		j+=3;
		if(opidx[i]>=10)j++;
		if(opidx[i]>=100)j++;

		if(j>64)
		{
			fprintf(fd, "\n");
			j=0;
		}
	}
	fprintf(fd, "-1};\n\n");


	fprintf(fd, "int basm_opnums[]={\n");
	j=0;
	for(i=0; i<ndecls; i++)
	{
		fprintf(fd, "%d, ", opnum[i]);
		j+=3;
		if(opnum[i]>=10)j++;
		if(opnum[i]>=100)j++;

		if(j>64)
		{
			fprintf(fd, "\n");
			j=0;
		}
	}
	fprintf(fd, "-1};\n\n");


	fprintf(fd, "char basm_opcat[]={\n");
	j=0;
	for(i=0; i<(16*ndecls); i++)
	{
		fprintf(fd, "%d, ", opcat[i]);
		j+=3;
		if(opcat[i]>=10)j++;
		if(opcat[i]>=100)j++;

		if(j>64)
//		if(1)
		{
			fprintf(fd, "\n");
			j=0;
		}
	}
	fprintf(fd, "0};\n\n");


	fprintf(fd, "char *basm_opdecl[]={\n");
	j=0;
	for(i=0; i<ndecls; i++)
	{
		s=opdecl[i];
		if(!s)s="";

		fprintf(fd, "\"%s\", ", s);
		j+=strlen(s)+4;

		if(j>64)
		{
			fprintf(fd, "\n");
			j=0;
		}
	}
	fprintf(fd, "NULL};\n\n");

	fprintf(fd, "char *basm_regs[]={\n");
	j=0;
	for(i=0; i<nregs; i++)
	{
		strcpy(buf, regs[i]);
		s=buf;
		while(*s)
		{
			if((*s>='A') && (*s<='Z'))
				*s=*s-'A'+'a';
			s++;
		}


		fprintf(fd, "\"%s\", ", buf);
		j+=strlen(buf)+4;

		if(j>64)
		{
			fprintf(fd, "\n");
			j=0;
		}
	}
	fprintf(fd, "NULL};\n\n");

	fprintf(fd, "int basm_regidx[]={\n");
	j=0;
	for(i=0; i<nregs; i++)
	{
		fprintf(fd, "%d, ", regidx[i]);
		j+=3;
		if(regidx[i]>=10)j++;
		if(regidx[i]>=100)j++;

		if(j>64)
		{
			fprintf(fd, "\n");
			j=0;
		}
	}
	fprintf(fd, "-1};\n\n");

	fclose(fd);



	fd=fopen(out_h, "wt");
	fprintf(fd, "/* Autogenerated header */\n\n");

	for(i=0; i<nops; i++)
	{
		strcpy(buf, ops[i]);
		s=buf;
		while(*s)
		{
			if((*s>='a') && (*s<='z'))
				*s=*s-'a'+'A';
			s++;
		}

//		j=(31-strlen(buf))/8;
		j=(63-(strlen(buf)+strlen("#define BASM_OP_")))/8;
		fprintf(fd, "#define BASM_OP_%s", buf, i);
		while((j--)>0)fprintf(fd, "\t");
		fprintf(fd, "%d\n", i);
	}

	for(i=0; i<nregs; i++)
	{
		strcpy(buf, regs[i]);
		s=buf;
		while(*s)
		{
			if((*s>='a') && (*s<='z'))
				*s=*s-'a'+'A';
			s++;
		}

		j=(63-(strlen(buf)+strlen("#define BASM_REG_")))/8;
		fprintf(fd, "#define BASM_REG_%s", buf);
		while((j--)>0)fprintf(fd, "\t");
		fprintf(fd, "%d\n", regidx[i]);
	}

	fclose(fd);

	return(0);
}
