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

#define GPIO_POWER_KEY_DET					(PAD_GPIO_ALV + 0)


/*
 *	Default Value
 */

/* USB Limit Value */
#define AXP228_USB_LIMIT_900				0x00		/* USB3.0 : 900mA */
#define AXP228_USB_LIMIT_500				0x01		/* USB2.0 : 500mA */
#define AXP228_USB_LIMIT_NO					0x03		/* No limit current */

/* DCDC-LDO Name */
#define AXP_DCDC1_NAME						"vdd_sys_3.3V"		/* VCC3P3_SYS			*/
#define AXP_DCDC2_NAME						"vdd_arm_1.3V"		/* VCC1P1_ARM			*/
#define AXP_DCDC3_NAME						"vdd_core_1.2V"		/* VCC1P1_ARM(CORE)	*/
#define AXP_DCDC4_NAME						"vdd_sys_1.6V"		/* VCC1P5_SYS			*/
#define AXP_DCDC5_NAME						"vdd_ddr_1.6V"		/* VCC1P5_DDR			*/
#define AXP_ALDO1_NAME						"valive_3.3V"		/* VCC3P3_ALIVE		*/
#define AXP_ALDO2_NAME						"valive_1.8V"		/* VCC1P8_ALIVE		*/
#define AXP_ALDO3_NAME						"valive_1.0V"		/* VCC1P0_ALIVE		*/
#define AXP_DLDO1_NAME						"vwide_3.3V"		/* VCC_WIDE			*/
#define AXP_DLDO2_NAME						"vcam1_1.8V"		/* VCC1P8_CAM			*/
#define AXP_DLDO3_NAME						"vdumy1_0.7V"		/* NC					*/
#define AXP_DLDO4_NAME						"vdumy2_0.7V"		/* NC					*/
#define AXP_ELDO1_NAME						"vsys1_1.8V"		/* VCC1P8_SYS			*/
#define AXP_ELDO2_NAME						"vwifi_3.3V"		/* VCC3P3_WIFI			*/
#define AXP_ELDO3_NAME						"vdumy3_0.7V"		/* NC					*/
#define AXP_DC5LDO_NAME						"vcvbs_1.2V"		/* VCC1P2_CVBS		*/

/* DCDC-LDO Voltage Value */
#define AXP_DCDC1_VALUE						3300000		/* VCC3P3_SYS			DCDC1	 : 	AXP22:1600~3400, 100/setp*/
#define AXP_DCDC2_VALUE						1100000		/* VCC1P1_ARM			DCDC2	£º 	AXP22:  600~1540,   20/step*/
#define AXP_DCDC3_VALUE						1100000		/* VCC1P0_CORE		DCDC3	£º 	AXP22:  600~1860,   20/step*/
#define AXP_DCDC4_VALUE						1500000		/* VCC1P5_SYS			DCDC4	£º 	AXP22:  600~1540,   20/step*/
#define AXP_DCDC5_VALUE						1500000		/* VCC1P5_DDR			DCDC5	£º 	AXP22:1000~2550,   50/step*/
#define AXP_ALDO1_VALUE						3300000		/* VCC3P3_ALIVE		ALDO1	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ALDO2_VALUE						1800000		/* VCC1P8_ALIVE		ALDO2	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ALDO3_VALUE						1000000		/* VCC1P0_ALIVE		ALDO3	£º 	AXP22:  700~3300, 100/step*/
#define AXP_DLDO1_VALUE						3300000		/* VCC_WIDE			DLDO1	£º 	AXP22:  700~3300, 100/step*/
#define AXP_DLDO2_VALUE						1800000		/* VCC1P8_CAM			DLDO2	£º 	AXP22 : 700~3300, 100/step*/
#define AXP_DLDO3_VALUE						 700000		/* NC					DLDO3	£º 	AXP22:  700~3300, 100/step*/
#define AXP_DLDO4_VALUE						 700000		/* NC					DLDO4	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ELDO1_VALUE						1800000		/* VCC1P8_SYS			ELDO1	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ELDO2_VALUE						3300000		/* VCC3P3_WIFI			ELDO2	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ELDO3_VALUE						 700000		/* NC					ELDO3	£º 	AXP22:  700~3300, 100/step*/
#define AXP_DC5LDO_VALUE					1200000		/* VCC1P2_CVBS		DC5LDO	£º 	AXP22:  700~1400, 100/step*/

/* DCDC-LDO Voltage Enable Value */
#define AXP_DCDC1_ENABLE					1		/* VCC3P3_SYS			DCDC1	 : 	AXP22:1600~3400, 100/setp*/
#define AXP_DCDC2_ENABLE					1		/* VCC1P1_ARM			DCDC2	£º 	AXP22:  600~1540,   20/step*/
#define AXP_DCDC3_ENABLE					1		/* VCC1P0_CORE		DCDC3	£º 	AXP22:  600~1860,   20/step*/
#define AXP_DCDC4_ENABLE					1		/* VCC1P5_SYS			DCDC4	£º 	AXP22:  600~1540,   20/step*/
#define AXP_DCDC5_ENABLE					1		/* VCC1P5_DDR			DCDC5	£º 	AXP22:1000~2550,   50/step*/
#define AXP_ALDO1_ENABLE					1		/* VCC3P3_ALIVE		ALDO1	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ALDO2_ENABLE					1		/* VCC1P8_ALIVE		ALDO2	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ALDO3_ENABLE					1		/* VCC1P0_ALIVE		ALDO3	£º 	AXP22:  700~3300, 100/step*/
#define AXP_DLDO1_ENABLE					1		/* VCC_WIDE			DLDO1	£º 	AXP22:  700~3300, 100/step*/
#define AXP_DLDO2_ENABLE					0		/* VCC1P8_CAM			DLDO2	£º 	AXP22 : 700~3300, 100/step*/
#define AXP_DLDO3_ENABLE					0		/* NC					DLDO3	£º 	AXP22:  700~3300, 100/step*/
#define AXP_DLDO4_ENABLE					0		/* NC					DLDO4	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ELDO1_ENABLE					1		/* VCC1P8_SYS			ELDO1	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ELDO2_ENABLE					0		/* VCC3P3_WIFI			ELDO2	£º 	AXP22:  700~3300, 100/step*/
#define AXP_ELDO3_ENABLE					0		/* NC					ELDO3	£º 	AXP22:  700~3300, 100/step*/
#define AXP_DC5LDO_ENABLE					0		/* VCC1P2_CVBS		DC5LDO	£º 	AXP22:  700~1400, 100/step*/
#define AXP_DC1SW_ENABLE					0		/* NC					DC1SW	£º 	AXP22: */

/* DCDC-LDO Sleep Enable Value */
#define AXP_DCDC1_SLEEP_OFF					(1)		/* VCC3P3_SYS			0 : sleep on, 1 : sleep off */
#define AXP_DCDC2_SLEEP_OFF					(1)		/* VCC1P1_ARM			0 : sleep on, 1 : sleep off */
#define AXP_DCDC3_SLEEP_OFF					(1)		/* VCC1P0_CORE		0 : sleep on, 1 : sleep off */
#define AXP_DCDC4_SLEEP_OFF					(1)		/* VCC1P5_SYS			0 : sleep on, 1 : sleep off */
#define AXP_DCDC5_SLEEP_OFF					(0)		/* VCC1P5_DDR			0 : sleep on, 1 : sleep off */
#define AXP_ALDO1_SLEEP_OFF					(0)		/* VCC3P3_ALIVE		0 : sleep on, 1 : sleep off */
#define AXP_ALDO2_SLEEP_OFF					(0)		/* VCC1P8_ALIVE		0 : sleep on, 1 : sleep off */
#define AXP_ALDO3_SLEEP_OFF					(0)		/* VCC1P0_ALIVE		0 : sleep on, 1 : sleep off */
#define AXP_DLDO1_SLEEP_OFF					(1)		/* VCC_WIDE			0 : sleep on, 1 : sleep off */
#define AXP_DLDO2_SLEEP_OFF					(1)		/* VCC1P8_CAM			0 : sleep on, 1 : sleep off */
#define AXP_DLDO3_SLEEP_OFF					(1)		/* NC					0 : sleep on, 1 : sleep off */
#define AXP_DLDO4_SLEEP_OFF					(1)		/* NC					0 : sleep on, 1 : sleep off */
#define AXP_ELDO1_SLEEP_OFF					(1)		/* VCC1P8_SYS			0 : sleep on, 1 : sleep off */
#define AXP_ELDO2_SLEEP_OFF					(1)		/* VCC3P3_WIFI			0 : sleep on, 1 : sleep off */
#define AXP_ELDO3_SLEEP_OFF					(1)		/* NC					0 : sleep on, 1 : sleep off */
#define AXP_DC5LDO_SLEEP_OFF				(1)		/* VCC1P2_CVBS		0 : sleep on, 1 : sleep off */


/* OCV Table */
#define OCVREG0								0		//3.13V
#define OCVREG1								0		//3.27V
#define OCVREG2								0		//3.34V
#define OCVREG3								0		//3.41V
#define OCVREG4								0		//3.48V
#define OCVREG5								0		//3.52V
#define OCVREG6								0		//3.55V
#define OCVREG7								0		//3.57V
#define OCVREG8								0		//3.59V
#define OCVREG9								0		//3.61V
#define OCVREGA								1		//3.63V
#define OCVREGB								2		//3.64V
#define OCVREGC								3		//3.66V
#define OCVREGD								8		//3.70V
#define OCVREGE								15		//3.73V 
#define OCVREGF								23		//3.77V
#define OCVREG10							29		//3.78V
#define OCVREG11							37		//3.80V
#define OCVREG12							42		//3.82V 
#define OCVREG13							48		//3.84V
#define OCVREG14							51		//3.85V
#define OCVREG15							55		//3.87V
#define OCVREG16							62		//3.91V
#define OCVREG17							68		//3.94V
#define OCVREG18							73		//3.98V
#define OCVREG19							78		//4.01V
#define OCVREG1A							82		//4.05V
#define OCVREG1B							86		//4.08V
#define OCVREG1C							89		//4.10V 
#define OCVREG1D							93		//4.12V
#define OCVREG1E							96		//4.14V
#define OCVREG1F							100		//4.15V

/* Battery capacity */
#define BATCAP								3000


#endif	/* __AXP228_POWER_H_ */
