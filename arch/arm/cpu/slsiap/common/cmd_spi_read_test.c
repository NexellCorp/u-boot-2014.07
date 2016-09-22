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

#define POLY 				0x04C11DB7L

extern ssize_t spi_slave_read (uchar *buffer, int len);
extern void spi_slave_int_req_init(U32 int_req);
extern void spi_slave_init(int Index, int Slave);

unsigned int spi_get_fcs(unsigned int fcs, unsigned char data)
{
	register int i;
	fcs ^= (unsigned int)data;
	for(i=0; i<8; i++) {
	 	if(fcs & 0x01) fcs ^= POLY; fcs >>= 1;
 	}
	return fcs;
}

int do_spi_read(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	ssize_t ret;
	unsigned int channel = 0, mode = 0;
	uchar buf[4096] = { 0, };
	U32 int_req = ((PAD_GPIO_E + 10) | PAD_FUNC_ALT0);

	channel = simple_strtoul(argv[1], NULL, 16);
	mode = 1;//simple_strtoul(argv[2], NULL, 16);

	spi_slave_int_req_init(int_req);
	spi_slave_init(channel, mode);

	ret = spi_slave_read(buf, 4096);

	if (ret)
		printf("read %d, buf:0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \n", 
			ret, buf[0], buf[1], buf[2], buf[3],
			buf[4], buf[5], buf[6], buf[7]);

	return 0;
}

U_BOOT_CMD(
	spiread, 2, 1,	do_spi_read,
	"spi slave read test\n",
	"<channel> \n"
	"    - channel  : SPI Channel \n"
	"Note.\n"
);





