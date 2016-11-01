// #include <bgbasm.h>

#if 1
extern char **basm_ops;
extern int *basm_opidx;
extern int *basm_opnums;

extern char *basm_opcat;
extern char **basm_opdecl;

extern char **basm_regs;
extern int *basm_regidx;
#endif

static char *bdisasm_bregs[]={"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
static char *bdisasm_bregs2[]=
	{"al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil",
	"r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b"};
static char *bdisasm_wregs[]=
	{"ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
	"r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"};
static char *bdisasm_dregs[]=
	{"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi",
	"r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"};
static char *bdisasm_qregs[]=
	{"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
	"r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

static char *bdisasm_fregs[]=
	{"st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7",
	"st8", "st9", "st10", "st11", "st12", "st13", "st14", "st15"};
static char *bdisasm_xregs[]=
	{"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
	"xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"};


BASM_API char *BASM_GetPtrName(void *ptr);


static void *int2ptr(int i)
{
#ifdef X86_64
	return((void *)((long long)i));
#else
	return((void *)i);
#endif
}

#ifndef BASM_HEXVAL_DEF
#define BASM_HEXVAL_DEF
static int hexval(int i)
{
	if((i>='0') && (i<='9'))return(i-'0');
	if((i>='A') && (i<='F'))return(i-'A'+10);
	if((i>='a') && (i<='f'))return(i-'a'+10);
	return(-1);
}

static int hexbyte(char *s)
{
	int i, j;
	i=hexval(s[0]);
	j=hexval(s[1]);
	if((i<0) || (j<0))return(-1);
	return((i<<4)+j);
}
#endif

int BDISASM_CheckOpStr(byte **rip, char **str, int *rfl, int op)
{
	byte *ip, *ip1, *oip;
	char *s, *s1;
	int i, fl, w;

	s=*str; ip=*rip; fl=(*rfl)&0xF;
	oip=ip;

	if((*s=='V') && !(fl&2)) { if(*ip!=0x67)return(0); s++; ip++; }
	if((*s=='W') && !(fl&2)) { if(*ip!=0x66)return(0); s++; ip++; }
	if((*s=='S') && !(fl&2))s++;
	if((*s=='T') && !(fl&2))s++;

	if((*s=='V') && (fl&2))s++;
	if((*s=='W') && (fl&2))s++;
	if((*s=='S') && (fl&2)) { if(*ip!=0x67)return(0); s++; ip++; }
	if((*s=='T') && (fl&2)) { if(*ip!=0x66)return(0); s++; ip++; }

	if((*s=='X') && !(fl&1))s++;
	if((*s=='X') && (fl&1))
	{
		if((*ip&0xF0)==0x40)
			fl|=((*ip++)&0xF)<<4;

		w=0;
		if(basm_opcat[op*16+0]==OPCAT_REG8 )w=8;
		if(basm_opcat[op*16+0]==OPCAT_REG16)w=16;
		if(basm_opcat[op*16+0]==OPCAT_REG32)w=32;
		if(basm_opcat[op*16+0]==OPCAT_REG64)w=64;

		if(basm_opcat[op*16+0]==OPCAT_RM8 )w=8;
		if(basm_opcat[op*16+0]==OPCAT_RM16)w=16;
		if(basm_opcat[op*16+0]==OPCAT_RM32)w=32;
		if(basm_opcat[op*16+0]==OPCAT_RM64)w=64;

		if((w==32) && (fl&0x80)) return(0);
		if((w==64) && !(fl&0x80)) return(0);

		s++;
	}

	while(*s)
	{
		i=hexbyte(s);

		if((i>=0) && (s[2]=='|'))
		{
			if((*ip&0xF8)!=i)
				return(0);
			ip++; s+=2;
			break;
		}
		if(i<0)break;
		if(*ip!=i)return(0);
		ip++; s+=2;
	}

	if((*s=='/') && (s[1]>='0') && (s[1]<='7'))
	{
		i=s[1]-'0';
		if(i!=(((*ip)>>3)&7))
			return(0);
	}

	ip1=ip; s1=s;

	if(*s=='|')s+=2;

	if(*s=='/')
	{
		ip=BDISASM_SkipModRM(ip, fl);
		s+=2;
	}

	if(!strncmp(s, ",ib", 3)) { ip++; s+=3; }
	if(!strncmp(s, ",iw", 3)) { ip+=2; s+=3; }
	if(!strncmp(s, ",id", 3)) { ip+=4; s+=3; }
	if(!strncmp(s, ",iq", 3)) { ip+=8; s+=3; }
	if(!strncmp(s, ",rb", 3)) { ip++; s+=3; }
	if(!strncmp(s, ",rw", 3)) { ip+=2; s+=3; }
	if(!strncmp(s, ",rd", 3)) { ip+=4; s+=3; }
	if(!strncmp(s, ",rq", 3)) { ip+=8; s+=3; }
	if(!strncmp(s, ",mw", 3)) { ip+=2; s+=3; }
	if(!strncmp(s, ",md", 3)) { ip+=4; s+=3; }
	if(!strncmp(s, ",mq", 3)) { ip+=8; s+=3; }

	while(*s)
	{
		i=hexbyte(s);
		if(i<0)break;
		if(*ip!=i)return(0);
		ip++; s+=2;
	}

	*str=s1; *rip=ip1; *rfl=fl;
	return(ip-oip);
}

byte *BDISASM_PrintModRM_RM16(byte *ip, int op, int fl, int w, int ar)
{
	static char *rpw[8]={"bx+si", "bx+di", "bp+si", "bp+di",
		"si", "di", "bp", "bx"};

	char **rl, **rp;
	int j, k;

	if((w==8) && !(fl&1))rl=bdisasm_bregs;
	if((w==8) && (fl&1))rl=bdisasm_bregs2;
	if(w==16)rl=bdisasm_wregs;
	if(w==32)rl=bdisasm_dregs;
	if(w==64)rl=bdisasm_qregs;

	if(w==17)rl=bdisasm_wregs;
	if(w==33)rl=bdisasm_dregs;
	if(w==65)rl=bdisasm_qregs;

	if(basm_opcat[op*16+ar]==OPCAT_FREG)rl=bdisasm_fregs;
	if(basm_opcat[op*16+ar]==OPCAT_FRM)rl=bdisasm_fregs;
	if(basm_opcat[op*16+ar]==OPCAT_XREG)rl=bdisasm_xregs;
	if(basm_opcat[op*16+ar]==OPCAT_XRM)rl=bdisasm_xregs;

	rp=rpw;
//	rp=bdisasm_dregs;
//	if(fl&0x80)rp=bdisasm_qregs;

	switch(((*ip)>>6)&3)
	{
	case 0:
		switch((*ip)&7)
		{
		case 6:
			j=ip[1]|(ip[2]<<8);
			printf("[0x%X]", j);
			ip+=3;
			break;
		default:
			j=((*ip)&7)|((fl&0x10)?8:0);
			printf("[%s]", rp[j]);
			ip++;
			break;
		}
		break;
	case 1:
		j=((*ip)&7)|((fl&0x10)?8:0);
		k=(char)ip[1];
		printf("[%s+%d]", rp[j], k);
		ip+=2;
		break;
	case 2:
		j=((*ip)&7)|((fl&0x10)?8:0);
		k=ip[1]|(ip[2]<<8);
		k=(short)k;
		printf("[%s+%d]", rp[j], k);
		ip+=3;
		break;
	case 3:
		j=((*ip)&7)|((fl&0x10)?8:0);
		printf("%s", rl[j]);
		ip++;
		break;
	}

	return(ip);
}

byte *BDISASM_PrintModRM_RM(byte *ip, int op, int fl, int w, int ar)
{
	byte *ip1;
	char **rl, **rp;
	char *s;
	int j, k;
	int sc, si, sb;

//	if(fl&8)
	if(((fl&2) && !(fl&8)) || (!(fl&2) && (fl&8)))
	{
		ip=BDISASM_PrintModRM_RM16(ip, op, fl, w, ar);
		return(ip);
	}

	if(!(fl&1))rl=bdisasm_dregs;
	if(fl&1)rl=bdisasm_qregs;

	if((w==8) && !(fl&1))rl=bdisasm_bregs;
	if((w==8) && (fl&1))rl=bdisasm_bregs2;
	if(w==16)rl=bdisasm_wregs;
	if(w==32)rl=bdisasm_dregs;
	if(w==64)rl=bdisasm_qregs;

	if(w==17)rl=bdisasm_wregs;
	if(w==33)rl=bdisasm_dregs;
	if(w==65)rl=bdisasm_qregs;

	if(basm_opcat[op*16+ar]==OPCAT_FREG)rl=bdisasm_fregs;
	if(basm_opcat[op*16+ar]==OPCAT_FRM)rl=bdisasm_fregs;
	if(basm_opcat[op*16+ar]==OPCAT_XREG)rl=bdisasm_xregs;
	if(basm_opcat[op*16+ar]==OPCAT_XRM)rl=bdisasm_xregs;

	rp=bdisasm_dregs;
//	if(fl&0x80)rp=bdisasm_qregs;
	if(fl&1)rp=bdisasm_qregs;

	switch(((*ip)>>6)&3)
	{
	case 0:
		switch((*ip)&7)
		{
		case 4:
			sc=1<<((ip[1]>>6)&3);
			si=((ip[1]>>3)&7)|((fl&0x20)?8:0);
			sb=(ip[1]&7)|((fl&0x10)?8:0);
			if(((si&7)==4) && ((sb&7)==5))
			{
				j=ip[2]|(ip[3]<<8)|(ip[4]<<16)|(ip[5]<<24);

				printf("[0x%X]", j);
				s=BASM_GetPtrName(int2ptr(j));
				if(s)printf("<%s>", s);

				ip+=6;
				break;
			}
			if(((si&7)!=4) && ((sb&7)==5))
			{
				j=ip[2]|(ip[3]<<8)|(ip[4]<<16)|(ip[5]<<24);

				printf("[%s*%d+0x%X]", rp[si], sc, j);
				s=BASM_GetPtrName(int2ptr(j));
				if(s)printf("<%s>", s);

				ip+=6;
				break;
			}
			if(((si&7)==4) && ((sb&7)!=5))
				printf("[%s]", rp[sb]);
			if(((si&7)!=4) && ((sb&7)!=5))
				printf("[%s+%s*%d]", rp[sb], rp[si], sc);
			ip+=2;
			break;
		case 5:
			j=ip[1]|(ip[2]<<8)|(ip[3]<<16)|(ip[4]<<24);
			j=(int)j;
//			if(fl&0x80)
			if(fl&1)
			{
//				printf("[rip%+d]", j);

				ip1=ip+5+j;

#ifdef X86_64
				printf("[%016llX]", (long long)ip1);
#else
				printf("[%08X]", (int)ip1);
#endif

				s=BASM_GetPtrName(ip1);
				if(s)printf("<%s>", s);
			}else
			{
				printf("[0x%X]", j);
				s=BASM_GetPtrName(int2ptr(j));
				if(s)printf("<%s>", s);
			}
			ip+=5;
			break;
		default:
			j=((*ip)&7)|((fl&0x10)?8:0);
			printf("[%s]", rp[j]);
			ip++;
			break;
		}
		break;
	case 1:
		switch((*ip)&7)
		{
		case 4:
			j=(char)ip[2];
			sc=1<<((ip[1]>>6)&3);
			si=((ip[1]>>3)&7)|((fl&0x20)?8:0);
			sb=(ip[1]&7)|((fl&0x10)?8:0);
			if(((si&7)==4) && ((sb&7)==5))printf("[ebp+0x%X]", j);
			if(((si&7)!=4) && ((sb&7)==5))
				printf("[ebp+%s*%d+0x%X]", rp[si], sc, j);
			if(((si&7)==4) && ((sb&7)!=5))
				printf("[%s%+d]", rp[sb], j);
			if(((si&7)!=4) && ((sb&7)!=5))
				printf("[%s+%s*%d+%d]",
					rp[sb], rp[si], sc, j);
			ip+=3;
			break;
		default:
			j=((*ip)&7)|((fl&0x10)?8:0);
			k=(char)ip[1];
			printf("[%s%+d]", rp[j], k);
			ip+=2;
			break;
		}
		break;
	case 2:
		switch((*ip)&7)
		{
		case 4:
			j=ip[2]|(ip[3]<<8)|(ip[4]<<16)|(ip[5]<<24);
			j=(int)j;

			sc=1<<((ip[1]>>6)&3);
			si=((ip[1]>>3)&7)|((fl&0x20)?8:0);
			sb=(ip[1]&7)|((fl&0x10)?8:0);
			if(((si&7)==4) && ((sb&7)==5))
				printf("[ebp+0x%X]", j);
			if(((si&7)!=4) && ((sb&7)==5))
				printf("[ebp+%s*%d+0x%X]", rp[si], sc, j);
			if(((si&7)==4) && ((sb&7)!=5))
				printf("[%s+%d]", rp[sb], j);
			if(((si&7)!=4) && ((sb&7)!=5))
				printf("[%s+%s*%d+%d]",
					rp[sb], rp[si], sc, j);
			ip+=6;
			break;
		default:
			j=((*ip)&7)|((fl&0x10)?8:0);
			k=ip[1]|(ip[2]<<8)|(ip[3]<<16)|(ip[4]<<24);
			k=(int)k;

			printf("[%s+%d]", rp[j], k);
			ip+=5;
			break;
		}
		break;
	case 3:
		j=((*ip)&7)|((fl&0x10)?8:0);
		printf("%s", rl[j]);
		ip++;
		break;
	}

	return(ip);
}

byte *BDISASM_PrintModRM_RegRM(byte *ip, int op, int fl, int w)
{
	char **rl;
	int i;

	if((w==32) && (fl&0x80))w=64;

	if((w==8) && !(fl&1))rl=bdisasm_bregs;
	if((w==8) && (fl&1))rl=bdisasm_bregs2;
	if(w==16)rl=bdisasm_wregs;
	if(w==32)rl=bdisasm_dregs;
	if(w==64)rl=bdisasm_qregs;

	if(w==17)rl=bdisasm_wregs;
	if(w==33)rl=bdisasm_dregs;
	if(w==65)rl=bdisasm_qregs;

	if(basm_opcat[op*16+0]==OPCAT_FREG)rl=bdisasm_fregs;
	if(basm_opcat[op*16+0]==OPCAT_FRM)rl=bdisasm_fregs;
	if(basm_opcat[op*16+0]==OPCAT_XREG)rl=bdisasm_xregs;
	if(basm_opcat[op*16+0]==OPCAT_XRM)rl=bdisasm_xregs;

	i=(((*ip)>>3)&7)|((fl&0x40)?8:0);
	printf("%s, ", rl[i]);

	ip=BDISASM_PrintModRM_RM(ip, op, fl, w, 1);
	return(ip);
}

byte *BDISASM_PrintModRM_RMReg(byte *ip, int op, int fl, int w)
{
	char **rl;
	int i;

	if((w==32) && (fl&0x80))w=64;

	if((w==8) && !(fl&1))rl=bdisasm_bregs;
	if((w==8) && (fl&1))rl=bdisasm_bregs2;
	if(w==16)rl=bdisasm_wregs;
	if(w==32)rl=bdisasm_dregs;
	if(w==64)rl=bdisasm_qregs;

	if(basm_opcat[op*16+1]==OPCAT_FREG)rl=bdisasm_fregs;
	if(basm_opcat[op*16+1]==OPCAT_FRM)rl=bdisasm_fregs;
	if(basm_opcat[op*16+1]==OPCAT_XREG)rl=bdisasm_xregs;
	if(basm_opcat[op*16+1]==OPCAT_XRM)rl=bdisasm_xregs;

	i=(((*ip)>>3)&7)|((fl&0x40)?8:0);

	ip=BDISASM_PrintModRM_RM(ip, op, fl, w, 0);
	printf(", %s", rl[i]);

	return(ip);
}

byte *BDISASM_SkipModRM(byte *ip, int fl)
{
	int si, sb;

	if(((fl&2) && !(fl&8)) || (!(fl&2) && (fl&8)))
	{
		switch(((*ip)>>6)&3)
		{
		case 0:
			switch((*ip)&7)
			{
			case 6: ip+=3; break;
			default: ip++; break;
			}
			break;
		case 1:
			ip+=2;
			break;
		case 2:
			ip+=3;
			break;
		case 3: ip++; break;
		}

		return(ip);
	}

	switch(((*ip)>>6)&3)
	{
	case 0:
		switch((*ip)&7)
		{
		case 4:
			si=((ip[1]>>3)&7)|((fl&0x20)?8:0);
			sb=(ip[1]&7)|((fl&0x10)?8:0);
			if(((si&7)==4) && ((sb&7)==5))
			{
				ip+=6;
				break;
			}
			ip+=2;
			break;
		case 5: ip+=5; break;
		default: ip++; break;
		}
		break;
	case 1:
		switch((*ip)&7)
		{
		case 4: ip+=3; break;
		default: ip+=2; break;
		}
		break;
	case 2:
		switch((*ip)&7)
		{
		case 4: ip+=6; break;
		default: ip+=5; break;
		}
		break;
	case 3: ip++; break;
	}

	return(ip);
}

int BDISASM_SizeOpStr(byte *oip, char *str, int fl)
{
	byte *ip;
	char *s;
	int i, w;

	s=str; ip=oip; fl&=0xF;
	if((*s=='V') && !(fl&2)) { s++; ip++; }
	if((*s=='W') && !(fl&2)) { s++; ip++; }
	if((*s=='S') && !(fl&2))s++;
	if((*s=='T') && !(fl&2))s++;

	if((*s=='V') && (fl&2))s++;
	if((*s=='W') && (fl&2))s++;
	if((*s=='S') && (fl&2)) { s++; ip++; }
	if((*s=='T') && (fl&2)) { s++; ip++; }

	if((*s=='X') && !(fl&1))s++;
	if((*s=='X') && (fl&1))
	{
		if((*ip&0xF0)==0x40)
			{ fl|=((*ip)&0xF)<<4; ip++; }
		s++;
	}

	while(*s)
	{
		i=hexbyte(s);

		if((i>=0) && (s[2]=='|'))
		{
			ip++; s+=4;
			break;
		}
		if(i<0)break;
		ip++; s+=2;
	}

	if(*s=='/')
	{
		ip=BDISASM_SkipModRM(ip, fl);
		s+=2;
	}

	if(!strncmp(s, ",ib", 3)) { ip++; s+=3; }
	if(!strncmp(s, ",iw", 3)) { ip+=2; s+=3; }
	if(!strncmp(s, ",id", 3)) { ip+=4; s+=3; }
	if(!strncmp(s, ",iq", 3)) { ip+=8; s+=3; }
	if(!strncmp(s, ",rb", 3)) { ip++; s+=3; }
	if(!strncmp(s, ",rw", 3)) { ip+=2; s+=3; }
	if(!strncmp(s, ",rd", 3)) { ip+=4; s+=3; }
	if(!strncmp(s, ",rq", 3)) { ip+=8; s+=3; }
	if(!strncmp(s, ",mw", 3)) { ip+=2; s+=3; }
	if(!strncmp(s, ",md", 3)) { ip+=4; s+=3; }
	if(!strncmp(s, ",mq", 3)) { ip+=8; s+=3; }

	while(*s)
	{
		i=hexbyte(s);
		if(i<0)break;
		ip++; s+=2;
	}

	return(ip-oip);
}

char *BDISASM_GetFixReg(int i, int j)
{
	if(basm_opcat[i*16+j]==OPCAT_AL)return("al");
	if(basm_opcat[i*16+j]==OPCAT_CL)return("cl");
	if(basm_opcat[i*16+j]==OPCAT_DL)return("dl");
	if(basm_opcat[i*16+j]==OPCAT_BL)return("bl");
	if(basm_opcat[i*16+j]==OPCAT_AH)return("ah");
	if(basm_opcat[i*16+j]==OPCAT_CH)return("ch");
	if(basm_opcat[i*16+j]==OPCAT_DH)return("dh");
	if(basm_opcat[i*16+j]==OPCAT_BH)return("bh");

	if(basm_opcat[i*16+j]==OPCAT_AX)return("ax");
	if(basm_opcat[i*16+j]==OPCAT_CX)return("cx");
	if(basm_opcat[i*16+j]==OPCAT_DX)return("dx");
	if(basm_opcat[i*16+j]==OPCAT_BX)return("bx");
	if(basm_opcat[i*16+j]==OPCAT_SP)return("sp");
	if(basm_opcat[i*16+j]==OPCAT_BP)return("bp");
	if(basm_opcat[i*16+j]==OPCAT_SI)return("si");
	if(basm_opcat[i*16+j]==OPCAT_DI)return("di");

	if(basm_opcat[i*16+j]==OPCAT_EAX)return("eax");
	if(basm_opcat[i*16+j]==OPCAT_ECX)return("ecx");
	if(basm_opcat[i*16+j]==OPCAT_EDX)return("edx");
	if(basm_opcat[i*16+j]==OPCAT_EBX)return("ebx");
	if(basm_opcat[i*16+j]==OPCAT_ESP)return("esp");
	if(basm_opcat[i*16+j]==OPCAT_EBP)return("ebp");
	if(basm_opcat[i*16+j]==OPCAT_ESI)return("esi");
	if(basm_opcat[i*16+j]==OPCAT_EDI)return("edi");

	if(basm_opcat[i*16+j]==OPCAT_RAX)return("rax");
	if(basm_opcat[i*16+j]==OPCAT_RCX)return("rcx");
	if(basm_opcat[i*16+j]==OPCAT_RDX)return("rdx");
	if(basm_opcat[i*16+j]==OPCAT_RBX)return("rbx");
	if(basm_opcat[i*16+j]==OPCAT_RSP)return("rsp");
	if(basm_opcat[i*16+j]==OPCAT_RBP)return("rbp");
	if(basm_opcat[i*16+j]==OPCAT_RSI)return("rsi");
	if(basm_opcat[i*16+j]==OPCAT_RDI)return("rdi");

	return(NULL);
}

void BDISASM_PrintFixRegPF(int i)
{
	char *s;

	s=BDISASM_GetFixReg(i, 0);
	if(s)printf("%s, ", s);
}

void BDISASM_PrintFixRegSF(int i)
{
	char *s;

	s=BDISASM_GetFixReg(i, 1);
	if(s)printf(", %s", s);
}


BASM_API byte *BDISASM_PrintOpcode(byte *ip, int *rfl)
{
	byte *oip, *eip;
	char **rl;
	char *s, *s1;
	s64 li;
	int i, j, k, w, fl;


#ifdef X86_64
	s1=BASM_GetPtrName(ip);
	if(s1)printf("----------------  %s:\n", s1);
	printf("%016llX  ", (long long)ip);
#else
	s1=BASM_GetPtrName(ip);
	if(s1)printf("--------  %s:\n", s1);
	printf("%08X  ", (int)ip);
#endif

	fl=*rfl;
//	if(fl&8)printf("(seta16) ");

	for(i=0; basm_opnums[i]>=0; i++)
	{
		if((basm_opcat[i*16+7]&OPFL_LONG) && !(fl&1))continue;
		if((basm_opcat[i*16+7]&OPFL_LEG) && (fl&1))continue;

		s=basm_opdecl[i];
		oip=ip;
		j=BDISASM_CheckOpStr(&ip, &s, &fl, i);
		if(!j)continue;

//		printf("%s ", basm_opdecl[i]);
#if 0
		w=0;
		if(basm_opcat[i*16+0]==OPCAT_REG8 )w=8;
		if(basm_opcat[i*16+0]==OPCAT_REG16)w=16;
		if(basm_opcat[i*16+0]==OPCAT_REG32)w=32;
		if(basm_opcat[i*16+0]==OPCAT_REG64)w=64;

		if(basm_opcat[i*16+0]==OPCAT_RM8 )w=8;
		if(basm_opcat[i*16+0]==OPCAT_RM16)w=16;
		if(basm_opcat[i*16+0]==OPCAT_RM32)w=32;
		if(basm_opcat[i*16+0]==OPCAT_RM64)w=64;

//		if((w==32) && (fl&0x80)) { ip=oip; continue; }
//		if((w==64) && !(fl&0x80)) { ip=oip; continue; }
#endif

		eip=oip+j;

//		j=BDISASM_SizeOpStr(oip, basm_opdecl[i], fl);
		for(k=0; k<j; k++)printf("%02X", oip[k]);
		for(; k<12; k++)printf("  ");

//		printf("%s(%s) %s", basm_ops[basm_opnums[i]],
//			basm_opdecl[i], s);

//		if(fl&8)printf("(seta16) ");

		printf("%s ", basm_ops[basm_opnums[i]]);

		if((oip[0]==0x67) && (j==1))
		{
			fl|=8;
			printf("\n");
			*rfl=fl;
			return(ip);
		}

		w=0;

		if(basm_opcat[i*16+0]==OPCAT_REG8 )w=8;
		if(basm_opcat[i*16+0]==OPCAT_REG16)w=16;
		if(basm_opcat[i*16+0]==OPCAT_REG32)w=32;
		if(basm_opcat[i*16+0]==OPCAT_REG64)w=64;

		if(basm_opcat[i*16+0]==OPCAT_RM8 )w=8;
		if(basm_opcat[i*16+0]==OPCAT_RM16)w=16;
		if(basm_opcat[i*16+0]==OPCAT_RM32)w=32;
		if(basm_opcat[i*16+0]==OPCAT_RM64)w=64;

		if((w==32) && (fl&0x80))w=64;

		if((w==8) && !(fl&1))rl=bdisasm_bregs;
		if((w==8) && (fl&1))rl=bdisasm_bregs2;
		if(w==16)rl=bdisasm_wregs;
		if(w==32)rl=bdisasm_dregs;
		if(w==64)rl=bdisasm_qregs;

		k=0;

		if(*s=='|')
		{
			if(basm_opcat[i*16+0]==OPCAT_FREG)rl=bdisasm_fregs;
			if(basm_opcat[i*16+0]==OPCAT_FRM)rl=bdisasm_fregs;
			if(basm_opcat[i*16+0]==OPCAT_XREG)rl=bdisasm_xregs;
			if(basm_opcat[i*16+0]==OPCAT_XRM)rl=bdisasm_xregs;
			k=((*(ip-1))&7)|((fl&0x10)?8:0);
			printf("%s", rl[k]);
			s+=2; k++;
		}

		if((*s=='/') && (s[1]=='r'))
		{
			if(	(basm_opcat[i*16+0]==OPCAT_RM8) ||
				(basm_opcat[i*16+0]==OPCAT_RM16) ||
				(basm_opcat[i*16+0]==OPCAT_RM32) ||
				(basm_opcat[i*16+0]==OPCAT_RM64) ||
				(basm_opcat[i*16+0]==OPCAT_FRM) ||
				(basm_opcat[i*16+0]==OPCAT_XRM))
				ip=BDISASM_PrintModRM_RMReg(ip, i, fl, w);
				else ip=BDISASM_PrintModRM_RegRM(ip, i, fl, w);
			s+=2; k++;
		}
		if(*s=='/')
		{
			ip=BDISASM_PrintModRM_RM(ip, i, fl, w, 0);
			s+=2; k++;
		}

		if(!*s)
		{
			fl&=~8;
			*rfl=fl;
			printf("\n");
			return(ip);
		}

		if(k)printf(", ");

		if(!strncmp(s, ",ib", 3))
		{
			BDISASM_PrintFixRegPF(i);
			printf("0x%X", (*ip));
			BDISASM_PrintFixRegSF(i);
			ip++;
		}
		if(!strncmp(s, ",iw", 3))
		{
			j=(unsigned short)(ip[0]+(ip[1]<<8));
			BDISASM_PrintFixRegPF(i);
			printf("0x%X", j);
			BDISASM_PrintFixRegSF(i);
			ip+=2;
		}
		if(!strncmp(s, ",id", 3))
		{
			j=(ip[0]+(ip[1]<<8)+(ip[2]<<16)+(ip[3]<<24));
			BDISASM_PrintFixRegPF(i);
			printf("0x%X", j);
			s1=BASM_GetPtrName(int2ptr(j));
			if(s1)printf("<%s>", s1);

			BDISASM_PrintFixRegSF(i);
			ip+=4;
		}
		if(!strncmp(s, ",iq", 3))
		{
			li=(ip[0]+(ip[1]<<8)+(ip[2]<<16)+(ip[3]<<24));
			li+=(s64)ip[4]<<32;
			li+=(s64)ip[5]<<40;
			li+=(s64)ip[6]<<48;
			li+=(s64)ip[7]<<56;
			BDISASM_PrintFixRegPF(i);
			printf("0x%llX", li);

#ifdef X86_64
			s1=BASM_GetPtrName((void *)(li));
			if(s1)printf("<%s>", s1);
#endif

			BDISASM_PrintFixRegSF(i);
			ip+=8;
		}

		if(!strncmp(s, ",rb", 3))
		{
			j=(char)(*ip++);
#ifdef X86_64
			printf("0x%llX", (long long)(ip+j));
#else
			printf("0x%X", (int)(ip+j));
#endif

			s1=BASM_GetPtrName(ip+j);
			if(s1)printf("<%s>", s1);
		}
		if(!strncmp(s, ",rw", 3))
		{
			j=(short)(ip[0]+(ip[1]<<8)); ip+=2;

#ifdef X86_64
			printf("0x%llX", (long long)(ip+j));
#else
			printf("0x%X", (int)(ip+j));
#endif

			s1=BASM_GetPtrName(ip+j);
			if(s1)printf("<%s>", s1);
		}
		if(!strncmp(s, ",rd", 3))
		{
			j=(int)(ip[0]+(ip[1]<<8)+(ip[2]<<16)+(ip[3]<<24));
			ip+=4;

#ifdef X86_64
			printf("0x%llX", (long long)(ip+j));
#else
			printf("0x%X", (int)(ip+j));
#endif

			s1=BASM_GetPtrName(ip+j);
			if(s1)printf("<%s>", s1);
		}

		if(!strncmp(s, ",mw", 3))
		{
			j=(unsigned short)(ip[0]+(ip[1]<<8));
			BDISASM_PrintFixRegPF(i);
			printf("[0x%X]", j);
			s1=BASM_GetPtrName(int2ptr(j));
			if(s1)printf("<%s>", s1);
			BDISASM_PrintFixRegSF(i);
			ip+=2;
		}
		if(!strncmp(s, ",md", 3))
		{
			j=(ip[0]+(ip[1]<<8)+(ip[2]<<16)+(ip[3]<<24));

			BDISASM_PrintFixRegPF(i);
			printf("[0x%X]", j);
			s1=BASM_GetPtrName(int2ptr(j));
			if(s1)printf("<%s>", s1);
			BDISASM_PrintFixRegSF(i);
			ip+=4;
		}

		while(*s)
		{
			j=hexbyte(s);
			if(j<0)break;
			ip++; s+=2;
		}

		fl&=~8;
		*rfl=fl;
		printf("\n");
		return(ip);
	}

	printf("db 0x%02X\n", *ip++);
	return(ip);
}

BASM_API int BDISASM_PrintOpcodes(byte *ip, int fl, int sz)
{
	byte *ipe, *ip1;
	int i;

	ipe=ip+sz;

	while(ip<ipe)
	{
		ip1=BDISASM_PrintOpcode(ip, &fl);
		if((ip1<=ip) || (ip1>(ip+16)))
		{
			printf("DisAsm: Decode problem %p\n", ip1);

			i=16;
			while(i--)printf("%02X ", *ip++);
			printf("\n");

			break;
		}
		ip=ip1;
	}
	return(0);
}

BASM_API int BDISASM_HexDump(byte *ip, int sz)
{
	int i, j, k;

	i=sz;
	while(i>0)
	{
#ifdef X86_64
		printf("%016llX  ", (long long)ip);
#else
		printf("%08X  ", (int)ip);
#endif

		for(j=0; j<16; j++)
		{
			if(j && !(j%4))printf(" ");
			if(j && !(j%8))printf(" ");
			if(j<i)printf("%02X ", ip[j]);
				else printf("   ");
		}

		for(j=0; j<16; j++)
		{
			k=ip[j];
			if(k<' ')k='~';
			if(k>'~')k='~';
			printf("%c", k);
		}

		printf("\n");

		ip+=16;
		i-=16;
	}

	return(0);
}

