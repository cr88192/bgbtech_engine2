#include <bteifgl.h>

BGBDT_MM_RegionInfo *bgbdt_mm_freergn;
BGBDT_MM_RegionInfo *bgbdt_mm_rgn[4096];
int bgbdt_mm_nrgn=1;
int bgbdt_mm_rovrgn=1;

int BGBDT_MM_FindFreeRegionId(void)
{
	int i;
	
	i=bgbdt_mm_rovrgn;
	while(1)
	{
		if(!bgbdt_mm_rgn[i])
			return(i);
		i++;
		if(i>=bgbdt_mm_nrgn)i=1;
		if(i==bgbdt_mm_rovrgn)
			break;
	}
	
	bgbdt_mm_rovrgn=1;
	i=bgbdt_mm_nrgn++;
	return(i);
}

BGBDT_MM_RegionInfo *BGBDT_MM_AllocRegionInfo(void)
{
	BGBDT_MM_RegionInfo *tmp;
	
	tmp=bgbdt_mm_freergn;
	if(tmp)
	{
		bgbdt_mm_freergn=tmp->next;
		memset(tmp, 0, sizeof(BGBDT_MM_RegionInfo));
		return(tmp);
	}
	
	tmp=dtmAlloc("bgbdt_mm_regioninfo_t",
		sizeof(BGBDT_MM_RegionInfo));
	return(tmp);
}

BGBDT_MM_RegionInfo *BGBDT_MM_AllocRegion(void)
{
	BGBDT_MM_RegionInfo *tmp;
	int id;

	tmp=BGBDT_MM_AllocRegionInfo();
	id=BGBDT_MM_FindFreeRegionId();
	bgbdt_mm_rgn[id]=tmp;
	tmp->rgnid=id;
	
	return(tmp);
}

int BGBDT_MM_AllocRegionId(void)
{
	BGBDT_MM_RegionInfo *tmp;
	
	tmp=BGBDT_MM_AllocRegion();
	return(tmp->rgnid);
}

int BGBDT_MM_GetObjRegionIdF(void *obj)
{
	BGBDT_MM_ObjHead *objh;
	void *bptr;
	int tyid;

	if(!ptr)return(0);
	
	bptr=((byte *)ptr)-sizeof(BGBDT_MM_ObjHead);
	objh=bptr;
//	tyid=objh->ty&4095;
//	BGBDT_MM_FreeObjectInner(bptr);
	return(objh->mrgn);
}

int BGBDT_MM_AddObjRegionF(BGBDT_MM_RegionInfo *rgn, void *obj, int lvl)
{
	BGBDT_MM_ObjHead *objh;
	void *bptr;
	int tyid;
	int ri;

	if(!ptr)return(0);
	
	bptr=((byte *)ptr)-sizeof(BGBDT_MM_ObjHead);
	objh=bptr;
	objh->mrgn=(rgn->rgnid<<12)|(lvl&4095);
	
	if(!rgn->ri_ptr)
	{
		i=16384;
//		rgn->ri_ptr=dtmAlloc("bgbdt_mm_regioninfo_riptr_t",
//			i*sizeof(void *));
		rgn->ri_ptr=frgl_malloc(i*sizeof(void *));
		rgn->ri_next=frgl_malloc(i*sizeof(int));
		rgn->ri_nptr=1;
		rgn->ri_mptr=i;
	}
	
	ri=rgn->rifree;
	if(ri>0)
		{ rgn->rifree=rgn->ri_next[ri]; }
	else
	{
		if((rgn->ri_nptr+1)>=rgn->ri_mptr)
		{
			i=rgn->ri_mptr+(rgn->ri_mptr>>1);
			rgn->ri_ptr=frgl_realloc(rgn->ri_ptr, i*sizeof(void *));
			rgn->ri_next=frgl_realloc(rgn->ri_next, i*sizeof(int));
			rgn->ri_mptr=i;
		}

		ri=rgn->ri_nptr++;
	}
	
	rgn->ri_ptr[ri]=obj;
	rgn->ri_next[ri]=rgn->rilvl[lvl];
	rgn->rilvl[lvl]=ri;
	return(1);
}

int BGBDT_MM_DestroyRegionLevel(BGBDT_MM_RegionInfo *rgn, int lvl)
{
	int ri, ri2;
	
	ri=rgn->rilvl[lvl];
	rgn->rilvl[lvl]=0;
	while(ri>0)
	{
		BGBDT_MM_Destroy(rgn->ri_ptr[ri]);
		ri2=rgn->ri_next[ri];
		rgn->ri_next[ri]=rgn->rifree;
		rgn->ri_ptr[ri]=NULL;
		rgn->rifree=ri;
		ri=ri2;
	}
	return(0);
}
