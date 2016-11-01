/*
Written 2007-2009 by Brendan G Bohannon
This code is re-assigned into the public domain.
*/

#include <bgbasm.h>


BASM_XMeta *basm_xmeta_root=NULL;

BASM_XMeta *BASM_LookupXMetaInfo(char *name)
{
	BASM_XMeta *cur;

	cur=basm_xmeta_root;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}
	return(NULL);
}

BASM_XMeta *BASM_GetXMetaInfo(char *name)
{
	BASM_XMeta *cur;

	cur=BASM_LookupXMetaInfo(name);
	if(cur)return(cur);

	cur=malloc(sizeof(BASM_XMeta));
	memset(cur, 0, sizeof(BASM_XMeta));

	cur->name=basm_strdup(name);
	cur->next=basm_xmeta_root;
	basm_xmeta_root=cur;

	return(cur);
}

void *BASM_GetLabelXMeta(char *name)
{
	BASM_XMeta *cur;
	char **a;
	void *p;
	int i;

	a=BASM_UnmangleSplit(name+4);
	if(!a)return(NULL);
	if(!a[0]) { free(a); return(NULL); }

	cur=BASM_LookupXMetaInfo(a[0]);
	if(!cur)
	{
		for(i=0; a[i]; i++)free(a[i]);
		free(a);
		return(NULL);
	}

	p=NULL;
	if(cur->mfcn)p=cur->mfcn(name, a[0], a+1);
	for(i=0; a[i]; i++)free(a[i]);
	free(a);
	return(p);
}

void BASM_NotifyLabelXMeta(char *name, void *ptr)
{
	BASM_XMeta *cur;
	char **a;
	char *s;
	int i;

	a=BASM_UnmangleSplit(name+4);
	if(!a)return;
	if(!a[0]) { free(a); return; }

	cur=BASM_GetXMetaInfo(a[0]);

	if(cur->mtrg)
	{
		cur->mtrg(name, a[0], a+1, ptr);
	}else
	{
		s=basm_strdup(name);
		if(!cur->tmp)
		{
			cur->tmp=malloc(16*sizeof(char *));
			cur->mtmp=16;
			cur->ntmp=0;
		}

		for(i=0; i<cur->ntmp; i++)
			if(cur->tmp[i]==s)
				break;
		if(i>=cur->ntmp)
		{
			if((cur->ntmp+1)>=cur->mtmp)
			{
				i=cur->mtmp+(cur->mtmp>>1);
				cur->tmp=realloc(cur->tmp, i*sizeof(char *));
				cur->mtmp=i;
			}
			i=cur->ntmp++;
			cur->tmp[i]=s;
		}
	}

	for(i=0; a[i]; i++)free(a[i]);
	free(a);
}

BASM_API int BASM_RegisterLinkMeta(char *name, basm_meta_ft fcn)
{
	BASM_XMeta *cur;
	int i;

	cur=BASM_GetXMetaInfo(name);
	cur->mfcn=fcn;
	return(0);
}

BASM_API int BASM_RegisterLinkMetaTrigger(char *name, basm_mtrg_ft fcn)
{
	BASM_XMeta *cur;
	char **a;
	void *p;
	int i, j;

	cur=BASM_GetXMetaInfo(name);
	cur->mtrg=fcn;

	if(cur->tmp)
	{
		for(i=0; i<cur->ntmp; i++)
		{
			p=BASM_GetPtr(cur->tmp[i]);
			a=BASM_UnmangleSplit(cur->tmp[i]+4);
			fcn(cur->tmp[i], a[0], a+1, p);
			for(j=0; a[j]; j++)free(a[j]);
			free(a);
		}

		free(cur->tmp);
		cur->tmp=NULL;
	}

	return(0);
}

BASM_API char *BASM_ComposeLinkMetaName(char *name, char **args)
{
	char tb[256], tb1[256];
	char *s, *t;
	int i;

	t=tb;
	*t++='_'; *t++='X';
	*t++='M'; *t++='_';

	BASM_MangleXCall(name, tb1);
	strcpy(t, tb1); t+=strlen(t);

	i=0;
	while(args[i])
	{
		*t++='_'; *t++='_';
		BASM_MangleXCall(args[i], tb1);
		strcpy(t, tb1); t+=strlen(t);
	}

	*t++=0;

	return(basm_rstrdup(tb));
}

BASM_API char *BASM_ComposeLinkNotifyName(char *name, char **args)
{
	char tb[256], tb1[256];
	char *s, *t;
	int i;

	t=tb;
	*t++='_'; *t++='X';
	*t++='N'; *t++='_';

	BASM_MangleXCall(name, tb1);
	strcpy(t, tb1); t+=strlen(t);

	i=0;
	while(args[i])
	{
		*t++='_'; *t++='_';
		BASM_MangleXCall(args[i], tb1);
		strcpy(t, tb1); t+=strlen(t);
	}

	*t++=0;

	return(basm_rstrdup(tb));
}


void *basm_xmeta_lookup(char *name)
{
	void *p;

//	printf("basm_xmeta_lookup: '%s'\n", name);

	if(!strncmp(name, "_XM_", 4))
	{
		p=BASM_GetLabelXMeta(name);
		return(p);
	}

	if(!strncmp(name, "_XT_", 4))
	{
		p=BASM_GetLabelXTls(name);
		return(p);
	}

	if(!strncmp(name, "_XC_", 4))
	{
		p=BASM_GetLabelXCall(name);
		return(p);
	}

	if(!strncmp(name, "_XV_", 4))
	{
		p=BASM_GetLabelXCall(name);
		return(p);
	}

	if(	((name[0]>='a')&&(name[0]<='z'))||
		((name[0]>='A')&&(name[0]<='Z')))
	{
		p=BASM_GetLabelNativeXCall(name);
		if(p)return(p);
	}

	return(NULL);
}

void basm_xmeta_notify(char *name, void *ptr)
{
	if(!strncmp(name, "_XN_", 4))
		BASM_NotifyLabelXMeta(name, ptr);
}

int BASM_InitXMeta()
{
	static int init=0;

	if(init)return(0);
	init=1;

	BLNK_RegisterLookup(basm_xmeta_lookup);
	BLNK_RegisterNotify(basm_xmeta_notify);
	return(1);
}
