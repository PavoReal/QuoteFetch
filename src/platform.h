#pragma once

#include <stdint.h>

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define INTERNAL static
#define GLOBAL static

#define BYTES(a) (a)
#define KILOBYTES(a) (BYTES(a) * 1024)
#define MEGABYTES(a) (KILOBYTES(a) * 1024)
#define GIGABYTES(a) (MEGABYTES(a) * 1024)

#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(a[0]))

#define UNUSED(a) (void) a

