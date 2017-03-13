#include <bgbrasw.h>

BGBRASW_Texture *BGBRASW_LookupTexture(
	BGBRASW_Context *ctx, int texid)
{
	BGBRASW_Texture *cur;
	int h;
	
	h=((texid*4093)>>12)&1023;
	cur=ctx->texhash[h];
	while(cur)
	{
		if(cur->texid==texid)
			return(cur);
		cur=cur->hnext;
	}
	return(NULL);
}

BGBRASW_Texture *BGBRASW_GetTexture(
	BGBRASW_Context *ctx, int texid)
{
	BGBRASW_Texture *cur;
	int i, j, h;
	
	cur=BGBRASW_LookupTexture(ctx, texid);
	if(cur)
		return(cur);
		
	cur=BGBRASW_AllocTexture(ctx);
	cur->texid=texid;
	
	cur->next=ctx->texlist;
	ctx->texlist=cur;

	h=((texid*4093)>>12)&1023;
	cur->next=ctx->texhash[h];
	ctx->texhash[h]=cur;
	
	if((texid>0) || (texid<32768))
	{
		i=(texid>>5); j=texid&31;
		ctx->tex_alloc[i]|=(1UL<<j);
	}
	
	return(cur);
}

int BGBRASW_DeleteTexture(
	BGBRASW_Context *ctx, int texid)
{
	BGBRASW_Texture *cur;
	int i, j, h;
	
	BGBRASW_Finish(ctx);
	
	cur=BGBRASW_LookupTexture(ctx, texid);
	if(!cur)return(0);
		
	if((texid>0) || (texid<32768))
	{
		i=(texid>>5); j=texid&31;
		ctx->tex_alloc[i]&=~(1UL<<j);
	}

	for(i=0; i<16; i++)
	{
		if(cur->rgba[i])
		{
			bgbrasw_free(cur->rgba[i]);
			cur->rgba[i]=NULL;
		}
	}

	return(0);
}

int BGBRASW_AllocTextureID(BGBRASW_Context *ctx)
{
	int i, j, k;

	ctx->tex_alloc[0]|=1;

	i=ctx->tex_alcrov;
	for(j=0; j<32; j++)
		if(!(ctx->tex_alloc[i]&(1UL<<j)))
	{
		ctx->tex_alloc[i]|=(1UL<<j);
		return(i*32+j);
	}

	for(i=0; i<1024; i++)
	{
		if(ctx->tex_alloc[i]==0xFFFFFFFFUL)
			continue;
		for(j=0; j<32; j++)
			if(!(ctx->tex_alloc[i]&(1UL<<j)))
		{
			ctx->tex_alcrov=i;
			ctx->tex_alloc[i]|=(1UL<<j);
			return(i*32+j);
		}
	}
	return(-1);
}


BGBRASW_API void BGBRASW_SetTexturePixelsSize(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex,
		bgbrasw_pixel *pix, int xs, int ys, int lvl)
{
	int i, j, k;

	BGBRASW_Finish(ctx);

	if(!(tex->rgba[lvl]))
	{
		tex->rgba[lvl]=bgbrasw_malloc(
			xs*ys*sizeof(bgbrasw_pixel));
		if(!lvl)
		{
			tex->xs=xs;
			tex->ys=ys;
		}
	}
	
	if(tex->rgba[lvl] && (tex->xs==(xs<<lvl)) && (tex->ys==(ys<<lvl)))
	{
		if(pix)memcpy(tex->rgba[lvl], pix, xs*ys*sizeof(bgbrasw_pixel));
		return;
	}
	
	if(tex->rgba[lvl])
	{
		bgbrasw_free(tex->rgba[lvl]);
		tex->rgba[lvl]=NULL;
	}

	tex->rgba[lvl]=bgbrasw_malloc(
		xs*ys*sizeof(bgbrasw_pixel));
	if(pix)memcpy(tex->rgba[lvl], pix, xs*ys*sizeof(bgbrasw_pixel));
	if(!lvl)
	{
		tex->xs=xs;
		tex->ys=ys;
	}
}

BGBRASW_API void BGBRASW_SetTextureSizeMip(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex, int xs, int ys, int lvl)
{
	int i, j, k;

//	if(lvl)
//	{
//		if((tex->xs!=(xs<<lvl)) && (tex->ys==(ys<<lvl)))
//			return;
//	}

	BGBRASW_Finish(ctx);

	if(!(tex->rgba[lvl]))
	{
		tex->rgba[lvl]=bgbrasw_malloc(
			xs*ys*sizeof(bgbrasw_pixel));
		if(!lvl)
		{
			tex->xs=xs;
			tex->ys=ys;
		}
		return;
	}
	
	if(lvl)return;

	if((tex->xs==xs) && (tex->ys==ys))
		return;

#if 1
	for(i=0; i<16; i++)
	{
		if(tex->rgba[i])
		{
			bgbrasw_free(tex->rgba[i]);
			tex->rgba[i]=NULL;
		}
	}
#endif

	tex->rgba[lvl]=bgbrasw_malloc(
		xs*ys*sizeof(bgbrasw_pixel));
	tex->xs=xs;
	tex->ys=ys;
}


BGBRASW_API void BGBRASW_SetCompressedTexturePixelsSize(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex,
		bgbrasw_pixel *pix, int xs, int ys, int lvl, int fmt)
{
	int xs1, ys1, bsz;
	int i, j, k;

	BGBRASW_Finish(ctx);

	xs1=(xs+3)>>2;
	ys1=(ys+3)>>2;

	switch(fmt)
	{
	case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		bsz=8;
		break;
	default:
		bsz=16;
		break;
	}

	if(!(tex->cmptx[lvl]))
	{
		tex->cmptx[lvl]=bgbrasw_malloc(xs1*ys1*bsz);
		if(!lvl)
		{
			tex->xs=xs;
			tex->ys=ys;
			tex->cm=fmt;
		}
	}
	
	if(tex->cmptx[lvl] && (tex->xs==(xs<<lvl)) && (tex->ys==(ys<<lvl)))
	{
		if(pix)memcpy(tex->cmptx[lvl], pix, xs1*ys1*bsz);
		return;
	}
	
	if(tex->cmptx[lvl])
	{
		bgbrasw_free(tex->cmptx[lvl]);
		tex->cmptx[lvl]=NULL;
	}

	tex->cmptx[lvl]=bgbrasw_malloc(xs1*ys1*bsz);
	if(pix)memcpy(tex->cmptx[lvl], pix, xs1*ys1*bsz);
	if(!lvl)
	{
		tex->xs=xs;
		tex->ys=ys;
		tex->cm=fmt;
	}
}

BGBRASW_API void BGBRASW_SetCompressedTextureSizeMip(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex,
	int xs, int ys, int lvl, int fmt)
{
	int xs1, ys1, bsz;
	int i, j, k;

	BGBRASW_Finish(ctx);

	xs1=(xs+3)>>2;
	ys1=(ys+3)>>2;

	switch(fmt)
	{
	case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		bsz=8;
		break;
	default:
		bsz=16;
		break;
	}

	if(!(tex->cmptx[lvl]))
	{
		tex->cmptx[lvl]=bgbrasw_malloc(xs1*ys1*bsz);
		if(!lvl)
		{
			tex->xs=xs;
			tex->ys=ys;
			tex->cm=fmt;
		}
		return;
	}
	
	if(lvl)return;

	if((tex->xs==xs) && (tex->ys==ys))
		return;

#if 1
	for(i=0; i<16; i++)
	{
		if(tex->rgba[i])
		{
			bgbrasw_free(tex->cmptx[i]);
			tex->cmptx[i]=NULL;
		}
	}
#endif

	tex->cmptx[lvl]=bgbrasw_malloc(xs1*ys1*bsz);
	tex->xs=xs;
	tex->ys=ys;
	tex->cm=fmt;
}


u32 bgbrasw_rgb565to24(u16 px)
{
	int cr, cg, cb;
	u32 pxb;
	
	cr=(px&0xF800)>>8;
	cg=(px&0x07E0)>>3;
	cb=(px&0x001F)<<3;
	cr|=cr>>5;
	cg|=cg>>6;
	cb|=cb>>5;
	
	pxb=(255<<24)|(cr<<16)|(cg<<8)|cb;
	return(pxb);
}

u32 bgbrasw_rgb565to24_33p(u16 px0, u16 px1)
{
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;

	int cr, cg, cb;
	u32 pxb;
	
	cr0=(px0&0xF800)>>8;
	cg0=(px0&0x07E0)>>3;
	cb0=(px0&0x001F)<<3;
	cr1=(px1&0xF800)>>8;
	cg1=(px1&0x07E0)>>3;
	cb1=(px1&0x001F)<<3;

	cr=(cr0*11+cr1*5)>>4;
	cg=(cg0*11+cg1*5)>>4;
	cb=(cb0*11+cb1*5)>>4;

	cr+=cr>>5;
	cg+=cg>>6;
	cb+=cb>>5;
	
	pxb=(255<<24)|(cr<<16)|(cg<<8)|cb;
	return(pxb);
}

void BGBRASW_DecodeBlock(byte *ibuf, bgbrasw_pixel *obuf, int fmt, int ystr)
{
	bgbrasw_pixel *ct;
	byte *ctb;
	u32 clrtab[8];
	u32 pxb, pxc, clrc, clrd;
	int clra, clrb;

	if(fmt==3)
	{
		BGBRASW_DecodeBlock(ibuf+8, obuf, 1, ystr);
		BGBRASW_DecodeBlock(ibuf+0, obuf, 4, ystr);
		return;
	}

	if(fmt==1)
	{
		clra=ibuf[0]|(ibuf[1]<<8);
		clrb=ibuf[2]|(ibuf[3]<<8);
		pxb=ibuf[4]|(ibuf[5]<<8)|(ibuf[6]<<16)|(ibuf[7]<<24);

		if(clrb>=clra)
		{
			clrtab[0]=bgbrasw_rgb565to24(clra);
			clrtab[1]=bgbrasw_rgb565to24(clrb);
			clrtab[2]=bgbrasw_rgb565to24_33p(clra, clrb);
			clrtab[3]=0;
		}else
		{
			clrtab[0]=bgbrasw_rgb565to24(clra);
			clrtab[1]=bgbrasw_rgb565to24(clrb);
			clrtab[2]=bgbrasw_rgb565to24_33p(clra, clrb);
			clrtab[3]=bgbrasw_rgb565to24_33p(clrb, clra);
		}
		
		ct=obuf;
		ct[0]=clrtab[(pxb>> 0)&3];	ct[1]=clrtab[(pxb>> 2)&3];
		ct[2]=clrtab[(pxb>> 4)&3];	ct[3]=clrtab[(pxb>> 6)&3];
		ct+=ystr;
		ct[0]=clrtab[(pxb>> 8)&3];	ct[1]=clrtab[(pxb>>10)&3];
		ct[2]=clrtab[(pxb>>12)&3];	ct[3]=clrtab[(pxb>>14)&3];
		ct+=ystr;
		ct[0]=clrtab[(pxb>>16)&3];	ct[1]=clrtab[(pxb>>18)&3];
		ct[2]=clrtab[(pxb>>20)&3];	ct[3]=clrtab[(pxb>>22)&3];
		ct+=ystr;
		ct[0]=clrtab[(pxb>>24)&3];	ct[1]=clrtab[(pxb>>26)&3];
		ct[2]=clrtab[(pxb>>28)&3];	ct[3]=clrtab[(pxb>>30)&3];
		return;
	}

	if(fmt==4)
	{
		clra=ibuf[0];
		clrb=ibuf[1];
		pxb=ibuf[2]|(ibuf[3]<<8)|(ibuf[4]<<16);
		pxc=ibuf[5]|(ibuf[6]<<8)|(ibuf[7]<<16);
		if(clrb>=clra)
		{
			clrtab[0]=clra;	clrtab[1]=clrb;
			clrtab[2]=(7*clra+1*clrb)>>3;
			clrtab[3]=(5*clra+3*clrb)>>3;
			clrtab[4]=(3*clra+5*clrb)>>3;
			clrtab[5]=(1*clra+7*clrb)>>3;
			clrtab[6]=0;	clrtab[7]=255;
		}else
		{
			clrtab[0]=clra;	clrtab[1]=clrb;
			clrtab[2]=(7*clra+1*clrb)>>3;
			clrtab[3]=(6*clra+2*clrb)>>3;
			clrtab[4]=(5*clra+3*clrb)>>3;
			clrtab[5]=(3*clra+5*clrb)>>3;
			clrtab[6]=(2*clra+6*clrb)>>3;
			clrtab[7]=(1*clra+7*clrb)>>3;
		}

		ctb=(byte *)obuf;
		ctb[ 3]=clrtab[(pxb>> 0)&7];	ctb[ 7]=clrtab[(pxb>> 3)&7];
		ctb[11]=clrtab[(pxb>> 6)&7];	ctb[15]=clrtab[(pxb>> 9)&7];
		ctb+=ystr*4;
		ctb[ 3]=clrtab[(pxb>>12)&7];	ctb[ 7]=clrtab[(pxb>>15)&7];
		ctb[11]=clrtab[(pxb>>18)&7];	ctb[15]=clrtab[(pxb>>21)&7];
		ctb+=ystr*4;
		ctb[ 3]=clrtab[(pxc>> 0)&7];	ctb[ 7]=clrtab[(pxc>> 3)&7];
		ctb[11]=clrtab[(pxc>> 6)&7];	ctb[15]=clrtab[(pxc>> 9)&7];
		ctb+=ystr*4;
		ctb[ 3]=clrtab[(pxc>>12)&7];	ctb[ 7]=clrtab[(pxc>>15)&7];
		ctb[11]=clrtab[(pxc>>18)&7];	ctb[15]=clrtab[(pxc>>21)&7];
		return;
	}
	
	if(fmt==7)
	{
		BGBBTJ_BC7_DecodeBlock(
			ibuf, obuf, 4, ystr*4, 1);
		return;
	}
}

void BGBRASW_DecodeImage(byte *ibuf, bgbrasw_pixel *obuf,
	int xs, int ys, int bsz, int fmt)
{
	bgbrasw_pixel *ct;
	byte *cs;
	int xs1, ys1;
	int x, y;

	xs1=(xs+3)>>2;
	ys1=(ys+3)>>2;
	
	cs=ibuf;
	for(y=0; y<ys1; y++)
		for(x=0; x<xs1; x++)
	{
		cs=ibuf+(y*xs1+x)*bsz;
		ct=obuf+y*4*xs+x*4;
		BGBRASW_DecodeBlock(cs, ct, fmt, xs);
	}
}

BGBRASW_API void BGBRASW_CheckDecodeTexture(
	BGBRASW_Context *ctx, BGBRASW_Texture *tex, int mip)
{
	int txs, tys, txs2, tys2;
	int i, j, k;

	if(!tex->cmptx[mip])
		return;

	txs=tex->xs>>mip;
	tys=tex->ys>>mip;
	if(!txs)txs=1;
	if(!tys)tys=1;
	
	txs2=((txs+3)/4)*4;
	tys2=((tys+3)/4)*4;
	
	tex->rgba[mip]=bgbrasw_malloc(
		txs2*tys2*sizeof(bgbrasw_pixel));
		
	switch(tex->cm)
	{
	case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		BGBRASW_DecodeImage(
			tex->cmptx[mip], tex->rgba[mip],
			txs, tys, 8, 1);
		break;
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		BGBRASW_DecodeImage(
			tex->cmptx[mip], tex->rgba[mip],
			txs, tys, 16, 3);
		break;
	case GL_COMPRESSED_RGBA_BPTC_UNORM:
	case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
		BGBRASW_DecodeImage(
			tex->cmptx[mip], tex->rgba[mip],
			txs, tys, 16, 7);
		break;
	
	default:
		k=0xFFFF00FFU;
		for(i=0; i<tys; i++)
			for(j=0; j<txs; j++)
		{
			k=((i^j)&1)?0xFFFF7FFFU:0xFF7FFF7FU;
			tex->rgba[mip][i*txs+j]=k;
		}
		break;
	}
}
