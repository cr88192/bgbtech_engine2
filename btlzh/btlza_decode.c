// #include <btlzazip.h>

#if 0
int BTLZA_Decode_DecodeDistance(BTLZA_Context *ctx)
{

	int i, j, k;

	j=BTLZA_BitArith_DecodeSymbol6(ctx, ctx->mdl_dist, (1<<12)-1);
//	k=btlza_dbase[j]+BTLZA_BitArith_InputExtraBits(ctx,
//		btlza_dextra[j], ctx->mdl_dist, (1<<12)-1);
	k=btlza_dbase[j]+BTLZA_BitArith_InputExtraBits(ctx,
		btlza_dextra[j], ctx->mdl_xbits, 4095);

	return(k);
}

int BTLZA_Decode_DecodeRun(BTLZA_Context *ctx, int sym)
{
	char *s;
	int i, j, k;

	i=sym-257;
//	j=btlza_lbase[i]+BTLZA_BitArith_InputExtraBits(ctx,
//		btlza_lextra[i], ctx->mdl_lit, ctx->ctxmask);
	ctx->wctx&=4095;
	j=btlza_lbase[i]+BTLZA_BitArith_InputExtraBits(ctx,
		btlza_lextra[i], ctx->mdl_xbits, 4095);
	k=BTLZA_Decode_DecodeDistance(ctx);
	s=ctx->ct-k;
	i=j;
	while(i--)
		{ *ctx->ct++=*s++; }
//	ctx->wctx=
//		((*(ctx->ct-3))<<18)|
//		((*(ctx->ct-2))<< 9)|
//		 (*(ctx->ct-1));
//	ctx->wctx&=ctx->ctxmask;
	ctx->wctx=0;
	return(0);
}

int BTLZA_Decode_DecodeSymbol(BTLZA_Context *ctx)
{
	return(BTLZA_BitArith_DecodeSymbol9(
		ctx, ctx->mdl_lit, ctx->ctxmask));
}

int BTLZA_Decode_DecodeBlockData(BTLZA_Context *ctx)
{
	int i;

	while(1)
	{
		i=BTLZA_Decode_DecodeSymbol(ctx);
		if(i<0)return(i);

		if(i<256)
		{
			*ctx->ct++=i;
			continue;
		}
		if(i==256)break;
		i=BTLZA_Decode_DecodeRun(ctx, i);
		if(i<0)return(i);
	}

	return(0);
}

int BTLZA_Decode_DecodeBlock(BTLZA_Context *ctx)
{
	int fi, ty, l, nl, fl;
	int i;

//	fi=PDUNZ_ReadBit();
//	ty=PDUNZ_Read2Bits();

//	i=*ctx->cs++;
//	if(i!=0xB1)return(-1);

	ty=*ctx->cs++;
	if(!ty)return(1);

//	printf("blk %d %d\n", fi, ty);

	switch(ty)
	{
	case 0xA0:
		ctx->cs=BGBBTJ_DecodeVLI(ctx->cs, &l);
		while(l--)
		{
			i=*ctx->cs++;
			*ctx->ct++=i;
		}
		break;
	case 0xA6:
		ctx->cs=BGBBTJ_DecodeVLI(ctx->cs, &fl);
		BTLZA_BitArith_SetupContextBits(ctx, 12+(fl&15));
		BTLZA_BitArith_SetupDecode(ctx, ctx->cs, ctx->cse-ctx->cs);
		i=BTLZA_Decode_DecodeBlockData(ctx);
		if(i<0)return(i);
		break;
//	case 2:
//		i=PDUNZ_DecodeHeader();
//		if(i<0)return(i);
//		i=PDUNZ_DecodeBlockData();
//		if(i<0)return(i);
//		break;

//	case 3:
	default:
		return(-2);
		break;
	}

	return(0);
}

BTEIFGL_API int BTLZA_Decode_DecodeStreamSz(byte *ibuf, byte *obuf, int isz,
	int osz, int *rosz, int flags)
{
	BTLZA_Context *ctx;
	int i, ws;

	ctx=BTLZA_AllocContext();

	ctx->ibuf=ibuf;
	ctx->obuf=obuf;
	ctx->cs=ibuf;
	ctx->cse=ibuf+isz;
	ctx->ct=obuf;
	ctx->cte=obuf+osz;

	while(1)
	{
		i=BTLZA_Decode_DecodeBlock(ctx);
		if(i<0)return(i);
		if(i)break;
	}

	if(rosz)
		*rosz=ctx->ct-ctx->obuf;

	i=ctx->cs-ibuf;
	return(i);
}
#endif

BTEIFGL_API int BTLZA_DecodeStreamSzZl(byte *ibuf, byte *obuf, int isz,
	int osz, int *rosz, int flags)
{
	BTLZA_Context *ctx;
	byte *cs, *cse;
	int i, j, cm;
	
	if(!ibuf || !obuf)
		return(-1);
	
//	i=(ibuf[0]<<8)+ibuf[1];
//	cm=(i>>8)&15;
	
	cs=ibuf; cse=ibuf+isz;
	i=(*cs)>>4;
//	if((i>=0) && (i<=7))
//	if((i>=0) && (i<=14))
//	if((i!=8) || ((*cs)==0x89))
	if((i==8) || (i==11))
	{
		j=(cs[0]<<8)+cs[1];
		if(j%31)
		{
			if(i==11)
			{
				j=j&0x8FFF;
				if(j%31)
					return(-5);
			}
			
			i=*cs++;
			cm=i&15;
		}else
		{
			i=(cs[0]<<8)+cs[1]; cs+=2;
			cm=(i>>8)&15;
		}
	}else
	{
		i=(cs[0]<<8)+cs[1]; cs+=2;
		cm=(i>>8)&15;
		if(i%31)
			return(-4);
	}

	if(cm==9)
	{
//		i=PDUNZ_DecodeStreamSz(ibuf+2, obuf, isz-2,
//			osz, rosz, flags|BGBBTJ_ZFL_DEFLATE64);

		ctx=BTLZA_AllocContext();
		if(!ctx)return(-2);
//		i=BTLZA_BitDec_DecodeStreamSz(ctx, ibuf+2, obuf, isz-2,
//			osz, rosz, flags|BGBBTJ_ZFL_DEFLATE64);
		i=BTLZA_BitDec_DecodeStreamSz(ctx, cs, obuf, cse-cs,
			osz, rosz, flags|BGBBTJ_ZFL_DEFLATE64);
		BTLZA_FreeContext(ctx);
		return(i);
	}
	if(cm==8)
	{
//		i=PDUNZ_DecodeStreamSz(ibuf+2, obuf, isz-2, osz, rosz, flags);
		ctx=BTLZA_AllocContext();
		if(!ctx)return(-2);
//		i=BTLZA_BitDec_DecodeStreamSz(ctx,
//			ibuf+2, obuf, isz-2, osz, rosz, flags);
		i=BTLZA_BitDec_DecodeStreamSz(ctx,
			cs, obuf, cse-cs, osz, rosz, flags);
		BTLZA_FreeContext(ctx);
		return(i);
	}
	if(cm==10)
	{
//		i=PDUNZ_DecodeStreamSz(ibuf+2, obuf, isz-2, osz, rosz, flags);
		ctx=BTLZA_AllocContext();
		if(!ctx)return(-2);
//		i=BTLZA_BitDec_DecodeStreamSz(ctx,
//			ibuf+2, obuf, isz-2, osz, rosz, flags);
		i=BTLZA_BitDec_DecodeStreamSz(ctx,
			cs, obuf, cse-cs, osz, rosz, flags);
		BTLZA_FreeContext(ctx);
		return(i);
	}

	return(-3);
}

BTEIFGL_API int BTLZA_DecodeStreamZl(byte *ibuf, byte *obuf, int isz, int osz)
{
	return(BTLZA_DecodeStreamSzZl(ibuf, obuf, isz, osz, NULL, 0));
}
