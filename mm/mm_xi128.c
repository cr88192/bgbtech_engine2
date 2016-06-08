#include <bteifgl.h>

BGBDTC_X128 *bgbdt_xi128_freesi128;
// BGBDTC_X128 *bgbdt_xi128_freeui128;

static int objty_si128=-1;
// static int objty_ui128=-1;
// static int objty_f128=-1;

BGBDTC_X128 *BGBDT_XI128_AllocInt128(void)
{
	BGBDTC_X128 *xp;
	int i;
	
	xp=bgbdt_xi128_freesi128;
	if(xp)
	{
		bgbdt_xi128_freesi128=*(BGBDTC_X128 **)xp;
		return(xp);
	}

	if(objty_si128<0)
		{ objty_si128=BGBDT_MM_GetIndexObjTypeName("_int128_t"); }

	xp=dtmAlloc("_int128_t", 128*sizeof(BGBDTC_X128));
	
	for(i=0; i<127; i++)
	{
		*(BGBDTC_X128 **)xp=bgbdt_xi128_freesi128;
		bgbdt_xi128_freesi128=xp;
		xp++;
	}
	return(xp);
}

#if 0
// BGBDTC_X128 *BGBDT_XI128_AllocUInt128(void)
{
	BGBDTC_X128 *xp;
	int i;
	
	xp=bgbdt_xi128_freeui128;
	if(xp)
	{
		bgbdt_xi128_freeui128=*(BGBDTC_X128 **)xp;
		return(xp);
	}

	if(objty_ui128<0)
		{ objty_ui128=BGBDT_MM_GetIndexObjTypeName("_uint128_t"); }

	xp=dtmAlloc("_uint128_t", 128*sizeof(BGBDTC_X128));
	
	for(i=0; i<127; i++)
	{
		*(BGBDTC_X128 **)xp=bgbdt_xi128_freeui128;
		bgbdt_xi128_freeui128=xp;
		xp++;
	}
	return(xp);
}
#endif

void BGBDT_XI128_FreeInt128(BGBDTC_X128 *xp)
{
	*(BGBDTC_X128 **)xp=bgbdt_xi128_freesi128;
	bgbdt_xi128_freesi128=xp;
}

#if 0
// void BGBDT_XI128_FreeUInt128(BGBDTC_X128 *xp)
{
	*(BGBDTC_X128 **)xp=bgbdt_xi128_freeui128;
	bgbdt_xi128_freeui128=xp;
}
#endif

BTEIFGL_API int BGBDT_XI128_IsInt128P(dtVal objv)
	{ return(dtvCheckPtrTagP(objv, objty_si128)); }

BTEIFGL_API int BGBDT_XI128_IsSmallInt128P(dtVal objv)
{
	if(dtvCheckPtrTagP(objv, objty_si128))
		return(1);
	if(dtvIsSmallLongP(objv))
		return(1);
	return(0);
}


BGBDTC_X128 BGBDT_XI128_FromLong(s64 li)
{
	BGBDTC_X128 c;
	
	c.a=(u32)li;
	c.b=(u32)(li>>32);
	c.c=(u32)(li>>63);
	c.d=(u32)(li>>63);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_FromLong2(u64 lo, u64 hi)
{
	BGBDTC_X128 c;
	
	c.a=(u32)(lo    );
	c.b=(u32)(lo>>32);
	c.c=(u32)(hi    );
	c.d=(u32)(hi>>32);
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

BGBDTC_X128 BGBDT_XI128_AddX(BGBDTC_X128 a, BGBDTC_X128 b)
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

BGBDTC_X128 BGBDT_XI128_NegX(BGBDTC_X128 a)
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

BGBDTC_X128 BGBDT_XI128_NotX(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=~a.a;	c.b=~a.b;
	c.c=~a.c;	c.d=~a.d;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_SubX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 nb, c;
	nb=BGBDT_XI128_NegX(b);
	c=BGBDT_XI128_AddX(a, nb);
	return(c);
}

#if 1
void BGBDT_XI128_AddWX(
	BGBDTC_X128 aa, BGBDTC_X128 ab,
	BGBDTC_X128 ba, BGBDTC_X128 bb,
	BGBDTC_X128 *rca, BGBDTC_X128 *rcb)
{
	BGBDTC_X128 ca, cb;
	u64 l;
	
	l=((u64)aa.a)+ba.a;		ca.a=(u32)l;
	l=((l>>32)+aa.b)+ba.b;	ca.b=(u32)l;
	l=((l>>32)+aa.c)+ba.c;	ca.c=(u32)l;
	l=((l>>32)+aa.d)+ba.d;	ca.d=(u32)l;
	l=((l>>32)+ab.a)+bb.a;	cb.a=(u32)l;
	l=((l>>32)+ab.b)+bb.b;	cb.b=(u32)l;
	l=((l>>32)+ab.c)+bb.c;	cb.c=(u32)l;
	l=((l>>32)+ab.d)+bb.d;	cb.d=(u32)l;
	*rca=ca; *rcb=cb;
}

void BGBDT_XI128_NegWX(BGBDTC_X128 aa, BGBDTC_X128 ab,
	BGBDTC_X128 *rca, BGBDTC_X128 *rcb)
{
	BGBDTC_X128 ca, cb;
	u64 l;
	l=((u64)(~aa.a))+1;	ca.a=(u32)l;
	l=(l>>32)+(~aa.b);	ca.b=(u32)l;
	l=(l>>32)+(~aa.c);	ca.c=(u32)l;
	l=(l>>32)+(~aa.d);	ca.d=(u32)l;
	l=(l>>32)+(~ab.b);	cb.a=(u32)l;
	l=(l>>32)+(~ab.b);	cb.b=(u32)l;
	l=(l>>32)+(~ab.c);	cb.c=(u32)l;
	l=(l>>32)+(~ab.d);	cb.d=(u32)l;
	*rca=ca; *rcb=cb;
}

void BGBDT_XI128_NotWX(BGBDTC_X128 aa, BGBDTC_X128 ab,
	BGBDTC_X128 *rca, BGBDTC_X128 *rcb)
{
	*rca=BGBDT_XI128_NotX(aa);
	*rcb=BGBDT_XI128_NotX(ab);
}

void BGBDT_XI128_SubWX(
	BGBDTC_X128 aa, BGBDTC_X128 ab,
	BGBDTC_X128 ba, BGBDTC_X128 bb,
	BGBDTC_X128 *rca, BGBDTC_X128 *rcb)
{
	BGBDTC_X128 nba, nbb, c;
	BGBDT_XI128_NegWX(ba, bb, &nba, &nbb);
	BGBDT_XI128_AddWX(aa, ab, nba, nbb, rca, rcb);
}
#endif

BGBDTC_X128 BGBDT_XI128_MulX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 nb, c;
	u64 l;
	
	l=((u64)a.a)*b.a;
	c.a=(u32)l;
	l=(l>>32)+(((u64)a.b)*b.a)+(((u64)a.a)*b.b);
	c.b=(u32)l;
	l=(l>>32)+(((u64)a.c)*b.a)+(((u64)a.a)*b.c)+(((u64)a.b)*b.b);
	c.c=(u32)l;
	l=(l>>32)+(((u64)a.d)*b.a)+(((u64)a.a)*b.d)+
		(((u64)a.c)*b.b)+(((u64)a.b)*b.c);
	c.d=(u32)l;
	return(c);
}

int BGBDT_XI128_MulwX(BGBDTC_X128 a, BGBDTC_X128 b,
	BGBDTC_X128 *rc, BGBDTC_X128 *rd)
{
	BGBDTC_X128 nb, c, d;
	u64 l;
	
	l=((u64)a.a)*b.a;
	c.a=(u32)l;
	l=(l>>32)+(((u64)a.b)*b.a)+(((u64)a.a)*b.b);
	c.b=(u32)l;
	l=(l>>32)+(((u64)a.c)*b.a)+(((u64)a.a)*b.c)+(((u64)a.b)*b.b);
	c.c=(u32)l;
	l=(l>>32)+(((u64)a.d)*b.a)+(((u64)a.a)*b.d)+
		(((u64)a.c)*b.b)+(((u64)a.b)*b.c);
	c.d=(u32)l;

	l=(l>>32)+(((u64)a.d)*b.b)+(((u64)a.b)*b.d)+
		(((u64)a.c)*b.c);
	d.a=(u32)l;
	l=(l>>32)+(((u64)a.d)*b.c)+(((u64)a.c)*b.d);
	d.b=(u32)l;
	l=(l>>32)+(((u64)a.d)*b.d);
	d.c=(u32)l;
	l=(l>>32);
	d.d=(u32)l;
	
	*rc=c;
	*rd=d;
	return(0);
}

BGBDTC_X128 BGBDT_XI128_AndX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a&b.a;
	c.b=a.b&b.b;
	c.c=a.c&b.c;
	c.d=a.d&b.d;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_OrX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a|b.a;
	c.b=a.b|b.b;
	c.c=a.c|b.c;
	c.d=a.d|b.d;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_XorX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a^b.a;
	c.b=a.b^b.b;
	c.c=a.c^b.c;
	c.d=a.d^b.d;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_ShlpX(BGBDTC_X128 a, int b)
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

BGBDTC_X128 BGBDT_XI128_ShrpX(BGBDTC_X128 a, int b)
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

BGBDTC_X128 BGBDT_XI128_SarpX(BGBDTC_X128 a, int b)
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

BGBDTC_X128 BGBDT_XI128_ShlX(BGBDTC_X128 a, int b)
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
	c=BGBDT_XI128_ShlpX(a2, b&31);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_ShrX(BGBDTC_X128 a, int b)
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
	c=BGBDT_XI128_ShrpX(a2, b&31);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_SarX(BGBDTC_X128 a, int b)
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
	c=BGBDT_XI128_SarpX(a2, b&31);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shr1X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=(a.a>>1)|(a.b<<31);
	c.b=(a.b>>1)|(a.c<<31);
	c.c=(a.c>>1)|(a.d<<31);
	c.d=(a.d>>1);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shl1X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=(a.d<<1)|(a.c>>31);
	c.c=(a.c<<1)|(a.b>>31);
	c.b=(a.b<<1)|(a.a>>31);
	c.a=(a.a<<1);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shl32X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=a.c;	c.c=a.b;
	c.b=a.a;	c.a=0;
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shl16X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=(a.d<<16)|(a.c>>16);
	c.c=(a.c<<16)|(a.b>>16);
	c.b=(a.b<<16)|(a.a>>16);
	c.a=(a.a<<16);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shl8X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=(a.d<<8)|(a.c>>24);
	c.c=(a.c<<8)|(a.b>>24);
	c.b=(a.b<<8)|(a.a>>24);
	c.a=(a.a<<8);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_Shrd1X(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=(a.a>>1)|(a.b<<31);
	c.b=(a.b>>1)|(a.c<<31);
	c.c=(a.c>>1)|(a.d<<31);
	c.d=(a.d>>1)|(b.a<<31);
	return(c);
}

int BGBDT_XI128_ZeroXP(BGBDTC_X128 a)
{
	return(!(a.a|a.b|a.c|a.d));
}

int BGBDT_XI128_NegXP(BGBDTC_X128 a)
{
	return(a.d>>31);
}

int BGBDT_XI128_CmpX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	if(a.d>b.d)return( 1);
	if(a.d<b.d)return(-1);
	if(a.c>b.c)return( 1);
	if(a.c<b.c)return(-1);
	if(a.b>b.b)return( 1);
	if(a.b<b.b)return(-1);
	if(a.a>b.a)return( 1);
	if(a.a<b.a)return(-1);
	return(0);
}

int BGBDT_XI128_CmpWX(
	BGBDTC_X128 aa, BGBDTC_X128 ab,
	BGBDTC_X128 ba, BGBDTC_X128 bb)
{
	if(ab.d>bb.d)return( 1);
	if(ab.d<bb.d)return(-1);
	if(ab.c>bb.c)return( 1);
	if(ab.c<bb.c)return(-1);
	if(ab.b>bb.b)return( 1);
	if(ab.b<bb.b)return(-1);
	if(ab.a>bb.a)return( 1);
	if(ab.a<bb.a)return(-1);
	if(aa.d>ba.d)return( 1);
	if(aa.d<ba.d)return(-1);
	if(aa.c>ba.c)return( 1);
	if(aa.c<ba.c)return(-1);
	if(aa.b>ba.b)return( 1);
	if(aa.b<ba.b)return(-1);
	if(aa.a>ba.a)return( 1);
	if(aa.a<ba.a)return(-1);
	return(0);
}

BGBDTC_X128 BGBDT_XI128_OrBitX(BGBDTC_X128 a, int pos)
{
	BGBDTC_X128 c;
	c=a;
	switch(pos>>5)
	{
	case 0: c.a|=1<<(pos&31); break;
	case 1: c.b|=1<<(pos&31); break;
	case 2: c.c|=1<<(pos&31); break;
	case 3: c.d|=1<<(pos&31); break;
	default: break;
	}
	return(c);
}

int BGBDT_XI128_DivuXI(BGBDTC_X128 a, BGBDTC_X128 b,
	BGBDTC_X128 *rq, BGBDTC_X128 *rr)
{
	BGBDTC_X128 dh, dl, q, r;
	int pos;
	
	dh=b;
	dl=BGBDT_XI128_FromLong(0);
	q=BGBDT_XI128_FromLong(0);
	r=a;
	for(pos=128; pos>=0; pos--)
	{
		if(!BGBDT_XI128_ZeroXP(dh))
		{
			dl=BGBDT_XI128_Shrd1X(dl, dh);
			dh=BGBDT_XI128_Shr1X(dh);
			continue;
		}
		if(BGBDT_XI128_CmpX(r, dl)>=0)
		{
			r=BGBDT_XI128_SubX(r, dl);
			q=BGBDT_XI128_OrBitX(q, pos);
		}
		dl=BGBDT_XI128_Shrd1X(dl, dh);
		dh=BGBDT_XI128_Shr1X(dh);
	}
	if(rq)
		{ *rq=q; }
	if(rr)
		{ *rr=r; }
	return(0);
}

int BGBDT_XI128_DivW1XI(
	BGBDTC_X128 al, BGBDTC_X128 ah,
	BGBDTC_X128 b,
	BGBDTC_X128 *rq, BGBDTC_X128 *rr)
{
	BGBDTC_X128 dh, dl, q, rl, rh, z;
	int pos;
	
	z.a=0; z.b=0; z.c=0; z.d=0;
	dh=b;
	dl=BGBDT_XI128_FromLong(0);
	q=BGBDT_XI128_FromLong(0);
	rl=al; rh=ah;
	for(pos=128; pos>=0; pos--)
	{
//		if(!BGBDT_XI128_ZeroXP(dh))
//		{
//			dl=BGBDT_XI128_Shrd1X(dl, dh);
//			dh=BGBDT_XI128_Shr1X(dh);
//			continue;
//		}
//		if(BGBDT_XI128_CmpX(r, dl)>=0)
		if(BGBDT_XI128_CmpWX(rl, rh, dl, dh)>=0)
		{
			BGBDT_XI128_SubWX(rl, rh, dl, dh, &rl, &rh);
			q=BGBDT_XI128_OrBitX(q, pos);
		}
		dl=BGBDT_XI128_Shrd1X(dl, dh);
		dh=BGBDT_XI128_Shr1X(dh);
	}
	if(rq)
		{ *rq=q; }
	if(rr)
		{ *rr=rl; }
	return(0);
}

int BGBDT_XI128_DivXI(BGBDTC_X128 a, BGBDTC_X128 b,
	BGBDTC_X128 *rq, BGBDTC_X128 *rr)
{
	BGBDTC_X128 ta, tb;
	int sg, rt;
	
	ta=a; tb=b; sg=0;
	if(BGBDT_XI128_NegXP(ta))
		{ ta=BGBDT_XI128_NegX(ta); sg=!sg; }
	if(BGBDT_XI128_NegXP(tb))
		{ tb=BGBDT_XI128_NegX(tb); sg=!sg; }
	rt=BGBDT_XI128_DivuXI(ta, tb, rq, rr);
	if(sg && rq)
	{
		*rq=BGBDT_XI128_NegX(*rq);
	}
	return(rt);
}

BGBDTC_X128 BGBDT_XI128_DivX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	BGBDT_XI128_DivXI(a, b, &c, NULL);
	return(c);
}

BGBDTC_X128 BGBDT_XI128_ModX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	BGBDT_XI128_DivXI(a, b, NULL, &c);
	return(c);
}


BTEIFGL_API dtVal BGBDT_XI128_WrapInt128(BGBDTC_X128 a)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XI128_AllocInt128();
	*xp=a;
	return(dtvWrapTagPtrF(xp, objty_si128));
}

#if 0
// BTEIFGL_API dtVal BGBDT_XI128_WrapUInt128(BGBDTC_X128 a)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XI128_AllocUInt128();
	*xp=a;
	return(dtvWrapTagPtrF(xp, objty_ui128));
}
#endif

BTEIFGL_API dtVal BGBDT_XI128_WrapInt128L(s64 l)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XI128_AllocInt128();
	*xp=BGBDT_XI128_FromLong(l);
	return(dtvWrapTagPtrF(xp, objty_si128));
}

BTEIFGL_API dtVal BGBDT_XI128_WrapInt128L2(u64 lo, u64 hi)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XI128_AllocInt128();
	xp->a=(u32)(lo    );
	xp->b=(u32)(lo>>32);
	xp->c=(u32)(hi    );
	xp->d=(u32)(hi>>32);
	return(dtvWrapTagPtrF(xp, objty_si128));
}

#if 0
// BTEIFGL_API dtVal BGBDT_XI128_WrapUInt128L2(u64 lo, u64 hi)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XV128_AllocUInt128();
	xp->a=(u32)(lo    );
	xp->b=(u32)(lo>>32);
	xp->c=(u32)(hi    );
	xp->d=(u32)(hi>>32);
	return(dtvWrapTagPtrF(xp, objty_ui128));
}
#endif

BTEIFGL_API void BGBDT_XI128_GetInt128v(dtVal vec, BGBDTC_X128 *v)
{
	BGBDTC_X128 *xp;
	xp=dtvUnwrapPtrF(vec);
	*v=*xp;
}

BTEIFGL_API BGBDTC_X128 BGBDT_XI128_UnwrapInt128(dtVal vec)
{
	BGBDTC_X128 *xp;
	s64 li;
	
	if(dtvCheckPtrTagP(vec, objty_si128))
	{
		xp=dtvUnwrapPtrF(vec);
		return(*xp);
	}
	
	li=dtvUnwrapLong(vec);
	return(BGBDT_XI128_FromLong(li));
}

#if 0
// BTEIFGL_API void BGBDT_XI128_GetUInt128v(dtVal vec, BGBDTC_X128 *v)
{
	BGBDTC_X128 *xp;
	xp=dtvUnwrapPtrF(vec);
	*v=*xp;
}
#endif


dtVal BGBDT_XI128_Add(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		xc=BGBDT_XI128_AddX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Sub(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		xc=BGBDT_XI128_SubX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Mul(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		xc=BGBDT_XI128_MulX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Div(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		xc=BGBDT_XI128_DivX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Mod(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		xc=BGBDT_XI128_ModX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_And(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		xc=BGBDT_XI128_AndX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Or(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		xc=BGBDT_XI128_OrX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}


dtVal BGBDT_XI128_Xor(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		xc=BGBDT_XI128_XorX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Shl(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		i=dtvUnwrapInt(b);
		xc=BGBDT_XI128_ShlX(xa, i);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Shr(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		i=dtvUnwrapInt(b);
		xc=BGBDT_XI128_ShrX(xa, i);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Sar(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	if(BGBDT_XI128_IsSmallInt128P(a) && BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		i=dtvUnwrapInt(b);
		xc=BGBDT_XI128_SarX(xa, i);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Neg(dtVal a)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	if(BGBDT_XI128_IsInt128P(a))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xc=BGBDT_XI128_NegX(xa);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_Not(dtVal a)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	if(BGBDT_XI128_IsInt128P(a))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xc=BGBDT_XI128_NotX(xa);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_XI128_LNot(dtVal a)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	if(BGBDT_XI128_IsInt128P(a))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		i=BGBDT_XI128_ZeroXP(xa);
		return(dtvWrapBool(i));
	}
	
	return(DTV_UNDEFINED);
}

int BGBDT_XI128_Compare(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	if(BGBDT_XI128_IsSmallInt128P(a) &&
		BGBDT_XI128_IsSmallInt128P(b))
	{
		xa=BGBDT_XI128_UnwrapInt128(a);
		xb=BGBDT_XI128_UnwrapInt128(b);
		i=BGBDT_XI128_CmpX(xa, xb);
		return(i);
	}
	
	return(-1);
}

int BGBDT_XI128_CompareB(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	xa=BGBDT_XI128_UnwrapInt128(a);
	xb=BGBDT_XI128_UnwrapInt128(b);
	i=BGBDT_XI128_CmpX(xa, xb);
	return(i);
}
