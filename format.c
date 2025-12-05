/* format.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <format.h>

inline void format_write_char(writer_t wr, char c)/*{{{*/
{
  wr->wr_putchar(wr->wr_info, c);
}/*}}}*/
void format_write_string(writer_t wr, const char *u)/*{{{*/
{
  while(*u) {
    format_write_char(wr, *u);
    u ++;
  }
}/*}}}*/
void format_write_word_dec_zb(writer_t wr, u32 x, int num_digits, int zero_blanking)/*{{{*/
{
  char digits[13];
  void f(char c) { format_write_char(wr, c); }

  if(num_digits > length(digits) - 1) num_digits = length(digits) - 1;

  if(zero_blanking && !x) {
    f('0');
  } else {
    int i;

    i = num_digits;
    digits[i] = 0;

    for(;;) {
      i --;
      digits[i] = '0' + (x % 10);
      x /= 10;
      if((zero_blanking && !x) || !i) break;
    }
    format_write_string(wr, digits + i);
  }
}/*}}}*/
void format_write_word_dec(writer_t wr, u32 x)/*{{{*/
{
  return format_write_word_dec_zb(wr, x, 12, 1);
}/*}}}*/
void format_write_word_signed_dec(writer_t wr, s32 y)/*{{{*/
{
  if(y < 0) {
    format_write_char(wr, '-');
    if(y == 0x80000000) {
      format_write_string(wr, "2147483648");
    } else format_write_word_dec(wr, (u32) (- y));
  } else {
    format_write_word_dec(wr, (u32) y);
  }
}/*}}}*/
void format_write_substring(writer_t wr, const char *u, int m)/*{{{*/
{
  while(*u && m--) {
    format_write_char(wr, *u);
    u ++;
  }
}
/*}}}*/
void format_write_word_hex(writer_t wr, u32 x)/*{{{*/
{
  int i;
  u8 y;
  for(i = 0; i < 8; i ++) {
    y = x >> 28;
    x <<= 4;
    y = y + 48;
    if(y >= 58) y += 39;
    format_write_char(wr, y);
  }
}/*}}}*/

enum {
  Q_NOTHING,
  Q_PERCENT,
  Q_ARG,
};

void format_aprintf(writer_t wr, const char *u, va_list va)/*{{{*/
{
  char c;
  int q;
  u32 x;
  int y;
  int i;
  const char *z;
  void f(char c) { format_write_char(wr, c); }
  u32 arg;
  bool zero;

  q = Q_NOTHING;
  i = 0;
  arg = 0;
  zero = false;

  for(;;) {
    c = *u;
    u ++;
    if(q != Q_NOTHING) {
      switch(c) {
        case 0:
          return;
        case '0':
          if(q == Q_PERCENT) zero = true;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          arg *= 10;
          arg += c & 15;
          q = Q_ARG;
          continue;
        case 'l':
          c = *u;
          u ++;
          switch(c) {
            case 0:
              return;
            case 'f':
              /* Crude conversion */
              {
                s32 a;
                float64 x;
                float64 one, zero, digits;

                x = va_arg(va, float64);

                a = float64_to_int32_round_to_zero(x);
                format_write_word_signed_dec(wr, a);

                if(a < 0) {
                  zero = int32_to_float64(0);
                  x = float64_sub(zero, x);
                }
                one = int32_to_float64(1);
                x = float64_rem(x, one);
                digits = int32_to_float64(100000000);
                x = float64_mul(x, digits);
                a = float64_to_int32(x);

                f('.');
                format_write_word_dec_zb(wr, a, 8, 0);
              }
              break;

            default:
              goto unknown;
          }
          break;
        case 'f':
          /* Crude conversion */
          {
            s32 a;
            float32 x;
            float32 one, zero, digits;

            x = va_arg(va, float32);

            a = float32_to_int32_round_to_zero(x);
            format_write_word_signed_dec(wr, a);

            if(a < 0) {
              zero = int32_to_float32(0);
              x = float32_sub(zero, x);
            }
            one = int32_to_float32(1);
            x = float32_rem(x, one);
            digits = int32_to_float32(100000000);
            x = float32_mul(x, digits);
            a = float32_to_int32(x);

            f('.');
            format_write_word_dec_zb(wr, a, 8, 0);
          }
          break;
        case 'd':
          y = va_arg(va, int);
          format_write_word_signed_dec(wr, y);
          break;
        case 'u':
          x = va_arg(va, u32);
          if(arg) {
            format_write_word_dec_zb(wr, x, arg, !zero);
          } else {
            format_write_word_dec(wr, x);
          }
          break;
        case 'p':
          x = va_arg(va, u32);
          format_write_string(wr, "0x");
          format_write_word_hex(wr, x);
          break;
        case 'x':
          x = va_arg(va, u32);
          format_write_word_hex(wr, x);
          break;
        case 'c':
          x = va_arg(va, int);
          f(x);
          break;
        case 's':
          z = va_arg(va, const char *);
          if(z) {
            format_write_string(wr, z);
          } else {
            format_write_string(wr, "(null)");
          }
          break;
        case 'i':
          /* Fix-point */
          y = va_arg(va, int);
          if(y < 0) {
            f('-');
            if(y == 0x80000000) {
              format_write_word_dec(wr, -(y >> FIX));
              format_write_string(wr, ".00000000");
              break;
            }
            y = -y;
          }
          format_write_word_dec(wr, y >> FIX);
          format_write_char(wr, '.');
          {
            int i;

            y &= (1 << FIX) - 1;
            for(i = 0; i < 8; i ++) {
              y *= 10;
              format_write_char(wr, '0' + (y >> FIX));
              y &= (1 << FIX) - 1;
            }
          }
          break;
        case '%':
          format_write_char(wr, '%');
          break;
        default:
        unknown:
          f('?');
          f(c);
      }
      q = Q_NOTHING;
    } else {
      switch(c) {
        case '%':
          q = Q_PERCENT;
          arg = 0;
          zero = false;
          break;
        case 0:
          return;
        default:
          f(c);
          break;
      }
    }
  }
  return;
}
/*}}}*/
void format_printf(writer_t wr, const char *u, ...)/*{{{*/
{
  va_list va;
  va_start(va, u);
  format_aprintf(wr, u, va);
  va_end(va);
}/*}}}*/
struct wrbuf_s {/*{{{*/
  char *wb_buf;
  u32 wb_remaining;
};/*}}}*/
void wrbuf_putchar(void *info, char c)/*{{{*/
{
  struct wrbuf_s *wb;

  wb = (struct wrbuf_s *) info;

  if(wb->wb_remaining) {
    *wb->wb_buf = c;
    wb->wb_buf ++;
    wb->wb_remaining --;
  }
}/*}}}*/
u32 snprintf(char *buf, u32 size, const char *fmt, ...)/*{{{*/
{
  va_list va;
  struct writer_s wr;
  struct wrbuf_s wb;

  wb.wb_buf = buf;
  wb.wb_remaining = size - 1;
  wr.wr_info = &wb;
  wr.wr_putchar = wrbuf_putchar;

  va_start(va, fmt);
  format_aprintf(&wr, fmt, va);
  va_end(va);

  *wb.wb_buf = 0;

  return wb.wb_buf - buf;
}/*}}}*/
