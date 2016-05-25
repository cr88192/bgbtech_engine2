#define BTEIFGL_MAIN

#ifndef MAIN_EXPORT
#ifdef _MSC_VER
#define MAIN_EXPORT __declspec(dllexport)
#else
#define MAIN_EXPORT
#endif
#endif

#include <bteifgl.h>

#ifdef _WIN32
#include <bteifgl_main_w32.h>
#endif

#ifdef linux
#include <bteifgl_main_lnx.h>
#endif
