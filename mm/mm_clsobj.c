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


BGBDTC_ClassInfo *BGBDTC_LookupClassSqGix(int sqid, int gix)
{
	BGBDTC_ClassInfo *cur;
	
	cur=bgbdtc_clsobj_root;
	while(cur)
	{
		if((cur->sqid==sqid) && (cur->gix==gix))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BGBDTC_ClassInfo *BGBDTC_GetClassSqGix(int sqid, int gix)
{
	BGBDTC_ClassInfo *cur;

	cur=BGBDTC_LookupClassSqGix(sqid, gix);
	if(cur)return(cur);
	
	cur=BGBDTC_AllocClass();
	cur->sqid=sqid;
	cur->gix=gix;
//	cur->qname=BGBDT_TagStr_StrSymbol(qname);
	
	cur->next=bgbdtc_clsobj_root;
	bgbdtc_clsobj_root=cur;
	
	return(cur);
}

void *BGBDTC_SlotI_GetPtr_Dfl(dtcObject obj, dtcField fi)
{
	return(((byte *)obj)+fi->offs);
}


s32 BGBDTC_SlotI_GetI_DflSB(dtcObject obj, dtcField fi)
	{ return(*(sbyte *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DflUB(dtcObject obj, dtcField fi)
	{ return(*(byte *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DflSS(dtcObject obj, dtcField fi)
	{ return(*(s16 *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DflUS(dtcObject obj, dtcField fi)
	{ return(*(u16 *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DflI(dtcObject obj, dtcField fi)
	{ return(*(s32 *)(((byte *)obj)+fi->offs)); }
s32 BGBDTC_SlotI_GetI_DeL(dtcObject obj, dtcField fi)
	{ return(fi->GetL(obj, fi)); }

s64 BGBDTC_SlotI_GetL_DflSI(dtcObject obj, dtcField fi)
	{ return(*(s32 *)(((byte *)obj)+fi->offs)); }
s64 BGBDTC_SlotI_GetL_DflUI(dtcObject obj, dtcField fi)
	{ return(*(u32 *)(((byte *)obj)+fi->offs)); }

s64 BGBDTC_SlotI_GetL_DflL(dtcObject obj, dtcField fi)
	{ return(*(s64 *)(((byte *)obj)+fi->offs)); }
s64 BGBDTC_SlotI_GetL_DeI(dtcObject obj, dtcField fi)
	{ return(fi->GetI(obj, fi)); }
s64 BGBDTC_SlotI_GetL_DeF(dtcObject obj, dtcField fi)
	{ return(fi->GetF(obj, fi)); }
s64 BGBDTC_SlotI_GetL_DeD(dtcObject obj, dtcField fi)
	{ return(fi->GetD(obj, fi)); }

f32 BGBDTC_SlotI_GetF_DflF(dtcObject obj, dtcField fi)
	{ return(*(f32 *)(((byte *)obj)+fi->offs)); }
f32 BGBDTC_SlotI_GetF_DflD(dtcObject obj, dtcField fi)
	{ return(*(f64 *)(((byte *)obj)+fi->offs)); }
f32 BGBDTC_SlotI_GetF_DeL(dtcObject obj, dtcField fi)
	{ return(fi->GetL(obj, fi)); }

f64 BGBDTC_SlotI_GetD_DflF(dtcObject obj, dtcField fi)
	{ return(*(f32 *)(((byte *)obj)+fi->offs)); }
f64 BGBDTC_SlotI_GetD_DflD(dtcObject obj, dtcField fi)
	{ return(*(f64 *)(((byte *)obj)+fi->offs)); }
f64 BGBDTC_SlotI_GetD_DeL(dtcObject obj, dtcField fi)
	{ return(fi->GetL(obj, fi)); }

dtVal BGBDTC_SlotI_GetA_DflA(dtcObject obj, dtcField fi)
	{ return(*(dtVal *)(((byte *)obj)+fi->offs)); }
dtVal BGBDTC_SlotI_GetA_DflP(dtcObject obj, dtcField fi)
	{ return(dtvWrapPtr(*(void **)(((byte *)obj)+fi->offs))); }
	
dtVal BGBDTC_SlotI_GetA_DeI(dtcObject obj, dtcField fi)
	{ return(dtvWrapInt(fi->GetI(obj, fi))); }
dtVal BGBDTC_SlotI_GetA_DeUI(dtcObject obj, dtcField fi)
	{ return(dtvWrapUInt(fi->GetI(obj, fi))); }
dtVal BGBDTC_SlotI_GetA_DeL(dtcObject obj, dtcField fi)
	{ return(dtvWrapLong(fi->GetL(obj, fi))); }
dtVal BGBDTC_SlotI_GetA_DeF(dtcObject obj, dtcField fi)
	{ return(dtvWrapFloat(fi->GetF(obj, fi))); }
dtVal BGBDTC_SlotI_GetA_DeD(dtcObject obj, dtcField fi)
	{ return(dtvWrapDouble(fi->GetD(obj, fi))); }

s32 BGBDTC_SlotI_GetI_DeA(dtcObject obj, dtcField fi)
	{ return(dtvUnwrapInt(fi->GetA(obj, fi))); }
s64 BGBDTC_SlotI_GetL_DeA(dtcObject obj, dtcField fi)
	{ return(dtvUnwrapLong(fi->GetA(obj, fi))); }
f32 BGBDTC_SlotI_GetF_DeA(dtcObject obj, dtcField fi)
	{ return(dtvUnwrapFloat(fi->GetA(obj, fi))); }
f64 BGBDTC_SlotI_GetD_DeA(dtcObject obj, dtcField fi)
	{ return(dtvUnwrapDouble(fi->GetA(obj, fi))); }

void BGBDTC_SlotI_SetI_DflB(dtcObject obj, dtcField fi, s32 v)
	{ *(byte *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetI_DflS(dtcObject obj, dtcField fi, s32 v)
	{ *(s16 *)(((byte *)obj)+fi->offs)=v; }

void BGBDTC_SlotI_SetI_DflI(dtcObject obj, dtcField fi, s32 v)
	{ *(s32 *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetL_DflL(dtcObject obj, dtcField fi, s64 v)
	{ *(s64 *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetF_DflF(dtcObject obj, dtcField fi, f32 v)
	{ *(f32 *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetD_DflD(dtcObject obj, dtcField fi, f64 v)
	{ *(f64 *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetA_DflA(dtcObject obj, dtcField fi, dtVal v)
	{ *(dtVal *)(((byte *)obj)+fi->offs)=v; }
void BGBDTC_SlotI_SetA_DflP(dtcObject obj, dtcField fi, dtVal v)
	{ *(void **)(((byte *)obj)+fi->offs)=dtvUnwrapPtr(v); }

void BGBDTC_SlotI_SetI_DeL(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetL(obj, fi, v); }
void BGBDTC_SlotI_SetI_DeF(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetF(obj, fi, v); }
void BGBDTC_SlotI_SetI_DeD(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetD(obj, fi, v); }

void BGBDTC_SlotI_SetL_DeI(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetI(obj, fi, v); }
void BGBDTC_SlotI_SetL_DeF(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetF(obj, fi, v); }
void BGBDTC_SlotI_SetL_DeD(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetD(obj, fi, v); }

void BGBDTC_SlotI_SetF_DeI(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetI(obj, fi, v); }
void BGBDTC_SlotI_SetF_DeL(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetL(obj, fi, v); }
void BGBDTC_SlotI_SetF_DeD(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetD(obj, fi, v); }

void BGBDTC_SlotI_SetD_DeI(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetI(obj, fi, v); }
void BGBDTC_SlotI_SetD_DeL(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetL(obj, fi, v); }
void BGBDTC_SlotI_SetD_DeF(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetF(obj, fi, v); }


void BGBDTC_SlotI_SetA_DeI(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetI(obj, fi, dtvUnwrapInt(v)); }
void BGBDTC_SlotI_SetA_DeL(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetL(obj, fi, dtvUnwrapLong(v)); }
void BGBDTC_SlotI_SetA_DeF(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetF(obj, fi, dtvUnwrapFloat(v)); }
void BGBDTC_SlotI_SetA_DeD(dtcObject obj, dtcField fi, dtVal v)
	{ fi->SetD(obj, fi, dtvUnwrapDouble(v)); }

void BGBDTC_SlotI_SetI_DeA(dtcObject obj, dtcField fi, s32 v)
	{ fi->SetA(obj, fi, dtvWrapInt(v)); }
void BGBDTC_SlotI_SetL_DeA(dtcObject obj, dtcField fi, s64 v)
	{ fi->SetA(obj, fi, dtvWrapLong(v)); }
void BGBDTC_SlotI_SetF_DeA(dtcObject obj, dtcField fi, f32 v)
	{ fi->SetA(obj, fi, dtvWrapFloat(v)); }
void BGBDTC_SlotI_SetD_DeA(dtcObject obj, dtcField fi, f64 v)
	{ fi->SetA(obj, fi, dtvWrapDouble(v)); }


int BGBDTC_SlotSetupVtGetSet(
	BGBDTC_ClassInfo *cls,
	BGBDTC_SlotInfo *vi)
{
	vi->GetPtr=BGBDTC_SlotI_GetPtr_Dfl;
	
	switch(*vi->sig)
	{
	case 'a':	case 'b':	case 'c':
		vi->GetI=BGBDTC_SlotI_GetI_DflSB;
		vi->GetL=BGBDTC_SlotI_GetL_DeI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflB;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;
		break;
	
	case 'h':
		vi->GetI=BGBDTC_SlotI_GetI_DflUB;
		vi->GetL=BGBDTC_SlotI_GetL_DeI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflB;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;
		break;

	case 's':
		vi->GetI=BGBDTC_SlotI_GetI_DflSS;
		vi->GetL=BGBDTC_SlotI_GetL_DeI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflS;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;
		break;
	case 't':	case 'w':
		vi->GetI=BGBDTC_SlotI_GetI_DflUS;
		vi->GetL=BGBDTC_SlotI_GetL_DeI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflS;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;
		break;
	case 'i':
		vi->GetI=BGBDTC_SlotI_GetI_DflI;
		vi->GetL=BGBDTC_SlotI_GetL_DflSI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeI;
		vi->SetI=BGBDTC_SlotI_SetI_DflI;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;
		break;
	case 'j':
		vi->GetI=BGBDTC_SlotI_GetI_DflI;
		vi->GetL=BGBDTC_SlotI_GetL_DflUI;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeUI;
		vi->SetI=BGBDTC_SlotI_SetI_DflI;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
		vi->SetF=BGBDTC_SlotI_SetF_DeI;
		vi->SetD=BGBDTC_SlotI_SetD_DeI;
		vi->SetA=BGBDTC_SlotI_SetA_DeI;
		break;

	case 'l':
#if defined(X86) || defined(ARM)
		vi->GetI=BGBDTC_SlotI_GetI_DflI;
		vi->GetL=BGBDTC_SlotI_GetL_DflSI;
		vi->SetI=BGBDTC_SlotI_SetI_DflI;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
#else
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DflL;
		vi->SetI=BGBDTC_SlotI_SetI_DeL;
		vi->SetL=BGBDTC_SlotI_SetL_DflL;
#endif
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeL;
		vi->SetF=BGBDTC_SlotI_SetF_DeL;
		vi->SetD=BGBDTC_SlotI_SetD_DeL;
		vi->SetA=BGBDTC_SlotI_SetA_DeL;
		break;

	case 'm':
#if defined(X86) || defined(ARM)
		vi->GetI=BGBDTC_SlotI_GetI_DflI;
		vi->GetL=BGBDTC_SlotI_GetL_DflUI;
		vi->SetI=BGBDTC_SlotI_SetI_DflI;
		vi->SetL=BGBDTC_SlotI_SetL_DeI;
#else
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DflL;
		vi->SetI=BGBDTC_SlotI_SetI_DeL;
		vi->SetL=BGBDTC_SlotI_SetL_DflL;
#endif
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeL;
		vi->SetF=BGBDTC_SlotI_SetF_DeL;
		vi->SetD=BGBDTC_SlotI_SetD_DeL;
		vi->SetA=BGBDTC_SlotI_SetA_DeL;
		break;

	case 'x':	case 'y':
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DflL;
		vi->GetF=BGBDTC_SlotI_GetF_DeL;
		vi->GetD=BGBDTC_SlotI_GetD_DeL;
		vi->GetA=BGBDTC_SlotI_GetA_DeL;

		vi->SetI=BGBDTC_SlotI_SetI_DeL;
		vi->SetL=BGBDTC_SlotI_SetL_DflL;
		vi->SetF=BGBDTC_SlotI_SetF_DeL;
		vi->SetD=BGBDTC_SlotI_SetD_DeL;
		vi->SetA=BGBDTC_SlotI_SetA_DeL;
		break;

	case 'f':
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DeF;
		vi->GetF=BGBDTC_SlotI_GetF_DflF;
		vi->GetD=BGBDTC_SlotI_GetD_DflF;
		vi->GetA=BGBDTC_SlotI_GetA_DeF;
		vi->SetI=BGBDTC_SlotI_SetI_DeF;
		vi->SetL=BGBDTC_SlotI_SetL_DeF;
		vi->SetF=BGBDTC_SlotI_SetF_DflF;
		vi->SetD=BGBDTC_SlotI_SetD_DeF;
		vi->SetA=BGBDTC_SlotI_SetA_DeF;
		break;
	case 'd':
	case 'e':
		vi->GetI=BGBDTC_SlotI_GetI_DeL;
		vi->GetL=BGBDTC_SlotI_GetL_DeD;
		vi->GetF=BGBDTC_SlotI_GetF_DflD;
		vi->GetD=BGBDTC_SlotI_GetD_DflD;
		vi->GetA=BGBDTC_SlotI_GetA_DeD;
		vi->SetI=BGBDTC_SlotI_SetI_DeD;
		vi->SetL=BGBDTC_SlotI_SetL_DeD;
		vi->SetF=BGBDTC_SlotI_SetF_DeD;
		vi->SetD=BGBDTC_SlotI_SetD_DflD;
		vi->SetA=BGBDTC_SlotI_SetA_DeD;
		break;

	case 'p':	case 'q':	case 'r':
	case 'Q':	case 'L':
		vi->GetI=BGBDTC_SlotI_GetI_DeA;
		vi->GetL=BGBDTC_SlotI_GetL_DeA;
		vi->GetF=BGBDTC_SlotI_GetF_DeA;
		vi->GetD=BGBDTC_SlotI_GetD_DeA;
		vi->GetA=BGBDTC_SlotI_GetA_DflA;

		vi->SetI=BGBDTC_SlotI_SetI_DeA;
		vi->SetL=BGBDTC_SlotI_SetL_DeA;
		vi->SetF=BGBDTC_SlotI_SetF_DeA;
		vi->SetD=BGBDTC_SlotI_SetD_DeA;
		vi->SetA=BGBDTC_SlotI_SetA_DflA;
		break;
	case 'P':
		vi->GetI=BGBDTC_SlotI_GetI_DeA;
		vi->GetL=BGBDTC_SlotI_GetL_DeA;
		vi->GetF=BGBDTC_SlotI_GetF_DeA;
		vi->GetD=BGBDTC_SlotI_GetD_DeA;
		vi->GetA=BGBDTC_SlotI_GetA_DflP;

		vi->SetI=BGBDTC_SlotI_SetI_DeA;
		vi->SetL=BGBDTC_SlotI_SetL_DeA;
		vi->SetF=BGBDTC_SlotI_SetF_DeA;
		vi->SetD=BGBDTC_SlotI_SetD_DeA;
		vi->SetA=BGBDTC_SlotI_SetA_DflP;
		break;
	}

	return(0);
}

int BGBDTC_FinishLayoutClass(
	BGBDTC_ClassInfo *cls)
{
	BGBDTC_SlotInfo *vi;
	int osz, oal, sz, al;
	int i, j, k;

	if(!cls)
		return(0);
	if(cls->clean)
		return(0);
	cls->clean=2;
	
	if(cls->super)
	{
		BGBDTC_FinishLayoutClass(cls->super);
		osz=cls->super->szdata;
		oal=cls->super->aldata;
	}else
	{
		osz=sizeof(void *); oal=osz;
	}
	
	for(i=0; i<cls->nslots; i++)
	{
		vi=cls->slots[i];
		BGBDTC_GetSigSizeAlign(vi->sig, &sz, &al);
		if(sz<0)continue;
		
		osz=(osz+(al-1))&(~(al-1));
		vi->offs=osz;
		
		osz=osz+sz;
		if(al>oal)oal=al;
		
		BGBDTC_SlotSetupVtGetSet(cls, vi);
	}

	osz=(osz+(oal-1))&(~(oal-1));
	cls->szdata=osz;
	cls->aldata=oal;
	cls->clean=1;
	return(1);
}

int BGBDTC_BeginLayoutClass(
	BGBDTC_ClassInfo *cls)
{
	cls->clean=0;
	return(0);
}

int BGBDTC_EndLayoutClass(
	BGBDTC_ClassInfo *cls)
{
	BGBDTC_FinishLayoutClass(cls);
	return(0);
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

dtcObject BGBDTC_AllocClassInstance(
	BGBDTC_ClassInfo *cls)
{
	void *ptr;
	
	ptr=dtmAlloc("bgbdtc_object_t", cls->szdata);
	*(void **)ptr=cls;
	return(ptr);
}
