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
	byte aidx[256];
	int acnt[256];
	BGBDT_RiceContext *ctx;
	int sz, ix;
	int i, j, k;


	ctx=BGBDT_Rice_AllocContext();
	ctx->lzctrl=3;

	BGBDT_Rice_SetupWrite(ctx, obuf, obsz);

//	BGBDT_Rice_WriteRice(ctx, 1, 3);
//	BGBDT_Rice_WriteRice(ctx, ts, 7);

	BGBDT_EncodeChunkLZ(ctx, ibuf, ibsz);

	sz=BGBDT_Rice_FlushBits(ctx);

//	printf("kr=%d(%d, %.1f) ks=%d(%d, %.1f) kl=%d(%d, %.1f) kd=%d(%d, %.1f)\n",
//		ctx->stat_kr, ctx->stat_nr, (1.0*ctx->stat_kr)/ctx->stat_nr,
//		ctx->stat_ks, ctx->stat_ns, (1.0*ctx->stat_ks)/ctx->stat_ns,
//		ctx->stat_kl, ctx->stat_nl, (1.0*ctx->stat_kl)/ctx->stat_nl,
//		ctx->stat_kd, ctx->stat_nd, (1.0*ctx->stat_kd)/ctx->stat_nd);

	printf("Current:\n");

	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			ix=i*16+j;
			ix=(byte)(ix+ctx->lzwpos);
			k=ctx->lzwin[ix];
			if((k<' ') || (k>'~'))k='.';
			printf("%02X ", ctx->lzwin[ix]);
		}

		for(j=0; j<16; j++)
		{
			ix=i*16+j;
			ix=(byte)(ix+ctx->lzwpos);
			k=ctx->lzwin[ix];
			if((k<' ') || (k>'~'))k='.';
			printf("%c", k);
		}
		printf("\n");
	}
	printf("Enc Ending LzWPos=%d\n", ctx->lzwpos);

	for(i=0; i<256; i++)
		{ aidx[i]=i; acnt[i]=ctx->lzstat[i]; }
	for(i=0; i<256; i++)
		for(j=i+1; j<256; j++)
	{
		if(acnt[j]>acnt[i])
		{
			k=aidx[i]; aidx[i]=aidx[j]; aidx[j]=k;
			k=acnt[i]; acnt[i]=acnt[j]; acnt[j]=k;
		}
	}

#if 1
	printf("Optimal:\n");
	for(i=0; i<16; i++)
	{
		for(j=0; j<16; j++)
		{
			ix=i*16+j;
			printf("%02X ", aidx[ix]);
		}

		for(j=0; j<16; j++)
		{
			ix=i*16+j;
			k=aidx[ix];
			if((k<' ') || (k>'~'))k='.';
			printf("%c", k);
		}
		printf("\n");
	}
#endif

	printf("kr: t=%d, n=%d, avg=%.2f\n",
		ctx->stat_kr, ctx->stat_nr, (1.0*ctx->stat_kr)/ctx->stat_nr);
	printf("ks: t=%d, n=%d, avg=%.2f\n",
		ctx->stat_ks, ctx->stat_ns, (1.0*ctx->stat_ks)/ctx->stat_ns);
	printf("kl: t=%d, n=%d, avg=%.2f\n",
		ctx->stat_kl, ctx->stat_nl, (1.0*ctx->stat_kl)/ctx->stat_nl);
	printf("kd: t=%d, n=%d, avg=%.2f\n",
		ctx->stat_kd, ctx->stat_nd, (1.0*ctx->stat_kd)/ctx->stat_nd);

	BGBDT_Rice_FreeContext(ctx);
	return(sz);
}

int CLZ_DecodeBuffer(byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	BGBDT_RiceContext *ctx;
	int sz;


	ctx=BGBDT_Rice_AllocContext();
	ctx->lzctrl=3;

	BGBDT_Rice_SetupRead(ctx, ibuf, ibsz);

//	BGBDT_Rice_WriteRice(ctx, 1, 3);
//	BGBDT_Rice_WriteRice(ctx, ts, 7);

	sz=BGBDT_DecodeChunkLZ(ctx, obuf, obsz);

	printf("Dec Ending LzWPos=%d\n", ctx->lzwpos);

	BGBDT_Rice_FreeContext(ctx);
	return(sz);
}

int CLZ_FindSTF3Err(byte *ibuf, int ibsz, int p1, int p2, int p3, int be)
{
	byte lzwin[256];
	byte lzidx[256];
	int ix0, ix1, ix2, ix3;
	int sy0, sy1, sym, lzwpos;
	int err;
	int i, j, k;
	
	for(i=0; i<256; i++)
		{ lzwin[i]=i; lzidx[i]=i; }

	err=0; lzwpos=0;
	for(i=0; i<ibsz; i++)
	{
		sym=ibuf[i];
	
		ix0=(byte)(lzidx[sym]-lzwpos);
//		ix1=(ix0*(p1-((ix0*p2)/256)))/256;

		if(ix0<p3)
		{
			ix1=(ix0*(256*p1-ix0*p2))/65536;
			ix2=(byte)(ix0+lzwpos);
			ix3=(byte)(ix1+lzwpos);
		}else
		{
			ix1=-1;
			ix2=(byte)(ix0+lzwpos);
			ix3=(byte)(ix1+lzwpos);
			lzwpos--;
		}

		sy0=lzwin[ix2];	sy1=lzwin[ix3];
		lzwin[ix2]=sy1;	lzwin[ix3]=sy0;
		lzidx[sy0]=ix3;	lzidx[sy1]=ix2;
		
		err+=ix0;
//		err+=ix0*ix0;
//		err+=(7*ix0+ix0*ix0)>>3;
		if(err>be)
			break;
	}
	return(err);
}

int CLZ_FindBestSTF3(byte *ibuf, int ibsz)
{
	int p0, p1, p2, pe;
	int b0, b1, b2, be;
	
//	b0=224; b1=132; b2=72;
//	be=CLZ_FindSTF3Err(ibuf, ibsz, b0, b1, b2, 999999999);
	be=1<<30;
	
	p0=224; p1=132; p2=72;
//	p1=68;
	p1=72;
//	p1=100;
//	for(p0=200; p0<244; p0++)
//		for(p1=32; p1<240; p1++)
			for(p2=64; p2<88; p2++)
	{
		pe=CLZ_FindSTF3Err(ibuf, ibsz, p0, p1, p2, be);
		
		if(pe<be)
		{
			b0=p0;
			b1=p1;
			b2=p2;
			be=pe;
		}
	}
	
	printf("Best=%d %d %d\n", b0, b1, b2);
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
	byte *ibuf, *obuf, *obuf2;
	int sz, osz, osz2;
	int i, j, k;
	
	ibuf=readfile(argv[1], &sz);
	if(!ibuf)
		return(-1);

	obuf=malloc(1<<20);
	obuf2=malloc(1<<20);
	osz=CLZ_EncodeBuffer(ibuf, obuf, sz, 1<<20);
	osz2=CLZ_DecodeBuffer(obuf, obuf2, osz, 1<<20);
	
	printf("%d->%d (%d%%)\n", sz, osz, (osz*100)/sz);
	printf("%d->%d (%d%%)\n", osz, osz2, (osz2*100)/osz);
	
	if(sz!=osz2)
	{
		printf("Size Mismatch\n");
		return(-1);
	}
	
	if(memcmp(ibuf, obuf2, sz))
	{
		printf("Data Corrupt\n");
//		return(-1);
		for(i=0; i<sz; i++)
			if(ibuf[i]!=obuf2[i])
				break;
		printf("@%d %02X -> %02X\n", i, ibuf[i], obuf2[i]);
	}else
	{
		printf("Data OK\n");
	}
	
//	CLZ_FindBestSTF3(ibuf, sz);
	
	return(0);
}
