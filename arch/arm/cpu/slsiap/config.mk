
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

GCCMACHINE =  $(shell $(CC) -dumpmachine | cut -f1 -d-)
GCCVERSION =  $(shell $(CC) -dumpversion | cut -f2 -d.)

ifeq "$(GCCMACHINE)" "arm"
ifeq "$(GCCVERSION)" "7"
PLATFORM_RELFLAGS += -mno-unaligned-access
endif
ifeq "$(GCCVERSION)" "8"
PLATFORM_RELFLAGS += -mno-unaligned-access
endif
endif	# ifeq "$(GCCMACHINE)" "arm"
