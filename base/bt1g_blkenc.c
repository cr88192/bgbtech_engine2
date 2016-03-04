/*

Reserve 128 bits.

Y,U,V,d,  Pa,Pb,Pc,Pd

d==0: Flat or Special
  Pa==0: Flat
  Pa==1: 2x2x2bpp
    Pb=Diff
    Pc=Pixel Bits
  Pa==2: 4x4x1bpp
    Pb=Diff
    Pc/Pd=Pixel Bits
  Pa==3: Skip (No Translate)
  Pa==4: Skip+Translate
	Pc=Y Offset
	Pd=X Offset
*/

#include <bteifgl.h>

int clamp255(int i)
	{ return((i<0)?0:((i>255)?255:i)); }
int clamp15(int i)
	{ return((i<0)?0:((i>15)?15:i)); }
int clamp31(int i)
	{ return((i<0)?0:((i>31)?31:i)); }
int clamp63(int i)
	{ return((i<0)?0:((i>63)?63:i)); }
int clamp127(int i)
	{ return((i<0)?0:((i>127)?127:i)); }

void BT1G_EncodeBlockYUY2(byte *block,
	byte *yuv, int xstride, int ystride,
	int dflat, int d2x2)
{
	static const char idxtab[16]=
		{ 0,0,0,0, 0,0,1,1, 2,2,3,3, 3,3,3,3 };
	byte pxy[16];
	int p0, p1, p2, p3, p4, p5, p6, p7;
	int l0, l1, l2, l3a, l3b;
	int mcy, mcu, mcv;
	int ncy, ncu, ncv;
	int acy, acu, acv;
	int cy, cu, cv, dy;
	int cy0, cy1, cy2, cy3;
	int cu0, cv0, cu1, cv1;
	int cu2, cv2, cu3, cv3;
	int i, j, k, l;

#if 0
	mcy=999; ncy=-999;
	for(i=0; i<4; i++)
	{
#if 0
		for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=i*ystride+(j&(~1))*xstride;
			cy=yuv[k]; cu=yuv[l+1]; cv=yuv[l+3];
			pxy[i*4+j]=cy;
			if(cy<mcy) { mcy=cy; mcu=cu; mcv=cv; }
			if(cy>ncy) { ncy=cy; ncu=cu; ncv=cv; }
		}
#endif

#if 0
		for(j=0; j<4; j++)
		{
			k=i*ystride+j*2;
			l=i*ystride+(j&(~1))*2;
			cy=yuv[k]; cu=yuv[l+1]; cv=yuv[l+3];
			pxy[i*4+j]=cy;
			if(cy<mcy) { mcy=cy; mcu=cu; mcv=cv; }
			if(cy>ncy) { ncy=cy; ncu=cu; ncv=cv; }
		}
#endif

#if 1
		k=i*ystride;
		cy0=yuv[k+0];	cu0=yuv[k+1];
		cy1=yuv[k+2];	cv0=yuv[k+3];
		cy2=yuv[k+4];	cu1=yuv[k+5];
		cy3=yuv[k+6];	cv1=yuv[k+7];

		l=i<<2;
		pxy[l+0]=cy0;	pxy[l+1]=cy1;
		pxy[l+2]=cy2;	pxy[l+3]=cy3;

		if(cy0<mcy) { mcy=cy0; mcu=cu0; mcv=cv0; }
		if(cy1<mcy) { mcy=cy1; mcu=cu0; mcv=cv0; }
		if(cy2<mcy) { mcy=cy2; mcu=cu1; mcv=cv1; }
		if(cy3<mcy) { mcy=cy3; mcu=cu1; mcv=cv1; }

		if(cy0>ncy) { ncy=cy0; ncu=cu0; ncv=cv0; }
		if(cy1>ncy) { ncy=cy1; ncu=cu0; ncv=cv0; }
		if(cy2>ncy) { ncy=cy2; ncu=cu1; ncv=cv1; }
		if(cy3>ncy) { ncy=cy3; ncu=cu1; ncv=cv1; }
#endif
	}
#endif

#if 1
	mcy=999; ncy=-999; acu=0; acv=0;
	for(i=0; i<4; i++)
	{
		k=i*ystride;
		cy0=yuv[k+0];	cy1=yuv[k+2];
		cy2=yuv[k+4];	cy3=yuv[k+6];

		l=i<<2;
		pxy[l+0]=cy0;	pxy[l+1]=cy1;
		pxy[l+2]=cy2;	pxy[l+3]=cy3;

		if(cy0<mcy) { mcy=cy0; }
		if(cy1<mcy) { mcy=cy1; }
		if(cy2<mcy) { mcy=cy2; }
		if(cy3<mcy) { mcy=cy3; }

		if(cy0>ncy) { ncy=cy0; }
		if(cy1>ncy) { ncy=cy1; }
		if(cy2>ncy) { ncy=cy2; }
		if(cy3>ncy) { ncy=cy3; }

		cu0=yuv[k+1];		cv0=yuv[k+3];
		cu1=yuv[k+5];		cv1=yuv[k+7];
		acu=acu+cu0+cu1;
		acv=acv+cv0+cv1;
	}
	acu=acu>>3;
	acv=acv>>3;
#endif
	
	acy=(mcy+ncy)>>1;
//	acu=(mcu+ncu)>>1;
//	acv=(mcv+ncv)>>1;
	dy=ncy-acy;

	l1=32768/(ncy-acy+1);		//Fix-Point Scale (Luma)
	l2=65536-2048;

	l1=(32768-6144)/(ncy-acy+1);		//Fix-Point Scale (Luma)
	l3a=65536-1024;
	l3b=65536+1024;

	block[0]=acy;	block[1]=acu;
	block[2]=acv;	block[3]=dy;

	if(dy<dflat)
	{
		block[3]=0;
		block[4]=0;
		block[5]=0;
		block[6]=0;
		block[7]=0;
		return;
	}

	if(dy<d2x2)
	{
		block[3]=0;
		block[4]=1;
		block[5]=i;
		block[6]=0;
		block[7]=0;

		p4=(pxy[ 0]+pxy[ 1]+pxy[ 4]+pxy[ 5])>>2;
		p5=(pxy[ 2]+pxy[ 3]+pxy[ 6]+pxy[ 7])>>2;
		p6=(pxy[ 8]+pxy[ 9]+pxy[12]+pxy[13])>>2;
		p7=(pxy[10]+pxy[11]+pxy[14]+pxy[15])>>2;
		p0=idxtab[((p4-acy)*l1+l3a)>>13];
		p1=idxtab[((p5-acy)*l1+l3b)>>13];
		p2=idxtab[((p6-acy)*l1+l3a)>>13];
		p3=idxtab[((p7-acy)*l1+l3b)>>13];
		block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;

		return;
	}

	p0=idxtab[((pxy[ 0]-acy)*l1+l3a)>>13];
	p1=idxtab[((pxy[ 1]-acy)*l1+l3b)>>13];
	p2=idxtab[((pxy[ 2]-acy)*l1+l3a)>>13];
	p3=idxtab[((pxy[ 3]-acy)*l1+l3b)>>13];
	block[4]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
	p0=idxtab[((pxy[ 4]-acy)*l1+l3b)>>13];
	p1=idxtab[((pxy[ 5]-acy)*l1+l3a)>>13];
	p2=idxtab[((pxy[ 6]-acy)*l1+l3b)>>13];
	p3=idxtab[((pxy[ 7]-acy)*l1+l3a)>>13];
	block[5]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
	p0=idxtab[((pxy[ 8]-acy)*l1+l3a)>>13];
	p1=idxtab[((pxy[ 9]-acy)*l1+l3b)>>13];
	p2=idxtab[((pxy[10]-acy)*l1+l3a)>>13];
	p3=idxtab[((pxy[11]-acy)*l1+l3b)>>13];
	block[6]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
	p0=idxtab[((pxy[12]-acy)*l1+l3b)>>13];
	p1=idxtab[((pxy[13]-acy)*l1+l3a)>>13];
	p2=idxtab[((pxy[14]-acy)*l1+l3b)>>13];
	p3=idxtab[((pxy[15]-acy)*l1+l3a)>>13];
	block[7]=(p0<<6)|(p1<<4)|(p2<<2)|p3;
}

void BTIC1G_EncodeImageYUY2(byte *block,
	byte *yuv, int xs, int ys, int qf)
{
	int df, d2x2, qr;
	int xs1, ys1;
	int i, j;
	
	qr=100-(qf&127);
	if(qr<0)qr=0;
	
	df=qr*0.16;
	d2x2=qr*0.64;
		
	xs1=xs>>2; ys1=ys>>2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		BT1G_EncodeBlockYUY2(
			block+(i*xs1+j)*16,
			yuv+(i*4*xs+j*4)*2,
			2, xs*2, df, d2x2);
	}
}

void BTIC1G_DecodeBlockMB2B(byte *block,
	byte *rgba, int xstride, int ystride)
{
	byte clr[4*4];
	int cy, cu, cv, cd, cy1, cu1, cv1;
	int cr, cg, cb;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int bt, pxb;
	int i, j, k, l;
	
	i=block[3];
	cd=i; bt=0;
	
	if(i==0)
	{
		if(block[4]==0)
		{
			cy=block[0];
			cu=block[1];
			cv=block[2];

			cu1=cu-128; cv1=cv-128;
			cr=65536*cy           +91881*cv1;
			cg=65536*cy- 22554*cu1-46802*cv1;
			cb=65536*cy+116130*cu1;
			cr>>=16; cg>>=16; cb>>=16;

			cr=(cr<0)?0:((cr>255)?255:cr);
			cg=(cg<0)?0:((cg>255)?255:cg);
			cb=(cb<0)?0:((cb>255)?255:cb);

			for(i=0; i<4; i++)
				for(j=0; j<4; j++)
			{
				k=i*ystride+j*xstride;
				rgba[k+0]=cr;
				rgba[k+1]=cg;
				rgba[k+2]=cb;
				rgba[k+3]=255;
			}
			return;
		}	

		if(block[4]==1)
			{ cd=block[5]; bt=1; }
		if(block[4]==2)
			{ cd=block[5]; bt=2; }
	}

	cy=block[0];
	cu=block[1];
	cv=block[2];
//	cd=block[3];

	cy1=cy-cd; cu1=cu-128; cv1=cv-128;
	cr1=65536*cy1           +91881*cv1;
	cg1=65536*cy1- 22554*cu1-46802*cv1;
	cb1=65536*cy1+116130*cu1;
	cr1>>=16; cg1>>=16; cb1>>=16;

	cy1=cy+cd;
	cr2=65536*cy1           +91881*cv1;
	cg2=65536*cy1- 22554*cu1-46802*cv1;
	cb2=65536*cy1+116130*cu1;
	cr2>>=16; cg2>>=16; cb2>>=16;

	clr[ 0]=clamp255(cr1);
	clr[ 1]=clamp255(cg1);
	clr[ 2]=clamp255(cb1);
	clr[ 3]=255;
	clr[12]=clamp255(cr2);
	clr[13]=clamp255(cg2);
	clr[14]=clamp255(cb2);	
	clr[15]=255;

	clr[ 4]=(clr[0]*11+clr[12]*5)>>4;
	clr[ 5]=(clr[1]*11+clr[13]*5)>>4;
	clr[ 6]=(clr[2]*11+clr[14]*5)>>4;
	clr[ 7]=255;
	clr[ 8]=(clr[0]*5+clr[12]*11)>>4;
	clr[ 9]=(clr[1]*5+clr[13]*11)>>4;
	clr[10]=(clr[2]*5+clr[14]*11)>>4;
	clr[11]=255;
	
	if(bt==0)
	{
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=((block[4+i]>>(6-2*j))&3)*4;
			rgba[k+0]=clr[l+0];
			rgba[k+1]=clr[l+1];
			rgba[k+2]=clr[l+2];
			rgba[k+3]=clr[l+3];
		}
	}else if(bt==2)
	{
		pxb=(block[6]<<8)|block[7];
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=(pxb>>((3-i)*4+(3-j)))&1;
			l=(l?3:0)*4;
			rgba[k+0]=clr[l+0];
			rgba[k+1]=clr[l+1];
			rgba[k+2]=clr[l+2];
			rgba[k+3]=clr[l+3];
		}
	}else if(bt==1)
	{
		pxb=block[6];
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=2*(i>>1)+(j>>1);
			l=((pxb>>(6-2*l))&3)*4;
			rgba[k+0]=clr[l+0];
			rgba[k+1]=clr[l+1];
			rgba[k+2]=clr[l+2];
			rgba[k+3]=clr[l+3];
		}
	}
}

void BTIC1G_DecodeImageMB2B(byte *block,
	byte *rgba, int xs, int ys, int stride)
{
	int xs1, ys1;
	int i, j;
	
	xs1=xs>>2; ys1=ys>>2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		BTIC1G_DecodeBlockMB2B(
			block+(i*xs1+j)*16,
			rgba+(i*4*xs+j*4)*stride,
			stride, xs*stride);
	}
}

void BTIC1G_DecodeImageStrideMB2B(byte *block,
	byte *rgba, int xs, int ys,
	int xstride, int ystride)
{
	int xs1, ys1;
	int i, j;
	
	xs1=xs>>2; ys1=ys>>2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		BTIC1G_DecodeBlockMB2B(
			block+(i*xs1+j)*16,
			rgba+(i*4)*ystride+(j*4)*xstride,
			xstride, ystride);
	}
}
