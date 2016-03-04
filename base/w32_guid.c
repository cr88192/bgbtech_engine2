#include <bteifgl.h>

#ifdef _WIN32

#include <windows.h>

int NET_CreateGuid(VGUID *buf)
{
	UUID guid;

	CoCreateGuid(&guid);
//	UuidCreate(&guid);
//	memcpy(buf, &guid, 16);

	buf->data1[0]=guid.Data1>>24;
	buf->data1[1]=guid.Data1>>16;
	buf->data1[2]=guid.Data1>> 8;
	buf->data1[3]=guid.Data1    ;
	buf->data2[0]=guid.Data2>> 8;
	buf->data2[1]=guid.Data2    ;
	buf->data3[0]=guid.Data3>> 8;
	buf->data3[1]=guid.Data3    ;
	memcpy(buf->data4, guid.Data4, 8);

	return(0);
}

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

	return(frgl_strdup(buf));
}

char *NET_String2Guid(VGUID *guid, char *s)
{
	memset(guid, 0, 16);

	if(*s!='{')return(NULL);
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

int NET_GuidIsSixteencc(VGUID *guid)
{
	int i;
	
	for(i=0; i<16; i++)
	{
		if(guid->data1[i]<' ')break;
		if(guid->data1[i]>'~')break;
	}
	return(i>=16);
}

int NET_GuidFromName(VGUID *guid, char *name)
{
	int i, j;

	if(*name=='{')
	{
		NET_String2Guid(guid, name);
		return(0);
	}
	
	if(strlen(name)<=4)
	{
		memset(guid, ' ', 16);

		for(i=0; i<4; i++)
		{
			if(!name[i])break;
			guid->data1[i]=name[i];
		}
		guid->data2[0]=0x00;	guid->data2[1]=0x00;
		guid->data3[0]=0x00;	guid->data3[1]=0x10;
		guid->data4[0]=0x80;	guid->data4[1]=0x00;
		guid->data4[2]=0x00;	guid->data4[3]=0xAA;
		guid->data4[4]=0x00;	guid->data4[5]=0x38;
		guid->data4[6]=0x9B;	guid->data4[7]=0x71;
		return(0);
	}
	
	if(strlen(name)>16)
		return(-1);
	memset(guid, ' ', 16);
	for(i=0; i<16; i++)
	{
		if(!name[i])break;
		guid->data1[i]=name[i];
	}
	return(0);
}

#endif
