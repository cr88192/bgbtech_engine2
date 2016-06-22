#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	unsigned char *charbuf, *map;

	FILE *fd, *ofd;
	char *buf;
	int s, e;
	int i, j;
	int w, h, wl;

	charbuf=malloc(65536*128);
	map=malloc(65536/8);
	memset(map, 0, 65536/8);

	w=8;
	h=16;
	wl=0;

	for(i=2; i<argc; i++)
	{
		printf("%s\n", argv[i]);
		fd=fopen(argv[i], "rb");

		s=fgetc(fd);
		s+=fgetc(fd)<<8;

		e=fgetc(fd);
		e+=fgetc(fd)<<8;

		if(s==0xBF00)
		{
			buf=malloc(16);
			fread(buf, 1, e, fd);
			if(!wl)
			{
				w=buf[1];
				h=buf[2];
				printf("head %dx%d\n", w, h);
				wl=1;
			}else if((buf[1]!=w) || (buf[2]!=h))
			{
				printf("font cell dimension mismatch\n");
				exit(-1);
			}
		}else
		{
			printf("missing head.\n");
			exit(-1);
		}

		while(1)
		{
			s=fgetc(fd);
			s+=fgetc(fd)<<8;

			e=fgetc(fd);
			e+=fgetc(fd)<<8;

			if(!s && !e)break;

//			printf("%d-%d ", s, e);

			buf=malloc(((e-s)+1)*((w*h)/8));
			fread(buf, (e-s)+1, (w*h)/8, fd);

			for(j=0; j<((e-s)+1); j++)if(!(map[(s+j)/8]&(1<<((s+j)&7))) )
			{
				map[(s+j)/8]|=1<<((s+j)&7);
				memcpy(charbuf+((s+j)*((w*h)/8)), buf+(j*((w*h)/8)), ((w*h)/8));
			}
		}
//		printf("\n");
		fclose(fd);
	}

	ofd=fopen(argv[1], "wb");

	//write head
	fputc(0x00, ofd);	//0xBF00
	fputc(0xBF, ofd);

	fputc(0x04, ofd);	//head len
	fputc(0x00, ofd);

	fputc(0x00, ofd);
	fputc(w, ofd);
	fputc(h, ofd);
	fputc(0x01, ofd);

	s=0;
	e=0;

	for(i=0; i<65536; i++)
	{
		if(map[i/8]&(1<<(i&7)))
		{
			if(!s)
			{
				s=i;
				e=i;
			}else e=i;
		}else
		{
			if(s || e)
			{
//				printf("%d-%d ", s, e);

				fputc(s&0xff, ofd);
				fputc((s>>8)&0xff, ofd);
				fputc(e&0xff, ofd);
				fputc((e>>8)&0xff, ofd);
				fwrite(charbuf+(s*((w*h)/8)), e-s+1, ((w*h)/8), ofd);
			}
			s=0;
			e=0;
		}
	}
	if(s || e)
	{
//		printf("%d-%d ", s, e);

		fputc(s&0xff, ofd);
		fputc((s>>8)&0xff, ofd);
		fputc(e&0xff, ofd);
		fputc((e>>8)&0xff, ofd);
		fwrite(charbuf+(s*((w*h)/8)), e-s+1, ((w*h)/8), ofd);
	}

	//special end of font marker...
	fputc(0, ofd);
	fputc(0, ofd);
	fputc(0, ofd);
	fputc(0, ofd);

//	fwrite(outbuf, 256*16, 256*8*3, ofd);

	return(0);
}
