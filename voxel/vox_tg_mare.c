/**
 * Terrain generation for Maretia
 * Maretia is similar to the MC overworld.
 */

BGBDT_VoxWorld *voxtg_world;

BGBDT_VoxData voxtg_td_air;
BGBDT_VoxData voxtg_td_airsky;
BGBDT_VoxData voxtg_td_snofce;
BGBDT_VoxData voxtg_td_sky;

BGBDT_VoxData voxtg_td_stone;
BGBDT_VoxData voxtg_td_hardstone;
BGBDT_VoxData voxtg_td_dirt;
BGBDT_VoxData voxtg_td_grass;
BGBDT_VoxData voxtg_td_sand;

BGBDT_VoxData voxtg_td_log;
BGBDT_VoxData voxtg_td_leaves;
BGBDT_VoxData voxtg_td_grassclump;
BGBDT_VoxData voxtg_td_fruitleaves;

BGBDT_VoxData voxtg_td_lava;
BGBDT_VoxData voxtg_td_water;

int BGBDT_VoxTg_SetupGenerate(
	BGBDT_VoxWorld *world)
{
	int vty;

	if(voxtg_world==world)
		return(0);
		
	voxtg_world=world;

	memset(&voxtg_td_air		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_airsky		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_snofce		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_sky		, 0, sizeof(BGBDT_VoxData));

	memset(&voxtg_td_stone		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_hardstone	, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_dirt		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_grass		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_sand		, 0, sizeof(BGBDT_VoxData));

	memset(&voxtg_td_log		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_leaves		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_grassclump	, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_fruitleaves, 0, sizeof(BGBDT_VoxData));

	memset(&voxtg_td_lava		, 0, sizeof(BGBDT_VoxData));
	memset(&voxtg_td_water		, 0, sizeof(BGBDT_VoxData));

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "air");
	voxtg_td_air.vtypel=vty;
	voxtg_td_air.vtypeh=vty>>8;
	voxtg_td_air.vattr=0;
	voxtg_td_air.vlight=0;
	voxtg_td_air.alight=0;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "air");
	voxtg_td_airsky.vtypel=vty;
	voxtg_td_airsky.vtypeh=vty>>8;
	voxtg_td_airsky.vattr=0;
	voxtg_td_airsky.vlight=0;
	voxtg_td_airsky.alight=0xF9;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "solidnoface");
	voxtg_td_snofce.vtypel=vty;
	voxtg_td_snofce.vtypeh=vty>>8;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "sky1");
	voxtg_td_sky.vtypel=vty;
	voxtg_td_sky.vtypeh=vty>>8;


	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "stone");
	voxtg_td_stone.vtypel=vty;
	voxtg_td_stone.vtypeh=vty>>8;
	voxtg_td_stone.vattr=0;
	voxtg_td_stone.vlight=0;
	voxtg_td_stone.alight=0;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "hardstone");
	voxtg_td_hardstone.vtypel=vty;
	voxtg_td_hardstone.vtypeh=vty>>8;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "dirt");
	voxtg_td_dirt.vtypel=vty;
	voxtg_td_dirt.vtypeh=vty>>8;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "grass");
	voxtg_td_grass.vtypel=vty;
	voxtg_td_grass.vtypeh=vty>>8;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "sand");
	voxtg_td_sand.vtypel=vty;
	voxtg_td_sand.vtypeh=vty>>8;


	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "log");
	voxtg_td_log.vtypel=vty;
	voxtg_td_log.vtypeh=vty>>8;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "leaves");
	voxtg_td_leaves.vtypel=vty;
	voxtg_td_leaves.vtypeh=vty>>8;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "grassclump");
	voxtg_td_grassclump.vtypel=vty;
	voxtg_td_grassclump.vtypeh=vty>>8;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "fruitleaves");
	voxtg_td_fruitleaves.vtypel=vty;
	voxtg_td_fruitleaves.vtypeh=vty>>8;


	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "lava");
	voxtg_td_lava.vtypel=vty;
	voxtg_td_lava.vtypeh=vty>>8;
	voxtg_td_lava.vattr=0;
	voxtg_td_lava.vlight=0xF4;
	voxtg_td_lava.alight=0;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "water");
	voxtg_td_water.vtypel=vty;
	voxtg_td_water.vtypeh=vty>>8;

	return(0);
}

int bgbdt_voxtg_seed;

int bgbdt_voxtg_vxrand()
{
	int j;
	j=(bgbdt_voxtg_seed>>16)&65535;
	bgbdt_voxtg_seed=bgbdt_voxtg_seed*65521+1;
	return(j);
}

int bgbdt_voxtg_vxsrand(int seed)
{
	bgbdt_voxtg_seed=seed;
	return(0);
}

int BGBDT_VoxTgMare_GenerateTree(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord xyz)
{
	BGBDT_VoxCoord xyz0, xyz1, xyz2;
	BGBDT_VoxData td;
	int x, y, z, z1;
	int h, r, d, fl;
	
	h=4+((bgbdt_voxtg_vxrand())&3);

	for(z=0; z<h; z++)
	{
		xyz0=xyz;
		xyz0.z+=(z<<BGBDT_XYZ_SHR_VOXEL);

		BGBDT_WorldGetVoxelData(world, xyz0, &td, NULL,
			BGBDT_ACCFL_ENNEWCHK);
//		fl=BGBDT_WorldVoxel_GetFlags(world, xyz0, td);
		if(!BGBDT_WorldVoxel_CheckAirP(world, xyz0, td))
			break;
	}
	
	if(z<h)
	{
		return(-1);
	}

	for(z=-2; z<3; z++)
		for(y=-2; y<3; y++)
			for(x=-2; x<3; x++)
	{
		d=x*x+y*y+z*z;
//		d=(d-2)+(bgbdt_voxtg_vxrand()&3);
//		if(d>(2*2+2*2+2*2))
//		if(d>8)
		if(d>(6+(bgbdt_voxtg_vxrand()&3)))
			continue;
	
		z1=z+h;
		xyz0=xyz;
		xyz0.x+=(x<<BGBDT_XYZ_SHR_VOXEL);
		xyz0.y+=(y<<BGBDT_XYZ_SHR_VOXEL);
		xyz0.z+=(z1<<BGBDT_XYZ_SHR_VOXEL);

		BGBDT_WorldGetVoxelData(world, xyz0, &td, NULL,
			BGBDT_ACCFL_ENNEWCHK);
		if(!BGBDT_WorldVoxel_CheckAirP(world, xyz0, td))
			continue;

//		BGBDT_WorldSetVoxelData(world, xyz0,
//			voxtg_td_air, BGBDT_ACCFL_ENNEWCHK);

		if(!(bgbdt_voxtg_vxrand()&63))
		{
			BGBDT_WorldSetVoxelData(world, xyz0, voxtg_td_fruitleaves, 0);
			continue;
		}

		BGBDT_WorldSetVoxelData(world, xyz0, voxtg_td_leaves, 0);
	}

	for(z=0; z<h; z++)
	{
		xyz0=xyz;
		xyz0.z+=(z<<BGBDT_XYZ_SHR_VOXEL);

//		BGBDT_WorldGetVoxelData(world, xyz0, &td, NULL,
//			BGBDT_ACCFL_ENNEWCHK);

//		BGBDT_WorldSetVoxelData(world, xyz0,
//			voxtg_td_air, BGBDT_ACCFL_ENNEWCHK);

		BGBDT_WorldSetVoxelData(world, xyz0, voxtg_td_log, 0);
	}

	return(0);
}
 
int BGBDT_VoxTgMare_GenerateChunkBasic(
	BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	BGBDT_NoiseSample tsamp, tsamp1;
	BGBDT_VoxCoord xyz0, xyz1, xyz2;
	BGBDT_VoxData td, td1;
	float f0, f1, f2, f3;
	float f4, f5, f6, f7;
	float f, g;
	int x, y, z, zbi, zli, rsk, vz, vty;
	int i, j, k;

	memset(&tsamp, 0, sizeof(BGBDT_NoiseSample));
	memset(&tsamp1, 0, sizeof(BGBDT_NoiseSample));

	BGBDT_VoxTg_SetupGenerate(world);
	
//	zbi=64<<BGBDT_XYZ_SHR_VOXEL;
	zbi=128<<BGBDT_XYZ_SHR_VOXEL;

//	if((chk->bz<=4) || (chk->bz>=12))
//	{
//		xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, 0, 0, 0);
//		BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, td_stone, 0);
//		return(0);
//	}

	if((chk->bz>=8) && (chk->bz<15))
	{
		xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, 0, 0, 0);
//		xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, x, y, z);
		BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
			voxtg_td_airsky, 0);
		return(0);
	}
	
	rsk=(chk->bx^chk->by^chk->bz)*251+world->wrlseed;
	rsk=rsk*65521+1;
	bgbdt_voxtg_vxsrand(rsk);
	for(z=0; z<16; z++)
		for(y=0; y<16; y++)
			for(x=0; x<16; x++)
	{	
		xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, x, y, z);

		xyz1=xyz0;
		xyz1.z=64<<BGBDT_XYZ_SHR_VOXEL;

		vz=xyz0.z>>BGBDT_XYZ_SHR_VOXEL;
//		if((vz==0) || (vz==127))
		if(vz==0)
		{
			BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, voxtg_td_snofce, 0);
			continue;
		}

//		if(vz==127)
		if(vz==255)
		{
			BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, voxtg_td_sky, 0);

			xyz2=xyz0;
			xyz2.z=127<<BGBDT_XYZ_SHR_VOXEL;
//			BGBDT_WorldSetChunkVoxelData(world, chk, xyz2, voxtg_td_airsky, 0);
			BGBDT_WorldSetVoxelData(world, xyz2, voxtg_td_airsky, 0);

			continue;
		}

		if(vz>=128)
		{
			BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
				voxtg_td_airsky, 0);
			continue;
		}

		BGBDT_WorldGetChunkVoxelData(world, x, y, z,
			chk, &td, NULL, 0);
//		if(td.vtypel || td.vtypeh)
//			continue;

//		if(td.vtypel &&
//				((td.vtypel==voxtg_td_log.vtypel) ||
//				 (td.vtypel==voxtg_td_leaves.vtypel)))

		if(td.vtypel && (td.vtypel!=chk->voxinfo[0].vtypel))
			continue;

		f0=BGBDT_VoxelWorld_NoiseChk(world, &tsamp, xyz0, 0);
		f1=BGBDT_VoxelWorld_NoiseVox4(world, &tsamp, xyz0, 0);

		f4=BGBDT_VoxelWorld_NoiseChk4(world, &tsamp1, xyz1, 7);
		f5=BGBDT_VoxelWorld_NoiseChk(world, &tsamp1, xyz1, 7);
		f6=BGBDT_VoxelWorld_NoiseVox4(world, &tsamp1, xyz1, 7);
		f7=BGBDT_VoxelWorld_NoiseChk16(world, &tsamp1, xyz1, 7);
//		f7=(5*f4+3*f5+f6)/9;
//		f7=(0.7*f7+0.5*f4+0.3*f5+0.1*f6);
//		f7=(1.1*f7+0.5*f4+0.3*f5+0.1*f6);
		f7=(0.9*f7+0.5*f4+0.3*f5+0.05*f6);
		
//		zli=70+f7*32;
		zli=70+f7*28;
		if(zli<44)zli=44;
//		if(zli>112)zli=112;
		if(zli>120)zli=120;

//		f0=btge_scurve(f0);
//		f1=btge_scurve(f1);

//		f1=sin(x*(M_PI/8))+sin(y*(M_PI/8));

//		f2=((xyz0.z-zbi)*BGBDT_XYZ_SCALE_TOCHUNK)*0.25;
		f2=((xyz0.z-zbi)*BGBDT_XYZ_SCALE_TOCHUNK)*0.5;
//		f2=1.25-f2*f2;
//		f2=2.75-f2*f2;
//		if(f2<0)f2=0;
//		f2=btge_scurve(f2);

//		f2=f7*0.5;
		f2=f7*1.0-0.5;

		f=2*f0+(f1*0.5-0.25)+f2-0.15;
		
//		f=f*f;

//		if(f>0.5)
		if(vz>zli)
		{
			if(vz<64)
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					voxtg_td_water, 0);
			}else
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					voxtg_td_airsky, 0);
			}
		}else if(f>0.5)
		{
			if(vz<24)
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, voxtg_td_lava, 0);
			}else
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, voxtg_td_air, 0);
			}
		}else if((vz+4)>=zli)
		{
//			if(vz<(64+3))
			if((vz<(64+3)) && (zli<68))
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					voxtg_td_sand, 0);
			}else if(vz==zli)
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					voxtg_td_grass, 0);

				xyz2=xyz0;
				xyz2.z+=1<<BGBDT_XYZ_SHR_VOXEL;

				j=(rsk>>24)&255;
				rsk=rsk*251+1;
//				if((j>248) && (vz<(128-16)))
				if((j>252) && (vz<(128-16)))
				{
					BGBDT_VoxTgMare_GenerateTree(world, xyz2);
				}else if((j>224) && (vz<(128-16)))
				{
					td1=voxtg_td_grassclump;
					td1.vattr=bgbdt_voxtg_vxrand()&15;
//					BGBDT_WorldSetChunkVoxelData(world, chk, xyz2,
//						td1, 0);
					BGBDT_WorldSetVoxelData(world,
						xyz2, td1, BGBDT_ACCFL_ENNEWCHK);
				}

			}else
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					voxtg_td_dirt, 0);
			}
		}else
		{
			j=(rsk>>24)&255;
			rsk=rsk*251+1;
			if(j>240)
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					voxtg_td_hardstone, 0);
			}else if(j>192)
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					voxtg_td_dirt, 0);
			}else
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					voxtg_td_stone, 0);
			}
		}
	}
	
	BGBDT_VoxLight_UpdateChunkLight(world, chk);
	
	return(0);
}
