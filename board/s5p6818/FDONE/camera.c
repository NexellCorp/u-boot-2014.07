#include <common.h>
#include <platform.h>
#include <asm/gpio.h>
#include <gpio.h>
#include <camera.h>
#include <vip.h>
#include <mlc.h>

static struct reg_val _sensor_init_data[] =
{
#if 1
    {0x02, 0x44},
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
#else
    {0x02, 0x44},
	{0x03, 0xA2},
	{0x05, 0x77},
	{0x06, 0x80},
	{0x0A, 0x0C},
	{0x10, 0x1E}, // Brightness
	{0x19, 0x58},
	{0x1A, 0x0F},
	{0x1C, 0x00},
	{0x2D, 0x07},
	{0x2F, 0xE4},
	{0x6B, 0x09},
	{0x6C, 0x19},
	{0x6D, 0x0A},
#endif
    END_MARKER
};

#define CAMERA_POWER_EN     (PAD_GPIO_A + 18)
#define CAMERA_RESET        (PAD_GPIO_D + 23)
static int _sensor_power_enable(bool enable)
{
    u32 io = CAMERA_POWER_EN;
    u32 reset_io = CAMERA_RESET;

    if (enable) {
        // enable power
        /*gpio_direction_output(io, 1);*/
        /*gpio_set_alt(io, 1);*/

        // reset to high
        gpio_direction_output(reset_io, 1);
        gpio_set_alt(reset_io, 0);
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
        /* VCLK, HSYNC, VSYNC */
        { PAD_GPIO_E +  4, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_E +  5, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_E +  6, NX_GPIO_PADFUNC_1 },
        /* DATA */
        { PAD_GPIO_D + 28, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_D + 29, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_D + 30, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_D + 31, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_E +  0, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_E +  1, NX_GPIO_PADFUNC_1 },
        { PAD_GPIO_E +  2, NX_GPIO_PADFUNC_1 }, { PAD_GPIO_E +  3, NX_GPIO_PADFUNC_1 },
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
     .bus       = 1,
     /*.chip      = 0x88 >> 1,*/
     .chip      = 0x8a >> 1,
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
#if 1 // apply screen width, height
    .right          = 0,
    .bottom         = 0,
#else
    .right          = CAM_WIDTH,
    .bottom         = CAM_HEIGHT,
#endif
    .priority       = 0,
};

#define VIP_MODULE_NUM  0
void camera_run(void)
{
    int camera_id = camera_register_sensor(&_sensor_data);
    camera_sensor_run(camera_id);

    nxp_vip_register_param(VIP_MODULE_NUM, &_vip_param);
    nxp_vip_set_addr(VIP_MODULE_NUM, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR);

    nxp_mlc_video_set_param(0, &_mlc_param);
    nxp_mlc_video_set_addr(0, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR,
            ALIGN(CAM_WIDTH, 64),
            ALIGN(CAM_WIDTH/2, 64),
            ALIGN(CAM_WIDTH/2, 64));
    /*printf("%s exit\n", __func__);*/
}

#define CAMERA_ACTIVE_DETECT (PAD_GPIO_A + 3)
#define CAMERA_DETECT_ACTIVE_VALUE 0
void camera_preview(void)
{
    int io = CAMERA_ACTIVE_DETECT;
    int val;

    gpio_direction_input(io);
    gpio_set_alt(io, 0);
    val = gpio_get_value(io);

    if (val == CAMERA_DETECT_ACTIVE_VALUE) {
        printf("run vip & mlc\n");
        nxp_vip_run(VIP_MODULE_NUM);
        nxp_mlc_video_run(0);
    }
    /*printf("%s exit\n", __func__);*/
}
