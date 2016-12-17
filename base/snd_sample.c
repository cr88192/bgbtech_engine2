#include <bteifgl.h>

BGBDT_SndSampler *bgbdt_snd_samples=NULL;
int bgbdt_snd_sampseq=1;

u32 BGBDT_Snd_HashName(char *name)
{
	return(BGBDT_MM_QHashName(name));
}

BGBDT_SndSampler *BGBDT_Snd_LookupSampler(char *name)
{
	BGBDT_SndSampler *cur;
	
	cur=bgbdt_snd_samples;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BGBDT_SndSampler *BGBDT_Snd_GetSampler(char *name)
{
	BGBDT_SndSampler *cur;

	cur=BGBDT_Snd_LookupSampler(name);
	if(cur)return(cur);
	
	cur=dtmAlloc("bgbdt_sndsample_t", sizeof(BGBDT_SndSampler));
	cur->name=frgl_strdup(name);
	cur->nhash=BGBDT_Snd_HashName(name);
	cur->sampid=bgbdt_snd_sampseq++;
	cur->next=bgbdt_snd_samples;
	bgbdt_snd_samples=cur;
	return(cur);
}

byte *BGBDT_Snd_RiffReadU32(byte *cs, u32 *rv)
{
	*rv=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24);
	return(cs+4);
}

byte *BGBDT_Snd_RiffReadS32(byte *cs, u32 *rv)
{
	*rv=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24);
	return(cs+4);
}

byte *BGBDT_Snd_RiffReadFcc(byte *cs, u32 *rv)
{
	*rv=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24);
	return(cs+4);
}

int bgbdt_log2up(int x)
{
	int i, j;
	
	i=x; j=0;
	while(i>1)
		{ i=(i+1)>>1; j++; }
	return(j);
}

BTEIFGL_API BGBDT_SndSampler *BGBDT_Snd_GetLoadSampler(char *name)
{
	BGBDT_SndSampler *cur;
	char tb[1024];
	
	sprintf(tb, "%s.wav", name);
	cur=BGBDT_Snd_GetLoadSamplerWAV(tb);
	if(cur)return(cur);
	
	printf("BGBDT_Snd_GetLoadSampler: Failed Load %s\n", name);
	return(NULL);
}

BTEIFGL_API BGBDT_SndSampler *BGBDT_Snd_GetLoadSamplerWAV(char *name)
{
	BGBDT_SndSampler *cur;
	byte *buf, *cs, *cs2, *cse, *css;
	u32 fcc1, fcc2, tsz;
	int szb;
	int i, j;

	cur=BGBDT_Snd_LookupSampler(name);
	if(cur)
		return(cur);
	
	buf=vf_loadfile(name, &szb);
	if(!buf)
	{
//		printf("BGBDT_Snd_GetLoadSampler: Failed Open %s\n", name);
		return(NULL);
	}
	
	css=buf;
	cs=buf;
	cs=BGBDT_Snd_RiffReadFcc(cs, &fcc1);
	cs=BGBDT_Snd_RiffReadU32(cs, &tsz);
	cs=BGBDT_Snd_RiffReadFcc(cs, &fcc2);

	if((fcc1!=BGBDT_SNDFCC_RIFF) ||
		(fcc2!=BGBDT_SNDFCC_WAVE))
	{
		printf("BGBDT_Snd_GetLoadSampler: Not WAV %s\n", name);

		vf_freefdbuf(buf);
		return(NULL);
	}

	cur=BGBDT_Snd_GetSampler(name);
	cur->tmp_lb=-9999;

	cse=(cs-4)+((tsz+1)&(~1));
//	cse=cs+((tsz+1)&(~1));
	while(cs<cse)
	{
		cs=BGBDT_Snd_RiffReadFcc(cs, &fcc1);
		if(fcc1==0)break;
		cs=BGBDT_Snd_RiffReadU32(cs, &tsz);
		cs2=cs+((tsz+1)&(~1));
		if((cs2>cse) || (cs2<css))
		{
//			__asm { int 3 }
			FRGL_DBGBREAK
			break;
		}
		
		if(fcc1==BGBDT_SNDFCC_fmt)
		{
			cur->wf_type=cs[0]+(cs[1]<<8);
			cur->wf_chan=cs[2]+(cs[3]<<8);
			cur->wf_rate=cs[4]+(cs[5]<<8)+(cs[6]<<16)+(cs[7]<<24);
			cur->wf_bytesec=cs[8]+(cs[9]<<8)+(cs[10]<<16)+(cs[11]<<24);
			cur->wf_blkalign=cs[12]+(cs[13]<<8);
			cur->wf_bits=cs[14];

#if 0
			printf("BGBDT_Snd_GetLoadSampler: %s (SzFmt=%d):\n"
				"\tTy=%d Ch=%d Rt=%d Bits=%d, B/s=%d Bsz=%d\n", name, tsz,
				cur->wf_type, cur->wf_chan, cur->wf_rate, cur->wf_bits,
				cur->wf_bytesec, cur->wf_blkalign);

			if(tsz>16)
			{
				for(i=16; i<tsz; i++)
					printf("%02X ", cs[i]);
				printf("\n");
			}
#endif

			cs=cs2;
			continue;
		}

		if(fcc1==BGBDT_SNDFCC_data)
		{
			cur->data=frgl_malloc(tsz);
			cur->szdata=tsz;
			memcpy(cur->data, cs, tsz);
			cs=cs2;
			continue;
		}

		/* unhandled tag */
		cs=cs2;
		continue;
	}

	if(cur->wf_type==BGBDT_SNDTY_PCM)
	{
		cur->GetSampleMonoFqClz=BGBDT_Snd_GetSampleMonoFqClz;
		cur->GetSampleMonoFqMod=BGBDT_Snd_GetSampleMonoFqMod;
		cur->GetSampleMonoMod=BGBDT_Snd_GetSampleMonoMod_Gen;
		cur->GetSampleMultiMod=BGBDT_Snd_GetSampleMultiMod_GenM;
		cur->sbrate=cur->wf_rate;

		if(cur->wf_bits==8)
		{
			if(cur->wf_chan==1)
			{
				cur->len=cur->szdata;
				cur->rcplen=(u32)(4294967296LL/cur->len);
				cur->GetSampleMono=BGBDT_Snd_GetSampleMono_PCM8M;
			}else if(cur->wf_chan==2)
			{
				cur->len=cur->szdata/2;
				cur->rcplen=(u32)(4294967296LL/cur->len);
				cur->GetSampleMono=BGBDT_Snd_GetSampleMono_PCM8S;
				cur->GetSampleMultiMod=BGBDT_Snd_GetSampleMultiMod_PCM8S;
			}
		}else if(cur->wf_bits==16)
		{
			if(cur->wf_chan==1)
			{
				cur->len=cur->szdata/2;
				cur->rcplen=(u32)(4294967296LL/cur->len);
				cur->GetSampleMono=BGBDT_Snd_GetSampleMono_PCM16M;
			}else if(cur->wf_chan==2)
			{
				cur->len=cur->szdata/4;
				cur->rcplen=(u32)(4294967296LL/cur->len);
				cur->GetSampleMono=BGBDT_Snd_GetSampleMono_PCM16S;
				cur->GetSampleMultiMod=BGBDT_Snd_GetSampleMultiMod_PCM16S;
			}
		}
		cur->GetSampleMonoRaw=cur->GetSampleMono;
	}else if(cur->wf_type==BGBDT_SNDTY_IMAADPCM)
	{
		cur->GetSampleMonoFqClz=BGBDT_Snd_GetSampleMonoFqClz;
		cur->GetSampleMonoFqMod=BGBDT_Snd_GetSampleMonoFqMod;
		cur->GetSampleMonoMod=BGBDT_Snd_GetSampleMonoMod_Gen;
		cur->GetSampleMultiMod=BGBDT_Snd_GetSampleMultiMod_GenM;
		cur->sbrate=cur->wf_rate;
		
		if(cur->wf_chan==1)
		{
			cur->sblksz=BGBDT_MsImaAdpcm_MonoSamplesFromBlockSize(
				cur->wf_blkalign);
			cur->cblg2=bgbdt_log2up(cur->sblksz);
			cur->len=cur->sblksz*(cur->szdata/cur->wf_blkalign);
			cur->cblg2b=cur->cblg2;

			cur->rcpsbsz=(u32)(4294967296LL/cur->sblksz);
			cur->rcplen=(u32)(4294967296LL/cur->len);

//			printf("\tSamples/Block=%d, Len=%d\n", cur->sblksz, cur->len);
			cur->GetSampleMono=BGBDT_Snd_GetSampleMono_MADPCM;
			cur->GetSampleMonoRaw=cur->GetSampleMono;
			
			if((cur->sblksz+8)==(1<<cur->cblg2))
			{
				cur->GetSampleMono=BGBDT_Snd_GetSampleMono_MADPCM_2N8;
				cur->GetSampleMonoMod=BGBDT_Snd_GetSampleMonoMod_MADPCM_2N8;
				cur->GetSampleMonoRaw=BGBDT_Snd_GetSampleMonoI_MADPCM_2N8;
				cur->len=((cur->szdata/cur->wf_blkalign)<<cur->cblg2);
				cur->rcplen=(u32)(4294967296LL/cur->len);
//				cur->sbrate=(cur->wf_rate*(1<<cur->cblg2))/cur->sblksz;
//				printf("\tSpecial Case: (2^N)-8, SbRate=%d\n", cur->sbrate);
			}
		}else if(cur->wf_chan==2)
		{
			cur->sblksz=BGBDT_MsImaAdpcm_StereoSamplesFromBlockSize(
				cur->wf_blkalign);
			cur->cblg2=bgbdt_log2up(cur->sblksz);
			cur->len=cur->sblksz*(cur->szdata/cur->wf_blkalign);
			cur->cblg2b=cur->cblg2+1;

			cur->rcpsbsz=(u32)(4294967296LL/cur->sblksz);
			cur->rcplen=(u32)(4294967296LL/cur->len);

//			printf("\tSamples/Block=%d\n", cur->sblksz);
			cur->GetSampleMono=BGBDT_Snd_GetSampleMono_SADPCM;
			cur->GetSampleMonoRaw=cur->GetSampleMono;

			if((cur->sblksz+8)==(1<<cur->cblg2))
			{
				cur->GetSampleMono=BGBDT_Snd_GetSampleMono_SADPCM_2N8;
				cur->len=((cur->szdata/cur->wf_blkalign)<<cur->cblg2);
				cur->rcplen=(u32)(4294967296LL/cur->len);
//				cur->sbrate=(cur->wf_rate*(1<<cur->cblg2))/cur->sblksz;
//				printf("\tSpecial Case: (2^N)-8, SbRate=%d\n", cur->sbrate);
			}
		}
	}else if(cur->wf_type==BGBDT_SNDTY_BTAC1C)
	{
		cur->GetSampleMonoFqClz=BGBDT_Snd_GetSampleMonoFqClz;
		cur->GetSampleMonoFqMod=BGBDT_Snd_GetSampleMonoFqMod;
		cur->GetSampleMonoMod=BGBDT_Snd_GetSampleMonoMod_Gen;
		cur->GetSampleMultiMod=BGBDT_Snd_GetSampleMultiMod_GenM;
		cur->sbrate=cur->wf_rate;
		
		if(cur->wf_chan==1)
		{
			cur->sblksz=BGBDT_MsImaAdpcm_MonoSamplesFromBlockSize(
				cur->wf_blkalign);
			cur->cblg2=bgbdt_log2up(cur->sblksz);
			cur->cblg2b=cur->cblg2;
			cur->len=cur->sblksz*(cur->szdata/cur->wf_blkalign);
			cur->rcpsbsz=(u32)(4294967296LL/cur->sblksz);
			cur->GetSampleMono=BGBDT_Snd_GetSampleMono_MADPCM;
			cur->GetSampleMonoRaw=cur->GetSampleMono;
			cur->rcplen=(u32)(4294967296LL/cur->len);
			
			if((cur->sblksz+8)==(1<<cur->cblg2))
			{
				cur->GetSampleMono=BGBDT_Snd_GetSampleMono_MADPCM_2N8;
				cur->GetSampleMonoMod=BGBDT_Snd_GetSampleMonoMod_MADPCM_2N8;
				cur->GetSampleMonoRaw=BGBDT_Snd_GetSampleMonoI_MADPCM_2N8;
				cur->len=((cur->szdata/cur->wf_blkalign)<<cur->cblg2);
				cur->rcplen=(u32)(4294967296LL/cur->len);
			}
		}else if(cur->wf_chan==2)
		{
			cur->sblksz=BGBDT_MsImaAdpcm_StereoSamplesFromBlockSize(
				cur->wf_blkalign);
			cur->cblg2=bgbdt_log2up(cur->sblksz);
			cur->cblg2b=cur->cblg2+1;
			cur->len=cur->sblksz*(cur->szdata/cur->wf_blkalign);

			cur->rcpsbsz=(u32)(4294967296LL/cur->sblksz);
			cur->GetSampleMono=BGBDT_Snd_GetSampleMono_SADPCM_2N16;
			cur->GetSampleMonoRaw=cur->GetSampleMono;
			cur->len=((cur->szdata/cur->wf_blkalign)<<cur->cblg2);
			cur->rcplen=(u32)(4294967296LL/cur->len);
		}
	}
	
	frgl_free(buf);
	
	return(cur);
}


int BGBDT_Snd_GetSampleMonoFqClz(BGBDT_SndSampler *samp, int fq, int pos)
{
	double sc, pos2, pf;
	double s0, s1, s2, s3;
	double t0, t1, t2;
	int pb;
	
//	sc=((double)samp->wf_rate)/(fq*0.0625);
	sc=((double)samp->sbrate)/(fq*0.0625);
	pos2=pos*sc*0.0625;
	
	pb=(int)pos2;
	s0=samp->GetSampleMono(samp, pb-1);
	s1=samp->GetSampleMono(samp, pb+0);
	s2=samp->GetSampleMono(samp, pb+1);
	s3=samp->GetSampleMono(samp, pb+2);
	pf=pos2-pb;

	t0=(s0*(1-(pf+1)))+(s1*(pf+1));
	t1=(s2*(1-(pf-1)))+(s3*(pf-1));
	t2=(t0*(1-pf))+(t1*pf);
	return(t2);
}

int BGBDT_Snd_SampleModSample(BGBDT_SndSampler *samp, int pos)
{
	int i, j;

	i=pos;
	if(i<0)
	{
#if 1
		i=-i;
		j=(i*((s64)samp->rcplen))>>32;
		i-=j*samp->len;
//		if(i>samp->len)
//			i-=samp->len;
		i-=i;
#endif

		while(i<0)
			i+=samp->len;
	}else if(i>samp->len)
	{
		j=(i*((s64)samp->rcplen))>>32;
		i-=j*samp->len;
		if(i>samp->len)
			i-=samp->len;
	
#if 0
		if((i>>3)>samp->len)
		{
//			return(i%samp->len);

//			for(j=12; j>1; j--)
//				if((i>>j)>samp->len)
//					i-=samp->len<<j;

//			while((i>>8)>samp->len)
//				i-=samp->len<<8;
			while((i>>4)>samp->len)
				i-=samp->len<<4;
			while((i>>2)>samp->len)
				i-=samp->len<<2;
		}

		while(i>samp->len)
			i-=samp->len;
#endif
	}
	return(i);
}

int BGBDT_Snd_GetSampleMonoFqMod(BGBDT_SndSampler *samp, int fq, int pos)
{
	double sc, pos2, pf;
	double s0, s1, s2, s3;
	double t0, t1, t2;
	int pb;
	
//	sc=((double)samp->wf_rate)/(fq*0.0625);
	sc=((double)samp->sbrate)/(fq*0.0625);
	pos2=pos*sc*0.0625;
	
	pb=(int)pos2;
	s0=samp->GetSampleMonoMod(samp, pb-1);
	s1=samp->GetSampleMonoMod(samp, pb+0);
	s2=samp->GetSampleMonoMod(samp, pb+1);
	s3=samp->GetSampleMonoMod(samp, pb+2);
	pf=pos2-pb;

	t0=(s0*(1-(pf+1)))+(s1*(pf+1));
	t1=(s2*(1-(pf-1)))+(s3*(pf-1));
	t2=(t0*(1-pf))+(t1*pf);
	return(t2);
}

int BGBDT_Snd_GetSampleMonoMod_Gen(BGBDT_SndSampler *samp, int pos)
{
	int i, j;
	i=BGBDT_Snd_SampleModSample(samp, pos);
	j=samp->GetSampleMono(samp, i);
	return(j);
}

int BGBDT_Snd_GetSampleMultiMod_GenM(BGBDT_SndSampler *samp,
	int pos, int ch)
{
	int i, j;
	i=BGBDT_Snd_SampleModSample(samp, pos);
	j=samp->GetSampleMono(samp, i);
	return(j);
}

int BGBDT_Snd_GetSampleMono_PCM8M(BGBDT_SndSampler *samp, int pos)
{
	int i, j, k;
	i=pos;
	if((i<0) || (i>=samp->len))
		return(0);
	j=samp->data[i];
	k=(j*256)-32768;
	return(k);
}

int BGBDT_Snd_GetSampleMono_PCM8S(BGBDT_SndSampler *samp, int pos)
{
	int i, j, k;
	i=pos;
	if((i<0) || (i>=samp->len))
		return(0);
	j=(samp->data[i*2+0]+samp->data[i*2+1])>>1;
	k=(j*256)-32768;
	return(k);
}

int BGBDT_Snd_GetSampleMultiMod_PCM8S(BGBDT_SndSampler *samp,
	int pos, int ch)
{
	int i, j, k;
	i=BGBDT_Snd_SampleModSample(samp, pos);
	j=samp->data[i*2+(ch&1)];
	k=(j*256)-32768;
	return(k);
}

int BGBDT_Snd_GetSampleMono_PCM16M(BGBDT_SndSampler *samp, int pos)
{
	int i, j, k;
	i=pos;
	if((i<0) || (i>=samp->len))
		return(0);
	j=(s16)(samp->data[i*2+0]|(samp->data[i*2+1]<<8));
	return(j);
}

int BGBDT_Snd_GetSampleMono_PCM16S(BGBDT_SndSampler *samp, int pos)
{
	int i, j, k;
	i=pos;
	if((i<0) || (i>=samp->len))
		return(0);
	j=(s16)(samp->data[i*4+0]|(samp->data[i*4+1]<<8));
	k=(s16)(samp->data[i*4+2]|(samp->data[i*4+3]<<8));
	return((j+k)>>1);
}

int BGBDT_Snd_GetSampleMultiMod_PCM16S(BGBDT_SndSampler *samp,
	int pos, int ch)
{
	int i, j, k;
	i=BGBDT_Snd_SampleModSample(samp, pos);

	if(ch&1)
		{ k=(s16)(samp->data[i*4+2]|(samp->data[i*4+3]<<8)); }
	else
		{ k=(s16)(samp->data[i*4+0]|(samp->data[i*4+1]<<8)); }
	return(k);
}


s16 *bgbdt_snd_blkcache_data[16];
u32 *bgbdt_snd_blkcache_key[16];
BGBDT_SndSampler **bgbdt_snd_blkcache_samp[16];

int BGBDT_Snd_GetCacheCheckKey(
	BGBDT_SndSampler *samp, int blkid, s16 **rbuf)
{
	u32 bk;
	u32 hk, hi;
	int lg2, rt;
	
	lg2=samp->cblg2b;
	if(!bgbdt_snd_blkcache_data[lg2])
	{
		bgbdt_snd_blkcache_data[lg2]=frgl_malloc((1024<<lg2)*2);
		bgbdt_snd_blkcache_key[lg2]=frgl_malloc(1024*sizeof(u32));
		bgbdt_snd_blkcache_samp[lg2]=frgl_malloc(
			1024*sizeof(BGBDT_SndSampler *));
	}
	
//	bk=(((u64)samp->nhash)<<32)|((u32)blkid);
	bk=(samp->sampid<<20)^blkid;
	
	hk=(samp->nhash*251+blkid)*251;
	hi=(hk>>8)&1023;
//	if(rbuf)
//		{ *rbuf=bgbdt_snd_blkcache_data[lg2]+(hi<<lg2); }
//	rt=(bgbdt_snd_blkcache_key[lg2][hi]==hk);
//	bgbdt_snd_blkcache_key[lg2][hi]=hk;

	if(bgbdt_snd_blkcache_samp[lg2][hi])
	{
		bgbdt_snd_blkcache_samp[lg2][hi]->tmp_lb=-999999;
	}

	*rbuf=bgbdt_snd_blkcache_data[lg2]+(hi<<lg2);
	rt=(bgbdt_snd_blkcache_key[lg2][hi]==bk);
	bgbdt_snd_blkcache_key[lg2][hi]=bk;
	bgbdt_snd_blkcache_samp[lg2][hi]=samp;

	return(rt);
}


int BGBDT_Snd_GetSampleMono_MADPCM(BGBDT_SndSampler *samp, int pos)
{
	s16 *sb;
	int b, r;
	int i, j, k;
	i=pos;
	if((i<0) || (i>=samp->len))
		return(0);
	
//	b=(pos*((s64)cur->rcpsbsz)+2147483647LL)>>32;
//	b=(pos*((s64)cur->rcpsbsz)+0x400000)>>32;
		
//	b=pos/samp->sblksz;
//	r=pos-(b*samp->sblksz);

	b=(pos*((s64)samp->rcpsbsz))>>32;
	r=pos-(b*samp->sblksz);
	if(r>=samp->sblksz)
		{ b++; r-=samp->sblksz; }
	
	i=BGBDT_Snd_GetCacheCheckKey(samp, b, &sb);
	if(i>0)
		{ return(sb[r]); }
	
	if(i==0)
	{
//		BGBDT_MsImaAdpcm_DecodeBlockMono(
//		BGBDT_SndBTAC1C_DecodeBlockMono(
//			samp->data+(b*samp->wf_blkalign),
//			sb, samp->sblksz);
		BGBDT_SndBTAC1C2_DecodeBlockMono(
			samp->data+(b*samp->wf_blkalign),
			sb, samp->sblksz, &(samp->idxst));
		return(sb[r]);
	}
	
	return(0);
//	j=samp->data[i];
//	k=(j*256)-32768;
//	return(k);
}

int BGBDT_Snd_GetSampleMono_SADPCM(BGBDT_SndSampler *samp, int pos)
{
	s16 *sb;
	int b, r;
	int i, j, k;
	i=pos;
	if((i<0) || (i>=samp->len))
		return(0);
		
//	b=pos/samp->sblksz;
//	b=(pos*((s64)cur->rcpsbsz+4194304))>>32;
//	r=pos-(b*samp->sblksz);

	b=(pos*((s64)samp->rcpsbsz))>>32;
	r=pos-(b*samp->sblksz);
	if(r>=samp->sblksz)
		{ b++; r-=samp->sblksz; }

	i=BGBDT_Snd_GetCacheCheckKey(samp, b, &sb);
	if(i>0)
		{ return((sb[r*2+0]+sb[r*2+1])>>1); }
	
	if(i==0)
	{
		BGBDT_MsImaAdpcm_DecodeBlockStereo(
			samp->data+(b*samp->wf_blkalign),
			sb, samp->sblksz);
		return((sb[r*2+0]+sb[r*2+1])>>1);
	}
	
	return(0);
	
//	j=samp->data[i];
//	k=(j*256)-32768;
//	return(k);
}

int BGBDT_Snd_GetSampleMonoI_MADPCM_2N8(BGBDT_SndSampler *samp, int pos)
{
	s16 *sb;
	int b, r;
	int i, j, k, l;
//	i=pos;
//	if((i<0) || (i>=samp->len))
//		return(0);
		
	l=1<<samp->cblg2;
	b=pos>>samp->cblg2;
	r=pos&(l-1);
	
	if(b==samp->tmp_lb)
		{ return(samp->tmp_sb[r]); }
	
	i=BGBDT_Snd_GetCacheCheckKey(samp, b, &sb);
	if(i>0)
	{
		samp->tmp_lb=b;
		samp->tmp_sb=sb;
		return(sb[r]);
	}
	
	if(i==0)
	{
#if 0
//		BGBDT_MsImaAdpcm_DecodeBlockMono(
		BGBDT_SndBTAC1C_DecodeBlockMono(
			samp->data+(b*samp->wf_blkalign),
			sb, samp->sblksz);
		
		/* skew resample */
		for(j=l-1; j>0; j--)
		{
			k=(j+1)>>(samp->cblg2-3);
			sb[j]=sb[j-k];
		}
#endif

//		BGBDT_SndBTAC1C_DecodeBlockMonoLg2(
//			samp->data+(b*samp->wf_blkalign),
//			sb, samp->cblg2);
		BGBDT_SndBTAC1C2_DecodeBlockMonoLg2(
			samp->data+(b*samp->wf_blkalign),
			sb, samp->cblg2, &(samp->idxst));
		
		return(sb[r]);
	}
	
	return(0);
}

int BGBDT_Snd_GetSampleMono_MADPCM_2N8(BGBDT_SndSampler *samp, int pos)
{
	if((pos<0) || (pos>=samp->len))
		return(0);
	return(BGBDT_Snd_GetSampleMonoI_MADPCM_2N8(samp, pos));
}

int BGBDT_Snd_GetSampleMonoMod_MADPCM_2N8(BGBDT_SndSampler *samp, int pos)
{
	int i, j;
	i=BGBDT_Snd_SampleModSample(samp, pos);
	return(BGBDT_Snd_GetSampleMonoI_MADPCM_2N8(samp, i));
//	j=samp->GetSampleMono(samp, i);
//	return(j);
}

int BGBDT_Snd_GetSampleMono_SADPCM_2N8(BGBDT_SndSampler *samp, int pos)
{
	s16 *sb;
	int b, r;
	int i, j, k, l;
	i=pos;
	if((i<0) || (i>=samp->len))
		return(0);
		
	l=1<<samp->cblg2;
	b=pos>>samp->cblg2;
	r=pos&(l-1);

	i=BGBDT_Snd_GetCacheCheckKey(samp, b, &sb);
	if(i>0)
		{ return((sb[r*2+0]+sb[r*2+1])>>1); }
	
	if(i==0)
	{
		BGBDT_MsImaAdpcm_DecodeBlockStereo(
			samp->data+(b*samp->wf_blkalign),
			sb, samp->sblksz);

		/* skew resample */
		for(j=l-1; j>0; j--)
		{
			k=(j+1)>>(samp->cblg2-3);
			sb[j*2+0]=sb[(j-k)*2+0];
			sb[j*2+1]=sb[(j-k)*2+1];
		}

		return((sb[r*2+0]+sb[r*2+1])>>1);
	}
	
	return(0);
}

int BGBDT_Snd_GetSampleMono_SADPCM_2N16(BGBDT_SndSampler *samp, int pos)
{
	s16 *sb;
	int b, r;
	int i, j, k, l;
	i=pos;
	if((i<0) || (i>=samp->len))
		return(0);
		
	l=1<<samp->cblg2;
	b=pos>>samp->cblg2;
	r=pos&(l-1);

	i=BGBDT_Snd_GetCacheCheckKey(samp, b, &sb);
	if(i>0)
		{ return((sb[r*2+0]+sb[r*2+1])>>1); }
	
	if(i==0)
	{
//		BGBDT_SndBTAC1C_DecodeBlockStereoLg2(
//			samp->data+(b*samp->wf_blkalign),
//			sb, samp->cblg2);
		BGBDT_SndBTAC1C2_DecodeBlockStereoLg2(
			samp->data+(b*samp->wf_blkalign),
			sb, samp->cblg2, &(samp->idxst));
		return((sb[r*2+0]+sb[r*2+1])>>1);
	}
	
	return(0);
}

BTEIFGL_API int BGBDT_Snd_GetSampleMono(BGBDT_SndSampler *samp, int pos)
{
	return(samp->GetSampleMono(samp, pos));
}

BTEIFGL_API int BGBDT_Snd_GetSampleMonoMod(BGBDT_SndSampler *samp, int pos)
{
	return(samp->GetSampleMonoMod(samp, pos));
}