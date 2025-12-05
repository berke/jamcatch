/* delay.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef DELAY_H
#define DELAY_H

#include <types.h>
#include <timer.h>

void delay_us(u32 microseconds);
static inline void delay_ms(u32 milliseconds) {
  timer_wait((milliseconds * TIMER0_FREQUENCY) / 1000);
};

#endif
