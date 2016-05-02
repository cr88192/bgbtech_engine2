#include <bteifgl.h>

BSVM2_Context *bsvm2_interp_freectx;

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

BTEIFGL_API BSVM2_Frame *BSVM2_Interp_AllocFrame(BSVM2_Context *ctx)
{
	BSVM2_Frame *tmp;
	
	tmp=ctx->freeframe;
	if(tmp)
	{
		ctx->freeframe=tmp->rnext;
		memset(tmp, 0, sizeof(BSVM2_Frame));
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


BTEIFGL_API int BSVM2_Interp_RunContext(BSVM2_Context *ctx, int lim)
{
	BSVM2_Trace *tr;
	int l;
	
	tr=ctx->trace; l=lim;
	while(tr && !(ctx->status) && ((l--)>0))
		{ tr=tr->Run(ctx->frame, tr); }
	if(tr)
		{ ctx->trace=tr; }
	return(ctx->status);
}
