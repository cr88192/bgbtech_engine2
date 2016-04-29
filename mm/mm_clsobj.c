#include <bteifgl.h>

BGBDTC_ClassInfo *bgbdtc_clsobj_root;

BGBDTC_ClassInfo *BGBDTC_AllocClass()
{
	BGBDTC_ClassInfo *tmp;
	
	tmp=dtmAlloc("bgbdtc_classinfo_t", sizeof(BGBDTC_ClassInfo));
	return(tmp);
}

BGBDTC_ClassInfo *BGBDTC_LookupClassQName(char *qname)
{
	BGBDTC_ClassInfo *cur;
	
	cur=bgbdtc_clsobj_root;
	while(cur)
	{
		if(cur->qname && !strcmp(cur->qname, qname))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BGBDTC_ClassInfo *BGBDTC_GetClassQName(char *qname)
{
	BGBDTC_ClassInfo *cur;

	cur=BGBDTC_LookupClassQName(qname);
	if(cur)return(cur);
	
	cur=BGBDTC_AllocClass();
	cur->qname=BGBDT_TagStr_StrSymbol(qname);
	
	cur->next=bgbdtc_clsobj_root;
	bgbdtc_clsobj_root=cur;
	
	return(cur);
}

BGBDTC_SlotInfo *BGBDTC_FetchClassSlotIndex(
	BGBDTC_ClassInfo *cls, int idx)
{
	if(!cls || (idx<0) || (idx>=cls->nslots))
		return(NULL);
	return(cls->slots[idx]);
}

int BGBDTC_CheckExpandClassSlotIndex(
	BGBDTC_ClassInfo *cls, int idx)
{
	int i;

	if(!cls->slots)
	{
		if(idx>0)
		{
			i=idx;
			if(i<4)i=4;
		}else
		{
			i=16;
//			while(i<=idx)
//				i=i+(i>>2);
		}
		cls->slots=dtmAlloc("bgbdtc_classslots_t",
			i*sizeof(BGBDTC_SlotInfo *));
		cls->mslots=i;
		return(1);
	}

	if(idx>=cls->mslots)
	{
		i=cls->mslots;
		while(i<=idx)
			i=i+(i>>1);
		cls->slots=dtmRealloc(cls->slots,
			i*sizeof(BGBDTC_SlotInfo *));
		cls->mslots=i;
		return(1);
	}
	
	return(0);
}

BGBDTC_SlotInfo *BGBDTC_GetClassSlotIndex(
	BGBDTC_ClassInfo *cls, int idx)
{
	BGBDTC_SlotInfo *vi;

	if(!cls || (idx<0))
		return(NULL);
	
	if(idx>=cls->nslots)
	{
		BGBDTC_CheckExpandClassSlotIndex(cls, idx);
		cls->nslots=idx+1;
	}
	
	vi=cls->slots[idx];
	if(!vi)
	{
		vi=dtmAlloc("bgbdtc_slotinfo_t", sizeof(BGBDTC_SlotInfo));
		cls->slots[idx]=vi;
	}
	
	return(vi);
}

BGBDTC_SlotInfo *BGBDTC_LookupClassSlotName(
	BGBDTC_ClassInfo *cls, char *name)
{
	BGBDTC_SlotInfo *vi;
	int i;

	for(i=0; i<cls->nslots; i++)
	{
		vi=cls->slots[i];
		if(vi->name && !strcmp(vi->name, name))
			return(vi);
	}
	return(NULL);
}

BGBDTC_SlotInfo *BGBDTC_GetClassSlotName(
	BGBDTC_ClassInfo *cls, char *name)
{
	BGBDTC_SlotInfo *vi;

	vi=BGBDTC_LookupClassSlotName(cls, name);
	if(vi)return(vi);
	
	vi=BGBDTC_GetClassSlotIndex(cls, cls->nslots);
	vi->name=BGBDT_TagStr_StrSymbol(name);
	return(vi);
}
