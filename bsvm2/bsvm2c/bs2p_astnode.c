/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

char *BS2P_GenSym(BS2CC_CompileContext *ctx)
{
	char tb[256];
	
	sprintf(tb, "$GS$%08X_%04X", ctx->gshash, ctx->gsseq++);
	return(BS2P_StrSym(ctx, tb));
}

char *BS2P_StrSym(BS2CC_CompileContext *ctx, char *str)
{
	return(BGBDT_TagStr_StrSymbol(str));
}

dtVal BS2P_NewAstNode(BS2CC_CompileContext *ctx, char *tag)
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

void BS2P_SetAstNodeAttrStr(dtVal obj, char *attr, char *val)
{
	BGBDT_MapObj_BindObjvSlotValueName(obj, attr,
		BGBDT_TagStr_String(val));
//	BGBDT_MapObj_BindObjvSlotValueName(obj, attr,
//		BGBDT_TagStr_Symbol(val));
}

void BS2P_SetAstNodeAttrI(dtVal obj, char *attr, s64 val)
{
	BGBDT_MapObj_BindObjvSlotValueName(obj, attr,
		dtvWrapLongF(val));
}

void BS2P_SetAstNodeAttrF(dtVal obj, char *attr, double val)
{
	BGBDT_MapObj_BindObjvSlotValueName(obj, attr,
		dtvWrapDoubleF(val));
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

f64 BS2P_GetAstNodeAttrF(dtVal obj, char *attr)
{
	dtVal val;
	val=BGBDT_MapObj_GetObjvSlotValueName(obj, attr);
	return(dtvUnwrapDouble(val));
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

dtVal BS2P_NewAstArray(BS2CC_CompileContext *ctx, int sz)
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

dtVal BS2P_NewAstWrapArray(BS2CC_CompileContext *ctx, dtVal *arrd, int sz)
{
	dtVal n0;
	int i;

	n0=BS2P_NewAstArray(ctx, sz);
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
	
	obj=BS2P_NewAstNode(ctx, "simplecmd");
	BS2P_SetAstNodeAttrS(obj, "cmd", str);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleCmdInt(BS2CC_CompileContext *ctx,
	char *str, int val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "simplecmdi");
	BS2P_SetAstNodeAttrS(obj, "cmd", str);
	BS2P_SetAstNodeAttrI(obj, "value", val);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleCmdVal(BS2CC_CompileContext *ctx,
	char *str, dtVal val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "simplecmdv");
	BS2P_SetAstNodeAttrS(obj, "cmd", str);
	BS2P_SetAstNodeAttr(obj, "value", val);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleCmdStr(BS2CC_CompileContext *ctx,
	char *str, char *sval)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "simplecmds");
	BS2P_SetAstNodeAttrS(obj, "cmd", str);
	BS2P_SetAstNodeAttrS(obj, "value", sval);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleTag(BS2CC_CompileContext *ctx, char *tag)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, tag);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleTagInt(BS2CC_CompileContext *ctx,
	char *tag, int val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, tag);
	BS2P_SetAstNodeAttrI(obj, "value", val);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleTagVal(BS2CC_CompileContext *ctx,
	char *tag, dtVal val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, tag);
	BS2P_SetAstNodeAttr(obj, "value", val);
	return(obj);
}

dtVal BS2P_ParseWrapSimpleTagStr(BS2CC_CompileContext *ctx,
	char *tag, char *sval)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, tag);
	BS2P_SetAstNodeAttrS(obj, "value", sval);
	return(obj);
}



dtVal BS2P_ParseWrapUnTag(BS2CC_CompileContext *ctx,
	char *op, dtVal ln)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, op);
	BS2P_SetAstNodeAttr(obj, "value", ln);
	return(obj);
}

dtVal BS2P_ParseWrapBinTag(BS2CC_CompileContext *ctx,
	char *op, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, op);
	BS2P_SetAstNodeAttr(obj, "lhs", ln);
	BS2P_SetAstNodeAttr(obj, "rhs", rn);
	return(obj);
}

dtVal BS2P_ParseWrapUnary(BS2CC_CompileContext *ctx,
	char *op, dtVal ln)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "unary");
	BS2P_SetAstNodeAttrS(obj, "op", op);
	BS2P_SetAstNodeAttr(obj, "value", ln);
	return(obj);
}

dtVal BS2P_ParseWrapBinary(BS2CC_CompileContext *ctx,
	char *op, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "binary");
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
	
	obj=BS2P_NewAstNode(ctx, "tern");
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
	
	obj=BS2P_NewAstNode(ctx, tag);
	BS2P_SetAstNodeAttr(obj, "cond", cc);
	BS2P_SetAstNodeAttr(obj, "then", ln);
	return(obj);
}

dtVal BS2P_ParseWrapTagIfElse(BS2CC_CompileContext *ctx,
	char *tag, dtVal cc, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, tag);
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
	
	obj=BS2P_NewAstNode(ctx, tag);
	BS2P_SetAstNodeAttr(obj, "init", in);
	BS2P_SetAstNodeAttr(obj, "cond", cc);
	BS2P_SetAstNodeAttr(obj, "step", st);
	BS2P_SetAstNodeAttr(obj, "then", th);
	return(obj);
}

dtVal BS2P_ParseWrapTagBody(BS2CC_CompileContext *ctx,
	char *tag, dtVal ln)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, tag);
	BS2P_SetAstNodeAttr(obj, "body", ln);
	return(obj);
}

dtVal BS2P_ParseWrapTagCatch(BS2CC_CompileContext *ctx,
	char *tag, dtVal cc, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, tag);
	BS2P_SetAstNodeAttr(obj, "cond", cc);
	BS2P_SetAstNodeAttr(obj, "lhs", ln);
	BS2P_SetAstNodeAttr(obj, "rhs", rn);
	return(obj);
}

dtVal BS2P_ParseWrapTagBinary(BS2CC_CompileContext *ctx,
	char *tag, char *op, dtVal ln, dtVal rn)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, tag);
	if(op)BS2P_SetAstNodeAttrS(obj, "op", op);
	BS2P_SetAstNodeAttr(obj, "lhs", ln);
	BS2P_SetAstNodeAttr(obj, "rhs", rn);
	return(obj);
}

dtVal BS2P_ParseWrapArray(BS2CC_CompileContext *ctx, dtVal lst)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "array");
	BS2P_SetAstNodeAttr(obj, "value", lst);
	return(obj);
}

dtVal BS2P_ParseWrapObject(BS2CC_CompileContext *ctx, dtVal lst)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "object");
	BS2P_SetAstNodeAttr(obj, "value", lst);
	return(obj);
}

dtVal BS2P_ParseWrapVector(BS2CC_CompileContext *ctx, dtVal lst)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "vector");
	BS2P_SetAstNodeAttr(obj, "value", lst);
	return(obj);
}


dtVal BS2P_ParseWrapImaginary(BS2CC_CompileContext *ctx,
	char *sfx, double val)
{
	dtVal obj;
	char *s;
	
	obj=BS2P_NewAstNode(ctx, "imag");
	BS2P_SetAstNodeAttrF(obj, "value", val);

	if(sfx)
	{
		if(!strcmp(sfx, "j"))sfx="J";
		if(!strcmp(sfx, "jf"))sfx="JF";
		if(!strcmp(sfx, "jd"))sfx="JD";
		BS2P_SetAstNodeAttrS(obj, "sfx", sfx);
	}
	return(obj);
}
