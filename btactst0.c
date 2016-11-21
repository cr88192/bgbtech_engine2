#include <bteifgl.h>
#include <time.h>
#include <math.h>

#undef printf

void BGBMID_StoreWAV(char *name, void *buf,
	int ch, int rt, int bits, int len)
{
	byte tb1[256];
	byte *ct;
	FILE *fd;
	int i;

	fd=fopen(name, "wb");
	
	ct=tb1;

	*ct++='R';		*ct++='I';
	*ct++='F';		*ct++='F';
	*ct++=0;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='W';		*ct++='A';
	*ct++='V';		*ct++='E';

	//format header
	*ct++='f';		*ct++='m';
	*ct++='t';		*ct++=' ';
	*ct++=16;		*ct++=0;
	*ct++=0;		*ct++=0;

	*ct++=1;		*ct++=0;
	*ct++=ch;		*ct++=0;

	*ct++=rt;		*ct++=rt>>8;
	*ct++=rt>>16;	*ct++=rt>>24;

	i=rt*ch*(bits/8);
	*ct++=i;		*ct++=i>>8;
	*ct++=i>>16;	*ct++=i>>24;

	*ct++=ch*(bits/8);		*ct++=0;
	*ct++=bits;			*ct++=0;


	//data
	*ct++='d';		*ct++='a';
	*ct++='t';		*ct++='a';

	i=len*ch*(bits/8);
	*ct++=i;		*ct++=i>>8;
	*ct++=i>>16;	*ct++=i>>24;

	fwrite(tb1, 1, ct-tb1, fd);

	fwrite(buf, 1, i, fd);
	if(i)fputc(0, fd);

	i=ftell(fd);
	fseek(fd, 4, 0);

	i-=8;	
	fputc(i&0xFF, fd);		fputc((i>>8)&0xFF, fd);
	fputc((i>>16)&0xFF, fd);	fputc((i>>24)&0xFF, fd);
	fclose(fd);
}

void BGBMID_StoreWAV_BTAC1C(char *name, void *buf,
	int ch, int rt, int bsz, int len)
{
	byte tb1[256];
	byte *ct;
	FILE *fd;
	int i, bl;

	fd=fopen(name, "wb");
	
	bl=2*bsz;
	ct=tb1;

	*ct++='R';		*ct++='I';
	*ct++='F';		*ct++='F';
	*ct++=0;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='W';		*ct++='A';
	*ct++='V';		*ct++='E';

	//format header
	*ct++='f';		*ct++='m';
	*ct++='t';		*ct++=' ';
	*ct++=16;		*ct++=0;
	*ct++=0;		*ct++=0;

	*ct++=0x1C;		*ct++=0x7B;
	*ct++=ch;		*ct++=0;

	*ct++=rt;		*ct++=rt>>8;
	*ct++=rt>>16;	*ct++=rt>>24;

	i=(rt*4)/8;
	*ct++=i;		*ct++=i>>8;
	*ct++=i>>16;	*ct++=i>>24;

	i=bsz;
	*ct++=i;			*ct++=i>>8;
	*ct++=4;			*ct++=0;


	//data
	*ct++='d';		*ct++='a';
	*ct++='t';		*ct++='a';

	i=((len+(bl-1))/bl)*bsz;
	*ct++=i;		*ct++=i>>8;
	*ct++=i>>16;	*ct++=i>>24;

	fwrite(tb1, 1, ct-tb1, fd);

	fwrite(buf, 1, i, fd);
	if(i)fputc(0, fd);

	i=ftell(fd);
	fseek(fd, 4, 0);

	i-=8;	
	fputc(i&0xFF, fd);		fputc((i>>8)&0xFF, fd);
	fputc((i>>16)&0xFF, fd);	fputc((i>>24)&0xFF, fd);
	fclose(fd);
}

int BGBMID_FlattenWAV_BTAC1C(byte *obuf, void *ibuf,
	int ch, int rt, int bsz, int len)
{
	byte *ct;
	int i, j, bl;

	bl=2*bsz;
	ct=obuf;

	*ct++='R';		*ct++='I';
	*ct++='F';		*ct++='F';
	*ct++=0;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='W';		*ct++='A';
	*ct++='V';		*ct++='E';

	//format header
	*ct++='f';		*ct++='m';
	*ct++='t';		*ct++=' ';
	*ct++=20;		*ct++=0;
	*ct++=0;		*ct++=0;

	if(ch!=1)
	{
		*ct++=0x1C;		*ct++=0x7B;
		*ct++=ch;		*ct++=0;
	}else
	{
		*ct++=0x11;		*ct++=0x00;
		*ct++=ch;		*ct++=0;
	}

	*ct++=rt;		*ct++=rt>>8;
	*ct++=rt>>16;	*ct++=rt>>24;

	i=(rt*4)/8;
	*ct++=i;		*ct++=i>>8;
	*ct++=i>>16;	*ct++=i>>24;

	i=bsz;
	*ct++=i;			*ct++=i>>8;
	*ct++=4;			*ct++=0;

	*ct++=2;			*ct++=0;
	*ct++=0xF9;			*ct++=0x03;

	//data
	*ct++='d';		*ct++='a';
	*ct++='t';		*ct++='a';

	i=((len+(bl-1))/bl)*bsz;
	*ct++=i;		*ct++=i>> 8;
	*ct++=i>>16;	*ct++=i>>24;

	j=(i+1)&(~1);
	memcpy(ct, ibuf, i);
	ct+=j;

	i=(ct-obuf)-8;
	obuf[4]=i;		obuf[5]=i>> 8;
	obuf[6]=i>>16;	obuf[7]=i>>24;
	
	return(ct-obuf);
}

int BtPk_ImageAddSound(BtPak0_Image *img, char *src, int rate)
{
	char tb[1024], tb2[1024];
	BGBDT_SndSampler *samp;
	s16 *sbuf, *sbuf2, *sbuf3;
	byte *bbuf, *obuf;
	double f, g;
	char *s, *t;
	int len, len2, bsz, blen, blen2, blg2, blks, idx, insz;
	int ch, sz;
	int i, j, k;

	sprintf(tb, "%s.wav", src);

	s=tb; t=tb2;
	strcpy(t, "resource_dev/dump/"); t+=strlen(t);
	while(*s)
	{
		if(*s=='/')
			{ s++; *t++='_'; continue; }
		*t++=*s++;
	}
	*t++=0;

//	samp=BGBDT_Snd_GetLoadSamplerWAV(src);
	samp=BGBDT_Snd_GetLoadSampler(src);

	len=samp->len;
	ch=samp->wf_chan;

	if(rate<=0)
		rate=22050;
		
	len=len*(((double)rate)/samp->wf_rate);

	sbuf=frgl_malloc((len+1024)*2*sizeof(s16));
//	sbuf2=frgl_malloc((len+1024)*2*sizeof(s16));
//	sbuf3=frgl_malloc((len+1024)*2*sizeof(s16));

	if(ch==1)
	{
		if(rate==samp->wf_rate)
		{
			for(i=0; i<len; i++)
				{ sbuf[i]=BGBDT_Snd_GetSampleMono(samp, i); }
		}else
		{
			for(i=0; i<len; i++)
			{
				sbuf[i]=samp->GetSampleMonoFqClz(samp,
					rate*16, i*16);
			}
		}

		bsz=512; blen=(bsz-4)*2; blen2=bsz*2;
		blks=(len+(blen2-1))/blen2;
		blg2=10; len2=blks*blen2;

		bbuf=frgl_malloc(blks*bsz);
		obuf=frgl_malloc(blks*bsz+1024);

		idx=0;
		for(i=0; i<blks; i++)
		{
			BGBDT_SndBTAC1C_EncodeBlockMonoLg2(sbuf+(i*blen2),
				bbuf+(i*bsz), blg2, &idx);
		}

		sz=BGBMID_FlattenWAV_BTAC1C(obuf,
			bbuf, 1, rate, bsz, len);
		BtPak_ImageStoreFile(img, tb, obuf, sz);	
	}else
	{
		if(rate==samp->wf_rate)
		{
			for(i=0; i<len; i++)
			{
				sbuf[i*2+0]=samp->GetSampleMultiMod(samp, i, 0);
				sbuf[i*2+1]=samp->GetSampleMultiMod(samp, i, 1);
			}
		}else
		{
			f=(((double)rate)/samp->wf_rate);
			f=1.0/f;
			for(i=0; i<len; i++)
			{
				j=i*f+0.5;
				sbuf[i*2+0]=samp->GetSampleMultiMod(samp, j, 0);
				sbuf[i*2+1]=samp->GetSampleMultiMod(samp, j, 1);
			}
		}

		bsz=512; blen=(bsz-4)*2; blen2=bsz*2;
		blks=(len+(blen2-1))/blen2;
		blg2=10; len2=blks*blen2;

		bbuf=frgl_malloc(blks*bsz);
		obuf=frgl_malloc(blks*bsz+1024);

		idx=0;
		for(i=0; i<blks; i++)
		{
			BGBDT_SndBTAC1C_EncodeBlockStereoLg2(sbuf+(i*2*blen2),
				bbuf+(i*bsz), blg2, &idx);
		}

		sz=BGBMID_FlattenWAV_BTAC1C(obuf,
			bbuf, 2, rate, bsz, len);
		BtPak_ImageStoreFile(img, tb, obuf, sz);
	}

	printf("added sound: %-48s %dk\n", src, (sz+511)/1024);

	vf_storefile(tb2, obuf, sz);

	return(0);
}

int BtPk_ImageAddTexImg(BtPak0_Image *img, char *src,
	int oxs, int oys, int qfl)
{
	char tb[1024], tb2[1024];
	byte *ibuf, *ibuf2, *obuf;
	char *s, *t;
	int xs, ys, xs1, ys1, txs, tys, sz, clrs, jclrs, al;
	int i, j, k, l;

	if(qfl&(1<<28))
	{
		sprintf(tb, "%s.jpg", src);
	}else
	{
		sprintf(tb, "%s.bmp", src);
	}
	
	s=tb; t=tb2;
	strcpy(t, "resource_dev/dump/"); t+=strlen(t);
	while(*s)
	{
		if(*s=='/')
			{ s++; *t++='_'; continue; }
		*t++=*s++;
	}
	*t++=0;

//	ibuf=Tex_LoadFile2Raw(src, &xs, &ys);
	ibuf=Tex_LoadFile2AlphaRaw(src, &xs, &ys);

	if(!ibuf)
	{
		printf("can't add teximg: %s\n", src);
		return(-1);
	}

	xs1=xs;
	ys1=ys;

	if((oxs>0) && (oys>0))
		{ xs1=oxs; ys1=oys; }
	if((oxs>0) && (oys==-1))
	{
		xs1=xs*(oxs/10000.0);
		ys1=ys*(oxs/10000.0);
	}

	txs=1;	tys=1;
	while(txs<xs1)txs<<=1;
	while(tys<ys1)tys<<=1;

//	if(txs>2048)txs=2048;
//	if(tys>2048)tys=2048;
	if(txs>4096)txs=4096;
	if(tys>4096)tys=4096;

	if((xs!=txs) || (ys!=tys))
	{
		ibuf2=frgl_malloc(txs*tys*4);
		Tex_ResampleSpline(ibuf, xs, ys, ibuf2, txs, tys);

		frgl_free(ibuf);
		ibuf=ibuf2;
		xs=txs; ys=tys;
	}

	obuf=frgl_malloc(1<<24);

	clrs=BTIC4B_CLRS_RGBA;
	jclrs=BTIC1H_PXF_RGBA;
	if(BTIC4B_Img_CheckRGBeP(ibuf, xs, ys))
	{
		clrs=BTIC4B_CLRS_RGB8E8;
		jclrs=BTIC1H_PXF_RGB8E8;
	}

	l=xs;
	if(ys<l)l=ys;

	al=0;
	for(i=0; i<l; i++)
	{
		j=ibuf[(i*xs+i)*4+3];
		if(j!=255)
			{ al=1; break; }
	}

	if(qfl&(1<<28))
	{
//		sz=PDJPG_EncodeRgba(ibuf, obuf, xs, ys, qfl);
		sz=PDJPG_EncodeClrs(ibuf, obuf, xs, ys, qfl, jclrs);
	}else
	{
		qfl|=BTIC4B_QFL_OPTBCN;
		sz=BTIC4B_EncodeImgBmpBuffer(obuf, 1<<24,
			ibuf, xs, ys, qfl, clrs);
	}

//	printf("added teximg: %s\n", src);
	printf("added teximg: %-48s %s %dk %1.1fbpp\n",
		src, (clrs!=BTIC4B_CLRS_RGBA)?"HDR":(al?"LDRA":"LDR"),
		(sz+511)/1024, sz*8.0/(xs*ys));
	BtPak_ImageStoreFile(img, tb, obuf, sz);
	vf_storefile(tb2, obuf, sz);

	frgl_free(ibuf);
	frgl_free(obuf);

	return(0);
}

int BtPk_ImageAddFile(BtPak0_Image *img, char *src)
{
	byte *ibuf;
	int isz;
		
	ibuf=VfLoadFile2(src, &isz);
	if(!ibuf)
	{
		printf("can't add file: %s\n", src);
		return(-1);
	}

//	printf("added file: %s\n", src);
	printf("added file: %-48s %dk\n", src, (isz+511)/1024);
	BtPak_ImageStoreFile(img, src, ibuf, isz);
	return(0);
}

int main()
{
	char tb[1024];
	BtPak0_Image *img;
	BGBDT_SndSampler *samp;
	byte *inbuf, *cs, *cse;
	s16 *sbuf, *sbuf2, *sbuf3;
	byte *bbuf;
	char **a;
	double e;
	int t0, t1, t2, ts, te, dt;
	int len, bsz, blen, blen2, blg2, blks, idx, insz;
	int xs, ys, qf, fl;
	int i, j, k, r;

	VfMount("resource_dev", "/", "dir", "");

	inbuf=VfLoadFile2("bt2pk_manifest.cfg", &insz);
	
	cs=inbuf; cse=inbuf+insz; img=NULL;
	while(*cs && (cs<cse))
	{
		cs=frgl_rsgets(tb, 1022, cs);
		a=frgl_rsplit(tb);
		
		if(!a || !a[0])
			continue;
		if(!strcmp(a[0], "image"))
		{
			if(img)
			{
				BtPak_ImageCloseImage(img);
				img=NULL;
			}

			if(a[1])
				img=BtPak_CreateImageRW(a[1]);
			continue;
		}
		
		if(!strcmp(a[0], "addsound"))
		{
			xs=0; fl=0;
			for(i=2; a[i]; i++)
			{
				if(!strncmp(a[i], "rt=", 3))
					{ xs=atoi(a[i]+3); continue; }
//				if(!strncmp(a[i], "qf=", 3))
//					{ qf=atoi(a[i]+3); continue; }
			}
		
			BtPk_ImageAddSound(img, a[1], xs);
			continue;
		}

		if(!strcmp(a[0], "addteximg"))
		{
			xs=0; ys=0; qf=80; fl=0;
			for(i=2; a[i]; i++)
			{
				if(!strncmp(a[i], "xs=", 3))
					{ xs=atoi(a[i]+3); continue; }
				if(!strncmp(a[i], "ys=", 3))
					{ ys=atoi(a[i]+3); continue; }
				if(!strncmp(a[i], "qf=", 3))
					{ qf=atoi(a[i]+3); continue; }

				if(!strncmp(a[i], "sc=", 3))
					{ xs=atof(a[i]+3)*10000+0.5; ys=-1; continue; }

				if(!strcmp(a[i], "jpg"))
					{ fl|=(1<<28); continue; }
			}
		
			BtPk_ImageAddTexImg(img, a[1], xs, ys, qf|fl);
			continue;
		}

		if(!strcmp(a[0], "addfile"))
		{
			BtPk_ImageAddFile(img, a[1]);
			continue;
		}
	}

	if(img)
	{
		BtPak_ImageCloseImage(img);
		img=NULL;
	}

#if 0
//	img=BtPak_LoadImageRW("test0.bpk");
	img=BtPak_CreateImageRW("test0.bpk");

//	samp=BGBDT_Snd_GetLoadSampler("sound_dev/pi0_amb0");
//	samp=BGBDT_Snd_GetLoadSampler("resource_dev/sound_dev/"
//		"LindseyStirling_Shadows_ADPCM_22");
	samp=BGBDT_Snd_GetLoadSampler("resource_dev/sound_dev/"
//		"OwenWasHer_Touhou6_PCM16S_22");
		"LindseyStirling_Shadows_PCM16S_22");

#if 0
	ts=clock(); t0=ts; te=ts+30000; r=0;
	while(t0<te)
	{
		for(i=0; i<16384; i++)
		{
//			BGBDT_Snd_GetSampleMonoMod(samp, r++);
//			BGBDT_Snd_GetSampleMono(samp, i); r++;
//			samp->GetSampleMonoRaw(samp, i); r++;
			samp->GetSampleMonoFqMod(samp, 44100*16, (r++)*16);
		}
		t0=clock();
		dt=t0-ts;
		printf("%d %d %.2fkS/s\r", dt, r, ((double)r)/dt);
	}
	printf("\n");
#endif

	len=samp->len;
	sbuf=frgl_malloc((len+1024)*2*sizeof(s16));
	sbuf2=frgl_malloc((len+1024)*2*sizeof(s16));
	sbuf3=frgl_malloc((len+1024)*2*sizeof(s16));
	for(i=0; i<len; i++)
	{
//		sbuf[i]=BGBDT_Snd_GetSampleMono(samp, i);
		sbuf[i*2+0]=samp->GetSampleMultiMod(samp, i, 0);
		sbuf[i*2+1]=samp->GetSampleMultiMod(samp, i, 1);
	}
	bsz=512; blen=(bsz-4)*2; blen2=bsz*2;
	blks=(len+(blen2-1))/blen2;
	blg2=10;

	bbuf=frgl_malloc(blks*bsz);
	
	idx=0;
	for(i=0; i<blks; i++)
	{
//		BGBDT_SndBTAC1C_EncodeBlockMonoLg2(sbuf+(i*blen2),
//			bbuf+(i*bsz), blg2, &idx);
		BGBDT_SndBTAC1C_EncodeBlockStereoLg2(sbuf+(i*2*blen2),
			bbuf+(i*bsz), blg2, &idx);
	}

	for(i=0; i<blks; i++)
	{
//		BGBDT_SndBTAC1C_DecodeBlockMonoLg2(
//			bbuf+(i*bsz), sbuf2+(i*blen2), blg2);
		BGBDT_SndBTAC1C_DecodeBlockStereoLg2(
			bbuf+(i*bsz), sbuf2+(i*2*blen2), blg2);
	}
	
	e=0;
	for(i=0; i<len; i++)
	{
//		j=sbuf[i]-sbuf2[i];
//		e+=j*j;
		j=sbuf[i*2+0]-sbuf2[i*2+0];
		k=sbuf[i*2+1]-sbuf2[i*2+1];
		e+=j*j+k*k;
	}

	for(i=0; i<len; i++)
	{
		sbuf3[i*2+0]=sbuf2[i*2+0]-sbuf[i*2+0];
		sbuf3[i*2+1]=sbuf2[i*2+1]-sbuf[i*2+1];
	}

	printf("RMSE=%f\n", sqrt(e/len));
	
//	BGBMID_StoreWAV("btactst0_sb1.wav", sbuf, 1, samp->wf_rate, 16, len);
//	BGBMID_StoreWAV("btactst0_sb2.wav", sbuf2, 1, samp->wf_rate, 16, len);

	BGBMID_StoreWAV_BTAC1C("btactst0_bt1.wav",
		bbuf, 2, samp->wf_rate, bsz, len);

	BGBMID_StoreWAV("btactst0_sb1.wav", sbuf, 2, samp->wf_rate, 16, len);
	BGBMID_StoreWAV("btactst0_sb2.wav", sbuf2, 2, samp->wf_rate, 16, len);
	BGBMID_StoreWAV("btactst0_se2.wav", sbuf3, 2, samp->wf_rate, 16, len);

	for(i=0; i<len; i++)
	{
		j=sbuf3[i*2+0];
		k=sbuf3[i*2+1];
		sbuf3[i*2+0]=(j+k)>>1;
		sbuf3[i*2+1]=j-((j+k)>>1);
	}

	BGBMID_StoreWAV("btactst0_se3.wav", sbuf3, 2, samp->wf_rate, 16, len);
	
	BtPak_ImageStoreFile(img, "btac0.dat", bbuf, blks*bsz);
	BtPak_ImageCommit(img);
#endif

}
