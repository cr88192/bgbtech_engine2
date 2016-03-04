#include <bteifgl.h>

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	64
#endif

char local_hostname[MAXHOSTNAMELEN];
unsigned long local_ipv4addr;


int NET_CreateGuid(VGUID *buf)
{
	byte tb[16];
	int i;

	for(i=0; i<16; i++)tb[i]=rand()&0xff;
	memcpy(buf, tb, 16);

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

	return(dystrdup(buf));
}

char *NET_String2Guid(VGUID *guid, char *s)
{
	memset(guid, 0, 16);

	if(*s!='{')return(s);
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


ushort kntohs(uint i)

{
	return((i>>8)|(i<<8));
}

ushort khtons(uint i)
{
	return((i>>8)|(i<<8));
}

uint kntohl(uint i)
{
	return((i>>24)|(i<<24)|((i>>8)&0xFF00)|
		((i<<8)&0xFF0000));
}

uint khtonl(uint i)
{
	return((i>>24)|(i<<24)|((i>>8)&0xFF00)|
		((i<<8)&0xFF0000));
}

char *ipv4tostr(uint addr)
{
	uint addrc;
	static char buf[4][16], slot=0;
	addrc=kntohl(addr);
	slot%=4;
	sprintf(buf[slot++], "%d.%d.%d.%d", (addrc>>24)&0xff, (addrc>>16)&0xff,
		(addrc>>8)&0xff, addrc&0xff);
	return(buf[slot-1]);
}

char *ipv6tostr(byte *addr)
{
	unsigned short *addrc;
	char *buf, *s;
	int i;

	addrc=(unsigned short *)addr;
	buf=gcralloc(64);

//	sprintf(buf, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", 
//		ntohs(addrc[0]), ntohs(addrc[1]), ntohs(addrc[2]), ntohs(addrc[3]),
//		ntohs(addrc[4]), ntohs(addrc[5]), ntohs(addrc[6]), ntohs(addrc[7]));

	s=buf;
	i=0;

	while(kntohs(addrc[i]))
	{
		if(i>=8)break;
		if(i)*s++=':';
		sprintf(s, "%x", kntohs(addrc[i]));
		s+=strlen(s);
		i++;
	}
	if((i<8) && !kntohs(addrc[i]))
	{
		*s++=':';
		*s=0;

		while(!kntohs(addrc[i]))
			i++;
	}
	while(kntohs(addrc[i]))
	{
		if(i>=8)break;
		if(i)*s++=':';
		sprintf(s, "%x", kntohs(addrc[i]));
		s+=strlen(s);
		i++;
	}

	return(buf);
}

char *NET_Addr2Str(VADDR *addr)
{
	char *s, *t;
	int addrc;

	switch(addr->proto)
	{
	case PROTO_IPV4UDP:
	case PROTO_IPV4TCP:
		s=gcralloc(64);

		addrc=kntohl(addr->ipv4.addr);
		sprintf(s, "%d.%d.%d.%d:%d",
			(addrc>>24)&0xff, (addrc>>16)&0xff,
			(addrc>>8)&0xff, addrc&0xff,
			kntohs(addr->ipv4.port));
		break;
	case PROTO_IPV6UDP:
	case PROTO_IPV6TCP:
		s=ipv6tostr(addr->ipv6.addr);
		t=gcralloc(strlen(s)+10);
		sprintf(t, "[%s]:%d", s, kntohs(addr->ipv6.port));
		break;
	default:
		s=gcralloc(1);
		*s=0;
		break;
	}
	return(s);
}

static int str2ipv4(char *s, int *port)
{
	int a, b, c, d;

	sscanf(s, "%d.%d.%d.%d:%d", &a, &b, &c, &d, port);
	return((a<<24)+(b<<16)+(c<<8)+d);
}

static int ishexnum(char c)
{
	if((c>='0') && (c<='9'))return(1);
	if((c>='A') && (c<='F'))return(1);
	if((c>='a') && (c<='f'))return(1);
	return(0);
}

static int hexval(char c)
{
	if((c>='0') && (c<='9'))return(c-'0');
	if((c>='A') && (c<='F'))return((c-'A')+10);
	if((c>='a') && (c<='f'))return((c-'a')+10);
	return(0);
}

static char *str2ipv6(byte *addr, char *s)
{
	short buf1[16], buf2[16];
	short *t1, *t2, *t3;
	int i;

	char buf[16];
	char *t;

	memset(buf1, 0, 16);
	memset(buf2, 0, 16);

	if(*s=='[')s++;

	t1=buf1;
	while(*s)
	{
		t=buf;
		while(ishexnum(*s))*t++=*s++;
		*t++=0;

		if(buf[0])
		{
			i=0;
			t=buf;
			while(*t)i=(i<<4)+hexval(*t++);
			*t1++=i;
		}
		if(*s!=':')break;
		s++;
		if(*s==':')
		{
			s++;
			break;
		}
	}

	t1=buf2;
	while(*s)
	{
		t=buf;
		while(ishexnum(*s))*t++=*s++;
		*t++=0;

		if(buf[0])
		{
			i=0;
			t=buf;
			while(*t)i=(i<<4)+hexval(*t++);
			*t1++=i;
		}
		if(*s!=':')break;
		s++;
		if(*s==':')
		{
			s++;
			break;
		}
	}

	if(*s==']')s++;

	t2=buf1+(8-(t1-buf2));
	t3=buf2;
	while(t3<t1)*t2++=*t3++;

	t=addr;
	for(i=0; i<8; i++)
	{
		*t++=buf1[i]>>8;
		*t++=buf1[i]&0xff;
	}

	return(s);
}

VADDR *NET_Str2Addr(char *str, int proto)
{
	VADDR *tmp;
	char *s;

	switch(proto)
	{
	case PROTO_IPV4UDP:
	case PROTO_IPV4TCP:
		tmp=gcalloc(sizeof(VADDR));
		tmp->proto=proto;

		tmp->ipv4.addr=khtonl(str2ipv4(str, &tmp->ipv4.port));
		tmp->ipv4.port=khtons(tmp->ipv4.port);
		break;
	case PROTO_IPV6UDP:
	case PROTO_IPV6TCP:
		tmp=gcalloc(sizeof(VADDR));
		tmp->proto=proto;

		s=str2ipv6(tmp->ipv6.addr, str);
		sscanf(s, ":%d", &tmp->ipv6.port);
		tmp->ipv6.port=khtons(tmp->ipv6.port);
		break;
	default:
		tmp=NULL;
		break;
	}
	return(tmp);
}

int NET_InitLow (void)
{
	static int started=0;

	if(started)return(0);
	started=1;

	printf("Net Init Low (Winsock).\n");

}

VADDR *NET_LookupHost(char *name)
{
	return(NULL);
}


VADDR *NET_DecodeHostname(char *name)
{
	VADDR *tmp;
	if(name[0]=='[')
	{
		tmp=NET_Str2Addr(name, PROTO_IPV6TCP);
		return(tmp);
	}
	if((name[0]>='0') && (name[0]<='9'))
	{
		tmp=NET_Str2Addr(name, PROTO_IPV4TCP);
		return(tmp);
	}

	tmp=NET_LookupHost(name);
	return(tmp);
}

int TCP_GetSocketAddr(int socket, VADDR *addrbuf)
{
	return(0);
}

/* VFILE interface */
VFILE *TCP_WrapSocket(int sock);

int tcpsock_read(void *buf, int s1, int s2, VFILE *sock)
{
	return(0);
}

int tcpsock_write(void *msg, int s1, int s2, VFILE *sock)
{
	return(0);
}

int tcpsock_flush(VFILE *sock)
{
	return(0);
}

int tcpsock_ioctl(VFILE *fd, int req, void *arg)
{
	return(-1);
}

int tcpsock_close(VFILE *sock)
{
	return(0);
}


VADDR *tcpsock_getaddr(VFILE *sock)
{
	VADDR *tmp;
	tmp=gcalloc(sizeof(VADDR));
	if(TCP_GetSocketAddr(*(int *)sock->data, tmp))
	{
		gcfree(tmp);
		tmp=NULL;
	}
	return(tmp);
}

VFILE_Interface vfsock_iface={
NULL,
"socket",
tcpsock_read,	//read
tcpsock_write,	//write
NULL,		//getc
NULL,		//putc
tcpsock_flush,	//flush
NULL,		//seek
NULL,		//tell
NULL,		//eof
tcpsock_close,	//close
tcpsock_ioctl	//ioctl
};

VFILE *TCP_WrapSocket(int sock)
{
	return(NULL);
}

VFILE *TCP_OpenSocket(int port)
{
	return(NULL);
}

VFILE *TCP_OpenListen(int port)
{
	return(NULL);
}


VFILE *TCP_OpenConnect(VADDR *targ)
{
	return(NULL);
}

int UDP_GetSocketAddr(int socket, VADDR *addrbuf)
{
	return(0);
}
