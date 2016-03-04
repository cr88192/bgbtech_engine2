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

BGBDT_RiceContext *bgbdt_rice_freectx=NULL;

BGBDT_RiceContext *BGBDT_Rice_AllocContext(void)
{
	BGBDT_RiceContext *tmp;

	if(bgbdt_rice_freectx)
	{
		tmp=bgbdt_rice_freectx;
		bgbdt_rice_freectx=tmp->next;
		memset(tmp, 0, sizeof(BGBDT_RiceContext));
		return(tmp);
	}
	
	tmp=frgl_malloc(sizeof(BGBDT_RiceContext));
	return(tmp);
}

void BGBDT_Rice_FreeContext(BGBDT_RiceContext *ctx)
{
	ctx->next=bgbdt_rice_freectx;
	bgbdt_rice_freectx=ctx;
}


void BGBDT_Rice_SetupWrite(BGBDT_RiceContext *ctx, byte *obuf, int osz)
{
	ctx->bs_win=0;
	ctx->bs_pos=0;
	ctx->bs_bits=0;

	ctx->bs_ct=obuf;
	ctx->bs_cte=obuf+osz;
}

void BGBDT_Rice_EmitByte(BGBDT_RiceContext *ctx, int i)
{
	*ctx->bs_ct++=i;
}

void BGBDT_Rice_WriteBit(BGBDT_RiceContext *ctx, int i)
{
	ctx->bs_bits++;

	ctx->bs_win|=(i&1)<<(31-ctx->bs_pos);
	ctx->bs_pos++;
	if(ctx->bs_pos>=8)
	{
		i=(ctx->bs_win>>24)&0xFF;
		BGBDT_Rice_EmitByte(ctx, i);
		ctx->bs_win<<=8;
		ctx->bs_pos-=8;
	}
}

void BGBDT_Rice_WriteNBits(BGBDT_RiceContext *ctx, int v, int n)
{
	int i, j, k;

	ctx->bs_bits+=n;

	v&=(1<<n)-1;

	j=ctx->bs_pos+n;
	k=ctx->bs_win|(v<<(32-j));
	if(j>=8)
	{
		*ctx->bs_ct++=(k>>24); k<<=8; j-=8;
		if(j>=8)
			{ *ctx->bs_ct++=(k>>24); k<<=8; j-=8; }
	}
	ctx->bs_pos=j;
	ctx->bs_win=k;
}

void BGBDT_Rice_WriteNBitsH(BGBDT_RiceContext *ctx, u32 i, int n)
{
	if(n>16)
	{
		BGBDT_Rice_WriteNBits(ctx, i>>(n-16), 16);
		n-=16;
	}
	BGBDT_Rice_WriteNBits(ctx, i, n);
}

void BGBDT_Rice_WriteNBitsL(BGBDT_RiceContext *ctx, s64 i, int n)
{
	while(n>16)
	{
		BGBDT_Rice_WriteNBits(ctx, i>>(n-16), 16);
		n-=16;
	}
	BGBDT_Rice_WriteNBits(ctx, i, n);
}

void BGBDT_Rice_Write2Bits(BGBDT_RiceContext *ctx, int i)
	{ BGBDT_Rice_WriteNBits(ctx, i, 2); }
void BGBDT_Rice_Write3Bits(BGBDT_RiceContext *ctx, int i)
	{ BGBDT_Rice_WriteNBits(ctx, i, 3); }
void BGBDT_Rice_Write4Bits(BGBDT_RiceContext *ctx, int i)
	{ BGBDT_Rice_WriteNBits(ctx, i, 4); }

void BGBDT_Rice_Write8Bits(BGBDT_RiceContext *ctx, int v)
{
	int i, j, k;

	ctx->bs_bits+=8;
	k=ctx->bs_win|((v&255)<<(24-ctx->bs_pos));
	*ctx->bs_ct++=k>>24;
	ctx->bs_win=k<<8;
}

void BGBDT_Rice_Write16Bits(BGBDT_RiceContext *ctx, int v)
{
	int i, j, k;

	ctx->bs_bits+=16;
	k=ctx->bs_win|((v&65535)<<(16-ctx->bs_pos));
	*ctx->bs_ct++=k>>24;
	*ctx->bs_ct++=k>>16;
	ctx->bs_win=k<<16;
}

void BGBDT_Rice_WriteRice(BGBDT_RiceContext *ctx, int v, int k)
{
	int j;
	
	j=v>>k;
	while(j--)
	 { BGBDT_Rice_WriteBit(ctx, 1); }
	BGBDT_Rice_WriteBit(ctx, 0);
	j=v&((1<<k)-1);
	BGBDT_Rice_WriteNBits(ctx, j, k);
}

int BGBDT_Rice_CountWriteRice(BGBDT_RiceContext *ctx, int v, int k)
{
	int j;
	j=v>>k;
	return(j+k);
}

void BGBDT_Rice_WriteSRice(BGBDT_RiceContext *ctx, int v, int k)
{
	BGBDT_Rice_WriteRice(ctx, (v>=0)?(v<<1):(((-v)<<1)-1), k);
}

int BGBDT_Rice_CountWriteSRice(BGBDT_RiceContext *ctx, int v, int k)
{
	return(BGBDT_Rice_CountWriteRice(ctx,
		(v>=0)?(v<<1):(((-v)<<1)-1), k));
}

void BGBDT_Rice_WriteAdRice(BGBDT_RiceContext *ctx, int v, int *rk)
{
	int p, b, n, bp, bw;
	int i, j, k;
	
	k=*rk;

	p=v>>k;

	i=p+k+1;
	if(i<=16)
	{
		j=v&((1<<k)-1);
		BGBDT_Rice_WriteNBits(ctx, (0xFFFE<<k)|j, i);

		if(p!=1)
		{
			if(!p)
				{ if(k>0)k--; }
			else if(p>1)
			{
				j=p;
				while(j>1)
					{ k++; j>>=1; }
			}
			*rk=k;
		}
		return;
	}

	j=p;

	while(j>=16)
		{ BGBDT_Rice_WriteNBits(ctx, 0xFFFF, 16); j-=16; }
	BGBDT_Rice_WriteNBits(ctx, 0xFFFE, j+1);

	BGBDT_Rice_WriteNBits(ctx, v, k);

	if(p!=1)
	{
		if(p>1)
		{
			j=p;
			while(j>1)
				{ k++; j>>=1; }
		}else
		{
			if(k>0)k--;
		}
		*rk=k;
	}
}

int BGBDT_Rice_CountWriteAdRice(BGBDT_RiceContext *ctx, int v, int *rk)
{
	int j, k, p, n;
	
	k=*rk;
	p=v>>k;
	n=p+k;

	if(!p && (k>0))k--;
	if(p>1)
	{
		j=p;
		while(j>1)
			{ k++; j>>=1; }
	}
	*rk=k;
	
	return(n);
}

void BGBDT_Rice_WriteAdSRice(BGBDT_RiceContext *ctx, int v, int *rk)
{
	BGBDT_Rice_WriteAdRice(ctx, (v<<1)^(v>>31), rk);
}

void BGBDT_Rice_WriteAdExpRice(BGBDT_RiceContext *ctx, int v, int *rk)
{
	int i, j, k;
	
	if(v<=0)
	{
		BGBDT_Rice_WriteAdRice(ctx, 0, rk);
		return;
	}
	
	i=v; j=0; k=*rk;
	while(i>0) { i>>=1; j++; }
	BGBDT_Rice_WriteAdRice(ctx, j, rk);
	j=v&((1<<k)-1);
	BGBDT_Rice_WriteNBitsL(ctx, j, i);
}

void BGBDT_Rice_WriteAdExpSRice(BGBDT_RiceContext *ctx, int v, int *rk)
{
	BGBDT_Rice_WriteAdExpRice(ctx, (v>=0)?(v<<1):(((-v)<<1)-1), rk);
}

int BGBDT_Rice_CountWriteAdRiceDc(BGBDT_RiceContext *ctx, int v, int *rk)
{
	int i, j, k, p, n;
	
	k=*rk;
	p=v>>k;
	n=p+k;

	if(p>=(8+k))
	{
		j=v; i=1;

		while(j>=(1<<(5+k)))
			{ j=j>>(5+k); i++; }
		n=8+(6+k)*i;

		k=k+2*i;
		if(k>15)k=15;
		*rk=k;
		return(n);
	}

	if(!p && (k>0))k--;
	if(p>1)
	{
		j=p;
		while(j>1)
			{ k++; j>>=1; }
	}
	*rk=k;
	
	return(n);
}

int BGBDT_Rice_CountWriteAdSRiceDc(BGBDT_RiceContext *ctx, int v, int *rk)
	{ return(BGBDT_Rice_CountWriteAdRiceDc(ctx, (v<<1)^(v>>31), rk)); }

void BGBDT_Rice_WriteAdRiceDc(BGBDT_RiceContext *ctx, int v, int *rk)
{
	int p, b, n, bp, bw;
	int i, j, k;
	
	k=*rk;

	p=v>>k;

	if(p>=(8+k))
	{
		j=v; i=1;

		while(j>=(1<<(5+k)))
			{ j=j>>(5+k); i++; }

		BGBDT_Rice_WriteNBitsH(ctx, 0xFFFFFFFEUL, 8+k+i);
		BGBDT_Rice_WriteNBitsH(ctx, v, i*(5+k));

		k+=2*i;
		if(k>15)k=15;
		*rk=k;
		return;
	}

	i=p+k+1;
	if(i<=16)
	{
		j=v&((1<<k)-1);
		BGBDT_Rice_WriteNBits(ctx, (0xFFFE<<k)|j, i);

		if(p!=1)
		{
			if(!p)
				{ if(k>0)k--; }
			else if(p>1)
			{
				j=p;
				while(j>1)
					{ k++; j>>=1; }
				if(k>15)k=15;
			}
			*rk=k;
		}
		return;
	}

	j=p;

	while(j>=16)
		{ BGBDT_Rice_WriteNBits(ctx, 0xFFFF, 16); j-=16; }
	BGBDT_Rice_WriteNBits(ctx, 0xFFFE, j+1);

	BGBDT_Rice_WriteNBits(ctx, v, k);

	if(p!=1)
	{
		if(p>1)
		{
			j=p;
			while(j>1)
				{ k++; j>>=1; }
			if(k>15)k=15;
		}else
		{
			if(k>0)k--;
		}
		*rk=k;
	}
}

void BGBDT_Rice_WriteAdSRiceDc(BGBDT_RiceContext *ctx, int v, int *rk)
{
	BGBDT_Rice_WriteAdRiceDc(ctx, (v<<1)^(v>>31), rk);
}

void BGBDT_Rice_FlushBits(BGBDT_RiceContext *ctx)
{
	int i;
	while(ctx->bs_pos>0)
	{
		i=(ctx->bs_win>>24)&0xFF;
		BGBDT_Rice_EmitByte(ctx, i);
		ctx->bs_win<<=8;
		ctx->bs_pos-=8;
	}
	ctx->bs_pos=0;
}
