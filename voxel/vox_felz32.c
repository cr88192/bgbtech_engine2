/*
FELZ32: LZ in terms of 32-bit DWORDs

tag:
  bits  0..15: md, match distance (DWORDs)
  bits 16..22: ml, match length (DWORDs)
  bits 23..29: rl, raw length (DWORDs)
  bits 30..31: al, tag/align

ml!=0:
  md=distance
  ml=length
  mr=raw
  al=align (0=DWORD aligned, others: byte offset)

ml==0, rl==0, al==0:
	md==0: EOB
	md!=0: long run of raw DWORDs
 */

#include <bteifgl.h>

int BGBDT_FeLz32_LookupMatch(BGBDT_FeLzContext *ctx,
	u32 *cs, u32 *cse, int *rml, int *rmd)
{
	u32 *s, *se, *t, *te, *ct;
	u32 v0, h;
	int l, d;
	
	v0=*cs;
	h=((v0*65521)>>16)&4095;
	
	ct=ctx->lzhash[h];
	if(!ct)
	{
		*rml=0; *rmd=0;
		return(0);
	}
	
	s=cs; se=s+126;
	if(cse<se)se=cse;
	t=ct;

	while(s<se)
	{
		if(*s!=*t)
			break;
		s++; t++;
	}
	
	l=s-cs;
	d=cs-ct;
	*rml=l;
	*rmd=d;
	return((l>1) && (d<65536));
}

int BGBDT_FeLz32_UpdateString(BGBDT_FeLzContext *ctx,
	u32 *cs, u32 *cse, int len)
{
	u32 v;
	int i, h;

	i=len;
	while(i--)
	{
		v=*cs;
		h=((v*65521)>>16)&4095;
		ctx->lzhash[h]=cs++;
	}
	return(0);
}

int BGBDT_FeLz32_EncodeBufferCtx(BGBDT_FeLzContext *ctx,
	u32 *ibuf, int ibsz, u32 *obuf, int obsz)
{
	u32 *cs, *cse, *csrb;
	u32 *ct, *cte;
	u32 tg;
	int ml, md, rl;
	int i, j, k;

	for(i=0; i<4096; i++)
		{ ctx->lzhash[i]=NULL; }
	
	cs=ibuf; cse=cs+ibsz;
	ct=obuf; cte=ct+obsz;
	csrb=cs;
	
	while(cs<cse)
	{	
		if(BGBDT_FeLz32_LookupMatch(ctx, cs, cse, &ml, &md))
		{
			rl=cs-csrb;
			while(rl>=127)
			{
				if(rl>=65536)
					rl=65535;
				*ct++=rl;
				i=rl;
				while(i--)
//				for(i=0; i<rl; i++)
					{ *ct++=*csrb++; }
				rl=cs-csrb;
			}
			
			tg=md|(ml<<16)|(rl<<23);
			*ct++=tg;
			i=rl;
			while(i--)
//			for(i=0; i<rl; i++)
				{ *ct++=*csrb++; }
			BGBDT_FeLz32_UpdateString(ctx, cs, cse, ml);
			cs+=ml;
			csrb=cs;
		}else
		{
			BGBDT_FeLz32_UpdateString(ctx, cs, cse, 1);
			cs++;
		}
	}

	rl=cs-csrb;
	while(rl>0)
	{
		if(rl>=65536)
			rl=65535;
		*ct++=rl;
		i=rl;
		while(i--)
			{ *ct++=*csrb++; }
		rl=cs-csrb;
	}
	
	*ct++=0;
	
	return(ct-obuf);
}

int BGBDT_FeLz32_EncodeBuffer(
	u32 *ibuf, int ibsz, u32 *obuf, int obsz)
{
	BGBDT_FeLzContext tctx;
	return(BGBDT_FeLz32_EncodeBufferCtx(&tctx, ibuf, ibsz, obuf, obsz));
}

int BGBDT_FeLz32_DecodeBuffer(
	u32 *ibuf, int ibsz, u32 *obuf, int obsz)
{
	u32 *cs, *cse, *cs1;
	u32 *ct, *cte;
	u32 tg;
	int ml, md, rl, al;
	int i, j, k;

	cs=ibuf; cse=cs+ibsz;
	ct=obuf; cte=ct+obsz;

	while(cs<cse)
	{
		tg=*cs++;
		ml=(tg>>16)&127;
		md=tg&65535;

		if(ml)
		{
			rl=(tg>>23)&127;
			al=(tg>>30)&3;

			i=rl;
			while(i--)
				{ *ct++=*cs++; }

			if(al)
			{
				FRGL_DBGBREAK_SOFT
				memmove(ct, ((byte *)(ct-md))+al, ml*4);
				ct+=ml;
			}else
			{
				cs1=ct-md;
				i=ml;
				while(i--)
					{ *ct++=*cs1++; }
			}
			continue;
		}
		
		if(!tg)
			break;
		
		if(!(tg&0xFFFF0000U))
		{
			i=md;
			while(i--)
				{ *ct++=*cs++; }
			continue;
		}
		
		FRGL_DBGBREAK_SOFT
	}
	
	return(ct-obuf);
}
