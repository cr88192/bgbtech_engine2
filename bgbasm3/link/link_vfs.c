// #include <bgblink.h>
#include <bgbasm.h>

BLNK_IOFuncs_t *blnk_io;

s64 blnk_tell_tmp(void *fd)
	{ return(ftell((FILE *)fd)); }
int blnk_seek_tmp(void *fd, s64 pos, int rel)
	{ return(fseek((FILE *)fd, pos, rel)); }

BLNK_IOFuncs_t *blnk_getio()
{
	if(!blnk_io)
	{
		blnk_io=malloc(sizeof(BLNK_IOFuncs_t));
		memset(blnk_io, 0, sizeof(BLNK_IOFuncs_t));
	}

	return(blnk_io);
}

BASM_API BLNK_IOFuncs_t *BLNK_GetIOFuncs()
{
	return(blnk_getio());
}

void blnk_initvfs()
{
	static int init=0;

	if(init)return;
	init=1;

	if(!blnk_io)
		blnk_io=blnk_getio();

	if(!blnk_io->fopen_fp)
	{
		blnk_io->fopen_fp=(void *(*)(char*,char*))&fopen;
		blnk_io->fclose_fp=(void (*)(void*))&fclose;
		blnk_io->fread_fp=(int (*)(void*,int,int,void*))&fread;
		blnk_io->fwrite_fp=(int (*)(void*,int,int,void*))&fwrite;
		blnk_io->feof_fp=(int (*)(void*))&feof;
//		blnk_io->ftell_fp=(s64 (*)(void*))&ftell;
//		blnk_io->fseek_fp=(int (*)(void*,s64,int))&fseek;

		blnk_io->ftell_fp=&blnk_tell_tmp;
		blnk_io->fseek_fp=&blnk_seek_tmp;
	}
}

void *blnk_fopen(char *name, char *mode)
{
	void *fd;

	blnk_initvfs();
	fd=blnk_io->fopen_fp(name, mode);
	return(fd);
}

void blnk_fclose(void *fd)
{
	blnk_initvfs();
	blnk_io->fclose_fp((FILE *)fd);
}

int blnk_fread(void *buf, int m, int n, void *fd)
{
	int i;
	blnk_initvfs();
	i=blnk_io->fread_fp(buf, m, n, fd);
	return(i);
}

int blnk_fwrite(void *buf, int m, int n, void *fd)
{
	int i;
	blnk_initvfs();
	i=blnk_io->fwrite_fp(buf, m, n, fd);
	return(i);
}

int blnk_feof(void *fd)
{
	blnk_initvfs();
	return(blnk_io->feof_fp(fd));
}

s64 blnk_ftell(void *fd)
{
	blnk_initvfs();
	return(blnk_io->ftell_fp(fd));
}

int blnk_fseek(void *fd, s64 pos, int rel)
{
	blnk_initvfs();
	blnk_io->fseek_fp(fd, pos, rel);
	return(blnk_io->ftell_fp(fd));
}

int blnk_fgetc(void *fd)
{
	unsigned char ch[4];
	int i;

	i=blnk_fread(ch, 1, 1, fd);
	if(i<1)return(-1);
	return(ch[0]);
}

void blnk_fputc(int c, void *fd)
{
	unsigned char ch[4];
	ch[0]=c;
	blnk_fwrite(ch, 1, 1, fd);
}


void *blnk_loadfile(char *name, int *rsz)
{
	void *buf;
	void *fd;
	int sz;

	blnk_initvfs();
	fd=blnk_fopen(name, "rb");
	if(!fd)return(NULL);

	blnk_fseek(fd, 0, 2);
	sz=blnk_ftell(fd);
	blnk_fseek(fd, 0, 0);

	buf=malloc(sz+16);
	memset(buf, 0, sz+16);
	blnk_fread(buf, 1, sz, fd);

	blnk_fclose(fd);

	if(rsz)*rsz=sz;
	return(buf);
}

int blnk_storefile(char *name, void *buf, int sz)
{
	void *fd;

	blnk_initvfs();
	fd=blnk_fopen(name, "wb");
	if(!fd)return(-1);

	blnk_fwrite(buf, 1, sz, fd);
	blnk_fclose(fd);
	return(0);
}

int blnk_storetextfile(char *name, char *buf)
{
	void *fd;
	char *s;

	blnk_initvfs();
	fd=blnk_fopen(name, "wt");
	if(!fd)return(-1);

	blnk_fwrite(buf, 1, strlen(buf), fd);
	blnk_fclose(fd);
	return(0);
}

