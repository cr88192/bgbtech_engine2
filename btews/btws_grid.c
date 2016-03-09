#include <bteifgl.h>

BTEWS_Component *btews_component_root=NULL;


BTEIFGL_API BTEWS_Component *BTEWS_LookupComponent(char *name)
{
	BTEWS_Component *cur;
	
	cur=btews_component_root;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTEIFGL_API BTEWS_Component *BTEWS_GetComponent(char *name)
{
	BTEWS_Component *cur;
	
	cur=BTEWS_LookupComponent(name);
	if(cur)return(cur);
	
//	cur=frgl_talloc("_btews_component_t", sizeof(BTEWS_Component));
	cur=dtmAlloc("_btews_component_t", sizeof(BTEWS_Component));
	cur->name=frgl_strdup(name);
	
	cur->next=btews_component_root;
	btews_component_root=cur;
	return(cur);
}


BTEIFGL_API BTEWS_WireGrid *BTEWS_AllocGrid(int xs, int ys)
{
	BTEWS_WireGrid *tmp;
	
//	tmp=frgl_talloc("_btews_wiregrid_t", sizeof(BTEWS_WireGrid));
	tmp=dtmAlloc("_btews_wiregrid_t", sizeof(BTEWS_WireGrid));
//	tmp->node=frgl_malloc(xs*ys*sizeof(BTEWS_WireNode *));
	tmp->node=dtmAlloc("_btews_wiregrid_data_t",
		xs*ys*sizeof(BTEWS_WireNode *));
	tmp->xs=xs; tmp->ys=ys;
	return(tmp);
}

BTEIFGL_API BTEWS_WireNode *BTEWS_GetNode(BTEWS_WireGrid *grid,
	int x, int y)
{
	if((x<0) || (y<0))
		return(NULL);
	if((x>=grid->xs) || (y>=grid->ys))
		return(NULL);
	return(grid->node[y*grid->xs+x]);
}

BTEIFGL_API BTEWS_WireNode *BTEWS_GetAllocNode(BTEWS_WireGrid *grid,
	int x, int y)
{
	BTEWS_WireNode *ncur;

	if((x<0) || (y<0))
		return(NULL);
	if((x>=grid->xs) || (y>=grid->ys))
		return(NULL);

	ncur=grid->node[y*grid->xs+x];
	if(!ncur)
	{
//		ncur=frgl_talloc("_btews_wirenode_t", sizeof(BTEWS_WireNode));
		ncur=dtmAlloc("_btews_wirenode_t", sizeof(BTEWS_WireNode));
		grid->node[y*grid->xs+x]=ncur;
		ncur->grid=grid;
		ncur->x=x;
		ncur->y=y;
		ncur->cx=-1;
		ncur->cy=-1;
	}
	
	return(ncur);
}

BTEIFGL_API int BTEWS_CheckEmptyNode(BTEWS_WireGrid *grid,
	int x, int y)
{
	BTEWS_WireNode *ncur;

	if((x<0) || (y<0))
		return(0);
	if((x>=grid->xs) || (y>=grid->ys))
		return(0);

	ncur=grid->node[y*grid->xs+x];
	if(ncur)
		return(0);
		
	return(1);
}

BTEIFGL_API void BTEWS_DeleteNode2(BTEWS_WireGrid *grid,
	int x, int y)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	BTEWS_WireNode *ncur, *ncur2, *nprv;

	if((x<0) || (y<0))
		return;
	if((x>=grid->xs) || (y>=grid->ys))
		return;

	ncur=grid->node[y*grid->xs+x];
	if(!ncur)return;

	npx=BTEWS_GetNode(grid, x+1, y  );
	nnx=BTEWS_GetNode(grid, x-1, y  );
	npy=BTEWS_GetNode(grid, x  , y+1);
	nny=BTEWS_GetNode(grid, x  , y-1);

	if(npx) { npx->wfl&=~BTEWS_WFL_CN_LF; }
	if(nnx) { nnx->wfl&=~BTEWS_WFL_CN_RT; }
	if(npy) { npy->wfl&=~BTEWS_WFL_CN_DN; }
	if(nny) { nny->wfl&=~BTEWS_WFL_CN_UP; }
	
	grid->node[y*grid->xs+x]=NULL;
	
	if(ncur->lnext || (ncur==grid->liveend))
	{
		ncur2=grid->live; nprv=NULL;
		while(ncur2)
		{
			if(ncur2==ncur)
			{
				if(nprv)
				{
					nprv->lnext=ncur2->lnext;
				}else
				{
					grid->live=ncur2->lnext;
				}
				if(ncur==grid->liveend)
					grid->liveend=nprv;
				break;
			}
			nprv=ncur2;
			ncur2=ncur2->lnext;
		}
	}
	
//	frgl_free(ncur);
	dtmFree(ncur);

	BTEWS_MarkNodeDirtyStar(grid, x, y);

	return;
}

BTEIFGL_API void BTEWS_DeleteNode(BTEWS_WireGrid *grid,
	int x, int y)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	BTEWS_WireNode *ncur;
	int cx, cy;
	int i, j;
	
	if((x<0) || (y<0))
		return;
	if((x>=grid->xs) || (y>=grid->ys))
		return;

	ncur=grid->node[y*grid->xs+x];
	if(!ncur)return;
	
	if((ncur->cx<0) || (ncur->cy<0))
	{
		BTEWS_DeleteNode2(grid, x, y);
		return;
	}

	cx=ncur->cx;	cy=ncur->cy;
	for(i=0; i<grid->ys; i++)
		for(j=0; j<grid->xs; j++)
	{
		ncur=grid->node[i*grid->xs+j];
		if(!ncur)
			continue;
		if((ncur->cx==cx) && (ncur->cy==cy))
			{ BTEWS_DeleteNode2(grid, j, i); }
	}
}

BTEIFGL_API void BTEWS_DrawGrid(BTEWS_WireGrid *grid,
	float xo, float yo, float xsc, float ysc)
{
	BTEWS_WireNode *ncur;
	float x0, y0, x1, y1;
	int i, j;
	
	for(i=0; i<grid->ys; i++)
		for(j=0; j<grid->xs; j++)
	{
		x0=xo+j*xsc; x1=x0+xsc;
		y0=yo+i*ysc; y1=y0+ysc;
		
		ncur=grid->node[i*grid->xs+j];
		if(!ncur || !ncur->Draw)
		{
			if(grid->DrawDefault)
			{
				grid->DrawDefault(grid,
					j, i, x0, y0, x1, y1);
			}
			continue;
		}
		
		ncur->Draw(ncur, x0, y0, x1, y1);
	}
}

BTEIFGL_API void BTEWS_AddNodeLive(BTEWS_WireGrid *grid,
	BTEWS_WireNode *node)
{
	if(node->lnext || node==grid->liveend)
		return;
	
	if(grid->liveend)
	{
		grid->liveend->lnext=node;
		grid->liveend=node;
	}else
	{
		grid->live=node;
		grid->liveend=node;
	}
}

BTEIFGL_API void BTEWS_MarkNodeDirty(BTEWS_WireGrid *grid,
	int x, int y)
{
	BTEWS_WireNode *node;
	
	node=BTEWS_GetNode(grid, x, y);
	if(!node)
		return;
	BTEWS_AddNodeLive(grid, node);
}

BTEIFGL_API void BTEWS_MarkNodeDirtyStar(BTEWS_WireGrid *grid,
	int x, int y)
{
	BTEWS_MarkNodeDirty(grid, x, y);
	BTEWS_MarkNodeDirty(grid, x-1, y);
	BTEWS_MarkNodeDirty(grid, x+1, y);
	BTEWS_MarkNodeDirty(grid, x, y-1);
	BTEWS_MarkNodeDirty(grid, x, y+2);
}

BTEIFGL_API void BTEWS_UpdateGridStep(BTEWS_WireGrid *grid, double dt)
{
	BTEWS_WireNode *ncur, *nnxt;
	int i, j;
	
	ncur=grid->live; j=10000;
	while(ncur && ((j--)>0))
	{
		nnxt=ncur->lnext;
		if(!ncur->Update)
		{
			ncur->lnext=NULL;
			ncur=nnxt;
			continue;
		}
		ncur->Update(ncur, 0);
		nnxt=ncur->lnext;
		ncur->lnext=NULL;
		ncur=nnxt;
	}
	grid->live=NULL;
	grid->liveend=NULL;

#if 1
	for(i=0; i<grid->ys; i++)
		for(j=0; j<grid->xs; j++)
	{
		ncur=grid->node[i*grid->xs+j];
		if(!ncur || !ncur->Update)
			{ continue; }
		ncur->Update(ncur, dt);
	}
#endif
}

BTEIFGL_API void BTEWS_UpdateGrid(BTEWS_WireGrid *grid, double dt)
{
	double vdt, vst;
	
	BTEWS_RebuildGridWid(grid);
	
	if(dt>(1.0/30))dt=0;
	
	vdt=dt*grid->sim_tf;
	vst=grid->sim_tf*grid->sim_tfs;
	grid->sim_accdt+=vdt;

	if((grid->sim_accdt/grid->sim_tf)>(1.0/30))
		grid->sim_accdt=0;

	while(grid->sim_accdt>=vst)
	{
		BTEWS_UpdateGridStep(grid, vst);
		grid->sim_ct+=vst;
		grid->sim_accdt-=vst;
	}	
}

BTEIFGL_API bool BTEWS_NodeIsWireJoin(BTEWS_WireGrid *grid,
	BTEWS_WireNode *self)
{
	if(self->wfl&BTEWS_WFL_JOIN)
	{
		if((self->wfl&BTEWS_WFL_CN_LFRT) &&
			(self->wfl&BTEWS_WFL_CN_UPDN))
		{
			if(((self->wfl&BTEWS_WFL_CN_LFRT)==BTEWS_WFL_CN_LFRT) ||
				((self->wfl&BTEWS_WFL_CN_UPDN)==BTEWS_WFL_CN_UPDN))
					return(true);
		}
	}
	return(false);
}

BTEIFGL_API void BTEWS_SpreadGridWireWid(BTEWS_WireGrid *grid,
	BTEWS_WireNode *self)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;

	npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
	nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
	npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
	nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);
	
	if(nnx && (nnx->cty!=BTEWS_CTY_WIRE) && (nnx->cty!=BTEWS_CTY_DGTWIRE))
		nnx=NULL;
	if(npx && (npx->cty!=BTEWS_CTY_WIRE) && (npx->cty!=BTEWS_CTY_DGTWIRE))
		npx=NULL;
	if(nny && (nny->cty!=BTEWS_CTY_WIRE) && (nny->cty!=BTEWS_CTY_DGTWIRE))
		nny=NULL;
	if(npy && (npy->cty!=BTEWS_CTY_WIRE) && (npy->cty!=BTEWS_CTY_DGTWIRE))
		npy=NULL;
	
	if(self->widx)
	{
		if(self->wfl&BTEWS_WFL_CN_LF)
		{
//			if(!nnx->widx && (nnx->cty==BTEWS_CTY_WIRE))
//			if(nnx && !nnx->widx)
			if(nnx && !nnx->widx &&
				!BTEWS_NodeIsWireJoin(grid, nnx))
			{
				nnx->widx=self->widx;
				if(nnx->wfl&BTEWS_WFL_JOIN)
					nnx->widy=self->widx;
				BTEWS_SpreadGridWireWid(grid, nnx);
			}
		}

		if(self->wfl&BTEWS_WFL_CN_RT)
		{
//			if(!npx->widx && (npx->cty==BTEWS_CTY_WIRE))
//			if(npx && !npx->widx)
			if(npx && !npx->widx &&
				!BTEWS_NodeIsWireJoin(grid, npx))
			{
				npx->widx=self->widx;
				if(npx->wfl&BTEWS_WFL_JOIN)
					npx->widy=self->widx;
				BTEWS_SpreadGridWireWid(grid, npx);
			}
		}
	}

	if(self->widy)
	{
		if(self->wfl&BTEWS_WFL_CN_DN)
		{
//			if(!nny->widy && (nny->cty==BTEWS_CTY_WIRE))
//			if(nny && !nny->widy)
			if(nny && !nny->widy &&
				!BTEWS_NodeIsWireJoin(grid, nny))
			{
				nny->widy=self->widy;
				if(nny->wfl&BTEWS_WFL_JOIN)
					nny->widx=self->widy;
				BTEWS_SpreadGridWireWid(grid, nny);
			}
		}

		if(self->wfl&BTEWS_WFL_CN_UP)
		{
//			if(!npy->widy && (npy->cty==BTEWS_CTY_WIRE))
//			if(npy && !npy->widy)
			if(npy && !npy->widy &&
				!BTEWS_NodeIsWireJoin(grid, npy))
			{
				npy->widy=self->widy;
				if(npy->wfl&BTEWS_WFL_JOIN)
					npy->widx=self->widy;
				BTEWS_SpreadGridWireWid(grid, npy);
			}
		}
	}
}

BTEIFGL_API void BTEWS_RebuildGridWireLinks(
	BTEWS_WireGrid *grid, BTEWS_WireNode *self)
{
	BTEWS_WireNode *npx, *nnx, *npy, *nny;
	BTEWS_NetWire *nwself;
	BTEWS_NetWire *nwpx, *nwnx, *nwpy, *nwny;

	if(self->wfl&BTEWS_WFL_CN_LFRT)
	{
		npx=BTEWS_GetNode(self->grid, self->x+1, self->y  );
		nnx=BTEWS_GetNode(self->grid, self->x-1, self->y  );
	}
	if(self->wfl&BTEWS_WFL_CN_UPDN)
	{
		npy=BTEWS_GetNode(self->grid, self->x  , self->y+1);
		nny=BTEWS_GetNode(self->grid, self->x  , self->y-1);
	}
	
	if(BTEWS_NodeIsWireJoin(grid, self))
	{
//		if(nnx)nwnx=BTEWS_LookupNetWireNode(grid, nnx->widx);
//		if(npx)nwpx=BTEWS_LookupNetWireNode(grid, npx->widx);
//		if(nny)nwny=BTEWS_LookupNetWireNode(grid, nny->widy);
//		if(npy)nwpy=BTEWS_LookupNetWireNode(grid, npy->widy);

		nwself=BTEWS_LookupNetWireNode(grid, self->widx);

		if(nnx && (self->wfl&BTEWS_WFL_CN_LF))
			nwself->cwid[0]=nnx->widx;
		if(npx && (self->wfl&BTEWS_WFL_CN_RT))
			nwself->cwid[1]=npx->widx;
		if(nny && (self->wfl&BTEWS_WFL_CN_DN))
			nwself->cwid[2]=nny->widy;
		if(npy && (self->wfl&BTEWS_WFL_CN_UP))
			nwself->cwid[3]=npy->widy;

		return;
	}

	if(self->wfl&BTEWS_WFL_CN_LFRT)
	{
		nwself=BTEWS_LookupNetWireNode(grid, self->widx);

		if(nnx && (self->wfl&BTEWS_WFL_CN_LF))
		{
			if(nnx->widx!=self->widx)
				nwself->cwid[0]=nnx->widx;
		}
		if(npx && (self->wfl&BTEWS_WFL_CN_RT))
		{
			if(npx->widx!=self->widx)
				nwself->cwid[1]=npx->widx;
		}
	}
}

BTEIFGL_API void BTEWS_FlushGridNetWires(BTEWS_WireGrid *grid)
{
	BTEWS_NetWire *cur, *nxt;
	
	cur=grid->netwire;
	grid->netwire=NULL;
	while(cur)
	{
		nxt=cur->next;
		cur->next=grid->freenetwire;
		grid->freenetwire=cur;
		cur=nxt;
	}
}

BTEIFGL_API void BTEWS_RebuildGridWid(BTEWS_WireGrid *grid)
{
	BTEWS_WireNode *ncur, *nnxt;
	int wid;
	int i, j, k;
	
	BTEWS_FlushGridNetWires(grid);
	
	for(i=0; i<grid->ys; i++)
		for(j=0; j<grid->xs; j++)
	{
		ncur=grid->node[i*grid->xs+j];
		if(!ncur)
			{ continue; }
		ncur->widx=0;
		ncur->widy=0;
	}
	
	wid=1;
	for(i=0; i<grid->ys; i++)
		for(j=0; j<grid->xs; j++)
	{
		ncur=grid->node[i*grid->xs+j];
		if(!ncur)
			{ continue; }
		if(ncur->widx && ncur->widy)
			continue;

		/* Multi-way joins get their own special wire IDs */
		if(BTEWS_NodeIsWireJoin(grid, ncur))
		{
			if(!ncur->widx && !ncur->widy)
			{
				k=wid++;
				ncur->widx=k;
				ncur->widy=k;
			}
			continue;
		}

		if(ncur->wfl&BTEWS_WFL_JOIN)
//		if(0)
		{
//			k=wid++;
			k=0;

			if(!ncur->widx && !ncur->widy)
			{
				k=wid++;
				ncur->widx=k;
				ncur->widy=k;
			}

//			if(!ncur->widx && (ncur->wfl&BTEWS_WFL_CN_LFRT))
//				{ if(!k)k=wid++; ncur->widx=k; }
//			if(!ncur->widy && (ncur->wfl&BTEWS_WFL_CN_UPDN))
//				{ if(!k)k=wid++; ncur->widy=k; }
		}else
		{
			if(!ncur->widx && (ncur->wfl&BTEWS_WFL_CN_LFRT))
				ncur->widx=wid++;
			if(!ncur->widy && (ncur->wfl&BTEWS_WFL_CN_UPDN))
				ncur->widy=wid++;

			if(!ncur->widx && !(ncur->wfl&BTEWS_WFL_CN_LFRT))
				ncur->widx=ncur->widy;
			if(!ncur->widy && !(ncur->wfl&BTEWS_WFL_CN_UPDN))
				ncur->widy=ncur->widx;
		}

		BTEWS_SpreadGridWireWid(grid, ncur);
	}
	
	for(i=1; i<wid; i++)
	{
		BTEWS_GetNetWireNode(grid, i);
	}

	for(i=0; i<grid->ys; i++)
		for(j=0; j<grid->xs; j++)
	{
		ncur=grid->node[i*grid->xs+j];
		if(!ncur)
			{ continue; }
		if(!ncur->widx && !ncur->widy)
			continue;

		BTEWS_RebuildGridWireLinks(grid, ncur);
	}
}

