/* display.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <debug.h>
#include <display.h>
#include <config.h>
#include <exec.h>
#include <keypad.h>
#include <lcd.h>
#include <display.h>
#include <rtc.h>
#include <indicator.h>
#include <dsp_loop.h>
#include <watchdog.h>
#include <alarm.h>
#include <random.h>
#include <settings.h>
#include <string.h>
#include <log.h>
#include <disk.h>
#include <format.h>
#include <ui_menu.h>
#include <ui.h>

port_node ui_port;
task_node ui_task;
u32 ui_stack[UI_STACK_SIZE];

static display_t ui_display;

#if UI_WATCHDOG_ENABLE
static watchpoint_node ui_watchpoint;
#endif

static log_t log_buffer;
static disk_request_t dr;
static bool disk_ok;

#define UI_Z_BARS 12
#define BANNER_WIDTH 8

static char *banner;
static u8 banner_length;
static u8 banner_phase;
static int banner_reset_count;
static char banner_buf[BANNER_WIDTH + 1];
static bool banner_changed;
static bool banner_needs_update;

bool insert_disk(void)/*{{{*/
{
  XDEBUGF("Insert");
  if(exec_question_busy(&dr.dr_qn)) return false;
  
  log_copy(&log_buffer);
  dr.dr_qn.question.q_question = DISK_INSERT;
  dr.dr_qn.question.q_question_extra = (u32) &dr;
  exec_ask(&disk_port, &dr.dr_qn);
  return dr.dr_qn.question.q_answer == DISK_ANSWER_OK;
}/*}}}*/
void save_log(void)/*{{{*/
{
  if(!disk_ok) return;

  XDEBUGF("Saving");
  if(exec_question_busy(&dr.dr_qn)) {
    XDEBUGF("Can't save, busy!");
    return;
  }
  
  log_copy(&log_buffer);
  dr.dr_qn.question.q_question = DISK_WRITE;
  dr.dr_qn.question.q_question_extra = (u32) &dr;
  dr.dr_card = DISK_LOG_BLOCK;
  dr.dr_data = (u8 *) &log_buffer;
  dr.dr_size = sizeof(log_buffer);
  exec_send(&disk_port, &dr.dr_qn);
}/*}}}*/
void load_log(void)/*{{{*/
{
  if(!disk_ok) return;

  DEBUGF("Loading");
  if(exec_question_busy(&dr.dr_qn)) return;

  dr.dr_qn.question.q_question = DISK_READ;
  dr.dr_qn.question.q_question_extra = (u32) &dr;
  dr.dr_card = DISK_LOG_BLOCK;
  dr.dr_data = (u8 *) &log_buffer;
  dr.dr_size = sizeof(log_buffer);
  exec_ask(&disk_port, &dr.dr_qn);
  log_restore(&log_buffer);
}/*}}}*/
void ui_message(char *msg)/*{{{*/
{
  question_node qn;

  qn.question.q_question = UI_MESSAGE;
  qn.question.q_question_extra = (u32) msg;
  exec_question_init(&qn);
  exec_ask(&ui_port, &qn);
}/*}}}*/
void ui_message_async(question_node *qn, char *msg)/*{{{*/
{
  XDEBUGF("Message %s", msg);
  if(!exec_question_busy(qn)) {
    XDEBUGF("Not busy");
    qn->question.q_question = UI_MESSAGE;
    qn->question.q_question_extra = (u32) msg;
    exec_send(&ui_port, qn);
  }
}/*}}}*/
void ui_alarm_async(question_node *qn)/*{{{*/
{
  XDEBUGF("Alarm");
  if(!exec_question_busy(qn)) {
    XDEBUGF("Not busy");
    qn->question.q_question = UI_ALARM;
    qn->question.q_question_extra = 0;
    exec_send(&ui_port, qn);
  }
}/*}}}*/

static const char *month_names[] = {/*{{{*/
  "Unknown",
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December",
};/*}}}*/
int ui_month_formatter(char *dst, int size, int i)/*{{{*/
{
  if(i < 1 || i > 12) i = 0;
  return snprintf(dst, size, "%s", month_names[i]);
}/*}}}*/
int ui_eas_threshold_formatter(char *dst, int size, int i) {/*{{{*/
  int db;

  db = -96 + 6 * i;

  return snprintf(dst, size, " %ddB", db);
}/*}}}*/
static void key_wait(void) {/*{{{*/
  u32 repeat_counter;

  for(repeat_counter = 0; repeat_counter < UI_KEYPAD_REPEAT_INITIATE; repeat_counter++) {
    timer_wait(UI_PERIOD);
#if UI_WATCHDOG_ENABLE
    watchdog_rearm(&ui_watchpoint);
#endif
  }

  for(repeat_counter = 0; keypad_get() && repeat_counter < UI_KEYPAD_REPEAT; repeat_counter++) {
    timer_wait(UI_PERIOD);
#if UI_WATCHDOG_ENABLE
    watchdog_rearm(&ui_watchpoint);
#endif
  }
}/*}}}*/
char *ui_readjust(void *info)/*{{{*/
{
  detector_reset(detectors);
  return "Readjusting";
}/*}}}*/
char *ui_alarm_threshold_notify(void *info)/*{{{*/
{
  XDEBUGF("Alarm threshold %d", settings.st_alarm_threshold);
  return "Changed";
}/*}}}*/
char *ui_alarm_decay_notify(void *info) { return 0; }
char *ui_alarm_enabled_notify(void *info)/*{{{*/
{
  if(settings.st_alarm_enabled) {
    alarm_enable();
    return "Enabled";
  } else {
    alarm_disable();
    return "Disabled";
  }
}/*}}}*/
char *ui_date_year_notify(void *info) { return 0; }
char *ui_date_month_notify(void *info) { return 0; }
char *ui_date_day_notify(void *info) { return 0; }
char *ui_date_hours_notify(void *info) { return 0; }
char *ui_date_minutes_notify(void *info) { return 0; }
char *ui_trigger_alarm(void *info) { return 0; }
char *ui_quit_menu(void *info) { return 0; }
char *ui_save_settings(void *info) {/*{{{*/
  return settings_save() ?  "Settings saved" : "Error saving";
}/*}}}*/
char *ui_load_settings(void *info) {/*{{{*/
  return settings_load() ?  "Settings loaded" : "Error loading";
}/*}}}*/
char *ui_reset_counter(void *info) {/*{{{*/
  alarm_reset();
  return "Reset";
}/*}}}*/
char *ui_date_and_time_set(void *info)/*{{{*/
{
  time_t t;

  t.t_year = ui_date_year;
  t.t_month = ui_date_month;
  t.t_day = ui_date_day;
  t.t_hours = ui_date_hours;
  t.t_minutes = ui_date_minutes;
  t.t_seconds = 0;

  XDEBUGF("Setting date & time to %04u-%02u-%02u %02u:%02u:%02u",
         t.t_year, t.t_month, t.t_day, t.t_hours, t.t_minutes, t.t_seconds);
  rtc_set_time(&t);

  return "RTC adjusted";
}/*}}}*/
char *ui_date_and_time_enter(void *info)/*{{{*/
{
  time_t t;

  rtc_get_time(&t);
  ui_date_year = t.t_year;
  ui_date_month = t.t_month;
  ui_date_day = t.t_day;
  ui_date_hours = t.t_hours;
  ui_date_minutes = t.t_minutes;

  return 0;
}/*}}}*/
char *ui_erase_log(void *info) {/*{{{*/
  log_erase();
  save_log();
  return "Log erased";
}/*}}}*/
char *ui_view_log(void *info)/*{{{*/
{
  u16 i, j, m;
  time_t t;
  log_entry_t *e;
  u8 key;
  static log_t log2;

  log_copy(&log2);

  i = log2.log_index; /* Next entry / oldest entry */
  m = log2.log_count;
  j = m - 1;

  if(i == m) {
    i = m - 1;
  } else {
    if(i > 0) i --;
    else i = m - 1;
  }

  void update() {
    e = log2.log_entries + i;
    XDEBUGF("table=%p e=%p i=%d", &log2, e, i);
    rtc_decode(&t, e->log_timestamp);

    lcd_clear();
    lcd_printf("%04u-%02u-%02u %02u:%02u\n",
               t.t_year,
               t.t_month,
               t.t_day,
               t.t_hours,
               t.t_minutes);
    lcd_printf("%d/%d ", j + 1, m);
    switch(e->log_event_kind) {
      case EVENT_BOOT:
        lcd_printf("Boot");
        break;
      case EVENT_ALARM:
        lcd_printf("Alarm");
        break;
      default:
        lcd_printf("Unknown");
        break;
    }
    lcd_refresh();
  }

  if(!m) return "Log is empty";

  update();

  for(;;) {
    for(;;) {
      timer_wait(UI_PERIOD);
#if UI_WATCHDOG_ENABLE
      watchdog_rearm(&ui_watchpoint);
#endif
      key = keypad_get();
      if(key) break;
    }

    switch(key) {
      case (1 << KEYPAD_RIGHT):
        i ++; if(i == m) i = 0;
        j ++; if(j == m) j = 0;
        break;
      case (1 << KEYPAD_LEFT):
        if(!i) i = m - 1; else i --;
        if(!j) j = m - 1; else j --;
        break;
      case (1 << KEYPAD_UP):
        return 0;

      default:
        break;
    }

    update();

    key_wait();
  }
}/*}}}*/

void banner_set(char *msg) {/*{{{*/
  u8 m;

  m = string_length(msg);
  XDEBUGF("Set m=%d %s", m, msg);
  banner_length = m;
  if(msg != banner || banner_phase >= m) banner_phase =0;
  banner = msg;
  banner_reset_count = timer_get_counter() + UI_MESSAGE_DURATION;
  banner_changed = true;
}/*}}}*/
void banner_init(void) {/*{{{*/
  banner = 0;
  banner_phase = 0;
  banner_length = 0;
  banner_set(UI_DEFAULT_BANNER);
}/*}}}*/
void banner_step() {/*{{{*/
  u32 i, j;

  XDEBUGF("Step");
  if(banner_reset_count && banner_reset_count < timer_get_counter()) {
    banner_set(UI_DEFAULT_BANNER);
    banner_reset_count = 0;
  }

  if(banner_length > BANNER_WIDTH) {
    j = banner_phase;
    for(i = 0; i < BANNER_WIDTH; i ++) {
      banner_buf[i] = banner[j];
      j ++;
      if(j == banner_length) j = 0;
    }
    banner_buf[i] = 0;

    banner_phase ++;
    if(banner_phase == banner_length) banner_phase = 0;
    banner_needs_update = true;
  } else if(banner_changed) {
    for(i = 0; i < banner_length; i ++) {
      banner_buf[i] = banner[i];
    }

    for(; i < BANNER_WIDTH; i ++) {
      banner_buf[i] = ' ';
    }
    banner_buf[i] = 0;
    banner_changed = false;
    banner_needs_update = true;
  }
}/*}}}*/
void ui_entry(void *a)/*{{{*/
{
  u8 key;
  bool idle;
  u32 idle_count;
  u32 last_seconds;
  u32 last_z;
  u32 last_alarms;
  u32 i;
  question_node *qn_request;
  int banner_timer;

  DEBUGF("Entry");
  void update() {/*{{{*/
    display_update(&ui_display, lcd_lines[0], lcd_lines[1], LCD_COLUMNS);
    lcd_line_lengths[0] = LCD_COLUMNS;
    lcd_line_lengths[1] = LCD_COLUMNS;
    lcd_refresh();
    XDEBUGF("LCD 0: %s", lcd_lines[0]);
    XDEBUGF("LCD 1: %s", lcd_lines[1]);
  }/*}}}*/
  void idle_display() {/*{{{*/
    time_t t;
    char colon;
    u32 z;
    u32 alarms;

    rtc_get_time(&t);
    alarms = indicator_u32_get(&alarm_count);
    z = float32_to_int32(indicator_f32_get(&dsp_td_z_score));

    if(banner_needs_update || t.t_seconds != last_seconds || alarms != last_alarms || z != last_z) {
      banner_needs_update = false;
      last_seconds = t.t_seconds;
      last_alarms = alarms;
      last_z = z;
      lcd_clear();

      if(z < 0) z = -z;
      if(z > UI_Z_BARS) z = UI_Z_BARS;

      /* 0123456789012345 */
      /* Jamcatch 234 ||| */
      lcd_printf("%s %03u ", banner_buf, alarms);
      lcd_print_bargraph(z, UI_Z_BARS);
      lcd_printf("\n");

      colon = t.t_seconds & 1 ? ' ' : ':';
      lcd_printf("%04u-%02u-%02u %02u%c%02u",
                 t.t_year,
                 t.t_month,
                 t.t_day,
                 t.t_hours,
                 colon,
                 t.t_minutes);
      lcd_refresh();
    }
  }/*}}}*/
  void demo() {/*{{{*/
    random_t rnd;

    random_init(&rnd);

    for(i = 0; i <= LCD_COLUMNS * 3;) {
      lcd_clear();
      lcd_printf(UI_LCD_BANNER1 "\n");
      lcd_print_bargraph(i, LCD_COLUMNS * 3);
      lcd_refresh();
      timer_wait(random_get(&rnd) % UI_DEMO_PERIOD);
      i += random_get(&rnd) % 4;
    }

    lcd_clear();
    lcd_printf(UI_LCD_BANNER1 "\n");
    lcd_printf(UI_LCD_BANNER2);
    lcd_refresh();
    timer_wait(UI_BANNER_DURATION);
  }/*}}}*/
  void setup() {/*{{{*/
    /* Setup default values */
    display_init(&ui_display, &ui_menu_root, CGDOWN CGUP CGLEFT CGRIGHT "  *");

    /* Merge settings from disk */
    lcd_clear();
    lcd_printf("Checking SD/MMC\n");
    lcd_refresh();

    if(insert_disk()) {
      lcd_clear();
      lcd_printf("Loading settings\n");
      lcd_refresh();
      disk_ok = true;
      settings_init();
      settings_dump();
    } else {
      lcd_clear();
      lcd_printf("SD/MMC error\n");
      lcd_printf("Check card");
      lcd_refresh();
      timer_wait(UI_MESSAGE_DURATION);
      disk_ok = false;
      settings_default();
    }

    /* DSP loop */
    DEBUGF("DSP loop start");
    dsp_loop_start();
    DEBUGF("DSP Loop");

    /* Notify */
    display_notify(&ui_display, &ui_menu_root);
  }/*}}}*/
  void event_loop() {/*{{{*/
    for(;;) {
      /* Check for asynchronous requests {{{*/
      qn_request = exec_check_question(&ui_port);
      if(qn_request) {
        XDEBUGF("Got request");
        switch(qn_request->question.q_question) {
          case UI_MESSAGE:
            banner_set((char *) qn_request->question.q_question_extra);
            qn_request->question.q_answer = 0;
            exec_reply(&ui_port, qn_request, true);

            break;
          case UI_ALARM:
            qn_request->question.q_answer = 0;
            exec_reply(&ui_port, qn_request, false);

            banner_set("ALARM");
            log_add(EVENT_ALARM, qn_request->question.q_question_extra);
            save_log();
            XDEBUGF("Log saved");
            break;
          default:
            qn_request->question.q_answer = 0;
            exec_reply(&ui_port, qn_request, true);

            break;
        }
        XDEBUGF("Replied");
      }
      /*}}}*/
      /* Refresh {{{*/
      timer_wait(UI_PERIOD);
#if UI_WATCHDOG_ENABLE
      watchdog_rearm(&ui_watchpoint);
#endif
      /*}}}*/
      /* Check for a key {{{*/
      key = keypad_get();
      if(key) {
        /* A key was pressed */
        idle_count = 0; /* Reset the idle count */
        if(idle) {
          /* If we were idling, show the menu. */
          display_set_menu(&ui_display, &ui_menu_root);
          idle = false;
        } else {
          XDEBUGF("Key %x", key);
          bool more = false;
          switch(key) {
            case (1 << KEYPAD_RIGHT):
              more = display_apply(&ui_display, DK_RIGHT);
              break;
            case (1 << KEYPAD_LEFT):
              more = display_apply(&ui_display, DK_LEFT);
              break;
            case (1 << KEYPAD_UP):
              more = display_apply(&ui_display, DK_UP);
              break;
            case (1 << KEYPAD_DOWN):
              more = display_apply(&ui_display, DK_DOWN);
              break;
            default:
              break;
          }
          if(!more) idle = true;
        }
        update();   /* Update the view */
        key_wait(); /* Wait for key release */
      } else {
        /* No key */
        if(!idle) {
          /* We were not idle. */
          idle_count ++;
          if(idle_count >= UI_IDLE_COUNT) {
            idle = true;
          }
        }
        if(idle) {
          /* We were, or just became idle. */
          banner_timer += UI_PERIOD;
          if(banner_timer > UI_BANNER_PERIOD) {
            banner_timer -= UI_BANNER_PERIOD;
            banner_step();  /* Step the banner */
          }
          idle_display(); /* Do the display */
        }
      }
      /*}}}*/
    }
  }/*}}}*/

  exec_question_init(&dr.dr_qn);
  DEBUGF("Disk request initialized");

  demo();
  DEBUGF("Demo OK");
  setup();
  DEBUGF("Setted up");
  load_log();
  DEBUGF("Log loaded");
  log_add(EVENT_BOOT, 0);
  DEBUGF("Boot added");
  save_log();
  DEBUGF("Log saved");

  banner_init();
  DEBUGF("Banner init");
  banner_timer = 0;
  banner_step();
  DEBUGF("Banner step");

  idle = true;
  idle_count = 0;
  last_seconds = 60;

  idle_display();
  DEBUGF("Idle display");
  event_loop();
  DEBUGF("Event loop");

}/*}}}*/
void ui_init(void)/*{{{*/
{
#if UI_WATCHDOG_ENABLE
  watchdog_register(&ui_watchpoint, "ui");
#endif
  exec_add_task(&ui_task, "ui", ui_entry, 0, ui_stack, length(ui_stack), UI_PRI);
  exec_add_port(&ui_port, "ui", &ui_task, 0);
}/*}}}*/

