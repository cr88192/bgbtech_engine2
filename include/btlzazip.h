#ifndef BTLZAZIP_H
#define BTLZAZIP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include <btlzazip_conf.h>

#define BGBBTJ_ZFL_ESCAPE_FF	1
#define BGBBTJ_ZFL_DEFLATE64	2
#define BGBBTJ_ZFL_ESCBTLZA		4
#define BGBBTJ_ZFL_ESCBTLZH		8
#define BGBBTJ_ZFL_ISBTLZH		16
#define BGBBTJ_ZFL_ARITH		32
#define BGBBTJ_ZFL_FASTENC		64
#define BGBBTJ_ZFL_RINGHUFF		128
#define BGBBTJ_ZFL_PRELOAD		256

#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif

#if defined(X86)||defined(X86_64)||defined(ARM)
#define btlza_getu16le(ptr)		(*(u16 *)(ptr))
#define btlza_getu32le(ptr)		(*(u32 *)(ptr))
#define btlza_getu64le(ptr)		(*(u64 *)(ptr))
#define btlza_gets16le(ptr)		(*(s16 *)(ptr))
#define btlza_gets32le(ptr)		(*(s32 *)(ptr))
#define btlza_gets64le(ptr)		(*(s64 *)(ptr))
#define btlza_setu16le(ptr, val)	(*(u16 *)(ptr))=(val)
#define btlza_setu32le(ptr, val)	(*(u32 *)(ptr))=(val)
#define btlza_setu64le(ptr, val)	(*(u64 *)(ptr))=(val)
#define btlza_sets16le(ptr, val)	(*(s16 *)(ptr))=(val)
#define btlza_sets32le(ptr, val)	(*(s32 *)(ptr))=(val)
#define btlza_sets64le(ptr, val)	(*(s64 *)(ptr))=(val)
#else
default_inline u16 btlza_getu16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
default_inline u32 btlza_getu32le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
default_inline u64 btlza_getu64le(byte *ptr)
	{ return(btlza_getu32le(ptr)|(((u64)btlza_getu32le(ptr+4))<<32)); }

default_inline s16 btlza_gets16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
default_inline s32 btlza_gets32le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
default_inline s64 btlza_gets64le(byte *ptr)
	{ return(btlza_getu32le(ptr)|(((s64)btlza_gets32le(ptr+4))<<32)); }

default_inline void btlza_setu16le(byte *ptr, u16 val)
	{ ptr[0]=val; ptr[1]=val>>8; }
default_inline void btlza_setu32le(byte *ptr, u32 val)
	{ ptr[0]=val; ptr[1]=val>>8; ptr[2]=val>>16; ptr[3]=val>>24; }
default_inline void btlza_setu64le(byte *ptr, u64 val)
	{ btlza_setu32le(ptr, val); btlza_setu32le(ptr+4, val>>32); }

default_inline void btlza_sets16le(byte *ptr, s16 val)
	{ btlza_setu16le(ptr, (u16)val); }
default_inline void btlza_sets32le(byte *ptr, s32 val)
	{ btlza_setu32le(ptr, (u32)val); }
default_inline void btlza_sets64le(byte *ptr, s64 val)
	{ btlza_setu64le(ptr, (u64)val); }
#endif

#include <btlzazip_btlza.h>
// #include <btlzazip_auto.h>

#endif
