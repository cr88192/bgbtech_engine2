#include <bteifgl.h>

BGBDT_VoxChunkMesh *BGBDT_AllocChunkMesh(BGBDT_VoxWorld *world)
{
	BGBDT_VoxChunkMesh *tmp;

	if(world->freemesh)
	{
		tmp=world->freemesh;
		world->freemesh=tmp->next;
		memset(tmp, 0, sizeof(BGBDT_VoxChunkMesh));
		return(tmp);
	}
	
	tmp=frgl_talloc("bgbdt_voxchunkmesh_t", sizeof(BGBDT_VoxChunkMesh));
	return(tmp);
}

void BGBDT_FreeChunkMeshTempData(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh)
{
	if(mesh->xyz)
	{
		BGBDT_WorldFreeVoxelTemp(mesh->rgn->world, mesh->xyz,
			mesh->mxyz*sizeof(BGBDT_VoxVertex));
		mesh->xyz=NULL;
	}

	if(mesh->tris)
	{
		BGBDT_WorldFreeVoxelTemp(mesh->rgn->world, mesh->tris,
			mesh->mtris*sizeof(BGBDT_VoxTriangle));
		mesh->tris=NULL;
	}
}

void BGBDT_FreeChunkMesh(BGBDT_VoxWorld *world, BGBDT_VoxChunkMesh *mesh)
{
	BGBDT_FreeChunkMeshTempData(world, mesh);

	if(mesh->vabuf)
	{
		BGBDT_WorldFreeVoxelTemp(world, mesh->vabuf, mesh->sz_vabuf);
		mesh->vabuf=0;
	}

	mesh->next=world->freemesh;
	world->freemesh=mesh;
}

int BGBDT_VoxMesh_IndexVertex(BGBDT_VoxChunkMesh *mesh,
	BGBDT_VoxVertex xyz)
{
	int h;
	int i, j, k;
	
	if(!mesh->xyz)
	{
		k=2048;
		mesh->xyz=BGBDT_WorldAllocVoxelTemp(mesh->rgn->world,
			k*sizeof(BGBDT_VoxVertex));
		mesh->nxyz=0;
		mesh->mxyz=k;
		
		for(i=0; i<64; i++)
			mesh->xyzh[i]=-1;
	}

#if 1
//	h=((xyz.x*251+xyz.y)*251+xyz.z)*251;
	h=((xyz.x<<4)^(xyz.y<<2)^xyz.z)*251;

	h=(h>>24)&63;
	i=mesh->xyzh[h];
	while(i>0)
	{
		if(	(mesh->xyz[i].x!=xyz.x) ||
			(mesh->xyz[i].y!=xyz.y) ||
			(mesh->xyz[i].z!=xyz.z))
		{
			i=mesh->xyz[i].next;
			continue;
		}
		mesh->xyz[i].fl|=xyz.fl;
		return(i);
	}
#endif

#if 0
	for(i=0; i<mesh->nxyz; i++)
	{
		if(	(mesh->xyz[i].x!=xyz.x) ||
			(mesh->xyz[i].y!=xyz.y) ||
			(mesh->xyz[i].z!=xyz.z))
				continue;
		mesh->xyz[i].fl|=xyz.fl;
		return(i);
//		if(!memcmp(mesh->xyz+i, &xyz, sizeof(BGBDT_VoxVertex)))
//			return(i);
	}
#endif

	if(mesh->nxyz<mesh->mxyz)
	{
		i=mesh->nxyz++;
		mesh->xyz[i].x=xyz.x;
		mesh->xyz[i].y=xyz.y;
		mesh->xyz[i].z=xyz.z;
		mesh->xyz[i].fl=xyz.fl;
		mesh->xyz[i].next=mesh->xyzh[h];
		mesh->xyzh[h]=i;
		return(i);
	}
	
	k=mesh->mxyz; k=k+(k>>1);
	mesh->xyz=BGBDT_WorldReallocVoxelTemp(
		mesh->rgn->world, mesh->xyz,
		mesh->mxyz*sizeof(BGBDT_VoxVertex),
		k*sizeof(BGBDT_VoxVertex));
	mesh->mxyz=k;

	i=mesh->nxyz++;
	mesh->xyz[i].x=xyz.x;
	mesh->xyz[i].y=xyz.y;
	mesh->xyz[i].z=xyz.z;
	mesh->xyz[i].fl=xyz.fl;
	mesh->xyz[i].next=mesh->xyzh[h];
	mesh->xyzh[h]=i;
	return(i);
}


int BGBDT_VoxMesh_IndexTriangle(BGBDT_VoxChunkMesh *mesh,
	BGBDT_VoxTriangle tri)
{
	int i, j, k;
	
	if(!mesh->tris)
	{
		k=2048;
		mesh->tris=BGBDT_WorldAllocVoxelTemp(mesh->rgn->world,
			k*sizeof(BGBDT_VoxTriangle));
		mesh->ntris=0;
		mesh->mtris=k;
	}
	
#if 0
	for(i=0; i<mesh->ntris; i++)
		if(!memcmp(mesh->tris+i, &tri, sizeof(BGBDT_VoxTriangle)))
			return(i);
#endif

	if(mesh->ntris<mesh->mtris)
	{
		i=mesh->ntris++;
		mesh->tris[i]=tri;
//		memcpy(mesh->tris+i, &tri, sizeof(BGBDT_VoxTriangle));
		return(i);
	}
	
	k=mesh->mtris; k=k+(k>>1);
	mesh->tris=BGBDT_WorldReallocVoxelTemp(
		mesh->rgn->world, mesh->tris,
		mesh->mtris*sizeof(BGBDT_VoxTriangle),
		k*sizeof(BGBDT_VoxTriangle));
	mesh->mtris=k;

	i=mesh->ntris++;
	mesh->tris[i]=tri;
//	memcpy(mesh->tris+i, &tri, sizeof(BGBDT_VoxTriangle));
	return(i);
}

int BGBDT_VoxMesh_IndexMaterial(BGBDT_VoxChunkMesh *mesh, int mat)
{
	int i, j, k;

	if(!mesh->mat)
	{
		k=16;
		mesh->mat=BGBDT_WorldAllocVoxelTemp(mesh->rgn->world,
			k*2*sizeof(int));
		mesh->nmat=0;
		mesh->mmat=k;
	}

	for(i=0; i<mesh->nmat; i++)
	{
		if(mesh->mat[i*2+0]==mat)
			return(i);
	}
	
	i=mesh->nmat++;
	mesh->mat[i*2+0]=mat;
	mesh->mat[i*2+1]=-1;
	return(i);
}

int BGBDT_VoxMesh_EmitTriangle(BGBDT_VoxChunkMesh *mesh,
	BGBDT_VoxVertex v0, BGBDT_VoxVertex v1, BGBDT_VoxVertex v2,
	int mat, int atxy, int light)
{
	BGBDT_VoxTriangle tri;
	int i0, i1, i2, ixm;
	int i;
	
	i0=BGBDT_VoxMesh_IndexVertex(mesh, v0);
	i1=BGBDT_VoxMesh_IndexVertex(mesh, v1);
	i2=BGBDT_VoxMesh_IndexVertex(mesh, v2);

	ixm=BGBDT_VoxMesh_IndexMaterial(mesh, mat);

	tri.v0=i0;
	tri.v1=i1;
	tri.v2=i2;
	tri.mat=mat;
	tri.atxy=atxy;
	tri.light=light;
	tri.matnext=mesh->mat[ixm*2+1];
	
	i=BGBDT_VoxMesh_IndexTriangle(mesh, tri);
	mesh->mat[ixm*2+1]=i;
	
	return(i);
}

int BGBDT_VoxMesh_EmitQuad(BGBDT_VoxChunkMesh *mesh,
	BGBDT_VoxVertex v0, BGBDT_VoxVertex v1,
	BGBDT_VoxVertex v2, BGBDT_VoxVertex v3,
	int mat, int atxy, int light)
{
	BGBDT_VoxMesh_EmitTriangle(mesh, v0, v1, v2, mat, atxy, light);
	BGBDT_VoxMesh_EmitTriangle(mesh, v0, v2, v3, mat, atxy, light);
	return(0);
}

int BGBDT_VoxMesh_EmitBlockFacesGeneric(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh, BGBDT_VoxChunk *chk,
	int bx, int by, int bz,
	BGBDT_VoxData td, BGBDT_VoxDataStatus tds)
{
	BGBDT_VoxVertex v0, v1, v2, v3, v4, v5, v6, v7;
	BGBDT_VoxVertex v08, v09, v10, v11, v12, v13, v14, v15;
	int li[6];
	BGBDT_VoxCoord xyzm, xyzn, xyzc;
	BGBDT_VoxTypeInfo *tyi;
	int tix, atxys, atxyt, atxyb;
	int vxfl, fmfl, vty;

	vty=td.vtypel|(td.vtypeh<<8);
	tix=vty&4095;
//	tix=td.vtype&4095;
	tyi=world->voxtypes[tix];
	if(!tyi)
		return(-1);

	if(tyi->flags&BGBDT_VOXFL_NOFACES)
		return(0);

	xyzm=BGBDT_WorldGetChunkVoxCoord(
		world, chk, bx, by, bz);
//	xyzn=BGBDT_WorldGetChunkVoxCoord(
//		world, chk, bx+1, by+1, bz+1);
	xyzn.x=xyzm.x+(BGBDT_XYZ_OFS_VOXEL-1);
	xyzn.y=xyzm.y+(BGBDT_XYZ_OFS_VOXEL-1);
	xyzn.z=xyzm.z+(BGBDT_XYZ_OFS_VOXEL-1);

	xyzc.x=xyzm.x+((BGBDT_XYZ_OFS_VOXEL-1)>>1);
	xyzc.y=xyzm.y+((BGBDT_XYZ_OFS_VOXEL-1)>>1);
	xyzc.z=xyzm.z+((BGBDT_XYZ_OFS_VOXEL-1)>>1);

	if((bx<15) || (chk->bx<15))
		{ xyzn.x++; }
	if((by<15) || (chk->by<15))
		{ xyzn.y++; }
	if((bz<15) || (chk->bz<15))
		{ xyzn.z++; }

//	xyzn.x=xyzm.x+BGBDT_XYZ_OFS_VOXEL;
//	xyzn.y=xyzm.y+BGBDT_XYZ_OFS_VOXEL;
//	xyzn.z=xyzm.z+BGBDT_XYZ_OFS_VOXEL;
//	if(xyzn.x>>BGBDT_XYZ_SHR_CHUNK)xyzn.x--;
//	if(xyzn.y>>BGBDT_XYZ_SHR_CHUNK)xyzn.y--;
//	if(xyzn.z>>BGBDT_XYZ_SHR_CHUNK)xyzn.z--;
	
	vxfl=0;
	vty=td.vtypel|(td.vtypeh<<8);
//	if(!(td.vtype&BGBDT_VTYFL_FLEXFORM))
	if(!(vty&BGBDT_VTYFL_FLEXFORM))
		vxfl|=BGBDT_VTXFL_NOMOVE;
	
	atxys=tyi->mat_side_x|(tyi->mat_side_y<<4);
	atxyt=tyi->mat_top_x|(tyi->mat_top_y<<4);
	atxyb=tyi->mat_bot_x|(tyi->mat_bot_y<<4);

	
	v0.x=xyzm.x; v0.y=xyzm.y; v0.z=xyzm.z; v0.fl=vxfl;
	v1.x=xyzn.x; v1.y=xyzm.y; v1.z=xyzm.z; v1.fl=vxfl;
	v2.x=xyzm.x; v2.y=xyzn.y; v2.z=xyzm.z; v2.fl=vxfl;
	v3.x=xyzn.x; v3.y=xyzn.y; v3.z=xyzm.z; v3.fl=vxfl;
	v4.x=xyzm.x; v4.y=xyzm.y; v4.z=xyzn.z; v4.fl=vxfl;
	v5.x=xyzn.x; v5.y=xyzm.y; v5.z=xyzn.z; v5.fl=vxfl;
	v6.x=xyzm.x; v6.y=xyzn.y; v6.z=xyzn.z; v6.fl=vxfl;
	v7.x=xyzn.x; v7.y=xyzn.y; v7.z=xyzn.z; v7.fl=vxfl;

	if(tyi->flags&BGBDT_VOXFL_CROSSSPR)
	{
		v08.x=xyzm.x; v08.y=xyzc.y; v08.z=xyzm.z; v08.fl=vxfl;
		v09.x=xyzn.x; v09.y=xyzc.y; v09.z=xyzm.z; v09.fl=vxfl;
		v10.x=xyzm.x; v10.y=xyzc.y; v10.z=xyzn.z; v10.fl=vxfl;
		v11.x=xyzn.x; v11.y=xyzc.y; v11.z=xyzn.z; v11.fl=vxfl;
		
		v12.x=xyzc.x; v12.y=xyzm.y; v12.z=xyzm.z; v12.fl=vxfl;
		v13.x=xyzc.x; v13.y=xyzn.y; v13.z=xyzm.z; v13.fl=vxfl;
		v14.x=xyzc.x; v14.y=xyzm.y; v14.z=xyzn.z; v14.fl=vxfl;
		v15.x=xyzc.x; v15.y=xyzn.y; v15.z=xyzn.z; v15.fl=vxfl;
	}

//	if(tds.adjfl&BGBDT_ADJFL_OPAQUE_S)
	if(tds.adjfl&BGBDT_ADJFL_SOLID_S)
	{
		fmfl=tds.adjfl&BGBDT_ADJFL_OPAQUE_MASK;
		fmfl&=(tds.adjfl<<6);
	}else
	{
		fmfl=tds.adjfl&BGBDT_ADJFL_OPAQUE_MASK;
	}
	
//	fmfl=0;
	
	li[0]=tds.adjlit[0];	li[1]=tds.adjlit[1];
	li[2]=tds.adjlit[2];	li[3]=tds.adjlit[3];
	li[4]=tds.adjlit[4];	li[5]=tds.adjlit[5];

	if(tyi->flags&BGBDT_VOXFL_CROSSSPR)
	{
		BGBDT_VoxMesh_EmitQuad(mesh, v08, v09, v11, v10,
			tyi->mat_side, atxys, li[5]);
		BGBDT_VoxMesh_EmitQuad(mesh, v09, v08, v10, v11,
			tyi->mat_side, atxys, li[5]);

		BGBDT_VoxMesh_EmitQuad(mesh, v12, v13, v15, v14,
			tyi->mat_side, atxys, li[5]);
		BGBDT_VoxMesh_EmitQuad(mesh, v13, v12, v14, v15,
			tyi->mat_side, atxys, li[5]);

		return(0);
	}

	if((tyi->flags&BGBDT_VOXFL_FLUID) &&
		(tyi->flags&BGBDT_VOXFL_TRANSPARENT))
	{
		if(!(fmfl&BGBDT_ADJFL_OPAQUE_NX))
			BGBDT_VoxMesh_EmitQuad(mesh, v4, v5, v2, v0,
				tyi->mat_side, atxys, li[0]);
		if(!(fmfl&BGBDT_ADJFL_OPAQUE_PX))
			BGBDT_VoxMesh_EmitQuad(mesh, v7, v5, v1, v3,
				tyi->mat_side, atxys, li[1]);

		if(!(fmfl&BGBDT_ADJFL_OPAQUE_NY))
			BGBDT_VoxMesh_EmitQuad(mesh, v5, v4, v0, v1,
				tyi->mat_side, atxys, li[2]);
		if(!(fmfl&BGBDT_ADJFL_OPAQUE_PY))
			BGBDT_VoxMesh_EmitQuad(mesh, v6, v7, v3, v2,
				tyi->mat_side, atxys, li[3]);

		if(!(fmfl&BGBDT_ADJFL_OPAQUE_NZ))
			BGBDT_VoxMesh_EmitQuad(mesh, v2, v3, v1, v0,
				tyi->mat_bot, atxyt, li[4]);
		if(!(fmfl&BGBDT_ADJFL_OPAQUE_PZ))
			BGBDT_VoxMesh_EmitQuad(mesh, v7, v6, v4, v5,
				tyi->mat_top, atxyb, li[5]);
	}

	if(!(fmfl&BGBDT_ADJFL_OPAQUE_NX))
		BGBDT_VoxMesh_EmitQuad(mesh, v0, v2, v6, v4,
			tyi->mat_side, atxys, li[0]);
	if(!(fmfl&BGBDT_ADJFL_OPAQUE_PX))
		BGBDT_VoxMesh_EmitQuad(mesh, v3, v1, v5, v7,
			tyi->mat_side, atxys, li[1]);

//	if(!(fmfl&BGBDT_ADJFL_OPAQUE_NY))
//		BGBDT_VoxMesh_EmitQuad(mesh, v0, v1, v5, v4,
//			tyi->mat_side, atxys, li[2]);
//	if(!(fmfl&BGBDT_ADJFL_OPAQUE_PY))
//		BGBDT_VoxMesh_EmitQuad(mesh, v3, v2, v6, v7,
//			tyi->mat_side, atxys, li[3]);

	if(!(fmfl&BGBDT_ADJFL_OPAQUE_NY))
		BGBDT_VoxMesh_EmitQuad(mesh, v1, v0, v4, v5,
			tyi->mat_side, atxys, li[2]);
	if(!(fmfl&BGBDT_ADJFL_OPAQUE_PY))
		BGBDT_VoxMesh_EmitQuad(mesh, v2, v3, v7, v6,
			tyi->mat_side, atxys, li[3]);

//	if(!(fmfl&BGBDT_ADJFL_OPAQUE_NZ))
//		BGBDT_VoxMesh_EmitQuad(mesh, v1, v0, v2, v3,
//			tyi->mat_bot, atxyt, li[4]);
//	if(!(fmfl&BGBDT_ADJFL_OPAQUE_PZ))
//		BGBDT_VoxMesh_EmitQuad(mesh, v4, v5, v7, v6,
//			tyi->mat_top, atxyb, li[5]);

	if(!(fmfl&BGBDT_ADJFL_OPAQUE_NZ))
		BGBDT_VoxMesh_EmitQuad(mesh, v0, v1, v3, v2,
			tyi->mat_bot, atxyt, li[4]);
	if(!(fmfl&BGBDT_ADJFL_OPAQUE_PZ))
		BGBDT_VoxMesh_EmitQuad(mesh, v5, v4, v6, v7,
			tyi->mat_top, atxyb, li[5]);

	return(0);
}

int BGBDT_VoxMesh_EmitChunkFaces(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh, BGBDT_VoxChunk *chk)
{
	static signed char relchidx[6][3]={
		{-1,0,0}, {1,0,0}, {0,-1,0}, {0,1,0}, {0,0,-1}, {0,0,1}};
	BGBDT_VoxData td;
	BGBDT_VoxDataStatus tds;
	BGBDT_VoxChunk *chk1, *chk2, *chk3, *chk4, *chk5, *chk6;
	BGBDT_VoxTypeInfo *tyi;
	int tix, vty;
	int x, y, z;
	int i;
	
	mesh->nmat=0;

	BGBDT_WorldUpdateChunkSolid(world, chk);

	if(chk->flags&BGBDT_CHKFL_ONLYAIR)
		return(0);

	if((chk->flags&BGBDT_CHKFL_ONLYSOLID) &&
		(chk->bx>0) && (chk->by>0) && 
		(chk->bx<15) && (chk->by<15))
//		(chk->bx>0) && (chk->by>0) && (chk->bz>0) &&
//		(chk->bx<15) && (chk->by<15) && (chk->bz<15))
	{
		for(i=0; i<4; i++)
		{
			chk1=BGBDT_GetRegionChunk(world, chk->rgn,
				chk->bx+relchidx[i][0],
				chk->by+relchidx[i][1],
				chk->bz+relchidx[i][2],
				0);
//				BGBDT_ACCFL_NOLOAD);
			if(!(chk1->flags&BGBDT_CHKFL_ONLYSOLID))
				break;
		}
		if(i>=4)
			return(0);
	}
	
	for(z=0; z<16; z++)
		for(y=0; y<16; y++)
			for(x=0; x<16; x++)
	{
		BGBDT_WorldGetChunkVoxelData(world,
			x, y, z, chk, &td, &tds,
			BGBDT_ACCFL_CHKADJ);
//			BGBDT_ACCFL_NOLOAD|BGBDT_ACCFL_CHKADJ);
//		BGBDT_VoxMesh_EmitChunkBlockFaces(world, mesh, chk,
//			x, y, z, td, tds);

		vty=td.vtypel|(td.vtypeh<<8);
		tix=vty&4095;
//		tix=td.vtype&4095;
		tyi=world->voxtypes[tix];
		if(!tyi)continue;
		
		if(tyi->EmitBlockFaces)
		{
			tyi->EmitBlockFaces(world, mesh, chk,
				x, y, z, td, tds);
			continue;
		}else
		{
			BGBDT_VoxMesh_EmitBlockFacesGeneric(world, mesh, chk,
				x, y, z, td, tds);
		}
	}
	return(0);
}

#if 0
int BGBDT_VoxMesh_IndexVaVertex(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh,
	float *xyz, float *st,
	byte *rgba, byte *norm)
{
	u16 *up;
	byte *bp;
	float *fp;
	int i, j, k;
	
#if 0
	for(i=0; i<mesh->va_nvec; i++)
	{
		fp=mesh->va_xyz+i*3;
		if(V3F_DIST2(fp, xyz)>0.01)
			continue;
		fp=mesh->va_st+i*2;
		if(V2F_DIST2(fp, st)>0.00001)
			continue;

		bp=mesh->va_rgba+i*4;
		j=bp[0]-rgba[0]; k=j*j;
		j=bp[1]-rgba[1]; k+=j*j;
		j=bp[2]-rgba[2]; k+=j*j;
		j=bp[3]-rgba[3]; k+=j*j;
		if(k>16)continue;

		bp=mesh->va_norm+i*3;
		j=bp[0]-norm[0]; k=j*j;
		j=bp[1]-norm[1]; k+=j*j;
		j=bp[2]-norm[2]; k+=j*j;
		if(k>12)continue;
		
		break;
	}
	
	if(i<mesh->va_nvec)
		return(i);
#endif

	i=mesh->va_nvec++;

	fp=mesh->va_xyz+i*3;
	fp[0]=xyz[0]; fp[1]=xyz[1]; fp[2]=xyz[2];
	fp=mesh->va_st+i*2;
	fp[0]=st[0]; fp[1]=st[1];

	bp=mesh->va_rgba+i*4;
	bp[0]=rgba[0]; bp[1]=rgba[1];
	bp[2]=rgba[2]; bp[3]=rgba[3];

	bp=mesh->va_norm+i*3;
	bp[0]=norm[0]; bp[1]=norm[1];
	bp[2]=norm[2];
	
	return(i);
}
#endif

#if 1
u16 bgbdt_float2hf(float f)
{
	u16 hf;
	u32 v;
	int e, sg;
	
	sg=0x0000; e=0x2400;
	if(f<0) { sg=0x8000; f=-f; }
	v=f*65536;
	if(v>=1024)
	{
		while(v>=2048)
			{ v=v>>1; e=e+0x0400; }
		if(e>=0x7C00)
			{ return(sg|0x7C00); }
		hf=sg|e|(v&1023);
		return(hf);
	}else
	{
		if(!v)
			{ return(0); }
		while(v<=512)
			{ v=v<<1; e=e-0x0400; }
		if(e<0x0400)
			{ return(0); }
		hf=sg|e|(v&1023);
		return(hf);
	}
}

int BGBDT_VoxMesh_IndexVaVertex(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh,
	float *xyz, float *st,
	byte *rgba, byte *norm)
{
	u16 *up;
	byte *bp;
	float *fp;
	int i, j, k;
	
	i=mesh->va_nvec++;

	up=((u16 *)mesh->va_xyz)+i*3;
//	up[0]=xyz[0]*(32767/256.0);
//	up[1]=xyz[1]*(32767/256.0);
//	up[2]=xyz[2]*(32767/256.0);

	up[0]=xyz[0]*127.996+0.5;
	up[1]=xyz[1]*127.996+0.5;
	up[2]=xyz[2]*127.996+0.5;

//	up[0]=xyz[0];
//	up[1]=xyz[1];
//	up[2]=xyz[2];

//	up=((u16 *)mesh->va_st)+i*2;
//	up[0]=st[0]*256;
//	up[1]=st[1]*256;

//	fp=mesh->va_st+i*2;
//	fp[0]=st[0]; fp[1]=st[1];

	up=((u16 *)mesh->va_st)+i*2;
	up[0]=bgbdt_float2hf(st[0]);
	up[1]=bgbdt_float2hf(st[1]);

	bp=mesh->va_rgba+i*4;
	bp[0]=rgba[0]; bp[1]=rgba[1];
	bp[2]=rgba[2]; bp[3]=rgba[3];

	bp=mesh->va_norm+i*3;
	bp[0]=norm[0]; bp[1]=norm[1];
	bp[2]=norm[2];
	
	return(i);
}
#endif

float bgbdt_clamp01(float v)
{
	if(v<0.0)return(0.0);
	if(v>1.0)return(1.0);
	return(v);
}

void bgbdt_voxmesh_clrclamp(byte *clr, float cr, float cg, float cb)
{
	int tr, tg, tb;
	int te;
	
	if((cr<=1.2) && (cg<=1.2) && (cb<=1.2))
	{
		clr[0]=bgbdt_clamp01(cr)*255.0;
		clr[1]=bgbdt_clamp01(cg)*255.0;
		clr[2]=bgbdt_clamp01(cb)*255.0;
		clr[3]=255;
		
		return;
	}
	
	tr=cr*255.0;
	tg=cg*255.0;
	tb=cb*255.0;
	te=0x7F;
	while((tr>=256) || (tg>=256) || (tb>=256))
		{ tr=tr>>1; tg=tg>>1; tb=tb>>1; te++; }
	clr[0]=tr;
	clr[1]=tg;
	clr[2]=tb;
	clr[3]=te;
}

int BGBDT_VoxMesh_EmitVaTriangle(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh, BGBDT_VoxTriangle *tri)
{
	static float clrt[16][3]={
	  {255/441.7, 255/441.7, 255/441.7},	//0
	  { 64/270.6,  64/270.6, 255/270.6},	//1
	  { 64/270.6, 255/270.6,  64/270.6},	//2
	  { 64/366.3, 255/366.3, 255/366.3},	//3
	  {255/270.6,  64/270.6,  64/270.6},	//4
	  {255/366.3,  64/366.3, 255/366.3},	//5
	  {255/366.3, 255/366.3,  64/366.3},	//6
	  {255/441.7, 255/441.7, 255/441.7},	//7
	  {255/441.7, 255/441.7, 255/441.7},	//8
	  {192/372.5, 192/372.5, 255/270.6},	//9
	  {192/372.5, 255/372.5, 192/372.5},	//10
	  {192/409.7, 255/409.7, 255/409.7},	//11
	  {255/372.5, 192/372.5, 192/372.5},	//12
	  {255/409.7, 192/409.7, 255/409.7},	//13
	  {255/409.7, 255/409.7, 192/409.7},	//14
	  {255/255.0, 255/255.0, 255/255.0},	//15
	};
	static byte clrtab[256*4];
	static int iclrt=0;
	float xyz[3][3];
	float dv[3][3];
	float st[3][2];
	float tv[3];
	byte rgba[4][4];
	byte norm[4];
	int i0, i1, i2;
	float la, lb, lk;
	float ms, ns, mt, nt;
	int ax, atx, aty;
	int i, j, k;
	
	if(!iclrt)
	{
		iclrt=1;
		for(i=0; i<16; i++)
		{
			la=V3F_LEN(clrt[i]);
			lb=1.732/la;
			V3F_SCALE(clrt[i], lb, clrt[i]);
		}

		for(i=0; i<256; i++)
		{
			lb=((i>>4)&15)/15.0;
			k=i&15;

		//	lk=0.032;
			lk=0.064;
//			clrtab[i*4+0]=bgbdt_clamp01(lk+lb*clrt[k][0])*255.0;
//			clrtab[i*4+1]=bgbdt_clamp01(lk+lb*clrt[k][1])*255.0;
//			clrtab[i*4+2]=bgbdt_clamp01(lk+lb*clrt[k][2])*255.0;
//			clrtab[i*4+3]=255;
			bgbdt_voxmesh_clrclamp(clrtab+i*4,
				lk+lb*clrt[k][0],
				lk+lb*clrt[k][1],
				lk+lb*clrt[k][2]);
		}
	}
	
#if 0
	xyz[0][0]=mesh->xyz[tri->v0].x*BGBDT_XYZ_SCALE_TOMETER;
	xyz[0][1]=mesh->xyz[tri->v0].y*BGBDT_XYZ_SCALE_TOMETER;
	xyz[0][2]=mesh->xyz[tri->v0].z*BGBDT_XYZ_SCALE_TOMETER;
	xyz[1][0]=mesh->xyz[tri->v1].x*BGBDT_XYZ_SCALE_TOMETER;
	xyz[1][1]=mesh->xyz[tri->v1].y*BGBDT_XYZ_SCALE_TOMETER;
	xyz[1][2]=mesh->xyz[tri->v1].z*BGBDT_XYZ_SCALE_TOMETER;
	xyz[2][0]=mesh->xyz[tri->v2].x*BGBDT_XYZ_SCALE_TOMETER;
	xyz[2][1]=mesh->xyz[tri->v2].y*BGBDT_XYZ_SCALE_TOMETER;
	xyz[2][2]=mesh->xyz[tri->v2].z*BGBDT_XYZ_SCALE_TOMETER;
#endif

#if 1
	xyz[0][0]=(mesh->xyz[tri->v0].x&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;
	xyz[0][1]=(mesh->xyz[tri->v0].y&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;
	xyz[0][2]=(mesh->xyz[tri->v0].z&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;

	xyz[1][0]=(mesh->xyz[tri->v1].x&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;
	xyz[1][1]=(mesh->xyz[tri->v1].y&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;
	xyz[1][2]=(mesh->xyz[tri->v1].z&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;

	xyz[2][0]=(mesh->xyz[tri->v2].x&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;
	xyz[2][1]=(mesh->xyz[tri->v2].y&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;
	xyz[2][2]=(mesh->xyz[tri->v2].z&BGBDT_XYZ_MASK_REGION)*
		BGBDT_XYZ_SCALE_TOMETER;
#endif
	
	V3F_SUB(xyz[1], xyz[0], dv[0]);
	V3F_SUB(xyz[2], xyz[1], dv[1]);
//	V3F_CROSS(dv[0], dv[1], dv[2]);
	V3F_CROSS(dv[1], dv[0], dv[2]);
	V3F_NORMALIZE(dv[2], dv[2]);
	
//	norm[0]=dv[2][0]*127+128;
//	norm[1]=dv[2][1]*127+128;
//	norm[2]=dv[2][2]*127+128;

	norm[0]=dv[2][0]*127.5;
	norm[1]=dv[2][1]*127.5;
	norm[2]=dv[2][2]*127.5;

//	if((tri->light>>8)&255)
	if(tri->light>>8)
	{
//		la=((tri->light>>8)&255)/255.0;
		la=((tri->light>>8)&255)/128.0;
		lb=((tri->light>>4)&15)/15.0;
		k=tri->light&15;

	//	lk=0.032;
		lk=0.064;
//		rgba[0][0]=bgbdt_clamp01(lk+la+lb*clrt[k][0])*255.0;
//		rgba[0][1]=bgbdt_clamp01(lk+la+lb*clrt[k][1])*255.0;
//		rgba[0][2]=bgbdt_clamp01(lk+la+lb*clrt[k][2])*255.0;
//		rgba[0][3]=255;
		bgbdt_voxmesh_clrclamp(rgba[0],
			lk+la+lb*clrt[k][0],
			lk+la+lb*clrt[k][1],
			lk+la+lb*clrt[k][2]);
	}else
	{
		i=tri->light&255;
		rgba[0][0]=clrtab[i*4+0];
		rgba[0][1]=clrtab[i*4+1];
		rgba[0][2]=clrtab[i*4+2];
		rgba[0][3]=clrtab[i*4+3];
	}

	rgba[1][0]=rgba[0][0];	rgba[1][1]=rgba[0][1];
	rgba[1][2]=rgba[0][2];	rgba[1][3]=rgba[0][3];
	rgba[2][0]=rgba[0][0];	rgba[2][1]=rgba[0][1];
	rgba[2][2]=rgba[0][2];	rgba[2][3]=rgba[0][3];

//	rgba[0][0]=255;	rgba[0][1]=255;
//	rgba[0][2]=255;	rgba[0][3]=255;
//	rgba[1][0]=255;	rgba[1][1]=255;
//	rgba[1][2]=255;	rgba[1][3]=255;
//	rgba[2][0]=255;	rgba[2][1]=255;
//	rgba[2][2]=255;	rgba[2][3]=255;

#if 1
	tv[0]=dv[2][0]*dv[2][0];
	tv[1]=dv[2][1]*dv[2][1];
	tv[2]=dv[2][2]*dv[2][2];
	if(tv[0]>tv[1])
	{
//		if(tv[0]>tv[2])		{ ax=0; }
//		else				{ ax=2; }
		if(tv[0]>tv[2])		{ ax=(dv[0]>=0)?0:4; }
		else				{ ax=(dv[2]>=0)?2:6; }
	}else
	{
//		if(tv[1]>tv[2])		{ ax=1; }
//		else				{ ax=2; }
		if(tv[1]>tv[2])		{ ax=(dv[1]>=0)?1:5; }
		else				{ ax=(dv[2]>=0)?2:6; }
	}
#endif
	
#if 0
	if(dv[2][0]>dv[2][1])
	{
		if(dv[2][0]>dv[2][2])
			{ ax=0; }
		else
			{ ax=2; }
	}else
	{
		if(dv[2][1]>dv[2][2])
			{ ax=1; }
		else
			{ ax=2; }
	}
#endif
	
	switch(ax)
	{
	case 0:
		st[0][0]=xyz[0][1];		st[0][1]=xyz[0][2];
		st[1][0]=xyz[1][1];		st[1][1]=xyz[1][2];
		st[2][0]=xyz[2][1];		st[2][1]=xyz[2][2];
		break;
	case 1:
		st[0][0]=xyz[0][0];		st[0][1]=xyz[0][2];
		st[1][0]=xyz[1][0];		st[1][1]=xyz[1][2];
		st[2][0]=xyz[2][0];		st[2][1]=xyz[2][2];
		break;
	case 2:
		st[0][0]=xyz[0][0];		st[0][1]=xyz[0][1];
		st[1][0]=xyz[1][0];		st[1][1]=xyz[1][1];
		st[2][0]=xyz[2][0];		st[2][1]=xyz[2][1];
		break;

	case 4:
		st[0][0]=-xyz[0][1];	st[0][1]=xyz[0][2];
		st[1][0]=-xyz[1][1];	st[1][1]=xyz[1][2];
		st[2][0]=-xyz[2][1];	st[2][1]=xyz[2][2];
		break;
	case 5:
		st[0][0]=-xyz[0][0];	st[0][1]=xyz[0][2];
		st[1][0]=-xyz[1][0];	st[1][1]=xyz[1][2];
		st[2][0]=-xyz[2][0];	st[2][1]=xyz[2][2];
		break;
	case 6:
		st[0][0]=-xyz[0][0];	st[0][1]=xyz[0][1];
		st[1][0]=-xyz[1][0];	st[1][1]=xyz[1][1];
		st[2][0]=-xyz[2][0];	st[2][1]=xyz[2][1];
		break;

	default:
		st[0][0]=xyz[0][0];		st[0][1]=xyz[0][1];
		st[1][0]=xyz[1][0];		st[1][1]=xyz[1][1];
		st[2][0]=xyz[2][0];		st[2][1]=xyz[2][1];
		break;
	}
	
	ms=st[0][0]; ns=st[0][0];
	mt=st[0][1]; nt=st[0][1];
	if(st[1][0]<ms)ms=st[1][0];
	if(st[1][0]>ns)ns=st[1][0];
	if(st[1][1]<mt)mt=st[1][1];
	if(st[1][1]>nt)nt=st[1][1];
	if(st[2][0]<ms)ms=st[2][0];
	if(st[2][0]>ns)ns=st[2][0];
	if(st[2][1]<mt)mt=st[2][1];
	if(st[2][1]>nt)nt=st[2][1];

	if(ns<(-3))
	{
		i=((int)(-ns))-1;
		ms+=i; ns+=i; st[0][0]+=i; st[1][0]+=i; st[2][0]+=i;
	}

	if(nt<(-3))
	{
		i=((int)(-nt))-1;
		mt+=i; nt+=i; st[0][1]+=i; st[1][1]+=i; st[2][1]+=i;
	}

	if(ns>3)
	{
		i=((int)ns)-1;
		ms-=i; ns-=i; st[0][0]-=i; st[1][0]-=i; st[2][0]-=i;
	}

	if(nt>3)
	{
		i=((int)nt)-1;
		mt-=i; nt-=i; st[0][1]-=i; st[1][1]-=i; st[2][1]-=i;
	}
	
	while(ms<(-0.5))
		{ ms+=1.0; ns+=1.0; st[0][0]+=1.0; st[1][0]+=1.0; st[2][0]+=1.0; }
	while(ns>(1.5))
		{ ms-=1.0; ns-=1.0; st[0][0]-=1.0; st[1][0]-=1.0; st[2][0]-=1.0; }

	while(mt<(-0.5))
		{ mt+=1.0; nt+=1.0; st[0][1]+=1.0; st[1][1]+=1.0; st[2][1]+=1.0; }
	while(nt>(1.5))
		{ mt-=1.0; nt-=1.0; st[0][1]-=1.0; st[1][1]-=1.0; st[2][1]-=1.0; }
	
	if(tri->atxy)
	{
		atx=tri->atxy&15;
		aty=15-(tri->atxy>>4)&15;
	
		st[0][0]=bgbdt_clamp01(st[0][0])*(1.0/16-0.001)+(atx/16.0+0.0005);
		st[0][1]=bgbdt_clamp01(st[0][1])*(1.0/16-0.001)+(aty/16.0+0.0005);
		st[1][0]=bgbdt_clamp01(st[1][0])*(1.0/16-0.001)+(atx/16.0+0.0005);
		st[1][1]=bgbdt_clamp01(st[1][1])*(1.0/16-0.001)+(aty/16.0+0.0005);
		st[2][0]=bgbdt_clamp01(st[2][0])*(1.0/16-0.001)+(atx/16.0+0.0005);
		st[2][1]=bgbdt_clamp01(st[2][1])*(1.0/16-0.001)+(aty/16.0+0.0005);
	}
	
	i0=BGBDT_VoxMesh_IndexVaVertex(world, mesh,
		xyz[0], st[0], rgba[0], norm);
	i1=BGBDT_VoxMesh_IndexVaVertex(world, mesh,
		xyz[1], st[1], rgba[1], norm);
	i2=BGBDT_VoxMesh_IndexVaVertex(world, mesh,
		xyz[2], st[2], rgba[2], norm);

#if 0
	i=mesh->va_nelem;
	mesh->va_nelem+=3;
	mesh->va_elem[i+0]=i0;
	mesh->va_elem[i+1]=i1;
	mesh->va_elem[i+2]=i2;
#endif

	return(0);
}

int BGBDT_VoxMesh_RebuildArrays(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh)
{
	BGBDT_VoxTriangle *tri;
	byte *tbuf, *tbuf2;
//	byte *cs, *ct;
	int nv, ne, nm, c, sz, szt, id;
	int i0, i1, i2;
	int i, j, k;
	
	if(mesh->vabuf)
	{
		BGBDT_WorldFreeVoxelTemp(world, mesh->vabuf, mesh->sz_vabuf);
		mesh->vabuf=NULL;
	}
	
	nv=mesh->ntris*4;
	szt=nv*10*4+mesh->nmat*4*4;
	szt=(szt+1023)&(~1023);
	
//	szt+=4096;
//	szt=1<<24;
	
	tbuf=frgl_malloc(szt);
	
	sz=0;
	mesh->va_xyz=(float *)(tbuf+sz);
//	sz+=nv*3*sizeof(float);
	sz+=nv*3*sizeof(u16);	sz=(sz+3)&(~3);
	mesh->va_st=(float *)(tbuf+sz);	
//	sz+=nv*2*sizeof(float);
	sz+=nv*2*sizeof(u16);	sz=(sz+3)&(~3);
	mesh->va_rgba=(tbuf+sz);
	sz+=nv*4*sizeof(byte);	sz=(sz+3)&(~3);
	mesh->va_norm=(tbuf+sz);
	sz+=nv*3*sizeof(byte); sz=(sz+3)&(~3);
	mesh->va_elem=(int *)(tbuf+sz);
	sz+=nv*sizeof(int);
	mesh->va_mesh=(int *)(tbuf+sz);
	
	for(i=0; i<mesh->nmat; i++)
	{
//		i0=mesh->va_nelem;
		i0=mesh->va_nvec;
	
		c=mesh->mat[i*2+1];
		while(c>=0)
		{
			tri=mesh->tris+c;
			BGBDT_VoxMesh_EmitVaTriangle(world, mesh, tri);
			c=tri->matnext;
		}

//		i1=mesh->va_nelem;
		i1=mesh->va_nvec;
		i2=mesh->mat[i*2+0];
		
		j=mesh->va_nmesh++;
		mesh->va_mesh[j*4+0]=i0;
		mesh->va_mesh[j*4+1]=i1-i0;
		mesh->va_mesh[j*4+2]=i2;
		mesh->va_mesh[j*4+3]=0;
	}
	
	sz=0; nv=mesh->va_nvec; ne=mesh->va_nelem; nm=mesh->va_nmesh;
//	mesh->ofs_xyz=sz;	sz+=nv*3*sizeof(float);	sz=(sz+15)&(~15);
//	mesh->ofs_st=sz;	sz+=nv*2*sizeof(float);	sz=(sz+15)&(~15);
//	mesh->ofs_rgba=sz;	sz+=nv*4*sizeof(byte);		sz=(sz+15)&(~15);
//	mesh->ofs_norm=sz;	sz+=nv*3*sizeof(byte);		sz=(sz+15)&(~15);
//	mesh->ofs_elem=sz;	sz+=ne  *sizeof(int);		sz=(sz+15)&(~15);
//	mesh->ofs_mesh=sz;	sz+=nm*4*sizeof(int);		sz=(sz+15)&(~15);

	mesh->ofs_xyz=sz;	sz+=nv*3*sizeof(u16);		sz=(sz+3)&(~3);
	mesh->ofs_st=sz;	sz+=nv*2*sizeof(u16);		sz=(sz+3)&(~3);
	mesh->ofs_rgba=sz;	sz+=nv*4*sizeof(byte);		sz=(sz+3)&(~3);
	mesh->ofs_norm=sz;	sz+=nv*3*sizeof(byte);		sz=(sz+3)&(~3);
	mesh->ofs_mesh=sz;	sz+=nm*4*sizeof(int);		sz=(sz+3)&(~3);
	
//	sz+=1024;
//	sz+=65536;
	
	tbuf2=BGBDT_WorldAllocVoxelTemp(world, sz);
//	memcpy(tbuf2+mesh->ofs_xyz , mesh->va_xyz , nv*3*sizeof(float));
//	memcpy(tbuf2+mesh->ofs_st  , mesh->va_st  , nv*2*sizeof(float));
//	memcpy(tbuf2+mesh->ofs_rgba, mesh->va_rgba, nv*4*sizeof(byte));
//	memcpy(tbuf2+mesh->ofs_norm, mesh->va_norm, nv*3*sizeof(byte));
//	memcpy(tbuf2+mesh->ofs_elem, mesh->va_elem, ne  *sizeof(int));
//	memcpy(tbuf2+mesh->ofs_mesh, mesh->va_mesh, nm*4*sizeof(int));

	memcpy(tbuf2+mesh->ofs_xyz , mesh->va_xyz , nv*3*sizeof(u16));
	memcpy(tbuf2+mesh->ofs_st  , mesh->va_st  , nv*2*sizeof(u16));
	memcpy(tbuf2+mesh->ofs_rgba, mesh->va_rgba, nv*4*sizeof(byte));
	memcpy(tbuf2+mesh->ofs_norm, mesh->va_norm, nv*3*sizeof(byte));
	memcpy(tbuf2+mesh->ofs_mesh, mesh->va_mesh, nm*4*sizeof(int));
	
	frgl_free(tbuf);
	
	mesh->vabuf=tbuf2;
	mesh->sz_vabuf=sz;
	mesh->va_xyz=(float *)(tbuf2+mesh->ofs_xyz);
	mesh->va_st=(float *)(tbuf2+mesh->ofs_st);
	mesh->va_rgba=tbuf2+mesh->ofs_rgba;
	mesh->va_norm=tbuf2+mesh->ofs_norm;
	mesh->va_elem=(int *)(tbuf2+mesh->ofs_elem);
	mesh->va_mesh=(int *)(tbuf2+mesh->ofs_mesh);

	if(mesh->vbo_id<=0)
	{
		frglGenBuffers(1, &id);
		frglBindBuffer(GL_ARRAY_BUFFER, id);
		frglBufferData(GL_ARRAY_BUFFER,
			mesh->sz_vabuf, mesh->vabuf, GL_STATIC_DRAW);
		mesh->vbo_id=id;
		frglBindBuffer(GL_ARRAY_BUFFER, 0);
	}else
	{
		frglBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_id);
		frglBufferData(GL_ARRAY_BUFFER,
			mesh->sz_vabuf, mesh->vabuf, GL_STATIC_DRAW);
		frglBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return(0);
}

int BGBDT_VoxMesh_RebuildChunkMesh(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *mesh, BGBDT_VoxChunk *chk)
{
	BGBDT_VoxMesh_EmitChunkFaces(world, mesh, chk);
	BGBDT_VoxMesh_RebuildArrays(world, mesh);
	BGBDT_FreeChunkMeshTempData(world, mesh);
	return(0);
}

int BGBDT_WorldDecodeChunkMesh(BGBDT_VoxWorld *world,
	BGBDT_VoxChunkMesh *chk, byte *ibuf, int accfl)
{
	byte *cs, *cse;
	int i, j;

	if(ibuf[0]==0xE4)
	{
		i=(ibuf[1]<<16)|(ibuf[2]<<8)|ibuf[3];
		
		cs=ibuf+8;
		cse=ibuf+i;
		
		if((ibuf[4]=='C') || (ibuf[5]=='H') ||
			(ibuf[6]=='K') || (ibuf[7]=='0'))
		{
			while(cs<cse)
			{
				if((*cs==0xE1) || (*cs==0xE3) || (*cs==0xE4))
				{
					j=BGBDT_WorldDecodeChunkMesh(world, chk, cs, accfl);
					if(j>0)return(j);
					i=(cs[1]<<16)|(cs[2]<<8)|cs[3];
					cs+=i;
					continue;
				}

				if((*cs==0xE2) || (*cs==0xE5))
				{
					j=BGBDT_WorldDecodeChunkMesh(world, chk, cs, accfl);
					if(j>0)return(j);
					i=cs[1];
					cs+=i;
					continue;
				}
			}
			return(0);
		}
		return(0);
	}

#if 0
	if(ibuf[0]==0xE1)
	{
		i=(ibuf[1]<<16)|(ibuf[2]<<8)|ibuf[3];
		BGBDT_WorldDecodeChunkBits(world, chk, ibuf+4, i-4, accfl);
		return(0);
	}
	
	if(ibuf[0]==0xE2)
	{
		i=ibuf[1];
		BGBDT_WorldDecodeChunkBits(world, chk, ibuf+2, i-2, accfl);
		return(0);
	}
#endif

	return(0);
}

BGBDT_VoxChunkMesh *BGBDT_WorldGetChunkMesh(BGBDT_VoxWorld *world,
	BGBDT_VoxCoord xyz, int accfl)
{
	BGBDT_VoxRegion *rgn;
	BGBDT_VoxChunk *chk;
	BGBDT_VoxChunkMesh *mesh;
	int bx, by, bz, bi;
	int i;
	
	rgn=BGBDT_WorldGetRegion(world, xyz, accfl);
	if(!rgn)return(NULL);
	
	bx=(xyz.x>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
	by=(xyz.y>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_XY;
	bz=(xyz.z>>BGBDT_XYZ_SHR_CHUNK)&BGBDT_XYZ_MASK_CHUNK_Z;
	
	mesh=BGBDT_GetRegionChunkMesh(
		world, rgn, bx, by, bz, accfl);
	return(mesh);
}

BGBDT_VoxChunkMesh *BGBDT_GetRegionChunkMesh(
	BGBDT_VoxWorld *world, BGBDT_VoxRegion *rgn,
	int bx, int by, int bz, int accfl)
{
	BGBDT_VoxCoord xyz;
	BGBDT_VoxChunk *chk;
	BGBDT_VoxChunkMesh *mesh;
	int bi;
	int i;

	bi=((bz*BGBDT_XYZ_SZ_REGION_XY)+by)*BGBDT_XYZ_SZ_REGION_XY+bx;

	mesh=rgn->chkmesh[bi];

	if(mesh && !(accfl&BGBDT_ACCFL_NOLOAD))
	{
		chk=rgn->chkptr[bi];
		if(chk && chk->flags&BGBDT_CHKFL_MESHDIRTY)
		{
			chk->flags&=~BGBDT_CHKFL_MESHDIRTY;
			BGBDT_FreeChunkMesh(world, mesh);
			mesh=NULL;
		}
	}

	if(mesh)
		return(mesh);
	
	if(accfl&BGBDT_ACCFL_NOLOAD)
		return(NULL);

	mesh=BGBDT_AllocChunkMesh(world);
	mesh->rgn=rgn;	mesh->bx=bx;
	mesh->by=by;	mesh->bz=bz;

	if(rgn->chkofs[bi])
	{	
		i=BGBDT_WorldDecodeChunkMesh(world, mesh,
			rgn->rgnbuf+rgn->chkofs[bi], accfl);
		if(i>0)
		{
			rgn->chkmesh[bi]=mesh;
			return(mesh);
		}
	}
	
	xyz=BGBDT_WorldGetRegionChunkCoord(world, rgn, bx, by, bz);
	chk=BGBDT_WorldGetChunk(world, xyz, accfl);
	if(chk)
	{
		BGBDT_VoxMesh_RebuildChunkMesh(world, mesh, chk);
		rgn->chkmesh[bi]=mesh;
		return(mesh);
	}
	
	BGBDT_FreeChunkMesh(world, mesh);
	return(NULL);
}
