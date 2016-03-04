#include <stdio.h>

#include <bteifgl.h>

#ifdef _WIN32

#include <windows.h>
#include <mmsystem.h>


#define	WAV_BUFFERS				16
#define	WAV_MASK				(WAV_BUFFERS-1)
#define	WAV_BUFFER_SIZE			0x1000
#define SECONDARY_BUFFER_SIZE	0x10000

HWAVEOUT    hWaveOut; 
DWORD	gSndBufSize;

HANDLE		hData;
HPSTR		lpData, lpData2;

HGLOBAL		hWaveHdr;
LPWAVEHDR	lpWaveHdr;

static int	sample16, samples;
static int	snd_sent, snd_completed;
static short	*snd_rover;

BTEIFGL_API int SoundDev_DeInit()
{
	int i;

	waveOutReset(hWaveOut);

	if(lpWaveHdr)
	{
		for(i=0; i<WAV_BUFFERS; i++)
			waveOutUnprepareHeader(hWaveOut, lpWaveHdr+i, sizeof(WAVEHDR));
	}

	waveOutClose(hWaveOut);

	if(hWaveHdr)
	{
		GlobalUnlock(hWaveHdr); 
		GlobalFree(hWaveHdr);
	}

	if(hData)
	{
		GlobalUnlock(hData);
		GlobalFree(hData);
	}

	return(0);
}

BTEIFGL_API int SoundDev_Init()
{
	WAVEFORMATEX format; 
	int i, j;
	HRESULT hr;
	short *buf;

	snd_sent=0;
	snd_completed=0;

	memset(&format, 0, sizeof(format));
	format.wFormatTag=WAVE_FORMAT_PCM;
	format.nChannels=1;
	format.wBitsPerSample=16;
	format.nSamplesPerSec=44100;
	format.nBlockAlign=format.nChannels*format.wBitsPerSample/8;
	format.cbSize=0;
	format.nAvgBytesPerSec=format.nSamplesPerSec*format.nBlockAlign; 
	
	/* Open a waveform device for output using window callback. */ 
	while((hr=waveOutOpen((LPHWAVEOUT)&hWaveOut, WAVE_MAPPER, 
		&format, 0, 0L, CALLBACK_NULL))!=MMSYSERR_NOERROR)
	{
		printf("waveOutOpen failed\n");
		return(-1);
	} 

	gSndBufSize=WAV_BUFFERS*WAV_BUFFER_SIZE;
	hData=GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, gSndBufSize); 
	if(!hData) 
	{ 
		printf("Sound: Out of memory.\n");
		SoundDev_DeInit();
		return(-1); 
	}
	lpData=GlobalLock(hData);
	if(!lpData)
	{ 
		printf("Sound: Failed to lock.\n");
		SoundDev_DeInit();
		return(-1); 
	} 
	memset(lpData, 0, gSndBufSize);

	hWaveHdr=GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, 
		(DWORD) sizeof(WAVEHDR)*WAV_BUFFERS); 

	if(hWaveHdr==NULL)
	{ 
		printf("Sound: Failed to Alloc header.\n");
		SoundDev_DeInit();
		return(-1); 
	} 

	lpWaveHdr =(LPWAVEHDR) GlobalLock(hWaveHdr); 

	if(lpWaveHdr==NULL)
	{ 
		printf("Sound: Failed to lock header.\n");
		SoundDev_DeInit();
		return(-1); 
	}

	memset(lpWaveHdr, 0, sizeof(WAVEHDR)*WAV_BUFFERS);

	/* After allocation, set up and prepare headers. */ 
	for(i=0; i<WAV_BUFFERS; i++)
	{
		lpWaveHdr[i].dwBufferLength=WAV_BUFFER_SIZE; 
		lpWaveHdr[i].lpData=lpData+i*WAV_BUFFER_SIZE;

		if(waveOutPrepareHeader(hWaveOut, lpWaveHdr+i, sizeof(WAVEHDR)) !=
				MMSYSERR_NOERROR)
		{
			printf("Sound: failed to prepare wave headers\n");
			SoundDev_DeInit();
			return(-1);
		}
	}

	samples=gSndBufSize/(16/8);
	sample16 =(16/8)-1;

	return(0);
}

BTEIFGL_API int SoundDev_GetDMAPos()
{
	MMTIME tm;
	int i, s;

	s=snd_sent*WAV_BUFFER_SIZE;
	s>>=sample16;
	s&=(samples-1);

	return(s);
}

BTEIFGL_API void SoundDev_Submit()
{
	LPWAVEHDR	h;
	int			wResult;

	while(1)
	{
		if(snd_completed==snd_sent)
		{
//			printf("Sound overrun\n");
			break;
		}

		if(!(lpWaveHdr[snd_completed&WAV_MASK].dwFlags&WHDR_DONE))
			break;

		snd_completed++;	// this buffer has been played
	}

	while(((snd_sent-snd_completed)>>sample16)<4)
	{
		h=lpWaveHdr+(snd_sent&WAV_MASK);

		snd_sent++;

		wResult=waveOutWrite(hWaveOut, h, sizeof(WAVEHDR)); 

		if(wResult!=MMSYSERR_NOERROR)
		{ 
			printf("Failed to write block to device\n");
			SoundDev_DeInit();
			return; 
		} 
	}
}

BTEIFGL_API int SoundDev_PaintSamples(short *buffer, int cnt)
{
	int l;
	short *buf, *rover;

//	printf("Writing %d samples\n", cnt);

	buf=(short *)lpData;
	rover=snd_rover;

	while(((rover-buf)+cnt)>=samples)
	{
		l=samples-(rover-buf);
		memcpy(rover, buffer, l*sizeof(short));
		rover=buf;

		buffer+=l;
		cnt-=l;
	}
	if(cnt>0)memcpy(rover, buffer, cnt*sizeof(short));

	return(0);
}

BTEIFGL_API int SoundDev_WriteSamples(short *buffer, int cnt)
{
	int i, l;
	short *buf, *dma;
	static short *ldma;

//	printf("Writing %d samples\n", cnt);

	buf=(short *)lpData;
	dma=buf+SoundDev_GetDMAPos();

	if(!snd_rover)snd_rover=dma;

	if(abs(snd_rover-dma)>(44100/10) && (dma>ldma))
	{
//		printf("Sound sync %d\n", snd_rover-dma);
		snd_rover=dma;
	}
	ldma=dma;

	SoundDev_PaintSamples(buffer, cnt);

	snd_rover+=cnt;
	while((snd_rover-buf)>=samples)
		snd_rover-=samples;

	SoundDev_PaintSamples(buffer, cnt);

	SoundDev_Submit();

	return(0);
}

#endif

