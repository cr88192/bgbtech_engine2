int BGBDT_WorldDecodeRegionData(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	byte *cs, *cse, *cs1, *cs2, *cs3;
	int hsz;
	int tsz;
	int i, j, k;

	if(!rgn || !rgn->rgnbuf)
		return(-1);
	
	cs=rgn->rgnbuf;
	if((cs[0]!=0xE4) || (cs[4]!='R') || (cs[5]!='G') ||
		(cs[6]!='N') || (cs[7]!='0'))
	{
		return(-1);
	}
	hsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
	cse=cs+hsz;
	cs+=8;
	
	while(cs<cse)
	{
		if(cs[0]==0xE3)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			cs1=cs+tsz;
			//...
			cs=cs1;
			continue;
		}
		if(cs[0]==0xE4)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			cs1=cs+tsz;
			cs2=cs+8;

			if((cs[4]=='O') && (cs[5]=='F') &&
				(cs[6]=='S') && (cs[7]=='4'))
			{
				for(i=0; i<BGBDT_XYZ_TSZ_RGN_XYZ; i++)
				{
					j=(cs2[i*4+0]<<24)|(cs2[i*4+1]<<16)|
						(cs2[i*4+2]<<8)|(cs2[i*4+3]);
					rgn->chkofs[i]=j;
				}
				cs=cs1;
				continue;
			}

			if((cs[4]=='O') && (cs[5]=='F') &&
				(cs[6]=='S') && (cs[7]=='3'))
			{
				for(i=0; i<BGBDT_XYZ_TSZ_RGN_XYZ; i++)
				{
					j=(cs2[i*3+0]<<16)|
						(cs2[i*3+1]<<8)|(cs2[i*3+2]);
					rgn->chkofs[i]=j;
				}
				cs=cs1;
				continue;
			}

			if((cs[4]=='C') && (cs[5]=='E') &&
				(cs[6]=='B') && (cs[7]=='M'))
			{
				memcpy(rgn->rgnmap, cs2, tsz-8);
				cs=cs1;
				continue;
			}

			if((cs[4]=='C') && (cs[5]=='O') &&
				(cs[6]=='B') && (cs[7]=='M'))
			{
				j=(cs2[0]<<24)|(cs2[1]<<16)|
					(cs2[2]<<8)|(cs2[3]);
				k=(cs2[4]<<24)|(cs2[5]<<16)|
					(cs2[6]<<8)|(cs2[7]);
				memcpy(rgn->rgnmap, rgn->rgnbuf+j, k);
				cs=cs1;
				continue;
			}

			if((cs[4]=='S') && (cs[5]=='T') &&
				(cs[6]=='R') && (cs[7]=='S'))
			{
				k=tsz;
				if(k<1024)
					k=1024;
			
				rgn->strtab_buf=frgl_malloc(k);
				memcpy(rgn->strtab_buf, cs2, tsz-8);
				cs3=rgn->strtab_buf+(tsz-8);
				while(((cs3+2)>rgn->strtab_buf) && !cs3[0] && !cs3[-1])
					cs3--;
				cs3++;
				
				rgn->strtab_buf[0]=0;
				rgn->strtab_buf[1]=0;
				rgn->strtab_msz=k;
				rgn->strtab_nsz=cs3-rgn->strtab_buf;
				if(rgn->strtab_nsz<2)
					rgn->strtab_nsz=2;
				
				cs=cs1;
				continue;
			}

			if((cs[4]=='S') && (cs[5]=='T') &&
				(cs[6]=='I') && (cs[7]=='3'))
			{
				k=(tsz-8)/3;
				j=k;
				if(j<256)j=256;

				rgn->strix_ofs=frgl_malloc(j*sizeof(int));
				rgn->strix_num=k;
				rgn->strix_max=j;

				for(i=0; i<k; i++)
				{
					j=(cs2[i*3+0]<<16)|
						(cs2[i*3+1]<<8)|(cs2[i*3+2]);
					rgn->strix_ofs[i]=j;
				}
				cs=cs1;
				continue;
			}

			if((cs[4]=='S') && (cs[5]=='T') &&
				(cs[6]=='I') && (cs[7]=='2'))
			{
				k=(tsz-8)/2;
				j=k;
				if(j<256)j=256;

				rgn->strix_ofs=frgl_malloc(j*sizeof(int));
				rgn->strix_num=k;
				rgn->strix_max=j;

				for(i=0; i<k; i++)
				{
					j=(cs2[i*2+0]<<8)|(cs2[i*2+1]);
					rgn->strix_ofs[i]=j;
				}
				cs=cs1;
				continue;
			}

			cs=cs1;
			continue;
		}
	}
	return(0);
}

#if 0
int BGBDT_DecodeChunkLZ(BGBDT_RiceContext *ctx,
	byte *obuf, int obsz)
{
	byte *ct, *cte, *cs, *cse;
	int ks, kl, kd;
	int l, d, ll, ld;
	int i0, i1, i2, i3;
	int i, j, k;
	
	for(i=0; i<256; i++)
		{ ctx->lzwin[i]=i; ctx->lzidx[i]=i; }
	ctx->lzwpos=0;
	
	ks=4; kl=4; kd=4; ll=1; ld=1;
	ct=obuf; cte=obuf+obsz;
	while(ct<cte)
	{
		i=BGBDT_Rice_ReadAdRiceDc(ctx, &ks);
		if(i>0)
		{
			i--;
			
			if(!i)
			{	*ct++=ctx->lzwin[ctx->lzwpos];
				continue;	}
			if(i<16)
			{
				i0=(ctx->lzwpos+i-1)&255;
				i1=(ctx->lzwpos+i)&255;
				i2=ctx->lzwin[i0]; i3=ctx->lzwin[i1];
				ctx->lzwin[i0]=i3; ctx->lzwin[i1]=i2;
				*ct++=i3;
				continue;
			}
			
			i0=(ctx->lzwpos+i)&255;
			i1=(ctx->lzwpos-1)&255;
			ctx->lzwpos=ctx->lzwpos-1;
			i2=ctx->lzwin[i0]; i3=ctx->lzwin[i1];
			ctx->lzwin[i0]=i3; ctx->lzwin[i1]=i2;
			*ct++=i2;
			continue;
		}

		i=BGBDT_Rice_ReadAdSRiceDc(ctx, &kl);
		if(i<=0)
		{
			l=-i;
			d=BGBDT_Rice_ReadAdRiceDc(ctx, &kd);
			if(!l)l=ll;
			if(!d)d=ld;
			
			if((ct+l)>cte)
				break;
			cs=ct-d; cse=cs+l;
			if(cs<obuf)
				break;
			while(cs<cse)
				*ct++=*cs++;
			continue;
		}
		if(i==1)
			break;
	}
	return(ct-obuf);
}
#endif

#if 1
int BGBDT_ReadLzSTF(BGBDT_RiceContext *ctx, int *rk)
{
	int ix0, ix1;
	int sy0, sy1;
	
//	ix0=BGBDT_Rice_ReadAdRiceDc(ctx, rk);
	ix0=ctx->ReadAdRiceLL(ctx, rk);
	ix1=(ix0*7)/8;
	sy0=ctx->lzwin[ix0];	sy1=ctx->lzwin[ix1];
	ctx->lzwin[ix0]=sy1;	ctx->lzwin[ix1]=sy0;
	ctx->lzidx[sy0]=ix1;	ctx->lzidx[sy1]=ix0;
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
	if((ctx->lzctrl&7)!=0)
		ctx->ReadAdRiceLL=BGBDT_Rice_ReadAdRiceLL;
	
	kr=4; ks=4; kl=4; kd=4; ll=1; ld=1;
	ct=obuf; cte=obuf+obsz;
//	while(ct<cte)
	while(1)
	{
//		r=BGBDT_Rice_ReadAdRiceDc(ctx, &kr);
		r=ctx->ReadAdRiceLL(ctx, &kr);
		if(r>0)
		{
			ct1=ct+r;
			if(ct1>cte)
			{
				__debugbreak();
				break;
			}
			while(ct<ct1)
				*ct++=BGBDT_ReadLzSTF(ctx, &ks);
		}

//		l=BGBDT_Rice_ReadAdRiceDc(ctx, &kl);
		l=ctx->ReadAdRiceLL(ctx, &kl);
		
		if(l<=0)
		{
			if(r<=0)
				break;
			continue;
		}
		
//		d=BGBDT_Rice_ReadAdRiceDc(ctx, &kd);
		d=ctx->ReadAdRiceLL(ctx, &kd);
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

int BGBDT_WorldDecodeChunkBits(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk, byte *ibuf, int ibsz, int accfl)
{
	byte tb0[1024];
	BGBDT_VoxData td;
	BGBDT_VoxCoord xyz;
	BGBDT_RiceContext *ctx;
	byte *tbuf, *cs, *ct;
	int cmd, ts, mts, tp, tpsz, tpn, vstr;
	int vfl;
	int i, j, k;
	
	ctx=BGBDT_Rice_AllocContext();
	
	BGBDT_Rice_SetupRead(ctx, ibuf, ibsz);
		
	ts=1; tp=15;
	while(1)
	{
		cmd=BGBDT_Rice_ReadRice(ctx, 3);
		if(!cmd)
			break;

		if(cmd==1)
		{
			ts=BGBDT_Rice_ReadRice(ctx, 7);

			if(accfl&BGBDT_ACCFL_ENNEWCHK)
			{
				mts=8;
				while(mts<=ts)
					{ mts=mts+(mts>>1); }
			}else
			{
				mts=ts;
			}
			continue;
		}

		if(cmd==2)
		{	tp=BGBDT_Rice_ReadRice(ctx, 7);
			continue;	}

		if(cmd==3)
		{
			i=BGBDT_Rice_ReadRice(ctx, 3);
			j=BGBDT_Rice_ReadQExpBase(ctx, 5);
			
			switch(i)
			{
			case 1: chk->flags=j; break;
			case 2: ctx->lzctrl=j; break;
			default: break;
			}
			continue;
		}

		if(cmd==4)
		{
			if(!chk->voxidx)
				chk->voxidx=BGBDT_WorldAllocVoxelIndexL(world);
			BGBDT_DecodeChunkLZ(ctx, chk->voxidx, 4096);
			continue;
		}
		if(cmd==5)
		{
			if(!chk->voxidxh)
				chk->voxidxh=BGBDT_WorldAllocVoxelIndexH(world);
			BGBDT_DecodeChunkLZ(ctx, chk->voxidxh, 2048);
			continue;
		}
		
		if(cmd==6)
		{
			tpn=0;
			for(i=0; i<16; i++)
				if(tp&(1<<i))tpn++;
			tpsz=tpn*ts;
			
			tbuf=tb0;
			if(tpsz>1024)
				tbuf=BGBDT_WorldAllocVoxelTemp(world, tpsz);
			BGBDT_DecodeChunkLZ(ctx, tbuf, tpsz);
			
			if(chk->voxinfo && (chk->nvoxinfo!=ts))
			{
				BGBDT_WorldFreeVoxelData(world,
					chk->voxinfo, chk->nvoxinfo);
				chk->voxinfo=NULL;
			}
			
			if(!chk->voxinfo)
				chk->voxinfo=BGBDT_WorldAllocVoxelData(world, mts);
			chk->nvoxinfo=ts;
			chk->mvoxinfo=mts;

			vstr=sizeof(BGBDT_VoxData);
			
			cs=tbuf; ct=(byte *)chk->voxinfo;
			for(i=0; i<vstr; i++)
			{
				if(!(tp&(1<<i)))
					continue;
				for(j=0; j<ts; j++)
					{ ct[j*vstr+i]=cs[j]; }
				cs+=ts;
			}

			if(tbuf!=tb0)
				BGBDT_WorldFreeVoxelTemp(world, tbuf, tpsz);
			continue;
		}
		
		__debugbreak();
		break;
	}

	chk->flags|=BGBDT_CHKFL_MESHDIRTY;
	xyz.x=chk->bx<<BGBDT_XYZ_SHR_CHUNK;
	xyz.y=chk->by<<BGBDT_XYZ_SHR_CHUNK;
	xyz.z=chk->bz<<BGBDT_XYZ_SHR_CHUNK;

	if(!(chk->flags&(BGBDT_CHKFL_ONLYSOLID|BGBDT_CHKFL_ONLYAIR)))
	{
		k=0;
		for(i=0; i<chk->nvoxinfo; i++)
		{
			td=chk->voxinfo[i];
			vfl=BGBDT_WorldVoxel_GetFlags(world, xyz, td);
			if(vfl&(BGBDT_VOXFL_TRANSPARENT|
				BGBDT_VOXFL_NOFACES|
				BGBDT_VOXFL_NONSOLID|
				BGBDT_VOXFL_FLUID))
					k|=1;
			if(!(vfl&BGBDT_VOXFL_NOFACES))
				k|=2;
		}
		if(!(k&1))
			chk->flags|=BGBDT_CHKFL_ONLYSOLID;
		if(!(k&2))
			chk->flags|=BGBDT_CHKFL_ONLYAIR;
	}
		
	
	BGBDT_Rice_FreeContext(ctx);
	return(0);
}

int BGBDT_WorldDecodeChunk(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk, byte *ibuf, int accfl)
{
	byte *cs, *cse;
	int i, j;

	if(ibuf[0]==0xE4)
	{
		i=(ibuf[1]<<16)|(ibuf[2]<<8)|ibuf[3];
		
		cs=ibuf+8;
		cse=ibuf+i;
		
		if((ibuf[4]=='C') || (ibuf[5]=='H') ||
			(ibuf[6]=='K') || (ibuf[7]=='0'))
		{
			while(cs<cse)
			{
				if((*cs==0xE1) || (*cs==0xE3) || (*cs==0xE4))
				{
					j=BGBDT_WorldDecodeChunk(world, chk, cs, accfl);
					if(j>0)return(j);
					i=(cs[1]<<16)|(cs[2]<<8)|cs[3];
					cs+=i;
					continue;
				}

				if((*cs==0xE2) || (*cs==0xE5))
				{
					j=BGBDT_WorldDecodeChunk(world, chk, cs, accfl);
					if(j>0)return(j);
					i=cs[1];
					cs+=i;
					continue;
				}
			}
			return(0);
		}

		printf("BGBDT_WorldDecodeChunk: Unexpected Marker Tag A\n");
		return(0);
	}

	
	if(ibuf[0]==0xE1)
	{
		i=(ibuf[1]<<16)|(ibuf[2]<<8)|ibuf[3];
		BGBDT_WorldDecodeChunkBits(world, chk, ibuf+4, i-4, accfl);
		return(1);
	}
	
	if(ibuf[0]==0xE2)
	{
		i=ibuf[1];
		BGBDT_WorldDecodeChunkBits(world, chk, ibuf+2, i-2, accfl);
		return(1);
	}
	
	printf("BGBDT_WorldDecodeChunk: Unexpected Marker Tag B\n");
	return(0);
}
