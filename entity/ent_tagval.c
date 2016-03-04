#include <bteifgl.h>

BGBDT_EntTagType *bgbdt_tval_types[4096];

int bgbdt_tval_hashname(char *str)
{
	char *s;
	int h;
	
	s=str; h=0;
	while(*s)h=h*251+(*s++);
	h=h*251;
	return((h>>8)&4095);
}

int bgbdt_tval_hashnameraw(char *str)
{
	char *s;
	int h;
	
	s=str; h=0;
	while(*s)h=h*251+(*s++);
	h=h*251;
	return(h);
}

BGBDT_EntTagType *BGBDT_TagVal_LookupTypeName(char *name)
{
	BGBDT_EntTagType *cur;
	int i, j, k;

	i=bgbdt_tval_hashnameraw(name);
	for(j=0; j<256; j++)
	{
		cur=bgbdt_tval_types[(i>>8)&4095];
		if(!cur)break;
		if(!strcmp(cur->name, name))
			return(cur);
		i=i*251+1;
	}
	return(NULL);
}

BGBDT_EntTagType *BGBDT_TagVal_GetTypeName(char *name)
{
	BGBDT_EntTagType *cur;
	int i, j, k;

	i=bgbdt_tval_hashnameraw(name);
	for(j=0; j<256; j++)
	{
		k=(i>>8)&4095;
		cur=bgbdt_tval_types[k];
		if(!cur)
		{
			cur=frgl_malloc(sizeof(BGBDT_EntTagType));
			cur->name=frgl_strdup(name);
			cur->tyid=k;
			bgbdt_tval_types[k]=cur;
			return(cur);
		}
		if(!strcmp(cur->name, name))
			return(cur);
		i=i*251+1;
	}
	return(NULL);
}
