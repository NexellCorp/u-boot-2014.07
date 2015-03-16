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

#include <config.h>
#include <common.h>
#include <mmc.h>
#include <pwm.h>
#include <asm/io.h>
#include <asm/gpio.h>

#include <platform.h>
#include <mach-api.h>
#include <rtc_nxp.h>
#include <pm.h>

#include <draw_lcd.h>

#include <power/pmic.h>
#include <power/battery.h>
#include <power/axp228.h>

#include <i2c.h>
#include <errno.h>


static u8 axp228_ocv_table[] = {
	OCVREG1,
	OCVREG2,
	OCVREG3,
	OCVREG4,
	OCVREG5,
	OCVREG6,
	OCVREG7,
	OCVREG8,
	OCVREG9,
	OCVREGA,
	OCVREGB,
	OCVREGC,
	OCVREGD,
	OCVREGE,
	OCVREGF,

	OCVREG10,
	OCVREG11,
	OCVREG12,
	OCVREG13,
	OCVREG14,
	OCVREG15,
	OCVREG16,
	OCVREG17,
	OCVREG18,
	OCVREG19,
	OCVREG1A,
	OCVREG1B,
	OCVREG1C,
	OCVREG1D,
	OCVREG1E,
	OCVREG1F,
};


static int axp228_i2c_read(struct axp228_power *power, u8 reg, u8 *value)
{
	uchar chip = power->i2c_addr;
	u32   addr = (u32)reg & 0xFF;
	int   alen = 1;
	return i2c_read(chip, addr, alen, value, 1);
}

static int axp228_i2c_write(struct axp228_power *power, u8 reg, u8 value)
{
	uchar chip = power->i2c_addr;
	u32   addr = (u32)reg & 0xFF;
	int   alen = 1;
	return i2c_write(chip, addr, alen, &value, 1);
}

static int axp228_i2c_set_bits(struct axp228_power *power, int reg, uint8_t bit_mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = axp228_i2c_read(power, reg, &reg_val);
	if (ret)
		return ret;

	if ((reg_val & bit_mask) != bit_mask) {
		reg_val |= bit_mask;
		ret = axp228_i2c_write(power, reg, reg_val);
	}
	return ret;
}

static int axp228_i2c_clr_bits(struct axp228_power *power, int reg, uint8_t bit_mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = axp228_i2c_read(power, reg, &reg_val);
	if (ret)
		return ret;

	if (reg_val & bit_mask) {
		reg_val &= ~bit_mask;
		ret = axp228_i2c_write(power, reg, reg_val);
	}
	return ret;
}

static int axp228_i2c_update(struct axp228_power *power, int reg, uint8_t val, uint8_t mask)
{
	u8 reg_val;
	int ret = 0;

	ret = axp228_i2c_read(power, reg, &reg_val);
	if (ret)
		return ret;

	if ((reg_val & mask) != val) {
		reg_val = (reg_val & ~mask) | val;
		ret = axp228_i2c_write(power, reg, reg_val);
	}
	return ret;
}


#if defined(CONFIG_PMIC_REG_DUMP)
void axp228_register_dump(struct axp228_power *power)
{
	int ret = 0;
	int i = 0;
	u8 value = 0;

	printf("##########################################################\n");
	printf("##\e[31m %s()\e[0m \n", __func__);
	printf("----------------------------------------------------------\n");
	printf("##      0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F\n");

	for(i=0; i<=AXP228_NUM_OF_REGS; i++)
	{
		if(i%16 == 0)
			printf("## %02X:", i);

		if(i%4 == 0)
			printf(" ");

		ret = axp228_i2c_read(power, i, &value);
		if(!ret)
		{
			printf("%02x ", value);
		}
		else
		{
			printf("\e[31mxx\e[0m: %d \n", ret);
			break;
		}

		if((i+1)%16 == 0)
			printf("\n");
	}
	printf("##########################################################\n");

}
#endif

static u8 axp228_get_vol_step(int want_vol, int step, int min, int max)
{
	u32	vol_step = 0;
	//u32	temp = 0;

	if (want_vol < min)
	{
		want_vol = min;
	}
	else if (want_vol > max)
	{
		want_vol = max;
	}

	//temp		= (want_vol - min);
	//vol_step	= (temp / step);

	vol_step = (want_vol - min + step - 1) / step;


	return	(u8)(vol_step & 0xFF);
}

static int axp228_param_setup(struct axp228_power *power)
{
	int ret=0, i=0;
	int var,tmp;
	u8 val2,val;
	int Cur_CoulombCounter,rdc;

	PMIC_DBGOUT("%s\n", __func__);

#if (CFG_POLY_PHASE_FUNCTION == 1)
	/* Enable DC-DC 2&3 Poly-phase Function*/
	axp228_i2c_set_bits(power, AXP22_DCDC_FREQSET, 0x10);
#endif


	/* REG 10H: DCDC1/2/3/4/5&ALDO1/2&DC5LDO Enable Set */
	val = 	( AXP_ALDO2_ENABLE << AXP_ALDO2_EN_BIT)
			|(AXP_ALDO1_ENABLE << AXP_ALDO1_EN_BIT)
			|(AXP_DCDC5_ENABLE << AXP_DCDC5_EN_BIT)
			|(AXP_DCDC4_ENABLE << AXP_DCDC4_EN_BIT)
			|(AXP_DCDC3_ENABLE << AXP_DCDC3_EN_BIT)
			|(AXP_DCDC2_ENABLE << AXP_DCDC2_EN_BIT)
			|(AXP_DCDC1_ENABLE << AXP_DCDC1_EN_BIT)
			|(AXP_DC5LDO_ENABLE << AXP_DC5LDO_EN_BIT);
	axp228_i2c_write(power, AXP22_LDO_DC_EN1, val);

	/* REG 12H: Power Output Control */
	val = 	( AXP_DC1SW_ENABLE << AXP_DC1SW_EN_BIT)
			|(AXP_DLDO4_ENABLE << AXP_DLDO4_EN_BIT)
			|(AXP_DLDO3_ENABLE << AXP_DLDO3_EN_BIT)
			|(AXP_DLDO2_ENABLE << AXP_DLDO2_EN_BIT)
			|(AXP_DLDO1_ENABLE << AXP_DLDO1_EN_BIT)
			|(AXP_ELDO3_ENABLE << AXP_ELDO3_EN_BIT)
			|(AXP_ELDO2_ENABLE << AXP_ELDO2_EN_BIT)
			|(AXP_ELDO1_ENABLE << AXP_ELDO1_EN_BIT);
	axp228_i2c_write(power, AXP22_LDO_DC_EN2, val);

	/* REG 13H:Power Output Control */
	val = 	( AXP_ALDO3_ENABLE << AXP_ALDO3_EN_BIT) | 0x00 ;
	axp228_i2c_write(power, AXP22_LDO_DC_EN3, val);

	/* REG 21H:DCDC1 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DCDC1_VALUE, AXP22_DCDC1_STEP, AXP22_DCDC1_MIN, AXP22_DCDC1_MAX);
	axp228_i2c_write(power, AXP22_DC1OUT_VOL, val);

	/* REG 22H:DCDC2 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DCDC2_VALUE, AXP22_DCDC2_STEP, AXP22_DCDC2_MIN, AXP22_DCDC2_MAX);
	axp228_i2c_write(power, AXP22_DC2OUT_VOL, val);

	/* REG 23H:DCDC3 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DCDC3_VALUE, AXP22_DCDC3_STEP, AXP22_DCDC3_MIN, AXP22_DCDC3_MAX);
	axp228_i2c_write(power, AXP22_DC3OUT_VOL, val);

	/* REG 24H:DCDC4 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DCDC4_VALUE, AXP22_DCDC4_STEP, AXP22_DCDC4_MIN, AXP22_DCDC4_MAX);
	axp228_i2c_write(power, AXP22_DC4OUT_VOL, val);

	/* REG 25H:DCDC5 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DCDC5_VALUE, AXP22_DCDC5_STEP, AXP22_DCDC5_MIN, AXP22_DCDC5_MAX);
	axp228_i2c_write(power, AXP22_DC5OUT_VOL, val);

	/* REG 15H:DLDO1 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DLDO1_VALUE, AXP22_DLDO1_STEP, AXP22_DLDO1_MIN, AXP22_DLDO1_MAX);
	axp228_i2c_write(power, AXP22_DLDO1OUT_VOL, val);

	/* REG 16H:DLDO2 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DLDO2_VALUE, AXP22_DLDO2_STEP, AXP22_DLDO2_MIN, AXP22_DLDO2_MAX);
	axp228_i2c_write(power, AXP22_DLDO2OUT_VOL, val);

	/* REG 17H:DLDO3 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DLDO3_VALUE, AXP22_DLDO3_STEP, AXP22_DLDO3_MIN, AXP22_DLDO3_MAX);
	axp228_i2c_write(power, AXP22_DLDO3OUT_VOL, val);

	/* REG 18H:DLDO4 Output Voltage Set */
	val = axp228_get_vol_step(AXP_DLDO4_VALUE, AXP22_DLDO4_STEP, AXP22_DLDO4_MIN, AXP22_DLDO4_MAX);
	axp228_i2c_write(power, AXP22_DLDO4OUT_VOL, val);

	/* REG 19H:ELDO1 Output Voltage Set */
	val = axp228_get_vol_step(AXP_ELDO1_VALUE, AXP22_ELDO1_STEP, AXP22_ELDO1_MIN, AXP22_ELDO1_MAX);
	axp228_i2c_write(power, AXP22_ELDO1OUT_VOL, val);

	/* REG 1AH:ELDO2 Output Voltage Set */
	val = axp228_get_vol_step(AXP_ELDO2_VALUE, AXP22_ELDO2_STEP, AXP22_ELDO2_MIN, AXP22_ELDO2_MAX);
	axp228_i2c_write(power, AXP22_ELDO2OUT_VOL, val);

	/* REG 1BH:ELDO3 Output Voltage Set */
	val = axp228_get_vol_step(AXP_ELDO3_VALUE, AXP22_ELDO3_STEP, AXP22_ELDO3_MIN, AXP22_ELDO3_MAX);
	axp228_i2c_write(power, AXP22_ELDO3OUT_VOL, val);

	/* REG 1CH:DC5LDO Output Voltage Set */
	val = axp228_get_vol_step(AXP_DC5LDO_VALUE, AXP22_DC5LDO_STEP, AXP22_DC5LDO_MIN, AXP22_DC5LDO_MAX);
	axp228_i2c_write(power, AXP22_DC5LDOOUT_VOL, val);

	/* REG 28H:ALDO1 Output Voltage Set */
	val = axp228_get_vol_step(AXP_ALDO1_VALUE, AXP22_ALDO1_STEP, AXP22_ALDO1_MIN, AXP22_ALDO1_MAX);
	axp228_i2c_write(power, AXP22_ALDO1OUT_VOL, val);

	/* REG 29H:ALDO2 Output Voltage Set */
	val = axp228_get_vol_step(AXP_ALDO2_VALUE, AXP22_ALDO2_STEP, AXP22_ALDO2_MIN, AXP22_ALDO2_MAX);
	axp228_i2c_write(power, AXP22_ALDO2OUT_VOL, val);

	/* REG 2AH:ALDO3 Output Voltage Set */
	val = axp228_get_vol_step(AXP_ALDO3_VALUE, AXP22_ALDO3_STEP, AXP22_ALDO3_MIN, AXP22_ALDO3_MAX);
	axp228_i2c_write(power, AXP22_ALDO3OUT_VOL, val);


	/* PWREN Control1 */
	val = (AXP_DCDC1_SLEEP_OFF << AXP_DCDC1_BIT)
			|(AXP_DCDC2_SLEEP_OFF << AXP_DCDC2_BIT)
			|(AXP_DCDC3_SLEEP_OFF << AXP_DCDC3_BIT)
			|(AXP_DCDC4_SLEEP_OFF << AXP_DCDC4_BIT)
			|(AXP_DCDC5_SLEEP_OFF << AXP_DCDC5_BIT)
			|(AXP_ALDO1_SLEEP_OFF << AXP_ALDO1_BIT)
			|(AXP_ALDO2_SLEEP_OFF << AXP_ALDO2_BIT)
			|(AXP_ALDO3_SLEEP_OFF << AXP_ALDO3_BIT);
	axp228_i2c_write(power, AXP22_PWREN_CTL1, val);

	/* PWREN Control2 */
	val = (AXP_DLDO1_SLEEP_OFF << AXP_DLDO1_BIT)
			|(AXP_DLDO1_SLEEP_OFF << AXP_DLDO2_BIT)
			|(AXP_DLDO1_SLEEP_OFF << AXP_DLDO3_BIT)
			|(AXP_DLDO1_SLEEP_OFF << AXP_DLDO4_BIT)
			|(AXP_ELDO1_SLEEP_OFF << AXP_ELDO1_BIT)
			|(AXP_ELDO1_SLEEP_OFF << AXP_ELDO2_BIT)
			|(AXP_ELDO1_SLEEP_OFF << AXP_ELDO3_BIT)
			|(AXP_DC5LDO_SLEEP_OFF << AXP_DC5LDO_BIT);
	axp228_i2c_write(power, AXP22_PWREN_CTL2, val);


	/* REG 31H: Wakeup Control and Voff Voltage Set */
	axp228_i2c_update(power, AXP22_VOFF_SET, 0x00, 0x7);


	/* REG 84H:ADC Sample rate Set ,TS Pin Control */
	axp228_i2c_set_bits(power, AXP22_ADC_CONTROL3,0x04);


	/* USB voltage limit */
	if((USBVOLLIM) && (USBVOLLIMEN))
	{
		axp228_i2c_set_bits(power, AXP22_CHARGE_VBUS, 0x40);
		var = USBVOLLIM * 1000;
		if(var >= 4000000 && var <=4700000)
		{
			tmp = (var - 4000000)/100000;
			axp228_i2c_read(power, AXP22_CHARGE_VBUS,&val);
			val &= 0xC7;
			val |= tmp << 3;
			axp228_i2c_write(power, AXP22_CHARGE_VBUS,val);
		}
	}
	else
	{
		axp228_i2c_clr_bits(power, AXP22_CHARGE_VBUS, 0x40);
	}


	/* USB current limit */
	if((USBCURLIM) && (USBCURLIMEN))
	{
		axp228_i2c_clr_bits(power, AXP22_CHARGE_VBUS, 0x02);
		var = USBCURLIM * 1000;
		if(var == 900000)
		{
			axp228_i2c_clr_bits(power, AXP22_CHARGE_VBUS, 0x03);
		}
		else if (var == 500000)
		{
			axp228_i2c_set_bits(power, AXP22_CHARGE_VBUS, 0x01);
		}
	}
	else
	{
		axp228_i2c_set_bits(power, AXP22_CHARGE_VBUS, 0x03);
	}


	/* AC current limit */
	val = (AC_LIMIT_CURRENT -200001)/150000;
	if(AC_LIMIT_CURRENT >= 300000 && AC_LIMIT_CURRENT <= 2550000)
		axp228_i2c_update(power, AXP22_CHARGE3, val,0x0F);
	else if(AC_LIMIT_CURRENT < 300000)
		axp228_i2c_clr_bits(power, AXP22_CHARGE3,0x0F);
	else
		axp228_i2c_set_bits(power, AXP22_CHARGE3,0x0F);


	/* AC current charge */
    if(AC_CHARGE_CURRENT == 0)
        axp228_i2c_clr_bits(power,AXP22_CHARGE1,0x80);
    else
        axp228_i2c_set_bits(power,AXP22_CHARGE1,0x80);

	val = (AC_CHARGE_CURRENT -200001)/150000;
	if(AC_CHARGE_CURRENT >= 300000 && AC_CHARGE_CURRENT <= 2550000)
		axp228_i2c_update(power, AXP22_CHARGE1, val,0x0F);
	else if(AC_LIMIT_CURRENT < 300000)
		axp228_i2c_clr_bits(power, AXP22_CHARGE1,0x0F);
	else
		axp228_i2c_set_bits(power, AXP22_CHARGE1,0x0F);


	// set lowe power warning/shutdown level
	axp228_i2c_write(power, AXP22_WARNING_LEVEL,((BATLOWLV1-5) << 4)+BATLOWLV2);


	/* OCV Table */
	for(i=0; i<= 0x1f; i++)
	{
		ret = axp228_i2c_write(power, AXP22_OCV_TABLE+i, axp228_ocv_table[i]);
	}


	/* pok open time set */
	axp228_i2c_read(power,AXP22_POK_SET,&val);
	if (PEKOPEN < 1000)
		val &= 0x3f;
	else if(PEKOPEN < 2000){
		val &= 0x3f;
		val |= 0x40;
	}
	else if(PEKOPEN < 3000){
		val &= 0x3f;
		val |= 0x80;
	}
	else {
		val &= 0x3f;
		val |= 0xc0;
	}
	axp228_i2c_write(power,AXP22_POK_SET,val);


	/* pok long time set*/
	if(PEKLONG < 1000)
		tmp = 1000;
	else if(PEKLONG > 2500)
		tmp = 2500;
	else
		tmp = PEKLONG;
	axp228_i2c_read(power,AXP22_POK_SET,&val);
	val &= 0xcf;
	val |= (((tmp - 1000) / 500) << 4);
	axp228_i2c_write(power,AXP22_POK_SET,val);


	/* pek offlevel poweroff en set*/
	if(PEKOFFEN)
		tmp = 1;
	else
		tmp = 0;			
	axp228_i2c_read(power,AXP22_POK_SET,&val);
	val &= 0xf7;
	val |= (tmp << 3);
	axp228_i2c_write(power,AXP22_POK_SET,val);


	/*Init offlevel restart or not */
	if(PEKOFFRESTART)
		axp228_i2c_set_bits(power,AXP22_POK_SET,0x04); //restart
	else
		axp228_i2c_clr_bits(power,AXP22_POK_SET,0x04); //not restart


	/* pek delay set */
	axp228_i2c_read(power,AXP22_OFF_CTL,&val);
	val &= 0xfc;
	val |= ((PEKDELAY / 8) - 1);
	axp228_i2c_write(power,AXP22_OFF_CTL,val);


	/* pek offlevel time set */
	if(PEKOFF < 4000)
		tmp = 4000;
	else if(PEKOFF > 10000)
		tmp =10000;
	else
		tmp = PEKOFF;
	tmp = (tmp - 4000) / 2000 ;
	axp228_i2c_read(power,AXP22_POK_SET,&val);
	val &= 0xfc;
	val |= tmp ;
	axp228_i2c_write(power,AXP22_POK_SET,val);


	/*Init 16's Reset PMU en */
	if(PMURESET)
		axp228_i2c_set_bits(power,0x8F,0x08); //enable
	else
		axp228_i2c_clr_bits(power,0x8F,0x08); //disable


	/*Init IRQ wakeup en*/
	if(IRQWAKEUP)
		axp228_i2c_set_bits(power,0x8F,0x80); //enable
	else
		axp228_i2c_clr_bits(power,0x8F,0x80); //disable


	/*Init N_VBUSEN status*/
	if(VBUSEN)
		axp228_i2c_set_bits(power,0x8F,0x10); //output
	else
		axp228_i2c_clr_bits(power,0x8F,0x10); //input


	/*Init InShort status*/
	if(VBUSACINSHORT)
		axp228_i2c_set_bits(power,0x8F,0x60); //InShort
	else
		axp228_i2c_clr_bits(power,0x8F,0x60); //auto detect


	/*Init CHGLED function*/
	if(CHGLEDFUN)
		axp228_i2c_set_bits(power,0x32,0x08); //control by charger
	else
		axp228_i2c_clr_bits(power,0x32,0x08); //drive MOTO


	/*set CHGLED Indication Type*/
	if(CHGLEDTYPE)
		axp228_i2c_set_bits(power,0x45,0x10); //Type A
	else
		axp228_i2c_clr_bits(power,0x45,0x10); //Type B


	/*Init PMU Over Temperature protection*/
	if(OTPOFFEN)
		axp228_i2c_set_bits(power,0x8f,0x04); //enable
	else
		axp228_i2c_clr_bits(power,0x8f,0x04); //disable


	/*Init battery capacity correct function*/
	if(BATCAPCORRENT)
		axp228_i2c_set_bits(power,0xb8,0x20); //enable
	else
		axp228_i2c_clr_bits(power,0xb8,0x20); //disable


	/* Init battery regulator enable or not when charge finish*/
	if(BATREGUEN)
		axp228_i2c_set_bits(power,0x34,0x20); //enable
	else
		axp228_i2c_clr_bits(power,0x34,0x20); //disable
 
	if(BATDET)
		axp228_i2c_set_bits(power,AXP22_PDBC,0x40);
	else
		axp228_i2c_clr_bits(power,AXP22_PDBC,0x40);
 	

	/* RDC initial */
	axp228_i2c_read(power, AXP22_RDC0,&val2);
	if((BATRDC) && (!(val2 & 0x40)))
	{
		rdc = (BATRDC * 10000 + 5371) / 10742;
		axp228_i2c_write(power, AXP22_RDC0, ((rdc >> 8) & 0x1F)|0x80);
		axp228_i2c_write(power,AXP22_RDC1,rdc & 0x00FF);
	}


	//probe RDC, OCV
	axp228_i2c_read(power,AXP22_BATFULLCAPH_RES,&val2);
	if((BATCAP) && (!(val2 & 0x80)))
	{
		Cur_CoulombCounter = BATCAP * 1000 / 1456;
		axp228_i2c_write(power, AXP22_BATFULLCAPH_RES, ((Cur_CoulombCounter >> 8) | 0x80));
		axp228_i2c_write(power, AXP22_BATFULLCAPL_RES, Cur_CoulombCounter & 0x00FF);		
	}
	else if(!BATCAP)
	{
		axp228_i2c_write(power, AXP22_BATFULLCAPH_RES, 0x00);
		axp228_i2c_write(power, AXP22_BATFULLCAPL_RES, 0x00);
	}

	return ret;
}

static int axp228_device_setup(struct axp228_power *power)
{
	int		bus = power->i2c_bus;
	int ret = 0;
#if defined(CONFIG_PMIC_REG_DUMP)
	int i;
	u8 value = 0;
#endif

	PMIC_DBGOUT("%s\n", __func__);

	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	i2c_set_bus_num(bus);

#if defined(CONFIG_PMIC_REG_DUMP)
	printf("##########################################################\n");
	printf("##\e[31m PMIC OTP Value \e[0m \n");
	printf("##      0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F\n");
	for(i=0; i<=AXP228_NUM_OF_REGS; i++)
	{
		if(i%16 == 0)
			printf("## %02X:", i);
		if(i%4 == 0)
			printf(" ");

		ret = axp228_i2c_read(power, i, &value);
		if(!ret)
		{
			printf("%02x ", value);
		}
		else
		{
			printf("\e[31mxx\e[0m: %d \n", ret);
			break;
		}
		if((i+1)%16 == 0)
			printf("\n");
	}
	printf("##########################################################\n");
#endif

	axp228_param_setup(power);

#if defined(CONFIG_PMIC_REG_DUMP)
	axp228_register_dump(power);
#endif

	return ret;
}


int power_pmic_function_init(void)
{
	int ret = 0;
	int i2c_bus = CONFIG_PMIC_I2C_BUS;
	PMIC_DBGOUT("%s\n", __func__);

	ret = power_pmic_init(i2c_bus);
	ret = power_bat_init(i2c_bus);
	ret = power_muic_init(i2c_bus);
	ret = power_fg_init(i2c_bus);

	return ret;
}

int bd_pmic_init(void)
{
	struct axp228_power nxe_power_config = {
		.i2c_addr = AXP228_I2C_ADDR,
		.i2c_bus = CONFIG_PMIC_I2C_BUS,
	};
	PMIC_DBGOUT("%s\n", __func__);

	axp228_device_setup(&nxe_power_config);
	return 0;
}

#if defined(CONFIG_BAT_CHECK)
#define CONFIG_BAT_GAUGE_CNT		4

static int skip_check(struct power_battery *pb, int bat_state)
{
	static int skip_bat_ani = 0;
	int ret = 0;

	if(bat_state != 3)
	{
		if (!gpio_get_value(GPIO_POWER_KEY_DET))
			skip_bat_ani++;
		else
			skip_bat_ani = 0;

		if ((skip_bat_ani > 20) && (pb->bat->capacity > BATLOW_ANIMATION_CAP))
		{
			printf("\n");
			ret = 1;
		}
	}

	if(ctrlc())
	{
		printf("\n");
		ret = 1;
	}

	return ret;	
}


int power_battery_check(int skip, void (*bd_display_run)(char *, int, int))
{
#if defined(CONFIG_DISPLAY_OUT)
	lcd_info lcd = {
		.fb_base		= CONFIG_FB_ADDR,
		.bit_per_pixel	= CFG_DISP_PRI_SCREEN_PIXEL_BYTE * 8,
		.lcd_width		= CFG_DISP_PRI_RESOL_WIDTH,
		.lcd_height		= CFG_DISP_PRI_RESOL_HEIGHT,
		.back_color		= 0x000000,
		.text_color		= (0<<16) + (255 << 8) + (255),
		.dbg_win_left	= 10,
		.dbg_win_width	= 1260,
		.dbg_win_top	= 10,
		.dbg_win_height	= 780,
		.alphablend		= 0,
	};

#if defined(CONFIG_PMIC_REG_DUMP)
	struct axp228_power nxe_power_config = {
		.i2c_addr = AXP228_I2C_ADDR,
		.i2c_bus = CONFIG_PMIC_I2C_BUS,
	};
#endif

	struct power_battery *pb;
	struct pmic *p_fg, *p_chrg, *p_muic, *p_bat;

	int ret=0, i=0;
	int chrg;
	int bl_duty = CFG_LCD_PRI_PWM_DUTYCYCLE;
	int show_bat_state = 0;
	int skip_bat_ani = 0;
	u8  power_depth = 6;
	u32 chg_state=0, val=0;

	PMIC_DBGOUT("%s\n", __func__);

	p_chrg = pmic_get("PMIC_AXP228");
	if (!p_chrg) {
		printf("PMIC_AXP228: Not found\n");
		return -ENODEV;
	}

	p_bat = pmic_get("BAT_AXP228");
	if (!p_bat) {
		printf("BAT_AXP228: Not found\n");
		return -ENODEV;
	}

	p_muic = pmic_get("MUIC_AXP228");
	if (!p_muic) {
		printf("MUIC_AXP228: Not found\n");
		return -ENODEV;
	}

	p_fg = pmic_get("FG_AXP228");
	if (!p_fg) {
		printf("FG_AXP228: Not found\n");
		return -ENODEV;
	}

	p_bat->low_power_mode = NULL;
	p_bat->pbat->battery_init(p_bat, p_fg, p_chrg, p_muic);
	pb = p_bat->pbat;

	chrg = p_muic->chrg->chrg_type(p_muic, 1);
	p_fg->fg->fg_battery_check(p_fg, p_bat);


	printf("## STATUS(0x%02x) : ", AXP22_STATUS);
	for(i=AXP22_STATUS; i<=AXP22_MODE_CHGSTATUS; i++)
	{
		pmic_reg_read(p_chrg, (u32)i, &val);	printf("0x%02x ", val);
	}
	printf("\n");
	printf("## IRQ(0x%02x)    : ", AXP22_INTSTS1);
	for(i=AXP22_INTSTS1; i<=AXP22_INTSTS5; i++)
	{
		pmic_reg_read(p_chrg, (u32)i, &val);	printf("0x%02x ", val);
	}
	printf("\n");
	printf("## CHG_TYPE     : %s\n", chrg == CHARGER_USB ? "USB" : (chrg == CHARGER_TA ? (((chg_state>>6) & 0x2) ? "ADP(USB)" : "ADP"): "NONE"));
	printf("## BAT_CAP      : %d%%\n", pb->bat->capacity);

	if(pb->bat->capacity <= BATLOW_ANIMATION_CAP)
	{
		show_bat_state = 2;
		skip_bat_ani = 0;
		if(chrg == CHARGER_NO || chrg == CHARGER_UNKNOWN)
		{
			bl_duty = (CFG_LCD_PRI_PWM_DUTYCYCLE / 25);
			power_depth = 3;
		}
	}
	else if(chrg == CHARGER_NO || chrg == CHARGER_UNKNOWN)
	{
		show_bat_state = 0;
		skip_bat_ani = 2;
	}
	else
	{
		show_bat_state = 1;
		skip_bat_ani = 0;
	}

	/*===========================================================*/
	if(skip)
		skip_bat_ani = 2;

	/*===========================================================*/
    if (skip_bat_ani > 1)
    {
        //bd_display_run(CONFIG_CMD_LOGO_WALLPAPERS, bl_duty, 1);
        goto skip_bat_animation;
    }
    else if (show_bat_state)
    {
        memset((void*)lcd.fb_base, 0, lcd.lcd_width * lcd.lcd_height * (lcd.bit_per_pixel/8));
        bd_display_run(CONFIG_CMD_LOGO_BATTERY, bl_duty, 1);
    }

	/*===========================================================*/
	// draw charing image
	if (show_bat_state)
	{
		int lcdw = lcd.lcd_width, lcdh = lcd.lcd_height;
		int bmpw = 240, bmph = 320;
#if (CONFIG_BAT_GAUGE_CNT == 10)
		int bw = 82, bh = 22;
		int bx = 80, by = 61;
		int sx, sy, dy, str_dy, clr_str_size;
		unsigned int color = (54<<16) + (221 << 8) + (19);
		int i=0, cnt=0, time_delay = 1000, msec = 10;
#else
		int bw = 82, bh = 55;
		int bx = 80, by = 68;
		int sx, sy, dy, str_dy, clr_str_size;
		unsigned int color = (54<<16) + (221 << 8) + (19);
		int i=0, cnt=0, msec = 10;
#endif
		char *str_charging		= " Charging...   ";
		char *str_discharging	= " Discharging...";
		char *str_lowbatt  		= " Low Battery...";
		char *str_nobatt  		= " No Battery... ";

		clr_str_size = max(strlen(str_charging), strlen(str_lowbatt));
		sx = (lcdw - bmpw)/2 + bx;
		sy = (lcdh - bmph)/2 + by;
		dy = sy + (bh+2)*(CONFIG_BAT_GAUGE_CNT-1);
		str_dy = dy;

		if(!p_chrg->chrg->chrg_bat_present(p_chrg))
		{
			printf("## No Battery \n");
			show_bat_state = 3;
		}

		lcd_debug_init(&lcd);

		if(show_bat_state == 3)
			lcd_draw_text(str_nobatt, (lcdw - strlen(str_lowbatt)*8*3)/2 + 30, str_dy+100, 3, 3, 0);
		else if(show_bat_state == 2)
			lcd_draw_text(str_lowbatt, (lcdw - strlen(str_lowbatt)*8*3)/2 + 30, str_dy+100, 3, 3, 0);
		else
		{
			if(chrg == CHARGER_NO || chrg == CHARGER_UNKNOWN)
				lcd_draw_text(str_discharging, (lcdw - strlen(str_discharging)*8*3)/2 + 30, str_dy+100, 3, 3, 0);
			else
				lcd_draw_text(str_charging, (lcdw - strlen(str_charging)*8*3)/2 + 30, str_dy+100, 3, 3, 0);
		}

		while(1)
		{
			printf(".");
			p_fg->fg->fg_battery_check(p_fg, p_bat);

#if (CONFIG_BAT_GAUGE_CNT == 10)
			if(pb->bat->capacity < 50)
				time_delay = 100;
			else
				time_delay = 50;

			if(pb->bat->capacity < 10)
				cnt = 1;
			else if(pb->bat->capacity < 20)
				cnt = 2;
			else if(pb->bat->capacity < 30)
				cnt = 3;
			else if(pb->bat->capacity < 40)
				cnt = 4;	
			else if(pb->bat->capacity < 50)
				cnt = 5;	
			else if(pb->bat->capacity < 60)
				cnt = 6;	
			else if(pb->bat->capacity < 70)
				cnt = 7;	
			else if(pb->bat->capacity < 80)
				cnt = 8;	
			else if(pb->bat->capacity < 90)
				cnt = 9;	
			else
				cnt = 10;

			if(i == 0)
				color = (255<<16) + (0 << 8) + (0);
			else if(i == 1)
				color = (255<<16) + (80 << 8) + (0);
			else if(i ==  2)
				color = (255<<16) + (150 << 8) + (0);
			else if(i ==  3)
				color = (255<<16) + (200 << 8) + (0);
			else if(i ==  4)
				color = (255<<16) + (255 << 8) + (0);
			else if(i ==  5)
				color = (255<<16) + (255 << 8) + (0);
			else if(i ==  6)
				color = (200<<16) + (255 << 8) + (0);
			else if(i ==  7)
				color = (150<<16) + (255 << 8) + (0);	
			else if(i ==  8)
				color = (80<<16) + (255 << 8) + (0);
			else
				color = (0<<16) + (255 << 8) + (0);

#else
			if(pb->bat->capacity < 20)
			{
				color = (255<<16) + (0 << 8) + (0);		cnt = 1;
			}
			else if(pb->bat->capacity < 50)
			{
				color = (255<<16) + (150 << 8) + (0);		cnt = 2;	
			}
			else if(pb->bat->capacity < 75)
			{
				color = (150<<16) + (255 << 8) + (0);		cnt = 3;	
			}
			else
			{
				color = (0<<16) + (255 << 8) + (0);		cnt = 4;
			}
#endif
			if(skip_check(pb, show_bat_state))
			{
		        goto skip_bat_animation;
			}

			/* Draw battery status */
			if(power_depth > 0)
			{
				i++;
				if(i <= cnt)
				{
					lcd_fill_rectangle(sx, dy, bw, bh, color, 0);
					dy -= (bh+2);
				}
				else
				{
					power_depth--;
#if (CONFIG_BAT_GAUGE_CNT == 10)
					while(msec--)
					{
						if(skip_check(pb, show_bat_state))
						{
					        goto skip_bat_animation;
						}
						udelay(100000);
					}
					msec = 10;
#endif
					printf("\n");
					lcd_draw_boot_logo(CONFIG_FB_ADDR, CFG_DISP_PRI_RESOL_WIDTH, CFG_DISP_PRI_RESOL_HEIGHT, CFG_DISP_PRI_SCREEN_PIXEL_BYTE);
					dy = sy + (bh+2)*(CONFIG_BAT_GAUGE_CNT-1);
					i = 0;
				}

				if(show_bat_state == 3)
					lcd_draw_text(str_nobatt, (lcdw - strlen(str_lowbatt)*8*3)/2 + 30, str_dy+100, 3, 3, 0);
				else if(show_bat_state == 2)
					lcd_draw_text(str_lowbatt, (lcdw - strlen(str_lowbatt)*8*3)/2 + 30, str_dy+100, 3, 3, 0);
				else
				{
					if(chrg == CHARGER_NO || chrg == CHARGER_UNKNOWN)
						lcd_draw_text(str_discharging, (lcdw - strlen(str_discharging)*8*3)/2 + 30, str_dy+100, 3, 3, 0);
					else
						lcd_draw_text(str_charging, (lcdw - strlen(str_charging)*8*3)/2 + 30, str_dy+100, 3, 3, 0);
				}
			}
			else
			{
				printf("\n");
				lcd_draw_boot_logo(CONFIG_FB_ADDR, CFG_DISP_PRI_RESOL_WIDTH, CFG_DISP_PRI_RESOL_HEIGHT, CFG_DISP_PRI_SCREEN_PIXEL_BYTE);
				goto enter_shutdown;
			}
#if (CONFIG_BAT_GAUGE_CNT == 10)
			mdelay(time_delay);
#else
			while(msec--)
			{
				if(skip_check(pb, show_bat_state))
				{
			        goto skip_bat_animation;
				}
				udelay(100000);
			}
			msec = 10;
#endif
		}

	}

skip_bat_animation:
#endif  /* CONFIG_DISPLAY_OUT */

	bd_display_run(CONFIG_CMD_LOGO_WALLPAPERS, bl_duty, 1);
	printf("## Skip BAT Animation. \n");
	//mdelay(200);

#if defined(CONFIG_PMIC_REG_DUMP)
	axp228_register_dump(&nxe_power_config);
#endif

	p_fg->fg->fg_battery_check(p_fg, p_bat);

	printf("## IRQ(0x%02x)   : ", AXP22_INTSTS1);
	for(i=AXP22_INTSTS1; i<=AXP22_INTSTS5; i++)
	{
		pmic_reg_read(p_chrg, (u32)i, &val);	printf("0x%02x ", val);	pmic_reg_write(p_chrg, (u32)i, 0xff);
	}
	printf("\n");
	printf("## chg_type    : %s \n", (chrg == CHARGER_USB ? "USB" : (chrg == CHARGER_TA ? (((chg_state>>6) & 0x2) ? "ADP(USB)" : "ADP"): "NONE")));
	printf("## battery_cap : %d%%\n", pb->bat->capacity);
	printf("## Booting \n");

	//chrg = p_muic->chrg->chrg_type(p_muic, 1);
	if (chrg == CHARGER_USB)
	{
		ret = 1;
	}

	return ret;

enter_shutdown:

#if defined(CONFIG_PMIC_REG_DUMP)
	axp228_register_dump(&nxe_power_config);
#endif
	p_fg->fg->fg_battery_check(p_fg, p_bat);

	printf("## IRQ(0x%02x)   : ", AXP22_INTSTS1);
	for(i=AXP22_INTSTS1; i<=AXP22_INTSTS5; i++)
	{
		pmic_reg_read(p_chrg, (u32)i, &val);	printf("0x%02x ", val);	pmic_reg_write(p_chrg, (u32)i, 0xff);
	}
	printf("\n");
	printf("## chg_type    : %s \n", (chrg == CHARGER_USB ? "USB" : (chrg == CHARGER_TA ? (((chg_state>>6) & 0x2) ? "ADP(USB)" : "ADP"): "NONE")));
	printf("## battery_cap : %d%%\n", pb->bat->capacity);
	printf("## Power Off\n");

	mdelay(500);

	pmic_reg_write(p_chrg, AXP22_BUFFERC, 0x00);
	mdelay(20);

	pmic_reg_read(p_chrg, AXP22_OFF_CTL, &val);
	val |= 0x80;
	pmic_reg_write(p_chrg, AXP22_OFF_CTL, val);
	
	while(1);

	return 0;
}
#endif /* CONFIG_BAT_CHECK */


