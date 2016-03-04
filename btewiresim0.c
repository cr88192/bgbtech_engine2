#include <bteifgl.h>
#include <bteifgl_main.h>

BTEWS_WireGrid *btews_grid=NULL;
BTEWS_Component *btews_component=NULL;

int main_prestart(int argc, char *argv[])
{
	GfxDrv_SetDefaults("BTE WireSim", 1024, 768);
	FRGL_CvarSetDefault("hide_os_cursor", "1");

	Con_ExecCmdBuf("config.cfg");

	return(0);
}

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

void btews_vccgnd_Update(BTEWS_WireNode *self, double dt)
{
	double v, w;
	
	v=self->vc0;
	self->vnx=v;	self->vpx=v;
	self->vny=v;	self->vpy=v;
	
	w=100000;
	self->pwnx=w;	self->pwpx=w;
	self->pwny=w;	self->pwpy=w;
	self->pwatt=w;
	self->dsttl=4096;
}

void btews_DrawDefault(BTEWS_WireGrid *grid, int x, int y,
	float x0, float y0, float x1, float y1)
{
	BTEWS_WireNode *ncur;
	int mx, my, mb;
	int tex;
	
	mx=frgl_state->mx;
	my=-frgl_state->my;
	mb=frgl_state->mb;
	
	tex=Tex_LoadFile("images/openhole0.png", NULL, NULL);

//	frglColor4f(1, 1, 1, 1);
	frglColor4f(0.5, 0.5, 0.5, 1);
	
	if((mx>=x0) && (mx<=x1) &&
		(my>=y0) && (my<=y1))
	{
//		frglColor4f(0.5, 1, 0.5, 1);
		frglColor4f(0.25, 0.5, 0.25, 1);
		
		if(mb&1)
		{
			if(btews_component)
			{
				if(btews_component->Place)
				{
					btews_component->Place(btews_component,
						grid, x, y);
				}
				return;
			}

#if 0
			frglColor4f(1, 0.5, 1, 1);
			ncur=BTEWS_GetAllocNode(grid, x, y);
			ncur->Draw=btews_wire_Draw;
			ncur->Update=btews_wire_Update;
			ncur->wfl=3;
			ncur->ohm=0.00022;
			ncur->temp=25;
			return;
#endif
		}
	}

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

int main_startup(int argc, char *argv[])
{
	BTEWS_Component *cmp;
	BTEWS_WireNode *ncur;

	Tex_Init2();
	GfxFont_Init();
	Con_Init();
//	ConCmds_Init();

//	NET_InitLow();
//	BTEIFGL_NET_Init();

	cmp=BTEWS_GetComponent("R102");
	cmp->Place=btews_resistor_Place;
	cmp->image="images/resistor0.png";

	cmp=BTEWS_GetComponent("D1");
	cmp->Place=btews_diode_Place;
	cmp->image="images/diode0.png";

	cmp=BTEWS_GetComponent("PNP");
	cmp->Place=btews_transistor_Place;
	cmp->image="images/bjt_to220_pnp1.png";
	cmp->cty=BTEWS_CTY_PNP;

	cmp=BTEWS_GetComponent("NPN");
	cmp->Place=btews_transistor_Place;
	cmp->image="images/bjt_to220_npn1.png";
	cmp->cty=BTEWS_CTY_NPN;

	cmp=BTEWS_GetComponent("PNPDar");
	cmp->Place=btews_transistor_Place;
	cmp->image="images/bjt_to220_pnpdar1.png";
	cmp->cty=BTEWS_CTY_PNPDAR;

	cmp=BTEWS_GetComponent("NPNDar");
	cmp->Place=btews_transistor_Place;
	cmp->image="images/bjt_to220_npndar1.png";
	cmp->cty=BTEWS_CTY_NPNDAR;

	cmp=BTEWS_GetComponent("PMOS");
	cmp->Place=btews_transistor_Place;
	cmp->image="images/bjt_to220_pmos1.png";
	cmp->cty=BTEWS_CTY_PMOS;

	cmp=BTEWS_GetComponent("NMOS");
	cmp->Place=btews_transistor_Place;
	cmp->image="images/bjt_to220_nmos1.png";
	cmp->cty=BTEWS_CTY_NMOS;

	cmp=BTEWS_GetComponent("DIP14");
	cmp->Place=btews_dip14_Place;
	cmp->image="images/base_dip14.png";
	cmp->cty=BTEWS_CTY_NMOS;


	cmp=BTEWS_GetComponent("W24");
	cmp->Place=btews_wire_Place;
	cmp->image=NULL;
	cmp->cty=BTEWS_CTY_WIRE;

	cmp=BTEWS_GetComponent("DSW");
	cmp->Place=btews_wire_PlaceDigital;
	cmp->image=NULL;
	cmp->cty=BTEWS_CTY_DGTWIRE;

	btews_component=cmp;

	btews_grid=BTEWS_AllocGrid(27, 21);
	btews_grid->DrawDefault=btews_DrawDefault;

	btews_grid->sim_ct=0;
	btews_grid->sim_accdt=0;
	btews_grid->sim_tf=1.0;
//	btews_grid->sim_tfs=0.0001;
	btews_grid->sim_tfs=0.00005;

	ncur=BTEWS_GetAllocNode(btews_grid, 0, 0);
	ncur->Draw=btews_vccgnd_Draw;
	ncur->Update=btews_vccgnd_Update;
	ncur->wfl=3;
	ncur->vc0=0.0;
	BTEWS_MarkNodeDirtyStar(btews_grid, 0, 0);

	ncur=BTEWS_GetAllocNode(btews_grid, 0, 20);
	ncur->Draw=btews_vccgnd_Draw;
	ncur->Update=btews_vccgnd_Update;
	ncur->wfl=3;
	ncur->vc0=12.0;
	BTEWS_MarkNodeDirtyStar(btews_grid, 0, 20);

	ncur=BTEWS_GetAllocNode(btews_grid, 26, 20);
	ncur->Draw=btews_vccgnd_Draw;
	ncur->Update=btews_vccgnd_Update;
	ncur->wfl=3|BTEWS_WFL_DGTL_VCC;
	ncur->vc0=3.3;
	BTEWS_MarkNodeDirtyStar(btews_grid, 26, 20);

	return(0);
}

int main_handle_input()
{
	char buf[64];
	float mins[3], maxs[3];
	float io[3], iv[4], fw[2], pt[4], f, g, v;
	float im[16], im1[16], *imv;

	int i, j, k;
	unsigned short *kcur;

	if(Con_IsDown())
		return(0);

//	if(PDGLUI_HasFocus())
//		return(0);
//	UI_Camera_Update();

	kcur=frgl_state->keys;
	while(*kcur)
	{
		switch(*kcur)
		{
		case 'q':
			btews_component=BTEWS_LookupComponent("DSW");
			break;

		case 'w':
			btews_component=BTEWS_LookupComponent("W24");
			break;
		case 'r':
			btews_component=BTEWS_LookupComponent("R102");
			break;

		case 'd':
			btews_component=BTEWS_LookupComponent("D1");
			break;

		case 'p':
			btews_component=BTEWS_LookupComponent("PNP");
			break;
		case 'n':
			btews_component=BTEWS_LookupComponent("NPN");
			break;

		case 'P':
			btews_component=BTEWS_LookupComponent("PMOS");
			break;
		case 'N':
			btews_component=BTEWS_LookupComponent("NMOS");
			break;

		case 'D':
			btews_component=BTEWS_LookupComponent("DIP14");
			break;

		case K_DEL:
			BTEWS_DeleteNode(btews_grid,
				btews_grid->mgx, btews_grid->mgy);
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
	char tb[1024];

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

//	mgx=(mx+16)/32+(btews_grid->xs>>1);
//	mgy=(my+16)/32+(btews_grid->ys>>1);

	mgx=(mx+16*btews_grid->xs)/32;
	mgy=(my+16*btews_grid->ys)/32;
	
	btews_grid->mgx=mgx;
	btews_grid->mgy=mgy;

	main_handle_input();
//	BTEIFGL_NET_Update(frgl_state->dt_f);

	BTEWS_UpdateGrid(btews_grid, frgl_state->dt_f);

	tex=Tex_LoadFile("images/openhole0.png", NULL, NULL);

	FRGL_Setup2D();

	f=-(btews_grid->xs*0.5)*32;
	g=-(btews_grid->ys*0.5)*32;
	BTEWS_DrawGrid(btews_grid, f, g, 32, 32);

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

#if 0
	for(i=0; i<21; i++)
		for(j=0; j<27; j++)
	{
		x0=(j-13)*32;
		y0=(i-10)*32;
		x1=x0+32;
		y1=y0+32;
	
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

#if 0
	l=BTIC1G_NET_GetStreamTexture(0);

	frglColor4f(1, 1, 1, 1);

	frglEnableTexture2D();
	frglBindTexture2D(l);
	frglBegin(GL_QUADS);
//	frglTexCoord2f(0, 0);
	frglTexCoord2f(0, 480/512.0);
	frglVertex2f(-320, -180);
//	frglTexCoord2f(640/1024.0, 0);
	frglTexCoord2f(640/1024.0, 480/512.0);
	frglVertex2f(160, -180);
//	frglTexCoord2f(640/1024.0, 480/512.0);
	frglTexCoord2f(640/1024.0, 0);
	frglVertex2f(160, 180);
//	frglTexCoord2f(0, 480/512.0);
	frglTexCoord2f(0, 0);
	frglVertex2f(-320, 180);
	frglEnd();

//	Con_Render();

	frglDisableTexture2D();
	frglColor4f(1, 0, 0, 1);
	
	imp0=0; imp1=0;

#if 1
	frglBegin(GL_QUADS);
	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
	{
//		if(1)
		if(FRGL_KeyDown(K_NUMPAD1+(i*3+j)))
		{
			imp0|=1<<(i*3+j+1);
			frglColor4f(1, 0, 0, 1);
		}else
		{
			frglColor4f(0.5, 0, 0, 0.25);
		}
		frglVertex2f(192+(j*2+0)*16, (i*2+1)*16);
		frglVertex2f(192+(j*2+0)*16, (i*2+0)*16);
		frglVertex2f(192+(j*2+1)*16, (i*2+0)*16);
		frglVertex2f(192+(j*2+1)*16, (i*2+1)*16);
	}
	frglEnd();
#endif

#if 1
	act=BTEIFGL_NET_GetPinsActive();
	mask=BTEIFGL_NET_GetPinsMask();
	pin=BTEIFGL_NET_GetPinsInMask();
	pout=BTEIFGL_NET_GetPinsOutMask();
	frglBegin(GL_QUADS);
	for(i=0; i<8; i++)
		for(j=0; j<4; j++)
	{
		k=i*4+j;
		l=BTEIFGL_NET_GetPinsTarget(k);
		f=l/255.0;

		if((act&(1<<k)) && (mask&(1<<k)))
		{
			cr=0; cg=0.25+f*0.75; cb=0;
//			frglColor4f(0, 0.25+f*0.75, 0, 0.75);
		}else if(mask&(1<<k))
		{
			cr=0.25+f*0.5; cg=0; cb=0;
//			frglColor4f(0.25+f*0.25, 0, 0, 0.75);
		}else
		{
			cr=0.25+f*0.25; cg=cr; cb=cr;
//			frglColor4f(0.25+f*0.25, 0.25+f*0.25, 0.25+f*0.25, 0.75);
		}
		
		if(pin&(1<<k))
			{ cb+=0.5; }
		if(pout&(1<<k))
			{ cr+=0.5; cg+=0.5; }

		frglColor4f(cr, cg, cb, 0.75);

		frglVertex2f(192+(j*2+0)*8, -(i*2+1)*8-16);
		frglVertex2f(192+(j*2+0)*8, -(i*2+0)*8-16);
		frglVertex2f(192+(j*2+1)*8, -(i*2+0)*8-16);
		frglVertex2f(192+(j*2+1)*8, -(i*2+1)*8-16);
	}
	frglEnd();
#endif

	BTEIFGL_NET_SetImpulse(imp0, imp1);
#endif

	frglEnableTexture2D();

	Con_Render();

//	frglDisableTexture2D();

//	s="images/resistor0.png";
	s=NULL;

	if(btews_component)
		{ s=btews_component->image; }

#if 1
	if(s)
	{
		tex=Tex_LoadFile(s, &txs, &tys);

		x0=frgl_state->mx-(txs/4);
		x1=frgl_state->mx+(txs/4);
		y0=-frgl_state->my-(tys/4);
		y1=-frgl_state->my+(tys/4);

		frglColor4f(192, 255, 192, 0.75);

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
