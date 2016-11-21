/*
PlatTest will be an attempt at a simple platformer.
*/

#include <bteifgl.h>
#include <bteifgl_main.h>

// BTEWS_WireGrid *btews_grid=NULL;
// BTEWS_Component *btews_component=NULL;

BSVM2_CodeImage *isotest_img;
BSVM2_ImageGlobal *isotest_vi_update;
BSVM2_ImageGlobal *isotest_vi_spawnworld;

dtcClass isotest_eb2d_cls;
dtcField isotest_eb2d_fi_origin;
dtcField isotest_eb2d_fi_velocity;
dtcField isotest_eb2d_fi_angle;
dtcField isotest_eb2d_fi_radius;
dtcField isotest_eb2d_fi_sprite;
dtcField isotest_eb2d_fi_sprite_sz;

dtcField isotest_eb2d_fi_spawn;
dtcField isotest_eb2d_fi_touch;
dtcField isotest_eb2d_fi_use;

dtcClass isotest_npcdiag_cls;
dtcField isotest_npcdiag_fi_face1;
dtcField isotest_npcdiag_fi_face2;
dtcField isotest_npcdiag_fi_text;
dtcField isotest_npcdiag_fi_voice;
dtcField isotest_npcdiag_fi_options;

dtcField isotest_npcdiag_fi_impulse;
dtcField isotest_npcdiag_fi_setup;

BGBDT_IsoMap *isotest_map;

float isotest_org[3];
float isotest_ang[3];
float isotest_rot[9];
float isotest_vel[3];
byte isotest_pldir;
byte isotest_iswalk;
byte isotest_isdead;
byte isotest_onground;
byte isotest_isback;
byte isotest_isball;

byte isotest_player_noclip;
byte isotest_player_undying;
int isotest_wrlflag;

dtVal isotest_player;
dtVal isotest_diagbox;
int isotest_diag_pchars;
int isotest_diag_lpchars;
int isotest_diag_sndid;

int isotest_snd_bgmid;
char *isotest_usebkg;

double isotest_diag_ptime;
double isotest_worldtime;

int isotest_invopen;
int isotest_invslot[8*16];


MAIN_EXPORT void IsoTest_SetDialog(dtVal dbox)
{
	isotest_diagbox=dbox;
	isotest_diag_pchars=0;
	isotest_diag_ptime=frgl_state->time_f;
	
	if(dtvTrueP(dbox))
		{ IsoTile_CallDiagSetup(dbox); }
}

MAIN_EXPORT void IsoTest_SetUseBackground(char *bkg)
{
//	isotest_diagbox=dbox;
//	isotest_diag_pchars=0;
//	isotest_diag_ptime=frgl_state->time_f;
	if(!bkg)
	{
		isotest_usebkg=NULL;
		return;
	}

	isotest_usebkg=frgl_strdup(bkg);
}

MAIN_EXPORT void IsoTest_SetApplyWorldFlags(int flag)
{
	isotest_wrlflag|=flag;
}

MAIN_EXPORT void IsoTest_SetClearWorldFlags(int flag)
{
	isotest_wrlflag&=~flag;
}

MAIN_EXPORT void IsoTest_GiveItem(int item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return;

	for(i=0; i<6*16; i++)
	{
		if((isotest_invslot[i]&255)==(item&255))
		{
			i0=((isotest_invslot[i]>>8)&255)+1;
			i1=((item>>8)&255)+1;
			i2=i0+i1;
			if(i2>256)
			{
				i0=256;
				i1=i2-256;
			}else
			{
				i0=i2;
				i1=0;
			}
			j=isotest_invslot[i];
			j=j&(~(0xFF00));
			j=j|((i0-1)<<8);
			isotest_invslot[i]=j;
			
			if(!i1)
			{
				item=0;
				break;
			}
			j=item;
			j=j&(~(0xFF00));
			j=j|((i1-1)<<8);
			item=j;
			continue;
		}
	}
	
	if(!item)
		return;

	for(i=0; i<6*16; i++)
	{
		if(!(isotest_invslot[i]&255))
		{
			isotest_invslot[i]=item;
			item=0;
			break;
		}
	}

	if(!item)
		return;

	/* drop item */
}


MAIN_EXPORT int IsoTest_TakeItem(int item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return(0);

	for(i=0; i<6*16; i++)
	{
		if((isotest_invslot[i]&255)==(item&255))
		{
			i0=((isotest_invslot[i]>>8)&255)+1;
			i1=((item>>8)&255)+1;
			i2=i0-i1;
			if(i2<0)
			{
				i1=i1-i0;
				i0=0;
//				i1=i2-256;
			}else
			{
				i0=i2;
				i1=0;
			}
			
			if(i0>0)
			{
				j=isotest_invslot[i];
				j=j&(~(0xFF00));
				j=j|((i0-1)<<8);
				isotest_invslot[i]=j;
			}else
			{
				isotest_invslot[i]=0;
			}
			
			if(!i1)
			{
				item=0;
				break;
			}
			j=item;
			j=j&(~(0xFF00));
			j=j|((i1-1)<<8);
			item=j;
			continue;
		}
	}
	
	if(!item)
		return(1);
	return(0);
}

MAIN_EXPORT int IsoTest_CheckItem(int item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return(0);

	for(i=0; i<6*16; i++)
	{
		if((isotest_invslot[i]&255)==(item&255))
		{
			i0=((isotest_invslot[i]>>8)&255)+1;
			i1=((item>>8)&255)+1;
			i2=i0-i1;
			if(i2<0)
			{
				i1=i1-i0;
				i0=0;
//				i1=i2-256;
			}else
			{
				i0=i2;
				i1=0;
			}
			
			if(i0>0)
			{
				j=isotest_invslot[i];
				j=j&(~(0xFF00));
				j=j|((i0-1)<<8);
//				isotest_invslot[i]=j;
			}else
			{
//				isotest_invslot[i]=0;
			}
			
			if(!i1)
			{
				item=0;
				break;
			}
			j=item;
			j=j&(~(0xFF00));
			j=j|((i1-1)<<8);
			item=j;
			continue;
		}
	}
	
	if(!item)
		return(1);
	return(0);
}

MAIN_EXPORT int IsoTest_SetBgm(char *bgm)
{
	char tb[256];
	BGBDT_SndMixChan *chn;

	chn=BGBDT_Sound_LookupMixChan(isotest_snd_bgmid);
	if(chn && !strcmp(chn->samp->name, bgm))
		return(0);

	sprintf(tb, "%s.wav", bgm);
	if(chn && !strcmp(chn->samp->name, tb))
		return(0);

	//bgm="sound/pi0_amb0";

	BGBDT_Sound_DeleteMixChan(isotest_snd_bgmid);
	isotest_snd_bgmid=BGBDT_Sound_PlaySound(bgm,
		64, BGBDT_SNDATT_NONE, BGBDT_SNDFL_LOOP);
	return(1);
}

vec2 IsoTest_EntGetOrigin(dtVal obj)
{
	void *p;
	if(!isotest_eb2d_fi_origin)
	{
		isotest_eb2d_fi_origin=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "origin");
		if(!isotest_eb2d_fi_origin)
			return(vec2(0,0));
	}
	p=dtcVaGetPtr(obj, isotest_eb2d_fi_origin);
	return(*(vec2 *)p);
}

void IsoTest_EntSetOrigin(dtVal obj, vec2 org)
{
	void *p;
	if(!isotest_eb2d_fi_origin)
	{
		isotest_eb2d_fi_origin=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "origin");
		if(!isotest_eb2d_fi_origin)
			return;
	}
	p=dtcVaGetPtr(obj, isotest_eb2d_fi_origin);
	(*(vec2 *)p)=org;
}

vec2 IsoTest_EntGetVelocity(dtVal obj)
{
	void *p;
	if(!isotest_eb2d_fi_velocity)
	{
		isotest_eb2d_fi_velocity=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "velocity");
		if(!isotest_eb2d_fi_velocity)
			return(vec2(0,0));
	}
	p=dtcVaGetPtr(obj, isotest_eb2d_fi_velocity);
	return(*(vec2 *)p);
}

void IsoTest_EntSetVelocity(dtVal obj, vec2 org)
{
	void *p;
	if(!isotest_eb2d_fi_velocity)
	{
		isotest_eb2d_fi_velocity=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "velocity");
		if(!isotest_eb2d_fi_velocity)
			return;
	}
	p=dtcVaGetPtr(obj, isotest_eb2d_fi_velocity);
	(*(vec2 *)p)=org;
}

float IsoTest_EntGetAngle(dtVal obj)
{
	if(!isotest_eb2d_fi_angle)
	{
		isotest_eb2d_fi_angle=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "angle");
		if(!isotest_eb2d_fi_angle)
			return(0);
	}
	return(dtcVaGetF(obj, isotest_eb2d_fi_angle));
}

float IsoTest_EntGetRadius(dtVal obj)
{
	if(!isotest_eb2d_fi_radius)
	{
		isotest_eb2d_fi_radius=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "radius");
		if(!isotest_eb2d_fi_radius)
			return(0);
	}

	return(dtcVaGetF(obj, isotest_eb2d_fi_radius));
}

void IsoTest_EntSetRadius(dtVal obj, float val)
{
	if(!isotest_eb2d_fi_radius)
	{
		isotest_eb2d_fi_radius=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "radius");
		if(!isotest_eb2d_fi_radius)
			return;
	}

	dtcVaSetF(obj, isotest_eb2d_fi_radius, val);
}

char *IsoTest_EntGetSprite(dtVal obj)
{
	dtVal v;

	if(!isotest_eb2d_fi_sprite)
	{
		isotest_eb2d_fi_sprite=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "sprite");
		if(!isotest_eb2d_fi_sprite)
			return(NULL);
	}

	v=dtcVaGetA(obj, isotest_eb2d_fi_sprite);
	return(BGBDT_TagStr_GetUtf8(v));
}

void IsoTest_EntGetSpriteSize(dtVal obj, float *xs, float *ys)
{
	float *fv;
	if(!isotest_eb2d_fi_sprite_sz)
	{
		isotest_eb2d_fi_sprite_sz=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "sprite_sz");
	}

	fv=dtcVaGetPtr(obj, isotest_eb2d_fi_sprite_sz);
	*xs=fv[0];
	*ys=fv[1];
}


char *IsoTest_DiagGetFace1(dtVal obj)
{
	dtVal v;

	if(!isotest_npcdiag_fi_face1)
	{
		isotest_npcdiag_fi_face1=BGBDTC_LookupClassSlotName(
			isotest_npcdiag_cls, "face1");
		if(!isotest_npcdiag_fi_face1)
			return(NULL);
	}

	v=dtcVaGetA(obj, isotest_npcdiag_fi_face1);
	return(BGBDT_TagStr_GetUtf8(v));
}

char *IsoTest_DiagGetFace2(dtVal obj)
{
	dtVal v;

	if(!isotest_npcdiag_fi_face2)
	{
		isotest_npcdiag_fi_face2=BGBDTC_LookupClassSlotName(
			isotest_npcdiag_cls, "face2");
		if(!isotest_npcdiag_fi_face2)
			return(NULL);
	}

	v=dtcVaGetA(obj, isotest_npcdiag_fi_face2);
	return(BGBDT_TagStr_GetUtf8(v));
}

char *IsoTest_DiagGetText(dtVal obj)
{
	dtVal v;

	if(!isotest_npcdiag_fi_text)
	{
		isotest_npcdiag_fi_text=BGBDTC_LookupClassSlotName(
			isotest_npcdiag_cls, "text");
		if(!isotest_npcdiag_fi_text)
			return(NULL);
	}

	v=dtcVaGetA(obj, isotest_npcdiag_fi_text);
	return(BGBDT_TagStr_GetUtf8(v));
}

dtVal IsoTest_DiagGetOptions(dtVal obj)
{
	dtVal v;

	if(!isotest_npcdiag_fi_options)
	{
		isotest_npcdiag_fi_options=BGBDTC_LookupClassSlotName(
			isotest_npcdiag_cls, "options");
		if(!isotest_npcdiag_fi_options)
			return(DTV_NULL);
	}

	v=dtcVaGetA(obj, isotest_npcdiag_fi_options);
	return(v);
}

char *IsoTest_DiagGetVoice(dtVal obj)
{
	dtVal v;

	if(!isotest_npcdiag_fi_voice)
	{
		isotest_npcdiag_fi_voice=BGBDTC_LookupClassSlotName(
			isotest_npcdiag_cls, "voice");
		if(!isotest_npcdiag_fi_voice)
			return(NULL);
	}

	v=dtcVaGetA(obj, isotest_npcdiag_fi_voice);
	return(BGBDT_TagStr_GetUtf8(v));
}

int main_prestart(int argc, char *argv[])
{
	char tb[256];
	int i, j, k;

	GfxDrv_SetDefaults("BTDT PlatTest", 1024, 768);
	FRGL_CvarSetDefault("hide_os_cursor", "1");

	Con_ExecCmdBuf("config.cfg");
	
	for(i=0; i<10; i++)
	{
		sprintf(tb, "resource/base%d.bpk", i);
		VfMount(tb, "/", "btpack", "");
	}

	return(0);
}

int main_loadscript(char *def)
{
	char tb[256];
	char *mods[256];
	int nmods;

	BGBDT_MM_ParsePrintInfo *prn;
	BS2CC_CompileContext *ctx;
	BSVM2_Context *vctx;
	BSVM2_ImageGlobal *vi;

	char **a;
	char *tbuf, *cs, *cse;
	dtVal v0, v1, v2, v3;
	int tsz;
	int i, j, k;

	tbuf=vf_loadfile(def, &tsz);
	if(!tbuf)
	{
		return(-1);
	}

	nmods=0;
	
	cs=tbuf; cse=tbuf+tsz;
	while(cs<cse)
	{
		cs=frgl_rsgets(tb, 256, cs);
		a=frgl_rsplit(tb);
		if(!a || !a[0])
			continue;
		if((*a[0]=='/') || (*a[0]=='#') || (*a[0]==';'))
			continue;

		if(!strcmp(a[0], "use"))
		{
			for(i=1; a[i]; i++)
				mods[nmods++]=frgl_strdup(a[i]);
		}
	}

	vf_freefdbuf(tbuf);
	mods[nmods]=0;

	ctx=BS2CC_AllocCompileContext();

	prn=BGBDT_MM_NewConsolePrinter();
	ctx->dbgprn=prn;

	BS2C_CompileModuleList(ctx, NULL, mods);

	if(ctx->ncerr || ctx->ncfatal)
	{
		return(-1);
	}

	tbuf=frgl_malloc(1<<20);

	i=BS2C_FlattenImage(ctx, tbuf, 1<<20);
	vf_storefile("plattest0.bsi", tbuf, i);

	isotest_img=BS2I_DecodeImageBuffer(tbuf, i);
	vi=BS2I_ImageGetMain(isotest_img, NULL);

	frgl_free(tbuf);
	BS2P_FreeCompileContext(ctx);

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, vi, NULL);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);

	isotest_vi_update=BS2I_ImageLookupFunc(
		isotest_img, "update");
	isotest_vi_spawnworld=BS2I_ImageLookupFunc(
		isotest_img, "spawn_world");

	return(0);
}

int main_scriptupdate(double dt)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;

	if(!isotest_vi_update)
		return(-1);

	args=targs;
	args[0].d=dt;

	IsoTile_SetGlobalA("world_entity", isotest_map->entarr);
	IsoTile_SetGlobalI("world_max_entity", isotest_map->nents);

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, isotest_vi_update, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	return(0);
}

int main_spawnworld(char *func)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;
	BSVM2_ImageGlobal *vi;

	if(!isotest_vi_spawnworld)
		return(-1);

	args=targs;

	IsoTile_SetGlobalA("world_entity", isotest_map->entarr);
	IsoTile_SetGlobalI("world_max_entity", isotest_map->nents);

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, isotest_vi_spawnworld, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	
	if(func)
	{
		vi=BS2I_ImageLookupFunc(isotest_img, func);

		if(vi)
		{
			vctx=BSVM2_Interp_AllocPoolContext();
			BSVM2_Interp_SetupCallVM(vctx, vi, args);
			BSVM2_Interp_RunContext(vctx, 999999999);
			BSVM2_Interp_FreePoolContext(vctx);
		}else
		{
			printf("main_spawnworld: no func %s\n",
				func);
			return(-2);
		}
	}
	
	return(0);
}

dtVal IsoTile_SpawnEntityBasic(char *cname, vec2 org)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;
	dtcClass clz;
	dtVal ent;
	dtVal obj, sf;
	void *sfp;
	int i, n;

	if(!isotest_img)
	{
		return(DTV_NULL);
	}

	if(!isotest_eb2d_fi_spawn)
	{
		isotest_eb2d_fi_spawn=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "spawn");
	}

	clz=BGBDTC_LookupClassQName(cname);
	if(!clz)
	{
		printf("Spawn Failed: No Class '%s'\n", cname);
		return(DTV_NULL);
	}
	
//	clz=BGBDTC_LookupClassQName(cname);
	obj=dtvWrapPtr(BGBDTC_AllocClassInstance(clz));

	n=isotest_map->nents++;
	dtvArraySetIndexDtVal(isotest_map->entarr, n, obj);

	IsoTest_EntSetOrigin(obj, org);

	sf=dtcVaGetA(obj, isotest_eb2d_fi_spawn);
	sfp=dtvUnwrapPtr(sf);
	if(!sfp)
	{
		printf("Spawn Failed: No Spawn Method '%s'\n", cname);
		return(DTV_NULL);
	}

	args=targs;
//	args[0].d=dt;

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupMethodCallVM(vctx, sfp, obj, args);
	i=BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);

	if(i)printf("IsoTile_SpawnEntityBasic: Classname=%s, Status=%d\n",
		cname, i);
	
	return(obj);
}

#if 1
int IsoTile_CallEntityTouch(dtVal self, dtVal other)
{
	BSVM2_Value targs[4];
	int i;

	targs[0].a=other;
	i=BSVM2_Interp_CallCacheMethodVM(self,
		&isotest_eb2d_fi_touch, targs,
		"EntityBase2D", "touch", NULL, 999999999);
	if(i)printf("IsoTile_CallEntityTouch: Status=%d\n", i);
	return(0);
}

int IsoTile_CallEntityUse(dtVal self, dtVal other)
{
	BSVM2_Value targs[4];
	int i;

	targs[0].a=other;
	i=BSVM2_Interp_CallCacheMethodVM(self,
		&isotest_eb2d_fi_use, targs,
		"EntityBase2D", "use", NULL, 999999999);
	if(i)printf("IsoTile_CallEntityUse: Status=%d\n", i);
	return(0);
}
#endif

int IsoTile_CallDiagImpulse(dtVal self, int imp)
{
	static dtcMethod fi=NULL;
	BSVM2_Value targs[4];
	int i;

	targs[0].i=imp;
	i=BSVM2_Interp_CallCacheMethodVM(self, &fi, targs,
		"NpcDialogBox", "impulse", NULL, 999999999);
	if(i)printf("IsoTile_CallDiagImpulse: Status=%d\n", i);
	return(0);
}

int IsoTile_CallDiagSetup(dtVal self)
{
	static dtcMethod fi=NULL;
	BSVM2_Value targs[4];
	int i;

//	targs[0].i=imp;
	i=BSVM2_Interp_CallCacheMethodVM(self, &fi, targs,
		"NpcDialogBox", "setup", NULL, 999999999);
	if(i)printf("IsoTile_CallDiagImpulse: Status=%d\n", i);
	return(0);
}

int IsoTile_SetGlobalA(char *cname, dtVal v)
{
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		isotest_img, cname);
	if(!vi)return(-1);
	BSVM2_Interp_SetGlobalA(vi, v);
	return(0);
}

int IsoTile_SetGlobalI(char *cname, int v)
{
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		isotest_img, cname);
	if(!vi)return(-1);
	BSVM2_Interp_SetGlobalI(vi, v);
	return(0);
}

int IsoTest_DrawSprite(char *spr, vec2 vorg, float xs, float ys)
{
	float org[4], v0[4], v1[4], v2[4], v3[4];
	float fw[4];
	int tex, flip;
	int i;

//	V3F_COPY(isotest_org, org);
	org[0]=v2x(vorg);
	org[1]=v2y(vorg);
	org[2]=4;
	flip=0;

	V3F_ADDSCALE(org, isotest_rot+0, -(xs/2), v0);
	V3F_ADDSCALE(org, isotest_rot+0,  (xs/2), v1);
	V3F_COPY(v0, v2);
	V3F_COPY(v1, v3);
//	v2[2]+=ys;
//	v3[2]+=ys;

	V3F_ADDSCALE(v0, isotest_rot+6, ys, v2);
	V3F_ADDSCALE(v1, isotest_rot+6, ys, v3);

	V3F_COPY(isotest_rot+3, fw);
	fw[2]=0;

	tex=Tex_LoadFile(spr, NULL, NULL);

	if(isotest_map)
		frglColor4fv(isotest_map->sprclr);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);

	frglTexCoord2f(flip?1:0, 0);
	frglVertex3fv(v0);
	frglTexCoord2f(flip?0:1, 0);
	frglVertex3fv(v1);
	frglTexCoord2f(flip?0:1, 1);
	frglVertex3fv(v3);
	frglTexCoord2f(flip?1:0, 1);
	frglVertex3fv(v2);

#if 0
	for(i=0; i<4; i++)
	{
		frglTexCoord2f(flip?1:0, 0);
		frglVertex3f(v0[0]+fw[0]*i*3, v0[1]+fw[1]*i*3, v0[2]+fw[2]*i*3);
		frglTexCoord2f(flip?0:1, 0);
		frglVertex3f(v1[0]+fw[0]*i*3, v1[1]+fw[1]*i*3, v1[2]+fw[2]*i*3);
		frglTexCoord2f(flip?0:1, 1);
		frglVertex3f(v3[0]+fw[0]*i*3, v3[1]+fw[1]*i*3, v3[2]+fw[2]*i*3);
		frglTexCoord2f(flip?1:0, 1);
		frglVertex3f(v2[0]+fw[0]*i*3, v2[1]+fw[1]*i*3, v2[2]+fw[2]*i*3);
	}
#endif

	frglEnd();
	return(0);
}

int IsoTile_DrawEntity(BGBDT_IsoMap *map,
	dtVal ent)
{
	vec2 org, corg;
	char *spr;
	float xs, ys, d;
	
	corg=vec2(isotest_org[0], isotest_org[1]);
	
	org=IsoTest_EntGetOrigin(ent);

	d=v2dist(org, corg);
	if(d>1024)
		return(0);
	if(d<=0)
		return(0);

	spr=IsoTest_EntGetSprite(ent);
	IsoTest_EntGetSpriteSize(ent, &xs, &ys);
	
	if(!spr)
		return(0);
	
//	if(xs<=0)xs=32;
//	if(ys<=0)ys=32;

	if(xs==0)xs=32;
	if(ys==0)ys=32;
	
	IsoTest_DrawSprite(spr, org, xs, ys);
	return(1);
}

int isotile_drawsky(BGBDT_IsoMap *map)
{
	float org[4], v0[4], v1[4], v2[4], v3[4];
	float s0, t0, s1, t1;
	float f, g;
	char *spr;
	int tex;

	spr=map->sky;

//	spr="testmap/swiss1_cyl0";
	if(!spr)
		spr="testmap/swiss1_day0";

	tex=Tex_LoadFile(spr, NULL, NULL);


//	v0[0]=(x+0)*8;	v0[1]=(y+0)*8;	v0[2]=0;
//	v1[0]=(x+1)*8;	v1[1]=(y+0)*8;	v1[2]=0;
//	v2[0]=(x+0)*8;	v2[1]=(y+1)*8;	v2[2]=0;
//	v3[0]=(x+1)*8;	v3[1]=(y+1)*8;	v3[2]=0;

	v0[0]=isotest_org[0]-512;	v1[0]=isotest_org[0]+512;
	v0[1]=isotest_org[1]-512;	v1[1]=isotest_org[1]-512;
	v2[0]=v0[0];				v2[1]=v0[1]+1024;
	v3[0]=v1[0];				v3[1]=v1[1]+1024;
	v0[2]=-256;	v1[2]=-256;
	v2[2]=-256;	v3[2]=-256;

	f=isotest_org[0]/(map->xs*8.0);
	g=isotest_org[1]/(map->xs*8.0);
//	f=0;

	s0=f*0.75;
	t0=0+g;
	s1=s0+0.25;
	t1=t0+0.85;
	
//	s0+=1.0/(16*64);
//	t0+=1.0/(16*64);
//	s1-=1.0/(16*64);
//	t1-=1.0/(16*64);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);

	frglTexCoord2f(s0, t0);
	frglVertex3fv(v0);
	frglTexCoord2f(s1, t0);
	frglVertex3fv(v1);
	frglTexCoord2f(s1, t1);
	frglVertex3fv(v3);
	frglTexCoord2f(s0, t1);
	frglVertex3fv(v2);

	frglEnd();
	return(0);
}

int isotile_drawmap2(BGBDT_IsoMap *map)
{
	float org[4], v0[4], v1[4], v2[4], v3[4];
	float s0, t0, s1, t1;
	char *spr;
	int tex;

	int x, y, mx, my, nx, ny;
	int i, j, k, l;

	x=isotest_org[0]/8.0;
	y=isotest_org[1]/8.0;
	mx=x-32; nx=x+34;
	my=y-16; ny=y+48;

	if(mx<0)mx=0;
	if(my<0)my=0;
	if(nx>map->xs)nx=map->xs;
	if(ny>map->ys)ny=map->ys;

	spr=map->atlas;
	if(!spr)
		spr="testmap/ghz_atlas0_1";

	tex=Tex_LoadFile(spr, NULL, NULL);

	frglColor4fv(map->wrlclr);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);

	for(y=my; y<ny; y++)
		for(x=mx; x<nx; x++)
	{
		k=map->tile_dtidx[y*map->xs+x];
		if(!k)continue;

		v0[0]=(x+0)*8;	v0[1]=(y+0)*8;	v0[2]=0;
		v1[0]=(x+1)*8;	v1[1]=(y+0)*8;	v1[2]=0;
		v2[0]=(x+0)*8;	v2[1]=(y+1)*8;	v2[2]=0;
		v3[0]=(x+1)*8;	v3[1]=(y+1)*8;	v3[2]=0;

		s0=(k&15)/16.0;
		t0=(15-(k>>4))/16.0;
		s1=s0+(1.0/16);
		t1=t0+(1.0/16);
		
//		s0+=1.0/(16*64);
//		t0+=1.0/(16*64);
//		s1-=1.0/(16*64);
//		t1-=1.0/(16*64);

		s0+=1.0/(16*32);
		t0+=1.0/(16*32);
		s1-=1.0/(16*32);
		t1-=1.0/(16*32);

		frglTexCoord2f(s0, t0);
		frglVertex3fv(v0);
		frglTexCoord2f(s1, t0);
		frglVertex3fv(v1);
		frglTexCoord2f(s1, t1);
		frglVertex3fv(v3);
		frglTexCoord2f(s0, t1);
		frglVertex3fv(v2);
	}
	
	frglEnd();
	return(0);
}

int isotile_drawmap(BGBDT_IsoMap *map)
{
//	BGBDT_IsoTile *tile;
	dtVal ent;
	int x, y, mx, my, nx, ny;
	int i, j, k, l;

	if(isotest_usebkg)
		return(0);

//	frglEnable(GL_CULL_FACE);
	frglDisable(GL_CULL_FACE);
	frglDisable(GL_ALPHA_TEST);

//	frglEnable(GL_ALPHA_TEST);

	isotile_drawsky(map);

	isotile_drawmap2(map);

	frglDisable(GL_CULL_FACE);
	frglEnable(GL_ALPHA_TEST);

	l=map->nents;
	for(i=0; i<l; i++)
	{
		ent=dtvArrayGetIndexDtVal(map->entarr, i);
		if(dtvNullP(ent))
			continue;
		IsoTile_DrawEntity(map, ent);
	}

	frglDisable(GL_ALPHA_TEST);

	frglColor4f(1,1,1,1);
	return(0);
}

#if 0
BGBDT_IsoTile *IsoTile_TileForCoord(BGBDT_IsoMap *map,
	float x, float y)
{
	int xo, yo;
	xo=(int)(x/64);
	yo=(int)(y/64);

	if((xo<0) || (yo<0))
		return(NULL);
	if((xo>=map->xs) || (yo>=map->ys))
		return(NULL);
	return(map->tile[yo*map->xs+xo]);
}

int IsoTile_CheckCollideTile(BGBDT_IsoMap *map, BGBDT_IsoTile *tile,
	float x, float y, float r)
{
	int rt;
	float x0, y0, x1, y1, x2, y2;
	
	if(!tile)
		return(0);
	
	if(tile->fl&BGBDT_ISOTFL_NONSOLID)
		return(0);
	
	x0=tile->x*64; x1=x0+64; x2=x0+32;
	y0=tile->y*64; y1=y0+64; y2=y0+32;

	if(!tile->tex_wall_xz && !tile->tex_wall_yz)
		return(0);

	if((x+r)<=x0)
		return(0);
	if((y+r)<=y0)
		return(0);
	if((x-r)>=x1)
		return(0);
	if((y-r)>=y1)
		return(0);

	if(tile->tex_wall_xz)
	{
		if(fabs(y-y2)<r)
		{
			if(tile->fl&BGBDT_ISOTFL_NX)
			{
				if((x-r)<=x2)
					return(1);
			}

			if(tile->fl&BGBDT_ISOTFL_PX)
			{
				if((x+r)>=x2)
					return(1);
			}
			
//			return(1);
		}
	}

	if(tile->tex_wall_yz)
	{
		if(fabs(x-x2)<r)
		{
			if(tile->fl&BGBDT_ISOTFL_NY)
			{
				if((y-r)<=y2)
					return(1);
			}

			if(tile->fl&BGBDT_ISOTFL_PY)
			{
				if((y+r)>=y2)
					return(1);
			}

//			return(1);
		}
	}

	return(0);
}

BGBDT_IsoTile *IsoTile_CheckTileCollide(BGBDT_IsoMap *map,
	float x, float y, float r)
{
	BGBDT_IsoTile *t0, *t1, *t2, *t3;
	int i;
	
	t0=IsoTile_TileForCoord(map, x-r, y-r);
	t1=IsoTile_TileForCoord(map, x+r, y-r);
	t2=IsoTile_TileForCoord(map, x-r, y+r);
	t3=IsoTile_TileForCoord(map, x+r, y+r);

	if(!t0)
		return(0);
	
	i=IsoTile_CheckCollideTile(map, t0, x, y, r);
	if(i)return(t0);

	if(t1!=t0)
	{
		i=IsoTile_CheckCollideTile(map, t1, x, y, r);
		if(i)return(t1);
	}

	if(t2!=t0)
	{
		i=IsoTile_CheckCollideTile(map, t2, x, y, r);
		if(i)return(t2);

		if(t3!=t2)
		{
			i=IsoTile_CheckCollideTile(map, t3, x, y, r);
			if(i)return(t3);
		}
	}
	return(NULL);
}
#endif


int IsoTile_EntCheckCollide(dtVal e0, dtVal e1)
{
	vec2 eo0, eo1;
	
	eo0=IsoTest_EntGetOrigin(e0);
	eo1=IsoTest_EntGetOrigin(e1);
	
	if(v2dist(eo0, eo1)<8)
		return(1);
	return(0);
}

int IsoTile_UpdateMap(BGBDT_IsoMap *map)
{
	dtVal e0, e1;
	int i, j, k;

	return(0);
}

dtVal IsoTile_QueryCollideMove(BGBDT_IsoMap *map,
	dtVal self, vec2 org, float rad);

byte IsoTile_CheckTileCollide2(BGBDT_IsoMap *map,
	float x, float y, float r)
{
	int mtx, ntx, mty, nty;
	int tx, ty, tt, t;

	mtx=(x-r)/8;	ntx=(x+r)/8;
	mty=(y-r)/8;	nty=(y+r)/8;

	if(mtx<0)mtx=0;
	if(mty<0)mty=0;
	if(ntx>=map->xs)ntx=map->xs-1;
	if(nty>=map->ys)nty=map->ys-1;

	if((mtx>ntx) || (mty>nty))
//		return(0);
		return(1);
		
	tt=9999;
	for(ty=mty; ty<=nty; ty++)
		for(tx=mtx; tx<=ntx; tx++)
	{
		t=map->tile_phidx[ty*map->xs+tx];
		if(!t)continue;
		if(t<tt)tt=t;
	}
	if(tt<256)
		return(tt);
	return(0);

//	tx=x/8;
//	ty=y/8;

//	if(tx<0)return(0);
//	if(ty<0)return(0);
//	if(tx>=map->xs)return(0);
//	if(ty>=map->ys)return(0);
//	return(map->tile_phidx[ty*map->xs+tx]);
}

int IsoTile_CheckCollideMove(BGBDT_IsoMap *map,
	dtVal self, vec2 org, float rad)
{
//	BGBDT_IsoTile *ti;
	dtVal other;
	int i;
	
	other=IsoTile_QueryCollideMove(map, self, org, rad);
	if(!dtvNullP(other))
	{
		IsoTile_CallEntityTouch(self, other);
		IsoTile_CallEntityTouch(other, self);
		return(1);
	}
	
//	ti=IsoTile_CheckTileCollide(map,
//		v2x(org), v2y(org), rad);
//	if(ti)
//		return(1);

	i=IsoTile_CheckTileCollide2(map, v2x(org), v2y(org), rad);
	if(i)return(i);
	
	return(0);
}

dtVal IsoTile_QueryCollideMove(BGBDT_IsoMap *map,
	dtVal self, vec2 org, float rad)
{
	dtVal ent;
	vec2 eorg;
	float erad;
	int i, j, k;

	if(rad<0)
		return(DTV_NULL);
	
	for(i=0; i<map->nents; i++)
	{
		ent=dtvArrayGetIndexDtVal(map->entarr, i);
		
		if(dtvEqqP(ent, self))
			continue;
		
		eorg=IsoTest_EntGetOrigin(ent);
		erad=IsoTest_EntGetRadius(ent);
		if(erad<0)
			continue;

		if(v2dist(eorg, org)<(erad+rad))
			return(ent);
	}
	return(DTV_NULL);
}

MAIN_EXPORT int IsoTile_CheckEntMoveCollidePos(dtVal self)
{
//	BGBDT_IsoTile *ti;
	vec2 org;
	dtVal other;
	float rad;
	
	org=IsoTest_EntGetOrigin(self);
	rad=IsoTest_EntGetRadius(self);

	other=IsoTile_QueryCollideMove(isotest_map, self, org, rad);
	if(!dtvNullP(other))
	{
		IsoTile_CallEntityTouch(self, other);
		IsoTile_CallEntityTouch(other, self);
		return(1);
	}
	
//	ti=IsoTile_CheckTileCollide(isotest_map,
//		v2x(org), v2y(org), rad);
//	if(ti)
//		return(1);
	
	return(0);
}

int isotest_drawplayer()
{
	float org[4], v0[4], v1[4], v2[4], v3[4];
	float rad;
	int tex, flip;
	int i;
	char *str;

	if(isotest_usebkg)
		return(0);

//	V3F_ADDSCALE(isotest_org,
//		isotest_rot+3, 256, org);
//	V3F_ADDSCALE(org,
//		isotest_rot+6, 256, org);

	V3F_COPY(isotest_org, org);
	org[2]=4;

	rad=IsoTest_EntGetRadius(isotest_player);
	org[1]-=rad;

	V3F_ADDSCALE(org, isotest_rot+0, -12, v0);
	V3F_ADDSCALE(org, isotest_rot+0,  12, v1);
	V3F_COPY(v0, v2);
	V3F_COPY(v1, v3);
//	v2[2]+=48;
//	v3[2]+=48;

	V3F_ADDSCALE(v0, isotest_rot+6, 48, v2);
	V3F_ADDSCALE(v1, isotest_rot+6, 48, v3);

	i=((int)(frgl_state->time_f*8))&1;
	switch(isotest_pldir)
	{
	case 0:
		if(isotest_iswalk)
		{
//			if(i)
//				str="sprites/yazil/yazil_walk1_lf1";
//			else
//				str="sprites/yazil/yazil_walk2_lf1";
			str="sprites/yazil/yazil_trun1";
			flip=0;
			break;
		}
		str="sprites/yazil/yazil_lf1";
		flip=0;
		break;
	case 1:
		if(isotest_iswalk)
		{
//			if(i)
//				str="sprites/yazil/yazil_walk1_lf1";
//			else
//				str="sprites/yazil/yazil_walk2_lf1";
			str="sprites/yazil/yazil_trun1";
			flip=1;
			break;
		}
		str="sprites/yazil/yazil_lf1";
		flip=1;
		break;
	case 2:
		if(isotest_iswalk)
		{
			str="sprites/yazil/yazil_walk_bk0";
			flip=((int)(frgl_state->time_f*8))&1;
			break;
		}
	
		str="sprites/yazil/yazil_bk0";
		flip=0;
		break;
	case 3:
		if(isotest_iswalk)
		{
			str="sprites/yazil/yazil_walk_ft0";
			flip=((int)(frgl_state->time_f*8))&1;
			break;
		}

		str="sprites/yazil/yazil_ft0";
		flip=0;
		break;
	}

	if(isotest_isball)
	{
		V3F_COPY(isotest_org, org);
//		V3F_ADDSCALE(isotest_org, isotest_rot+3, -16, org);
		org[2]=0;

//		V3F_ADDSCALE(org, isotest_rot+0, -16, v0);
//		V3F_ADDSCALE(org, isotest_rot+0,  16, v1);
//		V3F_COPY(v0, v2);
//		V3F_COPY(v1, v3);
//		v2[2]+=32;
//		v3[2]+=32;

//		V3F_ADDSCALE(v0, isotest_rot+3, 32, v2);
//		V3F_ADDSCALE(v1, isotest_rot+3, 32, v3);

		v0[0]=org[0]-16; v0[1]=org[1]-16; v0[2]=4;
		v1[0]=org[0]+16; v1[1]=org[1]-16; v1[2]=4;
		v2[0]=org[0]-16; v2[1]=org[1]+16; v2[2]=4;
		v3[0]=org[0]+16; v3[1]=org[1]+16; v3[2]=4;

		str="sprites/yazil/yazil_spin0_1";
		flip=0;
	}

	if(isotest_isdead)
	{
		V3F_COPY(isotest_org, org);
		org[2]=0;

		V3F_ADDSCALE(org, isotest_rot+0, -24, v0);
		V3F_ADDSCALE(org, isotest_rot+0,  24, v1);
		V3F_COPY(v0, v2);
		V3F_COPY(v1, v3);
		v2[2]+=32;
		v3[2]+=32;

		str="sprites/yazil/yazil_dead0";
		flip=0;
	}

	if(isotest_map)
		frglColor4fv(isotest_map->sprclr);

	tex=Tex_LoadFile(str, NULL, NULL);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(flip?1:0, 0);
	frglVertex3fv(v0);
	frglTexCoord2f(flip?0:1, 0);
	frglVertex3fv(v1);
	frglTexCoord2f(flip?0:1, 1);
	frglVertex3fv(v3);
	frglTexCoord2f(flip?1:0, 1);
	frglVertex3fv(v2);
	frglEnd();
	return(0);
}

int IsoTile_DrawDialog(dtVal dbox)
{
	char tb[4096];
	dtVal opts, a;
	char *face1, *face2, *text, *voc;
	char *sf0, *s0;
	float x0, y0, x1, y1;
	float x2, y2, x3, y3;
	float z0, z1;
	int tex;
	int i, j, k, l;

//	x0=tile->x*64; x1=x0+64; x2=x0+32;
//	y0=tile->y*64; y1=y0+64; y2=y0+32;
//	z0=0; z1=64;

	face1=IsoTest_DiagGetFace1(dbox);
	face2=IsoTest_DiagGetFace2(dbox);
	text=IsoTest_DiagGetText(dbox);
	opts=IsoTest_DiagGetOptions(dbox);
	voc=IsoTest_DiagGetVoice(dbox);
	
//	voc=NULL;

	if(!voc)
		voc="sound/box_aa1";

	x0=-320; y0=-240;
	x1=320; y1=240;

	x2=-320+4; y2=0+4;
	x3=-80-4; y3=240-4;

	sf0=face1; l=0;
	if(text)
	{
		l=strlen(text);
		if(isotest_diag_pchars<l)
		{
			i=((int)(frgl_state->time_f*8))&1;
			if(i)sf0=face2;

			if(isotest_diag_sndid<=0)
			{
				isotest_diag_sndid=BGBDT_Sound_PlaySound(
					voc, 32, 0, BGBDT_SNDFL_LOOP);
			}
		}else
		{
			if(isotest_diag_sndid>0)
			{
				BGBDT_Sound_DeleteMixChan(isotest_diag_sndid);
				isotest_diag_sndid=0;
			}
		}
	}
	
	isotest_diag_pchars=(frgl_state->time_f-isotest_diag_ptime)*24;
	if((isotest_diag_pchars<l) &&
		(isotest_diag_pchars!=isotest_diag_lpchars))
	{
//		isotest_diag_sndid=BGBDT_Sound_PlaySound(
//			"sound/box_aa2", 32, 0, 0);
	}

	tex=Tex_LoadFile("images/ui/dialog0_1", NULL, NULL);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex2f(x0, y0);
	frglTexCoord2f(1, 0);
	frglVertex2f(x1, y0);
	frglTexCoord2f(1, 1);
	frglVertex2f(x1, y1);
	frglTexCoord2f(0, 1);
	frglVertex2f(x0, y1);
	frglEnd();

	tex=Tex_LoadFile(sf0, NULL, NULL);

	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex2f(x2, y2);
	frglTexCoord2f(1, 0);
	frglVertex2f(x3, y2);
	frglTexCoord2f(1, 1);
	frglVertex2f(x3, y3);
	frglTexCoord2f(0, 1);
	frglVertex2f(x2, y3);
	frglEnd();
	
	if(!dtvNullP(opts) && dtvIsArrayP(opts))
	{
		l=dtvArrayGetSize(opts);
		for(i=0; i<l; i++)
		{
			a=dtvArrayGetIndexDtVal(opts, i);
			s0=BGBDT_TagStr_GetChars(a);
			
			if(s0)
			{
				sprintf(tb, "%d: %s", i+1, s0);
				GfxFont_DrawString(tb,
					-80+4, 240-16-4-i*12, 8, 8, 0, 0, 0, 255);
			}
		}
	}
	
	if(text)
	{
		l=strlen(text);
		if(isotest_diag_pchars<l)
		{
			strcpy(tb, text);
			tb[isotest_diag_pchars]=0;
			GfxFont_DrawString2(tb, -320+4+8, -8-4, 8, 8, 0, 0, 0, 255);
		}else
		{
	//		GfxFont_DrawString(text, -320+4, -8-4, 8, 8, 0, 0, 0, 255);
			GfxFont_DrawString2(text, -320+4+8, -8-4, 8, 8, 0, 0, 0, 255);
		}
	}
	return(0);
}

int IsoTile_DrawInventory(void)
{
	char tb[256];
	dtVal opts, a;
//	char *face1, *face2, *text;
	char *sf0, *s0;
	float x0, y0, x1, y1;
	float x2, y2, x3, y3;
	float z0, z1;
	float u0, v0, u1, v1;
	int tex;
	int i, j, k, l;

	x0=-320; y0=-240;
	x1=320; y1=240;

	x2=-320+4; y2=0+4;
	x3=-80-4; y3=240-4;

	tex=Tex_LoadFile("images/ui/inven0", NULL, NULL);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex2f(x0, y0);
	frglTexCoord2f(1, 0);
	frglVertex2f(x1, y0);
	frglTexCoord2f(1, 1);
	frglVertex2f(x1, y1);
	frglTexCoord2f(0, 1);
	frglVertex2f(x0, y1);
	frglEnd();
	
	tex=Tex_LoadFile("images/inven/invgrid0", NULL, NULL);
	frglBindTexture2D(tex);

//	isotest_invslot[0*16+1]=1;

	frglBegin(GL_QUADS);
	for(i=0; i<6; i++)
	{
		for(j=0; j<16; j++)
		{
			k=isotest_invslot[i*16+j];

			u0=(k&15)*(1.0/16);
			v0=(15-((k>>4)&15))*(1.0/16);
			u1=u0+(1.0/16);
			v1=v0+(1.0/16);

			x2=x0+j*40+6; x3=x2+32;
			y2=y0+i*40+6; y3=y2+32;

			frglTexCoord2f(u0, v0);
			frglVertex2f(x2, y2);
			frglTexCoord2f(u1, v0);
			frglVertex2f(x3, y2);
			frglTexCoord2f(u1, v1);
			frglVertex2f(x3, y3);
			frglTexCoord2f(u0, v1);
			frglVertex2f(x2, y3);
		}
	}
	frglEnd();

	for(i=0; i<6; i++)
	{
		for(j=0; j<16; j++)
		{		
			k=isotest_invslot[i*16+j];
			if(!(k&255))
				continue;
			l=((k>>8)&255)+1;
			if(l<2)
				continue;

			x2=x0+j*40+6; x3=x2+32;
			y2=y0+i*40+6; y3=y2+32;

			sprintf(tb, "%3d", l);
			GfxFont_DrawString(tb, x3-24, y2+2, 8, 8, 0, 0, 0, 255);
		}
	}
	return(0);
}

MAIN_EXPORT int IsoTest_KillPlayer(void)
{
	isotest_isdead=1;
	return(0);
}

int IsoTile_DrawDeadScreen(void)
{
	char tb[256];
	dtVal opts, a;
//	char *face1, *face2, *text;
	char *sf0, *s0;
	float x0, y0, x1, y1;
	float x2, y2, x3, y3;
	float z0, z1;
	float u0, v0, u1, v1;
	int tex;
	int i, j, k, l;

	x0=-512; y0=-384;
	x1=512; y1=384;

	tex=Tex_LoadFile("images/ui/dead0", NULL, NULL);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex2f(x0, y0);
	frglTexCoord2f(1, 0);
	frglVertex2f(x1, y0);
	frglTexCoord2f(1, 1);
	frglVertex2f(x1, y1);
	frglTexCoord2f(0, 1);
	frglVertex2f(x0, y1);
	frglEnd();
	return(0);
}

int IsoTile_DrawUseBackground(void)
{
	char tb[256];
	dtVal opts, a;
//	char *face1, *face2, *text;
	char *sf0, *s0;
	float x0, y0, x1, y1;
	float x2, y2, x3, y3;
	float z0, z1;
	float u0, v0, u1, v1;
	int tex;
	int i, j, k, l;

	if(!isotest_usebkg)
		return(0);

	x0=-512; y0=-384;
	x1=512; y1=384;

	tex=Tex_LoadFile(isotest_usebkg, NULL, NULL);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex2f(x0, y0);
	frglTexCoord2f(1, 0);
	frglVertex2f(x1, y0);
	frglTexCoord2f(1, 1);
	frglVertex2f(x1, y1);
	frglTexCoord2f(0, 1);
	frglVertex2f(x0, y1);
	frglEnd();
	return(0);
}

int isotile_blfclr_idx(byte *buf, int xs, int ys,
	int xo, int yo)
{
	int cr, cg, cb;
	int ix;

	if((xo<0) || (yo<0))
		return(0);
	if((xo>=xs) || (yo>=ys))
		return(0);

	cr=buf[(yo*xs+xo)*4+0];
	cg=buf[(yo*xs+xo)*4+1];
	cb=buf[(yo*xs+xo)*4+2];
	
	ix=((cr>>6)<<4)|((cg>>6)<<2)|(cb>>6);
	return(ix);
}

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
		IsoTest_GiveItem((64<<8)|4);
		IsoTest_GiveItem((64<<8)|5);
	}
}

byte *iso_fbuf;
byte *iso_wbuf;
byte *iso_ebuf;
char *iso_map;
char *iso_nextmap;
char *iso_callfunc;
int iso_xs, iso_ys;

byte iso_tileidxclr[256*4];

byte iso_entidxclr[256*4];
char *iso_entidxstr[256];
int nentidx;

byte iso_physidxclr[256*4];
char *iso_physidxstr[256];
int nphysidx;

int isotile_blfclr_tileidx(byte *buf, int xs, int ys,
	int xo, int yo)
{
	int cr, cg, cb, ca;
	int dr, dg, db;
	int ix, bd, d;
	int i, j, k;

	if((xo<0) || (yo<0))
		return(0);
	if((xo>=xs) || (yo>=ys))
		return(0);

	cr=buf[(yo*xs+xo)*4+0];
	cg=buf[(yo*xs+xo)*4+1];
	cb=buf[(yo*xs+xo)*4+2];
	ca=buf[(yo*xs+xo)*4+3];
	
//	ix=((cr>>6)<<4)|((cg>>6)<<2)|(cb>>6);

	if(ca<16)
		return(0);
	if((cr+cg+cb)<8)
		return(0);
	
	ix=-1; bd=999999999;
	for(i=0; i<256; i++)
	{
		if(!iso_tileidxclr[i*4+3])
			continue;
//		if((iso_tileidxclr[i*4+0]==cr) &&
//			(iso_tileidxclr[i*4+1]==cg) &&
//			(iso_tileidxclr[i*4+2]==cb))
//				return(i);
		dr=iso_tileidxclr[i*4+0]-cr;
		dg=iso_tileidxclr[i*4+1]-cg;
		db=iso_tileidxclr[i*4+2]-cb;
		d=dr*dr+dg*dg+db*db;
		if(d>768)
			continue;

		if(d<bd)
			{ ix=i; bd=d; }
	}

	return(ix);
}

char *isotile_blfclr_entidx(byte *buf, int xs, int ys,
	int xo, int yo)
{
	int cr, cg, cb, ca;
	int dr, dg, db;
	int ix, bd, d;
	int i, j, k;

	if((xo<0) || (yo<0))
		return(NULL);
	if((xo>=xs) || (yo>=ys))
		return(NULL);

	cr=buf[(yo*xs+xo)*4+0];
	cg=buf[(yo*xs+xo)*4+1];
	cb=buf[(yo*xs+xo)*4+2];
	ca=buf[(yo*xs+xo)*4+3];
	
//	ix=((cr>>6)<<4)|((cg>>6)<<2)|(cb>>6);

	if(ca<16)
		return(NULL);
	if((cr+cg+cb)<8)
		return(NULL);
	
	ix=-1; bd=999999999;
	for(i=0; i<nentidx; i++)
	{
		if(!iso_entidxclr[i*4+3])
			continue;
//		if((iso_tileidxclr[i*4+0]==cr) &&
//			(iso_tileidxclr[i*4+1]==cg) &&
//			(iso_tileidxclr[i*4+2]==cb))
//				return(i);
		dr=iso_entidxclr[i*4+0]-cr;
		dg=iso_entidxclr[i*4+1]-cg;
		db=iso_entidxclr[i*4+2]-cb;
		d=dr*dr+dg*dg+db*db;
		if(d>768)
			continue;
		if(d<bd)
			{ ix=i; bd=d; }
	}
	
	if(ix<0)
		return(NULL);

	return(iso_entidxstr[ix]);
}

char *isotile_blfclr_physidx(byte *buf, int xs, int ys,
	int xo, int yo)
{
	int cr, cg, cb, ca;
	int dr, dg, db;
	int ix, bd, d;
	int i, j, k;

	if((xo<0) || (yo<0))
		return(NULL);
	if((xo>=xs) || (yo>=ys))
		return(NULL);

	cr=buf[(yo*xs+xo)*4+0];
	cg=buf[(yo*xs+xo)*4+1];
	cb=buf[(yo*xs+xo)*4+2];
	ca=buf[(yo*xs+xo)*4+3];
	
//	ix=((cr>>6)<<4)|((cg>>6)<<2)|(cb>>6);

	if(ca<16)
		return(NULL);
	if((cr+cg+cb)<8)
		return(NULL);
	
	ix=-1; bd=999999999;
	for(i=0; i<nphysidx; i++)
	{
		if(!iso_physidxclr[i*4+3])
			continue;
		dr=iso_physidxclr[i*4+0]-cr;
		dg=iso_physidxclr[i*4+1]-cg;
		db=iso_physidxclr[i*4+2]-cb;
		d=dr*dr+dg*dg+db*db;
		if(d>768)
			continue;
		if(d<bd)
			{ ix=i; bd=d; }
	}
	
	if(ix<0)
		return(NULL);

	return(iso_physidxstr[ix]);
}

int isotest_hexnib(char c)
{
	if((c>='0') && (c<='9'))
		return(c-'0');
	if((c>='A') && (c<='F'))
		return(10+(c-'A'));
	if((c>='a') && (c<='f'))
		return(10+(c-'a'));
	return(0);
}

int isotest_hexbyte(char *s)
{
	return((isotest_hexnib(s[0])<<4)|isotest_hexnib(s[1]));
}

void isotest_loadmap(char *mapinf);

void isotest_newgame_init()
{
	isotest_loadmap("testmap/ghz_info.txt");
}

void isotest_loadmap(char *mapinf)
{
//	BGBDT_IsoTile *tile;
	float sclr[4], wclr[4];
	char tb[256];

	char *fname, *wname, *ename;
	char *sky, *atlas, *atlasix;
//	char *callfn;
	byte *ixbuf, *itbuf;
	char **a;

	char *s0, *s1, *cs, *cse;
	int xs0, ys0, stx, sty, sz;
	int k0, k1, k2, k3, k4, k5, k6, k7;
	int i, j, k, l;

#if 0
	if(iso_map && strcmp(iso_map, mapinf))
	{
		iso_fbuf=NULL;
		iso_wbuf=NULL;
		iso_ebuf=NULL;
	}
#endif

	if(!mapinf)
		return;

	if(!iso_fbuf || (iso_map && strcmp(iso_map, mapinf)))
	{
		nentidx=0;
		nphysidx=0;
//		itbuf=vf_loadfile("testmap/ghz_info.txt", &sz);
		itbuf=vf_loadfile(mapinf, &sz);

		if(!itbuf)
			return;


		iso_map=frgl_strdup(mapinf);
		iso_nextmap=NULL;
		iso_callfunc=NULL;
		isotest_usebkg=NULL;
		
		fname=NULL;
		wname=NULL;
		ename=NULL;
//		callfn=NULL;
		sky=NULL;
		atlas=NULL;
		atlasix=NULL;
		
		V4F_CONST(sclr, 1);
		V4F_CONST(wclr, 1);
		
		cs=itbuf; cse=cs+sz;
		while(*cs && (cs<cse))
		{
			cs=frgl_rsgets(tb, 255, cs);
			a=frgl_rsplit(tb);
			if(!a[0])continue;
			if((a[0][0]=='#') || (a[0][0]==';') || (a[0][0]=='/'))
				continue;
			
			if(!strcmp(a[0], "entdef"))
			{
				i=nentidx++;
				iso_entidxclr[i*4+0]=isotest_hexbyte(a[1]+1);
				iso_entidxclr[i*4+1]=isotest_hexbyte(a[1]+3);
				iso_entidxclr[i*4+2]=isotest_hexbyte(a[1]+5);
				iso_entidxclr[i*4+3]=255;
				iso_entidxstr[i]=frgl_strdup(a[2]);
				continue;
			}

			if(!strcmp(a[0], "physdef"))
			{
				i=nphysidx++;
				iso_physidxclr[i*4+0]=isotest_hexbyte(a[1]+1);
				iso_physidxclr[i*4+1]=isotest_hexbyte(a[1]+3);
				iso_physidxclr[i*4+2]=isotest_hexbyte(a[1]+5);
				iso_physidxclr[i*4+3]=255;
				iso_physidxstr[i]=frgl_strdup(a[2]);
				continue;
			}
			
			if(!strcmp(a[0], "tilemap"))
			{
				fname=frgl_strdup(a[1]);
				continue;
			}

			if(!strcmp(a[0], "physmap"))
			{
				wname=frgl_strdup(a[1]);
				continue;
			}

			if(!strcmp(a[0], "spawnmap"))
			{
				ename=frgl_strdup(a[1]);
				continue;
			}

			if(!strcmp(a[0], "nextmap"))
			{
				iso_nextmap=frgl_strdup(a[1]);
				continue;
			}

			if(!strcmp(a[0], "callfunc"))
			{
				iso_callfunc=frgl_strdup(a[1]);
				continue;
			}

			if(!strcmp(a[0], "skytex"))
			{
				sky=frgl_strdup(a[1]);
				continue;
			}

			if(!strcmp(a[0], "atlastex"))
			{
				atlas=frgl_strdup(a[1]);
				continue;
			}

			if(!strcmp(a[0], "atlasidx"))
			{
				atlasix=frgl_strdup(a[1]);
				continue;
			}

			if(!strcmp(a[0], "sprclr"))
			{
				 sclr[0]=atof(a[1]); sclr[1]=atof(a[2]);
				 sclr[2]=atof(a[3]); sclr[3]=atof(a[4]);
				 continue;
			}

			if(!strcmp(a[0], "wrlclr"))
			{
				 wclr[0]=atof(a[1]); wclr[1]=atof(a[2]);
				 wclr[2]=atof(a[3]); wclr[3]=atof(a[4]);
				 continue;
			}
		}
	
//		iso_fbuf=Tex_LoadFileRaw("testmap/Ghz1_M0_Tile.png",
//			&iso_xs, &iso_ys);
//		iso_wbuf=Tex_LoadFileRaw("testmap/Ghz1_M0_Solid.png", NULL, NULL);
//		iso_ebuf=Tex_LoadFileRaw("testmap/Ghz1_M0_Spawn.png", NULL, NULL);

		iso_fbuf=Tex_LoadFileRaw(fname, &iso_xs, &iso_ys);
		iso_wbuf=Tex_LoadFileRaw(wname, NULL, NULL);
		iso_ebuf=Tex_LoadFileRaw(ename, NULL, NULL);

		if(!isotest_map)
		{
			isotest_map=dtmAlloc("bgbdt_isomap_t",
				sizeof(BGBDT_IsoMap));
		}else
		{
			if(isotest_map->tile_dtidx)
			{
				dtmFree(isotest_map->tile_dtidx);
				isotest_map->tile_dtidx=NULL;
			}

			if(isotest_map->tile_phidx)
			{
				dtmFree(isotest_map->tile_phidx);
				isotest_map->tile_phidx=NULL;
			}

			if(isotest_map->tile_spidx)
			{
				dtmFree(isotest_map->tile_spidx);
				isotest_map->tile_spidx=NULL;
			}

			isotest_map->xs=0;
			isotest_map->ys=0;
		}

		isotest_map->sky=sky;
		isotest_map->atlas=atlas;

		V4F_COPY(sclr, isotest_map->sprclr);
		V4F_COPY(wclr, isotest_map->wrlclr);

		if(iso_fbuf)
		{
	//		isotest_map->tile=dtmAlloc("bgbdt_isomaptiles_t",
	//			iso_xs*iso_ys*sizeof(BGBDT_IsoTile *));
			isotest_map->xs=iso_xs;
			isotest_map->ys=iso_ys;
		
			isotest_map->tile_dtidx=dtmAlloc("bgbdt_tilebytes_t",
				iso_xs*iso_ys);
			isotest_map->tile_phidx=dtmAlloc("bgbdt_tilebytes_t",
				iso_xs*iso_ys);
			isotest_map->tile_spidx=dtmAlloc("bgbdt_tilebytes_t",
				iso_xs*iso_ys);
		}

		if(!atlasix)
			atlasix="testmap/ghz_atlas0_1_ix.png";
		ixbuf=Tex_LoadFileRaw(atlasix, &xs0, &ys0);
		stx=xs0>>4;
		sty=ys0>>4;
		
		for(i=0; i<16; i++)
			for(j=0; j<16; j++)
		{
			k=((15-i)*sty+1)*xs0+(j*stx+1);
			l=i*16+j;
			iso_tileidxclr[l*4+0]=ixbuf[k*4+0];
			iso_tileidxclr[l*4+1]=ixbuf[k*4+1];
			iso_tileidxclr[l*4+2]=ixbuf[k*4+2];
			iso_tileidxclr[l*4+3]=ixbuf[k*4+3];
		}
	}

	if(iso_fbuf)
	{
		for(i=0; i<iso_ys; i++)
			for(j=0; j<iso_xs; j++)
		{
			k=isotile_blfclr_tileidx(iso_fbuf, iso_xs, iso_ys, j, i);
	//		if(!k)
	//			continue;
			isotest_map->tile_dtidx[i*iso_xs+j]=k;
		}
	}

	if(iso_wbuf)
	{
		for(i=0; i<iso_ys; i++)
			for(j=0; j<iso_xs; j++)
		{
			s0=isotile_blfclr_physidx(iso_wbuf, iso_xs, iso_ys, j, i);
			if(!s0)
				continue;
				
			k=0;
			if(!strcmp(s0, "solid"))k=0x01;
			if(!strcmp(s0, "solid_mvup"))k=0x02;
			if(!strcmp(s0, "solid_roll"))k=0x03;
			if(!strcmp(s0, "solid_front"))k=0x04;
			if(!strcmp(s0, "solid_back"))k=0x05;

			if(!strcmp(s0, "next_map"))
				k=0x40;

			if(!strcmp(s0, "set_front"))
				k=0x44;
			if(!strcmp(s0, "set_back"))
				k=0x45;

			isotest_map->tile_phidx[i*iso_xs+j]=k;
		}
	}

	isotest_map->entarr=dtvNewArray(4096, BGBDT_BASETY_ADDRESS);
	isotest_map->nents=0;

	IsoTile_SetGlobalA("world_entity", isotest_map->entarr);

	isotest_org[0]=128;
	isotest_org[1]=128;
	isotest_org[2]=128;
	
//	isotest_ang[0]=-45;
	isotest_ang[0]=0;
//	isotest_ang[1]=-30;
	isotest_ang[1]=-90;
	isotest_ang[2]=0;

	if(iso_ebuf)
	{
		for(i=0; i<iso_ys; i++)
			for(j=0; j<iso_xs; j++)
		{
	//		k=isotile_blfclr_idx(iso_ebuf, iso_xs, iso_ys, j, i);
	//		if(!k)
	//			continue;

	//		k4=isotile_blfclr_idx(iso_wbuf, iso_xs, iso_ys, j, i);
	//		if(k4)
	//			continue;

			s0=isotile_blfclr_entidx(iso_ebuf, iso_xs, iso_ys, j, i);

			if(!s0)continue;

			if(!strcmp(s0, "info_player_start"))
			{
				isotest_org[0]=j*8;
				isotest_org[1]=i*8;
				continue;
			}

			IsoTile_SpawnEntityBasic(s0,
				vec2(j*8+4, i*8+4));
		}
	}

	isotest_player=IsoTile_SpawnEntityBasic("player", vec2(128,128));
	isotest_worldtime=0;
	
	for(i=0; i<8*16; i++)
		isotest_invslot[i]=0;
	isotest_isdead=0;

	main_spawnworld(iso_callfunc);

//	IsoTest_SetBgm("sound/pi0_amb0");
//	isotest_snd_bgmid=BGBDT_Sound_PlaySound("sound/pi0_amb0",
//		64, BGBDT_SNDATT_NONE, BGBDT_SNDFL_LOOP);
}

void isotest_menopt_newgame(FRGL_MenuInfo *menu, FRGL_MenuItem *mitm)
{
	isotest_newgame_init();
}

int main_startup(int argc, char *argv[])
{
//	BGBDT_IsoTile *tile;
	BGBDT_SndListen *mixl, *mixr;
	BGBDT_SndMixChan *mixchn;
	BGBDT_SndSampler *mixsmp;
	FRGL_MenuInfo *menu;
	FRGL_MenuItem *mitm;

	char *s0, *s1;
	dtcClass clz;
	float f, g;
	int xs, ys;
	int k0, k1, k2, k3, k4;
	int i, j, k;

	Tex_Init2();
	GfxFont_Init();
	Con_Init();
//	ConCmds_Init();

	SoundDev_Init();

//	NET_InitLow();
//	BTEIFGL_NET_Init();

	main_loadscript("isotest.cfg");
	
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

//	mitm=FRGL_GetMenuItem(menu, 1, "ldgame");
//	mitm->title=frgl_strdup("Load Game");
//	mitm=FRGL_GetMenuItem(menu, 2, "svgame");
//	mitm->title=frgl_strdup("Save Game");

	isotest_eb2d_cls=BGBDTC_LookupClassQName("EntityBase2D");
	isotest_eb2d_fi_origin=BGBDTC_LookupClassSlotName(
		isotest_eb2d_cls, "origin");
	isotest_eb2d_fi_angle=BGBDTC_LookupClassSlotName(
		isotest_eb2d_cls, "angle");

	isotest_npcdiag_cls=BGBDTC_LookupClassQName("NpcDialogBox");

	mixl=BGBDT_Sound_GetListener(0);
	mixr=BGBDT_Sound_GetListener(1);
	mixl->rate=44100;
	mixr->rate=44100;

	isotest_newgame_init();
	
	return(0);
}

int istest_player_issolid(int tt)
{
	if(!tt)
		return(0);
	if(tt&0x40)
		return(0);

	if((tt==0x04) && isotest_isback)
		return(0);
	if((tt==0x05) && !isotest_isback)
		return(0);

	return(1);
}

int istest_player_dophys2(double dt)
{
	float mins[3], maxs[3];
	float io[3], iv[4], fw[2], pt[4], f, g, v;
	float im[16], im1[16], *imv;
	float px, py, rad;
	float dv, d;
//	int tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8;
	int ttdir[16]=
		{   0,10,  0,-10, -10,  0, 10,  0, 
		  -10,10, 10, 10, -10,-10, 10,-10};
	int ttd[8];
	int ott, tt, tts, ett;
	int i, j, k, l;
	
	if(isotest_player_noclip)
		return(0);

	IsoTest_EntSetRadius(isotest_player, 7);
//	IsoTest_EntSetRadius(isotest_player, 12);

#if 0
	if(isotest_vel[0]<-256)
		isotest_vel[0]=-256;
	if(isotest_vel[0]>256)
		isotest_vel[0]=256;

	if(isotest_vel[1]<-256)
		isotest_vel[1]=-256;
	if(isotest_vel[1]>256)
		isotest_vel[1]=256;
#endif

#if 1
	if(isotest_vel[0]<-512)
		isotest_vel[0]=-512;
	if(isotest_vel[0]>512)
		isotest_vel[0]=512;

	if(isotest_vel[1]<-512)
		isotest_vel[1]=-512;
	if(isotest_vel[1]>512)
		isotest_vel[1]=512;
#endif

	V3F_COPY(isotest_org, pt);
	V3F_ADDSCALE(isotest_org, isotest_vel,
		dt, isotest_org);

	rad=IsoTest_EntGetRadius(isotest_player);
//	rad=16;

	tt=IsoTile_CheckCollideMove(isotest_map, isotest_player,
		vec2(isotest_org[0], isotest_org[1]), rad);

	ett=IsoTile_CheckCollideMove(isotest_map, isotest_player,
		vec2(isotest_org[0], isotest_org[1]), rad+8);
	
	if(tt&0x40)
	{
		if(tt==0x40)
		{
			isotest_loadmap(iso_nextmap);
			return(0);
		}

		if(tt==0x44)
			isotest_isback=0;
		if(tt==0x45)
			isotest_isback=1;
		return(0);
	}
	
	if((tt==0x04) && isotest_isback)
		return(0);
	if((tt==0x05) && !isotest_isback)
		return(0);
	
	if((tt==0x03) || (tt==0x04) || (tt==0x05) ||
		(ett==0x03) || (ett==0x04) || (ett==0x05))
	{
		isotest_onground=0;
		isotest_isball=1;
	}else
	{
//		isotest_vel[1]-=9.8*32*frgl_state->dt_f;
//		isotest_vel[1]-=15*32*dt;
	}

//	isotest_vel[1]-=15*32*dt;
	isotest_vel[1]-=9.8*32*dt;
	
	ott=tt;
	
	if(tt)
	{
//		if((tt==2) && (isotest_vel[1]>64) &&
//			((int)(isotest_org[0]/8)==(int)(pt[0]/8)))
		if((tt==2) && (isotest_vel[1]>0))
		{
			return(0);
		}

		if(isotest_isball && (tt!=0x01) && (tt!=0x02))
//		if(1)
		{

			for(i=0; i<8; i++)
			{
				ttd[i]=IsoTile_CheckCollideMove(
					isotest_map, isotest_player,
					vec2(isotest_org[0]+ttdir[i*2+0],
						 isotest_org[1]+ttdir[i*2+1]),
						rad);
			}

			j=-1; dv=-1;
			for(i=0; i<8; i++)
			{
				iv[0]=ttdir[i*2+0];
				iv[1]=ttdir[i*2+1];
				V2F_NORMALIZE(iv, iv);
			
				d=	(isotest_vel[0]*iv[0])+
					(isotest_vel[1]*iv[1]);
				if(istest_player_issolid(ttd[i]))
					continue;
				if((j<0) || (d>dv))
				{
					j=i; dv=d;
				}
			}

			if(j>=0)
			{
				f=V2F_LEN(isotest_vel);

//				isotest_org[0]=pt[0];
//				isotest_org[1]=pt[1];

//				isotest_org[0]+=ttdir[j*2+0]*0.75;
//				isotest_org[1]+=ttdir[j*2+1]*0.75;
				isotest_org[0]+=ttdir[j*2+0]*0.8;
				isotest_org[1]+=ttdir[j*2+1]*0.8;

//				isotest_org[0]+=ttdir[j*2+0];
//				isotest_org[1]+=ttdir[j*2+1];

//				isotest_vel[0]+=ttdir[j*2+0];
//				isotest_vel[1]+=ttdir[j*2+1];

				iv[0]=ttdir[j*2+0];
				iv[1]=ttdir[j*2+1];
				V2F_NORMALIZE(iv, iv);

//				isotest_vel[0]+=2*f*iv[0];
//				isotest_vel[1]+=2*f*iv[1];

				isotest_vel[0]+=f*iv[0];
				isotest_vel[1]+=f*iv[1];


//				isotest_vel[0]=ttdir[j*2+0];
//				isotest_vel[1]=ttdir[j*2+1];

				g=V2F_LEN(isotest_vel)+0.00001;
				V2F_SCALE(isotest_vel, (f/g), isotest_vel);
				return(0);
			}

#if 0
//			if((fabs(isotest_vel[1])>fabs(isotest_vel[0]))
//				&& (isotest_vel[0]>=(-8)))
			if(isotest_vel[0]>=(-8))
			{
				tt=IsoTile_CheckCollideMove(
					isotest_map, isotest_player,
					vec2(isotest_org[0]+10, isotest_org[1]), rad);
				if(!istest_player_issolid(tt))
				{
					f=V2F_LEN(isotest_vel);
					isotest_org[0]+=8;
					isotest_vel[0]+=32;
//					if(isotest_vel[0]<0)
//						isotest_vel[0]=-2*isotest_vel[0];
					g=V2F_LEN(isotest_vel)+0.001;
					V2F_SCALE(isotest_vel, (f/g), isotest_vel);
					return(0);
				}
			}

//			if((fabs(isotest_vel[1])>fabs(isotest_vel[0]))
//				&& (isotest_vel[0]<=8))
			if(isotest_vel[0]<=8)
			{
				tt=IsoTile_CheckCollideMove(
					isotest_map, isotest_player,
					vec2(isotest_org[0]-10, isotest_org[1]), rad);
				if(!istest_player_issolid(tt))
				{
					f=V2F_LEN(isotest_vel);
					isotest_org[0]-=8;
					isotest_vel[0]-=32;
//					if(isotest_vel[0]>0)
//						isotest_vel[0]=-2*isotest_vel[0];
					g=V2F_LEN(isotest_vel)+0.001;
					V2F_SCALE(isotest_vel, (f/g), isotest_vel);
					return(0);
				}
			}

//			if((fabs(isotest_vel[0])>fabs(isotest_vel[1]))
//				&& (isotest_vel[1]<=8))
			if(isotest_vel[1]<=8)
			{
				tt=IsoTile_CheckCollideMove(
					isotest_map, isotest_player,
					vec2(isotest_org[0], isotest_org[1]-10), rad);
				if(!istest_player_issolid(tt))
				{
					f=V2F_LEN(isotest_vel);
					isotest_org[1]-=8;
					isotest_vel[1]-=32;
//					if(isotest_vel[1]>0)
//						isotest_vel[1]=-2*isotest_vel[1];
					g=V2F_LEN(isotest_vel)+0.001;
					V2F_SCALE(isotest_vel, (f/g), isotest_vel);
					return(0);
				}
			}

//			if((fabs(isotest_vel[0])>fabs(isotest_vel[1]))
//				&& (isotest_vel[1]>=(-8)))
			if(isotest_vel[1]>=(-8))
			{
				tt=IsoTile_CheckCollideMove(
					isotest_map, isotest_player,
					vec2(isotest_org[0], isotest_org[1]+10), rad);
				if(!istest_player_issolid(tt))
				{
					f=V2F_LEN(isotest_vel);
					isotest_org[1]+=10;
					isotest_vel[1]+=32;
//					if(isotest_vel[1]<0)
//						isotest_vel[1]=-2*isotest_vel[1];
					g=V2F_LEN(isotest_vel)+0.0001;
					V2F_SCALE(isotest_vel, (f/g), isotest_vel);
					return(0);
				}
			}
#endif
		}

		tt=IsoTile_CheckCollideMove(
			isotest_map, isotest_player,
			vec2(isotest_org[0], pt[1]), rad);
		if(!istest_player_issolid(tt))
		{
			isotest_org[1]=pt[1];
//			if(isotest_vel[1]<0)
			if((isotest_vel[1]<0) &&
				(fabs(isotest_vel[1])>(0.5*fabs(isotest_vel[0]))))
			{
				if(fabs(isotest_vel[1])>(2*fabs(isotest_vel[0])))
					isotest_isball=0;
			
				isotest_vel[1]=0;
				isotest_onground=1;
			}

			if((ott==0x03) || (ott==0x04) || (ott==0x05))
			{
				isotest_onground=0;
				isotest_isball=1;
			}

			return(0);
		}
		
		tt=IsoTile_CheckCollideMove(
			isotest_map, isotest_player,
			vec2(isotest_org[0], pt[1]+12), rad);
		if(!istest_player_issolid(tt))
		{
			f=V2F_LEN(isotest_vel);

//			isotest_org[1]=pt[1]+12;
			isotest_org[1]=pt[1]+8;
			isotest_vel[1]+=12;

			g=V2F_LEN(isotest_vel)+0.0001;
			V2F_SCALE(isotest_vel, (f/g), isotest_vel);

			return(0);
		}
		
		tt=IsoTile_CheckCollideMove(
			isotest_map, isotest_player,
			vec2(isotest_org[0], pt[1]+28), rad);

		if(!istest_player_issolid(tt))
		{
			f=V2F_LEN(isotest_vel);

//			isotest_org[1]=pt[1]+28;
			isotest_org[1]=pt[1]+16;
			isotest_vel[1]+=28;

			g=V2F_LEN(isotest_vel)+0.0001;
			V2F_SCALE(isotest_vel, (f/g), isotest_vel);

			return(0);
		}
				
		tt=IsoTile_CheckCollideMove(isotest_map, isotest_player,
			vec2(pt[0], isotest_org[1]), rad);

		if(!istest_player_issolid(tt))
		{
			isotest_org[0]=pt[0];
			isotest_vel[0]=0;
			return(0);
		}


		tts=IsoTile_CheckCollideMove(isotest_map, isotest_player,
			vec2(pt[0], pt[1]), rad);
		if(!istest_player_issolid(tts))
		{
			V3F_COPY(pt, isotest_org);
			isotest_iswalk=0;
			isotest_vel[0]=0;
			isotest_vel[1]=0;
			return(0);
		}
	}

	return(0);
}

int istest_player_dophys(double dt)
{
	static double accDt;
	static const double step=1.0/240;
	
	accDt+=dt;
	while(accDt>=step)
	{
		istest_player_dophys2(step);
		accDt-=step;
	}
	return(0);
}

int main_handle_input()
{
	char buf[64];
	float mins[3], maxs[3];
	float io[3], iv[4], fw[2], pt[4], f, g, v;
	float im[16], im1[16], *imv;
	
	dtVal tent;

	int hituse;
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

	if(!dtvNullP(isotest_diagbox))
	{
		hituse=0;
		kcur=frgl_state->keys;
		while(*kcur)
		{
			switch(*kcur)
			{
			case K_DEL:
			case K_ESC:
				isotest_diagbox=DTV_NULL;
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
			IsoTile_CallDiagImpulse(isotest_diagbox, hituse);
		}
		return(0);
	}

	if(isotest_invopen || isotest_isdead)
	{
		hituse=0;
		kcur=frgl_state->keys;

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

		while(*kcur)
		{
			switch(*kcur)
			{
			case K_ESC:
				if(isotest_invopen)
				{
					isotest_invopen=0;
					break;
				}
				FRGL_SetMenuActive("main");
				break;
			case K_DEL:
//			case K_ESC:
			case K_TAB:
				isotest_invopen=0;
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
		return(0);
	}

	hituse=0;
	kcur=frgl_state->keys;
	while(*kcur)
	{
		switch(*kcur)
		{
		case K_ESC:
			FRGL_SetMenuActive("main");
			break;

		case K_DEL:
//			BTEWS_DeleteNode(btews_grid,
//				btews_grid->mgx, btews_grid->mgy);
			break;		
		case K_ENTER:
			hituse=1;
			break;
		case K_TAB:
			isotest_invopen=1;
			break;
		default:
			break;
		}
		kcur++;
	}

	if(frgl_state->dt_f>0.25)
		return(0);

	V3F_COPY(isotest_org, pt);

	isotest_iswalk=0;
	if(isotest_player_noclip)
	{
		if(FRGL_KeyDown(K_LEFTARROW))
		{
	//		isotest_px-=100*frgl_state->dt_f;
			V3F_ADDSCALE(isotest_org,
				isotest_rot+0, -256*frgl_state->dt_f,
				isotest_org);
			
			if(!FRGL_KeyDown(K_UPARROW) &&
				!FRGL_KeyDown(K_DOWNARROW))
					isotest_pldir=0;
			isotest_iswalk=1;
		}

		if(FRGL_KeyDown(K_RIGHTARROW))
		{
	//		isotest_px+=100*frgl_state->dt_f;
			V3F_ADDSCALE(isotest_org,
				isotest_rot+0, 256*frgl_state->dt_f,
				isotest_org);

			if(!FRGL_KeyDown(K_UPARROW) &&
				!FRGL_KeyDown(K_DOWNARROW))
					isotest_pldir=1;
			isotest_iswalk=1;
		}

		if(FRGL_KeyDown(K_UPARROW))
		{
			f=isotest_org[2];
	//		isotest_py+=100*frgl_state->dt_f;
			V3F_ADDSCALE(isotest_org,
				isotest_rot+3, 256*frgl_state->dt_f,
				isotest_org);
			V3F_ADDSCALE(isotest_org,
				isotest_rot+6, 256*frgl_state->dt_f,
				isotest_org);
			isotest_org[2]=f;

			if(!FRGL_KeyDown(K_LEFTARROW) &&
				!FRGL_KeyDown(K_RIGHTARROW))
					isotest_pldir=2;
			isotest_iswalk=1;
		}

		if(FRGL_KeyDown(K_DOWNARROW))
		{
			f=isotest_org[2];
	//		isotest_py-=100*frgl_state->dt_f;
			V3F_ADDSCALE(isotest_org,
				isotest_rot+3, -256*frgl_state->dt_f,
				isotest_org);
			V3F_ADDSCALE(isotest_org,
				isotest_rot+6, -256*frgl_state->dt_f,
				isotest_org);
			isotest_org[2]=f;

			if(!FRGL_KeyDown(K_LEFTARROW) &&
				!FRGL_KeyDown(K_RIGHTARROW))
					isotest_pldir=3;
			isotest_iswalk=1;
		}
		
		isotest_vel[0]=0;
		isotest_vel[1]=0;
	}else
	{
		if(FRGL_KeyDown(K_LEFTARROW))
		{
	//		isotest_px-=100*frgl_state->dt_f;
//			V3F_ADDSCALE(isotest_vel,
//				isotest_rot+0, -256*frgl_state->dt_f,
//				isotest_vel);

			if(isotest_onground)
			{
				isotest_vel[0]-=256*frgl_state->dt_f;
			}else
			{
				isotest_vel[0]-=128*frgl_state->dt_f;
			}
			
			isotest_pldir=0;
			isotest_iswalk=1;
		}else if(FRGL_KeyDown(K_RIGHTARROW))
		{
	//		isotest_px+=100*frgl_state->dt_f;
//			V3F_ADDSCALE(isotest_vel,
//				isotest_rot+0, 256*frgl_state->dt_f,
//				isotest_vel);

			if(isotest_onground)
			{
				isotest_vel[0]+=256*frgl_state->dt_f;
			}else
			{
				isotest_vel[0]+=128*frgl_state->dt_f;
			}

			isotest_pldir=1;
			isotest_iswalk=1;
		}else
		{
//			if(isotest_onground)
			if(!isotest_isball)
			{
//				f=1.0-16*frgl_state->dt_f;
				f=1.0-8*frgl_state->dt_f;
				if(f<0)f=0;
				isotest_vel[0]*=f;
			}
		}

		if(FRGL_KeyDown(K_UPARROW))
		{
			if(isotest_onground)
			{
				isotest_vel[1]=256;
//				isotest_vel[1]=512;
//				isotest_vel[1]=320;
				isotest_onground=0;
				isotest_isball=1;
			}
		}

//		isotest_vel[1]-=9.8*32*frgl_state->dt_f;
//		isotest_vel[1]-=15*32*frgl_state->dt_f;
//		V3F_ADDSCALE(isotest_org, isotest_vel,
//			frgl_state->dt_f, isotest_org);
	}

#if 0
	if(FRGL_KeyDown(K_NUMPAD4))
	{
		isotest_ang[0]-=100*frgl_state->dt_f;
	}

	if(FRGL_KeyDown(K_NUMPAD6))
	{
		isotest_ang[0]+=100*frgl_state->dt_f;
	}

	if(FRGL_KeyDown(K_NUMPAD8))
	{
		isotest_ang[1]+=100*frgl_state->dt_f;
	}

	if(FRGL_KeyDown(K_NUMPAD2))
	{
		isotest_ang[1]-=100*frgl_state->dt_f;
	}
#endif

	istest_player_dophys(frgl_state->dt_f);

	IsoTest_EntSetOrigin(isotest_player,
		vec2(isotest_org[0], isotest_org[1]));

	if(hituse)
	{
		tent=IsoTile_QueryCollideMove(isotest_map, isotest_player,
			vec2(isotest_org[0], isotest_org[1]),
			IsoTest_EntGetRadius(isotest_player)+4);

		if(dtvTrueP(tent))
		{
			IsoTile_CallEntityUse(tent, isotest_player);
		}
	}

	return(0);
}

int main_body()
{
	static float frm_dtf[256];
	static byte frm_rov;
	
	char tb[1024];
	float io[4], v0[4], im[16];

	BTEWS_WireNode *ncur;
	char *s;
	u32 imp0, imp1;
	u32 act, mask, pin, pout;
	float cr, cg, cb;
	double x0, x1, y0, y1;
	double f, g;
	int mx, my, mgx, mgy;
	int tex, txs, tys;
	int i, j, k, l;

	mx=frgl_state->mx;
	my=-frgl_state->my;

	main_handle_input();
//	BTEIFGL_NET_Update(frgl_state->dt_f);
	main_scriptupdate(frgl_state->dt_f);
	isotest_worldtime+=frgl_state->dt_f;

	IsoTile_UpdateMap(isotest_map);

//	BTEWS_UpdateGrid(btews_grid, frgl_state->dt_f);

//	io[0]=isotest_org[0];
//	io[1]=isotest_org[1];
//	io[2]=isotest_org[2];

//	V3F_ADDSCALE(isotest_org,
//		isotest_rot+3, -64, io);
//	V3F_ADDSCALE(io,
//		isotest_rot+6, -64, io);

	io[0]=isotest_org[0];
	io[1]=isotest_org[1]+128;

	io[2]=256;

	Mat3F_Identity(isotest_rot);
	
	v0[0]=1; v0[1]=0; v0[2]=0;
	Mat3F_Rotate3Matrix(isotest_rot,
		v0, isotest_ang[1]*(M_PI/180), isotest_rot);

	v0[0]=0; v0[1]=0; v0[2]=1;
	Mat3F_Rotate3Matrix(isotest_rot,
		v0, isotest_ang[0]*(M_PI/180), isotest_rot);


	BGBDT_Sound_SetListenOrg(vec3(
		isotest_org[0], isotest_org[1], 0));
	BGBDT_Sound_SetListenVecRt(vec3(
		isotest_rot[0], isotest_rot[1], isotest_rot[2]));
	BGBDT_Sound_SetListenVecFw(vec3(
		isotest_rot[3], isotest_rot[4], isotest_rot[5]));
	BGBDT_Sound_SetListenVecUp(vec3(
		isotest_rot[6], isotest_rot[7], isotest_rot[8]));

	BGBDT_Sound_MixTime(frgl_state->dt_f);


	FRGL_Setup3D(io, isotest_rot);
//	FRGL_Setup3D(isotest_org, isotest_rot);

	frglColor4f(1, 1, 1, 1);
	frglDisable(GL_CULL_FACE);
	isotile_drawmap(isotest_map);

	isotest_drawplayer();

//	tex=Tex_LoadFile("images/openhole0.png", NULL, NULL);

	FRGL_Setup2D();

	frglColor4f(1,1,1,1);
	IsoTile_DrawUseBackground();

	if(!dtvNullP(isotest_diagbox))
	{
		IsoTile_DrawDialog(isotest_diagbox);
	}else
	{
		if(isotest_diag_sndid>0)
		{
			BGBDT_Sound_DeleteMixChan(isotest_diag_sndid);
			isotest_diag_sndid=0;
		}
	}

	if(isotest_invopen)
	{
		IsoTile_DrawInventory();
	}

	if(isotest_isdead)
	{
		IsoTile_DrawDeadScreen();
	}

	FRGL_DrawMenu();

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

	sprintf(tb, "Cur=%.2fHz, Avg=%.2f, Min=%.2f, Max=%.2f "
			"Txy=%d,%d T=%d:%02d",
		1.0/frgl_state->dt_f, 1.0/g, 1.0/x1, 1.0/x0,
		(int)(isotest_org[0]/8), (int)(isotest_org[1]/8),
		(((int)isotest_worldtime)/60),
		(((int)isotest_worldtime)%60));
	GfxFont_DrawString(tb, -320, 384-16, 8, 8, 0, 255, 0, 255);

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
