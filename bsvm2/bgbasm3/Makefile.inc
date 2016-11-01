BASM_SRCS = \
	front/basm_api.c \
	front/basm_rand.c \
	front/basm_callsig.c \
	front/basm_vfs.c \
	front/basm_preproc.c \
	\
	asm/basm_core.c \
	asm/basm_parse.c \
	asm/basm_insns_x86.c \
	asm/basm_insns_arm.c \
	asm/basm_insns_thumb.c \
	asm/basm_disasm.c \
	asm/basm_coff.c \
	\
	link/link_context.c \
	link/link_core.c \
	link/link_thunk.c \
	link/link_lnx.c \
	link/link_w32.c \
	link/link_vfs.c \
	link/bldr_coff.c \
	link/bldr_elf.c

#	front/basm_xcall.c \
#	front/basm_xc_sysv.c \
#	front/basm_xc_w64.c \
#	front/basm_xmeta.c \
#	front/basm_unwind.c \

SRCS = \
	$(BASM_SRCS)

MSRCS = \
	basm_multi.c


DEP_HDRS=\
	../include/basm/bgbasm_x86ops.h \
	../include/basm/bgbasm_armops.h \
	../include/basm/bgbasm_thumbops.h \
	../include/basm/bgbasm_auto.h \


all: libs apps

FORCE:

libs: $(DLLPF)bgbasm$(DLLSUF) 
apps: basmtst$(EXESUF) basmtst2$(EXESUF) basmtst3$(EXESUF)

includes: $(DEP_HDRS)

../include/basm/bgbasm_auto.h: autohead$(BINSUF) $(SRCS)
	./autohead$(BINSUF) -api BASM_API $(SRCS) > \
		../include/basm/bgbasm_auto.h

../include/basm/bgbasm_x86ops.h: genx86$(BINSUF) asm/basm_insns_x86.txt
	./genx86$(BINSUF) -Mx86
../include/basm/bgbasm_armops.h: genx86$(BINSUF) asm/basm_insns_arm.txt
	./genx86$(BINSUF) -Marm
../include/basm/bgbasm_thumbops.h: genx86$(BINSUF) asm/basm_insns_thumb.txt
	./genx86$(BINSUF) -Mthumb

$(DLLPF)bgbasm$(DLLSUF): $(DEP_HDRS) $(SRCS)
	$(CC_OUTDLL)$(DLLPF)bgbasm$(DLLSUF) $(MSRCS) \
		$(DLL_CFLAGS) $(DLL_LDFLAGS)

autohead$(BINSUF): autohead.c
	$(CC_OUTTOOL)autohead$(BINSUF) autohead.c

genx86$(BINSUF): gen_x86_4.c
	$(CC_OUTTOOL)genx86$(BINSUF) gen_x86_4.c $(CFLAGS)

basmtst$(EXESUF): basmtst.c $(DEP_HDRS) libs
	$(CC_OUTEXE)basmtst$(EXESUF) basmtst.c $(CFLAGS) $(LDFLAGS)

basmtst2$(EXESUF): basmtst2.c $(DEP_HDRS) libs
	$(CC_OUTEXE)basmtst2$(EXESUF) basmtst2.c $(CFLAGS) $(LDFLAGS)

basmtst3$(EXESUF): basmtst3.c $(DEP_HDRS) libs
	$(CC_OUTEXE)basmtst3$(EXESUF) basmtst3.c $(CFLAGS) $(LDFLAGS)

%$(OBJSUF): %.c
	$(CC_OUTOBJ)$@ $< $(CFLAGS)

clean:

clean2:

