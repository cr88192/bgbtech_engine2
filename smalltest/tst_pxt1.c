#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long u64;
typedef signed long long s64;


int main()
{
	int pxt[4096];
	int pxd[4096];
	int cvx;
	int i, j, k, l;

	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
			for(k=0; k<16; k++)
	{
		l=(i*16+j)*16+k;
		pxt[l]=l;
		pxd[l]=i*i+j*j+k*k;
	}

	for(i=0; i<4096; i++)
		for(j=i+1; j<4096; j++)
	{
		if(pxd[j]<pxd[i])
		{
			k=pxt[i]; pxt[i]=pxt[j]; pxt[j]=k;
			k=pxd[i]; pxd[i]=pxd[j]; pxd[j]=k;
		}
	}

	for(i=0; i<4096; i++)
	{
		if(i && !(i&7))
			printf("\n");
		printf("0x%03X, ", pxt[i]);
	}
}
