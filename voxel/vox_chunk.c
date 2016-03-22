#include <bteifgl.h>

BGBDT_VoxRegion *BGBDT_AllocRegion(BGBDT_VoxWorld *world)
{
	BGBDT_VoxRegion *tmp;
	
	if(world->freergn)
	{
		tmp=world->freergn;
		world->freergn=tmp->next;
		memset(tmp, 0, sizeof(BGBDT_VoxRegion));
		return(tmp);
	}
	
	tmp=frgl_talloc("bgbdt_voxregion_t", sizeof(BGBDT_VoxRegion));
	return(tmp);
}

BGBDT_VoxChunk *BGBDT_AllocChunk(BGBDT_VoxWorld *world)
{
	BGBDT_VoxChunk *tmp;

	if(world->freechk)
	{
		tmp=world->freechk;
		world->freechk=tmp->next;
		memset(tmp, 0, sizeof(BGBDT_VoxChunk));
		return(tmp);
	}
	
	tmp=frgl_talloc("bgbdt_voxchunk_t", sizeof(BGBDT_VoxChunk));
	return(tmp);
}

byte *BGBDT_WorldAllocVoxelIndexL(BGBDT_VoxWorld *world)
{
	byte *buf;
	
	if(world->freeidxl)
	{
		buf=world->freeidxl;
		world->freeidxl=*(void **)buf;
		memset(buf, 0, 4096);
		return(buf);
	}
	
	buf=frgl_malloc(4096);
	return(buf);
}

byte *BGBDT_WorldAllocVoxelIndexH(BGBDT_VoxWorld *world)
{
	byte *buf;
	
	if(world->freeidxh)
	{
		buf=world->freeidxh;
		world->freeidxh=*(void **)buf;
		memset(buf, 0, 2048);
		return(buf);
	}
	
	buf=frgl_malloc(2048);
	return(buf);
}

void BGBDT_WorldFreeVoxelIndexL(BGBDT_VoxWorld *world, byte *buf)
{
	*(void **)buf=world->freeidxl;
	world->freeidxl=buf;
}

void BGBDT_WorldFreeVoxelIndexH(BGBDT_VoxWorld *world, byte *buf)
{
	*(void **)buf=world->freeidxh;
	world->freeidxh=buf;
}

void *BGBDT_WorldAllocVoxelData(BGBDT_VoxWorld *world, int num)
{
	byte *buf;
	
	buf=BGBDT_WorldAllocVoxelTemp(world, num*sizeof(BGBDT_VoxData));
	return(buf);
	
//	buf=frgl_malloc(num*sizeof(BGBDT_VoxData));
//	return(buf);
}

void BGBDT_WorldFreeVoxelData(BGBDT_VoxWorld *world, void *buf, int num)
{
	BGBDT_WorldFreeVoxelTemp(world, buf, num*sizeof(BGBDT_VoxData));
//	frgl_free(buf);
}

void *BGBDT_WorldReallocVoxelData(BGBDT_VoxWorld *world,
	void *oldbuf, int oldnum, int newnum)
{
	byte *buf;
	
	buf=BGBDT_WorldAllocVoxelData(world, newnum);
	memcpy(buf, oldbuf, oldnum*sizeof(BGBDT_VoxData));
	BGBDT_WorldFreeVoxelData(world, oldbuf, oldnum);
	return(buf);
}

u32 bgbdt_xyz2rgnid(int bx, int by, int bz)
{
	u32 v;
	
	v=((bz&0xFF)<<24)|((by&0xFFF)<<12)|(bx&0xFFF);
	return(v);
}

BTEIFGL_API BGBDT_VoxRegion *BGBDT_WorldGetRegion(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	char tbuf[256];
	BGBDT_VoxRegion *rcur;
	byte *buf;
	int szbuf;
	int bx, by, bz, bid;
	
	bx=xyz.x>>BGBDT_XYZ_SHR_REGION_XY;
	by=xyz.y>>BGBDT_XYZ_SHR_REGION_XY;
	bz=xyz.z>>BGBDT_XYZ_SHR_REGION_Z;

	rcur=world->region;
	while(rcur)
	{
		if((rcur->bx==bx) && (rcur->by==by) && (rcur->bz==bz))
			return(rcur);
		rcur=rcur->next;
	}

	if(accfl&BGBDT_ACCFL_NOLOAD)
		return(NULL);
	
	//load region.

	bid=bgbdt_xyz2rgnid(bx, by, bz);
	sprintf(tbuf, "region/%s/%08X.rgn", world->worldname, bid);

	buf=vf_loadfile(tbuf, &szbuf);
	
	if(buf)
	{
		rcur=BGBDT_AllocRegion(world);
		rcur->next=world->region;
		world->region=rcur;
		rcur->world=world;
		rcur->bx=bx;
		rcur->by=by;
		rcur->bz=bz;

		rcur->rgnbuf=buf;
		rcur->szrgnbuf=szbuf;
	
//		sprintf(tbuf, "region/%s%s.rgn", world->worldname,
//			bgbdt_xyz2nstr(bx, by, bz));
//		rcur->rgnbuf=vf_loadfile(tbuf, &(rcur->szrgnbuf));
		BGBDT_WorldDecodeRegionData(world, rcur);

		return(rcur);
	}

	if(accfl&BGBDT_ACCFL_ENNEWRGN)
	{
		rcur=BGBDT_AllocRegion(world);
		rcur->next=world->region;
		world->region=rcur;
		rcur->world=world;
		rcur->bx=bx;
		rcur->by=by;
		rcur->bz=bz;
		rcur->flags=BGBDT_RGNFL_NEWRGN;

		return(rcur);
	}

	return(NULL);
}

int BGBDT_WorldDecodeRegionData(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	byte *cs, *cse, *cs1, *cs2;
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

			cs=cs1;
			continue;
		}
	}
	return(0);
}

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

int BGBDT_WorldDecodeChunkBits(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk, byte *ibuf, int ibsz, int accfl)
{
	BGBDT_RiceContext *ctx;
	byte *tbuf, *cs, *ct;
	int cmd, ts, mts, tp, tpsz, tpn, vstr;
	int i, j, k;
	
	ctx=BGBDT_Rice_AllocContext();
	
	BGBDT_Rice_SetupRead(ctx, ibuf, ibsz);
	
	ts=1; tp=15;
	while(1)
	{
		cmd=BGBDT_Rice_ReadRiceDc(ctx, 4);
		if(!cmd)
			break;

		if(cmd==1)
		{
			ts=BGBDT_Rice_ReadRiceDc(ctx, 7);

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
		{	tp=BGBDT_Rice_ReadRiceDc(ctx, 7);
			continue;	}

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
					{ ct[j*vstr+i]=tbuf[j]; }
				cs+=ts;
			}

			BGBDT_WorldFreeVoxelTemp(world, tbuf, tpsz);
			continue;
		}
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
	
	return(0);
}

BTEIFGL_API BGBDT_VoxChunk *BGBDT_WorldGetChunk(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	BGBDT_VoxRegion *rgn;
	BGBDT_VoxChunk *chk;
	int bx, by, bz, bi;

	rgn=BGBDT_WorldGetRegion(world, xyz, accfl);
	if(!rgn)return(NULL);
	
	bx=(xyz.x>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
	by=(xyz.y>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
	bz=(xyz.z>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_Z;
	
	chk=BGBDT_GetRegionChunk(world, rgn, bx, by, bz, accfl);
	return(chk);
}

BTEIFGL_API BGBDT_VoxChunk *BGBDT_GetRegionChunk(
	BGBDT_VoxWorld *world, BGBDT_VoxRegion *rgn,
	int bx, int by, int bz, int accfl)
{
	BGBDT_VoxChunk *chk;
	int bi;
	
	bi=((bz*BGBDT_XYZ_SZ_REGION_XY)+by)*BGBDT_XYZ_SZ_REGION_XY+bx;
	chk=rgn->chkptr[bi];
	if(chk)
		return(chk);
	
	if(accfl&BGBDT_ACCFL_NOLOAD)
		return(NULL);
	
	if(rgn->chkofs[bi])
	{
		chk=BGBDT_AllocChunk(world);
		rgn->chkptr[bi]=chk;
		chk->rgn=rgn;	chk->bx=bx;
		chk->by=by;		chk->bz=bz;
	
		BGBDT_WorldDecodeChunk(world, chk,
			rgn->rgnbuf+rgn->chkofs[bi], accfl);
		return(chk);
	}
	
	if(accfl&BGBDT_ACCFL_ENNEWCHK)
	{
		chk=BGBDT_AllocChunk(world);
		rgn->chkptr[bi]=chk;
		chk->rgn=rgn;	chk->bx=bx;
		chk->by=by;		chk->bz=bz;
	
		if(world->GenerateChunk)
			world->GenerateChunk(world, chk);
		return(chk);
	}
	
	return(NULL);
}

void bgbdt_clearvoxdatastatus(BGBDT_VoxDataStatus *rts)
{
	if(rts)
	{
		rts->adjfl=0;
	}
}

void BGBDT_WorldGet_UpdateAdjFl(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxDataStatus *rts,
	int accfl)
{
	BGBDT_VoxData td1;
	BGBDT_VoxCoord xyz2;
	int vfl;
	
	if(!rts)
		return;
	
	xyz2.x=xyz.x-BGBDT_XYZ_OFS_VOXEL;
	xyz2.y=xyz.y;
	xyz2.z=xyz.z;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	vfl=BGBDT_WorldVoxel_GetFlags(world, xyz2, td1);
	if(!(vfl&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_NX; }
	if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NX; }
	rts->adjlit[0]=(td1.alight<<8)|td1.vlight;
	if(vfl&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[0]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x+BGBDT_XYZ_OFS_VOXEL;
	xyz2.y=xyz.y;
	xyz2.z=xyz.z;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	vfl=BGBDT_WorldVoxel_GetFlags(world, xyz2, td1);
	if(!(vfl&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_PX; }
	if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PX; }
	rts->adjlit[1]=(td1.alight<<8)|td1.vlight;
	if(vfl&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[1]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x;
	xyz2.y=xyz.y-BGBDT_XYZ_OFS_VOXEL;
	xyz2.z=xyz.z;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	vfl=BGBDT_WorldVoxel_GetFlags(world, xyz2, td1);
	if(!(vfl&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_NY; }
	if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NY; }
	rts->adjlit[2]=(td1.alight<<8)|td1.vlight;
	if(vfl&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[2]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x;
	xyz2.y=xyz.y+BGBDT_XYZ_OFS_VOXEL;
	xyz2.z=xyz.z;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	vfl=BGBDT_WorldVoxel_GetFlags(world, xyz2, td1);
	if(!(vfl&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_PY; }
	if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PY; }
	rts->adjlit[3]=(td1.alight<<8)|td1.vlight;
	if(vfl&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[3]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x;
	xyz2.y=xyz.y;
	xyz2.z=xyz.z-BGBDT_XYZ_OFS_VOXEL;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	vfl=BGBDT_WorldVoxel_GetFlags(world, xyz2, td1);
	if(!(vfl&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_NZ; }
	if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NZ; }
	rts->adjlit[4]=(td1.alight<<8)|td1.vlight;
	if(vfl&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[4]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x;
	xyz2.y=xyz.y;
	xyz2.z=xyz.z+BGBDT_XYZ_OFS_VOXEL;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	vfl=BGBDT_WorldVoxel_GetFlags(world, xyz2, td1);
	if(!(vfl&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_PZ; }
	if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PZ; }
	rts->adjlit[5]=(td1.alight<<8)|td1.vlight;
	if(vfl&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[5]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }
}

int BGBDT_WorldGetChunkVoxIdx(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk, int bx, int by, int bz)
{
	int bn, ix;
	int i, j, k;

	bn=(bz*BGBDT_XYZ_TSZ_CHK_XY)+(by*BGBDT_XYZ_SZ_CHUNK_XYZ)+bx;

	if(chk->voxidxh)
	{
		if(chk->voxidx)
		{
			j=chk->voxidxh[bn>>1];
			j=(bn&1)?(j&15):((j>>4)&15);
			ix=chk->voxidx[bn]|(j<<8);
		}else
		{
			j=chk->voxidxh[bn>>1];
			ix=(bn&1)?(j&15):((j>>4)&15);
		}
	}else
	{
		ix=chk->voxidx[bn];
	}
	return(ix);
}

BGBDT_VoxCoord BGBDT_WorldGetChunkVoxCoord(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk, int bx, int by, int bz)
{
	BGBDT_VoxCoord xyz;

	xyz.x=	(chk->rgn->bx<<BGBDT_XYZ_SHR_REGION_XY)+
			(chk->bx<<BGBDT_XYZ_SHR_CHUNK)+
			(bx<<BGBDT_XYZ_SHR_VOXEL);
	xyz.y=	(chk->rgn->by<<BGBDT_XYZ_SHR_REGION_XY)+
			(chk->by<<BGBDT_XYZ_SHR_CHUNK)+
			(by<<BGBDT_XYZ_SHR_VOXEL);
	xyz.z=	(chk->rgn->bz<<BGBDT_XYZ_SHR_REGION_Z)+
			(chk->bz<<BGBDT_XYZ_SHR_CHUNK)+
			(bz<<BGBDT_XYZ_SHR_VOXEL);
	return(xyz);
}

BGBDT_VoxCoord BGBDT_WorldGetRegionChunkCoord(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, int bx, int by, int bz)
{
	BGBDT_VoxCoord xyz;
	xyz.x=(rgn->bx<<BGBDT_XYZ_SHR_REGION_XY)+(bx<<BGBDT_XYZ_SHR_CHUNK);
	xyz.y=(rgn->by<<BGBDT_XYZ_SHR_REGION_XY)+(by<<BGBDT_XYZ_SHR_CHUNK);
	xyz.z=(rgn->bz<<BGBDT_XYZ_SHR_REGION_Z )+(bz<<BGBDT_XYZ_SHR_CHUNK);
	return(xyz);
}

BTEIFGL_API int BGBDT_WorldGetVoxelData(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData *rtd,
	BGBDT_VoxDataStatus *rts, int accfl)
{
	BGBDT_VoxChunk *chk;
	int bx, by, bz, bn;

	chk=BGBDT_WorldGetChunk(world, xyz, accfl);
	if(!chk)
	{
		memset(rtd, 0, sizeof(BGBDT_VoxData));
		return(-1);
	}

	bx=(xyz.x>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	by=(xyz.y>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	bz=(xyz.z>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	
	return(BGBDT_WorldGetChunkVoxelData(world,
		bx, by, bz, chk, rtd, rts, accfl));
}

BTEIFGL_API int BGBDT_WorldGetChunkVoxelData(BGBDT_VoxWorld *world,
	int bx, int by, int bz,
	BGBDT_VoxChunk *chk,
	BGBDT_VoxData *rtd,
	BGBDT_VoxDataStatus *rts, int accfl)
{
	BGBDT_VoxData td, td1, td2, td3, td4, td5, td6;
	BGBDT_VoxCoord xyz;
	int ix1, ix2, ix3, ix4, ix5, ix6;
	int vfl1, vfl2, vfl3, vfl4, vfl5, vfl6;
	int ix, vfl, bn;
	int i, j, k;

	if((bx|by|bz)>>4)
	{
		xyz=BGBDT_WorldGetChunkVoxCoord(
			world, chk, bx, by, bz);
		return(BGBDT_WorldGetVoxelData(world,
			xyz, rtd, rts, accfl));
	}

	bn=(bz*BGBDT_XYZ_TSZ_CHK_XY)+(by*BGBDT_XYZ_SZ_CHUNK_XYZ)+bx;

	bgbdt_clearvoxdatastatus(rts);

	if((!chk->voxidx && !chk->voxidxh) || !chk->voxinfo)
	{
		if(chk->voxinfo && (chk->nvoxinfo==1))
		{
			if(accfl&BGBDT_ACCFL_CHKADJ)
			{
				if(((bx+1)|(bx-1)|(by+1)|(by-1)|(bz+1)|(bz-1))>>4)
				{
					xyz=BGBDT_WorldGetChunkVoxCoord(
						world, chk, bx, by, bz);

					vfl=BGBDT_WorldVoxel_GetFlags(world,
						xyz, *chk->voxinfo);
					if(!(vfl&BGBDT_VOXFL_NONSOLID))
						{ rts->adjfl|=BGBDT_ADJFL_SOLID_S; }
					if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
						{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_S; }

					BGBDT_WorldGet_UpdateAdjFl(world, xyz, rts, accfl);
				}else
				{
					xyz=BGBDT_WorldGetChunkVoxCoord(
						world, chk, bx, by, bz);
					vfl=BGBDT_WorldVoxel_GetFlags(world,
						xyz, *chk->voxinfo);
					if(!(vfl&BGBDT_VOXFL_NONSOLID))
					{
						rts->adjfl|=BGBDT_ADJFL_SOLID_MASK|
							BGBDT_ADJFL_SOLID_S;
					}
					if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
					{ 
						rts->adjfl|=BGBDT_ADJFL_OPAQUE_MASK|
							BGBDT_ADJFL_OPAQUE_S;
					}
				}
			}
		
			*rtd=*chk->voxinfo;
			return(0);
		}

		memset(rtd, 0, sizeof(BGBDT_VoxData));
		return(-1);
	}
		
	if(chk->voxidxh)
	{
		if(chk->voxidx)
		{
			j=chk->voxidxh[bn>>1];
			j=(bn&1)?(j&15):((j>>4)&15);
			ix=chk->voxidx[bn]|(j<<8);
		}else
		{
			j=chk->voxidxh[bn>>1];
			ix=(bn&1)?(j&15):((j>>4)&15);
		}
	}else
	{
		ix=chk->voxidx[bn];
	}

	if(accfl&BGBDT_ACCFL_CHKADJ)
	{
		if(((bx+1)|(bx-1)|(by+1)|(by-1)|(bz+1)|(bz-1))>>4)
		{
			xyz=BGBDT_WorldGetChunkVoxCoord(
				world, chk, bx, by, bz);

			vfl=BGBDT_WorldVoxel_GetFlags(world,
				xyz, chk->voxinfo[ix]);
			if(!(vfl&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_S; }
			if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_S; }

			BGBDT_WorldGet_UpdateAdjFl(world, xyz, rts, accfl);
		}else
		{
			xyz=BGBDT_WorldGetChunkVoxCoord(
				world, chk, bx, by, bz);

			vfl=BGBDT_WorldVoxel_GetFlags(world,
				xyz, chk->voxinfo[ix]);
			if(!(vfl&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_S; }
			if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_S; }

			ix1=BGBDT_WorldGetChunkVoxIdx(world, chk, bx-1, by  , bz  );
			ix2=BGBDT_WorldGetChunkVoxIdx(world, chk, bx+1, by  , bz  );
			ix3=BGBDT_WorldGetChunkVoxIdx(world, chk, bx  , by-1, bz  );
			ix4=BGBDT_WorldGetChunkVoxIdx(world, chk, bx  , by+1, bz  );
			ix5=BGBDT_WorldGetChunkVoxIdx(world, chk, bx  , by  , bz-1);
			ix6=BGBDT_WorldGetChunkVoxIdx(world, chk, bx  , by  , bz+1);

			td1=chk->voxinfo[ix1];	td2=chk->voxinfo[ix2];
			td3=chk->voxinfo[ix3];	td4=chk->voxinfo[ix4];
			td5=chk->voxinfo[ix5];	td6=chk->voxinfo[ix6];

			vfl1=BGBDT_WorldVoxel_GetFlags(world, xyz, td1);
			vfl2=BGBDT_WorldVoxel_GetFlags(world, xyz, td2);
			vfl3=BGBDT_WorldVoxel_GetFlags(world, xyz, td3);
			vfl4=BGBDT_WorldVoxel_GetFlags(world, xyz, td4);
			vfl5=BGBDT_WorldVoxel_GetFlags(world, xyz, td5);
			vfl6=BGBDT_WorldVoxel_GetFlags(world, xyz, td6);

			rts->adjlit[0]=(td1.alight<<8)|td1.vlight;
			rts->adjlit[1]=(td2.alight<<8)|td2.vlight;
			rts->adjlit[2]=(td3.alight<<8)|td3.vlight;
			rts->adjlit[3]=(td4.alight<<8)|td4.vlight;
			rts->adjlit[4]=(td5.alight<<8)|td5.vlight;
			rts->adjlit[5]=(td6.alight<<8)|td6.vlight;

			if(vfl1&BGBDT_VOXFL_GLOWLIGHT)
				{ rts->adjlit[0]=BGBDT_WorldVoxel_GetGlow(world, xyz, td1); }
			if(vfl2&BGBDT_VOXFL_GLOWLIGHT)
				{ rts->adjlit[1]=BGBDT_WorldVoxel_GetGlow(world, xyz, td2); }
			if(vfl3&BGBDT_VOXFL_GLOWLIGHT)
				{ rts->adjlit[2]=BGBDT_WorldVoxel_GetGlow(world, xyz, td3); }
			if(vfl4&BGBDT_VOXFL_GLOWLIGHT)
				{ rts->adjlit[3]=BGBDT_WorldVoxel_GetGlow(world, xyz, td4); }
			if(vfl5&BGBDT_VOXFL_GLOWLIGHT)
				{ rts->adjlit[4]=BGBDT_WorldVoxel_GetGlow(world, xyz, td5); }
			if(vfl6&BGBDT_VOXFL_GLOWLIGHT)
				{ rts->adjlit[5]=BGBDT_WorldVoxel_GetGlow(world, xyz, td6); }

			if(!(vfl1&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_NX; }
			if(!(vfl1&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NX; }
			if(!(vfl2&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_PX; }
			if(!(vfl2&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PX; }
			if(!(vfl3&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_NY; }
			if(!(vfl3&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NY; }
			if(!(vfl4&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_PY; }
			if(!(vfl4&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PY; }
			if(!(vfl5&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_NZ; }
			if(!(vfl5&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NZ; }
			if(!(vfl6&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_PZ; }
			if(!(vfl6&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PZ; }
		}
	}

	*rtd=chk->voxinfo[ix];
	return(0);
}

int BGBDT_WorldTouchAdjacentChunks(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	BGBDT_VoxCoord xyz1;
	BGBDT_VoxChunk *chk;
	int bx, by, bz, bn, fl, acc2;

	bx=(xyz.x>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	by=(xyz.y>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	bz=(xyz.z>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;

	if(!(((bx+1)|(bx-1)|(by+1)|(by-1)|(bz+1)|(bz-1))>>4))
		return(0);

	acc2=accfl&(~BGBDT_ACCFL_CHKADJ);

	fl=BGBDT_CHKFL_DIRTY;
	if(accfl&BGBDT_ACCFL_LIGHTDIRTY)
		fl=BGBDT_CHKFL_LIGHTDIRTY;

	xyz1.x=xyz.x-BGBDT_XYZ_OFS_VOXEL; xyz1.y=xyz.y; xyz1.z=xyz.z;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; }

	xyz1.x=xyz.x+BGBDT_XYZ_OFS_VOXEL; xyz1.y=xyz.y; xyz1.z=xyz.z;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; }

	xyz1.x=xyz.x; xyz1.y=xyz.y-BGBDT_XYZ_OFS_VOXEL; xyz1.z=xyz.z;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; }

	xyz1.x=xyz.x; xyz1.y=xyz.y+BGBDT_XYZ_OFS_VOXEL; xyz1.z=xyz.z;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; }

	xyz1.x=xyz.x; xyz1.y=xyz.y; xyz1.z=xyz.z-BGBDT_XYZ_OFS_VOXEL;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; }

	xyz1.x=xyz.x; xyz1.y=xyz.y; xyz1.z=xyz.z+BGBDT_XYZ_OFS_VOXEL;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; }

	return(0);
}

BTEIFGL_API int BGBDT_WorldSetChunkVoxelData(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td, int accfl)
{
	int bx, by, bz, bn;
	int ix, isz;
	int i, j, k;

	if((!chk->voxidx && !chk->voxidxh) || !chk->voxinfo)
	{
		if(!chk->voxinfo)
		{
			isz=16;
			chk->voxinfo=BGBDT_WorldAllocVoxelData(world, isz);
			chk->nvoxinfo=1;
			chk->mvoxinfo=isz;
			memcpy(&(chk->voxinfo[0]), &td, sizeof(BGBDT_VoxData));
			return(0);
		}

//		if(chk->voxinfo && (chk->nvoxinfo==1))
		if(chk->nvoxinfo==1)
		{
			if(!memcmp(chk->voxinfo, &td, sizeof(BGBDT_VoxData)))
				return(0);
//			return(td);
		}

		if(!chk->voxidx && !chk->voxidxh && (chk->nvoxinfo<16))
			chk->voxidxh=BGBDT_WorldAllocVoxelIndexH(world);

//		if(!chk->voxidx)
		if(!chk->voxidx && !chk->voxidxh)
			chk->voxidx=BGBDT_WorldAllocVoxelIndexL(world);

		if(!chk->voxidxh && (chk->nvoxinfo>256))
		{
			if(!chk->voxidxh)
				chk->voxidxh=BGBDT_WorldAllocVoxelIndexH(world);
		}
	}
	
	bx=(xyz.x>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	by=(xyz.y>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	bz=(xyz.z>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	bn=(bz*BGBDT_XYZ_TSZ_CHK_XY)+(by*BGBDT_XYZ_SZ_CHUNK_XYZ)+bx;

	if(chk->voxidxh)
	{
		if(chk->voxidx)
		{
			j=chk->voxidxh[bn>>1];
			j=(bn&1)?(j&15):((j>>4)&15);
			ix=chk->voxidx[bn]|(j<<8);
		}else
		{
			j=chk->voxidxh[bn>>1];
			ix=(bn&1)?(j&15):((j>>4)&15);
		}
	}else
	{
		ix=chk->voxidx[bn];
	}

	if(!memcmp(&(chk->voxinfo[ix]), &td, sizeof(BGBDT_VoxData)))
		return(0);

	chk->flags|=BGBDT_CHKFL_DIRTY|
		BGBDT_CHKFL_MESHDIRTY|
		BGBDT_CHKFL_LIGHTDIRTY;

	if(accfl&BGBDT_ACCFL_CHKADJ)
		{ BGBDT_WorldTouchAdjacentChunks(world, xyz, accfl); }

	for(ix=0; ix<chk->nvoxinfo; ix++)
	{
		if(!memcmp(&(chk->voxinfo[ix]), &td, sizeof(BGBDT_VoxData)))
			break;
	}

	if(ix>=chk->nvoxinfo)
	{
		if(chk->nvoxinfo<chk->mvoxinfo)
		{
			ix=chk->nvoxinfo++;
			memcpy(&(chk->voxinfo[ix]), &td, sizeof(BGBDT_VoxData));
		}else
		{
			isz=chk->mvoxinfo; isz=isz+(isz>>1);
			chk->voxinfo=BGBDT_WorldReallocVoxelData(world,
				chk->voxinfo, chk->mvoxinfo, isz);
			chk->mvoxinfo=isz;

			ix=chk->nvoxinfo++;
			memcpy(&(chk->voxinfo[ix]), &td, sizeof(BGBDT_VoxData));
		}

		if(!chk->voxidx && (chk->nvoxinfo>16))
		{
			chk->voxidx=BGBDT_WorldAllocVoxelIndexL(world);

			if(chk->voxidxh)
			{
				for(i=0; i<4096; i++)
				{
					j=chk->voxidxh[i>>1];
					j=(i&1)?(j&15):((j>>4)&15);
					chk->voxidx[i]=j;
				}
				
				if(chk->nvoxinfo<256)
				{
					BGBDT_WorldFreeVoxelIndexH(world, chk->voxidxh);
					chk->voxidxh=NULL;
				}else
				{
					memset(chk->voxidxh, 0, 2048);
				}
			}
		}
		
		if(!chk->voxidxh && (chk->nvoxinfo>256))
		{
			if(!chk->voxidxh)
				chk->voxidxh=BGBDT_WorldAllocVoxelIndexH(world);
		}
	}
	
//	if(ix<chk->nvoxinfo)
	if(1)
	{
		if(chk->voxidxh)
		{
			if(chk->voxidx)
			{
				chk->voxidx[bn]=ix;

				if(bn&1)
				{
					j=chk->voxidxh[bn>>1];
					j=(j&0xF0)|((ix>>8)&0x0F);
					chk->voxidxh[bn>>1]=j;
				}else
				{
					j=chk->voxidxh[bn>>1];
					j=(j&0x0F)|((ix>>4)&0xF0);
					chk->voxidxh[bn>>1]=j;
				}
			}else
			{
				if(bn&1)
				{
					j=chk->voxidxh[bn>>1];
					j=(j&0xF0)|(ix&0x0F);
					chk->voxidxh[bn>>1]=j;
				}else
				{
					j=chk->voxidxh[bn>>1];
					j=(j&0x0F)|((ix<<4)&0xF0);
					chk->voxidxh[bn>>1]=j;
				}
			}
		}else
		{
			chk->voxidx[bn]=ix;
		}
		return(0);
	}

//	*rtd=chk->voxinfo[ix];
	return(0);
}

BTEIFGL_API int BGBDT_WorldSetVoxelData(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td, int accfl)
{
	BGBDT_VoxChunk *chk;
	int i;

	chk=BGBDT_WorldGetChunk(world, xyz, accfl);
	if(!chk)
	{
		return(-1);
	}
	
	i=BGBDT_WorldSetChunkVoxelData(world, chk, xyz, td, accfl);
	return(i);
}
