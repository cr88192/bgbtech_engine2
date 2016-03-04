#include <bteifgl.h>

static byte bt1g_quant4tab[256];
static byte bt1g_quant5tab[256];
static byte bt1g_quant6tab[256];

static byte bt1g_dequant4tab[16];
static byte bt1g_dequant5tab[32];
static byte bt1g_dequant6tab[64];

double btic1g_ssqrt(double f)
{
	return((f>=0)?sqrt(f):(-sqrt(-f)));
}

double btic1g_ssqr(double f)
{
	return((f>=0)?(f*f):(-f*f));
}

double btic1g_ssqrtp15(double f)
{
	return((f>=0)?pow(f, 0.667):(-pow(-f, 0.667)));
}

double btic1g_ssqrp15(double f)
{
	return((f>=0)?(pow(f, 1.5)):(-pow(-f, 1.5)));
}

double btic1g_slog(double f)
{
	return((f>=0)?log(f+1):(-log(-f+1)));
}

double btic1g_sexp(double f)
{
	return((f>=0)?exp(f)-1:(-exp(-f)+1));
}

void BTIC1G_InitQuant()
{
	static int init=0;
	int i, j, k;

	if(init)
		return;
	init=1;
	
	for(i=0; i<256; i++)
	{
		j=btic1g_ssqrt(i-128)*0.7071+0.5;
//		j=btic1g_ssqrt(i-128)*0.7072+0.5;
//		j=btic1g_ssqrt(i-128)*0.7071+0.25;
		j=btic1g_ssqrt(i-128)*0.7071;
//		j=btic1g_slog(i-128)*(8/log(128))+0.5;
		bt1g_quant4tab[i]=clamp15(j+8);

		j=btic1g_ssqrt(i-128)*1.4142+0.5;
//		j=btic1g_ssqrt(i-128)*1.4142+0.25;
		j=btic1g_ssqrt(i-128)*1.4142;
//		j=btic1g_slog(i-128)*(16/log(128))+0.5;
		bt1g_quant5tab[i]=clamp31(j+16);

//		j=btic1g_ssqrt(i-128)*2.8284;
//		j=btic1g_ssqrtp15(i-128)*0.8409;
//		j=btic1g_ssqrtp15(i-128)*1.1892;
		j=btic1g_ssqrtp15(i-128)*1.2579;
		bt1g_quant6tab[i]=clamp63(j+32);
	}
	
	
	for(i=0; i<16; i++)
	{
		j=btic1g_ssqr((i-8)/0.7071)+128+0.5;
//		j=btic1g_sexp((i-8)/(8/log(128)))+128;
		bt1g_dequant4tab[i]=clamp255(j);
	}

	for(i=0; i<32; i++)
	{
		j=btic1g_ssqr((i-16)/1.4142)+128+0.5;
//		j=btic1g_sexp((i-16)/(16/log(128)))+128;
		bt1g_dequant5tab[i]=clamp255(j);
	}

	for(i=0; i<64; i++)
	{
//		j=btic1g_ssqr((i-32)/2.8284)+128+0.5;
//		j=btic1g_ssqrp15((i-32)/0.8409)+128+0.5;
//		j=btic1g_ssqrp15((i-32)/1.1893)+128+0.5;
		j=btic1g_ssqrp15((i-32)/1.2579)+128+0.5;
		bt1g_dequant6tab[i]=clamp255(j);
	}
	
	for(i=0; i<16; i++)
	{
		printf("%3d ", bt1g_dequant4tab[i]);
	}
	printf("\n");

	for(i=0; i<16; i++)
		{ printf("%3d ", bt1g_dequant5tab[i]); }
	printf("\n");
	for(i=0; i<16; i++)
		{ printf("%3d ", bt1g_dequant5tab[16+i]); }
	printf("\n");
}

int btic1g_quant4(int c)
	{ return(bt1g_quant4tab[c]); }
int btic1g_quant5(int c)
	{ return(bt1g_quant5tab[c]); }
int btic1g_quant6(int c)
	{ return(bt1g_quant6tab[c]); }

int btic1g_dequant4(int c)
	{ return(bt1g_dequant4tab[c]); }
int btic1g_dequant5(int c)
	{ return(bt1g_dequant5tab[c]); }
int btic1g_dequant6(int c)
	{ return(bt1g_dequant6tab[c]); }

byte *BTIC1G_EncodeBlocksBuffer(byte *obuf,
	byte *blks, byte *lblks, int nblks, int stride,
	int obmsz, int *rnblks)
{
	byte *cs2, *csl2;
	byte *cs, *cse, *csl, *ct, *cte, *ct0, *cs1;
	int cy, cu, cv, cd, cm;
	int qcu4, qcv4, qcu5, qcv5, qcd6;
	int cy1, cu1, cv1, cd1, cm1;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	BTIC1G_InitQuant();
	
	cs=blks; cse=cs+nblks*stride; csl=lblks;
	ct=obuf; cte=obuf+obmsz;
	
	ct0=ct;
	while((cs<cse) && (ct<cte))
	{
		ct0=ct;
		if(lblks)
		{
			for(i=0; i<16; i++)
			{
				cs2=cs+i*stride;
				csl2=csl+i*stride;
				if(cs2>=cse)break;
				if(memcmp(cs2, csl2, 8))
					break;
			}
			
			if(i>0)
			{
				*ct++=0x80+(i-1);
				cs+=i*stride;
				csl+=i*stride;
				continue;
			}
		}


		cy=cs[0];	cu=cs[1];		cv=cs[2];

//		qcu4=cu;	qcv4=cv;
//		qcu5=cu;	qcv5=cv;
//		qcd6=cd;

//		qcu4=btic1g_quant4(cu)<<4;
//		qcv4=btic1g_quant4(cv)<<4;
//		qcu5=btic1g_quant5(cu)<<3;
//		qcv5=btic1g_quant5(cv)<<3;

		qcu4=btic1g_quant4(cu);
		qcv4=btic1g_quant4(cv);
		qcu5=btic1g_quant5(cu);
		qcv5=btic1g_quant5(cv);

//		qcu4=clamp255(cu+7);
//		qcv4=clamp255(cv+7);
//		qcu5=clamp255(cu+3);
//		qcv5=clamp255(cv+3);
//		qcd6=clamp255(cd+1);

		if(cs[3]==0)
		{
			cm=cs[4];
			if(cm==0)
			{
				for(i=1; i<16; i++)
				{
					cs1=cs+i*stride;
					if(cs1>cse)break;
				
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2];
					cm1=cs[4];
					if((cs1[3]!=0) || (cm1!=cm))
						break;
					j=(cy^cy1);
					k=(cu^cu1)|(cv^cv1);
					if((j&0xFE) || (k&0xF0))
						break;
				}

//				j=((cy&0xFE)<<7)|(qcu4&0xF0)|((qcv4&0xF0)>>4);
				j=((cy&0xFE)<<7)|(qcu4<<4)|qcv4;
				*ct++=0x90+(i-1);
				*ct++=(j>>8); *ct++=j;
			
				cs+=i*stride; csl+=i*stride;
				continue;
			}

			if((cs[3]==0) && ((cs[4]==1) || (cs[4]==2)))
			{
				cm=cs[4]; cd=cs[5];
				qcd6=cd>>2;

				for(i=1; i<16; i++)
				{
					cs1=cs+i*stride;
					if(cs1>cse)break;
				
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2];
					cm1=cs1[4];
					if((cs1[3]!=0) || (cm1!=cm))
						break;
					j=(cy^cy1);
					k=(cu^cu1)|(cv^cv1);
					if((j&0xFE) || (k&0xF8))
						break;
				}

				if(i>1)
//				if(1)
				{
//					j=((cy&0xFE)<<15)|
//						((qcd6&0xFC)<<8)|
//						((qcu5&0xF8)<<2)|
//						((qcv5&0xF8)>>3);
					j=((cy&0xFE)<<15)|
						(qcd6<<10)|
						(qcu5<<5)|
						(qcv5   );
					*ct++=0xA0+(cm<<4)+(i-1);
					*ct++=(j>>16);
					*ct++=(j>>8);
					*ct++=j;
				}else
				{
//					j=	(cm<<21)|
//						((cy&0xFE)<<13)|
//						((qcd6&0xFC)<<6)|
//						((qcu4&0xF0)   )|
//						((qcv4&0xF0)>>4);
					j=	(cm<<21)|
						((cy&0xFE)<<13)|
						(qcd6<<8)|
						(qcu4<<4)|
						(qcv4   );
					*ct++=(j>>16);
					*ct++=(j>>8);
					*ct++=j;
				}

				if(cm==1)
				{
					for(j=0; j<i; j++)
					{
						i0=cs[(j*stride)+6];
						*ct++=i0;
					}
				}else if(cm==2)
				{
					for(j=0; j<i; j++)
					{
						cs1=cs+(j*stride);
						i0=cs1[6];	i1=cs1[7];
						*ct++=i0; *ct++=i1;
					}
				}
			
				cs+=i*stride; csl+=i*stride;
				continue;
			}

			break;
		}
		
//		if(cs[3]!=0)
		if(1)
		{
			cd=cs[3];
			qcd6=cd>>2;

			for(i=1; i<16; i++)
			{
				cs1=cs+i*stride;
				if(cs1>cse)break;
				
				cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2]; cd1=cs1[3];
				if(!cd1)break;
				j=(cy^cy1);
				k=(cu^cu1)|(cv^cv1);
				l=(cd^cd1);
				if((j&0xFE) || (k&0xF8) || (l&0xFC))
					break;
			}

			if(i>1)
			{
//				j=((cy&0xFE)<<15)|
//					((qcd6&0xFC)<<8)|
//					((qcu5&0xF8)<<2)|
//					((qcv5&0xF8)>>3);
				j=((cy&0xFE)<<15)|
					(qcd6<<10)|
					(qcu5<<5)|
					(qcv5   );
				*ct++=0xD0+(i-1);
				*ct++=(j>>16);
				*ct++=(j>>8);
				*ct++=j;
			}else
			{
//				j=	0x600000|
//					((cy&0xFE)<<13)|
//					((qcd6&0xFC)<<6)|
//					((qcu4&0xF0)   )|
//					((qcv4&0xF0)>>4);
				j=	0x600000|
					((cy&0xFE)<<13)|
					(qcd6<<8)|
					(qcu4<<4)|
					(qcv4   );
				*ct++=(j>>16);
				*ct++=(j>>8);
				*ct++=j;
			}

			for(j=0; j<i; j++)
			{
				cs1=cs+(j*stride);
				i0=cs1[4];	i1=cs1[5];
				i2=cs1[6];	i3=cs1[7];
				*ct++=i0;	*ct++=i1;
				*ct++=i2;	*ct++=i3;
			}
			
			cs+=i*stride; csl+=i*stride;
			continue;
		}
	}

	if(ct>cte)ct=ct0;
	
	return(ct);
}
