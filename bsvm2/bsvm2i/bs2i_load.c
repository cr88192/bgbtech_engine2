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

/** \file
 * BS2 Image Loader
 *
 */

int bs2i_img_seqid=1;
BSVM2_CodeImage *bs2i_liveimg=NULL;
int bs2i_img_dynvseqid=1;
int bs2i_img_dynvlim=512;

/** Read TLV Tag from buffer */
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
		*rlen=j-3;
		return(cs);
	}

	if((i>=0x80) && (i<=0x8F))
	{
		j=i-0x80;
		j=(j<<8)|(*cs++);
//		k=cs[0]|(cs[1]<<8); cs+=2;
		k=(cs[0]<<8)|cs[1]; cs+=2;
		*rtag=k;
		*rlen=j-4;
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
		*rlen=j-6;
		return(cs);
	}

	if((i>=0xA0) && (i<=0xAF))
	{
		j=i-0xA0;
		j=(j<<8)|(*cs++);
//		k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24); cs+=4;
		k=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		*rtag=((u32)k);
		*rlen=j-6;
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
		*rlen=j-8;
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
		*rtag=(((u64)uj)<<32)|uk;
		*rlen=j-12;
		return(cs);
	}

	if((i>=0xC4) && (i<=0xC7))
	{
		uj=i-0xC4;
		uj=(uj<<8)|(*cs++);
		uj=(uj<<8)|(*cs++);
		uj=(uj<<8)|(*cs++);
		uk=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		li=(((u64)uj)<<32)|uk;

//		k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24); cs+=4;

		uj=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		uk=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3]; cs+=4;
		*rtag=(((u64)uj)<<32)|uk;
		*rlen=li-16;
		return(cs);
	}
	
	return(NULL);
}

/** Read TLV Tag from buffer, limited to 4GB FOURCC */
byte *BS2I_ReadTag2(byte *cs, u32 *rtag, int *rlen)
{
	u64 ttag;
	s64 tlen;

	cs=BS2I_ReadTag(cs, &ttag, &tlen);
	*rtag=ttag;
	*rlen=tlen;
	return(cs);
}

/** Read VLI from buffer */
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

/** Read UVLI from buffer */
byte *BS2I_ReadUVLI(byte *cs, s64 *rv)
{
	u64 uli;
	int i, l;
	
	cs=BS2I_ReadVLI_I(cs, &uli, &l);
	*rv=uli;
	
	return(cs);
}

/** Read SVLI from buffer */
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

/** Read FVLI from buffer */
byte *BS2I_ReadFVLI(byte *cs, double *rv)
{
	double f;
	u32 vi;
	u64 lj;
	s64 li;
	int i, j, k, e, sg;

	cs=BS2I_ReadSVLI(cs, &li);
	e=li;
	cs=BS2I_ReadSVLI(cs, &li);

	sg=0;
	if(li<0)
		{ li=-li; sg=1; }

	while(!(li>>32))
		{ li=(li<<16); e-=16; }
	while(!(li>>44))
		{ li=(li<<8); e-=8; }
	while(!(li>>52))
		{ li=(li<<1); e--; }

	e=e+1023;
	lj=(li&0x000FFFFFFFFFFFFFULL)|(((u64)e)<<52)|(((u64)sg)<<63);
	f=*(double *)(&lj);

	*rv=f;
	return(cs);
}

/** Decode opcode VLI from buffer */
byte *BS2I_ReadOpcodeNumber(byte *cs, int *rv)
{
	int i;

	i=*cs++;
	if(i<0xE0)
	{
		*rv=i;
		return(cs);
	}

	if((i>=0xE0) && (i<=0xEF))
	{
		i=((i-0xE0)<<8)|(*cs++);
	}else if((i>=0xF0) && (i<=0xF7))
	{
		i=i-0xF0;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
	}else if((i>=0xF8) && (i<=0xFB))
	{
		i=i-0xF8;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
	}
	
	BSVM2_DBGTRAP
	*rv=i;
	return(cs);
}

/** Allocate image */
BSVM2_CodeImage *BS2I_AllocImage(void)
{
	BSVM2_CodeImage *tmp;
	
	tmp=dtmAlloc("bsvm2_codeimage_s", sizeof(BSVM2_CodeImage));
	return(tmp);
}

/** Allocate global */
BSVM2_ImageGlobal *BS2I_AllocImageGlobal(BSVM2_CodeImage *img)
{
	BSVM2_ImageGlobal *tmp;
	
	tmp=dtmAlloc("bsvm2_imageglobal_s", sizeof(BSVM2_ImageGlobal));
	tmp->img=img;

	return(tmp);
}

/** Allocate code block */
BSVM2_CodeBlock *BS2I_AllocImageCodeBlock(BSVM2_CodeImage *img)
{
	BSVM2_CodeBlock *tmp;
	
	tmp=dtmAlloc("bsvm2_codeblock_s", sizeof(BSVM2_CodeBlock));
	tmp->img=img;

	return(tmp);
}

/** Check if the initial tag character is 'must understand' (ONECC) */
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

/** Check if the initial tag character is 'must understand' */
int BS2I_ImageTagCharMustUnderstandP(
	BSVM2_CodeImage *img, byte tagh)
{
	if((tagh>=0x60) && (tagh<=0x7F))
		return(0);
	if((tagh>=0x80) && (tagh<=0xBF))
		return(0);

	return(1);
}

/** Check if a lump tag represents a "Must Understand" value. */
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
	BSVM2_DBGTRAP
}

void BS2I_ImageCheckUnknownTag(BSVM2_CodeImage *img, u64 tag)
{
	BSVM2_DBGTRAP
}

/** Get an offset to the lump for a given global index. */
int BS2I_ImageGetGlobalOfs(BSVM2_CodeImage *img, int gix)
{
	int i, j;
	switch(img->gixstr)
	{
	case 2:
		j=(gix-1)*2;
		i=(img->gixtab[j+0]<<8)|img->gixtab[j+1];
		break;
	case 3: 
		j=(gix-1)*3;
		i=(img->gixtab[j+0]<<16)|
			(img->gixtab[j+1]<<8)|
			img->gixtab[j+2];
		break;
	case 4: 
		j=(gix-1)*4;
		i=	(img->gixtab[j+0]<<24)|
			(img->gixtab[j+1]<<16)|
			(img->gixtab[j+2]<< 8)|
			(img->gixtab[j+3]    );
		break;
	default:
		BSVM2_DBGTRAP
		i=0;
		break;
	}
	return(i);
}

/** Get a pointer to the lump for a given global index. */
byte *BS2I_ImageGetGlobalLump(BSVM2_CodeImage *img, int gix)
{
	byte *ptr;
	int gofs;
	
	gofs=BS2I_ImageGetGlobalOfs(img, gix);
	if(gofs<=0)
	{
		if(gofs<0)
			BSVM2_DBGTRAP
		return(NULL);
	}

	ptr=img->gblsdat+gofs;
	if(ptr>=img->egblsdat)
	{
		BSVM2_DBGTRAP
		return(NULL);
	}
	return(ptr);
}

/** Decode the information about a given global (Function or Method). */
int BS2I_ImageDecodeGlobalFunc(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl,
	u32 dtag, byte *data, byte *edata)
{
	int tgitab[256];
	byte *cs, *cs0, *cs1, *cse, *csn;
	char *s;
	s64 v;
	u32 tag;
	int len, ngi;
	int i, j, k;

	cs=data; cse=edata;
	while(cs<cse)
	{
		cs0=cs;
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
			
			if(tag==BS2CC_I1CC_NAMEH)
			{
				gbl->nameh=v;
				continue;
			}

			if(tag==BS2CC_I1CC_QNAMEH)
			{
				gbl->qnameh=v;
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
				gbl->cblk->gitab=dtmMalloc(ngi*sizeof(int));
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

		if(tag==BS2CC_I1CC_IMPL)
		{
			if(!gbl->cblk)
				gbl->cblk=BS2I_AllocImageCodeBlock(img);

			cs1=cs; ngi=0;
			while(cs1<csn)
			{
//				cs1=BS2I_ReadUVLI(cs1, &v);
				cs1=BS2I_ReadOpcodeNumber(cs1, &i);
				tgitab[ngi++]=i;
			}

			gbl->cblk->altab=dtmMalloc(ngi*sizeof(int));
//			gbl->nifgix=ngi;
			for(i=0; i<ngi; i++)
				{ gbl->cblk->altab[i]=tgitab[i]; }

			cs=csn;
			continue;
		}
	
		BS2I_ImageCheckUnknownTag(img, tag);
		cs=csn;
	}

	s=BSVM2_NatCall_SigGetRet(gbl->sig);
	gbl->brty=BSVM2_NatCall_GetSigOpZ(s);

	gbl->gvalue=(BSVM2_Value *)(gbl->baty+24);
	gbl->gvalue->a=dtvWrapPtr(gbl);

	if(gbl->cblk)
	{
		gbl->cblk->func=gbl;
		gbl->cblk->largs=gbl->cblk->bargs+gbl->nargs;
		gbl->cblk->szframe=gbl->cblk->stkdepth+gbl->cblk->largs;
	}

	return(0);
}

/** Try to get the QName for a given global. */
char *BS2I_ImageTryGetGlobalQName(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl)
{
	char tb[256];
	char *s0, *s1;

	if(gbl->qname)
		return(gbl->qname);
	if(!gbl->name)
		return(NULL);
	
	if(gbl->pkg)
	{
		s0=BS2I_ImageTryGetGlobalQName(img, gbl->pkg);
		if(s0)
		{
			sprintf(tb, "%s/%s", s0, gbl->name);
			return(bgbdt_mm_rstrdup(tb));
		}
	}
	return(gbl->name);
}

/** Calculate the hash-value for a name. */
s64 BS2I_Image_QHashName(char *str)
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

u64 BS2I_FlagsFromFlagstr(BSVM2_CodeImage *img, char *str)
{
	char *s;
	u64 t;
	
	t=0; s=str;
	while(*s)
	{
		if((*s>='a') && (*s<='z'))
		{
			switch(*s)
			{
			case 'a': t=t|BS2CC_TYFL_ABSTRACT; break;
			case 'b': t=t|BS2CC_TYFL_BIGENDIAN; break;
			case 'c': t=t|BS2CC_TYFL_CONST; break;
			case 'd': t=t|BS2CC_TYFL_DYNAMIC; break;
			case 'e': t=t|BS2CC_TYFL_EXTERN; break;
			case 'f': t=t|BS2CC_TYFL_FINAL; break;
			case 'g': t=t|BS2CC_TYFL_GETTER; break;
			case 'h': t=t|BS2CC_TYFL_SETTER; break;
			case 'i': t=t|BS2CC_TYFL_INLINE; break;
			case 'j': t=t|BS2CC_TYFL_ASYNC; break;
			case 'k': t=t|BS2CC_TYFL_DELEGATE; break;
			case 'l': t=t|BS2CC_TYFL_LTLENDIAN; break;
			case 'm': t=t|BS2CC_TYFL_TRANSIENT; break;
			case 'n': t=t|BS2CC_TYFL_NATIVE; break;
			case 'o': t=t|BS2CC_TYFL_STRICT; break;
			case 'p': t=t|BS2CC_TYFL_PUBLIC; break;
			case 'q': t=t|BS2CC_TYFL_PRIVATE; break;
			case 'r': t=t|BS2CC_TYFL_PROTECTED; break;
			case 's': t=t|BS2CC_TYFL_STATIC; break;
			case 't': t=t|BS2CC_TYFL_THREAD; break;
			case 'u': t=t|BS2CC_TYFL_SYNCHRONIZED; break;
			case 'v': t=t|BS2CC_TYFL_VOLATILE; break;

			case 'w': t=t|BS2CC_TYFL_DLLIMPORT; break;
			case 'x': t=t|BS2CC_TYFL_DLLEXPORT; break;
			case 'y': t=t|BS2CC_TYFL_TYPEDEF; break;
			default:
				break;
			}
			s++;
			continue;
		}
		if((*s>='A') && *s<='D')
		{
			if((s[1]>='a') && (s[1]<='z'))
			{
				s+=2;
				continue;
			}
		}
		break;
	}
	
	return(t);
}

/** Decode the information about a given global (variable or object). */
int BS2I_ImageDecodeGlobalVar(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl,
	u32 dtag, byte *data, byte *edata)
{
	int tgitab[1024];
	BGBDTC_ClassInfo *clsi;
	BGBDTC_SlotInfo *clsvi;
	BSVM2_ImageGlobal *vi;
	byte *cs, *cs1, *cse, *csn;
	char *s;
	double vinitf;
	s64 v, vinit;
	u32 tag;
	int len, ngi;
	int i, j, k;

//#ifdef linux
//	printf("BS2I_ImageDecodeGlobalVar A\n", tag);
//#endif

	vinit=0;
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

			if(tag==BS2CC_I1CC_NAMEH)
			{
				gbl->nameh=v;
				continue;
			}

			if(tag==BS2CC_I1CC_QNAMEH)
			{
				gbl->qnameh=v;
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
				gbl->flags=BS2I_FlagsFromFlagstr(img, gbl->flagstr);
				continue;
			}

			if(tag==BS2CC_I1CC_VARINIT)
			{
				vinit=v;
				continue;
			}

			if(tag==BS2CC_I1CC_PACKAGE)
			{
				gbl->pkg=BS2I_ImageGetGlobal(img, v);
				continue;
			}

			if(tag==BS2CC_I1CC_ECLASS)
			{
				gbl->giobj=v;

#if 0
				if(dtag==BS2CC_ITCC_CL)
				{
					gbl->sugix=v;
				}else
				{
					gbl->obj=BS2I_ImageGetGlobal(img, v);
				}
#endif
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

		if(tag==BS2CC_I1CC_GITAG)
		{
			cs1=cs; ngi=0;
			while(cs1<csn)
			{
				cs1=BS2I_ReadUVLI(cs1, &v);
				tgitab[ngi++]=v;
			}

			gbl->figix=dtmMalloc(ngi*sizeof(int));
			gbl->nfigix=ngi;
			for(i=0; i<ngi; i++)
				{ gbl->figix[i]=tgitab[i]; }

			cs=csn;
			continue;
		}

		if(tag==BS2CC_I1CC_IMPL)
		{
			cs1=cs; ngi=0;
			while(cs1<csn)
			{
				cs1=BS2I_ReadUVLI(cs1, &v);
				tgitab[ngi++]=v;
			}

			gbl->ifgix=dtmMalloc(ngi*sizeof(int));
			gbl->nifgix=ngi;
			for(i=0; i<ngi; i++)
				{ gbl->ifgix[i]=tgitab[i]; }

			cs=csn;
			continue;
		}

		if(tag==BS2CC_I1CC_VARINIT)
		{
			cs1=cs;
			cs1=BS2I_ReadFVLI(cs1, &vinitf);

			cs=csn;
			continue;
		}

		BS2I_ImageCheckUnknownTag(img, tag);
		cs=csn;
	}

//#ifdef linux
//	printf("BS2I_ImageDecodeGlobalVar B\n", tag);
//#endif

	gbl->obj=BS2I_ImageGetGlobal(img, gbl->giobj);

//#ifdef linux
//	printf("BS2I_ImageDecodeGlobalVar C\n", tag);
//#endif

	if((dtag==BS2CC_ITCC_CL) ||
		(dtag==BS2CC_ITCC_ST) ||
		(dtag==BS2CC_ITCC_IF))
	{
//#ifdef linux
//		printf("BS2I_ImageDecodeGlobalVar D\n");
//#endif

		clsi=BGBDTC_GetClassSqGix(img->seqid, gbl->gix);
		gbl->objinf=clsi;
		
		BGBDTC_BeginLayoutClass(clsi);
		
		if(dtag==BS2CC_ITCC_CL)
			clsi->clsty=BGBDTC_CTY_CLASS;
		if(dtag==BS2CC_ITCC_ST)
			clsi->clsty=BGBDTC_CTY_STRUCT;
		if(dtag==BS2CC_ITCC_IF)
			clsi->clsty=BGBDTC_CTY_IFACE;
		
		if(gbl->obj)
			{ clsi->super=gbl->obj->objinf; }
		
		s=BS2I_ImageTryGetGlobalQName(img, gbl);
		if(s)
			{ clsi->qname=BGBDT_TagStr_StrSymbol(s); }

//#ifdef linux
//		printf("BS2I_ImageDecodeGlobalVar E\n");
//#endif

		if(gbl->nifgix>0)
		{
			BGBDTC_CheckExpandClassIfaceIndex(clsi, gbl->nifgix-1);
			for(i=0; i<gbl->nifgix; i++)
			{
				vi=BS2I_ImageGetGlobal(img, gbl->ifgix[i]);
				clsi->iface[i]=vi->objinf;
			}
			clsi->niface=gbl->nifgix;
		}

//#ifdef linux
//		printf("BS2I_ImageDecodeGlobalVar F\n");
//#endif

		if(gbl->nfigix>0)
		{
			BGBDTC_CheckExpandClassSlotIndex(clsi, gbl->nfigix-1);
			for(i=0; i<gbl->nfigix; i++)
			{
//#ifdef linux
//				printf("BS2I_ImageDecodeGlobalVar %d/%d\n",
//					i, gbl->nfigix);
//#endif

				if((gbl->gix==12) && (i==4))
				{
					j=-1;
//					BSVM2_DBGTRAP
				}

				clsvi=BGBDTC_GetClassSlotIndex(clsi, i);
				vi=BS2I_ImageGetGlobal(img, gbl->figix[i]);
				vi->objinf=clsvi;

//#ifdef linux
//				printf("IDGV B\n");
//#endif
				
				if(vi->name)
					clsvi->name=vi->name;
				if(vi->sig)
					clsvi->sig=vi->sig;
					
				if(vi->tag==BS2CC_ITCC_SV)
					clsvi->slotty=BGBDTC_STY_FIELD;

				if(vi->flags&BS2CC_TYFL_BIGENDIAN)
					clsvi->slotfl|=BGBDTC_SFL_BE;
				if(vi->flags&BS2CC_TYFL_LTLENDIAN)
					clsvi->slotfl|=BGBDTC_SFL_LE;
//#ifdef linux
//				printf("IDGV C\n");
//#endif

				if(vi->tag==BS2CC_ITCC_SF)
				{
					if(!vi->nameh)
						{ vi->nameh=BS2I_Image_QHashName(vi->name); }
				
					clsvi->slotty=BGBDTC_STY_METHOD;
					clsvi->init=dtvWrapPtr(vi);
					clsvi->nameh=vi->nameh;
				}

//#ifdef linux
//				printf("IDGV D\n");
//#endif
			}
		}

//#ifdef linux
//		printf("BS2I_ImageDecodeGlobalVar G\n");
//#endif
		
		BGBDTC_EndLayoutClass(clsi);

//#ifdef linux
//		printf("BS2I_ImageDecodeGlobalVar H\n");
//#endif
	}

	if(dtag==BS2CC_ITCC_GV)
	{
		gbl->brty=BSVM2_NatCall_GetSigOpZ(gbl->sig);
		gbl->gvalue=(BSVM2_Value *)(gbl->baty+16);

		switch(gbl->brty)
		{
		case BSVM2_OPZ_INT:
		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_UBYTE:
		case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_USHORT:
		case BSVM2_OPZ_SHORT:
			gbl->gvalue->i=vinit; break;
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_ULONG:
		case BSVM2_OPZ_NLONG:
		case BSVM2_OPZ_UNLONG:
			gbl->gvalue->l=vinit; break;
		case BSVM2_OPZ_FLOAT:
			gbl->gvalue->f=vinitf; break;
		case BSVM2_OPZ_DOUBLE:
			gbl->gvalue->d=vinitf; break;
		case BSVM2_OPZ_ADDRESS:
			gbl->gvalue->a=BS2I_ImageDecodeTagIndexAddr(img, vinit);
			break;
		default:
			break;
		}

#if 0
		switch(*gbl->sig)
		{
		case 'a': case 'b': case 'c': case 'h':
		case 'i': case 'j': case 'k': case 'w':
		case 's': case 't':
			gbl->gvalue->i=varinit; break;
		case 'l': case 'm': case 'x': case 'y':
			gbl->gvalue->l=varinit; break;
		case 'f':	gbl->gvalue->f=varinitf; break;
		case 'd':	gbl->gvalue->d=varinitf; break;
		case 'P': case 'Q':
		case 'L': case 'X':
		case 'A':
			gbl->gvalue->a=BS2I_ImageDecodeTagIndexAddr(img, varinit);
			break;
		case 'C':
			switch(s[1])
			{
			case 's': case 't':
				gbl->gvalue->a=BS2I_ImageDecodeTagIndexAddr(img, varinit);
				break;
			default:
				break;
			}
		default:
			break;
		}
#endif

		if(gbl->flags&BS2CC_TYFL_DYNAMIC)
		{
			gbl->nargs=bs2i_img_dynvseqid++;
			if(gbl->nargs>=bs2i_img_dynvlim)
			bs2i_img_dynvlim=bs2i_img_dynvlim+(bs2i_img_dynvlim>>1);
		}
	}

	if(dtag==BS2CC_ITCC_SV)
	{
		gbl->brty=BSVM2_NatCall_GetSigOpZ(gbl->sig);
	}

	return(0);
}

dtVal BS2I_ImageDecodeTagIndexAddr(
	BSVM2_CodeImage *img, int gix)
{
	BSVM2_ImageGlobal *vi;
	dtVal v;
	char *s;
	int i, j;
	
	i=gix;
	if(!(i&3))
	{
		if(!i)return(DTV_NULL);
	
		vi=BS2I_ImageGetGlobal(img, i>>2);
		if(!vi)return(DTV_NULL);
		
		j=vi->tag;
		if(	((j>=BS2CC_ITCC_A0) && (j<=BS2CC_ITCC_A9)) ||
			((j>=BS2CC_ITCC_Aa) && (j<=BS2CC_ITCC_Az)) )
				{ return(vi->gvalue->a); }
		
		return(dtvWrapPtr(vi));
	}
	
	j=i>>4;
	switch(i&15)
	{
	case BSVM2_OPZY_STRU8:
		s=img->strtab+j;
		v=BGBDT_TagStr_String(s);
		break;
	case BSVM2_OPZY_STRU16:
		s=img->strtab+j;
		v=BGBDT_TagStr_String16u8(s);
		break;
	case BSVM2_OPZY_STRASC:
		s=img->strtab+j;
		v=BGBDT_TagStr_StringAsc(s);
		break;

	case BSVM2_OPZY_STRSYM:
		s=img->strtab+j;
		v=BGBDT_TagStr_Symbol(s);
		break;
	case BSVM2_OPZY_STRKEY:
		s=img->strtab+j;
		v=BGBDT_TagStr_Keyword(s);
		break;

	case BSVM2_OPZY_INT:
		j=bsvm2_interp_decsignfoldi(j);
		v=dtvWrapInt(j);
		break;
	case BSVM2_OPZY_LONG:
		j=bsvm2_interp_decsignfoldi(j);
		v=dtvWrapLong(j);
		break;
	case BSVM2_OPZY_UINT:
		v=dtvWrapUInt(j);
		break;
	case BSVM2_OPZY_FLOAT:
	case BSVM2_OPZY_DOUBLE:
		j=bsvm2_interp_decsignfoldi(j);
		v=dtvWrapDouble(j*(1.0/4096));
		break;
	default:
		v=DTV_UNDEFINED;
		break;
	}
	return(v);
}

int BS2I_ImageDecodeGlobalArray(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl,
	u32 dtag, byte *data, byte *edata)
{
	byte *cs, *ct;
	dtVal na;
	u32 uj, uk;
	u64 uli;
	s64 li;
	double f;
	int z, isv, l, str;
	int i, j, k;

	z=-1; isv=0;
	if((dtag>=BS2CC_ITCC_A0) && (dtag<=BS2CC_ITCC_A9))
		z=dtag-BS2CC_ITCC_A0;
	if((dtag>=BS2CC_ITCC_Aa) && (dtag<=BS2CC_ITCC_Af))
		z=10+(dtag-BS2CC_ITCC_Aa);
	if((dtag>=BS2CC_ITCC_Ag) && (dtag<=BS2CC_ITCC_Av))
		{ z=dtag-BS2CC_ITCC_Ag; isv=1; }
	
	cs=data;
	if(isv)
	{
		cs=BS2I_ReadUVLI(cs, &uli);
		l=uli;
		na=dtvNewArray(l, z);
		ct=dtvArrayGetIndexAddrB1(na, 0);

		for(i=0; i<l; i++)
		{
			switch(z)
			{
			case BSVM2_OPZ_INT:		case BSVM2_OPZ_LONG:
			case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_SHORT:
				cs=BS2I_ReadSVLI(cs, &li);
				uli=li;
				j=li;
				break;
			case BSVM2_OPZ_UINT:	case BSVM2_OPZ_ULONG:
			case BSVM2_OPZ_UBYTE:	case BSVM2_OPZ_USHORT:
			case BSVM2_OPZ_ADDRESS:
				cs=BS2I_ReadUVLI(cs, &uli);
				li=uli;
				j=uli;
				break;
			case BSVM2_OPZ_FLOAT:	case BSVM2_OPZ_DOUBLE:
				cs=BS2I_ReadFVLI(cs, &f);
				break;
			}

			switch(z)
			{
			case BSVM2_OPZ_UBYTE:
			case BSVM2_OPZ_SBYTE:
				*ct++=j; break;
			case BSVM2_OPZ_USHORT:
			case BSVM2_OPZ_SHORT:
				*(u16 *)ct=j; ct+=2; break;
			case BSVM2_OPZ_INT:
			case BSVM2_OPZ_UINT:
				*(u32 *)ct=j; ct+=4; break;
			case BSVM2_OPZ_LONG:
			case BSVM2_OPZ_ULONG:
				*(u64 *)ct=uli; ct+=8; break;

			case BSVM2_OPZ_FLOAT:
				*(f32 *)ct=j; ct+=4; break;
			case BSVM2_OPZ_DOUBLE:
				*(f64 *)ct=f; ct+=8; break;
			
			case BSVM2_OPZ_ADDRESS:
				*(dtVal *)ct=BS2I_ImageDecodeTagIndexAddr(img, j);
				ct+=sizeof(dtVal); break;
			}
		}
	}else
	{
		str=BS2C_Image_BaseTypePackSize(z);
		l=(edata-data)/str;

		na=dtvNewArray(l, z);
		ct=dtvArrayGetIndexAddrB1(na, 0);
		
		for(i=0; i<l; i++)
		{
			switch(z)
			{
			case BSVM2_OPZ_UBYTE:
			case BSVM2_OPZ_SBYTE:
				*ct++=*cs++; break;
			case BSVM2_OPZ_USHORT:
			case BSVM2_OPZ_SHORT:
				j=(cs[0]<<8)|cs[1]; *(u16 *)ct=j;
				ct+=2; cs+=2; break;
			case BSVM2_OPZ_INT:
			case BSVM2_OPZ_UINT:
			case BSVM2_OPZ_FLOAT:
				j=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
				*(u32 *)ct=j;
				ct+=4; cs+=4; break;
			case BSVM2_OPZ_LONG:
			case BSVM2_OPZ_ULONG:
			case BSVM2_OPZ_DOUBLE:
				uj=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
				uk=(cs[4]<<24)|(cs[5]<<16)|(cs[6]<<8)|cs[7];
				uli=(((u64)uj)<<32)|uk;
				*(u64 *)ct=uli; ct+=8; cs+=8; break;
			
			case BSVM2_OPZ_ADDRESS:
				j=(cs[0]<<24)|(cs[1]<<16)|(cs[2]<<8)|cs[3];
				*(dtVal *)ct=BS2I_ImageDecodeTagIndexAddr(img, j);
				ct+=sizeof(dtVal); cs+=4; break;
			}
		}
	}

	gbl->gvalue=(BSVM2_Value *)(gbl->baty+16);
	gbl->gvalue->a=na;
	return(0);
}

/** Decode the information about a given global. */
int BS2I_ImageDecodeGlobal(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl, byte *gdat)
{
	byte *cs, *cs1, *cse, *csn;
	u32 tag;
	int len;

	cs=gdat;
	cs=BS2I_ReadTag2(cs, &tag, &len);
	cse=cs+len;

//#ifdef linux
//	printf("BS2I_ImageDecodeGlobal: %08X\n", tag);
//#endif

	if((cse<cs) || (cse>img->egblsdat))
	{
		BS2I_ImageErrorCorruptTag(img);
		return(-1);
	}

	gbl->tag=tag;

	if(tag==BS2CC_ITCC_GF)
		{ return(BS2I_ImageDecodeGlobalFunc(img, gbl, tag, cs, cse)); }
	if(tag==BS2CC_ITCC_SF)
		{ return(BS2I_ImageDecodeGlobalFunc(img, gbl, tag, cs, cse)); }

	if(tag==BS2CC_ITCC_GV)
		{ return(BS2I_ImageDecodeGlobalVar(img, gbl, tag, cs, cse)); }
	if(tag==BS2CC_ITCC_SV)
		{ return(BS2I_ImageDecodeGlobalVar(img, gbl, tag, cs, cse)); }

	if(tag==BS2CC_ITCC_PK)
		{ return(BS2I_ImageDecodeGlobalVar(img, gbl, tag, cs, cse)); }

	if(tag==BS2CC_ITCC_ST)
		{ return(BS2I_ImageDecodeGlobalVar(img, gbl, tag, cs, cse)); }
	if(tag==BS2CC_ITCC_CL)
		{ return(BS2I_ImageDecodeGlobalVar(img, gbl, tag, cs, cse)); }
	if(tag==BS2CC_ITCC_IF)
		{ return(BS2I_ImageDecodeGlobalVar(img, gbl, tag, cs, cse)); }

	if((tag>=BS2CC_ITCC_A0) && (tag<=BS2CC_ITCC_A9))
		{ return(BS2I_ImageDecodeGlobalArray(img, gbl, tag, cs, cse)); }
	if((tag>=BS2CC_ITCC_Aa) && (tag<=BS2CC_ITCC_Az))
		{ return(BS2I_ImageDecodeGlobalArray(img, gbl, tag, cs, cse)); }

	BS2I_ImageCheckUnknownTag(img, tag);
	return(-1);
}

/** Get the global associated with a given index from an image. */
BSVM2_ImageGlobal *BS2I_ImageGetGlobal(BSVM2_CodeImage *img, int gix)
{
	BSVM2_ImageGlobal *gbl;
	byte *gdat;
	int gofs;
	
//#ifdef linux
//	printf("BS2I_ImageGetGlobal: %d\n", gix);
//#endif

	if(!gix)
		return(NULL);
	if(gix<0)
	{
		BSVM2_DBGTRAP
		return(NULL);
	}
	if(gix>=img->ngbls)
	{
		BSVM2_DBGTRAP
		return(NULL);
	}
	
	gbl=img->gbls[gix];
	if(gbl)
	{
		return(gbl);
	}

//#ifdef linux
//	printf("BS2I_ImageGetGlobal: %d\n", gix);
//#endif

	gdat=BS2I_ImageGetGlobalLump(img, gix);
	if(!gdat)
	{
		BSVM2_DBGTRAP
		return(NULL);
	}

	gbl=BS2I_AllocImageGlobal(img);
	gbl->gix=gix;
	img->gbls[gix]=gbl;
	
//	gofs=BS2I_ImageGetGlobalOfs(img, gix);
	BS2I_ImageDecodeGlobal(img, gbl, gdat);
	
//#ifdef linux
//	printf("BS2I_ImageGetGlobal: OK %d\n", gix);
//#endif

	return(gbl);
}

/** Preload a global for a given index when loading an image. */
BSVM2_ImageGlobal *BS2I_ImageGetGlobalInitial(BSVM2_CodeImage *img, int gix)
{
	BSVM2_ImageGlobal *gbl;
	byte *gdat;
	int gofs;
	u32 tag;
	int len;
	int i;
	
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

	BS2I_ReadTag2(gdat, &tag, &len);

	i=0;
	if(tag==BS2CC_ITCC_ST)i=1;
	if(tag==BS2CC_ITCC_CL)i=1;
	if(tag==BS2CC_ITCC_IF)i=1;
	if(tag==BS2CC_ITCC_GF)i=1;

	if(!i)
		return(NULL);

	gbl=BS2I_AllocImageGlobal(img);
	gbl->gix=gix;
	img->gbls[gix]=gbl;
	
//	gofs=BS2I_ImageGetGlobalOfs(img, gix);
	BS2I_ImageDecodeGlobal(img, gbl, gdat);
	
	return(gbl);
}

/** Lookup a function within an image. */
BS2VM_API BSVM2_ImageGlobal *BS2I_ImageLookupFunc(
	BSVM2_CodeImage *img, char *qname)
{
	BSVM2_ImageGlobal *gbl;
	int i;

	for(i=1; i<img->ngbls; i++)
	{
		gbl=img->gbls[i];
		if(!gbl)
			continue;
		if(gbl->name && !strcmp(gbl->name, qname))
			return(gbl);
		if(gbl->qname && !strcmp(gbl->qname, qname))
			return(gbl);
	}
	return(NULL);
}

/** Lookup a global variable within an image. */
BS2VM_API BSVM2_ImageGlobal *BS2I_ImageLookupGlobalVar(
	BSVM2_CodeImage *img, char *qname)
{
	BSVM2_ImageGlobal *gbl;
	int i;

	if(!img)
		return(NULL);

	for(i=1; i<img->ngbls; i++)
	{
		gbl=img->gbls[i];
		if(!gbl)
			continue;
		if(gbl->name && !strcmp(gbl->name, qname))
			return(gbl);
		if(gbl->qname && !strcmp(gbl->qname, qname))
			return(gbl);
	}
	return(NULL);
}

/** Get the main function for an image. */
BS2VM_API BSVM2_ImageGlobal *BS2I_ImageGetMain(
	BSVM2_CodeImage *img, char *qnpkg)
{
	char tb[256];
	BSVM2_ImageGlobal *vi, *bvi;
	int i, j, k;

	if(img->nmaix<=0)
		return(NULL);

	if(!qnpkg || (img->nmaix==1))
	{
		i=img->maix[0];
		vi=BS2I_ImageGetGlobal(img, i);
		return(vi);
	}

	i=img->maix[0];
	bvi=BS2I_ImageGetGlobal(img, i);
	
	for(i=0; i<img->nmaix; i++)
	{
		j=img->maix[i];
		vi=BS2I_ImageGetGlobal(img, j);
		
		if(vi->obj)
		{
			if(vi->obj->qname)
			{
				if(!strcmp(vi->obj->qname, qnpkg))
					{ bvi=vi; break; }
			}

			if(vi->obj->name && vi->obj->pkg && vi->obj->pkg->qname)
			{
				sprintf(tb, "%s/%s",
					vi->obj->pkg->qname,
					vi->obj->name);
				if(!strcmp(tb, qnpkg))
					{ bvi=vi; break; }
			}

			if(vi->obj->name)
			{
				if(!strcmp(vi->obj->name, qnpkg))
					{ bvi=vi; break; }
			}
			
			if(vi->obj->pkg && vi->obj->pkg->qname)
			{
				if(!strcmp(vi->obj->pkg->qname, qnpkg))
					{ bvi=vi; break; }
			}
		}
		
		if(vi->pkg && vi->pkg->qname)
		{
			if(!strcmp(vi->pkg->qname, qnpkg))
				{ bvi=vi; break; }
		}
	}
	
	return(bvi);
}

/** Get the entry-point trace for a given function. */
BS2VM_API BSVM2_Trace *BS2I_ImageGetFuncTrace(
	BSVM2_ImageGlobal *vi)
{
	BSVM2_Trace *tr;

	if(vi->ctrace)
		return(vi->ctrace);

	tr=BSVM2_Interp_DecodeBlockTraces(vi->cblk);
	vi->ctrace=tr;
	return(tr);
}

/** Get the trace associated with the entry point for an image. */
BS2VM_API BSVM2_Trace *BS2I_ImageGetMainTrace(
	BSVM2_CodeImage *img, char *qnpkg)
{
	BSVM2_ImageGlobal *vi;
	BSVM2_Trace *tr;
	
	vi=BS2I_ImageGetMain(img, qnpkg);
	if(!vi || !vi->cblk)
		return(NULL);
//	tr=BSVM2_Interp_DecodeBlockTraces(vi->cblk);
	tr=BS2I_ImageGetFuncTrace(vi);
	return(tr);
}

/** Load a bytecode image into the VM. */
BS2VM_API BSVM2_CodeImage *BS2I_DecodeImageBuffer(byte *ibuf, int isz)
{
	byte tb[4096];
	int tarri[256];
	BSVM2_CodeImage *img;
	u64 tag;
	s64 len, v;
	byte *cs, *cs1, *cse, *cse1, *csn;
	int i, j, k, l, n;

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
	img->seqid=bs2i_img_seqid++;
	
	img->data=dtmMalloc(isz);
	img->szdata=isz;
	memcpy(img->data, ibuf, isz);
	
	cs=img->data;
	cs=BS2I_ReadTag(cs, &tag, &len);
	cse=cs+len;
	
	while(cs<cse)
	{
		cs=BS2I_ReadTag(cs, &tag, &len);

		if(len==BS2CC_ILEN_SVLI)
		{
			cs=BS2I_ReadSVLI(cs, &v);

			if(tag==BS2CC_I1CC_MAIN)
			{
				img->maix=img->tmaix;
				img->maix[0]=v;
				img->nmaix=1;
				continue;
			}

			BS2I_ImageCheckUnknownTag(img, tag);
			continue;
		}

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

		if(tag==BS2CC_IFCC_MAIN)
		{
			cs1=cs; n=0;
			while(cs<csn)
			{
				cs=BS2I_ReadUVLI(cs, &v);
				tarri[n++]=v;
			}
			
			if(n>8)
				{ img->maix=dtmMalloc(n*sizeof(int)); }
			else
				{ img->maix=img->tmaix; }
			for(i=0; i<n; i++)
				img->maix[i]=tarri[i];
			img->nmaix=n;

			cs=csn;
			continue;
		}

		if((tag==BS2CC_IFCC_PVBM) || (tag==BS2CC_ITCC_PV))
		{
			img->pvbm=cs;
			img->epvbm=csn;
			cs=csn;
			continue;
		}

		BS2I_ImageCheckUnknownTag(img, tag);
		cs=csn;
		continue;
	}

	img->gbls=dtmMalloc(img->ngbls*sizeof(BSVM2_ImageGlobal *));
	
	if(img->pvbm)
	{
		memset(tb, 0, (img->ngbls+7)/8);
	
		cs=img->pvbm; cse=img->epvbm;
		n=0;
		while((cs<cse) && ((n*8)<img->ngbls))
		{
			i=*cs++;
			switch(i>>5)
			{
			case 0:
				l=(i&31)+1;
				for(j=0; j<l; j++)
					tb[n++]=0x00;
				break;
			case 1:
				l=(i&31)+1;
				for(j=0; j<l; j++)
					tb[n++]=0xFF;
				break;
			case 2:
				l=(i&31)+1;
				for(j=0; j<l; j++)
					tb[n++]=*cs++;
				break;

			case 4: tb[n++]=0x00|(i&31); break;
			case 5: tb[n++]=0xE0|(i&31); break;
			case 6: tb[n++]=0x00|((i&31)<<3); break;
			case 7: tb[n++]=0x07|((i&31)<<3); break;
			default:
				break;
			}
		}

		if(cs<cse)
			{ BSVM2_DBGTRAP }

//#ifdef linux
//		printf("Start PVBM\n");
//#endif
		for(i=1; i<img->ngbls; i++)
		{
			if(tb[i>>3]&(128>>(i&7)))
			{
//#ifdef linux
//				printf("PVBM %d/%d\n", i, img->ngbls);
//#endif
				BS2I_ImageGetGlobal(img, i);
			}
		}
//#ifdef linux
//		printf("End PVBM\n");
//#endif
	}else
	{
		for(i=1; i<img->ngbls; i++)
			BS2I_ImageGetGlobalInitial(img, i);
	}
	
	//HACK: need a "loader" or "process" context here
	img->next=bs2i_liveimg;
	bs2i_liveimg=img;
	
	return(img);
}

/** Lookup a global variable. */
BS2VM_API BSVM2_ImageGlobal *BS2I_GlobalLookupGlobalVar(char *qname)
{
	BSVM2_CodeImage *icur;
	BSVM2_ImageGlobal *gbl;
	
	icur=bs2i_liveimg;
	while(icur)
	{
		gbl=BS2I_ImageLookupGlobalVar(icur, qname);
		if(gbl)
			return(gbl);
		icur=icur->next;
	}
	return(NULL);
}
