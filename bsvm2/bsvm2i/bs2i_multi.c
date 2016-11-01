/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <bsvm2.h>

#if defined(X86) || (defined(X86_64) && defined(WIN64))
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
#include "bs2i_opvxi.c"
#include "bs2i_tailop.c"
#include "bs2i_topinit.c"

#include "bs2j_rvalut.c"
#include "bs2j_tr_x86.c"
#include "bs2j_tr_x64.c"
