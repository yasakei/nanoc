#ifndef _STDINT_H
#define _STDINT_H

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;
typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;

typedef int8_t int_fast8_t;
typedef uint8_t uint_fast8_t;
typedef int16_t int_fast16_t;
typedef uint16_t uint_fast16_t;
typedef int32_t int_fast32_t;
typedef uint32_t uint_fast32_t;
typedef int64_t int_fast64_t;
typedef uint64_t uint_fast64_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;
typedef long intmax_t;
typedef unsigned long uintmax_t;

#define INT8_MIN (-128)
#define INT8_MAX 127
#define UINT8_MAX ((uint8_t)~(uint8_t)0)
#define INT16_MIN (-32768)
#define INT16_MAX 32767
#define UINT16_MAX ((uint16_t)~(uint16_t)0)
#define INT32_MIN (-2147483647-1)
#define INT32_MAX 2147483647
#define UINT32_MAX ((uint32_t)~(uint32_t)0)
#define INT64_MIN (-((int64_t)1 << 63))
#define INT64_MAX ((int64_t)(((uint64_t)~(uint64_t)0) >> 1))
#define UINT64_MAX ((uint64_t)~(uint64_t)0)
#define INTPTR_MIN (-((intptr_t)1 << 63))
#define INTPTR_MAX ((intptr_t)(((uintptr_t)~(uintptr_t)0) >> 1))
#define UINTPTR_MAX ((uintptr_t)~(uintptr_t)0)
#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX
#define SIZE_MAX UINTPTR_MAX

#define INT8_C(value) value
#define UINT8_C(value) value##U
#define INT16_C(value) value
#define UINT16_C(value) value##U
#define INT32_C(value) value
#define UINT32_C(value) value##U
#define INT64_C(value) value##L
#define UINT64_C(value) value##UL
#define INTMAX_C(value) value##L
#define UINTMAX_C(value) value##UL

#endif
