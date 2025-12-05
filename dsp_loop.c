/* dsp_loop.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <types.h>
#include <sinus.h>
#include <adc.h>
#include <dac.h>
#include <serial.h>
#include <exec.h>
#include <timer.h>
#include <debug.h>
#include <config.h>
#include <indicator.h>
#include <string.h>
#include <fix.h>
#include <dsp_loop.h>
#include <fft.h>
#include <alarm.h>
#include <detect.h>
#include <watchdog.h>
#include <led.h>
#include <ui_menu.h>
#include <ui.h>
#include <random.h>

port_node dsp_port;
detector_t detectors[CHANNELS];

static task_node dsp_task;
static sigmask_t dsp_start_signal;
static u32 dsp_stack[DSP_STACK_SIZE];
static fix_t dsp_buffer0[ADC_BUFFER_LENGTH];
static watchpoint_node dsp_watchpoint;

indicator_node dsp_overrun_indicator;
indicator_node dsp_eas_level;
indicator_node dsp_frame_count;
indicator_node dsp_z_score;
indicator_node dsp_dc_z_score;
indicator_node dsp_td_value;
indicator_node dsp_td_z_score;
indicator_node dsp_total_z_score;
indicator_node dsp_adjuster_score;
indicator_node dsp_alarm;
indicator_node dsp_alarm_threshold;
indicator_node dsp_dot_frame_count;
indicator_node dsp_dot;

static question_node qn_ui_request;
static random_t indicator_rnd;

static const fix_t eas_threshold_table[] = {
  /* #0 -96.000000 */ F(0.000016),
  /* #1 -90.000000 */ F(0.000032),
  /* #2 -84.000000 */ F(0.000063),
  /* #3 -78.000000 */ F(0.000126),
  /* #4 -72.000000 */ F(0.000251),
  /* #5 -66.000000 */ F(0.000501),
  /* #6 -60.000000 */ F(0.001000),
  /* #7 -54.000000 */ F(0.001995),
  /* #8 -48.000000 */ F(0.003981),
  /* #9 -42.000000 */ F(0.007943),
  /* #10 -36.000000 */ F(0.015849),
  /* #11 -30.000000 */ F(0.031623),
  /* #12 -24.000000 */ F(0.063096),
  /* #13 -18.000000 */ F(0.125893),
  /* #14 -12.000000 */ F(0.251189),
  /* #15 -6.000000 */ F(0.501187),
};

static inline void dsp_process(detector_t *d, u16 *data)/*{{{*/
{
  int i;
  float32 at;
  bool alarm;

  indicator_u32_update(&dsp_frame_count, indicator_u32_get(&dsp_frame_count) + 1);

#if 0
  if(settings.st_alarm_threshold >= sizeof(threshold_table) / sizeof(*threshold_table)) {
    DEBUGF("Alarm threshold %x out of range", settings.st_alarm_threshold);
    settings.st_alarm_threshold = 0;
  }
#endif
  at = int32_to_float32(1 << settings.st_alarm_threshold);
  at = float32_div(at, int32_to_float32(65536));
  detectors[0].dt_alarm_threshold = at;
  indicator_f32_update(&dsp_alarm_threshold, detectors[0].dt_alarm_threshold);

  if(settings.st_eas_threshold >= sizeof(eas_threshold_table) / sizeof(*eas_threshold_table)) {
    DEBUGF("EAS threshold %x out of range", settings.st_eas_threshold);
    settings.st_eas_threshold = 0;
  }
  detectors[0].dt_eas_threshold = eas_threshold_table[settings.st_eas_threshold];

  if(settings.st_alarm_decay < 0 || settings.st_alarm_decay >= 30)
    settings.st_alarm_decay = 0;

  detectors[0].dt_alarm_decay = 
    float32_div(int32_to_float32(ALARM_DECAY_MAX - settings.st_alarm_decay), int32_to_float32(ALARM_DECAY_MAX));

#if 0
  /* Synthetic watchdog bug */
  if(indicator_u32_get(&dsp_frame_count) == 100) {
    for(;;); /* BUG */
  }
#endif

  /* Convert */
  for(i = 0; i < ADC_BUFFER_LENGTH; i++) {
    dsp_buffer0[i] = adc_du16_to_s16(data[i]) << (FIX - 16);
  }

  alarm = detector_process_frame(d, dsp_buffer0);

  indicator_s32_update(&dsp_eas_level, d->dt_eas_level);
  indicator_f32_update(&dsp_z_score, d->dt_z_score);
  indicator_f32_update(&dsp_dc_z_score, d->dt_dc_z_score);
  indicator_f32_update(&dsp_td_z_score, d->dt_td_z_score);
  indicator_fix_update(&dsp_td_value, d->dt_td_value);
  indicator_f32_update(&dsp_total_z_score, d->dt_total_z_score);
  indicator_f32_update(&dsp_adjuster_score, d->dt_adjuster_z);
  indicator_f32_update(&dsp_alarm, d->dt_alarm_filter);
  indicator_u32_update(&dsp_dot_frame_count, d->dt_total);
  indicator_fix_update(&dsp_dot, d->dt_last_dot);

  {
    u32 x, y;

    /* x : 0 .. 255(+) */
    x = d->dt_last_dot >> (FIX - 12);
    y = ((random_get(&indicator_rnd) & 255) * settings.st_indicator_threshold);
    /* y : 0 .. 2550 */
    if(x > y) {
      led_set(&led_b);
    } else {
      led_clr(&led_b);
    }
  }

  if(alarm_active()) {
    detector_set_pause(d, true);
  } else {
    detector_set_pause(d, false);

    /* Trigger any alarms */
    if(alarm) {
      XDEBUGF("Alarm");
      alarm_trigger();
      ui_alarm_async(&qn_ui_request);
    } else {
      /* XXX */
      if(d->dt_state_change) {
        d->dt_state_change = false;
        switch(d->dt_state) {
          case DETECTOR_RUN:
            led_set(&led_status);
            ui_message_async(&qn_ui_request, "Ready");
            break;
          case DETECTOR_LEARN:
            led_pattern(&led_status, LED_FAST_BLINK, true);
            ui_message_async(&qn_ui_request, "Observing - ");
            break;
          case DETECTOR_ADJUST:
            led_pattern(&led_status, LED_SLOW_BLINK, true);
            ui_message_async(&qn_ui_request, "Auto-adjusting... - ");
            break;
          case DETECTOR_EAS_WAIT:
            ui_message_async(&qn_ui_request, "Waiting for EAS - ");
            led_clr(&led_status);
            break;
          case DETECTOR_EAS_LOST:
            ui_message_async(&qn_ui_request, "No EAS");
            led_clr(&led_status);
            break;
          default:
            led_clr(&led_status);
            break;
        }
      }
    }
  }
}/*}}}*/
void dsp_entry(void *arg)/*{{{*/
{
  question_node qn;
  question_node *qn_request;
  u16 *data;
  u32 result;

  exec_question_init(&qn);
  exec_question_init(&qn_ui_request);

  DEBUGF("DSP waiting for signal");
  exec_wait(dsp_start_signal);
  DEBUGF("DSP got signal");

  led_clr(&led_status);
  for(;;) {
    watchdog_rearm(&dsp_watchpoint);
    result = adc_get_buffer(&qn, &data);
    if(result == ADC_OVERRUN) {
      led_pattern(&led_b, LED_FLASH, false);
      indicator_u32_update(&dsp_overrun_indicator, indicator_u32_get(&dsp_overrun_indicator) + 1);
    }
    dsp_process(detectors + 0, data);

    /* Got any data requests ? */
    qn_request = exec_check_question(&dsp_port);
    if(qn_request) {
      XDEBUGF("Got request");
      switch(qn_request->question.q_question) {
        case DSP_DATA_REQUEST:
          XDEBUGF("Will copy to %x", qn_request->question.q_question_extra);
          word_copy(dsp_buffer0, (fix_t *) qn_request->question.q_question_extra, sizeof(dsp_buffer0));
          qn_request->question.q_answer = 0;
          XDEBUGF("Request done");
          break;
        case DSP_FFT_REQUEST:
          /*XDEBUGF("Will copy FFT to %x", qn_request->question.q_question_extra);
          word_copy(dsp_fft_buffer, (fix_t *) qn_request->question.q_question_extra, sizeof(dsp_fft_buffer));
          qn_request->question.q_answer = 0;
          XDEBUGF("Request done");*/
          break;
        default:
          XDEBUGF("Unknown request %x", qn_request->question.q_question);
          qn_request->question.q_answer = (u32) "Unknown request";
          break;
      }
      exec_reply(&dsp_port, qn_request, 1);
      XDEBUGF("Replied");
    }
  }
}/*}}}*/
void dsp_loop_start(void)/*{{{*/
{
  DEBUGF("Register");
  watchdog_register(&dsp_watchpoint, "dsp");
  DEBUGF("Signal");
  exec_signal(&dsp_task, dsp_start_signal);
}/*}}}*/
void dsp_loop_init(void)/*{{{*/
{
  int i;

  exec_add_task(&dsp_task, "dsp", dsp_entry, 0, dsp_stack, length(dsp_stack), DSP_PRI);
  exec_add_port(&dsp_port, "dsp", &dsp_task, 0);
  dsp_start_signal = exec_alloc_signal(&dsp_task);
  DEBUGF("Allocated signal %d", dsp_start_signal);

  for(i = 0; i < CHANNELS; i ++) {
    detector_init(detectors + i);
  }

  indicator_u32_init(&dsp_frame_count);
  indicator_add(&dsp_frame_count, "d.fr");

  indicator_u32_init(&dsp_dot_frame_count);
  indicator_add(&dsp_dot_frame_count, "d.dfr");

  indicator_u32_init(&dsp_overrun_indicator);
  indicator_add(&dsp_overrun_indicator, "d.or");

  indicator_s32_init(&dsp_eas_level);
  indicator_add(&dsp_eas_level, "d.eas");

  indicator_f32_init(&dsp_z_score);
  indicator_add(&dsp_z_score, "d.z");

  indicator_f32_init(&dsp_dc_z_score);
  indicator_add(&dsp_dc_z_score, "d.dcz");

  indicator_fix_init(&dsp_td_value);
  indicator_add(&dsp_td_value, "d.td");

  indicator_f32_init(&dsp_td_z_score);
  indicator_add(&dsp_td_z_score, "d.tdz");

  indicator_f32_init(&dsp_total_z_score);
  indicator_add(&dsp_total_z_score, "d.tz");

  indicator_f32_init(&dsp_adjuster_score);
  indicator_add(&dsp_adjuster_score, "d.adj");

  indicator_f32_init(&dsp_alarm);
  indicator_add(&dsp_alarm, "d.a");

  indicator_f32_init(&dsp_alarm_threshold);
  indicator_add(&dsp_alarm_threshold, "d.at");

  indicator_fix_init(&dsp_dot);
  indicator_add(&dsp_dot, "d.dot");
}/*}}}*/
