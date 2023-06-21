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

#include <zephyr/mgmt/mcumgr/smp_bt.h>
#include "os_mgmt/os_mgmt.h"
#include "img_mgmt/img_mgmt.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include "tof.h"

#define SLEEP_TIME_MS   1000
#define LED_B_NODE DT_ALIAS(led_blue)
#define LED_G_NODE DT_ALIAS(led_green)
#define LED_R_NODE DT_ALIAS(led_red)

static const struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET(LED_B_NODE, gpios);
static const struct gpio_dt_spec ledG = GPIO_DT_SPEC_GET(LED_G_NODE, gpios);
static const struct gpio_dt_spec ledR = GPIO_DT_SPEC_GET(LED_R_NODE, gpios);

void main(void)
{
	uint8_t error;

	//init over-the-air updates
	os_mgmt_register_group();
	img_mgmt_register_group();
	smp_bt_register();

	//init ble, i2c, leds
	init_ble();
	error = i2c_configure(device_get_binding("i2c"),I2C_SPEED_SET(I2C_SPEED_STANDARD));
	k_sleep(K_MSEC(150));
		
	//init sensors
	//init_bmp();
	init_bmi();
	init_shtc();
	init_tof();
	init_veml();
	init_mlx();
	init_BAS();

	
	// LED

	gpio_pin_configure_dt(&ledB, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&ledR, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&ledG, GPIO_OUTPUT_INACTIVE);
	

	/*
	while (1) {
		//BLUE
		error = gpio_pin_toggle_dt(&ledB);
		k_msleep(SLEEP_TIME_MS);
		//RED
		error = gpio_pin_toggle_dt(&ledB);
		error = gpio_pin_toggle_dt(&ledR);
		k_msleep(SLEEP_TIME_MS);
		//GREEN
		error = gpio_pin_toggle_dt(&ledR);
		error = gpio_pin_toggle_dt(&ledG);
		k_msleep(SLEEP_TIME_MS);
		//OFF
		error = gpio_pin_toggle_dt(&ledG);
		k_msleep(SLEEP_TIME_MS);

	}
	*/
	
}
