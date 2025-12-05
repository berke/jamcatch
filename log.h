/* log.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef LOG_H
#define LOG_H

#include <types.h>
#include <softfloat.h>

enum {
  EVENT_BOOT,
  EVENT_ALARM,
  EVENT_SAVE_SETTINGS,
  EVENT_LOAD_SETTINGS,
  EVENT_COUNT
};

typedef struct {
  u32 log_timestamp;
  u8 log_event_kind;
  u32 log_info;
} log_entry_t;

#define LOG_NUM_ENTRIES 20

typedef struct {
  u16 log_count; /* Number of entries */
  u16 log_index; /* Index of first entry */
  log_entry_t log_entries[LOG_NUM_ENTRIES];
} log_t;

extern log_t log_table;

void log_add(u8 kind, u32 info);
void log_init(void);
/*bool log_load(void);
bool log_save(void);*/
void log_erase(void);
void log_copy(log_t *dst);
void log_restore(log_t *src);

#define LOG_ADD 0
#define LOG_SAVE 1
#define LOG_LOAD 2
#define LOG_ERASE 2

#endif
