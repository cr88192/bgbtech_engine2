#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef BGBASM_H
#define BGBASM_H

#if !defined(X86) && !defined(X86_64)
#ifdef __x86_64__
#define X86_64
#endif

#ifdef __i386__
#define X86
#endif
#endif


#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif

typedef struct {
int (*scanrange_f)(void **p, int cnt);

void *(*gcalloc_f)(int sz);
void *(*gcexec_f)(int sz);
void *(*gcatomic_f)(int sz);
void *(*gctalloc_f)(char *ty, int sz);
void *(*gctexec_f)(char *ty, int sz);
void *(*gctatomic_f)(char *ty, int sz);

void (*gcfree_f)(void *p);
void *(*gcrealloc_f)(void *p, int sz);

char *(*gcgettype_f)(void *p);

int (*gcalloctls_f)(char *name, int sz);
int (*gcgettlstls_f)();
}BGBASM_GCFuncs_t;

typedef struct BGBASM_Scope_s BGBASM_Scope;
typedef struct BGBASM_ScopeSym_s BGBASM_ScopeSym;
typedef struct BGBASM_Env_s BGBASM_Env;

struct BGBASM_ScopeSym_s {
char *name;
char *sig;
void *addr;
};

struct BGBASM_Scope_s {
BGBASM_Scope *super;
BGBASM_ScopeSym **syms;
int n_syms, m_syms;
};

struct BGBASM_Env_s {
BGBASM_Env *super;
void *data[1];
};

typedef struct {
byte jump[16];
byte *text;
byte *data;
BGBASM_Scope *scope;
BGBASM_Scope *env_info;
BGBASM_Env *xf_env;	//initial env (XFrame)
byte *xf_ip;		//initial rIP (XFrame)
BGBASM_Scope *args_info;
}BGBASM_Thunk;

typedef struct {
void *seh_next;	//next chain (nested unwind)
void *seh_hdlr;	//handler (SEH specific)
void *regs[12];	//saved regs (for restore)
void *seh_ehr;	//EXCEPTION_RECORD (SEH)
void *seh_ctx;	//CONTEXT (SEH)
}BGBASM_JmpBuf;


typedef void *(*basm_meta_ft)(char *sym, char *name, char **args);
typedef void (*basm_mtrg_ft)(char *sym, char *name, char **args, void *ptr);

typedef struct BASM_XMeta_s BASM_XMeta;
struct BASM_XMeta_s {
BASM_XMeta *next;
char *name;
basm_meta_ft mfcn;
basm_mtrg_ft mtrg;

char **tmp;
int ntmp, mtmp;
};


#include <bgbasm_asm.h>
#include <bgbasm_obj.h>

#include <bgbasm_x86ops.h>

#include <bgbasm_auto_front.h>
#include <bgbasm_auto_asm.h>
#include <bgbasm_auto_link.h>


#endif
