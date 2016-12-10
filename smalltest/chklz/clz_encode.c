void BGBDT_EmitLzSTF(BGBDT_RiceContext *ctx, int sym, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;

	ctx->lzstat[sym]++;
	
	ix0=ctx->lzidx[sym];
	ix1=(ix0*7)/8;
//	ix1=(ix0*3)/4;
	sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
	ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;
	
//	BGBDT_Rice_WriteAdRiceDc(ctx, ix0, rk);
	ctx->WriteAdRiceLL(ctx, ix0, rk);
//	ctx->WriteAdDist(ctx, ix0, rk);
//	return(0);
}

void BGBDT_EmitLzSTF3(BGBDT_RiceContext *ctx, int sym, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;

	ctx->lzstat[sym]++;
	
	ix0=ctx->lzidx[sym];
//	ix1=(ix0*7)/8;
//	ix1=(ix0*3)/4;
//	ix1=(ix0*(224-((ix0*71)>>8)))>>8;
//	ix1=(ix0*(57344-ix0*71))>>16;
	ix1=(ix0*(57344-ix0*68))>>16;
//	ix1=(ix0*((232*256)-ix0*93))>>16;
	sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
	ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;
	
//	BGBDT_Rice_WriteAdRiceDc(ctx, ix0, rk);
	ctx->WriteAdRiceLL(ctx, ix0, rk);
//	ctx->WriteAdDist(ctx, ix0, rk);
//	return(0);
}

void BGBDT_EmitLzSMTF2(BGBDT_RiceContext *ctx, int sym, int *rk)
{
	int ix0, ix1, ix2, ix3;
	int sy0, sy1;

	ctx->lzstat[sym]++;

	ix0=ctx->lzidx[sym];
	ix0=(byte)(ix0-ctx->lzwpos);
	
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
//		ix1=(ix0*(15-(ix0>>6)))/16;
//		ix1=(ix0*(15-(ix0>>5)))/16;
//		ix1=(ix0*(7-(ix0>>5)))/8;
//		ix1=(ix0*(7-(ix0>>6)))/8;
//		ix1=(ix0*(7-(ix0>>7)))/8;
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
	ctx->lzidx[sy0]=ix3;	ctx->lzidx[sy1]=ix2;
	
	ctx->WriteAdRiceLL(ctx, ix0, rk);

	if(sy0!=sym)
	{
		printf("BGBDT_EmitLzSMTF2: Fail\n");
//		__debugbreak();
	}

//	return(0);
}

void BGBDT_EmitLzWSTF1(BGBDT_RiceContext *ctx, int sym, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;

	ix0=ctx->lzidx[sym];
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
	ctx->lzstat[sym]++;
		
	ctx->WriteAdRiceLL(ctx, ix0, rk);
}

void BGBDT_ChunkLzHashChi(BGBDT_RiceContext *ctx,
	byte *ics, int *chi)
{
	int h;
//	h=((ics[0]*251+ics[1])*251+ics[2])*251;
	h=((ics[0]*251^ics[1])*251^ics[2])*251;

#if 0
	chi[0]=(h>>8)&1023;
	h=h*251+31;
	chi[1]=1024|((h>>8)&1023);
	h=h*251+31;
	chi[2]=1024|((h>>8)&1023);
	h=h*251+31;
	chi[3]=1024|((h>>8)&1023);
#endif

#if 0
	h=(h>>9)&511;
	chi[0]=(h<<2)|0;
	chi[1]=(h<<2)|1;
	chi[2]=(h<<2)|2;
	chi[3]=(h<<2)|3;
#endif

#if 0
//	h=(h>>9)&511;
	h=(h>>9)&255;
//	h=(h>>9)&127;
	chi[0]=(h<<3)|0;
	chi[1]=(h<<3)|1;
	chi[2]=(h<<3)|2;
	chi[3]=(h<<3)|3;
	chi[4]=(h<<3)|4;
	chi[5]=(h<<3)|5;
	chi[6]=(h<<3)|6;
	chi[7]=(h<<3)|7;
#endif

#if 1
	h=(h>>9)&255;
	chi[ 0]=(h<<4)| 0;
	chi[ 1]=(h<<4)| 1;
	chi[ 2]=(h<<4)| 2;
	chi[ 3]=(h<<4)| 3;
	chi[ 4]=(h<<4)| 4;
	chi[ 5]=(h<<4)| 5;
	chi[ 6]=(h<<4)| 6;
	chi[ 7]=(h<<4)| 7;
	chi[ 8]=(h<<4)| 8;
	chi[ 9]=(h<<4)| 9;
	chi[10]=(h<<4)|10;
	chi[11]=(h<<4)|11;
	chi[12]=(h<<4)|12;
	chi[13]=(h<<4)|13;
	chi[14]=(h<<4)|14;
	chi[15]=(h<<4)|15;
#endif

#if 0
	chi[0]=(h>>8)&1023;
	h=h*251+31;
	chi[1]=1024|((h>>8)&1023);
	h=h*251+31;
	chi[2]=2048|((h>>8)&1023);
	h=h*251+31;
	chi[3]=2048|((h>>8)&1023);

	h=h*251+31;	
	chi[4]=3072|((h>>8)&1023);
	h=h*251+31;
	chi[5]=3072|((h>>8)&1023);
	h=h*251+31;
	chi[6]=3072|((h>>8)&1023);
	h=h*251+31;
	chi[7]=3072|((h>>8)&1023);
#endif
}

int BGBDT_ChunkLzLookupMatch(BGBDT_RiceContext *ctx,
	byte *ics, byte *icss, byte *icse, int *rbl, int *rbd)
{
	int chi[16];
	byte *s0, *s1, *s2, *s3, *se;
	int hi, l, d, bl, bd, bi;
	int i, j, k;

	if((ics+5)>=icse)
		return(0);
	
//	hi=((ics[0]*251+ics[1])*251+ics[2])*251;
//	hi=((ics[0]*251^ics[1])*251^ics[2])*251;
//	hi=(hi>>8)&255;
	
	BGBDT_ChunkLzHashChi(ctx, ics, chi);
	
	bl=0; bd=0;
	
	for(i=1; i<6; i++)
	{
		s0=ics; se=s0+256;
		if(icse<se)se=icse;
		s1=ics-i;
		if(s1<icss)break;

		while((s0<se) && (*s0==*s1))
			{ s0++; s1++; }
		l=s0-ics;
		d=s0-s1;
		if(l>bl)
			{ bl=l; bd=d; }
	}

#if 0
	s0=ics; se=s0+256;
	if(icse<se)se=icse;
	s1=ctx->lzhash[hi];
	if(s1)
	{
		while((s0<se) && (*s0==*s1))
			{ s0++; s1++; }
		l=s0-ics;
		d=s0-s1;
		if(l>bl)
			{ bl=l; bd=d; }
	}
#endif

#if 1
	bi=0;
	for(i=0; i<16; i++)
//	for(i=0; i<8; i++)
//	for(i=0; i<4; i++)
//	for(i=0; i<2; i++)
	{
		s0=ics; se=s0+256;
		if(icse<se)se=icse;
//		s1=ctx->lzhash[(i<<8)|hi];
		s1=ctx->lzhash[chi[i]];
		if(s1)
		{
			while((s0<se) && (*s0==*s1))
				{ s0++; s1++; }
			l=s0-ics;
			d=s0-s1;
			if(l>bl)
				{ bl=l; bd=d; bi=i; }
		}
	}

#if 0
	switch(bi)
	{
	case 0: break;
	case 1:
		s0=ctx->lzhash[chi[0]];		s1=ctx->lzhash[chi[1]];
		ctx->lzhash[chi[0]]=s1;		ctx->lzhash[chi[1]]=s0;
		break;
	case 2:
		s0=ctx->lzhash[chi[0]];		s1=ctx->lzhash[chi[1]];
		s2=ctx->lzhash[chi[2]];		ctx->lzhash[chi[0]]=s2;
		ctx->lzhash[chi[1]]=s0;		ctx->lzhash[chi[2]]=s1;
		break;
	case 3:
		s0=ctx->lzhash[chi[0]];		s1=ctx->lzhash[chi[1]];
		s2=ctx->lzhash[chi[2]];		s3=ctx->lzhash[chi[3]];
		ctx->lzhash[chi[0]]=s3;		ctx->lzhash[chi[1]]=s0;
		ctx->lzhash[chi[2]]=s1;		ctx->lzhash[chi[3]]=s2;
		break;
	default:
		break;
	}
#endif

#if 0
	switch(bi)
	{
	case 0: break;
	case 1:
		s0=ctx->lzhash[chi[0]];		s1=ctx->lzhash[chi[1]];
		ctx->lzhash[chi[0]]=s1;		ctx->lzhash[chi[1]]=s0;
		break;
	case 2:
		s0=ctx->lzhash[chi[0]];		s1=ctx->lzhash[chi[2]];
		ctx->lzhash[chi[0]]=s1;		ctx->lzhash[chi[2]]=s0;
		break;
	case 3:
		s0=ctx->lzhash[chi[0]];		s1=ctx->lzhash[chi[3]];
		ctx->lzhash[chi[0]]=s1;		ctx->lzhash[chi[3]]=s0;
		break;
	default:
		break;
	}
#endif
#endif

	*rbl=bl;
	*rbd=bd;
	return(bl>=3);
}

int BGBDT_ChunkLzUpdateWindow(BGBDT_RiceContext *ctx, byte *ics, int len)
{
	int chi[16];
	byte *cs, *cse;
	int hi;
	
	cs=ics; cse=cs+len;
	while(cs<cse)
	{
//		hi=((cs[0]*251+cs[1])*251+cs[2])*251;
//		hi=((cs[0]*251^cs[1])*251^cs[2])*251;
//		hi=(hi>>8)&255;

		BGBDT_ChunkLzHashChi(ctx, cs, chi);

#if 1
		ctx->lzhash[chi[15]]=ctx->lzhash[chi[14]];
		ctx->lzhash[chi[14]]=ctx->lzhash[chi[13]];
		ctx->lzhash[chi[13]]=ctx->lzhash[chi[12]];
		ctx->lzhash[chi[12]]=ctx->lzhash[chi[11]];
		ctx->lzhash[chi[11]]=ctx->lzhash[chi[10]];
		ctx->lzhash[chi[10]]=ctx->lzhash[chi[ 9]];
		ctx->lzhash[chi[ 9]]=ctx->lzhash[chi[ 8]];
		ctx->lzhash[chi[ 8]]=ctx->lzhash[chi[ 7]];
#endif

#if 1
		ctx->lzhash[chi[7]]=ctx->lzhash[chi[6]];
		ctx->lzhash[chi[6]]=ctx->lzhash[chi[5]];
		ctx->lzhash[chi[5]]=ctx->lzhash[chi[4]];
		ctx->lzhash[chi[4]]=ctx->lzhash[chi[3]];
#endif

		ctx->lzhash[chi[3]]=ctx->lzhash[chi[2]];
		ctx->lzhash[chi[2]]=ctx->lzhash[chi[1]];
		ctx->lzhash[chi[1]]=ctx->lzhash[chi[0]];
		ctx->lzhash[chi[0]]=cs;

//		ctx->lzhash[768|hi]=ctx->lzhash[512|hi];
//		ctx->lzhash[512|hi]=ctx->lzhash[256|hi];
//		ctx->lzhash[256|hi]=ctx->lzhash[hi];
//		ctx->lzhash[hi]=cs;
		cs++;
	}
	return(0);
}

int BGBDT_EncodeChunkLZ(BGBDT_RiceContext *ctx,
	byte *ibuf, int ibsz)
{
	byte *ct, *cte, *cs, *css, *cse, *csrb;
	int kr, ks, kl, kd;
	int l, d, ll, ld, r;
	int i0, i1, i2, i3;
	int i, j, k;
	
	for(i=0; i<256; i++)
		{ ctx->lzwin[i]=i; ctx->lzidx[i]=i; }
	ctx->lzwpos=0;

//	for(i=0; i<256; i++)
	for(i=0; i<4096; i++)
		{ ctx->lzhash[i]=NULL; }

	ctx->WriteAdRiceLL=BGBDT_Rice_WriteAdRiceDc;
	ctx->WriteAdDist=BGBDT_Rice_WriteAdRiceDc;
	ctx->WriteSym=BGBDT_EmitLzSTF;

	if((ctx->lzctrl&7)!=0)
	{
		ctx->WriteAdRiceLL=BGBDT_Rice_WriteAdRiceLL;
		ctx->WriteAdDist=BGBDT_Rice_WriteAdRiceLL;
		
		if((ctx->lzctrl&7)==2)
		{
			ctx->WriteAdDist=BGBDT_Rice_WriteAdExp2Rice;
			ctx->WriteSym=BGBDT_EmitLzSMTF2;
//			ctx->WriteSym=BGBDT_EmitLzSTF3;
		}

		if((ctx->lzctrl&7)==3)
		{
			ctx->WriteAdDist=BGBDT_Rice_WriteAdExp2Rice;
			ctx->WriteSym=BGBDT_EmitLzWSTF1;
		}
	}
	
	kr=4; ks=4; kl=4; kd=4; ll=-1; ld=-1;
	cs=ibuf; cse=ibuf+ibsz; css=ibuf; csrb=cs;
	while(cs<cse)
	{
		i=BGBDT_ChunkLzLookupMatch(ctx, cs, css, cse, &l, &d);
		if(i>0)
		{
			r=cs-csrb;

			i0=BGBDT_Rice_OutBitPos(ctx);

//			BGBDT_Rice_WriteAdRiceDc(ctx, r, &kr);
			ctx->WriteAdRiceLL(ctx, r, &kr);
//			ctx->WriteAdDist(ctx, r, &kr);
			
			i1=BGBDT_Rice_OutBitPos(ctx);
			ctx->stat_kr+=i1-i0;
			ctx->stat_nr++;

			for(j=0; j<r; j++)
			{
//				BGBDT_EmitLzSTF(ctx, csrb[j], &ks);
				ctx->WriteSym(ctx, csrb[j], &ks);
			}
			i2=BGBDT_Rice_OutBitPos(ctx);
			ctx->stat_ks+=i2-i1;
			ctx->stat_ns+=r;
			

#if 1
			if(l==ll)
				{ ctx->WriteAdRiceLL(ctx, 1, &kl); }
			else
				{ ctx->WriteAdRiceLL(ctx, l, &kl); }
#endif

#if 0
			if(l==ll)
				{ ctx->WriteAdDist(ctx, 1, &kl); }
			else
				{ ctx->WriteAdDist(ctx, l, &kl); }
#endif

			i0=BGBDT_Rice_OutBitPos(ctx);
			ctx->stat_kl+=i0-i2;
			ctx->stat_nl++;

#if 0
			if(d==ld)
				{ ctx->WriteAdRiceLL(ctx, 0, &kd); }
			else
				{ ctx->WriteAdRiceLL(ctx, d, &kd); }
#endif

#if 1
			if(d==ld)
				{ ctx->WriteAdDist(ctx, 0, &kd); }
			else
				{ ctx->WriteAdDist(ctx, d, &kd); }
#endif

			i1=BGBDT_Rice_OutBitPos(ctx);
			ctx->stat_kd+=i1-i0;
			ctx->stat_nd++;


			ll=l; ld=d;
			BGBDT_ChunkLzUpdateWindow(ctx, cs, l);
			cs+=l;
			csrb=cs;
		}else
		{
			BGBDT_ChunkLzUpdateWindow(ctx, cs, 1);
			cs++;
		}
	}
	
	if(cs>csrb)
	{
		r=cs-csrb;
//		BGBDT_Rice_WriteAdRiceDc(ctx, r, &kr);
		ctx->WriteAdRiceLL(ctx, r, &kr);
//		ctx->WriteAdDist(ctx, r, &kr);
		for(j=0; j<r; j++)
		{
//			BGBDT_EmitLzSTF(ctx, csrb[j], &ks);
			ctx->WriteSym(ctx, csrb[j], &ks);
		}
//		BGBDT_Rice_WriteAdRiceDc(ctx, 0, &kl);
		ctx->WriteAdRiceLL(ctx, 0, &kl);
//		ctx->WriteAdDist(ctx, 0, &kl);
	}

//	BGBDT_Rice_WriteAdRiceDc(ctx, 0, &kr);
//	BGBDT_Rice_WriteAdRiceDc(ctx, 0, &kl);
	ctx->WriteAdRiceLL(ctx, 0, &kr);
	ctx->WriteAdRiceLL(ctx, 0, &kl);
//	ctx->WriteAdDist(ctx, 0, &kr);
//	ctx->WriteAdDist(ctx, 0, &kl);
	return(0);
}

