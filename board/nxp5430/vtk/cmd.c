/*
 * (C) Copyright 2009 Nexell Co.,
 * jung hyun kim<jhkim@nexell.co.kr>
 *
 * Configuation settings for the Nexell board.
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

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/arch/display.h>

DECLARE_GLOBAL_DATA_PTR;

#define	INIT_VIDEO_SYNC(name)								\
	struct disp_vsync_info name = {							\
		.h_active_len	= CFG_DISP_PRI_RESOL_WIDTH,         \
		.h_sync_width	= CFG_DISP_PRI_HSYNC_SYNC_WIDTH,    \
		.h_back_porch	= CFG_DISP_PRI_HSYNC_BACK_PORCH,    \
		.h_front_porch	= CFG_DISP_PRI_HSYNC_FRONT_PORCH,   \
		.h_sync_invert	= CFG_DISP_PRI_HSYNC_ACTIVE_HIGH,   \
		.v_active_len	= CFG_DISP_PRI_RESOL_HEIGHT,        \
		.v_sync_width	= CFG_DISP_PRI_VSYNC_SYNC_WIDTH,    \
		.v_back_porch	= CFG_DISP_PRI_VSYNC_BACK_PORCH,    \
		.v_front_porch	= CFG_DISP_PRI_VSYNC_FRONT_PORCH,   \
		.v_sync_invert	= CFG_DISP_PRI_VSYNC_ACTIVE_HIGH,   \
		.pixel_clock_hz	= CFG_DISP_PRI_PIXEL_CLOCK,   		\
		.clk_src_lv0	= CFG_DISP_PRI_CLKGEN0_SOURCE,      \
		.clk_div_lv0	= CFG_DISP_PRI_CLKGEN0_DIV,         \
		.clk_src_lv1	= CFG_DISP_PRI_CLKGEN1_SOURCE,      \
		.clk_div_lv1	= CFG_DISP_PRI_CLKGEN1_DIV,         \
	};

#define	INIT_PARAM_SYNCGEN(name)						\
	struct disp_syncgen_param name = {						\
		.interlace 		= CFG_DISP_PRI_MLC_INTERLACE,       \
		.out_format		= CFG_DISP_PRI_OUT_FORMAT,          \
		.lcd_mpu_type 	= 0,                                \
		.invert_field 	= CFG_DISP_PRI_OUT_INVERT_FIELD,    \
		.swap_RB		= CFG_DISP_PRI_OUT_SWAPRB,          \
		.yc_order		= CFG_DISP_PRI_OUT_YCORDER,         \
		.delay_mask		= 0,                                \
		.vclk_select	= CFG_DISP_PRI_PADCLKSEL,           \
		.clk_delay_lv0	= CFG_DISP_PRI_CLKGEN0_DELAY,       \
		.clk_inv_lv0	= CFG_DISP_PRI_CLKGEN0_INVERT,      \
		.clk_delay_lv1	= CFG_DISP_PRI_CLKGEN1_DELAY,       \
		.clk_inv_lv1	= CFG_DISP_PRI_CLKGEN1_INVERT,      \
		.clk_sel_div1	= CFG_DISP_PRI_CLKSEL1_SELECT,		\
	};

#define	INIT_PARAM_MULTILY(name)					\
	struct disp_multily_param name = {						\
		.x_resol		= CFG_DISP_PRI_RESOL_WIDTH,			\
		.y_resol		= CFG_DISP_PRI_RESOL_HEIGHT,		\
		.pixel_byte		= CFG_DISP_PRI_SCREEN_PIXEL_BYTE,	\
		.fb_layer		= CFG_DISP_PRI_SCREEN_LAYER,		\
		.video_prior	= CFG_DISP_PRI_VIDEO_PRIORITY,		\
		.rgb_format		= CFG_DISP_PRI_SCREEN_RGB_FORMAT,	\
		.bg_color		= CFG_DISP_PRI_BACK_GROUND_COLOR,	\
		.interlace		= CFG_DISP_PRI_MLC_INTERLACE,		\
	};

#if defined(CONFIG_DISPLAY_OUT_LVDS)

#define	INIT_PARAM_LVDS(name)							\
	struct disp_lvds_param name = {							\
		.lcd_format 	= CFG_DISP_LVDS_LCD_FORMAT,         \
	};

extern void display_lvds(int module, unsigned int fbbase,
					struct disp_vsync_info *pvsync, struct disp_syncgen_param *psgen,
					struct disp_multily_param *pmly, struct disp_lvds_param *plvds);

int do_lvds (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int moudle = 0;
	int xresol = CFG_DISP_PRI_RESOL_WIDTH;
	int yresol = CFG_DISP_PRI_RESOL_HEIGHT;
	int pixelb = CFG_DISP_PRI_SCREEN_PIXEL_BYTE;
	U32 fbbase = CONFIG_FB_ADDR;

	INIT_VIDEO_SYNC(vsync);
	INIT_PARAM_SYNCGEN(syncgen);
	INIT_PARAM_MULTILY(multily);
	INIT_PARAM_LVDS(lvds);

	if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	moudle = simple_strtol(argv[1], NULL, 10);

	printf("DISP: W=%4d, H=%4d, Bpp=%d\n", xresol, yresol, pixelb*8);

	display_lvds(moudle, fbbase, &vsync, &syncgen, &multily, &lvds);
	return 0;
}

U_BOOT_CMD(
	lvds, 3, 1,	do_lvds,
	"lvds draw logo to display module",
	"num\n"
	"    - lvds display out module\n"
);
#endif

#if defined(CONFIG_DISPLAY_OUT_HDMI)
extern void display_hdmi(int module, int preset, unsigned int fbbase,
					struct disp_vsync_info *pvsync, struct disp_syncgen_param *psgen,
					struct disp_multily_param *pmly);

int do_hdmi (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int moudle = 0, preset = 0;
	int xresol = CFG_DISP_PRI_RESOL_WIDTH;
	int yresol = CFG_DISP_PRI_RESOL_HEIGHT;
	int pixelb = CFG_DISP_PRI_SCREEN_PIXEL_BYTE;
	U32 fbbase = CONFIG_FB_ADDR;

	INIT_VIDEO_SYNC(vsync);
	INIT_PARAM_SYNCGEN(syncgen);
	INIT_PARAM_MULTILY(multily);

	if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}
	moudle = simple_strtol(argv[1], NULL, 10);

	if (argc == 3)
		preset = simple_strtol(argv[2], NULL, 10);

	switch(preset) {
	case 0:	xresol = 1280, yresol =  720;	break;
	case 1:	xresol = 1920, yresol = 1080;	break;
	default:
		printf("hdmi not support preset %d\n", preset);
		return 0;
	}

	printf("DISP: W=%4d, H=%4d, Bpp=%d\n", xresol, yresol, pixelb*8);
	display_hdmi(moudle, preset, fbbase, &vsync, &syncgen, &multily);
	return 0;
}

U_BOOT_CMD(
	hdmi, 3, 1,	do_hdmi,
	"hdmi draw logo to display module",
	"num \n"
	"    - hdmi display out module num)\n"
//	"num preset\n"
//	"    - hdmi display out module num and preset (0: 720p, 1:1080p)\n"
);
#endif