#include <bteifgl.h>

#if defined(X86_64) || defined(ARM64)
BGBDT_MM_LObjInfo bgbdt_mm_lobj[16384];
#else
BGBDT_MM_LObjInfo bgbdt_mm_lobj[1024];
#endif

int bgbdt_mm_numlobj=1;

int bgbdt_mm_lobjroot=0;
int bgbdt_mm_lobjfree=0;

int BGBDT_MM_AllocLObjInfo(void)
{
	BGBDT_MM_LObjInfo *tmp;
	int i;
	
	i=bgbdt_mm_lobjfree;
	if(i>0)
	{
		tmp=&bgbdt_mm_lobj[i];
		bgbdt_mm_lobjfree=tmp->lno;
		memset(tmp, 0, sizeof(BGBDT_MM_LObjInfo));
		return(i);
	}
	
	i=bgbdt_mm_numlobj++;
	tmp=&bgbdt_mm_lobj[i];
	memset(tmp, 0, sizeof(BGBDT_MM_LObjInfo));
	return(i);
}

void BGBDT_MM_FreeLObjInfo(int cni)
{
	bgbdt_mm_lobj[cni].lno=bgbdt_mm_lobjfree;
	bgbdt_mm_lobjfree=cni;
}

int BGBDT_MM_LObjInfoToIndex(BGBDT_MM_LObjInfo *inf)
{
	return(inf-bgbdt_mm_lobj);
}

int BGBDT_MM_LObjRebalNode(int cni)
{
//	BGBDT_MM_LObjInfo *inf;
	int lno, rno, llno, lrno, rlno, rrno;
	int ld, rd, cd;
	
//	inf=&bgbdt_mm_lobj[cni];
	
	lno=bgbdt_mm_lobj[cni].lno;
	rno=bgbdt_mm_lobj[cni].rno;
	ld=lno?bgbdt_mm_lobj[lno].depth:0;
	rd=rno?bgbdt_mm_lobj[rno].depth:0;
	
	if(ld>(rd+1))
	{
		llno=bgbdt_mm_lobj[lno].lno;
		lrno=bgbdt_mm_lobj[lno].rno;
		rlno=bgbdt_mm_lobj[rno].lno;
		rrno=bgbdt_mm_lobj[rno].rno;

		bgbdt_mm_lobj[cni].lno=lrno;
		cni=BGBDT_MM_LObjRebalNode(cni);
		bgbdt_mm_lobj[lno].rno=cni;
		lno=BGBDT_MM_LObjRebalNode(lno);
		return(lno);
	}

	if(rd>(ld+1))
	{
		llno=bgbdt_mm_lobj[lno].lno;
		lrno=bgbdt_mm_lobj[lno].rno;
		rlno=bgbdt_mm_lobj[rno].lno;
		rrno=bgbdt_mm_lobj[rno].rno;

		bgbdt_mm_lobj[cni].rno=rlno;
		cni=BGBDT_MM_LObjRebalNode(cni);
		bgbdt_mm_lobj[rno].lno=cni;
		rno=BGBDT_MM_LObjRebalNode(rno);
		return(rno);
	}
	
	cd=max(ld, rd)+1;
	bgbdt_mm_lobj[cni].depth=cd;
	return(cni);
}

int BGBDT_MM_InsertLObjR(int cni, int lni)
{
	BGBDT_MM_LObjInfo *cinf, *linf;
	
	if(lni==cni)
		return(cni);
	
	cinf=&bgbdt_mm_lobj[cni];
	linf=&bgbdt_mm_lobj[lni];

	if(linf->data<cinf->data)
	{
		if(cinf->lno)
		{
			cinf->lno=BGBDT_MM_InsertLObjR(cinf->lno, lni);
			cni=BGBDT_MM_LObjRebalNode(cni);
			return(cni);
		}
		
		cinf->lno=lni;
		cni=BGBDT_MM_LObjRebalNode(cni);
		return(cni);
	}else if(linf->data>cinf->data)
	{
		if(cinf->rno)
		{
			cinf->rno=BGBDT_MM_InsertLObjR(cinf->rno, lni);
			cni=BGBDT_MM_LObjRebalNode(cni);
			return(cni);
		}
		
		cinf->rno=lni;
		cni=BGBDT_MM_LObjRebalNode(cni);
		return(cni);
	}else
	{
		return(cni);
	}
}

int BGBDT_MM_InsertLObj(int lni)
{
	if(bgbdt_mm_lobjroot<=0)
	{
		bgbdt_mm_lobjroot=lni;
		return(0);
	}
	
	bgbdt_mm_lobjroot=BGBDT_MM_InsertLObjR(bgbdt_mm_lobjroot, lni);
	return(0);
}

int BGBDT_MM_AllocLObj(int sz)
{
	BGBDT_MM_LObjInfo *inf;
	int lni;
	
	lni=BGBDT_MM_AllocLObjInfo();
	inf=&bgbdt_mm_lobj[lni];
	inf->data=malloc(sz);
	inf->edata=inf->data+sz;
	
	((BGBDT_MM_ObjHead *)(inf->data))->sz=sz;
	
	BGBDT_MM_InsertLObj(lni);
	
	return(lni);
}

byte *BGBDT_MM_AllocLObjPtr(int sz)
{
	int lni;
	lni=BGBDT_MM_AllocLObj(sz);

	if(lni<=0)
		{ return(NULL); }
	return(bgbdt_mm_lobj[lni].data);
}


int BGBDT_MM_UnlinkLObjR(int cni, int lni)
{
	BGBDT_MM_LObjInfo *cinf, *linf;
	
	if(lni==cni)
	{
		cinf=&bgbdt_mm_lobj[cni];

		if(!cinf->lno && !cinf->rno)
			return(0);
	
		return(cni);
	}
	
	cinf=&bgbdt_mm_lobj[cni];
	linf=&bgbdt_mm_lobj[lni];

	if(linf->data<cinf->data)
	{
		if(cinf->lno)
		{
			cinf->lno=BGBDT_MM_UnlinkLObjR(cinf->lno, lni);
			cni=BGBDT_MM_LObjRebalNode(cni);
			return(cni);
		}
		return(cni);
	}else if(linf->data>cinf->data)
	{
		if(cinf->rno)
		{
			cinf->rno=BGBDT_MM_UnlinkLObjR(cinf->rno, lni);
			cni=BGBDT_MM_LObjRebalNode(cni);
			return(cni);
		}
		return(cni);
	}else
	{
		return(cni);
	}
}

int BGBDT_MM_UnlinkLObj(int lni)
{
	if(bgbdt_mm_lobjroot<=0)
	{
		return(0);
	}
	
	bgbdt_mm_lobjroot=BGBDT_MM_UnlinkLObjR(bgbdt_mm_lobjroot, lni);
	return(0);
}

int BGBDT_MM_LookupLObjR(int cni, byte *ptr)
{
	BGBDT_MM_LObjInfo *cinf;
	
//	if(lni==cni)
//		return(cni);
	
	cinf=&bgbdt_mm_lobj[cni];

	if(ptr<cinf->data)
	{
		if(cinf->lno)
		{
			return(BGBDT_MM_LookupLObjR(cinf->lno, ptr));
		}
		return(-1);
	}else if(ptr>=cinf->edata)
	{
		if(cinf->rno)
		{
			return(BGBDT_MM_LookupLObjR(cinf->rno, ptr));
		}
		return(-1);
	}else
	{
		return(cni);
	}
}

int BGBDT_MM_LookupLObj(byte *ptr)
{
	if(bgbdt_mm_lobjroot<=0)
		{ return(-1); }
	if(!ptr)return(0);
	return(BGBDT_MM_LookupLObjR(bgbdt_mm_lobjroot, ptr));
}

byte *BGBDT_MM_LookupLObjBase(byte *ptr)
{
	BGBDT_MM_LObjInfo *cinf;
	int cni;

	cni=BGBDT_MM_LookupLObj(ptr);

	if(cni<=0)
		return(NULL);

	cinf=&bgbdt_mm_lobj[cni];
	return(cinf->data);
}

void BGBDT_MM_FreeLObjPtr(byte *ptr)
{
	BGBDT_MM_LObjInfo *cinf;
	int cni;

	cni=BGBDT_MM_LookupLObj(ptr);
	if(cni<=0)
		return;
	BGBDT_MM_UnlinkLObj(cni);
	
	cinf=&bgbdt_mm_lobj[cni];
	free(cinf->data);
	
	BGBDT_MM_FreeLObjInfo(cni);
}


byte *BGBDT_MM_GetLObjPtrForIndex(int lni)
{
	if(lni<=0)
		{ return(NULL); }
	return(bgbdt_mm_lobj[lni].data);
}
