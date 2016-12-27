BGBDT_VoxWorld *bt2ent_voxworld;

BSVM2_CodeImage *bt2ent_img;
BSVM2_ImageGlobal *bt2ent_vi_update;
BSVM2_ImageGlobal *bt2ent_vi_spawnworld;

BSVM2_ImageGlobal *bt2ent_vi_toolset;
BSVM2_ImageGlobal *bt2ent_vi_tooluse;

dtcClass bt2ent_eb3d_cls;
dtcField bt2ent_eb3d_fi_origin;
dtcField bt2ent_eb3d_fi_velocity;
dtcField bt2ent_eb3d_fi_angle;
dtcField bt2ent_eb3d_fi_radius;
dtcField bt2ent_eb3d_fi_height;
dtcField bt2ent_eb3d_fi_sprite;
dtcField bt2ent_eb3d_fi_sprite_lf;
dtcField bt2ent_eb3d_fi_sprite_bk;
dtcField bt2ent_eb3d_fi_sprite_sz;
dtcField bt2ent_eb3d_fi_flags;
dtcField bt2ent_eb3d_fi_owner;

dtcField bt2ent_eb3d_fi_spawn;
dtcField bt2ent_eb3d_fi_touch;
dtcField bt2ent_eb3d_fi_use;

dtcClass bt2ent_npcdiag_cls;

// dtVal bt2ent_map_entarr;
// int bt2ent_map_nents;

BTEIFGL_API vec3d Bt2Ent_EntGetOrigin(dtVal obj)
{
	double dv[3];
	void *p;
	if(!bt2ent_eb3d_fi_origin)
	{
		bt2ent_eb3d_fi_origin=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "origin");
		if(!bt2ent_eb3d_fi_origin)
			return(vec3d(0,0,0));
	}
	p=dtcVaGetPtr(obj, bt2ent_eb3d_fi_origin);
	BSVM2_Unpack3XfTo3Dv(p, dv);
	return(vec3dvd(dv));
//	return(*(vec2 *)p);
}

BTEIFGL_API void Bt2Ent_EntSetOrigin(dtVal obj, vec3d org)
{
	double dv[3];
	void *p;
	if(!bt2ent_eb3d_fi_origin)
	{
		bt2ent_eb3d_fi_origin=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "origin");
		if(!bt2ent_eb3d_fi_origin)
			return;
	}
	p=dtcVaGetPtr(obj, bt2ent_eb3d_fi_origin);
	vdvec3d(dv, org);
	BSVM2_Pack3DvTo3Xf(p, dv);
//	(*(vec2 *)p)=org;
}

BTEIFGL_API vec3d Bt2Ent_EntGetVelocity(dtVal obj)
{
	double dv[3];
	void *p;
	if(!bt2ent_eb3d_fi_velocity)
	{
		bt2ent_eb3d_fi_velocity=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "velocity");
		if(!bt2ent_eb3d_fi_velocity)
			return(vec3d(0,0,0));
	}
	p=dtcVaGetPtr(obj, bt2ent_eb3d_fi_velocity);
	BSVM2_Unpack3XfTo3Dv(p, dv);
	return(vec3dvd(dv));
//	return(*(vec2 *)p);
}

BTEIFGL_API void Bt2Ent_EntSetVelocity(dtVal obj, vec3d org)
{
	double dv[3];
	void *p;
	if(!bt2ent_eb3d_fi_velocity)
	{
		bt2ent_eb3d_fi_velocity=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "velocity");
		if(!bt2ent_eb3d_fi_velocity)
			return;
	}
	p=dtcVaGetPtr(obj, bt2ent_eb3d_fi_velocity);
//	(*(vec2 *)p)=org;
	vdvec3d(dv, org);
	BSVM2_Pack3DvTo3Xf(p, dv);
}

BTEIFGL_API float Bt2Ent_EntGetAngle(dtVal obj)
{
	if(!bt2ent_eb3d_fi_angle)
	{
		bt2ent_eb3d_fi_angle=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "angle");
		if(!bt2ent_eb3d_fi_angle)
			return(0);
	}
	return(dtcVaGetF(obj, bt2ent_eb3d_fi_angle));
}

BTEIFGL_API void Bt2Ent_EntSetAngle(dtVal obj, float val)
{
	if(!bt2ent_eb3d_fi_angle)
	{
		bt2ent_eb3d_fi_angle=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "angle");
		if(!bt2ent_eb3d_fi_angle)
			return;
	}

	dtcVaSetF(obj, bt2ent_eb3d_fi_angle, val);
}


BTEIFGL_API float Bt2Ent_EntGetRadius(dtVal obj)
{
	if(!bt2ent_eb3d_fi_radius)
	{
		bt2ent_eb3d_fi_radius=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "radius");
		if(!bt2ent_eb3d_fi_radius)
			return(0);
	}

	return(dtcVaGetF(obj, bt2ent_eb3d_fi_radius));
}

BTEIFGL_API float Bt2Ent_EntGetHeight(dtVal obj)
{
	if(!bt2ent_eb3d_fi_height)
	{
		bt2ent_eb3d_fi_height=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "height");
		if(!bt2ent_eb3d_fi_height)
			return(0);
	}

	return(dtcVaGetF(obj, bt2ent_eb3d_fi_height));
}

BTEIFGL_API void Bt2Ent_EntSetRadius(dtVal obj, float val)
{
	if(!bt2ent_eb3d_fi_radius)
	{
		bt2ent_eb3d_fi_radius=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "radius");
		if(!bt2ent_eb3d_fi_radius)
			return;
	}

	dtcVaSetF(obj, bt2ent_eb3d_fi_radius, val);
}

BTEIFGL_API void Bt2Ent_EntSetHeight(dtVal obj, float val)
{
	if(!bt2ent_eb3d_fi_height)
	{
		bt2ent_eb3d_fi_height=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "height");
		if(!bt2ent_eb3d_fi_height)
			return;
	}

	dtcVaSetF(obj, bt2ent_eb3d_fi_height, val);
}

BTEIFGL_API char *Bt2Ent_EntGetSprite(dtVal obj)
{
	dtVal v;

	if(!bt2ent_eb3d_fi_sprite)
	{
		bt2ent_eb3d_fi_sprite=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "sprite");
		if(!bt2ent_eb3d_fi_sprite)
			return(NULL);
	}

	v=dtcVaGetA(obj, bt2ent_eb3d_fi_sprite);
	return(BGBDT_TagStr_GetUtf8(v));
}

BTEIFGL_API char *Bt2Ent_EntGetSpriteLf(dtVal obj)
{
	dtVal v;

	if(!bt2ent_eb3d_fi_sprite_lf)
	{
		bt2ent_eb3d_fi_sprite_lf=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "sprite_lf");
		if(!bt2ent_eb3d_fi_sprite_lf)
			return(NULL);
	}

	v=dtcVaGetA(obj, bt2ent_eb3d_fi_sprite_lf);
	return(BGBDT_TagStr_GetUtf8(v));
}

BTEIFGL_API char *Bt2Ent_EntGetSpriteBk(dtVal obj)
{
	dtVal v;

	if(!bt2ent_eb3d_fi_sprite_bk)
	{
		bt2ent_eb3d_fi_sprite_bk=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "sprite_bk");
		if(!bt2ent_eb3d_fi_sprite_bk)
			return(NULL);
	}

	v=dtcVaGetA(obj, bt2ent_eb3d_fi_sprite_bk);
	return(BGBDT_TagStr_GetUtf8(v));
}

BTEIFGL_API void Bt2Ent_EntGetSpriteSize(dtVal obj, float *xs, float *ys)
{
	float *fv;
	if(!bt2ent_eb3d_fi_sprite_sz)
	{
		bt2ent_eb3d_fi_sprite_sz=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "sprite_sz");
	}

	fv=dtcVaGetPtr(obj, bt2ent_eb3d_fi_sprite_sz);
	*xs=fv[0];
	*ys=fv[1];
}

BTEIFGL_API s64 Bt2Ent_EntGetFlags(dtVal obj)
{
	if(!bt2ent_eb3d_fi_flags)
	{
		bt2ent_eb3d_fi_flags=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "flags");
		if(!bt2ent_eb3d_fi_flags)
			return(0);
	}

	return(dtcVaGetL(obj, bt2ent_eb3d_fi_flags));
}

BTEIFGL_API void Bt2Ent_EntSetFlags(dtVal obj, s64 val)
{
	if(!bt2ent_eb3d_fi_flags)
	{
		bt2ent_eb3d_fi_flags=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "flags");
		if(!bt2ent_eb3d_fi_flags)
			return;
	}

	dtcVaSetL(obj, bt2ent_eb3d_fi_flags, val);
}

BTEIFGL_API dtVal Bt2Ent_EntGetOwner(dtVal obj)
{
	if(!bt2ent_eb3d_fi_owner)
	{
		bt2ent_eb3d_fi_owner=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "owner");
		if(!bt2ent_eb3d_fi_owner)
			return(DTV_NULL);
	}

	return(dtcVaGetA(obj, bt2ent_eb3d_fi_owner));
}

BTEIFGL_API void Bt2Ent_EntSetOwner(dtVal obj, dtVal val)
{
	if(!bt2ent_eb3d_fi_owner)
	{
		bt2ent_eb3d_fi_owner=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "owner");
		if(!bt2ent_eb3d_fi_owner)
			return;
	}

	dtcVaSetA(obj, bt2ent_eb3d_fi_owner, val);
}


BTEIFGL_API int Bt2Ent_LoadScript(char *def, char *imgname)
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
	vf_storefile(imgname, tbuf, i);

	bt2ent_img=BS2I_DecodeImageBuffer(tbuf, i);
	vi=BS2I_ImageGetMain(bt2ent_img, NULL);

	frgl_free(tbuf);
	BS2P_FreeCompileContext(ctx);

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, vi, NULL);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);

	bt2ent_vi_update=BS2I_ImageLookupFunc(
		bt2ent_img, "update");
	bt2ent_vi_spawnworld=BS2I_ImageLookupFunc(
		bt2ent_img, "spawn_world");

	bt2ent_vi_toolset=BS2I_ImageLookupFunc(
		bt2ent_img, "tool_set");
	bt2ent_vi_tooluse=BS2I_ImageLookupFunc(
		bt2ent_img, "tool_use");


	bt2ent_eb3d_cls=BGBDTC_LookupClassQName("EntityBase");
	bt2ent_eb3d_fi_origin=BGBDTC_LookupClassSlotName(
		bt2ent_eb3d_cls, "origin");
	bt2ent_eb3d_fi_angle=BGBDTC_LookupClassSlotName(
		bt2ent_eb3d_cls, "angle");

	bt2ent_npcdiag_cls=BGBDTC_LookupClassQName("NpcDialogBox");

	return(0);
}

BTEIFGL_API int Bt2Ent_ScriptUpdate(double dt)
{
	BSVM2_Value targs[4];
	double org[3];
	BSVM2_Context *vctx;
	BSVM2_Value *args;
	int i;

	if(!dtvNullP(bt2ent_voxworld->ent_player))
	{
		org[0]=bt2ent_voxworld->reforg[0]+bt2ent_voxworld->camorg[0];
		org[1]=bt2ent_voxworld->reforg[1]+bt2ent_voxworld->camorg[1];
		org[2]=bt2ent_voxworld->reforg[2]+bt2ent_voxworld->camorg[2];
		Bt2Ent_EntSetOrigin(bt2ent_voxworld->ent_player,
			vec3d(org[0], org[1], org[2]));
	}

	Bt2Ent_SetGlobalVec3("player_reforg", vec3d(
			bt2ent_voxworld->reforg[0],
			bt2ent_voxworld->reforg[1],
			bt2ent_voxworld->reforg[2]));

	Bt2Ent_SetGlobalVec3("player_view_fw", vec3d(
			bt2ent_voxworld->camrot[1*3+0],
			bt2ent_voxworld->camrot[1*3+1],
			bt2ent_voxworld->camrot[1*3+2]));
	Bt2Ent_SetGlobalVec3("player_view_rt", vec3d(
			bt2ent_voxworld->camrot[0*3+0],
			bt2ent_voxworld->camrot[0*3+1],
			bt2ent_voxworld->camrot[0*3+2]));
	Bt2Ent_SetGlobalVec3("player_view_up", vec3d(
			bt2ent_voxworld->camrot[2*3+0],
			bt2ent_voxworld->camrot[2*3+1],
			bt2ent_voxworld->camrot[2*3+2]));

	if(!bt2ent_vi_update)
		return(-1);

	args=targs;
	args[0].d=dt;

//	Bt2Ent_SetGlobalA("world_entity", bt2ent_map_entarr);
//	Bt2Ent_SetGlobalI("world_max_entity", bt2ent_map_nents);

	i=Bt2Ent_GetGlobalI("world_max_entity");
	if(i>bt2ent_voxworld->nents)
		bt2ent_voxworld->nents=i;

	Bt2Ent_SetGlobalA("world_entity", bt2ent_voxworld->entarr);
	Bt2Ent_SetGlobalI("world_max_entity", bt2ent_voxworld->nents);

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, bt2ent_vi_update, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);

	bt2ent_voxworld->entarr=Bt2Ent_GetGlobalA("world_entity");
	i=Bt2Ent_GetGlobalI("world_max_entity");
	if(i!=bt2ent_voxworld->nents)
		bt2ent_voxworld->nents=i;

	return(0);
}

BTEIFGL_API int Bt2Ent_BindVoxWorld(BGBDT_VoxWorld *world)
{
	bt2ent_voxworld=world;
	return(0);
}

BTEIFGL_API int Bt2Ent_SpawnWorld(char *func)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;
	BSVM2_ImageGlobal *vi;

	if(!bt2ent_vi_spawnworld)
		return(-1);

	args=targs;

//	if(!dtvTrueP(bt2ent_map_entarr))
//	{
//		bt2ent_map_entarr=dtvNewArray(4096, BGBDT_BASETY_ADDRESS);
//		bt2ent_map_nents=0;
//		bt2ent_voxworld->entarr=bt2ent_map_entarr;
//		bt2ent_voxworld->nents=bt2ent_map_nents;
//	}

	if(!dtvTrueP(bt2ent_voxworld->entarr))
	{
		bt2ent_voxworld->entarr=dtvNewArray(4096, BGBDT_BASETY_ADDRESS);
		bt2ent_voxworld->nents=0;
	}

//	Bt2Ent_SetGlobalA("world_entity", bt2ent_map_entarr);
//	Bt2Ent_SetGlobalI("world_max_entity", bt2ent_map_nents);

	Bt2Ent_SetGlobalA("world_entity", bt2ent_voxworld->entarr);
	Bt2Ent_SetGlobalI("world_max_entity", bt2ent_voxworld->nents);

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, bt2ent_vi_spawnworld, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	
	if(func)
	{
		vi=BS2I_ImageLookupFunc(bt2ent_img, func);

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

	Bt2Ent_SpawnPlayer();

	bt2ent_voxworld->entarr=Bt2Ent_GetGlobalA("world_entity");
	bt2ent_voxworld->nents=Bt2Ent_GetGlobalI("world_max_entity");

	return(0);
}


BTEIFGL_API int Bt2Ent_ToolSet(int id)
{
	BSVM2_Value targs[4];
	double org[3];
	BSVM2_Context *vctx;
	BSVM2_Value *args;

	if(!bt2ent_vi_toolset)
		return(-1);

	args=targs;
	args[0].i=id;

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, bt2ent_vi_toolset, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	return(0);
}

BTEIFGL_API int Bt2Ent_ToolUse(int flag)
{
	BSVM2_Value targs[4];
	double org[3];
	BSVM2_Context *vctx;
	BSVM2_Value *args;

	if(!bt2ent_vi_tooluse)
		return(-1);

	args=targs;
	args[0].i=flag;

	vctx=BSVM2_Interp_AllocPoolContext();
	BSVM2_Interp_SetupCallVM(vctx, bt2ent_vi_tooluse, args);
	BSVM2_Interp_RunContext(vctx, 999999999);
	BSVM2_Interp_FreePoolContext(vctx);
	return(0);
}

BTEIFGL_API dtVal Bt2Ent_SpawnPlayer(void)
{
	dtVal ent;
	
	if(!dtvNullP(bt2ent_voxworld->ent_player))
		return(bt2ent_voxworld->ent_player);
	
	ent=Bt2Ent_SpawnEntityBasic("player", vec3d(0,0,0));
	Bt2Ent_SetGlobalA("world_player", ent);
	bt2ent_voxworld->ent_player=ent;
	return(ent);
}

BTEIFGL_API dtVal Bt2Ent_SpawnEntityBasicXyz(
	char *cname, double x, double y, double z)
{
	return(Bt2Ent_SpawnEntityBasic(cname, vec3d(x, y, z)));
}

BTEIFGL_API dtVal Bt2Ent_SpawnEntityBasic(char *cname, vec3d org)
{
	BSVM2_Value targs[4];
	BSVM2_Context *vctx;
	BSVM2_Value *args;
	dtcClass clz;
	dtVal ent;
	dtVal obj, sf;
	void *sfp;
	int i, n;

	if(!bt2ent_img)
	{
		return(DTV_NULL);
	}

	if(!bt2ent_eb3d_fi_spawn)
	{
		bt2ent_eb3d_fi_spawn=BGBDTC_LookupClassSlotName(
			bt2ent_eb3d_cls, "spawn");
		if(!bt2ent_eb3d_fi_spawn)
		{
			printf("Failed get class spawn method %s\n", cname);
			return(DTV_NULL);
		}
	}

	clz=BGBDTC_LookupClassQName(cname);
	if(!clz)
	{
		printf("Spawn Failed: No Class '%s'\n", cname);
		return(DTV_NULL);
	}
	
//	clz=BGBDTC_LookupClassQName(cname);
	obj=dtvWrapPtr(BGBDTC_AllocClassInstance(clz));

//	n=bt2ent_map_nents++;
//	dtvArraySetIndexDtVal(bt2ent_map_entarr, n, obj);

	n=bt2ent_voxworld->nents++;
	dtvArraySetIndexDtVal(bt2ent_voxworld->entarr, n, obj);

	Bt2Ent_EntSetOrigin(obj, org);

	sf=dtcVaGetA(obj, bt2ent_eb3d_fi_spawn);
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

	if(i)printf("Bt2Ent_SpawnEntityBasic: Classname=%s, Status=%d\n",
		cname, i);
	
	return(obj);
}

#if 1
BTEIFGL_API int Bt2Ent_CallEntityTouch(dtVal self, dtVal other)
{
	BSVM2_Value targs[4];
	int i;

	targs[0].a=other;
	i=BSVM2_Interp_CallCacheMethodVM(self,
		&bt2ent_eb3d_fi_touch, targs,
		"EntityBase", "touch", NULL, 999999999);
	if(i)printf("Bt2Ent_CallEntityTouch: Status=%d\n", i);
	return(0);
}

BTEIFGL_API int Bt2Ent_CallEntityUse(dtVal self, dtVal other)
{
	BSVM2_Value targs[4];
	int i;

	targs[0].a=other;
	i=BSVM2_Interp_CallCacheMethodVM(self,
		&bt2ent_eb3d_fi_use, targs,
		"EntityBase", "use", NULL, 999999999);
	if(i)printf("Bt2Ent_CallEntityUse: Status=%d\n", i);
	return(0);
}
#endif

BTEIFGL_API dtVal Bt2Ent_GetGlobalA(char *cname)
{
	BSVM2_ImageGlobal *vi;
	dtVal v;

	vi=BS2I_ImageLookupGlobalVar(
		bt2ent_img, cname);
	if(!vi)return(DTV_UNDEFINED);
	v=BSVM2_Interp_GetGlobalA(vi);
	return(v);
}

BTEIFGL_API int Bt2Ent_GetGlobalI(char *cname)
{
	BSVM2_ImageGlobal *vi;
	int i;

	vi=BS2I_ImageLookupGlobalVar(
		bt2ent_img, cname);
	if(!vi)return(0);
	i=BSVM2_Interp_GetGlobalI(vi);
	return(i);
}



BTEIFGL_API int Bt2Ent_SetGlobalA(char *cname, dtVal v)
{
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		bt2ent_img, cname);
	if(!vi)return(-1);
	BSVM2_Interp_SetGlobalA(vi, v);
	return(0);
}

BTEIFGL_API int Bt2Ent_SetGlobalI(char *cname, int v)
{
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		bt2ent_img, cname);
	if(!vi)return(-1);
	BSVM2_Interp_SetGlobalI(vi, v);
	return(0);
}

BTEIFGL_API void Bt2Ent_SetGlobalVec3(char *cname, vec3d org)
{
	double dv[3];
	void *p;
	BSVM2_ImageGlobal *vi;

	vi=BS2I_ImageLookupGlobalVar(
		bt2ent_img, cname);
	if(!vi)return;

//	p=dtcVaGetPtr(obj, bt2ent_eb3d_fi_origin);
//	p=BSVM2_Interp_GetGlobalP(vi);

	vdvec3d(dv, org);
	BSVM2_Interp_SetGlobal3Xfv(vi, dv);
//	BSVM2_Pack3DvTo3Xf(p, dv);
//	(*(vec2 *)p)=org;
}


#if 1
int Bt2Ent_DrawSprite(BGBDT_VoxWorld *world,
	char *spr, vec3d vorg, float xs, float ys, int rang)
{
	float org[4], v0[4], v1[4], v2[4], v3[4];
	float fw[4];
	int tex, flip, zflip;
	int i;

//	V3F_COPY(bt2ent_org, org);
	org[0]=v3dx(vorg)-(world->reforg[0]);
	org[1]=v3dy(vorg)-(world->reforg[1]);
	org[2]=v3dz(vorg)-(world->reforg[2]);
	flip=0; zflip=0;

	if(xs<0)
		{ flip=1; xs=-xs; }
	if(ys<0)
		{ zflip=1; ys=-ys; }
	
//	if((rang==1) || (rang==2) || (rang==3))
//		flip=0;
//	if((rang==5) || (rang==6) || (rang==7))
//		flip=0;

	V3F_ADDSCALE(org, world->camrot+0, -(xs/2), v0);
	V3F_ADDSCALE(org, world->camrot+0,  (xs/2), v1);
	
	if(zflip)
	{
		v0[2]-=ys;
		v1[2]-=ys;
	}
	
	V3F_COPY(v0, v2);
	V3F_COPY(v1, v3);
	v2[2]+=ys;
	v3[2]+=ys;

//	V3F_ADDSCALE(v0, world->camrot+6, ys, v2);
//	V3F_ADDSCALE(v1, world->camrot+6, ys, v3);

	V3F_COPY(world->camrot+3, fw);
	fw[2]=0;

//	tex=Tex_LoadFile(spr, NULL, NULL);
	tex=FRGL_TexMat_GetLoadSprite(spr);

//	if(bt2ent_map)
//		frglColor4fv(bt2ent_map->sprclr);

	frglEnableTexture2D();
//	frglBindTexture2D(tex);
	FRGL_TexMat_BindSprite(tex, rang);
	
	if(FRGL_TexMat_BoundSpriteIsSideP())
		flip=0;
	
	if(FRGL_TexMat_BoundSpriteFlipP())
		flip=!flip;

	frglBegin(GL_QUADS);

	frglTexCoord2f(flip?1:0, zflip?1:0);
	frglVertex3fv(v0);
	frglTexCoord2f(flip?0:1, zflip?1:0);
	frglVertex3fv(v1);
	frglTexCoord2f(flip?0:1, zflip?0:1);
	frglVertex3fv(v3);
	frglTexCoord2f(flip?1:0, zflip?0:1);
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

int Bt2Ent_DrawEntity(BGBDT_VoxWorld *world, dtVal ent)
{
	vec3d org, corg, rdir;
	char *spr, *sprft, *sprlf, *sprbk;
	s64 fl;
	float xs, ys, d, ang, cang, ang1;
//	int xflip=0;
	int rang;
	
	corg=vec3d(
		world->camorg[0]+(world->reforg[0]),
		world->camorg[1]+(world->reforg[1]),
		world->camorg[2]+(world->reforg[2]));
	
	org=Bt2Ent_EntGetOrigin(ent);
	fl=Bt2Ent_EntGetFlags(ent);
	ang=Bt2Ent_EntGetAngle(ent);

	d=v3ddist(org, corg);
//	if(d>1024)
	if(d>128)
		return(0);
	if(d<=0)
		return(0);

	sprft=Bt2Ent_EntGetSprite(ent);
//	sprlf=Bt2Ent_EntGetSpriteLf(ent);
//	sprbk=Bt2Ent_EntGetSpriteBk(ent);

	spr=sprft;

#if 0
	spr=sprft;
	if(sprlf && sprbk)
	{
		rdir=v3dsub(org, corg);
		cang=atan2(v3dy(rdir), v3dx(rdir))*(180/M_PI);
		ang1=(ang-cang)+90;
		if(ang1<0)
			ang1+=360;
		if(ang1>=360)
			ang1-=360;
		
		if((ang1>=315) || (ang1<=45))
			{ spr=sprbk; xflip=0; }
		if((ang1>=45) && ang1<=135)
			{ spr=sprlf; xflip=0; }
		if((ang1>=135) && ang1<=225)
			{ spr=sprft; xflip=0; }
		if((ang1>=225) && ang1<=315)
			{ spr=sprlf; xflip=1; }
	}
#endif

	rdir=v3dsub(org, corg);
	cang=atan2(v3dy(rdir), v3dx(rdir))*(180/M_PI);
	ang1=(ang-cang)+90;
	if(ang1<0)
		ang1+=360;
	if(ang1>=360)
		ang1-=360;
	rang=ang1*(8.0/360);
//	rang&=7;

	Bt2Ent_EntGetSpriteSize(ent, &xs, &ys);
	
	if(!spr)
		return(0);
	
//	if(xs<=0)xs=32;
//	if(ys<=0)ys=32;

	if(xs==0)xs=32;
	if(ys==0)ys=32;

	if(fl&BGBDT_ENTFL_ZFLIP)
		ys=-ys;
//	if(xflip)
//		xs=-xs;

	Bt2Ent_DrawSprite(world, spr, org, xs/32, ys/32, rang);
	return(1);
}

BTEIFGL_API int Bt2Ent_DrawWorldEnts(BGBDT_VoxWorld *world)
{
//	BGBDT_IsoTile *tile;
	dtVal ent;
	int x, y, mx, my, nx, ny;
	int i, j, k, l;

//	if(isotest_usebkg)
//		return;

//	frglEnable(GL_CULL_FACE);
	frglDisable(GL_CULL_FACE);
	frglDisable(GL_ALPHA_TEST);

//	frglEnable(GL_ALPHA_TEST);

//	isotile_drawsky(map);
//	isotile_drawmap2(map);

	frglDisable(GL_CULL_FACE);
	frglEnable(GL_ALPHA_TEST);
	frglColor4f(1,1,1,1);

	l=world->nents;
	for(i=0; i<l; i++)
	{
		ent=dtvArrayGetIndexDtVal(world->entarr, i);
		if(dtvNullP(ent))
			continue;
		Bt2Ent_DrawEntity(world, ent);
	}

	frglDisable(GL_ALPHA_TEST);

	frglColor4f(1,1,1,1);
	return(0);
}
#endif

BTEIFGL_API int Bt2Ent_CheckBoxEnt(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord min, BGBDT_VoxCoord max,
	dtVal ent)
{
	double emin[3], emax[3];
	BGBDT_VoxCoord min2, max2;
	vec3d eorg, corg;
	double erad, ehgt;
	s64 efl;

	eorg=Bt2Ent_EntGetOrigin(ent);
	erad=Bt2Ent_EntGetRadius(ent);
	ehgt=Bt2Ent_EntGetHeight(ent);
	efl=Bt2Ent_EntGetFlags(ent);
	
	erad=erad/32;
	ehgt=ehgt/32;

	emin[0]=v3dx(eorg)-erad;
	emin[1]=v3dy(eorg)-erad;
	emax[0]=v3dx(eorg)+erad;
	emax[1]=v3dy(eorg)+erad;

	if(efl&BGBDT_ENTFL_ZFLIP)
	{
		emin[2]=v3dz(eorg)-ehgt;
		emax[2]=v3dz(eorg);
	}else
	{
		emin[2]=v3dz(eorg);
		emax[2]=v3dz(eorg)+ehgt;
	}

	min2.x=emin[0]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	min2.y=emin[1]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	min2.z=emin[2]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	max2.x=emax[0]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	max2.y=emax[1]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	max2.z=emax[2]*BGBDT_XYZ_SCALE_FROMMETER+0.5;

	if(BGBDT_CheckBoxCollideP(min, max, min2, max2))
		return(1);
	return(0);
}

BTEIFGL_API int Bt2Ent_GetBoxEnt(
	BGBDT_VoxWorld *world, dtVal ent,
	BGBDT_VoxCoord *rmin, BGBDT_VoxCoord *rmax)
{
	double emin[3], emax[3];
	BGBDT_VoxCoord min2, max2;
	vec3d eorg, corg;
	double erad, ehgt;
	s64 efl;

	eorg=Bt2Ent_EntGetOrigin(ent);
	erad=Bt2Ent_EntGetRadius(ent);
	ehgt=Bt2Ent_EntGetHeight(ent);
	efl=Bt2Ent_EntGetFlags(ent);
	
	erad=erad/32;
	ehgt=ehgt/32;

	emin[0]=v3dx(eorg)-erad;
	emin[1]=v3dy(eorg)-erad;
	emax[0]=v3dx(eorg)+erad;
	emax[1]=v3dy(eorg)+erad;

	if(efl&BGBDT_ENTFL_ZFLIP)
	{
		emin[2]=v3dz(eorg)-ehgt;
		emax[2]=v3dz(eorg);
	}else
	{
		emin[2]=v3dz(eorg);
		emax[2]=v3dz(eorg)+ehgt;
	}

	min2.x=emin[0]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	min2.y=emin[1]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	min2.z=emin[2]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	max2.x=emax[0]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	max2.y=emax[1]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	max2.z=emax[2]*BGBDT_XYZ_SCALE_FROMMETER+0.5;

	*rmin=min2;
	*rmax=max2;
	return(0);
}

BTEIFGL_API int Bt2Ent_CheckRayEnt(
	BGBDT_VoxWorld *world, BGBDT_VoxCoord spos, BGBDT_VoxCoord epos,
	dtVal ent)
{
	double emin[3], emax[3], eo2[3];
	double rso[3], reo[3], rdir[3];
	double cpa[3], cps[3], cpe[3];
	BGBDT_VoxCoord min2, max2;
	vec3d eorg, corg;
	double ezm, ezn;
	double erad, ehgt, d, de, cpd, cpr;
	s64 efl;

	eorg=Bt2Ent_EntGetOrigin(ent);
	erad=Bt2Ent_EntGetRadius(ent);
	ehgt=Bt2Ent_EntGetHeight(ent);
	efl=Bt2Ent_EntGetFlags(ent);

	erad=erad/32;
	ehgt=ehgt/32;	

	rso[0]=spos.x*BGBDT_XYZ_SCALE_TOMETER;
	rso[1]=spos.y*BGBDT_XYZ_SCALE_TOMETER;
	rso[2]=spos.z*BGBDT_XYZ_SCALE_TOMETER;

	reo[0]=epos.x*BGBDT_XYZ_SCALE_TOMETER;
	reo[1]=epos.y*BGBDT_XYZ_SCALE_TOMETER;
	reo[2]=epos.z*BGBDT_XYZ_SCALE_TOMETER;

	V3F_SUB(reo, rso, rdir);
	V3D_NORMALIZE(rdir, rdir);
	
	eo2[0]=v3dx(eorg);
	eo2[1]=v3dy(eorg);
	eo2[2]=v3dz(eorg);

	d=V3F_DOT(eo2, rdir)-V3F_DOT(rso, rdir);
	de=V3F_DOT(reo, rdir)-V3F_DOT(rso, rdir);
	if(d<0)d=0;
	if(d>de)d=de;

	V3F_ADDSCALE(rso, rdir, d, cpa);
	cpd=V2F_DISTP(eo2, cpa);
	if(cpd>=erad)
		return(0);

	cpr=erad-cpd;
	V3F_ADDSCALE(cpa, rdir, -cpr, cps);
	V3F_ADDSCALE(cpa, rdir,  cpr, cpe);

	if(efl&BGBDT_ENTFL_ZFLIP)
		{ ezn=eo2[2]; ezm=ezn-ehgt; }
	else
		{ ezm=eo2[2]; ezn=ezm+ehgt; }

	if((cps[2]<=ezm) && (cpe[2]<=ezm))
		return(0);
	if((cps[2]>=ezn) && (cpe[2]>=ezn))
		return(0);

	return(1);

//	emin[0]=v3dx(eorg)-erad;
//	emin[1]=v3dy(eorg)-erad;
//	emax[0]=v3dx(eorg)+erad;
//	emax[1]=v3dy(eorg)+erad;

//	emin[2]=v3dz(eorg);
//	emax[2]=v3dz(eorg)+ehgt;

//	min2.x=emin[0]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
//	min2.y=emin[1]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
//	min2.z=emin[2]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
//	max2.x=emax[0]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
//	max2.y=emax[1]*BGBDT_XYZ_SCALE_FROMMETER+0.5;
//	max2.z=emax[2]*BGBDT_XYZ_SCALE_FROMMETER+0.5;

//	if(BGBDT_CheckBoxCollideP(min, max, min2, max2))
//		return(1);
//	return(0);
}

BTEIFGL_API int Bt2Ent_BoxQueryWorldEnts(
	BGBDT_VoxWorld *world, dtVal self,
	BGBDT_VoxCoord min, BGBDT_VoxCoord max,
	dtVal *robj, int *rnobj, int qflag)
{
	dtVal ent, eown;
	int x, y, mx, my, nx, ny;
	int i, j, k, l, n;

	n=0;
	l=world->nents;
	for(i=0; i<l; i++)
	{
		ent=dtvArrayGetIndexDtVal(world->entarr, i);
		if(dtvNullP(ent))
			continue;
		if(dtvEqqP(ent, self))
			continue;

		eown=Bt2Ent_EntGetOwner(ent);
		if(dtvEqqP(eown, self))
			continue;

//		if(dtvEqqP(ent, world->ent_player))
//			continue;
//		Bt2Ent_DrawEntity(world, ent);
		if(Bt2Ent_CheckBoxEnt(world, min, max, ent))
			{ robj[n++]=ent; }
	}
	
	*rnobj=n;
	return(n>0);
}

BTEIFGL_API int Bt2Ent_RayQueryEntity(
	BGBDT_VoxWorld *world, dtVal self,
	BGBDT_VoxCoord spos, BGBDT_VoxCoord epos,
	dtVal *robj, int *rnobj, int tracefl)
{
	dtVal ent;
	int x, y, mx, my, nx, ny;
	int i, j, k, l, n;

	n=0;
	l=world->nents;
	for(i=0; i<l; i++)
	{
		ent=dtvArrayGetIndexDtVal(world->entarr, i);
		if(dtvNullP(ent))
			continue;
		if(dtvEqqP(ent, self))
			continue;
//		Bt2Ent_DrawEntity(world, ent);
		if(Bt2Ent_CheckRayEnt(world, spos, epos, ent))
			{ robj[n++]=ent; }
	}
	
	*rnobj=n;
	return(n>0);
}

BTEIFGL_API int Bt2Ent_RayCastEntity(
	BGBDT_VoxWorld *world, dtVal self,
	BGBDT_VoxCoord spos, BGBDT_VoxCoord epos,
	BGBDT_VoxCoord *rpos, dtVal *robj, int tracefl)
{
	dtVal tobj[64];
	vec3d sorg, eorg;
	BGBDT_VoxCoord pos;
	dtVal ent;
	double d, bd;
	int ntobj;
	int i, j, k;

	i=Bt2Ent_RayQueryEntity(world, self, spos, epos, tobj, &ntobj, tracefl);
	if(i<=0)return(0);
	
	ent=tobj[0];

	if(ntobj>1)
	{
		sorg=vec3d(
			spos.x*BGBDT_XYZ_SCALE_TOMETER,
			spos.y*BGBDT_XYZ_SCALE_TOMETER,
			spos.z*BGBDT_XYZ_SCALE_TOMETER);

		bd=9999999;
		for(i=0; i<ntobj; i++)
		{
			eorg=Bt2Ent_EntGetOrigin(tobj[i]);
			d=v3ddist(sorg, eorg);
			if(d<bd)
				{ bd=d; ent=tobj[i]; }
		}
	}

	eorg=Bt2Ent_EntGetOrigin(ent);
	pos.x=v3dx(eorg)*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	pos.y=v3dy(eorg)*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	pos.z=v3dz(eorg)*BGBDT_XYZ_SCALE_FROMMETER+0.5;
	
	*rpos=pos;
	*robj=ent;
	return(1);
}


int bt2ent_movetick_checkmove(dtVal ent, double *porg,
	double *relmin, double *relmax)
{
	BGBDT_VoxCoord min, max;
	dtVal ents[16];
	BGBDT_VoxCoord pos[16];
	BGBDT_VoxData td[16];
	double pomin[4], pomax[4];
	int nvox, vfl, nent;
	int i;

	pomin[0]=porg[0]+relmin[0];
	pomin[1]=porg[1]+relmin[1];
	pomin[2]=porg[2]+relmin[2];

	pomax[0]=porg[0]+relmin[0];
	pomax[1]=porg[1]+relmax[1];
	pomax[2]=porg[2]+relmax[2];

	min.x=pomin[0]*BGBDT_XYZ_SCALE_FROMMETER;
	min.y=pomin[1]*BGBDT_XYZ_SCALE_FROMMETER;
	min.z=pomin[2]*BGBDT_XYZ_SCALE_FROMMETER;
	max.x=pomax[0]*BGBDT_XYZ_SCALE_FROMMETER;
	max.y=pomax[1]*BGBDT_XYZ_SCALE_FROMMETER;
	max.z=pomax[2]*BGBDT_XYZ_SCALE_FROMMETER;

//	i=BGBDT_BoxQueryVoxel(bgbdt_voxworld, min, max, pos, td, &nvox,
//		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);
	i=BGBDT_BoxQueryVoxel(bt2ent_voxworld, min, max, pos, td, &nvox,
		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_SOLID);
	if(i)return(i);

	i=BGBDT_BoxQueryVoxel(bt2ent_voxworld, min, max, pos, td, &nvox,
		BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);

	if(i>0)
	{
		for(i=0; i<nvox; i++)
		{
			vfl=BGBDT_WorldVoxel_GetFlags(bt2ent_voxworld, pos[i], td[i]);
//			if((vfl&BGBDT_VOXFL_PHYS_MASK)==BGBDT_VOXFL_PHYS_FLIPUP)
//				isotest_player_flip=1;
//			if((vfl&BGBDT_VOXFL_PHYS_MASK)==BGBDT_VOXFL_PHYS_FLIPDN)
//				isotest_player_flip=0;
		}
	}

	i=Bt2Ent_BoxQueryWorldEnts(bt2ent_voxworld, ent, min, max,
		ents, &nent, BGBDT_TRFL_NOLOAD|BGBDT_TRFL_NONEMPTY);
	if(i>0)
	{
		return(1);
	}

	return(0);
}

BTEIFGL_API int Bt2Ent_EntityMoveCheckCollide(dtVal ent, void *rpos)
{
	double eorg[3], emin[3], emax[3];
	BGBDT_VoxCoord min2, max2;
//	vec3d eorg, corg;
	double erad, ehgt;
	s64 efl;
	int i;

//	eorg=Bt2Ent_EntGetOrigin(ent);
	erad=Bt2Ent_EntGetRadius(ent);
	ehgt=Bt2Ent_EntGetHeight(ent);
	efl=Bt2Ent_EntGetFlags(ent);
	
	erad=erad/32;
	ehgt=ehgt/32;

	BSVM2_Unpack3XfTo3Dv(rpos, eorg);

	emin[0]=0-erad;
	emin[1]=0-erad;
	emax[0]=0+erad;
	emax[1]=0+erad;

	if(efl&BGBDT_ENTFL_ZFLIP)
	{
		emin[2]=0-ehgt;
		emax[2]=0;
	}else
	{
		emin[2]=0;
		emax[2]=0+ehgt;
	}

	i=bt2ent_movetick_checkmove(ent, eorg, emin, emax);
	return(i);
}


BTEIFGL_API int Bt2Ent_EmitParticleExplosionBasic(dtVal ent,
	int cnt, int atxy, u32 clr)
{
	vec3d org;
	
	org=Bt2Ent_EntGetOrigin(ent);

	BGBDT_Part_ParticleExplosionBasic(org,
		vec3(0,0,0), vec3(25,25,25),
		cnt, atxy, clr);
}

BTEIFGL_API int Bt2Ent_EmitParticleEffectPara(dtVal ent, dtVal args)
{
	float tv[4];
	BGBDT_ParticleEmission tpara;
	BGBDT_ParticleEmission *para;
	dtVal n0, n1, n2, n3;
	char *s0;
	int i, j, k, l;

	para=&tpara;

	l=dtvArrayGetSize(args);
	BGBDT_Part_ParticleInitPara(para);
	para->org=Bt2Ent_EntGetOrigin(ent);

	for(i=0; i<l; i++)
	{
		n0=dtvArrayGetIndexDtVal(args, i);
		if(!BGBDT_TagStr_IsKeywordP(n0))
			continue;
		s0=BGBDT_TagStr_GetUtf8(n0);
		if(!s0)continue;

		i++;
		if(i>=l)break;
		n0=dtvArrayGetIndexDtVal(args, i);
		
		if(!strcmp(s0, "count"))
			{ para->cnt=dtvUnwrapInt(n0); }

		if(!strcmp(s0, "atxy"))
			{ para->atxy=dtvUnwrapInt(n0); }
		if(!strcmp(s0, "zacc"))
		{
			j=dtvUnwrapFloat(n0)*4+128;
			if(j<0)j=0;
			if(j>255)j=255;
			para->zacc=j;
		}

		if(!strcmp(s0, "clr"))
			{ para->clr=dtvUnwrapInt(n0); }
		if(!strcmp(s0, "clrvel"))
			{ para->clrvel=dtvUnwrapInt(n0); }

		if(!strcmp(s0, "rad"))
			{ para->rad=dtvUnwrapInt(n0); }
		if(!strcmp(s0, "ttl"))
			{ para->ttl=dtvUnwrapInt(n0); }

		if(!strcmp(s0, "bvel"))
		{
			BGBDT_XV128_GetVec3fv(n0, tv);
			para->bvel=vec3(tv[0], tv[1], tv[2]);
		}

		if(!strcmp(s0, "rvel"))
		{
			BGBDT_XV128_GetVec3fv(n0, tv);
			para->rvel=vec3(tv[0], tv[1], tv[2]);
		}
	}
	
	BGBDT_Part_ParticleEmitPara(para);
}
