#include <bteifgl.h>

BTEIFGL_API int BGBDT_WorldVoxel_GetTypeID(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	int tix, vty;
	vty=td.vtypel|(td.vtypeh<<8);
	tix=vty&4095;
	return(tix);
}

BTEIFGL_API int BGBDT_WorldVoxel_GetFlags(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	BGBDT_VoxTypeInfo *tyi;
	int tix, vty;
	
	vty=td.vtypel|(td.vtypeh<<8);
	tix=vty&4095;
//	tix=td.vtype&4095;
	tyi=world->voxtypes[tix];
	if(!tyi)return(0);
	return(tyi->flags);
}

BTEIFGL_API char *BGBDT_WorldVoxel_GetTypeName(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	BGBDT_VoxTypeInfo *tyi;
	int tix, vty;
	
	vty=td.vtypel|(td.vtypeh<<8);
	tix=vty&4095;
//	tix=td.vtype&4095;
	tyi=world->voxtypes[tix];
	if(!tyi)return(NULL);
	return(tyi->name);
}

BTEIFGL_API int BGBDT_WorldVoxel_CheckSolidP(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	int tfl;
	
	tfl=BGBDT_WorldVoxel_GetFlags(world, xyz, td);
	if(tfl&BGBDT_VOXFL_NONSOLID)
		return(0);
	return(1);
}

BTEIFGL_API int BGBDT_WorldVoxel_CheckTransparentP(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	int tfl;
	
	tfl=BGBDT_WorldVoxel_GetFlags(world, xyz, td);
	if(tfl&BGBDT_VOXFL_TRANSPARENT)
		return(1);
	return(0);
}

BTEIFGL_API int BGBDT_WorldVoxel_CheckAirP(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	char *tn;
	int tfl;
	
	tn=BGBDT_WorldVoxel_GetTypeName(world, xyz, td);
	if(!tn)
		return(0);
	if(!strcmp(tn, "air"))
		return(1);
	return(0);
}

BTEIFGL_API int BGBDT_WorldVoxel_CheckMatchP(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td, int tracefl)
{
	char *tn;
	int tfl;
	
	if(tracefl&BGBDT_TRFL_SOLID)
	{
		if(BGBDT_WorldVoxel_CheckSolidP(world, xyz, td))
			return(1);
	}

	if(tracefl&BGBDT_TRFL_OPAQUE)
	{
		if(!BGBDT_WorldVoxel_CheckTransparentP(world, xyz, td))
			return(1);
	}

	if(tracefl&BGBDT_TRFL_NONEMPTY)
	{
		if(!BGBDT_WorldVoxel_CheckAirP(world, xyz, td))
			return(1);
	}

	return(0);
}

BTEIFGL_API int BGBDT_WorldVoxel_GetGlow(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	BGBDT_VoxTypeInfo *tyi;
	int tix, vty;
	
	vty=td.vtypel|(td.vtypeh<<8);
	tix=vty&4095;
//	tix=td.vtype&4095;
	tyi=world->voxtypes[tix];
	if(!tyi)return(0);
	return(tyi->glow);
}

BTEIFGL_API BGBDT_VoxTypeInfo *BGBDT_VoxelWorld_LookupTypeInfoName(
	BGBDT_VoxWorld *world, char *name)
{
	BGBDT_VoxTypeInfo *tyi;
//	int tix;
	int i;
	
	for(i=0; i<4096; i++)
	{
		tyi=world->voxtypes[i];
		if(!tyi)
			break;
		if(!strcmp(tyi->name, name))
			return(tyi);
	}
	return(NULL);
	
//	tix=td.vtype&4095;
//	tyi=world->voxtypes[tix];
//	if(!tyi)return(0);
//	return(tyi->flags);
}

BTEIFGL_API BGBDT_VoxTypeInfo *BGBDT_VoxelWorld_GetTypeInfoName(
	BGBDT_VoxWorld *world, char *name)
{
	BGBDT_VoxTypeInfo *tyi;
//	int tix;
	int i;
	
	for(i=0; i<4096; i++)
	{
		tyi=world->voxtypes[i];
		if(!tyi)
			break;
		if(!strcmp(tyi->name, name))
			return(tyi);
	}
	
	if(i<4096)
	{
		tyi=frgl_malloc(sizeof(BGBDT_VoxTypeInfo));
		tyi->name=frgl_strdup(name);
		tyi->tidx=i;
		world->voxtypes[i]=tyi;
		return(tyi);
	}
	
	return(NULL);
}

BTEIFGL_API BGBDT_VoxTypeInfo *BGBDT_VoxelWorld_LookupTypeInfoIndex(
	BGBDT_VoxWorld *world, int idx)
{
	BGBDT_VoxTypeInfo *tyi;
	
	if((idx<0) || (idx>=4096))
		return(NULL);
	tyi=world->voxtypes[idx];
	return(tyi);
}

BTEIFGL_API int BGBDT_VoxelWorld_LookupTypeIndexName(
	BGBDT_VoxWorld *world, char *name)
{
	BGBDT_VoxTypeInfo *tyi;
	
	tyi=BGBDT_VoxelWorld_LookupTypeInfoName(world, name);
	if(!tyi)
		return(0);
	return(tyi->tidx);
}

int BGBDT_WorldRegionLookupString(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, char *str)
{
	char *s0;
	int i, j, k;

	if(!str)
		return(0);
	if(!*str)
		return(1);
		
	if(!rgn->strix_ofs)
		return(-1);

	for(i=0; i<rgn->strix_num; i++)
	{
		j=rgn->strix_ofs[i];
		if((j<0)||(j>=rgn->strtab_nsz))
		{
			rgn->strix_ofs[i]=0;
			continue;
		}
		s0=rgn->strtab_buf+j;
		if(!strcmp(s0, str))
			return(i);
	}
	return(-1);
}

int BGBDT_WorldRegionGetString(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, char *str)
{
	char *s0, *se;
	int i, j, k, l;

	if(!str)
		return(0);
	if(!*str)
		return(1);

	i=BGBDT_WorldRegionLookupString(world, rgn, str);
	if(i>=0)
		return(i);

	if(!rgn->strix_ofs)
	{
		k=256;
		rgn->strix_ofs=frgl_malloc(k*sizeof(int));
		rgn->strix_num=2;
		rgn->strix_max=k;
		
		rgn->strix_ofs[0]=0;
		rgn->strix_ofs[1]=1;
	}
	
	if((rgn->strix_num+1)>=rgn->strix_max)
	{
		k=rgn->strix_max;
		k=k+(k>>1);
		if(k<256)k=256;
		rgn->strix_ofs=frgl_realloc(rgn->strix_ofs, k*sizeof(int));
		rgn->strix_max=k;
	}
	
	if(!rgn->strtab_buf)
	{
		k=4096;
		rgn->strtab_buf=frgl_malloc(k);
		rgn->strtab_nsz=2;
		rgn->strtab_msz=k;
	}

	s0=rgn->strtab_buf; se=s0+rgn->strtab_nsz;
	while(s0<se)
	{
		if(!strcmp(s0, str))
			break;
		s0++;
	}
	
	if(s0<se)
	{
		for(i=2; i<rgn->strix_num; i++)
		{
			j=rgn->strix_ofs[i];
			if(!j)break;
		}
		if(i>=rgn->strix_num)
			i=rgn->strix_num++;
		rgn->strix_ofs[i]=s0-rgn->strtab_buf;
		return(i);
	}

	l=strlen(str)+1;
	if((rgn->strtab_nsz+l)>=rgn->strtab_msz)
	{
		j=rgn->strtab_nsz+l;
		k=rgn->strtab_msz;
		while(j>=k)
			k=k+(k>>1);
		rgn->strtab_buf=frgl_realloc(rgn->strtab_buf, k);
		rgn->strtab_msz=k;
	}
	
	s0=rgn->strtab_buf+rgn->strtab_nsz;
	rgn->strtab_nsz+=l;
	strcpy(s0, str);

	for(i=2; i<rgn->strix_num; i++)
	{
		j=rgn->strix_ofs[i];
		if(!j)break;
	}
	if(i>=rgn->strix_num)
		i=rgn->strix_num++;

	rgn->strix_ofs[i]=s0-rgn->strtab_buf;
	return(i);
}

char *BGBDT_WorldRegionStringForIndex(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, int idx)
{
	int i, j, k;

	if(idx<=0)
		return(NULL);
	if(idx==1)
		return("");
	if(idx>=rgn->strix_num)
		return(NULL);
	
	j=rgn->strix_ofs[idx];
	if((j<0) || (j>=rgn->strtab_nsz))
	{
		rgn->strix_ofs[idx]=0;
		return(NULL);
	}
	return(rgn->strtab_buf+j);
}
