/* command.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <config.h>
#include <system.h>
#include <serial.h>
#include <debug.h>
#include <assert.h>
#include <abort.h>
#include <machine.h>
#include <exec.h>
#include <led.h>
#include <keypad.h>
#include <joker.h>
#include <string.h>
#include <secret.h>
#include <command.h>
#include <indicator.h>
#include <timer.h>
#include <dsp_loop.h>
#include <fix.h>
#include <alarm.h>
#include <mmc.h>
#include <lcd.h>
#include <rtc.h>
#include <log.h>

task_node command_task;
u32 command_stack[COMMAND_STACK_SIZE];
u32 command_mode;
u32 command_privilege_level;
u32 *command_registers;

void dump_current_registers(void)/*{{{*/
{
  volatile u32 reg[17];
  volatile u32 *dst;

  dst = reg;

  asm volatile
     ("str r0, [%0]\n"
      "str r1, [%0, #4]\n"
      "str r2, [%0, #8]\n"
      "str r3, [%0, #12]\n"
      "str r4, [%0, #16]\n"
      "str r5, [%0, #20]\n"
      "str r6, [%0, #24]\n"
      "str r7, [%0, #28]\n"
      "str r8, [%0, #32]\n"
      "str r9, [%0, #36]\n"
      "str r10, [%0, #40]\n"
      "str r11, [%0, #44]\n"
      "str r12, [%0, #48]\n"
      "str r13, [%0, #52]\n"
      "str r14, [%0, #56]\n"
      "str r15, [%0, #60]\n"
      "str r15, [%0, #60]\n"
      "mrs r0, cpsr\n"
      "str r0, [%0, #64]\n"
      :
      :"r"(dst)
      :"r0"
      );

  machine_dump_registers((u32 *) reg, 0);
}/*}}}*/
int tokenize(char *u, int m, int *tokens, int *lengths, int n)/*{{{*/
{
  int i;
  int i0;
  int t;
  int q;

  i0 = 0;
  t = 0; /* Number of tokens */
  q = 0; /* 0 : spaces ; 1 : token */

  for(i = 0; i <= m; i++) {
    if(i == m) goto eof;
    switch(u[i]) {
      case ' ':
      case '\r':
      case '\n':
      case '\t':
      eof:
        if(q) {
          /* End of a token */
          if(t < n) {
            tokens[t] = i0;
            lengths[t] = i - i0;
            t ++;
            q = 0;
          } else {
            /* Too many tokens, abort */
            return -1;
          }
        } else {
          /* Nothing */
        }
        break;
      default:
        if(q) {
          /* Inside a token, do nothing */
        } else {
          /* Start of a new token */
          i0 = i;
          q = 1;
        }
        break;
    }
    if(!*u) break;
  }
  return t;
}/*}}}*/
bool dec_to_u32(char *u, int m, u32 *result)/*{{{*/
{
  u32 r;
  char c;

  r = 0;
  while(m --) {
    c = *(u ++);
    r *= 10;
    if('0' <= c && c <= '9') r += c & 15;
    else return 0;
  }
  *result = r;
  return true;
}/*}}}*/
int hex_to_u32(char *u, int m, u32 *result)/*{{{*/
{
  u32 r;
  char c;

  r = 0;
  while(m --) {
    c = *(u ++);
    r <<= 4;
    if('0' <= c && c <= '9') r += c & 15;
    else if('a' <= c && c <= 'f') r += (c - 87) & 15;
    else if('A' <= c && c <= 'F') r += (c - 75) & 15;
    else return 0;
  }
  *result = r;
  return 1;
}/*}}}*/
define_command(version)/*{{{*/
{
  serial_write_string("Operating system: Marie RTOS 1.2.2007-04-15\r\n"
                      "     Application: Jamcatch 1.0.2007-04-08\r\n"
                      "\r\n"
                      "Copyright (C)2007 Therodox R&D http://therodox.com/\r\n"
                      "All rights reserved.\r\n");

  return 0;
}/*}}}*/
int is_mapped(u32 x)/*{{{*/
{
  return
    (x < 0x80000) ||
    (0x40000000 <= x && x < 0x40008000) ||
    (0xe0000000 <= x);
}/*}}}*/
define_command(write)/*{{{*/
{
  u32 address;
  u32 data;
  
  if(hex_to_u32(command + tokens[0], lengths[0], &address)) {
    address |= extra << 28;
    if(is_mapped(address)) {
      if(hex_to_u32(command + tokens[1], lengths[1], &data)) {
        serial_printf("Memory[%x] =", address);
        *((u32 *) address) = data;
        serial_printf("%x" CRLF, data);
        return 0;
      } else return "Bad data";
    } else return "Not mapped";
  } else return "Bad address";
}/*}}}*/
define_command(search)/*{{{*/
{
  u32 address1;
  u32 address2;
  u32 data;
  u32 memory_data;
  char answer[4];
  int m;

  if(hex_to_u32(command + tokens[0], lengths[0], &address1) &&
     hex_to_u32(command + tokens[1], lengths[1], &address2) &&
     hex_to_u32(command + tokens[2], lengths[2], &data)) {
    address1 |= extra << 28;
    address2 |= extra << 28;
    if(!is_mapped(address1) || !is_mapped(address2)) return "Not mapped";
    if(address1 > address2) return "Empty range";
    serial_printf("Searching %x from %x to %x\r\n", data, address1, address2);
    while(address1 < address2) {
      memory_data = *((u32 *) address1);
      if(memory_data == data) {
        serial_printf("Hit at %x ", address1);
        for(;;) {
          serial_printf("[q:quit | CR:continue> ");
          m = serial_read_line(answer, sizeof(answer), 1);
          if(m == 1 && answer[0] == 'q') return 0;
          if(m == 0) break;
          serial_printf("Not understood ");
        }
      }
      address1 += 4;
    }
    return 0;
  } else return "Bad address or data";
}/*}}}*/
define_command(read)/*{{{*/
{
  u32 address;
  u32 data;
  u32 length;

  length = 1;
  
  if(hex_to_u32(command + tokens[0], lengths[0], &address)) {
    if(num_tokens == 2) {
      if(!hex_to_u32(command + tokens[1], lengths[1], &length)) {
        return "Bad length";
      }
    }

    while(length --) {
      address |= extra << 28;
      if(is_mapped(address)) {
        data = *((u32 *) address);
        serial_printf("%x %x | %x %u %d %i" CRLF, address, data, data, data, data, data);
      } else return "Not mapped";
      address += 4;
    }
    return 0;
  } else return "Bad address";
}/*}}}*/
define_command(go)/*{{{*/
{
  u32 address;
  u32 argument;

  argument = 0;
  
  if(hex_to_u32(command + tokens[0], lengths[0], &address)) {
    if(is_mapped(address)) {
      if(num_tokens == 2) {
        if(!hex_to_u32(command + tokens[1], lengths[1], &argument)) {
          return "Bad argument";
        }
      }
      serial_printf("Jumping to %x" CRLF, address);
      ((void (*)(u32))(address))(argument);
      return 0;
    } else return "Not mapped";
  } else return "Bad address";
}/*}}}*/
define_command(led)/*{{{*/
{
  u32 status;
  u32 led_number;
  led_t *led;
  
  if(hex_to_u32(command + tokens[0], lengths[0], &led_number) &&
     hex_to_u32(command + tokens[1], lengths[1], &status)) {
    switch(led_number) {
      case 0:
        led = &led_status;
        break;
      case 1:
        led = &led_a;
        break;
      case 2:
        led = &led_b;
        break;
      default:
        return "Invalid LED number";
    }
    switch(status) {
      case 0:
        led_clr(led);
        break;
      case 1:
        led_set(led);
        break;
      case 2:
        led_pattern(led, LED_FAST_BLINK, true);
        break;
      case 3:
        led_pattern(led, LED_SLOW_BLINK, true);
        break;
      default:
        return "Unknown pattern";
    }
    return 0;
  } else return "Bad status";
}/*}}}*/
define_command(checksum)/*{{{*/
{
  u32 sum;
  u32 *flash;
  int count;

  count = 2048;
  sum = 0;
  flash = (u32 *) 0;

  while(count --) {
    sum += *flash;
    flash ++;
  }

  serial_printf("Checksum is %x" CRLF, sum);

  return 0;
}/*}}}*/

#include <softfloat.h>

define_command(mmcins) { return mmc_insert(); }

bool check_quit(void)/*{{{*/
{
  bool success;
  char c;

  c = serial_maybe_read_char(&success);
  if(success) {
    switch(c) {
      case 'q':
        return true;
      default:
        break;
    }
  }
  return false;
}/*}}}*/
define_command(keypad) {/*{{{*/
  u8 r;
  for(;;) {
    r = keypad_get_raw();
    serial_printf("Keypad : %c%c%c%c %x\r",
	((r & (1 << KEYPAD_RIGHT)) ? 'R':' '),
	((r & (1 << KEYPAD_UP))    ? 'U':' '),
	((r & (1 << KEYPAD_DOWN))  ? 'D':' '),
	((r & (1 << KEYPAD_LEFT))  ? 'L':' '),
	r);
    timer_wait(TICKS(0.1));
    if(check_quit()) break;
  }
  serial_printf(CRLF "Bye." CRLF);
  return 0;
}/*}}}*/
define_command(ddump) {/*{{{*/
  u32 mode;

  if(hex_to_u32(command + tokens[0], lengths[0], &mode)) {
    detector_set_dump(detectors + 0, mode);
    if(mode != DETECTOR_DUMP_NONE) {
      for(;;) {
        if(check_quit()) break;
        timer_wait(TICKS(0.1));
      }
      detector_set_dump(detectors + 0, DETECTOR_DUMP_NONE);
    }
  }
  return 0;
}/*}}}*/


define_command(log)/*{{{*/
{
  u16 i;
  time_t t;
  log_entry_t *e;

  serial_printf("Log count=%d i=%d" CRLF, log_table.log_count, log_table.log_index);
  for(i = 0; i < log_table.log_count; i ++) {
    e = log_table.log_entries + i;
    XDEBUGF("table=%p e=%p i=%d", &log_table, e, i);
    rtc_decode(&t, e->log_timestamp);
    serial_printf("  #%d ", i);
    serial_printf("%04u-%02u-%02u %02u:%02u:%02u ",
               t.t_year,
               t.t_month,
               t.t_day,
               t.t_hours,
               t.t_minutes,
               t.t_seconds);

    switch(e->log_event_kind) {
      case EVENT_BOOT:
        serial_printf("Boot");
        break;
      case EVENT_ALARM:
        serial_printf("Alarm");
        break;
      default:
        serial_printf("Unknown");
        break;
    }

    serial_printf(CRLF);
  }

  return 0;
}/*}}}*/
define_command(mmcsr)/*{{{*/
{
  u32 serial;

  if(mmc_get_serial(&serial)) {
    serial_printf("Serial %x" CRLF, serial);
    return 0;
  } else return "MMC error";
}/*}}}*/

#if 0
static u32 data[MMC_BLOCK_SIZE / 4];
define_command(mmcrd)/*{{{*/
{
  int i;
  u32 block;

  if(hex_to_u32(command + tokens[0], lengths[0], &block)) {
    serial_printf("Reading block %d" CRLF, block);
    if(mmc_read_block(block, (u8 *) data)) {
      for(i = 0; i < MMC_BLOCK_SIZE / 4; i ++) {
        if(!(i & 7)) {
          if(i) serial_printf(CRLF);
          serial_printf("%d", i);
        }
        serial_printf(" %x", data[i]);
      }
      serial_printf(CRLF);
    } else return "Read error";
  } else return "Bad block number";
  return 0;
}/*}}}*/
define_command(mmcwr)/*{{{*/
{
  u32 block;

  if(hex_to_u32(command + tokens[0], lengths[0], &block)) {
    serial_printf("Writing block %d" CRLF, block);
    if(mmc_write_block(block, (u8 *) data)) {
      serial_printf("OK" CRLF);
    } else return "Write error";
  } else return "Bad block number";
  return 0;
}/*}}}*/
#endif

define_command(reg)/*{{{*/
{
  if(command_registers) {
    machine_dump_registers(command_registers, 1);
  } else {
    dump_current_registers();
  }
  return 0;
}/*}}}*/

define_command(li)    { indicator_list_available(); return 0; }
define_command(lt)    { exec_list_tasks(extra); return 0; }
define_command(lp)    { exec_list_ports(extra); return 0; }
define_command(abrt)  { abort("User request"); return 0; }

define_command(freeze)/*{{{*/
{
  u32 t;
  task_node *tn;

  if(hex_to_u32(command + tokens[0], lengths[0], &t)) {
    tn = (task_node *) t;
    serial_printf("Freezing task %p" CRLF, tn);
    exec_freeze(tn);
  } else return "Bad task number";
  return 0;
}/*}}}*/

#define MAX_INDICATORS 20

define_command(wi)/*{{{*/
{
  int i;
  int line;
  int num_indicators;
  int kind_column;
  int value_column;
  bool first_run;
  static indicator_node *in[MAX_INDICATORS];
  static data_t dt[MAX_INDICATORS];

  num_indicators = num_tokens;
  if(num_indicators > MAX_INDICATORS) return "Too many indicators";

  if(!num_indicators) {
    indicator_node *in_first;

    in_first = indicator_list_head(&indicators);
    for(i = 0; i < MAX_INDICATORS; i ++) {
      if(!in_first) break;
      in[i] = in_first;
      in_first = indicator_node_next(in_first);
    }
    num_indicators = i;
  } else {
    for(i = 0; i < num_indicators; i ++) {
      in[i] = indicator_find(command + tokens[i], lengths[i]);
      if(!in[i]) return "No such indicator";
    }
  }

  kind_column = 20;
  value_column = 40;
  serial_printf(CLS);
  line = 1;
  serial_printf(MOV "Name" MOV "| Kind" MOV "| Value" CRLF, line, 0, line, kind_column, line, value_column);
  serial_printf("--------------------------------------------------------------------------" CRLF);
  first_run = true;
  for(;;) {
    line = 3;
    for(i = 0; i < num_indicators; i ++) {
      if(first_run) {
        serial_printf(MOV, line, 0);
        serial_printf(CEOL YLW "%s" NML, in[i]->indicator.in_name);
        serial_printf(MOV "| ", line, kind_column);
        switch(in[i]->indicator.in_data.dt_kind) {
          case KD_U32:
            serial_printf("u32");
            break;
          case KD_S32:
            serial_printf("s32");
            break;
          case KD_F32:
            serial_printf("f32");
            break;
          case KD_F64:
            serial_printf("f64");
            break;
          case KD_BOOL:
            serial_printf("bool");
            break;
          case KD_FIX:
            serial_printf("fix");
            break;
          case KD_STRING:
            serial_printf("string");
            break;
        }
      }
      if(first_run || !word_compare(dt + i, &in[i]->indicator.in_data, sizeof(data_t))) {
        word_copy(&in[i]->indicator.in_data, dt + i, sizeof(data_t));
        switch(in[i]->indicator.in_data.dt_kind) {
          case KD_U32:
            serial_printf(MOV "| %u", line, value_column, in[i]->indicator.in_data.dt_u32.value);
            break;
          case KD_S32:
            serial_printf(MOV "| %d", line, value_column, in[i]->indicator.in_data.dt_s32.value);
            break;
          case KD_F32:
            serial_printf(MOV "| %f", line, value_column, in[i]->indicator.in_data.dt_f32.value);
            break;
          case KD_F64:
            serial_printf(MOV "| %lf", line, value_column, in[i]->indicator.in_data.dt_f64.value);
            break;
          case KD_FIX:
            serial_printf(MOV "| %i", line, value_column, in[i]->indicator.in_data.dt_fix.value);
            break;
          case KD_BOOL:
            serial_printf(MOV "| %s", line, value_column, in[i]->indicator.in_data.dt_bool ? "true":"false");
            break;
          case KD_STRING:
            serial_printf(MOV "| %s", line, value_column, in[i]->indicator.in_data.dt_string);
            break;
        }
        serial_printf(CEOL);
      }
      line ++;
    }
    first_run = false;
    serial_printf(MOV, line, 0);
    timer_wait(TICKS(1.0));
    if(check_quit()) break;
  }
  serial_printf(CLS);
  return 0;
}/*}}}*/
#if 0
static fix_t norm2(fix_t x, fix_t y)
{
  return mul(x,x) + mul(y,y);
}
define_command(fftdat)/*{{{*/
{
  question_node qn;
  static fix_t adc_fft_buffer[DSP_FFT_SIZE * 2];
  fix_t xr, xi;
  fix_t nx;
  fix_t f;
  int i;

  exec_question_init(&qn);
  qn.question.q_question = DSP_FFT_REQUEST;
  qn.question.q_question_extra = (u32) adc_fft_buffer;
  XDEBUGF("Asking");
  exec_ask(&dsp_port, &qn);
  if(!qn.question.q_answer) {
    for(i = 0; i < DSP_FFT_SIZE / 2; i ++) {
      xr = adc_fft_buffer[2 * i];
      xi = adc_fft_buffer[2 * i + 1];
      nx = norm2(xr, xi);
      f = ((ADC_FREQUENCY * i) / DSP_FFT_SIZE);

      serial_printf("%d %d %i %i %i" CRLF, i, f, xr, xi, nx);
    }
  }
  exec_question_release(&qn);
  return (char *) qn.question.q_answer;
}/*}}}*/
#endif
define_command(adcdat)/*{{{*/
{
  question_node qn;
  static fix_t adc_buffer[ADC_BUFFER_LENGTH];
  int i;
  u32 x;

  exec_question_init(&qn);
  qn.question.q_question = DSP_DATA_REQUEST;
  qn.question.q_question_extra = (u32) adc_buffer;
  XDEBUGF("Asking");
  exec_ask(&dsp_port, &qn);
  if(!qn.question.q_answer) {
    if(extra) {
      for(i = 0; i < ADC_BUFFER_LENGTH; i ++) {
        x = *((u32 *) (adc_buffer + i));
        if(!(i & 15)) serial_printf(CRLF);
        serial_printf("%x", x);
      }
    } else {
      for(i = 0; i < ADC_BUFFER_LENGTH; i ++) {
        serial_printf(i ? " %i":"%i", adc_buffer[i]);
      }
    }
    serial_printf(CRLF);
  }
  exec_question_release(&qn);
  return (char *) qn.question.q_answer;
}/*}}}*/
define_command(signal)/*{{{*/
{
#if 0
  task *t;
  u32 signal;

  t = exec_find_task(command + tokens[0], lengths[0]);
  if(!t) return "No such task";
  else {
    if(hex_to_u32(command + tokens[1], lengths[1], &signal)) {
      exec_signal(t, signal);
      return 0;
    } else return "Bad signal";
  }
#else
  return 0;
#endif
}/*}}}*/
define_command(passwd)/*{{{*/
{
  u32 hash[2];
  char *error;

  error = secret_hash_password(command + tokens[0], lengths[0], hash);
  if(error) return error;
  serial_printf("{ 0x%x, 0x%x };\r\n", hash[0], hash[1]);
  return 0;
}/*}}}*/
define_command(su)/*{{{*/
{
  char *user;
  u32 user_length;
  char *password;
  u32 password_length;
  char *error;

  if(extra) {
    user = "root";
    user_length = 4;
    password = command + tokens[0];
    password_length = lengths[0];
  } else {
    user = command + tokens[0];
    user_length = lengths[0];
    password = command + tokens[1];
    password_length = lengths[1];
  }

  error = secret_attempt_login(user, user_length, password, password_length, &command_privilege_level);
  if(error) return error;
  serial_printf("Your privilege level is %x\r\n", command_privilege_level);
  return 0;
}/*}}}*/
define_command(alm)/*{{{*/
{
  alarm_trigger();
  return 0;
}/*}}}*/
define_command(alen)/*{{{*/
{
  if(extra) alarm_enable();
  else alarm_disable();
  return 0;
}/*}}}*/
define_command(setat)/*{{{*/
{
  u32 at_r;
  float32 at_f;

  if(hex_to_u32(command + tokens[0], lengths[0], &at_r)) {
    at_f = float32_div(int32_to_float32(at_r), int32_to_float32(1000));
    serial_printf("Alarm threshold %f" CRLF, at_f);
    detectors[0].dt_alarm_threshold = at_f;
    return 0;
  } else return "Bad integer";
}/*}}}*/
define_command(dclas)/*{{{*/
{
  detector_dump_classifier(detectors);
  return 0;
}/*}}}*/
define_command(ddotf)/*{{{*/
{
  detector_dump_dot_fft(detectors);
  return 0;
}/*}}}*/
define_command(relearn)/*{{{*/
{
  detector_reset(detectors);
  return 0;
}/*}}}*/
define_command(lcd)/*{{{*/
{
  u32 x;

  if(hex_to_u32(command + tokens[0], lengths[0], &x)) {
    lcd_clear();
    lcd_printf("Integer %x\nIs also %d", x, x);
    lcd_refresh();
    return 0;
  } else return "Bad integer";
}/*}}}*/
define_command(joker)/*{{{*/
{
  joker_init();
  return 0;
}/*}}}*/
define_command(stime)/*{{{*/
{
  u32 date[6];
  int i;
  time_t t;

  for(i = 0; i < 6; i++) {
    if(!dec_to_u32(command + tokens[i], lengths[i], date + i))
      return "Bad integer";
  }

  serial_printf("Setting date & time to %04u-%02u-%02u %02u:%02u:%02u" CRLF,
                date[0],
                date[1],
                date[2],
                date[3],
                date[4],
                date[5]);

  t.t_year    = date[0];
  t.t_month   = date[1];
  t.t_day     = date[2];
  t.t_hours   = date[3];
  t.t_minutes = date[4];
  t.t_seconds = date[5];
  rtc_set_time(&t);
  return 0;
}/*}}}*/

declare_command(help);

const command_entry commands[] = {/*{{{*/
  { "su",       2, 2, 0, cmd_su,       EVERYBODY                       },
  { "sur",      1, 1, 1, cmd_su,       EVERYBODY                       },
  { "version",  0, 0, 0, cmd_version,  EVERYBODY                       },
  { "help",     0, 0, 0, cmd_help,     EVERYBODY                       },
  { "?",        0, 0, 0, cmd_help,     EVERYBODY                       },
  { "led",      2, 2, 0, cmd_led,      AT_LEAST(E_USER)                },
  { "log",      0, 0, 0, cmd_log,      AT_LEAST(E_USER)                },
  { "joker",    0, 0, 0, cmd_joker,    AT_LEAST(E_DEVELOPER)           },
  { "alen",     0, 0, 1, cmd_alen,     AT_LEAST(E_USER)                },
  { "aldis",    0, 0, 0, cmd_alen,     AT_LEAST(E_USER)                },
  { "setat",    1, 1, 0, cmd_setat,    AT_LEAST(E_USER)                },
  { "dclas",    0, 0, 0, cmd_dclas,    AT_LEAST(E_MAINTAINER)          },
  { "ddotf",    0, 0, 0, cmd_ddotf,    AT_LEAST(E_MAINTAINER)          },
  /*{ "fftdat",   0, 0, 0, cmd_fftdat,   AT_LEAST(E_MAINTAINER)          },*/
  { "adcdat",   0, 0, 0, cmd_adcdat,   AT_LEAST(E_MAINTAINER)          },
  { "adcdatx",  0, 0, 1, cmd_adcdat,   AT_LEAST(E_MAINTAINER)          },
  { "lt",       0, 0, 0, cmd_lt,       AT_LEAST(E_MAINTAINER)          },
  { "ltd",      0, 0, 1, cmd_lt,       AT_LEAST(E_MAINTAINER)          },
  { "lp",       0, 0, 0, cmd_lp,       AT_LEAST(E_MAINTAINER)          },
  { "relearn",  0, 0, 0, cmd_relearn,  AT_LEAST(E_USER)                },
  { "lpd",      0, 0, 1, cmd_lp,       AT_LEAST(E_MAINTAINER)          },
  { "alm",      0, 0, 1, cmd_alm,      AT_LEAST(E_MAINTAINER)          },
  { "li",       0, 0, 0, cmd_li,       AT_LEAST(E_USER)                },
  { "wi",       0, 9, 0, cmd_wi,       AT_LEAST(E_USER)                },
  { "stime",    6, 6, 0, cmd_stime,    AT_LEAST(E_USER)                },
  { "sc",       3, 3, 0, cmd_search,   E_DEVELOPER                     },
  { "sc4",      3, 3, 4, cmd_search,   E_DEVELOPER                     },
  { "r",        1, 2, 0, cmd_read,     E_DEVELOPER                     },
  { "r4",       1, 2, 4, cmd_read,     E_DEVELOPER                     },
  { "g",        1, 2, 0, cmd_go,       E_DEVELOPER                     },
  { "w",        2, 2, 0, cmd_write,    E_DEVELOPER                     },
  { "w4",       2, 2, 4, cmd_write,    E_DEVELOPER                     },
  { "reg",      0, 0, 0, cmd_reg,      E_DEVELOPER                     },
  { "ddump",    1, 1, 0, cmd_ddump,    E_DEVELOPER                     },
  { "keypad",   0, 0, 0, cmd_keypad,   E_DEVELOPER                     },
  { "mmcins",   0, 0, 0, cmd_mmcins,   E_DEVELOPER,                    },
#if 0
  { "mmcrd",    1, 1, 0, cmd_mmcrd,    E_DEVELOPER,                    },
  { "mmcwr",    1, 1, 0, cmd_mmcwr,    E_DEVELOPER,                    },
#endif
  { "mmcsr",    0, 0, 0, cmd_mmcsr,    E_DEVELOPER,                    },
  { "checksum", 0, 0, 0, cmd_checksum, E_DEVELOPER                     },
  { "signal",   2, 2, 0, cmd_signal,   E_DEVELOPER                     },
  { "abort",    0, 0, 0, cmd_abrt,     E_DEVELOPER                     },
  { "freeze",   1, 1, 0, cmd_freeze,   E_DEVELOPER                     },
  { "lcd",      1, 1, 0, cmd_lcd,      E_DEVELOPER                     },
  { "passwd",   1, 1, 0, cmd_passwd,   IF_BETA(EVERYBODY, E_DEVELOPER) },
};/*}}}*/

#define NUM_COMMANDS (sizeof(commands)/sizeof(*commands))

define_command(help)/*{{{*/
{
  int i;
  int j;
  const command_entry *ce;

  serial_write_string("Available commands:\r\n");
  for(i = 0; i < NUM_COMMANDS; i++) {
    ce = commands + i;
    if(ce->ce_required_privileges & command_privilege_level) {
      serial_printf("  %s", ce->ce_command);
      for(j = 0; j < ce->ce_min_params; j ++) {
        serial_printf(" <arg%d>", j);
      }
      for(j = ce->ce_min_params; j < ce->ce_max_params; j ++) {
        serial_printf(" [arg%d]", j);
      }
      serial_write_string("\r\n");
    }
  }
  return 0;
}/*}}}*/
void command_loop(void *arg)/*{{{*/
{
  char *error;
  char *msg;
  static char command[80];
  static int command_length;
  static int tokens[10];
  static int lengths[10];
  static int num_tokens;
  int i;
  const command_entry *ce;

  command_privilege_level = INITIAL_PRIVILEGE_LEVEL;
  command_registers = (u32 *) arg;

  serial_write_string("Entering monitor\r\n");
  for(;;) {
    serial_write_string(BLU "> " NML);
#if 0
    if(!command_mode) {
      int counter;
      serial_printf("Hihi...\r\n");
      for(counter = 0; ; counter ++) {
        if(counter == 60000000) {
          serial_printf("Hello %d\r\n", counter);
          counter = 0;
        }
      }
    }
#endif
    command_length = serial_read_line(command, sizeof(command), 1);
    error = 0;
    msg = 0;
    num_tokens = tokenize(command, command_length, tokens, lengths, sizeof(tokens)/sizeof(*tokens));

    if(num_tokens < 0) {
      error = "Too many tokens";
    } else {
      if(num_tokens) {
        error = "Unknown command";
        for(i = 0; i < NUM_COMMANDS; i ++) {
          ce = commands + i;
          if(strings_equal(ce->ce_command,
                           string_length(ce->ce_command),
                           command + tokens[0],
                           lengths[0])) {
            XDEBUGF("Required %x, got %x", ce->ce_required_privileges, command_privilege_level);
            if(ce->ce_required_privileges & command_privilege_level) {
              if(ce->ce_min_params <= num_tokens - 1 &&
                 num_tokens - 1 <= commands[i].ce_max_params) {
                error = ce->ce_handler(commands[i].ce_extra, command, tokens + 1, lengths + 1, num_tokens - 1);
              } else {
                error = "Wrong number of parameters";
                break;
              }
            } else {
              error = "Insufficient privileges";
            }
          }
        }
      }
    }

    if(error) {
      serial_printf(RED "ERROR" NML " %s" CRLF, error);
    }
  }
}/*}}}*/
void command_init()/*{{{*/
{
  command_mode = 0;
  exec_add_task(&command_task, "cli", command_loop, 0, command_stack, length(command_stack), COMMAND_PRI);
}/*}}}*/
void crash_handler(u32 *registers, int reason)/*{{{*/
{
  serial_set_debug_mode();
  serial_printf(CRLF CRLF NML RED "** CRASH ");
  switch(reason) {
    case 11:
      serial_printf("(undefined instruction)");
      break;
    case 22:
      serial_printf("(prefetch abort)");
      break;
    case 33:
      serial_printf("(data abort)");
      break;
    case 44:
      serial_printf("(software interrupt)");
      break;
    case 55:
      serial_printf("(fast interrupt)");
      break;
    case 66:
      serial_printf("(abortion)");
      break;
    default:
      serial_printf("(invalid code %d)", reason);
      break;
  }
  serial_printf(" **" NML CRLF);
  machine_dump_registers(registers, 1);

  if(abort_cookie == ABORT_COOKIE && abort_msg != 0) {
    serial_printf(RED "** ABORTION CAUSE: %s **" NML CRLF, abort_msg);
    abort_cookie = 0;
    abort_msg = 0;
  }

  command_mode = 1;
  command_loop(registers);
}/*}}}*/
