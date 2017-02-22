/*
 * (C) Copyright 2016 Nexell Co.,
 * jongsin park<pjsin865@nexell.co.kr>
 *
 * Configuation settings for the Nexell board.
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

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <spi.h>
#include <asm/sections.h>

#if defined (CONFIG_MACH_S5P4418)
#include <asm/arch/s5p4418_boot.h>
#elif defined (CONFIG_MACH_S5P6818)
#include <asm/arch/s5p6818_boot.h>
#endif

extern ssize_t spi_master_write (U16 u16_Addr, U8 data, U32 size);
extern void spi_master_init(int Index, int Slave);

int do_spi_write(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	ssize_t ret;
	unsigned int channel = 0, mode = 0;

	U8 buffer;

	channel = simple_strtoul(argv[1], NULL, 16);
	mode = 0;//simple_strtoul(argv[2], NULL, 16);

	//spi_slave_int_req_init(int_req);
	spi_master_init(channel, 0);

	//ret = spi_master_write(0xFFAA, 0x55, 1);
	ret = spi_master_write(0xF001, 0xAA, 1);

	mdelay(1);

	ret = spi_master_read(0xF001, &buffer, 1);

	printf("## \e[31m PJSMSG \e[0m [%s():%s:%d\t] buffer:0x%x \n", __FUNCTION__, strrchr(__FILE__, '/')+1, __LINE__, buffer);

	return 0;
}

U_BOOT_CMD(
	spiwrite, 2, 1,	do_spi_write,
	"spi master write test\n",
	"<channel> \n"
	"    - channel  : SPI Channel \n"
	"Note.\n"
);





