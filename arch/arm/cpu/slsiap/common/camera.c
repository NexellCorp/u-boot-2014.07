#include <common.h>
#include <command.h>
#include <i2c.h>

#include "camera.h"

#define MAX_SENSOR_NUM 2
static struct camera_sensor_data *_sensor_list[MAX_SENSOR_NUM] = { NULL, };
static int _sensor_num = 0;

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

		struct reg_val *regarray;
		int i=0;
		u8 value=0;

    if (id >= MAX_SENSOR_NUM || _sensor_list[id] == NULL) {
         printf("%s: invalid id %d\n", __func__, id);
         return -1;
    }

    sensor = _sensor_list[id];

    if (sensor->power_enable)
        sensor->power_enable(true);

    if (sensor->setup_io)
        sensor->setup_io();

    if (sensor->set_clock)
        sensor->set_clock(sensor->clk_rate);

    ret = i2c_set_bus_num(sensor->bus);
    if (ret)
        return ret;

		regarray = sensor->reg_val;
    reg_val = sensor->reg_val;

    while (reg_val->reg != 0xff) {
       ret =  i2c_write(sensor->chip, reg_val->reg, 1, &reg_val->val, 1);
			 //printf("Write Register : 0x%02X = Value : W[0x%02X], return : %d\n", reg_val->reg, reg_val->val, ret);
			 mdelay(10);
       reg_val++;
    }

#if 0
		printf("#################### Read Register Start#############################\n");
		while((regarray[i].reg != 0xff))
		{
			ret = i2c_read(sensor->chip, regarray[i].reg, 1, &value, 1);
			if(ret !=0)
				printf("read value failed\n");

			//if(regarray[i].val != value)
			//	printf("%s reg[0x%02X] check err,writte :0x%02X  read:0x%02X\n", __FUNCTION__, regarray[i].reg, regarray[i].val, value);
				
			printf("%s reg[0x%02X], writte :0x%02X  read:0x%02X\n", __FUNCTION__, regarray[i].reg, regarray[i].val, value);

			i++;
		}   
		printf("#################### Read Register End #############################\n");
#endif

    return 0;
}
