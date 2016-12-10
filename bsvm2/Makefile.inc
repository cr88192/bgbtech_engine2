export BS2VM_SRCS=\
	$(BSVM2_BASE)mm/mm_cell.c \
	$(BSVM2_BASE)mm/mm_clsobj.c \
	$(BSVM2_BASE)mm/mm_lobj.c \
	$(BSVM2_BASE)mm/mm_mapobj.c \
	$(BSVM2_BASE)mm/mm_objty.c \
	$(BSVM2_BASE)mm/mm_tagarith.c \
	$(BSVM2_BASE)mm/mm_tagarr.c \
	$(BSVM2_BASE)mm/mm_tagparse.c \
	$(BSVM2_BASE)mm/mm_tagprint.c \
	$(BSVM2_BASE)mm/mm_tagstr.c \
	$(BSVM2_BASE)mm/mm_thread.c \
	$(BSVM2_BASE)mm/mm_typesig.c \
	$(BSVM2_BASE)mm/mm_util.c \
	$(BSVM2_BASE)mm/mm_vfs.c \
	$(BSVM2_BASE)mm/mm_worker.c \
	$(BSVM2_BASE)mm/mm_xi128.c \
	$(BSVM2_BASE)mm/mm_xf128.c \
	$(BSVM2_BASE)mm/mm_xd128.c \
	$(BSVM2_BASE)mm/mm_xvec.c \
	\
	$(BSVM2_BASE)bsvm2c/bs2p_astnode.c \
	$(BSVM2_BASE)bsvm2c/bs2p_decl.c \
	$(BSVM2_BASE)bsvm2c/bs2p_litexpr.c \
	$(BSVM2_BASE)bsvm2c/bs2p_stmt.c \
	$(BSVM2_BASE)bsvm2c/bs2p_token.c \
	$(BSVM2_BASE)bsvm2c/bs2p_preproc.c \
	\
	$(BSVM2_BASE)bsvm2c/bs2c_compile.c \
	$(BSVM2_BASE)bsvm2c/bs2c_conv.c \
	$(BSVM2_BASE)bsvm2c/bs2c_disasm.c \
	$(BSVM2_BASE)bsvm2c/bs2c_emit.c \
	$(BSVM2_BASE)bsvm2c/bs2c_expr.c \
	$(BSVM2_BASE)bsvm2c/bs2c_image.c \
	$(BSVM2_BASE)bsvm2c/bs2c_infer.c \
	$(BSVM2_BASE)bsvm2c/bs2c_jcond.c \
	$(BSVM2_BASE)bsvm2c/bs2c_reduce.c \
	$(BSVM2_BASE)bsvm2c/bs2c_scope.c \
	$(BSVM2_BASE)bsvm2c/bs2c_stmt.c \
	$(BSVM2_BASE)bsvm2c/bs2c_topstmt.c \
	$(BSVM2_BASE)bsvm2c/bs2c_type.c \
	$(BSVM2_BASE)bsvm2c/bs2e_expr.c \
	\
	$(BSVM2_BASE)bsvm2i/bs2i_fcnblock.c \
	$(BSVM2_BASE)bsvm2i/bs2i_interp.c \
	$(BSVM2_BASE)bsvm2i/bs2i_load.c \
	$(BSVM2_BASE)bsvm2i/bs2i_natcall.c \
	$(BSVM2_BASE)bsvm2i/bs2i_oparith.c \
	$(BSVM2_BASE)bsvm2i/bs2i_oparr.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opcode.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opcvt.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opdtvar.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opinit.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opobj.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opptr.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opvars.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opvec2.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opvec3.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opvec3x.c \
	$(BSVM2_BASE)bsvm2i/bs2i_opvxi.c \
	$(BSVM2_BASE)bsvm2i/bs2i_tailop.c \
	$(BSVM2_BASE)bsvm2i/bs2i_topinit.c \
	\
	$(BSVM2_BASE)bsvm2i/bs2j_rvalut.c \
	$(BSVM2_BASE)bsvm2i/bs2j_tr_x86.c \
	$(BSVM2_BASE)bsvm2i/bs2j_tr_x64.c \


export BGBASM_SRCS = \
	$(BSVM2_BASE)bgbasm3/front/basm_api.c \
	$(BSVM2_BASE)bgbasm3/front/basm_rand.c \
	$(BSVM2_BASE)bgbasm3/front/basm_callsig.c \
	$(BSVM2_BASE)bgbasm3/front/basm_vfs.c \
	$(BSVM2_BASE)bgbasm3/front/basm_preproc.c \
	\
	$(BSVM2_BASE)bgbasm3/asm/basm_core.c \
	$(BSVM2_BASE)bgbasm3/asm/basm_parse.c \
	$(BSVM2_BASE)bgbasm3/asm/basm_insns_x86.c \
	$(BSVM2_BASE)bgbasm3/asm/basm_insns_arm.c \
	$(BSVM2_BASE)bgbasm3/asm/basm_insns_thumb.c \
	$(BSVM2_BASE)bgbasm3/asm/basm_disasm.c \
	$(BSVM2_BASE)bgbasm3/asm/basm_coff.c \
	\
	$(BSVM2_BASE)bgbasm3/link/link_context.c \
	$(BSVM2_BASE)bgbasm3/link/link_core.c \
	$(BSVM2_BASE)bgbasm3/link/link_thunk.c \
	$(BSVM2_BASE)bgbasm3/link/link_lnx.c \
	$(BSVM2_BASE)bgbasm3/link/link_w32.c \
	$(BSVM2_BASE)bgbasm3/link/link_vfs.c \
	$(BSVM2_BASE)bgbasm3/link/bldr_coff.c \
	$(BSVM2_BASE)bgbasm3/link/bldr_elf.c

export BSVM2_MULTISRCS = \
	$(BSVM2_BASE)mm/mm_multi.c \
	$(BSVM2_BASE)bsvm2c/bs2c_multi.c \
	$(BSVM2_BASE)bsvm2i/bs2i_multi.c \
	$(BSVM2_BASE)bgbasm3/basm_multi.c \

DEP_HDRS_BSVM=\
	$(BSVM2_BASE)include/basm/bgbasm_auto.h \
	$(BSVM2_BASE)include/bsvm2_auto.h

all_bsvm: libs_bsvm bsvm2tst0$(EXESUF) xt128tst0$(EXESUF)

libs_bsvm: $(DLLPF)btbsvm2$(DLLSUF)

$(BSVM2_BASE)include/basm/bgbasm_auto.h: \
		autohead$(BINSUF) $(BGBASM_SRCS)
	./autohead$(BINSUF) -api BASM_API $(BGBASM_SRCS) \
		> $(BSVM2_BASE)include/basm/bgbasm_auto.h

$(BSVM2_BASE)include/bsvm2_auto.h: \
		autohead$(BINSUF) $(BS2VM_SRCS)
	./autohead$(BINSUF) -api BS2VM_API $(BS2VM_SRCS) \
		> $(BSVM2_BASE)include/bsvm2_auto.h

$(DLLPF)btbsvm2$(DLLSUF): $(DEP_HDRS_BSVM) $(SRCS) $(BSVM2_MULTISRCS)
	$(CC_OUTDLL)$(DLLPF)btbsvm2$(DLLSUF) $(BSVM2_MULTISRCS) \
		$(DLL_CFLAGS) $(DLL_LDFLAGS)

autohead$(BINSUF): autohead.c
	$(CC_OUTTOOL)autohead$(BINSUF) autohead.c

bsvm2tst0$(EXESUF): bsvm2tst0.c $(DEP_HDRS_BSVM) libs_bsvm
	$(CC_OUTEXE)bsvm2tst0$(EXESUF) bsvm2tst0.c $(CFLAGS) $(LDFLAGS)

xt128tst0$(EXESUF): xt128tst0.c $(DEP_HDRS_BSVM) libs_bsvm
	$(CC_OUTEXE)xt128tst0$(EXESUF) xt128tst0.c $(CFLAGS) $(LDFLAGS)
