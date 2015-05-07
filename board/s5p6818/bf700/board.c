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

#if defined(CONFIG_REGULATOR_MP8845C)
#include <mp8845c_regulator.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#if (0)
#define DBGOUT(msg...)		{ printf("BD: " msg); }
#else
#define DBGOUT(msg...)		do {} while (0)
#endif

/*------------------------------------------------------------------------------
 * intialize nexell soc and board status.
 */
static inline void fdone_lcd_power(void)
{
	/* must be set after gpio init */	
#if 1
	/* Reset LCD_PWR_EN : default H */	
	mdelay(100);
	NX_GPIO_SetOutputValue(PAD_GET_GROUP(CFG_IO_LCD_PWR_ENB), PAD_GET_BITNO(CFG_IO_LCD_PWR_ENB), CFALSE);	
	mdelay(200);
	NX_GPIO_SetOutputValue(PAD_GET_GROUP(CFG_IO_LCD_PWR_ENB), PAD_GET_BITNO(CFG_IO_LCD_PWR_ENB), CTRUE);	
#else
    NX_GPIO_SetOutputValue(PAD_GET_GROUP(CFG_IO_LCD_PWR_ENB), PAD_GET_BITNO(CFG_IO_LCD_PWR_ENB), CTRUE);        
	NX_GPIO_SetOutputValue(PAD_GET_GROUP(CFG_IO_LCD_PWR_ENV), PAD_GET_BITNO(CFG_IO_LCD_PWR_ENB), CFALSE);
	mdelay(100);
	NX_GPIO_SetOutputValue(PAD_GET_GROUP(CFG_IO_LCD_PWR_ENV), PAD_GET_BITNO(CFG_IO_LCD_PWR_ENB), CTRUE);
	mdelay(700);
	NX_GPIO_SetOutputValue(PAD_GET_GROUP(CFG_IO_LCD_GD_PWR_EN), PAD_GET_BITNO(CFG_IO_LCD_GD_PWR_EN), CTRUE);
#endif

	/* 
	 * Set LCD_BL_EN : default L -> H 
	 * move to kernel part 
	 */	
//	mdelay(10);
//	NX_GPIO_SetOutputValue(PAD_GET_GROUP(CFG_IO_LCD_BL_ENB), PAD_GET_BITNO(CFG_IO_LCD_BL_ENB), CTRUE);	
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

			NX_GPIO_SetPullEnable(index, bit, (NX_GPIO_PULL)plup );
			NX_GPIO_SetDriveStrength(index, bit, (NX_GPIO_DRVSTRENGTH)stren); /* pad strength */
		}
	}

	/* Fine LCD Power control */
	fdone_lcd_power();
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
#if defined(CONFIG_REGULATOR_MP8845C)&& !defined(CONFIG_PMIC_REG_DUMP)
	bd_pmic_init();
#endif
	return 0;
}

#if defined(CONFIG_PMIC_REG_DUMP)
int power_init_board(void)
{
	int ret = 0;
	bd_pmic_init();
	return ret;
}
#endif

int board_init(void)
{
	DBGOUT("%s : done board init ...\n", CFG_SYS_BOARD_NAME);
	return 0;
}

void bd_display_run(char *cmd, int bl_duty, int bl_on)
{
	static int display_init = 0;

	if (cmd) {
		run_command(cmd, 0);
		lcd_draw_boot_logo(CONFIG_FB_ADDR, CFG_DISP_PRI_RESOL_WIDTH,
			CFG_DISP_PRI_RESOL_HEIGHT, CFG_DISP_PRI_SCREEN_PIXEL_BYTE);
	}

	if (!display_init) {
		bd_display();
		pwm_init(CFG_LCD_PRI_PWM_CH, 0, 0);
		display_init = 1;
	}

	pwm_config(CFG_LCD_PRI_PWM_CH,
		TO_DUTY_NS(bl_duty, CFG_LCD_PRI_PWM_FREQ),
		TO_PERIOD_NS(CFG_LCD_PRI_PWM_FREQ));

	if (bl_on)
		pwm_enable(CFG_LCD_PRI_PWM_CH);
}

extern void boot_animation(void);

int board_late_init(void)
{
	int power_key_val = 0;
	int mcu_fr_high_count = 0;
#if defined(CONFIG_SYS_MMC_BOOT_DEV)
	char boot[16];
	sprintf(boot, "mmc dev %d", CONFIG_SYS_MMC_BOOT_DEV);
#endif

// Add the update sd command 
#if defined (CONFIG_CMD_UPDATE_SDCARD)
    power_key_val = NX_GPIO_GetInputValue(PAD_GET_GROUP(CFG_IO_MCU_FR), 
                                          PAD_GET_BITNO(CFG_IO_MCU_FR));
	while(0 != power_key_val) {

		power_key_val = NX_GPIO_GetInputValue(PAD_GET_GROUP(CFG_IO_MCU_FR), 
                                               PAD_GET_BITNO(CFG_IO_MCU_FR));
		mcu_fr_high_count++;
		if(mcu_fr_high_count > 10) {
			printf("Emergency update \n");
			run_command("dna_mode 2 11000000 1",0); //Emergency update
        	run_command(CONFIG_CMD_RECOVERY_BOOT, 0);	/* recovery boot */
		}
        mdelay(500); //500ms
	}
	if (0 > run_command("update_sdcard mmc 0:1 0x48000000 partmap_burning.txt", 2)) {
		printf("## [%s():%d] ret_error \n", __func__,__LINE__);
	}

#endif


#ifndef CONFIG_USBBOOT_BURNING_MODE

	#if defined(CONFIG_RECOVERY_BOOT)
    if (RECOVERY_SIGNATURE == readl(SCR_RESET_SIG_READ)) {
        writel((-1UL), SCR_RESET_SIG_RESET); /* clear */

        printf("RECOVERY BOOT\n");
        //bd_display_run(CONFIG_CMD_LOGO_WALLPAPERS, CFG_LCD_PRI_PWM_DUTYCYCLE, 1);
		run_command("dna_mode 2 11000000 3",0); //Release update
        run_command(CONFIG_CMD_RECOVERY_BOOT, 0);	/* recovery boot */
    }
    writel((-1UL), SCR_RESET_SIG_RESET);
	#endif /* CONFIG_RECOVERY_BOOT */

	#ifdef CONFIG_SMP
	char *cmd = getenv("preloadcmd");
	if (cmd) {
		printf("preload cmd:%s\n", cmd);
		run_command(cmd, 0);
	}

	//boot_animation();
	#else
	#if defined(CONFIG_DISPLAY_OUT)
	//bd_display_run(0, CFG_LCD_PRI_PWM_DUTYCYCLE, 1);
	#endif
	#endif

	#if defined(CONFIG_VIP)
 	camera_run();
	camera_preview();
	#endif
#endif

#ifdef CONFIG_USBBOOT_BURNING_MODE
	run_command("fastboot nexell", 0);
#endif

	return 0;
}

