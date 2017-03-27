/*
 * (C) Copyright 2016 Nexell Co.,
 * jong sin park<pjsin865@nexell.co.kr>
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
#include <pwm.h>

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

typedef u8	BYTE;
typedef u8	*LPBYTE;
typedef u16	USHORT;
typedef u16	WORD;
typedef u32	DWORD;

static void UART_Status_FIFO(u32 channel)
{
	printf(" RTS : %4s \r\n", (NX_UART_GetCR(channel) & (1UL << 11)) ? "LOW" : "HIGH");
	printf(" CTS : %4s \r\n", (NX_UART_GetFR(channel) & (1UL << 0)) ? "LOW" : "HIGH");
	printf(" UARTFR (Flag Register) : %8X \r\n", NX_UART_GetFR(channel));
	printf("-------------------------------------------------------- \r\n");
	printf(" TXFE (Transmit FIFO Empty): %10s \r\n", NX_UART_GetFR(channel) & NX_UART_FLAG_TXFE ? "EMPTY" : "NOT EMPTY" );
	printf(" RXFF (Receive	FIFO Full): %10s \r\n", NX_UART_GetFR(channel) & NX_UART_FLAG_RXFF ? "FULL" : "NOT FULL" );
	printf(" TXFF (Transmit FIFO Full): %10s \r\n", NX_UART_GetFR(channel) & NX_UART_FLAG_TXFF ? "FULL" : "NOT FULL");
	printf(" RXFE (Receive	FIFO Empty): %10s \r\n", NX_UART_GetFR(channel) & NX_UART_FLAG_RXFE ? "EMPTY" : "NOT EMPTY");
	printf("-------------------------------------------------------- \r\n");
	printf(" RXRIS (Raw Receive  Interrupt Status): %11s \r\n", NX_UART_GetRIS(channel) & (1UL << 6) ? "PENDING" : "NOT PENDING");
	printf(" TXRIS (Raw Transmit Interrupt Status): %11s \r\n", NX_UART_GetRIS(channel) & (1UL << 5) ? "PENDING" : "NOT PENDING");
	printf(" RXRIS (Raw Receive  Interrupt Status): %11s \r\n", NX_UART_GetRIS(channel) & (1UL << 4) ? "PENDING" : "NOT PENDING");
	printf(" CTSRMIS  (Raw CTS	 Interrupt Status): %11s \r\n", NX_UART_GetRIS(channel) & (1UL << 1) ? "PENDING" : "NOT PENDING");
	printf("-------------------------------------------------------- \r\n");
	printf(" RXMIS (Receive  Interrupt Status): %11s \r\n", NX_UART_GetInterruptPending(channel, 6) ? "PENDING" : "NOT PENDING");
	printf(" TXMIS (Transmit Interrupt Status): %11s \r\n", NX_UART_GetInterruptPending(channel, 5) ? "PENDING" : "NOT PENDING");
	printf(" RXMIS (Receive  Interrupt Status): %11s \r\n", NX_UART_GetInterruptPending(channel, 4) ? "PENDING" : "NOT PENDING");
	printf(" CTSRMMIS (CTS	 Interrupt Status): %11s \r\n", NX_UART_GetInterruptPending(channel, 1) ? "PENDING" : "NOT PENDING");
	printf("-------------------------------------------------------- \r\n");
	//NX_MSG(" UART%d - Get  Byte : %d \r\n", Subchannel, NX_UART_GetByte(Subchannel));
	//NX_MSG(" UART%d - Get  Byte : %d \r\n", channel, NX_UART_GetByte(channel));
}

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

//------------------------------------------------------------------------------
static void UART_Init(u32 channel, u8 Parity)
{
	u32 srcclk;
	u32 clkgennumber;

	UART2_Pad_Init();

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

}

static void PWM_Disable(int channel)
{
	pwm_disable(channel);
}

static void PWM_init(int channel)
{
	pwm_init(channel, 0, 0);
	pwm_config(channel,TO_DUTY_NS(50, 8000000),TO_PERIOD_NS(8000000));
	pwm_enable(channel);
}

static int NXCBL_CHECK_VERSION(u32 channel, char *version)
{
	int ret = 0;
	u8 buffer[10];

	memset(buffer, 0x00, sizeof(u8)*10);

	UART_Read(channel, buffer, 2, 1000000);
	buffer[9] = 0;

	printf("nxc100 version : %s \n\n", buffer);

	if (strncmp(buffer, version, 2) != 0) {
		printf("## \e[31m%s():%d Version fail\e[0m : bin_version:%s, read_version:%s \n", __FUNCTION__, __LINE__, version, buffer);
		ret = -1;
	}

	return ret;
}

static void NXCBL_RESET(void)
{
	U32 grp , bit, pad;
	U32 gpio = (PAD_GPIO_E + 15);//((PAD_GPIO_C + 4) | PAD_FUNC_ALT1);

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);
	pad = PAD_GET_FUNC(gpio);

	NX_GPIO_SetPadFunction(grp, bit, pad);
	NX_GPIO_SetOutputEnable(grp, bit, CTRUE);

	NX_GPIO_SetOutputValue(grp, bit, CFALSE);

	mdelay(100);

	NX_GPIO_SetOutputValue(grp, bit, CTRUE);

	mdelay(100);
}

static void NXCBL_BOOTMODE(int mode)
{
	U32 grp , bit, pad;
	U32 gpio = (PAD_GPIO_C + 15);//((PAD_GPIO_C + 19) | PAD_FUNC_ALT1);

	grp = PAD_GET_GROUP(gpio);
	bit = PAD_GET_BITNO(gpio);
	pad = PAD_GET_FUNC(gpio);

	NX_GPIO_SetPadFunction(grp, bit, pad);
	NX_GPIO_SetOutputEnable(grp, bit, CTRUE);

	if (mode)
		NX_GPIO_SetOutputValue(grp, bit, CTRUE);
	else
		NX_GPIO_SetOutputValue(grp, bit, CFALSE);
}

static int do_nxc100_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	printf("\n");

	UART_Init(CFG_SYS_DEBUG_UART_CH, NX_UART_PARITYMODE_NONE);		// uart init
	PWM_init(1);				// pwm init
	NXCBL_BOOTMODE(0);			// NXC100 Boot mode
	NXCBL_RESET();				// NXC100 Reset

	printf("bin version   : %s \n", "$1");

	if(0 == NXCBL_CHECK_VERSION(CFG_SYS_DEBUG_UART_CH, "$1")) {
		printf("## NXC100 Normal Booting \n");
	} else {
		printf("## \e[31m%s():%d fail !!! \e[0m\n", __FUNCTION__, __LINE__);
	}

	return 0;
}


U_BOOT_CMD(
	nxc100,	3,	1,	do_nxc100_check,
	"nxc100 check",
	"       nxc100 : Normal Booting \n"
);


