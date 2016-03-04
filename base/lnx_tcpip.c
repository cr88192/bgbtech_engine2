//#define NODELAY
//#define BUFFER
//#define CONF_USEIPV6

#include <bgbnet.h>

#ifdef linux

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <errno.h>

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	64
#endif

typedef struct {
int sock;
int cnt;
byte *buf, *bufe;
}tcp_buf;

static byte in6_addr_any[16]=
	{0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0};
static byte in6_addr_loopback[16]=
	{0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1};

int __TCP_OpenSocket (int port)
{
	int newsocket;
	struct sockaddr_in address;
//	int t=1;

	if ((newsocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return -1;

//	ioctl (newsocket, FIONBIO, &t);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if( bind (newsocket, (void *)&address, sizeof(address)) == -1)
		return(-1);

	return(newsocket);
}

#ifdef CONF_USEIPV6

int __TCP_OpenSocket_IPv6 (int port)
{
	int newsocket;
	struct sockaddr_in6 address;
//	int t=1;

	if ((newsocket = socket (PF_INET6, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return -1;

//	ioctl (newsocket, FIONBIO, &t);

	address.sin6_family = AF_INET6;
	memcpy(&address.sin6_addr, &in6_addr_any, 16);
	address.sin6_port = htons(port);
	if( bind (newsocket, (void *)&address, sizeof(address)) == -1)
		return(-1);

	return(newsocket);
}

#endif

/*--
Cat pdnet;NET;Sock;TCP
Form
	int TCP_GetSocketAddr(int socket, VADDR *addrbuf);
Description
	Retrieves the address of a given socket.
Status Internal
--*/
BGBNET_API int TCP_GetSocketAddr(int socket, VADDR *addrbuf)
{
	union {
		struct sockaddr addru;
		struct sockaddr_in address4;
#ifdef CONF_USEIPV6
		struct sockaddr_in6 address6;
		struct sockaddr_storage address_gen;
#endif
	}address;

	int s;

	addrbuf->proto=0;

	s=sizeof(address);
	if(getsockname(socket, (struct sockaddr *)&address, &s))return(-1);

	switch(address.addru.sa_family)
	{
	case AF_INET:
		addrbuf->proto=PROTO_IPV4TCP;
		addrbuf->ipv4.port=ntohs(address.address4.sin_port);
		addrbuf->ipv4.addr=ntohl(address.address4.sin_addr.s_addr);
		break;
#ifdef CONF_USEIPV6
	case AF_INET6:
		addrbuf->proto=PROTO_IPV6TCP;
		addrbuf->ipv6.port=ntohs(address.address6.sin6_port);
		memcpy(addrbuf->ipv6.addr, &address.address6.sin6_addr, 16);
		break;
#endif
	default:
		break;
	}
	return(0);
}

/* VFILE interface */
BGBNET_API VFILE *TCP_WrapSocket(int sock);

// int tcpsock_read(void *buf, int sz, VFILE *sock)
int tcpsock_read(void *buf, int s1, int s2, VFILE *sock) //AH:ignore
{
	int fhnd, stat, sz;
	tcp_buf *inf;

//	printf("try get message.\n");

//	fhnd=*(int *)sock->data;
	inf=sock->data;

	sz=recv(inf->sock, buf, s1*s2, 0);

#if 0
	if(sz==-1)
	{
		stat=WSAGetLastError();
		switch(stat)
		{
		case WSAEWOULDBLOCK:
			sz=0;
			break;
		case WSAECONNRESET:
		case WSAECONNABORTED:
			gc_printf("TCP: Read: connection reset\n");
			break;
		default:
			gc_printf("TCP: Read: error %d\n", stat);
			break;
		}
	}
#endif

	return(sz);
}

//int tcpsock_write(void *msg, int len, VFILE *sock)
int tcpsock_write(void *msg, int s1, int s2, VFILE *sock)
{
	int i, stat, sz;
	tcp_buf *inf;

//	gc_printf("TCP: write %d\n", len);

	inf=sock->data;

	inf->cnt+=s1*s2;

#ifdef BN_TCP_USEBUFFER
	i=inf->bufe-inf->buf;
	if((i+len)<4096)
	{
		memcpy(inf->bufe, msg, len);
		inf->bufe+=len;
		return(len);
	}

	i=inf->bufe-inf->buf;
	if(i)send(inf->sock, inf->buf, i, 0);
#endif
	inf->bufe=inf->buf;

	i=send(inf->sock, msg, s1*s2, 0);

//		if(i==-1)gc_printf("TCP: write error %d on %d\n",
//			WSAGetLastError(), *(int *)sock->data);

#if 0
	if(i==-1)
	{
		stat=WSAGetLastError();
		switch(stat)
		{
		case WSAEWOULDBLOCK:
			break;
		case WSAECONNRESET:
			gc_printf("TCP: Write: connection reset\n");
			return(-1);
			break;
		default:
			gc_printf("TCP: Write: error %d\n", stat);
			return(-1);
			break;
		}
	}
#endif
	return(i);
}

int tcpsock_flush(VFILE *sock)
{
	tcp_buf *inf;
	int i;
	int stat;

	inf=sock->data;

	i=inf->bufe-inf->buf;
	if(i)
	{
		i=send(inf->sock, inf->buf, i, 0);
		inf->bufe=inf->buf;

//		gc_printf("TCP flush: %d bytes t:%d\n", inf->cnt, Sys_TimeMS());
		inf->cnt=0;
	}

#if 0
	if(i==-1)
	{
		stat=WSAGetLastError();
		switch(stat)
		{
		case WSAEWOULDBLOCK:
			break;
		case WSAECONNRESET:
			gc_printf("TCP: Write: connection reset\n");
			return(-1);
			break;
		default:
			gc_printf("TCP: Write: error %d\n", stat);
			return(-1);
			break;
		}
	}
#endif
	return(i);
}

#if 0
int tcpsock_ioctls(VFILE *fd, void **arg)
{
	char *s;
	int sock;
	union {
		struct sockaddr addru;
		struct sockaddr_in address4;
#ifdef CONF_USEIPV6
		struct sockaddr_in6 address6;
#endif
	}address;
	int sz, stat, t;
	fd_set rfds;
	struct timeval tv;
	VADDR *addrbuf;

	s=*arg;
	if(!strcmp(s, "accept"))
	{
//		gc_printf("TCP: accept\n");

		sz=sizeof(address);
		sock=accept(*(int *)fd->data, (struct sockaddr *)&address, &sz);


		if(sock==-1)
		{
			*(VFILE **)arg[1]=NULL;
			return(0);
		}

		gc_printf("TCP: new con, sock %d\n", sock);

		addrbuf=(VADDR *)arg[2];

		switch(address.addru.sa_family)
		{
		case AF_INET:
			addrbuf->proto=PROTO_IPV4TCP;
			addrbuf->ipv4.port=address.address4.sin_port;
			addrbuf->ipv4.addr=address.address4.sin_addr.s_addr;
			break;
#ifdef CONF_USEIPV6
		case AF_INET6:
			addrbuf->proto=PROTO_IPV6TCP;
			addrbuf->ipv6.port=address.address6.sin6_port;
			memcpy(addrbuf->ipv6.addr, &address.address6.sin6_addr, 16);
			break;
#endif
		default:
			break;
		}

		*(VFILE **)arg[1]=TCP_WrapSocket(sock);
		return(0);
	}

	if(!strcmp(s, "can-write?"))
	{
		FD_ZERO(&rfds);
		FD_SET(*(int *)fd->data, &rfds);
		tv.tv_sec=*(int *)arg[1];
		tv.tv_usec=0;

		stat=select(1, &rfds, NULL, NULL, &tv);

		if(!stat)
		{
			*(int *)arg[1]=0; //false
			return(0);
		}
		*(int *)arg[1]=-1;
		return(0);
	}

	return(-1);
}
#endif

#if 1
int tcpsock_ioctl(VFILE *fd, int req, void *arg)
{
	char *s;
	int sock;
	union {
		struct sockaddr addru;
		struct sockaddr_in address4;
#ifdef CONF_USEIPV6
		struct sockaddr_in6 address6;
#endif
	}address;
	int sz, stat, t;
	fd_set rfds;
	struct timeval tv;
	VADDR *addrbuf;
	void **argv;

	if(req==VFSOCK_IOCTL_ACCEPT)
	{
		sz=sizeof(address);
		sock=accept(*(int *)fd->data, (struct sockaddr *)&address, &sz);

		argv=(void **)arg;

		if(sock<0)
		{
			*(VFILE **)argv[0]=NULL;
			return(0);
		}

		gc_printf("TCP: new con, sock %d\n", sock);

#if 1
		addrbuf=(VADDR *)argv[1];
		switch(address.addru.sa_family)
		{
		case AF_INET:
			addrbuf->proto=PROTO_IPV4TCP;
			addrbuf->ipv4.port=address.address4.sin_port;
			addrbuf->ipv4.addr=address.address4.sin_addr.s_addr;
			break;
#ifdef CONF_USEIPV6
		case AF_INET6:
			addrbuf->proto=PROTO_IPV6TCP;
			addrbuf->ipv6.port=address.address6.sin6_port;
			memcpy(addrbuf->ipv6.addr, &address.address6.sin6_addr, 16);
			break;
#endif
		default:
			break;
		}
#endif

		*(VFILE **)argv[0]=TCP_WrapSocket(sock);
		return(0);
	}

	if(req==VFSOCK_IOCTL_CANWRITE)
	{
		FD_ZERO(&rfds);
		FD_SET(*(int *)fd->data, &rfds);
		tv.tv_sec=*(int *)arg;
		tv.tv_usec=0;

		stat=select(1, &rfds, NULL, NULL, &tv);

		if(!stat)
		{
			*(int *)arg=0; //false
			return(0);
		}
		*(int *)arg=-1;
		return(0);
	}

	return(-1);
}
#endif

int tcpsock_close(VFILE *sock)
{
	int fhnd, stat;

//	printf("try get message.\n");

	fhnd=*(int *)sock->data;

	close(fhnd);
	gcfree(sock);

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

static tcp_buf tcp_slot[1024];
static int tcp_slots=0;

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

/*--
Cat pdnet;NET;Sock;TCP
Form
	VFILE *TCP_WrapSocket(int sock);
Description
	Wraps a TCP socket in a VFILE.
--*/
BGBNET_API VFILE *TCP_WrapSocket(int sock)
{
	VFILE *tmp;
	tcp_buf *hnd;
	int t, i;

	t=1;
	ioctl(sock, FIONBIO, &t);

	tmp=vf_new();
	hnd=&tcp_slot[tcp_slots++];
	tmp->data=hnd;
	hnd->sock=sock;
	hnd->buf=gcalloc(4096);
	hnd->bufe=hnd->buf;

//	tmp->read_proc=&tcpsock_read;
//	tmp->write_proc=&tcpsock_write;
//	tmp->ioctls_proc=&tcpsock_ioctls;
//	tmp->flush_proc=&tcpsock_flush;
//	tmp->close_proc=&tcpsock_close;

	tmp->iface=&vfsock_iface;

	return(tmp);
}

/*--
Cat pdnet;NET;Sock;TCP
Form
	VFILE *TCP_OpenSocket(int port);
Description
	Opens a socket on a given port.
--*/
BGBNET_API VFILE *TCP_OpenSocket(int port)
{
	VFILE *tmp;
	tcp_buf *hnd;
	int sock;
	int t=1;
	int i;

	sock=__TCP_OpenSocket(port);
	if(sock==-1)
	{
		gc_printf("socket creation error.\n");
		return(NULL);
	}

	ioctl(sock, FIONBIO, &t);

	tmp=vf_new();
	hnd=&tcp_slot[tcp_slots++];
	tmp->data=hnd;
	hnd->sock=sock;
	hnd->buf=gcalloc(4096);
	hnd->bufe=hnd->buf;

//	tmp->read_proc=&tcpsock_read;
//	tmp->write_proc=&tcpsock_write;
//	tmp->ioctls_proc=&tcpsock_ioctls;
//	tmp->flush_proc=&tcpsock_flush;
//	tmp->close_proc=&tcpsock_close;

	tmp->iface=&vfsock_iface;

	return(tmp);
}

/*--
Cat pdnet;NET;Sock;TCP
Form
	VFILE *TCP_OpenListen(int port);
Description
	Opens a listening socket on a given port.
--*/
BGBNET_API VFILE *TCP_OpenListen(int port)
{
	VFILE *tmp;
	tcp_buf *hnd;
	int sock;
	int t=1;
	int i;

	sock=__TCP_OpenSocket(port);
	if(sock==-1)
	{
		gc_printf("socket creation error.\n");
		return(NULL);
	}

	listen(sock, 5);

	ioctl(sock, FIONBIO, &t);

	tmp=vf_new();
	hnd=&tcp_slot[tcp_slots++];
	tmp->data=hnd;
	hnd->sock=sock;
	hnd->buf=gcalloc(4096);
	hnd->bufe=hnd->buf;

//	tmp->read_proc=&tcpsock_read;
//	tmp->write_proc=&tcpsock_write;
//	tmp->ioctls_proc=&tcpsock_ioctls;
//	tmp->flush_proc=&tcpsock_flush;
//	tmp->close_proc=&tcpsock_close;

	tmp->iface=&vfsock_iface;

	return(tmp);
}

#ifdef CONF_USEIPV6
BGBNET_API VFILE *TCP_OpenListen_IPv6(int port)
{
	VFILE *tmp;
	tcp_buf *hnd;
	int sock;
	int t=1;
	int i;

	sock=__TCP_OpenSocket_IPv6(port);
	if(sock==-1)
	{
		gc_printf("socket creation error.\n");
		return(NULL);
	}

	listen(sock, 5);

	return(TCP_WrapSocket(sock));
}
#endif

/*--
Cat pdnet;NET;Sock;TCP
Form
	VFILE *TCP_OpenConnect(VADDR *targ);
Description
	Tries to connect to targ, returning the new connection.
--*/
BGBNET_API VFILE *TCP_OpenConnect(VADDR *targ)
{
	VFILE *tmp;
	tcp_buf *hnd;
	int sock, stat;
	int t=1;
	int i;

	struct sockaddr_in name4;
#ifdef CONF_USEIPV6
	struct sockaddr_in6 name6;
#endif

	sock=__TCP_OpenSocket(0);
	if(sock==-1)
	{
		gc_printf("socket creation error.\n");
		return(NULL);
	}

	stat=-1;

	if((targ->proto==PROTO_IPV4UDP) || (targ->proto==PROTO_IPV4TCP))
	{
		gc_printf("connect ipv4 %s\n", NET_Addr2Str(targ));

		name4.sin_family = AF_INET;
		name4.sin_addr.s_addr = targ->ipv4.addr;
		name4.sin_port = targ->ipv4.port;

		stat=connect(sock, (struct sockaddr *)&name4, sizeof(name4));
	}
#ifdef CONF_USEIPV6
	else if((targ->proto==PROTO_IPV6UDP) ||
		(targ->proto==PROTO_IPV6TCP))
	{
		gc_printf("connect ipv6 %s\n", NET_Addr2Str(targ));

		name6.sin6_family = AF_INET6;
		memcpy(&name6.sin6_addr, targ->ipv6.addr, 16);
		name6.sin6_port = targ->ipv6.port;

		stat=connect(sock, (struct sockaddr *)&name6, sizeof(name6));
	}
#endif

	if(stat == -1)
	{
//		gc_printf("socket connect fail %d.\n", WSAGetLastError());
		close(sock);
		return(NULL);
	}

	ioctl(sock, FIONBIO, &t);

	tmp=vf_new();
	hnd=&tcp_slot[tcp_slots++];
	tmp->data=hnd;
	hnd->sock=sock;
	hnd->buf=gcalloc(4096);
	hnd->bufe=hnd->buf;

//	tmp->read_proc=&tcpsock_read;
//	tmp->write_proc=&tcpsock_write;
//	tmp->ioctls_proc=&tcpsock_ioctls;
//	tmp->flush_proc=&tcpsock_flush;
//	tmp->close_proc=&tcpsock_close;

	tmp->iface=&vfsock_iface;

	return(tmp);
}

#endif
