#include <bteifgl.h>

BTEIFGL_API BTEWS_NetWire *BTEWS_AllocNetWireNode(BTEWS_WireGrid *grid)
{
	BTEWS_NetWire *cur;
	
	cur=grid->freenetwire;
	if(cur)
	{
		grid->freenetwire=cur->next;
		memset(cur, 0, sizeof(BTEWS_NetWire));
		return(cur);
	}
	
	cur=dtmAlloc("_btews_netwire_t", sizeof(BTEWS_NetWire));
	return(cur);
}

BTEIFGL_API BTEWS_NetWire *BTEWS_LookupNetWireNode(
	BTEWS_WireGrid *grid, int wid)
{
	BTEWS_NetWire *cur;
	cur=grid->netwire;
	while(cur)
	{
		if(cur->wid==wid)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTEIFGL_API BTEWS_NetWire *BTEWS_GetNetWireNode(
	BTEWS_WireGrid *grid, int wid)
{
	BTEWS_NetWire *cur;
	
	cur=BTEWS_LookupNetWireNode(grid, wid);
	if(cur)
		return(cur);
		
//	cur=dtmAlloc("_btews_netwire_t", sizeof(BTEWS_NetWire));
	cur=BTEWS_AllocNetWireNode(grid);
	cur->wid=wid;
	
	cur->next=grid->netwire;
	grid->netwire=cur;
	
	return(cur);
}

BTEIFGL_API BTEWS_NetComponent *BTEWS_AllocNetComponentNode(
	BTEWS_WireGrid *grid)
{
	BTEWS_NetComponent *cur;
	
	cur=grid->freenetcmp;
	if(cur)
	{
		grid->freenetcmp=cur->next;
		memset(cur, 0, sizeof(BTEWS_NetComponent));
		return(cur);
	}
	
	cur=dtmAlloc("_btews_netcomponent_t", sizeof(BTEWS_NetComponent));
	return(cur);
}

BTEIFGL_API BTEWS_NetComponent *BTEWS_LookupNetComponentNode(
	BTEWS_WireGrid *grid, int cid)
{
	BTEWS_NetComponent *cur;
	cur=grid->netcmp;
	while(cur)
	{
		if(cur->cid==cid)
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BTEIFGL_API BTEWS_NetComponent *BTEWS_GetNetComponentNode(
	BTEWS_WireGrid *grid, int cid)
{
	BTEWS_NetComponent *cur;
	
	cur=BTEWS_LookupNetComponentNode(grid, cid);
	if(cur)
		return(cur);
		
	cur=dtmAlloc("_btews_netcomponent_t", sizeof(BTEWS_NetComponent));
	cur->cid=cid;
	
	cur->next=grid->netcmp;
	grid->netcmp=cur;
	
	return(cur);
}
