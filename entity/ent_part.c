BGBDT_Particle *bgbdt_part_first;
BGBDT_Particle *bgbdt_part_free;

float *bgbdt_part_tris_xyz;
float *bgbdt_part_tris_st;
byte *bgbdt_part_tris_rgba;
int bgbdt_part_n_tris;
int bgbdt_part_m_tris;
int bgbdt_part_texmat;

BGBDT_Particle *BGBDT_Part_Alloc(void)
{
	BGBDT_Particle *tmp, *ttmp;
	int i;
	
	tmp=bgbdt_part_free;
	if(tmp)
	{
		bgbdt_part_free=tmp->next;
		memset(tmp, 0, sizeof(BGBDT_Particle));
		return(tmp);
	}
	
	ttmp=dtmAlloc("bgbdt_particle_t", 128*sizeof(BGBDT_Particle));
	
	for(i=0; i<128; i++)
	{
		ttmp->next=bgbdt_part_free;
		bgbdt_part_free=ttmp;
		ttmp++;
	}

	tmp=bgbdt_part_free;
	bgbdt_part_free=tmp->next;
	memset(tmp, 0, sizeof(BGBDT_Particle));
	return(tmp);
}

BGBDT_Particle *BGBDT_Part_SpawnParticle(vec3d org, vec3 vel, int atxy)
{
	BGBDT_Particle *tmp;
	
	tmp=BGBDT_Part_Alloc();
	
//	tmp->org=org;
//	tmp->vel=vel;

	vdvec3d(tmp->org, org);
	vfvec3(tmp->vel, vel);

	tmp->atxy=atxy;
	tmp->zacc=128;
	tmp->rad=64;
	tmp->clr=0xFFFFFFFFU;
	tmp->clrvel=0x80808080U;
	tmp->ttl=2000;

//	tmp->zacc=zacc;
//	tmp->clr=clr;
	
	tmp->next=bgbdt_part_first;
	bgbdt_part_first=tmp;
	
	return(tmp);
}

float bgbdt_part_randf()
{
	return((rand()&32767)/32767.0);
}

float bgbdt_part_randsf()
{
	return(((rand()&32767)/32767.0)*2.0-1.0);
}

float bgbdt_part_scurve(float a)
	{ return(a*a*(3.0-2.0*a)); }

BTEIFGL_API void BGBDT_Part_ParticleExplosionBasic(
	vec3d org, vec3 bvel, vec3 rvel, int cnt, int atxy, u32 clr)
{
	BGBDT_Particle *tmp;
	vec3 tvel, trvel;
	int i;
	
	for(i=0; i<cnt; i++)
	{
//		trvel=vec3(
//			bgbdt_part_scurve(bgbdt_part_randsf()),
//			bgbdt_part_scurve(bgbdt_part_randsf()),
//			bgbdt_part_scurve(bgbdt_part_randsf()));

//		trvel=vec3(
//			bgbdt_part_scurve(bgbdt_part_randf())*2-1,
//			bgbdt_part_scurve(bgbdt_part_randf())*2-1,
//			bgbdt_part_scurve(bgbdt_part_randf())*2-1);

		trvel=vec3(
			bgbdt_part_randsf(),
			bgbdt_part_randsf(),
			bgbdt_part_randsf());
		trvel=v3norm(trvel);
		trvel=v3scale(trvel, 1.0+bgbdt_part_randsf()*0.25);

		tvel=v3add(bvel, v3mul(rvel, trvel));

		tmp=BGBDT_Part_SpawnParticle(org, tvel, atxy);
		tmp->clr=clr;
		tmp->ttl=500;
	}
}

BTEIFGL_API void BGBDT_Part_ParticleInitPara(
	BGBDT_ParticleEmission *para)
{
	memset(para, 0, sizeof(BGBDT_ParticleEmission));
	para->bvel=vec3(0,0,0);
	para->rvel=vec3(25,25,25);
	para->cnt=128;
	para->atxy=0;
	para->zacc=0x80;
	para->clr=0xFF7F7F7FU;
	para->clrvel=0x80808080U;
	para->rad=64;
	para->ttl=500;
}

BTEIFGL_API void BGBDT_Part_ParticleEmitPara(
	BGBDT_ParticleEmission *para)
{
	BGBDT_Particle *tmp;
	vec3 tvel, trvel;
	int i;
	
	for(i=0; i<para->cnt; i++)
	{
		trvel=vec3(
			bgbdt_part_randsf(),
			bgbdt_part_randsf(),
			bgbdt_part_randsf());
		trvel=v3norm(trvel);
		trvel=v3scale(trvel, 1.0+bgbdt_part_randsf()*0.25);

		tvel=v3add(para->bvel, v3mul(para->rvel, trvel));

		tmp=BGBDT_Part_SpawnParticle(para->org, tvel, para->atxy);
		tmp->zacc=para->zacc;
		tmp->clr=para->clr;
		tmp->clrvel=para->clrvel;
		tmp->rad=para->rad;
		tmp->ttl=para->ttl;
		
		BGBDT_Part_UpdateParticleClr(tmp, bgbdt_part_randsf()*0.2);
	}
}


void BGBDT_Part_UpdateParticleClr(BGBDT_Particle *part, double dt)
{
	int dtms, d4ms;
	int cva, cvb, cvc, cvd;

	dtms=dt*1000;
	d4ms=dtms>>2;
	cva=((part->clrvel   )&0x00FF00FF)-0x00800080;
	cvb=((part->clrvel>>8)&0x00FF00FF)-0x00800080;
	cva=((cva*d4ms)>>8)&0x00FF00FF;
	cvb=((cvb*d4ms)>>8)&0x00FF00FF;
//	cvc=((part->clrvel   )&0x00FF00FF)+cva;
//	cvd=((part->clrvel>>8)&0x00FF00FF)+cvb;

	part->clr=part->clr+((cvb<<8)+cva);
}

void BGBDT_Part_UpdateParticle(BGBDT_Particle *part, double dt)
{
	double org1[3];
	float vel1[3];
	double f, g;
	BGBDT_VoxCoord xyz;
	BGBDT_VoxData td;
	int vty, vfl, dtms, d4ms;
	int cva, cvb, cvc, cvd;

//	V3F_ADDSCALE(part->org, part->vel, dt, part->org);
	V3F_ADDSCALE(part->org, part->vel, dt, org1);
//	part->vel[2]+=(part->zacc-128.0)*(dt/16.0);
	part->vel[2]+=(part->zacc-128.0)*(dt/4.0);

//	part->org=v3dadd(part->org,
//		v3ftov3d(v3scale(part->vel, dt)));
//	part->vel=v3add(part->vel,
//		vec3(0, 0, (part->zacc-128.0)*(dt/16.0)));
//	part->ttl-=dt*1000;

	dtms=dt*1000;
	part->ttl-=dtms;

	d4ms=dtms>>2;
	cva=((part->clrvel   )&0x00FF00FF)-0x00800080;
	cvb=((part->clrvel>>8)&0x00FF00FF)-0x00800080;
	cva=((cva*d4ms)>>8)&0x00FF00FF;
	cvb=((cvb*d4ms)>>8)&0x00FF00FF;
//	cvc=((part->clrvel   )&0x00FF00FF)+cva;
//	cvd=((part->clrvel>>8)&0x00FF00FF)+cvb;
//	part->clrvel=part->clrvel+((cvb<<8)+cva);
	part->clr=part->clr+((cvb<<8)+cva);

//	V3F_COPY(org1, part->org);

#if 1
	xyz.x=org1[0]*BGBDT_XYZ_SCALE_FROMMETER;
	xyz.y=org1[1]*BGBDT_XYZ_SCALE_FROMMETER;
	xyz.z=org1[2]*BGBDT_XYZ_SCALE_FROMMETER;
	BGBDT_WorldGetVoxelData(bt2ent_voxworld, xyz, &td, NULL,
		BGBDT_ACCFL_NOLOAD);
	BGBDT_WorldVoxel_GetTypeIdFlags(bt2ent_voxworld, xyz, td, &vty, &vfl);

	if(!(vfl&BGBDT_VOXFL_NONSOLID))
	{
		f=V3F_LEN(part->vel);
//		vel1[0]=bgbdt_part_randsf();
//		vel1[1]=bgbdt_part_randsf();
//		vel1[2]=bgbdt_part_randsf();

		vel1[0]=part->lgvel[0]+0.85*f*bgbdt_part_randsf();
		vel1[1]=part->lgvel[1]+0.85*f*bgbdt_part_randsf();
		vel1[2]=part->lgvel[2]+0.85*f*bgbdt_part_randsf();

		V3F_NORMALIZE(vel1, vel1);
		V3F_SCALE(vel1, f, part->vel);
	}else
	{
		V3F_COPY(org1, part->org);
		V3F_COPY(part->vel, part->lgvel);
	}
#endif
}

void BGBDT_Part_PartLocalOrigin(BGBDT_Particle *part, float *lorg)
{
//	lorg[0]=v3dx(part->org)-(bt2ent_voxworld->reforg[0]);
//	lorg[1]=v3dy(part->org)-(bt2ent_voxworld->reforg[1]);
//	lorg[2]=v3dz(part->org)-(bt2ent_voxworld->reforg[2]);

	lorg[0]=(part->org[0])-(bt2ent_voxworld->reforg[0]);
	lorg[1]=(part->org[1])-(bt2ent_voxworld->reforg[1]);
	lorg[2]=(part->org[2])-(bt2ent_voxworld->reforg[2]);
}

double BGBDT_Part_PartCameraDistance(BGBDT_Particle *part)
{
	float lorg[3];
	double d, dx, dy, dz;

	BGBDT_Part_PartLocalOrigin(part, lorg);
	
	dx=lorg[0]-(bt2ent_voxworld->camorg[0]);
	dy=lorg[1]-(bt2ent_voxworld->camorg[1]);
	dz=lorg[2]-(bt2ent_voxworld->camorg[2]);
	d=dx*dx+dy*dy+dz*dz;
	return(sqrt(d));
}

BTEIFGL_API void BGBDT_Part_UpdateParticles(double dt)
{
	BGBDT_Particle *cur, *lst, *nxt;
	double d0, d1;
	
	cur=bgbdt_part_first; lst=NULL;
	while(cur)
	{
		nxt=cur->next;

		BGBDT_Part_UpdateParticle(cur, dt);

		if(cur->ttl<0)
		{
			if(lst)
			{
				lst->next=cur->next;
			}else
			{
				bgbdt_part_first=cur->next;
			}
			
			cur->next=bgbdt_part_free;
			bgbdt_part_free=cur;
			
			cur=nxt;
			continue;
		}
		
		lst=cur;
		cur=nxt;
	}

	cur=bgbdt_part_first; lst=NULL;
	while(cur)
	{
		nxt=cur->next;
		if(!nxt)
			break;
		
		d0=BGBDT_Part_PartCameraDistance(cur);
		d1=BGBDT_Part_PartCameraDistance(nxt);
		if(d0<d1)
		{
			if(lst)
			{
				lst->next=nxt;
				cur->next=nxt->next;
				nxt->next=cur;
			}else
			{
				bgbdt_part_first=nxt;
				cur->next=nxt->next;
				nxt->next=cur;
			}
			lst=nxt;
		}else
		{
			lst=cur;
			cur=nxt;
		}
	}
}


void BGBDT_Part_DrawEmitParticle(BGBDT_Particle *part)
{
	float lorg[3], cam_x[3], cam_z[3];
	float pt0[3], pt1[3], pt2[3], pt3[3];
	float st0[2], st1[2], st2[2], st3[2];
//	float clr[4];
	float s0, s1, t0, t1;
	float *xyz, *st;
	byte *rgba;
	float rad;

	if((bgbdt_part_n_tris+2)>=bgbdt_part_m_tris)
		return;

	xyz=bgbdt_part_tris_xyz+bgbdt_part_n_tris*3*3;
	st=bgbdt_part_tris_st+bgbdt_part_n_tris*3*2;
	rgba=bgbdt_part_tris_rgba+bgbdt_part_n_tris*3*4;
	bgbdt_part_n_tris+=2;


	BGBDT_Part_PartLocalOrigin(part, lorg);
	rad=part->rad*(1.0/1000.0);
	
	V3F_COPY(bt2ent_voxworld->camrot+0, cam_x);
	V3F_COPY(bt2ent_voxworld->camrot+6, cam_z);
	
	V3F_ADDSCALE(lorg, cam_z, -rad, pt0);
	V3F_ADDSCALE(pt0, cam_x, -rad, pt0);
	V3F_ADDSCALE(lorg, cam_z, -rad, pt1);
	V3F_ADDSCALE(pt1, cam_x,  rad, pt1);
	V3F_ADDSCALE(lorg, cam_z, rad, pt2);
	V3F_ADDSCALE(pt2, cam_x, -rad, pt2);
	V3F_ADDSCALE(lorg, cam_z, rad, pt3);
	V3F_ADDSCALE(pt3, cam_x,  rad, pt3);

	s0=((part->atxy&15)+(1.0/64.0))*(1.0/16);
	s1=s0+(1.0-(2.0/64.0))*(1.0/16);
	t0=((15-((part->atxy>>4)&15))+(1.0/64.0))*(1.0/16);
	t1=t0+(1.0-(2.0/64.0))*(1.0/16);

	st0[0]=s0; st0[1]=t0;
	st1[0]=s1; st1[1]=t0;
	st2[0]=s0; st2[1]=t1;
	st3[0]=s1; st3[1]=t1;

	V3F_COPY(pt0, xyz+0*3);
	V3F_COPY(pt1, xyz+1*3);
	V3F_COPY(pt3, xyz+2*3);
	V3F_COPY(pt0, xyz+3*3);
	V3F_COPY(pt3, xyz+4*3);
	V3F_COPY(pt2, xyz+5*3);

	V2F_COPY(st0, st+0*2);
	V2F_COPY(st1, st+1*2);
	V2F_COPY(st3, st+2*2);
	V2F_COPY(st0, st+3*2);
	V2F_COPY(st3, st+4*2);
	V2F_COPY(st2, st+5*2);
	
	((u32 *)rgba)[0]=part->clr;
	((u32 *)rgba)[1]=part->clr;
	((u32 *)rgba)[2]=part->clr;
	((u32 *)rgba)[3]=part->clr;
	((u32 *)rgba)[4]=part->clr;
	((u32 *)rgba)[5]=part->clr;
}

BTEIFGL_API void BGBDT_Part_DrawParticles(void)
{
	BGBDT_Particle *cur;
	int m;
	
	if(!bgbdt_part_first)
		return;
	
	if(!bgbdt_part_tris_xyz)
	{
//		m=16384;
		m=65536;
		bgbdt_part_tris_xyz=frgl_malloc(m*3*3*sizeof(float));
		bgbdt_part_tris_st=frgl_malloc(m*3*2*sizeof(float));
		bgbdt_part_tris_rgba=frgl_malloc(m*3*4*sizeof(byte));
		bgbdt_part_m_tris=m;

		bgbdt_part_texmat=FRGL_TexMat_GetLoadIndex(
			"images/part/partgrid0");
	}
	
	bgbdt_part_n_tris=0;
	
	cur=bgbdt_part_first;
	while(cur)
	{
		BGBDT_Part_DrawEmitParticle(cur);
		cur=cur->next;
	}
	
	if(bgbdt_part_n_tris<=0)
		return;

	frglDepthFunc(GL_LEQUAL);
	frglBlendFunc(GL_ONE, GL_ZERO);
	frglEnable(GL_CULL_FACE);
	frglEnable(GL_ALPHA_TEST);

	FRGL_TexMat_BindMaterial(bgbdt_part_texmat);

	FRGL_DrawPrim_DrawArraysTexRGB(
		GL_TRIANGLES, 0, bgbdt_part_n_tris*3,
		3, GL_FLOAT, 3*4, bgbdt_part_tris_xyz,
		2, GL_FLOAT, 2*4, bgbdt_part_tris_st,
		4, GL_UNSIGNED_BYTE, 4, bgbdt_part_tris_rgba);

	frglDisable(GL_ALPHA_TEST);
	frglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
