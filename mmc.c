/* mmc.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <config.h>
#include <ssp.h>
#include <timer.h>
#include <spi_mmc.h>
#include <mmc.h>
#include <debug.h>
#include <serial.h>

void mmc_init()/*{{{*/
{
}/*}}}*/
static inline u8 mmc_init_crc7(void)/*{{{*/
{
  return 0;
}/*}}}*/
static inline u8 mmc_add_crc7(u8 crc, u8 byte)/*{{{*/
{
  int i;
  u8 d;
  u8 b6;

  for(i = 0; i < 8; i ++) {
    d = !!(byte & 0x80);
    b6 = !!(crc & 0x40);
    if(d ^ b6) {
      crc = ((crc << 1) ^ 9) & 0x7f;
    } else {
      crc = (crc << 1) & 0x7f;
    }
    byte <<= 1;
  }

  return crc;
}/*}}}*/
static inline u8 mmc_finalize_crc7(u8 crc)/*{{{*/
{
  return 1 | (crc << 1);
}/*}}}*/

static inline u16 mmc_init_crc16(void)/*{{{*/
{
  return 0;
}/*}}}*/
static inline u16 mmc_add_crc16(u16 crc, u8 byte)/*{{{*/
{
  int i;
  u8 d;
  u8 b16;

  for(i = 0; i < 8; i ++) {
    d = !!(byte & 0x80);
    b16 = !!(crc & 0x8000);
    crc <<= 1;
    if(d ^ b16) {
      crc ^= 0x1021;
    }
    byte <<= 1;
  }

  return crc;
}/*}}}*/
static inline u16 mmc_finalize_crc16(u16 crc)/*{{{*/
{
  return crc;
}/*}}}*/
void mmc_send_command(mmc_command_t cmd, u32 arg)/*{{{*/
{
  u8 crc;
  u8 byte;

  XDEBUGF("CMD > %x (%x)", cmd, arg);
  crc = mmc_init_crc7();

  byte = cmd | 0x40;
  spi_send_byte(byte);
  crc = mmc_add_crc7(crc, byte);

  byte = arg >> 24;
  spi_send_byte(byte);
  crc = mmc_add_crc7(crc, byte);

  byte = (arg >> 16) & 0xff;
  spi_send_byte(byte);
  crc = mmc_add_crc7(crc, byte);

  byte = (arg >> 8) & 0xff;
  spi_send_byte(byte);
  crc = mmc_add_crc7(crc, byte);

  byte = arg & 0xff;
  spi_send_byte(byte);
  crc = mmc_add_crc7(crc, byte);

  crc = mmc_finalize_crc7(crc);
  spi_send_byte(crc);
}/*}}}*/
/* Card has been inserted */
u8 mmc_get_response1(void)/*{{{*/
{
  u8 byte;
  
  for(;;) {
    byte = spi_read_byte();
    if(!(byte & 0x80)) break;
  }
  XDEBUGF("CMD < %x", byte);
  return byte;
}/*}}}*/
u32 mmc_read_ocr(void)/*{{{*/
{
  u32 x;
  u8 r;

  mmc_send_command(MMC_READ_OCR, 0);
  r = mmc_get_response1();
  if(r) {
    XDEBUGF("Error reading OCR !");
    return 0;
  }
  x = spi_read_byte();
  x <<= 8;
  x |= spi_read_byte();
  x <<= 8;
  x |= spi_read_byte();
  x <<= 8;
  x |= spi_read_byte();
  return x;
}/*}}}*/

bool mmc_read_data(u8 *data, u32 length)/*{{{*/
{
  int i;
  u16 crc;
  u16 crc_received;
  u8 byte;

  for(i = 0; i < MMC_READ_RETRIES; i ++) {
    byte = spi_read_byte();
    /* XDEBUGF("waiting %x", byte); */
    if(byte == MMC_TOKEN_SINGLE) break;
    timer_wait(1);
  }
  if(i == MMC_READ_RETRIES) return false;

  crc = mmc_init_crc16();
  while(length --) {
    byte = spi_read_byte();
    *(data ++) = byte;
    //XDEBUGF("got byte %x", byte);
    crc = mmc_add_crc16(crc, byte);
  }
  crc_received = spi_read_byte();
  crc_received = (crc_received << 8) | spi_read_byte();
  XDEBUGF("crc %x vs %x", crc, crc_received);

  return crc == crc_received;
}/*}}}*/
bool mmc_write_data(u8 *data, u32 length)/*{{{*/
{
  u16 crc;
  u8 byte;
  int i;

  spi_send_byte(0xff);

  spi_send_byte(MMC_TOKEN_SINGLE);

  crc = mmc_init_crc16();
  while(length --) {
    byte = *(data ++);
    spi_send_byte(byte);
    crc = mmc_add_crc16(crc, byte);
  }
  crc = mmc_finalize_crc16(crc);
  spi_send_byte(crc >> 8);
  spi_send_byte(crc & 0xff);

  for(i = 0; i < MMC_WRITE_RETRIES; i++) {
    byte = spi_read_byte();
    if((byte & MMC_DATA_RESPONSE_MASK) == MMC_DATA_RESPONSE_VALUE) break;
    timer_wait(100);
  }
  if(i == MMC_WRITE_RETRIES) {
    XDEBUGF("Timeout");
    return false;
  }
  switch(byte & MMC_DATA_STATUS_MASK) {
    case MMC_DATA_STATUS_ACCEPTED:
      XDEBUGF("Write OK");
      /* Wait until card is no more busy... */
      byte = mmc_get_response1();
      return true;
    case MMC_DATA_STATUS_REJECTED_CRC_ERROR:
      XDEBUGF("CRC error");
      return false;
    case MMC_DATA_STATUS_REJECTED_WRITE_ERROR:
      XDEBUGF("Write error");
      return false;
    default:
      XDEBUGF("Protocol error, got %x", byte);
      return false;
  }
}/*}}}*/
bool mmc_get_serial(u32 *serial_return)/*{{{*/
{
  u8 cid[16];
  u32 serial;

  mmc_send_command(MMC_SEND_CID, 0);
  if(!mmc_read_data(cid, sizeof(cid))) return false;

  serial = cid[10];
  serial <<= 8;
  serial |= cid[11];
  serial <<= 8;
  serial |= cid[12];
  serial <<= 8;
  serial |= cid[13];

  *serial_return = serial;

  return true;
}/*}}}*/
char *mmc_insert(void)/*{{{*/
{
  u8 r;
  int i;

  for(i = 0; i < MMC_RETRIES; i ++) {
    XDEBUGF("Synchronizing");
    spi_synchronize(200);
    XDEBUGF("Sending command");
    mmc_send_command(MMC_GO_IDLE_STATE, 0);
    r = mmc_get_response1();
    XDEBUGF("Got response %x", r);
    if(r == 0x01) break;
    timer_wait(TICKS(0.1));
  }
  if(i == MMC_RETRIES) return "Can't sync";

  XDEBUGF("Card is now in SPI mode");
  for(i = 0; i < MMC_RETRIES; i ++) {
    XDEBUGF("Initializing card...");
    mmc_send_command(MMC_SEND_OP_COND, 0);
    timer_wait(TICKS(0.1));
    r = mmc_get_response1();
    XDEBUGF("Got response %x", r);
    if(!r) break;
    XDEBUGF("Card still busy");
  }
  if(i == MMC_RETRIES) return "Can't init";

  /* Card is now initialized */
  XDEBUGF("Card ready");
  /* Turn CRC on */
  mmc_send_command(MMC_CRC_ON_OFF, 1);
  r = mmc_get_response1();
  if(r) {
    XDEBUGF("Can't turn CRC on ! (r = %x)");
  } else {
    XDEBUGF("CRC turned on");
  }
  /* Read CID */
  mmc_send_command(MMC_SEND_CID, 0);
  {
    u8 cid[16];

    mmc_read_data(cid, sizeof(cid));

    serial_printf("CID:" CRLF);
    serial_printf("  Manufacturer %x" CRLF, cid[0]);
    serial_printf("  OID %x" CRLF, (cid[1] << 8) | cid[2]);
    serial_printf("  Name ");
    {
      int i;
      char c;

      for(i = 0; i < 6; i ++) {
        c = cid[3 + i];
        if(c < 32 || c > 127) c = '?';
        serial_printf("%c", c);
      }
      serial_printf(CRLF);
    }
  }
  /* Read CSD */
  mmc_send_command(MMC_SEND_CSD, 0);
  {
    u8 csd[16];
    u8 multiplier;
    u32 size;
    int i;

    for(i = 0; i < 16; i ++) {
      serial_printf("  %d %x" CRLF, i, csd[i]);

    }

    mmc_read_data(csd, sizeof(csd));

    size = ((csd[7] & 3) << 10) | (csd[8] << 2) | (csd[9] >> 6);
    multiplier = csd[9];

    XDEBUGF("Size is %d %d", size, multiplier);
  }
  /* Set block length */
  mmc_send_command(MMC_SET_BLOCKLEN, MMC_BLOCK_SIZE);
  r = mmc_get_response1();
  if(r) {
    XDEBUGF("Can't set block size ! (%x)");
  } else {
    XDEBUGF("Block size set to %d", MMC_BLOCK_SIZE);
  }
  return 0;
}/*}}}*/
bool mmc_read_block(int num, u8 *data)/*{{{*/
{
  u8 r;

  mmc_send_command(MMC_READ_SINGLE_BLOCK, num * MMC_BLOCK_SIZE);
  r = mmc_get_response1();
  if(r) {
    XDEBUGF("RB %x", r);
    return false;
  }
  return mmc_read_data(data, MMC_BLOCK_SIZE);
}/*}}}*/
bool mmc_write_block(int num, u8 *data)/*{{{*/
{
  u8 r;

  mmc_send_command(MMC_WRITE_BLOCK, num * MMC_BLOCK_SIZE);
  r = mmc_get_response1();
  if(r) {
    XDEBUGF("WB %x", r);
    return false;
  }
  mmc_write_data(data, MMC_BLOCK_SIZE);
  return true;
}/*}}}*/

#if 0
#include <stdio.h>

void spi_send_byte(u8 x)
{
  printf("SEND 0x%02x\n", x);
}

int main(int argc, char **argv)
{
  mmc_send_command(MMC_GO_IDLE_STATE, 0);

  return 0;
}
#endif
