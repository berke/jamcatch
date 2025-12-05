/* disk.c
 *
 * Sane card system.
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <mmc.h>
#include <disk.h>
#include <debug.h>
#include <serial.h>
#include <string.h>
#include <assert.h>
#include <xtea.h>
#include <config.h>
#include <exec.h>
#include <random.h>
#include <rtc.h>

task_node disk_task;
u32 disk_stack[DISK_STACK_SIZE];
port_node disk_port;
random_t disk_rnd;

const u32 disk_key[] = {
  0xd4811664, 0xbdc350fb, 0xd364d2f5, 0x66a04a6d,
  0xc7b393c5, 0xc5e0af4b, 0x23b99f05, 0x5d9e7b5e
};

#define DISK_WORDS (MMC_BLOCK_SIZE / sizeof(u32))

static bool disk_inserted;
static u32 disk_buffer[DISK_WORDS];

bool disk_read_card(card_t card, u8 *data, u32 size)/*{{{*/
{
  int i;

  assert(size <= DISK_DATA_SIZE);

  if(!mmc_read_block(card, (u8 *) disk_buffer)) return false;

  xtea_decipher_cbc(disk_buffer, disk_key, sizeof(disk_buffer) / XTEA_BLOCK_SIZE);

  DEBUGF("buffer { 0x%x, 0x%x, 0x%x, 0x%x }",
         disk_buffer[DISK_SALT0_INDEX], disk_buffer[DISK_SALT1_INDEX],
         disk_buffer[DISK_SIGNATURE_INDEX], disk_buffer[DISK_NUMBER_INDEX]);

  if(disk_buffer[DISK_SIGNATURE_INDEX] == DISK_SIGNATURE &&
     disk_buffer[DISK_NUMBER_INDEX] == card) {
    for(i = 0; i < size; i ++) {
      data[i] = ((u8 *) disk_buffer)[DISK_DATA_START + i];
    }
    return true;
  } else return false;
}/*}}}*/
bool disk_write_card(card_t card, u8 *data, u32 size)/*{{{*/
{
  int i;

  assert(size <= DISK_DATA_SIZE);

  disk_buffer[DISK_SALT0_INDEX] = random_get(&disk_rnd);
  disk_buffer[DISK_SALT1_INDEX] = random_get(&disk_rnd);

  for(i = DISK_DATA_START; i < size; i ++) {
    ((u8 *) disk_buffer)[i] = data[i];
  }

  for(; i < sizeof(disk_buffer) - XTEA_BLOCK_SIZE; i ++) {
    ((u8 *) disk_buffer)[i] = 0;
  }

  disk_buffer[DISK_SIGNATURE_INDEX] = DISK_SIGNATURE;
  disk_buffer[DISK_NUMBER_INDEX] = card;
  DEBUGF("buffer { 0x%x, 0x%x, 0x%x, 0x%x }",
         disk_buffer[DISK_SALT0_INDEX], disk_buffer[DISK_SALT1_INDEX],
         disk_buffer[DISK_SIGNATURE_INDEX], disk_buffer[DISK_NUMBER_INDEX]);

  xtea_encipher_cbc(disk_buffer, disk_key, sizeof(disk_buffer) / XTEA_BLOCK_SIZE);

  return mmc_write_block(card, (u8 *) disk_buffer);
}/*}}}*/
void disk_entry() {/*{{{*/
  question_node *qn_request;
  disk_request_t *dr;
  char *error;
  u32 answer;

  bool check_insert(void) {
    if(disk_inserted) return true;
    disk_inserted = !mmc_insert();
    return disk_inserted;
  }

  DEBUGF("Disk entry");

  answer = DISK_ANSWER_ERROR;

  for(;;) {
    qn_request = exec_get_question(&disk_port);
    if(qn_request) {
      DEBUGF("Got request");
      switch(qn_request->question.q_question) {
        case DISK_READ:
          DEBUGF("Read");
          if(check_insert()) {
            dr = (disk_request_t *)qn_request->question.q_question_extra;
            DEBUGF("Read %u", dr->dr_card);
            if(disk_read_card(dr->dr_card, dr->dr_data, dr->dr_size)) answer = DISK_ANSWER_OK;
          }
          break;
        case DISK_WRITE:
          DEBUGF("Write");
          if(check_insert()) {
            dr = (disk_request_t *)qn_request->question.q_question_extra;
            DEBUGF("Write %u", dr->dr_card);
            if(disk_write_card(dr->dr_card, dr->dr_data, dr->dr_size)) answer = DISK_ANSWER_OK;
          }
          break;
        case DISK_INSERT:
          DEBUGF("Insert");
          error = mmc_insert();
          if(error) {
            DEBUGF("Insert error: %s", error);
            disk_inserted = false;
          } else {
            DEBUGF("Insert OK");
            disk_inserted = true;
            answer = DISK_ANSWER_OK;
          }
          break;
        default:
          DEBUGF("Unknown");
          break;
      }
      qn_request->question.q_answer = answer;
      exec_reply(&disk_port, qn_request, true);
      DEBUGF("Answered %d", answer);
    }
  }
}/*}}}*/
void disk_init()/*{{{*/
{
  time_t t;
  u32 s1, s2;

  disk_inserted = false;

  rtc_get_time(&t);
  s1 = rtc_encode(&t);
  s2 = s1;
  random_from_seed(&disk_rnd, s1, s2);

  exec_add_task(&disk_task, "disk", disk_entry, 0, disk_stack, length(disk_stack), DISK_PRI);
  exec_add_port(&disk_port, "disk", &disk_task, 0);
}/*}}}*/
