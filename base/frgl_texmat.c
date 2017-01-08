#include <bteifgl.h>

FRGL_TextureMaterial *frgl_texmat_infos[65536];
FRGL_TextureMaterial *frgl_texmat_hash[4096];
FRGL_TextureMaterial *frgl_texmat_current=NULL;
int frgl_texmat_ninfos=1;
float frgl_texmat_expose=1.0;

int frgl_texmat_normdfl;

bool bgbdt_voxel_noshader;

int frgl_shader_pgl_ModelViewMatrix;
int frgl_shader_pgl_ProjectionMatrix;
int frgl_shader_pgl_Color;
int frgl_shader_pgl_SecondaryColor;
int frgl_shader_pgl_Normal;
int frgl_shader_pgl_Vertex;
int frgl_shader_pgl_TexCoord;
int frgl_shader_pgl_TexBase;

int frgl_texmat_init()
{
	static int init=0;

	if(init)
		return(0);
	init=1;

	frgl_texmat_normdfl=Tex_LoadFile("textures/normdfl", NULL, NULL);

	return(1);
}

void FRGL_TexMat_FlushMatShaders(void)
{
	FRGL_TextureMaterial *cur;
	int i;

	for(i=0; i<frgl_texmat_ninfos; i++)
	{
		cur=frgl_texmat_infos[i];
		if(!cur)
			continue;
		cur->use_pgm=0;
	}
}

BTEIFGL_API void FRGL_TexMat_FlushShaders(void)
{
	FRGL_TexMat_FlushMatShaders();
	FRGL_UnloadShaders();
}

int frgl_texmat_hashname(char *name)
{
	char *s;
	int h;
	
	s=name; h=0;
	while(*s)
		h=(h*251)+(*s++);
	return((h>>8)&4095);
}

BTEIFGL_API FRGL_TextureMaterial *FRGL_TexMat_LookupInfo(char *name)
{
	FRGL_TextureMaterial *cur;
	int hi;

	hi=frgl_texmat_hashname(name);
	cur=frgl_texmat_hash[hi];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->hnext;
	}
	return(NULL);
}

BTEIFGL_API FRGL_TextureMaterial *FRGL_TexMat_GetInfo(char *name)
{
	FRGL_TextureMaterial *cur;
	int hi;

	hi=frgl_texmat_hashname(name);
	cur=frgl_texmat_hash[hi];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->hnext;
	}
	
	cur=frgl_talloc("frgl_texmat_t", sizeof(FRGL_TextureMaterial));
	cur->name=frgl_strdup(name);
	cur->matid=frgl_texmat_ninfos++;
	
	frgl_texmat_infos[cur->matid]=cur;
	cur->hnext=frgl_texmat_hash[hi];
	frgl_texmat_hash[hi]=cur;
	return(cur);
}

BTEIFGL_API int FRGL_TexMat_CheckLoadInfo(
	FRGL_TextureMaterial *mat, char *name)
{
	char tb[256];
	char *buf;
	char *cs;
	char **a;
	int sz;
	
	sprintf(tb, "%s_info.txt", name);
	buf=vf_loadfile(tb, &sz);
	if(!buf)
		return(-1);
	
	cs=buf;
	while(*cs)
	{
		cs=frgl_rsgets(tb, 255, cs);
		a=frgl_rsplit(tb);
		if(!a || !a[0])
			continue;
		if(a[0][0]=='/')continue;
		if(a[0][0]==';')continue;
		if(a[0][0]=='#')continue;
		
		if(!strcmp(a[0], "baseimg"))
		{
			mat->base_img=frgl_strdup(a[1]);
			continue;
		}

		if(!strcmp(a[0], "altimg"))
		{
			mat->alt_img=frgl_strdup(a[1]);
			continue;
		}

		if(!strcmp(a[0], "normimg"))
		{
			mat->norm_img=frgl_strdup(a[1]);
			continue;
		}

		if(!strcmp(a[0], "shader"))
		{
//			mat->alt_img=frgl_strdup(a[1]);
			mat->shader=frgl_strdup(a[1]);
//			mat->use_pgm=FRGL_LoadShader(a[1]);
			continue;
		}
	}
	
	frgl_free(buf);
	return(0);
}

BTEIFGL_API int FRGL_TexMat_GetLoadIndex(char *name)
{
	char tb[256];
	FRGL_TextureMaterial *cur;

	cur=FRGL_TexMat_LookupInfo(name);
	if(cur)return(cur->matid);

	frgl_texmat_init();

	cur=FRGL_TexMat_GetInfo(name);
	if(!cur->basetex)
	{
		FRGL_TexMat_CheckLoadInfo(cur, name);

		if(!cur->base_img)
			{ cur->base_img=frgl_strdup(name); }

		if(!cur->alt_img)
		{
			sprintf(tb, "%s_alt", name);
			cur->alt_img=frgl_strdup(tb);
		}

		if(!cur->norm_img)
		{
			sprintf(tb, "%s_norm", name);
			cur->norm_img=frgl_strdup(tb);
		}

//		cur->basetex=Tex_LoadFile(name, NULL, NULL);
		cur->basetex=Tex_LoadFile(cur->base_img, NULL, NULL);
		cur->alt_tex=Tex_LoadFile(cur->alt_img, NULL, NULL);
		cur->norm_tex=Tex_LoadFile(cur->norm_img, NULL, NULL);
	}
	return(cur->matid);
}

BTEIFGL_API int FRGL_TexMat_BindBasic(int idx)
{
	FRGL_TextureMaterial *cur;

	if((idx<0) || (idx>=frgl_texmat_ninfos))
		return(-1);

	cur=frgl_texmat_infos[idx];
	if(!cur)
	{
		if(!idx)
		{
			frgl_texmat_current=NULL;
			glBindTexture(GL_TEXTURE_2D, 0);
			return(0);
		}
		return(-1);
	}
	
	if(cur==frgl_texmat_current)
		return(0);
	
	frgl_texmat_current=cur;
	glBindTexture(GL_TEXTURE_2D, cur->basetex);
	return(0);
}

BTEIFGL_API int FRGL_TexMat_CheckTeardown(FRGL_TextureMaterial *mat)
{
	int i;

	if(!mat)
		return(0);
	if(mat->use_pgm<=0)
		return(0);
	
	for(i=1; i<8; i++)
	{
		frglActiveTexture(i);
		glDisable(GL_TEXTURE_2D);
	}
	frglActiveTexture(0);
	glEnable(GL_TEXTURE_2D);

	FRGL_BindShader(0);
	return(0);
}

BGBDT_VoxWorld *bt2ent_voxworld;

BTEIFGL_API int FRGL_TexMat_BindMaterial(int idx)
{
	FRGL_TextureMaterial *cur;
	int t0;

	if((idx<0) || (idx>=frgl_texmat_ninfos))
		return(-1);

	cur=frgl_texmat_infos[idx];
	if(!cur)
	{
		if(!idx)
		{
			FRGL_TexMat_CheckTeardown(frgl_texmat_current);
			frgl_texmat_current=NULL;
			glBindTexture(GL_TEXTURE_2D, 0);
			return(0);
		}
		return(-1);
	}
	
//#ifndef __EMSCRIPTEN__
#if 1
	if(cur==frgl_texmat_current)
		return(0);
#endif
	
	FRGL_TexMat_CheckTeardown(frgl_texmat_current);
	frgl_texmat_current=cur;


	frglActiveTexture(0);
	if(cur->alt_tex>0)
		{ glBindTexture(GL_TEXTURE_2D, cur->alt_tex); }
	else
		{ glBindTexture(GL_TEXTURE_2D, cur->basetex); }

//	if(cur->use_pgm>0)
	if(cur->shader && !bgbdt_voxel_noshader)
	{
		if(cur->use_pgm<=0)
		{
			cur->use_pgm=FRGL_LoadShader(cur->shader);
			printf("FRGL_TexMat_BindMaterial: Load Shader %s->%d\n",
				cur->shader, cur->use_pgm);
			if(cur->use_pgm<=0)
				return(-1);
		}

		FRGL_BindShader(cur->use_pgm);

		frgl_shader_pgl_ModelViewMatrix=
			FRGL_GetUniformLocation("pgl_ModelViewMatrix");
		frgl_shader_pgl_ProjectionMatrix=
			FRGL_GetUniformLocation("pgl_ProjectionMatrix");

		frgl_shader_pgl_Vertex=-1;

#if 0
		frgl_shader_pgl_Color=
			FRGL_GetAttribLocation("pgl_Color");
//		frgl_shader_pgl_SecondaryColor=
//			FRGL_GetAttribLocation("pgl_SecondaryColor");
		frgl_shader_pgl_Normal=
			FRGL_GetAttribLocation("pgl_Normal");
		frgl_shader_pgl_Vertex=
			FRGL_GetAttribLocation("pgl_Vertex");
		frgl_shader_pgl_TexCoord=
			FRGL_GetAttribLocation("pgl_TexCoord");
#endif

#if 0
		frgl_shader_pgl_Color=
			FRGL_GetAttribLocation("gl_Color");
//		frgl_shader_pgl_SecondaryColor=
//			FRGL_GetAttribLocation("gl_SecondaryColor");
		frgl_shader_pgl_Normal=
			FRGL_GetAttribLocation("gl_Normal");
		frgl_shader_pgl_Vertex=
			FRGL_GetAttribLocation("gl_Vertex");
		frgl_shader_pgl_TexCoord=
			FRGL_GetAttribLocation("gl_TexCoord");
#endif

		frgl_shader_pgl_TexBase=
			FRGL_GetUniformLocation("texBase");

//		frglUniform1i(frgl_shader_pgl_TexBase, 0);
		FRGL_Uniform1i("texBase", 0);

		if(cur->alt_tex>0)
			{ glBindTexture(GL_TEXTURE_2D, cur->alt_tex); }
		else
			{ glBindTexture(GL_TEXTURE_2D, cur->basetex); }

		frglActiveTexture(1);
		glEnable(GL_TEXTURE_2D);
		if(cur->norm_tex>0)
			{ glBindTexture(GL_TEXTURE_2D, cur->norm_tex); }
		else
			{ glBindTexture(GL_TEXTURE_2D, frgl_texmat_normdfl); }
		FRGL_Uniform1i("texNorm", 1);

		FRGL_Uniform1f("exposure", frgl_texmat_expose);

		t0=FRGL_TimeMS();
		FRGL_Uniform1f("time", t0/1000.0);

		FRGL_Uniform3fv("camOrg", 1, bt2ent_voxworld->camorg);
		FRGL_Uniform3fv("camRotX", 1, bt2ent_voxworld->camrot+0);
		FRGL_Uniform3fv("camRotY", 1, bt2ent_voxworld->camrot+3);
		FRGL_Uniform3fv("camRotZ", 1, bt2ent_voxworld->camrot+6);

		frglActiveTexture(0);
	}

	return(0);
}

BTEIFGL_API void FRGL_TexMat_SetExposure(float expose)
{
	frgl_texmat_expose=expose;
}


BTEIFGL_API int FRGL_TexMat_GetLoadSprite(char *name)
{
	static char *suf[]={"bk", "bl", "lf", "fl",  "ft", "fr", "rt", "br"};
	char tb[256];
	FRGL_TextureMaterial *cur;
	int idx;
	int i, j, k;

	cur=FRGL_TexMat_LookupInfo(name);
	if(cur)
		return(cur->matid);

	frgl_texmat_init();

	cur=FRGL_TexMat_GetInfo(name);

	j=Tex_LoadFile(name, NULL, NULL);
	cur->basetex=j;

	for(i=0; i<8; i++)
	{
		sprintf(tb, "%s_%s", name, suf[i]);
//		cur->spr_tex[i]=frgl_strdup(tb);
		j=Tex_LoadFile(tb, NULL, NULL);
		cur->spr_tex[i]=j;
	}
	
	if((cur->basetex>0) && !(cur->spr_tex[4]>0))
		cur->spr_tex[4]=cur->basetex;

	return(cur->matid);
}

int frgl_texmat_boundspriteflip;
int frgl_texmat_boundspriteisside;

BTEIFGL_API int FRGL_TexMat_BoundSpriteFlipP(void)
	{ return(frgl_texmat_boundspriteflip); }

BTEIFGL_API int FRGL_TexMat_BoundSpriteIsSideP(void)
	{ return(frgl_texmat_boundspriteisside); }

BTEIFGL_API int FRGL_TexMat_BindSprite(int idx, int rang)
{
	static byte ftab[8]={0, 7, 6, 5,  4, 3, 2, 1};
	static byte isang[8]={0, 1, 1, 1, 0, 1, 1, 1};
	FRGL_TextureMaterial *cur;
	int ra1, ra2, ra3, ra4;
	int t0;

	if((idx<0) || (idx>=frgl_texmat_ninfos))
		return(-1);

	cur=frgl_texmat_infos[idx];
	
	ra1=(rang+0)&7;
	ra2=(rang+1)&6;
//	ra3=(rang+4)&7;
//	ra4=(rang+5)&6;
	ra3=(ftab[ra1])&7;
	ra4=(ftab[ra2])&6;
	
	frgl_texmat_boundspriteisside=0;
	
	if(cur->spr_tex[ra1]>0)
	{
		frglBindTexture2D(cur->spr_tex[ra1]);
		frgl_texmat_boundspriteflip=0;
		frgl_texmat_boundspriteisside=isang[ra1];
		return(0);
	}

	if(cur->spr_tex[ra2]>0)
	{
		frglBindTexture2D(cur->spr_tex[ra2]);
		frgl_texmat_boundspriteflip=0;
		frgl_texmat_boundspriteisside=isang[ra2];
		return(0);
	}

	if(cur->spr_tex[ra3]>0)
	{
		frglBindTexture2D(cur->spr_tex[ra3]);
		frgl_texmat_boundspriteflip=1;
		frgl_texmat_boundspriteisside=isang[ra3];
		return(0);
	}

	if(cur->spr_tex[ra4]>0)
	{
		frglBindTexture2D(cur->spr_tex[ra4]);
		frgl_texmat_boundspriteflip=1;
		frgl_texmat_boundspriteisside=isang[ra4];
		return(0);
	}

	frglBindTexture2D(cur->basetex);
	frgl_texmat_boundspriteflip=0;
	return(0);
}
