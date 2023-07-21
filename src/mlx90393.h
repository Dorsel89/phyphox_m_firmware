#ifndef _MLX90393_H
#define _MLX90393_H

#include <zephyr/types.h>
#include <zephyr/drivers/i2c.h>
#include "sensors.h"

//#include <sys/util.h>

static struct device *mlx_device_t;

bool mlx_readData(float *x, float *y, float *z, struct device *mlx_device_t);
uint8_t startSingleMeasurement(struct device *mlx_device_t);
uint8_t startBurstMode(struct device *mlx_device_t);
bool mlx_exitMode(struct device *mlx_device_t);
bool mlx_readMeasurement(float *x, float *y, float *z, struct device *mlx_device_t);
bool mlx_exitMode(struct device *mlx_device_t);

#define MLX90393_DEFAULT_ADDR (0x0C) /* Can also be 0x18, depending on IC */

//const struct device *mlx_device_t = device_get_binding("I2C_1");

#define MLX90393_AXIS_ALL (0x0E)      /**< X+Y+Z axis bits for commands. */
#define MLX90393_CONF1 (0x00)         /**< Gain */
#define MLX90393_CONF2 (0x01)         /**< Burst, comm mode */
#define MLX90393_CONF3 (0x02)         /**< Oversampling, filter, res. */
#define MLX90393_CONF4 (0x03)         /**< Sensitivty drift. */
#define MLX90393_GAIN_SHIFT (4)       /**< Left-shift for gain bits. */
#define MLX90393_HALL_CONF (0x0C)     /**< Hall plate spinning rate adj. */
#define MLX90393_STATUS_OK (0x00)     /**< OK value for status response. */
#define MLX90393_STATUS_SMMODE (0x08) /**< SM Mode status response. */
#define MLX90393_STATUS_RESET (0x01)  /**< Reset value for status response. */
#define MLX90393_STATUS_ERROR (0xFF)  /**< OK value for status response. */
#define MLX90393_STATUS_MASK (0xFC)   /**< Mask for status OK checks. */

/** Register map. */
enum {
  MLX90393_REG_SB = (0x10),  /**< Start burst mode. */
  MLX90393_REG_SW = (0x20),  /**< Start wakeup on change mode. */
  MLX90393_REG_SM = (0x30),  /**> Start single-meas mode. */
  MLX90393_REG_RM = (0x40),  /**> Read measurement. */
  MLX90393_REG_RR = (0x50),  /**< Read register. */
  MLX90393_REG_WR = (0x60),  /**< Write register. */
  MLX90393_REG_EX = (0x80),  /**> Exit moode. */
  MLX90393_REG_HR = (0xD0),  /**< Memory recall. */
  MLX90393_REG_HS = (0x70),  /**< Memory store. */
  MLX90393_REG_RT = (0xF0),  /**< Reset. */
  MLX90393_REG_NOP = (0x00), /**< NOP. */
};

/** Gain settings for CONF1 register. */
typedef enum mlx90393_gain {
  MLX90393_GAIN_5X = (0x00),
  MLX90393_GAIN_4X,
  MLX90393_GAIN_3X,
  MLX90393_GAIN_2_5X,
  MLX90393_GAIN_2X,
  MLX90393_GAIN_1_67X,
  MLX90393_GAIN_1_33X,
  MLX90393_GAIN_1X
} mlx90393_gain_t;

/** Resolution settings for CONF3 register. */
typedef enum mlx90393_resolution {
  MLX90393_RES_16,
  MLX90393_RES_17,
  MLX90393_RES_18,
  MLX90393_RES_19,
} mlx90393_resolution_t;

/** Axis designator. */
typedef enum mlx90393_axis {
  MLX90393_X,
  MLX90393_Y,
  MLX90393_Z
} mlx90393_axis_t;

/** Digital filter settings for CONF3 register. */
typedef enum mlx90393_filter {
  MLX90393_FILTER_0,
  MLX90393_FILTER_1,
  MLX90393_FILTER_2,
  MLX90393_FILTER_3,
  MLX90393_FILTER_4,
  MLX90393_FILTER_5,
  MLX90393_FILTER_6,
  MLX90393_FILTER_7,
} mlx90393_filter_t;

/** Oversampling settings for CONF3 register. */
typedef enum mlx90393_oversampling {
  MLX90393_OSR_0,
  MLX90393_OSR_1,
  MLX90393_OSR_2,
  MLX90393_OSR_3,
} mlx90393_oversampling_t;

struct mlx90393_data {
	const struct device *i2c_master;

	uint32_t reg_val;
};

struct mlx90393_config {
	const char *i2c_bus;
	uint16_t i2c_addr;
};

#endif /* ZEPHYR_DRIVERS_SENSOR_MLX90939 */