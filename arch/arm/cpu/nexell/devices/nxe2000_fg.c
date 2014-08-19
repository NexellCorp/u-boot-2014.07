/*
 *  Copyright (C) 2013 NEXELL SOC Lab.
 *  BongKwan Kook <kook@nexell.co.kr>
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
#include <power/pmic.h>
#include <power/power_chrg.h>
#include <power/battery.h>
#include <power/fg_battery_cell_params.h>
#include <nxe2000_power.h>
#include <i2c.h>
#include <errno.h>
#include "nxe2000-private.h"

static int power_update_battery(struct pmic *p, struct pmic *bat)
{
	struct power_battery *pb = bat->pbat;
	u32 val;
    u32 vol0, vol1;
	int ret = 0;

	if (pmic_probe(p)) {
		puts("Can't find NXE2000 fuel gauge\n");
		return -1;
	}

	ret |= pmic_reg_read(p, NXE2000_REG_SOC, &val);
	pb->bat->state_of_chrg = val;

    ret |= pmic_reg_read(p, NXE2000_REG_VBATDATAH, &vol1);
	ret |= pmic_reg_read(p, NXE2000_REG_VBATDATAL, &vol0);

	val = (vol1 << 4) | vol0;
	debug("vfsoc: 0x%x\n", val);

    vol0 = (NXE2000_POS_ADCCNT3_ADRQ_SINGLE | NXE2000_POS_ADCCNT3_ADSEL_VBAT);
    pmic_reg_write(p, NXE2000_REG_ADCCNT3, vol0);

	/* conversion unit 1 Unit is 1.22mV (5000/4095 mV) */
	val = (val * 5000) / 4095;

	/* return unit should be 1uV */
	pb->bat->voltage_uV = val * 1000;

    ret |= pmic_reg_read(p, NXE2000_REG_RE_CAP_H, &vol1);
    ret |= pmic_reg_read(p, NXE2000_REG_RE_CAP_L, &vol0);

	pb->bat->capacity = (vol1 << 8) | vol0;

	return ret;
}

static int power_check_battery(struct pmic *p, struct pmic *bat)
{
	struct power_battery *pb = bat->pbat;
	unsigned int val;
	int ret = 0;

	if (pmic_probe(p)) {
		puts("Can't find NXE2000 fuel gauge\n");
		return -1;
	}

#if 0
	ret |= pmic_reg_read(p, NXE2000_REG_PONHIS, &val);
	debug("power on history: 0x%x\n", val);

	if (val & (1 << NXE2000_POS_PONHIS_PWRONPON))
		por_fuelgauge_init(p);
#endif

#if 0
	ret |= pmic_reg_read(p, NXE2000_REG_POFFHIS, &val);
	debug("power off history: 0x%x\n", val);

	if ( !val )
		por_fuelgauge_init(p);
#endif

	ret |= pmic_reg_read(p, NXE2000_REG_LSIVER, &val);
	pb->bat->version = val;

	power_update_battery(p, bat);
	debug("fg ver: 0x%x\n", pb->bat->version);
	printf("BAT: state_of_charge(SOC):%d%%\n",
	       pb->bat->state_of_chrg);

	printf("     voltage: %d.%6.6d [V] (expected to be %d [mAh])\n",
	       pb->bat->voltage_uV / 1000000,
	       pb->bat->voltage_uV % 1000000,
	       pb->bat->capacity);

	if (pb->bat->voltage_uV > 3850000)
		pb->bat->state = EXT_SOURCE;
	else if (pb->bat->voltage_uV < 3600000 || pb->bat->state_of_chrg < 5)
		pb->bat->state = CHARGE;
	else
		pb->bat->state = NORMAL;

	return ret;
}

static struct power_fg power_fg_ops = {
	.fg_battery_check = power_check_battery,
	.fg_battery_update = power_update_battery,
};

int power_fg_init(unsigned char bus)
{
	static const char name[] = "FG_NXE2000";
	struct pmic *p = pmic_alloc();

	if (!p) {
		printf("%s: POWER allocation error!\n", __func__);
		return -ENOMEM;
	}

	debug("Board Fuel Gauge init\n");

	p->name = name;
	p->interface = PMIC_I2C;
	p->number_of_regs = NXE2000_NUM_OF_REGS;
	p->hw.i2c.addr = NXE2000_I2C_ADDR;
	p->hw.i2c.tx_num = 1;
	p->bus = bus;

	p->fg = &power_fg_ops;
	return 0;
}
