#include <bteifgl.h>

#if defined(X86)
#define BS2I_USE_BASM

#ifndef BGBASM_DLL
#define BGBASM_DLL
#endif

#endif

#include "bs2i_fcnblock.c"
#include "bs2i_interp.c"
#include "bs2i_load.c"
#include "bs2i_natcall.c"
#include "bs2i_oparith.c"
#include "bs2i_oparr.c"
#include "bs2i_opcode.c"
#include "bs2i_opcvt.c"
#include "bs2i_opdtvar.c"
#include "bs2i_opinit.c"
#include "bs2i_opobj.c"
#include "bs2i_opptr.c"
#include "bs2i_opvars.c"
#include "bs2i_opvec2.c"
#include "bs2i_opvec3.c"
#include "bs2i_tailop.c"
#include "bs2i_topinit.c"

#include "bs2j_tr_x86.c"
