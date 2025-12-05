/* fft.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 *
 */

#include <fft.h>

static inline void swap(fix_t *x, fix_t i, fix_t j)
{
  fix_t t;

  t = x[i];
  x[i] = x[j];
  x[j] = t;
}

static void fft_bit_reverse(fix_t *x, fix_t m)
{
  fix_t i, j;
  fix_t mask;

  i = 0;
  j = 0;

  for(;;) {
    if(i == m) break;
    if(i < j) {
      swap(x, 2 * i, 2 * j);
      swap(x, 2 * i + 1, 2 * j + 1);
    }

    i ++;
    mask = m >> 1;
    while(mask) {
      if(!(j & mask)) break;
      j ^= mask;
      mask >>= 1;
    }
    j ^= mask;
  }
}

void fft(fix_t *x)
{
  fft_bit_reverse(x, FFT_CORE_N);
  fft_core(x);
}
