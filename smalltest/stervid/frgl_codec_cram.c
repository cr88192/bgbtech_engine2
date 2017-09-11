void FRGL_ConvBGR2RGB(byte *sbuf, byte *tbuf, int sz);
void FRGL_ConvBGRA2RGBA(byte *sbuf, byte *tbuf, int sz);

#if defined(ARCH_SH4LE) || defined(X86) || defined(X86_64)
// #if 0
#define CRAM_GETU16(cs)	(*(u16 *)(cs))
#else
#define CRAM_GETU16(cs)	((cs)[0]|((cs)[1]<<8))
#endif

u32 cram_tclr_rgb555(u16 pxa)
{
	u32 pxc;
	pxc=0xFF000000|
		((pxa&0x7C00)<<9)|
		((pxa&0x03E0)<<6)|
		((pxa&0x001F)<<3);
	return(pxc);
}

int cram_decompress_inner_bgra(byte *ibuf, u32 *obuf,
	int isz, int osz, cram_ctxinfo *info)
{
	u32 aclr[2], bclr[2], cclr[2], dclr[2];
	u32 *bbuf, *bb0, *bb1, *bb2, *bb3;
	u16 px;
	u16 clra, clrb;
	u32 clrc, clrd;
	byte *cs, *cse;
	int xs, ys;
	int n, bx, by, by2, bxs, bys;
	
	xs=info->ohead->biWidth;
	ys=info->ohead->biHeight;
	bxs=xs>>2;
	bys=ys>>2;
	
	cs=ibuf; cse=ibuf+isz; bx=0; by=0;
	by2=bys-by-1;
	while(cs<cse)
	{
//		px=cs[0]|(cs[1]<<8);
		px=CRAM_GETU16(cs);
		if(!px && (by>=bys))
//		if(!px)
			break;
		if(px&0x8000)
		{
			cs+=2;

			if((px&0xFC00)==0x8400)
			{
				n=px&1023;
//				if(!n)n=1024;
				bx+=n;
//				bx+=n+1;
				while(bx>=bxs)
				{
					by++; bx-=bxs;
					by2=bys-by-1;
					if(by>=bys)
						break;
				}
				if(by>=bys)
					break;
				continue;
			}
			
			clrc=cram_tclr_rgb555(px);
			
			bbuf=obuf+(by2*4*xs+bx*4);
			bb0=bbuf;
			bb1=bb0+xs;
			bb2=bb1+xs;
			bb3=bb2+xs;

			bb0[0]=clrc; bb0[1]=clrc; bb0[2]=clrc; bb0[3]=clrc;
			bb1[0]=clrc; bb1[1]=clrc; bb1[2]=clrc; bb1[3]=clrc;
			bb2[0]=clrc; bb2[1]=clrc; bb2[2]=clrc; bb2[3]=clrc;
			bb3[0]=clrc; bb3[1]=clrc; bb3[2]=clrc; bb3[3]=clrc;
			
			bx++;
			if(bx>=bxs)
			{
				by++; bx-=bxs;
				by2=bys-by-1;
				if(by>=bys)
					break;
			}
			continue;
		}

//		clra=cs[2]|(cs[3]<<8);
//		clrb=cs[4]|(cs[5]<<8);
		clra=CRAM_GETU16(cs+2);
		clrb=CRAM_GETU16(cs+4);

		bbuf=obuf+(by2*4*xs+bx*4);
//		bb0=bbuf;		bb1=bb0+xs;
//		bb2=bb1+xs;		bb3=bb2+xs;

		bb3=bbuf;		bb2=bb3+xs;
		bb1=bb2+xs;		bb0=bb1+xs;

		if(clra&0x8000)
		{
			cs+=6;
			aclr[1]=cram_tclr_rgb555(clra);
			aclr[0]=cram_tclr_rgb555(clrb);

#if 1
			bclr[1]=cram_tclr_rgb555(CRAM_GETU16(cs+ 0));
			bclr[0]=cram_tclr_rgb555(CRAM_GETU16(cs+ 2));
			cclr[1]=cram_tclr_rgb555(CRAM_GETU16(cs+ 4));
			cclr[0]=cram_tclr_rgb555(CRAM_GETU16(cs+ 6));
			dclr[1]=cram_tclr_rgb555(CRAM_GETU16(cs+ 8));
			dclr[0]=cram_tclr_rgb555(CRAM_GETU16(cs+10));
#endif

			cs+=12;

#if 1
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=bclr[(px>> 2)&1];	bb0[3]=bclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=bclr[(px>> 6)&1];	bb1[3]=bclr[(px>> 7)&1];
			bb2[0]=cclr[(px>> 8)&1];	bb2[1]=cclr[(px>> 9)&1];
			bb2[2]=dclr[(px>>10)&1];	bb2[3]=dclr[(px>>11)&1];
			bb3[0]=cclr[(px>>12)&1];	bb3[1]=cclr[(px>>13)&1];
			bb3[2]=dclr[(px>>14)&1];	bb3[3]=dclr[(px>>15)&1];
#endif

#if 0
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=aclr[(px>> 2)&1];	bb0[3]=aclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=aclr[(px>> 6)&1];	bb1[3]=aclr[(px>> 7)&1];
			bb2[0]=aclr[(px>> 8)&1];	bb2[1]=aclr[(px>> 9)&1];
			bb2[2]=aclr[(px>>10)&1];	bb2[3]=aclr[(px>>11)&1];
			bb3[0]=aclr[(px>>12)&1];	bb3[1]=aclr[(px>>13)&1];
			bb3[2]=aclr[(px>>14)&1];	bb3[3]=aclr[(px>>15)&1];
#endif
		}else
		{
			cs+=6;
			aclr[1]=cram_tclr_rgb555(clra);
			aclr[0]=cram_tclr_rgb555(clrb);

#if 1
			bb0[0]=aclr[(px>> 0)&1];	bb0[1]=aclr[(px>> 1)&1];
			bb0[2]=aclr[(px>> 2)&1];	bb0[3]=aclr[(px>> 3)&1];
			bb1[0]=aclr[(px>> 4)&1];	bb1[1]=aclr[(px>> 5)&1];
			bb1[2]=aclr[(px>> 6)&1];	bb1[3]=aclr[(px>> 7)&1];
			bb2[0]=aclr[(px>> 8)&1];	bb2[1]=aclr[(px>> 9)&1];
			bb2[2]=aclr[(px>>10)&1];	bb2[3]=aclr[(px>>11)&1];
			bb3[0]=aclr[(px>>12)&1];	bb3[1]=aclr[(px>>13)&1];
			bb3[2]=aclr[(px>>14)&1];	bb3[3]=aclr[(px>>15)&1];
#endif
		}

		bx++;
		if(bx>=bxs)
		{
			by++; bx-=bxs;
			by2=bys-by-1;
			if(by>=bys)
				break;
		}
		continue;
	}
	
	return(0);
}

byte cram_fliptab4[16]={
	0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
	0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};
byte cram_fliptab[256];
byte cram_fliptab_init=0;
u16 cram_frnum;
byte cram_hextab4[16]="0123456789ABCDEF";

#if 0
int cram_decompress_inner_rcon(byte *ibuf, u32 *obuf,
	int isz, int osz, cram_ctxinfo *info)
{
	u32 aclr[2], bclr[2], cclr[2], dclr[2];
	u32 *bbuf, *bb0, *bb1, *bb2, *bb3;
	u32 *conbufa, *conbufb;
	u16 px, px1;
	u16 clra, clrb;
	u32 clrc, clrd;
	byte *cs, *cse;
	int xs, ys;
	int n, bx, by, by2, bxs, bys, rt;
	int i, j, k;
	
	xs=info->ohead->biWidth;
	ys=info->ohead->biHeight;
	bxs=xs>>2;
	bys=ys>>2;
	
	if(!cram_fliptab_init)
	{
		cram_fliptab_init=1;
		for(i=0; i<16; i++)
			for(j=0; j<16; j++)
		{
			k=cram_fliptab4[j]|(cram_fliptab4[i]<<4);
//			k=cram_fliptab4[i]|(cram_fliptab4[j]<<4);
			cram_fliptab[i*16+j]=k;
		}
	}
	
	conbufa=(u32 *)0xA0FF0000;
	conbufb=conbufa+(80*61);

	rt=0;
	cs=ibuf; cse=ibuf+isz; bx=0; by=0;
	by2=bys-by-1;
	while(cs<cse)
	{
		px=CRAM_GETU16(cs);
		if(!px && (by>=bys))
//		if(!px)
		{
			rt=1;
			break;
		}
		if(px&0x8000)
		{
			cs+=2;

			if((px&0xFC00)==0x8400)
			{
				n=px&1023;
				bx+=n;
				while(bx>=bxs)
				{
					by++; bx-=bxs;
					by2=bys-by-1;
					if(by>=bys)
						break;
				}
				if(by>=bys)
				{
					rt=2;
					break;
				}
				continue;
			}
			
			conbufa[by2*80+bx]=0x3055FFFF;
			conbufb[by2*80+bx]=px;
						
			bx++;
			if(bx>=bxs)
			{
				by++; bx-=bxs;
				by2=bys-by-1;
				if(by>=bys)
				{
					rt=3;
					break;
				}
			}
			continue;
		}

//		clra=cs[2]|(cs[3]<<8);
//		clrb=cs[4]|(cs[5]<<8);
		clra=CRAM_GETU16(cs+2);
		clrb=CRAM_GETU16(cs+4);

		bbuf=obuf+(by2*4*xs+bx*4);
//		bb0=bbuf;		bb1=bb0+xs;
//		bb2=bb1+xs;		bb3=bb2+xs;

		bb3=bbuf;		bb2=bb3+xs;
		bb1=bb2+xs;		bb0=bb1+xs;

		if(clra&0x8000)
		{
			cs+=6;
			aclr[1]=clra;
			aclr[0]=clrb;
			bclr[1]=CRAM_GETU16(cs+ 0);
			bclr[0]=CRAM_GETU16(cs+ 2);
			cclr[1]=CRAM_GETU16(cs+ 4);
			cclr[0]=CRAM_GETU16(cs+ 6);
			dclr[1]=CRAM_GETU16(cs+ 8);
			dclr[0]=CRAM_GETU16(cs+10);

			cs+=12;

#if 0
			if((aclr[0]&0x03E0)>(aclr[1]&0x03E0))
				{ px1=aclr[0]; aclr[0]=aclr[1]; aclr[1]=px1; px^=0x0033; }
			if((bclr[0]&0x03E0)>(bclr[1]&0x03E0))
				{ px1=aclr[0]; bclr[0]=bclr[1]; bclr[1]=px1; px^=0x00CC; }

			if((cclr[0]&0x03E0)>(cclr[1]&0x03E0))
				{ px1=cclr[0]; cclr[0]=cclr[1]; cclr[1]=px1; px^=0x3300; }
			if((dclr[0]&0x03E0)>(dclr[1]&0x03E0))
				{ px1=dclr[0]; dclr[0]=dclr[1]; dclr[1]=px1; px^=0xCC00; }

			clra=aclr[0]; clrb=aclr[1];
			if((bclr[0]&0x03E0)<(clra&0x03E0)) { clra=bclr[0]; }
			if((cclr[0]&0x03E0)<(clra&0x03E0)) { clra=cclr[0]; }
			if((dclr[0]&0x03E0)<(clra&0x03E0)) { clra=dclr[0]; }
			if((bclr[1]&0x03E0)>(clrb&0x03E0)) { clrb=bclr[1]; }
			if((cclr[1]&0x03E0)>(clrb&0x03E0)) { clrb=cclr[1]; }
			if((dclr[1]&0x03E0)>(clrb&0x03E0)) { clrb=dclr[1]; }

			px1=(cram_fliptab[(px   )&255]   )|
				(cram_fliptab[(px>>8)&255]<<8);
//			px1=(cram_fliptab[(px   )&255]<<8)|
//				(cram_fliptab[(px>>8)&255]   );

			conbufa[by2*80+bx]=0x30550000|px1;
//			conbufb[by2*80+bx]=(aclr[0]<<16)|aclr[1];
#endif

//			conbufb[by2*80+bx]=(clrb<<16)|clra;
//			conbufb[by2*80+bx]=(clra<<16)|clrb;
			conbufb[by2*80+bx]=1;
		}else
		{
			cs+=6;
//			aclr[1]=cram_tclr_rgb555(clra);
//			aclr[0]=cram_tclr_rgb555(clrb);

			px1=(cram_fliptab[(px   )&255]   )|
				(cram_fliptab[(px>>8)&255]<<8);
//			px1=(cram_fliptab[(px   )&255]<<8)|
//				(cram_fliptab[(px>>8)&255]   );

			conbufa[by2*80+bx]=0x30550000|px1;
			conbufb[by2*80+bx]=(clrb<<16)|clra;
		}

		bx++;
		if(bx>=bxs)
		{
			by++; bx-=bxs;
			by2=bys-by-1;
			if(by>=bys)
			{
				rt=3;
				break;
			}
		}
		continue;
	}

	cram_frnum++;

	conbufa[59*80+68]=0x10070000|'C';
	conbufa[59*80+69]=0x10070000|'R';
	conbufa[59*80+70]=0x10070000|'A';
	conbufa[59*80+71]=0x10070000|'M';
	conbufa[59*80+72]=0x10070000|('0'+rt);

	conbufa[59*80+74]=0x10070000|(cram_hextab4[(cram_frnum>>8)&15]);
	conbufa[59*80+75]=0x10070000|(cram_hextab4[(cram_frnum>>4)&15]);
	conbufa[59*80+76]=0x10070000|(cram_hextab4[(cram_frnum   )&15]);
		
	return(0);
}
#endif

static int cram_decompress_frame_clrs(FRGL_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz, int clrs)
{
	cram_ctxinfo *info;
	int err, i, j, k, ib, ob;
	byte *sbuf, *tbuf;

	info=ctx->data;

	info->ihead->biSizeImage=ssz;
	info->ohead->biSizeImage=dsz;

//	memset(dst, 255, info->ohead->biWidth*info->ohead->biHeight*4);

	if(dst && (clrs==FRGL_JPG_BGRA))
	{
		cram_decompress_inner_bgra(src, dst, ssz, dsz, info);
	}else if(clrs==FRGL_JPG_RAWCON)
	{
//		cram_decompress_inner_rcon(src, info->buffer, ssz, dsz, info);
	}else
	{
		cram_decompress_inner_bgra(src, info->buffer, ssz, dsz, info);
	}

#if 0
	i=(!dst)?ICDECOMPRESS_PREROLL:0;	//frame skip
//	i=ICDECOMPRESS_NOTKEYFRAME;
//	err=ICDecompress(info->hicd, 0, info->ihead, src, info->ohead, dst);
	if(dst && (clrs==FRGL_JPG_BGRA))
	{
		err=ICDecompress(info->hicd, i,
			(BITMAPINFOHEADER *)info->ihead, src,
			(BITMAPINFOHEADER *)info->ohead, dst);
	}else
	{
		err=ICDecompress(info->hicd, i,
			(BITMAPINFOHEADER *)info->ihead, src,
			(BITMAPINFOHEADER *)info->ohead, info->buffer);
	}
#endif

	if(dst && (clrs!=FRGL_JPG_BGRA))
	{
		sbuf=info->buffer;
		tbuf=dst;

		j=info->ohead->biHeight;
		if(j<0)j=-j;
		j=info->ohead->biWidth*j;
		k=info->ohead->biBitCount/8;

		if(clrs==FRGL_JPG_RGBA)
		{
			if(k==3)FRGL_ConvBGR2RGB(sbuf, tbuf, j);
			if(k==4)FRGL_ConvBGRA2RGBA(sbuf, tbuf, j);
		}
	}
	return(0);
}

static int cram_decompress_frame(FRGL_VidCodecCTX *ctx,
	void *src, void *dst, int ssz, int dsz)
{
	return(cram_decompress_frame_clrs(ctx,
		src, dst, ssz, dsz, FRGL_JPG_BGRA));
}

int clamp255(int clr)
{
	if(clr<0)return(0);
	if(clr>255)return(255);
	return(clr);
}

int clamp(int clr, int min, int max)
{
	if(clr<min)return(min);
	if(clr>max)return(max);
	return(clr);
}

int cram_encodeblock(u16 *oblk, byte *iblk, int ystr)
{
	byte pxa[16*4];
	byte pxb[16*4];
	byte pxc[16*4];
	short pxy[16];
	
	u64 sta, stb, stc, std;
	int min, max;
	u32 blk, blka, blkb, blkc;

	int acr, acg, acb, acy;

	int cr, cg, cb, cy;
	int mr, mg, mb, my, mdy;
	int nr, ng, nb, ny, ndy;
	int cu, cv;
	int mu, mv, nu, nv;

	int mr2, mg2, mb2, my2, mu2, mv2;
	int nr2, ng2, nb2, ny2, nu2, nv2;

	int ax, dy;
	int e0, e1;

	int cya, cyb, ctsp;
	int acu, acv, ady;
	int ia, ib, ix, px;
	int ixa, ixb, ixc;
	int i, j, k, l;
	
	my=256; ny=-1;
	acr=0; acg=0; acb=0; acy=0;
	acu=0; acv=0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
		cy=(cr+2*cg+cb)/4;

		k=i*4+j;
//		k=(3-i)*4+j;
		pxc[k*4+0]=cr;
		pxc[k*4+1]=cg;
		pxc[k*4+2]=cb;
		pxc[k*4+3]=255;

		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;

		if(cy<my) { my=cy; }
		if(cy>ny) { ny=cy; }
		
		acr+=cr;	acg+=cg;
		acb+=cb;	acy+=cy;
		acu+=cu;	acv+=cv;
	}
	
	acr=acr/16;	acg=acg/16;
	acb=acb/16;	acy=acy/16;
	acu=acu/16;	acv=acv/16;

	acy=(2*acy+my+ny)/4;

	ax=0; dy=ny-my;
	
	dy=2*(ny-acy);
	
	mr=0; mg=0; mb=0; my=0; cya=0;
	nr=0; ng=0; nb=0; ny=0; cyb=0;
	mdy=0; ndy=0;

	my2=256; ny2=-1;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];

		cy=(cr+2*cg+cb)/4;
		pxy[i*4+j]=cy;
		
		if(cy<acy)
			{ mr+=cr; mg+=cg; mb+=cb; my+=cy; cya++; mdy+=acy-cy; }
		else
			{ nr+=cr; ng+=cg; nb+=cb; ny+=cy; cyb++; ndy+=cy-acy; }

		if(cy<my2)
			{ my2=cy; mr2=cr; mg2=cg; mb2=cb; }
		if(cy>ny2)
			{ ny2=cy; nr2=cr; ng2=cg; nb2=cb; }
	}
	
	if(!cya)cya++;
	if(!cyb)cyb++;

	mr=mr/cya; mg=mg/cya;
	mb=mb/cya; my=my/cya;

	nr=nr/cyb; ng=ng/cyb;
	nb=nb/cyb; ny=ny/cyb;

	mdy=mdy/cya;
	ndy=ndy/cya;
	ady=(mdy+ndy)/2;

	mr=(mr+mr2)/2;	mg=(mg+mg2)/2;
	mb=(mb+mb2)/2;	my=(my+my2)/2;

	nr=(nr+nr2)/2;	ng=(ng+ng2)/2;
	nb=(nb+nb2)/2;	ny=(ny+ny2)/2;
	
	mu=((mb-mg)/2)+128;
	mv=((mr-mg)/2)+128;
	nu=((nb-ng)/2)+128;
	nv=((nr-ng)/2)+128;

	ady=(ady+dy)/2;

	acy=clamp255(acy);	acu=clamp255(acu);
	acv=clamp255(acv);	ady=clamp255(ady);
	my=clamp255(my);	mu=clamp255(mu);	mv=clamp255(mv);
	ny=clamp255(ny);	nu=clamp255(nu);	nv=clamp255(nv);

//	my2=(my+15)>>5;	ny2=(ny+15)>>5;
//	mu2=(mu+31)>>6;	nu2=(nu+31)>>6;
//	mv2=(mv+31)>>6;	nv2=(nv+31)>>6;
//	my2=clamp(my2, 0, 7);	ny2=clamp(ny2, 0, 7);
//	mu2=clamp(mu2, 1, 3);	nu2=clamp(nu2, 1, 3);
//	mv2=clamp(mv2, 1, 3);	nv2=clamp(nv2, 1, 3);

//	ixa=0x8000|(my2<<11)|(ny2<<8)|
//		(mu2<<6)|(mv2<<4)|(nu2<<2)|(nv2<<0);

	mr=clamp255(mr);
	mg=clamp255(mg);
	mb=clamp255(mb);

	nr=clamp255(nr);
	ng=clamp255(ng);
	nb=clamp255(nb);

	ixa=(mr>>3)|((mg>>3)<<5)|((mb>>3)<<10);
	ixb=(nr>>3)|((ng>>3)<<5)|((nb>>3)<<10);

//	ixa=(mr>>3)|((mg>>2)<<5)|((mb>>3)<<11);
//	ixb=(nr>>3)|((ng>>2)<<5)|((nb>>3)<<11);
	
	cya=(5*my+3*ny)/8;
	cyb=(3*my+5*ny)/8;

	px=0;
#if 0
	if(pxy[ 0]>cya)px|=0x0008;
	if(pxy[ 1]>cyb)px|=0x0004;
	if(pxy[ 2]>cya)px|=0x0002;
	if(pxy[ 3]>cyb)px|=0x0001;

	if(pxy[ 4]>cyb)px|=0x0080;
	if(pxy[ 5]>cya)px|=0x0040;
	if(pxy[ 6]>cyb)px|=0x0020;
	if(pxy[ 7]>cya)px|=0x0010;

	if(pxy[ 8]>cya)px|=0x0800;
	if(pxy[ 9]>cyb)px|=0x0400;
	if(pxy[10]>cya)px|=0x0200;
	if(pxy[11]>cyb)px|=0x0100;

	if(pxy[12]>cyb)px|=0x8000;
	if(pxy[13]>cya)px|=0x4000;
	if(pxy[14]>cyb)px|=0x2000;
	if(pxy[15]>cya)px|=0x1000;
#endif

#if 1
	if(pxy[ 0]>cya)px|=0x0001;
	if(pxy[ 1]>cyb)px|=0x0002;
	if(pxy[ 2]>cya)px|=0x0004;
	if(pxy[ 3]>cyb)px|=0x0008;

	if(pxy[ 4]>cyb)px|=0x0010;
	if(pxy[ 5]>cya)px|=0x0020;
	if(pxy[ 6]>cyb)px|=0x0040;
	if(pxy[ 7]>cya)px|=0x0080;

	if(pxy[ 8]>cya)px|=0x0100;
	if(pxy[ 9]>cyb)px|=0x0200;
	if(pxy[10]>cya)px|=0x0400;
	if(pxy[11]>cyb)px|=0x0800;

	if(pxy[12]>cyb)px|=0x1000;
	if(pxy[13]>cya)px|=0x2000;
	if(pxy[14]>cyb)px|=0x4000;
	if(pxy[15]>cya)px|=0x8000;
#endif

#if 0
	if(pxy[ 0]>cya)px|=0x1000;
	if(pxy[ 1]>cyb)px|=0x2000;
	if(pxy[ 2]>cya)px|=0x4000;
	if(pxy[ 3]>cyb)px|=0x8000;

	if(pxy[ 4]>cyb)px|=0x0100;
	if(pxy[ 5]>cya)px|=0x0200;
	if(pxy[ 6]>cyb)px|=0x0400;
	if(pxy[ 7]>cya)px|=0x0800;

	if(pxy[ 8]>cya)px|=0x0010;
	if(pxy[ 9]>cyb)px|=0x0020;
	if(pxy[10]>cya)px|=0x0040;
	if(pxy[11]>cyb)px|=0x0080;

	if(pxy[12]>cyb)px|=0x0001;
	if(pxy[13]>cya)px|=0x0002;
	if(pxy[14]>cyb)px|=0x0004;
	if(pxy[15]>cya)px|=0x0008;
#endif

	blka=px|(ixa<<16);
	blkb=px|(ixb<<16);
	blkc=px|(ixc<<16);

	if(px&0x8000)
	{
		px^=0xFFFF;
		ixc=ixa;
		ixa=ixb;
		ixb=ixc;
	}
	
	oblk[0]=px;
	oblk[1]=ixb;
	oblk[2]=ixa;
	return(0);
}

static int cram_compress_frame(FRGL_VidCodecCTX *ctx,
	void *src, void *dst, int dsz, int qfl, int clrs, int *rfl)
{
	u16 tblk[4];
	cram_ctxinfo *info;
	int err, i, j, k, ib, ob;
	byte *sbuf, *tbuf;
	byte *ct;
	int xs, ys, xsc, ysc, ystr;
	int x, y;

	info=ctx->data;

	xs=info->ihead->biWidth;
	ys=info->ihead->biHeight;
	xsc=(xs+3)/4;
	ysc=(ys+3)/4;
	
	ystr=xs*4;
	
	sbuf=src;
	tbuf=dst;
	ct=tbuf;

	for(y=0; y<ysc; y++)
		for(x=0; x<xsc; x++)
	{
		cram_encodeblock(tblk, sbuf+(y*4*ystr)+(x*4*4), ystr);
		ct[0]=tblk[0];		ct[1]=tblk[0]>>8;
		ct[2]=tblk[1];		ct[3]=tblk[1]>>8;
		ct[4]=tblk[2];		ct[5]=tblk[2]>>8;
		ct+=6;
	}
	
	return(ct-tbuf);
//	return(cram_compress_frame_clrs(ctx,
//		src, dst, ssz, dsz, FRGL_JPG_BGRA));
}

static FRGL_VidCodecCTX *cram_begin_decompress(int fcc,
	FRGL_BITMAPINFOHEADER *in, FRGL_BITMAPINFOHEADER *out)
{
	FRGL_VidCodecCTX *ctx;
	cram_ctxinfo *info;
	int err;

	if(in->biCompression!=RIFF_TAG_CRAM)
		return(NULL);

	ctx=FRGL_VidCodecCTX_New();
//	info=bgbrpi_malloc(sizeof(cram_ctxinfo));
	info=malloc(sizeof(cram_ctxinfo));
	ctx->data=info;

	info->ihead=malloc(sizeof(FRGL_BITMAPINFOHEADER));
	memset(info->ihead, 0, sizeof(FRGL_BITMAPINFOHEADER));
	info->ihead->biSize		= sizeof(FRGL_BITMAPINFOHEADER);
	info->ihead->biWidth		= in->biWidth;
	info->ihead->biHeight		= in->biHeight;
	info->ihead->biPlanes		= in->biPlanes;
	info->ihead->biBitCount		= in->biBitCount;
	info->ihead->biCompression	= in->biCompression;
	info->ihead->biSizeImage	=
		in->biWidth*in->biHeight*in->biBitCount/8;

	info->ohead=malloc(sizeof(FRGL_BITMAPINFOHEADER));
	memset(info->ohead, 0, sizeof(FRGL_BITMAPINFOHEADER));
	info->ohead->biSize		= sizeof(FRGL_BITMAPINFOHEADER);
	info->ohead->biWidth		= out->biWidth;
//	info->ohead->biHeight		= -out->biHeight;
	info->ohead->biHeight		= out->biHeight;
	info->ohead->biPlanes		= out->biPlanes;
	info->ohead->biBitCount		= out->biBitCount;
	info->ohead->biCompression	= out->biCompression;
	info->ihead->biSizeImage	=
		out->biWidth*out->biHeight*out->biBitCount/8;

	info->buffer=malloc(out->biWidth*out->biHeight*out->biBitCount/8);

	ctx->decompress_frame=&cram_decompress_frame;
	ctx->decompress_frame_clrs=&cram_decompress_frame_clrs;

	return(ctx);
}


static FRGL_VidCodecCTX *cram_begin_compress(int fcc,
	FRGL_BITMAPINFOHEADER *in, FRGL_BITMAPINFOHEADER *out)
{
	FRGL_VidCodecCTX *ctx;
	cram_ctxinfo *info;
	int err;

	if(in->biCompression!=RIFF_TAG_CRAM)
		return(NULL);

	ctx=FRGL_VidCodecCTX_New();
//	info=bgbrpi_malloc(sizeof(cram_ctxinfo));
	info=malloc(sizeof(cram_ctxinfo));
	ctx->data=info;

	info->ihead=malloc(sizeof(FRGL_BITMAPINFOHEADER));
	memset(info->ihead, 0, sizeof(FRGL_BITMAPINFOHEADER));
	info->ihead->biSize		= sizeof(FRGL_BITMAPINFOHEADER);
	info->ihead->biWidth		= in->biWidth;
	info->ihead->biHeight		= in->biHeight;
	info->ihead->biPlanes		= in->biPlanes;
	info->ihead->biBitCount		= in->biBitCount;
	info->ihead->biCompression	= in->biCompression;
	info->ihead->biSizeImage	=
		in->biWidth*in->biHeight*in->biBitCount/8;

	info->ohead=malloc(sizeof(FRGL_BITMAPINFOHEADER));
	memset(info->ohead, 0, sizeof(FRGL_BITMAPINFOHEADER));
	info->ohead->biSize		= sizeof(FRGL_BITMAPINFOHEADER);
	info->ohead->biWidth		= out->biWidth;
//	info->ohead->biHeight		= -out->biHeight;
	info->ohead->biHeight		= out->biHeight;
	info->ohead->biPlanes		= out->biPlanes;
	info->ohead->biBitCount		= out->biBitCount;
	info->ohead->biCompression	= out->biCompression;
	info->ihead->biSizeImage	=
		out->biWidth*out->biHeight*out->biBitCount/8;

	info->buffer=malloc(out->biWidth*out->biHeight*out->biBitCount/8);

	ctx->compress_frame=&cram_compress_frame;
//	ctx->compress_frame_clrs=&cram_decompress_frame_clrs;

	return(ctx);
}


static void cram_shutdown()
{
	int i;
}

int FRGL_CodecCRAM_Init()
{
	FRGL_VidCodec *codec;
	int i, j;
	short *sw;
	char buf[256];
	char *t;

	printf("CRAM: Init\n");

	codec=FRGL_VidCodec_New();
	codec->begin_decompress=&cram_begin_decompress;
	codec->begin_compress=&cram_begin_compress;

	return(0);
}
