/** \file
 * Float128
 *
 * Implements logic for working with 128-bit floating-point numbers.
 * Makes extensive use of Int128.
 *
 * Format is IEEE754r Binary128
 */

// #include <bteifgl.h>

BGBDTC_X128 *bgbdt_xf128_freef128;
static int objty_f128=-1;

BS2VM_API BGBDTC_X128 *BGBDT_XF128_AllocFloat128(void)
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

BS2VM_API void BGBDT_XF128_FreeFloat128(BGBDTC_X128 *xp)
{
	*(BGBDTC_X128 **)xp=bgbdt_xf128_freef128;
	bgbdt_xf128_freef128=xp;
}



BS2VM_API BGBDTC_X128 BGBDT_XF128_FromInt128(BGBDTC_X128 a)
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

BS2VM_API BGBDTC_X128 BGBDT_XF128_FromLong(s64 a)
{
	BGBDTC_X128 fm;
	fm=BGBDT_XI128_FromLong(a);
	fm=BGBDT_XF128_FromInt128(fm);
	return(fm);
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_FromDouble(double v)
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

BS2VM_API double BGBDT_XF128_ToDouble(BGBDTC_X128 a)
{
	u64 la;
	int sgn, exp;

	sgn=(a.d>>31);
	exp=((a.d>>16)&0x7FFF)-(0x3FFF-0x3FF);
	if(exp<0)return(0.0);
	if(exp>0x7FE)
	{
		la=0x7FF0000000000000ULL|(((u64)sgn)<<63);
		return(*(double *)(&la));
	}
	
	la=(((u64)(a.d&0xFFFF))<<36)|(((u64)a.c)<<4)|
		((a.b+0x07FFFFFF)>>28)|(((u64)(exp|(sgn<<11)))<<52);
	return(*(double *)(&la));
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_ToInt128(BGBDTC_X128 a)
{
	BGBDTC_X128 fm, fi;
	int exp, sg, sh;

	fm=BGBDT_XF128_GetMantissaX(a);
	exp=BGBDT_XF128_GetExponentX(a);
	sg=(a.d>>31);
	
	sh=112-(exp-0x3FFF);
	fi=BGBDT_XI128_ShrlX(fm, sh);
	
	if(sg)
		{ fi=BGBDT_XI128_NegX(fi); }
	return(fi);
}

BS2VM_API void BGBDT_XF128_ToStringBuf(BGBDTC_X128 a,
	char *buf, int wfl)
{
	char tbi[256], tbf[256];
	BGBDTC_X128 fi, fm, fi2, fm2, fm3, c1b3, c1b4;
	u64 c1b, c1b2;
	int exp, e2, sg, wd;

	fm=BGBDT_XF128_GetMantissaX(a);
	exp=BGBDT_XF128_GetExponentX(a);
	sg=(a.d>>31);
	
	wd=wfl&255;
	
	if(!wd)wd=28;
	if(wd>36)wd=36;
	
	fi=BGBDT_XI128_ShrlX(fm, 112-(exp-0x3FFF));
	
	e2=16+(exp-0x3FFF);
	fm2=BGBDT_XI128_ShlrX(fm, e2);

	c1b=1000000000; c1b2=c1b*c1b;
	c1b3=BGBDT_XI128_FromLong(c1b2);
	c1b4=BGBDT_XI128_MulX(c1b3, c1b3);

	BGBDT_XI128_MulwX(fm2, c1b4, NULL, &fm3);

	if(sg)
		{ fi2=BGBDT_XI128_NegX(fi); }
	else
		{ fi2=fi; }

	BGBDT_XI128_ToStringBuf(fi2, tbi, 0);
	BGBDT_XI128_ToStringBuf(fm3, tbf, 36);
	tbf[wd]=0;
	sprintf(buf, "%s.%s", tbi, tbf);
}

BS2VM_API char *BGBDT_XF128_ToString(BGBDTC_X128 a)
{
	char tb[256];
	
	BGBDT_XF128_ToStringBuf(a, tb, 0);
	return(bgbdt_mm_rstrdup(tb));
}

BS2VM_API char *BGBDT_XF128_ToStringWd(BGBDTC_X128 a, int wfl)
{
	char tb[256];
	
	BGBDT_XF128_ToStringBuf(a, tb, wfl);
	return(bgbdt_mm_rstrdup(tb));
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_FromString(char *str)
{
	char tbi[256], tbf[256];
	BGBDTC_X128 fi, fm, fc, fr, mc, c10;
	s64 lc;
	char *s, *t;
	int sg, exc, exe;
	int i;
	
	s=str; sg=0;
	if(*s=='-')
		{ sg=1; s++; }
	
	t=tbi;
	while(*s && (*s!='.') && (*s!='E') && (*s!='e'))
		{ *t++=*s++; }
	*t++=0;
	if(*s=='.')
	{
		s++;
		t=tbf;
		while(*s && (*s!='E') && (*s!='e'))
			{ *t++=*s++; }
		*t++=0;
	}

	exe=0;
	if((*s=='E') || (*s=='e'))
		{ exe=atoi(s+1); }
	
	fi=BGBDT_XI128_FromStringD(tbi, 0);
//	fm=BGBDT_XI128_FromStringD(tbf, 27);

//	fc=BGBDT_XI128_FromLong2(0, 1ULL<<60);
//	fc=BGBDT_XI128_FromLong2(0, 1ULL<<62);
	fc=BGBDT_XI128_FromLong2(0, 1ULL<<63);
	fm=BGBDT_XI128_FromLong(0);

	c10=BGBDT_XI128_FromLong(10);

	s=tbf;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }
	
//		fc=BGBDT_XI128_DivX(fc,
//			BGBDT_XI128_FromLong(10));
		BGBDT_XI128_DivuXI(fc, c10, &fc, &fr);
//		if(fr.a>=3)
//			fc=BGBDT_XI128_AddIX(fc, 1);
//		fc=BGBDT_XI128_AddIX(fc, 10);

		fm=BGBDT_XI128_AddX(fm,
			BGBDT_XI128_MulX(fc,
			BGBDT_XI128_FromLong((*s++)-'0')));
	}
	
//	if(1)
//	{
//		fm=BGBDT_XI128_AddX(fm,
//			BGBDT_XI128_FromLong(8507));

//		fm=BGBDT_XI128_AddX(fm,
//			BGBDT_XI128_FromLong(16384));
//	}
	
//	fm=BGBDT_XI128_ShlX(fm, 4);
//	fm=BGBDT_XI128_ShlX(fm, 2);
	fm=BGBDT_XI128_Shl1X(fm);

//	lc=340282366921LL;
//	fm=BGBDT_XI128_MulX(fm,
//		BGBDT_XI128_FromLong(lc));
//	fm=BGBDT_XI128_FromLong(0);

	exc=16383+112;
	BGBDT_XF128_FixupMantissaWideX(fi, fm, &mc, &exc);

	mc.d=(mc.d&0x0000FFFF)|
		(exc<<16)|(sg<<31);
	
	if(exe)
	{
		fc=BGBDT_XF128_Exp10x(exe);
		mc=BGBDT_XF128_MulX(mc, fc);
	}
			
	return(mc);
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_Exp10x(int exe)
{
	BGBDTC_X128 mc, fc;
	s64 lc;
	int i;
	
	mc=BGBDT_XF128_FromLong(1);
	if(exe>0)
	{
		i=exe;
		if(i>=18)
		{
			fc=BGBDT_XF128_FromLong(1000000000000000000LL);
			while(i>=18)
				{ mc=BGBDT_XF128_MulX(mc, fc); i-=18; }
		}

		lc=1;
		while(i>0)
			{ lc=lc*10; i--; }
		fc=BGBDT_XF128_FromLong(lc);
		mc=BGBDT_XF128_MulX(mc, fc);

#if 0
		if(i>=3)
		{
			fc=BGBDT_XF128_FromLong(1000);
			while(i>=3)
				{ mc=BGBDT_XF128_MulX(mc, fc); i-=3; }
		}
		fc=BGBDT_XF128_FromLong(10);
		while(i>0)
			{ mc=BGBDT_XF128_MulX(mc, fc); i--; }
#endif
	}else if(exe<0)
	{
		i=-exe;
		if(i>=18)
		{
			fc=BGBDT_XF128_FromLong(1000000000000000000LL);
			fc=BGBDT_XF128_RcpX(fc);
			while(i>=18)
				{ mc=BGBDT_XF128_MulX(mc, fc); i-=18; }
		}

		lc=1;
		while(i>0)
			{ lc=lc*10; i--; }
		fc=BGBDT_XF128_FromLong(lc);
		fc=BGBDT_XF128_RcpX(fc);
		mc=BGBDT_XF128_MulX(mc, fc);

#if 0
		if(i>=3)
		{
			fc=BGBDT_XF128_FromLong(1000);
			fc=BGBDT_XF128_RcpX(fc);
			while(i>=3)
				{ mc=BGBDT_XF128_MulX(mf, fc); i-=3; }
		}
		fc=BGBDT_XF128_FromLong(10);
		fc=BGBDT_XF128_RcpX(fc);
		while(i>0)
			{ mc=BGBDT_XF128_MulX(mc, fc); i--; }
#endif
	}
	return(mc);
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_NegX(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c=a;
	c.d^=0x80000000U;
	return(c);
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_GetMantissaX(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=a.a;	c.b=a.b;
	c.c=a.c;	c.d=(a.d&0x0000FFFF)|0x00010000;
	return(c);
}

BS2VM_API int BGBDT_XF128_GetExponentX(BGBDTC_X128 a)
{
	return((a.d>>16)&0x7FFF);
}

BS2VM_API int BGBDT_XF128_FixupMantissaNarrowX(
	BGBDTC_X128 a, BGBDTC_X128 *rc, int *rexp)
{
	BGBDTC_X128 c, d;
	int exp;
	int i, j, k;
	
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

		if(!(c.d>>8))
		{
			exp-=8;
			c=BGBDT_XI128_Shl8X(c);
		}

#if 1
		if(!(c.d>>16))
//		if(!(c.d>>15))
		{
			i=1; k=c.d<<1;
			while(!(k>>12))
				{ i+=4; k=k<<4; }
			while(!(k>>16))
				{ i++; k=k<<1; }
			exp-=i;
			c=BGBDT_XI128_ShlpX(c, i);
		}
#if 0
		else if(!(c.d>>16))
		{
			exp--;
			c=BGBDT_XI128_Shl1X(c);
		}
#endif
#endif

#if 0
		while(!(c.d>>16))
		{
			exp--;
			c=BGBDT_XI128_Shl1X(c);
		}
#endif
	}
	
	*rc=c;
	*rexp=exp;
	return(0);
}

BS2VM_API int BGBDT_XF128_FixupMantissaWideX(
	BGBDTC_X128 a, BGBDTC_X128 b, BGBDTC_X128 *rc, int *rexp)
{
	BGBDTC_X128 c, d;
	int exp;
	int i, j, k;
	
	c=a; d=b;
	exp=*rexp;

	if(c.d>>16)
	{
		while((c.d>>16)>1)
		{
			exp++;
			c=BGBDT_XI128_AddIX(c, 1);
			c=BGBDT_XI128_Shr1X(c);
		}
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
//			d.d=d.d<<16;
		}
		
		if(!(c.d>>8))
		{
			exp-=8;
			c=BGBDT_XI128_Shl8X(c);
			c.a=c.a|(d.d>>24);
//			d=BGBDT_XI128_Shl8X(d);
			d.d=d.d<<8;
		}

#if 1
		if(!(c.d>>16))
		{
			i=1; k=c.d<<1;
			while(!(k>>12))
				{ i+=4; k=k<<4; }
			while(!(k>>16))
				{ i++; k=k<<1; }
			exp-=i;
			c=BGBDT_XI128_ShlpX(c, i);
			c.a=c.a|(d.d>>(32-i));
			d.d=d.d<<i;
		}
#endif


#if 0
//		while(!(c.d>>12))
		if(!(c.d>>12))
		{
			exp-=4;
			c=BGBDT_XI128_Shl4X(c);
			c.a=c.a|(d.d>>28);
//			d=BGBDT_XI128_Shl4X(d);
			d.d=d.d<<4;
		}

		if(!(c.d>>14))
		{
			exp-=2;
			c=BGBDT_XI128_Shl2X(c);
			c.a=c.a|(d.d>>30);
//			d=BGBDT_XI128_Shl2X(d);
			d.d=d.d<<2;
		}

//		while(!(c.d>>16))
		if(!(c.d>>16))
		{
			exp--;
			c=BGBDT_XI128_Shl1X(c);
			c.a=c.a|(d.d>>31);
//			d=BGBDT_XI128_Shl1X(d);
			d.d=d.d<<1;
		}
#endif

		if(d.d>>31)
			c=BGBDT_XI128_AddIX(c, 1);
	}
	
	*rc=c;
	*rexp=exp;
	return(0);
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_AddX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	BGBDTC_X128 ma, mb, mcl, mch, mc;
	int exa, exb, exc, sgn, sga, sgb, sgc;
	
	u64 t0, t1, t2, t3;
	BGBDTC_X128 l0, l1, l2;
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
				sgc=sga;
				exc=exa+1;
				mc=l1;
			}
		}else
		{
			i=exa-exb;
			if(i>108)return(a);
			l0=BGBDT_XI128_ShrX(mb, i+1);
			l1=BGBDT_XI128_Shr1X(ma);
			l2=BGBDT_XI128_AddX(l1, l0);

			sgc=sga;
			exc=exa+1;
			mc=l2;
		}
	}else
	{
		if(sga^sgb)
		{
			i=exb-exa;
			if(i>108)return(b);
			l0=BGBDT_XI128_ShrX(ma, i+1);
			l1=BGBDT_XI128_Shr1X(mb);
			l1=BGBDT_XI128_SubX(l1, l0);

			if(BGBDT_XI128_NegXP(l1))
			{
				sgc=!sgb;
				exc=exb+1;
				mc=BGBDT_XI128_NegX(l1);
			}else
			{
				sgc=sgb;
				exc=exb+1;
				mc=l1;
			}
		}else
		{
			i=exb-exa;
			if(i>108)return(b);
			l0=BGBDT_XI128_ShrX(ma, i+1);
			l1=BGBDT_XI128_Shr1X(mb);
			l2=BGBDT_XI128_AddX(l1, l0);

			sgc=sgb;
			exc=exb+1;
			mc=l2;
		}
	}

	if(BGBDT_XI128_ZeroXP(mc))
		{ return(mc); }

	BGBDT_XF128_FixupMantissaNarrowX(mc, &mc, &exc);

	mc.d=(mc.d&0x0000FFFF)|
		(exc<<16)|(sgc<<31);
	return(mc);
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_SubX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	return(BGBDT_XF128_AddX(a,
		BGBDT_XF128_NegX(b)));
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_MulX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 ma, mb, mcl, mch, mc;
	int exa, exb, exc, sgn;
	
	sgn=(a.d^b.d)>>31;
	exa=(a.d>>16)&0x7FFF;
	exb=(b.d>>16)&0x7FFF;
	ma=BGBDT_XF128_GetMantissaX(a);
	mb=BGBDT_XF128_GetMantissaX(b);
	BGBDT_XI128_MulwX(ma, mb, &mcl, &mch);
//	exc=exa+exb-0x3FFE+15;
//	BGBDT_XF128_FixupMantissaWideX(mch, mcl, &mc, &exc);

//	mch=BGBDT_XI128_ShlpX(mch, 15);
//	mch.a=mch.a|(mcl.d>>(32-15));
//	exc=exa+exb-0x3FFE+0;
//	BGBDT_XF128_FixupMantissaNarrowX(mch, &mc, &exc);

//	mch=BGBDT_XI128_ShlpX(mch, 16);
	mch=BGBDT_XI128_Shl16X(mch);
	mch.a=mch.a|(mcl.d>>(32-16));
	exc=exa+exb-0x3FFF;
	BGBDT_XF128_FixupMantissaNarrowX(mch, &mc, &exc);

	mc.d=(mc.d&0x0000FFFF)|
		(exc<<16)|(sgn<<31);
	return(mc);
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_RcpX(BGBDTC_X128 a)
{
	BGBDTC_X128 fm, fah, fal, fq, mc;
	int sg, ex, ex2;

#if 1
	sg=BGBDT_XI128_NegXP(a);
	ex=BGBDT_XF128_GetExponentX(a);
	fm=BGBDT_XF128_GetMantissaX(a);

	fal=BGBDT_XI128_FromLong(0);
	fah=BGBDT_XI128_FromLong2(0, 1LL<<32);

	BGBDT_XI128_DivW1XI(fal, fah, fm, &fq, NULL);
	ex2=(2*16383)-ex;

	BGBDT_XF128_FixupMantissaNarrowX(fq, &mc, &ex2);

	mc.d=(mc.d&0x0000FFFF)|
		(ex2<<16)|(sg<<31);
	return(mc);
#endif

#if 0
	BGBDTC_X128 c;
	double f;

	//HACK: use double for RCP
	//FIXME: do proper 128-bit RCP
	f=BGBDT_XF128_ToDouble(a);
	if(f!=0)f=1.0/f;
	c=BGBDT_XF128_FromDouble(f);
	return(c);
#endif
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_DivX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	return(BGBDT_XF128_MulX(a,
		BGBDT_XF128_RcpX(b)));
}

BS2VM_API int BGBDT_XF128_CmpX(BGBDTC_X128 a, BGBDTC_X128 b, int ord)
{
	BGBDTC_X128 ma, mb, mcl, mch, mc;
	int exa, exb, exc, sgn, sga, sgb, sgc;
	
	u64 t0, t1, t2, t3;
	BGBDTC_X128 l0, l1, l2;
	int i;

	sga=BGBDT_XI128_NegXP(a);
	sgb=BGBDT_XI128_NegXP(b);
		
	exa=BGBDT_XF128_GetExponentX(a);
	exb=BGBDT_XF128_GetExponentX(b);

	if((exa==0x7FFF) || (exb==0x7FFF))
	{

		if(exa==0x7FFF)
		{
			ma=BGBDT_XF128_GetMantissaX(a);
			if(BGBDT_XI128_ZeroXP(ma)) /* Inf */
				{ return(sga?(-1):1); }
			return(ord?2:(-2));
		}else
		{
			mb=BGBDT_XF128_GetMantissaX(b);
			if(BGBDT_XI128_ZeroXP(mb)) /* Inf */
				{ return(sgb?(-1):1); }
			return(ord?2:(-2));
		}
	}

	if(sga!=sgb)
		{ return(sga?(-1):1); }

	if(exa!=exb)
		{ return((exa>exb)?1:(-1)); }

	ma=BGBDT_XF128_GetMantissaX(a);
	mb=BGBDT_XF128_GetMantissaX(b);
	i=BGBDT_XI128_CmpX(ma, mb);
	return(i);
}

BS2VM_API int BGBDT_XF128_IsFloat128P(dtVal objv)
	{ return(dtvCheckPtrTagP(objv, objty_f128)); }

BS2VM_API int BGBDT_XF128_IsSmallFloat128P(dtVal objv)
{
	if(dtvCheckPtrTagP(objv, objty_f128))
		return(1);
	if(dtvIsSmallDoubleP(objv))
		return(1);
	if(BGBDT_XI128_IsInt128P(objv))
		return(1);
	return(0);
}

BS2VM_API dtVal BGBDT_XF128_WrapFloat128(BGBDTC_X128 a)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XF128_AllocFloat128();
	*xp=a;
	return(dtvWrapTagPtrF(xp, objty_f128));
}

BS2VM_API dtVal BGBDT_XF128_WrapFloat128L2(u64 lo, u64 hi)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XF128_AllocFloat128();
	xp->a=(u32)(lo    );
	xp->b=(u32)(lo>>32);
	xp->c=(u32)(hi    );
	xp->d=(u32)(hi>>32);
	return(dtvWrapTagPtrF(xp, objty_f128));
}

BS2VM_API BGBDTC_X128 BGBDT_XF128_UnwrapFloat128(dtVal vec)
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
	return(BGBDT_XF128_FromDouble(f));
}


BS2VM_API dtVal BGBDT_XF128_Add(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;

	if(BGBDT_XF128_IsSmallFloat128P(a) && BGBDT_XF128_IsSmallFloat128P(b))
	{
		xa=BGBDT_XF128_UnwrapFloat128(a);
		xb=BGBDT_XF128_UnwrapFloat128(b);
		xc=BGBDT_XF128_AddX(xa, xb);
		return(BGBDT_XF128_WrapFloat128(xc));
	}
	
	return(DTV_UNDEFINED);
}
