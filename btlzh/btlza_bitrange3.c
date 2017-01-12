/*
Bitwise Arithmetic Coder
 */

// #include <btlzazip.h>

byte bgbbtj_bitarith3_divtab8[256];		//division table
byte bgbbtj_bitarith3_transtab8[512];	//transition table

extern void *bgbbtj_btlza_mutex;


int btlza_ar3_ctxindex(int ctx, int mask)
{
	int i;

//	i=((ctx>>8)*251)>>8;
//	return((ctx^(i<<8))&mask);
	
	i=((ctx>>16)*251)>>8;
	return((ctx^(i<<16))&mask);

//	return(ctx&mask);
}

void BTLZA_BitArith3_Init()
{
	static int init=0;
	int i, j, k, l;

	if(init)return;
	init=1;

	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
	{
		k=((i+1)<<BTLZA_BITARITH_WBITS)/(i+j+2);
		bgbbtj_bitarith3_divtab8[i+(j<<4)]=k;
		
		k=i; l=j+1;
		if((k>=16) || (l>=16)) { k=k>>1; l=l>>1; }
		bgbbtj_bitarith3_transtab8[i+(j<<4)+(0<<8)]=k+(l<<4);

		k=i+1; l=j;
		if((k>=16) || (l>=16)) { k=k>>1; l=l>>1; }
		bgbbtj_bitarith3_transtab8[i+(j<<4)+(1<<8)]=k+(l<<4);
	}
}

int BTLZA_BitArith3_InputByte(BTLZA_Context *ctx)
{
//	if(ctx->cs>ctx->cse)
//		return(0);
	return(*ctx->cs++);
}

void BTLZA_BitArith3_OutputByte(BTLZA_Context *ctx, int i)
{
//	if(ctx->ct>ctx->cte)
//		return;
	*ctx->ct++=i;
}

void BTLZA_BitArith3_NormalizeOutput(BTLZA_Context *ctx)
{
	while(!((ctx->rmin^ctx->rmax)>>24))
	{
		BTLZA_BitArith3_OutputByte(ctx, ctx->rmin>>24);
		ctx->rmin<<=8;
		ctx->rmax=(ctx->rmax<<8)|255;
	}
}

void BTLZA_BitArith3_NormalizeInput(BTLZA_Context *ctx)
{
	while(!((ctx->rmin^ctx->rmax)>>24))
	{
		ctx->rmin<<=8;
		ctx->rmax=(ctx->rmax<<8)|255;
		ctx->rval=(ctx->rval<<8)|
			BTLZA_BitArith3_InputByte(ctx);
	}

}

void BTLZA_BitArith3_FlushWBits(BTLZA_Context *ctx)
{
	while((ctx->rmin!=BTLZA_BITARITH_LOWER) ||
		(ctx->rmax!=BTLZA_BITARITH_UPPER))
	{
		BTLZA_BitArith3_OutputByte(ctx, ctx->rmin>>24);
		ctx->rmin<<=8;
		ctx->rmax=(ctx->rmax<<8)|255;
	}
}


void BTLZA_BitArith3_OutputFixedBit(BTLZA_Context *ctx, int i)
{
	u32 r, r2, v;
	int j;

	r=ctx->rmax-ctx->rmin;
	v=ctx->rmin+(r>>1);

	if(i)ctx->rmin=v+1;
	else ctx->rmax=v;
	BTLZA_BitArith3_NormalizeOutput(ctx);
}

int BTLZA_BitArith3_InputFixedBit(BTLZA_Context *ctx)
{
	u32 r, r2, v, i;

	r=ctx->rmax-ctx->rmin;
	v=ctx->rmin+(r>>1);

	i=(ctx->rval>v);
	if(i)ctx->rmin=v+1;
	else ctx->rmax=v;

	BTLZA_BitArith3_NormalizeInput(ctx);
	return(i);
}

void BTLZA_BitArith3_OutputBit(BTLZA_Context *ctx,
	int i, u32 w)
{
	u32 r, r2, v;
	int j;

	r=ctx->rmax-ctx->rmin;
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	if(i)ctx->rmin=v+1;
	else ctx->rmax=v;

	BTLZA_BitArith3_NormalizeOutput(ctx);
}

int BTLZA_BitArith3_InputBit(BTLZA_Context *ctx, u32 w)
{
	u32 r, r2, v, i;

	r=ctx->rmax-ctx->rmin;
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	i=(ctx->rval>v);
	if(i)ctx->rmin=v+1;
	else ctx->rmax=v;

	BTLZA_BitArith3_NormalizeInput(ctx);
	return(i);
}

void BTLZA_BitArith3_OutputModelBit(BTLZA_Context *ctx,
	int i, byte *mctx)
{
	u32 r, v, w, s;
	int j;

	s=*mctx;
	r=ctx->rmax-ctx->rmin;
	w=bgbbtj_bitarith3_divtab8[s];
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	if(i)
	{
		ctx->rmin=v+1;
		*mctx=bgbbtj_bitarith3_transtab8[s|256];
	}else
	{
		ctx->rmax=v;
		*mctx=bgbbtj_bitarith3_transtab8[s];
	}

	BTLZA_BitArith3_NormalizeOutput(ctx);
}

int BTLZA_BitArith3_InputModelBit(BTLZA_Context *ctx, byte *mctx)
{
	u32 r, v, w, s;
	int i, j, k;

	s=*mctx;
	r=ctx->rmax-ctx->rmin;
	w=bgbbtj_bitarith3_divtab8[s];
	v=ctx->rmin+(r>>BTLZA_BITARITH_WBITS)*w;

	i=(ctx->rval>v);
	if(i)
	{
		ctx->rmin=v+1;
		*mctx=bgbbtj_bitarith3_transtab8[s|4096];
	}else
	{
		ctx->rmax=v;
		*mctx=bgbbtj_bitarith3_transtab8[s];
	}

	BTLZA_BitArith3_NormalizeInput(ctx);
	return(i);
}

void BTLZA_BitArith3_OutputFixedBits(BTLZA_Context *ctx,
	int v, int n)
{
	int i, j, k, l;

	i=n;
	while(i--)
		{ BTLZA_BitArith3_OutputFixedBit(ctx, (v>>i)&1); }
}

int BTLZA_BitArith3_InputFixedBits(BTLZA_Context *ctx, int n)
{
	int i, j, k, l;

	i=n; j=0;
	while(i--)
	{
		k=BTLZA_BitArith3_InputFixedBit(ctx);
		j=(j<<1)|k;
	}
	return(j);
}

int BTLZA_BitArith3_InputContextFixedBits(BTLZA_Context *ctx, int n)
{
	int i, j, k, l;

	i=n; j=ctx->wctx;
	while(i--)
	{
		k=BTLZA_BitArith3_InputFixedBit(ctx);
		j=(j<<1)|k;
	}
	ctx->wctx=j;
	return(j&((1<<n)-1));
}

int BTLZA_BitArith3_InputContextFixedBitsLE(BTLZA_Context *ctx, int n)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; l=0;
	while(i--)
	{
		k=BTLZA_BitArith3_InputFixedBit(ctx);
		j=(j<<1)|k;
		l|=k<<i;
	}
	ctx->wctx=j;
	return(l);
}

void BTLZA_BitArith3_OutputContextModelBits(BTLZA_Context *ctx,
	int v, int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; k=ctxmask;
	while(i--)
	{
		l=(v>>i)&1;
		BTLZA_BitArith3_OutputModelBit(ctx, l, mdl+btlza_ar3_ctxindex(j, k));
		j=(j<<1)|l;
	}
	ctx->wctx=j;
}

int BTLZA_BitArith3_InputContextModelBits(BTLZA_Context *ctx,
	int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; k=ctxmask;
	while(i--)
	{
		l=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k));
		j=(j<<1)|l;
	}
	ctx->wctx=j;
	return(j&((1<<n)-1));
}

void BTLZA_BitArith3_OutputExtraBits(BTLZA_Context *ctx,
	int v, int n, byte *mdl, int ctxmask)
{
	switch(n)
	{
	case 0:
		break;
	case 1: case 2:
	case 3: case 4:
		BTLZA_BitArith3_OutputContextModelBits(ctx,
			v, n, mdl, ctxmask);
		break;
	default:
		BTLZA_BitArith3_OutputContextModelBits(ctx,
			v>>(n-4), 4, mdl, ctxmask);
		BTLZA_BitArith3_OutputFixedBits(ctx, v, n-4);
		break;
	}
}

void BTLZA_BitArith3_OutputContextModelBitsLE(BTLZA_Context *ctx,
	int v, int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx;
//	while(i--)
	for(i=0; i<n; i++)
	{
		l=(v>>i)&1;
		BTLZA_BitArith3_OutputModelBit(ctx, l,
			mdl+btlza_ar3_ctxindex(j, ctxmask));
		j=(j<<1)|l;
	}
	ctx->wctx=j;
}

int BTLZA_BitArith3_InputContextModelBitsLE(BTLZA_Context *ctx,
	int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;
	int v;

	i=n; j=ctx->wctx; v=0;
	while(i--)
	{
		l=BTLZA_BitArith3_InputModelBit(ctx, mdl+
			btlza_ar3_ctxindex(j, ctxmask));
		j=(j<<1)|l;
		v|=(l<<i);
	}
	ctx->wctx=j;
	return(v);
}

int BTLZA_BitArith3_InputExtraBits(BTLZA_Context *ctx,
	int n, byte *mdl, int ctxmask)
{
	int i, j;
	int v;

	switch(n)
	{
	case 0: v=0; break;
	case 1: case 2:
	case 3: case 4:
		v=BTLZA_BitArith3_InputContextModelBits(ctx,
			n, mdl, ctxmask);
		break;
	default:
		i=BTLZA_BitArith3_InputContextModelBits(ctx,
			4, mdl, ctxmask);
		j=BTLZA_BitArith3_InputFixedBits(ctx, n-4);
		v=(i<<(n-4))|j;
		break;
	}
	return(v);
}

int BTLZA_BitArith3_SetupEncode(BTLZA_Context *ctx, byte *out, int sz)
{
	ctx->ct=out;
	ctx->cte=out+sz;
	ctx->rmin=BTLZA_BITARITH_LOWER;
	ctx->rmax=BTLZA_BITARITH_UPPER;
	return(0);
}

int BTLZA_BitArith3_SetupDecode(BTLZA_Context *ctx, byte *in, int sz)
{
	int i;

	ctx->cs=in;
	ctx->cse=in+sz;
	ctx->rmin=BTLZA_BITARITH_LOWER;
	ctx->rmax=BTLZA_BITARITH_UPPER;

	ctx->rval=BTLZA_BITARITH_LOWER;
	for(i=0; i<4; i++)
		ctx->rval=(ctx->rval<<8)|BTLZA_BitArith3_InputByte(ctx);
	return(0);
}

int BTLZA_BitArith3_SetupBeginEncode(BTLZA_Context *ctx)
{
	ctx->rmin=BTLZA_BITARITH_LOWER;
	ctx->rmax=BTLZA_BITARITH_UPPER;
	return(0);
}

int BTLZA_BitArith3_SetupBeginDecode(BTLZA_Context *ctx)
{
	int i;

	if(ctx->rmax)
	{
		return(0);
	}

	ctx->rmin=BTLZA_BITARITH_LOWER;
	ctx->rmax=BTLZA_BITARITH_UPPER;
//	ctx->range=BTLZA_BITARITH_UPPER;

	ctx->rval=BTLZA_BITARITH_LOWER;
	for(i=0; i<4; i++)
		ctx->rval=(ctx->rval<<8)|BTLZA_BitArith3_InputByte(ctx);
	return(0);
}

int BTLZA_BitArith3_SetupEndDecode(BTLZA_Context *ctx)
{
	ctx->rmin=0;
	ctx->rmax=0;
	return(0);
}

void BTLZA_BitArith3_EncodeSymbolN(BTLZA_Context *ctx, int v, int n,
	byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; k=ctxmask;
	while(i--)
	{
		l=(v>>i)&1;
		BTLZA_BitArith3_OutputModelBit(ctx, l, mdl+btlza_ar3_ctxindex(j, k));
		j=(j<<1)|l;
	}
	ctx->wctx=j;
}

void BTLZA_BitArith3_EncodeSymbol6(BTLZA_Context *ctx, int v,
	byte *mdl, int ctxmask)
		{ BTLZA_BitArith3_EncodeSymbolN(ctx, v, 6, mdl, ctxmask); }
void BTLZA_BitArith3_EncodeSymbol8(BTLZA_Context *ctx, int v,
	byte *mdl, int ctxmask)
		{ BTLZA_BitArith3_EncodeSymbolN(ctx, v, 8, mdl, ctxmask); }
void BTLZA_BitArith3_EncodeSymbol9(BTLZA_Context *ctx, int v,
	byte *mdl, int ctxmask)
		{ BTLZA_BitArith3_EncodeSymbolN(ctx, v, 9, mdl, ctxmask); }

void BTLZA_BitArith3_EncodeSymbol8LE(BTLZA_Context *ctx, int v,
	byte *mdl, int ctxmask)
{
	BTLZA_BitArith3_EncodeSymbolNLE(ctx, v, 8, mdl, ctxmask);
}

void BTLZA_BitArith3_EncodeSymbolNLE(BTLZA_Context *ctx,
	int v, int n, byte *mdl, int ctxmask)
{
	int i, j, k, l;

	i=n; j=ctx->wctx; k=ctxmask;
	for(i=0; i<n; i++)
	{
		l=(v>>i)&1;
		BTLZA_BitArith3_OutputModelBit(ctx, l, mdl+btlza_ar3_ctxindex(j, k));
		j=(j<<1)|l;
	}
	ctx->wctx=j;
}

int BTLZA_BitArith3_DecodeSymbol6(BTLZA_Context *ctx,
	byte *mdl, int ctxmask)
{
	int i, j, k, v;

	j=ctx->wctx; k=ctxmask;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	ctx->wctx=j;
	return(j&0x3F);
}

int BTLZA_BitArith3_DecodeSymbol8(BTLZA_Context *ctx,
	byte *mdl, int ctxmask)
{
	int i, j, k, v;

	j=ctx->wctx; k=ctxmask;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	ctx->wctx=j;
	return(j&0xFF);
}

int BTLZA_BitArith3_DecodeSymbol9(BTLZA_Context *ctx,
	byte *mdl, int ctxmask)
{
	int i, j, k, v;

	j=ctx->wctx; k=ctxmask;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	ctx->wctx=j;
	return(j&0x1FF);
}

int BTLZA_BitArith3_DecodeSymbol8LE(BTLZA_Context *ctx,
	byte *mdl, int ctxmask)
{
	int i, j, k, v;

	j=ctx->wctx; k=ctxmask;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	i=BTLZA_BitArith3_InputModelBit(ctx, mdl+btlza_ar3_ctxindex(j, k)); j=(j<<1)|i;
	ctx->wctx=j;
	return(bgbbtj_bitarith_transtab8[j&0xFF]);
}

void BTLZA_BitArith3_ResetContextWeights(
	BTLZA_Context *ctx)
{
	int i, j;

	j=1<<ctx->ctxbits;
	for(i=0; i<j; i++)
	{
//		ctx->mdl_lit[i]=1<<(BTLZA_BITARITH_WBITS-1);
		ctx->mdl_lit[i]=0;
	}

	for(i=0; i<4096; i++)
	{
//		ctx->mdl_dist[i]=1<<(BTLZA_BITARITH_WBITS-1);
//		ctx->mdl_xbits[i]=1<<(BTLZA_BITARITH_WBITS-1);
		ctx->mdl_dist[i]=0;
		ctx->mdl_xbits[i]=0;
	}

	ctx->wctx=0;
}

void BTLZA_BitArith3_SetupContextBits(
	BTLZA_Context *ctx, int bits)
{
	int i, j;

	BTLZA_BitArith3_Init();

	if(ctx->mdl_lit && (ctx->ctxbits==bits))
	{
		BTLZA_BitArith3_ResetContextWeights(ctx);
		return;
	}

	if(ctx->mdl_lit && (ctx->ctxbits!=bits))
		{ frgl_free(ctx->mdl_lit); ctx->mdl_lit=NULL; }
	
	j=1<<bits;
	if(!ctx->mdl_lit)
		{ ctx->mdl_lit=frgl_malloc(j*sizeof(byte)); }
	ctx->ctxbits=bits;
	ctx->ctxmask=(1<<bits)-1;
	ctx->wctx=0;

	j=1<<12;
	if(!ctx->mdl_dist)
		{ ctx->mdl_dist=frgl_malloc(j*sizeof(byte)); }
	if(!ctx->mdl_xbits)
		{ ctx->mdl_xbits=frgl_malloc(j*sizeof(byte)); }

	BTLZA_BitArith3_ResetContextWeights(ctx);
}


void BTLZA_BitArith3_ResetRawContextWeights(
	BTLZA_Context *ctx)
{
	int i, j;

	j=1<<ctx->ctxbits_raw;
	for(i=0; i<j; i++)ctx->mdl_rbits[i]=1<<(BTLZA_BITARITH_WBITS-1);

	ctx->wctx=0;
}

void BTLZA_BitArith3_SetupRawContextBits(
	BTLZA_Context *ctx, int bits)
{
	int i, j;

	BTLZA_BitArith3_Init();

	if(ctx->mdl_rbits && (ctx->ctxbits_raw==bits))
	{
		BTLZA_BitArith3_ResetRawContextWeights(ctx);
		return;
	}

	if(ctx->mdl_rbits && (ctx->ctxbits_raw!=bits))
		{ frgl_free(ctx->mdl_rbits); ctx->mdl_rbits=NULL; }
	
	j=1<<bits;
	if(!ctx->mdl_rbits)
		{ ctx->mdl_rbits=frgl_malloc(j*sizeof(byte)); }
	ctx->ctxbits_raw=bits;
	ctx->ctxmask_raw=(1<<bits)-1;
	ctx->wctx=0;

	BTLZA_BitArith3_ResetRawContextWeights(ctx);
}

BTEIFGL_API int BTLZA_BitArith3_EncodeDataCtx(
	BTLZA_Context *ctx,
	byte *ibuf, int isz, byte *obuf, int osz, int flags)
{
	int i, j, k, l, ll, le;
	byte *cs, *ct, *cse, *cte, *s2;

	if(isz<0)return(-1);
	if(isz<=0)return(0);

	BTLZA_BitArith3_Init();

	BTLZA_BitArith3_SetupContextBits(ctx,
		BTLZA_BITARITH_CTXBITS);

	ct=obuf; cte=obuf+osz;
	le=flags&32; ll=flags&31;

	le=1;

	//Emit Header
	*ct++=0xBA;		//magic byte
	ct=BTIC4B_EncEmitUVLI(ct, ((ll-12)&15)|(le?16:0));
	ct=BTIC4B_EncEmitUVLI(ct, isz);

	BTLZA_BitArith3_SetupEncode(ctx, ct, cte-ct);

	cs=ibuf;
	cse=ibuf+isz;
	i=0;

	while(cs<cse)
	{
		BTLZA_BitArith3_EncodeSymbol8(ctx, *cs++,
			ctx->mdl_lit, ctx->ctxmask);
	}
	BTLZA_BitArith3_FlushWBits(ctx);

	i=ctx->ct-obuf;
	return(i);
}

BTEIFGL_API int BTLZA_BitArith3_DecodeDataCtx(
	BTLZA_Context *ctx,
	byte *ibuf, int isz, byte *obuf, int osz)
{
	int i, j, k, l, fl, ll, osz1, le;
	s64 li;
	byte *cs, *ct, *cse, *cte, *s2;

	if(isz<=0)return(0);

	BTLZA_BitArith3_Init();

	cs=ibuf; cse=ibuf+isz;

	if(*cs!=0xBA)
		return(-1);
	cs++;

	cs=BTIC4B_BufReadUVLI(cs, &li);
	fl=li;

	if(fl&(~31))
		return(-1);

	BTLZA_BitArith3_SetupContextBits(ctx, 12+(fl&15));
	le=(fl&16)?1:0;

//	cs=BGBBTJ_DecodeVLI(cs, &osz1);
	cs=BTIC4B_BufReadUVLI(cs, &li);
	osz1=li;
	if(osz1>osz)return(-1);

	BTLZA_BitArith3_SetupDecode(ctx, cs, cse-cs);

	ct=obuf;
	cte=obuf+osz1;
	i=0;

	while(ct<cte)
	{
		*ct++=BTLZA_BitArith3_DecodeSymbol8(ctx,
			ctx->mdl_lit, ctx->ctxmask);
	}
	i=ct-obuf;
	return(i);
}

BTEIFGL_API int BTLZA_BitArith3_EncodeData(
	byte *ibuf, int isz, byte *obuf, int osz)
{
	BTLZA_Context *ctx;
	int i, j;
	
//	ctx=frgl_malloc(sizeof(BTLZA_Context));
//	ctx->cs=NULL; ctx->ct=NULL;
	ctx=BTLZA_AllocContext();
	i=BTLZA_BitArith3_EncodeDataCtx(ctx, ibuf, isz, obuf, osz, 13);
	BTLZA_FreeContext(ctx);
	return(i);
}

BTEIFGL_API int BTLZA_BitArith3_DecodeData(
	byte *ibuf, int isz, byte *obuf, int osz)
{
	BTLZA_Context *ctx;
	int i, j;
	
	ctx=BTLZA_AllocContext();
	i=BTLZA_BitArith3_DecodeDataCtx(ctx, ibuf, isz, obuf, osz);
	BTLZA_FreeContext(ctx);
	return(i);
}

BTEIFGL_API int BTLZA_BitArith3_EncodeTestData(
	byte *ibuf, int isz, byte *obuf, int osz)
{
	BTLZA_Context *ctx;
	byte *tbuf;
	int i, j, k;

	if(isz<0)return(-1);
	if(isz<=0)return(0);
	
	ctx=BTLZA_AllocContext();
	i=BTLZA_BitArith3_EncodeDataCtx(ctx, ibuf, isz, obuf, osz, 13);

	tbuf=frgl_malloc(2*isz);
	j=BTLZA_BitArith3_DecodeDataCtx(ctx, obuf, i, tbuf, 2*isz);
	
	if(j!=isz)
	{
		printf("BTLZA_BitArith3_EncodeTestData: Size Check %d->%d\n",
			isz, j);
	}
	
	for(k=0; k<j; k++)
	{
		if(ibuf[k]!=tbuf[k])
		{
			printf("BTLZA_BitArith3_EncodeTestData: "
				"Decode Fail At %d\n", k);
			break;
		}
	}

	if(k>=j)
	{
//		printf("BTLZA_BitArith3_EncodeTestData: OK\n");
	}else
	{
		printf("BTLZA_BitArith3_EncodeTestData: Fail\n");
		i=-1;
	}

	frgl_free(tbuf);

	BTLZA_FreeContext(ctx);
	
	return(i);
}
