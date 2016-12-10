static const int btac1c1_difftab[128]={
     0,     1,     2,     3,     4,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,
    16,    18,    20,    22,    24,    26,    28,    30,
    32,    36,    40,    44,    48,    52,    56,    60,
    64,    72,    80,    88,    96,   104,   112,   120,
   128,   145,   161,   177,   193,   209,   225,   241,
   256,   290,   322,   354,   387,   419,   451,   483,
   512,   580,   645,   709,   774,   838,   903,   967,
  1024,  1161,  1290,  1419,  1548,  1677,  1806,  1935,
  2048,  2322,  2580,  2838,  3096,  3354,  3612,  3870,
  4096,  4644,  5160,  5676,  6192,  6708,  7224,  7740,
  8192,  9288, 10320, 11352, 12384, 13416, 14448, 15480,
 16384, 18577, 20641, 22705, 24769, 26833, 28897, 30961,
 32768, 37154, 41282, 45410, 49539, 53667, 57795, 61923,
 65536, 74308, 82565, 90821, 99078,107334,115591,123847,
131072,148617,165130,181643,198156,214669,231182,247695,
};

static const int btac1c1_rangetab[64]={
     0,     1,     2,     3,
     4,     5,     6,     7,
     8,    10,    12,    14,
    16,    20,    24,    28,
    32,    40,    48,    56,
    64,    81,    97,   113,
   128,   162,   195,   227,
   256,   325,   390,   455,
   512,   650,   780,   910,
  1024,  1300,  1560,  1820,
  2048,  2600,  3120,  3640,
  4096,  5201,  6241,  7281,
  8192, 10402, 12483, 14563,
 16384, 20805, 24966, 29127,
 32768, 41610, 49932, 58254,
 65536, 83220, 99864,116508,
};


BTEIFGL_API void BGBDT_SndBTAC1C1_DecodeBlockMonoSRI(
	byte *ibuf, s16 *obuf, int len)
{
	static const byte wtab[4][2]={{16,0}, {11,5}, {5,11}, {0,16}};
	int pred, index, step, diff, uni, sni;
	int len1, sbpred, sbpred1, sbbase, sbrange;
	int sblo, sbhi, sblo1, sbhi1;
	int sbloadd, sbhiadd;
	int i0, i1, blk, stepadd, tag;
	int i, j, k;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2]&127;
	sbpred=pred;
	
	step=bgbmid_ima_step_table[index&127];
	
	sblo=sbpred-step;
	sbhi=sbpred+step;
	sblo1=sblo;
	sbhi1=sbhi;

	len1=len>>3;
	for(i0=0; i0<len1; i0++)
	{
		j=i0*4;
		blk=(ibuf[4+j+0]    )+(ibuf[4+j+1]<< 8)+
			(ibuf[4+j+2]<<16)+(ibuf[4+j+3]<<24);
		stepadd=0;

		k=btac1c1_difftab[blk&127];
		if(blk&128)k=-k;
		sbbase=sbpred+k;

		k=btac1c1_rangetab[(blk>>10)&63];
		sbrange=k;

		tag=(blk>>8)&7;
		switch(tag)
		{
		case 0:
			stepadd=0;
			sbpred=sbbase;
			sbpred1=sbbase;
			break;
		case 4:
//			sbpred=sbbase;
			sbpred1=sbbase;
			stepadd=(sbpred1-sbpred)/8;
			break;
		case 1:		case 5:
			sbpred=sbbase;					sbpred1=sbbase;
			sblo=sbbase-(sbrange>>1);		sbhi=sblo+sbrange;
			sblo1=sblo;						sbhi1=sbhi;
			sbloadd=0;						sbhiadd=0;
			break;
		case 2:		case 6:
			sbpred1=sbbase;
			sblo1=sbbase-(sbrange>>1);		sbhi1=sblo1+sbrange;
			sbloadd=(sblo1-sblo)/8;			sbhiadd=(sbhi1-sbhi)/8;
			break;
		}

		if((tag==0) || (tag==4))
		{
			step=bgbmid_ima_step_table[index];
			pred=sbpred;
			for(i1=0; i1<8; i1++)
			{
				i=(i0<<3)|i1;

				j=(blk>>(8+(i1*3)))&7;
				uni=j<<1;
				if(!i1)uni=0;

				index=index+bgbmid_ima_index_table[uni];
				index=(index<0)?0:((index>88)?88:index);
				diff=((2*(uni&7)+1)*step)/8;
				if(uni&8)diff=-diff;
				pred=pred+diff;
				step=bgbmid_ima_step_table[index];

				pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
				obuf[i]=pred;
				pred+=stepadd;
			}
		}else if(
			(tag==1) || (tag==5)||
			(tag==2) || (tag==6))
		{
			for(i1=0; i1<8; i1++)
			{
				i=(i0<<3)|i1;
				j=(blk>>(16+i1*2))&3;
				pred=((sblo*wtab[j][0])+(sbhi*wtab[j][1]))/16;

				pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
				obuf[i]=pred;
				sblo+=sbloadd;
				sbhi+=sbhiadd;
			}
		}else
		{
		}

		sbpred=sbpred1;
		sblo=sblo1;
		sbhi=sbhi1;
	}
}

int BGBDT_SndBTAC1C1_QuantSBaseSRI(int diff)
{
	int sgn, base;
	int ix;
	int d0, d1;
	
	if(diff<0)
		{ sgn=0x80; base=-diff; }
	else
		{ sgn=0x00; base=diff; }

	ix=0;
	while(btac1c1_difftab[ix+1]<base)ix++;
	d0=btac1c1_difftab[ix+0]-base;
	d1=btac1c1_difftab[ix+1]-base;
	d0=(d0<<1)^(d0>>31);
	d1=(d1<<1)^(d1>>31);
	if(d1<d0)ix++;
	return(ix|sgn);
}

int BGBDT_SndBTAC1C1_QuantSRangeSRI(int diff)
{
	int ix;
	int d0, d1;
	
	ix=0;
	while(btac1c1_rangetab[ix+1]<diff)ix++;
	d0=btac1c1_rangetab[ix+0]-diff;
	d1=btac1c1_rangetab[ix+1]-diff;
	d0=(d0<<1)^(d0>>31);
	d1=(d1<<1)^(d1>>31);
	if(d1<d0)ix++;
	return(ix);
}

BTEIFGL_API void BGBDT_SndBTAC1C1_EncodeBlockMonoSRI(
	s16 *ibuf, byte *obuf, int len, int *ridx, int usetag)
{
	static const byte wtab[4][2]={{16,0}, {11,5}, {5,11}, {0,16}};
	int p0, p1, p2, d0, d1, d2, uni1, uni2;
	int lsbt, lsbit;
	int tblk[8], terr[8];
	int tsbpred1[8];
	int tpred1[8], tindex1[8];
	int tsblo1[8], tsbhi1[8];
	int blk;
	int pred, pred0, index, index0, step, diff, uni, sni;
	int len1, sbpred, sbpred0, sbpred1, sbbase, sbrange;
	int sblo, sbhi, sblo0, sbhi0, sblo1, sbhi1;
	int sbix, sbmin, sbmax, sba;
	int sbloadd, sbhiadd;
	int tag, tmin, tmax, stepadd, err, d;
//	int p0, p1;
	int i0, i1;
	int i, j, k;
	
	if(usetag>=0)
	{
		tmin=usetag;
		tmax=usetag+1;
	}else
	{
		tmin=0; tmax=8;
	}
	
	pred=(ibuf[0]+ibuf[1])>>1;
//	pred=ibuf[0];
	index=ridx?(*ridx):0;
	
	obuf[0]=pred&255;
	obuf[1]=(pred>>8)&255;
	obuf[2]=128|index;
	obuf[3]=0;
	
	memset(obuf+4, 0, (len+1)/2);
	
	step=bgbmid_ima_step_table[index&127];
	lsbt=0;

	sbpred=pred;
	sblo=sbpred-step;
	sbhi=sbpred+step;

	len1=len>>3;
//	for(i=0; i<len; i++)
	for(i0=0; i0<len1; i0++)
	{
//		tag=2;
		tag=4;
		
		pred0=pred;
		sblo0=sblo;
		sbhi0=sbhi;
		sbpred0=sbpred;
		index0=index;
		
		for(i=0; i<8; i++)
			terr[i]=999999999;
		
//		for(tag=0; tag<8; tag++)
		for(tag=tmin; tag<tmax; tag++)
		{
			sbpred=sbpred0;
			sblo=sblo0;
			sbhi=sbhi0;
			pred=pred0;
			index=index0;
			
			p0=sbpred;
			if((tag==2) || (tag==4))
			{
				p1=ibuf[i0*8+8];
			}
			else if(tag==0)
			{
				p1=ibuf[i0*8+0];
			}
			else if(tag==1)
//			else if((tag==0) || (tag==1))
			{
				p1=0;
				for(i1=0; i1<8; i1++)
				{
					i=i0*8+i1;
					j=ibuf[i];
					p1+=j;
				}
				p1=p1/8;
			}else
			{
				continue;
			}

			d0=p1-p0;
			sbix=BGBDT_SndBTAC1C1_QuantSBaseSRI(d0);

			blk=sbix|(tag<<8);

			if((tag==1) || (tag==2))
			{
				sbmin=p0; sbmax=p0;
				if(p1<sbmin)sbmin=p1;
				if(p1>sbmax)sbmax=p1;
				for(i1=0; i1<8; i1++)
				{
					i=i0*8+i1;
					if(tag==2)
						{ i+=4; if(i>=len)i=len-1; }
					j=ibuf[i];
					if(j<sbmin)sbmin=j;
					if(j>sbmax)sbmax=j;
				}
				
				j=sbmax-sbmin;
				k=BGBDT_SndBTAC1C1_QuantSRangeSRI(j);
				blk|=(k<<10);
			}


			k=btac1c1_difftab[blk&127];
			if(blk&128)k=-k;
			sbbase=sbpred+k;

			k=btac1c1_rangetab[(blk>>10)&63];
			sbrange=k;

			switch(tag)
			{
			case 0:
				stepadd=0;
				sbpred=sbbase;
				sbpred1=sbbase;
				break;
			case 4:
	//			sbpred=sbbase;
				sbpred1=sbbase;
				stepadd=(sbpred1-sbpred)/8;
				break;
			case 1:		case 5:
				sbpred=sbbase;					sbpred1=sbbase;
				sblo=sbbase-(sbrange>>1);		sbhi=sblo+sbrange;
				sblo1=sblo;						sbhi1=sbhi;
				sbloadd=0;						sbhiadd=0;
				break;
			case 2:		case 6:
				sbpred1=sbbase;
				sblo1=sbbase-(sbrange>>1);		sbhi1=sblo1+sbrange;
				sbloadd=(sblo1-sblo)/8;			sbhiadd=(sbhi1-sbhi)/8;
				break;
			}

			err=0;
			p0=pred; p1=ibuf[i0*8];
			
			if((tag==0) || (tag==4))
			{
				step=bgbmid_ima_step_table[index];

				pred=sbpred;
				for(i1=0; i1<8; i1++)
				{
					i=i0*8+i1;
				
					j=ibuf[i];
					k=j-pred;

					if(k>=0)
						{ sni=(k*4)/step; uni=(sni>7)?7:sni; }
					else
						{ sni=(-k*4)/step; uni=(sni>7)?15:(8|sni); }

					uni=BGBDT_SndBTAC1C_EncodeQuantUni3(uni, step, pred, j, 2);
				
					index=index+bgbmid_ima_index_table[uni];
					index=(index<0)?0:((index>88)?88:index);
					diff=((2*(uni&7)+1)*step)/8;
					if(uni&8)diff=-diff;
					pred=pred+diff;
					step=bgbmid_ima_step_table[index];

					pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);

//					p0=(3*p0+pred)/4;
//					p1=(3*p1+ibuf[i])/4;
					p0=(p0+pred)/2;
					p1=(p1+ibuf[i])/2;
					d=p0-p1; d=(d<<1)^(d>>31);

//					d=ibuf[i]-pred; d=(d<<1)^(d>>31);
					err+=d;

					pred+=stepadd;
					
					if(i1)
					{
						blk|=(uni>>1)<<(8+i1*3);
					}
				}
			}else if(
				(tag==1) || (tag==5)||
				(tag==2) || (tag==6))
			{
				pred=sbpred;
				for(i1=0; i1<8; i1++)
				{
					i=i0*8+i1;
					j=ibuf[i];

					k=2;
					if(sbhi>sblo)
					{
//						sba=(sblo+sbhi)/2;
//						k=(256*(j-sba)+127)/(64*(sbhi-sblo));

						k=(256*(j-sblo)+31)/(64*(sbhi-sblo));
						if(k<0)k=0;
						if(k>3)k=3;
					}
					
					blk|=k<<(16+i1*2);

					pred=((sblo*wtab[k][0])+(sbhi*wtab[k][1]))/16;
					pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);

//					p0=(3*p0+pred)/4;
//					p1=(3*p1+ibuf[i])/4;
					p0=(p0+pred)/2;
					p1=(p1+ibuf[i])/2;
					d=p0-p1; d=(d<<1)^(d>>31);

//					d=ibuf[i]-pred; d=(d<<1)^(d>>31);
					err+=d;

					sblo+=sbloadd;
					sbhi+=sbhiadd;
				}
			}
			
			tblk[tag]=blk;
			terr[tag]=err;
			tsbpred1[tag]=sbpred1;
			tsblo1[tag]=sblo1;
			tsbhi1[tag]=sbhi1;
			tpred1[tag]=pred;
			tindex1[tag]=index;
		}

		tag=tmin;
//		for(i=0; i<8; i++)
		for(i=tmin; i<tmax; i++)
			if(terr[i]<terr[tag])
				tag=i;
				
//		tag=4;
//		tag=0;
//		tag=2;

		blk=tblk[tag];
		sbpred1=tsbpred1[tag];
		sblo1=tsblo1[tag];
		sbhi1=tsbhi1[tag];
		pred=tpred1[tag];
		index=tindex1[tag];

		obuf[4+i0*4+0]=blk    ;		obuf[4+i0*4+1]=blk>> 8;
		obuf[4+i0*4+2]=blk>>16;		obuf[4+i0*4+3]=blk>>24;

		sbpred=sbpred1;
		sblo=sblo1;
		sbhi=sbhi1;
	}

	if(ridx)
		{ *ridx=index; }
}

BTEIFGL_API void BGBDT_SndBTAC1C_DecodeBlockMono(
	byte *ibuf, s16 *obuf, int len)
{
	int i;
	
	i=ibuf[2];

	if(i<89)
	{
		BGBDT_MsImaAdpcm_DecodeBlockMono(ibuf, obuf, len);
		return;
	}
	
	if((i>=128) && (i<217))
	{
		BGBDT_SndBTAC1C1_DecodeBlockMonoSRI(ibuf, obuf, len);
		return;
	}
	
	for(i=0; i<len; i++)
		obuf[i]=0;
}

int BGBDT_SndBTAC1C1_ErrorBlockMonoSamples(s16 *ibuf0, s16 *ibuf1, int len)
{
	int p0, p1;
	double e, d;
	int i, j, k;
	
	e=0; p0=0; p1=0;
	for(i=0; i<len; i++)
	{
//		p0=(3*p0+ibuf0[i])/4;
//		p1=(3*p1+ibuf1[i])/4;
//		d=ibuf0[i]-ibuf1[i];

		p0=(p0+ibuf0[i])/2;
		p1=(p1+ibuf1[i])/2;

		d=p0-p1;
//		d=(d<<1)^(d>>31);
//		d=d^(d>>31);

//		d=d>>2;
		d=d*d;
		
		e+=d;
	}
	e=sqrt(e);
	if(e>(1<<30))
		e=(1<<30);
//	e=e/2;
	return(e);
}

BTEIFGL_API void BGBDT_SndBTAC1C1_EncodeBlockMono(
	s16 *ibuf, byte *obuf, int len, int *ridx)
{
	byte b0buf[1024];
	byte b1buf[1024];
	s16 stbuf[2048];
	int e0, e1, bsz, tag, utag;
	int idx, idx0, idx1, iidx0;

//	BGBDT_MsImaAdpcm_EncodeBlockMono(ibuf, obuf, len, ridx);

	bsz=BGBDT_MsImaAdpcm_MonoBlockSizeFromSamples(len);

	iidx0=*ridx;

	idx0=*ridx;
//	if(idx0>128)idx0=0;
	if((idx0<0)||(idx0>88))
		idx0=BGBDT_MsImaAdpcm_MonoInitIndex(ibuf);
	BGBDT_MsImaAdpcm_EncodeBlockMono(ibuf, b0buf, len, &idx0);
	BGBDT_SndBTAC1C_DecodeBlockMono(b0buf, stbuf, len);
	e0=BGBDT_SndBTAC1C1_ErrorBlockMonoSamples(ibuf, stbuf, len);

	memcpy(obuf, b0buf, bsz);
	*ridx=idx0;

	utag=-1;

#if 0
	for(tag=0; tag<5; tag++)
	{
		if(tag==1)
			continue;
		if(tag==2)
			continue;

		if(tag==3)
			continue;

//		idx1=*ridx;
		idx1=iidx0;
		if((idx1>>8)!=tag)
			idx1=0;
		idx1&=127;
		BGBDT_SndBTAC1C1_EncodeBlockMonoSRI(ibuf, b1buf, len, &idx1, tag);
		BGBDT_SndBTAC1C_DecodeBlockMono(b1buf, stbuf, len);
		e1=BGBDT_SndBTAC1C1_ErrorBlockMonoSamples(ibuf, stbuf, len);
		
		if(e1<e0)
		{
			memcpy(obuf, b1buf, bsz);
//			*ridx=idx1;
			*ridx=(128|idx1)|(tag<<8);
			utag=tag;
		}
	}
#endif
	
	if(utag>=0)
	{
		printf("UseTag %d\n", utag);
	}

#if 0
//	idx1=*ridx;
//	BGBDT_SndBTAC1C1_EncodeBlockMonoSRI(ibuf, b1buf, len, &idx1);
//	BGBDT_SndBTAC1C_DecodeBlockMono(b1buf, stbuf, len);
//	e1=BGBDT_SndBTAC1C1_ErrorBlockMonoSamples(ibuf, stbuf, len);

//	printf("UseEn: %d(%d) %d(%d)\n", e0, e0/len, e1, e1/len);
	
	if(e1<e0)
//	if(1)
	{
//		printf("UseE1\n");
		memcpy(obuf, b1buf, bsz);
		*ridx=idx1;
	}else
	{
		memcpy(obuf, b0buf, bsz);
		*ridx=idx0;
	}
#endif
}

BTEIFGL_API void BGBDT_SndBTAC1C1_EncodeBlockMonoLg2(
	s16 *ibuf, byte *obuf, int lg2, int *ridx)
{
	s16 stbuf[2048];
	int len, len2;
	int i, j, k;
	
	len2=(1<<lg2);
	len=len2-8;
	
	for(i=0; i<len2; i++)
	{
		j=i>>(lg2-3);
		stbuf[i-j]=ibuf[i];
	}
	
//	BGBDT_MsImaAdpcm_EncodeBlockMono(stbuf, obuf, len, ridx);
	BGBDT_SndBTAC1C1_EncodeBlockMono(stbuf, obuf, len, ridx);
}
