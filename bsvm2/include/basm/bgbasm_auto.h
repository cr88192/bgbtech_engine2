//AHSRC:bsvm2/bgbasm3/front/basm_api.c
BASM_API char *BASM_CPUID_String();
BASM_API u32 BASM_CPUID_Version();
BASM_API u32 BASM_CPUID_Unit();
BASM_API u32 BASM_CPUID_Ext();
BASM_API u32 BASM_CPUID_Feature();
BASM_API char *BASM_CPUID_FeatureStr();
BASM_API void BASM_AddNameBlacklist(char *name);
BASM_API void BASM_AddNamesBlacklist(char *nameslst);
BASM_API void *BASM_GetEBP();
BASM_API int BASM_GetReturnState(void **regs);
BASM_API int BASM_StackTrace(void **ip, void **bp, int lim);
BASM_API int BASM_StackTrace(void **ip, void **bp, int lim);
int BASM_Win64CheckEpilogueRet(byte *ip);
BASM_API byte *BASM_Win64LocateEpilogue(byte *ip);
BASM_API byte *BASM_Win64AdjustEpilogueRSP(byte *ip, byte *sp,BGBASM_RegsBuf *regs);
BASM_API int BASM_StackTrace(void **ip, void **bp, int lim);
BASM_API int BASM_StackTrace(void **ip, void **bp, int lim);
BASM_API void basm_putc(int c);
BASM_API void basm_puts(char *str);
BASM_API void basm_vprint(char *str, va_list lst);
BASM_API void basm_print(char *str, ...);
BASM_API void BASM_RegisterPtr(char *lbl, void *ptr);
BASM_API void *BASM_GetPtr(char *lbl);
BASM_API char *BASM_GetPtrName(void *ptr);
BASM_API char *BASM_GetLastNamePtr(void *ptr, void **rbp);
BASM_API void *BASM_FetchSymPrefix(char *name);
BASM_API char *BASM_FetchSymPrefixName(char *name);
BASM_API char *BASM_LookupExeSectionPtrName(void *ptr);
BASM_API char *BASM_LookupSectionPtrName(void *ptr);
BASM_API void *BASM_CreateLabelBSS(char *lbl, int sz);
BASM_API void BASM_DumpModule(char *name);
BASM_API void *BASM_GetFPtrDummy(char *lbl);
void *BASM_GetIP();
void *BASM_GetDP();
BASM_API char *BASM_GetAsmBuf();
BASM_API void BASM_ResetCtx(BASM_Context *ctx);
BASM_API void BASM_CopyLblCtx(BASM_Context *ctx);
BASM_API void BASM_AssembleCtxBuf(BASM_Context *ctx, char *buf);
BASM_API void BASM_AssembleCtxBufFast(BASM_Context *ctx, char *buf);
BASM_API void BASM_BeginAssembly(char *name);
BASM_API int basm_jitlog_puts(char *str);
BASM_API int basm_jitlog_printf(char *str, ...);
BASM_API void *BASM_EndAssembly();
BASM_API void *BASM_EndAssemblyQuiet();
BASM_API void *BASM_EndAssemblyDebug();
BASM_API void *BASM_EndAssemblyCache(char *name);
BASM_API void BASM_EndAssemblyFile(char *name);
BASM_API byte *BASM_EndAssemblyObjBuf(int *rsz);
BASM_API byte *BASM_EndAssemblyCacheObjBuf(int *rsz);
BASM_API BLNK_Thunk *BASM_EndAssemblyThunk(BLNK_Scope *scope);
BASM_API byte *BASM_EndAssemblyThunkInline(BLNK_Scope *scope);
BASM_API void BASM_EndAssemblyQueue();
BASM_API void *BASM_EndAssemblyFast();
BASM_API byte *BASM_EndAssemblyObjBufFast(int *rsz);
BASM_API byte *BASM_EndAssemblyCacheObjBufFast(int *rsz);
BASM_API void BASM_EndAssemblyQueueFast();
BASM_API void BASM_EndAssemblyVirtualHeader();
BASM_API void BASM_FreeAssembly(void *ip);
BASM_API void BASM_FreeObjBuf(void *ip);
BASM_API char *BASM_GenSym();
BASM_API char *BASM_GenSym2();
BASM_API int BASM_LoadObjectBuffer(char *name, byte *buf, int sz);
BASM_API int BASM_LoadFile(char *name);
BASM_API int BASM_AssembleFile(char *iname, char *oname);
BASM_API int BASM_ProcessEXE(char *name);
BASM_API void BASM_ThreadLocalInit();
BASM_API void BASM_Init();
//AHSRC:bsvm2/bgbasm3/front/basm_rand.c
int basm_genseed();
void BASM_InitRand();
u32 basm_rand();
char *basm_rand_key12();
char *basm_rand_key18();
//AHSRC:bsvm2/bgbasm3/front/basm_callsig.c
int BASM_LenSig(char *sig, char **esig);
int BASM_LenSig(char *sig, char **esig);
BASM_API void BASM_CallSig(void *fcn, char *sig, void *buf, void *ret);
BASM_API void BASM_CallSigStd(void *fcn, char *sig, void *buf, void *ret);
int BASM_SplitArgsSig(char *sig, char **esig,byte *buf, byte *abuf, byte *greg, byte *xreg);
int BASM_SplitArgsSig(char *sig, char **esig,byte *buf, byte *abuf, byte *greg, byte *xreg);
BASM_API void BASM_CallSig(void *fcn, char *sig, void *buf, void *ret);
BASM_API void BASM_CallSigStd(void *fcn, char *sig, void *buf, void *ret);
BASM_API void BASM_CallSig(void *fcn, char *sig, void *buf, void *ret);
void BASM_ProcessSigArgs(char *sig, va_list lst, void *buf);
int BASM_MergeArgsSig(char *sig, char **esig,byte *buf, byte *abuf, byte *greg, byte *xreg);
int BASM_MergeArgsSig(char *sig, char **esig,byte *buf, byte *abuf, byte *greg, byte *xreg);
void BASM_CreateStub_Inner32(void *args, void **data);
void BASM_CreateStub_Inner64(void *args, void **data, void *greg, void *xreg);
BASM_API void BASM_CreateStub(void *buf, void *fcn, void *data, char *sig);
BASM_API void *BASM_CreateStubGC(void *fcn, void *data, char *sig);
void BASM_InitSig();
BASM_API void *BASM_MakeBufferCallStub(void *fcn, char *sig);
BASM_API void *BASM_MakeBufferCallStub(void *fcn, char *sig);
void BASM_InitSig();
BASM_API void *BASM_MakeBufferCallStub(void *fcn, char *sig);
//AHSRC:bsvm2/bgbasm3/front/basm_vfs.c
s64 basm_tell_tmp(void *fd);
int basm_seek_tmp(void *fd, s64 pos, int rel);
BGBASM_IOFuncs_t *basm_getio();
void basm_initvfs();
void *basm_fopen(char *name, char *mode);
void basm_fclose(void *fd);
int basm_fread(void *buf, int m, int n, void *fd);
int basm_fwrite(void *buf, int m, int n, void *fd);
int basm_feof(void *fd);
s64 basm_ftell(void *fd);
int basm_fseek(void *fd, s64 pos, int rel);
int basm_fgetc(void *fd);
void basm_fputc(int c, void *fd);
void *basm_loadfile(char *name, int *rsz);
int basm_storefile(char *name, void *buf, int sz);
int basm_storetextfile(char *name, char *buf);
//AHSRC:bsvm2/bgbasm3/front/basm_preproc.c
void *basm_loadfile(char *name, int *rsz);
int BASM_PP_AddIncludePathFront(char *str);
int BASM_PP_AddIncludePathBack(char *str);
int BASM_PP_AddVirtualHeader(char *name, char *buf);
char *BASM_PP_LookupVirtualHeader(char *name);
char *BASM_PP_EmitString(char *t, char *s);
char *BASM_PP_EmitCharString(char *t, char *s);
void BASM_PP_DeleteDefine(char *name);
void BASM_PP_CleanupDefines();
void BASM_PP_CleanupLocalDefines();
void BASM_PP_AddDefine(char *name, char *str);
void BASM_PP_AddDefineArgs(char *name, char **args, char *str);
void BASM_PP_AppendDefineLine(char *name, char *str);
void BASM_PP_AddStaticDefine(char *name, char *str);
void BASM_PP_AddStaticDefineArgs(char *name, char **args, char *str);
void BASM_PP_AddLocalDefine(char *name, char *str);
void BASM_PP_AddLocalDefineArgs(char *name, char **args, char *str);
char *BASM_PP_LoadInclude(char *name, int *rsz);
void BASM_PP_Include(char *str);
void BASM_PP_PushIncludeLevel();
void BASM_PP_PopIncludeLevel();
void BASM_PP_Directive2(char *str);
void BASM_PP_Directive(char *str);
char *BASM_PP_ParseArg(char *s, char *b);
char *BASM_PP_PrintName(char *t, char *s);
char *BASM_PP_PrintToken(char *t, char *s, int ty);
int BASM_PP_Expand(char **src, char **dst, char *op);
int BASM_PP_ExpandBinary(char *larg, char *op, char *rarg, char *dst);
int BASM_PP_LineArith(char *str);
int BASM_PP_Line(char *str);
char *BASM_PP_ParseLine(char *s, char *b);
void BASM_PP_Buffer(char *ibuf);
int BASM_PP_Filter(char *ibuf, char *obuf);
//AHSRC:bsvm2/bgbasm3/asm/basm_core.c
void BASM_SetCPU(BASM_Context *ctx, int cpu);
void basm_warning(char *str, ...);
void basm_error(char *str, ...);
BASM_API BASM_Context *BASM_NewContext();
BASM_API void BASM_DestroyContext(BASM_Context *ctx);
BASM_API void *BASM_TempAllocTy(BASM_Context *ctx, char *ty, int sz);
BASM_API void *BASM_TempAlloc(BASM_Context *ctx, int sz);
BASM_API void BASM_OutPadText(BASM_Context *ctx, int i);
BASM_API void BASM_OutPadZero(BASM_Context *ctx, int i);
BASM_API void BASM_OutPadVLI(BASM_Context *ctx, int i);
BASM_API void BASM_OutPad(BASM_Context *ctx, int i);
BASM_API void BASM_Align(BASM_Context *ctx, int i);
BASM_API int BASM_GetSectionOffset(BASM_Context *ctx);
BASM_API void BASM_BundleAlign(BASM_Context *ctx);
BASM_API void BASM_Align(BASM_Context *ctx, int i);
BASM_API void BASM_AlignVLI(BASM_Context *ctx, int i);
BASM_API void BASM_OutByte(BASM_Context *ctx, int i);
BASM_API void BASM_OutWord(BASM_Context *ctx, int i);
BASM_API void BASM_OutDWord(BASM_Context *ctx, int i);
BASM_API void BASM_OutQWord(BASM_Context *ctx,long long i);
BASM_API void BASM_OutUVLIP(BASM_Context *ctx,unsigned long long i, int pad);
BASM_API void BASM_OutUVLI(BASM_Context *ctx, unsigned long long i);
BASM_API void BASM_OutSVLI(BASM_Context *ctx, long long i);
BASM_API void BASM_OutSVLIP(BASM_Context *ctx, long long i, int pad);
BASM_API void BASM_OutStr8(BASM_Context *ctx, char *str);
BASM_API void BASM_OutStr16(BASM_Context *ctx, char *str);
BASM_API void BASM_OutStr8Z(BASM_Context *ctx, char *str);
BASM_API void BASM_OutStr16Z(BASM_Context *ctx, char *str);
BASM_API void BASM_OutBytes(BASM_Context *ctx, byte *buf, int sz);
BASM_API void BASM_OutSection(BASM_Context *ctx, char *str);
BASM_API void BASM_OutBits(BASM_Context *ctx, int bits);
BASM_API void BASM_OutRelPtrDisp(BASM_Context *ctx, char *lbl, int disp);
BASM_API void BASM_OutRelSPtrDisp(BASM_Context *ctx, char *lbl, int disp);
void BASM_ModRM16(BASM_Context *ctx, int reg,int rm, int sc, int idx, int disp, char *lbl);
void BASM_Sib(BASM_Context *ctx, int rm, int sc, int idx);
BASM_API void BASM_LabelDisp(BASM_Context *ctx, char *lbl, int disp);
void BASM_LabelDispRel(BASM_Context *ctx, char *lbl, int disp);
void BASM_ModRM64(BASM_Context *ctx, int reg,int rm, int sc, int idx, int disp, char *lbl);
BASM_API void BASM_ModRM(BASM_Context *ctx, int reg,int rm, int sc, int idx, int disp, char *lbl);
BASM_API int BASM_RegREXW(int reg);
BASM_API int BASM_Reg16P(int reg);
BASM_API int BASM_Reg32P(int reg);
BASM_API void BASM_AddrOverride(BASM_Context *ctx, int breg);
BASM_API void BASM_SegOverride(BASM_Context *ctx);
BASM_API char *BASM_OutSufBytes(BASM_Context *ctx, char *s);
BASM_API char *BASM_OutBodyBytes(BASM_Context *ctx, char *s, int rex);
BASM_API char *BASM_OutImm(BASM_Context *ctx, char *s,long long imm, char *lbl);
BASM_API char *BASM_OutMOffs(BASM_Context *ctx,char *s, int disp, char *lbl);
BASM_API char *BASM_OutModRM(BASM_Context *ctx, char *s, int reg,int breg, int sc, int ireg, int disp, char *lbl);
BASM_API char *BASM_OutArmReg(BASM_Context *ctx, char *s, int reg);
BASM_API int BASM_SizeOpStr(BASM_Context *ctx, char *str);
BASM_API void BASM_BundleAlignOpStr(BASM_Context *ctx, char *str);
BASM_API void BASM_OutOpStr(BASM_Context *ctx, char *s);
BASM_API void BASM_OutOpStrReg(BASM_Context *ctx, char *s, int reg);
BASM_API void BASM_OutOpStrImm(BASM_Context *ctx,char *s, long long imm, char *lbl);
BASM_API void BASM_OutOpStrMem(BASM_Context *ctx, char *s,char *lbl, int breg, int ireg, int sc, int disp);
BASM_API void BASM_OutOpStrRegReg(BASM_Context *ctx,char *s, int r0, int r1);
BASM_API void BASM_OutOpStrRegImm(BASM_Context *ctx, char *s, int reg,long long imm, char *lbl);
BASM_API void BASM_OutOpStrRegMem(BASM_Context *ctx, char *s, int reg,char *lbl, int breg, int ireg, int sc, int disp);
BASM_API void BASM_OutOpStrMemImm(BASM_Context *ctx, char *s, int w,char *lbl, int breg, int ireg, int sc, int disp, long long imm, char *lbl2);
BASM_API void BASM_OutOpStrRegRegImm(BASM_Context *ctx, char *s,int r0, int r1, long long imm, char *lbl);
BASM_API void BASM_OutOpStrRegMemImm(BASM_Context *ctx, char *s, int reg,char *lbl, int breg, int ireg, int sc, int disp, long long imm, char *lbl2);
BASM_API void BASM_OutOpStrRegRegReg(BASM_Context *ctx, char *s,int r0, int r1, int r2);
BASM_API void BASM_OutOpStrRegMemReg(BASM_Context *ctx, char *s, int reg,char *lbl, int breg, int ireg, int sc, int disp, int reg2);
BASM_API int BASM_OpSingleP(int i);
BASM_API int BASM_GetRegWidth(int reg);
int BASM_OpArgRegP(int i, int j, int reg);
int BASM_OpArgRegP(int i, int j, int reg);
int BASM_OpArgMemP(int i, int j, int w, int fl);
int BASM_OpArgMemP(int i, int j, int w, int fl);
int BASM_OpArgImmP(int i, int j, int w, long long imm);
int BASM_OpArchP(BASM_Context *ctx, int i);
int BASM_OpRegP(int i, int reg);
int BASM_OpImmP(int i, int w, long long imm);
int BASM_OpMemP(int i, int w);
int BASM_OpRegRegP(int i, int r0, int r1);
int BASM_OpRegImmP(int i, int reg, long long imm);
int BASM_OpRegMemP(int i, int reg, int fl);
int BASM_OpMemRegP(int i, int reg, int fl);
int BASM_OpMemImmP(int i, int w, long long imm);
int BASM_OpRegRegImmP(int i, int r0, int r1, long long imm);
int BASM_OpRegMemImmP(int i, int reg, int fl, long long imm);
int BASM_OpMemRegImmP(int i, int reg, int fl, long long imm);
int BASM_OpRegRegRegP(int i, int r0, int r1, int r2);
int BASM_OpRegMemRegP(int i, int r0, int r1, int fl);
int BASM_OpMemRegRegP(int i, int r0, int r1, int fl);
BASM_API void BASM_OutOpSingle(BASM_Context *ctx, int op);
BASM_API void BASM_OutOpReg(BASM_Context *ctx, int op, int reg);
BASM_API void BASM_OutOpImm(BASM_Context *ctx, int op, int w,long long imm, char *lbl);
BASM_API void BASM_OutOpMem(BASM_Context *ctx, int op, int w,char *lbl, int breg, int ireg, int sc, long long disp);
BASM_API void BASM_OutOpRegReg(BASM_Context *ctx, int op, int r0, int r1);
BASM_API void BASM_OutOpRegImm(BASM_Context *ctx, int op,int reg, long long imm, char *lbl);
BASM_API void BASM_OutOpRegMem(BASM_Context *ctx, int op, int reg,char *lbl, int breg, int ireg, int sc, long long disp);
BASM_API void BASM_OutOpMemReg(BASM_Context *ctx, int op, int reg,char *lbl, int breg, int ireg, int sc, long long disp);
BASM_API void BASM_OutOpMemImm(BASM_Context *ctx, int op, int w,char *lbl, int breg, int ireg, int sc, long long disp, long long imm, char *lbl2);
BASM_API void BASM_OutOpRegRegImm(BASM_Context *ctx,int op, int r0, int r1, long long imm, char *lbl2);
BASM_API void BASM_OutOpRegMemImm(BASM_Context *ctx, int op, int reg,char *lbl, int breg, int ireg, int sc, long long disp, long long imm, char *lbl2);
BASM_API void BASM_OutOpMemRegImm(BASM_Context *ctx, int op, int reg,char *lbl, int breg, int ireg, int sc, long long disp, long long imm, char *lbl2);
BASM_API void BASM_OutOpRegRegReg(BASM_Context *ctx,int op, int r0, int r1, int r2);
BASM_API void BASM_OutOpRegMemReg(BASM_Context *ctx, int op, int reg,char *lbl, int breg, int ireg, int sc, long long disp, int reg2);
BASM_API void BASM_OutOpMemRegReg(BASM_Context *ctx, int op, int reg,char *lbl, int breg, int ireg, int sc, long long disp, int reg2);
BASM_API void BASM_OutOpGeneric1(BASM_Context *ctx, int op, int w,char *lbl, int breg, int ireg, int sc, long long disp);
BASM_API void BASM_OutOpGeneric2(BASM_Context *ctx, int op, int w,char *lbl0, int breg0, int ireg0, int sc0, long long disp0, char *lbl1, int breg1, int ireg1, int sc1, long long disp1);
BASM_API void BASM_OutOpGeneric3(BASM_Context *ctx, int op, int w,char *lbl0, int breg0, int ireg0, int sc0, long long disp0, char *lbl1, int breg1, int ireg1, int sc1, long long disp1, char *lbl2, int breg2, int ireg2, int sc2, long long disp2);
BASM_API void BASM_EmitLabelPos(BASM_Context *ctx, char *name, int pos);
BASM_API void BASM_EmitGotoPos(BASM_Context *ctx,char *name, int ty, int pos);
BASM_API void BASM_EmitLabel(BASM_Context *ctx, char *name);
BASM_API void BASM_EmitGoto(BASM_Context *ctx, char *name, int ty);
BASM_API void BASM_EmitConst(BASM_Context *ctx, char *name, long long val);
BASM_API int BASM_PredictPos(BASM_Context *ctx, char *name);
BASM_API int BASM_PredictDisp(BASM_Context *ctx, char *name);
//AHSRC:bsvm2/bgbasm3/asm/basm_parse.c
BASM_API char *BASM_ComposeLinkMetaName(char *name, char **args);
BASM_API char *BASM_ComposeLinkNotifyName(char *name, char **args);
BASM_API void BASM_RegisterSpecialOp(char *name,char *(*fcn)(BASM_Context *ctx, char *op, char **str));
char *basm_strdup_alloc(char *str);
BASM_API int basm_strdup_i(char *str);
BASM_API char *basm_strtab_i(int i);
BASM_API char *basm_strdup(char *str);
BASM_API int basm_strildup_i(char *str);
BASM_API void *basm_ralloc(int sz);
BASM_API char *basm_rstrdup(char *str);
BASM_API int basm_strisreal(char *str);
u32 basm_atof_u32(char *str);
u64 basm_atof_u64(char *str);
u32 basm_atof_u32_sg(char *str, int sg);
u64 basm_atof_u64_sg(char *str, int sg);
s64 basm_atoi(char *str);
int basm_stricmp(char *s1, char *s2);
BASM_API char *BASM_Parse_EatWhiteOnly(char *s);
BASM_API char *BASM_Parse_EatWhite(char *s);
BASM_API int BASM_Parse_IsWhiteOnly(char *s);
BASM_API int BASM_Parse_IsLineBreak(char *se);
BASM_API int BASM_Parse_OpChar(int c);
BASM_API int BASM_Parse_PeekChar(char **str);
BASM_API int BASM_Parse_ParseChar(char **str);
BASM_API int BASM_Parse_EmitChar(char **str, int j);
int BASM_Parse_MatchTokenListLen(char *str, char **lst);
BASM_API char *BASM_Parse_Token(char *s, char *b, int *ty);
char *BASM_Parse_TokenGAS(char *s, char *b, int *ty);
BASM_API int BASM_ParseReg(BASM_Context *ctx, char *b);
BASM_API int BASM_ParseReg(BASM_Context *ctx, char *b);
char *BASM_ParseArgGAS(BASM_Context *ctx, char *str,int *breg, int *ireg, int *sc, long long *disp, char **lbl);
BASM_API char *BASM_ParseArg(BASM_Context *ctx, char *str,int *breg, int *ireg, int *sc, long long *disp, char **lbl);
BASM_API int BASM_OpOnlySingleP(int op);
BASM_API char *BASM_ParseXMeta(BASM_Context *ctx, char *str,char *op, char *tag);
BASM_API char *BASM_ParseSpecial(BASM_Context *ctx, char *str, char *op);
BASM_API int BASM_LookupOpcode(BASM_Context *ctx, char *str);
int BASM_CheckARMCC(BASM_Context *ctx, char **str);
BASM_API int BASM_LookupOpcodeARMCC(BASM_Context *ctx, char *str);
char *BASM_ParseOpcodeInner(BASM_Context *ctx, char *str,char *opname, int opnum);
BASM_API char *BASM_ParseOpcode(BASM_Context *ctx, char *str);
BASM_API char *BASM_ParseOpcode(BASM_Context *ctx, char *str);
BASM_API char *BASM_ParseOpcodeList(BASM_Context *ctx, char *str);
//AHSRC:bsvm2/bgbasm3/asm/basm_insns_x86.c
//AHSRC:bsvm2/bgbasm3/asm/basm_insns_arm.c
//AHSRC:bsvm2/bgbasm3/asm/basm_insns_thumb.c
//AHSRC:bsvm2/bgbasm3/asm/basm_disasm.c
BASM_API char *BASM_GetPtrName(void *ptr);
int BDISASM_CheckOpStr(byte **rip, char **str, int *rfl, int op);
byte *BDISASM_PrintModRM_RM16(byte *ip, int op, int fl, int w, int ar);
byte *BDISASM_PrintModRM_RM(byte *ip, int op, int fl, int w, int ar);
byte *BDISASM_PrintModRM_RegRM(byte *ip, int op, int fl, int w);
byte *BDISASM_PrintModRM_RMReg(byte *ip, int op, int fl, int w);
byte *BDISASM_SkipModRM(byte *ip, int fl);
int BDISASM_SizeOpStr(byte *oip, char *str, int fl);
char *BDISASM_GetFixReg(int i, int j);
void BDISASM_PrintFixRegPF(int i);
void BDISASM_PrintFixRegSF(int i);
BASM_API byte *BDISASM_PrintOpcode(byte *ip, int *rfl);
BASM_API int BDISASM_PrintOpcodes(byte *ip, int fl, int sz);
BASM_API int BDISASM_HexDump(byte *ip, int sz);
//AHSRC:bsvm2/bgbasm3/asm/basm_coff.c
int BASM_COFF_ResolveRelocs(BASM_Context *ctx, BASM_COFF_Info *inf, int sec,int offs, int cnt);
BASM_Context *BASM_COFF_LoadObjectBuf(char *name, byte *buf, int sz);
BASM_Context *BASM_COFF_LoadObject(char *name);
int BASM_COFF_LoadLibrary(char *name);
void basm_coff_set16(byte *ptr, int i);
void basm_coff_set32(byte *ptr, int i);
int basm_coff_get16(byte *ptr);
int basm_coff_get32(byte *ptr);
int basm_coff_idxstr(char *strs, char *str);
int basm_coff_idxstr2(char *strs, char *str, int fl);
BASM_API int BASM_COFF_StoreObjectBuf(BASM_Context *ctx,byte *buf, int msz);
int BASM_COFF_StoreObject(char *name, BASM_Context *ctx);
BASM_API byte *BASM_COFF_EncodeObject(BASM_Context *ctx, int *rsz);
//AHSRC:bsvm2/bgbasm3/link/link_context.c
BLNK_Object *BLNK_NewContext();
int BLNK_DestroyContext(BLNK_Object *ctx);
void BLNK_EmitLabelPos(BLNK_Object *ctx, char *name, int pos);
void BLNK_EmitGotoPos(BLNK_Object *ctx, char *name, int ty, int pos);
//AHSRC:bsvm2/bgbasm3/link/link_core.c
BASM_API BLNK_GCFuncs_t *BLNK_GetGCFuncs();
BASM_API void BLNK_SetScanRange(int (*fcn)(void **p, int cnt));
void BLNK_ScanRange(void **p, int cnt);
void *BLNK_gcAlloc(int sz);
void *BLNK_gcExec(int sz);
void *BLNK_gcAtomic(int sz);
void *BLNK_gcTAlloc(char *ty, int sz);
void *BLNK_gcTExec(char *ty, int sz);
void *BLNK_gcTAtomic(char *ty, int sz);
void BLNK_gcFree(void *obj);
void *BLNK_gcReAlloc(void *obj, int sz);
int BLNK_gcAllocTls(char *name, int sz);
int BLNK_gcGetTlsTls();
BASM_API int BLNK_RegisterLookup(blnk_lookup_ft fcn);
BASM_API int BLNK_RegisterNotify(blnk_notify_ft fcn);
int BLNK_ObjAllocTextP(byte *obj);
void BLNK_AddExeSection(char *name, s64 va, s64 sz, int fl);
char *BLNK_LookupExeSectionPtrName(void *ptr);
char *BLNK_LookupModulePtrName(void *ptr);
BASM_API void BLNK_MarkData();
BASM_API int BLNK_CheckPointerImageP(void *p);
byte *BLNK_AllocChunkBuffer(int sz);
void BLNK_AllocTextChunk(int sz);
void BLNK_AllocDataChunk(int sz);
byte *BLNK_AllocTextSz(int sz);
byte *BLNK_AllocDataSz(int sz);
void BLNK_InitLink();
char *blnk_strdup(char *str);
int blnk_strdup_i(char *str);
char *blnk_strtab_i(int str);
char *blnk_strdup(char *str);
int blnk_strdup_i(char *str);
char *blnk_strtab_i(int str);
char *blnk_blacklist(char *str);
int blnk_blacklist_i(char *str);
char *blnk_blakcktab_i(int str);
int blnk_lookup_blacklist(char *str);
void BLNK_LinkReference(byte *ref, byte *ptr, int ty);
void BLNK_UnlinkReference(byte *ref, byte *ptr, int ty);
void BLNK_RelinkReference(byte *ref, byte *oldptr, byte *newptr, int ty);
void BLNK_LinkRefChain(int idx, byte *ptr);
void BLNK_UnlinkRefChain(int idx, byte *ptr);
void BLNK_RelinkRefChain(int idx, byte *oldptr, byte *newptr);
void BLNK_AddSymRef(char *name, byte *ref, int ty);
char *BLNK_UnmangleName(char *str);
void BLNK_AddSym(char *name, void *ptr);
void BLNK_AddSym(char *name, void *ptr);
void *BLNK_FetchSymHash(char *name);
void *BLNK_FetchSym(char *name);
void *BLNK_FetchSymPrefix(char *name);
int BLNK_FetchSymIndexPrefix(char *name);
char *BLNK_FetchSymPrefixName(char *name);
int BLNK_FetchSymIndex2(char *name);
int BLNK_FetchSymIndex(char *name);
int BLNK_GetSymIndex(char *name);
int BLNK_QuickHashName(char *name);
int BLNK_LookupQuickHash(char *name, void **rptr);
int BLNK_AssignQuickHash(char *name, void *ptr);
int BLNK_FlushQuickHash(void);
void *BLNK_LookupLabel(char *name);
int BLNK_CheckPtrBigSpace(void *ptr);
BASM_API void BLNK_AssignSym(char *name, void *ptr);
BASM_API void BLNK_AddProxyPtr(char *name, void *ptr);
BASM_API void BLNK_AddProxy(char *name);
BASM_API void BLNK_AddProxyImport(char *name);
byte *BLNK_LinkModule(BLNK_Object *ctx);
byte *BLNK_LinkModuleObj(char *name, void *buf, int sz);
void BLNK_LinkModuleQueue(BLNK_Object *ctx);
void BLNK_LinkModuleObjQueue(char *name, void *buf, int sz);
void BLNK_UnlinkModuleQueue(BLNK_Object *ctx);
BLNK_Object *BLNK_LookupSymbolQueue(char *sym);
void *BLNK_ResolveSymbolQueue(char *sym);
int BLNK_LookupModule(char *name);
void *BLNK_LookupModuleText(char *name, int *rsz);
void *BLNK_LookupModuleData(char *name, int *rsz);
char *BLNK_GetNamePtr(void *ptr);
char *BLNK_GetLastNamePtr(void *ptr, void **rbp);
int BLNK_EnumUndefSyms(char **lst, int max);
void BLNK_AddSymArray(char **name, void **ptr, int cnt);
BASM_API int BLNK_LoadLibrary(char *name);
//AHSRC:bsvm2/bgbasm3/link/link_thunk.c
BASM_API BLNK_Scope *BLNK_NewScope(BLNK_Scope *super);
BASM_API void BLNK_FreeScope(BLNK_Scope *scope);
BASM_API int BLNK_IndexScopeSym(BLNK_Scope *scope, char *name);
BASM_API void BLNK_BindScopeSym(BLNK_Scope *scope, char *name, void *ptr);
BASM_API void BLNK_BindScopeSymSig(BLNK_Scope *scope,char *name, char *sig, void *ptr);
BASM_API void BLNK_AssignScopeSym(BLNK_Scope *scope, char *name, void *ptr);
BASM_API void *BLNK_FetchScopeSym(BLNK_Scope *scope, char *name);
BASM_API void *BLNK_FetchScopeSym2(BLNK_Scope *scope, char *name);
BASM_API char *BLNK_FetchScopeSig(BLNK_Scope *scope, char *name);
BASM_API BLNK_Thunk *BLNK_LinkModuleThunk(BLNK_Object *ctx, BLNK_Scope *scope);
BASM_API byte *BLNK_LinkThunkInline(BLNK_Object *ctx, BLNK_Scope *scope);
BASM_API BLNK_Thunk *BLNK_LinkModuleThunkObj(BLNK_Scope *scope, void *buf, int sz);
BASM_API byte *BLNK_LinkModuleThunkInlineObj(BLNK_Scope *scope, void *buf, int sz);
//AHSRC:bsvm2/bgbasm3/link/link_lnx.c
int BLNK_InitDllsOS();
BASM_API int BLNK_GetModuleListOS(char **buf, int max);
BASM_API char **BLNK_GetEnvironOS();
char *BLNK_LookupAddrNameOS(void *addr, void **addr2);
int BLNK_LoadDynamicOS(char *name);
void BLNK_UpdateProxyOS(char *name, void *ptr);
void *BLNK_LookupLabelOS(char *name);
int BLNK_ProcessEXE(char *name);
void *BLNK_LoadFileRawOS(char *name, int *rsz);
BASM_API void *BLNK_LoadModuleFileDataOS(char *name, int *rsz);
BASM_API void *BLNK_LoadModuleWadDataOS(char *name, int *rsz);
BASM_API void BLNK_FreeModuleDataOS(void *buf);
//AHSRC:bsvm2/bgbasm3/link/link_w32.c
BASM_API int BLNK_GetModuleListOS(char **buf, int max);
BASM_API char **BLNK_GetEnvironOS();
void BLNK_ProbeTst(byte *buf, int sz);
void BLNK_UpdateDllsOS(void *hmod);
int BLNK_InitDllsOS();
int BLNK_LoadDynamicOS(char *name);
BASM_API void *BLNK_LoadModuleFileDataOS(char *name, int *rsz);
BASM_API void *BLNK_LoadModuleWadDataOS(char *name, int *rsz);
BASM_API void BLNK_FreeModuleDataOS(void *buf);
void BLNK_UpdateProxyOS(char *name, void *ptr);
void *BLNK_LookupLabelOS_1(char *name);
void *BLNK_LookupLabelOS_2(char *name, int *rfl);
void *BLNK_LookupLabelOS(char *name);
int BLNK_InitDbgHelpOS();
char *BLNK_LookupAddrNameOS(void *addr, void **addr2);
int BLNK_ProcessMAP(char *name);
int BLNK_ProcessEXE(char *name);
//AHSRC:bsvm2/bgbasm3/link/link_vfs.c
void *blnk_fopen_tmp(char *name, char *mode);
s64 blnk_tell_tmp(void *fd);
int blnk_seek_tmp(void *fd, s64 pos, int rel);
BLNK_IOFuncs_t *blnk_getio();
BASM_API BLNK_IOFuncs_t *BLNK_GetIOFuncs();
void blnk_initvfs();
void *blnk_fopen(char *name, char *mode);
void blnk_fclose(void *fd);
int blnk_fread(void *buf, int m, int n, void *fd);
int blnk_fwrite(void *buf, int m, int n, void *fd);
int blnk_feof(void *fd);
s64 blnk_ftell(void *fd);
int blnk_fseek(void *fd, s64 pos, int rel);
int blnk_fgetc(void *fd);
void blnk_fputc(int c, void *fd);
void *blnk_loadfile(char *name, int *rsz);
int blnk_storefile(char *name, void *buf, int sz);
int blnk_storetextfile(char *name, char *buf);
//AHSRC:bsvm2/bgbasm3/link/bldr_coff.c
int BLNK_COFF_ResolveRelocs(BLNK_Object *ctx, BLNK_COFF_Info *inf, int sec,int offs, int cnt);
BLNK_Object *BLNK_COFF_LoadObjectBuf(char *name, byte *buf, int sz);
BLNK_Object *BLNK_COFF_LoadObject(char *name);
int BLNK_COFF_LoadLibraryBuf(char *name, byte *buf, int sz);
int BLNK_COFF_LoadLibrary(char *name);
int BLNK_COFF_LoadLibrary(char *name);
void blnk_coff_set16(byte *ptr, int i);
void blnk_coff_set32(byte *ptr, int i);
int blnk_coff_get16(byte *ptr);
int blnk_coff_get32(byte *ptr);
int blnk_coff_idxstr(char *strs, char *str);
int blnk_coff_idxstr2(char *strs, char *str, int fl);
int BLNK_COFF_StoreObjectBuf(BLNK_Object *ctx, byte *buf, int msz);
int BLNK_COFF_StoreObject(char *name, BLNK_Object *ctx);
byte *BLNK_COFF_EncodeObject(BLNK_Object *ctx, int *rsz);
//AHSRC:bsvm2/bgbasm3/link/bldr_elf.c
BLNK_Object *BLNK_ELF_LoadObjectBuf(char *name, byte *buf, int sz);
