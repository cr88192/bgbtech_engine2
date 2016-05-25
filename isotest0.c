#include <bteifgl.h>
#include <bteifgl_main.h>

// BTEWS_WireGrid *btews_grid=NULL;
// BTEWS_Component *btews_component=NULL;

BSVM2_CodeImage *isotest_img;
BSVM2_ImageGlobal *isotest_vi_update;

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

dtVal isotest_player;
dtVal isotest_diagbox;
int isotest_diag_pchars;
double isotest_diag_ptime;

MAIN_EXPORT void IsoTest_SetDialog(dtVal dbox)
{
	isotest_diagbox=dbox;
	isotest_diag_pchars=0;
	isotest_diag_ptime=frgl_state->time_f;
}

vec2 IsoTest_EntGetOrigin(dtVal obj)
{
	void *p;
	p=dtcVaGetPtr(obj, isotest_eb2d_fi_origin);
	return(*(vec2 *)p);
}

void IsoTest_EntSetOrigin(dtVal obj, vec2 org)
{
	void *p;
	p=dtcVaGetPtr(obj, isotest_eb2d_fi_origin);
	(*(vec2 *)p)=org;
}

float IsoTest_EntGetAngle(dtVal obj)
{
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
	GfxDrv_SetDefaults("BTDT IsoTest", 1024, 768);
	FRGL_CvarSetDefault("hide_os_cursor", "1");

	Con_ExecCmdBuf("config.cfg");

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

dtVal IsoTile_SpawnEntityBasic(char *cname, vec2 org)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;
	dtcClass clz;
	dtVal ent;
	dtVal obj, sf;
	void *sfp;
	int n;

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
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	
	return(obj);
}

int IsoTile_CallEntityTouch(dtVal self, dtVal other)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;

	dtVal sf;
	void *sfp;

	if(!isotest_eb2d_fi_touch)
	{
		isotest_eb2d_fi_touch=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "touch");
	}

	sf=dtcVaGetA(self, isotest_eb2d_fi_touch);
	sfp=dtvUnwrapPtr(sf);
	if(!sfp)
		{ return(-1); }

	args=targs;
	args[0].a=other;

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupMethodCallVM(vctx, sfp, self, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	return(0);
}

int IsoTile_CallEntityUse(dtVal self, dtVal other)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;

	dtVal sf;
	void *sfp;

	if(!isotest_eb2d_fi_use)
	{
		isotest_eb2d_fi_use=BGBDTC_LookupClassSlotName(
			isotest_eb2d_cls, "use");
	}

	sf=dtcVaGetA(self, isotest_eb2d_fi_use);
	sfp=dtvUnwrapPtr(sf);
	if(!sfp)
		{ return(-1); }

	args=targs;
	args[0].a=other;

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupMethodCallVM(vctx, sfp, self, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	return(0);
}

int IsoTile_CallDiagImpulse(dtVal self, int imp)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;

	dtVal sf;
	void *sfp;

	if(!isotest_npcdiag_fi_impulse)
	{
		isotest_npcdiag_fi_impulse=BGBDTC_LookupClassSlotName(
			isotest_npcdiag_cls, "impulse");
	}

	sf=dtcVaGetA(self, isotest_npcdiag_fi_impulse);
	sfp=dtvUnwrapPtr(sf);
	if(!sfp)
		{ return(-1); }

	args=targs;
	args[0].i=imp;

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupMethodCallVM(vctx, sfp, self, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	return(0);
}

int IsoTile_SetGlobalA(char *cname, dtVal v)
{
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		isotest_img, cname);
	BSVM2_Interp_SetGlobalA(vi, v);
}

int IsoTile_SetGlobalI(char *cname, int v)
{
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		isotest_img, cname);
	BSVM2_Interp_SetGlobalI(vi, v);
}

int IsoTile_DrawTile(BGBDT_IsoMap *map,
	BGBDT_IsoTile *tile)
{
	float x0, y0, x1, y1;
	float x2, y2, x3, y3;
	float z0, z1;
	int tex;

	x0=tile->x*64; x1=x0+64; x2=x0+32;
	y0=tile->y*64; y1=y0+64; y2=y0+32;
	z0=0; z1=64;

	tex=Tex_LoadFile(tile->tex_floor, NULL, NULL);

	frglEnableTexture2D();
	frglBindTexture2D(tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex3f(x0, y0, z0);
	frglTexCoord2f(1, 0);
	frglVertex3f(x1, y0, z0);
	frglTexCoord2f(1, 1);
	frglVertex3f(x1, y1, z0);
	frglTexCoord2f(0, 1);
	frglVertex3f(x0, y1, z0);
	frglEnd();
	
	if(tile->tex_wall_xz)
	{
		tex=Tex_LoadFile(tile->tex_wall_xz, NULL, NULL);

		frglEnableTexture2D();
		frglBindTexture2D(tex);
		frglBegin(GL_QUADS);
		frglTexCoord2f(0, 0);
		frglVertex3f(x0, y2, z0);
		frglTexCoord2f(1, 0);
		frglVertex3f(x1, y2, z0);
		frglTexCoord2f(1, 1);
		frglVertex3f(x1, y2, z1);
		frglTexCoord2f(0, 1);
		frglVertex3f(x0, y2, z1);
		frglEnd();
	}

	if(tile->tex_wall_yz)
	{
		tex=Tex_LoadFile(tile->tex_wall_yz, NULL, NULL);

		frglEnableTexture2D();
		frglBindTexture2D(tex);
		frglBegin(GL_QUADS);
		frglTexCoord2f(0, 0);
		frglVertex3f(x2, y0, z0);
		frglTexCoord2f(1, 0);
		frglVertex3f(x2, y1, z0);
		frglTexCoord2f(1, 1);
		frglVertex3f(x2, y1, z1);
		frglTexCoord2f(0, 1);
		frglVertex3f(x2, y0, z1);
		frglEnd();
	}
}

int IsoTest_DrawSprite(char *spr, vec2 vorg, float xs, float ys)
{
	float org[4], v0[4], v1[4], v2[4], v3[4];
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
	frglEnd();
}

int IsoTile_DrawEntity(BGBDT_IsoMap *map,
	dtVal ent)
{
	vec2 org;
	char *spr;
	float xs, ys;
	
	org=IsoTest_EntGetOrigin(ent);
	spr=IsoTest_EntGetSprite(ent);
	IsoTest_EntGetSpriteSize(ent, &xs, &ys);
	
	if(!spr)
		return(0);
	
	if(xs<=0)xs=32;
	if(ys<=0)ys=32;
	
	IsoTest_DrawSprite(spr, org, xs, ys);
	return(1);
}

int isotile_drawmap(BGBDT_IsoMap *map)
{
	BGBDT_IsoTile *tile;
	dtVal ent;
	int x, y, mx, my, nx, ny;
	int i, j, k, l;

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
	float x0, y0, x1, y1, x2, y2;
	
	if(!tile)
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
			return(1);
	}

	if(tile->tex_wall_yz)
	{
		if(fabs(x-x2)<r)
			return(1);
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

	sf0=face1;
	if(text)
	{
		l=strlen(text);
		if(isotest_diag_pchars<l)
		{
			i=((int)(frgl_state->time_f*8))&1;
			if(i)sf0=face2;
		}
	}
	
	isotest_diag_pchars=(frgl_state->time_f-isotest_diag_ptime)*24;

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

int main_startup(int argc, char *argv[])
{
	BGBDT_IsoTile *tile;
	byte *iso_fbuf;
	byte *iso_wbuf;
	byte *iso_ebuf;
	dtcClass clz;
	float f, g;
	int xs, ys;
	int k0, k1, k2, k3, k4;
	int i, j, k;

	Tex_Init2();
	GfxFont_Init();
	Con_Init();
//	ConCmds_Init();

//	NET_InitLow();
//	BTEIFGL_NET_Init();

	main_loadscript("isotest.cfg");

	isotest_eb2d_cls=BGBDTC_LookupClassQName("EntityBase2D");
	isotest_eb2d_fi_origin=BGBDTC_LookupClassSlotName(
		isotest_eb2d_cls, "origin");
	isotest_eb2d_fi_angle=BGBDTC_LookupClassSlotName(
		isotest_eb2d_cls, "angle");

	isotest_npcdiag_cls=BGBDTC_LookupClassQName("NpcDialogBox");

	iso_fbuf=Tex_LoadFileRaw("isomap/map_floor.png", &xs, &ys);
	iso_wbuf=Tex_LoadFileRaw("isomap/map_wall.png", NULL, NULL);
	iso_ebuf=Tex_LoadFileRaw("isomap/map_spawn.png", NULL, NULL);

	isotest_map=dtmAlloc("bgbdt_isomap_t", sizeof(BGBDT_IsoMap));
	isotest_map->tile=dtmAlloc("bgbdt_isomaptiles_t",
		xs*ys*sizeof(BGBDT_IsoTile *));
	isotest_map->xs=xs;
	isotest_map->ys=ys;

	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
		k=isotile_blfclr_idx(iso_fbuf, xs, ys, j, i);
		if(!k)
			continue;

		tile=dtmAlloc("bgbdt_isotile_t", sizeof(BGBDT_IsoTile));
		isotest_map->tile[i*xs+j]=tile;
		
		tile->tex_floor="textures/rrock14";
		tile->x=j;
		tile->y=i;
				
//		if(!(rand()&63))
//			tile->tex_wall_xz="textures/rrock14";
//		if(!(rand()&63))
//			tile->tex_wall_yz="textures/rrock14";

		k4=isotile_blfclr_idx(iso_wbuf, xs, ys, j, i);
		k0=isotile_blfclr_idx(iso_wbuf, xs, ys, j-1, i  );
		k1=isotile_blfclr_idx(iso_wbuf, xs, ys, j+1, i  );
		k2=isotile_blfclr_idx(iso_wbuf, xs, ys, j  , i-1);
		k3=isotile_blfclr_idx(iso_wbuf, xs, ys, j  , i+1);
	
		if(k4)
		{
			if(k0 || k1)
			{
				tile->tex_wall_xz="textures/rrock14";
			}

			if(k2 || k3)
			{
				tile->tex_wall_yz="textures/rrock14";
			}
		}
	
	}

	isotest_map->entarr=dtvNewArray(4096, BGBDT_BASETY_ADDRESS);
	isotest_map->nents=0;

	IsoTile_SetGlobalA("world_entity", isotest_map->entarr);

	isotest_org[0]=0;
	isotest_org[1]=0;
	isotest_org[2]=128;
	
//	isotest_ang[0]=-45;
	isotest_ang[0]=0;
	isotest_ang[1]=-30;
	isotest_ang[2]=0;
	
	for(i=0; i<16; i++)
	{
		f=((u16)rand())/65536.0;
		g=((u16)rand())/65536.0;
		IsoTile_SpawnEntityBasic("misc_chest", vec2(f*32*64, g*32*64));
	}

	for(i=0; i<8; i++)
	{
		f=((u16)rand())/65536.0;
		g=((u16)rand())/65536.0;
		IsoTile_SpawnEntityBasic("npc_elf", vec2(f*48*64, g*48*64));
	}
	
//	clz=BGBDTC_LookupClassQName("player");
//	isotest_player=dtvWrapPtr(BGBDTC_AllocClassInstance(clz));
//	i=isotest_map->nents++;
//	dtvArraySetIndexDtVal(isotest_map->entarr, i, isotest_player);
	
	isotest_player=IsoTile_SpawnEntityBasic("player", vec2(0,0));
	
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


	hituse=0;
	kcur=frgl_state->keys;
	while(*kcur)
	{
		switch(*kcur)
		{
		case K_DEL:
//			BTEWS_DeleteNode(btews_grid,
//				btews_grid->mgx, btews_grid->mgy);
			break;		
		case K_ENTER:
			hituse=1;
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

	if(IsoTile_CheckCollideMove(isotest_map, isotest_player,
		vec2(isotest_org[0], isotest_org[1]),
		IsoTest_EntGetRadius(isotest_player)))
	{
		V3F_COPY(pt, isotest_org);
		isotest_iswalk=0;
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

	IsoTile_UpdateMap(isotest_map);

//	BTEWS_UpdateGrid(btews_grid, frgl_state->dt_f);

//	io[0]=isotest_org[0];
//	io[1]=isotest_org[1];
//	io[2]=isotest_org[2];

	V3F_ADDSCALE(isotest_org,
		isotest_rot+3, -64, io);
	V3F_ADDSCALE(io,
		isotest_rot+6, -64, io);
	io[2]=64;

	Mat3F_Identity(isotest_rot);
	
	v0[0]=1; v0[1]=0; v0[2]=0;
	Mat3F_Rotate3Matrix(isotest_rot,
		v0, isotest_ang[1]*(M_PI/180), isotest_rot);

	v0[0]=0; v0[1]=0; v0[2]=1;
	Mat3F_Rotate3Matrix(isotest_rot,
		v0, isotest_ang[0]*(M_PI/180), isotest_rot);

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
	}

//	isotile_drawmap(isotest_map);

//	f=-(btews_grid->xs*0.5)*32;
//	g=-(btews_grid->ys*0.5)*32;
//	BTEWS_DrawGrid(btews_grid, f, g, 32, 32);

#if 0
//	ncur=BTEWS_GetAllocNode(btews_grid, mgx, mgy);
	ncur=BTEWS_GetNode(btews_grid, mgx, mgy);
	if(ncur)
	{
		if(ncur->Describe)
		{
			s=ncur->Describe(ncur);
			if(!s)s="<N/A>";
			GfxFont_DrawString(s, -320, 384-16, 16, 16, 0, 255, 0, 255);
		}else
		{
			f=(ncur->vnx+ncur->vpx+ncur->vny+ncur->vpy)*0.25;
			sprintf(tb, "%d,%d: V=%2.2f A=%2.2f W=%2.2f ~%2.2fC Pw=%5.2f",
				ncur->widx, ncur->widy,
				f, ncur->icc, ncur->watt, ncur->temp, ncur->pwatt);
			GfxFont_DrawString(tb, -320, 384-16, 16, 16, 0, 255, 0, 255);
		}
	}
#endif

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
