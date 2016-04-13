#include <bteifgl.h>

#define BS2CC_IMG_TWOCC(a, b)			(((a)<<8)|(b))
#define BS2CC_IMG_FOURCC(a, b, c, d)	(((a)<<24)|((b)<<16)|((c)<<8)|(d))

#define BS2CC_IFCC_BS2I	BS2C_IMG_FOURCC('B', 'S', '2', 'I')
#define BS2CC_IFCC_STRS	BS2C_IMG_FOURCC('S', 'T', 'R', 'S')
#define BS2CC_IFCC_BADD	BS2C_IMG_FOURCC('B', 'A', 'D', 'D')

#define BS2CC_ITCC_BC	BS2C_IMG_TWOCC('B', 'C')
#define BS2CC_ITCC_CL	BS2C_IMG_TWOCC('C', 'L')
#define BS2CC_ITCC_GF	BS2C_IMG_TWOCC('G', 'F')
#define BS2CC_ITCC_GV	BS2C_IMG_TWOCC('G', 'V')
#define BS2CC_ITCC_IF	BS2C_IMG_TWOCC('I', 'F')
#define BS2CC_ITCC_PK	BS2C_IMG_TWOCC('P', 'K')
#define BS2CC_ITCC_SF	BS2C_IMG_TWOCC('S', 'F')
#define BS2CC_ITCC_ST	BS2C_IMG_TWOCC('S', 'T')
#define BS2CC_ITCC_SV	BS2C_IMG_TWOCC('S', 'V')

#define BS2CC_I1CC_NAME		'N'
#define BS2CC_I1CC_QNAME	'Q'
#define BS2CC_I1CC_SIG		'S'
#define BS2CC_I1CC_FLAGS	'F'
#define BS2CC_I1CC_PACKAGE	'P'


#define BS2CC_I1CC_NARGS	'A'
#define BS2CC_I1CC_BARGS	'B'
#define BS2CC_I1CC_CODE		'C'
#define BS2CC_I1CC_STKDEPTH	'D'

#define BS2CC_I1CC_NLOCALS	'L'


byte *BS2C_Image_EmitTag(byte *ct, u32 tag, int sz)
{
	int sz1;
	
	if(tag<0x0100)
	{
		if(sz<0x001E)
		{
			sz1=sz+2;
			*ct++=0x40+sz1;
			*ct++=tag;
			return(ct);
		}

		if(sz<0x1FFD)
		{
			sz1=sz+3;
			*ct++=0x60+((sz>>8)&0x3F);
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

	if((tag>='A') && (tag<='Z'))
	{
		*ct++=0x01+(tag-'A');
		ct=BS2C_Image_EmitSVLI(ct, val);
		return(ct);
	}

	if((tag>='a') && (tag<='z'))
	{
		*ct++=0x21+(tag-'a');
		ct=BS2C_Image_EmitSVLI(ct, val);
		return(ct);
	}

	ct1=BS2C_Image_EmitSVLI(tb, val);
	ct=BS2C_Image_EmitTagData(ct, tag, ct1-tb, tb);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_GblDefI(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	BS2CC_CcFrame *frm;
	int i, j, k;

	if(vari->bmfl&BS2CC_TYFL_PUBLIC)
	{
		i=BS2C_ImgLookupString(ctx, vari->name);
		if(i>0)
			{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_NAME, i); }
	}

	i=BS2C_ImgLookupString(ctx, vari->sig);
	if(i>0)
		{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_SIG, i); }
	
	if(vari->pkg)
	{
		i=vari->pkg->gid;
		ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_PACKAGE, i);
	}

	if((vari->vitype==BS2CC_VITYPE_GBLFUNC) ||
		(vari->vitype==BS2CC_VITYPE_STRFUNC))
	{
		ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_NARGS, vari->nargs);
	}

	frm=vari->body;
	if(frm && frm->cts)
	{
		ct=BS2C_Image_EmitTagSVLI(ct,
			BS2CC_I1CC_BARGS, frm->bargs);

		ct=BS2C_Image_EmitTagSVLI(ct,
			BS2CC_I1CC_NLOCALS, frm->nlocals);

		ct=BS2C_Image_EmitTagSVLI(ct,
			BS2CC_I1CC_STKDEPTH, frm->stacksize);

		ct=BS2C_Image_EmitTagData(ct, BS2CC_I1CC_CODE,
			frm->szt, frm->cts);
	}
	
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_GblVar(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	byte *ct1, *ct2;
	
	ct1=ct+16;
	ct2=BS2C_Image_FlattenGlobalInfo_GblDefI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_ITCC_GV, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_GblFunc(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	byte *ct1, *ct2;
	
	ct1=ct+16;
	ct2=BS2C_Image_FlattenGlobalInfo_GblDefI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_ITCC_GF, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_StrVar(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	byte *ct1, *ct2;
	
	ct1=ct+16;
	ct2=BS2C_Image_FlattenGlobalInfo_GblDefI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_ITCC_SV, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_StrFunc(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	byte *ct1, *ct2;
	
	ct1=ct+16;
	ct2=BS2C_Image_FlattenGlobalInfo_GblDefI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_ITCC_SF, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_StructI(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	if(vari->bmfl&BS2CC_TYFL_PUBLIC)
	{
		i=BS2C_ImgLookupString(ctx, vari->name);
		if(i>0)
			{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_NAME, i); }
	}

	if(vari->pkg)
	{
		i=vari->pkg->gid;
		ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_PACKAGE, i);
	}

}

byte *BS2C_Image_FlattenGlobalInfo(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	if(vari->vitype==BS2CC_VITYPE_GBLVAR)
	{
		ct=BS2C_Image_FlattenGlobalInfo_GblVar(ctx, vari, ct);
		return(ct);
	}

	if(vari->vitype==BS2CC_VITYPE_GBLFUNC)
	{
		ct=BS2C_Image_FlattenGlobalInfo_GblFunc(ctx, vari, ct);
		return(ct);
	}
	
	if(vari->vitype==BS2CC_VITYPE_STRVAR)
	{
		ct=BS2C_Image_FlattenGlobalInfo_StrVar(ctx, vari, ct);
		return(ct);
	}

	if(vari->vitype==BS2CC_VITYPE_STRFUNC)
	{
		ct=BS2C_Image_FlattenGlobalInfo_StrFunc(ctx, vari, ct);
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
		if(!(vari->bmfl&BS2CC_TYFL_CANREACH))
		{
			gix[i]=0;
			continue;
		}

		gix[i]=ct-obuf;
		ct=BS2C_Image_FlattenGlobalInfo(
			ctx, globals[i], ct);
	}
}

BTEIFGL_API void BS2C_TouchReachable_TouchReachDef(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari)
{
	BS2CC_CcFrame *frm;
	int i, j, k;

	if(vari->bmfl&BS2CC_TYFL_CANREACH)
		return;

	vari->bmfl|=BS2CC_TYFL_CANREACH;
	BS2C_ImgGetString(ctx, vari->sig);
	
	for(i=0; i<vari->nargs; i++)
	{
		BS2C_TouchReachable_TouchReachDef(ctx, vari->args[i]);
	}
	
	if(vari->body)
	{
		frm=vari->body;
		for(i=0; i<frm->ngbl; i++)
		{
			BS2C_TouchReachable_TouchReachDef(ctx,
				ctx->globals[frm->gbltab[i]]);
		}
	}
}

BTEIFGL_API void BS2C_TouchReachable_TouchPublicDef(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari)
{
	if(!(vari->bmfl&BS2CC_TYFL_PUBLIC))
		return;

	BS2C_ImgGetString(ctx, vari->name);
//	BS2C_ImgGetString(ctx, vari->sig);

	BS2C_TouchReachable_TouchReachDef(ctx, vari);
}

BTEIFGL_API int BS2C_TouchReachable(
	BS2CC_CompileContext *ctx)
{
	BS2CC_PkgFrame *pcur;
	BS2CC_VarInfo *vcur;
	
	pcur=ctx->pkg_first;
	while(pcur)
	{
		vcur=ctx->globals[pcur->gid];
		vcur->bmfl|=BS2CC_TYFL_CANREACH;

		vcur=pcur->vars;
		while(vcur)
		{
			BS2C_TouchReachable_TouchDef(ctx, vcur);
			vcur=vcur->pknext;
		}
		pcur=pcur->next;
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
