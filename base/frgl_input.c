#include <bteifgl.h>

#include <stdarg.h>
#include <time.h>

#if defined(linux) || defined(__EMSCRIPTEN__)
#include <sys/time.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct keyhandler_s keyhandler;
struct keyhandler_s {
keyhandler *next;
int (*func)(int key, int down);
};

static byte frgl_keymap[32];
static byte frgl_lkeymap[32];
static unsigned short frgl_keybuf2[64];
static int frgl_keybuf2_pos;

keyhandler *frgl_keyhandlers=NULL;

static int keyb_num_shift[10]={
')', '!', '@', '#', '$', '%', '^', '&', '*', '('
};

void (*frgl_shutdown[256])();
int frgl_n_shutdown=0;


FRGL_State *frgl_state_i=NULL;
FILE *frgl_log=NULL;

BTEIFGL_API int frgl_puts(char *str)
{
	Con_WriteString(str);
	if(frgl_log)
	{
		fputs(str, frgl_log);
		fflush(frgl_log);
	}
	fputs(str, stdout);

	return(0);
}

BTEIFGL_API int frgl_printf(char *str, ...)
{
	char tb[4096];
	va_list lst;
	
	if(!frgl_log)
		{ frgl_log=fopen("frgl_log.txt", "wt"); }
	
	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);

	Con_WriteString(tb);
	if(frgl_log)
	{
		fputs(tb, frgl_log);
		fflush(frgl_log);
	}
	fputs(tb, stdout);

	return(0);
}

BTEIFGL_API void *frgl_malloc(int sz)
{
	void *ptr;
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

BTEIFGL_API void frgl_free(void *ptr)
{
	free(ptr);
}

BTEIFGL_API void *frgl_talloc(char *ty, int sz)
{
	return(frgl_malloc(sz));
}

BTEIFGL_API void *frgl_tatomic(char *ty, int sz)
{
	return(frgl_malloc(sz));
}

BTEIFGL_API void *frgl_realloc(void *ptr, int sz)
{
	return(realloc(ptr, sz));
}

#if 0
VFILE *frgl_fopen(char *name, char *mode)
{
	char tb[512];
	FILE *fd;
	
	sprintf(tb, "resource/%s", name);
	fd=fopen(tb, mode);
	if(fd)
		return(fd);

	fd=fopen(name, mode);
	if(fd)
		return(fd);
	
	return(NULL);
}
#endif

// #define vfclose		fclose
// #define vfread		fread
// #define vfwrite		fwrite
// #define vfgetc		fgetc
// #define vfputc		fputc
// #define vftell		ftell
// #define vfseek		fseek

#if 0
void thLockFastMutex(void *ptr)
{
}

void thUnlockFastMutex(void *ptr)
{
}

void *thFastMutex()
{
	return(NULL);
}
#endif

BTEIFGL_API void *vf_bufferin_sz(VFILE *fd, int *rsz)
{
	void *buf;
	int sz;

	vfseek(fd, 0, 2);
	sz=vftell(fd);
	vfseek(fd, 0, 0);
	buf=frgl_malloc(sz+4);
	memset(buf, 0, sz+4);
	vfread(buf, 1, sz, fd);
	if(rsz)*rsz=sz;
	return(buf);
}

BTEIFGL_API void *vf_bufferin(VFILE *fd)
{
	return(vf_bufferin_sz(fd, NULL));
}

#if 0
BTEIFGL_API void *vf_loadfile(char *name, int *rsz)
{
	VFILE *fd;
	byte *buf;
	
	fd=vffopen(name, "rb");
	if(!fd)return(NULL);
	buf=vf_bufferin_sz(fd, rsz);
	vfclose(fd);
	return(buf);
}

BTEIFGL_API void vf_storefile(char *name, void *buf, int sz)
{
	VFILE *fd;
	
	fd=vffopen(name, "wb");
	if(!fd)return;
	vfwrite(buf, 1, sz, fd);
//	buf=vf_bufferin_sz(fd, rsz);
	vfclose(fd);
}
#endif

#if 1
BTEIFGL_API void *vf_loadfile(char *name, int *rsz)
{
	return(VfLoadFile2(name, rsz));
}

BTEIFGL_API void vf_storefile(char *name, void *buf, int sz)
{
	VfStoreFile(name, buf, sz);
}
#endif

BTEIFGL_API void vf_freefdbuf(void *buf)
{
	frgl_free(buf);
}

int FRGL_ParseChar(char **rs)
{
	unsigned char *s;
	int i, j;
	s=(unsigned char *)(*rs);
	i=*s++;
	if(i&0x80)
	{
		if((i>=0xC0) && (i<=0xDF))
		{
			j=((i&0x1F)<<6)|((*s++)&0x3F);
		}
		if((i>=0xE0) && (i<=0xEF))
		{
			j=((i&0x0F)<<12);
			j|=((*s++)&0x3F)<<6;
			j|=((*s++)&0x3F);
		}
		i=j;
	}
	*rs=(char *)s;
	return(i);
}

int FRGL_PeekChar(char **rs)
{
	char *s;
	s=*rs;
	return(FRGL_ParseChar(&s));
}

int FRGL_EmitChar(char **rs, int v)
{
	unsigned char *s;
	int i, j;
	s=(unsigned char *)(*rs);

	if(v<128)
	{
		*s++=v;
	}else if(v<8192)
	{
		*s++=0xC0|(v>>6);
		*s++=0x80|(v&0x3F);
	}else if(v<8192)
	{
		*s++=0xE0|(v>>12);
		*s++=0x80|((v>>6)&0x3F);
		*s++=0x80|(v&0x3F);
	}

	*rs=(char *)s;
	return(0);
}

BTEIFGL_API void *FRGL_GetState(void)
{
	if(!frgl_state_i)
	{
		frgl_state_i=frgl_malloc(sizeof(FRGL_State));
		frgl_state_i->maxhz=90;
		frgl_state_i->maxfasthz=1000;
//		frgl_state_i->maxhz=1000;
	}
	return(frgl_state_i);
}

BTEIFGL_API void FRGL_SetupFrame(void)
{
//	printf("dt %d\n", dt);

//	frgl_time=ct;
//	frgl_dt=dt;
	frgl_state_i->lxs=frgl_state_i->xs;
	frgl_state_i->lys=frgl_state_i->ys;
	GfxDrv_GetWindowSize(&frgl_state_i->xs, &frgl_state_i->ys);
	frgl_state_i->keys=FRGL_GetKeybuf();
//	PDGLUI_SetKeybuf(frgl_keybuf, frgl_dt/1000.0);

//	printf("kbuf %p\n", frgl_keybuf);

	frgl_state_i->lmx=frgl_state_i->mx;
	frgl_state_i->lmy=frgl_state_i->my;
	frgl_state_i->lmb=frgl_state_i->mb;

	GfxDrv_MouseGetPos(&frgl_state_i->mx, &frgl_state_i->my,
		&frgl_state_i->mb);

	frgl_state_i->dmx=frgl_state_i->mx-frgl_state_i->lmx;
	frgl_state_i->dmy=frgl_state_i->my-frgl_state_i->lmy;

	if(!(frgl_state_i->lmb&1))
	{
		frgl_state_i->omx=frgl_state_i->mx;
		frgl_state_i->omy=frgl_state_i->my;
	}

//	frgl_state_i->dt_f=frgl_dt/1000.0;
//	frgl_state_i->dt_f=dt_f;
	frgl_state_i->adt_f=(frgl_state_i->adt_f*0.90)+(frgl_state_i->dt_f*0.10);
	frgl_state_i->adt2_f=(frgl_state_i->adt2_f*0.95)+(frgl_state_i->adt_f*0.05);
	frgl_state_i->time_f=frgl_state_i->time/1000.0;

#if 0
	if(frgl_state_i->doshot&32)
	{
		frgl_acdt_f+=dt_f;
		f=FRGL_AVI_GetFrameTime();
		dt_f=0;
//		while(frgl_acdt_f>=f)
//			{ frgl_acdt_f-=f; dt_f+=f; }
		if(frgl_acdt_f>=f)
			{ frgl_acdt_f-=f; dt_f+=f; }
		if(dt_f<f)
			continue;

		frgl_time+=dt_f*1000+0.5;
		frgl_dt=dt_f*1000+0.5;
	}else
	{
		frgl_acdt_f=0;
		frgl_time+=dt;
		frgl_dt=dt;
	}
#endif
}

BTEIFGL_API void FRGL_SetupMain(void)
{
#if 0
#ifdef FRGL_BASEHEAP
	gcBaseHeap(FRGL_BASEHEAP);
#endif

#ifdef FRGL_BASEHEAP_LIMIT
	gcBaseHeapLimit(FRGL_BASEHEAP_LIMIT);
#endif

#ifdef FRGL_BASEHEAP_LOBJ
	gcBaseHeapLObj(FRGL_BASEHEAP_LOBJ);
#endif

#ifdef FRGL_BASEHEAP_CONS
	gcBaseHeapCons(FRGL_BASEHEAP_CONS);
#endif
#endif
}

BTEIFGL_API void FRGL_StartMain(void)
{
}

BTEIFGL_API void FRGL_FinishMain(void)
{
//	FRGL_AVI_NoCapture(frgl_frgl_state_i->doshot);

	FRGL_Shutdown();
	GfxDrv_Shutdown();
//	vf_umount(NULL);
}

BTEIFGL_API void FRGL_FinishFrame(void)
{
#if 0
//	if(frgl_state_i->doshot==1)
	if((frgl_state_i->doshot&15)==1)
	{
		Tex_DoScreenshot();
		frgl_state_i->doshot=0;
	}

	if(frgl_state_i->doshot&16)
	{
		Tex_DoScreenshot();
		frgl_state_i->doshot&=~16;
	}

//	if(frgl_state_i->doshot==2)
	if((frgl_state_i->doshot&15)==2)
	{
//		Tex_DoScreenshotFrame();
//		FRGL_AVI_DoScreenshotFrame(frgl_state_i->dt_f);
		FRGL_AVI_DoScreenshotFrame(dt_f, frgl_state_i->doshot);
	}else
	{
		FRGL_AVI_NoCapture(frgl_state_i->doshot);
		frgl_state_i->doshot=0;
	}
#endif

//	UI_Camera_SetDoShot(frgl_state_i->doshot);
}

BTEIFGL_API int FRGL_AddKeyHandler(int (*fcn)(int key, int down))
{
	keyhandler *tmp;

	tmp=frgl_malloc(sizeof(keyhandler));
	tmp->func=fcn;

	tmp->next=frgl_keyhandlers;
	frgl_keyhandlers=tmp;

	return(0);
}

BTEIFGL_API int FRGL_KeyDown(int key)
{
	if(frgl_keymap[key>>3]&(1<<(key&7)))return(1);
	return(0);
}

BTEIFGL_API int FRGL_LastKeyDown(int key)
{
	if(frgl_lkeymap[key>>3]&(1<<(key&7)))return(1);
	return(0);
}

BTEIFGL_API int FRGL_GetKeyMap(byte *map)
{
	memcpy(map, frgl_keymap, 32);
	return(0);
}

BTEIFGL_API int GfxDev_Key_Event(int key, int down)
{
	static int skm=0, skmd=0;
	int akey, akey2, nskm;
	keyhandler *cur;

	akey=key;
	if(FRGL_KeyDown(K_SHIFT))
	{
		if(key>='a' && key<='z')
			akey=key-'a'+'A';
		if(key>='0' && key<='9')
			akey=keyb_num_shift[key-'0'];

		if(key=='\'')akey='"';
		if(key=='[')akey='{';
		if(key==']')akey='}';
		if(key=='-')akey='_';
		if(key=='=')akey='+';
		if(key==';')akey=':';
		if(key==',')akey='<';
		if(key=='.')akey='>';
		if(key=='/')akey='?';
		if(key=='`')akey='~';
		if(key=='\\')akey='|';
	}

	nskm=0;
	if(FRGL_KeyDown(K_CTRL))
	{
		switch(akey)
		{
		case K_F1: nskm=K_SKM_F1; break;
		case K_F2: nskm=K_SKM_F2; break;
		case K_F3: nskm=K_SKM_F3; break;
		case K_F4: nskm=K_SKM_F4; break;
		case K_F5: nskm=K_SKM_F5; break;
		case K_F6: nskm=K_SKM_F6; break;
		case K_F7: nskm=K_SKM_F7; break;
		case K_F8: nskm=K_SKM_F8; break;
		case K_F9: nskm=K_SKM_F9; break;
		case K_F10: nskm=K_SKM_F10; break;
		case K_F11: nskm=K_SKM_F11; break;
		case K_F12: nskm=K_SKM_F12; break;
		default: break;
		}
	}else if(!skm)
	{
		if(!FRGL_KeyDown(K_CTRL) && !FRGL_KeyDown(K_ALT) &&
			!FRGL_KeyDown(K_SHIFT))
		{
			if(akey==K_ESC)
				nskm=K_SKM_ESC;
		}
	}

	if(down)
	{
		akey2=akey;
		if(skm && (akey>=' ') && (akey<='~'))
		{
			akey2=akey|skm;
			skmd=skm;
//			skm=0;
		}
	
		frgl_keymap[akey>>3]|=1<<(akey&7);
		if(frgl_keybuf2_pos<63)
		{
			frgl_keybuf2[frgl_keybuf2_pos++]=akey2;
			frgl_keybuf2[frgl_keybuf2_pos]=0;
		}

		cur=frgl_keyhandlers;
		while(cur)
		{
			cur->func(akey2, 1);
			cur=cur->next;
		}
	}else
	{
		akey2=akey;
		if(skm && skmd && (akey>=' ') && (akey<='~'))
		{
			akey2=akey|skm;
			skm=0; skmd=0;
		}
	
		frgl_keymap[akey>>3]&=~(1<<(akey&7));
		if(frgl_keybuf2_pos<63)
		{
			frgl_keybuf2[frgl_keybuf2_pos++]=akey2|0x8000;
			frgl_keybuf2[frgl_keybuf2_pos]=0;
		}

		cur=frgl_keyhandlers;
		while(cur)
		{
			cur->func(akey2, 0);
			cur=cur->next;
		}
	}
	
	if(nskm)
		{ skm=nskm; }
	return(0);
}

void FRGL_EndInputFrame()
{
	memcpy(frgl_lkeymap, frgl_keymap, 32);
}

BTEIFGL_API u16 *FRGL_GetKeybuf()
{
	static u16 frgl_keybuf3[64];
	u16 *buf;

//	memset(frgl_lkeymap, frgl_keymap, 32);

//	buf=gcralloc(128);
//	memcpy(buf, frgl_keybuf2, 128);

	buf=frgl_keybuf3;
	memcpy(buf, frgl_keybuf2, 128);

	frgl_keybuf2_pos=0;
	frgl_keybuf2[0]=0;

//	printf("buf %p\n", buf);

	return(buf);
}


BTEIFGL_API int FRGL_TimeMS()
{
#ifdef _WIN32
	static unsigned int init;
	unsigned int t;

	t=timeGetTime();
	if(!init)init=t;

	return((unsigned int)(t-init));
#else

#ifdef __EMSCRIPTEN__
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif

#ifndef linux
	static int init;
	int t;

	t=clock();
	t*=CLOCKS_PER_SEC/1000.0;

	if(!init)init=t;

	return((unsigned int)(t-init));
#endif
#ifdef linux
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif
#endif
}

BTEIFGL_API int FRGL_DeltaMS(int *l)
{
	int t, dt;

	t=FRGL_TimeMS();
	dt=t-(*l);
	*l=t;
	return(dt);
}

BTEIFGL_API int frgl_clock(void)
{
	return(FRGL_TimeMS());
}

BTEIFGL_API void FRGL_RegisterShutdown(void (*fcn)())
{
	int i;
	
	i=frgl_n_shutdown++;
	frgl_shutdown[i]=fcn;
}

BTEIFGL_API void FRGL_Shutdown()
{
	int i;

	for(i=0; i<frgl_n_shutdown; i++)
		frgl_shutdown[i]();
}
