/*
	Written by Brendan G Bohannon
	Free for use for whatever purpose...

	This was written sometime before 2003, but I don't know when...
	(some evidence points to the Fall-2001/Spring-2002 timeframe).

	This converts BDF fonts to a custom "BFN format".
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *in, *out;
	char buf[256], **bufs;
	int i, j, k, l;

	char charn[256];
	int idx, lidx, inbm, hp;
	int rs, re, chk;

	int w, h, head;

	char *charbuf;

	in=fopen(argv[1], "rt");
	if(!in)
	{
		printf("open %s failed\n", argv[1]);
		return(-1);
	}
	out=fopen(argv[2], "wb");

	bufs=malloc(64*sizeof(char *));
	for(i=0; i<64; i++)bufs[i]=malloc(64);
	inbm=0;

	charbuf=malloc(65536*32);
	memset(charbuf, 0, 65536*32);

	lidx=0;
	rs=0;
	re=0;
	chk=0;

	w=16;
	h=16;
	head=1;

	while(!feof(in))
	{
		fgets(buf, 255, in);

		j=0;
		k=0;
		for(i=0; i<256; i++)if(buf[i]<=' ')
		{
			bufs[j][k]=0;
			if(k)j++;
			k=0;
		}else bufs[j][k++]=buf[i];

//		for(i=0; i<8; i++)printf("'%s' ", bufs[i]);
//		printf("\n");

		if(!strcmp(bufs[0], "FONTBOUNDINGBOX"))
		{
			w=atoi(bufs[1]);
			h=atoi(bufs[2]);
			continue;
		}

		if(!strcmp(bufs[0], "STARTCHAR"))
		{
			for(i=0; i<256; i++)if(buf[i]<=' ')break;
			for(; i<256; i++)if(buf[i]>' ')break;

			j=0;
			for(; i<256; i++)if(buf[i]=='\n')break;
				else charn[j++]=buf[i];
			charn[j]=0;

//			strcpy(charn, buf+10);
			continue;
		}
		if(!strcmp(bufs[0], "ENCODING"))
		{
			idx=atoi(bufs[1]);
			if(idx!=(lidx+1))
			{
				printf("chunk %d, break %d-%d, last range %d-%d\n", chk, lidx, idx, rs, re);
				if(rs|re)
				{
					if(head)
					{
						fputc(0x00, out);	//0xBF00, big-endian
						fputc(0xBF, out);

						fputc(0x04, out);	//head len
						fputc(0x00, out);

						fputc(0x00, out);
						fputc(w, out);
						fputc(h, out);
						fputc(0x01, out);

						head=0;
					}

					fputc(rs&0xff, out);
					fputc((rs>>8)&0xff, out);
					fputc(re&0xff, out);
					fputc((re>>8)&0xff, out);
					fwrite(charbuf, re-rs+1, (w*h)/8, out);
				}
				rs=idx;
				re=idx;
				chk++;
			}else re=idx;
			lidx=idx;
			continue;
		}

		if(!strcmp(bufs[0], "BITMAP"))
		{
			inbm=1;
			hp=0;
			continue;
		}
		if(!strcmp(bufs[0], "ENDCHAR"))
		{
//			printf("char: %d: %s\n", idx, charn);
			inbm=0;
			continue;
		}
		if(inbm)
		{
			sscanf(bufs[0], "%X", &i);

			if(w==16)
			{
				charbuf[((idx-rs)*((w*h)/8))+((hp*w)/8)]=i>>8;
				charbuf[((idx-rs)*((w*h)/8))+((hp*w)/8)+1]=i;
			}else charbuf[((idx-rs)*((w*h)/8))+((hp*w)/8)]=i;

			hp++;
		}
	}

	printf("chunk %d, range %d-%d\n", chk, rs, re);
	if(rs|re)
	{
		if(head)
		{
			fputc(0x00, out);	//0xBF00
			fputc(0xBF, out);

			fputc(0x04, out);	//head len
			fputc(0x00, out);

			fputc(0x00, out);
			fputc(w, out);
			fputc(h, out);
			fputc(0x01, out);

			head=0;
		}
		fputc(rs&0xff, out);
		fputc((rs>>8)&0xff, out);
		fputc(re&0xff, out);
		fputc((re>>8)&0xff, out);
		fwrite(charbuf, re-rs+1, (w*h)/8, out);
	}

	//special end of font marker...
	fputc(0, out);
	fputc(0, out);
	fputc(0, out);
	fputc(0, out);

#if 0
	return(0);

	s=charbuf;
	cb2=malloc(65536*16);
	t=cb2;

	k=1;
	l=*s++;
	for(i=1; i<(65536*16); i++)
	{
		if((*s!=l) || (k>65534))
		{
			if(k>3 || (l==0x7f) || (l==0x7e))
			{
				if(k<256)
				{
					*t++=0x7f;
					*t++=k;
					*t++=l;
				}else {
					*t++=0x7e;
					*t++=k&0xff;
					*t++=(k>>8)&0xff;
					*t++=l;
				}
			}else while(k--)*t++=l;

			k=1;
			l=*s++;
		}else
		{
			k++;
			s++;
		}
	}
	if(k>3 || (l==0x7f) || (l==0x7e))
	{
		if(k<256)
		{
			*t++=0x7f;
			*t++=k;
			*t++=l;
		}else {
			*t++=0x7e;
			*t++=k&0xff;
			*t++=(k>>8)&0xff;
			*t++=l;
		}
	}else while(k--)*t++=l;

	fwrite(cb2, 1, (int)t-(int)cb2, out);
#endif

}
