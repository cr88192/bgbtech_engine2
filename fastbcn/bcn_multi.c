#include <bteifgl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// #include "bcn_decls.h"

#include "bcn_bitstream.c"

#include "bc7_common.c"
#include "bc7_blkdec.c"
#include "bc7_filter_cmy.c"
#include "bc7_filter_yuv.c"
#include "bc7_blkenc.c"
#include "bc7_blkenc_p2.c"

#include "bcn_s2blkenc.c"
#include "bcn_s2blkdec.c"

#include "bcn_decjpg.c"
#include "bcn_encjpg.c"

#include "bt1h_bitrange.c"
#include "bt1h_blkdec.c"
// #include "bt1h_blkdec_pyuv.c"
#include "bt1h_common.c"
#include "bt1h_decrice.c"
#include "bt1h_decode.c"
#include "bt1h_rangedec.c"
#include "bt1h_shim_bc7.c"
#include "bt1h_shim_s2tc.c"

// #include "bt1h_targa.c"

