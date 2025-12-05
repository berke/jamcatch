#include <stdio.h>
#include "types.h"

static inline u16 mmc_init_crc16(void)/*{{{*/
{
  return 0;
}/*}}}*/
static inline u16 mmc_add_crc16(u16 crc, u8 byte, u16 poly)/*{{{*/
{
  int i;
  u8 d;
  u8 b16;

  for(i = 0; i < 8; i ++) {
    d = !!(byte & 0x80);
    b16 = !!(crc & 0x8000);
    crc <<= 1;
    if(d ^ b16) {
      crc ^= poly;
    }
    byte <<= 1;
  }

  return crc;
}/*}}}*/
static inline u16 mmc_finalize_crc16(u16 crc)/*{{{*/
{
  return crc;
}/*}}}*/

u32 data[128] =
{
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x03000000,
  0x0f060006,
  0x0065cce0,
  0x999b0000,
  0x00000007,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0xaa550000,
};

/* 08d7 */

/* DEBUG mmc_read_data:162 crc 0000c8c0 vs 00007fa1 */

int main(int argc, char **argv)
{
  int i;
  u16 crc;
  u16 poly;
  u32 x;

  poly = 0xffff;

#if 0
  while(poly) {
    crc = mmc_init_crc16();
    for(i = 0; i < 128; i ++) {
      x = data[i];
      x = 0xffffffff;
      crc = mmc_add_crc16(crc, x & 0xff, poly); x >>= 8;
      crc = mmc_add_crc16(crc, x & 0xff, poly); x >>= 8;
      crc = mmc_add_crc16(crc, x & 0xff, poly); x >>= 8;
      crc = mmc_add_crc16(crc, x & 0xff, poly);
    }
    crc = mmc_finalize_crc16(crc);
    printf("0x%04x -> 0x%04x\n", poly, crc);
    poly --;
  }
#else
  poly = 0x1021;
  crc = mmc_init_crc16();
  for(i = 0; i < 128; i ++) {
    x = data[i];
    crc = mmc_add_crc16(crc, x & 0xff, poly); x >>= 8;
    crc = mmc_add_crc16(crc, x & 0xff, poly); x >>= 8;
    crc = mmc_add_crc16(crc, x & 0xff, poly); x >>= 8;
    crc = mmc_add_crc16(crc, x & 0xff, poly);
  }
  crc = mmc_finalize_crc16(crc);
  printf("0x%04x -> 0x%04x\n", poly, crc);
#endif
  return 0;
}
