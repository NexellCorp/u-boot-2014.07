/*
 * (C) Copyright 2015
 *  Jongshin Park, Nexell Co, <pjsin865@nexell.co.kr>
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

#include <config.h>
#include <common.h>
#include <mmc.h>
#include <pwm.h>
#include <asm/io.h>
#include <asm/gpio.h>

#include <platform.h>
#include <mach-api.h>


#include <i2c.h>
#include <errno.h>

struct usb2514 {
    struct i2c_client   *client;
    u16         model;
};

typedef struct
{
    u8 reg;
    u8 value;
} i2cparam;

i2cparam usb2514_on_setting[] =
{
    {0x00, 0x24},   {0x01, 0x04},   {0x02, 0x12},   {0x03, 0x25},   {0x04, 0xB3},   {0x05, 0x0B},   {0x06, 0x9B},   {0x07, 0x20},
    {0x08, 0x02},   {0x09, 0x00},   {0x0A, 0x00},   {0x0B, 0x00},   {0x0C, 0x01},   {0x0D, 0x32},   {0x0E, 0x01},   {0x0F, 0x32},
    {0x10, 0x32},   {0xF6, 0x00},   {0xF8, 0x00},   {0xFB, 0x21},   {0xFC, 0x43},   {0xFF, 0x01},
};


int bd_usb_hub_init_usb2514(int i2c_bus, int enable)
{
	int i;
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	i2c_set_bus_num(i2c_bus);

	gpio_set_value(USB_HUB_PORT1_PWR_EN, 1);
	gpio_set_value(USB_HUB_PORT2_PWR_EN, 1);
	gpio_set_value(USB_HUB_PORT3_PWR_EN, 1);

	// reset the hub chip
	gpio_direction_output(USB_HUB_RESET_PIN, 1);
	gpio_set_value(USB_HUB_RESET_PIN, 1);
	mdelay(10);
	gpio_set_value(USB_HUB_RESET_PIN, 0);
	mdelay(10);
	gpio_set_value(USB_HUB_RESET_PIN, 1);
	mdelay(10);

	for (i = 0; i < (sizeof(usb2514_on_setting)/sizeof(i2cparam)); i++)	{
       i2c_write(USB2514_I2C_ADDR, usb2514_on_setting[i].reg, 1, &(usb2514_on_setting[i].value), 1);
	}
	
	return 0;
}



