/*
 * (C) Copyright 2013
 * bong kwan kook, Nexell Co, <kook@nexell.co.kr>
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

#ifndef __NXE1100_POWER_H_
#define __NXE1100_POWER_H_

#include "nxe2000-private.h"


/*
 *	Default Value
 */
#define	NXE1100_DEF_DDC1_ON					1		/* VAL = 0: Disable, 1: Enable, default(OTP) = Enable */
#define	NXE1100_DEF_DDC2_ON					1		/* VAL = 0: Disable, 1: Enable, default(OTP) = Enable */
#define	NXE1100_DEF_DDC3_ON					1		/* VAL = 0: Disable, 1: Enable, default(OTP) = Enable */

#define	NXE1100_DEF_LDO1_ON					1		/* VAL = 0: Off, 1: On */	/* 3.3V GPS		*/
#define	NXE1100_DEF_LDO2_ON					1		/* VAL = 0: Off, 1: On */	/* 3.3V WIFI	*/
#define	NXE1100_DEF_LDO3_ON					1		/* VAL = 0: Off, 1: On */	/* 2.8V Camera	*/
#define	NXE1100_DEF_LDO4_ON					1		/* VAL = 0: Off, 1: On */	/* 3.3V HUB		*/
#define	NXE1100_DEF_LDO5_ON					1		/* VAL = 0: Off, 1: On */	/* 1.2V HSIC	*/
#define	NXE1100_DEF_LDORTC1_ON				0		/* VAL = 0: Off, 1: On */	/* 3.3V HUB		*/
#define	NXE1100_DEF_LDORTC2_ON				0		/* VAL = 0: Off, 1: On */	/* 1.2V HSIC	*/
	
#define	NXE1100_DEF_DDC1_VOL				1100000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */
#define	NXE1100_DEF_DDC2_VOL				1000000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.8V */
#define	NXE1100_DEF_DDC3_VOL				1500000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 1.3V */

#define	NXE1100_DEF_DDC1_SLP_VOL			3300000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */
#define	NXE1100_DEF_DDC2_SLP_VOL			1800000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */
#define	NXE1100_DEF_DDC3_SLP_VOL			1300000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV, default(OTP) = 3.3V */

#define	NXE1100_DEF_LDO1_VOL				3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1100_DEF_LDO2_VOL				3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1100_DEF_LDO3_VOL				2800000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step = 25mV, default(OTP) = 2.80V */
#define	NXE1100_DEF_LDO4_VOL				3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1100_DEF_LDO5_VOL				1200000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.20V */
#define	NXE1100_DEF_LDORTC1_VOL				1800000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1100_DEF_LDORTC2_VOL				1000000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.20V */

#define	NXE1100_DEF_LDO1_SLP_VOL			3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1100_DEF_LDO2_SLP_VOL			3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1100_DEF_LDO3_SLP_VOL			2800000	/* VAL(uV) = 0: 0.60 ~ 3.5V, Step = 25mV, default(OTP) = 2.80V */
#define	NXE1100_DEF_LDO4_SLP_VOL			3300000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 3.30V */
#define	NXE1100_DEF_LDO5_SLP_VOL			1200000	/* VAL(uV) = 0: 0.90 ~ 3.5V, Step = 25mV, default(OTP) = 1.20V */

#define	NXE1100_DEF_DDCx_VOL_STEP			12500	/* UINT = 1uV, 12.5mV */
#define	NXE1100_DEF_LDOx_VOL_STEP			25000	/* UINT = 1uV, 25mV */

#define	NXE1100_DEF_DDCx_VOL_MIN			600000	/* UINT = 1uV, 0.6V */
#define	NXE1100_DEF_DDCx_VOL_MAX			3500000	/* UINT = 1uV, 3.5V */

#define	NXE1100_DEF_LDOx_VOL_MIN			900000	/* UINT = 1uV, 0.9V */
#define	NXE1100_DEF_LDOx_VOL_MAX			3500000	/* UINT = 1uV, 3.5V */
#define	NXE1100_DEF_LDO3_VOL_MIN			600000	/* UINT = 1uV, 0.6V */
#define	NXE1100_DEF_LDO3_VOL_MAX			3500000	/* UINT = 1uV, 3.5V */
#define	NXE1100_DEF_LDORTC1_VOL_MIN			1700000	/* UINT = 1uV, 1.7V */
#define	NXE1100_DEF_LDORTC1_VOL_MAX			3500000	/* UINT = 1uV, 3.5V */

#define	NXE1100_DEF_DDC1_SLP_MODE			0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1100_DEF_DDC2_SLP_MODE			0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1100_DEF_DDC3_SLP_MODE			0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */

#define	NXE1100_DEF_DDC1_MODE				0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1100_DEF_DDC2_MODE				0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
#define	NXE1100_DEF_DDC3_MODE				0		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */

#define	NXE1100_DEF_DDC1_DSC_CTRL			1		/* VAL = 0: Off, 1: On(default) */
#define	NXE1100_DEF_DDC2_DSC_CTRL			1		/* VAL = 0: Off, 1: On(default) */
#define	NXE1100_DEF_DDC3_DSC_CTRL			1		/* VAL = 0: Off, 1: On(default) */

#define	NXE1100_DEF_DDC1_OSC_FREQ			0		/* VAL(MHz) = 0: 1.5, 1: prohibit, 2: 3.0, 3: prohibit, default(OTP) = 1.5MHz */
#define	NXE1100_DEF_DDC2_OSC_FREQ			0		/* VAL(MHz) = 0: 1.5, 1: prohibit, 2: 3.0, 3: prohibit, default(OTP) = 1.5MHz */
#define	NXE1100_DEF_DDC3_OSC_FREQ			0		/* VAL(MHz) = 0: 1.5, 1: prohibit, 2: 3.0, 3: prohibit, default(OTP) = 1.5MHz */

#define	NXE1100_DEF_DDC1_RAMP_SLOP			0		/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit, default = 14mV/us */
#define	NXE1100_DEF_DDC2_RAMP_SLOP			0		/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit, default = 14mV/us */
#define	NXE1100_DEF_DDC3_RAMP_SLOP			0		/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit, default = 14mV/us */

#define	NXE1100_DEF_DDC1_CUR_LIMIT			1		/* VAL(A) = 0: No limit, 1: 3.2A, 2: 3.7A, 3: 4.0A, default(OTP) = 3.2A */
#define	NXE1100_DEF_DDC2_CUR_LIMIT			1		/* VAL(A) = 0: No limit, 1: 3.2A, 2: 3.7A, 3: 4.0A, default(OTP) = 3.2A */
#define	NXE1100_DEF_DDC3_CUR_LIMIT			1		/* VAL(A) = 0: No limit, 1: 3.2A, 2: 3.7A, 3: 4.0A, default(OTP) = 3.2A */

#define	NXE1100_DEF_DDC1_LIMSHUT_EN			1		/* VAL = 0: Disable, 1: Enable */
#define	NXE1100_DEF_DDC2_LIMSHUT_EN			1		/* VAL = 0: Disable, 1: Enable */
#define	NXE1100_DEF_DDC3_LIMSHUT_EN			1		/* VAL = 0: Disable, 1: Enable */

#define NXE1100_DEF_CHG_ADP_EN 				1		/* VAL = 0: Off, 1: On, default = On */
#define NXE1100_DEF_CHG_USB_EN 				1		/* VAL = 0: Off, 1: On, default = On */
#define NXE1100_DEF_CHG_NOBAT_OVLIM_EN 		1		/* VAL = 0: Off, 1: On, default = On */

#define NXE1100_DEF_CHG_USB_VCONTMASK		0		/* VAL = 0: Off, 1: On, default = Off */
#define NXE1100_DEF_CHG_ADP_VCONTMASK		1		/* VAL = 0: Off, 1: On, default = On */
#define NXE1100_DEF_CHG_VUSB_BUCK_THS		0		/* VAL(V) = 0: 4.1 ~ 4.2, 1: 4.2 ~ 4.3, 2: 4.3 ~ 4.4, 3: 4.4 ~ 4.5, default(OTP) = 4.1 ~ 4.2V */
#define NXE1100_DEF_CHG_VADP_BUCK_THS		0		/* VAL(V) = 0: 4.1 ~ 4.2, 1: 4.2 ~ 4.3, 2: 4.3 ~ 4.4, 3: 4.4 ~ 4.5, default(OTP) = 4.1 ~ 4.2V */

#define NXE1100_DEF_CHG_VSYS_VOL			1		/* VAL(V) = 0: 3.60, 1: 4.00, 2: 4.40, 3: 4.55, default = 4.00V */
#define NXE1100_DEF_CHG_VSYS_OVER_VOL		3		/* VAL(V) = 0: 4.8 ~ 5.3, 1: 5.0 ~ 5.5, 2: 5.2 ~ 5.7, 3: 5.2 ~ 5.7, default = 5.0 ~ 5.5V */

#define NXE1100_DEF_CHG_RAPID_CTIME			2		/* VAL(sec) = 0: 0.1, 1: 0.5, 2: 1 3: 5, default = 1sec */
#define NXE1100_DEF_CHG_RAPID_RTIME			3		/* VAL(min) = 0: 120, 1: 180, 2: 240 3: 300, default = 300min */

#define NXE1100_DEF_CHG_POWER_ON_VOL		7		/* VAL(V) = 0: 2.7 ~ 2.8, 1: 2.8 ~ 2.9, 2: 2.9 ~ 3.0, 3: 3.0 ~ 3.1, 4: 3.1 ~ 3.2, 5: 3.2 ~ 3.3, 6: 3.3 ~ 3.4, 7: 3.4 ~ 3.5 */
													/* default(OTP) = 3.4 ~ 3.5V */
#define NXE1100_DEF_CHG_VBATOV_SET			0		/* VAL(V) = 0: 3.95 ~ 4.38, 1: 4.10 ~ 4.53, default = 3.95 ~ 4.38V */
#define NXE1100_DEF_CHG_VWEAK				3		/* VAL(V) = 0: 3.0 ~ 3.1, 1: 3.1 ~ 3.2, 2: 3.2 ~ 3.3, 3: 3.3 ~ 3.4, default(OTP) = 3.3 ~ 3.4V */
#define NXE1100_DEF_CHG_VDEAD				0		/* VAL(V) = 0: 2.9 ~ 3.0, 1: 2.8 ~ 2.9, default(OTP) = 2.9 ~ 3.0V */
#define NXE1100_DEF_CHG_VSHORT				0		/* VAL(V) = 0: 2.7 ~ 2.8, 1: 2.5 ~ 2.6, default(OTP) = 2.7 ~ 2.8V */

#define NXE1100_DEF_CHG_VFCHG				1		/* VAL(V) = 0: 4.05, 1: 4.10, 2: 4.15, 3: 4.20,  4 ~ 7: 4.35, default = 4.10V */
#define NXE1100_DEF_CHG_VRCHG				1		/* VAL(V) = 0: 3.85, 1: 3.90, 2: 3.95, 3: 4.00,  4 ~ 7: 4.10  default = 3.90V */

#define NXE1100_DEF_CHG_CURR_ADP 			24		/* VAL(mA) = 100mA x (n + 1), MAX = 2500mA, default(OTP) = 2500mA */
#define NXE1100_DEF_CHG_CURR_USB 			14		/* VAL(mA) = 100mA x (n + 1), MAX = 1500mA, default(OTP) = 1500mA */
#define NXE1100_DEF_CHG_CURR_RAPID_ADP		17		/* VAL(mA) = 100mA x (n + 1), MAX = 1800mA, default(OTP) = 100mA */
#define NXE1100_DEF_CHG_CURR_RAPID_USB		5		/* VAL(mA) = 100mA x (n + 1), MAX = 1800mA, default(OTP) = 100mA */
#define NXE1100_DEF_CHG_CURR_RAPID_COMP 	0		/* VAL(mA) = 50mA x (n + 1), MAX = 200mA, default = 50mA */

#define NXE1100_DEF_DIE_RETURN_TEMP			0		/* VAL(C)= 0: 80, 1: 100, default(OTP) = 80' */
#define NXE1100_DEF_DIE_ERROR_TEMP			0		/* VAL(C)= 0: 100, 1: 120 */
#define NXE1100_DEF_DIE_SHUTDOWN_TEMP		1		/* VAL(C)= 0: 100, 1: 120, 2: 135, 3: 135, default(OTP) = 120'  */

#define NXE1100_DEF_CHG_READY_CUR			0		/* VAL(mA)= 0:0(deefault), 1:10 */
#define NXE1100_DEF_CHG_SUSPENDB			1


/*
 * for no battery
 */
#define NXE1100_DEF_FET1CNT					0x05
#define NXE1100_DEF_FET2CNT					0x00
#define NXE1100_DEF_TSET					0x20
#define NXE1100_DEF_CMPSET					0x00
#define NXE1100_DEF_SUSPEND					0x01
/*
 * Policy to control pmic state machine.
 */
struct nxe1100_pmic_policy;
struct nxe1100_power;

/* Voltage Detector */
struct nxe1100_vdc_policy {
	/* VDCTRL: Detection Circuit Control Register(Address 02h) */
	int	battery_release_vol;	/* R:VDCTRL, VAL(V) = 0: 3.1, 1: 3.2,  2: 3.3(default), 3: 3.5 */
	int	reset_release_vol;		/* R:VDCTRL, VAL(V) = 0: prohibit, 1: 1.53,  2: 2.13, 3: 2.21, 4: 2.38, 5: 2.42, 6: 2.55, 7: 2.81 (default) */
	int	sys_limit_current; 		/* R:FET1CNT, NOTE> this value will affect the VCHG and VSYS pin, VAL(mA) = 0:120, 1:240, 2:360, 3:480, 4:600, 5:720(default), 6:840, 7:960, 8:1080, other:1200 */
};

/* Regulator */
struct nxe1100_ldo_policy {
	/* LDOON: LDO Output Control Register(Address 03h) */
	int	ldo_1_out_enb;		/* R:LDO1EN  , 1= On, 0 = Off */
	int	ldo_1_out_vol;		/* R:LDO1DAC, Pin VOUT1, VAL(V) = 0: 0.90 ~ 3.5V, Step = 25mV */
	int	ldo_2_out_enb;		/* R:LDO2EN  , 1= On, 0 = Off */
	int	ldo_2_out_vol;		/* R:LDO2DAC, Pin VOUT2, VAL(V) = 0: 0.90 ~ 3.5V, Step = 25mV */

	int	ldo_3_out_enb;		/* R:LDO3EN  , 1= On, 0 = Off */
	int	ldo_3_out_vol;		/* R:LDO3DAC, Pin VOUT3, VAL(V) = 0: 0.60 ~ 3.5V, Step = 25mV */
							/* LDO3 default(OTP) is Enable */

	int	ldo_4_out_enb;		/* R:LDO4EN  , 1= On, 0 = Off */
	int	ldo_4_out_vol;		/* R:LDO4DAC, Pin VOUT4, VAL(V) = 0: 0.90 ~ 3.5V, Step = 25mV */
	int	ldo_5_out_enb;		/* R:LDO5EN  , 1= On, 0 = Off */
	int	ldo_5_out_vol;		/* R:LDO5DAC, Pin VOUT4, VAL(V) = 0: 0.90 ~ 3.5V, Step = 25mV */

	int	ldo_rtc1_out_enb;	/* R:LDORTC1EN  , 1= On, 0 = Off */
	int	ldo_rtc1_out_vol;	/* R:LDORTC1DAC, Pin VOUTRTC1, VAL(V) = 0: 1.70 ~ 3.5V, Step = 25mV */
	int	ldo_rtc2_out_enb;	/* R:LDORTC2EN  , 1= On, 0 = Off */
	int	ldo_rtc2_out_vol;	/* R:LDORTC2DAC, Pin VOUTRTC2, VAL(V) = 0: 0.90 ~ 3.5V, Step = 25mV */
};

struct nxe1100_dcdc_policy {
	int	ddc_1_out_enb;		/* DC1EN , 1= On, 0 = Off */
	int ddc_1_out_vol;		/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV */
	int ddc_1_mode;			/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
	int ddc_1_mode_slp;		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
	int ddc_1_ramp_slop;	/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit */

	int	ddc_2_out_enb;		/* DC2EN , 1= On, 0 = Off */
	int ddc_2_out_vol;		/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV */
	int ddc_2_mode;			/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
	int ddc_2_mode_slp;		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
	int ddc_2_ramp_slop;	/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit */

	int	ddc_3_out_enb;		/* DC3EN , 1= On, 0 = Off */
	int ddc_3_out_vol;		/* VAL(uV) = 0: 0.60 ~ 3.5V, Step 12.5 mV */
	int ddc_3_mode;			/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
	int ddc_3_mode_slp;		/* VAL = 0:AUTO(default), 1:PWM, 2:PSM, 3:AUTO */
	int ddc_3_ramp_slop;	/* VAL(mV/us) = 0: 14, 1: 7, 2: 3.5, 3: prohibit */
};

struct nxe1100_batt_policy {
	int	batt_charge_enable;				/* R:CHGSTART, support battery charging when VCHG is high and battery is connected, VAL = 0: not support, 1: support */
	int	charge_voltage;					/* R:FET2CNT, Battery Charge Voltage (VBAT pin), VAL(V)= 0:4.2(default), 1:4.12, 2:4.07, 3:4.07 */
	int	charge_current;					/* R:FET2CNT, Battery Rapid Charge Current (VBAT pin), VAL(mA)= 0:100, 1:200, 2:300, 3:400, 4:500, 5:600, 6:700, 7:800, other:900 */
	int charge_time_up;					/* R:TSET, VAL(min)= 0:120(default), 1:180, 2:240, 3:300 */
	int charge_temperature_threshold;	/* R:TSET, VAL(C)= 0: detection (105), recovery (85), 1: detection (115), recovery (95), 2: detection (125), recovery (105) (default), 0: detection (135), recovery (115) */
	int charge_complete_current;		/* R:CMPSET, Charge complete current, VAL(mA)= 0:25(deefault), 1:50, 2:75, 3:100, 4:125:, 5:150, 6:175, 7:200 */
	int charge_ready_current;			/* R:SUSPEND, Charge ready current, VAL(mA)= 0:0(deefault), 1:10 */
};

struct nxe1100_intc_policy {
	/* Charge interrupt */
	int enb_adapter_in_out;			/* R:CHGEN1, Adapter insert & remove interrupt enable, VAL= 0:disable, 1:enable */
	int enb_die_temperature;		/* R:CHGEN1, Die abnormal temperature by SW1 or SW2 in charger interrupt enable, VAL= 0:disable, 1:enable */
	int enb_batt_temperature;		/* R:CHGEN1, Battery abnormal temperature interrupt enable, VAL= 0:disable, 1:enable */
	int enb_no_batt_dect;			/* R:CHGEN1, No Battery detect interrupt enable, VAL= 0:disable, 1:enable */
	int enb_adapter_over_vol;		/* R:CHGEN1, Adapter over voltage interrupt enable (VVCHG>6.2V), VAL= 0:disable, 1:enable */
	int enb_batt_over_vol;			/* R:CHGEN1, Battery over voltage interrupt enable (VVBAT > 4.6V), VAL= 0:disable, 1:enable */

	int enb_charge_ready;			/* R:CHGEN2, Shift to Charge-Ready state interrupt enable, VAL= 0:disable, 1:enable */
	int enb_charge_state;			/* R:CHGEN2,Shift to Rapid-Charge state interrupt enable, VAL= 0:disable, 1:enable */
	int enb_charge_complete;		/* R:CHGEN2, Charge complete interrupt enable, VAL= 0:disable, 1:enable */
	int enb_charge_time_up;			/* R:CHGEN2, Timer time out interrupt enable, VAL= 0:disable, 1:enable */
};

struct nxe1100_pmic_policy {
	struct nxe1100_vdc_policy	vdc;
	struct nxe1100_ldo_policy	ldo;
	struct nxe1100_dcdc_policy	dcdc;
	struct nxe1100_batt_policy	batt;
	struct nxe1100_intc_policy	intc;
};

/*
 * platform device data
 */
struct nxe1100_power {
	int							i2c_bus;
	int							i2c_addr;
	int							support_policy;
	int							warm_reset;
	struct nxe1100_pmic_policy	policy;
};

extern u8	nxe1100_get_ldo_step(u8 ldo_num, int want_vol);
extern u8	nxe1100_get_dcdc_step(u8 ldo_num, int want_vol);

extern int	nxe1100_device_setup(struct nxe1100_power *power);

#endif  //#ifndef __NXE1100_POWER_H_