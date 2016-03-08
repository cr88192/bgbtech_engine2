#include <bteifgl.h>
#include <bteifgl_main.h>

MSP430_Context *msp_ctx;

byte *graph_buf;
int graph_chcnts[16];
byte graph_chlval[16];
int graph_rov;
int graph_cnt;
int graph_tex;
int graph_dirty;
int graph_outpin;

byte ui_wpot[16];
int ui_wpsel;

int main_prestart(int argc, char *argv[])
{
	GfxDrv_SetDefaults("BTE MspTest", 1024, 768);
	FRGL_CvarSetDefault("hide_os_cursor", "1");

	Con_ExecCmdBuf("config.cfg");

	return(0);
}

int main_startup(int argc, char *argv[])
{
	int i, j, k;
	
	Tex_Init2();
	GfxFont_Init();
	Con_Init();
//	ConCmds_Init();

	GPIO_InitRdtsc();
	GPIO_Init();

//	agarai_log=fopen("agarai_log.txt", "at");
//	fprintf(agarai_log, "\n\n\nAgarAI: Start\n\n");

//	for(i=0; i<256; i++)
//	{
//		j=i^(i>>1);
//		argai_grayenc8[i]=j;
//		argai_graydec8[j]=i;
//	}

	msp_ctx=MSP430_NewContext(0, 256, 2048);
	MSP430_LoadIHex(msp_ctx, "VfdPwm.hex");
	MSP430_LoadMap(msp_ctx, "VfdPwm.map");

	graph_buf=malloc(512*1024*4);
	memset(graph_buf, 0, 512*1024*4);
	graph_rov=0;
	graph_cnt=0;

	return(0);
}

int main_handle_input()
{
	char buf[64];
	float mins[3], maxs[3];
	float io[3], iv[4], fw[2], pt[4], f, g, v;
	float im[16], im1[16], *imv;
	int x0, x1, y0, y1, wp;
	int i, j, k;
	unsigned short *kcur;

	if(Con_IsDown())
		return(0);

	wp=-1;
	for(i=0; i<16; i++)
	{
		x0=-512+(i*48);		y0=256-16;
		x1=x0+48;			y1=y0+32;
		
		if(	(frgl_state->mx>x0) &&
			(frgl_state->mx<x1) &&
			((-frgl_state->my)>y0) &&
			((-frgl_state->my)<y1))
		{
			wp=i;
			break;
		}
	}
	ui_wpsel=wp;

	if((frgl_state->mb&1) && !(frgl_state->lmb&1))
	{
		if(wp>=0)
			{ ui_wpot[wp]=255-ui_wpot[wp]; }
	}

	kcur=frgl_state->keys;
	while(*kcur)
	{
		switch(*kcur)
		{
		case K_MWHEELUP:
			if(wp>=0)
			{
				j=ui_wpot[wp];
//				j++;
				j+=7;
				if(j>255)j=255;
				ui_wpot[wp]=j;
			}
			break;
		case K_MWHEELDOWN:
			if(wp>=0)
			{
				j=ui_wpot[wp];
//				j--;
				j-=7;
				if(j<0)j=0;
				ui_wpot[wp]=j;
			}
			break;
		default:
			break;
		}
		kcur++;
	}

	return(0);
}

int main_updateGraphSetPinValue(int pin, int val)
{
	graph_chcnts[pin]=val>>1;
}

int main_updateGraphBits(int bits)
{
	int *ct;
	int m, n, c, c1, c2, cw, cg;
	int i, j, k, l;

	graph_cnt++;
	if(graph_cnt<128)
	{
		for(i=0; i<16; i++)
		{
			if(!(graph_outpin&(1<<i)))
				continue;
			if(bits&(1<<i))
				graph_chcnts[i]++;
		}
		return;
	}
	
	ct=(int *)(graph_buf+(graph_rov*512*4));
	c=0xFF000000;
	for(i=0; i<512; i++)
		ct[i]=c;
	
	cw=0xFFFFFFFF;
	cg=0xFF00FF00;
	c1=0xFF404040;
	c2=0xFF004000;

	if(graph_outpin&0xFF00)
	{
		for(i=0; i<16; i++)
		{
			ct[i*32+16]=c1;
			ct[i*32+0]=c2;
			ct[i*32+31]=c2;

			c=cw;
			if(!(graph_outpin&(1<<i)))
				c=cg;

//			if(!(graph_outpin&(1<<i)))
//				continue;

			j=graph_chcnts[i]>>(7-5);
			k=graph_chlval[i];
			
			m=j; n=j;
			if(k<m)m=k;
			if(k>n)n=k;
			for(k=m; k<=n; k++)
				ct[i*32+k]=c;
			graph_chlval[i]=j;
			
	//		ct[i*32+j]=c;
			if(graph_outpin&(1<<i))
				graph_chcnts[i]=0;
		}
	}else
	{
		for(i=0; i<8; i++)
		{
			ct[i*64+32]=c1;
			ct[i*64+0]=c2;
			ct[i*64+63]=c2;
		
			c=cw;
			if(!(graph_outpin&(1<<i)))
				c=cg;

//			if(!(graph_outpin&(1<<i)))
//				continue;

			j=graph_chcnts[i]>>(7-6);
			k=graph_chlval[i];
			
			m=j; n=j;
			if(k<m)m=k;
			if(k>n)n=k;
			for(k=m; k<=n; k++)
				ct[i*64+k]=c;
			graph_chlval[i]=j;
			
	//		ct[i*32+j]=c;
			if(graph_outpin&(1<<i))
				graph_chcnts[i]=0;
		}
	}

	graph_rov=(graph_rov+1)&1023;
	graph_cnt=0;
	graph_dirty=1;

	ct=(int *)(graph_buf+(graph_rov*512*4));
	c=0xFFFF0000;
	for(i=0; i<512; i++)
		ct[i]=c;
	
//	graph_tex=Tex_LoadTexture2(512, 512, graph_buf, 0, graph_tex, 0);
}

int main_body()
{
	char tb[1024];

	char *s;
	u32 imp0, imp1;
	u32 act, mask, pin, pout;
	float cr, cg, cb;
	double x0, x1, y0, y1, t0, t1;
	double f, g;
	int mx, my, mgx, mgy;
	int tex, txs, tys;
	int i, j, k, l;

	mx=frgl_state->mx;
	my=-frgl_state->my;

//	AgarAI_Tick(frgl_state->dt_f);
//	MSP430_RunCount(msp_ctx, 1000);
//	MSP430_RunCount(msp_ctx, 10000000);
//	MSP430_RunCount(msp_ctx, 100000);
//	MSP430_RunCount(msp_ctx, 1000000);

	GPIO_DummySetTimeFakeUS(frgl_state->time_f*1000000);

	j=msp_ctx->perib[MSP430_PERIB_P1DIR];
	k=msp_ctx->perib[MSP430_PERIB_P2DIR];
	for(i=0; i<8; i++)
	{
		if(j&(1<<i))
			GPIO_SetPinOutput(i);
		if(k&(1<<i))
			GPIO_SetPinOutput(8+i);
	}

	graph_outpin=j|(k<<8);

//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_5,
//		sin(frgl_state->time_f)+1.5);

//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_5,
//		0.3);

//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_4,
//		sin(frgl_state->time_f)+1.5);
//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_4, 0.2);
//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_4, 2.0);
//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_4, 2.2);

//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_5, 2.45);

//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_6,
//		sin(frgl_state->time_f)+1.5);
//	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_7, 2.0);

	for(i=0; i<16; i++)
	{
		if(graph_outpin&(1<<i))
			continue;

		MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_0+i,
			ui_wpot[i]*(3.3/256));
	
		main_updateGraphSetPinValue(i, ui_wpot[i]);
//		graph_chcnts[i]=ui_wpot[i];
	}

	MSP430_UpdatePeri(msp_ctx);

//	MSP430_RunCount(msp_ctx, 10000000*frgl_state->dt_f);
	t0=(frgl_state->time_f*1000000.0);
	l=44100*frgl_state->dt_f;
//	f=(10000000/44100.0)*frgl_state->dt_f;
//	f=(10000000/44100.0);
	f=(MSP430_GetCurrentMips(msp_ctx)/44100.0);
//	f=(MSP430_GetCurrentMHz(msp_ctx)/44100.0);
	g=0;
	for(i=0; i<l; i++)
	{
		GPIO_DummySetTimeFakeUS(
			t0+i*(1000000.0/44100.0));

		g+=f; j=(int)g; g=g-j;
		MSP430_RunCount(msp_ctx, j);
//		GPIO_PinSetOutMask8A(msp_ctx->perib[MSP430_PERIB_P1OUT]);
//		GPIO_PinSetOutMask8B(msp_ctx->perib[MSP430_PERIB_P2OUT]);

		k=MSP430_GetOutputPinRunning(msp_ctx);
		GPIO_PinSetOutMask8A(k&255);
		GPIO_PinSetOutMask8B((k>>8)&255);
		main_updateGraphBits(k);
	}

	if(graph_dirty && !(frgl_state->mb&1))
	{
		graph_tex=Tex_LoadTexture2(512, 1024, graph_buf, 0, graph_tex, 0);
		graph_dirty=0;
	}

	main_handle_input();

	FRGL_Setup2D();

	x0=-512; x1=x0+1024;
	y0=-256; y1=y0+512;

	frglEnableTexture2D();
	frglBindTexture2D(graph_tex);
	frglBegin(GL_QUADS);
	frglTexCoord2f(0, 0);
	frglVertex2f(x0, y0);
//	frglTexCoord2f(1, 0);
	frglTexCoord2f(0, 1);
	frglVertex2f(x1, y0);
	frglTexCoord2f(1, 1);
	frglVertex2f(x1, y1);
//	frglTexCoord2f(0, 1);
	frglTexCoord2f(1, 0);
	frglVertex2f(x0, y1);
	frglEnd();

	for(i=0; i<16; i++)
	{
		cr=128;
		cg=128;
		cb=128;

		if(i==ui_wpsel)
		{
			cr=0;
			cg=255;
			cb=0;
		}
	
		sprintf(tb, "%01X:%02X", i, ui_wpot[i]);
		GfxFont_DrawString(tb, -512+i*48, 256, 8, 8, cr, cg, cb, 255);
	}

	frglEnableTexture2D();

	Con_Render();

	sprintf(tb, "P1OUT=%02X, P2OUT=%02X, %.2fMIPS (%.2f MHz)",
		msp_ctx->perib[MSP430_PERIB_P1OUT],
		msp_ctx->perib[MSP430_PERIB_P2OUT],
//		(msp_ctx->runCycles/frgl_state->time_f)/1000000.0,
		(msp_ctx->runOps/frgl_state->time_f)/1000000.0,
		MSP430_GetCurrentMHz(msp_ctx));

	GfxFont_DrawString(tb, -320, 384-16, 16, 16, 0, 255, 0, 255);

//	AgarAI_Draw();

//	sprintf(tb, "Gen=%d GenClock=%.2fs",
//		agarai_generation, agarai_genclock);
//	GfxFont_DrawString(tb, -320, 384-16, 16, 16, 0, 255, 0, 255);

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
	GPIO_DeInit();
	return(0);
}
