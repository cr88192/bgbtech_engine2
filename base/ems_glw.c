#include <bteifgl.h>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <SDL/SDL.h>

#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>

// #include <SDL/SDL_opengl.h>

SDL_Surface *frgl_main_screen;


volatile int gfxdrv_dt_swap=0;

int gfxdrv_dtt_swap;

int window_center_x;
int window_center_y;
int window_mouse_buttons;
int window_width;
int window_height;
int window_active;
int window_lastactive;
int window_fullscreen;

int window_def_width=1024;
int window_def_height=640;

//int window_def_width=1024;
//int window_def_height=768;
char *window_def_label="BTEIFGL";
int window_def_fullscreen=0;

char *gl_vendor, *gl_renderer, *gl_version, *gl_extensions;

int gfxdrv_vmx, gfxdrv_vmy;

int gfxdrv_pmx, gfxdrv_pmy;
int gfxdrv_lpmx, gfxdrv_lpmy;


BTEIFGL_API int GfxDrv_MouseGetRelPos(int *dx, int *dy, int *mb)
{
	return(0);
}

BTEIFGL_API int GfxDrv_MouseGetPos(int *mx, int *my, int *mb)
{
	int b;

//	*mx=0;
//	*mx=0;
//	b=SDL_GetMouseState(mx, my);
//	*mb=b;

	b=SDL_GetMouseState(&gfxdrv_pmx, &gfxdrv_pmy);
	gfxdrv_vmx+=gfxdrv_pmx-gfxdrv_lpmx;
	gfxdrv_vmy+=gfxdrv_pmy-gfxdrv_lpmy;
	gfxdrv_lpmx=gfxdrv_pmx;
	gfxdrv_lpmy=gfxdrv_pmy;

	*mx=gfxdrv_vmx;
	*my=gfxdrv_vmy;
	*mb=b;

	return(0);
}

BTEIFGL_API int GfxDrv_MouseSetPos(int mx, int my)
{
//	SDL_WarpMouseGlobal(mx, my);
	gfxdrv_vmx=mx;
	gfxdrv_vmy=my;
	return(0);
}



static int gfxdrv_log2up(int v)
{
	int i, j;
	
	i=v; j=0;
	while(i>1) { i=(i+1)>>1; j++; }
	return(j);
}

byte gfxdrv_lastkeys[256];

static int scantokey[256]=
{
  0,    0,    0,    0,  'a',  'b',  'c',  'd', //  0-  7
'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l', //  8- 15
'm',  'n',  'o',  'p',  'q',  'r',  's',  't', // 16- 23
'u',  'v',  'w',  'x',  'y',  'z',  '1',  '2', // 24- 31
'3',  '4',  '5',  '6',  '7',  '8',  '9',  '0', // 32- 39
 13,   27,    8,    9,  ' ',  '-',  '=',  '[', // 40- 47
']', '\\', '\\',  ';',  '`', '\'',  ',',  '.', // 48- 55
'/',  160,  135,  136,  137,  138,  139,  140, // 56- 63
141,  142,  143,  144,  145,  146,    0,  161, // 64- 71
153,  147,  151,  150,  127,  152,  149,  131, // 72- 79
130,  129,  128,    0,  207,  206,  205,  204, // 80- 87
218,  209,  210,  211,  212,  213,  214,  215, // 88- 95
216,  217,  208,  219,  '`',    0,    0,  '=', // 96-103
  0,    0,    0,    0,    0,    0,    0,    0, //104-111
  0,    0,    0,    0,    0,    0,    0,    0, //112-119
  0,    0,    0,    0,    0,    0,    0,    0, //120-127
  0,    0,    0,    0,    0,    0,    0,    0, //128-135
  0,    0,    0,    0,    0,    0,    0,    0, //136-143
  0,    0,    0,    0,    0,    0,    0,    0, //144-151
  0,    0,    0,    0,    0,    0,    0,    0, //152-159
  0,    0,    0,    0,    0,    0,    0,    0, //160-167
  0,    0,    0,    0,    0,    0,    0,    0, //168-175
  0,    0,    0,    0,    0,    0,    0,    0, //176-183
  0,    0,  ';',  '=',    0,  '-',    0,    0, //184-191
  0,    0,    0,    0,    0,    0,    0,    0, //192-199
  0,    0,    0,    0,    0,    0,    0,    0, //200-207
  0,    0,    0,    0,    0,    0,    0,    0, //208-215
  0,    0,    0,    0,    0,    0,    0,    0, //216-223
133,  134,  132,    0,  133,  134,  132,    0, //224-231
};

int GfxDrv_MapKey(int key)
{
	int al, val;

//	frgl_printf("Key: 0x%08X\n", key);

	if(key>255)
		return(0);
	val=scantokey[key];
	return(val);
}

void GfxDrv_UpdateKeyboard()
{
	byte *keys;
	int i;

#if 0
	keys=SDL_GetKeyboardState(NULL);
	for(i=0; i<256; i++)
	{
		if(keys[i] && !gfxdrv_lastkeys[i])
		{
//			printf("keyup %d\n", i);
			GfxDev_Key_Event(i, 1);
			gfxdrv_lastkeys[i]=1;
		}else if(!keys[i] && gfxdrv_lastkeys[i])
		{
//			printf("keydown %d\n", i);
			GfxDev_Key_Event(i, 0);
			gfxdrv_lastkeys[i]=0;
		}
	}
#endif
}

void GfxDrv_UpdateEvents()
{
	SDL_Event ev;
	int i, j;

	while(SDL_PollEvent(&ev))
	{
		switch(ev.type)
		{
		case SDL_KEYDOWN:
			i=ev.key.keysym.scancode;
			j=GfxDrv_MapKey(i);
//			printf("EV Key Dn %02X %02X\n", i, j);
			GfxDev_Key_Event(j, 1);
//			GfxDev_Key_Event(ev.key.keysym.scancode, 1);
			break;
		case SDL_KEYUP:
//			GfxDev_Key_Event(ev.key.keysym.scancode, 0);
			i=ev.key.keysym.scancode;
			j=GfxDrv_MapKey(i);
//			printf("EV Key Up %02X %02X\n", i, j);
			GfxDev_Key_Event(j, 0);
			break;
		default:
			break;
		}
	}
}

BTEIFGL_API void GfxDrv_GetWindowSize(int *xs, int *ys)
	{ *xs=window_width; *ys=window_height; }

BTEIFGL_API void GfxDrv_GetWindowTexSize(int *xs, int *ys)
{
	*xs=1<<gfxdrv_log2up(window_width);
	*ys=1<<gfxdrv_log2up(window_height);
}

BTEIFGL_API bool GfxDrv_WindowIsActiveP(void)
	{ return(window_active); }

BTEIFGL_API void GfxDrv_BeginDrawing()
{
	int t0, t1;
	gfxdrv_dtt_swap=0;
	
	t0=frgl_clock();
//	GfxDrv_V_HandleMessages();

	SDL_PumpEvents();
	GfxDrv_UpdateKeyboard();
	GfxDrv_UpdateEvents();

	glViewport(0, 0, window_width, window_height);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	t1=frgl_clock();
	gfxdrv_dtt_swap+=t1-t0;
}

BTEIFGL_API void GfxDrv_EndDrawing(void)
{
	int t0, t1;

	t0=frgl_clock();

	glFinish();
//	SwapBuffers(maindc);
//	SDL_Flip(frgl_main_screen);
    SDL_GL_SwapBuffers();
	
	FRGL_EndInputFrame();

	t1=frgl_clock();
	gfxdrv_dtt_swap+=t1-t0;	
	gfxdrv_dt_swap=gfxdrv_dtt_swap;
}


void GfxDrv_InitGL()
{
	gl_vendor=(char *)glGetString(GL_VENDOR);
	gl_renderer=(char *)glGetString(GL_RENDERER);
	gl_version=(char *)glGetString(GL_VERSION);
	gl_extensions=(char *)glGetString(GL_EXTENSIONS);

	printf("OpenGL Vendor=%s\n", gl_vendor);
	printf("OpenGL Renderer=%s\n", gl_renderer);
	printf("OpenGL Version=%s\n", gl_version);
//	printf("OpenGL Extensions=%s\n", gl_extensions);

//	BGBBTJ_SetGlExtensions(gl_extensions);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);

	glDepthRange(0.0, 1.0);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0, 0, 0, 1);
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.75);
	glDisable(GL_ALPHA_TEST);

	glDisable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

BTEIFGL_API int GfxDrv_Start()
{
	window_width=1024;
//	window_height=768;
	window_height=640;
	window_active=1;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	frgl_main_screen = SDL_SetVideoMode(
		window_width, window_height, 32,
		SDL_OPENGL);
  
	GfxDrv_InitGL();
  
	return(0);
}

BTEIFGL_API void GfxDrv_SetDefaults(char *label, int width, int height)
{
	if(label)window_def_label=label;
	window_def_width=width;
	window_def_height=height;
}

BTEIFGL_API void GfxDrv_SetDefaultFullscreen(int fs)
{
	window_def_fullscreen=fs;
}

BTEIFGL_API int GfxDrv_Init(void *hInstance, int nCmdShow, int *rkill)
{
	return(0);
}

BTEIFGL_API int GfxDrv_SetKill(int *rkill)
{
//	gfxdrv_rkill=rkill;
	return(0);
}

BTEIFGL_API int GfxDrv_GetDtSwap()
{
	return(gfxdrv_dt_swap);
}

#endif
