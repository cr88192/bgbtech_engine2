#include <bteifgl.h>

#if defined(__MACH__) && defined(__APPLE__)
# include <mach/mach_time.h>
#endif

#if defined(__arm__) || defined(__mips__) || defined(__linux__)
#include <sys/time.h>
#endif

#if defined(_MSC_VER)
extern u64 __rdtsc();
#pragma intrinsic(__rdtsc)
#endif

s64 gpio_rdtsc_cyclespersecond=700000000LL;
int bgbrpi_sched_loopCyclesPerUsec;
int bgbrpi_sched_usleep_happy;
int bgbrpi_sched_dummyVar;

void BGBRPI_Sched_DoSomeNops()
{
#ifdef _M_IX86
	__asm {
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	}
#else
	bgbrpi_sched_dummyVar=bgbrpi_sched_dummyVar^1;
#endif
}

void BGBRPI_Sched_DelayLoop(int usec)
{
	int i, n;
	
//	if(usec>100)
	if(1)
	{
		n=(usec*bgbrpi_sched_loopCyclesPerUsec)>>4;
		for(i=0; i<n; i++)
			{ BGBRPI_Sched_DoSomeNops(); }
	}else
	{
		n=(usec*bgbrpi_sched_loopCyclesPerUsec)>>6;
		for(i=0; i<n; i++)
			{ BGBRPI_Sched_DoSomeNops(); }
	}
}

BTEIFGL_API void BGBRPI_Sched_Delay(int usec)
{
	s64 t0;

	if(usec>20000)
	{
		t0=GPIO_GetTimeUS();
		while(usec>20000)
		{
#ifdef _WIN32
			Sleep(usec/2000);
#else
			usleep(usec/2);
#endif
			usec-=GPIO_GetDeltaTimeUS(&t0);
		}
		bgbrpi_sched_usleep_happy=10000;
	}else if(bgbrpi_sched_usleep_happy<0)
	{
		t0=GPIO_GetTimeUS();
#ifdef _WIN32
		Sleep(0);
		bgbrpi_sched_usleep_happy=100000;
#else
		usleep(0);
		bgbrpi_sched_usleep_happy=10000;
#endif
		usec-=GPIO_GetDeltaTimeUS(&t0);
//		bgbrpi_sched_usleep_happy=10000;
	}
	if(usec>0)
	{
		BGBRPI_Sched_DelayLoop(usec);
		bgbrpi_sched_usleep_happy-=usec;
	}
}

BTEIFGL_API void GPIO_InitRdtsc()
{
	static int init=0;
	s64 ta0, tb0, ta1, tb1;
#ifdef _WIN32
	LARGE_INTEGER tc0, tc1;
#endif
	double tcyc, tacc;
	int i, j, k;

	if(init)return;
	init=1;
//	gpio_rdtsc_cyclespersecond=700000000LL;

	printf("GPIO_InitRdtsc\n");
	ta0=GPIO_Rdtsc();
	tb0=GPIO_GetTimeRtcUS();
	ta1=ta0; tb1=tb0;
	tcyc=0; tacc=0;
	while(tacc<1000000.0)
	{
		tcyc+=GPIO_GetDeltaRdtsc(&ta1);
		tacc+=GPIO_GetDeltaTimeRtcUS(&tb1);
	}
	gpio_rdtsc_cyclespersecond=tcyc*(1000000.0/tacc)+0.5;

#if defined(_MSC_VER)
	printf("GPIO_InitRdtsc: Hz=%I64d\n", gpio_rdtsc_cyclespersecond);
	
	QueryPerformanceFrequency(&tc0);
	printf("GPIO_InitRdtsc: QPF=%I64d\n", tc0);
#else
	printf("GPIO_InitRdtsc: Hz=%lld\n", gpio_rdtsc_cyclespersecond);
#endif

	bgbrpi_sched_loopCyclesPerUsec=16;

	ta0=GPIO_Rdtsc();
	tb0=GPIO_GetTimeRtcUS();
	ta1=ta0; tb1=tb0;
	tcyc=0; tacc=0;
	while(tacc<1000000.0)
	{
		BGBRPI_Sched_DelayLoop(1000);
		tcyc+=1000;
	
//		tcyc+=GPIO_GetDeltaRdtsc(&ta1);
		tacc+=GPIO_GetDeltaTimeRtcUS(&tb1);
	}
	bgbrpi_sched_loopCyclesPerUsec=tcyc*(16.0/tacc)+0.5;

	printf("GPIO_InitRdtsc: LCpUs=%d(%.3f)\n",
		bgbrpi_sched_loopCyclesPerUsec,
		bgbrpi_sched_loopCyclesPerUsec/16.0);


	tb0=GPIO_GetTimeRtcUS();
	BGBRPI_Sched_DelayLoop(1000);
	tacc=GPIO_GetDeltaTimeRtcUS(&tb1);

	printf("GPIO_InitRdtsc: LErr1=%.4f\n",
		tacc/1000.0);
		
	tb1=GPIO_GetTimeRtcUS();
	BGBRPI_Sched_Delay(1000);
	tacc=GPIO_GetDeltaTimeRtcUS(&tb1);
	printf("GPIO_InitRdtsc: LErr2(1000us)=%.4f\n",
		tacc/1000.0);

	tb1=GPIO_GetTimeRtcUS();
	BGBRPI_Sched_Delay(100);
	tacc=GPIO_GetDeltaTimeRtcUS(&tb1);
	printf("GPIO_InitRdtsc: LErr2(100us)=%.4f\n",
		tacc/100.0);

	tacc=0;
	tb1=GPIO_GetTimeRtcUS();
	for(i=0; i<100; i++)
	{
//		tb1=GPIO_GetTimeRtcUS();
		BGBRPI_Sched_Delay(10);
//		BGBRPI_Sched_DelayLoop(10);
//		tacc=GPIO_GetDeltaTimeRtcUS(&tb1);
	}
	tacc=GPIO_GetDeltaTimeRtcUS(&tb1);
	printf("GPIO_InitRdtsc: LErr2(10us)=%.4f\n",
		tacc/1000.0);
}

s64 GPIO_CyclesPerSecond()
{
	return(gpio_rdtsc_cyclespersecond);
//	return(700000000LL);
}

s64 GPIO_Rdtsc()
{
#if defined(__MACH__) && defined(__APPLE__)
	return mach_absolute_time();
#elif defined(__i386__)
	s64 ret;
	__asm__ volatile ("rdtsc" : "=A" (ret) );
	return ret;
#elif defined(__x86_64__) || defined(__amd64__)
	u64 low, high;
	__asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
	return (high << 32) | low;
#elif defined(__powerpc__) || defined(__ppc__)
	s64 tbl, tbu0, tbu1;
	asm("mftbu %0" : "=r" (tbu0));
	asm("mftb  %0" : "=r" (tbl));
	asm("mftbu %0" : "=r" (tbu1));
	tbl &= -(s64)(tbu0 == tbu1);
	return (tbu1 << 32) | tbl;
#elif defined(__sparc__)
	s64 tick;
	asm(".byte 0x83, 0x41, 0x00, 0x00");
	asm("mov   %%g1, %0" : "=r" (tick));
	return tick;
#elif defined(__ia64__)
	s64 itc;
	asm("mov %0 = ar.itc" : "=r" (itc));
	return itc;
#elif defined(_MSC_VER)
#ifdef _WIN32
	LARGE_INTEGER ret;
	QueryPerformanceCounter(&ret);
	return(ret.QuadPart);
#else
	return __rdtsc();
#endif
#elif defined(__arm__)
#if 0
	u32 pmccntr;
	u32 pmuseren;
	u32 pmcntenset;
	asm volatile ("mrc p15, 0, %0, c9, c14, 0" : "=r" (pmuseren));
	if (pmuseren & 1) {
		asm volatile ("mrc p15, 0, %0, c9, c12, 1" : "=r" (pmcntenset));
		if (pmcntenset & 0x80000000ul)
		{
			asm volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r" (pmccntr));
			return (s64)((pmccntr)*64);
		}
	}
#endif

#ifdef RASPBERRYPI
	return((s64)GPIO_GetUSecTimerValue());
#endif

	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (s64)((tv.tv_sec + tv.tv_usec * 0.000001) * 
		GPIO_CyclesPerSecond());
#elif defined(__mips__)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (s64)((tv.tv_sec + tv.tv_usec * 0.000001) *
		GPIO_CyclesPerSecond());
#else

#if defined(__linux__)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (s64)((tv.tv_sec + tv.tv_usec * 0.000001) *
		GPIO_CyclesPerSecond());
#else
#error GPIO_Rdtsc: Unhandled Target
#endif

#endif
}

BTEIFGL_API double GPIO_GetDeltaRdtsc(s64 *rt0)
{
	s64 t0, t1, dt;
	double t;
	
	t0=*rt0;
	t1=GPIO_Rdtsc();
	dt=t1-t0;
	if(dt<0)dt=0;
	*rt0=t1;
	
	t=dt;
	return(t);
}


BTEIFGL_API s64 GPIO_GetTimeUS(void)
{
	s64 t;

#ifdef RASPBERRYPI
	return((s64)GPIO_GetUSecTimerValue());
#endif
	
	t=GPIO_Rdtsc();
//	t=(t*1000000LL)/GPIO_CyclesPerSecond();
	t=(t*1000000.0)/GPIO_CyclesPerSecond();
	return(t);
}

BTEIFGL_API double GPIO_GetDeltaTimeUS(s64 *rt0)
{
	s64 t0, t1, dt;
	double t;

#ifdef RASPBERRYPI
	t0=*rt0;
	t1=(s64)GPIO_GetUSecTimerValue();
	dt=t1-t0;
	if(dt<0)dt=0;
	*rt0=t1;
	return((double)dt);
#endif
	
	t0=*rt0;
	t1=GPIO_Rdtsc();
	dt=t1-t0;
	if(dt<0)dt=0;
	*rt0=t1;
	
	t=(dt*1000000.0)/GPIO_CyclesPerSecond();
	return(t);
}


BTEIFGL_API double GPIO_GetTimeRtcUS()
{
#if defined(__linux__)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (s64)((tv.tv_sec*1000000LL + tv.tv_usec));
#else
#ifdef _WIN32
	LARGE_INTEGER tc, tf;
	QueryPerformanceFrequency(&tf);
	QueryPerformanceCounter(&tc);
	return(tc.QuadPart*(1000000.0/tf.QuadPart));

//	static unsigned int init;
//	unsigned int t;
//	t=timeGetTime();
//	if(!init)init=t;
//	return((unsigned int)(t-init));
#else
#error GPIO_GetTimeRtcUS: Unhandled Target
#endif
#endif
}

BTEIFGL_API double GPIO_GetDeltaTimeRtcUS(s64 *rt0)
{
	s64 t0, t1, dt;
	double t;
	
	t0=*rt0;
	t1=GPIO_GetTimeRtcUS();
	dt=t1-t0;
	if(dt<0)dt=0;
	*rt0=t1;
	
	t=dt;
	return(t);
}

BTEIFGL_API s64 GPIO_GetTimeRtcUS2(void)
	{ return(GPIO_GetTimeRtcUS()); }
