/*------------------------------------------------------------------------------
 *
 *	Copyright (C) 2009 Nexell Co., Ltd All Rights Reserved
 *	Nexell Co. Proprietary & Confidential
 *
 *	NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 *  AND	WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
 *  FOR A PARTICULAR PURPOSE.
 *
 *	Module     : System memory config
 *	Description:
 *	Author     : Platform Team
 *	Export     :
 *	History    :
 *	   2009/05/13 first implementation
 ------------------------------------------------------------------------------*/
#ifndef __CFG_MAIN_H__
#define __CFG_MAIN_H__

#include <nx_type.h>

//------------------------------------------------------------------------------
// PLL input crystal
//------------------------------------------------------------------------------
#define	CFG_SYS_PLLFIN					24000000UL

/*------------------------------------------------------------------------------
 * 	System Name
 */
#define	CFG_SYS_CPU_NAME				"s5p4418"
#define	CFG_SYS_BOARD_NAMEi				"ktws"

/*------------------------------------------------------------------------------
 * 	BUS config
 */
#define CFG_BUS_RECONFIG_ENB			1		/* if want bus reconfig, select this first */
#define CFG_BUS_RECONFIG_TOPBUSSI		0
#define CFG_BUS_RECONFIG_BOTTOMBUSSI	0
#define CFG_BUS_RECONFIG_BOTTOMBUSQOS	0
#define CFG_BUS_RECONFIG_DISPBUSSI		1

/*------------------------------------------------------------------------------
 * 	Debug Uart
 */
#define CFG_UART_DEBUG_CH				3
#define CFG_UART_DEBUG_BAUDRATE			115200
#define CFG_UART_DEBUG_USE_UART			CTRUE
#define	CFG_UART_CLKGEN_CLOCK_HZ		14750000	// 50000000

/*------------------------------------------------------------------------------
 * 	Timer List
 */
#define	CFG_TIMER_SYS_TICK_CH			0

/*------------------------------------------------------------------------------
 *  GMAC PHY
 */
#define CFG_ETHER_GMAC_PHY_IRQ_NUM		(IRQ_GPIO_E_START + 12)
#define CFG_ETHER_GMAC_PHY_RST_NUM		(PAD_GPIO_E + 23)

/*------------------------------------------------------------------------------
 * 	Nand (HWECC)
 */
/* MTD */
#define CFG_NAND_ECC_BYTES				1024
#define CFG_NAND_ECC_BITS				40			/* 512 - 4,8,16,24 1024 - 24,40,60 */
//#define CFG_NAND_ECCIRQ_MODE

/* FTL */
#define CFG_NAND_FTL_START_BLOCK		0x6000000	/* byte address, Must Be Multiple of 8MB */
#define CFG_BOOTIMG_OFFSET				0x100000	/* uboot.ecc */
#define CFG_BOOTIMG_REPEAT				32

/*------------------------------------------------------------------------------
 *	Nand (GPIO)
 */
#define CFG_IO_NAND_nWP					(-1)		/* GPIO */

/*------------------------------------------------------------------------------
 *  SPI
 */
#define CFG_SPI0_SRC_CLK				100*1000*1000
#define CFG_SPI0_OUT_CLK				20*1000*1000

#define CFG_SPI1_SRC_CLK				100*1000*1000
#define CFG_SPI1_OUT_CLK				30*1000*1000

#define CFG_SPI2_SRC_CLK				100*1000*1000
#define CFG_SPI2_OUT_CLK				30*1000*1000

//------------------------------------------------------------------------------
// Static Bus #0 ~ #9, NAND, IDE configuration
//------------------------------------------------------------------------------
//	_BW	  : Staic Bus width for Static #0 ~ #9            : 8 or 16
//
//	_TACS : adress setup time before chip select          : 0 ~ 15
//	_TCOS : chip select setup time before nOE is asserted : 0 ~ 15
//	_TACC : access cycle                                  : 1 ~ 256
//	_TSACC: burst access cycle for Static #0 ~ #9 & IDE   : 1 ~ 256
//	_TOCH : chip select hold time after nOE not asserted  : 0 ~ 15
//	_TCAH : address hold time after nCS is not asserted   : 0 ~ 15
//
//	_WAITMODE : wait enable control for Static #0 ~ #9 & IDE : 1=disable, 2=Active High, 3=Active Low
//	_WBURST	  : burst write mode for Static #0 ~ #9          : 0=disable, 1=4byte, 2=8byte, 3=16byte
//	_RBURST   : burst  read mode for Static #0 ~ #9          : 0=disable, 1=4byte, 2=8byte, 3=16byte
//
//------------------------------------------------------------------------------
#define CFG_SYS_STATICBUS_CONFIG( _name_, bw, tACS, tCOS, tACC, tSACC, tCOH, tCAH, wm, rb, wb )	\
	enum {											\
		CFG_SYS_ ## _name_ ## _BW		= bw,		\
		CFG_SYS_ ## _name_ ## _TACS		= tACS,		\
		CFG_SYS_ ## _name_ ## _TCOS		= tCOS,		\
		CFG_SYS_ ## _name_ ## _TACC		= tACC,		\
		CFG_SYS_ ## _name_ ## _TSACC	= tSACC,	\
		CFG_SYS_ ## _name_ ## _TCOH		= tCOH,		\
		CFG_SYS_ ## _name_ ## _TCAH		= tCAH,		\
		CFG_SYS_ ## _name_ ## _WAITMODE	= wm,		\
		CFG_SYS_ ## _name_ ## _RBURST	= rb,		\
		CFG_SYS_ ## _name_ ## _WBURST	= wb		\
	};

//                      ( _name_ , bw, tACS tCOS tACC tSACC tOCH tCAH, wm, rb, wb )
CFG_SYS_STATICBUS_CONFIG( STATIC0 ,  8,    1,   1,   6,    6,   1,   1,  1,  0,  0 )		// 0x0000_0000
CFG_SYS_STATICBUS_CONFIG( STATIC1 ,  8,    6,   6,  32,   32,   6,   6,  1,  0,  0 )		// 0x0400_0000
CFG_SYS_STATICBUS_CONFIG(    NAND ,  8,    0,   3,   9,    1,   3,   0,  1,  0,  0 )		// 0x2C00_0000, tOCH, tCAH must be greter than 0

#endif /* __CFG_MAIN_H__ */
