/* watchdog.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 * vim:set foldmarker={{{,}}}:
 *
 */

#include <lpc2138.h>
#include <config.h>
#include <watchdog.h>
#include <serial.h>
#include <debug.h>
#include <timer.h>

task_node watchdog_task;
u32 watchdog_stack[WATCHDOG_STACK_SIZE];
watchpoint_list watchpoints;
spinlock_t watchpoint_mutex;

void watchdog_register(watchpoint_node *wn, const char *name)/*{{{*/
{
  exec_spinlock_acquire(&watchpoint_mutex);
  wn->watchpoint.wp_name = name;
  wn->watchpoint.wp_trigger = 1;
  watchpoint_list_append(&watchpoints, wn);
  exec_spinlock_release(&watchpoint_mutex);
}/*}}}*/
static void watchdog_feed(void)/*{{{*/
{
#if WATCHDOG_FOR_REAL
  WDFEED = 0xaa;
  WDFEED = 0x55;
#endif
}/*}}}*/
void watchdog_entry(void *arg)/*{{{*/
{
  bool will_feed;
  watchpoint_node *wn;

  for(;;) {
    timer_wait(WATCHDOG_RELOAD);
    XDEBUGF("Watchdog");
    wn = watchpoint_list_head(&watchpoints);
    will_feed = true;
    while(wn) {
      if(wn->watchpoint.wp_trigger) {
        serial_printf("Watchpoint %s expired" CRLF, wn->watchpoint.wp_name);
        will_feed = false;
        wn->watchpoint.wp_trigger = 0;
      } else {
        wn->watchpoint.wp_trigger = 1;
      }
      wn = watchpoint_node_next(wn);
    }
    if(will_feed) watchdog_feed();
    else {
      serial_printf("Watchdog failure, waiting for timeout" CRLF);
      for(;;);
    }
  }
}/*}}}*/
void watchdog_start(void){/*{{{*/
  exec_add_task(&watchdog_task, "watchdog", watchdog_entry, 0, watchdog_stack, length(watchdog_stack),
                WATCHDOG_PRI);
  if(WDMOD_B.WDTOF) {
    WDMOD_B.WDTOF = 0;
    serial_printf("WARNING: Previously reset due to watchdog timeout" CRLF);
  }
#if WATCHDOG_FOR_REAL
  WDTC = WATCHDOG_TIMEOUT;
  WDTV = 0; /* XXX */
  WDMOD_B.WDRESET = 1;
  WDMOD_B.WDEN = 1;
  watchdog_feed();
#endif
}/*}}}*/
void watchdog_init(void)/*{{{*/
{
  exec_spinlock_init(&watchpoint_mutex);
  watchpoint_list_init(&watchpoints);
}/*}}}*/
