/* string.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef STRING_H
#define STRING_H

#include "types.h"

int string_length(const char *u);
int strings_equal(const char *u, int m, const char *v, int n);
void byte_copy(u8 *src, u8 *dst, int m);
bool word_compare(void *x, void *y, int m);
void word_copy(void *src, void *dst, int m);
void word_zero(void *dst, int m);

#endif
