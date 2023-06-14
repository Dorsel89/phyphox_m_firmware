#ifndef TOF_H
#define _TOF_H


#include "sensors.h"
#include <zephyr/drivers/gpio.h>
#include "VL53L4CD_api.h"
#include <stdio.h>

#include "ble.h"


#define TOF_INT DT_NODELABEL(button4)

extern int8_t init_tof();
extern void update_config_tof();
extern uint8_t sleep_tof(bool SLEEP);

static struct k_work work_tof;
static struct k_work config_work_tof;

static int8_t tofResult;

static Dev_t dev;
static uint8_t status, loop, isReady;
static uint16_t sensor_id;
static VL53L4CD_ResultsData_t results;

extern void send_data_tof();
static int8_t configure_tof_int();
#endif