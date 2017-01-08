#ifdef __EMSCRIPTEN__

BTEIFGL_API int SoundDev_DeInit()
{
	return(0);
}

BTEIFGL_API int SoundDev_Init()
{
	return(0);
}

BTEIFGL_API int SoundDev_WriteRawSamples(short *buffer,
	int cnt, int ovcnt)
{
	return(0);
}

BTEIFGL_API int SoundDev_WriteStereoSamples(short *buffer, int cnt)
{
	return(SoundDev_WriteRawSamples(buffer, cnt*2, cnt*2));
}

BTEIFGL_API int SoundDev_WriteStereoSamples2(
	short *buffer, int cnt, int ovcnt)
{
	return(SoundDev_WriteRawSamples(buffer, cnt*2, ovcnt*2));
}

BTEIFGL_API int SoundDev_WriteSamples(short *buffer, int cnt)
{
	static short *tbuf=NULL;
	static int tbcnt=0;
	int i;
	
	if(!tbuf)
	{
		i=16384;
		while(cnt>i)i=i+(i>>1);
		tbuf=frgl_malloc(i*2*sizeof(short));
		tbcnt=i;
	}
	
	if(cnt>tbcnt)
	{
		i=tbcnt;
		while(cnt>i)i=i+(i>>1);
		tbuf=frgl_realloc(tbuf, i*2*sizeof(short));
		tbcnt=i;
	}
	
	for(i=0; i<cnt; i++)
	{
		tbuf[i*2+0]=buffer[i];
		tbuf[i*2+1]=buffer[i];
	}
	
	i=SoundDev_WriteRawSamples(tbuf, cnt*2, cnt*2);
	return(i);
}

#endif
