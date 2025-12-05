/* fiq.S
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

        .include "arm.S"
	.text
	.arm
	.section .init, "ax"

	.code 32
	.align 2

	.global fiq_handler
	.func	fiq_handler

        /* FIQ registers are R8 .. R14 */

        .equ    AD0CR,  0xe0034000
        .equ    AD0GDR, 0xe0034004
        .equ    ADC_CHANNELS, 1
        .equ    ADC_BUFFER_LENGTH, 512
        .equ    VICvectAddr, 0xfffff030
        .equ    ADC_SOFTWARE_INTERRUPT, 1

        .equ    VICSoftInt, 0xfffff018

        .equ    AB_WRITING, 0
        .equ    AB_OVERRUN, 4
        .equ    AB_ACK, 8
        .equ    AB_EVEN, 12
        .equ    AB_ODD, 16

        /* r8  : &adc_buffer.ab_even or ab_odd */
        /* r9  : VICvectAddr */
        /* r10 : write index */
        /* r11 : AD0GDR */
        /* r12 : free */
        /* r13 : stack */
        /* r14 : LR */

fiq_handler:
        ldr     r12, [r11] /* Load AD0GDR */
        strh    r12, [r8, r10]
        add     r10, #2
        mov     r12, #0
        str     r12, [r9] /* Clear VICvectAddr */
        cmp     r10, #ADC_BUFFER_LENGTH * 2
        beq     _finished
        subs    pc, r14, #4

        /* Swap buffers */
_finished:
        stmdb   sp!, {r0}

        /* Signal FIQ completion */
        ldr     r12, =adc_buffer

        ldr     r10, [r12, #AB_ACK] /* Did the system acknowledge the previous buffer ? */
        tst     r10, r10
        beq     _overrun
        
        ldr     r10, [r12, #AB_WRITING] /* Swap channels */
        eors    r10, r10, #1
        str     r10, [r12, #AB_WRITING]
        add     r10, r12, #AB_ODD
        addeq   r10, r12, #AB_EVEN
        ldr     r8, [r10]

        /* Trigger VIC interrupt #1 */
_dont_swap:
        ldr     r10, =VICSoftInt
        mov     r0, #1 << ADC_SOFTWARE_INTERRUPT
        str     r0, [r10]

        mov     r10, #0                 /* Restore index */

        ldmia   sp!, {r0}
        subs    pc, r14, #4

_overrun:
        mov     r10, #1
        str     r10, [r12, #AB_OVERRUN] /* Signal an overrun */
        b       _dont_swap

        .endfunc

	.global fiq_init
	.func	fiq_init

fiq_init:
        ldr     r1, =adc_buffer

        ldr     r8, =adc_buffer_odd
        str     r8, [r1, #AB_ODD]
        ldr     r8, =adc_buffer_even
        str     r8, [r1, #AB_EVEN]

        ldr     r9, =VICvectAddr

        mov     r10, #0
        str     r10, [r1, #AB_WRITING]
        str     r10, [r1, #AB_OVERRUN]
        str     r10, [r1, #AB_ACK]

        ldr     r11, =AD0GDR

        mov     pc, lr

        .endfunc

#if 0
        .global test_registers

test_registers:
        ldr     r0, =0x01010101
        add     r1, r0, r0
        add     r2, r1, r0
        add     r3, r2, r0
        add     r4, r3, r0
        add     r5, r4, r0
        add     r6, r5, r0
        add     r7, r6, r0
        add     r8, r7, r0
        add     r9, r8, r0
        add     r10, r9, r0
        add     r11, r10, r0
        add     r12, r11, r0
        /* add     r13, r12, r0 */
        /* add     r14, r13, r0 */
        b       abortion
#endif
