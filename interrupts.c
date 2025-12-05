/* interrupts.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <system.h>
#include <config.h>
#include <timer.h>
#include <debug.h>
#include <serial.h>
#include <exec.h>
#include <adc.h>
#include <led.h>

/* Handler for non-vectored IRQs */
void default_irq(void)
{
  u32 r;

  DEBUG("Default IRQ");
  r = VICrawIntr;
  DEBUGF("VICrawIntr=%x\r\n", r);
  VICvectAddr = 0;
}

void interrupts_init(void)
{
  VICvectCntl0 = VIC_TIMER0 | IRQslot_en_;
  VICvectAddr0 = (u32) EXEC_TIMER_IRQ;

  VICvectCntl1 = VIC_SOFTWARE | IRQslot_en_;
  VICvectAddr1 = (u32) adc_software_irq;

  VICvectCntl2 = VIC_UART0 | IRQslot_en_;
  VICvectAddr2 = (u32) serial_irq;

  VICdefVectAddr = (u32) default_irq;

  VICintSelect = 1 << VIC_AD0; /* FIRQ for ADC0 */

  VICintEnable = (1 << VIC_TIMER0) |
                 (1 << VIC_SOFTWARE) |
                 (1 << VIC_UART0) |
                 (1 << VIC_AD0);

  VICvectAddr = 0;
}
