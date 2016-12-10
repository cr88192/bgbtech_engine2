volatile BGBDT_WorkItem *bgbdt_worklive=NULL;
volatile BGBDT_WorkItem *bgbdt_workend=NULL;

volatile BGBDT_WorkItem *bgbdt_workfree=NULL;

void *bgbdt_workmutex=NULL;
volatile int bgbdt_nworkers=0;
volatile bool bgbdt_killworkers=0;

int bgbdt_work_worker(void *ptr)
{
	BGBDT_WorkItem *slc;
//	BTIC4B_Context *ctx;
	int idle1, idle2;
	int i;

	bgbdt_nworkers++;
	idle1=1<<16; idle2=256;
	while(1)
	{
		thLockFastMutex(bgbdt_workmutex);
		slc=(BGBDT_WorkItem *)bgbdt_worklive;
		if(slc)
			{ bgbdt_worklive=slc->next; }
		thUnlockFastMutex(bgbdt_workmutex);

		if(!slc)
		{
			if(bgbdt_killworkers)
				break;

			if(idle2>0)
			{
				idle2--;
				thSleep(0);
				continue;
			}
			if(idle1<=0)
				break;
			idle1--;
			thSleep(1);
			continue;
		}

		idle1=1<<16;
		idle2=256;

//		ctx=&slc->tctx;
//		BTIC4B_SetupContextQf(ctx, slc->qf);
//		BTIC4B_EncImageBGRA(ctx,
//			slc->blks, slc->ibuf, slc->xs, slc->ys);
//		BTIC4B_EncImgBlocks(ctx, slc->obuf,
//			slc->blks, slc->lblks,
//			slc->xs, slc->ys, slc->qf);
		
		i=slc->DoWork(slc);
		
		if(i>0)
		{
			thLockFastMutex(bgbdt_workmutex);
			if(bgbdt_worklive)
			{
				bgbdt_workend->next=slc;
				bgbdt_workend=slc;
			}else
			{
				slc->next=NULL;
				bgbdt_worklive=slc;
				bgbdt_workend=slc;		
			}
			thUnlockFastMutex(bgbdt_workmutex);
		}else
		{
			slc->done=1;
		}
	}
	bgbdt_nworkers--;
	return(0);
}

BS2VM_API void BGBDT_Work_SubmitItem(BGBDT_WorkItem *item)
{
	int i;

	if(!bgbdt_workmutex)
	{
		bgbdt_workmutex=thFastMutex();
		for(i=0; i<4; i++)
			{ thThread(bgbdt_work_worker, NULL); }
	}else
	{
		if(bgbdt_nworkers<3)
		{
			for(i=0; i<2; i++)
				{ thThread(bgbdt_work_worker, NULL); }
		}
	}

	thLockFastMutex(bgbdt_workmutex);
//	item->next=bgbdt_worklive;
//	bgbdt_worklive=item;
	if(bgbdt_worklive)
	{
		bgbdt_workend->next=item;
		bgbdt_workend=item;
	}else
	{
		item->next=NULL;
		bgbdt_worklive=item;
		bgbdt_workend=item;		
	}
	thUnlockFastMutex(bgbdt_workmutex);
}

BS2VM_API BGBDT_WorkItem *BGBDT_Work_AllocItem()
{
	BGBDT_WorkItem *tmp;

	if(!bgbdt_workmutex)
		bgbdt_workmutex=thFastMutex();

	thLockFastMutex(bgbdt_workmutex);
	tmp=(BGBDT_WorkItem *)bgbdt_workfree;
	if(tmp)
	{
		bgbdt_workfree=tmp->next;
		thUnlockFastMutex(bgbdt_workmutex);
		
		memset(tmp, 0, sizeof(BGBDT_WorkItem));
		return(tmp);
	}
	thUnlockFastMutex(bgbdt_workmutex);
	
	tmp=dtmAlloc("bgbdt_workitem_t", sizeof(BGBDT_WorkItem));
	return(tmp);
}

BS2VM_API void BGBDT_Work_FreeItem(BGBDT_WorkItem *item)
{
	if(!bgbdt_workmutex)
		bgbdt_workmutex=thFastMutex();

	thLockFastMutex(bgbdt_workmutex);
	item->next=bgbdt_workfree;
	bgbdt_workfree=item;
	thUnlockFastMutex(bgbdt_workmutex);
}

BS2VM_API void BGBDT_Work_KillWorkers(void)
{
	bgbdt_killworkers=true;
}
