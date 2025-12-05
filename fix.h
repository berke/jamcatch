/* fix.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef FIX_H
#define FIX_H

#include "types.h"
#include "config.h"

typedef s32 fix_t;
typedef s64 long_fix_t;

#define FIX2 (2 * FIX)

#define F(x) ((fix_t)((1 << FIX) * (double)(x)))
#define DOUBLE(x) (((double) (x)) / ((double) (1 << FIX)))
#define DOUBLE2(x) (((double) (x)) / ((double) (1LL << (2 * FIX))))

static inline fix_t mul(fix_t x, fix_t y)/*{{{*/
{
  long_fix_t t;

  t = ((long_fix_t) x * (long_fix_t) y) >> FIX;

  return (fix_t) t;
}/*}}}*/
static inline fix_t s16_to_fix(s16 x)/*{{{*/
{
  return x << FIX;
}/*}}}*/
static inline fix_t inv(fix_t x)/*{{{*/
{
  return (1 << FIX) / x;
}/*}}}*/
static inline fix_t div(fix_t x, fix_t y)/*{{{*/
{
  long_fix_t t;

  t = (long_fix_t) x;
  t <<= FIX;
  t /= y;

  return (fix_t) t;
}/*}}}*/

#endif
