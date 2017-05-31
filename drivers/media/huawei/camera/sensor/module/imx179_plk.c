/*
 *  Hisilicon K3 SOC camera driver source file
 *
 *  Copyright (C) Huawei Technology Co., Ltd.
 *
 * Author:	  y00285329
 * Email:	  yinxuerui@huawei.com
 * Date:	  2014-12-08
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

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

#include "hwsensor.h"
#include "sensor_commom.h"
#include "hw_csi.h"

#define I2S(i) container_of(i, sensor_t, intf)

//add imx179_plk
extern struct hw_csi_pad hw_csi_pad;
static hwsensor_vtbl_t s_imx179_plk_vtbl;

int imx179_plk_config(hwsensor_intf_t* si, void  *argp);

static bool is_poweron = false;

struct sensor_power_setting imx179_plk_power_setting[] = {
	//MINIISP CS
	{
		.seq_type = SENSOR_CS,
		.config_val = SENSOR_GPIO_HIGH,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MINIISP CORE 1.1V-->1.8Switch
	{
		.seq_type = SENSOR_SUSPEND,
		.config_val = SENSOR_GPIO_LOW,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MINIISP IOVDD 1.8V
	{
		.seq_type = SENSOR_IOVDD,
		.data = (void*)"common-iovdd",
		.config_val = LDO_VOLTAGE_1P8V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MCAM1 AFVDD 2.85V
	{
		.seq_type = SENSOR_VCM_AVDD2,
		.data = (void*)"cameravcm-vcc",
		.config_val = LDO_VOLTAGE_V2P85V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MCAM1 AVDD 2.85V-->2.5V
	{
		.seq_type = SENSOR_AVDD2,
		.data = (void*)"main-sensor-avdd",
		.config_val = LDO_VOLTAGE_V2P5V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MCAM1 DVDD 1.0V
	{
		.seq_type = SENSOR_DVDD2,
		.config_val = LDO_VOLTAGE_1P05V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},
	//SCAM AVDD 2.85V
	{
		.seq_type = SENSOR_AVDD,
		.data = (void*)"slave-sensor-avdd",
		.config_val = LDO_VOLTAGE_V2P85V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//SCAM DVDD1.2V
    {
        .seq_type = SENSOR_DVDD,
        .config_val = LDO_VOLTAGE_1P2V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = 1,
    },

	{
		.seq_type = SENSOR_MCLK,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},
	{
		.seq_type = SENSOR_RST,
		.config_val = SENSOR_GPIO_LOW,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},
};

struct sensor_power_setting imx179_plk_power_setting_va[] = {
	//MINIISP CS
	{
		.seq_type = SENSOR_CS,
		.config_val = SENSOR_GPIO_HIGH,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MINIISP CORE 1.1V-->1.8Switch
	{
		.seq_type = SENSOR_MINIISP_VPP,
		.data = (void*)"miniISP-1P1V",
		.config_val = PMIC_1P8V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MINIISP IOVDD 1.8V
	{
		.seq_type = SENSOR_IOVDD,
		.data = (void*)"common-iovdd",
		.config_val = LDO_VOLTAGE_1P8V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MCAM1 AFVDD 2.85V
	{
		.seq_type = SENSOR_VCM_AVDD2,
		.data = (void*)"cameravcm-vcc",
		.config_val = LDO_VOLTAGE_V2P85V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MCAM1 AVDD 2.85V-->2.5V
	{
		.seq_type = SENSOR_AVDD2,
		.data = (void*)"main-sensor-avdd",
		.config_val = LDO_VOLTAGE_V2P5V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//MCAM1 DVDD 1.0V
	{
		.seq_type = SENSOR_DVDD2,
		.config_val = LDO_VOLTAGE_1P05V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},
	//SCAM AVDD 2.85V
	{
		.seq_type = SENSOR_AVDD,
		.data = (void*)"slave-sensor-avdd",
		.config_val = LDO_VOLTAGE_V2P85V,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},

	//SCAM DVDD1.2V
    {
        .seq_type = SENSOR_DVDD,
        .config_val = LDO_VOLTAGE_1P2V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = 1,
    },

	{
		.seq_type = SENSOR_MCLK,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},
	{
		.seq_type = SENSOR_RST,
		.config_val = SENSOR_GPIO_LOW,
		.sensor_index = SENSOR_INDEX_INVALID,
		.delay = 1,
	},
};

static sensor_t s_imx179_plk =
{
    .intf = { .vtbl = &s_imx179_plk_vtbl, },
    .power_setting_array = {
            .size = ARRAY_SIZE(imx179_plk_power_setting),
            .power_setting = imx179_plk_power_setting,
     },
};

static const struct of_device_id
s_imx179_plk_dt_match[] =
{
	{
        .compatible = "huawei,imx179_plk_sun",
        .data = &s_imx179_plk.intf,
    },
	{
    },
};

MODULE_DEVICE_TABLE(of, s_imx179_plk_dt_match);

static struct platform_driver
s_imx179_plk_driver =
{
	.driver =
    {
		.name = "huawei,imx179_plk",
		.owner = THIS_MODULE,
		.of_match_table = s_imx179_plk_dt_match,
	},
};

char const*
imx179_plk_get_name(
        hwsensor_intf_t* si)
{
    sensor_t* sensor = I2S(si);
    return sensor->board_info->name;
}

int
imx179_plk_power_up(
        hwsensor_intf_t* si)
{
	int ret = 0;
	sensor_t* sensor = NULL;
	sensor = I2S(si);
	if(!is_poweron) {
		ret = hw_sensor_power_up(sensor);
		cam_notice("+++imx179_plk power on!+++");
		is_poweron = true;
	} else {
		cam_notice("+++not power on+++");
	}
	return ret;
}

int
imx179_plk_power_down(
        hwsensor_intf_t* si)
{
	int ret = 0;
	sensor_t* sensor = NULL;
	sensor = I2S(si);
	if(is_poweron) {
		ret = hw_sensor_power_down(sensor);
		cam_notice("---imx179_plk power off!---");
		is_poweron = false;
	} else {
		cam_notice("---not power off---");
	}
	return ret;
}

int imx179_plk_csi_enable(hwsensor_intf_t* si)
{
	int ret = 0;
	sensor_t* sensor = NULL;
	sensor = I2S(si);

	ret = hw_csi_pad.hw_csi_enable(sensor->board_info->csi_index, sensor->board_info->csi_lane, sensor->board_info->csi_mipi_clk);
	return ret;
}

int imx179_plk_csi_disable(hwsensor_intf_t* si)
{
	int ret = 0;
	sensor_t* sensor = NULL;
	sensor = I2S(si);
	ret = hw_csi_pad.hw_csi_disable(sensor->board_info->csi_index);
	return ret;
}

static int
imx179_plk_match_id(
        hwsensor_intf_t* si, void * data)
{
    sensor_t* sensor = I2S(si);
    struct sensor_cfg_data *cdata = (struct sensor_cfg_data *)data;
    uint16_t sensor_id = 0;
    uint8_t modue_id = 0;
    cam_info("%s TODO.", __func__);

    cdata->data = SENSOR_INDEX_INVALID;

    if(misp_get_chipid() == EXTISP_AL6010) {
		misp_get_module_info(sensor->board_info->sensor_index,&sensor_id,&modue_id);
		if(sensor_id == 0x179) {
		    cdata->data = sensor->board_info->sensor_index;
		}
    } else {
    	cdata->data = sensor->board_info->sensor_index;
	}
    if(cdata->data != SENSOR_INDEX_INVALID) {
        hwsensor_writefile(sensor->board_info->sensor_index,sensor->board_info->name);
    }
    cam_info("%s TODO.  cdata->data=%d", __func__, cdata->data);
    return 0;
}

static hwsensor_vtbl_t
s_imx179_plk_vtbl =
{
	.get_name = imx179_plk_get_name,
	.config = imx179_plk_config,
	.power_up = imx179_plk_power_up,
	.power_down = imx179_plk_power_down,
	.match_id = imx179_plk_match_id,
	.csi_enable = imx179_plk_csi_enable,
	.csi_disable = imx179_plk_csi_disable,
};

int
imx179_plk_config(
        hwsensor_intf_t* si,
        void  *argp)
{
	struct sensor_cfg_data *data;

	int ret =0;
	data = (struct sensor_cfg_data *)argp;
	cam_debug("imx179_plk cfgtype = %d",data->cfgtype);
	switch(data->cfgtype){
		case SEN_CONFIG_POWER_ON:
			ret = si->vtbl->power_up(si);
			break;
		case SEN_CONFIG_POWER_OFF:
			ret = si->vtbl->power_down(si);
			break;
		case SEN_CONFIG_WRITE_REG:
			break;
		case SEN_CONFIG_READ_REG:
			break;
		case SEN_CONFIG_WRITE_REG_SETTINGS:
			break;
		case SEN_CONFIG_READ_REG_SETTINGS:
			break;
		case SEN_CONFIG_ENABLE_CSI:
			ret = si->vtbl->csi_enable(si);
			break;
		case SEN_CONFIG_DISABLE_CSI:
			ret = si->vtbl->csi_disable(si);
			break;
		case SEN_CONFIG_MATCH_ID:
			ret = si->vtbl->match_id(si,argp);
			break;
		default:
                cam_err("%s cfgtype(%d) is error", __func__, data->cfgtype);
			break;
	}
	cam_debug("%s exit",__func__);
	return ret;
}


//need modify
void extisp_power_on(int on)
{
	if(on)
		hw_sensor_power_up(&s_imx179_plk);
	else
		hw_sensor_power_down(&s_imx179_plk);
}
EXPORT_SYMBOL(extisp_power_on);


static int32_t
imx179_plk_platform_probe(
        struct platform_device* pdev)
{
	int rc = 0;
	cam_debug("enter %s",__func__);

	if (pdev->dev.of_node) {
		rc = hw_sensor_get_dt_data(pdev, &s_imx179_plk);
		if (rc < 0) {
			cam_err("%s failed line %d\n", __func__, __LINE__);
			goto imx179_plk_sensor_probe_fail;
		}

		if (s_imx179_plk.board_info->gpios[SUSPEND].gpio == 0) {
			s_imx179_plk.power_setting_array.power_setting = imx179_plk_power_setting_va;
			s_imx179_plk.power_setting_array.size = ARRAY_SIZE(imx179_plk_power_setting_va);
		}

	} else {
		cam_err("%s imx179_plk of_node is NULL.\n", __func__);
		goto imx179_plk_sensor_probe_fail;
	}

	rc = hwsensor_register(pdev, &s_imx179_plk.intf);
imx179_plk_sensor_probe_fail:
	return rc;
}

static int __init
imx179_plk_init_module(void)
{
	cam_info("Enter: %s", __func__);
    return platform_driver_probe(&s_imx179_plk_driver,
            imx179_plk_platform_probe);
}

static void __exit
imx179_plk_exit_module(void)
{
    platform_driver_unregister(&s_imx179_plk_driver);
}

module_init(imx179_plk_init_module);
module_exit(imx179_plk_exit_module);
MODULE_DESCRIPTION("imx179_plk");
MODULE_LICENSE("GPL v2");

