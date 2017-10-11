#include <bteifgl.h>
#include <bteifgl_main.h>

int swr_screentex;
u32 *swr_screenbuf;
BGBDT_SwContext *swr_ctx;
BGBDT_SwTexture *swr_tex0;

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

int main_startup(int argc, char *argv[])
{
	byte *ibuf;
	int xs, ys;

	Tex_Init2();
	GfxFont_Init();
//	Con_Init();
//	ConCmds_Init();

	Con_ReInit();

//	SoundDev_Init();

//	frgl_state->maxhz=60;
	frgl_state->maxhz=85;
	
	swr_screentex=Tex_AllocTexnum();
	swr_screenbuf=malloc(1024*512*4);
	
	memset(swr_screenbuf, 255, 1024*512*4);
	
	swr_ctx=BGBDT_SWR_AllocContext(640, 480);

	swr_tex0=BGBDT_SWR_GetTexture(swr_ctx, 1);

	ibuf=Tex_LoadFile2Raw("textures/rrock14", &xs, &ys);
	if(ibuf)
	{
		BGBDT_SWR_UpdateTextureImage(
			swr_ctx, swr_tex0,
			(u32 *)ibuf, 0, xs, ys,
			BTIC1H_PXF_RGBA);
	}

	return(0);
}

int main_body()
{
	BGBDT_SwTriangle *tri;
	float x, y, x0, y0, x1, y1, x2, y2, a;
	int xs, ys;
	
	BGBDT_SWR_ClearContext(swr_ctx);
	
	tri=BGBDT_SWR_AllocTriangle(swr_ctx);

#if 0
	tri->v0.x=64;	tri->v0.y=64;	tri->v0.z=64;
	tri->v0.s=0;	tri->v0.t=0;

	tri->v1.x=128;	tri->v1.y=128;	tri->v1.z=64;
	tri->v1.s=0;	tri->v1.t=0;

	tri->v2.x=64;	tri->v2.y=192;	tri->v2.z=64;
	tri->v2.s=0;	tri->v2.t=0;
#endif

#if 1
	a=frgl_state->time_f;

	x=64; y=64;
	x0=x*cos(a)+y*sin(a);
	y0=y*cos(a)-x*sin(a);

	x=128; y=128;
	x1=x*cos(a)+y*sin(a);
	y1=y*cos(a)-x*sin(a);

	x=64; y=192;
	x2=x*cos(a)+y*sin(a);
	y2=y*cos(a)-x*sin(a);
	
	x0+=320;	y0+=240;
	x1+=320;	y1+=240;
	x2+=320;	y2+=240;

	tri->v0.x=x0;	tri->v0.y=y0;	tri->v0.z=64;
	tri->v0.s=0x0000;	tri->v0.t=0x0000;
	tri->v0.clr=0xFFFFFF00;
//	tri->v0.clr=0xFFFF00FF;
//	tri->v0.clr=0xFF00FFFF;
//	tri->v0.clr=0x9FFFFFFF;
	
	tri->v1.x=x1;	tri->v1.y=y1;	tri->v1.z=64;
	tri->v1.s=0xFFFF;	tri->v1.t=0x8000;
//	tri->v1.clr=0xFFFFFF00;
	tri->v1.clr=0xFFFF00FF;
//	tri->v1.clr=0xFF00FFFF;
//	tri->v1.clr=0x9FFFFFFF;

	tri->v2.x=x2;	tri->v2.y=y2;	tri->v2.z=64;
	tri->v2.s=0x0000;	tri->v2.t=0xFFFF;
//	tri->v2.clr=0xFFFFFF00;
	tri->v2.clr=0xFF00FFFF;
//	tri->v2.clr=0xFFFF00FF;
//	tri->v2.clr=0xFF00FFFF;
//	tri->v2.clr=0x9FFFFFFF;
#endif
	
//	tri->tex=NULL;
	tri->tex=swr_tex0;
	
//	BGBDT_SWR_DrawTriangle(swr_ctx, tri);
//	BGBDT_SWR_FreeTriangle(swr_ctx, tri);

	BGBDT_SWR_DrawTriangleList(swr_ctx, tri);
	BGBDT_SWR_FreeTriangleList(swr_ctx, tri);
	
	BGBDT_SWR_GetContextImage(swr_ctx,
		(u32 *)swr_screenbuf, 1024, 512);
	xs=1024;
	ys=512;
	Tex_LoadTexture3B(
		&xs, &ys, (byte *)swr_screenbuf,
		swr_screentex, BTIC1H_PXF_BGRA, 0);

	FRGL_Setup2D();

	frglEnableTexture2D();

	frglBindTexture(GL_TEXTURE_2D, swr_screentex);
	frglColor4f(1,1,1,1);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex2f(-512, -256);
	frglTexCoord2f(1, 0);
	frglVertex2f(512, -256);
	frglTexCoord2f(1, 1);
	frglVertex2f(512, 256);
	frglTexCoord2f(0, 1);
	frglVertex2f(-512, 256);
	frglEnd();

	Con_Render();

	GfxFont_DrawString("X", frgl_state->mx-8, -frgl_state->my-8,
		16, 16, 255, 0, 0, 255);

}

int main_shutdown()
{
	return(0);
}
