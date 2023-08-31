/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stddef.h>
#include <zephyr/sys/util.h>
#include <zephyr/types.h>
#include "ble.h"
#include "bmpZephyr.h"
#include "bmiZephyr.h"
#include "veml6030.h"

#include <zephyr/mgmt/mcumgr/transport/smp_bt.h>
//#include "os_mgmt/os_mgmt.h"
//#include "img_mgmt/img_mgmt.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
//#include <zephyr/drivers/usb_device.h>
#include <zephyr/usb/usb_device.h>

#include "tof.h"

#include <nrfx_twi.h>
#include "leds.h"



static bool usb_plugged_in = true;

static void status_cb(enum usb_dc_status_code status, const uint8_t *param)
{	
	if(status == USB_DC_CONNECTED){
		gpio_pin_set_dt(&ledB,1);
		usb_plugged_in = true;
	}else if (status == USB_DC_DISCONNECTED)
	{
		gpio_pin_set_dt(&ledB,0);
		gpio_pin_set_dt(&ledG,0);
		usb_plugged_in = false;
	}
}

void main(void)
{
	uint8_t error;
	
	usb_enable(status_cb);
	//init over-the-air updates
	
	init_ble();
	error = i2c_configure(device_get_binding("i2c"),I2C_SPEED_SET(I2C_SPEED_FAST));

	k_sleep(K_MSEC(150));

	//init sensors
	init_bmp();
	init_bmi();
	init_shtc();
	//init_tof();
	//init_veml();
	init_mlx();
	init_BAS(&usb_plugged_in);
	
	// LED

	gpio_pin_configure_dt(&ledB, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&ledR, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&ledG, GPIO_OUTPUT_INACTIVE);	
}
