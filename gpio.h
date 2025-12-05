/* gpio.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef GPIO_H
#define GPIO_H

typedef enum {
  PORT0,
  PORT1,
  PORT_COUNT
} io_port_t;

typedef struct {
  const u32 *gpio_sel_lo;
  const u32 *gpio_sel_hi;
  const u32 *gpio_dir;
  const u32 *gpio_set;
  const u32 *gpio_clr;
  const u32 *gpio_pin;
} gpio_port_t;

typedef enum {
  IO_L = 0,
  IO_H = 1,
  IO_Z = 2,
} io_value_t;

typedef enum {
  IO_IN = 0,
  IO_OUT = 1,
} io_dir_t;

typedef struct {
  u8 io_port;
  u8 io_pin;
  u8 io_dir : 1;
  u8 io_inverted : 1;
  u8 io_initial : 1;
} gpio_t;

void gpio_init(void);
void gpio_setup(const gpio_t *pins, u32 count);
void gpio_direction(const gpio_t *pins, u32 count);
void gpio_output(const gpio_t *pins, u32 count, u32 set);
u32 gpio_input(const gpio_t *pins, u32 m);

#endif
