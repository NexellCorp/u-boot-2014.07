
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
	mov	 pc, lr						@ back to caller

#endif /* CONFIG_SKIP_LOWLEVEL_INIT */