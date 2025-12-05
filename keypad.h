/* keypad.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include <types.h>

enum {
  KEYPAD_RIGHT,
  KEYPAD_UP,
  KEYPAD_LEFT,
  KEYPAD_DOWN,
  KEYPAD_COUNT
};

void keypad_init();
u8 keypad_get();
u8 keypad_get_raw();

#endif

