#ifndef MLX90392_H
#define MLX90392_H

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include "sensors.h"
#include <stdint.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#define MLX_POWER_DOWN 0
#define CONTINUOUS_10 2
#define CONTINUOUS_20 3
#define CONTINUOUS_50 4
#define CONTINUOUS_100 5
#define CONTINUOUS_200 10
#define CONTINUOUS_500 11
#define CONTINUOUS_700 12
#define CONTINUOUS_1400 13

#define MLX_MODE_REG 0x10
#define MLX_STATUS_REG 0x00
#define MLX_DATA_REG 0x01
#define MLX_CONFIG1_REG 0x14
#define MLX_CONFIG2_REG 0x15

#define mlx_i2c_address 0x0C

#define MLX_NODE DT_ALIAS(i2c)
const static struct device *mlx_dev = DEVICE_DT_GET(MLX_NODE);


static uint8_t read_bytes( uint8_t reg_address, uint8_t bytes, uint8_t* dest);
static uint8_t write_byte(uint8_t reg_address, uint8_t data);

static struct k_timer timer_mlx;
static struct k_work work_mlx;
static struct k_work config_work_mlx;

extern uint8_t readout_sensor();
uint8_t set_mode();
    
extern uint8_t init_mlx();
extern uint8_t sleep_mlx(bool SLEEP);
//extern void submit_config_mlx();
extern void update_config_mlx();
static void mlxDataReady();

#endif