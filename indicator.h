/* indicator.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef INDICATOR_H
#define INDICATOR_H

#include <types.h>
#include <list.h>
#include <fix.h>
#include <softfloat.h>

#define INDICATOR_FLAG_SIGNED 1

typedef enum {
  KD_BOOL,
  KD_U32,
  KD_S32,
  KD_FIX,
  KD_F32,
  KD_F64,
  KD_STRING,
} kind_t;

typedef struct {
  kind_t dt_kind;
  union {
    struct {
      u32 min;
      u32 max;
      u32 value;
    } dt_u32;
    struct {
      s32 min;
      s32 max;
      s32 value;
    } dt_s32;
    struct {
      fix_t min;
      fix_t max;
      fix_t value;
    } dt_fix;
    struct {
      float32 min;
      float32 max;
      float32 value;
    } dt_f32;
    struct {
      float64 min;
      float64 max;
      float64 value;
    } dt_f64;
    string dt_string;
    bool dt_bool;
  };
} data_t;

typedef struct {
  string in_name;
  u32 in_identifier;
  data_t in_data;
  volatile u32 in_value;
} indicator;

List_Make(indicator);

static inline void indicator_s32_set_range(indicator_node *in, s32 min, s32 max) {
  in->indicator.in_data.dt_s32.min = min;
  in->indicator.in_data.dt_s32.max = max;
}

static inline void indicator_u32_set_range(indicator_node *in, u32 min, u32 max) {
  in->indicator.in_data.dt_u32.min = min;
  in->indicator.in_data.dt_u32.max = max;
}

static inline void indicator_fix_set_range(indicator_node *in, fix_t min, fix_t max) {
  in->indicator.in_data.dt_fix.min = min;
  in->indicator.in_data.dt_fix.max = max;
}

static inline string indicator_string_get(indicator_node *in) {
  return in->indicator.in_data.dt_string;
}

static inline fix_t indicator_fix_get(indicator_node *in) {
  return in->indicator.in_data.dt_fix.value;
}

static inline s32 indicator_s32_get(indicator_node *in) {
  return in->indicator.in_data.dt_s32.value;
}

static inline u32 indicator_u32_get(indicator_node *in) {
  return in->indicator.in_data.dt_u32.value;
}

static inline float32 indicator_f32_get(indicator_node *in) {
  return in->indicator.in_data.dt_f32.value;
}

static inline float64 indicator_f64_get(indicator_node *in) {
  return in->indicator.in_data.dt_f64.value;
}

static inline bool indicator_bool_get(indicator_node *in) {
  return in->indicator.in_data.dt_bool;
}

static inline void indicator_string_update(indicator_node *in, string value) {
  in->indicator.in_data.dt_string = value;
}

static inline void indicator_s32_update(indicator_node *in, s32 value) {
  in->indicator.in_data.dt_s32.value = value;
}

static inline void indicator_u32_update(indicator_node *in, u32 value) {
  in->indicator.in_data.dt_u32.value = value;
}

static inline void indicator_f32_update(indicator_node *in, float32 value) {
  in->indicator.in_data.dt_f32.value = value;
}

static inline void indicator_f64_update(indicator_node *in, float64 value) {
  in->indicator.in_data.dt_f64.value = value;
}

static inline void indicator_fix_update(indicator_node *in, fix_t value) {
  in->indicator.in_data.dt_fix.value = value;
}

static inline void indicator_bool_update(indicator_node *in, bool value) {
  in->indicator.in_data.dt_bool = value;
}

void indicator_init(void);
void indicator_list_available(void);
void indicator_fix_init(indicator_node *in);
void indicator_f32_init(indicator_node *in);
void indicator_f64_init(indicator_node *in);
void indicator_s32_init(indicator_node *in);
void indicator_u32_init(indicator_node *in);
void indicator_bool_init(indicator_node *in);
void indicator_string_init(indicator_node *in);
void indicator_add(indicator_node *in, string name);
indicator_node *indicator_find(const char *name, int length);
u32 indicator_read(indicator_node *in);

indicator_list indicators;

#endif
