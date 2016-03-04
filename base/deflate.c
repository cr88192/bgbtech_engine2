/*
Deflater

Use Huffman trees to calculate code lengths.
Use the inflater to construct the tables themselves.

Leaves will use negative ID numbers, nodes positive numbers.
May need to make 2 passes, first to an internal representation and secondly to
the deflated format.

Use 24 bits/run (8 bit len, 3..258, 16 bit distance, 1..65536).
Bytes will also be stored here.
Run mask will be stored Externally.

Maybe use a flag to indicate whether to search 32 or 64kB.

Output size is a sane maximum.

*/

#include <bteifgl.h>

// #define PDZ2_CHUNK	(1<<22)

#define PDZ2_CHUNK	262144
// #define PDZ2_CHUNK	65536
// #define PDZ2_CHUNK	16384

extern u16 pdunz_ltab_idx[256];	//literal index table
extern u16 pdunz_dtab_idx[256];	//distance index table

extern u16 pdunz_ltab_code[288];	//literal codes
extern u16 pdunz_ltab_mask[288];	//literal code masks
extern u16 pdunz_ltab_next[288];	//literal table code chains
extern byte pdunz_ltab_len[288];	//literal code lengths

extern u16 pdunz_dtab_code[32];	//distance codes
extern u16 pdunz_dtab_mask[32];	//distance code masks
extern u16 pdunz_dtab_next[32];	//distance table code chains
extern byte pdunz_dtab_len[32];		//distance code lengths

byte *pdz2_ibuf;	//input buffer
byte *pdz2_obuf;	//output buffer
byte *pdz2_cs;		//current input pos
byte *pdz2_ce;		//end of input buffer
byte *pdz2_ct;		//current output pos
byte *pdz2_cte;		//end of output buffer

u32 pdz2_win;		//bit window
int pdz2_pos;		//bit window position

byte pdz2_wbuf[65536];		//window buffer
u16 pdz2_lbuf[65536];	//link buffer
u16 pdz2_hash[4096];		//hash table
u16 pdz2_wpos;		//window position

int pdz2_sdepth;	//search depth
int pdz2_maxdist;	//max distance

#ifdef PDZ2_FIXCHUNK
byte pdz2_tbuf[PDZ2_CHUNK];
byte pdz2_mbuf[(PDZ2_CHUNK+7)/8];
#else
byte *pdz2_tbuf=NULL;
byte *pdz2_mbuf=NULL;
int pdz2_tsz;
int pdz2_msz;
#endif

void PDZ2_WriteBit(int i)
{
	i&=1;
	pdz2_win|=i<<pdz2_pos;
	pdz2_pos++;
	if(pdz2_pos>=8)
	{
		*pdz2_ct++=pdz2_win&0xFF;
		pdz2_win>>=8;
		pdz2_pos-=8;
	}
}

void PDZ2_Write2Bits(int i)
{
	i&=3;
	pdz2_win|=i<<pdz2_pos;
	pdz2_pos+=2;
	if(pdz2_pos>=8)
	{
		*pdz2_ct++=pdz2_win&0xFF;
		pdz2_win>>=8;
		pdz2_pos-=8;
	}
}

void PDZ2_Write3Bits(int i)
{
	i&=7;
	pdz2_win|=i<<pdz2_pos;
	pdz2_pos+=3;
	if(pdz2_pos>=8)
	{
		*pdz2_ct++=pdz2_win&0xFF;
		pdz2_win>>=8;
		pdz2_pos-=8;
	}
}

void PDZ2_Write4Bits(int i)
{
	i&=15;
	pdz2_win|=i<<pdz2_pos;
	pdz2_pos+=4;
	if(pdz2_pos>=8)
	{
		*pdz2_ct++=pdz2_win&0xFF;
		pdz2_win>>=8;
		pdz2_pos-=8;
	}
}

void PDZ2_Write5Bits(int i)
{
	i&=31;
	pdz2_win|=i<<pdz2_pos;
	pdz2_pos+=5;
	if(pdz2_pos>=8)
	{
		*pdz2_ct++=pdz2_win&0xFF;
		pdz2_win>>=8;
		pdz2_pos-=8;
	}
}

void PDZ2_WriteNBits(int i, int n)
{
	i&=((1<<n)-1);
	pdz2_win|=i<<pdz2_pos;
	pdz2_pos+=n;
	while(pdz2_pos>=8)
	{
		*pdz2_ct++=pdz2_win&0xFF;
		pdz2_win>>=8;
		pdz2_pos-=8;
	}
}

void PDZ2_FlushBits()
{
	while(pdz2_pos>0)
	{
		*pdz2_ct++=pdz2_win&0xFF;
		pdz2_win>>=8;
		pdz2_pos-=8;
	}
	pdz2_pos=0;
	pdz2_win=0;
}

int PDZ2_BalanceTree_r(short *nodes, short *nlen, int root, int h, int ml)
{
	int h0, h1, h2, h3;
	int l0, l1, l2;

	if(root<0)
	{
//		printf("L");
		return(0);
	}

//	printf("{");

	h1=PDZ2_BalanceTree_r(nodes, nlen, nodes[root*2+0], h+1, ml);
	h2=PDZ2_BalanceTree_r(nodes, nlen, nodes[root*2+1], h+1, ml);
	h0=((h1>h2)?h1:h2)+1;
	nlen[root]=h0;

	if((h+h0)<=ml)	//depth limit not exceeded
	{
//		printf("}");
		return(h0);
	}

	//ok, so part of the tree is too deep
	if((h1+1)<h2)
	{
		l0=nodes[root*2+1];
//		if(l0<0)return(h0);	//can't rebalance leaves

		l1=nodes[l0*2+1];
		nodes[l0*2+1]=nodes[l0*2+0];
		nodes[l0*2+0]=nodes[root*2+0];
		nodes[root*2+0]=l0;
		nodes[root*2+1]=l1;
	}else if((h2+1)<h1)
	{
		l0=nodes[root*2+0];
//		if(l0<0)return(h0);	//can't rebalance leaves

		l1=nodes[l0*2+0];
		nodes[l0*2+0]=nodes[l0*2+1];
		nodes[l0*2+1]=nodes[root*2+1];
		nodes[root*2+0]=l1;
		nodes[root*2+1]=l0;
	}else
	{
//		printf("bal}");
		//rotating would be ineffective or would make things worse...
		return(h0);
	}

	//recalc depth of modified sub-tree
	l1=nodes[l0*2+0];
	l2=nodes[l0*2+1];
	h1=(l1<0)?0:nlen[l1];
	h2=(l2<0)?0:nlen[l2];
	h3=((h1>h2)?h1:h2)+1;
	nlen[l0]=h3;

	//recalc height of root node
	l1=nodes[root*2+0];
	l2=nodes[root*2+1];
	h1=(l1<0)?0:nlen[l1];
	h2=(l2<0)?0:nlen[l2];
	h0=((h1>h2)?h1:h2)+1;
	nlen[root]=h0;

//	printf("rebal}");

	return(h0);
}

void PDZ2_CalcLengths_r(short *nodes, byte *cl, int root, int h)
{
	if(root<0)
	{
		cl[(-root)-1]=h;
		return;
	}

	PDZ2_CalcLengths_r(nodes, cl, nodes[root*2+0], h+1);
	PDZ2_CalcLengths_r(nodes, cl, nodes[root*2+1], h+1);
}

int PDZ2_BuildLengths(int *stat, int nc, byte *cl, int ml)
{
	static short nodes[1024], nlen[512];
	static short roots[512], clen[512];
	static int cnts[512];
	int nr, nn;
	int i, j, k, l;

	nr=0; nn=0;
	for(i=0; i<nc; i++)
	{
		if(!stat[i])continue;
		roots[nr]=-(i+1);
		cnts[nr]=stat[i];
		clen[nr]=0;
		nr++;
	}

	for(i=0; i<nc; i++)cl[i]=0;
	if(!nr)
	{
		printf("empty tree\n");
		return(-1);
	}


	while(nr>1)
	{
		if(cnts[0]>=cnts[1]) { j=0; k=1; }
			else { j=1; k=0; }
		for(i=2; i<nr; i++)
		{
			if(cnts[i]<=cnts[k])
			{
				j=k; k=i;
				continue;
			}
			if(cnts[i]<=cnts[j])
			{
				j=i;
				continue;
			}
		}

		nlen[nn]=((clen[j]>clen[k])?clen[j]:clen[k])+1;
		nodes[nn*2+0]=roots[j];
		nodes[nn*2+1]=roots[k];

		roots[nr]=nn;
		cnts[nr]=cnts[j]+cnts[k];
		clen[nr]=nlen[nn];

//		printf("%d %d %d\n", cnts[j], cnts[k], cnts[nr]);

		nn++; nr++;

		l=0;
		for(i=0; i<nr; i++)
		{
			if((i==j) || (i==k))continue;
			roots[l]=roots[i];
			cnts[l]=cnts[i];
			clen[l]=clen[i];
			l++;
		}
		nr=l;
	}

	l=roots[0];
	j=clen[0];
	k=j;

	i=4;
	while((i--) && (k>ml))
		k=PDZ2_BalanceTree_r(nodes, nlen, l, 0, ml);
	if(k>ml)
	{
		printf("tree balance failure\n");
		printf("tree depth %d, org %d, %d nodes\n", k, j, nn);
		return(-2);
	}

	PDZ2_CalcLengths_r(nodes, cl, l, 0);
	return(0);
}

int PDZ2_BuildLengthsAdjust(int *stat, int nc, byte *cl, int ml)
{
	int i, j;

	while(1)
	{
		j=PDZ2_BuildLengths(stat, nc, cl, ml);
		if(j<0)
			printf("PDZ2_BuildLengthsAdjust: Huff Fail %d\n");

		for(i=0; i<nc; i++)
			if(stat[i] && !cl[i])
				break;
		if(i>=nc)break;

		printf("PDZ2_BuildLengthsAdjust: Fiddle Adjust\n");
		for(i=0; i<nc; i++)
			stat[i]++;
		continue;
	}
	return(0);
}

void PDZ2_UpdateWindowByte(byte *s)
{
	int h;

	h=(s[0]^(s[1]<<4)^(s[2]<<8))&0xFFF;
//	h=(s[0]^(s[1]<3)^(s[2]<<6))&0xFFF;
	pdz2_lbuf[pdz2_wpos]=pdz2_hash[h];
	pdz2_hash[h]=pdz2_wpos;
	pdz2_wbuf[pdz2_wpos]=*s;
	pdz2_wpos++;
}

void PDZ2_UpdateWindowString(byte *s, int l)
{
	int h;

	while(l--)
	{
		h=(s[0]^(s[1]<<4)^(s[2]<<8))&0xFFF;
//		h=(s[0]^(s[1]<<3)^(s[2]<<6))&0xFFF;
		pdz2_lbuf[pdz2_wpos]=pdz2_hash[h];
		pdz2_hash[h]=pdz2_wpos;
		pdz2_wbuf[pdz2_wpos]=*s++;
		pdz2_wpos++;
	}
}

int PDZ2_LookupString(byte *cs, byte *ce, int *rl, int *rd)
{
	byte *s, *t, *se;
	int i, j, l;
	int bl, bi, md, sd;

	if(pdz2_maxdist>32768)
		{ se=cs+65536; }
	else
		{ se=cs+258; }

	if(ce<se)se=ce;
	if((ce-cs)<3)return(0);

	md=pdz2_maxdist;
	sd=pdz2_sdepth;

	i=(cs[0]^(cs[1]<<4)^(cs[2]<<8))&0xFFF;
	i=pdz2_hash[i];

#if 1
	bl=0; bi=0; l=2;
	while(l && (sd--))
	{
		j=pdz2_wpos-i;
		if(j<=0)j+=65536;
		if(j>=md)break;

		if(i<(65536-258))
		{
			s=cs; t=pdz2_wbuf+i;
			while((s<se) && (*s==*t)) { s++; t++; }
			j=s-cs;
		}else
		{
			s=cs;
			for(j=0; ((*s++)==pdz2_wbuf[(i+j)&0xFFFF]) &&
				(s<se); j++);
		}

		if(j>bl)
		{
			bl=j; bi=i;

			if(bl>=258)break;
			if(bl>64)
			{
				if(sd>256)sd=256;
				if(bl>128)if(sd>128)sd=128;
				if(bl>192)
				{
					if(sd>64)sd=64;
					if(bl>224)if(sd>16)sd=16;
					if(bl>=256)if(sd>4)sd=4;
				}
			}
		}

		j=pdz2_lbuf[i];
		if(j>=i)l--;
		i=j;
	}
#endif

//	printf("ok\n");

	if(bl>2)
	{
		i=pdz2_wpos-bi;
		if(i<0)i+=65536;

		*rl=bl;
		*rd=i;

		return(1);
	}
	return(0);
}

int PDZ2_LZCompressBuffer(byte *ibuf, byte *obuf, byte *mbuf, int sz)
{
	byte *cs, *ce, *ct, *mt;
	int l, d, l2, d2, mi, ni;
	int mp, n, pi, pj;
	int i, j;

//	if(pdz2_sdepth<2)
	if(pdz2_sdepth<1)
	{
		i=PDZ2_LZCompressBufferRLE(ibuf, obuf, mbuf, sz);
		return(i);
	}

	cs=ibuf;
	ce=ibuf+sz;
	ct=obuf; mt=mbuf;

	mp=0; n=0; pi=pdz2_wpos; pj=0;
	mi=PDZ2_LookupString(cs, ce, &l, &d);
	while(cs<ce)
	{
		j=((ce-cs)>=260)?260:(ce-cs);
		for(i=pj; i<j; i++)
			{ pdz2_wbuf[pi]=cs[i]; pi=(pi+1)&0xFFFF; pj++; }

		ni=PDZ2_LookupString(cs+1, ce, &l2, &d2);
		if(mi && ni && (l2>(l+1)))mi=0;

		if(mi)
		{
			*mt|=1<<(mp++);
			if(mp>=8) { mt++; mp-=8; }
			n++;

			if(l>=258)
			{
				*ct++=255;
				*ct++=d&0xFF;
				*ct++=(d>>8)&0xFF;
				*ct++=l&0xFF;
				*ct++=(l>>8)&0xFF;
			}else
			{
				*ct++=l-3;
				*ct++=d&0xFF;
				*ct++=(d>>8)&0xFF;
			}

			PDZ2_UpdateWindowString(cs, l);
			cs+=l;
			pj-=l;

			j=((ce-cs)>=260)?260:(ce-cs);
			for(i=pj; i<j; i++)
				{ pdz2_wbuf[pi]=cs[i];
				pi=(pi+1)&0xFFFF; pj++; }

			mi=PDZ2_LookupString(cs, ce, &l, &d);
			continue;
		}

		*mt&=~(1<<(mp++));
		if(mp>=8) { mt++; mp-=8; }
		n++;

		PDZ2_UpdateWindowByte(cs);
		*ct++=*cs++;
		pj--;

		mi=ni; l=l2; d=d2+1;
	}

	if((ct-obuf)>sz)
	{
		printf("Deflate: LZ Buffer Overflow\n");
		*(int *)-1=-1;
	}

	return(n);
}

int PDZ2_LZCompressBufferRLE(byte *ibuf, byte *obuf, byte *mbuf, int sz)
{
	byte *cs, *ce, *ct, *mt;
	int l, d, l2, d2, mi, ni, mxl;
	int mp, n, pi, pj;
	int i, j, lc;

	cs=ibuf;
	ce=ibuf+sz;
	ct=obuf; mt=mbuf;
	mxl=(pdz2_maxdist>32768)?65536:256;

	mp=0; n=0; pi=pdz2_wpos; pj=0; lc=-1;
	while(cs<ce)
	{
		j=ce-cs;
		if(j>mxl) { j=mxl; }
		for(i=0; i<j; i++)
			if(cs[i]!=lc)
				break;
		l=i; d=1;
//		l=0; d=1;
	
		if(l>=3)
		{
			*mt|=1<<(mp++);
			if(mp>=8) { mt++; mp-=8; }
			n++;

			if(l>=258)
			{
				*ct++=255;
				*ct++=d&0xFF;
				*ct++=(d>>8)&0xFF;
				*ct++=l&0xFF;
				*ct++=(l>>8)&0xFF;
			}else
			{
				*ct++=l-3;
				*ct++=d&0xFF;
				*ct++=(d>>8)&0xFF;
			}

			cs+=l;
			continue;
		}

		*mt&=~(1<<(mp++));
		if(mp>=8) { mt++; mp-=8; }
		n++;

		lc=*cs++;
		*ct++=lc;
	}

	if((ct-obuf)>sz)
	{
		printf("Deflate: LZ Buffer Overflow\n");
		*(int *)-1=-1;
	}

	return(n);
}

void PDZ2_StatLZRun(byte *cs, int *lstat, int *dstat)
{
	static int dbase[]={
		1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
		257, 385, 513, 769, 1025, 1537, 2049, 3073,
		4097, 6145, 8193, 12289, 16385, 24577, 32769, 49153};
	static int dextra[]={
		0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
		7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14};

	static int lbase[]={
		3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
		35, 43, 51, 59, 67, 83, 99, 115,
		131, 163, 195, 227, 258, 0, 0, 0};
	static int lextra[]={
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
		3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 0, 0, 0};

	int i, j, k;

	j=cs[0]+3;
	if(j==258)
	{
		lstat[285]++;
	}else
	{
		for(i=0; i<29; i++)
		{
			k=lbase[i]+(1<<lextra[i]);
			if((j>=lbase[i]) && (j<k))
			{
				lstat[257+i]++;
				break;
			}
		}
	}

	j=cs[1]|(cs[2]<<8);
	for(i=0; i<32; i++)
	{
		k=dbase[i]+(1<<dextra[i]);
		if((j>=dbase[i]) && (j<k))
		{
			dstat[i]++;
			break;
		}
	}
}

void PDZ2_StatLZBuffer(byte *tbuf, byte *mbuf, int n,
	int *lstat, int *dstat)
{
	byte *s;
	int i;

	for(i=0; i<288; i++)lstat[i]=0;
	for(i=0; i<32; i++)dstat[i]=0;

	s=tbuf;
	for(i=0; i<n; i++)
	{
		if(mbuf[i>>3]&(1<<(i&7)))
		{
			PDZ2_StatLZRun(s, lstat, dstat);
//			s+=3;
			if(s[0]==255)	{ s+=5; }
			else			{ s+=3; }
			continue;
		}

		lstat[*s++]++;
	}
	lstat[256]++;
}

void PDZ2_StatLengths(byte *cl, int nc, int *stat)
{
	int i, j, l;

	l=-1;
	for(i=0; i<nc;)
	{
		for(j=0; ((i+j)<nc) && (cl[i+j]==0); j++);
		if(j>138)j=138;
		if(j>10) { stat[18]++; i+=j; l=0; continue; }
		if(j>2) { stat[17]++; i+=j; l=0; continue; }

		for(j=0; ((i+j)<nc) && (cl[i+j]==l); j++);
		if(j>6)j=6;
		if(j>2) { stat[16]++; i+=j; continue; }

		l=cl[i++];
		stat[l]++;
	}
}

void PDZ2_EncodeSymbol(int v)
{
	if(!pdunz_ltab_len[v])
		printf("PDZ2_EncodeSymbol: no symbol %d\n", v);

	PDZ2_WriteNBits(pdunz_ltab_code[v],
		pdunz_ltab_len[v]);
}

void PDZ2_EncodeLengths(byte *cl, int nc)
{
	int i, j, l;

	i=0; l=-1;
	while(i<nc)
	{
		for(j=0; ((i+j)<nc) && (cl[i+j]==0); j++);
		if(j>10)
		{
			if(j>138)j=138;
			PDZ2_EncodeSymbol(18);
			PDZ2_WriteNBits(j-11, 7);
			i+=j;
			l=0;

//			printf("Z%d ", j);
			continue;
		}
		if(j>2)
		{
			PDZ2_EncodeSymbol(17);
			PDZ2_Write3Bits(j-3);
			i+=j;
			l=0;

//			printf("Z%d ", j);
			continue;
		}

		for(j=0; ((i+j)<nc) && (cl[i+j]==l); j++);
		if(j>2)
		{
			if(j>6)j=6;
			PDZ2_EncodeSymbol(16);
			PDZ2_Write2Bits(j-3);
			i+=j;

//			printf("R%d ", j);
			continue;
		}

		l=cl[i++];
		PDZ2_EncodeSymbol(l);

//		printf("L%d ", l);

		if((l<0) || (l>15))printf("problem: bad code length noted\n");
	}
//	printf("\n");
}

void PDZ2_EncodeLZRun(byte *cs)
{
	static int dbase[]={
		1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
		257, 385, 513, 769, 1025, 1537, 2049, 3073,
		4097, 6145, 8193, 12289, 16385, 24577, 32769, 49153};
	static int dextra[]={
		0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
		7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14};

	static int lbase[]={
		3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
		35, 43, 51, 59, 67, 83, 99, 115,
		131, 163, 195, 227, 258, 0, 0, 0};
	static int lextra[]={
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
		3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 0, 0, 0};

	int i, j, k;

	j=cs[0]+3;
	if(j==258)
	{
		PDZ2_EncodeSymbol(285);
		if(pdz2_maxdist>32768)
		{
			k=cs[3]|(cs[4]<<8);
			PDZ2_WriteNBits(k-3, 16);
		}
	}else
	{
		for(i=0; i<29; i++)
		{
			k=lbase[i]+(1<<lextra[i]);
			if((j>=lbase[i]) && (j<k))
			{
				PDZ2_EncodeSymbol(257+i);
				PDZ2_WriteNBits(j-lbase[i], lextra[i]);
				break;
			}
		}
	}

	j=cs[1]|(cs[2]<<8);
	for(i=0; i<32; i++)
	{
		k=dbase[i]+(1<<dextra[i]);
		if((j>=dbase[i]) && (j<k))
		{
			if(!pdunz_dtab_len[i])
				printf("PDZ2_EncodeLZRun: "
					"no dist sym %d, %d\n", i, j);

			PDZ2_WriteNBits(pdunz_dtab_code[i],
				pdunz_dtab_len[i]);

			PDZ2_WriteNBits(j-dbase[i], dextra[i]);
			break;
		}
	}
}

void PDZ2_EncodeLZBuffer(byte *tbuf, byte *mbuf, int n)
{
	byte *s;
	int i;

	s=tbuf;
	for(i=0; i<n; i++)
	{
		if(mbuf[i>>3]&(1<<(i&7)))
		{
			PDZ2_EncodeLZRun(s);
			if(s[0]==255)	{ s+=5; }
			else			{ s+=3; }
			continue;
		}

		PDZ2_EncodeSymbol(*s++);
	}
	PDZ2_EncodeSymbol(256);
}

int PDZ2_EncodeBlockStatic(byte *ibuf, int isz, int last)
{
	byte *tbuf, *mbuf;
	int n;

	tbuf=malloc(isz*2);
	mbuf=malloc((isz*2+7)/8);

	n=PDZ2_LZCompressBuffer(ibuf, tbuf, mbuf, isz);

	PDZ2_WriteBit(last);
	PDZ2_Write2Bits(1);

	PDUNZ_SetupStatic();

	PDZ2_EncodeLZBuffer(tbuf, mbuf, n);

	free(tbuf);
	free(mbuf);

	return(0);
}

int PDZ2_EncodeBlock(byte *ibuf, int isz, int last)
{
	static int lorder[]={
		16, 17, 18, 0, 8,7, 9,6, 10,5, 11,4, 12,3, 13,2, 14,1, 15};

//	static byte *tbuf=NULL, *mbuf=NULL;
//	static int tsz, msz;

	int lstat[288], dstat[32], hstat[24];
	byte lcl[288], dcl[32], hcl[24], hcl2[24];
	int lc, dc, hc;
	int i, j, n;

#ifndef PDZ2_FIXCHUNK
	i=isz;
	if(!pdz2_tbuf || (isz>pdz2_tsz))
	{
		if(pdz2_tbuf)free(pdz2_tbuf);
		if(pdz2_mbuf)free(pdz2_mbuf);
		pdz2_tsz=i; pdz2_msz=(i+7)/8;
		pdz2_tbuf=malloc(pdz2_tsz);
		pdz2_mbuf=malloc(pdz2_msz);
	}
#endif

	n=PDZ2_LZCompressBuffer(ibuf, pdz2_tbuf, pdz2_mbuf, isz);

	PDZ2_StatLZBuffer(pdz2_tbuf, pdz2_mbuf, n, lstat, dstat);

	if(pdz2_sdepth<2)
	{
		for(i=0; i<256; i++)
			lstat[i]++;
		for(i=0; i<8; i++)
			dstat[i]++;
	}

//	PDZ2_BuildLengths(lstat, 288, lcl, 15);
//	PDZ2_BuildLengths(dstat, 32, dcl, 15);
	PDZ2_BuildLengthsAdjust(lstat, 288, lcl, 15);
	PDZ2_BuildLengthsAdjust(dstat, 32, dcl, 15);

	for(i=0; i<32; i++)if(dcl[i])break;
	if(i==32)for(i=0; i<32; i++)dcl[i]=5;

	for(lc=257; lc<286; lc++)
	{
		for(i=lc; i<286; i++)
			if(lcl[i])break;
		if(i==286)break;
	}

	for(dc=1; dc<32; dc++)
	{
		for(i=dc; i<32; i++)
			if(dcl[i])break;
		if(i==32)break;
	}

	for(i=0; i<24; i++)hstat[i]=0;
	PDZ2_StatLengths(lcl, lc, hstat);
	PDZ2_StatLengths(dcl, dc, hstat);
//	PDZ2_BuildLengths(hstat, 24, hcl, 7);
	PDZ2_BuildLengthsAdjust(hstat, 24, hcl, 7);

	for(i=0; i<19; i++)hcl2[i]=hcl[lorder[i]];

	for(hc=4; hc<19; hc++)
	{
		for(i=hc; i<19; i++)
			if(hcl2[i])break;
		if(i==19)break;
	}

	PDZ2_WriteBit(last);
	PDZ2_Write2Bits(2);

	PDZ2_Write5Bits(lc-257);
	PDZ2_Write5Bits(dc-1);
	PDZ2_Write4Bits(hc-4);

	for(i=0; i<hc; i++)
		PDZ2_Write3Bits(hcl[lorder[i]]);

	j=PDUNZ_SetupTable(hcl, 24,
		pdunz_ltab_code, pdunz_ltab_mask, pdunz_ltab_len,
		pdunz_ltab_idx, pdunz_ltab_next);
	if(j<0)return(j);

	PDZ2_EncodeLengths(lcl, lc);
	PDZ2_EncodeLengths(dcl, dc);

	j=PDUNZ_SetupTable(lcl, lc,
		pdunz_ltab_code, pdunz_ltab_mask, pdunz_ltab_len,
		pdunz_ltab_idx, pdunz_ltab_next);
	if(j<0)return(j);

	j=PDUNZ_SetupTable(dcl, dc,
		pdunz_dtab_code, pdunz_dtab_mask, pdunz_dtab_len,
		pdunz_dtab_idx, pdunz_dtab_next);
	if(j<0)return(j);

	PDZ2_EncodeLZBuffer(pdz2_tbuf, pdz2_mbuf, n);

//	free(tbuf);
//	free(mbuf);

	return(0);
}

int PDZ2_EncodeStream_I(byte *ibuf, byte *obuf, int isz, int osz)
{
	byte *s;
	int i, j, k;

	pdz2_ibuf=ibuf;
	pdz2_obuf=obuf;
	pdz2_pos=0;
	pdz2_win=0;

	pdz2_ct=obuf;
	pdz2_cte=obuf+osz;

	memset(pdz2_wbuf, 0, 65536);

	for(i=0; i<4096; i++)pdz2_hash[i]=0;
	pdz2_lbuf[0]=0;
	pdz2_wpos=0xFFFF;

	s=ibuf;
	i=isz;	k=0;

#ifdef PDZ2_CHUNK
	while(i>PDZ2_CHUNK)
	{
//		printf("Encode Block %d\r", k++);

#if 1
		for(j=0; j<4096; j++)pdz2_hash[j]=0;
		pdz2_lbuf[0]=0;
		pdz2_wpos=0xFFFF;
#endif

		j=PDZ2_EncodeBlock(s, PDZ2_CHUNK, 0);
//		j=PDZ2_EncodeBlockStatic(s, PDZ2_CHUNK, 0);
		if(j<0)return(j);
		i-=PDZ2_CHUNK;
		s+=PDZ2_CHUNK;
	}
//	printf("\n");
#endif

	j=PDZ2_EncodeBlock(s, i, 1);
//	j=PDZ2_EncodeBlockStatic(s, i, 1);
	if(j<0)return(j);

	PDZ2_FlushBits();
	return(pdz2_ct-obuf);
}

int PDZ2_EncodeStream(byte *ibuf, byte *obuf, int isz, int osz)
{
	int i;

	pdz2_sdepth=4096;
	pdz2_maxdist=32768;

	i=PDZ2_EncodeStream_I(ibuf, obuf, isz, osz);
	return(i);
}

int PDZ2_EncodeStream64(byte *ibuf, byte *obuf, int isz, int osz)
{
	int i;

	pdz2_sdepth=4096;
	pdz2_maxdist=65536-260;

	i=PDZ2_EncodeStream_I(ibuf, obuf, isz, osz);
	return(i);
}

int PDZ2_EncodeStreamLvl(byte *ibuf, byte *obuf, int isz, int osz, int l)
{
	static int sd[10]=
		{1, 16, 16, 64, 64, 256, 256, 1024, 4096, 16384};
	static int md[10]=
		{1, 4096, 8192, 4096, 8192, 8192, 16384, 16384, 32768, 32768};

	int i;

	pdz2_sdepth=sd[l];
	pdz2_maxdist=md[l];

	i=PDZ2_EncodeStream_I(ibuf, obuf, isz, osz);
	return(i);
}

int PDZ2_EncodeStream64Lvl(byte *ibuf, byte *obuf,
	int isz, int osz, int lvl)
{
//	static int sd[10]=
//		{1, 16, 16, 64, 64, 256, 256, 1024, 4096, 16384};
//	static int sd[10]=
//		{1, 4, 16, 32, 64, 256, 256, 1024, 4096, 16384};
	static int sd[10]=
		{0, 1, 4, 16, 64, 256, 256, 1024, 4096, 16384};
	int i;

//	pdz2_sdepth=4096;
	pdz2_sdepth=sd[lvl];
	pdz2_maxdist=65536-260;

	i=PDZ2_EncodeStream_I(ibuf, obuf, isz, osz);
	return(i);
}

int PDZ2_EncodeStreamLvlZl(byte *ibuf, byte *obuf,
	int isz, int osz, int lvl)
{
	int i, j, k, l;

	j=PDZ2_EncodeStreamLvl(ibuf, obuf+2, isz, osz-6, lvl);

	k=(7<<12)|(8<<8)|(3<<6); l=k%31;
	if(l)k|=31-l;
	obuf[0]=k>>8; obuf[1]=k&0xFF;

	k=BGBBTJ_BufPNG_DataAdler32(ibuf, isz, 1);
	obuf[2+j+0]=(k>>24)&0xFF;
	obuf[2+j+1]=(k>>16)&0xFF;
	obuf[2+j+2]=(k>>8)&0xFF;
	obuf[2+j+3]=k&0xFF;

	j+=6;
	
	return(j);
}

int PDZ2_EncodeStream64LvlZl(byte *ibuf, byte *obuf,
	int isz, int osz, int lvl)
{
	int i, j, k, l;

	j=PDZ2_EncodeStream64Lvl(ibuf, obuf+2, isz, osz-6, lvl);

	k=(8<<12)|(9<<8)|(3<<6); l=k%31;
	if(l)k|=31-l;
	obuf[0]=k>>8; obuf[1]=k&0xFF;

	k=BGBBTJ_BufPNG_DataAdler32(ibuf, isz, 1);
	obuf[2+j+0]=(k>>24)&0xFF;
	obuf[2+j+1]=(k>>16)&0xFF;
	obuf[2+j+2]=(k>>8)&0xFF;
	obuf[2+j+3]=k&0xFF;

	j+=6;
	
	return(j);
}

int PDZ2_EncodeStream64LvlZlTest(byte *ibuf, byte *obuf,
	int isz, int osz, int lvl)
{
	byte *tbuf;
	int i, j, k, l;

	i=PDZ2_EncodeStream64LvlZl(ibuf, obuf, isz, osz, lvl);

	tbuf=malloc(isz+32768);
	PDUNZ_DecodeStreamSzZl(obuf, tbuf, i, isz+32768, &j, 0);
	if(j!=isz)
	{
		printf("PDZ2_EncodeStream64LvlZlTest: Size %d -> %d\n", isz, j);
		free(tbuf);
		return(-1);
	}
	
	for(k=0; k<isz; k++)
		if(tbuf[k]!=ibuf[k])
			break;
	if(k<isz)
	{
		printf("PDZ2_EncodeStream64LvlZlTest: Error @%d/%d\n", k, isz);
		free(tbuf);
		return(-1);
	}
	
	free(tbuf);
	return(i);
}
