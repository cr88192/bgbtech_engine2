/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


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
			*ct++=0x60+((sz1>>8)&0x3F);
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
	if(sz<0)
	{
		BSVM2_DBGTRAP
	}

	ct=BS2C_Image_EmitTag(ct, tag, sz);
	if(data)
	{
//		memcpy(ct, data, sz);
		memmove(ct, data, sz);
	}
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

byte *BS2C_Image_EmitFVLI(byte *ct, double val)
{
	u64 ix;
	s64 m;
	int e;

	ix=*(u64 *)(&val);

	m=(s64)((ix&0x000FFFFFFFFFFFFFULL)|
		0x0010000000000000ULL);
	e=((ix>>52)&2047)-(1023+52);

	while(!(m&255))
		{ m=m>>8; e=e+8; }
	while(!(m&1))
		{ m=m>>1; e++; }
	
	if(ix>>63)
		{ m=-m; }
	
	ct=BS2C_Image_EmitSVLI(ct, e);
	ct=BS2C_Image_EmitSVLI(ct, m);
	return(ct);
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

byte *BS2C_Image_EmitTagFloat(byte *ct, u32 tag, double val)
{
	byte tb[64];
	byte *ct1;

	ct1=BS2C_Image_EmitFVLI(tb, val);
	ct=BS2C_Image_EmitTagData(ct, tag, ct1-tb, tb);
	return(ct);
}

s64 BS2C_Image_QHashName(char *str)
{
	u64 h;
	char *s;
	int i;

	if(!str)
		return(-1);
	
	h=0;
	for(i=0; i<4; i++)
	{
		s=str;
		while(*s)
			{ h=(h*251)+(*s++); }
	}
	h=h*251+1;
	h=(h>>8)&0xFFFFFFFFFFFFLL;
	return(h);
}

u32 BS2C_Image_HashData(byte *buf, int sz)
{
	u32 h;
	int i;
	
	h=0;
	for(i=0; i<sz; i++)
		{ h=h*65521+buf[i]; }
	return(h);
}

char *BS2C_Image_FlagsToFlSig(BS2CC_CompileContext *ctx, s64 bmfl)
{
	char tb[256];
	char *t;
	
	t=tb;

	if(bmfl&BS2CC_TYFL_ABSTRACT)*t++='a';
	if(bmfl&BS2CC_TYFL_BIGENDIAN)*t++='b';
	if(bmfl&BS2CC_TYFL_CONST)*t++='c';	
	if(bmfl&BS2CC_TYFL_DYNAMIC)*t++='d';
	if(bmfl&BS2CC_TYFL_EXTERN)*t++='e';
	if(bmfl&BS2CC_TYFL_FINAL)*t++='f';
	if(bmfl&BS2CC_TYFL_GETTER)*t++='g';
	if(bmfl&BS2CC_TYFL_SETTER)*t++='h';
	if(bmfl&BS2CC_TYFL_INLINE)*t++='i';
	if(bmfl&BS2CC_TYFL_ASYNC)*t++='j';
	if(bmfl&BS2CC_TYFL_DELEGATE)*t++='k';
	if(bmfl&BS2CC_TYFL_LTLENDIAN)*t++='l';
	if(bmfl&BS2CC_TYFL_TRANSIENT)*t++='m';
	if(bmfl&BS2CC_TYFL_NATIVE)*t++='n';
	if(bmfl&BS2CC_TYFL_STRICT)*t++='o';
	if(bmfl&BS2CC_TYFL_PUBLIC)*t++='p';
	if(bmfl&BS2CC_TYFL_PRIVATE)*t++='q';
	if(bmfl&BS2CC_TYFL_PROTECTED)*t++='r';
	if(bmfl&BS2CC_TYFL_STATIC)*t++='s';
	if(bmfl&BS2CC_TYFL_THREAD)*t++='t';
	if(bmfl&BS2CC_TYFL_SYNCHRONIZED)*t++='u';
	if(bmfl&BS2CC_TYFL_VOLATILE)*t++='v';
	if(bmfl&BS2CC_TYFL_DLLIMPORT)*t++='w';
	if(bmfl&BS2CC_TYFL_DLLEXPORT)*t++='x';
	if(bmfl&BS2CC_TYFL_TYPEDEF)*t++='y';
	
	*t++=0;
	
	return(bgbdt_mm_rstrdup(tb));
}

int BS2C_Image_FlagsToFlSigId(BS2CC_CompileContext *ctx, s64 bmfl)
{
	char *s;
	int i;
	
	s=BS2C_Image_FlagsToFlSig(ctx, bmfl);
	if(!s || !(*s))
		return(0);

	i=BS2C_ImgLookupString(ctx, s);
	return(i);
}

byte *BS2C_Image_FlattenGlobalInfo_GblDefI(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *ct)
{
	BS2CC_CcFrame *frm;
	byte *ct1, *ct2;
	char *s;
	s64 li;
	int i, j, k;

//	if(vari->bmfl&BS2CC_TYFL_PUBLIC)
	if(vari->bmfl&BS2CC_TYFL_PUBVISIBLE)
	{
		i=BS2C_ImgLookupString(ctx, vari->name);
		if(i>0)
			{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_NAME, i); }
	}else if(vari->vitype==BS2CC_VITYPE_STRFUNC)
	{
		li=BS2C_Image_QHashName(vari->name);
		ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_NAMEH, li);
	}

	if((vari->vitype==BS2CC_VITYPE_GBLFUNC) ||
		(vari->vitype==BS2CC_VITYPE_STRFUNC) ||
		(vari->vitype==BS2CC_VITYPE_GBLVAR) ||
		(vari->vitype==BS2CC_VITYPE_STRVAR))
	{
		if(!vari->sig)
		{
			BSVM2_DBGTRAP
		}

		i=BS2C_ImgLookupString(ctx, vari->sig);
		if(i<=0)
		{
			BSVM2_DBGTRAP
		}
	}

	i=BS2C_ImgLookupString(ctx, vari->sig);
	if(i>0)
		{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_SIG, i); }
	
	i=BS2C_Image_FlagsToFlSigId(ctx, vari->bmfl);
	if(i>0)
		{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_FLAGS, i); }

	if(dtvTrueP(vari->initVal))
	{
		if(dtvIsSmallLongP(vari->initVal))
		{
			ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_VARINIT,
				dtvUnwrapLong(vari->initVal));
		}else if(dtvIsSmallDoubleP(vari->initVal))
		{
			ct=BS2C_Image_EmitTagFloat(ct, BS2CC_I1CC_VARINIT,
				dtvUnwrapDouble(vari->initVal));
//			BSVM2_DBGTRAP
		}else if(BGBDT_TagStr_IsStringP(vari->initVal))
		{
			i=BS2C_ImgLookupString(ctx,
				BGBDT_TagStr_GetUtf8(vari->initVal));
			ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_VARINIT,
				(i<<4)|1);
//			BSVM2_DBGTRAP
		}else if(dtvIsContIdxP(vari->initVal))
		{
			j=dtvUnwrapContIdx(vari->initVal);
			ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_VARINIT,
				(j<<2));
		}else
		{
			BSVM2_DBGTRAP
		}
	}

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

		if(frm->ngbl>0)
		{
			ct1=ct+256; ct2=ct1;
			for(i=0; i<frm->ngbl; i++)
			{
				ct2=BS2C_Image_EmitUVLI(ct2,
					frm->gbltab[i]);
			}

			ct=BS2C_Image_EmitTagData(ct,
				BS2CC_I1CC_GITAG, ct2-ct1, ct1);
		}

		ct=BS2C_Image_EmitTagData(ct, BS2CC_I1CC_CODE,
			frm->szt, frm->cts);
		ct2=ct-frm->szt;
//		printf("%s %08X %08X\n", vari->name,
//			BS2C_Image_HashData(frm->cts, frm->szt),
//			BS2C_Image_HashData(ct2, frm->szt));
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
	s64 li;
	int i, j, k;

//	if(vari->bmfl&BS2CC_TYFL_PUBLIC)
	if(vari->bmfl&BS2CC_TYFL_PUBVISIBLE)
	{
		i=BS2C_ImgLookupString(ctx, vari->name);
		if(i>0)
			{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_NAME, i); }
	}else
	{
		li=BS2C_Image_QHashName(vari->name);
		ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_NAMEH, li);
	}

	i=BS2C_Image_FlagsToFlSigId(ctx, vari->bmfl);
	if(i>0)
		{ ct=BS2C_Image_EmitTagSVLI(ct, BS2CC_I1CC_FLAGS, i); }

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
	
//	if(vari->bmfl&BS2CC_TYFL_PUBLIC)
	if(1)
	{
		i=BS2C_ImgLookupString(ctx, vari->qname);
		if(i>0)
			{ ct2=BS2C_Image_EmitTagSVLI(ct2, BS2CC_I1CC_QNAME, i); }

		i=BS2C_Image_FlagsToFlSigId(ctx, vari->bmfl);
		if(i>0)
			{ ct2=BS2C_Image_EmitTagSVLI(ct2, BS2CC_I1CC_FLAGS, i); }
	}

	ct3=ct2+256; ct4=ct3;
	vcur=pkg->vars;
	while(vcur)
	{
		if(vcur->bmfl&BS2CC_TYFL_PUBVISIBLE)
		{
			ct4=BS2C_Image_EmitUVLI(ct4,
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

int BS2C_Image_FlattenInitMapTagVal(
	BS2CC_CompileContext *ctx,
	dtVal val)
{
	int i, j, k, l;

	if(dtvIsContIdxP(val))
	{
		j=dtvUnwrapContIdx(val);
		return(j<<2);
	}

	if(BGBDT_TagStr_IsStringP(val))
	{
		j=BS2C_ImgGetString(ctx,
			BGBDT_TagStr_GetUtf8(val));
		return((j<<4)|1);
	}

	BSVM2_DBGTRAP
	return(0);
}

int BS2C_Image_BaseTypePackSize(int bty)
{
	int sz;
	switch(bty)
	{
	case BSVM2_OPZ_INT:			sz=4; break;
	case BSVM2_OPZ_LONG:		sz=8; break;
	case BSVM2_OPZ_FLOAT:		sz=4; break;
	case BSVM2_OPZ_DOUBLE:		sz=8; break;
	case BSVM2_OPZ_ADDRESS:		sz=4; break;
	case BSVM2_OPZ_UINT:		sz=4; break;
	case BSVM2_OPZ_UBYTE:		sz=1; break;
	case BSVM2_OPZ_SHORT:		sz=2; break;
	case BSVM2_OPZ_SBYTE:		sz=1; break;
	case BSVM2_OPZ_USHORT:		sz=2; break;
	case BSVM2_OPZ_ULONG:		sz=8; break;
	case BSVM2_OPZ_NLONG:		sz=4; break;
	case BSVM2_OPZ_UNLONG:		sz=4; break;
	case BSVM2_OPZ_X128:		sz=16; break;

	default: sz=8; break;
	}
	return(sz);
}

byte *BS2C_Image_FlattenGlobalInfo_InitArr(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari,
	byte *obuf)
{
	byte tb1[4096];
	dtVal n0, n1;
	byte *sbuf, *tbuf, *ct, *cs1, *ct1;
	u64 uli;
	int z, l, str, tcc1, tcc2;
	int i, j, k;
	
	if(!dtvTrueP(vari->initVal) ||
		!dtvIsArrayP(vari->initVal))
	{
		BSVM2_DBGTRAP
	}

	z=dtvArrayGetBaseType(vari->initVal);
	l=dtvArrayGetSize(vari->initVal);
	str=BS2C_Image_BaseTypePackSize(z);
	sbuf=dtvArrayGetIndexAddrB1(vari->initVal, 0);
		
	if(z<10)
		{ tcc1=BS2CC_IMG_TWOCC('A', '0'+z); }
	else
		{ tcc1=BS2CC_IMG_TWOCC('A', 'a'+(z-10)); }
	tcc2=BS2CC_IMG_TWOCC('A', 'g'+z);

	if((z==BSVM2_OPZ_UBYTE) || (z==BSVM2_OPZ_SBYTE))
	{
		cs1=dtvArrayGetIndexAddrB1(vari->initVal, 0);
		ct=BS2C_Image_EmitTagData(obuf, tcc1, l, sbuf);
		return(ct);
	}
	
	if((2*l*str)<=4096)
	{
		tbuf=tb1;
	}else
	{
		tbuf=bgbdt_mm_malloc(2*l*str);
	}

	if((z==BSVM2_OPZ_USHORT) || (z==BSVM2_OPZ_SHORT))
	{
		cs1=sbuf; ct1=tbuf;
		ct1=BS2C_Image_EmitUVLI(ct1, l);
		for(i=0; i<l; i++)
		{
			if(z==BSVM2_OPZ_USHORT)
				{ ct1=BS2C_Image_EmitUVLI(ct1, *(u16 *)cs1); }
			else
				{ ct1=BS2C_Image_EmitSVLI(ct1, *(s16 *)cs1); }
			cs1+=2;
		}
		k=ct1-tbuf;
		if(k<(l*str))
			{ ct=BS2C_Image_EmitTagData(obuf, tcc2, k, tbuf); return(ct); }
		cs1=sbuf; ct1=tbuf;
		for(i=0; i<l; i++)
		{
			j=*(u16 *)cs1;
			ct1[0]=j>>8;	ct1[1]=j;
			cs1+=2; ct1+=2;
		}
		ct=BS2C_Image_EmitTagData(obuf, tcc1, l, tbuf);
		return(ct);
	}

	if((z==BSVM2_OPZ_INT) || (z==BSVM2_OPZ_UINT) || (z==BSVM2_OPZ_FLOAT))
	{
		cs1=sbuf; ct1=tbuf;
		ct1=BS2C_Image_EmitUVLI(ct1, l);
		for(i=0; i<l; i++)
		{
			if(z==BSVM2_OPZ_FLOAT)
				{ ct1=BS2C_Image_EmitFVLI(ct1, *(f32 *)cs1); }
			else if(z==BSVM2_OPZ_UINT)
				{ ct1=BS2C_Image_EmitUVLI(ct1, *(u32 *)cs1); }
			else
				{ ct1=BS2C_Image_EmitSVLI(ct1, *(s32 *)cs1); }
			cs1+=4;
		}
		k=ct1-tbuf;
		if(k<(l*str))
			{ ct=BS2C_Image_EmitTagData(obuf, tcc2, k, tbuf); return(ct); }
		cs1=sbuf; ct1=tbuf;
		for(i=0; i<l; i++)
		{
			j=*(u32 *)cs1;
			ct1[0]=j>>24;	ct1[1]=j>>16;
			ct1[2]=j>>8;	ct1[3]=j;
			cs1+=4; ct1+=4;
		}
		ct=BS2C_Image_EmitTagData(obuf, tcc1, l, tbuf);
		return(ct);
	}

	if((z==BSVM2_OPZ_LONG) || (z==BSVM2_OPZ_ULONG) ||
		(z==BSVM2_OPZ_DOUBLE))
	{
		cs1=sbuf; ct1=tbuf;
		ct1=BS2C_Image_EmitUVLI(ct1, l);
		for(i=0; i<l; i++)
		{
			if(z==BSVM2_OPZ_DOUBLE)
				{ ct1=BS2C_Image_EmitFVLI(ct1, *(f64 *)cs1); }
			else if(z==BSVM2_OPZ_ULONG)
				{ ct1=BS2C_Image_EmitUVLI(ct1, *(u64 *)cs1); }
			else
				{ ct1=BS2C_Image_EmitSVLI(ct1, *(s64 *)cs1); }
			cs1+=8;
		}
		k=ct1-tbuf;
		if(k<(l*str))
			{ ct=BS2C_Image_EmitTagData(obuf, tcc2, k, tbuf); return(ct); }
		cs1=sbuf; ct1=tbuf;
		for(i=0; i<l; i++)
		{
			uli=*(u64 *)cs1;
			ct1[0]=uli>>56;	ct1[1]=uli>>48;
			ct1[0]=uli>>40;	ct1[1]=uli>>32;
			ct1[0]=uli>>24;	ct1[1]=uli>>16;
			ct1[2]=uli>> 8;	ct1[3]=uli    ;
			cs1+=8; ct1+=8;
		}
		ct=BS2C_Image_EmitTagData(obuf, tcc1, l, tbuf);
		return(ct);
	}
	
	if(z==BSVM2_OPZ_ADDRESS)
	{
		cs1=sbuf; ct1=tbuf;
		ct1=BS2C_Image_EmitUVLI(ct1, l);
		for(i=0; i<l; i++)
		{
			n0=dtvArrayGetIndexDtVal(vari->initVal, i);
			j=BS2C_Image_FlattenInitMapTagVal(ctx, n0);
			ct1=BS2C_Image_EmitUVLI(ct1, j);
		}
		k=ct1-tbuf;
		if(k<(l*str))
			{ ct=BS2C_Image_EmitTagData(obuf, tcc2, k, tbuf); return(ct); }
		cs1=sbuf; ct1=tbuf;
		for(i=0; i<l; i++)
		{
			n0=dtvArrayGetIndexDtVal(vari->initVal, i);
			j=BS2C_Image_FlattenInitMapTagVal(ctx, n0);
			ct1[0]=j>>24;	ct1[1]=j>>16;
			ct1[2]=j>>8;	ct1[3]=j;
			cs1+=4; ct1+=4;
		}
		ct=BS2C_Image_EmitTagData(obuf, tcc1, l, tbuf);
		return(ct);
	}

	BSVM2_DBGTRAP
	return(NULL);
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

	if(vari->vitype==BS2CC_VITYPE_INITARR)
	{
		ct=BS2C_Image_FlattenGlobalInfo_InitArr(ctx, vari, ct);
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
		if(!vari)
			continue;

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

BS2CC_VarInfo *bs2c_touchreach_stack[4096];
int bs2c_touchreach_stackpos=0;

BS2VM_API void BS2C_TouchReachable_TouchReachDef(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vari)
{
	BS2CC_CcFrame *frm;
	BS2CC_VarInfo *vi;
	dtVal n0, n1, n2;
	char *s;
	int z;
	int i, j, k, l;

//	if(vari->bmfl&BS2CC_TYFL_REACHDONE)
//		return;

	for(i=0; i<bs2c_touchreach_stackpos; i++)
		if(bs2c_touchreach_stack[i]==vari)
			return;
	i=bs2c_touchreach_stackpos++;
	bs2c_touchreach_stack[i]=vari;

#if 0
	if(vari->bmfl&BS2CC_TYFL_CANREACH)
	{
		if((vari->bmfl&BS2CC_TYFL_PUBLIC) &&
			(vari->bmfl&BS2CC_TYFL_PUBVISIBLE))
		{
			BS2C_ImgGetString(ctx, vari->name);
		}

		return;
	}
#endif

//	vari->bmfl|=BS2CC_TYFL_PUBVISIBLE;	//DEBUG

	if(vari->vitype==BS2CC_VITYPE_GBLFUNC)
	{
		if(!vari->body)
		{
			vari->bmfl|=BS2CC_TYFL_PUBVISIBLE;
			BS2C_ImgGetString(ctx, vari->name);
		}
	}

	if((vari->bmfl&BS2CC_TYFL_PUBLIC) &&
		(vari->bmfl&BS2CC_TYFL_PUBVISIBLE))
	{
		BS2C_ImgGetString(ctx, vari->name);
	}

	vari->bmfl|=BS2CC_TYFL_CANREACH;
	BS2C_ImgGetString(ctx, vari->sig);
	
	s=BS2C_Image_FlagsToFlSig(ctx, vari->bmfl);
	if(s && *s)BS2C_ImgGetString(ctx, s);
	
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


	if(dtvTrueP(vari->initVal))
	{
		if(BGBDT_TagStr_IsStringP(vari->initVal))
		{
			BS2C_ImgGetString(ctx,
				BGBDT_TagStr_GetUtf8(vari->initVal));
		}
		
		if(dtvIsArrayP(vari->initVal))
		{
			z=dtvArrayGetBaseType(vari->initVal);
			l=dtvArrayGetSize(vari->initVal);
			
			if(z==BSVM2_OPZ_ADDRESS)
			{
				for(i=0; i<l; i++)
				{
					n0=dtvArrayGetIndexDtVal(vari->initVal, i);
					
					if(BGBDT_TagStr_IsStringP(n0))
					{
						BS2C_ImgGetString(ctx,
							BGBDT_TagStr_GetUtf8(n0));
					}
					
					if(dtvIsContIdxP(n0))
					{
						j=dtvUnwrapContIdx(n0);
						BS2C_TouchReachable_TouchReachDef(ctx,
							ctx->globals[j]);
					}
				}
			}
		}
		
		if(dtvIsContIdxP(vari->initVal))
		{
			j=dtvUnwrapContIdx(vari->initVal);
			BS2C_TouchReachable_TouchReachDef(ctx,
				ctx->globals[j]);
		}

	}

//	if(vari->initGid>0)
//	{
//		BS2C_TouchReachable_TouchReachDef(ctx,
//			ctx->globals[vari->initGid]);
//	}

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

	vari->bmfl|=BS2CC_TYFL_REACHDONE;
	bs2c_touchreach_stackpos--;
}

BS2VM_API void BS2C_TouchReachable_TouchPublicDef(
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

BS2VM_API int BS2C_TouchReachable(
	BS2CC_CompileContext *ctx)
{
	BS2CC_PkgFrame *pcur;
	BS2CC_VarInfo *vcur;
	char *s;
	int i;
	
	pcur=ctx->pkg_first;
	while(pcur)
	{
		vcur=ctx->globals[pcur->gid];
		vcur->bmfl|=BS2CC_TYFL_CANREACH;

		BS2C_ImgGetString(ctx, vcur->qname);

		s=BS2C_Image_FlagsToFlSig(ctx, vcur->bmfl);
		if(s && *s)BS2C_ImgGetString(ctx, s);

		vcur=pcur->vars;
		while(vcur)
		{
			BS2C_TouchReachable_TouchPublicDef(ctx, vcur);
			vcur=vcur->pknext;
		}
		pcur=pcur->next;
	}

	for(i=0; i<ctx->nglobals; i++)
	{
		vcur=ctx->globals[i];
		if(!vcur)
			continue;

		if(vcur->vitype!=BS2CC_VITYPE_GBLFUNC)
			continue;
		
		if(!vcur->name)
			continue;
		if(!strcmp(vcur->name, "main"))
		{
			BS2C_TouchReachable_TouchReachDef(ctx, vcur);
//			vitab[n++]=vari;
		}
	}

	return(0);
}


int BS2C_Image_CompareMains(
	BS2CC_CompileContext *ctx,
	BS2CC_VarInfo *vi1,
	BS2CC_VarInfo *vi2)
{
	int i;

	if(vi1==vi2)
		return(0);
	
	/* Assume only 1 global main, and it always goes first */
	if(!vi1->pkg && !vi1->obj)
		return(-1);
	if(!vi2->pkg && !vi2->obj)
		return(1);
	
	if(vi1->pkg && vi2->pkg)
	{
		/* if 2 package-level mains, sort by package */
		if(!vi1->obj && !vi2->obj)
			return(strcmp(vi1->pkg->qname, vi2->pkg->qname));
	}

	/* package scope precedes object scope */
	if(!vi1->obj)
		return(-1);
	if(!vi2->obj)
		return(1);
	return(strcmp(vi1->obj->qname, vi2->obj->qname));
}

byte *BS2C_Image_FlattenMains(
	BS2CC_CompileContext *ctx, byte *obuf)
{
	BS2CC_VarInfo *vitab[256];
	BS2CC_VarInfo *vari;
	byte *ct, *ct1, *ct2;
	int n;
	int i, j, k;

	ct1=obuf+256; ct2=ct1;

//	ct2=BS2C_Image_EmitTagData(ct2, BS2CC_I1CC_NULL, 0, NULL);

	n=0;
	for(i=0; i<ctx->nglobals; i++)
	{
		vari=ctx->globals[i];
		if(!vari)
			continue;

		if(vari->vitype!=BS2CC_VITYPE_GBLFUNC)
			continue;
		
		if(!vari->name)
			continue;
		if(!strcmp(vari->name, "main"))
			{ vitab[n++]=vari; }
	}
	
	for(i=0; i<n; i++)
		for(j=i+1; j<n; j++)
	{
		if(BS2C_Image_CompareMains(ctx, vitab[j], vitab[i])<0)
		{
			vari=vitab[j];
			vitab[j]=vitab[i];
			vitab[i]=vari;
		}
	}

	if(n<=0)
		return(obuf);

	if(n==1)
	{
		vari=vitab[0];
		i=vari->gid;
		ct=BS2C_Image_EmitTagSVLI(obuf, BS2CC_I1CC_MAIN, i);
		return(ct);
	}

	for(i=0; i<n; i++)
	{
		vari=vitab[i];
		ct2=BS2C_Image_EmitUVLI(ct2, vari->gid);
	}

	ct=BS2C_Image_EmitTagData(obuf, BS2CC_IFCC_MAIN, ct2-ct1, ct1);
	return(ct);
}


byte *BS2C_Image_FlattenPvbm(
	BS2CC_CompileContext *ctx, byte *obuf)
{
	byte tb[4096];
	byte tb2[4096];
	BS2CC_VarInfo *vitab[256];
	BS2CC_VarInfo *vari;
	byte *cs, *cs1, *cse;
	byte *ct, *ct1, *ct2;
	int n, o;
	int i, j, k, l;

	ct1=obuf+256; ct2=ct1;

	memset(tb, 0, 4096);

	n=0;
	for(i=0; i<ctx->nglobals; i++)
	{
		vari=ctx->globals[i];
		if(!vari)
			continue;

		if(	(vari->vitype!=BS2CC_VITYPE_GBLFUNC) &&
			(vari->vitype!=BS2CC_VITYPE_GBLVAR) &&
			(vari->vitype!=BS2CC_VITYPE_CLASS) &&
			(vari->vitype!=BS2CC_VITYPE_STRUCT) &&
			(vari->vitype!=BS2CC_VITYPE_IFACE))
				continue;
		if(!vari->name)
			continue;
		if(!(vari->bmfl&BS2CC_TYFL_PUBVISIBLE))
			continue;

		n=i+1;
		tb[i>>3]|=(128>>(i&7));
	}
	
	l=(n+7)>>3;
	cs=tb; cse=tb+l;
	ct1=tb2; ct2=ct1;
	while(cs<cse)
	{
		if((*cs)==0x00)
		{
			cs1=cs+1;
			while((cs1<cse) && ((*cs1)==0x00))
				cs1++;
			i=cs1-cs;
			if(i>32)i=32;
			*ct2++=0x00|(i-1);
			cs+=i;
			continue;
		}

		if((*cs)==0xFF)
		{
			cs1=cs+1;
			while((cs1<cse) && ((*cs1)==0xFF))
				cs1++;
			i=cs1-cs;
			if(i>32)i=32;
			*ct2++=0x20|(i-1);
			cs+=i;
			continue;
		}

		cs1=cs+1;
		while((cs1<cse) &&
			((*cs1)!=0x00) && ((*cs1)!=0xFF))
				cs1++;
		i=cs1-cs;
		if(i==1)
		{
			j=*cs;
			if(j==(j&31))
				{ *ct2++=0x80|(j&31); cs++; continue; }
			if(j==(0xE0|(j&31)))
				{ *ct2++=0xA0|(j&31); cs++; continue; }
			if(j==(j&0xF8))
				{ *ct2++=0xC0|(j>>3); cs++; continue; }
			if(j==((j&0xF8)|0x07))
				{ *ct2++=0xE0|(j>>3); cs++; continue; }
		}
		if(i>32)i=32;
		*ct2++=0x40|(i-1);
		cs1=cs+i;
		while(cs<cs1)
			*ct2++=*cs++;
	}

//	ct=BS2C_Image_EmitTagData(obuf, BS2CC_IFCC_PVBM, ct2-ct1, ct1);
	ct=BS2C_Image_EmitTagData(obuf, BS2CC_ITCC_PV, ct2-ct1, ct1);
	return(ct);
}


BS2VM_API int BS2C_FlattenImage(
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
	ct=BS2C_Image_FlattenMains(ctx, ct);
	ct=BS2C_Image_FlattenPvbm(ctx, ct);

	ct=BS2C_Image_EmitTagData(obuf, BS2CC_IFCC_BS2I,
		ct-ct1, ct1);
	return(ct-obuf);
}
