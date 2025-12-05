/* assert.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef ASSERT_H
#define ASSERT_H

#if ASSERT_ENABLED
#include "abort.h"
#define _GNU_SOURCE
#define assert(x) if(!(x)) abort( "ASSERTION FAILED: " #x )
#else
#define assert(x)
#endif

#endif
