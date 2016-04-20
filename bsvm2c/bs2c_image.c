#include <bteifgl.h>


byte *BS2C_Image_EmitTag(byte *ct, u64 tag, s64 sz)
{
	s64 sz1;
	
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

		if(sz<0xFFFC)
		{
			sz1=sz+4;
			*ct++=0x40;
			*ct++=(sz1>>8)&255;
			*ct++=(sz1   )&255;
			*ct++=tag;
			return(ct);
		}

		sz1=sz+6;
		*ct++=0x41;
		*ct++=(sz1>>24)&255;
		*ct++=(sz1>>16)&255;
		*ct++=(sz1>> 8)&255;
		*ct++=(sz1    )&255;
		*ct++=(tag    )&255;
		return(ct);

#if 0
		if(sz<0x0FFFFFF9)
		{
			sz1=sz+6;
			*ct++=0x90+((sz1>>24)&0x0F);
			*ct++=(sz1>>16)&255;
			*ct++=(sz1>> 8)&255;
			*ct++=(sz1    )&255;
			*ct++=(tag    )&255;
			*ct++=(tag>> 8)&255;
			return(ct);
		}
#endif
	}

	if(tag<0x00010000)
	{
		if(sz<0x0FFC)
		{
			sz1=sz+4;
			*ct++=0x80+((sz1>>8)&0x0F);
			*ct++=sz1&255;
//			*ct++=(tag   )&255;
//			*ct++=(tag>>8)&255;

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
//			*ct++=(tag    )&255;
//			*ct++=(tag>> 8)&255;
			*ct++=(tag>> 8)&255;
			*ct++=(tag    )&255;
			return(ct);
		}
	}

	if(tag<0x100000000ULL)
	{
		if(sz<0x0FF9)
		{
			sz1=sz+6;
			*ct++=0xA0+((sz1>>8)&0x0F);
			*ct++=sz1&255;
//			*ct++=(tag    )&255;
//			*ct++=(tag>> 8)&255;
//			*ct++=(tag>>16)&255;
//			*ct++=(tag>>24)&255;

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
//			*ct++=(tag    )&255;
//			*ct++=(tag>> 8)&255;
//			*ct++=(tag>>16)&255;
//			*ct++=(tag>>24)&255;

			*ct++=(tag>>24)&255;
			*ct++=(tag>>16)&255;
			*ct++=(tag>> 8)&255;
			*ct++=(tag    )&255;
			return(ct);
		}
	}

	if(sz<0x03FFFFF3)
	{
		sz1=sz+12;
		*ct++=0xC0+((sz>>24)&0x03);
		*ct++=(sz1>>16)&255;
		*ct++=(sz1>> 8)&255;
		*ct++=(sz1    )&255;

//		*ct++=(tag    )&255;
//		*ct++=(tag>> 8)&255;
//		*ct++=(tag>>16)&255;
//		*ct++=(tag>>24)&255;
//		*ct++=(tag>>32)&255;
//		*ct++=(tag>>40)&255;
//		*ct++=(tag>>48)&255;
//		*ct++=(tag>>56)&255;

		*ct++=(tag>>56)&255;
		*ct++=(tag>>48)&255;
		*ct++=(tag>>40)&255;
		*ct++=(tag>>32)&255;
		*ct++=(tag>>24)&255;
		*ct++=(tag>>16)&255;
		*ct++=(tag>> 8)&255;
		*ct++=(tag    )&255;

		return(ct);
	}

//	if(sz<0x03FFFFF3)
	if(1)
	{
		sz1=sz+16;
		*ct++=0xC4+((sz>>56)&0x03);
		*ct++=(sz1>>48)&255;
		*ct++=(sz1>>40)&255;
		*ct++=(sz1>>32)&255;
		*ct++=(sz1>>24)&255;
		*ct++=(sz1>>16)&255;
		*ct++=(sz1>> 8)&255;
		*ct++=(sz1    )&255;

//		*ct++=(tag    )&255;
//		*ct++=(tag>> 8)&255;
//		*ct++=(tag>>16)&255;
//		*ct++=(tag>>24)&255;
//		*ct++=(tag>>32)&255;
//		*ct++=(tag>>40)&255;
//		*ct++=(tag>>48)&255;
//		*ct++=(tag>>56)&255;

		*ct++=(tag>>56)&255;
		*ct++=(tag>>48)&255;
		*ct++=(tag>>40)&255;
		*ct++=(tag>>32)&255;
		*ct++=(tag>>24)&255;
		*ct++=(tag>>16)&255;
		*ct++=(tag>> 8)&255;
		*ct++=(tag    )&255;

		return(ct);
	}
	
	return(ct);
}

byte *BS2C_Image_EmitTagData(byte *ct, u32 tag, int sz,
	byte *data)
{
	ct=BS2C_Image_EmitTag(ct, tag, sz);
	if(data)
		{ memcpy(ct, data, sz); }
	else
		{ memset(ct, 0, sz); }
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

	if((tag>='0') && (tag<='4'))
	{
		*ct++=0x1B+(tag-'A');
		ct=BS2C_Image_EmitSVLI(ct, val);
		return(ct);
	}

	if((tag>='5') && (tag<='9'))
	{
		*ct++=0x3B+(tag-'5');
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

//	if(vari->bmfl&BS2CC_TYFL_PUBLIC)
	if(vari->bmfl&BS2CC_TYFL_PUBVISIBLE)
	{
		i=BS2C_ImgLookupString(ctx, vari->name);
		if(i>0)
			{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_NAME, i); }
	}

	i=BS2C_ImgLookupString(ctx, vari->sig);
	if(i>0)
		{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_SIG, i); }
	
	if(	((vari->vitype==BS2CC_VITYPE_STRVAR) ||
		(vari->vitype==BS2CC_VITYPE_STRFUNC)) &&
		vari->obj)
	{
		i=vari->obj->gid;
		ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_ECLASS, i);
	}else if(vari->pkg)
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

//		ct=BS2C_Image_EmitTagSVLI(ct,
//			BS2CC_I1CC_NLOCALS, frm->nlocals);

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
	
	ct1=ct+256;
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
	
	ct1=ct+256;
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
	
	ct1=ct+256;
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
	
	ct1=ct+256;
	ct2=BS2C_Image_FlattenGlobalInfo_GblDefI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_ITCC_SF, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_StructI(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	byte *ct1, *ct2;
	int i, j, k;

//	if(vari->bmfl&BS2CC_TYFL_PUBLIC)
	if(vari->bmfl&BS2CC_TYFL_PUBVISIBLE)
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

	if(vari->super)
	{
		i=vari->super->gid;
		ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_ECLASS, i);
	}
	
	if(vari->niface>0)
	{
		ct1=ct+16; ct2=ct1;
		for(i=0; i<vari->niface; i++)
		{
			ct2=BS2C_Image_EmitUVLI(ct2,
				vari->iface[i]->gid);
		}

		ct=BS2C_Image_EmitTagData(ct, BS2CC_I1CC_IMPL, ct2-ct1, ct1);
	}

	ct1=ct+256; ct2=ct1;
	for(i=0; i<vari->nargs; i++)
	{
		ct2=BS2C_Image_EmitUVLI(ct2,
			vari->args[i]->gid);
	}

	ct=BS2C_Image_EmitTagData(ct, BS2CC_I1CC_GITAG, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_StructDef(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	byte *ct1, *ct2;
	
	ct1=ct+256;
	ct2=BS2C_Image_FlattenGlobalInfo_StructI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_ITCC_ST, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_ClassDef(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	byte *ct1, *ct2;
	
	ct1=ct+256;
	ct2=BS2C_Image_FlattenGlobalInfo_StructI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_ITCC_CL, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_IfaceDef(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	byte *ct1, *ct2;
	
	ct1=ct+256;
	ct2=BS2C_Image_FlattenGlobalInfo_StructI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(ct, BS2CC_ITCC_IF, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGlobalInfo_Package(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *obuf)
{
	BS2CC_PkgFrame *pkg;
	BS2CC_VarInfo *vcur;
	byte *ct, *ct1, *ct2, *ct3, *ct4;
	int i, j, k;
	
	pkg=vari->pkg;
	
	ct1=obuf+256; ct2=ct1;
	
	if(vari->bmfl&BS2CC_TYFL_PUBLIC)
	{
		i=BS2C_ImgLookupString(ctx, vari->qname);
		if(i>0)
			{ ct2=BS2C_Image_EmitTagSVLI(ct2, BS2CC_I1CC_QNAME, i); }
	}

	ct3=ct2+256; ct4=ct3;
	vcur=pkg->vars;
	while(vcur)
	{
		if(vcur->bmfl&BS2CC_TYFL_PUBVISIBLE)
		{
			ct4=BS2C_Image_EmitSVLI(ct4,
				vcur->gid);
		}
//		BS2C_TouchReachable_TouchPublicDef(ctx, vcur);
		vcur=vcur->pknext;
	}
	ct2=BS2C_Image_EmitTagData(ct2, BS2CC_I1CC_GITAG, ct4-ct3, ct3);
	
//	ct2=BS2C_Image_FlattenGlobalInfo_StructI(ctx, vari, ct1);
	ct=BS2C_Image_EmitTagData(obuf, BS2CC_ITCC_PK, ct2-ct1, ct1);
	return(ct);
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

	if(vari->vitype==BS2CC_VITYPE_STRUCT)
	{
		ct=BS2C_Image_FlattenGlobalInfo_StructDef(ctx, vari, ct);
		return(ct);
	}
	
	if(vari->vitype==BS2CC_VITYPE_CLASS)
	{
		ct=BS2C_Image_FlattenGlobalInfo_ClassDef(ctx, vari, ct);
		return(ct);
	}

	if(vari->vitype==BS2CC_VITYPE_IFACE)
	{
		ct=BS2C_Image_FlattenGlobalInfo_IfaceDef(ctx, vari, ct);
		return(ct);
	}

	if(vari->vitype==BS2CC_VITYPE_PACKAGE)
	{
		ct=BS2C_Image_FlattenGlobalInfo_Package(ctx, vari, ct);
		return(ct);
	}

	ct=BS2C_Image_EmitTag(ct, BS2CC_IFCC_BADD, 0);
	return(ct);
}

byte *BS2C_Image_FlattenGlobals(
	BS2CC_CompileContext *ctx,
	byte *obuf, int *gix)
{
	BS2CC_VarInfo *vari;
	byte *ct, *ct1, *ct2;
	int i, j, k;

	ct1=obuf+256; ct2=ct1;

	ct2=BS2C_Image_EmitTagData(ct2, BS2CC_I1CC_NULL, 0, NULL);

	for(i=0; i<ctx->nglobals; i++)
	{
		vari=ctx->globals[i];

		if(!(vari->bmfl&BS2CC_TYFL_CANREACH))
		{
			gix[i]=0;
			continue;
		}

		gix[i]=ct2-ct1;
		ct2=BS2C_Image_FlattenGlobalInfo(
			ctx, vari, ct2);
	}

	ct=BS2C_Image_EmitTagData(obuf, BS2CC_IFCC_GBLS, ct2-ct1, ct1);
	return(ct);
}

byte *BS2C_Image_FlattenGixArray(
	BS2CC_CompileContext *ctx,
	byte *obuf, int *gix)
{
	byte *ct, *ct0;
	int i, j, k, l, sz, s;
	
	l=0; sz=0;
	for(i=0; i<ctx->nglobals; i++)
	{
		j=gix[i];
		if(j)l=i+1;
		if(j>sz)j=sz;
	}
	
	if(sz<65536)
	{
		ct0=obuf+256; ct=ct0;
		for(i=1; i<l; i++)
		{
			j=gix[i];
			*ct++=(j>>8);
			*ct++=(j   );
		}

		ct=BS2C_Image_EmitTagData(obuf, BS2CC_IFCC_GIX2, ct-ct0, ct0);
		return(ct);
	}

	if(sz<16777216)
	{
		ct0=obuf+256; ct=ct0;
		for(i=1; i<l; i++)
		{
			j=gix[i];
			*ct++=(j>>16);
			*ct++=(j>> 8);
			*ct++=(j    );
		}

		ct=BS2C_Image_EmitTagData(obuf, BS2CC_IFCC_GIX3, ct-ct0, ct0);
		return(ct);
	}

	ct0=obuf+256; ct=ct0;
	for(i=1; i<l; i++)
	{
		j=gix[i];
		*ct++=(j>>24);
		*ct++=(j>>16);
		*ct++=(j>> 8);
		*ct++=(j    );
	}

	ct=BS2C_Image_EmitTagData(obuf, BS2CC_IFCC_GIX4, ct-ct0, ct0);
	return(ct);
}

BTEIFGL_API void BS2C_TouchReachable_TouchReachDef(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari)
{
	BS2CC_CcFrame *frm;
	BS2CC_VarInfo *vi;
	int i, j, k;

	if(vari->bmfl&BS2CC_TYFL_CANREACH)
		return;

	if((vari->bmfl&BS2CC_TYFL_PUBLIC) &&
		(vari->bmfl&BS2CC_TYFL_PUBVISIBLE))
	{
		BS2C_ImgGetString(ctx, vari->name);
	}

	vari->bmfl|=BS2CC_TYFL_CANREACH;
	BS2C_ImgGetString(ctx, vari->sig);
	
	if((vari->vitype==BS2CC_VITYPE_STRUCT) ||
		(vari->vitype==BS2CC_VITYPE_CLASS) ||
		(vari->vitype==BS2CC_VITYPE_IFACE))
	{
		if((vari->bmfl&BS2CC_TYFL_PUBLIC) &&
			(vari->bmfl&BS2CC_TYFL_PUBVISIBLE))
		{
			for(i=0; i<vari->nargs; i++)
			{
				vi=vari->args[i];
				if(vi->bmfl&BS2CC_TYFL_PUBLIC)
					vi->bmfl|=BS2CC_TYFL_PUBVISIBLE;
			}
		}

		for(i=0; i<vari->nargs; i++)
		{
			vi=vari->args[i];
			BS2C_TouchReachable_TouchReachDef(ctx, vi);
		}
	}else
	{
		for(i=0; i<vari->nargs; i++)
		{
			BS2C_TouchReachable_TouchReachDef(ctx, vari->args[i]);
		}
	}
	
	if(vari->body)
	{
		frm=vari->body;
		for(i=0; i<frm->ngbl; i++)
		{
			j=frm->gbltab[i];
			if(j&3)continue;
			BS2C_TouchReachable_TouchReachDef(ctx,
				ctx->globals[j>>2]);
		}
	}
}

BTEIFGL_API void BS2C_TouchReachable_TouchPublicDef(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari)
{
	if(!(vari->bmfl&BS2CC_TYFL_PUBLIC))
		return;

//	if((vari->vitype==BS2CC_VITYPE_STRUCT) ||
//		(vari->vitype==BS2CC_VITYPE_CLASS) ||
//		(vari->vitype==BS2CC_VITYPE_INTERFACE))
	if(vari->vitype==BS2CC_VITYPE_PACKAGE)
	{
		BS2C_ImgGetString(ctx, vari->qname);
	}else
	{
		BS2C_ImgGetString(ctx, vari->name);
//		BS2C_ImgGetString(ctx, vari->sig);
	}

	vari->bmfl|=BS2CC_TYFL_PUBVISIBLE;

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
			BS2C_TouchReachable_TouchPublicDef(ctx, vcur);
			vcur=vcur->pknext;
		}
		pcur=pcur->next;
	}
	return(0);
}

BTEIFGL_API int BS2C_FlattenImage(
	BS2CC_CompileContext *ctx,
	byte *obuf, int obmsz)
{
	int tgix[16384];
	byte *ct, *ct1, *ct2;
	int i, j, k;
	
	BS2C_TouchReachable(ctx);
	
	ct1=obuf+256; ct=ct1;
//	ct=BS2C_Image_EmitTag(ct, BS2CC_IFCC_BS2I, 0x0FFFFFF0);

	ct=BS2C_Image_EmitTagData(ct, BS2CC_IFCC_STRS,
		ctx->strct-ctx->strtab, ctx->strtab);

	ct=BS2C_Image_FlattenGlobals(ctx, ct, tgix);
	ct=BS2C_Image_FlattenGixArray(ctx, ct, tgix);

	ct=BS2C_Image_EmitTagData(obuf, BS2CC_IFCC_BS2I,
		ct-ct1, ct1);
	return(ct-obuf);
}
