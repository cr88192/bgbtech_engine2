#include <bteifgl.h>

FRGL_TextureMaterial *frgl_texmat_infos[65536];
FRGL_TextureMaterial *frgl_texmat_hash[4096];
int frgl_texmat_ninfos=0;

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

BTEIFGL_API int FRGL_TexMat_GetLoadIndex(char *name)
{
	FRGL_TextureMaterial *cur;

	cur=FRGL_TexMat_LookupInfo(name);
	if(cur)return(cur->matid);

	cur=FRGL_TexMat_GetInfo(name);
	if(!cur->basetex)
	{
		cur->basetex=Tex_LoadFile(name, NULL, NULL);
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
		return(-1);
	
	glBindTexture(GL_TEXTURE_2D, cur->basetex);
	return(0);
}
