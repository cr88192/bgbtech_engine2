#include <bteifgl.h>

float bgbdt_voxel_drawdist=256;
bool bgbdt_voxel_noshader=0;
bool bgbdt_voxel_novbo=0;
// float bgbdt_voxel_drawdist=192;

void BGBDT_CalcCoordLocalOrigin(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, float *vec)
{
//	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER);
//	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER);
//	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER);

	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[0]);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[1]);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[2]);
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
	
	dx=((xyz.x*BGBDT_XYZ_SCALE_TOMETER)-(world->camorg[0]+world->reforg[0]));
	dy=((xyz.y*BGBDT_XYZ_SCALE_TOMETER)-(world->camorg[1]+world->reforg[1]));
	dz=((xyz.z*BGBDT_XYZ_SCALE_TOMETER)-(world->camorg[2]+world->reforg[2]));
	
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

	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[0]);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[1]);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[2]);
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

	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[0]);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[1]);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[2]);
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

	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[0]);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[1]);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[2]);
	vec[3]=14;
}

BTEIFGL_API BGBDT_VoxCoord BGBDT_ConvLocalToVoxCoord(
	BGBDT_VoxWorld *world, float *vec)
{
	BGBDT_VoxCoord xyz;
	
	xyz.x=(vec[0]+world->reforg[0])/BGBDT_XYZ_SCALE_TOMETER+0.5;
	xyz.y=(vec[1]+world->reforg[1])/BGBDT_XYZ_SCALE_TOMETER+0.5;
	xyz.z=(vec[2]+world->reforg[2])/BGBDT_XYZ_SCALE_TOMETER+0.5;
	
	return(xyz);
}

BTEIFGL_API void BGBDT_ConvVoxToLocalCoord(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord xyz, float *vec)
{
	vec[0]=(xyz.x*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[0]);
	vec[1]=(xyz.y*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[1]);
	vec[2]=(xyz.z*BGBDT_XYZ_SCALE_TOMETER)-(world->reforg[2]);
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

	if(!(mesh->flags&BGBDT_MESHFL_QINHIBIT))
	{
		if(mesh->vistick)
		{
			mesh->vistick--;
			return;
		}

		if(mesh->flags&BGBDT_MESHFL_QLIVE)
		{
			frglGetQueryObjectuiv(mesh->oqm_id, GL_QUERY_RESULT, &i);
			if(i<=0)
			{
				mesh->flags&=~BGBDT_MESHFL_QLIVE;
				mesh->vistick=4;
				return;
			}
		}

		frglBeginQuery(GL_SAMPLES_PASSED, mesh->oqm_id);
	}

	if(mesh->vbo_id>0)
		frglBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_id);

	for(i=0; i<mesh->va_nmesh; i++)
	{
		FRGL_TexMat_BindMaterial(mesh->va_mesh[i*4+2]);

		if(mesh->vbo_id>0)
		{
			FRGL_DrawPrim_DrawArraysNormalTexRGB(
				GL_TRIANGLES, mesh->va_mesh[i*4+0], mesh->va_mesh[i*4+1],
				3, GL_SHORT, 3*2, (void *)(nlint)(mesh->ofs_xyz),
				2, GL_HALF_FLOAT, 2*2, (void *)(nlint)(mesh->ofs_st),
				3, GL_BYTE, 3, (void *)(nlint)(mesh->ofs_norm),
				4, GL_UNSIGNED_BYTE, 4, (void *)(nlint)(mesh->ofs_rgba));
		}else
		{
			FRGL_DrawPrim_DrawArraysNormalTexRGB(
				GL_TRIANGLES, mesh->va_mesh[i*4+0], mesh->va_mesh[i*4+1],
				3, GL_SHORT, 3*2, mesh->va_xyz,
				2, GL_HALF_FLOAT, 2*2, mesh->va_st,
				3, GL_BYTE, 3, mesh->va_norm,
				4, GL_UNSIGNED_BYTE, 4, mesh->va_rgba);
		}
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
//	frglBindBuffer(GL_ARRAY_BUFFER, 0);

	if(!(mesh->flags&BGBDT_MESHFL_QINHIBIT))
	{
		frglEndQuery(GL_SAMPLES_PASSED);
		mesh->flags|=BGBDT_MESHFL_QLIVE;
	}
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

	t0=frgl_clock();
	if(t0<(rgn->lastcvs+25) && (t0>rgn->lastcvs))
		return;
	rgn->lastcvs=t0;

//	world->vischk_tot=0;
//	world->vischk_tris=0;
	rgn->cvschk_tot=0;
	rgn->cvschk_tris=0;

	cvs=NULL;
	mcur=rgn->pvs;
	while(mcur)
	{
		BGBDT_CalcChunkMeshFrustumCenter(world, mcur, lvorg);
//		if((lvorg[2]+lvorg[7])<0)
//			{ mcur=mcur->pvsnext; continue; }
//		if((fabs(lvorg[0])-lvorg[3])>2)
//			{ mcur=mcur->pvsnext; continue; }
//		if((fabs(lvorg[1])-lvorg[3])>2)

		if(	((lvorg[2]+lvorg[7])<0) ||
			((fabs(lvorg[0])-lvorg[3])>2.25) ||
			((fabs(lvorg[1])-lvorg[3])>2) )
		{
			mcur->flags&=~BGBDT_MESHFL_VISALL;
			mcur->vistick=0;
			mcur=mcur->pvsnext;
			continue;
		}

		if(lvorg[2]<60)
		{
			mcur->flags|=BGBDT_MESHFL_QINHIBIT;
		}else
		{
			mcur->flags&=~BGBDT_MESHFL_QINHIBIT;
		}

//		world->vischk_tot++;
//		world->vischk_tris+=mcur->ntris;
		rgn->cvschk_tot++;
		rgn->cvschk_tris+=mcur->ntris;

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
	double f, g, d;
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
//	if(V3F_DIST(world->camorg, lorg)>256)
//		return;

	d=bgbdt_voxel_drawdist;
	if(d<128)d=128;
//	if(V3F_DIST(world->camorg, lorg)>(bgbdt_voxel_drawdist*1.5))
	if(V3F_DIST(world->camorg, lorg)>(d*1.5))
		return;

//	BGBDT_TickVoxRegion(world, rgn);
	BGBDT_UpdateVoxRegionCVS(world, rgn);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glEnable(GL_ALPHA_TEST);
	
//	glTranslatef(rgn->bx*256, rgn->by*256, rgn->bz*256);
	glTranslatef(
		(rgn->bx*256.0)-(world->reforg[0]),
		(rgn->by*256.0)-(world->reforg[1]),
		(rgn->bz*256.0)-(world->reforg[2]));
	
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
//		world->vischk_tot++;
//		world->vischk_tris+=mesh->ntris;

		BGBDT_DrawVoxChunkMesh(world, mesh);
		mesh=mesh->cvsnext;
	}
	frglBindBuffer(GL_ARRAY_BUFFER, 0);
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

	glDisable(GL_ALPHA_TEST);

	glPopMatrix();
}

BTEIFGL_API void BGBDT_DrawVoxWorld(BGBDT_VoxWorld *world)
{
	float lorg[4], lorg2[4];
	float bmin[3], bmax[3];
	BGBDT_VoxRegion *rcur, *rnxt, *rfst, *rcur2, *rprv2;
	double f, g, h, d, d2;
	float x0, y0, z0, x1, y1, z1;
	float x2, y2, z2, x3, y3, z3;
	float a0, a1, a2, a3;
	int x, y;
	int tx;

	world->tickstart=frgl_clock();

	bgbdt_voxel_drawdist=FRGL_CvarGetNum("r_drawdist");
	bgbdt_voxel_noshader=FRGL_CvarGetNum("r_noshader");
	bgbdt_voxel_novbo=FRGL_CvarGetNum("r_novbo");

	FRGL_TexMat_BindMaterial(0);

	if(world->insky && (world->insky<4))
	{
		switch(world->insky)
		{
		case 1: tx=FRGL_TexMat_GetLoadIndex("textures/sky1"); break;
		case 2: tx=FRGL_TexMat_GetLoadIndex("textures/sky2"); break;
		case 3: tx=FRGL_TexMat_GetLoadIndex("textures/sky3"); break;
		default: tx=0; break;
		}
		
		if(tx>0)
		{
			bmin[0]=world->camorg[0]-6000;
			bmin[1]=world->camorg[1]-6000;
			bmin[2]=world->camorg[2]-6000;

			bmax[0]=world->camorg[0]+6000;
			bmax[1]=world->camorg[1]+6000;
			bmax[2]=world->camorg[2]+6000;
		
//			frglColor4f(2,2,2,1);
			if(bgbdt_voxel_noshader)
			{
				frglColor4f(1,1,1,1);
			}else
			{
				frglColor4f(0.5,0.5,0.5,0x81/255.0);
			}
			FRGL_TexMat_BindMaterial(tx);
			
			h=1.0/8000;
			
			frglBegin(GL_QUADS);

			for(y=0; y<24; y++)
				for(x=0; x<48; x++)
			{
				a0=(x+0)*(2*M_PI/48);
				a1=(x+1)*(2*M_PI/48);

				a2=((y-4)+0)*(2*M_PI/24);
				a3=((y-4)+1)*(2*M_PI/24);

				f=cos(a2)*8000;
				x0=sin(a0)*f; y0=cos(a0)*f;
				x1=sin(a1)*f; y1=cos(a1)*f;
				z0=sin(a2)*8000;

				f=cos(a3)*8000;
				x2=sin(a0)*f; y2=cos(a0)*f;
				x3=sin(a1)*f; y3=cos(a1)*f;
				z1=sin(a3)*8000;

				frglTexCoord2f(x1*h*0.5+0.5, y1*h*0.5+0.5);
				frglVertex3f(x1, y1, z0);
				frglTexCoord2f(x0*h*0.5+0.5, y0*h*0.5+0.5);
				frglVertex3f(x0, y0, z0);
				frglTexCoord2f(x2*h*0.5+0.5, y2*h*0.5+0.5);
				frglVertex3f(x2, y2, z1);
				frglTexCoord2f(x3*h*0.5+0.5, y3*h*0.5+0.5);
				frglVertex3f(x3, y3, z1);
			}

			frglEnd();
		}
	}

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

	world->vischk_tot=0;
	world->vischk_tris=0;
	world->pvschk_tot=0;
	world->pvschk_tris=0;
	world->rawchk_tot=0;
	world->rawchk_tris=0;

	FRGL_TexMat_BindMaterial(0);

	rcur=world->region;
	while(rcur)
	{
		world->vischk_tot+=rcur->cvschk_tot;
		world->vischk_tris+=rcur->cvschk_tris;
		world->pvschk_tot+=rcur->pvschk_tot;
		world->pvschk_tris+=rcur->pvschk_tris;
		world->rawchk_tot+=rcur->rawchk_tot;
		world->rawchk_tris+=rcur->rawchk_tris;
	
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

	FRGL_TexMat_BindMaterial(0);

	world->dt_draw=frgl_clock()-world->tickstart;
}
