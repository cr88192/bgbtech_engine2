#include <bteifgl.h>

BGBDTC_X128 *bgbdt_xf128_freef128;
static int objty_f128=-1;

BGBDTC_X128 *BGBDT_XF128_AllocFloat128(void)
{
	BGBDTC_X128 *xp;
	int i;
	
	xp=bgbdt_xf128_freef128;
	if(xp)
	{
		bgbdt_xf128_freef128=*(BGBDTC_X128 **)xp;
		return(xp);
	}

	if(objty_f128<0)
		{ objty_f128=BGBDT_MM_GetIndexObjTypeName("_float128_t"); }

	xp=dtmAlloc("_float128_t", 128*sizeof(BGBDTC_X128));
	
	for(i=0; i<127; i++)
	{
		*(BGBDTC_X128 **)xp=bgbdt_xf128_freef128;
		bgbdt_xf128_freef128=xp;
		xp++;
	}
	return(xp);
}

void BGBDT_XF128_FreeFloat128(BGBDTC_X128 *xp)
{
	*(BGBDTC_X128 **)xp=bgbdt_xf128_freef128;
	bgbdt_xf128_freef128=xp;
}



BGBDTC_X128 BGBDT_XF128_FromInt128(BGBDTC_X128 a)
{
	BGBDTC_X128 fm;
	int sgn, exp;
	
	fm=a; sgn=0;
	if(BGBDT_XI128_NegXP(fm))
		{ sgn=1; fm=BGBDT_XI128_NegX(fm); }

	if(BGBDT_XI128_ZeroXP(fm))
		return(fm);
	
	exp=0x3FFF+112;
	if(fm.d>>16)
	{
		while((fm.d>>16)>1)
			{ exp++; fm=BGBDT_XI128_Shr1X(fm); }
	}else
	{
		if(!fm.d && !fm.c)
		{
			if(!fm.b)
				{ exp-=64; fm.c=fm.a; fm.b=0; fm.a=0; }
			else
				{ exp-=32; fm.c=fm.b; fm.b=fm.a; fm.a=0; }
			exp-=16; fm=BGBDT_XI128_Shl16X(fm);
		}

		if(!fm.d)
			{ exp-=16; fm=BGBDT_XI128_Shl16X(fm); }
		else if(!(fm.d>>8))
			{ exp-=8; fm=BGBDT_XI128_Shl8X(fm); }

		while(!(fm.d>>16))
			{ exp--; fm=BGBDT_XI128_Shl1X(fm); }
	}
	
	fm.d=(fm.d&0x0000FFFF)|
		(exp<<16)|(sgn<<31);
	return(fm);
}

BGBDTC_X128 BGBDT_XF128_FromLong(s64 a)
{
	BGBDTC_X128 fm;
	fm=BGBDT_XI128_FromLong(a);
	fm=BGBDT_XF128_FromInt128(fm);
	return(fm);
}

BGBDTC_X128 BGBDT_XF128_FromDouble(double v)
{
	BGBDTC_X128 fm;
	u64 la;
	int sgn, exp;

	la=*(u64 *)(&v);

	if(!la)
	{
		fm=BGBDT_XI128_FromLong(0);
		return(fm);
	}

	fm=BGBDT_XI128_FromLong(la&0x000FFFFFFFFFFFFFULL);
	exp=(la>>52)&0x7FF;
	sgn=(la>>63)&1;
	
	exp=exp+(0x3FFF-0x3FF);
	fm=BGBDT_XI128_ShlX(fm, 112-52);

	fm.d=(fm.d&0x0000FFFF)|
		(exp<<16)|(sgn<<31);
	return(fm);
}


BGBDTC_X128 BGBDT_XF128_NegX(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c=a;
	c.d^=0x80000000U;
	return(c);
}

BGBDTC_X128 BGBDT_XF128_GetMantissaX(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=a.a;	c.b=a.b;
	c.c=a.c;	c.d=(a.d&0x0000FFFF)|0x00010000;
	return(c);
}

int BGBDT_XF128_GetExponentX(BGBDTC_X128 a)
{
	return((a.d>>16)&0x7FFF);
}

int BGBDT_XF128_FixupMantissaNarrowX(
	BGBDTC_X128 a, BGBDTC_X128 *rc, int *rexp)
{
	BGBDTC_X128 c, d;
	int exp;
	
	c=a;
	exp=*rexp;

	if(c.d>>16)
	{
		while((c.d>>16)>1)
			{ exp++; c=BGBDT_XI128_Shr1X(c); }
	}else
	{
		if(!c.d && !c.c)
		{
			if(!c.b)
				{ exp-=64; c.c=c.a; c.b=0; c.a=0; }
			else
				{ exp-=32; c.c=c.b; c.b=c.a; c.a=0; }
			exp-=16;
			c=BGBDT_XI128_Shl16X(c);
		}

		if(!c.d)
		{
			exp-=16;
			c=BGBDT_XI128_Shl16X(c);
		}
		else if(!(c.d>>8))
		{
			exp-=8;
			c=BGBDT_XI128_Shl8X(c);
		}

		while(!(c.d>>16))
		{
			exp--;
			c=BGBDT_XI128_Shl1X(c);
		}
	}
	
	*rc=c;
	*rexp=exp;
}

int BGBDT_XF128_FixupMantissaWideX(
	BGBDTC_X128 a, BGBDTC_X128 b, BGBDTC_X128 *rc, int *rexp)
{
	BGBDTC_X128 c, d;
	int exp;
	
	c=a; d=b;
	exp=*rexp;

	if(c.d>>16)
	{
		while((c.d>>16)>1)
			{ exp++; c=BGBDT_XI128_Shr1X(c); }
	}else
	{
		if(!c.d && !c.c)
		{
			if(!c.b)
			{
				exp-=64;
				c.c=c.a; c.b=d.d; c.a=d.c;
				d.d=d.b; d.c=d.a; d.b=0; d.a=0;
			}
			else
			{
				exp-=32;
				c.c=c.b; c.b=c.a; c.a=d.d;
				d.d=d.c; d.c=d.b; d.b=d.a; d.a=0; 
			}
			exp-=16;
			c=BGBDT_XI128_Shl16X(c);
			c.a=c.a|(d.d>>16);
			d=BGBDT_XI128_Shl16X(d);
		}

		if(!c.d)
		{
			exp-=16;
			c=BGBDT_XI128_Shl16X(c);
			c.a=c.a|(d.d>>16);
			d=BGBDT_XI128_Shl16X(d);
		}
		else if(!(c.d>>8))
		{
			exp-=8;
			c=BGBDT_XI128_Shl8X(c);
			c.a=c.a|(d.d>>24);
			d=BGBDT_XI128_Shl8X(d);
		}

		while(!(c.d>>16))
		{
			exp--;
			c=BGBDT_XI128_Shl1X(c);
			c.a=c.a|(d.d>>31);
			d=BGBDT_XI128_Shl1X(d);
		}
	}
	
	*rc=c;
	*rexp=exp;
}

BGBDTC_X128 BGBDT_XF128_AddX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	BGBDTC_X128 ma, mb, mcl, mch, mc;
	int exa, exb, exc, sgn, sga, sgb;
	
	u64 t0, t1, t2, t3;
	BGBDTC_X128 l0, l1;
	int i;

	sga=BGBDT_XI128_NegXP(a);
	sgb=BGBDT_XI128_NegXP(b);
	exa=BGBDT_XF128_GetExponentX(a);
	exb=BGBDT_XF128_GetExponentX(b);
	ma=BGBDT_XF128_GetMantissaX(a);
	mb=BGBDT_XF128_GetMantissaX(b);

	if(exa>=exb)
	{
		if(sga^sgb)
		{
			i=exa-exb;
			if(i>108)return(a);
			l0=BGBDT_XI128_ShrX(mb, i+1);
			l1=BGBDT_XI128_Shr1X(ma);
			l1=BGBDT_XI128_SubX(l1, l0);

			if(BGBDT_XI128_NegXP(l1))
			{
				sgc=!sga;
				exc=exa+1;
//				l0.d=0x00010000; l0.c=0; l0.b=0; l0.a=0;
//				mc=BGBDT_XI128_SubX(l0, l1);
				mc=BGBDT_XI128_NegX(l1);
			}else
			{
				sgc=!sga;
				exc=exa+1;
				mc=l1;
			}
		}else
		{
			i=exa-exb;
			if(i>108)return(a);
			l0=BGBDT_XI128_ShrX(mb, i+1);
			l1=BGBDT_XI128_Shr1X(ma);
			l1=BGBDT_XI128_AddX(l1, l0);

			sgc=sga;
			exc=exa+1;
			mc=l1;
		}
	}else
	{
		if(sga^sga)
		{
			i=exb-exa;
			if(i>108)return(b);
			l0=BGBDT_XI128_ShrX(mb, i+1);
			l1=BGBDT_XI128_Shr1X(ma);
			l1=BGBDT_XI128_SubX(l1, l0);

			if(BGBDT_XI128_NegXP(l1))
			{
				sgc=!sgb;
				exc=exb+1;
				mc=BGBDT_XI128_NegX(l1);
			}else
			{
				sgc=!sgb;
				exc=exb+1;
				mc=l1;
			}
		}else
		{
			i=exb-exa;
			if(i>108)return(b);
			l0=BGBDT_XI128_ShrX(mb, i+1);
			l1=BGBDT_XI128_Shr1X(ma);
			l1=BGBDT_XI128_AddX(l1, l0);

			sgc=sgb;
			exc=exb+1;
			mc=l1;
		}
	}

	if(BGBDT_XI128_ZeroXP(mc))
		{ return(mc); }

	BGBDT_XF128_FixupMantissaNarrowX(mc, &mc, &exc);

	mc.d=(mc.d&0x0000FFFF)|
		(exc<<16)|(sgn<<31);
	return(mc);
}

BGBDTC_X128 BGBDT_XF128_SubX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	return(BGBDT_XF128_AddX(a,
		BGBDT_XF128_NegX(b)));
}

BGBDTC_X128 BGBDT_XF128_MulX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 ma, mb, mcl, mch, mc;
	int exa, exb, exc, sgn;
	
	sgn=(a.d^b.d)>>31;
	exa=(a.d>>16)&0x7FFF;
	exb=(b.d>>16)&0x7FFF;
	ma=BGBDT_XF128_GetMantissaX(a);
	mb=BGBDT_XF128_GetMantissaX(b);
	BGBDT_XI128_MulwX(ma, mb, &mcl, &mch);
	exc=exa+exb-0x3FFE;
	BGBDT_XF128_FixupMantissaWideX(mch, mcl, &mc, &exc);

	mc.d=(mc.d&0x0000FFFF)|
		(exc<<16)|(sgn<<31);
	return(mc);
}

BGBDTC_X128 BGBDT_XF128_RcpX(BGBDTC_X128 a)
{
//	return(BGBDT_XF128_AddX(a,
//		BGBDT_XF128_NegX(b)));
}

BGBDTC_X128 BGBDT_XF128_DivX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	return(BGBDT_XF128_MulX(a,
		BGBDT_XF128_RcpX(b)));
}

BTEIFGL_API int BGBDT_XI128_IsFloat128P(dtVal objv)
	{ return(dtvCheckPtrTagP(objv, objty_f128)); }

BTEIFGL_API int BGBDT_XI128_IsSmallFloat128P(dtVal objv)
{
	if(dtvCheckPtrTagP(objv, objty_f128))
		return(1);
	if(dtvIsSmallDoubleP(objv))
		return(1);
	if(BGBDT_XI128_IsInt128P(objv))
		return(1);
	return(0);
}

BTEIFGL_API dtVal BGBDT_XF128_WrapFloat128(BGBDTC_X128 a)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XF128_AllocFloat128();
	*xp=a;
	return(dtvWrapTagPtrF(xp, objty_f128));
}

BTEIFGL_API dtVal BGBDT_XF128_WrapFloat128L2(u64 lo, u64 hi)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XV128_AllocFloat128();
	xp->a=(u32)(lo    );
	xp->b=(u32)(lo>>32);
	xp->c=(u32)(hi    );
	xp->d=(u32)(hi>>32);
	return(dtvWrapTagPtrF(xp, objty_f128));
}

BTEIFGL_API BGBDTC_X128 BGBDT_XI128_UnwrapFloat128(dtVal vec)
{
	BGBDTC_X128 *xp;
	double f;
	
	if(dtvCheckPtrTagP(vec, objty_f128))
	{
		xp=dtvUnwrapPtrF(vec);
		return(*xp);
	}

	if(BGBDT_XI128_IsInt128P(vec))
	{
		xp=dtvUnwrapPtrF(vec);
		return(BGBDT_XF128_FromInt128(*xp));
	}
	
	f=dtvUnwrapDouble(vec);
	return(BGBDT_XF128_FromDouble(li));
}


dtVal BGBDT_XF128_Add(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XI128_IsSmallFloat128P(a) && BGBDT_XI128_IsSmallFloat128P(b))
	{
		xa=BGBDT_XF128_UnwrapFloat128(a);
		xb=BGBDT_XF128_UnwrapFloat128(b);
		xc=BGBDT_XF128_AddX(xa, xb);
		return(BGBDT_XI128_WrapInt128(xc));
	}
	
	return(DTV_UNDEFINED);
}
