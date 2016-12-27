int BTAC1C_FlattenWAV_PCM(byte *obuf, void *ibuf,
	int ch, int rt, int bits, int len)
{
	byte *ct;
	FILE *fd;
	int i, j, k;

	ct=obuf;

	*ct++='R';		*ct++='I';
	*ct++='F';		*ct++='F';
	*ct++=0;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='W';		*ct++='A';
	*ct++='V';		*ct++='E';

	//format header
	*ct++='f';		*ct++='m';
	*ct++='t';		*ct++=' ';
	*ct++=16;		*ct++=0;
	*ct++=0;		*ct++=0;

	*ct++=1;		*ct++=0;
	*ct++=ch;		*ct++=0;

	*ct++=rt;		*ct++=rt>>8;
	*ct++=rt>>16;	*ct++=rt>>24;

	i=rt*ch*(bits/8);
	*ct++=i;		*ct++=i>>8;
	*ct++=i>>16;	*ct++=i>>24;

	*ct++=ch*(bits/8);		*ct++=0;
	*ct++=bits;			*ct++=0;


	//data
	*ct++='d';		*ct++='a';
	*ct++='t';		*ct++='a';

	i=len*ch*(bits/8);
	*ct++=i;		*ct++=i>>8;
	*ct++=i>>16;	*ct++=i>>24;

	j=(i+1)&(~1);
	k=(ct-obuf)+j;
	if(ibuf)
	{
		memcpy(ct, ibuf, i);
		ct+=j;
	}

	i=k-8;
	obuf[4]=i;		obuf[5]=i>> 8;
	obuf[6]=i>>16;	obuf[7]=i>>24;
	
	return(ct-obuf);
}

int BTAC1C_FlattenWAV_BTAC1C(byte *obuf, void *ibuf,
	int ch, int rt, int bsz, int len)
{
	byte *ct;
	int i, j, k, bl, nb, isbt;

	bl=2*bsz;

	nb=(len+(bl-1))/bl;
	isbt=0;
	
	for(i=0; i<nb; i++)
	{
		j=((byte *)ibuf)[i*bsz+2];
		k=((byte *)ibuf)[i*bsz+3];
		if(j>88)isbt=1;
		if(k)isbt=1;
	}

	ct=obuf;

	*ct++='R';		*ct++='I';
	*ct++='F';		*ct++='F';
	*ct++=0;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='W';		*ct++='A';
	*ct++='V';		*ct++='E';

	//format header
	*ct++='f';		*ct++='m';
	*ct++='t';		*ct++=' ';
	*ct++=20;		*ct++=0;
	*ct++=0;		*ct++=0;

//	if(ch!=1)
//	if(1)
	if((ch!=1) || isbt)
	{
		*ct++=0x1C;		*ct++=0x7B;
		*ct++=ch;		*ct++=0;
	}else
	{
		*ct++=0x11;		*ct++=0x00;
		*ct++=ch;		*ct++=0;
	}

	*ct++=rt;		*ct++=rt>>8;
	*ct++=rt>>16;	*ct++=rt>>24;

	i=(rt*4)/8;
	*ct++=i;		*ct++=i>>8;
	*ct++=i>>16;	*ct++=i>>24;

	i=bsz;
	*ct++=i;			*ct++=i>>8;
	*ct++=4;			*ct++=0;

	*ct++=2;			*ct++=0;
	*ct++=0xF9;			*ct++=0x03;

	//data
	*ct++='d';		*ct++='a';
	*ct++='t';		*ct++='a';

	i=((len+(bl-1))/bl)*bsz;
	*ct++=i;		*ct++=i>> 8;
	*ct++=i>>16;	*ct++=i>>24;

	j=(i+1)&(~1);
	k=(ct-obuf)+j;
	if(ibuf)
	{
		memcpy(ct, ibuf, i);
		ct+=j;
	}

//	i=(ct-obuf)-8;
	i=k-8;
	obuf[4]=i;		obuf[5]=i>> 8;
	obuf[6]=i>>16;	obuf[7]=i>>24;
	
	return(ct-obuf);
}

int BTAC1C_StoreWAV_PCM(char *name, void *ibuf,
	int ch, int rt, int bits, int len)
{
	byte tb[256];
	FILE *fd;
	int i, j, k;
	
	fd=fopen(name, "wb");
	if(!fd)
		return(-1);
	j=len*ch*(bits/8);
	i=BTAC1C_FlattenWAV_PCM(tb, NULL, ch, rt, bits, len);
	fwrite(tb, 1, i, fd);
	fwrite(ibuf, 1, j, fd);
	if(j&1)fputc(0, fd);
	fclose(fd);
	return(0);
}

int BTAC1C_StoreWAV_BTAC1C(char *name, void *ibuf,
	int ch, int rt, int bsz, int len)
{
	byte tb[256];
	FILE *fd;
	int i, j, k;
	
	fd=fopen(name, "wb");
	if(!fd)
		return(-1);
	j=len*ch*(bits/8);
	i=BTAC1C_FlattenWAV_BTAC1C(tb, NULL, ch, rt, bsz, len);
	fwrite(tb, 1, i, fd);
	fwrite(ibuf, 1, j, fd);
	if(j&1)fputc(0, fd);
	fclose(fd);
	return(0);
}
