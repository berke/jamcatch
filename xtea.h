/* xtea.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef XTEA_H
#define XTEA_H

#include "types.h"

#define XTEA_BLOCK_SIZE 8
#define XTEA_BLOCK_SIZE_U32 2

void xtea_encipher(u32 *v, const u32 *k);
void xtea_decipher(u32 *v, const u32 *k);

/* m is the number of 64-bit blocks */
void xtea_decipher_cbc(u32 *v, const u32 *k, int m);
void xtea_encipher_cbc(u32 *v, const u32 *k, int m);
void xtea_hash(u32 *v, int m, u32 *hash);

#endif
