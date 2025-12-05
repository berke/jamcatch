/* format.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef FORMAT_H

#include <types.h>
#include <softfloat.h>
#include <fix.h>

struct writer_s {
  void *wr_info;
  void (*wr_putchar)(void *, char);
};

typedef struct writer_s *writer_t;

void format_printf(writer_t wr, const char *u, ...);
void format_aprintf(writer_t wr, const char *u, va_list va);
u32 snprintf(char *buf, u32 size, const char *fmt, ...);

#endif
