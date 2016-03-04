#include <bteifgl.h>

void BGBDT_DrawVoxRegionEntity(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn, BGBDT_VoxRenderEntity *ent)
{
	float pt0[3], pt1[3], pt2[3], pt3[3];
	float org[4];
	float xs, ys, xo, yo;
	float x0, x1, y0, y1;
	float s0, s1, t0, t1;
	int frx, fry;
	
	BGBDT_CalcCoordLocalOrigin(world, ent->org, org);
	xs=ent->xs*BGBDT_XYZ_SCALE_TOMETER;
	ys=ent->ys*BGBDT_XYZ_SCALE_TOMETER;
	xo=ent->xo*BGBDT_XYZ_SCALE_TOMETER;
	yo=ent->yo*BGBDT_XYZ_SCALE_TOMETER;
	
	x0=xo-(xs/2);	x1=xo+(xs/2);
	y0=yo-(ys/2);	y1=yo+(ys/2);
	
	V3F_ADDSCALEADDSCALE(org, world->camrot+0, x0, world->camrot+6, y0, pt0);
	V3F_ADDSCALEADDSCALE(org, world->camrot+0, x1, world->camrot+6, y0, pt1);
	V3F_ADDSCALEADDSCALE(org, world->camrot+0, x1, world->camrot+6, y1, pt2);
	V3F_ADDSCALEADDSCALE(org, world->camrot+0, x0, world->camrot+6, y1, pt3);
	
	frx=ent->frame%ent->mat_w;
	fry=ent->mat_h-(ent->frame/ent->mat_w)-1;
	s0=(1.0*frx)/ent->mat_w;
	s1=s0+(1.0/ent->mat_w);
	t0=(1.0*fry)/ent->mat_h;
	t1=t0+(1.0/ent->mat_h);
	
	frglBegin(GL_QUADS);
	frglTexCoord2f(s0, t0);
	frglVertex3fv(pt0);
	frglTexCoord2f(s1, t0);
	frglVertex3fv(pt1);
	frglTexCoord2f(s1, t1);
	frglVertex3fv(pt2);
	frglTexCoord2f(s0, t1);
	frglVertex3fv(pt3);
	frglEnd();
}

void BGBDT_UpdateVoxRegionEntityCVS(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	BGBDT_VoxRenderEntity *ecur, *elst;
	
	ecur=rgn->ent; elst=NULL;
	while(ecur)
	{
		ecur->visnext=elst;
		elst=ecur;
		ecur=ecur->next;
	}
	rgn->visent=elst;
}

void BGBDT_DrawVoxRegionEntities(BGBDT_VoxWorld *world,
	BGBDT_VoxRegion *rgn)
{
	BGBDT_VoxRenderEntity *ecur;
	
	ecur=rgn->visent;
	while(ecur)
	{
		BGBDT_DrawVoxRegionEntity(world, rgn, ecur);
		ecur=ecur->visnext;
	}
}
