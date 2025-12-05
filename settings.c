/* settings.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <settings.h>
#include <disk.h>
#include <debug.h>
#include <serial.h>
#include <config.h>
#include <string.h>

static disk_request_t dr;
settings_t settings;

void settings_init()/*{{{*/
{
  if(!settings_load()) {
    settings_default();
    settings_save();
  }
}/*}}}*/
void settings_copy(settings_t *dst){/*{{{*/
  word_copy((u32 *) &settings, (u32 *) dst, sizeof(settings_t));
}/*}}}*/
void settings_restore(settings_t *src){/*{{{*/
  if(src->st_version != SETTINGS_VERSION) {
    DEBUGF("Settings version mismatch, got %d", src->st_version);
    settings_default();
  }
  word_copy((u32 *) src, (u32 *) &settings, sizeof(settings_t));
}/*}}}*/
void settings_default()/*{{{*/
{
  DEBUGF("Default");
  settings.st_version = SETTINGS_VERSION;
  settings.st_alarm_threshold = ALARM_THRESHOLD;
  settings.st_alarm_decay = ALARM_DECAY;
  settings.st_alarm_enabled = ALARM_ENABLED;
  settings.st_alarm_count = 0;
  settings.st_eas_threshold = EAS_THRESHOLD;
}/*}}}*/
void settings_dump(void) {/*{{{*/
  serial_printf("Settings:" CRLF " at=%u" CRLF " decay=%d" CRLF " enabled=%d" CRLF " count=%d" CRLF,
                settings.st_alarm_threshold,
                settings.st_alarm_decay,
                settings.st_alarm_enabled,
                settings.st_alarm_count);
}/*}}}*/
bool settings_load(void)/*{{{*/
{
  bool result;

  DEBUGF("Loading");
  exec_question_init(&dr.dr_qn);
  dr.dr_qn.question.q_question = DISK_READ;
  dr.dr_qn.question.q_question_extra = (u32) &dr;
  dr.dr_card = DISK_SETTINGS_BLOCK;
  dr.dr_data = (u8 *) &settings;
  dr.dr_size = sizeof(settings);
  exec_ask(&disk_port, &dr.dr_qn);
  result = dr.dr_qn.question.q_answer == DISK_ANSWER_OK;
  exec_question_release(&dr.dr_qn);
  return result;
}/*}}}*/
bool settings_save(void)/*{{{*/
{
  bool result;

  DEBUGF("Saving");
  exec_question_init(&dr.dr_qn);
  dr.dr_qn.question.q_question = DISK_WRITE;
  dr.dr_qn.question.q_question_extra = (u32) &dr;
  dr.dr_card = DISK_SETTINGS_BLOCK;
  dr.dr_data = (u8 *) &settings;
  dr.dr_size = sizeof(settings);
  exec_ask(&disk_port, &dr.dr_qn);
  result = dr.dr_qn.question.q_answer == DISK_ANSWER_OK;
  exec_question_release(&dr.dr_qn);
  return result;
}/*}}}*/
