#include <bteifgl.h>

byte *BS2I_ReadTag(byte *cs, u64 *rtag, s64 *rlen)
{
	u64 li;
	u32 uj, uk;
	int i, j, k;

	i=*cs++;

	if(i==0x00)
	{
		*rtag=0;
		*rlen=0;
		return(cs);
	}

	if((i>=0x01) && (i<=0x1A))
	{
		*rtag='A'+(i-0x01);
		*rlen=BS2CC_ILEN_SVLI;
		return(cs);
	}
	if((i>=0x1B) && (i<=0x1F))
	{
		*rtag='0'+(i-0x1B);
		*rlen=BS2CC_ILEN_SVLI;
		return(cs);
	}

	if((i>=0x21) && (i<=0x3A))
	{
		*rtag='a'+(i-0x21);
		*rlen=BS2CC_ILEN_SVLI;
		return(cs);
	}
	if((i>=0x3B) && (i<=0x3F))
	{
		*rtag='5'+(i-0x3B);
		*rlen=BS2CC_ILEN_SVLI;
		return(cs);
	}

	if(i==0x40)
	{
		j=*cs++;
		j=(j<<8)|(*cs++);
		*rtag=*cs++;
		*rlen=j-4;
		return(cs);
	}

	if(i==0x41)
	{
		j=*cs++;
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		*rtag=*cs++;
		*rlen=j-6;
		return(cs);
	}

	if((i>=0x42) && (i<=0x5F))
	{
		*rtag=*cs++;
		*rlen=i-0x42;
		return(cs);
	}

	if((i>=0x60) && (i<=0x7F))
	{
		j=i-0x60;
		j=(j<<8)|(*cs++);
		*rtag=*cs++;
		*rlen=j;
		return(cs);
	}

	if((i>=0x80) && (i<=0x8F))
	{
		j=i-0x80;
		j=(j<<8)|(*cs++);
//		k=cs[0]|(cs[1]<<8); cs+=2;
		k=(cs[0]<<8)|cs[1]; cs+=2;
		*rtag=k;
		*rlen=j;
		return(cs);
	}

	if((i>=0x90) && (i<=0x9F))
	{
		j=i-0x90;
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
//		k=cs[0]|(cs[1]<<8); cs+=2;
		k=(cs[0]<<8)|cs[1]; cs+=2;
		*rtag=k;
		*rlen=j;
		return(cs);
	}

	if((i>=0xA0) && (i<=0xAF))
	{
		j=i-0xA0;
		j=(j<<8)|(*cs++);
//		k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24); cs+=4;
		k=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		*rtag=((u32)k);
		*rlen=j;
		return(cs);
	}

	if((i>=0xB0) && (i<=0xBF))
	{
		j=i-0xB0;
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
//		k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24); cs+=4;
		k=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		*rtag=((u32)k);
		*rlen=j;
		return(cs);
	}

	if((i>=0xC0) && (i<=0xC3))
	{
		j=i-0xC0;
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
//		k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24); cs+=4;

		uj=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		uk=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		*rtag=(uj<<32)|uk;
		*rlen=j;
		return(cs);
	}

	if((i>=0xC4) && (i<=0xC7))
	{
		uj=i-0xC4;
		uj=(uj<<8)|(*cs++);
		uj=(uj<<8)|(*cs++);
		uj=(uj<<8)|(*cs++);
		uk=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		li=(uj<<32)|uk;

//		k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24); cs+=4;

		uj=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		uk=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		*rtag=(uj<<32)|uk;
		*rlen=li;
		return(cs);
	}
}

byte *BS2I_ReadTag2(byte *cs, u32 *rtag, int *rlen)
{
	u64 ttag;
	s64 tlen;

	cs=BS2I_ReadTag2(cs, &ttag, &tlen);
	*rtag=ttag;
	*rlen=tlen;
	return(cs);
}

byte *BS2I_ReadVLI_I(byte *cs, u64 *rv, int *rl)
{
	u64 li;
	int i, j;
	
	i=*cs++;

	if(i<0x80)
	{
		*rv=i;
		*rl=0;
		return(cs);
	}

	if(i<0xC0)
	{
		i=i&0x3F;
		i=(i<<8)|(*cs++);
		*rv=i;
		*rl=1;
		return(cs);
	}

	if(i<0xE0)
	{
		i=i&0x1F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rv=i;
		*rl=2;
		return(cs);
	}

	if(i<0xF0)
	{
		i=i&0x0F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rv=i;
		*rl=3;
		return(cs);
	}

	if(i<0xF8)
	{
		i=i&0x07;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

//		i=(i<<8)|(*cs++);
		j=*cs++;
		*rv=(((u64)i)<<8)|j;
		*rl=4;
		return(cs);
	}

	if(i<0xFC)
	{
		i=i&0x03;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

		j=*cs++;
		j=(j<<8)|(*cs++);
		*rv=(((u64)i)<<16)|j;
		*rl=5;
		return(cs);
	}

	if(i<0xFE)
	{
		i=i&0x01;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

		j=*cs++;
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		*rv=(((u64)i)<<24)|j;
		*rl=6;
		return(cs);
	}

	if(i==0xFE)
	{
		i=*cs++;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

		j=*cs++;
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		*rv=(((u64)((u32)i))<<24)|j;
		*rl=7;
		return(cs);
	}

	i=*cs++;
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);

	j=*cs++;
	j=(j<<8)|(*cs++);
	j=(j<<8)|(*cs++);
	j=(j<<8)|(*cs++);
	*rv=(((u64)((u32)i))<<32)|((u32)j);
	*rl=8;
	return(cs);
}

byte *BS2I_ReadUVLI(byte *cs, s64 *rv)
{
	u64 uli;
	int i, l;
	
	cs=BS2I_ReadVLI_I(cs, &uli, &l);
	*rv=uli;
	
	return(cs);
}

byte *BS2I_ReadSVLI(byte *cs, s64 *rv)
{
	u64 uli;
	s64 li;
	int i, l;
	
	cs=BS2I_ReadVLI_I(cs, &uli, &l);
	li=(uli>>1)^(((s64)(uli<<63))>>63);
	*rv=li;
	
	return(cs);
}

BSVM2_CodeImage *BS2I_AllocImage(void)
{
	BSVM2_CodeImage *tmp;
	
	tmp=dtmAlloc("bsvm2_codeimage_s", sizeof(BSVM2_CodeImage));
	return(tmp);
}

BSVM2_ImageGlobal *BS2I_AllocImageGlobal(BSVM2_CodeImage *img)
{
	BSVM2_ImageGlobal *tmp;
	
	tmp=dtmAlloc("bsvm2_imageglobal_s", sizeof(BSVM2_ImageGlobal));
	return(tmp);
}

BSVM2_CodeBlock *BS2I_AllocImageCodeBlock(BSVM2_CodeImage *img)
{
	BSVM2_CodeBlock *tmp;
	
	tmp=dtmAlloc("bsvm2_codeblock_s", sizeof(BSVM2_CodeBlock));
	return(tmp);
}

int BS2I_ImageTagCharMustUnderstand2P(
	BSVM2_CodeImage *img, byte tagh)
{
	if((tagh>=0x35) && (tagh<=0x39))
		return(0);
	if((tagh>=0x60) && (tagh<=0x7F))
		return(0);
	if((tagh>=0x80) && (tagh<=0xBF))
		return(0);

	return(1);
}

int BS2I_ImageTagCharMustUnderstandP(
	BSVM2_CodeImage *img, byte tagh)
{
	if((tagh>=0x60) && (tagh<=0x7F))
		return(0);
	if((tagh>=0x80) && (tagh<=0xBF))
		return(0);

	return(1);
}

int BS2I_ImageTagMustUnderstandP(BSVM2_CodeImage *img, u64 tag)
{
	if(tag<0x100)
		{ return(BS2I_ImageTagCharMustUnderstand2P(img, tag)); }
	if(tag<0x10000)
		{ return(BS2I_ImageTagCharMustUnderstandP(img, tag>>8)); }
	if(tag<0x100000000ULL)
		{ return(BS2I_ImageTagCharMustUnderstandP(img, tag>>24)); }
	return(BS2I_ImageTagCharMustUnderstandP(img, tag>>56));
}

void BS2I_ImageErrorCorruptTag(BSVM2_CodeImage *img)
{
}

void BS2I_ImageCheckUnknownTag(BSVM2_CodeImage *img, u64 tag)
{
}

int BS2I_ImageGetGlobalOfs(BSVM2_CodeImage *img, int gix)
{
	int i;
	switch(img->gixstr)
	{
	case 2: 
		i=(img->gixtab[gix*2+0]<<8)|img->gixtab[gix*2+1];
		break;
	case 3: 
		i=(img->gixtab[gix*3+0]<<16)|
			(img->gixtab[gix*3+1]<<8)|
			img->gixtab[gix*3+2];
		break;
	case 4: 
		i=	(img->gixtab[gix*4+0]<<24)|
			(img->gixtab[gix*4+1]<<16)|
			(img->gixtab[gix*4+2]<< 8)|
			(img->gixtab[gix*4+3]    );
		break;
	default:
		i=0;
		break;
	}
	return(i);
}

byte *BS2I_ImageGetGlobalLump(BSVM2_CodeImage *img, int gix)
{
	byte *ptr;
	int gofs;
	
	gofs=BS2I_ImageGetGlobalOfs(img, gix);
	if(gofs<=0)
		return(NULL);

	ptr=img->gblsdat+gofs;
	if(ptr>=img->egblsdat)
		return(NULL);
	return(ptr);
}

int BS2I_ImageDecodeGlobalFunc(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl,
	u32 tag, byte *data, byte *edata)
{
	int tgitab[256];
	byte *cs, cs1, *cse, *cse, *csn;
	s64 v;
	u32 tag;
	int len, ngi;
	int i, j, k;

	cs=data; cse=edata;
	while(cs<cse)
	{
		cs=BS2I_ReadTag2(cs, &tag, &len);
		csn=cs+len;
		
		if(len==BS2CC_ILEN_SVLI)
		{
			cs=BS2I_ReadSVLI(cs, &v);
			
			
			if(tag==BS2CC_I1CC_NAME)
			{
				gbl->name=img->strtab+v;
				continue;
			}

			if(tag==BS2CC_I1CC_QNAME)
			{
				gbl->qname=img->strtab+v;
				continue;
			}
			
			if(tag==BS2CC_I1CC_SIG)
			{
				gbl->sig=img->strtab+v;
				continue;
			}
			
			if(tag==BS2CC_I1CC_FLAGS)
			{
				gbl->flagstr=img->strtab+v;
				continue;
			}

			if(tag==BS2CC_I1CC_PACKAGE)
			{
				gbl->pkg=BS2I_ImageGetGlobal(img, v);
				continue;
			}

			if(tag==BS2CC_I1CC_ECLASS)
			{
				gbl->obj=BS2I_ImageGetGlobal(img, v);
				continue;
			}
			
			if(tag==BS2CC_I1CC_NARGS)
			{
				gbl->nargs=v;
				continue;
			}

			if(tag==BS2CC_I1CC_BARGS)
			{
				if(!gbl->cblk)
					gbl->cblk=BS2I_AllocImageCodeBlock(img);
				gbl->cblk->bargs=v;
				continue;
			}

			if(tag==BS2CC_I1CC_STKDEPTH)
			{
				if(!gbl->cblk)
					gbl->cblk=BS2I_AllocImageCodeBlock(img);
				gbl->cblk->stkdepth=v;
				continue;
			}

			BS2I_ImageCheckUnknownTag(img, tag);
			continue;
		}
		
		if((csn<cs) || (csn>cse))
		{
			BS2I_ImageErrorCorruptTag(img);
			return(-1);
		}
		
		if((tag==BS2CC_I1CC_CODE) ||
			(tag==BS2CC_ITCC_BC))
		{
			if(!gbl->cblk)
				gbl->cblk=BS2I_AllocImageCodeBlock(img);
			gbl->cblk->code=cs;
			gbl->cblk->szcode=csn-cs;
			cs=csn;
			continue;
		}

		if(tag==BS2CC_I1CC_GITAG)
		{
			if(!gbl->cblk)
				gbl->cblk=BS2I_AllocImageCodeBlock(img);

			cs1=cs; ngi=0;
			while(cs1<csn)
			{
				cs1=BS2I_ReadUVLI(cs1, &v);
				tgitab[ngi++]=v;
			}

			if(ngi>8)
			{
				gbl->cblk->gitab=frgl_malloc(ngi*sizeof(dtVal));
				gbl->cblk->ngi=ngi;
			}else
			{
				gbl->cblk->gitab=gbl->cblk->tgitab;
				gbl->cblk->ngi=ngi;
			}
			
			for(i=0; i<ngi; i++)
				{ gbl->cblk->gitab[i]=tgitab[i]; }
			
			cs=csn;
			continue;
		}
			
		BS2I_ImageCheckUnknownTag(img, tag);
		cs=csn;
	}
	return(0);
}

int BS2I_ImageDecodeGlobalVar(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl,
	u32 tag, byte *data, byte *edata)
{
	byte *cs, cs1, *cse, *cse, *csn;
	s64 v;
	u32 tag;
	int len;

	cs=data; cse=edata;
	while(cs<cse)
	{
		cs=BS2I_ReadTag2(cs, &tag, &len);
		csn=cs+len;
		
		if(len==BS2CC_ILEN_SVLI)
		{
			cs=BS2I_ReadSVLI(cs, &v);
			
			
			if(tag==BS2CC_I1CC_NAME)
			{
				gbl->name=img->strtab+v;
				continue;
			}

			if(tag==BS2CC_I1CC_QNAME)
			{
				gbl->qname=img->strtab+v;
				continue;
			}
			
			if(tag==BS2CC_I1CC_SIG)
			{
				gbl->sig=img->strtab+v;
				continue;
			}
			
			if(tag==BS2CC_I1CC_FLAGS)
			{
				gbl->flagstr=img->strtab+v;
				continue;
			}

			if(tag==BS2CC_I1CC_PACKAGE)
			{
				gbl->pkg=BS2I_ImageGetGlobal(img, v);
				continue;
			}

			if(tag==BS2CC_I1CC_ECLASS)
			{
				gbl->obj=BS2I_ImageGetGlobal(img, v);
				continue;
			}

			BS2I_ImageCheckUnknownTag(img, tag);
			continue;
		}
		
		if((csn<cs) || (csn>cse))
		{
			BS2I_ImageErrorCorruptTag(img);
			return(-1);
		}
		
		BS2I_ImageCheckUnknownTag(img, tag);
		cs=csn;
	}
	return(0);
}

int BS2I_ImageDecodeGlobal(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl, byte *gdat)
{
	byte *cs, cs1, *cse, *cse, *csn;
	u32 tag;
	int len;

	cs=gdat;
	cs=BS2I_ReadTag2(cs, &tag, &len);
	cse=cs+len;

	if((cse<cs) || (cse>img->egblsdat))
	{
		BS2I_ImageErrorCorruptTag(img);
		return(-1);
	}

	if(tag==BS2CC_ITCC_GF)
		{ return(BS2I_ImageDecodeGlobalFunc(img, gbl, tag, cs, cse)); }
	if(tag==BS2CC_ITCC_SF)
		{ return(BS2I_ImageDecodeGlobalFunc(img, gbl, tag, cs, cse)); }

	if(tag==BS2CC_ITCC_GV)
		{ return(BS2I_ImageDecodeGlobalVar(img, gbl, tag, cs, cse)); }
	if(tag==BS2CC_ITCC_SV)
		{ return(BS2I_ImageDecodeGlobalVar(img, gbl, tag, cs, cse)); }

	BS2I_ImageCheckUnknownTag(img, tag);
	return(-1);
}

BSVM2_ImageGlobal *BS2I_ImageGetGlobal(BSVM2_CodeImage *img, int gix)
{
	BSVM2_ImageGlobal *gbl;
	byte *gdat;
	int gofs;
	
	if(gix<=0)
		return(NULL);
	if(gix>=img->ngbls)
		return(NULL);
	
	gbl=img->gbls[gix];
	if(gbl)
		return(gbl);

	gdat=BS2I_ImageGetGlobalLump(img, gix);
	if(!gdat)
		return(NULL);

	gbl=BS2I_AllocImageGlobal(img);
	gbl->gix=gix;
	img->gbls[gix]=gbl;
	
//	gofs=BS2I_ImageGetGlobalOfs(img, gix);
	BS2I_ImageDecodeGlobal(img, gbl, gdat);
	
	return(gbl);
}

BSVM2_CodeImage *BS2I_DecodeImageBuffer(byte *ibuf, int isz)
{
	BSVM2_CodeImage *img;
	u64 tag;
	s64 tlen;
	byte *cs, cs1, *cse, *cse, *csn;

	cs=ibuf; cse=ibuf+isz;
	cs=BS2I_ReadTag(cs, &tag, &len);
	cs1=cs+len;
	
	if((tag!=BS2CC_IFCC_BS2I) && (tag!=BS2CC_IFCC_BS2L) &&
		(tag!=BS2CC_IECC_BS2I0EXE) && (tag!=BS2CC_IECC_BS2I0DLL))
	{
		return(NULL);
	}
	
	if((cs1<cs) || (cs1>cse))
		return(NULL);


	img=BS2I_AllocImage();
	
	img->data=frgl_malloc(isz);
	img->szdata=isz;
	memcpy(img->data, ibuf, isz);
	
	cs=img->data;
	cs=BS2I_ReadTag(cs, &tag, &len);
	cse=cs+len;
	
	while(cs<cse)
	{
		cs=BS2I_ReadTag(cs, &tag, &len);
		csn=cs+len;
		if(tag==BS2CC_IFCC_STRS)
		{
			img->strtab=cs;
			img->estrtab=csn;
			cs=csn;
			continue;
		}

		if(tag==BS2CC_IFCC_GBLS)
		{
			img->gblsdat=cs;
			img->egblsdat=csn;
			cs=csn;
			continue;
		}

		if(tag==BS2CC_IFCC_GIX2)
		{
			img->gixtab=cs;
			img->gixstr=2;
			img->ngbls=(len/2)+1;
			cs=csn;
			continue;
		}

		if(tag==BS2CC_IFCC_GIX3)
		{
			img->gixtab=cs;
			img->gixstr=3;
			img->ngbls=(len/3)+1;
			cs=csn;
			continue;
		}

		if(tag==BS2CC_IFCC_GIX4)
		{
			img->gixtab=cs;
			img->gixstr=4;
			img->ngbls=(len/4)+1;
			cs=csn;
			continue;
		}

		BS2I_ImageCheckUnknownTag(img, tag);
		cs=csn;
		continue;
	}

	img->gbls=frgl_malloc(img->ngbls*sizeof(BSVM2_ImageGlobal *));
	
	return(img);
}
