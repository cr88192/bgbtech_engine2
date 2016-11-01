#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
#endif

#ifndef __cplusplus

#ifndef BOOL_T
#define BOOL_T
typedef unsigned char bool;
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif


#define BS2CC_IMG_TWOCC(a, b)			(((a)<<8)|(b))
#define BS2CC_IMG_FOURCC(a, b, c, d)	(((a)<<24)|((b)<<16)|((c)<<8)|(d))

#define BS2CC_IMG_EIGHTCC(a, b, c, d, e, f, g, h)	\
	((((u64)BS2CC_IMG_FOURCC(a, b, c, d))<<32)|		\
	BS2CC_IMG_FOURCC(e, f, g, h))

#define BS2CC_IECC_BS2I0EXE	BS2CC_IMG_EIGHTCC('B','S','2','I','0','E','X','E')
#define BS2CC_IECC_BS2I0DLL	BS2CC_IMG_EIGHTCC('B','S','2','I','0','D','L','L')

#define BS2CC_IFCC_BS2I	BS2CC_IMG_FOURCC('B', 'S', '2', 'I')
#define BS2CC_IFCC_BS2L	BS2CC_IMG_FOURCC('B', 'S', '2', 'L')

#define BS2CC_IFCC_STRS	BS2CC_IMG_FOURCC('S', 'T', 'R', 'S')
#define BS2CC_IFCC_BADD	BS2CC_IMG_FOURCC('B', 'A', 'D', 'D')

#define BS2CC_IFCC_GBLS	BS2CC_IMG_FOURCC('G', 'B', 'L', 'S')
#define BS2CC_IFCC_GIX2	BS2CC_IMG_FOURCC('G', 'I', 'X', '2')
#define BS2CC_IFCC_GIX3	BS2CC_IMG_FOURCC('G', 'I', 'X', '3')
#define BS2CC_IFCC_GIX4	BS2CC_IMG_FOURCC('G', 'I', 'X', '4')

#define BS2CC_IFCC_MAIN	BS2CC_IMG_FOURCC('M', 'A', 'I', 'N')
#define BS2CC_IFCC_PVBM	BS2CC_IMG_FOURCC('P', 'V', 'B', 'M')

#define BS2CC_ITCC_BC	BS2CC_IMG_TWOCC('B', 'C')
#define BS2CC_ITCC_CL	BS2CC_IMG_TWOCC('C', 'L')
#define BS2CC_ITCC_GF	BS2CC_IMG_TWOCC('G', 'F')
#define BS2CC_ITCC_GV	BS2CC_IMG_TWOCC('G', 'V')
#define BS2CC_ITCC_IF	BS2CC_IMG_TWOCC('I', 'F')
#define BS2CC_ITCC_PK	BS2CC_IMG_TWOCC('P', 'K')
#define BS2CC_ITCC_PV	BS2CC_IMG_TWOCC('P', 'V')
#define BS2CC_ITCC_SF	BS2CC_IMG_TWOCC('S', 'F')
#define BS2CC_ITCC_ST	BS2CC_IMG_TWOCC('S', 'T')
#define BS2CC_ITCC_SV	BS2CC_IMG_TWOCC('S', 'V')
// #define BS2CC_ITCC_SA	BS2C_IMG_TWOCC('S', 'A')

#define BS2CC_I1CC_NULL		' '

#define BS2CC_I1CC_NARGS	'A'
#define BS2CC_I1CC_BARGS	'B'
#define BS2CC_I1CC_CODE		'C'
#define BS2CC_I1CC_STKDEPTH	'D'
#define BS2CC_I1CC_ECLASS	'E'
#define BS2CC_I1CC_FLAGS	'F'
#define BS2CC_I1CC_GITAG	'G'
#define BS2CC_I1CC_VARINIT	'H'
#define BS2CC_I1CC_IMPL		'I'

#define BS2CC_I1CC_NLOCALS	'L'

#define BS2CC_I1CC_MAIN		'M'
#define BS2CC_I1CC_NAME		'N'

#define BS2CC_I1CC_PACKAGE	'P'
#define BS2CC_I1CC_QNAME	'Q'
#define BS2CC_I1CC_QNAMEH	'R'
#define BS2CC_I1CC_SIG		'S'
#define BS2CC_I1CC_NAMEH	'T'

#define BS2CC_ILEN_SVLI		-2


void *loadfile(char *name, int *rsz)
{
	void *buf;
	FILE *fd;
	int sz;

	fd=fopen(name, "rb");
	if(!fd)return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=malloc(sz+16);
	memset(buf, 0, sz+16);
	fread(buf, 1, sz, fd);

	fclose(fd);

	if(rsz)*rsz=sz;
	return(buf);
}

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

byte *BS2I_ReadTag2(byte *cs, u32 *rtag, int *rlen)
{
	u64 ttag;
	s64 tlen;

	cs=BS2I_ReadTag(cs, &ttag, &tlen);
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
	
	*rv=i;
	return(cs);
}

char *Tag2Str(u64 tag)
{
	static char buf[16*17];
	static byte pos;
	char *tb, *t;
	
	tb=buf+(pos*17);
	pos=(pos+1)&15;
	
	t=tb;
	
	if(!(tag>>8))
	{
		if((tag>=' ') && (tag<='~'))
		{
			*t++=tag;
			*t++=0;
			return(tb);
		}
		sprintf(tb, "%02X", (int)tag);
		return(tb);
	}

	if(!(tag>>16))
	{
		if((tag>=0x2020) && (tag<=0x7E7E))
		{
			*t++=tag>>8;
			*t++=tag;
			*t++=0;
			return(tb);
		}
		sprintf(tb, "%04X", (int)tag);
		return(tb);
	}

	if(!(tag>>32))
	{
		if((tag>=0x20202020) && (tag<=0x7E7E7E7E))
		{
			*t++=tag>>24;
			*t++=tag>>16;
			*t++=tag>> 8;
			*t++=tag    ;
			*t++=0;
			return(tb);
		}
		sprintf(tb, "%08X", (int)tag);
		return(tb);
	}

	if((tag>=0x2020202020202020ULL) && (tag<=0x7E7E7E7E7E7E7E7EULL))
	{
		*t++=tag>>56;
		*t++=tag>>48;
		*t++=tag>>40;
		*t++=tag>>32;
		*t++=tag>>24;
		*t++=tag>>16;
		*t++=tag>> 8;
		*t++=tag    ;
		*t++=0;
		return(tb);
	}
	sprintf(tb, "%016llX", tag);
	return(tb);
}

int Tag2Rec(u64 tag)
{
	if(tag==BS2CC_IFCC_BS2I)
		return(1);
	if(tag==BS2CC_IFCC_BS2L)
		return(1);

	if(tag==BS2CC_IFCC_GBLS)
		return(1);

	if(tag==BS2CC_ITCC_GF)
		return(1);
	if(tag==BS2CC_ITCC_SF)
		return(1);
	if(tag==BS2CC_ITCC_SV)
		return(1);
	if(tag==BS2CC_ITCC_CL)
		return(1);
	if(tag==BS2CC_ITCC_ST)
		return(1);
	if(tag==BS2CC_ITCC_IF)
		return(1);

	return(0);
}

char *tag_strs, *tag_strse;

u32 BS2C_Image_HashData(byte *buf, int sz)
{
	u32 h;
	int i;
	
	h=0;
	for(i=0; i<sz; i++)
		{ h=h*65521+buf[i]; }
	return(h);
}

int Rec_TagCmp(byte *buf1, byte *cs1e, byte *buf2, byte *cs2e, int ind)
{
	byte *cs1, *cs2, *cs1n, *cs2n;
	char *s0;
	u64 tag1, tag2;
	s64 len1, len2;
	s64 val1, val2;
	u32 h0, h1;
	int i, j;
	
	cs1=buf1;
	cs2=buf2;
	
	if(*cs1 != *cs2)
	{
		printf("Tag-Type Mismatch %02X != %02X\n", *cs1, *cs2);
		return(0);
	}
	
	while((cs1<cs1e) && (cs2<cs2e))
	{
		cs1=BS2I_ReadTag(cs1, &tag1, &len1);
		cs2=BS2I_ReadTag(cs2, &tag2, &len2);
		cs1n=cs1+len1;
		cs2n=cs2+len2;
	
		for(i=0; i<ind; i++)
			printf("  ");
	
		if((tag1!=tag2))
		{
			printf("Tag-ID Mismatch %s != %s\n",
				Tag2Str(tag1), Tag2Str(tag2));
			break;
		}

		if((len1!=len2))
		{
			printf("Length Mismatch %lld != %lld\n", len1, len2);
			break;
		}
		
		if(len1==BS2CC_ILEN_SVLI)
		{
			s0="";
		
			cs1=BS2I_ReadSVLI(cs1, &val1);
			cs2=BS2I_ReadSVLI(cs2, &val2);

			if(tag_strs)
			{
				i=((int)val1);
				if(((tag1==BS2CC_I1CC_NAME) ||
					(tag1==BS2CC_I1CC_SIG)) &&
					(i>0) && (tag_strs+i)<tag_strse)
						s0=tag_strs+i;
			}

			if(val1==val2)
			{
				printf("VTag '%s': %lld '%s'\n", Tag2Str(tag1), val1, s0);
				continue;
			}

			printf("VTag '%s': %lld!=%lld\n", Tag2Str(tag1), val1, val2);
			continue;
		}
		
		if(tag1==BS2CC_IFCC_STRS)
		{
			tag_strs=cs1;
			tag_strse=cs1n;
		}
		
		if(!memcmp(cs1, cs2, len1))
		{
			printf("Tag '%s' %lld, Match\n", Tag2Str(tag1), len1);
			cs1=cs1n;
			cs2=cs2n;
			continue;
		}

		printf("Tag '%s' %lld, Differ\n", Tag2Str(tag1), len1);
		
		if(Tag2Rec(tag1))
		{
			Rec_TagCmp(cs1, cs1n, cs2, cs2n, ind+1);
		}else
		{
			h0=BS2C_Image_HashData(cs1, len1);
			h1=BS2C_Image_HashData(cs2, len2);

			for(i=0; i<(ind+1); i++)
				printf("  ");
			printf("H: %08X %08X\n", h0, h1);
			
			for(i=0; i<(ind+1); i++)
				printf("  ");
			for(i=0; i<len1; i++)
			{
				if(cs1[i]!=cs2[i])
					break;
			}
			printf("%06X ", i);

			for(j=0; j<8; j++)
				printf("(%02X %02X) ", cs1[i+j], cs2[i+j]);
			printf("\n");
		}
		
		cs1=cs1n;
		cs2=cs2n;
	}
	return(0);
}

int main(int argc, char *argv[])
{
	byte *buf1, *buf2;
	int sz1, sz2;

	buf1=loadfile(argv[1], &sz1);
	buf2=loadfile(argv[2], &sz2);
	
	if((sz1==sz2) && !memcmp(buf1, buf2, sz1))
	{
		printf("Exact Match\n");
		return(0);
	}
	
	Rec_TagCmp(buf1, buf1+sz1, buf2, buf2+sz2, 0);
	return(0);
}
