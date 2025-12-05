/* rtc.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef RTC_H
#define RTC_H

#include <lpc2138.h>
#include <config.h>

typedef struct {
  u16 t_year;   /* 0000..9999 */
  u8 t_month;   /* 1..12 */
  u8 t_day;     /* 1..31 */
  u8 t_hours;   /* 0..23 */
  u8 t_minutes; /* 0..59 */
  u8 t_seconds; /* 0..59 */
} time_t;

void rtc_init(void);
void rtc_get_time(time_t *t);
void rtc_set_time(time_t *t);
void rtc_decode(time_t *t, u32 s);
u32 rtc_encode(time_t *t);

#endif
