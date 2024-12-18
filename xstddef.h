/* xstddef.h - basic type definitions */
#ifndef SL_XSTDDEF_H
#define SL_XSTDDEF_H

#include "config.h"


/* int8 */
#undef int8
#undef i8
typedef signed char int8;
typedef int8 i8;

/* int8 size */
#undef INT8_MAX
#define INT8_MAX (0x7f)

/* uint8 */
#undef uint8
#undef u8
typedef unsigned char uint8;
typedef uint8 u8;

/* uint8 size */
#undef UINT8_MAX
#define UINT8_MAX (0xff)

/* int16 */
#undef int16
#undef i16
typedef signed short int16;
typedef int16 i16;

/* in16 size */
#undef INT16_MAX
#define INT16_MAX (0x7fff)

/* uint16 */
#undef uint16
#undef u16
typedef unsigned short uint16;
typedef uint16 u16;

/* uin16 size */
#undef UINT16_MAX
#define UINT16_MAX (0xffff)

/* int32 */
#undef int32
#undef i32
typedef signed int int32;
typedef int32 i32;

/* int32 size */
#undef INT32_MAX
#define INT32_MAX (0x7fffffff)

/* uint32 */
#undef uint32
#undef u32
typedef unsigned int uint32;
typedef uint32 u32;

/* uint32 size */
#undef UINT32_MAX
#define UINT32_MAX (0xffffffff)

/* int64 */
#undef int64
#undef i64
typedef signed long int int64;
typedef int64 i64;

/* int64 size */
#undef INT64_MAX
#ifdef MARCH_BIT
#	if (MARCH_BIT == MARCH_BIT_32)
#		define INT64_MAX (0x7fffffff)
#	elif (MARCH_BIT == MARCH_BIT_64)
#		define INT64_MAX (0x7fffffffffffffff)
#	else
		!!!unknown MARCH_BIT!!!
#	endif
#else
	!!!undefined MARCH_BIT!!!
#endif

/* uint64 */
#undef uint64
#undef u64
typedef unsigned long int uint64;
typedef uint64 u64;

/* int64 size */
#undef UINT64_MAX
#ifdef MARCH_BIT
#	if (MARCH_BIT == MARCH_BIT_32)
#		define UINT64_MAX (0xffffffff)
#	elif (MARCH_BIT == MARCH_BIT_64)
#		define UINT64_MAX (0xffffffffffffffff)
#	else
		!!!unknown MARCH_BIT!!!
#	endif
#else
	!!!undefined MARCH_BIT!!!
#endif

/* int64L */
#undef int64L
#undef i64L
typedef signed long long int64L;
typedef int64L i64L;

/* int64L size */
#undef INT64L_MAX
#define INT64L_MAX (0x7fffffffffffffff)

/* uint64L */
#undef uint64L
#undef u64L
typedef unsigned long long uint64L;
typedef uint64L u64L;

/* uint64L size */
#undef UINT64L_MAX
#define UINT64L_MAX (0xffffffffffffffff)

#undef NULL
#define NULL ((void *)0)

#undef offsetof
#define offsetof(type, member) \
	((uint64)&(((type *)0)->member))

#undef container_of
#define container_of(ptr, type, member) \
	((type *)(((void *)ptr) - offsetof(type, member)))


#endif
