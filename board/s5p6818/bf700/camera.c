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
    // interrupt : VDLOSSCH enable
    /*{0xb1, 0x20},*/
    {0xb1, 0xa0},
    // interrupt enable
    {0xae, 0x80},
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


#ifdef CONFIG_TW9992
#include <nx_mipi.h>
#include <mmc.h>
#include <fat.h>
#include <fs.h>
#include <i2c.h>


#define TW9992_WIDTH       720
#define TW9992_HEIGHT      480

#define DEBUG_POINT printf( "[DEBUG tw9992] %s(%d):%s\n", __FILE__, __LINE__, __func__)
static struct reg_val tw9992_sensor_init_data[256] =
{
	0x00, 0x92,
	0x01, 0x00,
	0x02, 0x46,
	0x03, 0x78,
	0x04, 0x00,
	0x05, 0x09,
	0x06, 0x00,
	0x07, 0x02,
	0x08, 0x12,
	0x09, 0xF0,
	0x0A, 0x09,
	0x0B, 0xD1,
	0x0C, 0xCC,
	0x0D, 0x00,
	0x10, 0x00,
	0x11, 0x64,
	0x12, 0x11,
	0x13, 0x80,
	0x14, 0x80,
	0x15, 0x00,
	0x17, 0x80,
	0x18, 0x44,
	0x19, 0x06,
	0x1A, 0x10,
	0x1B, 0x00,
	0x1C, 0x0F,		//0x08,
	0x1D, 0x7F,
	0x1F, 0x00,
	0x20, 0x50,
	0x21, 0x22,
	0x22, 0xF0,
	0x23, 0xD8,
	0x24, 0xBC,
	0x25, 0xB8,
	0x26, 0x44,
	0x27, 0x38,
	0x28, 0x00,
	0x29, 0x00,
	0x2A, 0x78,
	0x2B, 0x44,
	0x2C, 0x30,
	0x2D, 0x14,
	0x2E, 0xA5,
	0x2F, 0xE0,
	0x30, 0x00,
	0x31, 0x10,
	0x32, 0xFF,
	0x33, 0x05,
	0x34, 0x1A,
	0x35, 0x00,
	0x36, 0x7A,			//0x5A, requested by Daniel Kim, 09242014
	0x37, 0x18,
	0x38, 0xDD,
	0x39, 0x00,
	0x3A, 0x30,
	0x3B, 0x00,
	0x3C, 0x00,
	0x3D, 0x00,
	0x3F, 0x1A,
	0x40, 0x80,
	0x41, 0x00,
	0x42, 0x00,
	0x48, 0x02,
	0x49, 0x00,
	0x4A, 0x81,
	0x4B, 0x0A,
	0x4C, 0x00,
	0x4D, 0x01,
	0x4E, 0x01,
	0x50, 0x00,
	0x51, 0x00,
	0x52, 0x00,
	0x53, 0x00,
	0x54, 0x06,
	0x55, 0x00,
	0x56, 0x00,
	0x57, 0x00,
	0x58, 0x00,
	0x60, 0x00,
	0x61, 0x00,
	0x62, 0x00,
	0x63, 0x00,
	0xC0, 0x06,
	0xC1, 0x20,

	0x71, 0xA5,
	0xA2, 0x30,
	0x70, 0x01, //start with MIPI Normal operation

	0x07, 0x02, //using Shadow Register so no Input cropping setting for NTSC
	0x08, 0x15,
	0x09, 0xf0,
	0x0a, 0x14,
	0x0b, 0xd0,

	END_MARKER
};

static int tw9992_sensor_power_enable(bool enable)
{
    unsigned int reset_io = ((PAD_GPIO_ALV + 0));

    if (enable) {
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


static struct camera_sensor_data tw9992_sensor_data = {
     .bus       = 4,
     .chip      = 0x7a >> 1,
     .reg_val   = tw9992_sensor_init_data,
     .power_enable = tw9992_sensor_power_enable,
     .set_clock = NULL,
     .setup_io  = NULL,
};

static struct nxp_vip_param tw9992_vip_param = {
    // interlace
    .port           = 1,
    .external_sync  = true,
    .is_mipi        = true,
    .h_active       = TW9992_WIDTH,
    .h_frontporch   = 1,
    .h_syncwidth    = 1,
    .h_backporch    = 1,
    .v_active       = TW9992_HEIGHT,
    .v_frontporch   = 1,
    .v_syncwidth    = 1,
    .v_backporch    = 1,
    .data_order     = 1,
    .interlace      = true,
};

static struct nxp_mlc_video_param tw9992_mlc_param = {
    .format         = NX_MLC_YUVFMT_420,
    .width          = CAM_WIDTH,
    .height         = CAM_HEIGHT/2,
    .left           = 0,
    .top            = 0,
#if 1 // apply screen width, height
    .right          = 0,
    .bottom         = 0,
#else
    .right          = TW9992_WIDTH  //CAM_WIDTH,
    .bottom         = TW9992_HEIGHT //CAM_HEIGHT,
#endif
    .priority       = 0,
};


static int tw9992_init_data_parse_string(const char *s, const char *e, char *b, int len)
{
	int l, a = 0;

	do { while (0x20 == *s || 0x09 == *s || 0x0a == *s) { s++; } } while(0);

	if (0x20 == *(e-1) || 0x09 == *(e-1))
		do { e--; while (0x20 == *e || 0x09 == *e) { e--; }; a = 1; } while(0);

	l = (e - s + a);
	if (l > len) {
		printf("-- Not enough buffer %d for string len %d [%s] --\n", len, l, s);
		return -1;
	}

	strncpy(b, s, l);
	b[l] = 0;

	return l;
}

static int tw9992_init_data_get_string(const char *ptable_str, int len, struct reg_val *reg_val)
{
	int cnt = 0;
	int i = 0,j = 0;
	int y = 0,z = 0;
	const char *id, *s;
	char str[32];

	uint8_t reg;

	s = id = ptable_str;

	for(i=0; i<=len; i++)
	{
		str[j] = id[i];
		if(id[i] == 0x2c)
		{
			str[j] = '\0';
			reg = simple_strtoul(str, NULL, 16);

			if(z == 0)
			{
				reg_val->reg = reg;
				z++;
			}
			else
			{
				reg_val->val = reg;
				reg_val++;
				cnt++;
				z = 0;
			}
			j=0;
		}
		else
		{
			j++;
		}
	}

	return cnt;
}


static int tw9992_init_data_sort_string(char *p, int len)
{
	int skip = 0;
	int i, j;
	for (i = 0, j = 0; len > i; i++) {

		if(0x2F == p[i] && skip == 0)
			skip = 1;

		if(0x0A == p[i])
			skip = 0;

		if(skip)
		{
			continue;
		}
		
		if (0x20 != p[i] && 0x09 != p[i] && 0x0D != p[i] && 0x0A != p[i])
		{
			p[j++] = p[i];
		}
	}
	p[j] = '\0';

	return j;
}

int tw9992_init_data_load(unsigned long addr)
{
	int fstype = FS_TYPE_FAT;
	const char *filename = "tw9992_init.txt";
	unsigned long bytes = 0;
	unsigned long pos = 0;
	unsigned long time;
	int len_read;

	if (fs_set_blk_dev("mmc", "0:1", fstype))
		return -1;

	time = get_timer(0);

	len_read = fs_read(filename, addr, pos, bytes);
	time = get_timer(time);
	if (len_read <= 0)
		return -1;

	printf("%d bytes read in %lu ms", len_read, time);
	if (time > 0) {
		puts(" (");
		print_size(len_read / time * 1000, "/s");
		puts(")");
	}
	puts("\n");


	return len_read;
}


void initialize_tw9992( void )
{
	int ret = 0;
	int camera_id = 0;
	unsigned long ram_addr = 0x49000000;
	int read_size, sort_size;
	char *p;

	u8 read_val = 0;

	DEBUG_POINT;

	read_size = tw9992_init_data_load(ram_addr);

	if(0 <= read_size)
	{
		uint8_t reg;

		p = (char*)ram_addr;
		p[read_size+1] = '\0';

		sort_size= tw9992_init_data_sort_string(p, read_size);

		printf("read_size:%d, sort_size:%d\n", read_size, sort_size);
		ret = tw9992_init_data_get_string(p, sort_size, tw9992_sensor_init_data);
		tw9992_sensor_init_data[ret].reg = 0xff;
		tw9992_sensor_init_data[ret].val = 0xff;
	}

	camera_id = camera_register_sensor(&tw9992_sensor_data);
	camera_sensor_run(camera_id);

	//mdelay(500);

	printf("==============================================\n");

	i2c_read(tw9992_sensor_data.chip, 0x00, 1, &read_val, 1);
	printf("===== Product ID code : 0x%x \n", read_val);

	i2c_read(tw9992_sensor_data.chip, 0x03, 1, &read_val, 1);
	printf("===== Reg 0x03 : 0x%x.\n", read_val);

	if(read_val & 0x80)	printf("===== Video not present.\n");
	else				printf("===== Video Detected.\n");

	if(read_val & 0x40)	printf("===== Horizontal sync PLL is locked to the incoming video source.\n");
	else				printf("===== Horizontal sync PLL is not locked.\n");

	if(read_val & 0x20)	printf("===== Sub-carrier PLL is locked to the incoming video source.\n");
	else				printf("===== Sub-carrier PLL is not locked.\n");

	if(read_val & 0x10)	printf("===== Odd field is being decoded.\n");
	else				printf("===== Even field is being decoded.\n");

	if(read_val & 0x8)	printf("===== Vertical logic is locked to the incoming video source.\n");
	else				printf("===== Vertical logic is not locked.\n");

	if(read_val & 0x2)	printf("===== No color burst signal detected.\n");
	else				printf("===== Color burst signal detected.\n");

	if(read_val & 0x1)	printf("===== 50Hz source detected.\n");
	else				printf("===== 60Hz source detected.\n");

	i2c_read(tw9992_sensor_data.chip, 0x1c, 1, &read_val, 1);
		printf("===== Reg 0x1C : 0x%x.\n", read_val);

	if(read_val & 0x80)	printf("===== Detection in progress.\n");
	else				printf("===== Idle.\n");

	//i2c_read(tw9992_sensor_data.chip, 0x7F, 1, &read_val, 1);
	//printf("===== Reg 0x7F : 0x%x.\n", read_val);

	printf("==============================================\n");

}

void nxp_vip_setting(int module, struct nxp_vip_param *param)
{
#if 1
	nxp_vip_register_param(module, param);
    nxp_vip_set_addr(module, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR);
#else
    const U32 VIP_ModuleIndex = module;
    const U32 VIP_WIDTH = param->h_active;
    const U32 VIP_HEIGHT = param->v_active;
    const U32 VIP_HFP = param->h_frontporch;
    const U32 VIP_HSW = param->h_syncwidth; 
    const U32 VIP_HBP = param->h_backporch;
    const U32 VIP_VFP = param->v_frontporch;  
    const U32 VIP_VSW = param->v_syncwidth;  
    const U32 VIP_VBP = param->v_backporch;  
    const U32 VIP_ClockIndex  = NX_VIP_GetClockNumber(module);
    const U32 VIP_ResetIndex  = NX_VIP_GetResetNumber(module);

    DEBUG_POINT;

    NX_CLKGEN_SetBaseAddress       (VIP_ClockIndex, (U32)IO_ADDRESS(NX_CLKGEN_GetPhysicalAddress(VIP_ClockIndex)));
    NX_CLKGEN_SetClockDivisorEnable(VIP_ClockIndex, CTRUE);
    NX_CLKGEN_SetClockBClkMode     (VIP_ClockIndex, NX_BCLKMODE_DYNAMIC);

    NX_VIP_Initialize();
	NX_VIP_SetBaseAddress( VIP_ModuleIndex, IO_ADDRESS(NX_VIP_GetPhysicalAddress(VIP_ModuleIndex)) );
	// NX_CLOCK_SetClockEnable( NX_VIP_GetClockNumber ( VIP_ModuleIndex ), CTRUE );
#if defined(CONFIG_MACH_S5P4418)
    NX_RSTCON_SetnRST(VIP_ResetIndex, RSTCON_nDISABLE);
    NX_RSTCON_SetnRST(VIP_ResetIndex, RSTCON_nENABLE );
#elif defined(CONFIG_MACH_S5P6818)
    NX_RSTCON_SetRST(VIP_ResetIndex , RSTCON_ASSERT  );
    NX_RSTCON_SetRST(VIP_ResetIndex , RSTCON_NEGATE  );
#endif
	NX_VIP_OpenModule( VIP_ModuleIndex );
	//NX_VIP_SetInterruptEnable(VIP_ModuleIndex, NX_VIP_INT_VSYNC, CFALSE);
	//NX_VIP_SetInterruptEnable(VIP_ModuleIndex, NX_VIP_INT_HSYNC, CFALSE);
	//NX_VIP_SetInterruptEnable(VIP_ModuleIndex, NX_VIP_INT_VSYNC , CTRUE);
	//NX_INT_RegisterHandler( NX_VIP_GetInterruptNumber(VIP_ModuleIndex), VIP_OnEvent, (void*)VIP_ModuleIndex );

	//NX_CONSOLE_PutString("[NXLOG] VIP Config\n");

	// (freq. of RX_BYTE_CLK_HS) X (number of data lane) X 8bits <= (freq. of Pixel clock) X (bitwidth of image format)
	//	125Mhz * 2bytes = 250Mbytes/sec <= 147Mhz * 2bytes = 294
	// NX_CLOCK_SetClockDivisor( NX_VIP_GetClockNumber(VIP_ModuleIndex),0,NX_CLK147P456MHZ,CNULL); // CSI output clock == VIP input clock
	// NX_CLOCK_SetClockDivisorEnable( NX_VIP_GetClockNumber(VIP_ModuleIndex),CTRUE);
    printf("%s: apply mipi csi clock!!!\n", __func__);
    NX_CLKGEN_SetClockSource (VIP_ClockIndex, 0, 0); /* external PCLK */
    NX_CLKGEN_SetClockDivisor(VIP_ClockIndex, 0, 8);
    NX_CLKGEN_SetClockDivisorEnable(VIP_ClockIndex, CTRUE);

	/********************************************************************
	* Clipper Config
	********************************************************************/
	NX_VIP_SetInputPort(VIP_ModuleIndex, NX_VIP_INPUTPORT_B); // for MIPI
	NX_VIP_SetDataMode(VIP_ModuleIndex, NX_VIP_DATAORDER_CBY0CRY1, 16); // Data Order: CRYCBY, 8Bit
	NX_VIP_SetHVSyncForMIPI(VIP_ModuleIndex, VIP_WIDTH*2, VIP_HEIGHT, VIP_HSW, VIP_HFP, VIP_HBP, VIP_VSW, VIP_VFP, VIP_VBP );
	NX_VIP_SetFieldMode(VIP_ModuleIndex, CFALSE, NX_VIP_FIELDSEL_BYPASS, CFALSE, CFALSE); // EXT Field Disable, EXT Field Bypass, Interlace TRUE, Inv Field FALSE
	//NX_VIP_ResetFIFO(VIP_ModuleIndex); // FIFO Reset Disable
	NX_VIP_SetFIFOResetMode(VIP_ModuleIndex, NX_VIP_FIFORESET_FRAMEEND); // FIFO Clr Frame End			
	NX_VIP_SetFIFOResetMode(VIP_ModuleIndex, NX_VIP_FIFORESET_CPU); // FIFO Clr by CPU
	//NX_VIP_ResetFIFO(VIP_ModuleIndex); // FIFO Reset Enable
	
	/********************************************************************
	* Clipper Config * Preview
	*********************************************************************/		
	NX_VIP_SetClipRegion(VIP_ModuleIndex, 0, 0, VIP_WIDTH-(VIP_WIDTH%32), VIP_HEIGHT-(VIP_HEIGHT%32));
	NX_VIP_SetClipperAddr(VIP_ModuleIndex, NX_VIP_FORMAT_420, VIP_WIDTH-(VIP_WIDTH%32), VIP_HEIGHT-(VIP_HEIGHT%32), 
													CONFIG_VIP_LU_ADDR,
													CONFIG_VIP_CB_ADDR,
													CONFIG_VIP_CR_ADDR,
													ALIGN((VIP_WIDTH-(VIP_WIDTH%32)), 64), ALIGN((VIP_WIDTH-(VIP_WIDTH%32))/2, 64) );

#if defined(CONFIG_MACH_S5P4418)
	NX_VIP_SetClipperFormat(VIP_ModuleIndex, NX_VIP_FORMAT_420, 0, 0, 0);
#elif defined(CONFIG_MACH_S5P6818)
	NX_VIP_SetClipperFormat(VIP_ModuleIndex, NX_VIP_FORMAT_420);
#endif

	//NX_VIP_SetVIPEnable(VIP_ModuleIndex, CTRUE, CTRUE, CTRUE, CFALSE);  // bVIPEnb, bSepEnb, bClipEnb, bDeciEnb  // modify @ junseo relocate this line
    {
        volatile NX_VIP_RegisterSet* pvip = (volatile NX_VIP_RegisterSet*)IO_ADDRESS(NX_VIP_GetPhysicalAddress(VIP_ModuleIndex));
        pvip->VIP_SYNCCTRL &= 0xFFFFFDFF;
    }		
#endif
}


void nxp_mipi_csi_setting(struct nxp_vip_param *param)
{
	volatile NX_MIPI_RegisterSet* pmipi;
	const U32 MIPI_ModuleIndex  = 0;
	int number_of_modules;
	int channel;

	const U32 WIDTH = param->h_active;
	const U32 HEIGHT = param->v_active;

	DEBUG_POINT;

	NX_MIPI_Initialize();

	number_of_modules = NX_MIPI_GetNumberOfModule();
	NX_ASSERT( number_of_modules==1 ); //for(MIPI_ModuleIndex=0; i< number_of_modules; MIPI_ModuleIndex++)		

	NX_ASSERT( NX_MIPI_GetNumberOfPADMode ( 0 ) == 1 );

	NX_TIEOFF_Set( TIEOFFINDEX_OF_MIPI0_NX_DPSRAM_1R1W_EMAA, 3 );
	NX_TIEOFF_Set( TIEOFFINDEX_OF_MIPI0_NX_DPSRAM_1R1W_EMAB, 3 );

	NX_MIPI_SetBaseAddress( MIPI_ModuleIndex, (U32)IO_ADDRESS(NX_MIPI_GetPhysicalAddress(MIPI_ModuleIndex)) );
	NX_CLKGEN_SetClockBClkMode( NX_MIPI_GetClockNumber ( MIPI_ModuleIndex ), NX_BCLKMODE_DYNAMIC );

#if defined(CONFIG_MACH_S5P4418)
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST       ), RSTCON_nDISABLE);
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_DSI_I ), RSTCON_nDISABLE);
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_CSI_I ), RSTCON_nDISABLE);
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_S ), RSTCON_nDISABLE);
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_M ), RSTCON_nDISABLE);            
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST       ), RSTCON_nENABLE );
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_DSI_I ), RSTCON_nENABLE );
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_CSI_I ), RSTCON_nENABLE );
#elif defined(CONFIG_MACH_S5P6818)
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST       ), RSTCON_ASSERT);
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_DSI_I ), RSTCON_ASSERT);
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_CSI_I ), RSTCON_ASSERT);
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_S ), RSTCON_ASSERT);
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_M ), RSTCON_ASSERT);            
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST       ), RSTCON_NEGATE );
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_DSI_I ), RSTCON_NEGATE );
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_CSI_I ), RSTCON_NEGATE );
#endif

	NX_MIPI_OpenModule( MIPI_ModuleIndex );
	pmipi = (volatile NX_MIPI_RegisterSet*)IO_ADDRESS(NX_MIPI_GetPhysicalAddress(MIPI_ModuleIndex));
	pmipi->CSIS_DPHYCTRL= (5 <<24);

	NX_MIPI_SetInterruptEnableAll( MIPI_ModuleIndex, 0 );
	NX_MIPI_ClearInterruptPendingAll( MIPI_ModuleIndex );
	//NX_MIPI_EnablePAD ( MIPI_ModuleIndex, MIPI_PADModeIndex );
	//NX_INT_RegisterHandler( NX_MIPI_GetInterruptNumber(MIPI_ModuleIndex), MIPI_Handler, ((void*)MIPI_ModuleIndex) );
	//-------------------------------------------------
	//	CSI
	//-------------------------------------------------
	//NX_CLOCK_SetClockDivisor( NX_MIPI_GetClockNumber(MIPI_ModuleIndex),0,NX_CLK147P456MHZ,CNULL); // CSI output clock == VIP input clock
	//NX_CLOCK_SetClockDivisorEnable( NX_MIPI_GetClockNumber(MIPI_ModuleIndex),CTRUE);
	NX_CLKGEN_SetClockSource (NX_MIPI_GetClockNumber(MIPI_ModuleIndex), 0, 0); /* external PCLK */
	NX_CLKGEN_SetClockDivisor(NX_MIPI_GetClockNumber(MIPI_ModuleIndex), 0, 8);
	NX_CLKGEN_SetClockDivisorEnable(NX_MIPI_GetClockNumber(MIPI_ModuleIndex), CTRUE);

	NX_MIPI_CSI_SetParallelDataAlignment32 ( MIPI_ModuleIndex, 1, CFALSE ); // It must be false for NX4330(VIP)
	NX_MIPI_CSI_SetYUV422Layout ( MIPI_ModuleIndex, 1, NX_MIPI_CSI_YUV422LAYOUT_FULL ); // It must be full for NX4330(VIP)
	NX_MIPI_CSI_SetFormat( MIPI_ModuleIndex, 1, NX_MIPI_CSI_FORMAT_YUV422_8 ); // It must be NX_MIPI_CSI_FORMAT_YUV422_8 for NX4330(VIP)
	NX_MIPI_CSI_EnableDecompress ( MIPI_ModuleIndex, CFALSE );// It must be false for NX4330(VIP)
	NX_MIPI_CSI_SetInterleaveMode( MIPI_ModuleIndex, NX_MIPI_CSI_INTERLEAVE_VC);// It must be NX_MIPI_CSI_INTERLEAVE_VC for NX4330(VIP)
	NX_MIPI_CSI_SetTimingControl( MIPI_ModuleIndex, 1, 32,16,368 ); // int T1, int T2, int T5
	NX_MIPI_CSI_SetInterleaveChannel( MIPI_ModuleIndex, 0, 1 );
	NX_MIPI_CSI_SetInterleaveChannel( MIPI_ModuleIndex, 1, 0 );
	NX_MIPI_CSI_SetSize  ( MIPI_ModuleIndex, 1, WIDTH, HEIGHT );
	NX_MIPI_CSI_SetVCLK( MIPI_ModuleIndex, 1, NX_MIPI_CSI_VCLKSRC_EXTCLK );

	NX_MIPI_CSI_SetPhy( MIPI_ModuleIndex
						,0 // U32   NumberOfDataLanes              , // 0~3
						,1 // CBOOL EnableClockLane                ,
						,1 // CBOOL EnableDataLane0                ,
						,0 // CBOOL EnableDataLane1                ,
						,0 // CBOOL EnableDataLane2                ,
						,0 // CBOOL EnableDataLane3                ,
						,0 // CBOOL SwapClockLane                  ,
						,0 // CBOOL SwapDataLane                   
						);

	NX_MIPI_CSI_SetEnable ( MIPI_ModuleIndex, CTRUE );

	//-------------------------------------------------
	//	DPHY
	//-------------------------------------------------
	//NX_RESET_LeaveReset( NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_S ) );			
	//NX_RESET_LeaveReset( NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_M ) );			
#if defined(CONFIG_MACH_S5P4418)
	NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_S ), RSTCON_nENABLE );
	//NX_RSTCON_SetnRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_M ), RSTCON_nENABLE );
#elif defined(CONFIG_MACH_S5P6818)
	NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_S ), RSTCON_NEGATE );
	//			//NX_RSTCON_SetRST(NX_MIPI_GetResetNumber ( MIPI_ModuleIndex, NX_MIPI_RST_PHY_M ), RSTCON_NEGATE );
#endif
	NX_MIPI_DSI_SetPLL( MIPI_ModuleIndex 
						,CTRUE	     // CBOOL Enable      ,  
						,0xFFFFFFFF  // U32 PLLStableTimer,
						,0x33E8	     // 19'h033E8: 1Ghz  19'h043E8: 750Mhz // Use LN28LPP_MipiDphyCore1p5Gbps_Supplement.
						,0xF         // 4'hF: 1Ghz  4'hC: 750Mhz           // Use LN28LPP_MipiDphyCore1p5Gbps_Supplement.
						,0	         // U32 M_PLLCTL      , // Refer to 10.2.2 M_PLLCTL of MIPI_D_PHY_USER_GUIDE.pdf  Default value is all "0". If you want to change register values, it need to confirm from IP Design Team
						,0	         // U32 B_DPHYCTL       // Refer to 10.2.3 M_PLLCTL of MIPI_D_PHY_USER_GUIDE.pdf or NX_MIPI_PHY_B_DPHYCTL enum or LN28LPP_MipiDphyCore1p5Gbps_Supplement. default value is all "0". If you want to change register values, it need to confirm from IP Design Team						  
						);
}
#endif



#define VIP_MODULE_NUM  0
int module_id = VIP_MODULE_NUM;

void camera_run(void)
{
#ifndef CONFIG_TW9992
    int camera_id = camera_register_sensor(&_sensor_data);
    camera_sensor_run(camera_id);
    nxp_vip_register_param(module_id, &_vip_param);

    nxp_vip_set_addr(module_id, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR);

    nxp_mlc_video_set_param(0, &_mlc_param);
    nxp_mlc_video_set_addr(0, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR,
            ALIGN(CAM_WIDTH, 64),
            ALIGN(CAM_WIDTH/2, 64),
            ALIGN(CAM_WIDTH/2, 64));
    /*printf("%s exit\n", __func__);*/
#else

    const U32 VIP_ModuleIndex = 0;
    const U32 MIPI_ModuleIndex  = 0;

	module_id = 0;
#if 1
	initialize_tw9992();

	nxp_vip_setting(module_id, &tw9992_vip_param);

	nxp_mipi_csi_setting(&tw9992_vip_param);

    nxp_mlc_video_set_param(0, &tw9992_mlc_param);
    nxp_mlc_video_set_addr(0, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR,
            ALIGN(CAM_WIDTH, 64),
            ALIGN(CAM_WIDTH/2, 64),
            ALIGN(CAM_WIDTH/2, 64));

	mdelay(100);
#else
    DEBUG_POINT;
    {
        int x,y;        
        for( y=0; y<TW9992_HEIGHT; y++ )
        for( x=0; x<TW9992_WIDTH ; x++ )
        {
            if( x==0 || x==TW9992_WIDTH -1 || 
                x==1 || x==TW9992_WIDTH -2 || 
                x==4 || x==TW9992_WIDTH -5 || 
                x==5 || x==TW9992_WIDTH -6 ||                 
                y==0 || y==TW9992_WIDTH -1 || 
                y==1 || y==TW9992_WIDTH -2 || 
                y==4 || y==TW9992_WIDTH -5 || 
                y==5 || y==TW9992_WIDTH -6 )
            {
                ((unsigned char*)CONFIG_VIP_LU_ADDR)[((y  )*ALIGN(TW9992_WIDTH  , 64)) + (x  )] = 0xFF;
                ((unsigned char*)CONFIG_VIP_CB_ADDR)[((y/2)*ALIGN(TW9992_WIDTH/2, 64)) + (x/2)] = 0x00;
                ((unsigned char*)CONFIG_VIP_CR_ADDR)[((y/2)*ALIGN(TW9992_WIDTH/2, 64)) + (x/2)] = 0x80;
            }
            else if( (x^y)&0x20 )
            {
                ((unsigned char*)CONFIG_VIP_LU_ADDR)[((y  )*ALIGN(TW9992_WIDTH  , 64)) + (x  )] = 0;
                ((unsigned char*)CONFIG_VIP_CB_ADDR)[((y/2)*ALIGN(TW9992_WIDTH/2, 64)) + (x/2)] = 0;
                ((unsigned char*)CONFIG_VIP_CR_ADDR)[((y/2)*ALIGN(TW9992_WIDTH/2, 64)) + (x/2)] = 0;
            }
            else
            {
                ((unsigned char*)CONFIG_VIP_LU_ADDR)[((y  )*ALIGN(TW9992_WIDTH  , 64)) + (x  )] = 0xFF;
                ((unsigned char*)CONFIG_VIP_CB_ADDR)[((y/2)*ALIGN(TW9992_WIDTH/2, 64)) + (x/2)] = 0xFF;
                ((unsigned char*)CONFIG_VIP_CR_ADDR)[((y/2)*ALIGN(TW9992_WIDTH/2, 64)) + (x/2)] = 0xFF;
            }
        }
    }
    nxp_mlc_video_set_param(0, &tw9992_mlc_param);
    nxp_mlc_video_set_addr(0, CONFIG_VIP_LU_ADDR, CONFIG_VIP_CB_ADDR, CONFIG_VIP_CR_ADDR,
            ALIGN(TW9992_WIDTH, 64),
            ALIGN(TW9992_WIDTH/2, 64),
            ALIGN(TW9992_WIDTH/2, 64));
    
    DEBUG_POINT;
    nxp_mlc_video_run(0);
    
    return;
#endif    

    

#if 0 // debug
    printf("nick - TIME_STAMP: %s %s \n", __DATE__, __TIME__);        
    {
        unsigned intCount[36];
        volatile NX_MIPI_RegisterSet* pmipi = (volatile NX_MIPI_RegisterSet*)IO_ADDRESS(NX_MIPI_GetPhysicalAddress(MIPI_ModuleIndex));
        volatile NX_VIP_RegisterSet*  pvip  = (volatile NX_VIP_RegisterSet* )IO_ADDRESS(NX_VIP_GetPhysicalAddress (VIP_ModuleIndex ));
        U32 CSIS_INTSRC;
        //U32 CSIS_INTSRC_block_log = 0;
        pmipi->CSIS_INTMSK = 0xFFFFFFFF;
        pmipi->CSIS_INTSRC = 0xFFFFFFFF;
        NX_VIP_SetInterruptEnable     (VIP_ModuleIndex , NX_VIP_INT_VSYNC , CTRUE );
        NX_VIP_SetInterruptEnable     (VIP_ModuleIndex , NX_VIP_INT_HSYNC , CFALSE);        
        NX_VIP_SetInterruptEnable     (VIP_ModuleIndex , NX_VIP_INT_DONE  , CTRUE );        
        //NX_VIP_SetInterruptEnableAll(VIP_ModuleIndex, CTRUE );
        NX_VIP_ClearInterruptPendingAll(VIP_ModuleIndex );
        
        memset( intCount, 0, sizeof(intCount) );
        
        DEBUG_POINT;
        //while( 1 )
		//while(!ctrlc())
        {
            //printf("nick - pmipi->CSIS_CONTROL: %08x\n", pmipi->CSIS_CONTROL );                    
            //printf("nick - pmipi->CSIS_INTSRC : %08x\n", pmipi->CSIS_INTSRC );                    
            CSIS_INTSRC = pmipi->CSIS_INTSRC ;
            //CSIS_INTSRC &= (~CSIS_INTSRC_block_log);
            //CSIS_INTSRC_block_log |= (CSIS_INTSRC & 0x000FFFFF);
            //if( CSIS_INTSRC & 0xFFF00000 ) CSIS_INTSRC_block_log=0;
            pmipi->CSIS_INTSRC = 0xFFFFFFFF;
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_EvenBefore     ) ){ intCount[ 0]++; printf( "CSI_EvenBefore     (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_EvenAfter      ) ){ intCount[ 1]++; printf( "CSI_EvenAfter      (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_OddBefore      ) ){ intCount[ 2]++; printf( "CSI_OddBefore      (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_OddAfter       ) ){ intCount[ 3]++; printf( "CSI_OddAfter       (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_FrameStart_CH3 ) ){ intCount[ 4]++; printf( "CSI_FrameStart_CH3 (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_FrameStart_CH2 ) ){ intCount[ 5]++; printf( "CSI_FrameStart_CH2 (%08x)\n" , CSIS_INTSRC ); }
            //if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_FrameStart_CH1 ) ){ intCount[ 6]++; printf( "CSI_FrameStart_CH1 (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_FrameStart_CH0 ) ){ intCount[ 7]++; printf( "CSI_FrameStart_CH0 (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_FrameEnd_CH3   ) ){ intCount[ 8]++; printf( "CSI_FrameEnd_CH3   (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_FrameEnd_CH2   ) ){ intCount[ 9]++; printf( "CSI_FrameEnd_CH2   (%08x)\n" , CSIS_INTSRC ); }
            //if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_FrameEnd_CH1   ) ){ intCount[10]++; printf( "CSI_FrameEnd_CH1   (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_FrameEnd_CH0   ) ){ intCount[11]++; printf( "CSI_FrameEnd_CH0   (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_SOT_HS_CH3 ) ){ intCount[12]++; printf( "CSI_ERR_SOT_HS_CH3 (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_SOT_HS_CH2 ) ){ intCount[13]++; printf( "CSI_ERR_SOT_HS_CH2 (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_SOT_HS_CH1 ) ){ intCount[14]++; printf( "CSI_ERR_SOT_HS_CH1 (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_SOT_HS_CH0 ) ){ intCount[15]++; printf( "CSI_ERR_SOT_HS_CH0 (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_LOST_FS_CH3) ){ intCount[16]++; printf( "CSI_ERR_LOST_FS_CH3(%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_LOST_FS_CH2) ){ intCount[17]++; printf( "CSI_ERR_LOST_FS_CH2(%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_LOST_FS_CH1) ){ intCount[18]++; printf( "CSI_ERR_LOST_FS_CH1(%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_LOST_FS_CH0) ){ intCount[19]++; printf( "CSI_ERR_LOST_FS_CH0(%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_LOST_FE_CH3) ){ intCount[20]++; printf( "CSI_ERR_LOST_FE_CH3(%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_LOST_FE_CH2) ){ intCount[21]++; printf( "CSI_ERR_LOST_FE_CH2(%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_LOST_FE_CH1) ){ intCount[22]++; printf( "CSI_ERR_LOST_FE_CH1(%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_LOST_FE_CH0) ){ intCount[23]++; printf( "CSI_ERR_LOST_FE_CH0(%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_OVER_CH3   ) ){ intCount[24]++; printf( "CSI_ERR_OVER_CH3   (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_OVER_CH2   ) ){ intCount[25]++; printf( "CSI_ERR_OVER_CH2   (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_OVER_CH1   ) ){ intCount[26]++; printf( "CSI_ERR_OVER_CH1   (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_OVER_CH0   ) ){ intCount[27]++; printf( "CSI_ERR_OVER_CH0   (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_ECC        ) ){ intCount[28]++; printf( "CSI_ERR_ECC        (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_CRC        ) ){ intCount[29]++; printf( "CSI_ERR_CRC        (%08x)\n" , CSIS_INTSRC ); }
            if( CSIS_INTSRC & (1<<NX_MIPI_INT_CSI_ERR_ID         ) ){ intCount[30]++; printf( "CSI_ERR_ID         (%08x)\n" , CSIS_INTSRC ); }
                

		    if( NX_VIP_GetInterruptPending( VIP_ModuleIndex, NX_VIP_INT_VSYNC) )
		    {
                intCount[31]++; //printf( "NX_VIP_INT_VSYNC \n" );
                NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_VSYNC );
                NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_VSYNC );
                NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_VSYNC );
                NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_VSYNC );
            }
    	    if( NX_VIP_GetInterruptPending( VIP_ModuleIndex, NX_VIP_INT_HSYNC) )
    	    {
    	        intCount[32]++; //printf( "NX_VIP_INT_HSYNC \n" );
    	        NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_HSYNC );
    	        NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_HSYNC );
    	        NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_HSYNC );
    	        NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_HSYNC );
    	    }
	        if( NX_VIP_GetInterruptPending( VIP_ModuleIndex, NX_VIP_INT_DONE ) ) 
	        {
	            intCount[33]++; //printf( "NX_VIP_INT_DONE  \n" );
	            NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_DONE );
	            NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_DONE );
	            NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_DONE );
	            NX_VIP_ClearInterruptPending(VIP_ModuleIndex, NX_VIP_INT_DONE );
            }
            
            //pvip->VIP_CONFIG |= (1<<8);
            //pvip->VIP_SYNCCTRL &= 0xFCFF;
            //{
            //    U32 temp = pvip->VIP_SYNCMON;
            //    if( temp ) printf( "VIP_SYNCMON : %08x\n", temp );
            //}
            
            if( intCount[ 6]==200 )
            {
                printf( "CSI_EvenBefore     (%4d)\n" , intCount[ 0] ); 
                printf( "CSI_EvenAfter      (%4d)\n" , intCount[ 1] ); 
                printf( "CSI_OddBefore      (%4d)\n" , intCount[ 2] ); 
                printf( "CSI_OddAfter       (%4d)\n" , intCount[ 3] ); 
                printf( "CSI_FrameStart_CH3 (%4d)\n" , intCount[ 4] ); 
                printf( "CSI_FrameStart_CH2 (%4d)\n" , intCount[ 5] ); 
                printf( "CSI_FrameStart_CH1 (%4d)\n" , intCount[ 6] ); 
                printf( "CSI_FrameStart_CH0 (%4d)\n" , intCount[ 7] ); 
                printf( "CSI_FrameEnd_CH3   (%4d)\n" , intCount[ 8] ); 
                printf( "CSI_FrameEnd_CH2   (%4d)\n" , intCount[ 9] ); 
                printf( "CSI_FrameEnd_CH1   (%4d)\n" , intCount[10] ); 
                printf( "CSI_FrameEnd_CH0   (%4d)\n" , intCount[11] ); 
                printf( "CSI_ERR_SOT_HS_CH3 (%4d)\n" , intCount[12] ); 
                printf( "CSI_ERR_SOT_HS_CH2 (%4d)\n" , intCount[13] ); 
                printf( "CSI_ERR_SOT_HS_CH1 (%4d)\n" , intCount[14] ); 
                printf( "CSI_ERR_SOT_HS_CH0 (%4d)\n" , intCount[15] ); 
                printf( "CSI_ERR_LOST_FS_CH3(%4d)\n" , intCount[16] ); 
                printf( "CSI_ERR_LOST_FS_CH2(%4d)\n" , intCount[17] ); 
                printf( "CSI_ERR_LOST_FS_CH1(%4d)\n" , intCount[18] ); 
                printf( "CSI_ERR_LOST_FS_CH0(%4d)\n" , intCount[19] ); 
                printf( "CSI_ERR_LOST_FE_CH3(%4d)\n" , intCount[20] ); 
                printf( "CSI_ERR_LOST_FE_CH2(%4d)\n" , intCount[21] ); 
                printf( "CSI_ERR_LOST_FE_CH1(%4d)\n" , intCount[22] ); 
                printf( "CSI_ERR_LOST_FE_CH0(%4d)\n" , intCount[23] ); 
                printf( "CSI_ERR_OVER_CH3   (%4d)\n" , intCount[24] ); 
                printf( "CSI_ERR_OVER_CH2   (%4d)\n" , intCount[25] ); 
                printf( "CSI_ERR_OVER_CH1   (%4d)\n" , intCount[26] ); 
                printf( "CSI_ERR_OVER_CH0   (%4d)\n" , intCount[27] ); 
                printf( "CSI_ERR_ECC        (%4d)\n" , intCount[28] ); 
                printf( "CSI_ERR_CRC        (%4d)\n" , intCount[29] ); 
                printf( "CSI_ERR_ID         (%4d)\n" , intCount[30] ); 
                
                printf( "VIP_INT_VSYNC      (%4d)\n" , intCount[31] ); 
                printf( "VIP_INT_HSYNC      (%4d)\n" , intCount[32] ); 
                printf( "VIP_INT_DONE       (%4d)\n" , intCount[33] ); 
                
                printf( "VIP size : %d x %d\n", NX_VIP_GetHorCount(VIP_ModuleIndex) , NX_VIP_GetVerCount(VIP_ModuleIndex) );
                               
             
                memset( intCount, 0, sizeof(intCount) );   
            }
        }
        
    }
    
//    {
//        volatile NX_MIPI_RegisterSet* pmipi = (volatile NX_MIPI_RegisterSet*)IO_ADDRESS(NX_MIPI_GetPhysicalAddress(MIPI_ModuleIndex));
//        pmipi->CSIS_INTMSK = 0xFFFFFFFF;
//        pmipi->CSIS_INTSRC = 0xFFFFFFFF;
//        
//        DEBUG_POINT;
//        while( 1 )
//        {
//            printf("nick - pmipi->CSIS_CONTROL: %08x\n", pmipi->CSIS_CONTROL );                    
//            printf("nick - pmipi->CSIS_INTSRC : %08x\n", pmipi->CSIS_INTSRC );                    
//            pmipi->CSIS_INTSRC = 0xFFFFFFFF;
//        }
//    }        
#endif



    /*printf("%s exit\n", __func__);*/    
#endif

}

// test for sensor i2c read
#include <i2c.h>
static u8 video_status = 0x80;
static void read_tw9900_intstatus(int bus_num, int chip_addr)
{
    u8 val;
    i2c_set_bus_num(bus_num);
    i2c_read(chip_addr, 0xb6, 1, &val, 1);
    printf("INT1RAWSTATUS: 0x%x\n", val);
    i2c_read(chip_addr, 0xb7, 1, &val, 1);
    printf("INT2RAWSTATUS: 0x%x\n", val);
    i2c_read(chip_addr, 0x01, 1, &val, 1);
    printf("STATUS1: 0x%x\n", val);
    video_status = val;
    i2c_read(chip_addr, 0xb9, 1, &val, 1);
    printf("INT1STATUS: 0x%x\n", val);
    i2c_read(chip_addr, 0xba, 1, &val, 1);
    printf("INT2STATUS: 0x%x\n", val);
}

#define CAMERA_ACTIVE_DETECT (PAD_GPIO_A + 6)
#define CAMERA_DETECT_ACTIVE_VALUE 0
static int is_tw9900_video_in(int bus_num, int chip_addr)
{
    u8 val;
    i2c_set_bus_num(bus_num);
    i2c_read(chip_addr, 0x01, 1, &val, 1);
    /*printf("%s: val 0x%x, %d\n", __func__, val, !(0x80 & val));*/
    // check twice
    if (val & 0x80) {
        mdelay(10);
        i2c_read(chip_addr, 0x01, 1, &val, 1);
        /*printf("%s: check2 val 0x%x, %d\n", __func__, val, !(0x80 & val));*/
    }
    if (val & 0x80) {
        mdelay(10);
        i2c_read(chip_addr, 0x01, 1, &val, 1);
        /*printf("%s: check3 val 0x%x, %d\n", __func__, val, !(0x80 & val));*/
    }
    /*{*/
        /*int io = CAMERA_ACTIVE_DETECT;*/
        /*int val;*/

        /*gpio_direction_input(io);*/
        /*gpio_set_alt(io, 0);*/
        /*val = gpio_get_value(io);*/
        /*printf("gpio val: %d\n", val);*/
    /*}*/
    /*{*/
        /*int io = PAD_GPIO_ALV + 4;*/
        /*int val;*/

        /*gpio_direction_input(io);*/
        /*gpio_set_alt(io, 0);*/
        /*val = gpio_get_value(io);*/
        /*printf("gpio val2: %d\n", val);*/
    /*}*/
    /*read_tw9900_intstatus(bus_num, chip_addr);*/
    return !(0x80 & val);
}

void camera_preview(void)
{

#ifndef CONFIG_TW9992
#if 0
    int io = CAMERA_ACTIVE_DETECT;
    int val;

    gpio_direction_input(io);
    gpio_set_alt(io, 0);
    val = gpio_get_value(io);

    read_tw9900_intstatus(_sensor_data.bus, _sensor_data.chip);

    if (val == CAMERA_DETECT_ACTIVE_VALUE) {
        printf("run vip & mlc\n");
        nxp_vip_run(module_id);
        nxp_mlc_video_run(0);
    }
#else
    /*read_tw9900_intstatus(_sensor_data.bus, _sensor_data.chip);*/
    /*int io = CAMERA_ACTIVE_DETECT;*/
    /*int val;*/

    /*gpio_direction_input(io);*/
    /*gpio_set_alt(io, 0);*/
    /*val = gpio_get_value(io);*/
    /*printf("gpio val: %d\n", val);*/
    if (is_tw9900_video_in(_sensor_data.bus, _sensor_data.chip)) {
        printf("run vip & mlc\n");
        nxp_vip_run(module_id);
        nxp_mlc_video_run(0);
    }
#endif


#else // CONFIG_TW9992

	printf("run vip & mlc, module_id:%d \n", module_id);
	nxp_vip_run(module_id);
	nxp_mlc_video_run(0);

#if 0
	{
		int i = 0;
		for(i=0; i<256; i++)
		{
	        printf("tw9992_init_data reg[0x%02x]:[0x%02x]\n", tw9992_sensor_init_data[i].reg, tw9992_sensor_init_data[i].val);
			if(tw9992_sensor_init_data[i].reg == 0xff)
				break;
		}
	}
#endif

#endif // CONFIG_TW9992

	//printf("%s exit - TIME_STAMP: %s %s \n", __func__, __DATE__, __TIME__);            
	//while(!ctrlc());

}
