#if defined(__x86_64__) || defined(_M_X64)
#ifndef X86_64
#define X86_64
#endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#ifndef X86
#define X86
#endif
#endif

#if defined(__arm__) || defined(_M_ARM)
#ifndef ARM
#define ARM
#endif
#endif


#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif

#ifdef _WIN64
#ifndef WIN64
#define WIN64
#endif
#endif

#ifdef _MSC_VER
#ifndef MSVC
#define MSVC
#endif
#endif

#ifdef __ANDROID__
#ifndef ANDROID
#define ANDROID
#endif
#endif

#ifdef __linux__
#ifndef linux
#define linux
#endif
#endif

#ifdef BTEIFGL_DRV
#define BTEIFGL_API
#else

#ifdef WIN32
#ifdef BTEIFGL_DLL
#define BTEIFGL_API __declspec(dllexport)
#else
#define BTEIFGL_API __declspec(dllimport)
#endif
#else
#define BTEIFGL_API
#endif

#endif

//HACK: for now assume, if Linux and ARM, we are on a Raspberry Pi
#if defined(__linux__) && defined(__arm__) && !defined(ANDROID)

#ifndef RASPBERRYPI
#define RASPBERRYPI
#endif

#endif

#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
#endif

#ifndef __cplusplus

#ifndef BOOL_T
#define BOOL_T
typedef unsigned char bool;
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#endif


#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif

#if !defined(LITTLEENDIAN) && !defined(BIGENDIAN)
#if defined(X86) || defined(X86_64) || defined(ARM)
#define LITTLEENDIAN
#endif
#endif

#ifndef F32_T
#define F32_T
typedef float f32;
#endif

#ifndef F64_T
#define F64_T
typedef double f64;
#endif


#ifndef static_inline

#ifdef _MSC_VER
#define static_inline static __inline
#define static_forceinline static __forceinline
#define default_inline __inline
#define force_inline __forceinline
#endif

#ifdef __GNUC__
#define static_inline static inline
#define static_forceinline static inline
#define default_inline inline
#define force_inline inline
#endif

#ifndef static_inline
#define static_inline
#define static_forceinline
#define default_inline
#define force_inline
#endif

#endif
