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
//	for(i=0; i<4; i++)
//	for(i=0; i<8; i++)
	for(i=0; i<16; i++)
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
//	for(i=0; i<2048; i++)
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
	i&=~(BGBDT_CHKFL_DIRTY|BGBDT_CHKFL_MESHDIRTY|BGBDT_CHKFL_SAVEDIRTY);
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
		__debugbreak();
	
	i=(rgn->rgnmap[idx>>2]>>((idx&3)<<1))&3;
	return(i);
}

int BGBDT_RegionCellSetTag(BGBDT_VoxRegion *rgn, int idx, int val)
{
	int i, j, k;

	if((idx<0) || (idx>=rgn->nrgncell))
		__debugbreak();

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
		for(j=1; j<n; j++)
			BGBDT_RegionCellSetTag(rgn, i+j, 2);
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
	for(i=ix+1; i<rgn->nrgncell; i++)
	{
		j=BGBDT_RegionCellGetTag(rgn, i);
		if(j!=2)break;
		BGBDT_RegionCellSetTag(rgn, i, 0);
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
	byte *ct;
	int bid;
	int bx, by, bz, bn;
	int n, sz0, sz1;
	int i, j, k;

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

	vf_storefile(tbuf, rgn->rgnbuf, rgn->szrgnbuf);
	return(0);
}

