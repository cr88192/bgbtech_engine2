#include <bteifgl.h>

#ifndef GLES

#include <GL/gl.h>
#include <GL_3/glext.h>

#ifdef _WIN32
#include <GL_3/wglext.h>
#endif

#endif

//allows small textures to be grouped into bigger ones
#define TEXBLOCKS	16
#define TEXBLOCK_SZ	1024
#define TEXBLOCK_DIV	64
static byte *texblock[TEXBLOCKS];
static byte texblockfl[TEXBLOCKS*64*64];
static int texblocknum[TEXBLOCKS];

#define FRGL_MAX_TEXTURES 16384
unsigned char texturebm[FRGL_MAX_TEXTURES/8];
int lasttexture;
short tex_width[FRGL_MAX_TEXTURES], tex_height[FRGL_MAX_TEXTURES];
byte *tex_buffer[FRGL_MAX_TEXTURES];
char *tex_name[FRGL_MAX_TEXTURES];
int tex_chain[FRGL_MAX_TEXTURES];
int tex_hash[4096];			//texture chain hash
int tex_hash2[4096];		//texture hint hash

int tex_screen=-1;

//elem gl_texfilter_var=MISC_NULL;
char *gl_texfilter_var=NULL;

void *tex_mutex=NULL;

typedef struct
{
char *name;
int	min, mag;
}pdgl_texmode;

pdgl_texmode texmodes[]=
{
	{"GL_NEAREST", GL_NEAREST, GL_NEAREST},
	{"GL_LINEAR", GL_LINEAR, GL_LINEAR},
	{"GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST},
	{"GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR},
	{"GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST},
	{"GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR},
	{"GL_NEAREST_MIPMAP_LINEAR_2", GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR},
	{NULL, 0, 0}
};

BTEIFGL_API int Tex_GetModeinfo(int *min, int *mag)
{
	int i;

	*min=GL_NEAREST;
	*mag=GL_NEAREST;

//	for(i=0; i<6; i++)
	for(i=0; texmodes[i].name; i++)
		if(!strcmp(gl_texfilter_var, texmodes[i].name))
	{
		*min=texmodes[i].min;
		*mag=texmodes[i].mag;
	}
	return(0);
}

void tex_lock()
{
	thLockFastMutex(tex_mutex);
}

void tex_unlock()
{
	thUnlockFastMutex(tex_mutex);
}

BTEIFGL_API int Tex_Init2()
{
	int i;

	printf("FRGL: Textures Init\n");
//	gl_texfilter_var=Var_SetString("gl_texfilter", "GL_LINEAR_MIPMAP_LINEAR");

	tex_mutex=thFastMutex();

	for(i=0; i<4096; i++)
	{
		tex_hash[i]=-1;
		tex_hash2[i]=-1;
	}

	gl_texfilter_var=FRGL_CvarGet("gl_texfilter");
	
	if(!gl_texfilter_var)
	{
#ifdef WIN32
		gl_texfilter_var="GL_LINEAR_MIPMAP_LINEAR";
#else
		gl_texfilter_var="GL_LINEAR_MIPMAP_LINEAR";
//		gl_texfilter_var="GL_NEAREST_MIPMAP_NEAREST";
//		gl_texfilter_var="GL_NEAREST";
#endif
	}

//	gl_texfilter_var="GL_LINEAR_MIPMAP_LINEAR";
//	gl_texfilter_var="GL_NEAREST";

	memset(texblockfl, 0, TEXBLOCKS*64*64);

//	memset(texturebm, 0, FRGL_MAX_TEXTURES/8);
	lasttexture=1;
	return(0);
}

BTEIFGL_API int Tex_AllocTexnum()
{
	int i;

	i=lasttexture+1;

//	glGenTextures(1, &i);
//	if((i%FRGL_MAX_TEXTURES)!=i)
//		return(i);

	while(1)
	{
		if(!(texturebm[i>>3]&(1<<(i&7))))
		{
			texturebm[i>>3]|=1<<(i&7);
			lasttexture=i;
			return(i);
		}
//		i++;

		i=(i+1)%FRGL_MAX_TEXTURES;

		if(i==lasttexture)
			return(-1);
	}
}

BTEIFGL_API int Tex_FreeTexnum(int n)
{
	texturebm[n>>3]&=~(1<<(n&7));
	return(0);
}

BTEIFGL_API int Tex_MarkTexnum(int n)
{
	texturebm[n>>3]|=(1<<(n&7));
	return(0);
}

BTEIFGL_API int Tex_DeleteTexture(int n)
{
	glDeleteTextures(1, &n);
	texturebm[n>>3]&=~(1<<(n&7));
	return(0);
}

BTEIFGL_API int Tex_FindFragment(int xs, int ys, int *x, int *y, int *z)
{
	int i, j,  k, l, m, n;
	int xf, yf;

	xf=(xs+15)/16;
	yf=(ys+15)/16;

	for(i=0; i<TEXBLOCKS; i++)
		for(k=0; k<64; k++)
	{
		if((k+yf)>64)break;
		for(l=0; l<64; l++)
		{
			if((l+xf)>64)break;

			j=0;
			for(m=0; m<yf; m++)for(n=0; n<xf; n++)
			{
				if(texblockfl[(i*64*64)+((k+m)*64)+(l+n)])
					j=1;
			}

			if(!j)
			{
				*x=l;
				*y=k;
				*z=i;
				return(0);
			}
		}
	}
	return(-1);
}

BTEIFGL_API int Tex_LockFragment(int x, int y, int z, int xs, int ys)
{
	int i, j;

	for(i=0; i<ys; i++)for(j=0; j<xs; j++)
		texblockfl[(z*64*64)+((y+i)*64)+(x+j)]=1;
	return(0);
}

BTEIFGL_API int Tex_AllocFragment(int xs, int ys, int *x, int *y, int *z)
{
	int xf, yf;
	int i;

	xf=(xs+15)/16;
	yf=(ys+15)/16;

	i=Tex_FindFragment(xs, ys, x, y, z);
	if(i<0)return(-1);
	Tex_LockFragment(*x, *y, *z, xf, yf);
	return(0);
}

BTEIFGL_API int Tex_LoadTextureFragment(int xs, int ys, byte *buf,
	float *stmin, float *stmax)
{
	int x, y, z;
	int xo, yo;
	int i, j, k;

	i=Tex_AllocFragment(xs, ys, &x, &y, &z);
	if(i<0)return(-1);

	if(!texblock[z])
	{
		texblock[z]=malloc(1024*1024*4);
		memset(texblock[z], 0, 1024*1024*4);
		texblocknum[z]=Tex_AllocTexnum();
	}

	xo=x*16;
	yo=y*16;

	for(i=0; i<ys; i++)for(j=0; j<xs; j++)for(k=0; k<4; k++)
		texblock[z][((yo+i)*1024+(xo+j))*4+k]=buf[(i*xs+j)*4+k];

	Tex_LoadTexture2(1024, 1024, texblock[z], 1, texblocknum[z], 0);

	stmin[0]=(x/64.0);
	stmin[1]=(y/64.0);
	stmax[0]=((x*16+xs)/1024.0);
	stmax[1]=((y*16+ys)/1024.0);

	return(texblocknum[z]);
}


#if 1

//faster but uglier
BTEIFGL_API int Tex_ResampleQuick
	(int *src, int iw, int ih, int *dst, int ow, int oh) //AH:ignore
{
	int i, j, ik, ix, iy;
	float xs, ys, xc, yc;

//	return(0);

	xs=((float)iw/(float)ow);
	ys=((float)ih/(float)oh);
	xc=0;
	yc=0;

	for(i=0; i<oh; i++)
	{
		iy=yc;
		ik=iy*iw;
		for(j=0; j<ow; j++)
		{
			ix=xc;
			*dst=src[ik+ix];

			dst++;
			xc+=xs;
		}

		xc=0;
		yc+=ys;
	}
	return(0);
}

#endif

BTEIFGL_API int Tex_Resample(byte *src, int iw, int ih, byte *dst, int ow, int oh)
{
	float xs, ys, xc, yc;
	float ix, iy, fx, fy, fxn, fyn;
	float fxya, fxyb, fxyc, fxyd;
	int i, j, k, l, ik, il;
	int la, lb, lc, ld, iw4;

//	return(0);

	xs=((float)iw/(float)ow);
	ys=((float)ih/(float)oh);
	iw4=iw*4;

	for(i=0; i<oh; i++)
	{
		iy=i*ys;
		ik=iy; fy=iy-ik; fyn=1-fy;

		for(j=0; j<ow; j++)
		{
			ix=j*xs;
			il=ix; fx=ix-il;
			l=(ik*iw+il)*4;
			fxn=1-fx;

			fxya=fxn*fyn;	fxyb=fx*fyn;
			fxyc=fxn*fy;	fxyd=fx*fy;
			la=l;			lb=l+4;
			lc=l+iw4;		ld=l+iw4+4;

			for(k=0; k<4; k++)
				dst[k]=
					src[la+k]*fxya+
					src[lb+k]*fxyb+
					src[lc+k]*fxyc+
					src[ld+k]*fxyd;
			dst+=4;
		}
	}
	return(0);
}


BTEIFGL_API int Tex_ResampleMono(byte *src, int iw, int ih,
	byte *dst, int ow, int oh)
{
	int i, j, ik, ix, iy;
	float xs, ys, xc, yc;

	xs=((float)iw/(float)ow);
	ys=((float)ih/(float)oh);
	xc=0;
	yc=0;

	for(i=0; i<oh; i++)
	{
		iy=yc;
		ik=iy*iw;
		for(j=0; j<ow; j++)
		{
			ix=xc;
			*dst=src[ik+ix];

			dst++;
			xc+=xs;
		}

		xc=0;
		yc+=ys;
	}
	return(0);
}


static void Tex_ScaleSpan(int *src, int iw, int *dst, int ow)
{
	int i, xc;

	if(ow>iw)
	{
		xc=ow;
		for(i=0; i<iw; i++)
		{
			*dst++=*src++;

			xc-=iw;
			while(xc<=iw)
			{
				*dst++=*src;
				xc+=ow;
			}
		}
	}else
	{
		xc=iw;
		for(i=0; i<iw; i++)
		{
			*dst++=*src++;

			xc-=ow;
			while(xc<=ow)
			{
				src++;
				xc+=iw;
			}
		}
	}
}

static void Tex_Resample2(
	int *src, int iw, int ih,
	int *dst, int ow, int oh)
{
	int i, yc;

	yc=oh;
	for(i=0; i<ih; i++)
	{
		Tex_ScaleSpan(src, iw, dst, ow);
//		LBXGL_ScaleSpan32(src, iw, dst, ow);
		src+=iw;
		dst+=ow;
		yc-=ih;

		if(yc<=ih)
		{
			Tex_ScaleSpan(src, iw, dst, ow);
//			LBXGL_ScaleSpan32(src, iw, dst, ow);
			dst+=ow;

			yc+=oh;
		}
	}
}

static void Tex_Resample2DN(
	int *src, int iw, int ih,
	int *dst, int ow, int oh)
{
	int i, yc;

	yc=ih;
	for(i=0; i<oh; i++)
	{
		Tex_ScaleSpan(src, iw, dst, ow);
//		LBXGL_ScaleSpan32(src, iw, dst, ow);
		src+=iw;
		dst+=ow;

//		yc-=oh;
		if(yc<=oh)
		{
			src+=iw;
			yc+=ih;
		}
	}
}

BTEIFGL_API int Tex_PadResample(int *src, int iw, int ih,
	int *dst, int ow, int oh)
{
	int i, j;

	for(i=0; i<ih; i++)
	{
		memcpy(dst, src, iw*4);
		src+=iw;
		dst+=ow;
	}
	return(0);
}

#if 1
BTEIFGL_API int Tex_HalfSample(byte *src, int w, int h)
{
	int w2, h2, i2, j2, k2;
	int a1, a2, a3, a4;
	int af1, af2, af3, af4;
	int i, j, k;

	w2=w>>1;
	h2=h>>1;
	for(i=0;i<h2; i++)
		for(j=0; j<w2; j++)
	{
		i2=i<<1;
		j2=j<<1;

		a1=src[((i2*w+j2)<<2)+3];
		a2=src[((i2*w+(j2+1))<<2)+3];
		a3=src[(((i2+1)*w+j2)<<2)+3];
		a4=src[(((i2+1)*w+(j2+1))<<2)+3];

		k=a1+a2+a3+a4;
		if(!k || (k==1020))
		{
			af1=1024;
			af2=1024;
			af3=1024;
			af4=1024;
		}else
		{
			k2=262144/k;
			af1=(k2*a1+7)>>6;
			af2=(k2*a2+7)>>6;
			af3=(k2*a3+7)>>6;
			af4=(k2*a4+7)>>6;

//			af1=(4096*a1)/k;
//			af2=(4096*a2)/k;
//			af3=(4096*a3)/k;
//			af4=(4096*a4)/k;
		}
		
		src[((i*w2+j)<<2)+0]=
			(src[((i2*w+j2)<<2)+0]*af1+
			src[((i2*w+(j2+1))<<2)+0]*af2+
			src[(((i2+1)*w+j2)<<2)+0]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+0]*af4)>>12;

		src[((i*w2+j)<<2)+1]=
			(src[((i2*w+j2)<<2)+1]*af1+
			src[((i2*w+(j2+1))<<2)+1]*af2+
			src[(((i2+1)*w+j2)<<2)+1]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+1]*af4)>>12;

		src[((i*w2+j)<<2)+2]=
			(src[((i2*w+j2)<<2)+2]*af1+
			src[((i2*w+(j2+1))<<2)+2]*af2+
			src[(((i2+1)*w+j2)<<2)+2]*af3+
			src[(((i2+1)*w+(j2+1))<<2)+2]*af4)>>12;

		src[((i*w2+j)<<2)+3]=
			(src[((i2*w+j2)<<2)+3]+		src[((i2*w+(j2+1))<<2)+3]+
			src[(((i2+1)*w+j2)<<2)+3]+	src[(((i2+1)*w+(j2+1))<<2)+3])>>2;
	}
	return(0);
}
#endif

BTEIFGL_API int Tex_HalfSample2(byte *src, int w, int h)
{
	int i, j, k, w2, h2, i2, j2;

	w2=w>>1;
	h2=h>>1;
	for(i=0;i<h2; i++)
		for(j=0; j<w2; j++)
	{
		i2=i<<1;
		j2=j<<1;

		k=	(src[((i2*w+j2)<<2)+0]+		src[((i2*w+(j2+1))<<2)+0]+
			src[(((i2+1)*w+j2)<<2)+0]+	src[(((i2+1)*w+(j2+1))<<2)+0]);
		if(k>255)k=255;
		src[((i*w2+j)<<2)+0]=k;

		k=	(src[((i2*w+j2)<<2)+1]+		src[((i2*w+(j2+1))<<2)+1]+
			src[(((i2+1)*w+j2)<<2)+1]+	src[(((i2+1)*w+(j2+1))<<2)+1]);
		if(k>255)k=255;
		src[((i*w2+j)<<2)+1]=k;

		k=	(src[((i2*w+j2)<<2)+2]+		src[((i2*w+(j2+1))<<2)+2]+
			src[(((i2+1)*w+j2)<<2)+2]+	src[(((i2+1)*w+(j2+1))<<2)+2]);
		if(k>255)k=255;
		src[((i*w2+j)<<2)+2]=k;

		k=	(src[((i2*w+j2)<<2)+3]+		src[((i2*w+(j2+1))<<2)+3]+
			src[(((i2+1)*w+j2)<<2)+3]+	src[(((i2+1)*w+(j2+1))<<2)+3]);
		if(k>255)k=255;
		src[((i*w2+j)<<2)+3]=k;
	}
	return(0);
}

BTEIFGL_API double Tex_Sinc(double x)
{
	if(x==0)return(1);
	return(sin(M_PI*x)/(M_PI*x));
}

BTEIFGL_API double Tex_Lanczos(double x, double a)
{
	if(x==0)return(1);
	if(x>=(a))return(0);
	if(x<=(-a))return(0);
	return(a*sin(M_PI*x)*sin((M_PI/a)*x)/(M_PI*M_PI*x*x));
}

BTEIFGL_API int Tex_SincPolateRGBA8(byte *src, int w, int h,
	float x, float y, float *rgba, int n)
{
	double f, g, tr, tg, tb, ta;
	int i, j, k, wsx, wex, wsy, wey;

	i=floor(x); wsx=i-n; wex=i+n;
	if(wsx<0)wsx=0; if(wsx>w)wsx=w;
	if(wex<0)wex=0; if(wex>w)wex=w;

	i=floor(y); wsy=i-n; wey=i+n;
	if(wsy<0)wsy=0; if(wsy>h)wsy=h;
	if(wey<0)wey=0; if(wey>h)wey=h;

	tr=0; tg=0; tb=0; ta=0;
	for(i=wsy; i<wey; i++)
		for(j=wsx; j<wex; j++)
	{
//		p+=samp[i]*BGBMID_Sinc(f-i);

//		f=Tex_Sinc(x-j);
//		g=Tex_Sinc(y-i);
//		f*=g;

//		f=Tex_Sinc(x-j) * Tex_Lanczos(x-j, n);
//		g=Tex_Sinc(y-i) * Tex_Lanczos(y-i, n);
//		f*=g;

		f=x-j; g=y-i; f=sqrt(f*f+g*g);
//		f=Tex_Sinc(f);
		f=Tex_Sinc(f) * Tex_Lanczos(f, n);
//		f=Tex_Lanczos(f, n);
//		f=Tex_Sinc(f)*cos(M_PI*f/n);

		k=(i*w+j)*4;
		tr+=src[k+0]*f;
		tg+=src[k+1]*f;
		tb+=src[k+2]*f;
		ta+=src[k+3]*f;
	}

	rgba[0]=tr;
	rgba[1]=tg;
	rgba[2]=tb;
	rgba[3]=ta;

	return(0);
}

BTEIFGL_API int Tex_ResampleSinc(byte *src, int iw, int ih,
	byte *dst, int ow, int oh)
{
	float tv[4];
	float xs, ys, xc, yc;
	float ix, iy, fx, fy;
	int i, j, k, l, ik, il;

//	return(0);

	xs=((float)iw/(float)ow);
	ys=((float)ih/(float)oh);

	for(i=0; i<oh; i++)
		for(j=0; j<ow; j++)
	{
		Tex_SincPolateRGBA8(src, iw, ih, j*xs, i*ys, tv, 16);
//		Tex_SincPolateRGBA8(src, iw, ih, j*xs, i*ys, tv, 4);

		k=tv[0]; if(k<0)k=0; if(k>255)k=255; dst[0]=k;
		k=tv[1]; if(k<0)k=0; if(k>255)k=255; dst[1]=k;
		k=tv[2]; if(k<0)k=0; if(k>255)k=255; dst[2]=k;
		k=tv[3]; if(k<0)k=0; if(k>255)k=255; dst[3]=k;

		dst+=4;
	}
	return(0);
}

BTEIFGL_API int Tex_SplinePolateRGBA8(byte *src, int w, int h,
	float x, float y, float *rgba)
{
	float tv[16], tva[4], tvb[4], tvc[4], tvd[4];
	double f, g, xf, yf, xg, yg;
	int i, j, k, l, xi, yi;
	int xi0, xi1, xi2, xi3;
	int yi0, yi1, yi2, yi3;

//	if(x<0)x=0; if(x>w)x=w;
//	if(y<0)y=0; if(y>h)y=h;

	xi=floor(x); yi=floor(y);
	xf=x-xi; yf=y-yi; xg=1-xf; yg=1-yf;

#if 0
	if((xi<1) || (xi>=(w-2)) || (yi<1) || (yi>=(h-2)))
	{
		if((xi>=(w-1)) || (yi>=(h-1)))
		{
			//far border, LERP not possible

			if((xi>=(w-1)) && (yi>=(h-1)))
			{
				k=((y-1)*w+(x-1))*4;
				rgba[0]=src[k+0]; rgba[1]=src[k+1];
				rgba[2]=src[k+2]; rgba[3]=src[k+3];
				return(0);
				//lowest corner
			}

			//at borders, use nearest pixel
			//FIX: could use H/V split here
			i=xi; j=yi;
			if(xi>=w)xi=w-1;
			if(yi>=y)yi=h-1;

			k=(j*w+i)*4;
			rgba[0]=src[k+0]; rgba[1]=src[k+1];
			rgba[2]=src[k+2]; rgba[3]=src[k+3];
			return(0);
		}

		//near edges, use LERP
		for(k=0; k<4; k++)
		{
			tv[0]=src[((yi  )*w+xi  )*4+k];
			tv[1]=src[((yi  )*w+xi+1)*4+k];
			tv[2]=src[((yi+1)*w+xi  )*4+k];
			tv[3]=src[((yi+1)*w+xi+1)*4+k];

			rgba[k]=tv[0]*xg*yg+tv[1]*xf*yg+
				tv[2]*xg*yf+tv[3]*xf*yf;
		}
		return(0);
	}

	for(k=0; k<4; k++)
	{
		tv[ 0]=src[((yi-1)*w+xi-1)*4+k];
		tv[ 1]=src[((yi-1)*w+xi  )*4+k];
		tv[ 2]=src[((yi-1)*w+xi+1)*4+k];
		tv[ 3]=src[((yi-1)*w+xi+2)*4+k];
		tv[ 4]=src[((yi  )*w+xi-1)*4+k];
		tv[ 5]=src[((yi  )*w+xi  )*4+k];
		tv[ 6]=src[((yi  )*w+xi+1)*4+k];
		tv[ 7]=src[((yi  )*w+xi+2)*4+k];
		tv[ 8]=src[((yi+1)*w+xi-1)*4+k];
		tv[ 9]=src[((yi+1)*w+xi  )*4+k];
		tv[10]=src[((yi+1)*w+xi+1)*4+k];
		tv[11]=src[((yi+1)*w+xi+2)*4+k];
		tv[12]=src[((yi+2)*w+xi-1)*4+k];
		tv[13]=src[((yi+2)*w+xi  )*4+k];
		tv[14]=src[((yi+2)*w+xi+1)*4+k];
		tv[15]=src[((yi+2)*w+xi+2)*4+k];

		tva[0]=tv[ 1]+xf*(tv[ 1]-tv[ 0]);
		tva[1]=tv[ 5]+xf*(tv[ 5]-tv[ 4]);
		tva[2]=tv[ 9]+xf*(tv[ 9]-tv[ 8]);
		tva[3]=tv[13]+xf*(tv[13]-tv[12]);

		tvb[0]=tv[ 2]+xg*(tv[ 2]-tv[ 3]);
		tvb[1]=tv[ 6]+xg*(tv[ 6]-tv[ 7]);
		tvb[2]=tv[10]+xg*(tv[10]-tv[11]);
		tvb[3]=tv[14]+xg*(tv[14]-tv[15]);

		tvc[0]=tv[ 4]+yf*(tv[ 4]-tv[ 0]);
		tvc[1]=tv[ 5]+yf*(tv[ 5]-tv[ 1]);
		tvc[2]=tv[ 6]+yf*(tv[ 6]-tv[ 2]);
		tvc[3]=tv[ 7]+yf*(tv[ 7]-tv[ 3]);

		tvd[0]=tv[ 8]+yg*(tv[ 8]-tv[12]);
		tvd[1]=tv[ 9]+yg*(tv[ 9]-tv[13]);
		tvd[2]=tv[10]+yg*(tv[10]-tv[14]);
		tvd[3]=tv[11]+yg*(tv[11]-tv[15]);


		tva[0]=tva[0]*xg+tvb[0]*xf;
		tva[1]=tva[1]*xg+tvb[1]*xf;
		tva[2]=tva[2]*xg+tvb[2]*xf;
		tva[3]=tva[3]*xg+tvb[3]*xf;

		tvb[0]=tvc[0]*yg+tvd[0]*yf;
		tvb[1]=tvc[1]*yg+tvd[1]*yf;
		tvb[2]=tvc[2]*yg+tvd[2]*yf;
		tvb[3]=tvc[3]*yg+tvd[3]*yf;

		tvc[0]=tva[1]+yf*(tva[1]-tva[0]);
		tvc[1]=tva[2]+yg*(tva[2]-tva[3]);
		tvc[2]=tvb[1]+xf*(tvb[1]-tvb[0]);
		tvc[3]=tvb[2]+xg*(tvb[2]-tvb[3]);

//		f=xf-0.5; g=yf-0.5;
//		g=1.0-(xf*xf+yf*yf);

		f=(tvc[0]*yg+tvc[1]*yf+
		   tvc[2]*xg+tvc[3]*xf)*0.5;

		rgba[k]=f;


//		f=tva[1]+yf*(tva[1]-tva[0]);
//		g=tva[2]+yg*(tva[2]-tva[3]);
//		rgba[k]=f*yg+g*yf;
	}
#endif

	xi0=xi-1; xi1=xi; xi2=xi+1; xi3=xi+2;
	yi0=yi-1; yi1=yi; yi2=yi+1; yi3=yi+2;

	if((xi<1) || (xi>=(w-2)) || (yi<1) || (yi>=(h-2)))
	{
		while(xi0<0)xi0+=w; while(xi0>=w)xi0-=w;
		while(xi1<0)xi1+=w; while(xi1>=w)xi1-=w;
		while(xi2<0)xi2+=w; while(xi2>=w)xi2-=w;
		while(xi3<0)xi3+=w; while(xi3>=w)xi3-=w;

		while(yi0<0)yi0+=h; while(yi0>=h)yi0-=h;
		while(yi1<0)yi1+=h; while(yi1>=h)yi1-=h;
		while(yi2<0)yi2+=h; while(yi2>=h)yi2-=h;
		while(yi3<0)yi3+=h; while(yi3>=h)yi3-=h;
	}

	for(k=0; k<4; k++)
	{
		tv[ 0]=src[(yi0*w+xi0)*4+k];
		tv[ 1]=src[(yi0*w+xi1)*4+k];
		tv[ 2]=src[(yi0*w+xi2)*4+k];
		tv[ 3]=src[(yi0*w+xi3)*4+k];
		tv[ 4]=src[(yi1*w+xi0)*4+k];
		tv[ 5]=src[(yi1*w+xi1)*4+k];
		tv[ 6]=src[(yi1*w+xi2)*4+k];
		tv[ 7]=src[(yi1*w+xi3)*4+k];
		tv[ 8]=src[(yi2*w+xi0)*4+k];
		tv[ 9]=src[(yi2*w+xi1)*4+k];
		tv[10]=src[(yi2*w+xi2)*4+k];
		tv[11]=src[(yi2*w+xi3)*4+k];
		tv[12]=src[(yi3*w+xi0)*4+k];
		tv[13]=src[(yi3*w+xi1)*4+k];
		tv[14]=src[(yi3*w+xi2)*4+k];
		tv[15]=src[(yi3*w+xi3)*4+k];

		tva[0]=tv[ 1]+xf*(tv[ 1]-tv[ 0]);
		tva[1]=tv[ 5]+xf*(tv[ 5]-tv[ 4]);
		tva[2]=tv[ 9]+xf*(tv[ 9]-tv[ 8]);
		tva[3]=tv[13]+xf*(tv[13]-tv[12]);

		tvb[0]=tv[ 2]+xg*(tv[ 2]-tv[ 3]);
		tvb[1]=tv[ 6]+xg*(tv[ 6]-tv[ 7]);
		tvb[2]=tv[10]+xg*(tv[10]-tv[11]);
		tvb[3]=tv[14]+xg*(tv[14]-tv[15]);

		tvc[0]=tv[ 4]+yf*(tv[ 4]-tv[ 0]);
		tvc[1]=tv[ 5]+yf*(tv[ 5]-tv[ 1]);
		tvc[2]=tv[ 6]+yf*(tv[ 6]-tv[ 2]);
		tvc[3]=tv[ 7]+yf*(tv[ 7]-tv[ 3]);

		tvd[0]=tv[ 8]+yg*(tv[ 8]-tv[12]);
		tvd[1]=tv[ 9]+yg*(tv[ 9]-tv[13]);
		tvd[2]=tv[10]+yg*(tv[10]-tv[14]);
		tvd[3]=tv[11]+yg*(tv[11]-tv[15]);


		tva[0]=tva[0]*xg+tvb[0]*xf;
		tva[1]=tva[1]*xg+tvb[1]*xf;
		tva[2]=tva[2]*xg+tvb[2]*xf;
		tva[3]=tva[3]*xg+tvb[3]*xf;

		tvb[0]=tvc[0]*yg+tvd[0]*yf;
		tvb[1]=tvc[1]*yg+tvd[1]*yf;
		tvb[2]=tvc[2]*yg+tvd[2]*yf;
		tvb[3]=tvc[3]*yg+tvd[3]*yf;

		tvc[0]=tva[1]+yf*(tva[1]-tva[0]);
		tvc[1]=tva[2]+yg*(tva[2]-tva[3]);
		tvc[2]=tvb[1]+xf*(tvb[1]-tvb[0]);
		tvc[3]=tvb[2]+xg*(tvb[2]-tvb[3]);

//		f=xf-0.5; g=yf-0.5;
//		g=1.0-(xf*xf+yf*yf);

		f=(tvc[0]*yg+tvc[1]*yf+
		   tvc[2]*xg+tvc[3]*xf)*0.5;

		rgba[k]=f;


//		f=tva[1]+yf*(tva[1]-tva[0]);
//		g=tva[2]+yg*(tva[2]-tva[3]);
//		rgba[k]=f*yg+g*yf;
	}
	return(0);
}

BTEIFGL_API int Tex_ResampleSpline(byte *src, int iw, int ih,
	byte *dst, int ow, int oh)
{
	float tv[4];
	float xs, ys, xc, yc;
	float ix, iy, fx, fy;
	int i, j, k, l, ik, il;

//	return(0);

	xs=((float)iw/(float)ow);
	ys=((float)ih/(float)oh);

	for(i=0; i<oh; i++)
		for(j=0; j<ow; j++)
	{
		Tex_SplinePolateRGBA8(src, iw, ih, j*xs, i*ys, tv);
		k=tv[0]; if(k<0)k=0; if(k>255)k=255; dst[0]=k;
		k=tv[1]; if(k<0)k=0; if(k>255)k=255; dst[1]=k;
		k=tv[2]; if(k<0)k=0; if(k>255)k=255; dst[2]=k;
		k=tv[3]; if(k<0)k=0; if(k>255)k=255; dst[3]=k;

		dst+=4;
	}
	return(0);
}

BTEIFGL_API int Tex_FilterBlur2(byte *src, byte *dst, int w, int h)
{
	float tv[4], tw;
	int i, j, k, l;
	int ix, iy;
	float f, g;

	for(i=0; i<h; i++)
		for(j=0; j<w; j++)
	{
		tw=0;

		k=i-1; l=j-1;
		if(k<0)k+=h; if(l<0)l+=w;
		if(k>=h)k-=h; if(l>=w)l-=w;
		tv[0]=src[(k*w+l)*4+0]*0.25;
		tv[1]=src[(k*w+l)*4+1]*0.25;
		tv[2]=src[(k*w+l)*4+2]*0.25;
		tv[3]=src[(k*w+l)*4+3]*0.25;

		k=i+1; l=j+1;
		if(k<0)k+=h; if(l<0)l+=w;
		if(k>=h)k-=h; if(l>=w)l-=w;
		tv[0]+=src[(k*w+l)*4+0]*0.25;
		tv[1]+=src[(k*w+l)*4+1]*0.25;
		tv[2]+=src[(k*w+l)*4+2]*0.25;
		tv[3]+=src[(k*w+l)*4+3]*0.25;

		k=i+1; l=j-1;
		if(k<0)k+=h; if(l<0)l+=w;
		if(k>=h)k-=h; if(l>=w)l-=w;
		tv[0]+=src[(k*w+l)*4+0]*0.25;
		tv[1]+=src[(k*w+l)*4+1]*0.25;
		tv[2]+=src[(k*w+l)*4+2]*0.25;
		tv[3]+=src[(k*w+l)*4+3]*0.25;

		k=i-1; l=j+1;
		if(k<0)k+=h; if(l<0)l+=w;
		if(k>=h)k-=h; if(l>=w)l-=w;
		tv[0]+=src[(k*w+l)*4+0]*0.25;
		tv[1]+=src[(k*w+l)*4+1]*0.25;
		tv[2]+=src[(k*w+l)*4+2]*0.25;
		tv[3]+=src[(k*w+l)*4+3]*0.25;

		tw+=1.0;


		k=i; l=j-1;
		if(k<0)k+=h; if(l<0)l+=w;
		tv[0]+=src[(k*w+l)*4+0]*0.5;
		tv[1]+=src[(k*w+l)*4+1]*0.5;
		tv[2]+=src[(k*w+l)*4+2]*0.5;
		tv[3]+=src[(k*w+l)*4+3]*0.5;

		k=i-1; l=j;
		if(k<0)k+=h; if(l<0)l+=w;
		tv[0]+=src[(k*w+l)*4+0]*0.5;
		tv[1]+=src[(k*w+l)*4+1]*0.5;
		tv[2]+=src[(k*w+l)*4+2]*0.5;
		tv[3]+=src[(k*w+l)*4+3]*0.5;

		k=i; l=j+1;
		if(k<0)k+=h; if(l<0)l+=w;
		if(k>=h)k-=h; if(l>=w)l-=w;
		tv[0]+=src[(k*w+l)*4+0]*0.5;
		tv[1]+=src[(k*w+l)*4+1]*0.5;
		tv[2]+=src[(k*w+l)*4+2]*0.5;
		tv[3]+=src[(k*w+l)*4+3]*0.5;

		k=i+1; l=j;
		if(k<0)k+=h; if(l<0)l+=w;
		if(k>=h)k-=h; if(l>=w)l-=w;
		tv[0]+=src[(k*w+l)*4+0]*0.5;
		tv[1]+=src[(k*w+l)*4+1]*0.5;
		tv[2]+=src[(k*w+l)*4+2]*0.5;
		tv[3]+=src[(k*w+l)*4+3]*0.5;

		tw+=2.0;


		k=i; l=j;
		if(k<0)k+=h; if(l<0)l+=w;
		tv[0]+=src[(k*w+l)*4+0]*1.0;
		tv[1]+=src[(k*w+l)*4+1]*1.0;
		tv[2]+=src[(k*w+l)*4+2]*1.0;
		tv[3]+=src[(k*w+l)*4+3]*1.0;

		tw+=1.0;

		tv[0]/=tw; tv[1]/=tw;
		tv[2]/=tw; tv[3]/=tw;

		k=tv[0]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+0]=k;
		k=tv[1]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+1]=k;
		k=tv[2]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+2]=k;
		k=tv[3]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+3]=k;
	}

	return(0);
}

BTEIFGL_API int Tex_FilterHBlur(byte *src, byte *dst,
	int w, int h, float bf)
{
	float tv[4], tw;
	int i, j, k, l;
	int ix, iy;
	float f, g;

	for(i=0; i<h; i++)
		for(j=0; j<w; j++)
	{
		tv[0]=src[(i*w+j)*4+0]*1.0;
		tv[1]=src[(i*w+j)*4+1]*1.0;
		tv[2]=src[(i*w+j)*4+2]*1.0;
		tv[3]=src[(i*w+j)*4+3]*1.0;

		k=i; l=j-1; if(l<0)l+=w;
		tv[0]+=src[(k*w+l)*4+0]*bf;
		tv[1]+=src[(k*w+l)*4+1]*bf;
		tv[2]+=src[(k*w+l)*4+2]*bf;
		tv[3]+=src[(k*w+l)*4+3]*bf;

		k=i; l=j+1; if(l>=w)l-=w;
		tv[0]+=src[(k*w+l)*4+0]*bf;
		tv[1]+=src[(k*w+l)*4+1]*bf;
		tv[2]+=src[(k*w+l)*4+2]*bf;
		tv[3]+=src[(k*w+l)*4+3]*bf;

		f=1.0+(2.0*bf);
		tv[0]/=f; tv[1]/=f;
		tv[2]/=f; tv[3]/=f;

		k=tv[0]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+0]=k;
		k=tv[1]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+1]=k;
		k=tv[2]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+2]=k;
		k=tv[3]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+3]=k;
	}

	return(0);
}

BTEIFGL_API int Tex_FilterVBlur(byte *src, byte *dst, int w, int h, float bf)
{
	float tv[4], tw;
	int i, j, k, l;
	int ix, iy;
	float f, g;

	for(i=0; i<h; i++)
		for(j=0; j<w; j++)
	{
		tv[0]=src[(i*w+j)*4+0]*1.0;
		tv[1]=src[(i*w+j)*4+1]*1.0;
		tv[2]=src[(i*w+j)*4+2]*1.0;
		tv[3]=src[(i*w+j)*4+3]*1.0;

		k=i-1; l=j; if(k<0)k+=h;
		tv[0]+=src[(k*w+l)*4+0]*bf;
		tv[1]+=src[(k*w+l)*4+1]*bf;
		tv[2]+=src[(k*w+l)*4+2]*bf;
		tv[3]+=src[(k*w+l)*4+3]*bf;

		k=i+1; l=j; if(k>=h)k-=h;
		tv[0]+=src[(k*w+l)*4+0]*bf;
		tv[1]+=src[(k*w+l)*4+1]*bf;
		tv[2]+=src[(k*w+l)*4+2]*bf;
		tv[3]+=src[(k*w+l)*4+3]*bf;

		f=1.0+(2.0*bf);
		tv[0]/=f; tv[1]/=f;
		tv[2]/=f; tv[3]/=f;

		k=tv[0]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+0]=k;
		k=tv[1]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+1]=k;
		k=tv[2]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+2]=k;
		k=tv[3]; if(k<0)k=0; if(k>255)k=255; dst[(i*w+j)*4+3]=k;
	}

	return(0);
}

BTEIFGL_API int Tex_FilterBlurHV(byte *src, byte *dst,
	int w, int h, float xf, float yf)
{
	byte *buf2;
	float tv[4], tw;
	int i, j, k, l;
	int ix, iy;
	float f, g;

	buf2=malloc(w*h*4);
	memcpy(dst, src, w*h*4);
	while(xf>=1)
	{
		Tex_FilterHBlur(dst, buf2, w, h, 0.5);
		memcpy(dst, buf2, w*h*4);
		xf-=1;
	}
	while(yf>=1)
	{
		Tex_FilterVBlur(dst, buf2, w, h, 0.5);
		memcpy(dst, buf2, w*h*4);
		yf-=1;
	}

	if(xf>0.01)
	{
		Tex_FilterHBlur(dst, buf2, w, h, xf*0.5);
		memcpy(dst, buf2, w*h*4);
	}

	if(yf>0.01)
	{
		Tex_FilterVBlur(dst, buf2, w, h, yf*0.5);
		memcpy(dst, buf2, w*h*4);
	}

	free(buf2);

	return(0);
}

BTEIFGL_API int Tex_FilterBlur(byte *src, byte *dst, int w, int h, int rad)
{
	byte *buf2;
	float tv[4], tw;
	int i, j, k, l;
	int ix, iy;
	float f, g;

	buf2=malloc(w*h*4);
	memcpy(dst, src, w*h*4);
	for(i=0; i<rad; i++)
	{
		Tex_FilterBlur2(dst, buf2, w, h);
		memcpy(dst, buf2, w*h*4);
	}
	free(buf2);

	return(0);

#if 0
	if(rad<=0)rad=1;

	for(i=0; i<h; i++)
		for(j=0; j<w; j++)
	{
		tv[0]=0; tv[1]=0; tv[2]=0; tv[3]=0;
		tw=0;
		for(k=-rad; k<=rad; k++)
			for(l=-rad; l<=rad; l++)
		{
			iy=i+k; ix=j+l;
			if(ix<0)ix+=w; if(iy<0)iy+=h;
			if(ix>=w)ix-=w; if(iy>=h)iy-=h;

			f=(M_PI/2)*((1.0*k)/rad);
			g=(M_PI/2)*((1.0*l)/rad);
			f=cos(f)*cos(g);

			tv[0]+=src[(iy*w+ix)*4+0]*f;
			tv[1]+=src[(iy*w+ix)*4+1]*f;
			tv[2]+=src[(iy*w+ix)*4+2]*f;
			tv[3]+=src[(iy*w+ix)*4+3]*f;
			tw+=f;
		}

		if(tw<=0)tw=1;
		tv[0]/=tw; tv[1]/=tw; tv[2]/=tw; tv[3]/=tw;

		k=tv[0]; if(k<0)k=0; if(k>255)k=255; dst[0]=k;
		k=tv[1]; if(k<0)k=0; if(k>255)k=255; dst[1]=k;
		k=tv[2]; if(k<0)k=0; if(k>255)k=255; dst[2]=k;
		k=tv[3]; if(k<0)k=0; if(k>255)k=255; dst[3]=k;
		dst+=4;
	}
#endif
}

BTEIFGL_API int Tex_ResampleSpline2(byte *src, int iw, int ih,
	byte *dst, int ow, int oh)
{
	byte *buf;
	float xf, yf;
	int i;

	if((ow==iw) && (oh==ih))
	{
		memcpy(dst, src, ow*oh*4);
		return(0);
	}

	if((ow>=iw) && (oh>=ih))
	{
		i=Tex_ResampleSpline(src, iw, ih, dst, ow, oh);
		return(i);
	}

	buf=malloc(iw*ih*4);

	xf=(1.0*iw)/ow-1.0; yf=(1.0*ih)/oh-1.0;

	//no blur if not compressing
	if(xf<=0)xf=0; if(yf<=0)yf=0;

	Tex_FilterBlurHV(src, buf, ow, ih, xf, yf);
	i=Tex_ResampleSpline(buf, iw, ih, dst, ow, oh);
	free(buf);
	return(i);
}

// static byte *resampbuf=NULL;
// static byte resampbuf[2048*2048*4];
// static byte resampbuf_cmp[2048*2048];
static int resamp_colorfmt;
static int tex_use_colorfmt=-1;

BTEIFGL_API int Tex_SetUseColorFormat(int clrfmt)
{
	tex_use_colorfmt=clrfmt;
	return(0);
}

BTEIFGL_API int Tex_GetLastColorFormat()
{
	return(resamp_colorfmt);
}

#if 0
BTEIFGL_API int Tex_LoadTexture(int w, int h, byte *buf, int calcmip)
{
	byte *resampbuf;
	byte *resampbuf_cmp;

	int tw, th, tl;
	int min, mag, cmp, sz, blksz, fmt;
	int num;

//	if(!resampbuf)resampbuf=malloc(2048*2048*4);

//	tex_lock();

	tw=1;
	th=1;
	while((tw<<1)<=w)tw<<=1;
	while((th<<1)<=h)th<<=1;
	if(tw<w && (tw<<1)>w)tw<<=1;
	if(th<h && (th<<1)>h)th<<=1;
//	if(tw>256)tw=256;
//	if(th>256)th=256;

	if(tw<th) { if((tw*2)<th)th=tw; else tw=th; }
	if(th<tw) { if((th*2)<tw)tw=th; else th=tw; }

//	if(tw>1024)tw=1024;
//	if(th>1024)th=1024;

	if(tw>2048)tw=2048;
	if(th>2048)th=2048;

	resampbuf=malloc(tw*th*4+256);
	resampbuf_cmp=malloc(tw*th+256);

	if((w!=tw) || (h!=th))
	{
//		*(int *)-1=-1;
	
//		Tex_Resample(buf, w, h, resampbuf, tw, th);
		Tex_ResampleSpline(buf, w, h, resampbuf, tw, th);
//		Tex_ResampleSinc(buf, w, h, resampbuf, tw, th);
	}
	else
	{
		memcpy(resampbuf, buf, w*h*4);
	}

	if(tex_use_colorfmt<0)
	{
//		BGBBTJ_BCn_EncodeImageDXT5(resampbuf_cmp, resampbuf, tw, th, 4);
		BGBBTJ_BCn_EncodeImageAutoDXTn(
			resampbuf_cmp, resampbuf, tw, th, 4, &fmt);
		resamp_colorfmt=fmt;
	}else
	{
		fmt=tex_use_colorfmt;
		resamp_colorfmt=fmt;

		BGBBTJ_BCn_EncodeImageDXTn(
			resampbuf_cmp, resampbuf, tw, th, 4, fmt);
	}

	switch(fmt)
	{
	case BGBBTJ_JPG_BC1:
	case BGBBTJ_JPG_BC1F:
		cmp=GL_COMPRESSED_RGB_S3TC_DXT1_EXT; blksz=8;
		break;
	case BGBBTJ_JPG_BC2:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; blksz=16;
		break;
	case BGBBTJ_JPG_BC3:
	case BGBBTJ_JPG_BC3F:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
		break;
	case BGBBTJ_JPG_BC3_UVAY:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
		break;
	case BGBBTJ_JPG_BC6H_SF16:
		cmp=GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT; blksz=16;
		break;
	case BGBBTJ_JPG_BC6H_UF16:
		cmp=GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT; blksz=16;
		break;
	case BGBBTJ_JPG_BC7:
		cmp=GL_COMPRESSED_RGBA_BPTC_UNORM; blksz=16;
		break;
	case BGBBTJ_JPG_BC7_SRGB:
		cmp=GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM; blksz=16;
		break;
	default:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
		break;
	}

	num=Tex_AllocTexnum();
	tex_width[num]=w;
	tex_height[num]=h;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, num);

	sz=(tw/4)*(th/4)*blksz;
	pdglCompressedTexImage2D(GL_TEXTURE_2D, 0, 
		cmp, tw, th, 0, sz, resampbuf_cmp);

//	pdglCompressedTexImage2D(GL_TEXTURE_2D, 0, 
//		GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
//		tw, th, 0, sz, resampbuf_cmp);

	if(!calcmip)
	{
		Tex_GetModeinfo(&min, &mag);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	}else if((calcmip&15)==2)
	{
		tl=0;
		while((tw>1) && (th>1))
		{
			Tex_HalfSample(resampbuf, tw, th);
			tw>>=1;
			th>>=1;
			tl++;

#if 1
			BGBBTJ_BCn_EncodeImageDXTn(
				resampbuf_cmp, resampbuf, tw, th, 4, fmt);

			sz=(tw/4)*(th/4)*blksz;
			if(sz<blksz)sz=blksz;
			pdglCompressedTexImage2D(GL_TEXTURE_2D, tl, 
				cmp, tw, th, 0, sz, resampbuf_cmp);
#endif
		}

//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_NEAREST_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}else
	{
//		printf("tex: mipmap\n");
		tl=0;
		while((tw>1) && (th>1))
		{
			Tex_HalfSample(resampbuf, tw, th);
			tw>>=1;
			th>>=1;
			tl++;

#if 1
			BGBBTJ_BCn_EncodeImageDXTn(
				resampbuf_cmp, resampbuf, tw, th, 4, fmt);

			sz=(tw/4)*(th/4)*blksz;
			if(sz<blksz)sz=blksz;
			pdglCompressedTexImage2D(GL_TEXTURE_2D, tl, 
				cmp, tw, th, 0, sz, resampbuf_cmp);
#endif
		}

		Tex_GetModeinfo(&min, &mag);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	}

	free(resampbuf);
	free(resampbuf_cmp);

//	tex_unlock();
	return(num);
}
#endif

#if 1
BTEIFGL_API int Tex_LoadTexture(int w, int h, byte *buf, int calcmip)
{
	byte *resampbuf;
	int tw, th, tl;
	int min, mag, cmp, sz;
	int num;

//	if(!resampbuf)resampbuf=malloc(2048*2048*4);

	tw=1;
	th=1;
	while((tw<<1)<=w)tw<<=1;
	while((th<<1)<=h)th<<=1;
	if(tw<w && (tw<<1)>w)tw<<=1;
	if(th<h && (th<<1)>h)th<<=1;
//	if(tw>256)tw=256;
//	if(th>256)th=256;

//	if(tw<th) { if((tw*2)<th)th=tw; else tw=th; }
//	if(th<tw) { if((th*2)<tw)tw=th; else th=tw; }

//	if(tw>1024)tw=1024;
//	if(th>1024)th=1024;

	if(tw>2048)tw=2048;
	if(th>2048)th=2048;

	resampbuf=malloc(tw*th*4);

	if((w!=tw) || (h!=th))
	{
//		*(int *)-1=-1;
	
		Tex_ResampleQuick((int *)buf, w, h, (int *)resampbuf, tw, th);
//		Tex_Resample(buf, w, h, resampbuf, tw, th);
//		Tex_ResampleSpline(buf, w, h, resampbuf, tw, th);
//		Tex_ResampleSinc(buf, w, h, resampbuf, tw, th);
	}
	else
	{
		memcpy(resampbuf, buf, w*h*4);
	}

	num=Tex_AllocTexnum();
	tex_width[num]=w;
	tex_height[num]=h;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, num);

#ifdef WIN32
	cmp=(calcmip&16)?4:GL_COMPRESSED_RGBA;
#else
	cmp=4;
#endif

	glTexImage2D(GL_TEXTURE_2D, 0, cmp,
		tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);

	if(!calcmip)
	{
		Tex_GetModeinfo(&min, &mag);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	}else if((calcmip&15)==2)
	{
		tl=0;
//		while((tw>1) && (th>1))
		while((tw>1) || (th>1))
		{
			Tex_HalfSample(resampbuf, tw, th);
			tw>>=1;
			th>>=1;
			if(!tw)tw=1;
			if(!th)th=1;
			tl++;
			glTexImage2D(GL_TEXTURE_2D, tl, cmp,
				tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);
		}

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_NEAREST_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);
	}else
	{
//		printf("tex: mipmap\n");
		tl=0;
//		while((tw>1) && (th>1))
		while((tw>1) || (th>1))
		{
			Tex_HalfSample(resampbuf, tw, th);
			tw>>=1;
			th>>=1;
			if(!tw)tw=1;
			if(!th)th=1;
			tl++;
			glTexImage2D(GL_TEXTURE_2D, tl, cmp,
				tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);
		}

		Tex_GetModeinfo(&min, &mag);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);
	}

	free(resampbuf);

	return(num);
}
#endif

BTEIFGL_API int Tex_LoadTexture2(int w, int h, byte *buf,
	int mip, int rnum, int enc)
{
	byte *resampbuf;
	int tw, th, tl;
	int min, mag;
	int num;

//	tex_lock();

//	if(!resampbuf)resampbuf=malloc(2048*2048*4);

	tw=1;
	th=1;
	while((tw<<1)<=w)tw<<=1;
	while((th<<1)<=h)th<<=1;
	if(tw<w && (tw<<1)>w)tw<<=1;
	if(th<h && (th<<1)>h)th<<=1;

//	if(tw<th)tw=th;
//	if(th<tw)th=tw;

	if(tw>1024)tw=1024;
	if(th>1024)th=1024;

#if 0
	if(mip==2)
	{
		tw<<=1;
		th<<=1;
	}
#endif

	resampbuf=malloc(tw*th*4);

	if((w!=tw) || (h!=th))
		Tex_ResampleQuick((int *)buf, w, h, (int *)resampbuf, tw, th);
//		Tex_Resample(buf, w, h, resampbuf, tw, th);
//		Tex_ResampleSpline(buf, w, h, resampbuf, tw, th);
		else memcpy(resampbuf, buf, w*h*4);

	num=rnum;
//	if(!num)num=Tex_AllocTexnum();
	if(num<=0)num=Tex_AllocTexnum();

	tex_width[num]=w;
	tex_height[num]=h;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, num);

	if(!mip)
	{
//		printf("tex: nonmip\n");
		glTexImage2D(GL_TEXTURE_2D, 0, 4, tw, th, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);
		Tex_GetModeinfo(&min, &mag);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);
	}else if(mip==2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, tw, th, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);

		tl=0;
		while(tw>1 && th>1)
		{
//			Tex_HalfSample2(resampbuf, tw, th);
			Tex_HalfSample(resampbuf, tw, th);
			tw>>=1;
			th>>=1;
			tl++;
			glTexImage2D(GL_TEXTURE_2D, tl, 4, tw, th, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);
		}

//		Tex_GetModeinfo(&min, &mag);
//		min=GL_NEAREST;
//		mag=GL_NEAREST;
//		min=GL_LINEAR_MIPMAP_LINEAR;
//		mag=GL_LINEAR;

		min=GL_NEAREST_MIPMAP_LINEAR;
//		mag=GL_LINEAR;
		mag=GL_NEAREST;

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);
	}else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, tw, th, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);

//		printf("tex: mipmap\n");
		tl=0;
		while(tw>1 && th>1)
		{
			Tex_HalfSample(resampbuf, tw, th);
			tw>>=1;
			th>>=1;
			tl++;
			glTexImage2D(GL_TEXTURE_2D, tl, 4, tw, th, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);
		}

		Tex_GetModeinfo(&min, &mag);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);
	}

	free(resampbuf);
//	tex_unlock();

	return(num);
}

#if 0
BTEIFGL_API int Tex_LoadTexture3A(int *wp, int *hp, byte *buf,
	int num, int clrs)
{ return(Tex_LoadTexture3B(wp, hp, buf, num, clrs, 0)); }

BTEIFGL_API int Tex_LoadTexture3B(int *wp, int *hp, byte *buf,
	int num, int clrs, int mip)
{
	byte *rsbuf, *cs;
	int tw, th, tl, rl;
	int cmp, blksz, sz;
	int min, mag;

	if((clrs==BGBBTJ_JPG_RGB) || (clrs==BGBBTJ_JPG_RGBA))
	{
		return(Tex_LoadTexture3(wp, hp, buf, num));
	}

//	tex_lock();

	switch(clrs)
	{
	case BGBBTJ_JPG_BC1:
	case BGBBTJ_JPG_BC1F:
		cmp=GL_COMPRESSED_RGB_S3TC_DXT1_EXT; blksz=8;
		break;
	case BGBBTJ_JPG_BC2:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; blksz=16;
		break;
	case BGBBTJ_JPG_BC3:
	case BGBBTJ_JPG_BC3F:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
	case BGBBTJ_JPG_BC3_UVAY:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
		break;
	case BGBBTJ_JPG_BC6:
		cmp=GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT; blksz=16;
		break;
	case BGBBTJ_JPG_BC6_UF16:
		cmp=GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT; blksz=16;
		break;
	case BGBBTJ_JPG_BC7:
		cmp=GL_COMPRESSED_RGBA_BPTC_UNORM; blksz=16;
		break;
	case BGBBTJ_JPG_BC7_SRGB:
		cmp=GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM; blksz=16;
		break;
	default:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
		break;
	}

	tw=*wp;
	th=*hp;
	if((tw&(tw-1)) || (th&(th-1)) || (tw!=th))
		return(num);

	if(num<=0)
	{
		num=Tex_AllocTexnum();
		rl=1;
	}else rl=0;

//	tex_width[num]=w;
//	tex_height[num]=h;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, num);

	sz=((tw+3)/4)*((th+3)/4)*blksz;
	pdglCompressedTexImage2D(GL_TEXTURE_2D, 0, 
		cmp, tw, th, 0, sz, buf);

	if(mip)
	{
		cs=buf+sz; tl=1;
//		while((tw>1) && (th>1))
		while((tw>1) || (th>1))
		{
//			tw>>=1; th>>=1;
			tw=(tw+1)>>1;
			th=(th+1)>>1;
			sz=((tw+3)/4)*((th+3)/4)*blksz;
			if(!sz)sz=blksz;
			pdglCompressedTexImage2D(GL_TEXTURE_2D, tl, 
				cmp, tw, th, 0, sz, cs);
			tl++;
			cs=cs+sz;
		}
	}

	if(rl)
	{
		if(mip)
		{
			Tex_GetModeinfo(&min, &mag);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		}else
		{
			Tex_GetModeinfo(&min, &mag);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		}
	}

	return(num);
}
#endif

BTEIFGL_API int Tex_LoadTexture3(int *wp, int *hp, byte *buf, int num)
{
	byte *resampbuf;
	int tw, th, tl;
	int min, mag;
	int h, w, rl;

//	if(!resampbuf)resampbuf=malloc(2048*2048*4);

//	tex_lock();

	w=*wp;
	h=*hp;

	tw=1;
	th=1;
	while((tw<<1)<=w)tw<<=1;
	while((th<<1)<=h)th<<=1;
//	if(tw<w && (tw<<1)>w)tw<<=1;
//	if(th<h && (th<<1)>h)th<<=1;

//	if(tw<th)tw=th;
//	if(th<tw)th=tw;

	if(tw>1024)tw=1024;
	if(th>1024)th=1024;

	resampbuf=malloc(tw*th*4);

	*wp=tw;
	*hp=th;

	if((w!=tw) || (h!=th))
//		Tex_Resample(buf, w, h, resampbuf, tw, th);
		Tex_ResampleQuick((int *)buf, w, h, (int *)resampbuf, tw, th);
//		Tex_Resample2(buf, w, h, resampbuf, tw, th);
//		Tex_ResampleSpline(buf, w, h, resampbuf, tw, th);
//		LBXGL_ScaleImage32(buf, w, h, resampbuf, tw, th);
		else memcpy(resampbuf, buf, w*h*4);

	if(num<=0)
	{
		num=Tex_AllocTexnum();
		rl=1;
	}else rl=0;

//	tex_width[num]=w;
//	tex_height[num]=h;

//	printf("LT3 %d %d %d %d (%d)\n", w, h, tw, th, num);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, num);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, tw, th, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, resampbuf);

	if(rl)
	{
		Tex_GetModeinfo(&min, &mag);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	free(resampbuf);
//	tex_unlock();
	
	return(num);
}

BTEIFGL_API int Tex_LoadTextureMono(int w, int h, byte *buf)
{
	byte *resampbuf;
	int tw, th, tl;
	int num, min, mag;

//	tex_lock();

//	if(!resampbuf)resampbuf=malloc(2048*2048*4);

	tw=1;
	th=1;
	while((tw<<1)<=w)tw<<=1;
	while((th<<1)<=h)th<<=1;
	if(tw<w && (tw<<1)>w)tw<<=1;
	if(th<h && (th<<1)>h)th<<=1;
//	if(tw>256)tw=256;
//	if(th>256)th=256;

//	if(tw<th)tw=th;
//	if(th<tw)th=tw;

	if(tw<th) { if((tw*2)<th)th=tw; else tw=th; }
	if(th<tw) { if((th*2)<tw)tw=th; else th=tw; }

	if(tw>1024)tw=1024;
	if(th>1024)th=1024;

	resampbuf=malloc(tw*th*4);

	if((w!=tw) || (h!=th))
		Tex_ResampleMono(buf, w, h, resampbuf, tw, th);
		else memcpy(resampbuf, buf, w*h);

	num=Tex_AllocTexnum();
	tex_width[num]=w;
	tex_height[num]=h;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, num);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, tw, th, 0, GL_LUMINANCE,
		GL_UNSIGNED_BYTE, resampbuf);

	Tex_GetModeinfo(&min, &mag);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(resampbuf);
//	tex_unlock();

	return(num);
}


#if 1
typedef struct FRGL_TGAHeader_s FRGL_TGAHeader;

struct FRGL_TGAHeader_s
{
byte 	id_length, ctype, itype;
u16		cindex, clength;
byte	csize;
u16		x_origin, y_origin, width, height;
byte	bpp, attributes;
};

BTEIFGL_API byte *Img_LoadTGA(VFILE *fd, int *w, int *h)
{
	FRGL_TGAHeader head;
	int pixlin, i, j;
	byte *dest, *src, *buf, *obuf, r, g, b, a;

	head.id_length=vfgetc(fd);
	head.ctype=vfgetc(fd);
	head.itype=vfgetc(fd);
	head.cindex=vfgetc(fd);
	head.cindex+=(vfgetc(fd)<<8);
	head.clength=vfgetc(fd);
	head.clength+=(vfgetc(fd)<<8);
	head.csize=vfgetc(fd);
	head.x_origin=vfgetc(fd);
	head.x_origin+=(vfgetc(fd)<<8);
	head.y_origin=vfgetc(fd);
	head.y_origin+=(vfgetc(fd)<<8);
	head.width=vfgetc(fd);
	head.width+=(vfgetc(fd)<<8);
	head.height=vfgetc(fd);
	head.height+=(vfgetc(fd)<<8);
	head.bpp=vfgetc(fd);
	head.attributes=vfgetc(fd);

	if(((head.itype!=2) && (head.itype!=3) &&
		(head.itype!=10) && (head.itype!=11)) ||
			((head.bpp!=32) && (head.bpp!=24) && (head.bpp!=8)) ||
			(head.ctype!=0))
	{
		printf("Img_LoadTGA: Unsupported Image Type\n");
		printf("   bpp: %d, ctype: %d, itype: %d\n",
			head.bpp, head.ctype, head.itype);
		return(NULL);
	}

	vfseek(fd, head.id_length, 1); /* skip comment */

	pixlin=head.width*head.height;
	obuf=malloc(pixlin*4);
	dest=obuf;

	buf=malloc(2*pixlin*(head.bpp>>3));
	vfread(buf, 1, 2*pixlin*(head.bpp>>3), fd);
	src=buf;

	if(head.itype==2)
	{
		for(i=0; i<pixlin; i++)
		{
			b=*src++;
			g=*src++;
			r=*src++;
			if(head.bpp==32)
				a=*src++;
				else a=255;
			*dest++=r;
			*dest++=g;
			*dest++=b;
			*dest++=a;
		}
	}

	if(head.itype==3)
	{
		for(i=0; i<pixlin; i++)
		{
			g=*src++;
			*dest++=g;
			*dest++=g;
			*dest++=g;
			*dest++=255;
		}
	}

	if(head.itype==10)
	{
		i=0;
		while(i<pixlin)
		{
			j=*src++;
			if(j<128)
			{
				j++;
				while(j)
				{
					b=*src++;
					g=*src++;
					r=*src++;
					if(head.bpp==32)
						a=*src++;
						else a=255;
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}else
			{
				b=*src++;
				g=*src++;
				r=*src++;
				if(head.bpp==32)
					a=*src++;
					else a=255;

				j-=127;
				while(j)
				{
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}
		}
	}

	if(head.itype==11)
	{
		i=0;
		while(i<pixlin)
		{
			j=*src++;
			if(j<128)
			{
				j++;
				while(j)
				{
					g=*src++;
					b=g; r=g;
					a=255;
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}else
			{
				g=*src++;
				b=g; r=g;
				a=255;

				j-=127;
				while(j)
				{
					*dest++=r;
					*dest++=g;
					*dest++=b;
					*dest++=a;

					j--;
					i++;
				}
			}
		}
	}

	free(buf);

//	if(!(head.attributes&32))
	if(head.attributes&32)
	{
		dest=malloc(pixlin*4);
		for(i=0; i<head.height; i++)
			memcpy(&dest[i*head.width*4],
				&obuf[(head.height-(i+1))*head.width*4], head.width*4);
		memcpy(obuf, dest, pixlin*4);
		free(dest);
	}

	*w=head.width;
	*h=head.height;
	return(obuf);
}

BTEIFGL_API int Img_StoreTGA(VFILE *fd, byte *buf, int w, int h)
{
	FRGL_TGAHeader head;
	int i;
	byte *s;

	head.id_length=0;
	head.ctype=0;
	head.itype=2;
	head.cindex=0;
	head.clength=0;
	head.csize=0;
	head.x_origin=0;
	head.y_origin=0;
	head.width=w;
	head.height=h;
	head.bpp=24;
	head.attributes=0;

	vfputc(head.id_length, fd);
	vfputc(head.ctype, fd);
	vfputc(head.itype, fd);

	vfputc(head.cindex, fd);
	vfputc(head.cindex>>8, fd);
	vfputc(head.clength, fd);
	vfputc(head.clength>>8, fd);

	vfputc(head.csize, fd);

	vfputc(head.x_origin, fd);
	vfputc(head.x_origin>>8, fd);
	vfputc(head.y_origin, fd);
	vfputc(head.y_origin>>8, fd);

	vfputc(head.width, fd);
	vfputc(head.width>>8, fd);
	vfputc(head.height, fd);
	vfputc(head.height>>8, fd);

	vfputc(head.bpp, fd);
	vfputc(head.attributes, fd);

//	vfwrite(buf, w*4, h, fd);

	s=buf;
	for(i=0; i<w*h; i++)
	{
		vfputc(s[2], fd);
		vfputc(s[1], fd);
		vfputc(s[0], fd);
		s+=4;
	}

	return(0);
}

typedef struct {
unsigned short tag; //BM, 0x4D42
unsigned int filesize;
unsigned int resv;
unsigned int dataoffs;
}BMPFileHeader;

typedef struct {
unsigned int infosize;
unsigned int width;
unsigned int height;
unsigned short planes;
unsigned short bpp;
unsigned int compression;
unsigned int imagesize;
unsigned int hdensity;
unsigned int vdensity;
unsigned int numcolors;
unsigned int numimportant;
}BMPInfoHeader;

typedef struct {
//BMP File Header
unsigned short tag; //BM, 0x4D42
unsigned int filesize;
unsigned int resv;
unsigned int dataoffs;
//BMP Info Header
unsigned int infosize;
unsigned int width;
unsigned int height;
unsigned short planes;
unsigned short bpp;
unsigned int compression;
unsigned int imagesize;
unsigned int hdensity;
unsigned int vdensity;
unsigned int numcolors;
unsigned int numimportant;
}BMPHeader;

BTEIFGL_API byte *Img_LoadBMP(VFILE *fd, int *w, int *h)
{
	BMPHeader head;
	int i, pixlin;
	int b, g, r, a;
	byte *buf;

	memset(&head, 0, sizeof(head));

	for(i=0; i<2; i++)
		head.tag|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.filesize|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		vfgetc(fd);
	for(i=0; i<4; i++)
		head.dataoffs|=vfgetc(fd)<<(i*8);

	for(i=0; i<4; i++)
		head.infosize|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.width|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.height|=vfgetc(fd)<<(i*8);
	for(i=0; i<2; i++)
		head.planes|=vfgetc(fd)<<(i*8);
	for(i=0; i<2; i++)
		head.bpp|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.compression|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.imagesize|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.hdensity|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.vdensity|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.numcolors|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.numimportant|=vfgetc(fd)<<(i*8);

	if(head.tag!=0x4D42)
	{
		printf("Img_LoadBMP: Invalid BM Tag\n");
		return(NULL);
	}
	if(head.bpp!=24)
	{
		printf("Img_LoadBMP: Bits Per Pixel is not 24 bits (%d)\n",
			head.bpp);
		return(NULL);
	}
	if(head.compression!=0)
	{
		printf("Img_LoadBMP: Don't support compressed images\n");
		return(NULL);
	}

	if(w)*w=head.width;
	if(h)*h=head.height;

	pixlin=head.width*head.height;
	buf=malloc(pixlin*4);

	vfseek(fd, head.dataoffs, 0);

	for(i=0; i<pixlin; i++)
	{
		b=vfgetc(fd);
		g=vfgetc(fd);
		r=vfgetc(fd);
		a=255;

		buf[(i*4)+0]=r;
		buf[(i*4)+1]=g;
		buf[(i*4)+2]=b;
		buf[(i*4)+3]=a;
	}

	return(buf);
}

//flatten the bitmap
BTEIFGL_API byte *Img_FlattenDIB(byte *img, int w, int h)
{
	BMPInfoHeader *ihead;
	byte *buf, *t;
	int i, n;
	static int init=0;

	if(!init)
	{
//		frgl_talloc("img_dib_t", "byte+");
		init=1;
	}

	buf=frgl_talloc("img_dib_t", (w*h*3)+sizeof(BMPInfoHeader));
	ihead=(BMPInfoHeader *)buf;
	t=buf+sizeof(BMPInfoHeader);

	ihead->infosize=sizeof(BMPInfoHeader);
	ihead->width=w;
	ihead->height=h;
	ihead->planes=1;
	ihead->bpp=24;
	ihead->compression=0;
	ihead->imagesize=w*h*3;
	ihead->hdensity=72*12*3;
	ihead->vdensity=72*12*3;
	ihead->numcolors=0;
	ihead->numimportant=0;

	n=w*h;
	for(i=0; i<n; i++)
	{
		t[(i*3)+0]=img[(i*4)+2];
		t[(i*3)+1]=img[(i*4)+1];
		t[(i*3)+2]=img[(i*4)+0];
	}

	return(buf);
}

BTEIFGL_API byte *Img_UnFlattenDIB(byte *img, int *w, int *h)
{
	BMPInfoHeader *ihead;
	byte *buf, *s;
	int i, n;

	ihead=(BMPInfoHeader *)img;
	s=img+sizeof(BMPInfoHeader);

	if(ihead->infosize!=sizeof(BMPInfoHeader))
	{
		printf("Img_UnFlattenDIB: "
			"Header Size mismatch (%d, %d)\n",
			ihead->infosize, sizeof(BMPInfoHeader));
		return(NULL);
	}

	if(ihead->bpp!=24)
	{
		printf("Img_UnFlattenDIB: "
			"Bits Per Pixel is not 24 bits (%d)\n", ihead->bpp);
		return(NULL);
	}
	if(ihead->compression!=0)
	{
		printf("Img_UnFlattenDIB: "
			"Doesn't support compressed images\n");
		return(NULL);
	}

	*w=ihead->width;
	*h=ihead->height;
	n=ihead->width*ihead->height;
	buf=frgl_malloc(n*3);

	for(i=0; i<n; i++)
	{
		buf[(i*4)+0]=s[(i*3)+2];
		buf[(i*4)+1]=s[(i*3)+1];
		buf[(i*4)+2]=s[(i*3)+0];
		buf[(i*4)+3]=255;
	}

	return(buf);
}

#if 0
//flatten the bitmap
BTEIFGL_API byte *Img_FlattenBPX(byte *img, int w, int h, int clrs, int *rsz)
{
	BMPHeader *ihead;
	byte *buf, *t;
	int i, n;
	static int init=0;

	if(!init)
	{
//		frgl_talloc("img_dib_t", "byte+");
		init=1;
	}

	n=((w+3)/4)*((h+3)/4);
	buf=frgl_talloc("img_bmp_t", (n*2*16)+sizeof(BMPHeader));
	ihead=(BMPHeader *)buf;
	t=buf+sizeof(BMPHeader);

	ihead->tag=0x4D42;
	ihead->dataoffs=(sizeof(BMPHeader)+15)&(~15);
	ihead->filesize=(n*2*16)+ihead->dataoffs;

	ihead->infosize=sizeof(BMPInfoHeader);
	ihead->width=w;
	ihead->height=h;
	ihead->planes=1;
	ihead->bpp=24;
	ihead->compression=0;
	ihead->imagesize=w*h*3;
	ihead->hdensity=72*12*3;
	ihead->vdensity=72*12*3;
	ihead->numcolors=0;
	ihead->numimportant=0;

	if(clrs==BGBBTJ_JPG_BC7)
	{
		ihead->compression=RIFF_MAKETAG('B', 'C', '7', ' ');
		ihead->filesize=(n*2*16)+ihead->dataoffs;
		memcpy(buf+ihead->dataoffs, img, (n*2*16));
	}

	if((clrs==BGBBTJ_JPG_BC3) || (clrs==BGBBTJ_JPG_BC3F))
	{
		ihead->compression=RIFF_MAKETAG('D', 'X', 'T', '5');
		ihead->filesize=(n*2*16)+ihead->dataoffs;
		memcpy(buf+ihead->dataoffs, img, (n*2*16));
	}

	if((clrs==BGBBTJ_JPG_BC1) ||
		(clrs==BGBBTJ_JPG_BC1F) ||
		(clrs==BGBBTJ_JPG_BC1A))
	{
		ihead->compression=RIFF_MAKETAG('D', 'X', 'T', '1');
		ihead->filesize=(n*2*8)+ihead->dataoffs;
		memcpy(buf+ihead->dataoffs, img, (n*2*8));
	}

	if(rsz)*rsz=ihead->filesize;
	return(buf);
}

BTEIFGL_API byte *Img_LoadBPX(VFILE *fd, int *w, int *h, int *rtxc)
{
	BMPHeader head;
	int i, pixlin, bsz, txc, bcnt;
	int b, g, r, a;
	byte *buf;

	memset(&head, 0, sizeof(head));

	for(i=0; i<2; i++)
		head.tag|=vfgetc(fd)<<(i*8);

	for(i=0; i<2; i++)
		vfgetc(fd);

	for(i=0; i<4; i++)
		head.filesize|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		vfgetc(fd);
	for(i=0; i<4; i++)
		head.dataoffs|=vfgetc(fd)<<(i*8);

	for(i=0; i<4; i++)
		head.infosize|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.width|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.height|=vfgetc(fd)<<(i*8);
	for(i=0; i<2; i++)
		head.planes|=vfgetc(fd)<<(i*8);
	for(i=0; i<2; i++)
		head.bpp|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.compression|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.imagesize|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.hdensity|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.vdensity|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.numcolors|=vfgetc(fd)<<(i*8);
	for(i=0; i<4; i++)
		head.numimportant|=vfgetc(fd)<<(i*8);

	if(head.tag!=0x4D42)
	{
		printf("Img_LoadBPX: Invalid BM Tag\n");
		return(NULL);
	}
//	if(head.bpp!=24)
//	{
//		printf("Img_LoadBPX: Bits Per Pixel is not 24 bits (%d)\n",
//			head.bpp);
//		return(NULL);
//	}
//	if(head.compression!=0)
//	{
//		printf("Img_LoadBPX: Don't support compressed images\n");
//		return(NULL);
//	}

	if(head.compression==RIFF_MAKETAG('B', 'C', '7', ' '))
	{
		txc=BGBBTJ_JPG_BC7;
		bsz=16;
	}

	if(head.compression==RIFF_MAKETAG('D', 'X', 'T', '5'))
	{
		txc=BGBBTJ_JPG_BC3;
		bsz=16;
	}

	if(head.compression==RIFF_MAKETAG('D', 'X', 'T', '1'))
	{
		txc=BGBBTJ_JPG_BC1A;
		bsz=8;
	}

	if(w)*w=head.width;
	if(h)*h=head.height;
	if(rtxc)*rtxc=txc;

	bcnt=((head.width+3)>>2)*((head.height+3)>>2)*2;
//	pixlin=head.width*head.height;
	buf=malloc(bcnt*bsz);

	vfseek(fd, head.dataoffs, 0);
	vfread(buf, bsz, bcnt, fd);

#if 0
	for(i=0; i<pixlin; i++)
	{
		b=vfgetc(fd);
		g=vfgetc(fd);
		r=vfgetc(fd);
		a=255;

		buf[(i*4)+0]=r;
		buf[(i*4)+1]=g;
		buf[(i*4)+2]=b;
		buf[(i*4)+3]=a;
	}
#endif

	if(rtxc)*rtxc=txc;
	return(buf);
}

BTEIFGL_API byte *Img_TryLoadTextureBPX(
	char *name, int *w, int *h, int *rtxc)
{
	char tb1[256], tb2[256];
	byte *obuf;
	byte *ct;
	char *s, *t;
	VFILE *fd;

	int tw, th, tl, xs, ys;
	int min, mag, cmp, sz, blksz, fmt;
	int num;

	s=name; t=tb1;
	while(*s)
	{
		if(*s=='/') { s++; *t++='_'; continue; }
		if(*s=='|') { s++; *t++='-'; continue; }
		if(*s=='.')
			break;
		*t++=*s++;
	}
	*t++=0;
	
	sprintf(tb2, "texcache/%s.bpx", tb1);
	fd=vffopen(tb2, "rb");
	if(!fd)return(NULL);

	obuf=Img_LoadBPX(fd, w, h, rtxc);
	vfclose(fd);
	return(obuf);
}

#if 1
BTEIFGL_API int Img_SaveTextureCacheBPX(
	char *name, int w, int h,
	byte *buf, int calcmip)
{
	char tb1[256], tb2[256];
	byte *resampbuf;
	byte *resampbuf_cmp;
	byte *obuf;
	byte *ct;
	char *s, *t;
	VFILE *ofd;

	int tw, th, tl, xs, ys;
	int min, mag, cmp, sz, blksz, fmt;
	int num;

	s=name; t=tb1;
	while(*s)
	{
		if(*s=='/') { s++; *t++='_'; continue; }
		if(*s=='|') { s++; *t++='-'; continue; }
		if(*s=='.')
			break;
		*t++=*s++;
	}
	*t++=0;
	
	sprintf(tb2, "texcache/%s.bpx", tb1);
	ofd=vffopen(tb2, "wb");
	if(!ofd)return(-1);

	tw=1;
	th=1;
	while((tw<<1)<=w)tw<<=1;
	while((th<<1)<=h)th<<=1;
	if(tw<w && (tw<<1)>w)tw<<=1;
	if(th<h && (th<<1)>h)th<<=1;

	if(tw<th) { if((tw*2)<th)th=tw; else tw=th; }
	if(th<tw) { if((th*2)<tw)tw=th; else th=tw; }

	if(tw>2048)tw=2048;
	if(th>2048)th=2048;

	resampbuf=malloc(tw*th*4+256);
	resampbuf_cmp=malloc(tw*th*2+256);

	xs=tw; ys=th;

	if((w!=tw) || (h!=th))
	{
//		Tex_Resample(buf, w, h, resampbuf, tw, th);
		Tex_ResampleSpline(buf, w, h, resampbuf, tw, th);
//		Tex_ResampleSinc(buf, w, h, resampbuf, tw, th);
	}
	else
	{
		memcpy(resampbuf, buf, w*h*4);
	}

	if(tex_use_colorfmt<0)
	{
//		BGBBTJ_BCn_EncodeImageDXT5(resampbuf_cmp, resampbuf, tw, th, 4);
		BGBBTJ_BCn_EncodeImageAutoDXTn(
			resampbuf_cmp, resampbuf, tw, th, 4, &fmt);
		resamp_colorfmt=fmt;
	}else
	{
		fmt=tex_use_colorfmt;
		resamp_colorfmt=fmt;

		BGBBTJ_BCn_EncodeImageDXTn(
			resampbuf_cmp, resampbuf, tw, th, 4, fmt);
		ct=resampbuf_cmp+((tw+3)/4)*((th+3)/4)*16;
	}

	switch(fmt)
	{
	case BGBBTJ_JPG_BC1:
	case BGBBTJ_JPG_BC1F:
		cmp=GL_COMPRESSED_RGB_S3TC_DXT1_EXT; blksz=8;
		break;
	case BGBBTJ_JPG_BC2:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; blksz=16;
		break;
	case BGBBTJ_JPG_BC3:
	case BGBBTJ_JPG_BC3F:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
		break;
	case BGBBTJ_JPG_BC3_UVAY:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
		break;
	case BGBBTJ_JPG_BC6H_SF16:
		cmp=GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT; blksz=16;
		break;
	case BGBBTJ_JPG_BC6H_UF16:
		cmp=GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT; blksz=16;
		break;
	case BGBBTJ_JPG_BC7:
		cmp=GL_COMPRESSED_RGBA_BPTC_UNORM; blksz=16;
		break;
	case BGBBTJ_JPG_BC7_SRGB:
		cmp=GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM; blksz=16;
		break;
	default:
		cmp=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blksz=16;
		break;
	}

	sz=((tw+3)/4)*((th+3)/4)*blksz;
	ct=resampbuf_cmp+sz;

	if(calcmip)
	{
		tl=0;
//		while((tw>1) && (th>1))
		while((tw>1) || (th>1))
		{
			Tex_HalfSample(resampbuf, tw, th);
			tw=(tw+1)>>1; th=(th+1)>>1;
			tl++;
			BGBBTJ_BCn_EncodeImageDXTn(
				ct, resampbuf, tw, th, 4, fmt);
			sz=((tw+3)/4)*((th+3)/4)*blksz;
			if(sz<blksz)sz=blksz;
			ct+=sz;
		}
	}

	obuf=Img_FlattenBPX(resampbuf_cmp, xs, ys, fmt, &sz);
	vfwrite(obuf, 1, sz, ofd);
	vfclose(ofd);

	gcfree(obuf);

	free(resampbuf);
	free(resampbuf_cmp);

//	tex_unlock();
	return(num);
}
#endif

#endif

BTEIFGL_API byte *Img_LoadPNG(VFILE *fd, int *w, int *h)
{
	byte *buf, *obuf;
	int sz;
	
	vfseek(fd, 0, 2);
	sz=vftell(fd);
	vfseek(fd, 0, 0);
	
	buf=malloc(sz+16);
	vfread(buf, 1, sz, fd);

	obuf=BGBBTJ_BufPNG_Decode(buf, sz, w, h);
	free(buf);
	return(obuf);
}

BTEIFGL_API byte *Img_LoadJPG(VFILE *fd, int *w, int *h)
{
	PDJPG_Context *ctx;
	byte *buf, *obuf;
	int xs, ys;
	int sz;
	
	vfseek(fd, 0, 2);
	sz=vftell(fd);
	vfseek(fd, 0, 0);
	
	buf=malloc(sz+16);
	vfread(buf, 1, sz, fd);

//	ctx=PDJPG_AllocPoolContext();
	ctx=PDJPG_AllocContext();
	PDJPG_DecodeBasic(ctx, buf, sz, &xs, &ys);
	obuf=frgl_malloc(xs*ys*4);
	PDJPG_GetImageRGBA(ctx, obuf, xs, ys);
//	PDJPG_FreePoolContext(ctx);
	PDJPG_FreeContext(ctx);
	free(buf);
	if(w)*w=xs;
	if(h)*h=ys;
	return(obuf);
}

//int tex_basecmp(char *s)

BTEIFGL_API int Tex_LoadFile(char *name, int *w, int *h)
{
//	static int hash[1024];
	char tb1[256], tb2[256];

	VFILE *fd;
	byte *buf;
	int i, j, k, n, hi, tw, th, txc;
	char *s, *t;

	if(!name)
	{
		if(w)*w=-1;
		if(h)*h=-1;
		return(-1);
	}

	j=0; t=name;
	while(*t)j=j*251+(*t++);
//	j=((j*251)>>8)&0x3FF;
	j=((j*251)>>8)&4095;
	hi=j;

#if 0
	i=tex_hash2[j];
	if((i>=0) && tex_name[i] && !strcmp(tex_name[i], name))
	{
		if(w)*w=tex_width[i];
		if(h)*h=tex_height[i];
		return(i);
	}
#endif

#if 1
	i=tex_hash2[hi];
	if((i>=0) && tex_name[i])
	{
		if(!strcmp(tex_name[i], name))
		{
			if(w)*w=tex_width[i];
			if(h)*h=tex_height[i];
			return(i);
		}
		
		k=strlen(name);
		if(!strncmp(tex_name[i], name, k) && (tex_name[i][k]=='.'))
		{
			if(w)*w=tex_width[i];
			if(h)*h=tex_height[i];
			return(i);
		}
	}
#endif

#if 0
	i=tex_hash[hi];
	while(i>0)
	{
		if(tex_name[i] && !strcmp(tex_name[i], name))
		{
			if(w)*w=tex_width[i];
			if(h)*h=tex_height[i];
			return(i);
		}
		i=tex_chain[i];
	}
#endif

	t=name+strlen(name);
	while((t>name) && (*t!='.'))t--;

	buf=NULL;
	if(*t!='.')
	{
#if 1
		s=name; t=tb1;
		while(*s)
		{
			if(*s=='/') { s++; *t++='_'; continue; }
			if(*s=='.') break;
			*t++=*s++;
		}
		*t++=0;
		
		sprintf(tb2, "texcache/%s.bpx", tb1);
		i=Tex_LoadFile(tb2, w, h);
		if(i>=0) { tex_hash2[hi]=i; return(i); }
#endif
	
#if 0
		t=gcrsprint("%s.tga", name);
		i=Tex_LoadFile(t, w, h);
//		if(i>=0) { hash[j]=i; return(i); }
//		if(i>=0) { tex_chain[i]=tex_hash[j]; tex_hash[j]=i; return(i); }
		if(i>=0) { tex_hash2[hi]=i; return(i); }
#endif

#if 0
		i=Tex_LoadFileExtAlpha(name, "tga", w, h);
//		if(i>=0) { hash[j]=i; return(i); }
//		if(i>=0) { tex_chain[i]=tex_hash[j]; tex_hash[j]=i; return(i); }
		if(i>=0) { tex_hash2[hi]=i; return(i); }
#endif

#if 1
		i=Tex_LoadFileJpegAlpha(name, w, h);
//		if(i>=0) { hash[j]=i; return(i); }
//		if(i>=0) { tex_chain[i]=tex_hash[j]; tex_hash[j]=i; return(i); }
		if(i>=0) { tex_hash2[hi]=i; return(i); }
#endif

#if 1
		i=Tex_LoadFileExtAlpha(name, "png", w, h);
//		if(i>=0) { hash[j]=i; return(i); }
//		if(i>=0) { tex_chain[i]=tex_hash[j]; tex_hash[j]=i; return(i); }
		if(i>=0) { tex_hash2[hi]=i; return(i); }
#endif

#if 1
		i=Tex_LoadFileExtAlpha(name, "tga", w, h);
//		if(i>=0) { hash[j]=i; return(i); }
//		if(i>=0) { tex_chain[i]=tex_hash[j]; tex_hash[j]=i; return(i); }
		if(i>=0) { tex_hash2[hi]=i; return(i); }
#endif

#if 0
		t=gcrsprint("%s.png", name);
		i=Tex_LoadFile(t, w, h);
//		if(i>=0) { hash[j]=i; return(i); }
//		if(i>=0) { tex_chain[i]=tex_hash[j]; tex_hash[j]=i; return(i); }
		if(i>=0) { tex_hash2[hi]=i; return(i); }
#endif

#if 0
		t=gcrsprint("%s.jpg", name);
		i=Tex_LoadFile(t, w, h);
//		if(i>=0) { hash[j]=i; return(i); }
//		if(i>=0) { tex_chain[i]=tex_hash[j]; tex_hash[j]=i; return(i); }
		if(i>=0) { tex_hash2[hi]=i; return(i); }
#endif

//		t=sprintf(tb1, "%s.bmp", name);
		sprintf(tb1, "%s.bmp", name);
		i=Tex_LoadFile(tb1, w, h);
//		t=gcrsprint("%s.bmp", name);
//		i=Tex_LoadFile(t, w, h);
		if(i>=0) { tex_hash2[hi]=i; return(i); }

//		t=sprintf(tb1, "%s.pcx", name);
		sprintf(tb1, "%s.pcx", name);
		i=Tex_LoadFile(tb1, w, h);
//		t=gcrsprint("%s.pcx", name);
//		i=Tex_LoadFile(t, w, h);
		if(i>=0) { tex_hash2[hi]=i; return(i); }

		return(-1);
	}

#if 1
	i=tex_hash[hi];
	while(i>0)
	{
		if(tex_name[i] && !strcmp(tex_name[i], name))
		{
			if(w)*w=tex_width[i];
			if(h)*h=tex_height[i];
			return(i);
		}
		i=tex_chain[i];
	}
#endif

#if 0
	//brute force search
	for(i=0; i<FRGL_MAX_TEXTURES; i++)
		if(tex_name[i] && !strcmp(tex_name[i], name))
	{
		hash[j]=i;
		if(w)*w=tex_width[i];
		if(h)*h=tex_height[i];
		return(i);
	}
#endif

	if(!gl_texfilter_var)
		Tex_Init2();

	if(!w)w=&tw;
	if(!h)h=&th;

	t++;

	fd=vffopen(name, "rb");
//	fd=btcl_vffopen(name, "rb");
	if(!fd)return(-1);

	buf=NULL; txc=0;

#if 0
	if(!stricmp(t, "bpx"))
	{
//		printf("load bmp %s\n", name);
		buf=Img_LoadBPX(fd, w, h, &txc);
		vfclose(fd);
		fd=NULL;
	}
#endif

	if(!stricmp(t, "tga"))
	{
//		printf("load tga %s\n", name);
		buf=Img_LoadTGA(fd, w, h);
		vfclose(fd);
		fd=NULL;
	}
	if(!stricmp(t, "bmp"))
	{
//		printf("load bmp %s\n", name);
		buf=Img_LoadBMP(fd, w, h);
		vfclose(fd);
		fd=NULL;
	}

#if 0
	if(!stricmp(t, "pcx"))
	{
//		printf("load pcx %s\n", name);
		buf=PCX_Load32(fd, w, h);
		vfclose(fd);
		fd=NULL;
	}
#endif

#if 1
	if(!stricmp(t, "png"))
	{
//		printf("load png %s\n", name);
//		buf=PDPNG_LoadN(fd, w, h, name);
		buf=Img_LoadPNG(fd, w, h);
		vfclose(fd);
		fd=NULL;
	}
#endif

#if 1
	if(!stricmp(t, "jpg") || !stricmp(t, "jpeg"))
	{
//		printf("load jpeg %s\n", name);
//		buf=PDJPG_Load(fd, w, h);
		buf=Img_LoadJPG(fd, w, h);
		vfclose(fd);
		fd=NULL;
	}
#endif

	if(!buf)
	{
		if(fd)vfclose(fd);
		return(-1);
	}

//	printf("file %s loaded %d %d\n", name, *w, *h);

//	if(((*w)!=(*h)) || ((*w)&((*w)-1)))
	if(((*w)&((*w)-1)) || ((*h)&((*h)-1)))
	{
		printf("Texture Size Warning: %s, Size=%d,%d\n", name, *w, *h);
	}

// BTEIFGL_API int Tex_LoadTexture3A(int *wp, int *hp, byte *buf,
//	int num, int clrs)

	if(txc)
	{
		n=Tex_LoadTexture(*w, *h, buf, 1);
//		n=Tex_LoadTexture3B(w, h, buf, 0, txc, 1);
//		tex_buffer[n]=buf;
		tex_buffer[n]=NULL;
		tex_width[n]=*w;
		tex_height[n]=*h;
		tex_name[n]=frgl_strdup(name);

		tex_chain[n]=tex_hash[hi];
		tex_hash[hi]=n;
	}else
	{
//		Img_SaveTextureCacheBPX(name, *w, *h, buf, 1);

		n=Tex_LoadTexture(*w, *h, buf, 1);
//		tex_buffer[n]=buf;
		tex_buffer[n]=NULL;
		tex_width[n]=*w;
		tex_height[n]=*h;
		tex_name[n]=frgl_strdup(name);

		tex_chain[n]=tex_hash[hi];
		tex_hash[hi]=n;
	}

	frgl_free(buf);

//	printf("loaded %d %d %d\n", n, *w, *h);

	return(n);
}

BTEIFGL_API byte *Tex_GetBuffer(int n)
{
	return(tex_buffer[n]);
}

BTEIFGL_API void Tex_FreeRaw(void *p)
{
	free(p);
}

BTEIFGL_API byte *Tex_LoadFileRaw(char *name, int *w, int *h)
{
	VFILE *fd;
	byte *buf;
	char *t;

	fd=vffopen(name, "rb");
//	fd=btcl_vffopen(name, "rb");
	if(!fd)return(NULL);

	t=name+strlen(name);
	while((t>name) && (*t!='.'))t--;
	if(*t=='.')t++;

	buf=NULL;
	if(!stricmp(t, "tga"))
	{
//		printf("load tga raw %s\n", name);
		buf=Img_LoadTGA(fd, w, h);
		vfclose(fd);
	}
	if(!stricmp(t, "bmp"))
	{
//		printf("load bmp raw %s\n", name);
		buf=Img_LoadBMP(fd, w, h);
		vfclose(fd);
	}

#if 0
	if(!stricmp(t, "pcx"))
	{
//		printf("load pcx raw %s\n", name);
		buf=PCX_Load32(fd, w, h);
		vfclose(fd);
	}
#endif

#if 1
	if(!stricmp(t, "png"))
	{
//		printf("load png %s\n", name);
//		buf=PDPNG_Load(fd, w, h);
		buf=Img_LoadPNG(fd, w, h);
		vfclose(fd);
	}
#endif

#if 1
	if(!stricmp(t, "jpg") || !stricmp(t, "jpeg"))
	{
//		printf("load jpeg %s\n", name);
//		buf=PDJPG_Load(fd, w, h);
		buf=Img_LoadJPG(fd, w, h);
		vfclose(fd);
	}
#endif

	return(buf);
}

BTEIFGL_API byte *Tex_LoadFile2Raw(char *name, int *w, int *h)
{
	char tb[512];
	byte *buf;
	
	sprintf(tb, "%s.tga", name);
	buf=Tex_LoadFileRaw(tb, w, h);
	if(buf)return(buf);

	sprintf(tb, "%s.bmp", name);
	buf=Tex_LoadFileRaw(tb, w, h);
	if(buf)return(buf);

	sprintf(tb, "%s.pcx", name);
	buf=Tex_LoadFileRaw(tb, w, h);
	if(buf)return(buf);

	sprintf(tb, "%s.png", name);
	buf=Tex_LoadFileRaw(tb, w, h);
	if(buf)return(buf);

	sprintf(tb, "%s.jpg", name);
	buf=Tex_LoadFileRaw(tb, w, h);
	if(buf)return(buf);
	
	return(NULL);
}

BTEIFGL_API byte *Tex_LoadFileExtAlphaRaw(
	char *name, char *ext, int *w, int *h)
{
	return(Tex_LoadFileExtBaseSuffixRaw(
		name, ext, "", "_alpha", w, h));
}

BTEIFGL_API byte *Tex_LoadFileExtBaseSuffixRaw(
	char *name, char *ext, char *base, char *suff, int *w, int *h)
{
	static char *extlist[]={"png", "jpg", "tga", "bmp", "pcx", NULL};
	char tb0[256], tb1[256];
	byte *buf0, *buf1;
	int xs, ys, xs1, ys1;
	int i;
	
	if(!ext)
	{
		for(i=0; extlist[i]; i++)
		{
			buf0=Tex_LoadFileExtBaseSuffixRaw(name,
				extlist[i], base, suff, w, h);
			if(buf0)return(buf0);
		}
		return(NULL);
	}
	
	if(!base)base="";
	if(!suff)suff="";
	
//	sprintf(tb0, "%s.%s", name, ext);
//	sprintf(tb1, "%s_alpha.%s", name, ext);

	sprintf(tb0, "%s%s.%s", name, base, ext);
	sprintf(tb1, "%s%s.%s", name, suff, ext);
	
	buf0=Tex_LoadFileRaw(tb0, &xs, &ys);
	if(!buf0)return(NULL);

	buf1=Tex_LoadFileRaw(tb1, &xs1, &ys1);
	if(buf1 && (xs1==xs) && (ys1==ys))
	{
		for(i=0; i<(xs*ys); i++)
			buf0[i*4+3]=buf1[i*4+1];
		free(buf1);
	}
	if(w)*w=xs;
	if(h)*h=ys;
	return(buf0);
}

BTEIFGL_API byte *Tex_LoadFileJpegAlphaRaw(char *name, int *w, int *h)
	{ return(Tex_LoadFileExtAlphaRaw(name, "jpg", w, h)); }

BTEIFGL_API int Tex_LoadFileExtAlpha(char *name, char *ext, int *w, int *h)
{
	byte *buf;
	int i, n, xs, ys;

	//brute force search
	for(i=0; i<FRGL_MAX_TEXTURES; i++)
		if(tex_name[i] && !strcmp(tex_name[i], name))
	{
//		hash[j]=i;
		if(w)*w=tex_width[i];
		if(h)*h=tex_height[i];
		return(i);
	}

	if(!gl_texfilter_var)
		Tex_Init2();

	buf=Tex_LoadFileExtAlphaRaw(name, ext, &xs, &ys);
	if(!buf)return(-1);

//	if((xs!=ys) || (xs&(xs-1)))
	if((xs&(xs-1)) || (ys&(ys-1)))
	{
		printf("Texture Size Warning: %s, Size=%d,%d\n", name, xs, ys);
	}

//	Img_SaveTextureCacheBPX(name, xs, ys, buf, 1);

	n=Tex_LoadTexture(xs, ys, buf, 1);
	tex_buffer[n]=buf;
	tex_width[n]=xs;
	tex_height[n]=ys;
	tex_name[n]=strdup(name);

	if(w)*w=xs;
	if(h)*h=ys;
	
	frgl_free(buf);

	return(n);
}

BTEIFGL_API int Tex_LoadFileJpegAlpha(char *name, int *w, int *h)
	{ return(Tex_LoadFileExtAlpha(name, "jpg", w, h)); }

BTEIFGL_API int Tex_LoadFileExtBaseSuffix(
	char *name, char *ext, char *base, char *suff, int *w, int *h)
{
	char tbn[256];
	byte *buf;
	int i, n, xs, ys, txc;

	if(!name)return(-1);

	sprintf(tbn, "%s|%s|%s.%s", name,
		base?base:"_", suff?suff:"_", ext?ext:"_");

	//brute force search
	for(i=0; i<FRGL_MAX_TEXTURES; i++)
		if(tex_name[i] && !strcmp(tex_name[i], tbn))
	{
//		hash[j]=i;
		if(w)*w=tex_width[i];
		if(h)*h=tex_height[i];
		return(i);
	}

	if(!gl_texfilter_var)
		Tex_Init2();

	txc=0; buf=NULL;
//	buf=Img_TryLoadTextureBPX(tbn, &xs, &ys, &txc);

	if(!buf)
		{ buf=Tex_LoadFileExtBaseSuffixRaw(name,
			ext, base, suff, &xs, &ys); }
	if(!buf)return(-1);

	if(txc)
	{
//		n=Tex_LoadTexture3B(&xs, &ys, buf, 0, txc, 1);
		n=Tex_LoadTexture(xs, ys, buf, 1);
		tex_buffer[n]=buf;
		tex_width[n]=xs;
		tex_height[n]=ys;
		tex_name[n]=strdup(tbn);
	}else
	{
//		Img_SaveTextureCacheBPX(tbn, xs, ys, buf, 1);

		n=Tex_LoadTexture(xs, ys, buf, 1);
		tex_buffer[n]=buf;
		tex_width[n]=xs;
		tex_height[n]=ys;
		tex_name[n]=strdup(tbn);
	}
	if(w)*w=xs;
	if(h)*h=ys;

	return(n);
}

BTEIFGL_API int Tex_LoadFileBaseSuffix(
	char *name, char *base, char *suff, int *w, int *h)
{
	return(Tex_LoadFileExtBaseSuffix(name, NULL, base, suff, w, h));
}


BTEIFGL_API byte *Tex_LoadImageBufferRaw(char *name, char *type,
	byte *imgbuf, int imgsz, int *rw, int *rh)
{
//	VFILE *fd;
	byte *buf;
	char *t;

	buf=NULL;
	t=type;

#if 0
	if(!stricmp(type, "jpg") || !stricmp(type, "jpeg"))
	{
//		printf("load jpeg %s\n", name);
		buf=PDJPG_Decode(imgbuf, imgsz, rw, rh);
	}
#endif

#if 0
	if(!stricmp(t, "tga"))
	{
//		printf("load tga raw %s\n", name);
		buf=Img_LoadTGA(fd, w, h);
		vfclose(fd);
	}
#endif

#if 0
	if(!stricmp(t, "bmp"))
	{
//		printf("load bmp raw %s\n", name);
		buf=Img_LoadBMP(fd, w, h);
		vfclose(fd);
	}
#endif

#if 0
	if(!stricmp(t, "pcx"))
	{
//		printf("load pcx raw %s\n", name);
		buf=PCX_Load32(fd, w, h);
		vfclose(fd);
	}
#endif

#if 1
	if(!stricmp(type, "png"))
	{
//		printf("load png %s\n", name);
//		buf=PDPNG_Load(fd, w, h);
//		buf=Img_LoadPNG(fd, rw, rh);
		buf=BGBBTJ_BufPNG_Decode(imgbuf, imgsz, rw, rh);
//		vfclose(fd);
	}
#endif

#if 0
	if(!stricmp(t, "jpg") || !stricmp(t, "jpeg"))
	{
//		printf("load jpeg %s\n", name);
		buf=PDJPG_Load(fd, w, h);
		vfclose(fd);
	}
#endif

	return(buf);
}



BTEIFGL_API int Tex_StoreFile(char *name, byte *buf, int w, int h)
{
	VFILE *fd;
	char *t;

	fd=vffopen(name, "wb");
	if(!fd)return(-1);

	t=name+strlen(name);
	while((t>name) && (*t!='.'))t--;
	if(*t=='.')t++;

	if(!stricmp(t, "tga"))
	{
		Img_StoreTGA(fd, buf, w, h);
		vfclose(fd);
	}

#if 0
	if(!stricmp(t, "bmp"))
	{
		Img_StoreBMP(fd, buf, w, h);
		vfclose(fd);
	}
#endif

#if 0
	if(!stricmp(t, "png"))
	{
		printf("store png %s\n", name);
		PDPNG_Store(fd, buf, w, h);
		vfclose(fd);
	}

	if(!stricmp(t, "jpg") || !stricmp(t, "jpeg"))
	{
		PDJPG_Store(fd, buf, w, h, 85);
		vfclose(fd);
	}
#endif

	return(0);
}

#if 0
BTEIFGL_API int Tex_Screenshot_Clipboard()
{
	int width, height, pixlin;
	byte *buf, *dib;

	width=800;
	height=600;

	pixlin=width*height;
	buf=malloc(pixlin*4);

	glFinish();
	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf); 

	dib=Img_FlattenDIB(buf, width, height);
	free(buf);
	LBXGL_ClipBoard_Export(dib);

	return(0);
}
#endif

BTEIFGL_API int Tex_Screenshot(char *name)
{
	static byte *buf;
	VFILE *fd;
	int width, height, pixlin;

//	width=800;
//	height=600;

	GfxDrv_GetWindowSize(&width, &height);

	pixlin=width*height;
	if(!buf)buf=malloc(pixlin*4);

	glFinish();
	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf); 

//	fd=vffopen(name, "wb");
//	Img_StoreTGA(fd, buf, width, height);
//	PDPNG_Store(fd, buf, width, height);
//	free(buf);

	Tex_StoreFile(name, buf, width, height);

	return(0);
}

BTEIFGL_API int Tex_Screenshot2(char *name)
{
	static byte *buf;
	VFILE *fd;
	int width, height;
	int i, j, k, l;

//	width=800;
//	height=600;

	GfxDrv_GetWindowSize(&width, &height);

	if(!buf)buf=malloc(width*(height+1)*4);

	glFinish();
	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf); 

#if 1
	for(i=0; i<300; i++)
		for(j=0; j<400; j++)
	{
		k=buf[((i*2+0)*800+(j*2+0))*4+0];
		k+=buf[((i*2+0)*800+(j*2+1))*4+0];
		k+=buf[((i*2+1)*800+(j*2+0))*4+0];
		k+=buf[((i*2+1)*800+(j*2+1))*4+0];
		buf[(i*400+j)*4+0]=k>>2;

		k=buf[((i*2+0)*800+(j*2+0))*4+1];
		k+=buf[((i*2+0)*800+(j*2+1))*4+1];
		k+=buf[((i*2+1)*800+(j*2+0))*4+1];
		k+=buf[((i*2+1)*800+(j*2+1))*4+1];
		buf[(i*400+j)*4+1]=k>>2;

		k=buf[((i*2+0)*800+(j*2+0))*4+2];
		k+=buf[((i*2+0)*800+(j*2+1))*4+2];
		k+=buf[((i*2+1)*800+(j*2+0))*4+2];
		k+=buf[((i*2+1)*800+(j*2+1))*4+2];
		buf[(i*400+j)*4+2]=k>>2;
	}
#endif

#if 0
	for(i=0; i<600; i++)
		for(j=0; j<160; j++)
			for(k=0; k<3; k++)
	{
		l=buf[(i*800+(j*5+0))*4+k];
		l+=buf[(i*800+(j*5+1))*4+k];
		l+=buf[(i*800+(j*5+2))*4+k];
		buf[(i*320+j*2+0)*4+k]=l/3;

		l=buf[(i*800+(j*5+2))*4+k];
		l+=buf[(i*800+(j*5+3))*4+k];
		l+=buf[(i*800+(j*5+4))*4+k];
		buf[(i*320+j*2+1)*4+k]=l/3;
	}

	for(i=0; i<120; i++)
		for(j=0; j<320; j++)
			for(k=0; k<3; k++)
	{
		l=buf[((i*5+0)*320+j)*4+k];
		l+=buf[((i*5+1)*320+j)*4+k];
		l+=buf[((i*5+2)*320+j)*4+k];
		buf[((i*2+0)*320+j)*4+k]=l/3;

		l=buf[((i*5+2)*320+j)*4+k];
		l+=buf[((i*5+2)*320+j)*4+k];
		l+=buf[((i*5+4)*320+j)*4+k];
		buf[((i*2+1)*320+j)*4+k]=l/3;
	}
#endif

	Tex_StoreFile(name, buf, 400, 300);

	return(0);
}

BTEIFGL_API int Tex_DoScreenshot()
{
	static int num=0;
	char buf[64];
	int nbase;

//	nbase=bgbrng_genvalue();
	nbase=rand()^(rand()<<12)^(rand()<<16);

//	sprintf(buf, "shot%d.tga", num++);
//	sprintf(buf, "scrnshot/shot%d.png", num++);
	sprintf(buf, "scrnshot/shot_%08X.png", nbase);

	printf("screenshot to %s\n", buf);

	Tex_Screenshot(buf);

	return(0);
}

#if 0
BTEIFGL_API int Tex_DoScreenshotFrame()
{
	static char *base=NULL;
	static int num=0;
	char buf[64];

	if(!base)
	{
		base=strdup(Sys_GetTimeStr2());
		sprintf(buf, "record_%s", base);
		Sys_MkDir(buf);
	}

	sprintf(buf, "record_%s/frame%d.jpg", base, num++);
	Tex_Screenshot2(buf);

	return(0);
}
#endif

BTEIFGL_API int Tex_ScreenshotTexture()
{
	GLuint num;
	byte *data;
	int txs, tys;
	int i;

//	GfxDrv_GetWindowTexSize(&txs, &tys);
	GfxDrv_GetWindowMaxTexSize(&txs, &tys);

	num=Tex_AllocTexnum();
//	tex_width[num]=1024;
//	tex_height[num]=1024;
	tex_width[num]=txs;
	tex_height[num]=tys;

//	data=malloc(1024*1024*4);
//	memset(data, 0, 1024*1024*4);
	data=malloc(txs*tys*4);
	memset(data, 0, txs*tys*4);

	glBindTexture(GL_TEXTURE_2D, num);

#ifdef GLES
	i=GL_RGBA;
#else
	i=FRGL_CvarGetNum("r_usehdr");
	i=(i==1)?GL_RGBA16F_ARB:GL_RGBA;
#endif

//	glTexImage2D(GL_TEXTURE_2D, 0, 4, 1024, 1024, 0,
//		GL_RGBA, GL_UNSIGNED_BYTE, data);
//	glTexImage2D(GL_TEXTURE_2D, 0, i, 1024, 1024, 0,
//		GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, i, txs, tys, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifndef GLES
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif

	free(data);

	return(num);
}

BTEIFGL_API int Tex_ScreenshotDepthTexture()
{
	GLuint num;
	byte *data;
	int txs, tys;
	int i;

#ifdef GLES
	return(-1);

#else

//	GfxDrv_GetWindowTexSize(&txs, &tys);
	GfxDrv_GetWindowMaxTexSize(&txs, &tys);

	while(glGetError());	//clear errors

	num=Tex_AllocTexnum();
//	tex_width[num]=1024;
//	tex_height[num]=1024;
	tex_width[num]=txs;
	tex_height[num]=tys;

//	data=malloc(1024*1024*4);
//	memset(data, 0, 1024*1024*4);

	glBindTexture(GL_TEXTURE_2D, num);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0,
//		GL_DEPTH_COMPONENT, GL_FLOAT, data);

//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0,
//		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL_EXT, 1024, 1024, 0,
//		GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, NULL);

//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL_EXT, 1024, 1024, 0,
//		GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, NULL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL_EXT, txs, tys, 0,
		GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
//	glTexParameteri(GL_TEXTURE_2D,
//		GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);

//	free(data);

	i=glGetError();
	while(i)
	{
		printf("Tex_ScreenshotDepthTexture: Error %08X\n", i);
		i=glGetError();
	}

	return(num);
#endif
}

BTEIFGL_API int Tex_FillTextureScreenshot(int texnum)
{
	static byte *buf;
	VFILE *fd;
	int xo, yo, xs, ys;
	int txs, tys;
	int width, height, pixlin;
	int i;

//	width=800;
//	height=600;

//	GfxDrv_GetWindowSize(&width, &height);
//	GfxDrv_GetWindowTexSize(&txs, &tys);

//	FRGL_GetViewOrigin(&xo, &yo);
//	FRGL_GetViewSize(&xs, &ys);
//	GfxDrv_GetWindowTexSize(&txs, &tys);
	GfxDrv_GetWindowMaxTexSize(&txs, &tys);

//	pixlin=width*height;
//	if(!buf)buf=malloc(pixlin*4);

	glFinish();

#ifdef GLES
	i=GL_RGBA;
#else
	i=FRGL_CvarGetNum("r_usehdr");
	i=(i==1)?GL_RGBA16F_ARB:GL_RGBA;
#endif

	glBindTexture(GL_TEXTURE_2D, texnum);
//	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 1024, 1024, 0);

//	glCopyTexImage2D(GL_TEXTURE_2D, 0, i, 0, 0, 1024, 1024, 0);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, i, 0, 0, txs, tys, 0);

//	glCopyTexImage2D(GL_TEXTURE_2D, 0, i, 0, 0, 800, 600, 0);

//	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf); 

	return(0);
}

BTEIFGL_API int Tex_FillTextureScreenshotDepth(int texnum)
{
#ifndef GLES
	static byte *buf;
	VFILE *fd;
	int txs, tys;
	int width, height, pixlin;
	int i;

//	width=800;
//	height=600;

	GfxDrv_GetWindowSize(&width, &height);
	GfxDrv_GetWindowTexSize(&txs, &tys);

	while(glGetError());	//clear errors

//	printf("Tex_FillTextureScreenshotDepth\n");

//	pixlin=width*height;
//	if(!buf)buf=malloc(pixlin*4);

	glFinish();

	glBindTexture(GL_TEXTURE_2D, texnum);
//	glCopyTexImage2D(GL_TEXTURE_2D, 0,
//		GL_DEPTH_COMPONENT, 0, 0, 1024, 1024, 0);

	glCopyTexImage2D(GL_TEXTURE_2D, 0,
		GL_DEPTH_STENCIL_EXT, 0, 0, txs, tys, 0);

//	glCopyTexImage2D(GL_TEXTURE_2D, 0,
//		GL_DEPTH_STENCIL_EXT, 0, 0, 1024, 1024, 0);
//	glCopyTexImage2D(GL_TEXTURE_2D, 0,
//		GL_DEPTH_STENCIL_EXT, 0, 0, 800, 600, 0);

	i=glGetError();
	while(i)
	{
		printf("Tex_FillTextureScreenshotDepth: Error %08X\n", i);
		i=glGetError();
	}

//	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf); 

	return(0);
#endif

#ifdef GLES
	return(-1);
#endif

}

BTEIFGL_API int Tex_EmptyTexture(int xs, int ys)
{
	GLuint num;
	byte *data;

	num=Tex_AllocTexnum();
	tex_width[num]=xs;
	tex_height[num]=ys;

	data=malloc(xs*ys*4);
	memset(data, 0, xs*ys*4);

	glBindTexture(GL_TEXTURE_2D, num);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, xs, ys, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(data);

	return num;
}

BTEIFGL_API int Tex_EmptyTextureRGB(int xs, int ys)
{
	GLuint num;
	byte *data;

	num=Tex_AllocTexnum();
	tex_width[num]=xs;
	tex_height[num]=ys;

	data=malloc(xs*ys*3);
	memset(data, 0, xs*ys*3);

	glBindTexture(GL_TEXTURE_2D, num);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xs, ys, 0,
		GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(data);

	return num;
}

BTEIFGL_API int Tex_ScreenshotBuffer(byte *buf,
	int xo, int yo, int xs, int ys)
{
//	static byte *buf;
//	int width, height, pixlin;

//	width=800;
//	height=600;

//	GfxDrv_GetWindowSize(&width, &height);

//	pixlin=width*height;
//	if(!buf)buf=malloc(pixlin*4);

	glFinish();
//	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf); 
	glReadPixels (xo, yo, xs, ys, GL_RGBA, GL_UNSIGNED_BYTE, buf); 

//	Tex_StoreFile(name, buf, width, height);

	return(0);
}

BTEIFGL_API int Tex_ScreenshotBufferYUV(byte *obuf,
	int xo, int yo, int xs, int ys)
{
	static byte *buf;
	int width, height, pixlin;
	int cr, cg, cb, cy, cu, cv, cv2;
	int i, j;

//	width=800;
//	height=600;

//	GfxDrv_GetWindowSize(&width, &height);

	pixlin=xs*ys;
	if(!buf)buf=malloc(pixlin*4);

	glFinish();
//	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf); 
	glReadPixels (xo, yo, xs, ys, GL_RGBA, GL_UNSIGNED_BYTE, buf); 

//	Tex_StoreFile(name, buf, width, height);

	for(i=0; i<ys; i++)
		for(j=0; j<xs; j++)
	{
		cr=buf[(i*xs+j)*4+0];
		cg=buf[(i*xs+j)*4+1];
		cb=buf[(i*xs+j)*4+2];
		
		cy=(cr+2*cg+cb)>>2;
		cu=(((cb-cy)*127)>>8)+128;
		cv=(((cr-cy)*127)>>8)+128;
		
		cv2=(j&1)?cv:cu;
		obuf[(i*xs+j)*2+0]=cy;
		obuf[(i*xs+j)*2+1]=cv2;
	}

	return(0);
}

#endif
