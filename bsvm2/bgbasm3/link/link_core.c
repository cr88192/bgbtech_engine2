// #include <bgblink.h>
// #include <bgbasm.h>

#ifdef linux
#include <sys/mman.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifndef BASM_SHAVEMEM
#define BLNK_SZCHUNK	(1<<20)
#else
#define BLNK_SZCHUNK	(1<<16)
#endif

#ifdef WIN64
byte *blnk_bigspace=NULL;
byte *blnk_bigspace_mid=NULL;
byte *blnk_bigmap=NULL;
int blnk_bigmap_mid=0;
int blnk_bigrov=0;
#endif

byte *blnk_text[256];	//text section
byte *blnk_data[256];	//data section

int blnk_textpos[256];	//text position
int blnk_datapos[256];	//data position
int blnk_textsz[256];	//text size
int blnk_datasz[256];	//data size
int blnk_ntext;		//num text blocks
int blnk_ndata;		//num data blocks

char *blnk_exesec_n[256];
s64 blnk_exesec_va[256];
s64 blnk_exesec_sz[256];
int blnk_n_exesec;

//global symbol table

char **blnk_sym_name;	//declared symbol name
char **blnk_sym_name2;	//declared symbol name (mangled)
void **blnk_sym_addr;	//declared symbol address
int *blnk_sym_ref;	//declared index into reference table
int blnk_nsym;
int blnk_msym;

int blnk_sym_hash[4096];


//symbol reference table

byte **blnk_ref_addr;	//reference table addresses
int *blnk_ref_next;	//reference table links
short *blnk_ref_type;	//reference table type/flags
int blnk_nref;
int blnk_mref;


//strings table

char *blnk_strtab;	//strings table
char *blnk_strtabe;	//strings table end

char *blnk_strhash[4096];

//blacklist
char *blnk_strtab_bl;	//strings table (blacklist)
char *blnk_strtabe_bl;	//strings table end (blacklist)
char *blnk_strhash_bl[4096];


//modules
char *blnk_mod_name[1024];
byte *blnk_mod_text[1024];
byte *blnk_mod_data[1024];
int blnk_mod_textsz[1024];
int blnk_mod_datasz[1024];
int blnk_nmod;

//
void **bgblink_proxy=NULL;	//proxy pointers
int bgblink_nproxy;

blnk_lookup_ft blnk_lookup[256];
blnk_notify_ft blnk_notify[256];
int blnk_nlookup=0;
int blnk_nnotify=0;

BLNK_GCFuncs_t *blnk_gc;

BLNK_Object *blnk_queue;

//awkward crap for patching into the GC...

BASM_API BLNK_GCFuncs_t *BLNK_GetGCFuncs()
{
	if(!blnk_gc)
	{
		blnk_gc=malloc(sizeof(BLNK_GCFuncs_t));
		memset(blnk_gc, 0, sizeof(BLNK_GCFuncs_t));
	}
	return(blnk_gc);
}

BASM_API void BLNK_SetScanRange(int (*fcn)(void **p, int cnt))
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	gc->scanrange_f=fcn;
}

void BLNK_ScanRange(void **p, int cnt)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->scanrange_f)
		gc->scanrange_f(p, cnt);
}

void *BLNK_gcAlloc(int sz)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gcalloc_f)
		return(gc->gcalloc_f(sz));
	return(malloc(sz));
}

void *BLNK_gcExec(int sz)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gcexec_f)
		return(gc->gcexec_f(sz));
	return(BLNK_AllocTextSz(sz));
}

void *BLNK_gcAtomic(int sz)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gcatomic_f)
		return(gc->gcatomic_f(sz));
	return(malloc(sz));
}

void *BLNK_gcTAlloc(char *ty, int sz)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gctalloc_f)
		return(gc->gctalloc_f(ty, sz));
	return(malloc(sz));
}

void *BLNK_gcTExec(char *ty, int sz)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gctexec_f)
		return(gc->gctexec_f(ty, sz));
	return(BLNK_AllocTextSz(sz));
}

void *BLNK_gcTAtomic(char *ty, int sz)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gctatomic_f)
		return(gc->gctatomic_f(ty, sz));
	return(malloc(sz));
}

void BLNK_gcFree(void *obj)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gcfree_f)
		gc->gcfree_f(obj);

	if(BLNK_ObjAllocTextP((byte *)obj))return;
	free(obj);
}

void *BLNK_gcReAlloc(void *obj, int sz)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gcrealloc_f)
		return(gc->gcrealloc_f(obj, sz));
	return(realloc(obj, sz));
}

int BLNK_gcAllocTls(char *name, int sz)
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gcalloctls_f)
		return(gc->gcalloctls_f(name, sz));
	return(-1);
}

int BLNK_gcGetTlsTls()
{
	BLNK_GCFuncs_t *gc;

	gc=BLNK_GetGCFuncs();
	if(gc->gcgettlstls_f)
		return(gc->gcgettlstls_f());
	return(-1);
}

BASM_API int BLNK_RegisterLookup(blnk_lookup_ft fcn)
{
	int i;

	i=blnk_nlookup++;
	blnk_lookup[i]=fcn;
	return(i);
}

BASM_API int BLNK_RegisterNotify(blnk_notify_ft fcn)
{
	int i;

	i=blnk_nnotify++;
	blnk_notify[i]=fcn;
	return(i);
}


int BLNK_ObjAllocTextP(byte *obj)
{
	byte *p;
	int i, j;

	for(i=0; i<blnk_ntext; i++)
	{
		j=obj-blnk_text[i];
		if((j>=0) && (j<blnk_textsz[i]))
			return(1);
	}
	return(0);
}

void BLNK_AddExeSection(char *name, s64 va, s64 sz, int fl)
{
	int i;

	if(blnk_n_exesec>=256)
		return;

	i=blnk_n_exesec++;
	blnk_exesec_n[i]=blnk_strdup(name);
	blnk_exesec_va[i]=va;
	blnk_exesec_sz[i]=sz;
}

char *BLNK_LookupExeSectionPtrName(void *ptr)
{
	s64 va, vae;
	int i, j, k;
	
	for(i=0; i<blnk_n_exesec; i++)
	{
		va=blnk_exesec_va[i];
		vae=va+blnk_exesec_sz[i];
		if(((byte *)ptr)<((byte *)va))
			continue;
		if(((byte *)ptr)>=((byte *)vae))
			continue;
		return(blnk_exesec_n[i]);
	}
	return(NULL);
}

char *BLNK_LookupModulePtrName(void *ptr)
{
	s64 va, vae;
	int i, j, k;
	
	for(i=0; i<blnk_n_exesec; i++)
	{
		va=(s64)(blnk_mod_text[i]);
		vae=va+blnk_mod_textsz[i];
		if(	(((byte *)ptr)>=((byte *)va)) &&
			(((byte *)ptr)<((byte *)vae)))
		{
			return(blnk_mod_name[i]);
		}

		va=(s64)(blnk_mod_data[i]);
		vae=va+blnk_mod_datasz[i];
		if(	(((byte *)ptr)>=((byte *)va)) &&
			(((byte *)ptr)<((byte *)vae)))
		{
			return(blnk_mod_name[i]);
		}
	}
	return(NULL);
}

BASM_API void BLNK_MarkData()
{
	int i, j;

	for(i=0; i<blnk_n_exesec; i++)
	{
		j=blnk_exesec_sz[i]/sizeof(void *);
		BLNK_ScanRange((void **)(blnk_exesec_va[i]), j);
	}

	for(i=0; i<blnk_nmod; i++)
	{
		j=blnk_mod_datasz[i]/sizeof(void *);
		if(!j)continue;
		BLNK_ScanRange((void **)(blnk_mod_data[i]), j);
	}
}

BASM_API int BLNK_CheckPointerImageP(void *p)
{
	byte *sp, *ep, *cp;
	int i, j;

	cp=p;

	for(i=0; i<blnk_n_exesec; i++)
	{
		sp=(byte *)(blnk_exesec_va[i]);
		ep=sp+blnk_exesec_sz[i];
		if((cp>=sp) && (cp<ep))
			return(1);
	}

	for(i=0; i<blnk_ntext; i++)
	{
		sp=(byte *)(blnk_text[i]);
		ep=sp+blnk_textsz[i];
		if((cp>=sp) && (cp<ep))
			return(1);
	}

	for(i=0; i<blnk_ndata; i++)
	{
		sp=(byte *)(blnk_data[i]);
		ep=sp+blnk_datasz[i];
		if((cp>=sp) && (cp<ep))
			return(1);
	}

	return(0);
}


byte *BLNK_AllocChunkBuffer(int sz)
{
	byte *p;
	int i, j, k;

#ifdef WIN64
	if(!blnk_bigspace)
	{
		p=(void *)(0xB00000000LL);
		blnk_bigspace=(byte *)VirtualAlloc(p, 1LL<<32,
			MEM_RESERVE|MEM_TOP_DOWN,
			PAGE_EXECUTE_READWRITE);

		if(!blnk_bigspace)
		{
			blnk_bigspace=(byte *)VirtualAlloc(NULL, 1LL<<32,
				MEM_RESERVE|MEM_TOP_DOWN,
				PAGE_EXECUTE_READWRITE);
		}

		blnk_bigrov=0;

		blnk_bigmap=malloc(8192);
		memset(blnk_bigmap, 0, 8192);

		blnk_bigspace_mid=blnk_bigspace+(1LL<<31);
		blnk_bigmap_mid=32768;

		printf("BLNK_AllocChunkBuffer: Space at %p .. %p\n",
			blnk_bigspace, blnk_bigspace+(1LL<<32));
	}

#if 1
	while(1)
	{
		i=blnk_bigrov++;
		i=(i&1)?(-((i>>1)+1)):(i>>1);
		i+=blnk_bigmap_mid;

		if(blnk_bigmap[i>>3]&(1<<(i&7)))
			continue;

		k=(sz+65535)>>16;
		for(j=0; j<k; j++)
			if(blnk_bigmap[(i+j)>>3]&(1<<((i+j)&7)))
				break;
		if(j<k)continue;

		for(j=0; j<k; j++)
			blnk_bigmap[(i+j)>>3]|=1<<((i+j)&7);

		p=blnk_bigspace_mid+(i-blnk_bigmap_mid)*65536LL;

		p=(byte *)VirtualAlloc(p, sz,
			MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		printf("BLNK_AllocChunkBuffer: Chunk at %p\n", p);

		return(p);
		break;
	}
#endif

#endif

#ifdef linux
#ifdef X86_64
	/* (void *)(s64)i */
	p=(byte *)mmap(NULL, sz,
		PROT_READ|PROT_WRITE|PROT_EXEC,
		MAP_PRIVATE|MAP_ANONYMOUS|MAP_32BIT, -1, 0);
#else
	p=(byte *)mmap(NULL, sz,
		PROT_READ|PROT_WRITE|PROT_EXEC,
		MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
#endif

#else
#ifdef WIN32
	p=(byte *)VirtualAlloc(NULL, sz,
		MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else
	p=(byte *)malloc(sz);
#endif
#endif

	return(p);
}

void BLNK_AllocTextChunk(int sz)
{
	int i;

	i=blnk_ntext++;
	blnk_textpos[i]=0;
	blnk_textsz[i]=sz;
	blnk_text[i]=BLNK_AllocChunkBuffer(sz);

	memset(blnk_text[i], 0xCC, sz);
}

void BLNK_AllocDataChunk(int sz)
{
	int i;

	i=blnk_ndata++;
	blnk_datapos[i]=0;
	blnk_datasz[i]=sz;
	blnk_data[i]=BLNK_AllocChunkBuffer(sz);

	memset(blnk_data[i], 0x00, sz);
}

byte *BLNK_AllocTextSz(int sz)
{
	byte *p;
	int i, j;

	for(i=0; i<blnk_ntext; i++)
	{
		j=blnk_textsz[i]-blnk_textpos[i];
		if(j>=sz)
		{
			p=blnk_text[i]+blnk_textpos[i];

			j=blnk_textpos[i]+sz;
			j=(j+15)&(~15);
			blnk_textpos[i]=j;

			return(p);
		}
	}

	if(sz>=BLNK_SZCHUNK)
	{
		j=BLNK_SZCHUNK;
		while(j<=sz)j+=j>>1;

		BLNK_AllocTextChunk(j);
	}else BLNK_AllocTextChunk(BLNK_SZCHUNK);

	p=BLNK_AllocTextSz(sz);
	return(p);
}

byte *BLNK_AllocDataSz(int sz)
{
	byte *p;
	int i, j;

	for(i=0; i<blnk_ndata; i++)
	{
		j=blnk_datasz[i]-blnk_datapos[i];
		if(j>=sz)
		{
			p=blnk_data[i]+blnk_datapos[i];

			j=blnk_datapos[i]+sz;
			j=(j+15)&(~15);
			blnk_datapos[i]=j;

			return(p);
		}
	}

	if(sz>=BLNK_SZCHUNK)
	{
		j=BLNK_SZCHUNK;
		while(j<=sz)j+=j>>1;

		BLNK_AllocDataChunk(j);
	}else BLNK_AllocDataChunk(BLNK_SZCHUNK);

	p=BLNK_AllocDataSz(sz);
	return(p);
}

void BLNK_InitLink()
{
	static int init=0;
	int i, j;

	if(init)return;
	init=1;

//	BLNK_InitRand();

	i=4096;
	blnk_sym_name=(char **)malloc(i*sizeof(char *));
	blnk_sym_name2=(char **)malloc(i*sizeof(char *));
	blnk_sym_addr=(void **)malloc(i*sizeof(void *));
	blnk_sym_ref=(int *)malloc(i*sizeof(int));
	blnk_nsym=0;	blnk_msym=i;

	blnk_ref_addr=(byte **)malloc(i*sizeof(byte *));
	blnk_ref_next=(int *)malloc(i*sizeof(int));
	blnk_ref_type=(short *)malloc(i*sizeof(short));
	blnk_nref=0;	blnk_mref=i;

	blnk_nmod=0;

	BLNK_AllocTextChunk(BLNK_SZCHUNK);
	BLNK_AllocDataChunk(BLNK_SZCHUNK);
}

#if 1
BASM_API int basm_strdup_i(char *str);	//AH:ignore
BASM_API char *basm_strtab_i(int i);		//AH:ignore
BASM_API char *basm_strdup(char *str);		//AH:ignore

char *blnk_strdup(char *str)
	{ return(basm_strdup(str)); }
int blnk_strdup_i(char *str)
	{ return(basm_strdup_i(str)); }
char *blnk_strtab_i(int str)
	{ return(basm_strtab_i(str)); }
#endif

#if 0
char *blnk_strdup(char *str)
{
	char *s, *t;
	int i, hi;

#if 1
	if(!blnk_strtab)
	{
		blnk_strtab=(char *)malloc(1<<22);
		memset(blnk_strtab, 0, 1<<22);
		blnk_strtabe=blnk_strtab+1;
		for(i=0; i<4096; i++)blnk_strhash[i]=NULL;
	}
#endif

	if((str>=blnk_strtab) && (str<blnk_strtabe))
		return(str);

	if(!str)return(NULL);
	if(!*str)return(blnk_strtab);


	s=str; i=0;
	while(*s)i=(i*251)+(*s++);
	hi=((i*251)>>8)&0xFFF;

//	t=blnk_strhash[hi];
//	if(t && !strcmp(t, str))return(t);

	if(blnk_strhash[hi] && !strcmp(blnk_strhash[hi], str))
		return(blnk_strhash[hi]);

	s=blnk_strtab+1;
	while(*s)
	{
		if(!strcmp(s, str))
			return(s);
		s+=strlen(s)+1;
	}

	strcpy(s, str);
	blnk_strtabe=s+strlen(s)+1;
	blnk_strhash[hi]=s;

	i=blnk_strtabe-blnk_strtab;
	if(i>=(1<<19))
	{
		printf("blnk_strdup: string table size limit approach\n");
	}

	return(s);
}

int blnk_strdup_i(char *str)
{
	char *s;
	s=blnk_strdup(str);
	return(s-blnk_strtab);
}

char *blnk_strtab_i(int str)
{
	return(blnk_strtab+str);
}
#endif


#if 1
char *blnk_blacklist(char *str)
{
	char *s, *t;
	int i, hi;

#if 1
	if(!blnk_strtab_bl)
	{
		blnk_strtab_bl=(char *)malloc(1<<16);
		memset(blnk_strtab_bl, 0, 1<<16);
		blnk_strtabe_bl=blnk_strtab_bl+1;
		for(i=0; i<4096; i++)blnk_strhash_bl[i]=NULL;
	}
#endif

	if((str>=blnk_strtab_bl) && (str<blnk_strtabe_bl))
		return(str);

	if(!str)return(NULL);
	if(!*str)return(blnk_strtab_bl);


	s=str; i=0;
	while(*s)i=(i*251)+(*s++);
	hi=((i*251)>>8)&0xFFF;

//	t=blnk_strhash[hi];
//	if(t && !strcmp(t, str))return(t);

	if(blnk_strhash_bl[hi] && !strcmp(blnk_strhash_bl[hi], str))
		return(blnk_strhash_bl[hi]);

	s=blnk_strtab_bl+1;
	while(*s)
	{
		if(!strcmp(s, str))
			return(s);
		s+=strlen(s)+1;
	}

	strcpy(s, str);
	blnk_strtabe_bl=s+strlen(s)+1;
	blnk_strhash_bl[hi]=s;

	i=blnk_strtabe_bl-blnk_strtab_bl;
	if(i>=(1<<15))
	{
		printf("blnk_strdup: string table size limit approach\n");
	}

	return(s);
}

int blnk_blacklist_i(char *str)
{
	char *s;
	s=blnk_blacklist(str);
	return(s-blnk_strtab_bl);
}

char *blnk_blakcktab_i(int str)
{
	return(blnk_strtab_bl+str);
}

int blnk_lookup_blacklist(char *str)
{
	char *s, *t;
	int i, hi;

#if 1
	if(!blnk_strtab_bl)
		return(-1);
#endif

	if((str>=blnk_strtab_bl) && (str<blnk_strtabe_bl))
		return(str-blnk_strtab_bl);

	if(!str)return(0);
	if(!*str)return(0);

	s=str; i=0;
	while(*s)i=(i*251)+(*s++);
	hi=((i*251)>>8)&0xFFF;

//	t=blnk_strhash[hi];
//	if(t && !strcmp(t, str))return(t);

	if(blnk_strhash_bl[hi] && !strcmp(blnk_strhash_bl[hi], str))
		return(blnk_strhash_bl[hi]-blnk_strtab_bl);

	s=blnk_strtab_bl+1;
	while(*s)
	{
		if(!strcmp(s, str))
			return(s-blnk_strtab_bl);
		s+=strlen(s)+1;
	}

	return(-1);
}
#endif


void BLNK_LinkReference(byte *ref, byte *ptr, int ty)
{
	s64 l;
	int i, j, k;

//	printf("Link %p->%p %d\n", ref, ptr, ty);

#ifdef X86_64
	l=ptr-ref;
	switch(ty)
	{
	case BLNK_JMP_SHORT:
		if(abs(l)>127)
			printf("BLNK_LinkReference: Near8 range fail\n");
		break;
	case BLNK_JMP_NEAR16:
		if(abs(l)>32767)
			printf("BLNK_LinkReference: Near16 range fail\n");
		break;
	case BLNK_JMP_NEAR32:
		if(abs(l)>2147483647)
			printf("BLNK_LinkReference: Near32 range fail\n");
		break;

	case BLNK_JMP_ABS16:
		if(((s64)ptr)>65535)
			printf("BLNK_LinkReference: Abs16 range fail\n");
		break;
	case BLNK_JMP_ABS32:
		if(((s64)ptr)>4294967295LL)
			printf("BLNK_LinkReference: Abs32 range fail\n");
		break;
	case BLNK_JMP_ABS64:
		break;
	default:
		break;
	}
#endif

	k=ptr-ref;
	switch(ty)
	{
	case BLNK_JMP_SHORT:
		k+=(char)(*(ref-1));
		*(ref-1)=k;
		break;
	case BLNK_JMP_NEAR16:
		k+=(short)((*(ref-2))+(*(ref-1)<<8));
		*(ref-2)=k&0xFF;
		*(ref-1)=(k>>8)&0xFF;
		break;
	case BLNK_JMP_NEAR32:
		k+=(int)((*(ref-4))+(*(ref-3)<<8)+
			(*(ref-2)<<16)+(*(ref-1)<<24));
		*(ref-4)=k&0xFF;
		*(ref-3)=(k>>8)&0xFF;
		*(ref-2)=(k>>16)&0xFF;
		*(ref-1)=(k>>24)&0xFF;
		break;

	case BLNK_JMP_ABS16:
#ifdef X86_64
		k=(long long)ptr;
#else
		k=(int)ptr;
#endif
		k+=(short)((*(ref-2))+(*(ref-1)<<8));
		*(ref-2)=k&0xFF;
		*(ref-1)=(k>>8)&0xFF;
		break;
	case BLNK_JMP_ABS32:
#ifdef X86_64
		k=(long long)ptr;
#else
		k=(int)ptr;
#endif
		k+=(int)((*(ref-4))+(*(ref-3)<<8)+
			(*(ref-2)<<16)+(*(ref-1)<<24));
		*(ref-4)=k&0xFF;
		*(ref-3)=(k>>8)&0xFF;
		*(ref-2)=(k>>16)&0xFF;
		*(ref-1)=(k>>24)&0xFF;
		break;
	case BLNK_JMP_ABS64:
#ifdef X86_64
		l=(long long)ptr;
#else
		l=(int)ptr;
#endif
		l+=(long long)
			(((long long)*(ref-8))+
			(((long long)*(ref-7))<<8)+
			(((long long)*(ref-6))<<16)+
			(((long long)*(ref-5))<<24)+
			(((long long)*(ref-4))<<32)+
			(((long long)*(ref-3))<<40)+
			(((long long)*(ref-2))<<48)+
			(((long long)*(ref-1))<<56));
		*(ref-8)=l&0xFF;
		*(ref-7)=(l>>8)&0xFF;
		*(ref-6)=(l>>16)&0xFF;
		*(ref-5)=(l>>24)&0xFF;
		*(ref-4)=(l>>32)&0xFF;
		*(ref-3)=(l>>40)&0xFF;
		*(ref-2)=(l>>48)&0xFF;
		*(ref-1)=(l>>56)&0xFF;
		break;

	case BLNK_JMP_ABS:
		break;
	case BLNK_JMP_ABS32NB:
		break;

	case BLNK_JMP_ARM_NEAR24:
		k+=(int)((*(ref-4))+(*(ref-3)<<8)+
			(*(ref-2)<<16));
		*(ref-4)=k&0xFF;
		*(ref-3)=(k>>8)&0xFF;
		*(ref-2)=(k>>16)&0xFF;
		break;
	case BLNK_JMP_THUMB_NEAR8:
		k+=(char)(*(ref-2));
		*(ref-2)=k;
		break;
	case BLNK_JMP_THUMB_NEAR11:
		i=((*(ref-2))+(*(ref-1)<<8))&2047;
		if(i&1024)i|=-1<<11;
		k+=(short)i;
		*(ref-2)=k&0xFF;
		*(ref-1)=(*(ref-1)&0xF8)|((k>>8)&0x07);
		break;
	case BLNK_JMP_THUMB_NEAR22:
		i=((*(ref-2))+(*(ref-1)<<8))&2047;
		j=((*(ref-4))+(*(ref-3)<<8))&2047;
		i=i|(j<<11);
		if(i&2097152)i|=-1<<22;
		k+=i;
		*(ref-4)=(k>>11)&0xFF;
		*(ref-3)=(*(ref-3)&0xF8)|((k>>19)&0x07);
		*(ref-2)=k&0xFF;
		*(ref-1)=(*(ref-1)&0xF8)|((k>>8)&0x07);
		break;

	default:
		break;
	}
}

void BLNK_UnlinkReference(byte *ref, byte *ptr, int ty)
{
	long long l;
	int i, j, k;

	k=ref-ptr;	//inverse
	switch(ty)
	{
	case BLNK_JMP_SHORT:
		k+=(char)(*(ref-1));	*(ref-1)=k;
		break;
	case BLNK_JMP_NEAR16:
		k+=(short)((*(ref-2))+(*(ref-1)<<8));
		*(ref-2)=k&0xFF;	*(ref-1)=(k>>8)&0xFF;
		break;
	case BLNK_JMP_NEAR32:
		k+=(int)((*(ref-4))+(*(ref-3)<<8)+
			(*(ref-2)<<16)+(*(ref-1)<<24));
		*(ref-4)=k&0xFF;	*(ref-3)=(k>>8)&0xFF;
		*(ref-2)=(k>>16)&0xFF;	*(ref-1)=(k>>24)&0xFF;
		break;

	case BLNK_JMP_ABS16:
		k=(short)((*(ref-2))+(*(ref-1)<<8));
#ifdef X86_64
		k-=(long long)ptr;
#else
		k-=(int)ptr;
#endif
		*(ref-2)=k&0xFF;	*(ref-1)=(k>>8)&0xFF;
		break;
	case BLNK_JMP_ABS32:
		k=(int)((*(ref-4))+(*(ref-3)<<8)+
			(*(ref-2)<<16)+(*(ref-1)<<24));
#ifdef X86_64
		k-=(long long)ptr;
#else
		k-=(int)ptr;
#endif
		*(ref-4)=k&0xFF;
		*(ref-3)=(k>>8)&0xFF;
		*(ref-2)=(k>>16)&0xFF;
		*(ref-1)=(k>>24)&0xFF;
		break;
	case BLNK_JMP_ABS64:
		l=(long long)
			(((long long)*(ref-8))+
			(((long long)*(ref-7))<<8)+
			(((long long)*(ref-6))<<16)+
			(((long long)*(ref-5))<<24)+
			(((long long)*(ref-4))<<32)+
			(((long long)*(ref-3))<<40)+
			(((long long)*(ref-2))<<48)+
			(((long long)*(ref-1))<<56));

#ifdef X86_64
		l-=(long long)ptr;
#else
		l-=(int)ptr;
#endif
		*(ref-8)=l&0xFF;	*(ref-7)=(l>>8)&0xFF;
		*(ref-6)=(l>>16)&0xFF;	*(ref-5)=(l>>24)&0xFF;
		*(ref-4)=(l>>32)&0xFF;	*(ref-3)=(l>>40)&0xFF;
		*(ref-2)=(l>>48)&0xFF;	*(ref-1)=(l>>56)&0xFF;
		break;
	default:
		break;
	}
}

void BLNK_RelinkReference(byte *ref, byte *oldptr, byte *newptr, int ty)
{
#ifdef X86_64
	s64 k, l;

	l=newptr-ref;
	switch(ty)
	{
	case BLNK_JMP_SHORT:
		if(abs(l)>127)
			printf("BLNK_LinkReference: Near8 range fail\n");
		break;
	case BLNK_JMP_NEAR16:
		if(abs(l)>32767)
			printf("BLNK_LinkReference: Near16 range fail\n");
		break;
	case BLNK_JMP_NEAR32:
		if(abs(l)>2147483647)
			printf("BLNK_LinkReference: Near32 range fail\n");
		break;

	case BLNK_JMP_ABS16:
		if(((s64)newptr)>65535)
			printf("BLNK_LinkReference: Abs16 range fail\n");
		break;
	case BLNK_JMP_ABS32:
		if(((s64)newptr)>4294967295LL)
			printf("BLNK_LinkReference: Abs32 range fail\n");
		break;
	case BLNK_JMP_ABS64:
		break;
	default:
		break;
	}

	//x86-64 does not like unaligned access...
	l=newptr-oldptr;
	switch(ty)
	{
	case BLNK_JMP_SHORT:	*(((char *)ref)-1)+=newptr-oldptr; break;
	case BLNK_JMP_NEAR16:
	case BLNK_JMP_ABS16:
		k=(short)((*(ref-2))+(*(ref-1)<<8));
		k+=l;
		*(ref-2)=k&0xFF; *(ref-1)=(k>>8)&0xFF;
		break;
	case BLNK_JMP_NEAR32:
	case BLNK_JMP_ABS32:
		k=(int)((*(ref-4))+(*(ref-3)<<8)+
			(*(ref-2)<<16)+(*(ref-1)<<24));
		k+=l;
		*(ref-4)=k&0xFF;	*(ref-3)=(k>>8)&0xFF;
		*(ref-2)=(k>>16)&0xFF;	*(ref-1)=(k>>24)&0xFF;
		break;
	case BLNK_JMP_ABS64:
		k=(s64)
			(((s64)*(ref-8))+
			(((s64)*(ref-7))<<8)+
			(((s64)*(ref-6))<<16)+
			(((s64)*(ref-5))<<24)+
			(((s64)*(ref-4))<<32)+
			(((s64)*(ref-3))<<40)+
			(((s64)*(ref-2))<<48)+
			(((s64)*(ref-1))<<56));

		k+=l;

		*(ref-8)=k&0xFF;	*(ref-7)=(k>>8)&0xFF;
		*(ref-6)=(k>>16)&0xFF;	*(ref-5)=(k>>24)&0xFF;
		*(ref-4)=(k>>32)&0xFF;	*(ref-3)=(k>>40)&0xFF;
		*(ref-2)=(k>>48)&0xFF;	*(ref-1)=(k>>56)&0xFF;
		break;
	default: break;
	}
#else
	switch(ty)
	{
	case BLNK_JMP_SHORT:	*(((char *)ref)-1)+=newptr-oldptr; break;
	case BLNK_JMP_NEAR16:	*(((short *)ref)-1)+=newptr-oldptr; break;
	case BLNK_JMP_NEAR32:	*(((int *)ref)-1)+=newptr-oldptr; break;
	case BLNK_JMP_ABS16:	*(((short *)ref)-1)+=newptr-oldptr; break;
	case BLNK_JMP_ABS32:	*(((int *)ref)-1)+=newptr-oldptr; break;
	case BLNK_JMP_ABS64:	*(((long long *)ref)-1)+=newptr-oldptr; break;
	default: break;
	}
#endif
}

void BLNK_LinkRefChain(int idx, byte *ptr)
{
	while(idx>=0)
	{
		BLNK_LinkReference(blnk_ref_addr[idx], ptr,
			blnk_ref_type[idx]&15);
		idx=blnk_ref_next[idx];
	}
}

void BLNK_UnlinkRefChain(int idx, byte *ptr)
{
	while(idx>=0)
	{
		BLNK_UnlinkReference(blnk_ref_addr[idx], ptr,
			blnk_ref_type[idx]&15);
		idx=blnk_ref_next[idx];
	}
}

void BLNK_RelinkRefChain(int idx, byte *oldptr, byte *newptr)
{
	while(idx>=0)
	{
		BLNK_RelinkReference(blnk_ref_addr[idx], oldptr, newptr,
			blnk_ref_type[idx]&15);
		idx=blnk_ref_next[idx];
	}
}

void BLNK_AddSymRef(char *name, byte *ref, int ty)
{
	int i, j;

	i=blnk_nref++;

	if(i>=blnk_mref)
	{
		blnk_mref+=blnk_mref>>1;
		blnk_ref_addr=(byte **)realloc(blnk_ref_addr, blnk_mref*sizeof(byte *));
		blnk_ref_next=(int *)realloc(blnk_ref_next, blnk_mref*sizeof(int));
		blnk_ref_type=(short *)realloc(blnk_ref_type, blnk_mref*sizeof(short));
	}

	j=BLNK_GetSymIndex(name);

	blnk_ref_addr[i]=ref;
	blnk_ref_type[i]=ty;

	blnk_ref_next[i]=blnk_sym_ref[j];
	blnk_sym_ref[j]=i;

	BLNK_LinkReference(ref, (byte *)blnk_sym_addr[j], ty);
}

char *BLNK_UnmangleName(char *str)
{
	char buf[64];
	char *s, *t;
	int i;


//	printf("BLNK_UnmangleName: %s\n", str);

	s=str;
	if((s[0]!='_') || (s[1]!='Z'))
		return(NULL);

//	printf("BLNK_UnmangleName: %s\n", str);

	s+=2;

	i=0;
	while((*s>='0') && (*s<='9'))
		i=(i*10)+((*s++)-'0');
	if(!i)return(NULL);

	t=buf;
	while(*s && i--)*t++=*s++;
	*t=0;

//	printf("BLNK_UnmangleName: %s %d\n", buf, i);

	if(i>=0)return(NULL);

	return(blnk_strdup(buf));
}

#if 0
void BLNK_AddSym(char *name, void *ptr)
{
	char *s, *name2;
	int i, hi;

	name2=NULL;
	s=BLNK_UnmangleName(name);
	if(s)
	{
		name2=name;
		name=s;
	}

	i=0; s=name;
	while(*s)i=i*251+(*s++);
	hi=(i>>8)&0xFFF;

	//see if the symbol is in the table
	i=blnk_sym_hash[hi];
	if((i>=0) && (i<blnk_nsym))
		if(!strcmp(blnk_sym_name[i], name))
	{
		BLNK_UnlinkRefChain(blnk_sym_ref[i], (byte *)blnk_sym_addr[i]);
		BLNK_LinkRefChain(blnk_sym_ref[i], (byte *)ptr);
		blnk_sym_addr[i]=ptr;
		return;
	}

	//otherwise, perform a linear search
	for(i=0; i<blnk_nsym; i++)
		if(!strcmp(blnk_sym_name[i], name))
	{
		BLNK_UnlinkRefChain(blnk_sym_ref[i], (byte *)blnk_sym_addr[i]);
		BLNK_LinkRefChain(blnk_sym_ref[i], (byte *)ptr);
		blnk_sym_addr[i]=ptr;
		blnk_sym_hash[hi]=i;
		return;
	}

	i=blnk_nsym++;
	if(i>=blnk_msym)
	{
		blnk_msym+=blnk_msym>>1;
		blnk_sym_name=(char **)realloc(blnk_sym_name, blnk_msym*sizeof(char *));
		blnk_sym_name2=(char **)realloc(blnk_sym_name2, blnk_msym*sizeof(char *));
		blnk_sym_addr=(void **)realloc(blnk_sym_addr, blnk_msym*sizeof(void *));
		blnk_sym_ref=(int *)realloc(blnk_sym_ref, blnk_msym*sizeof(int));
	}

	blnk_sym_name[i]=blnk_strdup(name);
	blnk_sym_name2[i]=blnk_strdup(name2);
	blnk_sym_addr[i]=ptr;
	blnk_sym_ref[i]=-1;

	blnk_sym_hash[hi]=i;
}

#endif


void BLNK_AddSym(char *name, void *ptr)
{
	char *s, *name2;
	int i, hi;

//	printf("BLNK_AddSym: '%s' %p\n", name, ptr);

	i=BLNK_GetSymIndex(name);
//	BLNK_UnlinkRefChain(blnk_sym_ref[i], (byte *)blnk_sym_addr[i]);
//	BLNK_LinkRefChain(blnk_sym_ref[i], (byte *)ptr);

	BLNK_RelinkRefChain(blnk_sym_ref[i],
		(byte *)blnk_sym_addr[i], (byte *)ptr);
	blnk_sym_addr[i]=ptr;

//	if(!strncmp(name, "_XN_", 4))
//		BLNK_NotifyLabelXMeta(name, ptr);

	for(i=0; i<blnk_nnotify; i++)
		blnk_notify[i](name, ptr);
}

void *BLNK_FetchSymHash(char *name)
{
	char *s;
	int i, hi;

	s=BLNK_UnmangleName(name);
	if(s)name=s;

	//compute hash-index
	i=0; s=name;
	while(*s)i=i*251+(*s++);
	hi=(i>>8)&0xFFF;

	//see if the symbol is in the table
	i=blnk_sym_hash[hi];
	if((i>=0) && (i<blnk_nsym))
		if(!strcmp(blnk_sym_name[i], name))
			return(blnk_sym_addr[i]);

	return(NULL);
}

void *BLNK_FetchSym(char *name)
{
	char *s;
	int i, hi;

	if(!strncmp(name, "G.", 2))
	{
		i=BLNK_FetchSymIndex(name+2);
		if(i<0)return(NULL);
		return((void *)(&(blnk_sym_addr[i])));
	}

	i=BLNK_FetchSymIndex(name);
	if(i<0)return(NULL);
	return(blnk_sym_addr[i]);
}

void *BLNK_FetchSymPrefix(char *name)
{
	char *s;
	int i, hi;

	i=BLNK_FetchSymIndexPrefix(name);
	if(i<0)return(NULL);
	return(blnk_sym_addr[i]);
}

int BLNK_FetchSymIndexPrefix(char *name)
{
	char *s;
	int i, hi;

	//always, perform a linear search (prefix useless for hash)
	for(i=0; i<blnk_nsym; i++)
		if(blnk_sym_name2[i] &&
			!strncmp(blnk_sym_name2[i], name, strlen(name)))
				return(i);
	return(-1);
}

char *BLNK_FetchSymPrefixName(char *name)
{
	char *s;
	int i, hi;

	i=BLNK_FetchSymIndexPrefix(name);
	if(i<0)return(NULL);
	return(blnk_sym_name2[i]);
}

int BLNK_FetchSymIndex2(char *name)
{
	char *s;
	int i, hi;

	//compute hash-index
	i=0; s=name;
	while(*s)i=i*251+(*s++);
	hi=(i>>8)&0xFFF;

	//see if the symbol is in the table
	i=blnk_sym_hash[hi];
	if((i>=0) && (i<blnk_nsym))
		if(blnk_sym_name2[i] && !strcmp(blnk_sym_name2[i], name))
			return(i);

	//otherwise, perform a linear search
	for(i=0; i<blnk_nsym; i++)
		if(blnk_sym_name2[i] && !strcmp(blnk_sym_name2[i], name))
	{
		blnk_sym_hash[hi]=i;
		return(i);
	}
	return(-1);
}

int BLNK_FetchSymIndex(char *name)
{
	char *s, *name2;
	int i, hi;

	i=BLNK_FetchSymIndex2(name);
	if(i>=0)return(i);

	name2=name;
	s=BLNK_UnmangleName(name);
	if(s) { name2=name; name=s; }

	//compute hash-index
	i=0; s=name;
	while(*s)i=i*251+(*s++);
	hi=(i>>8)&0xFFF;

	//see if the symbol is in the table
	i=blnk_sym_hash[hi];
	if((i>=0) && (i<blnk_nsym))
		if(!strcmp(blnk_sym_name[i], name))
	{
		if(name2 && blnk_sym_name2[i])
		{
			if(!strcmp(blnk_sym_name2[i], name2))
				return(i);
		}else
		{
			return(i);
		}
	}

	//otherwise, perform a linear search
	for(i=0; i<blnk_nsym; i++)
		if(!strcmp(blnk_sym_name[i], name))
	{
		if(name2 && blnk_sym_name2[i])
			if(strcmp(blnk_sym_name2[i], name2))
				continue;
		blnk_sym_hash[hi]=i;
		return(i);
	}
	return(-1);
}

int BLNK_GetSymIndex(char *name)
{
	char *s, *name2;
	int i, hi;

	i=BLNK_FetchSymIndex(name);
	if(i>=0)return(i);

	name2=name;
	s=BLNK_UnmangleName(name);
	if(s)
	{
		name2=name;
		name=s;
	}

	//compute hash-index
	i=0; s=name;
	while(*s)i=i*251+(*s++);
	hi=(i>>8)&0xFFF;

	i=blnk_nsym++;
	if(i>=blnk_msym)
	{
		blnk_msym+=blnk_msym>>1;
		blnk_sym_name=(char **)realloc(blnk_sym_name, blnk_msym*sizeof(char *));
		blnk_sym_name2=(char **)realloc(blnk_sym_name2, blnk_msym*sizeof(char *));
		blnk_sym_addr=(void **)realloc(blnk_sym_addr, blnk_msym*sizeof(void *));
		blnk_sym_ref=(int *)realloc(blnk_sym_ref, blnk_msym*sizeof(int));
	}

	blnk_sym_name[i]=blnk_strdup(name);
	blnk_sym_name2[i]=blnk_strdup(name2);
	blnk_sym_addr[i]=NULL;
	blnk_sym_ref[i]=-1;

	blnk_sym_hash[hi]=i;

	return(i);
}

static char *blnk_qhash_name[4096];
static void *blnk_qhash_ptr[4096];

int BLNK_QuickHashName(char *name)
{
	char *s;
	int i, hi;

	i=0; s=name;
	while(*s)i=i*251+(*s++);
	hi=((i*251)>>8)&0xFFF;
	return(hi);
}

int BLNK_LookupQuickHash(char *name, void **rptr)
{
	char *s;
	int i, hi;
	
	hi=BLNK_QuickHashName(name);
	s=blnk_qhash_name[hi];
	if(s && !strcmp(s, name))
	{
		*rptr=blnk_qhash_ptr[hi];
		return(1);
	}
	return(0);
}

int BLNK_AssignQuickHash(char *name, void *ptr)
{
	int i, hi;
	
	hi=BLNK_QuickHashName(name);
	blnk_qhash_name[hi]=blnk_strdup(name);
	blnk_qhash_ptr[hi]=ptr;
	return(0);
}

int BLNK_FlushQuickHash(void)
{
	int i;
	
	for(i=0; i<4096; i++)
		blnk_qhash_name[i]=NULL;
	return(0);
}

void *BLNK_LookupLabel(char *name)
{
#ifdef WIN32
	char nbuf[512];
#endif
	void *p, *q;
	int i;

	if(strlen(name)>=256)
	{
		*(int *)-1=-1;
	}

	BLNK_InitLink();

	if(BLNK_LookupQuickHash(name, &p))
		{ return(p); }

	//hack, absolute-address lavels...
#if 0
//	if((name[0]=='@') && (name[1]>='0') && (name[1]<='9'))
	if((name[0]=='@') && (name[1]=='0') && (name[2]=='x'))
	{
		p=(void *)((nlint)basm_atoi(name+1));
		BLNK_AssignQuickHash(name, p);
		return(p);
	}
#endif

#if 1
	if((name[0]=='@') && (name[1]=='0') && (name[2]=='x'))
	{
		p=BLNK_FetchSymHash(name);
//		if(p)return(p);
		if(!p)p=BLNK_FetchSym(name);
		if(p)
		{
			BLNK_AssignQuickHash(name, p);
			return(p);
		}

//		sprintf(nbuf, "@%s", name+1);
		p=(void *)((nlint)basm_atoi(name+1));
		BLNK_AssignSym(name, p);
//		BLNK_AssignSym(nbuf, p);
//		BLNK_AssignQuickHash(nbuf, p);

		p=BLNK_FetchSym(name);
		BLNK_AssignQuickHash(name, p);
		return(p);
	}
#endif

#if 1
	if((name[0]=='G') && (name[1]=='.') &&
		(name[2]=='0') && (name[3]=='x'))
	{
//		p=BLNK_FetchSymHash(name);
//		if(p)return(p);
		p=BLNK_FetchSym(name);
		if(p)
		{
			BLNK_AssignQuickHash(name, p);
			return(p);
		}

//		sprintf(nbuf, "@%s", name+2);
		p=(void *)((nlint)basm_atoi(name+2));
		BLNK_AssignSym(name+2, p);
//		BLNK_AssignSym(nbuf, p);
//		BLNK_AssignQuickHash(nbuf, p);

		p=BLNK_FetchSym(name);
		BLNK_AssignQuickHash(name, p);
		return(p);
	}
#endif

//	printf("BLNK_LookupLabelOS: %s\n", name);

	p=BLNK_FetchSymHash(name);
	if(p)return(p);

#ifdef WIN32
	sprintf(nbuf, "_%s", name);
	p=BLNK_FetchSymHash(nbuf);
	if(p)return(p);
#endif

#ifdef WIN32
	if(*name=='_')
	{
		p=BLNK_FetchSym(name);
		if(p)return(p);
		p=BLNK_FetchSym(nbuf);
		if(p)return(p);

		p=BLNK_ResolveSymbolQueue(name);
		if(p)return(p);
		p=BLNK_ResolveSymbolQueue(nbuf);
		if(p)return(p);

		p=BLNK_LookupLabelOS(name);
//		if(p)BLNK_AddSym(name, p);
		if(p)
		{
			BLNK_AssignSym(name, p);
			BLNK_AssignQuickHash(name, p);
			return(p);
		}

		for(i=0; i<blnk_nlookup; i++)
		{
			p=blnk_lookup[i](name);
			if(p)
			{
//				BLNK_AddSym(name, p);
				BLNK_AssignSym(name, p);
				BLNK_AssignQuickHash(name, p);
				return(p);
			}
		}

//		BLNK_AssignSym(name, NULL);
		BLNK_AssignQuickHash(name, NULL);
		return(NULL);
	}
#endif

#ifdef WIN32
	p=BLNK_FetchSym(nbuf);
	if(p)
	{
		BLNK_AssignQuickHash(name, p);
		return(p);
	}
#endif
	p=BLNK_FetchSym(name);
	if(p)
	{
		BLNK_AssignQuickHash(name, p);
		return(p);
	}

#ifdef WIN32
	p=BLNK_ResolveSymbolQueue(nbuf);
	if(p)
	{
		BLNK_AssignQuickHash(name, p);
		return(p);
	}
#endif
	p=BLNK_ResolveSymbolQueue(name);
	if(p)
	{
		BLNK_AssignQuickHash(name, p);
		return(p);
	}

	p=BLNK_LookupLabelOS(name);
#ifdef WIN32
	if(!p)p=BLNK_LookupLabelOS(nbuf);
#endif

	if(p)
	{
//		BLNK_AddSym(name, p);
		BLNK_AssignSym(name, p);
		BLNK_AssignQuickHash(name, p);
		return(p);
	}

	for(i=0; i<blnk_nlookup; i++)
	{
		p=blnk_lookup[i](name);
		if(p)
		{
//			BLNK_AddSym(name, p);
			BLNK_AssignSym(name, p);
			BLNK_AssignQuickHash(name, p);
			return(p);
		}
	}

//	BLNK_AssignSym(name, NULL);
	BLNK_AssignQuickHash(name, NULL);
	return(NULL);
}

int BLNK_CheckPtrBigSpace(void *ptr)
{
	long long l;
#ifdef X86_64
#ifdef WIN64
	l=((byte *)ptr)-blnk_bigspace_mid;
	if(l<0)l=-l;
	if( l >= (1LL<<31) )
	{
//		printf("Chk %p\n", (void *)l);
		return(1);
	}
#endif
#ifdef linux
	if(ptr && (((s64)ptr)&0xFFFFFFFF80000000LL) )
		return(1);
#endif
#endif
	return(0);
}

BASM_API void BLNK_AssignSym(char *name, void *ptr)
{
	char buf[256];
	void *p, *q;

	q=BLNK_FetchSym(name);

	sprintf(buf, "__proxy_%s", name);
//	p=BLNK_LookupLabel(buf);
	p=BLNK_FetchSym(buf);
	if(p)
	{
		if(ptr==q)
			return;
		*(void **)p=ptr;
		return;
	}

#ifdef WIN32
	//if windows and a func is a dll import, patch this instead
	sprintf(buf, "__imp_%s", name);
//	p=BLNK_LookupLabel(buf);
	p=BLNK_FetchSym(buf);
	if(p)
	{
		if(ptr==q)
			return;
		*(void **)p=ptr;
		return;
	}
#endif

//	p=BLNK_LookupLabel(name);
	p=BLNK_FetchSym(name);
	if(p)
	{
		BLNK_AddProxy(name);

		sprintf(buf, "__proxy_%s", name);
//		p=BLNK_LookupLabel(buf);
		p=BLNK_FetchSym(buf);
		if(!p)
		{
			printf("BLNK_AssignSym: failed create "
				"proxy for '%s'\n", name);
			BLNK_AddSym(name, ptr);
			return;
		}

//		BLNK_UpdateProxyOS(name, p);
		*(void **)p=ptr;
		return;
	}

#ifdef X86_64
// #if 0
//	if( ((s64)ptr) & 0xFFFFFFFF00000000LL )
//	if( abs(((byte *)ptr)-blnk_bigspace) >= (1LL<<31) )
	if(ptr && BLNK_CheckPtrBigSpace(ptr))
	{
		printf("BLNK_AssignSym (Proxy) %s->%p\n", name, ptr);
		BLNK_AddProxyPtr(name, ptr);
		return;
	}
#endif

//	printf("BLNK_AssignSym %s->%p\n", name, ptr);
	BLNK_AddSym(name, ptr);
	BLNK_UpdateProxyOS(name, ptr);
}


BASM_API void BLNK_AddProxyPtr(char *name, void *ptr)
{
	char tb[256];
	byte *text, *p, *q;
	int i, j, k;

	q=BLNK_FetchSym(name);

#ifdef WIN32
	sprintf(tb, "__imp_%s", name);
//	p=BLNK_LookupLabel(tb);
	p=BLNK_FetchSym(tb);
	if(p)
	{
		if(q==ptr)
			return;

		*(void **)p=ptr;
		return;
	}
#endif

	sprintf(tb, "__proxy_%s", name);
	p=BLNK_FetchSym(tb);
	if(p)
	{
		if(q==ptr)
			return;

		*(void **)p=ptr;
		return;
	}

	text=BLNK_AllocTextSz(8);

	if(!bgblink_proxy)
		bgblink_proxy=(void **)BLNK_AllocDataSz(4096*sizeof(void *));

	p=(byte *)(&bgblink_proxy[bgblink_nproxy++]);
	*(byte **)p=ptr;

#ifdef X86_64
	j=(int)(p-(text+6));
	text[0]=0xFF;		text[1]=0x25;
	text[2]=j&0xFF;		text[3]=(j>>8)&0xFF;
	text[4]=(j>>16)&0xFF;	text[5]=(j>>24)&0xFF;
#else
	j=(int)p;
	text[0]=0xFF;		text[1]=0x25;
	text[2]=j&0xFF;		text[3]=(j>>8)&0xFF;
	text[4]=(j>>16)&0xFF;	text[5]=(j>>24)&0xFF;
#endif

	BLNK_AddSym(name, text);
	BLNK_AddSym(tb, p);

	BLNK_UpdateProxyOS(name, text);
}

BASM_API void BLNK_AddProxy(char *name)
{
	char tb[256];
	byte *text, *ptr, *ptr1;
	int i, j, k;

#ifdef WIN32
	sprintf(tb, "__imp_%s", name);
	ptr=BLNK_LookupLabel(tb);
	if(ptr)return;
#endif

	sprintf(tb, "__proxy_%s", name);
	ptr=BLNK_LookupLabel(tb);

	if(ptr)
	{
		printf("BLNK_AddProxy: proxy exists for '%s'\n", name);
		return;
	}

	text=BLNK_AllocTextSz(8);

	if(!bgblink_proxy)
		bgblink_proxy=(void **)BLNK_AllocDataSz(4096*sizeof(void *));

	ptr=(byte *)(&bgblink_proxy[bgblink_nproxy++]);
	ptr1=BLNK_LookupLabel(name);
	*(byte **)ptr=ptr1;

#ifdef X86_64
	j=(int)(ptr-(text+6));
	text[0]=0xFF;		text[1]=0x25;
	text[2]=j&0xFF;		text[3]=(j>>8)&0xFF;
	text[4]=(j>>16)&0xFF;	text[5]=(j>>24)&0xFF;
#else
	j=(int)ptr;
	text[0]=0xFF;		text[1]=0x25;
	text[2]=j&0xFF;		text[3]=(j>>8)&0xFF;
	text[4]=(j>>16)&0xFF;	text[5]=(j>>24)&0xFF;
#endif

	BLNK_AddSym(name, text);
	BLNK_AddSym(tb, ptr);
	BLNK_UpdateProxyOS(name, text);
}

BASM_API void BLNK_AddProxyImport(char *name)
{
	char tb[256];
	byte *text, *ptr, *ptr1;
	int i, j, k;

	sprintf(tb, "__imp_%s", name);
	ptr=BLNK_LookupLabel(tb);
	if(ptr)return;

	text=BLNK_AllocTextSz(8);

	if(!bgblink_proxy)
		bgblink_proxy=(void **)BLNK_AllocDataSz(4096*sizeof(void *));

	ptr=(byte *)(&bgblink_proxy[bgblink_nproxy++]);
	ptr1=BLNK_LookupLabel(name);
	*(byte **)ptr=ptr1;

#ifdef X86_64
	j=(int)(ptr-(text+6));
	text[0]=0xFF;		text[1]=0x25;
	text[2]=j&0xFF;		text[3]=(j>>8)&0xFF;
	text[4]=(j>>16)&0xFF;	text[5]=(j>>24)&0xFF;
#else
	j=(int)ptr;
	text[0]=0xFF;		text[1]=0x25;
	text[2]=j&0xFF;		text[3]=(j>>8)&0xFF;
	text[4]=(j>>16)&0xFF;	text[5]=(j>>24)&0xFF;
#endif

	BLNK_AddSym(name, text);
	BLNK_AddSym(tb, ptr);
	BLNK_UpdateProxyOS(name, text);
}

byte *BLNK_LinkModule(BLNK_Object *ctx)
{
	byte *text, *data, *ptr;
	char *s, *t;
	int tsz, dsz;
	int i, j, k;

	if(!ctx)return(NULL);

	BLNK_FlushQuickHash();

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

	for(i=0; i<ctx->n_gotos; i++)
	{
		s=ctx->goto_name[i];
		t="__imp_";
		if(!strncmp(s, t, strlen(t)))
		{
			BLNK_AddProxyImport(s+strlen(t));
		}
	}

//	tsz=ctx->ip-ctx->text;
//	dsz=ctx->dp-ctx->data;

	tsz=ctx->text_sz;
	dsz=ctx->data_sz;

	text=BLNK_AllocTextSz(tsz);
	data=BLNK_AllocDataSz(dsz);

	i=blnk_nmod++;
	blnk_mod_name[i]=ctx->name;
	blnk_mod_text[i]=text;
	blnk_mod_data[i]=data;
	blnk_mod_textsz[i]=tsz;
	blnk_mod_datasz[i]=dsz;


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
			BLNK_AssignSym(ctx->label_name[i], ptr);
		}

		if(k==2)
		{
			ptr=(byte *)BLNK_FetchSym(ctx->label_name[i]);
			if(ptr)
			{
				printf("Ignore redefined variable '%s'\n",
					ctx->label_name[i]);
				continue;
			}
			ptr=data+j;
			BLNK_AddSym(ctx->label_name[i], ptr);
		}

//		printf("Bind %s %p (%d %X)\n", ctx->label_name[i], ptr, k, j);
	}

	for(i=0; i<ctx->n_gotos; i++)
	{
		BLNK_LookupLabel(ctx->goto_name[i]);

		j=ctx->goto_pos[i]&0xFFFFFF;
		k=ctx->goto_pos[i]>>24;
		ptr=NULL;
		if(k==1)ptr=text+j;
		if(k==2)ptr=data+j;
		BLNK_AddSymRef(ctx->goto_name[i], ptr, ctx->goto_type[i]);
	}

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

#if 0
#ifdef X86_64
	BDISASM_PrintOpcodes(text, 1, tsz);
#else
	BDISASM_PrintOpcodes(text, 0, tsz);

//	BDISASM_PrintOpcodes(BLNK_GetPtr("mcount"), 0, 64);
#endif
#endif

	return(text);
}

byte *BLNK_LinkModuleObj(char *name, void *buf, int sz)
{
	BLNK_Object *ctx;
	byte *p;

	ctx=BLNK_COFF_LoadObjectBuf(name, buf, sz);
	if(!ctx)return(NULL);

	p=BLNK_LinkModule(ctx);
	BLNK_DestroyContext(ctx);
	return(p);
}

void BLNK_LinkModuleQueue(BLNK_Object *ctx)
{
	if(!ctx)return;

	ctx->next=blnk_queue;
	blnk_queue=ctx;
}

void BLNK_LinkModuleObjQueue(char *name, void *buf, int sz)
{
	BLNK_Object *ctx;
	byte *p;

	ctx=BLNK_COFF_LoadObjectBuf(name, buf, sz);
	if(!ctx)return;

	BLNK_LinkModuleQueue(ctx);
}

void BLNK_UnlinkModuleQueue(BLNK_Object *ctx)
{
	BLNK_Object *cur, *prv;

	if(!ctx)return;

	cur=blnk_queue; prv=NULL;
	while(cur)
	{
		if(cur==ctx)break;
		prv=cur; cur=cur->next;
	}

	if(cur)
	{
		if(prv)
		{
			prv->next=cur->next;
			return;
		}else
		{
			blnk_queue=cur->next;
		}
	}
}

BLNK_Object *BLNK_LookupSymbolQueue(char *sym)
{
	BLNK_Object *cur;
	int i;

	cur=blnk_queue;
	while(cur)
	{
		for(i=0; i<cur->n_labels; i++)
			if(!strcmp(cur->label_name[i], sym))
		{
			return(cur);
		}
		cur=cur->next;
	}

	return(NULL);
}

void *BLNK_ResolveSymbolQueue(char *sym)
{
	BLNK_Object *ctx;
	void *p;

	ctx=BLNK_LookupSymbolQueue(sym);
	if(ctx)
	{
		BLNK_UnlinkModuleQueue(ctx);
		BLNK_LinkModule(ctx);
		BLNK_DestroyContext(ctx);

		p=BLNK_FetchSym(sym);
		return(p);
	}


	return(NULL);
}


int BLNK_LookupModule(char *name)
{
	int i;

	for(i=0; i<blnk_nmod; i++)
		if(blnk_mod_name[i] && !strcmp(blnk_mod_name[i], name))
			return(i);
	return(-1);
}

void *BLNK_LookupModuleText(char *name, int *rsz)
{
	void *p;
	int i, j;

	for(i=0; i<blnk_nmod; i++)
		if(blnk_mod_name[i] && !strcmp(blnk_mod_name[i], name))
	{
		p=blnk_mod_text[i];
		j=blnk_mod_textsz[i];

		*rsz=j;
		return(p);
	}
	return(NULL);
}

void *BLNK_LookupModuleData(char *name, int *rsz)
{
	void *p;
	int i, j;

	for(i=0; i<blnk_nmod; i++)
		if(blnk_mod_name[i] && !strcmp(blnk_mod_name[i], name))
	{
		p=blnk_mod_data[i];
		j=blnk_mod_datasz[i];

		*rsz=j;
		return(p);
	}
	return(NULL);
}


char *BLNK_GetNamePtr(void *ptr)
{
	void *q;
	char *s;
	int i;

	for(i=0; i<blnk_nsym; i++)
		if(blnk_sym_addr[i]==ptr)
			return(blnk_sym_name[i]);

	s=BLNK_LookupAddrNameOS(ptr, &q);
	if(s && (q==ptr))return(s);

	return(NULL);
}

char *BLNK_GetLastNamePtr(void *ptr, void **rbp)
{
	void *p, *bp;
	int i, j, bd;
	char *s, *bn;

	bp=NULL; bn=NULL;
	for(i=0; i<blnk_nsym; i++)
	{
		p=blnk_sym_addr[i];
		if(p>ptr)continue;
		if(p==ptr)
		{
			if(rbp)*rbp=p;
			return(blnk_sym_name[i]);
		}

		j=((byte *)ptr)-((byte *)p);
		if(j<0)j=999999999;
		if(!bp || (j<bd))
			{ bp=p; bd=j; bn=blnk_sym_name[i]; }

	}

	s=BLNK_LookupAddrNameOS(ptr, &p);
	j=((byte *)ptr)-((byte *)p);
	if(j<0)j=999999999;
	if(!bp || (j<bd))
		{ bp=p; bd=j; bn=s; }

#if 0
	if(!bp)
	{
		s=BLNK_LookupAddrNameOS(ptr, rbp);
		if(s)return(s);

		return(NULL);
	}
#endif

	if(rbp)*rbp=bp;
//	return(blnk_sym_name[bn]);
	return(bn);
}

int BLNK_EnumUndefSyms(char **lst, int max)
{
	int i, j;

	for(i=0; i<blnk_nsym; i++)
		if(!blnk_sym_addr[i])
			BLNK_LookupLabel(blnk_sym_name[i]);

	j=0;
	for(i=0; i<blnk_nsym; i++)
		if(!blnk_sym_addr[i])
	{
		if(j>=max)break;
		lst[j++]=blnk_sym_name[i];
	}

	return(j);
}

void BLNK_AddSymArray(char **name, void **ptr, int cnt)
{
	int i;

	for(i=0; i<cnt; i++)
		BLNK_AddSym(name[i], ptr[i]);
}

BASM_API int BLNK_LoadLibrary(char *name)
{
	int i;

	i=BLNK_LoadDynamicOS(name);
	if(i>=0)
	{
		BLNK_FlushQuickHash();
		return(i);
	}

	i=BLNK_COFF_LoadLibrary(name);
	if(i>=0)
	{
		BLNK_FlushQuickHash();
		return(i);
	}

	return(-1);
}

