/* serial.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef SERIAL_H

#include "types.h"
#include "lpc2138.h"

void serial_init(void);
void serial_init_device(void);
char serial_read_char(void);
char serial_maybe_read_char(bool *success);
int serial_read_line(char *u, int length, int echo);
void serial_write_char(char c);
void serial_write_string(const char *u);
void serial_write_word_hex(u32 x);
void serial_write_word_dec(u32 x);
void serial_write_substring(const char *u, int m);
#if 0
void serial_printf(const char *u, ...) __attribute__ ((format (printf, 1, 2)));
#else
void serial_printf(const char *u, ...);
#endif

#if DEBUG_ENABLED
void serial_debug(const char *function, int line, const char *message);
void serial_debugf(const char *u, ...);
#endif

void serial_display_sp(void);
void serial_set_debug_mode(void);
void INTERRUPT serial_irq(void);

#define CLS "\033[2J"
#define CEOL "\033[K"
#define MOV "\033[%d;%dH"
#define RED "\033[31m"
#define GRN "\033[32m"
#define YLW "\033[33m"
#define BLU "\033[34m"
#define INV "\033[7m"
#define BLD "\033[1m"
#define NML "\033[0m"
#define CRLF "\r\n"

#endif
