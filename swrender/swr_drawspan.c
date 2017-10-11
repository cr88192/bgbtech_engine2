///////////////////////////////////////////////////////////////////////////////
#include <bteifgl.h>

u16 bgbdt_swdivtab[1024];
byte bgbdt_swdivtab_init=0;

bgbdt_swpixel bgbdt_swr_white;

int bgbdt_swdiv_init()
{
	int i, j, k;
	
	if(bgbdt_swdivtab_init)
		return(0);
	bgbdt_swdivtab_init=1;
	
	bgbdt_swdivtab[0]=65535;
	bgbdt_swdivtab[1]=65535;
	for(i=2; i<1024; i++)
	{
		j=65536/i;
		bgbdt_swdivtab[i]=j;
	}
	return(1);
}

int bgbdt_swdiv(int x, int y)
{
	int z;

	if(y<0)
	{
		return(-bgbdt_swdiv(x, -y));
	}
	
	if(y<1024)
	{
		z=(((s64)x)*bgbdt_swdivtab[y])>>16;
		return(z);
	}
	
	z=x/y;
	return(z);
}

int bgbdt_swrcp(int y)
{
	int z;

	if(y<0)
	{
		return(-bgbdt_swrcp(-y));
	}
	
	if(y<1024)
	{
		z=bgbdt_swdivtab[y];
		return(z);
	}
	
	z=65536/y;
	return(z);
}

int bgbdt_swr_log2(int x)
{
	int i, j;
	
	i=x; j=0;
	while(i>1)
		{ i=i>>1; j++; }
	return(j);
}

void *bgbdt_swr_malloc(int sz)
{
	void *p;
	p=malloc(sz);
	memset(p, 0, sz);
	return(p);
}

static int clamp255(int i)
{
	if(i<0)return(0);
	if(i>255)return(255);
	return(i);
}

u32 bgbdt_swr_pix2fbpix(bgbdt_swpixel pix)
{
	u32 v;
	int cr, cg, cb, ca;

	if(pix&0x8000)
	{
		cg=(pix>>7)&0xF8; cg|=(cg>>5);
		cr=(pix>>2)&0xF8; cr|=(cr>>5);
		cb=(pix<<3)&0xF8; cb|=(cb>>5);
//		cg=(pix>>10)&31; cg=(cg<<3)|(cg>>2);
//		cr=(pix>> 5)&31; cr=(cr<<3)|(cr>>2);
//		cb=(pix    )&31; cb=(cb<<3)|(cb>>2);
		ca=255;
	}else
	{
		cg=(pix>>8)&15; cg=(cg<<4)|cg;
		cr=(pix>>4)&15; cr=(cr<<4)|cr;
		cb=(pix   )&15; cb=(cb<<4)|cb;
		ca=(pix>>12)&7; ca=(ca<<5)|(ca<<2)|(ca>>1);
	}
	
	v=(ca<<24)|(cr<<16)|(cg<<8)|(cb);
	return(v);
}

u32 bgbdt_swr_pix2rgba(bgbdt_swpixel pix)
{
	return(bgbdt_swr_pix2fbpix(pix));
}

u32 bgbdt_swr_fbpix2rgba(bgbdt_fbpixel pix)
{
	return(pix);
}


bgbdt_fbpixel bgbdt_swr_pix2modulate(
	bgbdt_swpixel pix, u32 cmod)
{
	u32 v;
	int cr, cg, cb, ca;
	int mr, mg, mb, ma;
	int i, j, k;

#if 0
	if(cmod==0xFFFFFFFF)
	{
		v=bgbdt_swr_pix2rgba(pix);
		return(v);
//		return(pix);
	}
#endif

	if(pix&0x8000)
	{
		cg=(pix>>10)&31; cg=(cg<<3)|(cg>>2);
		cr=(pix>> 5)&31; cr=(cr<<3)|(cr>>2);
		cb=(pix    )&31; cb=(cb<<3)|(cb>>2);
		ca=255;
	}else
	{
		cg=(pix>>8)&15; cg=(cg<<4)|cg;
		cr=(pix>>4)&15; cr=(cr<<4)|cr;
		cb=(pix   )&15; cb=(cb<<4)|cb;
		ca=(pix>>12)&7; ca=(ca<<5)|(ca<<2)|(ca>>1);
	}

#if 1
	if(cmod==0xFFFFFFFF)
	{
		v=(ca<<24)|(cr<<16)|(cg<<8)|cb;
		return(v);
//		return(pix);
	}
#endif
	
	mr=(cmod    )&255;
	mg=(cmod>> 8)&255;
	mb=(cmod>>16)&255;
	ma=(cmod>>24)&255;
	if((ma>=112) && (ma<=144))
	{
		i=(ma-127);
		if(i>=0)
			{ mr<<=i; mg<<=i; mb<<=i; }
		else
			{ mr>>=-i; mg>>=-i; mb>>=-i; }
		ma=255;
	}
	
	cr=(cr*mr)>>8;
	cg=(cg*mg)>>8;
	cb=(cb*mb)>>8;
	ca=(ca*ma)>>8;
	
	if((cr|cg|cb|ca)&(~255))
	{
		cr=clamp255(cr);
		cg=clamp255(cg);
		cb=clamp255(cb);
		ca=clamp255(ca);
	}

	v=(ca<<24)|(cr<<16)|(cg<<8)|cb;

#if 0
	if(ca>=224)
	{
		v=0x8000|
			((cg<<7)&0x7C00)|
			((cr<<2)&0x03E0)|
			((cb>>3)&0x001F);
	}else
	{
		v=	((ca<<7)&0x7000)|
			((cg<<4)&0x0F00)|
			((cr   )&0x00F0)|
			((cb>>4)&0x000F);
	}
#endif

//	v=(ca<<24)|(cr<<16)|(cg<<8)|(cb);
	return(v);
}

bgbdt_fbpixel bgbdt_swr_fbpix2ablend(
	bgbdt_fbpixel spix, bgbdt_fbpixel dpix)
{
	int cr1, cg1, cb1, ca1;
	int cr2, cg2, cb2, ca2;
	int cr3, cg3, cb3, ca3;
	u32 v;

	cb1=(spix    )&255;
	cg1=(spix>> 8)&255;
	cr1=(spix>>16)&255;
	ca1=(spix>>24)&255;

	cb2=(dpix    )&255;
	cg2=(dpix>> 8)&255;
	cr2=(dpix>>16)&255;
	ca2=(dpix>>24)&255;
	
	cr3=(ca1*cr1+(255-ca1)*cr2)>>8;
	cg3=(ca1*cg1+(255-ca1)*cg2)>>8;
	cb3=(ca1*cb1+(255-ca1)*cb2)>>8;
	ca3=(ca1*ca1+(255-ca1)*ca2)>>8;

	v=(ca3<<24)|(cr3<<16)|(cg3<<8)|(cb3);
	return(v);
}


BTEIFGL_API BGBDT_SwContext *BGBDT_SWR_AllocContext(int xs, int ys)
{
	BGBDT_SwContext *ctx;
	int xss1;
	
	bgbdt_swdiv_init();
	
	ctx=bgbdt_swr_malloc(sizeof(BGBDT_SwContext));
	ctx->xs=xs;
	ctx->ys=ys;
	
	xss1=(xs+15)>>4;
	
	ctx->fbuf=bgbdt_swr_malloc(xs*ys*sizeof(bgbdt_fbpixel));
	ctx->zbuf=bgbdt_swr_malloc(xs*ys*sizeof(bgbdt_swzval));
	ctx->eozbuf=bgbdt_swr_malloc(xss1*ys*sizeof(byte));

	ctx->lim_alloc_span=(1<<18);
	
//	ctx->lim_alloc_span=ys*2;
	ctx->lim_alloc_tris=131072;

	bgbdt_swr_white=0xFFFF;
	
	ctx->st_ilace=0;
	
	return(ctx);
}

BTEIFGL_API void BGBDT_SWR_ClearContext(BGBDT_SwContext *ctx)
{
	bgbdt_fbpixel *fpix;
	bgbdt_swzval *zpix;
	byte *eozpix;
	int sxss1, ilst, ilms;
	int x, y;
	int i, j, k, n, eon;
	
	sxss1=(ctx->xs+15)>>4;
	n=ctx->xs*ctx->ys;
	eon=sxss1*ctx->ys;
	
	fpix=ctx->fbuf;
	zpix=ctx->zbuf;
	
//	if(ctx->st_ilace)
//		ctx->st_ilace^=1;

	i=FRGL_CvarGetNum("r_interlace");
	switch(i)
	{
	case 0:
		ctx->st_ilace=0;
		break;
	case 1: case 2:
		if((ctx->st_ilace<2) || (ctx->st_ilace>3))
			ctx->st_ilace=i;
		break;
	case 4:
		if((ctx->st_ilace<4) || (ctx->st_ilace>7))
			ctx->st_ilace=i;
		break;
	}

	switch(ctx->st_ilace)
	{
		case 0: ctx->st_ilace=0; ctx->st_ilace_msk=0; break;
		case 1: ctx->st_ilace=2; ctx->st_ilace_msk=1; break;
		case 2: ctx->st_ilace=3; ctx->st_ilace_msk=1; break;
		case 3: ctx->st_ilace=2; ctx->st_ilace_msk=1; break;
		case 4: ctx->st_ilace=6; ctx->st_ilace_msk=3; break;
		case 5: ctx->st_ilace=7; ctx->st_ilace_msk=3; break;
		case 6: ctx->st_ilace=5; ctx->st_ilace_msk=3; break;
		case 7: ctx->st_ilace=4; ctx->st_ilace_msk=3; break;
	}

#if 0
	for(i=0; i<256; i++)
	{
		ctx->mzbuf_zbuf[i]=0xFFFF;
		ctx->mzbuf_cmax[i]=0x0000;
//		ctx->mzbuf_ccnt[i]=128*128;
		ctx->mzbuf_ccnt[i]=65535;
	}
#endif

	ilst=ctx->st_ilace;
	ilms=ctx->st_ilace_msk;

	for(y=ctx->vclip_miny; y<ctx->vclip_maxy; y++)
	{
//		if(ctx->st_ilace && ((y^ctx->st_ilace)&1))
//		if(ctx->st_ilace && ((y^ctx->st_ilace)&ctx->st_ilace_msk))
		if(((y^ilst)&ilms))
			continue;
	
		fpix=ctx->fbuf+(y*ctx->xs)+ctx->vclip_minx;
		zpix=ctx->zbuf+(y*ctx->xs)+ctx->vclip_minx;
		for(x=ctx->vclip_minx; (x+8)<ctx->vclip_maxx; x+=8)
		{
			fpix[0]=0x0000;		fpix[1]=0x0000;
			fpix[2]=0x0000;		fpix[3]=0x0000;
			fpix[4]=0x0000;		fpix[5]=0x0000;
			fpix[6]=0x0000;		fpix[7]=0x0000;

			zpix[0]=0xFFFF;		zpix[1]=0xFFFF;
			zpix[2]=0xFFFF;		zpix[3]=0xFFFF;
			zpix[4]=0xFFFF;		zpix[5]=0xFFFF;
			zpix[6]=0xFFFF;		zpix[7]=0xFFFF;
			fpix+=8;			zpix+=8;
		}

		for(; x<ctx->vclip_maxx; x++)
			{ *fpix++=0x0000; *zpix++=0xFFFF; }
	}

#if 0
	k=n;
	while(k>8)
	{
		fpix[0]=0x0000;		fpix[1]=0x0000;
		fpix[2]=0x0000;		fpix[3]=0x0000;
		fpix[4]=0x0000;		fpix[5]=0x0000;
		fpix[6]=0x0000;		fpix[7]=0x0000;

//		fpix[0]=0x83E0;		fpix[1]=0x03E0;
//		fpix[2]=0x83E0;		fpix[3]=0x03E0;
//		fpix[4]=0xFC1F;		fpix[5]=0xFC1F;
//		fpix[6]=0xFC1F;		fpix[7]=0xFC1F;

		zpix[0]=0xFFFF;		zpix[1]=0xFFFF;
		zpix[2]=0xFFFF;		zpix[3]=0xFFFF;
		zpix[4]=0xFFFF;		zpix[5]=0xFFFF;
		zpix[6]=0xFFFF;		zpix[7]=0xFFFF;
		fpix+=8;		zpix+=8;	k-=8;
	}

	k=eon; eozpix=ctx->eozbuf;
	while(k>8)
	{
		eozpix[0]=255;	eozpix[1]=255;
		eozpix[2]=255;	eozpix[3]=255;
		eozpix[4]=255;	eozpix[5]=255;
		eozpix[6]=255;	eozpix[7]=255;
		eozpix+=8;	k-=8;
	}
#endif
}

BTEIFGL_API void BGBDT_SWR_GetContextImage(BGBDT_SwContext *ctx,
	u32 *obuf, int xs, int ys)
{
	bgbdt_fbpixel *fpix;
	u32 *opix;
	int cxs, cys;
	int i, j, k, n;
	
	n=ctx->xs*ctx->ys;
	
	fpix=ctx->fbuf;
	opix=obuf;
	k=n;
//	while((k--)>0)
//		{ *opix++=bgbdt_swr_pix2rgba(*fpix++); }

	cxs=ctx->xs;
	cys=ctx->ys;
	if(xs<cxs)cxs=xs;
	if(ys<cys)cys=ys;
	for(i=0; i<cys; i++)
	{
		fpix=ctx->fbuf+(i*ctx->xs);
		opix=obuf+(i*xs);

		for(j=0; j<cxs; j++)
		{
//			*opix++=bgbdt_swr_pix2rgba(*fpix++);
			*opix++=bgbdt_swr_fbpix2rgba(*fpix++);
		}
	}
}

void BGBDT_SWR_DrawSpan_DflZ(BGBDT_SwContext *ctx, BGBDT_SwSpan *span)
{
	BGBDT_SwSpan *spcur;
	bgbdt_fbpixel *fpix;
	bgbdt_swzval *zpix;
	bgbdt_swpixel *tex;

	bgbdt_swzval *mzpix;
	u16 *mzmax, *mzcnt;

	bgbdt_swpixel pix, pix1;
	bgbdt_fbpixel opix;
	int x, y, xa, xb, xs, xd, txs, tys;
	int s, t, z, sd, td, zd, ss, ts, zs;
	int txshr, tyshr, txshl, tyshl, xfd, txbi, tybi;
	int tpx, tpy, tpx1, tpy1;
	int minx, maxx;
	u32 crba, cgaa, csrb;
	u32 crbb, cgab, csga;
	u32 cdrb, cdga, crb, cga;
	u32 c2, ca, cb;
	u16 apval;
	int mzx, mzy, mzi, lmzx;
	int oqcnt;
	
//	bgbdt_swdiv_init();
	
#if 0
	xs=ctx->xs;
	txshr=ctx->tex_xshr;
	tyshr=ctx->tex_yshr;
	txs=65536>>txshr;
	tys=65536>>tyshr;
	tex=ctx->tex_buf;
	txshl=16-txshr;
	tyshl=16-tyshr;
#endif
	
	minx=ctx->vclip_minx;
	maxx=ctx->vclip_maxx;
	
	oqcnt=ctx->oq_cnt;
	
//	apval=0;
	
	spcur=span;
	while(spcur)
	{
#if 1
		xs=ctx->xs;
		txshr=spcur->tex_xshr;
		tyshr=spcur->tex_yshr;
		txs=65536>>txshr;
		tys=65536>>tyshr;
		tex=spcur->tex_buf;
		txshl=16-txshr;
		tyshl=16-tyshr;
		
		txbi=((1<<txshr)-1)>>1;
		tybi=((1<<tyshr)-1)>>1;
#endif

		y=spcur->y;
		xa=spcur->xa;
		xb=spcur->xb;
		xd=xb-xa;

		s=spcur->sa;
		t=spcur->ta;
		z=spcur->za;
		ca=spcur->ca;
		cb=spcur->cb;
		
		
		sd=spcur->sb-s;
		td=spcur->tb-t;
		zd=spcur->zb-z;

		xfd=bgbdt_swrcp(xd);
		ss=(((s64)sd)*xfd)>>16;
		ts=(((s64)td)*xfd)>>16;
		zs=(((s64)zd)*xfd)>>16;

//		if(ca!=cb)
		if(0)
		{
			crba=((ca<<8)&0xFF00FF00);
			cgaa=((ca   )&0xFF00FF00);
			crbb=((cb<<8)&0xFF00FF00);
			cgab=((cb   )&0xFF00FF00);
			cdrb=crbb-crba;
			cdga=cgab-cgaa;

			csrb=	(((((s64)(cdrb&0xFFFF0000))*xfd)>>16)&0xFFFF0000) |
					(((((s64)(cdrb&0x0000FFFF))*xfd)>>16)&0x0000FFFF);
			csga=	(((((s64)(cdga&0xFFFF0000))*xfd)>>16)&0xFFFF0000) |
					(((((s64)(cdga&0x0000FFFF))*xfd)>>16)&0x0000FFFF);

			crb=crba;
			cga=cgaa;
		}else
		{
			crb=((ca<<8)&0xFF00FF00);
			cga=((ca   )&0xFF00FF00);
			csrb=0;		csga=0;
		}

//		if(xa<0)
		if(xa<minx)
		{
			xa=minx-xa;
			s+=ss*xa;		t+=ts*xa;
			z+=zs*xa;
			crb+=csrb*xa;	cga+=csga*xa;
			xa=minx;
		}
		
//		if(xb>xs)
		if(xb>=(maxx-1))
		{
//			xb=xs;
			xb=maxx-1;
		}

		fpix=ctx->fbuf+y*xs+xa;
		zpix=ctx->zbuf+y*xs+xa;
//		apval=((y*251+xa)*251)&0x7FFF;
		apval=((y^xa)*4093)&0x7FFF;

//		tpx=(((u16)s)>>txshr);
//		tpy=(((u16)t)>>tyshr);
//		tpx=(((u16)(s+txbi))>>txshr);
//		tpy=(((u16)(t+tybi))>>tyshr);
//		tpx1=(tpx&0xFFF5)|((tpy&0x0005)<<1);
//		tpy1=(tpy&0xFFFA)|((tpx&0x000A)>>1);

		mzy=y>>8; lmzx=-1;

		if((ca==cb) && (ca==0xFFFFFFFF))
		{
			if(ctx->ztest)
			{
				for(x=xa; x<=xb; x++)
				{				
					if(z<=(*zpix))
					{
#if 0
						mzx=x>>8;
						if(mzx!=lmzx)
						{
							lmzx=mzx;
							mzi=(mzy<<4)+mzx;
							mzpix=ctx->mzbuf_zbuf+mzi;
							mzmax=ctx->mzbuf_cmax+mzi;
							mzcnt=ctx->mzbuf_ccnt+mzi;
						}
						if(z>(*mzmax))*mzmax=z;
						if((*zpix)>(*mzmax))
						{
							*mzcnt--;
							if(!(*mzcnt))
							{
								*mzpix=*mzmax;
								*mzmax=0;
//								*mzcnt=128*128;
								*mzcnt=65535;
							}
						}
#endif

						tpx=(((u16)s)>>txshr);
						tpy=(((u16)t)>>tyshr);
//						tpx=(((u16)(s+txbi))>>txshr);
//						tpy=(((u16)(t+tybi))>>tyshr);
						tpx1=(tpx&0xFFF5)|((tpy&0x0005)<<1);
						tpy1=(tpy&0xFFFA)|((tpx&0x000A)>>1);

//						pix=tex[(tpy<<txshl)+tpx];
						pix=tex[(tpy1<<txshl)+tpx1];
						opix=bgbdt_swr_pix2fbpix(pix);
//						if((opix>>24)>=240)
						if(pix&0x8000)
						{
							*fpix=opix;
							*zpix=z;
							oqcnt++;
						}else
						{
							apval+=((opix>>17)&0xFF00);
							if(apval&0x8000)
							{
								apval&=~0x8000;
								*fpix=opix;
								*zpix=z;
								oqcnt++;
							}
						}
					}
					
					fpix++;
					zpix++;
					s+=ss;
					t+=ts;
					z+=zs;
				}
			}else
			{
				for(x=xa; x<=xb; x++)
				{		
//					tpx=(((u16)s)>>txshr);
//					tpy=(((u16)t)>>tyshr);
					tpx=(((u16)(s+txbi))>>txshr);
					tpy=(((u16)(t+tybi))>>tyshr);
					tpx1=(tpx&0xFFF5)|((tpy&0x0005)<<1);
					tpy1=(tpy&0xFFFA)|((tpx&0x000A)>>1);

//					pix=tex[(tpy<<txshl)+tpx];
					pix=tex[(tpy1<<txshl)+tpx1];
					opix=bgbdt_swr_pix2fbpix(pix);
//					if((opix>>24)>=240)
					if(pix&0x8000)
					{
						*fpix=opix;
						oqcnt++;
					}else
					{
#if 0
						apval+=((opix>>17)&0xFF00);
						if(apval&0x8000)
						{
							apval&=~0x8000;
							*fpix=opix;
							oqcnt++;
						}
#endif
					}
					
					fpix++;
					s+=ss;		t+=ts;
				}
			}

		}else
			if(ctx->ztest)
		{
			for(x=xa; x<=xb; x++)
			{		
				if(z<=(*zpix))
	//			if(1)
				{
#if 0
					mzx=x>>8;
					if(mzx!=lmzx)
					{
						lmzx=mzx;
						mzi=(mzy<<4)+mzx;
						mzpix=ctx->mzbuf_zbuf+mzi;
						mzmax=ctx->mzbuf_cmax+mzi;
						mzcnt=ctx->mzbuf_ccnt+mzi;
					}
					if(z>(*mzmax))*mzmax=z;
					if((*zpix)>(*mzmax))
					{
						*mzcnt--;
						if(!(*mzcnt))
						{
							*mzpix=*mzmax;
							*mzmax=0;
//							*mzcnt=128*128;
							*mzcnt=65535;
						}
					}
#endif

					tpx=(((u16)s)>>txshr);
					tpy=(((u16)t)>>tyshr);
//					tpx=(((u16)(s+txbi))>>txshr);
//					tpy=(((u16)(t+tybi))>>tyshr);
					tpx1=(tpx&0xFFF5)|((tpy&0x0005)<<1);
					tpy1=(tpy&0xFFFA)|((tpx&0x000A)>>1);
					
//					pix=tex[(tpy<<txshl)+tpx];
					pix=tex[(tpy1<<txshl)+tpx1];

					c2=	((crb>>8)&0x00FF00FF)|
						((cga   )&0xFF00FF00);
					opix=bgbdt_swr_pix2modulate(pix, c2);
	//				opix=pix;
					
	//				if(pix1&0x8000)
					if((opix>>24)>=240)
	//				if(1)
					{
						*fpix=opix;
						*zpix=z;
						oqcnt++;
					}else
					{
	//					apval+=(opix&0x7000);
						apval+=((opix>>17)&0xFF00);
						if(apval&0x8000)
						{
							apval&=~0x8000;
	//						*fpix=pix1;
							*fpix=opix;
							*zpix=z;
							oqcnt++;
						}
					}
				}
				
				fpix++;
				zpix++;
				s+=ss;
				t+=ts;
				z+=zs;
				crb+=csrb;
				cga+=csga;
			}
		}else
		{
			for(x=xa; x<xb; x++)
			{		
//				tpx=(((u16)s)>>txshr);
//				tpy=(((u16)t)>>tyshr);
				tpx=(((u16)(s+txbi))>>txshr);
				tpy=(((u16)(t+tybi))>>tyshr);
				tpx1=(tpx&0xFFF5)|((tpy&0x0005)<<1);
				tpy1=(tpy&0xFFFA)|((tpx&0x000A)>>1);
				
//				pix=tex[(tpy<<txshl)+tpx];
				pix=tex[(tpy1<<txshl)+tpx1];

				c2=	((crb>>8)&0x00FF00FF)|
					((cga   )&0xFF00FF00);
				opix=bgbdt_swr_pix2modulate(pix, c2);
				oqcnt++;
				
				if((opix>>24)>=240)
					{ *fpix=opix; }
				else
					{ *fpix=bgbdt_swr_fbpix2ablend(opix, *fpix); }
				
				fpix++;
				s+=ss;
				t+=ts;
				crb+=csrb;
				cga+=csga;
			}
		}
		
		spcur=spcur->next;
	}

	ctx->oq_cnt=oqcnt;
}

BGBDT_SwSpan *BGBDT_SWR_AllocSpan(BGBDT_SwContext *ctx)
{
	BGBDT_SwSpan *sp;
	int i;
	
	sp=ctx->freespan;
	if(sp)
	{
		ctx->freespan=sp->next;
		return(sp);
	}
	
	if(ctx->lim_alloc_span<=0)
		return(NULL);
	ctx->lim_alloc_span-=256;
	
	sp=bgbdt_swr_malloc(256*sizeof(BGBDT_SwSpan));
	for(i=0; i<255; i++)
	{
		sp->next=ctx->freespan;
		ctx->freespan=sp;
		sp++;
	}
	return(sp);
}

void BGBDT_SWR_FreeSpan(BGBDT_SwContext *ctx, BGBDT_SwSpan *sp)
{
	sp->next=ctx->freespan;
	ctx->freespan=sp;
}

void BGBDT_SWR_FreeSpanList(BGBDT_SwContext *ctx, BGBDT_SwSpan *sp)
{
	BGBDT_SwSpan *spcur, *spnxt;
	
	spcur=sp;
	while(spcur)
	{
		spnxt=spcur->next;
		spcur->next=ctx->freespan;
		ctx->freespan=spcur;
		spcur=spnxt;
	}
}


BTEIFGL_API void BGBDT_SWR_DrawTriangleI(
	BGBDT_SwContext *ctx, BGBDT_SwTriangle *tri)
{
	BGBDT_SwTriVert v0, v1, v2;
//	BGBDT_SwSpan *sp, *splst;
	BGBDT_SwSpan *sp;

	bgbdt_swpixel *tex_buf;
	bgbdt_swzval *zpix;
	byte *eozpix;
	int tex_xshr;
	int tex_yshr;

	int y0, y1, y2;
	int yd0, yd1, yd2;
	int yf0, yf1, yf2;
	int xa, xb, xsa, xsb, xc, xsc;
	int za, zb, zsa, zsb, zc, zsc;
	int sa, sb, ssa, ssb, sc, ssc;
	int ta, tb, tsa, tsb, tc, tsc;

	int crba, crbb, csrba, csrbb, crbc, csrbc;
	int cgaa, cgab, csgaa, csgab, cgac, csgac;
	int c0rb, c0ga, c1rb, c1ga, c2rb, c2ga;

	int xta, xtb, zta, ztb;
	int sta, stb, tta, ttb;
	int sxs, sys, sxflip, sxss1;
	int smx, smy, snx, sny;
	int mzx, mzy, mzi, mzskip;
	int xta2, xtb2, zta2, ztb2;
	int y, lvl, ilst, ilms;
	int i, j, k;

	if(tri->v0.y<tri->v1.y)
	{
		if(tri->v0.y<tri->v2.y)
		{
			if(tri->v1.y<tri->v2.y)
			{
				v0=tri->v0;
				v1=tri->v1;
				v2=tri->v2;
			}else if(tri->v1.y>tri->v2.y)
			{
				v0=tri->v0;
				v1=tri->v2;
				v2=tri->v1;
			}else
			{
				if(tri->v1.x<tri->v2.x)
				{
					v0=tri->v0;
					v1=tri->v1;
					v2=tri->v2;
				}else
				{
					v0=tri->v0;
					v1=tri->v2;
					v2=tri->v1;
				}
			}
		}else if(tri->v0.y>tri->v2.y)
		{
			v0=tri->v2;
			v1=tri->v0;
			v2=tri->v1;
		}else
		{
			if(tri->v0.x<tri->v2.x)
			{
				v0=tri->v0;
				v1=tri->v2;
				v2=tri->v1;
			}else
			{
				v0=tri->v2;
				v1=tri->v0;
				v2=tri->v1;
			}
		}
	}else
	{
		if(tri->v0.y<tri->v2.y)
		{
			v0=tri->v1;
			v1=tri->v0;
			v2=tri->v2;
		}else
		{
			if(tri->v1.y<tri->v2.y)
			{
				v0=tri->v1;
				v1=tri->v2;
				v2=tri->v0;
			}else if(tri->v1.y>tri->v2.y)
			{
				v0=tri->v2;
				v1=tri->v1;
				v2=tri->v0;
			}else
			{
				if(tri->v1.x<tri->v2.x)
				{
					v0=tri->v1;
					v1=tri->v2;
					v2=tri->v0;
				}else
				{
					v0=tri->v2;
					v1=tri->v1;
					v2=tri->v0;
				}
			}
		}
	}
	
	sxs=ctx->xs;
	sys=ctx->ys;
	smx=ctx->vclip_minx;
	snx=ctx->vclip_maxx;
	smy=ctx->vclip_miny;
	sny=ctx->vclip_maxy;
	sxss1=(sxs+15)>>4;
	
	y0=v0.y;
	y1=v1.y;
	y2=v2.y;
	
	yd0=y1-y0;
	yd1=y2-y1;
	yd2=y2-y0;
	yf0=bgbdt_swrcp(yd0);
	yf1=bgbdt_swrcp(yd1);
	yf2=bgbdt_swrcp(yd2);

	if(tri->tex && ctx->en_tex2d)
	{
		tex_xshr=16-tri->tex->xshl;
		tex_yshr=16-tri->tex->yshl;

		sta=(v2.s-v0.s)>>tex_xshr;
		tta=(v2.t-v0.t)>>tex_yshr;
		stb=(sta>=0)?sta:(-sta);
		ttb=(tta>=0)?tta:(-tta);
		j=stb+ttb; lvl=0;
		while(j>(yd2*2))
			{ lvl++; j>>=1; }

		if(lvl>6)lvl=6;

//		ctx->tex_buf=tri->tex->mbuf[0];
//		ctx->tex_xshr=16-tri->tex->xshl;
//		ctx->tex_yshr=16-tri->tex->yshl;

		tex_buf=tri->tex->mbuf[lvl];
		tex_xshr=16-(tri->tex->xshl-lvl);
		tex_yshr=16-(tri->tex->yshl-lvl);

		if(!tex_buf)
		{
			tex_buf=tri->tex->mbuf[0];
			tex_xshr=16-tri->tex->xshl;
			tex_yshr=16-tri->tex->yshl;
		}
		
		if(!tex_buf)
		{
			tex_buf=&bgbdt_swr_white;
			tex_xshr=16;	tex_yshr=16;
		}
	}else
	{
//		ctx->tex_buf=&bgbdt_swr_white;
//		ctx->tex_xshr=16;
//		ctx->tex_yshr=16;

		tex_buf=&bgbdt_swr_white;
		tex_xshr=16;	tex_yshr=16;
	}

//	xa=v0.x<<8;		xb=v0.x<<8;
//	za=v0.z<<8;		zb=v0.z<<8;
//	sa=v0.s<<8;		sb=v0.s<<8;
//	ta=v0.t<<8;		tb=v0.t<<8;

//	if(v0.y==v1.y)
	if(0)
	{
		xa=v0.x<<8;		xb=v1.x<<8;
		za=v0.z<<8;		zb=v1.z<<8;
		sa=v0.s<<8;		sb=v1.s<<8;
		ta=v0.t<<8;		tb=v1.t<<8;
	}else
	{
		xa=v0.x<<8;		xb=v0.x<<8;
		za=v0.z<<8;		zb=v0.z<<8;
		sa=v0.s<<8;		sb=v0.s<<8;
		ta=v0.t<<8;		tb=v0.t<<8;
	}

#if 1
	xsa=((s64)(v1.x-v0.x)*256*yf0)>>16;
	zsa=((s64)(v1.z-v0.z)*256*yf0)>>16;
	ssa=((s64)(v1.s-v0.s)*256*yf0)>>16;
	tsa=((s64)(v1.t-v0.t)*256*yf0)>>16;

	xsb=((s64)(v2.x-v0.x)*256*yf2)>>16;
	zsb=((s64)(v2.z-v0.z)*256*yf2)>>16;
	ssb=((s64)(v2.s-v0.s)*256*yf2)>>16;
	tsb=((s64)(v2.t-v0.t)*256*yf2)>>16;

	xsc=((s64)(v2.x-v1.x)*256*yf1)>>16;
	zsc=((s64)(v2.z-v1.z)*256*yf1)>>16;
	ssc=((s64)(v2.s-v1.s)*256*yf1)>>16;
	tsc=((s64)(v2.t-v1.t)*256*yf1)>>16;

//	csrba=((s64)(c1rb-c0rb)*yf0)>>16;
//	csgaa=((s64)(c1ga-c0ga)*yf0)>>16;
//	csrbb=((s64)(c2rb-c0rb)*yf1)>>16;
//	csgab=((s64)(c2ga-c0ga)*yf1)>>16;
//	csrbc=((s64)(c2rb-c1rb)*yf2)>>16;
//	csgac=((s64)(c2ga-c1ga)*yf2)>>16;

//	csrba=((s64)(c1rb-c0rb)*(yf0>>8))>>8;
//	csgaa=((s64)(c1ga-c0ga)*(yf0>>8))>>8;
//	csrbb=((s64)(c2rb-c0rb)*(yf1>>8))>>8;
//	csgab=((s64)(c2ga-c0ga)*(yf1>>8))>>8;
//	csrbc=((s64)(c2rb-c1rb)*(yf2>>8))>>8;
//	csgac=((s64)(c2ga-c1ga)*(yf2>>8))>>8;

//	if((v0.clr!=v1.clr) || (v1.clr!=v2.clr))
	if(0)
	{
		c0rb=((v0.clr<<8)&0xFF00FF00);
		c0ga=((v0.clr   )&0xFF00FF00);
		c1rb=((v1.clr<<8)&0xFF00FF00);
		c1ga=((v1.clr   )&0xFF00FF00);
		c2rb=((v2.clr<<8)&0xFF00FF00);
		c2ga=((v2.clr   )&0xFF00FF00);
		
		csrba=	(((((s64)((c1rb-c0rb)&0xFFFF0000))*yf0)>>16)&0xFFFF0000)|
				(((((s64)((c1rb-c0rb)&0x0000FFFF))*yf0)>>16)&0x0000FFFF);
		csgaa=	(((((s64)((c1ga-c0ga)&0xFFFF0000))*yf0)>>16)&0xFFFF0000)|
				(((((s64)((c1ga-c0ga)&0x0000FFFF))*yf0)>>16)&0x0000FFFF);

		csrbb=	(((((s64)((c2rb-c0rb)&0xFFFF0000))*yf2)>>16)&0xFFFF0000)|
				(((((s64)((c2rb-c0rb)&0x0000FFFF))*yf2)>>16)&0x0000FFFF);
		csgab=	(((((s64)((c2ga-c0ga)&0xFFFF0000))*yf2)>>16)&0xFFFF0000)|
				(((((s64)((c2ga-c0ga)&0x0000FFFF))*yf2)>>16)&0x0000FFFF);

		csrbc=	(((((s64)((c2rb-c1rb)&0xFFFF0000))*yf1)>>16)&0xFFFF0000)|
				(((((s64)((c2rb-c1rb)&0x0000FFFF))*yf1)>>16)&0x0000FFFF);
		csgac=	(((((s64)((c2ga-c1ga)&0xFFFF0000))*yf1)>>16)&0xFFFF0000)|
				(((((s64)((c2ga-c1ga)&0x0000FFFF))*yf1)>>16)&0x0000FFFF);
	}else
	{
		c0rb=((v0.clr<<8)&0xFF00FF00);
		c0ga=((v0.clr   )&0xFF00FF00);
		csrba=0;		csgaa=0;
		csrbb=0;		csgab=0;
		csrbc=0;		csgac=0;
	}
#endif

	crba=c0rb;	cgaa=c0ga;	
	crbb=c0rb;	cgab=c0ga;

#if 0
	xsa=bgbdt_swdiv((v1.x-v0.x)*255, yd0);
	zsa=bgbdt_swdiv((v1.z-v0.z)*255, yd0);
	ssa=bgbdt_swdiv((v1.s-v0.s)*255, yd0);
	tsa=bgbdt_swdiv((v1.t-v0.t)*255, yd0);

	xsb=bgbdt_swdiv((v2.x-v0.x)*255, yd2);
	zsb=bgbdt_swdiv((v2.z-v0.z)*255, yd2);
	ssb=bgbdt_swdiv((v2.s-v0.s)*255, yd2);
	tsb=bgbdt_swdiv((v2.t-v0.t)*255, yd2);

	xsc=bgbdt_swdiv((v2.x-v1.x)*255, yd1);
	zsc=bgbdt_swdiv((v2.z-v1.z)*255, yd1);
	ssc=bgbdt_swdiv((v2.s-v1.s)*255, yd1);
	tsc=bgbdt_swdiv((v2.t-v1.t)*255, yd1);
#endif

	sxflip=0;
	if(xsb<xsa)
	{
		k=xsa; xsa=xsb; xsb=k;
		k=zsa; zsa=zsb; zsb=k;
		k=ssa; ssa=ssb; ssb=k;
		k=tsa; tsa=tsb; tsb=k;
		sxflip=1;
	}
		
//	splst=NULL;
	ilst=ctx->st_ilace;
	ilms=ctx->st_ilace_msk;

	y=y0;
	while(y<=y2)
	{
		if(y==y1)
		{
			if(!sxflip)
			{
				xsa=xsc;		zsa=zsc;
				ssa=ssc;		tsa=tsc;
				csrba=csrbc;	csgaa=csgac;
				xa=v1.x<<8;		za=v1.z<<8;
				sa=v1.s<<8;		ta=v1.t<<8;
			}else
			{
				xsb=xsc;		zsb=zsc;
				ssb=ssc;		tsb=tsc;
				csrbb=csrbc;	csgab=csgac;
				xb=v1.x<<8;		zb=v1.z<<8;
				sb=v1.s<<8;		tb=v1.t<<8;
			}
		}

		mzskip=0;
//		if(ctx->st_ilace && ((y^ctx->st_ilace)&1))
//		if(ctx->st_ilace && ((y^ctx->st_ilace)&ctx->st_ilace_msk))
		if((y^ilst)&ilms)
			mzskip=1;
	
//		if((y<0) || (y>=sys) || (xb<=0) || (xa>=(sxs<<8)))
		if((y<smy) || (y>=sny) || (xb<=(smx<<8)) || (xa>=(snx<<8)) || mzskip)
		{
			xa+=xsa;		xb+=xsb;
			za+=zsa;		zb+=zsb;
			sa+=ssa;		sb+=ssb;
			ta+=tsa;		tb+=tsb;
			crba+=csrba;	crbb+=csrbb;
			cgaa+=csgaa;	cgab+=csgab;
			y++;
			continue;
		}
		
//		xta=xa>>8;		zta=za>>8;
//		sta=sa>>8;		tta=ta>>8;
//		xtb=xb>>8;		ztb=zb>>8;
//		stb=sb>>8;		ttb=tb>>8;

		xta=(xa+127)>>8;	zta=(za+127)>>8;
		sta=(sa+127)>>8;	tta=(ta+127)>>8;
//		xta=(xa    )>>8;	zta=(za+127)>>8;
//		sta=(sa    )>>8;	tta=(ta+127)>>8;
		xtb=(xb+127)>>8;	ztb=(zb+127)>>8;
		stb=(sb+127)>>8;	ttb=(tb+127)>>8;

//		mzskip=0;

#if 1
		zpix=ctx->zbuf+y*ctx->xs;
		eozpix=ctx->eozbuf+y*sxss1;
		
		xta2=xta; xtb2=xtb;
		if(xta2<smx)xta2=smx;
		if(xtb2>snx)xtb2=snx;
		zta2=zta; ztb2=zta;
		if(ztb<zta2)zta2=ztb;
		if(ztb>ztb2)ztb2=ztb;

#if 0
		xta2=(xta2   )>>4;		xtb2=(xtb2+15)>>4;
		zta2=(zta2>>8);			ztb2=((ztb2+255)>>8);
		if((zta2>eozpix[xta2]) && (zta2>eozpix[xtb2]))
		{
			for(j=xta2; j<=xtb2; j++)
				if(zta2<eozpix[j])
					break;
			if(j>xtb2)
				mzskip=1;
		}
		if(!mzskip)
		{
			xta2=(xta2+15)>>4;
			xtb2=(xtb2   )>>4;
			for(j=xta2; j<xtb2; j++)
			{
				if(ztb2<eozpix[j])
					eozpix[j]=ztb2;
			}
		}
#endif

#if 1
		if((zta2>zpix[xta2]) && (zta2>zpix[xtb2]))
		{
			for(j=xta2; j<xtb2; j++)
				if(zta2<zpix[j])
					break;
			if(j>=xtb2)
				mzskip=1;
		}
#endif
#endif

#if 0
		if((xta>>8)==(xtb>>8))
		{
			mzx=xta>>8;
			mzy=y>>8;
			mzi=(mzy<<4)|mzx;
			
			if((zta>ctx->mzbuf_zbuf[mzi]) &&
				(ztb>ctx->mzbuf_zbuf[mzi]))
			{
			}
		}
#endif

		if(!mzskip)
		{
			sp=BGBDT_SWR_AllocSpan(ctx);
			sp->y=y;
			sp->xa=xta;		sp->xb=xtb;
			sp->za=zta;		sp->zb=ztb;
			sp->sa=sta;		sp->sb=stb;
			sp->ta=tta;		sp->tb=ttb;

			sp->ca=	((crba>>8)&0x00FF00FF)|
					((cgaa   )&0xFF00FF00);
			sp->cb=	((crbb>>8)&0x00FF00FF)|
					((cgab   )&0xFF00FF00);

			sp->tex_buf=tex_buf;
			sp->tex_xshr=tex_xshr;
			sp->tex_yshr=tex_yshr;

	//		sp->next=splst;
	//		splst=sp;

			sp->next=ctx->splst[y];
			ctx->splst[y]=sp;
		}

		xa+=xsa;		xb+=xsb;
		za+=zsa;		zb+=zsb;
		sa+=ssa;		sb+=ssb;
		ta+=tsa;		tb+=tsb;
		crba+=csrba;	crbb+=csrbb;
		cgaa+=csgaa;	cgab+=csgab;
		y++;
	}
	
//	BGBDT_SWR_DrawSpan_DflZ(ctx, splst);
//	BGBDT_SWR_FreeSpanList(ctx, splst);
}

BTEIFGL_API void BGBDT_SWR_DrawTriangleList(
	BGBDT_SwContext *ctx, BGBDT_SwTriangle *trilst)
{
	BGBDT_SwSpan *splst;
	BGBDT_SwTriangle *tri;
	int i;
	
	tri=trilst;
	while(tri)
	{
		BGBDT_SWR_DrawTriangleI(ctx, tri);
		tri=tri->next;
	}
	
	for(i=0; i<ctx->ys; i++)
	{
		splst=ctx->splst[i];
		ctx->splst[i]=NULL;
		BGBDT_SWR_DrawSpan_DflZ(ctx, splst);
		BGBDT_SWR_FreeSpanList(ctx, splst);
	}
}

BTEIFGL_API BGBDT_SwTriangle *BGBDT_SWR_AllocTriangle(BGBDT_SwContext *ctx)
{
	BGBDT_SwTriangle *sp;
	int i;
	
	sp=ctx->freetris;
	if(sp)
	{
		ctx->freetris=sp->next;
		sp->next=NULL;
		return(sp);
	}
	
	if(ctx->lim_alloc_tris<=0)
		return(NULL);
	ctx->lim_alloc_tris-=256;

	sp=bgbdt_swr_malloc(256*sizeof(BGBDT_SwTriangle));
	for(i=0; i<255; i++)
	{
		sp->next=ctx->freetris;
		ctx->freetris=sp;
		sp++;
	}

	sp->next=NULL;
	return(sp);
}

BTEIFGL_API void BGBDT_SWR_FreeTriangle(
	BGBDT_SwContext *ctx, BGBDT_SwTriangle *sp)
{
	sp->next=ctx->freetris;
	ctx->freetris=sp;
}

BTEIFGL_API void BGBDT_SWR_FreeTriangleList(
	BGBDT_SwContext *ctx, BGBDT_SwTriangle *sp)
{
	BGBDT_SwTriangle *spcur, *spnxt;
	
	spcur=sp;
	while(spcur)
	{
		spnxt=spcur->next;
		spcur->next=ctx->freetris;
		ctx->freetris=spcur;
		spcur=spnxt;
	}
}


BTEIFGL_API BGBDT_SwTexture *BGBDT_SWR_LookupTexture(
	BGBDT_SwContext *ctx, int id)
{
	BGBDT_SwTexture *tcur;
	
	tcur=ctx->texlist;
	while(tcur)
	{
		if(tcur->id==id)
			return(tcur);
		tcur=tcur->next;
	}
	return(NULL);
}

BTEIFGL_API BGBDT_SwTexture *BGBDT_SWR_GetTexture(
	BGBDT_SwContext *ctx, int id)
{
	BGBDT_SwTexture *tcur;
	
	tcur=BGBDT_SWR_LookupTexture(ctx, id);
	if(tcur)
		return(tcur);
	
	tcur=bgbdt_swr_malloc(sizeof(BGBDT_SwTexture));
	tcur->id=id;
	tcur->next=ctx->texlist;
	ctx->texlist=tcur;
	return(tcur);
}

BTEIFGL_API void BGBDT_SWR_UpdateTextureImage(
	BGBDT_SwContext *ctx, BGBDT_SwTexture *tex,
	u32 *pixbuf, int mip, int xs, int ys, int clrs)
{
	bgbdt_swpixel *pbuf;
	bgbdt_swpixel v1;
	u32 v;
	int mxs, mys, tpx, tpy, tpx1, tpy1;
	int cr, cg, cb, ca;
	int i, j, k, n;

	if(xs&(xs-1))
		return;
	if(ys&(ys-1))
		return;

	if(!mip)
	{
		tex->txs=xs;
		tex->tys=ys;
		tex->xshl=bgbdt_swr_log2(xs);
		tex->yshl=bgbdt_swr_log2(ys);
		if(!tex->mbuf[0])
			tex->mbuf[0]=bgbdt_swr_malloc(xs*ys*sizeof(bgbdt_swpixel));
		pbuf=tex->mbuf[0];
	}else
	{
		mxs=(tex->txs>>mip);
		mys=(tex->tys>>mip);
		if(!mxs)mxs=1;
		if(!mys)mys=1;
		if((xs!=mxs) || (ys!=mys))
			return;

		if(!tex->mbuf[mip])
			tex->mbuf[mip]=bgbdt_swr_malloc(xs*ys*sizeof(bgbdt_swpixel));
		pbuf=tex->mbuf[mip];
	}
	
	n=xs*ys;
//	for(i=0; i<n; i++)
	for(tpy=0; tpy<ys; tpy++)
		for(tpx=0; tpx<xs; tpx++)
	{
//		v=pixbuf[i];
		v=pixbuf[tpy*xs+tpx];
		switch(clrs)
		{
		case BTIC1H_PXF_RGBA:
			cr=(v    )&255;
			cg=(v>> 8)&255;
			cb=(v>>16)&255;
			ca=(v>>24)&255;
			break;
		case BTIC1H_PXF_BGRA:
			cb=(v    )&255;
			cg=(v>> 8)&255;
			cr=(v>>16)&255;
			ca=(v>>24)&255;
			break;
		default:
			cr=((tpx^tpy)&1)?255:0;
			cg=((tpx^tpy)&1)?0:255;
			cb=((tpx^tpy)&1)?255:0;
			ca=255;
			break;
		}
		
		if(ca>=224)
		{
			v1=0x8000|
				((cg<<7)&0x7C00)|
				((cr<<2)&0x03E0)|
				((cb>>3)&0x001F);
		}else
		{
			v1=	((ca<<7)&0x7000)|
				((cg<<4)&0x0F00)|
				((cr   )&0x00F0)|
				((cb>>4)&0x000F);
		}

		tpx1=(tpx&0xFFF5)|((tpy&0x0005)<<1);
		tpy1=(tpy&0xFFFA)|((tpx&0x000A)>>1);
//		pbuf[tpy*xs+tpx]=v1;
		pbuf[tpy1*xs+tpx1]=v1;

//		pbuf[i]=v1;
	}
}

BTEIFGL_API void BGBDT_SWR_BindTexture(
	BGBDT_SwContext *ctx, int id)
{
	if(id<=0)
	{
		ctx->cur_tex=NULL;
		return;
	}

	ctx->cur_tex=BGBDT_SWR_GetTexture(ctx, id);
}

BTEIFGL_API int BGBDT_SWR_AllocTextureId(
	BGBDT_SwContext *ctx)
{
	int i;
	
	ctx->texidmap[0]|=1;
	
	i=ctx->texidrov;
	while(ctx->texidmap[i>>5]&(1<<(i&31)))
		i=(i+1)&32767;
	ctx->texidmap[i>>5]|=(1<<(i&31));
	ctx->texidrov=(i+1)&32767;
	return(i);
}

BTEIFGL_API void BGBDT_SWR_ProjectVertex(
	BGBDT_SwContext *ctx, float *ivec, float *ovec)
{
	float tv[4];
	float *mat;
	float sc;

	mat=ctx->xform;
	tv[0]=ivec[0]*mat[0]+ivec[1]*mat[4]+ivec[2]*mat[ 8]+mat[12];
	tv[1]=ivec[0]*mat[1]+ivec[1]*mat[5]+ivec[2]*mat[ 9]+mat[13];
	tv[2]=ivec[0]*mat[2]+ivec[1]*mat[6]+ivec[2]*mat[10]+mat[14];
	tv[3]=ivec[0]*mat[3]+ivec[1]*mat[7]+ivec[2]*mat[11]+mat[15];
	
	if(tv[3]>0.1)
		{ sc=1.0F/tv[3]; }
	else if(tv[3]<(-0.1))
		{ sc=1.0F/(-tv[3]); }
	else
		{ sc=1.0F; }

	ovec[0]=tv[0]*sc;
	ovec[1]=tv[1]*sc;
	ovec[2]=tv[2]*sc;
	ovec[3]=tv[3];
}

u16 bgbdt_swr_f32tohf16(float f)
{
	u32 v;
	u16 hv;
	int sg, ex, fr;
	
	v=*(u32 *)(&f);
	sg=(v>>31)&1;
	ex=(v>>23)&255;
	fr=(v>>13)&1023;
	ex=ex-112;
	
	if(ex<0)return(0x0000);
	if(ex>31)return(0xFFFF);
	hv=(sg<<15)|(ex<<10)|fr;
	return(hv);
}

float bgbdt_swr_hf16tof32(u16 fv)
{
	u32 v;
	float f;
	int sg, ex, fr;

	sg=(fv>>15)&1;
	ex=(fv>>10)&31;
	fr=fv&1023;
	
	ex=ex+112;
	fr=fr<<13;
	v=(sg<<31)|(ex<<23)|fr;
	f=*(float *)(&v);
	return(f);
}

float bgbdt_swr_min3f(float f0, float f1, float f2)
{
	float f;
	f=f0;
	if(f1<f)f=f1;
	if(f2<f)f=f2;
	return(f);
}

float bgbdt_swr_max3f(float f0, float f1, float f2)
{
	float f;
	f=f0;
	if(f1>f)f=f1;
	if(f2>f)f=f2;
	return(f);
}

BTEIFGL_API void BGBDT_SWR_EmitDrawTriangle(
	BGBDT_SwContext *ctx,
	float *xyz0, float *st0, byte *rgb0,
	float *xyz1, float *st1, byte *rgb1,
	float *xyz2, float *st2, byte *rgb2)
{
	float txv0[4], txv1[4], txv2[4];
	float txy0[4], txy1[4], txy2[4];
	float tst0[2], tst1[2], tst2[2];
	byte trgb0[4], trgb1[4], trgb2[4];
	float minv[3], maxv[3];
	float xsc, ysc, xbi, ybi;
	float dv0[2], dv1[2];
	float f, g;
	BGBDT_SwTriangle *tri;
	
	BGBDT_SWR_ProjectVertex(ctx, xyz0, txv0);
	BGBDT_SWR_ProjectVertex(ctx, xyz1, txv1);
	BGBDT_SWR_ProjectVertex(ctx, xyz2, txv2);
	
	minv[0]=bgbdt_swr_min3f(txv0[0], txv1[0], txv2[0]);
	minv[1]=bgbdt_swr_min3f(txv0[1], txv1[1], txv2[1]);
	minv[2]=bgbdt_swr_min3f(txv0[2], txv1[2], txv2[2]);
	maxv[0]=bgbdt_swr_max3f(txv0[0], txv1[0], txv2[0]);
	maxv[1]=bgbdt_swr_max3f(txv0[1], txv1[1], txv2[1]);
	maxv[2]=bgbdt_swr_max3f(txv0[2], txv1[2], txv2[2]);
	
	if(maxv[2]<0)
		return;
	if((minv[0]>=1.0) || (minv[1]>=1.0))
		return;
	if((maxv[0]<=-1.0) || (maxv[1]<=-1.0))
		return;
	
	if(ctx->fcull)
	{
		dv0[0]=txv1[0]-txv0[0];	dv0[1]=txv1[1]-txv0[1];
		dv1[0]=txv2[0]-txv0[0];	dv1[1]=txv2[1]-txv0[1];
		f=dv0[0]*dv1[1]-dv0[1]*dv1[0];
		if(f>=0)return;
	}

	xsc=(ctx->viewport_xs/2); xbi=xsc+ctx->viewport_x+0.5;
	ysc=(ctx->viewport_ys/2); ybi=ysc+ctx->viewport_y+0.5;

	f=(maxv[0]-minv[0])*xsc;
	g=(maxv[1]-minv[1])*ysc;
	if((f>128) || (g>128))
	{
		V3F_SCALEADDSCALE(xyz0, 0.5, xyz1, 0.5, txy0);
		V3F_SCALEADDSCALE(xyz1, 0.5, xyz2, 0.5, txy1);
		V3F_SCALEADDSCALE(xyz2, 0.5, xyz0, 0.5, txy2);
		V2F_SCALEADDSCALE(st0, 0.5, st1, 0.5, tst0);
		V2F_SCALEADDSCALE(st1, 0.5, st2, 0.5, tst1);
		V2F_SCALEADDSCALE(st2, 0.5, st0, 0.5, tst2);
		V4F_SCALEADDSCALE(rgb0, 0.5, rgb1, 0.5, trgb0);
		V4F_SCALEADDSCALE(rgb1, 0.5, rgb2, 0.5, trgb1);
		V4F_SCALEADDSCALE(rgb2, 0.5, rgb0, 0.5, trgb2);
		
		BGBDT_SWR_EmitDrawTriangle(ctx,
			xyz0, st0, rgb0,
			txy0, tst0, trgb0,
			txy2, tst2, trgb2);
		BGBDT_SWR_EmitDrawTriangle(ctx,		//ok
			txy0, tst0, trgb0,
			xyz1, st1, rgb1,
			txy1, tst1, trgb1);

		BGBDT_SWR_EmitDrawTriangle(ctx,
			txy1, tst1, trgb1,
			xyz2, st2, rgb2,
			txy2, tst2, trgb2);
		BGBDT_SWR_EmitDrawTriangle(ctx,
			txy0, tst0, trgb0,
			txy1, tst1, trgb1,
			txy2, tst2, trgb2);
		return;
	}
	
//	if((f<3) && (g<3))
//	{
//		return;
//	}

	
	tri=BGBDT_SWR_AllocTriangle(ctx);
	tri->next=ctx->trilst;
	ctx->trilst=tri;
	
	tri->tex=ctx->cur_tex;

//	xsc=(ctx->xs/2); xbi=xsc;
//	ysc=(ctx->ys/2); ybi=ysc;
	
	tri->v0.x=txv0[0]*xsc+xbi;
	tri->v0.y=txv0[1]*ysc+ybi;
//	tri->v0.z=bgbdt_swr_f32tohf16(txv0[2]);
	tri->v0.z=txv0[2]*32000;
	tri->v0.s=st0[0]*65536;
	tri->v0.t=st0[1]*65536;
	tri->v0.clr=*(u32 *)rgb0;

	tri->v1.x=txv1[0]*xsc+xbi;
	tri->v1.y=txv1[1]*ysc+ybi;
//	tri->v1.z=bgbdt_swr_f32tohf16(txv1[2]);
	tri->v1.z=txv1[2]*32000;
	tri->v1.s=st1[0]*65536;
	tri->v1.t=st1[1]*65536;
	tri->v1.clr=*(u32 *)rgb1;

	tri->v2.x=txv2[0]*xsc+xbi;
	tri->v2.y=txv2[1]*ysc+ybi;
//	tri->v2.z=bgbdt_swr_f32tohf16(txv2[2]);
	tri->v2.z=txv2[2]*32000;
	tri->v2.s=st2[0]*65536;
	tri->v2.t=st2[1]*65536;
	tri->v2.clr=*(u32 *)rgb2;
}

BTEIFGL_API void BGBDT_SWR_EmitDrawTriArrayFixS16HF(
	BGBDT_SwContext *ctx,
	int base, int num,
	s16 *xyza, u16 *sta, byte *rgba)
{
	BGBDT_SwTriangle *tris;
	s16 *xyz;
	u16 *st;
	byte *rgb;
	float v0[3], v1[3], v2[3];
	float stv[6];
	int i;
	
//	return;
	
	ctx->trilst=NULL;
	
	xyz=xyza+(base*9);
	st=sta+(base*6);
	rgb=rgba+(base*12);
	for(i=0; i<num; i++)
	{
		v0[0]=xyz[0]*1.0;	v0[1]=xyz[1]*1.0;	v0[2]=xyz[2]*1.0;
		v1[0]=xyz[3]*1.0;	v1[1]=xyz[4]*1.0;	v1[2]=xyz[5]*1.0;
		v2[0]=xyz[6]*1.0;	v2[1]=xyz[7]*1.0;	v2[2]=xyz[8]*1.0;
		stv[0]=bgbdt_swr_hf16tof32(st[0]);
		stv[1]=bgbdt_swr_hf16tof32(st[1]);
		stv[2]=bgbdt_swr_hf16tof32(st[2]);
		stv[3]=bgbdt_swr_hf16tof32(st[3]);
		stv[4]=bgbdt_swr_hf16tof32(st[4]);
		stv[5]=bgbdt_swr_hf16tof32(st[5]);
		
		BGBDT_SWR_EmitDrawTriangle(ctx,
			v0, stv+0, rgb+0,
			v1, stv+2, rgb+4,
			v2, stv+4, rgb+8);
		
		xyz+=9;
		st+=6;
		rgb+=12;
	}
	
	tris=ctx->trilst;
	ctx->trilst=NULL;
	
	BGBDT_SWR_DrawTriangleList(ctx, tris);
	BGBDT_SWR_FreeTriangleList(ctx, tris);
}

BTEIFGL_API void BGBDT_SWR_EmitDrawTriArrayFixF32(
	BGBDT_SwContext *ctx,
	int base, int num,
	float *xyza, float *sta, void *rgba, int rgbty)
{
	BGBDT_SwTriangle *tris;
	float *xyz;
	float *st;
	byte *rgb;
	float *rgbf;
	float v0[3], v1[3], v2[3];
	float stv[6];
	byte trgb[16];
	int i, j;
	
	ctx->trilst=NULL;
	
	xyz=xyza+(base*9);
	st=sta+(base*6);
	rgb=((byte *)rgba)+(base*12);
	rgbf=((float *)rgba)+(base*12);
	for(i=0; i<num; i++)
	{
		if(rgbty&2)
		{
			v0[0]=xyz[0];	v0[1]=xyz[1];	v0[2]=xyz[2];
			v1[0]=xyz[4];	v1[1]=xyz[5];	v1[2]=xyz[6];
			v2[0]=xyz[8];	v2[1]=xyz[9];	v2[2]=xyz[10];
		}else
		{
			v0[0]=xyz[0];	v0[1]=xyz[1];	v0[2]=xyz[2];
			v1[0]=xyz[3];	v1[1]=xyz[4];	v1[2]=xyz[5];
			v2[0]=xyz[6];	v2[1]=xyz[7];	v2[2]=xyz[8];
		}

		stv[0]=(st[0]);		stv[1]=(st[1]);
		stv[2]=(st[2]);		stv[3]=(st[3]);
		stv[4]=(st[4]);		stv[5]=(st[5]);
		
		if(rgbty&2)
		{
			for(j=0; j<12; j++)
				trgb[j]=clamp255(rgbf[j]*255.0);
		}else
		{
			memcpy(trgb, rgb, 12);
		}
		
		BGBDT_SWR_EmitDrawTriangle(ctx,
			v0, stv+0, trgb+0,
			v1, stv+2, trgb+4,
			v2, stv+4, trgb+8);
		
		if(rgbty)
			xyz+=12;
		else
			xyz+=9;
		st+=6;
		rgb+=12;
		rgbf+=12;
	}
	
	tris=ctx->trilst;
	ctx->trilst=NULL;
	
	BGBDT_SWR_DrawTriangleList(ctx, tris);
	BGBDT_SWR_FreeTriangleList(ctx, tris);
}

BTEIFGL_API void BGBDT_SWR_EmitDrawQuadArrayFixF32(
	BGBDT_SwContext *ctx,
	int base, int num,
	float *xyza, float *sta, void *rgba, int rgbty)
{
	BGBDT_SwTriangle *tris;
	float *xyz;
	float *st;
	float *rgbf;
	byte *rgb;
	float v0[3], v1[3], v2[3], v3[3];
	float stv[8];
	byte trgb[16];
	int i, j;
	
	ctx->trilst=NULL;
	
	xyz=xyza+(base*12);
	st=sta+(base*8);
	rgb=((byte *)rgba)+(base*16);
	rgbf=((float *)rgba)+(base*16);
	for(i=0; i<num; i++)
	{
		if(rgbty&2)
		{
			v0[0]=xyz[0];	v0[1]=xyz[ 1];	v0[2]=xyz[ 2];
			v1[0]=xyz[4];	v1[1]=xyz[ 5];	v1[2]=xyz[ 6];
			v2[0]=xyz[8];	v2[1]=xyz[ 9];	v2[2]=xyz[10];
			v3[0]=xyz[12];	v3[1]=xyz[13];	v3[2]=xyz[14];
		}else
		{
			v0[0]=xyz[0];	v0[1]=xyz[ 1];	v0[2]=xyz[ 2];
			v1[0]=xyz[3];	v1[1]=xyz[ 4];	v1[2]=xyz[ 5];
			v2[0]=xyz[6];	v2[1]=xyz[ 7];	v2[2]=xyz[ 8];
			v3[0]=xyz[9];	v3[1]=xyz[10];	v3[2]=xyz[11];
		}
		stv[0]=(st[0]);		stv[1]=(st[1]);
		stv[2]=(st[2]);		stv[3]=(st[3]);
		stv[4]=(st[4]);		stv[5]=(st[5]);
		stv[6]=(st[6]);		stv[7]=(st[7]);
		
		if(rgbty)
		{
			for(j=0; j<16; j++)
				trgb[j]=clamp255(rgbf[j]*255.0);
		}else
		{
			memcpy(trgb, rgb, 16);
		}
		
		BGBDT_SWR_EmitDrawTriangle(ctx,
			v0, stv+0, trgb+0,
			v1, stv+2, trgb+4,
			v2, stv+4, trgb+8);

		BGBDT_SWR_EmitDrawTriangle(ctx,
			v0, stv+0, trgb+0,
			v2, stv+4, trgb+8,
			v3, stv+6, trgb+11);
		
		if(rgbty&2)
			xyz+=16;
		else
			xyz+=12;
		st+=8;
		rgb+=16;
		rgbf+=16;
	}
	
	tris=ctx->trilst;
	ctx->trilst=NULL;
	
	BGBDT_SWR_DrawTriangleList(ctx, tris);
	BGBDT_SWR_FreeTriangleList(ctx, tris);
}
