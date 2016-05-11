#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long u64;
typedef signed long long s64;

typedef unsigned int u32;
typedef signed int s32;


float fcn_sin(float x)
	{ return(sin(x)); }
	
float chop_ident(float x)
	{ return(x); }

float chop_q2(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=(li>>2);
	lj=li<<2;
	f=*(float *)(&lj);
	return(f);
}

float chop_q3(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=(li>>3);
	lj=li<<3;
	f=*(float *)(&lj);
	return(f);
}

float chop_q4(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=(li>>4);
	lj=li<<4;
	f=*(float *)(&lj);
	return(f);
}

float chop_q2r(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=((li+1)>>2);
	lj=li<<2;
	f=*(float *)(&lj);
	return(f);
}

float chop_q3r(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=((li+3)>>3);
	lj=li<<3;
	f=*(float *)(&lj);
	return(f);
}

float chop_q4r(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=((li+7)>>4);
	lj=li<<4;
	f=*(float *)(&lj);
	return(f);
}

float chop_q2r2(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=((li+2)>>2);
	lj=li<<2;
	f=*(float *)(&lj);
	return(f);
}

float chop_q3r2(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=((li+4)>>3);
	lj=li<<3;
	f=*(float *)(&lj);
	return(f);
}

float chop_q4r2(float x)
{
	float f;
	u32 li, lj;
	
	li=*(u32 *)(&x);
	li=((li+8)>>4);
	lj=li<<4;
	f=*(float *)(&lj);
	return(f);
}

float interchop(float min, float max,
	float (*fcn)(float x),
	float (*chop)(float x))
{
	float f, g, h;
	float x, s, y;
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
	float (*chop)(float x);
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
	float m, n, ref;
	float f, g, h;
	s64 li, lb;
	int i, j, k, bi, np;

	for(i=0; i<16; i++)
	{
		best[i]=0;
		terr[i]=0;
	}

	np=64;
	for(i=0; i<np; i++)
	{
		m=(rand()/32768.0)*7;
		n=(rand()/32768.0)*7;
		
		lb=999999999; bi=0;
		
		ref=interchop(m, n, fcn_sin, chop_ident);
//		printf("min=%f max=%f ref=%f (%08X)\n",
//			m, n, ref, *(u32 *)(&ref));

		for(j=0; chfcn[j].name; j++)
		{
			f=interchop(m, n, fcn_sin, chfcn[j].chop);
			li=(*(s32 *)(&ref))-(*(s32 *)(&f));
			if(li<0)li=-li;
			if(li<lb) { lb=li; bi=j; }
			terr[j]+=li;
//			printf("%-8s val=%f (%08X), E=%d\n",
//				chfcn[j].name, f, *(u32 *)(&f), li);
		}

		best[bi]++;
		
//		printf("\n");
	}
	
	for(i=0; chfcn[i].name; i++)
	{
		printf("%-8s %d %d\n", chfcn[i].name, best[i], (int)(terr[i]/np));
	}
}