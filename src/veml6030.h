#ifndef VEML6030_H
#define _VEML6030_H

#include "sensors.h"
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include "i2c_zephyr.h"

#define VEML_INT DT_NODELABEL(button2)
#define VEML_ADDRESS 0x10

extern int8_t init_veml();
extern void update_config_veml();
extern uint8_t sleep_veml(bool SLEEP);

static int8_t set_configuration(bool sleep);
static struct k_work work_veml;
static struct k_work config_work_veml;
static struct k_timer timer_veml;

static bool skipped_first = false;

static int8_t vemlResult;

extern void send_data_veml();
static int8_t configure_veml_int();

static void vemlDataReady(const struct device *dev, struct gpio_callback *cb,uint32_t pins);
static int8_t veml_get_data();

#endif