
#ifndef CONFIG_SKIP_LOWLEVEL_INIT
/*
 *************************************************************************
 *
 * CPU_init_critical
 *
 *************************************************************************
 */
	.globl lowlevel_init
lowlevel_init:
#if 0
	.word 0xEC510F1F
	.word 0xE3800040
	.word 0xEC410F1F
#endif
	@ NXP5430
	ldr	r0, =0xfaaa0
	mcr	p15, 0, r0, c10, c2, 0		@ set PRRR
	ldr	r0, =0xffecffec
	mcr	p15, 0, r0, c10, c2, 1		@ set NMRR

	mov	 pc, lr						@ back to caller

#endif /* CONFIG_SKIP_LOWLEVEL_INIT */