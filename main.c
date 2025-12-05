/* main.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <config.h>
#include <system.h>
#include <interrupts.h>
#include <serial.h>
#include <exec.h>
#include <dsp_loop.h>
#include <dac.h>
#include <adc.h>
#include <pwm.h>
#include <led.h>
#include <timer.h>
#include <command.h>
#include <debug.h>
#include <assert.h>
#include <crash.h>
#include <abort.h>
#include <indicator.h>
#include <alarm.h>
#include <softfloat.h>
#include <ssp.h>
#include <mmc.h>
#include <watchdog.h>
#include <keypad.h>
#include <random.h>
#include <lcd.h>
#include <log.h>
#include <rtc.h>
#include <ui.h>
#include <disk.h>
#include <joker.h>

#define CRLF "\r\n"

#define BANNER \
CRLF \
CRLF \
" _____________________________________________ " CRLF \
"|                        _  ____ _   __       |" CRLF \
"| ._ _   _. ._ o  _     /_>  /  / \\  \\_       |" CRLF \
"| | | | (_| |  | (/_   / \\  /   \\_/ __/   1.0 |" CRLF \
"|                                             |" CRLF \
"|                        (C)2007 Therodox R&D |" CRLF \
" ============================================= " CRLF

void run(void)
{
  int i;

#if KMD
  extern void prepare(void);
  prepare();
#else
  system_init();

  serial_init();

  for(i = 0; i < 256; i ++) {
    serial_write_string(CRLF);
  }
  serial_write_string(BANNER);
  DEBUG("Serial OK");

  DEBUG("Watchdog init");
  watchdog_init();

  DEBUG("Timer prepare");
  timer_prepare();

  DEBUG("Exec");
  exec_init();

  DEBUG("Interrupts");
  interrupts_init();

#if 1
  DEBUG("GPIO");
  gpio_init();

  DEBUG("Led");
  led_init();

  DEBUG("Keypad");
  keypad_init();
#endif

#if 0
  DEBUG("DAC");
  dac_init();
#endif

#if 1
  DEBUG("ADC HW");
  adc_hardware_init();

  DEBUG("PWM");
  pwm_init();

  DEBUG("Indicator");
  indicator_init();
#endif

  DEBUG("Command");
  command_init();

  DEBUG("Timer start");
  timer_start();

#if 1
  DEBUG("RTC");
  rtc_init();
#endif

  serial_init_device();
  DEBUG("Serial device");

#if 1
  lcd_init();
  DEBUG("LCD");
#endif

  timer_init_device();
  DEBUG("Timer device");

#if 1
  adc_device_init();
  DEBUG("ADC Device");

  alarm_init();
  DEBUG("Alarm");

  float_init();
  DEBUG("Softfloat");
  
  ssp_init();
  DEBUG("SSP");

  mmc_init();
  DEBUG("MMC");

  disk_init();
  DEBUG("Disk");

  log_init();
  DEBUG("Log");

  dsp_loop_init();
  DEBUG("DSP");

  ui_init();
  DEBUG("UI");
#endif

  FIQENA();
  DEBUG("Enabled FIQ");

  //joker_init();
  //DEBUG("Joker");

  watchdog_start();
  DEBUG("Watchdog start");

  /* exec_schedule will enable IRQs */
  DEBUG("Scheduling");
  exec_start();

  DEBUG("Not reached");
#endif
}
