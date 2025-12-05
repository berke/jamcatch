/* dac.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <system.h>
#include <config.h>
#include <sinus.h>

#include <serial.h>

void dac_init(void)
{
  PINSEL1_B.PS_25 = 2;
  DACR_B.BIAS = 0;
}

void dac_signal(u32 increment)
{
  u32 phase;
  u32 d;

  phase = 0;

  for(;;) {
    d = (sinus[phase >> 22] >> 6) + 512;
    DACR_B.VALUE = d;
    phase += increment;
  }
}
