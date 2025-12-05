/* system.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <system.h>
#include <config.h>

void set_memmap1(void)
{
  *((u8 *) 0xe01fc040) = 1;
}

void set_memmap2(void)
{
  MEMMAP_B.MAP = 1; /* User flash mode */
}

void system_init(void)
{
  //set_memmap2();
  PLLCFG_B.MSEL = PLL_MSEL;
  PLLCFG_B.PSEL = PLL_PSEL;
  PLLCON_B.PLLE = 1;
  PLLFEED_B.FEED = 0xaa;
  PLLFEED_B.FEED = 0x55;

  while(!PLLSTAT_B.PLOCK);

  PLLCON_B.PLLC = 1;
  PLLFEED_B.FEED = 0xaa;
  PLLFEED_B.FEED = 0x55;
  APBDIV_B.DIV = APBDIV_SEL;
  MAMCR = 0;
  MAMTIM = 3;
  MAMCR = 2;
}
