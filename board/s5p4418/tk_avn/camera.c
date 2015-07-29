#include <common.h>
#include <platform.h>
#include <gpio.h>
#include <camera.h>
#include <vip.h>
#include <mlc.h>

#if 0
static struct reg_val _sensor_init_data[] =
{
    {0x03, 0xa6},
    {0x07, 0x02},
    {0x08, 0x12},
    {0x09, 0xf0},
    {0x0a, 0x1c},
    /*{0x0b, 0xd0}, // 720 */
    {0x0b, 0xc0}, // 704
    {0x1b, 0x00},
    {0x10, 0x10},
    {0x11, 0x42},
    {0x2f, 0xe6},
    {0x55, 0x00},
    END_MARKER
};
#endif

static struct reg_val _sensor_init_data[] =
{
    {0x02, 0x40},
    {0x03, 0xa2},
    {0x07, 0x02},
    {0x08, 0x12},
    {0x09, 0xf0},
    {0x0a, 0x1c},
    /*{0x0b, 0xd0}, // 720 */
    {0x0b, 0xc0}, // 704
    {0x1b, 0x00},
    /*{0x10, 0xfa},*/
    {0x10, 0x1e},
    {0x11, 0x64},
    {0x2f, 0xe6},
    {0x55, 0x00},
#if 1
    /*{0xb1, 0x20},*/
    /*{0xb1, 0x02},*/
    {0xaf, 0x00},
    {0xb1, 0x20},
    {0xb4, 0x20},
    /*{0x06, 0x80},*/
#endif
    /*{0xaf, 0x40},*/
    /*{0xaf, 0x00},*/
    /*{0xaf, 0x80},*/
    END_MARKER
};


#define CAMERA_POWER_MUX		(PAD_GPIO_C + 24)
#define CAMERA_RESET        (PAD_GPIO_C + 15)
#define CAMERA_POWER_DOWN   (PAD_GPIO_C + 16)

#define BACK_GEAR					  (PAD_GPIO_E + 10)

static int _sensor_power_enable(bool enable)
{
    //u32 back_gear_io = BACK_GEAR;
    u32 mux_io = CAMERA_POWER_MUX;
    u32 io = CAMERA_POWER_DOWN;
    u32 reset_io = CAMERA_RESET;

    if (enable) {
				// back gear
				//gpio_direction_input(back_gear_io);
        //gpio_set_alt(back_gear_io, 0);

				// camera mux enable
				gpio_direction_output(mux_io, 0);
        gpio_set_alt(mux_io, 1);
        mdelay(1);

        gpio_set_value(mux_io, 1);
        mdelay(10);

        // disable power down
        gpio_direction_output(io, 0);
        gpio_set_alt(io, 1);

        // reset to high
        gpio_direction_output(reset_io, 1);
        gpio_set_alt(reset_io, 1);
        mdelay(1);

        // reset to low
        gpio_set_value(reset_io, 0);
        mdelay(10);

        // reset to high
        gpio_set_value(reset_io, 1);
        mdelay(10);
    }

		return 0;
}

static void _sensor_setup_io(void)
{
    u_int *pad;
    int i, len;
    u_int io, fn;

    /* VIP0:0 = VCLK, VID0 ~ 7 */
    const u_int port[][2] = {
#if 0
        /* VCLK, HSYNC, VSYNC */
        { PAD_GPIO_E +  4, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_E +  5, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_E +  6, NX_GPIO_PADFUNC_1 },
        /* DATA */
        { PAD_GPIO_D + 28, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_D + 29, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_D + 30, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_D + 31, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_E +  0, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_E +  1, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_E +  2, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_E +  3, NX_GPIO_PADFUNC_1 },
#endif
				/* VCLK, HSYNC, VSYNC */
				{ PAD_GPIO_A + 28, NX_GPIO_PADFUNC_1 },
				{ PAD_GPIO_E + 13, NX_GPIO_PADFUNC_2 },
				{ PAD_GPIO_E +  7, NX_GPIO_PADFUNC_2 },

				{ PAD_GPIO_A + 30, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_B +  0, NX_GPIO_PADFUNC_1 },
				{ PAD_GPIO_B +  2, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_B +  4, NX_GPIO_PADFUNC_1 },
				{ PAD_GPIO_B +  6, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_B +  8, NX_GPIO_PADFUNC_1 },
				{ PAD_GPIO_B +  9, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_B + 10, NX_GPIO_PADFUNC_1 },
    };

    pad = (u_int *)port;
    len = sizeof(port)/sizeof(port[0]);

    for (i = 0; i < len; i++) {
        io = *pad++;
        fn = *pad++;
        gpio_direction_input(io);
        gpio_set_alt(io, fn);
    }
}

static struct camera_sensor_data _sensor_data = {
     .bus       = 0,
     .chip      = 0x88 >> 1,
	 /*.chip      = 0x8a >> 1,*/
     .reg_val   = _sensor_init_data,
     .power_enable = _sensor_power_enable,
     .set_clock = NULL,
     .setup_io  = _sensor_setup_io,
};

#define CAM_WIDTH       704
#define CAM_HEIGHT      480

static struct nxp_vip_param _vip_param = {
    // interlace
    .port           = 0,
    .external_sync  = false,
    .is_mipi        = false,
    .h_active       = CAM_WIDTH,
    .h_frontporch   = 7,
    .h_syncwidth    = 1,
    .h_backporch    = 10,
    .v_active       = CAM_HEIGHT,
    .v_frontporch   = 0,
    .v_syncwidth    = 2,
    .v_backporch    = 3,
    .data_order     = 0,
    .interlace      = true,
};

static struct nxp_mlc_video_param _mlc_param = {
    .format         = NX_MLC_YUVFMT_420,
    .width          = CAM_WIDTH,
    .height         = CAM_HEIGHT,
    .left           = 0,
    .top            = 0,
    .right          = 0,
    .bottom         = 0,
#if defined(CONFIG_MLC_RGB_OVERLAY)
    .priority       = 0,
#else
    .priority       = 0,
#endif
};

//#define VIP_MODULE_NUM  0
#define VIP_MODULE_NUM  1
int module_id = VIP_MODULE_NUM;

void camera_run(void)
{
    int camera_id = camera_register_sensor(&_sensor_data);
    camera_sensor_run(camera_id);

    nxp_vip_register_param(module_id, &_vip_param);
    nxp_vip_set_addr(module_id, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR);

    nxp_mlc_video_set_param(0, &_mlc_param);
    /*nxp_mlc_video_set_addr(0, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR, 704, 384, 384);*/
    nxp_mlc_video_set_addr(0, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR,
            ALIGN(CAM_WIDTH, 64),
            ALIGN(CAM_WIDTH/2, 64),
            ALIGN(CAM_WIDTH/2, 64));
    //printf("%s exit\n", __func__);
}

void camera_preview(void)
{
		int back_gear_io = BACK_GEAR;

    //int back_gear_io = (PAD_GPIO_E + 11);
    /*int io = (PAD_GPIO_E + 12); // GMAC_TXER*/

  	gpio_direction_input(back_gear_io);
    gpio_set_alt(back_gear_io, 0);

		//run vip
    nxp_vip_run(module_id);

    if (gpio_get_value(back_gear_io)) {
        //printf("run vip & mlc\n");
     		//nxp_vip_run(module_id);
#if 1 // keun
        nxp_mlc_video_run(0);
#endif
    }
    //printf("%s exit\n", __func__);
}
