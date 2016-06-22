/*
IsoTest is a simplistic "test game" for the engine core.

As-is, it is "actually pretty bad" in many regards (ex: the way it currently does rendering is terrible/barely works, but was less effort than doing it properly).
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
dtcField isotest_npcdiag_fi_options;

dtcField isotest_npcdiag_fi_impulse;

BGBDT_IsoMap *isotest_map;

float isotest_org[3];
float isotest_ang[3];
float isotest_rot[9];
byte isotest_pldir;
byte isotest_iswalk;
byte isotest_isdead;
byte isotest_player_noclip;
byte isotest_player_undying;
int isotest_wrlflag;

dtVal isotest_player;
dtVal isotest_diagbox;
int isotest_diag_pchars;
int isotest_diag_lpchars;
int isotest_diag_sndid;

int isotest_snd_bgmid;

double isotest_diag_ptime;
double isotest_worldtime;

int isotest_invopen;
int isotest_invslot[8*16];


MAIN_EXPORT void IsoTest_SetDialog(dtVal dbox)
{
	isotest_diagbox=dbox;
	isotest_diag_pchars=0;
	isotest_diag_ptime=frgl_state->time_f;
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

int main_prestart(int argc, char *argv[])
{
	char tb[256];
	int i, j, k;

	GfxDrv_SetDefaults("BTDT IsoTest", 1024, 768);
	FRGL_CvarSetDefault("hide_os_cursor", "1");

	Con_ExecCmdBuf("config.cfg");
	
	for(i=0; i<10; i++)
	{
		sprintf(tb, "resource/base%d.bpk", i);
		VfMount(tb, "/", "btpack", "");
	}

	return(0);
}

#if 0
void btews_vccgnd_Draw(BTEWS_WireNode *self,
	float x0, float y0, float x1, float y1)
{
	char *s;
	int tex;

	self->wfl|=BTEWS_WFL_JOIN;

	btews_wire_Draw(self, x0, y0, x1, y1);

	s="images/node_vcc.png";
	if(self->vc0==0)
		s="images/node_gnd.png";
	if(self->vc0<0)
		s="images/node_vgnd.png";

	if((self->wfl&BTEWS_WFL_DGTL_ALL)==BTEWS_WFL_DGTL_VCC)
		s="images/node_vhi.png";

	tex=Tex_LoadFile(s, NULL, NULL);

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
}
#endif

int main_loadscript(char *def)
{
//	static char *mods[]={
//		"bstest.fib",
//		NULL
//	};

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
//	prn=BGBDT_MM_NewStringPrinter(tb, 16383);
//	prn=BGBDT_MM_NewStringPrinter(tb, (1<<18)-1);

	prn=BGBDT_MM_NewConsolePrinter();
	ctx->dbgprn=prn;

	BS2C_CompileModuleList(ctx, NULL, mods);

	if(ctx->ncerr || ctx->ncfatal)
	{
		return(-1);
	}

//	printf("%s\n", tb);

	tbuf=frgl_malloc(1<<20);

	i=BS2C_FlattenImage(ctx, tbuf, 1<<20);
	vf_storefile("isotest0.bsi", tbuf, i);

	isotest_img=BS2I_DecodeImageBuffer(tbuf, i);
	vi=BS2I_ImageGetMain(isotest_img, NULL);

	frgl_free(tbuf);

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

int main_spawnworld()
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;

	if(!isotest_vi_spawnworld)
		return(-1);

	args=targs;
//	args[0].d=dt;

	IsoTile_SetGlobalA("world_entity", isotest_map->entarr);
	IsoTile_SetGlobalI("world_max_entity", isotest_map->nents);

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, isotest_vi_spawnworld, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
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

//	isotest_eb2d_cls=BGBDTC_LookupClassQName("EntityBase2D");
//	isotest_eb2d_fi_origin=BGBDTC_LookupClassSlotName(
//		isotest_eb2d_cls, "origin");

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

int IsoTile_SetGlobalA(char *cname, dtVal v)
{
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		isotest_img, cname);
	if(!vi)return(-1);
	BSVM2_Interp_SetGlobalA(vi, v);
}

int IsoTile_SetGlobalI(char *cname, int v)
{
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		isotest_img, cname);
	if(!vi)return(-1);
	BSVM2_Interp_SetGlobalI(vi, v);
}

int IsoTile_DrawTile(BGBDT_IsoMap *map,
	BGBDT_IsoTile *tile)
{
	float x0, y0, x1, y1;
	float x2, y2, x3, y3;
	float x2a, x2b, y2a, y2b;
	float z0, z1;
	char *s;
	int tex;

	x0=tile->x*64; x1=x0+64; x2=x0+32;
	y0=tile->y*64; y1=y0+64; y2=y0+32;
	z0=0; z1=64;
	
	x2a=x2-4;	x2b=x2+4;
	y2a=y2-4;	y2b=y2+4;

	s=tile->tex_floor;
	if(isotest_wrlflag&1)
		s=tile->tex_floor_alt;
	tex=Tex_LoadFile(s, NULL, NULL);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
//	frglTexCoord2f(0, 0);	frglVertex3f(x0, y0, z0);
//	frglTexCoord2f(1, 0);	frglVertex3f(x1, y0, z0);
//	frglTexCoord2f(1, 1);	frglVertex3f(x1, y1, z0);
//	frglTexCoord2f(0, 1);	frglVertex3f(x0, y1, z0);

	frglTexCoord2f(0, 1);	frglVertex3f(x0, y1, z0);
	frglTexCoord2f(1, 1);	frglVertex3f(x1, y1, z0);
	frglTexCoord2f(1, 0);	frglVertex3f(x1, y0, z0);
	frglTexCoord2f(0, 0);	frglVertex3f(x0, y0, z0);
	frglEnd();
	
	if(tile->tex_wall_xz)
	{
		s=tile->tex_wall_xz;
		if(isotest_wrlflag&1)
			s=tile->tex_wall_xz_alt;

		tex=Tex_LoadFile(s, NULL, NULL);

		if((tile->fl&BGBDT_ISOTFL_PNX)==BGBDT_ISOTFL_PNX)
		{
			frglEnableTexture2D();
			frglBindTexture2D(tex);
			frglBegin(GL_QUADS);

#if 0
			frglTexCoord2f(0, 0);
			frglVertex3f(x0, y2, z0);
			frglTexCoord2f(1, 0);
			frglVertex3f(x1, y2, z0);
			frglTexCoord2f(1, 1);
			frglVertex3f(x1, y2, z1);
			frglTexCoord2f(0, 1);
			frglVertex3f(x0, y2, z1);
#endif

#if 1
//			frglTexCoord2f(0, 0);	frglVertex3f(x0, y2a, z0);
//			frglTexCoord2f(1, 0);	frglVertex3f(x1, y2a, z0);
//			frglTexCoord2f(1, 1);	frglVertex3f(x1, y2a, z1);
//			frglTexCoord2f(0, 1);	frglVertex3f(x0, y2a, z1);

			frglTexCoord2f(0, 1);	frglVertex3f(x0, y2a, z1);
			frglTexCoord2f(1, 1);	frglVertex3f(x1, y2a, z1);
			frglTexCoord2f(1, 0);	frglVertex3f(x1, y2a, z0);
			frglTexCoord2f(0, 0);	frglVertex3f(x0, y2a, z0);

			frglTexCoord2f(0, 0);	frglVertex3f(x0, y2b, z0);
			frglTexCoord2f(1, 0);	frglVertex3f(x1, y2b, z0);
			frglTexCoord2f(1, 1);	frglVertex3f(x1, y2b, z1);
			frglTexCoord2f(0, 1);	frglVertex3f(x0, y2b, z1);

//			frglTexCoord2f(0, 1);	frglVertex3f(x0, y2b, z1);
//			frglTexCoord2f(1, 1);	frglVertex3f(x1, y2b, z1);
//			frglTexCoord2f(1, 0);	frglVertex3f(x1, y2b, z0);
//			frglTexCoord2f(0, 0);	frglVertex3f(x0, y2b, z0);

			frglTexCoord2f(0, 0.44);	frglVertex3f(x0, y2b, z1);
			frglTexCoord2f(1, 0.44);	frglVertex3f(x1, y2b, z1);
			frglTexCoord2f(1, 0.56);	frglVertex3f(x1, y2a, z1);
			frglTexCoord2f(0, 0.56);	frglVertex3f(x0, y2a, z1);
#endif

			frglEnd();
		}else if(tile->fl&BGBDT_ISOTFL_NX)
		{
			frglEnableTexture2D();
			frglBindTexture2D(tex);
			frglBegin(GL_QUADS);

#if 0
			frglTexCoord2f(0, 0);
			frglVertex3f(x0, y2, z0);
			frglTexCoord2f(1, 0);
			frglVertex3f(x2, y2, z0);
			frglTexCoord2f(1, 1);
			frglVertex3f(x2, y2, z1);
			frglTexCoord2f(0, 1);
			frglVertex3f(x0, y2, z1);
#endif

#if 1
//			frglTexCoord2f(0, 0);	frglVertex3f(x0, y2a, z0);
//			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y2a, z0);
//			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y2a, z1);
//			frglTexCoord2f(0, 1);	frglVertex3f(x0, y2a, z1);

			frglTexCoord2f(0, 1);	frglVertex3f(x0, y2a, z1);
			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y2a, z1);
			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y2a, z0);
			frglTexCoord2f(0, 0);	frglVertex3f(x0, y2a, z0);

			frglTexCoord2f(0, 0);	frglVertex3f(x0, y2b, z0);
			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y2b, z0);
			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y2b, z1);
			frglTexCoord2f(0, 1);	frglVertex3f(x0, y2b, z1);

			frglTexCoord2f(0, 0.44);	frglVertex3f(x0, y2b, z1);
			frglTexCoord2f(1, 0.44);	frglVertex3f(x2b, y2b, z1);
			frglTexCoord2f(1, 0.56);	frglVertex3f(x2b, y2a, z1);
			frglTexCoord2f(0, 0.56);	frglVertex3f(x0, y2a, z1);
			
			if(!(tile->fl&BGBDT_ISOTFL_PNY))
			{
//				frglTexCoord2f(0.44, 0);	frglVertex3f(x2b, y2a, z0);
//				frglTexCoord2f(0.56, 0);	frglVertex3f(x2b, y2b, z0);
//				frglTexCoord2f(0.56, 1);	frglVertex3f(x2b, y2b, z1);
//				frglTexCoord2f(0.44, 1);	frglVertex3f(x2b, y2a, z1);

				frglTexCoord2f(0.44, 1);	frglVertex3f(x2b, y2a, z1);
				frglTexCoord2f(0.56, 1);	frglVertex3f(x2b, y2b, z1);
				frglTexCoord2f(0.56, 0);	frglVertex3f(x2b, y2b, z0);
				frglTexCoord2f(0.44, 0);	frglVertex3f(x2b, y2a, z0);
			}
#endif

			frglEnd();
		}else if(tile->fl&BGBDT_ISOTFL_PX)
		{
			frglEnableTexture2D();
			frglBindTexture2D(tex);
			frglBegin(GL_QUADS);

#if 0
			frglTexCoord2f(0, 0);
			frglVertex3f(x2, y2, z0);
			frglTexCoord2f(1, 0);
			frglVertex3f(x1, y2, z0);
			frglTexCoord2f(1, 1);
			frglVertex3f(x1, y2, z1);
			frglTexCoord2f(0, 1);
			frglVertex3f(x2, y2, z1);
#endif

#if 1
//			frglTexCoord2f(0, 0);	frglVertex3f(x2a, y2a, z0);
//			frglTexCoord2f(1, 0);	frglVertex3f(x1, y2a, z0);
//			frglTexCoord2f(1, 1);	frglVertex3f(x1, y2a, z1);
//			frglTexCoord2f(0, 1);	frglVertex3f(x2a, y2a, z1);

			frglTexCoord2f(0, 1);	frglVertex3f(x2a, y2a, z1);
			frglTexCoord2f(1, 1);	frglVertex3f(x1, y2a, z1);
			frglTexCoord2f(1, 0);	frglVertex3f(x1, y2a, z0);
			frglTexCoord2f(0, 0);	frglVertex3f(x2a, y2a, z0);

			frglTexCoord2f(0, 0);	frglVertex3f(x2a, y2b, z0);
			frglTexCoord2f(1, 0);	frglVertex3f(x1, y2b, z0);
			frglTexCoord2f(1, 1);	frglVertex3f(x1, y2b, z1);
			frglTexCoord2f(0, 1);	frglVertex3f(x2a, y2b, z1);

			frglTexCoord2f(0, 0.44);	frglVertex3f(x2a, y2b, z1);
			frglTexCoord2f(1, 0.44);	frglVertex3f(x1, y2b, z1);
			frglTexCoord2f(1, 0.56);	frglVertex3f(x1, y2a, z1);
			frglTexCoord2f(0, 0.56);	frglVertex3f(x2a, y2a, z1);

			if(!(tile->fl&BGBDT_ISOTFL_PNY))
			{
				frglTexCoord2f(0.44, 0);	frglVertex3f(x2a, y2a, z0);
				frglTexCoord2f(0.56, 0);	frglVertex3f(x2a, y2b, z0);
				frglTexCoord2f(0.56, 1);	frglVertex3f(x2a, y2b, z1);
				frglTexCoord2f(0.44, 1);	frglVertex3f(x2a, y2a, z1);
			}
#endif

			frglEnd();
		}
	}

	if(tile->tex_wall_yz)
	{
		s=tile->tex_wall_yz;
		if(isotest_wrlflag&1)
			s=tile->tex_wall_yz_alt;

		tex=Tex_LoadFile(s, NULL, NULL);

		if((tile->fl&BGBDT_ISOTFL_PNY)==BGBDT_ISOTFL_PNY)
		{
			frglEnableTexture2D();
			frglBindTexture2D(tex);
			frglBegin(GL_QUADS);
#if 0
			frglTexCoord2f(0, 0);
			frglVertex3f(x2, y0, z0);
			frglTexCoord2f(1, 0);
			frglVertex3f(x2, y1, z0);
			frglTexCoord2f(1, 1);
			frglVertex3f(x2, y1, z1);
			frglTexCoord2f(0, 1);
			frglVertex3f(x2, y0, z1);
#endif

#if 1
			frglTexCoord2f(0, 0);	frglVertex3f(x2a, y0, z0);
			frglTexCoord2f(1, 0);	frglVertex3f(x2a, y1, z0);
			frglTexCoord2f(1, 1);	frglVertex3f(x2a, y1, z1);
			frglTexCoord2f(0, 1);	frglVertex3f(x2a, y0, z1);

//			frglTexCoord2f(0, 1);	frglVertex3f(x2a, y0, z1);
//			frglTexCoord2f(1, 1);	frglVertex3f(x2a, y1, z1);
//			frglTexCoord2f(1, 0);	frglVertex3f(x2a, y1, z0);
//			frglTexCoord2f(0, 0);	frglVertex3f(x2a, y0, z0);

//			frglTexCoord2f(0, 0);	frglVertex3f(x2b, y0, z0);
//			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y1, z0);
//			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y1, z1);
//			frglTexCoord2f(0, 1);	frglVertex3f(x2b, y0, z1);

			frglTexCoord2f(0, 1);	frglVertex3f(x2b, y0, z1);
			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y1, z1);
			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y1, z0);
			frglTexCoord2f(0, 0);	frglVertex3f(x2b, y0, z0);

			frglTexCoord2f(0, 0.44);	frglVertex3f(x2a, y0, z1);
			frglTexCoord2f(1, 0.44);	frglVertex3f(x2a, y1, z1);
			frglTexCoord2f(1, 0.56);	frglVertex3f(x2b, y1, z1);
			frglTexCoord2f(0, 0.56);	frglVertex3f(x2b, y0, z1);
#endif

			frglEnd();
		}else if(tile->fl&BGBDT_ISOTFL_NY)
		{
			frglEnableTexture2D();
			frglBindTexture2D(tex);
			frglBegin(GL_QUADS);

#if 0
			frglTexCoord2f(0, 0);
			frglVertex3f(x2, y0, z0);
			frglTexCoord2f(1, 0);
			frglVertex3f(x2, y2, z0);
			frglTexCoord2f(1, 1);
			frglVertex3f(x2, y2, z1);
			frglTexCoord2f(0, 1);
			frglVertex3f(x2, y0, z1);
#endif

#if 1
			frglTexCoord2f(0, 0);	frglVertex3f(x2a, y0, z0);
			frglTexCoord2f(1, 0);	frglVertex3f(x2a, y2b, z0);
			frglTexCoord2f(1, 1);	frglVertex3f(x2a, y2b, z1);
			frglTexCoord2f(0, 1);	frglVertex3f(x2a, y0, z1);

//			frglTexCoord2f(0, 0);	frglVertex3f(x2b, y0, z0);
//			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y2b, z0);
//			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y2b, z1);
//			frglTexCoord2f(0, 1);	frglVertex3f(x2b, y0, z1);

			frglTexCoord2f(0, 1);	frglVertex3f(x2b, y0, z1);
			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y2b, z1);
			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y2b, z0);
			frglTexCoord2f(0, 0);	frglVertex3f(x2b, y0, z0);

			frglTexCoord2f(0, 0.44);	frglVertex3f(x2a, y0, z1);
			frglTexCoord2f(1, 0.44);	frglVertex3f(x2a, y2b, z1);
			frglTexCoord2f(1, 0.56);	frglVertex3f(x2b, y2b, z1);
			frglTexCoord2f(0, 0.56);	frglVertex3f(x2b, y0, z1);

			if(!(tile->fl&BGBDT_ISOTFL_PNX))
			{
				frglTexCoord2f(0.44, 0);	frglVertex3f(x2a, y2b, z0);
				frglTexCoord2f(0.56, 0);	frglVertex3f(x2b, y2b, z0);
				frglTexCoord2f(0.56, 1);	frglVertex3f(x2b, y2b, z1);
				frglTexCoord2f(0.44, 1);	frglVertex3f(x2a, y2b, z1);
			}
#endif

			frglEnd();
		}else if(tile->fl&BGBDT_ISOTFL_PY)
		{
			frglEnableTexture2D();
			frglBindTexture2D(tex);
			frglBegin(GL_QUADS);

#if 0
			frglTexCoord2f(0, 0);
			frglVertex3f(x2, y2, z0);
			frglTexCoord2f(1, 0);
			frglVertex3f(x2, y1, z0);
			frglTexCoord2f(1, 1);
			frglVertex3f(x2, y1, z1);
			frglTexCoord2f(0, 1);
			frglVertex3f(x2, y2, z1);
#endif

#if 1
			frglTexCoord2f(0, 0);	frglVertex3f(x2a, y2a, z0);
			frglTexCoord2f(1, 0);	frglVertex3f(x2a, y1, z0);
			frglTexCoord2f(1, 1);	frglVertex3f(x2a, y1, z1);
			frglTexCoord2f(0, 1);	frglVertex3f(x2a, y2a, z1);

//			frglTexCoord2f(0, 0);	frglVertex3f(x2b, y2a, z0);
//			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y1, z0);
//			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y1, z1);
//			frglTexCoord2f(0, 1);	frglVertex3f(x2b, y2a, z1);

			frglTexCoord2f(0, 1);	frglVertex3f(x2b, y2a, z1);
			frglTexCoord2f(1, 1);	frglVertex3f(x2b, y1, z1);
			frglTexCoord2f(1, 0);	frglVertex3f(x2b, y1, z0);
			frglTexCoord2f(0, 0);	frglVertex3f(x2b, y2a, z0);

			frglTexCoord2f(0, 0.44);	frglVertex3f(x2a, y2a, z1);
			frglTexCoord2f(1, 0.44);	frglVertex3f(x2a, y1, z1);
			frglTexCoord2f(1, 0.56);	frglVertex3f(x2b, y1, z1);
			frglTexCoord2f(0, 0.56);	frglVertex3f(x2b, y2a, z1);

			if(!(tile->fl&BGBDT_ISOTFL_PNX))
			{
//				frglTexCoord2f(0.44, 0);	frglVertex3f(x2a, y2a, z0);
//				frglTexCoord2f(0.56, 0);	frglVertex3f(x2b, y2a, z0);
//				frglTexCoord2f(0.56, 1);	frglVertex3f(x2b, y2a, z1);
//				frglTexCoord2f(0.44, 1);	frglVertex3f(x2a, y2a, z1);

				frglTexCoord2f(0.44, 1);	frglVertex3f(x2a, y2a, z1);
				frglTexCoord2f(0.56, 1);	frglVertex3f(x2b, y2a, z1);
				frglTexCoord2f(0.56, 0);	frglVertex3f(x2b, y2a, z0);
				frglTexCoord2f(0.44, 0);	frglVertex3f(x2a, y2a, z0);
			}
#endif

			frglEnd();
		}
	}
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
	org[2]=0;
	flip=0;

	V3F_ADDSCALE(org, isotest_rot+0, -(xs/2), v0);
	V3F_ADDSCALE(org, isotest_rot+0,  (xs/2), v1);
	V3F_COPY(v0, v2);
	V3F_COPY(v1, v3);
	v2[2]+=ys;
	v3[2]+=ys;

	V3F_COPY(isotest_rot+3, fw);
	fw[2]=0;

	tex=Tex_LoadFile(spr, NULL, NULL);

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
}

int IsoTile_DrawEntity(BGBDT_IsoMap *map,
	dtVal ent)
{
	vec2 org, corg;
	char *spr;
	float xs, ys;
	
	corg=vec2(isotest_org[0], isotest_org[1]);
	
	org=IsoTest_EntGetOrigin(ent);

	if(v2dist(org, corg)>1024)
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

int isotile_drawmap(BGBDT_IsoMap *map)
{
	BGBDT_IsoTile *tile;
	dtVal ent;
	int x, y, mx, my, nx, ny;
	int i, j, k, l;

	frglEnable(GL_CULL_FACE);

	frglEnable(GL_ALPHA_TEST);

	x=isotest_org[0]/64.0;
	y=isotest_org[1]/64.0;
	mx=x-16; nx=x+16;
	my=y-16; ny=y+16;

	if(mx<0)mx=0;
	if(my<0)my=0;
	if(nx>map->xs)nx=map->xs;
	if(ny>map->ys)ny=map->ys;

//	frglModelviewMatrix();
//	frglPushMatrix();
	
//	frglTranslatef(mat[4], mat[5], 0);
//	frglScalef(1.0, 0.5, 1);
//	frglRotatef(45, 0, 0, 1);
	
//	for(i=0; i<map->ys; i++)
//		for(j=0; j<map->xs; j++)
	for(i=my; i<ny; i++)
		for(j=mx; j<nx; j++)
	{
		tile=map->tile[i*map->xs+j];
		if(!tile)
			continue;
		
		IsoTile_DrawTile(map, tile);
	}
	
//	frglPopMatrix();

	frglDisable(GL_CULL_FACE);

	l=map->nents;
	for(i=0; i<l; i++)
	{
		ent=dtvArrayGetIndexDtVal(map->entarr, i);
		if(dtvNullP(ent))
			continue;
		IsoTile_DrawEntity(map, ent);
	}

	frglDisable(GL_ALPHA_TEST);
}

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

#if 0
	for(i=0; i<map->nents; i++)
	{
		e0=dtvArrayGetIndexDtVal(map->entarr, i);
		for(j=i+1; j<map->nents; j++)
		{
			e1=dtvArrayGetIndexDtVal(map->entarr, j);
			if(IsoTile_EntCheckCollide(e0, e1))
			{
				IsoTile_CallEntityTouch(e0, e1);
				IsoTile_CallEntityTouch(e1, e0);
			}
		}
	}
#endif
}

dtVal IsoTile_QueryCollideMove(BGBDT_IsoMap *map,
	dtVal self, vec2 org, float rad);

int IsoTile_CheckCollideMove(BGBDT_IsoMap *map,
	dtVal self, vec2 org, float rad)
{
	BGBDT_IsoTile *ti;
	dtVal other;
	
	other=IsoTile_QueryCollideMove(map, self, org, rad);
	if(!dtvNullP(other))
		return(1);
	
	ti=IsoTile_CheckTileCollide(map,
		v2x(org), v2y(org), rad);
	if(ti)
		return(1);
	
	return(0);
}

dtVal IsoTile_QueryCollideMove(BGBDT_IsoMap *map,
	dtVal self, vec2 org, float rad)
{
	dtVal ent;
	vec2 eorg;
	float erad;
	int i, j, k;
	
	for(i=0; i<map->nents; i++)
	{
		ent=dtvArrayGetIndexDtVal(map->entarr, i);
		
		if(dtvEqqP(ent, self))
			continue;
		
		eorg=IsoTest_EntGetOrigin(ent);
		erad=IsoTest_EntGetRadius(ent);

		if(v2dist(eorg, org)<(erad+rad))
			return(ent);
	}
	return(DTV_NULL);
}

MAIN_EXPORT int IsoTile_CheckEntMoveCollidePos(dtVal self)
{
	BGBDT_IsoTile *ti;
	vec2 org;
	dtVal other;
	float rad;
	
	org=IsoTest_EntGetOrigin(self);
	rad=IsoTest_EntGetRadius(self);

	other=IsoTile_QueryCollideMove(isotest_map, self, org, rad);
	if(!dtvNullP(other))
		return(1);
	
	ti=IsoTile_CheckTileCollide(isotest_map,
		v2x(org), v2y(org), rad);
	if(ti)
		return(1);
	
	return(0);
}

int isotest_drawplayer()
{
	float org[4], v0[4], v1[4], v2[4], v3[4];
	int tex, flip;
	int i;
	char *str;

//	V3F_ADDSCALE(isotest_org,
//		isotest_rot+3, 256, org);
//	V3F_ADDSCALE(org,
//		isotest_rot+6, 256, org);

	V3F_COPY(isotest_org, org);
	org[2]=0;

	V3F_ADDSCALE(org, isotest_rot+0, -12, v0);
	V3F_ADDSCALE(org, isotest_rot+0,  12, v1);
	V3F_COPY(v0, v2);
	V3F_COPY(v1, v3);
	v2[2]+=48;
	v3[2]+=48;

//	V3F_ADDSCALE(v0, isotest_rot+6, 64, v2);
//	V3F_ADDSCALE(v1, isotest_rot+6, 64, v3);

	i=((int)(frgl_state->time_f*8))&1;
	switch(isotest_pldir)
	{
	case 0:
		if(isotest_iswalk)
		{
			if(i)
				str="sprites/yazil/yazil_walk1_lf1";
			else
				str="sprites/yazil/yazil_walk2_lf1";
			flip=0;
			break;
		}
		str="sprites/yazil/yazil_lf1";
		flip=0;
		break;
	case 1:
		if(isotest_iswalk)
		{
			if(i)
				str="sprites/yazil/yazil_walk1_lf1";
			else
				str="sprites/yazil/yazil_walk2_lf1";
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
}

int IsoTile_DrawDialog(dtVal dbox)
{
	char tb[4096];
	dtVal opts, a;
	char *face1, *face2, *text;
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
					"sound/box_aa1", 32, 0, BGBDT_SNDFL_LOOP);
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

//	face1=IsoTest_DiagGetFace1(dbox);
//	face2=IsoTest_DiagGetFace2(dbox);
//	text=IsoTest_DiagGetText(dbox);
//	opts=IsoTest_DiagGetOptions(dbox);

	x0=-320; y0=-240;
	x1=320; y1=240;

	x2=-320+4; y2=0+4;
	x3=-80-4; y3=240-4;

//	isotest_diag_pchars=(frgl_state->time_f-isotest_diag_ptime)*24;

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
//			s0=NULL;
//			s0="images/inven/greenbox0";
//			if(!s0)
//				continue;
		
			k=isotest_invslot[i*16+j];

			u0=(k&15)*(1.0/16);
			v0=(15-((k>>4)&15))*(1.0/16);
			u1=u0+(1.0/16);
			v1=v0+(1.0/16);

			x2=x0+j*40+6; x3=x2+32;
			y2=y0+i*40+6; y3=y2+32;

//			tex=Tex_LoadFile(s0, NULL, NULL);

//			frglEnableTexture2D();
//			frglBindTexture2D(tex);
//			frglBegin(GL_QUADS);
			frglTexCoord2f(u0, v0);
			frglVertex2f(x2, y2);
			frglTexCoord2f(u1, v0);
			frglVertex2f(x3, y2);
			frglTexCoord2f(u1, v1);
			frglVertex2f(x3, y3);
			frglTexCoord2f(u0, v1);
			frglVertex2f(x2, y3);
//			frglEnd();
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
}

MAIN_EXPORT int IsoTest_KillPlayer(void)
{
	isotest_isdead=1;
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
int iso_xs, iso_ys;

void isotest_newgame_init()
{
	BGBDT_IsoTile *tile;
	char *s0, *s1;
	int k0, k1, k2, k3, k4, k5, k6, k7;
	int i, j, k, l;

	if(!iso_fbuf)
	{
		iso_fbuf=Tex_LoadFileRaw("isomap/map_floor.png", &iso_xs, &iso_ys);
		iso_wbuf=Tex_LoadFileRaw("isomap/map_wall.png", NULL, NULL);
		iso_ebuf=Tex_LoadFileRaw("isomap/map_spawn.png", NULL, NULL);

		isotest_map=dtmAlloc("bgbdt_isomap_t", sizeof(BGBDT_IsoMap));
		isotest_map->tile=dtmAlloc("bgbdt_isomaptiles_t",
			iso_xs*iso_ys*sizeof(BGBDT_IsoTile *));
		isotest_map->xs=iso_xs;
		isotest_map->ys=iso_ys;
	}

	for(i=0; i<iso_ys; i++)
		for(j=0; j<iso_xs; j++)
	{
		k=isotile_blfclr_idx(iso_fbuf, iso_xs, iso_ys, j, i);
		if(!k)
			continue;

		tile=isotest_map->tile[i*iso_xs+j];
		if(!tile)
			tile=dtmAlloc("bgbdt_isotile_t", sizeof(BGBDT_IsoTile));
		isotest_map->tile[i*iso_xs+j]=tile;
		
		tile->tex_floor="textures/rrock14";
		tile->tex_floor_alt="textures/rrock14_a";
		tile->x=j;
		tile->y=i;

//		if(!(rand()&15))
		if(!(rand()&15) && ((i>48) || (j>48)))
		{
			tile->tex_floor="textures/rrock14_rspat";
			tile->tex_floor_alt="textures/rrock14_rspat_a";
		}
		
		if(k==16)tile->tex_floor="textures_dev/synrust0_hex0";
		if(k==32)tile->tex_floor="textures/lava4_1";
		if(k==48)tile->tex_floor="textures/redbrick0";
		if(k==1)
		{
			tile->tex_floor="textures/rrock14_rspat";
			tile->tex_floor_alt="textures/rrock14_rspat_a";
		}
		
//		if(!(rand()&63))
//			tile->tex_wall_xz="textures/rrock14";
//		if(!(rand()&63))
//			tile->tex_wall_yz="textures/rrock14";

		k4=isotile_blfclr_idx(iso_wbuf, iso_xs, iso_ys, j, i);
		k0=isotile_blfclr_idx(iso_wbuf, iso_xs, iso_ys, j-1, i  );
		k1=isotile_blfclr_idx(iso_wbuf, iso_xs, iso_ys, j+1, i  );
		k2=isotile_blfclr_idx(iso_wbuf, iso_xs, iso_ys, j  , i-1);
		k3=isotile_blfclr_idx(iso_wbuf, iso_xs, iso_ys, j  , i+1);
	
//		if((k4==1) || (k4==16))
//			continue;

		s0="textures/rrock14";
		s1="textures/rrock14_a";
//		if(!(rand()&15))
		if(!(rand()&15) && ((i>48) || (j>48)))
		{
			s0="textures/rrock14_rspat";
			s1="textures/rrock14_rspat_a";
		}
		if(k4==1)
		{
			s0="textures/rrock14_rspat";
			s1="textures/rrock14_rspat_a";
		}

		if(k4==16)
		{
			s0="textures_dev/gate0";
			s1=s0;
			tile->fl|=BGBDT_ISOTFL_NONSOLID;
		}

		if(k4)
		{
			if(k0 || k1)
			{
				if(k0)tile->fl|=BGBDT_ISOTFL_NX;
				if(k1)tile->fl|=BGBDT_ISOTFL_PX;
				tile->tex_wall_xz=s0;
				tile->tex_wall_xz_alt=s1;
			}

			if(k2 || k3)
			{
				if(k2)tile->fl|=BGBDT_ISOTFL_NY;
				if(k3)tile->fl|=BGBDT_ISOTFL_PY;
				tile->tex_wall_yz=s0;
				tile->tex_wall_yz_alt=s1;
			}
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
	isotest_ang[1]=-45;
	isotest_ang[2]=0;

	for(i=0; i<iso_ys; i++)
		for(j=0; j<iso_xs; j++)
	{
		k=isotile_blfclr_idx(iso_ebuf, iso_xs, iso_ys, j, i);
		if(!k)
			continue;

		k4=isotile_blfclr_idx(iso_wbuf, iso_xs, iso_ys, j, i);
		if(k4)
			continue;

		s0=NULL;

		if(k==16)
		{
			s0="npc_elf";
			if(!(rand()&15) && ((i>64) || (j>64)))
			{
				s0="npc_santa";
			}
		
//			IsoTile_SpawnEntityBasic("npc_elf",
//				vec2(j*64+32, i*64+32));
		}

		if(k==1)
		{
			s0="misc_chest";
//			IsoTile_SpawnEntityBasic("misc_chest",
//				vec2(j*64+32, i*64+32));
		}

		if(!s0)continue;

		IsoTile_SpawnEntityBasic(s0,
			vec2(j*64+32, i*64+32));
	}

	isotest_player=IsoTile_SpawnEntityBasic("player", vec2(128,128));
	isotest_worldtime=0;
	
	for(i=0; i<8*16; i++)
		isotest_invslot[i]=0;
	isotest_isdead=0;

	main_spawnworld();

	IsoTest_SetBgm("sound/pi0_amb0");
//	isotest_snd_bgmid=BGBDT_Sound_PlaySound("sound/pi0_amb0",
//		64, BGBDT_SNDATT_NONE, BGBDT_SNDFL_LOOP);
}

void isotest_menopt_newgame(FRGL_MenuInfo *menu, FRGL_MenuItem *mitm)
{
	isotest_newgame_init();
}

int main_startup(int argc, char *argv[])
{
	BGBDT_IsoTile *tile;
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

//	if(PDGLUI_HasFocus())
//		return(0);
//	UI_Camera_Update();

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
		}

		while(*kcur)
		{
			switch(*kcur)
			{
			case K_DEL:
			case K_ESC:
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

	V3F_COPY(isotest_org, pt);

	isotest_iswalk=0;
	if(FRGL_KeyDown(K_LEFTARROW))
	{
//		isotest_px-=100*frgl_state->dt_f;
		V3F_ADDSCALE(isotest_org,
			isotest_rot+0, -100*frgl_state->dt_f,
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
			isotest_rot+0, 100*frgl_state->dt_f,
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
			isotest_rot+3, 100*frgl_state->dt_f,
			isotest_org);
		V3F_ADDSCALE(isotest_org,
			isotest_rot+6, 100*frgl_state->dt_f,
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
			isotest_rot+3, -100*frgl_state->dt_f,
			isotest_org);
		V3F_ADDSCALE(isotest_org,
			isotest_rot+6, -100*frgl_state->dt_f,
			isotest_org);
		isotest_org[2]=f;

		if(!FRGL_KeyDown(K_LEFTARROW) &&
			!FRGL_KeyDown(K_RIGHTARROW))
				isotest_pldir=3;
		isotest_iswalk=1;
	}

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

	if(!isotest_player_noclip)
	{
		if(IsoTile_CheckCollideMove(isotest_map, isotest_player,
			vec2(isotest_org[0], isotest_org[1]),
			IsoTest_EntGetRadius(isotest_player)))
		{
			if(!IsoTile_CheckCollideMove(isotest_map, isotest_player,
				vec2(pt[0], isotest_org[1]),
				IsoTest_EntGetRadius(isotest_player)))
			{
				isotest_org[0]=pt[0];
			}else if(!IsoTile_CheckCollideMove(
				isotest_map, isotest_player,
				vec2(isotest_org[0], pt[1]),
				IsoTest_EntGetRadius(isotest_player)))
			{
				isotest_org[1]=pt[1];
			}else
			{
				V3F_COPY(pt, isotest_org);
				isotest_iswalk=0;
			}
		}
	}

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

	V3F_ADDSCALE(isotest_org,
		isotest_rot+3, -64, io);
	V3F_ADDSCALE(io,
		isotest_rot+6, -64, io);
	io[2]=128;

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

	tex=Tex_LoadFile("images/openhole0.png", NULL, NULL);

	FRGL_Setup2D();

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
		(int)(isotest_org[0]/64), (int)(isotest_org[1]/64),
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
