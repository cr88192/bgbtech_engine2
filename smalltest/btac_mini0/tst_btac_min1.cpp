#include "btac1c_mini0.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

int main()
{
	btac1c_s64 v, v0, v1, t, tt;
	int t0, t1, t2, te;
	int i, j, k;
	
	v=69696969;
//	v=40934093;
//	v=254;
//	printf("%d %d\n", (int)sqrt(v), (int)btac1c2_fakesqrt(v));
	
	for(i=0; i<16; i++)
	{
		v=((btac1c_s64)rand())*rand();
		v0=sqrt(v);
//		v1=btac1c2_fakesqrt(v);
		v1=btac1c2_fakesqrt32(v);
		if(v0!=v1)
			printf("%d!=%d\n", (int)v0, (int)v1);
	}
	
	v=69696969; t=0; tt=0;
	t0=clock(); t1=t0; te=t0+10*CLOCKS_PER_SEC;
	while((t1<te) && (t1>=t0))
	{
		for(i=0; i<1000; i++)
		{
//			v1=btac1c2_fakesqrt(v);
			v1=btac1c2_fakesqrt32(v);
//			v1=sqrt(v);
			tt+=v1;
		}
		t+=1000;
		t1=clock();
	}
	
	printf("%lld %lld\n", t, tt);
}
