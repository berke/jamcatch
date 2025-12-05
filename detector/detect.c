/* detect.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#if XX
#include <stddef.h>
#endif

#include <types.h>
#include <fix.h>
#include <serial.h>
#include <debug.h>
#include <string.h>
#include <detect.h>
#include <random.h>
#include <sort.h>

#include "fft.h"

u32 log_2(u32 x) {/*{{{*/
  if(x < 0x00008000) {
    if(x < 0x00000080) {
      if(x < 0x00000008) {
        if(x < 0x00000002) {
          if(x < 0x00000001) {
            return 0;
          } else {
            return 1;
          }
        } else {
          if(x < 0x00000004) {
            return 2;
          } else {
            return 3;
          }
        }
      } else {
        if(x < 0x00000020) {
          if(x < 0x00000010) {
            return 4;
          } else {
            return 5;
          }
        } else {
          if(x < 0x00000040) {
            return 6;
          } else {
            return 7;
          }
        }
      }
    } else {
      if(x < 0x00000800) {
        if(x < 0x00000200) {
          if(x < 0x00000100) {
            return 8;
          } else {
            return 9;
          }
        } else {
          if(x < 0x00000400) {
            return 10;
          } else {
            return 11;
          }
        }
      } else {
        if(x < 0x00002000) {
          if(x < 0x00001000) {
            return 12;
          } else {
            return 13;
          }
        } else {
          if(x < 0x00004000) {
            return 14;
          } else {
            return 15;
          }
        }
      }
    }
  } else {
    if(x < 0x00800000) {
      if(x < 0x00080000) {
        if(x < 0x00020000) {
          if(x < 0x00010000) {
            return 16;
          } else {
            return 17;
          }
        } else {
          if(x < 0x00040000) {
            return 18;
          } else {
            return 19;
          }
        }
      } else {
        if(x < 0x00200000) {
          if(x < 0x00100000) {
            return 20;
          } else {
            return 21;
          }
        } else {
          if(x < 0x00400000) {
            return 22;
          } else {
            return 23;
          }
        }
      }
    } else {
      if(x < 0x08000000) {
        if(x < 0x02000000) {
          if(x < 0x01000000) {
            return 24;
          } else {
            return 25;
          }
        } else {
          if(x < 0x04000000) {
            return 26;
          } else {
            return 27;
          }
        }
      } else {
        if(x < 0x20000000) {
          if(x < 0x10000000) {
            return 28;
          } else {
            return 29;
          }
        } else {
          if(x < 0x40000000) {
            return 30;
          } else {
            return 31;
          }
        }
      }
    }
  }
}/*}}}*/
static void detect_normify(fix_t *src, fix_t *dst, int m)/*{{{*/
{
  int i;

  /* Do FFT ! */

  for(i = 0; i < m; i ++) {
    dst[i] = mul(src[2 * i], src[2 * i]) + mul(src[2 * i + 1], src[2 * i + 1]);
  }
}/*}}}*/
static fix_t detect_dot_product(const fix_t *u, const fix_t *v, u32 m)/*{{{*/
{
  int i;
  long_fix_t d;

  d = 0;
  for(i = 0; i < m; i ++) {
    d += mul(u[i], v[i]);
  }
  return (d >> log_2(m));
}/*}}}*/
static inline void detect_normalize(fix_t *u, int m, int shift)/*{{{*/
{
  int i;

  for(i = 0; i < m; i++) {
    u[i] >>= shift;
  }
}/*}}}*/
static inline float32 positive_part(float32 x)/*{{{*/
{
  return float32_lt(x, 0) ? 0 : x;
}/*}}}*/

extern fix_t window1[];
extern fix_t class1[];
extern fix_t window2[];

const fix_t threshold = F(3.0);

float64 fix64;
float32 fix32;

void float32_scorer_context_init(float32_scorer_context_t *sc, int samples)/*{{{*/
{
  float32 one;

  sc->sc_samples = samples;
  one = int32_to_float32(1);
  sc->sc_1m = float32_div(one, int32_to_float32(samples));
  sc->sc_11m = float32_sub(one, sc->sc_1m);
}/*}}}*/
void float32_scorer_init(float32_scorer_t *s)/*{{{*/
{
  s->s_mu = 0;
  s->s_sigma = 0;
}/*}}}*/
/* x=5.66366222 mu=0.00000000 sigma=0.00000000 mu2=2147483647.47483647 sigma2=2147483647.47483647 */
void float32_scorer_update(float32_scorer_context_t *sc, float32_scorer_t *s, float32 sample)/*{{{*/
{
  float32 mu, sigma;
  float32 mu2, sigma2;

  mu = s->s_mu;
  sigma = s->s_sigma;

  mu2 =
    float32_add(
      float32_mul(mu, sc->sc_11m),
      float32_mul(sample, sc->sc_1m));

  sigma2 =
    float32_mul(
      float32_add(
        sigma,
        float32_mul(mu, mu)),
      sc->sc_11m);
  
  sigma2 = float32_add(sigma2,
             float32_mul(
               sc->sc_1m,
               float32_mul(sample, sample)));

  sigma2 = float32_sub(sigma2, float32_mul(mu2, mu2));

  s->s_mu = mu2;
  s->s_sigma = sigma2;
  XDEBUGF("x=%f mu=%f sigma=%f mu2=%f sigma2=%f", sample, mu, sigma, mu2, sigma2);
}/*}}}*/
float32 float32_scorer_get_z_score(float32_scorer_context_t *sc, float32_scorer_t *s, float32 sample)/*{{{*/
{
  float32 z;
  if(!s->s_sigma) return 0;

  sample = float32_sub(sample, s->s_mu);
  z = float32_div(float32_mul(sample, sample), s->s_sigma);
  if(float32_lt(sample, 0)) z = float32_sub(0, z);
  return z;
}/*}}}*/

static inline float32 fix_to_float32(fix_t x)/*{{{*/
{
  return float32_div(int32_to_float32(x), fix32);
}/*}}}*/
static inline float64 fix_to_float64(fix_t x)/*{{{*/
{
  return float64_div(int32_to_float64(x), fix64);
}/*}}}*/
static inline float32 z_score(fix_t x, float32 mu, float32 sigma)/*{{{*/
{
  float32 xf;
  float32 dx;

  xf = fix_to_float32(x);
  if(sigma == 0) return 0;
  dx = float32_sub(mu, xf);
  dx = float32_mul(dx, dx);
  dx = float32_div(dx, sigma);
  return dx;
}/*}}}*/
bool detector_process_frame(detector_t *d, fix_t *packet)/*{{{*/
{
  int i;
  bool result;
  detector_state_t old_state;

  void classifier_reset(void)/*{{{*/
  {
#if DETECTOR_FEATURES_ENABLED
    int i;

#if XX
    fprintf(stderr, "RESET %d %d %d\n", sizeof(d->dt_sigma2), offsetof(detector_t, dt_sigma2), offsetof(detector_t, dt_count));
#endif
    for(i = 0; i < DETECTOR_FEATURES; i ++) {
      d->dt_sx[i] = 0;
      d->dt_sxx[i] = 0;
    }
#endif
  }/*}}}*/
  void dc_classifier_reset(void)/*{{{*/
  {
    float32_scorer_init(&d->dt_dc_scorer);
  }/*}}}*/
  void td_classifier_reset(void)/*{{{*/
  {
    float32_scorer_init(&d->dt_td_scorer);
  }/*}}}*/
  void classifier_learn(fix_t *features)/*{{{*/
  {
#if DETECTOR_FEATURES_ENABLED
    int i;

    for(i = 0; i < DETECTOR_FEATURES; i ++) {
#if XX
      fprintf(stderr, "learn %d = %f\n", i, DOUBLE(features[i]));
#endif
      d->dt_sx[i] += features[i];
      d->dt_sxx[i] += (long_fix_t) features[i] * (long_fix_t) features[i];
    }
#if XX
    fprintf(stderr, " count = %d\n", d->dt_count);
#endif
#endif
  }/*}}}*/
  void dc_classifier_learn(fix_t *features) {/*{{{*/
    float32_scorer_update(&d->dt_dc_context, &d->dt_dc_scorer, fix_to_float32(features[0]));
  }/*}}}*/
  fix_t compute_td_value(fix_t *frame, fix_t *buffer1, fix_t *buffer2) {/*{{{*/
    /* Compute minimum... */
    int i;
    fix_t tenth;

    for(i = 0; i < DETECTOR_DOT_LENGTH; i ++) {
      buffer1[i] = frame[i];
    }

    sort_fix(buffer1, buffer2, DETECTOR_DOT_LENGTH);

    tenth = 0;
    for(i = DETECTOR_TD_LOW; i < DETECTOR_TD_HIGH; i ++) {
      tenth += buffer1[i];
    }

    return tenth;
  }/*}}}*/
  void td_classifier_learn(fix_t x) {/*{{{*/
    float32_scorer_update(&d->dt_td_context, &d->dt_td_scorer, fix_to_float32(x));
  }/*}}}*/
  float32 classify_dc(fix_t *features)/*{{{*/
  {
    float32 sample;

    sample = fix_to_float32(features[0]);
    if(!(random_get(&d->dt_rnd) & DETECTOR_DC_UPDATE_MASK))
      float32_scorer_update(&d->dt_dc_context, &d->dt_dc_scorer, sample);
    return positive_part(float32_scorer_get_z_score(&d->dt_dc_context, &d->dt_dc_scorer, sample));
  }/*}}}*/
  float32 classify_td(fix_t x)/*{{{*/
  {
    float32 sample;

    sample = fix_to_float32(x);
    if(!(random_get(&d->dt_rnd) & DETECTOR_TD_UPDATE_MASK))
      float32_scorer_update(&d->dt_td_context, &d->dt_td_scorer, sample);
    return positive_part(float32_scorer_get_z_score(&d->dt_td_context, &d->dt_td_scorer, sample));
  }/*}}}*/
  float32 classify(fix_t *features)/*{{{*/
  {
#if DETECTOR_FEATURES_ENABLED
    int i;
    float32 total;

    total = 0;

    for(i = 1; i < DETECTOR_FEATURES; i ++) {
      total = float32_add(total, z_score(features[i], d->dt_mu[i], d->dt_sigma[i]));
#if XX
        fprintf(stderr, "feature[%d] = %f, z=%f, mu=%f, sigma=%f %c\n",
               i,
               DOUBLE(features[i]),
               DOUBLE(z),
               DOUBLE(d->dt_mu[i]),
               DOUBLE2(d->dt_sigma2[i]),
               (z > 3 * d->dt_sigma2[i]) ? '+':' ');
#endif

    }

    total = float32_div(total, int32_to_float32(DETECTOR_FEATURES));

    return total;
#else
    return 0;
#endif
  }/*}}}*/
  void do_alarm(float32 z)/*{{{*/
  {
  }/*}}}*/
  bool process_alarm(float32 z)/*{{{*/
  {
    XDEBUGF("filter=%f decay=%f z=%f", d->dt_alarm_filter, d->dt_alarm_decay, z);
    d->dt_alarm_filter =
      float32_add(
        float32_mul(d->dt_alarm_decay, d->dt_alarm_filter),
        float32_mul(float32_sub(int32_to_float32(1), d->dt_alarm_decay), z));
    return float32_lt(d->dt_alarm_threshold, d->dt_alarm_filter);
  }/*}}}*/
  void adjuster_process(float32 z)/*{{{*/
  {
    XDEBUGF("adj %f %f %f", z, d->dt_adjuster_sz, d->dt_adjuster_szz);
    d->dt_adjuster_sz = float32_add(d->dt_adjuster_sz, z);
    d->dt_adjuster_szz = float32_add(d->dt_adjuster_szz, float32_mul(z, z));
  }/*}}}*/
  void adjuster_init(void)/*{{{*/
  {
    d->dt_adjuster_sz = 0;
    d->dt_adjuster_szz = 0;
  }/*}}}*/
  void adjuster_finalize(void)/*{{{*/
  {
    float32 ez, ezz;
    float32 m;

    m = int32_to_float32(DETECTOR_ADJUST_FRAMES);
    ez = float32_div(d->dt_adjuster_sz, m);
    ezz = float32_div(d->dt_adjuster_szz, m);
    d->dt_adjuster_mu = ez;
    d->dt_adjuster_sigma = float32_sqrt(float32_sub(ezz, float32_mul(ez,ez)));
    DEBUGF("Adjuster mu=%f sigma=%f" CRLF, d->dt_adjuster_mu, d->dt_adjuster_sigma);
  }/*}}}*/
  float32 adjuster_get_z_score(float32 z)/*{{{*/
  {
    float32 zz;

    zz = float32_div(float32_sub(z, d->dt_adjuster_mu), d->dt_adjuster_sigma);
    if(float32_lt(zz, 0)) {
      zz = 0;
      /* zz = float32_sub(0, zz); */
    }

    return zz;
  }/*}}}*/
  void classifier_study(void)/*{{{*/
  {
#if DETECTOR_FEATURES_ENABLED
    int i;
    long_fix_t ex, exx;
    long_fix_t s;
    float64 sigma2;

    DEBUGF("Studying");

    for(i = 0; i < DETECTOR_FEATURES; i ++) {
      ex = d->dt_sx[i] / DETECTOR_LEARN_FRAMES;
      exx = d->dt_sxx[i] / DETECTOR_LEARN_FRAMES;
      s = exx - ex * ex;
      s >>= FIX;
      sigma2 = int32_to_float64((fix_t) s);
      sigma2 = float64_div(sigma2, fix64);
      sigma2 = float64_sqrt(sigma2);
      d->dt_mu[i] = fix_to_float32(ex);
      d->dt_sigma[i] = float64_to_float32(sigma2);
    }
#endif
  }/*}}}*/
  void build_frame(fix_t *u, fix_t *v, fix_t *w, int m, int n)/*{{{*/
  {
    int i, j;

    for(i = 0; i < m; i ++) {
      w[i] = u[i];
    }
    for(j = 0; i < n; i ++, j ++) {
      w[i] = v[j];
    }
  }/*}}}*/
  void concat(fix_t *u, int m, fix_t *v, int n, fix_t *w)/*{{{*/
  {
    int i, j;

    i = 0;

    for(j = 0; j < m; j ++) {
      w[i ++] = u[j];
    }
    for(j = 0; j < n; j ++) {
      w[i ++] = v[j];
    }
  }/*}}}*/
  void complexify(fix_t *u, int m, fix_t *v)/*{{{*/
  {
    while(m --) {
      *(v ++) = *(u ++);
      *(v ++) = 0;
    }
  }/*}}}*/
  void post_dot_frame(fix_t *frame)/*{{{*/
  {
    int i;

#if XX
    fprintf(stderr, "Posting dot frame..\n");
#endif

    void update_z_scores(void) {/*{{{*/
#if ONLY_TD
      d->dt_z_score = classify(d->dt_dot_fft_norm);
      d->dt_dc_z_score = classify_dc(d->dt_dot_fft_norm);
#endif
      d->dt_td_z_score = classify_td(d->dt_td_value);
      d->dt_total_z_score = float32_sqrt(d->dt_td_z_score);
#if 0
      d->dt_total_z_score =
        float32_sqrt(
            float32_add(
              d->dt_z_score,
              float32_add(
                d->dt_dc_z_score,
                d->dt_td_z_score)));
#endif
    }/*}}}*/
    void clear_z_scores(void) {/*{{{*/
      d->dt_z_score = 0;
      d->dt_dc_z_score = 0;
      //d->dt_td_z_score = 0; // XXX
      d->dt_total_z_score = 0;
      d->dt_alarm_filter = 0;
    }/*}}}*/

    d->dt_td_value = compute_td_value(frame, d->dt_dot_complex, d->dt_dot_complex + DETECTOR_DOT_LENGTH);

#if !ONLY_TD
    for(i = 0; i < DETECTOR_DOT_LENGTH; i++) {
      d->dt_dot_complex[2 * i] = mul(frame[i], window2[i]);
      d->dt_dot_complex[2 * i + 1] = 0;
    }
#endif

    if(d->dt_dump == DETECTOR_DUMP_DOT_FRAMES) {
      u32 x;

      serial_printf("dotframe" CRLF);
      for(i = 0; i < DETECTOR_DOT_LENGTH; i++) {
        x = d->dt_frame[2 * i];
        serial_write_char(x & 0xff); x >>= 8;
        serial_write_char(x & 0xff); x >>= 8;
        serial_write_char(x & 0xff); x >>= 8;
        serial_write_char(x & 0xff);
      }
    }

    d->dt_total ++;

#if !ONLY_TD
    fft_bit_reverse(d->dt_dot_complex, DETECTOR_DOT_LENGTH);
#if DETECTOR_DOT_LENGTH == 1024
    fft_1024(d->dt_dot_complex);
#else
#if DETECTOR_DOT_LENGTH == 512
    fft_512(d->dt_dot_complex);
#else
#error "Unexpected dot frame length"
#endif
#endif
    /* detect_normalize(d->dt_dot_complex, 2 * DETECTOR_DOT_LENGTH, log_2(DETECTOR_DOT_LENGTH)); */
    detect_normify(d->dt_dot_complex, d->dt_dot_fft_norm, DETECTOR_DOT_LENGTH / 2);
#endif

    switch(d->dt_state) {
      case DETECTOR_DROP:
        d->dt_count ++;
        if(d->dt_count == DETECTOR_DROP_FRAMES)
        {
          d->dt_state = DETECTOR_EAS_WAIT;
          d->dt_count = 0;
#if !ONLY_TD
          classifier_reset();
          dc_classifier_reset();
#endif
          td_classifier_reset();
        }
        break;
      case DETECTOR_EAS_LOST:
      case DETECTOR_EAS_WAIT:
        if(d->dt_eas_level > DETECTOR_EAS_ENTER_LEVEL) {
          d->dt_state = DETECTOR_LEARN;
          d->dt_count = 0;
        }
        break;
      case DETECTOR_LEARN:
        d->dt_count ++;
        if(d->dt_count == DETECTOR_LEARN_FRAMES) {
          d->dt_state = DETECTOR_ADJUST;
          d->dt_count = 0;
#if !ONLY_TD
          classifier_study();
#endif
          adjuster_init();
        } else {
#if !ONLY_TD
          classifier_learn(d->dt_dot_fft_norm);
#endif
        }
#if !ONLY_TD
        dc_classifier_learn(d->dt_dot_fft_norm);
#endif
        td_classifier_learn(d->dt_td_value);
        break;
      case DETECTOR_ADJUST:
        d->dt_count ++;
        update_z_scores();
        adjuster_process(d->dt_total_z_score);
        if(d->dt_count == DETECTOR_ADJUST_FRAMES) {
           adjuster_finalize();
           d->dt_state = DETECTOR_RUN;
        }
        break;
      case DETECTOR_RUN:
        if(!d->dt_pause) {
          update_z_scores();

          if(d->dt_eas_level < DETECTOR_EAS_EXIT_LEVEL) {
            d->dt_state = DETECTOR_EAS_LOST;
            d->dt_count = 0;
          }

          /*d->dt_adjuster_z = adjuster_get_z_score(d->dt_total_z_score);
          result |= process_alarm(d->dt_adjuster_z);*/
          result |= process_alarm(d->dt_total_z_score);
        } else {
          clear_z_scores();
        }
        if(d->dt_dump == DETECTOR_DUMP_CLASS) {
#if 0
          serial_printf("%d %i %f %f %f %f %f %f %f %d" CRLF,
                        d->dt_total,
                        d->dt_last_dot,
                        d->dt_z_score,
                        d->dt_dc_z_score,
                        d->dt_td_z_score,
                        d->dt_total_z_score,
                        d->dt_adjuster_z,
                        d->dt_alarm_filter,
                        d->dt_alarm_threshold,
                        d->dt_pause ? 1 : 0);
#else
          serial_printf("%d td:%i,%f s:%f,%f Z:%f a:%f al:%f th:%f P:%d" CRLF,
                        d->dt_total,
                        d->dt_td_value,
                        d->dt_td_z_score,
                        d->dt_td_scorer.s_mu,
                        d->dt_td_scorer.s_sigma,
                        d->dt_total_z_score,
                        d->dt_adjuster_z,
                        d->dt_alarm_filter,
                        d->dt_alarm_threshold,
                        d->dt_pause ? 1 : 0);
#endif
        }
        break;
    }
  }/*}}}*/
  void post_dot_packet(fix_t *prev, fix_t *cur)/*{{{*/
  {
    int shift;

    /* Initial overlapping part */
    for(i = 1; i < /* = XXX */ DETECTOR_DOT_LENGTH / DETECTOR_DOT_OVERLAP; i ++) {
      shift = i * DETECTOR_DOT_OVERLAP;
      concat(
          prev + shift,
          DETECTOR_DOT_LENGTH - shift,
          cur,
          shift,
          d->dt_dot_frame);
      post_dot_frame(d->dt_dot_frame);
    }
  }/*}}}*/
  void post_dot(fix_t dot)/*{{{*/
  {
    fix_t *prev, *cur;

    if(d->dt_dot_frame_select) {
      cur  = d->dt_dot_frame1;
      prev = d->dt_dot_frame0;
    } else {
      cur  = d->dt_dot_frame0;
      prev = d->dt_dot_frame1;
    }

    d->dt_last_dot = dot;
    cur[d->dt_dot_i ++] = dot;

    /* Adjust EAS detector */
    if(dot > d->dt_eas_threshold) {
      d->dt_eas_level += DETECTOR_EAS_UP;
    } else {
      d->dt_eas_level -= DETECTOR_EAS_DOWN;
    }
    if(d->dt_eas_level > DETECTOR_EAS_MAX) {
      d->dt_eas_level = DETECTOR_EAS_MAX;
    } else if(d->dt_eas_level < DETECTOR_EAS_MIN) {
      d->dt_eas_level = DETECTOR_EAS_MIN;
    }

    /* Post frame if finished */
    if(d->dt_dot_i == DETECTOR_DOT_LENGTH) {
      d->dt_dot_i = 0;
      post_dot_packet(prev, cur);
      d->dt_dot_frame_select = !d->dt_dot_frame_select;
    }
  }/*}}}*/
  void post_frame(fix_t *frame)/*{{{*/
  {
    int i;
    fix_t dot;

    for(i = 0; i < DETECTOR_FRAME_LENGTH; i++) {
      d->dt_complex_frame[2 * i] = mul(frame[i], window1[i]);
      d->dt_complex_frame[2 * i + 1] = 0;
    }

    fft_bit_reverse(d->dt_complex_frame, DETECTOR_FRAME_LENGTH);
#if DETECTOR_FRAME_LENGTH == 64
    fft_64(d->dt_complex_frame);
#else
#error "Unexpected frame length"
#endif
    /* detect_normalize(d->dt_complex_frame, 2 * DETECTOR_FRAME_LENGTH, DETECTOR_FRAME_LENGTH); */
    detect_normify(d->dt_complex_frame, d->dt_fft_norm, DETECTOR_FRAME_LENGTH / 2);
    dot = detect_dot_product(d->dt_fft_norm, class1, DETECTOR_FRAME_LENGTH / 2);
    post_dot(dot);
  }/*}}}*/

  old_state = d->dt_state;
  result = false;

  /* Initial overlapping part *//*{{{*/
  for(i = 1; i < DETECTOR_FRAME_LENGTH / DETECTOR_FRAME_OVERLAP; i ++) {
    build_frame(d->dt_previous_frame + i * DETECTOR_FRAME_OVERLAP,
                packet,
                d->dt_frame,
                DETECTOR_FRAME_LENGTH - i * DETECTOR_FRAME_OVERLAP,
                DETECTOR_FRAME_LENGTH);
    post_frame(d->dt_frame);
  }

  /* Non-overlapping parts... */
  for(i = 0; i < (DETECTOR_PACKET_LENGTH / DETECTOR_FRAME_OVERLAP)
                 - (DETECTOR_FRAME_LENGTH / DETECTOR_FRAME_OVERLAP - 1); i ++) {
    post_frame(packet + i * DETECTOR_FRAME_OVERLAP);
  }

  /* Copy last part */
  for(i = 0; i < DETECTOR_FRAME_LENGTH; i ++) {
    d->dt_previous_frame[i] = packet[DETECTOR_PACKET_LENGTH - DETECTOR_FRAME_LENGTH + i];
  }/*}}}*/

  if(old_state != d->dt_state) d->dt_state_change = true;
  return result;
}/*}}}*/
#if ARM
void detector_dump_classifier(detector_t *d)/*{{{*/
{
#if DETECTOR_FEATURES_ENABLED
  int i;
  u32 score;

  for(i = 0; i < DETECTOR_FEATURES; i ++) {
    score = z_score(d->dt_dot_fft_norm[i], d->dt_mu[i], d->dt_sigma[i]);
    serial_printf("  F[%d]=%i mu=%f sigma=%f z=%f" CRLF,
                  i,
                  d->dt_dot_fft_norm[i],
                  d->dt_mu[i],
                  d->dt_sigma[i],
                  score);
  }
#endif
}/*}}}*/
void detector_dump_dot_fft(detector_t *d)/*{{{*/
{
  int i;

  for(i = 0; i < DETECTOR_DOT_LENGTH / 2; i ++) {
    serial_printf("%d %i" CRLF,
                  i,
                  d->dt_dot_fft_norm[i]);
  }
}/*}}}*/
#endif
void detector_init(detector_t *d)/*{{{*/
{
#if ARM
  word_zero(d, sizeof(d));
#endif

  d->dt_dot_i = 0;
  d->dt_dot_frame_select = false;
  d->dt_count = 0;
  d->dt_total = 0;
  d->dt_state = DETECTOR_DROP;
  d->dt_alarm_filter = 0;
  d->dt_adjuster_z = 0;
  d->dt_adjuster_sz = 0;
  d->dt_adjuster_szz = 0;
  d->dt_pause = false;
  d->dt_dump = false;
  d->dt_state_change = false;
  d->dt_dot_max = 0;

  random_init(&d->dt_rnd);

  d->dt_z_score = 0;
  d->dt_dc_z_score = 0;
  d->dt_td_value = 0;
  d->dt_td_z_score = 0;
  d->dt_triggered = 0;

  d->dt_eas_threshold = DETECTOR_EAS_THRESHOLD;
  d->dt_eas_level = 0;

  float32_scorer_context_init(&d->dt_dc_context, DETECTOR_DC_SCORE_SAMPLES);
  float32_scorer_init(&d->dt_dc_scorer);

  float32_scorer_context_init(&d->dt_td_context, DETECTOR_TD_SCORE_SAMPLES);
  float32_scorer_init(&d->dt_td_scorer);

  fix32 = int32_to_float32(1 << FIX);
  fix64 = int32_to_float64(1 << FIX);

  d->dt_alarm_decay = fix_to_float32(DETECTOR_ALARM_DECAY);
}/*}}}*/
void detector_reset(detector_t *d)/*{{{*/
{
  /* MUTEX */
  d->dt_state = DETECTOR_DROP;
  d->dt_alarm_threshold = fix_to_float32(DETECTOR_ALARM_THRESHOLD);
  d->dt_count = 0;
}/*}}}*/
void detector_set_pause(detector_t *d, bool pause)/*{{{*/
{
  d->dt_pause = pause;
}/*}}}*/
void detector_set_dump(detector_t *d, detector_dump_t dump)/*{{{*/
{
  d->dt_dump = dump;
}/*}}}*/
