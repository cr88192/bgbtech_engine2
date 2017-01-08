/*
Decimal 128

Low 110 bits:
	11x 10 bits (Mod1k), representing 33 decimal digits
High 16 bits:
	MSB=Sign
	15 bits=exponent (in powers of 10)

For exponents <= 16383, there are no integer-part digits.
For exponents >= 16417, there are no fractional-part digits.

Change:
	3x 30 bit DPD
	1x 20 bit DPD
	11 bit exponent
	1 bit sign

Unpacked to 4x ten's complement, exponent
*/

BS2VM_API void BGBDT_XD128_UnpackDPD(BGBDTC_X128 a, s32 *tb)
{
	tb[0]=a.a;
	tb[1]=a.b;
	tb[2]=a.c;
	tb[3]=a.d&0x000FFFFF;
	tb[4]=(a.d>>20)&0x7FF;
	
	if(a.d>>31)
	{
		tb[0]=999999999-tb[0];
		tb[1]=999999999-tb[1];
		tb[2]=999999999-tb[2];
		tb[3]=999999999-tb[3];
	}
}

BS2VM_API BGBDTC_X128 BGBDT_XD128_PackDPD(s32 *ta)
{
	s32 tc[5];
	BGBDTC_X128 c;
	int s;
		
	tc[0]=ta[0];
	tc[1]=ta[1];
	tc[2]=ta[2];
	tc[3]=ta[3];
	tc[4]=ta[4];
	
	s=0;
	if(tc[3]>500000000)
	{
		tc[0]=999999999-tc[0];
		tc[1]=999999999-tc[1];
		tc[2]=999999999-tc[2];
		tc[3]=999999999-tc[3];
		s=1;
	}
	c.a=tc[0];
	c.b=tc[1];
	c.c=tc[2];
	c.d=tc[3]|(tc[4]<<20)|(s<<31);

	return(c);
}

BS2VM_API void BGBDT_XD128_AdjustAddDPD(s32 *ta, s32 *tc)
{
	if(ta!=tc)
	{
		tc[0]=ta[0];	tc[1]=ta[1];
		tc[2]=ta[2];	tc[3]=ta[3];
		tc[4]=ta[4];
	}

	if(tc[0]>999999999)
		{ tc[1]++; tc[0]-=999999999; }
	if(tc[1]>999999999)
		{ tc[2]++; tc[1]-=999999999; }
	if(tc[2]>999999999)
		{ tc[3]++; tc[2]-=999999999; }

	if(tc[3]>=1000000000)
		{ tc[3]=tc[3]-1000000000; }

	if(tc[3]>=500000000)
	{
		if(tc[3]<999000000)
		{
			tc[4]++;
			BGBDT_XD128_ShrDPD1(tc, tc);
		}
	}else
	{
		if(tc[3]>999999)
		{
			tc[4]++;
			BGBDT_XD128_ShrDPD1(tc, tc);
		}
	}	
}

BS2VM_API void BGBDT_XD128_ShrDPD1(s32 *ta, s32 *tb)
{
	int s;

	tb[0]=ta[0];
	tb[1]=ta[1];
	tb[2]=ta[2];
	tb[3]=ta[3];
//	s=(ta[3]>>24)!=0;
	s=(ta[3]>500000000);

	tb[0]=(tb[0]/10)+(tb[1]%10)*100000000;
	tb[1]=(tb[1]/10)+(tb[2]%10)*100000000;
	tb[2]=(tb[2]/10)+(tb[3]%10)*100000000;
	tb[3]=tb[3]/10;
	if(s) { tb[3]=tb[3]+900000000; }
}

BS2VM_API void BGBDT_XD128_ShrDPD(s32 *ta, s32 *tc, int c)
{
	s32 tb[5];
	int j, s;

	if(!c)return;
	
	tb[0]=ta[0];
	tb[1]=ta[1];
	tb[2]=ta[2];
	tb[3]=ta[3];
//	s=(ta[3]>>24)!=0;
	s=(ta[3]>500000000);
	
	j=c;
	while(j>=9)
	{
		tb[0]=tb[1];
		tb[1]=tb[2];
		if(s)
		{
			tb[2]=tb[3]+999000000;
			tb[3]=999999;
		}else
		{
			tb[2]=tb[3];
			tb[3]=0;
		}
		j-=9;
	}

	while(j>=3)
	{
		tb[0]=(tb[0]>>10)|((tb[1]&1023)<<20);
		tb[1]=(tb[1]>>10)|((tb[2]&1023)<<20);
		tb[2]=(tb[2]>>10)|((tb[3]&1023)<<20);
		tb[3]=tb[3]>>10;
		if(s) { tb[3]=tb[3]+999000; }
		j-=3;
	}

	while(j>=1)
	{
		tb[0]=(tb[0]/10)+(tb[1]%10)*100000000;
		tb[1]=(tb[1]/10)+(tb[2]%10)*100000000;
		tb[2]=(tb[2]/10)+(tb[3]%10)*100000000;
		tb[3]=tb[3]/10;
		if(s) { tb[3]=tb[3]+900000; }
		j--;
	}
	tc[0]=tb[0];
	tc[1]=tb[1];
	tc[2]=tb[2];
	tc[3]=tb[3];
}

void BGBDT_XD128_NormDPD(s32 *ta, s32 *tc)
{
	tc[0]=ta[0];	tc[1]=ta[1];
	tc[2]=ta[2];	tc[3]=ta[3];
	tc[4]=ta[4];

	if(tc[3]==0)
	{
		if((tc[0]|tc[1]|tc[2])==0)
			{ tc[4]=0; return; }
		while((tc[3]==0) && (tc[2]<=999999))
		{
			tc[3]=tc[2];	tc[2]=tc[1];
			tc[1]=tc[0];	tc[0]=0;
			tc[4]-=9;
		}
		while(tc[3]<=999)
		{
			tc[3]=(tc[3]*1000)+(tc[2]/1000000);
			tc[2]=((tc[2]%1000000)*1000)+(tc[1]/1000000);
			tc[1]=((tc[1]%1000000)*1000)+(tc[0]/1000000);
			tc[0]=((tc[0]%1000000)*1000);
			tc[4]-=3;
		}
	}

	if(tc[3]==999999999)
	{
		if(((tc[0]|tc[1]|tc[2])==999999999))
			{ tc[4]=0; return; }
		while((tc[3]==999999999) && (tc[2]>=999000000))
		{
			tc[3]=tc[2];	tc[2]=tc[1];
			tc[1]=tc[0];	tc[0]=999999999;
			tc[4]-=9;
		}
		while(tc[3]>=999999000)
		{
			tc[3]=((tc[3]%1000000)*1000)+(tc[2]/1000000);
			tc[2]=((tc[2]%1000000)*1000)+(tc[1]/1000000);
			tc[1]=((tc[1]%1000000)*1000)+(tc[0]/1000000);
			tc[0]=((tc[0]%1000000)*1000)+999;
			tc[4]-=3;
		}
	}
}

void BGBDT_XD128_NegDPD(s32 *ta, s32 *tb)
{
	int s;

	tb[0]=999999999-ta[0];
	tb[1]=999999999-ta[1];
	tb[2]=999999999-ta[2];
	tb[3]=999999999-ta[3];
	tb[4]=ta[4];
}

BS2VM_API BGBDTC_X128 BGBDT_XD128_AddX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	s32 ta[5], tb[5], tc[5];
	BGBDTC_X128 c;

	BGBDT_XD128_UnpackDPD(a, ta);
	BGBDT_XD128_UnpackDPD(b, tb);
	
	if(ta[4]!=tb[4])
	{
		tc[4]=ta[4];
		if(tb[4]>tc[4])
			tc[4]=tb[4];
		BGBDT_XD128_ShrDPD(ta, ta, tc[4]-ta[4]);
		BGBDT_XD128_ShrDPD(tb, tb, tc[4]-tb[4]);
	}
	
	tc[0]=ta[0]+tb[0];
	tc[1]=ta[1]+tb[1];
	tc[2]=ta[2]+tb[2];
	tc[3]=ta[3]+tb[3];
	BGBDT_XD128_AdjustAddDPD(tc, tc);
	BGBDT_XD128_NormDPD(tc, tc);
	c=BGBDT_XD128_PackDPD(tc);
	return(c);
}

BS2VM_API BGBDTC_X128 BGBDT_XD128_SubX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	s32 ta[5], tb[5], tc[5];
	BGBDTC_X128 c;

	BGBDT_XD128_UnpackDPD(a, ta);
	BGBDT_XD128_UnpackDPD(b, tb);
	BGBDT_XD128_NegDPD(tb, tb);
	
	if(ta[4]!=tb[4])
	{
		tc[4]=ta[4];
		if(tb[4]>tc[4])
			tc[4]=tb[4];
		BGBDT_XD128_ShrDPD(ta, ta, tc[4]-ta[4]);
		BGBDT_XD128_ShrDPD(tb, tb, tc[4]-tb[4]);
	}
	
	tc[0]=ta[0]+tb[0];
	tc[1]=ta[1]+tb[1];
	tc[2]=ta[2]+tb[2];
	tc[3]=ta[3]+tb[3];
	BGBDT_XD128_AdjustAddDPD(tc, tc);
	BGBDT_XD128_NormDPD(tc, tc);
	c=BGBDT_XD128_PackDPD(tc);
	return(c);
}

BS2VM_API BGBDTC_X128 BGBDT_XD128_MulX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	s32 ta[5], tb[5], tc[5], td[9];
	BGBDTC_X128 c;
	s64 l;

	BGBDT_XD128_UnpackDPD(a, ta);
	BGBDT_XD128_UnpackDPD(b, tb);

	l=((s64)ta[0])*tb[0];
	td[0]=l%1000000000;
	l=(l/1000000000)+(((s64)ta[0])*tb[1])+(((s64)ta[1])*tb[0]);
	td[1]=l%1000000000;
	l=(l/1000000000)+(((s64)ta[0])*tb[2])+
		(((s64)ta[1])*tb[1])+(((s64)ta[2])*tb[0]);
	td[2]=l%1000000000;
	l=(l/1000000000)+(((s64)ta[0])*tb[3])+(((s64)ta[1])*tb[2])+
		(((s64)ta[2])*tb[1])+(((s64)ta[3])*tb[0]);
	td[3]=l%1000000000;
	l=(l/1000000000)+(((s64)ta[1])*tb[3])+(((s64)ta[2])*tb[2])+
		(((s64)ta[3])*tb[1]);
	td[4]=l%1000000000;
	l=(l/1000000000)+(((s64)ta[2])*tb[3])+(((s64)ta[3])*tb[2]);
	td[5]=l%1000000000;
	l=(l/1000000000)+(((s64)ta[3])*tb[3]);
	td[6]=l%1000000000;
	l=(l/1000000000);
	td[7]=l%1000000000;

	td[8]=ta[4]+(tb[4]-16383);
	
	if(!(td[0]|td[1]|td[2]|td[3]|td[4]|td[5]|td[6]|td[7]))
	{
		c.a=0;	c.b=0;
		c.c=0;	c.d=0;
		return(c);
	}
	
	if(td[7]==0)
	{
		while((td[7]==0) && (td[6]<=999999))
		{
			td[7]=td[6];	td[6]=td[5];
			td[5]=td[4];	td[4]=td[3];
			td[3]=td[2];	td[2]=td[1];
			td[1]=td[0];	td[0]=0;
			td[8]-=9;
		}

		while(td[7]<=999)
		{
			td[7]=(td[7]*1000)+(td[6]/1000000);
			td[6]=((td[6]%1000000)*1000)+(td[5]/1000000);
			td[5]=((td[5]%1000000)*1000)+(td[4]/1000000);
			td[4]=((td[4]%1000000)*1000)+(td[3]/1000000);
			td[3]=((td[3]%1000000)*1000)+(td[2]/1000000);
			td[2]=((td[2]%1000000)*1000)+(td[1]/1000000);
			td[1]=((td[1]%1000000)*1000)+(td[0]/1000000);
			td[0]=((td[0]%1000000)*1000);
			td[8]-=3;
		}
	}else if(td[7]==999999999)
	{
		while((td[7]==999999999) && (td[6]>=999000000))
		{
			td[7]=td[6];	td[6]=td[5];
			td[5]=td[4];	td[4]=td[3];
			td[3]=td[2];	td[2]=td[1];
			td[1]=td[0];	td[0]=999999999;
			td[8]-=9;
		}

		while(td[7]>=999999000)
		{
			td[7]=(td[7]*1000)+(td[6]/1000000);
			td[6]=((td[6]%1000000)*1000)+(td[5]/1000000);
			td[5]=((td[5]%1000000)*1000)+(td[4]/1000000);
			td[4]=((td[4]%1000000)*1000)+(td[3]/1000000);
			td[3]=((td[3]%1000000)*1000)+(td[2]/1000000);
			td[2]=((td[2]%1000000)*1000)+(td[1]/1000000);
			td[1]=((td[1]%1000000)*1000)+(td[0]/1000000);
			td[0]=((td[0]%1000000)*1000)+999;
			td[8]-=3;
		}
	}

	tc[0]=td[4];	tc[1]=td[5];
	tc[2]=td[6];	tc[3]=td[7];
	tc[4]=td[8];
	c=BGBDT_XD128_PackDPD(tc);
	return(c);
}

BS2VM_API BGBDTC_X128 BGBDT_XD128_RcpX(BGBDTC_X128 a)
{
	s32 ta[5], tba[5], tbb[5], tc[5];
	BGBDTC_X128 c;
	int s;

	BGBDT_XD128_UnpackDPD(a, ta);
	
	s=0;
	if(ta[3]>=500000000)
	{
		s=1;
		ta[0]=999999999-ta[0];
		ta[1]=999999999-ta[1];
		ta[2]=999999999-ta[2];
		ta[3]=999999999-ta[3];
	}
	
	if(ta[3])
		{ tba[3]=1000000/ta[3];	tbb[3]=1000000%ta[3]; }
	else
		{ tba[3]=0; tbb[3]=0; }
	if(ta[2])
		{ tba[2]=1000000000/ta[2];	tbb[2]=1000000000%ta[2]; }
	else
		{ tba[2]=0; tbb[2]=0; }
	if(ta[1])
		{ tba[1]=1000000000/ta[1];	tbb[1]=1000000000%ta[1]; }
	else
		{ tba[1]=0; tbb[1]=0; }
	if(ta[0])
		{ tba[0]=1000000000/ta[0];	tbb[0]=1000000000%ta[0]; }
	else
		{ tba[0]=0; tbb[0]=0; }
	
	tc[0]=tba[0]+tbb[1];
	tc[1]=tba[1]+tbb[2];
	tc[2]=tba[2]+tbb[3];
	tc[3]=tba[3];
	tc[4]=16383+(16383-ta[4]);

	BGBDT_XD128_AdjustAddDPD(tc, tc);
	
	if(s)
	{
		tc[0]=999999999-tc[0];
		tc[1]=999999999-tc[1];
		tc[2]=999999999-tc[2];
		tc[3]=999999999-tc[3];
	}
	
	BGBDT_XD128_NormDPD(tc, tc);
	c=BGBDT_XD128_PackDPD(tc);
	return(c);
}

BS2VM_API BGBDTC_X128 BGBDT_XD128_DivX(BGBDTC_X128 a, BGBDTC_X128 b)
{
	return(BGBDT_XD128_MulX(a, BGBDT_XD128_RcpX(b)));
}


BS2VM_API void BGBDT_XD128_DumpStr(BGBDTC_X128 a, char *buf)
{
	s32 ta[5], tb[5], tc[5];
	char *t;
	int s;

	BGBDT_XD128_UnpackDPD(a, ta);

	s=0;
	if(ta[3]>=500000000)
	{
		s=1;
		ta[0]=999999999-ta[0];
		ta[1]=999999999-ta[1];
		ta[2]=999999999-ta[2];
		ta[3]=999999999-ta[3];
	}
	
	t=buf;
	if(s)*t++='-';
	sprintf(t, "%06d%09d%09d%09dE%d", ta[3], ta[2], ta[1], ta[0],
		ta[4]-16383);
}

s32 bgbdt_xd128_readdgts(char **rs, int n)
{
	char *s;
	s32 t;
	int i;
	
	s=*rs; t=0;
	for(i=0; i<n; i++)
		t=(t*10)+((*s++)-'0');
	*rs=s;
	return(t);
}

BS2VM_API BGBDTC_X128 BGBDT_XD128_ReadDumpStr(char *buf)
{
	s32 ta[5];
	BGBDTC_X128 c;
	char *cs;
	int sg;
	
	cs=buf;
	
	sg=0;
	if(*cs=='-') { cs++; sg=1; }
	
	ta[3]=bgbdt_xd128_readdgts(&cs, 6);
	ta[2]=bgbdt_xd128_readdgts(&cs, 9);
	ta[1]=bgbdt_xd128_readdgts(&cs, 9);
	ta[0]=bgbdt_xd128_readdgts(&cs, 9);
	if(*cs=='E')
		ta[4]=atoi(cs+1);
	
	if(sg)
	{
		ta[0]=999999999-ta[0];
		ta[1]=999999999-ta[1];
		ta[2]=999999999-ta[2];
		ta[3]=999999999-ta[3];
	}

	BGBDT_XD128_NormDPD(ta, ta);
	c=BGBDT_XD128_PackDPD(ta);
	return(c);
}
