#ifdef _MSC_VER
#pragma warning(disable:4996)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef BGBLINK_H
#define BGBLINK_H

#include <basm/bgbasm_conf.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
int (*scanrange_f)(void **p, int cnt);

void *(*gcalloc_f)(size_t sz);
void *(*gcexec_f)(size_t sz);
void *(*gcatomic_f)(size_t sz);
void *(*gctalloc_f)(char *ty, size_t sz);
void *(*gctexec_f)(char *ty, size_t sz);
void *(*gctatomic_f)(char *ty, size_t sz);

void (*gcfree_f)(void *p);
void *(*gcrealloc_f)(void *p, size_t sz);

char *(*gcgettype_f)(void *p);

int (*gcalloctls_f)(char *name, int sz);
int (*gcgettlstls_f)();
}BLNK_GCFuncs_t;

typedef struct {
void *(*fopen_fp)(char *name, char *mode);
void (*fclose_fp)(void *fd);
int (*fread_fp)(void *buf, int m, int n, void *fd);
int (*fwrite_fp)(void *buf, int m, int n, void *fd);
int (*feof_fp)(void *fd);
s64 (*ftell_fp)(void *fd);
int (*fseek_fp)(void *fd, s64 pos, int rel);
}BLNK_IOFuncs_t;

typedef struct BLNK_Scope_s BLNK_Scope;
typedef struct BLNK_ScopeSym_s BLNK_ScopeSym;
typedef struct BLNK_Env_s BLNK_Env;

struct BLNK_ScopeSym_s {
char *name;
char *sig;
void *addr;
};

struct BLNK_Scope_s {
BLNK_Scope *super;
BLNK_ScopeSym **syms;
int n_syms, m_syms;
};

struct BLNK_Env_s {
BLNK_Env *super;
void *data[1];
};

typedef struct {
byte jump[16];
byte *text;
byte *data;
BLNK_Scope *scope;
BLNK_Scope *env_info;
BLNK_Env *xf_env;	//initial env (XFrame)
byte *xf_ip;		//initial rIP (XFrame)
BLNK_Scope *args_info;
}BLNK_Thunk;

typedef void *(*blnk_lookup_ft)(char *sym);
typedef void (*blnk_notify_ft)(char *sym, void *ptr);


#include <basm/bgbasm_lnkctx.h>
#include <basm/bgbasm_lnkobj.h>

#ifdef __cplusplus
}
#endif

#endif
