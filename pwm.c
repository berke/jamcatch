/* pwm.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <system.h>
#include <config.h>

void pwm_init(void)
{
  /* Set pin 7 as PWM */
  PINSEL0_B.PS_7 = 2;

  /* No prescaling */
  PWMPR = 0;

  /* Reset, don't stop, don't interrupt on match 0 */
  PWMMCR_B.PWMMR0R = 1;
  PWMMCR_B.PWMMR0S = 0;
  PWMMCR_B.PWMMR0I = 0;

  /* Will set all PWMs @ 1636 */
  PWMMR0 = PWM0_PERIOD;/* 14.745600 MHz / 1636 = 36052.811 878 Hz */

  /* Set PWM2 single-edge */
  PWMPCR_B.PWMSEL2 = 0;
  PWMPCR_B.PWMENA2 = 1;

  PWMMR2 = PWM0_PERIOD / 2; /* Will clear PWMMR2 @ 818 clocks */

  PWMLER_B.EPWMM0L = 1;
  PWMLER_B.EPWMM2L = 1;

  PWMTCR_B.PWMENA = 1;
  PWMTCR_B.RST = 0;
  PWMTCR_B.ENA = 1;
}
