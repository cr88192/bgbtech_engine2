#include <bgbnet.h>

#ifdef linux

/*--
Cat pdnet;NET;Sock
Form
	int NET_CreateGuid(VGUID *buf);
Description
	Create a new guid and put it into buf.
--*/
int NET_CreateGuid(VGUID *buf)
{
	byte tb[16];
	int i;

	for(i=0; i<16; i++)tb[i]=rand()&0xff;

	memcpy(buf, tb, 16);

	return(0);
}

/*--
Cat pdnet;NET;Sock
Form
	int NET_GuidEqualP(VGUID *a, VGUID *b);
Description
	Returns nonzero if the GUID's are equal.
--*/
int NET_GuidEqualP(VGUID *a, VGUID *b)
{
	return(!memcmp(a, b, 16));
}

static char *print_byte(char *s, int v)
{
	if((v>>4)<10)*s++='0'+(v>>4);
		else *s++='a'+((v>>4)-10);
	if((v&0xf)<10)*s++='0'+(v&0xf);
		else *s++='a'+((v&0xf)-10);
	return(s);
}

static char *parse_byte(char *s, byte *v)
{
	*v=0;

	if(!*s)return(s);

	if((*s>='0') && (*s<='9'))*v+=*s-'0';
	if((*s>='a') && (*s<='f'))*v+=*s-'a'+10;
	if((*s>='A') && (*s<='F'))*v+=*s-'A'+10;
	s++;

	if(!*s)return(s);

	*v<<=4;
	if((*s>='0') && (*s<='9'))*v+=*s-'0';
	if((*s>='a') && (*s<='f'))*v+=*s-'a'+10;
	if((*s>='A') && (*s<='F'))*v+=*s-'A'+10;
	s++;

	return(s);
}

/*--
Cat pdnet;NET;Sock
Form
	char *NET_Guid2String(VGUID *guid);
Description
	Returns a string containing a textual representation of the guid.
	It is formatted as: {AABBCCDD-EEFF-GGHH-IIJJ-KKLLMMNNOOPP}
	Allocated using 'gcalloc'.
--*/
char *NET_Guid2String(VGUID *guid)
{
	char buf[256];
	char *s;

	s=buf;
	*s++='{';
	s=print_byte(s, guid->data1[0]);
	s=print_byte(s, guid->data1[1]);
	s=print_byte(s, guid->data1[2]);
	s=print_byte(s, guid->data1[3]);
	*s++='-';
	s=print_byte(s, guid->data2[0]);
	s=print_byte(s, guid->data2[1]);
	*s++='-';
	s=print_byte(s, guid->data3[0]);
	s=print_byte(s, guid->data3[1]);
	*s++='-';
	s=print_byte(s, guid->data4[0]);
	s=print_byte(s, guid->data4[1]);
	*s++='-';
	s=print_byte(s, guid->data4[2]);
	s=print_byte(s, guid->data4[3]);
	s=print_byte(s, guid->data4[4]);
	s=print_byte(s, guid->data4[5]);
	s=print_byte(s, guid->data4[6]);
	s=print_byte(s, guid->data4[7]);
	*s++='}';
	*s++=0;

	return(dystrdup(buf));
}

/*--
Cat pdnet;NET;Sock
Form
	char *NET_String2Guid(VGUID *guid, char *s);
Description
	Converts a string to a GUID, filling in guid.
	It is formatted as: {AABBCCDD-EEFF-GGHH-IIJJ-KKLLMMNNOOPP}
	Returns a pointer to the end of the GUID.
--*/
char *NET_String2Guid(VGUID *guid, char *s)
{
	memset(guid, 0, 16);

	if(*s!='{')return(-1);
	s++;

	s=parse_byte(s, guid->data1+0);
	s=parse_byte(s, guid->data1+1);
	s=parse_byte(s, guid->data1+2);
	s=parse_byte(s, guid->data1+3);
	*s++;
	s=parse_byte(s, guid->data2+0);
	s=parse_byte(s, guid->data2+1);
	*s++;
	s=parse_byte(s, guid->data3+0);
	s=parse_byte(s, guid->data3+1);
	*s++;
	s=parse_byte(s, guid->data4+0);
	s=parse_byte(s, guid->data4+1);
	*s++;
	s=parse_byte(s, guid->data4+2);
	s=parse_byte(s, guid->data4+3);
	s=parse_byte(s, guid->data4+4);
	s=parse_byte(s, guid->data4+5);
	s=parse_byte(s, guid->data4+6);
	s=parse_byte(s, guid->data4+7);
	*s++;

	return(s);
}

#endif
