#include <bteifgl.h>

void BGBDT_CalcCoordLocalOrigin(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, float *vec)
{
	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER);
}

float BGBDT_CalcCameraChunkMeshDistance(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh)
{
	BGBDT_VoxCoord xyz;
	float dx, dy, dz, d;

	xyz=BGBDT_WorldGetRegionChunkCoord(world, mesh->rgn,
		mesh->bx, mesh->by, mesh->bz);
	xyz.x+=8<<BGBDT_XYZ_SHR_VOXEL;
	xyz.y+=8<<BGBDT_XYZ_SHR_VOXEL;
	xyz.z+=8<<BGBDT_XYZ_SHR_VOXEL;
	
	dx=((xyz.x*BGBDT_XYZ_SCALE_TOMETER)-(world->camorg[0]));
	dy=((xyz.y*BGBDT_XYZ_SCALE_TOMETER)-(world->camorg[1]));
	dz=((xyz.z*BGBDT_XYZ_SCALE_TOMETER)-(world->camorg[2]));
	
	d=sqrt(dx*dx+dy*dy+dz*dz)-14;
	return(d);
}

void BGBDT_CalcChunkMeshLocalCenter(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh, float *vec)
{
	BGBDT_VoxCoord xyz;

	xyz=BGBDT_WorldGetRegionChunkCoord(world, mesh->rgn,
		mesh->bx, mesh->by, mesh->bz);
	xyz.x+=8<<BGBDT_XYZ_SHR_VOXEL;
	xyz.y+=8<<BGBDT_XYZ_SHR_VOXEL;
	xyz.z+=8<<BGBDT_XYZ_SHR_VOXEL;

	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER);
	vec[3]=14;
}

void BGBDT_CalcChunkMeshViewCenter(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh, float *vec)
{
	float vmorg[4];
	float dx, dy, dz, d;
	
	BGBDT_CalcChunkMeshLocalCenter(world, mesh, vmorg);
	V3F_SUB(vmorg, world->camorg, vmorg);
	
	vec[0]=V3F_DOT(vmorg, world->camrot+0);
	vec[1]=V3F_DOT(vmorg, world->camrot+3);
	vec[2]=V3F_DOT(vmorg, world->camrot+6);
	vec[3]=vmorg[3];
}

void BGBDT_CalcChunkLocalCenter(BGBDT_VoxWorld *world,
	BGBDT_VoxChunk *chk, float *vec)
{
	BGBDT_VoxCoord xyz;

	xyz=BGBDT_WorldGetRegionChunkCoord(world, chk->rgn,
		chk->bx, chk->by, chk->bz);
	xyz.x+=8<<BGBDT_XYZ_SHR_VOXEL;
	xyz.y+=8<<BGBDT_XYZ_SHR_VOXEL;
	xyz.z+=8<<BGBDT_XYZ_SHR_VOXEL;

	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER);
	vec[3]=14;
}

void BGBDT_CalcRegionBlockLocalCenter(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, int bx, int by, int bz, float *vec)
{
	BGBDT_VoxCoord xyz;

	xyz=BGBDT_WorldGetRegionVoxelCoord(world, rgn,
		bx, by, bz);
	xyz.x+=8<<BGBDT_XYZ_SHR_VOXEL;
	xyz.y+=8<<BGBDT_XYZ_SHR_VOXEL;
	xyz.z+=8<<BGBDT_XYZ_SHR_VOXEL;

	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER);
	vec[3]=14;
}

BTEIFGL_API BGBDT_VoxCoord BGBDT_ConvLocalToVoxCoord(
	BGBDT_VoxWorld *world, float *vec)
{
	BGBDT_VoxCoord xyz;
	
	xyz.x=vec[0]/BGBDT_XYZ_SCALE_TOMETER+0.5;
	xyz.y=vec[1]/BGBDT_XYZ_SCALE_TOMETER+0.5;
	xyz.z=vec[2]/BGBDT_XYZ_SCALE_TOMETER+0.5;
	
	return(xyz);
}

BTEIFGL_API void BGBDT_ConvVoxToLocalCoord(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord xyz, float *vec)
{
	vec[0]=xyz.x*BGBDT_XYZ_SCALE_TOMETER;
	vec[1]=xyz.y*BGBDT_XYZ_SCALE_TOMETER;
	vec[2]=xyz.z*BGBDT_XYZ_SCALE_TOMETER;
}

void BGBDT_CalcChunkMeshFrustumCenter(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh, float *vec)
{
	float vmorg[4];
	BGBDT_CalcChunkMeshViewCenter(world, mesh, vmorg);
	
	if((vmorg[1]*vmorg[1])>1)
	{
		vec[0]=vmorg[0]/vmorg[1];
		vec[1]=vmorg[2]/vmorg[1];
		vec[2]=vmorg[1];
		vec[3]=fabs(vmorg[3]/vmorg[1]);
		vec[4]=vmorg[0];	vec[5]=vmorg[1];
		vec[6]=vmorg[2];	vec[7]=vmorg[3];
	}else
	{
		vec[0]=vmorg[0];
		vec[1]=vmorg[2];
		vec[2]=vmorg[1];
		vec[3]=vmorg[3];
		vec[4]=vmorg[0];	vec[5]=vmorg[1];
		vec[6]=vmorg[2];	vec[7]=vmorg[3];
	}
}

void BGBDT_DrawVoxChunkMesh(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh)
{
	int i, j, k;
	
	for(i=0; i<mesh->va_nmesh; i++)
	{
		FRGL_TexMat_BindBasic(mesh->va_mesh[i*4+2]);

#if 1
		FRGL_DrawPrim_DrawArraysNormalTexRGB(
			GL_TRIANGLES, mesh->va_mesh[i*4+0], mesh->va_mesh[i*4+1],
			3, GL_SHORT, 3*2, mesh->va_xyz,
			2, GL_HALF_FLOAT, 2*2, mesh->va_st,
			3, GL_BYTE, 3, mesh->va_norm,
			4, GL_UNSIGNED_BYTE, 4, mesh->va_rgba);
#endif

#if 0
		FRGL_DrawPrim_DrawElementsNormalTexRGB(
			GL_TRIANGLES, mesh->va_mesh[i*4+0],
				mesh->va_mesh[i*4+1], mesh->va_elem,
			3, GL_SHORT, 3*2, mesh->va_xyz,
			2, GL_FLOAT, 2*4, mesh->va_st,
			3, GL_BYTE, 3, mesh->va_norm,
			4, GL_UNSIGNED_BYTE, 4, mesh->va_rgba);
#endif

#if 0
		FRGL_DrawPrim_DrawElementsNormalTexRGB(
			GL_TRIANGLES, mesh->va_mesh[i*4+0],
				mesh->va_mesh[i*4+1], mesh->va_elem,
			3, GL_FLOAT, 3*4, mesh->va_xyz,
			2, GL_FLOAT, 2*4, mesh->va_st,
			3, GL_BYTE, 3, mesh->va_norm,
			4, GL_UNSIGNED_BYTE, 4, mesh->va_rgba);
#endif
	}
}

static byte pxtab[16][16]={
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15},
{ 1, 0, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15},
{ 2, 1, 3, 0, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15},
{ 3, 2, 4, 1, 5, 0, 6, 7, 8, 9,10,11,12,13,14,15},
{ 4, 3, 5, 2, 6, 1, 7, 0, 8, 9,10,11,12,13,14,15},
{ 5, 4, 6, 3, 7, 2, 8, 1, 9, 0,10,11,12,13,14,15},
{ 6, 5, 7, 4, 8, 3, 9, 2,10, 1,11, 0,12,13,14,15},
{ 7, 6, 8, 5, 9, 4,10, 3,11, 2,12, 1,13, 0,14,15},
{ 8, 7, 9, 6,10, 5,11, 4,12, 3,13, 2,14, 1,15, 0},
{ 9, 8,10, 7,11, 6,12, 5,13, 4,14, 3,15, 2, 1, 0},
{10, 9,11, 8,12, 7,13, 6,14, 5,15, 4, 3, 2, 1, 0},
{11,10,12, 9,13, 8,14, 7,15, 6, 5, 4, 3, 2, 1, 0},
{12,11,13,10,14, 9,15, 8, 7, 6, 5, 4, 3, 2, 1, 0},
{13,12,14,11,15,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
{14,13,15,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
{15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}
};

void BGBDT_TickVoxRegion(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	float lorg[4];
	BGBDT_VoxCoord cvorg;
	BGBDT_VoxChunkMesh *mesh;
	BGBDT_VoxChunk *chk;
	byte *pxt, *pyt, *pzt;
	int t0, t1, t2;
	int bx, by, bz;
	int cx, cy, cz;
	int cvx, cvy, cvz;
	int i;

	BGBDT_CalcRegionBlockLocalCenter(world,
		rgn, 128, 128, 128, lorg);
	if(V3F_DIST(world->camorg, lorg)>256)
		return;

	cvorg=BGBDT_ConvLocalToVoxCoord(world, world->camorg);
	cvx=(cvorg.x-(rgn->bx<<BGBDT_XYZ_SHR_REGION_XY))>>BGBDT_XYZ_SHR_CHUNK;
	cvy=(cvorg.y-(rgn->by<<BGBDT_XYZ_SHR_REGION_XY))>>BGBDT_XYZ_SHR_CHUNK;
	cvz=(cvorg.z-(rgn->bz<<BGBDT_XYZ_SHR_REGION_Z ))>>BGBDT_XYZ_SHR_CHUNK;
	if(cvx<0)cvx=0;
	if(cvy<0)cvy=0;
	if(cvz<0)cvz=0;
	if(cvx>15)cvx=15;
	if(cvy>15)cvy=15;
	if(cvz>15)cvz=15;

	pxt=pxtab[cvx];
	pyt=pxtab[cvy];
	pzt=pxtab[cvz];

	if(rgn->flags&BGBDT_RGNFL_NEWRGN)
	{
		for(bz=0; bz<8; bz++)
			for(by=0; by<16; by++)
				for(bx=0; bx<16; bx++)
		{
			chk=BGBDT_GetRegionChunk(world, rgn,
				bx, by, bz, BGBDT_ACCFL_ENNEWCHK);
		}
		rgn->flags&=~BGBDT_RGNFL_NEWRGN;
		return;
	}

#if 1

#if 0
	bx=rand()&15;	by=rand()&15;	bz=rand()&7;
	chk=BGBDT_GetRegionChunk(world, rgn, bx, by, bz, BGBDT_ACCFL_NOLOAD);
	BGBDT_VoxLight_UpdateChunkLight(world, chk);
#endif

//	t0=clock();
	t0=world->tickstart;

//	for(bz=0; bz<8; bz++)
//		for(by=0; by<16; by++)
//			for(bx=0; bx<16; bx++)
	for(bz=rgn->tbz; bz<8; bz++)
	{
		for(by=rgn->tby; by<16; by++)
		{
			for(bx=rgn->tbx; bx<16; bx++)
			{
				t1=clock();
				t2=t1-t0;
				if(t2>10)
		//		if(t2>3)
				{
					rgn->tbx=bx;
					rgn->tby=by;
					rgn->tbz=bz;
					return;
				}

				cx=pxt[bx];
				cy=pyt[by];
				cz=pzt[bz];

		//		BGBDT_CalcChunkLocalCenter(world, mcur, lorg);
				BGBDT_CalcRegionBlockLocalCenter(world,
		//			rgn, bx*16+8, by*16+8, bz*16+8, lorg);
					rgn, cx*16+8, cy*16+8, cz*16+8, lorg);
					
				if(V3F_DIST(world->camorg, lorg)>256)
//					continue;
					break;
			
				chk=BGBDT_GetRegionChunk(world, rgn,
		//			bx, by, bz, 0);
					cx, cy, cz, 0);
				if(chk && (chk->flags&BGBDT_CHKFL_LIGHTDIRTY))
				{
					BGBDT_VoxLight_UpdateChunkLight(world, chk);
					rgn->flags|=BGBDT_RGNFL_RGNDIRTY;
				}
			}
			rgn->tbx=0;
		}
		rgn->tby=0;
	}
	rgn->tbz=0;
	
	if(rgn->flags&BGBDT_RGNFL_RGNDIRTY)
	{
		BGBDT_WorldSaveRegionData(world, rgn);
		rgn->flags&=~BGBDT_RGNFL_RGNDIRTY;
	}
#endif
}

void BGBDT_UpdateVoxRegionPVS(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	BGBDT_VoxChunkMesh *mesh;
	BGBDT_VoxChunkMesh *pvs;
	BGBDT_VoxCoord cvorg;
	float lvorg[8];
	byte *pxt, *pyt, *pzt;
	float d;
	int t0, t1, t2;
	int bx, by, bz;
	int cx, cy, cz;
	int cvx, cvy, cvz;
	int i, j, k, accfl;

	t0=clock();
	if(t0<(rgn->lastpvs+500))
		return;
//	rgn->lastpvs=t0;

	cvorg=BGBDT_ConvLocalToVoxCoord(world, world->camorg);
	cvx=(cvorg.x-(rgn->bx<<BGBDT_XYZ_SHR_REGION_XY))>>BGBDT_XYZ_SHR_CHUNK;
	cvy=(cvorg.y-(rgn->by<<BGBDT_XYZ_SHR_REGION_XY))>>BGBDT_XYZ_SHR_CHUNK;
	cvz=(cvorg.z-(rgn->bz<<BGBDT_XYZ_SHR_REGION_Z ))>>BGBDT_XYZ_SHR_CHUNK;
	if(cvx<0)cvx=0;
	if(cvy<0)cvy=0;
	if(cvz<0)cvz=0;
	if(cvx>15)cvx=15;
	if(cvy>15)cvy=15;
	if(cvz>15)cvz=15;

	pxt=pxtab[cvx];
	pyt=pxtab[cvy];
	pzt=pxtab[cvz];

	t0=world->tickstart;
	accfl=0;

	pvs=NULL;
	for(bz=0; bz<16; bz++)
		for(by=0; by<16; by++)
			for(bx=0; bx<16; bx++)
	{
		cx=pxt[bx];
		cy=pyt[by];
		cz=pzt[bz];
	
		BGBDT_CalcRegionBlockLocalCenter(world,
//			rgn, bx*16+8, by*16+8, bz*16+8, lvorg);
			rgn, cx*16+8, cy*16+8, cz*16+8, lvorg);
		if(V3F_DIST(world->camorg, lvorg)>192)
			continue;

		t1=clock();
		t2=t1-t0;
//		if(t2>10)
		if(t2>15)
//		if(t2>20)
//		if(t2>30)
//		if(t2>45)
		{
			accfl=BGBDT_ACCFL_NOLOAD;
//			rgn->tbx=bx;
//			rgn->tby=by;
//			rgn->tbz=bz;
//			return;
		}

		mesh=BGBDT_GetRegionChunkMesh(world, rgn,
//			bx, by, bz, BGBDT_ACCFL_ENNEWCHK);
//			bx, by, bz, accfl);
			cx, cy, cz, accfl);
		if(!mesh)continue;
		if(!mesh->nmat)
			continue;
		
		d=BGBDT_CalcCameraChunkMeshDistance(world, mesh);
		if(d>128)continue;

#if 0
		if(d>0)
		{
			BGBDT_CalcChunkMeshFrustumCenter(world, mesh, lvorg);
			if(lvorg[2]<0)
				continue;
			if((fabs(lvorg[0])-lvorg[3])>2)
				continue;
			if((fabs(lvorg[1])-lvorg[3])>2)
				continue;
		}
#endif
		
		mesh->pvsnext=pvs;
		pvs=mesh;
//		BGBDT_DrawVoxChunkMesh(world, mesh);
	}

	if(!(accfl&BGBDT_ACCFL_NOLOAD))
		rgn->lastpvs=t0;
	
	rgn->pvs=pvs;
}

void BGBDT_UpdateVoxRegionCVS(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	BGBDT_VoxChunkMesh *mesh, *mcur;
	BGBDT_VoxChunkMesh *cvs;
	int t0, t1, t2;
	float lvorg[8];
	float d;

//	BGBDT_UpdateVoxRegionPVS(world, rgn);

	t0=clock();
	if(t0<(rgn->lastcvs+25))
		return;
	rgn->lastcvs=t0;

	cvs=NULL;
	mcur=rgn->pvs;
	while(mcur)
	{
		BGBDT_CalcChunkMeshFrustumCenter(world, mcur, lvorg);
		if((lvorg[2]+lvorg[7])<0)
			{ mcur=mcur->pvsnext; continue; }
		if((fabs(lvorg[0])-lvorg[3])>2)
			{ mcur=mcur->pvsnext; continue; }
		if((fabs(lvorg[1])-lvorg[3])>2)
			{ mcur=mcur->pvsnext; continue; }

		mcur->cvsnext=cvs;
		cvs=mcur;
		mcur=mcur->pvsnext;
	}

	rgn->cvs=cvs;
	
	BGBDT_UpdateVoxRegionEntityCVS(world, rgn);
}

void BGBDT_DrawVoxRegion(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	float lorg[4];
	BGBDT_VoxChunkMesh *mesh;
	BGBDT_VoxChunk *chk;
	double f, g;
	int bx, by, bz;
	int i;

//	corg[0]=rgn->bx*256+128;
//	corg[1]=rgn->by*256+128;
//	corg[2]=rgn->bz*256+128;

//	f=V3F_LEN(world->camorg, corg);
//	if(f>=256)
//		return;

	BGBDT_CalcRegionBlockLocalCenter(world,
		rgn, 128, 128, 128, lorg);
	if(V3F_DIST(world->camorg, lorg)>256)
		return;

//	BGBDT_TickVoxRegion(world, rgn);
	BGBDT_UpdateVoxRegionCVS(world, rgn);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glTranslatef(rgn->bx*256, rgn->by*256, rgn->bz*256);
	
//	glScalef(1.0/128, 1.0/128, 1.0/128);

//	glScalef(
//		(1.0/128)*(32768.0/32767.0),
//		(1.0/128)*(32768.0/32767.0),
//		(1.0/128)*(32768.0/32767.0));
	glScalef(1.0/127.996, 1.0/127.996, 1.0/127.996);

#if 1
	mesh=rgn->cvs;
	while(mesh)
	{
		BGBDT_DrawVoxChunkMesh(world, mesh);
		mesh=mesh->cvsnext;
	}
#endif
	
#if 0
//	for(i=0; i<4096; i++)
	for(bz=0; bz<16; bz++)
		for(by=0; by<16; by++)
			for(bx=0; bx<16; bx++)
	{
		mesh=BGBDT_GetRegionChunkMesh(world, rgn,
//			bx, by, bz, BGBDT_ACCFL_ENNEWCHK);
			bx, by, bz, 0);
		if(!mesh)continue;
		BGBDT_DrawVoxChunkMesh(world, mesh);
	}
#endif	

	glPopMatrix();
}

BTEIFGL_API void BGBDT_DrawVoxWorld(BGBDT_VoxWorld *world)
{
	float lorg[4], lorg2[4];
	BGBDT_VoxRegion *rcur, *rnxt, *rfst, *rcur2, *rprv2;
	double d, d2;

	world->tickstart=clock();

#if 1
	rcur=world->region; rfst=NULL;
	while(rcur)
	{
		rnxt=rcur->next;
		BGBDT_CalcRegionBlockLocalCenter(world,
			rcur, 128, 128, 128, lorg);
		d=V3F_DIST(world->camorg, lorg);
		
		rcur2=rfst; rprv2=NULL;
		while(rcur2)
		{
			BGBDT_CalcRegionBlockLocalCenter(world,
				rcur2, 128, 128, 128, lorg2);
			d2=V3F_DIST(world->camorg, lorg2);
			if(d<d2)
				break;
			rprv2=rcur2;
			rcur2=rcur2->next;
		}
		
		if(rprv2)
		{
			rcur->next=rcur2;
			rprv2->next=rcur;
		}else
		{
			rcur->next=rfst;
			rfst=rcur;
		}
		
		rcur=rnxt;
	}
	world->region=rfst;
#endif

	rcur=world->region;
	while(rcur)
	{
		BGBDT_DrawVoxRegion(world, rcur);
		rcur=rcur->next;
	}

#if 1
	rcur=world->region;
	while(rcur)
	{
		BGBDT_DrawVoxRegionEntities(world, rcur);
		rcur=rcur->next;
	}
#endif

	world->dt_draw=clock()-world->tickstart;
}


BTEIFGL_API void BGBDT_TickVoxWorld(BGBDT_VoxWorld *world)
{
	BGBDT_VoxRegion *rcur;
	
	world->tickstart=clock();
	rcur=world->region;
	while(rcur)
	{
		BGBDT_TickVoxRegion(world, rcur);
		rcur=rcur->next;
	}
	world->dt_tick=clock()-world->tickstart;

	world->tickstart=clock();
	rcur=world->region;
	while(rcur)
	{
		BGBDT_UpdateVoxRegionPVS(world, rcur);
		rcur=rcur->next;
	}
	world->dt_pvs=clock()-world->tickstart;

//	BGBDT_UpdateVoxRegionPVS(world, rgn);
}
