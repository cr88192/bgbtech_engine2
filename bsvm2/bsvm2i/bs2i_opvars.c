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


void BSVM2_Op_LDI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->local[op->i0].i; }
void BSVM2_Op_LDL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->local[op->i0].l; }
void BSVM2_Op_LDF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->local[op->i0].f; }
void BSVM2_Op_LDD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->local[op->i0].d; }
void BSVM2_Op_LDA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=frm->local[op->i0].a; }

void BSVM2_Op_STI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->stack[op->t0].i; }
void BSVM2_Op_STL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].l=frm->stack[op->t0].l; }
void BSVM2_Op_STF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].f=frm->stack[op->t0].f; }
void BSVM2_Op_STD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].d=frm->stack[op->t0].d; }
void BSVM2_Op_STA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].a=frm->stack[op->t0].a; }

BS2VM_API void BSVM2_Op_LDCI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=op->v.i; }
BS2VM_API void BSVM2_Op_LDCL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=op->v.l; }
BS2VM_API void BSVM2_Op_LDCF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=op->v.f; }
BS2VM_API void BSVM2_Op_LDCD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=op->v.d; }
BS2VM_API void BSVM2_Op_LDCA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=op->v.a; }

#if 1
BS2VM_API void BSVM2_Op_LDLXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].i=frm->lxvar[op->i0].i; }
BS2VM_API void BSVM2_Op_LDLXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].l=frm->lxvar[op->i0].l; }
BS2VM_API void BSVM2_Op_LDLXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].f=frm->lxvar[op->i0].f; }
BS2VM_API void BSVM2_Op_LDLXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].d=frm->lxvar[op->i0].d; }
BS2VM_API void BSVM2_Op_LDLXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=frm->lxvar[op->i0].a; }

BS2VM_API void BSVM2_Op_STLXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->lxvar[op->i0].i=frm->stack[op->t0].i; }
BS2VM_API void BSVM2_Op_STLXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->lxvar[op->i0].l=frm->stack[op->t0].l; }
BS2VM_API void BSVM2_Op_STLXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->lxvar[op->i0].f=frm->stack[op->t0].f; }
BS2VM_API void BSVM2_Op_STLXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->lxvar[op->i0].d=frm->stack[op->t0].d; }
BS2VM_API void BSVM2_Op_STLXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->lxvar[op->i0].a=frm->stack[op->t0].a; }
#endif

#if 1
BS2VM_API void BSVM2_Op_LDDRLXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	frm->stack[op->t0].i=*(s32 *)p; }
BS2VM_API void BSVM2_Op_LDDRLXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	frm->stack[op->t0].l=*(s64 *)p; }
BS2VM_API void BSVM2_Op_LDDRLXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	frm->stack[op->t0].f=*(f32 *)p; }
BS2VM_API void BSVM2_Op_LDDRLXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	frm->stack[op->t0].d=*(f64 *)p; }
BS2VM_API void BSVM2_Op_LDDRLXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	frm->stack[op->t0].a=*(dtVal *)p; }

BS2VM_API void BSVM2_Op_STDRLXI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	*(s32 *)p=frm->stack[op->t0].i; }
BS2VM_API void BSVM2_Op_STDRLXL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	*(s64 *)p=frm->stack[op->t0].l; }
BS2VM_API void BSVM2_Op_STDRLXF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	*(f32 *)p=frm->stack[op->t0].f; }
BS2VM_API void BSVM2_Op_STDRLXD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	*(f64 *)p=frm->stack[op->t0].d; }
BS2VM_API void BSVM2_Op_STDRLXA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	void *p; p=dtvUnwrapPtrF(frm->lxvar[op->i0].a);
	*(dtVal *)p=frm->stack[op->t0].a; }
#endif

BS2VM_API void BSVM2_Op_MVI(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=frm->local[op->i1].i; }
BS2VM_API void BSVM2_Op_MVL(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].l=frm->local[op->i1].l; }
BS2VM_API void BSVM2_Op_MVF(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].f=frm->local[op->i1].f; }
BS2VM_API void BSVM2_Op_MVD(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].d=frm->local[op->i1].d; }
BS2VM_API void BSVM2_Op_MVA(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].a=frm->local[op->i1].a; }

BS2VM_API void BSVM2_Op_MVIC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].i=op->v.i; }
BS2VM_API void BSVM2_Op_MVLC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].l=op->v.l; }
BS2VM_API void BSVM2_Op_MVFC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].f=op->v.f; }
BS2VM_API void BSVM2_Op_MVDC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].d=op->v.d; }
BS2VM_API void BSVM2_Op_MVAC(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->local[op->i0].a=op->v.a; }


BS2VM_API void BSVM2_Op_LDGSI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].i=vi->gvalue->i;		}
BS2VM_API void BSVM2_Op_LDGSL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].l=vi->gvalue->l;		}
BS2VM_API void BSVM2_Op_LDGSF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].f=vi->gvalue->f;		}
BS2VM_API void BSVM2_Op_LDGSD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].d=vi->gvalue->d;		}
BS2VM_API void BSVM2_Op_LDGSA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	frm->stack[op->t0].a=vi->gvalue->a;		}

BS2VM_API void BSVM2_Op_STGSI(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->i=frm->stack[op->t0].i;		}
BS2VM_API void BSVM2_Op_STGSL(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->l=frm->stack[op->t0].l;		}
BS2VM_API void BSVM2_Op_STGSF(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->f=frm->stack[op->t0].f;		}
BS2VM_API void BSVM2_Op_STGSD(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->d=frm->stack[op->t0].d;		}
BS2VM_API void BSVM2_Op_STGSA(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_ImageGlobal *vi;	vi=op->v.p;
	vi->gvalue->a=frm->stack[op->t0].a;		}

BS2VM_API void BSVM2_Op_LDGSX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	BSVM2_ValX128 *a;
	void *p;

	vi=op->v.p;
	a=BSVM2_FrameAllocX128(frm);
	p=vi->gvalue->p;
	*a=*(BSVM2_ValX128 *)p;
	frm->stack[op->t0].p=a;
}

BS2VM_API void BSVM2_Op_STGSX(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	BSVM2_ValX128 *a;
	void *p;

	if(dtvNullP(frm->stack[op->t0].a))
		{ frm->ctx->status=BSVM2_EXS_NULLEX; return; }

	a=frm->stack[op->t0].p;
	vi=op->v.p;
	p=vi->gvalue->p;
	if(!p)
	{
		p=BSVM2_FrameAllocX128(frm);
		vi->gvalue->p=p;
	}

	*(BSVM2_ValX128 *)p=*a;
	BSVM2_FrameFreeX128(frm, a);

//	vi->gvalue->a=frm->stack[op->t0].a;
}

#if 1
BS2VM_API void BSVM2_Op_LDGS_DY(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	
	if(!frm->ctx->dynenv)
	{
		frm->stack[op->t0].l=0;
		return;
	}
	
	vi=op->v.p;
	if((vi->nargs<=0) || (vi->nargs>=frm->ctx->szdynenv))
	{
		frm->stack[op->t0].l=0;
		return;
	}
	
	switch(op->i1)
	{
	case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
		frm->stack[op->t0].i=frm->ctx->dynenv[vi->nargs].i;
		break;
	case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
		frm->stack[op->t0].l=frm->ctx->dynenv[vi->nargs].l;
		break;
	case BSVM2_OPZ_FLOAT:
		frm->stack[op->t0].f=frm->ctx->dynenv[vi->nargs].f;
		break;
	case BSVM2_OPZ_DOUBLE:
		frm->stack[op->t0].d=frm->ctx->dynenv[vi->nargs].d;
		break;
	case BSVM2_OPZ_ADDRESS:
		frm->stack[op->t0].a=frm->ctx->dynenv[vi->nargs].a;
		break;
	}
}

BS2VM_API void BSVM2_Op_STGS_DY(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_ImageGlobal *vi;
	
	if(!frm->ctx->dynenv)
	{
		frm->ctx->dynenv=dtmAlloc("bs2i_dynenv_t",
			bs2i_img_dynvlim*sizeof(BSVM2_Value));
		frm->ctx->szdynenv=bs2i_img_dynvlim;
	}

	vi=op->v.p;
	if((vi->nargs<=0) || (vi->nargs>=bs2i_img_dynvlim))
		return;
	if((vi->nargs>=frm->ctx->szdynenv))
	{
		frm->ctx->dynenv=dtmRealloc(frm->ctx->dynenv,
			bs2i_img_dynvlim*sizeof(BSVM2_Value));
		frm->ctx->szdynenv=bs2i_img_dynvlim;
	}
	
	switch(op->i1)
	{
	case BSVM2_OPZ_INT:		case BSVM2_OPZ_UINT:
	case BSVM2_OPZ_SBYTE:	case BSVM2_OPZ_UBYTE:
	case BSVM2_OPZ_SHORT:	case BSVM2_OPZ_USHORT:
		frm->ctx->dynenv[vi->nargs].i=frm->stack[op->t0].i;
		break;
	case BSVM2_OPZ_LONG:	case BSVM2_OPZ_ULONG:
		frm->ctx->dynenv[vi->nargs].l=frm->stack[op->t0].l;
		break;
	case BSVM2_OPZ_FLOAT:
		frm->ctx->dynenv[vi->nargs].f=frm->stack[op->t0].f;
		break;
	case BSVM2_OPZ_DOUBLE:
		frm->ctx->dynenv[vi->nargs].d=frm->stack[op->t0].d;
		break;
	case BSVM2_OPZ_ADDRESS:
		frm->ctx->dynenv[vi->nargs].a=frm->stack[op->t0].a;
		break;
	}
}
#endif

BS2VM_API void BSVM2_Op_LDCTH(BSVM2_Frame *frm, BSVM2_Opcode *op)
	{ frm->stack[op->t0].a=frm->self; }

BS2VM_API void BSVM2_Op_MKLFCN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_Lambda *lfcn;
	
//	lfcn=dtmAlloc("bsvm2_lambda_t",
//		sizeof(BSVM2_Lambda));
	lfcn=BSVM2_Interp_AllocLambda(frm->ctx);
//	lfcn->func=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->func=op->v.p;
	lfcn->lxvar=lfcn->t_lxvar;
	frm->stack[op->t0].a=dtvWrapPtr(lfcn);
}

BS2VM_API void BSVM2_Op_IFXLFCN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_Lambda *lfcn;
	BSVM2_ImageGlobal *vi;
	void *p;

	vi=op->v.p;
	lfcn=BSVM2_Interp_AllocLambda(frm->ctx);
	lfcn->func=op->v.p;
	lfcn->lxvar=lfcn->t_lxvar;
	frm->local[op->i0].a=dtvWrapPtr(lfcn);
}

BS2VM_API void BSVM2_Op_DFXLFCN(BSVM2_Frame *frm, BSVM2_Opcode *op)
{
	BSVM2_Lambda *lfcn;
	dtVal a;
	void *p;

	a=frm->local[op->i0].a;
	p=dtvUnwrapPtr(a);
	if(p)
		dtmFree(p);
}

BS2VM_API void BSVM2_Op_DSTIXUZ_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].i=frm->stack[op->t1].i;		}
BS2VM_API void BSVM2_Op_DSTIXUZ_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].l=frm->stack[op->t1].l;		}
BS2VM_API void BSVM2_Op_DSTIXUZ_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].f=frm->stack[op->t1].f;		}
BS2VM_API void BSVM2_Op_DSTIXUZ_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].d=frm->stack[op->t1].d;		}
BS2VM_API void BSVM2_Op_DSTIXUZ_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].a=frm->stack[op->t1].a;		}

BS2VM_API void BSVM2_Op_DSTIXUZL_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].i=frm->local[op->i1].i;		}
BS2VM_API void BSVM2_Op_DSTIXUZL_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].l=frm->local[op->i1].l;		}
BS2VM_API void BSVM2_Op_DSTIXUZL_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].f=frm->local[op->i1].f;		}
BS2VM_API void BSVM2_Op_DSTIXUZL_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].d=frm->local[op->i1].d;		}
BS2VM_API void BSVM2_Op_DSTIXUZL_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].a=frm->local[op->i1].a;		}

BS2VM_API void BSVM2_Op_DSTIXUZRL_I(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].a=dtvWrapTyTagPtrF(frm->local+op->i1, 1);		}
BS2VM_API void BSVM2_Op_DSTIXUZRL_L(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].a=dtvWrapTyTagPtrF(frm->local+op->i1, 2);		}
BS2VM_API void BSVM2_Op_DSTIXUZRL_F(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].a=dtvWrapTyTagPtrF(frm->local+op->i1, 3);		}
BS2VM_API void BSVM2_Op_DSTIXUZRL_D(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].a=dtvWrapTyTagPtrF(frm->local+op->i1, 4);		}
BS2VM_API void BSVM2_Op_DSTIXUZRL_A(BSVM2_Frame *frm, BSVM2_Opcode *op)
{	BSVM2_Lambda *lfcn;		lfcn=dtvUnwrapPtr(frm->stack[op->t0].a);
	lfcn->lxvar[op->i0].a=dtvWrapTyTagPtrF(frm->local+op->i1, 5);		}
