#include <bgbrasw.h>

void BGBRASW_DrawSpanFlatBasic(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel clr)
{
	bgbrasw_pixel *ct, *cte;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;

	if(xo<0)
	{
		npix+=xo; xo=0;
		if(npix<=0)
			return;
	}
	if((xo+npix)>tabs->ctx->tgt_xs)
	{
		npix=tabs->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	tabs->ctx->query_cur+=npix;

//	ct=span; cte=span+npix;
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;

	while((ct+16)<=cte)
	{
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
	}
	if((ct+8)<=cte)
	{
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
		*ct++=clr;	*ct++=clr;	*ct++=clr;	*ct++=clr;
	}
	if((ct+4)<=cte)
		{ *ct++=clr; *ct++=clr; *ct++=clr; *ct++=clr; }

	while(ct<cte)
		{ *ct++=clr; }
}

void BGBRASW_DrawSpanFlatInterp(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1)
{
	bgbrasw_pixel *ct, *cte;
	int cr, crv;
	int cg, cgv;
	int cb, cbv;
	int ca, cav;
	int crag, crag2, cragv;
	int crrb, crrb2, crrbv;
	int clr, npix1;

#if 1
	if(clr0==clr1)
	{
		BGBRASW_DrawSpanFlatBasic(tabs, span, npix, xo, yo, clr0);
		return;
	}
#endif

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;

	if(npix<=0)
		return;

#if 0
	if(npix>1024)
	{
		npix1=npix>>1;
		clr=((clr0>>1)&0x7F7F7F7F)+((clr1>>1)&0x7F7F7F7F);
		BGBRASW_DrawSpanFlatInterp(span, npix1, clr0, clr);
		BGBRASW_DrawSpanFlatInterp(span+npix1, npix-npix1, clr, clr1);
	}
#endif

#if 1
	crag=clr0&0xFF00FF00;
	crrb=(clr0<<8)&0xFF00FF00;
//	crag2=clr1&0xFF00FF00;
//	crrb2=(clr1<<8)&0xFF00FF00;

//	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
//	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;

	crv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix;
	cgv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
	cbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
	cav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix;
	cragv=((cav<<16)&0xFFFF0000)|(cgv&0x00000FFFF);
	crrbv=((crv<<16)&0xFFFF0000)|(cbv&0x00000FFFF);

//	crv=((BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<24)/npix;
//	cgv=((BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
//	cbv=((BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
//	cav=((BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<24)/npix;
//	cragv=(cav&0xFFFF0000)|(cgv&0x00000FFFF);
//	crrbv=(crv&0xFFFF0000)|(cbv&0x00000FFFF);

	if(xo<0)
	{
		crag-=xo*cragv; crrb-=xo*crrbv;
		npix+=xo; xo=0;
		if(npix<=0)
			return;
	}
	if((xo+npix)>tabs->ctx->tgt_xs)
	{
		npix=tabs->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	tabs->ctx->query_cur+=npix;

//	ct=span; cte=span+npix;
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;

#if 1
	while((ct+8)<=cte)
	{
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
	}
#endif

//	while((ct+4)<=cte)
	if((ct+4)<=cte)
	{
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv; *ct++=clr;
//		crag=crag&0xFFF8FFF8;
//		crrb=crrb&0xFFF8FFF8;
	}

	while(ct<cte)
	{
		clr=(crag&0xFF00FF00)|((crrb>>8)&0x00FF00FF);
		crag+=cragv; crrb+=crrbv;
//		crag=(crag+cragv)&0xFFFEFFFE;
//		crrb=(crrb+crrbv)&0xFFFEFFFE;
		*ct++=clr;
	}
#endif

#if 0
//	npix1=npix+1;
	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix;

	ct=span; cte=span+npix;
	while(ct<cte)
	{
		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
//		clr=BGBRASW_MAKEPIXEL(
//			(cr>>16)&255, (cg>>16)&255,
//			(cb>>16)&255, (ca>>16)&255);
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		*ct++=clr;
	}
#endif
}

void BGBRASW_DrawSpanFlatBasicZTest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel clr, bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;

	if(npix<=0)
		return;

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
	if((xo+npix)>tabs->ctx->tgt_xs)
	{
		npix=tabs->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	ppix=0;
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

	while(ct<cte)
	{
//		tz2=tz&BGBRASW_MASK_DEPTH;
		tz2=tz;
		if(tz2>(*ctz))
		{
			//Z fail
			ct++; ctz++;
			tz+=tzv;
			continue;
		}
		//Z pass
		*ct++=clr;
		*ctz++=tz2;
//		*ctz++=tz2|(z0&BGBRASW_MASK_STENCIL);
		tz+=tzv;
		ppix++;
	}

	tabs->ctx->query_cur+=ppix;
}

void BGBRASW_DrawSpanFlatInterpZTest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv, tz2;
	int cr, crv;
	int cg, cgv;
	int cb, cbv;
	int ca, cav;
	int clr, npix1, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;

	if(npix<=0)
		return;

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
	if((xo+npix)>tabs->ctx->tgt_xs)
	{
		npix=tabs->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	ppix=0;
//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

	while(ct<cte)
	{
//		tz2=tz&BGBRASW_MASK_DEPTH;
		tz2=tz;
		if(tz2>(*ctz))
		{
			//Z fail
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ct++; ctz++;
			tz+=tzv;
			continue;
		}
		//Z pass
		clr=BGBRASW_MAKEPIXEL(cr>>16, cg>>16, cb>>16, ca>>16);
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		*ct++=clr;
		*ctz++=tz2;
//		*ctz++=tz2|(z0&BGBRASW_MASK_STENCIL);
		tz+=tzv;
		ppix++;
	}

	tabs->ctx->query_cur+=ppix;
}
