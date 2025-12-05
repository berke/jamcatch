/* arm.S
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

	.set  MODE_USR, 0x10	/* User Mode */
	.set  MODE_FIQ, 0x11	/* FIQ Mode */
	.set  MODE_IRQ, 0x12	/* IRQ Mode */
	.set  MODE_SVC, 0x13	/* Supervisor Mode */
	.set  MODE_ABT, 0x17	/* Abort Mode */
	.set  MODE_UND, 0x1B	/* Undefined Mode */
	.set  MODE_SYS, 0x1F	/* System Mode */

	.equ  I_BIT, 0x80	/* when I bit is set, IRQ is disabled */
	.equ  F_BIT, 0x40	/* when F bit is set, FIQ is disabled */
