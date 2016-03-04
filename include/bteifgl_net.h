#ifndef BTEIFGL_NET_H
#define BTEIFGL_NET_H

typedef struct {
byte data1[4];
byte data2[2];
byte data3[2];
byte data4[8];
}VGUID;

#define PROTO_IPV4	(1<<8)
#define	PROTO_IPV6	(2<<8)
#define PROTO_TCP	1
#define	PROTO_UDP	2

#define PROTO_UNDEFINED	0
#define PROTO_IPV4UDP	(PROTO_IPV4|PROTO_UDP)
#define PROTO_IPV4TCP	(PROTO_IPV4|PROTO_TCP)
#define PROTO_IPV6UDP	(PROTO_IPV6|PROTO_UDP)
#define PROTO_IPV6TCP	(PROTO_IPV6|PROTO_TCP)

/* these may be loaded in the lower 8 flag bits for send and similar */
#define PROTO_PF_UNDEFINED	0

#define PROTO_PF_IPV4		4
#define PROTO_PF_TCP		6
#define PROTO_PF_UDP		17

#define PROTO_PF_IPV6		41
#define PROTO_PF_IPV6ROUTE	43
#define PROTO_PF_IPV6FRAG	44
#define PROTO_PF_IPV6ICMP	58
#define PROTO_PF_IPV6NONXT	59
#define PROTO_PF_IPV6OPTS	60

typedef union VADDR_u VADDR;

union VADDR_u {
	int proto;

	struct {
	int proto;
	int flags; /* lower 8 bits=netmask count */
	unsigned short port;
	unsigned long addr;
	}ipv4;

	struct {
	int proto;
	int flags; /* lower 8 bits=netmask count */
	unsigned short port;
	byte addr[16];
	}ipv6;
};

typedef struct {
VGUID nodeid;
VADDR addr;
u32 nodefl;
int id;
}BTEIFGL_PeerInfo;

#endif
