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
#include <fs.h>
#include <ext4fs.h>
#include <fat.h>
#include <pwm.h>

#include <linux/spinlock.h>
#include <asm/arch/cpu.h>
#include <asm/arch/display.h>
#include <asm/arch/pm.h>

#if (0)
#define	pr_debug(m...)	printf(m)
#else
#define	pr_debug(m...)	do { } while (0)
#endif

static void do_load_animation(int cpu);
static void do_draw_animation(int cpu);

void boot_animation(void)
{
	smp_cpu_register_fn(6, do_load_animation);
	smp_cpu_register_fn(7, do_draw_animation);
	smp_cpu_raise(6);
	smp_cpu_raise(7);
}

static void bd_backlight(int on)
{
	int ch = CFG_LCD_PRI_PWM_CH;
	u32 duty_ns = TO_DUTY_NS(CFG_LCD_PRI_PWM_DUTYCYCLE, CFG_LCD_PRI_PWM_FREQ);
	u32 period_ns = TO_PERIOD_NS(CFG_LCD_PRI_PWM_FREQ);

	if (on)	{
		pwm_init(ch, 0, 0);
		pwm_config(ch, duty_ns, period_ns);
		pwm_enable(ch);
		mdelay(10);
	} else {
		pwm_disable(ch);
		mdelay(10);
	}
}

extern void	bd_display(void);

#define SPLASH_TAG			"splash"
#define SPLASH_TAG_SIZE		6
#define	IMAGE_SIZE_MAX		40

typedef struct SPLASH_IMAGE_INFO{
	unsigned char	ucImageName[16];
	unsigned int 	ulImageAddr;
	unsigned int	ulImageSize;
	unsigned int	ulImageWidth;
	unsigned int	ulImageHeight;
	unsigned int 	Padding;
	unsigned char	ucRev[12];
} SPLASH_IMAGE_INFO;

typedef struct SPLASH_IMAGE_Header {
	unsigned char ucPartition[8];
	unsigned int  ulNumber;
	unsigned char ucRev[4];
} SPLASH_IMAGE_Header;

#define	ALIGN_BYTES			512
#define	DISK_ALIGN(x)		(((x+(ALIGN_BYTES-1))/ALIGN_BYTES) * ALIGN_BYTES)

#define	SPLASH_ADDRESS		0x7A600000
#define	SPLASH_FSLOAD		"ext4load"
#define	SPLASH_IFNAME		"mmc 2:1"
#define	SPLASH_IMAGE		"splash.img"
#define	SPLASH_FPS			(1*2)

#define	COMMAND_LOAD		"ext4load mmc 2:1 0x7A600000 splash.img"	// "fatload mmc 0:1 0x7A600000 splash.img"
static void do_load_animation(int cpu)
{
	char buffer[128] = { 0, };

	/* load splash.img */
	sprintf(buffer, "%s %s 0x%x %s", SPLASH_FSLOAD, SPLASH_IFNAME, SPLASH_ADDRESS, SPLASH_IMAGE);
	run_command(buffer, 0);
}

static void do_draw_animation(int cpu)
{
	u32 splash_base = SPLASH_ADDRESS;
	int width, height;
	int count, n = 0;
	u32 address;
	int module = 0, layer = 0;

	SPLASH_IMAGE_Header	*splash_header = (SPLASH_IMAGE_Header*)splash_base;
	SPLASH_IMAGE_INFO *splash_info;

	/* wait for load image */
	mdelay(100);

	/* check splash image */
	if (strncmp(SPLASH_TAG, (char*)splash_header->ucPartition, SPLASH_TAG_SIZE)) {
		printf("can't find splash image at 0x%x ...\n", splash_base);
		return;
	}

	/* parse splash.img */
	count = splash_header->ulNumber;
	if (count > IMAGE_SIZE_MAX) {
		printf("splash images %d is over max %d ...\n", count, IMAGE_SIZE_MAX);
		return;
	}

	splash_info = (SPLASH_IMAGE_INFO*)(splash_base + sizeof(*splash_header));
	width = splash_info->ulImageWidth;
	height = splash_info->ulImageHeight;
	address = splash_info->ulImageAddr + splash_base;

	pr_debug("splash %d * %d (%dEA)...\n", width, height, count);

	/* out dislay  */
	bd_backlight(0);

	bd_display();

	/* set FB and wait sync  */
	disp_mlc_set_address(module, layer, address);
	disp_mlc_wait_vsync(module, layer, SPLASH_FPS);
	bd_backlight(1);
	n++;

	while (count) {
		SPLASH_IMAGE_INFO *splash = &splash_info[n++%count];
		address = splash->ulImageAddr + splash_base;

		pr_debug("[%2d]: 0x%x\n", (n-1), address);
		/* change FB and wait sync  */
		disp_mlc_set_address(module, layer, address);
		disp_mlc_wait_vsync(module, layer, SPLASH_FPS);

		/* Exit */
		if (smp_cpu_check_stop()) {
			timer_stop(CFG_TIMER_SYS_TICK_CH);
			disp_mlc_set_enable(module, layer, 0);

			/* signature end */
			smp_cpu_set_end();
			return;
		}
	}
}
