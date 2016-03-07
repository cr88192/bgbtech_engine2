#include <bteifgl.h>
#include <bteifgl_main.h>

MSP430_Context *msp_ctx;

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

	kcur=frgl_state->keys;
	while(*kcur)
	{
		switch(*kcur)
		{

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

	MSP430_PinInputVoltage(msp_ctx, MSP430_PIN_P1_5,
		sin(frgl_state->time_f)+1.5);

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

	MSP430_UpdatePeri(msp_ctx);

//	MSP430_RunCount(msp_ctx, 10000000*frgl_state->dt_f);
	t0=(frgl_state->time_f*1000000.0);
	l=44100*frgl_state->dt_f;
//	f=(10000000/44100.0)*frgl_state->dt_f;
//	f=(10000000/44100.0);
	f=(MSP430_GetCurrentMips(msp_ctx)/44100.0);
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
	}

	main_handle_input();

	FRGL_Setup2D();

	frglEnableTexture2D();

	Con_Render();

	sprintf(tb, "P1OUT=%02X, P2OUT=%02X, %.2fMIPS (%.2f MHz)",
		msp_ctx->perib[MSP430_PERIB_P1OUT],
		msp_ctx->perib[MSP430_PERIB_P2OUT],
		(msp_ctx->runCycles/frgl_state->time_f)/1000000.0,
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
