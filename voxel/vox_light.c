#include <bteifgl.h>

static const byte bgbdt_voxlight_blendtab[16*16]=
{
/*      0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
/*0*/ 0x0,0x9,0xA,0xB,0xC,0xD,0xE,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, 
/*1*/ 0x9,0x1,0x3,0x3,0x5,0x5,0x0,0x9,0x9,0x9,0xB,0xB,0xD,0xD,0x0,0x9,
/*2*/ 0xA,0x3,0x2,0x3,0x6,0x0,0x6,0xA,0xA,0xB,0xA,0x0,0xE,0x0,0xE,0xA,
/*3*/ 0xB,0x3,0x3,0x3,0x0,0x0,0x0,0xB,0xB,0xB,0xB,0xB,0x0,0x0,0x0,0xB,
/*4*/ 0xC,0x5,0x6,0x0,0x4,0x5,0x6,0xC,0xC,0xD,0xC,0x0,0xC,0xD,0xE,0xC,
/*5*/ 0xD,0x5,0x0,0x0,0x5,0x5,0x0,0xD,0xD,0xD,0x0,0x0,0xD,0xD,0x0,0xD,
/*6*/ 0xE,0x0,0x6,0x0,0x6,0x0,0x6,0xE,0xE,0x0,0xE,0x0,0xE,0x0,0xE,0xE,
/*7*/ 0xF,0x9,0xA,0xB,0xC,0xD,0xE,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF,
/*8*/ 0x0,0x9,0xA,0xB,0xC,0xD,0xE,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0xF,
/*9*/ 0x0,0x9,0xB,0xB,0xD,0xD,0x0,0x0,0x0,0x9,0xB,0xB,0xD,0xD,0x0,0xF,
/*A*/ 0x0,0xB,0xA,0xB,0xC,0x0,0xE,0x0,0x0,0xE,0xA,0xB,0xE,0x0,0xE,0xF,
/*B*/ 0x0,0xB,0x0,0xB,0x0,0x0,0x0,0x0,0x0,0xB,0xB,0xB,0x0,0x0,0x0,0xF,
/*C*/ 0x0,0xD,0xE,0x0,0xC,0xD,0xE,0x0,0x0,0xD,0xE,0x0,0xC,0xD,0xE,0xF,
/*D*/ 0x0,0xD,0x0,0x0,0xD,0xD,0x0,0x0,0x0,0xD,0x0,0x0,0xD,0xD,0x0,0xF,
/*E*/ 0x0,0x0,0xE,0x0,0xE,0x0,0xE,0x0,0x0,0x0,0xE,0x0,0xE,0x0,0xE,0xF,
/*F*/ 0x0,0x9,0xA,0xB,0xC,0xD,0xE,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,
};

int bgbdt_voxlight_blendv(int la, int lb, int vfl)
{

	int lc;
	if((la>>4)>(lb>>4))
		{ return(la); }

	if((la>>4)==(lb>>4))
	{
		lc=(la&0xF0)|bgbdt_voxlight_blendtab[((la&15)<<4)|(lb&15)];
		return(lc);
	}

	if(vfl&BGBDT_VOXFL_FLUID)
	{
		lc=la;
		if((la>>4)<((lb>>4)-4))
			lc=((lb-64)&0xF0)|9;
		return(lc);
	}

	if((la>>4)<((lb>>4)-2))
		return(lb-16);
	lc=((lb-16)&0xF0)|bgbdt_voxlight_blendtab[((la&15)<<4)|(lb&15)];
	return(lc);
}

int bgbdt_voxlight_blenda(int la, int lb, int ix, int vfl)
{
	int lc;
	
	if(vfl&BGBDT_VOXFL_FLUID)
	{
		lc=la;
		if((lb&0xF8)>((la+32)&0xF8))
			lc=lb-32;
		return(lc);
	}

	lc=la;
	if(((lb&7)>=1) && ((lb&7)<=3))
	{
//		if(ix<4)
		if(ix<5)
		{
			if((lb&0xF8)>((la+8)&0xF8))
				lc=lb-8;
		}else
		{
			if((lb&0xF8)>=(la&0xF8))
				lc=lb;
		}
	}else
	{
		if((lb&0xF8)>((la+8)&0xF8))
			lc=lb-8;
	}
	return(lc);
}

int BGBDT_VoxLight_UpdateChunkLight(
	BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	BGBDT_VoxData td, td1;
	BGBDT_VoxDataStatus tds;
	BGBDT_VoxCoord xyz0;
	BGBDT_VoxTypeInfo *tyi;
	int tix, ds, dst, fl, oldcfl;
	int x, y, z, w;
	int a, b, c;
	int vty, vty_air;
	int i, j, k;
	
	if(!chk)
		return(-1);
	if(!chk->voxinfo)
		return(-1);

	vty_air=BGBDT_VoxelWorld_LookupTypeIndexName(world, "air");

	if(chk->voxinfo && (chk->nvoxinfo==1))
	{
		vty=chk->voxinfo[0].vtypel|(chk->voxinfo[0].vtypeh<<8);
//		if((chk->voxinfo[0].vtype&4095)!=vty_air)
		if((vty&4095)!=vty_air)
			return(0);
		if(chk->voxinfo[0].alight<16)
			{ chk->voxinfo[0].alight=16; }
	}else
	{
		for(i=0; i<chk->nvoxinfo; i++)
		{
			vty=chk->voxinfo[i].vtypel|(chk->voxinfo[0].vtypeh<<8);
//			if((chk->voxinfo[i].vtype&4095)!=vty_air)
			if((vty&4095)!=vty_air)
				break;
		}
		if(i>=chk->nvoxinfo)
		{
			for(j=0; j<chk->nvoxinfo; j++)
			{
				if(chk->voxinfo[j].alight<16)
					{ chk->voxinfo[j].alight=16; }
			}
		}
	}

#if 0
	for(z=0; z<16; z++)
		for(y=0; y<16; y++)
			for(x=0; x<16; x++)
	{
		BGBDT_WorldGetChunkVoxelData(world,
			x, y, z, chk, &td, &tds,
			BGBDT_ACCFL_NOLOAD|BGBDT_ACCFL_CHKADJ);

		vty=td.vtypel|(td.vtypeh<<8);
		tix=vty&4095;
//			tix=td.vtype&4095;
		tyi=world->voxtypes[tix];
		if(!tyi)continue;

		if(!(tyi->flags&BGBDT_VOXFL_TRANSPARENT))
			continue;
//			if(tix!=vty_air)
//				continue;

		ds=0;
					
		if(td.alight>4)
		{
			td.alight=0;
			ds=1;
		}

		if(ds)
		{
			dst++;
			xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, x, y, z);
			BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, td, 
				BGBDT_ACCFL_CHKADJ|BGBDT_ACCFL_LIGHTDIRTY);
		}
	}
#endif

	oldcfl=chk->flags;
	for(w=0; w<6; w++)
	{
		dst=0;
		for(z=0; z<16; z++)
			for(y=0; y<16; y++)
				for(x=0; x<16; x++)
		{
			BGBDT_WorldGetChunkVoxelData(world,
				x, y, z, chk, &td, &tds,
				BGBDT_ACCFL_NOLOAD|BGBDT_ACCFL_CHKADJ);

			vty=td.vtypel|(td.vtypeh<<8);
			tix=vty&4095;
//			tix=td.vtype&4095;
			tyi=world->voxtypes[tix];
			if(!tyi)continue;

			if(!(tyi->flags&BGBDT_VOXFL_TRANSPARENT))
				continue;
//			if(tix!=vty_air)
//				continue;

			ds=0;
						
			for(i=0; i<6; i++)
			{
#if 0
				a=(tds.adjlit[i]>>8)&255;
				if(i<4)
				{
					if(a>(td.alight+8))
						{ td.alight=a-8; ds=1; }
				}else
				{
					if(a>(td.alight+1))
						{ td.alight=a-1; ds=1; }
				}
#endif

				a=(tds.adjlit[i]>>8)&255;
				c=bgbdt_voxlight_blenda(td.alight, a, i, tyi->flags);
				if(c!=td.alight)
					{ td.alight=c; ds=1; }

				b=tds.adjlit[i]&255;
				c=bgbdt_voxlight_blendv(td.vlight, b, tyi->flags);
				if(c!=td.vlight)
					{ td.vlight=c; ds=1; }
			}
			
			if(ds)
			{
				dst++;
				xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, x, y, z);
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, td, 
					BGBDT_ACCFL_CHKADJ|BGBDT_ACCFL_LIGHTDIRTY);
			}
		}
		if(!dst)
			break;
	}
	
	if((oldcfl&BGBDT_CHKFL_MESHDIRTY) &&
		!(chk->flags&BGBDT_CHKFL_MESHDIRTY))
	{
		printf("BGBDT_VoxLight_UpdateChunkLight: Flags Hack\n");
		chk->flags=oldcfl;
	}
	
	chk->flags&=~BGBDT_CHKFL_LIGHTDIRTY;
	return(0);
}

BTEIFGL_API int BGBDT_VoxLight_ClearBlockLightRadius(
	BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	BGBDT_VoxCoord xyz1;
	BGBDT_VoxData td;
	BGBDT_VoxDataStatus tds;
	int x, y, z;
	int i, j, k;
	
	for(z=-16; z<=16; z++)
		for(y=-16; y<=16; y++)
			for(x=-16; x<=16; x++)
	{
		xyz1.x=xyz.x+(x<<BGBDT_XYZ_SHR_VOXEL);
		xyz1.y=xyz.y+(y<<BGBDT_XYZ_SHR_VOXEL);
		xyz1.z=xyz.z+(z<<BGBDT_XYZ_SHR_VOXEL);
	
		i=BGBDT_WorldGetVoxelData(world, xyz1, &td, &tds, 0);
		if(i<0)
			continue;
		td.vlight=0;
		BGBDT_WorldSetVoxelData(world, xyz1, td, 0);
	}
	return(0);
}

BTEIFGL_API int BGBDT_VoxLight_ClearBlockAreaLightRadius(
	BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	BGBDT_VoxCoord xyz1;
	BGBDT_VoxData td;
	BGBDT_VoxDataStatus tds;
	int x, y, z;
	int i, j, k;
	
	for(z=-16; z<=16; z++)
		for(y=-32; y<=32; y++)
			for(x=-32; x<=32; x++)
	{
		xyz1.x=xyz.x+(x<<BGBDT_XYZ_SHR_VOXEL);
		xyz1.y=xyz.y+(y<<BGBDT_XYZ_SHR_VOXEL);
		xyz1.z=xyz.z+(z<<BGBDT_XYZ_SHR_VOXEL);
	
		i=BGBDT_WorldGetVoxelData(world, xyz1, &td, &tds, 0);
		if(i<0)
			continue;
		td.alight=0;
		BGBDT_WorldSetVoxelData(world, xyz1, td, 0);
	}
	return(0);
}

BTEIFGL_API int BGBDT_VoxLight_CheckClearBlockLightRadius(
	BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	BGBDT_VoxData td;
	BGBDT_VoxDataStatus tds;
	int ac;
	int a0, a1, a2, a3, a4, a5;
	int vfl;
	int i;

	i=BGBDT_WorldGetVoxelData(world, xyz, &td, &tds, 0);
	if(i<0)
		return(0);
	vfl=BGBDT_WorldVoxel_GetFlags(world, xyz, td);
	if(vfl&BGBDT_VOXFL_GLOWLIGHT)
	{
		BGBDT_VoxLight_ClearBlockLightRadius(world, xyz, accfl);
	}

#if 0
	if(td.alight && (td.alight&7) && ((td.alight&7)<4))
	{
		ac=td.alight;
		a0=(tds.adjlit[0]>>8)&255;
		a1=(tds.adjlit[1]>>8)&255;
		a2=(tds.adjlit[2]>>8)&255;
		a3=(tds.adjlit[3]>>8)&255;
		a5=(tds.adjlit[5]>>8)&255;
		
		if((a5&0xF8)<(ac&0xF8))
		{
			if(((a0&0xF8)<=(ac&0xF8)) &&
				((a1&0xF8)<=(ac&0xF8)) &&
				((a2&0xF8)<=(ac&0xF8)) &&
				((a3&0xF8)<=(ac&0xF8)))
			{
				BGBDT_VoxLight_ClearBlockAreaLightRadius(
					world, xyz, accfl);
			}
		}
	}
#endif
	
	return(0);
}

BTEIFGL_API int BGBDT_VoxLight_GetBlockLightIntensity(
	BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td)
{
	int i, j;
	
	i=(td.vlight>>4);
	j=(td.alight>>3);
	if(j>i)i=j;
	return(i);
}


int BGBDT_VoxEnt_UpdateChunkSpawnEnts(
	BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	BGBDT_VoxTypeInfo *tyi;
	BGBDT_VoxDataStatus tds;
	BGBDT_VoxData td;
	BGBDT_VoxCoord xyz;
	int vfl, vty, tix, vval, vt_entmob;
	char *vs0;
	int x, y, z;
	int i, j, k;

	if(chk->flags&BGBDT_CHKFL_ENTSPAWN)
		return(0);

	vt_entmob=BGBDT_VoxelWorld_LookupTypeIndexName(world, "ent_mob");

	k=0;
	for(i=0; i<chk->nvoxinfo; i++)
	{
		td=chk->voxinfo[i];
//		vfl=BGBDT_WorldVoxel_GetFlags(world, xyz, td);
//		vty=BGBDT_WorldVoxel_GetTypeID(world, xyz, td);
		vty=td.vtypel|(td.vtypeh<<8);
		tix=vty&4095;
		if(tix==vt_entmob)
			break;
	}
	
	if(i>=chk->nvoxinfo)
	{
		chk->flags|=BGBDT_CHKFL_ENTSPAWN;
		return(0);
	}


	for(z=0; z<16; z++)
		for(y=0; y<16; y++)
			for(x=0; x<16; x++)
	{
		BGBDT_WorldGetChunkVoxelData(world,
			x, y, z, chk, &td, &tds,
			BGBDT_ACCFL_NOLOAD);
//			BGBDT_ACCFL_NOLOAD|BGBDT_ACCFL_CHKADJ);

		vty=td.vtypel|(td.vtypeh<<8);
		tix=vty&4095;
		tyi=world->voxtypes[tix];
		if(!tyi)continue;
		if(tix!=vt_entmob)
			continue;
			
		xyz=BGBDT_WorldGetChunkVoxCoord(world, chk, x, y, z);
		vval=td.vdatal|(td.vdatah<<8);
		vs0=BGBDT_WorldGetStringForIndex(world, xyz, 0, vval);
		if(!vs0)continue;
		
		BGBDT_VoxEnt_CheckSpawnEnitity(world, xyz, td, vs0);
	}

	chk->flags|=BGBDT_CHKFL_ENTSPAWN;
	return(1);
}

BTEIFGL_API int BGBDT_VoxEnt_CheckSpawnEnitity(
	BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, BGBDT_VoxData td, char *cname)
{
	vec3d orgd;
	dtVal ent;
	s64 efl;

	if(td.vattr&0x40)
	{
		orgd=vec3d(
			xyz.x*BGBDT_XYZ_SCALE_TOMETER+0.5,
			xyz.y*BGBDT_XYZ_SCALE_TOMETER+0.5,
			xyz.z*BGBDT_XYZ_SCALE_TOMETER+0.99);
	}else
	{
		orgd=vec3d(
			xyz.x*BGBDT_XYZ_SCALE_TOMETER+0.5,
			xyz.y*BGBDT_XYZ_SCALE_TOMETER+0.5,
			xyz.z*BGBDT_XYZ_SCALE_TOMETER+0.01);
	}

	printf("Spawn %s @ (%.2f %.2f %.2f)\n",
		cname, v3dx(orgd), v3dy(orgd), v3dz(orgd));

	ent=Bt2Ent_SpawnEntityBasic(cname, orgd);
	if(!dtvNullP(ent))
	{
		if(td.vattr&0x40)
		{
			efl=Bt2Ent_EntGetFlags(ent);
			efl|=BGBDT_ENTFL_ZFLIP;
			Bt2Ent_EntSetFlags(ent, efl);
		}
	}
	
	return(0);
}

