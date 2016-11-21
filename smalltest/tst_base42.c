#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#if defined(WIN64) || (_M_IX86_FP>=2)
#include <emmintrin.h>
#endif


char *base42_enc=
	"BCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";
char *base64_enc=
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

char base42_dec[64]={
//@  A  B  C  D  E  F  G   H  I  J  K  L  M  N  O  (40-4F)
 -1,-1, 0, 1, 2,-1, 3, 4,  5,-1, 6, 7, 8, 9,10,-1,
//P  Q  R  S  T  U  V  W   X  Y  Z  [  \  ]  ^  _  (50-5F)
 11,12,13,14,15,-1,16,17, 18,19,20,-1,-1,-1,-1,-1,
//`  a  b  c  d  e  f  g   h  i  j  k  l  m  n  o  (60-6F)
 -1,-1,21,22,23,-1,24,25, 26,-1,27,28,29,30,31,-1,
//p  q  r  s  t  u  v  w   x  y  z  {  |  }  ~  ~  (70-7F)
 32,33,34,35,36,-1,37,38, 39,40,41,-1,-1,-1,-1,-1,
};

int Base42_EncUnit(int iv, char *ov)
{
	unsigned int v0, v1, v2;
	unsigned int w0, w1, w2;
	
//	v2=iv%42;
//	v1=(iv/42)%42;
//	v0=iv/1764;

#if 1
	v2=iv;
	v1=v2/42;
	v2-=v1*42;
	v0=v1/42;
	v1-=v0*42;
#endif

#if 0	
	w2=iv;
//	w1=(w2*780+328)>>15;
//	w1=(w2*780+780)>>15;
	w1=(w2*780+787)>>15;
	w2-=w1*42;
//	w0=(w1*780+328)>>15;
//	w0=(w1*780+780)>>15;
	w0=(w1*780+787)>>15;
	w1-=w0*42;
#endif

#if 0
	w2=iv;
//	w1=(w2*1560+1576)>>16;
	w1=w2/42;
	w2-=w1*42;
//	w0=(w1*1560+1576)>>16;
	w0=w1/42;
	w1-=w0*42;
#endif

#if 0
	if((w2!=v2) || (w1!=v1) || (w0!=v0))
	{
		printf("%04X: %d=%d %d=%d %d=%d\n", iv, v0, w0, v1, w1, v2, w2);
		return(-1);
	}
#endif

#if 0
	v2=iv;
//	v1=(v2*780+6242)>>15;
	v1=(v2*799)>>15;
	v2-=v1*42;
//	v0=(v1*780+6242)>>15;
	v1-=v0*42;
#endif

//	if(v2<0)v2=0;
//	if(v2>41)v2=41;
	
	ov[0]=base42_enc[v0];
	ov[1]=base42_enc[v1];
	ov[2]=base42_enc[v2];
	return(0);
}

int Base42_DecUnit(char *iv)
{
	int v0, v1, v2;
	int i, j, k;
	
	v0=base42_dec[iv[0]-0x40];
	v1=base42_dec[iv[1]-0x40];
	v2=base42_dec[iv[2]-0x40];
	i=v0*1764+v1*42+v2;
	return(i);
}

int Base42_CopyFlipA(char *ibuf, char *obuf, int n)
{
	int i;
	char cr, cg, cb, ca;

	for(i=0; i<n; i++)
	{
		cr=ibuf[i*4+0]; cg=ibuf[i*4+1];
		cb=ibuf[i*4+2]; ca=ibuf[i*4+3];
		obuf[i*4+0]=cb; obuf[i*4+1]=cg;
		obuf[i*4+2]=cr; obuf[i*4+3]=ca;
	}
}

int Base42_CopyFlipB(char *ibuf, char *obuf, int n)
{
	int *cs, *ct, *cse;
	int i0, i1, i2, i3;
	
	cs=(int *)ibuf;
	cse=cs+((n+3)&(~3));
	ct=(int *)obuf;
	while(cs<cse)
	{
		i0=cs[0]; i1=cs[1];
		i2=cs[2]; i3=cs[3];
		cs+=4;
		ct[0]=(i0&0xFF00FF00)|
			((i0>>16)&0x000000FF)|
			((i0<<16)&0x00FF0000);
		ct[1]=(i1&0xFF00FF00)|
			((i1>>16)&0x000000FF)|
			((i1<<16)&0x00FF0000);
		ct[2]=(i2&0xFF00FF00)|
			((i2>>16)&0x000000FF)|
			((i2<<16)&0x00FF0000);
		ct[3]=(i3&0xFF00FF00)|
			((i3>>16)&0x000000FF)|
			((i3<<16)&0x00FF0000);
		ct+=4;
	}
}

int Base42_CopyFlipC(char *ibuf, char *obuf, int n)
{
	int *cs, *ct, *cse;
	int i0, i1, i2, i3;
	
	cs=(int *)ibuf;
	cse=cs+n;
	ct=(int *)obuf;
	while(cs<cse)
	{
		i0=*cs++;
		*ct++=(i0&0xFF00FF00)|
			((i0>>16)&0x000000FF)|
			((i0<<16)&0x00FF0000);

		i0=*cs++;
		*ct++=(i0&0xFF00FF00)|
			((i0>>16)&0x000000FF)|
			((i0<<16)&0x00FF0000);

		i0=*cs++;
		*ct++=(i0&0xFF00FF00)|
			((i0>>16)&0x000000FF)|
			((i0<<16)&0x00FF0000);

		i0=*cs++;
		*ct++=(i0&0xFF00FF00)|
			((i0>>16)&0x000000FF)|
			((i0<<16)&0x00FF0000);
	}
}

int Base42_CopyFlipD(char *ibuf, char *obuf, int n)
{
	int *cs, *ct, *cse;
	__m128i m0, m1, m2;
	__m128i x0, x1, x2, x3, x4;

	m0=_mm_set_epi32(0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00);
	m1=_mm_set_epi32(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
	m2=_mm_set_epi32(0x00FF0000, 0x00FF0000, 0x00FF0000, 0x00FF0000);
	
	cs=(int *)ibuf;
	cse=cs+n;
	ct=(int *)obuf;
	while(cs<cse)
	{
		x0=_mm_loadu_si128(cs);
		x1=_mm_and_si128(x0, m0);
		x2=_mm_and_si128(_mm_srli_epi32(x0, 16), m1);
		x3=_mm_and_si128(_mm_slli_epi32(x0, 16), m2);
		x4=_mm_or_si128(x1, _mm_or_si128(x2, x3));
		_mm_storeu_si128(ct, x4);
		cs+=4;
		ct+=4;
	}
}


int main()
{
	char *bufa, *bufb;
	char sv[64];
	int i0, i1, i2;
	int i, j, k, l;
	
	for(i=0; i<42; i++)
	{
		j=i*42;
//		k=(j*780+6242)>>15;
		i0=j*780+6242;
//		i0=j*780+780;
//		i0=j*780+328;
		k=(i0)>>15;

		i1=(j-1)*780+328;
		l=(i1)>>15;

		printf("%d %d %d %04X\n", i, k, l, i0&32767);
	}

#if 0
//	for(i=0; i<42*42; i++)
	for(i=0; i<65536; i++)
	{
		j=(i/42)%42;
//		k=(i*1560+1576)>>16;
		k=(i*780+787)>>15;
		l=(k*780+787)>>15;
		k-=l*42;
		if(j!=k)
		{
			printf("%04X: %d!=%d\n", i, j, k);
			break;
		}
	}
#endif

	memcpy(sv+16, sv, 12);
	printf("%08X\n", *(long long *)(sv+16));

#if 1
	for(i=0; i<65536; i++)
	{
		j=Base42_EncUnit(i, sv);
		if(j<0)break;
		sv[3]=0;
		j=Base42_DecUnit(sv);
		if(i!=j)
		{
			printf("%04X %04X %s\n", i, j, sv);
			break;
		}
	}
#endif

	bufa=malloc(1<<24);
	bufb=malloc(1<<24);

	i0=clock(); j=0;
	for(i=0; i<128; i++)
	{
		memcpy(bufa, bufb, 1<<24);
		j+=1<<22;
	}
	i1=clock();
	i2=i1-i0;
	printf("Mcpy: %d %f\n", i2,
		(j/1000000.0)/(i2/((double)CLOCKS_PER_SEC)));


	i0=clock(); j=0;
	for(i=0; i<128; i++)
	{
		Base42_CopyFlipA(bufa, bufb, 1<<22);
		j+=1<<22;
	}
	i1=clock();
	i2=i1-i0;
	printf("A: %d %f\n", i2, (j/1000000.0)/(i2/((double)CLOCKS_PER_SEC)));

	i0=clock(); j=0;
	for(i=0; i<128; i++)
	{
		Base42_CopyFlipB(bufa, bufb, 1<<22);
//		Base42_CopyFlipC(bufa, bufb, 1<<22);
		j+=1<<22;
	}
	i1=clock();
	i2=i1-i0;
	printf("B: %d %f\n", i2, (j/1000000.0)/(i2/((double)CLOCKS_PER_SEC)));

	i0=clock(); j=0;
	for(i=0; i<128; i++)
	{
		Base42_CopyFlipD(bufa, bufb, 1<<22);
		j+=1<<22;
	}
	i1=clock();
	i2=i1-i0;
	printf("D: %d %f\n", i2, (j/1000000.0)/(i2/((double)CLOCKS_PER_SEC)));
}
