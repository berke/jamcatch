/* timer.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <led.h>
#include <system.h>
#include <config.h>
#include <debug.h>
#include <exec.h>
#include <timer.h>

static volatile u32 timer_counter;
port_node timer_port;

u32 timer_get_counter(void) {/*{{{*/
  return timer_counter;
}/*}}}*/
void timer_interrupt(void)/*{{{*/
{
  question_node *qn, *qn_next;
  timer_request *tr;

  timer_counter ++;
  qn = question_list_head(&timer_port.port.port_questions); /* We are in an IRQ, no need for critical sections XXX */
  /* DEBUGF("Timer IRQ: questions %p", qn); */
  while(qn) {
    qn_next = question_node_next(qn);
    tr = (timer_request *) qn->question.q_question;
    /* DEBUGF("Question %p has TR %p whose counter is %u while time is %u", qn, tr, tr->tr_start, timer_counter); */
    if(timer_counter >= tr->tr_start) {
      /* This timer has expired */
      /* DEBUGF("Firing question %p", qn); */
      if(tr->tr_flags & TIMER_FLAG_REARM) {
        tr->tr_start = timer_counter + tr->tr_period;
      }
      exec_reply(&timer_port, qn, false);
      //question_list_remove(&timer_port.port.port_questions, qn);
    }
    qn = qn_next;
  }

  T0IR_B.MR0I |= 1; 
  VICvectAddr = VICvectAddr;
}/*}}}*/
void timer_prepare(void)/*{{{*/
{
  timer_counter = 0;
  T0MR0 = TIMER0_PERIOD;
  T0MCR_B.MR0I = 1;
  T0MCR_B.MR0R = 1;
  T0EMR_B.EMC1 = 3;
}/*}}}*/
void timer_start(void)/*{{{*/
{
  T0PR = 0;
  T0IR_B.MR0I |= 1; 
  T0TCR_B.CR = 1;
  T0TCR_B.CE = 1;
  T0TCR_B.CR = 0;
}/*}}}*/
void timer_init_device()/*{{{*/
{
  exec_add_port(&timer_port, "timer", 0, PORT_FLAGS_IRQ);
}/*}}}*/
void timer_prepare_and_launch_request(timer_request *tr)/*{{{*/
{
  critical section;
  question_node *qn;

  ENTER_CRITICAL(section);
  tr->tr_start += timer_counter;
  LEAVE_CRITICAL(section);
  qn = &tr->tr_question;
  qn->question.q_question = (u32) tr;
  exec_question_init(qn);
  exec_ask(&timer_port, qn);
}/*}}}*/
void timer_release_request(timer_request *tr)/*{{{*/
{
  critical section;
  question_node *qn;

  qn = &tr->tr_question;
  if(tr->tr_flags & TIMER_FLAG_REARM) {
    ENTER_CRITICAL(section);
    tr->tr_start = timer_counter;
    tr->tr_flags = TIMER_FLAG_NONE;
    LEAVE_CRITICAL(section);
    exec_ask(&timer_port, qn);
  }
  exec_question_release(qn);
}/*}}}*/
void timer_wait_for_request(timer_request *tr)/*{{{*/
{
  exec_ask(&timer_port, &tr->tr_question);
}/*}}}*/
void timer_wait(u32 ticks)/*{{{*/
{
  timer_request tr;

  tr.tr_start = ticks;
  tr.tr_period = 0;
  tr.tr_flags = TIMER_FLAG_NONE;
  timer_prepare_and_launch_request(&tr);
  timer_release_request(&tr);
}/*}}}*/
