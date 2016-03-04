#include <bteifgl.h>

FRGL_Cvar *cvars_root;

FRGL_Cvar *FRGL_LookupCvar(char *name)
{
	FRGL_Cvar *cur;
	
	cur=cvars_root;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	
	return(NULL);
}

FRGL_Cvar *FRGL_GetCvar(char *name)
{
	FRGL_Cvar *cur;

	cur=FRGL_LookupCvar(name);
	if(cur)return(cur);
	
	cur=frgl_talloc("btge_cvar_t", sizeof(FRGL_Cvar));
	cur->name=frgl_strdup(name);
	cur->next=cvars_root;
	cvars_root=cur;
	
	return(cur);
}

FRGL_Cvar *FRGL_LookupCvarPrefix(char *name)
{
	FRGL_Cvar *cur;
	
	cur=cvars_root;
	while(cur)
	{
		if(!strncmp(cur->name, name, strlen(name)))
			return(cur);
		cur=cur->next;
	}
	
	return(NULL);
}

BTEIFGL_API char *FRGL_CvarComplete(char *name)
{
	FRGL_Cvar *cvar;

	cvar=FRGL_LookupCvarPrefix(name);
	if(cvar)return(cvar->name);
	return(NULL);
}

BTEIFGL_API char *FRGL_CvarGet(char *name)
{
	char tb[256];
	FRGL_Cvar *cvar;
	char *s;

//	cvar=FRGL_GetCvar(name);
	cvar=FRGL_LookupCvar(name);
	if(cvar && cvar->value)
		return(cvar->value);
	return(NULL);

//	sprintf(tb, "console/cvar/%s:value", name);
//	s=dyllMetaLookupKey(tb);
//	if(s)
//	{
//		cvar=FRGL_GetCvar(name);
//		cvar->value=frgl_strdup(s);
//	}
//	return(s);
}

BTEIFGL_API bool FRGL_CvarSet(char *name, char *val)
{
	char tb[256];
	FRGL_Cvar *cvar;
	char *s;

	cvar=FRGL_GetCvar(name);
	if(cvar)cvar->value=frgl_strdup(val);

//	sprintf(tb, "console/cvar/%s", name);
//	dyllMetaBindKey(tb, "cvar");
//	sprintf(tb, "console/cvar/%s:value", name);
//	dyllMetaBindKey(tb, val);
	return(true);
}

BTEIFGL_API bool FRGL_CvarSetDefault(char *name, char *val)
{
	char *s;
	s=FRGL_CvarGet(name);
	if(s)return(false);
	return(FRGL_CvarSet(name, val));
}

BTEIFGL_API double FRGL_CvarGetNum(char *name)
{
	char *s;
	s=FRGL_CvarGet(name);
	if(s)return(atof(s));
	return(0);
}

BTEIFGL_API bool FRGL_CvarSetNum(char *name, double val)
{
	char tb[256];
	sprintf(tb, "%f", val);
	return(FRGL_CvarSet(name, tb));
}

BTEIFGL_API int FRGL_CvarGetInt(char *name)
{
	char *s;
	s=FRGL_CvarGet(name);
	if(s)return(atoi(s));
	return(0);
}
