#include <bteifgl.h>

#ifdef FRGL_WRAP_IMM_GL

int frgl_shader_cur;

int frgl_shader_pgl_ModelViewMatrix;
int frgl_shader_pgl_ProjectionMatrix;
int frgl_shader_pgl_Color;
int frgl_shader_pgl_SecondaryColor;
int frgl_shader_pgl_Normal;
int frgl_shader_pgl_Vertex;
int frgl_shader_pgl_TexCoord;

BTEIFGL_API void FRGL_DrawPrim_SyncSendMatrix(void)
{
	float *mvm, *pvm;

	mvm=frglGetModelviewMatrixPtr();
	pvm=frglGetProjectionMatrixPtr();

	if(frgl_shader_cur>0)
	{
//		FRGL_UniformMatrix4fv("pgl_ModelViewMatrix", 1, mvm);
//		FRGL_UniformMatrix4fv("pgl_ProjectionMatrix", 1, pvm);

		if(frgl_shader_pgl_ModelViewMatrix>=0)
		{
			frglUniformMatrix4fv(frgl_shader_pgl_ModelViewMatrix, 1, mvm);
			frglUniformMatrix4fv(frgl_shader_pgl_ProjectionMatrix, 1, pvm);
		}

#if !defined(GLES2)
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(pvm);	

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(mvm);
#endif
	}else
	{
#if !defined(GLES2)
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(pvm);	

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(mvm);
#endif
	}
}

#else

BTEIFGL_API void FRGL_DrawPrim_SyncSendMatrix(void)
{
}

#endif

#if defined(__EMSCRIPTEN__) || defined(GLES2)
#define FRGL_DRAWPRIM_LIMXYZ64K		if(nxyz>=65535)nxyz=65535;
#else
#define FRGL_DRAWPRIM_LIMXYZ64K		
#endif

#ifdef FRGL_WRAP_IMM_GL
BTEIFGL_API void FRGL_DrawPrim_DrawArraysSimpleFlat(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz)
{
	FRGL_DRAWPRIM_LIMXYZ64K

	FRGL_DrawPrim_SyncSendMatrix();

	if((frgl_shader_cur>0) && (frgl_shader_pgl_Vertex>=0))
	{
		frglEnableVertexAttribArray(frgl_shader_pgl_Vertex);

		frglVertexAttribPointer(frgl_shader_pgl_Vertex,
			xyzsize, xyztype, 0, xyzstep, xyz);

		glDrawArrays(prim, base, nxyz);

		frglDisableVertexAttribArray(frgl_shader_pgl_Vertex);
	}else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
		glDrawArrays(prim, base, nxyz);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysNormalFlat(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int normsize, int normtype, int normstep, void *norm)
{
	FRGL_DRAWPRIM_LIMXYZ64K

	FRGL_DrawPrim_SyncSendMatrix();

	if((frgl_shader_cur>0) && (frgl_shader_pgl_Vertex>=0))
	{
		frglEnableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglEnableVertexAttribArray(frgl_shader_pgl_Normal);

		frglVertexAttribPointer(frgl_shader_pgl_Vertex,
			xyzsize, xyztype, 0, xyzstep, xyz);
		frglVertexAttribPointer(frgl_shader_pgl_Normal,
			normsize, normtype, 0, normstep, norm);

		glDrawArrays(prim, base, nxyz);

		frglDisableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglDisableVertexAttribArray(frgl_shader_pgl_Normal);
	}else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
		glNormalPointer(normtype, normstep, norm);
		glDrawArrays(prim, base, nxyz);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysNormalFlatRGB(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	FRGL_DRAWPRIM_LIMXYZ64K

	FRGL_DrawPrim_SyncSendMatrix();

	if((frgl_shader_cur>0) && (frgl_shader_pgl_Vertex>=0))
	{
		frglEnableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglEnableVertexAttribArray(frgl_shader_pgl_Normal);
		frglEnableVertexAttribArray(frgl_shader_pgl_Color);

		frglVertexAttribPointer(frgl_shader_pgl_Vertex,
			xyzsize, xyztype, 0, xyzstep, xyz);
		frglVertexAttribPointer(frgl_shader_pgl_Normal,
			normsize, normtype, 0, normstep, norm);
		frglVertexAttribPointer(frgl_shader_pgl_Color,
			rgbsize, rgbtype, 0, rgbstep, rgb);

		glDrawArrays(prim, base, nxyz);

		frglDisableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglDisableVertexAttribArray(frgl_shader_pgl_Normal);
		frglDisableVertexAttribArray(frgl_shader_pgl_Color);
	}else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
		glNormalPointer(normtype, normstep, norm);
		glColorPointer(rgbsize, rgbtype, rgbstep, rgb);
		glDrawArrays(prim, base, nxyz);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysNormalTex(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm)
{
	FRGL_DRAWPRIM_LIMXYZ64K

	FRGL_DrawPrim_SyncSendMatrix();

	if((frgl_shader_cur>0) && (frgl_shader_pgl_Vertex>=0))
	{
		frglEnableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglEnableVertexAttribArray(frgl_shader_pgl_TexCoord);
		frglEnableVertexAttribArray(frgl_shader_pgl_Normal);

		frglVertexAttribPointer(frgl_shader_pgl_Vertex,
			xyzsize, xyztype, 0, xyzstep, xyz);
		frglVertexAttribPointer(frgl_shader_pgl_TexCoord,
			stsize, sttype, 0, ststep, st);
		frglVertexAttribPointer(frgl_shader_pgl_Normal,
			normsize, normtype, 0, normstep, norm);

		glDrawArrays(prim, base, nxyz);

		frglDisableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglDisableVertexAttribArray(frgl_shader_pgl_TexCoord);
		frglDisableVertexAttribArray(frgl_shader_pgl_Normal);
	}else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
		glTexCoordPointer(stsize, sttype, ststep, st);
		glNormalPointer(normtype, normstep, norm);
		glDrawArrays(prim, base, nxyz);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysNormalTexRGB(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	bool stp, nvp, clp;
	bool nvb, clb;

	FRGL_DRAWPRIM_LIMXYZ64K

	FRGL_DrawPrim_SyncSendMatrix();

	if((frgl_shader_cur>0) && (frgl_shader_pgl_Vertex>=0))
	{
		stp=(st && (frgl_shader_pgl_TexCoord>=0));
		nvp=(norm && (frgl_shader_pgl_Normal>=0));
		clp=(rgb && (frgl_shader_pgl_Color>=0));
		nvb=(normtype==GL_BYTE);
		clb=(normtype==GL_UNSIGNED_BYTE);

		FRGL_ErrorStatusUniform("<?FRGL_DrawPrim_DrawArraysNormalTexRGB,A>");

		frglEnableVertexAttribArray(frgl_shader_pgl_Vertex);
		if(stp)frglEnableVertexAttribArray(frgl_shader_pgl_TexCoord);
		if(nvp)frglEnableVertexAttribArray(frgl_shader_pgl_Normal);
		if(clp)frglEnableVertexAttribArray(frgl_shader_pgl_Color);

		frglVertexAttribPointer(frgl_shader_pgl_Vertex,
			xyzsize, xyztype, 0, xyzstep, xyz);
		if(stp)
			frglVertexAttribPointer(frgl_shader_pgl_TexCoord,
				stsize, sttype, 0, ststep, st);
		if(nvp)
			frglVertexAttribPointer(frgl_shader_pgl_Normal,
				normsize, normtype, nvb, normstep, norm);
		if(clp)
			frglVertexAttribPointer(frgl_shader_pgl_Color,
				rgbsize, rgbtype, clb, rgbstep, rgb);

		glDrawArrays(prim, base, nxyz);

		frglDisableVertexAttribArray(frgl_shader_pgl_Vertex);
		if(stp)frglDisableVertexAttribArray(frgl_shader_pgl_TexCoord);
		if(nvp)frglDisableVertexAttribArray(frgl_shader_pgl_Normal);
		if(clp)frglDisableVertexAttribArray(frgl_shader_pgl_Color);

		FRGL_ErrorStatusUniform("<?FRGL_DrawPrim_DrawArraysNormalTexRGB,B>");
	}else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
		glTexCoordPointer(stsize, sttype, ststep, st);
		glNormalPointer(normtype, normstep, norm);
		glColorPointer(rgbsize, rgbtype, rgbstep, rgb);
		glDrawArrays(prim, base, nxyz);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysTexRGB(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	FRGL_DRAWPRIM_LIMXYZ64K

	FRGL_DrawPrim_SyncSendMatrix();

	if((frgl_shader_cur>0) && (frgl_shader_pgl_Vertex>=0))
	{
		frglEnableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglEnableVertexAttribArray(frgl_shader_pgl_TexCoord);
		frglEnableVertexAttribArray(frgl_shader_pgl_Color);

		frglVertexAttribPointer(frgl_shader_pgl_Vertex,
			xyzsize, xyztype, 0, xyzstep, xyz);
		frglVertexAttribPointer(frgl_shader_pgl_TexCoord,
			stsize, sttype, 0, ststep, st);
		frglVertexAttribPointer(frgl_shader_pgl_Color,
			rgbsize, rgbtype, 1, rgbstep, rgb);

		glDrawArrays(prim, base, nxyz);

		frglDisableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglDisableVertexAttribArray(frgl_shader_pgl_TexCoord);
		frglDisableVertexAttribArray(frgl_shader_pgl_Color);
	}else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
		glTexCoordPointer(stsize, sttype, ststep, st);
		glColorPointer(rgbsize, rgbtype, rgbstep, rgb);
		glDrawArrays(prim, base, nxyz);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
}


BTEIFGL_API void FRGL_DrawPrim_DrawElementsNormalTexRGB(
	int prim, int base, int nxyz, int *indices,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	FRGL_DRAWPRIM_LIMXYZ64K

	FRGL_DrawPrim_SyncSendMatrix();

	if((frgl_shader_cur>0) && (frgl_shader_pgl_Vertex>=0))
	{
		frglEnableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglEnableVertexAttribArray(frgl_shader_pgl_TexCoord);
		frglEnableVertexAttribArray(frgl_shader_pgl_Normal);
		frglEnableVertexAttribArray(frgl_shader_pgl_Color);

		frglVertexAttribPointer(frgl_shader_pgl_Vertex,
			xyzsize, xyztype, 0, xyzstep, xyz);
		frglVertexAttribPointer(frgl_shader_pgl_TexCoord,
			stsize, sttype, 0, ststep, st);
		frglVertexAttribPointer(frgl_shader_pgl_Normal,
			normsize, normtype, 0, normstep, norm);
		frglVertexAttribPointer(frgl_shader_pgl_Color,
			rgbsize, rgbtype, 0, rgbstep, rgb);

		glDrawElements(prim, nxyz, GL_UNSIGNED_INT, indices+base);

		frglDisableVertexAttribArray(frgl_shader_pgl_Vertex);
		frglDisableVertexAttribArray(frgl_shader_pgl_TexCoord);
		frglDisableVertexAttribArray(frgl_shader_pgl_Normal);
		frglDisableVertexAttribArray(frgl_shader_pgl_Color);
	}else
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
		glTexCoordPointer(stsize, sttype, ststep, st);
		glNormalPointer(normtype, normstep, norm);
		glColorPointer(rgbsize, rgbtype, rgbstep, rgb);
		glDrawElements(prim, nxyz, GL_UNSIGNED_INT, indices+base);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
}
#endif


#ifndef FRGL_WRAP_IMM_GL

BTEIFGL_API void FRGL_DrawPrim_DrawArraysSimpleFlat(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz)
{
	FRGL_DRAWPRIM_LIMXYZ64K

//	FRGL_DrawPrim_SyncSendMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
	glDrawArrays(prim, base, nxyz);
	glDisableClientState(GL_VERTEX_ARRAY);
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysNormalFlat(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int normsize, int normtype, int normstep, void *norm)
{
	FRGL_DRAWPRIM_LIMXYZ64K

//	FRGL_DrawPrim_SyncSendMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
	glNormalPointer(normtype, normstep, norm);
	glDrawArrays(prim, base, nxyz);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysNormalFlatRGB(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	FRGL_DRAWPRIM_LIMXYZ64K

//	FRGL_DrawPrim_SyncSendMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
	glNormalPointer(normtype, normstep, norm);
	glColorPointer(rgbsize, rgbtype, rgbstep, rgb);
	glDrawArrays(prim, base, nxyz);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysNormalTex(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm)
{
	FRGL_DRAWPRIM_LIMXYZ64K

//	FRGL_DrawPrim_SyncSendMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
	glTexCoordPointer(stsize, sttype, ststep, st);
	glNormalPointer(normtype, normstep, norm);
	glDrawArrays(prim, base, nxyz);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysNormalTexRGB(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	FRGL_DRAWPRIM_LIMXYZ64K

//	FRGL_DrawPrim_SyncSendMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
	glTexCoordPointer(stsize, sttype, ststep, st);
	glNormalPointer(normtype, normstep, norm);
	glColorPointer(rgbsize, rgbtype, rgbstep, rgb);
	glDrawArrays(prim, base, nxyz);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

BTEIFGL_API void FRGL_DrawPrim_DrawArraysTexRGB(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	FRGL_DRAWPRIM_LIMXYZ64K

//	FRGL_DrawPrim_SyncSendMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
	glTexCoordPointer(stsize, sttype, ststep, st);
	glColorPointer(rgbsize, rgbtype, rgbstep, rgb);
	glDrawArrays(prim, base, nxyz);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}


BTEIFGL_API void FRGL_DrawPrim_DrawElementsNormalTexRGB(
	int prim, int base, int nxyz, int *indices,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
	FRGL_DRAWPRIM_LIMXYZ64K

//	FRGL_DrawPrim_SyncSendMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(xyzsize, xyztype, xyzstep, xyz);
	glTexCoordPointer(stsize, sttype, ststep, st);
	glNormalPointer(normtype, normstep, norm);
	glColorPointer(rgbsize, rgbtype, rgbstep, rgb);
	glDrawElements(prim, nxyz, GL_UNSIGNED_INT, indices+base);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
#endif
