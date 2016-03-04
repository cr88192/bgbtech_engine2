#include <bteifgl.h>

#undef printf

char *bsvmt_tstprg=
"int i, j;\n"
"int[] arr;\n"
"int fib(int x)\n"
"{\n"
"	if(x<2)return(1);\n"
"	return(fib(x-1)+fib(x-2));\n"
"}\n"
;

int main()
{
	BGBDT_MM_ParsePrintInfo *prn;
	BS2CC_CompileContext *ctx;

	char tb[16384];
	dtVal v0, v1, v2, v3;
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
	v0=BS2P_ParseBuffer(ctx, bsvmt_tstprg, strlen(bsvmt_tstprg));

//	BGBDT_MM_PrintValueToStrBuf(tb, 1023, v0);
	BGBDT_MM_FormatValueToStrBuf(tb, 16383, v0);
	printf("TB v0: %s\n", tb);
}
