/* rtc.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <rtc.h>
#include <delay.h>
#include <debug.h>

void rtc_get_time(time_t *t)/*{{{*/
{
  CTIME0_R t0;
  CTIME1_R t1;

  t0 = CTIME0_B;
  t1 = CTIME1_B;

  t->t_seconds = t0.C_SECONDS;
  t->t_minutes = t0.C_MINUTES;
  t->t_hours   = t0.C_HOURS;

  t->t_day     = t1.C_DAY_OF_MONTH;
  t->t_month   = t1.C_MONTH;
  t->t_year    = t1.C_YEAR;
}/*}}}*/

u32 rtc_encode(time_t *t) {/*{{{*/
  u32 s;

  s = t->t_year - 2000;

  s *= 12;
  s += t->t_month - 1;

  s *= 31;
  s += t->t_day - 1;

  s *= 24;
  s += t->t_hours;

  s *= 60;
  s += t->t_minutes;

  s *= 60;
  s += t->t_seconds;

  return s;
}/*}}}*/
void rtc_decode(time_t *t, u32 s) {/*{{{*/
  t->t_seconds = s % 60;
  s /= 60;

  t->t_minutes = s % 60;
  s /= 60;

  t->t_hours = s % 24;
  s /= 24;

  t->t_day = 1 + s % 31;
  s /= 31;

  t->t_month = 1 + s % 12;
  s /= 12;

  t->t_year = s + 2000;
}/*}}}*/
void rtc_set_time(time_t *t)/*{{{*/
{
  CCR_B.CLKEN = 0;

  CCR_B.CTCRST = 1;
  delay_us(1);
  CCR_B.CTCRST = 0;
  CCR_B.CLKSRC = 1;

  SEC = t->t_seconds;
  MIN = t->t_minutes;
  HOUR = t->t_hours;

  DOM = t->t_day;
  MONTH = t->t_month;
  YEAR = t->t_year;

  CCR_B.CLKEN = 1;
}/*}}}*/
bool rtc_is_time_valid(time_t *t)/*{{{*/
{
  return
    !CCR_B.CTCRST &&
    CCR_B.CLKSRC &&
    t->t_seconds < 60 &&
    t->t_minutes < 60 &&
    t->t_hours < 24 &&
    t->t_day <= 31 &&
    t->t_month <= 12 &&
    t->t_year <= 2099 &&
    t->t_year >= 2007;
}/*}}}*/
void rtc_init(void)/*{{{*/
{
  time_t t;

  rtc_get_time(&t);
  if(!rtc_is_time_valid(&t)) {
    t.t_seconds = 0;
    t.t_minutes = 0;
    t.t_hours = 0;
    t.t_day = 1;
    t.t_month = 1;
    t.t_year = 2007;
    DEBUGF("Invalid");
    rtc_set_time(&t); /* Will set CLKEN to 1 */
  } else {
    DEBUGF("Valid");
    CCR_B.CLKEN = 1;
  }
}/*}}}*/
