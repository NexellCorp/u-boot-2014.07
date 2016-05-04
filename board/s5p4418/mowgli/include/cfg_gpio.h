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
#ifndef __CFG_GPIO_H__
#define __CFG_GPIO_H__

/*------------------------------------------------------------------------------
 *
 *	(GROUP_A)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOA0      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PVCLK               ,2:_                    ,3: TESTMODE[4]         =LCD_PCLK
#define PAD_GPIOA1      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[0]          ,2:_                    ,3:_                    =LCD_XD_B0
#define PAD_GPIOA2      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[1]          ,2:_                    ,3: TESTMODE[0]         =LCD_XD_B1
#define PAD_GPIOA3      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[2]          ,2:_                    ,3: TESTMODE[1]         =LCD_XD_B2
#define PAD_GPIOA4      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[3]          ,2:_                    ,3: TESTMODE[2]         =LCD_XD_B3
#define PAD_GPIOA5      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[4]          ,2:_                    ,3: TESTMODE[3]         =LCD_XD_B4
#define PAD_GPIOA6      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[5]          ,2:_                    ,3:_                    =LCD_XD_B5
#define PAD_GPIOA7      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[6]          ,2:_                    ,3:_                    =LCD_XD_B6
#define PAD_GPIOA8      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[7]          ,2:_                    ,3:_                    =LCD_XD_B7
#define PAD_GPIOA9      (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[8]          ,2:_                    ,3:_                    =LCD_XD_G0
#define PAD_GPIOA10     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[9]          ,2:_                    ,3:_                    =LCD_XD_G1
#define PAD_GPIOA11     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[10]         ,2:_                    ,3:_                    =LCD_XD_G2
#define PAD_GPIOA12     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[11]         ,2:_                    ,3:_                    =LCD_XD_G3
#define PAD_GPIOA13     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[12]         ,2:_                    ,3:_                    =LCD_XD_G4
#define PAD_GPIOA14     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[13]         ,2:_                    ,3:_                    =LCD_XD_G5
#define PAD_GPIOA15     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[14]         ,2:_                    ,3:_                    =LCD_XD_G6
#define PAD_GPIOA16     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[15]         ,2:_                    ,3:-                    =LCD_XD_G7
#define PAD_GPIOA17     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[16]         ,2:_                    ,3:_                    =LCD_XD_B0
#define PAD_GPIOA18     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[17]         ,2:_                    ,3:_                    =LCD_XD_B1
#define PAD_GPIOA19     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[18]         ,2:_                    ,3:_                    =LCD_XD_B2
#define PAD_GPIOA20     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[19]         ,2:_                    ,3:_                    =LCD_XD_B3
#define PAD_GPIOA21     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[20]         ,2:_                    ,3:_                    =LCD_XD_B4
#define PAD_GPIOA22     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[21]         ,2:_                    ,3:_                    =LCD_XD_B5
#define PAD_GPIOA23     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[22]         ,2:_                    ,3:_                    =LCD_XD_B6
#define PAD_GPIOA24     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDRGB24[23]         ,2:_                    ,3:_                    =LCD_XD_B7
#define PAD_GPIOA25     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDVSYNC             ,2:_                    ,3:_                    =LCD_VSYNC
#define PAD_GPIOA26     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDHSYNC             ,2:_                    ,3:_                    =LCD_HSYNC
#define PAD_GPIOA27     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_3)     // 0: GPIO          ,1: PDDE                ,2:_                    ,3:_                    =LCD_DE
#define PAD_GPIOA28     (PAD_MODE_OUT  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VP0_EXTCLK          ,2: I2S2_CLK            ,3: I2S1_CLK            =Reserved
#define PAD_GPIOA29     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_2)     // 0: GPIO          ,1: SDMMC0_CCLK         ,2:_                    ,3:_                    =EMMC_CLK
#define PAD_GPIOA30     (PAD_MODE_OUT  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[0]          ,2: SDEX[0]             ,3: I2S1_BCLK           =Reserved
#define PAD_GPIOA31     (PAD_MODE_ALT  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW   | PAD_PULL_OFF | PAD_STRENGTH_2)     // 0: GPIO          ,1: SDMMC0_CMD          ,2:_                    ,3:_                    =EMMC_CMD

/*------------------------------------------------------------------------------
 *	(GROUP_B)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOB0      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[1]          ,2: SDEX[1]             ,3: I2S1_LRCLK          =Reserved
#define PAD_GPIOB1      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_2)     // 0: GPIO          ,1: SDMMC0_CDATA[0]     ,2:_                    ,3:_                    =EMMC_DAT0
#define PAD_GPIOB2      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[2]          ,2: SDEX[2]             ,3: I2S2_BCLK           =Reserved
#define PAD_GPIOB3      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_2)     // 0: GPIO          ,1: SDMMC0_CDATA[1]     ,2:_                    ,3:_                    =EMMC_DAT1
#define PAD_GPIOB4      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[3]          ,2: SDEX[3]             ,3: I2S2_LRCLK          =Reserved
#define PAD_GPIOB5      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_2)     // 0: GPIO          ,1: SDMMC0_CDATA[2]     ,2:_                    ,3:_                    =EMMC_DAT2
#define PAD_GPIOB6      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[4]          ,2: SDEX[4]             ,3: I2S1SDO             =Reserved
#define PAD_GPIOB7      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_2)     // 0: GPIO          ,1: SDMMC0_CDATA[3]     ,2:_                    ,3:_                    =EMMC_DAT3
#define PAD_GPIOB8      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[5]          ,2: SDEX[5]             ,3: I2S2SDO             =Reserved
#define PAD_GPIOB9      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[6]          ,2: SDEX[6]             ,3: I2S1SDI             =Reserved
#define PAD_GPIOB10     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[7]          ,2: SDEX[7]             ,3: I2S2SDI             =Reserved
#define PAD_GPIOB11     (PAD_MODE_IN  | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: CLE           ,1: CLE1                ,2: GPIO                ,3:_                    =USB20H1_FLT
#define PAD_GPIOB12     (PAD_MODE_IN  | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: ALE           ,1: ALE1                ,2: GPIO                ,3:_                    =USB OTG ID Check
#define PAD_GPIOB13     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[0]    ,1: GPIO                ,2:_                    ,3:_                    =SD0
#define PAD_GPIOB14     (PAD_MODE_OUT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_RnB      ,1: MCUS_RnB1           ,2: GPIO                ,3:_                    =USBH2_PWR_EN
#define PAD_GPIOB15     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[1]    ,1: GPIO                ,2:_                    ,3:_                    =SD1
#define PAD_GPIOB16     (PAD_MODE_IN  | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_nNOFE    ,1: MCUS_nNOFE1         ,2: GPIO                ,3:_                    =USB20H2_FLT
#define PAD_GPIOB17     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[2]    ,1: GPIO                ,2:_                    ,3:_                    =SD2
#define PAD_GPIOB18     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_nNFWE    ,1: MCUS_nNFWE1         ,2: GPIO                ,3:_                    =USBH1_PWR_EN
#define PAD_GPIOB19     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[3]    ,1: GPIO                ,2:_                    ,3:_                    =SD3
#define PAD_GPIOB20     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[4]    ,1: GPIO                ,2:_                    ,3:_                    =SD4
#define PAD_GPIOB21     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[5]    ,1: GPIO                ,2:_                    ,3:_                    =SD5
#define PAD_GPIOB22     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[6]    ,1: GPIO                ,2:_                    ,3:_                    =SD6
#define PAD_GPIOB23     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[7]    ,1: GPIO                ,2:_                    ,3:_                    =SD7
#define PAD_GPIOB24     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[8]    ,1: GPIO                ,2: MPEGTSI0_TDATA[0]   ,3:_                    =Reserved
#define PAD_GPIOB25     (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[9]    ,1: GPIO                ,2: MPEGTSI0_TDATA[1]   ,3:_                    =JACK_DET
#define PAD_GPIOB26     (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[10]   ,1: GPIO                ,2: MPEGTSI0_TDATA[2]   ,3: ECID_BONDING_ID[2]  =MIC_DET
#define PAD_GPIOB27     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_HIGH  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[11]   ,1: GPIO                ,2: MPEGTSI0_TDATA[3]   ,3:_                    =SPK_AMP_ON
#define PAD_GPIOB28     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[12]   ,1: GPIO                ,2: MPEGTSI0_TDATA[4]   ,3: UART4_RXD           =CD_POP_SHDN
#define PAD_GPIOB29     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[13]   ,1: GPIO                ,2: MPEGTSI0_TDATA[5]   ,3: UART4_TXD           =MONO_POP_SHDN
#define PAD_GPIOB30     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[14]   ,1: GPIO                ,2: MPEGTSI0_TDATA[6]   ,3: UART5_RXD           =CD_PWR_EN
#define PAD_GPIOB31     (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[15]   ,1: GPIO                ,2: MPEGTSI0_TDATA[7]   ,3: UART5_TXD           =AVIN_DET

/*------------------------------------------------------------------------------
 *	(GROUP_C)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOC0      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[0]  ,1: GPIO                ,2: MPEGTSI0_TSERR      ,3:_                    =Reserved
#define PAD_GPIOC1      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[1]  ,1: GPIO                ,2: MPEGTSI1_TSERR      ,3:_                    =Reserved
#define PAD_GPIOC2      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[2]  ,1: GPIO                ,2:_                    ,3:_                    =Reserved
#define PAD_GPIOC3      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[3]  ,1: GPIO                ,2: HDMI_CEC            ,3: SDMMC0_nRST         =Reserved
#define PAD_GPIOC4      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[4]  ,1: GPIO                ,2: UART1_DCD           ,3: SDMMC0_CARD_nint    =Reserved
#define PAD_GPIOC5      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[5]  ,1: GPIO                ,2: UART1_CTS           ,3: SDMMC0_WP           =Reserved
#define PAD_GPIOC6      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[6]  ,1: GPIO                ,2: UART1_RTS           ,3: SDMMC0_DETECT       =Reserved
#define PAD_GPIOC7      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[7]  ,1: GPIO                ,2: UART1_DSR           ,3: SDMMC1_nRST         =Reserved
#define PAD_GPIOC8      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[8]  ,1: GPIO                ,2: UART1_DTR           ,3: SDMMC1_CARD_nint    =Reserved
#define PAD_GPIOC9      (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[9]  ,1: GPIO                ,2: SSP2_CLK_IO         ,3: PDM_STROBE          =Reserved
#define PAD_GPIOC10     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[10] ,1: GPIO                ,2: SSP2_FSS            ,3: MCUS_nNCS[2]        =Reserved
#define PAD_GPIOC11     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[11] ,1: GPIO                ,2: SSP2_RXD            ,3: USB2.0OTG0_DRVBUS   =Reserved
#define PAD_GPIOC12     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[12] ,1: GPIO                ,2: SSP2_TXD            ,3: SDMMC2_nRST         =Reserved
#define PAD_GPIOC13     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[13] ,1: GPIO                ,2: PWM1_OUT            ,3: SDMMC2_CARD_nint    =Reserved
#define PAD_GPIOC14     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[14] ,1: GPIO                ,2: PWM2_OUT            ,3: VIP0_ExtCLK2        =Reserved
#define PAD_GPIOC15     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[15] ,1: GPIO                ,2: MPEGTSI0_TSCLK      ,3: VIP0_HSYNC2         =Reserved
#define PAD_GPIOC16     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[16] ,1: GPIO                ,2: MPEGTSI0_TSYNC0     ,3: VIP0_VSYNC2         =Reserved
#define PAD_GPIOC17     (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[17] ,1: GPIO                ,2: MPEGTSI0_TDP0       ,3: VIP0_VD2[0]         =SD_WP
#define PAD_GPIOC18     (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[18] ,1: GPIO                ,2: SDMMC2_CCLK         ,3: VIP0_VD2[1]         =SD_CLK
#define PAD_GPIOC19     (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[19] ,1: GPIO                ,2: SDMMC2_CMD          ,3: VIP0_VD2[2]         =SD_CMD
#define PAD_GPIOC20     (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[20] ,1: GPIO                ,2: SDMMC2_CDATA[0]     ,3: VIP0_VD2[3]         =SD_DAT0
#define PAD_GPIOC21     (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[21] ,1: GPIO                ,2: SDMMC2_CDATA[1]     ,3: VIP0_VD2[4]         =SD_DAT1
#define PAD_GPIOC22     (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[22] ,1: GPIO                ,2: SDMMC2_CDATA[2]     ,3: VIP0_VD2[5]         =SD_DAT2
#define PAD_GPIOC23     (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[23] ,1: GPIO                ,2: SDMMC2_CDATA[3]     ,3: VIP0_VD2[6]         =SD_DAT3
#define PAD_GPIOC24     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_LATADDR  ,1: GPIO                ,2: SPDIFIN             ,3: VIP0_VD2[7]         =Reserved
#define PAD_GPIOC25     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_nSWAIT   ,1: GPIO                ,2: SPDIF_DATA          ,3:_                    =Reserved
#define PAD_GPIOC26     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: MCUS_RDnWR    ,1: GPIO                ,2: PDM_DATA0           ,3:_                    =USB Switch Enable
#define PAD_GPIOC27     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_nSDQM1   ,1: GPIO                ,2: PDM_DATA1           ,3:_                    =Reserved
#define PAD_GPIOC28     (PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: MCUS_nSCS[1]        ,2: UART1_TRI           ,3:_                    =Reserved
#define PAD_GPIOC29     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SSP0_CLKIO          ,2:_                    ,3:_                    =SPI_DMB_SCLK
#define PAD_GPIOC30     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SSP0_FSS            ,2:_                    ,3:_                    =SPI_DMB_CSN
#define PAD_GPIOC31     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SSP0_TXD            ,2:_                    ,3:_                    =SPI_DMB_SDO

/*------------------------------------------------------------------------------
 *	(GROUP_D)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOD0      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SSP0_RXD            ,2: PWM3_OUT            ,3:_                    =SPI_DMB_SDI
#define PAD_GPIOD1      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: PWM0_OUT            ,2: MCUS_ADDR[25]       ,3:_                    =LCD_BL_PWM
#define PAD_GPIOD2      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: I2C0_SCL            ,2: UART4_SMCAYEN       ,3:_                    =VID_SCL
#define PAD_GPIOD3      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: I2C0_SDA            ,2: UART5_SMCAYEN       ,3:_                    =VID_SDA
#define PAD_GPIOD4      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2C1_SCL            ,2:_                    ,3:_                    =TOUCH_SCL1
#define PAD_GPIOD5      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2C1_SDA            ,2:_                    ,3:_                    =TOUCH_SDA1
#define PAD_GPIOD6      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2C2_SCL            ,2:_                    ,3:_                    =CD_I2C_SCL
#define PAD_GPIOD7      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2C2_SDA            ,2:_                    ,3:_                    =CD_I2C_SDA
#define PAD_GPIOD8      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: PPM_IN              ,2:_                    ,3:_                    =Reserved
#define PAD_GPIOD9      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_SDO            ,2: AC97_ACSDATAOUT     ,3:_                    =CD_I2S_SDOUT
#define PAD_GPIOD10     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_BCLK           ,2: AC97_ACBITCLK       ,3:_                    =CD_I2S_SCLK
#define PAD_GPIOD11     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_SDI            ,2: AC97_ACSDATAIN      ,3:_                    =CD_I2S_SDIN
#define PAD_GPIOD12     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_LRCLK          ,2: AC97_ACSYNC         ,3:_                    =CD_I2S_LRCLK
#define PAD_GPIOD13     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_CODCLK         ,2: AC97_nACRESET       ,3:_                    =CD_I2S_MCLK
#define PAD_GPIOD14     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART0RXD            ,2: UART1_SMCAYEN       ,3:_                    =UART_DEBUG_RXD
#define PAD_GPIOD15     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART1RXD            ,2: UART2_SMCAYEN       ,3:_                    =GPS_RXD
#define PAD_GPIOD16     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART2RXD            ,2: CAN0_TX             ,3:_                    =Reserved
#define PAD_GPIOD17     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART3RXD            ,2: CAN1_TX             ,3:_                    =Reserved
#define PAD_GPIOD18     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART0TXD            ,2:_                    ,3: SDnCD2              =UART_DEBUG_TXD
#define PAD_GPIOD19     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART1TXD            ,2:_                    ,3:_                    =GPS_TXD
#define PAD_GPIOD20     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART2TXD            ,2:_                    ,3:_                    =Reserved
#define PAD_GPIOD21     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART3TXD            ,2:_                    ,3:_                    =Reserved
#define PAD_GPIOD22     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_HIGH  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CCLK         ,2:_                    ,3:_                    =DMB 1.2V LDO Enable
#define PAD_GPIOD23     (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CMD          ,2:_                    ,3:_                    =DMB_INT
#define PAD_GPIOD24     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_HIGH  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CDATA[0]     ,2:_                    ,3:_                    =DMB 3.3 LDO Enable
#define PAD_GPIOD25     (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CDATA[1]     ,2:_                    ,3:_                    =PWR_DET
#define PAD_GPIOD26     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CDATA[3]     ,2:_                    ,3:_                    =Reserved
#define PAD_GPIOD27     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CDATA[3]     ,2:_                    ,3:_                    =SD_PWR_EN
#define PAD_GPIOD28     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP1_VD[0]          ,2: MPEGTSI_TDATA1[0]   ,3: MCUS_ADDR[24]       =TW9900_D0
#define PAD_GPIOD29     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP1_VD[1]          ,2: MPEGTSI_TDATA1[1]   ,3:_                    =TW9900_D1
#define PAD_GPIOD30     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP1_VD[2]          ,2: MPEGTSI_TDATA1[2]   ,3:_                    =TW9900_D2
#define PAD_GPIOD31     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP1_VD[3]          ,2: MPEGTSI_TDATA1[3]   ,3:_                    =TW9900_D3

/*------------------------------------------------------------------------------
 *	(GROUP_E)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOE0      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_VD[4]           ,2: MPEGTSI_TDATA1[0]   ,3:_                    =TW9900_D4
#define PAD_GPIOE1      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_VD[5]           ,2: MPEGTSI_TDATA1[0]   ,3:_                    =TW9900_D5
#define PAD_GPIOE2      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_VD[6]           ,2: MPEGTSI_TDATA1[0]   ,3:_                    =TW9900_D6
#define PAD_GPIOE3      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_VD[7]           ,2: MPEGTSI_TDATA1[0]   ,3:_                    =TW9900_D7
#define PAD_GPIOE4      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_ExtCLK          ,2: MPEGTSI_TCLK1       ,3:_                    =TW9900_CLK
#define PAD_GPIOE5      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_HSYNC           ,2: MPEGTSI_TSYNC1      ,3:_                    =TW9900_HSYNC
#define PAD_GPIOE6      (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: VIP_VSYNC            ,2: MPEGTSI_TDP1        ,3:_                    =TW9900_VSYNC
#define PAD_GPIOE7      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXD[0]     ,2: VIP0_Ext_VSYNC      ,3:_                    =Reserved
#define PAD_GPIOE8      (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXD[1]     ,2:_                    ,3:_                    =TW9900_MPOUT
#define PAD_GPIOE9      (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXD[2]     ,2:_                    ,3:_                    =TW9900_RSTN
#define PAD_GPIOE10     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXD[3]     ,2:_                    ,3:_                    =TW9900_PDN
#define PAD_GPIOE11     (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXEN       ,2:_                    ,3:_                    =TW9900_INT
#define PAD_GPIOE12     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_HIGH  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXER       ,2:_                    ,3:_                    =GPS Ext LDO Enable
#define PAD_GPIOE13     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_COL        ,2: VIP0_Ext_HSYNC      ,3:_                    =Reserved
#define PAD_GPIOE14     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXD[0]     ,2: SSP1_CLKIO          ,3:_                    =PMIC_SCL
#define PAD_GPIOE15     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXD[1]     ,2: SSP1_FSS            ,3:_                    =PMIC_SDA
#define PAD_GPIOE16     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXD[2]     ,2:_                    ,3:_                    =Reserved
#define PAD_GPIOE17     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXD[3]     ,2:_                    ,3:_                    =Reserved
#define PAD_GPIOE18     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_CLK_RX         ,2: SSP1_RXD            ,3:_                    =Reserved
#define PAD_GPIOE19     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RX_DV      ,2: SSP1_TXD            ,3:_                    =Reserved
#define PAD_GPIOE20     (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_GMII_MDC       ,2:_                    ,3:_                    =TOUCH_INT
#define PAD_GPIOE21     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_GMII_MDI       ,2:_                    ,3:_                    =TOUCH_PWR_EN
#define PAD_GPIOE22     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_HIGH  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXER       ,2:_                    ,3:_                    =LCD_RESET
#define PAD_GPIOE23     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_HIGH  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_CRS        ,2:_                    ,3:_                    =LCD_PWREN
#define PAD_GPIOE24     (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_HIGH  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_GTX_CLK        ,2:_                    ,3:_                    =LCD_DR_PWR_ON
#define PAD_GPIOE25     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: nTRST         1: GPIO                 ,2:_                    ,3:_                    =JTAG_TRST
#define PAD_GPIOE26     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: TMS           1: GPIO                 ,2:_                    ,3:_                    =JTAG_TMD
#define PAD_GPIOE27     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: TDI           1: GPIO                 ,2:_                    ,3:_                    =JTAG_TDI
#define PAD_GPIOE28     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: TCLK          1: GPIO                 ,2:_                    ,3:_                    =JTAG_TCLK
#define PAD_GPIOE29     (PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: TDO           1: GPIO                 ,2:_                    ,3:_                    =JTAG_TDO
#define PAD_GPIOE30     (PAD_MODE_IN | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_UP | PAD_STRENGTH_0)     // 0: MCUS_nSOE     1: GPIO                 ,2:_                    ,3:_                    =Control Box Key input
#define PAD_GPIOE31     (PAD_MODE_IN | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_UP | PAD_STRENGTH_0)     // 0: MCUS_nSWE     1: GPIO                 ,2:_                    ,3:_                    =Control Box Detect check

/*------------------------------------------------------------------------------
 *	(GROUPALV)
 *	0                     4                             8        12
 *	| MODE(IN/OUT/DETECT) | ALIVE OUT or ALIVE DETMODE0 | PullUp |
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOALV0    (PAD_MODE_IN  | PAD_LEVEL_LOW  | PAD_PULL_UP )				//PWRKEY
#define PAD_GPIOALV1    (PAD_MODE_IN  | PAD_LEVEL_LOW  | PAD_PULL_UP )				//SD_CDN
#define PAD_GPIOALV2    (PAD_MODE_OUT | PAD_LEVEL_LOW  | PAD_PULL_UP )				//Reserved
#define PAD_GPIOALV3    (PAD_MODE_OUT | PAD_LEVEL_LOW  | PAD_PULL_UP )				//Reserved
#define PAD_GPIOALV4    (PAD_MODE_IN  | PAD_LEVEL_LOW  | PAD_PULL_UP )				//PMIC_INT
#define PAD_GPIOALV5    (PAD_MODE_OUT  | PAD_LEVEL_LOW  | PAD_PULL_UP )				//Reserved

#define CFG_PWRKEY                                  (PAD_GPIO_ALV + 0)

/*------------------------------------------------------------------------------
 *	GPIO LCD (Backlight Enable/PWM)
 */
#define	CFG_IO_LCD_PWR_ENB					((PAD_GPIO_E + 23) | PAD_FUNC_ALT0 )	/* GPIO */
#define	CFG_IO_LCD_DR_PWR_ON				((PAD_GPIO_E + 24) | PAD_FUNC_ALT0 )	/* GPIO */
#define	CFG_IO_LCD_RESET				((PAD_GPIO_E + 22) | PAD_FUNC_ALT0 )	/* GPIO */


#endif	/* __CFG_GPIO_H__ */

