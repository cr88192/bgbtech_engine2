#include <bteifgl.h>

// #define MAXMSG 512
#define MAXMSG 1472

#define TCC_PI FRGL_MAKETCC('P', 'I')
#define TCC_PA FRGL_MAKETCC('P', 'A')

#define FCC_PING FRGL_MAKEFCC('P', 'I', 'N', 'G')
#define FCC_PIAK FRGL_MAKEFCC('P', 'I', 'A', 'K')
#define FCC_RCTR FRGL_MAKEFCC('R', 'C', 'T', 'R')
#define FCC_RCAK FRGL_MAKEFCC('R', 'C', 'A', 'K')
#define FCC_CTRL FRGL_MAKEFCC('C', 'T', 'R', 'L')
#define FCC_IDLE FRGL_MAKEFCC('I', 'D', 'L', 'E')		//keepalive
#define FCC_PINS FRGL_MAKEFCC('P', 'I', 'N', 'S')		//GPIO pin status
#define FCC_VS1G FRGL_MAKEFCC('V', 'S', '1', 'G')		//GPIO pin status

extern unsigned long local_ipv4addr;
VGUID net_nodeid;
VGUID net_nullid;

BTEIFGL_PeerInfo *net_conntgt;
// VADDR net_tgtaddr;
VGUID net_tgtid;
VGUID net_tgtauth;
byte net_conn;

double net_accDt;
double net_accTime;
double net_accKeepAlive;

BTEIFGL_PeerInfo *net_peer[1024];
int net_n_peers;

int net_sock;

u32 net_impulse1;
u32 net_impulse2;

u32 net_pins_active;
u32 net_pins_mask;
u32 net_pins_inmask;
u32 net_pins_outmask;
byte net_pins_target[32];

int net_vidbytes;
int net_vidnmesg;

BTEIFGL_API int BTEIFGL_NET_AddrEqual(VADDR *a, VADDR *b)
{
	if(!a || !b)return(0);
	if(a->proto != b->proto)return(0);
	switch(a->proto)
	{
	case PROTO_IPV4UDP:
		if(a->ipv4.addr != b->ipv4.addr)return(0);
		if(a->ipv4.port && b->ipv4.port)
			if(a->ipv4.port != b->ipv4.port)
			return(0);
		break;
	case PROTO_IPV4TCP:
		if(a->ipv4.addr != b->ipv4.addr)return(0);
		if(a->ipv4.port && b->ipv4.port)
			if(a->ipv4.port != b->ipv4.port)
			return(0);
		break;
	default:
		return(0);
		break;
	}
	return(1);
}

BTEIFGL_API BTEIFGL_PeerInfo *BTEIFGL_NET_LookupPeerAddr(VADDR *addr)
{
	int i;
	
	for(i=0; i<net_n_peers; i++)
	{
		if(BTEIFGL_NET_AddrEqual(&(net_peer[i]->addr), addr))
			return(net_peer[i]);
	}
	return(NULL);
}

BTEIFGL_API BTEIFGL_PeerInfo *BTEIFGL_NET_LookupPeerGuid(VGUID *nodeid)
{
	int i;
	
	for(i=0; i<net_n_peers; i++)
	{
		if(NET_GuidEqualP(&(net_peer[i]->nodeid), nodeid))
			return(net_peer[i]);
	}
	return(NULL);
}

BTEIFGL_API int BTEIFGL_NET_RegisterPeer(
	VADDR *addr, VGUID *id, int nodefl)
{
	BTEIFGL_PeerInfo *inf;
	int i;
	
//	if(BTEIFGL_NET_LookupPeerAddr(addr))
//		return(0);
	if(BTEIFGL_NET_LookupPeerGuid(id))
		return(0);
	
	inf=frgl_malloc(sizeof(BTEIFGL_PeerInfo));
	inf->nodeid=*id;
	inf->addr=*addr;
	inf->nodefl=nodefl;

	i=net_n_peers++;
	net_peer[i]=inf;
	inf->id=i;
	return(i);
}

BTEIFGL_API int BTEIFGL_NET_Init(void)
{
	VADDR addr;
	u32 laddr, ltaddr;
	char *s;
	int i;

	NET_InitLow();

	s=FRGL_CvarGet("net_nodeid");
	if(s && *s && (*s!='-'))
		NET_GuidFromName(&net_nodeid, s);

	s=FRGL_CvarGet("net_targetid");
	if(s && *s && (*s!='-'))
		NET_GuidFromName(&net_tgtid, s);

	s=FRGL_CvarGet("net_targetauth");
	if(s && *s && (*s!='-'))
		NET_GuidFromName(&net_tgtauth, s);

	if(NET_GuidEqualP(&net_nodeid, &net_nullid))
		NET_CreateGuid(&net_nodeid);


	net_sock=UDP_OpenSocket(16980+10);
	for(i=0; (net_sock<0) && (i<100); i++)
		net_sock=UDP_OpenSocket(16980+i);
	
	if(net_sock>=0)
	{
		UDP_GetSocketAddr(net_sock, &addr);
		s=NET_Addr2Str(&addr);
		frgl_printf("Host UDP Socket Created %s\n", s);
	}

	laddr=ntohl(local_ipv4addr);
	ltaddr=laddr|0x000000FF;
	BTEIFGL_NET_SendProbe(ltaddr);
	
#if 0
	laddr=ntohl(local_ipv4addr);
	for(i=1; i<255; i++)
	{
		ltaddr=(laddr&0xFFFFFF00)|i;
		BTEIFGL_NET_SendProbe(ltaddr);
	}
#endif

	return(0);
}

char *frgl_twocc2str(u16 tcc)
{
	char tb[8];
	
	tb[0]=tcc&255;
	tb[1]=(tcc>>8)&255;
	tb[2]=0;
	return(frgl_rstrdup(tb));
}

char *frgl_fourcc2str(u32 fcc)
{
	char tb[8];
	
	tb[0]=fcc&255;
	tb[1]=(fcc>>8)&255;
	tb[2]=(fcc>>16)&255;
	tb[3]=(fcc>>24)&255;
	tb[4]=0;
	return(frgl_rstrdup(tb));
}

int FRGL_ParseU16(byte **rcs)
{
	byte *cs;
	int i;
	cs=*rcs;
	i=*cs++;
	i=(i<<8)|(*cs++);
	*rcs=cs;
	return(i);
}

u32 FRGL_ParseU24(byte **rcs)
{
	byte *cs;
	u32 i;
	cs=*rcs;
	i=*cs++;
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	*rcs=cs;
	return(i);
}

u32 FRGL_ParseU32(byte **rcs)
{
	byte *cs;
	u32 i;
	cs=*rcs;
	i=*cs++;
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	*rcs=cs;
	return(i);
}

void FRGL_ParseGUID(byte **rcs, VGUID *rval)
{
	byte *cs;
	cs=*rcs;
	rval->data1[0]=*cs++;	rval->data1[1]=*cs++;
	rval->data1[2]=*cs++;	rval->data1[3]=*cs++;
	rval->data2[0]=*cs++;	rval->data2[1]=*cs++;
	rval->data3[0]=*cs++;	rval->data3[1]=*cs++;
	rval->data4[0]=*cs++;	rval->data4[1]=*cs++;
	rval->data4[2]=*cs++;	rval->data4[3]=*cs++;
	rval->data4[4]=*cs++;	rval->data4[5]=*cs++;
	rval->data4[6]=*cs++;	rval->data4[7]=*cs++;
	*rcs=cs;
}

void FRGL_EmitU16(byte **rct, u16 val)
{
	byte *ct;
	ct=*rct;
	*ct++=val>> 8;
	*ct++=val    ;
	*rct=ct;
}

void FRGL_EmitU24(byte **rct, u32 val)
{
	byte *ct;
	ct=*rct;
	*ct++=val>>16;
	*ct++=val>> 8;
	*ct++=val    ;
	*rct=ct;
}

void FRGL_EmitU32(byte **rct, u32 val)
{
	byte *ct;
	ct=*rct;
	*ct++=val>>24;
	*ct++=val>>16;
	*ct++=val>> 8;
	*ct++=val    ;
	*rct=ct;
}

void FRGL_EmitGUID(byte **rct, VGUID guid)
{
	byte *ct;
	ct=*rct;
	*ct++=guid.data1[0];	*ct++=guid.data1[1];
	*ct++=guid.data1[2];	*ct++=guid.data1[3];
	*ct++=guid.data2[0];	*ct++=guid.data2[1];
	*ct++=guid.data3[0];	*ct++=guid.data3[1];
	*ct++=guid.data4[0];	*ct++=guid.data4[1];
	*ct++=guid.data4[2];	*ct++=guid.data4[3];
	*ct++=guid.data4[4];	*ct++=guid.data4[5];
	*ct++=guid.data4[6];	*ct++=guid.data4[7];
	*rct=ct;
}


int BTEIFGL_NET_ProcessSubMsgTWOCC(
	byte *msg, int sz, u32 fcc, VADDR *addr)
{
	frgl_printf("Got Msg Sz=%d, tcc=%s(%08X)\n",
		frgl_twocc2str(fcc), sz, fcc);
}

int BTEIFGL_NET_ProcessSubMsgFOURCC(
	byte *msg, int sz, u32 fcc, VADDR *addr)
{
	byte tb[1024];
	VGUID tuid;
	byte *cs, *ct;
	int i, j, k;
	
//	if((fcc!=FCC_PING) && (fcc!=FCC_PIAK))
//	{
//		frgl_printf("Got Msg Sz=%d, fcc=%s(%08X)\n",
//			sz, frgl_fourcc2str(fcc), fcc);
//	}
	
	if(fcc==FCC_PING)
	{
		i=1;
	
		ct=tb;
		FRGL_EmitGUID(&ct, net_nodeid);
		FRGL_EmitU32(&ct, i);
		BTEIFGL_NET_SendSimpleFcc(addr, tb, ct-tb, FCC_PIAK);
		return(0);
	}

	if(fcc==FCC_PIAK)
	{
		cs=msg;
		FRGL_ParseGUID(&cs, &tuid);
		i=FRGL_ParseU32(&cs);
		j=BTEIFGL_NET_RegisterPeer(addr, &tuid, i);

		if(!net_conn && NET_GuidEqualP(&tuid, &net_tgtid))
		{
			net_accKeepAlive=0;
			net_conntgt=net_peer[j];

			ct=tb;
			FRGL_EmitGUID(&ct, net_nodeid);
			FRGL_EmitGUID(&ct, net_tgtauth);
			FRGL_EmitU16(&ct, 0);
			FRGL_EmitU16(&ct, 0);
			BTEIFGL_NET_SendSimpleFcc(addr, tb, ct-tb, FCC_RCTR);
			net_conn=1;
			return(0);
		}

		return(0);
	}

	if(fcc==FCC_RCAK)
	{
		net_accKeepAlive=0;
		net_conn=2;
		return(0);
	}

	if(fcc==FCC_IDLE)
	{
		net_accKeepAlive=0;
		return(0);
	}

	if(fcc==FCC_PINS)
	{
		net_accKeepAlive=0;

		cs=msg;
		net_pins_inmask=FRGL_ParseU32(&cs);
		net_pins_outmask=FRGL_ParseU32(&cs);
		net_pins_mask=FRGL_ParseU32(&cs);
		net_pins_active=FRGL_ParseU32(&cs);

		for(i=0; i<32; i++)
		{
			if(net_pins_mask&(1<<i))
				{ net_pins_target[i]=*cs++; }
			else
				{ net_pins_target[i]=0; }
		}

		return(0);
	}

#if 0
	if(fcc==FCC_VS1G)
	{
		net_accKeepAlive=0;
		net_vidbytes+=sz;
		net_vidnmesg++;
		BTIC1G_NET_ProcessMsgVS1G(msg, sz, addr);
		return(0);
	}
#endif

	frgl_printf("Unhandled Msg Sz=%d, fcc=%s(%08X)\n",
		sz, frgl_fourcc2str(fcc), fcc);
}

int BTEIFGL_NET_CheckSync(byte *cs, byte *cse)
{
	byte tb[16];
	int tsz, h0, h1;
	int i;
	
	if(cs[0]!=0xEF)
	{
		frgl_printf("Start Byte != 0xEF\n");
		return(0);
	}
	
	switch(cs[4])
	{
	case 0xE3: tsz=4+6; break;
	case 0xE4: tsz=4+8; break;
	case 0xE5: tsz=4+4; break;
	default: tsz=0; break;
	}

	if(tsz<=0)
	{
		frgl_printf("Bad Tag %02X\n", cs[4]);
		return(0);
	}

	if((cs+tsz)>cse)
	{
		frgl_printf("Truncated Header\n");
		return(0);
	}
	
	h0=(cs[1]<<16)|(cs[2]<<8)|cs[3];
	
	memcpy(tb, cs, tsz);
	tb[1]='B';	tb[2]='T';	tb[3]='E';
	
	h1=0;
	for(i=0; i<tsz; i++)
		h1=(h1*251)+tb[i];
	h1=((h1*251)>>8)&0xFFFFFF;

	if(h1!=h0)
	{
		frgl_printf("Hash Fail %06X!=%06X\n", h0, h1);
		return(0);
	}
	return(1);
}

int BTEIFGL_NET_ProcessMsg(byte *msg, int sz, VADDR *addr)
{
	byte *cs, *cse;
	u32 tfcc;
	int tsz;

//	frgl_printf("Got %d bytes from %s\n", sz, NET_Addr2Str(addr));
	
	cs=msg; cse=msg+sz;
		
	if(BTEIFGL_NET_CheckSync(cs, cse)<=0)
	{
		frgl_printf("Message Lacks Valid Sync\n");
		return(-1);
	}
	
	cs+=4;
	
	while(cs<cse)
	{
		if(*cs==0xE3)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			tfcc=cs[4]|(cs[5]<<8);
			BTEIFGL_NET_ProcessSubMsgTWOCC(cs+6, tsz-6, tfcc, addr);
			cs=cs+tsz;
			continue;
		}
		if(*cs==0xE4)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			tfcc=cs[4]|(cs[5]<<8)|(cs[6]<<16)|(cs[7]<<24);
			BTEIFGL_NET_ProcessSubMsgFOURCC(cs+8, tsz-8, tfcc, addr);
			cs=cs+tsz;
			continue;
		}
		if(*cs==0xE5)
		{
			tsz=cs[1];
			tfcc=cs[2]|(cs[3]<<8);
			BTEIFGL_NET_ProcessSubMsgTWOCC(cs+4, tsz-4, tfcc, addr);
			cs=cs+tsz;
			continue;
		}

		if(*cs==0xEF)
		{
			cs+=4;
			continue;
		}
	}
	
	return(0);
}

BTEIFGL_API int BTEIFGL_NET_Update(double dt)
{
	byte msg[16384];
	double f, g;
	VADDR addr;
	byte *ct;
	u32 laddr, ltaddr;
	int sz;
	
	if(net_sock<0)
		return(-1);
	
	if(dt>0.25)dt=0;
	
	net_accKeepAlive+=dt;
	while(1)
	{
		sz=UDP_SockGet(msg, 16384, &addr, net_sock);
		if(sz<0)break;
		
		BTEIFGL_NET_ProcessMsg(msg, sz, &addr);
	}

	if((net_conn!=0) && (net_accKeepAlive>10))
	{
		frgl_printf("Keepalive Timeout\n");
		net_conn=0;
	}

	net_accTime+=dt;
	net_accDt+=dt;
	if((net_conn==0) && (net_accDt>1))
	{
		laddr=ntohl(local_ipv4addr);
		ltaddr=laddr|0x000000FF;
		BTEIFGL_NET_SendProbe(ltaddr);
		net_accDt=0;
	}

#if 0
	if(net_vidbytes && (net_accDt>0.1))
	{
		f=((net_vidbytes/net_accTime)/1024);
		g=net_vidbytes/((double)(net_vidnmesg+1));
		frgl_printf("Video Bytes: %.2f kB/sec (%.2fMbps), avgSz=%.2f\n",
			f, f*8/1000.0, g);
	}
#endif

	if((net_conn==2) && (net_accDt>0.1))
	{
		ct=msg;
		FRGL_EmitU32(&ct, net_impulse1);
		FRGL_EmitU32(&ct, net_impulse2);
		BTEIFGL_NET_SendSimpleFcc(&(net_conntgt->addr),
			msg, ct-msg, FCC_CTRL);
		net_accDt=0;
	}
		
	return(0);
}

BTEIFGL_API int BTEIFGL_NET_SetImpulse(u32 imp1, u32 imp2)
{
	net_impulse1=imp1;
	net_impulse2=imp2;
	return(0);
}

BTEIFGL_API u32 BTEIFGL_NET_GetPinsMask(void)
	{ return(net_pins_mask); }
BTEIFGL_API u32 BTEIFGL_NET_GetPinsActive(void)
	{ return(net_pins_active); }
BTEIFGL_API int BTEIFGL_NET_GetPinsTarget(int pin)
	{ return(net_pins_target[pin]); }

BTEIFGL_API u32 BTEIFGL_NET_GetPinsInMask(void)
	{ return(net_pins_inmask); }
BTEIFGL_API u32 BTEIFGL_NET_GetPinsOutMask(void)
	{ return(net_pins_outmask); }

BTEIFGL_API int BTEIFGL_NET_SetupSync(byte *cs)
{
	int tsz, h0, h1;
	int i;
	
	if(cs[0]!=0xEF)
		{ return(0); }
	
	switch(cs[4])
	{
	case 0xE3: tsz=4+6; break;
	case 0xE4: tsz=4+8; break;
	case 0xE5: tsz=4+4; break;
	default: tsz=0; break;
	}
	if(tsz<=0)
		return(0);
	
	h1=0;
	for(i=0; i<tsz; i++)
		h1=(h1*251)+cs[i];
	h1=((h1*251)>>8)&0xFFFFFF;

//	frgl_printf("Hash %06X\n", h1);

	cs[1]=(h1>>16)&255;
	cs[2]=(h1>> 8)&255;
	cs[3]=(h1    )&255;
	return(1);
}

BTEIFGL_API int BTEIFGL_NET_SendProbe(u32 ipv4)
{
	byte buf[512];
	VADDR addr;
	byte *ct;
	int szbuf;
	int i;

	ct=buf;
	*ct++=0xEF;
	*ct++='B';
	*ct++='T';
	*ct++='E';
	*ct++=0xE4;
	*ct++=0;
	*ct++=0;
	*ct++=8;
	*ct++='P';
	*ct++='I';
	*ct++='N';
	*ct++='G';
	BTEIFGL_NET_SetupSync(buf);
	
	szbuf=ct-buf;

	addr.proto=PROTO_IPV4UDP;
//	addr.ipv4.addr=htonl(ipv4);
//	addr.ipv4.port=htons(16980);
	addr.ipv4.addr=ipv4;
	addr.ipv4.port=16980;

//	frgl_printf("Send Ping To %s\n", NET_Addr2Str(&addr));

//	UDP_SockSend(net_sock, &addr, buf, szbuf, 0);

	for(i=0; i<100; i++)
	{
//		addr.ipv4.port=htons(16980+i);
		addr.ipv4.port=16980+i;
		UDP_SockSend(net_sock, &addr, buf, szbuf, 0);
	}

	return(0);
}

BTEIFGL_API int BTEIFGL_NET_SendSimpleFcc(VADDR *addr,
	byte *data, int szData, u32 fcc)
{
	byte buf[MAXMSG];
	byte *ct;
	int szbuf;
	int i, j, k;

	j=szData+12;
	if(j>MAXMSG)
	{
		frgl_printf("Oversized Message, Sz=%d\n", j);
		return(-1);
	}

	i=8+szData;
	ct=buf;
	*ct++=0xEF;
	*ct++='B';
	*ct++='T';
	*ct++='E';
	*ct++=0xE4;
	*ct++=(i>>16);
	*ct++=(i>> 8);
	*ct++=(i    );
	*ct++=FRGL_FCC_GETA(fcc);
	*ct++=FRGL_FCC_GETB(fcc);
	*ct++=FRGL_FCC_GETC(fcc);
	*ct++=FRGL_FCC_GETD(fcc);
	memcpy(ct, data, szData);
	ct+=szData;

	BTEIFGL_NET_SetupSync(buf);
	
	szbuf=ct-buf;

	if((fcc!=FCC_PING) && (fcc!=FCC_PIAK) && (fcc!=FCC_CTRL))
	{
		frgl_printf("Send %s To %s\n",
			frgl_fourcc2str(fcc), NET_Addr2Str(addr));
	}

	UDP_SockSend(net_sock, addr, buf, szbuf, 0);

	return(0);
}
