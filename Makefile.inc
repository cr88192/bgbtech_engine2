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
	base/frgl_menu.c \
	base/frgl_shaddll.c \
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
	base/btpk_image.c \
	\
	base/snd_adpcm.c \
	base/snd_btac1c1.c \
	base/snd_btac1c2.c \
	base/snd_mixer.c \
	base/snd_sample.c \
	\
	base/vf31_btpk.c \
	base/vf31_osdir.c \
	base/vf31_vfile.c \
	\
	base/w32_glw.c \
	base/w32_mouse.c \
	base/w32_sound.c \
	\
	base/ems_glw.c \
	base/ems_sound.c \
	\
	base/w32_guid.c \
	base/w32_net.c \
	base/w32_udpip.c \
	\
	voxel/vox_chunk.c \
	voxel/vox_decrice.c \
	voxel/vox_draw.c \
	voxel/vox_drawent.c \
	voxel/vox_light.c \
	voxel/vox_load.c \
	voxel/vox_mesh.c \
	voxel/vox_rice.c \
	voxel/vox_save.c \
	voxel/vox_tick.c \
	voxel/vox_trace.c \
	voxel/vox_voxty.c \
	voxel/vox_world.c \
	\
	voxel/vox_felz32.c \
	voxel/vox_tg_mare.c \
	\
	entity/ent_bsvm.c \
	entity/ent_dialog.c \
	entity/ent_part.c \
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
	\
	fastbcn/bcn_bitstream.c \
	fastbcn/bc7_common.c \
	fastbcn/bc7_blkdec.c \
	fastbcn/bc7_filter_cmy.c \
	fastbcn/bc7_filter_yuv.c \
	fastbcn/bc7_blkenc.c \
	fastbcn/bc7_blkenc_p2.c \
	fastbcn/bcn_s2blkenc.c \
	fastbcn/bcn_s2blkdec.c \
	fastbcn/bcn_decjpg.c \
	fastbcn/bcn_encjpg.c \
	fastbcn/bcn_bt1h_common.c \
	fastbcn/bcn_bt1h_decrice.c \
	fastbcn/bcn_bt1h_blkdec_pyuv.c \
	fastbcn/bcn_bt1h_decode.c \
	\
	btic4b/bt4b_bitread.c \
	btic4b/bt4b_bitwrite.c \
	btic4b/bt4b_blkdec.c \
	btic4b/bt4b_blkenc.c \
	btic4b/bt4b_decode.c \
	btic4b/bt4b_encode.c \
	btic4b/bt4b_grad16.c \
	btic4b/bt4b_shim_bc7.c \
	btic4b/bt4b_shim_s2tc.c \
	btic4b/bt4b_dump_hdr.c \
	\
	btlzh/btlza_fastdfl.c		\
	btlzh/btlza_bitdecode.c	\
	btlzh/btlza_bitencode.c	\
	btlzh/btlza_bitrange.c	\
	btlzh/btlza_bitrange3.c	\
	btlzh/btlza_bitstream.c	\
	btlzh/btlza_decode.c		\
	btlzh/btlza_encode.c		\


BTEIFGL_MULTISRCS=\
	base/base_multi.c \
	voxel/vox_multi.c \
	btews/btws_multi.c \
	fastbcn/bcn_multi.c \
	btic4b/bt4b_multi.c \
	btlzh/btlza_multi.c \
	\
	$(BSVM2_MULTISRCS)

#	entity/ent_tagval.c \
#	\

SRCS=$(BTEIFGL_SRCS)
MSRCS=$(BTEIFGL_MULTISRCS)

# BSVM2_BASE=bsvm2/

DEP_HDRS=\
	include/bteifgl_auto.h \
	$(BSVM2_BASE)include/basm/bgbasm_auto.h \
	$(BSVM2_BASE)include/bsvm2_auto.h

# all: libs apps

FORCE:

include $(BSVM2_BASE)Makefile.inc

libs: $(DLLPF)btewsim$(DLLSUF)

apps: deeptail0$(EXESUF) bsvm2tst0$(EXESUF) \
	btewiresim0$(EXESUF) msptst0$(EXESUF) \
	isotest0$(EXESUF) btactst0$(EXESUF) \
	plattest0$(EXESUF) btactst1$(EXESUF)

apps2: deeptail0$(EXESUF)

includes: $(DEP_HDRS)

include/bteifgl_auto.h: autohead$(BINSUF) $(SRCS)
	./autohead$(BINSUF) -api BTEIFGL_API $(SRCS) > include/bteifgl_auto.h

# include/bsvm2/basm/bgbasm_auto.h: autohead$(BINSUF) $(BGBASM_SRCS)
#	./autohead$(BINSUF) -api BASM_API $(BGBASM_SRCS) \
#		> include/bsvm2/basm/bgbasm_auto.h

# include/bsvm2/bsvm2_auto.h: autohead$(BINSUF) $(BS2VM_SRCS)
#	./autohead$(BINSUF) -api BS2VM_API $(BS2VM_SRCS) \
#		> include/bsvm2/bsvm2_auto.h

$(DLLPF)btewsim$(DLLSUF): $(DEP_HDRS) $(SRCS) $(MSRCS)
	$(CC_OUTDLL)$(DLLPF)btewsim$(DLLSUF) $(MSRCS) $(DLL_CFLAGS) $(DLL_LDFLAGS)

autohead$(BINSUF): autohead.c
	$(CC_OUTTOOL)autohead$(BINSUF) autohead.c

deeptail0$(EXESUF): deeptail0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)deeptail0$(EXESUF) deeptail0.c $(CFLAGS) $(LDFLAGS)

btewiresim0$(EXESUF): btewiresim0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)btewiresim0$(EXESUF) btewiresim0.c $(CFLAGS) $(LDFLAGS)

msptst0$(EXESUF): msptst0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)msptst0$(EXESUF) msptst0.c $(CFLAGS) $(LDFLAGS)

isotest0$(EXESUF): isotest0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)isotest0$(EXESUF) isotest0.c $(CFLAGS) $(LDFLAGS)

btactst0$(EXESUF): btactst0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)btactst0$(EXESUF) btactst0.c $(CFLAGS) $(LDFLAGS)

btactst1$(EXESUF): btactst1.c $(DEP_HDRS) libs
	$(CC_OUTEXE)btactst1$(EXESUF) btactst1.c $(CFLAGS) $(LDFLAGS)

plattest0$(EXESUF): plattest0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)plattest0$(EXESUF) plattest0.c $(CFLAGS) $(LDFLAGS)

clean:
clean2:

