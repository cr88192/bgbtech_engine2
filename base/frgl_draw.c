#include <bteifgl.h>

BTEIFGL_API int Draw_SetSolid_2D(
	float aspect, int xo, int yo, int xs, int ys)
{
//	pdgl_view_xo=xo;
//	pdgl_view_yo=yo;
//	pdgl_view_xs=xs;
//	pdgl_view_ys=ys;

	frglViewport(xo, yo, xs, ys);

//	frglMatrixMode(GL_PROJECTION);
	frglProjectionMatrix();
	frglLoadIdentity();
	frglOrtho(-1000, 1000, -1000/aspect, 1000/aspect, -99999, 99999);

//	frglMatrixMode(GL_MODELVIEW);
	frglModelviewMatrix();
	frglLoadIdentity();

	frglDisable(GL_DEPTH_TEST);
	frglDisable(GL_CULL_FACE);
	frglEnable(GL_BLEND);
	frglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frglEnable(GL_TEXTURE_2D);
	frglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	frglColor4f(1,1,1,1);
	return(0);
}

BTEIFGL_API int Draw_SetSolid2_2D(float aspect, float size,
	float *org, float *angles, int xo, int yo, int xs, int ys)
{
	float h2;

//	printf("%g %g %g\n", org[0], org[1], org[2]);
	h2=size/aspect;

//	pdgl_view_xo=xo;
//	pdgl_view_yo=yo;
//	pdgl_view_xs=xs;
//	pdgl_view_ys=ys;

	frglViewport(xo, yo, xs, ys);

//	frglMatrixMode(GL_PROJECTION);
	frglProjectionMatrix();
	frglLoadIdentity();

	frglOrtho(-size, size, -h2, h2, -999999999, 999999999);

	if(angles)
	{
		frglRotatef(-angles[2],  0, 0, 1);
		frglRotatef(-angles[0],  1, 0, 0);
		frglRotatef(-angles[1],  0, 1, 0);
	}
	if(org)
	{
		frglTranslatef(-org[0],  -org[1],  -org[2]);
	}

//	frglMatrixMode(GL_MODELVIEW);
	frglModelviewMatrix();
	frglLoadIdentity();

	frglDisable(GL_DEPTH_TEST);
	frglDisable(GL_CULL_FACE);
	frglDisable(GL_ALPHA_TEST);

	frglEnable(GL_BLEND);
	frglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	frglShadeModel(GL_SMOOTH);

	frglEnable(GL_TEXTURE_2D);
	frglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//	frglEnable(GL_STENCIL_TEST);
//	frglEnable(GL_SCISSOR_TEST);

	frglDisable(GL_STENCIL_TEST);
	frglDisable(GL_SCISSOR_TEST);

	frglColor4f(1.0,1.0,1.0,1.0);
	return(0);
}


BTEIFGL_API int Draw_SetOrtho_3D(float aspect, float size,
	float *org, float *angles, int x, int y, int w, int h)
{
	float h2;

//	pdgl_view_xo=x;
//	pdgl_view_yo=y;
//	pdgl_view_xs=w;
//	pdgl_view_ys=h;

	frglViewport(x, y, w, h);

//	frglMatrixMode(GL_PROJECTION);
	frglProjectionMatrix();
	frglLoadIdentity();

	h2=size/aspect;
	frglOrtho(-size, size, -h2, h2, -999999999, 999999999);

//	if(leftcoords)
//	frglScalef(1, 1, -1);

	if(angles)
	{
		frglRotatef(-angles[1],  0, 1, 0);
		frglRotatef(-angles[0],  1, 0, 0);
		frglRotatef(-angles[2],  0, 0, 1);
	}
	if(org)
	{
		frglTranslatef(-org[0],  -org[1],  -org[2]);
	}

	frglDisable(GL_DEPTH_TEST);
	frglDisable(GL_CULL_FACE);
	frglEnable(GL_BLEND);
	frglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frglDisable(GL_TEXTURE_2D);
	frglPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	frglColor4f(0,0,0,1);

//	frglMatrixMode(GL_MODELVIEW);
	frglModelviewMatrix();
	frglLoadIdentity();
	return(0);
}

BTEIFGL_API int Draw_SetSolid3_2D(float lxs, float lys,
	float *org, int xo, int yo, int xs, int ys)
{
//	pdgl_view_xo=xo;
//	pdgl_view_yo=yo;
//	pdgl_view_xs=xs;
//	pdgl_view_ys=ys;

	frglViewport(xo, yo, xs, ys);

//	frglMatrixMode(GL_PROJECTION);
	frglProjectionMatrix();
	frglLoadIdentity();

	frglOrtho(0, lxs, 0, lys, 0, 999999999);
	if(org)
	{
		frglTranslatef(-org[0],  -org[1],  -org[2]);
	}

//	frglMatrixMode(GL_MODELVIEW);
	frglModelviewMatrix();
	frglLoadIdentity();

	frglEnable(GL_DEPTH_TEST);
	frglDisable(GL_CULL_FACE);
	frglDisable(GL_ALPHA_TEST);

	frglDepthFunc(GL_LEQUAL);

	frglEnable(GL_BLEND);
	frglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	frglShadeModel(GL_SMOOTH);

	frglEnable(GL_TEXTURE_2D);
	frglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//	frglEnable(GL_STENCIL_TEST);
//	frglEnable(GL_SCISSOR_TEST);

	frglColor4f(1,1,1,1);

	frglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
		GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	return(0);
}

BTEIFGL_API int Draw_SetSolid4_2D(
	float *mins, float *maxs,
	float *org, float *ang, int xo, int yo, int xs, int ys)
{
//	pdgl_view_xo=xo;
//	pdgl_view_yo=yo;
//	pdgl_view_xs=xs;
//	pdgl_view_ys=ys;

	frglViewport(xo, yo, xs, ys);

//	frglMatrixMode(GL_PROJECTION);
	frglModelviewMatrix();
	frglLoadIdentity();

	frglOrtho(mins[0], maxs[0], mins[1], maxs[1], mins[2], maxs[2]);

	if(ang)
	{
		frglRotatef(-ang[1],  0, 1, 0);
		frglRotatef(-ang[0],  1, 0, 0);
		frglRotatef(-ang[2],  0, 0, 1);
	}
	if(org)
	{
		frglTranslatef(-org[0],  -org[1],  -org[2]);
	}

//	frglMatrixMode(GL_MODELVIEW);
	frglModelviewMatrix();
	frglLoadIdentity();

	frglEnable(GL_DEPTH_TEST);
//	frglDisable(GL_CULL_FACE);
	frglDisable(GL_ALPHA_TEST);

	frglEnable(GL_CULL_FACE);
	frglCullFace(GL_FRONT);
	frglFrontFace(GL_CCW);

	frglDepthFunc(GL_LEQUAL);

	frglEnable(GL_BLEND);
	frglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	frglShadeModel(GL_SMOOTH);

	frglEnable(GL_TEXTURE_2D);
	frglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//	frglEnable(GL_STENCIL_TEST);
//	frglEnable(GL_SCISSOR_TEST);

	frglColor4f(1,1,1,1);

	frglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
		GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	return(0);
}

BTEIFGL_API int Draw_SetPerspective_3D(
	float aspect, float fov, float d,
	float *org, float *angles, int xo, int yo, int xs, int ys)
{
	GLdouble xmin, xmax, ymin, ymax, dst;
	float pos2[16];

//	pdgl_view_xo=xo;
//	pdgl_view_yo=yo;
//	pdgl_view_xs=xs;
//	pdgl_view_ys=ys;

	frglViewport(xo, yo, xs, ys);

//	frglMatrixMode(GL_PROJECTION);
	frglProjectionMatrix();
	frglLoadIdentity();

//	if(leftcoords)

//	dst=0.1;
	dst=0.001;

	xmax=dst*tan(fov*(M_PI/360.0));
	xmin=-xmax;

	ymin=xmin/aspect;
	ymax=xmax/aspect;

	frglFrustum(xmin, xmax, ymin, ymax, dst, 10000);
//	frglFrustum(xmin, xmax, ymin, ymax, dst, 100000);
//	frglFrustum(xmin, xmax, ymin, ymax, dst, 100000000);
//	frglFrustum(xmin, xmax, ymin, ymax, dst, 1000000);

//	frglScalef(1, 1, -1);

	frglTranslatef(0, 0, -d);

	if(angles)
	{
		frglRotatef(-angles[1],  0, 1, 0);
		frglRotatef(-angles[0],  1, 0, 0);
		frglRotatef(-angles[2],  0, 0, 1);
	}
	if(org)
	{
		frglTranslatef(-org[0],  -org[1],  -org[2]);
	}

//	MatN_SwapOrder(pos, pos2, 4);
//	frglMultMatrixd(pos2);

//	frglMatrixMode(GL_MODELVIEW);
	frglModelviewMatrix();
	frglLoadIdentity();

	frglEnable(GL_DEPTH_TEST);
	frglDepthFunc(GL_LESS);

	frglEnable(GL_CULL_FACE);
	frglCullFace(GL_FRONT);
	frglFrontFace(GL_CCW);

	frglDisable(GL_SCISSOR_TEST);
	frglDisable(GL_ALPHA_TEST);

//	frglDisable(GL_CULL_FACE);

	frglEnable(GL_BLEND);
	frglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	frglShadeModel(GL_SMOOTH);

	frglEnable(GL_TEXTURE_2D);
	frglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//	frglColor4f(0,0,0,1);
	frglColor4f(1,1,1,1);

//	frglHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	return(0);
}

BTEIFGL_API int Draw_SetPerspective2_3D(
	float aspect, float fov, float width,
	float *org, float *rot, int xo, int yo, int xs, int ys)
{
	GLdouble xmin, xmax, ymin, ymax, dst;
	float pos[16];

//	pdgl_view_xo=xo;
//	pdgl_view_yo=yo;
//	pdgl_view_xs=xs;
//	pdgl_view_ys=ys;

	frglViewport(xo, yo, xs, ys);

//	frglMatrixMode(GL_PROJECTION);
	frglProjectionMatrix();
	frglLoadIdentity();

//	dst=0.1;
	dst=0.001;

//	xmax=dst*tan(fov*(M_PI/360.0));
//	xmin=-xmax;

	xmax=width/2;
	xmin=-xmax;

	ymin=xmin/aspect;
	ymax=xmax/aspect;

	dst=xmax/tan(fov*(M_PI/360.0));
//	dst=dst*0.25;

	frglFrustum(xmin, xmax, ymin, ymax, dst, 10000);
//	frglFrustum(xmin, xmax, ymin, ymax, dst, 100000);
//	frglFrustum(xmin, xmax, ymin, ymax, dst, 100000000);

//	frglScalef(1, 1, -1);
	frglTranslatef(0, 0, -dst);
//	frglTranslatef(0, 0, -2*dst);
//	frglTranslatef(0, 0, -dst*0.5);

//	Mat3to4F_Transpose(rot, pos);

	frglRotatef(-90,  1, 0, 0);

	if(rot)
	{
		pos[0]=rot[0]; 	pos[1]=rot[3];	pos[2]=rot[6];	pos[3]=0;
		pos[4]=rot[1];	pos[5]=rot[4];	pos[6]=rot[7];	pos[7]=0;
		pos[8]=rot[2];	pos[9]=rot[5];	pos[10]=rot[8];	pos[11]=0;
		pos[12]=0;	pos[13]=0;	pos[14]=0;	pos[15]=1;

		frglMultMatrixf(pos);
	}

	if(org)
	{
		frglTranslatef(-org[0],  -org[1],  -org[2]);
	}


//	frglMatrixMode(GL_MODELVIEW);
	frglModelviewMatrix();
	frglLoadIdentity();

	frglEnable(GL_DEPTH_TEST);
	frglDepthFunc(GL_LESS);

	frglEnable(GL_CULL_FACE);
	frglCullFace(GL_FRONT);
	frglFrontFace(GL_CCW);

	frglDisable(GL_SCISSOR_TEST);
	frglDisable(GL_ALPHA_TEST);

//	frglDisable(GL_CULL_FACE);

	frglEnable(GL_BLEND);
	frglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	frglShadeModel(GL_SMOOTH);

	frglEnable(GL_TEXTURE_2D);
	frglPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//	frglColor4f(0,0,0,1);
	frglColor4f(1,1,1,1);

//	frglHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	return(0);
}

BTEIFGL_API void FRGL_Setup2D()
{
	float f;
//	UI_Camera_Init();
//	Draw_SetSolid2_2D(4.0/3.0, 400,
//		VEC3(0, 0, 0), VEC3(0, 0, 0), 0, 0,
//		pdglui_state->xs, pdglui_state->ys);

	f=((float)frgl_state->xs)/frgl_state->ys;
	Draw_SetSolid2_2D(f, frgl_state->xs/2,
		NULL, NULL, 0, 0,
		frgl_state->xs, frgl_state->ys);
}

BTEIFGL_API void FRGL_Setup3D(float *org, float *rot)
{
	float f;
//	UI_Camera_Init();
//	Draw_SetSolid2_2D(4.0/3.0, 400,
//		VEC3(0, 0, 0), VEC3(0, 0, 0), 0, 0,
//		pdglui_state->xs, pdglui_state->ys);

	f=((float)frgl_state->xs)/frgl_state->ys;
//	Draw_SetSolid2_2D(f, frgl_state->xs/2,
//		NULL, NULL, 0, 0,
//		frgl_state->xs, frgl_state->ys);

//	Draw_SetPerspective2_3D(
//		f, 90, 1, org, rot,
//		0, 0, frgl_state->xs, frgl_state->ys);

	Draw_SetPerspective2_3D(
		f, 90, 0.75, org, rot,
		0, 0, frgl_state->xs, frgl_state->ys);
}
