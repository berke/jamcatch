/* debug.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef DEBUG_H
#define DEBUG_H

#if DEBUG_ENABLED
#if HOST
#include "stdio.h"
#define DEBUG(x) fprintf("DEBUG %s:%d %s\n", __FUNCTION__, __LINE__, x)
#define DEBUGF(x, y...) fprintf(stderr, "DEBUG %s:%d " x "\n", __FUNCTION__, __LINE__, ## y)
#else
#include "serial.h"
#define DEBUG(x) serial_debug(__FUNCTION__, __LINE__, x)
#define DEBUGF(x, y...) serial_debugf(YLW "DEBUG %s:%d " x NML CRLF, __FUNCTION__, __LINE__, ## y)
#endif
#else
#define DEBUG(x)
#define DEBUGF(x, y...)
#endif

#define XDEBUGF(x, y...)
#define XDEBUG(x)

#endif
