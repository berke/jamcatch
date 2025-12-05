/* ENTRY(_boot) */
MEMORY
{
  rom (rx) : ORIGIN = 0x00000000, LENGTH = 500k
  ram (rw) : ORIGIN = 0x40000000, LENGTH = 32k
}

SECTIONS
{
  .init : {
    /* *test1.o ( .text ) */
    /* PROVIDE(foo = .); */
  } > rom

  .text : {
    *(.text)                   /* remaining code */
    *(.rodata)                 /* read-only data (constants) */
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
  } > rom

  .bss (NOLOAD) :
  {
    __bss_start = . ;
    __bss_start__ = . ;
    *(.bss)
    *(COMMON)
    . = ALIGN(4);
  } > ram

  . = ALIGN(4);
  __bss_end__ = . ;
  PROVIDE (__bss_end = .);

  _end = . ;
  PROVIDE (end = .);

  . = ORIGIN(ram) + LENGTH(ram) - 64;

  . -=  4; PROVIDE (abort_msg      = . );
  . -=  4; PROVIDE (abort_cookie   = . );
  . -=  4; /* XXX */
           PROVIDE (crash_end      = . );
  . -=  4; PROVIDE (crash_pc       = . );
  . -=  4; PROVIDE (crash_info     = . );
  . -=  4; PROVIDE (crash_cookie   = . );

  . -= (37 * 4);
           PROVIDE (crash_regs     = . );

  . -= 512;
           PROVIDE (crash_stack    = . );

  . -= 16;
	   PROVIDE (_und_stack     = . );

  . -= 16;
	   PROVIDE (_abt_stack     = . );

  . -= 16;
	   PROVIDE (_fiq_stack     = . );
           
  . -= 256;
	   PROVIDE (_irq_stack     = . );

  . -= 16;
	   PROVIDE (_svc_stack     = . );

  . -= 256;
           PROVIDE (_user_stack    = . );

           PROVIDE (_stack_low     = . );

  PROVIDE (_deadbeef_end = _stack_low);
  PROVIDE (_deadbeef_start = 0x40000000);
}
