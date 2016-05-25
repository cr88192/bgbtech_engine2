/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <bgbasm.h>

// void *(*basm_fopen_fp)(char *name, char *mode);
// void (*basm_fclose_fp)(void *fd);
// int (*basm_fread_fp)(void *buf, int m, int n, void *fd);
// int (*basm_fwrite_fp)(void *buf, int m, int n, void *fd);
// int (*basm_feof_fp)(void *fd);
// s64 (*basm_ftell_fp)(void *fd);
// int (*basm_fseek_fp)(void *fd, s64 pos, int rel);

BGBASM_IOFuncs_t *basm_io;

s64 basm_tell_tmp(void *fd)
	{ return(ftell((FILE *)fd)); }
int basm_seek_tmp(void *fd, s64 pos, int rel)
	{ return(fseek((FILE *)fd, pos, rel)); }

BGBASM_IOFuncs_t *basm_getio()
{
	basm_initvfs();
	return(basm_io);
}

void basm_initvfs()
{
	static int init=0;

	if(init)return;
	init=1;

	blnk_initvfs();
	basm_io=blnk_getio();

#if 0
	if(!basm_io)
	{
		basm_io=malloc(sizeof(BGBASM_IOFuncs_t));
		memset(basm_io, 0, sizeof(BGBASM_IOFuncs_t));
	}

	if(!basm_io->fopen_fp)
	{
		basm_io->fopen_fp=(void *(*)(char*,char*))&fopen;
		basm_io->fclose_fp=(void (*)(void*))&fclose;
		basm_io->fread_fp=(int (*)(void*,int,int,void*))&fread;
		basm_io->fwrite_fp=(int (*)(void*,int,int,void*))&fwrite;
		basm_io->feof_fp=(int (*)(void*))&feof;
//		basm_io->ftell_fp=(s64 (*)(void*))&ftell;
//		basm_io->fseek_fp=(int (*)(void*,s64,int))&fseek;

		basm_io->ftell_fp=&basm_tell_tmp;
		basm_io->fseek_fp=&basm_seek_tmp;
	}
#endif
}

void *basm_fopen(char *name, char *mode)
{
	void *fd;

	basm_initvfs();
	fd=basm_io->fopen_fp(name, mode);
	return(fd);
}

void basm_fclose(void *fd)
{
	basm_initvfs();
	basm_io->fclose_fp((FILE *)fd);
}

int basm_fread(void *buf, int m, int n, void *fd)
{
	int i;
	basm_initvfs();
	i=basm_io->fread_fp(buf, m, n, fd);
	return(i);
}

int basm_fwrite(void *buf, int m, int n, void *fd)
{
	int i;
	basm_initvfs();
	i=basm_io->fwrite_fp(buf, m, n, fd);
	return(i);
}

int basm_feof(void *fd)
{
	basm_initvfs();
	return(basm_io->feof_fp(fd));
}

s64 basm_ftell(void *fd)
{
	basm_initvfs();
	return(basm_io->ftell_fp(fd));
}

int basm_fseek(void *fd, s64 pos, int rel)
{
	basm_initvfs();
	basm_io->fseek_fp(fd, pos, rel);
	return(basm_io->ftell_fp(fd));
}

int basm_fgetc(void *fd)
{
	unsigned char ch[4];
	int i;

	i=basm_fread(ch, 1, 1, fd);
	if(i<1)return(-1);
	return(ch[0]);
}

void basm_fputc(int c, void *fd)
{
	unsigned char ch[4];
	ch[0]=c;
	basm_fwrite(ch, 1, 1, fd);
}


void *basm_loadfile(char *name, int *rsz)
{
	void *buf;
	void *fd;
	int sz;

	basm_initvfs();
	fd=basm_fopen(name, "rb");
	if(!fd)return(NULL);

	basm_fseek(fd, 0, 2);
	sz=basm_ftell(fd);
	basm_fseek(fd, 0, 0);

	buf=malloc(sz+16);
	memset(buf, 0, sz+16);
	basm_fread(buf, 1, sz, fd);

	basm_fclose(fd);

	if(rsz)*rsz=sz;
	return(buf);
}

int basm_storefile(char *name, void *buf, int sz)
{
	void *fd;

	basm_initvfs();
	fd=basm_fopen(name, "wb");
	if(!fd)return(-1);

	basm_fwrite(buf, 1, sz, fd);
	basm_fclose(fd);
	return(0);
}

int basm_storetextfile(char *name, char *buf)
{
	void *fd;
	char *s;

	basm_initvfs();
	fd=basm_fopen(name, "wt");
	if(!fd)return(-1);
	basm_fwrite(buf, 1, strlen(buf), fd);
	basm_fclose(fd);
	return(0);
}
