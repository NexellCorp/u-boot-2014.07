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

#ifndef __AXP228_POWER_H_
#define __AXP228_POWER_H_

/*
 *	Config Value
 */

#define AXP228_I2C_ADDR						(0x68 >> 1)

#define GPIO_POWER_KEY_DET					(PAD_GPIO_ALV + 0)

/*
 *	Default Value
 */

#define	AXP228_OFF_PRESS_TIME				5		/* VAL = 0:0, 1:1, 2:2, 3:4, 4:6, 5:8, 6:10, 7:12 sec */

#define AXP228_LIMIT_ADP_AMP				2500000	/* VAL(mA), MAX = 2500mA */
#define AXP228_LIMIT_USB_AMP				1500000	/* VAL(mA), MAX = 1500mA */
#define AXP228_LIMIT_USBDATA_AMP			 500000	/* VAL(mA), MAX = 1500mA */

#define AXP228_CHG_ADP_AMP					1500000	/* VAL(mA), MAX = 1800mA */
#define AXP228_CHG_USB_AMP					1500000	/* VAL(mA), MAX = 1800mA */

#define AXP228_USB_LIMIT_900				0x00		/* USB3.0 : 900mA */
#define AXP228_USB_LIMIT_500				0x01		/* USB2.0 : 500mA */
#define AXP228_USB_LIMIT_NO					0x03		/* No limit current */


#define AXP_DLDO1_VOL						(1)		/* VCC_WIDE			VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DLDO2_VOL						(1)		/* VCC1P8_CAM			VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DLDO3_VOL						(1)		/* NC					VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DLDO4_VOL						(1)		/* NC					VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_ELDO1_VOL						(1)		/* VCC1P8_SYS			VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_ELDO2_VOL						(1)		/* VCC3P3_WIFI			VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_ELDO3_VOL						(1)		/* NC					VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DC5LDO_VOL						(1)		/* VCC1P2_CVBS		VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DCDC1_VOL						(1)		/* VCC3P3_SYS			VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DCDC2_VOL						(1)		/* VCC1P1_ARM			VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DCDC3_VOL						(1)		/* VCC1P1_ARM(CORE)	VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DCDC4_VOL						(1)		/* VCC1P5_SYS			VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_DCDC5_VOL						(0)		/* VCC1P5_DDR			VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_ALDO1_VOL						(0)		/* VCC3P3_ALIVE		VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_ALDO2_VOL						(0)		/* VCC1P8_ALIVE		VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define AXP_ALDO3_VOL						(0)		/* VCC1P0_ALIVE		VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */

#define	AXP228_DDCx_VOL_STEP				12500	/* UINT = 1uV, 12.5mV */
#define	AXP228_LDOx_VOL_STEP				25000	/* UINT = 1uV, 25mV */

#define	AXP228_DDCx_VOL_MIN					600000	/* UINT = 1uV, 0.6V */
#define	AXP228_DDCx_VOL_MAX					3500000	/* UINT = 1uV, 3.5V */

#define	AXP228_LDOx_VOL_MIN					900000	/* UINT = 1uV, 0.9V */
#define	AXP228_LDOx_VOL_MAX					3500000	/* UINT = 1uV, 3.5V */
#define	AXP228_LDO56_VOL_MIN				600000	/* UINT = 1uV, 0.6V */
#define	AXP228_LDO56_VOL_MAX				3500000	/* UINT = 1uV, 3.5V */
#define	AXP228_LDORTC1_VOL_MIN				1700000	/* UINT = 1uV, 1.7V */
#define	AXP228_LDORTC1_VOL_MAX				3500000	/* UINT = 1uV, 3.5V */

#define AXP_DLDO1_ON						(1)		/* VCC_WIDE			0: Off, 1: On */
#define AXP_DLDO2_ON						(1)		/* VCC1P8_CAM			0: Off, 1: On */
#define AXP_DLDO3_ON						(1)		/* NC					0: Off, 1: On */
#define AXP_DLDO4_ON						(1)		/* NC					0: Off, 1: On */
#define AXP_ELDO1_ON						(1)		/* VCC1P8_SYS			0: Off, 1: On */
#define AXP_ELDO2_ON						(1)		/* VCC3P3_WIFI			0: Off, 1: On */
#define AXP_ELDO3_ON						(1)		/* NC					0: Off, 1: On */
#define AXP_DC5LDO_ON						(1)		/* VCC1P2_CVBS		0: Off, 1: On */
#define AXP_DCDC1_ON						(1)		/* VCC3P3_SYS			0: Off, 1: On */
#define AXP_DCDC2_ON						(1)		/* VCC1P1_ARM			0: Off, 1: On */
#define AXP_DCDC3_ON						(1)		/* VCC1P1_ARM(CORE)	0: Off, 1: On */
#define AXP_DCDC4_ON						(1)		/* VCC1P5_SYS			0: Off, 1: On */
#define AXP_DCDC5_ON						(0)		/* VCC1P5_DDR			0: Off, 1: On */
#define AXP_ALDO1_ON						(0)		/* VCC3P3_ALIVE		0: Off, 1: On */
#define AXP_ALDO2_ON						(0)		/* VCC1P8_ALIVE		0: Off, 1: On */
#define AXP_ALDO3_ON						(0)		/* VCC1P0_ALIVE		0: Off, 1: On */


#define AXP_DLDO1_SLEEP_OFF					(1)		/* VCC_WIDE			0 : sleep on, 1 : sleep off */
#define AXP_DLDO2_SLEEP_OFF					(1)		/* VCC1P8_CAM			0 : sleep on, 1 : sleep off */
#define AXP_DLDO3_SLEEP_OFF					(1)		/* NC					0 : sleep on, 1 : sleep off */
#define AXP_DLDO4_SLEEP_OFF					(1)		/* NC					0 : sleep on, 1 : sleep off */
#define AXP_ELDO1_SLEEP_OFF					(1)		/* VCC1P8_SYS			0 : sleep on, 1 : sleep off */
#define AXP_ELDO2_SLEEP_OFF					(1)		/* VCC3P3_WIFI			0 : sleep on, 1 : sleep off */
#define AXP_ELDO3_SLEEP_OFF					(1)		/* NC					0 : sleep on, 1 : sleep off */
#define AXP_DC5LDO_SLEEP_OFF				(1)		/* VCC1P2_CVBS		0 : sleep on, 1 : sleep off */
#define AXP_DCDC1_SLEEP_OFF					(1)		/* VCC3P3_SYS			0 : sleep on, 1 : sleep off */
#define AXP_DCDC2_SLEEP_OFF					(1)		/* VCC1P1_ARM			0 : sleep on, 1 : sleep off */
#define AXP_DCDC3_SLEEP_OFF					(1)		/* VCC1P1_ARM(CORE)	0 : sleep on, 1 : sleep off */
#define AXP_DCDC4_SLEEP_OFF					(1)		/* VCC1P5_SYS			0 : sleep on, 1 : sleep off */
#define AXP_DCDC5_SLEEP_OFF					(0)		/* VCC1P5_DDR			0 : sleep on, 1 : sleep off */
#define AXP_ALDO1_SLEEP_OFF					(0)		/* VCC3P3_ALIVE		0 : sleep on, 1 : sleep off */
#define AXP_ALDO2_SLEEP_OFF					(0)		/* VCC1P8_ALIVE		0 : sleep on, 1 : sleep off */
#define AXP_ALDO3_SLEEP_OFF					(0)		/* VCC1P0_ALIVE		0 : sleep on, 1 : sleep off */


#define	AXP228_ADC_AIN0						0		/* VAL = 0: Disable, 1: Enable, default = Disable*/
#define	AXP228_ADC_AIN1						0		/* VAL = 0: Disable, 1: Enable, default = Disable*/
#define	AXP228_ADC_VTHM						0		/* VAL = 0: Disable, 1: Enable, default = Disable*/
#define	AXP228_ADC_VSYS						1		/* VAL = 0: Disable, 1: Enable, default = Disable*/
#define	AXP228_ADC_VUSB						0		/* VAL = 0: Disable, 1: Enable, default = Disable*/
#define	AXP228_ADC_VADP						0		/* VAL = 0: Disable, 1: Enable, default = Disable*/
#define	AXP228_ADC_VBAT						1		/* VAL = 0: Disable, 1: Enable, default = Disable*/
#define	AXP228_ADC_ILIM						0		/* VAL = 0: Disable, 1: Enable, default = Disable*/


#endif	/* __AXP228_POWER_H_ */
