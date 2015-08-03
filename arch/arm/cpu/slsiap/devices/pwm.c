/*
 * (C) Copyright 2009
 * jung hyun kim, Nexell Co, <jhkim@nexell.co.kr>
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
#include <common.h>
#include <errno.h>
#include <asm/io.h>

#include <platform.h>
#include "mach-api.h"

struct pwm_device {
	int ch;
	int io;
	int fn;
	struct clk *clk;
	unsigned long max_freq;
};

static struct pwm_device pwm_dev[] = {
	[0] = { .ch	= 0, .io = (PAD_GPIO_D +  1), .fn = NX_GPIO_PADFUNC_1, },
	[1] = { .ch	= 1, .io = (PAD_GPIO_C + 13), .fn = NX_GPIO_PADFUNC_2, },
	[2] = { .ch = 2, .io = (PAD_GPIO_C + 14), .fn = NX_GPIO_PADFUNC_2, },
	[3] = { .ch = 3, .io = (PAD_GPIO_D +  0), .fn = NX_GPIO_PADFUNC_2, },
};

#define PWM_COMPARE(c, d) (((10 > c ? c * 10 : c) * d) / (100 * (10 > c ? 10 : 1)))
#define NS_IN_HZ (1000000000UL)
#define TO_PERCENT(duty, period)        (((duty)*100)/(period))
#define TO_HZ(period)                   (NS_IN_HZ/(period))

/*
 * PWM HW
 */
#define	PWM_CFG0		(0x00)
#define	PWM_CFG1		(0x04)
#define	PWM_TCON		(0x08)
#define	PWM_CNTB		(0x0C)
#define	PWM_CMPB		(0x10)
#define	PWM_CNTO		(0x14)
#define	PWM_STAT		(0x44)

#define	TCON_AUTO		(1<<3)
#define	TCON_INVT		(1<<2)
#define	TCON_UP			(1<<1)
#define	TCON_RUN		(1<<0)
#define CFG0_CH(ch)		(ch == 0 || ch == 1 ? 0 : 8)
#define CFG1_CH(ch)		(ch * 4)
#define TCON_CH(ch)		(ch ? ch * 4  + 4 : 0)
#define TINT_CH(ch)		(ch)
#define TINT_CS_CH(ch)	(ch + 5)
#define	TINT_CS_MASK	(0x1F)
#define PWM_CH_OFFS	 	(0xC)

#define	PWM_STATUS_STOP	(0<<0)
#define	PWM_STATUS_RUN	(1<<0)

#define	PWM_BASE	IO_ADDRESS(PHY_BASEADDR_PWM)
#define	DUTY_MIN_VAL	2

static inline void pwm_clock(int ch, int mux, int scl)
{
	volatile U32 val;

	val  = readl(PWM_BASE + PWM_CFG0);
	val &= ~(0xFF   << CFG0_CH(ch));
	val |=  ((scl-1)<< CFG0_CH(ch));
	writel(val, PWM_BASE + PWM_CFG0);

	val  = readl(PWM_BASE + PWM_CFG1);
	val &= ~(0xF << CFG1_CH(ch));
	val |=  (mux << CFG1_CH(ch));
	writel(val, PWM_BASE + PWM_CFG1);
}

static inline void pwm_count(int ch, unsigned int cnt, unsigned int cmp)
{
	writel((cnt-1), ((ulong)(PWM_BASE + PWM_CNTB + (PWM_CH_OFFS * ch))));
	writel((cmp-1), ((ulong)(PWM_BASE + PWM_CMPB + (PWM_CH_OFFS * ch))));
}

static inline void pwm_start(int ch, int irqon)
{
	volatile U32 val;
	int on = irqon ? 1 : 0;

	val  = readl(PWM_BASE + PWM_STAT);
	val &= ~(TINT_CS_MASK<<5 | 0x1 << TINT_CH(ch));
	val |=  (0x1 << TINT_CS_CH(ch) | on << TINT_CH(ch));
	writel(val, PWM_BASE + PWM_STAT);

	val = readl(PWM_BASE + PWM_TCON);
	val &= ~(0xE << TCON_CH(ch));
	val |=  (TCON_UP << TCON_CH(ch));
	writel(val, PWM_BASE + PWM_TCON);

	val &= ~(TCON_UP << TCON_CH(ch));
	val |=  ((TCON_AUTO | TCON_RUN) << TCON_CH(ch));	/* set pwm out invert ? */
	writel(val, PWM_BASE + PWM_TCON);
}

/*
 * PWM APIs
 */
int pwm_init(int pwm_id, int div, int invert)
{
	int ch = pwm_id;
	struct clk *clk = clk_get(NULL, CORECLK_NAME_PCLK);
	char name[16];

	pwm_dev[ch].max_freq = clk_get_rate(clk);
	sprintf(name, "%s.%d", DEV_NAME_PWM, ch);
printf("[%s] PWM NAME : %s\n", __func__, name);
	pwm_dev[ch].clk = clk_get(NULL, name);

#if defined(CONFIG_MACH_S5P4418)
	NX_RSTCON_SetnRST(RESET_ID_PWM, RSTCON_nENABLE);
#elif defined(CONFIG_MACH_S5P6818)
	NX_RSTCON_SetRST(RESET_ID_PWM, RSTCON_NEGATE);
#endif
	return 0;
}

int pwm_config(int pwm_id, int duty_ns, int period_ns)
{
	printf("[%s] pwm_id : %d\n", __func__, pwm_id);
	printf("[%s] duty_ns : %d\n", __func__, duty_ns);
	printf("[%s] period_ns : %d\n", __func__, period_ns);

	int ch = pwm_id;
	unsigned int request = TO_HZ(period_ns);
	int duty = TO_PERCENT(duty_ns, period_ns);

	printf("[%s] ch : %d\n", __func__, ch);
	printf("[%s] duty : %d\n", __func__, duty);
	printf("[%s] request : %d\n", __func__, request);

	struct clk *clk = pwm_dev[ch].clk;
	unsigned long max_clock = pwm_dev[ch].max_freq;
	unsigned long rate, freq, clock = 0;
	unsigned long hz = 0, pwmhz = 0;
	unsigned int tcnt;
	int i, n, end = 0;
	unsigned int counter, compare;

	int grp = PAD_GET_GROUP(pwm_dev[ch].io);
	int bit = PAD_GET_BITNO(pwm_dev[ch].io);

	if (duty == 0 || duty >= 100) {
		NX_GPIO_SetOutputValue(grp, bit, duty == 0 ? CFALSE : CTRUE);
		return 0;
	}

	for (n = 1; !end; n *= 10) {
		for (i = (n == 1 ? DUTY_MIN_VAL : 1); 10 > i; i++) {
			freq = request * i * n;
			if (freq > max_clock) {
				end = 1;
				break;
			}
			rate = clk_round_rate(clk, freq);
			tcnt = rate/request;
			hz   = rate/tcnt;
			if (0 == rate%request) {
				clock = rate, pwmhz = hz, end = 1;
				break;
			}
			if (hz && (abs(hz-request) >= abs(pwmhz-request)))
				continue;
			clock = rate, pwmhz = hz;
		}
	}

	clk_set_rate(clk, clock);
	clk_enable(clk);

	counter = (clock/request);
	compare = PWM_COMPARE(counter, duty) ? : 1;

	PWM_COMPARE(counter, duty) ? : 1;
	pwm_clock(ch, 5, 1);
	pwm_count(ch, counter, compare);
	pwm_start(ch, 0);

	printf("[%s] rate(clock) : %ld\n", __func__, clock);
	printf("[%s] duty : %d\n", __func__, duty);
	printf("[%s] pwn_hz  : %ld\n", __func__, pwmhz);
	printf("[%s] counter  : %d\n", __func__, counter);
	printf("[%s] compare  : %d\n", __func__, compare);

	/* PWM altfunction */
	NX_GPIO_SetPadFunction(grp, bit, pwm_dev[ch].fn);
	return 0;
}

int pwm_config_camera(int ch, unsigned int request, unsigned int duty)
{
	//printf("[%s] pwm_id : %d\n", __func__, pwm_id);
	//printf("[%s] duty_ns : %d\n", __func__, duty_ns);
	//printf("[%s] period_ns : %d\n", __func__, period_ns);

	printf("[%s] ch : %d\n", __func__, ch);
	printf("[%s] duty : %d\n", __func__, duty);
	printf("[%s] request : %d\n", __func__, request);

	struct clk *clk = pwm_dev[ch].clk;
	unsigned long max_clock = pwm_dev[ch].max_freq;
	unsigned long rate, freq, clock = 0;
	unsigned long hz = 0, pwmhz = 0;
	unsigned int tcnt;
	int i, n, end = 0;
	unsigned int counter, compare;

	int grp = PAD_GET_GROUP(pwm_dev[ch].io);
	int bit = PAD_GET_BITNO(pwm_dev[ch].io);

	if (duty == 0 || duty >= 100) {
		NX_GPIO_SetOutputValue(grp, bit, duty == 0 ? CFALSE : CTRUE);
		return 0;
	}

	for (n = 1; !end; n *= 10) {
		for (i = (n == 1 ? DUTY_MIN_VAL : 1); 10 > i; i++) {
			freq = request * i * n;
			printf("[%s] freq : %ld, max_clock : %ld\n", __func__, freq, max_clock);
			if (freq > max_clock) {
				printf("[%s] Line : %d\n", __func__, __LINE__);
				end = 1;
				break;
			}
			rate = clk_round_rate(clk, freq);
			printf("[%s] rate: %ld, freq : %ld\n", __func__, rate, freq);
			tcnt = rate/request;
			printf("[%s] tcnt : %d\n", __func__, tcnt);
			hz   = rate/tcnt;
			printf("[%s] hz: %ld\n", __func__, hz);
			if (0 == rate%request) {
				clock = rate, pwmhz = hz, end = 1;
				printf("[%s - %d] clock : %ld, pwmhz : %ld\n", __func__, __LINE__, clock, pwmhz);
				break;
			}
			if (hz && (abs(hz-request) >= abs(pwmhz-request)))
				continue;
			clock = rate, pwmhz = hz;
			printf("[%s - %d] clock : %ld, pwmhz : %ld\n", __func__, __LINE__, clock, pwmhz);
		}
	}

	clk_set_rate(clk, clock);
	clk_enable(clk);

	counter = (clock/request);
	compare = PWM_COMPARE(counter, duty) ? : 1;

	PWM_COMPARE(counter, duty) ? : 1;
	pwm_clock(ch, 5, 1);
	pwm_count(ch, counter, compare);
	pwm_start(ch, 0);

	printf("[%s] rate(clock) : %ld\n", __func__, clock);
	printf("[%s] duty : %d\n", __func__, duty);
	printf("[%s] pwn_hz  : %ld\n", __func__, pwmhz);
	printf("[%s] counter  : %d\n", __func__, counter);
	printf("[%s] compare  : %d\n", __func__, compare);

	/* PWM altfunction */
	NX_GPIO_SetPadFunction(grp, bit, pwm_dev[ch].fn);
	return 0;
}

int pwm_enable(int pwm_id)
{
	int ch = pwm_id;
	int grp = PAD_GET_GROUP(pwm_dev[ch].io);
	int bit = PAD_GET_BITNO(pwm_dev[ch].io);

	NX_GPIO_SetOutputValue(grp, bit, CTRUE);
	return 0;
}

void pwm_disable(int pwm_id)
{
	int ch = pwm_id;
	int grp = PAD_GET_GROUP(pwm_dev[ch].io);
	int bit = PAD_GET_BITNO(pwm_dev[ch].io);

	NX_GPIO_SetOutputValue(grp, bit, CFALSE);
}

#if 0
static unsigned long    clk_in_max = 0;
#define DUTY_MIN_VAL  2

unsigned long pwm_set_frequency(int ch, unsigned int request, unsigned int duty)
{
  //struct pwm_device *pwm = &devs_pwm[ch];
  struct pwm_device *pwm = &pwm_dev[ch];
  volatile unsigned long rate, freq, clock = 0;
  volatile unsigned long hz = 0, pwmhz = 0;
  volatile unsigned int tcnt;
  int i, n, end = 0;


  printf("[%s] ch : %d\n", __func__, ch);
  printf("[%s] duty : %d\n", __func__, duty);
  printf("[%s] request : %d\n", __func__, request);
#if 0
  //RET_ASSERT_VAL(PWN_CHANNELS > ch, -EINVAL);
  //RET_ASSERT_VAL(ch >= 0, -EINVAL);
  //RET_ASSERT_VAL(100 >= duty && duty >= 0, -EINVAL);

  if( PWN_CHANNELS > ch )
  {
    printf("PWN_CHANNELS > ch error...!!\n");
    return -EINVAL;
  }
#endif
printf("[%s] Line : %d\n", __func__, __LINE__);
	struct clk *clk = clk_get(NULL, CORECLK_NAME_PCLK);

printf("[%s] Line : %d\n", __func__, __LINE__);
  clk_in_max  = clk_get_rate(clk);
  clk_in_max /= 2;

printf("[%s] Line : %d\n", __func__, __LINE__);
  if (request > (clk_in_max/DUTY_MIN_VAL)) {
    printf("Invalid request=%u hz over max pwm out %ld hz\n",
      request, (clk_in_max/DUTY_MIN_VAL));
    return -EINVAL;
  }
  if (0 == duty) {
//    pwm->compare = 0;
//    pwm->request = request;
    //pwm_set_device(pwm);
 //   return request;
  }

  /* for suspend -> resume */
#if 0
    if (pwm->request == request) {
        clock = pwm->rate;
        pwmhz = pwm->pwm_hz;
    }
#endif

  /* change only dutycycle */
#if 0
  if (pwm->request == request && pwm->duty != duty) {
    clock = pwm->rate;
    pwmhz = pwm->pwm_hz;
  /* set new pwm out */
  } else {
#endif
printf("[%s] Line : %d\n", __func__, __LINE__);
    for (n = 1; !end; n *= 10) {
printf("[%s] Line : %d\n", __func__, __LINE__);
      for (i = (n == 1 ? DUTY_MIN_VAL : 1); 10 > i; i++) {
printf("[%s] Line : %d\n", __func__, __LINE__);
        freq = request * i * n;
printf("[%s] Line : %d\n", __func__, __LINE__);
        if (freq > clk_in_max) {
printf("[%s] Line : %d\n", __func__, __LINE__);
          end = 1;
          break;
				}
printf("[%s] Line : %d\n", __func__, __LINE__);
        rate = clk_round_rate(pwm->clk, freq);

printf("[%s] Line : %d\n", __func__, __LINE__);
        tcnt = rate/request;
        hz = rate/tcnt;
        if (0 == rate%request) {
printf("[%s] Line : %d\n", __func__, __LINE__);
          clock = rate, pwmhz = hz, end = 1;
          break;
        }

        if (hz && (abs(hz-request) >= abs(pwmhz-request)))
          continue;
        clock = rate, pwmhz = hz;
      }
printf("[%s] Line : %d\n", __func__, __LINE__);
    }
#if 0
  }
#endif

	//printf("[%s] clk : %d\n", __func__, pwm->clk);
	printf("[%s] request : %d\n", __func__, request);
	printf("[%s] rate : %ld\n", __func__, clock);
	printf("[%s] duty : %d\n", __func__, duty);
	printf("[%s] pwn_hz  : %ld\n", __func__, pwmhz);
	printf("[%s] counter  : %ld\n", __func__, clock/request);
	printf("[%s] compare  : %ld\n", __func__, PWM_COMPARE((clock/request), duty) ? : 1);

#if 0
  if (clock) {
    pwm->request = request;
    pwm->rate = clock;
    pwm->duty = duty;
    pwm->pwm_hz = pwmhz;
    pwm->counter = clock/request;
    pwm->compare = PWM_COMPARE(pwm->counter, duty) ? : 1;
   // pwm_set_device(pwm);
  } else {
        printk("%s: can't find clock!!!\n", __func__);
    }
#endif

  return clock ? pwmhz : 0;
}
#endif







