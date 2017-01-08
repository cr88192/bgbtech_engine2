/*
 * VFS Wrapper. Allows client to plug in its own filesystem interface.
 */
BGBDT_MM_IOFuncs *bgbdt_mm_io;

void *bgbdt_mm_fopen_tmp(char *name, char *mode)
{
	char tb[1024];
	void *fd;
	
	fd=fopen(name, mode);
	if(fd)
		return(fd);
	
	sprintf(tb, "resource/%s", name);
	fd=fopen(tb, mode);
	if(fd)
		return(fd);
	
	return(NULL);
}

s64 bgbdt_mm_tell_tmp(void *fd)
	{ return(ftell((FILE *)fd)); }
int bgbdt_mm_seek_tmp(void *fd, s64 pos, int rel)
	{ return(fseek((FILE *)fd, pos, rel)); }

BGBDT_MM_IOFuncs *bgbdt_mm_getio()
{
	if(!bgbdt_mm_io)
	{
		bgbdt_mm_io=malloc(sizeof(BGBDT_MM_IOFuncs));
		memset(bgbdt_mm_io, 0, sizeof(BGBDT_MM_IOFuncs));
	}

	return(bgbdt_mm_io);
}

BS2VM_API BGBDT_MM_IOFuncs *BGBDT_MM_GetIOFuncs()
{
	return(bgbdt_mm_getio());
}

void bgbdt_mm_initvfs()
{
	static int init=0;

	if(init)return;
	init=1;

	if(!bgbdt_mm_io)
		bgbdt_mm_io=bgbdt_mm_getio();

	if(!bgbdt_mm_io->fopen_fp)
	{
//		bgbdt_mm_io->fopen_fp=(void *(*)(char*,char*))&fopen;
		bgbdt_mm_io->fopen_fp=&bgbdt_mm_fopen_tmp;
		bgbdt_mm_io->fclose_fp=(void (*)(void*))&fclose;
		bgbdt_mm_io->fread_fp=(int (*)(void*,int,int,void*))&fread;
		bgbdt_mm_io->fwrite_fp=(int (*)(void*,int,int,void*))&fwrite;
		bgbdt_mm_io->feof_fp=(int (*)(void*))&feof;
//		bgbdt_mm_io->ftell_fp=(s64 (*)(void*))&ftell;
//		bgbdt_mm_io->fseek_fp=(int (*)(void*,s64,int))&fseek;

		bgbdt_mm_io->ftell_fp=&bgbdt_mm_tell_tmp;
		bgbdt_mm_io->fseek_fp=&bgbdt_mm_seek_tmp;
	}
}

BS2VM_API void *bgbdt_mm_fopen(char *name, char *mode)
{
	void *fd;

	bgbdt_mm_initvfs();
	fd=bgbdt_mm_io->fopen_fp(name, mode);
	return(fd);
}

BS2VM_API void bgbdt_mm_fclose(void *fd)
{
	bgbdt_mm_initvfs();
	bgbdt_mm_io->fclose_fp((FILE *)fd);
}

BS2VM_API int bgbdt_mm_fread(void *buf, int m, int n, void *fd)
{
	int i;
	bgbdt_mm_initvfs();
	i=bgbdt_mm_io->fread_fp(buf, m, n, fd);
	return(i);
}

BS2VM_API int bgbdt_mm_fwrite(void *buf, int m, int n, void *fd)
{
	int i;
	bgbdt_mm_initvfs();
	i=bgbdt_mm_io->fwrite_fp(buf, m, n, fd);
	return(i);
}

BS2VM_API int bgbdt_mm_feof(void *fd)
{
	bgbdt_mm_initvfs();
	if(!bgbdt_mm_io->feof_fp)
		return(0);
	return(bgbdt_mm_io->feof_fp(fd));
}

BS2VM_API s64 bgbdt_mm_ftell(void *fd)
{
	bgbdt_mm_initvfs();
	return(bgbdt_mm_io->ftell_fp(fd));
}

BS2VM_API int bgbdt_mm_fseek(void *fd, s64 pos, int rel)
{
	bgbdt_mm_initvfs();
	bgbdt_mm_io->fseek_fp(fd, pos, rel);
	return(bgbdt_mm_io->ftell_fp(fd));
}

BS2VM_API int bgbdt_mm_fgetc(void *fd)
{
	unsigned char ch[4];
	int i;

	i=bgbdt_mm_fread(ch, 1, 1, fd);
	if(i<1)return(-1);
	return(ch[0]);
}

BS2VM_API void bgbdt_mm_fputc(int c, void *fd)
{
	unsigned char ch[4];
	ch[0]=c;
	bgbdt_mm_fwrite(ch, 1, 1, fd);
}


BS2VM_API void *bgbdt_mm_loadfile(char *name, int *rsz)
{
	void *buf;
	void *fd;
	int sz;

	bgbdt_mm_initvfs();

	fd=bgbdt_mm_fopen(name, "rb");
	if(!fd)return(NULL);

	bgbdt_mm_fseek(fd, 0, 2);
	sz=bgbdt_mm_ftell(fd);
	bgbdt_mm_fseek(fd, 0, 0);

	buf=malloc(sz+16);
	memset(buf, 0, sz+16);
	bgbdt_mm_fread(buf, 1, sz, fd);

#ifndef __EMSCRIPTEN__
	bgbdt_mm_fclose(fd);
#endif

	if(rsz)*rsz=sz;
	return(buf);
}

BS2VM_API int bgbdt_mm_storefile(char *name, void *buf, int sz)
{
	void *fd;

	bgbdt_mm_initvfs();
	fd=bgbdt_mm_fopen(name, "wb");
	if(!fd)return(-1);

	bgbdt_mm_fwrite(buf, 1, sz, fd);
	bgbdt_mm_fclose(fd);
	return(0);
}

BS2VM_API int bgbdt_mm_storetextfile(char *name, char *buf)
{
	void *fd;
	char *s;

	bgbdt_mm_initvfs();
	fd=bgbdt_mm_fopen(name, "wt");
	if(!fd)return(-1);

	bgbdt_mm_fwrite(buf, 1, strlen(buf), fd);
	bgbdt_mm_fclose(fd);
	return(0);
}

