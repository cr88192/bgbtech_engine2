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

#include <bteifgl.h>

static const byte bgbdt_rice_ntab[16]={
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 2, 2, 3, 4};

static const byte bgbdt_rice_ntab2[256]={
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

static int bgbdt_rice_valtab[16][256];

int BGBDT_Rice_InitTables()
{
	static int init=0;
	int i, j, k, l, k1, k2;
	
	if(init)
		return(0);
	init=1;
	
	for(i=0; i<16; i++)
		for(j=0; j<256; j++)
	{
		k1=bgbdt_rice_ntab2[j];

		l=i+k1+1;
		if(l>8)
		{
			bgbdt_rice_valtab[i][j]=-1;
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
		
		bgbdt_rice_valtab[i][j]=k|(l<<16)|(k2<<20);
	}
	
	return(1);
}

int BGBDT_Rice_NextByte(BGBDT_RiceContext *ctx)
{
	int i, j;

	i=*ctx->bs_cs++;
	return(i);
}

int BGBDT_Rice_SetupRead(BGBDT_RiceContext *ctx, byte *buf, int szbuf)
{
	BGBDT_Rice_InitTables();

	ctx->bs_pos=0;

	ctx->bs_cs=buf;
	ctx->bs_cse=buf+szbuf;
	
	ctx->bs_win=BGBDT_Rice_NextByte(ctx);
	ctx->bs_win=(ctx->bs_win<<8)|BGBDT_Rice_NextByte(ctx);
	ctx->bs_win=(ctx->bs_win<<8)|BGBDT_Rice_NextByte(ctx);
	ctx->bs_win=(ctx->bs_win<<8)|BGBDT_Rice_NextByte(ctx);
	return(0);
}

int BGBDT_Rice_ReadBit(BGBDT_RiceContext *ctx)
{
	int i;

	i=(ctx->bs_win>>(31-ctx->bs_pos))&1;
	ctx->bs_pos++;
	if(ctx->bs_pos>=8)
	{
		ctx->bs_win=(ctx->bs_win<<8)|BGBDT_Rice_NextByte(ctx);
		ctx->bs_pos-=8;
	}
	return(i);
}

int BGBDT_Rice_ReadNBits(BGBDT_RiceContext *ctx, int n)
{
	int i, j, k;

	j=ctx->bs_pos;	k=ctx->bs_win;
	i=(k>>(32-n-j))&((1<<n)-1);
	j+=n;
	if(j>=8)
	{
		if(j>=16)
		{
			k=(k<<8)|BGBDT_Rice_NextByte(ctx);
			k=(k<<8)|BGBDT_Rice_NextByte(ctx);
			j-=16;
		}else
		{
			k=(k<<8)|BGBDT_Rice_NextByte(ctx);
			j-=8;
		}
		ctx->bs_win=k;
	}
	ctx->bs_pos=j;

	return(i);
}

int BGBDT_Rice_ReadNBitsNoMask(BGBDT_RiceContext *ctx, int n)
{
	int i, j, k;

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
}

int BGBDT_Rice_Read4Bits(BGBDT_RiceContext *ctx)
	{ return(BGBDT_Rice_ReadNBits(ctx, 4)); }

int BGBDT_Rice_Read8Bits(BGBDT_RiceContext *ctx)
{
	int i;

	i=(ctx->bs_win>>(24-ctx->bs_pos))&255;
	ctx->bs_win=(ctx->bs_win<<8)|BGBDT_Rice_NextByte(ctx);
	return(i);
}

int BGBDT_Rice_Read16Bits(BGBDT_RiceContext *ctx)
{
	int i;

	i=(ctx->bs_win>>(16-ctx->bs_pos))&65535;
	ctx->bs_win=(ctx->bs_win<<8)|BGBDT_Rice_NextByte(ctx);
	ctx->bs_win=(ctx->bs_win<<8)|BGBDT_Rice_NextByte(ctx);
	return(i);
}

u32 BGBDT_Rice_Read32Bits(BGBDT_RiceContext *ctx)
{
	int i, j;
	i=BGBDT_Rice_Read16Bits(ctx);
	j=BGBDT_Rice_Read16Bits(ctx);
	return((i<<16)|j);
}

u64 BGBDT_Rice_Read48Bits(BGBDT_RiceContext *ctx)
{
	u64 i, j, k;
	i=BGBDT_Rice_Read16Bits(ctx);
	j=BGBDT_Rice_Read16Bits(ctx);
	k=BGBDT_Rice_Read16Bits(ctx);
	return((i<<32)|(j<<16)|k);
}

void BGBDT_Rice_SkipNBits(BGBDT_RiceContext *ctx, int n)
{
	int i, j;
	i=ctx->bs_pos+n;
	if(i>=8)
	{
		if(i>=16)
		{
			ctx->bs_win=(ctx->bs_win<<16)|
				(ctx->bs_cs[0]<<8)|(ctx->bs_cs[1]);
			ctx->bs_cs+=2;
			ctx->bs_pos=i-16;
		}else
		{
			ctx->bs_win=(ctx->bs_win<<8)|(*ctx->bs_cs++);
			ctx->bs_pos=i-8;
		}
	}else
	{
		ctx->bs_pos=i;
	}
}

void BGBDT_Rice_Skip8Bits(BGBDT_RiceContext *ctx)
{
	ctx->bs_win=(ctx->bs_win<<8)|BGBDT_Rice_NextByte(ctx);
}

int BGBDT_Rice_PeekWord(BGBDT_RiceContext *ctx)
{
	int i;
	i=(ctx->bs_win>>(16-ctx->bs_pos))&65535;
	return(i);
}

int BGBDT_Rice_PeekByte(BGBDT_RiceContext *ctx)
{
	int i;
	i=(ctx->bs_win>>(24-ctx->bs_pos))&255;
	return(i);
}

int BGBDT_Rice_PeekNibble(BGBDT_RiceContext *ctx)
{
	int i;
	i=(ctx->bs_win>>(28-ctx->bs_pos))&15;
	return(i);
}


int BGBDT_Rice_ReadRiceQ(BGBDT_RiceContext *ctx)
{
	int i, j, k;

	k=0;
	i=BGBDT_Rice_PeekByte(ctx);
	if(i==0xFF)
	{
		k=0;
		while(i==0xFF)
		{
			BGBDT_Rice_Skip8Bits(ctx);
			k+=8;
			i=BGBDT_Rice_PeekByte(ctx);
		}
		j=bgbdt_rice_ntab2[i];
		BGBDT_Rice_SkipNBits(ctx, j+1);
		i=k+j;
		return(i);
	}else
	{
		j=bgbdt_rice_ntab2[i];
		BGBDT_Rice_SkipNBits(ctx, j+1);
		return(j);
	}
}

int BGBDT_Rice_ReadRice(BGBDT_RiceContext *ctx, int k)
{
	int i, j;
	
	i=BGBDT_Rice_ReadRiceQ(ctx);
	j=BGBDT_Rice_ReadNBits(ctx, k);
	i=(i<<k)|j;
	return(i);
}

int BGBDT_Rice_ReadAdRice2(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k;
	
	k=*rk;
	i=BGBDT_Rice_ReadRiceQ(ctx);
	j=BGBDT_Rice_ReadNBits(ctx, k);
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

#if 1
int BGBDT_Rice_ReadAdRiceI(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k, l;
	
	k=*rk;
	i=BGBDT_Rice_PeekByte(ctx);

	if(i!=0xFF)
	{
		j=bgbdt_rice_ntab2[i];
		
		l=j+k+1;
		if(l<=16)
		{
//			i=BGBDT_Rice_ReadNBits(ctx, l);
			i=BGBDT_Rice_ReadNBitsNoMask(ctx, l);
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

	i=BGBDT_Rice_ReadRiceQ(ctx);
	j=BGBDT_Rice_ReadNBits(ctx, k);
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

int BGBDT_Rice_ReadAdRice(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k, l;
	
#if 1
//	if(k<8)
	if(1)
	{
		k=*rk;
		i=BGBDT_Rice_PeekByte(ctx);
		j=bgbdt_rice_valtab[k][i];
		if(j>=0)
		{
			i=j&65535;
			l=(j>>16)&15;
			*rk=(j>>20)&15;
			BGBDT_Rice_SkipNBits(ctx, l);
			return(i);
		}
	}
#endif

	i=BGBDT_Rice_ReadAdRiceI(ctx, rk);
	return(i);
}

#endif

int BGBDT_Rice_ReadAdSRice(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k, l;
	
#if 1
//	if(k<8)
	if(1)
	{
		k=*rk;
		i=BGBDT_Rice_PeekByte(ctx);
		j=bgbdt_rice_valtab[k][i];
		if(j>=0)
		{
			i=j&65535;
			l=(j>>16)&15;
			*rk=(j>>20)&15;
			i=(i>>1)^((i<<31)>>31);
			BGBDT_Rice_SkipNBits(ctx, l);
			return(i);
		}
	}
#endif

	i=BGBDT_Rice_ReadAdRiceI(ctx, rk);
	i=(i>>1)^((i<<31)>>31);
	return(i);
}


#if 1
int BGBDT_Rice_ReadRiceDcI(BGBDT_RiceContext *ctx, int rk)
{
	int i, j, k, l;
	
	k=rk;
	i=BGBDT_Rice_PeekByte(ctx);

	if(i!=0xFF)
	{
		j=bgbdt_rice_ntab2[i];
		
		l=j+k+1;
		if(l<=16)
		{
//			i=BGBDT_Rice_ReadNBits(ctx, l);
			i=BGBDT_Rice_ReadNBitsNoMask(ctx, l);
			i=(j<<k)|(i&((1<<k)-1));
			return(i);
		}
	}

	i=BGBDT_Rice_ReadRiceQ(ctx);
	if(i>=(8+k))
	{
		i=i-(7+k);
		j=BGBDT_Rice_ReadNBits(ctx, i*(5+k));
		return(j);
	}
	
	j=BGBDT_Rice_ReadNBits(ctx, k);
	j=(i<<k)|j;
	return(j);
}

int BGBDT_Rice_ReadRiceDc(BGBDT_RiceContext *ctx, int rk)
{
	int i, j, k, l;
	
	k=rk;
	i=BGBDT_Rice_PeekByte(ctx);
	j=bgbdt_rice_valtab[k][i];
	if(j>=0)
	{
		i=j&65535;
		l=(j>>16)&15;
		BGBDT_Rice_SkipNBits(ctx, l);
		return(i);
	}

	i=BGBDT_Rice_ReadRiceDcI(ctx, rk);
	return(i);
}
#endif

#if 1
int BGBDT_Rice_ReadAdRiceDcI(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k, l;
	
	k=*rk;
	i=BGBDT_Rice_PeekByte(ctx);

	if(i!=0xFF)
	{
		j=bgbdt_rice_ntab2[i];
		
		l=j+k+1;
		if(l<=16)
		{
//			i=BGBDT_Rice_ReadNBits(ctx, l);
			i=BGBDT_Rice_ReadNBitsNoMask(ctx, l);
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

	i=BGBDT_Rice_ReadRiceQ(ctx);
	if(i>=(8+k))
	{
		i=i-(7+k);
		j=BGBDT_Rice_ReadNBits(ctx, i*(5+k));
		k=k+2*i;
		if(k>15)k=15;
		*rk=k;
		return(j);
	}
	
	j=BGBDT_Rice_ReadNBits(ctx, k);
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

int BGBDT_Rice_ReadAdRiceDc(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k, l;
	
	k=*rk;
	i=BGBDT_Rice_PeekByte(ctx);
	j=bgbdt_rice_valtab[k][i];
	if(j>=0)
	{
		i=j&65535;
		l=(j>>16)&15;
		*rk=(j>>20)&15;
		BGBDT_Rice_SkipNBits(ctx, l);
		return(i);
	}

	i=BGBDT_Rice_ReadAdRiceDcI(ctx, rk);
	return(i);
}

#endif

#if 1
int BGBDT_Rice_ReadAdRiceLLI(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k, l;
	
	k=*rk;
	i=BGBDT_Rice_PeekByte(ctx);

	if(i!=0xFF)
	{
		j=bgbdt_rice_ntab2[i];
		
		l=j+k+1;
		if(l<=16)
		{
//			i=BGBDT_Rice_ReadNBits(ctx, l);
			i=BGBDT_Rice_ReadNBitsNoMask(ctx, l);
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

	i=BGBDT_Rice_ReadRiceQ(ctx);
	if(i>=8)
	{
		i=i-8;
		j=BGBDT_Rice_ReadNBits(ctx, 5+i*3);
		k=k+3+i;
		if(k>15)k=15;
		*rk=k;
		return(j);
	}
	
	j=BGBDT_Rice_ReadNBits(ctx, k);
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

int BGBDT_Rice_ReadAdRiceLL(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k, l;
	
	k=*rk;
	i=BGBDT_Rice_PeekByte(ctx);
	j=bgbdt_rice_valtab[k][i];
	if(j>=0)
	{
		i=j&65535;
		l=(j>>16)&15;
		*rk=(j>>20)&15;
		BGBDT_Rice_SkipNBits(ctx, l);
		return(i);
	}

	i=BGBDT_Rice_ReadAdRiceLLI(ctx, rk);
	return(i);
}

#endif

int BGBDT_Rice_ReadAdSRiceDc(BGBDT_RiceContext *ctx, int *rk)
{
	int i, j, k, l;

	k=*rk;
	i=BGBDT_Rice_PeekByte(ctx);
	j=bgbdt_rice_valtab[k][i];
	if(j>=0)
	{
		i=j&65535;
		l=(j>>16)&15;
		*rk=(j>>20)&15;
		i=(i>>1)^((i<<31)>>31);
		BGBDT_Rice_SkipNBits(ctx, l);
		return(i);
	}

	i=BGBDT_Rice_ReadAdRiceDcI(ctx, rk);
	i=(i>>1)^((i<<31)>>31);
	return(i);
}


int BGBDT_Rice_ReadQExpBase(BGBDT_RiceContext *ctx, int k)
{
	int i, j;
	
	i=BGBDT_Rice_ReadRiceQ(ctx);
	j=BGBDT_Rice_ReadNBits(ctx, (i+1)*k);
//	i=(i<<k)|j;
	return(j);
}

int BGBDT_Rice_ReadAdExp2Rice(BGBDT_RiceContext *ctx, int *rk)
{
	int p, v;
	int i, j, k;
	
	p=BGBDT_Rice_ReadAdRiceLL(ctx, rk);
	j=p>>4; i=p&15;
	k=BGBDT_Rice_ReadNBits(ctx, j);
	v=(i<<j)|k;
	return(v);
}
