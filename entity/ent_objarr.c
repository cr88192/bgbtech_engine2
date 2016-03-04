BGBDT_EntArray *BGBDT_EntObj_NewArray(char *sig, int sz)
{
	BGBDT_EntArray *obj;
	int es;
	
	es=BGBDT_EntObj_GetSigSize(sig);
	
	obj=frgl_talloc("bgbdt_entarray_t", sizeof(BGBDT_EntArray));
	obj->sig=frgl_strdup(sig);
	obj->size=sz;
	obj->data=frgl_malloc(es*sz);
	return(obj);
}

char *BGBDT_EntObj_GetArraySig(BGBDT_EntArray *arr)
	{ return(arr->sig); }

int BGBDT_EntObj_GetArraySize(BGBDT_EntArray *arr)
	{ return(arr->size); }

int BGBDT_EntObj_GetArrayIndexI(BGBDT_EntArray *arr, int idx)
{
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
	return(BGBDT_EntObj_GetSigPtrValueI(arr->data+(sz*idx), arr->sig));
}

s64 BGBDT_EntObj_GetArrayIndexL(BGBDT_EntArray *arr, int idx)
{
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
	return(BGBDT_EntObj_GetSigPtrValueI(arr->data+(sz*idx), arr->sig));
}

double BGBDT_EntObj_GetArrayIndexF(BGBDT_EntArray *arr, int idx)
{
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
	return(BGBDT_EntObj_GetSigPtrValueF(arr->data+(sz*idx), arr->sig));
}

int BGBDT_EntObj_GetArrayIndexP(BGBDT_EntArray *arr, int idx)
{
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
	return(BGBDT_EntObj_GetSigPtrValueP(arr->data+(sz*idx), arr->sig));
}


void BGBDT_EntObj_SetArrayIndexI(BGBDT_EntArray *arr, int idx, int val)
{
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
	BGBDT_EntObj_SetSigPtrValueI(arr->data+(sz*idx), arr->sig, val);
}

void BGBDT_EntObj_SetArrayIndexL(BGBDT_EntArray *arr, int idx, s64 val)
{
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
	BGBDT_EntObj_SetSigPtrValueI(arr->data+(sz*idx), arr->sig, val);
}

void BGBDT_EntObj_SetArrayIndexF(BGBDT_EntArray *arr, int idx, double val)
{
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
	BGBDT_EntObj_SetSigPtrValueF(arr->data+(sz*idx), arr->sig, val);
}

void BGBDT_EntObj_SetArrayIndexP(BGBDT_EntArray *arr, int idx, void *val)
{
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
	BGBDT_EntObj_SetSigPtrValueP(arr->data+(sz*idx), arr->sig, val);
}

void BGBDT_EntObj_SetArrayIndexFxV(BGBDT_EntArray *arr,
	int idx, int *val, int nval)
{
	int *ti;
	int sz, ofs;
	sz=BGBDT_EntObj_GetSigSize(arr->sig);
//	BGBDT_EntObj_SetSigPtrValueP(arr->data+(sz*idx), arr->sig, val);
	ti=arr->data+(sz*idx);
	ti[0]=val[0];
	ti[1]=val[1];
	ti[2]=val[2];
}
