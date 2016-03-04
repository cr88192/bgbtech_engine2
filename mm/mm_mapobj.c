#include <bteifgl.h>

BGBDT_MapObjFieldInfo *bgbdt_mapobj_finf[256];
u16 bgbdt_mapobj_finfhash[4096];
int bgbdt_mapobj_nfinf=1;

BGBDT_MapObjHead *bgbdt_mapobj_freeobj=NULL;
BGBDT_MapObjNode *bgbdt_mapobj_freenode=NULL;

int BGBDT_MapObj_HashNameSig(char *name, char *sig, char *flsig)
{
	int hi;
	hi=BGBDT_MM_QHashName(name);
//	hi=(hi*4093)+BGBDT_MM_QHashName(sig);
//	hi=(hi*4093)+BGBDT_MM_QHashName(flsig);
	return(hi);
}

int BGBDT_MapObj_CheckStrMatch(char *str, char *ref)
{
	if(!ref)
		return(1);
	if(!str)
		return(0);
	return(!strcmp(str, ref));
}

int BGBDT_MapObj_CheckStrMatch2(char *str, char *ref)
{
	if(!str && !ref)
		return(1);
	if(!str || !ref)
		return(0);
	return(!strcmp(str, ref));
}

BGBDT_MapObjFieldInfo *BGBDT_MapObj_GetFieldInfoForIndex(int sli)
{
	return(bgbdt_mapobj_finf[sli>>8]+(sli&255));
}

int BGBDT_MapObj_LookupFieldIndex(char *name, char *sig, char *flsig)
{
	BGBDT_MapObjFieldInfo *finf;
	int c, hi;

	hi=BGBDT_MapObj_HashNameSig(name, sig, flsig);
	c=bgbdt_mapobj_finfhash[(hi>>12)&4095];
	while(c)
	{
		finf=bgbdt_mapobj_finf[c>>8]+(c&255);
		if(!strcmp(finf->name, name))
		{
			if(BGBDT_MapObj_CheckStrMatch(finf->sig, sig) &&
				BGBDT_MapObj_CheckStrMatch(finf->flsig, flsig))
					return(c);
		}
		c=finf->hnext;
	}
	return(0);
}

int BGBDT_MapObj_GetFieldIndex(char *name, char *sig, char *flsig)
{
	BGBDT_MapObjFieldInfo *finf;
	int c, hi;

//	c=BGBDT_MapObj_LookupFieldIndex(name, sig, flsig);
//	if(c>0)return(c);

	hi=BGBDT_MapObj_HashNameSig(name, sig, flsig);
	hi=(hi>>12)&4095;

	c=bgbdt_mapobj_finfhash[hi];
	while(c)
	{
		finf=bgbdt_mapobj_finf[c>>8]+(c&255);
		if(!strcmp(finf->name, name))
		{
			if(BGBDT_MapObj_CheckStrMatch2(finf->sig, sig) &&
				BGBDT_MapObj_CheckStrMatch2(finf->flsig, flsig))
					return(c);
		}
		c=finf->hnext;
	}
	
	c=bgbdt_mapobj_nfinf++;
	if(!bgbdt_mapobj_finf[c>>8])
	{
		bgbdt_mapobj_finf[c>>8]=dtmAlloc("bgbdt_mapfinf_t",
			256*sizeof(BGBDT_MapObjFieldInfo));
	}

//	hi=BGBDT_MapObj_HashNameSig(name, sig, flsig);
//	hi=(hi>>12)&4095;

	finf=bgbdt_mapobj_finf[c>>8]+(c&255);
	finf->name=frgl_strdup(name);
	finf->sig=frgl_strdup(sig);
	finf->flsig=frgl_strdup(flsig);
	finf->fidx=c;
	
	finf->hnext=bgbdt_mapobj_finfhash[hi];
	bgbdt_mapobj_finfhash[hi]=c;
	return(c);
}

char *BGBDT_MapObj_GetFieldIndexName(int fidx)
{
	BGBDT_MapObjFieldInfo *finf;

	if(!bgbdt_mapobj_finf[fidx>>8])
		return(NULL);

	finf=bgbdt_mapobj_finf[fidx>>8]+(fidx&255);
	return(finf->name);
}

char *BGBDT_MapObj_GetFieldIndexSig(int fidx)
{
	BGBDT_MapObjFieldInfo *finf;

	if(!bgbdt_mapobj_finf[fidx>>8])
		return(NULL);

	finf=bgbdt_mapobj_finf[fidx>>8]+(fidx&255);
	return(finf->sig);
}

char *BGBDT_MapObj_GetFieldIndexFlSig(int fidx)
{
	BGBDT_MapObjFieldInfo *finf;

	if(!bgbdt_mapobj_finf[fidx>>8])
		return(NULL);

	finf=bgbdt_mapobj_finf[fidx>>8]+(fidx&255);
	return(finf->flsig);
}



BGBDT_MapObjHead *BGBDT_MapObj_AllocObj()
{
	BGBDT_MapObjHead *tmp;
	
	tmp=bgbdt_mapobj_freeobj;
	if(tmp)
	{
		bgbdt_mapobj_freeobj=*(BGBDT_MapObjHead **)tmp;
		memset(tmp, 0, sizeof(BGBDT_MapObjHead));
		return(tmp);
	}
	
	tmp=dtmAlloc("bgbdt_mapobj_t", sizeof(BGBDT_MapObjHead));
	return(tmp);
}

BGBDT_MapObjNode *BGBDT_MapObj_AllocNode()
{
	BGBDT_MapObjNode *tmp;
	
	tmp=bgbdt_mapobj_freenode;
	if(tmp)
	{
		bgbdt_mapobj_freenode=tmp->lno;
		memset(tmp, 0, sizeof(BGBDT_MapObjNode));
		return(tmp);
	}
	
	tmp=dtmAlloc("bgbdt_mapnode_t", sizeof(BGBDT_MapObjNode));
	return(tmp);
}

void BGBDT_MapObj_FreeObjNodes(BGBDT_MapObjNode *no)
{
	int i;

	if(no->lvl)
	{
		for(i=0; i<no->nslot; i++)
		{
			BGBDT_MapObj_FreeObjNodes(no->sval[i].vp);
		}
	}else
	{
	}
	
	no->lno=bgbdt_mapobj_freenode;
	bgbdt_mapobj_freenode=no;
}

void BGBDT_MapObj_FreeObj(BGBDT_MapObjHead *obj)
{
	if(obj->node)
		BGBDT_MapObj_FreeObjNodes(obj->node);
	
	*(BGBDT_MapObjHead **)obj=bgbdt_mapobj_freeobj;
	bgbdt_mapobj_freeobj=obj;
}


int BGBDT_MapObj_SlotSoftMatch(int sli, int sref)
{
	BGBDT_MapObjFieldInfo *finfs, *finfr;

	if(sli==sref)
		return(1);
		
	finfs=BGBDT_MapObj_GetFieldInfoForIndex(sli);
	finfr=BGBDT_MapObj_GetFieldInfoForIndex(sref);
	
//	if(BGBDT_MapObj_CheckStrMatch(finfs->name, finfr->name) &&
//		BGBDT_MapObj_CheckStrMatch(finfs->sig, finfr->sig) &&
//		BGBDT_MapObj_CheckStrMatch(finfs->flsig, finfr->flsig))
	if(BGBDT_MapObj_CheckStrMatch(finfs->name, finfr->name) &&
		BGBDT_MapObj_CheckStrMatch(finfs->sig, finfr->sig))
			return(1);
	return(0);
}

dtVal *BGBDT_MapObj_LookupNodeSlotSoftRef(BGBDT_MapObjHead *obj,
	BGBDT_MapObjNode *no, int sli)
{
	int i, j;
	if(no->lvl)
	{
		for(i=0; i<no->nslot; i++)
		{
			return(BGBDT_MapObj_LookupNodeSlotSoftRef(
				obj, no->sval[i].vp, sli));
		}

#if 0
		j=no->nslot-1;
		for(i=0; i<j; i++)
		{
			if((sli>=no->sidx[i]) && (sli<no->sidx[i+1]))
				return(BGBDT_MapObj_LookupNodeSlotSoftRef(
					obj, no->sval[i].vp, sli));
		}
		return(BGBDT_MapObj_LookupNodeSlotSoftRef(
			obj, no->sval[i].vp, sli));
#endif
	}

	for(i=0; i<no->nslot; i++)
	{
		if(BGBDT_MapObj_SlotSoftMatch(no->sidx[i], sli))
			return(&no->sval[i]);
	}
	return(NULL);
}

dtVal *BGBDT_MapObj_LookupObjSlotSoftRef(BGBDT_MapObjHead *obj, int sli)
{
	int i;
	
	if(obj->node)
	{
		return(BGBDT_MapObj_LookupNodeSlotSoftRef(obj, obj->node, sli));
	}
	
	for(i=0; i<obj->nslot; i++)
	{
		if(BGBDT_MapObj_SlotSoftMatch(obj->sidx[i], sli))
			return(obj->sval+i);
	}
	return(NULL);
}

dtVal *BGBDT_MapObj_LookupNodeSlotRef(BGBDT_MapObjHead *obj,
	BGBDT_MapObjNode *no, int sli)
{
	int i, j;

	if(no->lvl)
	{
		j=no->nslot-1;
		for(i=0; i<j; i++)
		{
			if((sli>=no->sidx[i]) && (sli<no->sidx[i+1]))
				return(BGBDT_MapObj_LookupNodeSlotRef(
					obj, no->sval[i].vp, sli));
		}
		return(BGBDT_MapObj_LookupNodeSlotRef(
			obj, no->sval[i].vp, sli));
	}

	for(i=0; i<no->nslot; i++)
	{
		if(sli==no->sidx[i])
			return(&no->sval[i]);
	}
	return(NULL);
}

dtVal *BGBDT_MapObj_LookupObjSlotRef(BGBDT_MapObjHead *obj, int sli)
{
	int i;

	if(obj->node)
	{
		return(BGBDT_MapObj_LookupNodeSlotRef(obj, obj->node, sli));
	}
	
	for(i=0; i<obj->nslot; i++)
	{
		if(obj->sidx[i]==sli)
			return(obj->sval+i);
	}
	return(NULL);
}

void BGBDT_MapObj_InsertOrSplitNode(BGBDT_MapObjHead *obj,
	BGBDT_MapObjNode *no, BGBDT_MapObjNode *cno1, BGBDT_MapObjNode *cno2)
{
	BGBDT_MapObjNode *no2, *tno;
	int i, j;

	if(!no)
	{
		no=BGBDT_MapObj_AllocNode();
		no->nslot=2;
		no->sval[0].vp=cno1;
		no->sidx[0]=cno1->sidx[0];
		no->sval[1].vp=cno2;
		no->sidx[1]=cno2->sidx[0];
		no->lvl=no->lvl+1;
		
		obj->node=no;
		return;
	}

	if(no->nslot<16)
	{
		i=no->nslot++;
		while(i>0)
		{
			if(no->sval[i-1].vp==cno1)
				break;
			no->sval[i]=no->sval[i-1];
			no->sidx[i]=no->sidx[i-1];
			i--;
		}
		cno2->pno=no;
		no->sval[i].vp=cno2;
		no->sidx[i]=cno2->sidx[0];
		return;
	}

	no2=BGBDT_MapObj_AllocNode();
	j=no->nslot>>1;
	
	for(i=j; i<no->nslot; i++)
	{
		no2->sval[i-j]=no->sval[i];
		no2->sidx[i-j]=no->sidx[i];
		tno=no->sval[i].vp;
		tno->pno=no2;
	}
	no2->nslot=no->nslot-j;
	no->nslot=j;
	
	no2->rno=no->rno;
	no2->lno=no;
	no->rno=no2;
	BGBDT_MapObj_InsertOrSplitNode(obj, no->pno, no, no2);
	
	if(cno2->sidx[0]<no2->sidx[0])
		{ BGBDT_MapObj_InsertOrSplitNode(obj, no, cno1, cno2); }
	else
		{ BGBDT_MapObj_InsertOrSplitNode(obj, no2, cno1, cno2); }
}

dtVal *BGBDT_MapObj_GetNodeSlotRef(BGBDT_MapObjHead *obj,
	BGBDT_MapObjNode *no, int sli)
{
	BGBDT_MapObjNode *no2;
	int i, j;

	if(no->lvl)
	{
		j=no->nslot-1;
		for(i=0; i<j; i++)
		{
			if((sli>=no->sidx[i]) && (sli<no->sidx[i+1]))
				return(BGBDT_MapObj_GetNodeSlotRef(
					obj, no->sval[i].vp, sli));
		}
		return(BGBDT_MapObj_GetNodeSlotRef(
			obj, no->sval[i].vp, sli));
	}

	for(i=0; i<no->nslot; i++)
	{
		if(sli==no->sidx[i])
			return(&(no->sval[i]));
	}

	if(no->nslot<16)
	{
		i=no->nslot++;
		while(i>0)
		{
			if(no->sidx[i-1]<sli)
				break;
			no->sval[i]=no->sval[i-1];
			no->sidx[i]=no->sidx[i-1];
			i--;
		}
		no->sidx[i]=sli;
		return(&(no->sval[i]));
	}
	
	no2=BGBDT_MapObj_AllocNode();
	j=no->nslot>>1;
	
	for(i=j; i<no->nslot; i++)
	{
		no2->sval[i-j]=no->sval[i];
		no2->sidx[i-j]=no->sidx[i];
	}
	no2->nslot=no->nslot-j;
	no->nslot=j;
	
	no2->rno=no->rno;
	no2->lno=no;
	no->rno=no2;
	BGBDT_MapObj_InsertOrSplitNode(obj, no->pno, no, no2);
	
	if(sli<no2->sidx[0])
		{ return(BGBDT_MapObj_GetNodeSlotRef(obj, no, sli)); }
	else
		{ return(BGBDT_MapObj_GetNodeSlotRef(obj, no2, sli)); }
	
//	return(NULL);
}

dtVal *BGBDT_MapObj_GetObjSlotRef(BGBDT_MapObjHead *obj, int sli)
{
	dtVal *trv;
	int i, j, k;

	if(obj->node)
	{
		return(BGBDT_MapObj_GetNodeSlotRef(obj, obj->node, sli));
	}
	
	for(i=0; i<obj->nslot; i++)
	{
		if(obj->sidx[i]==sli)
			return(obj->sval+i);
	}
	
	if(obj->nslot<8)
	{
		i=obj->nslot++;
		while(i>0)
		{
			if(obj->sidx[i-1]<sli)
				break;
			obj->sval[i]=obj->sval[i-1];
			obj->sidx[i]=obj->sidx[i-1];
			i--;
		}
		
		obj->sidx[i]=sli;
		return(obj->sval+i);
	}
	
	obj->node=BGBDT_MapObj_AllocNode();
	for(i=0; i<obj->nslot; i++)
	{
		trv=BGBDT_MapObj_GetNodeSlotRef(obj,
			obj->node, obj->sidx[i]);
		*trv=obj->sval[i];
	}
	
	trv=BGBDT_MapObj_GetNodeSlotRef(obj,
		obj->node, sli);
	return(trv);
}

dtVal BGBDT_MapObj_LookupObjSlotValueName(
	BGBDT_MapObjHead *obj, char *name)
{
	dtVal *trv;
	int sli;
	
//	sli=BGBDT_MapObj_GetFieldIndex(name, "r", NULL);
//	trv=BGBDT_MapObj_LookupObjSlotRef(obj, sli);

	sli=BGBDT_MapObj_GetFieldIndex(name, NULL, NULL);
	trv=BGBDT_MapObj_LookupObjSlotSoftRef(obj, sli);
	if(trv)return(*trv);
	return(DTV_UNDEFINED);
}

int BGBDT_MapObj_SetObjSlotValueName(
	BGBDT_MapObjHead *obj, char *name, dtVal val)
{
	dtVal *trv;
	int sli;
	
//	sli=BGBDT_MapObj_GetFieldIndex(name, "r", NULL);
//	trv=BGBDT_MapObj_LookupObjSlotRef(obj, sli);

	sli=BGBDT_MapObj_GetFieldIndex(name, NULL, NULL);
	trv=BGBDT_MapObj_LookupObjSlotSoftRef(obj, sli);

	if(trv)
	{
		*trv=val;
		return(0);
	}
	return(-1);
}

int BGBDT_MapObj_BindObjSlotValueName(
	BGBDT_MapObjHead *obj, char *name, dtVal val)
{
	dtVal *trv;
	int sli;

	sli=BGBDT_MapObj_GetFieldIndex(name, NULL, NULL);
	trv=BGBDT_MapObj_LookupObjSlotSoftRef(obj, sli);
	if(trv)
	{
		*trv=val;
		return(0);
	}
	
	sli=BGBDT_MapObj_GetFieldIndex(name, "r", NULL);
	trv=BGBDT_MapObj_GetObjSlotRef(obj, sli);
	if(trv)
	{
		*trv=val;
		return(0);
	}
	return(-1);
}


BTEIFGL_API dtVal BGBDT_MapObj_AllocObjv()
{
	BGBDT_MapObjHead *tmp;
	tmp=BGBDT_MapObj_AllocObj();
	return(dtvWrapPtr(tmp));
}

BTEIFGL_API int BGBDT_MapObj_BindObjvSlotValueName(
	dtVal objv, char *name, dtVal val)
{
	BGBDT_MapObjHead *obj;
	
	obj=dtvUnwrapPtrF(objv);
	return(BGBDT_MapObj_BindObjSlotValueName(obj, name, val));
}

BTEIFGL_API dtVal BGBDT_MapObj_GetObjvSlotValue(dtVal objv, int sli)
{
	BGBDT_MapObjHead *obj;
	dtVal *trv;
	
	obj=dtvUnwrapPtrF(objv);

	trv=BGBDT_MapObj_LookupObjSlotSoftRef(obj, sli);
	if(trv)return(*trv);
	return(DTV_UNDEFINED);
}

BTEIFGL_API dtVal BGBDT_MapObj_GetObjvSlotValueName(dtVal objv, char *name)
{
	BGBDT_MapObjHead *obj;
	dtVal *trv;
	int sli;
	
	obj=dtvUnwrapPtrF(objv);

	sli=BGBDT_MapObj_GetFieldIndex(name, NULL, NULL);
	trv=BGBDT_MapObj_LookupObjSlotSoftRef(obj, sli);
	if(trv)return(*trv);
	return(DTV_UNDEFINED);
}

BTEIFGL_API int BGBDT_MapObj_IsObjectP(dtVal objv)
{
	static int objty_mapobj=-1;
	if(objty_mapobj<0)
	{
		objty_mapobj=BGBDT_MM_GetIndexObjTypeName("bgbdt_mapobj_t");
	}
	return(dtvCheckPtrTagP(objv, objty_mapobj));
}


int BGBDT_MapObj_QueryNodeSlotList(BGBDT_MapObjHead *obj,
	BGBDT_MapObjNode *no, int *slis, int mslis)
{
	BGBDT_MapObjNode *no2;
	int i, j, k, l;

	if(mslis<=0)
		return(0);

	if(no->lvl)
	{
		j=0;
		for(i=0; i<no->nslot; i++)
		{
			j+=BGBDT_MapObj_QueryNodeSlotList(
					obj, no->sval[i].vp,
					slis?(slis+j):NULL, mslis-j);
		}
		return(j);
	}

	l=no->nslot;
	if(mslis<l)
		l=mslis;
		
	if(slis)
	{
		for(i=0; i<l; i++)
			{ slis[i]=no->sidx[i]; }
	}

	return(l);
}

BTEIFGL_API int BGBDT_MapObj_GetObjSlotCount(dtVal objv)
{
	BGBDT_MapObjHead *obj;
	int i;
	
	obj=dtvUnwrapPtrF(objv);
	
	if(obj->node)
	{
		i=BGBDT_MapObj_QueryNodeSlotList(obj, obj->node, NULL, 99999999);
		return(i);
	}
	
	return(obj->nslot);
}

BTEIFGL_API int BGBDT_MapObj_QueryObjSlotList(
	dtVal objv, int *slis, int mslis)
{
	BGBDT_MapObjHead *obj;
	int i, n;
	
	obj=dtvUnwrapPtrF(objv);

	if(obj->node)
	{
		i=BGBDT_MapObj_QueryNodeSlotList(obj,
			obj->node, slis, mslis);
		return(i);
	}
	
	n=obj->nslot;
	if(mslis<n)n=mslis;
	
	for(i=0; i<n; i++)
	{
		slis[i]=obj->sidx[i];
	}
	
	return(n);
}
