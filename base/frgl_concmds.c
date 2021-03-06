#include <bteifgl.h>

FRGL_ConCmd *cmds_root;

BTEIFGL_API FRGL_ConCmd *FRGL_LookupConCmd(char *name)
{
	FRGL_ConCmd *cur;
	
	cur=cmds_root;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	
	return(NULL);
}

BTEIFGL_API FRGL_ConCmd *FRGL_GetConCmd(char *name)
{
	FRGL_ConCmd *cur;

	cur=FRGL_LookupConCmd(name);
	if(cur)return(cur);
	
	cur=frgl_talloc("frgl_concmd_t", sizeof(FRGL_ConCmd));
	cur->name=frgl_strdup(name);
	cur->next=cmds_root;
	cmds_root=cur;
	
	return(cur);
}

BTEIFGL_API FRGL_ConCmd *FRGL_LookupConCmdPrefix(char *name)
{
	FRGL_ConCmd *cur;
	
	cur=cmds_root;
	while(cur)
	{
		if(!strncmp(cur->name, name, strlen(name)))
			return(cur);
		cur=cur->next;
	}
	
	return(NULL);
}

BTEIFGL_API char *FRGL_ConCmdComplete(char *name)
{
	FRGL_ConCmd *cvar;

	cvar=FRGL_LookupConCmdPrefix(name);
	if(cvar)return(cvar->name);
	return(NULL);
}

BTEIFGL_API FRGL_ConCmd *FRGL_RegisterConCmd(char *name, char *desc,
	int (*func)(FRGL_ConsoleInfo *con, char **args))
{
	FRGL_ConCmd *cur;

	cur=FRGL_GetConCmd(name);
	if(cur)
	{
		cur->Func=func;
		return(cur);
	}
	return(NULL);
}
