/* random.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <types.h>

typedef u32 random_t[2];

void random_init(random_t *rnd);
void random_from_seed(random_t *rnd, u32 s1, u32 s2);
u32 random_get(random_t *rnd);

#endif
