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
#include <netdev.h>

/*------------------------------------------------------------------------------
 * u-boot eth interface
 */
#include <net.h>

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
#if defined(CONFIG_DRIVER_DM9000)
    dm9000_initialize(bis);
    return eth_init(bis);
#endif  // CONFIG_DRIVER_DM9000

#if defined(CONFIG_DESIGNWARE_ETH)
    u32 interface;

    interface = PHY_INTERFACE_MODE_MII;
#if defined(CONFIG_DW_AUTONEG)
    interface = PHY_INTERFACE_MODE_RGMII;
#endif

    if (designware_initialize(0, CONFIG_DWCGMAC_BASE, CONFIG_PHY_ADDR,
                interface) >= 0)
        return 1;
#endif  // CONFIG_DESIGNWARE_ETH

    return -1;
}
#endif  /* CONFIG_CMD_NET */

