/* xtea.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <xtea.h>

#undef ntoh32
#undef hton32
#define ntoh32(x) x
#define hton32(x) x

void xtea_decipher_cbc(u32 *v, const u32 *k, int m)
{
  int i;
  u32 x0, x1;
  u32 x0b, x1b;
  x0 = 0;
  x1 = 0;

  for(i = 0; i < m; i++) {
    x0b = v[0];
    x1b = v[1];
    xtea_decipher(v, k);
    v[0] ^= x0;
    v[1] ^= x1;
    x0 = x0b;
    x1 = x1b;
    v += 2;
  }
}

void xtea_encipher_cbc(u32 *v, const u32 *k, int m)
{
  int i;

  for(i = 0; i < m; i++) {
    xtea_encipher(v, k);
    if(i + 1 < m) {
      v[2] ^= v[0];
      v[3] ^= v[1];
      v += 2;
    }
  }
}

const u32 xtea_hash_key[] = {
  0x68621e57, 0x2b99c580, 0x05759705, 0x9aa34c5b,
  0x4840c010, 0xddd2b0dc, 0x9a4511f2, 0x03963510
};

void xtea_hash(u32 *u, int m, u32 *hash)
{
  int i;
  u32 v[2];

  v[0] = 0;
  v[1] = 0;

  for(i = 0; i < m; i++) {
    v[0] ^= u[0];
    v[1] ^= u[1];
    xtea_encipher(v, xtea_hash_key);
    u += 2;
  }

  hash[0] = v[0];
  hash[1] = v[1];
}

void xtea_encipher(u32 *v, const u32 *k)
{
    u32 v0, v1;
    u32 sum, delta;
    u8 i;

    v0 = ntoh32(v[0]);
    v1 = ntoh32(v[1]);
    sum = 0;
    delta = 0x9e3779b9;

    for(i=0; i<32; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
    }
    v[0] = hton32(v0);
    v[1] = hton32(v1);
}

void xtea_decipher(u32 *v, const u32 *k)
{
    u32 v0, v1;
    u32 sum, delta;
    u8 i;

    v0 = ntoh32(v[0]);
    v1 = ntoh32(v[1]);
    sum = 0xc6ef3720;
    delta = 0x9e3779b9;

    for(i=0; i<32; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum&3]);
    }

    v[0] = hton32(v0);
    v[1] = hton32(v1);
}

#if 0
#include <stdio.h>

unsigned long key[4] = { 0x432e91c8, 0x2f8e4bc7, 0x6fe06fb0, 0x9ca0af08 };
/*unsigned long key[4] = {0,0,0,0};*/
u32 test[] = { 0x00000001, 0x00000000, 0, 0 };

int main(int argc, char **argv)
{
  int i;
  int m;

  m = sizeof(test)/sizeof(test[0]);

  xtea_encipher_cbc(test, key, m / 2);
  printf("let e = [|");
  for(i = 0; i < m; i ++) {
    printf(" 0x%08lxl;", ntoh32(test[i]));
  }
  printf(" |];;\n");

  xtea_decipher_cbc(test, key, m / 2);
  printf("let d = [|");
  for(i = 0; i < m; i ++) {
    printf(" 0x%08lxl;", ntoh32(test[i]));
  }
  printf(" |];;\n");
  return 0;
}
#endif
