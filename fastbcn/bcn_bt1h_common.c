BTIC1H_Context *BTIC1H_AllocContext(void)
{
	BTIC1H_Context *tmp;
	
	tmp=malloc(sizeof(BTIC1H_Context));
	memset(tmp, 0, sizeof(BTIC1H_Context));
	return(tmp);
}

int BTIC1H_DestroyContext(BTIC1H_Context *ctx)
{
	if(ctx->blks)free(ctx->blks);
	if(ctx->lblks)free(ctx->lblks);
	free(ctx);
	return(0);
}

void BTIC1H_SetupContextInitial(BTIC1H_Context *ctx)
{
	int i, j, k;

	ctx->mark1=0x12345678;
	ctx->mark2=0x12488421;
	ctx->mark3=0x87654321;

	for(i=0; i<256; i++)
		{ ctx->cmdwin[i]=-1; }

	for(i=0; i<256; i++)
		{ ctx->cmdidx[i]=-1; }

	ctx->cmdwpos=0;

	ctx->cy=0;	ctx->cu=0;
	ctx->cv=0;	ctx->cd=0;
	
	ctx->absyuvbias=128;
	ctx->dyuv=0;
	ctx->updmask=0;
	ctx->nextupdmask=0;
	ctx->absupdmask=0;
	ctx->nextabsupdmask=0;
	ctx->tgtupdmask=0;
	ctx->colorpred=0;
	
	ctx->cdy=0;		ctx->cdu=0;		ctx->cdv=0;

	ctx->qfy=1;		ctx->qfuv=1;	ctx->qfd=1;
	ctx->qfdy=1;	ctx->qfduv=1;

	ctx->qfay=1;	ctx->qfauv=1;	ctx->qfad=1;
	ctx->qfady=1;	ctx->qfaduv=1;

	ctx->fx_qfy  =4096;		ctx->fx_qfuv  =4096;
	ctx->fx_qfdy =4096;		ctx->fx_qfduv =4096;
	ctx->fx_qfay =4096;		ctx->fx_qfauv =4096;
	ctx->fx_qfady=4096;		ctx->fx_qfaduv=4096;
	ctx->fx_qfd  =4096;		ctx->fx_qfad  =4096;

	ctx->rk_cmdidx=2;	ctx->rk_cmdabs=4;
	ctx->rk_cmdcnt=2;

	ctx->rk_parmxy=2;	ctx->rk_parmvar=2;
	ctx->rk_parmval=2;	ctx->rk_parmix=2;

	ctx->rk_dy=2;		ctx->rk_duv=2;
	ctx->rk_ddy=2;		ctx->rk_dduv=2;

	ctx->rk_ay=5;		ctx->rk_auv=5;
	ctx->rk_ady=5;		ctx->rk_aduv=5;

	ctx->rk_qfy=3;		ctx->rk_qfuv=3;
	ctx->rk_qfdy=3;		ctx->rk_qfduv=3;

#if 1
	ctx->cnt_cmds=0;		ctx->bits_cmdidx=0;
	ctx->bits_cmdabs=0;

	ctx->bits_dyuv=0;		ctx->bits_dy=0;
	ctx->bits_duv=0;		ctx->bits_ddy=0;
	ctx->bits_dduv=0;

	ctx->bits_pix4x4=0;		ctx->bits_pix4x4x1=0;
	ctx->bits_pix4x4x3=0;	ctx->bits_pix4x2=0;
	ctx->bits_pix2x2=0;		ctx->bits_pix2x1=0;
#endif

	ctx->cnt_dpts=0;
	ctx->cnt_dzpts=0;
	
	for(i=0; i<8; i++)
	{
		ctx->cnt_dcpts[i]=0;
		ctx->cnt_dczpts[i]=0;
	}

	for(i=0; i<256; i++)
	{
		j=255-i;
		ctx->maskwin[i]=j;
		ctx->maskidx[j]=i;
	}

	for(i=0; i<256; i++)
		{ ctx->cmdwin[i]=i; }
	for(i=0; i<256; i++)
		{ ctx->cmdidx[i]=i; }
}

void BTIC1H_SetupDecodeContextInitial(BTIC1H_Context *ctx)
{
	BTIC1H_SetupContextInitial(ctx);
}
