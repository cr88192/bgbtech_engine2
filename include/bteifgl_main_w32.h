#include <windows.h>

#include <bteifgl.h>

#ifndef FRGL_STUB_H
#define FRGL_STUB_H

#ifdef __cplusplus
extern "C" {
#endif

char **frgl_argv;
int frgl_argc;

#ifdef __cplusplus
}
#endif

int frgl_main(int argc, char **argv);

int main_prestart(int argc, char *argv[]);
int main_startup(int argc, char *argv[]);
int main_body();
int main_shutdown();
int main_fastbody();

int main_i()
{
	int i, j, x, y, w, h;
	double f, g, dt_f;
	int ct, dt;

	FRGL_SetupMain();

	i=main_prestart(frgl_argc, frgl_argv);
	if(i<0)return(-1);

	GfxDrv_SetKill(&(frgl_state->kill));
	GfxDrv_Start();

	i=main_startup(frgl_argc, frgl_argv);
	if(i<0)return(-1);

	ct=0;
	dt=FRGL_DeltaMS(&ct);
	if(dt<0)dt=0;

	while(1)
	{
		//framerate limiter
		i=ct;
		dt=FRGL_DeltaMS(&i);
		if(dt<0)dt=0;
			
		if(!(frgl_state->doshot&32) &&
			(frgl_state->maxhz>0) && (dt<(1000/frgl_state->maxhz)))
		{
//			j=1000/(2*frgl_state->maxhz);
			j=1000/(4*frgl_state->maxhz);
			Sleep(j);
			continue;
		}

		ct=i;
		dt_f=dt/1000.0;

		frgl_state->time=ct;
		frgl_state->dt_f=dt_f;
		FRGL_SetupFrame();
		GfxDrv_BeginDrawing();
		i=main_body();
		FRGL_FinishFrame();
		GfxDrv_EndDrawing();

		if((i<0) || frgl_state->kill)break;
	}

	i=main_shutdown();

	FRGL_FinishMain();

	return(i);
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char buf[256], *s, *t;
	int i, j, x, y, w, h;
	int ct, dt;
	char *argv[64];

	frgl_argv=argv;

	frgl_argv[0]="";
	frgl_argv[1]=NULL;
	frgl_argc=0;

	s=GetCommandLineA();
	
	while(*s)
	{
		while(*s && (*s<=' '))s++;

		if(*s=='"')
		{
			s++;
			t=buf;
			while(*s && (*s!='\"'))*t++=*s++;
			if(*s=='\"')s++;
			*t++=0;
			frgl_argv[frgl_argc++]=strdup(buf);
		}else if(*s)
		{
			t=buf;
			while((*s>' '))*t++=*s++;
			*t++=0;
			frgl_argv[frgl_argc++]=strdup(buf);
		}
	}

	GfxDrv_SetInstance(hInstance, nCmdShow);
	i=main_i();
	return(i);
}

#endif
