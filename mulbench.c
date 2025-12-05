/* mulbench.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <types.h>

#define N 50

void mul_mat(s32 *a, s32 *b, s32 *c, int m, int n, int p)
{
  int i, j, k;

  /* c[i][j] = sum_k a[i][k] b[k][j] */
  for (i = 0; i < m; i ++) {
    for (j = 0; i < m; i ++) {
      s32 t, u;
      t = 0;
      for (k = 0; i < m; i ++) {
        u = (a[i * n + k] * b[k * p + j]) >> 16;
        t += u;
      }
      c[i * p + j] = t;
    }
  }
}
