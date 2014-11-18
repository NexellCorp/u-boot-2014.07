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
#include <errno.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <power/pmic.h>
#include <power/power_chrg.h>

#if defined(CONFIG_OTG_PHY_NEXELL)
#include <otg_phy.h>
#endif
#include "nxe2000-private.h"
#include <nxe2000_power.h>

#define NXE2000_CHG_PRIO		(0)		/* 1:VUSB, 0:VADP	*/

#if (NXE2000_CHG_PRIO == 1)
#define NXE2000_CHG_BITS    ( (0x1 << NXE2000_POS_CHGCTL1_CHGP)         \
                            | (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN) )
#else
#define NXE2000_CHG_BITS    ( (0x0 << NXE2000_POS_CHGCTL1_CHGP)         \
                            | (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN) )
#endif

#define NXE2000_SPD_MASK    ( (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN)    \
                            | (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN)    \
                            | (0x1 << NXE2000_POS_CHGCTL1_SUSPEND) )

#define NXE2000_CHG_MASK    ( (0x1 << NXE2000_POS_CHGCTL1_CHGP)         \
                            | (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN)    \
                            | (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN)    \
                            | (0x1 << NXE2000_POS_CHGCTL1_SUSPEND) )


#if !defined (CONFIG_PMIC_VOLTAGE_CHECK_WITH_CHARGE)
u32 chgctl_reg_val;
#endif
#if defined(CONFIG_FASTBOOT) && defined(CONFIG_SW_UBC_DETECT)
extern int otg_bind_check(int miliSec_Timeout);

static int s_otg_bind_status;
static int s_otg_bind_flag;
static int muic_chrg_get_type(struct pmic *p, u32 ctrl_en)
{
	u32 val = 0, tmp = 0;
	u32 chg_state;
	unsigned char charger;

	if (pmic_probe(p))
		return CHARGER_NO;

	pmic_reg_read(p, NXE2000_REG_CHGSTATE, &chg_state);
	if ((chg_state & 0xC0) == 0x00)
		return CHARGER_UNKNOWN;

#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP) || (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP_UBC)
#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP_UBC)
	if (chg_state & 0x40)
#endif
	{
		if (ctrl_en)
		{
			pmic_reg_read(p, NXE2000_REG_CHGCTL1, &val);
			val &= ~(0x1 << NXE2000_POS_CHGCTL1_CHGP);
			val |=  (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN);
			pmic_reg_write(p, NXE2000_REG_CHGCTL1, val);
		}
		else
		{
			chgctl_reg_val &= ~(0x1 << NXE2000_POS_CHGCTL1_CHGP);
			chgctl_reg_val |= (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN);
		}
		charger = CHARGER_TA;
		return charger;
	}

	if (ctrl_en)
	{
		pmic_reg_read(p, NXE2000_REG_CHGCTL1, &val);
		val &= ~(0x1 << NXE2000_POS_CHGCTL1_CHGP);
		val |= (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
		pmic_reg_write(p, NXE2000_REG_CHGCTL1, val);
	}
	else
	{
		chgctl_reg_val &= ~(0x1 << NXE2000_POS_CHGCTL1_CHGP);
		chgctl_reg_val |= (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
	}
#endif

	s_otg_bind_status = otg_bind_check(500);
	s_otg_bind_flag = 1;

#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_UBC)
	if (ctrl_en)
	{
		pmic_reg_read(p, NXE2000_REG_CHGCTL1, &val);
		val |= (0x1 << NXE2000_POS_CHGCTL1_CHGP)
			|  (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
		pmic_reg_write(p, NXE2000_REG_CHGCTL1, val);
	}
	else
	{
		chgctl_reg_val |= (0x1 << NXE2000_POS_CHGCTL1_CHGP);
		chgctl_reg_val |= (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
	}
#endif

	pmic_reg_read(p, NXE2000_REG_CHGSTATE, &chg_state);

	if (s_otg_bind_status == 1) {
		tmp = (NXE2000_DEF_LIMIT_USBDATA_AMP / 100000);
		charger = CHARGER_USB;
	} else {
		tmp = (NXE2000_DEF_LIMIT_USB_AMP / 100000);
		charger = CHARGER_TA;
	}
	val = 0xE0 | ((tmp < 15) ? (tmp - 1) : 14);
	pmic_reg_write(p, NXE2000_REG_REGISET2, val);

	return charger;
}
#else

static int nxe2000_apsd_detect(struct pmic *p)
{
	int ret = 0;
	int retry = 30;
    u32 value, tmp;
    u32 chg_state;

	pmic_reg_read(p, NXE2000_REG_CHGSTATE, &chg_state);

	value = (1 << NXE2000_POS_GCHGDET_DET_RESTART);
	ret  = pmic_reg_write(p, NXE2000_REG_EXTIF_GCHGDET, value);
    udelay(10);

recheck_detect:
	ret = pmic_reg_read(p, NXE2000_REG_EXTIF_GCHGDET, &value);
    if ( (ret || (value & 0x01)) && retry )
    {
        udelay(10);
        retry--;
        goto recheck_detect;
    }

    if ( !retry )
    {
        ret = -1;
        goto apsd_fail;
    }

	retry = 30;

recheck_ubc:
	mdelay(30);

    ret = pmic_reg_read(p, NXE2000_REG_EXTIF_GCHGDET, &value);
    if (ret != 0)
    {
        if (retry)
        {
            retry--;
            goto recheck_ubc;
        }
        else
        {
            ret = -1;
            goto apsd_fail;
        }
    }

    tmp = ((value >> NXE2000_POS_GCHGDET_GC_DET) & NXE2000_GCHGDET_GC_DET_MASK);
    if ( (tmp != NXE2000_GCHGDET_GC_DET_COMPLETE) && retry )
    {
        retry--;
        goto recheck_ubc;
    }

	ret = (value >> NXE2000_POS_GCHGDET_VBUS_TYPE) & NXE2000_GCHGDET_VBUS_TYPE_MASK;

apsd_fail:
	return ret;
}

static int muic_chrg_get_type(struct pmic *p, u32 ctrl_en)
{
	int chg_ilim_uA;
	int type;
	u32 val = 0, tmp = 0;
	u32 chg_state;
	unsigned char charger;

	if (pmic_probe(p))
		return CHARGER_NO;

	pmic_reg_read(p, NXE2000_REG_CHGSTATE, &chg_state);
	if ((chg_state & 0xC0) == 0x00)
		return CHARGER_UNKNOWN;

#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP) || (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP_UBC)
#if (CONFIG_PMIC_NXE2000_CHARGING_PATH == CONFIG_PMIC_CHARGING_PATH_ADP_UBC)
	if (chg_state & 0x40)
#endif
	{
		val = (NXE2000_DEF_LIMIT_ADP_AMP / 100000) - 1;
		pmic_reg_write(p, NXE2000_REG_REGISET1, val);

		val = (CHARGER_CURRENT_COMPLETE << NXE2000_POS_CHGISET_ICCHG) + ((NXE2000_DEF_CHG_ADP_AMP / 100000) - 1);
		pmic_reg_write(p, NXE2000_REG_CHGISET, val);

#if defined(CONFIG_PMIC_VOLTAGE_CHECK_WITH_CHARGE)
		pmic_reg_read(p, NXE2000_REG_CHGCTL1, &val);
		val |= (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN);
		pmic_reg_write(p, NXE2000_REG_CHGCTL1, val);
#else
		if (ctrl_en)
		{
			pmic_reg_read(p, NXE2000_REG_CHGCTL1, &val);
			val |= (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN);
			pmic_reg_write(p, NXE2000_REG_CHGCTL1, val);
		}
		else
		{
			chgctl_reg_val |= (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN);
		}
#endif

		return CHARGER_TA;
	}
#endif

#if defined(CONFIG_PMIC_VOLTAGE_CHECK_WITH_CHARGE)
	pmic_reg_read(p, NXE2000_REG_CHGCTL1, &val);
	val |= (0x1 << NXE2000_POS_CHGCTL1_CHGP)
		|  (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
	pmic_reg_write(p, NXE2000_REG_CHGCTL1, val);
#else
	if (ctrl_en)
	{
		pmic_reg_read(p, NXE2000_REG_CHGCTL1, &val);
		val |= (0x1 << NXE2000_POS_CHGCTL1_CHGP)
			|  (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
		pmic_reg_write(p, NXE2000_REG_CHGCTL1, val);
	}
	else
	{
		chgctl_reg_val  |= (0x1 << NXE2000_POS_CHGCTL1_CHGP)
						|  (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
	}
#endif

	/* PMIC GPIO4 : Set output mode */
	pmic_reg_read(p, NXE2000_REG_IOSEL, &tmp);
	val = (tmp | 0x10) & 0xFF;
	pmic_reg_write(p, NXE2000_REG_IOSEL, val);

	/* PMIC GPIO4 : Set low level */
	pmic_reg_read(p, NXE2000_REG_IOOUT, &tmp);
	val = (tmp & ~0x10) & 0xFF;
	pmic_reg_write(p, NXE2000_REG_IOOUT, val);
	mdelay(10);

#if defined(CONFIG_OTG_PHY_NEXELL)
	otg_clk_disable();
	otg_phy_off();
#endif

	pmic_reg_read(p, NXE2000_REG_CHGSTATE, &chg_state);

	tmp = 0;

	if(chg_state & NXE2000_POS_CHGSTATE_PWRSRC_MASK)
	{
		type = nxe2000_apsd_detect(p);

		switch (type)
		{
		case NXE2000_GCHGDET_VBUS_TYPE_CDP:
		case NXE2000_GCHGDET_VBUS_TYPE_DCP:
			tmp     = (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN);
			charger = CHARGER_TA;
			break;
		case NXE2000_GCHGDET_VBUS_TYPE_SDP:
		default:
			tmp     = (0x1 << NXE2000_POS_CHGCTL1_CHGP)
					| (0x1 << NXE2000_POS_CHGCTL1_VUSBCHGEN);
			charger = CHARGER_USB;
			break;
		}

// DEVLOPER_MODE
#if defined(CONFIG_PMIC_NXE2000_ADP_CHARGER_ONLY_MODE)
		tmp     = (0x1 << NXE2000_POS_CHGCTL1_VADPCHGEN);
		charger = CHARGER_TA;
#endif
	}
	else
	{
		charger = CHARGER_NO;
	}

/* ------------------------- */
	switch (charger)
	{
	case CHARGER_TA:
		chg_ilim_uA = NXE2000_DEF_CHG_ADP_AMP;  // 800mA

		val = (NXE2000_DEF_LIMIT_ADP_AMP / 100000) - 1;
		pmic_reg_write(p, NXE2000_REG_REGISET1, val);
		break;
	default:
		chg_ilim_uA = NXE2000_DEF_CHG_USB_AMP;  // 500mA

		val = (NXE2000_DEF_LIMIT_USB_AMP / 100000) - 1;
		pmic_reg_write(p, NXE2000_REG_REGISET2, val);
		break;
	}

	val = (CHARGER_CURRENT_COMPLETE << NXE2000_POS_CHGISET_ICCHG) + ((chg_ilim_uA / 100000) - 1);
	pmic_reg_write(p, NXE2000_REG_CHGISET, val);
/* ------------------------- */

	pmic_reg_read(p, NXE2000_REG_CHGCTL1, &val);
	val = (val & ~NXE2000_CHG_MASK) | (tmp & NXE2000_CHG_MASK);

	if ( !tmp )
		val = 0;

	/* Set GPIO4 Condition */
	pmic_reg_read(p, NXE2000_REG_IOOUT, &tmp);
	val = (tmp | 0x10) & 0xFF;	// High(Hi-Z)
	pmic_reg_write(p, NXE2000_REG_IOOUT, val);

#if defined(CONFIG_OTG_PHY_NEXELL)
	otg_phy_init();
	otg_clk_enable();
#endif

	return charger;
}
#endif

static struct power_chrg power_chrg_muic_ops = {
	.chrg_type = muic_chrg_get_type,
};

int power_muic_init(unsigned int bus)
{
	static const char name[] = "MUIC_NXE2000";
	struct pmic *p = pmic_alloc();

#if defined(CONFIG_FASTBOOT) && defined(CONFIG_SW_UBC_DETECT)
	s_otg_bind_status   = 0;
	s_otg_bind_flag     = 0;
#endif

	if (!p) {
		printf("%s: POWER allocation error!\n", __func__);
		return -ENOMEM;
	}

	debug("Board Micro USB Interface Controller init\n");

	p->name = name;
	p->interface = PMIC_I2C;
	p->number_of_regs = NXE2000_NUM_OF_REGS;
	p->hw.i2c.addr = NXE2000_I2C_ADDR;
	p->hw.i2c.tx_num = 1;
	p->bus = bus;

	p->chrg = &power_chrg_muic_ops;
	return 0;
}
