/* test_detector.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <stdio.h>
#include <stdlib.h>

#include "detect.h"

#define LEARN_FRAMES 256
#define DROP_FRAMES 64

int main(int argc, char **argv)
{
  detector_t d;
  double t, x;
  fix_t packet[DETECTOR_PACKET_LENGTH];
  int i;
  int frames;
  u32 result;

  detector_init(&d);

  for(frames = 0; ; frames ++) {
    for(i = 0; i < DETECTOR_PACKET_LENGTH; i ++) {
      if(2 != fscanf(stdin, "%lf %lf", &t, &x)) {
        fprintf(stderr, "Input error.\n");
        exit(EXIT_FAILURE);
      }
      packet[i] = F(x);
    }
    result = detector_process_frame(&d, packet);
    printf("%f %d\n", (double) (frames * DETECTOR_PACKET_LENGTH) / 18000, result);
  }

  return 0;
}
