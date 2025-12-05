/* schedule.S
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

        .equ    EXEC_YIELD, 0

	.global exec_restore_from_user
	.func	exec_restore_from_user

exec_restore_from_user:
        add     r0, #8
        ldr     r13, [r0, #(13 * 4)]
        ldr     r14, [r0, #(14 * 4)]
        msr     cpsr, #(MODE_SVC|I_BIT)    /* Move to service mode, IRQs disabled */
        mov     r13, r0                    /* R13 is R13_SVC */

        ldmia   r0, {r0-r12}               /* R0-R12 are now restored */

        ldr     r14, [r13, #64]            /* Get stored CPSR */
        msr     spsr, r14                  /* Put it in SPSR */

        add     r13, #(13*4)
        ldmia	r13, {r13-r15}^

        .endfunc

        .global exec_restore_from_interrupt
	.func	exec_restore_from_interrupt

/* We are in SYSTEM mode.  We don't have any banked registers */
exec_restore_from_interrupt:
        b       exec_restore_from_user

//        add     r0, #8
//        msr     cpsr, #(MODE_SVC|I_BIT)    /* Move to service mode, IRQs disabled */
//        ldr     r1, [r0, #64]
//        msr     spsr, r1 /* was cpsr_f */
//        ldmia	r0, {r0-r15}^

//        add     r0, #8                     /* Skip linked list stuff */
//        ldr     r1, [r0, #64]              /* Get status register */
//        msr     cpsr_fc, r1                /* Put it back */
//        ldmia	r0, {r0-r15}               /* Will also load CPSR */

        .endfunc

	.global exec_yield_from_interrupt
	.func	exec_yield_from_interrupt

        /* we are called using r13_irq */
exec_yield_from_interrupt:
        sub     lr, lr, #4                 /* Fix LR (ARM pipeline stuff) - this is the continuation PC
                                            * for the suspended task. */

        str     r0, [sp, #-4]              /* sp[4] = r0_i */

        /* We can now use r0 */
        /* Find the current task */
        ldr     r0, =kernel                /* r0 = &kernel */
        ldr     r0, [r0]                   /* r0 = kernel->k_current */
        add     r0, #12                    /* r0 = &kernel->k_ready.t_regs + 1; */

        /* The usual registers r1..r12 */
        stmia   r0, {r1-r12}

        /* The PC */
        str     lr, [r0, #(15*4 + 2*4 - 12)]              /* Store LR as return PC */

        /* Get back and store r0 */
        ldr     r1, [sp, #-4]
        str     r1, [r0, #-4]

        /* Nothing to say till here... */
        /* The status register */
        mrs     r1, spsr
        str     r1, [r0, #(16*4 + 2*4 - 12)]

        /* Store the user SP(R13) & LR(R14) */
        add     r0, #(13*4 + 2*4 - 12)
        stmia   r0, {sp, lr}^

        /* Context saved. */
        msr     cpsr, #(MODE_SYS|I_BIT)    /* Move to system mode, IRQs disabled */
        ldr     sp, =exec_scheduler_sp     /* Restore an appropriate stack pointer */
        ldr     sp, [sp]
        ldr     r0, =0xfffff030
	ldr	pc,[r0]                    /* IRQ - read the VIC */

        .endfunc

	.global exec_yield_critical
	.func	exec_yield_critical

        /* SHOULD BE CALLED FROM CRITICAL SECTIONS */

exec_yield_critical:
        str     r0, [sp, #-4]              /* sp[4] = r0_i */

        /* We can now use r0 */
        /* Find the current task */
        ldr     r0, =kernel                /* r0 = &kernel */
        ldr     r0, [r0]                   /* r0 = kernel->k_current */
        add     r0, #12                    /* r0 = &kernel->k_ready.t_regs + 1; */

        /* The usual registers r1..r12 */
        stmia   r0, {r1-r12}

        /* The PC */
        str     lr, [r0, #(15*4 + 2*4 - 12)]              /* Store LR as return PC */

        /* Get back and store r0 */
        ldr     r1, [sp, #-4]
        str     r1, [r0, #-4]

        /* Nothing to say till here... */
        /* The status register WAS in R0, now in R1 */
        str     r1, [r0, #(16*4 + 2*4 - 12)]

        /* Store SP(R13) & LR(R14) */
        add     r0, #(13*4 + 2*4 - 12)
        stmia   r0, {sp, lr}

        /* Context saved. */
        ldr     sp, =exec_scheduler_sp     /* Restore an appropriate stack pointer */
        ldr     sp, [sp]
        mov     r0, #EXEC_YIELD            /* Indicate that we are NOT calling from an interrupt */
        b       exec_schedule

        .endfunc
