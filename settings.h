/* settings.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <types.h>

#define SETTINGS_VERSION 20070926

typedef struct {
  int st_version;
  int st_alarm_threshold; /* Alarm threshold */
  int st_alarm_decay;
  int st_alarm_enabled;
  int st_alarm_count; /* Total number of alarms */
  int st_indicator_threshold;
  int st_eas_threshold;
} settings_t;

extern settings_t settings;

void settings_init(void);
bool settings_load(void);
bool settings_save(void);
void settings_default(void);
void settings_dump(void);

#endif
