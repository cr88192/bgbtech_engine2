/** \file
 * Int128
 *
 * Implements logic for working with 128-bit integers.
 *
 * Integers are (presumably) in two's complement little-endian ordering.
 */

// #include <bteifgl.h>

BGBDTC_X128 *bgbdt_xi128_freesi128;
// BGBDTC_X128 *bgbdt_xi128_freeui128;

static int objty_si128=-1;
// static int objty_ui128=-1;
// static int objty_f128=-1;

/** Allocate Int128 value from pool. */
BS2VM_API BGBDTC_X128 *BGBDT_XI128_AllocInt128(void)
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

/** Free allocated int128 value to pool. */
BS2VM_API void BGBDT_XI128_FreeInt128(BGBDTC_X128 *xp)
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

/** Returns true if tagged value contains an int128. */
BS2VM_API bool BGBDT_XI128_IsInt128P(dtVal objv)
	{ return(dtvCheckPtrTagP(objv, objty_si128)); }

/** Returns true if tagged value contains an int128.
  * Will also return true if the value promotes to int128.
  */
BS2VM_API bool BGBDT_XI128_IsSmallInt128P(dtVal objv)
{
	if(dtvCheckPtrTagP(objv, objty_si128))
		return(1);
	if(dtvIsSmallLongP(objv))
		return(1);
	return(0);
}

/** Compose an int128 value from a 64-bit long (s64) */
BS2VM_API BGBDTC_X128 BGBDT_XI128_FromLong(s64 li)
{
	BGBDTC_X128 c;
	
	c.a=(u32)li;
	c.b=(u32)(li>>32);
	c.c=(u32)(li>>63);
	c.d=(u32)(li>>63);
	return(c);
}

/** Compose an int128 value from a pair of 64-bit longs (u64) */
BS2VM_API BGBDTC_X128 BGBDT_XI128_FromLong2(u64 lo, u64 hi)
{
	BGBDTC_X128 c;
	
	c.a=(u32)(lo    );
	c.b=(u32)(lo>>32);
	c.c=(u32)(hi    );
	c.d=(u32)(hi>>32);
	return(c);
}

/** Get low order bits of int128 as a long (s64) */
BS2VM_API s64 BGBDT_XI128_ToLong(BGBDTC_X128 a)
{
	s64 li;
	li=a.b;
	li=(li<<32)|a.a;
	return(li);
}

/** Dump int128 value to a string (as a hex number). */
BS2VM_API char *BGBDT_XI128_DumpString(BGBDTC_X128 a)
{
	char tb[256];
	sprintf(tb, "0x%08X_%08X_%08X_%08X", a.d, a.c, a.b, a.a);
	return(bgbdt_mm_rstrdup(tb));
}

/** Print int128 to a string buffer */
BS2VM_API void BGBDT_XI128_ToStringBuf(
	BGBDTC_X128 a, char *buf, int wfl)
{
	char tb1[256], tb2[256];
	BGBDTC_X128 c10;
	BGBDTC_X128 c1b;
	BGBDTC_X128 b, br, bq;
	u32 ba;
	char *s, *t, *te;
	int wd, pa;
	int i, j, k, n, sg;

//	sprintf(tb, "0x%08X_%08X_%08X_%08X", a.d, a.c, a.b, a.a);

	wd=wfl&255; pa='0';
	if(wfl&0x100)pa=' ';

	if(BGBDT_XI128_ZeroXP(a))
	{
		t=buf; te=t+wd;
		*t++='0';
		while(t<te)
			*t++=pa;
		*t++=0;
		return;
	}

	b=a; sg=0;
	if(BGBDT_XI128_NegXP(b))
		{ b=BGBDT_XI128_NegX(b); sg=1; }

//	c10=BGBDT_XI128_FromLong(10);
	c1b=BGBDT_XI128_FromLong(1000000000);

	t=tb1; te=tb1+64;
	while(!BGBDT_XI128_ZeroXP(b) && (t<te))
	{
//		br=BGBDT_XI128_ModX(b, c1b);
		BGBDT_XI128_DivXI(b, c1b, &bq, &br);

		if(!BGBDT_XI128_ZeroXP(bq))
		{
			ba=br.a;
			for(i=0; i<9; i++)
			{
				j=ba%10; ba=ba/10;
				*t++='0'+j;
			}
			b=bq;
			continue;
		}else
		{
			ba=br.a;
			for(i=0; (ba!=0); i++)
			{
				j=ba%10; ba=ba/10;
				*t++='0'+j;
			}
			break;
		}
	}
	
	s=t;
	t=buf; te=buf+wd;
	if(sg)*t++='-';
	while(s>tb1)
		{ *t++=*(--s); }
	while(t<te)
		*t++=pa;
	*t++=0;
}

/** Print int128 to a string and return temp string. */
BS2VM_API char *BGBDT_XI128_ToString(BGBDTC_X128 a)
{
	char tb[256];
	
	BGBDT_XI128_ToStringBuf(a, tb, 0);
	return(bgbdt_mm_rstrdup(tb));
}

/** Parse int128 value from string */
BS2VM_API BGBDTC_X128 BGBDT_XI128_FromString(char *str)
{
	if(str[0]==0)
	{
		if(str[1]=='x')
			return(BGBDT_XI128_FromStringRx(str+2, 16));
		if(str[1]=='b')
			return(BGBDT_XI128_FromStringRx(str+2, 2));
		if(str[1]=='d')
			return(BGBDT_XI128_FromStringRx(str+2, 10));
		if((str[1]>='0') && (str[1]<='7'))
			return(BGBDT_XI128_FromStringRx(str+1, 8));
	}

	return(BGBDT_XI128_FromStringD(str, 0));
}

/** Parse int128 value from a string of decimal digits.
  * \param w If nonzero, number will be parsed as if w digits were present.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_FromStringD(char *str, int w)
{
	BGBDTC_X128 c;
	char *s;
	int tv, nv, sg, n;
	
	s=str; sg=0;
	if(*s=='-')
		{ sg=1; s++; }
	
	c=BGBDT_XI128_FromLong(0);
	tv=0; nv=1; n=0;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }
		if((*s<'0') || (*s>'9'))
			break;

		if(nv>=1000000000)
		{
			c=BGBDT_XI128_MulX(c,
				BGBDT_XI128_FromLong(nv));
			c=BGBDT_XI128_AddX(c,
				BGBDT_XI128_FromLong(tv));
			tv=0; nv=1;
			continue;
		}
		tv=(tv*10)+((*s++)-'0');
		nv=nv*10; n++;
	}
	c=BGBDT_XI128_MulX(c, BGBDT_XI128_FromLong(nv));
	c=BGBDT_XI128_AddX(c, BGBDT_XI128_FromLong(tv));

	if(n<w)
	{
		while((n+9)<=w)
		{
			c=BGBDT_XI128_MulX(c,
				BGBDT_XI128_FromLong(1000000000));
			n+=9;
		}
		while((n+3)<=w)
		{
			c=BGBDT_XI128_MulX(c,
				BGBDT_XI128_FromLong(1000));
			n+=3;
		}
		while(n<w)
		{
			c=BGBDT_XI128_MulX(c,
				BGBDT_XI128_FromLong(10));
			n++;
		}
	}
	
	if(sg)
		c=BGBDT_XI128_NegX(c);
	return(c);
}

/** Parse from a string of digits with a given a radix.
  * Underscores in input are ignored.
  * /param rx Radix in the range of 2 to 36.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_FromStringRx(char *str, int rx)
{
	BGBDTC_X128 c;
	char *s;
	int tv, nv, sg, v, n;
	
	s=str; sg=0;
	c=BGBDT_XI128_FromLong(0);
	tv=0; nv=1; n=0;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }

		if(nv>=(1<<24))
		{
			c=BGBDT_XI128_MulX(c,
				BGBDT_XI128_FromLong(nv));
			c=BGBDT_XI128_AddX(c,
				BGBDT_XI128_FromLong(tv));
			tv=0; nv=1;
			continue;
		}

		v=-1;
		if((*s>='0') && (*s<='9'))v=*s-'0';
		if((*s>='A') && (*s<='Z'))v=10+(*s-'A');
		if((*s>='a') && (*s<='z'))v=10+(*s-'a');
		if(v<0)break;
		if(v>=rx)break;
		
		s++;

		tv=(tv*rx)+v;
		nv=nv*rx; n++;
	}
	c=BGBDT_XI128_MulX(c, BGBDT_XI128_FromLong(nv));
	c=BGBDT_XI128_AddX(c, BGBDT_XI128_FromLong(tv));
	
	if(sg)
		c=BGBDT_XI128_NegX(c);
	return(c);
}

/** Add int128 value and an integer. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_AddIX(BGBDTC_X128 a, int b)
{
	BGBDTC_X128 c;
	int bs;
	u64 l;
	
	bs=b>>31;
	
	l=((u64)a.a)+b;
	c.a=(u32)l;
	l=((l>>32)+a.b)+bs;
	c.b=(u32)l;
	l=((l>>32)+a.c)+bs;
	c.c=(u32)l;
	l=((l>>32)+a.d)+bs;
	c.d=(u32)l;
	return(c);
}

/** Add pair of int128 values. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_AddX(BGBDTC_X128 a, BGBDTC_X128 b)
{
// #ifdef X86_MSVC
#if 0
//	BGBDTC_X128 c;
	__asm {
//		movdqu xmm0, [a]
//		movdqu [c], xmm0
		mov eax, [b+ 0]
		mov edx, [b+ 4]
		add [a+ 0], eax
		adc [a+ 4], edx
		mov eax, [b+ 8]
		mov edx, [b+12]
		adc [a+ 8], eax
		adc [a+12], edx
	}
	return(a);
#else
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
#endif
}

/** Negate int128 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_NegX(BGBDTC_X128 a)
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

/** Bitwise not of int128 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_NotX(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=~a.a;	c.b=~a.b;
	c.c=~a.c;	c.d=~a.d;
	return(c);
}

#if 0
BS2VM_API BGBDTC_X128 BGBDT_XI128_SubX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 nb, c;
	nb=BGBDT_XI128_NegX(b);
	c=BGBDT_XI128_AddX(a, nb);
	return(c);
}
#endif

/** Subtract int128 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_SubX(BGBDTC_X128 a, BGBDTC_X128 b)
{
// #ifdef X86_MSVC
#if 0
//	BGBDTC_X128 c;
	__asm {
//		movdqu xmm0, [a]
//		movdqu [c], xmm0
		mov eax, [b+ 0]
		mov edx, [b+ 4]
		sub [a+ 0], eax
		sbb [a+ 4], edx
		mov eax, [b+ 8]
		mov edx, [b+12]
		sbb [a+ 8], eax
		sbb [a+12], edx
	}
	return(a);
#else
	BGBDTC_X128 c;
	u64 l;
	
	l=((u64)a.a)+(~b.a)+1;
	c.a=(u32)l;
	l=((l>>32)+a.b)+(~b.b);
	c.b=(u32)l;
	l=((l>>32)+a.c)+(~b.c);
	c.c=(u32)l;
	l=((l>>32)+a.d)+(~b.d);
	c.d=(u32)l;
	return(c);
#endif
}

#if 1
/**
  * Add int128 values given as pairs, each representing a 256 bit value.
  * /param aa Low 128 bits of left param.
  * /param ab High 128 bits of left param.
  * /param ba Low 128 bits of right param.
  * /param bb High 128 bits of right param.
  * /param rca Low 128 bits of result.
  * /param rcb High 128 bits of result.
  */
BS2VM_API void BGBDT_XI128_AddWX(
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

/**
  * Nagate int128 values given as a pair representing a 256 bit value.
  * /param aa Low 128 bits of input.
  * /param ab High 128 bits of input.
  * /param rca Low 128 bits of result.
  * /param rcb High 128 bits of result.
  */
BS2VM_API void BGBDT_XI128_NegWX(BGBDTC_X128 aa, BGBDTC_X128 ab,
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

/**
  * Bitwise not of int128 values given as a pair
  *   representing a 256 bit value.
  * /param aa Low 128 bits of input.
  * /param ab High 128 bits of input.
  * /param rca Low 128 bits of result.
  * /param rcb High 128 bits of result.
  */
BS2VM_API void BGBDT_XI128_NotWX(BGBDTC_X128 aa, BGBDTC_X128 ab,
	BGBDTC_X128 *rca, BGBDTC_X128 *rcb)
{
	*rca=BGBDT_XI128_NotX(aa);
	*rcb=BGBDT_XI128_NotX(ab);
}

/**
  * Subtract int128 values given as pairs representing 256 bit values.
  * /param aa Low 128 bits of left param.
  * /param ab High 128 bits of left param.
  * /param ba Low 128 bits of right param.
  * /param bb High 128 bits of right param.
  * /param rca Low 128 bits of result.
  * /param rcb High 128 bits of result.
  */
BS2VM_API void BGBDT_XI128_SubWX(
	BGBDTC_X128 aa, BGBDTC_X128 ab,
	BGBDTC_X128 ba, BGBDTC_X128 bb,
	BGBDTC_X128 *rca, BGBDTC_X128 *rcb)
{
	BGBDTC_X128 nba, nbb, c;
	BGBDT_XI128_NegWX(ba, bb, &nba, &nbb);
	BGBDT_XI128_AddWX(aa, ab, nba, nbb, rca, rcb);
}
#endif

/** Multiply pair of int128 values. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_MulX(BGBDTC_X128 a, BGBDTC_X128 b)
{
// #ifdef X86_MSVC
#if 0
	BGBDTC_X128 c;
	u64 l;

	__asm {

#if 1
	mov esi, [a+0]
	mov edi, [b+0]

	mov eax, esi
	mul edi
	mov [c+0], eax
	mov ecx, edx

	mov eax, [a+4]
	mul edi
	add ecx, eax
	mov ebx, edx

	mov eax, esi
	mul dword ptr [b+4]
	add ecx, eax
	add ebx, edx

	mov [c+4], ecx
#endif

#if 1
	mov edi, [b+0]

	mov eax, [a+8]
	mul edi
	add ebx, eax
	mov ecx, edx
	
	mov eax, [a+12]
	mul edi
	add ecx, eax

	mov esi, [b+4]

	mov eax, [a+4]
	mul esi
	add ebx, eax
	add ecx, edx
	mov eax, [a+8]
	mul esi
	add ecx, eax


	mov esi, [a+0]
	mov edi, [b+8]

	mov eax, esi
	mul edi
	add ebx, eax
	add ecx, edx

	mov eax, [a+4]
	mul edi
	add ecx, eax

	mov eax, esi
	mul dword ptr [b+12]
	add ecx, eax

	mov [c+8], ebx
	mov [c+12], ecx
#endif

#if 0
	mov eax, [a+0]
	mul dword ptr [b+0]
	mov [c+0], eax
	mov ecx, edx

	mov eax, [a+4]
	mul dword ptr [b+0]
	add ecx, eax
	mov ebx, edx

	mov eax, [a+0]
	mul dword ptr [b+4]
	add ecx, eax
	add ebx, edx

	mov [c+4], ecx
#endif

#if 0
	mov eax, [a+8]
	mul dword ptr [b+0]
	add ebx, eax
	mov ecx, edx

	mov eax, [a+4]
	mul dword ptr[b+4]
	add ebx, eax
	add ecx, edx

	mov eax, [a+0]
	mul dword ptr [b+8]
	add ebx, eax
	add ecx, edx

	mov [c+8], ebx


	mov eax, [a+12]
	mul dword ptr [b+0]
	add ecx, eax

	mov eax, [a+8]
	mul dword ptr [b+4]
	add ecx, eax

	mov eax, [a+4]
	mul dword ptr [b+8]
	add ecx, eax

	mov eax, [a+0]
	mul dword ptr [b+12]
	add ecx, eax

	mov [c+12], ecx
#endif
	}
	
#if 0
	l=((u64)a.a)*b.a;
	c.a=(u32)l;
	l=(l>>32)+(((u64)a.b)*b.a)+(((u64)a.a)*b.b);
	c.b=(u32)l;
	l=(l>>32)+(((u64)a.c)*b.a)+(((u64)a.a)*b.c)+(((u64)a.b)*b.b);
	c.c=(u32)l;
	l=(l>>32)+(((u64)a.d)*b.a)+(((u64)a.a)*b.d)+
		(((u64)a.c)*b.b)+(((u64)a.b)*b.c);
	c.d=(u32)l;
#endif

	return(c);
#else
	BGBDTC_X128 c;
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
#endif
}

/**
  * Multiply int128 values given as pairs representing 256 bit values.
  * /param aa Low 128 bits of left param.
  * /param ab High 128 bits of left param.
  * /param ba Low 128 bits of right param.
  * /param bb High 128 bits of right param.
  * /param rca Low 128 bits of result.
  * /param rcb High 128 bits of result.
  */
BS2VM_API int BGBDT_XI128_MulwX(BGBDTC_X128 a, BGBDTC_X128 b,
	BGBDTC_X128 *rc, BGBDTC_X128 *rd)
{
	BGBDTC_X128 c, d;
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
	
	if(rc)*rc=c;
	if(rd)*rd=d;
	return(0);
}

/** Bitwise AND of 128-bit values. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_AndX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a&b.a;
	c.b=a.b&b.b;
	c.c=a.c&b.c;
	c.d=a.d&b.d;
	return(c);
}

/** Bitwise OR of 128-bit values. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_OrX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a|b.a;
	c.b=a.b|b.b;
	c.c=a.c|b.c;
	c.d=a.d|b.d;
	return(c);
}

/** Bitwise XOR of 128-bit values. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_XorX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.a^b.a;
	c.b=a.b^b.b;
	c.c=a.c^b.c;
	c.d=a.d^b.d;
	return(c);
}

/** Partial ShiftLeft of 128 bit value.
  * /param b Shift in the range of 0 to 31.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ShlpX(BGBDTC_X128 a, int b)
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

/** Partial Logical ShiftRight of 128 bit value.
  * /param b Shift in the range of 0 to 31.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ShrpX(BGBDTC_X128 a, int b)
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

/** Partial Arithmetic ShiftRight of 128 bit value.
  * /param b Shift in the range of 0 to 31.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_SarpX(BGBDTC_X128 a, int b)
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

/** ShiftLeft of 128 bit value.
  * /param b Shift.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ShlX(BGBDTC_X128 a, int b)
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

/** Logical ShiftRight of 128 bit value.
  * /param b Shift.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ShrX(BGBDTC_X128 a, int b)
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

/** Arithmetic ShiftRight of 128 bit value.
  * /param b Shift.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_SarX(BGBDTC_X128 a, int b)
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

/** Logical ShiftRight or ShiftLeft of 128 bit value.
  * /param b Shift, Positive=ShiftRight, Negative=ShiftLeft.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ShrlX(BGBDTC_X128 a, int b)
{
	if(b>=0)
		return(BGBDT_XI128_ShrX(a, b));
	else
		return(BGBDT_XI128_ShlX(a, -b));
}

/** Logical ShiftRight or ShiftLeft of 128 bit value.
  * /param b Shift, Positive=ShiftLeft, Negative=ShiftRight.
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ShlrX(BGBDTC_X128 a, int b)
{
	if(b>=0)
		return(BGBDT_XI128_ShlX(a, b));
	else
		return(BGBDT_XI128_ShrX(a, -b));
}

/** Shift right int128 value by 1 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shr1X(BGBDTC_X128 a)
{
#ifdef X86_MSVC
// #if 0
	BGBDTC_X128 c;
	__asm {
		mov eax, [a+ 0]
		mov ecx, [a+ 4]
		mov edx, [a+ 8]
		shrd eax, ecx, 1
		shrd ecx, edx, 1
		mov [c+ 0], eax
		mov [c+ 4], ecx
		mov eax, [a+12]
		shrd edx, eax, 1
		shr eax, 1
		mov [c+ 8], edx
		mov [c+12], eax
	}
	return(c);
#else
	BGBDTC_X128 c;
	c.a=(a.a>>1)|(a.b<<31);
	c.b=(a.b>>1)|(a.c<<31);
	c.c=(a.c>>1)|(a.d<<31);
	c.d=(a.d>>1);
	return(c);
#endif
}

/** Shift right int128 value by 2 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shr2X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=(a.a>>2)|(a.b<<30);
	c.b=(a.b>>2)|(a.c<<30);
	c.c=(a.c>>2)|(a.d<<30);
	c.d=(a.d>>2);
	return(c);
}

/** Shift right int128 value by 4 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shr4X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=(a.a>>4)|(a.b<<28);
	c.b=(a.b>>4)|(a.c<<28);
	c.c=(a.c>>4)|(a.d<<28);
	c.d=(a.d>>4);
	return(c);
}

/** Shift right int128 value by 8 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shr8X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=(a.a>>8)|(a.b<<24);
	c.b=(a.b>>8)|(a.c<<24);
	c.c=(a.c>>8)|(a.d<<24);
	c.d=(a.d>>8);
	return(c);
}

/** Shift right int128 value by 16 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shr16X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=(a.a>>16)|(a.b<<16);
	c.b=(a.b>>16)|(a.c<<16);
	c.c=(a.c>>16)|(a.d<<16);
	c.d=(a.d>>16);
	return(c);
}

/** Shift right int128 value by 32 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shr32X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.a=a.b;	c.b=a.c;
	c.c=a.d;	c.d=0;
	return(c);
}

/** Shift left int128 value by 1 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shl1X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=(a.d<<1)|(a.c>>31);
	c.c=(a.c<<1)|(a.b>>31);
	c.b=(a.b<<1)|(a.a>>31);
	c.a=(a.a<<1);
	return(c);
}

/** Shift left int128 value by 2 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shl2X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=(a.d<<2)|(a.c>>30);
	c.c=(a.c<<2)|(a.b>>30);
	c.b=(a.b<<2)|(a.a>>30);
	c.a=(a.a<<2);
	return(c);
}

/** Shift left int128 value by 4 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shl4X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=(a.d<<4)|(a.c>>28);
	c.c=(a.c<<4)|(a.b>>28);
	c.b=(a.b<<4)|(a.a>>28);
	c.a=(a.a<<4);
	return(c);
}

/** Shift left int128 value by 8 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shl8X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=(a.d<<8)|(a.c>>24);
	c.c=(a.c<<8)|(a.b>>24);
	c.b=(a.b<<8)|(a.a>>24);
	c.a=(a.a<<8);
	return(c);
}

/** Shift left int128 value by 16 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shl16X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=(a.d<<16)|(a.c>>16);
	c.c=(a.c<<16)|(a.b>>16);
	c.b=(a.b<<16)|(a.a>>16);
	c.a=(a.a<<16);
	return(c);
}

/** Shift left int128 value by 32 */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shl32X(BGBDTC_X128 a)
{
	BGBDTC_X128 c;
	c.d=a.c;	c.c=a.b;
	c.b=a.a;	c.a=0;
	return(c);
}

/** Partial Shift Right int128 value.
  * Bits are shifted in from another value.
  * /param a Value being shifted.
  * /param b Value which is being shifted-in.
  * /param n Number of bits to shift, 0 to 31
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ShrdpX(
	BGBDTC_X128 a, BGBDTC_X128 b, int n)
{
	BGBDTC_X128 c;
	if(!n)
		return(a);
	c.a=(a.a>>n)+(a.b<<(32-n));
	c.b=(a.b>>n)+(a.c<<(32-n));
	c.c=(a.c>>n)+(a.d<<(32-n));
	c.d=(a.d>>n)+(b.a<<(32-n));
	return(c);
}


/** Shift Right/In int128 value by 1 bit. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shrd1X(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=(a.a>>1)|(a.b<<31);
	c.b=(a.b>>1)|(a.c<<31);
	c.c=(a.c>>1)|(a.d<<31);
	c.d=(a.d>>1)|(b.a<<31);
	return(c);
}

/** Shift Right/In int128 value by 2 bits. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shrd2X(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=(a.a>>2)|(a.b<<30);
	c.b=(a.b>>2)|(a.c<<30);
	c.c=(a.c>>2)|(a.d<<30);
	c.d=(a.d>>2)|(b.a<<30);
	return(c);
}

/** Shift Right/In int128 value by 4 bits. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shrd4X(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=(a.a>>4)|(a.b<<28);
	c.b=(a.b>>4)|(a.c<<28);
	c.c=(a.c>>4)|(a.d<<28);
	c.d=(a.d>>4)|(b.a<<28);
	return(c);
}

/** Shift Right/In int128 value by 8 bits. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shrd8X(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=(a.a>>8)|(a.b<<24);
	c.b=(a.b>>8)|(a.c<<24);
	c.c=(a.c>>8)|(a.d<<24);
	c.d=(a.d>>8)|(b.a<<24);
	return(c);
}

/** Shift Right/In int128 value by 16 bits. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shrd16X(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=(a.a>>16)|(a.b<<16);
	c.b=(a.b>>16)|(a.c<<16);
	c.c=(a.c>>16)|(a.d<<16);
	c.d=(a.d>>16)|(b.a<<16);
	return(c);
}

/** Shift Right/In int128 value by 32 bits. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shrd32X(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.a=a.b;
	c.b=a.c;
	c.c=a.d;
	c.d=b.a;
	return(c);
}

/** Partial Shift Left int128 value.
  * Bits are shifted in from another value.
  * /param a Value being shifted.
  * /param b Value which is being shifted-in.
  * /param n Number of bits to shift, 0 to 31
  */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ShldpX(
	BGBDTC_X128 a, BGBDTC_X128 b, int n)
{
	BGBDTC_X128 c;
	c.a=(a.a<<n)|(b.d>>(32-n));
	c.b=(a.b<<n)|(a.a>>(32-n));
	c.c=(a.c<<n)|(a.b>>(32-n));
	c.d=(a.d<<n)|(a.c>>(32-n));
	return(c);
}

/** Shift Left/In int128 value by 1 bit. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_Shld1X(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	c.d=(a.d<<1)|(a.c>>31);
	c.c=(a.c<<1)|(a.b>>31);
	c.b=(a.b<<1)|(a.a>>31);
	c.a=(a.a<<1)|(b.d>>31);
	return(c);
}

/** Returns true if int128 is zero. */
BS2VM_API bool BGBDT_XI128_ZeroXP(BGBDTC_X128 a)
{
	return(!(a.a|a.b|a.c|a.d));
}

/** Returns true if int128 is negative. */
BS2VM_API bool BGBDT_XI128_NegXP(BGBDTC_X128 a)
{
	return(a.d>>31);
}

/** Signed compare of int128 values. */
BS2VM_API int BGBDT_XI128_CmpSgX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	if(((s32)a.d)>((s32)b.d))return( 1);
	if(((s32)a.d)<((s32)b.d))return(-1);
	if(((s32)a.c)>((s32)b.c))return( 1);
	if(((s32)a.c)<((s32)b.c))return(-1);
	if(((s32)a.b)>((s32)b.b))return( 1);
	if(((s32)a.b)<((s32)b.b))return(-1);
	if(((s32)a.a)>((s32)b.a))return( 1);
	if(((s32)a.a)<((s32)b.a))return(-1);
	return(0);
}

/** Unsigned compare of int128 values. */
BS2VM_API int BGBDT_XI128_CmpX(BGBDTC_X128 a, BGBDTC_X128 b)
{
#if 0
	int sa, sb, sc, sd;
	int r;

	sa=a.a-b.a;
	sb=a.b-b.b;
	sc=a.c-b.c;
	sd=a.d-b.d;

	r=sd?sd:(sc?sc:(sb?sb:sa));
	return(r);
#endif

#if 1
	if(a.d>b.d)return( 1);
	if(a.d<b.d)return(-1);
	if(a.c>b.c)return( 1);
	if(a.c<b.c)return(-1);
	if(a.b>b.b)return( 1);
	if(a.b<b.b)return(-1);
	if(a.a>b.a)return( 1);
	if(a.a<b.a)return(-1);
	return(0);
#endif

}

#if 0
// BS2VM_API int BGBDT_XI128_CmpGtX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	int sa, sb, sc, sd;
	int r;

	sa=a.a-b.a;
	sb=a.b-b.b;
	sc=a.c-b.c;
	sd=a.d-b.d;
	r=(sd!=0)?sd:(sc!=0)?sc:(sb!=0)?sb:sa;
	return(r>0);
}
#endif

/** Wide (256 bit) compare of pairs of 128 bit values. */
BS2VM_API int BGBDT_XI128_CmpWX(
	BGBDTC_X128 aa, BGBDTC_X128 ab,
	BGBDTC_X128 ba, BGBDTC_X128 bb)
{
#if 1
	if(ab.d!=bb.d)
	{	if(ab.d>bb.d)return( 1);
		if(ab.d<bb.d)return(-1);	}
	if(ab.c!=bb.c)
	{	if(ab.c>bb.c)return( 1);
		if(ab.c<bb.c)return(-1);	}
	if(ab.b!=bb.b)
	{	if(ab.b>bb.b)return( 1);
		if(ab.b<bb.b)return(-1);	}
	if(ab.a!=bb.a)
	{	if(ab.a>bb.a)return( 1);
		if(ab.a<bb.a)return(-1);	}
	if(aa.d!=ba.d)
	{	if(aa.d>ba.d)return( 1);
		if(aa.d<ba.d)return(-1);	}
	if(aa.c!=ba.c)
	{	if(aa.c>ba.c)return( 1);
		if(aa.c<ba.c)return(-1);	}
	if(aa.b!=ba.b)
	{	if(aa.b>ba.b)return( 1);
		if(aa.b<ba.b)return(-1);	}
	if(aa.a!=ba.a)
	{	if(aa.a>ba.a)return( 1);
		if(aa.a<ba.a)return(-1);	}
#endif

#if 0
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
#endif
	return(0);
}

/* Set a given bit in an int128 value. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_OrBitX(BGBDTC_X128 a, int pos)
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

#if 0
// BS2VM_API int BGBDT_XI128_DivunXI(BGBDTC_X128 a, u32 bv,
	BGBDTC_X128 *rq, BGBDTC_X128 *rr)
{
	BGBDTC_X128 q, r;
	u64 l0;
	u32 fr;

	fr=(1LL<<32)/bv;

	l0=((u64)a.d)*fr;	q.d=(u32)(l0>>32);	r.d=(u32)l0;
	l0=((u64)a.c)*fr;	q.c=(u32)(l0>>32);	r.c=(u32)l0;
	l0=((u64)a.b)*fr;	q.b=(u32)(l0>>32);	r.b=(u32)l0;
	l0=((u64)a.a)*fr;	q.a=(u32)(l0>>32);	r.a=(u32)l0;
	
//	q.a+=r.b*fr;	q.b+=r.c*fr;	q.c+=r.d*fr;
	q.a+=r.b;	q.b+=r.c;	q.c+=r.d;

	l0=((u64)q.c)*fr;	q.c=(u32)(l0>>32);	r.c=(u32)l0;
	l0=((u64)q.b)*fr;	q.b=(u32)(l0>>32);	r.b=(u32)l0;
	l0=((u64)q.a)*fr;	q.a=(u32)(l0>>32);	r.a=(u32)l0;

//	q.a+=r.b*fr;	q.b+=r.c*fr;
	q.a+=r.b;		q.b+=r.c;

	l0=((u64)q.b)*fr;	q.b=(u32)(l0>>32);	r.b=(u32)l0;
	l0=((u64)q.a)*fr;	q.a=(u32)(l0>>32);	r.a=(u32)l0;

//	q.a+=r.b*fr;
	q.a+=r.b;
	l0=((u64)q.a)*fr;	q.a=(u32)(l0>>32);	r.a=(u32)l0;

	r.a=(((u64)r.a)*bv)>>32;

	r.b=0;	r.c=0;	r.d=0;

	if(rq)
		{ *rq=q; }
	if(rr)
		{ *rr=r; }
	return(0);
}
#endif

/** Unsigned divide of int128 values. */
BS2VM_API int BGBDT_XI128_DivuXI(BGBDTC_X128 a, BGBDTC_X128 b,
	BGBDTC_X128 *rq, BGBDTC_X128 *rr)
{
//	static int rp=0;
	BGBDTC_X128 dhx, dlx, q, r, dl1, dl2, dl3;
	u32 ui;
	int pos;
	int i;

#if 0
//	if(!(b.b|b.c|b.d))
//		{ return(BGBDT_XI128_DivunXI(a, b.a, rq, rr)); }

	if(!(b.b|b.c|b.d) && !rp)
	{
		rp=1;
		if(b.a<256)
		{
			i=BGBDT_XI128_Divu2nXI(a, b.a, rq, rr);
			rp=0;
			return(i);
		}
		rp=0;
	}
#endif
	
	dhx=b;
	dlx=BGBDT_XI128_FromLong(0);
	q=BGBDT_XI128_FromLong(0);
	r=a;

	pos=128;

//	if(BGBDT_XI128_ZeroXP(dh))
//		{ return(-1); }
	
	if(dhx.c|dhx.d)
	{
		dlx.a=dlx.c;	dlx.b=dlx.d;
		dlx.c=dhx.a;	dlx.d=dhx.b;
		dhx.a=dhx.c;	dhx.b=dhx.d;
		dhx.c=0;		dhx.d=0;
		pos-=64;
	}

	if(dhx.b)
	{
		dlx=BGBDT_XI128_Shrd32X(dlx, dhx);
		dhx=BGBDT_XI128_Shr32X(dhx);
		pos-=32;
	}

	if(dhx.a>>31)
	{
		dlx=BGBDT_XI128_Shrd32X(dlx, dhx);
		dhx=BGBDT_XI128_Shr32X(dhx);
		pos-=32;
	}else
	{
		ui=dhx.a; i=0;
		if(ui>>16) { ui=ui>>16; i+=16; }
		if(ui>> 8) { ui=ui>> 8; i+=8; }
		if(ui>> 4) { ui=ui>> 4; i+=4; }
		if(ui>> 2) { ui=ui>> 2; i+=2; }
		if(ui>> 1) { ui=ui>> 1; i++; }
		if(ui    ) { ui=ui>> 1; i++; }

		dlx=BGBDT_XI128_ShrdpX(dlx, dhx, i);
		dhx=BGBDT_XI128_ShrX(dhx, i);
		pos-=i;
	}

#ifdef X86_MSVC
	__asm {
	mov ebx, [pos]
	L2:
	
	mov eax, [r+12]
	cmp eax, [dlx+12]
	ja L1
	jb L0
	mov eax, [r+8]
	cmp eax, [dlx+8]
	ja L1
	jb L0
	mov eax, [r+4]
	cmp eax, [dlx+4]
	ja L1
	jb L0
	mov eax, [r+0]
	cmp eax, [dlx+0]
	jb L0
	
	L1:
	mov eax, [dlx+ 0]
	mov edx, [dlx+ 4]
	sub [r+ 0], eax
	sbb [r+ 4], edx
	mov eax, [dlx+ 8]
	mov edx, [dlx+12]
	sbb [r+ 8], eax
	sbb [r+12], edx

	mov eax, ebx
	mov ecx, ebx
	shr eax, 5
	and ecx, 31
	mov edx, 1
	shl edx, cl
	or [q+eax*4], edx

	L0:
#if 0
	movdqu xmm0, [dlx]
	movdqu xmm1, xmm0
	psrld xmm1, 1
	pslld xmm0, 31
	psrldq xmm0, 4
	por xmm0, xmm1
	movdqu [dlx], xmm0
#endif
	
#if 1
	mov eax, [dlx+ 0]
	mov ecx, [dlx+ 4]
	mov edx, [dlx+ 8]
	shrd eax, ecx, 1
	shrd ecx, edx, 1
	mov [dlx+ 0], eax
	mov [dlx+ 4], ecx
	mov eax, [dlx+12]
	shrd edx, eax, 1
	shr eax, 1
	mov [dlx+ 8], edx
	mov [dlx+12], eax
#endif

	dec ebx
	jns L2
	}
#else
	for(; pos>=0; pos--)
	{
		if(BGBDT_XI128_CmpX(r, dlx)>=0)
		{
			r=BGBDT_XI128_SubX(r, dlx);
			q=BGBDT_XI128_OrBitX(q, pos);
		}
		dlx=BGBDT_XI128_Shr1X(dlx);
	}
#endif
	if(rq)
		{ *rq=q; }
	if(rr)
		{ *rr=r; }
	return(0);
}

/** Wide divide of 128 bit values. */
BS2VM_API int BGBDT_XI128_DivW1XI(
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

/** Caching divide special-case of 128 bit values. */
BS2VM_API int BGBDT_XI128_Divu2nXI(
	BGBDTC_X128 a, u32 bv, BGBDTC_X128 *rq, BGBDTC_X128 *rr)
{
	static BGBDTC_X128 rcptab[256];
	static BGBDTC_X128 rcp_hr[256];
	static u32 rcp_hv[256];
	static int init=0;
	BGBDTC_X128 ta, tb, tc, td, tr;
	int i, j, k;
	
	if(!init)
	{
		init=1;
		for(i=1; i<256; i++)
		{
			ta=BGBDT_XI128_FromLong2(0, 1ULL<<63);
			tb=BGBDT_XI128_FromLong(i);
			BGBDT_XI128_DivuXI(ta, tb, &tc, NULL);
			rcptab[i]=tc;
		}

		for(i=0; i<256; i++)
			{ rcp_hv[i]=0; }
	}
	
	if(bv<=2)
	{
		if(bv==1)
		{
			*rq=a;
			return(0);
		}
		tc=BGBDT_XI128_Shr1X(a);
		*rq=tc;
		return(0);
	}

	if(bv<256)
	{
		BGBDT_XI128_MulwX(a, rcptab[bv], &tc, &td);
	}else
	{
		i=((bv*65521*251)>>8)&255;
		if(rcp_hv[i]==bv)
		{
			BGBDT_XI128_MulwX(a, rcp_hr[i], &tc, &td);
		}else
		{
			ta=BGBDT_XI128_FromLong2(0, 1ULL<<63);
			tb=BGBDT_XI128_FromLong(bv);
			BGBDT_XI128_DivuXI(ta, tb, &tc, NULL);
			rcp_hr[i]=tc;
			rcp_hv[i]=bv;

			BGBDT_XI128_MulwX(a, rcp_hr[i], &tc, &td);
		}
	}

	tc=BGBDT_XI128_Shld1X(td, tc);
	td=BGBDT_XI128_MulX(tc,
		BGBDT_XI128_FromLong(bv));
	tr=BGBDT_XI128_SubX(a, td);

	if((tr.b|tr.c|tr.d) || (tr.a>=(4*bv)))
	{
		/* Oh Crap Special Case */
		tb=BGBDT_XI128_FromLong(bv);
		BGBDT_XI128_DivuXI(a, tb, &tc, &tr);
	}

	while(tr.a>=bv)
	{
		tc=BGBDT_XI128_AddIX(tc, 1);
//		tr=BGBDT_XI128_AddIX(tr, -bv);

		td=BGBDT_XI128_MulX(tc,
			BGBDT_XI128_FromLong(bv));
		tr=BGBDT_XI128_SubX(a, td);
	}
	
	if(rq)*rq=tc;
	if(rr)*rr=tr;
	return(0);
}

/** Signed divide of 128 bit values, with remainder. */
BS2VM_API int BGBDT_XI128_DivXI(BGBDTC_X128 a, BGBDTC_X128 b,
	BGBDTC_X128 *rq, BGBDTC_X128 *rr)
{
	BGBDTC_X128 ta, tb;
	int sg, rt;
	
	ta=a; tb=b; sg=0;
	if(BGBDT_XI128_NegXP(ta))
		{ ta=BGBDT_XI128_NegX(ta); sg=!sg; }
	if(BGBDT_XI128_NegXP(tb))
		{ tb=BGBDT_XI128_NegX(tb); sg=!sg; }

	if(!(tb.b|tb.c|tb.d))
	{
//		if(tb.a<256)
//			{ return(BGBDT_XI128_Divu2nXI(a, tb.a, rq, rr)); }
		return(BGBDT_XI128_Divu2nXI(a, tb.a, rq, rr));
	}

	rt=BGBDT_XI128_DivuXI(ta, tb, rq, rr);
	if(sg && rq)
	{
		*rq=BGBDT_XI128_NegX(*rq);
	}
	return(rt);
}

/** Signed divide of 128 bit values, no remainder. */
BS2VM_API int BGBDT_XI128_Div2XI(BGBDTC_X128 a, BGBDTC_X128 b,
	BGBDTC_X128 *rq)
{
	BGBDTC_X128 ta, tb;
	int sg, rt;
	
	ta=a; tb=b; sg=0;
	if(BGBDT_XI128_NegXP(ta))
		{ ta=BGBDT_XI128_NegX(ta); sg=!sg; }
	if(BGBDT_XI128_NegXP(tb))
		{ tb=BGBDT_XI128_NegX(tb); sg=!sg; }

	if(!(tb.b|tb.c|tb.d))
	{
//		if(tb.a<256)
//			{ return(BGBDT_XI128_Divu2nXI(a, tb.a, rq, NULL)); }
		return(BGBDT_XI128_Divu2nXI(a, tb.a, rq, NULL));
	}
	
	rt=BGBDT_XI128_DivuXI(ta, tb, rq, NULL);
	if(sg && rq)
		{ *rq=BGBDT_XI128_NegX(*rq); }
	return(rt);
}

/** Signed divide of 128 bit values. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_DivX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
//	BGBDT_XI128_DivXI(a, b, &c, NULL);
	BGBDT_XI128_Div2XI(a, b, &c);
	return(c);
}

/** Signed modulo of 128 bit values. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_ModX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	BGBDTC_X128 c;
	BGBDT_XI128_DivXI(a, b, NULL, &c);
	return(c);
}


/** Wrap int128 as tagged reference. */
BS2VM_API dtVal BGBDT_XI128_WrapInt128(BGBDTC_X128 a)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XI128_AllocInt128();
	*xp=a;
	return(dtvWrapTagPtrF(xp, objty_si128));
}

#if 0
// BS2VM_API dtVal BGBDT_XI128_WrapUInt128(BGBDTC_X128 a)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XI128_AllocUInt128();
	*xp=a;
	return(dtvWrapTagPtrF(xp, objty_ui128));
}
#endif

/** Wrap int128 as tagged reference, created from an s64. */
BS2VM_API dtVal BGBDT_XI128_WrapInt128L(s64 l)
{
	BGBDTC_X128 *xp;
	
	xp=BGBDT_XI128_AllocInt128();
	*xp=BGBDT_XI128_FromLong(l);
	return(dtvWrapTagPtrF(xp, objty_si128));
}

/** Wrap int128 as tagged reference, created from a pair of u64 */
BS2VM_API dtVal BGBDT_XI128_WrapInt128L2(u64 lo, u64 hi)
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
// BS2VM_API dtVal BGBDT_XI128_WrapUInt128L2(u64 lo, u64 hi)
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

/** Get int128 value from an int128 tagged reference. */
BS2VM_API void BGBDT_XI128_GetInt128v(dtVal vec, BGBDTC_X128 *v)
{
	BGBDTC_X128 *xp;
	xp=dtvUnwrapPtrF(vec);
	*v=*xp;
}

/** Unwrap an int128 value from a tagged reference. */
BS2VM_API BGBDTC_X128 BGBDT_XI128_UnwrapInt128(dtVal vec)
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
// BS2VM_API void BGBDT_XI128_GetUInt128v(dtVal vec, BGBDTC_X128 *v)
{
	BGBDTC_X128 *xp;
	xp=dtvUnwrapPtrF(vec);
	*v=*xp;
}
#endif

/** Add int128 values given by tagged references. */
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

/** Subtract int128 values given by tagged references. */
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

/** Multiply int128 values given by tagged references. */
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

/** Divide int128 values given by tagged references. */
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

/** Modulo int128 values given by tagged references. */
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

/** Bitwise AND int128 values given by tagged references. */
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

/** Bitwise OR int128 values given by tagged references. */
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


/** Bitwise XOR int128 values given by tagged references. */
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

/** Shift Left int128 values given by tagged references. */
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

/** Shoft Right int128 values given by tagged references. */
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

/** Shift Right Signed int128 values given by tagged references. */
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

/** Negate int128 value given by tagged reference. */
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

/** Bitwise NOT of int128 value given by tagged reference. */
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

/** Loginal Not of int128 value given by tagged reference. */
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

/** Compare int128 values given by tagged references.
  * Returns -2 if values do not promote to int128;
  */
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
	
	return(-2);
}

/** Compare int128 values given by tagged references.
  * Coerces values to Int128.
  */
int BGBDT_XI128_CompareB(dtVal a, dtVal b)
{
	BGBDTC_X128 xa, xb, xc;
	int i;

	xa=BGBDT_XI128_UnwrapInt128(a);
	xb=BGBDT_XI128_UnwrapInt128(b);
	i=BGBDT_XI128_CmpX(xa, xb);
	return(i);
}
