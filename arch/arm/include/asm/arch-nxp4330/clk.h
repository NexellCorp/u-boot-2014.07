#ifndef __ASM_ARM_ARCH_CLK_H_
#define __ASM_ARM_ARCH_CLK_H_

#include <clkdev.h>

typedef unsigned int 					spinlock_t;
#define	spin_lock_init(l)				do { } while (0)
#define	spin_lock_irqsave(l, f)			(f=*(unsigned int*)l)
#define	spin_unlock_irqrestore(l, f)	(*(unsigned int*)l=f)
#define	__init

#define	__init
struct device {
	char *init_name;
};

#define	dev_name(d)		(d->init_name)
#define	EXPORT_SYMBOL(_f)

#endif
