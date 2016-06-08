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

int main()
{
	BGBDT_SndSampler *samp;
	s16 *sbuf, *sbuf2, *sbuf3;
	byte *bbuf;
	double e;
	int t0, t1, t2, ts, te, dt;
	int len, bsz, blen, blen2, blg2, blks, idx;
	int i, j, k, r;

//	samp=BGBDT_Snd_GetLoadSampler("sound_dev/pi0_amb0");
//	samp=BGBDT_Snd_GetLoadSampler("resource_dev/sound_dev/"
//		"LindseyStirling_Shadows_ADPCM_22");
	samp=BGBDT_Snd_GetLoadSampler("resource_dev/sound_dev/"
		"OwenWasHer_Touhou6_PCM16S_11");
//		"LindseyStirling_Shadows_PCM16S_22");

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
}
