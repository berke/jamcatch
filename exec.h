/* exec.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef EXEC_H
#define EXEC_H

#include <types.h>
#include <list.h>
#include <lpc2138.h>
#include <config.h>

#define NUM_SAVED_REGISTERS 17

#define R_SP 13
#define R_LR 14
#define R_PC 15
#define R_SR 16

#define MODE_USR 0x10
#define MODE_SYS 0x1f

#define RUNNING_MODE MODE_SYS

typedef u16 sigmask_t;

#define SIGMASK_NONE 0
#define SIGMASK_ALL 0xffff
#define SIGMASK_FREEZE 0x8000

enum {
  TASK_ORPHAN = 0,
  TASK_READY = 1,
  TASK_WAIT = 2
};

typedef u8 task_status_t;

/* Number of distinct priority levels */

#define EXEC_PRI_HIGHEST 0
#define EXEC_PRI_0 0
#define EXEC_PRI_HIGH 1
#define EXEC_PRI_1 1
#define EXEC_PRI_NORMAL 2
#define EXEC_PRI_2 2
#define EXEC_PRI_LOW 3
#define EXEC_PRI_3 3
#define EXEC_PRI_LOWEST 4
#define EXEC_PRI_4 4
#define EXEC_PRI_NUMBER 5

typedef u8 task_priority_t;

typedef struct {
  /*  0 */ u32 task_registers[NUM_SAVED_REGISTERS];

  task_status_t task_status;
  task_priority_t task_priority;
  sigmask_t task_sigpend;    /* Pending signals */
  sigmask_t task_sigwait;    /* Signals the task is waiting for */
  sigmask_t task_sigfree;    /* Free signals */

  u32 task_activations;
  u32 *task_stack_high;
  u32 *task_stack_low;

#if EXEC_TRACE_STACK_USAGE
  u32 *task_stack_low_water;
#endif
  string task_name;
} task;

List_Make(task)

#define QUESTION_FLAG_DONE 1

typedef u16 question_flags_t;

typedef struct {
  task_node *q_asker;
  question_flags_t q_flags;
  sigmask_t q_signal;
  union {
    u32 q_question;
    u32 q_answer;
  };
  union {
    u32 q_question_extra;
    u32 q_answer_extra;
  };
} question;

List_Make(question);

typedef u16 port_flags_t;

#define PORT_FLAGS_NONE 0
#define PORT_FLAGS_IRQ 1

typedef struct {
  question_list port_questions;
  task_node *port_answerer;
  port_flags_t port_flags;
  sigmask_t port_signal;
  string port_name;
} port;

typedef struct {
  int mutex_count;
} mutex;

void mutex_init(mutex *mx);
void mutex_trylock(mutex *mx);
void mutex_unlock(mutex *mx);

List_Make(port);

typedef struct {
  task_list pp_ready;
  task_list pp_wait;
} priority_pool;

typedef struct {
  /*  0 */ task_node *k_current;       /* Currently running task -- hard referenced, do not change ! */
  priority_pool k_pools[EXEC_PRI_NUMBER]; /* Task list */
  port_list k_ports;                   /* Known ports */
} kernel_t;

extern kernel_t kernel;
extern void *exec_scheduler_sp;

typedef struct {
  volatile u32 sl_value;
} spinlock_t;

void exec_spinlock_init(spinlock_t *);
void exec_spinlock_acquire(spinlock_t *);
void exec_spinlock_release(spinlock_t *);

/* Initialization and interrupts */
void exec_init(void);
void exec_start(void);

/* Interrupts */

#define EXEC_TIMER_IRQ exec_timer_interrupt

/* Software yield, for instance due to a port operation */
#define EXEC_YIELD 0
/* Timer interrupt -- pre-empt running tasks */
#define EXEC_PREEMPT 1
/* Called from another interrupt */
#define EXEC_OTHER_INTERRUPT 2
/* Initial call */
#define EXEC_FIRST_SCHEDULE 3

void exec_timer_interrupt(void);

/* Tasks */
void exec_add_task(task_node *tn, string name,
                   void (*entry)(void *), void *arg, u32 *stack, u32 stack_size, task_priority_t priority);
void exec_list_tasks(int details);
task_node *exec_current_task(void);
void exec_freeze(task_node *tn);

/* Scheduling */
void exec_schedule(int interrupt);
void exec_make_task_ready(task_node *tn);
extern void exec_restore_from_user(task_node *t);
extern void exec_restore_from_interrupt(task_node *t);
extern void exec_yield(void);
extern void exec_yield_from_interrupt(void);
extern void exec_yield_critical(u32 section);

/* Signals */
sigmask_t exec_wait(sigmask_t sigmask);
sigmask_t exec_alloc_signal(task_node *tn);
void exec_free_signal(sigmask_t signal);
void exec_signal(task_node *tn, sigmask_t signal);
sigmask_t exec_check_signal(sigmask_t signal);

/* Questions */
void exec_question_init(question_node *qn);
void exec_question_release(question_node *qn);
bool exec_question_busy(question_node *qn);

/* Ports */
void exec_add_port(port_node *pn, string name, task_node *answerer, port_flags_t flags);
void exec_list_ports(int details);
void exec_ask(port_node *pn, question_node *qn);
question_node *exec_get_question(port_node *pn);
question_node *exec_check_question(port_node *pn);
void exec_reply(port_node *pn, question_node *q, bool yield);
void exec_wait_for_answer(port_node *pn, question_node *qn);
void exec_send(port_node *pn, question_node *qn);

/* Misc */

void exec_exit(void);

#endif
