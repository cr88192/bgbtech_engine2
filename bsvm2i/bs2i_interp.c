#include <bteifgl.h>

BSVM2_Context *bsvm2_interp_freectx;

BSVM2_Context *bsvm2_interp_freepoolctx;

BTEIFGL_API int bsvm2_natfib(int x)
{
		if(x<2)return(1);
		return(bsvm2_natfib(x-1)+bsvm2_natfib(x-2));
}

BTEIFGL_API void bsvm2_natsorttst(int n)
{
	int *arr;
	int i, j, k;
	
	arr=frgl_malloc(n*sizeof(int));
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
	
	frgl_free(arr);
}

BTEIFGL_API BSVM2_Context *BSVM2_Interp_AllocContext(void)
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

BTEIFGL_API BSVM2_Context *BSVM2_Interp_AllocPoolContext(void)
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

BTEIFGL_API void BSVM2_Interp_FreePoolContext(BSVM2_Context *ctx)
{
//	if(ctx->tstackref!=0)
//		*(int *)-1=-1;

	ctx->tstackref=0;
	ctx->next=bsvm2_interp_freepoolctx;
	bsvm2_interp_freepoolctx=ctx;
}

BTEIFGL_API BSVM2_Frame *BSVM2_Interp_AllocFrame(BSVM2_Context *ctx)
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

BTEIFGL_API BSVM2_Trace *BSVM2_Interp_SetupCallVM(
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

	frmb->stack=ctx->tstack+ctx->tstackref;
	frmb->local=frmb->stack+vi->cblk->stkdepth;
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

BTEIFGL_API BSVM2_Trace *BSVM2_Interp_SetupMethodCallVM(
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

	frmb->stack=ctx->tstack+ctx->tstackref;
	frmb->local=frmb->stack+vi->cblk->stkdepth;
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

BTEIFGL_API int BSVM2_Interp_SetupCacheMethodCallVM(
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

BTEIFGL_API int BSVM2_Interp_CallCacheMethodVM(dtVal self,
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

BTEIFGL_API BSVM2_Value *BSVM2_Interp_GetGlobalValue(BSVM2_ImageGlobal *vi)
{
	return(vi->gvalue);
}

BTEIFGL_API s32 BSVM2_Interp_GetGlobalI(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(0);
	return(gv->i);
}

BTEIFGL_API s64 BSVM2_Interp_GetGlobalL(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(0);
	return(gv->l);
}

BTEIFGL_API f32 BSVM2_Interp_GetGlobalF(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(0);
	return(gv->f);
}

BTEIFGL_API f64 BSVM2_Interp_GetGlobalD(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(0);
	return(gv->d);
}

BTEIFGL_API dtVal BSVM2_Interp_GetGlobalA(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(DTV_UNDEFINED);
	return(gv->a);
}

BTEIFGL_API int BSVM2_Interp_SetGlobalI(BSVM2_ImageGlobal *vi, s32 v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->i=v;
	return(0);
}

BTEIFGL_API int BSVM2_Interp_SetGlobalL(BSVM2_ImageGlobal *vi, s64 v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->l=v;
	return(0);
}

BTEIFGL_API int BSVM2_Interp_SetGlobalF(BSVM2_ImageGlobal *vi, f32 v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->f=v;
	return(0);
}

BTEIFGL_API int BSVM2_Interp_SetGlobalD(BSVM2_ImageGlobal *vi, f64 v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->d=v;
	return(0);
}

BTEIFGL_API int BSVM2_Interp_SetGlobalA(BSVM2_ImageGlobal *vi, dtVal v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(-1);
	gv->a=v;
	return(0);
}

BTEIFGL_API dtVal BSVM2_Interp_GetGlobalDy(BSVM2_ImageGlobal *vi)
{
	BSVM2_Value *gv;
	dtVal v;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(DTV_UNDEFINED);
	v=BSVM2_Sig_GetSigPtrDtVal(gv, vi->sig);
	return(v);
}

BTEIFGL_API dtVal BSVM2_Interp_SetGlobalDy(BSVM2_ImageGlobal *vi, dtVal v)
{
	BSVM2_Value *gv;
	gv=BSVM2_Interp_GetGlobalValue(vi);
	if(!gv)return(DTV_UNDEFINED);
	BSVM2_Sig_SetSigPtrDtVal(gv, vi->sig, v);
	return(v);
}

BTEIFGL_API int BSVM2_Interp_RunContext(BSVM2_Context *ctx, int lim)
{
	BSVM2_Trace *tr;
	int l;
	
	if(lim<0)
	{
		tr=ctx->trace;
//		while(tr && !(ctx->status))
		while(tr)
			{ tr=tr->Run(ctx->frame, tr); }
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
