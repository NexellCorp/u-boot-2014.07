/*
 *  Copyright (C) 2016 NEXELL SOC Lab.
 *  Jongsin Park <pjsin865@nexell.co.kr>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <errno.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <power/pmic.h>

#include "nxe1500-private.h"
#include <nxe1500_power.h>

static int nxe1500_get_enable_reg(
		int pwr_src, u32 *reg, u32 *mask)
{
	switch (pwr_src) {
	case NXE1500_BUCK1 ... NXE1500_BUCK4:
		*reg = NXE1500_REG_DC1CTL + ((pwr_src - NXE1500_BUCK1) * 2);
		*mask = 1;
		break;
	case NXE1500_LDO1 ... NXE1500_LDO5:
		*reg = NXE1500_REG_LDOEN1;
		*mask = 1 << (pwr_src - NXE1500_LDO1);
		break;
    case NXE1500_LDORTC1 ... NXE1500_LDORTC2:
        *reg = NXE1500_REG_LDOEN2;
        *mask = 1 << ((pwr_src - NXE1500_LDORTC1) + 4);
        break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int nxe1500_get_vol_reg(
		int pwr_src, u32 *_reg, u32 *_shift, u32 *_mask)
{
	u32 reg, shift = 0, mask = 0xff;

	switch (pwr_src) {
	case NXE1500_BUCK1 ... NXE1500_BUCK4:
		reg = NXE1500_REG_DC1VOL + (pwr_src - NXE1500_BUCK1);
		mask = 0xff;
		break;
	case NXE1500_LDO1 ... NXE1500_LDORTC2:
		reg = NXE1500_REG_LDO1VOL + (pwr_src - NXE1500_LDO1);
		mask = 0x7f;
		break;
	default:
		return -EINVAL;
	}

	*_reg = reg;
	*_shift = shift;
	*_mask = mask;

	return 0;
}

static u32 nxe1500_get_vol_val(
		int pwr_src, int set_uV)
{
	int min_uV, max_uV, step_uV;
    u32 val;

	val = 0;

	switch (pwr_src) {
	case NXE1500_BUCK1 ... NXE1500_BUCK4:
        min_uV  = 600000;
        max_uV  = 3500000;
        step_uV = 12500;
		break;
    case NXE1500_LDO3:
        min_uV  = 600000;
        max_uV  = 3500000;
        step_uV = 50000;
        break;
	case NXE1500_LDO1 ... NXE1500_LDO2:
	case NXE1500_LDO4 ... NXE1500_LDO5:
    case NXE1500_LDORTC2:
        min_uV  = 900000;
        max_uV  = 3500000;
        step_uV = 50000;
		break;
    case NXE1500_LDORTC1:
        min_uV  = 1200000;
        max_uV  = 3500000;
        step_uV = 50000;
        break;
	default:
        return val;
	}

	if (set_uV < min_uV)
        set_uV = min_uV;
	if (set_uV > max_uV)
        set_uV = max_uV;

    val = (set_uV - min_uV) / step_uV;

	return val;
}

int nxe1500_set_vol(struct pmic *p, int pwr_src, int set_uV, int pwr_on)
{
	u32 reg, val, shift, mask;
	int ret = 0;

	if (pwr_on) {
		/* Set Voltage */
		ret = nxe1500_get_vol_reg(pwr_src, &reg, &shift, &mask);
		if (ret < 0)
			return ret;

		val = nxe1500_get_vol_val(pwr_src, set_uV);
		ret = pmic_reg_write(p, reg, val);
		if (ret < 0)
			return ret;
	}

	ret = nxe1500_get_enable_reg(pwr_src, &reg, &mask);
	if (ret < 0)
		return ret;

	ret = pmic_reg_read(p, reg, &val);
	if (ret < 0)
		return ret;

	if (pwr_on)	/* Set Enable */
		val |= mask;
	else	/* Set Disable */
		val &= ~mask;

	ret = pmic_reg_write(p, reg, val);
	if (ret < 0)
		return ret;

	return 0;
}

static int pmic_charger_state(struct pmic *p, int state, int current)
{
    int ret = 0;

	if (pmic_probe(p))
		return -1;

	return ret;
}

static int pmic_charger_bat_present(struct pmic *p)
{
    int ret = 0;

	if (pmic_probe(p))
		return -1;

    return ret;
}

static int pmic_chrg_get_type(struct pmic *p, u32 ctrl_en)
{
	return CHARGER_NO;
}

static struct power_chrg power_chrg_pmic_ops = {
	.chrg_type = pmic_chrg_get_type,
	.chrg_bat_present = pmic_charger_bat_present,
	.chrg_state = pmic_charger_state,
};

int power_pmic_init(unsigned char bus)
{
	static const char name[] = "PMIC_NXE1500";
	struct pmic *p = pmic_alloc();

	if (!p) {
		printf("%s: POWER allocation error!\n", __func__);
		return -ENOMEM;
	}

	debug("Board PMIC init\n");

	p->name = name;
	p->interface = PMIC_I2C;
	p->number_of_regs = NXE1500_NUM_OF_REGS;
	p->hw.i2c.addr = NXE1500_I2C_ADDR;
	p->hw.i2c.tx_num = 1;
	p->bus = bus;

	p->chrg = &power_chrg_pmic_ops;
	return 0;
}
