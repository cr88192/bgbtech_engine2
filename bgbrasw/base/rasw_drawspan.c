#include <bgbrasw.h>


void BGBRASW_DrawSpanTextureBasic(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t)
{
	bgbrasw_pixel *ct, *cte;
	int ts, tt, tsv, ttv, tx, ty;
	int npix1;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	ts=st0s; tt=st0t;
//	tsv=(st1s-st0s)/npix;
//	ttv=(st1t-st0t)/npix;
	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

	if(xo<0)
	{
		ts-=xo*tsv;	tt-=xo*ttv;
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

	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	while(ct<cte)
	{
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
		*ct++=tex[ty*txs+tx];
		ts+=tsv; tt+=ttv;
	}
}

void BGBRASW_DrawSpanTextureInterp(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	int ts, tt, tsv, ttv, tx, ty;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

//	npix1=npix+1;
//	npix1=npix;
//	cr=BGBRASW_PIXEL_R(clr0)<<16;	cg=BGBRASW_PIXEL_G(clr0)<<16;
//	cb=BGBRASW_PIXEL_B(clr0)<<16;	ca=BGBRASW_PIXEL_A(clr0)<<16;
//	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<16)/npix1;
//	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<16)/npix1;
//	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<16)/npix1;
//	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<16)/npix1;

	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;
	crv=((int)(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))<<8)/npix;
	cgv=((int)(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))<<8)/npix;
	cbv=((int)(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))<<8)/npix;
	cav=((int)(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))<<8)/npix;

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

	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
//	ct=span; cte=span+npix;
	while(ct<cte)
	{
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
		clrt=tex[ty*txs+tx];

		crt=(BGBRASW_PIXEL_R(clrt)*cr);
		cgt=(BGBRASW_PIXEL_G(clrt)*cg);
		cbt=(BGBRASW_PIXEL_B(clrt)*cb);
		cat=(BGBRASW_PIXEL_A(clrt)*ca);
		clr=BGBRASW_MAKEPIXEL(crt>>16, cgt>>16, cbt>>16, cat>>16);

		//Z pass
		*ct++=clr;
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv;	tt+=ttv;
	}
}

void BGBRASW_DrawSpanTextureBasicATest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t)
{
	bgbrasw_pixel *ct, *cte;
	int ts, tt, tsv, ttv, tx, ty;
	int clr;
	int npix1, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	ts=st0s; tt=st0t;
	tsv=(st1s-st0s)/npix;
	ttv=(st1t-st0t)/npix;

	if(xo<0)
	{
		ts-=xo*tsv;	tt-=xo*ttv;
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
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	while(ct<cte)
	{
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
		clr=tex[ty*txs+tx];
		if(clr&0x80000000)
			{ *ct=clr; ppix++; }
		ct++;
		ts+=tsv; tt+=ttv;
	}
	tabs->ctx->query_cur+=ppix;
}

void BGBRASW_DrawSpanTextureInterpATest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	int ts, tt, tsv, ttv, tx, ty;
	int cr, crv, crt;
	int cg, cgv, cgt;
	int cb, cbv, cbt;
	int ca, cav, cat;
	int clr, clrt, npix1, ppix, npfrac;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

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

	ts=st0s; tt=st0t;
	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);

	if(xo<0)
	{
		cr-=xo*crv;	cg-=xo*cgv;
		cb-=xo*cbv;	ca-=xo*cav;
		ts-=xo*tsv;	tt-=xo*ttv;
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
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	while(ct<cte)
	{
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
		clrt=tex[ty*txs+tx];

		if(clrt&0x80000000)
		{
			crt=(BGBRASW_PIXEL_R(clrt)*cr);
			cgt=(BGBRASW_PIXEL_G(clrt)*cg);
			cbt=(BGBRASW_PIXEL_B(clrt)*cb);
			cat=(BGBRASW_PIXEL_A(clrt)*ca);
			clr=BGBRASW_MAKEPIXEL(crt>>16, cgt>>16, cbt>>16, cat>>16);
		}else
		{
			clr=0;
		}

		//Z pass
		if(clr&0x80000000)
			{ *ct=clr; ppix++; }
		ct++;
		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv;	tt+=ttv;
	}
	tabs->ctx->query_cur+=ppix;
}


void BGBRASW_DrawSpanTextureBasicZTest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int txsn1, tysn1;
	u32 tz, tzv, tz2;
	int npix1, npfrac, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	ts=st0s; tt=st0t;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1;	tysn1=tys-1;

	if(xo<0)
	{
		ts-=xo*tsv;	tt-=xo*ttv;
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

#if 1
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

	if(tz>(*ctz))
	{
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			//Z fail
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{
		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:
			//Z pass
			tx=(ts>>8)&txsn1;
			ty=(tt>>8)&tysn1;
			*ct++=tex[ty*txs+tx];
			*ctz++=tz;
			ts+=tsv; tt+=ttv; tz+=tzv;
			ppix++;
		}
	}
#endif

#if 0
	ct=span; cte=span+npix;
	ctz=spanz; ctze=spanz+npix;
	while(ct<cte)
	{
		if(tz>(*ctz))
		{
			//Z fail
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
		//Z pass
		tx=(ts>>8)&(txs-1);
		ty=(tt>>8)&(tys-1);
		*ct++=tex[ty*txs+tx];
//		*ctz++=tz2|(z0&BGBRASW_MASK_STENCIL);
		*ctz++=tz;
		ts+=tsv; tt+=ttv; tz+=tzv;
	}
#endif

	tabs->ctx->query_cur+=ppix;
}

void BGBRASW_DrawSpanTextureInterpZTest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv;
	u32 tz0, tz1, tz2, tz3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int cr, crv, crt, cg, cgv, cgt;
	int cb, cbv, cbt, ca, cav, cat;
	int txsn1, tysn1;
	int clr, clrt, npix1, npfrac, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;

	npfrac=bgbrasw_fixdiv_s32_u16(256, npix);
	crv=(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac;
	cgv=(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac;
	cbv=(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac;
	cav=(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac;

	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1; tysn1=tys-1;

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
	if((xo+npix)>tabs->ctx->tgt_xs)
	{
		npix=tabs->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	ppix=0;

	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

#if 1
	if(tz>(*ctz))
	{
		//Z fail
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{
		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:
			tx=(ts>>8)&txsn1;
			ty=(tt>>8)&tysn1;
			clrt=tex[ty*txs+tx];

			cat=(((clrt>>16)&0xFF00)*ca)     &0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;

			//Z pass
			*ct++=clr;
			*ctz++=tz;
			ppix++;

			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}
#endif

#if 0
	while(ct<cte)
	{
		if(tz>(*ctz))
		{
			//Z fail
			while(ct<cte)
			{
				if(tz<=(*ctz))
					goto loopPass;
				loopFail:
				cr+=crv;	cg+=cgv;
				cb+=cbv;	ca+=cav;
				ct++; ctz++;
				ts+=tsv; tt+=ttv; tz+=tzv;
				continue;
			}
		}

		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:
			tx=(ts>>8)&(txs-1);
			ty=(tt>>8)&(tys-1);
			clrt=tex[ty*txs+tx];

			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
//			cat=0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;

			//Z pass
			*ct++=clr;
			*ctz++=tz;

			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}
#endif

	tabs->ctx->query_cur+=ppix;
}


void BGBRASW_DrawSpanTextureBasicLinear(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t)
{
	bgbrasw_pixel *ct, *cte;
//	bgbrasw_zbuf *ctz, *ctze;
	bgbrasw_pixel tclr0, tclr1, tclr2, tclr3;
	bgbrasw_pixel clrt;
	int tx0, ty0, tx1, ty1;
	int tx2, ty2, tx3, ty3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int txsn1, tysn1;
//	u32 tz, tzv, tz2;
	int npix1, npfrac;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;
	ts=st0s; tt=st0t;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1;	tysn1=tys-1;

	if(xo<0)
	{
		ts-=xo*tsv;	tt-=xo*ttv;
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

	while(ct<cte)
	{
		tx=ts>>8;	ty=tt>>8;
		tx0=(tx  )&(txs-1);	ty0=(ty  )&(tys-1);
		tx1=(tx+1)&(txs-1);	ty1=(ty  )&(tys-1);
		tx2=(tx  )&(txs-1);	ty2=(ty+1)&(tys-1);
		tx3=(tx+1)&(txs-1);	ty3=(ty+1)&(tys-1);
		tclr0=tex[ty0*txs+tx0];	tclr1=tex[ty1*txs+tx1];
		tclr2=tex[ty2*txs+tx2];	tclr3=tex[ty3*txs+tx3];
		clrt=bgbrasw_interp_linear(
			tclr0, tclr1, tclr2, tclr3,
			ts&255, tt&255);

		*ct++=clrt;
		ts+=tsv; tt+=ttv;
	}
}

void BGBRASW_DrawSpanTextureInterpLinear(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_pixel tclr0, tclr1, tclr2, tclr3;
	int tx0, ty0, tx1, ty1;
	int tx2, ty2, tx3, ty3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int cr, crv, crt, cg, cgv, cgt;
	int cb, cbv, cbt, ca, cav, cat;
	int txsn1, tysn1;
	int clr, clrt, npix1, npfrac;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;

	npfrac=bgbrasw_fixdiv_s32_u16(256, npix);
	crv=(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac;
	cgv=(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac;
	cbv=(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac;
	cav=(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac;

	ts=st0s; tt=st0t;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1; tysn1=tys-1;

	if(xo<0)
	{
		cr-=xo*crv;	cg-=xo*cgv;
		cb-=xo*cbv;	ca-=xo*cav;
		ts-=xo*tsv;	tt-=xo*ttv;
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

	while(ct<cte)
	{
		tx=ts>>8;	ty=tt>>8;
		tx0=(tx  )&(txs-1);	ty0=(ty  )&(tys-1);
		tx1=(tx+1)&(txs-1);	ty1=(ty  )&(tys-1);
		tx2=(tx  )&(txs-1);	ty2=(ty+1)&(tys-1);
		tx3=(tx+1)&(txs-1);	ty3=(ty+1)&(tys-1);
		tclr0=tex[ty0*txs+tx0];	tclr1=tex[ty1*txs+tx1];
		tclr2=tex[ty2*txs+tx2];	tclr3=tex[ty3*txs+tx3];
		clrt=bgbrasw_interp_linear(
			tclr0, tclr1, tclr2, tclr3,
			ts&255, tt&255);

		cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
		crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
		cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
		cbt=((((byte) clrt     )*cb)>>16);
		clr=cat|crt|cgt|cbt;

		*ct++=clr;

		cr+=crv;	cg+=cgv;
		cb+=cbv;	ca+=cav;
		ts+=tsv; tt+=ttv;
	}
}

void BGBRASW_DrawSpanTextureBasicZTestLinear(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	bgbrasw_pixel tclr0, tclr1, tclr2, tclr3;
	bgbrasw_pixel clrt;
	int tx0, ty0, tx1, ty1;
	int tx2, ty2, tx3, ty3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int txsn1, tysn1;
	u32 tz, tzv, tz2;
	int npix1, npfrac, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	ts=st0s; tt=st0t;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1;	tysn1=tys-1;

	if(xo<0)
	{
		ts-=xo*tsv;	tt-=xo*ttv;
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

//	ct=span; cte=span+npix;
//	ctz=spanz; ctze=spanz+npix;

	ppix=0;

	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

	if(tz>(*ctz))
	{
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			//Z fail
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{

		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:

			tx=ts>>8;	ty=tt>>8;
			tx0=(tx  )&(txs-1);	ty0=(ty  )&(tys-1);
			tx1=(tx+1)&(txs-1);	ty1=(ty  )&(tys-1);
			tx2=(tx  )&(txs-1);	ty2=(ty+1)&(tys-1);
			tx3=(tx+1)&(txs-1);	ty3=(ty+1)&(tys-1);
			tclr0=tex[ty0*txs+tx0];	tclr1=tex[ty1*txs+tx1];
			tclr2=tex[ty2*txs+tx2];	tclr3=tex[ty3*txs+tx3];
			clrt=bgbrasw_interp_linear(
				tclr0, tclr1, tclr2, tclr3,
				ts&255, tt&255);

			ppix++;
			*ct++=clrt; *ctz++=tz;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}

	tabs->ctx->query_cur+=ppix;
}

void BGBRASW_DrawSpanTextureInterpZTestLinear(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	bgbrasw_pixel tclr0, tclr1, tclr2, tclr3;
	u32 tz, tzv;
	u32 tz0, tz1, tz2, tz3;
	int tx0, ty0, tx1, ty1;
	int tx2, ty2, tx3, ty3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int cr, crv, crt, cg, cgv, cgt;
	int cb, cbv, cbt, ca, cav, cat;
	int txsn1, tysn1;
	int clr, clrt, npix1, npfrac, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;

	npfrac=bgbrasw_fixdiv_s32_u16(256, npix);
	crv=(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac;
	cgv=(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac;
	cbv=(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac;
	cav=(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac;

	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1; tysn1=tys-1;

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

	if(tz>(*ctz))
	{
		//Z fail
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{
		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:

			tx=ts>>8;	ty=tt>>8;
			tx0=(tx  )&(txs-1);	ty0=(ty  )&(tys-1);
			tx1=(tx+1)&(txs-1);	ty1=(ty  )&(tys-1);
			tx2=(tx  )&(txs-1);	ty2=(ty+1)&(tys-1);
			tx3=(tx+1)&(txs-1);	ty3=(ty+1)&(tys-1);
			tclr0=tex[ty0*txs+tx0];	tclr1=tex[ty1*txs+tx1];
			tclr2=tex[ty2*txs+tx2];	tclr3=tex[ty3*txs+tx3];
			clrt=bgbrasw_interp_linear(
				tclr0, tclr1, tclr2, tclr3,
				ts&255, tt&255);

			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;

			//Z pass
			*ct++=clr;
			*ctz++=tz;

			ppix++;
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}

	tabs->ctx->query_cur+=ppix;
}


void BGBRASW_DrawSpanTextureBasicZATest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int txsn1, tysn1;
	u32 tz, tzv, tz2, px;
	int npix1, npfrac, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	ts=st0s; tt=st0t;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1;	tysn1=tys-1;

	if(xo<0)
	{
		ts-=xo*tsv;	tt-=xo*ttv;
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
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

	if(tz>(*ctz))
	{
		//Z fail
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{
		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:

			//Z pass
			tx=(ts>>8)&(txs-1);
			ty=(tt>>8)&(tys-1);
			px=tex[ty*txs+tx];
			if(px&0x80000000)
				{ *ct=px; *ctz=tz; ppix++; }
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}

	tabs->ctx->query_cur+=ppix;
}

void BGBRASW_DrawSpanTextureInterpZATest(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	u32 tz, tzv;
	u32 tz0, tz1, tz2, tz3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int cr, crv, crt, cg, cgv, cgt;
	int cb, cbv, cbt, ca, cav, cat;
	int txsn1, tysn1;
	int clr, clrt, npix1, npfrac, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;

	npfrac=bgbrasw_fixdiv_s32_u16(256, npix);
	crv=(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac;
	cgv=(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac;
	cbv=(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac;
	cav=(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac;

	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1; tysn1=tys-1;

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
	if((xo+npix)>tabs->ctx->tgt_xs)
	{
		npix=tabs->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}

	ppix=0;
	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

	if(tz>(*ctz))
	{
		//Z fail
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{
		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:

			tx=(ts>>8)&(txs-1);
			ty=(tt>>8)&(tys-1);
			clrt=tex[ty*txs+tx];

			if(!(clrt&0x80000000U))
			{
				//A fail
				cr+=crv;	cg+=cgv;
				cb+=cbv;	ca+=cav;
				ct++; ctz++;
				ts+=tsv; tt+=ttv; tz+=tzv;
				continue;
			}
			
			cat= (((clrt>>16)&0xFF00)*ca)    &0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;

			//Z pass
			*ct++=clr;
			*ctz++=tz;

			ppix++;
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}

	tabs->ctx->query_cur+=ppix;
}


void BGBRASW_DrawSpanTextureBasicZATestLinear(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	bgbrasw_pixel tclr0, tclr1, tclr2, tclr3;
	bgbrasw_pixel clrt;
	int tx0, ty0, tx1, ty1;
	int tx2, ty2, tx3, ty3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int txsn1, tysn1;
	u32 tz, tzv, tz2;
	int npix1, npfrac, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	ts=st0s; tt=st0t;
	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1;	tysn1=tys-1;

	if(xo<0)
	{
		ts-=xo*tsv;	tt-=xo*ttv;
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

	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

	if(tz>(*ctz))
	{
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			//Z fail
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{

		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:

			tx=ts>>8;	ty=tt>>8;
			tx0=(tx  )&(txs-1);	ty0=(ty  )&(tys-1);
			tx1=(tx+1)&(txs-1);	ty1=(ty  )&(tys-1);
			tx2=(tx  )&(txs-1);	ty2=(ty+1)&(tys-1);
			tx3=(tx+1)&(txs-1);	ty3=(ty+1)&(tys-1);
			tclr0=tex[ty0*txs+tx0];	tclr1=tex[ty1*txs+tx1];
			tclr2=tex[ty2*txs+tx2];	tclr3=tex[ty3*txs+tx3];
			clrt=bgbrasw_interp_linear(
				tclr0, tclr1, tclr2, tclr3,
				ts&255, tt&255);

			if(clrt&0x80000000)
				{ *ct=clrt; *ctz=tz; ppix++; }
			ct++; ctz++;

			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}

	tabs->ctx->query_cur+=ppix;
}

void BGBRASW_DrawSpanTextureInterpZATestLinear(
	BGBRASW_TestBlendData *tabs,
	bgbrasw_pixel *span, bgbrasw_zbuf *spanz, int npix, int xo, int yo,
	bgbrasw_pixel *tex, int txs, int tys,
	int st0s, int st0t, int st1s, int st1t,
	bgbrasw_pixel clr0, bgbrasw_pixel clr1,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1)
{
	bgbrasw_pixel *ct, *cte;
	bgbrasw_zbuf *ctz, *ctze;
	bgbrasw_pixel tclr0, tclr1, tclr2, tclr3;
	u32 tz, tzv;
	u32 tz0, tz1, tz2, tz3;
	int tx0, ty0, tx1, ty1;
	int tx2, ty2, tx3, ty3;
	int ts, tt, tsv, ttv, tx, ty, dsv, dtv;
	int cr, crv, crt, cg, cgv, cgt;
	int cb, cbv, cbt, ca, cav, cat;
	int txsn1, tysn1;
	int clr, clrt, npix1, npfrac, ppix;

	if(((u32)yo)>=tabs->ctx->tgt_ys)
		return;
	if(npix<=0)return;

	cr=BGBRASW_PIXEL_R(clr0)<<8;	cg=BGBRASW_PIXEL_G(clr0)<<8;
	cb=BGBRASW_PIXEL_B(clr0)<<8;	ca=BGBRASW_PIXEL_A(clr0)<<8;

	npfrac=bgbrasw_fixdiv_s32_u16(256, npix);
	crv=(BGBRASW_PIXEL_R(clr1)-BGBRASW_PIXEL_R(clr0))*npfrac;
	cgv=(BGBRASW_PIXEL_G(clr1)-BGBRASW_PIXEL_G(clr0))*npfrac;
	cbv=(BGBRASW_PIXEL_B(clr1)-BGBRASW_PIXEL_B(clr0))*npfrac;
	cav=(BGBRASW_PIXEL_A(clr1)-BGBRASW_PIXEL_A(clr0))*npfrac;

	tz=z0; tzv=bgbrasw_fixdiv_s32_u16(z1-z0, npix);
	tzv&=BGBRASW_MASK_DEPTH;

	ts=st0s; tt=st0t;

	tsv=bgbrasw_fixdiv_s32_u16(st1s-st0s, npix);
	ttv=bgbrasw_fixdiv_s32_u16(st1t-st0t, npix);
	txsn1=txs-1; tysn1=tys-1;

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
	if((xo+npix)>tabs->ctx->tgt_xs)
	{
		npix=tabs->ctx->tgt_xs-xo;
		if(npix<=0)
			return;
	}
	
	ppix=0;

	ct=span+yo*tabs->ctx->tgt_xs+xo; cte=ct+npix;
	ctz=spanz+yo*tabs->ctx->tgt_xs+xo; ctze=ctz+npix;

	if(tz>(*ctz))
	{
		//Z fail
		while(ct<cte)
		{
			if(tz<=(*ctz))
				goto loopPass;
			loopFail:
			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ct++; ctz++;
			ts+=tsv; tt+=ttv; tz+=tzv;
			continue;
		}
	}else
	{
		while(ct<cte)
		{
			if(tz>(*ctz))
				goto loopFail;
			loopPass:

			tx=ts>>8;	ty=tt>>8;
			tx0=(tx  )&(txs-1);	ty0=(ty  )&(tys-1);
			tx1=(tx+1)&(txs-1);	ty1=(ty  )&(tys-1);
			tx2=(tx  )&(txs-1);	ty2=(ty+1)&(tys-1);
			tx3=(tx+1)&(txs-1);	ty3=(ty+1)&(tys-1);
			tclr0=tex[ty0*txs+tx0];	tclr1=tex[ty1*txs+tx1];
			tclr2=tex[ty2*txs+tx2];	tclr3=tex[ty3*txs+tx3];
			clrt=bgbrasw_interp_linear(
				tclr0, tclr1, tclr2, tclr3,
				ts&255, tt&255);

			cat= (((clrt>>16)&0xFF00)*ca)     &0xFF000000;
			crt= (((byte)(clrt>>16))*cr)     &0x00FF0000;
			cgt=((((byte)(clrt>> 8))*cg)>> 8)&0x0000FF00;
			cbt=((((byte) clrt     )*cb)>>16);
			clr=cat|crt|cgt|cbt;

			//Z pass
			if(clr&0x80000000)
				{ *ct=clr; *ctz=tz; ppix++; }
			ct++; ctz++;

			cr+=crv;	cg+=cgv;
			cb+=cbv;	ca+=cav;
			ts+=tsv; tt+=ttv; tz+=tzv;
		}
	}

	tabs->ctx->query_cur+=ppix;
}
