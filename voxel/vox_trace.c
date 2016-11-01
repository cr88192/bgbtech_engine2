#include <bteifgl.h>

int BGBDT_VoxTrace_CheckChunkEmpty(
	BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	int vty, vty_air;
	int i, j, k;
	
	if(!chk)
		return(-1);
	if(!chk->voxinfo)
		return(-1);

	vty_air=BGBDT_VoxelWorld_LookupTypeIndexName(world, "air");

	for(i=0; i<chk->nvoxinfo; i++)
	{
		vty=chk->voxinfo[i].vtypel|(chk->voxinfo[i].vtypeh<<8);
//		if((chk->voxinfo[i].vtype&4095)!=vty_air)
		if((vty&4095)!=vty_air)
			break;
	}
	return(i>=chk->nvoxinfo);
}

int BGBDT_VoxTrace_CheckChunkSolid(
	BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	BGBDT_VoxTypeInfo *tyi;
	int tix, vty;
	int i, j, k;
	
	if(!chk)
		return(-1);
	if(!chk->voxinfo)
		return(-1);

	for(i=0; i<chk->nvoxinfo; i++)
	{
		vty=chk->voxinfo[i].vtypel|(chk->voxinfo[i].vtypeh<<8);
		tix=vty&4095;
//		tix=chk->voxinfo[i].vtype&4095;
		tyi=world->voxtypes[tix];
		if(!tyi)break;
		if(tyi->flags&BGBDT_VOXFL_NONSOLID)
			break;
	}
	return(i>=chk->nvoxinfo);
}

int BGBDT_VoxTrace_CheckChunkOpaque(
	BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	BGBDT_VoxTypeInfo *tyi;
	int tix, vty;
	int i, j, k;
	
	if(!chk)
		return(-1);
	if(!chk->voxinfo)
		return(-1);

	for(i=0; i<chk->nvoxinfo; i++)
	{
		vty=chk->voxinfo[i].vtypel|(chk->voxinfo[i].vtypeh<<8);
		tix=vty&4095;
//		tix=chk->voxinfo[i].vtype&4095;
		tyi=world->voxtypes[tix];
		if(!tyi)break;
		if(tyi->flags&BGBDT_VOXFL_TRANSPARENT)
			break;
	}
	return(i>=chk->nvoxinfo);
}

BTEIFGL_API BGBDT_VoxChunk *BGBDT_RayCastChunk(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord spos, BGBDT_VoxCoord epos,
	BGBDT_VoxCoord *rpos, int tracefl)
{
	BGBDT_VoxCoord cpos, diff, step;
	BGBDT_VoxChunk *chk;
	int m, d, n, accfl;
	int i, j, k;
	
	diff.x=epos.x-spos.x;
	diff.y=epos.y-spos.y;
	diff.z=epos.z-spos.z;
	
	m=abs(diff.x);
	i=abs(diff.y); if(i>m)m=i;
	i=abs(diff.z); if(i>m)m=i;
//	d=m>>BGBDT_XYZ_SHR_CHUNK;
//	if(d>0)
	if(m>0)
	{
//		d=m>>BGBDT_XYZ_SHR_CHUNK;
		d=(1<<(12+BGBDT_XYZ_SHR_CHUNK))/m;
		step.x=(((s64)diff.x)*d+2048)>>12;
		step.y=(((s64)diff.y)*d+2048)>>12;
		step.z=(((s64)diff.z)*d+2048)>>12;

//		step.x=diff.x/d;
//		step.y=diff.y/d;
//		step.z=diff.z/d;
	}
	
	accfl=0;
	if(tracefl&BGBDT_TRFL_NOLOAD)
		accfl|=BGBDT_ACCFL_NOLOAD;
	
	n=1+(m>>BGBDT_XYZ_SHR_CHUNK);
	cpos=spos;
	for(i=0; i<n; i++)
	{
		chk=BGBDT_WorldGetChunk(world, cpos, accfl);
		if(!chk)
		{
			if(tracefl&BGBDT_TRFL_NONCHUNK)
				break;
			cpos.x=cpos.x+step.x;
			cpos.y=cpos.y+step.y;
			cpos.z=cpos.z+step.z;
			continue;
		}
	
		if(tracefl&BGBDT_TRFL_SOLID)
		{
			if(!BGBDT_VoxTrace_CheckChunkSolid(world, chk))
				break;
		}

		if(tracefl&BGBDT_TRFL_OPAQUE)
		{
			if(!BGBDT_VoxTrace_CheckChunkOpaque(world, chk))
				break;
		}
		
		if(tracefl&BGBDT_TRFL_NONEMPTY)
		{
			if(!BGBDT_VoxTrace_CheckChunkEmpty(world, chk))
				break;
		}
	
		cpos.x=cpos.x+step.x;
		cpos.y=cpos.y+step.y;
		cpos.z=cpos.z+step.z;
	}
	
	if(i<n)
	{
		if(rpos)
			{ *rpos=cpos; }
		return(chk);
	}
	
	if(rpos)
		{ *rpos=cpos; }
	return(NULL);
}



BTEIFGL_API int BGBDT_RayCastVoxel(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord spos, BGBDT_VoxCoord epos,
	BGBDT_VoxCoord *rpos, BGBDT_VoxCoord *rlpos,
	BGBDT_VoxData *rtd, BGBDT_VoxDataStatus *rts,
	int tracefl)
{
	BGBDT_VoxCoord cpos, lcpos, diff, step;
	BGBDT_VoxData td;
	BGBDT_VoxDataStatus tds;
	BGBDT_VoxChunk *chk;
	int m, d, n, accfl;
	int i, j, k;
	
	diff.x=epos.x-spos.x;
	diff.y=epos.y-spos.y;
	diff.z=epos.z-spos.z;
	
	m=abs(diff.x);
	i=abs(diff.y); if(i>m)m=i;
	i=abs(diff.z); if(i>m)m=i;
	if(m>0)
	{
		d=(1<<(16+BGBDT_XYZ_SHR_VOXEL))/m;
		step.x=(((s64)diff.x)*d+32768)>>16;
		step.y=(((s64)diff.y)*d+32768)>>16;
		step.z=(((s64)diff.z)*d+32768)>>16;
	}
	
	accfl=0;
	if(tracefl&BGBDT_TRFL_NOLOAD)
		accfl|=BGBDT_ACCFL_NOLOAD;
	
	n=1+(m>>BGBDT_XYZ_SHR_VOXEL);
	cpos=spos;
	for(i=0; i<n; i++)
	{
		BGBDT_WorldGetVoxelData(world, cpos, &td, &tds, accfl);

		if(tracefl&BGBDT_TRFL_SOLID)
		{
			if(BGBDT_WorldVoxel_CheckSolidP(world, cpos, td))
				break;
		}

		if(tracefl&BGBDT_TRFL_OPAQUE)
		{
			if(!BGBDT_WorldVoxel_CheckTransparentP(world, cpos, td))
				break;
		}

		if(tracefl&BGBDT_TRFL_NONEMPTY)
		{
			if(!BGBDT_WorldVoxel_CheckAirP(world, cpos, td))
				break;
		}

#if 0
//		chk=BGBDT_WorldGetChunk(world, cpos, accfl);
		if(!chk)
		{
			if(tracefl&BGBDT_TRFL_NONCHUNK)
				break;
			cpos.x=cpos.x+step.x;
			cpos.y=cpos.y+step.y;
			cpos.z=cpos.z+step.z;
			continue;
		}
	
		if(tracefl&BGBDT_TRFL_SOLID)
		{
			if(!BGBDT_VoxTrace_CheckChunkSolid(world, chk))
				break;
		}

		if(tracefl&BGBDT_TRFL_OPAQUE)
		{
			if(!BGBDT_VoxTrace_CheckChunkOpaque(world, chk))
				break;
		}
		
		if(tracefl&BGBDT_TRFL_NONEMPTY)
		{
			if(!BGBDT_VoxTrace_CheckChunkEmpty(world, chk))
				break;
		}
#endif
	
		lcpos=cpos;
		cpos.x=cpos.x+step.x;
		cpos.y=cpos.y+step.y;
		cpos.z=cpos.z+step.z;
	}

	lcpos.x=lcpos.x&(~BGBDT_XYZ_MASK_VOXEL);
	lcpos.y=lcpos.y&(~BGBDT_XYZ_MASK_VOXEL);
	lcpos.z=lcpos.z&(~BGBDT_XYZ_MASK_VOXEL);
	
	cpos.x=cpos.x&(~BGBDT_XYZ_MASK_VOXEL);
	cpos.y=cpos.y&(~BGBDT_XYZ_MASK_VOXEL);
	cpos.z=cpos.z&(~BGBDT_XYZ_MASK_VOXEL);
	
	if(i<n)
	{
		if(rtd)
			{ *rtd=td; }
		if(rts)
			{ *rts=tds; }
		if(rpos)
			{ *rpos=cpos; }
		if(rlpos)
			{ *rlpos=lcpos; }
		return(1);
	}
	
	if(rtd)
		{ *rtd=td; }
	if(rts)
		{ *rts=tds; }
	if(rpos)
		{ *rpos=cpos; }
	return(0);
}

BTEIFGL_API int BGBDT_BoxQueryVoxel(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord min, BGBDT_VoxCoord max,
	BGBDT_VoxCoord *rpos, BGBDT_VoxData *rtd, int *rnvox, 
	int tracefl)
{
	BGBDT_VoxCoord cpos;
	BGBDT_VoxData td;
	BGBDT_VoxDataStatus tds;
	int x, y, z;
	int bx, by, bz;
	int nx, ny, nz;
	int n, s, accfl;
	
	bx=min.x>>BGBDT_XYZ_SHR_VOXEL;
	by=min.y>>BGBDT_XYZ_SHR_VOXEL;
	bz=min.z>>BGBDT_XYZ_SHR_VOXEL;
	nx=(max.x+BGBDT_XYZ_MASK_VOXEL)>>BGBDT_XYZ_SHR_VOXEL;
	ny=(max.y+BGBDT_XYZ_MASK_VOXEL)>>BGBDT_XYZ_SHR_VOXEL;
	nz=(max.z+BGBDT_XYZ_MASK_VOXEL)>>BGBDT_XYZ_SHR_VOXEL;
	
	accfl=0;
	if(tracefl&BGBDT_TRFL_NOLOAD)
		accfl|=BGBDT_ACCFL_NOLOAD;
	
	n=0;
	for(z=bz; z<nz; z++)
		for(y=by; y<ny; y++)
			for(x=bx; x<nx; x++)
	{
		cpos.x=x<<BGBDT_XYZ_SHR_VOXEL;
		cpos.y=y<<BGBDT_XYZ_SHR_VOXEL;
		cpos.z=z<<BGBDT_XYZ_SHR_VOXEL;
	
		s=0;
		BGBDT_WorldGetVoxelData(world, cpos, &td, &tds, accfl);

		if(BGBDT_WorldVoxel_CheckMatchP(world, cpos, td, tracefl))
		{
			if(rpos)
				rpos[n]=cpos;
			if(rtd)
				rtd[n]=td;
			n++;
		}
	}
	if(rnvox)
		*rnvox=n;
	return(n>0);
//	return(0);
}
