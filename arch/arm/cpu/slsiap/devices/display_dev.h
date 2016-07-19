/*
 * (C) Copyright 2009
 * jung hyun kim, Nexell Co, <jhkim@nexell.co.kr>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <config.h>
#include <common.h>
#include <errno.h>
#include <platform.h>
#include <asm/arch/display.h>

#if (0)
#define DBGOUT(msg...)		{ printf("BD: " msg); }
#else
#define DBGOUT(msg...)		do {} while (0)
#endif

#ifndef printk
#define	printk		printf
#define	KERN_ERR
#define	KERN_INFO
#endif

/*
 * 	D V	P R E S E T	V A L U E S
 */
#define V4L2_DV_INVALID				0
#define V4L2_DV_480P59_94			1 /* BT.1362 */
#define V4L2_DV_576P50				2 /* BT.1362 */
#define V4L2_DV_720P24				3 /* SMPTE 296M */
#define V4L2_DV_720P25				4 /* SMPTE 296M */
#define V4L2_DV_720P30				5 /* SMPTE 296M */
#define V4L2_DV_720P50				6 /* SMPTE 296M */
#define V4L2_DV_720P59_94			7 /* SMPTE 274M */
#define V4L2_DV_720P60				8 /* SMPTE 274M/296M */
#define V4L2_DV_1080I29_97			9 /* BT.1120/ SMPTE 274M */
#define V4L2_DV_1080I30				10 /* BT.1120/ SMPTE 274M */
#define V4L2_DV_1080I25				11 /* BT.1120 */
#define V4L2_DV_1080I50				12 /* SMPTE 296M */
#define V4L2_DV_1080I60				13 /* SMPTE 296M */
#define V4L2_DV_1080P24				14 /* SMPTE 296M */
#define V4L2_DV_1080P25				15 /* SMPTE 296M */
#define V4L2_DV_1080P30				16 /* SMPTE 296M */
#define V4L2_DV_1080P50				17 /* BT.1120 */
#define V4L2_DV_1080P60				18 /* BT.1120 */

#define V4L2_DV_480P60				19
#define V4L2_DV_1080I59_94			20
#define V4L2_DV_1080P59_94			21

#define V4L2_DV_720P60_FP			22
#define V4L2_DV_720P60_SB_HALF		23
#define V4L2_DV_720P60_TB			24
#define V4L2_DV_720P59_94_FP		25
#define V4L2_DV_720P59_94_SB_HALF	26
#define V4L2_DV_720P59_94_TB		27
#define V4L2_DV_720P50_FP			28
#define V4L2_DV_720P50_SB_HALF		29
#define V4L2_DV_720P50_TB			30
#define V4L2_DV_1080P24_FP			31
#define V4L2_DV_1080P24_SB_HALF		32
#define V4L2_DV_1080P24_TB			33
#define V4L2_DV_1080P23_98_FP		34
#define V4L2_DV_1080P23_98_SB_HALF	35
#define V4L2_DV_1080P23_98_TB		36
#define V4L2_DV_1080I60_SB_HALF		37
#define V4L2_DV_1080I59_94_SB_HALF	38
#define V4L2_DV_1080I50_SB_HALF		39
#define V4L2_DV_1080P60_SB_HALF		40
#define V4L2_DV_1080P60_TB			41
#define V4L2_DV_1080P30_FP			42
#define V4L2_DV_1080P30_SB_HALF		43
#define V4L2_DV_1080P30_TB			44

#define V4L2_DV_800_480_60			45

#define	MLC_LAYER_RGB_0			0	/* number of RGB layer 0 */
#define	MLC_LAYER_RGB_1			1	/* number of RGB layer 1 */
#define	MLC_LAYER_VIDEO			3	/* number of Video layer: 3 = VIDEO */

extern void disp_lcd_device(int io);
extern void disp_initialize(void);
extern void disp_topctl_reset(void);
extern void disp_syncgen_reset(void);
extern void disp_syncgen_init(int module);
extern void disp_syncgen_enable(int module, int enable);
extern int  disp_syncgen_setup(int module, struct disp_vsync_info *psync, struct disp_syncgen_param *par);
extern void disp_multily_init(int module);
extern void disp_multily_enable(int module, int layer, int enable);
extern int disp_multily_setup(int module, struct disp_multily_param *par, unsigned int fbbase);
