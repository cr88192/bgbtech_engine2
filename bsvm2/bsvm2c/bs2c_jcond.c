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

int BS2C_CompileGetCondBool(BS2CC_CompileContext *ctx, dtVal cc)
{
	char *fn;
	int i;

	if(dtvIsBoolP(cc))
	{
		i=dtvUnwrapInt(cc);
		return(i!=0);
	}
	if(dtvIsFixIntP(cc))
	{
		i=dtvUnwrapInt(cc);
		return(i!=0);
	}

	if(BGBDT_TagStr_IsSymbolP(cc))
	{
		fn=BGBDT_TagStr_GetUtf8(cc);
		if(!strcmp(fn, "true"))return(1);
		if(!strcmp(fn, "false"))return(0);
		if(!strcmp(fn, "null"))return(0);
	}

	return(-1);
}

#if 0
void BS2C_CompileTempJmpIfElse(BS2CC_CompileContext *ctx,
	dtVal cc, int tt, int tf)
{
	dtVal ln, rn, cn;
	int t0, t1, t2, t3;
	int lt, rt, cty;
	char *tag;
	char *op;
	int z, o;
	int i;

	i=BS2C_CompileGetCondBool(ctx, cc);
	if(i==1)
	{
		BS2C_EmitTempJump(ctx, tt);
		return;
	}else if(i==0)
	{
		BS2C_EmitTempJump(ctx, tf);
		return;
	}

	tag=BS2P_GetAstNodeTag(cc);
	if(!tag)
	{
		BS2C_EmitTempJump(ctx, tt);
//		BS2C_CaseError(ctx);
		return;
	}

	if(!strcmp(tag, "bincmp"))
	{
		ln=BS2P_GetAstNodeAttr(cc, "lhs");
		rn=BS2P_GetAstNodeAttr(cc, "rhs");
		op=BS2P_GetAstNodeAttrS(cc, "op");

		ln=BS2C_ReduceExpr(ctx, ln);
		rn=BS2C_ReduceExpr(ctx, rn);

		lt=BS2C_InferExpr(ctx, ln);
		rt=BS2C_InferExpr(ctx, rn);
//		cty=BS2C_InferSuperType(ctx, lt, rt);
		cty=BS2C_TypeCompareSuperType(ctx, lt, rt);

		BS2C_CompileExpr(ctx, ln, cty);
		BS2C_CompileExpr(ctx, rn, cty);

		o=-1;
		if(!strcmp(op, "=="))
			o=BSVM2_OP_JEQ;
		if(!strcmp(op, "!="))
			o=BSVM2_OP_JNE;
		if(!strcmp(op, "<"))
			o=BSVM2_OP_JLT;
		if(!strcmp(op, ">"))
			o=BSVM2_OP_JGT;
		if(!strcmp(op, "<="))
			o=BSVM2_OP_JLE;
		if(!strcmp(op, ">="))
			o=BSVM2_OP_JGE;
		if(!strcmp(op, "==="))
			o=BSVM2_OP_JEQ;
		if(!strcmp(op, "!=="))
			o=BSVM2_OP_JNE;

		if(BS2C_TypeSmallIntP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPI);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitTempJAddr(ctx, tt);
			BS2C_EmitTempJump(ctx, tf);
			BS2C_CompileExprPopType2(ctx);
			return;
		}

		if(BS2C_TypeSmallLongP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPL);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitTempJAddr(ctx, tt);
			BS2C_EmitTempJump(ctx, tf);
			BS2C_CompileExprPopType2(ctx);
			return;
		}

		if(BS2C_TypeSmallFloatP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPF);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitTempJAddr(ctx, tt);
			BS2C_EmitTempJump(ctx, tf);
			BS2C_CompileExprPopType2(ctx);
			return;
		}

		if(BS2C_TypeSmallDoubleP(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPD);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitTempJAddr(ctx, tt);
			BS2C_EmitTempJump(ctx, tf);
			BS2C_CompileExprPopType2(ctx);
			return;
		}

		if(BS2C_TypeInt128P(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPXL);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitTempJAddr(ctx, tt);
			BS2C_EmitTempJump(ctx, tf);
			BS2C_CompileExprPopType2(ctx);
			return;
		}

		if(BS2C_TypeFloat128P(ctx, cty))
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_CMPXF);
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitTempJAddr(ctx, tt);
			BS2C_EmitTempJump(ctx, tf);
			BS2C_CompileExprPopType2(ctx);
			return;
		}

		if(BS2C_TypeAddressP(ctx, cty))
		{
			if(!strcmp(op, "===") || !strcmp(op, "!=="))
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_CMP2A); }
			else
				{ BS2C_EmitOpcode(ctx, BSVM2_OP_CMPA); }
			BS2C_EmitOpcode(ctx, o);
			BS2C_EmitTempJAddr(ctx, tt);
			BS2C_EmitTempJump(ctx, tf);
			BS2C_CompileExprPopType2(ctx);
			return;
		}

		BS2C_CaseError(ctx);
		return;
	}

	BS2C_CaseError(ctx);
}
#endif

void BS2C_CompileTempJmpIfElse(BS2CC_CompileContext *ctx,
	dtVal cc, int tt, int tf)
{
	dtVal ln, rn, cn;
	int t0, t1, t2, t3;
	int lt, rt, cty;
	char *tag;
	char *op;
	int z, o;
	int i;

	i=BS2C_CompileGetCondBool(ctx, cc);
	if(i==1)
	{
		BS2C_EmitTempJump(ctx, tt);
		return;
	}else if(i==0)
	{
		BS2C_EmitTempJump(ctx, tf);
		return;
	}

	tag=BS2P_GetAstNodeTag(cc);
	if(tag)
	{
		if(!strcmp(tag, "unary"))
		{
			ln=BS2P_GetAstNodeAttr(cc, "value");
			op=BS2P_GetAstNodeAttrS(cc, "op");

			if(!strcmp(op, "!"))
			{
				ln=BS2C_ReduceExpr(ctx, ln);
				BS2C_CompileTempJmpIfElse(ctx, ln, tf, tt);
				return;
			}
		}
	}

//	BS2C_CompileTempJmpThen(ctx, cc, tt);
	BS2C_CompileTempJmpThen2(ctx, cc, tt, tf);
	BS2C_EmitTempJump(ctx, tf);
}

void BS2C_CompileTempJmpThen(BS2CC_CompileContext *ctx,
	dtVal cc, int tt)
{
	BS2C_CompileTempJmpThen2(ctx, cc, tt, 0);
}

void BS2C_CompileTempJmpThen2(BS2CC_CompileContext *ctx,
	dtVal cc, int tt, int tfa)
{
	dtVal ln, rn, cn;
	s64 li;
	int t0, t1, t2, t3;
	int lt, rt, cty;
	char *tag;
	char *op;
	int z, o, o2;
	int i;

	i=BS2C_CompileGetCondBool(ctx, cc);
	if(i==1)
	{
		BS2C_EmitTempJump(ctx, tt);
		return;
	}else if(i==0)
	{
//		BS2C_EmitTempJump(ctx, tf);
		return;
	}

	tag=BS2P_GetAstNodeTag(cc);
	
//	if(!tag)
//	{
//		BS2C_EmitTempJump(ctx, tt);
//		return;
//	}
	
	if(tag)
	{
		if(!strcmp(tag, "bincmp"))
		{
			ln=BS2P_GetAstNodeAttr(cc, "lhs");
			rn=BS2P_GetAstNodeAttr(cc, "rhs");
			op=BS2P_GetAstNodeAttrS(cc, "op");

			ln=BS2C_ReduceExpr(ctx, ln);
			rn=BS2C_ReduceExpr(ctx, rn);

			lt=BS2C_InferExpr(ctx, ln);
			rt=BS2C_InferExpr(ctx, rn);
//			cty=BS2C_InferSuperType(ctx, lt, rt);
			cty=BS2C_TypeCompareSuperType(ctx, lt, rt);

			o=-1;
			if(!strcmp(op, "=="))
				{ o=BSVM2_OP_JEQ; o2=o; }
			if(!strcmp(op, "!="))
				{ o=BSVM2_OP_JNE; o2=o; }
			if(!strcmp(op, "<"))
				{ o=BSVM2_OP_JLT; o2=BSVM2_OP_JGT; }
			if(!strcmp(op, ">"))
				{ o=BSVM2_OP_JGT; o2=BSVM2_OP_JLT; }
			if(!strcmp(op, "<="))
				{ o=BSVM2_OP_JLE; o2=BSVM2_OP_JGE; }
			if(!strcmp(op, ">="))
				{ o=BSVM2_OP_JGE; o2=BSVM2_OP_JLE; }
			if(!strcmp(op, "==="))
				{ o=BSVM2_OP_JEQ; o2=o; }
			if(!strcmp(op, "!=="))
				{ o=BSVM2_OP_JNE; o2=o; }

			if(dtvIsSmallLongP(rn) && !BS2C_TypeUnsigned2P(ctx, cty))
			{
				li=dtvUnwrapLong(rn);
			
				t0=BS2C_InferExprLocalIndex(ctx, ln);
				if(BS2C_TypeSmallIntP(ctx, cty) && (t0>=0))
				{
					BS2C_CompileExprResvPush(ctx, 1);
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPILC);
					BS2C_EmitOpcodeSKx(ctx, t0, li);
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitTempJAddr(ctx, tt);
					return;
				}
			
				if(BS2C_TypeSmallIntP(ctx, cty))
				{
					BS2C_CompileExpr(ctx, ln, cty);

//					BS2C_CompileExprResvPush(ctx, 1);
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPIC);
					BS2C_EmitOpcodeSCx(ctx, li);

					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitTempJAddr(ctx, tt);
					BS2C_CompileExprPopType1(ctx);
					return;
				}
			}

			if(dtvIsSmallLongP(ln) && !BS2C_TypeUnsigned2P(ctx, cty))
			{
				li=dtvUnwrapLong(ln);
			
				t0=BS2C_InferExprLocalIndex(ctx, rn);
				if(BS2C_TypeSmallIntP(ctx, cty) && (t0>=0))
				{
					BS2C_CompileExprResvPush(ctx, 1);
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPILC);
					BS2C_EmitOpcodeSKx(ctx, t0, li);
					BS2C_EmitOpcode(ctx, o2);
					BS2C_EmitTempJAddr(ctx, tt);
					return;
				}
			
				if(BS2C_TypeSmallIntP(ctx, cty))
				{
					BS2C_CompileExpr(ctx, rn, cty);

//					BS2C_CompileExprResvPush(ctx, 1);
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPIC);
					BS2C_EmitOpcodeSCx(ctx, li);

					BS2C_EmitOpcode(ctx, o2);
					BS2C_EmitTempJAddr(ctx, tt);
					BS2C_CompileExprPopType1(ctx);
					return;
				}
			}

#if 1
			t0=BS2C_InferExprLocalIndex(ctx, ln);
			t1=BS2C_InferExprLocalIndex(ctx, rn);
			if(BS2C_TypeSmallIntP(ctx, cty) && (t0>=0) && (t1>=0) &&
				!BS2C_TypeUnsigned2P(ctx, cty))
			{
				BS2C_CompileExprResvPush(ctx, 1);
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPILL);
				BS2C_EmitOpcodeJx(ctx, t0, t1);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tt);
				return;
			}
#endif

			BS2C_CompileExpr(ctx, ln, cty);
			BS2C_CompileExpr(ctx, rn, cty);

			if(BS2C_TypeSmallIntP(ctx, cty))
			{
				if(BS2C_TypeUnsigned2P(ctx, cty))
					BS2C_EmitOpcode(ctx, BSVM2_OP_UCMPI);
				else
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPI);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tt);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeSmallLongP(ctx, cty))
			{
				if(BS2C_TypeUnsigned2P(ctx, cty))
					BS2C_EmitOpcode(ctx, BSVM2_OP_UCMPL);
				else
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPL);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tt);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeSmallFloatP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPF);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tt);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeSmallDoubleP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPD);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tt);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeInt128P(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPXL);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tt);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeFloat128P(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPXF);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tt);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeAddressP(ctx, cty))
			{
				if(!strcmp(op, "===") || !strcmp(op, "!=="))
					{ BS2C_EmitOpcode(ctx, BSVM2_OP_CMP2A); }
				else
				{
					if(BS2C_TypePointerP(ctx, cty))
						BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPA);
				}
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tt);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(!strcmp(tag, "unary"))
		{
			ln=BS2P_GetAstNodeAttr(cc, "value");
			op=BS2P_GetAstNodeAttrS(cc, "op");

			if(!strcmp(op, "!"))
			{
				ln=BS2C_ReduceExpr(ctx, ln);
//				BS2C_CompileTempJmpElse(ctx, ln, tt);
				BS2C_CompileTempJmpElse2(ctx, ln, tt, tfa);
				return;
			}
		}

		if(!strcmp(tag, "binary"))
		{
			ln=BS2P_GetAstNodeAttr(cc, "lhs");
			rn=BS2P_GetAstNodeAttr(cc, "rhs");
			op=BS2P_GetAstNodeAttrS(cc, "op");

			if(!strcmp(op, "&&"))
			{
				ln=BS2C_ReduceExpr(ctx, ln);
				rn=BS2C_ReduceExpr(ctx, rn);

				if(tfa>0)
				{
					BS2C_CompileTempJmpElse(ctx, ln, tfa);
					BS2C_CompileTempJmpThen2(ctx, rn, tt, tfa);
					return;
				}
			
				t0=BS2C_GenTempLabel(ctx);
				BS2C_CompileTempJmpElse(ctx, ln, t0);
				BS2C_CompileTempJmpThen2(ctx, rn, tt, t0);
				BS2C_EmitTempLabelB(ctx, t0);
				return;
			}

			if(!strcmp(op, "||"))
			{
				ln=BS2C_ReduceExpr(ctx, ln);
				rn=BS2C_ReduceExpr(ctx, rn);
				BS2C_CompileTempJmpThen(ctx, ln, tt);
				BS2C_CompileTempJmpThen2(ctx, rn, tt, tfa);
				return;
			}
		}
	}

	cty=BS2C_InferExpr(ctx, cc);
	if(BS2C_TypeSmallIntP(ctx, cty))
	{
		BS2C_CompileExpr(ctx, cc, cty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_JNE);
		BS2C_EmitTempJAddr(ctx, tt);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeSmallLongP(ctx, cty))
	{
//		BS2C_CompileExpr(ctx, cc, BS2CC_TYZ_INT);
		
		BS2C_CompileExpr(ctx, cc, cty);
		BS2C_CompilePushDummy(ctx, BS2CC_TYZ_LONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_CMPL);
		BS2C_CompileExprPopType1(ctx);
		
		BS2C_EmitOpcode(ctx, BSVM2_OP_JNE);
		BS2C_EmitTempJAddr(ctx, tt);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeAddressP(ctx, cty))
	{
		t0=BS2C_InferExprLocalIndex(ctx, cc);
		if(t0>=0)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_JNENULLL);
			BS2C_EmitOpcodeUCx(ctx, t0);
			BS2C_EmitTempJAddr(ctx, tt);
			return;
		}

		BS2C_CompileExpr(ctx, cc, cty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_JNENULL);
		BS2C_EmitTempJAddr(ctx, tt);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	BS2C_CaseError(ctx);
}

void BS2C_CompileTempJmpElse(BS2CC_CompileContext *ctx,
	dtVal cc, int tf)
{
	BS2C_CompileTempJmpElse2(ctx, cc, tf, 0);
}

void BS2C_CompileTempJmpElse2(BS2CC_CompileContext *ctx,
	dtVal cc, int tf, int tta)
{
	dtVal ln, rn, cn;
	s64 li;
	int t0, t1, t2, t3;
	int lt, rt, cty;
	char *tag;
	char *op;
	int z, o, o2;
	int i;

	i=BS2C_CompileGetCondBool(ctx, cc);
	if(i==1)
	{
//		BS2C_EmitTempJump(ctx, tt);
		return;
	}else if(i==0)
	{
		BS2C_EmitTempJump(ctx, tf);
		return;
	}

	tag=BS2P_GetAstNodeTag(cc);
		
	if(tag)
	{
		if(!strcmp(tag, "bincmp"))
		{
			ln=BS2P_GetAstNodeAttr(cc, "lhs");
			rn=BS2P_GetAstNodeAttr(cc, "rhs");
			op=BS2P_GetAstNodeAttrS(cc, "op");

			ln=BS2C_ReduceExpr(ctx, ln);
			rn=BS2C_ReduceExpr(ctx, rn);

			lt=BS2C_InferExpr(ctx, ln);
			rt=BS2C_InferExpr(ctx, rn);
//			cty=BS2C_InferSuperType(ctx, lt, rt);
			cty=BS2C_TypeCompareSuperType(ctx, lt, rt);

			o=-1;
			if(!strcmp(op, "=="))
				{ o=BSVM2_OP_JNE; o2=o; }
			if(!strcmp(op, "!="))
				{ o=BSVM2_OP_JEQ; o2=o; }
			if(!strcmp(op, "<"))
				{ o=BSVM2_OP_JGE; o2=BSVM2_OP_JLE; }
			if(!strcmp(op, ">"))
				{ o=BSVM2_OP_JLE; o2=BSVM2_OP_JGE; }
			if(!strcmp(op, "<="))
				{ o=BSVM2_OP_JGT; o2=BSVM2_OP_JLT; }
			if(!strcmp(op, ">="))
				{ o=BSVM2_OP_JLT; o2=BSVM2_OP_JGT; }
			if(!strcmp(op, "==="))
				{ o=BSVM2_OP_JNE; o2=o; }
			if(!strcmp(op, "!=="))
				{ o=BSVM2_OP_JEQ; o2=o; }

			if(dtvIsSmallLongP(rn) && !BS2C_TypeUnsigned2P(ctx, cty))
			{
				li=dtvUnwrapLong(rn);

				t0=BS2C_InferExprLocalIndex(ctx, ln);
				if(BS2C_TypeSmallIntP(ctx, cty) && (t0>=0))
				{
					BS2C_CompileExprResvPush(ctx, 1);
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPILC);
					BS2C_EmitOpcodeSKx(ctx, t0, li);
					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitTempJAddr(ctx, tf);
					return;
				}
			
				if(BS2C_TypeSmallIntP(ctx, cty))
				{
					BS2C_CompileExpr(ctx, ln, cty);

					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPIC);
					BS2C_EmitOpcodeSCx(ctx, li);

					BS2C_EmitOpcode(ctx, o);
					BS2C_EmitTempJAddr(ctx, tf);
					BS2C_CompileExprPopType1(ctx);
					return;
				}
			}

			if(dtvIsSmallLongP(ln) && !BS2C_TypeUnsigned2P(ctx, cty))
			{
				li=dtvUnwrapLong(ln);
			
				t0=BS2C_InferExprLocalIndex(ctx, rn);
				if(BS2C_TypeSmallIntP(ctx, cty) && (t0>=0))
				{
					BS2C_CompileExprResvPush(ctx, 1);
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPILC);
					BS2C_EmitOpcodeSKx(ctx, t0, li);
					BS2C_EmitOpcode(ctx, o2);
					BS2C_EmitTempJAddr(ctx, tf);
					return;
				}
			
				if(BS2C_TypeSmallIntP(ctx, cty))
				{
					BS2C_CompileExpr(ctx, rn, cty);

					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPIC);
					BS2C_EmitOpcodeSCx(ctx, li);

					BS2C_EmitOpcode(ctx, o2);
					BS2C_EmitTempJAddr(ctx, tf);
					BS2C_CompileExprPopType1(ctx);
					return;
				}
			}

#if 1
			t0=BS2C_InferExprLocalIndex(ctx, ln);
			t1=BS2C_InferExprLocalIndex(ctx, rn);
			if(BS2C_TypeSmallIntP(ctx, cty) && (t0>=0) && (t1>=0) &&
				!BS2C_TypeUnsigned2P(ctx, cty))
			{
				BS2C_CompileExprResvPush(ctx, 1);
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPILL);
				BS2C_EmitOpcodeJx(ctx, t0, t1);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tf);
				return;
			}
#endif

			BS2C_CompileExpr(ctx, ln, cty);
			BS2C_CompileExpr(ctx, rn, cty);

			if(BS2C_TypeSmallIntP(ctx, cty))
			{
				if(BS2C_TypeUnsigned2P(ctx, cty))
					BS2C_EmitOpcode(ctx, BSVM2_OP_UCMPI);
				else
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPI);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tf);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeSmallLongP(ctx, cty))
			{
				if(BS2C_TypeUnsigned2P(ctx, cty))
					BS2C_EmitOpcode(ctx, BSVM2_OP_UCMPL);
				else
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPL);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tf);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeSmallFloatP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPF);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tf);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeSmallDoubleP(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPD);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tf);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeInt128P(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPXL);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tf);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeFloat128P(ctx, cty))
			{
				BS2C_EmitOpcode(ctx, BSVM2_OP_CMPXF);
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tf);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			if(BS2C_TypeAddressP(ctx, cty))
			{
				if(!strcmp(op, "===") || !strcmp(op, "!=="))
					{ BS2C_EmitOpcode(ctx, BSVM2_OP_CMP2A); }
				else
				{
					if(BS2C_TypePointerP(ctx, cty))
						BS2C_EmitOpcode(ctx, BSVM2_OP_HPTR);
					BS2C_EmitOpcode(ctx, BSVM2_OP_CMPA);
				}
				BS2C_EmitOpcode(ctx, o);
				BS2C_EmitTempJAddr(ctx, tf);
				BS2C_CompileExprPopType2(ctx);
				return;
			}

			BS2C_CaseError(ctx);
			return;
		}

		if(!strcmp(tag, "unary"))
		{
			ln=BS2P_GetAstNodeAttr(cc, "value");
			op=BS2P_GetAstNodeAttrS(cc, "op");

			if(!strcmp(op, "!"))
			{
				ln=BS2C_ReduceExpr(ctx, ln);
//				BS2C_CompileTempJmpThen(ctx, ln, tf);
				BS2C_CompileTempJmpThen2(ctx, ln, tf, tta);
				return;
			}
		}

		if(!strcmp(tag, "binary"))
		{
			ln=BS2P_GetAstNodeAttr(cc, "lhs");
			rn=BS2P_GetAstNodeAttr(cc, "rhs");
			op=BS2P_GetAstNodeAttrS(cc, "op");

	//		ln=BS2C_ReduceExpr(ctx, ln);
	//		rn=BS2C_ReduceExpr(ctx, rn);

	//		lt=BS2C_InferExpr(ctx, ln);
	//		rt=BS2C_InferExpr(ctx, rn);
			
			if(!strcmp(op, "&&"))
			{
				ln=BS2C_ReduceExpr(ctx, ln);
				rn=BS2C_ReduceExpr(ctx, rn);
				BS2C_CompileTempJmpElse(ctx, ln, tf);
				BS2C_CompileTempJmpElse2(ctx, rn, tf, tta);
				return;
			}

			if(!strcmp(op, "||"))
			{
				ln=BS2C_ReduceExpr(ctx, ln);
				rn=BS2C_ReduceExpr(ctx, rn);

				if(tta>0)
				{
					BS2C_CompileTempJmpThen(ctx, ln, tta);
					BS2C_CompileTempJmpElse2(ctx, rn, tf, tta);
					return;
				}
			
				t0=BS2C_GenTempLabel(ctx);
				BS2C_CompileTempJmpThen(ctx, ln, t0);
				BS2C_CompileTempJmpElse2(ctx, rn, tf, t0);
				BS2C_EmitTempLabelB(ctx, t0);
				return;
			}
		}
	}

	cty=BS2C_InferExpr(ctx, cc);
	if(BS2C_TypeSmallIntP(ctx, cty))
	{
		BS2C_CompileExpr(ctx, cc, cty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_JEQ);
		BS2C_EmitTempJAddr(ctx, tf);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeSmallLongP(ctx, cty))
	{
//		BS2C_CompileExpr(ctx, cc, BS2CC_TYZ_INT);
		BS2C_CompileExpr(ctx, cc, cty);
		BS2C_CompilePushDummy(ctx, BS2CC_TYZ_LONG);
		BS2C_EmitOpcode(ctx, BSVM2_OP_CMPL);
		BS2C_CompileExprPopType1(ctx);

		BS2C_EmitOpcode(ctx, BSVM2_OP_JEQ);
		BS2C_EmitTempJAddr(ctx, tf);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	if(BS2C_TypeAddressP(ctx, cty))
	{
		t0=BS2C_InferExprLocalIndex(ctx, cc);
		if(t0>=0)
		{
			BS2C_EmitOpcode(ctx, BSVM2_OP_JEQNULLL);
			BS2C_EmitOpcodeUCx(ctx, t0);
			BS2C_EmitTempJAddr(ctx, tf);
			return;
		}

		BS2C_CompileExpr(ctx, cc, cty);
		BS2C_EmitOpcode(ctx, BSVM2_OP_JEQNULL);
		BS2C_EmitTempJAddr(ctx, tf);
		BS2C_CompileExprPopType1(ctx);
		return;
	}

	BS2C_CaseError(ctx);
}
