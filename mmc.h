/* mmc.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef MMC_H

#include <types.h>
#include <lpc2138.h>

#define MMC_BLOCK_SIZE 512

void mmc_init(void);
char *mmc_insert(void);
bool mmc_read_block(int num, u8 *data);
bool mmc_write_block(int num, u8 *data);
bool mmc_get_serial(u32 *);

#endif
