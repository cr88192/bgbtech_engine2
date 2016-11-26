///////////////////////////////////////////////////////////////////////////////
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

void BGBDT_FreeRegion(BGBDT_VoxWorld *world, BGBDT_VoxRegion *rgn)
{
	rgn->next=world->freergn;
	world->freergn=rgn;
}

void BGBDT_FreeChunk(BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	chk->next=world->freechk;
	world->freechk=chk;
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

	rcur=world->lastrgn;
	if(rcur && (rcur->bx==bx) && (rcur->by==by) && (rcur->bz==bz))
		return(rcur);

	rcur=world->region;
	while(rcur)
	{
		if((rcur->bx==bx) && (rcur->by==by) && (rcur->bz==bz))
		{
			world->lastrgn=rcur;
			return(rcur);
		}
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
		
		rcur->nrgncell=szbuf>>BGBDT_RGN_CELLSHR;
		rcur->rgnmap=frgl_malloc((rcur->nrgncell+3)/4);
	
//		sprintf(tbuf, "region/%s%s.rgn", world->worldname,
//			bgbdt_xyz2nstr(bx, by, bz));
//		rcur->rgnbuf=vf_loadfile(tbuf, &(rcur->szrgnbuf));
		BGBDT_WorldDecodeRegionData(world, rcur);

		world->lastrgn=rcur;
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

		world->lastrgn=rcur;
		return(rcur);
	}

	return(NULL);
}

BTEIFGL_API BGBDT_VoxChunk *BGBDT_WorldGetChunk(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	BGBDT_VoxRegion *rgn;
	BGBDT_VoxChunk *chk;
	int bx, by, bz, bi;

#if 0
	chk=world->lastchk;
	if(chk)
	{
		rgn=chk->rgn;
		bx=xyz.x>>BGBDT_XYZ_SHR_REGION_XY;
		by=xyz.y>>BGBDT_XYZ_SHR_REGION_XY;
		bz=xyz.z>>BGBDT_XYZ_SHR_REGION_Z;
		if((rgn->bx==bx) && (rgn->by==by) && (rgn->bz==bz))
		{
			bx=(xyz.x>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
			by=(xyz.y>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
			bz=(xyz.z>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_Z;

			if((chk->bx==bx) && (chk->by==by) && (chk->bz==bz))
			{
				return(chk);
			}

			bi=((bz*BGBDT_XYZ_SZ_REGION_XY)+by)*BGBDT_XYZ_SZ_REGION_XY+bx;
			chk=rgn->chkptr[bi];
			if(chk)
			{
				world->lastchk=chk;
				return(chk);
			}

			chk=BGBDT_GetRegionChunk(world, rgn, bx, by, bz, accfl);
			world->lastchk=chk;
			return(chk);
		}
	}
#endif

#if 1
	rgn=world->lastrgn;
	bx=xyz.x>>BGBDT_XYZ_SHR_REGION_XY;
	by=xyz.y>>BGBDT_XYZ_SHR_REGION_XY;
	bz=xyz.z>>BGBDT_XYZ_SHR_REGION_Z;
	if(rgn && (rgn->bx==bx) && (rgn->by==by) && (rgn->bz==bz))
	{
		bx=(xyz.x>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
		by=(xyz.y>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
		bz=(xyz.z>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_Z;

		bi=((bz*BGBDT_XYZ_SZ_REGION_XY)+by)*BGBDT_XYZ_SZ_REGION_XY+bx;
		chk=rgn->chkptr[bi];
		if(chk)
		{
			world->lastchk=chk;
			return(chk);
		}

		chk=BGBDT_GetRegionChunk(world, rgn, bx, by, bz, accfl);
		world->lastchk=chk;
		return(chk);
	}
#endif
	
	rgn=BGBDT_WorldGetRegion(world, xyz, accfl);
	if(!rgn)return(NULL);
	
	bx=(xyz.x>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
	by=(xyz.y>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
	bz=(xyz.z>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_Z;
	
	chk=BGBDT_GetRegionChunk(world, rgn, bx, by, bz, accfl);
	world->lastrgn=rgn;
	world->lastchk=chk;
	return(chk);
}

BTEIFGL_API BGBDT_VoxChunk *BGBDT_GetRegionChunk(
	BGBDT_VoxWorld *world, BGBDT_VoxRegion *rgn,
	int bx, int by, int bz, int accfl)
{
	static int rcnt=0;
	BGBDT_VoxChunk *chk;
	int bi, bid;
	
//	if((bx<0) || (by<0) || (bz<0))
//		return(NULL);
//	if((bx>15) || (by>15) || (bz>15))
//		return(NULL);
	if((bx|by|bz)>>4)
		return(NULL);
	
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
		if(world->CheckRgnGenChunkP(world, rgn, bx, by, bz)<=0)
			return(NULL);

		if(rcnt>=16)
			return(NULL);
		rcnt++;
		
		chk=BGBDT_AllocChunk(world);
		rgn->chkptr[bi]=chk;
		chk->rgn=rgn;	chk->bx=bx;
		chk->by=by;		chk->bz=bz;

		if(world->GenerateChunk)
			world->GenerateChunk(world, chk);

		rgn->flags|=BGBDT_RGNFL_RGNDIRTY;

		rcnt--;
		return(chk);
	}
	
//	if(bz<8)
	if(1)
	{
		if(world->CheckRgnGenChunkP(world, rgn, bx, by, bz)<=0)
			return(NULL);

		bid=bgbdt_xyz2rgnid(rgn->bx, rgn->by, rgn->bz);
		printf("BGBDT_GetRegionChunk: Null Chunk %d,%d,%d RGN=%08X\n",
			bx, by, bz, bid);
		
		chk=BGBDT_GetRegionChunk(world, rgn, bx, by, bz,
			accfl|BGBDT_ACCFL_ENNEWCHK);
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

void BGBDT_WorldVoxel_GetTypeIdFlags(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td, int *rtix, int *rfl)
{
	BGBDT_VoxTypeInfo *tyi;
	int tix, vty;

#if defined(X86) || defined(X86_64)
	tix=(*(u16 *)(&td.vtypel))&4095;
#else
	vty=td.vtypel|(td.vtypeh<<8);
	tix=vty&4095;
#endif
//	tix=td.vtype&4095;
	tyi=world->voxtypes[tix];
	if(!tyi)
	{
		*rtix=tix;
		*rfl=0;
		return;
	}
	*rtix=tix;
	*rfl=tyi->flags;
}

void BGBDT_WorldGet_UpdateAdjFl(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxDataStatus *rts,
	int accfl)
{
	BGBDT_VoxData td, td1;
	BGBDT_VoxCoord xyz2;
	int vfl, vty, vfl1, vty1;
	
	if(!rts)
		return;

	BGBDT_WorldGetVoxelData(world, xyz, &td, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz, td, &vty, &vfl);
	
	if(!(vfl&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_S; }
	if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_S; }


	xyz2.x=xyz.x-BGBDT_XYZ_OFS_VOXEL; xyz2.y=xyz.y; xyz2.z=xyz.z;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz2, td1, &vty1, &vfl1);
	if(!(vfl1&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_NX; }
	if(!(vfl1&BGBDT_VOXFL_TRANSPARENT) || (vty1==vty))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NX; }
	rts->adjlit[0]=(td1.alight<<8)|td1.vlight;
	if(vfl1&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[0]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x+BGBDT_XYZ_OFS_VOXEL; xyz2.y=xyz.y; xyz2.z=xyz.z;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz2, td1, &vty1, &vfl1);
	if(!(vfl1&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_PX; }
	if(!(vfl1&BGBDT_VOXFL_TRANSPARENT) || (vty1==vty))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PX; }
	rts->adjlit[1]=(td1.alight<<8)|td1.vlight;
	if(vfl1&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[1]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x; xyz2.y=xyz.y-BGBDT_XYZ_OFS_VOXEL; xyz2.z=xyz.z;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz2, td1, &vty1, &vfl1);
	if(!(vfl1&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_NY; }
	if(!(vfl1&BGBDT_VOXFL_TRANSPARENT) || (vty1==vty))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NY; }
	rts->adjlit[2]=(td1.alight<<8)|td1.vlight;
	if(vfl1&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[2]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x; xyz2.y=xyz.y+BGBDT_XYZ_OFS_VOXEL; xyz2.z=xyz.z;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz2, td1, &vty1, &vfl1);
	if(!(vfl1&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_PY; }
	if(!(vfl1&BGBDT_VOXFL_TRANSPARENT) || (vty1==vty))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PY; }
	rts->adjlit[3]=(td1.alight<<8)|td1.vlight;
	if(vfl1&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[3]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x; xyz2.y=xyz.y; xyz2.z=xyz.z-BGBDT_XYZ_OFS_VOXEL;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz2, td1, &vty1, &vfl1);
	if(!(vfl1&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_NZ; }
	if(!(vfl1&BGBDT_VOXFL_TRANSPARENT) || (vty1==vty))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NZ; }
	rts->adjlit[4]=(td1.alight<<8)|td1.vlight;
	if(vfl1&BGBDT_VOXFL_GLOWLIGHT)
		{ rts->adjlit[4]=BGBDT_WorldVoxel_GetGlow(world, xyz2, td1); }

	xyz2.x=xyz.x; xyz2.y=xyz.y; xyz2.z=xyz.z+BGBDT_XYZ_OFS_VOXEL;
	BGBDT_WorldGetVoxelData(world, xyz2, &td1, NULL,
		accfl&(~BGBDT_ACCFL_CHKADJ));
	BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz2, td1, &vty1, &vfl1);
	if(!(vfl1&BGBDT_VOXFL_NONSOLID))
		{ rts->adjfl|=BGBDT_ADJFL_SOLID_PZ; }
	if(!(vfl1&BGBDT_VOXFL_TRANSPARENT) || (vty1==vty))
		{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PZ; }
	rts->adjlit[5]=(td1.alight<<8)|td1.vlight;
	if(vfl1&BGBDT_VOXFL_GLOWLIGHT)
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
//			ix=(bn&1)?(j&15):((j>>4)&15);
			ix=(j>>((1-(bn&1))*4))&15;
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

BGBDT_VoxCoord BGBDT_WorldGetRegionVoxelCoord(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, int bx, int by, int bz)
{
	BGBDT_VoxCoord xyz;
	xyz.x=(rgn->bx<<BGBDT_XYZ_SHR_REGION_XY)+(bx<<BGBDT_XYZ_SHR_VOXEL);
	xyz.y=(rgn->by<<BGBDT_XYZ_SHR_REGION_XY)+(by<<BGBDT_XYZ_SHR_VOXEL);
	xyz.z=(rgn->bz<<BGBDT_XYZ_SHR_REGION_Z )+(bz<<BGBDT_XYZ_SHR_VOXEL);
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
	int vt1, vt2, vt3, vt4, vt5, vt6;
	int ix, vfl, vty, ufl, ifl, bn;
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

	if(!chk->voxinfo)
	{
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
//			ix=(bn&1)?(j&15):((j>>4)&15);
			ix=(j>>((1-(bn&1))*4))&15;
		}
	}else if(chk->voxidx)
	{
		ix=chk->voxidx[bn];
	}else
	{
		if(chk->nvoxinfo==1)
		{
			if(accfl&BGBDT_ACCFL_CHKADJ)
			{
				if(((bx+1)|(bx-1)|(by+1)|(by-1)|(bz+1)|(bz-1))>>4)
				{
					xyz=BGBDT_WorldGetChunkVoxCoord(
						world, chk, bx, by, bz);

//					vfl=BGBDT_WorldVoxel_GetFlags(world,
//						xyz, *chk->voxinfo);
//					if(!(vfl&BGBDT_VOXFL_NONSOLID))
//						{ rts->adjfl|=BGBDT_ADJFL_SOLID_S; }
//					if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
//						{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_S; }

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

	if(accfl&BGBDT_ACCFL_CHKADJ)
	{
		if(((bx+1)|(bx-1)|(by+1)|(by-1)|(bz+1)|(bz-1))>>4)
		{
			xyz=BGBDT_WorldGetChunkVoxCoord(
				world, chk, bx, by, bz);

//			vfl=BGBDT_WorldVoxel_GetFlags(world,
//				xyz, chk->voxinfo[ix]);
//			if(!(vfl&BGBDT_VOXFL_NONSOLID))
//				{ rts->adjfl|=BGBDT_ADJFL_SOLID_S; }
//			if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
//				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_S; }

			BGBDT_WorldGet_UpdateAdjFl(world, xyz, rts, accfl);
		}else
		{
			xyz=BGBDT_WorldGetChunkVoxCoord(
				world, chk, bx, by, bz);

			td=chk->voxinfo[ix];
			BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz, td, &vty, &vfl);

//			vfl=BGBDT_WorldVoxel_GetFlags(world,
//				xyz, chk->voxinfo[ix]);
			if(!(vfl&BGBDT_VOXFL_NONSOLID))
				{ rts->adjfl|=BGBDT_ADJFL_SOLID_S; }
			if(!(vfl&BGBDT_VOXFL_TRANSPARENT))
				{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_S; }

//			vty=BGBDT_WorldVoxel_GetTypeID(world, xyz, chk->voxinfo[ix]);

			ix1=BGBDT_WorldGetChunkVoxIdx(world, chk, bx-1, by  , bz  );
			ix2=BGBDT_WorldGetChunkVoxIdx(world, chk, bx+1, by  , bz  );
			ix3=BGBDT_WorldGetChunkVoxIdx(world, chk, bx  , by-1, bz  );
			ix4=BGBDT_WorldGetChunkVoxIdx(world, chk, bx  , by+1, bz  );
			ix5=BGBDT_WorldGetChunkVoxIdx(world, chk, bx  , by  , bz-1);
			ix6=BGBDT_WorldGetChunkVoxIdx(world, chk, bx  , by  , bz+1);

			td1=chk->voxinfo[ix1];	td2=chk->voxinfo[ix2];
			td3=chk->voxinfo[ix3];	td4=chk->voxinfo[ix4];
			td5=chk->voxinfo[ix5];	td6=chk->voxinfo[ix6];

			BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz, td1, &vt1, &vfl1);
			BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz, td2, &vt2, &vfl2);
			BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz, td3, &vt3, &vfl3);
			BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz, td4, &vt4, &vfl4);
			BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz, td5, &vt5, &vfl5);
			BGBDT_WorldVoxel_GetTypeIdFlags(world, xyz, td6, &vt6, &vfl6);

			rts->adjlit[0]=(td1.alight<<8)|td1.vlight;
			rts->adjlit[1]=(td2.alight<<8)|td2.vlight;
			rts->adjlit[2]=(td3.alight<<8)|td3.vlight;
			rts->adjlit[3]=(td4.alight<<8)|td4.vlight;
			rts->adjlit[4]=(td5.alight<<8)|td5.vlight;
			rts->adjlit[5]=(td6.alight<<8)|td6.vlight;

			ufl=vfl1|vfl2|vfl3|vfl4|vfl5|vfl6;
			ifl=vfl1&vfl2&vfl3&vfl4&vfl5&vfl6;

			if(ufl&BGBDT_VOXFL_GLOWLIGHT)
			{
				if(vfl1&BGBDT_VOXFL_GLOWLIGHT)
					{ rts->adjlit[0]=BGBDT_WorldVoxel_GetGlow(
						world, xyz, td1); }
				if(vfl2&BGBDT_VOXFL_GLOWLIGHT)
					{ rts->adjlit[1]=BGBDT_WorldVoxel_GetGlow(
						world, xyz, td2); }
				if(vfl3&BGBDT_VOXFL_GLOWLIGHT)
					{ rts->adjlit[2]=BGBDT_WorldVoxel_GetGlow(
						world, xyz, td3); }
				if(vfl4&BGBDT_VOXFL_GLOWLIGHT)
					{ rts->adjlit[3]=BGBDT_WorldVoxel_GetGlow(
						world, xyz, td4); }
				if(vfl5&BGBDT_VOXFL_GLOWLIGHT)
					{ rts->adjlit[4]=BGBDT_WorldVoxel_GetGlow(
						world, xyz, td5); }
				if(vfl6&BGBDT_VOXFL_GLOWLIGHT)
					{ rts->adjlit[5]=BGBDT_WorldVoxel_GetGlow(
						world, xyz, td6); }
			}

			if(!(ifl&BGBDT_VOXFL_NONSOLID))
			{
				if(!(ufl&BGBDT_VOXFL_NONSOLID))
				{
					rts->adjfl|=
						BGBDT_ADJFL_SOLID_NX|BGBDT_ADJFL_SOLID_PX|
						BGBDT_ADJFL_SOLID_NY|BGBDT_ADJFL_SOLID_PY|
						BGBDT_ADJFL_SOLID_NZ|BGBDT_ADJFL_SOLID_PZ;
				}else
				{
					if(!(vfl1&BGBDT_VOXFL_NONSOLID))
						{ rts->adjfl|=BGBDT_ADJFL_SOLID_NX; }
					if(!(vfl2&BGBDT_VOXFL_NONSOLID))
						{ rts->adjfl|=BGBDT_ADJFL_SOLID_PX; }
					if(!(vfl3&BGBDT_VOXFL_NONSOLID))
						{ rts->adjfl|=BGBDT_ADJFL_SOLID_NY; }
					if(!(vfl4&BGBDT_VOXFL_NONSOLID))
						{ rts->adjfl|=BGBDT_ADJFL_SOLID_PY; }
					if(!(vfl5&BGBDT_VOXFL_NONSOLID))
						{ rts->adjfl|=BGBDT_ADJFL_SOLID_NZ; }
					if(!(vfl6&BGBDT_VOXFL_NONSOLID))
						{ rts->adjfl|=BGBDT_ADJFL_SOLID_PZ; }
				}
			}

//			if(!(ifl&BGBDT_VOXFL_TRANSPARENT))

			if(!(ufl&BGBDT_VOXFL_TRANSPARENT))
			{
				rts->adjfl|=
					BGBDT_ADJFL_OPAQUE_NX|BGBDT_ADJFL_OPAQUE_PX|
					BGBDT_ADJFL_OPAQUE_NY|BGBDT_ADJFL_OPAQUE_PY|
					BGBDT_ADJFL_OPAQUE_NZ|BGBDT_ADJFL_OPAQUE_PZ;
			}else
			{
				if(!(vfl1&BGBDT_VOXFL_TRANSPARENT) || (vt1==vty))
					{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NX; }
				if(!(vfl2&BGBDT_VOXFL_TRANSPARENT) || (vt2==vty))
					{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PX; }
				if(!(vfl3&BGBDT_VOXFL_TRANSPARENT) || (vt3==vty))
					{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NY; }
				if(!(vfl4&BGBDT_VOXFL_TRANSPARENT) || (vt4==vty))
					{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PY; }
				if(!(vfl5&BGBDT_VOXFL_TRANSPARENT) || (vt5==vty))
					{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_NZ; }
				if(!(vfl6&BGBDT_VOXFL_TRANSPARENT) || (vt6==vty))
					{ rts->adjfl|=BGBDT_ADJFL_OPAQUE_PZ; }
			}
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
	int bx, by, bz, bn, fl, flcl, acc2;

	bx=(xyz.x>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	by=(xyz.y>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;
	bz=(xyz.z>>BGBDT_XYZ_SHR_VOXEL)&BGBDT_XYZ_MASK_CHKVOX;

	if(!(((bx+1)|(bx-1)|(by+1)|(by-1)|(bz+1)|(bz-1))>>4))
		return(0);

	acc2=accfl&(~BGBDT_ACCFL_CHKADJ);

//	fl=BGBDT_CHKFL_DIRTY;
	if(accfl&BGBDT_ACCFL_LIGHTDIRTY)
	{
		fl=BGBDT_CHKFL_LIGHTDIRTY;
		flcl=-1;
	}else
	{
		fl=BGBDT_CHKFL_ALLDIRTY;
		flcl=~(BGBDT_CHKFL_ONLYSOLID|
			BGBDT_CHKFL_ONLYAIR|BGBDT_CHKFL_MIXEDSOLID|
			BGBDT_CHKFL_NOVISFACE);
	}

	xyz1.x=xyz.x-BGBDT_XYZ_OFS_VOXEL; xyz1.y=xyz.y; xyz1.z=xyz.z;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; chk->flags&=flcl; }

	xyz1.x=xyz.x+BGBDT_XYZ_OFS_VOXEL; xyz1.y=xyz.y; xyz1.z=xyz.z;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; chk->flags&=flcl; }

	xyz1.x=xyz.x; xyz1.y=xyz.y-BGBDT_XYZ_OFS_VOXEL; xyz1.z=xyz.z;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; chk->flags&=flcl; }

	xyz1.x=xyz.x; xyz1.y=xyz.y+BGBDT_XYZ_OFS_VOXEL; xyz1.z=xyz.z;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; chk->flags&=flcl; }

	xyz1.x=xyz.x; xyz1.y=xyz.y; xyz1.z=xyz.z-BGBDT_XYZ_OFS_VOXEL;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; chk->flags&=flcl; }

	xyz1.x=xyz.x; xyz1.y=xyz.y; xyz1.z=xyz.z+BGBDT_XYZ_OFS_VOXEL;
	chk=BGBDT_WorldGetChunk(world, xyz1, acc2);
	if(chk) { chk->flags|=fl; chk->flags&=flcl; }

	return(0);
}

BTEIFGL_API int BGBDT_WorldSetChunkVoxelData(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td, int accfl)
{
	BGBDT_VoxData td1;
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
	if((bx|by|bz)>>4)
	{
		__debugbreak();
	}
	
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

	td1=chk->voxinfo[ix];
	if((td1.vtypel==td.vtypel) && (td1.vtypeh==td.vtypeh) &&
		(td1.vattr==td.vattr))
//	if(0)
	{
		chk->flags|=BGBDT_CHKFL_ALLDIRTY;
//		if((td1.vdatal!=td.vdatal) || (td1.vdatah!=td.vdatah))
		chk->rgn->flags|=BGBDT_RGNFL_RGNDIRTY;
	}else
	{
	//	chk->flags|=BGBDT_CHKFL_DIRTY|
	//		BGBDT_CHKFL_MESHDIRTY|
	//		BGBDT_CHKFL_LIGHTDIRTY|
	//		BGBDT_CHKFL_SAVEDIRTY;
		chk->flags|=BGBDT_CHKFL_ALLDIRTY;
		chk->rgn->flags|=BGBDT_RGNFL_RGNDIRTY;

		chk->flags&=~(BGBDT_CHKFL_ONLYSOLID|
			BGBDT_CHKFL_ONLYAIR|BGBDT_CHKFL_MIXEDSOLID|
			BGBDT_CHKFL_NOVISFACE);
	}

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
			isz=chk->mvoxinfo;
			isz=isz+(isz>>1);
			if(isz<16)isz=16;
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
//	if(!(chk->flags&BGBDT_CHKFL_MESHDIRTY))
//	{
//		printf("BGBDT_WorldSetVoxelData: No Mesh Dirty\n");
//	}
	return(i);
}

BTEIFGL_API int BGBDT_WorldUpdateChunkSolid(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk)
{
	BGBDT_VoxData td;
	BGBDT_VoxCoord xyz;
	int vfl, vty;
	int i, j, k;

	if(chk->flags&(BGBDT_CHKFL_ONLYSOLID|
			BGBDT_CHKFL_ONLYAIR|BGBDT_CHKFL_MIXEDSOLID))
		return(0);

	xyz.x=chk->bx<<BGBDT_XYZ_SHR_CHUNK;
	xyz.y=chk->by<<BGBDT_XYZ_SHR_CHUNK;
	xyz.z=chk->bz<<BGBDT_XYZ_SHR_CHUNK;

	k=0;
	for(i=0; i<chk->nvoxinfo; i++)
	{
		td=chk->voxinfo[i];
		vfl=BGBDT_WorldVoxel_GetFlags(world, xyz, td);
		vty=BGBDT_WorldVoxel_GetTypeID(world, xyz, td);
		if(vty==0)
			continue;
		if(vfl&(BGBDT_VOXFL_TRANSPARENT|
//			BGBDT_VOXFL_NOFACES|
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

	if((k&3)==3)
		chk->flags|=BGBDT_CHKFL_MIXEDSOLID;

	return(1);
}


BTEIFGL_API int BGBDT_WorldGetString(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl, char *str)
{
	BGBDT_VoxRegion *rgn;
	int i, j, k;

	rgn=BGBDT_WorldGetRegion(world, xyz, accfl);
	if(!rgn)return(-1);
	i=BGBDT_WorldRegionGetString(world, rgn, str);
	return(i);
}

BTEIFGL_API char *BGBDT_WorldGetStringForIndex(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl, int idx)
{
	BGBDT_VoxRegion *rgn;
	char *s;
	int i, j, k;

	rgn=BGBDT_WorldGetRegion(world, xyz, accfl);
	if(!rgn)return(NULL);
	s=BGBDT_WorldRegionStringForIndex(world, rgn, idx);
	return(s);
}


void BGBDT_UnloadVoxRegionChunk(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, BGBDT_VoxChunk *chk)
{
	if(chk->voxidxh)
	{
		BGBDT_WorldFreeVoxelIndexH(world, chk->voxidxh);
		chk->voxidxh=NULL;
	}

	if(chk->voxidx)
	{
		BGBDT_WorldFreeVoxelIndexL(world, chk->voxidx);
		chk->voxidx=NULL;
	}

	if(chk->voxinfo)
	{
		BGBDT_WorldFreeVoxelData(world,
			chk->voxinfo, chk->mvoxinfo);
		chk->voxinfo=NULL;
	}

	BGBDT_FreeChunk(world, chk);
}

void BGBDT_UnloadVoxRegion(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	BGBDT_VoxRegion *rcur, *rprv;
	BGBDT_VoxChunkMesh *mesh;
	BGBDT_VoxChunk *chk;
	int bx, by, bz, bi;

	world->lastchk=NULL;
	world->lastrgn=NULL;

	if(rgn->flags&BGBDT_RGNFL_RGNDIRTY)
	{
		BGBDT_WorldSaveRegionData(world, rgn);
		rgn->flags&=~BGBDT_RGNFL_RGNDIRTY;
	}
	
	for(bz=0; bz<16; bz++)
		for(by=0; by<16; by++)
			for(bx=0; bx<16; bx++)
	{
		bi=((bz*BGBDT_XYZ_SZ_REGION_XY)+by)*BGBDT_XYZ_SZ_REGION_XY+bx;

		mesh=rgn->chkmesh[bi];
		if(mesh)
		{
			BGBDT_FreeChunkMesh(world, mesh);
			rgn->chkmesh[bi]=NULL;
		}

		chk=rgn->chkptr[bi];
		if(chk)
		{
			BGBDT_UnloadVoxRegionChunk(world, rgn, chk);
			rgn->chkptr[bi]=NULL;
		}
	}
	
	if(rgn->rgnmap)
	{
		frgl_free(rgn->rgnmap);
		rgn->rgnmap=NULL;
	}

	if(rgn->rgnbuf)
	{
		frgl_free(rgn->rgnbuf);
		rgn->rgnbuf=NULL;
	}
	
	rcur=world->region; rprv=NULL;
	while(rcur && (rcur!=rgn))
	{
		rprv=rcur;
		rcur=rcur->next;
	}
	
	if(rcur)
	{
		if(rprv)
		{
			rprv->next=rcur->next;
		}else
		{
			world->region=rcur->next;
		}
	}
	
	BGBDT_FreeRegion(world, rgn);
}


void BGBDT_UnloadRegionMeshes(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	BGBDT_VoxChunkMesh *mesh;
	int bx, by, bz, bi;

	for(bz=0; bz<16; bz++)
		for(by=0; by<16; by++)
			for(bx=0; bx<16; bx++)
	{
		bi=((bz*BGBDT_XYZ_SZ_REGION_XY)+by)*BGBDT_XYZ_SZ_REGION_XY+bx;

		mesh=rgn->chkmesh[bi];
		if(mesh)
		{
			BGBDT_FreeChunkMesh(world, mesh);
			rgn->chkmesh[bi]=NULL;
		}
	}
}
