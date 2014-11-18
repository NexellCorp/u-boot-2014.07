include $(srctree)/config.mk

extra-y := start.o vectors.o

obj-y 	+= cache_v7.o
obj-y 	+= low_init.o
obj-y 	+= cpu.o
obj-y 	+= s5p6818.o
obj-y 	+= clock.o
obj-y 	+= reset.o
obj-y 	+= timer.o
obj-y 	+= device.o

obj-$(CONFIG_MMU_ENABLE) += mmu.o mmu_asm.o

include $(srctree)/arch/arm/cpu/$(CPU)/$(SOC)/prototype.mk
include $(srctree)/arch/arm/cpu/$(CPU)/$(SOC)/devices.mk
