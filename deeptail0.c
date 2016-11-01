#include <bteifgl.h>
#include <bteifgl_main.h>

BGBDT_VoxWorld *bgbdt_voxworld;

float bgbdt_org[3];
float bgbdt_ang[3];
float bgbdt_rot[9];

float bgbdt_imp[3];
float bgbdt_vel[3];

float bgbdt_tsorg[3];			//trace local origin
BGBDT_VoxCoord bgbdt_tsxyz;		//trace ending xyz
BGBDT_VoxCoord bgbdt_tslxyz;	//trace ending last xyz
int voxui_active;
int voxui_placeidx;
int voxui_voxui_idx;
int voxui_tsvidx;

bool isotest_player_noclip=true;
bool isotest_player_undying=true;
bool isotest_player_onground=false;

double bgbdt_worldtime;

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

	if(!strcmp(mitm->name, "all_cookies"))
	{
//		IsoTest_GiveItem((64<<8)|4);
//		IsoTest_GiveItem((64<<8)|5);
	}
}

void isotest_menopt_newgame(FRGL_MenuInfo *menu, FRGL_MenuItem *mitm)
{
//	isotest_newgame_init();
}

int main_movetick_checkmove(float *porg)
{
	BGBDT_VoxCoord min, max;
	BGBDT_VoxCoord pos[16];
	BGBDT_VoxData td[16];
	float pomin[4], pomax[4];
	int nvox;
	int i;

	pomin[0]=porg[0]-0.45;
	pomin[1]=porg[1]-0.45;
	pomin[2]=porg[2]-1.30;

	pomax[0]=porg[0]+0.45;
	pomax[1]=porg[1]+0.45;
	pomax[2]=porg[2]+0.30;

	min=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, pomin);
	max=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, pomax);
	i=BGBDT_BoxQueryVoxel(bgbdt_voxworld, min, max, pos, td, &nvox,
		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);
	return(i);
}

int main_movetick_step(double dt)
{
	float porg[4], pvel[4];
	float f, g;
	int i;

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

	bgbdt_vel[2]-=9.8*dt;
	V3F_ADDSCALE(bgbdt_org, bgbdt_vel, dt, porg);

//	xyzs=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, bgbdt_org);
//	xyze=BGBDT_ConvLocalToVoxCoord(bgbdt_voxworld, tv1);
//	i=BGBDT_RayCastVoxel(bgbdt_voxworld, xyzs, xyze,
//		&xyzt, &xyztl, &td, &tds,
//		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);

//	BGBDT_ConvVoxToLocalCoord(bgbdt_voxworld, xyzt, tsorg);

	isotest_player_onground=false;
	i=main_movetick_checkmove(porg);
	if(i>0)
	{
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
			f=1-8*dt;
			if(f<0)f=0;
			pvel[0]*=f;
			pvel[1]*=f;
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
	}else if(!i)
	{
		V3F_COPY(porg, bgbdt_org);
	}
	
	if(isotest_player_onground)
	{
//		bgbdt_vel[0]=0;
//		bgbdt_vel[1]=0;
//		bgbdt_vel[2]=0;

//		bgbdt_vel[0]+=bgbdt_imp[0]*dt*12;
//		bgbdt_vel[1]+=bgbdt_imp[1]*dt*12;
		bgbdt_vel[0]+=bgbdt_imp[0]*0.5;
		bgbdt_vel[1]+=bgbdt_imp[1]*0.5;
		
		f=V2F_LEN(bgbdt_vel);
		if(f>10)
		{
			g=10/f;
			bgbdt_vel[0]*=g;
			bgbdt_vel[1]*=g;
		}
		
		if(bgbdt_imp[2]>7)
			bgbdt_vel[2]+=7;
	}else
	{
		bgbdt_vel[0]+=bgbdt_imp[0]*dt*0.25;
		bgbdt_vel[1]+=bgbdt_imp[1]*dt*0.25;
		bgbdt_vel[2]+=bgbdt_imp[2]*dt*0.25;
//		V3F_COPY(porg, bgbdt_org);
	}

	return(0);
}

int main_saveplayer()
{
	char tb[256];
	BGBDT_VoxCoord xyz, org;
	VFILE *fd;
	int fl;

#if 1
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

	fd=vffopen("region/player.ini", "wt");
	if(!fd)return(-1);
	
	sprintf(tb, "origin %f %f %f\n",
		bgbdt_org[0], bgbdt_org[1], bgbdt_org[2]);
	vfwrite(tb, 1, strlen(tb), fd);
	sprintf(tb, "angles %f %f %f\n",
		bgbdt_ang[0], bgbdt_ang[1], bgbdt_ang[2]);
	vfwrite(tb, 1, strlen(tb), fd);

	sprintf(tb, "noclip %d\n",
		isotest_player_noclip);
	vfwrite(tb, 1, strlen(tb), fd);

	vfclose(fd);
	return(0);
}

int main_loadplayer()
{
//	char tb[256];
//	VFILE *fd;
	char *buf, *cs, *cse;
	char *s;
	char **a;
	int sz;

//	fd=vffopen("region/player.ini", "wt");
//	if(!fd)return(-1);
	
	buf=vf_loadfile("region/player.ini", &sz);
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

		if(!strcmp(a[0], "noclip"))
		{
			isotest_player_noclip=atoi(a[1]);
		}
	}

	frgl_free(buf);
}

int main_movetick(double dt)
{
	static double accdt=0;
	static double svdt=0;
	
	if(dt>0.5)
		return(-1);
	
	accdt+=dt;
	svdt+=dt;

	while(accdt>=(1.0/30))
	{
		main_movetick_step(1.0/30);
		accdt-=(1.0/30);
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

	GfxDrv_SetDefaults("BGBTech2-1 Test0", 1024, 768);
	FRGL_CvarSetDefault("hide_os_cursor", "1");

	Con_ExecCmdBuf("config.cfg");

	for(i=0; i<10; i++)
	{
		sprintf(tb, "resource/base%d.bpk", i);
		VfMount(tb, "/", "btpack", "");
	}

	return(0);
}

int main_startup(int argc, char *argv[])
{
	FRGL_MenuInfo *menu;
	FRGL_MenuItem *mitm;
	BGBDT_SndListen *mixl, *mixr;
	BGBDT_VoxCoord xyz;
	int cli, clj, clk, cll;
	int cr, cg, cb, ca;
	int x, y;
	int i, j, k;

	Tex_Init2();
	GfxFont_Init();
	Con_Init();
//	ConCmds_Init();

	SoundDev_Init();

	frgl_state->maxhz=60;

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

	mixl=BGBDT_Sound_GetListener(0);
	mixr=BGBDT_Sound_GetListener(1);
	mixl->rate=44100;
	mixr->rate=44100;

	bgbdt_voxworld=BGBDT_CreateBasicWorld("wrltest0");

	xyz.x=-4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.y=-4<<BGBDT_XYZ_SHR_CHUNK;
	xyz.z= 4<<BGBDT_XYZ_SHR_CHUNK;
	BGBDT_WorldGetChunk(bgbdt_voxworld, xyz,
		BGBDT_ACCFL_ENNEWRGN|BGBDT_ACCFL_ENNEWCHK);

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

	main_loadplayer();

	return(0);
}

int main_drawvoxui()
{
	BGBDT_VoxTypeInfo *tyi;
	int x0, y0, x1, y1;
	float s0, s1, t0, t1;
	int ix, mat;
	int i, j, k;

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
	
	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
	{
		ix=(15-i)*16+j;
		tyi=BGBDT_VoxelWorld_LookupTypeInfoIndex(bgbdt_voxworld, ix);
		
		if(tyi)
		{
			mat=tyi->mat_top;
			s0=tyi->mat_top_x/16.0; s1=s0+1.0/16;
			t0=(15-tyi->mat_top_y)/16.0; t1=t0+1.0/16;
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
}

int main_drawvoxhud()
{
	BGBDT_VoxTypeInfo *tyi;
	int x0, y0, x1, y1;
	float s0, s1, t0, t1;
	int wxs, wys;
	int ix, mat;
	int i, j, k;
	
	GfxDrv_GetWindowSize(&wxs, &wys);

	if(1)
	{
		ix=voxui_placeidx;
		tyi=BGBDT_VoxelWorld_LookupTypeInfoIndex(bgbdt_voxworld, ix);
		
		if(tyi)
		{
			mat=tyi->mat_top;
			s0=tyi->mat_top_x/16.0; s1=s0+1.0/16;
			t0=(15-tyi->mat_top_y)/16.0; t1=t0+1.0/16;
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
}

int main_handle_input()
{
	static int lmlook=0;
	char buf[64];
	float mins[3], maxs[3];
	float io[3], iv[4], fw[2], pt[4], f, g, h, v;
	float sh, ch, sg, cg;
	float im[16], im1[16], *imv;

	BGBDT_VoxData td;
	int vt_air, vt_place;
	int mlook;

	int i, j, k;
	unsigned short *kcur;

	if(Con_IsDown())
		return(0);

	if(FRGL_MenuActiveP())
	{
		kcur=frgl_state->keys;
		FRGL_MenuHandleKeys(kcur);
		return(0);
	}

	if(voxui_active)
	{
		if((frgl_state->mb&1) && !(frgl_state->lmb&1))
		{
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
				voxui_placeidx=voxui_voxui_idx;
				break;
			case K_TAB:
				voxui_active=!voxui_active;
				break;
			case K_DEL:
				break;
			case K_INS:
				break;
			default:
				break;
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
			(frgl_state->mb&2);
//    lmlook=	FRGL_LastKeyDown(K_SHIFT) ||
//			FRGL_LastKeyDown(K_NUMPAD5) ||
//			(frgl_state->lmb&2);
    
    if(!isotest_player_noclip)
    {
		mlook=1;
    }
    
    V3F_ZERO(iv);
    
	if(mlook)
	{
		if(lmlook)
		{
			bgbdt_ang[0]-=frgl_state->my*45*frgl_state->dt_f;
			bgbdt_ang[2]-=frgl_state->mx*45*frgl_state->dt_f;

			if(bgbdt_ang[0]<(-89))
				bgbdt_ang[0]=-89;
			if(bgbdt_ang[0]>(89))
				bgbdt_ang[0]=89;
		}

		GfxDrv_MouseSetPos(0, 0);

		if(!isotest_player_noclip)
		{
			if(FRGL_KeyDown(K_LEFTARROW))
				{ iv[0]-=25*bgbdt_rot[0]; iv[1]-=25*bgbdt_rot[1]; }
			if(FRGL_KeyDown(K_RIGHTARROW))
				{ iv[0]+=25*bgbdt_rot[0]; iv[1]+=25*bgbdt_rot[1]; }
			if(FRGL_KeyDown(K_UPARROW))
				{ iv[0]+=25*bgbdt_rot[3]; iv[1]+=25*bgbdt_rot[4]; }
			if(FRGL_KeyDown(K_DOWNARROW))
				{ iv[0]-=25*bgbdt_rot[3]; iv[1]-=25*bgbdt_rot[4]; }

			if(FRGL_KeyDown(K_HOME))
				{ iv[2]+=25; }
			if(FRGL_KeyDown(K_END))
				{ iv[2]-=25; }
		}else
		{
			if(FRGL_KeyDown(K_LEFTARROW))
			{
				iv[0]-=25*bgbdt_rot[0];
				iv[1]-=25*bgbdt_rot[1];
				iv[2]-=25*bgbdt_rot[2];
			}

			if(FRGL_KeyDown(K_RIGHTARROW))
			{
				iv[0]+=25*bgbdt_rot[0];
				iv[1]+=25*bgbdt_rot[1];
				iv[2]+=25*bgbdt_rot[2];
			}

			if(FRGL_KeyDown(K_END))
			{
				iv[0]-=25*bgbdt_rot[6];
				iv[1]-=25*bgbdt_rot[7];
				iv[2]-=25*bgbdt_rot[8];
			}

			if(FRGL_KeyDown(K_HOME))
			{
				iv[0]+=25*bgbdt_rot[6];
				iv[1]+=25*bgbdt_rot[7];
				iv[2]+=25*bgbdt_rot[8];
			}
		}
	}else
	{
//		if(FRGL_KeyDown(K_LEFTARROW))
//			{ bgbdt_ang[2]+=135*frgl_state->dt_f; }
//		if(FRGL_KeyDown(K_RIGHTARROW))
//			{ bgbdt_ang[2]-=135*frgl_state->dt_f; }

		if(FRGL_KeyDown(K_LEFTARROW))
		{
//			bgbdt_org[0]-=25*bgbdt_rot[0]*frgl_state->dt_f;
//			bgbdt_org[1]-=25*bgbdt_rot[1]*frgl_state->dt_f;
//			bgbdt_org[2]-=25*bgbdt_rot[2]*frgl_state->dt_f;

			iv[0]-=25*bgbdt_rot[0];
			iv[1]-=25*bgbdt_rot[1];
			iv[2]-=25*bgbdt_rot[2];
		}

		if(FRGL_KeyDown(K_RIGHTARROW))
		{
//			bgbdt_org[0]+=25*bgbdt_rot[0]*frgl_state->dt_f;
//			bgbdt_org[1]+=25*bgbdt_rot[1]*frgl_state->dt_f;
//			bgbdt_org[2]+=25*bgbdt_rot[2]*frgl_state->dt_f;

			iv[0]+=25*bgbdt_rot[0];
			iv[1]+=25*bgbdt_rot[1];
			iv[2]+=25*bgbdt_rot[2];
		}

		if(FRGL_KeyDown(K_HOME))
//			{ bgbdt_org[2]+=25*frgl_state->dt_f; }
			{ iv[2]+=25; }
		if(FRGL_KeyDown(K_END))
//			{ bgbdt_org[2]-=25*frgl_state->dt_f; }
			{ iv[2]-=25; }
		if(FRGL_KeyDown(K_PGUP))
			{ bgbdt_ang[0]+=135*frgl_state->dt_f; }
		if(FRGL_KeyDown(K_PGDN))
			{ bgbdt_ang[0]-=135*frgl_state->dt_f; }
	}

	if(isotest_player_noclip)
	{
		if(FRGL_KeyDown(K_UPARROW) || FRGL_KeyDown(K_NUMPAD7))
		{
			iv[0]+=25*bgbdt_rot[3];
			iv[1]+=25*bgbdt_rot[4];
			iv[2]+=25*bgbdt_rot[5];
		}

		if(FRGL_KeyDown(K_DOWNARROW) || FRGL_KeyDown(K_NUMPAD9))
		{
			iv[0]-=25*bgbdt_rot[3];
			iv[1]-=25*bgbdt_rot[4];
			iv[2]-=25*bgbdt_rot[5];
		}

		if(FRGL_KeyDown(K_NUMPAD4))
		{
			bgbdt_ang[2]+=135*frgl_state->dt_f;
		}

		if(FRGL_KeyDown(K_NUMPAD6))
		{
			bgbdt_ang[2]-=135*frgl_state->dt_f;
		}

		if(FRGL_KeyDown(K_NUMPAD2))
		{
			iv[0]-=25*bgbdt_rot[6];
			iv[1]-=25*bgbdt_rot[7];
			iv[2]-=25*bgbdt_rot[8];
		}

		if(FRGL_KeyDown(K_NUMPAD8))
		{
			iv[0]+=25*bgbdt_rot[6];
			iv[1]+=25*bgbdt_rot[7];
			iv[2]+=25*bgbdt_rot[8];
		}
	}

	V3F_COPY(iv, bgbdt_imp);
	lmlook=mlook;

	vt_air=BGBDT_VoxelWorld_LookupTypeIndexName(bgbdt_voxworld, "air");
//	vt_place=BGBDT_VoxelWorld_LookupTypeIndexName(bgbdt_voxworld, "stone");
	vt_place=voxui_placeidx;

	if((frgl_state->mb&1) && !(frgl_state->lmb&1))
	{
		memset(&td, 0, sizeof(BGBDT_VoxData));
		td.vtypel=vt_place;		td.vtypeh=vt_place>>8;
		td.vattr=0;
		td.vlight=0;		td.alight=0;
		BGBDT_WorldSetVoxelData(
			bgbdt_voxworld, bgbdt_tslxyz, td, BGBDT_ACCFL_CHKADJ);
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
		case K_ESC:
			FRGL_SetMenuActive("main");
			break;
		case K_ENTER:
			if(voxui_active)
			{
				voxui_placeidx=voxui_voxui_idx;
			}
			break;
		case K_TAB:
			voxui_active=!voxui_active;
			break;
		case K_DEL:
			BGBDT_VoxLight_CheckClearBlockLightRadius(
				bgbdt_voxworld, bgbdt_tsxyz, 0);
		
			memset(&td, 0, sizeof(BGBDT_VoxData));
			td.vtypel=vt_air;	td.vtypeh=vt_air>>8;
			td.vattr=0;
			td.vlight=0;		td.alight=0;
			BGBDT_WorldSetVoxelData(
				bgbdt_voxworld, bgbdt_tsxyz, td, BGBDT_ACCFL_CHKADJ);
			break;
		case K_INS:
			memset(&td, 0, sizeof(BGBDT_VoxData));
			td.vtypel=vt_place;	td.vtypeh=vt_place>>8;
			td.vattr=0;
			td.vlight=0;		td.alight=0;
			BGBDT_WorldSetVoxelData(
				bgbdt_voxworld, bgbdt_tslxyz, td, BGBDT_ACCFL_CHKADJ);
			break;
		default:
			break;
		}
		kcur++;
	}

	return(0);
}

int main_body()
{
	static float frm_dtf[256];
	static byte frm_rov;

	char tb[1024];
	int txa[6];
	float org[3], rot[9], lorg[3], aclr[4];
	float tsorg[3];
	float tv0[3], tv1[3], tv2[3];

	BGBDT_VoxCoord xyzs, xyze, xyzt, xyztl;
	BGBDT_VoxData td;
	BGBDT_VoxDataStatus tds;

//	BTEWS_WireNode *ncur;
	char *s;
	u32 imp0, imp1;
	u32 act, mask, pin, pout;
	float cr, cg, cb;
	double x0, x1, y0, y1, z0, z1;
	double f, g, h;
	int mx, my, mgx, mgy;
	int tex, txs, tys, wxs, wys;
	int i, j, k, l;

	mx=frgl_state->mx;
	my=-frgl_state->my;

	main_handle_input();

#if 0
	org[0]=0;	org[1]=-1024;	org[2]=0;
//	org[0]=0;	org[1]=0;	org[2]=0;

	f=frgl_state->time_f;
//	h=f+(M_PI/2);
	h=f;
	rot[0]=cos(h);	rot[1]=sin(h);	rot[2]=0;
	rot[3]=-sin(h);	rot[4]=cos(h);	rot[5]=0;
	rot[6]=0;		rot[7]=0;		rot[8]=1;

//	g=512;
//	org[0]=sin(f)*g;	org[1]=-cos(f)*g;	org[2]=0;

	x0=0; y0=-512; z0=0;

	x0=256; y0=-256; z0=0;
#endif

#if 0
//	x0=256; y0=-64; z0=0;
	org[0]=rot[0]*x0+rot[3]*y0+rot[6]*z0;
	org[1]=rot[1]*x0+rot[4]*y0+rot[7]*z0;
	org[2]=rot[2]*x0+rot[5]*y0+rot[8]*z0;
#endif

	bgbdt_voxworld->camorg[0]=bgbdt_org[0];
	bgbdt_voxworld->camorg[1]=bgbdt_org[1];
	bgbdt_voxworld->camorg[2]=bgbdt_org[2];

	for(i=0; i<9; i++)
		bgbdt_voxworld->camrot[i]=bgbdt_rot[i];


	BGBDT_Sound_SetListenOrg(vec3(
		bgbdt_org[0], bgbdt_org[1], 0));
	BGBDT_Sound_SetListenVecRt(vec3(
		bgbdt_rot[0], bgbdt_rot[1], bgbdt_rot[2]));
	BGBDT_Sound_SetListenVecFw(vec3(
		bgbdt_rot[3], bgbdt_rot[4], bgbdt_rot[5]));
	BGBDT_Sound_SetListenVecUp(vec3(
		bgbdt_rot[6], bgbdt_rot[7], bgbdt_rot[8]));

	BGBDT_Sound_MixTime(frgl_state->dt_f);

	main_movetick(frgl_state->dt_f);

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

	BGBDT_ConvVoxToLocalCoord(bgbdt_voxworld, xyzt, tsorg);

//	voxui_tsvidx=td.vtype;
	voxui_tsvidx=td.vtypel|(td.vtypeh<<8);
	bgbdt_tsxyz=xyzt;
	bgbdt_tslxyz=xyztl;
	V3F_COPY(tsorg, bgbdt_tsorg);

	if(i<=0)voxui_tsvidx=0;

	FRGL_Setup3D(bgbdt_org, bgbdt_rot);
//	FRGL_Setup3D(org, rot);

	glDisable(GL_CULL_FACE);

	lorg[0]=0;
	lorg[1]=1024;
	lorg[2]=0;

//	main_DrawCubeSphere(256, txa, lorg);

	frglEnableTexture2D();
//	frglDisableTexture2D();
	glEnable(GL_CULL_FACE);

	BGBDT_TickVoxWorld(bgbdt_voxworld);

	BGBDT_DrawVoxWorld(bgbdt_voxworld);

	glEnable(GL_CULL_FACE);


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

//	aclr[0]=0.25;	aclr[1]=0.75;
//	aclr[2]=1.00;	aclr[3]=0.33;
//	main_DrawColorSphere(264, aclr, lorg);

	FRGL_Setup2D();

//	f=-(btews_grid->xs*0.5)*32;
//	g=-(btews_grid->ys*0.5)*32;
//	BTEWS_DrawGrid(btews_grid, f, g, 32, 32);

	main_drawvoxhud();

	if(voxui_active)
	{
		frglColor4f(1, 1, 1, 1);
		frglEnableTexture2D();

		main_drawvoxui();
	}

	FRGL_DrawMenu();

	frglColor4f(1, 1, 1, 1);
	frglEnableTexture2D();

	GfxFont_SetFont("fixed", 0);
	sprintf(tb, "(%.2f %.2f %.2f) (%.2f %.2f %.2f)",
		bgbdt_org[0], bgbdt_org[1], bgbdt_org[2],
		bgbdt_ang[0], bgbdt_ang[1], bgbdt_ang[2]);
	GfxFont_DrawString(tb, (wxs/2)-48*8, (wys/2)-1*8,
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
	frm_dtf[frm_rov++]=f;
	
	g=0; x0=f; x1=f;
	for(i=0; i<256; i++)
	{
		f=frm_dtf[i];
		if(f<x0)x0=f;
		if(f>x1)x1=f;
		g+=f;
	}
	g=g/256.0;

	sprintf(tb, "Cur=%.2fHz, Avg=%.2fHz, Min=%.2fHz, Max=%.2fHz "
			"Time=%d:%02d",
		1.0/frgl_state->dt_f, 1.0/g, 1.0/x1, 1.0/x0,
		(((int)bgbdt_worldtime)/60),
		(((int)bgbdt_worldtime)%60));
	GfxFont_DrawString(tb, -320, 384-16, 8, 8, 0, 255, 0, 255);

	sprintf(tb, "dtTick=%dms dtPvs=%dms dtDraw=%dms",
		bgbdt_voxworld->dt_tick,
		bgbdt_voxworld->dt_pvs,
		bgbdt_voxworld->dt_draw);
	GfxFont_DrawString(tb, -320, 384-24, 8, 8, 0, 255, 0, 255);

	frglColor4f(1, 1, 1, 1);

	frglEnableTexture2D();

	Con_Render();

	frglDisableTexture2D();

#if 1
	frglColor4f(255, 0, 0, 0.75);

	frglBegin(GL_LINES);
	frglVertex2f(frgl_state->mx-10, -frgl_state->my);
	frglVertex2f(frgl_state->mx+10, -frgl_state->my);
	frglVertex2f(frgl_state->mx, -frgl_state->my-10);
	frglVertex2f(frgl_state->mx, -frgl_state->my+10);
	frglEnd();
#endif

	return(0);
}

int main_shutdown()
{
	return(0);
}
