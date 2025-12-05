/* detector.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef DETECT_H
#define DETECT_H

#include <types.h>
#include <fix.h>
#include <softfloat.h>
#include <random.h>

#include "detector_config.h"

extern const fix_t detector_window1[DETECTOR_FFT1_N];
extern const fix_t detector_classifier1[DETECTOR_FFT1_N / 2];

typedef enum {
  DETECTOR_DUMP_NONE,
  DETECTOR_DUMP_CLASS,
  DETECTOR_DUMP_DOT_FRAMES,
} detector_dump_t;

typedef enum {
  DETECTOR_DROP,
  DETECTOR_EAS_WAIT,
  DETECTOR_EAS_LOST,
  DETECTOR_LEARN,
  DETECTOR_ADJUST,
  DETECTOR_RUN,
} detector_state_t;

typedef struct {
  float32 s_mu;
  float32 s_sigma;
} float32_scorer_t;

typedef struct {
  int sc_samples;
  float32 sc_1m;
  float32 sc_11m;
} float32_scorer_context_t;

typedef struct {
  detector_state_t dt_state;
  bool dt_state_change;
  bool dt_triggered;
  int dt_count;
  u32 dt_total;
  bool dt_pause;
  bool dt_dump;

  random_t dt_rnd;

  fix_t dt_previous_frame[DETECTOR_FRAME_LENGTH];
  fix_t dt_frame[DETECTOR_FRAME_LENGTH];
  fix_t dt_complex_frame[DETECTOR_FRAME_LENGTH * 2];

  fix_t dt_fft_norm[DETECTOR_FRAME_LENGTH / 2];
  fix_t dt_dot_frame0[DETECTOR_DOT_LENGTH];
  fix_t dt_dot_frame1[DETECTOR_DOT_LENGTH];
  bool dt_dot_frame_select;
  fix_t dt_last_dot;

  fix_t dt_eas_threshold;
  s32 dt_eas_level; /* For detecting the EAS signal */

  fix_t dt_dot_max;
  int dt_dot_i;
  fix_t dt_dot_frame[DETECTOR_DOT_LENGTH];
  fix_t dt_dot_complex[DETECTOR_DOT_LENGTH * 2];
  fix_t dt_dot_fft_norm[DETECTOR_DOT_LENGTH / 2];

#if DETECTOR_FEATURES_ENABLED
  long_fix_t dt_sx[DETECTOR_FEATURES];
  long_fix_t dt_sxx[DETECTOR_FEATURES];
  float32 dt_mu[DETECTOR_FEATURES];
  float32 dt_sigma[DETECTOR_FEATURES];
#endif

  float32 dt_z_score;
  float32 dt_dc_z_score;
  float32 dt_td_z_score;
  fix_t dt_td_value;
  float32 dt_total_z_score;

  float32_scorer_context_t dt_dc_context;
  float32_scorer_t dt_dc_scorer;

  float32_scorer_context_t dt_td_context;
  float32_scorer_t dt_td_scorer;

  float32 dt_alarm_threshold;
  float32 dt_alarm_decay;
  float32 dt_alarm_filter;
  union {
    float32 dt_adjuster_sz;
    float32 dt_adjuster_mu;
  };
  union {
    float32 dt_adjuster_szz;
    float32 dt_adjuster_sigma;
  };
  float32 dt_adjuster_z;
} detector_t;

void detector_init(detector_t *d);
u32 detector_process_frame(detector_t *d, fix_t *packet);
void detector_dump_classifier(detector_t *d);
void detector_dump_dot_fft(detector_t *d);
void detector_reset(detector_t *dt);
void detector_set_pause(detector_t *d, bool pause);
void detector_set_dump(detector_t *d, detector_dump_t dump);

#endif
