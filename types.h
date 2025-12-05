/* types.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef TYPES_H
#define TYPES_H

typedef enum { false, true } bool;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef signed char      s8;
typedef signed short     s16;
typedef signed int       s32;
typedef signed long long s64;

typedef volatile u8 *h8;
typedef volatile u16 *h16;
typedef volatile u32 *h32;

typedef const char *string;

#define length(x) (sizeof(x)/sizeof(*(x)))

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
typedef __builtin_va_list va_list;

#endif
