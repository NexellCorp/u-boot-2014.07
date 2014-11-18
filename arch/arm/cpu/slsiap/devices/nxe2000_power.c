/*
 * (C) Copyright 2013
 * bong kwan kook, Nexell Co, <kook@nexell.co.kr>
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

#include <common.h>
#include <command.h>
#include <i2c.h>

#include "nxe2000_power.h"


/*
 * Debug
 */
#ifdef CONFIG_NXE2000_REG_DUMP
#define DBGOUT(msg...)		do { printf("pmic: " msg); } while (0)
#else
#define DBGOUT(msg...)		do {} while (0)
#endif

static u_char nxe2000_cache_reg[256];

static int nxe2000_i2c_read(u8 reg, u8 *value, struct nxe2000_power *power)
{
	uchar chip = power->i2c_addr;
	u32   addr = (u32)reg & 0xFF;
	int   alen = 1;

	return i2c_read(chip, addr, alen, value, 1);
}

static int nxe2000_i2c_write(u8 reg, u8 value, struct nxe2000_power *power)
{
	uchar chip = power->i2c_addr;
	u32   addr = (u32)reg & 0xFF;
	int   alen = 1;

	return i2c_write(chip, addr, alen, &value, 1);
}

u8 nxe2000_get_ldo_step(u8 ldo_num, int want_vol)
{
	u32	vol_step = 0;
	u32	temp = 0;

	if ((ldo_num == 5) || (ldo_num == 6))
	{
		if (want_vol < NXE2000_DEF_LDO56_VOL_MIN)
		{
			want_vol = NXE2000_DEF_LDO56_VOL_MIN;
		}
		else if (want_vol > NXE2000_DEF_LDO56_VOL_MAX)
		{
			want_vol = NXE2000_DEF_LDO56_VOL_MAX;
		}

		temp = (want_vol - NXE2000_DEF_LDO56_VOL_MIN);
	}
	else if (ldo_num == 11)
	{
		if (want_vol < NXE2000_DEF_LDORTC1_VOL_MIN)
		{
			want_vol = NXE2000_DEF_LDORTC1_VOL_MIN;
		}
		else if (want_vol > NXE2000_DEF_LDORTC1_VOL_MAX)
		{
			want_vol = NXE2000_DEF_LDORTC1_VOL_MAX;
		}

		temp = (want_vol - NXE2000_DEF_LDORTC1_VOL_MIN);
	}
	else
	{
		if (want_vol < NXE2000_DEF_LDOx_VOL_MIN)
		{
			want_vol = NXE2000_DEF_LDOx_VOL_MIN;
		}
		else if (want_vol > NXE2000_DEF_LDOx_VOL_MAX)
		{
			want_vol = NXE2000_DEF_LDOx_VOL_MAX;
		}

		temp = (want_vol - NXE2000_DEF_LDOx_VOL_MIN);
	}

	vol_step	= (temp / NXE2000_DEF_LDOx_VOL_STEP);

	if ((ldo_num == 5) || (ldo_num == 6))
	{
		temp	= (vol_step * NXE2000_DEF_LDOx_VOL_STEP) + NXE2000_DEF_LDO56_VOL_MIN;
	}
	else if (ldo_num == 11)
	{
		temp	= (vol_step * NXE2000_DEF_LDOx_VOL_STEP) + NXE2000_DEF_LDORTC1_VOL_MIN;
	}
	else
	{
		temp	= (vol_step * NXE2000_DEF_LDOx_VOL_STEP) + NXE2000_DEF_LDOx_VOL_MIN;
	}

	return	(u8)(vol_step & 0x7F);
}

u8 nxe2000_get_dcdc_step(u8 dcdc_num, int want_vol)
{
	u32	vol_step = 0;
	u32	temp = 0;

	if (want_vol < NXE2000_DEF_DDCx_VOL_MIN)
	{
		want_vol = NXE2000_DEF_DDCx_VOL_MIN;
	}
	else if (want_vol > NXE2000_DEF_DDCx_VOL_MAX)
	{
		want_vol = NXE2000_DEF_DDCx_VOL_MAX;
	}

	temp		= (want_vol - NXE2000_DEF_DDCx_VOL_MIN);
	vol_step	= (temp / NXE2000_DEF_DDCx_VOL_STEP);
	temp		= (vol_step * NXE2000_DEF_DDCx_VOL_STEP) + NXE2000_DEF_DDCx_VOL_MIN;

	return	(u8)(vol_step & 0xFF);
}

#if defined(CONFIG_NXE2000_REG_DUMP)
void nxe2000_register_dump(struct nxe2000_power *power)
{
	s32 ret=0;
	u16 i=0;
	u8 value[NXE2000_NUM_OF_REGS]={0};

	printf("##########################################################\n");
	printf("##\e[31m %s()\e[0m \n", __func__);
	printf("----------------------------------------------------------\n");
	printf("       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F\n");

	for(i=0; i<=NXE2000_NUM_OF_REGS; i++)
	{
		if(i%16 == 0)
			printf("  %02X:", i);

		if(i%4 == 0)
			printf(" ");

		ret = nxe2000_i2c_read(i, &value[i], power);
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

int nxe2000_param_setup(struct nxe2000_power *power)
{
	u_char *cache = nxe2000_cache_reg;
	u_char	temp;

	DBGOUT("%s\n", __func__);

	/* for Key error. */
	nxe2000_i2c_write(NXE2000_REG_ADCCNT3, 0, power);

	/* Set GPIO4 Condition */
	nxe2000_i2c_read(NXE2000_REG_IOOUT, &temp, power);
	temp |= 0x10;	// High(Hi-Z)
	nxe2000_i2c_write(NXE2000_REG_IOOUT, temp, power);

#if defined(CONFIG_SW_UBC_DETECT)
	/* Set GPIO4 Direction */
	nxe2000_i2c_read(NXE2000_REG_IOSEL, &temp, power);
	temp &= ~0x10;   // input
	nxe2000_i2c_write(NXE2000_REG_IOSEL, temp, power);
#else
	/* Set GPIO4 Direction */
	nxe2000_i2c_read(NXE2000_REG_IOSEL, &temp, power);
	temp |= 0x10;	// output
	nxe2000_i2c_write(NXE2000_REG_IOSEL, temp, power);
#endif
	nxe2000_i2c_read(NXE2000_REG_CHGSTATE	, &cache[NXE2000_REG_CHGSTATE]	, power);
	nxe2000_i2c_read(NXE2000_REG_PWRONTIMSET, &cache[NXE2000_REG_PWRONTIMSET], power);
    cache[NXE2000_REG_PWRONTIMSET] &= ~(0x7 << NXE2000_POS_PWRONTIMSET_OFF_PRESS_PWRON);
    cache[NXE2000_REG_PWRONTIMSET] |= (NXE2000_DEF_OFF_PRESS_TIME << NXE2000_POS_PWRONTIMSET_OFF_PRESS_PWRON);
	nxe2000_i2c_write(NXE2000_REG_PWRONTIMSET, cache[NXE2000_REG_PWRONTIMSET], power);

	nxe2000_i2c_read(NXE2000_REG_POFFHIS	, &cache[NXE2000_REG_POFFHIS]	, power);
	power->warm_reset = cache[NXE2000_REG_POFFHIS] ? 1 : 0;

	nxe2000_i2c_read(NXE2000_REG_FG_CTRL    , &cache[NXE2000_REG_FG_CTRL]   , power);
	cache[NXE2000_REG_FG_CTRL]  &= (1 << NXE2000_POS_FG_CTRL_FG_ACC);

	cache[NXE2000_REG_FG_CTRL]  = (1 << NXE2000_POS_FG_CTRL_SRST0)|
										(1 << NXE2000_POS_FG_CTRL_SRST1);
	nxe2000_i2c_write(NXE2000_REG_FG_CTRL   , cache[NXE2000_REG_FG_CTRL]    , power);

	/* Set DCDC voltage register */
	cache[NXE2000_REG_DC1VOL]		= nxe2000_get_dcdc_step(1, NXE2000_DEF_DDC1_VOL);
	cache[NXE2000_REG_DC2VOL]		= nxe2000_get_dcdc_step(2, NXE2000_DEF_DDC2_VOL);
	cache[NXE2000_REG_DC3VOL]		= nxe2000_get_dcdc_step(3, NXE2000_DEF_DDC3_VOL);
	cache[NXE2000_REG_DC4VOL]		= nxe2000_get_dcdc_step(4, NXE2000_DEF_DDC4_VOL);
	cache[NXE2000_REG_DC5VOL]		= nxe2000_get_dcdc_step(5, NXE2000_DEF_DDC5_VOL);

	cache[NXE2000_REG_DC1VOL_SLP]	= nxe2000_get_dcdc_step(1, NXE2000_DEF_DDC1_SLP_VOL);
	cache[NXE2000_REG_DC2VOL_SLP]	= nxe2000_get_dcdc_step(2, NXE2000_DEF_DDC2_SLP_VOL);
	cache[NXE2000_REG_DC3VOL_SLP]	= nxe2000_get_dcdc_step(3, NXE2000_DEF_DDC3_SLP_VOL);
	cache[NXE2000_REG_DC4VOL_SLP]	= nxe2000_get_dcdc_step(4, NXE2000_DEF_DDC4_SLP_VOL);
	cache[NXE2000_REG_DC5VOL_SLP]	= nxe2000_get_dcdc_step(5, NXE2000_DEF_DDC5_SLP_VOL);

	/* Set LDO voltage register */
	cache[NXE2000_REG_LDO1VOL]		= nxe2000_get_ldo_step(1, NXE2000_DEF_LDO1_VOL);
	cache[NXE2000_REG_LDO2VOL]		= nxe2000_get_ldo_step(2, NXE2000_DEF_LDO2_VOL);
	cache[NXE2000_REG_LDO3VOL]		= nxe2000_get_ldo_step(3, NXE2000_DEF_LDO3_VOL);
	cache[NXE2000_REG_LDO4VOL]		= nxe2000_get_ldo_step(4, NXE2000_DEF_LDO4_VOL);
	cache[NXE2000_REG_LDO5VOL]		= nxe2000_get_ldo_step(5, NXE2000_DEF_LDO5_VOL);
	cache[NXE2000_REG_LDO6VOL]		= nxe2000_get_ldo_step(6, NXE2000_DEF_LDO6_VOL);
	cache[NXE2000_REG_LDO7VOL]		= nxe2000_get_ldo_step(7, NXE2000_DEF_LDO7_VOL);
	cache[NXE2000_REG_LDO8VOL]		= nxe2000_get_ldo_step(8, NXE2000_DEF_LDO8_VOL);
	cache[NXE2000_REG_LDO9VOL]		= nxe2000_get_ldo_step(9, NXE2000_DEF_LDO9_VOL);
	cache[NXE2000_REG_LDO10VOL]		= nxe2000_get_ldo_step(10, NXE2000_DEF_LDO10_VOL);

	cache[NXE2000_REG_LDORTC1VOL]	= nxe2000_get_ldo_step(11, NXE2000_DEF_LDORTC1_VOL);
	cache[NXE2000_REG_LDORTC2VOL]	= nxe2000_get_ldo_step(12, NXE2000_DEF_LDORTC2_VOL);

	cache[NXE2000_REG_LDO1VOL_SLP]	= nxe2000_get_ldo_step(1, NXE2000_DEF_LDO1_SLP_VOL);
	cache[NXE2000_REG_LDO2VOL_SLP]	= nxe2000_get_ldo_step(2, NXE2000_DEF_LDO2_SLP_VOL);
	cache[NXE2000_REG_LDO3VOL_SLP]	= nxe2000_get_ldo_step(3, NXE2000_DEF_LDO3_SLP_VOL);
	cache[NXE2000_REG_LDO4VOL_SLP]	= nxe2000_get_ldo_step(4, NXE2000_DEF_LDO4_SLP_VOL);
	cache[NXE2000_REG_LDO5VOL_SLP]	= nxe2000_get_ldo_step(5, NXE2000_DEF_LDO5_SLP_VOL);
	cache[NXE2000_REG_LDO6VOL_SLP]	= nxe2000_get_ldo_step(6, NXE2000_DEF_LDO6_SLP_VOL);
	cache[NXE2000_REG_LDO7VOL_SLP]	= nxe2000_get_ldo_step(7, NXE2000_DEF_LDO7_SLP_VOL);
	cache[NXE2000_REG_LDO8VOL_SLP]	= nxe2000_get_ldo_step(8, NXE2000_DEF_LDO8_SLP_VOL);
	cache[NXE2000_REG_LDO9VOL_SLP]	= nxe2000_get_ldo_step(9, NXE2000_DEF_LDO9_SLP_VOL);
	cache[NXE2000_REG_LDO10VOL_SLP]	= nxe2000_get_ldo_step(10, NXE2000_DEF_LDO10_SLP_VOL);

	/* Set DCDC control register */
	cache[NXE2000_REG_DC1CTL2]	= ( (NXE2000_DEF_DDC1_OSC_FREQ	 << NXE2000_POS_DCxCTL2_DCxOSC)	|
									(NXE2000_DEF_DDC1_RAMP_SLOP  << NXE2000_POS_DCxCTL2_DCxSR)	|
									(NXE2000_DEF_DDC1_CUR_LIMIT  << NXE2000_POS_DCxCTL2_DCxLIM)	|
									(NXE2000_DEF_DDC1_LIMSHUT_EN << NXE2000_POS_DCxCTL2_DCxLIMSDEN) );

	cache[NXE2000_REG_DC2CTL2]	= ( (NXE2000_DEF_DDC2_OSC_FREQ	 << NXE2000_POS_DCxCTL2_DCxOSC)	|
									(NXE2000_DEF_DDC2_RAMP_SLOP  << NXE2000_POS_DCxCTL2_DCxSR)	|
									(NXE2000_DEF_DDC2_CUR_LIMIT  << NXE2000_POS_DCxCTL2_DCxLIM)	|
									(NXE2000_DEF_DDC2_LIMSHUT_EN << NXE2000_POS_DCxCTL2_DCxLIMSDEN) );

	cache[NXE2000_REG_DC3CTL2]	= ( (NXE2000_DEF_DDC3_OSC_FREQ	 << NXE2000_POS_DCxCTL2_DCxOSC)	|
									(NXE2000_DEF_DDC3_RAMP_SLOP  << NXE2000_POS_DCxCTL2_DCxSR)	|
									(NXE2000_DEF_DDC3_CUR_LIMIT  << NXE2000_POS_DCxCTL2_DCxLIM)	|
									(NXE2000_DEF_DDC3_LIMSHUT_EN << NXE2000_POS_DCxCTL2_DCxLIMSDEN) );

	cache[NXE2000_REG_DC4CTL2]	= ( (NXE2000_DEF_DDC4_OSC_FREQ	 << NXE2000_POS_DCxCTL2_DCxOSC)	|
									(NXE2000_DEF_DDC4_RAMP_SLOP  << NXE2000_POS_DCxCTL2_DCxSR)	|
									(NXE2000_DEF_DDC4_CUR_LIMIT  << NXE2000_POS_DCxCTL2_DCxLIM)	|
									(NXE2000_DEF_DDC4_LIMSHUT_EN << NXE2000_POS_DCxCTL2_DCxLIMSDEN) );

	cache[NXE2000_REG_DC5CTL2]	= ( (NXE2000_DEF_DDC5_OSC_FREQ	 << NXE2000_POS_DCxCTL2_DCxOSC)	|
									(NXE2000_DEF_DDC5_RAMP_SLOP  << NXE2000_POS_DCxCTL2_DCxSR)	|
									(NXE2000_DEF_DDC5_CUR_LIMIT  << NXE2000_POS_DCxCTL2_DCxLIM)	|
									(NXE2000_DEF_DDC5_LIMSHUT_EN << NXE2000_POS_DCxCTL2_DCxLIMSDEN) );

	/* DCDC - Enable */
	cache[NXE2000_REG_DC1CTL]	= ( (NXE2000_DEF_DDC1_SLP_MODE	<< NXE2000_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE2000_DEF_DDC1_OSC_FREQ	<< NXE2000_POS_DCxCTL_DCxMODE)	|
									(NXE2000_DEF_DDC1_DSC_CTRL	<< NXE2000_POS_DCxCTL_DCxDIS)	|
									(NXE2000_DEF_DDC1_ON		<< NXE2000_POS_DCxCTL_DCxEN) );

	cache[NXE2000_REG_DC2CTL]	= ( (NXE2000_DEF_DDC2_SLP_MODE	<< NXE2000_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE2000_DEF_DDC2_OSC_FREQ	<< NXE2000_POS_DCxCTL_DCxMODE)	|
									(NXE2000_DEF_DDC2_DSC_CTRL	<< NXE2000_POS_DCxCTL_DCxDIS)	|
									(NXE2000_DEF_DDC2_ON		<< NXE2000_POS_DCxCTL_DCxEN) );

	cache[NXE2000_REG_DC3CTL]	= ( (NXE2000_DEF_DDC3_SLP_MODE	<< NXE2000_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE2000_DEF_DDC3_OSC_FREQ	<< NXE2000_POS_DCxCTL_DCxMODE)	|
									(NXE2000_DEF_DDC3_DSC_CTRL	<< NXE2000_POS_DCxCTL_DCxDIS)	|
									(NXE2000_DEF_DDC3_ON		<< NXE2000_POS_DCxCTL_DCxEN) );

	cache[NXE2000_REG_DC4CTL]	= ( (NXE2000_DEF_DDC4_SLP_MODE	<< NXE2000_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE2000_DEF_DDC4_OSC_FREQ	<< NXE2000_POS_DCxCTL_DCxMODE)	|
									(NXE2000_DEF_DDC4_DSC_CTRL	<< NXE2000_POS_DCxCTL_DCxDIS)	|
									(NXE2000_DEF_DDC4_ON		<< NXE2000_POS_DCxCTL_DCxEN) );

	cache[NXE2000_REG_DC5CTL]	= ( (NXE2000_DEF_DDC5_SLP_MODE	<< NXE2000_POS_DCxCTL_DCxMODE_SLP)	|
									(NXE2000_DEF_DDC5_OSC_FREQ	<< NXE2000_POS_DCxCTL_DCxMODE)	|
									(NXE2000_DEF_DDC5_DSC_CTRL	<< NXE2000_POS_DCxCTL_DCxDIS)	|
									(NXE2000_DEF_DDC5_ON		<< NXE2000_POS_DCxCTL_DCxEN) );

	/* LDO - Enable */
	cache[NXE2000_REG_LDOEN1]	= ( (NXE2000_DEF_LDO1_ON << NXE2000_POS_LDOEN1_LDO1EN) |
									(NXE2000_DEF_LDO2_ON << NXE2000_POS_LDOEN1_LDO2EN) |
									(NXE2000_DEF_LDO3_ON << NXE2000_POS_LDOEN1_LDO3EN) |
									(NXE2000_DEF_LDO4_ON << NXE2000_POS_LDOEN1_LDO4EN) |
                                    (NXE2000_DEF_LDO5_ON << NXE2000_POS_LDOEN1_LDO5EN) |
                                    (NXE2000_DEF_LDO6_ON << NXE2000_POS_LDOEN1_LDO6EN) |
                                    (NXE2000_DEF_LDO7_ON << NXE2000_POS_LDOEN1_LDO7EN) |
									(NXE2000_DEF_LDO8_ON << NXE2000_POS_LDOEN1_LDO8EN) );

	cache[NXE2000_REG_LDOEN2]	= ( (NXE2000_DEF_LDORTC1_ON	<< NXE2000_POS_LDOEN2_LDORTC1EN)|
                                    (NXE2000_DEF_LDORTC2_ON	<< NXE2000_POS_LDOEN2_LDORTC2EN)|
                                    (NXE2000_DEF_LDO9_ON	<< NXE2000_POS_LDOEN2_LDO9EN)	|
									(NXE2000_DEF_LDO10_ON	<< NXE2000_POS_LDOEN2_LDO10EN) );

	cache[NXE2000_REG_ADCCNT1]  =  ((NXE2000_DEF_ADC_AIN0 << NXE2000_POS_ADC_AIN0)	|
									(NXE2000_DEF_ADC_AIN1 << NXE2000_POS_ADC_AIN1)	|
									(NXE2000_DEF_ADC_VTHM << NXE2000_POS_ADC_VTHM)	|
									(NXE2000_DEF_ADC_VSYS << NXE2000_POS_ADC_VSYS)	|
									(NXE2000_DEF_ADC_VUSB << NXE2000_POS_ADC_VUSB)	|
									(NXE2000_DEF_ADC_VADP << NXE2000_POS_ADC_VADP)	|
									(NXE2000_DEF_ADC_VBAT << NXE2000_POS_ADC_VBAT)	|
									(NXE2000_DEF_ADC_ILIM << NXE2000_POS_ADC_ILIM) );

	cache[NXE2000_REG_ADCCNT3]  =  ((NXE2000_DEF_ADCCNT3_ADRQ	<< NXE2000_POS_ADCCNT3_ADRQ)	|
									(NXE2000_DEF_ADCCNT3_AVE	<< NXE2000_POS_ADCCNT3_AVE)		|
									(NXE2000_DEF_ADCCNT3_ADSEL	<< NXE2000_POS_ADCCNT3_ADSEL) );

	/* Set charge control register. */
#if defined(CONFIG_HAVE_BATTERY)
	cache[NXE2000_REG_CHGCTL1]  =  ((NXE2000_DEF_CHG_PRIORITY		<< NXE2000_POS_CHGCTL1_CHGP)		|
									(NXE2000_DEF_CHG_COMPLETE_DIS	<< NXE2000_POS_CHGCTL1_CHGCMP_DIS)	|
									(NXE2000_DEF_CHG_NOBAT_OVLIM_EN	<< NXE2000_POS_CHGCTL1_NOBATOVLIM)	|
									(NXE2000_DEF_CHG_OTG_BOOST		<< NXE2000_POS_CHGCTL1_OTG_BOOST_EN)|
									(NXE2000_DEF_CHG_SUSPEND		<< NXE2000_POS_CHGCTL1_SUSPEND)		|
									(NXE2000_DEF_CHG_JEITAEN		<< NXE2000_POS_CHGCTL1_JEITAEN)		|
									(NXE2000_DEF_CHG_USB_EN			<< NXE2000_POS_CHGCTL1_VUSBCHGEN)	|
									(NXE2000_DEF_CHG_ADP_EN			<< NXE2000_POS_CHGCTL1_VADPCHGEN) );

	#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP_UBC)
		cache[NXE2000_REG_CHGCTL1] &= ~(1 << NXE2000_POS_CHGCTL1_CHGP);
	#endif
	#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP)
		cache[NXE2000_REG_CHGCTL1] &= ~(1 << NXE2000_POS_CHGCTL1_CHGP);
		cache[NXE2000_REG_CHGCTL1] &= ~(1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
	#endif

	#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_UBC)
		cache[NXE2000_REG_CHGCTL1] &= ~(1 << NXE2000_POS_CHGCTL1_VADPCHGEN);
	#endif

#else

	#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP)
		cache[NXE2000_REG_CHGCTL1]	= (1 << NXE2000_POS_CHGCTL1_SUSPEND);
	#endif
#endif	/* CONFIG_HAVE_BATTERY */

	cache[NXE2000_REG_CHGCTL2]	= ( (NXE2000_DEF_CHG_USB_VCONTMASK	<< NXE2000_POS_CHGCTL2_USB_VCONTMASK) |
									(NXE2000_DEF_CHG_ADP_VCONTMASK	<< NXE2000_POS_CHGCTL2_ADP_VCONTMASK) |
									(NXE2000_DEF_CHG_VUSB_BUCK_THS	<< NXE2000_POS_CHGCTL2_VUSBBUCK_VTH) |
									(NXE2000_DEF_CHG_VADP_BUCK_THS	<< NXE2000_POS_CHGCTL2_VADPBUCK_VTH) );

	cache[NXE2000_REG_VSYSSET]	= ( (NXE2000_DEF_CHG_VSYS_VOL		<< NXE2000_POS_VSYSSET_VSYSSET) |
									(NXE2000_DEF_CHG_VSYS_OVER_VOL	<< NXE2000_POS_VSYSSET_VSYSOVSET) );

	/* Charge current setting register. */
	cache[NXE2000_REG_REGISET1]	= (NXE2000_DEF_LIMIT_ADP_AMP / 100000) - 1;
	cache[NXE2000_REG_REGISET2]	= (NXE2000_DEF_LIMIT_USBDATA_AMP / 100000) - 1;

#if defined(CONFIG_SW_UBC_DETECT)
	cache[NXE2000_REG_CHGISET]	=   ( (CHARGER_CURRENT_COMPLETE << NXE2000_POS_CHGISET_ICCHG) |
									 		((NXE2000_DEF_CHG_ADP_AMP / 100000) - 1) );
#else
	cache[NXE2000_REG_CHGISET]	=   ( (CHARGER_CURRENT_COMPLETE << NXE2000_POS_CHGISET_ICCHG) |
											((NXE2000_DEF_CHG_USB_AMP / 100000) - 1) );
#endif

	cache[NXE2000_REG_TIMSET]	= ( (NXE2000_DEF_CHG_RAPID_TTIM_80 << NXE2000_POS_TIMSET_TTIMSET) |
										((NXE2000_DEF_CHG_RAPID_CTIME & 3) << NXE2000_POS_TIMSET_CTIMSET) |
										(NXE2000_DEF_CHG_RAPID_RTIME & 3) );

	cache[NXE2000_REG_BATSET1]	= ( ((NXE2000_DEF_CHG_POWER_ON_VOL & 7)	<< NXE2000_POS_BATSET1_CHGPON) |
										((NXE2000_DEF_CHG_VBATOV_SET & 1)	<< NXE2000_POS_BATSET1_VBATOVSET) |
										((NXE2000_DEF_CHG_VWEAK	& 3)		<< NXE2000_POS_BATSET1_VWEAK) |
										((NXE2000_DEF_CHG_VDEAD	& 1)		<< NXE2000_POS_BATSET1_VDEAD) |
										((NXE2000_DEF_CHG_VSHORT & 1)		<< NXE2000_POS_BATSET1_VSHORT) );

	cache[NXE2000_REG_BATSET2]	= ( ((NXE2000_DEF_CHG_VFCHG & 7) << NXE2000_POS_BATSET2_VFCHG) | (NXE2000_DEF_CHG_VRCHG & 7) );

	cache[NXE2000_REG_FG_CTRL]  = ( (1 << NXE2000_POS_FG_CTRL_FG_ACC)
									|(1 << NXE2000_POS_FG_CTRL_FG_EN) );

	cache[NXE2000_REG_DIESET] = ((NXE2000_DEF_DIE_RETURN_TEMP << NXE2000_POS_DIESET_DIERTNTEMP)
								| (NXE2000_DEF_DIE_ERROR_TEMP << NXE2000_POS_DIESET_DIEERRTEMP)
								| (NXE2000_DEF_DIE_SHUTDOWN_TEMP << NXE2000_POS_DIESET_DIESHUTTEMP));

	cache[NXE2000_REG_REPCNT] = ((NXE2000_DEF_REPCNT_OFF_RESETO	<< NXE2000_POS_REPCNT_OFF_RESETO)
								| (NXE2000_DEF_REPCNT_REPWRTIM	<< NXE2000_POS_REPCNT_REPWRTIM)
								| (NXE2000_DEF_REPCNT_REPWRON	<< NXE2000_POS_REPCNT_REPWRON));

	cache[NXE2000_REG_WATCHDOG] = ((NXE2000_DEF_WATHDOG_WDOGSLPEN	<< NXE2000_POS_WATHDOG_WDOGSLPEN)
								| (NXE2000_DEF_WATHDOG_WDOG_EN		<< NXE2000_POS_WATHDOG_WDOG_EN)
								| (NXE2000_DEF_WATHDOG_WDOGTIM		<< NXE2000_POS_WATHDOG_WDOGTIM));

	cache[NXE2000_REG_PWRIREN] = ((NXE2000_DEF_PWRIREN_WDOG			<< NXE2000_POS_PWRIREN_WDOG)
								| (NXE2000_DEF_PWRIREN_NOE_OFF		<< NXE2000_POS_PWRIREN_NOE_OFF)
								| (NXE2000_DEF_PWRIREN_PWRON_OFF	<< NXE2000_POS_PWRIREN_PWRON_OFF)
								| (NXE2000_DEF_PWRIREN_PREOT		<< NXE2000_POS_PWRIREN_PREOT)
								| (NXE2000_DEF_PWRIREN_PRVINDT		<< NXE2000_POS_PWRIREN_PRVINDT)
								| (NXE2000_DEF_PWRIREN_EXTIN		<< NXE2000_POS_PWRIREN_EXTIN)
								| (NXE2000_DEF_PWRIREN_PWRON		<< NXE2000_POS_PWRIREN_PWRON));
	return 0;
}

int nxe2000_device_setup(struct nxe2000_power *power)
{
	u_char	*cache = nxe2000_cache_reg;
	int		bus = power->i2c_bus;
#if defined(CONFIG_NXE2000_REG_DUMP)
	int i;
	s32 ret=0;
#endif

	DBGOUT("%s\n", __func__);

	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	i2c_set_bus_num(bus);

#if defined(CONFIG_NXE2000_REG_DUMP)
	printf("##########################################################\n");
	printf("##\e[31m PMIC OTP Value \e[0m \n");
	printf("       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F\n");
	for(i=0; i<=NXE2000_NUM_OF_REGS; i++)
	{
		if(i%16 == 0)
			printf("  %02X:", i);
		if(i%4 == 0)
			printf(" ");

		ret = nxe2000_i2c_read(i, &cache[i]	, power);
		if(!ret)
			printf("%02x ", cache[i]);
		else
			printf("\e[31mxx\e[0m ");

		if((i+1)%16 == 0)
			printf("\n");
	}
	printf("##########################################################\n");
#endif

	nxe2000_param_setup(power);

	/* Set fuel gauge enable */
#if defined(CONFIG_HAVE_BATTERY)
	nxe2000_i2c_write(NXE2000_REG_CHGCTL1	, cache[NXE2000_REG_CHGCTL1]	, power);
	nxe2000_i2c_write(NXE2000_REG_FG_CTRL	, cache[NXE2000_REG_FG_CTRL]	, power);

	do {
		nxe2000_i2c_read(NXE2000_REG_FG_CTRL, &cache[NXE2000_REG_FG_CTRL]	, power);
	} while(cache[NXE2000_REG_FG_CTRL] & 0xC0);
#endif

	/* Set DCDC voltage register */
	nxe2000_i2c_write(NXE2000_REG_DC1VOL	, cache[NXE2000_REG_DC1VOL]		, power);
	nxe2000_i2c_write(NXE2000_REG_DC2VOL	, cache[NXE2000_REG_DC2VOL]		, power);
	nxe2000_i2c_write(NXE2000_REG_DC3VOL	, cache[NXE2000_REG_DC3VOL]		, power);
	nxe2000_i2c_write(NXE2000_REG_DC4VOL	, cache[NXE2000_REG_DC4VOL]		, power);
	nxe2000_i2c_write(NXE2000_REG_DC5VOL	, cache[NXE2000_REG_DC5VOL]		, power);

	nxe2000_i2c_write(NXE2000_REG_DC1VOL_SLP, cache[NXE2000_REG_DC1VOL_SLP]	, power);
	nxe2000_i2c_write(NXE2000_REG_DC2VOL_SLP, cache[NXE2000_REG_DC2VOL_SLP]	, power);
	nxe2000_i2c_write(NXE2000_REG_DC3VOL_SLP, cache[NXE2000_REG_DC3VOL_SLP]	, power);
	nxe2000_i2c_write(NXE2000_REG_DC4VOL_SLP, cache[NXE2000_REG_DC4VOL_SLP]	, power);
	nxe2000_i2c_write(NXE2000_REG_DC5VOL_SLP, cache[NXE2000_REG_DC5VOL_SLP]	, power);

	/* Set LDO voltage register */
	nxe2000_i2c_write(NXE2000_REG_LDO1VOL	, cache[NXE2000_REG_LDO1VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO2VOL	, cache[NXE2000_REG_LDO2VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO3VOL	, cache[NXE2000_REG_LDO3VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO4VOL	, cache[NXE2000_REG_LDO4VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO5VOL	, cache[NXE2000_REG_LDO5VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO6VOL	, cache[NXE2000_REG_LDO6VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO7VOL	, cache[NXE2000_REG_LDO7VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO8VOL	, cache[NXE2000_REG_LDO8VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO9VOL	, cache[NXE2000_REG_LDO9VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDO10VOL	, cache[NXE2000_REG_LDO10VOL]	, power);

	nxe2000_i2c_write(NXE2000_REG_LDORTC1VOL, cache[NXE2000_REG_LDORTC1VOL]	, power);
	nxe2000_i2c_write(NXE2000_REG_LDORTC2VOL, cache[NXE2000_REG_LDORTC2VOL]	, power);

	nxe2000_i2c_write(NXE2000_REG_LDO1VOL_SLP, cache[NXE2000_REG_LDO1VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO2VOL_SLP, cache[NXE2000_REG_LDO2VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO3VOL_SLP, cache[NXE2000_REG_LDO3VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO4VOL_SLP, cache[NXE2000_REG_LDO4VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO5VOL_SLP, cache[NXE2000_REG_LDO5VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO6VOL_SLP, cache[NXE2000_REG_LDO6VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO7VOL_SLP, cache[NXE2000_REG_LDO7VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO8VOL_SLP, cache[NXE2000_REG_LDO8VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO9VOL_SLP, cache[NXE2000_REG_LDO9VOL_SLP], power);
	nxe2000_i2c_write(NXE2000_REG_LDO10VOL_SLP,cache[NXE2000_REG_LDO10VOL_SLP], power);

	/* Set DCDC control register */
	nxe2000_i2c_write(NXE2000_REG_DC1CTL2	, cache[NXE2000_REG_DC1CTL2]	, power);
	nxe2000_i2c_write(NXE2000_REG_DC2CTL2	, cache[NXE2000_REG_DC2CTL2]	, power);
	nxe2000_i2c_write(NXE2000_REG_DC3CTL2	, cache[NXE2000_REG_DC3CTL2]	, power);
	nxe2000_i2c_write(NXE2000_REG_DC4CTL2	, cache[NXE2000_REG_DC4CTL2]	, power);
	nxe2000_i2c_write(NXE2000_REG_DC5CTL2	, cache[NXE2000_REG_DC5CTL2]	, power);

	/* Set DCDC enable register */
	nxe2000_i2c_write(NXE2000_REG_DC1CTL	, cache[NXE2000_REG_DC1CTL]		, power);
	nxe2000_i2c_write(NXE2000_REG_DC2CTL	, cache[NXE2000_REG_DC2CTL]		, power);
	nxe2000_i2c_write(NXE2000_REG_DC3CTL	, cache[NXE2000_REG_DC3CTL]		, power);
	nxe2000_i2c_write(NXE2000_REG_DC4CTL	, cache[NXE2000_REG_DC4CTL]		, power);
	nxe2000_i2c_write(NXE2000_REG_DC5CTL	, cache[NXE2000_REG_DC5CTL]		, power);

	/* Set LDO enable register */
	nxe2000_i2c_write(NXE2000_REG_LDOEN1	, cache[NXE2000_REG_LDOEN1]		, power);
	nxe2000_i2c_write(NXE2000_REG_LDOEN2	, cache[NXE2000_REG_LDOEN2]		, power);

#if defined(CONFIG_HAVE_BATTERY)
	/* Set charge control register. */
	//nxe2000_i2c_write(NXE2000_REG_CHGCTL1	, cache[NXE2000_REG_CHGCTL1]	, power);
	nxe2000_i2c_write(NXE2000_REG_CHGCTL2	, cache[NXE2000_REG_CHGCTL2]	, power);
	nxe2000_i2c_write(NXE2000_REG_VSYSSET	, cache[NXE2000_REG_VSYSSET]	, power);

	/* Charge current setting register. */
	nxe2000_i2c_write(NXE2000_REG_REGISET1	, cache[NXE2000_REG_REGISET1]	, power);
	nxe2000_i2c_write(NXE2000_REG_REGISET2	, cache[NXE2000_REG_REGISET2]	, power);
	nxe2000_i2c_write(NXE2000_REG_CHGISET	, cache[NXE2000_REG_CHGISET]	, power);
#endif

	nxe2000_i2c_write(NXE2000_REG_TIMSET	, cache[NXE2000_REG_TIMSET]		, power);
	nxe2000_i2c_write(NXE2000_REG_BATSET1	, cache[NXE2000_REG_BATSET1]	, power);
	nxe2000_i2c_write(NXE2000_REG_BATSET2	, cache[NXE2000_REG_BATSET2]	, power);

	/* Redetect for UBC */
	nxe2000_i2c_write(NXE2000_REG_EXTIF_GCHGDET,    0x01,   power);

	nxe2000_i2c_write(NXE2000_REG_DIESET	, cache[NXE2000_REG_DIESET]		, power);

	nxe2000_i2c_write(NXE2000_REG_REPCNT	, cache[NXE2000_REG_REPCNT]		, power);

	nxe2000_i2c_write(NXE2000_REG_WATCHDOG	, cache[NXE2000_REG_WATCHDOG]	, power);

	nxe2000_i2c_write(NXE2000_REG_PWRIREN	, cache[NXE2000_REG_PWRIREN]	, power);

#ifdef CONFIG_NXE2000_REG_DUMP
	nxe2000_register_dump(power);
#endif

	return 0;
}


