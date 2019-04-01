/*
 * (C) Copyright 2009 Nexell Co.,
 * jung hyun kim<jhkim@nexell.co.kr>
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

/*-----------------------------------------------------------------------
 * soc headers
 */
#ifndef	__ASM_STUB_PROCESSOR_H__
#include <platform.h>
#endif

/*-----------------------------------------------------------------------
 *  u-boot-2014.07
 */
#define CONFIG_SYS_LDSCRIPT "arch/arm/cpu/slsiap/u-boot.lds"
#define CONFIG_SYS_GENERIC_BOARD

#define	CONFIG_MACH_S5P4418

/*-----------------------------------------------------------------------
 *  System memory Configuration
 */
#define CONFIG_RELOC_TO_TEXT_BASE						/* Relocate u-boot code to TEXT_BASE */

#define	CONFIG_SYS_TEXT_BASE			0x42C00000
#define	CONFIG_SYS_INIT_SP_ADDR			CONFIG_SYS_TEXT_BASE					/* init and run stack pointer */

/* malloc() pool */
#define	CONFIG_MEM_MALLOC_START			0x43000000
#define CONFIG_MEM_MALLOC_LENGTH		32*1024*1024

/* when CONFIG_LCD */
#define CONFIG_FB_ADDR					0x46000000
#define CONFIG_BMP_ADDR					0x47000000

/* Download OFFSET */
#define CONFIG_MEM_LOAD_ADDR			0x48000000

/*-----------------------------------------------------------------------
 *  High Level System Configuration
 */
#undef  CONFIG_USE_IRQ
#define CONFIG_SYS_HZ				1000				/* decrementer freq: 1ms ticks */

#define	CONFIG_SYS_SDRAM_BASE			CFG_MEM_PHY_SYSTEM_BASE		/* board_init_f */
#define	CONFIG_SYS_SDRAM_SIZE			CFG_MEM_PHY_SYSTEM_SIZE

#define CONFIG_NR_DRAM_BANKS			1				/* dram 1 bank num */

#define	CONFIG_SYS_MALLOC_END			(CONFIG_MEM_MALLOC_START + CONFIG_MEM_MALLOC_LENGTH)	/* relocate_code and  board_init_r */
#define CONFIG_SYS_MALLOC_LEN			(CONFIG_MEM_MALLOC_LENGTH - 0x8000)						/* board_init_f, more than 2M for ubifs */

#define CONFIG_SYS_LOAD_ADDR			CONFIG_MEM_LOAD_ADDR		/* kernel load address */

#define CONFIG_SYS_MEMTEST_START		CONFIG_SYS_MALLOC_END		/* memtest works on */
#define CONFIG_SYS_MEMTEST_END			(CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_SDRAM_SIZE)

/*-----------------------------------------------------------------------
 *  System initialize options (board_init_f)
 */
#define CONFIG_ARCH_CPU_INIT							/* board_init_f->init_sequence, call arch_cpu_init */
#define	CONFIG_BOARD_EARLY_INIT_F						/* board_init_f->init_sequence, call board_early_init_f */
#define	CONFIG_BOARD_LATE_INIT							/* board_init_r, call board_early_init_f */
//#define	CONFIG_DISPLAY_CPUINFO						[> board_init_f->init_sequence, call print_cpuinfo <]
#define	CONFIG_SYS_DCACHE_OFF							/* board_init_f, CONFIG_SYS_ICACHE_OFF */
//#define	CONFIG_ARCH_MISC_INIT						[> board_init_r, call arch_misc_init <]
//#define	CONFIG_SYS_ICACHE_OFF

#define CONFIG_MMU_ENABLE
#ifdef  CONFIG_MMU_ENABLE
#undef  CONFIG_SYS_DCACHE_OFF
#endif

/*-----------------------------------------------------------------------
 *	U-Boot default cmd
 */
#define CONFIG_CMD_MEMORY   /* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_NET      /* bootp, tftpboot, rarpboot    */
#define CONFIG_CMD_RUN      /* run command in env variable  */
#define CONFIG_CMD_SAVEENV  /* saveenv          */
#define CONFIG_CMD_SOURCE   /* "source" command support */
#define CONFIG_CMD_BOOTD	/* "boot" command support */
#define	CONFIG_CMD_MEMTEST

/*-----------------------------------------------------------------------
 *	U-Boot Environments
 */
/* refer to common/env_common.c	*/
#define CONFIG_BOOTDELAY	   			0
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_BOOTFILE					"uImage"  		/* File to load	*/

#define CONFIG_BOOTCOMMAND "fatload mmc 0 48000000 uImage;bootm 48000000"

/*-----------------------------------------------------------------------
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_PROMPT				"s5p4418_slt# "		/* Monitor Command Prompt   */
#define CONFIG_SYS_LONGHELP							/* undef to save memory	   */
#define CONFIG_SYS_CBSIZE				1024			/* Console I/O Buffer Size  */
#define CONFIG_SYS_PBSIZE				(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) 	/* Print Buffer Size */
#define CONFIG_SYS_MAXARGS				16			/* max number of command args   */
#define CONFIG_SYS_BARGSIZE				CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size    */

/*-----------------------------------------------------------------------
 * allow to overwrite serial and ethaddr
 */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_HUSH_PARSER			/* use "hush" command parser	*/
#ifdef 	CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#endif

/*-----------------------------------------------------------------------
 * Etc Command definition
 */
#define	CONFIG_CMD_BDI					/* board info	*/
#define	CONFIG_CMD_IMI					/* image info	*/
#define	CONFIG_CMD_MEMORY
#define	CONFIG_CMD_RUN					/* run commands in an environment variable	*/
#define CONFIG_CMDLINE_EDITING			/* add command line history	*/
#define	CONFIG_CMDLINE_TAG				/* use bootargs commandline */
//#define CONFIG_SETUP_MEMORY_TAGS
//#define CONFIG_INITRD_TAG
//#define CONFIG_SERIAL_TAG
//#define CONFIG_REVISION_TAG

#undef	CONFIG_BOOTM_NETBSD
#undef	CONFIG_BOOTM_RTEMS
#undef  CONFIG_BOOTM_VXWORKS
#undef  CONFIG_CMD_IMPORTENV
#undef  CONFIG_CMD_EXPORTENV
// #undef	CONFIG_GZIP

/*-----------------------------------------------------------------------
 * serial console configuration
 */
#define CONFIG_PL011_SERIAL
#define CONFIG_CONS_INDEX				CFG_UART_DEBUG_CH
#define CONFIG_PL011_CLOCK				CFG_UART_CLKGEN_CLOCK_HZ
#define CONFIG_PL01x_PORTS				{ (void *)IO_ADDRESS(PHY_BASEADDR_UART0), 	\
										  (void *)IO_ADDRESS(PHY_BASEADDR_UART1), 	\
										  (void *)IO_ADDRESS(PHY_BASEADDR_UART2), 	\
										  (void *)IO_ADDRESS(PHY_BASEADDR_UART3) }

#define CONFIG_BAUDRATE					CFG_UART_DEBUG_BAUDRATE
#define CONFIG_SYS_BAUDRATE_TABLE		{ 9600, 19200, 38400, 57600, 115200 }
#define CONFIG_PL011_SERIAL_FLUSH_ON_INIT

/*-----------------------------------------------------------------------
 * NOR FLASH
 */
#define	CONFIG_SYS_NO_FLASH

/*-----------------------------------------------------------------------
 * USB Host / Gadget
 *
 * command
 *
 * #> usb start
 * #> fatls   usb 0 "directory"
 * #> fatload usb 0  0x.....	"file"
 */
//#define CONFIG_CMD_USB
#if defined(CONFIG_CMD_USB)
	#define CONFIG_USB_EHCI_SYNOPSYS
	#define CONFIG_USB_EHCI_MODE
	//#define CONFIG_USB_HSIC_MODE
	#define CONFIG_USB_STORAGE
	#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS 2

	#undef  CONFIG_PREBOOT
	#define CONFIG_PREBOOT						"usb start"
#endif

/* Gadget */
#define	CONFIG_USB_GADGET
#if defined(CONFIG_USB_GADGET)
	#define CONFIG_NXP_USBDOWN
	#define CONFIG_NXP_DWC_OTG
	#define CONFIG_NXP_DWC_OTG_PHY
#endif

/*-----------------------------------------------------------------------
 * SD/MMC
 * 	#> mmcinfo			-> get current device mmc info or detect mmc device
 * 	#> mmc rescan		-> rescan mmc device
 * 	#> mmc dev 'num'	-> set current sdhc device for mmcinfo or mmc rescan
 * 						  (ex. "mmc dev 0" or "mmc dev 1")
 *
 * #> fatls   mmc 0 "directory"
 * #> fatload mmc 0  0x.....	"file"
 *
 */
#define	CONFIG_CMD_MMC
#define CONFIG_ENV_IS_IN_MMC

#if defined(CONFIG_CMD_MMC)

	#define	CONFIG_MMC
	#define CONFIG_GENERIC_MMC
	#define HAVE_BLOCK_DEVICE

	#define CONFIG_MMC0_ATTACH		TRUE	/* 0 = MMC0 : Boot (eMMC)*/
	#define CONFIG_MMC1_ATTACH		TRUE	/* 1 = MMC1 : External  SD */
	#define CONFIG_MMC2_ATTACH		FALSE	/* 2 = MMC2 :*/

	#define CONFIG_MMC1_CLOCK			50000000
	#define CONFIG_MMC1_CLK_DELAY       DW_MMC_DRIVE_DELAY(0) | DW_MMC_SAMPLE_DELAY(0) | DW_MMC_DRIVE_PHASE(2)| DW_MMC_SAMPLE_PHASE(1)

	#define CONFIG_MMC0_CLOCK			50000000
	#define CONFIG_MMC0_CLK_DELAY       DW_MMC_DRIVE_DELAY(0) | DW_MMC_SAMPLE_DELAY(0) | DW_MMC_DRIVE_PHASE(2)| DW_MMC_SAMPLE_PHASE(1)
	#define CONFIG_MMC0_BUS_WIDTH       4
	#define CONFIG_MMC0_TRANS_MODE      0 // 1 : DDR_MODE, 0: SDR_MODE

	#define CONFIG_DWMMC
	#define CONFIG_NXP_DWMMC
	#define CONFIG_MMC_PARTITIONS
	#define CONFIG_CMD_MMC_UPDATE
	#define CONFIG_SYS_MMC_BOOT_DEV		(0)		/* BOOT MMC DEVICE NUM */

	#if defined(CONFIG_ENV_IS_IN_MMC)
	//#define	CONFIG_ENV_OFFSET			512*1024			/* 0x00080000 */
	#define	CONFIG_ENV_OFFSET			476*1024			/* 0x00077000*/
	#define CONFIG_ENV_SIZE			32*1024				/* N block size (512Byte Per Block)  */
	#define CONFIG_ENV_RANGE		CONFIG_ENV_SIZE * 2	/* avoid bad block */
	#define CONFIG_SYS_MMC_ENV_DEV		CONFIG_SYS_MMC_BOOT_DEV
	#endif
#endif

/*-----------------------------------------------------------------------
 *	GPIO LIBRARY
 */
#define CONFIG_GPIOLIB_NXP

/*-----------------------------------------------------------------------
 * Default environment organization
 */
#if !defined(CONFIG_ENV_IS_IN_MMC) && !defined(CONFIG_ENV_IS_IN_NAND) &&	\
	!defined(CONFIG_ENV_IS_IN_FLASH) && !defined(CONFIG_ENV_IS_IN_EEPROM)
	#define CONFIG_ENV_IS_NOWHERE						/* default: CONFIG_ENV_IS_NOWHERE */
	#define	CONFIG_ENV_OFFSET			  1024
	#define CONFIG_ENV_SIZE				4*1024		/* env size */
	#undef	CONFIG_CMD_IMLS								/* imls - list all images found in flash, default enable so disable */
#endif

/*-----------------------------------------------------------------------
 * FAT Partition
 */
#if defined(CONFIG_MMC) || defined(CONFIG_CMD_USB)
	#define CONFIG_DOS_PARTITION

	#define CONFIG_CMD_FAT
	#define CONFIG_FS_FAT
	#define CONFIG_FAT_WRITE

	#define CONFIG_CMD_EXT4
	#define CONFIG_CMD_EXT4_WRITE
	#define CONFIG_FS_EXT4
	#define CONFIG_EXT4_WRITE
#endif


/*-----------------------------------------------------------------------
 * FASTBOOT
 */
#define CONFIG_FASTBOOT

#if defined(CONFIG_FASTBOOT) & defined(CONFIG_USB_GADGET)
#define CFG_FASTBOOT_TRANSFER_BUFFER        CONFIG_MEM_LOAD_ADDR
#define CFG_FASTBOOT_TRANSFER_BUFFER_OFFSET \
	(CONFIG_MEM_LOAD_ADDR - CFG_MEM_PHY_SYSTEM_BASE)
#define CFG_FASTBOOT_TRANSFER_BUFFER_SIZE \
	(CFG_MEM_PHY_SYSTEM_SIZE - CFG_FASTBOOT_TRANSFER_BUFFER_OFFSET)

#define	FASTBOOT_PARTS_DEFAULT		\
			"flash=mmc,0:2ndboot:2nd:0x200,0x7e00;"\
			"flash=mmc,0:bootloader:boot:0x8000,0x77000;" \
			"flash=mmc,0:kernel:raw:0x100000,0x500000" \
			"flash=mmc,0:ramdisk:raw:0x700000,0x3000000;" \
			"flash=mmc,0:userdata:ext4:0x3700000,0x0;"
#endif


/*-----------------------------------------------------------------------
 * Debug message
 */
//#define DEBUG							/* u-boot debug macro, nand, ethernet,... */

#endif /* __CONFIG_H__ */

