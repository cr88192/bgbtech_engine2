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
	
	return(-1);
}
