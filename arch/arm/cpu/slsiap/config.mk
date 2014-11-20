
# =========================================================================
#	u-boot.lds path
# =========================================================================
SOCDIR=arch/$(ARCH)/cpu$(if $(CPU),/$(CPU),)$(if $(SOC),/$(SOC),)
LDPPFLAGS += -DSOCDIR=$(SOCDIR)

#
# avoid build error complict with multiple defined rasie,,,, in eabi_compat.c
# when export CROSS_COMPILE	(arch/arm/config.mk)
#
PLATFORM_LIBS :=
