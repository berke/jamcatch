/* Delay */

	.global delay_us
	.func	delay_us

delay_us:
        mov     r0, r0, lsl #3
        tst     r0, r0
        beq     _bye

_loop_us:
        nop
        nop
        nop
        nop
        nop
        subs    r0, r0, #1
        bne     _loop_us

_bye:
        bx      r14

        .endfunc
