/* alarm.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <alarm.h>
#include <system.h>
#include <config.h>
#include <exec.h>
#include <timer.h>
#include <led.h>
#include <gpio.h>

task_node alarm_task;
port_node alarm_port;
sigmask_t alarm_signal;
indicator_node alarm_count;
static bool alarm_is_active;
static bool alarm_enabled;

const gpio_t alarm_a_pin = {
  .io_pin      = ALARM_A_PIN,
  .io_port     = ALARM_A_PORT,
  .io_dir      = IO_OUT,
  .io_initial  = IO_L,
  .io_inverted = false,
};

const gpio_t alarm_b_pin = {
  .io_pin      = ALARM_B_PIN,
  .io_port     = ALARM_B_PORT,
  .io_dir      = IO_OUT,
  .io_initial  = IO_L,
  .io_inverted = false,
};

u32 alarm_stack[ALARM_STACK_SIZE];

void alarm_reset(void) {{{{
  indicator_u32_update(&alarm_count, 0);
}}}}
void alarm_entry(void *a)/*{{{*/
{
  for(;;) {
    exec_wait(alarm_signal);
    alarm_is_active = true;
    gpio_output(&alarm_a_pin, 1, 1);
    indicator_u32_update(&alarm_count, indicator_u32_get(&alarm_count) + 1);
    led_set(&led_a);
    timer_wait(ALARM_ON_PERIOD);
    gpio_output(&alarm_a_pin, 1, 0);
    timer_wait(ALARM_REST_PERIOD);
    alarm_is_active = false;
    led_clr(&led_a);
  }
}/*}}}*/
bool alarm_active(void)/*{{{*/
{
  return alarm_is_active;
}/*}}}*/
void alarm_disable(void)/*{{{*/
{
  alarm_enabled = false;
}/*}}}*/
void alarm_enable(void)/*{{{*/
{
  alarm_enabled = true;
}/*}}}*/
void alarm_trigger(void)/*{{{*/
{
  if(alarm_enabled)
    exec_signal(&alarm_task, alarm_signal);
}/*}}}*/
void alarm_init(void)/*{{{*/
{
  gpio_setup(&alarm_a_pin, 1);
  gpio_setup(&alarm_b_pin, 1);

  indicator_u32_init(&alarm_count);
  indicator_add(&alarm_count, "ac");
  exec_add_task(&alarm_task, "alarm", alarm_entry, 0, alarm_stack, length(alarm_stack), ALARM_PRI);
  alarm_is_active = false;
  alarm_enabled = true;
  alarm_signal = exec_alloc_signal(&alarm_task);
}/*}}}*/
