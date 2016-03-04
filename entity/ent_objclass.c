#include <bteifgl.h>

BGBDT_EntField *bgbdt_entobj_fields[4096];
int bgbdt_entobj_nfields;

BGBDT_EntClass *bgbdt_entobj_class[1024];
int bgbdt_entobj_nclass;

int BGBDT_EntObj_GetSigSize(char *sig)
{
	int sz;

	switch(*sig)
	{
	case 'a': sz=sizeof(char); break;
	case 'b': sz=sizeof(char); break;
	case 'c': sz=sizeof(char); break;
	case 'd': sz=sizeof(double); break;
	case 'e': sz=sizeof(double); break;
	case 'f': sz=sizeof(float); break;

	case 'h': sz=sizeof(char); break;
	case 'i': sz=sizeof(int); break;
	case 'j': sz=sizeof(int); break;
	case 'k': sz=sizeof(short); break;
	case 'l': sz=sizeof(long); break;
	case 'm': sz=sizeof(long); break;
	case 'n': sz=sizeof(long long); break;
	case 'o': sz=sizeof(long long); break;

	case 's': sz=sizeof(short); break;
	case 't': sz=sizeof(short); break;

	case 'v': sz=sizeof(char); break;
	case 'w': sz=sizeof(short); break;
	case 'x': sz=sizeof(long long); break;
	case 'y': sz=sizeof(long long); break;
	case 'z': sz=sizeof(void *); break;

	case 'C':
		switch(sig[1])
		{
		case 'a': sz=2*sizeof(int); break;
		case 'b': sz=3*sizeof(int); break;
		case 'c': sz=4*sizeof(int); break;
		default: break;
		}
		break;

	case 'P': sz=sizeof(void *); break;

	default: sz=sizeof(void *); break;
	}
	
	return(sz);
}

int BGBDT_EntObj_GetSigAlign(char *sig)
{
	int sz;

	switch(*sig)
	{
	case 'a': sz=sizeof(char); break;
	case 'b': sz=sizeof(char); break;
	case 'c': sz=sizeof(char); break;
	case 'd': sz=sizeof(double); break;
	case 'e': sz=sizeof(double); break;
	case 'f': sz=sizeof(float); break;

	case 'h': sz=sizeof(char); break;
	case 'i': sz=sizeof(int); break;
	case 'j': sz=sizeof(int); break;
	case 'k': sz=sizeof(short); break;
	case 'l': sz=sizeof(long); break;
	case 'm': sz=sizeof(long); break;
	case 'n': sz=sizeof(long long); break;
	case 'o': sz=sizeof(long long); break;

	case 's': sz=sizeof(short); break;
	case 't': sz=sizeof(short); break;

	case 'v': sz=sizeof(char); break;
	case 'w': sz=sizeof(short); break;
	case 'x': sz=sizeof(long long); break;
	case 'y': sz=sizeof(long long); break;
	case 'z': sz=sizeof(void *); break;

	case 'C':
		switch(sig[1])
		{
		case 'a': sz=sizeof(int); break;
		case 'b': sz=sizeof(int); break;
		case 'c': sz=sizeof(int); break;
		default: break;
		}
		break;

	case 'P': sz=sizeof(void *); break;

	default: sz=sizeof(void *); break;
	}
	
	return(sz);
}

s64 BGBDT_EntObj_GetSigPtrValueI(void *ptr, char *sig)
{
	s64 v;

	switch(*sig)
	{
	case 'a': v=*(char *)ptr; break;
	case 'b': v=*(char *)ptr; break;
	case 'c': v=*(char *)ptr; break;
	case 'd': v=*(double *)ptr; break;
	case 'e': v=*(double *)ptr; break;
	case 'f': v=*(float *)ptr; break;

	case 'h': v=*(unsigned char *)ptr; break;
	case 'i': v=*(int *)ptr; break;
	case 'j': v=*(unsigned int *)ptr; break;
	case 'k': v=*(short *)ptr; break;
	case 'l': v=*(long *)ptr; break;
	case 'm': v=*(unsigned long *)ptr; break;
	case 'n': v=*(long long *)ptr; break;
	case 'o': v=*(unsigned long long *)ptr; break;

	case 's': v=*(short *)ptr; break;
	case 't': v=*(unsigned short *)ptr; break;

	case 'v': v=*(char *)ptr; break;
	case 'w': v=*(unsigned short *)ptr; break;
	case 'x': v=*(long long *)ptr; break;
	case 'y': v=*(unsigned long long *)ptr; break;
	case 'z': v=(nlint)(*(void **)ptr); break;

	case 'P': v=(nlint)(*(void **)ptr); break;
	default: v=(nlint)(*(void **)ptr); break;
	}
	
	return(v);
}

double BGBDT_EntObj_GetSigPtrValueF(void *ptr, char *sig)
{
	double v;

	switch(*sig)
	{
	case 'a': v=*(char *)ptr; break;
	case 'b': v=*(char *)ptr; break;
	case 'c': v=*(char *)ptr; break;
	case 'd': v=*(double *)ptr; break;
	case 'e': v=*(double *)ptr; break;
	case 'f': v=*(float *)ptr; break;

	case 'h': v=*(unsigned char *)ptr; break;
	case 'i': v=*(int *)ptr; break;
	case 'j': v=*(unsigned int *)ptr; break;
	case 'k': v=*(short *)ptr; break;
	case 'l': v=*(long *)ptr; break;
	case 'm': v=*(unsigned long *)ptr; break;
	case 'n': v=*(long long *)ptr; break;
	case 'o': v=*(unsigned long long *)ptr; break;

	case 's': v=*(short *)ptr; break;
	case 't': v=*(unsigned short *)ptr; break;

	case 'v': v=*(char *)ptr; break;
	case 'w': v=*(unsigned short *)ptr; break;
	case 'x': v=*(long long *)ptr; break;
	case 'y': v=*(unsigned long long *)ptr; break;
	case 'z': v=(nlint)(*(void **)ptr); break;

	case 'P': v=(nlint)(*(void **)ptr); break;
	default: v=(nlint)(*(void **)ptr); break;
	}
	
	return(v);
}

void *BGBDT_EntObj_GetSigPtrValueP(void *ptr, char *sig)
{
	void *v;

	switch(*sig)
	{
	case 'a':	case 'b':	case 'c':	case 'd':
	case 'e':	case 'f':	case 'h':	case 'k':
	case 's':	case 't':	case 'v':	case 'w':
		v=NULL; break;

	case 'i': v=(void *)(*(int *)ptr); break;
	case 'j': v=(void *)(*(unsigned int *)ptr); break;
	case 'l': v=(void *)(*(long *)ptr); break;
	case 'm': v=(void *)(*(unsigned long *)ptr); break;

	case 'n':	case 'o':	case 'x':	case 'y':
		v=(void *)(*(long long *)ptr); break;

	case 'z': v=*(void **)ptr; break;

	case 'P': v=*(void **)ptr; break;
	default: v=*(void **)ptr; break;
	}
	
	return(v);
}

void BGBDT_EntObj_SetSigPtrValueI(void *ptr, char *sig, s64 val)
{
	s64 v;

	switch(*sig)
	{
	case 'a': *(char *)ptr=val; break;
	case 'b': *(char *)ptr=val; break;
	case 'c': *(char *)ptr=val; break;
	case 'd': *(double *)ptr=val; break;
	case 'e': *(double *)ptr=val; break;
	case 'f': *(float *)ptr=val; break;

	case 'h': *(unsigned char *)ptr=val; break;
	case 'i': *(int *)ptr=val; break;
	case 'j': *(unsigned int *)ptr=val; break;
	case 'k': *(short *)ptr=val; break;
	case 'l': *(long *)ptr=val; break;
	case 'm': *(unsigned long *)ptr=val; break;
	case 'n': *(long long *)ptr=val; break;
	case 'o': *(unsigned long long *)ptr=val; break;

	case 's': *(short *)ptr=val; break;
	case 't': *(unsigned short *)ptr=val; break;

	case 'v': *(char *)ptr=val; break;
	case 'w': *(unsigned short *)ptr=val; break;
	case 'x': *(long long *)ptr=val; break;
	case 'y': *(unsigned long long *)ptr=val; break;
	case 'z': *(void **)ptr=(void *)val; break;

	case 'P': *(void **)ptr=(void *)val; break;
	default: *(void **)ptr=(void *)val; break;
	}
}

void BGBDT_EntObj_SetSigPtrValueF(void *ptr, char *sig, double val)
{
	s64 v;

	switch(*sig)
	{
	case 'a': *(char *)ptr=val; break;
	case 'b': *(char *)ptr=val; break;
	case 'c': *(char *)ptr=val; break;
	case 'd': *(double *)ptr=val; break;
	case 'e': *(double *)ptr=val; break;
	case 'f': *(float *)ptr=val; break;

	case 'h': *(unsigned char *)ptr=val; break;
	case 'i': *(int *)ptr=val; break;
	case 'j': *(unsigned int *)ptr=val; break;
	case 'k': *(short *)ptr=val; break;
	case 'l': *(long *)ptr=val; break;
	case 'm': *(unsigned long *)ptr=val; break;
	case 'n': *(long long *)ptr=val; break;
	case 'o': *(unsigned long long *)ptr=val; break;

	case 's': *(short *)ptr=val; break;
	case 't': *(unsigned short *)ptr=val; break;

	case 'v': *(char *)ptr=val; break;
	case 'w': *(unsigned short *)ptr=val; break;
	case 'x': *(long long *)ptr=val; break;
	case 'y': *(unsigned long long *)ptr=val; break;
	case 'z': *(void **)ptr=(void *)val; break;

	case 'P': *(void **)ptr=(void *)val; break;
	default: *(void **)ptr=(void *)val; break;
	}
}

void BGBDT_EntObj_SetSigPtrValueP(void *ptr, char *sig, void *val)
{
	switch(*sig)
	{
	case 'a':	case 'b':	case 'c':	case 'd':
	case 'e':	case 'f':	case 'h':	case 'k':
	case 's':	case 't':	case 'v':	case 'w':
		break;

	case 'i': (*(int *)ptr)=(int)val; break;
	case 'j': (*(unsigned int *)ptr)=(int)val; break;
	case 'l': (*(long *)ptr)=(int)val; break;
	case 'm': (*(unsigned long *)ptr)=(int)val; break;

	case 'n':	case 'o':	case 'x':	case 'y':
		(*(long long *)ptr)=(int)val; break;

	case 'z': *(void **)ptr=val; break;

	case 'P': *(void **)ptr=val; break;
	default: *(void **)ptr=val; break;
	}
}


BGBDT_EntField *BGBDT_EntObj_LookupField(
	char *name, char *sig)
{
	BGBDT_EntField *fcur;
	int i;
	
	for(i=0; i<bgbdt_entobj_nfields; i++)
	{
		fcur=bgbdt_entobj_fields[i];
		if(!strcmp(fcur->name, name) && !strcmp(fcur->sig, sig))
			return(fcur);
	}
	return(NULL);
}

BGBDT_EntField *BGBDT_EntObj_GetField(
	char *name, char *sig)
{
	BGBDT_EntField *fcur;
	int i;
	
	fcur=BGBDT_EntObj_LookupField(name, sig);
	if(fcur)
		return(fcur);
	
	i=bgbdt_entobj_nfields++;
	fcur=frgl_malloc(sizeof(BGBDT_EntField));
	fcur->name=frgl_strdup(name);
	fcur->sig=frgl_strdup(sig);
	bgbdt_entobj_fields[i]=fcur;
	
	return(fcur);
}

int BGBDT_EntObj_LookupClassField(
	BGBDT_EntClass *clinf, BGBDT_EntField *finf)
{
	int i;
	for(i=0; i<clinf->n_field; i++)
	{
		if(clinf->field[i]==finf)
			return(i);
	}
	return(-1);
}

int BGBDT_EntObj_CheckClassHasFields(
	BGBDT_EntClass *clinf, BGBDT_EntField **finf, int nfinf)
{
	int i, j;
	
	for(i=0; i<nfinf; i++)
	{
		j=BGBDT_EntObj_LookupClassField(clinf, finf[i]);
		if(j<0)break;
	}
	if(i<nfinf)
		return(0);
	return(1);
}

int BGBDT_EntObj_CheckClassMatchFields(
	BGBDT_EntClass *clinf, BGBDT_EntField **finf, int nfinf)
{
	if(clinf->n_field!=nfinf)
		return(0);
	return(BGBDT_EntObj_CheckClassHasFields(
		clinf, finf, nfinf));
}

BGBDT_EntClass *BGBDT_EntObj_LookupClass(BGBDT_EntField **finf, int nfinf)
{
	BGBDT_EntClass *clinf;
	int i, j, k;
	
	for(i=0; i<bgbdt_entobj_nclass; i++)
	{
		clinf=bgbdt_entobj_class[i];
		if(BGBDT_EntObj_CheckClassMatchFields(clinf, finf, nfinf)>0)
			return(clinf);
	}
	return(NULL);
}

BGBDT_EntClass *BGBDT_EntObj_GetClass(BGBDT_EntField **finf, int nfinf)
{
	BGBDT_EntClass *clinf;
	int i, j, k;
	
	clinf=BGBDT_EntObj_LookupClass(finf, nfinf);
	if(clinf)
		return(clinf);
	
	clinf=frgl_malloc(sizeof(BGBDT_EntClass) +
		nfinf*sizeof(BGBDT_EntField *) +
		nfinf*sizeof(int));
	clinf->field=(void *)(clinf+1);
	clinf->field_ofs=(void *)(clinf->field+nfinf);
	clinf->n_field=nfinf;
	
	k=0;
	for(i=0; i<nfinf; i++)
	{
		j=BGBDT_EntObj_GetSigAlign(finf->sig);
		k=(k+(j-1))&(~(j-1));
	
		clinf->field[i]=finf[i];
		clinf->field_ofs[i]=k;

		j=BGBDT_EntObj_GetSigSize(finf->sig);
		k+=j;
	}
	clinf->sz_data=k;
	
	i=bgbdt_entobj_nclass++;
	bgbdt_entobj_class[i]=clinf;
	return(clinf);
}

BGBDT_EntObj *BGBDT_EntObj_NewInstance(BGBDT_EntClass *clinf)
{
	BGBDT_EntObj *obj;
	
	obj=frgl_talloc("bgbdt_entobj_t", sizeof(BGBDT_EntObj));
	obj->clinf=clinf;
	obj->data=frgl_malloc(clinf->sz_data);
	return(obj);
}

int BGBDT_EntObj_LookupClassFieldName(
	BGBDT_EntClass *clinf, char *name)
{
	int i;
	for(i=0; i<clinf->n_field; i++)
	{
		if(!strcmp(clinf->field[i]->name, name))
			return(i);
	}
	return(-1);
}

void *BGBDT_EntObj_LookupObjFieldPtrName(
	BGBDT_EntObj *obj, char *name)
{
	int idx;
	idx=BGBDT_EntObj_LookupClassFieldName(obj->clinf, name);
	if(idx<0)return(NULL);
	return(obj->data+obj->clinf->field_ofs[idx]);
}

s64 BGBDT_EntObj_GetObjFieldNameI(
	BGBDT_EntObj *obj, char *name)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassFieldName(obj->clinf, name);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	return(BGBDT_EntObj_GetSigPtrValueI(ptr,
		obj->clinf->field[idx]->sig));
}

double BGBDT_EntObj_GetObjFieldNameF(
	BGBDT_EntObj *obj, char *name)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassFieldName(obj->clinf, name);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	return(BGBDT_EntObj_GetSigPtrValueF(ptr,
		obj->clinf->field[idx]->sig));
}

void BGBDT_EntObj_SetObjFieldNameI(
	BGBDT_EntObj *obj, char *name, s64 val)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassFieldName(obj->clinf, name);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	BGBDT_EntObj_SetSigPtrValueI(ptr,
		obj->clinf->field[idx]->sig, val);
}

void BGBDT_EntObj_SetObjFieldNameF(
	BGBDT_EntObj *obj, char *name, double val)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassFieldName(obj->clinf, name);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	BGBDT_EntObj_SetSigPtrValueF(ptr,
		obj->clinf->field[idx]->sig, val);
}


int BGBDT_EntObj_GetObjFieldI(
	BGBDT_EntObj *obj, BGBDT_EntField *finf)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassField(obj->clinf, finf);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	return(BGBDT_EntObj_GetSigPtrValueI(ptr,
		obj->clinf->field[idx]->sig));
}

s64 BGBDT_EntObj_GetObjFieldL(
	BGBDT_EntObj *obj, BGBDT_EntField *finf)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassField(obj->clinf, finf);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	return(BGBDT_EntObj_GetSigPtrValueI(ptr,
		obj->clinf->field[idx]->sig));
}

double BGBDT_EntObj_GetObjFieldF(
	BGBDT_EntObj *obj, BGBDT_EntField *finf)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassField(obj->clinf, finf);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	return(BGBDT_EntObj_GetSigPtrValueF(ptr,
		obj->clinf->field[idx]->sig));
}

void *BGBDT_EntObj_GetObjFieldP(
	BGBDT_EntObj *obj, BGBDT_EntField *finf)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassField(obj->clinf, finf);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	return(BGBDT_EntObj_GetSigPtrValueP(ptr,
		obj->clinf->field[idx]->sig));
}

void BGBDT_EntObj_SetObjFieldI(
	BGBDT_EntObj *obj, BGBDT_EntField *finf, int val)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassField(obj->clinf, finf);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	BGBDT_EntObj_SetSigPtrValueI(ptr,
		obj->clinf->field[idx]->sig, val);
}

void BGBDT_EntObj_SetObjFieldL(
	BGBDT_EntObj *obj, BGBDT_EntField *finf, s64 val)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassField(obj->clinf, finf);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	BGBDT_EntObj_SetSigPtrValueI(ptr,
		obj->clinf->field[idx]->sig, val);
}

void BGBDT_EntObj_SetObjFieldF(
	BGBDT_EntObj *obj, BGBDT_EntField *finf, double val)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassField(obj->clinf, finf);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	BGBDT_EntObj_SetSigPtrValueF(ptr,
		obj->clinf->field[idx]->sig, val);
}

void BGBDT_EntObj_SetObjFieldP(
	BGBDT_EntObj *obj, BGBDT_EntField *finf, void *pval)
{
	int idx;
	void *ptr;
	idx=BGBDT_EntObj_LookupClassField(obj->clinf, finf);
	if(idx<0)return(NULL);
	ptr=obj->data+obj->clinf->field_ofs[idx];
	BGBDT_EntObj_SetSigPtrValueP(ptr,
		obj->clinf->field[idx]->sig, pval);
}
