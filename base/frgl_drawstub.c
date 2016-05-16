#include <bteifgl.h>

BTEIFGL_API void frglBegin(int prim)
	{ glBegin(prim); }
BTEIFGL_API void frglEnd(void)
	{ glEnd(); }
BTEIFGL_API void frglVertex2fv(float *v)
	{ glVertex2fv(v); }
BTEIFGL_API void frglVertex3fv(float *v)
	{ glVertex3fv(v); }

BTEIFGL_API void frglVertex3dv(double *v)
	{ glVertex3dv(v); }

BTEIFGL_API void frglTexCoord2fv(float *v)
	{ glTexCoord2fv(v); }

BTEIFGL_API void frglNormal3fv(float *v)
	{ glNormal3fv(v); }

BTEIFGL_API void frglNormal3sbv(sbyte *v)
{
	float ntmp[3];

	ntmp[0]=v[0]*(1.0/127.0);
	ntmp[1]=v[1]*(1.0/127.0);
	ntmp[2]=v[2]*(1.0/127.0);
	glNormal3fv(ntmp);
}

BTEIFGL_API void frglNormal3bv(sbyte *v)
{
	float ntmp[3];

	ntmp[0]=v[0]*(1.0/127.0);
	ntmp[1]=v[1]*(1.0/127.0);
	ntmp[2]=v[2]*(1.0/127.0);
	glNormal3fv(ntmp);
}

BTEIFGL_API void frglColor3fv(float *v)
{
	glColor3fv(v);
}

BTEIFGL_API void frglColor4fv(float *v)
{
	glColor4fv(v);
}

BTEIFGL_API void frglColor4ubv(byte *v)
{
	float clr[4];
	clr[0]=v[0]*(1.0/255.0);
	clr[1]=v[1]*(1.0/255.0);
	clr[2]=v[2]*(1.0/255.0);
	clr[3]=v[3]*(1.0/255.0);
	glColor4fv(clr);
}

BTEIFGL_API void frglColor3f(float r, float g, float b)
{
	glColor3f(r, g, b);
}

BTEIFGL_API void frglColor4f(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
}

BTEIFGL_API void frglColor4ub(int r, int g, int b, int a)
{
	glColor4f(r/255.0, g/255.0, b/255.0, a/255.0);
}

BTEIFGL_API void frglTexCoord2f(float s, float t)
{
	glTexCoord2f(s, t);
}

BTEIFGL_API void frglVertex2f(float x, float y)
{
	glVertex2f(x, y);
}

BTEIFGL_API void frglVertex3f(float x, float y, float z)
{
	glVertex3f(x, y, z);
}

BTEIFGL_API void frglNormal3f(float x, float y, float z)
{
	glNormal3f(x, y, z);
}

BTEIFGL_API void frglColorMask(bool r, bool g, bool b, bool a)
{
	glColorMask(r, g, b, a);
}

BTEIFGL_API void frglEnableTexture2D(void)
{
	glEnable(GL_TEXTURE_2D);
}

BTEIFGL_API void frglDisableTexture2D(void)
{
	glDisable(GL_TEXTURE_2D);
}

BTEIFGL_API void frglBindTexture2D(int tex)
{
	glBindTexture(GL_TEXTURE_2D, tex);
}

BTEIFGL_API void frglEnableLighting(void)
{
	glEnable(GL_LIGHTING);
}

BTEIFGL_API void frglDisableLighting(void)
{
	glDisable(GL_LIGHTING);
}

BTEIFGL_API void frglEnable(int flag)
{
	glEnable(flag);
}

BTEIFGL_API void frglDisable(int flag)
{
	glDisable(flag);
}


BTEIFGL_API void frglModelviewMatrix(void)
{
	glMatrixMode(GL_MODELVIEW);
}

BTEIFGL_API void frglProjectionMatrix(void)
{
	glMatrixMode(GL_PROJECTION);
}

BTEIFGL_API void frglLoadIdentity(void)
{
	glLoadIdentity();
}

BTEIFGL_API void frglPushMatrix(void)
{
	glPushMatrix();
}

BTEIFGL_API void frglPopMatrix(void)
{
	glPopMatrix();
}

BTEIFGL_API void frglMultMatrixf(float *mat)
{
	glMultMatrixf(mat);
}

BTEIFGL_API void frglTranslatef(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

BTEIFGL_API void frglScalef(float x, float y, float z)
{
	glScalef(x, y, z);
}

BTEIFGL_API void frglRotatef(float th, float nx, float ny, float nz)
{
	glRotatef(th, nx, ny, nz);
}

BTEIFGL_API void frglGetModelviewMatrix(float *v)
{
	glGetFloatv(GL_MODELVIEW_MATRIX, v);
}

BTEIFGL_API void frglOrtho(
	float mx, float nx,
	float my, float ny,
	float mz, float nz)
{
	glOrtho(mx, nx, my, ny, mz, nz);
}
