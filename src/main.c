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

/*
#include <zephyr/mgmt/mcumgr/smp_bt.h>
#include "os_mgmt/os_mgmt.h"
#include "img_mgmt/img_mgmt.h"
*/
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include "tof.h"

#include <nrfx_twi.h>


#define SLEEP_TIME_MS   1000
#define LED_B_NODE DT_ALIAS(led_blue)
#define LED_G_NODE DT_ALIAS(led_green)
#define LED_R_NODE DT_ALIAS(led_red)

static const struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET(LED_B_NODE, gpios);
static const struct gpio_dt_spec ledG = GPIO_DT_SPEC_GET(LED_G_NODE, gpios);
static const struct gpio_dt_spec ledR = GPIO_DT_SPEC_GET(LED_R_NODE, gpios);


//SPI TEST
#define SLEEP_TIME_MS   1000
#define MY_SPI_MASTER DT_NODELABEL(my_spi_master)

/*
const struct device *spi_dev;

static struct k_poll_signal spi_done_sig = K_POLL_SIGNAL_INITIALIZER(spi_done_sig);
struct spi_cs_control spim_cs = {
	.gpio = SPI_CS_GPIOS_DT_SPEC_GET(DT_NODELABEL(reg_my_spi_master)),
	.delay = 0,
};
static const struct spi_config spi_cfg = {
	.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB |
				 SPI_MODE_CPOL | SPI_MODE_CPHA,
	.frequency = 400000,
	.slave = 0,
	.cs = &spim_cs,
};


static int spi_write_test_msg(void)
{
	static uint8_t counter = 0;
	static uint8_t tx_buffer[4];
	static uint8_t rx_buffer[4];

	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer)
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1
	};

	struct spi_buf rx_buf = {
		.buf = rx_buffer,
		.len = sizeof(rx_buffer),
	};
	const struct spi_buf_set rx = {
		.buffers = &rx_buf,
		.count = 1
	};

	// Update the TX buffer with a rolling counter
	tx_buffer[0] = counter++;
	printf("SPI TX: 0x%.2x, 0x%.2x\n", tx_buffer[0], tx_buffer[1]);

	// Reset signal
	k_poll_signal_reset(&spi_done_sig);
	
	// Start transaction
	int error = spi_transceive_async(spi_dev, &spi_cfg, &tx, &rx, &spi_done_sig);
	if(error != 0){
		printf("SPI transceive error: %i\n", error);
		return error;
	}

	// Wait for the done signal to be raised and log the rx buffer
	int spi_signaled, spi_result;
	do{
		k_poll_signal_check(&spi_done_sig, &spi_signaled, &spi_result);
	} while(spi_signaled == 0);
	printf("SPI RX: 0x%.2x, 0x%.2x\n", rx_buffer[0], rx_buffer[1]);
	return 0;
}
*/
void main(void)
{
	uint8_t error;
	
	//init over-the-air updates
/*
	os_mgmt_register_group();
	img_mgmt_register_group();
	smp_bt_register();
*/
	//init ble, i2c, leds
	
	init_ble();
	error = i2c_configure(device_get_binding("i2c"),I2C_SPEED_SET(I2C_SPEED_FAST));

	
	
	k_sleep(K_MSEC(150));
		
		//spi_init();
	//spi_write_test_msg();

	//init sensors
	init_bmp();
	init_bmi();
	init_shtc();
	//init_tof();
	//init_veml();
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
