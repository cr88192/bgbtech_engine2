#include <bgbrasw.h>

void BGBRASW_DrawPrimitive(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *prim)
{
	int id;

	id=prim->tabs->query_id;
	if(id)
	{
		prim->tabs->ctx->query_cur=0;
	}

	switch(prim->type)
	{
	case BGBRASW_PTYPE_LINE:
		BGBRASW_DrawPrimitive_Line(ctx, prim);
		break;
	case BGBRASW_PTYPE_BOX:
		BGBRASW_DrawPrimitive_Box(ctx, prim);
		break;
	case BGBRASW_PTYPE_TRIANGLE:
		BGBRASW_DrawPrimitive_Triangle(ctx, prim);
		break;
	case BGBRASW_PTYPE_QUAD:
		BGBRASW_DrawPrimitive_Quad(ctx, prim);
		break;
	default:
		break;
	}
	
	if(id)
	{
		if(prim->tabs->ctx->query_buf[id>>12])
		{
			prim->tabs->ctx->query_buf[id>>12][id&4095]+=
				prim->tabs->ctx->query_cur;
		}

//		if(ctx->pctx)
//		{
//			if(ctx->pctx->query_buf[id>>12])
//				ctx->pctx->query_buf[id>>12][id&4095]+=prim->tabs->ctx->query_cur;
//		}else
//		{
//			if(ctx->query_buf[id>>12])
//				ctx->query_buf[id>>12][id&4095]+=prim->tabs->ctx->query_cur;
//		}
	}
}

BGBRASW_API void BGBRASW_DrawPrimitiveList(
	BGBRASW_Context *ctx,
	BGBRASW_Primitive *list)
{
	BGBRASW_Primitive *cur;
	
	cur=list;
	while(cur)
	{
		BGBRASW_DrawPrimitive(ctx, cur);
		cur=cur->next;
	}
}
