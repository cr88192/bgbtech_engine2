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

// #include <bgbasm_asm.h>
#include <basm/bgbasm_link.h>

#ifndef BGBASM_H
#define BGBASM_H

#include <basm/bgbasm_conf.h>

#ifdef _MSC_VER
#define BASM_THREAD __declspec(thread)
#else
#ifdef __GNUC__
#define BASM_THREAD __thread
#endif
#endif

#if defined(ARM) && defined(BASM_THREAD)
#undef BASM_THREAD
#endif

#ifndef BASM_THREAD
#define BASM_THREAD
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <basm/bgbasm_cpu.h>
#include <basm/bgbasm_obj.h>

#include <basm/bgbasm_x86ops.h>
#include <basm/bgbasm_armops.h>
#include <basm/bgbasm_thumbops.h>

#if 0
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
#endif

typedef struct {
void *seh_next;	//next chain (nested unwind)
void *seh_hdlr;	//handler (SEH specific)
#ifdef WIN64
void *regs[16+20];	//saved regs (for restore)
#else
void *regs[12];	//saved regs (for restore)
#endif
void *seh_ehr;	//EXCEPTION_RECORD (SEH)
void *seh_ctx;	//CONTEXT (SEH)
}BGBASM_JmpBuf;

typedef struct {
#ifdef X86_64
void *gregs[16];	//@0
int xregs[16*4];	//@128
void *spregs[4];	//@384, special (return RIP, return RSP)
#else
void *gregs[8];		//@0
int xregs[8*4];		//@32
void *spregs[4];	//
#endif
}BGBASM_RegsBuf;

// typedef struct {
// }BGBASM_ApiThreadState;

typedef BLNK_IOFuncs_t BGBASM_IOFuncs_t;

#include <basm/bgbasm_auto.h>

#ifdef __cplusplus
}
#endif

#endif
