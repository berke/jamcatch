/* watchdog.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 * vim:set foldmarker={{{,}}}:
 *
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "list.h"

typedef struct {
  const char *wp_name;
  volatile u32 wp_trigger;
} watchpoint;

List_Make(watchpoint);

void watchdog_init(void);
void watchdog_start(void);
void watchdog_register(watchpoint_node *wn, const char *name);
static inline void watchdog_rearm(watchpoint_node *wn) { wn->watchpoint.wp_trigger = 0; }
#endif
