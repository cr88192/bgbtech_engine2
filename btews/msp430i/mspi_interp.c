#include <bteifgl.h>

void *msp430_malloc(int sz)
{
	void *ptr;
	
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

BTEIFGL_API MSP430_Context *MSP430_AllocContext(void)
{
	MSP430_Context *ctx;
	
	ctx=malloc(sizeof(MSP430_Context));
	memset(ctx, 0, sizeof(MSP430_Context));
	return(ctx);
}

BTEIFGL_API MSP430_Context *MSP430_NewContext(int ty, int szRam, int szRom)
{
	MSP430_Context *ctx;
	
	ctx=MSP430_AllocContext();
	ctx->ram=msp430_malloc(szRam);
	ctx->ram_cfl=msp430_malloc(szRam/2);
	ctx->rom=msp430_malloc(szRom);
	ctx->szRam=szRam;
	ctx->szRom=szRom;
	ctx->vaRom=65536-szRom;
	
	ctx->drom[MSP430_DROM_CALDCO_1MHZ]=1;
	ctx->drom[MSP430_DROM_CALBC1_1MHZ]=0;
	ctx->drom[MSP430_DROM_CALDCO_8MHZ]=8;
	ctx->drom[MSP430_DROM_CALBC1_8MHZ]=0;
	ctx->drom[MSP430_DROM_CALDCO_12MHZ]=12;
	ctx->drom[MSP430_DROM_CALBC1_12MHZ]=0;
	ctx->drom[MSP430_DROM_CALDCO_16MHZ]=16;
	ctx->drom[MSP430_DROM_CALBC1_16MHZ]=0;
	
	return(ctx);
}

int msp430_readHexDigit(char ch)
{
	if((ch>='0') && (ch<='9'))
		return(ch-'0');
	if((ch>='A') && (ch<='F'))
		return(10+(ch-'A'));
	if((ch>='a') && (ch<='f'))
		return(10+(ch-'a'));	
//	*(int *)-1=-1;
	return(-1);
}

int msp430_readHexNibble(char **rcs)
{
	char *cs;
	int i;
	cs=*rcs;
	i=msp430_readHexDigit(*cs++);
	*rcs=cs;
	return(i);
}

int msp430_readHexByte(char **rcs)
{
	char *cs;
	int i;
	cs=*rcs;
	i=(msp430_readHexDigit(cs[0])<<4)|
		msp430_readHexDigit(cs[1]);
	*rcs=cs+2;
	return(i);
}

int msp430_readHexWord(char **rcs)
{
	char *cs;
	int i;
	cs=*rcs;
	i=	(msp430_readHexDigit(cs[0])<<12)|
		(msp430_readHexDigit(cs[1])<< 8)|
		(msp430_readHexDigit(cs[2])<< 4)|
		(msp430_readHexDigit(cs[3])    );
	if(i<0)i=-1;
	*rcs=cs+4;
	return(i);
}

int msp430_readHexDWord(char **rcs)
{
	char *cs;
	int i, j, k;
	i=msp430_readHexWord(rcs);
	j=msp430_readHexWord(rcs);
	k=(i<<16)|j;
	return(k);
}

BTEIFGL_API double MSP430_GetCurrentMHz(MSP430_Context *ctx)
{
	int i, j;
	
//	i=ctx->perib[MSP430_PERIB_BCSCTL1];
	i=ctx->perib[MSP430_PERIB_DCOCTL];
	if(!i)i=1;
	j=(i&31);
	return(j);
}

BTEIFGL_API int MSP430_GetCurrentMips(MSP430_Context *ctx)
{
	int i, j;
	
//	i=ctx->perib[MSP430_PERIB_BCSCTL1];
	i=ctx->perib[MSP430_PERIB_DCOCTL];
	if(!i)i=1;
//	j=(1<<24)/((i&31)+1);
	j=(i&31)*1000000;
//	j=j*0.288;
	return(j);
}

BTEIFGL_API double MSP430_GetCurrentAvgMips(MSP430_Context *ctx)
{
	double fq, sc;
	int i, j;

	sc=(ctx->runOps+1.0)/(ctx->runCycles+1.0);
	fq=MSP430_GetCurrentMHz(ctx);
	
	return(sc*fq);
}

BTEIFGL_API int MSP430_GetOutputPinRunning(MSP430_Context *ctx)
{
	int i, j;
	
	j=0;
	for(i=0; i<16; i++)
	{
		if(ctx->opincnt[i]>=128)
		{
			j=j|(1<<i);
			ctx->opincnt[i]=128;
		}else
		{
			ctx->opincnt[i]=127;
		}
	}
	return(j);
}

BTEIFGL_API int MSP430_DecodeIHex(MSP430_Context *ctx, char *ibuf, int szBuf)
{
	byte buf[256];
	char *cs, *cse;
	int n, o, t, ct, cv, sb;
	int i, j, k;

	MSP430_FlushICache(ctx);

	cs=ibuf; cse=ibuf+szBuf; sb=0;
	while(cs<cse)
	{
		//IHex Record
		if(*cs==':')
		{
			cs++;
			n=msp430_readHexByte(&cs);
			o=msp430_readHexWord(&cs);
			t=msp430_readHexByte(&cs);
			ct=n+t+((o>>8)&255)+(o&255);
			for(i=0; i<n; i++)
			{
				j=msp430_readHexByte(&cs);
				buf[i]=j;
				ct+=j;
			}
			ct=(byte)(-((byte)ct));
			cv=msp430_readHexByte(&cs);
			if(ct!=cv)
			{
				printf("MSP430_DecodeIHex: "
					"Checksum Fail %02X!=%02X\n", ct, cv);
			}
			while(*cs && (*cs<=' '))cs++;
		
			if(t==0)
			{
				for(i=0; i<n; i++)
				{
					MSP430_VaWriteByte(ctx, MSP430_VAM_BURN+sb+o+i, buf[i]);
				}
				continue;
			}
		
			if(t==1)
			{
				break;
			}

			if(t==2)
			{
				i=(buf[0]<<8)|buf[1];
				sb=i*16;
				continue;
			}

			if(t==4)
			{
				i=(buf[0]<<8)|buf[1];
				sb=i<<16;
				continue;
			}

			continue;
		}

		//TekHex Record
		if(*cs=='%')
		{
			cs++;
			n=msp430_readHexByte(&cs);
			t=msp430_readHexNibble(&cs);
			cv=msp430_readHexByte(&cs);
			msp430_readHexNibble(&cs);
			o=msp430_readHexDWord(&cs);

			n=(n-14)/2;

			for(i=0; i<n; i++)
			{
				j=msp430_readHexByte(&cs);
				buf[i]=j;
				ct+=j;
			}

			while(*cs && (*cs<=' '))cs++;
			
			if(t==6)
			{
				for(i=0; i<n; i++)
				{
					MSP430_VaWriteByte(ctx, MSP430_VAM_BURN+o+i, buf[i]);
				}
				continue;
			}

			if(t==8)
			{
				if(o)
				{
					MSP430_VaWriteWord(ctx, MSP430_VAM_BURN+0xFFFE, o);
				}
				break;
			}
			continue;
		}
	}

	MSP430_Reset(ctx);

	return(0);
}

BTEIFGL_API int MSP430_LoadIHex(MSP430_Context *ctx, char *fname)
{
	byte *buf;
	int fsz;
	int i;

	buf=vf_loadfile(fname, &fsz);
	if(!buf)
		return(-1);

	i=MSP430_DecodeIHex(ctx, buf, fsz);
	frgl_free(buf);
	return(i);
}

BTEIFGL_API char *MSP430_LookupMapSymbol(
	MSP430_Context *ctx, int addr, int *rbase)
{
	int bd, bi, d;
	int i, j, k;
	
	if(addr&(~(0x000FFFFF)))
		return(NULL);
	
	bi=-1; bd=999999;
	for(i=0; i<ctx->szmap; i++)
	{
		if(ctx->mapaddr[i]==addr)
		{
			if(rbase)
				*rbase=addr;
			return(ctx->mapsym[i]);
		}
		
		d=addr-ctx->mapaddr[i];
		if(d<0)continue;
		if(d<bd)
			{ bi=i; bd=d; }
	}
	
	if(bi<0)
	{
		return(NULL);
	}
	
	if(rbase)
		*rbase=ctx->mapaddr[bi];
	return(ctx->mapsym[bi]);
}

BTEIFGL_API int MSP430_AddMapSymbol(
	MSP430_Context *ctx, char *sym, int addr)
{
	int i, j, k;
	
	for(i=0; i<ctx->szmap; i++)
	{
		if(ctx->mapaddr[i]==addr)
			return(i);
	}
	
	i=ctx->szmap++;
	ctx->mapaddr[i]=addr;
	ctx->mapsym[i]=frgl_strdup(sym);
	return(i);
}

BTEIFGL_API int MSP430_DecodeMap(MSP430_Context *ctx, char *ibuf, int szBuf)
{
	byte buf[256];
	char *cs, *cs1, *cse;
	char *ct;
//	int n, o, t, ct, cv, sb;
	int i, j, k;

//	MSP430_FlushICache(ctx);

	ctx->szmap=0;

	cs=ibuf; cse=ibuf+szBuf;
//	sb=0;
	while(cs<cse)
	{
		cs1=cs;
		i=msp430_readHexDWord(&cs1);
		if((i>=0) && (cs1[0]==' ') && (cs1[1]==' ') && (cs1[2]!=' '))
		{
			cs1+=2;
			ct=buf;
			while((cs1<cse) && (*cs1>' '))
				*ct++=*cs1++;
			*ct++=0;
			
			MSP430_AddMapSymbol(ctx, buf, i);

			cs=cs1;
			while((cs<cse) && (*cs!='\n'))
				cs++;
			if((cs<cse) && (*cs=='\n'))
				cs++;
			continue;
		}
	
		if(1)
		{
			while((cs<cse) && (*cs!='\n'))
				cs++;
			if((cs<cse) && (*cs=='\n'))
				cs++;
			continue;
		}
	}
	return(0);
}

BTEIFGL_API int MSP430_LoadMap(MSP430_Context *ctx, char *fname)
{
	byte *buf;
	int fsz;
	int i;

	buf=vf_loadfile(fname, &fsz);
	if(!buf)
		return(-1);

	i=MSP430_DecodeMap(ctx, buf, fsz);
	frgl_free(buf);
	return(i);
}

BTEIFGL_API int MSP430_FlushICache(MSP430_Context *ctx)
{
	MSP430_Opcode *cur, *nxt;
	int i;
	
	for(i=0; i<256; i++)
		ctx->ophash[i]=NULL;
	ctx->pc_op=NULL;

	cur=ctx->ops;
	ctx->ops=NULL;
	while(cur)
	{
		nxt=cur->next;
		cur->next=ctx->freeops;
		ctx->freeops=cur;
		cur=nxt;
	}
	
	return(0);
}

BTEIFGL_API int MSP430_Reset(MSP430_Context *ctx)
{
	int i, j, k;
	
	for(i=0; i<16; i++)
	{
		ctx->reg[i]=0;
//		ctx->regh[i]=0;
	}
	
	i=MSP430_VaReadWord(ctx, 0xFFFE);
	ctx->reg[MSP430_REG_PC]=i;
	ctx->reg[MSP430_REG_SP]=0x1100+ctx->szRam-4;
	
	ctx->wdtCycles=32768;
	ctx->periw[MSP430_PERIW_WDTCTL]=0x6900;
	
//	j=MSP430_SRM_GIE;
//	ctx->reg[MSP430_REG_SR]=j;
	
	return(0);
}

BTEIFGL_API int MSP430_Run(MSP430_Context *ctx)
{
	MSP430_Opcode *cur;

	cur=MSP430_GetDecodeOpcode(ctx, ctx->reg[MSP430_REG_PC]);
	while(cur)
	{
		cur=cur->Run(ctx, cur);
	}
	return(0);
}

BTEIFGL_API int MSP430_RunCount(MSP430_Context *ctx, int cnt)
{
	MSP430_Opcode *cur;
	int n, n1r, n1, wdtctl, no;
	int i, j, k;

	if(ctx->errStatus)
		return(ctx->errStatus);

	if(ctx->reg[MSP430_REG_CG]!=0)
	{
		printf("MSP430_RunCount: CG!=0\n");
		ctx->reg[MSP430_REG_CG]=0;
	}
	ctx->errStatus=0;

	cur=ctx->pc_op;
	if(!cur)
		{ cur=MSP430_GetDecodeOpcode(ctx, ctx->reg[MSP430_REG_PC]); }
	n=cnt; no=0;
	if(n>ctx->wdtCycles)
	{
		wdtctl=ctx->periw[MSP430_PERIW_WDTCTL];

		while(cur && (n>0) && !ctx->errStatus)
		{
			n1=n;
			if(n1>ctx->wdtCycles)
				n1=ctx->wdtCycles;
			n1r=n1;

			while(cur && (n1>0) && !ctx->errStatus)
			{
#if 1
				i=ctx->oprov;
				ctx->ophist[i]=cur;
				ctx->oprov=(i+1)&63;
#endif
			
	//			MSP430_DumpOpcode(ctx, cur);
				n1-=cur->clks; no++;
				cur=cur->Run(ctx, cur);
//				n1--;
			}

			n-=n1r-n1;
			ctx->wdtCycles-=n1r-n1;

			if(cur)
			{
				ctx->pc_op=cur;
				ctx->reg[MSP430_REG_PC]=cur->pc;
			}

			if(ctx->wdtCycles<=0)
			{
				if(wdtctl&0x10)
				{
					ctx->wdtCycles=ctx->wdtRstCnt;
					MSP430_Interupt(ctx, 26);
				}else
				{
					MSP430_Interupt(ctx, 31);
				}
				cur=MSP430_GetDecodeOpcode(ctx,
					ctx->reg[MSP430_REG_PC]);
			}
		}
	}else
	{
		while(cur && (n>0) && !ctx->errStatus)
		{
#if 1
			i=ctx->oprov;
			ctx->ophist[i]=cur;
			ctx->oprov=(i+1)&63;
#endif
		
//			MSP430_DumpOpcode(ctx, cur);
			n-=cur->clks; no++;
			cur=cur->Run(ctx, cur);
//			n--;
		}
	}

	if(cur)
	{
		ctx->pc_op=cur;
		ctx->reg[MSP430_REG_PC]=cur->pc;
	}else
	{
		printf("MSP430_RunCount: Op=NULL\n",
			ctx->errStatus, ctx->errParm1, ctx->errParm2);
		ctx->errStatus=MSP430_ERR_GENERIC;
	}
	
	ctx->runCycles+=cnt-n;
	ctx->wdtCycles-=(cnt-n);
	ctx->runOps+=no;
	
	if(ctx->errStatus==MSP430_ERR_TRAP_SMC)
	{
		MSP430_FlushICache(ctx);
		ctx->errStatus=0;
	}
	
	if(ctx->errStatus)
	{
#if 1
		for(i=0; i<60; i++)
		{
			j=((ctx->oprov-60)+i)&63;
			MSP430_DumpOpcode(ctx, ctx->ophist[j]);
		}
#endif
	
		printf("MSP430_RunCount: errStatus=%d %08X %08X\n",
			ctx->errStatus, ctx->errParm1, ctx->errParm2);
		for(i=0; i<2; i++)
		{
			printf("    ");
			for(j=0; j<8; j++)
			{
				k=i*8+j;
				printf("%3s=%04X ",
					MSP430_DumpOpcode_NameForReg(ctx, k),
					ctx->reg[k]);
			}
			printf("\n");
		}
	}
	
	return(ctx->errStatus);
}

BTEIFGL_API int MSP430_Push(MSP430_Context *ctx, int val)
{
	int i, j, a;
	a=ctx->reg[MSP430_REG_SP]-2;
	ctx->reg[MSP430_REG_SP]=a;
	MSP430_VaWriteWordSP(ctx, a, val);
	return(0);
}

BTEIFGL_API int MSP430_Interupt(MSP430_Context *ctx, int inum)
{
	int i;
	MSP430_Push(ctx, ctx->reg[MSP430_REG_PC]);
	MSP430_Push(ctx, ctx->reg[MSP430_REG_SR]);

	i=MSP430_VaReadWord(ctx, 0xFFC0+(inum<<1));
	ctx->pc_op=NULL;
	ctx->reg[MSP430_REG_PC]=i;
	return(0);
}

BTEIFGL_API int MSP430_UpdatePeri(MSP430_Context *ctx)
{
	int ctrl0, wdtctl;
	double vrm, vrn, vrr, vref;
	double f, g;
	int i, j;
	
	ctrl0=ctx->periw[MSP430_PERIW_ADC10CTL0];
	if(ctrl0&MSP430_ADC10_SC)
	{
		if(ctrl0&MSP430_ADC10_REFON)
		{
			if(ctrl0&MSP430_ADC10_REF2_5V)
				{ vref=2.5; }
			else
				{ vref=1.5; }
		}else
		{
			vref=0.1;
		}

		switch((ctrl0>>13)&7)
		{
		case 0:
			vrm=0;
			vrn=3.3;
			vrr=vrn-vrm;
			break;
		case 1:
			vrm=0;
			vrn=vref;
			vrr=vrn-vrm;
			break;
		default:
			vrm=0;
			vrn=vref;
			vrr=vrn-vrm;
			break;
		}

		for(i=0; i<8; i++)
		{
			if(ctx->perib[MSP430_PERIB_ADC10AE0]&(1<<i))
			{
				f=(ctx->pinv[i]-vrm)/vrr;
				j=1023*f;
				if(j<0)j=0;
				if(j>1023)j=1023;
				ctx->periw[MSP430_PERIW_ADC10MEM]=j;
			}
		}
		
		if((ctrl0&MSP430_ADC10_IE) &&
			(ctx->reg[MSP430_REG_SR]&MSP430_SRM_GIE))
		{
			MSP430_Interupt(ctx, 21);
		}
		
		ctx->periw[MSP430_PERIW_ADC10CTL0]&=~MSP430_ADC10_SC;
	}

	wdtctl=ctx->periw[MSP430_PERIW_WDTCTL];
	if(!(wdtctl&0x80))
	{
		if(ctx->wdtCycles<0)
		{
			if(wdtctl&0x10)
			{
				MSP430_Interupt(ctx, 26);
			}else
			{
				MSP430_Interupt(ctx, 31);
			}

			switch(wdtctl&3)
			{
			case 0:		ctx->wdtRstCnt=32768;	break;
			case 1:		ctx->wdtRstCnt=8192;	break;
			case 2:		ctx->wdtRstCnt=512;		break;
			case 3:		ctx->wdtRstCnt=64;		break;
			}
			ctx->wdtCycles+=ctx->wdtRstCnt;
		}
	}else
	{
		ctx->wdtCycles=(1<<31)-1;
		ctx->wdtRstCnt=(1<<31)-1;
	}

	return(0);
}

BTEIFGL_API int MSP430_PinInputVoltage(MSP430_Context *ctx,
	int inum, float val)
{
	float f, g;
	int in, im, iv, liv;
	int i, j, k;

	if((inum<0) || (inum>=16))
		return(-1);

	f=ctx->pinv[inum];
	ctx->lpinv[inum]=f;
	ctx->pinv[inum]=val;
	
	liv=(f>1.5);
	iv=(val>1.5);

	if(iv!=liv)
	{
		in=inum&7;
		im=1<<in;
		i=(inum>=8)?MSP430_PERIB_P2IN:MSP430_PERIB_P1IN;
		j=ctx->perib[i];
		k=iv?(j|im):(j&(~im));
		ctx->perib[i]=k;
	}
	
	return(0);
}
