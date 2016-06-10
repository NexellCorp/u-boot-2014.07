/*
 *  Copyright (C) 2016 NEXELL SOC Lab.
 *  Jongsin Park <pjsin865@nexell.co.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <config.h>
#include <common.h>
#include <mmc.h>
#include <pwm.h>
#include <asm/io.h>
#include <asm/gpio.h>

#include <platform.h>
#include <mach-api.h>
#include <rtc_nxp.h>
#include <pm.h>

#include <draw_lcd.h>

#include <power/pmic.h>
#include <power/battery.h>
#include <nxe1500-private.h>
#include "nxe1500_power.h"

#include <i2c.h>
#include <errno.h>

/*
 * Debug
 */
#if defined(CONFIG_PMIC_REG_DUMP)
#define DBGOUT(msg...)		do { printf("pmic: " msg); } while (0)
#else
#define DBGOUT(msg...)		do {} while (0)
#endif

static u_char nxe1500_cache_reg[256];

static int nxe1500_i2c_read(u8 reg, u8 *value, struct nxe1500_power *power)
{
	uchar chip = power->i2c_addr;
	u32   addr = (u32)reg & 0xFF;
	int   alen = 1;

	return i2c_read(chip, addr, alen, value, 1);
}

static int nxe1500_i2c_write(u8 reg, u8 value, struct nxe1500_power *power)
{
	uchar chip = power->i2c_addr;
	u32   addr = (u32)reg & 0xFF;
	int   alen = 1;

	return i2c_write(chip, addr, alen, &value, 1);
}

u8 nxe1500_get_ldo_step(u8 ldo_num, int want_vol)
{
	u32	vol_step = 0;
	u32	temp = 0;

	if (ldo_num == NXE1500_LDO3) {

		if (want_vol < NXE1500_DEF_LDO3_VOL_MIN)
		{
			want_vol = NXE1500_DEF_LDO3_VOL_MIN;
		}
		else if (want_vol > NXE1500_DEF_LDO3_VOL_MAX)
		{
			want_vol = NXE1500_DEF_LDO3_VOL_MAX;
		}
		temp = (want_vol - NXE1500_DEF_LDO3_VOL_MIN);
	}
	else if (ldo_num == NXE1500_LDORTC1)
	{
		if (want_vol < NXE1500_DEF_LDORTC1_VOL_MIN)
		{
			want_vol = NXE1500_DEF_LDORTC1_VOL_MIN;
		}
		else if (want_vol > NXE1500_DEF_LDORTC1_VOL_MAX)
		{
			want_vol = NXE1500_DEF_LDORTC1_VOL_MAX;
		}
		temp = (want_vol - NXE1500_DEF_LDORTC1_VOL_MIN);
	}
	else
	{
		if (want_vol < NXE1500_DEF_LDOx_VOL_MIN)
		{
			want_vol = NXE1500_DEF_LDOx_VOL_MIN;
		}
		else if (want_vol > NXE1500_DEF_LDOx_VOL_MAX)
		{
			want_vol = NXE1500_DEF_LDOx_VOL_MAX;
		}
		temp = (want_vol - NXE1500_DEF_LDOx_VOL_MIN);
	}

	vol_step	= (temp / NXE1500_DEF_LDOx_VOL_STEP);

	if (ldo_num == NXE1500_LDO3)
	{
		temp	= (vol_step * NXE1500_DEF_LDOx_VOL_STEP) + NXE1500_DEF_LDO3_VOL_MIN;
	}
	else if (ldo_num == NXE1500_LDORTC1)
	{
		temp	= (vol_step * NXE1500_DEF_LDOx_VOL_STEP) + NXE1500_DEF_LDORTC1_VOL_MIN;
	}
	else
	{
		temp	= (vol_step * NXE1500_DEF_LDOx_VOL_STEP) + NXE1500_DEF_LDOx_VOL_MIN;
	}

	return	(u8)(vol_step & 0x7F);
}

u8 nxe1500_get_dcdc_step(u8 dcdc_num, int want_vol)
{
	u32	vol_step = 0;
	u32	temp = 0;

	if (want_vol < NXE1500_DEF_DDCx_VOL_MIN)
	{
		want_vol = NXE1500_DEF_DDCx_VOL_MIN;
	}
	else if (want_vol > NXE1500_DEF_DDCx_VOL_MAX)
	{
		want_vol = NXE1500_DEF_DDCx_VOL_MAX;
	}

	temp		= (want_vol - NXE1500_DEF_DDCx_VOL_MIN);
	vol_step	= (temp / NXE1500_DEF_DDCx_VOL_STEP);
	temp		= (vol_step * NXE1500_DEF_DDCx_VOL_STEP) + NXE1500_DEF_DDCx_VOL_MIN;

	return	(u8)(vol_step & 0xFF);
}

#if defined(CONFIG_PMIC_REG_DUMP)
void nxe1500_register_dump(struct nxe1500_power *power)
{
	s32 ret=0;
	u16 i=0;
	u8 value[NXE1500_NUM_OF_REGS]={0};

	printf("##########################################################\n");
	printf("##\e[31m %s()\e[0m \n", __func__);
	printf("----------------------------------------------------------\n");
	printf("       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F\n");

	for(i=0; i<=NXE1500_NUM_OF_REGS; i++)
	{
		if(i%16 == 0)
			printf("  %02X:", i);

		if(i%4 == 0)
			printf(" ");

		ret = nxe1500_i2c_read(i, &value[i], power);
		if(!ret)
			printf("%02x ", value[i]);
		else
			printf("\e[31mxx\e[0m ");

		if((i+1)%16 == 0)
			printf("\n");
	}
	printf("##########################################################\n");

}
#endif

static int nxe1500_param_setup(struct nxe1500_power *power)
{
	u_char *cache = nxe1500_cache_reg;
	u_char	temp = 0x0;

	DBGOUT("%s\n", __func__);

	//temp = ((1 << NXE1500_POS_GPLED_FUNC_GP1_LEDMODE)
	//			| (0x3 << NXE1500_POS_GPLED_FUNC_GP1_LEDFUNC)
	//			);
	//nxe1500_i2c_write(NXE1500_REG_GPLED_FUNC, temp, power);

	nxe1500_i2c_read(NXE1500_REG_PWRONTIMSET, &cache[NXE1500_REG_PWRONTIMSET], power);
    cache[NXE1500_REG_PWRONTIMSET] &= ~(0x7 << NXE1500_POS_PWRONTIMSET_OFF_PRESS_PWRON);
    cache[NXE1500_REG_PWRONTIMSET] |= (NXE1500_DEF_OFF_PRESS_TIME << NXE1500_POS_PWRONTIMSET_OFF_PRESS_PWRON);
	nxe1500_i2c_write(NXE1500_REG_PWRONTIMSET, cache[NXE1500_REG_PWRONTIMSET], power);

	nxe1500_i2c_read(NXE1500_REG_POFFHIS	, &cache[NXE1500_REG_POFFHIS]	, power);
	power->warm_reset = cache[NXE1500_REG_POFFHIS] ? 1 : 0;

	/* Set DCDC voltage register */
	cache[NXE1500_REG_DC1VOL]		= nxe1500_get_dcdc_step(NXE1500_BUCK1, NXE1500_DEF_DDC1_VOL);
	cache[NXE1500_REG_DC2VOL]		= nxe1500_get_dcdc_step(NXE1500_BUCK2, NXE1500_DEF_DDC2_VOL);
	cache[NXE1500_REG_DC3VOL]		= nxe1500_get_dcdc_step(NXE1500_BUCK3, NXE1500_DEF_DDC3_VOL);
	cache[NXE1500_REG_DC4VOL]		= nxe1500_get_dcdc_step(NXE1500_BUCK4, NXE1500_DEF_DDC4_VOL);
	cache[NXE1500_REG_DC1VOL_SLP]	= nxe1500_get_dcdc_step(NXE1500_BUCK1, NXE1500_DEF_DDC1_SLP_VOL);
	cache[NXE1500_REG_DC2VOL_SLP]	= nxe1500_get_dcdc_step(NXE1500_BUCK2, NXE1500_DEF_DDC2_SLP_VOL);
	cache[NXE1500_REG_DC3VOL_SLP]	= nxe1500_get_dcdc_step(NXE1500_BUCK3, NXE1500_DEF_DDC3_SLP_VOL);
	cache[NXE1500_REG_DC4VOL_SLP]	= nxe1500_get_dcdc_step(NXE1500_BUCK4, NXE1500_DEF_DDC4_SLP_VOL);

	/* Set LDO voltage register */
	cache[NXE1500_REG_LDO1VOL]		= nxe1500_get_ldo_step(NXE1500_LDO1, NXE1500_DEF_LDO1_VOL);
	cache[NXE1500_REG_LDO2VOL]		= nxe1500_get_ldo_step(NXE1500_LDO2, NXE1500_DEF_LDO2_VOL);
	cache[NXE1500_REG_LDO3VOL]		= nxe1500_get_ldo_step(NXE1500_LDO3, NXE1500_DEF_LDO3_VOL);
	cache[NXE1500_REG_LDO4VOL]		= nxe1500_get_ldo_step(NXE1500_LDO4, NXE1500_DEF_LDO4_VOL);
	cache[NXE1500_REG_LDO5VOL]		= nxe1500_get_ldo_step(NXE1500_LDO5, NXE1500_DEF_LDO5_VOL);
	cache[NXE1500_REG_LDORTC1VOL]	= nxe1500_get_ldo_step(NXE1500_LDORTC1, NXE1500_DEF_LDORTC1_VOL);
	cache[NXE1500_REG_LDORTC2VOL]	= nxe1500_get_ldo_step(NXE1500_LDORTC2, NXE1500_DEF_LDORTC2_VOL);
	cache[NXE1500_REG_LDO1VOL_SLP]	= nxe1500_get_ldo_step(NXE1500_LDO1, NXE1500_DEF_LDO1_SLP_VOL);
	cache[NXE1500_REG_LDO2VOL_SLP]	= nxe1500_get_ldo_step(NXE1500_LDO2, NXE1500_DEF_LDO2_SLP_VOL);
	cache[NXE1500_REG_LDO3VOL_SLP]	= nxe1500_get_ldo_step(NXE1500_LDO3, NXE1500_DEF_LDO3_SLP_VOL);
	cache[NXE1500_REG_LDO4VOL_SLP]	= nxe1500_get_ldo_step(NXE1500_LDO4, NXE1500_DEF_LDO4_SLP_VOL);
	cache[NXE1500_REG_LDO5VOL_SLP]	= nxe1500_get_ldo_step(NXE1500_LDO5, NXE1500_DEF_LDO5_SLP_VOL);

	/* Set DCDC control register */
	cache[NXE1500_REG_DC1CTL2]	= ( (NXE1500_DEF_DDC1_OSC_FREQ	 << NXE1500_POS_DCxCTL2_DCxOSC)	|
									(NXE1500_DEF_DDC1_RAMP_SLOP  << NXE1500_POS_DCxCTL2_DCxSR)	|
									(NXE1500_DEF_DDC1_CUR_LIMIT  << NXE1500_POS_DCxCTL2_DCxLIM)	|
									(NXE1500_DEF_DDC1_LIMSHUT_EN << NXE1500_POS_DCxCTL2_DCxLIMSDEN) );
	cache[NXE1500_REG_DC2CTL2]	= ( (NXE1500_DEF_DDC2_OSC_FREQ	 << NXE1500_POS_DCxCTL2_DCxOSC)	|
									(NXE1500_DEF_DDC2_RAMP_SLOP  << NXE1500_POS_DCxCTL2_DCxSR)	|
									(NXE1500_DEF_DDC2_CUR_LIMIT  << NXE1500_POS_DCxCTL2_DCxLIM)	|
									(NXE1500_DEF_DDC2_LIMSHUT_EN << NXE1500_POS_DCxCTL2_DCxLIMSDEN) );
	cache[NXE1500_REG_DC3CTL2]	= ( (NXE1500_DEF_DDC3_OSC_FREQ	 << NXE1500_POS_DCxCTL2_DCxOSC)	|
									(NXE1500_DEF_DDC3_RAMP_SLOP  << NXE1500_POS_DCxCTL2_DCxSR)	|
									(NXE1500_DEF_DDC3_CUR_LIMIT  << NXE1500_POS_DCxCTL2_DCxLIM)	|
									(NXE1500_DEF_DDC3_LIMSHUT_EN << NXE1500_POS_DCxCTL2_DCxLIMSDEN) );
	cache[NXE1500_REG_DC4CTL2]	= ( (NXE1500_DEF_DDC4_OSC_FREQ	 << NXE1500_POS_DCxCTL2_DCxOSC)	|
									(NXE1500_DEF_DDC4_RAMP_SLOP  << NXE1500_POS_DCxCTL2_DCxSR)	|
									(NXE1500_DEF_DDC4_CUR_LIMIT  << NXE1500_POS_DCxCTL2_DCxLIM)	|
									(NXE1500_DEF_DDC4_LIMSHUT_EN << NXE1500_POS_DCxCTL2_DCxLIMSDEN) );

	/* DCDC - Enable */
	cache[NXE1500_REG_DC1CTL]	= ( (NXE1500_DEF_DDC1_SLP_MODE	<< NXE1500_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE1500_DEF_DDC1_OSC_FREQ	<< NXE1500_POS_DCxCTL_DCxMODE)	|
									(NXE1500_DEF_DDC1_DSC_CTRL	<< NXE1500_POS_DCxCTL_DCxDIS)	|
									(NXE1500_DEF_DDC1_ON		<< NXE1500_POS_DCxCTL_DCxEN) );
	cache[NXE1500_REG_DC2CTL]	= ( (NXE1500_DEF_DDC2_SLP_MODE	<< NXE1500_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE1500_DEF_DDC2_OSC_FREQ	<< NXE1500_POS_DCxCTL_DCxMODE)	|
									(NXE1500_DEF_DDC2_DSC_CTRL	<< NXE1500_POS_DCxCTL_DCxDIS)	|
									(NXE1500_DEF_DDC2_ON		<< NXE1500_POS_DCxCTL_DCxEN) );
	cache[NXE1500_REG_DC3CTL]	= ( (NXE1500_DEF_DDC3_SLP_MODE	<< NXE1500_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE1500_DEF_DDC3_OSC_FREQ	<< NXE1500_POS_DCxCTL_DCxMODE)	|
									(NXE1500_DEF_DDC3_DSC_CTRL	<< NXE1500_POS_DCxCTL_DCxDIS)	|
									(NXE1500_DEF_DDC3_ON		<< NXE1500_POS_DCxCTL_DCxEN) );
	cache[NXE1500_REG_DC4CTL]	= ( (NXE1500_DEF_DDC4_SLP_MODE	<< NXE1500_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE1500_DEF_DDC4_OSC_FREQ	<< NXE1500_POS_DCxCTL_DCxMODE)	|
									(NXE1500_DEF_DDC4_DSC_CTRL	<< NXE1500_POS_DCxCTL_DCxDIS)	|
									(NXE1500_DEF_DDC4_ON		<< NXE1500_POS_DCxCTL_DCxEN) );

	/* LDO - Enable */
	cache[NXE1500_REG_LDOEN1]	= ( (NXE1500_DEF_LDO1_ON << NXE1500_POS_LDOEN1_LDO1EN) |
									(NXE1500_DEF_LDO2_ON << NXE1500_POS_LDOEN1_LDO2EN) |
									(NXE1500_DEF_LDO3_ON << NXE1500_POS_LDOEN1_LDO3EN) |
									(NXE1500_DEF_LDO4_ON << NXE1500_POS_LDOEN1_LDO4EN) |
                                    (NXE1500_DEF_LDO5_ON << NXE1500_POS_LDOEN1_LDO5EN));
	cache[NXE1500_REG_LDOEN2]	= ( (NXE1500_DEF_LDORTC1_ON	<< NXE1500_POS_LDOEN2_LDORTC1EN)|
                                    (NXE1500_DEF_LDORTC2_ON	<< NXE1500_POS_LDOEN2_LDORTC2EN));
	cache[NXE1500_REG_REPCNT] = ((NXE1500_DEF_REPCNT_OFF_RESETO	<< NXE1500_POS_REPCNT_OFF_RESETO)
								| (NXE1500_DEF_REPCNT_REPWRTIM	<< NXE1500_POS_REPCNT_REPWRTIM)
								| (NXE1500_DEF_REPCNT_REPWRON	<< NXE1500_POS_REPCNT_REPWRON));
	cache[NXE1500_REG_WATCHDOG] = ((NXE1500_DEF_WATHDOG_WDOGSLPEN	<< NXE1500_POS_WATHDOG_WDOGSLPEN)
								| (NXE1500_DEF_WATHDOG_WDOG_EN		<< NXE1500_POS_WATHDOG_WDOG_EN)
								| (NXE1500_DEF_WATHDOG_WDOGTIM		<< NXE1500_POS_WATHDOG_WDOGTIM));
	cache[NXE1500_REG_PWRIREN] = ((NXE1500_DEF_PWRIREN_WDOG			<< NXE1500_POS_PWRIREN_WDOG)
								| (NXE1500_DEF_PWRIREN_NOE_OFF		<< NXE1500_POS_PWRIREN_NOE_OFF)
								| (NXE1500_DEF_PWRIREN_PWRON_OFF	<< NXE1500_POS_PWRIREN_PWRON_OFF)
								| (NXE1500_DEF_PWRIREN_PREOT		<< NXE1500_POS_PWRIREN_PREOT)
								| (NXE1500_DEF_PWRIREN_PRVINDT		<< NXE1500_POS_PWRIREN_PRVINDT)
								| (NXE1500_DEF_PWRIREN_EXTIN		<< NXE1500_POS_PWRIREN_EXTIN)
								| (NXE1500_DEF_PWRIREN_PWRON		<< NXE1500_POS_PWRIREN_PWRON));

	return 0;
}

static int nxe1500_device_setup(struct nxe1500_power *power)
{
	u_char	*cache = nxe1500_cache_reg;
	int		bus = power->i2c_bus;
#if defined(CONFIG_PMIC_REG_DUMP)
	int i;
	s32 ret=0;
#endif

	DBGOUT("%s\n", __func__);

	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	i2c_set_bus_num(bus);

#if defined(CONFIG_PMIC_REG_DUMP)
	printf("##########################################################\n");
	printf("##\e[31m PMIC OTP Value \e[0m \n");
	printf("       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F\n");
	for(i=0; i<=NXE1500_NUM_OF_REGS; i++)
	{
		if(i%16 == 0)
			printf("  %02X:", i);
		if(i%4 == 0)
			printf(" ");

		ret = nxe1500_i2c_read(i, &cache[i]	, power);
		if(!ret)
			printf("%02x ", cache[i]);
		else
			printf("\e[31mxx\e[0m ");

		if((i+1)%16 == 0)
			printf("\n");
	}
	printf("##########################################################\n");
#endif

	nxe1500_param_setup(power);

	/* Set DCDC voltage register */
#ifdef CONFIG_ENABLE_INIT_VOLTAGE
	nxe1500_i2c_write(NXE1500_REG_DC1VOL	, cache[NXE1500_REG_DC1VOL]		, power);
	nxe1500_i2c_write(NXE1500_REG_DC2VOL	, cache[NXE1500_REG_DC2VOL]		, power);
	nxe1500_i2c_write(NXE1500_REG_DC3VOL	, cache[NXE1500_REG_DC3VOL]		, power);
	nxe1500_i2c_write(NXE1500_REG_DC4VOL	, cache[NXE1500_REG_DC4VOL]		, power);
#endif

	nxe1500_i2c_write(NXE1500_REG_DC1VOL_SLP, cache[NXE1500_REG_DC1VOL_SLP]	, power);
	nxe1500_i2c_write(NXE1500_REG_DC2VOL_SLP, cache[NXE1500_REG_DC2VOL_SLP]	, power);
	nxe1500_i2c_write(NXE1500_REG_DC3VOL_SLP, cache[NXE1500_REG_DC3VOL_SLP]	, power);
	nxe1500_i2c_write(NXE1500_REG_DC4VOL_SLP, cache[NXE1500_REG_DC4VOL_SLP]	, power);

	/* Set LDO voltage register */
	nxe1500_i2c_write(NXE1500_REG_LDO1VOL	, cache[NXE1500_REG_LDO1VOL]	, power);
	nxe1500_i2c_write(NXE1500_REG_LDO2VOL	, cache[NXE1500_REG_LDO2VOL]	, power);
	nxe1500_i2c_write(NXE1500_REG_LDO3VOL	, cache[NXE1500_REG_LDO3VOL]	, power);
	nxe1500_i2c_write(NXE1500_REG_LDO4VOL	, cache[NXE1500_REG_LDO4VOL]	, power);
	nxe1500_i2c_write(NXE1500_REG_LDO5VOL	, cache[NXE1500_REG_LDO5VOL]	, power);
	nxe1500_i2c_write(NXE1500_REG_LDORTC1VOL, cache[NXE1500_REG_LDORTC1VOL]	, power);
	nxe1500_i2c_write(NXE1500_REG_LDORTC2VOL, cache[NXE1500_REG_LDORTC2VOL]	, power);
	nxe1500_i2c_write(NXE1500_REG_LDO1VOL_SLP, cache[NXE1500_REG_LDO1VOL_SLP], power);
	nxe1500_i2c_write(NXE1500_REG_LDO2VOL_SLP, cache[NXE1500_REG_LDO2VOL_SLP], power);
	nxe1500_i2c_write(NXE1500_REG_LDO3VOL_SLP, cache[NXE1500_REG_LDO3VOL_SLP], power);
	nxe1500_i2c_write(NXE1500_REG_LDO4VOL_SLP, cache[NXE1500_REG_LDO4VOL_SLP], power);
	nxe1500_i2c_write(NXE1500_REG_LDO5VOL_SLP, cache[NXE1500_REG_LDO5VOL_SLP], power);

	/* Set DCDC control register */
	nxe1500_i2c_write(NXE1500_REG_DC1CTL2	, cache[NXE1500_REG_DC1CTL2]	, power);
	nxe1500_i2c_write(NXE1500_REG_DC2CTL2	, cache[NXE1500_REG_DC2CTL2]	, power);
	nxe1500_i2c_write(NXE1500_REG_DC3CTL2	, cache[NXE1500_REG_DC3CTL2]	, power);
	nxe1500_i2c_write(NXE1500_REG_DC4CTL2	, cache[NXE1500_REG_DC4CTL2]	, power);

	/* Set DCDC enable register */
	nxe1500_i2c_write(NXE1500_REG_DC1CTL	, cache[NXE1500_REG_DC1CTL]		, power);
	nxe1500_i2c_write(NXE1500_REG_DC2CTL	, cache[NXE1500_REG_DC2CTL]		, power);
	nxe1500_i2c_write(NXE1500_REG_DC3CTL	, cache[NXE1500_REG_DC3CTL]		, power);
	nxe1500_i2c_write(NXE1500_REG_DC4CTL	, cache[NXE1500_REG_DC4CTL]		, power);

	/* Set LDO enable register */
	nxe1500_i2c_write(NXE1500_REG_LDOEN1	, cache[NXE1500_REG_LDOEN1]		, power);
	nxe1500_i2c_write(NXE1500_REG_LDOEN2	, cache[NXE1500_REG_LDOEN2]		, power);

	nxe1500_i2c_write(NXE1500_REG_REPCNT	, cache[NXE1500_REG_REPCNT]		, power);
	nxe1500_i2c_write(NXE1500_REG_WATCHDOG	, cache[NXE1500_REG_WATCHDOG]	, power);
	nxe1500_i2c_write(NXE1500_REG_PWRIREN	, cache[NXE1500_REG_PWRIREN]	, power);

#if defined(CONFIG_PMIC_REG_DUMP)
	nxe1500_register_dump(power);
#endif

	return 0;
}

int power_pmic_function_init(void)
{
	int ret = 0;
	int i2c_bus = CONFIG_PMIC_I2C_BUS;

	ret = power_pmic_init(i2c_bus);

	return ret;
}

int bd_pmic_init(void)
{
	struct nxe1500_power nxe_power_config = {
		.i2c_addr = NXE1500_I2C_ADDR,
		.i2c_bus = CONFIG_PMIC_I2C_BUS,
	};

	nxe1500_device_setup(&nxe_power_config);
	return 0;
}

