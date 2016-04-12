#include <bteifgl.h>

#define BS2CC_IMG_TWOCC(a, b)			(((a)<<8)|(b))
#define BS2CC_IMG_FOURCC(a, b, c, d)	(((a)<<24)|((b)<<16)|((c)<<8)|(d))

#define BS2CC_IFCC_BS2I	BS2C_IMG_FOURCC('B', 'S', '2', 'I')
#define BS2CC_IFCC_STRS	BS2C_IMG_FOURCC('S', 'T', 'R', 'S')
#define BS2CC_IFCC_BADD	BS2C_IMG_FOURCC('B', 'A', 'D', 'D')

#define BS2CC_I1CC_NAME		'N'


byte *BS2C_Image_EmitTag(byte *ct, u32 tag, int sz)
{
	int sz1;
	if(tag<0x0100)
	{
		if(sz<0x003E)
		{
			sz1=sz+2;
			*ct++=sz1;
			*ct++=tag;
			return(ct);
		}

		if(sz<0x3FFD)
		{
			sz1=sz+3;
			*ct++=0x40+((sz>>8)&0x3F);
			*ct++=sz1&255;
			*ct++=tag;
			return(ct);
		}

		if(sz<0x0FFFFFF9)
		{
			sz1=sz+6;
			*ct++=0x90+((sz1>>24)&0x0F);
			*ct++=(sz1>>16)&255;
			*ct++=(sz1>> 8)&255;
			*ct++=(sz1    )&255;
			*ct++=(tag>>8)&255;
			*ct++=(tag   )&255;
			return(ct);
		}
	}

	if(tag<0x00010000)
	{
		if(sz<0x0FFC)
		{
			sz1=sz+4;
			*ct++=0x80+((sz1>>8)&0x0F);
			*ct++=sz1&255;
			*ct++=(tag>>8)&255;
			*ct++=(tag   )&255;
			return(ct);
		}

		if(sz<0x0FFFFFF9)
		{
			sz1=sz+6;
			*ct++=0x90+((sz1>>24)&0x0F);
			*ct++=(sz1>>16)&255;
			*ct++=(sz1>> 8)&255;
			*ct++=(sz1    )&255;
			*ct++=(tag>>8)&255;
			*ct++=(tag   )&255;
			return(ct);
		}
	}

	if(sz<0x0FF9)
	{
		sz1=sz+6;
		*ct++=0xA0+((sz1>>8)&0x0F);
		*ct++=sz1&255;
		*ct++=(tag>>24)&255;
		*ct++=(tag>>16)&255;
		*ct++=(tag>> 8)&255;
		*ct++=(tag    )&255;
		return(ct);
	}

	if(sz<0x0FFFFFF7)
	{
		sz1=sz+8;
		*ct++=0xB0+((sz>>24)&0x0F);
		*ct++=(sz1>>16)&255;
		*ct++=(sz1>> 8)&255;
		*ct++=(sz1    )&255;
		*ct++=(tag>>24)&255;
		*ct++=(tag>>16)&255;
		*ct++=(tag>> 8)&255;
		*ct++=(tag    )&255;
		return(ct);
	}
}

byte *BS2C_Image_EmitTagData(byte *ct, u32 tag, int sz,
	byte *data)
{
	ct=BS2C_Image_EmitTag(ct, tag, sz);
	memcpy(ct, data, sz);
	ct+=sz;
	return(ct);
}

byte *BS2C_Image_EmitUVLI(byte *ct, u64 v)
{
	if(v<0x80)
	{
		*ct++=v;
		return(ct);
	}
	if(v<0x4000)
	{
		*ct++=0x80|(v>>8);
		*ct++=v;
		return(ct);
	}
	if(v<0x200000)
	{
		*ct++=0xC0|(v>>16);
		*ct++=v>>8;
		*ct++=v;
		return(ct);
	}
	if(v<0x10000000)
	{
		*ct++=0xE0|(v>>24);
		*ct++=v>>16;
		*ct++=v>>8;
		*ct++=v;
		return(ct);
	}
	if(v<0x800000000ULL)
	{
		*ct++=0xF0|(v>>32);
		*ct++=v>>24;
		*ct++=v>>16;
		*ct++=v>>8;
		*ct++=v;
		return(ct);
	}
	if(v<0x40000000000ULL)
	{
		*ct++=0xF8|(v>>40);
		*ct++=v>>32;
		*ct++=v>>24;
		*ct++=v>>16;
		*ct++=v>>8;
		*ct++=v;
		return(ct);
	}
	if(v<0x2000000000000ULL)
	{
		*ct++=0xFC|(v>>48);
		*ct++=v>>40;
		*ct++=v>>32;
		*ct++=v>>24;
		*ct++=v>>16;
		*ct++=v>>8;
		*ct++=v;
		return(ct);
	}

	if(v<0x100000000000000ULL)
	{
		*ct++=0xFE;
		*ct++=v>>48;
		*ct++=v>>40;
		*ct++=v>>32;
		*ct++=v>>24;
		*ct++=v>>16;
		*ct++=v>>8;
		*ct++=v;
		return(ct);
	}

	*ct++=0xFF;
	*ct++=v>>56;
	*ct++=v>>48;
	*ct++=v>>40;
	*ct++=v>>32;
	*ct++=v>>24;
	*ct++=v>>16;
	*ct++=v>>8;
	*ct++=v;
	return(ct);
}

byte *BS2C_Image_EmitSVLI(byte *ct, s64 v)
{
	return(BS2C_Image_EmitUVLI(ct, (v<<1)^(v>>63)));
}

byte *BS2C_Image_EmitTagUVLI(byte *ct, u32 tag, u64 val)
{
	byte tb[16];
	byte *ct1;

	ct1=BS2C_Image_EmitUVLI(tb, val);
	ct=BS2C_Image_EmitTagData(ct, tag, ct1-tb, tb);
	return(ct);
}

byte *BS2C_Image_EmitTagSVLI(byte *ct, u32 tag, s64 val)
{
	byte tb[16];
	byte *ct1;

	ct1=BS2C_Image_EmitSVLI(tb, val);
	ct=BS2C_Image_EmitTagData(ct, tag, ct1-tb, tb);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_Var(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
}

byte *BS2C_Image_FlattenGlobalInfo(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	if(vari->vitype==BS2CC_VITYPE_GBLVAR)
	{
		ct=BS2C_Image_FlattenGlobalInfo_Var(ctx, vari, ct);
		return(ct);
	}
	
	ct=BS2C_Image_EmitTag(ct, BS2CC_IFCC_BADD, 0);
	return(ct);
}

byte *BS2C_Image_FlattenGlobals(
	BS2CC_CompileContext *ctx,
	byte *obuf, int *gix)
{
	byte *ct;
	int i, j, k;

	ct=obuf;
	for(i=0; i<ctx->nglobals; i++)
	{
		gix[i]=ct-obuf;
		ct=BS2C_Image_FlattenGlobalInfo(
			ctx, globals[i], ct);
	}
}

BTEIFGL_API int BS2C_FlattenImage(
	BS2CC_CompileContext *ctx,
	byte *obuf, int obmsz)
{
	byte *ct;
	
	ct=obuf;
	ct=BS2C_Image_EmitTag(ct, BS2CC_IFCC_BS2I, 0x0FFFFFF0);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_IFCC_STRS,
		ctx->strtabct-ctx->strtab, ctx->strtab);
}
