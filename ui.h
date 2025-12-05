/* ui.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef UI_H
#define UI_H

#define UI_MESSAGE 1
#define UI_ALARM 2

#include <exec.h>

void ui_init(void);

void ui_message(char *msg);
void ui_message_async(question_node *qn, char *msg);
void ui_alarm_async(question_node *qn);

char *ui_alarm_threshold_notify(void *info);
char *ui_alarm_decay_notify(void *info);
char *ui_alarm_enabled_notify(void *info);
char *ui_date_year_notify(void *info);
char *ui_date_month_notify(void *info);
char *ui_date_day_notify(void *info);
char *ui_date_hours_notify(void *info);
char *ui_date_minutes_notify(void *info);
char *ui_date_and_time_enter(void *info);
char *ui_trigger_alarm(void *info);
char *ui_quit_menu(void *info);
char *ui_save_settings(void *info);
char *ui_load_settings(void *info);
char *ui_date_and_time_set(void *info);
char *ui_erase_log(void *info);
char *ui_reset_counter(void *info);
char *ui_view_log(void *info);
char *ui_readjust(void *info);
int ui_month_formatter(char *, int, int);
int ui_eas_threshold_formatter(char *, int, int);

extern port_node ui_port;

#endif
