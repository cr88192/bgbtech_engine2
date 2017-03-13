#include <bteifgl.h>

#ifdef FRGL_WRAP_IMM_GL

float *frgl_drawprim_xyzbuf;
float *frgl_drawprim_stbuf;
float *frgl_drawprim_nbuf;
float *frgl_drawprim_clrbuf;

int frgl_drawprim_nvec;
int frgl_drawprim_mvec;

int frgl_drawprim_prim;
int frgl_drawprim_blend_src;
int frgl_drawprim_blend_dst;

float frgl_drawprim_sttmp[2];
float frgl_drawprim_ntmp[4];
float frgl_drawprim_clr[4];

static int frgl_drawprim_init=0;

float frgl_drawprim_matrixstack[4][16][16];
// int frgl_drawprim_matrixstackpos[4]={-1, -1, -1, -1};
int frgl_drawprim_matrixstackpos[4]={0, 0, 0, 0};
float frgl_drawprim_matrix[4][16];
int frgl_drawprim_matrixmode;

int frgl_drawprim_whitetex;

int FRGL_DrawPrim_Init()
{
	byte tb[16*16*4];
	int i;

	if(frgl_drawprim_init)
		return(0);
	frgl_drawprim_init=1;

	i=4096;
	frgl_drawprim_xyzbuf=frgl_malloc(i*4*sizeof(float));
	frgl_drawprim_stbuf=frgl_malloc(i*2*sizeof(float));
	frgl_drawprim_nbuf=frgl_malloc(i*4*sizeof(float));
	frgl_drawprim_clrbuf=frgl_malloc(i*4*sizeof(float));
	frgl_drawprim_mvec=i;
	frgl_drawprim_nvec=0;

	memset(tb, 255, 16*16*4);
	frgl_drawprim_whitetex=Tex_LoadTexture(16, 16, tb, 0);
	
	return(1);
}

BTEIFGL_API void frglBegin(int prim)
{
	FRGL_DrawPrim_Init();
	frgl_drawprim_nvec=0;
	frgl_drawprim_prim=prim;
}

BTEIFGL_API void frglEnd(void)
{
	FRGL_DrawPrim_DrawArrays(frgl_drawprim_prim,
		frgl_drawprim_xyzbuf, frgl_drawprim_stbuf,
		frgl_drawprim_nbuf, frgl_drawprim_clrbuf,
		frgl_drawprim_nvec);
}

void FRGL_DrawPrim_DrawArrays(
	int prim,
	float *xyz, float *st, float *norm, float *rgba,
	int nxyz)
{
	int i, j;

	i=4*sizeof(float);
	j=2*sizeof(float);
	FRGL_DrawPrim_DrawArraysNormalTexRGB(
		prim, 0, nxyz,
		3, GL_FLOAT, i, xyz,
		2, GL_FLOAT, j, st,
		3, GL_FLOAT, i, norm,
		4, GL_FLOAT, i, rgba);
}

void FRGL_DrawPrim_VertexCheckExpand()
{
	int i;

	if(frgl_drawprim_nvec>=frgl_drawprim_mvec)
	{
		i=frgl_drawprim_mvec+(frgl_drawprim_mvec>>1);
		frgl_drawprim_xyzbuf=
			frgl_realloc(frgl_drawprim_xyzbuf, i*4*sizeof(float));
		frgl_drawprim_stbuf=
			frgl_realloc(frgl_drawprim_stbuf, i*2*sizeof(float));
		frgl_drawprim_nbuf=
			frgl_realloc(frgl_drawprim_nbuf, i*4*sizeof(float));
		frgl_drawprim_clrbuf=
			frgl_realloc(frgl_drawprim_clrbuf, i*4*sizeof(float));
		frgl_drawprim_mvec=i;
	}
}

BTEIFGL_API void frglVertex3fv(float *v)
{
	float *t;
	int i;

	FRGL_DrawPrim_VertexCheckExpand();

	t=frgl_drawprim_stbuf+frgl_drawprim_nvec*2;
	V2F_COPY(frgl_drawprim_sttmp, t);

	t=frgl_drawprim_nbuf+frgl_drawprim_nvec*4;
	V4F_COPY(frgl_drawprim_ntmp, t);

	t=frgl_drawprim_xyzbuf+frgl_drawprim_nvec*4;
	V3F_COPY(v, t); t[3]=0;

	t=frgl_drawprim_clrbuf+frgl_drawprim_nvec*4;
	V4F_COPY(frgl_drawprim_clr, t);

	frgl_drawprim_nvec++;
}

BTEIFGL_API void frglVertex3dv(double *v)
{
	float tv[3];
	tv[0]=v[0]; tv[1]=v[1]; tv[2]=v[2];
	frglVertex3fv(tv);
}

BTEIFGL_API void frglTexCoord2fv(float *v)
{
	float *t;
	V2F_COPY(v, frgl_drawprim_sttmp);
}

BTEIFGL_API void frglNormal3fv(float *v)
{
	float *t;

	V3F_COPY(v, frgl_drawprim_ntmp);
	frgl_drawprim_ntmp[3]=0;
}

BTEIFGL_API void frglNormal4fv(float *v)
{
	float *t;
	V4F_COPY(v, frgl_drawprim_ntmp);
}

BTEIFGL_API void frglNormal3sbv(sbyte *v)
{
	frgl_drawprim_ntmp[0]=v[0]*(1.0/127.0);
	frgl_drawprim_ntmp[1]=v[1]*(1.0/127.0);
	frgl_drawprim_ntmp[2]=v[2]*(1.0/127.0);
	frgl_drawprim_ntmp[3]=0;
}

BTEIFGL_API void frglNormal3bv(sbyte *v)
{
	float ntmp[3];

	ntmp[0]=v[0]*(1.0/127.0);
	ntmp[1]=v[1]*(1.0/127.0);
	ntmp[2]=v[2]*(1.0/127.0);
	frglNormal3fv(ntmp);
}

BTEIFGL_API void frglColor3fv(float *v)
{
	V3F_COPY(v, frgl_drawprim_clr);
//	glColor3fv(v);
}

BTEIFGL_API void frglColor4fv(float *v)
{
	V4F_COPY(v, frgl_drawprim_clr);
//	glColor4fv(v);
}

BTEIFGL_API void frglColor4ubv(byte *v)
{
	frgl_drawprim_clr[0]=v[0]*(1.0/255.0);
	frgl_drawprim_clr[1]=v[1]*(1.0/255.0);
	frgl_drawprim_clr[2]=v[2]*(1.0/255.0);
	frgl_drawprim_clr[3]=v[3]*(1.0/255.0);
}

BTEIFGL_API void frglColor3f(float r, float g, float b)
{
	frgl_drawprim_clr[0]=r;
	frgl_drawprim_clr[1]=g;
	frgl_drawprim_clr[2]=b;

//	glColor4f(r, g, b, 1.0);
}

BTEIFGL_API void frglColor4f(float r, float g, float b, float a)
{
	frgl_drawprim_clr[0]=r;
	frgl_drawprim_clr[1]=g;
	frgl_drawprim_clr[2]=b;
	frgl_drawprim_clr[3]=a;
	
//	glColor4f(r, g, b, a);
}

BTEIFGL_API void frglColor4ub(int r, int g, int b, int a)
{
	frglColor4f(r/255.0, g/255.0, b/255.0, a/255.0);
}

BTEIFGL_API void frglTexCoord2f(float s, float t)
{
	frgl_drawprim_sttmp[0]=s;
	frgl_drawprim_sttmp[1]=t;
}

BTEIFGL_API void frglVertex2f(float x, float y)
{
	float tv[3];
	tv[0]=x; tv[1]=y; tv[2]=0;
	frglVertex3fv(tv);
}

BTEIFGL_API void frglVertex3f(float x, float y, float z)
{
	float tv[3];
	tv[0]=x; tv[1]=y; tv[2]=z;
	frglVertex3fv(tv);
}

BTEIFGL_API void frglNormal3f(float x, float y, float z)
{
	frgl_drawprim_ntmp[0]=x;
	frgl_drawprim_ntmp[1]=y;
	frgl_drawprim_ntmp[2]=z;
	frgl_drawprim_ntmp[3]=0;
}

//BTEIFGL_API void frglColorMask(bool r, bool g, bool b, bool a)
//{
//	glColorMask(r, g, b, a);
//}

BTEIFGL_API void frglEnableTexture2D(void)
{
	frglEnable(GL_TEXTURE_2D);
}

BTEIFGL_API void frglDisableTexture2D(void)
{
	frglDisable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, frgl_drawprim_whitetex);
}

BTEIFGL_API void frglBindTexture2D(int tex)
{
	frglBindTexture(GL_TEXTURE_2D, tex);
}

BTEIFGL_API void frglEnableLighting(void)
{
//	glEnable(GL_LIGHTING);
}

BTEIFGL_API void frglDisableLighting(void)
{
//	glDisable(GL_LIGHTING);
}

//BTEIFGL_API void frglEnable(int flag)
//{
//	glEnable(flag);
//}

//BTEIFGL_API void frglDisable(int flag)
//{
//	glDisable(flag);
//}


BTEIFGL_API void frglModelviewMatrix(void)
{
	frgl_drawprim_matrixmode=0;
//	glMatrixMode(GL_MODELVIEW);
}

BTEIFGL_API void frglProjectionMatrix(void)
{
	frgl_drawprim_matrixmode=1;
//	glMatrixMode(GL_PROJECTION);
}

BTEIFGL_API void frglLoadIdentity()
{
//	FRGL_DrawPrim_Init();
	Mat4F_Identity(frgl_drawprim_matrix[frgl_drawprim_matrixmode]);
//	glLoadIdentity();
}

BTEIFGL_API void frglPushMatrix(void)
{
	int i, j;

//	FRGL_DrawPrim_Init();

	if(frgl_drawprim_matrixstackpos[frgl_drawprim_matrixmode]<0)
	{
		Mat4F_Identity(frgl_drawprim_matrix[frgl_drawprim_matrixmode]);
		frgl_drawprim_matrixstackpos[frgl_drawprim_matrixmode]=0;
	}

	if(frgl_drawprim_matrixstackpos[frgl_drawprim_matrixmode]>=16)
	{
		*(int *)-1=-1;
	}

	i=frgl_drawprim_matrixstackpos[frgl_drawprim_matrixmode]++;
	for(j=0; j<16; j++)
		frgl_drawprim_matrixstack[frgl_drawprim_matrixmode][i][j]=
			frgl_drawprim_matrix[frgl_drawprim_matrixmode][j];

//	glPushMatrix();
}

BTEIFGL_API void frglPopMatrix(void)
{
	int i, j;

//	FRGL_DrawPrim_Init();

	i=--frgl_drawprim_matrixstackpos[frgl_drawprim_matrixmode];
	if(i<0)
	{
		Mat4F_Identity(frgl_drawprim_matrix[frgl_drawprim_matrixmode]);
		frgl_drawprim_matrixstackpos[frgl_drawprim_matrixmode]=0;
		return;
	}

	for(j=0; j<16; j++)
		frgl_drawprim_matrix[frgl_drawprim_matrixmode][j]=
			frgl_drawprim_matrixstack[frgl_drawprim_matrixmode][i][j];

//	glPopMatrix();
}

BTEIFGL_API void frglMultMatrixf(float *mat)
{
	float tmat[16];

	Mat4F_MatMultT(frgl_drawprim_matrix[frgl_drawprim_matrixmode], mat, tmat);
//	Mat4F_MatMult(frgl_drawprim_matrix[frgl_drawprim_matrixmode], mat, tmat);
	Mat4F_Copy(tmat, frgl_drawprim_matrix[frgl_drawprim_matrixmode]);

//	glMultMatrixf(mat);
}

BTEIFGL_API void frglTranslatef(float x, float y, float z)
{
	float tmat[16];
	Mat4F_Identity(tmat);
	tmat[12]=x; tmat[13]=y; tmat[14]=z;
	frglMultMatrixf(tmat);

//	glTranslatef(x, y, z);
}

BTEIFGL_API void frglScalef(float x, float y, float z)
{
	float tmat[16];

	Mat4F_Identity(tmat);
	tmat[0]=x; tmat[5]=y; tmat[10]=z;
	frglMultMatrixf(tmat);

//	glScalef(x, y, z);
}

BTEIFGL_API void frglRotatef(float th, float nx, float ny, float nz)
{
	float tv[4];
	float tmat[16];

	tv[0]=nx; tv[1]=ny; tv[2]=nz; tv[3]=0;
	Mat4F_AxisTo4MatrixT(tv, -th*(M_PI/180), tmat);
	frglMultMatrixf(tmat);

//	glRotatef(th, nx, ny, nz);
}

BTEIFGL_API void frglGetModelviewMatrix(float *v)
{
//	glGetFloatv(GL_MODELVIEW_MATRIX, v);
	Mat4F_Copy(frgl_drawprim_matrix[0], v);
}

BTEIFGL_API void frglGetProjectionMatrix(float *v)
{
//	glGetFloatv(GL_MODELVIEW_MATRIX, v);
	Mat4F_Copy(frgl_drawprim_matrix[1], v);
}

BTEIFGL_API float *frglGetModelviewMatrixPtr(void)
{
	return(frgl_drawprim_matrix[0]);
}

BTEIFGL_API float *frglGetProjectionMatrixPtr(void)
{
	return(frgl_drawprim_matrix[1]);
}

BTEIFGL_API void frglOrtho(
	float mx, float nx,
	float my, float ny,
	float mz, float nz)
{
	float mat[16];
	Mat4F_SetupOrtho(mat, mx, nx, my, ny, mz, nz);
	frglMultMatrixf(mat);

//	glOrtho(mx, nx, my, ny, mz, nz);
}

BTEIFGL_API void frglFrustum(
	float mx, float nx,
	float my, float ny,
	float mz, float nz)
{
	float mat[16];
	Mat4F_SetupFrustum(mat, mx, nx, my, ny, mz, nz);
	frglMultMatrixf(mat);

//	glFrustum(mx, nx, my, ny, mz, nz);
}

#endif

#ifndef FRGL_WRAP_IMM_GL
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

BTEIFGL_API void frglFrustum(
	float mx, float nx,
	float my, float ny,
	float mz, float nz)
{
	glFrustum(mx, nx, my, ny, mz, nz);
}

#endif
