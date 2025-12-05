/* crt0.S
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

/* vim:set ts=10 noexpandtab: */

	.global _etext		/* -> .data initial values in ROM */
	.global _data		/* -> .data area in RAM */
	.global _edata		/* end of .data area */
	.global __bss_start	/* -> .bss area in RAM */
	.global __bss_end__	/* end of .bss area */
	.global _stack		/* top of stack */

/* Stack Sizes */
	.set  UND_STACK_SIZE, 0x00000010
	.set  ABT_STACK_SIZE, 0x00000010
	.set  FIQ_STACK_SIZE, 0x00000010
	.set  IRQ_STACK_SIZE, 0x00000100 /* 4 should be enough, but 32 to be safe... */
	.set  SVC_STACK_SIZE, 0x00000080

	.include	"arm.S"

#include "config.h"

	.text
	.arm
	.section .init, "ax"

	.code 32
	.align 2

	.global _boot
	.func	_boot
	.org	0
_boot:

vectors:
	b	_start		/* reset - _start */
	ldr	pc,_undf		/* undefined - _undf */
	ldr	pc,_swi		/* SWI - _swi */
	ldr	pc,_pabt		/* program abort - _pabt */
	ldr	pc,_dabt		/* data abort - _dabt */
	nop			/* reserved */

	ldr	pc,_vic

	/*ldr	pc,[pc,#(0xfffff030 - 8 * 4)]	*//* IRQ - read the VIC */

	ldr	pc,_fiq		/* FIQ - _fiq */

_vic:	.word exec_yield_from_interrupt
_undf:	.word _blink_undf
_pabt:	.word _blink_pabt
_dabt:	.word _blink_dabt
_swi:	.word _blink_swi
_fiq:	.word fiq_handler

_blink_undf:
	stmdb	sp!, {r0-r3}
	mov	r2, #11
	b	_blink

_blink_pabt:
	stmdb	sp!, {r0-r3}
	mov	r2, #22
	b	_blink

_blink_dabt:
	stmdb	sp!, {r0-r3}
	mov	r2, #33
	b	_blink

_blink_swi:
	stmdb	sp!, {r0-r3}
	mov	r2, #44
	b	_blink

_blink_fiq:
	stmdb	sp!, {r0-r3}
	mov	r2, #55
	b	_blink

	.global	abortion

abortion:
	msr	CPSR_c, #MODE_SVC|I_BIT|F_BIT

	/* sp[-16] = r0
	 * sp[-12] = r1
	 * sp[- 8] = r2
	 * sp[- 4] = r3 */

	stmdb	sp!, {r0-r3}
	mov	r2, #66

_blink:
#if !BETA
	b	_holy_fucking_shit
#endif

	/* Write crash cookie */
	ldr	r0, =crash_cookie
	ldr	r3, [r0]
	ldr	r1, =0xdefaced
	cmp	r1, r3
	beq	_holy_fucking_shit

	/* Save crash registers */
	ldr	r0, =crash_regs

	/* r0 : 0 */
	ldr	r1, [sp]
	str	r1, [r0]
	ldr	r1, [sp, #4]
	str	r1, [r0, #4]
	ldr	r1, [sp, #8]
	str	r1, [r0, #8]
	ldr	r1, [sp, #12]
	str	r1, [r0, #12]

	add	r0, #16

	/* r0 : 16 */
	stmia	r0, {r4-r15}^

	/* r0 : 16 */
	mrs	r1, spsr
	str	r1, [r0, #64]
	add	r0, #(68 - 16)

	/* Now do banked registers */

	/* SVC registers, r0 = 68 */
	msr	CPSR_c, #MODE_SVC|I_BIT|F_BIT
	stmia	r0!, {r13, r14}
	mrs	r1, spsr
	stmia	r0!, {r1}

	/* ABT registers, r0 = 80 */
	msr	CPSR_c, #MODE_ABT|I_BIT|F_BIT
	stmia	r0!, {r13, r14}
	mrs	r1, spsr
	stmia	r0!, {r1}

	/* UND registers, r0 = 92 */
	msr	CPSR_c, #MODE_UND|I_BIT|F_BIT
	stmia	r0!, {r13, r14}
	mrs	r1, spsr
	stmia	r0!, {r1}

	/* IRQ registers, r0 = 104 */
	msr	CPSR_c, #MODE_IRQ|I_BIT|F_BIT
	stmia	r0!, {r13, r14}
	mrs	r1, spsr
	stmia	r0!, {r1}

	/* FIQ registers, r0 = 112 */
	msr	CPSR_c, #MODE_FIQ|I_BIT|F_BIT
	stmia	r0!, {r8 - r14}
	mrs	r1, spsr
	stmia	r0!, {r1}

	/* Jump to crash handler in system mode */
	ldr	r0, =crash_regs
	mov	r1, r2
	ldr	sp, =crash_stack
	msr	CPSR_c, #MODE_SYS|I_BIT|F_BIT
	b	crash_handler

_holy_fucking_shit:
	str	r1, [r0]
	str	r2, [r0, #4]
	str	lr, [r0, #8]

_loop:
	bl	_led_set
	mov	r0, #0x500000

_b0:	subs	r0, r0, #1
	bne	_b0
	bl	_led_clr
	mov	r0, #0x1000000

_b1:	subs	r0, r0, #1
	bne	_b1
	b	_loop

_led_set:
	ldr	r0, =0xe0028008
	ldr	r1, =0x80000000
	str	r1, [r0]
	ldr	r0, =0xe002800c
	str	r1, [r0]
	bx	r14

_led_clr:
	ldr	r0, =0xe0028008
	ldr	r1, =0x80000000
	str	r1, [r0]
	ldr	r0, =0xe0028004
	str	r1, [r0]
	bx	r14
	
	.size	_boot, . - _boot

	.endfunc

	.org	0x1fc
_protect:
#if CODE_READ_PROTECTION
#warning "Code read protection enabled"
	.word	0x87654321
#else
#warning "Code read protection DISABLED"
	.word	0xcafebabe
#endif

	.org	0x200
	.global	_start
	.func	_start

_start:
	tst	r0, r0
	mov	r0, #0
	mov	r1, #0
	mov	r2, #0
	mov	r3, #0
	mov	r4, #0
	mov	r5, #0
	mov	r6, #0
	mov	r7, #0
	mov	r8, #0
	mov	r9, #0
	mov	r10, #0
	mov	r11, #0
	mov	r12, #0
	mov	r13, #0
	mov	r14, #0

#if 1
	ldr	r0,=_deadbeef_start
	ldr	r1,=_deadbeef_end
	ldr	r2,=0xdeadbeef

_deadbeef_loop:
	stmia	r0!,{r2}
	cmp	r0,r1
	bne	_deadbeef_loop
#endif

	ldr	r0, =command_privilege_level
	mov	r1, #1
	str	r1, [r0]

	/* Undefined Instruction Mode */
	msr	CPSR_c, #MODE_UND|I_BIT|F_BIT
	ldr	sp, =_und_stack

	/* Abort Mode */
	msr	CPSR_c, #MODE_ABT|I_BIT|F_BIT
	ldr	sp, =_abt_stack

	/* FIQ Mode */
	msr	CPSR_c, #MODE_FIQ|I_BIT|F_BIT
	ldr	sp, =_fiq_stack
	bl	fiq_init

	/* IRQ Mode */
	msr	CPSR_c, #MODE_IRQ|I_BIT|F_BIT
	ldr	sp, =_irq_stack

	/* Supervisor Mode */
	msr	CPSR_c, #MODE_SVC|I_BIT|F_BIT
	ldr	sp, =_svc_stack

	/* System Mode */
	msr	CPSR_c, #MODE_SYS|I_BIT|F_BIT
	ldr	sp, =_user_stack

	ldr	r10, =run
	bx	r10
	.endfunc

	.end
