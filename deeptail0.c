#include <bteifgl.h>
#include <bteifgl_main.h>

BGBDT_VoxWorld *bgbdt_voxworld;

float bgbdt_org[3];
float bgbdt_ang[3];
float bgbdt_rot[9];

float bgbdt_imp[3];
float bgbdt_vel[3];

double bgbdt_absorg[3];
double bgbdt_reforg[3];

double bgbdt_spawnorg[3];
float bgbdt_spawnang[3];
bool isotest_player_spawnflip=false;

float bgbdt_tsorg[3];			//trace local origin
BGBDT_VoxCoord bgbdt_tsxyz;		//trace ending xyz
BGBDT_VoxCoord bgbdt_tslxyz;	//trace ending last xyz
BGBDT_VoxCoord bgbdt_tsexyz;	//trace ending entity xyz
dtVal bgbdt_tent;				//trace entity

dtVal bgbdt_player;				//player entity
int bgbdt_toolslot;

//char *bgbdt_toolspr[10];

int voxui_active;
int voxui_placeidx;
int voxui_voxui_idx;
int voxui_tsvidx;

BGBDT_VoxCoord voxui_xyz;
bool voxui_isedit;

char voxui_editbuf[256];
int voxui_editpos;

bool isotest_player_noclip=false;
bool isotest_player_undying=false;
bool isotest_player_onground=false;
bool isotest_player_flip=false;
bool isotest_player_isdead=false;

float bgbdt_player_movestep;
float bgbdt_player_moveswim;

double bgbdt_player_deadtime;

double bgbdt_worldtime;
bool bgbdt_uipause;

void isotest_menopt_toggle_act(FRGL_MenuInfo *menu, FRGL_MenuItem *mitm)
{
	if(!strcmp(mitm->name, "noclip"))
	{
		isotest_player_noclip=!isotest_player_noclip;
		mitm->opt_str=isotest_player_noclip?"True":"False";
		return;
	}

	if(!strcmp(mitm->name, "undying"))
	{
		isotest_player_undying=!isotest_player_undying;
		mitm->opt_str=isotest_player_undying?"True":"False";
		return;
	}

	if(!strcmp(mitm->name, "flip"))
	{
		isotest_player_flip=!isotest_player_flip;
		mitm->opt_str=isotest_player_flip?"True":"False";
		return;
	}

	if(!strcmp(mitm->name, "all_cookies"))
	{
		Bt2Ent_GiveItem((64<<8)|4);
		Bt2Ent_GiveItem((64<<8)|5);
	}
}

void isotest_menopt_newgame(FRGL_MenuInfo *menu, FRGL_MenuItem *mitm)
{
//	isotest_newgame_init();
}

int main_movetick_checkmove(float *porg)
{
	BGBDT_VoxCoord min, max, vorg;
	dtVal ents[16];
	BGBDT_VoxCoord pos[16];
	BGBDT_VoxData td[16];
	float pomin[4], pomax[4];
	char *str;
	int nvox, vfl, nent;
	int i, j, k;

#if 0
	if(porg[0]>( 524287))		porg[0]-=1048576;
	if(porg[1]>( 524287))		porg[1]-=1048576;
	if(porg[2]>( 524287))		porg[2]-=1048576;
	if(porg[0]<(-524288))		porg[0]+=1048576;
	if(porg[1]<(-524288))		porg[1]+=1048576;
	if(porg[2]<(-524288))		porg[2]+=1048576;
#endif

	if(isotest_player_isdead)
	{
		pomin[0]=porg[0]-0.40;
		pomin[1]=porg[1]-0.40;
		pomin[2]=porg[2]-0.40;

		pomax[0]=porg[0]+0.40;
		pomax[1]=porg[1]+0.40;
		pomax[2]=porg[2]+0.40;
	}else if(isotest_player_flip)
	{
		pomin[0]=porg[0]-0.40;
		pomin[1]=porg[1]-0.40;
		pomin[2]=porg[2]-0.20;

		pomax[0]=porg[0]+0.40;
		pomax[1]=porg[1]+0.40;
		pomax[2]=porg[2]+1.30;
	}else
	{
		pomin[0]=porg[0]-0.40;
		pomin[1]=porg[1]-0.40;
		pomin[2]=porg[2]-1.30;

		pomax[0]=porg[0]+0.40;
		pomax[1]=porg[1]+0.40;
		pomax[2]=porg[2]+0.20;
	}

	vorg=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, porg);

	min=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, pomin);
	max=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, pomax);
//	i=BGBDT_BoxQueryVoxel(bgbdt_voxworld, min, max, pos, td, &nvox,
//		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);
	i=BGBDT_BoxQueryVoxel(bgbdt_voxworld, min, max, pos, td, &nvox,
		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_SOLID);
	if(i)return(i);

	i=BGBDT_BoxQueryVoxel(bgbdt_voxworld, min, max, pos, td, &nvox,
		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);

	bgbdt_voxworld->inwater=0;
	bgbdt_voxworld->cam_inwater=0;
	if(i>0)
	{
		for(i=0; i<nvox; i++)
		{
			vfl=BGBDT_WorldVoxel_GetFlags(bgbdt_voxworld, pos[i], td[i]);
			if((vfl&BGBDT_VOXFL_PHYS_MASK)==BGBDT_VOXFL_PHYS_FLIPUP)
				isotest_player_flip=1;
			if((vfl&BGBDT_VOXFL_PHYS_MASK)==BGBDT_VOXFL_PHYS_FLIPDN)
				isotest_player_flip=0;
			if(vfl&BGBDT_VOXFL_FLUID)
			{
				if((pos[i].x>>BGBDT_XYZ_SHR_VOXEL)==
						(vorg.x>>BGBDT_XYZ_SHR_VOXEL) &&
					(pos[i].y>>BGBDT_XYZ_SHR_VOXEL)==
						(vorg.y>>BGBDT_XYZ_SHR_VOXEL) &&
					(pos[i].z>>BGBDT_XYZ_SHR_VOXEL)==
						(vorg.z>>BGBDT_XYZ_SHR_VOXEL))
				{
					bgbdt_voxworld->cam_inwater=(vfl>>4)&0x1F;
				}
			
				bgbdt_voxworld->inwater=(vfl>>4)&0x1F;
			}

			if((vfl&BGBDT_VOXFL_PHYS_MASK)==BGBDT_VOXFL_PHYS_TRIG)
			{
				j=td[i].vdatal|(td[i].vdatah<<8);
//				isotest_player_flip=0;
				str=BGBDT_WorldGetStringForIndex(bgbdt_voxworld,
					pos[i], 0, j);
				if(str)
					Con_StuffCmdBuf(str);
			}

			if((vfl&BGBDT_VOXFL_PHYS_MASK)==BGBDT_VOXFL_PHYS_LADDER)
			{
				bgbdt_voxworld->inwater=(vfl>>4)&0x1F;
			}
		}
	}

	i=Bt2Ent_BoxQueryWorldEnts(bgbdt_voxworld,
		bgbdt_voxworld->ent_player, min, max,
		ents, &nent, BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);
	if(i>0)
	{
		return(1);
	}

	return(0);
}

int main_movetick_findmove(
	float *org0, float *vel0,
	float *org1, float *vel1,
	double dt)
{
	float porg[4], pvel[4];
	float f, g;
	int i, j, k, fsg;

	fsg=1;
	if(isotest_player_flip)
		fsg=-1;

	V3F_ADDSCALE(org0, vel0, dt, porg);

//	isotest_player_onground=false;
	i=main_movetick_checkmove(porg);
	if(!i)
	{
		if(fabs(vel0[2])>0.01)
			isotest_player_onground=false;
		V3F_COPY(porg, org1);
		V3F_COPY(vel0, vel1);
		return(0);
	}
	
	if((vel0[2]*fsg)<=0)
	{
		V3F_COPY(vel0, pvel);
		pvel[2]=0;

		V3F_ADDSCALE(org0, pvel, dt, porg);
		i=main_movetick_checkmove(porg);
		if(!i)
		{
			isotest_player_onground=true;
			V3F_COPY(porg, org1);
			V3F_COPY(pvel, vel1);
			return(0);
		}

		porg[2]+=fsg*0.55;
		i=main_movetick_checkmove(porg);
		if(!i)
		{
			isotest_player_onground=true;
			V3F_COPY(porg, org1);
			V3F_COPY(pvel, vel1);
			return(0);
		}

		if((fabs(pvel[0])>0) && (fabs(pvel[1])>0))
		{
			if(fabs(pvel[0])<fabs(pvel[1]))
				{ pvel[0]=0; }
			else
				{ pvel[1]=0; }
		}

		V3F_ADDSCALE(org0, pvel, dt, porg);
		i=main_movetick_checkmove(porg);
		if(!i)
		{
			isotest_player_onground=true;
			V3F_COPY(porg, org1);
			V3F_COPY(pvel, vel1);
			return(0);
		}
	}

	isotest_player_onground=false;

	V3F_COPY(vel0, pvel);
	if((fabs(pvel[0])>0) && (fabs(pvel[1])>0) && (fabs(pvel[2])>0))
	{
		if(fabs(pvel[0])<fabs(pvel[1]))
		{
			if(fabs(pvel[2])<fabs(pvel[0]))
				{ pvel[2]=0; }
			else
				{ pvel[0]=0; }
		}
		else
		{
			if(fabs(pvel[2])<fabs(pvel[1]))
				{ pvel[2]=0; }
			else
				{ pvel[1]=0; }
		}
	}

	V3F_ADDSCALE(org0, pvel, dt, porg);
	i=main_movetick_checkmove(porg);
	if(!i)
	{
		V3F_COPY(porg, org1);
		V3F_COPY(pvel, vel1);
		return(0);
	}

	V3F_COPY(vel0, pvel);
	if((fabs(pvel[0])>0) && (fabs(pvel[1])>0))
	{
		if(fabs(pvel[0])<fabs(pvel[1]))
			{ pvel[0]=0; }
		else
			{ pvel[1]=0; }
	}

	V3F_ADDSCALE(org0, pvel, dt, porg);
	i=main_movetick_checkmove(porg);
	if(!i)
	{
		V3F_COPY(porg, org1);
		V3F_COPY(pvel, vel1);
		return(0);
	}

#if 1
	for(j=0; j<3; j++)
	{
		V3F_COPY(vel0, pvel);
		pvel[j]=0;

		V3F_ADDSCALE(org0, pvel, dt, porg);
		i=main_movetick_checkmove(porg);
		if(!i)
			break;
	}

	if(j<3)
	{
		V3F_COPY(porg, org1);
		V3F_COPY(pvel, vel1);
		return(0);
	}

	for(j=0; j<3; j++)
	{
		V3F_COPY(vel0, pvel);
		pvel[(j+1)%3]=0;
		pvel[(j+2)%3]=0;

		V3F_ADDSCALE(org0, pvel, dt, porg);
		i=main_movetick_checkmove(porg);
		if(!i)
			break;
	}

	if(j<3)
	{
		V3F_COPY(porg, org1);
		V3F_COPY(pvel, vel1);
		return(0);
	}
#endif

	V3F_COPY(org0, org1);
	V3F_ZERO(vel1);
	return(1);
}

int main_movetick_step(double dt)
{
	float porg[4], pvel[4], oorg[4], ovel[4];
	float f, g;
	int i, fsg;

//	if(bgbdt_uipause)
//		return(0);

	bgbdt_worldtime+=dt;

	if(bgbdt_ang[2]>=360)
		bgbdt_ang[2]-=360;
	if(bgbdt_ang[2]<0)
		bgbdt_ang[2]+=360;

	if(isotest_player_noclip)
	{
		V3F_ZERO(bgbdt_vel);
		V3F_ADDSCALE(bgbdt_org, bgbdt_imp, dt, bgbdt_org);
		return(0);
	}

	fsg=1;
	if(isotest_player_flip)
		fsg=-1;

	if(bgbdt_voxworld->inwater)
	{
		f=1.0-2*dt;
		if(f<0)f=0;
		V3F_SCALE(bgbdt_vel, f, bgbdt_vel);
		bgbdt_vel[2]-=fsg*2*dt;
	}else
	{
		bgbdt_vel[2]-=fsg*9.8*dt;
	}
	
	V3F_ADDSCALE(bgbdt_org, bgbdt_vel, dt, porg);

//	xyzs=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, bgbdt_org);
//	xyze=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, tv1);
//	i=BGBDT_RayCastVoxel(bgbdt_voxworld, xyzs, xyze,
//		&xyzt, &xyztl, &td, &tds,
//		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);

//	BGBDT_ConvVoxToLocalCoord(bgbdt_voxworld, xyzt, tsorg);

	V3F_COPY(bgbdt_org, oorg);
	V3F_COPY(bgbdt_vel, ovel);

	main_movetick_findmove(
		bgbdt_org, bgbdt_vel,
		bgbdt_org, bgbdt_vel,
		dt);

	f=V3F_DIST(bgbdt_vel, ovel);
	if(f>12)
	{
		BGBDT_Sound_PlaySound("sound/player/land", 32, 0, 0);
	}
	

	if(bgbdt_voxworld->inwater)
	{
		f=V3F_DIST(bgbdt_org, oorg);
		bgbdt_player_moveswim+=f;
		bgbdt_player_movestep=0;
		
		if(bgbdt_player_moveswim>8)
		{
			if(bgbdt_voxworld->inwater==4)
			{
			}else
			{
				BGBDT_Sound_PlaySound("sound/misc/watr_in", 32, 0, 0);
			}
			bgbdt_player_moveswim=0;
		}
	}else if(isotest_player_onground)
	{
		f=V2F_DIST(bgbdt_org, oorg);
		bgbdt_player_movestep+=f;
		bgbdt_player_moveswim=0;

		if(bgbdt_player_movestep>6)
		{
			BGBDT_Sound_PlaySound("sound/player/step", 8, 0, 0);
			bgbdt_player_movestep=0;
		}
	}else
	{
		bgbdt_player_moveswim=0;
		bgbdt_player_movestep=0;
	}

#if 0
	i=main_movetick_findmove(
		bgbdt_org, bgbdt_vel,
		porg, pvel, dt);
	if(!i)
	{
		V3F_COPY(pvel, bgbdt_vel);
		V3F_COPY(porg, bgbdt_org);
	}else
	{
		V3F_COPY(bgbdt_org, porg);
		V3F_COPY(bgbdt_vel, pvel);

		porg[2]+=0.65;
		i=main_movetick_findmove(
			porg, pvel, porg, pvel, dt);
		if(!i)
		{
			V3F_COPY(pvel, bgbdt_vel);
			V3F_COPY(porg, bgbdt_org);
		}else
		{
			V3F_ZERO(bgbdt_vel);
		}
	}
#endif

#if 0
	isotest_player_onground=false;
	i=main_movetick_checkmove(porg);
	if(i>0)
	{
#if 1
		V3F_COPY(bgbdt_vel, pvel);
		if((fabs(pvel[0])>0) && (fabs(pvel[1])>0))
		{
			if(fabs(pvel[0])<fabs(pvel[1]))
				{ pvel[0]=0; }
			else
				{ pvel[1]=0; }
		}else
		{
			pvel[0]=0;
			pvel[1]=0;
		}

		V3F_ADDSCALE(bgbdt_org, pvel, dt, porg);
		i=main_movetick_checkmove(porg);
		if(!i)
		{
			V3F_COPY(pvel, bgbdt_vel);
			V3F_COPY(porg, bgbdt_org);
		}else
		{
			isotest_player_onground=true;

			V3F_COPY(bgbdt_vel, pvel);

			if(!bgbdt_voxworld->inwater)
			{
				f=1-8*dt;
				if(f<0)f=0;
				pvel[0]*=f;
				pvel[1]*=f;
			}
			
			pvel[2]=0;

			V3F_ADDSCALE(bgbdt_org, pvel, dt, porg);
			i=main_movetick_checkmove(porg);

			if(!i)
			{
				V3F_COPY(pvel, bgbdt_vel);
				V3F_COPY(porg, bgbdt_org);
			}else
			{
				V3F_ZERO(bgbdt_vel);
			}
		}
#endif
	}else if(!i)
	{
		V3F_COPY(porg, bgbdt_org);
	}
#endif
	
	if(bgbdt_voxworld->inwater)
	{
		bgbdt_vel[0]+=bgbdt_imp[0]*dt*0.75;
		bgbdt_vel[1]+=bgbdt_imp[1]*dt*0.75;
		bgbdt_vel[2]+=bgbdt_imp[2]*dt*0.75;
	}else if(isotest_player_onground)
	{
//		bgbdt_vel[0]=0;
//		bgbdt_vel[1]=0;
//		bgbdt_vel[2]=0;

		f=1-8*dt;
		if(f<0)f=0;
		bgbdt_vel[0]*=f;
		bgbdt_vel[1]*=f;

//		bgbdt_vel[0]+=bgbdt_imp[0]*dt*12;
//		bgbdt_vel[1]+=bgbdt_imp[1]*dt*12;
		bgbdt_vel[0]+=bgbdt_imp[0]*0.5;
		bgbdt_vel[1]+=bgbdt_imp[1]*0.5;
		
		f=V2F_LEN(bgbdt_vel);
		if(f>7)
		{
			g=7/f;
			bgbdt_vel[0]*=g;
			bgbdt_vel[1]*=g;
		}
		
		if((fsg*bgbdt_imp[2])>7)
		{
			BGBDT_Sound_PlaySound("sound/player/jump1", 32, 0, 0);
			bgbdt_vel[2]+=fsg*7;
		}
	}else
	{
		bgbdt_vel[0]+=bgbdt_imp[0]*dt*0.10;
		bgbdt_vel[1]+=bgbdt_imp[1]*dt*0.10;

		f=V2F_LEN(bgbdt_vel);
		if(f>7)
		{
			g=7/f;
			bgbdt_vel[0]*=g;
			bgbdt_vel[1]*=g;
		}
	}

	return(0);
}

int main_saveplayer()
{
	char tb[256];
	BGBDT_VoxCoord xyz, org;
	VFILE *fd;
	int fl;
	s64 li, lj;
	int i, j, k;

#if 1
// #ifndef __EMSCRIPTEN__
	org=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, bgbdt_org);
//	fl=BGBDT_ACCFL_ENNEWRGN|BGBDT_ACCFL_ENNEWCHK;
	fl=BGBDT_ACCFL_ENNEWRGN;

	xyz.x=org.x-(2<<BGBDT_XYZ_SHR_CHUNK);
	xyz.y=org.y-(2<<BGBDT_XYZ_SHR_CHUNK);
	xyz.z=      (2<<BGBDT_XYZ_SHR_CHUNK);
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz, fl);

	xyz.x=org.x+(2<<BGBDT_XYZ_SHR_CHUNK);
	xyz.y=org.y-(2<<BGBDT_XYZ_SHR_CHUNK);
	xyz.z=      (2<<BGBDT_XYZ_SHR_CHUNK);
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz, fl);

	xyz.x=org.x-(2<<BGBDT_XYZ_SHR_CHUNK);
	xyz.y=org.y+(2<<BGBDT_XYZ_SHR_CHUNK);
	xyz.z=      (2<<BGBDT_XYZ_SHR_CHUNK);
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz, fl);

	xyz.x=org.x+(2<<BGBDT_XYZ_SHR_CHUNK);
	xyz.y=org.y+(2<<BGBDT_XYZ_SHR_CHUNK);
	xyz.z=      (2<<BGBDT_XYZ_SHR_CHUNK);
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz, fl);
#endif

	sprintf(tb, "region/%s/player.ini", bgbdt_voxworld->worldname);

	fd=vffopen(tb, "wt");
//	fd=vffopen("region/player.ini", "wt");
	if(!fd)return(-1);
	
	sprintf(tb, "origin %.3f %.3f %.3f\n",
		bgbdt_absorg[0], bgbdt_absorg[1], bgbdt_absorg[2]);
	vfwrite(tb, 1, strlen(tb), fd);
	sprintf(tb, "angles %.2f %.2f %.2f\n",
		bgbdt_ang[0], bgbdt_ang[1], bgbdt_ang[2]);
	vfwrite(tb, 1, strlen(tb), fd);

	sprintf(tb, "spawnorg %.3f %.3f %.3f\n",
		bgbdt_spawnorg[0], bgbdt_spawnorg[1], bgbdt_spawnorg[2]);
	vfwrite(tb, 1, strlen(tb), fd);
	sprintf(tb, "spawnang %.3f %.3f %.3f\n",
		bgbdt_spawnang[0], bgbdt_spawnang[1], bgbdt_spawnang[2]);
	vfwrite(tb, 1, strlen(tb), fd);

	sprintf(tb, "noclip %d\n",
		isotest_player_noclip);
	vfwrite(tb, 1, strlen(tb), fd);

	sprintf(tb, "zflip %d\n",
		isotest_player_flip);
	vfwrite(tb, 1, strlen(tb), fd);

	sprintf(tb, "spawnzflip %d\n",
		isotest_player_spawnflip);
	vfwrite(tb, 1, strlen(tb), fd);

	for(i=0; i<32; i++)
	{
		sprintf(tb, "inven %d 0x%08X 0x%08X 0x%08X 0x%08X\n",
			i*4,
			Bt2Ent_GetInvenSlot(i*4+0), Bt2Ent_GetInvenSlot(i*4+1),
			Bt2Ent_GetInvenSlot(i*4+2), Bt2Ent_GetInvenSlot(i*4+3));
		vfwrite(tb, 1, strlen(tb), fd);
	}
	
	k=Bt2Ent_GetNumToken();
	for(i=0; i<k; i++)
	{
		lj=Bt2Ent_GetTokenSlot(i);
		if(lj)
		{
			sprintf(tb, "token %d 0x%016llX\n", i, lj);
			vfwrite(tb, 1, strlen(tb), fd);
		}
	}

	vfclose(fd);
	return(0);
}

int main_loadplayer()
{
	char tb[256];
//	VFILE *fd;
	char *buf, *cs, *cse;
	char *s;
	char **a;
	s64 li, lj;
	int sz, b;
	int i, j, k;

//	fd=vffopen("region/player.ini", "wt");
//	if(!fd)return(-1);
	
	sprintf(tb, "region/%s/player.ini", bgbdt_voxworld->worldname);
	
	buf=vf_loadfile(tb, &sz);
//	buf=vf_loadfile("region/player.ini", &sz);
	if(!buf)
		return(0);
	
	cs=buf; cse=buf+sz;
	while(cs<cse)
	{
		s=frgl_rgets(&cs);
		a=frgl_rsplit(s);
		
		if(!a[0])continue;
		if(!strcmp(a[0], "origin"))
		{
			bgbdt_org[0]=atof(a[1]);
			bgbdt_org[1]=atof(a[2]);
			bgbdt_org[2]=atof(a[3]);
			continue;
		}

		if(!strcmp(a[0], "angles"))
		{
			bgbdt_ang[0]=atof(a[1]);
			bgbdt_ang[1]=atof(a[2]);
			bgbdt_ang[2]=atof(a[3]);
			continue;
		}

		if(!strcmp(a[0], "spawnorg"))
		{
			bgbdt_spawnorg[0]=atof(a[1]);
			bgbdt_spawnorg[1]=atof(a[2]);
			bgbdt_spawnorg[2]=atof(a[3]);
			continue;
		}

		if(!strcmp(a[0], "spawnang"))
		{
			bgbdt_spawnang[0]=atof(a[1]);
			bgbdt_spawnang[1]=atof(a[2]);
			bgbdt_spawnang[2]=atof(a[3]);
			continue;
		}

		if(!strcmp(a[0], "noclip"))
		{
			isotest_player_noclip=atoi(a[1]);
			continue;
		}

		if(!strcmp(a[0], "zflip"))
		{
			isotest_player_flip=atoi(a[1]);
			continue;
		}

		if(!strcmp(a[0], "spawnzflip"))
		{
			isotest_player_spawnflip=atoi(a[1]);
			continue;
		}

		if(!strcmp(a[0], "inven"))
		{
			b=atoi(a[1]);
			for(i=0; a[i+2]; i++)
			{
				j=frgl_ratoi(a[i+2]);
				Bt2Ent_SetInvenSlot(b+i, j);
			}
			continue;
		}

		if(!strcmp(a[0], "token"))
		{
			b=atoi(a[1]);
			for(i=0; a[i+2]; i++)
			{
				lj=frgl_ratoi(a[i+2]);
				Bt2Ent_SetTokenSlot(b+i, lj);
			}
			continue;
		}
	}

	frgl_free(buf);
	return(0);
}

int main_movetick(double dt)
{
	static double accdt=0;
	static double svdt=0;
	int rx, ry, rz;

	if(bgbdt_uipause)
		return(0);

	if(dt>0.5)
		return(-1);
	
	accdt+=dt;
	svdt+=dt;

	while(accdt>=(1.0/30))
	{
		main_movetick_step(1.0/30);
		accdt-=(1.0/30);
	}

	V3F_ADD(bgbdt_org, bgbdt_reforg, bgbdt_absorg);

	bgbdt_absorg[0]=
		((s32)((s64)(bgbdt_absorg[0]*BGBDT_XYZ_SCALE_FROMMETER)))*
			BGBDT_XYZ_SCALE_TOMETER;
	bgbdt_absorg[1]=
		((s32)((s64)(bgbdt_absorg[1]*BGBDT_XYZ_SCALE_FROMMETER)))*
			BGBDT_XYZ_SCALE_TOMETER;
	bgbdt_absorg[2]=
		((s32)((s64)(bgbdt_absorg[2]*BGBDT_XYZ_SCALE_FROMMETER)))*
			BGBDT_XYZ_SCALE_TOMETER;

#if 0
	if(bgbdt_absorg[0]>524287.25)
		bgbdt_absorg[0]-=1048576;
	if(bgbdt_absorg[1]>524287.25)
		bgbdt_absorg[1]-=1048576;
	if(bgbdt_absorg[2]>524287.25)
		bgbdt_absorg[2]-=1048576;

	if(bgbdt_absorg[0]<(-524288))
		bgbdt_absorg[0]+=1048576;
	if(bgbdt_absorg[1]<(-524288))
		bgbdt_absorg[1]+=1048576;
	if(bgbdt_absorg[2]<(-524288))
		bgbdt_absorg[2]+=1048576;
#endif

	rx=bgbdt_absorg[0]/256.0;
	ry=bgbdt_absorg[1]/256.0;
	rz=bgbdt_absorg[2]/256.0;
//	if(rx<0)rx++;
//	if(ry<0)ry++;
//	if(rz<0)rz++;

	bgbdt_reforg[0]=rx*256;
	bgbdt_reforg[1]=ry*256;
	bgbdt_reforg[2]=rz*256;

	V3F_SUB(bgbdt_absorg, bgbdt_reforg, bgbdt_org);

	if(V3F_LEN(bgbdt_spawnorg)<1)
	{
		V3F_COPY(bgbdt_absorg, bgbdt_spawnorg);
		V3F_COPY(bgbdt_ang, bgbdt_spawnang);
	}

	if(svdt>1)
	{
		main_saveplayer();
		svdt-=1;
	}
	
	return(0);
}


int main_prestart(int argc, char *argv[])
{
	char tb[256];
	int i, j, k;

	Con_Init();

	GfxDrv_SetDefaults("BGBTech2-1 Test0", 1024, 768);
	FRGL_CvarSetDefault("hide_os_cursor", "1");

	FRGL_CvarSetDefault("r_drawdist", "256");
	FRGL_CvarSetDefault("r_noshader", "0");
	FRGL_CvarSetDefault("r_novbo", "0");
	FRGL_CvarSetDefault("r_nosky", "0");

	FRGL_CvarSetDefault("gl_texfilter", "GL_LINEAR_MIPMAP_LINEAR");
	FRGL_CvarSetDefault("gl_driver", "opengl32");

	FRGL_CvarSetDefault("g_startworld", "maretst0");
	FRGL_CvarSetDefault("g_startwtype", "mare");

	Con_ExecCmdBuf("config.cfg");

	Con_DoExecCmds();

#ifdef __EMSCRIPTEN__
	for(i=0; i<10; i++)
	{
		sprintf(tb, "resource_ems/base%d.bpk", i);
		VfMount(tb, "/", "btpack", "");
	}
#else
	for(i=0; i<10; i++)
	{
		sprintf(tb, "resource/base%d.bpk", i);
		VfMount(tb, "/", "btpack", "");
	}
#endif

	return(0);
}

int main_concmd_tp(FRGL_ConsoleInfo *con, char **args)
{
	bgbdt_org[0]=atof(args[1]);
	bgbdt_org[1]=atof(args[2]);
	bgbdt_org[2]=atof(args[3]);
	
	bgbdt_vel[0]=0;
	bgbdt_vel[1]=0;
	bgbdt_vel[2]=0;
	
	bgbdt_reforg[0]=0;
	bgbdt_reforg[1]=0;
	bgbdt_reforg[2]=0;

	return(0);
}

int main_concmd_shaderflush(FRGL_ConsoleInfo *con, char **args)
{
	FRGL_TexMat_FlushShaders();
	return(0);
}

int main_concmd_antilight(FRGL_ConsoleInfo *con, char **args)
{
	BGBDT_VoxCoord xyz;

//	FRGL_TexMat_FlushShaders();
	xyz=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, bgbdt_org);
	BGBDT_VoxLight_ClearBlockLightRadius(
		bgbdt_voxworld, xyz, 0);
	BGBDT_VoxLight_ClearBlockAreaLightRadius(
		bgbdt_voxworld, xyz, 0);
	return(0);
}

int main_concmd_clearinven(FRGL_ConsoleInfo *con, char **args)
{
	Bt2Ent_ClearInven();
	Bt2Ent_ClearTokens();
//	FRGL_TexMat_FlushShaders();
	return(0);
}

int main_startup(int argc, char *argv[])
{
	FRGL_MenuInfo *menu;
	FRGL_MenuItem *mitm;
	BGBDT_SndListen *mixl, *mixr;
	BGBDT_VoxCoord xyz;
	char *s0, *s1;
	int cli, clj, clk, cll;
	int cr, cg, cb, ca;
	int x, y;
	int i, j, k;

	Tex_Init2();
	GfxFont_Init();
//	Con_Init();
//	ConCmds_Init();

	Con_ReInit();

	SoundDev_Init();

//	frgl_state->maxhz=60;
	frgl_state->maxhz=85;
	
	FRGL_RegisterConCmd("tp", NULL, main_concmd_tp);
	FRGL_RegisterConCmd("shaderflush", NULL, main_concmd_shaderflush);
	FRGL_RegisterConCmd("antilight", NULL, main_concmd_antilight);
	FRGL_RegisterConCmd("clearinven", NULL, main_concmd_clearinven);

	BSVM2_Interp_SetDisable(BSVM2_VMCTL_JITENABLE);

	Bt2Ent_LoadScript("bte3d.cfg", "bte3d.bsi");

	menu=FRGL_GetMenu("main");
	menu->title=frgl_strdup("Main Menu");

	mitm=FRGL_GetMenuItem(menu, 0, "newgame");
	mitm->title=frgl_strdup("New Game");
	mitm->Action=isotest_menopt_newgame;

	mitm=FRGL_GetMenuItem(menu, 1, "ldgame");
	mitm->title=frgl_strdup("Load Game");

	mitm=FRGL_GetMenuItem(menu, 2, "svgame");
	mitm->title=frgl_strdup("Save Game");

	mitm=FRGL_GetMenuItem(menu, 3, "options");
	mitm->title=frgl_strdup("Options");

	menu=FRGL_GetMenu("options");
	menu->title=frgl_strdup("Options");
	menu->back=frgl_strdup("main");

	mitm=FRGL_GetMenuItem(menu, 0, "noclip");
	mitm->title=frgl_strdup("No Clip");
	mitm->Action=isotest_menopt_toggle_act;

	mitm=FRGL_GetMenuItem(menu, 1, "undying");
	mitm->title=frgl_strdup("Undying");
	mitm->Action=isotest_menopt_toggle_act;

	mitm=FRGL_GetMenuItem(menu, 2, "all_cookies");
	mitm->title=frgl_strdup("All Cookies");
	mitm->Action=isotest_menopt_toggle_act;

	mitm=FRGL_GetMenuItem(menu, 3, "flip");
	mitm->title=frgl_strdup("Flip");
	mitm->Action=isotest_menopt_toggle_act;

	mixl=BGBDT_Sound_GetListener(0);
	mixr=BGBDT_Sound_GetListener(1);
	mixl->rate=44100;
	mixr->rate=44100;

//	bgbdt_voxworld=BGBDT_CreateBasicWorld("wrltest0");
//	bgbdt_voxworld=BGBDT_CreateBasicWorld2("maretst0", "mare");
	
	s0=FRGL_CvarGet("g_startworld");
	s1=FRGL_CvarGet("g_startwtype");
	bgbdt_voxworld=BGBDT_CreateBasicWorld2(s0, s1);
	
	Bt2Ent_BindVoxWorld(bgbdt_voxworld);

//#ifndef __EMSCRIPTEN__
#if 1
//	xyz.x=-4<<BGBDT_XYZ_SHR_CHUNK;
//	xyz.y=-4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.x=((u32)(~3))<<BGBDT_XYZ_SHR_CHUNK;
	xyz.y=((u32)(~3))<<BGBDT_XYZ_SHR_CHUNK;
	xyz.z= 4<<BGBDT_XYZ_SHR_CHUNK;
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz,
		BGBDT_ACCFL_ENNEWRGN|BGBDT_ACCFL_ENNEWCHK);
#endif

#if 0
	xyz.x= 4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.y=-4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.z= 4<<BGBDT_XYZ_SHR_CHUNK;
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz,
		BGBDT_ACCFL_ENNEWRGN|BGBDT_ACCFL_ENNEWCHK);

	xyz.x=-4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.y= 4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.z= 4<<BGBDT_XYZ_SHR_CHUNK;
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz,
		BGBDT_ACCFL_ENNEWRGN|BGBDT_ACCFL_ENNEWCHK);

	xyz.x= 4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.y= 4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.z= 4<<BGBDT_XYZ_SHR_CHUNK;
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz,
		BGBDT_ACCFL_ENNEWRGN|BGBDT_ACCFL_ENNEWCHK);
#endif

	voxui_placeidx=BGBDT_VoxelWorld_LookupTypeIndexName(
		bgbdt_voxworld, "stone");

	Bt2Ent_SpawnWorld(NULL);

	bgbdt_absorg[0]=0;	bgbdt_absorg[1]=0;	bgbdt_absorg[2]=0;
	bgbdt_reforg[0]=0;	bgbdt_reforg[1]=0;	bgbdt_reforg[2]=0;
	main_loadplayer();

	return(0);
}

int main_drawvoxui()
{
	static int frcnt=0;
	char tb[256];
	BGBDT_VoxTypeInfo *tyi;
	BGBDT_VoxData td, td1;
	BGBDT_VoxCoord xyz2;
	char *vs0;
	int vfl, vty, vfl1, vty1, vval;
	int x0, y0, x1, y1;
	float s0, s1, t0, t1;
	int ix, mat, setp;
	int i, j, k;

	if(voxui_isedit)
	{
		BGBDT_WorldGetVoxelData(bgbdt_voxworld, voxui_xyz, &td, NULL, 0);
		vfl=BGBDT_WorldVoxel_GetFlags(bgbdt_voxworld, voxui_xyz, td);
		vty=BGBDT_WorldVoxel_GetTypeID(bgbdt_voxworld, voxui_xyz, td);
		
		voxui_placeidx=vty;
	}

	frglDisableTexture2D();

	x0=-9*36;	y0=-9*36;
	x1=9*36;	y1=9*36;

	frglColor4f(0.5, 0.5, 0.5, 0.75);
	
//	FRGL_TexMat_BindBasic(mat);
	frglBegin(GL_QUADS);
//	frglTexCoord2f(s0, t0);
	frglVertex2f(x0, y0);
//	frglTexCoord2f(s1, t0);
	frglVertex2f(x1, y0);
//	frglTexCoord2f(s1, t1);
	frglVertex2f(x1, y1);
//	frglTexCoord2f(s0, t1);
	frglVertex2f(x0, y1);
	frglEnd();

	frglEnableTexture2D();
	
	voxui_voxui_idx=-1;
	
	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
	{
		ix=(15-i)*16+j;
		tyi=BGBDT_VoxelWorld_LookupTypeInfoIndex(bgbdt_voxworld, ix);
		
		if(tyi)
		{
//			mat=tyi->mat_top;
//			s0=tyi->mat_top_x/16.0; s1=s0+1.0/16;
//			t0=(15-tyi->mat_top_y)/16.0; t1=t0+1.0/16;

			mat=tyi->mat_ico;
			s0=tyi->mat_ico_x/16.0; s1=s0+1.0/16;
			t0=(15-tyi->mat_ico_y)/16.0; t1=t0+1.0/16;
		}else
		{
			mat=FRGL_TexMat_GetLoadIndex("textures/atlas0");
//			s0=4/16.0; s1=s0+1.0/16;
//			t0=(15-1)/16.0; t1=t0+1.0/16;

			s0=15/16.0; s1=s0+1.0/16;
			t0=(15-15)/16.0; t1=t0+1.0/16;

//			s0=0;	t0=0;
//			s1=1;	t1=1;
		}
		
		x0=(j-8)*36+ 0;		x1=(j-8)*36+32;
		y0=(i-8)*36+ 0;		y1=(i-8)*36+32;
		
		if(tyi && strcmp(tyi->name, "null") &&
				strcmp(tyi->name, "air") &&
			(frgl_state->mx>=x0) && (frgl_state->mx<=x1) &&
			((-frgl_state->my)>=y0) && ((-frgl_state->my)<=y1))
		{
			voxui_voxui_idx=ix;
		}
		
		if(voxui_voxui_idx==ix)
		{
			frglColor4f(0.75, 1, 0.75, 1);
		}else
		{
			frglColor4f(1, 1, 1, 1);
		}
		
		if(voxui_placeidx==ix)
		{
			frglColor4f(1, 0.75, 1, 1);
		}
		
		FRGL_TexMat_BindBasic(mat);
		frglBegin(GL_QUADS);
		frglTexCoord2f(s0, t0);
		frglVertex2f(x0, y0);
		frglTexCoord2f(s1, t0);
		frglVertex2f(x1, y0);
		frglTexCoord2f(s1, t1);
		frglVertex2f(x1, y1);
		frglTexCoord2f(s0, t1);
		frglVertex2f(x0, y1);
		frglEnd();
	}
	
	frcnt++;
	
	if(voxui_isedit)
	{
		BGBDT_WorldGetVoxelData(bgbdt_voxworld, voxui_xyz, &td, NULL, 0);
		vfl=BGBDT_WorldVoxel_GetFlags(bgbdt_voxworld, voxui_xyz, td);
		vty=BGBDT_WorldVoxel_GetTypeID(bgbdt_voxworld, voxui_xyz, td);
		vval=td.vdatal|(td.vdatah<<8);
		setp=0;

		if(FRGL_KeyDown(K_ENTER) && !FRGL_LastKeyDown(K_ENTER))
		{
			if(voxui_voxui_idx>=0)
			{
				voxui_placeidx=voxui_voxui_idx;
			}else if(strlen(voxui_editbuf)>0)
			{
				vs0=voxui_editbuf;
				if(*vs0=='#')
				{
					vval=frgl_ratoi(vs0+1);
					td.vdatal=vval;
					td.vdatah=vval>>8;
					setp=1;
				}else
				{
					vval=BGBDT_WorldGetString(
						bgbdt_voxworld, voxui_xyz, 0, voxui_editbuf);
					td.vdatal=vval;
					td.vdatah=vval>>8;
					setp=1;
				}
			}
		}

		if(voxui_placeidx!=vty)
		{
			td.vtypel=voxui_placeidx;
			td.vtypeh=voxui_placeidx>>8;
			setp=1;
		}

		if(setp)
		{
//			BGBDT_WorldSetVoxelData(bgbdt_voxworld,
//				voxui_xyz, td, BGBDT_ACCFL_CHKADJ);
			BGBDT_WorldSetVoxelData(bgbdt_voxworld,
				voxui_xyz, td, BGBDT_ACCFL_CHKADJ|BGBDT_ACCFL_NOLOADRGN);
		}

//		vs0=NULL;

		vs0=BGBDT_WorldGetStringForIndex(bgbdt_voxworld,
				voxui_xyz, 0, vval);

		if(!vs0)vs0="";

		x0=-32*8;
		y0=8*36+36;
		GfxFont_SetFont("fixed", 0);
		sprintf(tb, "Vty=%04X Vdat=%04X Vstr=%s", vty, vval, vs0);
		GfxFont_DrawString(tb, x0, y0, 8, 8, 0, 255, 0, 255);
		
		memset(tb, 0, 256);
		sprintf(tb, "> %s", voxui_editbuf);
		if(frcnt&1)
			tb[voxui_editpos+2]='|';
		GfxFont_DrawString(tb, x0, y0-8, 8, 8, 0, 255, 0, 255);
	}
	return(0);
}

int main_drawvoxhud()
{
	BGBDT_VoxTypeInfo *tyi;
	char *spr;
	int x0, y0, x1, y1;
	float s0, s1, t0, t1;
	int wxs, wys;
	int ix, mat;
	int i, j, k;
	
	GfxDrv_GetWindowSize(&wxs, &wys);

	FRGL_TexMat_BindBasic(0);
	frglDisable(GL_CULL_FACE);

	if(bgbdt_toolslot)
	{
//		bgbdt_toolspr[4]="images/ui/chain0";
//		spr=bgbdt_toolspr[bgbdt_toolslot];

		spr=Bt2Ent_GetToolSprite();
	
		if(spr)
		{
			mat=FRGL_TexMat_GetLoadIndex(spr);

			x0=-(wxs/4);	x1=x0+(wxs/2);
			y0=-(wys/2);	y1=y0+(wys/2);
			
			s0=0; s1=1;
			t0=0; t1=1;
			
			frglColor4f(1, 1, 1, 1);
			
			FRGL_TexMat_BindBasic(mat);
			frglBegin(GL_QUADS);
			frglTexCoord2f(s0, t0);
			frglVertex2f(x0, y0);
			frglTexCoord2f(s1, t0);
			frglVertex2f(x1, y0);
			frglTexCoord2f(s1, t1);
			frglVertex2f(x1, y1);
			frglTexCoord2f(s0, t1);
			frglVertex2f(x0, y1);
			frglEnd();
		}
	
		return(0);
	}
	
	if(1)
	{
		ix=voxui_placeidx;
		tyi=BGBDT_VoxelWorld_LookupTypeInfoIndex(bgbdt_voxworld, ix);
		
		if(tyi)
		{
//			mat=tyi->mat_top;
//			s0=tyi->mat_top_x/16.0; s1=s0+1.0/16;
//			t0=(15-tyi->mat_top_y)/16.0; t1=t0+1.0/16;

			mat=tyi->mat_ico;
			s0=tyi->mat_ico_x/16.0; s1=s0+1.0/16;
			t0=(15-tyi->mat_ico_y)/16.0; t1=t0+1.0/16;
		}else
		{
			mat=FRGL_TexMat_GetLoadIndex("textures/atlas0");
//			s0=4/16.0; s1=s0+1.0/16;
//			t0=(15-1)/16.0; t1=t0+1.0/16;

			s0=15/16.0; s1=s0+1.0/16;
			t0=(15-15)/16.0; t1=t0+1.0/16;

//			s0=0;	t0=0;
//			s1=1;	t1=1;
		}
		
//		x0=(j-8)*36+ 0;		x1=(j-8)*36+32;
//		y0=(i-8)*36+ 0;		y1=(i-8)*36+32;
		x0=0;			x1=x0+64;
		y0=-(wys/2);	y1=y0+64;
		
		frglColor4f(1, 1, 1, 1);
		
		FRGL_TexMat_BindBasic(mat);
		frglBegin(GL_QUADS);
		frglTexCoord2f(s0, t0);
		frglVertex2f(x0, y0);
		frglTexCoord2f(s1, t0);
		frglVertex2f(x1, y0);
		frglTexCoord2f(s1, t1);
		frglVertex2f(x1, y1);
		frglTexCoord2f(s0, t1);
		frglVertex2f(x0, y1);
		frglEnd();
	}
	return(0);
}

int main_setplacevox(BGBDT_VoxData *rtd)
{
	BGBDT_VoxData td;
	char *vtn;
	int vt_place;
	int vdir;

	vt_place=voxui_placeidx;

	memset(&td, 0, sizeof(BGBDT_VoxData));
	td.vtypel=vt_place;		td.vtypeh=vt_place>>8;
	td.vattr=0;
	td.vlight=0;		td.alight=0;
	*rtd=td;

	vdir=0;
//	if((bgbdt_ang[2]>=0) && (bgbdt_ang[2]<=45))
//		vdir=0;
	if((bgbdt_ang[2]>=45) && (bgbdt_ang[2]<=135))
		vdir=1;
	if((bgbdt_ang[2]>=135) && (bgbdt_ang[2]<=225))
		vdir=2;
	if((bgbdt_ang[2]>=225) && (bgbdt_ang[2]<=315))
		vdir=3;

//	BGBDT_WorldSetVoxelData(
//		bgbdt_voxworld, bgbdt_tslxyz, td, BGBDT_ACCFL_CHKADJ);

	vtn=BGBDT_WorldVoxel_GetTypeName(bgbdt_voxworld, bgbdt_tslxyz, td);
	
	if(!strcmp(vtn, "ent_mob"))
	{
		td.vattr=vdir;
		if(isotest_player_flip)
			td.vattr|=0x40;
	}

	if(!strcmp(vtn, "stairs"))
	{
		td.vattr=vdir;
		if(isotest_player_flip)
			td.vattr|=0x40;
	}

	*rtd=td;
	return(0);
}

int main_handle_input()
{
	static int lmlook=0;
	char buf[64];
	float mins[3], maxs[3];
	float io[3], iv[4], fw[2], pt[4], f, g, h, v;
	float sh, ch, sg, cg;
	float im[16], im1[16], *imv;
	int mvsp, imfl;

	BGBDT_VoxData td;
	int vt_air, vt_place;
	int mlook, fsg, hituse;

	int i, j, k;
	unsigned short *kcur;

	bgbdt_uipause=false;

	if(Con_IsDown())
	{
		bgbdt_uipause=true;
		return(0);
	}

	if(FRGL_MenuActiveP())
	{
		bgbdt_uipause=true;
		kcur=frgl_state->keys;
		FRGL_MenuHandleKeys(kcur);
		return(0);
	}

	if(Bt2Ent_DialogActiveP())
	{
		V3F_ZERO(bgbdt_imp);
		hituse=0;
		kcur=frgl_state->keys;
		while(*kcur)
		{
			switch(*kcur)
			{
			case K_DEL:
			case K_ESC:
//				isotest_diagbox=DTV_NULL;
				Bt2Ent_SetDialog(DTV_NULL);
				break;		
			case K_ENTER:
				hituse=1;
				break;
			case '0':	case '1':
			case '2':	case '3':
			case '4':	case '5':
			case '6':	case '7':
			case '8':	case '9':
				hituse=1+(*kcur-'0');
				break;
			default:
				break;
			}
			kcur++;
		}

		if(hituse>0)
		{
			Bt2Ent_DialogImpulse(hituse);
		}
		return(0);
	}

//	if(isotest_invopen || isotest_isdead)
	if(Bt2Ent_InvenActiveP())
	{
//		V3F_ZERO(bgbdt_imp);
		bgbdt_uipause=true;

		hituse=0;
		kcur=frgl_state->keys;

#if 0
		if(isotest_isdead && isotest_player_undying)
		{
			if(kcur && *kcur)
				isotest_isdead=0;
			if(FRGL_KeyDown(K_LEFTARROW) ||
				FRGL_KeyDown(K_RIGHTARROW) ||
				FRGL_KeyDown(K_UPARROW) ||
				FRGL_KeyDown(K_DOWNARROW))
					isotest_isdead=0;
		}
#endif

		while(*kcur)
		{
			switch(*kcur)
			{
			case K_ESC:
				if(Bt2Ent_InvenActiveP())
				{
					Bt2Ent_InvenSetActive(0);
					break;
				}
				FRGL_SetMenuActive("main");
				break;
			case K_DEL:
//			case K_ESC:
			case K_TAB:
				Bt2Ent_InvenSetActive(0);
				break;		
			case K_ENTER:
				hituse=1;
				break;

			case 'i':
				Bt2Ent_InvenSetActive(0);
				break;
			
			case '0':	case '1':
			case '2':	case '3':
			case '4':	case '5':
			case '6':	case '7':
			case '8':	case '9':
				hituse=1+(*kcur-'0');
				break;
			default:
				break;
			}
			kcur++;
		}
		return(0);
	}

	if(voxui_active)
	{
//		V3F_ZERO(bgbdt_imp);
		bgbdt_uipause=true;

		if((frgl_state->mb&1) && !(frgl_state->lmb&1))
		{
			if(voxui_voxui_idx>0)
				voxui_placeidx=voxui_voxui_idx;
		}

		if((frgl_state->mb&4) && !(frgl_state->lmb&4))
		{
			if(voxui_tsvidx>1)
				{ voxui_placeidx=voxui_tsvidx; }
		}

		kcur=frgl_state->keys;
		while(*kcur)
		{
			switch(*kcur)
			{
			case K_ENTER:
				if(voxui_voxui_idx>0)
					voxui_placeidx=voxui_voxui_idx;
				break;
			case K_TAB:
				voxui_active=!voxui_active;
				break;
			case K_DEL:
				memmove(
					voxui_editbuf+voxui_editpos,
					voxui_editbuf+voxui_editpos+1,
					254-voxui_editpos);
				break;
			case K_BACKSPACE:
				if(voxui_editpos>0)
				{
					memmove(
						voxui_editbuf+voxui_editpos-1,
						voxui_editbuf+voxui_editpos,
						254-voxui_editpos);
					voxui_editpos--;
				}
				break;
			case K_INS:
				break;
			case K_LEFTARROW:
				if(voxui_editpos>0)
					voxui_editpos--;
				break;
			case K_RIGHTARROW:
				if((voxui_editpos<252) &&
					(strlen(voxui_editbuf+voxui_editpos)>0))
						voxui_editpos++;
				break;
			default:
				break;
			}
			
			if((*kcur>=' ') && (*kcur<='~'))
			{
				memmove(
					voxui_editbuf+voxui_editpos+1,
					voxui_editbuf+voxui_editpos,
					254-voxui_editpos);
				voxui_editbuf[voxui_editpos]=*kcur;
				voxui_editpos++;
			}
			kcur++;
		}

		return(0);
	}

//	f=frgl_state->time_f;
	h=bgbdt_ang[2]*(M_PI/180);
	g=bgbdt_ang[0]*(M_PI/180);

//	bgbdt_rot[0]=cos(h);	bgbdt_rot[1]=sin(h);	bgbdt_rot[2]=0;
//	bgbdt_rot[3]=-sin(h);	bgbdt_rot[4]=cos(h);	bgbdt_rot[5]=0;
//	bgbdt_rot[6]=0;		bgbdt_rot[7]=0;		bgbdt_rot[8]=1;

//	sh=sin(h); ch=cos(h);
//	sg=sin(g); cg=cos(g);
//	bgbdt_rot[0]=ch;	bgbdt_rot[1]=sh*sg;	bgbdt_rot[2]=sh*sg;
//	bgbdt_rot[3]=-sh;	bgbdt_rot[4]=ch*cg;	bgbdt_rot[5]=ch*sh;
//	bgbdt_rot[6]=0;		bgbdt_rot[7]=-sg; 	bgbdt_rot[8]=cg;

	Mat3F_Identity(bgbdt_rot);
	Mat3F_Rotate3Matrix(bgbdt_rot, bgbdt_rot+6,
		bgbdt_ang[2]*(M_PI/180), bgbdt_rot);
	Mat3F_Rotate3Matrix(bgbdt_rot, bgbdt_rot+0,
		bgbdt_ang[0]*(M_PI/180), bgbdt_rot);
	Mat3F_Rotate3Matrix(bgbdt_rot,bgbdt_rot+3,
		bgbdt_ang[1]*(M_PI/180), bgbdt_rot);
    
	mlook=	FRGL_KeyDown(K_SHIFT) ||
			FRGL_KeyDown(K_NUMPAD5) ||
			FRGL_KeyDown('/') ||
			FRGL_KeyDown('?') ||
			(frgl_state->mb&2);
//    lmlook=	FRGL_LastKeyDown(K_SHIFT) ||
//			FRGL_LastKeyDown(K_NUMPAD5) ||
//			(frgl_state->lmb&2);
    
#ifndef __EMSCRIPTEN__
	if(!isotest_player_noclip)
	{
		mlook=1;
	}
#endif
    
    if(!GfxDrv_WindowIsActiveP())
		mlook=0;
    
	V3F_ZERO(iv);

	if(isotest_player_flip)
		{ fsg=-1; }
	else
		{ fsg=1; }

	mvsp=15;

// #ifdef __EMSCRIPTEN__
#if 0
	if(1)
	{
		mvsp=5;

		if(FRGL_KeyDown('a'))
			{ iv[0]-=mvsp*bgbdt_rot[0]; iv[1]-=mvsp*bgbdt_rot[1]; }
		if(FRGL_KeyDown('d'))
			{ iv[0]+=mvsp*bgbdt_rot[0]; iv[1]+=mvsp*bgbdt_rot[1]; }
		if(FRGL_KeyDown('w'))
			{ iv[0]+=mvsp*bgbdt_rot[3]; iv[1]+=mvsp*bgbdt_rot[4]; }
		if(FRGL_KeyDown('s'))
			{ iv[0]-=mvsp*bgbdt_rot[3]; iv[1]-=mvsp*bgbdt_rot[4]; }
		if(FRGL_KeyDown(' '))
			{ iv[2]+=3*fsg*mvsp; }

		if(FRGL_KeyDown('t'))
			{ iv[2]+=3*fsg*mvsp; }
		if(FRGL_KeyDown('g'))
			{ iv[2]-=3*fsg*mvsp; }

		if(FRGL_KeyDown('h'))
			{ bgbdt_ang[2]+=60*frgl_state->dt_f; }
		if(FRGL_KeyDown('k'))
			{ bgbdt_ang[2]-=60*frgl_state->dt_f; }
		if(FRGL_KeyDown('u'))
			{ bgbdt_ang[0]+=60*frgl_state->dt_f; }
		if(FRGL_KeyDown('j'))
			{ bgbdt_ang[0]-=60*frgl_state->dt_f; }

		if(FRGL_KeyDown('/') ||
			FRGL_KeyDown('?') ||
			(frgl_state->mb&2))
		{
			if(mlook)
			{
				if(lmlook)
				{
					h=frgl_state->dt_f;
					if(h>0.5)h=0;
					bgbdt_ang[0]-=fsg*frgl_state->my*30*h;
					bgbdt_ang[2]-=fsg*frgl_state->mx*30*h;

					if(bgbdt_ang[0]<(-89))
						bgbdt_ang[0]=-89;
					if(bgbdt_ang[0]>(89))
						bgbdt_ang[0]=89;
				}

				GfxDrv_MouseSetPos(0, 0);
			}
		}
	}

#else
	if(mlook)
	{
		if(lmlook)
		{
			h=frgl_state->dt_f;
			if(h>0.5)h=0;
//			bgbdt_ang[0]-=fsg*frgl_state->my*45*h;
//			bgbdt_ang[2]-=fsg*frgl_state->mx*45*h;
			bgbdt_ang[0]-=fsg*frgl_state->my*30*h;
			bgbdt_ang[2]-=fsg*frgl_state->mx*30*h;

			if(bgbdt_ang[0]<(-89))
				bgbdt_ang[0]=-89;
			if(bgbdt_ang[0]>(89))
				bgbdt_ang[0]=89;
		}

		GfxDrv_MouseSetPos(0, 0);

		if(!isotest_player_noclip)
		{
			if(FRGL_KeyDown(K_LEFTARROW))
				{ iv[0]-=mvsp*bgbdt_rot[0]; iv[1]-=mvsp*bgbdt_rot[1]; }
			if(FRGL_KeyDown(K_RIGHTARROW))
				{ iv[0]+=mvsp*bgbdt_rot[0]; iv[1]+=mvsp*bgbdt_rot[1]; }
			if(FRGL_KeyDown(K_UPARROW))
				{ iv[0]+=mvsp*bgbdt_rot[3]; iv[1]+=mvsp*bgbdt_rot[4]; }
			if(FRGL_KeyDown(K_DOWNARROW))
				{ iv[0]-=mvsp*bgbdt_rot[3]; iv[1]-=mvsp*bgbdt_rot[4]; }

			if(FRGL_KeyDown(K_HOME))
				{ iv[2]+=fsg*mvsp; }
			if(FRGL_KeyDown(K_END))
				{ iv[2]-=fsg*mvsp; }

			if(FRGL_KeyDown('a'))
				{ iv[0]-=mvsp*bgbdt_rot[0]; iv[1]-=mvsp*bgbdt_rot[1]; }
			if(FRGL_KeyDown('d'))
				{ iv[0]+=mvsp*bgbdt_rot[0]; iv[1]+=mvsp*bgbdt_rot[1]; }
			if(FRGL_KeyDown('w'))
				{ iv[0]+=mvsp*bgbdt_rot[3]; iv[1]+=mvsp*bgbdt_rot[4]; }
			if(FRGL_KeyDown('s'))
				{ iv[0]-=mvsp*bgbdt_rot[3]; iv[1]-=mvsp*bgbdt_rot[4]; }
			if(FRGL_KeyDown(' '))
				{ iv[2]+=fsg*mvsp; }

			if(FRGL_KeyDown('t'))
				{ iv[2]+=3*fsg*mvsp; }
			if(FRGL_KeyDown('g'))
				{ iv[2]-=3*fsg*mvsp; }

			if(FRGL_KeyDown('h'))
				{ bgbdt_ang[2]+=60*frgl_state->dt_f; }
			if(FRGL_KeyDown('k'))
				{ bgbdt_ang[2]-=60*frgl_state->dt_f; }
			if(FRGL_KeyDown('u'))
				{ bgbdt_ang[0]+=60*frgl_state->dt_f; }
			if(FRGL_KeyDown('j'))
				{ bgbdt_ang[0]-=60*frgl_state->dt_f; }

//			if(FRGL_KeyDown('e'))
//				{ iv[2]+=fsg*mvsp; }
//			if(FRGL_KeyDown('q'))
//				{ iv[2]-=fsg*mvsp; }
		}else
		{
			if(FRGL_KeyDown(K_LEFTARROW) || FRGL_KeyDown('a'))
			{
				iv[0]-=mvsp*bgbdt_rot[0];
				iv[1]-=mvsp*bgbdt_rot[1];
				iv[2]-=mvsp*bgbdt_rot[2];
			}

			if(FRGL_KeyDown(K_RIGHTARROW) || FRGL_KeyDown('d'))
			{
				iv[0]+=mvsp*bgbdt_rot[0];
				iv[1]+=mvsp*bgbdt_rot[1];
				iv[2]+=mvsp*bgbdt_rot[2];
			}

			if(FRGL_KeyDown(K_END) || FRGL_KeyDown('t'))
			{
				iv[0]-=mvsp*bgbdt_rot[6];
				iv[1]-=mvsp*bgbdt_rot[7];
				iv[2]-=mvsp*bgbdt_rot[8];
			}

			if(FRGL_KeyDown(K_HOME) || FRGL_KeyDown('g'))
			{
				iv[0]+=mvsp*bgbdt_rot[6];
				iv[1]+=mvsp*bgbdt_rot[7];
				iv[2]+=mvsp*bgbdt_rot[8];
			}
		}
	}else
	{
//		if(FRGL_KeyDown(K_LEFTARROW))
//			{ bgbdt_ang[2]+=135*frgl_state->dt_f; }
//		if(FRGL_KeyDown(K_RIGHTARROW))
//			{ bgbdt_ang[2]-=135*frgl_state->dt_f; }

		if(FRGL_KeyDown(K_LEFTARROW) || FRGL_KeyDown('a'))
		{
//			bgbdt_org[0]-=25*bgbdt_rot[0]*frgl_state->dt_f;
//			bgbdt_org[1]-=25*bgbdt_rot[1]*frgl_state->dt_f;
//			bgbdt_org[2]-=25*bgbdt_rot[2]*frgl_state->dt_f;

			iv[0]-=mvsp*bgbdt_rot[0];
			iv[1]-=mvsp*bgbdt_rot[1];
			iv[2]-=mvsp*bgbdt_rot[2];
		}

		if(FRGL_KeyDown(K_RIGHTARROW) || FRGL_KeyDown('d'))
		{
//			bgbdt_org[0]+=25*bgbdt_rot[0]*frgl_state->dt_f;
//			bgbdt_org[1]+=25*bgbdt_rot[1]*frgl_state->dt_f;
//			bgbdt_org[2]+=25*bgbdt_rot[2]*frgl_state->dt_f;

			iv[0]+=mvsp*bgbdt_rot[0];
			iv[1]+=mvsp*bgbdt_rot[1];
			iv[2]+=mvsp*bgbdt_rot[2];
		}

		if(FRGL_KeyDown(K_HOME) || FRGL_KeyDown('t'))
//			{ bgbdt_org[2]+=25*frgl_state->dt_f; }
			{ iv[2]+=fsg*mvsp; }
		if(FRGL_KeyDown(K_END) || FRGL_KeyDown('g'))
//			{ bgbdt_org[2]-=25*frgl_state->dt_f; }
			{ iv[2]-=fsg*mvsp; }
		if(FRGL_KeyDown(K_PGUP))
			{ bgbdt_ang[0]+=120*frgl_state->dt_f; }
		if(FRGL_KeyDown(K_PGDN))
			{ bgbdt_ang[0]-=120*frgl_state->dt_f; }
	}

	if(isotest_player_noclip || !mlook)
	{
		if(FRGL_KeyDown(K_UPARROW) || FRGL_KeyDown(K_NUMPAD7) ||
			FRGL_KeyDown('w'))
		{
			iv[0]+=mvsp*bgbdt_rot[3];
			iv[1]+=mvsp*bgbdt_rot[4];
			iv[2]+=mvsp*bgbdt_rot[5];
		}

		if(FRGL_KeyDown(K_DOWNARROW) || FRGL_KeyDown(K_NUMPAD9) ||
			FRGL_KeyDown('s'))
		{
			iv[0]-=mvsp*bgbdt_rot[3];
			iv[1]-=mvsp*bgbdt_rot[4];
			iv[2]-=mvsp*bgbdt_rot[5];
		}

		if(FRGL_KeyDown(K_NUMPAD4))
		{
			bgbdt_ang[2]+=120*frgl_state->dt_f;
		}

		if(FRGL_KeyDown(K_NUMPAD6))
		{
			bgbdt_ang[2]-=120*frgl_state->dt_f;
		}

		if(FRGL_KeyDown(K_NUMPAD2))
		{
			iv[0]-=mvsp*bgbdt_rot[6];
			iv[1]-=mvsp*bgbdt_rot[7];
			iv[2]-=mvsp*bgbdt_rot[8];
		}

		if(FRGL_KeyDown(K_NUMPAD8))
		{
			iv[0]+=mvsp*bgbdt_rot[6];
			iv[1]+=mvsp*bgbdt_rot[7];
			iv[2]+=mvsp*bgbdt_rot[8];
		}
	}
#endif

	if(isotest_player_flip)
	{
		if(bgbdt_ang[1]<180)
			bgbdt_ang[1]+=540*frgl_state->dt_f;
		if(bgbdt_ang[1]>180)
			bgbdt_ang[1]=180;
//		bgbdt_ang[1]=180;
	}else
	{
		if(bgbdt_ang[1]>0)
			bgbdt_ang[1]-=540*frgl_state->dt_f;
		if(bgbdt_ang[1]<0)
			bgbdt_ang[1]=0;
//		bgbdt_ang[1]=0;
	}

	f=Bt2Ent_EntGetHealth(bgbdt_voxworld->ent_player);
	if(f<=0)
	{
		if(!isotest_player_isdead)
			bgbdt_player_deadtime=frgl_state->time_f;
		isotest_player_isdead=1;
		V3F_ZERO(iv);
	}

	V3F_COPY(iv, bgbdt_imp);
	lmlook=mlook;

	vt_air=BGBDT_VoxelWorld_LookupTypeIndexName(bgbdt_voxworld, "air");
//	vt_place=BGBDT_VoxelWorld_LookupTypeIndexName(bgbdt_voxworld, "stone");
	vt_place=voxui_placeidx;

	imfl=0;

	if((bgbdt_toolslot==0) && !isotest_player_isdead)
	{
		if((frgl_state->mb&1) && !(frgl_state->lmb&1))
		{
	//		memset(&td, 0, sizeof(BGBDT_VoxData));
	//		td.vtypel=vt_place;		td.vtypeh=vt_place>>8;
	//		td.vattr=0;
	//		td.vlight=0;		td.alight=0;
			
			if((voxui_placeidx>1) && (voxui_tsvidx>0))
			{
				main_setplacevox(&td);
				BGBDT_WorldSetVoxelData(
					bgbdt_voxworld, bgbdt_tslxyz, td,
					BGBDT_ACCFL_CHKADJ|BGBDT_ACCFL_NOLOADRGN);
			}
		}

		if((frgl_state->mb&4) && !(frgl_state->lmb&4))
		{
			if(voxui_tsvidx>1)
				{ voxui_placeidx=voxui_tsvidx; }
		}
	}else
	{
//		imfl=0;
		if(frgl_state->mb&1)
			imfl|=1;
		if(frgl_state->mb&4)
			imfl|=2;

		if(FRGL_KeyDown(K_CTRL))
			imfl|=1;
	}

	if(isotest_player_isdead)
	{
		bgbdt_toolslot=0;
		Bt2Ent_ToolSet(bgbdt_toolslot);

		if(isotest_player_undying && imfl &&
			((bgbdt_player_deadtime+0.25)<frgl_state->time_f))
		{
			Bt2Ent_EntSetHealth(bgbdt_voxworld->ent_player, 10);
			isotest_player_isdead=false;
		}else if(imfl && ((bgbdt_player_deadtime+3.0)<frgl_state->time_f))
		{
			Bt2Ent_EntSetHealth(bgbdt_voxworld->ent_player, 100);
			V3F_COPY(bgbdt_spawnorg, bgbdt_org);
			V3F_COPY(bgbdt_spawnang, bgbdt_ang);
			isotest_player_isdead=false;
			isotest_player_flip=isotest_player_spawnflip;

			bgbdt_vel[0]=0;
			bgbdt_vel[1]=0;
			bgbdt_vel[2]=0;
			
			bgbdt_reforg[0]=0;
			bgbdt_reforg[1]=0;
			bgbdt_reforg[2]=0;
		}
	}

	kcur=frgl_state->keys;
	while(*kcur)
	{
		switch(*kcur)
		{
		case K_ESC:
			FRGL_SetMenuActive("main");
			break;
		case K_ENTER:
		case 'e':
			if(isotest_player_isdead)
				break;

			if(bgbdt_toolslot)
			{
				if(!dtvNullP(bgbdt_tent))
				{
					Bt2Ent_CallEntityUse(bgbdt_tent, bgbdt_player);
				}

				break;
			}
			
			if(voxui_active)
			{
				voxui_placeidx=voxui_voxui_idx;
			}else if(voxui_tsvidx>0)
			{
				voxui_xyz=bgbdt_tsxyz;
				voxui_active=1;
				voxui_isedit=1;
			}else if(!dtvNullP(bgbdt_tent))
			{
				Bt2Ent_CallEntityUse(bgbdt_tent, bgbdt_player);
			}
			break;

		case 'i':
			if(isotest_player_isdead)
				break;

			Bt2Ent_InvenSetActive(1);
			break;

		case K_TAB:
			if(isotest_player_isdead)
				break;

			voxui_active=!voxui_active;
			voxui_isedit=0;
			break;

		case K_DEL:
		case 'q':
			if(isotest_player_isdead)
				break;

			if(bgbdt_toolslot)
			{
				break;
			}

			if(voxui_tsvidx<=0)
				break;
		
			BGBDT_VoxLight_CheckClearBlockLightRadius(
				bgbdt_voxworld, bgbdt_tsxyz, 0);
		
			memset(&td, 0, sizeof(BGBDT_VoxData));
			td.vtypel=vt_air;	td.vtypeh=vt_air>>8;
			td.vattr=0;
			td.vlight=0;		td.alight=0;
			BGBDT_WorldSetVoxelData(
				bgbdt_voxworld, bgbdt_tsxyz, td,
				BGBDT_ACCFL_CHKADJ|BGBDT_ACCFL_NOLOADRGN);
			break;

		case K_INS:
		case 'r':
			if(isotest_player_isdead)
				break;

			if(bgbdt_toolslot)
			{
				break;
			}

			if((voxui_tsvidx<=0) || (voxui_placeidx<=0))
				break;

//			memset(&td, 0, sizeof(BGBDT_VoxData));
//			td.vtypel=vt_place;	td.vtypeh=vt_place>>8;
//			td.vattr=0;
//			td.vlight=0;		td.alight=0;
			main_setplacevox(&td);
			BGBDT_WorldSetVoxelData(
				bgbdt_voxworld, bgbdt_tslxyz, td,
				BGBDT_ACCFL_CHKADJ|BGBDT_ACCFL_NOLOADRGN);
			break;

		case '\\':		
		case 'f':
			if(isotest_player_isdead)
				break;

			if(bgbdt_toolslot)
			{
				break;
			}

			if((voxui_tsvidx<=0) || (voxui_placeidx<=0))
				break;

//			memset(&td, 0, sizeof(BGBDT_VoxData));
//			td.vtypel=vt_place;	td.vtypeh=vt_place>>8;
//			td.vattr=0;
//			td.vlight=0;		td.alight=0;
			main_setplacevox(&td);
			BGBDT_WorldSetVoxelData(
				bgbdt_voxworld, bgbdt_tsxyz, td,
				BGBDT_ACCFL_CHKADJ|BGBDT_ACCFL_NOLOADRGN);
			break;

		case '0':	case '1':
		case '2':	case '3':
		case '4':	case '5':
		case '6':	case '7':
		case '8':	case '9':
			if(isotest_player_isdead)
				break;

			bgbdt_toolslot=*kcur-'0';
			Bt2Ent_ToolSet(bgbdt_toolslot);
			break;

		default:
			break;
		}
		kcur++;
	}
	
	Bt2Ent_ToolUse(imfl);

	return(0);
}

int main_body()
{
	static float frm_dtf[256];
	static byte frm_rov;
	static float expose=1.0;

	char tb[1024];
	int txa[6];
	float org[3], rot[9], lorg[3], aclr[4];
	float tsorg[3], temin[3], temax[3];
	float tv0[3], tv1[3], tv2[3];

	BGBDT_VoxCoord xyzs, xyze, xyzt, xyztl, xyzte;
	BGBDT_VoxCoord emin, emax, xyz;
	BGBDT_VoxData td;
	BGBDT_VoxDataStatus tds;
	dtVal ent;

//	BTEWS_WireNode *ncur;
	char *s;
	u32 imp0, imp1;
	u32 act, mask, pin, pout;
	float cr, cg, cb;
	double x0, x1, y0, y1, z0, z1;
	double f, g, h;
	int t0, t1, t2, t3;
	int t00;
	int mx, my, mgx, mgy;
	int tex, txs, tys, wxs, wys;
	int i, j, k, l;

	t00=frgl_clock();

	mx=frgl_state->mx;
	my=-frgl_state->my;

	t0=frgl_clock();

	main_handle_input();

	Con_DoExecCmds();

	if(!bgbdt_uipause)
		Bt2Ent_ScriptUpdate(frgl_state->dt_f);

	bgbdt_voxworld->camorg[0]=bgbdt_org[0];
	bgbdt_voxworld->camorg[1]=bgbdt_org[1];
	bgbdt_voxworld->camorg[2]=bgbdt_org[2];
	bgbdt_voxworld->reforg[0]=bgbdt_reforg[0];
	bgbdt_voxworld->reforg[1]=bgbdt_reforg[1];
	bgbdt_voxworld->reforg[2]=bgbdt_reforg[2];

	for(i=0; i<9; i++)
		bgbdt_voxworld->camrot[i]=bgbdt_rot[i];

	BGBDT_Part_UpdateParticles(frgl_state->dt_f);

	t1=frgl_clock();
	t2=t1-t0;
	bgbdt_voxworld->dt_phytick=t2;


	t0=frgl_clock();

	BGBDT_Sound_SetListenOrg(vec3(
		bgbdt_org[0], bgbdt_org[1], bgbdt_org[2]));
	BGBDT_Sound_SetListenVecRt(vec3(
		bgbdt_rot[0], bgbdt_rot[1], bgbdt_rot[2]));
	BGBDT_Sound_SetListenVecFw(vec3(
		bgbdt_rot[3], bgbdt_rot[4], bgbdt_rot[5]));
	BGBDT_Sound_SetListenVecUp(vec3(
		bgbdt_rot[6], bgbdt_rot[7], bgbdt_rot[8]));

	BGBDT_Sound_MixTime(frgl_state->dt_f);

	t1=frgl_clock();
	t2=t1-t0;
	bgbdt_voxworld->dt_sound=t2;

	main_movetick(frgl_state->dt_f);

	bgbdt_voxworld->camorg[0]=bgbdt_org[0];
	bgbdt_voxworld->camorg[1]=bgbdt_org[1];
	bgbdt_voxworld->camorg[2]=bgbdt_org[2];
	bgbdt_voxworld->reforg[0]=bgbdt_reforg[0];
	bgbdt_voxworld->reforg[1]=bgbdt_reforg[1];
	bgbdt_voxworld->reforg[2]=bgbdt_reforg[2];

	GfxDrv_GetWindowSize(&wxs, &wys);

	h=(1.0*wxs)/wys;
//	f=(h*mx)/(wxs/2.0);
	f=(1.0*mx)/(wxs/2.0);
	g=((1/h)*my)/(wys/2.0);
	tv0[0]=bgbdt_rot[3]+(f*bgbdt_rot[0])+(g*bgbdt_rot[6]);
	tv0[1]=bgbdt_rot[4]+(f*bgbdt_rot[1])+(g*bgbdt_rot[7]);
	tv0[2]=bgbdt_rot[5]+(f*bgbdt_rot[2])+(g*bgbdt_rot[8]);

//	tv1[0]=bgbdt_org[0]+256*tv0[0];
//	tv1[1]=bgbdt_org[1]+256*tv0[1];
//	tv1[2]=bgbdt_org[2]+256*tv0[2];

//	tv1[0]=bgbdt_org[0]+16*tv0[0];
//	tv1[1]=bgbdt_org[1]+16*tv0[1];
//	tv1[2]=bgbdt_org[2]+16*tv0[2];

	tv1[0]=bgbdt_org[0]+24*tv0[0];
	tv1[1]=bgbdt_org[1]+24*tv0[1];
	tv1[2]=bgbdt_org[2]+24*tv0[2];

	xyzs=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, bgbdt_org);
	xyze=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, tv1);
	i=BGBDT_RayCastVoxel(bgbdt_voxworld, xyzs, xyze,
		&xyzt, &xyztl, &td, &tds,
		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);

	j=Bt2Ent_RayCastEntity(bgbdt_voxworld,
		bgbdt_voxworld->ent_player, xyzs, xyzt, &xyzte, &ent,
//	j=Bt2Ent_RayCastEntity(bgbdt_voxworld, xyzs, xyze, &xyzte, &ent,
		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);

	BGBDT_ConvVoxToLocalCoord(bgbdt_voxworld, xyzt, tsorg);

//	voxui_tsvidx=td.vtype;
	voxui_tsvidx=td.vtypel|(td.vtypeh<<8);
	bgbdt_tsxyz=xyzt;
	bgbdt_tslxyz=xyztl;
	V3F_COPY(tsorg, bgbdt_tsorg);

	if(i<=0)voxui_tsvidx=0;

	bgbdt_tent=DTV_NULL;
	if(j>0)
	{
		voxui_tsvidx=0;
		bgbdt_tent=ent;
		bgbdt_tsexyz=xyzte;

		Bt2Ent_GetBoxEnt(bgbdt_voxworld, ent, &emin, &emax);
		BGBDT_ConvVoxToLocalCoord(bgbdt_voxworld, emin, temin);
		BGBDT_ConvVoxToLocalCoord(bgbdt_voxworld, emax, temax);
	}
	
	Bt2Ent_SetToolTraceEnt(bgbdt_tent);

//	BGBDT_WorldGetVoxelData(bgbdt_voxworld, xyzs, &td, NULL, 0);
	BGBDT_WorldGetVoxelData(bgbdt_voxworld, xyzs, &td, NULL, BGBDT_TRFL_NOLOAD);
	i=BGBDT_VoxLight_GetBlockLightIntensity(bgbdt_voxworld, xyzs, td);
	
	f=(i+1)/16.0;	f=1.0/f;
//	f=f*1.12;
	f=f-0.25;
//	f=sqrt(f);
	f=pow(f, 0.3333);
//	if(f<0.65)f=0.65;
//	if(f>3.00)f=3.00;

	g=f-expose;
	if(fabs(g)>0.25)
	{
		f=expose+(g*(0.25/fabs(g)));
	}

	if(g>0)
	{
		expose=((1.0-frgl_state->dt_f*0.1)*expose)+
			(frgl_state->dt_f*0.1*f);
	}else
	{
		expose=((1.0-frgl_state->dt_f*0.3)*expose)+
			(frgl_state->dt_f*0.3*f);
	}

//	if(expose<0.65)expose=0.65;
	if(expose<0.50)expose=0.50;
	if(expose>3.00)expose=3.00;

	FRGL_TexMat_SetExposure(expose);

	if(td.alight && ((td.alight>>3)>4))
	{
		bgbdt_voxworld->insky=(td.alight&7);
	}else
	{
		bgbdt_voxworld->insky=0;
	}

	FRGL_Setup3D(bgbdt_org, bgbdt_rot);
//	FRGL_Setup3D(org, rot);

	frglDisable(GL_CULL_FACE);

	lorg[0]=0;
	lorg[1]=1024;
	lorg[2]=0;

//	main_DrawCubeSphere(256, txa, lorg);

	frglEnableTexture2D();
//	frglDisableTexture2D();
	frglEnable(GL_CULL_FACE);

	BGBDT_TickVoxWorld(bgbdt_voxworld);

// #ifndef __EMSCRIPTEN__
#if 1
	BGBDT_DrawVoxWorld(bgbdt_voxworld);

	Bt2Ent_DrawWorldEnts(bgbdt_voxworld);
	
	BGBDT_Part_DrawParticles();
#endif

	frglEnable(GL_CULL_FACE);


	frglDisableTexture2D();

#if 1
	if(voxui_tsvidx>0)
	{
		frglColor4f(255, 0, 0, 0.75);

		frglBegin(GL_LINES);
//		frglVertex3f(tsorg[0]-2, tsorg[1]  , tsorg[2]  );
//		frglVertex3f(tsorg[0]+2, tsorg[1]  , tsorg[2]  );
//		frglVertex3f(tsorg[0]  , tsorg[1]-2, tsorg[2]  );
//		frglVertex3f(tsorg[0]  , tsorg[1]+2, tsorg[2]  );
//		frglVertex3f(tsorg[0]  , tsorg[1]  , tsorg[2]-2);
//		frglVertex3f(tsorg[0]  , tsorg[1]  , tsorg[2]+2);

		frglVertex3f(tsorg[0]-0.1, tsorg[1]-0.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]-0.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]-0.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]-0.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]+1.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]+1.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]+1.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]+1.1, tsorg[2]+1.1);

		frglVertex3f(tsorg[0]-0.1, tsorg[1]-0.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]-0.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]-0.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]+1.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]+1.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]+1.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]+1.1, tsorg[2]-0.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]-0.1, tsorg[2]-0.1);

		frglVertex3f(tsorg[0]-0.1, tsorg[1]-0.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]-0.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]-0.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]+1.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]+1.1, tsorg[1]+1.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]+1.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]+1.1, tsorg[2]+1.1);
		frglVertex3f(tsorg[0]-0.1, tsorg[1]-0.1, tsorg[2]+1.1);

		frglEnd();
	}
#endif

#if 1
	if(!dtvNullP(bgbdt_tent))
	{
		frglColor4f(255, 0, 0, 0.75);

		frglBegin(GL_LINES);

		frglVertex3f(temin[0], temin[1], temin[2]);
		frglVertex3f(temin[0], temin[1], temax[2]);
		frglVertex3f(temax[0], temin[1], temin[2]);
		frglVertex3f(temax[0], temin[1], temax[2]);
		frglVertex3f(temin[0], temax[1], temin[2]);
		frglVertex3f(temin[0], temax[1], temax[2]);
		frglVertex3f(temax[0], temax[1], temin[2]);
		frglVertex3f(temax[0], temax[1], temax[2]);

		frglVertex3f(temin[0], temin[1], temin[2]);
		frglVertex3f(temax[0], temin[1], temin[2]);
		frglVertex3f(temax[0], temin[1], temin[2]);
		frglVertex3f(temax[0], temax[1], temin[2]);
		frglVertex3f(temax[0], temax[1], temin[2]);
		frglVertex3f(temin[0], temax[1], temin[2]);
		frglVertex3f(temin[0], temax[1], temin[2]);
		frglVertex3f(temin[0], temin[1], temin[2]);

		frglVertex3f(temin[0], temin[1], temax[2]);
		frglVertex3f(temax[0], temin[1], temax[2]);
		frglVertex3f(temax[0], temin[1], temax[2]);
		frglVertex3f(temax[0], temax[1], temax[2]);
		frglVertex3f(temax[0], temax[1], temax[2]);
		frglVertex3f(temin[0], temax[1], temax[2]);
		frglVertex3f(temin[0], temax[1], temax[2]);
		frglVertex3f(temin[0], temin[1], temax[2]);

		frglEnd();
	}
#endif

//	aclr[0]=0.25;	aclr[1]=0.75;
//	aclr[2]=1.00;	aclr[3]=0.33;
//	main_DrawColorSphere(264, aclr, lorg);

	FRGL_Setup2D();

//	f=-(btews_grid->xs*0.5)*32;
//	g=-(btews_grid->ys*0.5)*32;
//	BTEWS_DrawGrid(btews_grid, f, g, 32, 32);

	main_drawvoxhud();

	Bt2Ent_DrawDialog();

	if(voxui_active)
	{
		frglColor4f(1, 1, 1, 1);
		frglEnableTexture2D();

		main_drawvoxui();
	}

	FRGL_DrawMenu();

	frglColor4f(1.0, 1.0, 1.0, 1.0);
	frglEnableTexture2D();

	GfxFont_SetFont("fixed", 0);
	sprintf(tb, "Rel=(%.2f %.2f %.2f) Ang=(%.1f %.1f %.1f)",
		bgbdt_org[0], bgbdt_org[1], bgbdt_org[2],
		bgbdt_ang[0], bgbdt_ang[1], bgbdt_ang[2]);
	GfxFont_DrawString(tb, (wxs/2)-52*8, (wys/2)-1*8,
		8, 8, 0, 255, 0, 255);

	sprintf(tb, "Abs=(%.2f %.2f %.2f) Ref=(%.1f %.1f %.1f)",
		bgbdt_absorg[0], bgbdt_absorg[1], bgbdt_absorg[2],
		bgbdt_reforg[0], bgbdt_reforg[1], bgbdt_reforg[2]);
	GfxFont_DrawString(tb, (wxs/2)-52*8, (wys/2)-2*8,
		8, 8, 0, 255, 0, 255);

	xyz=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, bgbdt_org);
	i=bgbdt_xyz2rgnid(
		xyz.x>>BGBDT_XYZ_SHR_REGION,
		xyz.y>>BGBDT_XYZ_SHR_REGION,
		xyz.z>>BGBDT_XYZ_SHR_REGION);
	
	f=0.0;
	if(!dtvNullP(bgbdt_voxworld->ent_player))
		f=Bt2Ent_EntGetHealth(bgbdt_voxworld->ent_player);
	
	sprintf(tb, "Region=%08X Health=%.1f", i, f);
	GfxFont_DrawString(tb, (wxs/2)-52*8, (wys/2)-3*8,
		8, 8, 0, 255, 0, 255);

#if 0
	GfxFont_SetFont("dptext_16px", 0);
	sprintf(tb, "this is a test",
		bgbdt_org[0], bgbdt_org[1], bgbdt_org[2],
		bgbdt_ang[0], bgbdt_ang[1], bgbdt_ang[2]);
	GfxFont_DrawString(tb, (wxs/2)-32*16, (wys/2)-2*16,
		16, 16, 0, 255, 0, 255);
#endif

	f=frgl_state->dt_f;
	frm_dtf[(frm_rov++)&255]=f;
	
	g=0; x0=f; x1=f;
	for(i=0; i<256; i++)
	{
		f=frm_dtf[i];
		if(f<x0)x0=f;
		if(f>x1)x1=f;
		g+=f;
	}
	g=g/256.0;

	frglColor4f(1.0, 1.0, 1.0, 1.0);
	frglEnableTexture2D();

	sprintf(tb, "Cur=%.2fHz, Avg=%.2fHz, Min=%.2fHz, Max=%.2fHz "
			"Time=%d:%02d expose=%.3f",
		1.0/frgl_state->dt_f, 1.0/g, 1.0/x1, 1.0/x0,
		(((int)bgbdt_worldtime)/60),
		(((int)bgbdt_worldtime)%60),
		expose);
//	GfxFont_DrawString(tb, -480, 384-16, 8, 8, 0, 255, 0, 255);
	GfxFont_DrawString(tb, -(wxs/2), (wys/2)-16, 8, 8, 0, 255, 0, 255);

	sprintf(tb, "dt/ms ( phy=%2d tick=%2d pvs=%2d "
			"draw=%2d sound=%2d )->frame=%2d swap=%2d",
		bgbdt_voxworld->dt_phytick,
		bgbdt_voxworld->dt_tick,
		bgbdt_voxworld->dt_pvs,
		bgbdt_voxworld->dt_draw,
		bgbdt_voxworld->dt_sound,
		bgbdt_voxworld->dt_frame,
		GfxDrv_GetDtSwap());
//	GfxFont_DrawString(tb, -480, 384-24, 8, 8, 0, 255, 0, 255);
	GfxFont_DrawString(tb, -(wxs/2), (wys/2)-24, 8, 8, 0, 255, 0, 255);

	sprintf(tb, "Mesh gen-novis=%d/%d (%2.2f%%) "
			"cvs=%d (%d tris) pvs=%d (%d tris) raw=%d (%d tris)",
		bgbdt_voxworld->meshchk_nvis,
		bgbdt_voxworld->meshchk_tot,
		(100.0*bgbdt_voxworld->meshchk_nvis)/
			(bgbdt_voxworld->meshchk_tot+1.0),
		bgbdt_voxworld->vischk_tot,
		bgbdt_voxworld->vischk_tris,
		bgbdt_voxworld->pvschk_tot,
		bgbdt_voxworld->pvschk_tris,
		bgbdt_voxworld->rawchk_tot,
		bgbdt_voxworld->rawchk_tris);
//	GfxFont_DrawString(tb, -480, 384-16, 8, 8, 0, 255, 0, 255);
	GfxFont_DrawString(tb, -(wxs/2), (wys/2)-32, 8, 8, 0, 255, 0, 255);

	frglColor4f(1, 1, 1, 1);

	frglEnableTexture2D();

	Con_Render();

#if 0
	frglDisableTexture2D();

	frglColor4f(255, 0, 0, 0.75);

	frglBegin(GL_LINES);
	frglVertex2f(frgl_state->mx-10, -frgl_state->my);
	frglVertex2f(frgl_state->mx+10, -frgl_state->my);
	frglVertex2f(frgl_state->mx, -frgl_state->my-10);
	frglVertex2f(frgl_state->mx, -frgl_state->my+10);
	frglEnd();
#endif

	GfxFont_DrawString("X", frgl_state->mx-8, -frgl_state->my-8,
		16, 16, 255, 0, 0, 255);

	t1=frgl_clock();
	t2=t1-t00;
	bgbdt_voxworld->dt_frame=t2;

	return(0);
}

int main_shutdown()
{
	return(0);
}
