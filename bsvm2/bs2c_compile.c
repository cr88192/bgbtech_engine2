#include <bteifgl.h>

BTEIFGL_API int BS2C_CompileModuleBuffer(
	BS2CC_CompileContext *ctx, char *buf, int szbuf)
{
	dtVal v0;

	v0=BS2P_ParseBuffer(ctx, buf, szbuf);

//	BGBDT_MM_PrintValueToStrBuf(tb, 1023, v0);
//	BGBDT_MM_FormatValueToStrBuf(tb, 16383, v0);
//	printf("TB v0: %s\n", tb);
	
	BS2C_CompileTopStatement(ctx, v0);
	BS2C_CompileFuncs(ctx);

//	BS2C_DumpErrors2(ctx);
//	prn=BGBDT_MM_NewStringPrinter(tb, 16383);
//	BS2C_DisAsmFuncs(prn, ctx);
//	printf("%s\n", tb);

	return(0);
}

BTEIFGL_API int BS2C_CompileModuleList(
	BS2CC_CompileContext *ctx,
	char *base, char **names)
{
	char tb[256], tb1[256];
	byte *tbuf;
	char *s, *t;
	int tsz;
	dtVal v0;
	int i, j, k;

	for(i=0; names[i]; i++)
	{
		s=names[i];
		t=tb1;
		while(*s)
		{
			if(*s=='.')
				{ *t++='/'; s++; continue; }
			*t++=*s++;
		}
		*t++=0;
	
		if(base)
		{
			sprintf(tb, "%s/%s.bs", base, tb1);
		}else
		{
			sprintf(tb, "%s.bs", tb1);
		}
	
		tbuf=vf_loadfile(tb, &tsz);
		if(!tbuf)
		{
			BS2C_CompileError(ctx, BS2CC_ERRN_FATALNOFILE);
		}

		v0=BS2P_ParseBuffer(ctx, tbuf, tsz);
		
		if(ctx->dbgprn)
			{ BGBDT_MM_FormatValue(ctx->dbgprn, v0); }
		
		BS2C_CompileTopStatement(ctx, v0);

		vf_freefdbuf(tbuf);
	}

	BS2C_CompileFuncs(ctx);

	if(ctx->dbgprn)
	{
		BS2C_DisAsmFuncs(ctx->dbgprn, ctx);
	}
	return(0);
}
