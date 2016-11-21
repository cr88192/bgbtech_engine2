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


BS2VM_API void BSVM2_Op_MKV2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].ui=frm->stack[op->t0].i;
	frm->stack[op->t0].uj=frm->stack[op->t1].i;
}

BS2VM_API void BSVM2_Op_MKV2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].fx=frm->stack[op->t0].f;
	frm->stack[op->t0].fy=frm->stack[op->t1].f;
}

BS2VM_API void BSVM2_Op_CVTVI2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
//	frm->stack[op->t0].ui=frm->stack[op->t0].i;
//	frm->stack[op->t0].uj=frm->stack[op->t1].i;
}

BS2VM_API void BSVM2_Op_CVTVF2AA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].a=dtvWrapVec2f(
		frm->stack[op->t0].f,
		frm->stack[op->t1].f);
}

BS2VM_API void BSVM2_Op_CVTAA2VI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
//	frm->stack[op->t0].ui=frm->stack[op->t0].i;
//	frm->stack[op->t0].uj=frm->stack[op->t1].i;
}

BS2VM_API void BSVM2_Op_CVTAA2VF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	float fv[2];
	dtvUnwrapVec2fv(frm->stack[op->t0].a, fv);
	frm->stack[op->t0].fx=fv[0];
	frm->stack[op->t0].fy=fv[1];
}

BS2VM_API void BSVM2_Op_BINOPX_ADD_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].fx+=frm->stack[op->t1].fx;
	frm->stack[op->t0].fy+=frm->stack[op->t1].fy;	}
BS2VM_API void BSVM2_Op_BINOPX_SUB_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].fx-=frm->stack[op->t1].fx;
	frm->stack[op->t0].fy-=frm->stack[op->t1].fy;	}
BS2VM_API void BSVM2_Op_BINOPX_MUL_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].fx*=frm->stack[op->t1].fx;
	frm->stack[op->t0].fy*=frm->stack[op->t1].fy;	}
BS2VM_API void BSVM2_Op_BINOPX_DIV_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].fx/=frm->stack[op->t1].fx;
	frm->stack[op->t0].fy/=frm->stack[op->t1].fy;	}

BS2VM_API void BSVM2_Op_BINOPX_DOT_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].f=
		(frm->stack[op->t0].fx*frm->stack[op->t1].fx)+
		(frm->stack[op->t0].fy*frm->stack[op->t1].fy);
}

BS2VM_API void BSVM2_Op_BINOPX_CROSS_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].f=
		(frm->stack[op->t0].fx*frm->stack[op->t1].fy)-
		(frm->stack[op->t0].fy*frm->stack[op->t1].fx);
}

BS2VM_API void BSVM2_Op_BINOPX_CMUL_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	float fa, fb, fc, fd;
	float f, g;
	fa=frm->stack[op->t0].fx;
	fb=frm->stack[op->t0].fy;
	fc=frm->stack[op->t1].fx;
	fd=frm->stack[op->t1].fy;
	f=(fa*fc)-(fb*fd);
	g=(fa*fd)+(fb*fc);
	frm->stack[op->t0].fx=f;
	frm->stack[op->t0].fy=g;
}

BS2VM_API void BSVM2_Op_BINOPX_CDIV_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	float fa, fb, fc, fd;
	float f, g, h;

	fa=frm->stack[op->t0].fx;
	fb=frm->stack[op->t0].fy;
	fc=frm->stack[op->t1].fx;
	fd=frm->stack[op->t1].fy;
	h=1.0/(fc*fc+fd*fd);
	f=((fa*fc)+(fb*fd))*h;
	g=((fb*fc)-(fa*fd))*h;
	frm->stack[op->t0].fx=f;
	frm->stack[op->t0].fy=g;
}

float bsvm2_opf_ssqrtf(float x)
{
	return((x>=0)?sqrt(x):(-sqrt(-x)));
}

BS2VM_API void BSVM2_Op_UNOPX_NEG_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].fx=-frm->stack[op->t0].fx;
	frm->stack[op->t0].fy=-frm->stack[op->t0].fy;	}
BS2VM_API void BSVM2_Op_UNOPX_RCP_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].fx=1.0/frm->stack[op->t0].fx;
	frm->stack[op->t0].fy=1.0/frm->stack[op->t0].fy;	}
BS2VM_API void BSVM2_Op_UNOPX_SQRT_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].fx=bsvm2_opf_ssqrtf(frm->stack[op->t0].fx);
	frm->stack[op->t0].fy=bsvm2_opf_ssqrtf(frm->stack[op->t0].fy);	}
BS2VM_API void BSVM2_Op_UNOPX_RSQRT_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	frm->stack[op->t0].fx=bsvm2_opf_ssqrtf(1.0/frm->stack[op->t0].fx);
	frm->stack[op->t0].fy=bsvm2_opf_ssqrtf(1.0/frm->stack[op->t0].fy);	}

BS2VM_API void BSVM2_Op_UNOPX_LEN_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].f=sqrt(
		(frm->stack[op->t0].fx*frm->stack[op->t0].fx)+
		(frm->stack[op->t0].fy*frm->stack[op->t0].fy));
}

BS2VM_API void BSVM2_Op_UNOPX_LEN2_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	frm->stack[op->t0].f=
		(frm->stack[op->t0].fx*frm->stack[op->t0].fx)+
		(frm->stack[op->t0].fy*frm->stack[op->t0].fy);
}

BS2VM_API void BSVM2_Op_LDV2IA_V2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].i; }
BS2VM_API void BSVM2_Op_LDV2IB_V2I(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->stack[op->t0].j; }

BS2VM_API void BSVM2_Op_LDV2FA_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].fx; }
BS2VM_API void BSVM2_Op_LDV2FB_V2F(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->stack[op->t0].fy; }

BSVM2_Opcode *BSVM2_OpVec_DecodeBinOpX(
	BSVM2_CodeBlock *cblk, BSVM2_Opcode *op, int opn)
{
	int z, o;
	
	z=opn&15; o=opn>>4;
	
	switch(z)
	{
	case BSVM2_OPVZ_V2F:
		switch(o)
		{
		case BSVM2_OPRV_ADD:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_ADD_V2F);
			break;
		case BSVM2_OPRV_SUB:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SUB_V2F);
			break;
		case BSVM2_OPRV_MUL:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_MUL_V2F);
			break;
		case BSVM2_OPRV_DIV:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_DIV_V2F);
			break;
		case BSVM2_OPRV_DOT:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_BINOPX_DOT_V2F);
			break;
		case BSVM2_OPRV_CROSS:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_BINOPX_CROSS_V2F);
			break;
		case BSVM2_OPRV_CMUL:
		case BSVM2_OPRV_CMUL2:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CMUL_V2F);
			break;
		case BSVM2_OPRV_CDIV:
		case BSVM2_OPRV_CDIV2:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CDIV_V2F);
			break;
		}
		break;

	case BSVM2_OPVZ_V3F:
		switch(o)
		{
		case BSVM2_OPRV_ADD:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_ADD_V3F);
			break;
		case BSVM2_OPRV_SUB:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SUB_V3F);
			break;
		case BSVM2_OPRV_MUL:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_MUL_V3F);
			break;
		case BSVM2_OPRV_DIV:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_DIV_V3F);
			break;
		case BSVM2_OPRV_DOT:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_BINOPX_DOT_V3F);
			break;
		case BSVM2_OPRV_CROSS:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CROSS_V3F);
			break;
		case BSVM2_OPRV_CMUL:
		case BSVM2_OPRV_CMUL2:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CMUL_V3F);
			break;
		}
		break;

	case BSVM2_OPVZ_V4F:
		switch(o)
		{
		case BSVM2_OPRV_ADD:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_ADD_V4F);
			break;
		case BSVM2_OPRV_SUB:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SUB_V4F);
			break;
		case BSVM2_OPRV_MUL:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_MUL_V4F);
			break;
		case BSVM2_OPRV_DIV:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_DIV_V4F);
			break;
		case BSVM2_OPRV_DOT:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_BINOPX_DOT_V4F);
			break;
		case BSVM2_OPRV_CROSS:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CROSS_V4F);
			break;
		case BSVM2_OPRV_CMUL:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CMUL_V4F);
			break;
		case BSVM2_OPRV_CMUL2:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CMUL2_V4F);
			break;
		case BSVM2_OPRV_CDIV:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CDIV_V4F);
			break;
		case BSVM2_OPRV_CDIV2:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CDIV2_V4F);
			break;
		}
		break;

	case BSVM2_OPVZ_V3XF:
		switch(o)
		{
		case BSVM2_OPRV_ADD:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_ADD_V3XF);
			break;
		case BSVM2_OPRV_SUB:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SUB_V3XF);
			break;
		case BSVM2_OPRV_MUL:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_MUL_V3XF);
			break;
		case BSVM2_OPRV_DIV:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_DIV_V3XF);
			break;
		case BSVM2_OPRV_DOT:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_BINOPX_DOT_V3XF);
			break;
		case BSVM2_OPRV_CROSS:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CROSS_V3XF);
			break;
//		case BSVM2_OPRV_CMUL:
//		case BSVM2_OPRV_CMUL2:
//			BSVM2_Interp_SetupOpBin(cblk, op,
//				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_CMUL_V3XF);
//			break;
		}
		break;

	case BSVM2_OPVZ_I128:
	case BSVM2_OPVZ_UI128:
		switch(o)
		{
		case BSVM2_OPRI_ADD:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_ADD_XI128);
			break;
		case BSVM2_OPRI_SUB:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SUB_XI128);
			break;
		case BSVM2_OPRI_MUL:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_MUL_XI128);
			break;
		case BSVM2_OPRI_AND:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_AND_XI128);
			break;
		case BSVM2_OPRI_OR:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_OR_XI128);
			break;
		case BSVM2_OPRI_XOR:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_XOR_XI128);
			break;
		case BSVM2_OPRI_SHL:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SHL_XI128);
			break;
		case BSVM2_OPRI_SAR:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SAR_XI128);
			break;
		case BSVM2_OPRI_SHR:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SHR_XI128);
			break;
		case BSVM2_OPRI_DIV:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_DIV_XI128);
			break;
		case BSVM2_OPRI_MOD:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_MOD_XI128);
			break;
		}
		break;

	case BSVM2_OPVZ_F128:
		switch(o)
		{
		case BSVM2_OPRI_ADD:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_ADD_XF128);
			break;
		case BSVM2_OPRI_SUB:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_SUB_XF128);
			break;
		case BSVM2_OPRI_MUL:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_MUL_XF128);
			break;
		case BSVM2_OPRI_DIV:
		case BSVM2_OPRF_DIV:
			BSVM2_Interp_SetupOpBin(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_BINOPX_DIV_XF128);
			break;
		}
		break;
	}
	return(op);
}

BSVM2_Opcode *BSVM2_OpVec_DecodeUnOpX(
	BSVM2_CodeBlock *cblk, BSVM2_Opcode *op, int opn)
{
	int z, o;

	z=opn&15; o=opn>>4;

	switch(z)
	{
	case BSVM2_OPVZ_V2F:
		switch(o)
		{
		case BSVM2_OPUV_NEG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_NEG_V2F);
			break;
		case BSVM2_OPUV_RCP:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_RCP_V2F);
			break;
		case BSVM2_OPUV_SQRT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_SQRT_V2F);
			break;
		case BSVM2_OPUV_RSQRT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_RSQRT_V2F);
			break;
		case BSVM2_OPUV_LEN:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_UNOPX_LEN_V2F);
			break;
		case BSVM2_OPUV_LEN2:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_UNOPX_LEN2_V2F);
			break;
		}
		break;

	case BSVM2_OPVZ_V3F:
		switch(o)
		{
		case BSVM2_OPUV_NEG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_NEG_X3F);
			break;
		case BSVM2_OPUV_RCP:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_RCP_X3F);
			break;
		case BSVM2_OPUV_SQRT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_SQRT_X3F);
			break;
		case BSVM2_OPUV_RSQRT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_RSQRT_X3F);
			break;
		case BSVM2_OPUV_LEN:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_UNOPX_LEN_X3F);
			break;
		case BSVM2_OPUV_LEN2:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_UNOPX_LEN2_X3F);
			break;
		}
		break;

	case BSVM2_OPVZ_V4F:
		switch(o)
		{
		case BSVM2_OPUV_NEG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_NEG_X4F);
			break;
		case BSVM2_OPUV_RCP:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_RCP_X4F);
			break;
		case BSVM2_OPUV_SQRT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_SQRT_X4F);
			break;
		case BSVM2_OPUV_RSQRT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_RSQRT_X4F);
			break;
		case BSVM2_OPUV_LEN:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_UNOPX_LEN_X4F);
			break;
		case BSVM2_OPUV_LEN2:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_UNOPX_LEN2_X4F);
			break;
		}
		break;

	case BSVM2_OPVZ_V3XF:
		switch(o)
		{
		case BSVM2_OPUV_NEG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_NEG_X3XF);
			break;
		case BSVM2_OPUV_RCP:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_RCP_X3XF);
			break;
		case BSVM2_OPUV_SQRT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_SQRT_X3XF);
			break;
		case BSVM2_OPUV_RSQRT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_ADDR, BSVM2_Op_UNOPX_RSQRT_X3XF);
			break;
		case BSVM2_OPUV_LEN:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_UNOPX_LEN_X3XF);
			break;
		case BSVM2_OPUV_LEN2:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_DOUBLE, BSVM2_Op_UNOPX_LEN2_X3XF);
			break;
		}
		break;

	case BSVM2_OPVZ_I128:
	case BSVM2_OPVZ_UI128:
		switch(o)
		{
		case BSVM2_OPRU_NEG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_BINOPX_NEG_XI128);
			break;
		case BSVM2_OPRU_NOT:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_BINOPX_NOT_XI128);
			break;
		}
		break;

	case BSVM2_OPVZ_F128:
		switch(o)
		{
		case BSVM2_OPRU_NEG:
			BSVM2_Interp_SetupOpUn(cblk, op,
				BSVM2_OPZ_FLOAT, BSVM2_Op_BINOPX_NEG_XF128);
			break;
		}
		break;
	}
	return(op);
}


BSVM2_Opcode *BSVM2_OpVec_DecodeVectorOpcode(
	BSVM2_CodeBlock *cblk, BSVM2_Opcode *op, int opn)
{
	int opn2;

	switch(opn)
	{
	case BSVM2_OP_MKX4I:
		BSVM2_Interp_SetupOpQuad(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_MKX4I);
		break;
	case BSVM2_OP_MKX2L:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_MKX2L);
		break;
	case BSVM2_OP_MKX4F:
		BSVM2_Interp_SetupOpQuad(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_MKX4F);
		break;
	case BSVM2_OP_MKX2D:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_MKX2D);
		break;
	case BSVM2_OP_MKX3D:
		BSVM2_Interp_SetupOpTrin(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_MKX3D);
		break;

	case BSVM2_OP_DUPX:
		BSVM2_Interp_SetupOpUnP(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_DUPX);
		break;
	case BSVM2_OP_POPX:
		BSVM2_Interp_SetupOpPopUn(cblk, op, BSVM2_Op_POPX);
		break;

	case BSVM2_OP_LDXL:
		BSVM2_Interp_SetupOpUnL(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_LDXL);
		break;
	case BSVM2_OP_STXL:
		BSVM2_Interp_SetupOpUstL(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_STXL);
		break;

	case BSVM2_OP_IFXX:
		BSVM2_Interp_SetupOpUatL(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_IFXX);
		break;
	case BSVM2_OP_DFXX:
		BSVM2_Interp_SetupOpUatL(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_DFXX);
		break;
	case BSVM2_OP_IFXXA:
		BSVM2_Interp_SetupOpUatL(cblk, op, BSVM2_OPZ_ADDR, BSVM2_Op_IFXXA);
		break;

	case BSVM2_OP_BINOPX:
		opn2=BSVM2_Interp_DecodeOpUCxI(cblk);
		op=BSVM2_OpVec_DecodeBinOpX(cblk, op, opn2);
		break;
	case BSVM2_OP_UNOPX:
		opn2=BSVM2_Interp_DecodeOpUCxI(cblk);
		op=BSVM2_OpVec_DecodeUnOpX(cblk, op, opn2);
		break;

	case BSVM2_OP_MKV2I:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_MKV2I);
		break;
	case BSVM2_OP_MKV2F:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_MKV2F);
		break;

	case BSVM2_OP_CMPXL:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPXL);
		break;
	case BSVM2_OP_UCMPXL:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_UCMPXL);
		break;
	case BSVM2_OP_CMPXF:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMPXF);
		break;
	case BSVM2_OP_CMP2XF:
		BSVM2_Interp_SetupOpBin(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_CMP2XF);
		break;

	case BSVM2_OP_CVTI2XL:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTI2XL);
		break;
	case BSVM2_OP_CVTL2XL:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTL2XL);
		break;
	case BSVM2_OP_CVTUI2XL:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTUI2XL);
		break;
	case BSVM2_OP_CVTUL2XL:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTUL2XL);
		break;

	case BSVM2_OP_CVTXL2I:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTXL2I);
		break;
	case BSVM2_OP_CVTXL2L:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTXL2L);
		break;
	case BSVM2_OP_CVTD2XF:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTD2XF);
		break;
	case BSVM2_OP_CVTXF2D:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTXF2D);
		break;
	case BSVM2_OP_CVTXL2XF:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTXL2XF);
		break;
	case BSVM2_OP_CVTXF2XL:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTXF2XL);
		break;
	case BSVM2_OP_CVTXL2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTXL2AA);
		break;
	case BSVM2_OP_CVTXF2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTXF2AA);
		break;
	case BSVM2_OP_CVTAA2XL:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTAA2XL);
		break;
	case BSVM2_OP_CVTAA2XF:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTAA2XF);
		break;

	case BSVM2_OP_CVTVI2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTVI2AA);
		break;
	case BSVM2_OP_CVTVF2AA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTVF2AA);
		break;
	case BSVM2_OP_CVTAA2VI:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTAA2VI);
		break;
	case BSVM2_OP_CVTAA2VF:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTAA2VF);
		break;
		
	case BSVM2_OP_LDV2IA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDV2IA_V2I);
		break;
	case BSVM2_OP_LDV2IB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDV2IB_V2I);
		break;
	case BSVM2_OP_LDV2FA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDV2FA_V2F);
		break;
	case BSVM2_OP_LDV2FB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDV2FB_V2F);
		break;

	case BSVM2_OP_LDX2LA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_LONG, BSVM2_Op_LDX2LA_X2L);
		break;
	case BSVM2_OP_LDX2LB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_LONG, BSVM2_Op_LDX2LB_X2L);
		break;
	case BSVM2_OP_LDX2DA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_LDX2DA_X2D);
		break;
	case BSVM2_OP_LDX2DB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_LDX2DB_X2D);
		break;

	case BSVM2_OP_LDX4IA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDX4IA_X4I);
		break;
	case BSVM2_OP_LDX4IB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDX4IB_X4I);
		break;
	case BSVM2_OP_LDX4IC:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDX4IC_X4I);
		break;
	case BSVM2_OP_LDX4ID:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_INT, BSVM2_Op_LDX4ID_X4I);
		break;
	case BSVM2_OP_LDX4FA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDX4FA_X4F);
		break;
	case BSVM2_OP_LDX4FB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDX4FB_X4F);
		break;
	case BSVM2_OP_LDX4FC:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDX4FC_X4F);
		break;
	case BSVM2_OP_LDX4FD:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_FLOAT, BSVM2_Op_LDX4FD_X4F);
		break;

	case BSVM2_OP_LDX3DA:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_LDX3DA_X3D);
		break;
	case BSVM2_OP_LDX3DB:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_LDX3DB_X3D);
		break;
	case BSVM2_OP_LDX3DC:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_DOUBLE, BSVM2_Op_LDX3DC_X3D);
		break;

	case BSVM2_OP_CVTX3F2X3D:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTX3F2X3D);
		break;
	case BSVM2_OP_CVTX3D2X3F:
		BSVM2_Interp_SetupOpUn(cblk, op,
			BSVM2_OPZ_ADDR, BSVM2_Op_CVTX3D2X3F);
		break;

	default:
		BSVM2_Interp_FreeOpcode(cblk, op);
		op=NULL;
		break;
	}

	return(op);
}
