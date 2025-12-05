/* machine.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <serial.h>
#include <machine.h>

#define R_NEWLINE (-1)
#define R_BANKED (-2)

typedef struct {
  u32 r_offset;
  string r_name;
} register_t;

static const register_t register_names[] =/*{{{*/
{
  {   0, "      r0" },
  {   4, "  r1" },
  {   8, "  r2" },
  {  12, "  r3" },
  { R_NEWLINE, 0 },
  {  16, "      r4" },
  {  20, "  r5" },
  {  24, "  r6" },
  {  28, "  r7" },
  { R_NEWLINE, 0 },
  {  32, "      r8" },
  {  36, "  r9" },
  {  40, " r10" },
  {  44, " r11" },
  { R_NEWLINE, 0 },
  {  48, "     r12" },
  {  52, " r13" },
  {  56, " r14" },
  {  60, " r15" },
  { R_NEWLINE, 0 },
  {  64, "      sr" },
  { R_NEWLINE, 0 },
  { R_BANKED, 0 },
  {  76, "svc spsr" },
  {  68, " r13" },
  {  72, " r14" },
  { R_NEWLINE, 0 },
  {  88, "abt spsr" },
  {  80, " r13" },
  {  84, " r14" },
  { R_NEWLINE, 0 },
  { 100, "und spsr" },
  {  92, " r13" },
  {  96, " r14" },
  { R_NEWLINE, 0 },
  { 112, "irq spsr" },
  { 104, " r13" },
  { 108, " r14" },
  { R_NEWLINE, 0 },
  { 144, "fiq spsr" },
  { R_NEWLINE, 0 },
  { 116, "fiq   r8" },
  { 120, "  r9" },
  { 124, " r10" },
  { 128, " r11" },
  { R_NEWLINE, 0 },
  { 132, "fiq  r12" },
  { 136, " r13" },
  { 140, " r14" },
  { R_NEWLINE, 0 },
};/*}}}*/
void machine_dump_registers(u32 *reg, u32 banked)/*{{{*/
{
  int i;

  for(i = 0; i < length(register_names); i++) {
    switch(register_names[i].r_offset) {
      case R_NEWLINE:
        serial_write_string("\r\n");
        break;
      case R_BANKED:
        if(!banked) return;
        break;
      default:
        serial_printf(BLU "%s" NML " %x", register_names[i].r_name, reg[register_names[i].r_offset >> 2]);
        break;
    }
  }
}/*}}}*/
