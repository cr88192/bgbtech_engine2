#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long u64;
typedef signed long long s64;


int main()
{
	int pxt[16];
	int cvx;
	int i, j, k, l;

	for(cvx=0; cvx<16; cvx++)
	{
		for(i=0; i<16; i++)
		{
			pxt[i]=-1;
		}

		for(i=0; i<16; i++)
		{
			j=i-cvx;
			j=(j<<1)^(j>>31);
			pxt[j&15]=i;
		}
		
		for(i=0; i<16; i++)
			printf("%d ", pxt[i]);
		printf("\n");
	}
}
