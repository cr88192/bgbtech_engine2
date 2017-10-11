#ifdef _WIN32

#include <windows.h>
// #include <bgbrasw.h>

BOOL APIENTRY bgbdt_swrgl_wglCopyContext(
	HGLRC a0, HGLRC a1, UINT a2)
{
	return(0);
}

HGLRC APIENTRY bgbdt_swrgl_wglCreateContext(HDC hdc)
{
	BGBDT_SwContext *ragl;
	BITMAPINFO dib, *pdib;
	void *pix;
	HBITMAP hbmp;
	HDC hdcDib;
	HGDIOBJ hdlSel;
	int xs, ys;
	
	xs=GetDeviceCaps(hdc, HORZRES);
	ys=GetDeviceCaps(hdc, VERTRES);

	ragl=BGBDT_SWR_AllocContext(xs, ys);

	pdib=&dib;
	memset(pdib, 0, sizeof(BITMAPINFO));
	
	pdib->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	pdib->bmiHeader.biWidth         = xs;
	pdib->bmiHeader.biHeight        = ys;
	pdib->bmiHeader.biPlanes        = 1;
	pdib->bmiHeader.biBitCount      = 32;
	pdib->bmiHeader.biCompression   = BI_RGB;
	pdib->bmiHeader.biSizeImage     = 0;
	pdib->bmiHeader.biXPelsPerMeter = 0;
	pdib->bmiHeader.biYPelsPerMeter = 0;
	pdib->bmiHeader.biClrUsed       = 0;
	pdib->bmiHeader.biClrImportant  = 0;

	pix=NULL;
	hbmp=CreateDIBSection(hdc, pdib, DIB_RGB_COLORS, &pix, NULL, 0);

	ragl->os_hdc=hdc;
	ragl->os_hbmp=hbmp;
	ragl->os_rgba=pix;
	
	GdiFlush();
	memset(pix, 0, xs*ys*4);
	
	hdcDib=CreateCompatibleDC(hdc);
	ragl->os_hdcdib=hdcDib;
	
	hdlSel=SelectObject(hdcDib, hbmp);
	ragl->os_hdlsel=hdlSel;
	
	return((HGLRC)ragl);
}

HGLRC APIENTRY bgbdt_swrgl_wglCreateLayerContext(
	HDC a0, int a1)
{
	return(0);
}

BOOL APIENTRY bgbdt_swrgl_wglDeleteContext(HGLRC a0)
{
	return(0);
}

BOOL APIENTRY bgbdt_swrgl_wglDescribeLayerPlane(
	HDC a0, int a1, int a2, UINT a3, LPLAYERPLANEDESCRIPTOR a4)
{
	return(0);
}

HGLRC APIENTRY bgbdt_swrgl_wglGetCurrentContext(void)
{
	BGBDT_SwContext *ragl;
	ragl=bgbdt_swrgl_GetContext();
	return((HGLRC)ragl);
}

HDC APIENTRY bgbdt_swrgl_wglGetCurrentDC(void)
{
	BGBDT_SwContext *ragl;
	ragl=bgbdt_swrgl_GetContext();
	return((HDC)(ragl->os_hdc));
}

int APIENTRY bgbdt_swrgl_wglGetLayerPaletteEntries(
	HDC a0, int a1, int a2, int a3, COLORREF *a4)
{
	return(0);
}

PROC APIENTRY bgbdt_swrgl_wglGetProcAddress(
	LPCSTR a0)
{
	return(NULL);
}

BOOL APIENTRY bgbdt_swrgl_wglMakeCurrent(
	HDC hdc, HGLRC hglrc)
{
	BGBDT_SwContext *ragl;
	
	if(!hdc && !hglrc)
	{
		bgbdt_swrgl_SetContext(NULL);
		return(1);
	}
	
	ragl=(BGBDT_SwContext *)hglrc;
	bgbdt_swrgl_SetContext(ragl);
	ragl->os_hdc=hdc;
	return(1);
}

BOOL APIENTRY bgbdt_swrgl_wglRealizeLayerPalette(
	HDC a0, int a1, BOOL a2)
{
	return(0);
}

int APIENTRY bgbdt_swrgl_wglSetLayerPaletteEntries(
	HDC a0, int a1, int a2, int a3, const COLORREF *a4)
{
	return(0);
}

BOOL APIENTRY bgbdt_swrgl_wglShareLists(
	HGLRC a0, HGLRC a1)
{
	return(0);
}

BOOL APIENTRY bgbdt_swrgl_wglSwapLayerBuffers(
	HDC a0, UINT a1)
{
	return(0);
}

BOOL APIENTRY bgbdt_swrgl_wglUseFontBitmapsA(
	HDC a0, DWORD a1, DWORD a2, DWORD a3)
{
	return(0);
}

BOOL APIENTRY bgbdt_swrgl_wglUseFontBitmapsW(
	HDC a0, DWORD a1, DWORD a2, DWORD a3)
{
	return(0);
}

BOOL APIENTRY bgbdt_swrgl_wglUseFontOutlinesA(
	HDC a0, DWORD a1, DWORD a2, DWORD a3, FLOAT a4,
	FLOAT a5, int a6, LPGLYPHMETRICSFLOAT a7)
{
	return(0);
}

BOOL APIENTRY bgbdt_swrgl_wglUseFontOutlinesW(
	HDC a0, DWORD a1, DWORD a2, DWORD a3, FLOAT a4,
	FLOAT a5, int a6, LPGLYPHMETRICSFLOAT a7)
{
	return(0);
}


int APIENTRY bgbdt_swrgl_wglChoosePixelFormat(
	HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd)
{
	PIXELFORMATDESCRIPTOR pfd2, *ppfd2;

	ppfd2=&pfd2;
	memcpy(ppfd2, ppfd, sizeof(PIXELFORMATDESCRIPTOR));
	ppfd2->dwFlags&=~(PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER);
	ppfd2->dwFlags&=~(PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER);
	ppfd2->cColorBits=32;
	ppfd2->cDepthBits=0;
	ppfd2->cStencilBits=0;

	return(ChoosePixelFormat(hdc, ppfd2));
}

int APIENTRY bgbdt_swrgl_wglGetPixelFormat(HDC hdc)
{
	return(GetPixelFormat(hdc));
}

BOOL APIENTRY bgbdt_swrgl_wglSetPixelFormat(
	HDC hdc, INT iPixelFormat, const PIXELFORMATDESCRIPTOR *ppfd)
{
	PIXELFORMATDESCRIPTOR pfd2, *ppfd2;

	ppfd2=&pfd2;
	memcpy(ppfd2, ppfd, sizeof(PIXELFORMATDESCRIPTOR));
	ppfd2->dwFlags&=~(PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER);
	ppfd2->dwFlags&=~(PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER);
	ppfd2->cColorBits=32;
	ppfd2->cDepthBits=0;
	ppfd2->cStencilBits=0;

	return(SetPixelFormat(hdc, iPixelFormat, ppfd2));
}

int APIENTRY bgbdt_swrgl_wglDescribePixelFormat(
	HDC hdc, INT iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd)
{
	return(DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd));
}

BOOL APIENTRY bgbdt_swrgl_wglSwapBuffers(HDC hdc)
{
	BGBDT_SwContext *ragl;

	ragl=bgbdt_swrgl_GetContext();
	if(!ragl)return(0);
	if(!ragl->os_rgba)return(0);
//	if(!ragl->ractx->tgt_rgba)return(0);

//	RASWGL_Finish(ragl);

	GdiFlush();

//	if(!ragl->ractx->tgt_rgba)return(0);
//	memcpy(ragl->os_rgba, ragl->ractx->tgt_rgba,
//		ragl->ractx->tgt_xs*ragl->ractx->tgt_ys*sizeof(u32));

	BGBDT_SWR_GetContextImage(ragl,
		(u32 *)ragl->os_rgba, ragl->xs, ragl->ys);


	BitBlt(ragl->os_hdc,
		ragl->viewport_x,
		0,
		ragl->viewport_xs,
		ragl->viewport_ys,
		ragl->os_hdcdib,
		0, ragl->ys-(ragl->viewport_y+ragl->viewport_ys),
		SRCCOPY);

	return(1);
}

#endif
