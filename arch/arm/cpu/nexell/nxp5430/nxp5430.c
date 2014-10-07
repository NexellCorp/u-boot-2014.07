/*
 * (C) Copyright 2009 Nexell Co.,
 * jung hyun kim<jhkim@nexell.co.kr>
 *
 * Configuation settings for the Nexell board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <platform.h>
#include <mach-api.h>
#include <pm.h>

#define DBGOUT(msg...)		do { printf("sys: " msg); } while (0)
#define printk(msg...)		do { printf(msg); } while (0)

static void cpu_base_init(void)
{
	int i = 0;

	NX_RSTCON_Initialize();
	NX_RSTCON_SetBaseAddress((U32)IO_ADDRESS(NX_RSTCON_GetPhysicalAddress()));

	NX_TIEOFF_Initialize();
	NX_TIEOFF_SetBaseAddress((U32)IO_ADDRESS(NX_TIEOFF_GetPhysicalAddress()));

	NX_CLKGEN_Initialize();
	for (i = 0; NX_CLKGEN_GetNumberOfModule() > i; i++)
		NX_CLKGEN_SetBaseAddress(i, (U32)IO_ADDRESS(NX_CLKGEN_GetPhysicalAddress(i)));

	NX_GPIO_Initialize();
	for (i = 0; NX_GPIO_GetNumberOfModule() > i; i++) {
		NX_GPIO_SetBaseAddress(i, (U32)IO_ADDRESS(NX_GPIO_GetPhysicalAddress(i)));
		NX_GPIO_OpenModule(i);
	}

	NX_ALIVE_Initialize();
	NX_ALIVE_SetBaseAddress((U32)IO_ADDRESS(NX_ALIVE_GetPhysicalAddress()));
	NX_ALIVE_OpenModule();

	NX_CLKPWR_Initialize();
	NX_CLKPWR_SetBaseAddress((U32)IO_ADDRESS(NX_CLKPWR_GetPhysicalAddress()));
	NX_CLKPWR_OpenModule();

    /*
     * NOTE> ALIVE Power Gate must enable for RTC register access.
     * 		 must be clear wfi jump address
 	 */
	NX_ALIVE_SetWriteEnable(CTRUE);
	__raw_writel(0xFFFFFFFF, SCR_ARM_SECOND_BOOT);
}

static void cpu_bus_init(void)
{
	/* MCUS for Static Memory. */
	NX_MCUS_Initialize();
	NX_MCUS_SetBaseAddress((U32)IO_ADDRESS(NX_MCUS_GetPhysicalAddress()));
	NX_MCUS_OpenModule();

	/*
	 * MCU-Static config: Static Bus #0 ~ #1
	 */
	#define STATIC_BUS_CONFIGUTATION( _n_ )								\
	NX_MCUS_SetStaticBUSConfig											\
	( 																	\
		NX_MCUS_SBUSID_STATIC ## _n_, 									\
		CFG_SYS_STATIC ## _n_ ## _BW, 									\
		CFG_SYS_STATIC ## _n_ ## _TACS, 								\
		CFG_SYS_STATIC ## _n_ ## _TCAH, 								\
		CFG_SYS_STATIC ## _n_ ## _TCOS, 								\
		CFG_SYS_STATIC ## _n_ ## _TCOH, 								\
		CFG_SYS_STATIC ## _n_ ## _TACC, 								\
		CFG_SYS_STATIC ## _n_ ## _TSACC,								\
		(NX_MCUS_WAITMODE ) CFG_SYS_STATIC ## _n_ ## _WAITMODE, 		\
		(NX_MCUS_BURSTMODE) CFG_SYS_STATIC ## _n_ ## _RBURST, 			\
		(NX_MCUS_BURSTMODE) CFG_SYS_STATIC ## _n_ ## _WBURST			\
	);

	STATIC_BUS_CONFIGUTATION( 0);
	STATIC_BUS_CONFIGUTATION( 1);
}

/*------------------------------------------------------------------------------
 *	CPU initialize
 */
void nxp_cpu_periph_init(void)
{
}

void nxp_cpu_arch_init(void)
{
	cpu_base_init();
	cpu_bus_init();
}

unsigned int nxp_cpu_version(void)
{
	return 0;
}

void nxp_print_cpu_info(void)
{
	nxp_cpu_clock_print();
}

