/* ssp.h
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef SSP_H

#include <types.h>
#include <lpc2138.h>

void ssp_init(void);
void spi_send_byte(u8 byte);
u8 spi_read_byte(void);
void spi_synchronize(int count);

#endif
