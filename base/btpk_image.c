#include <bteifgl.h>

BtPak0_Image *btpak_image=NULL;

#define btpak_malloc(sz)	frgl_malloc(sz)
#define btpak_free(ptr)		frgl_free(ptr)

u16 bkpak_swdu16(btpk_u16 v)
{
#if defined(X86) || defined(X86_64)
	return(*(u16 *)(v.v));
#else
	return(v.v[0]|(v.v[1]<<8));
#endif
}

u32 bkpak_swdu32(btpk_u32 v)
{
#if defined(X86) || defined(X86_64)
	return(*(u32 *)(v.v));
#else
	return(v.v[0]|(v.v[1]<<8)|(v.v[2]<<16)|(v.v[3]<<24));
#endif
}

u64 bkpak_swdu64(btpk_u64 v)
{
#if defined(X86) || defined(X86_64)
	return(*(u64 *)(v.v));
#else
	u32 c0, c1;
	c0=v.v[0]|(v.v[1]<<8)|(v.v[2]<<16)|(v.v[3]<<24);
	c1=v.v[4]|(v.v[5]<<8)|(v.v[6]<<16)|(v.v[7]<<24);
	return(c0|(((u64)c1)<<32));
#endif
}

btpk_u16 bkpak_sweu16(u16 v)
{
	btpk_u16 c;
#if defined(X86) || defined(X86_64)
	*((u16 *)c.v)=v;
#else
	c.v[0]=v;		c.v[1]=v>> 8;
#endif
	return(c);
}

btpk_u32 bkpak_sweu32(u32 v)
{
	btpk_u32 c;
#if defined(X86) || defined(X86_64)
	*((u32 *)c.v)=v;
#else
	c.v[0]=v;		c.v[1]=v>> 8;
	c.v[2]=v>>16;	c.v[3]=v>>24;
#endif
	return(c);
}

btpk_u64 bkpak_sweu64(u64 v)
{
	btpk_u64 c;
#if defined(X86) || defined(X86_64)
	*((u64 *)c.v)=v;
#else
	c.v[0]=v;		c.v[1]=v>> 8;
	c.v[2]=v>>16;	c.v[3]=v>>24;
	c.v[4]=v>>32;	c.v[5]=v>>40;
	c.v[6]=v>>48;	c.v[7]=v>>56;
#endif
	return(c);
}


BTEIFGL_API BtPak0_Image *BtPak_LookupImage(char *name)
{
	BtPak0_Image *cur;

	cur=btpak_image;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTEIFGL_API BtPak0_Image *BtPak_LoadImageRO(char *name)
{
	BtPak0_Image *cur;
	FILE *fd;

	cur=BtPak_LookupImage(name);
	if(cur)
		return(cur);
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	
	cur=btpak_malloc(sizeof(BtPak0_Image));
	memset(cur, 0, sizeof(BtPak0_Image));
	cur->name=frgl_strdup(name);
	
	cur->fd=fd;
	fread(&cur->head, 1, sizeof(BtPak0_Header), fd);
	
	cur->ofs_chunks=bkpak_swdu64(cur->head.ofsChkInfo);
	cur->n_chunks=bkpak_swdu32(cur->head.nChunks);
	cur->m_chunks=cur->n_chunks;

	cur->chki=btpak_malloc(
		cur->n_chunks*sizeof(BtPak0_ChunkInfo));
	fseek(cur->fd, cur->ofs_chunks, 0);
	fread(cur->chki,
		sizeof(BtPak0_ChunkInfo), cur->n_chunks,
		cur->fd);
	
	cur->lcDir=BtPak_ImageLoadChunk(cur,
		bkpak_swdu32(cur->head.dirChkId));
	cur->n_dirent=cur->lcDir->szData/sizeof(BtPak0_DirEnt);
	cur->m_dirent=cur->n_dirent;

	cur->next=btpak_image;
	btpak_image=cur;
	
	return(cur);
}

BTEIFGL_API BtPak0_Image *BtPak_LoadImageRW(char *name)
{
	BtPak0_Image *cur;
	FILE *fd;

	cur=BtPak_LookupImage(name);
	if(cur)
		return(cur);
	
	fd=fopen(name, "r+b");
	if(!fd)
	{
		cur=BtPak_CreateImageRW(name);
		return(cur);
	}
	
	cur=btpak_malloc(sizeof(BtPak0_Image));
	memset(cur, 0, sizeof(BtPak0_Image));
	cur->name=frgl_strdup(name);
	
	cur->fd=fd;
	fread(&cur->head, 1, sizeof(BtPak0_Header), fd);
	
	cur->ofs_chunks=bkpak_swdu64(cur->head.ofsChkInfo);
	cur->n_chunks=bkpak_swdu32(cur->head.nChunks);
	cur->m_chunks=cur->n_chunks;

	cur->chki=btpak_malloc(
		cur->n_chunks*sizeof(BtPak0_ChunkInfo));
	fseek(cur->fd, cur->ofs_chunks, 0);
	fread(cur->chki,
		sizeof(BtPak0_ChunkInfo), cur->n_chunks,
		cur->fd);
	
	cur->lcDir=BtPak_ImageLoadChunk(cur,
		bkpak_swdu32(cur->head.dirChkId));
	cur->n_dirent=cur->lcDir->szData/sizeof(BtPak0_DirEnt);
	cur->m_dirent=cur->n_dirent;

	cur->lcBlkm=BtPak_ImageLoadChunk(cur,
		bkpak_swdu32(cur->head.blkmChkId));
	cur->blkm=cur->lcBlkm->data;
	cur->n_blocks=cur->lcBlkm->szData*4;
	cur->m_blocks=cur->lcBlkm->szData*4;
	
	cur->next=btpak_image;
	btpak_image=cur;
	
	return(cur);
}

BTEIFGL_API BtPak0_Image *BtPak_CreateImageRW(char *name)
{
	BtPak0_Image *cur;
	FILE *fd;

	cur=BtPak_LookupImage(name);
	if(cur)
		return(cur);
	
	fd=fopen(name, "wb");
	if(!fd)
		return(NULL);
	
	cur=btpak_malloc(sizeof(BtPak0_Image));
	memset(cur, 0, sizeof(BtPak0_Image));
	cur->name=frgl_strdup(name);
	
	cur->fd=fd;
//	fread(&cur->head, 1, sizeof(BtPak0_Header), fd);
	
	memcpy(&cur->head.magic, "BT2PACK0", 8);
		
	cur->n_chunks=0;
	cur->m_chunks=256;
	cur->chki=btpak_malloc(256*sizeof(BtPak0_ChunkInfo));
	
	cur->lcDir=BtPak_ImageNewLoadChunk(cur, 1<<16);
	cur->n_dirent=0;
	cur->n_dirent=1<<(16-6);

	cur->lcBlkm=BtPak_ImageNewLoadChunk(cur, 1<<16);
	cur->blkm=cur->lcBlkm->data;
	
	cur->blkm[0]=0x0F;
	
	cur->n_blocks=2;
	cur->m_blocks=1<<(16+2);
	
	cur->head.dirChkId=bkpak_sweu32(cur->lcDir->chkId);
	cur->head.blkmChkId=bkpak_sweu32(cur->lcBlkm->chkId);
	
	cur->next=btpak_image;
	btpak_image=cur;
	
	return(cur);
}

BTEIFGL_API int BtPak_ImageCloseImage(BtPak0_Image *img)
{
	BtPak_ImageCommit(img);
	return(0);
}

BtPak0_LoadChunk *BtPak_ImageLookupChunk(BtPak0_Image *img, int idx)
{
	BtPak0_LoadChunk *cur;
	cur=img->lchk;
	while(cur)
	{
		if(cur->chkId==idx)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BtPak0_LoadChunk *BtPak_ImageLoadChunk(BtPak0_Image *img, int idx)
{
	BtPak0_ChunkInfo *chi;
	BtPak0_LoadChunk *cur;
	u64 ofs;
	u32 sz;

	if(idx<=0)
		return(NULL);

	cur=BtPak_ImageLookupChunk(img, idx);
	if(cur)
		return(cur);
	
	cur=btpak_malloc(sizeof(BtPak0_LoadChunk));
	cur->chkId=idx;
	
	chi=img->chki+(idx-1);
	cur->cm=chi->cm;
	cur->cfl=chi->cfl;
	cur->shOsz=chi->shOsz;
	cur->shCell=chi->shCell;
	cur->ctid=bkpak_swdu16(chi->ctId);

	if(chi->cm==0)
	{
		sz=bkpak_swdu32(chi->szChk);
		ofs=bkpak_swdu64(chi->ofsChk);
		cur->data=btpak_malloc(sz);
		fseek(img->fd, ofs, 0);
		fread(cur->data, 1, sz, img->fd);
		cur->szData=sz;
	}

	cur->next=img->lchk;
	img->lchk=cur;
	return(cur);
}

static int btpak_getcbm(byte *bm, int ix)
	{ return((bm[ix>>2]>>((ix&3)<<1))&3); }
static void btpak_setcbm(byte *bm, int ix, int v)
{
	bm[ix>>2]&=~(3<<((ix&3)<<1));
	bm[ix>>2]|=(v&3)<<((ix&3)<<1);
}

int BtPak_ImageFindFreeBlocks(BtPak0_Image *img, int nblk)
{
	byte *bm;
	int ix, ix1;
	int i, j, k;
	
	ix=img->blkrov; bm=img->blkm;
	while(ix<img->n_blocks)
	{
		if(btpak_getcbm(bm, ix))
			{ ix++; continue; }
		ix1=ix+nblk;
		if(ix1>img->n_blocks)
			break;
		i=ix;
		while((i<ix1) && !btpak_getcbm(bm, i))
			{ i++; }
		if(i>=ix1)
		{
			img->blkrov=ix1;
			return(ix);
		}
		ix=i;
	}

	ix=0; bm=img->blkm;
	while(ix<img->n_blocks)
	{
		if(btpak_getcbm(bm, ix))
			{ ix++; continue; }
		ix1=ix+nblk;
		if(ix1>img->n_blocks)
			break;
		i=ix;
		while((i<ix1) && !btpak_getcbm(bm, i))
			{ i++; }
		if(i>=ix1)
		{
			img->blkrov=ix1;
			return(ix);
		}
		ix=i;
	}
	
	if((img->n_blocks+nblk)>img->m_blocks)
	{
		/* Resize Bitmap */
		j=4096;
		k=img->n_blocks+nblk;
		while(j<k)j=j+(j>>1);
		j=(j+3)/4;
		j=((j+511)/512)*512;
		BtPak_ImageResizeLoadChunk(img, img->lcBlkm, j);
		img->m_blocks=j*4;
		
		img->blkm=img->lcBlkm->data;
		bm=img->blkm;
	}
	
	ix=img->n_blocks;
	while((ix>0) && !btpak_getcbm(bm, ix-1))
		{ ix--; }
	ix1=ix+nblk;
	img->blkrov=ix1;
	img->n_blocks=ix1+1;
	return(ix);
}

int BtPak_ImageAllocBlocks(BtPak0_Image *img, int nblk)
{
	int ix;
	int i, j, k;
	
	ix=BtPak_ImageFindFreeBlocks(img, nblk);
	
	img->lcBlkm->isDirty|=1;
	btpak_setcbm(img->blkm, ix, 1);
	for(i=1; i<nblk; i++)
		btpak_setcbm(img->blkm, ix+i, 2);
	return(ix);
}

int BtPak_ImageFreeBlocks(BtPak0_Image *img, int ix)
{
	int i, j, k, n;
	
	i=btpak_getcbm(img->blkm, ix);
	if(i!=1)
		return(-1);
	
	img->lcBlkm->isDirty|=1;
	n=img->n_blocks-ix;
	btpak_setcbm(img->blkm, ix, 0);
	for(i=1; i<n; i++)
	{
		if(btpak_getcbm(img->blkm, ix+i)!=2)
			break;
		btpak_setcbm(img->blkm, ix+i, 0);
	}
	return(i-ix);
}

int BtPak_ImageAllocChunkId(BtPak0_Image *img)
{
	BtPak0_ChunkInfo *chi;
	int i, j, k;
	
	for(i=1; i<=img->n_chunks; i++)
	{
		chi=img->chki+(i-1);
		if(!bkpak_swdu64(chi->ofsChk) &&
			!bkpak_swdu32(chi->szChk))
			return(i);
	}
	
	img->n_chunks++;
	i=img->n_chunks;
	return(i);
}

BtPak0_LoadChunk *BtPak_ImageNewLoadChunk(BtPak0_Image *img, int sz)
{
	BtPak0_ChunkInfo *chi;
	BtPak0_LoadChunk *cur;
	u64 ofs;
	int idx;

	idx=BtPak_ImageAllocChunkId(img);
	cur=btpak_malloc(sizeof(BtPak0_LoadChunk));
	memset(cur, 0, sizeof(BtPak0_LoadChunk));
	cur->chkId=idx;
	
	chi=img->chki+(idx-1);
	chi->szChk=bkpak_sweu32(sz);

	cur->data=btpak_malloc(sz);
	cur->szData=sz;
	cur->isDirty=1;

	cur->next=img->lchk;
	img->lchk=cur;
	return(cur);
}

int BtPak_ImageResizeLoadChunk(
	BtPak0_Image *img, BtPak0_LoadChunk *chk, int newsz)
{
	chk->data=frgl_realloc(chk->data, newsz);
	chk->szData=newsz;
	chk->isDirty=1;
	return(0);
}

int BtPak_ImageCommitLoadChunk(
	BtPak0_Image *img, BtPak0_LoadChunk *chk)
{
	BtPak0_ChunkInfo *chi;
	u64 ofs;
	int sz, sz1, csz, bix, bn;

	if(!chk->isDirty)
		return(0);

	chi=img->chki+(chk->chkId-1);
	ofs=bkpak_swdu64(chi->ofsChk);
	sz=bkpak_swdu32(chi->szChk);
	csz=bkpak_swdu32(chi->cszChk);

	if(ofs && (chk->szData>csz))
	{
		BtPak_ImageFreeBlocks(img, (int)(ofs>>9));
		ofs=0;
	}
	
	if(!ofs)
	{
		sz=chk->szData;
		
		if(chk->cfl&BTPAK_CFL_MULTI)
		{
			sz1=chk->n_cell<<chk->shCell;
//			sz=sz1;
			if(sz1<sz)sz=sz1;
		}

		bn=(sz+511)>>9;
		bix=BtPak_ImageAllocBlocks(img, bn);
		csz=bn<<9;
		ofs=bix<<9;
	}
	
	fseek(img->fd, ofs, 0);
	fwrite(chk->data, 1, sz, img->fd);
	
	chi->ofsChk=bkpak_sweu64(ofs);
	chi->szChk=bkpak_sweu32(sz);
	chi->cszChk=bkpak_sweu32(csz);
	chi->cm=chk->cm;
	chi->cfl=chk->cfl;
	chi->shOsz=chk->shOsz;
	chi->shCell=chk->shCell;
	chi->ctId=bkpak_sweu16(chk->ctid);
	chk->isDirty=0;
	return(1);
}

int BtPak_ImageCommitChunks(
	BtPak0_Image *img)
{
	BtPak0_LoadChunk *lccur;
	int sz, csz, bn, bix, ofs;
	int i, j, k, l;

	if(!img->lcBlkm)
		return(0);

	ofs=img->ofs_chunks;
	sz=img->n_chunks*sizeof(BtPak0_ChunkInfo);
	
	if(!ofs)
	{
		bn=(sz+511)>>9;
		bix=BtPak_ImageAllocBlocks(img, bn);
		csz=bn<<9;
		ofs=bix<<9;
		img->ofs_chunks=ofs;
	}
	
	l=1;
	while(l>0)
	{
		l=0;
		lccur=img->lchk;
		while(lccur)
		{
			i=BtPak_ImageCommitLoadChunk(img, lccur);
			if(i>0)l++;
			lccur=lccur->next;
		}
	}
	
	fseek(img->fd, ofs, 0);
	fwrite(img->chki, 1, sz, img->fd);

	img->head.ofsChkInfo=bkpak_sweu64(ofs);
	img->head.nChunks=bkpak_sweu32(img->n_chunks);

	img->head.nDirEnts=bkpak_sweu32(img->n_dirent);

	fseek(img->fd, 0, 0);
	fwrite(&(img->head), 1,
		sizeof(BtPak0_Header), img->fd);
	
	return(1);
}

BtPak0_LoadChunk *BtPak_ImageFindMultiChunk(
	BtPak0_Image *img, int ctid, int sz)
{
	BtPak0_LoadChunk *lccur;
	BtPak0_ChunkInfo *chi;
	u64 ofs;
	int csz, bix, bn, cti;
	int i, j, k;
	
	lccur=img->lchk;
	while(lccur)
	{
		if((lccur->cfl&BTPAK_CFL_MULTI) &&
			(lccur->ctid==ctid))
		{
			if((lccur->szData+sz)<(1<<24))
				return(lccur);
		}

		lccur=lccur->next;
	}
	
	for(i=1; i<=img->n_chunks; i++)
	{
		chi=img->chki+(i-1);
		cti=bkpak_swdu16(chi->ctId);
		csz=bkpak_swdu32(chi->szChk);
		
		if((chi->cfl&BTPAK_CFL_MULTI) &&
			(cti==ctid) && ((csz+sz)<(1<<24)))
		{
			lccur=BtPak_ImageLoadChunk(img, i);
			return(lccur);
		}
	}
	
	csz=1<<20;
	while(sz>csz)
		csz=csz+(csz>>1);
	
	csz=((csz+511)>>9)<<9;
	
	lccur=BtPak_ImageNewLoadChunk(img, csz);
	lccur->ctid=ctid;
	lccur->cfl=BTPAK_CFL_MULTI;
	lccur->shOsz=18;
	lccur->shCell=6;
	
	k=((csz>>4)+3)/4;
	lccur->lcBm=BtPak_ImageNewLoadChunk(img, k);
	lccur->bm=lccur->lcBm->data;
	lccur->n_cell=0;
	lccur->m_cell=csz>>lccur->shCell;
	
	return(lccur);
}

int BtPak_ImageFindFreeMultiChunkSpace(
	BtPak0_Image *img, BtPak0_LoadChunk *chk, int nblk)
{
	byte *bm;
	int ix, ix1;
	int i, j, k;
	
	ix=chk->rov; bm=chk->bm;
	while(ix<chk->n_cell)
	{
		if(btpak_getcbm(bm, ix))
			{ ix++; continue; }
		ix1=ix+nblk;
		if(ix1>chk->n_cell)
			break;
		i=ix;
		while((i<ix1) && !btpak_getcbm(bm, i))
			{ i++; }
		if(i>=ix1)
		{
			chk->rov=ix1;
			return(ix);
		}
		ix=i;
	}
	
	ix=0; bm=chk->bm;
	while(ix<chk->n_cell)
	{
		if(btpak_getcbm(bm, ix))
			{ ix++; continue; }
		ix1=ix+nblk;
		if(ix1>chk->n_cell)
			break;
		i=ix;
		while((i<ix1) && !btpak_getcbm(bm, i))
			{ i++; }
		if(i>=ix1)
		{
			chk->rov=ix1;
			return(ix);
		}
		ix=i;
	}
	
	if((chk->n_cell+nblk)>chk->m_cell)
	{
		j=chk->m_cell;
		k=chk->n_cell+nblk;
		while(j<k)j=j+(j>>1);
		j=(j+3)/4;
		j=((j+511)/512)*512;
		BtPak_ImageResizeLoadChunk(img, chk->lcBm, j);
		chk->m_cell=j*4;
		
		chk->bm=chk->lcBm->data;
		bm=chk->bm;
	}
	
	ix=chk->n_cell;
	while((ix>0) && !btpak_getcbm(bm, ix-1))
		{ ix--; }
	ix1=ix+nblk;
	chk->rov=ix1;
	chk->n_cell=ix1+1;
	
	j=chk->n_cell<<chk->shCell;
	j=((j+511)/512)*512;
	if(j>chk->szData)
		{ BtPak_ImageResizeLoadChunk(img, chk, j); }
	
	return(ix);
}

int BtPak_ImageAllocMultiChunkSpace(
	BtPak0_Image *img, BtPak0_LoadChunk *chk, int isz)
{
	int ix, bn;
	int i;

	bn=(isz+((1<<chk->shCell)-1))>>chk->shCell;
	ix=BtPak_ImageFindFreeMultiChunkSpace(img, chk, bn);

	chk->lcBm->isDirty|=1;
	chk->isDirty|=1;

	btpak_setcbm(chk->bm, ix, 1);
	for(i=1; i<bn; i++)
		{ btpak_setcbm(chk->bm, ix+i, 2); }
	return(ix);
}

int BtPak_ImageFreeMultiChunkSpace(
	BtPak0_Image *img, BtPak0_LoadChunk *chk, int ix)
{
	int i, j, k, n;
	
	i=btpak_getcbm(chk->bm, ix);
	if(i!=1)
		return(-1);
	
	chk->lcBm->isDirty|=1;
	n=chk->n_cell-ix;
	btpak_setcbm(chk->bm, ix, 0);
	for(i=1; i<n; i++)
	{
		if(btpak_getcbm(chk->bm, ix+i)!=2)
			break;
		btpak_setcbm(chk->bm, ix+i, 0);
	}
	return(i-ix);
}



char *BtPak_ImageGetNameOvf(BtPak0_Image *img, byte *nbuf)
{
	BtPak0_ChunkInfo *chi;
	BtPak0_LoadChunk *chk;
	char *s;
	int i, j, k, l;

	j=*(u32 *)(nbuf+0);
	if(j!=0xFFFFFF)
		return(NULL);
	j=*(u32 *)(nbuf+4);
	k=*(u64 *)(nbuf+8);
	
	chk=BtPak_ImageLoadChunk(img, j);
	if(!chk)
		return(NULL);

	chi=img->chki+(j-1);
	l=(k&((1<<chi->shOsz)-1))<<chi->shCell;
	s=((char *)chk->data)+l;
	return(s);
}

int BtPak_ImageLookupDirName(BtPak0_Image *img,
	int sidx, char *name)
{
	BtPak0_DirEnt *de;
	char *s0;
	int i, j, k, l;

	if(sidx<=0)
		return(0);

	l=strlen(name);

	i=sidx;
	while(i>0)
	{
		de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);
		if(!de->name[0])
			break;

		if(l>40)
		{
			s0=BtPak_ImageGetNameOvf(img, de->name);
			if(!s0)
			{
				i=bkpak_swdu32(de->deNext);
				continue;
			}
			if(!strcmp(s0, name))
				return(i);
		}else
		{
			if(!strncmp(de->name, name, 40))
				return(i);
		}
				
		i=bkpak_swdu32(de->deNext);
	}
	return(0);
}

int BtPak_ImageAllocDirEnt(BtPak0_Image *img)
{
	BtPak0_DirEnt *de;
	int i, j, k;

	img->lcDir->isDirty|=1;

	for(i=1; i<=img->n_dirent; i++)
	{
		de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);
		if(!de->name[0])
			return(i);
	}

	if((img->n_dirent+1)>img->m_dirent)
	{
		i=img->m_dirent;
		if(i<256)i=256;
		j=img->n_dirent+1;
		while(i<j)
			i=i+(i>>1);
		i=((i+7)/8)*8;
		BtPak_ImageResizeLoadChunk(img, img->lcDir,
			i*sizeof(BtPak0_DirEnt));
		img->m_dirent=i;
	}
	
	i=img->n_dirent++;
	de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);
	memset(de, 0, sizeof(BtPak0_DirEnt));
	return(i);
}

int BtPak_ImageGetDirName(BtPak0_Image *img,
	int didx, int sidx, char *name)
{
	BtPak0_DirEnt *de, *de1;
	char *s0;
	int li;
	int i, j, k, l;

//	if(sidx<=0)
//	{
//		return(0);
//	}

	l=strlen(name);

	i=sidx; li=0;
	while(i>0)
	{
		de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);
		if(!de->name[0])
			break;

		if(l>40)
		{
			s0=BtPak_ImageGetNameOvf(img, de->name);
			if(s0 && !strcmp(s0, name))
				return(i);
		}else
		{
			if(!strncmp(de->name, name, 40))
				return(i);
		}

		li=i;
		i=bkpak_swdu32(de->deNext);
	}

	img->lcDir->isDirty|=1;

	if(li)
	{
		de1=((BtPak0_DirEnt *)img->lcDir->data)+(li-1);

		i=BtPak_ImageAllocDirEnt(img);
		de1->deNext=bkpak_sweu32(i);
		de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);

		if(l<=40)
		{
			strncpy(de->name, name, 40);
		}else
		{
		}
		return(i);
	}else if(didx)
	{
		de1=((BtPak0_DirEnt *)img->lcDir->data)+(didx-1);

		i=BtPak_ImageAllocDirEnt(img);
		de1->chkId=bkpak_sweu32(i);
		de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);

		if(l<=40)
		{
			strncpy(de->name, name, 40);
		}else
		{
		}
		return(i);
	}else
	{
		/* This path should only happen if the image is empty */
		i=BtPak_ImageAllocDirEnt(img);
		de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);

		if(l<=40)
		{
			strncpy(de->name, name, 40);
		}else
		{
		}
		return(i);
	}
	
	return(0);
}

int BtPak_ImageLookupDirPath(BtPak0_Image *img, char *path)
{
	char tb[256];
	BtPak0_DirEnt *de;
	char *s, *t;
	int i;
	
	s=path; i=1;
	while(*s=='/')s++;
	while(*s)
	{
		t=tb;
		while(*s && (*s!='/'))
			{ *t++=*s++; }
		*t++=0;
		i=BtPak_ImageLookupDirName(img, i, tb);
		if(i<=0)break;
		if(*s=='/')
		{
			while(*s=='/')s++;
			if(*s)
			{
				de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);
				if(de->deTy==2)
				{
					i=bkpak_swdu32(de->chkId);
					continue;
				}
				i=-1;
				break;
			}
		}
	}
	return(i);
}

int BtPak_ImageGetDirPath(BtPak0_Image *img, char *path)
{
	char tb[256];
	BtPak0_DirEnt *de;
	char *s, *t;
	int i, j;
	
	s=path; i=1; j=0;
	while(*s=='/')s++;
	while(*s)
	{
		t=tb;
		while(*s && (*s!='/'))
			{ *t++=*s++; }
		*t++=0;
		i=BtPak_ImageGetDirName(img, j, i, tb);
		if(i<=0)break;
		if(*s=='/')
		{
			while(*s=='/')s++;
			if(*s)
			{
				de=((BtPak0_DirEnt *)img->lcDir->data)+(i-1);
				if(de->deTy==2)
				{
					j=i;
					i=bkpak_swdu32(de->chkId);
					continue;
				}
				if(de->deTy==0)
				{
					img->lcDir->isDirty|=1;
					de->deTy=2;
					j=i;
					i=bkpak_swdu32(de->chkId);
					continue;
				}
				i=-1;
				break;
			}
		}
	}
	return(i);
}

BtPak0_DirEnt *BtPak_ImageGetDirent(BtPak0_Image *img, int id)
{
	BtPak0_DirEnt *de;
	if(id<=0)
		return(NULL);
	de=((BtPak0_DirEnt *)img->lcDir->data)+(id-1);
	return(de);
}

BTEIFGL_API int BtPak_ImageLoadFile(BtPak0_Image *img, char *path,
	byte **rbuf, int *rsz)
{
	BtPak0_LoadChunk *chk;
	BtPak0_DirEnt *de;
	u64 osz;
	int ofs, sz, sz1;
	int i, j, k, l;
	byte *buf, *cs;
	int id;
	
	id=BtPak_ImageLookupDirPath(img, path);
	if(id<=0)return(-1);
	
	de=BtPak_ImageGetDirent(img, id);

	j=bkpak_swdu32(de->chkId);
	osz=bkpak_swdu64(de->oszFile);
	
	chk=BtPak_ImageLoadChunk(img, j);
	if(!chk)
		return(-1);

	sz=osz>>chk->shOsz;
	ofs=(osz&((1<<chk->shOsz)-1))<<chk->shCell;
	cs=((byte *)chk->data)+ofs;
	
	buf=*rbuf; sz1=*rsz;
	if(buf && (sz>sz1))
	{
		*rsz=sz;
		return(0);
	}
	
	if(!buf)
		{ buf=btpak_malloc(sz+64); }
	
	memcpy(buf, cs, sz);
	*rbuf=buf;
	*rsz=sz;
	return(id);
}

int BtPak_ImageCalcPathCtId(BtPak0_Image *img, char *path)
{
	char *s, *s1;
	int h;
	
	s=path+strlen(path);
	while(s>path && (*s!='.'))
		s--;
	
	h=0;
	s1=s;	while(*s1)	{ h=(h*251)+(*s1++)+1; }
	s1=s;	while(*s1)	{ h=(h*251)+(*s1++)+1; }
	h=((h*251)>>8)&65535;
	return(h);
}

BTEIFGL_API int BtPak_ImageStoreFile(BtPak0_Image *img, char *path,
	byte *ibuf, int isz)
{
	BtPak0_LoadChunk *chk;
	BtPak0_DirEnt *de;
	u64 osz;
	int ix;
	int ofs, sz, sz1;
	int i, j, k, l;
	byte *buf, *cs;
	int id;
	
	id=BtPak_ImageGetDirPath(img, path);
	if(id<=0)return(-1);
	
	de=BtPak_ImageGetDirent(img, id);

	if(de->deTy==0)
	{
		img->lcDir->isDirty|=1;
		de->deTy=1;
//		i=bkpak_swdu32(de->chkId);
		
		if(isz>=(1<<20))
		{
			chk=BtPak_ImageNewLoadChunk(img, isz);
			de->chkId=bkpak_sweu32(chk->chkId);
			de->oszFile=bkpak_sweu64(isz);
		}else
		{
			k=BtPak_ImageCalcPathCtId(img, path);
			chk=BtPak_ImageFindMultiChunk(img, k, isz);
			de->chkId=bkpak_sweu32(chk->chkId);
			
			ix=BtPak_ImageAllocMultiChunkSpace(img, chk, isz);
			osz=(((u64)isz)<<chk->shOsz)|ix;
			de->oszFile=bkpak_sweu64(osz);
		}
	}

	img->lcDir->isDirty|=1;

	j=bkpak_swdu32(de->chkId);
	osz=bkpak_swdu64(de->oszFile);
	
	chk=BtPak_ImageLoadChunk(img, j);
	if(!chk)
		return(-1);

	sz=osz>>chk->shOsz;
	ix=osz&((1<<chk->shOsz)-1);
	ofs=ix<<chk->shCell;
	cs=((byte *)chk->data)+ofs;

	if(isz>sz)
	{
		if(chk->cfl&BTPAK_CFL_MULTI)
		{
			BtPak_ImageFreeMultiChunkSpace(img, chk, ix);
			
			ix=BtPak_ImageAllocMultiChunkSpace(img, chk, isz);
			osz=(((u64)isz)<<chk->shOsz)|ix;
			de->oszFile=bkpak_sweu64(osz);

			ofs=ix<<chk->shCell;
			cs=((byte *)chk->data)+ofs;
		}else
		{
			BtPak_ImageResizeLoadChunk(img, chk, isz);
			de->oszFile=bkpak_sweu64(isz);
			cs=((byte *)chk->data);
		}
	}else if(isz<sz)
	{
		if(chk->cfl&BTPAK_CFL_MULTI)
		{
			osz=(((u64)isz)<<chk->shOsz)|ix;
			de->oszFile=bkpak_sweu64(osz);
		}else
		{
			BtPak_ImageResizeLoadChunk(img, chk, isz);
			de->oszFile=bkpak_sweu64(isz);
			cs=((byte *)chk->data);
		}
	}

	memcpy(cs, ibuf, isz);	
	return(0);
}

BTEIFGL_API int BtPak_ImageCommit(
	BtPak0_Image *img)
{
	BtPak_ImageCommitChunks(img);
	return(0);
}


BTEIFGL_API BtPak0_OpenFile *BtPak_ImageFileOpenRead(
	BtPak0_Image *img, char *path)
{
	BtPak0_OpenFile *tfd;
	byte *buf;
	int sz, id;
	
	buf=NULL;
	id=BtPak_ImageLoadFile(img, path, &buf, &sz);
	if(!buf)
		return(NULL);
	
	tfd=btpak_malloc(sizeof(BtPak0_OpenFile));
	tfd->data=buf;
	tfd->szData=sz;
	tfd->deId=id;
	tfd->pos=0;
	
	return(tfd);
}

BTEIFGL_API BtPak0_OpenFile *BtPak_ImageFileOpen(
	BtPak0_Image *img, char *path, char *mode)
{
	if((mode[0]=='r') && (mode[1]!='+'))
	{
		return(BtPak_ImageFileOpenRead(img, path));
	}
	
	return(NULL);
}

BTEIFGL_API int BtPak_FileClose(BtPak0_OpenFile *fd)
{
	btpak_free(fd->data);
	btpak_free(fd);
	return(0);
}

BTEIFGL_API int BtPak_FileSeek(BtPak0_OpenFile *fd, s64 pos, int rel)
{
	switch(rel)
	{
	case 0: fd->pos=pos; break;
	case 1: fd->pos+=pos; break;
	case 2: fd->pos=fd->szData+pos; break;
	default: break;
	}
	return(0);
}

BTEIFGL_API s64 BtPak_FileTell(BtPak0_OpenFile *fd)
{
	return(fd->pos);
}

BTEIFGL_API int BtPak_FileRead(
	void *buf, int sz1, int sz2, BtPak0_OpenFile *fd)
{
	byte *ct;
	int sz;
	int i, j, n;

	sz=sz1*sz2;	
	if((fd->pos>=0) && ((fd->pos+sz)<=fd->szData))
	{
		memcpy(buf, fd->data+fd->pos, sz);
		fd->pos+=sz;
		return(sz2);
	}
	
	if(fd->pos<0)
		return(0);
	
	ct=buf;
	for(i=0; i<sz2; i++)
	{
		if((fd->pos+sz1)>fd->szData)
			break;
		memcpy(ct, fd->data+fd->pos, sz1);
		fd->pos+=sz1;
		ct+=sz1;
	}
	return(i);
}

BTEIFGL_API int BtPak_FileWrite(
	void *buf, int sz1, int sz2, BtPak0_OpenFile *fd)
{
	return(0);
}
