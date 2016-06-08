#include <bteifgl.h>

dtVal BGBDT_TagAri_Add(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(li+lj);
		return(c);
	}

	if(dtvIsSmallDoubleP(a) && dtvIsSmallDoubleP(b))
	{
		f=dtvUnwrapDouble(a);
		g=dtvUnwrapDouble(b);
		c=dtvWrapDouble(f+g);
		return(c);
	}
	
	c=BGBDT_XV128_Add(a, b);
	if(!dtvUndefinedP(c))
		return(c);
	
	c=BGBDT_XI128_Add(a, b);
	if(!dtvUndefinedP(c))
		return(c);
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Sub(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(li-lj);
		return(c);
	}

	if(dtvIsSmallDoubleP(a) && dtvIsSmallDoubleP(b))
	{
		f=dtvUnwrapDouble(a);
		g=dtvUnwrapDouble(b);
		c=dtvWrapDouble(f-g);
		return(c);
	}
	
	c=BGBDT_XV128_Sub(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	c=BGBDT_XI128_Sub(a, b);
	if(!dtvUndefinedP(c))
		return(c);
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Mul(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(li*lj);
		return(c);
	}

	if(dtvIsSmallDoubleP(a) && dtvIsSmallDoubleP(b))
	{
		f=dtvUnwrapDouble(a);
		g=dtvUnwrapDouble(b);
		c=dtvWrapDouble(f*g);
		return(c);
	}
	
	c=BGBDT_XV128_Mul(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	c=BGBDT_XI128_Mul(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Div(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		if(!lj)return(DTV_NULL);
		c=dtvWrapLong(li/lj);
		return(c);
	}

	if(dtvIsSmallDoubleP(a) && dtvIsSmallDoubleP(b))
	{
		f=dtvUnwrapDouble(a);
		g=dtvUnwrapDouble(b);
		if(g==0)return(DTV_NULL);
		c=dtvWrapDouble(f/g);
		return(c);
	}
	
	c=BGBDT_XV128_Div(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	c=BGBDT_XI128_Div(a, b);
	if(!dtvUndefinedP(c))
		return(c);
	
	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Mod(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		if(!lj)return(DTV_NULL);
		c=dtvWrapLong(li%lj);
		return(c);
	}

	c=BGBDT_XI128_Mod(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_And(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(li&lj);
		return(c);
	}

	c=BGBDT_XI128_And(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Or(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(li|lj);
		return(c);
	}

	c=BGBDT_XI128_Or(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Xor(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(li^lj);
		return(c);
	}

	c=BGBDT_XI128_Xor(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Shl(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(li<<lj);
		return(c);
	}

	c=BGBDT_XI128_Shl(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Shr(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(((u64)li)>>lj);
		return(c);
	}

	c=BGBDT_XI128_Shr(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Sar(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		c=dtvWrapLong(li>>lj);
		return(c);
	}

	c=BGBDT_XI128_Sar(a, b);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Neg(dtVal a)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a))
	{
		li=dtvUnwrapLong(a);
		c=dtvWrapLong(-li);
		return(c);
	}

	if(dtvIsSmallDoubleP(a))
	{
		f=dtvUnwrapDouble(a);
		c=dtvWrapDouble(-f);
		return(c);
	}

	c=BGBDT_XI128_Neg(a);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_Not(dtVal a)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a))
	{
		li=dtvUnwrapLong(a);
		c=dtvWrapLong(~li);
		return(c);
	}

	c=BGBDT_XI128_Not(a);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

dtVal BGBDT_TagAri_LNot(dtVal a)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a))
	{
		li=dtvUnwrapLong(a);
		c=dtvWrapInt(li==0);
		return(c);
	}

	if(dtvIsSmallDoubleP(a))
	{
		f=dtvUnwrapDouble(a);
		c=dtvWrapInt(f==0);
		return(c);
	}

	c=BGBDT_XI128_LNot(a);
	if(!dtvUndefinedP(c))
		return(c);

	return(DTV_UNDEFINED);
}

int BGBDT_TagAri_Compare(dtVal a, dtVal b)
{
	double f, g, h;
	s64 li, lj, lk;
	dtVal c;

	if(dtvIsSmallLongP(a) && dtvIsSmallLongP(b))
	{
		li=dtvUnwrapLong(a);
		lj=dtvUnwrapLong(b);
		return((li>lj)|(0-(li<lj)));
	}

	if(dtvIsSmallDoubleP(a) && dtvIsSmallDoubleP(b))
	{
		f=dtvUnwrapDouble(a);
		g=dtvUnwrapDouble(b);
		return((f>g)|(0-(f<g)));
	}
	
	if(BGBDT_XI128_IsSmallInt128P(a) &&
		BGBDT_XI128_IsSmallInt128P(b))
	{
		return(BGBDT_XI128_CompareB(a, b));
	}

	return(-1);
}
