#include <bteifgl.h>

BTEIFGL_API void FRGL_DrawPrim_DrawArraysSimpleFlat(
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz)
{
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
