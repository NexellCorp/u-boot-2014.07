/*
 *  Copyright (C) 2013 NEXELL SOC Lab.
 *  BongKwan Kook <kook@nexell.co.kr>
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

#include <platform.h>
#include <common.h>
#include <asm/io.h>
#include <asm/gpio.h>

#if (0)
#define DBGOUT(msg...)		{ printf("BD: " msg); }
#else
#define DBGOUT(msg...)		do {} while (0)
#endif

void nxp_gpio_set_alt(int gpio, int mode)
{
	int grp, bit;

	if (gpio > (PAD_GPIO_ALV - 1) )
		return;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	NX_GPIO_SetPadFunction(grp, bit, (NX_GPIO_PADFUNC)mode);
}

int nxp_gpio_get_alt(int gpio)
{
	int grp, bit;

	if (gpio > (PAD_GPIO_ALV - 1) )
		return -1;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	return (int) NX_GPIO_GetPadFunction(grp, bit);
}

void nxp_gpio_direction_output(int gpio, int val)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
	{
		NX_GPIO_SetOutputValue(grp, bit, (val ? CTRUE : CFALSE));
		NX_GPIO_SetOutputEnable(grp, bit, CTRUE);
	}
	else
	{
		NX_ALIVE_SetOutputValue(bit, (val ? CTRUE : CFALSE));
		NX_ALIVE_SetOutputEnable(bit, CTRUE);
	}
}

void nxp_gpio_direction_input(int gpio)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
		NX_GPIO_SetOutputEnable(grp, bit, CFALSE);
	else
		NX_ALIVE_SetOutputEnable(bit, CFALSE);
}

void nxp_gpio_set_value(int gpio, int val)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
		NX_GPIO_SetOutputValue(grp, bit, (val ? CTRUE : CFALSE));
	else
		NX_ALIVE_SetOutputValue(bit, (val ? CTRUE : CFALSE));
}

int nxp_gpio_get_value(int gpio)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
		return (int) NX_GPIO_GetInputValue(grp, bit);
	else
		return (int) NX_ALIVE_GetInputValue(bit);
}

void nxp_gpio_set_pull_enb(int gpio, int mode)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	NX_GPIO_SetPullEnable(grp, bit, mode);
}

void nxp_gpio_set_pull_sel(int gpio, int mode)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	NX_GPIO_SetPullSelect(grp, bit, mode);
}

void nxp_gpio_set_drv(int gpio, int mode)
{
	int drv1 = 0, drv0 = 0;
	int drv1_value, drv0_value;
	int grp, bit;

	if (gpio > (PAD_GPIO_ALV - 1) )
		return;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	switch (mode)
	{
	case GPIO_DRV_1X: drv0 = 0, drv1 = 0; break;
	case GPIO_DRV_2X: drv0 = 0, drv1 = 1; break;
	case GPIO_DRV_3X: drv0 = 1, drv1 = 0; break;
	case GPIO_DRV_4X: drv0 = 1, drv1 = 1; break;
	default: break;
	}
	DBGOUT("DRV Strength : GRP : i %x Bit: %x  mode :%d  \n   ",
				grp, bit, mode);

	drv1_value = NX_GPIO_GetDRV1(grp) & ~(1 << bit);
	drv0_value = NX_GPIO_GetDRV0(grp) & ~(1 << bit);

	if (drv1) drv1_value |= (drv1 << bit);
	if (drv0) drv0_value |= (drv0 << bit);

	DBGOUT(" Value : drv1 :%8x  drv0 %8x \n ", drv1_value, drv0_value);

	NX_GPIO_SetDRV0 ( grp, drv0_value );
	NX_GPIO_SetDRV1 ( grp, drv1_value );
}

int nxp_gpio_get_group(int gpio)
{
	return (int) ((gpio / 32) & 0x07);
}

int nxp_gpio_get_pin(int gpio)
{
	return (int) (gpio % 32);
}



/* Interrupt Interface  */

void nxp_gpio_set_int_mode(int gpio, int d_mode)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
	{
		NX_GPIO_SetPadFunction(grp, bit, PAD_MODE_INT);
		NX_GPIO_SetInterruptMode(grp, bit, d_mode);
	}
	else
	{
		NX_ALIVE_SetDetectMode( d_mode, bit, CTRUE );
	}
}

void nxp_gpio_set_int_en(int gpio, int enable)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
		NX_GPIO_SetInterruptEnable(grp, bit, enable);
	else
		NX_ALIVE_SetInterruptEnable(bit , enable);
}


int nxp_gpio_get_int_en(int gpio)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
		return (int) NX_GPIO_GetInterruptEnable(grp, bit);
	else
		return (int) NX_ALIVE_GetInterruptEnable(bit);
}

int nxp_gpio_get_int_pend(int gpio)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
		return (int) NX_GPIO_GetInterruptPending(grp, bit);
	else
		return (int) NX_ALIVE_GetInterruptPending(bit);
}

void nxp_gpio_set_int_clear(int gpio)
{
	int grp, bit;

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);

	if (gpio < PAD_GPIO_ALV)
		NX_GPIO_ClearInterruptPending(grp, bit);
	else
		NX_ALIVE_ClearInterruptPending(bit);
}

/* Common GPIO API */

int gpio_request(unsigned gpio, const char *label)
{
	return 0;
}

int gpio_free(unsigned gpio)
{
	return 0;
}

int gpio_direction_input(unsigned gpio)
{
	nxp_gpio_direction_input(gpio);
	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	nxp_gpio_direction_output(gpio, value);
	return 0;
}

int gpio_get_value(unsigned gpio)
{
	return nxp_gpio_get_value(gpio);
}

int gpio_set_value(unsigned gpio, int value)
{
	nxp_gpio_set_value(gpio, value);

	return 0;
}
