#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main()
{
	int i, j, k;
	
	for(i=0; i<128; i++)
	{
//		if(i && !(i&7))
		if(i && !(i%9))
			printf("\n");

#if 1
//		j=(i&7);
//		k=i>>3;
//		if(k)j|=8;
		j=(i%9);
		k=i/9;
		if(k)j+=9;
		k=(k>1)?(k-1):0;
		j=j<<k;
//		if(i&7)
			j=j|(j>>7)|(j>>14);
#endif
		
//		j=pow(2, i*((log(65536)/log(2))/127));
		
		printf("%6d,", j);
	}
	printf("\n");
	printf("\n");

	for(i=0; i<64; i++)
	{
		if(i && !(i&3))
//		if(i && !(i%5))
			printf("\n");

		j=(i&3);
		k=i>>2;
		if(k)j|=4;
//		j=(i%5);
//		k=i/5;
//		if(k)j+=5;

		k=(k>1)?(k-1):0;
		j=j<<k;
		if(i&3)
			j=j|(j>>6)|(j>>12);
		
		printf("%6d,", j);
	}
	printf("\n");
}
