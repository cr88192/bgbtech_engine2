#include <bteifgl.h>

#ifndef GLES

#ifdef _WIN32
#include <windows.h>
#include <wingdi.h>
#include <commctrl.h>
#endif

char *frgl_shader_name[1024];
int frgl_shader_hdl[1024];
int frgl_shader_cnt;
int frgl_shader_cur;

PFNGLATTACHSHADERPROC		pglAttachShader;
PFNGLBINDATTRIBLOCATIONPROC	pglBindAttribLocation;
PFNGLCOMPILESHADERPROC		pglCompileShader;
PFNGLCREATEPROGRAMPROC		pglCreateProgram;
PFNGLCREATESHADERPROC		pglCreateShader;
PFNGLDELETEPROGRAMPROC		pglDeleteProgram;
PFNGLDELETESHADERPROC		pglDeleteShader;
PFNGLDETACHSHADERPROC		pglDetachShader;

PFNGLGETSHADERIVPROC		pglGetShaderiv;
PFNGLGETSHADERINFOLOGPROC		pglGetShaderInfoLog;

PFNGLGETUNIFORMLOCATIONPROC		pglGetUniformLocation;
PFNGLGETUNIFORMFVPROC			pglGetUniformfv;
PFNGLGETUNIFORMIVPROC			pglGetUniformiv;
PFNGLGETVERTEXATTRIBDVPROC		pglGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC		pglGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC		pglGetVertexAttribuv;

PFNGLVERTEXATTRIBPOINTERPROC		pglVertexAttribPointer;
PFNGLGETVERTEXATTRIBPOINTERVPROC	pglGetVertexAttribPointerv;

PFNGLLINKPROGRAMPROC		pglLinkProgram;
PFNGLSHADERSOURCEPROC		pglShaderSource;
PFNGLUSEPROGRAMPROC		pglUseProgram;

PFNGLUNIFORM1FPROC		pglUniform1f;
PFNGLUNIFORM2FPROC		pglUniform2f;
PFNGLUNIFORM3FPROC		pglUniform3f;
PFNGLUNIFORM4FPROC		pglUniform4f;
PFNGLUNIFORM1IPROC		pglUniform1i;
PFNGLUNIFORM2IPROC		pglUniform2i;
PFNGLUNIFORM3IPROC		pglUniform3i;
PFNGLUNIFORM4IPROC		pglUniform4i;
PFNGLUNIFORM1FVPROC		pglUniform1fv;
PFNGLUNIFORM2FVPROC		pglUniform2fv;
PFNGLUNIFORM3FVPROC		pglUniform3fv;
PFNGLUNIFORM4FVPROC		pglUniform4fv;
PFNGLUNIFORM1IVPROC		pglUniform1iv;
PFNGLUNIFORM2IVPROC		pglUniform2iv;
PFNGLUNIFORM3IVPROC		pglUniform3iv;
PFNGLUNIFORM4IVPROC		pglUniform4iv;
PFNGLUNIFORMMATRIX2FVPROC	pglUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC	pglUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC	pglUniformMatrix4fv;

void *frglGetProcAddress(char *name)
{
#ifdef _WIN32
	return(wglGetProcAddress(name));
#else
	return(dyllGetAddr(name));
#endif
}

BTEIFGL_API int FRGL_InitShader()
{
	static int init=0;

	if(init)return(0);
	init=1;

	printf("FRGL_InitShader\n");

	pglAttachShader=frglGetProcAddress("glAttachShader");
	pglBindAttribLocation=frglGetProcAddress("glBindAttribLocation");
	pglCompileShader=frglGetProcAddress("glCompileShader");
	pglCreateProgram=frglGetProcAddress("glCreateProgram");
	pglCreateShader=frglGetProcAddress("glCreateShader");
	pglDeleteProgram=frglGetProcAddress("glDeleteProgram");
	pglDeleteShader=frglGetProcAddress("glDeleteShader");
	pglDetachShader=frglGetProcAddress("glDetachShader");

	pglGetShaderiv=frglGetProcAddress("glGetShaderiv");
	pglGetShaderInfoLog=frglGetProcAddress("glGetShaderInfoLog");

	pglGetUniformLocation=frglGetProcAddress("glGetUniformLocation");
	pglGetUniformfv=frglGetProcAddress("glGetUniformfv");
	pglGetUniformiv=frglGetProcAddress("glGetUniformiv");
	pglGetVertexAttribdv=frglGetProcAddress("glGetVertexAttribdv");
	pglGetVertexAttribfv=frglGetProcAddress("glGetVertexAttribfv");
	pglGetVertexAttribuv=frglGetProcAddress("glGetVertexAttribuv");

	pglVertexAttribPointer=
		frglGetProcAddress("glVertexAttribPointer");
	pglGetVertexAttribPointerv=
		frglGetProcAddress("glGetVertexAttribPointerv");

	pglLinkProgram=frglGetProcAddress("glLinkProgram");
	pglShaderSource=frglGetProcAddress("glShaderSource");
	pglUseProgram=frglGetProcAddress("glUseProgram");

	pglUniform1f=frglGetProcAddress("glUniform1f");
	pglUniform2f=frglGetProcAddress("glUniform2f");
	pglUniform3f=frglGetProcAddress("glUniform3f");
	pglUniform4f=frglGetProcAddress("glUniform4f");
	pglUniform1i=frglGetProcAddress("glUniform1i");
	pglUniform2i=frglGetProcAddress("glUniform2i");
	pglUniform3i=frglGetProcAddress("glUniform3i");
	pglUniform4i=frglGetProcAddress("glUniform4i");
	pglUniform1fv=frglGetProcAddress("glUniform1fv");
	pglUniform2fv=frglGetProcAddress("glUniform2fv");
	pglUniform3fv=frglGetProcAddress("glUniform3fv");
	pglUniform4fv=frglGetProcAddress("glUniform4fv");
	pglUniform1iv=frglGetProcAddress("glUniform1iv");
	pglUniform2iv=frglGetProcAddress("glUniform2iv");
	pglUniform3iv=frglGetProcAddress("glUniform3iv");
	pglUniform4iv=frglGetProcAddress("glUniform4iv");
	pglUniformMatrix2fv=frglGetProcAddress("glUniformMatrix2fv");
	pglUniformMatrix3fv=frglGetProcAddress("glUniformMatrix3fv");
	pglUniformMatrix4fv=frglGetProcAddress("glUniformMatrix4fv");

	printf("FRGL_InitShader: %s\n", pglCreateShader?"OK":"Fail");

	frgl_shader_cnt=0;

	return(1);
}

BTEIFGL_API int FRGL_LookupShaderSource(char *name, int ty)
{
	char tb[256];
	int i, j;

	sprintf(tb, "shaders/%s.%s", name,
		(ty==GL_VERTEX_SHADER)?"vert":"frag");

	for(i=0; i<frgl_shader_cnt; i++)
		if(!strcmp(frgl_shader_name[i], tb))
			return(frgl_shader_hdl[i]);
	return(-1);
}

BTEIFGL_API int FRGL_LoadShaderSource(char *name, int ty)
{
	char tb[4096];
	VFILE *fd;
	char *buf;
	int i, j;

	if(!pglShaderSource)
		return(-1);

	if((ty!=GL_VERTEX_SHADER) && (ty!=GL_FRAGMENT_SHADER))
		return(-1);

	i=FRGL_LookupShaderSource(name, ty);
	if(i>0)return(i);

	sprintf(tb, "shaders/%s.%s", name,
		(ty==GL_VERTEX_SHADER)?"vert":"frag");
	fd=vffopen(tb, "rb");
//	fd=btcl_vffopen(tb, "rb");
	if(!fd)return(-1);
	buf=vf_bufferin(fd);
	vfclose(fd);

	i=pglCreateShader(ty);
	pglShaderSource(i, 1, (const char **)(&buf), NULL);
	pglCompileShader(i);

	pglGetShaderiv(i, GL_COMPILE_STATUS, &j);
	if(j==GL_FALSE)
	{
		printf("Fail compile shader %s\n", tb);
		
		pglGetShaderInfoLog(i, 4096, &j, tb);
		printf("%s\n", tb);
		
		return(-1);
	}

	j=frgl_shader_cnt++;
	frgl_shader_name[j]=strdup(tb);
	frgl_shader_hdl[j]=i;

	return(i);
}


BTEIFGL_API int FRGL_LookupShader(char *name)
{
	int i, j;

	for(i=0; i<frgl_shader_cnt; i++)
		if(!strcmp(frgl_shader_name[i], name))
			return(frgl_shader_hdl[i]);
	return(-1);
}

BTEIFGL_API int FRGL_LoadShader(char *name)
{
	int i, j, k;

	FRGL_InitShader();

	i=FRGL_LookupShader(name);
	if(i>0)return(i);

	if(!pglCreateProgram || !pglLinkProgram)
		return(-1);

	i=FRGL_LoadShaderSource(name, GL_VERTEX_SHADER);
	j=FRGL_LoadShaderSource(name, GL_FRAGMENT_SHADER);

	if((i<0) || (j<0))
		return(-1);

	k=pglCreateProgram();
	pglAttachShader(k, i);
	pglAttachShader(k, j);
	pglLinkProgram(k);

	i=frgl_shader_cnt++;
	frgl_shader_name[i]=strdup(name);
	frgl_shader_hdl[i]=k;

	return(k);
}

BTEIFGL_API int FRGL_BindShader(int num)
{
	if(frgl_shader_cur==num)
		return(0);

	if(!pglUseProgram)
		return(-1);
	if(num<=0)
	{
		frgl_shader_cur=0;
		pglUseProgram(0);
		return(0);
	}

	frgl_shader_cur=num;
	pglUseProgram(num);
	return(0);
}

BTEIFGL_API int FRGL_UnbindShader()
{
	FRGL_BindShader(0);
	return(0);
}

BTEIFGL_API int FRGL_CheckGlExtension(char *str)
{
	return(0);
//	return(BGBBTJ_CheckGlExtension(str));
}

#ifndef GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#endif

#ifndef GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
#endif

BTEIFGL_API int FRGL_GetFreeVideoMemory()
{
	int mem[]={-1,-1,-1,-1};

	if(FRGL_CheckGlExtension("GL_NVX_gpu_memory_info"))
	{
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
			&mem[0]);
//		printf("FRGL_GetFreeVideoMemory: NVidia\n");
	}

	if(FRGL_CheckGlExtension("GL_ATI_meminfo"))
	{
		glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI,
			mem);
//		printf("FRGL_GetFreeVideoMemory: ATI/AMD\n");
	}

	return(mem[0]);
}


BTEIFGL_API int frglGetUniformLocation(int shader, char *name)
{
	int i;
//	return(frglGetAttribLocation(frgl_shader_cur, name));

	if(!pglGetUniformLocation)return(-1);
	i=pglGetUniformLocation(shader, name);
	return(i);
}

BTEIFGL_API int frglGetUniformLocationF(int shader, char *name)
{
	static int hash_sdr[4096];
	static char *hash_name[4096];
	static int hash_var[4096];
	char *s;
	int i, hi;
//	return(frglGetAttribLocation(frgl_shader_cur, name));

//	hi=shader*4093; s=name;
//	while(*s) { hi=(hi*4093)+(*s++); }

//	hi=((shader*4093)+((nlint)name))*4093;
	hi=((shader*4093)+((long)name))*4093;
	hi=(hi>>12)&4095;
	
	if((hash_sdr[hi]==shader) && (hash_name[hi]==name))
//	if((hash_sdr[hi]==shader) && hash_name[hi] &&
//		!strcmp(hash_name[hi], name))
	{
		return(hash_var[hi]);
	}

	if(!pglGetUniformLocation)return(-1);
	i=pglGetUniformLocation(shader, name);
	
	if(i>0)
//	if(i>=0)
	{
		hash_sdr[hi]=shader;
		hash_name[hi]=name;
		hash_var[hi]=i;
	}

	return(i);
}

BTEIFGL_API void FRGL_ErrorStatusUniform(char *name)
{
	int i;

	i=glGetError();
	while(i!=GL_NO_ERROR)
	{
		printf("FRGL_ErrorStatusUniform: Var=%s Error 0x%04X\n", name, i);
		*(int *)-1=-1;
		i=glGetError();
	}
}

BTEIFGL_API int FRGL_CheckForUniform(char *name)
{
	int i;
	
	i=frglGetUniformLocation(frgl_shader_cur, name);
	if(i<0)return(0);
	return(1);
}

BTEIFGL_API int FRGL_Uniform1f(char *name, float x)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform1f(i, x);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform2f(char *name, float x, float y)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform2f(i, x, y);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform3f(char *name, float x, float y, float z)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform3f(i, x, y, z);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform4f(char *name, float x, float y, float z, float w)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform4f(i, x, y, z, w);
	FRGL_ErrorStatusUniform(name);
	return(1);
}


BTEIFGL_API int FRGL_Uniform1i(char *name, int x)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform1i(i, x);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform2i(char *name, int x, int y)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform2i(i, x, y);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform3i(char *name, int x, int y, int z)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform3i(i, x, y, z);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform4i(char *name, int x, int y, int z, int w)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform4i(i, x, y, z, w);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform1fv(char *name, int cnt, float *pf)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform1fv(i, cnt, pf);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform2fv(char *name, int cnt, float *pf)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform2fv(i, cnt, pf);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform3fv(char *name, int cnt, float *pf)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform3fv(i, cnt, pf);
	FRGL_ErrorStatusUniform(name);
	return(1);
}

BTEIFGL_API int FRGL_Uniform4fv(char *name, int cnt, float *pf)
{
	int i;
	if(!pglGetUniformLocation)return(-1);
//	i=pglGetUniformLocation(frgl_shader_cur, name);
	i=frglGetUniformLocationF(frgl_shader_cur, name);
	if(i<0)return(0); pglUniform4fv(i, cnt, pf);
	FRGL_ErrorStatusUniform(name);
	return(1);
}


#if 1
BTEIFGL_API void frglUniform1f(int var, float x)
	{ pglUniform1f(var, x); }
BTEIFGL_API void frglUniform2f(int var, float x, float y)
	{ pglUniform2f(var, x, y); }
BTEIFGL_API void frglUniform3f(int var, float x, float y, float z)
	{ pglUniform3f(var, x, y, z); }
BTEIFGL_API void frglUniform4f(int var, float x, float y, float z, float w)
	{ pglUniform4f(var, x, y, z, w); }

BTEIFGL_API void frglUniform1i(int var, int x)
	{ pglUniform1i(var, x); }
BTEIFGL_API void frglUniform2i(int var, int x, int y)
	{ pglUniform2i(var, x, y); }
BTEIFGL_API void frglUniform3i(int var, int x, int y, int z)
	{ pglUniform3i(var, x, y, z); }
BTEIFGL_API void frglUniform4i(int var, int x, int y, int z, int w)
	{ pglUniform4i(var, x, y, z, w); }

BTEIFGL_API void frglUniform1fv(int var, int cnt, float *pf)
	{ pglUniform1fv(var, cnt, pf); }
BTEIFGL_API void frglUniform2fv(int var, int cnt, float *pf)
	{ pglUniform2fv(var, cnt, pf); }
BTEIFGL_API void frglUniform3fv(int var, int cnt, float *pf)
	{ pglUniform3fv(var, cnt, pf); }
BTEIFGL_API void frglUniform4fv(int var, int cnt, float *pf)
	{ pglUniform4fv(var, cnt, pf); }
#endif

BTEIFGL_API int frglVertexAttrib1f(int index, float v0)
{
	static PFNGLVERTEXATTRIB1FPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glVertexAttrib1f");
		if(!fcn)return(-1);
	}

	fcn(index, v0);
	return(0);
}

BTEIFGL_API int frglVertexAttrib2f(int index, float v0, float v1)
{
	static PFNGLVERTEXATTRIB2FPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glVertexAttrib2f");
		if(!fcn)return(-1);
	}

	fcn(index, v0, v1);
	return(0);
}

BTEIFGL_API int frglVertexAttrib3f(int index, float v0, float v1, float v2)
{
	static PFNGLVERTEXATTRIB3FPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glVertexAttrib3f");
		if(!fcn)return(-1);
	}

	fcn(index, v0, v1, v2);
	return(0);
}

BTEIFGL_API int frglVertexAttrib4f(int index,
	float v0, float v1, float v2, float v3)
{
	static PFNGLVERTEXATTRIB4FPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glVertexAttrib4f");
		if(!fcn)return(-1);
	}

	fcn(index, v0, v1, v2, v3);
	return(0);
}

BTEIFGL_API int frglVertexAttrib1fv(int index, float *v)
{
	static PFNGLVERTEXATTRIB1FVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glVertexAttrib1fv");
		if(!fcn)return(-1);
	}

	fcn(index, v);
	return(0);
}

BTEIFGL_API int frglVertexAttrib2fv(int index, float *v)
{
	static PFNGLVERTEXATTRIB2FVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glVertexAttrib2fv");
		if(!fcn)return(-1);
	}

	fcn(index, v);
	return(0);
}

BTEIFGL_API int frglVertexAttrib3fv(int index, float *v)
{
	static PFNGLVERTEXATTRIB3FVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glVertexAttrib3fv");
		if(!fcn)return(-1);
	}

	fcn(index, v);
	return(0);
}

BTEIFGL_API int frglVertexAttrib4fv(int index, float *v)
{
	static PFNGLVERTEXATTRIB4FVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glVertexAttrib4fv");
		if(!fcn)return(-1);
	}

	fcn(index, v);
	return(0);
}



BTEIFGL_API int frglGetAttribLocation(int program, char *name)
{
	static PFNGLGETATTRIBLOCATIONPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glGetAttribLocation");
		if(!fcn)return(-1);
	}

	return(fcn(program, name));
}

BTEIFGL_API int FRGL_GetAttribLocation(char *name)
{
	return(frglGetAttribLocation(frgl_shader_cur, name));
}

BTEIFGL_API int frglVertexAttribPointer(
	int index, int size, int type, int normalized, int stride,
	void *pointer)
{
	if(!pglVertexAttribPointer)return(-1);
	pglVertexAttribPointer(index, size, type,
		normalized, stride, pointer);
	return(0);
}

BTEIFGL_API int frglEnableVertexAttribArray(int idx)
{
	static PFNGLENABLEVERTEXATTRIBARRAYPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glEnableVertexAttribArray");
		if(!fcn)return(-1);
	}

	fcn(idx);
	return(0);
}

BTEIFGL_API int frglDisableVertexAttribArray(int idx)
{
	static PFNGLDISABLEVERTEXATTRIBARRAYPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glDisableVertexAttribArray");
		if(!fcn)return(-1);
	}

	fcn(idx);
	return(0);
}

BTEIFGL_API int frglActiveTexture(int idx)
{
	static PFNGLACTIVETEXTUREPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glActiveTexture");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx);
	return(0);
}

BTEIFGL_API int frglMultiTexCoord1f(int idx, float s)
{
	static PFNGLMULTITEXCOORD1FPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glMultiTexCoord1f");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx, s);
	return(0);
}

BTEIFGL_API int frglMultiTexCoord2f(int idx, float s, float t)
{
	static PFNGLMULTITEXCOORD2FPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glMultiTexCoord2f");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx, s, t);
	return(0);
}

BTEIFGL_API int frglMultiTexCoord3f(int idx, float s, float t, float p)
{
	static PFNGLMULTITEXCOORD3FPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glMultiTexCoord3f");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx, s, t, p);
	return(0);
}

BTEIFGL_API int frglMultiTexCoord4f(int idx, float s, float t, float p, float q)
{
	static PFNGLMULTITEXCOORD4FPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glMultiTexCoord4f");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx, s, t, p, q);
	return(0);
}


BTEIFGL_API int frglMultiTexCoord1fv(int idx, float *v)
{
	static PFNGLMULTITEXCOORD1FVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glMultiTexCoord1fv");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx, v);
	return(0);
}

BTEIFGL_API int frglMultiTexCoord2fv(int idx, float *v)
{
	static PFNGLMULTITEXCOORD2FVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glMultiTexCoord2fv");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx, v);
	return(0);
}

BTEIFGL_API int frglMultiTexCoord3fv(int idx, float *v)
{
	static PFNGLMULTITEXCOORD3FVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glMultiTexCoord3fv");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx, v);
	return(0);
}

BTEIFGL_API int frglMultiTexCoord4fv(int idx, float *v)
{
	static PFNGLMULTITEXCOORD4FVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glMultiTexCoord4fv");
		if(!fcn)return(-1);
	}

	fcn(GL_TEXTURE0+idx, v);
	return(0);
}

BTEIFGL_API int frglBeginQuery(int tgt, int id)
{
	static PFNGLBEGINQUERYPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glBeginQuery");
		if(!fcn)return(-1);
	}

	fcn(tgt, id);
	return(0);
}

BTEIFGL_API int frglEndQuery(int tgt)
{
	static PFNGLENDQUERYPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glEndQuery");
		if(!fcn)return(-1);
	}

	fcn(tgt);
	return(0);
}

BTEIFGL_API int frglGetQueryObjectiv(int id, int pname, int *params)
{
	static PFNGLGETQUERYOBJECTIVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glGetQueryObjectiv");
		if(!fcn)return(-1);
	}

	fcn(id, pname, params);
	return(0);
}

BTEIFGL_API int frglGetQueryObjectuiv(int id, int pname,
	unsigned int *params)
{
	static PFNGLGETQUERYOBJECTUIVPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glGetQueryObjectuiv");
		if(!fcn)return(-1);
	}

	fcn(id, pname, params);
	return(0);
}


BTEIFGL_API int frglGenFramebuffers(int n, int *ids)
{
	static PFNGLGENFRAMEBUFFERSEXTPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glGenFramebuffersEXT");
		if(!fcn)return(-1);
	}

	fcn(n, ids);
	return(0);
}

BTEIFGL_API int frglGenRenderbuffers(int n, int *ids)
{
	static PFNGLGENRENDERBUFFERSEXTPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glGenRenderbuffersEXT");
		if(!fcn)return(-1);
	}

	fcn(n, ids);
	return(0);
}

BTEIFGL_API int frglBindFramebuffer(int target, int framebuffer)
{
	static PFNGLBINDFRAMEBUFFEREXTPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glBindFramebufferEXT");
		if(!fcn)return(-1);
	}

	fcn(target, framebuffer);
	return(0);
}

BTEIFGL_API int frglBindRenderbuffer(int target, int framebuffer)
{
	static PFNGLBINDRENDERBUFFEREXTPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glBindRenderbufferEXT");
		if(!fcn)return(-1);
	}

	fcn(target, framebuffer);
	return(0);
}

BTEIFGL_API int frglRenderbufferStorage(int target,
	int internalformat, int width, int height)
{
	static PFNGLRENDERBUFFERSTORAGEEXTPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glRenderbufferStorageEXT");
		if(!fcn)return(-1);
	}

	fcn(target, internalformat, width, height);
	return(0);
}

BTEIFGL_API int frglFramebufferTexture2D(
	int target, int attachment, int textarget, int texture, int level)
{
	static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glFramebufferTexture2DEXT");
		if(!fcn)return(-1);
	}

	fcn(target, attachment, textarget, texture, level);
	return(0);
}

BTEIFGL_API int frglFramebufferRenderbuffer(
	int target, int attachment, int textarget, int texture)
{
	static PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glFramebufferRenderbufferEXT");
		if(!fcn)return(-1);
	}

	fcn(target, attachment, textarget, texture);
	return(0);
}

BTEIFGL_API int frglCheckFramebufferStatus(int target)
{
	static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC fcn=NULL;
	static int set=0;
	int i;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glCheckFramebufferStatusEXT");
		if(!fcn)return(-1);
	}

	i=fcn(target);
	return(i);
}

BTEIFGL_API int frglDrawBuffers(int n, int *ids)
{
	static PFNGLDRAWBUFFERSPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glDrawBuffers");
		if(!fcn)return(-1);
	}

	fcn(n, ids);
	return(0);
}


BTEIFGL_API int frglGenBuffers(int n, int *ids)
{
	static PFNGLGENBUFFERSPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glGenBuffers");
		if(!fcn)return(-1);
	}

	fcn(n, ids);
	return(0);
}

BTEIFGL_API int frglDeleteBuffers(int n, int *ids)
{
	static PFNGLGENBUFFERSPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glDeleteBuffers");
		if(!fcn)return(-1);
	}

	fcn(n, ids);
	return(0);
}

BTEIFGL_API int frglBindBuffer(int target, int buffer)
{
	static PFNGLBINDBUFFERPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glBindBuffer");
		if(!fcn)return(-1);
	}

	fcn(target, buffer);
	return(0);
}

BTEIFGL_API int frglBufferData(
	int target, int size,
	const void *data, int usage)
{
	static PFNGLBUFFERDATAPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glBufferData");
		if(!fcn)return(-1);
	}

	fcn(target, size, data, usage);
	return(0);
}

BTEIFGL_API int frglBufferSubData(
	int target, int offset, int size,
	const void *data)
{
	static PFNGLBUFFERSUBDATAPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glBufferSubData");
		if(!fcn)return(-1);
	}

	fcn(target, offset, size, data);
	return(0);
}

BTEIFGL_API void *frglMapBuffer(int target, int access)
{
	static PFNGLMAPBUFFERPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(NULL); set=1;
		fcn=frglGetProcAddress("glMapBuffer");
		if(!fcn)return(NULL);
	}

	return(fcn(target, access));
}

BTEIFGL_API bool frglUnmapBuffer(int target)
{
	static PFNGLUNMAPBUFFERPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(false); set=1;
		fcn=frglGetProcAddress("glUnmapBuffer");
		if(!fcn)return(false);
	}

	return(fcn(target));
}


BTEIFGL_API int frglCompressedTexImage2D(
	int		target,
	int		level,
	int		internalformat,
	int		width,
	int		height,
	int		border,
	int		imageSize,
	const void *data)
{
	static PFNGLCOMPRESSEDTEXIMAGE2DPROC fcn=NULL;
	static int set=0;

	if(!fcn)
	{
		if(set)return(-1); set=1;
		fcn=frglGetProcAddress("glCompressedTexImage2D");
		if(!fcn)return(-1);
	}

	fcn(target, level, internalformat, width, height,
		border, imageSize, data);
	return(0);
}

#endif
