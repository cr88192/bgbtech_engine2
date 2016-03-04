#include <bteifgl.h>

BGBDT_VoxWorld *BGBDT_AllocVoxelWorld()
{
	BGBDT_VoxWorld *tmp;
	
	tmp=frgl_malloc(sizeof(BGBDT_VoxWorld));
	return(tmp);
}

int bgbdt_log2f2(int v)
{
	int i, j, k;
	
	i=v; j=0;
	while(i>4)
		{ i=(i+1)>>1; j++; }
//		{ i=i>>1; j++; }
	k=(j<<2)|(i&3);
	return(k);
}

int bgbdt_exp2f2(int v)
{
	int i, j;
	i=4|(v&3); j=v>>2;
	while(j>0)
		{ i=i<<1; j--; }
	return(i);
}

void *BGBDT_WorldAllocVoxelTemp(BGBDT_VoxWorld *world, int sz)
{
	void *buf;
	int ix, sz2;
	
	ix=bgbdt_log2f2(sz);
	if(world->freetmp[ix])
	{
		buf=world->freetmp[ix];
		world->freetmp[ix]=*(void **)buf;
		memset(buf, 0, sz);
		return(buf);
	}
	
	sz2=bgbdt_exp2f2(ix);
	buf=frgl_malloc(sz2);
	return(buf);
}

void BGBDT_WorldFreeVoxelTemp(BGBDT_VoxWorld *world, void *buf, int sz)
{
	int ix, sz2;
	
	ix=bgbdt_log2f2(sz);
	*(void **)buf=world->freetmp[ix];
	world->freetmp[ix]=buf;

//	frgl_free(buf);
}

void *BGBDT_WorldReallocVoxelTemp(BGBDT_VoxWorld *world,
	void *oldbuf, int oldsz, int newsz)
{
	void *buf;
	
	buf=BGBDT_WorldAllocVoxelTemp(world, newsz);
	memcpy(buf, oldbuf, oldsz);
	BGBDT_WorldFreeVoxelTemp(world, oldbuf, oldsz);
	return(buf);
}

int BGBDT_VoxelWorld_NoisePRandR(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord xyz, int k)
{
	u64 v;
	int bx, by, bz, bw;
	
	bx=xyz.x>>BGBDT_XYZ_SHR_REGION_XY;
	by=xyz.y>>BGBDT_XYZ_SHR_REGION_XY;
	bz=xyz.z>>BGBDT_XYZ_SHR_REGION_Z;

	bw=(bx^by^bz)>>8;
	bx=world->seedperm_x[bx&255];
	by=world->seedperm_y[by&255];
	bz=world->seedperm_z[bz&255];
	bw=world->seedperm_w[bw&255];
	
	v=((u64)world->wrlseed)*65521+k;
	v=(v+bz)*65521;	v=(v+by)*65521;	v=(v+bx)*65521;	v=(v+bw)*65521;
	v=(v+bx)*65521;	v=(v+by)*65521;	v=(v+bz)*65521;	v=(v+bw)*65521;
//	v=(v+bz)*65521;	v=(v+by)*65521;	v=(v+bx)*65521;
	v=v*2147483647;		//Mersenne Prime 31
	
	return((v>>48)&65535);
}

int BGBDT_VoxelWorld_NoisePRandC(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord xyz, int k)
{
	u64 v;
	int bx, by, bz, bw, bi;
	
	bx=xyz.x>>BGBDT_XYZ_SHR_CHUNK;
	by=xyz.y>>BGBDT_XYZ_SHR_CHUNK;
	bz=xyz.z>>BGBDT_XYZ_SHR_CHUNK;
	bi=bx^by^bz;

	bw=bi>>8;
	bx=world->seedperm_x[bx&255];
	by=world->seedperm_y[by&255];
	bz=world->seedperm_z[bz&255];
	bw=world->seedperm_w[bw&255];
	
	v=((u64)world->wrlseed)*65521+k;
//	v=(v+bi)*65521;
	v=(v+bz)*65521;	v=(v+by)*65521;	v=(v+bx)*65521;	v=(v+bw)*65521;
	v=(v-bx)*65521;	v=(v-by)*65521;	v=(v-bz)*65521;	v=(v-bw)*65521;
//	v=(v+bx)*65521;	v=(v+by)*65521;	v=(v+bz)*65521;
//	v=(v+bz)*65521;	v=(v+by)*65521;	v=(v+bx)*65521;
	v=v*2147483647;		//Mersenne Prime 31
	
	return((v>>48)&65535);
}

#if 0
int BGBDT_VoxelWorld_NoisePRandV(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord xyz, int k)
{
	u64 v;
	int bx, by, bz;
	
	bx=xyz.x>>BGBDT_XYZ_SHR_VOXEL;
	by=xyz.y>>BGBDT_XYZ_SHR_VOXEL;
	bz=xyz.z>>BGBDT_XYZ_SHR_VOXEL;
	
	v=((u64)world->wrlseed)*65521+k;
	v=(v+bz)*65521;	v=(v+by)*65521;	v=(v+bx)*65521;
	v=(v+bx)*65521;	v=(v+by)*65521;	v=(v+bz)*65521;
//	v=(v+by)*65521;	v=(v+bx)*65521;	v=(v+bz)*65521;

//	v=(v+bz)*65521;	v=(v+by)*65521;	v=(v+bx)*65521;
//	v=(v+bz)*65521;	v=(v+by)*65521;	v=(v+bx)*65521;
	v=v*2147483647;		//Mersenne Prime 31
	
//	return((v>>32)&65535);
	return((v>>48)&65535);
}
#endif

#if 1
int BGBDT_VoxelWorld_NoisePRandV(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord xyz, int k)
{
	u32 v;
	int bx, by, bz, bi;
	
	bx=xyz.x>>BGBDT_XYZ_SHR_VOXEL;
	by=xyz.y>>BGBDT_XYZ_SHR_VOXEL;
	bz=xyz.z>>BGBDT_XYZ_SHR_VOXEL;
	
//	v=world->wrlseed*65521+k;
//	v=(v+bz)*65521;	v=(v+by)*65521;	v=(v+bx)*65521;

	bi=bx^by^bz;
//	v=world->wrlseed*251+k;
//	v=(v+bi)*251;
//	v=(v+bz)*251;	v=(v+by)*251;	v=(v+bx)*251;

	v=world->wrlseed*127+k;
	v=(v+bi)*127;
	v=(v+bz)*127;	v=(v+by)*127;	v=(v+bx)*127;
	v=(v-bx)*127;	v=(v-by)*127;	v=(v-bz)*127;

//	v=(v^world->wrlseed+k)*251;
//	v=v*251*251;
	return((v>>16)&65535);
}
#endif

void BGBDT_VoxelWorld_InitSamplerChk(
	BGBDT_VoxWorld *world, BGBDT_NoiseSample *samp,
	BGBDT_VoxCoord xyz, int kf)
{
	BGBDT_VoxCoord xyz0, xyz1, xyz2;
	int bx, by, bz, bh;
	int tx, ty, tz;
	int i, j, k;
	
	bx=xyz.x>>BGBDT_XYZ_SHR_CHUNK;
	by=xyz.y>>BGBDT_XYZ_SHR_CHUNK;
	bz=xyz.z>>BGBDT_XYZ_SHR_CHUNK;
	
	bh=(((bx*251+by)*251+bz)*251+kf)*251;

	if(samp->bh==bh)
		return;

	samp->bh=bh;
		
	xyz0.x=xyz.x&(~((1<<BGBDT_XYZ_SHR_CHUNK)-1));
	xyz0.y=xyz.y&(~((1<<BGBDT_XYZ_SHR_CHUNK)-1));
	xyz0.z=xyz.z&(~((1<<BGBDT_XYZ_SHR_CHUNK)-1));
	xyz1.x=xyz0.x+(1<<BGBDT_XYZ_SHR_CHUNK);
	xyz1.y=xyz0.y+(1<<BGBDT_XYZ_SHR_CHUNK);
	xyz1.z=xyz0.z+(1<<BGBDT_XYZ_SHR_CHUNK);
	
	for(i=0; i<8; i++)
	{
		xyz2.x=(i&1)?xyz1.x:xyz0.x;
		xyz2.y=(i&2)?xyz1.y:xyz0.y;
		xyz2.z=(i&4)?xyz1.z:xyz0.z;
		samp->lvt[i]=BGBDT_VoxelWorld_NoisePRandC(world, xyz2, kf);
		samp->lvf[i]=(samp->lvt[i]*(1.0/32767.5))-1.0;
	}

#if 0
//	for(i=0; i<64; i++)
	for(tz=0; tz<5; tz++)
		for(ty=0; ty<5; ty++)
			for(tx=0; tx<5; tx++)
	{
		i=tz*25+ty*5+tx;
	
		xyz2.x=xyz0.x+(tx<<BGBDT_XYZ_SHR_VOXEL);
		xyz2.y=xyz0.y+(ty<<BGBDT_XYZ_SHR_VOXEL);
		xyz2.z=xyz0.z+(tz<<BGBDT_XYZ_SHR_VOXEL);
		samp->lvs[i]=BGBDT_VoxelWorld_NoisePRandV(world, xyz2, kf);
		samp->lvg[i]=(samp->lvs[i]*(1.0/32767.5))-1.0;
	}
#endif

#if 1
	k=(bh*65521+world->wrlseed)*65521;
	for(i=0; i<(5*5*5); i++)
	{
		j=(k>>16)&65535;
		k=k*65521+1;

		samp->lvs[i]=j;
		samp->lvg[i]=(j*(1.0/32767.5))-1.0;
	}
#endif
}

float BGBDT_VoxelWorld_NoiseChk(
	BGBDT_VoxWorld *world, BGBDT_NoiseSample *samp,
	BGBDT_VoxCoord xyz, int k)
{
	BGBDT_VoxCoord xyz0, xyz1, xyz2, xyzf;
	float fv[8];
	float fx, fy, fz;
	float f, g;

	BGBDT_VoxelWorld_InitSamplerChk(world, samp, xyz, k);

	xyzf.x=xyz.x&((1<<BGBDT_XYZ_SHR_CHUNK)-1);
	xyzf.y=xyz.y&((1<<BGBDT_XYZ_SHR_CHUNK)-1);
	xyzf.z=xyz.z&((1<<BGBDT_XYZ_SHR_CHUNK)-1);
	
	fx=xyzf.x*BGBDT_XYZ_SCALE_TOCHUNK;
	fy=xyzf.y*BGBDT_XYZ_SCALE_TOCHUNK;
	fz=xyzf.z*BGBDT_XYZ_SCALE_TOCHUNK;
	
	fv[0]=(samp->lvf[0]*(1.0-fx))+(samp->lvf[1]*fx);
	fv[1]=(samp->lvf[2]*(1.0-fx))+(samp->lvf[3]*fx);
	fv[2]=(samp->lvf[4]*(1.0-fx))+(samp->lvf[5]*fx);
	fv[3]=(samp->lvf[6]*(1.0-fx))+(samp->lvf[7]*fx);

	fv[4]=(fv[0]*(1.0-fy))+(fv[1]*fy);
	fv[5]=(fv[2]*(1.0-fy))+(fv[3]*fy);
	f=(fv[4]*(1.0-fz))+(fv[5]*fz);
	return(f);
}

float BGBDT_VoxelWorld_NoiseVox4(
	BGBDT_VoxWorld *world, BGBDT_NoiseSample *samp,
	BGBDT_VoxCoord xyz, int k)
{
	BGBDT_VoxCoord xyzf;
	int bx, by, bz, bi;
	float fv[8];
	float fx, fy, fz;
	float f, g;

	BGBDT_VoxelWorld_InitSamplerChk(world, samp, xyz, k);

	bx=(xyz.x>>BGBDT_XYZ_SHR_VOXEL4)&3;
	by=(xyz.y>>BGBDT_XYZ_SHR_VOXEL4)&3;
	bz=(xyz.z>>BGBDT_XYZ_SHR_VOXEL4)&3;
	bi=bz*25+by*5+bx;

	xyzf.x=xyz.x&((1<<BGBDT_XYZ_SHR_VOXEL4)-1);
	xyzf.y=xyz.y&((1<<BGBDT_XYZ_SHR_VOXEL4)-1);
	xyzf.z=xyz.z&((1<<BGBDT_XYZ_SHR_VOXEL4)-1);
	
	fx=xyzf.x*BGBDT_XYZ_SCALE_TOVOX4;
	fy=xyzf.y*BGBDT_XYZ_SCALE_TOVOX4;
	fz=xyzf.z*BGBDT_XYZ_SCALE_TOVOX4;
	
	fv[0]=(samp->lvg[bi+ 0]*(1.0-fx))+(samp->lvg[bi+ 1]*fx);
	fv[1]=(samp->lvg[bi+ 5]*(1.0-fx))+(samp->lvg[bi+ 6]*fx);
	fv[2]=(samp->lvg[bi+25]*(1.0-fx))+(samp->lvg[bi+26]*fx);
	fv[3]=(samp->lvg[bi+30]*(1.0-fx))+(samp->lvg[bi+31]*fx);
	fv[4]=(fv[0]*(1.0-fy))+(fv[1]*fy);
	fv[5]=(fv[2]*(1.0-fy))+(fv[3]*fy);
	f=(fv[4]*(1.0-fz))+(fv[5]*fz);
	return(f);
}

float btge_scurve(float a)
	{ return(a*a*(3-2*a)); }

int BGBDT_VoxelWorld_GenerateChunkBasic(
	BGBDT_VoxWorld *world, BGBDT_VoxChunk *chk)
{
	BGBDT_NoiseSample tsamp;
	BGBDT_VoxCoord xyz0;
	BGBDT_VoxData td_air, td_stone, td_hardstone;
	BGBDT_VoxData td_dirt, td_snofce, td_lava;
	float f0, f1, f2, f3;
	float f, g;
	int x, y, z, zbi, rsk, vz;
	int i, j, k;

	memset(&tsamp, 0, sizeof(BGBDT_NoiseSample));
	
	td_air.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world, "air");
	td_air.vattr=0;
//	td_air.vlight=0xFF;
//	td_air.alight=0xFF;
	td_air.vlight=0;
	td_air.alight=0;


	td_stone.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world, "stone");
	td_stone.vattr=0;	td_stone.vlight=0;	td_stone.alight=0;

	td_hardstone.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world,
		"hardstone");
	td_hardstone.vattr=0;	td_hardstone.vlight=0;	td_hardstone.alight=0;

	td_dirt.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world, "dirt");
	td_dirt.vattr=0;	td_dirt.vlight=0;	td_dirt.alight=0;

	td_snofce.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world,
		"solidnoface");
	td_snofce.vattr=0;	td_snofce.vlight=0;	td_snofce.alight=0;

	td_lava.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world, "lava");
	td_lava.vattr=0;	td_lava.vlight=0xF4;	td_lava.alight=0;
	
	zbi=64<<BGBDT_XYZ_SHR_VOXEL;

//	if((chk->bz<=4) || (chk->bz>=12))
//	{
//		xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, 0, 0, 0);
//		BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, td_stone, 0);
//		return(0);
//	}
	
	rsk=(chk->bx^chk->by^chk->bz)*251+world->wrlseed;
	rsk=rsk*65521;
	for(z=0; z<16; z++)
		for(y=0; y<16; y++)
			for(x=0; x<16; x++)
	{	
		xyz0=BGBDT_WorldGetChunkVoxCoord(world, chk, x, y, z);

		vz=xyz0.z>>BGBDT_XYZ_SHR_VOXEL;
		if((vz==0) || (vz==127))
		{
			BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, td_snofce, 0);
			continue;
		}
	
		f0=BGBDT_VoxelWorld_NoiseChk(world, &tsamp, xyz0, 0);
		f1=BGBDT_VoxelWorld_NoiseVox4(world, &tsamp, xyz0, 0);

//		f0=btge_scurve(f0);
//		f1=btge_scurve(f1);

//		f1=sin(x*(M_PI/8))+sin(y*(M_PI/8));

//		f2=((xyz0.z-zbi)*BGBDT_XYZ_SCALE_TOCHUNK)*0.25;
		f2=((xyz0.z-zbi)*BGBDT_XYZ_SCALE_TOCHUNK)*0.5;
		f2=1.25-f2*f2;
//		f2=btge_scurve(f2);

		f=2*f0+(f1*0.5-0.25)+f2;
		
//		f=f*f;
		
		if(f>0.5)
		{
			if(vz<24)
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, td_lava, 0);
			}else
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, td_air, 0);
			}
		}else
		{
			j=(rsk>>24)&255;
			rsk=rsk*251;
			if(j>240)
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					td_hardstone, 0);
			}else if(j>192)
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0,
					td_dirt, 0);
			}else
			{
				BGBDT_WorldSetChunkVoxelData(world, chk, xyz0, td_stone, 0);
			}
		}
	}
	
	BGBDT_VoxLight_UpdateChunkLight(world, chk);
	
	return(0);
}

BTEIFGL_API BGBDT_VoxWorld *BGBDT_CreateBasicWorld(char *name)
{
	BGBDT_VoxTypeInfo *tyi;
	BGBDT_VoxWorld *wrl;
	char *s;
	int i0, i1, i2, i3;
	int i, j, k, h;
	
	wrl=BGBDT_AllocVoxelWorld();
	wrl->GenerateChunk=BGBDT_VoxelWorld_GenerateChunkBasic;
	
	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "null");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=0; tyi->mat_side_y=0;
	tyi->mat_top_x=0; tyi->mat_top_y=0;
	tyi->mat_bot_x=0; tyi->mat_bot_y=0;
	tyi->flags=BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NOFACES;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "air");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=0; tyi->mat_side_y=0;
	tyi->mat_top_x=0; tyi->mat_top_y=0;
	tyi->mat_bot_x=0; tyi->mat_bot_y=0;
	tyi->flags=BGBDT_VOXFL_NONSOLID|
		BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NOFACES;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "hardstone");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=1; tyi->mat_side_y=0;
	tyi->mat_top_x=1; tyi->mat_top_y=0;
	tyi->mat_bot_x=1; tyi->mat_bot_y=0;
	tyi->flags=0;
	
	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "dirt");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=2; tyi->mat_side_y=0;
	tyi->mat_top_x=2; tyi->mat_top_y=0;
	tyi->mat_bot_x=2; tyi->mat_bot_y=0;
	tyi->flags=0;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "grass");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=3; tyi->mat_side_y=0;
	tyi->mat_top_x=3; tyi->mat_top_y=0;
	tyi->mat_bot_x=3; tyi->mat_bot_y=0;
	tyi->flags=0;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "stone");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=4; tyi->mat_side_y=0;
	tyi->mat_top_x=4; tyi->mat_top_y=0;
	tyi->mat_bot_x=4; tyi->mat_bot_y=0;
	tyi->flags=0;
	
	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "bigbrick_brown");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=0; tyi->mat_side_y=1;
	tyi->mat_top_x=0; tyi->mat_top_y=1;
	tyi->mat_bot_x=0; tyi->mat_bot_y=1;
	tyi->flags=0;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "bigbrick_gray");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=1; tyi->mat_side_y=1;
	tyi->mat_top_x=1; tyi->mat_top_y=1;
	tyi->mat_bot_x=1; tyi->mat_bot_y=1;
	tyi->flags=0;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "brick_red");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=2; tyi->mat_side_y=1;
	tyi->mat_top_x=2; tyi->mat_top_y=1;
	tyi->mat_bot_x=2; tyi->mat_bot_y=1;
	tyi->flags=0;
	
	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "stone_red");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=3; tyi->mat_side_y=1;
	tyi->mat_top_x=3; tyi->mat_top_y=1;
	tyi->mat_bot_x=3; tyi->mat_bot_y=1;
	tyi->flags=0;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "solidnoface");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=4; tyi->mat_side_y=1;
	tyi->mat_top_x=4; tyi->mat_top_y=1;
	tyi->mat_bot_x=4; tyi->mat_bot_y=1;
	tyi->flags=BGBDT_VOXFL_NOFACES;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "glass");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=5; tyi->mat_side_y=1;
	tyi->mat_top_x=5; tyi->mat_top_y=1;
	tyi->mat_bot_x=5; tyi->mat_bot_y=1;
	tyi->flags=BGBDT_VOXFL_TRANSPARENT;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "lava");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=0; tyi->mat_side_y=2;
	tyi->mat_top_x=0; tyi->mat_top_y=2;
	tyi->mat_bot_x=0; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_FLUID_LAVA|
		BGBDT_VOXFL_NONSOLID|BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xF4;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "water");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=1; tyi->mat_side_y=2;
	tyi->mat_top_x=1; tyi->mat_top_y=2;
	tyi->mat_bot_x=1; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_FLUID_WATER|BGBDT_VOXFL_NONSOLID;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "slime");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=2; tyi->mat_side_y=2;
	tyi->mat_top_x=2; tyi->mat_top_y=2;
	tyi->mat_bot_x=2; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_FLUID_SLIME|
		BGBDT_VOXFL_NONSOLID|BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xF2;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "lantern_red");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=3; tyi->mat_side_y=2;
	tyi->mat_top_x=3; tyi->mat_top_y=2;
	tyi->mat_bot_x=3; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xF4;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "lantern_blue");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=4; tyi->mat_side_y=2;
	tyi->mat_top_x=4; tyi->mat_top_y=2;
	tyi->mat_bot_x=4; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xF1;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "lantern_yellow");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=5; tyi->mat_side_y=2;
	tyi->mat_top_x=5; tyi->mat_top_y=2;
	tyi->mat_bot_x=5; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xF6;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "lantern_green");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=6; tyi->mat_side_y=2;
	tyi->mat_top_x=6; tyi->mat_top_y=2;
	tyi->mat_bot_x=6; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xF2;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "lantern_cyan");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=7; tyi->mat_side_y=2;
	tyi->mat_top_x=7; tyi->mat_top_y=2;
	tyi->mat_bot_x=7; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xF3;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "lantern_violet");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=8; tyi->mat_side_y=2;
	tyi->mat_top_x=8; tyi->mat_top_y=2;
	tyi->mat_bot_x=8; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xF5;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "lantern_white");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=9; tyi->mat_side_y=2;
	tyi->mat_top_x=9; tyi->mat_top_y=2;
	tyi->mat_bot_x=9; tyi->mat_bot_y=2;
	tyi->flags=BGBDT_VOXFL_GLOWLIGHT;
	tyi->glow=0xFF;

	s=name; h=0;
	while(*s)
		h=(h*251)+(*s++);
	for(i=0; i<16; i++)
		h=(h*251)+i;
	wrl->wrlseed=h;
	
	for(i=0; i<256; i++)
	{
		wrl->seedperm_x[i]=i;
		wrl->seedperm_y[i]=i;
		wrl->seedperm_z[i]=i;
		wrl->seedperm_w[i]=i;
		h=h*251+1;
	}

	for(i=0; i<256; i++)
	{
		j=(h>>24)&255; h=h*251+1;
		i0=wrl->seedperm_x[i];	i1=wrl->seedperm_x[j];
		wrl->seedperm_x[i]=i1;	wrl->seedperm_x[j]=i0;
	}

	for(i=0; i<256; i++)
	{
		j=(h>>24)&255; h=h*251+1;
		i0=wrl->seedperm_y[i];	i1=wrl->seedperm_y[j];
		wrl->seedperm_y[i]=i1;	wrl->seedperm_y[j]=i0;
	}

	for(i=0; i<256; i++)
	{
		j=(h>>24)&255; h=h*251+1;
		i0=wrl->seedperm_z[i];	i1=wrl->seedperm_z[j];
		wrl->seedperm_z[i]=i1;	wrl->seedperm_z[j]=i0;
	}

	for(i=0; i<256; i++)
	{
		j=(h>>24)&255; h=h*251+1;
		i0=wrl->seedperm_w[i];	i1=wrl->seedperm_w[j];
		wrl->seedperm_w[i]=i1;	wrl->seedperm_w[j]=i0;
	}
	
	return(wrl);
}
