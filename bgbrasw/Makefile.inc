#	base/memory2_2$(OBJSUF) \
#	base/int128$(OBJSUF) \

OBJS=\
	base/rasw_api$(OBJSUF) \
	base/rasw_context$(OBJSUF) \
	base/rasw_drawbox$(OBJSUF) \
	base/rasw_drawprim$(OBJSUF) \
	base/rasw_drawline$(OBJSUF) \
	base/rasw_drawspan$(OBJSUF) \
	base/rasw_drawtris$(OBJSUF) \
	base/rasw_drawtris_flat$(OBJSUF) \
	base/rasw_drawtris_tex$(OBJSUF) \
	base/rasw_drsp_flat$(OBJSUF) \
	base/rasw_drsp_tblend$(OBJSUF) \
	base/rasw_targa$(OBJSUF) \
	base/rasw_testblend$(OBJSUF) \
	base/rasw_texture$(OBJSUF) \
	base/rasw_thread$(OBJSUF) \
	\
	base/rasw_bc7_bitstream$(OBJSUF) \
	base/rasw_bc7_common$(OBJSUF) \
	base/rasw_bc7_dec$(OBJSUF) \
	\
	raswgl/ragl_begin$(OBJSUF) \
	raswgl/ragl_clip$(OBJSUF) \
	raswgl/ragl_gldrv$(OBJSUF) \
	raswgl/ragl_glw$(OBJSUF) \
	raswgl/ragl_light$(OBJSUF) \
	raswgl/ragl_raswprim$(OBJSUF) \
	raswgl/ragl_teximage$(OBJSUF) \
	raswgl/ragl_transform$(OBJSUF) \
	raswgl/ragl_vertexarray$(OBJSUF) \
	\
	vecmath/math_clip$(OBJSUF) \
	vecmath/math_hullf$(OBJSUF) \
	vecmath/math_m3$(OBJSUF) \
	vecmath/math_m4$(OBJSUF) \
	vecmath/math_mf$(OBJSUF) \
	vecmath/math_n$(OBJSUF) \
	vecmath/math_plane$(OBJSUF) \
	vecmath/math_quat$(OBJSUF) \
	vecmath/math_solid$(OBJSUF) \
	vecmath/math_v3f$(OBJSUF) \
	vecmath/math_vecnf$(OBJSUF) \
	vecmath/math_vnf$(OBJSUF) \
	vecmath/mathlib_d$(OBJSUF) \
	vecmath/mathlib_f$(OBJSUF) \

#	base/rasw_bitmap$(OBJSUF) \
#	base/rasw_yytestblend$(OBJSUF) \

SRCS=$(OBJS:$(OBJSUF)=.c)

MULTI_SRCS=\
	bgbrasw_multi.c

SRCS_RASWGL32=\
	glstub/rasw_glext.c \
	glstub/rasw_glstub.c \
	glstub/rasw_gl_sgl.c \

#	\
#	glstub/glstub.def

SRCS_RASWGLDRV_NULL=\
	glstub/rasw_gl_null.c \

DEP_HDRS=\
	include/bgbrasw_auto.h

all: libs apps

FORCE:

libs: $(DLLPF)bgbrasw$(DLLSUF) \
	$(DLLPF)rasw_opengl32$(DLLSUF) \
	$(DLLPF)rasw_gldrv_null$(DLLSUF)
	cp *$(DLLSUF) ..

apps: bgbraswtst0$(EXESUF)  bgbraswgltst0$(EXESUF)

includes: $(DEP_HDRS)

include/bgbrasw_auto.h: autohead$(BINSUF) $(SRCS)
	./autohead$(BINSUF) -api BGBRASW_API $(SRCS) > include/bgbrasw_auto.h
	./autohead$(BINSUF) -apionly VECMATH_API $(SRCS) >> include/bgbrasw_auto.h

%$(OBJSUF): %.c
	$(CC_OUTOBJ)$@ $< $(CFLAGS)

$(DLLPF)bgbrasw$(DLLSUF): $(DEP_HDRS) $(SRCS)
	$(CC_OUTDLL)$(DLLPF)bgbrasw$(DLLSUF) $(MULTI_SRCS) \
		$(DLL_CFLAGS) $(DLL_LDFLAGS)

$(DLLPF)rasw_opengl32$(DLLSUF): $(DEP_HDRS) $(SRCS_RASWGL32)
	$(CC_OUTDLL)$(DLLPF)rasw_opengl32$(DLLSUF) $(SRCS_RASWGL32) \
	$(DLL_CFLAGS) $(DLL_LDFLAGS)

$(DLLPF)rasw_gldrv_null$(DLLSUF): $(DEP_HDRS) $(SRCS_RASWGLDRV_NULL)
	$(CC_OUTDLL)$(DLLPF)rasw_gldrv_null$(DLLSUF) $(SRCS_RASWGLDRV_NULL) \
	$(DLL_CFLAGS) $(DLL_LDFLAGS)

# bgbgc.lib: $(DEP_HDRS) $(OBJS)
#	lib /out:bgbgc.lib $(OBJS)

autohead$(BINSUF): autohead.c
	$(CC_OUTTOOL)autohead$(BINSUF) autohead.c

bgbraswtst0$(EXESUF): bgbraswtst0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)bgbraswtst0$(EXESUF) bgbraswtst0.c $(CFLAGS) $(LDFLAGS)

bgbraswgltst0$(EXESUF): bgbraswgltst0.c $(DEP_HDRS) libs
	$(CC_OUTEXE)bgbraswgltst0$(EXESUF) bgbraswgltst0.c $(CFLAGS) $(LDFLAGS)

clean:
clean2:

