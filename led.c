/* led.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <config.h>
#include <exec.h>
#include <timer.h>
#include <led.h>
#include <debug.h>

task_node led_task;
led_t led_status, led_a, led_b;
u32 led_stack[LED_STACK_SIZE];

void led_update(led_t *led)/*{{{*/
{
  bool state;

  state = !!(led->led_state & 1);

  gpio_output(&led->led_pin, 1, state);

  if(led->led_flags & LED_ROTATE) {
    led->led_state = (led->led_state >> 1) | (led->led_state << 31);
  } else {
    led->led_state >>= 1;
  }
}/*}}}*/
void led_setup(led_t *led)/*{{{*/
{
  gpio_setup(&led->led_pin, 1);
  led->led_state = 0;
  led_update(led);
}/*}}}*/
void led_set(led_t *led)/*{{{*/
{
  led->led_flags |= LED_ROTATE;
  led->led_state = LED_ON;
}/*}}}*/
void led_clr(led_t *led)/*{{{*/
{
  led->led_state = LED_OFF;
}/*}}}*/
void led_pattern(led_t *led, u32 pattern, bool rotate)/*{{{*/
{
  if(rotate) led->led_flags |= LED_ROTATE;
  else led->led_flags &= ~LED_ROTATE;

  led->led_state = pattern;
}/*}}}*/
void led_entry(void *a)/*{{{*/
{
  for(;;) {
    timer_wait(LED_PERIOD);
    led_update(&led_status);
    led_update(&led_a);
    led_update(&led_b);
  }
}/*}}}*/
void led_init()/*{{{*/
{
  led_status.led_pin.io_pin      = STATUS_LED_PIN;
  led_status.led_pin.io_port     = STATUS_LED_PORT;
  led_status.led_pin.io_dir      = IO_OUT;
  led_status.led_pin.io_initial  = IO_H;
  led_status.led_pin.io_inverted = false;
  led_status.led_flags           = 0;
  led_setup(&led_status);

  led_a.led_pin.io_pin      = LED_A_PIN;
  led_a.led_pin.io_port     = LED_A_PORT;
  led_a.led_pin.io_dir      = IO_OUT;
  led_a.led_pin.io_initial  = IO_L;
  led_a.led_pin.io_inverted = false;
  led_a.led_flags           = 0;
  led_setup(&led_a);

  led_b.led_pin.io_pin      = LED_B_PIN;
  led_b.led_pin.io_port     = LED_B_PORT;
  led_b.led_pin.io_dir      = IO_OUT;
  led_b.led_pin.io_initial  = IO_L;
  led_b.led_pin.io_inverted = false;
  led_b.led_flags           = 0;
  led_setup(&led_b);

  exec_add_task(&led_task, "led", led_entry, 0, led_stack, length(led_stack), LED_PRI);
}/*}}}*/
