#include <common.h>
#include <command.h>
#include <i2c.h>

#include "camera.h"

#define MAX_SENSOR_NUM 2
static struct camera_sensor_data *_sensor_list[MAX_SENSOR_NUM] = { NULL, };
static int _sensor_num = 0;


static uint8_t read_data[] =
{
    0x80,
		0x81,
    0x82,
		0xff
};

static int sensor_check_array(int chip, struct reg_val *regarray)
{
	int ret=0; 
  int i = 0; 

  u8 value=0;

  while((regarray[i].reg != 0xff) || (regarray[i].val != 0xff))
  {    
		ret = i2c_read(chip, regarray[i].reg, 1, &value, 1);
  	if(ret !=0) 
    {    
    	printf("read value failed\n");
    }    

    if(regarray[i].val != value)
    {    
			printf("%s reg[0x%02X] check err,writte :0x%02X  read:0x%02X\n", __FUNCTION__, regarray[i].reg, regarray[i].val, value);
    }    

		i++;
	}    
    
  return 0;
}

int camera_register_sensor(struct camera_sensor_data *data)
{
    int id = _sensor_num;

    if (_sensor_num >= MAX_SENSOR_NUM) {
        printf("%s: can't register %p, exceed limit of camera sensor(%d)\n", __func__, data, _sensor_num);
        return -1;
    }

    _sensor_list[_sensor_num] = data;
    _sensor_num++;
    return id;
}

int camera_sensor_run(int id)
{
    int ret;
    struct camera_sensor_data *sensor;
    struct reg_val *reg_val;
    struct reg_val *reg_val_dis;
    struct reg_val *reg_read;
		uint8_t read_val;
		uint8_t temp;
		uint8_t *r_data;
		volatile u32 *clkgen_base;
		int i=0;

    if (id >= MAX_SENSOR_NUM || _sensor_list[id] == NULL) {
         printf("%s: invalid id %d\n", __func__, id);
         return -1;
    }

    sensor = _sensor_list[id];

    if (sensor->setup_io)
        sensor->setup_io();

    if (sensor->power_enable)
        sensor->power_enable(true);

    if (sensor->set_clock)
        sensor->set_clock(sensor->clk_rate);

    ret = i2c_set_bus_num(sensor->bus);
    if (ret)
        return ret;

#if 0
		if(sensor->clock_invert)
		{
			printf("[%s]KEUN Line : %d\n", __func__, __LINE__);
	
			clkgen_base = (volatile u32 *)IO_ADDRESS(NX_CLKGEN_GetPhysicalAddress(NX_VIP_GetClockNumber(sensor->module)));
			*clkgen_base |= 1 << 4;
		}
#endif
    reg_val = sensor->reg_val;

#if 0
		reg_val_dis = sensor->reg_val_dis;
		 while (reg_val_dis->reg != 0xff) {
        ret = i2c_write(sensor->chip, reg_val_dis->reg, 1, &reg_val_dis->val, 1);
				printf("Write Register : 0x%02X = Value : W[0x%02X], return : %d\n", reg_val_dis->reg, reg_val_dis->val, ret);
				mdelay(10);

        reg_val_dis++;
				
    }
mdelay(1000);
#endif

//#define SP2518              0x53
		printf("#################### Read Register Start#############################\n");
		ret = i2c_read(sensor->chip, 0x02, 1, &read_val, 1);
		printf("Read REG : 0x02 = Value : W[0x%02X] -  R[0x%02X], ret : %d\n", 0x02, read_val, ret);
		printf("#################### Read Register End #############################\n");

#if 1

#if 0
		for(i=0; i<365; i++)
		{
			ret = i2c_write(sensor->chip, reg_val->reg, 1, &reg_val->val, 1);
			printf("Write Register : 0x%02X = Value : W[0x%02X], return : %d\n", reg_val->reg, reg_val->val, ret);
			//mdelay(500);
			//reg_val++;
		}
#endif


    while (reg_val->reg != 0xff) {
        ret = i2c_write(sensor->chip, reg_val->reg, 1, &reg_val->val, 1);
				printf("Write Register : 0x%02X = Value : W[0x%02X], return : %d\n", reg_val->reg, reg_val->val, ret);
				mdelay(10);

//				i2c_read(sensor->chip, reg_val->reg, 1, &read_val, 1);
				//printf("Read REG : 0x%02X = Value : W[0x%02X] -  R[0x%02X]\n", reg_val->reg, reg_val->val, i2c_reg_read(sensor->chip, reg_val->reg));
//				printf("Read REG : 0x%02X = Value : W[0x%02X] -  R[0x%02X]\n", reg_val->reg, reg_val->val, read_val);
//				mdelay(1);

        reg_val++;
				
    }
#else
	for(i=0; i<365; i++)
	{
		ret = i2c_write(sensor->chip, reg_val->reg, 1, &reg_val->val, 1);
		printf("Write Register : 0x%02X = Value : W[0x%02X], return : %d\n", reg_val->reg, reg_val->val, ret);
		//mdelay(500);
    reg_val++;
	}

#endif

#if 0
	printf("#################### Read Register TEST Start#############################\n");

	r_data = read_data;	
	while(*r_data != 0xff)
	{	
		read_val = 0x0;
		temp = 0x20;
		i2c_write(sensor->chip, 0x03, 1, &temp, 1);
		ret = i2c_read(sensor->chip, *r_data, 1, &read_val, 1);
		printf("Read REG : 0x%02X = Value : R[0x%02X], ret : %d\n", *r_data, read_val, ret);

		r_data++;
	}
	printf("#################### Read Register TEST End #############################\n");
#else

	reg_read = sensor->reg_val;
	sensor_check_array(sensor->chip, reg_read); 

#endif

    return 0;
}

