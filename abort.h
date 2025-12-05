/* abort.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef ABORT_H
#define ABORT_H

#include "types.h"

#define ABORT_COOKIE 0xdeafbabe
extern char *abort_msg;
extern u32 abort_cookie;
extern void abortion(void);
#define _GNU_SOURCE
#define abort(msg) do { \
    abort_cookie = ABORT_COOKIE; \
    abort_msg = msg; \
    abortion(); \
} while(0);

#endif
