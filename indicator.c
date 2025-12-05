/* indicator.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <indicator.h>
#include <exec.h>
#include <config.h>
#include <serial.h>
#include <string.h>
#include <debug.h>

indicator_list indicators;
u32 indicator_count;

void indicator_init(void)/*{{{*/
{
  indicator_list_init(&indicators);
  indicator_count = 0;
}/*}}}*/
void indicator_add(indicator_node *in, string name)/*{{{*/
{
  DEBUGF("Add %p %s", in, name);
  in->indicator.in_name = name;
  in->indicator.in_identifier = indicator_count;
  indicator_count ++;
  indicator_list_append(&indicators, in);
}/*}}}*/
indicator_node *indicator_find(const char *name, int length)/*{{{*/
{
  indicator_node *in;

  /* XXX: Mutexify */
  in = indicator_list_head(&indicators);
  while(in) {
    if(strings_equal(in->indicator.in_name, string_length(in->indicator.in_name), name, length)) {
      return in;
    }
    in = indicator_node_next(in);
  }

  return 0;
}/*}}}*/
void indicator_list_available(void)/*{{{*/
{
  indicator_node *in;
  data_t dt;

  /* XXX: Mutexify */
  serial_printf("Indicators:" CRLF);

  in = indicator_list_head(&indicators);
  while(in) {
    serial_printf(" " YLW " %p" NML " %u ", in, in->indicator.in_identifier);
    dt = in->indicator.in_data;
    serial_printf(BLU);
    switch(dt.dt_kind) {
      case KD_BOOL:
        serial_printf("%s", dt.dt_bool ? "true" : "false");
        break;
      case KD_U32:
        serial_printf("%u <= %u <= %u", dt.dt_u32.min, dt.dt_u32.value, dt.dt_u32.max);
        break;
      case KD_S32:
        serial_printf("%d <= %d <= %d", dt.dt_s32.min, dt.dt_s32.value, dt.dt_s32.max);
        break;
      case KD_FIX:
        serial_printf("%i <= %i <= %i", dt.dt_fix.min, dt.dt_fix.value, dt.dt_fix.max);
        break;
      case KD_F32:
        serial_printf("%f <= %f <= %f", dt.dt_f32.min, dt.dt_f32.value, dt.dt_f32.max);
        break;
      case KD_F64:
        serial_printf("%lf <= %lf <= %lf", dt.dt_f64.min, dt.dt_f64.value, dt.dt_f64.max);
        break;
      case KD_STRING:
        serial_printf("%s", dt.dt_string);
        break;

    }
    serial_printf(" | " YLW "%s" NML CRLF, in->indicator.in_name);
    in = indicator_node_next(in);
  }
}/*}}}*/
void indicator_fix_init(indicator_node *in)/*{{{*/
{
  in->indicator.in_data.dt_kind = KD_FIX;
  in->indicator.in_data.dt_fix.value = 0;
  in->indicator.in_data.dt_fix.min = (fix_t) 0x80000000;
  in->indicator.in_data.dt_fix.max = (fix_t) 0x7fffffff;
}/*}}}*/
void indicator_s32_init(indicator_node *in)/*{{{*/
{
  in->indicator.in_data.dt_kind = KD_S32;
  in->indicator.in_data.dt_s32.value = 0;
  in->indicator.in_data.dt_s32.min = (s32) 0x80000000;
  in->indicator.in_data.dt_s32.max = (s32) 0x7fffffff;
}/*}}}*/
void indicator_f32_init(indicator_node *in)/*{{{*/
{
  in->indicator.in_data.dt_kind = KD_F32;
  in->indicator.in_data.dt_f32.value = 0;
  in->indicator.in_data.dt_f32.min = 0;
  in->indicator.in_data.dt_f32.max = 0;
}/*}}}*/
void indicator_f64_init(indicator_node *in)/*{{{*/
{
  in->indicator.in_data.dt_kind = KD_F64;
  in->indicator.in_data.dt_f64.value = int32_to_float64(0);
  in->indicator.in_data.dt_f64.min = int32_to_float64(0);
  in->indicator.in_data.dt_f64.max = int32_to_float64(0);
}/*}}}*/
void indicator_string_init(indicator_node *in)/*{{{*/
{
  in->indicator.in_data.dt_kind = KD_STRING;
  in->indicator.in_data.dt_string = "?";
}/*}}}*/
void indicator_u32_init(indicator_node *in)/*{{{*/
{
  in->indicator.in_data.dt_kind = KD_U32;
  in->indicator.in_data.dt_u32.value = 0;
  in->indicator.in_data.dt_u32.min = 0x00000000;
  in->indicator.in_data.dt_u32.max = 0xffffffff;
}/*}}}*/
void indicator_bool_init(indicator_node *in)/*{{{*/
{
  in->indicator.in_data.dt_kind = KD_BOOL;
  in->indicator.in_data.dt_bool = 0;
}/*}}}*/
