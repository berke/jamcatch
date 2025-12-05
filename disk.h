/* disk.h
 *
 * Sane file system.
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#ifndef DISK_H
#define DISK_H

#include <exec.h>
#include <mmc.h>

#define DISK_BLOCK_SIZE MMC_BLOCK_SIZE

typedef u32 card_t;

#define DISK_INSERT 0
#define DISK_READ 1
#define DISK_WRITE 2

#define DISK_ANSWER_OK 0
#define DISK_ANSWER_ERROR 1

#define DISK_SIGNATURE 0x31415926

#define DISK_SALT0_INDEX 0
#define DISK_SALT1_INDEX 1
#define DISK_SIGNATURE_INDEX (DISK_WORDS - 2)
#define DISK_NUMBER_INDEX (DISK_WORDS - 1)
#define DISK_DATA_START 8

#define DISK_BLOCK_OVERHEAD 16
#define DISK_DATA_SIZE (DISK_BLOCK_SIZE - DISK_BLOCK_OVERHEAD)

void disk_init(void);
bool disk_read_card(card_t card, u8 *data, u32 size);
bool disk_write_card(card_t card, u8 *data, u32 size);

typedef struct {
  question_node dr_qn;
  card_t dr_card;
  u8 *dr_data;
  u32 dr_size;
} disk_request_t;

extern port_node disk_port;

#endif
