include $(srctree)/config.mk

extra-y := start.o vectors.o

obj-y 	+= low_init.o
obj-y 	+= cpu.o
obj-y 	+= s5p4418.o
obj-y 	+= clock.o
obj-y 	+= reset.o
obj-y 	+= timer.o
obj-y 	+= device.o
obj-y 	+= ../common/cache_v7.o

obj-$(CONFIG_MMU_ENABLE) += mmu.o mmu_asm.o
obj-$(CONFIG_SMP) += ../devices/gic.o

obj-$(CONFIG_NAND_FTL) += ../devices/mio/

include $(srctree)/arch/arm/cpu/$(CPU)/$(SOC)/prototype.mk
include $(srctree)/arch/arm/cpu/$(CPU)/$(SOC)/devices.mk
