/* command.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "lpc2138.h"
#include "types.h"

void command_loop(void *arg);
void command_init(void);

#define define_command(x) char *cmd_##x(int extra, char *command, int *tokens, int *lengths, int num_tokens)
#define declare_command(x) define_command(x)

typedef char *(command_handler_t)(int, char *, int *, int *, int);

typedef struct {
  char *ce_command;
  int ce_min_params;
  int ce_max_params;
  int ce_extra;
  command_handler_t *ce_handler;
  u32 ce_required_privileges;
} command_entry;

#endif

