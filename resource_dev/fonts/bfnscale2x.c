#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

int main(int argc, char *argv[])
{
	byte *charbuf, *map;
	FILE *fd, *ofd;
	char *buf, *outbuf;
	byte *cells, *cmask;
	char head[18];
	int s, e, w, h;
	int i, j, k, l;
	int x, y, rx0, ry0, rx1, ry1, rx2, ry2;
	int cw, ch, csz, cw2, ch2, csz2;
	int pa, pb, pc, pd, pe, pf, pg, ph, pi;
	int p;

	fd=fopen(argv[1], "rb");
	ofd=fopen(argv[2], "wb");

	charbuf=malloc(65536*512);
	map=malloc(65536/8);
	memset(map, 0, 65536/8);

	w=8;
	h=16;
//	wl=0;

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
		csz=(cw*ch+7)/8;
	}else
	{
		printf("no header\n");
		exit(-1);
	}

	while(1)
	{
		s=fgetc(fd);
		s+=fgetc(fd)<<8;

		e=fgetc(fd);
		e+=fgetc(fd)<<8;

		if(!s && !e)break;

		printf("Read %d-%d\n", s, e);

		buf=malloc(((e-s)+1)*csz);
		fread(buf, (e-s)+1, csz, fd);

		for(j=0; j<((e-s)+1); j++)if(!(map[(s+j)/8]&(1<<((s+j)&7))) )
		{
			map[(s+j)/8]|=1<<((s+j)&7);
			memcpy(charbuf+((s+j)*csz), buf+(j*csz), csz);
		}
	}
//	printf("\n");
	fclose(fd);

	cw2=cw*2;
	ch2=ch*2;

	csz=(cw*ch+7)/8;
	csz2=(cw2*ch2+7)/8;

	cells=malloc(65536*csz2);
	cmask=malloc(65536/8);
	memcpy(cmask, map, 65536/8);

	for(i=0; i<65536; i++)
	{
		for(y=0; y<ch2; y++)
			for(x=0; x<cw2; x++)
		{
			rx0=x>>1;
			ry0=y>>1;

//			rx1=(x&1)?((x+1)>>1):((x-1)>>1);
//			ry1=(y&1)?((y+1)>>1):((y-1)>>1);
//			rx2=(x&1)?((x-1)>>1):((x+1)>>1);
//			ry2=(y&1)?((y-1)>>1):((y+1)>>1);

			rx1=(x&1)?((x>>1)+1):((x>>1)-1);
			ry1=(y&1)?((y>>1)+1):((y>>1)-1);
			rx2=(x&1)?((x>>1)-1):((x>>1)+1);
			ry2=(y&1)?((y>>1)-1):((y>>1)+1);

			if((rx1<0) || (rx1>=cw))rx1=rx0;
			if((ry1<0) || (ry1>=cw))ry1=ry0;
			if((rx2<0) || (rx2>=cw))rx2=rx0;
			if((ry2<0) || (ry2>=cw))ry2=ry0;

			j=ry1*cw+rx1;
			pa=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;
			j=ry1*cw+rx0;
			pb=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;
			j=ry1*cw+rx2;
			pe=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;

			j=ry0*cw+rx1;
			pc=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;
			j=ry0*cw+rx0;
			pd=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;
			j=ry0*cw+rx2;
			pf=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;

			j=ry2*cw+rx1;
			pg=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;
			j=ry2*cw+rx0;
			ph=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;
			j=ry2*cw+rx2;
			pi=(charbuf[i*csz+(j>>3)]&(128>>(j&7)))?1:0;
			
			p=pd;
			if(pd)
			{
//				if(!pa && !pb && !pc)pe=0;
			}else
			{
//				if(pa && pb && pc)pe=1;
//				if(pb && pc)p=1;
//				if(pb && pc && !pf && !ph)p=1;
//				if(pb && pc && !pe && !pf && !ph && !pi)p=1;
//				if(pb && pc && !pf && !ph && !pi)p=1;
//				if(pb && pc && !pf && !ph)p=1;
				if(pb && pc && !(pf && ph))p=1;
			}
			
			j=y*cw2+x;
			if(p)
			{
				cells[(i*csz2)+(j>>3)]|=(128>>(j&7));
			}else
			{
				cells[(i*csz2)+(j>>3)]&=~(128>>(j&7));
			}
		}
	}

	cw=cw2;
	ch=ch2;
	csz=(cw*ch+7)/8;

	//write head
	fputc(0x00, ofd);	//0xBF00
	fputc(0xBF, ofd);

	fputc(0x04, ofd);	//head len
	fputc(0x00, ofd);

	fputc(0x00, ofd);
	fputc(cw, ofd);
	fputc(ch, ofd);
	fputc(0x01, ofd);

	s=0;
	e=0;

	for(i=0; i<65536; i++)
	{
		if(cmask[i>>3]&(1<<(i&7)))
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
				printf("W %d-%d\n", s, e);

				fputc(s&0xff, ofd);
				fputc((s>>8)&0xff, ofd);
				fputc(e&0xff, ofd);
				fputc((e>>8)&0xff, ofd);
				fwrite(cells+(s*csz), e-s+1, csz, ofd);
			}
			s=0;
			e=0;
		}
	}
	if(s || e)
	{
		printf("W %d-%d\n", s, e);

		fputc(s&0xff, ofd);
		fputc((s>>8)&0xff, ofd);
		fputc(e&0xff, ofd);
		fputc((e>>8)&0xff, ofd);
		fwrite(cells+(s*csz), e-s+1, csz, ofd);
	}

	//special end of font marker...
	fputc(0, ofd);
	fputc(0, ofd);
	fputc(0, ofd);
	fputc(0, ofd);

	return(0);
}

