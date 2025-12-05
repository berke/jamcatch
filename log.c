/* log.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <log.h>
#include <disk.h>
#include <debug.h>
#include <config.h>
#include <string.h>
#include <rtc.h>
#include <exec.h>

log_t log_table;
spinlock_t log_lock;

void log_init(void) {/*{{{*/
  log_table.log_count = 0;
  log_table.log_index = 0;
  exec_spinlock_init(&log_lock);
}/*}}}*/
void log_validate(void) {/*{{{*/
  if(log_table.log_count > LOG_NUM_ENTRIES) {
    XDEBUGF("Adjusting count");
    log_table.log_count = LOG_NUM_ENTRIES;
  }
  if(log_table.log_index > log_table.log_count || log_table.log_index >= LOG_NUM_ENTRIES) {
    log_table.log_index = 0;
  }
}/*}}}*/
void log_copy(log_t *dst)/*{{{*/
{
  exec_spinlock_acquire(&log_lock);
  word_copy((u32 *) &log_table, (u32 *) dst, sizeof(log_t));
  exec_spinlock_release(&log_lock);
}/*}}}*/
void log_restore(log_t *src)/*{{{*/
{
  word_copy((u32 *) src, (u32 *) &log_table, sizeof(log_t));
  log_validate();
}/*}}}*/
#if 0
bool log_load(void)/*{{{*/
{
  XDEBUGF("Loading");
  return disk_read_card(DISK_LOG_BLOCK, (u8 *) &log_table, sizeof(log_t));
}/*}}}*/
bool log_save(void)/*{{{*/
{
  XDEBUGF("Saving");
  return disk_write_card(DISK_LOG_BLOCK, (u8 *) &log_table, sizeof(log_t));
}/*}}}*/
#endif
void log_add(u8 kind, u32 info)/*{{{*/
{
  time_t t;
  log_entry_t *e;

  e = log_table.log_entries + log_table.log_index;
  rtc_get_time(&t);
  e->log_timestamp = rtc_encode(&t);
  e->log_event_kind = kind;
  e->log_info = info;
  XDEBUGF("Adding log_table entry, t=%u, kind=%d", e->log_timestamp, e->log_event_kind);

  if(log_table.log_count < LOG_NUM_ENTRIES) log_table.log_count ++;

  log_table.log_index ++;
  if(log_table.log_index == LOG_NUM_ENTRIES) {
    log_table.log_index = 0;
  }
}/*}}}*/
void log_erase(void) {/*{{{*/
  log_table.log_count = 0;
  log_table.log_index = 0;
}/*}}}*/
