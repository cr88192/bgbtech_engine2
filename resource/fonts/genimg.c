/*
	Written by Brendan G Bohannon
	Free for use for whatever purpose...

	This was written sometime before 2003, but I don't know when...
	This converts BFN fonts into a huge TGA image...
*/

#include <stdio.h>
#include <stdlib.h>

struct tgahead_s {
unsigned char id_len, ctype, itype;
short cindex, clength;
unsigned char csize;
unsigned short x_origin, y_origin, width, height;
unsigned char bpp, attrib;
};

int main(int argc, char *argv[])
{
	FILE *fd, *ofd;
	char *buf, *outbuf;
	char head[18];
	int s, e, w, h;
	int i, j, k, l;
	int x, y, rx, ry;
	int cw, ch;

	fd=fopen(argv[1], "rb");
	ofd=fopen(argv[2], "wb");

	s=fgetc(fd);
	s+=fgetc(fd)<<8;
	e=fgetc(fd);
	e+=fgetc(fd)<<8;

	if(s==0xBF00)
	{
		buf=malloc(16);
		fread(buf, 1, e, fd);
		cw=buf[1];
		ch=buf[2];
	}else
	{
		printf("no header\n");
		exit(-1);
	}

	outbuf=malloc(65536*cw*ch*3);
	memset(outbuf, 0, 65536*cw*ch*3);

	w=256*cw;
	h=256*ch;

	memset(head, 0, 18);
	head[2]=2;
	head[12]=w&0xff;
	head[13]=w>>8;
	head[14]=h&0xff;
	head[15]=h>>8;
	head[16]=24;

	fwrite(head, 1, 18, ofd);

	while(1)
	{
		s=fgetc(fd);
		s+=fgetc(fd)<<8;

		e=fgetc(fd);
		e+=fgetc(fd)<<8;

		if(!s && !e)break;

		printf("chunk %d-%d\n", s, e);

		buf=malloc(((e-s)+1)*((cw*ch)/8));
		fread(buf, (e-s)+1, ((cw*ch)/8), fd);

		for(i=0; i<((e-s)+1); i++)
		{
			j=i+s;
			x=(j&0xff)*cw;
			y=(j>>8)*ch;
			for(k=0; k<((cw*ch)/8); k++)for(l=0; l<8; l++)
			{
				if(buf[(i*((cw*ch)/8))+k]&(128>>l))
				{
					rx=((k*8)+l)%cw;
					ry=((k*8)+l)/cw;
					outbuf[(((h-(y+ry)-1)*w)+(x+rx))*3]=0xff;
					outbuf[(((h-(y+ry)-1)*w)+(x+rx))*3+1]=0xff;
					outbuf[(((h-(y+ry)-1)*w)+(x+rx))*3+2]=0xff;
				}
			}
		}
	}
	fwrite(outbuf, 256*ch, 256*cw*3, ofd);

	return(0);
}
