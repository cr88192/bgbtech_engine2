#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

struct ar_hdr {
  char ar_name[16];		/* name of this member */
  char ar_date[12];		/* file mtime */
  char ar_uid[6];		/* owner uid; printed as decimal */
  char ar_gid[6];		/* owner gid; printed as decimal */
  char ar_mode[8];		/* file mode, printed as octal   */
  char ar_size[10];		/* file size, printed as decimal */
  char ar_fmag[2];		/* should contain ARFMAG */
};


int main(int argc, char *argv[])
{
	FILE *fd;
	byte *buf, *cs, *ce;
	struct ar_hdr *arh;

	char *s;
	int sz;
	int i, j, k, l;

	printf("open %s\n", argv[1]);
	fd=fopen(argv[1], "rb");

	if(!fd)
	{
		printf("fail open\n");
		return(-1);
	}

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=malloc(sz);
	fread(buf, 1, sz, fd);

	fclose(fd);


	if(strncmp(buf, "!<arch>\n", 8))
	{
		printf("bad arch magic\n");
		return(-1);
	}

	cs=buf+8; ce=buf+sz;

	while(cs<ce)
	{
		arh=(struct ar_hdr *)cs;

		if(arh->ar_fmag[0]!='`')
		{
			printf("Parse Fail @ %X\n", cs-buf);
			break;
		}

		arh->ar_fmag[1]=0;

		i=atoi(arh->ar_size);
		printf("%s %d\n", cs, i);

		if(i&1)i++;
		cs+=60+i;
	}

	return(0);
}
