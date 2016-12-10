dtcClass bt2ent_npcdiag_cls;
dtcField bt2ent_npcdiag_fi_face1;
dtcField bt2ent_npcdiag_fi_face2;
dtcField bt2ent_npcdiag_fi_text;
dtcField bt2ent_npcdiag_fi_voice;
dtcField bt2ent_npcdiag_fi_options;

dtcField bt2ent_npcdiag_fi_impulse;
dtcField bt2ent_npcdiag_fi_setup;

// BGBDT_IsoMap *bt2ent_map;
// float bt2ent_org[3];
// float bt2ent_ang[3];
// float bt2ent_rot[9];
// float bt2ent_vel[3];
// byte bt2ent_pldir;
// byte bt2ent_iswalk;
// byte bt2ent_isdead;
// byte bt2ent_onground;
// byte bt2ent_isback;
// byte bt2ent_isball;

// byte bt2ent_player_noclip;
// byte bt2ent_player_undying;
int bt2ent_wrlflag;

dtVal bt2ent_player;
dtVal bt2ent_diagbox;
int bt2ent_diag_pchars;
int bt2ent_diag_lpchars;
int bt2ent_diag_sndid;

int bt2ent_snd_bgmid;
char *bt2ent_usebkg;

double bt2ent_diag_ptime;
double bt2ent_worldtime;

int bt2ent_invopen;
int bt2ent_invslot[8*16];

s64 bt2ent_tokenslot[1024];
int bt2ent_ntoken;


BTEIFGL_API int Bt2Ent_DialogActiveP(void)
{
	if(!dtvNullP(bt2ent_diagbox))
		return(1);
	return(0);
}

BTEIFGL_API int Bt2Ent_DialogImpulse(int imp)
{
	if(dtvNullP(bt2ent_diagbox))
		return(0);
	Bt2Ent_CallDiagImpulse(bt2ent_diagbox, imp);
	return(0);
}

BTEIFGL_API int Bt2Ent_InvenActiveP(void)
{
	return(bt2ent_invopen);
}

BTEIFGL_API int Bt2Ent_DrawDialog(void)
{
	if(Bt2Ent_DialogActiveP())
	{
		Bt2Ent_DrawDialogBox(bt2ent_diagbox);
	}else
	{
		if(bt2ent_diag_sndid>0)
		{
			BGBDT_Sound_DeleteMixChan(bt2ent_diag_sndid);
			bt2ent_diag_sndid=0;
		}
	}

	if(bt2ent_invopen)
	{
		Bt2Ent_DrawInventory();
	}

//	if(bt2ent_isdead)
//	{
//		Bt2Ent_DrawDeadScreen();
//	}

	return(0);
}

BTEIFGL_API int Bt2Ent_InvenSetActive(int val)
{
	bt2ent_invopen=val;
	return(0);
}

BTEIFGL_API void Bt2Ent_SetDialog(dtVal dbox)
{
	if(bt2ent_diag_sndid>0)
	{
		BGBDT_Sound_DeleteMixChan(bt2ent_diag_sndid);
		bt2ent_diag_sndid=0;
	}

	bt2ent_diagbox=dbox;
	bt2ent_diag_pchars=0;
	bt2ent_diag_ptime=frgl_state->time_f;
	
	if(dtvTrueP(dbox))
		{ Bt2Ent_CallDiagSetup(dbox); }
}

BTEIFGL_API void Bt2Ent_SetUseBackground(char *bkg)
{
//	bt2ent_diagbox=dbox;
//	bt2ent_diag_pchars=0;
//	bt2ent_diag_ptime=frgl_state->time_f;
	if(!bkg)
	{
		bt2ent_usebkg=NULL;
		return;
	}

	bt2ent_usebkg=frgl_strdup(bkg);
}

BTEIFGL_API void Bt2Ent_SetApplyWorldFlags(int flag)
{
	bt2ent_wrlflag|=flag;
}

BTEIFGL_API void Bt2Ent_SetClearWorldFlags(int flag)
{
	bt2ent_wrlflag&=~flag;
}

BTEIFGL_API int Bt2Ent_GetInvenSlot(int slot)
{
	if((slot<0) || (slot>=(8*16)))
		return(0);
	return(bt2ent_invslot[slot]);
}

BTEIFGL_API int Bt2Ent_SetInvenSlot(int slot, int val)
{
	int oval;

	if((slot<0) || (slot>=(8*16)))
		return(0);
	oval=bt2ent_invslot[slot];
	bt2ent_invslot[slot]=val;
	return(oval);
}

BTEIFGL_API int Bt2Ent_ClearInven(void)
{
	int i;
	for(i=0; i<(8*16); i++)
		bt2ent_invslot[i]=0;
	return(0);
}

BTEIFGL_API s64 Bt2Ent_GetTokenSlot(int slot)
{
	if((slot<0) || (slot>=1024))
		return(0);
	return(bt2ent_tokenslot[slot]);
}

BTEIFGL_API int Bt2Ent_SetTokenSlot(int slot, s64 val)
{
	int oval;

	if((slot<0) || (slot>=1024))
		return(0);
		
	if(slot>=bt2ent_ntoken)
		bt2ent_ntoken=slot+1;

	oval=bt2ent_tokenslot[slot];
	bt2ent_tokenslot[slot]=val;
	return(oval);
}

BTEIFGL_API int Bt2Ent_GetNumToken(void)
{
	return(bt2ent_ntoken);
}

BTEIFGL_API int Bt2Ent_ClearTokens(void)
{
	bt2ent_ntoken=0;
	return(0);
}

BTEIFGL_API void Bt2Ent_GiveItem(int item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return;

	for(i=0; i<6*16; i++)
	{
//		if((bt2ent_invslot[i]&255)==(item&255))
		if(((bt2ent_invslot[i]&255)==(item&255)) &&
			((bt2ent_invslot[i]>>16)==(item>>16)))
		{
			i0=((bt2ent_invslot[i]>>8)&255)+1;
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
			j=bt2ent_invslot[i];
			j=j&(~(0xFF00));
			j=j|((i0-1)<<8);
			bt2ent_invslot[i]=j;
			
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
		if(!(bt2ent_invslot[i]&255))
		{
			bt2ent_invslot[i]=item;
			item=0;
			break;
		}
	}

	if(!item)
		return;

	/* drop item */
}


BTEIFGL_API int Bt2Ent_TakeItem(int item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return(0);

	for(i=0; i<6*16; i++)
	{
//		if((bt2ent_invslot[i]&255)==(item&255))
		if(((bt2ent_invslot[i]&255)==(item&255)) &&
			((bt2ent_invslot[i]>>16)==(item>>16)))
		{
			i0=((bt2ent_invslot[i]>>8)&255)+1;
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
				j=bt2ent_invslot[i];
				j=j&(~(0xFF00));
				j=j|((i0-1)<<8);
				bt2ent_invslot[i]=j;
			}else
			{
				bt2ent_invslot[i]=0;
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

BTEIFGL_API int Bt2Ent_CheckItem(int item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return(0);

	for(i=0; i<6*16; i++)
	{
//		if((bt2ent_invslot[i]&255)==(item&255))
		if(((bt2ent_invslot[i]&255)==(item&255)) &&
			((bt2ent_invslot[i]>>16)==(item>>16)))
		{
			i0=((bt2ent_invslot[i]>>8)&255)+1;
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
				j=bt2ent_invslot[i];
				j=j&(~(0xFF00));
				j=j|((i0-1)<<8);
//				bt2ent_invslot[i]=j;
			}else
			{
//				bt2ent_invslot[i]=0;
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


#if 1
BTEIFGL_API void Bt2Ent_GiveToken(s64 item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return;

	for(i=0; i<bt2ent_ntoken; i++)
	{
		if(((bt2ent_tokenslot[i]&255)==(item&255)) &&
			((bt2ent_tokenslot[i]>>16)==(item>>16)))
		{
			i0=((bt2ent_tokenslot[i]>>8)&255)+1;
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
			j=bt2ent_tokenslot[i];
			j=j&(~(0xFF00));
			j=j|((i0-1)<<8);
			bt2ent_tokenslot[i]=j;
			
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

	for(i=0; i<bt2ent_ntoken; i++)
	{
		if(!(bt2ent_tokenslot[i]&255))
		{
			bt2ent_tokenslot[i]=item;
			item=0;
			break;
		}
	}

	if(!item)
		return;

	i=bt2ent_ntoken++;
	bt2ent_tokenslot[i]=item;

	/* drop item */
}


BTEIFGL_API int Bt2Ent_TakeToken(s64 item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return(0);

	for(i=0; i<bt2ent_ntoken; i++)
	{
		if(((bt2ent_tokenslot[i]&255)==(item&255)) &&
			((bt2ent_tokenslot[i]>>16)==(item>>16)))
		{
			i0=((bt2ent_tokenslot[i]>>8)&255)+1;
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
				j=bt2ent_tokenslot[i];
				j=j&(~(0xFF00));
				j=j|((i0-1)<<8);
				bt2ent_tokenslot[i]=j;
			}else
			{
				bt2ent_tokenslot[i]=0;
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

BTEIFGL_API int Bt2Ent_CheckToken(s64 item)
{
	int i0, i1, i2, i3;
	int i, j, k;

	if(!(item&255))
		return(0);

	for(i=0; i<bt2ent_ntoken; i++)
	{
		if(((bt2ent_tokenslot[i]&255)==(item&255)) &&
			((bt2ent_tokenslot[i]>>16)==(item>>16)))
		{
			i0=((bt2ent_tokenslot[i]>>8)&255)+1;
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
				j=bt2ent_tokenslot[i];
				j=j&(~(0xFF00));
				j=j|((i0-1)<<8);
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
#endif

BTEIFGL_API int Bt2Ent_SetBgm(char *bgm)
{
	char tb[256];
	BGBDT_SndMixChan *chn;

	chn=BGBDT_Sound_LookupMixChan(bt2ent_snd_bgmid);
	if(chn && !strcmp(chn->samp->name, bgm))
		return(0);

	sprintf(tb, "%s.wav", bgm);
	if(chn && !strcmp(chn->samp->name, tb))
		return(0);

	//bgm="sound/pi0_amb0";

	BGBDT_Sound_DeleteMixChan(bt2ent_snd_bgmid);
	bt2ent_snd_bgmid=BGBDT_Sound_PlaySound(bgm,
		64, BGBDT_SNDATT_NONE, BGBDT_SNDFL_LOOP);
	return(1);
}

char *Bt2Ent_DiagGetFace1(dtVal obj)
{
	dtVal v;

	if(!bt2ent_npcdiag_fi_face1)
	{
		bt2ent_npcdiag_fi_face1=BGBDTC_LookupClassSlotName(
			bt2ent_npcdiag_cls, "face1");
		if(!bt2ent_npcdiag_fi_face1)
			return(NULL);
	}

	v=dtcVaGetA(obj, bt2ent_npcdiag_fi_face1);
	return(BGBDT_TagStr_GetUtf8(v));
}

char *Bt2Ent_DiagGetFace2(dtVal obj)
{
	dtVal v;

	if(!bt2ent_npcdiag_fi_face2)
	{
		bt2ent_npcdiag_fi_face2=BGBDTC_LookupClassSlotName(
			bt2ent_npcdiag_cls, "face2");
		if(!bt2ent_npcdiag_fi_face2)
			return(NULL);
	}

	v=dtcVaGetA(obj, bt2ent_npcdiag_fi_face2);
	return(BGBDT_TagStr_GetUtf8(v));
}

char *Bt2Ent_DiagGetText(dtVal obj)
{
	dtVal v;

	if(!bt2ent_npcdiag_fi_text)
	{
		bt2ent_npcdiag_fi_text=BGBDTC_LookupClassSlotName(
			bt2ent_npcdiag_cls, "text");
		if(!bt2ent_npcdiag_fi_text)
			return(NULL);
	}

	v=dtcVaGetA(obj, bt2ent_npcdiag_fi_text);
	return(BGBDT_TagStr_GetUtf8(v));
}

dtVal Bt2Ent_DiagGetOptions(dtVal obj)
{
	dtVal v;

	if(!bt2ent_npcdiag_fi_options)
	{
		bt2ent_npcdiag_fi_options=BGBDTC_LookupClassSlotName(
			bt2ent_npcdiag_cls, "options");
		if(!bt2ent_npcdiag_fi_options)
			return(DTV_NULL);
	}

	v=dtcVaGetA(obj, bt2ent_npcdiag_fi_options);
	return(v);
}

char *Bt2Ent_DiagGetVoice(dtVal obj)
{
	dtVal v;

	if(!bt2ent_npcdiag_fi_voice)
	{
		bt2ent_npcdiag_fi_voice=BGBDTC_LookupClassSlotName(
			bt2ent_npcdiag_cls, "voice");
		if(!bt2ent_npcdiag_fi_voice)
			return(NULL);
	}

	v=dtcVaGetA(obj, bt2ent_npcdiag_fi_voice);
	return(BGBDT_TagStr_GetUtf8(v));
}

int Bt2Ent_CallDiagImpulse(dtVal self, int imp)
{
	static dtcMethod fi=NULL;
	BSVM2_Value targs[4];
	int i;

	targs[0].i=imp;
	i=BSVM2_Interp_CallCacheMethodVM(self, &fi, targs,
		"NpcDialogBox", "impulse", NULL, 999999999);
	if(i)printf("Bt2Ent_CallDiagImpulse: Status=%d\n", i);
	return(0);
}

int Bt2Ent_CallDiagSetup(dtVal self)
{
	static dtcMethod fi=NULL;
	BSVM2_Value targs[4];
	int i;

//	targs[0].i=imp;
	i=BSVM2_Interp_CallCacheMethodVM(self, &fi, targs,
		"NpcDialogBox", "setup", NULL, 999999999);
	if(i)printf("Bt2Ent_CallDiagImpulse: Status=%d\n", i);
	return(0);
}



int Bt2Ent_DrawDialogBox(dtVal dbox)
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

	face1=Bt2Ent_DiagGetFace1(dbox);
	face2=Bt2Ent_DiagGetFace2(dbox);
	text=Bt2Ent_DiagGetText(dbox);
	opts=Bt2Ent_DiagGetOptions(dbox);
	voc=Bt2Ent_DiagGetVoice(dbox);
	
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
		if(bt2ent_diag_pchars<l)
		{
			i=((int)(frgl_state->time_f*8))&1;
			if(i)sf0=face2;

			if(bt2ent_diag_sndid<=0)
			{
				bt2ent_diag_sndid=BGBDT_Sound_PlaySound(
					voc, 32, 0, BGBDT_SNDFL_LOOP);
			}
		}else
		{
			if(bt2ent_diag_sndid>0)
			{
				BGBDT_Sound_DeleteMixChan(bt2ent_diag_sndid);
				bt2ent_diag_sndid=0;
			}
		}
	}
	
	bt2ent_diag_pchars=(frgl_state->time_f-bt2ent_diag_ptime)*24;
	if((bt2ent_diag_pchars<l) &&
		(bt2ent_diag_pchars!=bt2ent_diag_lpchars))
	{
//		bt2ent_diag_sndid=BGBDT_Sound_PlaySound(
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
		if(bt2ent_diag_pchars<l)
		{
			strcpy(tb, text);
			tb[bt2ent_diag_pchars]=0;
			GfxFont_DrawString2(tb, -320+4+8, -8-4, 8, 8, 0, 0, 0, 255);
		}else
		{
	//		GfxFont_DrawString(text, -320+4, -8-4, 8, 8, 0, 0, 0, 255);
			GfxFont_DrawString2(text, -320+4+8, -8-4, 8, 8, 0, 0, 0, 255);
		}
	}
	return(0);
}

int Bt2Ent_DrawInventory(void)
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

//	bt2ent_invslot[0*16+1]=1;

	frglBegin(GL_QUADS);
	for(i=0; i<6; i++)
	{
		for(j=0; j<16; j++)
		{
			k=bt2ent_invslot[i*16+j];

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
			k=bt2ent_invslot[i*16+j];
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

int Bt2Ent_DrawDeadScreen(void)
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

int Bt2Ent_DrawUseBackground(void)
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

	if(!bt2ent_usebkg)
		return(0);

	x0=-512; y0=-384;
	x1=512; y1=384;

	tex=Tex_LoadFile(bt2ent_usebkg, NULL, NULL);

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
