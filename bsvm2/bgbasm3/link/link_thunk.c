// #include <bgblink.h>
// #include <bgbasm.h>


BASM_API BLNK_Scope *BLNK_NewScope(BLNK_Scope *super)
{
	BLNK_Scope *tmp;

	tmp=BLNK_gcTAlloc("_blnk_scope_t", sizeof(BLNK_Scope));
	tmp->super=super;

	tmp->syms=BLNK_gcAlloc(16*sizeof(BLNK_ScopeSym *));
	tmp->n_syms=0;
	tmp->m_syms=16;

	return(tmp);
}

BASM_API void BLNK_FreeScope(BLNK_Scope *scope)
{
	BLNK_gcFree(scope->syms);
	BLNK_gcFree(scope);
}

BASM_API int BLNK_IndexScopeSym(BLNK_Scope *scope, char *name)
{
	int i;

	for(i=0; i<scope->n_syms; i++)
		if(!strcmp(scope->syms[i]->name, name))
			return(i);

	if((scope->n_syms+1)>=scope->m_syms)
	{
		i=scope->m_syms+(scope->m_syms>>1);
		scope->syms=BLNK_gcReAlloc(scope->syms,
			i*sizeof(BLNK_ScopeSym *));
		scope->m_syms=i;
	}

	i=scope->n_syms++;
	if(!scope->syms[i])
		scope->syms[i]=BLNK_gcTAlloc("_blnk_scopesym_t",
			sizeof(BLNK_ScopeSym));
	scope->syms[i]->name=blnk_strdup(name);
	scope->syms[i]->addr=NULL;
	scope->syms[i]->sig=NULL;

	return(i);
}

BASM_API void BLNK_BindScopeSym(BLNK_Scope *scope, char *name, void *ptr)
{
	int i;

	i=BLNK_IndexScopeSym(scope, name);
	scope->syms[i]->addr=ptr;
}

BASM_API void BLNK_BindScopeSymSig(BLNK_Scope *scope,
	char *name, char *sig, void *ptr)
{
	int i;

	i=BLNK_IndexScopeSym(scope, name);
	scope->syms[i]->addr=ptr;
	scope->syms[i]->sig=blnk_strdup(sig);
}

BASM_API void BLNK_AssignScopeSym(BLNK_Scope *scope, char *name, void *ptr)
{
	int i;

	for(i=0; i<scope->n_syms; i++)
		if(!strcmp(scope->syms[i]->name, name))
	{
		scope->syms[i]->addr=ptr;
		return;
	}

	if(scope->super)
	{
		BLNK_AssignScopeSym(scope, name, ptr);
		return;
	}
}

BASM_API void *BLNK_FetchScopeSym(BLNK_Scope *scope, char *name)
{
	int i;

	if(!strncmp(name, "G.", 2))
	{
		for(i=0; i<scope->n_syms; i++)
			if(!strcmp(scope->syms[i]->name, name+2))
				return((void *)(&(scope->syms[i]->addr)));
	}else
	{
		for(i=0; i<scope->n_syms; i++)
			if(!strcmp(scope->syms[i]->name, name))
				return(scope->syms[i]->addr);
	}

	if(scope->super)
		return(BLNK_FetchScopeSym(scope, name));

	return(BLNK_FetchSym(name));
}

BASM_API void *BLNK_FetchScopeSym2(BLNK_Scope *scope, char *name)
{
	int i;

	if(!strncmp(name, "G.", 2))
	{
		for(i=0; i<scope->n_syms; i++)
			if(!strcmp(scope->syms[i]->name, name+2))
				return((void *)(&(scope->syms[i]->addr)));
	}else
	{
		for(i=0; i<scope->n_syms; i++)
			if(!strcmp(scope->syms[i]->name, name))
				return(scope->syms[i]->addr);
	}

	if(scope->super)
		return(BLNK_FetchScopeSym2(scope, name));
	return(NULL);
}

BASM_API char *BLNK_FetchScopeSig(BLNK_Scope *scope, char *name)
{
	int i;

	if(!strncmp(name, "G.", 2))
	{
		for(i=0; i<scope->n_syms; i++)
			if(!strcmp(scope->syms[i]->name, name+2))
				return(scope->syms[i]->sig);
	}else
	{
		for(i=0; i<scope->n_syms; i++)
			if(!strcmp(scope->syms[i]->name, name))
				return(scope->syms[i]->sig);
	}

	if(scope->super)
		return(BLNK_FetchScopeSig(scope, name));
	return(NULL);
}

BASM_API BLNK_Thunk *BLNK_LinkModuleThunk(BLNK_Object *ctx, BLNK_Scope *scope)
{
	BLNK_Thunk *obj;
	byte *text, *data, *ptr, *ptr1;
	char *s, *t;
#ifdef __x86_64__
	unsigned long long uli;
#endif
	int tsz, dsz;
	int i, j, k;

	if(!scope)scope=BLNK_NewScope(NULL);
	obj=BLNK_gcTExec("_blnk_thunk_t", sizeof(BLNK_Thunk));
	obj->scope=scope;

//	tsz=ctx->ip-ctx->text;
//	dsz=ctx->dp-ctx->data;

	tsz=ctx->text_sz;
	dsz=ctx->data_sz;

	text=BLNK_gcExec(tsz);
	data=BLNK_gcAlloc(dsz);

	memcpy(text, ctx->text, tsz);
	memcpy(data, ctx->data, dsz);

	obj->text=text;
	obj->data=data;

#if 0
	for(i=0; i<ctx->n_proxy; i++)
		BLNK_AddProxy(ctx->proxy_name[i]);

	for(i=0; i<ctx->n_labels; i++)
	{
		s=ctx->label_name[i];
		t="__proxy_";
		if(!strncmp(s, t, strlen(t)))
		{
			BLNK_AddProxy(s+strlen(t));
		}
	}
#endif

#if 1
	for(i=0; i<ctx->n_labels; i++)
	{
		j=ctx->label_pos[i]&0xFFFFFF;
		k=ctx->label_pos[i]>>24;
		ptr=NULL;
		if(k==1)
		{
			ptr=text+j;
			BLNK_BindScopeSym(scope, ctx->label_name[i], ptr);
		}

		if(k==2)
		{
			ptr=(byte *)BLNK_FetchScopeSym(scope,
				ctx->label_name[i]);
			if(ptr)continue;
			ptr=data+j;
			BLNK_BindScopeSym(scope, ctx->label_name[i], ptr);
		}

//		printf("Bind %s %p (%d %X)\n", ctx->label_name[i], ptr, k, j);
	}
#endif

	for(i=0; i<ctx->n_gotos; i++)
	{
		j=ctx->goto_pos[i]&0xFFFFFF;
		k=ctx->goto_pos[i]>>24;
		ptr=NULL;
		if(k==1)ptr=text+j;
		if(k==2)ptr=data+j;
//		BLNK_AddSymRef(ctx->goto_name[i], ptr, ctx->goto_type[i]);

		ptr1=(byte *)BLNK_FetchScopeSym(scope, ctx->goto_name[i]);
		BLNK_LinkReference(ptr, ptr1, ctx->goto_type[i]);
	}

#if 0
	//for loaded object files:
	//grab proxy value from loaded section
	//modify existing proxy
	for(i=0; i<ctx->n_labels; i++)
	{
		s=ctx->label_name[i];
		t="__proxy_";
		if(!strncmp(s, t, strlen(t)))
		{
			s+=strlen(t);
			BLNK_AddProxy(s);

			j=ctx->label_pos[i]&0xFFFFFF;
			k=ctx->label_pos[i]>>24;
			if(k==1)ptr=text+j;
			if(k==2)ptr=data+j;
			BLNK_AssignSym(s, ptr);
		}
	}
#endif

#ifdef __i386__
//	ptr=(byte *)BLNK_FetchScopeSym2(scope, "thunk_start");
//	if(!ptr)ptr=text;

//	ptr1=obj->jump+5; i=ptr-ptr1;
//	obj->jump[0]=0xE9;
//	obj->jump[1]=(i>> 0)&0xFF;
//	obj->jump[2]=(i>> 8)&0xFF;
//	obj->jump[3]=(i>>16)&0xFF;
//	obj->jump[4]=(i>>24)&0xFF;

	ptr=(byte *)BLNK_FetchScopeSym2(scope, "G.thunk_start");
	if(!ptr)ptr=(byte *)(&(obj->text));

	i=(int)ptr;
	obj->jump[0]=0xFF;	//jmp FF/4
	obj->jump[1]=0x25;	//[disp32]
	obj->jump[2]=(i>> 0)&0xFF;
	obj->jump[3]=(i>> 8)&0xFF;
	obj->jump[4]=(i>>16)&0xFF;
	obj->jump[5]=(i>>24)&0xFF;
#endif

#ifdef __x86_64__
//	ptr=(byte *)BLNK_FetchScopeSym2(scope, "thunk_start");
//	if(!ptr)ptr=text;

//	ptr1=obj->jump+5; i=ptr-ptr1;
//	obj->jump[0]=0xE9;
//	obj->jump[1]=(i>> 0)&0xFF;
//	obj->jump[2]=(i>> 8)&0xFF;
//	obj->jump[3]=(i>>16)&0xFF;
//	obj->jump[4]=(i>>24)&0xFF;

	ptr=(byte *)BLNK_FetchScopeSym2(scope, "G.thunk_start");
	if(!ptr)ptr=(byte *)(&(obj->text));

	uli=(unsigned long long)ptr;
	obj->jump[0]=0x48;	//REX.W
	obj->jump[1]=0xB8;	//mov rax, imm64
	obj->jump[2]=(uli>> 0)&0xFF;
	obj->jump[3]=(uli>> 8)&0xFF;
	obj->jump[4]=(uli>>16)&0xFF;
	obj->jump[5]=(uli>>24)&0xFF;
	obj->jump[6]=(uli>>32)&0xFF;
	obj->jump[7]=(uli>>40)&0xFF;
	obj->jump[8]=(uli>>48)&0xFF;
	obj->jump[9]=(uli>>56)&0xFF;

	obj->jump[10]=0x48;	//REX.W
	obj->jump[11]=0xFF;	//jmp FF/4
	obj->jump[12]=0xE0;	//rax
#endif

	return(obj);
}

BASM_API byte *BLNK_LinkThunkInline(BLNK_Object *ctx, BLNK_Scope *scope)
{
	static BLNK_Scope *tscope=NULL;
	byte *text, *data, *ptr, *ptr1;
	char *s, *t;
	int tsz, dsz;
	int i, j, k;

	if(!scope)
	{
		if(!tscope)tscope=BLNK_NewScope(NULL);
		tscope->n_syms=0;
		scope=tscope;		
	}

//	tsz=ctx->ip-ctx->text;
//	dsz=ctx->dp-ctx->data;

	tsz=ctx->text_sz;
	dsz=ctx->data_sz;

	if(dsz)
	{
		i=(tsz+15)&(~15);
		text=BLNK_gcExec(i+dsz);
		data=text+i;
	}else
	{
		text=BLNK_gcExec(tsz);
		data=text+tsz;
	}

	memcpy(text, ctx->text, tsz);
	memcpy(data, ctx->data, dsz);

	for(i=0; i<ctx->n_labels; i++)
	{
		j=ctx->label_pos[i]&0xFFFFFF;
		k=ctx->label_pos[i]>>24;
		ptr=NULL;
		if(k==1)
		{
			ptr=text+j;
			BLNK_BindScopeSym(scope, ctx->label_name[i], ptr);
		}

		if(k==2)
		{
			ptr=(byte *)BLNK_FetchScopeSym(scope,
				ctx->label_name[i]);
			if(ptr)continue;
			ptr=data+j;
			BLNK_BindScopeSym(scope, ctx->label_name[i], ptr);
		}
	}

	for(i=0; i<ctx->n_gotos; i++)
	{
		j=ctx->goto_pos[i]&0xFFFFFF;
		k=ctx->goto_pos[i]>>24;
		ptr=NULL;
		if(k==1)ptr=text+j;
		if(k==2)ptr=data+j;

		ptr1=(byte *)BLNK_FetchScopeSym(scope, ctx->goto_name[i]);
		BLNK_LinkReference(ptr, ptr1, ctx->goto_type[i]);
	}

	return(text);
}

BASM_API BLNK_Thunk *BLNK_LinkModuleThunkObj(BLNK_Scope *scope, void *buf, int sz)
{
	BLNK_Object *ctx;
	BLNK_Thunk *thnk;

	ctx=BLNK_COFF_LoadObjectBuf(NULL, buf, sz);
	thnk=BLNK_LinkModuleThunk(ctx, scope);
	BLNK_DestroyContext(ctx);
	return(thnk);
}

BASM_API byte *BLNK_LinkModuleThunkInlineObj(BLNK_Scope *scope, void *buf, int sz)
{
	BLNK_Object *ctx;
	byte *p;

	ctx=BLNK_COFF_LoadObjectBuf(NULL, buf, sz);
	p=BLNK_LinkThunkInline(ctx, scope);
	BLNK_DestroyContext(ctx);
	return(p);
}
