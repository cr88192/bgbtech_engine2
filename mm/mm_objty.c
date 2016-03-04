#include <bteifgl.h>

BGBDT_MM_ObjTypeInfo bgbdt_mm_objtype[4096];
BGBDT_MM_ObjLLnInfo bgbdt_mm_objlln[4096];

int BGBDT_MM_QHashName(char *name)
{
	char *s;
	int h;
	
	if(!name)
		return(0);
	
	s=name; h=0;
	while(*s)
		h=h*4093+(*s++)+1;
	return(h);
}

int BGBDT_MM_LookupIndexObjTypeName(char *name)
{
	BGBDT_MM_ObjTypeInfo *oti;
	int ch;
	int i, j, k;
	
	ch=BGBDT_MM_QHashName(name);
	ch=ch*4093+1;
	for(i=0; i<64; i++)
	{
		j=(ch>>12)&255;
		oti=&(bgbdt_mm_objtype[j]);
		if(!oti->name)
			return(-1);
		if(!strcmp(oti->name, name))
			return(i);
		ch=ch*4093+1;
	}

	for(i=0; i<4096; i++)
	{
		oti=&(bgbdt_mm_objtype[i]);
		if(!oti->name)
			return(-1);
		if(!strcmp(oti->name, name))
			return(i);
	}
	return(-1);
}

int BGBDT_MM_GetIndexObjTypeName(char *name)
{
	BGBDT_MM_ObjTypeInfo *oti;
	int ch;
	int i, j, k;
	
	ch=BGBDT_MM_QHashName(name);
	ch=ch*4093+1;
	for(i=0; i<64; i++)
	{
		j=(ch>>12)&255;
		oti=&(bgbdt_mm_objtype[j]);
		if(!oti->name)
			break;
		if(!strcmp(oti->name, name))
			return(j);
		ch=ch*4093+1;
	}
	
	if(i<64)
	{
		oti->name=frgl_strdup(name);
		oti->tyid=j;
		return(j);
	}

	for(i=0; i<4096; i++)
	{
		oti=&(bgbdt_mm_objtype[i]);
		if(!oti->name)
			break;
		if(!strcmp(oti->name, name))
			return(i);
	}

	if(i<4096)
	{
		oti->name=frgl_strdup(name);
		oti->tyid=i;
		return(i);
	}

	return(-1);
}

char *BGBDT_MM_GetObjTypeNameForIndex(int idx)
{
	BGBDT_MM_ObjTypeInfo *oti;
	oti=&(bgbdt_mm_objtype[idx&4095]);
	return(oti->name);
}

int BGBDT_MM_GetIndexObjLLn(char *fn, int ln)
{
	BGBDT_MM_ObjLLnInfo *oti;
	int ch;
	int i, j, k;
	
	ch=BGBDT_MM_QHashName(fn);
	ch=ch*4093+ln+1;
	ch=ch*4093+1;
	for(i=0; i<64; i++)
	{
		j=(ch>>12)&255;
		oti=&(bgbdt_mm_objlln[j]);
		if(!oti->fn)
			break;
		if(!strcmp(oti->fn, fn) || (oti->ln==ln))
			return(j);
		ch=ch*4093+1;
	}
	
	if(i<64)
	{
		oti->fn=frgl_strdup(fn);
		oti->ln=ln;
		return(j);
	}

	for(i=0; i<4096; i++)
	{
		oti=&(bgbdt_mm_objlln[i]);
		if(!oti->fn)
			break;
		if(!strcmp(oti->fn, fn) || (oti->ln==ln))
			return(i);
	}

	if(i<4096)
	{
		oti->fn=frgl_strdup(fn);
		oti->ln=ln;
		return(i);
	}

	return(-1);
}

int BGBDT_MM_QHashBytes(byte *buf, int sz)
{
	byte *cs, *cse;
	int h;
	
	cs=buf; cse=buf+sz; h=0;
	while((cs+8)<=cse)
	{
		h=(h*31)+(*cs++)+1;	h=(h*31)+(*cs++)+1;
		h=(h*31)+(*cs++)+1;	h=(h*31)+(*cs++)+1;
		h=(h*31)+(*cs++)+1;	h=(h*31)+(*cs++)+1;
		h=(h*31)+(*cs++)+1;	h=(h*31)+(*cs++)+1;
	}

	while(cs<cse)
		{ h=(h*31)+(*cs++)+1; }
	return(h);
}

byte *BGBDT_MM_AllocObject(int tyid, int size, int lln)
{
	BGBDT_MM_ObjHead *objh;
	int h;
	
	objh=(BGBDT_MM_ObjHead *)BGBDT_MM_AllocObjectInner(size);
	objh->ty=tyid;
	objh->lln=lln&4095;
	objh->pad1=0;
	objh->pad2=0;
	
	h=BGBDT_MM_QHashBytes((byte *)objh, sizeof(BGBDT_MM_ObjHead));
	objh->lln|=(~h)<<12;
	
	return((byte *)(objh+1));
}

void *BGBDT_MM_AllocLLn(char *type, int size, char *fn, int ln)
{
	void *ptr;
	int tid, lid;
	
	tid=BGBDT_MM_GetIndexObjTypeName(type);
	lid=BGBDT_MM_GetIndexObjLLn(fn, ln);
	ptr=BGBDT_MM_AllocObject(tid, size, lid);
}

void BGBDT_MM_FreeAny(void *ptr)
{
	void *bptr;
	
	bptr=BGBDT_MM_GetObjectPtrBase(ptr);
	if(bptr)
		BGBDT_MM_FreeObjectInner(bptr);
}

void BGBDT_MM_Free(void *ptr)
{
	void *bptr;

	if(!ptr)
		return;
	
	bptr=((byte *)ptr)-sizeof(BGBDT_MM_ObjHead);
	BGBDT_MM_FreeObjectInner(bptr);
}

dtVal BGBDT_MM_DtvWrapObjPtrF(void *ptr)
{
	BGBDT_MM_ObjHead *objh;
	void *bptr;
	int tyid;

	if(!ptr)
		return(DTV_NULL);
	
	bptr=((byte *)ptr)-sizeof(BGBDT_MM_ObjHead);
	objh=bptr;
	tyid=objh->ty&4095;
//	BGBDT_MM_FreeObjectInner(bptr);
	return(dtvWrapTagPtrF(ptr, tyid));
}

dtVal BGBDT_MM_DtvWrapObjPtr(void *ptr)
{
	BGBDT_MM_ObjHead *objh;
	void *bptr;
	int tyid;

	if(!ptr)
		return(DTV_NULL);
	
	bptr=BGBDT_MM_GetObjectPtrBase(ptr);
	if(!bptr)
	{
		return(dtvWrapPtrF(ptr));
//		return(DTV_UNDEFINED);
	}

	objh=bptr;
	tyid=objh->ty&4095;
//	BGBDT_MM_FreeObjectInner(bptr);
	return(dtvWrapTagPtrF(ptr, tyid));
}
