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

void BGBDT_TickVoxRegion(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	BGBDT_VoxChunkMesh *mesh;
	BGBDT_VoxChunk *chk;
	int t0, t1, t2;
	int bx, by, bz;
	int i;

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

	t0=clock();

//	for(bz=0; bz<8; bz++)
//		for(by=0; by<16; by++)
//			for(bx=0; bx<16; bx++)
	for(bz=rgn->tbz; bz<8; bz++)
		for(by=rgn->tby; by<16; by++)
			for(bx=rgn->tbx; bx<16; bx++)
	{
		t1=clock();
		t2=t1-t0;
		if(t2>10)
		{
			rgn->tbx=bx;
			rgn->tby=by;
			rgn->tbz=bz;
			return;
		}
	
//		chk=BGBDT_GetRegionChunk(world, rgn,
//			bx, by, bz, BGBDT_ACCFL_ENNEWCHK);
		chk=BGBDT_GetRegionChunk(world, rgn,
			bx, by, bz, 0);
		if(chk && (chk->flags&BGBDT_CHKFL_LIGHTDIRTY))
		{
			BGBDT_VoxLight_UpdateChunkLight(world, chk);
		}
//		if(!mesh)continue;
//		BGBDT_DrawVoxChunkMesh(world, mesh);
	}
	rgn->tbx=0;
	rgn->tby=0;
	rgn->tbz=0;
#endif
}

void BGBDT_UpdateVoxRegionPVS(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	BGBDT_VoxChunkMesh *mesh;
	BGBDT_VoxChunkMesh *pvs;
	float lvorg[8];
	float d;
	int t0, t1, t2;
	int bx, by, bz;
	int i;

	t0=clock();
	if(t0<(rgn->lastpvs+500))
		return;
	rgn->lastpvs=t0;

	pvs=NULL;
	for(bz=0; bz<16; bz++)
		for(by=0; by<16; by++)
			for(bx=0; bx<16; bx++)
	{
		mesh=BGBDT_GetRegionChunkMesh(world, rgn,
//			bx, by, bz, BGBDT_ACCFL_ENNEWCHK);
			bx, by, bz, 0);
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

	BGBDT_UpdateVoxRegionPVS(world, rgn);

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
	BGBDT_VoxChunkMesh *mesh;
	BGBDT_VoxChunk *chk;
	int bx, by, bz;
	int i;

	BGBDT_TickVoxRegion(world, rgn);
	BGBDT_UpdateVoxRegionCVS(world, rgn);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glTranslatef(rgn->bx*256, rgn->by*256, rgn->bz*256);
	
	glScalef(1.0/128, 1.0/128, 1.0/128);

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
	BGBDT_VoxRegion *rcur;
	
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
}
