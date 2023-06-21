#ifndef I2C_ZEPHYR_H
#define _I2C_ZEPHYR_H

#include "sensors.h"
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <zephyr/drivers/i2c.h>

static uint8_t i2c_ret;

const static struct device *tw_dev = DEVICE_DT_GET(DT_ALIAS(i2c));


extern uint8_t write_bytes(uint8_t reg_addr, uint8_t *data, int length, uint8_t device_address);
extern uint8_t read_bytes(uint8_t reg_addr, uint8_t *data, int length, uint8_t device_address);

#endif