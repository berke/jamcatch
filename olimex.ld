/* ENTRY(_boot) */
STACK_SIZE = 0x400;

MEMORY
{
  rom (rx) : ORIGIN = 0x00000000, LENGTH = 500k
  ram (rw) : ORIGIN = 0x40000000, LENGTH = 32k
}

SECTIONS
{
  /* first section is .text which is used for code */
  .init : {
    *crt0.o ( .init )            /* Startup code */
  } > rom

  .text :
  {
/*    *crt0.o (.text)            /* Startup code */
    *(.text)                   /* remaining code */
    *(.rodata)                 /* read-only data (constants) */
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t) */
  } > rom

  . = ALIGN(4);
  
	/* .ctors .dtors are used for c++ constructors/destructors */
	/* added by Martin Thomas 4/2005 based on Anglia Design example */
	.ctors :
	{
		PROVIDE(__ctors_start__ = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		PROVIDE(__ctors_end__ = .);
	} >rom
	
	.dtors :
	{
		PROVIDE(__dtors_start__ = .); 
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		PROVIDE(__dtors_end__ = .);
	} >rom
  
    . = ALIGN(4);
	/* mthomas - end */
  
  
  _etext = . ;
  PROVIDE (etext = .);

  /* .data section which is used for initialized data */
  .data : AT (_etext)
  {
    _data = .;
    *(.data)
	SORT(CONSTRUCTORS) /* mt 4/2005 */
  } > ram
 
  . = ALIGN(4);
  _edata = . ;
  PROVIDE (edata = .);

  /* .bss section which is used for uninitialized data */
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

  .stack ALIGN(256) :
  {
    . += STACK_SIZE;
    PROVIDE (_stack = .);
  } > ram

  _end = . ;
  PROVIDE (end = .);
}
