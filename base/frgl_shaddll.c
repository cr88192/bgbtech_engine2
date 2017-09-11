#include <bteifgl.h>

#ifndef GLES

#ifdef _WIN32
#include <windows.h>
#include <wingdi.h>
#include <commctrl.h>
#endif

#ifdef _WIN32

//PFNWGLGETPROCADDRESS		pwglGetProcAddress;
void *(APIENTRY *pwglGetProcAddress)(char *name);

HGLRC (APIENTRY *pwglCreateContext)(HDC hdc);
BOOL (APIENTRY *pwglDeleteContext)(HGLRC hglrc);
HGLRC (APIENTRY *pwglGetCurrentContext)(void);
HDC (APIENTRY *pwglGetCurrentDC)(void);
BOOL (APIENTRY *pwglMakeCurrent)(HDC hdc, HGLRC hglrc);
BOOL (APIENTRY *pwglShareLists)(HGLRC a0, HGLRC a1);

int (APIENTRY *pwglChoosePixelFormat)(
	HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);
int (APIENTRY *pwglGetPixelFormat)(HDC hdc);
BOOL (APIENTRY *pwglSetPixelFormat)(
		HDC hdc, INT iPixelFormat, const PIXELFORMATDESCRIPTOR *ppfd);
int (APIENTRY *pwglDescribePixelFormat)(
		HDC hdc, INT iPixelFormat, UINT nBytes,
		LPPIXELFORMATDESCRIPTOR ppfd);
BOOL (APIENTRY *pwglSwapBuffers)(HDC hdc);

HMODULE frgl_opengl32_dll=NULL;
HMODULE frgl_gdi32_dll=NULL;
// char *frgl_opengl32_dllname="opengl32";
char *frgl_opengl32_dllname="rasw_opengl32";
byte frgl_nocompressedtextures=0;
byte frgl_nobc7=0;
byte frgl_israsw=0;

int FRGL_NoCompressedTexturesP()
{
	return(frgl_nocompressedtextures);
//	return(0);
}

int FRGL_NoBC7P()
	{ return(frgl_nobc7); }

int FRGL_IsRASWP()
	{ return(frgl_israsw); }

int FRGL_InitOpenGlDLL()
{
	char tb[256];

	if(frgl_opengl32_dll)
		return(0);
	
	frgl_opengl32_dllname=FRGL_CvarGet("gl_driver");
	
	sprintf(tb, "%s.dll", frgl_opengl32_dllname);
	frgl_opengl32_dll=LoadLibrary(tb);

	frgl_gdi32_dll=LoadLibrary("gdi32.dll");

	pwglGetProcAddress=(void *)GetProcAddress(
		frgl_opengl32_dll, "wglGetProcAddress");
	
	pwglCreateContext		=frwglGetProcAddress("wglCreateContext");
	pwglDeleteContext		=frwglGetProcAddress("wglDeleteContext");
	pwglGetCurrentContext	=frwglGetProcAddress("wglGetCurrentContext");
	pwglGetCurrentDC		=frwglGetProcAddress("wglGetCurrentDC");
	pwglMakeCurrent			=frwglGetProcAddress("wglMakeCurrent");
	pwglShareLists			=frwglGetProcAddress("wglShareLists");

	if(!strcmp(frgl_opengl32_dllname, "opengl32"))
	{
		pwglChoosePixelFormat	=frwglGetProcAddress("ChoosePixelFormat");
		pwglGetPixelFormat		=frwglGetProcAddress("GetPixelFormat");
		pwglSetPixelFormat		=frwglGetProcAddress("SetPixelFormat");
		pwglDescribePixelFormat	=frwglGetProcAddress("DescribePixelFormat");
		pwglSwapBuffers			=frwglGetProcAddress("SwapBuffers");

//		frgl_nobc7=1;
	}else
	{
		pwglChoosePixelFormat	=frwglGetProcAddress("wglChoosePixelFormat");
		pwglGetPixelFormat		=frwglGetProcAddress("wglGetPixelFormat");
		pwglSetPixelFormat		=frwglGetProcAddress("wglSetPixelFormat");
		pwglDescribePixelFormat	=frwglGetProcAddress("wglDescribePixelFormat");
		pwglSwapBuffers			=frwglGetProcAddress("wglSwapBuffers");

		frgl_nocompressedtextures=0;
//		frgl_nobc7=1;
		frgl_israsw=1;
	}
	
	FRGL_InitOpenGlFuncs();
}

void *frwglGetProcAddress(char *name)
{
	void *ptr;

	if(pwglGetProcAddress)
	{
		ptr=pwglGetProcAddress(name);
		if(ptr)return(ptr);
	}
	
	if(!frgl_opengl32_dll)
	{
		FRGL_InitOpenGlDLL();
		if(!frgl_opengl32_dll)
			return(NULL);

		if(pwglGetProcAddress)
		{
			ptr=pwglGetProcAddress(name);
			if(ptr)return(ptr);
		}
	}

	ptr=GetProcAddress(frgl_opengl32_dll, name);
	if(ptr)return(ptr);

	if(frgl_gdi32_dll)
	{
		ptr=GetProcAddress(frgl_gdi32_dll, name);
		if(ptr)return(ptr);
	}

	return(NULL);
}

HGLRC frwglCreateContext(HDC hdc)
	{ return(pwglCreateContext(hdc)); }

BOOL frwglDeleteContext(HGLRC hglrc)
	{ return(pwglDeleteContext(hglrc)); }

HGLRC frwglGetCurrentContext(void)
	{ return(pwglGetCurrentContext()); }

HDC frwglGetCurrentDC(void)
	{ return(pwglGetCurrentDC()); }

BOOL frwglMakeCurrent(HDC hdc, HGLRC hglrc)
	{ return(pwglMakeCurrent(hdc, hglrc)); }

BOOL frwglShareLists(HGLRC a0, HGLRC a1)
	{ return(pwglShareLists(a0, a1)); }

int frwglChoosePixelFormat(
		HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd)
	{ return(pwglChoosePixelFormat(hdc, ppfd)); }

int frwglGetPixelFormat(HDC hdc)
	{ return(pwglGetPixelFormat(hdc)); }

BOOL frwglSetPixelFormat(
		HDC hdc, INT iPixelFormat, const PIXELFORMATDESCRIPTOR *ppfd)
	{ return(pwglSetPixelFormat(hdc, iPixelFormat, ppfd)); }

int frwglDescribePixelFormat(
		HDC hdc, INT iPixelFormat, UINT nBytes,
		LPPIXELFORMATDESCRIPTOR ppfd)
	{ return(pwglDescribePixelFormat(hdc, iPixelFormat,
		nBytes, ppfd)); }

BOOL frwglSwapBuffers(HDC hdc)
	{ return(pwglSwapBuffers(hdc)); }

#endif

#ifdef __EMSCRIPTEN__
void *frwglGetProcAddress(char *name)
{
	FRGL_InitOpenGlFuncs();
	return(SDL_GL_GetProcAddress(name));
}
#endif

void (APIENTRY *pglAlphaFunc)(GLenum func, GLclampf ref);
void (APIENTRY *pglBindTexture)(GLenum target, GLuint texture);
void (APIENTRY *pglBlendFunc)(GLenum sfactor, GLenum dfactor);
void (APIENTRY *pglClear)(GLbitfield mask);
void (APIENTRY *pglClearColor)(
	GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (APIENTRY *pglColor4f)(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (APIENTRY *pglColorMask)(
	GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void (APIENTRY *pglColorPointer)(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (APIENTRY *pglCopyTexImage2D)(
	GLenum target, GLint level, GLenum internalformat,
	GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void (APIENTRY *pglCullFace)(GLenum mode);
void (APIENTRY *pglDeleteTextures)(GLsizei n, const GLuint *textures);
void (APIENTRY *pglDepthFunc)(GLenum func);
void (APIENTRY *pglDisable)(GLenum cap);
void (APIENTRY *pglDisableClientState)(GLenum cap);
void (APIENTRY *pglDrawArrays)(GLenum mode, GLint first, GLsizei count);
void (APIENTRY *pglDrawElements)(
	GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void (APIENTRY *pglEnable)(GLenum cap);
void (APIENTRY *pglEnableClientState)(GLenum cap);
void (APIENTRY *pglFinish)(void);
void (APIENTRY *pglFlush)(void);
void (APIENTRY *pglFrontFace)(GLenum mode);
void (APIENTRY *pglGenTextures)(GLsizei n, GLuint *textures);
GLenum (APIENTRY *pglGetError)(void);
void (APIENTRY *pglGetIntegerv)(GLenum pname, GLint *params);
void (APIENTRY *pglHint)(GLenum target, GLenum mode);
const GLubyte* (APIENTRY *pglGetString)(GLenum name);
void (APIENTRY *pglLoadIdentity)(void);
void (APIENTRY *pglMatrixMode)(GLenum mode);
void (APIENTRY *pglMultMatrixf)(const GLfloat *m);
void (APIENTRY *pglNormalPointer)(
	GLenum type, GLsizei stride, const GLvoid *ptr);
void (APIENTRY *pglPolygonMode)(GLenum face, GLenum mode);
void (APIENTRY *pglReadPixels)(
	GLint x, GLint y, GLsizei width, GLsizei height,
	GLenum format, GLenum type, GLvoid *pixels);
void (APIENTRY *pglShadeModel)(GLenum mode);
void (APIENTRY *pglTexCoordPointer)(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (APIENTRY *pglTexEnvf)(
	GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *pglTexEnvi)(
	GLenum target, GLenum pname, GLint param);
void (APIENTRY *pglTexImage2D)(
	GLenum target, GLint level, GLint internalFormat,
	GLsizei width, GLsizei height, GLint border, GLenum format,
	GLenum type, const GLvoid *pixels);
void (APIENTRY *pglTexParameterf)(
	GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *pglTexParameteri)(
	GLenum target, GLenum pname, GLint param);
void (APIENTRY *pglVertexPointer)(
	GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
void (APIENTRY *pglViewport)(
	GLint x, GLint y, GLsizei width, GLsizei height);

int FRGL_InitOpenGlFuncs()
{
	if(pglAlphaFunc)
		return(0);

	pglAlphaFunc			=frwglGetProcAddress("glAlphaFunc");
	pglBindTexture			=frwglGetProcAddress("glBindTexture");
	pglBlendFunc			=frwglGetProcAddress("glBlendFunc");
	pglClear				=frwglGetProcAddress("glClear");
	pglClearColor			=frwglGetProcAddress("glClearColor");
	pglColor4f				=frwglGetProcAddress("glColor4f");
	pglColorMask			=frwglGetProcAddress("glColorMask");
	pglColorPointer			=frwglGetProcAddress("glColorPointer");
	pglCopyTexImage2D		=frwglGetProcAddress("glCopyTexImage2D");
	pglCullFace				=frwglGetProcAddress("glCullFace");
	pglDeleteTextures		=frwglGetProcAddress("glDeleteTextures");
	pglDepthFunc			=frwglGetProcAddress("glDepthFunc");
	pglDisable				=frwglGetProcAddress("glDisable");
	pglDisableClientState	=frwglGetProcAddress("glDisableClientState");
	pglDrawArrays			=frwglGetProcAddress("glDrawArrays");
	pglDrawElements			=frwglGetProcAddress("glDrawElements");
	pglEnable				=frwglGetProcAddress("glEnable");
	pglEnableClientState	=frwglGetProcAddress("glEnableClientState");
	pglFinish				=frwglGetProcAddress("glFinish");
	pglFlush				=frwglGetProcAddress("glFlush");
	pglFrontFace			=frwglGetProcAddress("glFrontFace");

	pglGenTextures			=frwglGetProcAddress("glGenTextures");

	pglGetError				=frwglGetProcAddress("glGetError");
	pglGetIntegerv			=frwglGetProcAddress("glGetIntegerv");
	pglGetString			=frwglGetProcAddress("glGetString");
	pglHint					=frwglGetProcAddress("glHint");
	pglLoadIdentity			=frwglGetProcAddress("glLoadIdentity");
	pglMatrixMode			=frwglGetProcAddress("glMatrixMode");
	pglMultMatrixf			=frwglGetProcAddress("glMultMatrixf");
	pglNormalPointer		=frwglGetProcAddress("glNormalPointer");
	pglPolygonMode			=frwglGetProcAddress("glPolygonMode");
	pglReadPixels			=frwglGetProcAddress("glReadPixels");
	pglShadeModel			=frwglGetProcAddress("glShadeModel");
	pglTexCoordPointer		=frwglGetProcAddress("glTexCoordPointer");
	pglTexEnvf				=frwglGetProcAddress("glTexEnvf");
	pglTexEnvi				=frwglGetProcAddress("glTexEnvi");
	pglTexImage2D			=frwglGetProcAddress("glTexImage2D");
	pglTexParameterf		=frwglGetProcAddress("glTexParameterf");
	pglTexParameteri		=frwglGetProcAddress("glTexParameteri");
	pglVertexPointer		=frwglGetProcAddress("glVertexPointer");
	pglViewport				=frwglGetProcAddress("glViewport");

	return(1);
}

BTEIFGL_API void frglAlphaFunc(GLenum func, GLclampf ref)
{
	pglAlphaFunc(func, ref);
}
BTEIFGL_API void frglBindTexture(GLenum target, GLuint texture)
{
	pglBindTexture(target, texture);
}
BTEIFGL_API void frglBlendFunc(GLenum sfactor, GLenum dfactor)
{
	pglBlendFunc(sfactor, dfactor);
}
BTEIFGL_API void frglClear(GLbitfield mask)
{
	pglClear(mask);
}
BTEIFGL_API void frglClearColor(
		GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	pglClearColor(red, green, blue, alpha);
}

//BTEIFGL_API void frglColor4f(
//		GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
//{
//	pglColor4f(red, green, blue, alpha);
//}

BTEIFGL_API void frglColorMask(
		GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	pglColorMask(red, green, blue, alpha);
}
BTEIFGL_API void frglColorPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	pglColorPointer(size, type, stride, ptr);
}
BTEIFGL_API void frglCopyTexImage2D(
		GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	pglCopyTexImage2D(
		target, level, internalformat, x, y, width, height, border);
}
BTEIFGL_API void frglCullFace(GLenum mode)
{
	pglCullFace(mode);
}
BTEIFGL_API void frglDeleteTextures(GLsizei n, const GLuint *textures)
{
	pglDeleteTextures(n, textures);
}
BTEIFGL_API void frglDepthFunc(GLenum func)
{
	pglDepthFunc(func);
}
BTEIFGL_API void frglDisable(GLenum cap)
{
	pglDisable(cap);
}
BTEIFGL_API void frglDisableClientState(GLenum cap)
{
	pglDisableClientState(cap);
}
BTEIFGL_API void frglDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	pglDrawArrays(mode, first, count);
}
BTEIFGL_API void frglDrawElements(
		GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	pglDrawElements(mode, count, type, indices);
}
BTEIFGL_API void frglEnable(GLenum cap)
{
	pglEnable(cap);
}
BTEIFGL_API void frglEnableClientState(GLenum cap)
{
	pglEnableClientState(cap);
}
BTEIFGL_API void frglFinish(void)
{
	pglFinish();
}
BTEIFGL_API void frglFlush(void)
{
	pglFlush();
}
BTEIFGL_API void frglFrontFace(GLenum mode)
{
	pglFrontFace(mode);
}
BTEIFGL_API void frglGenTextures(GLsizei n, GLuint *textures)
{
	*textures=0;
	pglGenTextures(n, textures);
}

BTEIFGL_API GLenum frglGetError(void)
	{ return(pglGetError());
}

BTEIFGL_API void frglGetIntegerv(GLenum pname, GLint *params)
{
	pglGetIntegerv(pname, params);
}

BTEIFGL_API void frglHint(GLenum target, GLenum mode)
{
	pglHint(target, mode);
}

BTEIFGL_API GLubyte *frglGetString(GLenum name)
{
	return((GLubyte *)(pglGetString(name)));
}

//BTEIFGL_API void frglLoadIdentity(void)
//{
//	pglLoadIdentity();
//}

BTEIFGL_API void frglMatrixMode(GLenum mode)
{
	pglMatrixMode(mode);
}

//BTEIFGL_API void frglMultMatrixf(const GLfloat *m)
//{
//	pglMultMatrixf(m);
//}

BTEIFGL_API void frglNormalPointer(
		GLenum type, GLsizei stride, const GLvoid *ptr)
{
	pglNormalPointer(type, stride, ptr);
}
BTEIFGL_API void frglPolygonMode(GLenum face, GLenum mode)
{
	pglPolygonMode(face, mode);
}
BTEIFGL_API void frglReadPixels(
		GLint x, GLint y, GLsizei width, GLsizei height,
		GLenum format, GLenum type, GLvoid *pixels)
{
	pglReadPixels(
		x, y, width, height, format, type, pixels);
}
BTEIFGL_API void frglShadeModel(GLenum mode)
{
	pglShadeModel(mode);
}
BTEIFGL_API void frglTexCoordPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	pglTexCoordPointer(size, type, stride, ptr);
}
BTEIFGL_API void frglTexEnvf(
		GLenum target, GLenum pname, GLfloat param)
{
	pglTexEnvf(target, pname, param);
}
BTEIFGL_API void frglTexEnvi(
		GLenum target, GLenum pname, GLint param)
{
	pglTexEnvi(target, pname, param);
}
BTEIFGL_API void frglTexImage2D(
		GLenum target, GLint level, GLint internalFormat,
		GLsizei width, GLsizei height, GLint border, GLenum format,
		GLenum type, const GLvoid *pixels)
{
	pglTexImage2D(target, level, internalFormat,
		width, height, border, format, type, pixels);
}
BTEIFGL_API void frglTexParameterf(
		GLenum target, GLenum pname, GLfloat param)
{
	pglTexParameterf(target, pname, param);
}
BTEIFGL_API void frglTexParameteri(
		GLenum target, GLenum pname, GLint param)
{
	pglTexParameteri(target, pname, param);
}
BTEIFGL_API void frglVertexPointer(
		GLint size, GLenum type, GLsizei stride, const GLvoid *ptr)
{
	pglVertexPointer(size, type, stride, ptr);
}
BTEIFGL_API void frglViewport(
		GLint x, GLint y, GLsizei width, GLsizei height)
{
	pglViewport(x, y, width, height);
}

#endif