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
#define CONFIG_RELOC_TO_TEXT_BASE												/* Relocate u-boot code to TEXT_BASE */

#define	CONFIG_SYS_TEXT_BASE			0x42C00000
#define	CONFIG_SYS_INIT_SP_ADDR			CONFIG_SYS_TEXT_BASE					/* init and run stack pointer */

/* malloc() pool */
#define	CONFIG_MEM_MALLOC_START			0x43000000
#define CONFIG_MEM_MALLOC_LENGTH		32*1024*1024

/* when CONFIG_LCD */
#define CONFIG_FB_ADDR_46000000					0x46000000
#define CONFIG_FB_ADDR                  0x6731e000
#define CONFIG_BMP_ADDR					0x47000000

/* Download OFFSET */
#define CONFIG_MEM_LOAD_ADDR			0x48000000

/*-----------------------------------------------------------------------
 *  High Level System Configuration
 */
#undef  CONFIG_USE_IRQ		     												/* Not used: not need IRQ/FIQ stuff	*/
#define CONFIG_SYS_HZ	   				1000									/* decrementer freq: 1ms ticks */

#define	CONFIG_SYS_SDRAM_BASE			CFG_MEM_PHY_SYSTEM_BASE					/* board_init_f */
#define	CONFIG_SYS_SDRAM_SIZE			CFG_MEM_PHY_SYSTEM_SIZE

#define CONFIG_NR_DRAM_BANKS	   		1										/* dram 1 bank num */

#define	CONFIG_SYS_MALLOC_END			(CONFIG_MEM_MALLOC_START + CONFIG_MEM_MALLOC_LENGTH)	/* relocate_code and  board_init_r */
#define CONFIG_SYS_MALLOC_LEN			(CONFIG_MEM_MALLOC_LENGTH - 0x8000)						/* board_init_f, more than 2M for ubifs */

#define CONFIG_SYS_LOAD_ADDR			CONFIG_MEM_LOAD_ADDR					/* kernel load address */

#define CONFIG_SYS_MEMTEST_START		CONFIG_SYS_MALLOC_END					/* memtest works on */
#define CONFIG_SYS_MEMTEST_END			(CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_SDRAM_SIZE)

/*-----------------------------------------------------------------------
 *  System initialize options (board_init_f)
 */
#define CONFIG_ARCH_CPU_INIT													/* board_init_f->init_sequence, call arch_cpu_init */
#define	CONFIG_BOARD_EARLY_INIT_F												/* board_init_f->init_sequence, call board_early_init_f */
#define	CONFIG_BOARD_LATE_INIT													/* board_init_r, call board_early_init_f */
//#define	CONFIG_DISPLAY_CPUINFO													/* board_init_f->init_sequence, call print_cpuinfo */
#define	CONFIG_SYS_DCACHE_OFF													/* board_init_f, CONFIG_SYS_ICACHE_OFF */
//#define	CONFIG_ARCH_MISC_INIT													/* board_init_r, call arch_misc_init */
//#define	CONFIG_SYS_ICACHE_OFF

#define CONFIG_MMU_ENABLE
#ifdef  CONFIG_MMU_ENABLE
#undef  CONFIG_SYS_DCACHE_OFF
#endif

/*-----------------------------------------------------------------------
 *	U-Boot default cmd
 */
#define CONFIG_CMD_MEMORY   /* md mm nm mw cp cmp crc base loop mtest */
//#define CONFIG_CMD_NET      /* bootp, tftpboot, rarpboot    */
//#define CONFIG_CMD_RUN      /* run command in env variable  */
#define CONFIG_CMD_SAVEENV  /* saveenv          */
//#define CONFIG_CMD_SOURCE   /* "source" command support */
#define CONFIG_CMD_BOOTD	/* "boot" command support */

/*-----------------------------------------------------------------------
 *	U-Boot Environments
 */
/* refer to common/env_common.c	*/
#define CONFIG_BOOTDELAY	   			0
#define CONFIG_ZERO_BOOTDELAY_CHECK
//#define CONFIG_ETHADDR		   			00:e2:1c:ba:e8:60
//#define CONFIG_NETMASK		   			255.255.255.0
//#define CONFIG_IPADDR					192.168.1.165
//#define CONFIG_SERVERIP					192.168.1.164
//#define CONFIG_GATEWAYIP				192.168.1.254
//#define CONFIG_BOOTFILE					"uImage"  		/* File to load	*/

//#define CONFIG_BOOTCOMMAND "ext4load mmc 0:1 0x48000000 uImage;ext4load mmc 0:1 0x49000000 root.img.gz;bootm 0x48000000"
//#define CONFIG_BOOTCOMMAND "ext4load mmc 0:1 0x40008000 Image;goimage 0x40008000"
#define CONFIG_BOOTCOMMAND "ext4load mmc 0:1 0x40008000 Image;ext4load mmc 0:1 0x49000000 root.img.gz;goimage 0x40008000"

/*-----------------------------------------------------------------------
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_PROMPT				"mowgli# "     										/* Monitor Command Prompt   */
#define CONFIG_SYS_LONGHELP				       												/* undef to save memory	   */
#define CONFIG_SYS_CBSIZE		   		1024		   											/* Console I/O Buffer Size  */
#define CONFIG_SYS_PBSIZE		   		(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) 	/* Print Buffer Size */
#define CONFIG_SYS_MAXARGS			   	16		       										/* max number of command args   */
#define CONFIG_SYS_BARGSIZE			   	CONFIG_SYS_CBSIZE	       							/* Boot Argument Buffer Size    */

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
//#define	CONFIG_CMD_BDI					/* board info	*/
//#define	CONFIG_CMD_IMI					/* image info	*/
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
// #undef	CONFIG_GZIP

/*-----------------------------------------------------------------------
 * serial console configuration
 */
#define CONFIG_PL011_SERIAL
#define CONFIG_CONS_INDEX				CFG_UART_DEBUG_CH
#define CONFIG_PL011_CLOCK				CFG_UART_CLKGEN_CLOCK_HZ
#define CONFIG_PL01x_PORTS				{ (void *)IO_ADDRESS(PHY_BASEADDR_UART0), 	\
										  (void *)IO_ADDRESS(PHY_BASEADDR_UART1) }

#define CONFIG_BAUDRATE		   			CFG_UART_DEBUG_BAUDRATE
#define CONFIG_SYS_BAUDRATE_TABLE	   	{ 9600, 19200, 38400, 57600, 115200 }
#define CONFIG_PL011_SERIAL_FLUSH_ON_INIT

#define	CONFIG_SYS_NO_FLASH

/*-----------------------------------------------------------------------
 * SPI
 */

#if defined  (CONFIG_SPI)
    #if defined (CONFIG_SPI_MODULE_0)
		#define CONFIG_SPI_MODULE_0_SOURCE_CLOCK    CFG_SPI0_SRC_CLK
        #define CONFIG_SPI_MODULE_0_CLOCK           CFG_SPI0_OUT_CLK
        #define CONFIG_SPI_MODULE_0_EEPROM          CONFIG_SPI0_TYPE    /* 1: EEPROM, 0: SPI device */
    #endif
    #if defined (CONFIG_SPI_MODULE_1)
		#define CONFIG_SPI_MODULE_1_SOURCE_CLOCK    CFG_SPI1_SRC_CLK
        #define CONFIG_SPI_MODULE_1_CLOCK           CFG_SPI0_OUT_CLK
        #define CONFIG_SPI_MODULE_1_EEPROM          CONFIG_SPI1_TYPE    /* 1: EEPROM, 0: SPI device */
    #endif
    #if defined (CONFIG_SPI_MODULE_2)
		#define CONFIG_SPI_MODULE_2_SOURCE_CLOCK    CFG_SPI2_SRC_CLK
        #define CONFIG_SPI_MODULE_2_CLOCK           CFG_SPI0_OUT_CLK
        #define CONFIG_SPI_MODULE_2_EEPROM          CONFIG_SPI2_TYPE    /* 1: EEPROM, 0: SPI device */
    #endif
#endif

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
 * PMIC
 */

#define CONFIG_PMIC
	#if defined(CONFIG_PMIC)
		#define CONFIG_CMD_I2C
		#define CONFIG_PMIC_I2C
		#define CONFIG_PMIC_I2C_BUS							I2C_3

		#define CONFIG_PMIC_CHARGING_PATH_ADP               (0) // Support only VADP. Do not supported USB ADP.
		#define CONFIG_PMIC_CHARGING_PATH_UBC               (1) // Support only VUSB. (USB connector - USB ADP & PC)
		#define CONFIG_PMIC_CHARGING_PATH_ADP_UBC           (2) // Using VADP, VUSB power path. Separated power path.
		#define CONFIG_PMIC_CHARGING_PATH_ADP_UBC_LINKED    (3) // Using VADP, VUSB power path. Linked power path.

		#define CONFIG_POWER
		#define CONFIG_POWER_I2C
		#define CONFIG_POWER_BATTERY
		#define CONFIG_POWER_FG
		#define CONFIG_POWER_MUIC

		#define CONFIG_PMIC_NXE2000
	#endif

	#if defined(CONFIG_PMIC_NXE2000)
		#define CONFIG_POWER_NXE2000
		#define CONFIG_POWER_BATTERY_NXE2000
		#define CONFIG_POWER_MUIC_NXE2000
		#define CONFIG_POWER_FG_NXE2000

		#define CONFIG_PMIC_CHARGING_PATH	CONFIG_PMIC_CHARGING_PATH_ADP_UBC

		#define	CFG_IO_I2C0_SCL	((PAD_GPIO_E + 14) | PAD_FUNC_ALT0)
		#define	CFG_IO_I2C0_SDA	((PAD_GPIO_E + 15) | PAD_FUNC_ALT0)

		#define CONFIG_SW_UBC_DETECT							/* need with CONFIG_FASTBOOT. */

		#define CONFIG_HAVE_BATTERY

		//#define CONFIG_ENABLE_INIT_VOLTAGE						/* enable set voltage(ARM, CORE)  */

		//#define CONFIG_PMIC_REG_DUMP
	#endif

	#if defined(CONFIG_HAVE_BATTERY)
		//#define CONFIG_PMIC_VOLTAGE_CHECK_WITH_CHARGE
		//#define CONFIG_POWER_BATTERY_SMALL
			#ifndef CONFIG_POWER_BATTERY_SMALL
			//#define CONFIG_BAT_CHECK
			#define CONFIG_NXP_RTC_USE
			#endif
	#endif


/*-----------------------------------------------------------------------
 * I2C
 *
 * probe
 * 	#> i2c probe
 *
 * speed
 * 	#> i2c speed xxxxxx
 *
 * select bus
 * 	#> i2c dev n
 *
 * write
 * 	#> i2c mw 0x30 0xRR 0xDD 1
 *	- 0x30 = slave, 0xRR = register, 0xDD = data, 1 = write length
 *
 * read
 * 	#> i2c md 0x30 0xRR 1
 *	- 0x30 = slave, 0xRR = register, 1 = read length
 *
 */
#define	CONFIG_CMD_I2C
#if defined(CONFIG_CMD_I2C)
	#define	CONFIG_HARD_I2C
	#define CONFIG_I2C_MULTI_BUS

	#define CONFIG_I2C_GPIO_MODE							/* gpio i2c */
	#define	CONFIG_SYS_I2C_SPEED		100000				/* default speed, 100 khz */

	#define	CONFIG_I2C0_NEXELL								/* 0 = i2c 0 */
	#define	CONFIG_I2C0_NO_STOP				1				/* when tx end, 0= generate stop signal , 1: skip stop signal */

	#define	CONFIG_I2C1_NEXELL								/* 1 = i2c 1 */
	#define	CONFIG_I2C1_NO_STOP				0				/* when tx end, 0= generate stop signal , 1: skip stop signal */

	#define	CONFIG_I2C2_NEXELL								/* 2 = i2c 2 */
	#define	CONFIG_I2C2_NO_STOP				0				/* when tx end, 0= generate stop signal , 1: skip stop signal */

	#define	CONFIG_I2C3_NEXELL								/* 3 = i2c 3 */
	#define	CONFIG_I2C3_NO_STOP				0				/* when tx end, 0= generate stop signal , 1: skip stop signal */
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

	#define CONFIG_MMC0_ATTACH      	TRUE    /* 0 = MMC0 : BOOT */
	#define CONFIG_MMC1_ATTACH      	FALSE    /* 1 = MMC1 : none*/
	#define CONFIG_MMC2_ATTACH      	TRUE    /* 2 = MMC2 : External */

	#define CONFIG_MMC0_CLOCK			50000000
	#define CONFIG_MMC0_CLK_DELAY       DW_MMC_DRIVE_DELAY(0) | DW_MMC_SAMPLE_DELAY(0x1c) | DW_MMC_DRIVE_PHASE(2)| DW_MMC_SAMPLE_PHASE(1)

	#define CONFIG_MMC2_CLOCK			50000000
	#define CONFIG_MMC2_CLK_DELAY       DW_MMC_DRIVE_DELAY(0) | DW_MMC_SAMPLE_DELAY(0x1c) | DW_MMC_DRIVE_PHASE(2)| DW_MMC_SAMPLE_PHASE(1)

	#define CONFIG_DWMMC
	#define CONFIG_NXP_DWMMC
	#define CONFIG_MMC_PARTITIONS
	#define CONFIG_CMD_MMC_UPDATE
	#define CONFIG_SYS_MMC_BOOT_DEV  	(0)		/* BOOT MMC DEVICE NUM */

	#if defined(CONFIG_ENV_IS_IN_MMC)
	#define	CONFIG_ENV_OFFSET			512*1024				/* 0x00080000 */
	#define CONFIG_ENV_SIZE           	32*1024					/* N block size (512Byte Per Block)  */
	#define CONFIG_ENV_RANGE			CONFIG_ENV_SIZE * 2 	/* avoid bad block */
	#define CONFIG_SYS_MMC_ENV_DEV  	CONFIG_SYS_MMC_BOOT_DEV
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
	#define	CONFIG_ENV_OFFSET			  	  1024
	#define CONFIG_ENV_SIZE           		4*1024		/* env size */
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
 * UBIFS Partition
 * #>ubi part extra
 * #>ubi info
 * #>ubi info l
 * #>ubifsmount extra
 * #>ubifsls
 */
//#define CONFIG_CMD_UBIFS

#if defined(CONFIG_CMD_UBIFS)
	#define CONFIG_RBTREE
	#define	CONFIG_CMD_UBI
	#define	CONFIG_LZO

	//#define	CONFIG_UBIFS_FS_DEBUG
	#if defined(CONFIG_UBIFS_FS_DEBUG)
	#define	CONFIG_UBIFS_FS_DEBUG_MSG_LVL	1	/* For ubifs debug message = 0 ~ 3 */
	#endif
#endif

/*-----------------------------------------------------------------------
 * FASTBOOT
 */
#define CONFIG_FASTBOOT

#if defined(CONFIG_FASTBOOT) & defined(CONFIG_USB_GADGET)
#define CFG_FASTBOOT_TRANSFER_BUFFER        CONFIG_MEM_LOAD_ADDR
#define CFG_FASTBOOT_TRANSFER_BUFFER_SIZE	(CFG_MEM_PHY_SYSTEM_SIZE - CFG_FASTBOOT_TRANSFER_BUFFER)

#define	FASTBOOT_PARTS_DEFAULT		\
        "flash=mmc,0:2ndboot:2nd:0x200,0x8e00;" \
        "flash=mmc,0:bootloader:boot:0x8000,0x77000;" \
		"flash=mmc,0:boot:ext4:0x000100000,0x004000000;" \
		"flash=mmc,0:system:ext4:0x04100000,0x1F400000" \
		"flash=mmc,0:cache:ext4:0x23600000,0x1AC00000;" \
        "flash=mmc,0:misc:emmc:0x3E300000,0x00800000;" \
		"flash=mmc,0:recovery:emmc:0x3EC00000,0x01600000;" \
		"flash=mmc,0:reserv1:emmc:0x40300000,0x0010000;" \
		"flash=mmc,0:reserv2:emmc:0x40400000,0x00100000;" \
		"flash=mmc,0:reserv3:emmc:0x40500000,0x00100000;" \
		"flash=mmc,0:vendor:ext4:0x40600000,0x1F400000;" \
		"flash=mmc,0:ota:ext4:0x5FA00000,0x1F400000;" \
		"flash=mmc,0:reserv4:emmc:0x7EF00000,0x08000000;" \
		"flash=mmc,0:reserv5:emmc:0x87000000,0x04000000;"\
		"flash=mmc,0:reserv6:emmc:0x8B100000,0x00A00000;" \
		"flash=mmc,0:reserv7:emmc:0x8BC00000,0x00A00000;" \
		"flash=mmc,0:reserv8:emmc:0x8C700000,0x00A00000;" \
		"flash=mmc,0:reserv9:emmc:0x8D200000,0x00A00000;" \
		"flash=mmc,0:reserv10:emmc:0x8DD00000,0x00A00000;" \
		"flash=mmc,0:userdata:ext4:0x8E700000,0x0;"
#endif

/*-----------------------------------------------------------------------
 * Logo command
 */
#define CONFIG_DISPLAY_OUT

#define CONFIG_LOGO_DEVICE_MMC

#if defined(CONFIG_LOGO_DEVICE_MMC) && defined(CONFIG_LOGO_DEVICE_NAND)
#error "Duplicated config for logo device!!!"
#endif

#if	defined(CONFIG_DISPLAY_OUT)
	#define	CONFIG_PWM			/* backlight */
	/* display out device */
	#define	CONFIG_DISPLAY_OUT_RGB

	/* display logo */
	#define CONFIG_LOGO_NEXELL				/* Draw loaded bmp file to FB or fill FB */
//	#define CONFIG_CMD_LOGO_LOAD

	/* Logo command: board.c */
	#if defined(CONFIG_LOGO_DEVICE_NAND)
	/* From NAND */
    #define CONFIG_CMD_LOGO_WALLPAPERS "ext4load mmc 0:1 0x47000000 logo.bmp; drawbmp 0x47000000"
    #define CONFIG_CMD_LOGO_BATTERY "ext4load mmc 0:1 0x47000000 battery.bmp; drawbmp 0x47000000"
    #define CONFIG_CMD_LOGO_UPDATE "ext4load mmc 0:1 0x47000000 update.bmp; drawbmp 0x47000000"
	#else
	/* From MMC */
    #define CONFIG_CMD_LOGO_WALLPAPERS "ext4load mmc 0:1 0x47000000 logo.bmp; drawbmp 0x47000000"
    #define CONFIG_CMD_LOGO_RECOVERY "ext4load mmc 0:1 0x47000000 recover-on-android.bmp; drawbmp 0x47000000"
    #define CONFIG_CMD_LOGO_BATTERY "ext4load mmc 0:1 0x47000000 battery.bmp; drawbmp 0x47000000"
    #define CONFIG_CMD_LOGO_UPDATE "ext4load mmc 0:1 0x47000000 update.bmp; drawbmp 0x47000000"
	#endif
#endif


/*-----------------------------------------------------------------------
 * Recover boot
 */
#define	CONFIG_RECOVERY_BOOT
#if defined (CONFIG_RECOVERY_BOOT)
	#define CONFIG_CMD_RECOVERY_BOOT "setenv bootargs console=ttyAMA0,115200n8 androidboot.hardware=mowgli androidboot.console=ttyAMA0 androidboot.serialno=12345 initrd=0x49000000,0x200000 init=/init quite;ext4load mmc 0:1 0x48000000 uImage;ext4load mmc 0:1 0x49000000 ramdisk-recovery.img;bootm 0x48000000"
#endif

/*-----------------------------------------------------------------------
 * POWER_OFF boot
 */
#define	CONFIG_POWER_OFF_BOOT

/*-----------------------------------------------------------------------
 * Debug message
 */
//#define DEBUG							/* u-boot debug macro, nand, ethernet,... */
//#define CONFIG_PROTOTYPE_DEBUG		/* prototype debug mode */
#define CONFIG_SILENT_CONSOLE
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#ifdef CONFIG_SILENT_CONSOLE
#define CONFIG_SILENT_U_BOOT_ONLY       /* for bootm command, if not defined fail bootm command */
#endif


#endif /* __CONFIG_H__ */

