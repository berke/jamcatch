/* strings.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <types.h>

int strings_equal(char *u, int m, char *v, int n)/*{{{*/
{
  if(m != n) {
    return 0;
  } else {
    int i;
    for(i = 0; i < m; i++) {
      if(u[i] != v[i]) return 0;
    }
    return 1;
  }
}/*}}}*/
int string_length(const char *u)/*{{{*/
{
  int m;

  m = 0;

  while(*u) {
    m ++;
    u ++;
  }

  return m;
}/*}}}*/
int ntstrings_equal(const char *u, const char *v)/*{{{*/
{
  for(;;) {
    if(!*u || !*v || *u != *v) break;
    u ++;
    v ++;
  }
  return *u == *v;
}/*}}}*/
void byte_copy(u8 *src, u8 *dst, int m)/*{{{*/
{
  while(m --) {
    *(dst ++) = *(src ++);
  }
}/*}}}*/
bool word_compare(void *xx, void *yy, int m)/*{{{*/
{
  u32 *x, *y;

  x = (u32 *) xx;
  y = (u32 *) yy;
  m /= sizeof(u32);

  while(m --) {
    if(*(x ++) != *(y ++)) return false;
  }
  return true;
}/*}}}*/
void word_copy(void *src, void *dst, int m)/*{{{*/
{
  u32 *s, *d;
  u8 *sb, *db;

  s = (u32 *) src;
  d = (u32 *) dst;

  while(m >= sizeof(u32)) {
    *(d ++) = *(s ++);
    m -= sizeof(u32);
  }

  sb = (u8 *) s;
  db = (u8 *) d;

  while(m --) {
    *(db ++) = *(sb ++);
  }
}/*}}}*/
void word_zero(void *dst, int m)/*{{{*/
{
  u32 *d;

  d = (u32 *) dst;
  m /= sizeof(u32);

  while(m--) {
    *(d ++) = 0;
  }
}/*}}}*/
