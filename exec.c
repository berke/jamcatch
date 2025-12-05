/* exec.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 * vim:set foldmarker={{{,}}}:
 *
 */

#include <exec.h>
#include <debug.h>
#include <serial.h>
#include <timer.h>
#include <machine.h>
#include <config.h>
#include <string.h>
#include <abort.h>
#include <assert.h>

#define STACK_COOKIE 0x097423753

u32 idle_stack[64];
kernel_t kernel;
u32 exec_scheduler_stack[EXEC_SCHEDULER_STACK_SIZE];
void *exec_scheduler_sp;

task_node idle_task;

/* Debugging */
static inline void exec_abort(char *msg)/*{{{*/
{
  abort(msg);
}
/*}}}*/

/* Scheduler */
void exec_continue(int interrupt)/*{{{*/
{
  if(interrupt == EXEC_YIELD) {
    exec_restore_from_user(kernel.k_current);
  } else {
    exec_restore_from_interrupt(kernel.k_current);
  }
}/*}}}*/
void exec_schedule(int interrupt)/*{{{*/
{
  int i;
  task_node *tn;

#if 0
  DEBUGF("Scheduling (interrupt=%d)", interrupt);
#endif

  /* Find a task to schedule */
  for(i = 0; i < EXEC_PRI_NUMBER; i ++) {
    if(interrupt == EXEC_PREEMPT) {
      task_list_rotate(&kernel.k_pools[i].pp_ready);
    }
    tn = task_list_head(&kernel.k_pools[i].pp_ready);
    if(tn) break;
  }

  if(tn) {
    kernel.k_current = tn;
    tn->task.task_activations ++;

#if EXEC_CHECK_STACK_BOUNDS
    {
      u32 *sp;

      sp = (u32 *) tn->task.task_registers[R_SP];
      if(sp < tn->task.task_stack_low || sp > tn->task.task_stack_high) {
        serial_printf(">>> TASK %p (%s) HAS SP %p OUT OF BOUNDS!" CRLF,
                      tn, tn->task.task_name, sp);
        exec_list_tasks(true);
        abort("SP out of bounds");
      }
    }
#endif

#if EXEC_TRACE_STACK_USAGE
    {
      u32 *sp;

      sp = (u32 *) tn->task.task_registers[R_SP];
      if(sp < tn->task.task_stack_low_water) tn->task.task_stack_low_water = sp;
    }
#endif

#if EXEC_CHECK_STACK_COOKIES
    if(tn->task.task_stack_low[0] != STACK_COOKIE) {
      serial_printf(">>> TASK %p (%s) LOST HIS COOKIE" CRLF, tn, tn->task.task_name);
      abort("Lost cookie");
    }
#endif
  }

  exec_continue(interrupt);
}/*}}}*/
void exec_timer_interrupt(void)/*{{{*/
{
  timer_interrupt();
  exec_schedule(EXEC_PREEMPT);
}/*}}}*/

/* Tasks */
void exec_add_task(task_node *tn, string name, void (*entry)(void *), void *arg, u32 *stack, u32 stack_size, task_priority_t priority)/*{{{*/
{
  int i;
  task *t;

  t = task_node_contents(tn);
  t->task_name = name;
  stack[0] = STACK_COOKIE;
  t->task_stack_low = stack;
  t->task_stack_high = stack + stack_size;
  t->task_status = TASK_ORPHAN;
  assert(priority < EXEC_PRI_NUMBER);
  t->task_priority = priority;

#if EXEC_TRACE_STACK_USAGE
  t->task_stack_low_water = stack + stack_size;
#endif

  t->task_sigpend = SIGMASK_NONE;
  t->task_sigwait = SIGMASK_NONE;
  t->task_sigfree = SIGMASK_ALL;

  t->task_activations = 0;

  t->task_registers[0] = (u32) arg;
  for(i = 1; i < NUM_SAVED_REGISTERS; i ++) {
    t->task_registers[i] = (u32) tn;
  }
  t->task_registers[R_SP] = (u32) t->task_stack_high - 4; /* -4 not necessary but... */
  t->task_registers[R_LR] = (u32) exec_exit;
  t->task_registers[R_PC] = (u32) entry;
  t->task_registers[R_SR] = RUNNING_MODE;

  task_node_init(tn);

  exec_make_task_ready(tn);
}/*}}}*/
void exec_dump_tasks_in_list(task_list *tl, int details)/*{{{*/
{
  task_node *tn;
  task *t;
  char c;

  tn = task_list_head(tl);

  for(;;) {
    if(!tn) break;
    t = &tn->task;

    c = '?';
    switch(t->task_status) {
      case TASK_ORPHAN:
        c = 'O';
        break;
      case TASK_READY:
        c = 'R';
        break;
      case TASK_WAIT:
        c = 'W';
        break;
    }

    serial_printf(YLW "%p" RED " %c %d" NML " %x %x %x" BLU " %p %p" NML
                  GRN " %08u" NML /* Activations */
#if EXEC_TRACE_STACK_USAGE
                  BLU " %04u" NML
#endif
                  " | " YLW "%s" NML CRLF,
                  tn,
                  c,
                  t->task_priority,
                  t->task_sigpend,
                  t->task_sigwait,
                  t->task_sigfree,
                  t->task_stack_low,
                  t->task_stack_high,
                  t->task_activations,
#if EXEC_TRACE_STACK_USAGE
                  t->task_stack_high - t->task_stack_low_water,
#endif
                  t->task_name);
    if(details) {
      machine_dump_registers(t->task_registers, 0);
    }
    tn = task_node_next(tn);
  }
}/*}}}*/
void exec_list_tasks(int details)/*{{{*/
{
  critical section;
  int i;

  ENTER_CRITICAL(section);
  for(i = 0; i < EXEC_PRI_NUMBER; i ++) {
    exec_dump_tasks_in_list(&kernel.k_pools[i].pp_ready, details);
    exec_dump_tasks_in_list(&kernel.k_pools[i].pp_wait, details);
  }
  LEAVE_CRITICAL(section);
}/*}}}*/
void exec_make_task_ready(task_node *tn)/*{{{*/
{
  critical section;

  /* DEBUGF("Making task %p ready", tn); */
  ENTER_CRITICAL(section);
  switch(tn->task.task_status) {
    case TASK_READY:
      break;
    case TASK_WAIT:
      task_list_remove(&kernel.k_pools[tn->task.task_priority].pp_wait, tn);
      /* FALL */
    case TASK_ORPHAN:
      tn->task.task_status = TASK_READY;
      task_list_prepend(&kernel.k_pools[tn->task.task_priority].pp_ready, tn);
      break;
  }
  /* NO kernel.k_current = tn; !! */
  LEAVE_CRITICAL(section);
}/*}}}*/
void exec_put_task_to_sleep(task_node *tn)/*{{{*/
{
  critical section;

  ENTER_CRITICAL(section);
  switch(tn->task.task_status) {
    case TASK_WAIT:
      break;
    case TASK_READY:
      task_list_remove(&kernel.k_pools[tn->task.task_priority].pp_ready, tn);
      /* FALL */
    case TASK_ORPHAN:
      tn->task.task_status = TASK_WAIT;
      task_list_append(&kernel.k_pools[tn->task.task_priority].pp_wait, tn);
      break;
  }
  LEAVE_CRITICAL(section);
}/*}}}*/
void exec_freeze(task_node *tn)/*{{{*/
{
  critical section;

  ENTER_CRITICAL(section);
  tn->task.task_sigwait = SIGMASK_FREEZE;
  exec_put_task_to_sleep(tn);
  LEAVE_CRITICAL(section);
}/*}}}*/
void exec_exit(void)/*{{{*/
{

}/*}}}*/
inline task_node *exec_current_task(void)/*{{{*/
{
  return kernel.k_current;
}
/*}}}*/

/* Signals */
static sigmask_t exec_alloc_signal_inner(task_node *tn)/*{{{*/
{
  sigmask_t free;
  sigmask_t mask;
  int i;

  free = tn->task.task_sigfree;
  if(!free) exec_abort("No free signals");

  i = 0;
  mask = 1;
  for(;;) {
    if(free & mask) {
      tn->task.task_sigfree &= ~mask;
      break;
    }
    mask <<= 1;
  }
  return mask;
}/*}}}*/
sigmask_t exec_alloc_signal(task_node *tn)/*{{{*/
{
  critical section;
  sigmask_t mask;

  ENTER_CRITICAL(section);
  if(!tn) tn = exec_current_task();
  mask = exec_alloc_signal_inner(tn);
  LEAVE_CRITICAL(section);
  return mask;
}/*}}}*/
static inline void exec_free_signal_inner(task_node *tn, sigmask_t signal)/*{{{*/
{
  tn->task.task_sigfree |= signal;
}
/*}}}*/
void exec_free_signal(sigmask_t signal)/*{{{*/
{
  critical section;
  task_node *tn;

  ENTER_CRITICAL(section);
  tn = exec_current_task();
  exec_free_signal_inner(tn, signal);
  LEAVE_CRITICAL(section);
}
/*}}}*/
static sigmask_t exec_wait_critical(sigmask_t signal, critical section)/*{{{*/
{
  task_node *tn;
  sigmask_t mask;

  tn = exec_current_task();
  for(;;) {
    /* Is one or more of the signals in the set of requested ones pending ? */
    mask = tn->task.task_sigpend & signal;
    if(mask) {
      /* Yes.  Clear them from the set of pending and wait signals. */
      tn->task.task_sigpend &= ~mask;
      tn->task.task_sigwait &= ~mask;
      LEAVE_CRITICAL(section);
      return mask;
    } else {
      /* No.  Wait on the set. */
      tn->task.task_sigwait |= signal;
      exec_put_task_to_sleep(tn);
      exec_yield_critical(section); /* What about spurious wakeups ? */
      /* Here, we won't be in the critical section. */
      ENTER_CRITICAL(section);
    }
  }
}/*}}}*/
sigmask_t exec_wait(sigmask_t signal)/*{{{*/
{
  critical section;

  ENTER_CRITICAL(section);
  return exec_wait_critical(signal, section);
}/*}}}*/
sigmask_t exec_check_signal(sigmask_t signal)/*{{{*/
{
  critical section;
  task_node *tn;
  sigmask_t mask;

  tn = exec_current_task();
  ENTER_CRITICAL(section);
  mask = tn->task.task_sigpend & signal;
  tn->task.task_sigpend &= ~mask;
  LEAVE_CRITICAL(section);
  return mask;
}/*}}}*/
void exec_signal(task_node *tn, sigmask_t signal)/*{{{*/
{
  critical section;

  /* DEBUGF("Making task %p ready", tn); */
  ENTER_CRITICAL(section);
  tn->task.task_sigpend |= signal;
  if(tn->task.task_sigwait & signal) { /* XXX s/\& signal/\& task,task_sigpend/ ?*/
    exec_make_task_ready(tn);
  }
  /* NO kernel.k_current = tn; !! */
  LEAVE_CRITICAL(section);
}/*}}}*/

/* Questions */
void exec_question_init(question_node *qn)/*{{{*/
{
  critical section;

  ENTER_CRITICAL(section);
  qn->question.q_asker = exec_current_task();
  qn->question.q_signal = exec_alloc_signal_inner(qn->question.q_asker);
  qn->question.q_flags = QUESTION_FLAG_DONE;
  LEAVE_CRITICAL(section);
}/*}}}*/
void exec_question_release(question_node *qn)/*{{{*/
{
  critical section;

  ENTER_CRITICAL(section);
  exec_free_signal_inner(qn->question.q_asker, qn->question.q_signal);
  LEAVE_CRITICAL(section);
}/*}}}*/
bool exec_question_busy(question_node *qn)/*{{{*/
{
  return !(qn->question.q_flags & QUESTION_FLAG_DONE);
}/*}}}*/

/* Ports */
void exec_add_port(port_node *pn, string name, task_node *answerer, port_flags_t flags)/*{{{*/
{
  critical section;

  port_node_init(pn);
  question_list_init(&pn->port.port_questions);
  pn->port.port_answerer = answerer;
  pn->port.port_flags = flags;
  pn->port.port_name = name;
  ENTER_CRITICAL(section);
  if(!(flags & PORT_FLAGS_IRQ))
    pn->port.port_signal = exec_alloc_signal_inner(answerer);
  else
    pn->port.port_signal = 0;
  port_list_append(&kernel.k_ports, pn);
  LEAVE_CRITICAL(section);
}/*}}}*/
void exec_list_ports(int details)/*{{{*/
{
  critical section;
  port_node *pn;
  task_node *tn;

  serial_printf("Ports:\r\n");
  ENTER_CRITICAL(section);
  pn = port_list_head(&kernel.k_ports);
  while(pn) {
    tn = pn->port.port_answerer;

    serial_printf("  Port [%p] %s %s %x" CRLF,
        pn,
        pn->port.port_name,
        tn ? tn->task.task_name : "(IRQ)",
        pn->port.port_signal);

    if(details) {
      question_node *qn;

      qn = question_list_head(&pn->port.port_questions);
      while(qn) {
        serial_printf("    Question [%p] %x %x %s\r\n",
                      qn,
                      qn->question.q_question,
                      qn->question.q_answer,
                      qn->question.q_asker->task.task_name);
        qn = question_node_next(qn);
      }
    }
    pn = port_node_next(pn);
  }
  serial_printf("End.\r\n");
  LEAVE_CRITICAL(section);
}/*}}}*/
void exec_reply(port_node *pn, question_node *qn, bool yield)/*{{{*/
{
  critical section;

  /* DEBUGF("Reply to question %p", qn); */
  ENTER_CRITICAL(section);
  qn->question.q_flags |= QUESTION_FLAG_DONE;
  /* DEBUGF("Making %p ready", qn->question.q_asker); */
  /* exec_make_task_ready(qn->question.q_asker); */
  exec_signal(qn->question.q_asker, qn->question.q_signal);
  question_list_remove(&pn->port.port_questions, qn);
  /* XXX: maybe not */
  if(yield) {
    exec_yield_critical(section);
  } // XXX
  LEAVE_CRITICAL(section);
  /* XXX Should we schedule ? */
}/*}}}*/
question_node *exec_check_question(port_node *pn)/*{{{*/
{
  question_node *qn;
  task_node *tn;
  critical section;

  ENTER_CRITICAL(section);
  tn = exec_current_task();
  qn = question_list_head(&pn->port.port_questions);
  if(qn) question_list_remove(&pn->port.port_questions, qn);
  LEAVE_CRITICAL(section);
  return qn;
}/*}}}*/
question_node *exec_get_question(port_node *pn)/*{{{*/
{
  question_node *qn;
  task_node *tn;
  critical section;

  /* DEBUGF("Get question for port %p", pn); */
  for(;;) {
    /* DEBUGF("Get question %p: entering critical", qn); */
    ENTER_CRITICAL(section);
    tn = exec_current_task();
    assert(tn == pn->port.port_answerer);
    qn = question_list_head(&pn->port.port_questions);
    /* DEBUGF("Get question, qn=%p", qn); */

    if(qn) {
      question_list_remove(&pn->port.port_questions, qn);
      LEAVE_CRITICAL(section);
      return qn;
    } else {
      /* Will need to block task... */
      /* DEBUGF("Need to block."); */
      exec_wait_critical(pn->port.port_signal, section);
      /* exec_put_task_to_sleep(tn); */
      /* XXX NO exec_leave_critical_section(); */
    }
  }
}/*}}}*/
void exec_ask(port_node *pn, question_node *qn)/*{{{*/
{
  task_node *tn_asker, *tn_answerer;
  critical section;

  qn->question.q_flags = 0;

  ENTER_CRITICAL(section);
  tn_asker = exec_current_task();
  qn->question.q_asker = tn_asker;
  question_list_append(&pn->port.port_questions, qn);
  tn_answerer = pn->port.port_answerer;

  for(;;) {
    if(tn_answerer) {
      exec_signal(tn_answerer, pn->port.port_signal);
    }
    exec_wait_critical(qn->question.q_signal, section);
    if(qn->question.q_flags & QUESTION_FLAG_DONE) break;
    ENTER_CRITICAL(section);
  }
}/*}}}*/
void exec_send(port_node *pn, question_node *qn)/*{{{*/
{
  /* Send a question without waiting for an answer. */
  task_node *tn_asker, *tn_answerer;
  critical section;

  if(!(qn->question.q_flags & QUESTION_FLAG_DONE)) {
    /* Question is still busy. */
    exec_wait_for_answer(pn, qn);
  }

  qn->question.q_flags = 0;

  ENTER_CRITICAL(section);
  tn_asker = exec_current_task();
  qn->question.q_asker = tn_asker;
  question_list_append(&pn->port.port_questions, qn);
  tn_answerer = pn->port.port_answerer;

  if(tn_answerer) {
    exec_signal(tn_answerer, pn->port.port_signal);
  }
  LEAVE_CRITICAL(section);
}/*}}}*/
void exec_wait_for_answer(port_node *pn, question_node *qn)/*{{{*/
{
  critical section;
  task_node *tn_asker, *tn_answerer;

  ENTER_CRITICAL(section);

  if(qn->question.q_flags & QUESTION_FLAG_DONE) {
    LEAVE_CRITICAL(section);
    return;
  }
  tn_asker = exec_current_task();
  qn->question.q_asker = tn_asker;
  question_list_append(&pn->port.port_questions, qn);
  tn_answerer = pn->port.port_answerer;

  for(;;) {
    if(tn_answerer) {
      exec_signal(tn_answerer, pn->port.port_signal);
    }
    exec_wait_critical(qn->question.q_signal, section);
    if(qn->question.q_flags & QUESTION_FLAG_DONE) break;
    ENTER_CRITICAL(section);
  }
}/*}}}*/

/* Spinlocks */
void exec_spinlock_init(spinlock_t *sl)/*{{{*/
{
  sl->sl_value = 0;
}/*}}}*/
void exec_spinlock_acquire(spinlock_t *sl)/*{{{*/
{
  critical section;
  bool ok;
  volatile int k;
  int l;

  ok = false;

  l = 3841;

  for(;;) {
    ENTER_CRITICAL(section);
    if(!sl->sl_value) {
      sl->sl_value ++;
      ok = true;
    }
    LEAVE_CRITICAL(section);
    if(ok) return;

    k = l;
    while(k --);
    l = 0xfff & ((l << 1) || (l >> 11));
  }
}/*}}}*/
void exec_spinlock_release(spinlock_t *sl)/*{{{*/
{
  critical section;

  ENTER_CRITICAL(section);
  if(sl->sl_value) {
    sl->sl_value --;
  }
  sl->sl_value = 0;
  LEAVE_CRITICAL(section);
}/*}}}*/

/* Start */
void __attribute__((naked)) idle_entry(void *arg)/*{{{*/
{
  critical section;

  for(;;) {
    ENTER_CRITICAL(section);
    exec_yield_critical(section);
    LEAVE_CRITICAL(section);
  }
}/*}}}*/
void exec_init(void)/*{{{*/
{
  int i;

  kernel.k_current = 0;
  for(i = 0; i < EXEC_PRI_NUMBER; i++) {
    task_list_init(&kernel.k_pools[i].pp_ready);
    task_list_init(&kernel.k_pools[i].pp_wait);
  }
  port_list_init(&kernel.k_ports);

  exec_add_task(&idle_task, "idle", idle_entry, 0, idle_stack, length(idle_stack), EXEC_PRI_LOWEST);
  exec_scheduler_sp = exec_scheduler_stack + length(exec_scheduler_stack);
}
/*}}}*/
void exec_start(void)/*{{{*/
{
  exec_schedule(EXEC_FIRST_SCHEDULE);
}/*}}}*/
