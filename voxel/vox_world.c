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
//	while(i>4)
//	while(i>>3)
	while(i>=8)
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
//	int tlvt4a[8], tlvt4b[8];
	BGBDT_VoxCoord xyz0, xyz1, xyz2;
	int bx, by, bz, bh;
	int tx, ty, tz, kf2;
	int txa, txb, txc, txd;
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

#if 1
	xyz0.x=(bx>>2)<<(BGBDT_XYZ_SHR_CHUNK+2);
	xyz0.y=(by>>2)<<(BGBDT_XYZ_SHR_CHUNK+2);
	xyz0.z=(bz>>2)<<(BGBDT_XYZ_SHR_CHUNK+2);
	xyz1.x=xyz0.x+(4<<BGBDT_XYZ_SHR_CHUNK);
	xyz1.y=xyz0.y+(4<<BGBDT_XYZ_SHR_CHUNK);
	xyz1.z=xyz0.z+(4<<BGBDT_XYZ_SHR_CHUNK);

	kf2=(kf*251+1)>>8;
	for(i=0; i<8; i++)
	{
		xyz2.x=(i&1)?xyz1.x:xyz0.x;
		xyz2.y=(i&2)?xyz1.y:xyz0.y;
		xyz2.z=(i&4)?xyz1.z:xyz0.z;
		samp->lvt4[i]=BGBDT_VoxelWorld_NoisePRandC(world, xyz2, kf2);
		samp->lvf4[i]=(samp->lvt4[i]*(1.0/32767.5))-1.0;
	}
#endif

#if 1
	xyz0.x=(bx>>4)<<(BGBDT_XYZ_SHR_CHUNK+4);
	xyz0.y=(by>>4)<<(BGBDT_XYZ_SHR_CHUNK+4);
	xyz0.z=(bz>>4)<<(BGBDT_XYZ_SHR_CHUNK+4);
	xyz1.x=xyz0.x+(16<<BGBDT_XYZ_SHR_CHUNK);
	xyz1.y=xyz0.y+(16<<BGBDT_XYZ_SHR_CHUNK);
	xyz1.z=xyz0.z+(16<<BGBDT_XYZ_SHR_CHUNK);

	kf2=((kf+11)*251+1)>>8;
	for(i=0; i<8; i++)
	{
		xyz2.x=(i&1)?xyz1.x:xyz0.x;
		xyz2.y=(i&2)?xyz1.y:xyz0.y;
		xyz2.z=(i&4)?xyz1.z:xyz0.z;
		samp->lvt16[i]=BGBDT_VoxelWorld_NoisePRandC(world, xyz2, kf2);
		samp->lvf16[i]=(samp->lvt16[i]*(1.0/32767.5))-1.0;
	}
#endif

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

float BGBDT_VoxelWorld_NoiseChk4(
	BGBDT_VoxWorld *world, BGBDT_NoiseSample *samp,
	BGBDT_VoxCoord xyz, int k)
{
	BGBDT_VoxCoord xyz0, xyz1, xyz2, xyzf;
	float fv[8];
	float fx, fy, fz;
	float f, g;

	BGBDT_VoxelWorld_InitSamplerChk(world, samp, xyz, k);

	xyzf.x=xyz.x&((4<<BGBDT_XYZ_SHR_CHUNK)-1);
	xyzf.y=xyz.y&((4<<BGBDT_XYZ_SHR_CHUNK)-1);
	xyzf.z=xyz.z&((4<<BGBDT_XYZ_SHR_CHUNK)-1);
	
	fx=xyzf.x*BGBDT_XYZ_SCALE_TOCHUNK4;
	fy=xyzf.y*BGBDT_XYZ_SCALE_TOCHUNK4;
	fz=xyzf.z*BGBDT_XYZ_SCALE_TOCHUNK4;
	
	fv[0]=(samp->lvf4[0]*(1.0-fx))+(samp->lvf4[1]*fx);
	fv[1]=(samp->lvf4[2]*(1.0-fx))+(samp->lvf4[3]*fx);
	fv[2]=(samp->lvf4[4]*(1.0-fx))+(samp->lvf4[5]*fx);
	fv[3]=(samp->lvf4[6]*(1.0-fx))+(samp->lvf4[7]*fx);

	fv[4]=(fv[0]*(1.0-fy))+(fv[1]*fy);
	fv[5]=(fv[2]*(1.0-fy))+(fv[3]*fy);
	f=(fv[4]*(1.0-fz))+(fv[5]*fz);
	return(f);
}

float BGBDT_VoxelWorld_NoiseChk16(
	BGBDT_VoxWorld *world, BGBDT_NoiseSample *samp,
	BGBDT_VoxCoord xyz, int k)
{
	BGBDT_VoxCoord xyz0, xyz1, xyz2, xyzf;
	float fv[8];
	float fx, fy, fz;
	float f, g;

	BGBDT_VoxelWorld_InitSamplerChk(world, samp, xyz, k);

	xyzf.x=xyz.x&((16<<BGBDT_XYZ_SHR_CHUNK)-1);
	xyzf.y=xyz.y&((16<<BGBDT_XYZ_SHR_CHUNK)-1);
	xyzf.z=xyz.z&((16<<BGBDT_XYZ_SHR_CHUNK)-1);
	
	fx=xyzf.x*BGBDT_XYZ_SCALE_TOCHUNK16;
	fy=xyzf.y*BGBDT_XYZ_SCALE_TOCHUNK16;
	fz=xyzf.z*BGBDT_XYZ_SCALE_TOCHUNK16;
	
	fv[0]=(samp->lvf16[0]*(1.0-fx))+(samp->lvf16[1]*fx);
	fv[1]=(samp->lvf16[2]*(1.0-fx))+(samp->lvf16[3]*fx);
	fv[2]=(samp->lvf16[4]*(1.0-fx))+(samp->lvf16[5]*fx);
	fv[3]=(samp->lvf16[6]*(1.0-fx))+(samp->lvf16[7]*fx);

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
	int x, y, z, zbi, rsk, vz, vty;
	int i, j, k;

	memset(&tsamp, 0, sizeof(BGBDT_NoiseSample));

	memset(&td_air		, 0, sizeof(BGBDT_VoxData));
	memset(&td_stone	, 0, sizeof(BGBDT_VoxData));
	memset(&td_hardstone, 0, sizeof(BGBDT_VoxData));
	memset(&td_dirt		, 0, sizeof(BGBDT_VoxData));
	memset(&td_snofce	, 0, sizeof(BGBDT_VoxData));
	memset(&td_lava		, 0, sizeof(BGBDT_VoxData));

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "air");
	td_air.vtypel=vty;	td_air.vtypeh=vty>>8;
//	td_air.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world, "air");
	td_air.vattr=0;
//	td_air.vlight=0xFF;
//	td_air.alight=0xFF;
	td_air.vlight=0;
	td_air.alight=0;


	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "stone");
	td_stone.vtypel=vty;	td_stone.vtypeh=vty>>8;
//	td_stone.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world, "stone");
	td_stone.vattr=0;	td_stone.vlight=0;	td_stone.alight=0;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "hardstone");
	td_hardstone.vtypel=vty;	td_hardstone.vtypeh=vty>>8;
//	td_hardstone.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world,
//		"hardstone");
	td_hardstone.vattr=0;	td_hardstone.vlight=0;	td_hardstone.alight=0;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "dirt");
	td_dirt.vtypel=vty;	td_dirt.vtypeh=vty>>8;
//	td_dirt.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world, "dirt");
	td_dirt.vattr=0;	td_dirt.vlight=0;	td_dirt.alight=0;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "solidnoface");
	td_snofce.vtypel=vty;	td_snofce.vtypeh=vty>>8;
//	td_snofce.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world,
//		"solidnoface");
	td_snofce.vattr=0;	td_snofce.vlight=0;	td_snofce.alight=0;

	vty=BGBDT_VoxelWorld_LookupTypeIndexName(world, "lava");
	td_lava.vtypel=vty;	td_lava.vtypeh=vty>>8;
//	td_lava.vtype=BGBDT_VoxelWorld_LookupTypeIndexName(world, "lava");
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

struct {
char *name;
char *mat_side;
char *mat_top;
char *mat_bot;
int mat_side_x, mat_side_y;
int mat_top_x, mat_top_y;
int mat_bot_x, mat_bot_y;
int flags;
int glow;
}bgbdt_matinfo[]={

{"null", "textures/atlas0", NULL, NULL,
	0, 0,  0, 0,  0, 0,
	BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NOFACES, 0},

{"air", "textures/atlas0", NULL, NULL,
  0, 0,  0, 0,  0, 0,  BGBDT_VOXFL_NONSOLID|
	BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NOFACES, 0x00},

{"hardstone", "textures/atlas0", NULL, NULL,
	1, 0,  1, 0,  1, 0,  0, 0x00},
{"dirt", "textures/atlas0", NULL, NULL,
	2, 0,  2, 0,  2, 0,  0, 0x00},
{"grass", "textures/atlas0", NULL, NULL,
	3, 0,  3, 0,  3, 0,  0, 0x00},
{"stone", "textures/atlas0", NULL, NULL,
	4, 0,  4, 0,  4, 0,  0, 0x00},
{"bigbrick_brown", "textures/atlas0", NULL, NULL,
	0, 1,  0, 1,  0, 1,  0, 0x00},
{"bigbrick_gray", "textures/atlas0", NULL, NULL,
	1, 1,  1, 1,  1, 1,  0, 0x00},
{"brick_red", "textures/atlas0", NULL, NULL,
	2, 1,  2, 1,  2, 1,  0, 0x00},
{"stone_red", "textures/atlas0", NULL, NULL,
	3, 1,  3, 1,  3, 1,  0, 0x00},
{"solidnoface", "textures/atlas0", NULL, NULL,
	15, 15,  15, 15,  15, 15,  BGBDT_VOXFL_NOFACES, 0x00},
{"glass", "textures/atlas0", NULL, NULL,
	15, 0,  15, 0,  15, 0,  BGBDT_VOXFL_TRANSPARENT, 0x00},

{"lava", "textures/atlas0", NULL, NULL,
	0, 2,  0, 2,  0, 2,  BGBDT_VOXFL_FLUID_LAVA|
	BGBDT_VOXFL_NONSOLID|BGBDT_VOXFL_GLOWLIGHT,  0xF4},
{"water", "textures/atlas0", NULL, NULL,
	1, 2,  1, 2,  1, 2,  BGBDT_VOXFL_FLUID_WATER|
	BGBDT_VOXFL_NONSOLID|BGBDT_VOXFL_TRANSPARENT, 0x00},
{"slime", "textures/atlas0", NULL, NULL,
	2, 2,  2, 2,  2, 2,  BGBDT_VOXFL_FLUID_SLIME|
	BGBDT_VOXFL_NONSOLID|BGBDT_VOXFL_GLOWLIGHT|
	BGBDT_VOXFL_TRANSPARENT,  0xF2},

{"lantern_red", "textures/atlas0", NULL, NULL,
	3, 2,  3, 2,  3, 2,  BGBDT_VOXFL_GLOWLIGHT,  0xF4},
{"lantern_blue", "textures/atlas0", NULL, NULL,
	4, 2,  4, 2,  4, 2,  BGBDT_VOXFL_GLOWLIGHT,  0xF1},
{"lantern_yellow", "textures/atlas0", NULL, NULL,
	5, 2,  5, 2,  5, 2,  BGBDT_VOXFL_GLOWLIGHT,  0xF6},
{"lantern_green", "textures/atlas0", NULL, NULL,
	6, 2,  6, 2,  6, 2,  BGBDT_VOXFL_GLOWLIGHT,  0xF2},
{"lantern_cyan", "textures/atlas0", NULL, NULL,
	7, 2,  7, 2,  7, 2,  BGBDT_VOXFL_GLOWLIGHT,  0xF3},
{"lantern_violet", "textures/atlas0", NULL, NULL,
	8, 2,  8, 2,  8, 2,  BGBDT_VOXFL_GLOWLIGHT,  0xF5},
{"lantern_white", "textures/atlas0", NULL, NULL,
	9, 2,  9, 2,  9, 2,  BGBDT_VOXFL_GLOWLIGHT,  0xFF},
{"grate", "textures/atlas0", NULL, NULL,
	15, 1,  15, 1,  15, 1,  BGBDT_VOXFL_TRANSPARENT, 0x00},
{"bars", "textures/atlas0", NULL, NULL,
	14, 0,  14, 0,  14, 0,  BGBDT_VOXFL_TRANSPARENT, 0x00},

{"stone2", "textures/atlas0", NULL, NULL,
	5, 0,  5, 0,  5, 0,  0, 0x00},
{"stone3", "textures/atlas0", NULL, NULL,
	4, 1,  4, 1,  4, 1,  0, 0x00},
{"bigbrick_stone", "textures/atlas0", NULL, NULL,
	5, 1,  5, 1,  5, 1,  0, 0x00},

{"tridirt0", "textures/atlas0", NULL, NULL,
	6, 1,  6, 1,  6, 1,  0, 0x00},
{"tridirt1", "textures/atlas0", NULL, NULL,
	7, 0,  7, 0,  7, 0,  0, 0x00},
{"tridirt2", "textures/atlas0", NULL, NULL,
	6, 0,  6, 0,  6, 0,  0, 0x00},
{"tridirt3", "textures/atlas0", NULL, NULL,
	7, 1,  7, 1,  7, 1,  0, 0x00},

{"ent_mob", "textures/atlas0", NULL, NULL,
	14, 14,  14, 14,  14, 14,
	BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NONSOLID, 0},
{"flip_up", "textures/atlas0", NULL, NULL,
	15, 14,  15, 14,  15, 14,
	BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NONSOLID|BGBDT_VOXFL_PHYS_FLIPUP, 0},
{"flip_dn", "textures/atlas0", NULL, NULL,
	14, 15,  14, 15,  14, 15,
	BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NONSOLID|BGBDT_VOXFL_PHYS_FLIPDN, 0},

//{"sky1", "textures/atlas0", NULL, NULL,
//	0, 14,  0, 14,  0, 14,  BGBDT_VOXFL_GLOWLIGHT, 0xF900},
//{"sky2", "textures/atlas0", NULL, NULL,
//	1, 14,  1, 14,  1, 14,  BGBDT_VOXFL_GLOWLIGHT, 0xFA00},
//{"sky3", "textures/atlas0", NULL, NULL,
//	1, 15,  1, 15,  1, 15,  BGBDT_VOXFL_GLOWLIGHT, 0xFB00},

{"sky1", "textures/sky1", NULL, NULL,
	0, 0,  0, 0,  0, 0,  BGBDT_VOXFL_GLOWLIGHT, 0xF900},
{"sky2", "textures/sky2", NULL, NULL,
	0, 0,  0, 0,  0, 0,  BGBDT_VOXFL_GLOWLIGHT, 0xFA00},
{"sky3", "textures/sky3", NULL, NULL,
	0, 0,  0, 0,  0, 0,  BGBDT_VOXFL_GLOWLIGHT, 0xFB00},

{"fence1", "textures/atlas0", NULL, NULL,
	14, 1,  14, 1,  14, 1,  BGBDT_VOXFL_TRANSPARENT, 0x00},
{"fence2", "textures/atlas0", NULL, NULL,
	14, 2,  14, 2,  14, 2,  BGBDT_VOXFL_TRANSPARENT, 0x00},
{"fence3", "textures/atlas0", NULL, NULL,
	15, 2,  15, 2,  15, 2,  BGBDT_VOXFL_TRANSPARENT, 0x00},

{"bricks", "textures/atlas0", NULL, NULL,
	8, 1,  8, 1,  8, 1,  0, 0},
{"log", "textures/atlas0", NULL, NULL,
	8, 0,  8, 0,  8, 0,  0, 0},
{"leaves", "textures/atlas0", NULL, NULL,
	9, 0,  9, 0,  9, 0,  BGBDT_VOXFL_TRANSPARENT, 0},
{"fruitleaves", "textures/atlas0", NULL, NULL,
	9, 1,  9, 1,  9, 1,  BGBDT_VOXFL_TRANSPARENT, 0},
{"sandstone", "textures/atlas0", NULL, NULL,
	10, 0,  10, 0,  10, 0,  0, 0},
{"sand", "textures/atlas0", NULL, NULL,
	10, 1,  10, 1,  10, 1,  0, 0},
{"snow", "textures/atlas0", NULL, NULL,
	10, 2,  10, 2,  10, 2,  0, 0},
{"cobble", "textures/atlas0", NULL, NULL,
	11, 0,  11, 0,  11, 0,  0, 0},
{"greencobble", "textures/atlas0", NULL, NULL,
	11, 1,  11, 1,  11, 1,  0, 0},

{"grassclump", "textures/atlas0", NULL, NULL,
	14, 3,  14, 3,  14, 3, 
	BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NONSOLID|
	BGBDT_VOXFL_CROSSSPR, 0},

{"somatred", "textures/atlas0", NULL, NULL,
	3, 3,  3, 3,  3, 3,  0, 0},
{"somatblu", "textures/atlas0", NULL, NULL,
	4, 3,  4, 3,  4, 3,  0, 0},
{"somatyel", "textures/atlas0", NULL, NULL,
	5, 3,  5, 3,  5, 3,  0, 0},
{"somatgrn", "textures/atlas0", NULL, NULL,
	6, 3,  6, 3,  6, 3,  0, 0},
{"somatcyn", "textures/atlas0", NULL, NULL,
	7, 3,  7, 3,  7, 3,  0, 0},
{"somatpur", "textures/atlas0", NULL, NULL,
	8, 3,  8, 3,  8, 3,  0, 0},
{"somatwht", "textures/atlas0", NULL, NULL,
	9, 3,  9, 3,  9, 3,  0, 0},

{"stairs", "textures/atlas0", NULL, NULL,
	11, 0,  11, 0,  11, 0,
	BGBDT_VOXFL_SOLIDNSBOX|BGBDT_VOXFL_TRANSPARENT, 0},

{"gungerock", "textures/atlas0", NULL, NULL,
	11, 2,  11, 2,  11, 2,  0, 0},

{"stoneslab", "textures/atlas0", NULL, NULL,
	10, 3,  10, 3,  10, 3,  0, 0},
{"woodslab", "textures/atlas0", NULL, NULL,
	11, 3,  11, 3,  11, 3,  0, 0},

{"trigger", "textures/atlas0", NULL, NULL,
	13, 14,  13, 14,  13, 14,
	BGBDT_VOXFL_TRANSPARENT|BGBDT_VOXFL_NONSOLID|
	BGBDT_VOXFL_PHYS_TRIG, 0},

{"planks", "textures/atlas0", NULL, NULL,
	12, 1,  12, 1,  12, 1,  0, 0},
{"planks2", "textures/atlas0", NULL, NULL,
	12, 2,  12, 2,  12, 2,  0, 0},
{"somatblk", "textures/atlas0", NULL, NULL,
	2, 3,  2, 3,  2, 3,  0, 0},
{"brick_blk", "textures/atlas0", NULL, NULL,
	2, 4,  2, 4,  2, 4,  0, 0x00},

{"cloud", "textures/atlas0", NULL, NULL,
	0, 3,  0, 3,  0, 3,  BGBDT_VOXFL_TRANSPARENT, 0},

{NULL, NULL}
};

int BGBDT_CheckRgnGenChunkBasicP(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, int bx, int by, int bz)
{
	if(rgn->bz)
		return(0);
	if(bz<0)
		return(0);

	if(bz>=8)
		return(0);
	return(1);
}

int BGBDT_CheckRgnGenChunkBasic256P(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, int bx, int by, int bz)
{
	if(rgn->bz)
		return(0);
	if(bz<0)
		return(0);

	if(bz>=16)
		return(0);
	return(1);
}

BTEIFGL_API BGBDT_VoxWorld *BGBDT_CreateBasicWorld(char *name)
{
	return(BGBDT_CreateBasicWorld2(name, "default"));
}

BTEIFGL_API BGBDT_VoxWorld *BGBDT_CreateBasicWorld2(char *name, char *wrlty)
{
	BGBDT_VoxTypeInfo *tyi;
	BGBDT_VoxWorld *wrl;
	char *s;
	int i0, i1, i2, i3;
	int i, j, k, h;
	
	wrl=BGBDT_AllocVoxelWorld();

	wrl->worldname=frgl_strdup(name);
	wrl->worldtype=frgl_strdup(wrlty);

	BGBDT_VoxelWorld_LoadWorldInfo(wrl);

	BGBDT_SetupBasicWorld2(wrl, name, wrlty);

	BGBDT_VoxelWorld_SaveWorldInfo(wrl);

	return(wrl);
}

BTEIFGL_API BGBDT_VoxWorld *BGBDT_SetupBasicWorld2(
	BGBDT_VoxWorld *wrl, char *name, char *wrlty)
{
	BGBDT_VoxTypeInfo *tyi;
	char *s;
	int i0, i1, i2, i3;
	int i, j, k, h;
	
	wrl->GenerateChunk=BGBDT_VoxelWorld_GenerateChunkBasic;
	wrl->CheckRgnGenChunkP=BGBDT_CheckRgnGenChunkBasicP;
	if(!strcmp(wrlty, "mare"))
	{
		wrl->GenerateChunk=BGBDT_VoxTgMare_GenerateChunkBasic;
//		wrl->CheckRgnGenChunkP=BGBDT_CheckRgnGenChunkBasicP;
		wrl->CheckRgnGenChunkP=BGBDT_CheckRgnGenChunkBasic256P;
	}

	for(i=0; bgbdt_matinfo[i].name; i++)
	{
		tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, bgbdt_matinfo[i].name);
		tyi->mat_side=FRGL_TexMat_GetLoadIndex(bgbdt_matinfo[i].mat_side);
		if(!bgbdt_matinfo[i].mat_top)
			{ tyi->mat_top=tyi->mat_side; }
		if(!bgbdt_matinfo[i].mat_bot)
			{ tyi->mat_bot=tyi->mat_side; }

		tyi->mat_side_x=bgbdt_matinfo[i].mat_side_x;
		tyi->mat_side_y=bgbdt_matinfo[i].mat_side_y;
		tyi->mat_top_x=bgbdt_matinfo[i].mat_top_x;
		tyi->mat_top_y=bgbdt_matinfo[i].mat_top_y;
		tyi->mat_bot_x=bgbdt_matinfo[i].mat_bot_x;
		tyi->mat_bot_y=bgbdt_matinfo[i].mat_bot_y;
		tyi->flags=bgbdt_matinfo[i].flags;
		tyi->glow=bgbdt_matinfo[i].glow;

		tyi->mat_ico=tyi->mat_side;
		tyi->mat_ico_x=bgbdt_matinfo[i].mat_side_x;
		tyi->mat_ico_y=bgbdt_matinfo[i].mat_side_y;
		
		tyi->EmitBlockFaces=BGBDT_VoxMesh_EmitBlockFacesGeneric;

		if(tyi->flags&BGBDT_VOXFL_SOLIDNSBOX)
			tyi->EmitBlockFaces=BGBDT_VoxMesh_EmitBlockFacesNSBox;
	}

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "grass");
	if(tyi)tyi->RandomTick=BGBDT_VoxelWorld_RandomTick_Grass;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "grassclump");
	if(tyi)tyi->RandomTick=BGBDT_VoxelWorld_RandomTick_TallGrass;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "stairs");
	if(tyi)
		{ tyi->mat_ico_x=12; tyi->mat_ico_y=0; }

#if 0
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
	tyi->mat_side_x=15; tyi->mat_side_y=15;
	tyi->mat_top_x=15; tyi->mat_top_y=15;
	tyi->mat_bot_x=15; tyi->mat_bot_y=15;
	tyi->flags=BGBDT_VOXFL_NOFACES;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "glass");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=15; tyi->mat_side_y=0;
	tyi->mat_top_x=15; tyi->mat_top_y=0;
	tyi->mat_bot_x=15; tyi->mat_bot_y=0;
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

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "grate");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=15; tyi->mat_side_y=1;
	tyi->mat_top_x=15; tyi->mat_top_y=1;
	tyi->mat_bot_x=15; tyi->mat_bot_y=1;
	tyi->flags=BGBDT_VOXFL_TRANSPARENT;

	tyi=BGBDT_VoxelWorld_GetTypeInfoName(wrl, "bars");
	tyi->mat_side=FRGL_TexMat_GetLoadIndex("textures/atlas0");
	tyi->mat_top=tyi->mat_side;	tyi->mat_bot=tyi->mat_side;
	tyi->mat_side_x=14; tyi->mat_side_y=0;
	tyi->mat_top_x=14; tyi->mat_top_y=0;
	tyi->mat_bot_x=14; tyi->mat_bot_y=0;
	tyi->flags=BGBDT_VOXFL_TRANSPARENT;
#endif

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

int BGBDT_VoxelWorld_SaveWorldInfo(
	BGBDT_VoxWorld *world)
{
	char tb[256];
	BGBDT_VoxTypeInfo *tyi;
	BGBDT_VoxCoord xyz, org;
	VFILE *fd;
	int fl;
	int i, j, k;

	sprintf(tb, "region/%s/world.ini", world->worldname);

	fd=vffopen(tb, "wt");
	if(!fd)return(-1);
	
	if(world->worldname)
	{
		sprintf(tb, "worldname %s\n",
			world->worldname);
		vfwrite(tb, 1, strlen(tb), fd);
	}

	if(world->worldtype)
	{
		sprintf(tb, "worldtype %s\n",
			world->worldtype);
		vfwrite(tb, 1, strlen(tb), fd);
	}
	
	for(i=0; i<4096; i++)
	{
		tyi=world->voxtypes[i];
		if(!tyi)
			break;

		sprintf(tb, "voxty %d %s\n",
			i, tyi->name);
		vfwrite(tb, 1, strlen(tb), fd);
	}

	vfclose(fd);
	return(0);
}

int BGBDT_VoxelWorld_LoadWorldInfo(
	BGBDT_VoxWorld *world)
{
	char tb[256];
	BGBDT_VoxTypeInfo *tyi;
	char *buf, *cs, *cse;
	char *s;
	char **a;
	int sz, b;
	int i, j, k;

	sprintf(tb, "region/%s/world.ini", world->worldname);
	
	buf=vf_loadfile(tb, &sz);
	if(!buf)
		return(0);
	
	cs=buf; cse=buf+sz;
	while(cs<cse)
	{
		s=frgl_rgets(&cs);
		a=frgl_rsplit(s);
		
		if(!a[0])continue;

		if(!strcmp(a[0], "worldtype"))
		{
			world->worldtype=frgl_strdup(a[1]);
			continue;
		}

		if(!strcmp(a[0], "voxty"))
		{
			i=atoi(a[1]);
			tyi=BGBDT_VoxelWorld_GetTypeInfoIndex(world, i);
			if(!tyi)
				continue;
			tyi->name=frgl_strdup(a[2]);
			continue;
		}
	}

	frgl_free(buf);
	return(0);
}
