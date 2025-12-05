/* keypad.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <config.h>
#include <timer.h>
#include <gpio.h>
#include <keypad.h>

task_node keypad_task;
u32 keypad_stack[KEYPAD_STACK_SIZE];
static u8 keypad_state;

const gpio_t keypad_pins[KEYPAD_COUNT] = {
  /* KEYPAD_RIGHT */
  { .io_port = KEYPAD_RIGHT_PORT,
    .io_pin  = KEYPAD_RIGHT_PIN,
    .io_dir = IO_IN,
    .io_inverted = true,
    .io_initial = IO_L },

  /* KEYPAD_UP */
  { .io_port = KEYPAD_UP_PORT,
    .io_pin  = KEYPAD_UP_PIN,
    .io_dir = IO_IN,
    .io_inverted = true,
    .io_initial = IO_L },

  /* KEYPAD_LEFT */
  { .io_port = KEYPAD_LEFT_PORT,
    .io_pin  = KEYPAD_LEFT_PIN,
    .io_dir = IO_IN,
    .io_inverted = true,
    .io_initial = IO_L },

  /* KEYPAD_DOWN */
  { .io_port = KEYPAD_DOWN_PORT,
    .io_pin  = KEYPAD_DOWN_PIN,
    .io_dir = IO_IN,
    .io_inverted = true,
    .io_initial = IO_L },
};

u8 keypad_get_raw(void)/*{{{*/
{
  return gpio_input(keypad_pins, KEYPAD_COUNT);
}/*}}}*/
static void keypad_post(u8 state)/*{{{*/
{
  keypad_state = state;
}/*}}}*/
u8 keypad_get(void)/*{{{*/
{
  return keypad_state;
}/*}}}*/
void keypad_entry(void *a)/*{{{*/
{
  u8 last_state, state;
  u32 count;

  count = 0;
  last_state = keypad_get_raw();

  for(;;) {
    state = keypad_get_raw();
    if(state == last_state && state < 10) {
      count ++;
      if(count == KEYPAD_CONSISTENCY_COUNT) {
        keypad_post(state);
      }
    } else {
      count = 0;
    }
    last_state = state;
    timer_wait(KEYPAD_PERIOD);
  }
}/*}}}*/
void keypad_init()/*{{{*/
{
  gpio_setup(keypad_pins, KEYPAD_COUNT);
  keypad_state = keypad_get_raw();
  exec_add_task(&keypad_task, "keypad", keypad_entry, 0, keypad_stack, length(keypad_stack), KEYPAD_PRI);
}/*}}}*/
