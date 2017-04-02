void BGBDT_EmitLzSTF(BGBDT_RiceContext *ctx, int sym, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;
	
	ix0=ctx->lzidx[sym];
	ix1=(ix0*7)/8;
	sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
	ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;
	
//	BGBDT_Rice_WriteAdRiceDc(ctx, ix0, rk);
	ctx->WriteAdRiceLL(ctx, ix0, rk);
//	return(0);
}

void BGBDT_EmitLzSTF3(BGBDT_RiceContext *ctx, int sym, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;

//	ctx->lzstat[sym]++;
	
	ix0=ctx->lzidx[sym];
	ix1=(ix0*(57344-ix0*68))>>16;
	sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
	ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;
	
	ctx->WriteAdRiceLL(ctx, ix0, rk);
}

void BGBDT_EmitLzSMTF2(BGBDT_RiceContext *ctx, int sym, int *rk)
{
	int ix0, ix1, ix2, ix3;
	int sy0, sy1;

	ix0=ctx->lzidx[sym];
	ix0=(byte)(ix0-ctx->lzwpos);
	
	if(ix0<72)
	{
		ix1=(ix0*(57344-ix0*72))>>16;
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
}

int BGBDT_ChunkLzHashHi(BGBDT_RiceContext *ctx, byte *ics)
{
	int h;
	h=((ics[0]*251^ics[1])*251^ics[2])*251;
//	h=(h>>9)&255;
	h=(h>>9)&(ctx->lzhm);

	return(h);
}

#if 0
void BGBDT_ChunkLzHashChi(BGBDT_RiceContext *ctx,
	byte *ics, int *chi)
{
	int h, ix;

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
	h=(h>>9)&255;
	chi[0]=(h<<3)|0;
	chi[1]=(h<<3)|1;
	chi[2]=(h<<3)|2;
	chi[3]=(h<<3)|3;
	chi[4]=(h<<3)|4;
	chi[5]=(h<<3)|5;
	chi[6]=(h<<3)|6;
	chi[7]=(h<<3)|7;
#endif

#if 0
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

#if 1
	h=(h>>9)&255;
	ix=ctx->lzhix[h];
	chi[ 0]=(h<<4)|((ix+ 0)&15);
	chi[ 1]=(h<<4)|((ix+ 1)&15);
	chi[ 2]=(h<<4)|((ix+ 2)&15);
	chi[ 3]=(h<<4)|((ix+ 3)&15);
	if(ctx->lzsd>4)
	{
		chi[ 4]=(h<<4)|((ix+ 4)&15);
		chi[ 5]=(h<<4)|((ix+ 5)&15);
		chi[ 6]=(h<<4)|((ix+ 6)&15);
		chi[ 7]=(h<<4)|((ix+ 7)&15);

		if(ctx->lzsd>8)
		{
			chi[ 8]=(h<<4)|((ix+ 8)&15);
			chi[ 9]=(h<<4)|((ix+ 9)&15);
			chi[10]=(h<<4)|((ix+10)&15);
			chi[11]=(h<<4)|((ix+11)&15);
			chi[12]=(h<<4)|((ix+12)&15);
			chi[13]=(h<<4)|((ix+13)&15);
			chi[14]=(h<<4)|((ix+14)&15);
			chi[15]=(h<<4)|((ix+15)&15);
		}
	}
#endif
}
#endif

int BGBDT_ChunkLzLookupMatch(BGBDT_RiceContext *ctx,
	byte *ics, byte *icss, byte *icse, int *rbl, int *rbd)
{
//	int chi[16];
	byte *s0, *s1, *s2, *s3, *se;
	int hi, l, d, bl, bd, bi;
	int i, j, k, sd, rsd;

	if((ics+5)>=icse)
		return(0);
	
//	hi=((ics[0]*251+ics[1])*251+ics[2])*251;
//	hi=((ics[0]*251^ics[1])*251^ics[2])*251;
//	hi=(hi>>8)&255;
	
//	BGBDT_ChunkLzHashChi(ctx, ics, chi);
	hi=BGBDT_ChunkLzHashHi(ctx, ics);
	
	bl=0; bd=0;
	sd=ctx->lzsd;
	rsd=ctx->lzrsd;
	
//	for(i=1; i<6; i++)
	for(i=1; i<rsd; i++)
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
//	for(i=0; i<4; i++)
//	for(i=0; i<8; i++)
//	for(i=0; i<16; i++)
	for(i=0; i<sd; i++)
	{
		s0=ics; se=s0+256;
		if(icse<se)se=icse;
//		s1=ctx->lzhash[(i<<8)|hi];
//		s1=ctx->lzhash[chi[i]];
		j=(hi<<ctx->lzsd2)|((ctx->lzhix[hi]+i)&ctx->lzixm);

		if(j>>14)
			{ FRGL_DBGBREAK }

		s1=ctx->lzhash[j];
		if(s1)
		{
			if((s1<icss) || (s1>=icse))
			{
				FRGL_DBGBREAK_SOFT
				continue;
			}

			while((s0<se) && (*s0==*s1))
				{ s0++; s1++; }
			l=s0-ics;
			d=s0-s1;
			if(l>bl)
				{ bl=l; bd=d; bi=i; }
		}
	}
#endif

	*rbl=bl;
	*rbd=bd;
	return(bl>=3);
}

int BGBDT_ChunkLzUpdateWindow(BGBDT_RiceContext *ctx, byte *ics, int len)
{
//	int chi[16];
	byte *cs, *cse;
	int hi, hj;
	
	cs=ics; cse=cs+len;
	while(cs<cse)
	{
//		hi=((cs[0]*251+cs[1])*251+cs[2])*251;
//		hi=((cs[0]*251^cs[1])*251^cs[2])*251;
//		hi=(hi>>8)&255;

//		BGBDT_ChunkLzHashChi(ctx, cs, chi);
//		ctx->lzhash[chi[15]]=cs;
//		ctx->lzhix[(chi[15]>>4)&255]--;

//		hi=(chi[15]>>4)&255;
//		ctx->lzhix[hi]--;
//		hj=(hi<<4)|(ctx->lzhix[hi]&15);
//		ctx->lzhash[hj]=cs;

		hi=BGBDT_ChunkLzHashHi(ctx, cs);
		ctx->lzhix[hi]--;
		hj=(hi<<ctx->lzsd2)|((ctx->lzhix[hi]+0)&ctx->lzixm);
		if(hj>>14)
			{ FRGL_DBGBREAK }
		ctx->lzhash[hj]=cs;

#if 0
		ctx->lzhash[chi[15]]=ctx->lzhash[chi[14]];
		ctx->lzhash[chi[14]]=ctx->lzhash[chi[13]];
		ctx->lzhash[chi[13]]=ctx->lzhash[chi[12]];
		ctx->lzhash[chi[12]]=ctx->lzhash[chi[11]];
		ctx->lzhash[chi[11]]=ctx->lzhash[chi[10]];
		ctx->lzhash[chi[10]]=ctx->lzhash[chi[ 9]];
		ctx->lzhash[chi[ 9]]=ctx->lzhash[chi[ 8]];
		ctx->lzhash[chi[ 8]]=ctx->lzhash[chi[ 7]];
#endif

#if 0
		ctx->lzhash[chi[7]]=ctx->lzhash[chi[6]];
		ctx->lzhash[chi[6]]=ctx->lzhash[chi[5]];
		ctx->lzhash[chi[5]]=ctx->lzhash[chi[4]];
		ctx->lzhash[chi[4]]=ctx->lzhash[chi[3]];
#endif

#if 0
		ctx->lzhash[chi[3]]=ctx->lzhash[chi[2]];
		ctx->lzhash[chi[2]]=ctx->lzhash[chi[1]];
		ctx->lzhash[chi[1]]=ctx->lzhash[chi[0]];
		ctx->lzhash[chi[0]]=cs;

//		ctx->lzhash[768|hi]=ctx->lzhash[512|hi];
//		ctx->lzhash[512|hi]=ctx->lzhash[256|hi];
//		ctx->lzhash[256|hi]=ctx->lzhash[hi];
//		ctx->lzhash[hi]=cs;
#endif

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

	ctx->lzrsd=6;
	ctx->lzsd2=4;
	ctx->lzsd=1<<ctx->lzsd2;
	ctx->lzhs2=12-ctx->lzsd2;
	ctx->lzixm=(1<<ctx->lzsd2)-1;
	ctx->lzhm=(1<<ctx->lzhs2)-1;
	
	for(i=0; i<256; i++)
		{ ctx->lzwin[i]=i; ctx->lzidx[i]=i; }
	ctx->lzwpos=0;

//	for(i=0; i<256; i++)
//	for(i=0; i<2048; i++)
	for(i=0; i<4096; i++)
		{ ctx->lzhash[i]=NULL; }
	for(i=0; i<256; i++)
		{ ctx->lzhix[i]=0; }

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
	}
	
	kr=4; ks=4; kl=4; kd=4; ll=-1; ld=-1;
	cs=ibuf; cse=ibuf+ibsz; css=ibuf; csrb=cs;
	while(cs<cse)
	{
		i=BGBDT_ChunkLzLookupMatch(ctx, cs, css, cse, &l, &d);
		if(i>0)
		{
			r=cs-csrb;
//			BGBDT_Rice_WriteAdRiceDc(ctx, r, &kr);
			ctx->WriteAdRiceLL(ctx, r, &kr);
			for(j=0; j<r; j++)
			{
//				BGBDT_EmitLzSTF(ctx, csrb[j], &ks);
				ctx->WriteSym(ctx, csrb[j], &ks);
			}

#if 0
			if(l==ll)
				{ BGBDT_Rice_WriteAdRiceDc(ctx, 1, &kl); }
			else
				{ BGBDT_Rice_WriteAdRiceDc(ctx, l, &kl); }

			if(d==ld)
				{ BGBDT_Rice_WriteAdRiceDc(ctx, 0, &kd); }
			else
				{ BGBDT_Rice_WriteAdRiceDc(ctx, d, &kd); }
#endif

#if 1
			if(l==ll)
				{ ctx->WriteAdRiceLL(ctx, 1, &kl); }
			else
				{ ctx->WriteAdRiceLL(ctx, l, &kl); }

//			if(d==ld)
//				{ ctx->WriteAdRiceLL(ctx, 0, &kd); }
//			else
//				{ ctx->WriteAdRiceLL(ctx, d, &kd); }
#endif

#if 1
			if(d==ld)
				{ ctx->WriteAdDist(ctx, 0, &kd); }
			else
				{ ctx->WriteAdDist(ctx, d, &kd); }
#endif

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
		for(j=0; j<r; j++)
		{
//			BGBDT_EmitLzSTF(ctx, csrb[j], &ks);
			ctx->WriteSym(ctx, csrb[j], &ks);
		}
//		BGBDT_Rice_WriteAdRiceDc(ctx, 0, &kl);
		ctx->WriteAdRiceLL(ctx, 0, &kl);
	}

//	BGBDT_Rice_WriteAdRiceDc(ctx, 0, &kr);
//	BGBDT_Rice_WriteAdRiceDc(ctx, 0, &kl);
	ctx->WriteAdRiceLL(ctx, 0, &kr);
	ctx->WriteAdRiceLL(ctx, 0, &kl);
	return(0);
}

int BGBDT_WorldEncodeChunkBits(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk, byte *obuf, int obsz, int accfl)
{
	BGBDT_RiceContext *ctx;
	byte tpbuf[1<<15];
//	byte *tpbuf;
	byte *cs, *ct;
	int ts, tp, tpn, tpsz, vstr, sz;
	int i, j, k;

	BGBDT_WorldUpdateChunkSolid(world, chk);

	ts=chk->nvoxinfo;	
	tp=15;

	vstr=sizeof(BGBDT_VoxData);

	cs=(byte *)chk->voxinfo;

	tp=0;
	for(i=0; i<vstr; i++)
	{
		for(j=0; j<ts; j++)
		{
			if(cs[j*vstr+i])
				{ tp|=(1<<i); break; }
		}
	}

	tpn=0;
	for(i=0; i<16; i++)
		if(tp&(1<<i))tpn++;
	tpsz=tpn*ts;

	if(tpsz>0)
	{
//		tpbuf=BGBDT_WorldAllocVoxelTemp(world, tpsz);

		cs=(byte *)chk->voxinfo; ct=tpbuf;
		for(i=0; i<vstr; i++)
		{
			if(!(tp&(1<<i)))
				continue;
			for(j=0; j<ts; j++)
				{ ct[j]=cs[j*vstr+i]; }
			ct+=ts;
		}
	}

	ctx=BGBDT_Rice_AllocContext();
	
	ctx->lzctrl=(ctx->lzctrl&(~7))|2;
	
	BGBDT_Rice_SetupWrite(ctx, obuf, obsz);

	BGBDT_Rice_WriteRice(ctx, 1, 3);
	BGBDT_Rice_WriteRice(ctx, ts, 7);

	BGBDT_Rice_WriteRice(ctx, 2, 3);
	BGBDT_Rice_WriteRice(ctx, tp, 7);

	if(ctx->lzctrl)
	{
		BGBDT_Rice_WriteRice(ctx, 3, 3);
		BGBDT_Rice_WriteRice(ctx, 2, 3);
		BGBDT_Rice_WriteQExpBase(ctx, ctx->lzctrl, 5);
	}

	if(chk->voxidx)
	{
		BGBDT_Rice_WriteRice(ctx, 4, 3);
		BGBDT_EncodeChunkLZ(ctx, chk->voxidx, 4096);
	}

	if(chk->voxidxh)
	{
		BGBDT_Rice_WriteRice(ctx, 5, 3);
		BGBDT_EncodeChunkLZ(ctx, chk->voxidxh, 2048);
	}

	if(tpsz>0)
	{
		BGBDT_Rice_WriteRice(ctx, 6, 3);
		BGBDT_EncodeChunkLZ(ctx, tpbuf, tpsz);

//		BGBDT_WorldFreeVoxelTemp(world, tpbuf, tpsz);
	}

	i=chk->flags;
//	i&=~(BGBDT_CHKFL_DIRTY|BGBDT_CHKFL_MESHDIRTY|BGBDT_CHKFL_SAVEDIRTY);
	i&=~BGBDT_CHKFL_ALLDIRTY;
	i&=~BGBDT_CHKFL_ENTSPAWN;
	
	if(i)
	{
		BGBDT_Rice_WriteRice(ctx, 3, 3);
		BGBDT_Rice_WriteRice(ctx, 1, 3);
		BGBDT_Rice_WriteQExpBase(ctx, i, 5);
	}

	BGBDT_Rice_WriteRice(ctx, 0, 3);
	
	sz=BGBDT_Rice_FlushBits(ctx);
	BGBDT_Rice_FreeContext(ctx);
	return(sz);
}

int BGBDT_WorldEncodeChunkBuffer(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk, byte *obuf, int obsz, int accfl)
{
	byte *ct, *cte;
	int sz;
	int i, j, k, l;
	
	ct=obuf; cte=ct+obsz;

	sz=BGBDT_WorldEncodeChunkBits(world, chk, ct+16, cte-(ct+16), accfl);
	if(sz>=252)
	{
		k=sz+4;
		ct[0]=0xE1;		ct[1]=k>>16;
		ct[2]=k>> 8;	ct[3]=k    ;
		memmove(ct+4, ct+16, sz);
		ct+=k;
	}else
	{
		k=sz+2;
		ct[0]=0xE2;		ct[1]=k;
		memmove(ct+2, ct+16, sz);
		ct+=k;
	}
	
	return(ct-obuf);
}

int BGBDT_RegionCellGetTag(BGBDT_VoxRegion *rgn, int idx)
{
	int i, j, k;
	
	if((idx<0) || (idx>=rgn->nrgncell))
		FRGL_DBGBREAK
	
	i=(rgn->rgnmap[idx>>2]>>((idx&3)<<1))&3;
	return(i);
}

int BGBDT_RegionCellSetTag(BGBDT_VoxRegion *rgn, int idx, int val)
{
	int i, j, k;

	if((idx<0) || (idx>=rgn->nrgncell))
		FRGL_DBGBREAK

	rgn->rgnmap[idx>>2]&=~(3<<((idx&3)<<1));
	rgn->rgnmap[idx>>2]|=(val&3)<<((idx&3)<<1);
	return(0);
}

int BGBDT_RegionFindFreeCellData(BGBDT_VoxRegion *rgn, int len)
{
	int i, j, k, l, nc;
	
	if(!rgn->rgnmap)
		return(-1);
	
	nc=rgn->nrgncell;
	i=0;
	while(i<nc)
	{
		j=BGBDT_RegionCellGetTag(rgn, i);
		if(j)
		{
			while(i<nc)
			{
				j=BGBDT_RegionCellGetTag(rgn, i);
				if(!j)break;
				i++;
			}
			continue;
		}
		
		l=i+len;
		if(l>nc)
			break;
		for(j=i; j<l; j++)
		{
			k=BGBDT_RegionCellGetTag(rgn, j);
			if(k)break;
		}

		if(j>=l)
			return(i);
		i=j+1;
	}
	return(-1);
}

int BGBDT_RegionExpandCellData(BGBDT_VoxRegion *rgn, int len)
{
	int nc, nc1, nl;
	int nb, nb1;

	nc=rgn->nrgncell;
	nc1=nc+(nc>>2);
	nl=nc+len;
	while(nc1<=nl)
		nc1=nc1+(nc1>>2);
	
	nc1=((nc1+15)>>4)<<4;
	
	nb=(nc+3)>>2;
	nb1=(nc1+3)>>2;
	
	rgn->rgnbuf=frgl_realloc(rgn->rgnbuf, (nc1<<BGBDT_RGN_CELLSHR)+4096);
	rgn->rgnmap=frgl_realloc(rgn->rgnmap, nb1+256);
	memset(rgn->rgnmap+nb, 0, nb1-nb);
	
	rgn->szrgnbuf=nc1<<BGBDT_RGN_CELLSHR;
	rgn->nrgncell=nc1;
	return(0);
}

int BGBDT_RegionAllocCellData(BGBDT_VoxRegion *rgn, int sz)
{
	int i, j, k, n;
	
	n=(sz+BGBDT_RGN_CELLPAD)>>BGBDT_RGN_CELLSHR;
	i=BGBDT_RegionFindFreeCellData(rgn, n);
	if(i>=0)
	{
		BGBDT_RegionCellSetTag(rgn, i, 1);
		memset(rgn->rgnbuf+(i<<BGBDT_RGN_CELLSHR),
			0, 1<<BGBDT_RGN_CELLSHR);

		for(j=1; j<n; j++)
		{
			BGBDT_RegionCellSetTag(rgn, i+j, 2);
			memset(rgn->rgnbuf+((i+j)<<BGBDT_RGN_CELLSHR),
				0, 1<<BGBDT_RGN_CELLSHR);
		}
		return(i);
	}
	
	BGBDT_RegionExpandCellData(rgn, n);
	return(BGBDT_RegionAllocCellData(rgn, sz));
}

int BGBDT_RegionFreeCellData(BGBDT_VoxRegion *rgn, int ix)
{
	int i, j, k;

	j=BGBDT_RegionCellGetTag(rgn, ix);
	if(j!=1)
		return(-1);

	BGBDT_RegionCellSetTag(rgn, ix, 0);
	memset(rgn->rgnbuf+(ix<<BGBDT_RGN_CELLSHR),
		0, 1<<BGBDT_RGN_CELLSHR);
	for(i=ix+1; i<rgn->nrgncell; i++)
	{
		j=BGBDT_RegionCellGetTag(rgn, i);
		if(j!=2)break;
		BGBDT_RegionCellSetTag(rgn, i, 0);
		memset(rgn->rgnbuf+(i<<BGBDT_RGN_CELLSHR),
			0, 1<<BGBDT_RGN_CELLSHR);
	}
	return(0);
}

int BGBDT_WorldEncodeChunk(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, BGBDT_VoxChunk *chk, int accfl)
{
	byte tmpbuf[1<<16];
	int ix, sz, ofs;
	
	sz=BGBDT_WorldEncodeChunkBuffer(world, chk, tmpbuf, 1<<16, accfl);

	ix=BGBDT_RegionAllocCellData(rgn, sz);
	ofs=ix<<BGBDT_RGN_CELLSHR;
	
	memcpy(rgn->rgnbuf+ofs, tmpbuf, sz);
	return(ofs);
}

int BGBDT_WorldSaveRegionData(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	char tbuf[256];
	BGBDT_VoxChunk *chk;
	byte *tobuf;
	byte *ct;
	int bid;
	int bx, by, bz, bn;
	int n, sz0, sz1, sz2, sz3;
	int i, j, k;

#ifdef __EMSCRIPTEN__
	return(0);
#endif

#if 0
	if(rgn->szrgnbuf>(6<<20) && (rgn->rgnmap[12288]==0xFF))
	{
		for(bz=0; bz<16; bz++)
			for(by=0; by<16; by++)
				for(bx=0; bx<16; bx++)
		{
			chk=BGBDT_GetRegionChunk(world, rgn, bx, by, bz, 0);
			if(chk)
				chk->flags|=BGBDT_CHKFL_SAVEDIRTY;

			bn=(bz*BGBDT_XYZ_TSZ_CHK_XY)+(by*BGBDT_XYZ_SZ_CHUNK_XYZ)+bx;
			rgn->chkofs[bn]=0;
		}
		
		frgl_free(rgn->rgnbuf);
		frgl_free(rgn->rgnmap);
		rgn->rgnbuf=NULL;
		rgn->rgnmap=NULL;
	}
#endif
	
	if(!rgn->rgnbuf)
	{
//		sz0=1<<22;
		sz0=1<<20;
		rgn->rgnbuf=frgl_malloc(sz0);
		rgn->szrgnbuf=sz0;
		rgn->nrgncell=sz0>>BGBDT_RGN_CELLSHR;
		rgn->rgnmap=frgl_malloc((rgn->nrgncell+3)/4);
		
		k=262144/64;
		for(i=0; i<k; i++)
		{
			BGBDT_RegionCellSetTag(rgn, i, 3);
		}
	}

	n=BGBDT_XYZ_TSZ_RGN_XYZ;
	for(i=0; i<n; i++)
	{
		chk=rgn->chkptr[i];
		if(!chk)
			continue;

		if(!(chk->flags&BGBDT_CHKFL_SAVEDIRTY) && rgn->chkofs[i])
			continue;

		j=rgn->chkofs[i];
		if(j)
		{
			BGBDT_RegionFreeCellData(rgn, j>>BGBDT_RGN_CELLSHR);
			rgn->chkofs[i]=0;
		}
		
		j=BGBDT_WorldEncodeChunk(world, rgn, chk, 0);
		rgn->chkofs[i]=j;
		chk->flags&=~BGBDT_CHKFL_SAVEDIRTY;
		
//		printf("Saved Chk %p %04X\n", chk, chk->flags);
		
		if(!j)
		{
			bid=bgbdt_xyz2rgnid(rgn->bx, rgn->by, rgn->bz);
			printf("BGBDT_WorldSaveRegionData: Fail Save %d,%d,%d Rgn=%08F\n",
				chk->bx, chk->by, chk->bz, bid);
		}
	}

#if 1
	for(i=0; i<rgn->nrgncell; i++)
	{
		j=BGBDT_RegionCellGetTag(rgn, i);
		if(!j)
		{
//			BGBDT_RegionCellSetTag(rgn, i, 0);
			memset(rgn->rgnbuf+(i<<BGBDT_RGN_CELLSHR),
				0, 1<<BGBDT_RGN_CELLSHR);
		}
	}
#endif
	
	ct=rgn->rgnbuf;
	ct[0]=0xE4;
	ct[4]='R';	ct[5]='G';
	ct[6]='N';	ct[7]='0';
	ct+=8;

	n=BGBDT_XYZ_TSZ_RGN_XYZ;
	if(rgn->szrgnbuf>=(1<<24))
	{
		sz0=8+n*4;
		ct[0]=0xE4;		ct[1]=sz0>>16;
		ct[2]=sz0>> 8;	ct[3]=sz0    ;
		ct[4]='O';		ct[5]='F';
		ct[6]='S';		ct[7]='4';
		
		for(i=0; i<n; i++)
		{
			j=rgn->chkofs[i];
			ct[8+i*4+0]=j>>24;
			ct[8+i*4+1]=j>>16;
			ct[8+i*4+2]=j>> 8;
			ct[8+i*4+3]=j    ;
		}
		ct+=sz0;
	}else
	{
		sz0=8+n*3;
		ct[0]=0xE4;		ct[1]=sz0>>16;
		ct[2]=sz0>> 8;	ct[3]=sz0    ;
		ct[4]='O';	ct[5]='F';
		ct[6]='S';	ct[7]='3';
		
		for(i=0; i<n; i++)
		{
			j=rgn->chkofs[i];
			ct[8+i*3+0]=j>>16;
			ct[8+i*3+1]=j>> 8;
			ct[8+i*3+2]=j    ;
		}
		ct+=sz0;
	}

	sz0=8+((rgn->nrgncell+3)/4);
	ct[0]=0xE4;		ct[1]=sz0>>16;
	ct[2]=sz0>> 8;	ct[3]=sz0    ;
	ct[4]='C';		ct[5]='E';
	ct[6]='B';		ct[7]='M';
	memcpy(ct+8, rgn->rgnmap, sz0-8);
	ct+=sz0;

	if(rgn->strtab_buf)
	{
		sz0=8+((rgn->strtab_nsz+3)/4)*4;
		ct[0]=0xE4;		ct[1]=sz0>>16;
		ct[2]=sz0>> 8;	ct[3]=sz0    ;
		ct[4]='S';		ct[5]='T';
		ct[6]='R';		ct[7]='S';
		memcpy(ct+8, rgn->strtab_buf, sz0-8);
		ct+=sz0;
		
		n=rgn->strix_num;
		
		if(rgn->strtab_nsz>65530)
		{
			sz0=8+(rgn->strix_num*3);
			ct[0]=0xE4;		ct[1]=sz0>>16;
			ct[2]=sz0>> 8;	ct[3]=sz0    ;
			ct[4]='S';		ct[5]='T';
			ct[6]='I';		ct[7]='3';
			for(i=0; i<n; i++)
			{
				j=rgn->strix_ofs[i];
				ct[8+i*3+0]=j>>16;
				ct[8+i*3+1]=j>> 8;
				ct[8+i*3+2]=j    ;
			}
			ct+=sz0;
		}else
		{
			sz0=8+(rgn->strix_num*2);
			ct[0]=0xE4;		ct[1]=sz0>>16;
			ct[2]=sz0>> 8;	ct[3]=sz0    ;
			ct[4]='S';		ct[5]='T';
			ct[6]='I';		ct[7]='2';
			for(i=0; i<n; i++)
			{
				j=rgn->strix_ofs[i];
				ct[8+i*2+0]=j>> 8;
				ct[8+i*2+1]=j    ;
			}
			ct+=sz0;
		}
	}

	sz1=ct-rgn->rgnbuf;
	ct=rgn->rgnbuf;
	ct[0]=0xE4;		ct[1]=sz1>>16;
	ct[2]=sz1>> 8;	ct[3]=sz1    ;

	bid=bgbdt_xyz2rgnid(rgn->bx, rgn->by, rgn->bz);
	sprintf(tbuf, "region/%s/%08X.rgn", world->worldname, bid);

	printf("Saving Region %08X\n", bid);

#if 1
	tobuf=bgbdt_voxrgn_enctobuf;
	sz3=bgbdt_voxrgn_szenctobuf;

	sz2=rgn->szrgnbuf*1.25;

	if(!tobuf || (sz2>sz3))
	{
		frgl_free(tobuf);
		tobuf=frgl_malloc(sz2);
		bgbdt_voxrgn_szenctobuf=sz2;
	}
	
//	sz3=BGBDT_WorldEncodeRegionLZ4(world, rgn,
//		rgn->rgnbuf, rgn->szrgnbuf, tobuf, sz2);

//	sz3=BGBDT_WorldEncodeRegionBLZ(world, rgn,
//		rgn->rgnbuf, rgn->szrgnbuf, tobuf, sz2);

	sz3=BGBDT_WorldEncodeRegionFeLz32(world, rgn,
		rgn->rgnbuf, rgn->szrgnbuf, tobuf, sz2);

	if(sz3>0)
	{
		vf_storefile(tbuf, tobuf, sz3);
	}else
	{
		vf_storefile(tbuf, rgn->rgnbuf, rgn->szrgnbuf);
	}
//	frgl_free(tobuf);
#else
	vf_storefile(tbuf, rgn->rgnbuf, rgn->szrgnbuf);
#endif

	return(0);
}


byte *BGBDT_EncLZ4_EmitMod255(byte *ct, int val)
{
	int v;
	
	v=val;
	while(v>=255)
	{
		*ct++=0xFF;
		v-=255;
	}
	*ct++=v;
	return(ct);
}

int BGBDT_EncodeRegionBufLZ4(BGBDT_RiceContext *ctx,
	byte *ibuf, int ibsz, byte *obuf, int obsz)
{
	byte *ct, *cte, *cs, *css, *cse, *csrb;
	int kr, ks, kl, kd;
	int l, d, r, tk, l1;
	int i0, i1, i2, i3;
	int i, j, k;
	
	for(i=0; i<4096; i++)
		{ ctx->lzhash[i]=NULL; }
	for(i=0; i<1024; i++)
		{ ctx->lzhix[i]=0; }

	ctx->lzrsd=2;
	ctx->lzsd2=1;
	ctx->lzsd=1<<ctx->lzsd2;
	ctx->lzhs2=14-ctx->lzsd2;
	ctx->lzixm=(1<<ctx->lzsd2)-1;
	ctx->lzhm=(1<<ctx->lzhs2)-1;

	ct=obuf; cte=obuf+obsz;
	cs=ibuf; cse=ibuf+ibsz; css=ibuf; csrb=cs;
	while((cs<cse) && (ct<cte))
	{
		i=BGBDT_ChunkLzLookupMatch(ctx, cs, css, cse, &l, &d);
		if((i>0) && (l>=4) && (d>0) && (d<65536))
		{
			r=cs-csrb;
			
			l1=l-4;
			tk=(r>=15)?0xF0:(r<<4);
			tk|=(l1>=15)?0x0F:l1;
			*ct++=tk;

			if(r>=15)
				{ ct=BGBDT_EncLZ4_EmitMod255(ct, r-15); }
			
			for(j=0; j<r; j++)
				{ *ct++=csrb[j]; }

			if((cs-d)<ibuf)
				{ FRGL_DBGBREAK_SOFT }

			*ct++=d;
			*ct++=d>>8;

			if(l1>=15)
				{ ct=BGBDT_EncLZ4_EmitMod255(ct, l1-15); }
			
			BGBDT_ChunkLzUpdateWindow(ctx, cs, l);
			cs+=l;
			csrb=cs;
		}else
		{
			BGBDT_ChunkLzUpdateWindow(ctx, cs, 1);
			cs++;
		}
	}
	
	if(ct>=cte)
	{
		FRGL_DBGBREAK_SOFT
	}
	
	if(cs>csrb)
	{
		r=cs-csrb;

		tk=(r>=15)?0xF0:(r<<4);
		*ct++=tk;

		if(r>=15)
			{ ct=BGBDT_EncLZ4_EmitMod255(ct, r-15); }

		for(j=0; j<r; j++)
			{ *ct++=csrb[j]; }
		*ct++=0x00;
		*ct++=0x00;
	}else
	{
		*ct++=0x00;
		*ct++=0x00;
		*ct++=0x00;
	}

	return(ct-obuf);
}

int BGBDT_WorldEncodeRegionLZ4(
	BGBDT_VoxWorld *world, BGBDT_VoxRegion *rgn,
	byte *ibuf, int ibsz, byte *obuf, int obsz)
{
	BGBDT_RiceContext *ctx;
	byte *tbuf;
	byte *cs, *ct;
	int sz, tsz;
	int i, j, k;

	ctx=BGBDT_Rice_AllocContext();

	i=BGBDT_EncodeRegionBufLZ4(ctx, ibuf, ibsz, obuf+20, obsz-20-1024);
	BGBDT_Rice_FreeContext(ctx);
	if(i<0)return(i);
	
	j=ibsz;
	k=i+4;
	obuf[ 0]=0xE4;		obuf[ 1]=0x00;
	obuf[ 2]=0x00;		obuf[ 3]=0x10;
	obuf[ 4]='R';		obuf[ 5]='G';
	obuf[ 6]='L';		obuf[ 7]='Z';
	obuf[ 8]=j>>24;		obuf[ 9]=j>>16;
	obuf[10]=j>> 8;		obuf[11]=j    ;
	obuf[12]=0x04;		obuf[13]=0x00;
	obuf[14]=0x00;		obuf[15]=0x00;
	obuf[16]=0xE1;		obuf[17]=k>>16;
	obuf[18]=k>> 8;		obuf[19]=k    ;
	sz=20+i;

#if 1
	tbuf=NULL; tsz=0;
	i=BGBDT_VoxRgn_UnpackRgnLz(world, obuf, sz, &tbuf, &tsz);
	if((i<0) || !tbuf)
	{
		printf("BGBDT_WorldEncodeRegionLZ4: Fail: Decode Status=%d\n", i);
		FRGL_DBGBREAK_SOFT
		return(i);
	}
	
	if(ibsz!=tsz)
	{
		printf("BGBDT_WorldEncodeRegionLZ4: Fail: Size Mismatch\n");
		FRGL_DBGBREAK_SOFT

		if(tbuf)
			frgl_free(tbuf);
		return(-1);
	}
	
	if(memcmp(ibuf, tbuf, tsz)!=0)
	{
		printf("BGBDT_WorldEncodeRegionLZ4: Fail: Data Mismatch\n");

		for(i=0; i<tsz; i++)
			if(ibuf[i]!=tbuf[i])
				break;

		FRGL_DBGBREAK_SOFT

		frgl_free(tbuf);
		return(-1);
	}
	frgl_free(tbuf);
#endif

	printf("BGBDT_WorldEncodeRegionLZ4: OK %p (%d->%d, %.2f%%)\n",
		rgn, ibsz, sz, (100.0*sz)/ibsz);
	return(sz);
}

int BGBDT_WorldEncodeRegionBLZ(
	BGBDT_VoxWorld *world, BGBDT_VoxRegion *rgn,
	byte *ibuf, int ibsz, byte *obuf, int obsz)
{
	BGBDT_RiceContext *ctx;
	byte *tbuf;
	byte *cs, *ct;
	int sz, tsz, bid;
	int i, j, k;

	i=BTLZA_BitEncF_EncodeStreamXLvlZl(
		ibuf, obuf+20, ibsz, obsz-20-1024, 1);
	if(i<0)return(i);
	
	j=ibsz;
	k=i+4;
	obuf[ 0]=0xE4;		obuf[ 1]=0x00;
	obuf[ 2]=0x00;		obuf[ 3]=0x10;
	obuf[ 4]='R';		obuf[ 5]='G';
	obuf[ 6]='L';		obuf[ 7]='Z';
	obuf[ 8]=j>>24;		obuf[ 9]=j>>16;
	obuf[10]=j>> 8;		obuf[11]=j    ;
	obuf[12]=0x0A;		obuf[13]=0x00;
	obuf[14]=0x00;		obuf[15]=0x00;
	obuf[16]=0xE1;		obuf[17]=k>>16;
	obuf[18]=k>> 8;		obuf[19]=k    ;
	sz=20+i;

#if 0
	tbuf=NULL; tsz=0;
	i=BGBDT_VoxRgn_UnpackRgnLz(world, obuf, sz, &tbuf, &tsz);
	if((i<0) || !tbuf)
	{
		printf("BGBDT_WorldEncodeRegionBLZ: Fail: Decode Status=%d\n", i);
		FRGL_DBGBREAK_SOFT
		return(i);
	}
	
	if(ibsz!=tsz)
	{
		printf("BGBDT_WorldEncodeRegionBLZ: Fail: Size Mismatch\n");
		FRGL_DBGBREAK_SOFT

		if(tbuf)
			frgl_free(tbuf);
		return(-1);
	}
	
	if(memcmp(ibuf, tbuf, tsz)!=0)
	{
		printf("BGBDT_WorldEncodeRegionBLZ: Fail: Data Mismatch\n");

		for(i=0; i<tsz; i++)
			if(ibuf[i]!=tbuf[i])
				break;

		FRGL_DBGBREAK_SOFT

		frgl_free(tbuf);
		return(-1);
	}
	frgl_free(tbuf);
#endif

	bid=bgbdt_xyz2rgnid(rgn->bx, rgn->by, rgn->bz);
	printf("BGBDT_WorldEncodeRegionBLZ: OK %08X (%d->%d, %.2f%%)\n",
		bid, ibsz, sz, (100.0*sz)/ibsz);
	return(sz);
}


int BGBDT_WorldEncodeRegionFeLz32(
	BGBDT_VoxWorld *world, BGBDT_VoxRegion *rgn,
	byte *ibuf, int ibsz, byte *obuf, int obsz)
{
	byte *tbuf;
	byte *cs, *ct;
	int t0, t1, t2;
	int sz, tsz;
	int i, j, k;

	t0=FRGL_TimeMS();
	i=BGBDT_FeLz32_EncodeBuffer(
		(u32 *)ibuf, (ibsz+3)/4,
		(u32 *)(obuf+20), (obsz-20-1024)/4);
	if(i<0)return(i);
	t1=FRGL_TimeMS();
	
	t2=t1-t0;
	
	i=i*4;
	
	j=ibsz;
	k=i+4;
	obuf[ 0]=0xE4;		obuf[ 1]=0x00;
	obuf[ 2]=0x00;		obuf[ 3]=0x10;
	obuf[ 4]='R';		obuf[ 5]='G';
	obuf[ 6]='L';		obuf[ 7]='Z';
	obuf[ 8]=j>>24;		obuf[ 9]=j>>16;
	obuf[10]=j>> 8;		obuf[11]=j    ;
	obuf[12]=0x03;		obuf[13]=0x00;
	obuf[14]=0x00;		obuf[15]=0x00;
	obuf[16]=0xE1;		obuf[17]=k>>16;
	obuf[18]=k>> 8;		obuf[19]=k    ;
	sz=20+i;

#if 1
	tbuf=NULL; tsz=0;
	i=BGBDT_VoxRgn_UnpackRgnLz(world, obuf, sz, &tbuf, &tsz);
	if((i<0) || !tbuf)
	{
		printf("BGBDT_WorldEncodeRegionFeLz32: Fail: Decode Status=%d\n", i);
		FRGL_DBGBREAK_SOFT
		return(i);
	}
	
	if(ibsz!=tsz)
	{
		printf("BGBDT_WorldEncodeRegionFeLz32: Fail: Size Mismatch\n");
		FRGL_DBGBREAK_SOFT

		if(tbuf)
			frgl_free(tbuf);
		return(-1);
	}
	
	if(memcmp(ibuf, tbuf, tsz)!=0)
	{
		printf("BGBDT_WorldEncodeRegionFeLz32: Fail: Data Mismatch\n");

		for(i=0; i<tsz; i++)
			if(ibuf[i]!=tbuf[i])
				break;

		FRGL_DBGBREAK_SOFT

		frgl_free(tbuf);
		return(-1);
	}
	frgl_free(tbuf);
#endif

	printf("BGBDT_WorldEncodeRegionFeLz32: OK %p (%d->%d, %.2f%%) %dms\n",
		rgn, ibsz, sz, (100.0*sz)/ibsz, t2);
	return(sz);
}
