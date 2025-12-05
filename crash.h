/* crash.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef CRASH_H

#include "types.h"
#include "lpc2138.h"

extern u32 crash_info;
extern u32 crash_cookie;
extern u32 crash_pc;

#define CRASH_COOKIE 0xdefaced

#endif
