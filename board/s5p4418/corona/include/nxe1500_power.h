/*
 *  Copyright (C) 2016 NEXELL SOC Lab.
 *  Jongsin Park <pjsin865@nexell.co.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __NXE1500_POWER_H_
#define __NXE1500_POWER_H_

#include <power/pmic.h>
#include <platform.h>
#include "nxe1500-private.h"


/*
 *	Config Value
 */

#define NXE1500_I2C_ADDR					(0x66 >> 1)

#define CHARGER_MIN_CURRENT					100		/* VAL(mV) */
#define CHARGER_MAX_CURRENT					800		/* VAL(mV) */
#define CHARGER_CURRENT_RESOLUTION			100		/* VAL(mV) */
#define CHARGER_CURRENT_COMPLETE			3		/* (n + 1) x 50mA   */

#define GPIO_POWER_KEY_DET					(PAD_GPIO_ALV + 0)

/*
 *	Default Value
 */

#define	NXE1500_DEF_OFF_PRESS_TIME			5		/* VAL = 0:0, 1:1, 2:2, 3:4, 4:6, 5:8, 6:10, 7:12 sec */

#define	NXE1500_DEF_DDC1_ON					1		/* VAL = 0: Off, 1: On, default(OTP) = On */	/* 1.3V ARM		*/
#define	NXE1500_DEF_DDC2_ON					1		/* VAL = 0: Off, 1: On, default(OTP) = On */	/* 1.2V CORE	*/
#define	NXE1500_DEF_DDC3_ON					1		/* VAL = 0: Off, 1: On, default(OTP) = On */	/* 3.3V SYS		*/
#define	NXE1500_DEF_DDC4_ON					1		/* VAL = 0: Off, 1: On, default(OTP) = On */	/* 1.6V DDR		*/

#define	NXE1500_DEF_LDO1_ON					1		/* VAL = 0: Off, 1: On */	/* 3.3V GPS & WIFI	*/
#define	NXE1500_DEF_LDO2_ON					1		/* VAL = 0: Off, 1: On */	/* 1.8V CAM1	*/
#define	NXE1500_DEF_LDO3_ON					1		/* VAL = 0: Off, 1: On */	/* 1.8V SYS1,	Boot On	*/
#define	NXE1500_DEF_LDO4_ON					0		/* VAL = 0: Off, 1: On */	/* 1.9V SYS,		Boot On	*/
#define	NXE1500_DEF_LDO5_ON					0		/* VAL = 0: Off, 1: On */	/* 2.8V CAM		*/
#define	NXE1500_DEF_LDORTC1_ON				1		/* VAL = 0: Off, 1: On */	/* 1.8V ALIVE,	Boot On	*/
#define	NXE1500_DEF_LDORTC2_ON				1		/* VAL = 0: Off, 1: On */	/* 1.0V ALIVE,	Boot On	*/

#define	NXE1500_DEF_DDC1_VOL				1250000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define	NXE1500_DEF_DDC2_VOL				1100000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.2V */
#define	NXE1500_DEF_DDC3_VOL				1500000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */
#define	NXE1500_DEF_DDC4_VOL				1500000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */

#define	NXE1500_DEF_DDC1_SLP_VOL			1250000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */
#define	NXE1500_DEF_DDC2_SLP_VOL			1100000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.2V */
#define	NXE1500_DEF_DDC3_SLP_VOL			1500000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */
#define	NXE1500_DEF_DDC4_SLP_VOL			1500000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.6V */

#define	NXE1500_DEF_LDO1_VOL				3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1500_DEF_LDO2_VOL				1800000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.80V */
#define	NXE1500_DEF_LDO3_VOL				3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.80V */
#define	NXE1500_DEF_LDO4_VOL				1800000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.90V */
#define	NXE1500_DEF_LDO5_VOL				2800000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step = 25mV, default(OTP) = 2.80V */
#define	NXE1500_DEF_LDORTC1_VOL				1800000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1500_DEF_LDORTC2_VOL				1000000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.20V */

#define	NXE1500_DEF_LDO1_SLP_VOL			3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1500_DEF_LDO2_SLP_VOL			1800000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.80V */
#define	NXE1500_DEF_LDO3_SLP_VOL			1800000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.80V */
#define	NXE1500_DEF_LDO4_SLP_VOL			1900000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.90V */
#define	NXE1500_DEF_LDO5_SLP_VOL			2800000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step = 25mV, default(OTP) = 2.80V */

#define	NXE1500_DEF_DDCx_VOL_STEP			12500	/* UINT = 1uV, 12.5mV */
#define	NXE1500_DEF_LDOx_VOL_STEP			25000	/* UINT = 1uV, 25mV */

#define	NXE1500_DEF_DDCx_VOL_MIN			600000	/* UINT = 1uV, 0.6V */
#define	NXE1500_DEF_DDCx_VOL_MAX			3500000	/* UINT = 1uV, 3.5V */
#define	NXE1500_DEF_LDOx_VOL_MIN			900000	/* UINT = 1uV, 0.9V */
#define	NXE1500_DEF_LDOx_VOL_MAX			3500000	/* UINT = 1uV, 3.5V */
#define	NXE1500_DEF_LDO3_VOL_MIN			600000	/* UINT = 1uV, 0.6V */
#define	NXE1500_DEF_LDO3_VOL_MAX			3500000	/* UINT = 1uV, 3.5V */
#define	NXE1500_DEF_LDORTC1_VOL_MIN			1200000	/* UINT = 1uV, 1.7V */
#define	NXE1500_DEF_LDORTC1_VOL_MAX			3500000	/* UINT = 1uV, 3.5V */

#define	NXE1500_DEF_DDC1_SLP_MODE			0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1500_DEF_DDC2_SLP_MODE			0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1500_DEF_DDC3_SLP_MODE			0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1500_DEF_DDC4_SLP_MODE			0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */

#define	NXE1500_DEF_DDC1_MODE				0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1500_DEF_DDC2_MODE				0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1500_DEF_DDC3_MODE				0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1500_DEF_DDC4_MODE				0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */

#define	NXE1500_DEF_DDC1_DSC_CTRL			1		/* VAL = 0: Off, 1: On(default) */
#define	NXE1500_DEF_DDC2_DSC_CTRL			1		/* VAL = 0: Off, 1: On(default) */
#define	NXE1500_DEF_DDC3_DSC_CTRL			1		/* VAL = 0: Off, 1: On(default) */
#define	NXE1500_DEF_DDC4_DSC_CTRL			1		/* VAL = 0: Off, 1: On(default) */

#define	NXE1500_DEF_DDC1_OSC_FREQ			0		/* VAL(MHz) = 0: 1.5, 1: prohibit, 2: 3.0, 3: prohibit, default(OTP) = 1.5MHz */
#define	NXE1500_DEF_DDC2_OSC_FREQ			0		/* VAL(MHz) = 0: 1.5, 1: prohibit, 2: 3.0, 3: prohibit, default(OTP) = 1.5MHz */
#define	NXE1500_DEF_DDC3_OSC_FREQ			0		/* VAL(MHz) = 0: 1.5, 1: prohibit, 2: 3.0, 3: prohibit, default(OTP) = 1.5MHz */
#define	NXE1500_DEF_DDC4_OSC_FREQ			0		/* VAL(MHz) = 0: 1.5, 1: prohibit, 2: 3.0, 3: prohibit, default(OTP) = 1.5MHz */

#define	NXE1500_DEF_DDC1_RAMP_SLOP			0		/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit, default = 14mV/us */
#define	NXE1500_DEF_DDC2_RAMP_SLOP			0		/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit, default = 14mV/us */
#define	NXE1500_DEF_DDC3_RAMP_SLOP			0		/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit, default = 14mV/us */
#define	NXE1500_DEF_DDC4_RAMP_SLOP			0		/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit, default = 14mV/us */

#define	NXE1500_DEF_DDC1_CUR_LIMIT			3		/* VAL(A) = 0: No limit, 1: 3.2A, 2: 3.7A, 3: 4.0A, default(OTP) = 3.2A */
#define	NXE1500_DEF_DDC2_CUR_LIMIT			3		/* VAL(A) = 0: No limit, 1: 3.2A, 2: 3.7A, 3: 4.0A, default(OTP) = 3.2A */
#define	NXE1500_DEF_DDC3_CUR_LIMIT			1		/* VAL(A) = 0: No limit, 1: 3.2A, 2: 3.7A, 3: 4.0A, default(OTP) = 3.2A */
#define	NXE1500_DEF_DDC4_CUR_LIMIT			1		/* VAL(A) = 0: No limit, 1: 3.2A, 2: 3.7A, 3: 4.0A, default(OTP) = 3.2A */

#define	NXE1500_DEF_DDC1_LIMSHUT_EN			0		/* VAL = 0: Disable, 1: Enable */
#define	NXE1500_DEF_DDC2_LIMSHUT_EN			0		/* VAL = 0: Disable, 1: Enable */
#define	NXE1500_DEF_DDC3_LIMSHUT_EN			1		/* VAL = 0: Disable, 1: Enable */
#define	NXE1500_DEF_DDC4_LIMSHUT_EN			1		/* VAL = 0: Disable, 1: Enable */

#define NXE1500_DEF_DIE_RETURN_TEMP			0		/* VAL(C)= 0: 80, 1: 100, default(OTP) = 80' */
#define NXE1500_DEF_DIE_ERROR_TEMP			0		/* VAL(C)= 0: 100, 1: 120 */
#define NXE1500_DEF_DIE_SHUTDOWN_TEMP		1		/* VAL(C)= 0: 100, 1: 120, 2: 135, 3: 135, default(OTP) = 120'  */

/* REPCNT */
#define	NXE1500_DEF_REPCNT_REPWRON			0
#define	NXE1500_DEF_REPCNT_REPWRTIM			0
#define	NXE1500_DEF_REPCNT_OFF_RESETO		0

/* WATHDOG */
#define	NXE1500_DEF_WATHDOG_WDOGTIM			3
#define	NXE1500_DEF_WATHDOG_WDOG_EN			0
#define	NXE1500_DEF_WATHDOG_WDOGSLPEN		0

/* PWRIREN */
#define	NXE1500_DEF_PWRIREN_WDOG			0
#define	NXE1500_DEF_PWRIREN_NOE_OFF			0
#define	NXE1500_DEF_PWRIREN_PWRON_OFF 		0
#define	NXE1500_DEF_PWRIREN_PREOT			0
#define	NXE1500_DEF_PWRIREN_PRVINDT			0
#define	NXE1500_DEF_PWRIREN_EXTIN			0
#define	NXE1500_DEF_PWRIREN_PWRON			0

#define NXE1500_NUM_BUCK					5
#define NXE1500_NUM_LDO						10
#define NXE1500_NUM_PSO						5

#define NXE1500_DEF_FET1CNT					0x05
#define NXE1500_DEF_FET2CNT					0x00
#define NXE1500_DEF_TSET					0x20
#define NXE1500_DEF_CMPSET					0x00
#define NXE1500_DEF_SUSPEND					0x01

#endif  //#ifndef __NXE1500_POWER_H_
