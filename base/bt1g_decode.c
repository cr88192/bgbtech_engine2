#include <bteifgl.h>

#define TCC_SH FRGL_MAKETCC('S', 'H')
#define TCC_PA FRGL_MAKETCC('P', 'A')

BTIC1G_DecoderContext *btic1g_decoder=NULL;

byte *BTIC1G_DecodeBlocksBuffer(byte *ibuf,
	byte *blks, byte *lblks, int nblks, int stride,
	int ibsz)
{
	byte *cs, *cse, *ct, *cte, *csl;
	int cy, cu, cv, cd, op;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=blks; cte=blks+nblks*stride;
	csl=lblks;
	
	while((cs<cse) && (ct<cte))
	{
		if((*cs)<0x80)
		{
			i=(cs[0]<<16)|(cs[1]<<8)|cs[2]; cs+=3;

			cy=(i>>13)&0xFE;
			cd=(i>> 6)&0xFC;
			cu=(i    )&0xF0;
			cv=(i<< 4)&0xF0;
//			if(cu!=128)cu=cu|(cu>>4);
//			if(cv!=128)cv=cv|(cv>>4);
			cy=cy|(cy>>7);
			cu=btic1g_dequant4(cu>>4);
			cv=btic1g_dequant4(cv>>4);

			switch((i>>21)&3)
			{
			case 0:
#if 1
				if(i&0x100)
				{
					cs--; i=i>>8;
					cy=(i>>5)&0xFC;
					cd=0;
					cu=(i<< 1)&0xE0;
					cv=(i<< 4)&0xE0;

					cy=cy|(cy>>6);
					cu=btic1g_dequant4(cu>>4);
					cv=btic1g_dequant4(cv>>4);
				}else if(i&0x200)
				{
					cy=(i>>13)&0xFF;
					cd=0;
					cu=(i    )&0xF0;
					cv=(i<< 4)&0xF0;
					cu|=(i>>9)&0x08;
					cv|=(i>>8)&0x08;
					j=(i>>8)&0x04;
					cu|=j;	cv|=j;
					cu=btic1g_dequant6(cu>>2);
					cv=btic1g_dequant6(cv>>2);
//					cu=btic1g_dequant5(cu>>3);
//					cv=btic1g_dequant5(cv>>3);
				}
#endif
				ct[0]=cy;	ct[1]=cu;
				ct[2]=cv;	ct[3]=0;
				ct[4]=0;	ct[5]=cd;
				ct[6]=0;	ct[7]=0;
				break;
			case 1:
				j=*cs++;
				ct[0]=cy;	ct[1]=cu;
				ct[2]=cv;	ct[3]=0;
				ct[4]=1;	ct[5]=cd;
				ct[6]=j;	ct[7]=0;
				break;
			case 2:
				i0=*cs++;	i1=*cs++;
				ct[0]=cy;	ct[1]=cu;
				ct[2]=cv;	ct[3]=0;
				ct[4]=2;	ct[5]=cd;
				ct[6]=i0;	ct[7]=i1;
				break;
			case 3:
				i0=*cs++;	i1=*cs++;
				i2=*cs++;	i3=*cs++;
				ct[0]=cy;	ct[1]=cu;
				ct[2]=cv;	ct[3]=cd;
				ct[4]=i0;	ct[5]=i1;
				ct[6]=i2;	ct[7]=i3;
				break;
			}
			ct+=stride;
			csl+=stride;
			continue;
		}
		op=*cs++;
		switch((op>>4)&15)
		{
		case 8:
			l=(op&15)+1;
			for(i=0; i<l; i++)
			{
				memcpy(ct, csl, 8);
				ct+=stride;
				csl+=stride;
			}
			break;

		case 9:
			l=(op&15)+1;

			j=(cs[0]<<8)|cs[1]; cs+=2;
			cy=(j>> 7)&0xFE;
			cu=(j    )&0xF0;
			cv=(j<< 4)&0xF0;

			cy=cy|(cy>>7);
			cu=btic1g_dequant4(cu>>4);
			cv=btic1g_dequant4(cv>>4);

			for(i=0; i<l; i++)
			{
				ct[0]=cy;	ct[1]=cu;
				ct[2]=cv;	ct[3]=0;
				ct[4]=0;	ct[5]=0;
				ct[6]=0;	ct[7]=0;
				ct+=stride;
				csl+=stride;
			}
			break;
		case 10:
			l=(op&15)+1;
			for(i=0; i<l; i++)
			{
				j=(cs[0]<<8)|cs[1]; cs+=2;
				cy=(j>> 7)&0xFE;
				cu=(j    )&0xF0;
				cv=(j<< 4)&0xF0;
				cy=cy|(cy>>7);
				cu=btic1g_dequant4(cu>>4);
				cv=btic1g_dequant4(cv>>4);

				ct[0]=cy;	ct[1]=cu;
				ct[2]=cv;	ct[3]=0;
				ct[4]=0;	ct[5]=0;
				ct[6]=0;	ct[7]=0;
				ct+=stride;
				csl+=stride;
			}
			break;

		case 11:
		case 12:
		case 13:
			l=(op&15)+1;
			j=(cs[0]<<16)|(cs[1]<<8)|cs[2]; cs+=3;
			cy=(j>>15)&0xFE;
			cd=(j>> 8)&0xFC;
			cu=(j>> 2)&0xF8;
			cv=(j<< 3)&0xF8;
			cy=cy|(cy>>7);
			cu=btic1g_dequant5(cu>>3);
			cv=btic1g_dequant5(cv>>3);

			if((op>>4)==11)
			{
				for(i=0; i<l; i++)
				{
					ct[0]=cy;		ct[1]=cu;
					ct[2]=cv;		ct[3]=0;
					ct[4]=1;		ct[5]=cd;
					ct[6]=*cs++;	ct[7]=0;
					ct+=stride;		csl+=stride;
				}
			}else if((op>>4)==12)
			{
				for(i=0; i<l; i++)
				{
					ct[0]=cy;		ct[1]=cu;
					ct[2]=cv;		ct[3]=0;
					ct[4]=2;		ct[5]=cd;
					ct[6]=*cs++;	ct[7]=*cs++;
					ct+=stride;		csl+=stride;
				}
			}else if((op>>4)==13)
			{
				for(i=0; i<l; i++)
				{
					ct[0]=cy;		ct[1]=cu;
					ct[2]=cv;		ct[3]=cd;
					ct[4]=*cs++;	ct[5]=*cs++;
					ct[6]=*cs++;	ct[7]=*cs++;
					ct+=stride;		csl+=stride;
				}
			}
			break;
		default:
			break;
		}
	}
}

void BTIC1G_DecodeInit()
{
	static int init=0;
	BTIC1G_DecoderContext *ctx;

	if(init)return;
	init=1;

	BTIC1G_InitQuant();

	ctx=frgl_malloc(sizeof(BTIC1G_DecoderContext));
	btic1g_decoder=ctx;

//	ctx->framebuf=frgl_malloc(1024*512*4);
//	ctx->framebuf=frgl_malloc(1024*512*4);
//	ctx->xs=640;
//	ctx->ys=480;
//	ctx->nBlocks=160*120;
	ctx->qf=25;

	ctx->framebuf=frgl_malloc(512*256*4);
	ctx->xs=320;
	ctx->ys=240;
	ctx->nBlocks=80*60;
		
	ctx->blkbuf=frgl_malloc(ctx->nBlocks*16);
	ctx->lblkbuf=frgl_malloc(ctx->nBlocks*16);
}

int BTIC1G_NET_ProcessSubMsgTWOCC(
	byte *msg, int sz, u16 tcc, VADDR *addr)
{
	byte tb[1024];
	BTIC1G_DecoderContext *ctx;
	byte *cs, *ct;
	int i, j, k;
	
//	frgl_printf("Got Msg Sz=%d, tcc=%s(%08X)\n",
//		frgl_twocc2str(fcc), sz, fcc);

	if(tcc==TCC_SH)
	{
		ctx=btic1g_decoder;
	
		cs=msg;
//		i=FRGL_ParseU16(&cs);
		i=*cs++;
		j=*cs++;
		ctx->msgFrame=FRGL_ParseU16(&cs);
		ctx->msgStartBlock=FRGL_ParseU16(&cs);
		ctx->msgEndBlock=FRGL_ParseU16(&cs);
		ctx->msgStartBlock|=(*cs++)<<16;
		ctx->msgEndBlock|=(*cs++)<<16;

		if(ctx->msgFrame>ctx->msgCurFrame)
		{
			memcpy(ctx->lblkbuf, ctx->blkbuf,
				ctx->nBlocks*16);
			ctx->msgCurFrame=ctx->msgFrame;
		}else if((ctx->msgFrame^ctx->msgCurFrame)&0x8000)
		{
			memcpy(ctx->lblkbuf, ctx->blkbuf,
				ctx->nBlocks*16);
			ctx->msgCurFrame=ctx->msgFrame;
		}

		return(0);
	}

	frgl_printf("VS1G: Unhandled Msg Sz=%d, tcc=%s(%08X)\n",
		sz, frgl_twocc2str(tcc), tcc);
}

int BTIC1G_NET_ProcessSubMsgFOURCC(
	byte *msg, int sz, u32 fcc, VADDR *addr)
{
}

//byte *BTIC1G_DecodeBlocksBuffer(byte *ibuf,
//	byte *blks, byte *lblks, int nblks, int stride,
//	int ibsz)

int BTIC1G_NET_ProcessMsgVS1G(byte *msg, int sz, VADDR *addr)
{
	BTIC1G_DecoderContext *ctx;
	byte *cs, *cse;
	u32 tfcc;
	int tsz, bi, bn;

	BTIC1G_DecodeInit();

	cs=msg; cse=msg+sz;
		
	while(cs<cse)
	{
		if(*cs==0xE1)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];

			ctx=btic1g_decoder;
			ctx->clean=0;

			if((ctx->msgFrame<ctx->msgCurFrame) ||
				((ctx->msgFrame^ctx->msgCurFrame)&0x8000))
			{
				cs=cs+tsz;
				continue;
			}

			bi=ctx->msgStartBlock;
			bn=ctx->msgEndBlock-bi;
			BTIC1G_DecodeBlocksBuffer(cs+4,
				ctx->blkbuf+bi*16, ctx->lblkbuf+bi*16,
				bn, 16, tsz-4);
			cs=cs+tsz;
			continue;
		}
	
		if(*cs==0xE3)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			tfcc=cs[4]|(cs[5]<<8);
			BTIC1G_NET_ProcessSubMsgTWOCC(cs+6, tsz-6, tfcc, addr);
			cs=cs+tsz;
			continue;
		}
		if(*cs==0xE4)
		{
			tsz=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			tfcc=cs[4]|(cs[5]<<8)|(cs[6]<<16)|(cs[7]<<24);
			BTIC1G_NET_ProcessSubMsgFOURCC(cs+8, tsz-8, tfcc, addr);
			cs=cs+tsz;
			continue;
		}
		if(*cs==0xE5)
		{
			tsz=cs[1];
			tfcc=cs[2]|(cs[3]<<8);
			BTIC1G_NET_ProcessSubMsgTWOCC(cs+4, tsz-4, tfcc, addr);
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

BTEIFGL_API int BTIC1G_NET_GetStreamTexture(int strm)
{
	BTIC1G_DecoderContext *ctx;
	int xs, ys;

	BTIC1G_InitQuant();
	BTIC1G_DecodeInit();

	ctx=btic1g_decoder;
	
	if(ctx->clean)
		return(ctx->texId);
	
	BTIC1G_DecodeImageStrideMB2B(ctx->blkbuf, ctx->framebuf,
		ctx->xs, ctx->ys,
		4, 4*512);
		
//	if(ctx->texId<=0)
//		ctx->texId=Tex_AllocTexnum();
		
//	xs=1024; ys=512;
	xs=512; ys=256;
	ctx->texId=Tex_LoadTexture3(&xs, &ys,
		ctx->framebuf, ctx->texId);
	ctx->clean=1;
	return(ctx->texId);
}
