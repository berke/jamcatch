/* joker.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <types.h>
#include <serial.h>
#include <debug.h>
#include <exec.h>
#include <timer.h>
#include <lcd.h>
#include <random.h>

#define NUM_TASKS 4
#define STACK_SIZE 64

volatile static int alpha_counter;

typedef struct {
  int alpha_id;
  task_node alpha_task;
  u32 alpha_stack[STACK_SIZE];
  port_node alpha_port;
} alpha_t;

alpha_t alphas[NUM_TASKS];

enum {
  ALPHA_SEND,
  ALPHA_RECEIVE,
  ALPHA_WAIT,
  ALPHA_BUSY,
  ALPHA_COUNT,
};

void alpha_entry(void *arg)
{
  alpha_t *alpha;
  volatile int count;
  question_node qn, *qnp;
  random_t rnd;
  int id;

  alpha = (alpha_t *) arg;

  serial_printf("Entering task alpha %d" CRLF, alpha->alpha_id);
  count = 0;
  random_init(&rnd);
  exec_question_init(&qn);

  for(;;) {
    alpha_counter ++;

    if(!alpha->alpha_id) {
      lcd_clear();
      lcd_printf("Alpha: %d", alpha_counter);
      lcd_refresh();
    }

    switch(random_get(&rnd) % ALPHA_COUNT) {
      case ALPHA_SEND:
        id = random_get(&rnd) % ALPHA_COUNT;
        if(id < alpha->alpha_id) {
          qn.question.q_question = count;
          exec_ask(&alphas[id].alpha_port, &qn);
        }
        break;

      case ALPHA_RECEIVE:
        qnp = exec_check_question(&alpha->alpha_port);
        if(qnp) {
          qnp->question.q_answer = 100 * qnp->question.q_question + 23;
          exec_reply(&alpha->alpha_port, qnp, 0);
        }
        break;

      case ALPHA_WAIT:
        timer_wait(random_get(&rnd) % TICKS(0.1));
        break;

      case ALPHA_BUSY:
        {
          volatile int i;
          int x;

          x = random_get(&rnd) % 10000;
          for(i = 0; i < x; i ++) {
            i ++;
          }
        }
        break;

      default:
        break;
    }
  }
}

void joker_init(void)
{
  int i;
  critical section;

  ENTER_CRITICAL(section);
  alpha_counter = 0;
  for(i = 0; i < NUM_TASKS; i ++) {
    alphas[i].alpha_id = i;
    exec_add_port(&alphas[i].alpha_port, "alphaport", &alphas[i].alpha_task, PORT_FLAGS_NONE);
    exec_add_task(&alphas[i].alpha_task, "alpha", alpha_entry, alphas + i,
                  alphas[i].alpha_stack,
                  length(alphas[i].alpha_stack),
                  EXEC_PRI_3);
  }
  LEAVE_CRITICAL(section);
}
