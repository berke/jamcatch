/* rotary.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lcd.h>
#include <timer.h>
#include <delay.h>
#include <gpio.h>
#include <string.h>
#include <debug.h>
#include <format.h>

#define HD44780_CLR      0x01
#define HD44780_HOME     0x02
#define HD44780_ENTMODE(inc, shift) (0x04|(inc? 0x02: 0)|(shift? 1: 0))
#define HD44780_DISPCTL(disp, cursor, blink) \
(0x08|(disp? 0x04: 0)|(cursor? 0x02: 0)|(blink? 1: 0))
#define HD44780_SHIFT(shift, right) \
(0x10|(shift? 0x08: 0)|(right? 0x04: 0))
#define HD44780_FNSET(if8bit, twoline, font8x10) \
(0x20|(if8bit? 0x10: 0)|(twoline? 0x08: 0)|(font8x10? 0x04: 0))
#define HD44780_CGADDR(addr) (0x40|(addr & 0x3f))
#define HD44780_DDADDR(addr) (0x80|(addr & 0x7f))
#define HD44780_BUSYFLAG 0x80

static char lcd_line_buffer[LCD_LINES * (LCD_COLUMNS + 1)];
char *lcd_lines[LCD_LINES];
u8 lcd_line_lengths[LCD_LINES];
static u8 lcd_line;
static u8 lcd_available;
static u8 lcd_error;
static u32 lcd_tries;

/* Special CG characters */

const u32 lcd_cgram[LCD_CGRAM_COUNT][LCD_CGRAM_LINES]= {
  {
    0x00100,
    0x01110,
    0x11111,
    0x00100,
    0x00100,
    0x00100,
    0x00100,
    0x00100,
  },
  {
    0x00100,
    0x00100,
    0x00100,
    0x00100,
    0x00100,
    0x11111,
    0x01110,
    0x00100,
  },
  {
    0x00000,
    0x00100,
    0x01100,
    0x11111,
    0x01100,
    0x00100,
    0x00000,
    0x00000,
  },
  {
    0x00000,
    0x00100,
    0x00110,
    0x11111,
    0x00110,
    0x00100,
    0x00000,
    0x00000,
  },

  {
    0x10000,
    0x10000,
    0x10000,
    0x10000,
    0x10000,
    0x10000,
    0x10000,
    0x10000,
  },

  {
    0x10100,
    0x10100,
    0x10100,
    0x10100,
    0x10100,
    0x10100,
    0x10100,
    0x10100,
  },

  {
    0x10101,
    0x10101,
    0x10101,
    0x10101,
    0x10101,
    0x10101,
    0x10101,
    0x10101,
  },

};

/* Order is important ! */

enum {
  LCD_DB4   = 0,
  LCD_DB5   = 1,
  LCD_DB6   = 2,
  LCD_DB7   = 3,
  LCD_RS    = 4,
  LCD_RW    = 5,
  LCD_E     = 6,
  LCD_COUNT = 7
};

const gpio_t lcd_input_pins[] = {
  { .io_port = LCD_PORT,
    .io_pin = LCD_DB7_PIN,
    .io_dir = IO_IN,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_DB6_PIN,
    .io_dir = IO_IN,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_DB5_PIN,
    .io_dir = IO_IN,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_DB4_PIN,
    .io_dir = IO_IN,
    .io_inverted = false,
    .io_initial = IO_L },
};

const gpio_t lcd_output_pins[] = {
  { .io_port = LCD_PORT,
    .io_pin = LCD_DB4_PIN,
    .io_dir = IO_OUT,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_DB5_PIN,
    .io_dir = IO_OUT,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_DB6_PIN,
    .io_dir = IO_OUT,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_DB7_PIN,
    .io_dir = IO_OUT,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_RS_PIN,
    .io_dir = IO_OUT,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_RW_PIN,
    .io_dir = IO_OUT,
    .io_inverted = false,
    .io_initial = IO_L },

  { .io_port = LCD_PORT,
    .io_pin = LCD_E_PIN,
    .io_dir = IO_OUT,
    .io_inverted = false,
    .io_initial = IO_L },
};

const char lcd_bargraph[] = CG03 CG13 CG23 CG33;

static void lcd_e_up(void)/*{{{*/
{
  gpio_output(lcd_output_pins + LCD_E, 1, 1);
}/*}}}*/
static void lcd_e_down(void)/*{{{*/
{
  gpio_output(lcd_output_pins + LCD_E, 1, 0);
}/*}}}*/
static void lcd_write_nibble(u8 x, u8 rs)/*{{{*/
{
  gpio_output(lcd_output_pins + LCD_RS, 2, (rs ? 1:0));
  delay_us(LCD_DELAY_US);
  lcd_e_up();
  delay_us(LCD_DELAY_US);
  gpio_output(lcd_output_pins, 6, x | (rs ? 0x10:0x00));
  delay_us(LCD_DELAY_US);
  lcd_e_down();
}/*}}}*/
static u8 lcd_read_byte(u8 rs)/*{{{*/
{
  u8 x;

  /* Set pins to input */
  gpio_direction(lcd_input_pins, 4);

  gpio_output(lcd_output_pins + LCD_RS, 2, 2 | (rs ? 1:0));

  delay_us(LCD_DELAY_US);

  lcd_e_up();
  delay_us(LCD_DELAY_US);
  x = gpio_input(lcd_input_pins, 4);
  lcd_e_down();
  delay_us(LCD_DELAY_US);

  x <<= 4;

  lcd_e_up();
  delay_us(LCD_DELAY_US);
  x |= gpio_input(lcd_input_pins, 4);
  lcd_e_down();
  delay_us(LCD_DELAY_US);

  /* Clear R/_W */
  gpio_output(lcd_output_pins + LCD_RW, 1, 0);
  /* Set pins to output */
  gpio_direction(lcd_output_pins + LCD_DB4, 4);

  return x;
}/*}}}*/
static void lcd_problem(void)/*{{{*/
{
  DEBUGF("LCD problem");
  lcd_error = true;
}/*}}}*/
static u8 lcd_wait_return_address(void)/*{{{*/
{
  u32 b;
  u32 count;

  count = 0;

  for(;;) {
    b = lcd_read_byte(0);
    if(!(b & HD44780_BUSYFLAG)) {
      return b;
    }
    count ++;
    if(count == LCD_TIMEOUT) {
      XDEBUGF("LCD timeout");
      lcd_problem();
    }
  }
}/*}}}*/
static void lcd_wait(void)/*{{{*/
{
  (void) lcd_wait_return_address();
  return;
}/*}}}*/
static void lcd_write_byte(u8 x, u8 rs)/*{{{*/
{
  lcd_write_nibble(x >> 4, rs);
  lcd_write_nibble(x & 15, rs);
  delay_us(LCD_DELAY_US);
}/*}}}*/
static void lcd_instruction(u8 x)/*{{{*/
{
  lcd_write_byte(x, 0);
}/*}}}*/
static void lcd_data(u8 x)/*{{{*/
{
  lcd_write_byte(x, 1);
}/*}}}*/
static void lcd_write_char(void *info, char c)/*{{{*/
{
  if(c == '\n') {
    lcd_line ++;
    if(lcd_line == LCD_LINES) lcd_line = 0;
    lcd_line_lengths[lcd_line] = 0;
  } else {
    if(lcd_line_lengths[lcd_line] < LCD_COLUMNS) {
      lcd_lines[lcd_line][lcd_line_lengths[lcd_line] ++] = c;
    }
  }
}/*}}}*/
static void lcd_cls(void)/*{{{*/
{
  lcd_instruction(HD44780_CLR);
  lcd_wait();
}/*}}}*/
void lcd_clear(void)/*{{{*/
{
  u32 i;

  for(i = 0; i < LCD_LINES; i ++) {
    lcd_line_lengths[i] = 0;
  }
  lcd_line = 0;
}/*}}}*/
static void lcd_write_data(u8 *u, u32 length)/*{{{*/
{
  while(length --) {
    lcd_data(*u);
    u ++;
    lcd_wait();
  }
}/*}}}*/
static u8 shrink_u32_to_u8(u32 x)/*{{{*/
{
  int i;
  u8 y;

  y = 0;

  for(i = 0; i < 8; i ++) {
    y <<= 1;
    if(x & 0xf0000000) y |= 1;
    x <<= 4;
  }

  return y;
}/*}}}*/
static bool lcd_init_cgram(void)/*{{{*/
{
  int i, j;
  u32 y;

  for(j = 0; j < LCD_CGRAM_COUNT; j ++) {
    lcd_instruction(HD44780_CGADDR(j * LCD_CGRAM_LINES));
    for(i = 0; i < LCD_CGRAM_LINES; i ++) {
      y = shrink_u32_to_u8(lcd_cgram[j][i]);
      lcd_data(y);
      lcd_wait();
    }
  }
  if(lcd_error) return false;

  return true;
}/*}}}*/
void lcd_init(void)/*{{{*/
{
  int i;

  gpio_setup(lcd_output_pins, LCD_COUNT);
  lcd_available = false;
  lcd_tries = 0;

  for(i = 0; i < LCD_LINES; i ++) {
    lcd_lines[i] = lcd_line_buffer + i * (LCD_COLUMNS + 1);
    lcd_lines[i][LCD_COLUMNS] = 0;
  }

  for(;;) {
    lcd_tries ++;
    if(lcd_tries == LCD_RETRIES) {
      DEBUGF("LCD not available");
      return;
    }
    lcd_error = false;

    delay_us(30000);
    lcd_write_nibble(HD44780_FNSET(1, 1, 0) >> 4, 0);
    delay_us(5000);
    lcd_write_nibble(HD44780_FNSET(1, 1, 0) >> 4, 0);
    delay_us(150);
    lcd_write_nibble(HD44780_FNSET(1, 1, 0) >> 4, 0);
    delay_us(150);

    lcd_write_nibble(HD44780_FNSET(0, 1, 0) >> 4, 0);
    lcd_wait();
    if(lcd_error) continue;

    lcd_instruction(HD44780_FNSET(0, 1, 0));
    lcd_wait();
    if(lcd_error) continue;

    lcd_instruction(HD44780_DISPCTL(1, 0, 0));
    lcd_wait();
    if(lcd_error) continue;

    lcd_instruction(HD44780_CLR);
    delay_us(100);
    //lcd_wait();
    //if(lcd_error) continue;

    lcd_instruction(HD44780_ENTMODE(1, 0));
    lcd_wait();
    if(lcd_error) continue;

    if(!lcd_init_cgram()) continue;
    if(lcd_error) continue;

    lcd_cls();
    if(lcd_error) continue;

    lcd_clear();
    if(lcd_error) continue;

    /* Check lcd */
    {
      u8 a1, a2;

      for(a1 = 0x00; a1 < 0x1f; a1 ++) {
        lcd_instruction(HD44780_DDADDR(a1));
        a2 = lcd_wait_return_address();
        if(a1 != a2) {
          DEBUGF("Mismatch a1=%x a2=%x", a1, a2);
          continue;
        }
      }
    }

    break;
  }

  lcd_available = true;
}/*}}}*/
void lcd_refresh(void)/*{{{*/
{
  u32 i;
  u8 space;

  if(lcd_available) {
    lcd_instruction(HD44780_DDADDR(0x00));
    if(lcd_line_lengths[0] > LCD_COLUMNS) {
      lcd_line_lengths[0] = LCD_COLUMNS;
      DEBUGF("lcd_line_lengths[0] = %d", lcd_line_lengths[0]);
    }
    lcd_write_data((u8 *) lcd_lines[0], lcd_line_lengths[0]);

    space = ' ';

    for(i = lcd_line_lengths[0]; i < LCD_COLUMNS; i ++) {
      lcd_write_data(&space, 1);
    }

    lcd_instruction(HD44780_DDADDR(0x40));
    if(lcd_line_lengths[1] > LCD_COLUMNS) {
      lcd_line_lengths[1] = LCD_COLUMNS;
      DEBUGF("lcd_line_lengths[1] = %d", lcd_line_lengths[1]);
    }
    lcd_write_data((u8 *) lcd_lines[1], lcd_line_lengths[1]);

    for(i = lcd_line_lengths[1]; i < LCD_COLUMNS; i ++) {
      lcd_write_data(&space, 1);
    }
  }
}/*}}}*/
void lcd_printf(const char *fmt, ...)/*{{{*/
{
  va_list va;
  struct writer_s wr;

  wr.wr_info = 0;
  wr.wr_putchar = lcd_write_char;

  va_start(va, fmt);
  format_aprintf(&wr, fmt, va);
  va_end(va);
}/*}}}*/
void lcd_print_bargraph(int value, int width)/*{{{*/
{
  int i, j;
  int cols;

  cols = (width + 2) / 3;

  i = 0;

  for(j = 0; j < value / 3; j ++) {
    lcd_write_char(0, lcd_bargraph[3]);
    i ++;
  }

  if(value % 3) {
    lcd_write_char(0, lcd_bargraph[value % 3]);
    i ++;
  }
  
  for(i = 0; i < cols; i ++) {
    lcd_write_char(0, lcd_bargraph[0]);
  }
}/*}}}*/
