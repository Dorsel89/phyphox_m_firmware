#ifndef SHTC3_H
#define SHTC3_H

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>

#include "sensors.h"
#include "ble.h"

#define SHTC_NODE DT_ALIAS(shtc_3)
#if DT_NODE_HAS_STATUS(SHTC_NODE, okay)
const static struct device *shtc_dev = DEVICE_DT_GET(SHTC_NODE);
#else
#error "Node is disabled"
#endif

typedef struct {
	float humidity;
	float temperature;
	float timestamp;
	float array[3];
	uint8_t timer_interval;
	uint8_t config[20];
}SHTC;

extern SHTC shtc_data;

static struct sensor_value shtc_temp, shtc_humid;

static struct k_timer timer_shtc;
static struct k_work work_shtc;
static struct k_work config_work_shtc;

extern bool init_shtc();
extern void sleep_shtc(bool sleep);
extern void submit_config_shtc();

void send_data_shtc();
void shtc_data_ready();
void set_config_shtc();

#endif  // SHTC3_H