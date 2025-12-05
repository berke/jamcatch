/* dsp_loop.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef DSP_LOOP_H
#define DSP_LOOP_H

#include <types.h>
#include <exec.h>
#include <detect.h>
#include <indicator.h>

#define DSP_DATA_REQUEST 0
#define DSP_FFT_REQUEST 1
#define DSP_OK 0
#define DSP_ERROR 1

void dsp_loop_init(void);
void dsp_loop_start(void);
extern port_node dsp_port;
extern detector_t detectors[CHANNELS];
extern indicator_node dsp_td_z_score;

#endif
