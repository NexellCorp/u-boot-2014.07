/*
 * (C) Copyright 2009
 * jung hyun kim, Nexell Co, <jhkim@nexell.co.kr>
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

#if defined(CONFIG_PMIC_NXE2000)
#include <power/pmic.h>
#include <nxe2000-private.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#include "eth.c"

#if (0)
#define DBGOUT(msg...)		{ printf("BD: " msg); }
#else
#define DBGOUT(msg...)		do {} while (0)
#endif

#define FASTBOOT_SIGNATURE		0x46415354 /* (ASCII) : FAST  */

/*------------------------------------------------------------------------------
 * BUS Configure
 */
#if (CFG_BUS_RECONFIG_ENB == 1)
#include <asm/arch/s5p4418_bus.h>

const u8 g_DrexBRB_RD[2] = {
	0x1,            // Port0
	0xF             // Port1
};

const u8 g_DrexBRB_WR[2] = {
	0x1,            // Port0
	0xF             // Port1
};

const u16 g_DrexQoS[2] = {
	0x100,          // S0
	0xFFF           // S1, Default value
};


#if (CFG_BUS_RECONFIG_TOPBUSSI == 1)
const u8 g_TopBusSI[8] = {
	TOPBUS_SI_SLOT_DMAC0,
	TOPBUS_SI_SLOT_USBOTG,
	TOPBUS_SI_SLOT_USBHOST0,
	TOPBUS_SI_SLOT_DMAC1,
	TOPBUS_SI_SLOT_SDMMC,
	TOPBUS_SI_SLOT_USBOTG,
	TOPBUS_SI_SLOT_USBHOST1,
	TOPBUS_SI_SLOT_USBOTG
};
#endif

#if (CFG_BUS_RECONFIG_TOPBUSQOS == 1)
const u8 g_TopQoSSI[2] = {
	1,      // Tidemark
	(1<<TOPBUS_SI_SLOT_DMAC0) |     // Control
	(1<<TOPBUS_SI_SLOT_MP2TS) |
	(1<<TOPBUS_SI_SLOT_DMAC1) |
	(1<<TOPBUS_SI_SLOT_SDMMC) |
	(1<<TOPBUS_SI_SLOT_USBOTG) |
	(1<<TOPBUS_SI_SLOT_USBHOST0) |
	(1<<TOPBUS_SI_SLOT_USBHOST1)
};
#endif

#if (CFG_BUS_RECONFIG_BOTTOMBUSSI == 1)
const u8 g_BottomBusSI[8] = {
        BOTBUS_SI_SLOT_1ST_ARM,
        BOTBUS_SI_SLOT_MALI,
        BOTBUS_SI_SLOT_DEINTERLACE,
        BOTBUS_SI_SLOT_1ST_CODA,
        BOTBUS_SI_SLOT_2ND_ARM,
        BOTBUS_SI_SLOT_SCALER,
        BOTBUS_SI_SLOT_TOP,
        BOTBUS_SI_SLOT_2ND_CODA
};
#endif

#if (CFG_BUS_RECONFIG_BOTTOMBUSQOS == 1)
const u8 g_BottomQoSSI[2] = {
	1,      // Tidemark
	(1<<BOTBUS_SI_SLOT_1ST_ARM) |   // Control
		(1<<BOTBUS_SI_SLOT_2ND_ARM) |
		(1<<BOTBUS_SI_SLOT_MALI) |
		(1<<BOTBUS_SI_SLOT_TOP) |
		(1<<BOTBUS_SI_SLOT_DEINTERLACE) |
		(1<<BOTBUS_SI_SLOT_1ST_CODA)
};
#endif

#if (CFG_BUS_RECONFIG_DISPBUSSI == 1)
const u8 g_DispBusSI[3] = {
	DISBUS_SI_SLOT_1ST_DISPLAY,
	DISBUS_SI_SLOT_2ND_DISPLAY,
	DISBUS_SI_SLOT_2ND_DISPLAY	//DISBUS_SI_SLOT_GMAC
};
#endif
#endif	/* #if (CFG_BUS_RECONFIG_ENB == 1) */


/*------------------------------------------------------------------------------
 * intialize nexell soc and board status.
 */
static void set_gpio_strenth(U32 Group, U32 BitNumber, U32 mA)
{
	U32 drv1=0, drv0=0;
	U32 drv1_value, drv0_value;

	switch( mA )
	{
		case 0 : drv0 = 0; drv1 = 0; break;
		case 1 : drv0 = 0; drv1 = 1; break;
		case 2 : drv0 = 1; drv1 = 0; break;
		case 3 : drv0 = 1; drv1 = 1; break;
		default: drv0 = 0; drv1 = 0; break;
	}
	DBGOUT("DRV Strength : GRP : i %x Bit: %x  ma :%d  \n",
				Group, BitNumber, mA);

	drv1_value = NX_GPIO_GetDRV1(Group) & ~(1 << BitNumber);
	drv0_value = NX_GPIO_GetDRV0(Group) & ~(1 << BitNumber);

	if (drv1) drv1_value |= (drv1 << BitNumber);
	if (drv0) drv0_value |= (drv0 << BitNumber);

	DBGOUT(" Value : drv1 :%8x  drv0 %8x \n ",drv1_value, drv0_value);

	NX_GPIO_SetDRV0 ( Group, drv0_value );
	NX_GPIO_SetDRV1 ( Group, drv1_value );
}

static void bd_gpio_init(void)
{
	int index, bit;
	int mode, func, out, lv, plup, stren;
	U32 gpio;

	const U32 pads[NUMBER_OF_GPIO_MODULE][32] = {
	{	/* GPIO_A */
	PAD_GPIOA0 , PAD_GPIOA1 , PAD_GPIOA2 , PAD_GPIOA3 , PAD_GPIOA4 , PAD_GPIOA5 , PAD_GPIOA6 , PAD_GPIOA7 , PAD_GPIOA8 , PAD_GPIOA9 ,
	PAD_GPIOA10, PAD_GPIOA11, PAD_GPIOA12, PAD_GPIOA13, PAD_GPIOA14, PAD_GPIOA15, PAD_GPIOA16, PAD_GPIOA17, PAD_GPIOA18, PAD_GPIOA19,
	PAD_GPIOA20, PAD_GPIOA21, PAD_GPIOA22, PAD_GPIOA23, PAD_GPIOA24, PAD_GPIOA25, PAD_GPIOA26, PAD_GPIOA27, PAD_GPIOA28, PAD_GPIOA29,
	PAD_GPIOA30, PAD_GPIOA31
	}, { /* GPIO_B */
	PAD_GPIOB0 , PAD_GPIOB1 , PAD_GPIOB2 , PAD_GPIOB3 , PAD_GPIOB4 , PAD_GPIOB5 , PAD_GPIOB6 , PAD_GPIOB7 , PAD_GPIOB8 , PAD_GPIOB9 ,
	PAD_GPIOB10, PAD_GPIOB11, PAD_GPIOB12, PAD_GPIOB13, PAD_GPIOB14, PAD_GPIOB15, PAD_GPIOB16, PAD_GPIOB17, PAD_GPIOB18, PAD_GPIOB19,
	PAD_GPIOB20, PAD_GPIOB21, PAD_GPIOB22, PAD_GPIOB23, PAD_GPIOB24, PAD_GPIOB25, PAD_GPIOB26, PAD_GPIOB27, PAD_GPIOB28, PAD_GPIOB29,
	PAD_GPIOB30, PAD_GPIOB31
	}, { /* GPIO_C */
	PAD_GPIOC0 , PAD_GPIOC1 , PAD_GPIOC2 , PAD_GPIOC3 , PAD_GPIOC4 , PAD_GPIOC5 , PAD_GPIOC6 , PAD_GPIOC7 , PAD_GPIOC8 , PAD_GPIOC9 ,
	PAD_GPIOC10, PAD_GPIOC11, PAD_GPIOC12, PAD_GPIOC13, PAD_GPIOC14, PAD_GPIOC15, PAD_GPIOC16, PAD_GPIOC17, PAD_GPIOC18, PAD_GPIOC19,
	PAD_GPIOC20, PAD_GPIOC21, PAD_GPIOC22, PAD_GPIOC23, PAD_GPIOC24, PAD_GPIOC25, PAD_GPIOC26, PAD_GPIOC27, PAD_GPIOC28, PAD_GPIOC29,
	PAD_GPIOC30, PAD_GPIOC31
	}, { /* GPIO_D */
	PAD_GPIOD0 , PAD_GPIOD1 , PAD_GPIOD2 , PAD_GPIOD3 , PAD_GPIOD4 , PAD_GPIOD5 , PAD_GPIOD6 , PAD_GPIOD7 , PAD_GPIOD8 , PAD_GPIOD9 ,
	PAD_GPIOD10, PAD_GPIOD11, PAD_GPIOD12, PAD_GPIOD13, PAD_GPIOD14, PAD_GPIOD15, PAD_GPIOD16, PAD_GPIOD17, PAD_GPIOD18, PAD_GPIOD19,
	PAD_GPIOD20, PAD_GPIOD21, PAD_GPIOD22, PAD_GPIOD23, PAD_GPIOD24, PAD_GPIOD25, PAD_GPIOD26, PAD_GPIOD27, PAD_GPIOD28, PAD_GPIOD29,
	PAD_GPIOD30, PAD_GPIOD31
	}, { /* GPIO_E */
	PAD_GPIOE0 , PAD_GPIOE1 , PAD_GPIOE2 , PAD_GPIOE3 , PAD_GPIOE4 , PAD_GPIOE5 , PAD_GPIOE6 , PAD_GPIOE7 , PAD_GPIOE8 , PAD_GPIOE9 ,
	PAD_GPIOE10, PAD_GPIOE11, PAD_GPIOE12, PAD_GPIOE13, PAD_GPIOE14, PAD_GPIOE15, PAD_GPIOE16, PAD_GPIOE17, PAD_GPIOE18, PAD_GPIOE19,
	PAD_GPIOE20, PAD_GPIOE21, PAD_GPIOE22, PAD_GPIOE23, PAD_GPIOE24, PAD_GPIOE25, PAD_GPIOE26, PAD_GPIOE27, PAD_GPIOE28, PAD_GPIOE29,
	PAD_GPIOE30, PAD_GPIOE31
	},
	};

	/* GPIO pad function */
	for (index = 0; NUMBER_OF_GPIO_MODULE > index; index++) {

		NX_GPIO_ClearInterruptPendingAll(index);

		for (bit = 0; 32 > bit; bit++) {
			gpio  = pads[index][bit];
			func  = PAD_GET_FUNC(gpio);
			mode  = PAD_GET_MODE(gpio);
			lv    = PAD_GET_LEVEL(gpio);
			stren = PAD_GET_STRENGTH(gpio);
			plup  = PAD_GET_PULLUP(gpio);

			/* get pad alternate function (0,1,2,4) */
			switch (func) {
			case PAD_GET_FUNC(PAD_FUNC_ALT0): func = NX_GPIO_PADFUNC_0;	break;
			case PAD_GET_FUNC(PAD_FUNC_ALT1): func = NX_GPIO_PADFUNC_1;	break;
			case PAD_GET_FUNC(PAD_FUNC_ALT2): func = NX_GPIO_PADFUNC_2;	break;
			case PAD_GET_FUNC(PAD_FUNC_ALT3): func = NX_GPIO_PADFUNC_3;	break;
			default: printf("ERROR, unknown alt func (%d.%02d=%d)\n", index, bit, func);
				continue;
			}

			switch (mode) {
			case PAD_GET_MODE(PAD_MODE_ALT): out = 0;
			case PAD_GET_MODE(PAD_MODE_IN ): out = 0;
			case PAD_GET_MODE(PAD_MODE_INT): out = 0; break;
			case PAD_GET_MODE(PAD_MODE_OUT): out = 1; break;
			default: printf("ERROR, unknown io mode (%d.%02d=%d)\n", index, bit, mode);
				continue;
			}

			NX_GPIO_SetPadFunction(index, bit, func);
			NX_GPIO_SetOutputEnable(index, bit, (out ? CTRUE : CFALSE));
			NX_GPIO_SetOutputValue(index, bit,  (lv  ? CTRUE : CFALSE));
			NX_GPIO_SetInterruptMode(index, bit, (lv));

			NX_GPIO_SetPullMode(index, bit, plup);
			set_gpio_strenth(index, bit, stren); /* pad strength */
		}

		NX_GPIO_SetDRV0_DISABLE_DEFAULT(index, 0xFFFFFFFF);
		NX_GPIO_SetDRV1_DISABLE_DEFAULT(index, 0xFFFFFFFF);
	}
}

static void bd_alive_init(void)
{
	int index, bit;
	int mode, out, lv, plup, detect;
	U32 gpio;

	const U32 pads[] = {
	PAD_GPIOALV0, PAD_GPIOALV1, PAD_GPIOALV2,
	PAD_GPIOALV3, PAD_GPIOALV4, PAD_GPIOALV5
	};

	index = sizeof(pads)/sizeof(pads[0]);

	/* Alive pad function */
	for (bit = 0; index > bit; bit++) {
		NX_ALIVE_ClearInterruptPending(bit);
		gpio = pads[bit];
		mode = PAD_GET_MODE(gpio);
		lv   = PAD_GET_LEVEL(gpio);
		plup = PAD_GET_PULLUP(gpio);

		switch (mode) {
		case PAD_GET_MODE(PAD_MODE_IN ):
		case PAD_GET_MODE(PAD_MODE_INT): out = 0; break;
		case PAD_GET_MODE(PAD_MODE_OUT): out = 1; break;
		case PAD_GET_MODE(PAD_MODE_ALT):
			printf("ERROR, alive.%d not support alt function\n", bit);
			continue;
		default :
			printf("ERROR, unknown alive mode (%d=%d)\n", bit, mode);
			continue;
		}

		NX_ALIVE_SetOutputEnable(bit, (out ? CTRUE : CFALSE));
		NX_ALIVE_SetOutputValue (bit, (lv));
		NX_ALIVE_SetPullUpEnable(bit, (plup & 1 ? CTRUE : CFALSE));
		/* set interrupt mode */
		for (detect = 0; 6 > detect; detect++) {
			if (mode == PAD_GET_MODE(PAD_MODE_INT))
				NX_ALIVE_SetDetectMode(detect, bit, (lv == detect ? CTRUE : CFALSE));
			else
				NX_ALIVE_SetDetectMode(detect, bit, CFALSE);
		}
		NX_ALIVE_SetDetectEnable(bit, (mode == PAD_MODE_INT ? CTRUE : CFALSE));
	}
}

/* call from u-boot */
int board_early_init_f(void)
{
	bd_gpio_init();
	bd_alive_init();
#if (defined(CONFIG_PMIC_NXE2000)||defined(CONFIG_PMIC_AXP228))&& !defined(CONFIG_PMIC_REG_DUMP)
	bd_pmic_init();
#endif
#if defined(CONFIG_NXP_RTC_USE)
	nxp_rtc_init();
#endif
	return 0;
}

int board_init(void)
{
	DBGOUT("%s : done board init ...\n", CFG_SYS_BOARD_NAME);
	return 0;
}

#if defined(CONFIG_PMIC_NXE2000)||defined(CONFIG_PMIC_AXP228)
int power_init_board(void)
{
	int ret = 0;
#if defined(CONFIG_PMIC_REG_DUMP)
	bd_pmic_init();
#endif
	ret = power_pmic_function_init();
	return ret;
}
#endif

extern void	bd_display(void);

static void auto_update(int io, int wait)
{
	unsigned int grp = PAD_GET_GROUP(io);
	unsigned int bit = PAD_GET_BITNO(io);
	int level = 1, i = 0;
	char *cmd = "fastboot";

	for (i = 0; wait > i; i++) {
		switch (io & ~(32-1)) {
		case PAD_GPIO_A:
		case PAD_GPIO_B:
		case PAD_GPIO_C:
		case PAD_GPIO_D:
		case PAD_GPIO_E:
			level = NX_GPIO_GetInputValue(grp, bit);	break;
		case PAD_GPIO_ALV:
			level = NX_ALIVE_GetInputValue(bit);	break;
		};
		if (level)
			break;
		mdelay(1);
	}

	if (i == wait)
		run_command (cmd, 0);
}

struct init_reg_val {
	u16 addr;
	u8 val;
};
static struct init_reg_val mj40150n_init_data[] = {
	{0xF000, 0x55},
	{0xF001, 0xAA},
	{0xF002, 0x52},
	{0xF003, 0x08},
	{0xF004, 0x02},
	{0xF600, 0x60},
	{0xF601, 0x40},
	{0xFE00, 0x01},
	{0xFE01, 0x80},
	{0xFE02, 0x09},
	{0xFE03, 0x09},
	{0xF000, 0x55},
	{0xF001, 0xAA},
	{0xF002, 0x52},
	{0xF003, 0x08},
	{0xF004, 0x01},
	{0xB000, 0x0F},
	{0xB001, 0x0F},
	{0xB002, 0x0F},
	{0xB100, 0x08},
	{0xB101, 0x08},
	{0xB102, 0x08},
	{0xB500, 0x02},
	{0xB501, 0x02},
	{0xB502, 0x02},
	{0xB600, 0x34},
	{0xB601, 0x34},
	{0xB602, 0x34},
	{0xB700, 0x44},
	{0xB701, 0x44},
	{0xB702, 0x44},
	{0xB800, 0x24},
	{0xB801, 0x24},
	{0xB802, 0x24},
	{0xB900, 0x34},
	{0xB901, 0x34},
	{0xB902, 0x34},
	{0xBA00, 0x04},
	{0xBA01, 0x04},
	{0xBA02, 0x04},
	{0xBC00, 0x00},
	{0xBC01, 0x78},
	{0xBC02, 0x00},
	{0xBD00, 0x00},
	{0xBD01, 0x78},
	{0xBD02, 0x00},
	{0xBE00, 0x00},
	{0xBE01, 0x69},
	{0xD100, 0x00},
	{0xD101, 0x00},
	{0xD102, 0x00},
	{0xD103, 0x02},
	{0xD104, 0x00},
	{0xD105, 0x0B},
	{0xD106, 0x00},
	{0xD107, 0x1D},
	{0xD108, 0x00},
	{0xD109, 0x34},
	{0xD10A, 0x00},
	{0xD10B, 0x6B},
	{0xD10C, 0x00},
	{0xD10D, 0x9E},
	{0xD10E, 0x00},
	{0xD10F, 0xEA},
	{0xD110, 0x01},
	{0xD111, 0x1E},
	{0xD112, 0x01},
	{0xD113, 0x63},
	{0xD114, 0x01},
	{0xD115, 0x92},
	{0xD116, 0x01},
	{0xD117, 0xD2},
	{0xD118, 0x02},
	{0xD119, 0x02},
	{0xD11A, 0x02},
	{0xD11B, 0x04},
	{0xD11C, 0x02},
	{0xD11D, 0x2E},
	{0xD11E, 0x02},
	{0xD11F, 0x57},
	{0xD120, 0x02},
	{0xD121, 0x6E},
	{0xD122, 0x02},
	{0xD123, 0x89},
	{0xD124, 0x02},
	{0xD125, 0x9A},
	{0xD126, 0x02},
	{0xD127, 0xAF},
	{0xD128, 0x02},
	{0xD129, 0xBD},
	{0xD12A, 0x02},
	{0xD12B, 0xD0},
	{0xD12C, 0x02},
	{0xD12D, 0xDD},
	{0xD12E, 0x02},
	{0xD12F, 0xF2},
	{0xD130, 0x03},
	{0xD131, 0x22},
	{0xD132, 0x03},
	{0xD133, 0xFB},
	{0xD200, 0x00},
	{0xD201, 0x00},
	{0xD202, 0x00},
	{0xD203, 0x02},
	{0xD204, 0x00},
	{0xD205, 0x0B},
	{0xD206, 0x00},
	{0xD207, 0x1D},
	{0xD208, 0x00},
	{0xD209, 0x34},
	{0xD20A, 0x00},
	{0xD20B, 0x6B},
	{0xD20C, 0x00},
	{0xD20D, 0x9E},
	{0xD20E, 0x00},
	{0xD20F, 0xEA},
	{0xD210, 0x01},
	{0xD211, 0x1E},
	{0xD212, 0x01},
	{0xD213, 0x63},
	{0xD214, 0x01},
	{0xD215, 0x92},
	{0xD216, 0x01},
	{0xD217, 0xD2},
	{0xD218, 0x02},
	{0xD219, 0x02},
	{0xD21A, 0x02},
	{0xD21B, 0x04},
	{0xD21C, 0x02},
	{0xD21D, 0x2E},
	{0xD21E, 0x02},
	{0xD21F, 0x57},
	{0xD220, 0x02},
	{0xD221, 0x6E},
	{0xD222, 0x02},
	{0xD223, 0x89},
	{0xD224, 0x02},
	{0xD225, 0x9A},
	{0xD226, 0x02},
	{0xD227, 0xAF},
	{0xD228, 0x02},
	{0xD229, 0xBD},
	{0xD22A, 0x02},
	{0xD22B, 0xD0},
	{0xD22C, 0x02},
	{0xD22D, 0xDD},
	{0xD22E, 0x02},
	{0xD22F, 0xF2},
	{0xD230, 0x03},
	{0xD231, 0x22},
	{0xD232, 0x03},
	{0xD233, 0xFB},
	{0xD300, 0x00},
	{0xD301, 0x00},
	{0xD302, 0x00},
	{0xD303, 0x02},
	{0xD304, 0x00},
	{0xD305, 0x0B},
	{0xD306, 0x00},
	{0xD307, 0x1D},
	{0xD308, 0x00},
	{0xD309, 0x34},
	{0xD30A, 0x00},
	{0xD30B, 0x6B},
	{0xD30C, 0x00},
	{0xD30D, 0x9E},
	{0xD30E, 0x00},
	{0xD30F, 0xEA},
	{0xD310, 0x01},
	{0xD311, 0x1E},
	{0xD312, 0x01},
	{0xD313, 0x63},
	{0xD314, 0x01},
	{0xD315, 0x92},
	{0xD316, 0x01},
	{0xD317, 0xD2},
	{0xD318, 0x02},
	{0xD319, 0x02},
	{0xD31A, 0x02},
	{0xD31B, 0x04},
	{0xD31C, 0x02},
	{0xD31D, 0x2E},
	{0xD31E, 0x02},
	{0xD31F, 0x57},
	{0xD320, 0x02},
	{0xD321, 0x6E},
	{0xD322, 0x02},
	{0xD323, 0x89},
	{0xD324, 0x02},
	{0xD325, 0x9A},
	{0xD326, 0x02},
	{0xD327, 0xAF},
	{0xD328, 0x02},
	{0xD329, 0xBD},
	{0xD32A, 0x02},
	{0xD32B, 0xD0},
	{0xD32C, 0x02},
	{0xD32D, 0xDD},
	{0xD32E, 0x02},
	{0xD32F, 0xF2},
	{0xD330, 0x03},
	{0xD331, 0x22},
	{0xD332, 0x03},
	{0xD333, 0xFB},
	{0xD400, 0x00},
	{0xD401, 0x00},
	{0xD402, 0x00},
	{0xD403, 0x02},
	{0xD404, 0x00},
	{0xD405, 0x0B},
	{0xD406, 0x00},
	{0xD407, 0x1D},
	{0xD408, 0x00},
	{0xD409, 0x34},
	{0xD40A, 0x00},
	{0xD40B, 0x6B},
	{0xD40C, 0x00},
	{0xD40D, 0x9E},
	{0xD40E, 0x00},
	{0xD40F, 0xEA},
	{0xD410, 0x01},
	{0xD411, 0x1E},
	{0xD412, 0x01},
	{0xD413, 0x63},
	{0xD414, 0x01},
	{0xD415, 0x92},
	{0xD416, 0x01},
	{0xD417, 0xD2},
	{0xD418, 0x02},
	{0xD419, 0x02},
	{0xD41A, 0x02},
	{0xD41B, 0x04},
	{0xD41C, 0x02},
	{0xD41D, 0x2E},
	{0xD41E, 0x02},
	{0xD41F, 0x57},
	{0xD420, 0x02},
	{0xD421, 0x6E},
	{0xD422, 0x02},
	{0xD423, 0x89},
	{0xD424, 0x02},
	{0xD425, 0x9A},
	{0xD426, 0x02},
	{0xD427, 0xAF},
	{0xD428, 0x02},
	{0xD429, 0xBD},
	{0xD42A, 0x02},
	{0xD42B, 0xD0},
	{0xD42C, 0x02},
	{0xD42D, 0xDD},
	{0xD42E, 0x02},
	{0xD42F, 0xF2},
	{0xD430, 0x03},
	{0xD431, 0x22},
	{0xD432, 0x03},
	{0xD433, 0xFB},
	{0xD500, 0x00},
	{0xD501, 0x00},
	{0xD502, 0x00},
	{0xD503, 0x02},
	{0xD504, 0x00},
	{0xD505, 0x0B},
	{0xD506, 0x00},
	{0xD507, 0x1D},
	{0xD508, 0x00},
	{0xD509, 0x34},
	{0xD50A, 0x00},
	{0xD50B, 0x6B},
	{0xD50C, 0x00},
	{0xD50D, 0x9E},
	{0xD50E, 0x00},
	{0xD50F, 0xEA},
	{0xD510, 0x01},
	{0xD511, 0x1E},
	{0xD512, 0x01},
	{0xD513, 0x63},
	{0xD514, 0x01},
	{0xD515, 0x92},
	{0xD516, 0x01},
	{0xD517, 0xD2},
	{0xD518, 0x02},
	{0xD519, 0x02},
	{0xD51A, 0x02},
	{0xD51B, 0x04},
	{0xD51C, 0x02},
	{0xD51D, 0x2E},
	{0xD51E, 0x02},
	{0xD51F, 0x57},
	{0xD520, 0x02},
	{0xD521, 0x6E},
	{0xD522, 0x02},
	{0xD523, 0x89},
	{0xD524, 0x02},
	{0xD525, 0x9A},
	{0xD526, 0x02},
	{0xD527, 0xAF},
	{0xD528, 0x02},
	{0xD529, 0xBD},
	{0xD52A, 0x02},
	{0xD52B, 0xD0},
	{0xD52C, 0x02},
	{0xD52D, 0xDD},
	{0xD52E, 0x02},
	{0xD52F, 0xF2},
	{0xD530, 0x03},
	{0xD531, 0x22},
	{0xD532, 0x03},
	{0xD533, 0xFB},
	{0xD600, 0x00},
	{0xD601, 0x00},
	{0xD602, 0x00},
	{0xD603, 0x02},
	{0xD604, 0x00},
	{0xD605, 0x0B},
	{0xD606, 0x00},
	{0xD607, 0x1D},
	{0xD608, 0x00},
	{0xD609, 0x34},
	{0xD60A, 0x00},
	{0xD60B, 0x6B},
	{0xD60C, 0x00},
	{0xD60D, 0x9E},
	{0xD60E, 0x00},
	{0xD60F, 0xEA},
	{0xD610, 0x01},
	{0xD611, 0x1E},
	{0xD612, 0x01},
	{0xD613, 0x63},
	{0xD614, 0x01},
	{0xD615, 0x92},
	{0xD616, 0x01},
	{0xD617, 0xD2},
	{0xD618, 0x02},
	{0xD619, 0x02},
	{0xD61A, 0x02},
	{0xD61B, 0x04},
	{0xD61C, 0x02},
	{0xD61D, 0x2E},
	{0xD61E, 0x02},
	{0xD61F, 0x57},
	{0xD620, 0x02},
	{0xD621, 0x6E},
	{0xD622, 0x02},
	{0xD623, 0x89},
	{0xD624, 0x02},
	{0xD625, 0x9A},
	{0xD626, 0x02},
	{0xD627, 0xAF},
	{0xD628, 0x02},
	{0xD629, 0xBD},
	{0xD62A, 0x02},
	{0xD62B, 0xD0},
	{0xD62C, 0x02},
	{0xD62D, 0xDD},
	{0xD62E, 0x02},
	{0xD62F, 0xF2},
	{0xD630, 0x03},
	{0xD631, 0x22},
	{0xD632, 0x03},
	{0xD633, 0xFB},
	{0xF000, 0x55},
	{0xF001, 0xAA},
	{0xF002, 0x52},
	{0xF003, 0x08},
	{0xF004, 0x03},
	{0xB000, 0x03},
	{0xB001, 0x17},
	{0xB002, 0xF9},
	{0xB003, 0x24},
	{0xB004, 0x53},
	{0xB005, 0x00},
	{0xB006, 0x30},
	{0xB200, 0xFB},
	{0xB201, 0xFC},
	{0xB202, 0xFD},
	{0xB203, 0xFE},
	{0xB204, 0xF0},
	{0xB205, 0x53},
	{0xB206, 0x00},
	{0xB207, 0xC3},
	{0xB208, 0x08},
	{0xB300, 0x5B},
	{0xB301, 0x00},
	{0xB302, 0xFB},
	{0xB303, 0x20},
	{0xB304, 0x22},
	{0xB305, 0x0C},
	{0xB400, 0xFF},
	{0xB401, 0x00},
	{0xB402, 0x01},
	{0xB403, 0x02},
	{0xB404, 0xC0},
	{0xB405, 0x40},
	{0xB406, 0x03},
	{0xB407, 0x08},
	{0xB408, 0x53},
	{0xB500, 0x00},
	{0xB501, 0x00},
	{0xB502, 0xFF},
	{0xB503, 0x83},
	{0xB504, 0x22},
	{0xB505, 0x24},
	{0xB506, 0x00},
	{0xB507, 0x00},
	{0xB508, 0x33},
	{0xB509, 0x00},
	{0xB50A, 0x44},
	{0xB600, 0x80},
	{0xB601, 0x00},
	{0xB602, 0x00},
	{0xB603, 0x00},
	{0xB604, 0x96},
	{0xB605, 0x81},
	{0xB606, 0x00},
	{0xB700, 0x00},
	{0xB701, 0x00},
	{0xB702, 0x16},
	{0xB703, 0x16},
	{0xB704, 0x16},
	{0xB705, 0x16},
	{0xB706, 0x00},
	{0xB707, 0x00},
	{0xB800, 0xD2},
	{0xB801, 0x00},
	{0xB802, 0x00},
	{0xB900, 0x90},
	{0xBA00, 0x6F},
	{0xBA01, 0xDB},
	{0xBA02, 0x96},
	{0xBA03, 0x16},
	{0xBA04, 0x26},
	{0xBA05, 0x36},
	{0xBA06, 0xFF},
	{0xBA07, 0xFF},
	{0xBA08, 0xFF},
	{0xBA09, 0xFF},
	{0xBA0A, 0xF0},
	{0xBA0B, 0xF1},
	{0xBA0C, 0xF0},
	{0xBA0D, 0xF8},
	{0xBA0E, 0xAC},
	{0xBA0F, 0xEF},
	{0xBB00, 0xF8},
	{0xBB01, 0xAC},
	{0xBB02, 0xEF},
	{0xBB03, 0xF0},
	{0xBB04, 0x2F},
	{0xBB05, 0x3F},
	{0xBB06, 0xFF},
	{0xBB07, 0xFF},
	{0xBB08, 0xFF},
	{0xBB09, 0xFF},
	{0xBB0A, 0xF0},
	{0xBB0B, 0xF1},
	{0xBB0C, 0x1F},
	{0xBB0D, 0xFF},
	{0xBB0E, 0xDB},
	{0xBB0F, 0x9F},
	{0xBC00, 0x85},
	{0xBC01, 0xFF},
	{0xBC02, 0xFF},
	{0xBC03, 0xA1},
	{0xBD00, 0x86},
	{0xBD01, 0xFF},
	{0xBD02, 0xFF},
	{0xBD03, 0x61},
	{0xC000, 0x00},
	{0xC001, 0x01},
	{0xC002, 0xFA},
	{0xC003, 0x00},
	{0xF000, 0x55},
	{0xF001, 0xAA},
	{0xF002, 0x52},
	{0xF003, 0x08},
	{0xF004, 0x00},
	{0xB000, 0x00},
	{0xB001, 0x10},
	{0xB100, 0xFC},
	{0xBA00, 0x01},
	{0xB400, 0x10},
	{0xBC00, 0x02},
	{0xBC01, 0x02},
	{0xBC02, 0x02},
	{0x3500, 0x00},
	{0x3A00, 0x77},// {0x3A00, 0x55},
	{0x3600, 0x08},
};

extern ssize_t spi_master_write (U16 u16_Addr, U8 data, U32 size);
extern void spi_master_init(int Index, int Slave);

void MJ40150N_lcd_on(void)
{
	ssize_t ret;
	int i, init_size;

	spi_master_init(0, 0);

	ret = spi_master_write(0x1100, 0x00, 0);
	mdelay(100);

	ret = spi_master_write(0x2900, 0x00, 0);
	mdelay(20);
	return;
}


void MJ40150N_lcd_init(void)
{
	ssize_t ret;
	int i, init_size;
	int grp = PAD_GET_GROUP(CFG_IO_LCD_RST_ENB);
	int bit = PAD_GET_BITNO(CFG_IO_LCD_RST_ENB);
	int wait;
	CBOOL Level = CTRUE;

	NX_GPIO_SetOutputValue(grp, bit, CTRUE);
	mdelay(10);

	spi_master_init(0, 0);
	init_size = sizeof(mj40150n_init_data)/sizeof(mj40150n_init_data[0]);

	for (i=0; i<init_size; i++)
		ret = spi_master_write(mj40150n_init_data[i].addr, mj40150n_init_data[i].val, 1);

	ret = spi_master_write(0x1100, 0x00, 0);
	mdelay(200);

	ret = spi_master_write(0x2900, 0x00, 0);
	mdelay(20);

	return;
}

void bd_display_run(char *cmd, int bl_duty, int bl_on)
{
	static int display_init = 0;

	if (cmd) {
		/* clear FB */
		memset((void*)CONFIG_FB_ADDR, 0x00,
			CFG_DISP_PRI_RESOL_WIDTH * CFG_DISP_PRI_RESOL_HEIGHT * CFG_DISP_PRI_SCREEN_PIXEL_BYTE);
		run_command(cmd, 0);
		lcd_draw_boot_logo(CONFIG_FB_ADDR, CFG_DISP_PRI_RESOL_WIDTH,
			CFG_DISP_PRI_RESOL_HEIGHT, CFG_DISP_PRI_SCREEN_PIXEL_BYTE);
	}

	if (!display_init) {
		bd_display();
		MJ40150N_lcd_init();
		pwm_init(CFG_LCD_PRI_PWM_CH, 0, 0);
		display_init = 1;
	}

	pwm_config(CFG_LCD_PRI_PWM_CH,
		TO_DUTY_NS(bl_duty, CFG_LCD_PRI_PWM_FREQ),
		TO_PERIOD_NS(CFG_LCD_PRI_PWM_FREQ));

	if (bl_on)
		pwm_enable(CFG_LCD_PRI_PWM_CH);
}

#define	UPDATE_KEY			(PAD_GPIO_ALV + 0)
#define	UPDATE_CHECK_TIME	(3000)	/* ms */

int board_late_init(void)
{
#if defined(CONFIG_SYS_MMC_BOOT_DEV)
	char boot[16];
	sprintf(boot, "mmc dev %d", CONFIG_SYS_MMC_BOOT_DEV);
	run_command(boot, 0);
#endif

#if 1 // reboot bootloader -> fastboot(download)
	if (FASTBOOT_SIGNATURE == readl(SCR_USER_SIG6_READ)) {
		writel((-1UL), SCR_USER_SIG6_RESET); /* clear */
		printf("\nuser reset : fastboot\n");
		run_command ("fastboot", 0);	/* fastboot */
		writel((-1UL), SCR_USER_SIG6_RESET);
		return 0;
	}
	writel((-1UL), SCR_USER_SIG6_RESET);
#endif

#if defined CONFIG_RECOVERY_BOOT
    if (RECOVERY_SIGNATURE == readl(SCR_RESET_SIG_READ)) {
        writel((-1UL), SCR_RESET_SIG_RESET); /* clear */

        printf("RECOVERY BOOT\n");
        bd_display_run(CONFIG_CMD_LOGO_WALLPAPERS, CFG_LCD_PRI_PWM_DUTYCYCLE, 1);
        run_command(CONFIG_CMD_RECOVERY_BOOT, 0);	/* recovery boot */
    }

#endif /* CONFIG_RECOVERY_BOOT */

#if defined CONFIG_UPDATE_BOOT
    if (UPDATE_SIGNATURE == readl(SCR_RESET_SIG_READ)) {
        writel((-1UL), SCR_RESET_SIG_RESET); /* clear */

        printf("UPDATE BOOT\n");
        bd_display_run(CONFIG_CMD_LOGO_WALLPAPERS, CFG_LCD_PRI_PWM_DUTYCYCLE, 1);
        run_command(CONFIG_CMD_UPDATE_BOOT, 0);        /* recovery boot */
    }
#endif /* CONFIG_RECOVERY_BOOT */

    writel((-1UL), SCR_RESET_SIG_RESET);
#if defined(CONFIG_BAT_CHECK)
	{
		int ret =0;
		int bat_check_skip = 0;
	    // psw0523 for cts
	    // bat_check_skip = 1;

#if defined(CONFIG_DISPLAY_OUT)
		ret = power_battery_check(bat_check_skip, bd_display_run);
#else
		ret = power_battery_check(bat_check_skip, NULL);
#endif
		if(ret == 1)
			auto_update(UPDATE_KEY, UPDATE_CHECK_TIME);
	}
#else /* CONFIG_BAT_CHECK */

#if defined(CONFIG_DISPLAY_OUT)
	bd_display_run(CONFIG_CMD_LOGO_WALLPAPERS, CFG_LCD_PRI_PWM_DUTYCYCLE, 1);
#endif

	/* Temp check gpio to update */
	auto_update(UPDATE_KEY, UPDATE_CHECK_TIME);

#endif /* CONFIG_BAT_CHECK */

#ifdef CONFIG_SYS_BURNING
    run_command("fastboot nexell", 0);
#endif
	return 0;
}

