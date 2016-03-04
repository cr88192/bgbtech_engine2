#include <bteifgl.h>

BTEIFGL_API int BGBDT_WorldVoxel_GetFlags(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	BGBDT_VoxTypeInfo *tyi;
	int tix;
	
	tix=td.vtype&4095;
	tyi=world->voxtypes[tix];
	if(!tyi)return(0);
	return(tyi->flags);
}

BTEIFGL_API char *BGBDT_WorldVoxel_GetTypeName(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	BGBDT_VoxTypeInfo *tyi;
	int tix;
	
	tix=td.vtype&4095;
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
	int tix;
	
	tix=td.vtype&4095;
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
