#include <bteifgl.h>

int MSP430_VaReadByte(MSP430_Context *ctx, int addr)
{
	int i;

	if(addr>=0xC0000000)
	{
		if((byte)(addr>>24)==0xC0)
		{
			return(addr&255);
		}

		if((byte)(addr>>24)==0xC1)
		{
			return((byte)ctx->reg[addr&15]);
		}

		ctx->errParm1=addr;
		ctx->errStatus=MSP430_ERR_INV_READ;
		return(-1);
	}

	if((addr>=ctx->vaRom) && (addr<=0xFFFF))
	{
		i=addr-ctx->vaRom;
		if(i>=ctx->szRom)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->rom[i]);
	}

	if((addr>=0x0200) && (addr<0x0A00))
	{
		i=addr-0x0200;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->ram[i]);
	}

	if((addr>=0x1100) && (addr<0x3900))
	{
		i=addr-0x1100;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->ram[i]);
	}

	if((addr>=0x1000) && (addr<0x1100))
	{
		i=addr-0x1000;
		return(ctx->drom[i]);
	}

	if((addr>=0x0000) && (addr<0x0100))
	{
		i=addr-0x0000;
		if(i>=256)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->perib[i]);
	}

	if((addr>=0x0100) && (addr<0x0200))
	{
		i=(addr-0x0100)>>1;
		if(i>=128)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->periw[i]);
	}
	
	ctx->errParm1=addr;
	ctx->errStatus=MSP430_ERR_INV_READ;
	return(-1);
}

int MSP430_VaWriteByte(MSP430_Context *ctx, int addr, int val)
{
	int addr2;
	int i, j, k;

	if(addr>=0xC0000000)
	{
		if((byte)(addr>>24)==0xC1)
		{
			addr2=addr&15;
			
			if((addr2==MSP430_REG_PC) || (addr2==MSP430_REG_CG))
			{
				ctx->errParm1=addr;
				ctx->errStatus=MSP430_ERR_INV_WRITE;
				return(-1);
			}

			ctx->reg[addr2]=(byte)val;
			return(0);
		}

		if((byte)(addr>>24)==0xC0)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRITE;
			return(-1);
		}

		if((byte)(addr>>24)==0xC2)
		{
			addr2=addr&0xFFFFFF;
			if((addr2>=ctx->vaRom) && (addr2<=0xFFFF))
			{
				i=addr2-ctx->vaRom;
				if(i>=ctx->szRom)
				{
					ctx->errParm1=addr;
					ctx->errStatus=MSP430_ERR_INV_WRITE;
					return(-1);
				}
				ctx->rom[i]=val;
				return(0);
			}
		}

		ctx->errParm1=addr;
		ctx->errStatus=MSP430_ERR_INV_WRITE;
		return(-1);
	}

	if((addr>=0x0200) && (addr<0x0A00))
	{
		i=addr-0x0200;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRITE;
			return(-1);
		}
		if(ctx->ram_cfl[i>>1]&1)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_TRAP_SMC;
		}
		ctx->ram[i]=val;
		return(0);
	}

	if((addr>=0x1100) && (addr<0x3900))
	{
		i=addr-0x1100;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRITE;
			return(-1);
		}
		if(ctx->ram_cfl[i>>1]&1)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_TRAP_SMC;
		}
		ctx->ram[i]=val;
		return(0);
	}

	if((addr>=0x0000) && (addr<0x0100))
	{
		i=addr-0x0000;
//		if(i>=256)
//			{ return(-1); }
		ctx->perib[i]=val;
		switch(i)
		{
		case MSP430_PERIB_P1OUT:
			for(j=0; j<8; j++)
			{
				if(val&(1<<j))
				{
					if(ctx->opincnt[j]<255)
						ctx->opincnt[j]++;
				}else
				{
					if(ctx->opincnt[j]>0)
						ctx->opincnt[j]--;
				}
			}
			break;
		case MSP430_PERIB_P2OUT:
			for(j=0; j<8; j++)
			{
				if(val&(1<<j))
				{
					if(ctx->opincnt[8+j]<255)
						ctx->opincnt[8+j]++;
				}else
				{
					if(ctx->opincnt[8+j]>0)
						ctx->opincnt[8+j]--;
				}
			}
			break;
		default:
			break;
		}
		return(0);
	}

	if((addr>=0x0100) && (addr<0x0200))
	{
		i=(addr-0x0100)>>1;
//		if(i>=128)
//			{ return(-1); }
		ctx->periw[i]=(byte)val;
		return(0);
	}

	if((addr>=ctx->vaRom) && (addr<=0xFFFF))
	{
		i=addr-ctx->vaRom;
		if(i>=ctx->szRom)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRROM;
			return(-1);
		}
//		return(ctx->rom[i]);
//		ctx->rom[i]=val;
		ctx->errParm1=addr;
		ctx->errStatus=MSP430_ERR_INV_WRROM;
		return(-1);
	}

	if((addr>=0x1000) && (addr<0x1100))
	{
		i=addr-0x1000;
		ctx->drom[i]=val;
		return(0);
	}

	ctx->errParm1=addr;
	ctx->errStatus=MSP430_ERR_INV_WRITE;
	return(-1);
}

int MSP430_VaReadWord(MSP430_Context *ctx, int addr)
{
	int i;

	if(addr>=0xC0000000)
	{
		if((byte)(addr>>24)==0xC0)
		{
			return(addr&65535);
		}

		if((byte)(addr>>24)==0xC1)
		{
			return(ctx->reg[addr&15]);
		}

		ctx->errParm1=addr;
		ctx->errStatus=MSP430_ERR_INV_READ;
		return(-1);
	}

	if((addr>=0x0200) && (addr<0x0A00))
	{
		i=addr-0x0200;
		if((i+1)>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->ram[i+0]|(ctx->ram[i+1]<<8));
	}

	if((addr>=ctx->vaRom) && (addr<=0xFFFF))
	{
		i=addr-ctx->vaRom;
		if(i>=ctx->szRom)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->rom[i+0]|(ctx->rom[i+1]<<8));
	}

	if((addr>=0x1100) && (addr<0x3900))
	{
		i=addr-0x1100;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->ram[i+0]|(ctx->ram[i+1]<<8));
	}

	if((addr>=0x1000) && (addr<0x1100))
	{
		i=addr-0x1000;
		return(ctx->drom[i+0]|(ctx->drom[i+1]<<8));
	}

	if((addr>=0x0000) && (addr<0x0100))
	{
		i=addr-0x0000;
		return(ctx->perib[i+0]|(ctx->perib[i+1]<<8));
	}

	if((addr>=0x0100) && (addr<0x0200))
	{
		i=(addr-0x0100)>>1;
		return(ctx->periw[i]);
	}
	
	ctx->errParm1=addr;
	ctx->errStatus=MSP430_ERR_INV_READ;
	return(-1);
}

int MSP430_VaWriteWord(MSP430_Context *ctx, int addr, int val)
{
	int addr2;
	int i;

	if(addr>=0xC0000000)
	{
		if((byte)(addr>>24)==0xC0)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRITE;
			return(-1);
		}

		if((byte)(addr>>24)==0xC1)
		{
			addr2=addr&15;

//			if((addr2==MSP430_REG_PC) || (addr2==MSP430_REG_CG))
			if(addr2==MSP430_REG_CG)
			{
				ctx->errParm1=addr;
				ctx->errStatus=MSP430_ERR_INV_WRITE;
				return(-1);
			}

			ctx->reg[addr2]=val;
			return(0);
		}

		if((byte)(addr>>24)==0xC2)
		{
			addr2=addr&0xFFFFFF;
			if((addr2>=ctx->vaRom) && (addr2<=0xFFFF))
			{
				i=addr2-ctx->vaRom;
//				if(i>=ctx->szRom)
//				{
//					return(-1);
//				}
				ctx->rom[i+0]=val;
				ctx->rom[i+1]=val>>8;
				return(0);
			}
		}

		ctx->errParm1=addr;
		ctx->errStatus=MSP430_ERR_INV_WRITE;
		return(-1);
	}

	if((addr>=0x0200) && (addr<0x0A00))
	{
		i=addr-0x0200;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRITE;
			return(-1);
		}
		if(ctx->ram_cfl[i>>1]&1)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_TRAP_SMC;
		}
		ctx->ram[i+0]=val;
		ctx->ram[i+1]=val>>8;
		return(0);
	}

	if((addr>=0x1100) && (addr<0x3900))
	{
		i=addr-0x1100;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRITE;
			return(-1);
		}
		if(ctx->ram_cfl[i>>1]&1)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_TRAP_SMC;
		}
		ctx->ram[i+0]=val;
		ctx->ram[i+1]=val>>8;
		return(0);
	}

	if((addr>=0x0000) && (addr<0x0100))
	{
		i=addr-0x0000;
//		if(i>=256)
//			{ return(-1); }
		ctx->perib[i+0]=val;
//		ctx->perib[i+1]=val>>8;
		return(0);
	}

	if((addr>=0x0100) && (addr<0x0200))
	{
		i=(addr-0x0100)>>1;
//		if(i>=128)
//			{ return(-1); }
		ctx->periw[i]=val;
		
		switch(i)
		{
		case MSP430_PERIW_WDTCTL:
			ctx->periw[i]=0x6900|(val&255);

			if((val>>8)!=0x5A)
			{
				ctx->errParm1=addr;
				ctx->errStatus=MSP430_ERR_INV_WRITE;
				break;
			}

			switch(val&3)
			{
			case 0:		ctx->wdtRstCnt=32768;	break;
			case 1:		ctx->wdtRstCnt=8192;	break;
			case 2:		ctx->wdtRstCnt=512;		break;
			case 3:		ctx->wdtRstCnt=64;		break;
			}
			if(val&0x80)
				{ ctx->wdtRstCnt=(1<<31)-1; }
			ctx->wdtCycles=ctx->wdtRstCnt;

			break;
		default:
			break;
		}
		
		return(0);
	}

	if((addr>=ctx->vaRom) && (addr<=0xFFFF))
	{
		i=addr-ctx->vaRom;
		if(i>=ctx->szRom)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRITE;
			return(-1);
		}
//		ctx->rom[i+0]=val;
//		ctx->rom[i+1]=val>>8;
//		return(ctx->rom[i]);
		ctx->errParm1=addr;
		ctx->errStatus=MSP430_ERR_INV_WRROM;
		return(-1);
	}

	if((addr>=0x1000) && (addr<0x1100))
	{
		i=addr-0x1000;
		ctx->drom[i+0]=val;
		ctx->drom[i+1]=val>>8;
		return(0);
	}

	ctx->errParm1=addr;
	ctx->errStatus=MSP430_ERR_INV_WRITE;
	return(-1);
}

int MSP430_VaReadWordSP(MSP430_Context *ctx, int addr)
{
	int i;

	if((addr>=0x0200) && (addr<0x0A00))
	{
		i=addr-0x0200;
		if((i+1)>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		return(ctx->ram[i+0]|(ctx->ram[i+1]<<8));
	}
	
	return(MSP430_VaReadWord(ctx, addr));
}

int MSP430_VaWriteWordSP(MSP430_Context *ctx, int addr, int val)
{
	int i;

	if((addr>=0x0200) && (addr<0x0A00))
	{
		i=addr-0x0200;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_WRITE;
			return(-1);
		}
		ctx->ram[i+0]=val;
		ctx->ram[i+1]=val>>8;
		return(0);
	}

	return(MSP430_VaWriteWord(ctx, addr, val));
}

int MSP430_VaReadWordPC(MSP430_Context *ctx, int addr)
{
	int i;

	if((addr>=0x0200) && (addr<0x0A00))
	{
		i=addr-0x0200;
		if((i+1)>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		ctx->ram_cfl[i>>1]|=1;
		return(ctx->ram[i+0]|(ctx->ram[i+1]<<8));
	}

	if((addr>=0x1100) && (addr<0x3900))
	{
		i=addr-0x1100;
		if(i>=ctx->szRam)
		{
			ctx->errParm1=addr;
			ctx->errStatus=MSP430_ERR_INV_READ;
			return(-1);
		}
		ctx->ram_cfl[i>>1]|=1;
		return(ctx->ram[i+0]|(ctx->ram[i+1]<<8));
	}
	
	return(MSP430_VaReadWord(ctx, addr));
}

int MSP430_GetRegB(MSP430_Context *ctx, int reg)
{
	return(ctx->reg[reg]&255);
}

int MSP430_VaAddrIsRomP(MSP430_Context *ctx, int addr)
{
	if((addr>=ctx->vaRom) && (addr<=0xFFFF))
		return(true);
	return(false);
}
