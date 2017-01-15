// #include <btlzazip.h>

// #if defined(WIN64) || (_M_IX86_FP>=2)
// #include <xmmintrin.h>
// #include <emmintrin.h>
// #define HAVE_SSE2
// #endif

#ifdef __EMSCRIPTEN__
#define BTLZA_NOBASIC2
#endif


int BTLZA_BitDec_TransposeByte(int v)
{
	return(btlza_trans8[v]);
}

int BTLZA_BitDec_TransposeWord(int v)
{
	int i;
	i=btlza_trans8[(v>>8)&0xFF]|(btlza_trans8[v&0xFF]<<8);
	return(i);
}

int BTLZA_BitDec_DecodeSymbolPartBits(
	int bits, int nbits,
	u16 *tc, u16 *tm, byte *tl,
	u32 *ti, u16 *tn)
{
	int i, j;

	i=bits;
	j=ti[i&0xFF]&511;

	if(((i&tm[j])==tc[j]) && (tl[j]<=nbits))
		{ return(j); }

	while((i&tm[j])!=tc[j])
		j=tn[j];
	if(((i&tm[j])==tc[j]) && (tl[j]<=nbits))
		{ return(j); }

	return(-1);
}

int BTLZA_BitDec_SetupTable(byte *cl, int ncl,
	u16 *tc, u16 *tm, byte *tl,
	u32 *ti, u16 *tn)
{
	return(BTLZA_BitDec_SetupTableI(cl, ncl, tc, tm, tl, ti, tn, 1));
}

int BTLZA_BitDec_SetupTableI(byte *cl, int ncl,
	u16 *tc, u16 *tm, byte *tl,
	u32 *ti, u16 *tn, int flag)
{
	int cnt[16], nc[16], nn[16], nf[16];
	int i, j, k, k2, l, ff;

	for(i=0; i<16; i++) { cnt[i]=0; nc[i]=0; nn[i]=-1; nf[i]=-1; }
	for(i=0; i<256; i++)ti[i]=0xFFFF;

	for(i=0; i<ncl; i++)cnt[cl[i]]++;
	cnt[0]=0;

#if 1
	j=cl[0];
	for(i=15; i>=1; i--)
		if(cnt[i])break;
	if(j>i)j=i;
	if(!i)return(-12);	//no codes

	j=1;
	for(i=1; i<16; i++)
	{
		j<<=1;
		j-=cnt[i];

		if(j<0)return(-10);	//over subscribed
	}
	if((j>0) && ((ncl-cnt[0])!=1))
		return(-11);	//incomplete set
#endif

	j=0;
	for(i=1; i<16; i++)
	{
		j=(j+cnt[i-1])<<1;
		nc[i]=j;
	}

	for(i=0; i<ncl; i++)
	{
		l=cl[i];
		if(!l)
		{
			tl[i]=cl[i];
			continue;
		}

		tl[i]=cl[i];
		tm[i]=(1<<tl[i])-1;
		tc[i]=nc[l]++;

		if(nn[l]>=0)
		{
			tn[nn[l]]=i;
			nn[l]=i;
		}else
		{
			nf[l]=i;
			nn[l]=i;
		}
	}

	j=-1; ff=-1;
	for(i=1; i<16; i++)
	{
		if(nf[i]>=0)
		{
			if(ff<0)ff=nf[i];
			if(j>=0)tn[j]=nf[i];
			j=nn[i];
		}
	}
	if(j>=0)tn[j]=0xFFFF;

	for(i=ff; i!=0xFFFF; i=tn[i])
	{
		l=cl[i];
		if(l<=8)
		{
			k2=tc[i]<<(8-l);
			j=1<<(8-l);
			while(j--)
			{
				k=BTLZA_BitDec_TransposeByte(k2++);
				ti[k]=i;
			}
		}else
		{
			k2=tc[i]>>(l-8);
			k=BTLZA_BitDec_TransposeByte(k2);
			if(ti[k]==0xFFFF)
				ti[k]=i;
		}

		k2=(tc[i])<<(16-l);
		k=BTLZA_BitDec_TransposeWord(k2++);
		tc[i]=k;
	}

	for(i=0; i<256; i++)
		if(ti[i]==0xFFFF)
	{
		printf("BTLZA_BitDec_SetupTable: Table bad index %d\n", i);
		return(-9);
	}

//	if(flag&1)
		for(i=0; i<256; i++)
	{
		j=ti[i];
		l=tl[j];
		ti[i]|=(l&15)<<24;
	}

	return(0);
}

int BTLZA_BitDec_SetupStatic(BTLZA_Context *ctx)
{
	byte lcl[288], dcl[32];
	int hl, hd;
	int i, j;

	hl=288;
	for(i=0; i<144; i++)lcl[i]=8;
	for(i=144; i<256; i++)lcl[i]=9;
	for(i=256; i<280; i++)lcl[i]=7;
	for(i=280; i<288; i++)lcl[i]=8;

	hd=32;
	for(i=0; i<32; i++)dcl[i]=5;

	j=BTLZA_BitDec_SetupTable(lcl, hl,
		ctx->bs_ltab_code, ctx->bs_ltab_mask, ctx->bs_ltab_len,
		ctx->bs_ltab_idx, ctx->bs_ltab_next);
	if(j<0)
	{
		printf("Static Literal/Length Table Problem %d\n", j);
		return(j);
	}

	j=BTLZA_BitDec_SetupTable(dcl, hd,
		ctx->bs_dtab_code, ctx->bs_dtab_mask, ctx->bs_dtab_len,
		ctx->bs_dtab_idx, ctx->bs_dtab_next);
	if(j<0)
	{
		printf("Static Distance Table Problem %d\n", j);
		return(j);
	}

	return(0);
}

int BTLZA_BitDec_DecodeSymbol(BTLZA_Context *ctx)
	{ return(ctx->BS_DecodeSymbol(ctx)); }
int BTLZA_BitDec_DecodeDistanceSymbol(BTLZA_Context *ctx)
	{ return(ctx->BS_DecodeDistanceSymbol(ctx)); }
int BTLZA_BitDec_DecodeClSymbol(BTLZA_Context *ctx)
	{ return(ctx->BS_DecodeClSymbol(ctx)); }

int BTLZA_BitDec_DecodeSymbolBasic(BTLZA_Context *ctx)
{
	int i, j;

	i=BTLZA_BitDec_PeekWord(ctx);
	j=ctx->bs_ltab_idx[i&0xFF]&511;

	if((i&ctx->bs_ltab_mask[j])==ctx->bs_ltab_code[j])
	{
		BTLZA_BitDec_SkipNBits(ctx, ctx->bs_ltab_len[j]);
		return(j);
	}else
	{
		j=ctx->bs_ltab_next[j];
	}

	while((i&ctx->bs_ltab_mask[j])!=ctx->bs_ltab_code[j])
		j=ctx->bs_ltab_next[j];

	BTLZA_BitDec_SkipNBits(ctx, ctx->bs_ltab_len[j]);
	return(j);
}

int BTLZA_BitDec_DecodeDistanceSymbolBasic(BTLZA_Context *ctx)
{
	int i, j;

	i=BTLZA_BitDec_PeekWord(ctx);
	j=ctx->bs_dtab_idx[i&0xFF]&255;

	if((i&ctx->bs_dtab_mask[j])==ctx->bs_dtab_code[j])
	{
		BTLZA_BitDec_SkipNBits(ctx, ctx->bs_dtab_len[j]);
		return(j);
	}else
	{
		j=ctx->bs_dtab_next[j];
	}

	while((i&ctx->bs_dtab_mask[j])!=ctx->bs_dtab_code[j])
		j=ctx->bs_dtab_next[j];

	BTLZA_BitDec_SkipNBits(ctx, ctx->bs_dtab_len[j]);
	return(j);
}

#ifndef BTLZA_NOBASIC2
// force_inline
int BTLZA_BitDec_ReadNBitsBasic2(
	BTLZA_Context *ctx, int n)
{
	int i, j, k;

	i=(ctx->bs_win>>ctx->bs_pos)&((1<<n)-1);
	j=ctx->bs_pos+n;

// #if defined(X86) || defined(X86_64)
#if 1
	k=j&(~7);
//	ctx->bs_win=(ctx->bs_win>>k)|(((*(u32 *)ctx->cs)<<(24-k))<<8);
//	ctx->bs_win=(ctx->bs_win>>k)|(((*(u32 *)ctx->cs)<<(31-k))<<1);
	ctx->bs_win=(ctx->bs_win>>k)|((btlza_getu32le(ctx->cs)<<(31-k))<<1);
//	ctx->bs_pos=j-k;
	ctx->bs_pos=j&7;
	ctx->cs+=j>>3;
#endif

	return(i);
}

// force_inline
void BTLZA_BitDec_SkipNBitsBasic2(
	BTLZA_Context *ctx, int n)
{
	int i, j, k;

// #if defined(X86) || defined(X86_64)
#if 1
// #if 0
	i=ctx->bs_pos+n;
	k=i&(~7);
//	ctx->bs_win=(ctx->bs_win>>k)|(((*(u32 *)ctx->cs)<<(24-k))<<8);
//	ctx->bs_win=(ctx->bs_win>>k)|(((*(u32 *)ctx->cs)<<(31-k))<<1);
	ctx->bs_win=(ctx->bs_win>>k)|((btlza_getu32le(ctx->cs)<<(31-k))<<1);
//	ctx->bs_pos=i-k;
	ctx->bs_pos=i&7;
	ctx->cs+=i>>3;
#endif
}

// force_inline
int BTLZA_BitDec_DecodeSymbolBasic2(
	BTLZA_Context *ctx)
{
	int i, j, k, j2, l, l2;

	i=(ctx->bs_win>>ctx->bs_pos);
	k=ctx->bs_ltab_idx[i&0xFF];
	j=k&511;
	l=(k>>24)&15;

	if(l<=8)
	{
		BTLZA_BitDec_SkipNBitsBasic2(ctx, l);
		return(j);
	}

	while((i&ctx->bs_ltab_mask[j])!=ctx->bs_ltab_code[j])
		j=ctx->bs_ltab_next[j];

	BTLZA_BitDec_SkipNBitsBasic2(ctx, ctx->bs_ltab_len[j]);
	return(j);
}

// force_inline 
int BTLZA_BitDec_DecodeDistanceSymbolBasic2(
	BTLZA_Context *ctx)
{
	int i, j, k, l;

	i=ctx->bs_win>>ctx->bs_pos;
	k=ctx->bs_dtab_idx[i&0xFF];
	j=k&255;
	l=(k>>24)&15;

	if(l<=8)
	{
		BTLZA_BitDec_SkipNBitsBasic2(ctx, l);
//		BTLZA_BitDec_SkipNBitsL8Basic2(ctx, l);
		return(j);
	}

	while((i&ctx->bs_dtab_mask[j])!=ctx->bs_dtab_code[j])
		j=ctx->bs_dtab_next[j];

	BTLZA_BitDec_SkipNBitsBasic2(ctx, ctx->bs_dtab_len[j]);
	return(j);
}
#endif

#if 1
int BTLZA_BitDec_DecodeSymbolRingHuff(BTLZA_Context *ctx)
{
	int rh;
	int i, j, k;

	rh=(ctx->bs_rhtab_lrov++)&7;

//	i=BTLZA_BitDec_PeekWord(ctx);
	i=(ctx->bs_win>>ctx->bs_pos)&65535;
	j=ctx->bs_ltab_idx2[rh][i&0xFF]&511;

	if((i&ctx->bs_ltab_mask2[rh][j])==ctx->bs_ltab_code2[rh][j])
	{
		BTLZA_BitDec_SkipNBitsBasic(ctx, ctx->bs_ltab_len2[rh][j]);
		return(j);
	}else
	{
		j=ctx->bs_ltab_next2[rh][j];
	}

	while((i&ctx->bs_ltab_mask2[rh][j])!=ctx->bs_ltab_code2[rh][j])
		j=ctx->bs_ltab_next2[rh][j];

	BTLZA_BitDec_SkipNBitsBasic(ctx, ctx->bs_ltab_len2[rh][j]);
	return(j);
}

int BTLZA_BitDec_DecodeDistanceSymbolRingHuff(BTLZA_Context *ctx)
{
	int rh;
	int i, j;

	rh=(ctx->bs_rhtab_drov++)&7;

//	i=BTLZA_BitDec_PeekWord(ctx);
	i=(ctx->bs_win>>ctx->bs_pos)&65535;
	j=ctx->bs_dtab_idx2[rh][i&0xFF]&255;

	if((i&ctx->bs_dtab_mask2[rh][j])==ctx->bs_dtab_code2[rh][j])
	{
		BTLZA_BitDec_SkipNBitsBasic(ctx, ctx->bs_dtab_len2[rh][j]);
		return(j);
	}else
	{
		j=ctx->bs_dtab_next2[rh][j];
	}

	while((i&ctx->bs_dtab_mask2[rh][j])!=ctx->bs_dtab_code2[rh][j])
		j=ctx->bs_dtab_next2[rh][j];

	BTLZA_BitDec_SkipNBitsBasic(ctx, ctx->bs_dtab_len2[rh][j]);
	return(j);
}
#endif

int BTLZA_BitDec_DecodeSymbolReadBit(BTLZA_Context *ctx)
{
	int i, j, k, l;

	i=0;
	for(l=1; l<16; l++)
	{
		i|=BTLZA_BitDec_ReadLiteralBit(ctx)<<(l-1);
		j=ctx->bs_ltab_idx[i&0xFF];
		while(j!=0xFFFF)
		{
			if(ctx->bs_ltab_len[j]>l)
				{ j=ctx->bs_ltab_next[j]; continue; }
			if((i&ctx->bs_ltab_mask[j])!=ctx->bs_ltab_code[j])
				{ j=ctx->bs_ltab_next[j]; continue; }
			break;
		}		
		if(j!=0xFFFF)
			break;
	}
	return(j);
}

int BTLZA_BitDec_DecodeDistanceSymbolReadBit(BTLZA_Context *ctx)
{
	int i, j, k, l;

	i=0;
	for(l=1; l<16; l++)
	{
		i|=BTLZA_BitDec_ReadDistanceBit(ctx)<<(l-1);
		j=ctx->bs_dtab_idx[i&0xFF];
		while(j!=0xFFFF)
		{
			if(ctx->bs_dtab_len[j]>l)
				{ j=ctx->bs_dtab_next[j]; continue; }
			if((i&ctx->bs_dtab_mask[j])!=ctx->bs_dtab_code[j])
				{ j=ctx->bs_dtab_next[j]; continue; }
			break;
		}		
		if(j!=0xFFFF)
			break;
	}
	return(j);
}

int BTLZA_BitDec_DecodeDistance(BTLZA_Context *ctx)
{

	int i, j, k;
	j=BTLZA_BitDec_DecodeDistanceSymbol(ctx);
	k=btlza_dbase1[j]+BTLZA_BitDec_ReadExtraNBits(ctx, btlza_dextra1[j]);
	return(k);
}

#ifndef BTLZA_NOBASIC2
// force_inline
int BTLZA_BitDec_DecodeDistanceBasic2(BTLZA_Context *ctx)
{
	int i, j, k;
	j=BTLZA_BitDec_DecodeDistanceSymbolBasic2(ctx);
	k=btlza_dbase1[j]+BTLZA_BitDec_ReadNBitsBasic2(ctx, btlza_dextra1[j]);
	return(k);
}
#endif

int BTLZA_BitDec_DecodeCodeLengths(BTLZA_Context *ctx,
	byte *cl, int ncl)
{
	int i, j, k;

	i=0; k=0;
	while(i<ncl)
	{
		j=BTLZA_BitDec_DecodeClSymbol(ctx);
		if(j<0)return(j);

		if(j<16)
		{
			k=j;
			cl[i++]=j;
			continue;
		}

		if(j==16)
		{
			j=BTLZA_BitDec_Read2Bits(ctx)+3;
			while(j--)cl[i++]=k;
			continue;
		}

		if(j==17)
		{
			j=BTLZA_BitDec_Read3Bits(ctx)+3;
			while(j--)cl[i++]=0;
			k=0;
			continue;
		}
		if(j==18)
		{
			j=BTLZA_BitDec_ReadNBits(ctx, 7)+11;
			while(j--)cl[i++]=0;
			k=0;
			continue;
		}

		if(j==19)
		{
			j=BTLZA_BitDec_ReadBit(ctx)*2-1;
			k=k+j;
			cl[i++]=k;
			continue;
		}

		return(-7);
	}
	return(0);
}

int BTLZA_BitDec_DecodeCodeLengthsRh(BTLZA_Context *ctx,
	byte *cl, byte *lcl, int ncl)
{
	int i, j, k;

	i=0; k=0;
	while(i<ncl)
	{
		j=BTLZA_BitDec_DecodeClSymbol(ctx);
		if(j<0)return(j);

		if(j<16)
		{
			k=j;
			cl[i++]=j;
			continue;
		}

		if(j==16)
		{
			j=BTLZA_BitDec_Read2Bits(ctx)+3;
			while(j--)cl[i++]=k;
			continue;
		}

		if(j==17)
		{
			j=BTLZA_BitDec_Read3Bits(ctx)+3;
			while(j--)
				{ k=lcl[i]; cl[i++]=k; }
			continue;
		}
		if(j==18)
		{
			j=BTLZA_BitDec_ReadNBits(ctx, 7)+11;
			while(j--)
				{ k=lcl[i]; cl[i++]=k; }
			continue;
		}

		if(j==19)
		{
			j=BTLZA_BitDec_ReadBit(ctx)*2-1;
			k=lcl[i]+j; cl[i++]=k;
			continue;
		}

		return(-7);
	}
	return(0);
}

int BTLZA_BitDec_DecodeHeader(BTLZA_Context *ctx)
{
	static int lorder[]={
		16, 17, 18, 0, 8,7, 9,6, 10,5, 11,4, 12,3, 13,2, 14,1, 15, 19};

	byte hcl[32], lcl[512], dcl[32];
	int hl, hd, hc;
	int i, j;

	hl=BTLZA_BitDec_ReadNBits(ctx, 5)+257;
	hd=BTLZA_BitDec_ReadNBits(ctx, 5)+1;
	hc=BTLZA_BitDec_ReadNBits(ctx, 4)+4;
	ctx->bs_rhtab_n=0;	/* Narrow header can't use RingHuff */

//	printf("%d %d %d\n", hl, hd, hc);

	for(i=0; i<32; i++)hcl[i]=0;
	for(i=0; i<hc; i++)
		hcl[lorder[i]]=BTLZA_BitDec_ReadNBits(ctx, 3);

//	for(i=0; i<20; i++)printf("%d:%d ", i, hcl[i]);
//	printf("\n");

	j=BTLZA_BitDec_SetupTable(hcl, 32,
		ctx->bs_ltab_code, ctx->bs_ltab_mask, ctx->bs_ltab_len,
		ctx->bs_ltab_idx, ctx->bs_ltab_next);
	if(j<0)
	{
		printf("Codes Table Problem %d\n", j);
		for(i=0; i<20; i++)printf("%d:%d ", i, hcl[i]);
		printf("\n");
		return(j);
	}

	i=BTLZA_BitDec_DecodeCodeLengths(ctx, lcl, hl);
	if(i<0)return(i);
	i=BTLZA_BitDec_DecodeCodeLengths(ctx, dcl, hd);
	if(i<0)return(i);

	j=BTLZA_BitDec_SetupTable(lcl, hl,
		ctx->bs_ltab_code, ctx->bs_ltab_mask, ctx->bs_ltab_len,
		ctx->bs_ltab_idx, ctx->bs_ltab_next);
	if(j<0)
	{
		printf("Literal/Length Table Problem %d\n", j);
		for(i=0; i<hl; i++)printf("%d:%d ", i, lcl[i]);
		printf("\n");
		return(j);
	}

	j=BTLZA_BitDec_SetupTable(dcl, hd,
		ctx->bs_dtab_code, ctx->bs_dtab_mask, ctx->bs_dtab_len,
		ctx->bs_dtab_idx, ctx->bs_dtab_next);
	if(j<0)
	{
		printf("Distance Table Problem %d\n", j);
		for(i=0; i<hd; i++)printf("%d:%d ", i, dcl[i]);
		printf("\n");
		return(j);
	}

	return(0);
}

int BTLZA_BitDec_CheckSetupRingHuffTables(
	BTLZA_Context *ctx, int hnrh)
{
	int i, j, k;

	if(hnrh>1)
	{
		if(hnrh>ctx->bs_rhtab_sz)
		{
			if(ctx->bs_ltab_rhuff)
			{
				free(ctx->bs_ltab_rhuff);
				free(ctx->bs_dtab_rhuff);
				ctx->bs_ltab_rhuff=NULL;
				ctx->bs_dtab_rhuff=NULL;
			}
		}
		
		if(!ctx->bs_ltab_rhuff)
		{
			i=hnrh*(384*8)+(hnrh*1024);
			j=hnrh*(64*8)+(hnrh*1024);
			ctx->bs_ltab_rhuff=malloc(i);
			ctx->bs_dtab_rhuff=malloc(j);
			ctx->bs_rhtab_sz=hnrh;
		}
		
		for(i=0; i<hnrh; i++)
		{
			ctx->bs_ltab_idx2[i]=(u32 *)(ctx->bs_ltab_rhuff+(i*1024));
			ctx->bs_dtab_idx2[i]=(u32 *)(ctx->bs_dtab_rhuff+(i*1024));

			ctx->bs_ltab_code2[i]=(u16 *)(ctx->bs_ltab_rhuff+
				(hnrh*1024)+(i*384*8)+(0*384));
			ctx->bs_ltab_mask2[i]=(u16 *)(ctx->bs_ltab_rhuff+
				(hnrh*1024)+(i*384*8)+(2*384));
			ctx->bs_ltab_next2[i]=(u16 *)(ctx->bs_ltab_rhuff+
				(hnrh*1024)+(i*384*8)+(4*384));
			ctx->bs_ltab_len2[i]=(byte *)(ctx->bs_ltab_rhuff+
				(hnrh*1024)+(i*384*8)+(6*384));

			ctx->bs_dtab_code2[i]=(u16 *)(ctx->bs_dtab_rhuff+
				(hnrh*1024)+(i*64*8)+(0*64));
			ctx->bs_dtab_mask2[i]=(u16 *)(ctx->bs_dtab_rhuff+
				(hnrh*1024)+(i*64*8)+(2*64));
			ctx->bs_dtab_next2[i]=(u16 *)(ctx->bs_dtab_rhuff+
				(hnrh*1024)+(i*64*8)+(4*64));
			ctx->bs_dtab_len2[i]=(byte *)(ctx->bs_dtab_rhuff+
				(hnrh*1024)+(i*64*8)+(6*64));
		}
		for(; i<8; i++)
		{
			k=i&(hnrh-1);
			ctx->bs_ltab_idx2[i]=ctx->bs_ltab_idx2[k];
			ctx->bs_dtab_idx2[i]=ctx->bs_dtab_idx2[k];
			ctx->bs_ltab_code2[i]=ctx->bs_ltab_code2[k];
			ctx->bs_ltab_mask2[i]=ctx->bs_ltab_mask2[k];
			ctx->bs_ltab_next2[i]=ctx->bs_ltab_next2[k];
			ctx->bs_ltab_len2[i]=ctx->bs_ltab_len2[k];
			ctx->bs_dtab_code2[i]=ctx->bs_dtab_code2[k];
			ctx->bs_dtab_mask2[i]=ctx->bs_dtab_mask2[k];
			ctx->bs_dtab_next2[i]=ctx->bs_dtab_next2[k];
			ctx->bs_dtab_len2[i]=ctx->bs_dtab_len2[k];
		}
	}else
	{
		for(i=0; i<8; i++)
		{
			ctx->bs_ltab_idx2[i]=ctx->bs_ltab_idx;
			ctx->bs_dtab_idx2[i]=ctx->bs_dtab_idx;
			ctx->bs_ltab_code2[i]=ctx->bs_ltab_code;
			ctx->bs_ltab_mask2[i]=ctx->bs_ltab_mask;
			ctx->bs_ltab_next2[i]=ctx->bs_ltab_next;
			ctx->bs_ltab_len2[i]=ctx->bs_ltab_len;
			ctx->bs_dtab_code2[i]=ctx->bs_dtab_code;
			ctx->bs_dtab_mask2[i]=ctx->bs_dtab_mask;
			ctx->bs_dtab_next2[i]=ctx->bs_dtab_next;
			ctx->bs_dtab_len2[i]=ctx->bs_dtab_len;
		}
	}
	ctx->bs_rhtab_n=hnrh;
	return(0);
}

int BTLZA_BitDec_DecodeHeaderBTLZH(BTLZA_Context *ctx)
{
	static int lorder[]={
		16, 17, 18, 0, 8,7, 9,6, 10,5, 11,4, 12,3, 13,2, 14,1, 15, 19,
		20, 21, 22, 23, 24, 25, 26, 27};
	byte hcl[64], lcl[384*8], dcl[64*8];
	int hl, hd, hc, hr, hnrh;
	int i, j, k;

	hr=BTLZA_BitDec_ReadNBits(ctx, 3);
	if(hr!=0)
	{
		if(hr&4)
			return(-1);
		hnrh=1<<(hr&3);
		ctx->bs_rhtab_lrov=0;
		ctx->bs_rhtab_drov=0;
	}else
	{
		hnrh=0;
	}
	
	hl=BTLZA_BitDec_ReadNBits(ctx, 7)+257;
	hd=BTLZA_BitDec_ReadNBits(ctx, 6)+1;
	hc=BTLZA_BitDec_ReadNBits(ctx, 5)+4;

	BTLZA_BitDec_CheckSetupRingHuffTables(ctx, hnrh);

	for(i=0; i<64; i++)hcl[i]=0;
	for(i=0; i<hc; i++)
		hcl[lorder[i]]=BTLZA_BitDec_ReadNBits(ctx, 3);

	j=BTLZA_BitDec_SetupTable(hcl, 64,
		ctx->bs_ltab_code, ctx->bs_ltab_mask, ctx->bs_ltab_len,
		ctx->bs_ltab_idx, ctx->bs_ltab_next);
	if(j<0)
	{
		printf("LZH Codes Table Problem %d\n", j);
		for(i=0; i<24; i++)printf("%d:%d ", i, hcl[i]);
		printf("\n");
		return(j);
	}

	i=BTLZA_BitDec_DecodeCodeLengths(ctx, lcl, hl);
	if(i<0)return(i);
	i=BTLZA_BitDec_DecodeCodeLengths(ctx, dcl, hd);
	if(i<0)return(i);

	if(hnrh>1)
	{
		for(j=1; j<hnrh; j++)
		{
			i=BTLZA_BitDec_DecodeCodeLengthsRh(ctx,
				lcl+(j*384), lcl+((j-1)*384), hl);
			if(i<0)return(i);
			i=BTLZA_BitDec_DecodeCodeLengthsRh(ctx,
				dcl+(j*64), dcl+((j-1)*64), hd);
			if(i<0)return(i);
		}

		for(j=0; j<hnrh; j++)
		{
			i=BTLZA_BitDec_SetupTable(lcl+(j*384), hl,
				ctx->bs_ltab_code2[j], ctx->bs_ltab_mask2[j],
				ctx->bs_ltab_len2[j],
				ctx->bs_ltab_idx2[j], ctx->bs_ltab_next2[j]);
			if(i<0)
				return(i);

			i=BTLZA_BitDec_SetupTable(dcl+(j*64), hd,
				ctx->bs_dtab_code2[j], ctx->bs_dtab_mask2[j],
				ctx->bs_dtab_len2[j],
				ctx->bs_dtab_idx2[j], ctx->bs_dtab_next2[j]);
			if(i<0)
				return(i);
		}
	}else
	{
		j=BTLZA_BitDec_SetupTable(lcl, hl,
			ctx->bs_ltab_code, ctx->bs_ltab_mask, ctx->bs_ltab_len,
			ctx->bs_ltab_idx, ctx->bs_ltab_next);
		if(j<0)
		{
			printf("Literal/Length Table Problem %d\n", j);
			for(i=0; i<hl; i++)printf("%d:%d ", i, lcl[i]);
			printf("\n");
			return(j);
		}

		j=BTLZA_BitDec_SetupTable(dcl, hd,
			ctx->bs_dtab_code, ctx->bs_dtab_mask, ctx->bs_dtab_len,
			ctx->bs_dtab_idx, ctx->bs_dtab_next);
		if(j<0)
		{
			printf("Distance Table Problem %d\n", j);
			for(i=0; i<hd; i++)printf("%d:%d ", i, dcl[i]);
			printf("\n");
			return(j);
		}
	}

	return(0);
}

int BTLZA_BitDec_DecodeStaticHeader2(BTLZA_Context *ctx)
{
	byte lcl[384], dcl[64];
	int hl, hd, tl, td;
	int i, j;

	tl=BTLZA_BitDec_ReadNBits(ctx, 3);
	td=BTLZA_BitDec_ReadNBits(ctx, 2);

	switch(tl)
	{
	case 0:
		hl=288;
		for(i=0; i<144; i++)lcl[i]=8;
		for(i=144; i<256; i++)lcl[i]=9;
		for(i=256; i<280; i++)lcl[i]=7;
		for(i=280; i<288; i++)lcl[i]=8;
		break;
	case 1:
		hl=384;
		for(i=0; i<127; i++)lcl[i]=8;
		for(i=128; i<384; i++)lcl[i]=9;
		break;
	case 2:
		hl=384;
		for(i=0; i<255; i++)lcl[i]=9;
		for(i=256; i<384; i++)lcl[i]=8;
		break;
	case 7:
		return(0);
		break;
	default:
		return(-1);
		break;
	}

	switch(td)
	{
	case 0:
		hd=64;
		for(i=0; i<64; i++)dcl[i]=6;
		break;
	case 1:
		hd=32;
		for(i=0; i<32; i++)dcl[i]=5;
		break;
	case 2:
		hd=16;
		for(i=0; i<16; i++)dcl[i]=4;
		break;
	case 3:
		hd=8;
		for(i=0; i<8; i++)dcl[i]=3;
		break;
	default:
		break;
	}

	j=BTLZA_BitDec_SetupTable(lcl, hl,
		ctx->bs_ltab_code, ctx->bs_ltab_mask, ctx->bs_ltab_len,
		ctx->bs_ltab_idx, ctx->bs_ltab_next);
	if(j<0)
	{
		printf("Static2 Literal/Length Table Problem %d\n", j);
		return(j);
	}

	j=BTLZA_BitDec_SetupTable(dcl, hd,
		ctx->bs_dtab_code, ctx->bs_dtab_mask, ctx->bs_dtab_len,
		ctx->bs_dtab_idx, ctx->bs_dtab_next);
	if(j<0)
	{
		printf("Static2 Distance Table Problem %d\n", j);
		return(j);
	}

	return(0);
}



#if 1
void BTLZA_BitDec_MemCpy(byte *dst, byte *src, int len)
{
	byte *s, *t, *te;
#ifdef X86_64
	s64 d;
#else
	int d;
#endif
	int i, j, k, l;

#if defined(X86) || defined(X86_64) || defined(ARM)
	s=src; t=dst; i=len;
	d=dst-src;
	te=dst+len;
	if(d>>3)
	{
		while(t<=(te-8))
		{
			*((s64 *)t)=*((s64 *)s);
			t+=8; s+=8;
		}
		if(t<=(te-4))
		{
			*((u32 *)t)=*((u32 *)s);
			t+=4; s+=4;
		}
		if(t<=(te-2))
			{ *t++=*s++; *t++=*s++; }
		if(t<te)
			{ *t++=*s++; }
		return;
	}else if(d>>2)
	{
		while(t<=(te-8))
		{
			((u32 *)t)[0]=((u32 *)s)[0];
			((u32 *)t)[1]=((u32 *)s)[1];
			t+=8; s+=8;
		}
		if(t<=(te-4))
			{ *((int *)t)=*((int *)s); t+=4; s+=4; }
		if(t<=(te-2))
			{ *t++=*s++; *t++=*s++; }
		if(t<te)
			{ *t++=*s++; }
		return;
	}else
	{
		while(t<=(te-8))
		{
			t[ 0]=s[ 0];	t[ 1]=s[ 1];
			t[ 2]=s[ 2];	t[ 3]=s[ 3];
			t[ 4]=s[ 4];	t[ 5]=s[ 5];
			t[ 6]=s[ 6];	t[ 7]=s[ 7];
			t+=8; s+=8;
		}
		if(t<=(te-4))
		{
			t[ 0]=s[ 0];	t[ 1]=s[ 1];
			t[ 2]=s[ 2];	t[ 3]=s[ 3];
			t+=4; s+=4;
		}
		if(t<=(te-2))
		{
			*t++=*s++; *t++=*s++;
		}
		if(t<te)
			{ *t++=*s++; }
		return;
	}
#else
	s=src; t=dst; i=len;
	while(i--)*t++=*s++;
#endif
}
#endif


void BTLZA_BitDec_MemCpyLax(byte *dst, byte *src, int len)
{
	byte *s, *t, *te;
#ifdef X86_64
	s64 d;
#else
	int d;
#endif
	int i, j, k, l;

#if defined(X86) || defined(X86_64) || defined(ARM)
	s=src; t=dst; i=len;
	d=dst-src;
	te=dst+len;
	if(d>>3)
	{
		while(t<te)
		{
			((s64 *)t)[0]=((s64 *)s)[0];
			((s64 *)t)[1]=((s64 *)s)[1];
			t+=16; s+=16;
		}
		return;
	}else if(d>>2)
	{
		while(t<te)
		{
			((u32 *)t)[0]=((u32 *)s)[0];
			((u32 *)t)[1]=((u32 *)s)[1];
			t+=8; s+=8;
		}
		return;
	}else
	{
		switch(d)
		{
		case 1:
			i=s[0]; i=i|(i<<8); i=i|(i<<16);
			while(t<te)
				{ ((u32 *)t)[0]=i; ((u32 *)t)[1]=i; t+=8; }
			break;
		case 2:
			i=*(u16 *)s; i=i|(i<<16);
			while(t<te)
				{ ((u32 *)t)[0]=i; ((u32 *)t)[1]=i; t+=8; }
			break;

		case 4:
			i=*(u32 *)s;
			while(t<te)
				{ ((u32 *)t)[0]=i; ((u32 *)t)[1]=i; t+=8; }
			break;
		default:
			while(t<te)
			{
				t[ 0]=s[ 0];	t[ 1]=s[ 1];
				t[ 2]=s[ 2];	t[ 3]=s[ 3];
				t[ 4]=s[ 4];	t[ 5]=s[ 5];
				t[ 6]=s[ 6];	t[ 7]=s[ 7];
				t+=8; s+=8;
			}
			break;
		}
		return;
	}
#else
	s=src; t=dst; te=dst+len;
	while(t<te)
	{
		t[ 0]=s[ 0];	t[ 1]=s[ 1];
		t[ 2]=s[ 2];	t[ 3]=s[ 3];
		t[ 4]=s[ 4];	t[ 5]=s[ 5];
		t[ 6]=s[ 6];	t[ 7]=s[ 7];
		t+=8; s+=8;
	}
#endif
}


int BTLZA_BitDec_DecodeRunLzMatch(BTLZA_Context *ctx, int sym)
{
	byte *s, *t;
	int i, j, k, l;

	i=sym-257;
	j=ctx->lbase[i]+BTLZA_BitDec_ReadNBits(ctx, ctx->lextra[i]);
	k=BTLZA_BitDec_DecodeDistance(ctx);
	ctx->lz_lastdist=k;
	ctx->lz_lastrun=j;
	s=ctx->ct-k;
	i=j;
	BTLZA_BitDec_MemCpy(ctx->ct, s, i);
	ctx->ct+=i;
	return(0);
}

#ifndef BTLZA_NOBASIC2
// force_inline
int BTLZA_BitDec_DecodeRunLzMatchBasic2(
	BTLZA_Context *ctx, int sym)
{
	byte *s, *t;
	int i, j, k, l;

	i=sym-257;
	j=ctx->lbase[i]+BTLZA_BitDec_ReadNBitsBasic2(ctx, ctx->lextra[i]);
	k=BTLZA_BitDec_DecodeDistanceBasic2(ctx);
	ctx->lz_lastdist=k;
	ctx->lz_lastrun=j;
	s=ctx->ct-k;
	i=j;
	BTLZA_BitDec_MemCpyLax(ctx->ct, s, i);
	ctx->ct+=i;
	return(0);
}
#endif

int BTLZA_BitDec_DecodeRunExtMatch(BTLZA_Context *ctx, int sym)
{
	byte *s, *t;
	int i, j, k, l;

	i=sym-318;
	j=btlza_dbase1[i]+BTLZA_BitDec_ReadExtraNBits(ctx, btlza_dextra1[i]);
//	j=btlza_dbase1[i]+BTLZA_BitDec_ReadNBitsBasic2(ctx,
//		btlza_dextra1[i]);
		
	switch(j)
	{
	case 1:		/* Prior Length and Distance */
		k=ctx->lz_lastdist;
		l=ctx->lz_lastrun;
		s=ctx->ct-k; i=l;
		BTLZA_BitDec_MemCpyLax(ctx->ct, s, i);
		ctx->ct+=i;
		break;
	case 2:		/* Prior Length */
		k=BTLZA_BitDec_DecodeDistance(ctx);
		ctx->lz_lastdist=k;
		l=ctx->lz_lastrun;
		s=ctx->ct-k; i=l;
		BTLZA_BitDec_MemCpyLax(ctx->ct, s, i);
		ctx->ct+=i;
		break;
	case 3:		/* Prior Distance */
		i=BTLZA_BitDec_DecodeSymbol(ctx);
		if(i<0)return(i);
		i=i-257;
		l=ctx->lbase[i]+BTLZA_BitDec_ReadNBits(ctx, ctx->lextra[i]);
		ctx->lz_lastrun=l;
		k=ctx->lz_lastdist;
		s=ctx->ct-k; i=l;
		BTLZA_BitDec_MemCpyLax(ctx->ct, s, i);
		ctx->ct+=i;
		break;
	default:
		return(-1);
		break;
	}
	return(0);
}

int BTLZA_BitDec_DecodeRun(BTLZA_Context *ctx, int sym)
{
	if(sym<318)
		{ return(BTLZA_BitDec_DecodeRunLzMatch(ctx, sym)); }
	if(sym<334)
		{ return(BTLZA_BitDec_DecodeRunExtMatch(ctx, sym)); }

	return(-1);
}

#ifndef BTLZA_NOBASIC2
int BTLZA_BitDec_DecodeRunBasic2(BTLZA_Context *ctx, int sym)
{
	if(sym<318)
		{ return(BTLZA_BitDec_DecodeRunLzMatchBasic2(ctx, sym)); }
	if(sym<334)
		{ return(BTLZA_BitDec_DecodeRunExtMatch(ctx, sym)); }

	return(-1);
}
#endif

int BTLZA_BitDec_DecodeBlockDataI(BTLZA_Context *ctx)
{
	int i;

	while(1)
	{
		i=BTLZA_BitDec_DecodeSymbol(ctx);
		if(i<0)return(i);

		if(i<256)
		{
			*ctx->ct++=i;
			continue;
		}
		if(i==256)break;

		i=BTLZA_BitDec_DecodeRun(ctx, i);
		if(i<0)return(i);
	}

	return(0);
}

#ifndef BTLZA_NOBASIC2
int BTLZA_BitDec_DecodeBlockDataBasic2I(BTLZA_Context *ctx)
{
	int sab[4];
	int i;

	while(1)
	{
		i=BTLZA_BitDec_DecodeSymbolBasic2(ctx);
		if(i<0)return(i);

		if(i<256)
		{
			*ctx->ct++=i;
			continue;
		}
		if(i==256)break;

		i=BTLZA_BitDec_DecodeRunBasic2(ctx, i);
		if(i<0)return(i);
	}

	return(0);
}
#endif

int BTLZA_BitDec_DecodeBlockData(BTLZA_Context *ctx)
{
	int i;

	if(ctx->bs_flags&BGBBTJ_ZFL_ISBTLZH)
		{ ctx->lbase=btlza_lbase3; ctx->lextra=btlza_lextra3; }
	else if(ctx->bs_flags&BGBBTJ_ZFL_DEFLATE64)
		{ ctx->lbase=btlza_lbase2; ctx->lextra=btlza_lextra2; }
	else
		{ ctx->lbase=btlza_lbase1; ctx->lextra=btlza_lextra1; }

	if(ctx->bs_rhtab_n>1)
	{
		ctx->BS_DecodeSymbol=
			BTLZA_BitDec_DecodeSymbolRingHuff;
		ctx->BS_DecodeDistanceSymbol=
			BTLZA_BitDec_DecodeDistanceSymbolRingHuff;
		return(BTLZA_BitDec_DecodeBlockDataI(ctx));
	}else if(ctx->ctxbits || (ctx->bs_flags&BGBBTJ_ZFL_ESCAPE_FF))
	{
		ctx->BS_DecodeSymbol=
			BTLZA_BitDec_DecodeSymbolBasic;
		ctx->BS_DecodeDistanceSymbol=
			BTLZA_BitDec_DecodeDistanceSymbolBasic;
		return(BTLZA_BitDec_DecodeBlockDataI(ctx));
	}else
	{
#ifndef BTLZA_NOBASIC2
		ctx->BS_DecodeSymbol=
			BTLZA_BitDec_DecodeSymbolBasic2;
		ctx->BS_DecodeDistanceSymbol=
			BTLZA_BitDec_DecodeDistanceSymbolBasic2;
		return(BTLZA_BitDec_DecodeBlockDataBasic2I(ctx));
#else
		ctx->BS_DecodeSymbol=
			BTLZA_BitDec_DecodeSymbolBasic;
		ctx->BS_DecodeDistanceSymbol=
			BTLZA_BitDec_DecodeDistanceSymbolBasic;
		return(BTLZA_BitDec_DecodeBlockDataI(ctx));
#endif
	}
}

int BTLZA_BitDec_AlignBitsEnd(BTLZA_Context *ctx)
{
	int i, j;

	//N/A in arithmetic mode
	if(ctx->rmax)
		return(0);

	while(ctx->bs_pos<=24)
	{
		ctx->cs--;
		ctx->bs_pos+=8;
		ctx->bs_win<<=8;
	}
	return(0);
}

int BTLZA_BitDec_DecodeHeaderBTArith(BTLZA_Context *ctx)
{
	int hl, hd, hc, he, hr;
	int i, j;

	hr=BTLZA_BitDec_Read3Bits(ctx);
	if(hr==0)
	{
		//Switch to Byte mode
		BTLZA_BitArith_SetupEndDecode(ctx);
		ctx->ctxbits=0;
		if(ctx->bs_flags&BGBBTJ_ZFL_ESCAPE_FF)
			{ ctx->BS_ReadByte=BTLZA_BitDec_ReadByteEscape; }
		else
			{ ctx->BS_ReadByte=BTLZA_BitDec_ReadByteBasic; }
		return(0);
	}
	
	if(hr==1)
	{
		//Switch to Arithmetic Mode
		hl=BTLZA_BitDec_ReadNBits(ctx, 4)+12;
//		he=BTLZA_BitDec_ReadBit(ctx);
		hd=BTLZA_BitDec_ReadNBits(ctx, 5);

		BTLZA_BitDec_AlignBitsEnd(ctx);

		BTLZA_BitArith_SetupRawContextBits(ctx, hl);
		BTLZA_BitArith_SetupBeginDecode(ctx);
		ctx->BS_ReadByte=BTLZA_BitDec_ReadByteArithLE;
		ctx->BS_DecodeSymbol=BTLZA_BitDec_DecodeSymbolBasic;
		ctx->BS_DecodeClSymbol=BTLZA_BitDec_DecodeSymbolBasic;
		ctx->BS_DecodeDistanceSymbol=BTLZA_BitDec_DecodeDistanceSymbolBasic;

		ctx->bs_win=0;
		ctx->bs_pos=32;
		BTLZA_BitDec_ReadAdjust(ctx);
		return(0);
	}
	
	return(-1);
}

int BTLZA_BitDec_DecodeBlock(BTLZA_Context *ctx)
{
	int fi, ty, ty2, l, nl;
	int i;

	fi=BTLZA_BitDec_ReadBit(ctx);
	ty=BTLZA_BitDec_Read2Bits(ctx);

	switch(ty)
	{
	case 0:
		BTLZA_BitDec_AlignByte(ctx);
		l=BTLZA_BitDec_ReadAlignedWord(ctx);
		nl=(~BTLZA_BitDec_ReadAlignedWord(ctx))&0xFFFF;
		if(l!=nl)
			{ return(-3); }

		while(l--)
		{
			i=BTLZA_BitDec_ReadAlignedByte(ctx);
			*ctx->ct++=i;
		}
		break;
	case 1:
		i=BTLZA_BitDec_SetupStatic(ctx);
		if(i<0)return(i);
		i=BTLZA_BitDec_DecodeBlockData(ctx);
		if(i<0)return(i);
		break;
	case 2:
		i=BTLZA_BitDec_DecodeHeader(ctx);
		if(i<0)return(i);
		i=BTLZA_BitDec_DecodeBlockData(ctx);
		if(i<0)return(i);
		break;

	case 3:
		ty2=BTLZA_BitDec_Read3Bits(ctx);
		if(ty2==7)
		{
			if(ctx->bs_flags&BGBBTJ_ZFL_ESCBTLZA)
				return(2);
			return(-2);
		}

		if(ty2==0)
		{
			i=BTLZA_BitDec_ReadNBits(ctx, 6);
			l=btlza_dbase1[i]+BTLZA_BitDec_ReadNBits(ctx, btlza_dextra1[i]);
			while(l--)
			{
				i=BTLZA_BitDec_ReadNBits(ctx, 8);
				*ctx->ct++=i;
			}
			break;
		}
			
		if(ty2==1)
		{
			i=BTLZA_BitDec_DecodeHeaderBTArith(ctx);
			if(i<0)return(i);
			break;
		}
			
		if(ty2==2)
		{
			i=BTLZA_BitDec_DecodeHeaderBTLZH(ctx);
			if(i<0)return(i);
			ctx->bs_flags|=BGBBTJ_ZFL_ISBTLZH;
			i=BTLZA_BitDec_DecodeBlockData(ctx);
			ctx->bs_flags&=~BGBBTJ_ZFL_ISBTLZH;
			if(i<0)return(i);
			break;
		}

		if(ty2==3)
		{
			i=BTLZA_BitDec_DecodeStaticHeader2(ctx);
			if(i<0)return(i);
			i=BTLZA_BitDec_DecodeBlockData(ctx);
			if(i<0)return(i);
			break;
		}

		return(-2);
		break;

	default:
		return(-2);
		break;
	}

	return(fi);
}

int BTLZA_BitDec_DecodeStream(BTLZA_Context *ctx,
	byte *ibuf, byte *obuf, int isz, int osz)
{
	return(BTLZA_BitDec_DecodeStreamSz(ctx, ibuf, obuf, isz, osz, NULL, 0));
}

int BTLZA_BitDec_DecodeStreamSz(BTLZA_Context *ctx,
	byte *ibuf, byte *obuf, int isz,
	int osz, int *rosz, int flags)
{
	int i, k;

	if(flags&BGBBTJ_ZFL_ESCAPE_FF)
	{
		ctx->BS_ReadByte=BTLZA_BitDec_ReadByteEscape;
	}else
	{
		ctx->BS_ReadByte=BTLZA_BitDec_ReadByteBasic;
	}

#ifdef BTLZA_NOBASIC2
	ctx->BS_DecodeSymbol=BTLZA_BitDec_DecodeSymbolBasic;
	ctx->BS_DecodeClSymbol=BTLZA_BitDec_DecodeSymbolBasic;
	ctx->BS_DecodeDistanceSymbol=BTLZA_BitDec_DecodeDistanceSymbolBasic;
#else
	ctx->BS_DecodeSymbol=BTLZA_BitDec_DecodeSymbolBasic2;
	ctx->BS_DecodeClSymbol=BTLZA_BitDec_DecodeSymbolBasic2;
	ctx->BS_DecodeDistanceSymbol=BTLZA_BitDec_DecodeDistanceSymbolBasic2;
#endif

	ctx->BS_ReadExtraNBits=BTLZA_BitDec_ReadExtraNBitsBasic;
	ctx->BS_ReadNBits=BTLZA_BitDec_ReadNBitsBasic;
	ctx->BS_SkipNBits=BTLZA_BitDec_SkipNBitsBasic;
	ctx->BS_ReadBit=BTLZA_BitDec_ReadBitBasic;

	ctx->ibuf=ibuf;
	ctx->obuf=obuf;
//	ctx->cs=ibuf+4;
	ctx->cs=ibuf;
	ctx->ct=obuf;
	ctx->cse=ibuf+isz;
	ctx->cte=obuf+osz;

	ctx->bs_win=(ctx->bs_win>>8)|(BTLZA_BitDec_ReadByte(ctx)<<24);
	ctx->bs_win=(ctx->bs_win>>8)|(BTLZA_BitDec_ReadByte(ctx)<<24);
	ctx->bs_win=(ctx->bs_win>>8)|(BTLZA_BitDec_ReadByte(ctx)<<24);
	ctx->bs_win=(ctx->bs_win>>8)|(BTLZA_BitDec_ReadByte(ctx)<<24);
	ctx->bs_pos=0;
	
	ctx->bs_flags=flags;

	k=0;
	while(1)
	{
		i=BTLZA_BitDec_DecodeBlock(ctx);
		if(i<0)
		{
			if(rosz)
				{ *rosz=ctx->ct-ctx->obuf; }
			return(i);
		}
		if(i)break;
	}

	i=ctx->bs_pos;
	while(i<=24)
	{
		ctx->cs--;
		i+=8;
	}

	if(rosz)
		*rosz=ctx->ct-ctx->obuf;

	i=ctx->cs-ibuf;
	return(i);
}
