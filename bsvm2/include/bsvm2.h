#ifndef BSVM2_H
#define BSVM2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <math.h>

#ifdef BTEIFGL_DLL
#define BS2VM_DLL
#endif

#include <bsvm2_conf.h>

typedef struct BGBDT_MM_ParsePrintInfo_s BGBDT_MM_ParsePrintInfo;

#include <bsvm2_mm.h>

#include <bsvm2_tagval.h>
#include <bsvm2_tagclass.h>

#include <bsvm2_bs2cc.h>
#include <bsvm2_bsvm21.h>

struct BGBDT_MM_ParsePrintInfo_s {
BGBDT_MM_ParsePrintInfo *next;
char *buf;
char *cs, *css, *cse;
char *ct, *cts, *cte;

dtVal recarr[256];
int nrec;
int nchars;

int (*putstr)(BGBDT_MM_ParsePrintInfo *inf, char *str);
};

#include <bsvm2_auto.h>

#endif
