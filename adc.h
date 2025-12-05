/* adc.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef ADC_H
#define ADC_H

#include "types.h"
#include "config.h"
#include "exec.h"

typedef struct {
  u32 ab_writing; /* If 0, ab_even is being filled by FIQ; otherwise it's ab_odd */
  u32 ab_overrun; /* If 1, overrun has occurred */
  u32 ab_ack;     /* When 1, FIQ can fill !ab_writing */
  u16 *ab_even;   /* WARNING: bits 0 to 5 should be discarded */
  u16 *ab_odd;
} adc_buffer_t;

#define ADC_OK 0
#define ADC_OVERRUN 1

#define ADC_BITS 10

extern adc_buffer_t adc_buffer;

void adc_software_irq(void);
void adc_hardware_init(void);
void adc_device_init(void);
u16 adc_get_sample(void);
u32 adc_get_buffer(question_node *qn, u16 **target);

static inline s16 adc_du16_to_s16(u16 x) {
  s16 y;

  y = (x & 0xffc0) - 32768;

  return y;
}

#endif
