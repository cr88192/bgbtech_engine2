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
	base/snd_adpcm.c \
	base/snd_mixer.c \
	base/snd_sample.c \
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
	mm/mm_tagparse.c \
	mm/mm_tagprint.c \
	mm/mm_tagstr.c \
	mm/mm_thread.c \
	mm/mm_typesig.c \
	mm/mm_xi128.c \
	mm/mm_xvec.c \
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
	bsvm2c/bs2c_topstmt.c \
	bsvm2c/bs2c_type.c \
	bsvm2c/bs2e_expr.c \
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
	bsvm2i/bs2i_opobj.c \
	bsvm2i/bs2i_opvars.c \
	bsvm2i/bs2i_opvec2.c \
	bsvm2i/bs2i_opvec3.c \
	bsvm2i/bs2i_tailop.c \
	bsvm2i/bs2i_topinit.c \
	\
	bsvm2i/bs2j_tr_x86.c \
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

BGBASM_SRCS = \
	bgbasm3/front/basm_api.c \
	bgbasm3/front/basm_rand.c \
	bgbasm3/front/basm_callsig.c \
	bgbasm3/front/basm_vfs.c \
	bgbasm3/front/basm_preproc.c \
	\
	bgbasm3/asm/basm_core.c \
	bgbasm3/asm/basm_parse.c \
	bgbasm3/asm/basm_insns_x86.c \
	bgbasm3/asm/basm_insns_arm.c \
	bgbasm3/asm/basm_insns_thumb.c \
	bgbasm3/asm/basm_disasm.c \
	bgbasm3/asm/basm_coff.c \
	\
	bgbasm3/link/link_context.c \
	bgbasm3/link/link_core.c \
	bgbasm3/link/link_thunk.c \
	bgbasm3/link/link_lnx.c \
	bgbasm3/link/link_w32.c \
	bgbasm3/link/link_vfs.c \
	bgbasm3/link/bldr_coff.c \
	bgbasm3/link/bldr_elf.c


BTEIFGL_MULTISRCS=\
	base/base_multi.c \
	mm/mm_multi.c \
	bsvm2c/bs2c_multi.c \
	bsvm2i/bs2i_multi.c \
	voxel/vox_multi.c \
	btews/btws_multi.c \
	bgbasm3/basm_multi.c \


SRCS=$(BTEIFGL_SRCS)
MSRCS=$(BTEIFGL_MULTISRCS)

DEP_HDRS=\
	include/bteifgl_auto.h \
	include/basm/bgbasm_auto.h

all: libs apps

FORCE:

libs: $(DLLPF)btewsim$(DLLSUF)

apps: deeptail0$(EXESUF) bsvm2tst0$(EXESUF) \
	btewiresim0$(EXESUF) msptst0$(EXESUF) \
	isotest0$(EXESUF)

includes: $(DEP_HDRS)

include/bteifgl_auto.h: autohead$(BINSUF) $(SRCS)
	./autohead$(BINSUF) -api BTEIFGL_API $(SRCS) > include/bteifgl_auto.h

include/basm/bgbasm_auto.h: autohead$(BINSUF) $(BGBASM_SRCS)
	./autohead$(BINSUF) -api BASM_API $(BGBASM_SRCS) \
		> include/basm/bgbasm_auto.h

$(DLLPF)btewsim$(DLLSUF): $(DEP_HDRS) $(SRCS) $(MSRCS)
	$(CC_OUTDLL)$(DLLPF)btewsim$(DLLSUF) $(MSRCS) $(DLL_CFLAGS) $(DLL_LDFLAGS)

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

isotest0$(EXESUF): isotest0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)isotest0$(EXESUF) isotest0.c $(CFLAGS) $(LDFLAGS)

clean:
clean2:

