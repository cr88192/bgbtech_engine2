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

#include "bs2p_astnode.c"
#include "bs2p_decl.c"
#include "bs2p_litexpr.c"
#include "bs2p_stmt.c"
#include "bs2p_token.c"

#include "bs2p_preproc.c"

#include "bs2c_compile.c"
#include "bs2c_conv.c"
#include "bs2c_disasm.c"
#include "bs2c_emit.c"
#include "bs2c_expr.c"
#include "bs2c_image.c"
#include "bs2c_infer.c"
#include "bs2c_jcond.c"
#include "bs2c_reduce.c"
#include "bs2c_scope.c"
#include "bs2c_stmt.c"
#include "bs2c_topstmt.c"
#include "bs2c_type.c"
#include "bs2e_expr.c"
