/* timer.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef TIMER_H
#define TIMER_H

#include "lpc2138.h"
#include "config.h"
#include "exec.h"

#define TIMER_FLAG_NONE 0
#define TIMER_FLAG_REARM 1

typedef u32 timer_flags_t;

typedef struct {
  question_node tr_question;
  u32 tr_start;
  u32 tr_period;
  timer_flags_t tr_flags;
} timer_request;

#define TICKS(s) ((u32) (s * TIMER0_FREQUENCY))

void timer_prepare(void);
void timer_start(void);
void INTERRUPT timer_irq(void);
void timer_interrupt(void);
void timer_wait(u32 ticks);
void timer_init_device(void);
void timer_prepare_and_launch_request(timer_request *tr);
void timer_release_request(timer_request *tr);
void timer_wait_for_request(timer_request *tr);
void timer_wait(u32 ticks);
u32 timer_get_counter(void);

#endif
