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
#include <config.h>
#include <platform.h>

#include <asm/byteorder.h>
#include <command.h>
#include <malloc.h>
#include <mmc.h>
#include <fat.h>
#include <fs.h>

#include <nx_clkgen.h>
#include <nx_rstcon.h>
#include <nx_clkpwr.h>
#include <nx_uart.h>


#define UART_PL01x_RSR_OE               0x08
#define UART_PL01x_RSR_BE               0x04
#define UART_PL01x_RSR_PE               0x02
#define UART_PL01x_RSR_FE               0x01

#define UART_PL01x_FR_TXFE              0x80
#define UART_PL01x_FR_RXFF              0x40
#define UART_PL01x_FR_TXFF              0x20
#define UART_PL01x_FR_RXFE              0x10
#define UART_PL01x_FR_BUSY              0x08
#define UART_PL01x_FR_TMSK              (UART_PL01x_FR_TXFF + UART_PL01x_FR_BUSY)


#define CFG_SYS_DEBUG_UART_CH			2
#define	CFG_SYS_DEBUG_UART_CLKSRC		0
#define	CFG_SYS_DEBUG_UART_CLKDIV		50 		// (N divider)

//#define CFG_SYS_DEBUG_UART_BAUDRATE		1200
//#define CFG_SYS_DEBUG_UART_BAUDRATE		2400
//#define CFG_SYS_DEBUG_UART_BAUDRATE		4800
//#define CFG_SYS_DEBUG_UART_BAUDRATE		9600
//#define CFG_SYS_DEBUG_UART_BAUDRATE		14400
//#define CFG_SYS_DEBUG_UART_BAUDRATE		19200
//#define CFG_SYS_DEBUG_UART_BAUDRATE		38400
//#define CFG_SYS_DEBUG_UART_BAUDRATE		56000
//#define CFG_SYS_DEBUG_UART_BAUDRATE		57600
#define CFG_SYS_DEBUG_UART_BAUDRATE		115200


static void UART_Reset_FIFO(u32 channel)
{
	while(!(NX_UART_FLAG_TXFE & NX_UART_GetTxRxStatus(channel)));
	while(!(NX_UART_FLAG_RXFE & NX_UART_GetTxRxStatus(channel)))
		NX_UART_GetByte(channel);
	NX_UART_SetFIFOEnb(channel, CFALSE);
	NX_UART_SetFIFOEnb(channel, CTRUE);
}

static int UART_Write_Byte(u32 channel, U8 data)
{
	int ret = 0;

	NX_UART_SendByte(channel, data);

	UART_Reset_FIFO(channel);

	return ret;
}

static int UART_Write(u32 channel, u8 *buffer, u32 size)
{
	int ret = 0;
	int i = 0;

	for (i=0; i<size; i++)
		ret = UART_Write_Byte(channel, *(buffer+i));

	if (ret < 0)
		printf("## \e[31m%s() fail! \e[0m \n", __FUNCTION__);
	else
		ret = size;

	return ret;
}

static u8 UART_Read_Byte(u32 channel, ulong time_out_us)
{
	u8 ret = 0;
	int i = 0;
	ulong time_start_us, time_us = 0;

	register struct NX_UART_RegisterSet	*puart;
	puart = (struct NX_UART_RegisterSet*)NX_UART_GetBaseAddress(channel);

	time_start_us = timer_get_us();

	while (puart->FR & UART_PL01x_FR_RXFE) {
		if (time_out_us > 0) {
			time_us = timer_get_us();
			if ((time_us - time_start_us) > time_out_us) {
				printf("## \e[31m%s() time out ! \e[0m \n", __FUNCTION__);
				break;
			}
		}
	}

	ret = NX_UART_GetByte(channel);

	return ret;
}


static int UART_Read(u32 channel, u8 *buffer, u32 size, ulong time_out_us)
{
	int ret = 0;
	int i = 0;

	for (i= 0; i<size; i++)
		*(buffer+i) = UART_Read_Byte(channel, time_out_us);

	return size;
}

//------------------------------------------------------------------------------
static void UART_Init(u32 channel, u8 Parity)
{
	u32 srcclk;
	u32 clkgennumber;

	NX_CLKPWR_Initialize();
	NX_CLKPWR_SetBaseAddress(NX_CLKPWR_GetPhysicalAddress());
	NX_CLKPWR_OpenModule();

	srcclk = NX_CLKPWR_GetPLLFreq(CFG_SYS_DEBUG_UART_CLKSRC, 24000)/CFG_SYS_DEBUG_UART_CLKDIV;
	clkgennumber = NX_UART_GetClockNumber(channel);

	printf("%s() SRCCLK:%d, BAUDRATE:%d\n", __FUNCTION__, srcclk, CFG_SYS_DEBUG_UART_BAUDRATE);

	NX_CLKGEN_SetBaseAddress(clkgennumber, NX_CLKGEN_GetPhysicalAddress(clkgennumber));
	NX_CLKGEN_SetClockSource(clkgennumber, 0, CFG_SYS_DEBUG_UART_CLKSRC);	
	NX_CLKGEN_SetClockDivisor(clkgennumber, 0, CFG_SYS_DEBUG_UART_CLKDIV);	
	NX_CLKGEN_SetClockDivisorEnable(clkgennumber, CTRUE);

	NX_RSTCON_SetBaseAddress(NX_RSTCON_GetPhysicalAddress());
	NX_RSTCON_SetnRST(NX_UART_GetResetNumber(channel), RSTCON_nENABLE);

	NX_UART_Initialize();
	NX_UART_SetBaseAddress(channel, NX_UART_GetPhysicalAddress(channel));
	NX_UART_OpenModule(channel);

	// UART clock
	NX_UART_SetBRD(channel, NX_UART_MakeBRD(CFG_SYS_DEBUG_UART_BAUDRATE, srcclk));
	NX_UART_SetFBRD(channel, NX_UART_MakeFBRD(CFG_SYS_DEBUG_UART_BAUDRATE, srcclk));

	NX_UART_SetFIFOConfig(channel, CFALSE, 4, 4);

	// Frame Configuration : Data 8 - Parity 0 - Stop 1
	NX_UART_SetFrameConfiguration(channel, Parity, 8, 1);

	//NX_UART_GetFIFOConfig(channel, &GetFIFOEnb, &GetFIFO_Tx, &GetFIFO_Rx);
    //NX_UART_GetFrameConfiguration(channel, &GetParityMode, &GetDataWidth, &GetStopBit);

	// UART Mode : Tx, Rx Only
	NX_UART_SetUARTEnable(channel, CFALSE);
	NX_UART_SetTxDMAMode(channel, CFALSE);
	NX_UART_SetRxDMAMode(channel, CFALSE);
	NX_UART_SetTXEnable(channel, CFALSE);
	NX_UART_SetRXEnable(channel, CFALSE);
	NX_UART_SetSIRMode(channel, CFALSE);
	NX_UART_SetLoopBackMode(channel, CFALSE);
	NX_UART_SetInterruptEnableAll(channel, CFALSE);		
	//----------------------------------------------
	NX_UART_SetSIRMode(channel, CFALSE);
	NX_UART_SetLoopBackMode(channel, CFALSE);
	NX_UART_SetTXEnable(channel, CTRUE);
	NX_UART_SetRXEnable(channel, CTRUE);
	//NX_UART_SetTxDMAMode(channel, CTRUE);
	//NX_UART_SetRxDMAMode(channel, CTRUE);
	NX_UART_SetUARTEnable(channel, CTRUE);

	NX_UART_SetAutoFlowControl(channel, CFALSE);

	//NX_UART_SetInterruptEnable(channel, 1, CTRUE);
	//NX_UART_SetInterruptEnable(channel, 6, CTRUE);	 // Receive Time Out Count Enable
	//NX_UART_SetInterruptEnable(channel, 5, CTRUE);
	//NX_UART_SetInterruptEnable(channel, 4, CTRUE);

	NX_UART_ClearInterruptPending32(channel, 0x7FF);		// Modem Interrupt Clear

	UART_Reset_FIFO(channel);

	// Tx Rx Operation : Polling
	NX_UART_SetInterruptEnableAll(channel, CFALSE);
	NX_UART_ClearInterruptPendingAll(channel);
	return CTRUE;
}

static void UART2_Pad_Init(void)
{
	U32 grp , bit, pad;
	U32 uart_rx = ((PAD_GPIO_D + 16) | PAD_FUNC_ALT1);
	U32 uart_tx = ((PAD_GPIO_D + 20) | PAD_FUNC_ALT1);

	grp = PAD_GET_GROUP(uart_rx);
	bit = PAD_GET_BITNO(uart_rx);
	pad = PAD_GET_FUNC(uart_rx);
	NX_GPIO_SetPadFunction (grp, bit, pad);	// RX
	NX_GPIO_SetOutputEnable(grp, bit, CFALSE);

	grp = PAD_GET_GROUP(uart_tx);
	bit = PAD_GET_BITNO(uart_tx);
	pad = PAD_GET_FUNC(uart_tx);
	NX_GPIO_SetPadFunction (grp, bit, pad);	// TX
	NX_GPIO_SetOutputEnable(grp, bit, CTRUE);
}

static void UART_TEST_RUN(u32 channel)
{
	u8 i = 0;

	for(i = 65; i < 123; i++)  // ASCII Code Output
		NX_UART_SendByte(channel, i);
	NX_UART_SendByte(channel, '\n');
	UART_Reset_FIFO(channel);
}



static int do_uart_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u8 c = 0;

	UART2_Pad_Init();

	UART_Init(CFG_SYS_DEBUG_UART_CH, NX_UART_PARITYMODE_NONE);		// uart init

	UART_TEST_RUN(CFG_SYS_DEBUG_UART_CH);

#if 0
	while (!ctrlc()) {

		c = 0x00;
		c = NX_UART_GetByte(CFG_SYS_DEBUG_UART_CH);

		if (c != 0x0) {

			printf("%c : 0x%x \n", c, c);
			if(c == 'x') {
				printf("\n");
				break;
			}
		}
		//mdelay(100);
	}
#endif

	return 0;
}


U_BOOT_CMD(
	uart_test,	1,	1,	do_uart_test,
	"uart_test",
	" \n"
);


