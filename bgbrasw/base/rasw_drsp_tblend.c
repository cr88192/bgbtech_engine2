#include <bgbrasw.h>

void BGBRASW_DrawSpanFlatBasicTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel clr, bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	BGBRASW_TestBlendFunc_ft testBlend;
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;

	if(npix<=0)
		return;

#ifdef BGBRASW_SS_YUVA
	clr=bgbrasw_pixel2yypixel(clr);
#endif

//	tz=z0; tzv=((s32)(z1-z0))/npix;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	if(xo<0)
	{
		tz-=xo*tzv;
		npix+=xo; xo=0;
		if(npix<=0)
			return;
	}
	if((xo+npix)>testData->ctx->tgt_xs)
	{
		npix=testData->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	testBlend=testData->testAndBlend;
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	ct=span+yo*testData->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*testData->ctx->tgt_xs+xo; ctze=ctz+npix;

	while(ct<cte)
	{
		tz2=tz;
		testBlend(testData, &clr, &tz2, ct, ctz);
		ct++; ctz++; tz+=tzv;
	}
}

void BGBRASW_DrawSpanFlatInterpTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1, bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	BGBRASW_TestBlendFunc_ft testBlend;
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int cr, crv;
	int cg, cgv;
	int cb, cbv;
	int ca, cav;
	int clr, npix1;

	if(npix<=0)
		return;

	if(clr0==clr1)
	{
		BGBRASW_DrawSpanFlatBasicTestBlend(
			testData, span, spanz, npix, xo, yo, clr0, z0, z1);
		return;
	}

	npix1=npix;
//	npix1=npix+1;
	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix1;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix1;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix1;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix1;
//	tz=z0; tzv=((s32)(z1-z0))/npix1;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	if(xo<0)
	{
		cr-=xo*crv;	cg-=xo*cgv;
		cb-=xo*cbv;	ca-=xo*cav;
		tz-=xo*tzv;
		npix+=xo; xo=0;
		if(npix<=0)
			return;
	}
	if((xo+npix)>testData->ctx->tgt_xs)
	{
		npix=testData->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	testBlend=testData->testAndBlend;
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	ct=span+yo*testData->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*testData->ctx->tgt_xs+xo; ctze=ctz+npix;

	while(ct<cte)
	{
		tz2=tz;
		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
#ifdef BGBRASW_SS_YUVA
		clr=bgbrasw_pixel2yypixel(clr);
#endif
		testBlend(testData, &clr, &tz2, ct, ctz);
		ct++;		ctz++;
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		tz+=tzv;
	}
}

void BGBRASW_DrawSpanTextureBasicTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	BGBRASW_TestBlendFunc_ft testBlend;
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int ts, tt, tsv, ttv, tx, ty;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1;

	if(npix<=0)return;

//	npix1=npix+1;
	npix1=npix;
//	tz=z0; tzv=((s32)(z1-z0))/npix1;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;
	tsv=(st1s-st0s)/npix;
	ttv=(st1t-st0t)/npix;
//	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
//	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

	if(xo<0)
	{
		ts-=xo*tsv;	tt-=xo*ttv;
		tz-=xo*tzv;
		npix+=xo; xo=0;
		if(npix<=0)
			return;
	}
	if((xo+npix)>testData->ctx->tgt_xs)
	{
		npix=testData->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	testBlend=testData->testAndBlend;
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	ct=span+yo*testData->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*testData->ctx->tgt_xs+xo; ctze=ctz+npix;

	while(ct<cte)
	{
		tz2=tz;
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
		clr=tex[ty*txs+tx];

		testBlend(testData, &clr, &tz2, ct, ctz);
		ct++; ctz++;
		ts+=tsv;	tt+=ttv;
		tz+=tzv;
	}
}

void BGBRASW_DrawSpanTextureInterpTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	BGBRASW_TestBlendFunc_ft testBlend;
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int ts, tt, tsv, ttv, tx, ty;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1, npfrac;

	if(npix<=0)return;

	if((clr0==clr1) && (clr0==0xFFFFFFFF))
	{
		BGBRASW_DrawSpanTextureBasicTestBlend(
			testData, span, spanz, npix, xo, yo,
			tex, txs, tys,
			st0s, st0t, st1s, st1t,
			z0, z1);
		return;
	}

//	npix1=npix+1;
	npix1=npix;
#if 0
	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix1;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix1;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix1;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix1;
#endif
#if 0
	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix1;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix1;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix1;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix1;
#endif
#if 1
	npfrac=bgbrasw_fixdiv_s32_u16(65536, npix);
	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
	crv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac)>>8;
	cgv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac)>>8;
	cbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac)>>8;
	cav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac)>>8;
#endif
//	tz=z0; tzv=((s32)(z1-z0))/npix1;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;
	tsv=(st1s-st0s)/npix;
	ttv=(st1t-st0t)/npix;
//	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
//	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

//	ts=st0s<<8; tt=st0t<<8;
//	tsv=((st1s-st0s)<<8)/npix1;
//	ttv=((st1t-st0t)<<8)/npix1;

	if(xo<0)
	{
		cr-=xo*crv;	cg-=xo*cgv;
		cb-=xo*cbv;	ca-=xo*cav;
		ts-=xo*tsv;	tt-=xo*ttv;
		tz-=xo*tzv;
		npix+=xo; xo=0;
		if(npix<=0)
			return;
	}
	if((xo+npix)>testData->ctx->tgt_xs)
	{
		npix=testData->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	testBlend=testData->testAndBlend;
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	ct=span+yo*testData->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*testData->ctx->tgt_xs+xo; ctze=ctz+npix;

	while(ct<cte)
	{
//		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
//		tz2=(tz&BGBRASW_MASK_DEPTH)|(z0&BGBRASW_MASK_STENCIL);
		tz2=tz;

		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
		clrt=tex[ty*txs+tx];

#ifdef BGBRASW_SS_YUVA
		clr=BGBRASW_MAKEPIXEL(cr>>8, cg>>8, cb>>8, ca>>8);
		clr=bgbrasw_colormul_yymod(clrt, clr);
#else
//		crt=(BGBRASW_PIXEL_R(clrt)*(cr>>8));
//		cgt=(BGBRASW_PIXEL_G(clrt)*(cg>>8));
//		cbt=(BGBRASW_PIXEL_B(clrt)*(cb>>8));
//		cat=(BGBRASW_PIXEL_A(clrt)*(ca>>8));
		crt=(BGBRASW_PIXEL_R(clrt)*cr);
		cgt=(BGBRASW_PIXEL_G(clrt)*cg);
		cbt=(BGBRASW_PIXEL_B(clrt)*cb);
		cat=(BGBRASW_PIXEL_A(clrt)*ca);

//		clr=BGBRASW_MAKEPIXEL(crt>>8, cgt>>8, cbt>>8, cat>>8);
		clr=BGBRASW_MAKEPIXEL(crt>>16, cgt>>16, cbt>>16, cat>>16);
#endif

		testBlend(testData, &clr, &tz2, ct, ctz);
//		testData->testAndBlend(testData, &clr, &tz2, ct, ctz);
		ct++; ctz++;

		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv;	tt+=ttv;
		tz+=tzv;
	}
}

bgbrasw_pixel bgbrasw_interp_linear(
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_pixel clr2, bgbrasw_pixel clr3,
	int fx, int fy)
{
#ifdef HAVE_SSE2
//#if 0
	int ar[16];
	__m128i ca, cb, cc, cd, cm;
	bgbrasw_pixel clr;
	int nx, ny;

	nx=255-fx;	ny=255-fy;

	ca=_mm_set_epi32(clr2, clr2>>8, clr0, clr0>>8);
	cb=_mm_set_epi32(clr3, clr3>>8, clr1, clr1>>8);
	
	cm=_mm_set_epi32(0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF);
	ca=_mm_and_si128(ca, cm);	cb=_mm_and_si128(cb, cm);

	cc=_mm_set_epi16(nx, nx, nx, nx, nx, nx, nx, nx);
	cd=_mm_set_epi16(fx, fx, fx, fx, fx, fx, fx, fx);
	ca=_mm_mullo_epi16(ca, cc);	cb=_mm_mullo_epi16(cb, cd);
	cc=_mm_add_epi16(ca, cb);
	ca=_mm_srli_epi16(cc, 8);

#if 1
//	cb=_mm_shuffle_epi32(ca, _MM_SHUFFLE2(1, 1));
	cb=_mm_unpackhi_epi64(ca, ca);

	cc=_mm_set_epi16(ny, ny, ny, ny, ny, ny, ny, ny);
	cd=_mm_set_epi16(fy, fy, fy, fy, fy, fy, fy, fy);
	ca=_mm_mullo_epi16(ca, cc);	cb=_mm_mullo_epi16(cb, cd);
	cc=_mm_add_epi16(ca, cb);

	_mm_storeu_si128((__m128i *)(&ar), cc);
//	_mm_storeu_si128((__m128i *)(&ar), ca);
//	_mm_storeu_si128((__m128i *)(&ar), ca);
//	ar[0]=ar[0]+ar[2];
//	ar[1]=ar[1]+ar[3];
#else
	_mm_storeu_si128((__m128i *)(&ar), ca);
//	ar[0]=ar[0]<<8;
//	ar[1]=ar[1]<<8;
	ar[0]=ny*ar[0]+fy*ar[2];
	ar[1]=ny*ar[1]+fy*ar[3];
#endif

	clr=	(ar[0]&0xFF00FF00UL)|
			((ar[1]>>8)&0x00FF00FFUL);
  
	return(clr);
#else
	bgbrasw_pixel clr0ag, clr0rb;
	bgbrasw_pixel clr1ag, clr1rb;
	bgbrasw_pixel clr2ag, clr2rb;
	bgbrasw_pixel clr3ag, clr3rb;
	bgbrasw_pixel clr4ag, clr4rb;
	bgbrasw_pixel clr5ag, clr5rb;
	bgbrasw_pixel clr6ag, clr6rb;
	bgbrasw_pixel clr7ag, clr7rb;
	bgbrasw_pixel clr;
	int nx, ny;

	nx=255-fx;	ny=255-fy;
//	clr0ag=(clr0>>8)&0x00FF00FF;	clr0rb=(clr0   )&0x00FF00FF;
//	clr1ag=(clr1>>8)&0x00FF00FF;	clr1rb=(clr1   )&0x00FF00FF;
//	clr2ag=(clr2>>8)&0x00FF00FF;	clr2rb=(clr2   )&0x00FF00FF;
//	clr3ag=(clr3>>8)&0x00FF00FF;	clr3rb=(clr3   )&0x00FF00FF;
//	clr4ag=nx*clr0ag+fx*clr1ag;		clr4rb=nx*clr0rb+fx*clr1rb;
//	clr5ag=nx*clr2ag+fx*clr3ag;		clr5rb=nx*clr2rb+fx*clr3rb;
//	clr4ag=(clr4ag>>8)&0x00FF00FF;	clr4rb=(clr4rb>>8)&0x00FF00FF;
//	clr5ag=(clr5ag>>8)&0x00FF00FF;	clr5rb=(clr5rb>>8)&0x00FF00FF;
//	clr6ag=ny*clr4ag+fy*clr5ag;		clr6rb=ny*clr4rb+fy*clr5rb;

	clr4ag=nx*((clr0>>8)&0x00FF00FF)+fx*((clr1>>8)&0x00FF00FF);
	clr4rb=nx*((clr0   )&0x00FF00FF)+fx*((clr1   )&0x00FF00FF);
	clr5ag=nx*((clr2>>8)&0x00FF00FF)+fx*((clr3>>8)&0x00FF00FF);
	clr5rb=nx*((clr2   )&0x00FF00FF)+fx*((clr3   )&0x00FF00FF);
	clr6ag=ny*((clr4ag>>8)&0x00FF00FF)+fy*((clr5ag>>8)&0x00FF00FF);
	clr6rb=ny*((clr4rb>>8)&0x00FF00FF)+fy*((clr5rb>>8)&0x00FF00FF);

//	clr=(clr6ag&0xFF00FF00UL)|(clr6rb&0x00FF00FFUL);
	clr=(clr6ag&0xFF00FF00UL)|((clr6rb>>8)&0x00FF00FFUL);

	return(clr);
#endif
}

void BGBRASW_DrawSpanTextureLinearInterpTestBlend(
	BGBRASW_TestBlendData *testData,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	bgbrasw_pixel tclr0, tclr1, tclr2, tclr3;
	u32 tz, tzv, tz2;
	int ts, tt, tsv, ttv;
	int tx, ty;
	int tx0, ty0, tx1, ty1;
	int tx2, ty2, tx3, ty3;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1, npfrac;

	if(npix<=0)return;

//	npix1=npix+1;
	npix1=npix;
	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
//	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix;
//	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
//	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
//	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix;

	npfrac=bgbrasw_fixdiv_s32_u16(256, npix);
	crv=(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac;
	cgv=(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac;
	cbv=(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac;
	cav=(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac;

//	tz=z0; tzv=((s32)(z1-z0))/npix;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;
//	tsv=(st1s-st0s)/npix;
//	ttv=(st1t-st0t)/npix;
	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

//	ts=st0s<<8; tt=st0t<<8;
//	tsv=((st1s-st0s)<<8)/npix1;
//	ttv=((st1t-st0t)<<8)/npix1;

	if(xo<0)
	{
		cr-=xo*crv;	cg-=xo*cgv;
		cb-=xo*cbv;	ca-=xo*cav;
		ts-=xo*tsv;	tt-=xo*ttv;
		tz-=xo*tzv;
		npix+=xo; xo=0;
		if(npix<=0)
			return;
	}
	if((xo+npix)>testData->ctx->tgt_xs)
	{
		npix=testData->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	ct=span+yo*testData->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*testData->ctx->tgt_xs+xo; ctze=ctz+npix;

	while(ct<cte)
	{
//		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
//		tz2=(tz&BGBRASW_MASK_DEPTH)|(z0&BGBRASW_MASK_STENCIL);
		tz2=tz;

		tx=ts>>8;	ty=tt>>8;
		tx0=(tx  )&(txs-1);	ty0=(ty  )&(tys-1);
		tx1=(tx+1)&(txs-1);	ty1=(ty  )&(tys-1);
		tx2=(tx  )&(txs-1);	ty2=(ty+1)&(tys-1);
		tx3=(tx+1)&(txs-1);	ty3=(ty+1)&(tys-1);
		tclr0=tex[ty0*txs+tx0];	tclr1=tex[ty1*txs+tx1];
		tclr2=tex[ty2*txs+tx2];	tclr3=tex[ty3*txs+tx3];
//		clrt=tclr0;
		clrt=bgbrasw_interp_linear(
			tclr0, tclr1, tclr2, tclr3,
			ts&255, tt&255);
		
//		tx=(ts>>8)&(txs-1);
//		ty=(tt>>8)&(tys-1);
//		tx=((ts+128)>>8)&(txs-1);
//		ty=((tt+128)>>8)&(tys-1);
//		clrt=tex[ty*txs+tx];
//		crt=(BGBRASW_PIXEL_R(clrt)*(cr>>8));
//		cgt=(BGBRASW_PIXEL_G(clrt)*(cg>>8));
//		cbt=(BGBRASW_PIXEL_B(clrt)*(cb>>8));
//		cat=(BGBRASW_PIXEL_A(clrt)*(ca>>8));
		crt=(BGBRASW_PIXEL_R(clrt)*cr);
		cgt=(BGBRASW_PIXEL_G(clrt)*cg);
		cbt=(BGBRASW_PIXEL_B(clrt)*cb);
		cat=(BGBRASW_PIXEL_A(clrt)*ca);

		clr=BGBRASW_MAKEPIXEL(crt>>16, cgt>>16, cbt>>16, cat>>16);

		testData->testAndBlend(testData, &clr, &tz2, ct, ctz);
		ct++; ctz++;

		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv;	tt+=ttv;
		tz+=tzv;
	}
}
