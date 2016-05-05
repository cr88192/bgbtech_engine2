#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long u64;
typedef signed long long s64;


double fcn_sin(double x)
	{ return(sin(x)); }
	
double chop_ident(double x)
	{ return(x); }

double chop_q2(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=(li>>2);
	lj=li<<2;
	f=*(double *)(&lj);
	return(f);
}

double chop_q3(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=(li>>3);
	lj=li<<3;
	f=*(double *)(&lj);
	return(f);
}

double chop_q4(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=(li>>4);
	lj=li<<4;
	f=*(double *)(&lj);
	return(f);
}

double chop_q2r(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=((li+1)>>2);
	lj=li<<2;
	f=*(double *)(&lj);
	return(f);
}

double chop_q3r(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=((li+3)>>3);
	lj=li<<3;
	f=*(double *)(&lj);
	return(f);
}

double chop_q4r(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=((li+7)>>4);
	lj=li<<4;
	f=*(double *)(&lj);
	return(f);
}

double chop_q2r2(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=((li+2)>>2);
	lj=li<<2;
	f=*(double *)(&lj);
	return(f);
}

double chop_q3r2(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=((li+4)>>3);
	lj=li<<3;
	f=*(double *)(&lj);
	return(f);
}

double chop_q4r2(double x)
{
	double f;
	u64 li, lj;
	
	li=*(u64 *)(&x);
	li=((li+8)>>4);
	lj=li<<4;
	f=*(double *)(&lj);
	return(f);
}

double interchop(double min, double max,
	double (*fcn)(double x),
	double (*chop)(double x))
{
	double f, g, h;
	double x, s, y;
	int i, n;
	
	n=65536;
	s=(max-min)/n;
	x=min;

	s=chop(s);

	y=0;
	for(i=0; i<n; i++)
	{
		f=fcn(x);
		g=chop(f);
		x=x+s;
		y=y+g;
		x=chop(x);
		y=chop(y);
	}

//	y=chop(y);
	return(y);
}

int main()
{
	struct {
	char *name;
	double (*chop)(double x);
	}chfcn[]={
	{"chop2u", chop_q2},
	{"chop3u", chop_q3},
	{"chop4u", chop_q4},
	{"chop2r1", chop_q2r},
	{"chop3r1", chop_q3r},
	{"chop4r1", chop_q4r},
	{"chop2r2", chop_q2r2},
	{"chop3r2", chop_q3r2},
	{"chop4r2", chop_q4r2},
	{NULL, NULL}
	};
	
	int best[16];
	u64 terr[16];
	double m, n, ref;
	double f, g, h;
	s64 li, lb;
	int i, j, k, bi, np;

	for(i=0; i<16; i++)
	{
		best[i]=0;
		terr[i]=0;
	}

	np=8;
	for(i=0; i<np; i++)
	{
		m=(rand()/32768.0)*7;
		n=(rand()/32768.0)*7;
		
		lb=999999999; bi=0;
		
		ref=interchop(m, n, fcn_sin, chop_ident);
		printf("min=%f max=%f ref=%f (%I64X)\n",
			m, n, ref, *(u64 *)(&ref));

		for(j=0; chfcn[j].name; j++)
		{
			f=interchop(m, n, fcn_sin, chfcn[j].chop);
			li=(*(s64 *)(&ref))-(*(s64 *)(&f));
			if(li<0)li=-li;
			if(li<lb) { lb=li; bi=j; }
			terr[j]+=li;
			printf("%-8s val=%f (%I64X), E=%I64d\n",
				chfcn[j].name, f, *(u64 *)(&f), li);
		}

		best[bi]++;
		
		printf("\n");
	}
	
	for(i=0; chfcn[i].name; i++)
	{
		printf("%-8s %d %I64d\n", chfcn[i].name, best[i], terr[i]/np);
	}
}