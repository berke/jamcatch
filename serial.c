/* serial.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <system.h>
#include <config.h>
#include <serial.h>
#include <types.h>
#include <debug.h>
#include <fix.h>
#include <exec.h>
#include <delay.h>
#include <softfloat.h>
#include <format.h>

#define SERIAL_FIFO_LENGTH 32
#define SERIAL_FIFO_MASK (SERIAL_FIFO_LENGTH - 1)

static char serial_fifo[SERIAL_FIFO_LENGTH];
static int serial_fifo_i;
static int serial_fifo_m;
static int serial_debug_mode;
port_node serial_port;
struct writer_s serial_writer;

#if DEBUG_ENABLED
static spinlock_t debug_lock;
#endif

void serial_write_char(char c)/*{{{*/
{
  while(!U0LSR_B.THRE);
  U0THR = c;
}/*}}}*/
void serial_write_char_info(void *info, char c)/*{{{*/
{
  serial_write_char(c);
}/*}}}*/
void serial_irq(void)/*{{{*/
{
  char c;
  question_node *qn;

  XDEBUGF("Serial IRQ");
  while(!U0IIR_B.I_PEND)
  {
    U0LSR;
    if(U0LSR_B.RDR) {
      c = U0RBR;
      XDEBUGF("Got char 0x%x, i=%d, m=%d", c, serial_fifo_i, serial_fifo_m);
      serial_fifo[serial_fifo_i] = c;
      serial_fifo_i ++;
      serial_fifo_m ++;
      if(serial_fifo_i == SERIAL_FIFO_LENGTH) serial_fifo_i = 0;
      if(serial_fifo_m == SERIAL_FIFO_LENGTH) {
        /* FIFO overrun ; disable interrupts */
        XDEBUGF("Serial FIFO overrun");
        U0LCR_B.DLAB = 0;
        U0IER_B.RBR_IE = 1;
      }
      XDEBUGF("Added char to fifo");
    } else {
      DEBUGF("Spurious serial ?");
    }
  }

  /*U0LCR_B.DLAB = 0;
  U0IER_B.RBR_IE = 1; */

  /* Any questions to answer ? */
  if(serial_fifo_m) {
    XDEBUGF("Checking questions");
    qn = exec_check_question(&serial_port);
    XDEBUGF("Question %p", qn);
    if(qn) {
      exec_reply(&serial_port, qn ,0);
    }
  }

  VICvectAddr = 0;
  XDEBUGF("Cleared VIC, bye");

  exec_schedule(EXEC_OTHER_INTERRUPT);
}/*}}}*/
void serial_init(void)/*{{{*/
{
  delay_us(10000);

  U0LCR_B.DLAB = 1;
  U0DLM = (SERIAL_DL >> 8) & 255;
  U0DLL = SERIAL_DL & 255;
  U0LCR_B.DLAB = 0;
  U0LCR_B.WLS = 3;
  U0LCR_B.SBS = 0;
  U0LCR_B.PE = 0;
  U0LCR_B.BC = 0;
  PINSEL0_B.PS_0 = 1; /* TXD */
  PINSEL0_B.PS_1 = 1; /* RXD */

  delay_us(100000);

  /* IRQ */
  serial_debug_mode = 0;

  serial_writer.wr_info = 0;
  serial_writer.wr_putchar = serial_write_char_info;

#if DEBUG_ENABLED
  exec_spinlock_init(&debug_lock);
#endif
};/*}}}*/
void serial_set_debug_mode(void)/*{{{*/
{
  U0IER_B.RBR_IE = 0;
  serial_debug_mode = 1;
}/*}}}*/
void serial_init_device(void)/*{{{*/
{
  serial_fifo_i = 0;
  serial_fifo_m = 0;

  exec_add_port(&serial_port, "serial", 0, PORT_FLAGS_IRQ);
  U0IER_B.RBR_IE = 1;
}/*}}}*/
int serial_read_line(char *u, int m, int echo)/*{{{*/
{
  u8 c;
  int i;

  i = 0;
  for(;;) {
    c = serial_read_char();
    if(echo) {
      if(c == 8 || c == 127) {
        if(i > 0) {
          serial_write_char(8);
          serial_write_char(' ');
          serial_write_char(8);
        }
        c = 8;
      } else serial_write_char(c);
    }
    if(c == 8) {
      if(i > 0) i --;
      continue;
    } else if(c == '\r') {
      serial_write_char('\n');
      break;
    } else {
      if(c != '\n') { /* Ignore LF */
        if(i + 1 < m) {
          u[i] = c;
          i ++;
        }
      }
    }
  }
  u[i] = 0;
  return i;
}/*}}}*/
char serial_maybe_read_char(bool *success)/*{{{*/
{
  char c;
  int got_char;
  critical section;
  question_node qn;

  if(success) *success = false;

  if(!serial_debug_mode) {
    /* DEBUGF("Serial read char"); */
    for(;;) {
      ENTER_CRITICAL(section);
      if(serial_fifo_m) {
        c = serial_fifo[(serial_fifo_i - serial_fifo_m) & SERIAL_FIFO_MASK];
        got_char = 1;
        serial_fifo_m --;
        got_char = 1;
      } else {
        c = 0;
        got_char = 0;
      }
      LEAVE_CRITICAL(section);
      if(got_char) {
        /* DEBUGF("Got char %d", c); */
        if(success) *success = true;
        return c;
      } else {
        if(success) {
          return 0;
        } else {
          /* FIFO is empty, will need to wait... */
          /* DEBUGF("FIFO empty, waiting"); */
          exec_question_init(&qn);
          exec_ask(&serial_port, &qn);
          exec_question_release(&qn);
        }
      }
    }
  } else {
    while(!U0LSR_B.RDR);
    if(success) *success = true;
    return U0RBR;
  }
}/*}}}*/
char serial_read_char(void)/*{{{*/
{
  return serial_maybe_read_char(0);
}/*}}}*/
void serial_write_string(const char *u)/*{{{*/
{
  while(*u) {
    serial_write_char(*u);
    u ++;
  }
}/*}}}*/
#if DEBUG_ENABLED
void serial_debugf(const char *u, ...)/*{{{*/
{
  va_list va;
  critical section;

  ENTER_CRITICAL(section);
  va_start(va, u);
  format_aprintf(&serial_writer, u, va);
  va_end(va);
  LEAVE_CRITICAL(section);
}/*}}}*/
void serial_debug(const char *function, int line, const char *message)
{
  critical section;

  ENTER_CRITICAL(section);
  serial_printf(YLW "DEBUG %s:%d %s" NML CRLF, function, line, message);
  LEAVE_CRITICAL(section);
}
#endif/*}}}*/
void serial_printf(const char *u, ...)/*{{{*/
{
  va_list va;

  va_start(va, u);
  format_aprintf(&serial_writer, u, va);
  va_end(va);
}
/*}}}*/
void serial_display_sp(void)/*{{{*/
{
  register u32 sp;

  asm volatile
    ("mov %0, sp\n"
     :"=r"(sp)
     :);
  serial_printf("SP=%p\r\n", (void *) sp);
}/*}}}*/
