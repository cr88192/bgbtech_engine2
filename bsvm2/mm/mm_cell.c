/*
 * Good old cell alloctor.
 */

// #include <bteifgl.h>


BGBDT_MM_ChunkInfo bgbdt_mm_sobjchk[1024];
BGBDT_MM_ChunkInfo bgbdt_mm_mobjchk[1024];
int bgbdt_mm_numsobjchk;
int bgbdt_mm_nummobjchk;

int bgbdt_mm_rovsobjchk;
int bgbdt_mm_rovmobjchk;

byte *bgbdt_mm_freesobj[1024];
byte *bgbdt_mm_freemobj[4096];


int BGBDT_MM_AllocSObjChk()
{
	BGBDT_MM_ChunkInfo *chk;
	int i;

	i=bgbdt_mm_numsobjchk++;
	chk=&(bgbdt_mm_sobjchk[i]);
	chk->data=malloc((1<<BGBDT_MM_SOBJ_TOTSHL)+
		(1<<BGBDT_MM_SOBJ_CHKSHL));
	chk->edata=chk->data+(1<<BGBDT_MM_SOBJ_TOTSHL);
	chk->map=chk->data+(1<<BGBDT_MM_SOBJ_TOTSHL);
	chk->ncell=1<<BGBDT_MM_SOBJ_CHKSHL;
	chk->shcell=BGBDT_MM_SOBJ_CELSHL;
	memset(chk->data, 0, (1<<BGBDT_MM_SOBJ_TOTSHL)+
		(1<<BGBDT_MM_SOBJ_CHKSHL));
	chk->magic1=BGBDT_MM_CHKMAG1;
	chk->magic2=BGBDT_MM_CHKMAG2;
	
	printf("BGBDT_MM_AllocSObjChk: %d %p..%p\n",
		i, chk->data, chk->edata);
	
	return(i);
}

int BGBDT_MM_AllocMObjChk()
{
	BGBDT_MM_ChunkInfo *chk;
	int i;

	i=bgbdt_mm_nummobjchk++;
	chk=&(bgbdt_mm_mobjchk[i]);
	chk->data=malloc((1<<BGBDT_MM_MOBJ_TOTSHL)+
		(1<<BGBDT_MM_MOBJ_CHKSHL));
	chk->edata=chk->data+(1<<BGBDT_MM_MOBJ_TOTSHL);
	chk->map=chk->data+(1<<BGBDT_MM_MOBJ_TOTSHL);
	chk->ncell=1<<BGBDT_MM_MOBJ_CHKSHL;
	chk->shcell=BGBDT_MM_MOBJ_CELSHL;
	memset(chk->data, 0, (1<<BGBDT_MM_MOBJ_TOTSHL)+
		(1<<BGBDT_MM_MOBJ_CHKSHL));
	chk->magic1=BGBDT_MM_CHKMAG1;
	chk->magic2=BGBDT_MM_CHKMAG2;

	printf("BGBDT_MM_AllocMObjChk: %d %p..%p\n",
		i, chk->data, chk->edata);

	return(i);
}


int BGBDT_FindFreeCellsChk(BGBDT_MM_ChunkInfo *chk, int cells)
{
	int i, j, k, l;

	if(!chk)
		return(-1);
	if(cells<=0)
		return(-1);
	
	if((chk->magic1!=BGBDT_MM_CHKMAG1) ||
		(chk->magic2!=BGBDT_MM_CHKMAG2))
			{ BSVM2_DBGTRAP }
	
	i=chk->rov;
	while(i<chk->ncell)
	{
		if(chk->map[i])
		{
			l=chk->ncell;
			while(i<l)
			{
				if(!chk->map[i])
					break;
				i++;
			}
			continue;
		}
		
		j=i;
		l=i+cells;
		if(l>chk->ncell)
			l=chk->ncell;
		while(j<l)
		{
			if(chk->map[j])
				break;
			j++;
		}
		if((j-i)>=cells)
		{
//			chk->rov=j;
			return(i);
		}
		i=j;
	}
	return(-1);
}

int BGBDT_MM_AllocCellsChk(BGBDT_MM_ChunkInfo *chk, int cells)
{
	int i, j, k, l;

	if((chk->magic1!=BGBDT_MM_CHKMAG1) ||
		(chk->magic2!=BGBDT_MM_CHKMAG2))
			{ BSVM2_DBGTRAP }

	i=BGBDT_FindFreeCellsChk(chk, cells);
	if(i<0)return(i);
	
	l=i+cells; j=i;
	chk->map[j++]=1;
	while(j<l)
		{ chk->map[j++]=2; }
	chk->rov=j;
	return(i);
}

int BGBDT_FreeCellsChk(BGBDT_MM_ChunkInfo *chk, int idx, int cells)
{
	int i, j, k, l;

	if((chk->magic1!=BGBDT_MM_CHKMAG1) ||
		(chk->magic2!=BGBDT_MM_CHKMAG2))
			{ BSVM2_DBGTRAP }

//	i=BGBDT_FindFreeCellsChk(chk, cells);
//	if(i<0)return(i);
	
	if((idx<0) || (idx>=chk->ncell))
		return(-1);
	
	if(cells>0)
	{
		l=idx+cells;
		if((l<0) || (l>chk->ncell))
			return(-1);
	}else
	{
		l=chk->ncell;
	}
	
	i=idx;
	if((chk->map[i]&3)!=1)
		return(-1);
	chk->map[i++]=0;
	while((i<l) && ((chk->map[i]&3)==2))
		chk->map[i++]=0;
	return(i);
}

int BGBDT_MM_LookupSObjChunkPtr(byte *ptr)
{
	BGBDT_MM_ChunkInfo *chk;
	int i, j, k, l;

	l=bgbdt_mm_numsobjchk;
	for(i=0; i<l; i++)
	{
		chk=&(bgbdt_mm_sobjchk[i]);
		if((ptr>=chk->data) && (ptr<chk->edata))
			return(i);
	}
	return(-1);
}

int BGBDT_MM_LookupMObjChunkPtr(byte *ptr)
{
	BGBDT_MM_ChunkInfo *chk;
	int i, j, k, l;

	l=bgbdt_mm_nummobjchk;
	for(i=0; i<l; i++)
	{
		chk=&(bgbdt_mm_mobjchk[i]);
		if((ptr>=chk->data) && (ptr<chk->edata))
			return(i);
	}
	return(-1);
}

int BGBDT_MM_LookupSObjChunkCellPtr(byte *ptr, int *rcell)
{
	BGBDT_MM_ChunkInfo *chk;
	int i, j, k, l;

	l=bgbdt_mm_numsobjchk;
	for(i=0; i<l; i++)
	{
		chk=&(bgbdt_mm_sobjchk[i]);
		if((ptr>=chk->data) && (ptr<chk->edata))
		{
//			j=(ptr-chk->data)>>chk->shcell;
			j=(ptr-chk->data)>>BGBDT_MM_SOBJ_CELSHL;
//			if(*rcell)
				*rcell=j;
			return(i);
		}
	}
	return(-1);
}

int BGBDT_MM_LookupMObjChunkCellPtr(byte *ptr, int *rcell)
{
	BGBDT_MM_ChunkInfo *chk;
	int i, j, k, l;

	l=bgbdt_mm_nummobjchk;
	for(i=0; i<l; i++)
	{
		chk=&(bgbdt_mm_mobjchk[i]);
		if((ptr>=chk->data) && (ptr<chk->edata))
		{
//			j=(ptr-chk->data)>>chk->shcell;
			j=(ptr-chk->data)>>BGBDT_MM_MOBJ_CELSHL;
//			if(*rcell)
				*rcell=j;
			return(i);
		}
	}
	return(-1);
}

int BGBDT_MM_LookupSObjChunkCellBasePtr(byte *ptr, int *rcell)
{
	BGBDT_MM_ChunkInfo *chk;
	byte *ptr2;
	int chi, cli;
	int i;

	chi=BGBDT_MM_LookupSObjChunkCellPtr(ptr, &cli);
	if(chi<0)return(-1);

	chk=&(bgbdt_mm_sobjchk[chi]);
	i=cli;
	while((i>0) && ((chk->map[i]&3)==2))
		i--;
	if((chk->map[i]&3)!=1)
		return(-1);
	*rcell=i;
	return(chi);
}

int BGBDT_MM_LookupMObjChunkCellBasePtr(byte *ptr, int *rcell)
{
	BGBDT_MM_ChunkInfo *chk;
	byte *ptr2;
	int chi, cli;
	int i;

	chi=BGBDT_MM_LookupMObjChunkCellPtr(ptr, &cli);
	if(chi<0)return(-1);

	chk=&(bgbdt_mm_mobjchk[chi]);
	i=cli;
	while((i>0) && ((chk->map[i]&3)==2))
		i--;
	if((chk->map[i]&3)!=1)
		return(-1);
	*rcell=i;
	return(chi);
}

byte *BGBDT_MM_LookupSObjPtrBase(byte *ptr)
{
	BGBDT_MM_ChunkInfo *chk;
	byte *ptr2;
	int chi, cli;
	int i;

	chi=BGBDT_MM_LookupSObjChunkCellPtr(ptr, &cli);
	if(chi<0)return(NULL);

	chk=&(bgbdt_mm_sobjchk[chi]);
	i=cli;
	while((i>0) && ((chk->map[i]&3)==2))
		i--;
	if((chk->map[i]&3)!=1)
		return(NULL);
	ptr2=chk->data+(i<<BGBDT_MM_SOBJ_CELSHL);
	return(ptr2);
}

byte *BGBDT_MM_LookupMObjPtrBase(byte *ptr)
{
	BGBDT_MM_ChunkInfo *chk;
	byte *ptr2;
	int chi, cli;
	int i;

	chi=BGBDT_MM_LookupMObjChunkCellPtr(ptr, &cli);
	if(chi<0)return(NULL);

	chk=&(bgbdt_mm_mobjchk[chi]);
	i=cli;
	while((i>0) && ((chk->map[i]&3)==2))
		i--;
	if((chk->map[i]&3)!=1)
		return(NULL);
	ptr2=chk->data+(i<<BGBDT_MM_MOBJ_CELSHL);
	return(ptr2);
}

void BGBDT_MM_FreeSObjPtr(byte *ptr)
{
	BGBDT_MM_ChunkInfo *chk;
	byte *ptr2;
	int chi, cli;
	int i;

	chi=BGBDT_MM_LookupSObjChunkCellPtr(ptr, &cli);
	if(chi<0)return;

	chk=&(bgbdt_mm_sobjchk[chi]);
	i=cli;
	while((i>0) && ((chk->map[i]&3)==2))
		i--;
	if((chk->map[i]&3)!=1)
		return;
	BGBDT_FreeCellsChk(chk, cli, 0);
}

void BGBDT_MM_FreeMObjPtr(byte *ptr)
{
	BGBDT_MM_ChunkInfo *chk;
	byte *ptr2;
	int chi, cli;
	int i;

	chi=BGBDT_MM_LookupMObjChunkCellPtr(ptr, &cli);
	if(chi<0)return;

	chk=&(bgbdt_mm_mobjchk[chi]);
	i=cli;
	while((i>0) && ((chk->map[i]&3)==2))
		i--;
	if((chk->map[i]&3)!=1)
		return;
	BGBDT_FreeCellsChk(chk, cli, 0);
}

byte *BGBDT_MM_AllocSObjCells(int cells)
{
	BGBDT_MM_ChunkInfo *chk;
	byte *ptr;
	int i, j, k, l;
	
	i=bgbdt_mm_rovsobjchk; l=bgbdt_mm_numsobjchk;
	while(i<l)
	{
		chk=&(bgbdt_mm_sobjchk[i]);
		j=BGBDT_MM_AllocCellsChk(chk, cells);
		if(j>=0)
		{
			bgbdt_mm_rovsobjchk=i;
			ptr=chk->data+(j<<chk->shcell);
			return(ptr);
		}
		i++;
	}

	i=0; l=bgbdt_mm_numsobjchk;
	while(i<l)
	{
		chk=&(bgbdt_mm_sobjchk[i]);
		chk->rov=0;
		
		j=BGBDT_MM_AllocCellsChk(chk, cells);
		if(j>=0)
		{
			bgbdt_mm_rovsobjchk=i;
			ptr=chk->data+(j<<chk->shcell);
			return(ptr);
		}
		i++;
	}

	i=BGBDT_MM_AllocSObjChk();
	if(i>=0)
	{
		chk=&(bgbdt_mm_sobjchk[i]);
		j=BGBDT_MM_AllocCellsChk(chk, cells);
		if(j>=0)
		{
			bgbdt_mm_rovsobjchk=i;
			ptr=chk->data+(j<<chk->shcell);
			return(ptr);
		}
	}
	
	return(NULL);
}

byte *BGBDT_MM_AllocMObjCells(int cells)
{
	BGBDT_MM_ChunkInfo *chk;
	byte *ptr;
	int i, j, k, l;
	
	i=bgbdt_mm_rovmobjchk; l=bgbdt_mm_nummobjchk;
	while(i<l)
	{
		chk=&(bgbdt_mm_mobjchk[i]);
		j=BGBDT_MM_AllocCellsChk(chk, cells);
		if(j>=0)
		{
			bgbdt_mm_rovmobjchk=i;
			ptr=chk->data+(j<<chk->shcell);
			return(ptr);
		}
		i++;
	}

	i=0; l=bgbdt_mm_nummobjchk;
	while(i<l)
	{
		chk=&(bgbdt_mm_mobjchk[i]);
		chk->rov=0;
		
		j=BGBDT_MM_AllocCellsChk(chk, cells);
		if(j>=0)
		{
			bgbdt_mm_rovmobjchk=i;
			ptr=chk->data+(j<<chk->shcell);
			return(ptr);
		}
		i++;
	}

	i=BGBDT_MM_AllocMObjChk();
	if(i>=0)
	{
		chk=&(bgbdt_mm_mobjchk[i]);
		j=BGBDT_MM_AllocCellsChk(chk, cells);
		if(j>=0)
		{
			bgbdt_mm_rovmobjchk=i;
			ptr=chk->data+(j<<chk->shcell);
			return(ptr);
		}
	}
	
	return(NULL);
}

byte *BGBDT_MM_AllocObjectInner(int size)
{
	byte *ptr;
	int nc, sz;
	
	sz=size+16;
	
	if((sz+BGBDT_MM_SOBJ_CELMSK)<=BGBDT_MM_SOBJ_OBJLIM)
	{
		nc=(sz+BGBDT_MM_SOBJ_CELMSK)>>BGBDT_MM_SOBJ_CELSHL;
		
		ptr=bgbdt_mm_freesobj[nc];
		if(ptr)
		{
			bgbdt_mm_freesobj[nc]=*(byte **)ptr;
			((BGBDT_MM_ObjHead *)(ptr))->sz=sz;
			return(ptr);
		}
		
		ptr=BGBDT_MM_AllocSObjCells(nc);
		((BGBDT_MM_ObjHead *)(ptr))->sz=sz;
		return(ptr);
	}

	if((sz+BGBDT_MM_MOBJ_CELMSK)<=BGBDT_MM_MOBJ_OBJLIM)
	{
		nc=(sz+BGBDT_MM_MOBJ_CELMSK)>>BGBDT_MM_MOBJ_CELSHL;

		ptr=bgbdt_mm_freemobj[nc];
		if(ptr)
		{
			bgbdt_mm_freemobj[nc]=*(byte **)ptr;
			((BGBDT_MM_ObjHead *)(ptr))->sz=sz;
			return(ptr);
		}

		ptr=BGBDT_MM_AllocMObjCells(nc);
		((BGBDT_MM_ObjHead *)(ptr))->sz=sz;
		return(ptr);
	}
	
	ptr=BGBDT_MM_AllocLObjPtr(sz);
	return(ptr);
}

void BGBDT_MM_FreeObjectInner(byte *ptr)
{
	BGBDT_MM_ObjHead *oh;
	int sz, nc;
	
	oh=(BGBDT_MM_ObjHead *)ptr;
	sz=oh->sz;
	
	if(*(u32 *)ptr==BGBDT_MM_CHKMAG1)
		{ BSVM2_DBGTRAP }
	
	if((sz+BGBDT_MM_SOBJ_CELMSK)<=BGBDT_MM_SOBJ_OBJLIM)
	{
		nc=(sz+BGBDT_MM_SOBJ_CELMSK)>>BGBDT_MM_SOBJ_CELSHL;
		if(nc>>10)
			{ BSVM2_DBGTRAP }
		*(byte **)ptr=bgbdt_mm_freesobj[nc];
		bgbdt_mm_freesobj[nc]=ptr;
		return;
	}

	if((sz+BGBDT_MM_MOBJ_CELMSK)<=BGBDT_MM_MOBJ_OBJLIM)
	{
		nc=(sz+BGBDT_MM_MOBJ_CELMSK)>>BGBDT_MM_MOBJ_CELSHL;
		if(nc>>12)
			{ BSVM2_DBGTRAP }

		*(byte **)ptr=bgbdt_mm_freemobj[nc];
		bgbdt_mm_freemobj[nc]=ptr;
		return;
	}

	BGBDT_MM_FreeLObjPtr(ptr);
}

byte *BGBDT_MM_GetObjectPtrBase(byte *ptr)
{
	byte *bptr;

	bptr=BGBDT_MM_LookupSObjPtrBase(ptr);
	if(bptr)return(bptr);
	bptr=BGBDT_MM_LookupMObjPtrBase(ptr);
	if(bptr)return(bptr);
	bptr=BGBDT_MM_LookupLObjBase(ptr);
	return(bptr);
}

/*
 * ObjID:
 *   Low 2 bits give type tag.
 * 0,2: Small Object
 *   Bits 1..20=Cell Index (1M), 21..31=Chunk Index (2048)
 * 1  : Medium Object
 *   Bits 2..18=Cell Index (64k), 18..31=Chunk Index (16k)
 * 3  : Large Object
 *   Bits 2..31: LObj Number
 */

BS2VM_API void *BGBDT_MM_GetPtrForObjId(int objid)
{
	byte *ptr;
	int t, ch, ci;

	switch(objid&3)
	{
	case 0: case 2:
		ci=(objid>>1)&((1<<BGBDT_MM_SOBJ_CHKSHL)-1);
		ch=((u32)objid)>>(BGBDT_MM_SOBJ_CHKSHL+1);
		ptr=bgbdt_mm_sobjchk[ch].data+
			(ci<<BGBDT_MM_SOBJ_CELSHL);
		return(ptr);
	case 1:
		ci=(objid>>2)&((1<<BGBDT_MM_MOBJ_CHKSHL)-1);
		ch=((u32)objid)>>(BGBDT_MM_MOBJ_CHKSHL+2);
		ptr=bgbdt_mm_mobjchk[ch].data+
			(ci<<BGBDT_MM_MOBJ_CELSHL);
		return(ptr);
	case 3:
		ci=objid>>2;
		ptr=BGBDT_MM_GetLObjPtrForIndex(ci);
		return(ptr);
	}
	
	return(NULL);
	
#if 0
	t=objid&3;
	if((t==0) || (t==2))
	{
		ci=(objid>>1)&((1<<20)-1);
		ch=((u32)objid)>>21;
		ptr=bgbdt_mm_sobjchk[ch].data+(ci<<4);
		return(ptr);
	}

	if(t==1)
	{
		ci=(objid>>2)&((1<<16)-1);
		ch=((u32)objid)>>18;
		ptr=bgbdt_mm_mobjchk[ch].data+(ci<<8);
		return(ptr);
	}

	if(t==3)
	{
		ci=objid>>2;
		ptr=BGBDT_MM_GetLObjPtrForIndex(ci);
		return(ptr);
	}
	
	return(NULL);
#endif
}

BS2VM_API int BGBDT_MM_GetObjIdForPtr(void *ptr)
{
	int chi, cli;
	int i;

	chi=BGBDT_MM_LookupSObjChunkCellBasePtr(ptr, &cli);
	if(chi>=0)
	{
		i=(chi<<(BGBDT_MM_SOBJ_CHKSHL+1))|(cli<<1);
		return(i);
	}

	chi=BGBDT_MM_LookupMObjChunkCellBasePtr(ptr, &cli);
	if(chi>=0)
	{
		i=(chi<<(BGBDT_MM_MOBJ_CHKSHL+2))|(cli<<2)|1;
		return(i);
	}
	
	chi=BGBDT_MM_LookupLObj(ptr);
	if(chi>=0)
	{
		i=(chi<<2)|3;
		return(i);
	}
	
	return(3);
}

BS2VM_API void *BGBDT_MM_GetDataPtrForObjId(int objid)
{
	byte *ptr;
	int t, ch, ci;
	
	switch(objid&3)
	{
	case 0: case 2:
		ci=(objid>>1)&((1<<BGBDT_MM_SOBJ_CHKSHL)-1);
		ch=((u32)objid)>>(BGBDT_MM_SOBJ_CHKSHL+1);
		ptr=bgbdt_mm_sobjchk[ch].data+
			(ci<<BGBDT_MM_SOBJ_CELSHL);
		return(ptr+sizeof(BGBDT_MM_ObjHead));
	case 1:
		ci=(objid>>2)&((1<<BGBDT_MM_MOBJ_CHKSHL)-1);
		ch=((u32)objid)>>(BGBDT_MM_MOBJ_CHKSHL+2);
		ptr=bgbdt_mm_mobjchk[ch].data+
			(ci<<BGBDT_MM_MOBJ_CELSHL);
		return(ptr+sizeof(BGBDT_MM_ObjHead));
	case 3:
		ci=objid>>2;
		if(!ci)return(NULL);
		ptr=BGBDT_MM_GetLObjPtrForIndex(ci);
		return(ptr+sizeof(BGBDT_MM_ObjHead));
	}
	
	return(NULL);

//	ptr=BGBDT_MM_GetPtrForObjId(objid);
//	if(!ptr)return(NULL);
//	return(ptr+sizeof(BGBDT_MM_ObjHead));
}
