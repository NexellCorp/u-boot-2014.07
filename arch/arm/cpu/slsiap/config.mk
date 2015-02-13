
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

GCCVERSION =  $(shell $(CC) -dumpversion | cut -f2 -d.)

#ifeq "$(GCCVERSION)" "7"
#PLATFORM_RELFLAGS += -mno-unaligned-access
#endif
