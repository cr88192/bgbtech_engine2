#include <bteifgl.h>

FRGL_TextureMaterial *frgl_texmat_infos[65536];
FRGL_TextureMaterial *frgl_texmat_hash[4096];
FRGL_TextureMaterial *frgl_texmat_current=NULL;
int frgl_texmat_ninfos=1;
float frgl_texmat_expose=1.0;

int frgl_texmat_normdfl;

int frgl_texmat_init()
{
	static int init=0;

	if(init)
		return(0);
	init=1;

	frgl_texmat_normdfl=Tex_LoadFile("textures/normdfl", NULL, NULL);

	return(1);
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
			mat->use_pgm=FRGL_LoadShader(a[1]);
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
	
	if(cur==frgl_texmat_current)
		return(0);
	
	FRGL_TexMat_CheckTeardown(frgl_texmat_current);
	frgl_texmat_current=cur;


	frglActiveTexture(0);
	if(cur->alt_tex>0)
		{ glBindTexture(GL_TEXTURE_2D, cur->alt_tex); }
	else
		{ glBindTexture(GL_TEXTURE_2D, cur->basetex); }

	if(cur->use_pgm>0)
	{
		FRGL_BindShader(cur->use_pgm);
		FRGL_Uniform1i("texBase", 0);

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
