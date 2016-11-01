#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
	__int128 li, lj, lk, ll;
	__float128 lf, lg, lh;
	time_t t0, t1, t2, t3;
	double dt;
	int n;
	int i, j, k;

#if 1
	printf("N-Int128 Add:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0; ll=0;
	while(t1<t2)
	{
		li=rand(); lj=rand()+1; lk=0;
		li=li*li*li*li*li*li;
		lj=lj*lj*lj*lj*lj*lj;
		for(i=0; i<100000; i++)
			{ lk+=li+lj; n++; }
		ll+=lk;
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("N-Int128 Mul:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0; ll=0;
	while(t1<t2)
	{
		li=rand(); lj=rand()+1; lk=0;
		li=li*li*li*li*li*li;
		lj=lj*lj*lj*lj*lj*lj;
		for(i=0; i<100000; i++)
			{ lk+=li*lj; n++; }
		ll+=lk;
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("N-Int128 Div:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0; ll=0;
	while(t1<t2)
	{
		li=rand(); lj=rand()+1; lk=0;
		li=li*li*li*li*li*li;
		lj=lj*lj*lj*lj*lj*lj;
		for(i=0; i<100000; i++)
			{ lk+=li/lj; n++; }
		ll+=lk;
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");
#endif


#if 1
	printf("N-F128 Add:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0; ll=0;
	while(t1<t2)
	{
		li=rand(); lj=rand()+1; lh=0;
		lf=li*li*li*li*li*li;
		lg=lj*lj*lj*lj*lj*lj;
		for(i=0; i<100000; i++)
			{ lh+=lf+lg; n++; }
		ll+=lh;
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("N-F128 Mul:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0; ll=0;
	while(t1<t2)
	{
		li=rand(); lj=rand()+1; lh=0;
		lf=li*li*li*li*li*li;
		lg=lj*lj*lj*lj*lj*lj;
		for(i=0; i<100000; i++)
			{ lh+=lf*lg; n++; }
		ll+=lh;
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("N-F128 Div:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0; ll=0;
	while(t1<t2)
	{
		li=rand(); lj=rand()+1; lh=0;
		lf=li*li*li*li*li*li;
		lg=lj*lj*lj*lj*lj*lj;
		for(i=0; i<100000; i++)
			{ lh+=lf/lg; n++; }
		ll+=lh;
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");
#endif
}