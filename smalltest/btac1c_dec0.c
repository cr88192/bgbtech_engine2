static int bgbmid_ima_index_table[16] = {
  -1, -1, -1, -1, 2, 4, 6, 8,
  -1, -1, -1, -1, 2, 4, 6, 8
}; 

static int bgbmid_ima_step_table[128] = { 
     7,     8,     9,    10,    11,    12,    13,    14,    16,    17, 
    19,    21,    23,    25,    28,    31,    34,    37,    41,    45, 
    50,    55,    60,    66,    73,    80,    88,    97,   107,   118, 
   130,   143,   157,   173,   190,   209,   230,   253,   279,   307,
   337,   371,   408,   449,   494,   544,   598,   658,   724,   796,
   876,   963,  1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066, 
  2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,
  5894,  6484,  7132,  7845,  8630,  9493, 10442, 11487, 12635, 13899, 
 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 
};

void BGBDT_SndBTAC1C2_DecodeBlockMono(
	byte *ibuf, s16 *obuf, int len);

void BGBDT_SndBTAC1C2_DecodeMonoBlockStereo(
	byte *ibuf, s16 *obuf, int len);
void BGBDT_SndBTAC1C2_DecodeJointBlockStereo(
	byte *ibuf, s16 *obuf, int len);
void BGBDT_SndBTAC1C2_DecodeStereoBlockStereo(
	byte *ibuf, s16 *obuf, int len);

void BGBDT_SndBTAC1C_DecodeBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int i;
	
	i=ibuf[6];

	if(i<89)
	{
		BGBDT_SndBTAC1C2_DecodeStereoBlockStereo(ibuf, obuf, len);
		return;
	}

	if(i==89)
	{
		BGBDT_SndBTAC1C2_DecodeMonoBlockStereo(ibuf, obuf, len);
		return;
	}

	if((i>=128) && (i<217))
	{
		BGBDT_SndBTAC1C2_DecodeJointBlockStereo(ibuf, obuf, len);
		return;
	}
}

void BGBDT_SndBTAC1C_DecodeBlockStereoLg2(
	byte *ibuf, s16 *obuf, int lg2)
{
	int i, j, k, l;
	BGBDT_SndBTAC1C_DecodeBlockStereo(ibuf, obuf, (1<<lg2)-16);
	
	l=1<<lg2;
	for(i=l-1; i>0; i--)
	{
		j=(i+1)>>(lg2-4);
		obuf[i*2+0]=obuf[(i-j)*2+0];
		obuf[i*2+1]=obuf[(i-j)*2+1];
	}
}


void BGBDT_SndBTAC1C_DecodeBlockMonoLg2(
	byte *ibuf, s16 *obuf, int lg2)
{
	int i, j, k, l;

	BGBDT_SndBTAC1C2_DecodeBlockMono(ibuf, obuf, (1<<lg2)-8);
	
	l=1<<lg2;
	for(i=l-1; i>0; i--)
	{
		j=(i+1)>>(lg2-3);
		obuf[i]=obuf[i-j];
	}
}

int BGBDT_SndBTAC1C2_PredictSample(int *psamp, int idx, int pfcn)
{
	int pred, p0, p1;
	int i;
	
	i=idx;

	switch(pfcn)
	{
	case 0:
		pred=psamp[(i-1)&7];
		break;
	case 1:
		pred=2*psamp[(i-1)&7]-psamp[(i-2)&7];
		break;
	case 2:
		pred=(3*psamp[(i-1)&7]-psamp[(i-2)&7])>>1;
		break;
	case 3:
		pred=(5*psamp[(i-1)&7]-psamp[(i-2)&7])>>2;
		break;
	case 4:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]);
		p1=(psamp[(i-2)&7]+psamp[(i-3)&7]);
		pred=p0-(p1>>1);
		break;
	case 5:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]);
		p1=(psamp[(i-2)&7]+psamp[(i-3)&7]);
		pred=(3*p0-p1)>>2;
		break;
	case 6:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]);
		p1=(psamp[(i-2)&7]+psamp[(i-3)&7]);
		pred=(5*p0-p1)>>3;
		break;
	case 7:
		pred=(	18*psamp[(i-1)&7]-4*psamp[(i-2)&7]+
				 3*psamp[(i-3)&7]-2*psamp[(i-4)&7]+
				 1*psamp[(i-5)&7])/16;
		break;
	case 8:
		pred=(	72*psamp[(i-1)&7]-16*psamp[(i-2)&7]+
				12*psamp[(i-3)&7]- 8*psamp[(i-4)&7]+
				 5*psamp[(i-5)&7]- 3*psamp[(i-6)&7]+
				 3*psamp[(i-7)&7]- 1*psamp[(i-8)&7])/64;
		break;
	case 9:
		pred=(	76*psamp[(i-1)&7]-17*psamp[(i-2)&7]+
				10*psamp[(i-3)&7]- 7*psamp[(i-4)&7]+
				 5*psamp[(i-5)&7]- 4*psamp[(i-6)&7]+
				 4*psamp[(i-7)&7]- 3*psamp[(i-8)&7])/64;
		break;
	case 10:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]+psamp[(i-3)&7]+psamp[(i-4)&7]);
		p1=(psamp[(i-5)&7]+psamp[(i-6)&7]+psamp[(i-7)&7]+psamp[(i-8)&7]);
		pred=(5*p0-p1)>>4;
		break;
	case 11:
		p0=(psamp[(i-1)&7]+psamp[(i-2)&7]+psamp[(i-3)&7]+psamp[(i-4)&7]);
		p1=(psamp[(i-5)&7]+psamp[(i-6)&7]+psamp[(i-7)&7]+psamp[(i-8)&7]);
		pred=(p0+p1)>>3;
		break;

	default:
		pred=0;
		break;
	}
	return(pred);
}

void BGBDT_SndBTAC1C2_DecodeBlockMono(
	byte *ibuf, s16 *obuf, int len)
{
	int psamp[8];
	int p0, p1, p2, p3;
	int pred, pfcn, index, step, diff, uni, sni;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2]&127;
	pfcn=ibuf[3]&15;
	
	psamp[0]=pred;	psamp[1]=pred;
	psamp[2]=pred;	psamp[3]=pred;
	psamp[4]=pred;	psamp[5]=pred;
	psamp[6]=pred;	psamp[7]=pred;
	
	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(ibuf[4+(i>>1)]>>((i&1)*4))&15;
		uni=j;

		if(pfcn)
			{ pred=BGBDT_SndBTAC1C2_PredictSample(psamp, i, pfcn); }

		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i]=pred;
		psamp[i&7]=pred;
	}
}

void BGBDT_SndBTAC1C2_DecodeMonoBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int psamp[8];
	int pred, index, step, diff, uni, sni, ofs;
	int pfcn;
	int lp, rp;
	int i, j;
	
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2]&127;
	ofs=(s16)(ibuf[4]+(ibuf[5]<<8));
	pfcn=ibuf[3]&15;
	
	psamp[0]=pred;	psamp[1]=pred;
	psamp[2]=pred;	psamp[3]=pred;
	psamp[4]=pred;	psamp[5]=pred;
	psamp[6]=pred;	psamp[7]=pred;
	
	step=bgbmid_ima_step_table[index&127];

	for(i=0; i<len; i++)
	{
		j=(ibuf[8+(i>>1)]>>((i&1)*4))&15;
		uni=j;

		if(pfcn)
			{ pred=BGBDT_SndBTAC1C2_PredictSample(psamp, i, pfcn); }

		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		rp=(pred<<1)-ofs;
		lp=rp+ofs;

		lp=(lp<(-32768))?(-32768):((lp>32767)?32767:lp);
		rp=(rp<(-32768))?(-32768):((rp>32767)?32767:rp);
		obuf[i*2+0]=lp;
		obuf[i*2+1]=rp;
		psamp[i&7]=pred;
	}
}

void BGBDT_SndBTAC1C2_DecodeJointBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int psamp[8];
	int pred, index, step, diff, uni, sni, ofs;
	int lp, rp, pfcn, ispf;
	int p0, p1, p2, p3;
	int i, j, k, l;
	
	/* decode center channel */
	pred=(s16)(ibuf[0]+(ibuf[1]<<8));
	index=ibuf[2]&127;
	pfcn=ibuf[3]&15;
	step=bgbmid_ima_step_table[index&127];	
	psamp[0]=pred;	psamp[1]=pred;
	psamp[2]=pred;	psamp[3]=pred;
	psamp[4]=pred;	psamp[5]=pred;
	psamp[6]=pred;	psamp[7]=pred;
	
	ispf=(ibuf[3]!=0)|(ibuf[7]!=0);
	
	for(i=0; i<len; i++)
	{
		k=ibuf[8+(i>>2)*2+0]|((ibuf[8+(i>>2)*2+1])<<8);
		j=(k>>((i&3)*3))&7;
		uni=j<<1;
		uni|=((uni&6)==6)&ispf;

		if(pfcn)
			{ pred=BGBDT_SndBTAC1C2_PredictSample(psamp, i, pfcn); }

		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i*2+0]=pred;
		psamp[i&7]=pred;
	}
	
	l=len>>2;

	/* decode side channel */
	pred=(s16)(ibuf[4]+(ibuf[5]<<8));
	index=ibuf[6]&127;
	pfcn=ibuf[7]&15;
	step=bgbmid_ima_step_table[index&127];
	psamp[0]=pred;	psamp[1]=pred;
	psamp[2]=pred;	psamp[3]=pred;
	psamp[4]=pred;	psamp[5]=pred;
	psamp[6]=pred;	psamp[7]=pred;
	
	for(i=0; i<l; i++)
	{
		k=ibuf[8+i*2+0]|((ibuf[8+i*2+1])<<8);
		uni=(k>>12)&15;

		if(pfcn)
			{ pred=BGBDT_SndBTAC1C2_PredictSample(psamp, i, pfcn); }

		index=index+bgbmid_ima_index_table[uni];
		index=(index<0)?0:((index>88)?88:index);
		diff=((2*(uni&7)+1)*step)/8;
		if(uni&8)diff=-diff;
		pred=pred+diff;
		step=bgbmid_ima_step_table[index];

		pred=(pred<(-32768))?(-32768):((pred>32767)?32767:pred);
		obuf[i*8+1]=pred;
		psamp[i&7]=pred;
	}

	/* center/side -> stereo */
	for(i=0; i<l; i++)
	{
		ofs=obuf[i*8+1];
		for(j=0; j<4; j++)
		{
			k=(i*4+j)*2;
			pred=obuf[k+0];
			lp=pred+ofs; rp=lp-(ofs<<1);
			lp=(lp<(-32768))?(-32768):((lp>32767)?32767:lp);
			rp=(rp<(-32768))?(-32768):((rp>32767)?32767:rp);
			obuf[k+0]=lp; obuf[k+1]=rp;
		}
	}
}

void BGBDT_SndBTAC1C2_DecodeStereoBlockStereoI(
	byte *ibuf, s16 *obuf, int len)
{
	int plsamp[8], prsamp[8];
	int lpred, lindex, lstep, ldiff, luni, lsni;
	int rpred, rindex, rstep, rdiff, runi, rsni;
	int pfcn;
	int i, j, k;
	
	lpred=(s16)(ibuf[0]+(ibuf[1]<<8));
	rpred=(s16)(ibuf[4]+(ibuf[5]<<8));
	lindex=ibuf[2];
	rindex=ibuf[6];
	pfcn=ibuf[3]&15;

	lstep=bgbmid_ima_step_table[lindex&127];
	rstep=bgbmid_ima_step_table[rindex&127];

	plsamp[0]=lpred;	plsamp[1]=lpred;
	plsamp[2]=lpred;	plsamp[3]=lpred;
	plsamp[4]=lpred;	plsamp[5]=lpred;
	plsamp[6]=lpred;	plsamp[7]=lpred;
	prsamp[0]=rpred;	prsamp[1]=rpred;
	prsamp[2]=rpred;	prsamp[3]=rpred;
	prsamp[4]=rpred;	prsamp[5]=rpred;
	prsamp[6]=rpred;	prsamp[7]=rpred;

	for(i=0; i<len; i++)
	{
		k=((i>>3)*8)+((i&7)>>1);
		j=(ibuf[ 8+k]>>((i&1)*4))&15;
		luni=j;

		j=(ibuf[12+k]>>((i&1)*4))&15;
		runi=j;

		if(pfcn)
		{
			lpred=BGBDT_SndBTAC1C2_PredictSample(plsamp, i, pfcn);
			rpred=BGBDT_SndBTAC1C2_PredictSample(prsamp, i, pfcn);
		}

		lindex=lindex+bgbmid_ima_index_table[luni];
		lindex=(lindex<0)?0:((lindex>88)?88:lindex);
		ldiff=((2*(luni&7)+1)*lstep)/8;
		if(luni&8)ldiff=-ldiff;
		lpred=lpred+ldiff;
		lstep=bgbmid_ima_step_table[lindex];

		rindex=rindex+bgbmid_ima_index_table[runi];
		rindex=(rindex<0)?0:((rindex>88)?88:rindex);
		rdiff=((2*(runi&7)+1)*rstep)/8;
		if(runi&8)rdiff=-rdiff;
		rpred=rpred+rdiff;
		rstep=bgbmid_ima_step_table[rindex];

		lpred=(lpred<(-32768))?(-32768):((lpred>32767)?32767:lpred);
		rpred=(rpred<(-32768))?(-32768):((rpred>32767)?32767:rpred);
		
		obuf[i*2+0]=lpred;
		obuf[i*2+1]=rpred;
		plsamp[i&7]=lpred;
		prsamp[i&7]=rpred;
	}
}

void BGBDT_SndBTAC1C2_DecodeStereoBlockStereo(
	byte *ibuf, s16 *obuf, int len)
{
	int i, j, k, l;

	BGBDT_SndBTAC1C2_DecodeStereoBlockStereoI(ibuf, obuf, len>>1);
	
	l=len>>1;
	for(i=len-1; i>0; i--)
	{
		j=i>>1;
		obuf[i*2+0]=obuf[j*2+0];
		obuf[i*2+1]=obuf[j*2+1];
	}
}
