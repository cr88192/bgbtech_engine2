BTEIFGL_SRCS=\
	base/frgl_concmds.c \
	base/frgl_console.c \
	base/frgl_cvar.c \
	base/frgl_draw.c \
	base/frgl_drawarr.c \
	base/frgl_drawstub.c \
	base/frgl_fonts.c \
	base/frgl_fonts_qi2.c \
	base/frgl_input.c \
	base/frgl_shader.c \
	base/frgl_texture.c \
	base/frgl_util.c \
	base/frgl_texmat.c \
	\
	base/math_m3.c \
	base/math_m4.c \
	\
	base/gpio_rdtsc.c \
	base/gpio_dummy_w32.c \
	base/net_btemsg.c \
	\
	base/deflate.c \
	base/inflate.c \
	base/bufpng.c \
	\
	base/w32_glw.c \
	base/w32_mouse.c \
	base/w32_sound.c \
	\
	base/w32_guid.c \
	base/w32_net.c \
	base/w32_udpip.c \
	\
	mm/mm_cell.c \
	mm/mm_clsobj.c \
	mm/mm_lobj.c \
	mm/mm_mapobj.c \
	mm/mm_objty.c \
	mm/mm_tagarith.c \
	mm/mm_tagarr.c \
	mm/mm_tagprint.c \
	mm/mm_tagstr.c \
	mm/mm_thread.c \
	\
	bsvm2c/bs2p_astnode.c \
	bsvm2c/bs2p_decl.c \
	bsvm2c/bs2p_litexpr.c \
	bsvm2c/bs2p_stmt.c \
	bsvm2c/bs2p_token.c \
	\
	bsvm2c/bs2c_compile.c \
	bsvm2c/bs2c_disasm.c \
	bsvm2c/bs2c_emit.c \
	bsvm2c/bs2c_expr.c \
	bsvm2c/bs2c_image.c \
	bsvm2c/bs2c_infer.c \
	bsvm2c/bs2c_jcond.c \
	bsvm2c/bs2c_reduce.c \
	bsvm2c/bs2c_scope.c \
	bsvm2c/bs2c_stmt.c \
	bsvm2c/bs2c_type.c \
	\
	bsvm2i/bs2i_fcnblock.c \
	bsvm2i/bs2i_interp.c \
	bsvm2i/bs2i_load.c \
	bsvm2i/bs2i_natcall.c \
	bsvm2i/bs2i_oparith.c \
	bsvm2i/bs2i_oparr.c \
	bsvm2i/bs2i_opcode.c \
	bsvm2i/bs2i_opcvt.c \
	bsvm2i/bs2i_opdtvar.c \
	bsvm2i/bs2i_opinit.c \
	bsvm2i/bs2i_opvars.c \
	bsvm2i/bs2i_tailop.c \
	bsvm2i/bs2i_topinit.c \
	\
	entity/ent_tagval.c \
	\
	voxel/vox_chunk.c \
	voxel/vox_decrice.c \
	voxel/vox_draw.c \
	voxel/vox_drawent.c \
	voxel/vox_light.c \
	voxel/vox_mesh.c \
	voxel/vox_rice.c \
	voxel/vox_trace.c \
	voxel/vox_voxty.c \
	voxel/vox_world.c \
	\
	btews/btws_diode.c \
	btews/btws_dip.c \
	btews/btws_grid.c \
	btews/btws_netnode.c \
	btews/btws_resistor.c \
	btews/btws_transistor.c \
	btews/btws_wire.c \
	\
	btews/msp430i/mspi_interp.c \
	btews/msp430i/mspi_mmu.c \
	btews/msp430i/mspi_oparith.c \
	btews/msp430i/mspi_opcode.c \
	btews/msp430i/mspi_opmisc.c \
	

SRCS=$(BTEIFGL_SRCS)

DEP_HDRS=\
	include/bteifgl_auto.h

all: libs apps

FORCE:

libs: $(DLLPF)btewsim$(DLLSUF)

apps: deeptail0$(EXESUF) bsvm2tst0$(EXESUF) \
	btewiresim0$(EXESUF) msptst0$(EXESUF)

includes: $(DEP_HDRS)

include/bteifgl_auto.h: autohead$(BINSUF) $(SRCS)
	./autohead$(BINSUF) -api BTEIFGL_API $(SRCS) > include/bteifgl_auto.h

$(DLLPF)btewsim$(DLLSUF): $(DEP_HDRS) $(SRCS)
	$(CC_OUTDLL)$(DLLPF)btewsim$(DLLSUF) $(SRCS) $(DLL_CFLAGS) $(DLL_LDFLAGS)

autohead$(BINSUF): autohead.c
	$(CC_OUTTOOL)autohead$(BINSUF) autohead.c

deeptail0$(EXESUF): deeptail0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)deeptail0$(EXESUF) deeptail0.c $(CFLAGS) $(LDFLAGS)

bsvm2tst0$(EXESUF): bsvm2tst0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)bsvm2tst0$(EXESUF) bsvm2tst0.c $(CFLAGS) $(LDFLAGS)

btewiresim0$(EXESUF): btewiresim0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)btewiresim0$(EXESUF) btewiresim0.c $(CFLAGS) $(LDFLAGS)

msptst0$(EXESUF): msptst0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)msptst0$(EXESUF) msptst0.c $(CFLAGS) $(LDFLAGS)

clean:
clean2:

