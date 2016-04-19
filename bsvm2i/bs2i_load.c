#include <bteifgl.h>

byte *BS2I_ReadTag(byte *cs, u64 *rtag, s64 *rlen)
{
	u64 li;
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
		k=cs[0]|(cs[1]<<8); cs+=2;
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
		k=cs[0]|(cs[1]<<8); cs+=2;
		*rtag=k;
		*rlen=j;
		return(cs);
	}

	if((i>=0xA0) && (i<=0xAF))
	{
		j=i-0xA0;
		j=(j<<8)|(*cs++);
		k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24); cs+=4;
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
		k=cs[0]|(cs[1]<<8)|(cs[2]<<16)|(cs[3]<<24); cs+=4;
		*rtag=((u32)k);
		*rlen=j;
		return(cs);
	}
}
