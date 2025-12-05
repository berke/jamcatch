/* gpio.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <config.h>
#include <gpio.h>
#include <assert.h>
#include <debug.h>

const gpio_port_t gpio_ports[PORT_COUNT] = {
  /* Port 0 */
  { .gpio_pin    = (u32 *) 0xe0028000,
    .gpio_set    = (u32 *) 0xe0028004,
    .gpio_dir    = (u32 *) 0xe0028008,
    .gpio_clr    = (u32 *) 0xe002800c, },

  /* Port 1 */
  { .gpio_pin    = (u32 *) 0xe0028010,
    .gpio_set    = (u32 *) 0xe0028014,
    .gpio_dir    = (u32 *) 0xe0028018,
    .gpio_clr    = (u32 *) 0xe002801c, },
};

void gpio_init(void)/*{{{*/
{

}/*}}}*/
static inline void affect_ports(u32 *zclr, u32 *zset, u32 *clr, u32 *set)/*{{{*/
{
  int i;
  const gpio_port_t *port;
  critical section;

  port = gpio_ports;

  XDEBUGF("affect");
  for(i = 0; i < PORT_COUNT; i ++) {
    u32 x, y;

    if(zclr[i] || zset[i]) {
      ENTER_CRITICAL(section);
      x = H32(port->gpio_dir);
      y = (x & ~zclr[i]) | zset[i];
      XDEBUGF("zc=%x zs=%x i=%d x=%x y=%x", zclr[i], zset[i], i, x, y);
      H32(port->gpio_dir) = y;
      LEAVE_CRITICAL(section);
    }

#if 0
    ENTER_CRITICAL(section);
    x = H32(port->gpio_pin);
    y = (x & ~clr[i]) | set[i];
    H32(port->gpio_pin) = y;
    x = H32(port->gpio_pin);
    LEAVE_CRITICAL(section);
#else
    if(set[i]) H32(port->gpio_set) = set[i];
    if(clr[i]) H32(port->gpio_clr) = clr[i];
#endif

    port ++;
  }
}/*}}}*/
void gpio_setup(const gpio_t *pins, u32 m)/*{{{*/
{
  int i, j;
  u32 clr[PORT_COUNT], set[PORT_COUNT];
  u32 zclr[PORT_COUNT], zset[PORT_COUNT];
  u32 mask;

  XDEBUGF("setup m=%d", m);

  for(i = 0; i < PORT_COUNT; i ++) {
    clr[i] = 0;
    set[i] = 0;
    zclr[i] = 0;
    zset[i] = 0;
  }

  while(m --) {
    j = pins->io_port;
    mask = 1 << pins->io_pin;

    if(pins->io_dir == IO_OUT && pins->io_initial != IO_Z) {
      zset[j] |= mask;
      if(pins->io_inverted ^ pins->io_initial) {
        set[j] |= mask;
      } else {
        clr[j] |= mask;
      }
    } else {
      zclr[j] |= mask;
    }

    pins ++;
  }

  affect_ports(zclr, zset, clr, set);
}/*}}}*/
void gpio_direction(const gpio_t *pins, u32 m)/*{{{*/
{
  int i, j;
  u32 clr[PORT_COUNT], set[PORT_COUNT];
  u32 zclr[PORT_COUNT], zset[PORT_COUNT];
  u32 mask;

  XDEBUGF("direction m=%d", m);

  for(i = 0; i < PORT_COUNT; i ++) {
    clr[i] = 0;
    set[i] = 0;
    zclr[i] = 0;
    zset[i] = 0;
  }

  while(m --) {
    j = pins->io_port;
    mask = 1 << pins->io_pin;
    XDEBUGF("j=%d mask=%x io_dir=%d", j, mask, pins->io_dir);
        
    if(pins->io_dir == IO_OUT) {
      zset[j] |= mask;
    } else {
      zclr[j] |= mask;
    }

    pins ++;
  }

  XDEBUGF("affecting...");
  affect_ports(zclr, zset, clr, set);
}/*}}}*/
void gpio_output(const gpio_t *pins, u32 m, u32 value)/*{{{*/
{
  int i;
  u32 clr[PORT_COUNT], set[PORT_COUNT];
  u32 zclr[PORT_COUNT], zset[PORT_COUNT];
  const gpio_port_t *port;
  u8 j;
  u32 mask;

  for(i = 0; i < PORT_COUNT; i ++) {
    clr[i] = 0;
    set[i] = 0;
    zclr[i] = 0;
    zset[i] = 0;
  }

  while(m --) {
    j = pins->io_port;
    assert(j < PORT_COUNT);

    mask = 1 << pins->io_pin;

    if(pins->io_inverted ^ (value & 1)) {
      set[j] |= mask;
    } else {
      clr[j] |= mask;
    }

    value >>= 1;
    pins ++;
  }

  port = gpio_ports;
  affect_ports(zclr, zset, clr, set);
}/*}}}*/
u32 gpio_input(const gpio_t *pins, u32 m)/*{{{*/
{
  u8 j;
  u32 mask;
  u32 result;
  u32 inputs[PORT_COUNT];

  for(j = 0; j < PORT_COUNT; j ++) {
    inputs[j] = H32(gpio_ports[j].gpio_pin);
  }

  result = 0;

  while(m --) {
    j = pins->io_port;
    mask = 1 << pins->io_pin;
    result <<= 1;
    if(inputs[j] & mask) result |= 1;
    if(pins->io_inverted) result ^= 1;
    pins ++;
  }

  return result;
}/*}}}*/
