
BGBDT_SwContext *bgbdt_swrgl_ctx;

BGBDT_SwContext *bgbdt_swrgl_GetContext()
{
	return(bgbdt_swrgl_ctx);
}

void bgbdt_swrgl_SetContext(BGBDT_SwContext *ctx)
{
	bgbdt_swrgl_ctx=ctx;
}

BGBDT_SwContext *swrglGetContext()
{
	return(bgbdt_swrgl_ctx);
}



void APIENTRY bgbdt_swrgl_glAlphaFunc(GLenum func, GLclampf ref)
{
}

void APIENTRY bgbdt_swrgl_glBindTexture(GLenum target, GLuint texture)
{
	BGBDT_SWR_BindTexture(swrglGetContext(), texture);
}

void APIENTRY bgbdt_swrgl_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
}

void APIENTRY bgbdt_swrgl_glClear(GLbitfield mask)
{
	BGBDT_SWR_ClearContext(swrglGetContext());
}

void APIENTRY bgbdt_swrgl_glClearColor(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
}

void APIENTRY bgbdt_swrgl_glColor4f(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	FRGL_DBGBREAK
}

void APIENTRY bgbdt_swrgl_glColorMask(
		GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
}

void APIENTRY bgbdt_swrgl_glColorPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();

	ctx->rgbptr_size=size;
	ctx->rgbptr_type=type;
	ctx->rgbptr_stride=stride;
	ctx->rgbptr_ptr=(void *)ptr;
}

void APIENTRY bgbdt_swrgl_glCopyTexImage2D(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
}

void APIENTRY bgbdt_swrgl_glCullFace(GLenum mode)
{
}

void APIENTRY bgbdt_swrgl_glDeleteTextures(GLsizei n, const GLuint *textures)
{
}

void APIENTRY bgbdt_swrgl_glDepthFunc(GLenum func)
{
}

void APIENTRY bgbdt_swrgl_glDisable(GLenum cap)
{
	BGBDT_SwContext *ctx;
	int i;

	ctx=swrglGetContext();
	
	switch(cap)
	{
	case GL_DEPTH_TEST:
		ctx->ztest=0; break;
	case GL_CULL_FACE:
		ctx->fcull=0; break;
	case GL_TEXTURE_2D:
		ctx->en_tex2d=0; break;
	default:
		break;
	}
}

void APIENTRY bgbdt_swrgl_glDisableClientState(GLenum cap)
{
}

void APIENTRY bgbdt_swrgl_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();
	
	if(mode==GL_TRIANGLES)
	{
		if((ctx->xyzptr_type==GL_SHORT) &&
			(ctx->stptr_type==GL_HALF_FLOAT) &&
			(ctx->rgbptr_type==GL_UNSIGNED_BYTE) &&
			(ctx->xyzptr_size==3) &&
			(ctx->stptr_size==2) &&
			(ctx->rgbptr_size==4))
		{
			BGBDT_SWR_EmitDrawTriArrayFixS16HF(ctx, first/3, count/3,
				ctx->xyzptr_ptr,
				ctx->stptr_ptr,
				ctx->rgbptr_ptr);
			return;
		}

		if(	(ctx->xyzptr_type==GL_FLOAT) &&
			(ctx->stptr_type==GL_FLOAT) &&
			(ctx->rgbptr_type==GL_UNSIGNED_BYTE) &&
			(ctx->xyzptr_size==3) &&
			(ctx->stptr_size==2) &&
			(ctx->rgbptr_size==4))
		{
			BGBDT_SWR_EmitDrawTriArrayFixF32(ctx, first/3, count/3,
				ctx->xyzptr_ptr,
				ctx->stptr_ptr,
				ctx->rgbptr_ptr, 0);
			return;
		}

		if(	(ctx->xyzptr_type==GL_FLOAT) &&
			(ctx->stptr_type==GL_FLOAT) &&
			(ctx->rgbptr_type==GL_FLOAT) &&
			(ctx->xyzptr_size==3) &&
			(ctx->stptr_size==2) &&
			(ctx->rgbptr_size==4))
		{
			BGBDT_SWR_EmitDrawTriArrayFixF32(ctx, first/3, count/3,
				ctx->xyzptr_ptr,
				ctx->stptr_ptr,
				ctx->rgbptr_ptr, 3);
			return;
		}
	}

	if(mode==GL_QUADS)
	{
		if(	(ctx->xyzptr_type==GL_FLOAT) &&
			(ctx->stptr_type==GL_FLOAT) &&
			(ctx->rgbptr_type==GL_UNSIGNED_BYTE) &&
			(ctx->xyzptr_size==3) &&
			(ctx->stptr_size==2) &&
			(ctx->rgbptr_size==4))
		{
			BGBDT_SWR_EmitDrawQuadArrayFixF32(ctx, first/4, count/4,
				ctx->xyzptr_ptr,
				ctx->stptr_ptr,
				ctx->rgbptr_ptr, 0);
			return;
		}

		if(	(ctx->xyzptr_type==GL_FLOAT) &&
			(ctx->stptr_type==GL_FLOAT) &&
			(ctx->rgbptr_type==GL_FLOAT) &&
			(ctx->xyzptr_size==3) &&
			(ctx->stptr_size==2) &&
			(ctx->rgbptr_size==4))
		{
			BGBDT_SWR_EmitDrawQuadArrayFixF32(ctx, first/4, count/4,
				ctx->xyzptr_ptr,
				ctx->stptr_ptr,
				ctx->rgbptr_ptr, 3);
			return;
		}
	}

	if(mode==GL_LINES)
	{
		return;
	}
	
	FRGL_DBGBREAK_SOFT
}

void APIENTRY bgbdt_swrgl_glDrawElements(
		GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
}

void APIENTRY bgbdt_swrgl_glEnable(GLenum cap)
{
	BGBDT_SwContext *ctx;
	int i;

	ctx=swrglGetContext();
	
	switch(cap)
	{
	case GL_DEPTH_TEST:
		ctx->ztest=1; break;
	case GL_CULL_FACE:
		ctx->fcull=1; break;
	case GL_TEXTURE_2D:
		ctx->en_tex2d=1; break;
	default:
		break;
	}
}

void APIENTRY bgbdt_swrgl_glEnableClientState(GLenum cap)
{
}

void APIENTRY bgbdt_swrgl_glFinish(void)
{
}

void APIENTRY bgbdt_swrgl_glFlush(void)
{
}

void APIENTRY bgbdt_swrgl_glFrontFace(GLenum mode)
{
}

void APIENTRY bgbdt_swrgl_glGenTextures(GLsizei n, GLuint *textures)
{
	BGBDT_SwContext *ctx;
	int i;

	ctx=swrglGetContext();
	
	for(i=0; i<n; i++)
	{
		textures[i]=BGBDT_SWR_AllocTextureId(ctx);
	}
}

GLenum APIENTRY bgbdt_swrgl_glGetError(void)
{
}

void APIENTRY bgbdt_swrgl_glGetIntegerv(GLenum pname, GLint *params)
{
}

void APIENTRY bgbdt_swrgl_glHint(GLenum target, GLenum mode)
{
}

GLubyte * APIENTRY bgbdt_swrgl_glGetString(GLenum name)
{
	if(name==GL_VENDOR)
		return("BGBTech");
	if(name==GL_RENDERER)
		return("SWRGL");
	if(name==GL_VERSION)
		return("1.0");
	if(name==GL_EXTENSIONS)
		return("");
	return(NULL);
}

void APIENTRY bgbdt_swrgl_glLoadIdentity(void)
{
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();
	
	if(ctx->matmode==1)
	{
		Mat4F_Identity(ctx->mat_mdlv);
//		Mat4F_MatMultT(ctx->mat_mdlv, ctx->mat_proj, ctx->xform);
		Mat4F_MatMult(ctx->mat_mdlv, ctx->mat_proj, ctx->xform);
	}

	if(ctx->matmode==2)
	{
		Mat4F_Identity(ctx->mat_proj);
//		Mat4F_MatMultT(ctx->mat_mdlv, ctx->mat_proj, ctx->xform);
		Mat4F_MatMult(ctx->mat_mdlv, ctx->mat_proj, ctx->xform);
	}
}

void APIENTRY bgbdt_swrgl_glMatrixMode(GLenum mode)
{
	int md1;
	
	md1=0;
	if(mode==GL_MODELVIEW)md1=1;
	if(mode==GL_MODELVIEW_MATRIX)md1=1;
	if(mode==GL_PROJECTION)md1=2;
	if(mode==GL_PROJECTION_MATRIX)md1=2;
	swrglGetContext()->matmode=md1;
}

void APIENTRY bgbdt_swrgl_glMultMatrixf(const GLfloat *m)
{
	float tmat[16];
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();
	
	if(ctx->matmode==1)
	{
		Mat4F_MatMultT(ctx->mat_mdlv, (float *)m, tmat);
		Mat4F_Copy(tmat, ctx->mat_mdlv);
//		Mat4F_MatMultT(ctx->mat_mdlv, ctx->mat_proj, ctx->xform);
		Mat4F_MatMult(ctx->mat_mdlv, ctx->mat_proj, ctx->xform);
	}

	if(ctx->matmode==2)
	{
		Mat4F_MatMultT(ctx->mat_proj, (float *)m, tmat);
		Mat4F_Copy(tmat, ctx->mat_proj);
//		Mat4F_MatMultT(ctx->mat_mdlv, ctx->mat_proj, ctx->xform);
		Mat4F_MatMult(ctx->mat_mdlv, ctx->mat_proj, ctx->xform);
	}
}

void APIENTRY bgbdt_swrgl_glNormalPointer(
		GLenum type, GLsizei stride, const GLvoid *ptr)
{
}

void APIENTRY bgbdt_swrgl_glPolygonMode(GLenum face, GLenum mode)
{
}

void APIENTRY bgbdt_swrgl_glReadPixels(
		GLint x, GLint y, GLsizei width, GLsizei height,
		GLenum format, GLenum type, GLvoid *pixels)
{
}

void APIENTRY bgbdt_swrgl_glShadeModel(GLenum mode)
{
}

void APIENTRY bgbdt_swrgl_glTexCoordPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();

	ctx->stptr_size=size;
	ctx->stptr_type=type;
	ctx->stptr_stride=stride;
	ctx->stptr_ptr=(void *)ptr;
}

void APIENTRY bgbdt_swrgl_glTexEnvf(
		GLenum target, GLenum pname, GLfloat param)
{
}

void APIENTRY bgbdt_swrgl_glTexEnvi(
		GLenum target, GLenum pname, GLint param)
{
}

void APIENTRY bgbdt_swrgl_glTexImage2D(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLsizei height, GLint border, GLenum format,
		GLenum type, const GLvoid *pixels)
{
	BGBDT_SwContext *ctx;
	int clrs;
	ctx=swrglGetContext();

	if(!ctx->cur_tex)
		return;

	clrs=-1;
	if(format==GL_RGBA)
		clrs=BTIC1H_PXF_RGBA;
	if(format==GL_BGRA)
		clrs=BTIC1H_PXF_BGRA;
	
	if(clrs<0)
		return;
	
	BGBDT_SWR_UpdateTextureImage(ctx, ctx->cur_tex,
		(u32 *)pixels, level, width, height, clrs);
}

void APIENTRY bgbdt_swrgl_glTexParameterf(
		GLenum target, GLenum pname, GLfloat param)
{
}

void APIENTRY bgbdt_swrgl_glTexParameteri(
		GLenum target, GLenum pname, GLint param)
{
}

void APIENTRY bgbdt_swrgl_glVertexPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();

	ctx->xyzptr_size=size;
	ctx->xyzptr_type=type;
	ctx->xyzptr_stride=stride;
	ctx->xyzptr_ptr=(void *)ptr;
}

void APIENTRY bgbdt_swrgl_glViewport(
		GLint x, GLint y, GLsizei width, GLsizei height)
{
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();

	ctx->viewport_x=x;
	ctx->viewport_y=y;
	ctx->viewport_xs=width;
	ctx->viewport_ys=height;

	ctx->vclip_minx=x;
	ctx->vclip_miny=y;
	ctx->vclip_maxx=x+width;
	ctx->vclip_maxy=y+height;
	if(ctx->vclip_minx<0)ctx->vclip_minx=0;
	if(ctx->vclip_miny<0)ctx->vclip_miny=0;
	if(ctx->vclip_maxx<0)ctx->vclip_maxx=0;
	if(ctx->vclip_maxy<0)ctx->vclip_maxy=0;
	if(ctx->vclip_minx>ctx->xs)ctx->vclip_minx=ctx->xs;
	if(ctx->vclip_miny>ctx->ys)ctx->vclip_miny=ctx->ys;
	if(ctx->vclip_maxx>ctx->xs)ctx->vclip_maxx=ctx->xs;
	if(ctx->vclip_maxy>ctx->ys)ctx->vclip_maxy=ctx->ys;
}

void APIENTRY bgbdt_swrgl_glBeginQuery(int tgt, int id)
{
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();

	if(tgt==GL_SAMPLES_PASSED)
	{
		ctx->oq_cnt=0;
		ctx->oq_id=id;
	}
}

void APIENTRY bgbdt_swrgl_glEndQuery(int tgt)
{
	BGBDT_SwContext *ctx;
	ctx=swrglGetContext();

	if(tgt==GL_SAMPLES_PASSED)
	{
		if(ctx->oq_id<=0)
			return;
		ctx->oq_cntarr[ctx->oq_id&4095]=ctx->oq_cnt;
	}
}

void APIENTRY bgbdt_swrgl_glGetQueryObjectiv(int id, int pname, int *params)
{
	BGBDT_SwContext *ctx;
	int i;
	ctx=swrglGetContext();

	if(pname==GL_QUERY_RESULT)
	{
		i=ctx->oq_cntarr[id&4095];
		params[0]=i;
	}
}

void APIENTRY bgbdt_swrgl_glGetQueryObjectuiv(int id, int pname,
	unsigned int *params)
{
	bgbdt_swrgl_glGetQueryObjectiv(id, pname, (int *)params);
}

void *BGBDT_SWR_GetProcAddress(char *name)
{
	if(!strcmp(name, "glAlphaFunc"))
		return(bgbdt_swrgl_glAlphaFunc);
	if(!strcmp(name, "glBindTexture"))
		return(bgbdt_swrgl_glBindTexture);
	if(!strcmp(name, "glBlendFunc"))
		return(bgbdt_swrgl_glBlendFunc);
	if(!strcmp(name, "glClear"))
		return(bgbdt_swrgl_glClear);
	if(!strcmp(name, "glClearColor"))
		return(bgbdt_swrgl_glClearColor);
	if(!strcmp(name, "glColor4f"))
		return(bgbdt_swrgl_glColor4f);
	if(!strcmp(name, "glColorMask"))
		return(bgbdt_swrgl_glColorMask);
	if(!strcmp(name, "glColorPointer"))
		return(bgbdt_swrgl_glColorPointer);
	if(!strcmp(name, "glCopyTexImage2D"))
		return(bgbdt_swrgl_glCopyTexImage2D);
	if(!strcmp(name, "glCullFace"))
		return(bgbdt_swrgl_glCullFace);
	if(!strcmp(name, "glDeleteTextures"))
		return(bgbdt_swrgl_glDeleteTextures);
	if(!strcmp(name, "glDepthFunc"))
		return(bgbdt_swrgl_glDepthFunc);
	if(!strcmp(name, "glDisable"))
		return(bgbdt_swrgl_glDisable);
	if(!strcmp(name, "glDisableClientState"))
		return(bgbdt_swrgl_glDisableClientState);
	if(!strcmp(name, "glDrawArrays"))
		return(bgbdt_swrgl_glDrawArrays);
	if(!strcmp(name, "glDrawElements"))
		return(bgbdt_swrgl_glDrawElements);
	if(!strcmp(name, "glEnable"))
		return(bgbdt_swrgl_glEnable);
	if(!strcmp(name, "glEnableClientState"))
		return(bgbdt_swrgl_glEnableClientState);
	if(!strcmp(name, "glFinish"))
		return(bgbdt_swrgl_glFinish);
	if(!strcmp(name, "glFlush"))
		return(bgbdt_swrgl_glFlush);
	if(!strcmp(name, "glFrontFace"))
		return(bgbdt_swrgl_glFrontFace);

	if(!strcmp(name, "glGenTextures"))
		return(bgbdt_swrgl_glGenTextures);

	if(!strcmp(name, "glGetError"))
		return(bgbdt_swrgl_glGetError);
	if(!strcmp(name, "glGetIntegerv"))
		return(bgbdt_swrgl_glGetIntegerv);
	if(!strcmp(name, "glGetString"))
		return(bgbdt_swrgl_glGetString);
	if(!strcmp(name, "glHint"))
		return(bgbdt_swrgl_glHint);
	if(!strcmp(name, "glLoadIdentity"))
		return(bgbdt_swrgl_glLoadIdentity);
	if(!strcmp(name, "glMatrixMode"))
		return(bgbdt_swrgl_glMatrixMode);
	if(!strcmp(name, "glMultMatrixf"))
		return(bgbdt_swrgl_glMultMatrixf);
	if(!strcmp(name, "glNormalPointer"))
		return(bgbdt_swrgl_glNormalPointer);
	if(!strcmp(name, "glPolygonMode"))
		return(bgbdt_swrgl_glPolygonMode);
	if(!strcmp(name, "glReadPixels"))
		return(bgbdt_swrgl_glReadPixels);
	if(!strcmp(name, "glShadeModel"))
		return(bgbdt_swrgl_glShadeModel);
	if(!strcmp(name, "glTexCoordPointer"))
		return(bgbdt_swrgl_glTexCoordPointer);
	if(!strcmp(name, "glTexEnvf"))
		return(bgbdt_swrgl_glTexEnvf);
	if(!strcmp(name, "glTexEnvi"))
		return(bgbdt_swrgl_glTexEnvi);
	if(!strcmp(name, "glTexImage2D"))
		return(bgbdt_swrgl_glTexImage2D);
	if(!strcmp(name, "glTexParameterf"))
		return(bgbdt_swrgl_glTexParameterf);
	if(!strcmp(name, "glTexParameteri"))
		return(bgbdt_swrgl_glTexParameteri);
	if(!strcmp(name, "glVertexPointer"))
		return(bgbdt_swrgl_glVertexPointer);
	if(!strcmp(name, "glViewport"))
		return(bgbdt_swrgl_glViewport);


	if(!strcmp(name, "glBeginQuery"))
		return(bgbdt_swrgl_glBeginQuery);
	if(!strcmp(name, "glEndQuery"))
		return(bgbdt_swrgl_glEndQuery);
	if(!strcmp(name, "glGetQueryObjectiv"))
		return(bgbdt_swrgl_glGetQueryObjectiv);
	if(!strcmp(name, "glGetQueryObjectuiv"))
		return(bgbdt_swrgl_glGetQueryObjectuiv);

#ifdef _WIN32
	if(!strcmp(name, "wglCopyContext"))
		return(bgbdt_swrgl_wglCopyContext);
	if(!strcmp(name, "wglCreateContext"))
		return(bgbdt_swrgl_wglCreateContext);
	if(!strcmp(name, "wglCreateLayerContext"))
		return(bgbdt_swrgl_wglCreateLayerContext);
	if(!strcmp(name, "wglDeleteContext"))
		return(bgbdt_swrgl_wglDeleteContext);
	if(!strcmp(name, "wglDescribeLayerPlane"))
		return(bgbdt_swrgl_wglDescribeLayerPlane);
	if(!strcmp(name, "wglGetCurrentContext"))
		return(bgbdt_swrgl_wglGetCurrentContext);
	if(!strcmp(name, "wglGetCurrentDC"))
		return(bgbdt_swrgl_wglGetCurrentDC);
	if(!strcmp(name, "wglGetLayerPaletteEntries"))
		return(bgbdt_swrgl_wglGetLayerPaletteEntries);
	if(!strcmp(name, "wglGetProcAddress"))
		return(bgbdt_swrgl_wglGetProcAddress);
	if(!strcmp(name, "wglMakeCurrent"))
		return(bgbdt_swrgl_wglMakeCurrent);
	if(!strcmp(name, "wglRealizeLayerPalette"))
		return(bgbdt_swrgl_wglRealizeLayerPalette);
	if(!strcmp(name, "wglSetLayerPaletteEntries"))
		return(bgbdt_swrgl_wglSetLayerPaletteEntries);
	if(!strcmp(name, "wglShareLists"))
		return(bgbdt_swrgl_wglShareLists);
	if(!strcmp(name, "wglSwapLayerBuffers"))
		return(bgbdt_swrgl_wglSwapLayerBuffers);
	if(!strcmp(name, "wglUseFontBitmapsA"))
		return(bgbdt_swrgl_wglUseFontBitmapsA);
	if(!strcmp(name, "wglUseFontBitmapsW"))
		return(bgbdt_swrgl_wglUseFontBitmapsW);
	if(!strcmp(name, "wglUseFontOutlinesA"))
		return(bgbdt_swrgl_wglUseFontOutlinesA);
	if(!strcmp(name, "wglUseFontOutlinesW"))
		return(bgbdt_swrgl_wglUseFontOutlinesW);
	if(!strcmp(name, "wglChoosePixelFormat"))
		return(bgbdt_swrgl_wglChoosePixelFormat);
	if(!strcmp(name, "wglGetPixelFormat"))
		return(bgbdt_swrgl_wglGetPixelFormat);
	if(!strcmp(name, "wglSetPixelFormat"))
		return(bgbdt_swrgl_wglSetPixelFormat);
	if(!strcmp(name, "wglDescribePixelFormat"))
		return(bgbdt_swrgl_wglDescribePixelFormat);
	if(!strcmp(name, "wglSwapBuffers"))
		return(bgbdt_swrgl_wglSwapBuffers);
#endif

	return(NULL);
}
