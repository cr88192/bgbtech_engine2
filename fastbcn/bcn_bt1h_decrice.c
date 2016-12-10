/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

static const byte btic1h_rice_ntab[16]={
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 2, 2, 3, 4};

static const byte btic1h_rice_ntab2[256]={
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,	/* 00-0F */
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,	/* 10-1F */
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,	/* 20-2F */
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,	/* 30-3F */
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,	/* 40-4F */
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,	/* 50-5F */
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,	/* 60-6F */
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,	/* 70-7F */
	1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,	/* 80-8F */
	1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,	/* 90-9F */
	1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,	/* A0-AF */
	1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,	/* B0-BF */
	2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2,	/* C0-CF */
	2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2,	/* D0-DF */
	3, 3, 3, 3, 3, 3, 3, 3,  3, 3, 3, 3, 3, 3, 3, 3,	/* E0-EF */
	4, 4, 4, 4, 4, 4, 4, 4,  5, 5, 5, 5, 6, 6, 7, 8};	/* F0-FF */

// static int btic1h_rice_valtab[16][256];
static u16 btic1h_rice_valtab[16][256];

int BTIC1H_Rice_InitTables()
{
	static int init=0;
	int i, j, k, l, k1, k2;
	
	if(init)
		return(0);
	init=1;
	
	for(i=0; i<16; i++)
		for(j=0; j<256; j++)
	{
		k1=btic1h_rice_ntab2[j];

		l=i+k1+1;
		if(l>8)
		{
//			btic1h_rice_valtab[i][j]=-1;
			btic1h_rice_valtab[i][j]=0;
			continue;
		}
		
		k=(k1<<i)|((j>>(8-l))&((1<<i)-1));
		
#if 1
		k2=i;
		if(k1!=1)
		{
			if(k1>1)
			{
				while(k1>1)
					{ k2++; k1=k1>>1; }
				if(k2>15)k2=15;
			}else
			{
				if(k2>0)k2--;
			}
		}
#endif
		
//		btic1h_rice_valtab[i][j]=k|(l<<16)|(k2<<20);
		btic1h_rice_valtab[i][j]=k|(l<<8)|(k2<<12);
	}
	
	return(1);
}

int BTIC1H_Rice_NextByteBasic(BTIC1H_Context *ctx)
{
	int i, j;

	i=*ctx->bs_cs++;
	return(i);
}

int BTIC1H_Rice_NextByteBasic(BTIC1H_Context *ctx);
int BTIC1H_Rice_ReadNBitsBasic(BTIC1H_Context *ctx, int n);
int BTIC1H_Rice_Read8BitsBasic(BTIC1H_Context *ctx);
int BTIC1H_Rice_Read16BitsBasic(BTIC1H_Context *ctx);
void BTIC1H_Rice_SkipNBitsBasic(BTIC1H_Context *ctx, int n);

int BTIC1H_Rice_ReadAdRiceBasic(BTIC1H_Context *ctx, int *rk);
int BTIC1H_Rice_ReadAdSRiceBasic(BTIC1H_Context *ctx, int *rk);
int BTIC1H_Rice_ReadAdRiceDcBasic(BTIC1H_Context *ctx, int *rk);
int BTIC1H_Rice_ReadAdSRiceDcBasic(BTIC1H_Context *ctx, int *rk);

int BTIC1H_Rice_SetupRead(BTIC1H_Context *ctx, byte *buf, int szbuf)
{
	BTIC1H_Rice_InitTables();

	ctx->NextByte=BTIC1H_Rice_NextByteBasic;
	ctx->ReadNBits=BTIC1H_Rice_ReadNBitsBasic;
	ctx->Read8Bits=BTIC1H_Rice_Read8BitsBasic;
	ctx->Read16Bits=BTIC1H_Rice_Read16BitsBasic;
	ctx->SkipNBits=BTIC1H_Rice_SkipNBitsBasic;
	ctx->ReadAdRice=BTIC1H_Rice_ReadAdRiceBasic;
	ctx->ReadAdSRice=BTIC1H_Rice_ReadAdSRiceBasic;
	ctx->ReadAdRiceDc=BTIC1H_Rice_ReadAdRiceDcBasic;
	ctx->ReadAdSRiceDc=BTIC1H_Rice_ReadAdSRiceDcBasic;

	ctx->bs_pos=0;

	ctx->bs_cs=buf;
	ctx->bs_css=buf;
	ctx->bs_cse=buf+szbuf;

	ctx->bs_win=BTIC1H_Rice_NextByte(ctx);
	ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
	ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
	ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);

	return(0);
}

default_inline int BTIC1H_Rice_NextByte(BTIC1H_Context *ctx)
	{ return(ctx->NextByte(ctx)); }
default_inline int BTIC1H_Rice_ReadNBits(BTIC1H_Context *ctx, int n)
	{ return(ctx->ReadNBits(ctx, n)); }
default_inline int BTIC1H_Rice_Read8Bits(BTIC1H_Context *ctx)
	{ return(ctx->Read8Bits(ctx)); }
default_inline int BTIC1H_Rice_Read16Bits(BTIC1H_Context *ctx)
	{ return(ctx->Read16Bits(ctx)); }
default_inline void BTIC1H_Rice_SkipNBits(BTIC1H_Context *ctx, int n)
	{ ctx->SkipNBits(ctx, n); }

default_inline int BTIC1H_Rice_ReadAdRice(BTIC1H_Context *ctx, int *rk)
	{ return(ctx->ReadAdRice(ctx, rk)); }
default_inline int BTIC1H_Rice_ReadAdSRice(BTIC1H_Context *ctx, int *rk)
	{ return(ctx->ReadAdSRice(ctx, rk)); }

default_inline int BTIC1H_Rice_ReadAdRiceDc(BTIC1H_Context *ctx, int *rk)
	{ return(ctx->ReadAdRiceDc(ctx, rk)); }
default_inline int BTIC1H_Rice_ReadAdSRiceDc(BTIC1H_Context *ctx, int *rk)
	{ return(ctx->ReadAdSRiceDc(ctx, rk)); }

int BTIC1H_Rice_ReadBit(BTIC1H_Context *ctx)
{
	int i;

	i=(ctx->bs_win>>(31-ctx->bs_pos))&1;
	ctx->bs_pos++;
	if(ctx->bs_pos>=8)
	{
		ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
		ctx->bs_pos-=8;
	}
	return(i);
}

int BTIC1H_Rice_ReadNBitsBasic(BTIC1H_Context *ctx, int n)
{
	int i, j, k, l;

// #if defined(X86) || defined(X86_64)
#if 0
//	int i, j, k, l;

	j=ctx->bs_pos;	k=ctx->bs_win;
	l=(k>>(32-n-j))&((1<<n)-1);

	i=ctx->bs_pos+n;
	k=i&(~7);
//	j=(ctx->bs_cs[0]<<8)|(ctx->bs_cs[1]);
	j=*(u16 *)ctx->bs_cs; j=(j<<8)|(j>>8);
	ctx->bs_win=(ctx->bs_win<<k)|(j>>(16-k));
	ctx->bs_pos=i&7;
	ctx->bs_cs+=i>>3;
	return(l);
#endif

#if 1
	j=ctx->bs_pos;	k=ctx->bs_win;
	i=(k>>(32-n-j))&((1<<n)-1);
	j+=n;
	if(j>=8)
	{
		if(j>=16)
		{
			k=(k<<8)|BTIC1H_Rice_NextByte(ctx);
			k=(k<<8)|BTIC1H_Rice_NextByte(ctx);
			j-=16;
		}else
		{
			k=(k<<8)|BTIC1H_Rice_NextByte(ctx);
			j-=8;
		}
		ctx->bs_win=k;
	}
	ctx->bs_pos=j;
#endif

//	if(!n)
//		return(0);

#if 0
	j=ctx->bs_pos;	k=ctx->bs_win;
	i=(k>>(32-n-j))&((1<<n)-1);
	j+=n;
	if(j>=8)
	{
		while(j>=8)
		{
			k=(k<<8)|BTIC1H_Rice_NextByte(ctx);
			j-=8;
		}
		ctx->bs_win=k;
	}
	ctx->bs_pos=j;
#endif

#if 0
	i=(ctx->bs_win>>(32-n-ctx->bs_pos))&((1<<n)-1);
	ctx->bs_pos+=n;
	while(ctx->bs_pos>=8)
	{
		ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
		ctx->bs_pos-=8;
	}
#endif
	return(i);
}

#if 0
// force_inline int BTIC1H_Rice_ReadNBitsNoMask(BTIC1H_Context *ctx, int n)
{
	int i, j, k;

#if 1
	j=ctx->bs_pos+n; k=ctx->bs_win;
	i=k>>(32-j);
	if(j>=8)
	{
		k=(k<<8)|(*ctx->bs_cs++); j-=8;
		if(j>=8)
			{ k=(k<<8)|(*ctx->bs_cs++); j-=8; }
		ctx->bs_win=k;
	}
	ctx->bs_pos=j;
	return(i);
#endif

#if 0
	ctx->bs_pos+=n;
	i=ctx->bs_win>>(32-ctx->bs_pos);
//	i=ctx->bs_win>>(32-n-ctx->bs_pos);
//	ctx->bs_pos+=n;
	while(ctx->bs_pos>=8)
	{
		ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
		ctx->bs_pos-=8;
	}
	return(i);
#endif
}
#endif

int BTIC1H_Rice_Read4Bits(BTIC1H_Context *ctx)
	{ return(BTIC1H_Rice_ReadNBits(ctx, 4)); }

//int BTIC1H_Rice_Read8Bits(BTIC1H_Context *ctx)
//	{ return(BTIC1H_Rice_ReadNBits(ctx, 8)); }

//int BTIC1H_Rice_Read16Bits(BTIC1H_Context *ctx)
//	{ return(BTIC1H_Rice_ReadNBits(ctx, 16)); }

int BTIC1H_Rice_Read8BitsBasic(BTIC1H_Context *ctx)
{
	int i;

	i=(ctx->bs_win>>(24-ctx->bs_pos))&255;
//	ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
	BTIC1H_Rice_SkipNBitsBasic(ctx, 8);
	return(i);
}

int BTIC1H_Rice_Read16BitsBasic(BTIC1H_Context *ctx)
{
	int i;

	i=(ctx->bs_win>>(16-ctx->bs_pos))&65535;
//	ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
//	ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
	BTIC1H_Rice_SkipNBitsBasic(ctx, 16);
	return(i);
}

#if 0
int BTIC1H_Rice_Read16Bits(BTIC1H_Context *ctx)
{
	int i, j;
	i=BTIC1H_Rice_ReadNBits(ctx, 8);
	j=BTIC1H_Rice_ReadNBits(ctx, 8);
	return((i<<8)|j);
}
#endif

u32 BTIC1H_Rice_Read24Bits(BTIC1H_Context *ctx)
{
	int i, j;
	i=BTIC1H_Rice_Read8Bits(ctx);
	j=BTIC1H_Rice_Read16Bits(ctx);
	return((i<<16)|j);
}

u32 BTIC1H_Rice_Read32Bits(BTIC1H_Context *ctx)
{
	int i, j;
//	i=BTIC1H_Rice_ReadNBits(ctx, 16);
//	j=BTIC1H_Rice_ReadNBits(ctx, 16);

	i=BTIC1H_Rice_Read16Bits(ctx);
	j=BTIC1H_Rice_Read16Bits(ctx);
	return((i<<16)|j);
}

u64 BTIC1H_Rice_Read48Bits(BTIC1H_Context *ctx)
{
	u64 i, j, k;
	i=BTIC1H_Rice_Read16Bits(ctx);
	j=BTIC1H_Rice_Read16Bits(ctx);
	k=BTIC1H_Rice_Read16Bits(ctx);
	return((i<<32)|(j<<16)|k);
}

void BTIC1H_Rice_SkipNBitsBasic(BTIC1H_Context *ctx, int n)
{
// #if defined(X86) || defined(X86_64)
#if 0
	int i, j, k;
	i=ctx->bs_pos+n;
	k=i&(~7);
//	j=(ctx->bs_cs[0]<<8)|(ctx->bs_cs[1]);
	j=*(u16 *)ctx->bs_cs; j=(j<<8)|(j>>8);
	ctx->bs_win=(ctx->bs_win<<k)|(j>>(16-k));
	ctx->bs_pos=i&7;
	ctx->bs_cs+=i>>3;
	return;
#endif

// #if !defined(X86) && !defined(X86_64)
#if 1
	int i, j;
	i=ctx->bs_pos+n;
	if(i>=8)
	{
		if(i>=16)
		{
//			ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
//			ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
			ctx->bs_win=(ctx->bs_win<<16)|
				(ctx->bs_cs[0]<<8)|(ctx->bs_cs[1]);
			ctx->bs_cs+=2;
			ctx->bs_pos=i-16;
		}else
		{
//			ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
			ctx->bs_win=(ctx->bs_win<<8)|(*ctx->bs_cs++);
			ctx->bs_pos=i-8;
		}
	}else
	{
		ctx->bs_pos=i;
	}
#endif

#if 0
	ctx->bs_pos+=n;
	while(ctx->bs_pos>=8)
	{
		ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
		ctx->bs_pos-=8;
	}
#endif
}

//void BTIC1H_Rice_Skip8BitsBasic(BTIC1H_Context *ctx)
//{
//	ctx->bs_win=(ctx->bs_win<<8)|BTIC1H_Rice_NextByte(ctx);
//}

void BTIC1H_Rice_Skip8Bits(BTIC1H_Context *ctx)
{
	ctx->SkipNBits(ctx, 8);
}

int BTIC1H_Rice_PeekWord(BTIC1H_Context *ctx)
{
	int i;
	i=(ctx->bs_win>>(16-ctx->bs_pos))&65535;
	return(i);
}

force_inline int BTIC1H_Rice_PeekByte(BTIC1H_Context *ctx)
{
	int i;
	i=(ctx->bs_win>>(24-ctx->bs_pos))&255;
	return(i);
}

int BTIC1H_Rice_PeekNibble(BTIC1H_Context *ctx)
{
	int i;
	i=(ctx->bs_win>>(28-ctx->bs_pos))&15;
	return(i);
}


default_inline int BTIC1H_Rice_ReadRiceQ(BTIC1H_Context *ctx)
{
	int i, j, k;

#if 0
	i=0;
	while(BTIC1H_Rice_ReadBit(ctx))
		i++;
	return(i);
#endif

	k=0;
	i=BTIC1H_Rice_PeekByte(ctx);
	if(i==0xFF)
	{
		k=0;
		while(i==0xFF)
		{
//			BTIC1H_Rice_ReadNBits(ctx, 8);
			BTIC1H_Rice_Skip8Bits(ctx);
			k+=8;
			i=BTIC1H_Rice_PeekByte(ctx);
		}
//		j=btic1h_rice_ntab[i>>4];
//		if(j>=4)
//			{ j+=btic1h_rice_ntab[i&15]; }
		j=btic1h_rice_ntab2[i];
//		BTIC1H_Rice_ReadNBits(ctx, j+1);
		BTIC1H_Rice_SkipNBits(ctx, j+1);
		i=k+j;
		return(i);
	}else
	{
//		j=btic1h_rice_ntab[i>>4];
//		if(j>=4)
//			{ j+=btic1h_rice_ntab[i&15]; }
		j=btic1h_rice_ntab2[i];
//		BTIC1H_Rice_ReadNBits(ctx, j+1);
		BTIC1H_Rice_SkipNBits(ctx, j+1);
		return(j);
	}
}

int BTIC1H_Rice_ReadRice(BTIC1H_Context *ctx, int k)
{
	int i, j;
	
	i=BTIC1H_Rice_ReadRiceQ(ctx);
	j=BTIC1H_Rice_ReadNBits(ctx, k);
	i=(i<<k)|j;
	return(i);
}

#if 0
// int BTIC1H_Rice_ReadAdRice2(BTIC1H_Context *ctx, int *rk)
{
	int i, j, k;
	
	k=*rk;
	i=BTIC1H_Rice_ReadRiceQ(ctx);
	j=BTIC1H_Rice_ReadNBits(ctx, k);
	j=(i<<k)|j;

#if 1
	if(i!=1)
	{
		if(i>1)
		{
			while(i>1)
				{ k++; i=i>>1; }
			if(k>15)k=15;
		}else
		{
			if(k>0)k--;
//			k--;
//			if(k<0)k=0;
		}
		*rk=k;
	}
#endif

	return(j);
}
#endif

#if 1
default_inline int BTIC1H_Rice_ReadAdRiceI(BTIC1H_Context *ctx, int *rk)
{
	int i, j, k, l;
	
	k=*rk;
	i=BTIC1H_Rice_PeekByte(ctx);

	if(i!=0xFF)
	{
		j=btic1h_rice_ntab2[i];
		
		l=j+k+1;
		if(l<=16)
		{
			i=BTIC1H_Rice_ReadNBitsBasic(ctx, l);
//			i=BTIC1H_Rice_ReadNBitsNoMask(ctx, l);
			i=(j<<k)|(i&((1<<k)-1));

			if(j!=1)
			{
				if(j>1)
				{
					while(j>1)
						{ k++; j=j>>1; }
				}else
				{
					if(k>0)k--;
				}
				*rk=k;
			}

			return(i);
		}
	}

	i=BTIC1H_Rice_ReadRiceQ(ctx);
	j=BTIC1H_Rice_ReadNBitsBasic(ctx, k);
	j=(i<<k)|j;

#if 1
	if(i!=1)
	{
		if(i>1)
		{
			while(i>1)
				{ k++; i=i>>1; }
			if(k>15)k=15;
		}else
		{
			if(k>0)k--;
		}
		*rk=k;
	}
#endif

	return(j);
}

int BTIC1H_Rice_ReadAdRiceBasic(BTIC1H_Context *ctx, int *rk)
{
	int i, j, k, l;
	
#if 1
//	if(k<8)
	if(1)
	{
		k=*rk;
		i=BTIC1H_Rice_PeekByte(ctx);
		j=btic1h_rice_valtab[k][i];
//		if(j>0)
		if(j)
		{
//			i=j&65535;
//			l=(j>>16)&15;
//			*rk=(j>>20)&15;

			i=j&255;
			l=(j>>8)&15;
			*rk=(j>>12)&15;

			BTIC1H_Rice_SkipNBitsBasic(ctx, l);
			return(i);
		}
	}
#endif

	i=BTIC1H_Rice_ReadAdRiceI(ctx, rk);
	return(i);
}

#endif

int BTIC1H_Rice_ReadAdSRiceBasic(BTIC1H_Context *ctx, int *rk)
{
	int i, j, k, l;
	
#if 1
//	if(k<8)
	if(1)
	{
		k=*rk;
		i=BTIC1H_Rice_PeekByte(ctx);
		j=btic1h_rice_valtab[k][i];
//		if(j>0)
		if(j)
		{
//			i=j&65535;
//			l=(j>>16)&15;
//			*rk=(j>>20)&15;

			i=j&255;
			l=(j>>8)&15;
			*rk=(j>>12)&15;

			i=(i>>1)^((i<<31)>>31);
			BTIC1H_Rice_SkipNBitsBasic(ctx, l);
			return(i);
		}
	}
#endif

	i=BTIC1H_Rice_ReadAdRiceI(ctx, rk);
	i=(i>>1)^((i<<31)>>31);
	return(i);
}

#if 1
default_inline int BTIC1H_Rice_ReadAdRiceDcI(BTIC1H_Context *ctx, int *rk)
{
	int i, j, k, l;
	
	k=*rk;
	i=BTIC1H_Rice_PeekByte(ctx);

	if(i!=0xFF)
	{
		j=btic1h_rice_ntab2[i];
		
		l=j+k+1;
		if(l<=16)
		{
			i=BTIC1H_Rice_ReadNBits(ctx, l);
//			i=BTIC1H_Rice_ReadNBitsNoMask(ctx, l);
			i=(j<<k)|(i&((1<<k)-1));

			if(j!=1)
			{
				if(j>1)
				{
					while(j>1)
						{ k++; j=j>>1; }
				}else
				{
					if(k>0)k--;
				}
				*rk=k;
			}

			return(i);
		}
	}

	i=BTIC1H_Rice_ReadRiceQ(ctx);
	if(i>=(8+k))
	{
		i=i-(7+k);
		j=BTIC1H_Rice_ReadNBits(ctx, i*(5+k));
		k=k+2*i;
		if(k>15)k=15;
		*rk=k;
		return(j);
	}
	
	j=BTIC1H_Rice_ReadNBits(ctx, k);
	j=(i<<k)|j;

	if(i!=1)
	{
		if(i>1)
		{
			while(i>1)
				{ k++; i=i>>1; }
			if(k>15)k=15;
		}else
		{
			if(k>0)k--;
		}
		*rk=k;
	}
	return(j);
}

default_inline int BTIC1H_Rice_ReadAdRiceDcBasic(
	BTIC1H_Context *ctx, int *rk)
{
	int i, j, k, l;
	
	k=*rk;
	i=BTIC1H_Rice_PeekByte(ctx);
	j=btic1h_rice_valtab[k][i];
	if(j)
	{
		i=j&255;
		l=(j>>8)&15;
		*rk=(j>>12)&15;

		BTIC1H_Rice_SkipNBitsBasic(ctx, l);
		return(i);
	}

	i=BTIC1H_Rice_ReadAdRiceDcI(ctx, rk);
	return(i);
}

#endif

default_inline int BTIC1H_Rice_ReadAdSRiceDcBasic(
	BTIC1H_Context *ctx, int *rk)
{
	int i, j, k, l;

	k=*rk;
	i=BTIC1H_Rice_PeekByte(ctx);
	j=btic1h_rice_valtab[k][i];
	if(j)
	{
		i=j&255;
		l=(j>>8)&15;
		*rk=(j>>12)&15;

		i=(i>>1)^((i<<31)>>31);
		BTIC1H_Rice_SkipNBitsBasic(ctx, l);
		return(i);
	}

	i=BTIC1H_Rice_ReadAdRiceDcI(ctx, rk);
	i=(i>>1)^((i<<31)>>31);
	return(i);
}


int BTIC1H_DecodeReadVLI(BTIC1H_Context *ctx, byte **rcs)
{
	byte *cs;
	int i;
	
	cs=*rcs;
	i=*cs++;
	
	if(i<0x80)
	{
	}else if(i<0xC0)
	{
		i=i&0x3F;
		i=(i<<8)|(*cs++);
	}else if(i<0xE0)
	{
		i=i&0x1F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
	}else if(i<0xF0)
	{
		i=i&0x0F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
	}else if(i<0xF8)
	{
		i=i&0x07;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
	}
	
	*rcs=cs;
	return(i);
}

int BTIC1H_Rice_SetupRead2(
	BTIC1H_Context *ctx, byte *buf, int szbuf)
{
	byte *cs, *cse, *cs1, *cs2;
	int cim, cxbits;
	int i, j, k, l;
	
	cs=buf; cse=buf+szbuf;
	
	i=*cs++;
	cim=i&15;
	cxbits=8+((i>>4)&7);
	
	ctx->slscl=0;
	ctx->clryuv=0;
	ctx->clrdty=0;

	while(i&128)
	{
		i=cs[0];
		l=(i>>4)&7;
//		if(!l)l=cs[1];
//		cs1=cs+l;

		j=i&15;
		cs1=cs+1;
		if(l==0)
			{ l=BTIC1H_DecodeReadVLI(ctx, &cs1); }
		if(j==0)
			{ j=BTIC1H_DecodeReadVLI(ctx, &cs1); }
		cs2=cs1;
		cs1=cs+l;

//		switch(i&15)
		switch(j)
		{
		case 1:
			ctx->slscl=BTIC1H_DecodeReadVLI(ctx, &cs2)*4;
			break;
		case 2:
			j=BTIC1H_DecodeReadVLI(ctx, &cs2);
			ctx->clryuv=j&7;
			ctx->clrdty=(j>>3)&7;
			break;
		default:
			break;
		}
		
		cs=cs1;
	}

	switch(cim)
	{
	case 0:
		BTIC1H_Rice_SetupRead(ctx, cs, cse-cs);
		break;
	case 1:
//		BTIC1H_Range_SetupContextBits(ctx, cxbits);
//		BTIC1H_Rice_SetupReadRange(ctx, cs, cse-cs);
		break;
	default:
		break;
	}
	return(0);
}