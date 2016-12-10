#if 1
int BGBDT_ReadLzSTF(BGBDT_RiceContext *ctx, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;
	
//	ix0=BGBDT_Rice_ReadAdRiceDc(ctx, rk);
	ix0=ctx->ReadAdRiceLL(ctx, rk);
//	ix0=ctx->ReadAdDist(ctx, rk);
	ix1=(ix0*7)/8;
//	ix1=(ix0*3)/4;
	sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
	ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;
	return(sy0);
}

int BGBDT_ReadLzSTF3(BGBDT_RiceContext *ctx, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;
	
//	ix0=BGBDT_Rice_ReadAdRiceDc(ctx, rk);
	ix0=ctx->ReadAdRiceLL(ctx, rk);
//	ix0=ctx->ReadAdDist(ctx, rk);
//	ix1=(ix0*7)/8;
//	ix1=(ix0*3)/4;
//	ix1=(ix0*(57344-ix0*71))>>16;
	ix1=(ix0*(57344-ix0*68))>>16;
//	ix1=(ix0*((232*256)-ix0*93))>>16;
	sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
	ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;
	return(sy0);
}

int BGBDT_ReadLzSMTF2(BGBDT_RiceContext *ctx, int *rk)
{
	int ix0, ix1, ix2, ix3;
	int sy0, sy1;
	
//	ix0=ctx->lzidx[sym];
//	ix0=(byte)(ix0-ctx->lzwpos);
	ix0=ctx->ReadAdRiceLL(ctx, rk);
	
//	if(ix0<96)
//	if(ix0<80)
	if(ix0<72)
//	if(ix0<64)
//	if(ix0<48)
//	if(ix0<32)
//	if(1)
	{
//		ix1=(ix0*(224-((ix0*120)/256)))/256;
//		ix1=(ix0*(224-((ix0*128)/256)))/256;
//		ix1=(ix0*(224-((ix0*136)/256)))/256;
//		ix1=(ix0*(224-((ix0*132)/256)))/256;

//		ix1=(ix0*(221-((ix0*49)/256)))/256;
//		ix1=(ix0*(236-((ix0*101)/256)))/256;
//		ix1=(ix0*(209-((ix0*53)/256)))/256;
//		ix1=(ix0*(223-((ix0*77)/256)))/256;
//		ix1=(ix0*(224-((ix0*71)/256)))/256;
//		ix1=(ix0*(57344-ix0*68))>>16;
//		ix1=(ix0*(57344-ix0*100))>>16;
		ix1=(ix0*(57344-ix0*72))>>16;

//		ix1=(ix0*(224-(ix0>>0)))/256;

//		ix1=(ix0*(224-(ix0>>2)))/256;
//		ix1=(ix0*(224-(ix0>>1)))/256;
//		ix1=(ix0*(112-(ix0>>2)))/128;
//		ix1=(ix0*(56-(ix0>>3)))/64;
//		ix1=(ix0*(28-(ix0>>4)))/32;

//		ix1=(ix0*(31-(ix0>>4)))/32;
//		ix1=(ix0*(15-(ix0>>4)))/16;
//		ix1=(ix0*(15-(ix0>>6)))/16;
//		ix1=(ix0*(15-(ix0>>5)))/16;
//		ix1=(ix0*(7-(ix0>>5)))/8;
//		ix1=(ix0*(7-(ix0>>6)))/8;
//		ix1=(ix0*(7-(ix0>>7)))/8;
//		ix1=(7-(ix0>>6))/8;
//		ix1=(15-(ix0>>4))/16;
//		ix1=(ix0*3)/4;
//		ix1=(ix0*7)/8;
//		ix1=(ix0>0)?(ix0-1):0;
		ix2=(byte)(ix0+ctx->lzwpos);
		ix3=(byte)(ix1+ctx->lzwpos);
	}else
	{
		ix1=-1;
		ix2=(byte)(ix0+ctx->lzwpos);
		ix3=(byte)(ix1+ctx->lzwpos);
		ctx->lzwpos--;
	}

	sy0=ctx->lzwin[ix2];	sy1=ctx->lzwin[ix3];
	ctx->lzwin[ix2]=sy1;	ctx->lzwin[ix3]=sy0;
//	ctx->lzidx[sy0]=ix3;	ctx->lzidx[sy1]=ix2;
	
	return(sy0);
}

int BGBDT_ReadLzWSTF1(BGBDT_RiceContext *ctx, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;
	
	ix0=ctx->ReadAdRiceLL(ctx, rk);
	ix1=(ix0*(57344-ix0*72))>>16;
	sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
	if((ctx->lzstat[sy0]+1)>=ctx->lzstat[sy1])
	{	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
		ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;	}
	else
	{
		ix1=(ix0*255)>>8;
		sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
		if((ctx->lzstat[sy0]+1)>=ctx->lzstat[sy1])
		{	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
			ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;	}
	}
	ctx->lzstat[sy0]++;

	return(sy0);
}

int BGBDT_DecodeChunkLZ(BGBDT_RiceContext *ctx,
	byte *obuf, int obsz)
{
	byte *ct, *ct1, *cte, *cs, *cse;
	int kr, ks, kl, kd;
	int r, l, d, ll, ld;
	int i0, i1, i2, i3;
	int i, j, k;
	
	for(i=0; i<256; i++)
		{ ctx->lzwin[i]=i; ctx->lzidx[i]=i; }
	ctx->lzwpos=0;
	
	ctx->ReadAdRiceLL=BGBDT_Rice_ReadAdRiceDc;
	ctx->ReadAdDist=BGBDT_Rice_ReadAdRiceDc;
	ctx->ReadSym=BGBDT_ReadLzSTF;

	if((ctx->lzctrl&7)!=0)
	{
		ctx->ReadAdRiceLL=BGBDT_Rice_ReadAdRiceLL;
		ctx->ReadAdDist=BGBDT_Rice_ReadAdRiceLL;

		if((ctx->lzctrl&7)==2)
		{
			ctx->ReadAdDist=BGBDT_Rice_ReadAdExp2Rice;
			ctx->ReadSym=BGBDT_ReadLzSMTF2;
//			ctx->ReadSym=BGBDT_ReadLzSTF3;
		}

		if((ctx->lzctrl&7)==3)
		{
			ctx->ReadAdDist=BGBDT_Rice_ReadAdExp2Rice;
			ctx->ReadSym=BGBDT_ReadLzWSTF1;
		}
	}
	
	kr=4; ks=4; kl=4; kd=4; ll=1; ld=1;
	ct=obuf; cte=obuf+obsz;
//	while(ct<cte)
	while(1)
	{
//		r=BGBDT_Rice_ReadAdRiceDc(ctx, &kr);
		r=ctx->ReadAdRiceLL(ctx, &kr);
//		r=ctx->ReadAdDist(ctx, &kr);
		if(r>0)
		{
			ct1=ct+r;
			if(ct1>cte)
			{
				__debugbreak();
				break;
			}
			while(ct<ct1)
			{
//				*ct++=BGBDT_ReadLzSTF(ctx, &ks);
				*ct++=ctx->ReadSym(ctx, &ks);
			}
		}

//		l=BGBDT_Rice_ReadAdRiceDc(ctx, &kl);
		l=ctx->ReadAdRiceLL(ctx, &kl);
//		l=ctx->ReadAdDist(ctx, &kl);
		
		if(l<=0)
		{
			if(r<=0)
				break;
			continue;
		}
		
//		d=BGBDT_Rice_ReadAdRiceDc(ctx, &kd);
//		d=ctx->ReadAdRiceLL(ctx, &kd);
		d=ctx->ReadAdDist(ctx, &kd);
		if(l<3)
		{
			if(l==2)
			{
				__debugbreak();
				//special
				break;
			}
			l=ll;
		}
		
		if(!d)d=ld;
		ll=l; ld=d;
		if((ct+l)>cte)
		{
			__debugbreak();
			break;
		}
		cs=ct-d; cse=cs+l;
		if(cs<obuf)
		{
			__debugbreak();
			break;
		}
		while(cs<cse)
			*ct++=*cs++;
	}
	return(ct-obuf);
}
#endif
