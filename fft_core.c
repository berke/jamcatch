/* fft_core.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved.
 */

#include <fft_core.h>

static const fix_t w_2[2] = {
  16777216, 0,
};

static const fix_t w_4[4] = {
  16777216, 0,
  0, -16777216,
};

static const fix_t w_8[8] = {
  16777216, 0,
  11863283, -11863283,
  0, -16777216,
  -11863283, -11863283,
};

static const fix_t w_16[16] = {
  16777216, 0,
  15500126, -6420362,
  11863283, -11863283,
  6420362, -15500126,
  0, -16777216,
  -6420362, -15500126,
  -11863283, -11863283,
  -15500126, -6420362,
};

static const fix_t w_32[32] = {
  16777216, 0,
  16454846, -3273072,
  15500126, -6420362,
  13949745, -9320921,
  11863283, -11863283,
  9320921, -13949745,
  6420362, -15500126,
  3273072, -16454846,
  0, -16777216,
  -3273072, -16454846,
  -6420362, -15500126,
  -9320921, -13949745,
  -11863283, -11863283,
  -13949745, -9320921,
  -15500126, -6420362,
  -16454846, -3273072,
};

static const fix_t w_64[64] = {
  16777216, 0,
  16696429, -1644454,
  16454846, -3273072,
  16054794, -4870168,
  15500126, -6420362,
  14796183, -7908724,
  13949745, -9320921,
  12968963, -10643353,
  11863283, -11863283,
  10643353, -12968963,
  9320921, -13949745,
  7908724, -14796183,
  6420362, -15500126,
  4870168, -16054794,
  3273072, -16454846,
  1644454, -16696429,
  0, -16777216,
  -1644454, -16696429,
  -3273072, -16454846,
  -4870168, -16054794,
  -6420362, -15500126,
  -7908724, -14796183,
  -9320921, -13949745,
  -10643353, -12968963,
  -11863283, -11863283,
  -12968963, -10643353,
  -13949745, -9320921,
  -14796183, -7908724,
  -15500126, -6420362,
  -16054794, -4870168,
  -16454846, -3273072,
  -16696429, -1644454,
};

void fft_core(fix_t *x) { /* 64 */
  int i, j;
  fix_t *ap, *bp, ar, ai, br, bi, wr, wi, tr, ti;

  ap = x;
  bp = x + 2;
  for(i = 0; i < 32; i ++) {
    for(j = 0; j < 1; j ++) {
      ar = ap[0];
      ai = ap[1];
      br = bp[0];
      bi = bp[1];
      wr = w_2[2 * j];
      wi = w_2[2 * j + 1];
      tr = ar + mul(br, wr) - mul(bi, wi);
      ti = ai + mul(bi, wr) + mul(br, wi);
      ap[0] = tr;
      ap[1] = ti;
      tr = ar - mul(br, wr) + mul(bi, wi);
      ti = ai - mul(bi, wr) - mul(br, wi);
      bp[0] = tr;
      bp[1] = ti;
      ap += 2;
      bp += 2;
    }
    ap += 2;
    bp += 2;
  }
  ap = x;
  bp = x + 4;
  for(i = 0; i < 16; i ++) {
    for(j = 0; j < 2; j ++) {
      ar = ap[0];
      ai = ap[1];
      br = bp[0];
      bi = bp[1];
      wr = w_4[2 * j];
      wi = w_4[2 * j + 1];
      tr = ar + mul(br, wr) - mul(bi, wi);
      ti = ai + mul(bi, wr) + mul(br, wi);
      ap[0] = tr;
      ap[1] = ti;
      tr = ar - mul(br, wr) + mul(bi, wi);
      ti = ai - mul(bi, wr) - mul(br, wi);
      bp[0] = tr;
      bp[1] = ti;
      ap += 2;
      bp += 2;
    }
    ap += 4;
    bp += 4;
  }
  ap = x;
  bp = x + 8;
  for(i = 0; i < 8; i ++) {
    for(j = 0; j < 4; j ++) {
      ar = ap[0];
      ai = ap[1];
      br = bp[0];
      bi = bp[1];
      wr = w_8[2 * j];
      wi = w_8[2 * j + 1];
      tr = ar + mul(br, wr) - mul(bi, wi);
      ti = ai + mul(bi, wr) + mul(br, wi);
      ap[0] = tr;
      ap[1] = ti;
      tr = ar - mul(br, wr) + mul(bi, wi);
      ti = ai - mul(bi, wr) - mul(br, wi);
      bp[0] = tr;
      bp[1] = ti;
      ap += 2;
      bp += 2;
    }
    ap += 8;
    bp += 8;
  }
  ap = x;
  bp = x + 16;
  for(i = 0; i < 4; i ++) {
    for(j = 0; j < 8; j ++) {
      ar = ap[0];
      ai = ap[1];
      br = bp[0];
      bi = bp[1];
      wr = w_16[2 * j];
      wi = w_16[2 * j + 1];
      tr = ar + mul(br, wr) - mul(bi, wi);
      ti = ai + mul(bi, wr) + mul(br, wi);
      ap[0] = tr;
      ap[1] = ti;
      tr = ar - mul(br, wr) + mul(bi, wi);
      ti = ai - mul(bi, wr) - mul(br, wi);
      bp[0] = tr;
      bp[1] = ti;
      ap += 2;
      bp += 2;
    }
    ap += 16;
    bp += 16;
  }
  ap = x;
  bp = x + 32;
  for(i = 0; i < 2; i ++) {
    for(j = 0; j < 16; j ++) {
      ar = ap[0];
      ai = ap[1];
      br = bp[0];
      bi = bp[1];
      wr = w_32[2 * j];
      wi = w_32[2 * j + 1];
      tr = ar + mul(br, wr) - mul(bi, wi);
      ti = ai + mul(bi, wr) + mul(br, wi);
      ap[0] = tr;
      ap[1] = ti;
      tr = ar - mul(br, wr) + mul(bi, wi);
      ti = ai - mul(bi, wr) - mul(br, wi);
      bp[0] = tr;
      bp[1] = ti;
      ap += 2;
      bp += 2;
    }
    ap += 32;
    bp += 32;
  }
  ap = x;
  bp = x + 64;
  for(i = 0; i < 1; i ++) {
    for(j = 0; j < 32; j ++) {
      ar = ap[0];
      ai = ap[1];
      br = bp[0];
      bi = bp[1];
      wr = w_64[2 * j];
      wi = w_64[2 * j + 1];
      tr = ar + mul(br, wr) - mul(bi, wi);
      ti = ai + mul(bi, wr) + mul(br, wi);
      ap[0] = tr;
      ap[1] = ti;
      tr = ar - mul(br, wr) + mul(bi, wi);
      ti = ai - mul(bi, wr) - mul(br, wi);
      bp[0] = tr;
      bp[1] = ti;
      ap += 2;
      bp += 2;
    }
    ap += 64;
    bp += 64;
  }
}
