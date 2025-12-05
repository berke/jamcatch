/* random.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <debug.h>
#include <random.h>

const u32 random_key[] = {
  0xf8eaf9c7, 0x23049940, 0x12319845, 0x50483458,
  0x93568380, 0x11032486, 0x99418384, 0x50385789
};

void random_init(random_t *rnd)/*{{{*/
{
  (*rnd)[0] = 0x31415926;
  (*rnd)[1] = 0xfeafbfae;
}/*}}}*/
void random_from_seed(random_t *rnd, u32 s1, u32 s2)/*{{{*/
{
  (*rnd)[0] = 0x31415926 ^ s1;
  (*rnd)[1] = 0xfeafbfae ^ s2;
}/*}}}*/
u32 random_get(random_t *rnd)/*{{{*/
{
  u32 v0, v1;
  u32 sum, delta;
  u8 i;

  v0 = (*rnd)[0];
  v1 = (*rnd)[1];
  sum = 0xc6ef3720;
  delta = 0x9e3779b9;

  for(i=0; i<4 /* 32 */; i++) {
    v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + random_key[(sum>>11) & 3]);
    sum -= delta;
    v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + random_key[sum&3]);
  }

  (*rnd)[0] = v0;
  (*rnd)[1] = v1;

  XDEBUGF("Random %x", v0);
  return v0;
}/*}}}*/
