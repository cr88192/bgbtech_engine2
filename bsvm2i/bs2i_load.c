#include <bteifgl.h>

byte *BS2I_ReadTag(byte *cs, u64 *rtag, s64 *rlen)
{
	u64 li;
	u32 uj, uk;
	int i, j, k;

	i=*cs++;

	if((i>=0x01) && (i<=0x1A))
	{
		*rtag='A'+(i-0x01);
		*rlen=-1;
		return(cs);
	}
	if((i>=0x1B) && (i<=0x1F))
	{
		*rtag='0'+(i-0x1B);
		*rlen=-1;
		return(cs);
	}

	if((i>=0x21) && (i<=0x3A))
	{
		*rtag='a'+(i-0x21);
		*rlen=-1;
		return(cs);
	}
	if((i>=0x3B) && (i<=0x3F))
	{
		*rtag='5'+(i-0x3B);
		*rlen=-1;
		return(cs);
	}

	if((i>=0x01) && (i<=0x1A))
	{
		*rtag='A'+(i-0x01);
		*rlen=-1;
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

int BS2I_ImageDecodeGlobal(
	BSVM2_CodeImage *img, BSVM2_ImageGlobal *gbl, byte *gdat)
{
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

		cs=csn;
		continue;
	}

	img->gbls=frgl_malloc(img->ngbls*sizeof(BSVM2_ImageGlobal *));
	
	return(img);
}
