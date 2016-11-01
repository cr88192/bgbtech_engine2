#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <time.h>

#ifndef BGBASM_DLL
#define BGBASM_DLL
#endif

//#include <bsvm2_bsvm21.h>
#include <bsvm2.h>

#include <bsvm2_basm.h>

#include "front/basm_api.c"
#include "front/basm_rand.c"
#include "front/basm_callsig.c"
#include "front/basm_vfs.c"
// #include "front/basm_xcall.c"
// #include "front/basm_xc_sysv.c"
// #include "front/basm_xc_w64.c"
// #include "front/basm_xmeta.c"
// #include "front/basm_unwind.c"
#include "front/basm_preproc.c"

#include "asm/basm_core.c"
#include "asm/basm_parse.c"
#include "asm/basm_insns_x86.c"
#include "asm/basm_insns_arm.c"
#include "asm/basm_insns_thumb.c"
#include "asm/basm_disasm.c"
#include "asm/basm_coff.c"

#include "link/link_context.c"
#include "link/link_core.c"
#include "link/link_thunk.c"
#include "link/link_lnx.c"
#include "link/link_w32.c"
#include "link/link_vfs.c"
#include "link/bldr_coff.c"
// #include "link/bldr_elf.c"
