/* ssp.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <config.h>
#include <ssp.h>
#include <timer.h>
#include <spi_mmc.h>
#include <debug.h>

#define SCK_PIN 17
#define MISO_PIN 18
#define MOSI_PIN 19
#define SSEL_PIN 20

void ssp_init(void)/*{{{*/
{
  SSPCR0_R r;

  r = SSPCR0_B;
  r.DSS = 7; /* 8-bit transfer */
  r.FRF = 0; /* SPI */
  r.CPOL = 0;
  r.CPHA = 0;
  r.SCR = 255; /* ((PCLK / 2) / SPI_RATE) - 1; */
  SSPCR0_B = r;

  DEBUGF("SSP SCR=%x", r.SCR);

  SSPCPSR = 2;

  SSPCR1_B.LBM = 0;
  SSPCR1_B.MS = 0;
  SSPCR1_B.SOD = 0;
  SSPCR1_B.SSE = 1; /* Enable */

  PINSEL1_B.PS_17 = 2; /* SCK */
  PINSEL1_B.PS_18 = 2; /* MISO */
  PINSEL1_B.PS_19 = 2; /* MOSI */
  PINSEL1_B.PS_20 = 2; /* SSEL */
}/*}}}*/
void spi_send_byte(u8 byte)/*{{{*/
{
  while(!SSPSR_B.TNF);
  SSPDR = byte;
}/*}}}*/
u8 spi_read_byte(void)/*{{{*/
{
  u8 byte;

  SSPDR = 0xff;
  while(!SSPSR_B.RNE);
  byte = SSPDR;
  return byte;
}/*}}}*/
void spi_synchronize(int count)/*{{{*/
{
  PINSEL1_B.PS_20 = 0; /* SSEL */
  IODIR1 |= (1 << SSEL_PIN);
  IOSET1 |= (1 << SSEL_PIN);
  while(count --) {
    XDEBUGF("Synchronizing %d", count);
    spi_send_byte(0xff);
  }
  PINSEL1_B.PS_20 = 2; /* SSEL */
}/*}}}*/
