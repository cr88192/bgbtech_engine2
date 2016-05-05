#include <bteifgl.h>

BGBDTC_X128 BGBDT_XI128_FromLong(s64 li)
{
	BGBDTC_X128 c;
	
	c.a=(u32)li;
	c.b=(u32)(li>>32);
	c.c=(u32)(li>>63);
	c.d=(u32)(li>>63);
	return(c);
}

s64 BGBDT_XI128_ToLong(BGBDTC_X128 a)
{
	s64 li;
	li=a.b;
	li=(li<<32)|a.a;
	return(li);
}

char *BGBDT_XI128_ToString(BGBDTC_X128 a)
{
	char tb[256];
	sprintf(tb, "0x%08X_%08X_%08X_%08X", a.d, a.c, a.b, a.a);
	return(frgl_rstrdup(tb));
}

BGBDTC_X128 BGBDT_XI128_Add(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	u64 l;
	
	l=((u64)a.a)+b.a;
	c.a=(u32)l;
	l=((l>>32)+a.b)+b.b;
	c.b=(u32)l;
	l=((l>>32)+a.c)+b.c;
	c.c=(u32)l;
	l=((l>>32)+a.d)+b.d;
	c.d=(u32)l;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Neg(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	u64 l;
	
	l=((u64)(~a.a))+1;
	c.a=(u32)l;
	l=(l>>32)+(~a.b);
	c.b=(u32)l;
	l=(l>>32)+(~a.c);
	c.c=(u32)l;
	l=(l>>32)+(~a.d);
	c.d=(u32)l;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Sub(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 nb, c;
	nb=BGBDT_XI128_Neg(b);
	c=BGBDT_XI128_Add(a, nb);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Mul(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 nb, c;
	u64 l;
	
	l=((u64)a.a)*b.a;
	c.a=(u32)l;
	l=(l>>32)+(((u64)a.b)*b.a)+(((u64)a.b)*b.a);
	c.b=(u32)l;
	l=(l>>32)+(((u64)a.c)*b.a)+(((u64)a.c)*b.a)+(((u64)a.b)*b.b);
	c.c=(u32)l;
	l=(l>>32)+(((u64)a.d)*b.a)+(((u64)a.d)*b.a)+
		(((u64)a.c)*b.b)+(((u64)a.b)*b.c);
	c.d=(u32)l;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_And(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a&b.a;
	c.b=a.b&b.b;
	c.c=a.c&b.c;
	c.d=a.d&b.d;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Or(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a|b.a;
	c.b=a.b|b.b;
	c.c=a.c|b.c;
	c.d=a.d|b.d;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Xor(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a^b.a;
	c.b=a.b^b.b;
	c.c=a.c^b.c;
	c.d=a.d^b.d;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shlp(BGBDTC_X128 a, int b)
{
	BGBDTC_X128 c;
	if(!b)
		return(a);
	c.a=(a.a<<b);
	c.b=(a.b<<b)+(a.a>>(32-b));
	c.c=(a.c<<b)+(a.b>>(32-b));
	c.d=(a.d<<b)+(a.c>>(32-b));
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shrp(BGBDTC_X128 a, int b)
{
	BGBDTC_X128 c;
	if(!b)
		return(a);
	c.a=(a.a>>b)+(a.b<<(32-b));
	c.b=(a.b>>b)+(a.c<<(32-b));
	c.c=(a.c>>b)+(a.d<<(32-b));
	c.d=(a.d>>b);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Sarp(BGBDTC_X128 a, int b)
{
	BGBDTC_X128 c;
	u32 sx;

	if(!b)
		return(a);
	sx=((s32)a.d)>>31;
	c.a=(a.a>>b)+(a.b<<(32-b));
	c.b=(a.b>>b)+(a.c<<(32-b));
	c.c=(a.c>>b)+(a.d<<(32-b));
	c.d=(a.d>>b)+(sx<<(32-b));
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shl(BGBDTC_X128 a, int b)
{
	BGBDTC_X128 a2, c;
	switch(b>>5)
	{
		case 0:
			a2=a; break;
		case 1:
			a2.a=0;		a2.b=a.a;
			a2.c=a.b;	a2.d=a.d;
			break;
		case 2:
			a2.a=0;		a2.b=0;
			a2.c=a.a;	a2.d=a.b;
			break;
		case 3:
			a2.a=0; a2.b=0;
			a2.c=0; a2.d=a.a;
			break;
		default:
			a2.a=0; a2.b=0;
			a2.c=0; a2.d=0;
			break;
	}
	c=BGBDT_XI128_Shlp(a2, b&31);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shr(BGBDTC_X128 a, int b)
{
	BGBDTC_X128 a2, c;
	switch(b>>5)
	{
		case 0:
			a2=a; break;
		case 1:
			a2.a=a.b;	a2.b=a.c;
			a2.c=a.d;	a2.d=0;
			break;
		case 2:
			a2.a=a.c;	a2.b=a.d;
			a2.c=0;		a2.d=0;
			break;
		case 3:
			a2.a=a.d; a2.b=0;
			a2.c=0;	 a2.d=0;
			break;
		default:
			a2.a=0; a2.b=0;
			a2.c=0; a2.d=0;
			break;
	}
	c=BGBDT_XI128_Shrp(a2, b&31);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Sar(BGBDTC_X128 a, int b)
{
	BGBDTC_X128 a2, c;
	u32 sx;

	sx=((s32)a.d)>>31;
	
	switch(b>>5)
	{
		case 0:
			a2=a; break;
		case 1:
			a2.a=a.b;	a2.b=a.c;
			a2.c=a.d;	a2.d=sx;
			break;
		case 2:
			a2.a=a.c;	a2.b=a.d;
			a2.c=sx;	 a2.d=sx;
			break;
		case 3:
			a2.a=a.d; a2.b=sx;
			a2.c=sx;	a2.d=sx;
			break;
		default:
			a2.a=sx; a2.b=sx;
			a2.c=sx; a2.d=sx;
			break;
	}
	c=BGBDT_XI128_Sarp(a2, b&31);
	return(c);
}
