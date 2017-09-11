#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

#define VFILE	FILE
#define vffopen	fopen
#define vfread	fread
#define vfwrite	fwrite
#define vfseek	fseek
#define vftell	ftell
#define vfgetc	fgetc
#define vfputc	fputc
#define vfclose	fclose

// #define FRGL_JPG_RGBA 0
// #define FRGL_JPG_BGRA 1

#include "bgbdt_dummyavi.h"

#include "tk_outavi.h"

void *frgl_malloc(int sz)
{
	void *ptr;
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

#include "frgl_readavi.c"
#include "tk_outavi.c"

int ParrTrans(byte *obuf, byte *ibuf, int xs, int ys)
{
	static int ldc=0;
	int paxmap[80*50];
	int y0, y1, y2, y3;
	int x0, x1, x2, x3;
	int xt0, yt0, xt1, yt1, xts, yts, xtf, ytf;
	int p0, p1, p2, p3;
	int y, x, xc, yc, dc, dc1, e, be, bdc1, ldc1, ndc1;
	int pmsc, bi;
	int i, j, k, l;

	memcpy(obuf, ibuf, xs*ys*4);
	
	dc=0;
	
	y=ys/2; be=999999;
	for(i=0; i<(xs/8); i++)
	{
		e=0;
		for(y=0; y<8; y++)
			for(x=0; x<(xs/2); x++)
		{
			y0=y*(ys/8);
			
			x0=x;
			x1=(xs/2)+x;

			x0-=i;
			x1+=i;
			x0=clamp(x0, 0, (xs/2-1));
			x1=clamp(x1, xs/2, (xs-1));

			l=1;
	//		for(l=0; l<3; l++)
			if(1)
			{
				p0=ibuf[(y0*xs+x0)*4+l];
				p1=ibuf[(y0*xs+x1)*4+l];
				
				j=p0-p1; j=j^(j>>31);
				e+=j;
			}
		}
		
		if(e<be)
			{ dc=i; be=e; }
	}

	k=((ldc*240)>>8)+(16*dc);
	ldc=k;
	dc=(k+127)>>8;
	
//	dc=(ldc*3+dc)/4;
//	ldc=dc;


	xts=(xs/2)/80;
	yts=ys/45;
	
	ndc1=dc;

	for(y=0; y<45; y++)
		for(x=0; x<80; x++)
	{
		y0=y*(ys/8);
		
		xt0=(x*xts)+(xts/2);
		xt1=(xs/2)+xt0;
//		yt0=y*yts+(yts/2);
		yt0=y*yts;

		bdc1=dc; be=999999; bi=0;
		for(i=0; i<64; i++)
		{
			j=(i>>1)^((i<<31)>>31);
			dc1=dc+j;
			if(dc1<0)
				continue;

//			e=0;
			e=i*16;
			for(yc=0; yc<4; yc++)
				for(xc=0; xc<64; xc++)
			{
				y1=yt0+yc*(yts/4);
				x0=xt0-dc1+(xc-32);
				x1=xt1+dc1+(xc-32);
				x0=clamp(x0, 0, (xs/2-1));
				x1=clamp(x1, xs/2, (xs-1));
				
				p0=ibuf[(y1*xs+x0)*4+1];
				p1=ibuf[(y1*xs+x1)*4+1];

				j=p0-p1;
//				j=j^(j>>31);
				j=j*j;
				e+=j;
			}
			
//			if(e>(2*be))
			if(e>(2*be) && !((bi^i)&1))
				break;

			if(e<be)
				{ bdc1=dc1; be=e; bi=i; }
		}
		
		paxmap[y*80+x]=bdc1;
		
		if(bdc1>ndc1)
			ndc1=bdc1;
	}

	pmsc=65536/(ndc1+1);


	xtf=256/xts;
	ytf=256/yts;
	ldc1=dc*256;
	
	for(y=0; y<ys; y++)
	{
		for(x=0; x<(xs/2); x++)
		{
			xt0=(x*xtf)>>8;
			yt0=(y*ytf)>>8;
			xt0=clamp(xt0, 0, 80);
			yt0=clamp(yt0, 0, 45);
			dc1=paxmap[yt0*80+xt0];

			k=((ldc1*240)>>8)+(16*dc1);
//			k=((ldc1*208)>>8)+(48*dc1);
			ldc1=k;
			dc1=(k+127)>>8;
		
			x0=x;
			x1=(xs/2)+x;
			x2=x;
			x3=(xs/2)+x;
			
			x0-=dc1;
			x1+=dc1;
			x0=clamp(x0, 0, (xs/2-1));
			x1=clamp(x1, xs/2, (xs-1));
			
			y0=y;
			y1=y/2;
			y2=y1+(ys/2);

			for(l=0; l<3; l++)
			{
				p0=ibuf[(y*xs+x0)*4+l];
				p1=ibuf[(y*xs+x1)*4+l];
				obuf[(y0*xs+x2)*4+l]=(p0+p1)/2;
				obuf[(y1*xs+x3)*4+l]=(p0-p1)/2+128;

				k=(dc1*pmsc)>>8;
				k=clamp255(255-k);
				obuf[(y2*xs+x3)*4+l]=k;
			}
		}
	}
	return(0);
}

int main(int argc, char *argv[])
{
	short sndbuf[8192];
	char *ifn, *ofn;

	FRGL_AVI_Context *ictx;
	FRGL_AVICtx *octx;
	FRGL_Video_Stats *vstat;
	byte *buf;
	byte *obuf;
	byte *tbuf;
	int t0, t1, t2, te;
	int xs, ys;
	int sz, sndlen, slen2;

	u32 px;
	int i, j, k, l;

	
//	ifn="M:\\FantasticFallsPT2.avi";
//	ifn="M:\\3DFirstPersonMario.avi";
	ifn="M:\\Demo3D.avi";
	ofn="M:\\TestOut3D_CRAM0.avi";

	ictx=FRGL_AVI_LoadAVI(ifn);
	vstat=FRGL_AVI_GetStats(ictx);
	
	xs=vstat->width;
	ys=vstat->height;
	
	octx=FRGL_AVI_OpenOutStream2(ofn, vstat->width, vstat->height,
		1000000.0/vstat->frametime, RIFF_TAG_CRAM, FRGL_AVI_AUID_11K8BM);

	tbuf=malloc(1<<20);
//	sndlen=44100*(vstat->frametime/1000000.0);
	sndlen=11025*(vstat->frametime/1000000.0)+0.5;
	sndlen=8192;
	
	obuf=malloc(xs*ys*4);
	
//	memset(sndbuf, 0, 2*sndlen);
	
	for(i=0; i<vstat->num_frames; i++)
	{
		printf("%d/%d\r", i, vstat->num_frames);
		buf=FRGL_AVI_DecodeFrame2(ictx, 0, FRGL_JPG_BGRA);
		slen2=FRGL_AVI_GetMonoSamplesRate(ictx, sndbuf, sndlen, 11025);

		ParrTrans(obuf, buf, xs, ys);

//		sz=cram_repak_buf(buf, tbuf, 1<<20, 1<<20, vstat);

		FRGL_AVI_WriteContextMonoSamples(octx, sndbuf, slen2);
//		FRGL_AVI_EmitCodedFrame(octx, tbuf, sz, 0);
//		FRGL_AVI_EmitAudioFrame(octx);
		FRGL_AVI_EncodeFrame2(octx, obuf,
			xs, ys, 90|FRGL_QFL_BATCHENCODE, FRGL_JPG_BGRA);
	}
	printf("\n");
	
	FRGL_AVI_CloseOutStream(octx);
	
	printf("Done\n");
}
