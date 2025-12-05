/* adc.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <lpc2138.h>
#include <system.h>
#include <config.h>
#include <adc.h>
#include <debug.h>
#include <exec.h>

adc_buffer_t adc_buffer;
u16 adc_buffer_even[ADC_BUFFER_LENGTH];
u16 adc_buffer_odd[ADC_BUFFER_LENGTH];
port_node adc_port;

void adc_hardware_init(void)/*{{{*/
{
  ADCR_R reg;

  /* Prepare TIMER1 */
  T1MR0 = TIMER1_PERIOD;
  T1MCR_B.MR0I = 0; /* Don't generate interrupt on match */
  T1MCR_B.MR0R = 1; /* Reset on match */
  T1EMR_B.EMC0 = 3; /* Toggle MAT1.0 */

  T1PR = 0; /* No prescaling */
  T1TCR_B.CR = 1; /* Reset */
  T1TCR_B.CE = 1; /* Enable */
  T1TCR_B.CR = 0; /* Reset */

  /* Prepare AD0 */
  PINSEL1_B.PS_27 = 1;
  reg = AD0CR_B;
  reg.SEL = 1;
  reg.CLKDIV = 13;
  reg.CLKS = 0;
  reg.PDN = 1;
  reg.BURST = 0;
  reg.START = 6; /* MAT1.0 */
  reg.EDGE = 0; /* Rising edge */
  AD0CR_B = reg;
}/*}}}*/
void adc_software_irq(void)/*{{{*/
{
  question_node *qn;

  XDEBUGF("ADC software IRQ");
  qn = exec_check_question(&adc_port);
  if(qn) {
    XDEBUGF("Got a question %p", qn);
    if(adc_buffer.ab_overrun) {
      qn->question.q_answer = ADC_OVERRUN;
      adc_buffer.ab_overrun = 0;
    } else {
      qn->question.q_answer = ADC_OK;
    }
    adc_buffer.ab_ack = 1;
    if(adc_buffer.ab_writing) {
      qn->question.q_answer_extra = (u32) adc_buffer.ab_odd; /* Even is being filled */
    } else {
      qn->question.q_answer_extra = (u32) adc_buffer.ab_even; /* Even is being filled */
    }
    exec_reply(&adc_port, qn, false);
  }

  XDEBUGF("Leaving interrupt");
  VICsoftIntClr |= 1 << VIC_SOFTWARE;
  VICvectAddr = 0;
  exec_schedule(EXEC_OTHER_INTERRUPT);
}/*}}}*/
u16 adc_get_sample(void)/*{{{*/
{
  u16 x;

  while(!AD0GDR_B.DONE);
  x = AD0GDR_B.RESULT;
  AD0CR_B.START = 1;
  return x;
}/*}}}*/
u32 adc_get_buffer(question_node *qn, u16 **target)/*{{{*/
{
  XDEBUGF("ADC get buffer");
  exec_ask(&adc_port, qn);
  XDEBUGF("Got target %p", target);
  *target = (u16 *) qn->question.q_answer_extra;
  return qn->question.q_answer;
}/*}}}*/
void adc_device_init(void)/*{{{*/
{
  exec_add_port(&adc_port, "adc", 0, PORT_FLAGS_IRQ);
}/*}}}*/
