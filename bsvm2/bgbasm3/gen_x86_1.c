/*
use istruction table to generate assembler data.
each instruction will be given an ID code, which will identify its opcode name.
this will be part of the public interface.

each opcode will itself be sorted into categories.
the first byte will give the type of the first arg;
the second will give the type of the second;
the third will give the bit size.

types:
0=none;
1=reg;
2=mem;
3=imm;
4=rm;
5=eax;
6=rel addr;
7=float reg;
8=moffs16;
9=moffs32;
10=moffs64.

it will be assumed that more preferable encodings precede less preferable ones
in the case where multiple cases are possible.

an opcode will be given multiple entries if it accepts a wider variety of
types.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPCAT_REG	1
#define OPCAT_MEM	2
#define OPCAT_IMM	3
#define OPCAT_RM	4
#define OPCAT_EAX	5
#define OPCAT_REL	6

#define OPCAT_MOFFS16	8
#define OPCAT_MOFFS32	9
#define OPCAT_MOFFS64	10
#define OPCAT_SR	11
#define OPCAT_CR	12
#define OPCAT_DR	13

#define OPCAT_FREG	14	//float/MMX reg
#define OPCAT_FRM	15	//float/MMX reg/memory
#define OPCAT_XREG	16	//SSE reg
#define OPCAT_XRM	17	//SSE reg/memory

char *in_txt="basm_insns.txt";
char *out_c="basm_insns.c";
char *out_h="bgbasm_x86ops.h";

char *ops[4096];	//op names
int opidx[4096];	//op index
int nops;

int   opnum [4096];
char  opcat [4096*4];
char *opdecl[4096];
int ndecls;

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

	nops=0; n=0;
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
			for(n=0; n<nops; n++)
				if(!strcmp(a[0], ops[n]))
					break;
			if(n>=nops)
			{
				n=nops++;
				ops[n]=strdup(a[0]);
				opidx[n]=ndecls;
			}

			a++;
			if(!a[0])continue;
		}

		i=ndecls++;
		opnum[i]=n;
		opdecl[i]=strdup(a[0]);
		opcat[i*4+0]=0; opcat[i*4+1]=0;
		opcat[i*4+2]=0; opcat[i*4+3]=0;

		if(a[1])
		{
			j=0; k=0;

			s=a[1];
			while(*s)
			{
				if(*s==',') { j++; s++; }
				if(!strncmp(s, "i8", 2))
					{ opcat[i*4+j]=OPCAT_IMM; s+=2; 
						if(j && (k!=8))k|=1;
							else k=8;
						continue; }
				if(!strncmp(s, "i16", 3))
					{ opcat[i*4+j]=OPCAT_IMM; s+=3;
						if(j && (k!=16))k|=2;
							else k=16;
						continue; }
				if(!strncmp(s, "i32", 3))
					{ opcat[i*4+j]=OPCAT_IMM; s+=3;
//					if(!j)k=32;
					if(j && (k!=32))k|=4;
						else k=32;
					continue; }
				if(!strncmp(s, "i64", 3))
					{ opcat[i*4+j]=OPCAT_IMM; s+=3; k=64;
						continue; }

				if(!strncmp(s, "r8", 2))
					{ opcat[i*4+j]=OPCAT_REG; s+=2; k=8;
						continue; }
				if(!strncmp(s, "r16", 3))
					{ opcat[i*4+j]=OPCAT_REG; s+=3; k=16;
						continue; }
				if(!strncmp(s, "r32", 3))
					{ opcat[i*4+j]=OPCAT_REG; s+=3; k=32;
						continue; }
				if(!strncmp(s, "r64", 3))
					{ opcat[i*4+j]=OPCAT_REG; s+=3; k=64;
						continue; }

				if(!strncmp(s, "rm8", 3))
					{ opcat[i*4+j]=OPCAT_RM; s+=3; k=8;
						continue; }
				if(!strncmp(s, "rm16", 4))
					{ opcat[i*4+j]=OPCAT_RM; s+=4; k=16;
						continue; }
				if(!strncmp(s, "rm32", 4))
					{ opcat[i*4+j]=OPCAT_RM; s+=4; k=32;
						continue; }
				if(!strncmp(s, "rm64", 4))
					{ opcat[i*4+j]=OPCAT_RM; s+=4; k=64;
						continue; }

				if(!strncmp(s, "al", 2))
					{ opcat[i*4+j]=OPCAT_EAX; s+=2; k=8;
						continue; }
				if(!strncmp(s, "ax", 2))
					{ opcat[i*4+j]=OPCAT_EAX; s+=2; k=16;
						continue; }
				if(!strncmp(s, "eax", 3))
					{ opcat[i*4+j]=OPCAT_EAX; s+=3; k=32;
						continue; }
				if(!strncmp(s, "rax", 3))
					{ opcat[i*4+j]=OPCAT_EAX; s+=3; k=64;
						continue; }

				if(!strncmp(s, "ar8", 3))
					{ opcat[i*4+j]=OPCAT_REL; s+=3; k=8;
						continue; }
				if(!strncmp(s, "ar16", 4))
					{ opcat[i*4+j]=OPCAT_REL; s+=4; k=16;
						continue; }
				if(!strncmp(s, "ar32", 4))
					{ opcat[i*4+j]=OPCAT_REL; s+=4; k=32;
						continue; }
				if(!strncmp(s, "ar64", 4))
					{ opcat[i*4+j]=OPCAT_REL; s+=4; k=64;
						continue; }

				if(!strncmp(s, "sr", 2))
					{ opcat[i*4+j]=OPCAT_SR; s+=2; k=16;
						continue; }
				if(!strncmp(s, "cr", 2))
					{ opcat[i*4+j]=OPCAT_CR; s+=2;
						continue; }
				if(!strncmp(s, "dr", 2))
					{ opcat[i*4+j]=OPCAT_DR; s+=2;
						continue; }

				if(!strncmp(s, "mo16", 4))
					{ opcat[i*4+j]=OPCAT_MOFFS16; s+=4;
						continue; }
				if(!strncmp(s, "mo32", 4))
					{ opcat[i*4+j]=OPCAT_MOFFS32; s+=4;
						continue; }
				if(!strncmp(s, "mo64", 4))
					{ opcat[i*4+j]=OPCAT_MOFFS64; s+=4;
						continue; }

				if(!strncmp(s, "m", 1))
					{ opcat[i*4+j]=OPCAT_MEM; s+=1; k=0;
						continue; }

				if(!strncmp(s, "frm", 3))
					{ opcat[i*4+j]=OPCAT_FRM; s+=3;
						continue; }
				if(!strncmp(s, "fr2", 3))
					{ opcat[i*4+j]=OPCAT_FRM; s+=3;
						continue; }
				if(!strncmp(s, "fr", 2))
					{ opcat[i*4+j]=OPCAT_FREG; s+=2;
						continue; }
				if(!strncmp(s, "xrm", 3))
					{ opcat[i*4+j]=OPCAT_XRM; s+=3;
						continue; }
				if(!strncmp(s, "xr2", 3))
					{ opcat[i*4+j]=OPCAT_XRM; s+=3;
						continue; }
				if(!strncmp(s, "xr", 2))
					{ opcat[i*4+j]=OPCAT_XREG; s+=2;
						continue; }

				printf("parse problem op %s\n", ops[n]);
				break;
			}

			opcat[i*4+2]=k;
		}
	}
	fclose(fd);


	fd=fopen(out_c, "wt");
	fprintf(fd, "/* Autogenerated source */\n\n");

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
	fprintf(fd, "\"\"};\n\n");

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
	for(i=0; i<(4*ndecls); i++)
	{
		fprintf(fd, "%d, ", opcat[i]);
		j+=3;
		if(opcat[i]>=10)j++;
		if(opcat[i]>=100)j++;

		if(j>64)
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
	fprintf(fd, "\"\"};\n\n");

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

		j=(31-strlen(buf))/8;
		fprintf(fd, "#define BASM_OP_%s", buf, i);
		while(j--)fprintf(fd, "\t");
		fprintf(fd, "%d\n", i);
	}
	fclose(fd);

	return(0);
}
