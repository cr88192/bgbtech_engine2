#include <bteifgl.h>

dtVal BS2P_NewAstNode(char *tag)
{
	dtVal obj;
	char *s;
	
	obj=BGBDT_MapObj_AllocObjv();
//	BGBDT_MapObj_BindObjvSlotValueName(obj, "tag",
//		BGBDT_TagStr_String(tag));
	BGBDT_MapObj_BindObjvSlotValueName(obj, "tag",
		BGBDT_TagStr_Symbol(tag));
	return(obj);
}

void BS2P_SetAstNodeAttr(dtVal obj, char *attr, dtVal val)
{
	BGBDT_MapObj_BindObjvSlotValueName(obj, attr, val);
}

void BS2P_SetAstNodeAttrS(dtVal obj, char *attr, char *val)
{
//	BGBDT_MapObj_BindObjvSlotValueName(obj, attr,
//		BGBDT_TagStr_String(val));
	BGBDT_MapObj_BindObjvSlotValueName(obj, attr,
		BGBDT_TagStr_Symbol(val));
}

void BS2P_SetAstNodeAttrI(dtVal obj, char *attr, s64 val)
{
	BGBDT_MapObj_BindObjvSlotValueName(obj, attr,
		dtvWrapLongF(val));
}

dtVal BS2P_GetAstNodeAttr(dtVal obj, char *attr)
{
	dtVal val;
	val=BGBDT_MapObj_GetObjvSlotValueName(obj, attr);
	return(val);
}

char *BS2P_GetAstNodeAttrS(dtVal obj, char *attr)
{
	dtVal val;
	val=BGBDT_MapObj_GetObjvSlotValueName(obj, attr);
	return(BGBDT_TagStr_GetUtf8(val));
}

s64 BS2P_GetAstNodeAttrI(dtVal obj, char *attr)
{
	dtVal val;
	val=BGBDT_MapObj_GetObjvSlotValueName(obj, attr);
	return(dtvUnwrapLong(val));
}

int BS2P_CheckAstNodeTag(dtVal obj, char *tag)
{
	char *s0;

	if(!BGBDT_MapObj_IsObjectP(obj))
		return(0);
	s0=BS2P_GetAstNodeAttrS(obj, "tag");
	if(s0 && !strcmp(s0, tag))
		return(1);
	return(0);
}

char *BS2P_GetAstNodeTag(dtVal obj)
{
	char *s0;

	if(!BGBDT_MapObj_IsObjectP(obj))
		return(NULL);
	s0=BS2P_GetAstNodeAttrS(obj, "tag");
	return(s0);
}

dtVal BS2P_NewAstArray(int sz)
{
	dtVal arv;

	arv=BGBDT_TagArr_NewArray(sz, BGBDT_BASETY_ADDRESS);
	return(arv);
}

int BS2P_GetAstArraySize(dtVal obj)
{
	return(dtvArrayGetSize(obj));
}

dtVal BS2P_GetAstArrayIdx(dtVal obj, int idx)
{
	return(dtvArrayGetIndexDtVal(obj, idx));
//	dtvArraySetIndexDtVal(obj, idx, val);
}

void BS2P_SetAstArrayIdx(dtVal obj, int idx, dtVal val)
{
	dtvArraySetIndexDtVal(obj, idx, val);
}

dtVal BS2P_NewAstWrapArray(dtVal *arrd, int sz)
{
	dtVal n0;
	int i;

	n0=BS2P_NewAstArray(sz);
	for(i=0; i<sz; i++)
	{
		BS2P_SetAstArrayIdx(n0, i, arrd[i]);
	}

	return(n0);
}


dtVal BS2P_ParseWrapString(BS2CC_CompileContext *ctx, char *str)
{
	char *s;
	
	s=str;
	while(*s && ((*(byte *)s)<0x80))s++;

	if(*s)
		{ return(BGBDT_TagStr_String(str)); }

	return(BGBDT_TagStr_StringAsc(str));
}

dtVal BS2P_ParseWrapIdentifier(BS2CC_CompileContext *ctx, char *str)
{
	dtVal obj;
	char *s;
	
//	obj=BS2P_NewAstNode("ident");
//	BS2P_SetAstNodeAttrS(obj, "value", str);
//	return(obj);
	return(BGBDT_TagStr_Symbol(str));
}

dtVal BS2P_ParseWrapQName(BS2CC_CompileContext *ctx, char *str)
{
	return(BGBDT_TagStr_Symbol(str));
}

dtVal BS2P_ParseWrapSimpleCmd(BS2CC_CompileContext *ctx, char *str)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("simplecmd");
	BS2P_SetAstNodeAttrS(obj, "cmd", str);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleCmdInt(BS2CC_CompileContext *ctx,
	char *str, int val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("simplecmdi");
	BS2P_SetAstNodeAttrS(obj, "cmd", str);
	BS2P_SetAstNodeAttrI(obj, "value", val);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleCmdVal(BS2CC_CompileContext *ctx,
	char *str, dtVal val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("simplecmdv");
	BS2P_SetAstNodeAttrS(obj, "cmd", str);
	BS2P_SetAstNodeAttr(obj, "value", val);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleCmdStr(BS2CC_CompileContext *ctx,
	char *str, char *sval)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("simplecmds");
	BS2P_SetAstNodeAttrS(obj, "cmd", str);
	BS2P_SetAstNodeAttrS(obj, "value", sval);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleTag(BS2CC_CompileContext *ctx, char *tag)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(tag);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleTagInt(BS2CC_CompileContext *ctx,
	char *tag, int val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(tag);
	BS2P_SetAstNodeAttrI(obj, "value", val);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleTagVal(BS2CC_CompileContext *ctx,
	char *tag, dtVal val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(tag);
	BS2P_SetAstNodeAttr(obj, "value", val);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleTagStr(BS2CC_CompileContext *ctx,
	char *tag, char *sval)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(tag);
	BS2P_SetAstNodeAttrS(obj, "value", sval);
	return(obj);
}



dtVal BS2P_ParseWrapUnTag(BS2CC_CompileContext *ctx,
	char *op, dtVal ln)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(op);
	BS2P_SetAstNodeAttr(obj, "value", ln);
	return(obj);
}

dtVal BS2P_ParseWrapBinTag(BS2CC_CompileContext *ctx,
	char *op, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(op);
	BS2P_SetAstNodeAttr(obj, "lhs", ln);
	BS2P_SetAstNodeAttr(obj, "rhs", rn);
	return(obj);
}

dtVal BS2P_ParseWrapUnary(BS2CC_CompileContext *ctx,
	char *op, dtVal ln)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("unary");
	BS2P_SetAstNodeAttrS(obj, "op", op);
	BS2P_SetAstNodeAttr(obj, "value", ln);
	return(obj);
}

dtVal BS2P_ParseWrapBinary(BS2CC_CompileContext *ctx,
	char *op, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("binary");
	BS2P_SetAstNodeAttrS(obj, "op", op);
	BS2P_SetAstNodeAttr(obj, "lhs", ln);
	BS2P_SetAstNodeAttr(obj, "rhs", rn);
	return(obj);
}

dtVal BS2P_ParseWrapTern(BS2CC_CompileContext *ctx,
	dtVal cc, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("tern");
	BS2P_SetAstNodeAttr(obj, "cond", cc);
	BS2P_SetAstNodeAttr(obj, "lhs", ln);
	BS2P_SetAstNodeAttr(obj, "rhs", rn);
	return(obj);
}

dtVal BS2P_ParseWrapTagIf(BS2CC_CompileContext *ctx,
	char *tag, dtVal cc, dtVal ln)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(tag);
	BS2P_SetAstNodeAttr(obj, "cond", cc);
	BS2P_SetAstNodeAttr(obj, "then", ln);
	return(obj);
}

dtVal BS2P_ParseWrapTagIfElse(BS2CC_CompileContext *ctx,
	char *tag, dtVal cc, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(tag);
	BS2P_SetAstNodeAttr(obj, "cond", cc);
	BS2P_SetAstNodeAttr(obj, "then", ln);
	BS2P_SetAstNodeAttr(obj, "else", rn);
	return(obj);
}

dtVal BS2P_ParseWrapTagFor(BS2CC_CompileContext *ctx,
	char *tag, dtVal in, dtVal cc, dtVal st, dtVal th)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(tag);
	BS2P_SetAstNodeAttr(obj, "init", in);
	BS2P_SetAstNodeAttr(obj, "cond", cc);
	BS2P_SetAstNodeAttr(obj, "step", st);
	BS2P_SetAstNodeAttr(obj, "then", th);
	return(obj);
}

dtVal BS2P_ParseWrapTagBinary(BS2CC_CompileContext *ctx,
	char *tag, char *op, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(tag);
	BS2P_SetAstNodeAttrS(obj, "op", op);
	BS2P_SetAstNodeAttr(obj, "lhs", ln);
	BS2P_SetAstNodeAttr(obj, "rhs", rn);
	return(obj);
}

dtVal BS2P_ParseWrapArray(BS2CC_CompileContext *ctx, dtVal lst)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("array");
	BS2P_SetAstNodeAttr(obj, "value", lst);
	return(obj);
}

dtVal BS2P_ParseWrapObject(BS2CC_CompileContext *ctx, dtVal lst)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode("object");
	BS2P_SetAstNodeAttr(obj, "value", lst);
	return(obj);
}
