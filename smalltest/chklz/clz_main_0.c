#include "clz_multi.c"

void *frgl_malloc(int sz)
{
	void *ptr;
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

int CLZ_EncodeBuffer(byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	BGBDT_RiceContext *ctx;
	int sz;


	ctx=BGBDT_Rice_AllocContext();
	ctx->lzctrl=1;

	BGBDT_Rice_SetupWrite(ctx, obuf, obsz);

//	BGBDT_Rice_WriteRice(ctx, 1, 3);
//	BGBDT_Rice_WriteRice(ctx, ts, 7);

	BGBDT_EncodeChunkLZ(ctx, ibuf, ibsz);

	sz=BGBDT_Rice_FlushBits(ctx);
	BGBDT_Rice_FreeContext(ctx);
	return(sz);
}

byte *readfile(char *path, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz;
	
	fd=fopen(path, "rb");
	if(!fd)
		return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz);
	fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

int main(int argc, char *argv[])
{
	byte *ibuf, *obuf;
	int sz, osz;
	
	ibuf=readfile(argv[1], &sz);
	if(!ibuf)
		return(-1);

	obuf=malloc(1<<20);
	osz=CLZ_EncodeBuffer(ibuf, obuf, sz, 1<<20);
	
	printf("%d->%d (%d%%)\n", sz, osz, (osz*100)/sz);
	
	return(0);
}
