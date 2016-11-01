#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char ta[4], tb[4];
	int i, j, k, bi, sc, sh;

//	bi=209;
//	bi=0; sc=2620; sh=8;
	bi=4096; sc=2580; sh=8;
	for(i=0; i<1000; i++)
	{
		sprintf(ta, "%03d", i);

		j=i;

//		if(((j>>8)&15)>=10)j+=6<<8;
//		if(((j>>4)&15)>=10)j+=6<<4;
//		if((j&15)>=10)j+=6;
//		if(((j>>4)&15)>=10)j+=6<<4;
//		if(((j>>8)&15)>=10)j+=6<<8;
//		sprintf(tb, "%03X", j);

//		j=(j*1038)>>10;
//		j=(j*1048)>>10;
//		j=(j*1047+1023)>>10;
//		j=(j*1073741+524288)>>20;

//		j=(((long long)i)*((long long)((1LL<<32)*1.024))+(1LL<<31))>>32;
//		j=(((long long)i)*((long long)((1LL<<32)*1.024))+((1LL<<32)-1))>>32;

//		j=(((long long)i)*((long long)((1LL<<20)*1.024))+((1LL<<20)-1))>>20;
		j=(i*1073741+1048575)>>20;

#if 1
//		j=j*10;
		j=j*10;
//		j+=j>>10;

//		j=j*10+bi;
//		j=(j*sc+bi)>>sh;
//		if(((j&1023)>=1000))j+=24;
		tb[0]='0'+(j>>10);
		j&=1023;

//		j=j*10;
		j=j*10;
//		j+=j>>10;
//		j=j*10+bi;
//		j=(j*sc+bi)>>sh;
//		if(((j&1023)>=1000))j+=24;
		tb[1]='0'+(j>>10);
		j&=1023;

//		j=j*10;
		j=j*10;
//		j+=j>>10;
//		j=j*10+bi;
//		j=(j*sc+bi)>>sh;
//		if(((j&1023)>=1000))j+=24;
		tb[2]='0'+(j>>10);
		j&=1023;
		tb[3]=0;
#endif
		
//		if(strcmp(tb, ta))
//			printf("%s!=%s (e=%d)\n", tb, ta, atoi(ta)-atoi(tb));
		printf("% d ", atoi(ta)-atoi(tb));
		if(i && !((i+1)%25))
			printf("\n");
	}
}
