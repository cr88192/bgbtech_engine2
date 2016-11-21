#include <bteifgl.h>

FRGL_FontInfo *font_cur;

int font_mode, font_time, font_size;
char *font_name;

int frgl_clampi(int v, int m, int n)
{
	if(v<m)return(m);
	if(v>n)return(n);
	return(v);
}

BTEIFGL_API FRGL_TextVBO *FRGL_TextVBO_Create(void)
{
	FRGL_TextVBO *ctx;
	
	ctx=frgl_talloc("frgl_textvbo_t", sizeof(FRGL_TextVBO));
	return(ctx);
}

BTEIFGL_API void FRGL_TextVBO_Reset(FRGL_TextVBO *ctx)
{
	ctx->n_xyz=0;
	ctx->n_prim=0;
}

BTEIFGL_API void FRGL_TextVBO_Upload(FRGL_TextVBO *ctx)
{
	int i, id;

	ctx->sz_xyz=ctx->n_xyz*3*sizeof(float);
	ctx->sz_st=ctx->n_xyz*2*sizeof(float);
	ctx->sz_rgba=ctx->n_xyz*4*sizeof(byte);
	
	ctx->ofs_xyz=0;
	ctx->ofs_st=(ctx->ofs_xyz+ctx->sz_xyz+15)&(~15);
	ctx->ofs_rgba=(ctx->ofs_st+ctx->sz_st+15)&(~15);
	ctx->ofs_end=(ctx->ofs_rgba+ctx->sz_rgba+15)&(~15);
	
	if(!ctx->vbo_buf)
	{
		i=256;
		while(i<ctx->ofs_end)
			i=i+(i>>1);
		ctx->vbo_buf=frgl_tatomic("frgl_textvbo_vbobuf", i);
		ctx->sz_vbo=i;
	}
	
	if(ctx->ofs_end>ctx->sz_vbo)
	{
		i=ctx->sz_vbo;
		while(i<ctx->ofs_end)
			i=i+(i>>1);
		ctx->vbo_buf=frgl_realloc(ctx->vbo_buf, i);
		ctx->sz_vbo=i;
	}
	
	memcpy(ctx->vbo_buf+ctx->ofs_xyz, ctx->xyz, ctx->sz_xyz);
	memcpy(ctx->vbo_buf+ctx->ofs_st, ctx->st, ctx->sz_st);
	memcpy(ctx->vbo_buf+ctx->ofs_rgba, ctx->rgba, ctx->sz_rgba);

	if(ctx->vbo<=0)
	{
		frglGenBuffers(1, &id);
		frglBindBuffer(GL_ARRAY_BUFFER, id);
		frglBufferData(GL_ARRAY_BUFFER,
			ctx->sz_vbo, ctx->vbo_buf, GL_STATIC_DRAW);
		ctx->vbo=id;
		frglBindBuffer(GL_ARRAY_BUFFER, 0);
	}else
	{
		frglBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
		frglBufferData(GL_ARRAY_BUFFER,
			ctx->sz_vbo, ctx->vbo_buf, GL_STATIC_DRAW);
		frglBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

BTEIFGL_API void FRGL_TextVBO_Draw(FRGL_TextVBO *ctx)
{
	int i, j, k;

	glDisable (GL_CULL_FACE);
	frglEnableTexture2D();

	frglBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	for(i=0; i<ctx->n_prim; i++)
	{
		glBindTexture(GL_TEXTURE_2D, ctx->prim[i*4+3]);
		FRGL_DrawPrim_DrawArraysTexRGB(
			ctx->prim[i*4+2], ctx->prim[i*4+0], ctx->prim[i*4+1],
			3, GL_FLOAT, 0, (byte *)(nlint)ctx->ofs_xyz,
			2, GL_FLOAT, 0, (byte *)(nlint)ctx->ofs_st,
			4, GL_UNSIGNED_BYTE, 0, (byte *)(nlint)ctx->ofs_rgba);
	}
	frglBindBuffer(GL_ARRAY_BUFFER, 0);
}

BTEIFGL_API void FRGL_TextVBO_Color4fv(FRGL_TextVBO *ctx, float *fv)
{
	ctx->cur_clr[0]=frgl_clampi(fv[0]*255, 0, 255);
	ctx->cur_clr[1]=frgl_clampi(fv[1]*255, 0, 255);
	ctx->cur_clr[2]=frgl_clampi(fv[2]*255, 0, 255);
	ctx->cur_clr[3]=frgl_clampi(fv[3]*255, 0, 255);
}

BTEIFGL_API void FRGL_TextVBO_Color4f(FRGL_TextVBO *ctx,
	float r, float g, float b, float a)
{
	ctx->cur_clr[0]=frgl_clampi(r*255, 0, 255);
	ctx->cur_clr[1]=frgl_clampi(g*255, 0, 255);
	ctx->cur_clr[2]=frgl_clampi(b*255, 0, 255);
	ctx->cur_clr[3]=frgl_clampi(a*255, 0, 255);
}

BTEIFGL_API void FRGL_TextVBO_TexCoord2fv(FRGL_TextVBO *ctx, float *fv)
{
	fv[0]=ctx->cur_st[0];
	fv[1]=ctx->cur_st[1];
}

BTEIFGL_API void FRGL_TextVBO_TexCoord2f(FRGL_TextVBO *ctx,
	float s, float t)
{
	ctx->cur_st[0]=s;
	ctx->cur_st[1]=t;
}

BTEIFGL_API void FRGL_TextVBO_VertexCheckExpand(FRGL_TextVBO *ctx)
{
	int i, n;
	
	if(!ctx->xyz)
	{
//		n=256;
		n=98304;
		ctx->xyz=frgl_tatomic("frgl_textvbo_xyz", n*3*sizeof(float));
		ctx->st=frgl_tatomic("frgl_textvbo_st", n*2*sizeof(float));
		ctx->rgba=frgl_tatomic("frgl_textvbo_rgba", n*4*sizeof(byte));
		ctx->m_xyz=n;
	}
	
	if((ctx->n_xyz+1)>=ctx->m_xyz)
	{
		n=ctx->m_xyz+(ctx->m_xyz>>1);
		ctx->xyz=frgl_realloc(ctx->xyz, n*3*sizeof(float));
		ctx->st=frgl_realloc(ctx->st, n*2*sizeof(float));
		ctx->rgba=frgl_realloc(ctx->rgba, n*4*sizeof(byte));
		ctx->m_xyz=n;
	}
}

BTEIFGL_API void FRGL_TextVBO_Vertex3fv(FRGL_TextVBO *ctx, float *fv)
{
	int i;

	FRGL_TextVBO_VertexCheckExpand(ctx);
	
	i=ctx->n_xyz++;
//	V3F_COPY(fv, ctx->xyz+i*3);
//	V2F_COPY(ctx->cur_st, ctx->st+i*2);
//	V4F_COPY(ctx->cur_clr, ctx->rgba+i*4);

	fv[0]=ctx->xyz[i*3+0];
	fv[1]=ctx->xyz[i*3+1];
	fv[2]=ctx->xyz[i*3+2];
	ctx->cur_st[0]=ctx->st[i*2+0];
	ctx->cur_st[1]=ctx->st[i*2+1];
	ctx->cur_clr[0]=ctx->rgba[i*4+0];
	ctx->cur_clr[1]=ctx->rgba[i*4+1];
	ctx->cur_clr[2]=ctx->rgba[i*4+2];
	ctx->cur_clr[3]=ctx->rgba[i*4+3];
}

BTEIFGL_API void FRGL_TextVBO_Vertex2f(FRGL_TextVBO *ctx, float x, float y)
{
	float tv[4];
	
	tv[0]=x; tv[1]=y; tv[2]=0;
	FRGL_TextVBO_Vertex3fv(ctx, tv);
}

BTEIFGL_API void FRGL_TextVBO_CheckExpandPrim(FRGL_TextVBO *ctx)
{
	int n;

	if(!ctx->prim)
	{
		n=64;
		ctx->prim=frgl_tatomic("frgl_textvbo_prim", n*4*sizeof(int));
		ctx->m_prim=n;
	}
	
	if((ctx->n_prim+1)>=ctx->m_prim)
	{
		n=ctx->m_prim+(ctx->m_prim>>1);
		ctx->prim=frgl_realloc(ctx->prim, n*4*sizeof(int));
		ctx->m_prim=n;
	}
}

BTEIFGL_API void FRGL_TextVBO_Begin(FRGL_TextVBO *ctx, int prim)
{
	ctx->cur_basexyz=ctx->n_xyz;
	ctx->cur_prim=prim;
}

BTEIFGL_API void FRGL_TextVBO_End(FRGL_TextVBO *ctx)
{
	int i, n;

	FRGL_TextVBO_CheckExpandPrim(ctx);
	
	n=ctx->n_prim++;
	
	ctx->prim[n*4+0]=ctx->cur_basexyz;
	ctx->prim[n*4+1]=ctx->n_xyz-ctx->cur_basexyz;
	ctx->prim[n*4+2]=ctx->cur_prim;
	ctx->prim[n*4+3]=ctx->cur_tex;
}

BTEIFGL_API void FRGL_TextVBO_BindTexture(FRGL_TextVBO *ctx, int tex)
{
	ctx->cur_tex=tex;
}

BTEIFGL_API int FRGL_TextVBO_DrawCharModeQI2(FRGL_TextVBO *ctx,
	int c,
	int x, int y, int w, int h,
	int r, int g, int b, int a,
	int m)
{
	static int texn;
	static int qb;
	int cw, ch, n, tm;
	float s1, t1, s2, t2;
	FRGL_FontFrag *frag;

	if(!c || (c==' '))return(0);

	if(c==-1)
	{
		texn=-1; qb=0;
		return(0);
	}

	if(c==-2)
	{
		if(qb) { FRGL_TextVBO_End(ctx); qb=0; }
		return(0);
	}

//	if(font_mode&FRGL_FONT_BLINK)
	if(m&FRGL_FONT_BLINK)
	{
		if((font_time%1000)>=500)
			return(0);
	}

	tm=font_mode;
	font_mode=m;

	GfxFont_SetFontSize(font_name, font_mode, h);

	frag=GfxFont_TryFetchFrag(font_cur, c);

	if(!frag || (frag->texnum!=texn))
	{
		if(qb) { FRGL_TextVBO_End(ctx); qb=0; }
		frag=GfxFont_FetchFrag(font_cur, c);
		if(!frag)
			return(-1);
		texn=frag->texnum;
	}

	if(!qb)
	{
		FRGL_TextVBO_BindTexture(ctx, texn);
		FRGL_TextVBO_Begin(ctx, FRGL_TRIANGLES);
		qb=1;
	}

	n=c&255;

	s1=(n&15)/16.0;
	t1=(n>>4)/16.0;
	s2=((n&15)+1)/16.0;
	t2=((n>>4)+1)/16.0;

	FRGL_TextVBO_Color4f(ctx, r/255.0, g/255.0, b/255.0, a/255.0);

	if(font_mode&FRGL_FONT_ITALIC)
	{
#if 1
		FRGL_TextVBO_TexCoord2f(ctx, s1, t1);
		FRGL_TextVBO_Vertex2f(ctx, x-(w*0.5), y);

		FRGL_TextVBO_TexCoord2f(ctx, s1, t2);
		FRGL_TextVBO_Vertex2f(ctx, x+(w*0.5), y+h);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t2);
		FRGL_TextVBO_Vertex2f(ctx, x+w+(w*0.5), y+h);


		FRGL_TextVBO_TexCoord2f(ctx, s1, t1);
		FRGL_TextVBO_Vertex2f(ctx, x-(w*0.5), y);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t2);
		FRGL_TextVBO_Vertex2f(ctx, x+w+(w*0.5), y+h);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t1);
		FRGL_TextVBO_Vertex2f(ctx, x+w-(w*0.5), y);
#endif

#if 0
		FRGL_TextVBO_TexCoord2f(ctx, s1, t1);
		FRGL_TextVBO_Vertex2f(ctx, x-(w*0.5), y);

		FRGL_TextVBO_TexCoord2f(ctx, s1, t2);
		FRGL_TextVBO_Vertex2f(ctx, x+(w*0.5), y+h);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t2);
		FRGL_TextVBO_Vertex2f(ctx, x+w+(w*0.5), y+h);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t1);
		FRGL_TextVBO_Vertex2f(ctx, x+w-(w*0.5), y);
#endif
	}else
	{
#if 1
		FRGL_TextVBO_TexCoord2f(ctx, s1, t1);
		FRGL_TextVBO_Vertex2f(ctx, x, y);

		FRGL_TextVBO_TexCoord2f(ctx, s1, t2);
		FRGL_TextVBO_Vertex2f(ctx, x, y+h);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t2);
		FRGL_TextVBO_Vertex2f(ctx, x+w, y+h);


		FRGL_TextVBO_TexCoord2f(ctx, s1, t1);
		FRGL_TextVBO_Vertex2f(ctx, x, y);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t2);
		FRGL_TextVBO_Vertex2f(ctx, x+w, y+h);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t1);
		FRGL_TextVBO_Vertex2f(ctx, x+w, y);
#endif

#if 0
		FRGL_TextVBO_TexCoord2f(ctx, s1, t1);
		FRGL_TextVBO_Vertex2f(ctx, x, y);

		FRGL_TextVBO_TexCoord2f(ctx, s1, t2);
		FRGL_TextVBO_Vertex2f(ctx, x, y+h);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t2);
		FRGL_TextVBO_Vertex2f(ctx, x+w, y+h);

		FRGL_TextVBO_TexCoord2f(ctx, s2, t1);
		FRGL_TextVBO_Vertex2f(ctx, x+w, y);
#endif
	}

	font_mode=tm;

	return(0);
}
