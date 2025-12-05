/* math.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 *
 */

#include <types.h>
#include <debug.h>

u32 __udivsi3(u32 x, u32 y)/*{{{*/
{
  u32 z;
  u32 a;

  if(!y) return 0;
  if(y > x) return 0;

  z = 0;
  a = 1;
  while(!(y & 0x80000000) && (y << 1) <= x) {
    y <<= 1;
    a <<= 1;
  }
  while(a) {
    while(y <= x) {
      x -= y;
      z += a;
    }
    a >>= 1;
    y >>= 1;
  }
  return z;
}/*}}}*/
s32 __divsi3(s32 x, s32 y)/*{{{*/
{
  if(!y) return 0;
  if(x < 0) {
    if(y < 0) {
      return __udivsi3((u32) -x, (u32) -y);
    } else {
      return -__udivsi3((u32) -x, (u32) y);
    }
  } else {
    if(y < 0) {
      return -__udivsi3((u32) x, (u32) -y);
    } else {
      return __udivsi3((u32) x, (u32) y);
    }
  }
}
/*}}}*/
u64 __udivdi3(u64 x, u64 y)/*{{{*/
{
  u64 z;
  u64 a;

  if(!y) return 0;
  if(y > x) return 0;

  z = 0;
  a = 1;
  while((y << 1) <= x) {
    //printf("A y=%08x a=%08x z=%08x\n", y, a, z);
    y <<= 1;
    a <<= 1;
  }
  while(a) {
    while(y <= x) {
      //printf("B y=%08x a=%08x z=%08x\n", y, a, z);
      x -= y;
      z += a;
    }
    a >>= 1;
    y >>= 1;
    //printf("C y=%08x a=%08x z=%08x\n", y, a, z);
  }
  return z;
}/*}}}*/
s64 __divdi3(s64 x, s64 y)/*{{{*/
{
  if(!y) return 0;
  if(x < 0) {
    if(y < 0) {
      return __udivdi3((u32) -x, (u32) -y);
    } else {
      return -__udivdi3((u32) -x, (u32) y);
    }
  } else {
    if(y < 0) {
      return -__udivdi3((u32) x, (u32) -y);
    } else {
      return __udivdi3((u32) x, (u32) y);
    }
  }
}
/*}}}*/
#if 0
#include <stdio.h>

int main(int argc, char **argv)
{
  u32 i, j, k, l;
  u32 x, y;

  x = 3543348019U;
  y = 42068;
  printf("foo %u / %u = %u\n", x, y, __udivsi3(x,y));

#if 1
  for(i = 0; i < 1000; i ++) {
    for(j = 0; j < 1000; j ++) {
      if(j) {
        k = i / j;
        l = __udivsi3(i, j);
        if(k != l) {
          printf("%d / %d = %d (%d)\n", i, j, k, l);
        }
      }
    }
  }
#endif
  return 0;
}
#endif

u32 div_u32_u32(u32 x, u32 y)/*{{{*/
{
  return x / y;
}
u32 div_s32_s32(s32 x, s32 y)/*{{{*/
{
  return x / y;
}
