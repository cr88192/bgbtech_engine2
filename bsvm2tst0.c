#include <bteifgl.h>

#undef printf

char *bsvmt_tstprg=
"package foo.bar {"
"struct foo_s { int x, y; double z; }\n"
"int i, j;\n"
"int[] arr;\n"
"int fib(int x)\n"
"{\n"
"	if(x<2)return(1);\n"
"	return(fib(x-1)+fib(x-2));\n"
"}\n"
"}\n"
;

void BS2C_DumpErrors2(
	BS2CC_CompileContext *ctx)
{
	int i, j, k;
	
	if(!ctx->ncerr && !ctx->ncfatal)
	{
		for(i=0; i<ctx->ncwarn; i++)
		{
			printf("%d:%d %04X\n",
				(ctx->cwarnln[i]>>20), ctx->cwarnln[i]&0xFFFFF,
				ctx->cwarn[i]);
		}
		printf("%d warnings\n", ctx->ncwarn);
	}
	
	for(i=0; i<ctx->ncerr; i++)
	{
		printf("%d:%d %04X\n",
			(ctx->cerrln[i]>>20), ctx->cerrln[i]&0xFFFFF,
			ctx->cerr[i]);
	}
	printf("%d errors\n", ctx->ncerr);

	if(ctx->ncfatal)
	{
		for(i=0; i<ctx->ncfatal; i++)
		{
			printf("%d:%d %04X\n",
				(ctx->cfatalln[i]>>20), ctx->cfatalln[i]&0xFFFFF,
				ctx->cfatal[i]);
		}
		printf("%d fatal errors\n", ctx->ncfatal);
	}
}

int main()
{
	static char *mods[]={
		"bstest.fib",
		NULL
	};
	BGBDT_MM_ParsePrintInfo *prn;
	BS2CC_CompileContext *ctx;
	BSVM2_Context *vctx;
	BSVM2_CodeImage *img;
	BSVM2_ImageGlobal *vi;

	char tb[1<<18];
	dtVal v0, v1, v2, v3;
	int i, j, k;

	printf("BSVM Test0\n");
	
	v0=dtvWrapIntF(314159);
//	v1=dtvWrapLongF(314159);

	v1=dtvWrapLong(0x51003CFFFFDAF3B4LL);

//	v2=dtvWrapChar(0xC0C0);
//	v2=dtvWrapDoubleF(3.14159);

	v2=dtvNewArray(8, BGBDT_BASETY_ADDRESS);
	
	dtvArraySetIndexDtVal(v2, 0, dtvWrapDoubleF(3.14159));
	dtvArraySetIndexDtVal(v2, 1, dtvWrapChar(0xC0C0));
	dtvArraySetIndexDtVal(v2, 2, dtvWrapChar('$'));
	
	v3=BGBDT_MapObj_AllocObjv();
	
	BGBDT_MapObj_BindObjvSlotValueName(v3, "x", v0);
	BGBDT_MapObj_BindObjvSlotValueName(v3, "y", dtvWrapChar(0xDECA));

	BGBDT_MapObj_BindObjvSlotValueName(v3, "z",
		BGBDT_TagStr_String("test\r\n\xCF\x9A"));
	
	printf("v0: %08X_%08X\n", v0.hi, v0.lo);
	printf("v1: %08X_%08X\n", v1.hi, v1.lo);
	printf("v2: %08X_%08X\n", v2.hi, v2.lo);
	printf("v3: %08X_%08X\n", v3.hi, v3.lo);
	
	BGBDT_MM_PrintValueToStrBuf(tb, 1023, v0);
	printf("TB v0: %s\n", tb);

	BGBDT_MM_PrintValueToStrBuf(tb, 1023, v1);
	printf("TB v1: %s\n", tb);

	BGBDT_MM_PrintValueToStrBuf(tb, 1023, v2);
	printf("TB v2: %s\n", tb);

	BGBDT_MM_PrintValueToStrBuf(tb, 1023, v3);
	printf("TB v3: %s\n", tb);
	
	ctx=BS2CC_AllocCompileContext();
//	prn=BGBDT_MM_NewStringPrinter(tb, 16383);
	prn=BGBDT_MM_NewStringPrinter(tb, (1<<18)-1);
	ctx->dbgprn=prn;

	BS2C_CompileModuleList(ctx, NULL, mods);

	printf("%s\n", tb);

	i=BS2C_FlattenImage(ctx, tb, 1<<18);
	vf_storefile("bstest0.bsi", tb, i);

	img=BS2I_DecodeImageBuffer(tb, i);
	vi=BS2I_ImageGetMain(img, NULL);
//	BS2I_ImageGetMainTrace(img, NULL);

	vctx=BSVM2_Interp_AllocContext();
	BSVM2_Interp_SetupCallVM(vctx, vi, NULL);
	BSVM2_Interp_RunContext(vctx, 999999999);

#if 0
	v0=BS2P_ParseBuffer(ctx, bsvmt_tstprg, strlen(bsvmt_tstprg));

//	BGBDT_MM_PrintValueToStrBuf(tb, 1023, v0);
	BGBDT_MM_FormatValueToStrBuf(tb, 16383, v0);
	printf("TB v0: %s\n", tb);
	
	BS2C_CompileTopStatement(ctx, v0);
	BS2C_CompileFuncs(ctx);
	BS2C_DumpErrors2(ctx);

	prn=BGBDT_MM_NewStringPrinter(tb, 16383);
	BS2C_DisAsmFuncs(prn, ctx);
	printf("%s\n", tb);
#endif

}
