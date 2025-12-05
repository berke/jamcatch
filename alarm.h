/* alarm.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef ALARM_H
#define ALARM_H

#include <lpc2138.h>
#include <types.h>
#include <indicator.h>

indicator_node alarm_count;

void alarm_trigger(void);
void alarm_init(void);
bool alarm_active(void);
void alarm_enable(void);
void alarm_disable(void);
void alarm_reset(void);

#endif

