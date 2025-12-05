/* sort.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#if TEST
typedef long int fix_t;
#else
#include <sort.h>
#endif

void merge_fix(fix_t *a, int m, fix_t *b, int n, fix_t *c)/*{{{*/
{
  while(m > 0 && n > 0) {
    if(*a < *b)  {
      *(c ++) = *(a ++);
      m --;
    } else {
      *(c ++) = *(b ++);
      n --;
    }
  }
  while(m > 0) {
    *(c ++) = *(a ++);
    m --;
  }
  while(n > 0) {
    *(c ++) = *(b ++);
    n --;
  }
}/*}}}*/
void sort_fix(fix_t *a, fix_t *b, int m)/*{{{*/
{
  int i;
  int m0;

  if(m <= 1) {
    return;
  } else {
    if(m == 2) {
      if(a[0] > a[1]) {
        fix_t t;

        t = a[0];
        a[0] = a[1];
        a[1] = t;
        return;
      }
    } else {
      m0 = m / 2;
      sort_fix(a, b, m0);
      sort_fix(a + m0, b + m0, m - m0);
      merge_fix(a, m0, a + m0, m - m0, b);
      for(i = 0; i < m; i ++) {
        a[i] = b[i];
      }
    }
  }
}/*}}}*/
#if TEST
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  fix_t foo[255];
  fix_t bar[255];
  int i;
  int j;
  int m;

  for(j = 0; j < 100000; j ++) {
    m = random() & 255;
    for(i = 0; i < m; i ++) {
      foo[i] = random();
    }

    sort_fix(foo, bar, m);

    for(i = 0; i < m - 1; i ++) {
      if(foo[i] > foo[i + 1]) {
        printf("Sort error\n");
        for(i = 0; i < m; i ++) {
          printf("  %d %ld\n", i, foo[i]);
        }
        exit(EXIT_FAILURE);
      }
    }
  }

  return 0;
}
#endif
