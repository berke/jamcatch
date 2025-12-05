/* lcd.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef LCD_H
#define LCD_H

#include <lpc2138.h>
#include <config.h>
#include <timer.h>

enum {
  LCD_CGRAM_UP    = 8,
  LCD_CGRAM_DOWN  = 1,
  LCD_CGRAM_LEFT  = 2,
  LCD_CGRAM_RIGHT = 3,
  LCD_CGRAM_13    = 4,
  LCD_CGRAM_23    = 5,
  LCD_CGRAM_33    = 6,
  LCD_CGRAM_COUNT
};

#define CGUP    "\x08"
#define CGDOWN  "\x01"
#define CGLEFT  "\x02"
#define CGRIGHT "\x03"
#define CG03    " "
#define CG13    "\x04"
#define CG23    "\x05"
#define CG33    "\x06"

#define LCD_CGRAM_LINES 8

const char lcd_bargraph[4];
extern char *lcd_lines[LCD_LINES];
extern u8 lcd_line_lengths[LCD_LINES];

void lcd_init(void);
void lcd_printf(const char *fmt, ...);
void lcd_clear(void);
void lcd_refresh(void);
void lcd_print_bargraph(int value, int width);

#endif
