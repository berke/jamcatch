/* machine.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef MACHINE_H
#define MACHINE_H

#include "types.h"

#define NUM_NORMAL_REGISTERS 17
#define NUM_BANKED_REGISTERS 37

void machine_dump_registers(u32 *reg, u32 banked);

#endif
