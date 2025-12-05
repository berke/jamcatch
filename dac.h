/* dac.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef DAC_H
#define DAC_H

#include "lpc2138.h"
#include "types.h"

void dac_init(void);
void dac_signal(u32 increment);
static inline void dac_set(u16 x) { DACR_B.VALUE = x; };

#endif
