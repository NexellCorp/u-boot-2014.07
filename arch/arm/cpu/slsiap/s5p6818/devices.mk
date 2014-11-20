
obj-$(CONFIG_S5P_SERIAL)			+= ../devices/serial_s5p.o
obj-$(CONFIG_PWM)					+= ../devices/pwm.o
obj-$(CONFIG_MTD_NAND_NXP)		+= ../devices/nand.o
obj-$(CONFIG_MTD_NAND_ECC_HW)		+= ../devices/nand_ecc.o
obj-$(CONFIG_CMD_I2C)				+= ../devices/i2c_gpio.o
obj-$(CONFIG_LCD)					+= ../devices/lcd.o
obj-$(CONFIG_SPI)					+= ../devices/spi_slsi.o
obj-$(CONFIG_PMIC_NXE1100)			+= ../devices/nxe1100_power.o
obj-$(CONFIG_PMIC_NXE2000)			+= ../devices/nxe2000_power.o
obj-$(CONFIG_POWER_BATTERY_NXE2000)	+= ../devices/nxe2000_bat.o
obj-$(CONFIG_POWER_FG_NXE2000)		+= ../devices/nxe2000_fg.o
obj-$(CONFIG_POWER_NXE2000)			+= ../devices/nxe2000_pmic.o
obj-$(CONFIG_POWER_MUIC_NXE2000)	+= ../devices/nxe2000_muic.o
obj-$(CONFIG_NXP_DWMMC)				+= ../devices/dw_mmc.o
obj-$(CONFIG_DISPLAY_OUT)			+= ../devices/display_dev.o
obj-$(CONFIG_DISPLAY_OUT_LVDS)		+= ../devices/display_lvds.o
obj-$(CONFIG_DISPLAY_OUT_HDMI)		+= ../devices/display_hdmi.o
obj-$(CONFIG_DISPLAY_OUT_MIPI)		+= ../devices/display_mipi.o
obj-$(CONFIG_DISPLAY_OUT_RGB)		+= ../devices/display_rgb.o
obj-$(CONFIG_GPIOLIB_NXP)			+= ../devices/gpio_nxp.o
obj-$(CONFIG_NXP_RTC_USE)			+= ../devices/rtc_nxp.o
obj-$(CONFIG_NXP_DWC_OTG) 			+= ../devices/dwc_otg_hs.o
obj-$(CONFIG_NXP_DWC_OTG_PHY)		+= ../devices/dwc_otg_phy.o

