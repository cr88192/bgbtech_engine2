#include <bteifgl.h>

BSVM2_Context *bsvm2_interp_freectx;

BSVM2_Context *BSVM2_Interp_AllocContext(void)
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


int BSVM2_Interp_RunContext(BSVM2_Context *ctx, int lim)
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
