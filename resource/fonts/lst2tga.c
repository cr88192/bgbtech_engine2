#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fd, *lfd, *ofd;
	char *buf, *outbuf;
	unsigned char *charbuf, *map;
	char head[18], lbuf[256];
	int s, e, w, h;
	int i, j, k, l;
	int x, y, rx, ry;
	int cw, ch;

	fd=fopen(argv[1], "rb");
	lfd=fopen(argv[2], "rt");
	ofd=fopen(argv[3], "wb");

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

	outbuf=malloc(256*cw*ch*3);
	memset(outbuf, 0, 256*cw*ch*3);

	charbuf=malloc(65536*128);
	map=malloc(65536/8);
	memset(map, 0, 65536/8);

	while(1)
	{
		s=fgetc(fd);
		s+=fgetc(fd)<<8;

		e=fgetc(fd);
		e+=fgetc(fd)<<8;

		if(!s && !e)break;

//		printf("%d-%d ", s, e);

		buf=malloc(((e-s)+1)*((cw*ch)/8));
		fread(buf, (e-s)+1, (cw*ch)/8, fd);

		for(j=0; j<((e-s)+1); j++)if(!(map[(s+j)/8]&(1<<((s+j)&7))) )
		{
			map[(s+j)/8]|=1<<((s+j)&7);
			memcpy(charbuf+((s+j)*((cw*ch)/8)), buf+(j*((cw*ch)/8)), ((cw*ch)/8));
		}
	}

	w=16*cw;
	h=16*ch;

	memset(head, 0, 18);
	head[2]=2;
	head[12]=w&0xff;
	head[13]=w>>8;
	head[14]=h&0xff;
	head[15]=h>>8;
	head[16]=24;

	fwrite(head, 1, 18, ofd);

	while(!feof(lfd))
	{
		memset(lbuf, 0, 256);
		fgets(lbuf, 255, lfd);
		if(lbuf[0]!='=')continue;

		sscanf(lbuf, "=%X U+%X", &i, &j);
		printf("map %X->%X\n", j, i);

		x=(i&0xf)*cw;
		y=(i>>4)*ch;
		for(k=0; k<((cw*ch)/8); k++)for(l=0; l<8; l++)
		{
			if(charbuf[(j*((cw*ch)/8))+k]&(128>>l))
			{
				rx=((k*8)+l)%cw;
				ry=((k*8)+l)/cw;
				outbuf[(((h-(y+ry)-1)*w)+(x+rx))*3]=0xff;
				outbuf[(((h-(y+ry)-1)*w)+(x+rx))*3+1]=0xff;
				outbuf[(((h-(y+ry)-1)*w)+(x+rx))*3+2]=0xff;
			}
		}
	}
	fwrite(outbuf, 16*ch, 16*cw*3, ofd);

	return(0);
}
