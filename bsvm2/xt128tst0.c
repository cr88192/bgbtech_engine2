#include <bsvm2.h>

#include <time.h>

int main()
{
	int t0, t1, t2, t3;
	s64 li, lj, lk, ll;
	u64 la, lb;
	BGBDTC_X128 va, vb, vc, vd;
	double dt;
	int n;
	int i, j, k, l;
	
//	__int128 xit;


	va=BGBDT_XF128_FromDouble(1.0);
	vb=BGBDT_XF128_FromDouble(2.0);

	vc=BGBDT_XF128_AddX(va, vb);
	vd=BGBDT_XF128_AddX(vb, va);

	printf("a=%f, b=%f, a+b=%f, b+a=%f\n",
		BGBDT_XF128_ToDouble(va), BGBDT_XF128_ToDouble(vb),
		BGBDT_XF128_ToDouble(vc), BGBDT_XF128_ToDouble(vd));

	vc=BGBDT_XF128_SubX(va, vb);
	vd=BGBDT_XF128_SubX(vb, va);
	printf("a=%f, b=%f, a-b=%f, b-a=%f\n",
		BGBDT_XF128_ToDouble(va), BGBDT_XF128_ToDouble(vb),
		BGBDT_XF128_ToDouble(vc), BGBDT_XF128_ToDouble(vd));

	va=BGBDT_XF128_FromDouble(3.1415926535897932384626433832795);
	vb=BGBDT_XF128_FromDouble(2.0);

	vc=BGBDT_XF128_MulX(va, vb);
	vd=BGBDT_XF128_MulX(vb, va);
	printf("a=%f, b=%f, a*b=%f, b*a=%f\n",
		BGBDT_XF128_ToDouble(va), BGBDT_XF128_ToDouble(vb),
		BGBDT_XF128_ToDouble(vc), BGBDT_XF128_ToDouble(vd));

	vc=BGBDT_XF128_DivX(va, vb);
	vd=BGBDT_XF128_DivX(vb, va);
	printf("a=%f, b=%f, a/b=%f, b/a=%f\n",
		BGBDT_XF128_ToDouble(va), BGBDT_XF128_ToDouble(vb),
		BGBDT_XF128_ToDouble(vc), BGBDT_XF128_ToDouble(vd));

	printf("a=%s\nb=%s\n",
		BGBDT_XF128_ToStringWd(va, 36),
		BGBDT_XF128_ToStringWd(vb, 36));

	vc=BGBDT_XF128_FromString("3.1415926535897932384626433832795");
	vd=vc;
	printf("c=%s\nd=%s\n",
		BGBDT_XF128_ToStringWd(vc, 36),
		BGBDT_XF128_ToStringWd(vd, 36));

	vc=BGBDT_XD128_AddX(va, vb);
	vc=BGBDT_XD128_MulX(va, vb);

#if 0
	printf("F128 Add:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
		{
//			__asm { nop }
			vc=BGBDT_XF128_AddX(va, vb); n++;
		}
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("F128 Mul:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
			{ vc=BGBDT_XF128_MulX(va, vb); n++; }
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("F128 Div:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
			{ vc=BGBDT_XF128_DivX(va, vb); n++; }
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");
#endif

#if 0
	printf("D128 Add:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
			{ vc=BGBDT_XD128_AddX(va, va); n++; }
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("D128 Mul:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
			{ vc=BGBDT_XD128_MulX(va, va); n++; }
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");
#endif

#if 1
	printf("I128 Add:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
		{
#if 0
			__asm {
				nop
				mov esi, eax
				mul ecx
				adc eax, edx
				nop
			}
#endif
			vc=BGBDT_XI128_AddX(va, vb); n++;
		}
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("I128 Mul:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
			{ vc=BGBDT_XI128_MulX(va, vb); n++; }
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	printf("I128 Div:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
			{ vc=BGBDT_XI128_DivX(va, vb); n++; }
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");

	vd=BGBDT_XI128_FromLong(3);
	printf("I128 Div 2:\n");
	t0=clock(); t1=t0; t2=t0+15*CLOCKS_PER_SEC; n=0;
	while(t1<t2)
	{
		for(i=0; i<100000; i++)
			{ vc=BGBDT_XI128_DivX(va, vd); n++; }
		t1=clock();
		dt=(t1-t0)/((double)CLOCKS_PER_SEC);
		printf("%f %d %.3f M/s  \r", dt, n, (n/1000000.0)/(dt+0.0001));
	}
	printf("\n");
#endif

#if 1
	printf("LL Div:\n");
	t0=clock(); t1=t0; t2=t0+1*CLOCKS_PER_SEC; n=0; ll=0;
	while(t1<t2)
	{
		li=rand(); lj=rand()+1; lk=0;
		lj=lj*lj*lj*lj;
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
//	la=3.1415926535897932384626433832795*(1LL<<60);
//	lb=(1LL<<62)/3.1415926535897932384626433832795;

//	la=0x3141592653589793ULL;
//	lb=0x2384626433832795ULL;

	la=3141592653589793ULL;
	lb=2384626433832795ULL;

//	va=BGBDT_XI128_FromLong2(lb, la);
//	vb=BGBDT_XI128_FromLong2(la, lb);

	va=BGBDT_XI128_FromLong(la);
	vb=BGBDT_XI128_FromLong(lb);

	printf("a=%s\nb=%s\n",
		BGBDT_XI128_ToString(va),
		BGBDT_XI128_ToString(vb));

	vc=BGBDT_XI128_AddX(va, vb);
	vd=BGBDT_XI128_AddX(vb, va);
	printf("a+b: %s\nb+a: %s\n",
		BGBDT_XI128_ToString(vc), BGBDT_XI128_ToString(vd));

	vc=BGBDT_XI128_SubX(va, vb);
	vd=BGBDT_XI128_SubX(vb, va);
	printf("a-b: %s\nb-a: %s\n",
		BGBDT_XI128_ToString(vc), BGBDT_XI128_ToString(vd));

	vc=BGBDT_XI128_MulX(va, vb);
	vd=BGBDT_XI128_MulX(vb, va);
	printf("a*b: %s\nb*a: %s\n",
		BGBDT_XI128_ToString(vc), BGBDT_XI128_ToString(vd));

	vc=BGBDT_XI128_DivX(va, vb);
	vd=BGBDT_XI128_DivX(vb, va);
	printf("a/b: %s\nb/a: %s\n",
		BGBDT_XI128_ToString(vc), BGBDT_XI128_ToString(vd));

	vc=BGBDT_XI128_ModX(va, vb);
	vd=BGBDT_XI128_ModX(vb, va);
	printf("a%%b: %s\nb%%a: %s\n",
		BGBDT_XI128_ToString(vc), BGBDT_XI128_ToString(vd));
#endif
}
