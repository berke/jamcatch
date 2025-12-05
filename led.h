/* led.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef LED_H
#define LED_H

#include <lpc2138.h>
#include <types.h>
#include <gpio.h>

#define LED_ROTATE 1

#define LED_ON 0xffffffff
#define LED_OFF 0
#define LED_FAST_BLINK 0x55555555
#define LED_SLOW_BLINK 0x00ff00ff
#define LED_FLASH 0x000000ff

typedef struct {
  gpio_t led_pin;
  u8 led_flags;
  u32 led_state;
} led_t;

extern led_t led_status, led_a, led_b;

void led_init();
void led_set(led_t *led);
void led_clr(led_t *led);
void led_pattern(led_t *led, u32 pattern, bool rotate);

#endif

