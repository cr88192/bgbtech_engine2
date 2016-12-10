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

/** \file
 * BS2 Interpreter
 *
 */


BSVM2_Context *bsvm2_interp_freectx;

BSVM2_Context *bsvm2_interp_freepoolctx;
byte bsvm2_interp_nojit;

BS2VM_API int BSVM2_Interp_SetEnable(int parm)
{
	switch(parm)
	{
	case 0x1000:
		bsvm2_interp_nojit=0; break;
	default:
		break;
	}
	return(0);
}

BS2VM_API int BSVM2_Interp_SetDisable(int parm)
{
	switch(parm)
	{
	case 0x1000:
		bsvm2_interp_nojit=1; break;
	default:
		break;
	}
	return(0);
}

BS2VM_API int bsvm2_natfib(int x)
{
		if(x<2)return(1);
		return(bsvm2_natfib(x-1)+bsvm2_natfib(x-2));
}

BS2VM_API void bsvm2_natsorttst(int n)
{
	int *arr;
	int i, j, k;
	
	arr=bgbdt_mm_malloc(n*sizeof(int));
	for(i=0; i<n; i++)
		arr[i]=rand();

	for(i=0; i<n; i++)
		for(j=i+1; j<n; j++)
	{
		if(arr[j]<arr[i])
		{
			k=arr[i];
			arr[i]=arr[j];
			arr[j]=k;
		}
	}
	
	bgbdt_mm_free(arr);
}

BS2VM_API dtVal bsvm2_vm_printf(dtVal str, dtVal va)
{
	char tb[1024];
	char *str1, *s0;
	char *s, *t;
	dtVal vn;
	double f;
	s64 li;
	int fl, w, pr;
	int i, j, k, n;

	str1=BGBDT_TagStr_GetUtf8(str);
	
	s=str1; t=tb; n=0;
	while(*s)
	{
		if(*s!='%')
		{
			*t++=*s++;
			continue;
		}
		
		s++;
		if(*s=='%')
		{
			*t++=*s++;
			continue;
		}
		
		if(dtvIsArrayP(va))
			{ vn=dtvArrayGetIndexDtVal(va, n++); }
		else
			{ vn=va; }
		
		fl=0; w=0; pr=0;
		
		if(*s=='-') { fl|=1; s++; }
		if(*s=='+') { fl|=2; s++; }
		if(*s=='0') { fl|=4; s++; }
		if(*s==' ') { fl|=8; s++; }
		if(*s=='#') { fl|=16; s++; }
		if(*s=='\'') { fl|=32; s++; }
		
		if((*s>='1') && (*s<='9'))
		{
			i=0;
			while((*s>='0') && (*s<='9'))
				{ i=(i*10)+((*s++)-'0'); }
			w=i;
		}
		
		if(*s=='.')
		{
			s++;
			i=0;
			while((*s>='0') && (*s<='9'))
				{ i=(i*10)+((*s++)-'0'); }
			pr=i;
		}
		
		if(*s=='h')
		{
			s++;
			if(*s=='h')s++;
		}
		if(*s=='l')
		{
			s++;
			if(*s=='l')s++;
		}
		if(*s=='L')s++;
		if(*s=='q')s++;
		
		if(*s=='f')
		{
			f=dtvUnwrapDouble(vn);
			sprintf(t, "%f", f);
			s++; t+=strlen(t);
			continue;
		}

		if((*s=='d') || (*s=='i'))
		{
			li=dtvUnwrapLong(vn);
			if(w)	sprintf(t, "%*lld", w, li);
			else	sprintf(t, "%lld", li);
			s++; t+=strlen(t);
			continue;
		}

		if((*s=='x') || (*s=='X'))
		{
			li=dtvUnwrapLong(vn);
			if(w)	sprintf(t, "%0*llX", w, li);
			else	sprintf(t, "%llX", li);
			s++; t+=strlen(t);
			continue;
		}
		
		if(*s=='c')
		{
			i=dtvUnwrapInt(vn);
			*t++=i;
			s++;
			continue;
		}

		if(*s=='s')
		{
			s0=BGBDT_TagStr_GetUtf8(vn);
			if(w)
			{
				if(fl&1)
					sprintf(t, "%-*s", w, s0);
				else
					sprintf(t, "%*s", w, s0);
			}
			else
				{ sprintf(t, "%s", s0); }
			s++; t+=strlen(t);
			continue;
		}

		s++;
		*t++='?';
		continue;
	}
	*t++=0;
	
	fputs(tb, stdout);
	
	return(DTV_NULL);
}

/** Allocate VM Context. */
BS2VM_API BSVM2_Context *BSVM2_Interp_AllocContext(void)
{
	BSVM2_Context *tmp;
	
	tmp=bsvm2_interp_freectx;
	if(tmp)
	{
		bsvm2_interp_freectx=tmp->next;
		memset(tmp, 0, sizeof(BSVM2_Context));
		return(tmp);
	}
	
	tmp=dtmAlloc("bsvm2_context_t", sizeof(BSVM2_Context));
	return(tmp);
}

/** Allocate VM context from a pool. */
BS2VM_API BSVM2_Context *BSVM2_Interp_AllocPoolContext(void)
{
	BSVM2_Context *tmp;
	
	tmp=bsvm2_interp_freepoolctx;
	if(tmp)
	{
		bsvm2_interp_freepoolctx=tmp->next;
		return(tmp);
	}
	
	tmp=BSVM2_Interp_AllocContext();
	return(tmp);
}

/** Free VM Context to the pool. */
BS2VM_API void BSVM2_Interp_FreePoolContext(BSVM2_Context *ctx)
{
//	if(ctx->tstackref!=0)
//		*(int *)-1=-1;

	ctx->tstackref=0;
	ctx->next=bsvm2_interp_freepoolctx;
	bsvm2_interp_freepoolctx=ctx;
}

/** Allocate a call frame */
BS2VM_API BSVM2_Frame *BSVM2_Interp_AllocFrame(BSVM2_Context *ctx)
{
	BSVM2_Frame *tmp;
	
	tmp=ctx->freeframe;
	if(tmp)
	{
		ctx->freeframe=tmp->rnext;
//		memset(tmp, 0, sizeof(BSVM2_Frame));
		tmp->ctx=ctx;
		return(tmp);
	}
	
	tmp=dtmAlloc("bsvm2_frame_t", sizeof(BSVM2_Frame));
	tmp->ctx=ctx;
	return(tmp);
}

BS2VM_API bool BSVM2_Interp_IsLambdaP(dtVal objv)
{
	static int objty_lambda=-1;

	if(objty_lambda<0)
		{ objty_lambda=BGBDT_MM_GetIndexObjTypeName("bsvm2_lambda_t"); }
	return(dtvCheckPtrTagP(objv, objty_lambda));
}

BS2VM_API BSVM2_Lambda *BSVM2_Interp_AllocLambda(BSVM2_Context *ctx)
{
	BSVM2_Lambda *lfcn;
	
	lfcn=dtmAlloc("bsvm2_lambda_t",
		sizeof(BSVM2_Lambda));
	return(lfcn);
}

/** Setup VM context for calling a function. */
BS2VM_API BSVM2_Trace *BSVM2_Interp_SetupCallVM(
	BSVM2_Context *ctx, BSVM2_ImageGlobal *vi,
	BSVM2_Value *args)
{
	BSVM2_Frame *frm;
	BSVM2_Frame *frmb;
	BSVM2_Trace *tr;
	int i;
	
	if(!ctx->tstack)
	{
//		ctx->cstack=dtmMalloc((1<<8)*sizeof(BSVM2_Value));
		ctx->tstack=dtmMalloc((1<<16)*sizeof(BSVM2_Value));
		ctx->tstackref=0;
	}
	
	frm=BSVM2_Interp_AllocFrame(ctx);
	frmb=BSVM2_Interp_AllocFrame(ctx);

	tr=BSVM2_Interp_AllocTrace(NULL);
	tr->Run=BSVM2_TrRun_NULL;

	frm->stack=ctx->tstack+(ctx->tstackref++);

//	frmb->stack=ctx->tstack+ctx->tstackref;
//	frmb->local=frmb->stack+vi->cblk->stkdepth;

	frmb->local=ctx->tstack+ctx->tstackref;
	frmb->stack=frmb->local+vi->cblk->largs;

	ctx->tstackref=ctx->tstackref+vi->cblk->szframe;
	frmb->tstkpos=ctx->tstackref;

	frmb->rnext=frm;
	frm->rtrace=tr;
	frm->rcsrv=0;
	ctx->frame=frmb;

	if(args)
	{
		for(i=0; i<vi->nargs; i++)
			{ frmb->local[vi->cblk->bargs+i]=args[i]; }
	}

	tr=BS2I_ImageGetFuncTrace(vi);
	ctx->trace=tr;
	return(tr);
}

/** Setup VM context for calling a method. */
BS2VM_API BSVM2_Trace *BSVM2_Interp_SetupMethodCallVM(
	BSVM2_Context *ctx, BSVM2_ImageGlobal *vi,
	dtVal self, BSVM2_Value *args)
{
	BSVM2_Frame *frm;
	BSVM2_Frame *frmb;
	BSVM2_Trace *tr;
	int i;
	
	if(!ctx->tstack)
	{
//		ctx->cstack=dtmMalloc((1<<8)*sizeof(BSVM2_Value));
		ctx->tstack=dtmMalloc((1<<16)*sizeof(BSVM2_Value));
		ctx->tstackref=0;
	}
	
	frm=BSVM2_Interp_AllocFrame(ctx);
	frmb=BSVM2_Interp_AllocFrame(ctx);

	tr=BSVM2_Interp_AllocTrace(NULL);
	tr->Run=BSVM2_TrRun_NULL;

	frm->stack=ctx->tstack+(ctx->tstackref++);

//	frmb->stack=ctx->tstack+ctx->tstackref;
//	frmb->local=frmb->stack+vi->cblk->stkdepth;

	frmb->local=ctx->tstack+ctx->tstackref;
	frmb->stack=frmb->local+vi->cblk->largs;

	ctx->tstackref=ctx->tstackref+vi->cblk->szframe;
	frmb->tstkpos=ctx->tstackref;
	frmb->self=self;

	frmb->rnext=frm;
	frm->rtrace=tr;
	frm->rcsrv=0;
	ctx->frame=frmb;

	if(args)
	{
		for(i=0; i<vi->nargs; i++)
			{ frmb->local[vi->cblk->bargs+i]=args[i]; }
	}

	tr=BS2I_ImageGetFuncTrace(vi);
	ctx->trace=tr;
	return(tr);
}

/** Setup VM context for calling a method, caching the method handle. */
BS2VM_API int BSVM2_Interp_SetupCacheMethodCallVM(
	BSVM2_Context *ctx, dtVal self,
	dtcMethod *rfi, BSVM2_Value *args,
	char *qn, char *fn, char *sig)
{
	dtcClass cls;
	dtcMethod fi;
	dtVal sf;
	void *sfp;

	fi=*rfi;
	if(!fi)
	{
		cls=BGBDTC_LookupClassQName(qn);
		if(sig)
			{ fi=BGBDTC_LookupClassSlotNameSig(cls, fn, sig); }
		else
			{ fi=BGBDTC_LookupClassSlotName(cls, fn); }
		*rfi=fi;
		
		if(!fi)
			return(-1);
	}

	sf=dtcVaGetA(self, fi);
	sfp=dtvUnwrapPtr(sf);
	if(!sfp)
		{ return(-1); }
	BSVM2_Interp_SetupMethodCallVM(ctx, sfp, self, args);
	return(0);
}

/** Call a method while caching method handle for later use. */
BS2VM_API int BSVM2_Interp_CallCacheMethodVM(dtVal self,
	dtcMethod *rfi, BSVM2_Value *args,
	char *qn, char *fn, char *sig, int lim)
{
	BSVM2_Context *vctx;
	int i;

	vctx=BSVM2_Interp_AllocPoolContext();
	i=BSVM2_Interp_SetupCacheMethodCallVM(vctx,
		self, rfi, args, qn, fn, sig);

	if(i<0)
	{
		BSVM2_Interp_FreePoolContext(vctx);
		return(i);
	}

	i=BSVM2_Interp_RunContext(vctx, lim);
	BSVM2_Interp_FreePoolContext(vctx);
	return(i);
}

/** Get the value field for a global. */
BS2VM_API BSVM2_Value *BSVM2_Interp_GetGlobalValue(BSVM2_ImageGlobal *vi)
{
	return(vi->gvalue);
}

/** Get the value of an int global. */
BS2VM_API s32 BSVM2_Interp_GetGlobalI(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(0);
	return(gv->i);
}

/** Get the value of an long global. */
BS2VM_API s64 BSVM2_Interp_GetGlobalL(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(0);
	return(gv->l);
}

/** Get the value of an float global. */
BS2VM_API f32 BSVM2_Interp_GetGlobalF(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(0);
	return(gv->f);
}

/** Get the value of an double global. */
BS2VM_API f64 BSVM2_Interp_GetGlobalD(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(0);
	return(gv->d);
}

/** Get the value of an address global. */
BS2VM_API dtVal BSVM2_Interp_GetGlobalA(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(DTV_UNDEFINED);
	return(gv->a);
}

/** Set the value of an int global. */
BS2VM_API int BSVM2_Interp_SetGlobalI(BSVM2_ImageGlobal *vi, s32 v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->i=v;
	return(0);
}

/** Set the value of an long global. */
BS2VM_API int BSVM2_Interp_SetGlobalL(BSVM2_ImageGlobal *vi, s64 v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->l=v;
	return(0);
}

/** Set the value of an float global. */
BS2VM_API int BSVM2_Interp_SetGlobalF(BSVM2_ImageGlobal *vi, f32 v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->f=v;
	return(0);
}

/** Set the value of an double global. */
BS2VM_API int BSVM2_Interp_SetGlobalD(BSVM2_ImageGlobal *vi, f64 v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->d=v;
	return(0);
}

/** Set the value of an address global. */
BS2VM_API int BSVM2_Interp_SetGlobalA(BSVM2_ImageGlobal *vi, dtVal v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->a=v;
	return(0);
}

/** Get the value of a global as a dynamically-typed value. */
BS2VM_API dtVal BSVM2_Interp_GetGlobalDy(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	dtVal v;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(DTV_UNDEFINED);
	v=BSVM2_Sig_GetSigPtrDtVal(gv, vi->sig);
	return(v);
}

/** Set the value of a global as a dynamically typed value. */
BS2VM_API dtVal BSVM2_Interp_SetGlobalDy(BSVM2_ImageGlobal *vi, dtVal v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(DTV_UNDEFINED);
	BSVM2_Sig_SetSigPtrDtVal(gv, vi->sig, v);
	return(v);
}

/** Run the context for a given number of iterations. */
BS2VM_API int BSVM2_Interp_RunContext(BSVM2_Context *ctx, int lim)
{
	BSVM2_Trace *tr, *ltr, *ltr2;
	int l;
	
	if(lim<0)
	{
		tr=ctx->trace;
//		while(tr && !(ctx->status))
		while(tr)
		{
//			ltr2=ltr; ltr=tr;
//			tr->runcnt++;
			tr=tr->Run(ctx->frame, tr);
		}
		if(tr)
			{ ctx->trace=tr; }
		return(ctx->status);
	}
	
	tr=ctx->trace; l=lim;
//	while(tr && !(ctx->status) && ((l--)>0))
	while(tr && ((l--)>0))
		{ tr=tr->Run(ctx->frame, tr); }
	if(tr)
		{ ctx->trace=tr; }
	if((l<=0) && !ctx->status)
		return(BSVM2_EXS_RUNLIMIT);
	return(ctx->status);
}

int BSVM2_Interp_RunDoWork(BGBDT_WorkItem *item)
{
	int i;

	i=BSVM2_Interp_RunContext(item->data, 16384);
	if(i==BSVM2_EXS_RUNLIMIT)
		return(1);
	
	return(0);
}

BS2VM_API BGBDT_WorkItem *BSVM2_Interp_RunContextWork(BSVM2_Context *ctx)
{
	BGBDT_WorkItem *tmp;
	
	tmp=BGBDT_Work_AllocItem();
	tmp->data=ctx;
	tmp->DoWork=BSVM2_Interp_RunDoWork;
	BGBDT_Work_SubmitItem(tmp);
	return(tmp);
}
